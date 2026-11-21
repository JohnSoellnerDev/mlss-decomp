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
    struct ActorObj *kind;      /* second sprite that moves with this one (child) */
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
    u8 byte74;                  /* draw group */
    u8 byte75;                  /* draw depth within the group */
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

// ActorPose.flags: the current animation has finished.
#define POSE_FLAG_ANIM_DONE 0x08

// ActorObj.flags: targetX/targetY/phase are absolute, not relative to the position
// (read and cleared by start_motion).
#define ACTOR_FLAG_ABS_TARGET 0x20

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

// A hero (Mario or Luigi): his actor starts 8 bytes in.
struct Hero
{
    u8 filler0[0x08];
    struct ActorObj body;
};

struct ActorCands
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct ActorCands *gWorld;

// Falls one frame (targetX is the height speed, phase the gravity). On landing: plays action 3,
// walks to x 0x8000 if either hero's part has a slot (dx 1) or else to 0x6800 (dx 0), at y 0x6C00,
// starts sound 0x81 and waits in side_walk_obj_index_zero_action4 for the walk to end.
void obj_fall_and_launch_b(struct ActorObj *obj)
{
    u8 *flags;
    struct ActorObj *heroA;
    struct ActorObj *heroB;

    obj->posH += obj->targetX;
    obj->targetX -= obj->phase;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        sprite_set_action(obj, 3, 0, 0);
        heroA = &gWorld->heroA->body;
        heroB = &gWorld->heroB->body;
        if (heroB->part->slot >= 0 || heroA->part->slot >= 0)
        {
            obj->dx = 1;
            obj->targetX = 0x8000;
            obj->targetY = 0x6C00;
            obj->phase = obj->baseH;
            obj->motionSpeed = 0x200;
            flags = &obj->flags;
            *flags |= ACTOR_FLAG_ABS_TARGET;
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
            *flags |= ACTOR_FLAG_ABS_TARGET;
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

/* Second leg of the loop started by obj_start_motion_timer: leaves effect 0x2122 every 10 frames
 * and, once the current walk is done (index 0), walks (side 1) to x 40 px (dx set) or -8 px,
 * y 108 px, at paramA4 speed, then goes on with obj_timer_sprite_move_2. */
void obj_timer_sprite_move_1(struct ActorObj *obj)
{
    u16 *effectTimer = &obj->timer;
    u8 *flags;

    (*effectTimer)--;
    if ((s16)*effectTimer <= 0)
    {
        effect_spawn_at(0x2122, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *effectTimer = 10;
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
        *flags |= ACTOR_FLAG_ABS_TARGET;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_timer_sprite_move_2;
    }
}

/* Third leg of the loop started by obj_start_motion_timer: leaves effect 0x2122 every 10 frames
 * and, once the current walk is done (index 0), walks (side 2) to x 72 px (dx set) or 48 px,
 * y 126 px (param9C set) or 90 px, at paramA4 speed, then goes on with obj_timer_sprite_move_3. */
void obj_timer_sprite_move_2(struct ActorObj *obj)
{
    u16 *effectTimer = &obj->timer;
    u8 *flags;

    (*effectTimer)--;
    if ((s16)*effectTimer <= 0)
    {
        effect_spawn_at(0x2122, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *effectTimer = 10;
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
        *flags |= ACTOR_FLAG_ABS_TARGET;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_timer_sprite_move_3;
    }
}

/* Fourth leg of the loop started by obj_start_motion_timer: leaves effect 0x212B every 10 frames
 * and, once the current walk is done (index 0), walks (side 3) to x 128 px (dx set) or 104 px,
 * y 108 px, at paramA4 speed, then goes on with obj_timer_sprite_countdown. */
void obj_timer_sprite_move_3(struct ActorObj *obj)
{
    u16 *effectTimer = &obj->timer;
    u8 *flags;

    (*effectTimer)--;
    if ((s16)*effectTimer <= 0)
    {
        effect_spawn_at(0x212B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        *effectTimer = 10;
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
        *flags |= ACTOR_FLAG_ABS_TARGET;
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

/* State: once the current animation has finished, counts the timer down; at zero both sprites take
   action 7, arriving calls on_arrive, effect 0x178E is spawned (gUnk_03000E3C), and obj walks with
   actor_side_walk_steps to 0x18 pixels right of the owner's part's target at speed 0x199, then keeps
   the effect on itself (obj_effect_sync_bytes). */
void side_walk_obj_timer_spawn_effect(struct ActorObj *obj)
{
    u8 *mode;
    u8 *flags;

    if (obj->pose->flags & POSE_FLAG_ANIM_DONE)
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
            *flags |= ACTOR_FLAG_ABS_TARGET;
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

/*
 * State: on side 0, both sprites take action 9 and walk (speed 0x300) to the
 * part's target, then reset once the index is zero. Otherwise the owner runs
 * its handler and goes back to walking idle, and obj and its second sprite
 * (obj->kind) take the pose picked by paramA0 (zero or not), param9C (the
 * random 0/1 choice) and whether the owner's part has bits 0-1 of 0x11E set,
 * with effect 0x1F5B (paramA0 zero) or 0x1F74; then the owner is released.
 */
void obj_setup_partner_pose(struct ActorObj *obj)
{
    struct ActorObj *owner;
    u32 ownerFlagged;
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
    ownerFlagged = (owner->part->unk11E & 3) != 0;
    obj_detach_active(owner->pose);
    if (obj->paramA0 == 0)
    {
        if (obj->param9C == 0)
        {
            if (ownerFlagged != 0)
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
            if (ownerFlagged != 0)
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
            if (ownerFlagged != 0)
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
            if (ownerFlagged != 0)
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

// State after obj_setup_partner_pose: once the animation ends, releases the owner (stops its
// state and unlinks it), plays the release animation on both sprites (the pair picked by the
// random param9C), then switches them to action 9 and walks back to the part's target.
void obj_owner_release_motion(struct ActorObj *obj)
{
    u8 *flags;

    if (obj->pose->flags & POSE_FLAG_ANIM_DONE)
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
        *flags |= ACTOR_FLAG_ABS_TARGET;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_reset_when_index_zero;
    }
}

/*
 * State: puts obj and its second sprite in the owner's draw group, at the owner's depth and one
 * above it, plays action 0xA (or 0xB when paramA0 is set) on both with effect 0x1F8D (0x1F95) at
 * the base position, plays sound 0x88, then waits in obj_move_to_base_right.
 */
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
    set_slot_value(0x88, -1); /* plays sound 0x88 */
    obj->state = obj_move_to_base_right;
}
