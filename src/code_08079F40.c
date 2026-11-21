#include "global.h"

struct SpriteEntry
{
    u8 filler0[0x12];
    u8 flags;
};

struct SpriteObj
{
    u8 filler0[0x08];
    struct SpriteEntry *entry;
    u8 filler0C[0x28 - 0x0C];
    struct SpriteObj *main;
    struct SpriteObj *owner;
    struct SpriteObj *child;
    u8 filler34[0x38 - 0x34];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    void (*callback)(struct SpriteObj *obj);
    u8 filler50[0x9C - 0x50];
    u32 mode;
    u32 unkA0;
    u32 phaseTimer;
    u32 phase;
    u16 timer;
    u16 timer2;
    u8 fillerB0[0x113 - 0xB0];
    u8 unk113;
};

/*
 * A screen wipe (iris) task: iris_timer_step redraws the window shape each frame from
 * framesLeft and duration, and the task ends (calling release) when framesLeft runs out.
 */
struct ShapeTask
{
    u8 filler0[0x18];
    void *cls;
    void *buffer;           /* the shape's vertices (struct WipeVertex), or -1 when it failed */
    void (*release)(u32 arg);
    u32 releaseArg;         /* also the shape's own data: (x, y, radius) or a vertex callback */
    u8 filler28[0x32 - 0x28];
    u8 vertexCount;
    u8 kind;                /* the shape; each comes in a pair that runs in opposite directions */
    u8 filler34[0x40 - 0x34];
    u16 framesLeft;
    u16 duration;
};

/* A vertex of a wipe shape, in 8.8 fixed-point screen coordinates. */
struct WipeVertex
{
    s32 x;
    s32 y;
};

#define WIPE_VERTS(task) ((struct WipeVertex *)(task)->buffer)

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160
#define FX8(n) ((n) << 8)

extern u8 gUnk_03000ED0;
extern u32 gUnk_03000F6C;
extern const u8 gShapeTaskClass[];
extern const u8 gHeapTagKindSpriteBuf[];

void task_insert(void *node, u8 kind, u32 data);
u32 start_window_buffer(u32 a, u32 b);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void sprite_release(struct ShapeTask *obj, u32 freeMem);
void iris_timer_step(struct SpriteObj *obj);
void sprite_set_action(struct SpriteObj *obj, u32 action, u32 arg, u32 extra);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct SpriteObj *owner);
void set_anim_10_spawn_child_random(struct SpriteObj *obj);
void set_anim_14_spawn_child(struct SpriteObj *obj);
void set_slot_value(u32 id, s32 value);
void obj_release(struct SpriteObj *obj);
void stop_dma_channel(void);
void heap_free(void *p);
void task_remove(void *node, u32 kind);
void anim_15_place_2a58(struct SpriteObj *obj);
void anim_13_place_29d2(struct SpriteObj *obj);
void start_child_at_main(struct SpriteObj *obj);
void place_child_and_clear(struct SpriteObj *obj);
void wait_child_then_pass_mode(struct SpriteObj *obj);
void set_anim_f_place(struct SpriteObj *obj);
void random_anim_by_mode(struct SpriteObj *obj);
void timer2_anim0(struct SpriteObj *obj);
void anim0_count(struct SpriteObj *obj);
void start_phase_anim_d(struct SpriteObj *obj);
void phase_step(struct SpriteObj *obj);
void wait_child_anim12(struct SpriteObj *obj);
void unlink_actor_pair(struct SpriteObj *obj);
void timer_spawn_child_402f(struct SpriteObj *obj);
void spawn_child_4030_with_nodes(struct SpriteObj *obj);
void set_anim_by_alt_flag(struct SpriteObj *obj);
void timer_anim_5_or_9(struct SpriteObj *obj);
void anim16_timer6_mode0(struct SpriteObj *obj);
void countdown_set_anim_0b_place_part(struct SpriteObj *obj);
void anim_d_flagged(struct SpriteObj *obj);
void spawn_child_4059_moving(struct SpriteObj *obj);
void toggle_mode_until_timer_done(struct SpriteObj *obj);
void obj_list_remove(struct SpriteObj *obj);
u32 lookup_slot(struct SpriteObj *obj, u32 arg);
u32 mt19937_next(void);

void anim0_clear_cb_if_active_c(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->callback = NULL;
    }
}

void timer2_anim_d(struct SpriteObj *obj)
{
    s32 t;

    t = obj->timer2 - 1;
    obj->timer2 = t;
    if ((t << 16) <= 0)
    {
        set_slot_value(0x54, -1);
        obj->timer2 = 0xA;
    }
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->callback = timer2_anim0;
    }
}

void remove_if_no_child(struct SpriteObj *obj)
{
    if (obj->child == NULL)
    {
        obj_release(obj);
    }
}

void actor_link_anim_steps(void)
{
}

u32 sub_08079FC8(struct SpriteObj *obj)
{
    obj->callback = anim_15_place_2a58;
    return 1;
}

u32 sub_08079FD4(struct SpriteObj *obj)
{
    obj->callback = anim_13_place_29d2;
    return 1;
}

u32 sub_08079FE0(struct SpriteObj *obj)
{
    obj->callback = start_child_at_main;
    return 1;
}

u32 sub_08079FEC(struct SpriteObj *obj)
{
    obj->callback = place_child_and_clear;
    return 1;
}

u32 sub_08079FF8(struct SpriteObj *obj)
{
    obj->callback = wait_child_then_pass_mode;
    return 1;
}

u32 sub_0807A004(struct SpriteObj *obj)
{
    obj->callback = set_anim_f_place;
    return 1;
}

u32 sub_0807A010(struct SpriteObj *obj)
{
    obj->callback = random_anim_by_mode;
    return 1;
}

u32 check_and_place(struct SpriteObj *obj, u32 arg)
{
    u32 result;

    result = lookup_slot(obj, arg);
    if (result != 0)
    {
        return result;
    }
    obj_list_remove(obj);
    set_slot_value(0xE3, -1);
    effect_spawn_at(0x2AA6, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    obj_release(obj);
    obj->main->phaseTimer = 1;
    obj->main->unk113 = 1;
    return -1;
}

void place_child_and_clear(struct SpriteObj *obj)
{
    struct SpriteObj *child = obj->child;

    effect_spawn_at(0x2AA6, child->posX / 0x100, child->posY / 0x100, child->posZ / 0x100, child);
    obj_release(obj->child);
    obj->callback = NULL;
    set_slot_value(0x55, -1);
    gUnk_03000ED0 = 0;
}

void remove_if_active(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        obj_release(obj);
    }
}

void set_anim_3_or_8(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        if (gUnk_03000F6C == 0)
        {
            sprite_set_action(obj, 3, 0x4029, 0);
        }
        else
        {
            sprite_set_action(obj, 8, 0x4029, 0);
        }
        obj->timer = 6;
        obj->callback = timer_anim_5_or_9;
    }
}

void wait_child_then_pass_mode(struct SpriteObj *obj)
{
    if (obj->child == NULL)
    {
        obj->callback = NULL;
    }
    else
    {
        sprite_set_action(obj, 0x11, 0, 0);
        obj->callback = wait_child_anim12;
        obj->child->unkA0 = obj->mode;
        obj->child->callback = set_anim_by_alt_flag;
    }
}

void set_anim_f_place(struct SpriteObj *obj)
{
    sprite_set_action(obj, 0xF, 0, 0);
    effect_spawn_at(0x2A8B, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    set_slot_value(0x119, -1);
    obj->callback = set_anim_10_spawn_child_random;
}

void anim_from_phase_plus5(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, obj->phase + 5, 0, 0);
        obj->timer = 0x14;
        obj->callback = spawn_child_4030_with_nodes;
    }
}

void anim_15_place_2a58(struct SpriteObj *obj)
{
    sprite_set_action(obj, 0x15, 0, 0);
    effect_spawn_at(0x2A58, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    obj->callback = anim16_timer6_mode0;
}

void anim_13_place_29d2(struct SpriteObj *obj)
{
    sprite_set_action(obj, 0x13, 0, 0);
    effect_spawn_at(0x29D2, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    set_slot_value(0x10C, -1);
    obj->callback = set_anim_14_spawn_child;
}

void timer2_anim0(struct SpriteObj *obj)
{
    s32 t;

    t = obj->timer2 - 1;
    obj->timer2 = t;
    if ((t << 16) <= 0)
    {
        set_slot_value(0x54, -1);
        obj->timer2 = 0xA;
    }
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->callback = unlink_actor_pair;
    }
}

void random_anim_by_mode(struct SpriteObj *obj)
{
    s32 rnd;
    s32 mode;

    rnd = mt19937_next();
    obj->mode = mode = rnd & 3;
    switch (mode)
    {
    case 0:
        sprite_set_action(obj, 7, 0, 0);
        break;
    case 1:
        sprite_set_action(obj, 8, 0, 0);
        break;
    case 2:
        sprite_set_action(obj, 9, 0, 0);
        break;
    default:
        sprite_set_action(obj, 0xA, 0, 0);
        break;
    }
    obj->callback = countdown_set_anim_0b_place_part;
    obj->timer = 4;
    set_slot_value(0x42, -1);
}

void timer_anim_5_or_9(struct SpriteObj *obj)
{
    s32 t;

    if (obj->entry->flags & 8)
    {
        t = obj->timer - 1;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            if (gUnk_03000F6C == 0)
            {
                sprite_set_action(obj, 5, 0x4029, 0);
            }
            else
            {
                sprite_set_action(obj, 9, 0x4029, 0);
            }
            obj->callback = anim_d_flagged;
        }
    }
}

void wait_child_anim12(struct SpriteObj *obj)
{
    if (obj->child->callback == NULL)
    {
        sprite_set_action(obj, 0x12, 0, 0);
        obj->callback = anim0_count;
    }
}

void anim16_timer6_mode0(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0x16, 0, 0);
        obj->timer = 6;
        obj->mode = 0;
        obj->callback = timer_spawn_child_402f;
    }
}

void anim_d_flagged(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0xD, 0x4029, 0);
        obj->callback = start_phase_anim_d;
    }
}

void anim0_count(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->callback = NULL;
        gUnk_03000ED0++;
    }
}

void start_phase_anim_d(struct SpriteObj *obj)
{
    sprite_set_action(obj, 0xD, 0x4029, 0);
    obj->callback = phase_step;
    obj->phaseTimer = 0;
    obj->phase = 0;
}

void phase_step(struct SpriteObj *obj)
{
    s32 t;
    s32 phase;

    t = obj->phaseTimer - 1;
    obj->phaseTimer = t;
    if (t <= 0)
    {
        spawn_child_4059_moving(obj);
        set_slot_value(0x86, -1);
        obj->phaseTimer = 6;
        phase = obj->phase + 1;
        obj->phase = phase;
        if (phase > 5)
        {
            if (obj->entry->flags & 8)
            {
                if (gUnk_03000F6C == 0)
                {
                    sprite_set_action(obj, 6, 0x4029, 0);
                }
                else
                {
                    sprite_set_action(obj, 0xA, 0x4029, 0);
                }
                obj->callback = toggle_mode_until_timer_done;
            }
        }
    }
}

/*
 * Starts a screen wipe task of the given shape that runs for duration frames: opens the
 * window buffer, allocates the shape's vertex buffer (filling in the vertices that never move:
 * screen corners and edge midpoints) and draws the first frame. Kinds 0-1 are a 10-point
 * shape, 2-3 a diamond, 4-5 a rectangle, 6-7 and 8-9 4-point shapes, 10-15 shapes drawn
 * without vertices, and 16-17 a polygon from the callback in releaseArg (called with -1 for
 * its vertex count). If the window buffer can't be had, the task is released at once.
 */
struct ShapeTask *spawn_kind_sprite(struct ShapeTask *obj, u8 taskKind, u32 taskData, u8 kind, u16 duration, u32 releaseArg, u32 window, u32 windowArg, void (*release)(u32 arg))
{
    task_insert(obj, taskKind, taskData);
    obj->cls = (void *)gShapeTaskClass;
    if ((u8)start_window_buffer(window, windowArg) == 0)
    {
        obj->buffer = (void *)-1;
        if (obj != NULL)
        {
            sprite_release(obj, 3);
        }
        return obj;
    }
    obj->buffer = NULL;
    obj->kind = kind;
    obj->duration = duration;
    obj->release = release;
    obj->releaseArg = releaseArg;
    switch (kind)
    {
    case 0:
    case 1:
        obj->vertexCount = 10;
        obj->buffer = heap_alloc_raw(1, 10 * sizeof(struct WipeVertex), gHeapTagKindSpriteBuf);
        obj->framesLeft = duration;
        iris_timer_step((struct SpriteObj *)obj);
        break;
    case 2:
    case 3:
        /* diamond: the edge midpoints, the others move */
        obj->vertexCount = 8;
        obj->buffer = heap_alloc_raw(1, 8 * sizeof(struct WipeVertex), gHeapTagKindSpriteBuf);
        obj->framesLeft = duration;
        WIPE_VERTS(obj)[1].x = FX8(SCREEN_WIDTH / 2);
        WIPE_VERTS(obj)[1].y = 0;
        WIPE_VERTS(obj)[3].x = FX8(SCREEN_WIDTH);
        WIPE_VERTS(obj)[3].y = FX8(SCREEN_HEIGHT / 2);
        WIPE_VERTS(obj)[5].x = FX8(SCREEN_WIDTH / 2);
        WIPE_VERTS(obj)[5].y = FX8(SCREEN_HEIGHT);
        WIPE_VERTS(obj)[7].x = 0;
        WIPE_VERTS(obj)[7].y = FX8(SCREEN_HEIGHT / 2);
        iris_timer_step((struct SpriteObj *)obj);
        break;
    case 4:
    case 5:
        /* rectangle: the corners and one coordinate of each edge midpoint */
        obj->vertexCount = 8;
        obj->buffer = heap_alloc_raw(1, 8 * sizeof(struct WipeVertex), gHeapTagKindSpriteBuf);
        obj->framesLeft = duration;
        WIPE_VERTS(obj)[0].x = 0;
        WIPE_VERTS(obj)[0].y = 0;
        WIPE_VERTS(obj)[1].x = FX8(SCREEN_WIDTH / 2);
        WIPE_VERTS(obj)[2].x = FX8(SCREEN_WIDTH);
        WIPE_VERTS(obj)[2].y = 0;
        WIPE_VERTS(obj)[3].y = FX8(SCREEN_HEIGHT / 2);
        WIPE_VERTS(obj)[4].x = FX8(SCREEN_WIDTH);
        WIPE_VERTS(obj)[4].y = FX8(SCREEN_HEIGHT);
        WIPE_VERTS(obj)[5].x = FX8(SCREEN_WIDTH / 2);
        WIPE_VERTS(obj)[6].x = 0;
        WIPE_VERTS(obj)[6].y = FX8(SCREEN_HEIGHT);
        WIPE_VERTS(obj)[7].y = FX8(SCREEN_HEIGHT / 2);
        iris_timer_step((struct SpriteObj *)obj);
        break;
    case 6:
    case 7:
        /* three corners fixed, vertex 1 sweeps round */
        obj->vertexCount = 4;
        obj->buffer = heap_alloc_raw(1, 4 * sizeof(struct WipeVertex), gHeapTagKindSpriteBuf);
        obj->framesLeft = duration;
        WIPE_VERTS(obj)[0].x = 0;
        WIPE_VERTS(obj)[0].y = 0;
        WIPE_VERTS(obj)[2].x = FX8(SCREEN_WIDTH);
        WIPE_VERTS(obj)[2].y = FX8(SCREEN_HEIGHT);
        WIPE_VERTS(obj)[3].x = 0;
        WIPE_VERTS(obj)[3].y = FX8(SCREEN_HEIGHT);
        iris_timer_step((struct SpriteObj *)obj);
        break;
    case 8:
    case 9:
        obj->vertexCount = 4;
        obj->buffer = heap_alloc_raw(1, 4 * sizeof(struct WipeVertex), gHeapTagKindSpriteBuf);
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        obj->framesLeft = duration;
        iris_timer_step((struct SpriteObj *)obj);
        break;
    case 16:
    case 17:
        obj->vertexCount = ((u8 (*)(s32))obj->releaseArg)(-1);
        obj->buffer = heap_alloc_raw(1, obj->vertexCount << 3, gHeapTagKindSpriteBuf);
        obj->framesLeft = duration;
        ((u32 (*)(struct SpriteObj *))iris_timer_step)((struct SpriteObj *)obj);
        return obj;
    default:
        return obj;
    }
    return obj;
}

ASM_FUNC("asm/code_08079F40/iris_timer_step.s", void iris_timer_step(struct SpriteObj *obj))

void sprite_release(struct ShapeTask *obj, u32 freeMem)
{
    obj->cls = (void *)gShapeTaskClass;
    if (obj->buffer != (void *)-1)
    {
        stop_dma_channel();
        if (obj->buffer != NULL)
        {
            heap_free(obj->buffer);
        }
        if (obj->release != NULL)
        {
            obj->release(obj->releaseArg);
        }
    }
    task_remove(obj, freeMem);
}

ASM_FUNC("asm/code_08079F40/sprite_frame_update.s", void sprite_frame_update(void))

ASM_FUNC("asm/code_08079F40/update_slot_sprites.s", void update_slot_sprites(void))
