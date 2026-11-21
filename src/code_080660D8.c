#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 cmd20;
};

#define ACTOR_POSE_ANIM_DONE 0x08 /* ActorPose.flags: the current animation has finished */

struct ActorPart
{
    u8 filler0[0xB4];
    u8 flagB4;
    u8 fillerB5[0xBA - 0xB5];
    s16 valueBA;
    u8 fillerBC[0xD8 - 0xBC];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler3[0x11E - 0xF8];
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
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x54 - 0x50];
    void (*handler54)(struct ActorObj *);
    s32 (*handler58)(struct ActorObj *, u32);
    s32 (*handler5C)(struct ActorObj *, s32, s32);
    s32 (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
    u8 dir;
    u8 flags;           /* MOTION_TARGET_ABSOLUTE */
    s16 motionStart;    /* start_motion: frames of the move, or 0 to work them out from motionSpeed */
    s16 motionSpeed;
    s32 index;          /* time of the move left, 8.8 frames (0: arrived) */
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;    /* arc moves (obj_lift_step_finish): height curve, < 0; else a mode/counter */
    s32 paramA0;    /* arc length in frames */
    s32 paramA4;    /* X distance to travel (8.8) */
    s32 paramA8;    /* Y distance to travel (8.8) */
    u16 timer;      /* frames into the arc */
    s16 dx;         /* arc start, pixels */
    s16 dy;
    s16 dh;
};

/* ActorObj.flags: start_motion takes targetX/Y/H as a position instead of an offset from posX/Y/H
   (and clears the flag) */
#define MOTION_TARGET_ABSOLUTE 0x20

/* ActorObj.param9C during the child sequence (obj_respawn_child_6, obj_respawn_child_7_move):
   bits 4-7 the step, bits 0-3 how often the step has been restarted after its link was lost */
#define CHILD_SEQ_STEP_MASK 0xF0
#define CHILD_SEQ_RETRY_MASK 0x0F
#define CHILD_SEQ_STEP_SPAWN 0x00 /* obj_respawn_child_6 */
#define CHILD_SEQ_STEP_MOVE 0x10  /* obj_respawn_child_7_move */

s32 is_state_busy(void);
s32 query_contact(struct ActorObj *obj, s32 a, s32 b);
s32 obj_query_contact(struct ActorObj *obj, s32 a, s32 b);
u32 lookup_slot(struct ActorObj *obj);
u32 lookup_link_slot(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
extern u32 gUnk_03000E3C;
void obj_pose_active_place_sprite_a(struct ActorObj *obj);
void obj_pose_active_finish(struct ActorObj *obj);
void obj_active_place_wait(struct ActorObj *obj);
void obj_active_move_to_owner(struct ActorObj *obj);
void obj_active_move_to_owner_action_d(struct ActorObj *obj);
void obj_active_spawn_child_5(struct ActorObj *obj);
void obj_action2_cmd10(struct ActorObj *obj);
void obj_timer_next_action(struct ActorObj *obj);
void obj_timer_action_a_slot(struct ActorObj *obj);
void obj_timer_remove_action_f(struct ActorObj *obj);
void obj_child_timer_finish(struct ActorObj *obj);
void obj_active_spawn_part_sprite(struct ActorObj *obj);
void obj_spawn_pair_child(struct ActorObj *obj);
void obj_motion_to_owner_800(struct ActorObj *obj);
void obj_idle_check_motion(struct ActorObj *obj);
void obj_no_child_action_c(struct ActorObj *obj);
void obj_wait_ready_action2_c(struct ActorObj *obj);
void glide_obj_wait_ready_action2(struct ActorObj *obj);
void glide_obj_wait_ready_action2_b(struct ActorObj *obj);
void glide_obj_timer_action8(struct ActorObj *obj);
void obj_spawn_effect_child(struct ActorObj *obj);
void obj_set_target_xyh(struct ActorObj *obj, s32 x, s32 y, s32 h);
void obj_counter_action8(struct ActorObj *obj);
void obj_active_timer_action_b(struct ActorObj *obj);
void obj_active_wait_state(struct ActorObj *obj);
void obj_active_motion_14c(struct ActorObj *obj);
void obj_active_motion_14c_b(struct ActorObj *obj);
void obj_index_wait_idle_action0(struct ActorObj *obj);
void obj_init_timer_14(struct ActorObj *obj);
void glide_obj_index_wait_action4(struct ActorObj *obj);
void obj_index_wait_slot_then_state(struct ActorObj *obj);
void obj_spawn_linked_child(struct ActorObj *obj);
void obj_timer_dispatch_10_11(struct ActorObj *obj);
void obj_timer_spawn_child_b(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *a, struct ActorObj *b, s32 c, s32 d, u32 e);
void obj_active_action_6_or_a(struct ActorObj *obj);
void obj_track_child_timer(struct ActorObj *obj);
void obj_respawn_child_6(struct ActorObj *obj);
void obj_respawn_child_7_move(struct ActorObj *obj);
void obj_wait_slots_action(struct ActorObj *obj);
void obj_index_wait_finish_child(struct ActorObj *obj);
void obj_track_child_then_finish(struct ActorObj *obj);
u32 obj_link_lookup_action1(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);

/* After the actor lost its link in the child sequence (obj_link_lookup_action1 waits 8 frames):
 * the first time in a step it restarts that step (action 5 for obj_respawn_child_6, action 6 for
 * obj_respawn_child_7_move) and counts the retry in param9C; a second loss, or a later step, gives
 * up with action 0xC and waits in wait_for_index. */
void obj_timer_next_action(struct ActorObj *obj)
{
    s32 step;

    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        step = obj->param9C & CHILD_SEQ_STEP_MASK;
        switch (step)
        {
        case CHILD_SEQ_STEP_SPAWN:
            if ((obj->param9C & CHILD_SEQ_RETRY_MASK) == 0)
            {
                sprite_set_action(obj, 5, 0, 0);
                obj->param9C++;
                obj->state = obj_respawn_child_6;
            }
            else
            {
                sprite_set_action(obj, 0xC, 0, 0);
                obj->state = wait_for_index;
            }
            break;
        case CHILD_SEQ_STEP_MOVE:
            if ((obj->param9C & CHILD_SEQ_RETRY_MASK) == 0)
            {
                sprite_set_action(obj, 6, 0, 0);
                obj->param9C++;
                obj->state = obj_respawn_child_7_move;
            }
            else
            {
                sprite_set_action(obj, 0xC, 0, 0);
                obj->state = wait_for_index;
            }
            break;
        default:
            sprite_set_action(obj, 0xC, 0, 0);
            obj->pose->cmd20 = 0x10;
            obj->state = wait_for_index;
            break;
        }
    }
}

/* One frame of the arc obj_spawn_linked_child throws an object in: X moves evenly from dx over
 * paramA4 in paramA0 frames, the height follows a parabola (param9C < 0) peaking 0x3000 above dh
 * halfway through. Once it is down to 8 pixels, spawns effect 0x113D where it is and releases it. */
void obj_lift_step_finish(struct ActorObj *obj)
{
    s32 fromPeak;

    obj->posX = obj->paramA4 * (s16)obj->timer / obj->paramA0 + ((s32)obj->dx << 8);
    fromPeak = (s16)obj->timer - obj->paramA0 / 2;
    obj->posH = (fromPeak * fromPeak) * obj->param9C + 0x3000 + ((s32)obj->dh << 8);
    obj->timer++;
    if (obj->posH <= 0x800)
    {
        effect_spawn_at(0x113D, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
    }
}

void obj_active_move_to_owner_action_d(struct ActorObj *obj)
{
    u8 *flags;
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x3000;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->baseH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x180;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_index_wait_slot_then_state;
        set_slot_value(0x81, -1);
    }
}

void obj_index_wait_slot_then_state(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        obj->timer = obj->paramA8;
        obj->state = obj_timer_action_a_slot;
        sound_stop_song(0x81);
        set_slot_value(0xAF, -1);
    }
}

u32 obj_lookup_slot_or_finish(struct ActorObj *obj)
{
    u32 result = lookup_slot(obj);
    if (result != 0)
    {
        return result;
    }
    effect_spawn_at(0x113D, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
    return 0;
}

u32 obj_link_lookup_action1(struct ActorObj *obj)
{
    u32 result = lookup_link_slot(obj);
    if (result != 0)
    {
        return result;
    }
    if (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    sprite_set_action(obj, 1, 0, 0);
    obj->timer = 8;
    obj->state = obj_timer_next_action;
    return 0;
}

void obj_index_wait_finish_child(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj_release(obj->child);
        obj->timer = 3;
        obj->state = glide_obj_timer_action8;
    }
}

void glide_obj_index_wait_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_active_spawn_child_5;
        sound_stop_song(0x81);
    }
}

s32 obj_init_side_counter(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 2;
    }
    obj->paramA8 = obj->param9C;
    obj->state = glide_obj_wait_ready_action2_b;
    return 1;
}

s32 obj_init_motion_params(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 3;
        obj->paramA0 = 0x20;
    }
    obj->paramA4 = obj->paramA0;
    obj->paramA8 = obj->param9C;
    obj->state = obj_action2_cmd10;
    return 1;
}

void obj_timer_action_a_slot(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            set_slot_value(0xAF, -1);
        }
        else
        {
            sprite_set_action(obj, 0xA, 0, 0);
            obj->timer = obj->paramA8 - 1;
            obj->state = obj_active_timer_action_b;
            set_slot_value(0xD5, -1);
        }
    }
}

void glide_obj_wait_ready_action2_b(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_active_move_to_owner_action_d;
    }
}

void glide_obj_timer_action8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sprite_set_action(obj, 8, 0, 0);
            obj->timer = obj->paramA8;
            obj->state = obj_active_spawn_part_sprite;
        }
    }
}

void obj_action2_cmd10(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->pose->cmd20 = 0x10;
    obj->state = obj_active_move_to_owner;
}

void obj_active_timer_action_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            set_slot_value(0xD5, -1);
        }
        else
        {
            sprite_set_action(obj, 0xB, 0, 0);
            obj_list_push_default(obj);
            obj->state = obj_motion_to_owner_800;
        }
    }
}

void obj_active_spawn_part_sprite(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 9, 0, 0);
        effect_spawn_at(0x112B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_spawn_linked_child(obj);
        set_slot_value(0x9C, -1);
        obj->state = obj_timer_dispatch_10_11;
    }
}

void obj_motion_to_owner_800(struct ActorObj *obj)
{
    u8 *flags;
    obj->targetX = obj->owner->part->targetX + 0x800;
    obj->targetY = obj->owner->part->targetY;
    obj->targetH = obj->baseH;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionSpeed = 0x200;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = obj_init_timer_14;
}

void obj_timer_dispatch_10_11(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            sprite_set_action(obj, 0x10, 0, 0);
            obj->param9C = 4;
            obj->state = obj_counter_action8;
        }
        else
        {
            sprite_set_action(obj, 0x11, 0, 0);
            obj->state = obj_active_wait_state;
        }
    }
}

void obj_init_timer_14(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj->timer = 0x14;
        obj->state = obj_timer_remove_action_f;
    }
}

void obj_active_wait_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->state = obj_no_child_action_c;
    }
}

void obj_counter_action8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->param9C--;
        if (obj->param9C <= 0)
        {
            sprite_set_action(obj, 8, 0, 0);
            obj->state = obj_active_spawn_part_sprite;
        }
    }
}

void obj_timer_remove_action_f(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        obj_list_remove(obj);
        sprite_set_action(obj, 0xF, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_no_child_action_c(struct ActorObj *obj)
{
    u8 count = 0;
    struct ActorObj *walk = obj->child;
    if (walk != NULL)
    {
        do
        {
            count++;
            walk = walk->child;
        } while (walk != NULL);
    }
    if (count == 0)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        obj->state = wait_for_index;
    }
}

/* Once the actor's animation has finished: it plays action 3 (4 when param9C is set) and moves at
   speed 0x14C to 0x40 px right of its owner's part's target, at Y 0x68 px and its own base height,
   with move sound 0x129 (0x12A when paramA0 is set), which obj_wait_slots_action stops when it
   arrives. */
void obj_active_motion_14c(struct ActorObj *obj)
{
    u8 *flags;

    if (obj->pose->flags & ACTOR_POSE_ANIM_DONE)
    {
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 3, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 4, 0, 0);
        }
        obj->targetX = obj->owner->part->targetX + 0x4000;
        obj->targetY = 0x6800;
        obj->targetH = obj->baseH;
        flags = &obj->flags;
        *flags |= MOTION_TARGET_ABSOLUTE;
        obj->motionSpeed = 0x14C;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_wait_slots_action;
        if (obj->paramA0 == 0)
        {
            set_slot_value(0x129, -1);
        }
        else
        {
            set_slot_value(0x12A, -1);
        }
    }
}
