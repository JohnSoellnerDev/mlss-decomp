#include "global.h"
#include "ram_routines.h"

struct Owner
{
    u8 filler0[0x4];
    void (*state)(struct Owner *);
    struct Obj *obj;
    u8 filler0C[0x10 - 0x0C];
    u16 progress;
    s16 startY;
    s16 startX;
    u16 timer;
    u16 flag18;
};

struct OamEntry
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 unk6;
    u32 unk8;
    struct OamEntry *unkC;
    struct OamEntry *unk10;
};

struct OamList
{
    u8 filler0[0x2];
    u8 count;
    u8 filler3[0x38 - 0x3];
    struct OamEntry *head;
};

struct Pic
{
    s16 posX;
    s16 posY;
    s16 scaleX; /* 0x100 = 1.0 */
    s16 scaleY;
    u8 filler8[0x11 - 0x8];
    u8 flags11;
    u8 filler12[0x21 - 0x12];
    u8 kind;
    u8 filler22[0x34 - 0x22];
    struct OamEntry *anchor;
};

struct Obj
{
    u8 filler0[0x08];
    struct Pic *pic;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x38 - 0x1C];
    s32 unk38;
    s32 unk3C;
    u8 filler40[0x4C - 0x40];
    struct Owner *owner;
    u8 filler50[0x7C - 0x50];
    u16 speed;
    u8 ctrl;
    u8 filler7F[0x84 - 0x7F];
    s32 x0;
    s32 y0;
    s32 h;
    s32 unk90;
    s32 unk94;
    s32 unk98;
    s32 kind;
    u32 state;
    s32 unkA4;
    s32 unkA8;
    u16 phase;
    u8 fillerAE[0x11E - 0xAE];
    u16 bits;
};

struct SpriteBox
{
    u8 filler0[0x18];
    s32 z;      /* height above the ground, 8.8 fixed point */
    u8 filler1C[0x28 - 0x1C];
    void *body; /* the figure: signed_byte_e4 gives its head height in pixels */
};

/* The bro whose turn it is in battle */
struct ActiveObj
{
    u8 filler0[0x8];
    struct SpriteBox sprite;
    u8 filler34[0x48 - 0x34];
    void *action48;
    u8 filler4C[0xBA - 0x4C];
    s16 speed; /* vertical speed, positive going up */
};

/* gWorld->lock bits that freeze the battle's command blocks */
#define WORLD_LOCK_INPUT 0xE0

/* Obj.state of a command block (set by status_slots_update) */
#define CMD_BLOCK_STATE_DISABLED 1     /* greyed out: a jump bounces off it but can't pick it */
#define CMD_BLOCK_STATE_PASS_THROUGH 2 /* a jump goes through it */
#define CMD_BLOCK_TOP_Z 0x3000    /* height (8.8) a bro's head reaches when it hits a command block */

struct StatusObj
{
    u8 filler0[0x10];
    s16 index;
};

struct WorldEffect
{
    u8 filler0[0x4];
    void (*fn)(void);
    u8 *table;
    s32 kind;
    u16 unk10;
    u8 filler12[0x14 - 0x12];
    s16 unk14;
};

struct WorldObj
{
    u8 filler0[0x40];
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    u8 filler4C[0x7E - 0x4C];
    u8 ctrl;
    u8 filler7F[0xE4 - 0x7F];
    s8 *offsets;
};

struct Voice
{
    u8 filler0[0x8];
    s32 textId;
    s8 unkC;
    u8 unkD;
    u8 filler0E[0x12 - 0xE];
    u8 flags12;
};

struct World
{
    u8 filler0[0x07];
    u8 lock;
    u8 filler08[0x0B - 0x08];
    u8 flags0B;
    u8 filler0C[0x1C - 0x0C];
    u8 *flagPtr1C;
    u8 filler20[0x74 - 0x20];
    s32 unk74;
    u8 filler78[0x7C - 0x78];
    struct ActiveObj *active;
    struct Obj *fig1;
    struct Obj *fig2;
    u8 filler88[0x24C - 0x88];
    void *unk24C;
    void *slot250;
    u8 filler254[0x278 - 0x254];
    struct Obj *slot278;
    u8 filler27C[0x28C - 0x27C];
    struct WorldEffect *effect;
    struct Owner *timerObj;
    u8 filler294[0x29C - 0x294];
    struct Voice voice29C;
    u8 filler2B0[0x2BC - 0x2B0];
    u8 uploadState;
    u8 filler2BD[0x2FC - 0x2BD];
    struct Obj *slots[5];
    struct StatusObj *statusObj;
    u16 counter314;
    u16 scroll316;
    u8 filler318[0x340 - 0x318];
    u8 flag340;
    u8 list341;
};

struct SlotMarkers
{
    u8 filler0[0x2C2];
    u8 lo[3];
    u8 hi[3];
};

extern struct World *gWorld;
extern u8 *gWorkBuf;
extern struct OamList *gModelCtx;
extern u8 gOamTileTable[];
/* Per list-window kind (WorldEffect.kind & 0xF): the box's tile column, tile row, width and
   height in tiles. */
extern u8 gSlotTableX[];
extern u8 gSlotTableZ[];
extern u8 gGaugeTableC[];
extern u8 gGaugeTableD[];
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

void status_slots_update(struct Owner *owner);
void status_wait_slot(struct Owner *owner);
void obj_release(struct Obj *obj);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
s32 signed_byte_e4(void *part);
void set_slot_value(u32 id, s32 value);
void draw_world_effect_rows(void);
s32 parabola_height(s32 t, s32 v, s32 s, s32 h);
void place_world_object(s32 x, s32 y, s32 z, s32 mode, u32 kind);
struct Owner *task_create_list_4c(void (*fn)(struct Owner *), struct Obj *obj);
void state_motion_step(struct Owner *owner);
void scroll_text_step(struct Owner *owner);
void set_tile_slot(void *slot, u32 idx, u32 flag, u32 value, u32 p5, u32 p6, u32 p7);
void draw_slot_frame(void *slot, u32 idx, u32 rect, u32 kind);
void fill_slot(void *slot, u32 idx, u32 value);
void *slot_block_ptr(void *slot, u32 idx);
void voice_setup_params(void *voice, void *block, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
u32 next_text_id(void);
s32 measure_text(u32 font, void *text, u32 pad);
s32 track_run(void *voice);
u32 clamp_reflect_offsets(s32 *outA, s32 *outB, s32 lo, s32 hi, s32 v);
struct WorldEffect *task_create_list_48(void (*fn)(void), u32 arg);

struct PadState
{
    u8 filler0[0x42];
    u16 held;
};

struct KeyRepeat
{
    u8 filler0[0x45];
    u8 rate;
};

extern u8 gSys[];

void state_status_wait_slot(struct Owner *owner)
{
    struct World *world;
    struct Obj *slot;
    void (*next)(struct Owner *);
    s32 counter;
    s32 mask;
    s32 lockv;

    status_slots_update(owner);
    world = gWorld;
    counter = world->counter314 += 0x400;
    if ((s32)(counter << 16) > 0x1FFF0000)
    {
        world->counter314 = 0x2000;
        slot = gWorld->slots[gWorld->statusObj->index];
        if (slot->owner != 0)
        {
            owner->state = status_wait_slot;
        }
        else
        {
            world->lock &= 0x1F;
            owner->state = status_slots_update;
        }
    }
}

NONMATCH("asm/code_0810A2FC/state_status_release.s", void state_status_release(struct Owner *owner))
{
    struct World *world;
    struct Obj *slot;
    s32 counter;

    if (owner == 0)
    {
        while (owner == 0)
            ;
    }
    status_slots_update(owner);
    owner->state = state_status_release;
    world = gWorld;
    counter = world->counter314 - 0x400;
    world->counter314 = counter;
    if ((s32)(counter << 16) <= 0)
    {
        slot = world->slots[0];
        if (slot != 0)
        {
            slot->owner->state = 0;
            obj_release(slot);
            gWorld->slots[0] = 0;
        }
        slot = gWorld->slots[1];
        if (slot != 0)
        {
            slot->owner->state = 0;
            obj_release(slot);
            gWorld->slots[1] = 0;
        }
        slot = gWorld->slots[2];
        if (slot != 0)
        {
            slot->owner->state = 0;
            obj_release(slot);
            gWorld->slots[2] = 0;
        }
        slot = gWorld->slots[3];
        if (slot != 0)
        {
            slot->owner->state = 0;
            obj_release(slot);
            gWorld->slots[3] = 0;
        }
        slot = gWorld->slots[4];
        if (slot != 0)
        {
            slot->owner->state = 0;
            obj_release(slot);
            gWorld->slots[4] = 0;
        }
        if (gWorld->statusObj != 0)
        {
            owner->state = 0;
            gWorld->statusObj = 0;
        }
        ((struct KeyRepeat *)gSys)->rate = 4;
    }
}
END_NONMATCH

void sprite_set_action(struct Obj *obj, u32 action, u32 arg, u32 extra);

NONMATCH("asm/code_0810A2FC/status_slots_update.s", void status_slots_update(struct Owner *owner))
{
    struct World *world;
    struct ActiveObj *active;
    struct Obj *slot;
    struct Obj *fig;
    s32 activeX;
    s32 activeY;
    s32 value;
    s32 progress;
    s32 statusSlot;
    s32 pos;
    s32 target;
    s32 delta;
    s32 half;
    s32 i;
    s32 state;
    s32 anim;
    s32 radius;
    s32 idx;
    s32 part;
    s32 dx;
    s32 dy;
    s32 dyRaw;
    s32 offset;
    s32 step;
    s32 ry;
    s32 rx;
    s32 height;
    s32 timer;
    s32 limit;
    s32 sv;

    world = gWorld;
    active = world->active;
    activeX = ((struct Obj *)active)->z;
    activeY = *(s32 *)((u8 *)active + 0x1C);
    value = (s16)owner->timer;
    progress = (s16)world->counter314;
    statusSlot = (s16)world->statusObj->index;
    pos = (s16)world->scroll316;

    target = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(statusSlot << 8, value);
    delta = (target - pos) & 0xFF;
    half = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((value / 2) + 0x20, value);
    if (delta > 0x7F)
    {
        if (delta - 0x100 + half <= 0)
        {
            pos -= half;
        }
        else
        {
            pos = target;
        }
    }
    else if (delta < half)
    {
        pos = target;
    }
    else
    {
        pos += half;
    }
    pos &= 0xFF;
    gWorld->scroll316 = pos;
    if (value > 1)
    {
        ((struct KeyRepeat *)gSys)->rate = 8;
    }
    else
    {
        ((struct KeyRepeat *)gSys)->rate = 4;
    }

    for (i = 0; i < 4; i++)
    {
        slot = gWorld->slots[i];
        if (slot == 0)
        {
            continue;
        }
        state = 0;
        switch (slot->kind)
        {
        case 0:
        case 4:
            break;
        case 1:
            state = 1;
            fig = gWorld->fig1;
            if ((fig->ctrl & 6) == 2)
            {
                if ((gWorld->fig2->ctrl & 6) == 2 && gWorld->flag340 != 0 && (fig->bits & 0x300) == 0 && (gWorld->fig2->bits & 0x300) == 0)
                {
                    state = 0;
                }
            }
            break;
        case 2:
            if (gWorld->list341 == 0 || (((struct Obj *)active)->bits & 0x8000) != 0)
            {
                state = 1;
            }
            break;
        case 3:
            if ((*gWorld->flagPtr1C & 0x80) != 0)
            {
                state = 1;
            }
            else
            {
                fig = gWorld->fig1;
                if ((fig->ctrl & 6) == 2 && (fig->bits & 0x300) != 0)
                {
                    state = 1;
                }
                else
                {
                    fig = gWorld->fig2;
                    if ((fig->ctrl & 6) == 2 && (fig->bits & 0x300) != 0)
                    {
                        state = 1;
                    }
                }
            }
            break;
        case 5:
            gWorld->statusObj->index = 0;
            statusSlot = 0;
            if (*(s32 *)((u8 *)active + 0x34) != 0)
            {
                state = 2;
            }
            break;
        }
        slot->state = state;
        if (i == (gWorld->lock >> 5) - 1)
        {
            continue;
        }
        anim = slot->kind;
        if (anim == 5)
        {
            anim = 4;
        }
        if (state == 1)
        {
            anim += 5;
        }
        if (slot->pic->kind != anim)
        {
            sprite_set_action(slot, anim, 0, 0);
        }

        if (value > 1)
        {
            sv = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(i << 8, value);
            idx = (sv - pos) & 0xFF;
            part = gSineTable[idx];
            if (part < 0)
            {
                part += 0x3F;
            }
            part = part >> 6;
            part = progress * part;
            if (part < 0)
            {
                part += 0xFF;
            }
            dx = part >> 8;
            part = gCosineTable[idx];
            if (part < 0)
            {
                part += 0x3F;
            }
            part = part >> 6;
            part = progress * part;
            if (part < 0)
            {
                part += 0xFF;
            }
            dyRaw = part >> 8;
            dy = dyRaw - 0x2000;
            offset = dy;
            if (dy < 0)
            {
                offset = dyRaw - 0x1FF1;
            }
            step = 0xC80 - (offset >> 4);
            radius = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC80000, step);
            rx = radius * dx;
            if (rx < 0)
            {
                rx += 0xFFF;
            }
            slot->x = activeX + (rx >> 12);
            ry = (radius * 25) << 10;
            if (ry < 0)
            {
                ry += 0xFFF;
            }
            slot->y = activeY + (ry >> 12) - 0x6400;
            slot->x0 = activeX + dx;
            slot->y0 = activeX + dy;
        }
        else
        {
            offset = progress - 0x2000;
            if (offset < 0)
            {
                offset = progress - 0x1FF1;
            }
            step = 0xC80 - (offset >> 4);
            radius = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC80000, step);
            slot->x = activeX;
            ry = (radius * 25) << 10;
            if (ry < 0)
            {
                ry += 0xFFF;
            }
            slot->y = activeY + (ry >> 12) - 0x6400;
        }

        if (i == statusSlot && state == 0)
        {
            timer = (s16)owner->flag18;
            if (timer != 0)
            {
                slot->z += timer;
                owner->flag18 -= 0x80;
                part = gSineTable[1];
                if (part < 0)
                {
                    part += 0x3F;
                }
                limit = ((part >> 6) << 1) + 0x3000;
                if (limit >= slot->z)
                {
                    slot->phase = state;
                    owner->flag18 = state;
                }
            }
            else
            {
                idx = (((s32)slot->phase << 2) + 1) & 0xFF;
                slot->phase++;
                part = gSineTable[idx];
                if (part < 0)
                {
                    part += 0x3F;
                }
                height = ((part >> 6) << 1) + 0x3000;
                slot->z = height;
            }
        }
        else
        {
            slot->z = 0x3000;
        }

        if (state == 2)
        {
            slot->pic->flags11 = (slot->pic->flags11 & -13) | 4;
            slot->pic->scaleX = 0x80;
            slot->pic->scaleY = 0x80;
        }
        else
        {
            slot->pic->flags11 &= -13;
            slot->pic->scaleX = radius / 0x10;
            slot->pic->scaleY = radius / 0x10;
        }
        if (i != statusSlot)
        {
            slot->pic->flags11 = (slot->pic->flags11 & -13) | 4;
        }
    }
}
END_NONMATCH

NONMATCH("asm/code_0810A2FC/oam_link_slot_entries.s", void oam_link_slot_entries(void))
{
    s32 attrHi = 0xFFFFD800;
    s32 attrBit = 0x4000;
    struct OamEntry *cursor;
    struct OamEntry *nextEntry;
    struct OamEntry *prevEntry;
    struct OamEntry *anchorNext;
    struct OamEntry *anchor;
    struct Obj *slot;
    struct Pic *pic;
    s32 i;
    s32 kind;
    s32 posX;
    s32 posY;
    s32 attrX;
    s32 tile;

    cursor = gModelCtx->head;
    for (i = 0; i < 5; i++)
    {
        slot = gWorld->slots[i];
        if (slot != 0)
        {
            pic = slot->pic;
            kind = pic->kind;
            if (kind == 5)
            {
                kind = 4;
            }
            if (kind > 4)
            {
                kind -= 5;
            }
            posX = pic->posX;
            posY = pic->posY;
            tile = gOamTileTable[kind] + 0x5D;
            if (kind == 3)
            {
                attrX = (posX - 8) & 0x1FF;
                cursor->attr0 = ((posY - 4) & 0xFF) | 0x4000;
            }
            else
            {
                cursor->attr0 = ((posY - 4) & 0xFF) | 0x4000;
                attrX = ((posX - 0x10) & 0x1FF) | attrBit;
            }
            cursor->attr1 = attrX;
            cursor->attr2 = tile | attrHi;
            nextEntry = cursor->unk10;
            prevEntry = cursor->unkC;
            anchor = slot->pic->anchor;
            anchorNext = anchor->unkC;
            cursor->unk6 = 0;
            cursor->unk8 = anchorNext->unk8;
            nextEntry->unkC = prevEntry;
            prevEntry->unk10 = nextEntry;
            cursor->unkC = anchorNext;
            cursor->unk10 = anchor;
            anchorNext->unk10 = cursor;
            anchor->unkC = cursor;
            gModelCtx->count++;
            cursor = nextEntry;
        }
    }
    gModelCtx->head = cursor;
}
END_NONMATCH

NONMATCH("asm/code_0810A2FC/obj_motion_start.s", struct Owner *obj_motion_start(struct Obj *obj, s32 x, s32 y, s32 height, s32 kind))
{
    struct Owner *task;
    s32 t;
    s32 clampX;
    s32 clampY;
    s32 (*div)(s32, s32);

    task = obj->owner;
    if (task == 0)
    {
        task = task_create_list_4c(state_motion_step, obj);
        obj->owner = task;
    }
    clamp_reflect_offsets(&clampX, &clampY, obj->z, height, kind);
    obj->unkA4 = clampX;
    obj->unkA8 = clampY;
    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    t = clampX;
    if (clampX < 0)
    {
        t += 0x1F;
    }
    t = (t >> 5) * clampX;
    if (t < 0)
    {
        t += 0x3F;
    }
    obj->speed = div(0x2CC000, t >> 6);
    obj->x0 = x;
    obj->y0 = y;
    obj->h = height;
    obj->unk98 = kind;
    obj->unk94 = obj->pic->scaleX;
    t = obj->unk38;
    if (t < 0)
    {
        t += 0xFF;
    }
    task->startY = t >> 8;
    t = obj->unk3C;
    if (t < 0)
    {
        t += 0xFF;
    }
    task->startX = t >> 8;
    task->progress = 0;
    return task;
}
END_NONMATCH

/* Task step of an object's jump (started by obj_motion_start): adds the object's speed to the
 * progress (0x4000 = landed). Until then the object moves in a straight line from the task's start
 * spot (in pixels; startY holds the x) to (x0, y0), at the height parabola_height gives, and its
 * sprite scales from the start scale (unk94) to 1.0. At the end it is put on (x0, y0) at height h
 * with scale 1.0, and the task ends. */
void state_motion_step(struct Owner *task)
{
    struct Obj *obj;
    struct Pic *pic;
    s32 progress;
    s32 t;
    s32 rounded;
    s32 frac;
    s32 delta;
    s32 fromX;
    s32 fromY;
    s32 toX;
    s32 toY;
    s32 startScale;

    obj = task->obj;
    progress = obj->speed + task->progress;
    task->progress = progress;
    if ((s32)(progress << 16) > 0x40000000)
    {
        obj->x = obj->x0;
        obj->y = obj->y0;
        obj->z = obj->h;
        obj->pic->scaleX = 0x100;
        obj->pic->scaleY = 0x100;
        task->state = 0;
        obj->owner = 0;
    }
    else
    {
        t = (s16)task->progress;
        fromX = task->startY << 8;
        fromY = task->startX << 8;
        toX = obj->x0;
        toY = obj->y0;
        /* the part of the way done, 0x100 = all */
        rounded = t;
        if (rounded < 0)
        {
            rounded += 0x3F;
        }
        frac = rounded >> 6;
        delta = frac * (toX - fromX);
        if (delta < 0)
        {
            delta += 0xFF;
        }
        obj->x = fromX + (delta >> 8);
        delta = frac * (toY - fromY);
        if (delta < 0)
        {
            delta += 0xFF;
        }
        obj->y = fromY + (delta >> 8);
        obj->z = parabola_height(t, obj->unkA4, obj->unkA8, obj->unk98);
        startScale = obj->unk94;
        pic = obj->pic;
        delta = frac * (0x100 - startScale);
        if (delta < 0)
        {
            delta += 0xFF;
        }
        pic->scaleY = pic->scaleX = startScale + (delta >> 8);
    }
}

/* Moves the status screen's cursor (low 7 bits of `cursor`) one place left or right among `count`
   entries, wrapping at both ends, while input isn't locked, the selected slot has no owner and the
   active object isn't busy. Each step scrolls the view by one and plays sound 0x78. The result is
   stored in the status object and returned. */
s32 status_cursor_step(u32 cursor, u32 count)
{
    s32 sel;
    s32 next;

    sel = cursor & 0x7F;
    if ((gWorld->lock & 0xE0) == 0)
    {
        if (gWorld->slots[sel] == NULL || gWorld->slots[sel]->owner == 0)
        {
            if (gWorld->active->action48 == 0)
            {
                if ((((struct PadState *)gSys)->held & DPAD_LEFT) != 0 && count != 1)
                {
                    next = count - 1;
                    if (sel > 0)
                    {
                        next = sel - 1;
                    }
                    sel = next;
                    gWorld->scroll316 -= 1;
                    set_slot_value(0x78, -1);
                }
                if ((((struct PadState *)gSys)->held & DPAD_RIGHT) != 0 && count != 1)
                {
                    next = count - 1;
                    sel = (sel < next) ? sel + 1 : 0;
                    gWorld->scroll316 += 1;
                    set_slot_value(0x78, -1);
                }
            }
        }
    }
    gWorld->statusObj->index = sel;
    return sel;
}

/*
 * Battle: checks whether the jumping bro's head hits the command block in `slot`. While input isn't
 * locked and the block is solid, a rising bro that reaches the block's height is held there and
 * bounces back down at half speed. Returns 1 when that picks the block (plays sound 0x77), -1 when
 * it is a kind-4 or kind-5 block, and 0 when nothing is picked (no hit, or a disabled block).
 */
s32 slot_marker_step(u32 slot)
{
    struct World *world;
    struct Obj *block;
    struct ActiveObj *bro;
    struct SpriteBox *sprite;
    s16 *speedZ;
    u32 *blockState;
    s32 headHeight;
    s32 topZ;
    s32 speed;

    world = gWorld;
    block = world->slots[slot];
    if ((world->lock & WORLD_LOCK_INPUT) != 0)
    {
        return 0;
    }
    if (block == NULL)
    {
        return 0;
    }
    if (block->owner != NULL)
    {
        return 0;
    }
    blockState = &block->state;
    if (*blockState == CMD_BLOCK_STATE_PASS_THROUGH)
    {
        return 0;
    }
    bro = world->active;
    sprite = &bro->sprite;
    speedZ = &bro->speed;
    if (*speedZ < 0)
    {
        return 0;
    }
    headHeight = signed_byte_e4(sprite->body);
    topZ = CMD_BLOCK_TOP_Z - (headHeight << 8);
    if (sprite->z < topZ)
    {
        return 0;
    }
    sprite->z = topZ;
    speed = -*speedZ;
    *speedZ = (s32)(speed + ((u32)speed >> 31)) >> 1;
    if (*blockState == CMD_BLOCK_STATE_DISABLED)
    {
        return 0;
    }
    if (block->kind == 4)
    {
        return -1;
    }
    if (block->kind > 4)
    {
        if (block->kind == 5)
        {
            return -1;
        }
    }
    set_slot_value(0x77, -1);
    return 1;
}

ASM_FUNC("asm/code_0810A2FC/draw_number_rotated.s", void draw_number_rotated(void))

ASM_FUNC("asm/code_0810A2FC/battle_display_draw_number.s", void battle_display_draw_number(void))

ASM_FUNC("asm/code_0810A2FC/unknown_thumb_0810b238.s", void unknown_thumb_0810b238(void))

void release_sprite_state(struct Owner *owner);
struct Obj *obj_spawn(s32 part, u32 kind, s32 x, s32 y, s32 z, void *parent, s32 flag);
void sprite_set_action(struct Obj *obj, u32 action, u32 arg, u32 extra);
void sprite_entry_update(struct Pic *pic);
void start_motion(struct Obj *obj);

NONMATCH("asm/code_0810A2FC/place_world_object.s", void place_world_object(s32 x, s32 y, s32 z, s32 mode, u32 kind))
{
    struct World *world;
    struct Obj *obj;
    struct Pic *sub;
    s32 prio;
    s32 handle;
    s32 slotOfs;
    s32 bits;
    s32 *px;
    s32 *py;
    s32 *ph;

    prio = mode;
    world = gWorld;
    slotOfs = 0x278;
    obj = *(struct Obj **)((u8 *)world + slotOfs);
    if ((s32)world->active == world->unk74)
    {
        prio += 1;
    }
    if (obj == 0)
    {
        obj = obj_spawn(0, (u16)kind, x, y, z, 0, 1);
        *((u8 *)obj + 0x79) |= 1;
        sprite_set_action(obj, prio, 0, 0);
        *((u8 *)obj + 0x74) = 0x80;
        *(struct Obj **)((u8 *)gWorld + slotOfs) = obj;
    }
    else
    {
        if ((*((u8 *)obj->pic + 0x12) & 0x20) != 0)
        {
            sprite_entry_update(obj->pic);
        }
        sub = obj->pic;
        bits = *((u8 *)sub + 0x1F);
        bits = (bits & 0x3F) | 0x80;
        bits = (bits & -0x31) | 0x20;
        bits = (bits & -0xD) | 8;
        bits = ((bits & -4) | 2);
        *((u8 *)sub + 0x1F) = bits;
        if (*((u8 *)obj->pic + 0x21) != prio)
        {
            sprite_set_action(obj, prio, 0, 0);
        }
    }
    x <<= 8;
    y <<= 8;
    z <<= 8;
    px = &obj->x0;
    py = &obj->y0;
    ph = &obj->h;
    if (*px != x || *py != y || *ph != z)
    {
        *px = x;
        *py = y;
        *ph = z;
        *((u8 *)obj + 0x79) |= 0x20;
        obj->speed = 0x1000;
        handle = *(s32 *)((u8 *)obj + 0x80);
        start_motion(obj);
        if (handle == 0)
        {
            task_create_list_4c(release_sprite_state, obj);
        }
    }
}
END_NONMATCH

/* Puts the target cursor (sprite 0x7001, see place_world_object) on a figure, playing action
 * `action`. Actions above 16 put it 0x20 px right of the figure at height 4; otherwise it goes to
 * the figure's cursor offset (offsets[4], offsets[5]), at height +0x10 for a side-3 figure (ctrl
 * bits 1-2) and offsets[6] (signed_byte_e4) for the others. Coordinates are in pixels. */
void refresh_world_object(struct WorldObj *fig, s32 action)
{
    s32 x;
    s32 y;
    s32 z;
    s32 v;
    s32 w;
    s32 base;

    if (action > 16)
    {
        v = fig->srcX;
        if (v < 0)
        {
            v += 0xFF;
        }
        x = (v >> 8) + 0x20;
        v = fig->srcY;
        if (v < 0)
        {
            v += 0xFF;
        }
        y = v >> 8;
        z = 4;
    }
    else if ((fig->ctrl & 6) == 6)
    {
        v = fig->srcX;
        if (v < 0)
        {
            v += 0xFF;
        }
        x = (v >> 8) + fig->offsets[4];
        w = fig->srcY;
        if (w < 0)
        {
            w += 0xFF;
        }
        y = (w >> 8) + fig->offsets[5];
        v = fig->srcZ;
        if (v < 0)
        {
            v += 0xFF;
        }
        z = (v >> 8) + 0x10;
    }
    else
    {
        v = fig->srcX;
        if (v < 0)
        {
            v += 0xFF;
        }
        x = (v >> 8) + fig->offsets[4];
        w = fig->srcY;
        if (w < 0)
        {
            w += 0xFF;
        }
        y = (w >> 8) + fig->offsets[5];
        w = signed_byte_e4(fig);
        v = fig->srcZ;
        if (v < 0)
        {
            v += 0xFF;
        }
        z = (v >> 8) + w;
    }
    place_world_object(x, y, z, action, 0x7001);
}

NONMATCH("asm/code_0810A2FC/update_slot_marker.s", void update_slot_marker(s32 value, s32 slot, s32 flag, s32 mode_base))
{
    struct World **pw;
    s32 pos = value;
    s32 limit;
    s32 hi;
    u16 mode;
    s32 x;
    s32 z;

    switch (slot)
    {
    case 0:
        limit = *((u8 *)gWorld + 0x33F);
        break;
    case 1:
        limit = *((u8 *)gWorld + 0x340);
        break;
    case 2:
        limit = *((u8 *)gWorld + 0x341);
        break;
    default:
        break;
    }
    pw = &gWorld;
    limit = (s16)limit;
    if (limit > 5)
    {
        hi = (s8)((struct SlotMarkers *)*pw)->hi[slot];
        pos = pos - (s8)((struct SlotMarkers *)*pw)->lo[slot];
        if (pos < 0)
        {
            pos += limit;
        }
        if (pos - 1 > hi)
        {
            pos = hi - 1;
        }
        if ((u32)pos > 4)
        {
            pos = hi;
        }
    }
    ((struct SlotMarkers *)*pw)->hi[slot] = pos;
    if (flag != 0)
    {
        mode = mode_base + 9;
    }
    else
    {
        mode = 0xB;
    }
    x = (gSlotTableX[slot] << 3) + ((s16)obj_last_x(gWorld->unk24C) + 4);
    z = obj_last_y(gWorld->unk24C);
    z = (s16)z - (pos * 0xB + (gSlotTableZ[slot] << 3)) - 0x18;
    place_world_object(x, 0, z, (s16)mode, 0x7001);
}
END_NONMATCH

/* A 2-frame OBJ tile animation: each frame is 8 tiles (0x100 bytes) of a 16-tile block in gWorkBuf,
 * copied to the same OBJ VRAM tiles. gWorld->uploadState counts the frames down. */
#define ANIM_TILES_SRC      0x4E70     /* gWorkBuf offset of the animation's tile blocks */
#define ANIM_TILES_VRAM     0x060113A0 /* OBJ VRAM the current frame is copied to */
#define ANIM_FRAME_BYTES    0x100      /* 8 tiles of 4bpp */
#define ANIM_PERIOD         0x26       /* frames per animation cycle */
#define ANIM_SECOND_HALF    0x13       /* countdown value that shows the second frame */

/* One frame of the 2-frame OBJ tile animation of tile block `block`: when the countdown runs out it
 * copies the block's first 8 tiles to OBJ VRAM and restarts at ANIM_PERIOD; halfway it copies the
 * other 8 tiles. */
void tile_upload_step(u16 block)
{
    struct World *world;
    u8 countdown;

    world = gWorld;
    countdown = world->uploadState;
    if (countdown == 0)
    {
        dma_copy_or_queue(gWorkBuf + ((block << 9) + ANIM_TILES_SRC), (void *)ANIM_TILES_VRAM, ANIM_FRAME_BYTES, 0x20, 0);
        gWorld->uploadState = ANIM_PERIOD;
    }
    else if (countdown == ANIM_SECOND_HALF)
    {
        dma_copy_or_queue(gWorkBuf + ((((block << 4) + 8) << 5) + ANIM_TILES_SRC), (void *)ANIM_TILES_VRAM, ANIM_FRAME_BYTES, 0x20, 0);
    }
    gWorld->uploadState = gWorld->uploadState - 1;
}

/* Opens the voice message window: sets up tile slot 4 as a framed 0x11x0x11 text box, clears its
   tiles, points the world's voice at it with the next text, notes whether that text fits the window
   (narrower than 0x68 px: no scrolling), renders the first line, clears world flag 0x40 and hands
   over to scroll_text_step after a 90 frame wait. */
void start_voice_message(struct Owner *owner)
{
    void *tiles;
    struct Voice *voice;
    u32 text;
    s16 more;

    owner->startY = 0x40;
    owner->timer = 0;
    set_tile_slot(gWorld->slot250, 4, 0, 0xf, 0x11, 0x11, 2);
    draw_slot_frame(gWorld->slot250, 5, 4, 4);
    fill_slot(gWorld->slot250, 4, 0x90);
    tiles = slot_block_ptr(gWorld->slot250, 4);
    voice = &gWorld->voice29C;
    voice_setup_params(voice, tiles, 0x11, 0, 0, 0, 1, 2, 1, 3);
    text = next_text_id();
    owner->flag18 = measure_text(3, (void *)text, 0) < 0x68;
    /* gMemRoutines + 0: the IWRAM fill routine (value, dst, bytes) */
    ((void (*)(u32, void *, u32))gMemRoutines)(0, tiles, 0x440);
    voice->textId = text;
    voice->unkC = 0;
    voice->unkD = 0xd;
    *((u8 *)voice + 0x12) |= 0x20; /* flags12 */
    do
    {
        if (voice->unkC < 0)
            break;
        more = track_run(voice);
    } while (more != 0);
    *(s8 *)&gWorld->flags0B &= ~0x40;
    owner->state = scroll_text_step;
    owner->timer = 90;
}

NONMATCH("asm/code_0810A2FC/update_voice_message.s", void update_voice_message(struct Owner *owner))
{
    struct Owner *timer;
    struct Voice *voice;
    u8 *block;
    s32 zero;
    u32 i;
    s16 sVar;
    u16 timerVal;
    s32 *p;

    timer = gWorld->timerObj;
    timerVal = timer->startY;
    if ((s16)timer->startY > 0x7FF)
    {
        timerVal -= 0x800;
        zero = 0;
        timer->startY = timerVal;
        block = slot_block_ptr(gWorld->slot250, 4);
        MEM_ROUTINE(void (*)(void *, void *, u32), iwram_copy)(block + 0x20, block, 0x420);
        for (i = 0; i < 8; i++)
        {
            *(s32 *)(block + 0x200 + i * 4) = zero;
        }
        p = (s32 *)(block + 0x420);
        for (i = 0; i < 8; i++)
        {
            p[i] = zero;
        }
        voice = &gWorld->voice29C;
        if (*(u8 *)voice->textId != 0)
        {
            voice->unkC -= 8;
            do
            {
                if (voice->unkC < 0)
                {
                    break;
                }
                sVar = track_run(voice);
            } while (sVar != 0);
            owner->startY = 0;
        }
        else
        {
            owner->startY++;
            if ((s16)owner->startY > 10)
            {
                voice->textId = next_text_id();
            }
        }
    }
}
END_NONMATCH

void set_world_effect(s32 kind)
{
    struct World *world;
    struct WorldEffect *obj;

    obj = gWorld->effect;
    if (obj != 0)
    {
        obj->fn = draw_world_effect_rows;
    }
    else
    {
        obj = task_create_list_48(draw_world_effect_rows, 0);
        obj->kind = -1;
    }
    world = gWorld;
    world->effect = obj;
    obj->unk14 = 0;
    obj->unk10 = 0;
    if ((obj->kind & 0xF) != (kind & 0xF))
    {
        switch (kind)
        {
        case 0:
            obj->table = (u8 *)world + 0x320;
            break;
        case 1:
            obj->table = (u8 *)world + 0x323;
            break;
        case 2:
            obj->table = (u8 *)world + 0x32C;
            break;
        }
        obj->kind = kind | 0x80000000;
    }
}

/* Sets up the display for the world effect's list window: scrolls BG2 vertically by the effect's
   unk14 in pixels (8.8 fixed point; values below 0x40 count as 8), shows every layer inside the
   windows but hides BG2 outside them, and puts window 1 on the inside of the list box of the
   effect's kind (gSlotTableX/Z: the box's tile column and row, gGaugeTableC/D: its width and
   height in tiles, one tile of frame on each side). */
void update_world_effect_windows(void)
{
    struct WorldEffect *effect;
    vu16 *win;
    u32 kind;
    s32 scrollY;
    s32 offset;

    effect = gWorld->effect;
    kind = effect->kind & 0xF;
    if (effect->unk14 <= 0x3F)
    {
        scrollY = 8;
    }
    else
    {
        offset = effect->unk14;
        if (offset < 0)
        {
            offset += 0xFF;
        }
        scrollY = offset >> 8;
    }
    REG_BG2HOFS = 0;
    REG_BG2VOFS = scrollY - 3;
    REG_WININ = 0x3F3F;
    REG_WINOUT = 0x3F3B;
    win = (vu16 *)REG_ADDR_WIN1H;
    *win = ((gSlotTableX[kind] + gGaugeTableC[kind] - 1) << 3) | ((gSlotTableX[kind] + 1) << 11);
    win += 2;
    *win = ((gSlotTableZ[kind] + gGaugeTableD[kind] - 1) << 3) | ((gSlotTableZ[kind] + 1) << 11);
}
