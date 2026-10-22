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
    s16 a4;
    s16 a6;
    u8 filler8[0x21 - 0x8];
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
    u8 filler7E[0x84 - 0x7E];
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
};

struct SpriteBox
{
    u8 filler0[0x18];
    s32 x18;
    u8 filler1C[0x28 - 0x1C];
    void *part;
};

struct ActiveObj
{
    u8 filler0[0x8];
    struct SpriteBox sprite;
    u8 filler34[0x48 - 0x34];
    void *action48;
    u8 filler4C[0xBA - 0x4C];
    s16 speed;
};

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
    u8 filler0C[0x74 - 0x0C];
    s32 unk74;
    u8 filler78[0x7C - 0x78];
    struct ActiveObj *active;
    u8 filler80[0x24C - 0x80];
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
};

extern struct World *gWorld;
extern u8 *gWorkBuf;
extern struct OamList *gModelCtx;
extern u8 gOamTileTable[];
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

NONMATCH("asm/code_0810A2FC/state_status_wait_slot.s", void state_status_wait_slot(struct Owner *owner))
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
            next = status_wait_slot;
        }
        else
        {
            lockv = world->lock;
            mask = 0x1F;
            lockv = mask & lockv;
            world->lock = lockv;
            next = status_slots_update;
        }
        owner->state = next;
    }
}
END_NONMATCH

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

ASM_FUNC("asm/code_0810A2FC/status_slots_update.s", void status_slots_update(struct Owner *owner))

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
    obj->unk94 = obj->pic->a4;
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

void state_motion_step(struct Owner *owner)
{
    struct Obj *obj;
    struct Pic *pic;
    s32 sum;
    s32 t;
    s32 frac;
    s32 ease;
    s32 dx;
    s32 sy;
    s32 sx;
    s32 x0;
    s32 y0;
    s32 v;

    obj = owner->obj;
    sum = obj->speed + owner->progress;
    owner->progress = sum;
    if ((s32)(sum << 16) > 0x40000000)
    {
        obj->x = obj->x0;
        obj->y = obj->y0;
        obj->z = obj->h;
        obj->pic->a4 = 0x100;
        obj->pic->a6 = 0x100;
        owner->state = 0;
        obj->owner = 0;
    }
    else
    {
        t = (s16)owner->progress;
        sy = owner->startY << 8;
        sx = owner->startX << 8;
        x0 = obj->x0;
        y0 = obj->y0;
        frac = t;
        if (frac < 0)
        {
            frac += 0x3F;
        }
        ease = frac >> 6;
        dx = ease * (x0 - sy);
        if (dx < 0)
        {
            dx += 0xFF;
        }
        obj->x = sy + (dx >> 8);
        dx = ease * (y0 - sx);
        if (dx < 0)
        {
            dx += 0xFF;
        }
        obj->y = sx + (dx >> 8);
        obj->z = parabola_height(t, obj->unkA4, obj->unkA8, obj->unk98);
        v = obj->unk94;
        pic = obj->pic;
        dx = ease * (0x100 - v);
        if (dx < 0)
        {
            dx += 0xFF;
        }
        pic->a6 = pic->a4 = v + (dx >> 8);
    }
}

s32 status_cursor_step(u32 cursor, u32 count)
{
    s32 sel;
    s32 back;

    sel = cursor & 0x7F;
    if ((gWorld->lock & 0xE0) == 0)
    {
        if (gWorld->slots[sel] == 0 || gWorld->slots[sel]->owner == 0)
        {
            if (gWorld->active->action48 == 0)
            {
                if ((((struct PadState *)gSys)->held & 0x20) != 0 && count != 1)
                {
                    back = count - 1;
                    if (sel > 0)
                    {
                        back = sel - 1;
                    }
                    sel = back;
                    gWorld->scroll316 -= 1;
                    set_slot_value(0x78, -1);
                }
                if ((((struct PadState *)gSys)->held & 0x10) != 0 && count != 1)
                {
                    back = count - 1;
                    sel = (sel < back) ? sel + 1 : 0;
                    gWorld->scroll316 += 1;
                    set_slot_value(0x78, -1);
                }
            }
        }
    }
    gWorld->statusObj->index = sel;
    return sel;
}

s32 slot_marker_step(u32 slot)
{
    struct World *world;
    struct Obj *marker;
    struct ActiveObj *active;
    struct SpriteBox *sprite;
    s16 *speedp;
    u32 *state;
    s32 scale;
    s32 limit;
    s32 speed;

    world = gWorld;
    marker = world->slots[slot];
    if ((world->lock & 0xE0) != 0)
    {
        return 0;
    }
    if (marker == 0)
    {
        return 0;
    }
    if (marker->owner != 0)
    {
        return 0;
    }
    state = &marker->state;
    if (*state == 2)
    {
        return 0;
    }
    active = world->active;
    sprite = &active->sprite;
    speedp = &active->speed;
    if (*speedp < 0)
    {
        return 0;
    }
    scale = signed_byte_e4(sprite->part);
    limit = 0x3000 - (scale << 8);
    if (sprite->x18 < limit)
    {
        return 0;
    }
    sprite->x18 = limit;
    speed = -*speedp;
    *speedp = (s32)(speed + ((u32)speed >> 31)) >> 1;
    if (*state == 1)
    {
        return 0;
    }
    if (marker->kind == 4)
    {
        return -1;
    }
    if (marker->kind > 4)
    {
        if (marker->kind == 5)
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

    world = gWorld;
    slotOfs = 0x278;
    prio = mode;
    if ((s32)world->active != world->unk74)
    {
        prio += 1;
    }
    obj = *(struct Obj **)((u8 *)world + slotOfs);
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
        *((u8 *)sub + 0x1F) = (((((((bits & 0x3F) | 0x80) & -0x31) | 0x20) & -0xD) | 8) & -4) | 2;
        if (*((u8 *)obj->pic + 0x21) != prio)
        {
            sprite_set_action(obj, prio, 0, 0);
        }
    }
    x <<= 8;
    y <<= 8;
    z <<= 8;
    if (obj->x0 != x || obj->y0 != y || obj->h != z)
    {
        obj->x0 = x;
        obj->y0 = y;
        obj->h = z;
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

void refresh_world_object(struct WorldObj *obj, s32 mode)
{
    s32 x;
    s32 y;
    s32 z;
    s32 t;
    s32 u;
    s32 base;

    if (mode > 16)
    {
        t = obj->srcX;
        if (t < 0)
        {
            t += 0xFF;
        }
        x = (t >> 8) + 0x20;
        t = obj->srcY;
        if (t < 0)
        {
            t += 0xFF;
        }
        y = t >> 8;
        z = 4;
    }
    else if ((obj->ctrl & 6) == 6)
    {
        t = obj->srcX;
        if (t < 0)
        {
            t += 0xFF;
        }
        x = (t >> 8) + obj->offsets[4];
        u = obj->srcY;
        if (u < 0)
        {
            u += 0xFF;
        }
        y = (u >> 8) + obj->offsets[5];
        t = obj->srcZ;
        if (t < 0)
        {
            t += 0xFF;
        }
        z = (t >> 8) + 0x10;
    }
    else
    {
        t = obj->srcX;
        if (t < 0)
        {
            t += 0xFF;
        }
        x = (t >> 8) + obj->offsets[4];
        u = obj->srcY;
        if (u < 0)
        {
            u += 0xFF;
        }
        y = (u >> 8) + obj->offsets[5];
        u = signed_byte_e4(obj);
        t = obj->srcZ;
        if (t < 0)
        {
            t += 0xFF;
        }
        z = (t >> 8) + u;
    }
    place_world_object(x, y, z, mode, 0x7001);
}

NONMATCH("asm/code_0810A2FC/update_slot_marker.s", void update_slot_marker(s32 value, s32 slot, s32 flag, s32 mode_base))
{
    s32 kc = 0x2C5;
    struct World **pw;
    s32 pos;
    s32 limit;
    s32 hi;
    u16 mode;
    s32 x;
    s32 z;

    pos = value;
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
        hi = (s8)*((u8 *)*pw + 0x2C5 + slot);
        pos = pos - (s8)*((u8 *)*pw + 0x2C2 + slot);
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
    *((u8 *)*pw + kc + slot) = pos;
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

void tile_upload_step(u16 block)
{
    struct World *world;
    u8 state;

    world = gWorld;
    state = world->uploadState;
    if (state == 0)
    {
        dma_copy_or_queue(gWorkBuf + ((block << 9) + 0x4E70), (void *)0x060113A0, 0x100, 0x20, 0);
        gWorld->uploadState = 0x26;
    }
    else if (state == 0x13)
    {
        dma_copy_or_queue(gWorkBuf + ((((block << 4) + 8) << 5) + 0x4E70), (void *)0x060113A0, 0x100, 0x20, 0);
    }
    gWorld->uploadState = gWorld->uploadState - 1;
}

void start_voice_message(struct Owner *owner)
{
    void *block;
    struct Voice *voice;
    u32 text;
    s16 sVar;

    owner->startY = 0x40;
    owner->timer = 0;
    set_tile_slot(gWorld->slot250, 4, 0, 0xf, 0x11, 0x11, 2);
    draw_slot_frame(gWorld->slot250, 5, 4, 4);
    fill_slot(gWorld->slot250, 4, 0x90);
    block = slot_block_ptr(gWorld->slot250, 4);
    voice = &gWorld->voice29C;
    voice_setup_params(voice, block, 0x11, 0, 0, 0, 1, 2, 1, 3);
    text = next_text_id();
    owner->flag18 = measure_text(3, (void *)text, 0) < 0x68;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, block, 0x440);
    voice->textId = text;
    voice->unkC = 0;
    voice->unkD = 0xd;
    *((u8 *)voice + 0x12) |= 0x20;
    do
    {
        if (voice->unkC < 0)
            break;
        sVar = track_run(voice);
    } while (sVar != 0);
    *(s8 *)&gWorld->flags0B &= -0x41;
    owner->state = scroll_text_step;
    owner->timer = 0x5a;
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
        for (i = 0; i < 8; i++)
        {
            *(s32 *)(block + 0x420 + i * 4) = zero;
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

void update_world_effect_windows(void)
{
    struct WorldEffect *effect;
    vu16 *win;
    u32 kind;
    s32 scale;
    s32 t;
    u32 x;
    u32 y;

    effect = gWorld->effect;
    kind = effect->kind & 0xF;
    if (effect->unk14 <= 0x3F)
    {
        scale = 8;
    }
    else
    {
        t = effect->unk14;
        if (t < 0)
        {
            t += 0xFF;
        }
        scale = t >> 8;
    }
    REG_BG2HOFS = 0;
    REG_BG2VOFS = scale - 3;
    REG_WININ = 0x3F3F;
    REG_WINOUT = 0x3F3B;
    win = (vu16 *)REG_ADDR_WIN1H;
    *win = ((gSlotTableX[kind] + gGaugeTableC[kind] - 1) << 3) | ((gSlotTableX[kind] + 1) << 11);
    win += 2;
    *win = ((gSlotTableZ[kind] + gGaugeTableD[kind] - 1) << 3) | ((gSlotTableZ[kind] + 1) << 11);
}
