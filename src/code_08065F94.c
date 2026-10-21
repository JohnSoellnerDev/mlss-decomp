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
    u8 flags;
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

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

// The child takes over the kind and two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child, u32 kind)
{
    ((struct Flags78Bits *)&child->dir)->kind = kind >> 5;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

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

void obj_lift_step_finish(struct ActorObj *obj);
u32 obj_lookup_slot_or_finish(struct ActorObj *obj);

void obj_spawn_linked_child(struct ActorObj *obj)
{
    struct ActorObj *child;
    struct ActorObj *walk;
    s32 diff;
    s32 t;
    s32 q;
    s32 half;
    s32 speed;

    child = obj_spawn(obj->part, 0x40A4, obj->baseX / 0x100, obj->baseY / 0x100 + 2,
                      obj->baseH / 0x100 + 0x20, ((u32)obj->pose->unk11 << 25) >> 31, 1);
    walk = obj->child;
    if (walk != NULL)
    {
        do
        {
            walk = walk->child;
        } while (walk != NULL);
    }
    obj_child_flags(obj, child, 0x20);
    child->handler68 = on_arrive;
    child->link = obj->link;
    child->owner = obj->owner;
    obj_list_push_default(child);
    child->handler60 = (s32 (*)(struct ActorObj *))obj_lookup_slot_or_finish;
    child->byte74 = obj->byte74;
    child->targetX = obj->owner->part->targetX;
    child->dx = child->baseX / 0x100;
    child->dh = child->baseH / 0x100;
    diff = ((s32)child->dx << 8) - child->targetX;
    t = diff;
    if (diff < 0)
    {
        t = diff + 0x1FF;
    }
    q = t >> 9;
    half = (q + (s32)((u32)t >> 31)) >> 1;
    speed = 0x3000 / (half * half);
    if (speed == 1)
    {
        speed = 2;
    }
    child->param9C = -speed;
    child->paramA0 = q;
    child->paramA4 = -diff;
    child->timer = 0;
    child->state = obj_lift_step_finish;
}
