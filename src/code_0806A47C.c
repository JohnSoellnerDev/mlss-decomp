#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
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

extern u32 gUnk_03000E3C;

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 arg, u32 extra);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void start_motion(struct ActorObj *obj);
u32 mt19937_next(void);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_link_slot(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj, u32 a, u32 b);
u32 is_state_busy(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void wait_for_index(struct ActorObj *obj);
void actor_stop_when_pose_active(struct ActorObj *obj);
void actor_wait_ready_show_sprite(struct ActorObj *obj);
void actor_fly_then_land_ext(struct ActorObj *obj);
void actor_move_countdown_next(struct ActorObj *obj);
void actor_drift_left_release(struct ActorObj *obj);
void obj_set_action2_then_state(struct ActorObj *obj);
void jump_obj_wait_ready_action2(struct ActorObj *obj);
void obj_pose_active_move_to_owner(struct ActorObj *obj);
void obj_pose_active_move_to_base(struct ActorObj *obj);
void obj_timer_effect_then_state(struct ActorObj *obj);
void obj_timer_arc_setup(struct ActorObj *obj);
void obj_arc_descend(struct ActorObj *obj);
void obj_fall_to_idle(struct ActorObj *obj);
void obj_index_zero_timer(struct ActorObj *obj);
void obj_index_zero_timer30(struct ActorObj *obj);
void obj_timer_action6_idle(struct ActorObj *obj);
void obj_wait_busy_then_state(struct ActorObj *obj);
void obj_counter_motion_to_owner(struct ActorObj *obj);
void obj_timer_move_then_sprite(struct ActorObj *obj);
void obj_timer_move_part_target(struct ActorObj *obj);
void obj_timer_move_sprite(struct ActorObj *obj);
void obj_timer_place_sprite_then_wait(struct ActorObj *obj);
void obj_index_zero_random_wait(struct ActorObj *obj);
void obj_timer_place_sprite_index(struct ActorObj *obj);
void obj_timer_place_sprite_index_zero(struct ActorObj *obj);
void obj_spawn_part_child(struct ActorObj *obj);
void obj_index_zero_slot_timer(struct ActorObj *obj);
u32 obj_link_other_then_idle(struct ActorObj *obj, struct ActorObj *partner);
u32 obj_notify_lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 obj_link_owner_move_part(struct ActorObj *obj);
void obj_lookup_part_b_then_state(void);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_list_push_default(struct ActorObj *obj);
s32 resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void actor_release_when_free(struct ActorObj *obj);
void actor_timed_effect_then_child(struct ActorObj *obj);
void obj_land_pose_link(struct ActorObj *obj);
void obj_jump_arc_fly(struct ActorObj *obj);
void jump_obj_pose_active_idle(struct ActorObj *obj);
void obj_index_zero_clear_state(struct ActorObj *obj);
u32 obj_link_owner_then_descend(struct ActorObj *obj);
u32 obj_lookup_part_then_wait(struct ActorObj *obj, u32 a, u32 b);
void obj_list_remove(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void jump_obj_pose_active_clear_state(struct ActorObj *obj);
u32 obj_pair_link_state(struct ActorObj *obj);
u32 actor_lookup_or_remove(struct ActorObj *obj, u32 slot);

ASM_FUNC("asm/code_0806A47C/obj_jump_arc_land.s", void obj_jump_arc_land(void))

void obj_jump_setup(struct ActorObj *obj)
{
    s32 t = obj->timer;
    s32 px;
    s32 py;
    s32 spanX;
    s32 spanY;
    s32 dist;
    s32 duration;
    s32 half;
    s32 curve;
    s32 height;
    s32 sign;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        partner_link_update(obj, obj->owner, 0xFF);
        if (obj->targetY == 1)
        {
            sprite_set_action(obj, 5, 0x2000, 0);
        }
        else
        {
            sprite_set_action(obj, 5, 0x204D, 0);
        }
        height = 0x40;
        obj->targetH = height;
        px = obj->part->targetX;
        py = obj->part->targetY;
        obj->dx = obj->baseX / 0x100;
        obj->dy = obj->baseY / 0x100;
        spanX = px - (obj->dx << 8);
        spanY = py - (obj->dy << 8);
        dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(spanX * spanX + spanY * spanY);
        if (dist < 0)
        {
            dist += 0x3FF;
        }
        duration = dist >> 10;
        sign = (u32)dist >> 31;
        half = (duration + sign) >> 1;
        curve = (height << 8) / (half * half);
        if (curve == 1)
        {
            curve = 2;
        }
        obj->param9C = -curve;
        obj->paramA0 = duration;
        obj->paramA4 = spanX;
        obj->paramA8 = spanY;
        obj->timer = 0;
        obj->state = obj_jump_arc_fly;
    }
}

ASM_FUNC("asm/code_0806A47C/obj_jump_arc_fly.s", void obj_jump_arc_fly(struct ActorObj *obj))
