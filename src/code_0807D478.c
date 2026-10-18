#include "global.h"

struct Task
{
    struct Task *next;
    u32 fn;
    u8 *data;
    struct PopupNode *node;
    u16 timer;
};

struct PopupNode
{
    struct PopupNode *next;
    s32 x;
    s32 y;
    u8 *out;
    s16 vx;
    u16 vy;
    u8 filler14[2];
    u16 life;
    s16 kind;
};

struct PopupDirtyBits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
};

struct SpritePart
{
    u8 filler0[0x77];
    u8 index;
};

struct PartIndexBits
{
    u8 low : 3;
    u8 high : 5;
};

struct LevelBits
{
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 hi4 : 4;
};

struct ModeBits
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

struct ObjSlot
{
    u8 filler0[0x38];
    struct SpriteObj *link;
    u8 filler3C[0x7E - 0x3C];
    u8 flags7E;
};

struct SpriteObj;

typedef s32 (*ObjHandler)(struct SpriteObj *obj, struct SpritePart *part);

struct RingFlagsBits
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct Flags76Bits
{
    u8 lo3 : 3;
    u8 mid3 : 3;
    u8 hi2 : 2;
};

struct RecordStateBits
{
    u8 lo1 : 1;
    u8 mid2 : 2;
    u8 hi5 : 5;
};

struct SpriteRecord
{
    u8 filler0[0x12];
    u8 state;
};

struct SpriteRing
{
    u8 filler0[0xEC];
    s16 kind;
    u8 fillerEE[0xF6 - 0xEE];
    s16 f6;
    u8 fillerF8[0x111 - 0xF8];
    u8 flags111;
    u8 filler112[0x11E - 0x112];
    u16 flags11E;
};

struct SpriteObj
{
    u8 filler0[0x08];
    struct SpriteRecord *record;
    u8 filler0C[0x28 - 0x0C];
    struct SpriteRing *ring;
    u8 filler2C[0x30 - 0x2C];
    struct SpriteObj *parent;
    struct SpriteObj *activeNext;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    u32 stateFn;
    u8 filler50[0x54 - 0x50];
    u32 unk54;
    ObjHandler handlers[4];
    u32 unk68;
    u8 filler6C[0x76 - 0x6C];
    u8 flags76;
    u8 filler77[0xB8 - 0x77];
    u16 b8;
};

struct RasterNode
{
    struct RasterNode *next;
    u32 unk4;
    u8 filler8[4];
    void (*handler)(struct RasterNode *node);
    s16 line;
};

struct World
{
    u8 filler0[0x0B];
    u8 dirty;
    u8 flagsC;
    u8 levelD;
    u8 filler0E[0x3C - 0x0E];
    struct Task *taskFree;
    u8 filler40[0x48 - 0x40];
    struct Task *taskList;
    struct Task *list4C;
    u8 filler50[0x54 - 0x50];
    struct SpriteObj *objs;
    u8 filler58[0x70 - 0x58];
    struct SpriteObj *unk70;
    struct SpriteObj *unk74;
    u8 filler78[0x80 - 0x78];
    struct ObjSlot *slotA;
    struct ObjSlot *slotB;
    u8 filler88[0x240 - 0x88];
    struct PopupNode *nodePool;
    u8 filler244[0x24C - 0x244];
    void *camera;
    u8 filler250[0x254 - 0x250];
    struct RasterNode *rasterList;
    struct RasterNode *rasterCur;
};

extern struct World *gWorld;
void sprite_set_action(struct SpriteObj *obj, s32 index, s32 value, s32 flags);
void set_slot_value(s32 slot, s32 value);
void load_table_entries(s32 count);
void release_object(struct SpriteRing *ring);
void place_targets(struct SpriteObj *obj, s32 col, s32 row, s32 z, s32 extra);
void set_motion_start(struct SpriteObj *obj, s32 value);
void task_release_obj(struct Task *task);
extern void obj_update_pose(void);
void raster_irq_vcount(void);
void obj_run_part(struct SpriteObj *obj, struct SpritePart *part);
s32 obj_try_run_part(struct SpriteObj *obj, struct SpritePart *part);
void popup_rise_step_b(struct Task *task);
void set_irq_callback(u32 kind, void *cb);
void raster_irq_default(void);
extern vu16 gUnk_02000004;
void oam_write_tiles(u8 *digits, s32 x, s32 y, s32 kind, s32 flags);
void oam_write_number(u8 *digits, s32 x, s32 y, s32 scale, s32 flags);
void popup_fade_step(struct Task *task);
void popup_fade_step_b(struct Task *task);
void effect_follow(void);
u32 search_frame_pairs(struct SpriteObj *obj, struct SpritePart *part);

ASM_FUNC("asm/code_0807D478/number_popup.s", void number_popup(void))

void popup_rise_step(struct Task *task)
{
    struct PopupNode *node = task->node;
    s32 t;

    node->vx = (node->vx * 0xFA) / 0x100;
    node->vy += 0x3D;
    node->x += node->vx;
    node->y += (s16)node->vy;
    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    if ((u16)t == 0)
    {
        node->life = 0x18;
        task->fn = (u32)popup_fade_step;
    }
}

void popup_fade_step(struct Task *task)
{
    struct PopupNode *node = task->node;
    u8 *out;
    s16 life;
    s32 t;

    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    life = t;
    if (life == 0)
    {
        out = node->out;
        if (out != NULL)
            *out = 0;
        if (node != NULL)
        {
            node->next = gWorld->nodePool;
            gWorld->nodePool = node;
        }
        task->fn = life;
    }
}

ASM_FUNC("asm/code_0807D478/number_popup_spawn.s", struct Task *number_popup_spawn(u32 value, s32 x, s32 y, s32 z, s32 kind))

void popup_rise_step_b(struct Task *task)
{
    struct PopupNode *node = task->node;
    s32 t;

    node->vx = (node->vx * 0xFA) / 0x100;
    node->vy += 0x3D;
    node->x += node->vx;
    node->y += (s16)node->vy;
    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    if ((u16)t == 0)
    {
        node->life = 0x18;
        task->fn = (u32)popup_fade_step_b;
    }
}

void popup_fade_step_b(struct Task *task)
{
    struct PopupNode *node = task->node;
    u8 *out;
    s16 life;
    s32 t;

    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    life = t;
    if (life == 0)
    {
        out = node->out;
        if (out != NULL)
            *out = 0;
        if (node != NULL)
        {
            node->next = gWorld->nodePool;
            gWorld->nodePool = node;
        }
        task->fn = life;
    }
}

ASM_FUNC("asm/code_0807D478/popup_digit_spawn.s", void popup_digit_spawn(void))

void popup_wave_step(struct Task *task)
{
    struct PopupNode *node = task->node;
    u8 *digits;
    s32 t;

    node->vy -= 0xB4;
    node->y += (s16)node->vy;
    t = node->life - 1;
    node->life = t;
    digits = task->data;
    oam_write_number(digits, node->x / 0x100, node->y / 0x100, ((s16)node->life * 2 + (s16)node->life) * 4, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    if (node->y / 0x100 <= -16)
    {
        if (node != NULL)
        {
            node->next = gWorld->nodePool;
            gWorld->nodePool = node;
        }
        task->fn = 0;
    }
}

ASM_FUNC("asm/code_0807D478/oam_write_number.s", void oam_write_number(u8 *digits, s32 x, s32 y, s32 scale, s32 flags))

NONMATCH("asm/code_0807D478/raster_line_add.s", struct RasterNode *raster_line_add(void (*handler)(struct RasterNode *node), s32 line, void *callback))
{
    struct RasterNode *node;
    struct RasterNode *cur;
    struct RasterNode *prev;
    vu16 *irq;

    set_irq_callback(3, raster_irq_vcount);
    REG_IME = 0;
    gUnk_02000004 &= 0xFF;
    gUnk_02000004 |= 0x20;
    irq = (vu16 *)REG_ADDR_IE;
    *irq |= 4;
    irq++;
    *irq |= 4;
    REG_IME = 1;
    node = (struct RasterNode *)gWorld->taskFree;
    gWorld->taskFree = (struct Task *)node->next;
    cur = gWorld->rasterList;
    prev = NULL;
    while (cur != NULL && line >= cur->line)
    {
        prev = cur;
        cur = cur->next;
    }
    node->next = cur;
    if (prev != NULL)
        prev->next = node;
    else
        gWorld->rasterList = node;
    if (callback != NULL)
        node->unk4 = (u32)callback;
    else
        node->unk4 = (u32)raster_irq_default;
    node->handler = handler;
    node->line = line;
    return node;
}
END_NONMATCH

ASM_FUNC("asm/code_0807D478/task_frame_update.s", void task_frame_update(void))

ASM_FUNC("asm/code_0807D478/raster_irq_vcount.s", void raster_irq_vcount(void))

ASM_FUNC("asm/code_0807D478/obj_draw_part.s", void obj_draw_part(void))

ASM_FUNC("asm/code_0807D478/draw_obj_pose.s", void draw_obj_pose(void))

ASM_FUNC("asm/code_0807D478/sprite_objects_obj_set_mode.s", void sprite_objects_obj_set_mode(void))

void task_begin_obj(struct Task *task)
{
    struct SpriteObj *obj = (struct SpriteObj *)task->data;

    ((struct Flags76Bits *)&obj->flags76)->mid3 = 0;
    if (obj->ring->f6 == 0)
    {
        ((struct RingFlagsBits *)&obj->ring->flags111)->bit0 = 1;
        task_release_obj(task);
        load_table_entries(0x24);
        return;
    }
    if (obj->ring->kind == -1)
        sprite_set_action(obj, 0, 0x2000, 0);
    else
        sprite_set_action(obj, 0, 0x204D, 0);
    if (obj->ring->flags11E & 0x200)
    {
        if (obj->ring->kind == -1)
            set_slot_value(0x47, -1);
        else
            set_slot_value(0x47, -1);
    }
    task->fn = 0;
}

void task_release_obj(struct Task *task)
{
    struct SpriteObj *obj = (struct SpriteObj *)task->data;
    struct SpriteRecord *rec;

    release_object(obj->ring);
    obj->stateFn = 0;
    obj->unk68 = 0;
    if (obj->ring->kind == -1)
    {
        sprite_set_action(obj, 1, 0x2028, 0);
        set_slot_value(0x71, -1);
    }
    else
    {
        sprite_set_action(obj, 1, 0x2057, 0);
        set_slot_value(0x71, -1);
    }
    rec = obj->record;
    ((struct RecordStateBits *)&rec->state)->mid2 = 0;
    place_targets(obj, obj->posX / 0x100 - 0x16, obj->posY / 0x100, obj->posZ / 0x100, 0x100);
    set_motion_start(obj, 0x80);
    task->fn = (u32)obj_update_pose;
}

ASM_FUNC("asm/code_0807D478/obj_run_part.s", void obj_run_part(struct SpriteObj *obj, struct SpritePart *part))

ASM_FUNC("asm/code_0807D478/obj_try_run_part.s", s32 obj_try_run_part(struct SpriteObj *obj, struct SpritePart *part))

void update_active_objs(void)
{
    struct SpriteObj *obj;
    struct SpriteObj *next;
    struct ObjSlot *slot;
    struct SpriteObj *link;

    ((struct LevelBits *)&gWorld->levelD)->mid2 = 0;
    obj = gWorld->objs;
    while (obj != NULL)
    {
        next = obj->activeNext;
        slot = gWorld->slotA;
        if (slot != NULL && ((struct ModeBits *)&slot->flags7E)->mode == 1)
        {
            link = slot->link;
            if (link == NULL || ((struct ModeBits *)&link->flags76)->mode != 1)
                obj_run_part(obj, (struct SpritePart *)((u8 *)slot + 8));
            else if (obj_try_run_part(obj, (struct SpritePart *)link) == 0)
                obj_run_part(obj, (struct SpritePart *)((u8 *)slot + 8));
        }
        slot = gWorld->slotB;
        if (slot != NULL && ((struct ModeBits *)&slot->flags7E)->mode == 1)
        {
            link = slot->link;
            if (link == NULL || ((struct ModeBits *)&link->flags76)->mode != 1)
                obj_run_part(obj, (struct SpritePart *)((u8 *)slot + 8));
            else if (obj_try_run_part(obj, (struct SpritePart *)link) == 0)
                obj_run_part(obj, (struct SpritePart *)((u8 *)slot + 8));
        }
        obj = next;
    }
}

ASM_FUNC("asm/code_0807D478/unknown_mario_bros_code_0807e996.s", void unknown_mario_bros_code_0807e996(void))
