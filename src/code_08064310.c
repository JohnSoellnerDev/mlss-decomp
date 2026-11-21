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

struct Flags77Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct PoseBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct PoseScale
{
    u8 filler0[0x04];
    u16 scaleX;
    u16 scaleY;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct ActorPart
{
    u8 filler0[0xB4];
    u16 unkB4;
    u8 filler1[0xD8 - 0xB6];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler2[0xEC - 0xE4];
    s16 slot;
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
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
    u8 filler76[0x77 - 0x76];
    u8 flags77;
    u8 flags78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

// The spawned child takes over the kind and two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child, u32 kind)
{
    ((struct Flags78Bits *)&child->flags78)->kind = kind >> 5;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void on_arrive(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void set_slot_value(u32 id, s32 value);
void obj_release(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 is_state_busy(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
u32 lookup_part(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_query_part_action3(struct ActorObj *obj);
void obj_pose_active_effect_c(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
extern u32 gUnk_03000E18;
extern s32 gUnk_03000E60;
extern s32 gUnk_03000E64;
u32 poll_spline_path(u32 buf, s32 *x, s32 *h);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void heap_free_b(u32 ptr);
void set_targets_from_record(struct ActorObj *obj);
void obj_motion_from_owner(struct ActorObj *obj);
void obj_place_sprite_finish_b(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj);

void obj_timer_effect_b(struct ActorObj *obj);
void obj_action7_wait(struct ActorObj *obj);
void obj_wait_ready_action3(struct ActorObj *obj);
u32 obj_query_effect(struct ActorObj *obj);
void obj_effect_action_c(struct ActorObj *obj);
void obj_drift_left(struct ActorObj *obj);
void obj_countdown_then_action1(struct ActorObj *obj);
void obj_pose_active_action0_ext(struct ActorObj *obj);
void parts_obj_pose_active_timer_action_d(struct ActorObj *obj);
void obj_spawn_part_sprite(struct ActorObj *obj);
void obj_slide_step_then_cull(struct ActorObj *obj);
void parts_obj_spawn_child_launch(struct ActorObj *obj);
void obj_update_position_timer(struct ActorObj *obj);
u32 obj_slot_check_action1(struct ActorObj *obj);
void obj_pose_active_stop(struct ActorObj *obj);
u32 obj_query_part_then_state(struct ActorObj *obj);
u32 obj_lookup_slot_then_state(struct ActorObj *obj);
void obj_pose_active_action0_next(struct ActorObj *obj);
u32 parts_obj_start_wait_place_sprite(struct ActorObj *obj);
u32 obj_start_choose_zero(struct ActorObj *obj);
u32 obj_start_restart_action6(struct ActorObj *obj);
u32 obj_start_choose_one(struct ActorObj *obj);
u32 obj_start_action3_zero(struct ActorObj *obj);
void obj_move_xh_then_cull(struct ActorObj *obj);
void obj_restart_action6(struct ActorObj *obj);
void parts_obj_wait_ready_place_sprite(struct ActorObj *obj);
void parts_obj_fall_step(struct ActorObj *obj);
void obj_place_sprite_and_finish(struct ActorObj *obj);
void actor_parts_steps(struct ActorObj *obj);
void obj_action3_restart_spawn(struct ActorObj *obj);
void obj_wait_ready_choose_state(struct ActorObj *obj);
void obj_timer_up_then_idle(struct ActorObj *obj);
void obj_pose_active_action3(struct ActorObj *obj);
void parts_obj_pose_active_clear_state(struct ActorObj *obj);
void obj_active_place_then_wait(struct ActorObj *obj);
void parts_obj_spawn_child_b(struct ActorObj *obj);
void unknown_code_08065044(struct ActorObj *obj);
void obj_poll_buffer_or_free(struct ActorObj *obj);
void obj_timer_start_motion(struct ActorObj *obj);
void obj_wait_then_slide_and_finish(struct ActorObj *obj);
void obj_move_step_c(struct ActorObj *obj);
void obj_move_step_d(struct ActorObj *obj);
void obj_glide_step_finish(struct ActorObj *obj);
void obj_wait_flag_then_cull(struct ActorObj *obj);
void obj_wait_then_state_5619(struct ActorObj *obj);
void obj_pose_active_action5_or_9(struct ActorObj *obj);
u32 obj_check_flag_a0_zero(struct ActorObj *obj);
u32 obj_check_flag_a0_one(struct ActorObj *obj);
void parts_obj_pose_active_idle(struct ActorObj *obj);
void parts_obj_wait_ready_action2(struct ActorObj *obj);
void obj_pose_active_place_sprite_a(struct ActorObj *obj);
void obj_timer_spawn_child_a(struct ActorObj *obj);
void obj_timer_spawn_child_b(struct ActorObj *obj);
void obj_timer_spawn_effect_place(struct ActorObj *obj);
u32 obj_lookup_place_cull(struct ActorObj *obj);
void obj_pose_active_finish(struct ActorObj *obj);
void obj_pose_active_effect_b(struct ActorObj *obj);
u32 obj_lookup_slot_then_state_bc0(struct ActorObj *obj);
void obj_move_place_cull(struct ActorObj *obj);
void obj_no_sprite_idle(struct ActorObj *obj);

void obj_timer_effect_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 0xB, 0, 0);
            effect_from_table_obj(obj, 0x1471);
            obj->handler5C = (void (*)(struct ActorObj *))obj_query_effect;
            set_slot_value(0x83, -1);
            obj->state = obj_effect_action_c;
        }
    }
}

void obj_action7_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = obj_drift_left;
    }
}

void obj_wait_ready_action3(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_motion_from_owner;
    }
}

u32 obj_query_effect(struct ActorObj *obj)
{
    u32 result = lookup_part(obj);

    if (result == 0)
    {
        obj->state = obj_countdown_then_action1;
        obj->targetX = 0x10;
    }
    return result;
}

void obj_effect_action_c(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        effect_from_table_obj(obj, 0x1485);
        obj->handler5C = (void (*)(struct ActorObj *))obj_query_effect;
        obj->timer = 3;
        obj->state = parts_obj_pose_active_timer_action_d;
    }
}

void obj_drift_left(struct ActorObj *obj)
{
    s32 x;
    s32 old = obj->posX;

    x = old - 0x200;
    obj->posX = x;
    if (x < 0)
        x += 0xFF;
    old = x >> 8;
    if (old < -0x20)
    {
        obj->posX = 0x10000;
        obj->state = wait_for_index;
    }
}

void obj_countdown_then_action1(struct ActorObj *obj)
{
    s32 t = obj->targetX;

    t--;
    obj->targetX = t;
    if (t <= 0)
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_action0_ext;
    }
}

void obj_pose_active_action0_ext(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = set_targets_from_record;
    }
}

void parts_obj_pose_active_timer_action_d(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 0xD, 0, 0);
            obj->state = obj_pose_active_action0_ext;
        }
    }
}

/* Once the current animation has finished: play action 4 with effect 0x1079 and launch a 0x40F1
   projectile from in front of the object. Variant 0 (param9C) is half size, plays sound 0x113 and
   checks for a hit through handler5C; variant 1 is full size, plays 0x114 and checks through
   handler60. The projectile flies towards the owner's target along a straight line
   (obj_slide_step_then_cull), starting from its spawn point (paramA0/paramA4). */
void obj_spawn_part_sprite(struct ActorObj *obj)
{
    struct ActorObj *shot;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        effect_spawn_at(0x1079, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        if (obj->param9C == 0)
        {
            set_slot_value(0x113, -1);
            shot = obj_spawn(obj->part, 0x40F1, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                             obj->baseH / 0x100 + 0xA, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
            ((struct PoseScale *)shot->pose)->scaleX = 0x80;
            ((struct PoseScale *)shot->pose)->scaleY = 0x80;
            obj->child = shot;
            obj_child_flags(obj, shot, 0x20);
            shot->handler68 = on_arrive;
            shot->unk6C = obj->unk6C;
            shot->owner = obj->owner;
            obj_list_push_default(shot);
            shot->handler5C = (void (*)(struct ActorObj *))obj_query_part_then_state;
        }
        else
        {
            set_slot_value(0x114, -1);
            shot = obj_spawn(obj->part, 0x40F1, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                             obj->baseH / 0x100 + 0x10, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
            ((struct PoseScale *)shot->pose)->scaleX = 0x100;
            ((struct PoseScale *)shot->pose)->scaleY = 0x100;
            obj->child = shot;
            obj_child_flags(obj, shot, 0x20);
            shot->handler68 = on_arrive;
            shot->unk6C = obj->unk6C;
            shot->owner = obj->owner;
            obj_list_push_default(shot);
            shot->handler60 = (void (*)(struct ActorObj *))obj_lookup_slot_then_state;
        }
        shot->tag = obj->tag;
        shot->timer = 0;
        sprite_set_action(shot, 1, 0, 0);
        shot->dx = 1;
        shot->targetX = obj->owner->part->targetX - shot->baseX;
        shot->targetY = obj->owner->part->targetY - shot->baseY;
        shot->param9C = obj->param9C;
        shot->paramA0 = shot->baseX;
        shot->paramA4 = shot->baseY;
        shot->state = obj_slide_step_then_cull;
        obj->state = obj_pose_active_action0_next;
    }
}

void obj_slide_step_then_cull(struct ActorObj *obj)
{
    s32 x;
    s32 delta;

    if (obj->pose->flags & 8)
    {
        if (obj->dx != 0)
        {
            sprite_set_action(obj, 0, 0, 0);
            obj->dx = 0;
        }
    }
    if (obj->param9C == 0)
        obj->posX += -0x219;
    else
        obj->posX += -0x133;
    delta = obj->baseX - obj->paramA0;
    obj->posY = obj->paramA4 + delta * obj->targetY / obj->targetX;
    x = obj->posX;
    if (x < 0)
        x += 0xFF;
    if ((x >> 8) < -0x20)
        obj_release(obj);
}

/*
 * Once obj's pose is done, throws a spinning projectile (object 0x40F2) from just in front of obj
 * at the owner's part: the child flies a parabola (obj_update_position_timer) from its spawn point to
 * 8 pixels right of the part's target, one frame per pixel of distance, peaking 0x40 pixels above
 * the start at half time, and is released when it lands. obj then waits in obj_pose_active_stop.
 */
void parts_obj_spawn_child_launch(struct ActorObj *obj)
{
    struct ActorObj *shot;
    s32 spanX;
    s32 spanY;
    s32 root;
    s32 duration;
    s32 halfTime;
    s32 steepness;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        shot = obj_spawn(obj->part, 0x40F2, obj->baseX / 0x100 - 9, obj->baseY / 0x100,
                         obj->baseH / 0x100 + 0xF, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
        obj->child = shot;
        obj_child_flags(obj, shot, 0x20);
        shot->handler68 = on_arrive;
        shot->unk6C = obj->unk6C;
        shot->owner = obj->owner;
        obj_list_push_default(shot);
        shot->handler60 = (void (*)(struct ActorObj *))obj_slot_check_action1;
        shot->tag = obj->tag;

        /* aim at the owner's part; dx/dy/dh keep the start in pixels */
        shot->targetX = obj->owner->part->targetX + 0x800;
        shot->targetY = obj->owner->part->targetY;
        shot->targetH = shot->baseH;
        shot->dx = shot->baseX / 0x100;
        shot->dy = shot->baseY / 0x100;
        shot->dh = shot->baseH / 0x100;
        spanX = shot->targetX - ((s32)shot->dx << 8);
        spanY = shot->targetY - ((s32)shot->dy << 8);
        /* gMathRoutines starts with the integer square root */
        root = ((s32 (*)(s32))gMathRoutines)(spanX * spanX + spanY * spanY);
        duration = root / 0x100;
        halfTime = duration / 2;
        steepness = 0x4000 / (halfTime * halfTime);
        if (steepness == 1)
            steepness = 2;
        shot->param9C = -steepness; /* curvature of the arc */
        shot->paramA0 = duration;
        shot->paramA4 = spanX;
        shot->paramA8 = spanY;
        shot->timer = 0;
        shot->state = obj_update_position_timer;
        set_slot_value(0x128, -1);
        obj->state = obj_pose_active_stop;
    }
}
