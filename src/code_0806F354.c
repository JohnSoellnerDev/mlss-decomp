#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 phase;
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
    u8 filler2[0xF6 - 0xEE];
    s16 cursorX;
    s16 cursorY;
    u8 filler3[0x104 - 0xFA];
    s16 unk104;
    u8 filler4[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
};

struct PoseBits
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 top : 1;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gUnk_03000FD8;
extern u32 gUnk_03000E3C;
extern u8 gUnk_03000E7C;
extern u8 gUnk_03000E7D;
extern const u8 gStepSequenceTable[];

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
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
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

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_arc_start(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_anim5_then_spawn(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_place_at_base_when_index_zero(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_timer_follow_owner(struct ActorObj *obj);
void obj_start_move_right(struct ActorObj *obj);
void obj_copy_bytes_sprite_88(struct ActorObj *obj);
void obj_init_pair_motion(struct ActorObj *obj);
void obj_anim6_wait4(struct ActorObj *obj);
void obj_move_to_part_x_offset(struct ActorObj *obj);
void obj_spawn_child_4115(struct ActorObj *obj);
void obj_spawn_child_4113(struct ActorObj *obj);
void obj_place_sprite_anim9(struct ActorObj *obj);
void obj_link_owner_go(struct ActorObj *obj);
void obj_lookup_slot_start(struct ActorObj *obj);
void obj_setup_wait_target(struct ActorObj *obj);
void obj_lookup_part_setup(struct ActorObj *obj);
void obj_fall_to_ground(struct ActorObj *obj);
void obj_wave_left(struct ActorObj *obj);
void obj_timer_to_move(struct ActorObj *obj);
void obj_wave_left_to_target(struct ActorObj *obj);
void obj_wave_rise_stop(struct ActorObj *obj);
void obj_wave_fall_stop(struct ActorObj *obj);
void obj_move_by_path(void);
void obj_count_down_exit(struct ActorObj *obj);
void obj_timer_table_next(struct ActorObj *obj);
void obj_link_then_finish(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_index_zero_count(struct ActorObj *obj);
void obj_notify_and_show(struct ActorObj *obj);
void obj_lookup_part_move(struct ActorObj *obj);
void obj_setup_part_motion(struct ActorObj *obj);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 mt19937_next(void);
void obj_list_remove(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_anim_a_wait60(struct ActorObj *obj);
void obj_countdown_then_anim_b(struct ActorObj *obj);
void obj_wait_then_spawn_child(struct ActorObj *obj);
void side_walk_obj_timer_spawn_effect(struct ActorObj *obj);
void obj_pose_release_partner(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);

ASM_FUNC("asm/code_0806F354/obj_spawn_child_arc.s", void obj_spawn_child_arc(void))

ASM_FUNC("asm/code_0806F354/obj_arc_step_clamped.s", void obj_arc_step_clamped(void))

ASM_FUNC("asm/code_0806F354/obj_countdown_place_and_setup.s", void obj_countdown_place_and_setup(void))

void obj_spawn_child_from_table(struct ActorObj *obj)
{
    struct ActorObj *partner;
    struct ActorObj *child;
    struct ActorPart *part;
    s32 index;

    index = gStepSequenceTable[gUnk_03000E7D + gUnk_03000E7C * 6];
    partner = (struct ActorObj *)(gUnk_03000FD8->partners[index + 6] + 8);
    part = obj->part;
    child = obj_spawn(part, 0x40E8, partner->part->targetX / 0x100,
                      partner->part->targetY / 0x100 + 4, partner->part->targetH / 0x100,
                      ((u32)obj->pose->unk11 << 25) >> 31, 1);
    obj->child = child;
    child->byte74 = obj->byte74;
    child->byte75 = obj->byte75;
    *(u8 *)&child->flags |= 1;
    child->state = 0;
    if (partner->part->targetY <= 0x67FF)
    {
        sprite_set_action(obj, 0xA, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 8, 0, 0);
    }
    obj->timer = 0x12;
    obj->state = obj_timer_table_next;
}

ASM_FUNC("asm/code_0806F354/obj_spawn_child_seq.s", void obj_spawn_child_seq(void))

ASM_FUNC("asm/code_0806F354/obj_arc_to_base_sprite.s", void obj_arc_to_base_sprite(void))
