#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0x11E - 0xEE];
    u16 unk11E;
};

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x28 - 0x1C];
    struct ActorPart *part;
    struct ActorObj *owner;
    struct ActorObj *kind;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    u8 filler5C[0x60 - 0x5C];
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 phase;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 side;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ActorFlagBits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 high : 4;
};

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
u32 obj_lookup_part_to_drift(struct ActorObj *obj, u32 arg);
void obj_wave_drift(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_timer_sprite_move_1(struct ActorObj *obj);
void side_walk_obj_index_zero_action4(struct ActorObj *obj);
void obj_timer_sprite_move_2(struct ActorObj *obj);
void obj_timer_sprite_move_3(struct ActorObj *obj);
void obj_timer_sprite_countdown(struct ActorObj *obj);
void obj_timer_clear_slot_action6(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_timer_action5_setup(struct ActorObj *obj);
void obj_timer_exit_left(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 mt19937_next(void);
void obj_action2_then_fall_d70c(struct ActorObj *obj);
void obj_action2_then_fall_d408(struct ActorObj *obj);
void obj_fall_and_launch(struct ActorObj *obj);
u32 obj_lookup_part_pick_state(struct ActorObj *obj, u32 arg);
void obj_wait_30_when_index_zero(struct ActorObj *obj);
void obj_anim5_when_index_zero(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
void actor_side_walk_steps(struct ActorObj *obj);
void obj_setup_partner_pose(struct ActorObj *obj);
void obj_timer_clear_slot_action6_alt(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_reset_when_index_zero(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_detach_active(struct ActorPose *pose);
void sprite_entry_update(struct ActorPose *pose);
void obj_owner_release_motion(struct ActorObj *obj);
void obj_move_to_base_right(struct ActorObj *obj);
void obj_owner_callback_step(struct ActorObj *obj);
void obj_lookup_slot_step(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void obj_drift_up_until_height(struct ActorObj *obj);
void obj_aim_at_owner(struct ActorObj *obj);
void obj_prepare_when_index_clear(struct ActorObj *obj);
void obj_arc_land(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_finish_if_active(struct ActorObj *obj);
void obj_idle_when_active(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_sync_child_reset(struct ActorObj *obj);

struct ActorCands
{
    u8 filler0[0x70];
    u8 *cand70;
    u8 *cand74;
};

extern struct ActorCands *gUnk_03000FD8;

void obj_fall_and_launch_b(struct ActorObj *obj)
{
    u8 *flags;
    struct ActorObj *candA;
    struct ActorObj *candB;

    obj->posH += obj->targetX;
    obj->targetX -= obj->phase;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        sprite_set_action(obj, 3, 0, 0);
        candA = (struct ActorObj *)(gUnk_03000FD8->cand70 + 8);
        candB = (struct ActorObj *)(gUnk_03000FD8->cand74 + 8);
        if (candB->part->slot >= 0 || candA->part->slot >= 0)
        {
            obj->dx = 1;
            obj->targetX = 0x8000;
            obj->targetY = 0x6C00;
            obj->phase = obj->baseH;
            obj->motionSpeed = 0x200;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionStart = 0;
            start_motion(obj);
        }
        else
        {
            obj->dx = 0;
            obj->targetX = 0x6800;
            obj->targetY = 0x6C00;
            obj->phase = obj->baseH;
            obj->motionSpeed = 0x200;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionStart = 0;
            start_motion(obj);
        }
        obj->state = side_walk_obj_index_zero_action4;
        set_slot_value(0x81, -1);
    }
}

void obj_start_motion_timer(struct ActorObj *obj)
{
    obj->side = 0;
    if (obj->param9C != 0)
    {
        obj->targetY = 0x5A00;
    }
    else
    {
        obj->targetY = 0x7E00;
    }
    if (obj->dx != 0)
    {
        obj->targetX = 0x4800;
    }
    else
    {
        obj->targetX = 0x3000;
    }
    obj->phase = obj->baseH;
    obj->motionSpeed = obj->paramA4;
    {
        u8 *flags = &obj->flags;
        *flags |= 0x20;
    }
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 10;
    obj->state = obj_timer_sprite_move_1;
}

void obj_timer_sprite_move_1(struct ActorObj *obj)
{
    u16 *timer = &obj->timer;
    u8 *flags;

    (*timer)--;
    if ((s16)*timer <= 0)
    {
        effect_spawn_at(0x2122, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *timer = 10;
    }
    if (obj->index == 0)
    {
        obj->side = 1;
        obj->targetY = 0x6C00;
        if (obj->dx != 0)
        {
            obj->targetX = 0x2800;
        }
        else
        {
            obj->targetX = -0x800;
        }
        obj->phase = obj->baseH;
        obj->motionSpeed = obj->paramA4;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_timer_sprite_move_2;
    }
}

void obj_timer_sprite_move_2(struct ActorObj *obj)
{
    u16 *timer = &obj->timer;
    u8 *flags;

    (*timer)--;
    if ((s16)*timer <= 0)
    {
        effect_spawn_at(0x2122, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *timer = 10;
    }
    if (obj->index == 0)
    {
        obj->side = 2;
        if (obj->param9C != 0)
        {
            obj->targetY = 0x7E00;
        }
        else
        {
            obj->targetY = 0x5A00;
        }
        if (obj->dx != 0)
        {
            obj->targetX = 0x4800;
        }
        else
        {
            obj->targetX = 0x3000;
        }
        obj->phase = obj->baseH;
        obj->motionSpeed = obj->paramA4;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_timer_sprite_move_3;
    }
}

void obj_timer_sprite_move_3(struct ActorObj *obj)
{
    u16 *timer = &obj->timer;
    u8 *flags;

    (*timer)--;
    if ((s16)*timer <= 0)
    {
        effect_spawn_at(0x212B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *timer = 10;
    }
    if (obj->index == 0)
    {
        obj->side = 3;
        obj->targetY = 0x6C00;
        if (obj->dx != 0)
        {
            obj->targetX = 0x8000;
        }
        else
        {
            obj->targetX = 0x6800;
        }
        obj->phase = obj->baseH;
        obj->motionSpeed = obj->paramA4;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_timer_sprite_countdown;
    }
}

void obj_timer_sprite_countdown(struct ActorObj *obj)
{
    u16 *timer = &obj->timer;

    (*timer)--;
    if ((s16)*timer <= 0)
    {
        effect_spawn_at(0x212B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *timer = 10;
    }
    if (obj->index == 0)
    {
        obj->paramA0--;
        if (obj->paramA0 > 0)
        {
            obj_start_motion_timer(obj);
        }
        else
        {
            obj->timer = 0x2D;
            obj->state = obj_timer_clear_slot_action6;
        }
    }
}

void side_walk_obj_index_zero_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x81);
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 4;
        obj->state = obj_timer_action5_setup;
    }
}

u32 obj_lookup_part_to_drift(struct ActorObj *obj, u32 arg)
{
    partner_link_update(obj, arg, 0xFF);
    obj->state = obj_wave_drift;
    return 0;
}

void obj_timer_clear_slot_action6(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        sound_stop_song(0xD2);
        sprite_set_action(obj, 6, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_timer_clear_slot_action6_alt(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        sound_stop_song(0xD2);
        sprite_set_action(obj, 6, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_index_zero_action4_exit(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x81);
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 4;
        obj->state = obj_timer_exit_left;
    }
}

u32 obj_random_side_init_a(struct ActorObj *obj)
{
    if (obj->paramA0 == 0)
    {
        obj->param9C = mt19937_next() & 1;
        obj->paramA0 = 2;
        obj->paramA4 = 0x280;
    }
    obj->state = obj_action2_then_fall_d70c;
    return 1;
}

u32 obj_random_side_init_b(struct ActorObj *obj)
{
    if (obj->paramA0 == 0)
    {
        obj->param9C = mt19937_next() & 1;
        obj->paramA0 = 3;
        obj->paramA4 = 2;
    }
    if (obj->param9C != 0)
    {
        obj->paramA4 = -obj->paramA4;
    }
    obj->state = obj_action2_then_fall_d408;
    return 1;
}

void obj_timer_action5_setup(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            set_slot_value(0xD2, -1);
            sprite_set_action(obj, 5, 0, 0);
            obj->handler68 = on_arrive;
            ((struct ActorModeBits *)&obj->mode)->bit6 = 0;
            ((struct ActorModeBits *)&obj->mode)->bit5 = 0;
            obj_list_push_default(obj);
            obj->handler58 = (void (*)(struct ActorObj *))obj_lookup_part_pick_state;
            obj->state = obj_start_motion_timer;
        }
    }
}

void obj_action2_then_fall_d70c(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->targetX = 0x300;
    obj->phase = 0x33;
    obj->state = obj_fall_and_launch_b;
}

void obj_action2_then_fall_d408(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->targetX = 0x300;
    obj->phase = 0x33;
    obj->state = obj_fall_and_launch;
}

u32 obj_lookup_part_pick_state(struct ActorObj *obj, u32 arg)
{
    partner_link_update(obj, arg, 0xFF);
    switch (obj->side)
    {
    case 0:
        obj->state = obj_timer_sprite_move_1;
        break;
    case 1:
        obj->state = obj_timer_sprite_move_2;
        break;
    case 2:
        obj->state = obj_timer_sprite_move_3;
        break;
    default:
        obj->state = obj_timer_sprite_countdown;
        break;
    }
    return 0;
}

void obj_init_pair_motion(struct ActorObj *obj)
{
    u8 *flags;

    gUnk_03000E3C = 0;
    obj->byte74 = obj->owner->byte74;
    obj->kind->byte74 = obj->owner->byte74;
    sprite_set_action(obj, 3, 0, 0);
    sprite_set_action(obj->kind, 3, 0, 0);
    obj->targetX = obj->baseX + 0x1000;
    obj->targetY = obj->baseY;
    obj->phase = obj->baseH;
    obj->motionSpeed = 0x100;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = obj_wait_30_when_index_zero;
}

void obj_timer_follow_owner(struct ActorObj *obj)
{
    u8 *flags;

    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        sprite_set_action(obj->kind, 4, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x6800;
        obj->targetY = obj->owner->part->targetY;
        obj->phase = obj->baseH;
        obj->motionSpeed = 0x100;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        set_slot_value(0x6C, -1);
        obj->state = obj_anim5_when_index_zero;
    }
}

void side_walk_obj_timer_spawn_effect(struct ActorObj *obj)
{
    u8 *mode;
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sprite_set_action(obj, 7, 0, 0);
            sprite_set_action(obj->kind, 7, 0, 0);
            mode = &obj->mode;
            *mode |= 0x40;
            *mode |= 0x20;
            obj->handler68 = on_arrive;
            gUnk_03000E3C = effect_from_table_obj(obj, 0x178E);
            obj->handler58 = actor_side_walk_steps;
            obj->side = 0;
            obj->targetX = obj->owner->part->targetX + 0x1800;
            obj->targetY = obj->owner->part->targetY;
            obj->phase = obj->baseH;
            obj->motionSpeed = 0x199;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionStart = 0;
            start_motion(obj);
            obj->state = obj_effect_sync_bytes;
        }
    }
}

void obj_effect_sync_bytes(struct ActorObj *obj)
{
    struct ActorObj *effect = (struct ActorObj *)gUnk_03000E3C;

    effect->posX = obj->posX;
    effect->posY = obj->posY;
    effect->posH = obj->posH;
    obj->byte75 = obj->owner->byte75 - 1;
    obj->kind->byte75 = obj->owner->byte75 + 5;
    if (obj->index == 0)
    {
        obj->byte75 = obj->owner->byte75;
        obj->kind->byte75 = obj->owner->byte75 + 1;
        obj->state = obj_setup_partner_pose;
    }
}

void obj_setup_partner_pose(struct ActorObj *obj)
{
    struct ActorObj *owner;
    u32 nonzero;
    u8 *flags;

    if (obj->side == 0)
    {
        sprite_set_action(obj, 9, 0, 0);
        sprite_set_action(obj->kind, 9, 0, 0);
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->phase = obj->part->targetH;
        obj->motionSpeed = 0x300;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_reset_when_index_zero;
        return;
    }
    if (obj->owner->handler68 != 0)
    {
        obj->owner->handler68(obj->owner);
    }
    owner = obj->owner;
    owner->state = actor_side_walk_idle;
    nonzero = (owner->part->unk11E & 3) != 0;
    obj_detach_active(owner->pose);
    if (obj->paramA0 == 0)
    {
        if (obj->param9C == 0)
        {
            if (nonzero != 0)
            {
                sprite_set_action(obj->kind, 0, 0x411F, 0);
                sprite_set_action(obj, 0, 0x411E, 0);
            }
            else
            {
                sprite_set_action(obj->kind, 0, 0x4117, 0);
                sprite_set_action(obj, 0, 0x4116, 0);
            }
        }
        else
        {
            if (nonzero != 0)
            {
                sprite_set_action(obj->kind, 0, 0x4123, 0);
                sprite_set_action(obj, 0, 0x4122, 0);
            }
            else
            {
                sprite_set_action(obj->kind, 0, 0x411B, 0);
                sprite_set_action(obj, 0, 0x411A, 0);
            }
        }
        effect_spawn_at(0x1F5B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    }
    else
    {
        if (obj->param9C == 0)
        {
            if (nonzero != 0)
            {
                sprite_set_action(obj->kind, 0, 0x4121, 0);
                sprite_set_action(obj, 0, 0x4120, 0);
            }
            else
            {
                sprite_set_action(obj->kind, 0, 0x4119, 0);
                sprite_set_action(obj, 0, 0x4118, 0);
            }
        }
        else
        {
            if (nonzero != 0)
            {
                sprite_set_action(obj->kind, 0, 0x4125, 0);
                sprite_set_action(obj, 0, 0x4124, 0);
            }
            else
            {
                sprite_set_action(obj->kind, 0, 0x411D, 0);
                sprite_set_action(obj, 0, 0x411C, 0);
            }
        }
        effect_spawn_at(0x1F74, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    }
    set_slot_value(0xAE, -1);
    obj->state = obj_owner_release_motion;
}

void obj_owner_release_motion(struct ActorObj *obj)
{
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        sprite_entry_update(obj->owner->pose);
        obj->owner->state = 0;
        partner_link_update(obj, (u32)obj->owner, 0xFF);
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 0, 0x4112, 0);
            sprite_set_action(obj->kind, 0, 0x4113, 0);
            sprite_set_action(obj, 9, 0, 0);
            sprite_set_action(obj->kind, 9, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0, 0x4114, 0);
            sprite_set_action(obj->kind, 0, 0x4115, 0);
            sprite_set_action(obj, 9, 0, 0);
            sprite_set_action(obj->kind, 9, 0, 0);
        }
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->phase = obj->part->targetH;
        obj->motionSpeed = 0x300;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_reset_when_index_zero;
    }
}

void obj_copy_bytes_sprite_88(struct ActorObj *obj)
{
    obj->byte74 = obj->owner->byte74;
    obj->kind->byte74 = obj->owner->byte74;
    obj->byte75 = obj->owner->byte75;
    obj->kind->byte75 = obj->owner->byte75 + 1;
    if (obj->paramA0 == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        sprite_set_action(obj->kind, 0xA, 0, 0);
        effect_spawn_at(0x1F8D, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    }
    else
    {
        sprite_set_action(obj, 0xB, 0, 0);
        sprite_set_action(obj->kind, 0xB, 0, 0);
        effect_spawn_at(0x1F95, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    }
    set_slot_value(0x88, -1);
    obj->state = obj_move_to_base_right;
}
