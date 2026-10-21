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

struct ShapeTask
{
    u8 filler0[0x18];
    void *cls;
    void *buffer;
    void (*release)(u32 arg);
    u32 releaseArg;
    u8 filler28[0x32 - 0x28];
    u8 unk32;
    u8 kind;
    u8 filler34[0x40 - 0x34];
    u16 unk40;
    u16 unk42;
};

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

ASM_FUNC("asm/code_08079F40/random_anim_by_mode.s", void random_anim_by_mode(struct SpriteObj *obj))

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

NONMATCH("asm/code_08079F40/spawn_kind_sprite.s", struct ShapeTask *spawn_kind_sprite(struct ShapeTask *obj, u8 flags, u32 taskArg, u8 kind, u16 half, u32 releaseArg, u32 window, u32 windowArg2, void (*release)(u32 arg)))
{
    task_insert(obj, flags, taskArg);
    obj->cls = (void *)gShapeTaskClass;
    if ((u8)start_window_buffer(window, windowArg2) == 0)
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
    obj->unk42 = half;
    obj->release = release;
    obj->releaseArg = releaseArg;
    switch (kind)
    {
    case 0:
    case 1:
        obj->unk32 = 0xA;
        obj->buffer = heap_alloc_raw(1, 0x50, gHeapTagKindSpriteBuf);
        obj->unk40 = half;
        break;
    case 2:
    case 3:
        obj->unk32 = 8;
        obj->buffer = heap_alloc_raw(1, 0x40, gHeapTagKindSpriteBuf);
        obj->unk40 = half;
        ((u32 *)obj->buffer)[2] = 0x7800;
        ((u32 *)obj->buffer)[3] = 0;
        ((u32 *)obj->buffer)[6] = 0xF000;
        ((u32 *)obj->buffer)[7] = 0x5000;
        ((u32 *)obj->buffer)[10] = 0x7800;
        ((u32 *)obj->buffer)[11] = 0xA000;
        ((u32 *)obj->buffer)[14] = 0;
        ((u32 *)obj->buffer)[15] = 0x5000;
        break;
    case 4:
    case 5:
        obj->unk32 = 8;
        obj->buffer = heap_alloc_raw(1, 0x40, gHeapTagKindSpriteBuf);
        obj->unk40 = half;
        ((u32 *)obj->buffer)[0] = 0;
        ((u32 *)obj->buffer)[1] = 0;
        ((u32 *)obj->buffer)[2] = 0x7800;
        ((u32 *)obj->buffer)[4] = 0xF000;
        ((u32 *)obj->buffer)[5] = 0;
        ((u32 *)obj->buffer)[7] = 0x5000;
        ((u32 *)obj->buffer)[8] = 0xF000;
        ((u32 *)obj->buffer)[9] = 0xA000;
        ((u32 *)obj->buffer)[10] = 0x7800;
        ((u32 *)obj->buffer)[12] = 0;
        ((u32 *)obj->buffer)[13] = 0xA000;
        ((u32 *)obj->buffer)[15] = 0x5000;
        break;
    case 6:
    case 7:
        obj->unk32 = 4;
        obj->buffer = heap_alloc_raw(1, 0x20, gHeapTagKindSpriteBuf);
        obj->unk40 = half;
        ((u32 *)obj->buffer)[0] = 0;
        ((u32 *)obj->buffer)[1] = 0;
        ((u32 *)obj->buffer)[4] = 0xF000;
        ((u32 *)obj->buffer)[5] = 0xA000;
        ((u32 *)obj->buffer)[6] = 0;
        ((u32 *)obj->buffer)[7] = 0xA000;
        break;
    case 8:
    case 9:
        obj->unk32 = 4;
        obj->buffer = heap_alloc_raw(1, 0x20, gHeapTagKindSpriteBuf);
        obj->unk40 = half;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        obj->unk40 = half;
        break;
    case 16:
    case 17:
        obj->unk32 = ((u8(*)(s32, u32))obj->releaseArg)(-1, obj->releaseArg);
        obj->buffer = heap_alloc_raw(1, obj->unk32 << 3, gHeapTagKindSpriteBuf);
        obj->unk40 = half;
        iris_timer_step((struct SpriteObj *)obj);
        return obj;
    default:
        return obj;
    }
    iris_timer_step((struct SpriteObj *)obj);
    return obj;
}
END_NONMATCH

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
