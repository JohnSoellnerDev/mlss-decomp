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
    u8 filler0C[0x7C - 0x0C];
    struct ActiveObj *active;
    u8 filler80[0x250 - 0x80];
    void *slot250;
    u8 filler254[0x28C - 0x254];
    struct WorldEffect *effect;
    u8 filler290[0x29C - 0x290];
    struct Voice voice29C;
    u8 filler2B0[0x2BC - 0x2B0];
    u8 uploadState;
    u8 filler2BD[0x2FC - 0x2BD];
    struct Obj *slots[5];
    struct StatusObj *statusObj;
    u16 counter314;
    u16 scroll316;
};

extern struct World *gUnk_03000FD8;
extern u8 *gWorkBuf;
extern struct OamList *gUnk_03000D74;
extern u8 gOamTileTable[];
extern u8 gSlotTableX[];
extern u8 gSlotTableZ[];
extern u8 gGaugeTableC[];
extern u8 gGaugeTableD[];

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

extern u8 gUnk_0300034C[];

NONMATCH("asm/code_0810A2FC/state_status_wait_slot.s", void state_status_wait_slot(struct Owner *owner))
{
    struct World *world;
    struct Obj *slot;
    void (*next)(struct Owner *);
    s32 counter;

    status_slots_update(owner);
    world = gUnk_03000FD8;
    counter = 0x400 + world->counter314;
    world->counter314 = counter;
    if ((s32)(counter << 16) > 0x1FFF0000)
    {
        world->counter314 = 0x2000;
        slot = gUnk_03000FD8->slots[gUnk_03000FD8->statusObj->index];
        if (slot->owner != 0)
        {
            next = status_wait_slot;
        }
        else
        {
            world->lock = world->lock & 0x1F;
            next = status_slots_update;
        }
        owner->state = next;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0810A2FC/state_status_release.s", void state_status_release(void))

ASM_FUNC("asm/code_0810A2FC/status_slots_update.s", void status_slots_update(struct Owner *owner))

ASM_FUNC("asm/code_0810A2FC/oam_link_slot_entries.s", void oam_link_slot_entries(void))

NONMATCH("asm/code_0810A2FC/obj_motion_start.s", struct Owner *obj_motion_start(struct Obj *obj, s32 x, s32 y, s32 height, s32 kind))
{
    struct Owner *task;
    s32 clampX;
    s32 clampY;
    s32 t;

    task = obj->owner;
    if (task == 0)
    {
        task = task_create_list_4c(state_motion_step, obj);
        obj->owner = task;
    }
    clamp_reflect_offsets(&clampX, &clampY, obj->z, height, kind);
    obj->unkA4 = clampX;
    obj->unkA8 = clampY;
    if (clampX < 0)
    {
        t = clampX + 0x1F;
    }
    else
    {
        t = clampX;
    }
    t = (t >> 5) * clampX;
    if (t < 0)
    {
        t += 0x3F;
    }
    obj->speed = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x2CC000, t >> 6);
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

NONMATCH("asm/code_0810A2FC/state_motion_step.s", void state_motion_step(struct Owner *owner))
{
    struct Obj *obj;
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
        dx = ease * (0x100 - v);
        if (dx < 0)
        {
            dx += 0xFF;
        }
        v = v + (dx >> 8);
        obj->pic->a4 = v;
        obj->pic->a6 = v;
    }
}
END_NONMATCH

NONMATCH("asm/code_0810A2FC/status_cursor_step.s", s32 status_cursor_step(u32 cursor, u32 count))
{
    s32 sel;
    s32 back;

    sel = cursor & 0x7F;
    if ((gUnk_03000FD8->lock & 0xE0) == 0)
    {
        if (gUnk_03000FD8->slots[sel] == 0 || gUnk_03000FD8->slots[sel]->owner == 0)
        {
            if (gUnk_03000FD8->active->action48 == 0)
            {
                if ((*(u16 *)(gUnk_0300034C + 0x42) & 0x20) != 0 && count != 1)
                {
                    back = count - 1;
                    if (sel > 0)
                    {
                        back = sel - 1;
                    }
                    sel = back;
                    gUnk_03000FD8->scroll316 -= 1;
                    set_slot_value(0x78, -1);
                }
                if ((*(u16 *)(gUnk_0300034C + 0x42) & 0x10) != 0 && count != 1)
                {
                    back = count - 1;
                    if (sel < back)
                    {
                        sel = sel + 1;
                    }
                    else
                    {
                        sel = 0;
                    }
                    gUnk_03000FD8->scroll316 += 1;
                    set_slot_value(0x78, -1);
                }
            }
        }
    }
    gUnk_03000FD8->statusObj->index = sel;
    return sel;
}
END_NONMATCH

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

    world = gUnk_03000FD8;
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

ASM_FUNC("asm/code_0810A2FC/place_world_object.s", void place_world_object(s32 x, s32 y, s32 z, s32 mode, u32 kind))

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

ASM_FUNC("asm/code_0810A2FC/update_slot_marker.s", void update_slot_marker(void))

void tile_upload_step(u16 block)
{
    struct World *world;
    u8 state;

    world = gUnk_03000FD8;
    state = world->uploadState;
    if (state == 0)
    {
        dma_copy_or_queue(gWorkBuf + ((block << 9) + 0x4E70), (void *)0x060113A0, 0x100, 0x20, 0);
        gUnk_03000FD8->uploadState = 0x26;
    }
    else if (state == 0x13)
    {
        dma_copy_or_queue(gWorkBuf + ((((block << 4) + 8) << 5) + 0x4E70), (void *)0x060113A0, 0x100, 0x20, 0);
    }
    gUnk_03000FD8->uploadState = gUnk_03000FD8->uploadState - 1;
}

ASM_FUNC("asm/code_0810A2FC/start_voice_message.s", void start_voice_message(void))

ASM_FUNC("asm/code_0810A2FC/update_voice_message.s", void update_voice_message(void))

void set_world_effect(s32 kind)
{
    struct World *world;
    struct WorldEffect *obj;

    obj = gUnk_03000FD8->effect;
    if (obj != 0)
    {
        obj->fn = draw_world_effect_rows;
    }
    else
    {
        obj = task_create_list_48(draw_world_effect_rows, 0);
        obj->kind = -1;
    }
    world = gUnk_03000FD8;
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

void unknown_thumb_0810ba66(void)
{
    struct WorldEffect *effect;
    vu16 *win;
    u32 kind;
    s32 scale;
    s32 t;
    u32 x;
    u32 y;

    effect = gUnk_03000FD8->effect;
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
