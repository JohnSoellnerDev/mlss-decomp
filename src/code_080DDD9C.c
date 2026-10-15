#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 busy;
    u8 filler22[0x23 - 0x22];
    u8 record23;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 partner;
    u8 filler0EE[0x113 - 0xEE];
    u8 unk113;
};

struct ActorPoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u32 unk4;
    u32 unk8;
    u32 unkC;
};

struct ActorObj
{
    u8 filler0[0x04];
    struct SpriteNode *node4;
    struct ActorPose *pose;
    struct SpriteNode *node0C;
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x22 - 0x1C];
    u16 fxLife;
    u8 filler24[0x28 - 0x24];
    struct ActorPart *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct ActorObj *);
    u8 filler50[0x54 - 0x50];
    void (*savedState)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u16 progress;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x92 - 0x90];
    s16 unk92;
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 spawnLeft;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
    u8 filler0B4[0xC3 - 0xB4];
    s8 unkC3;
    s8 unkC4;
    s8 unkC5;
};

struct ObjFlags79Bit0
{
    u8 filler0[0x79];
    u8 bit0 : 1;
    u8 rest : 7;
};

struct ObjFlags79
{
    u8 filler0[0x79];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FxTask
{
    u8 filler0[4];
    void *unk4;
    void *unk8;
    u8 filler0C[0x10 - 0x0C];
    u16 counter;
    u8 filler12[0x18 - 0x12];
    u16 unk18;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FxRec
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 0x4];
    u16 unkE;
};

struct FxBit12
{
    u8 filler0[0x12];
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct FxBits1F
{
    u8 filler0[0x1F];
    u8 a : 2;
    u8 b : 2;
    u8 c : 2;
    u8 d : 2;
};

u32 is_state_busy(void);
void release_obj_131c(void *p);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void on_arrive(struct ActorObj *obj);
void obj_active_part_action_e_then_d43c(struct ActorObj *obj);
void obj_active_action_e_then_d090(struct ActorObj *obj);
void obj_active_pair_action_c_then_e2f8(struct ActorObj *obj);
void obj_pose_active_idle_arrive_b(struct ActorObj *obj);
void obj_float_offscreen_finish(struct ActorObj *obj);
void move_states_obj_pose_active_spawn_child(struct ActorObj *obj);
void set_slot_value(u32 id, s32 value);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_active_drop_to_ground_then_cf58(struct ActorObj *obj);
void obj_active_action_b_then_e2b4(struct ActorObj *obj);
void obj_drop_land_stop(struct ActorObj *obj);
void obj_drop_land_spawn_child(struct ActorObj *obj);
void obj_arrive_toggle_pose(struct ActorObj *obj);
void obj_timer_release_child_start_motion(struct ActorObj *obj);
void obj_partner_action_timer(struct ActorObj *obj);
void obj_timed_bounce_or_finish(struct ActorObj *obj);
void obj_begin_fall_to_owner(struct ActorObj *obj);
u32 obj_slot_found_recoil(struct ActorObj *obj, u32 slot);
void obj_settle_step_land_slot69(struct ActorObj *obj);
extern u8 *gUnk_03000FD8;
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void place_targets(struct ActorObj *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct ActorObj *obj, u32 offsets);
s32 find_frame_entry(struct ActorObj *obj, s32 index);
void obj_list_push_default(struct ActorObj *obj);
u32 link_slot_lookup_clear_11c(struct ActorObj *obj, u32 a, u32 b);
void obj_action8_slot126_timer(struct ActorObj *obj);
u32 obj_random_slot_link_then_next(struct ActorObj *obj, struct ActorObj *arg);
void change_item_count(u32 item, u32 count);
void obj_lower_height_512(struct ActorObj *obj);
extern u8 *gUnk_03000FF4;
u32 mt19937_next(void);
void start_motion(struct ActorObj *obj);
u32 sub_081DE850(u32 n, u32 r);
void sound_stop_song(u32 song);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_release(struct ActorObj *obj);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void *task_create_list_4c(void (*fn)(struct FxTask *), void *obj);
void fx_task_tick_release(struct FxTask *task);
void obj_spawn_aimed_child_pair(struct ActorObj *obj);
void obj_child_trail_update(struct ActorObj *obj);
void obj_begin_fall_to_owner_alt(struct ActorObj *obj);
struct SpriteNode *sprite_node_take(void);
void obj_list_push_arg(struct ActorObj *obj, u32 arg);
void obj_fall_spawn_fx_offscreen(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_active_spawn_then_e1fc(struct ActorObj *obj);
void obj_active_spawn_then_e278(struct ActorObj *obj);

static inline void pose_set_mode_one(struct ActorObj *obj)
{
    ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
}

static inline void obj_base_effect(struct ActorObj *obj)
{
    effect_spawn_at(0xCAC, obj->baseX / 0x100 - 0x14, obj->baseY / 0x100 + 1, obj->baseH / 0x100 + 0x10, obj->owner);
}

void obj_wait_free_then_df7c(struct ActorObj *obj);
void obj_wait_free_then_dfa8(struct ActorObj *obj);
void obj_wait_free_then_dfd4(struct ActorObj *obj);
void obj_wait_free_then_e000(struct ActorObj *obj);
void obj_action_d_pose_mode1_then_e02c(struct ActorObj *obj);
void obj_action_d_pose_mode1_then_e0b4(struct ActorObj *obj);
void obj_action_9_pose_mode1_then_e104(struct ActorObj *obj);
void obj_action_9_pose_mode1_then_e180(struct ActorObj *obj);

void obj_pose_active_idle_arrive(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}

void obj_float_offscreen_finish(struct ActorObj *obj)
{
    s32 pos[3];
    s32 v;

    obj->posX = obj->baseX + obj->dx;
    obj->posH = obj->baseH + obj->dh;
    obj->dh -= 0x5E;
    v = obj->posX;
    if (v < 0)
    {
        v += 0xFF;
    }
    pos[0] = v >> 8;
    v = obj->posY;
    if (v < 0)
    {
        v += 0xFF;
    }
    pos[1] = v >> 8;
    v = obj->posH;
    if (v < 0)
    {
        v += 0xFF;
    }
    pos[2] = v >> 8;
    offset_three_words_camera(&pos[0], &pos[1], &pos[2], 0);
    if (pos[0] < -16 || pos[1] > 272)
    {
        obj_release(obj);
    }
}

u32 obj_slot_found_recoil_kids(struct ActorObj *obj, u32 slot)
{
    u32 found = lookup_slot(obj, slot);

    if (found == 0)
    {
        obj->dx = -obj->dx / 2;
        obj->dy = found;
        obj->dh = found;
        if (obj->child != 0)
        {
            obj->child->timer = 8;
        }
        obj->state = obj_float_offscreen_finish;
        sprite_set_action(obj, 0, 0, 0);
    }
    return found;
}

u32 sprite_state_chain_d(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_df7c;
    return 1;
}

u32 sub_080DDE9C(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_dfa8;
    return 1;
}

u32 sub_080DDEA8(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_dfd4;
    return 1;
}

u32 sub_080DDEB4(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_e000;
    return 1;
}

u32 sub_080DDEC0(struct ActorObj *obj)
{
    void (*next)(struct ActorObj *);
    u32 r = sub_081DE850(4, mt19937_next());

    switch (r)
    {
    case 0:
        next = obj_wait_free_then_e000;
        break;
    case 1:
        next = obj_wait_free_then_dfd4;
        break;
    case 2:
        next = obj_wait_free_then_dfa8;
        break;
    case 3:
        next = obj_wait_free_then_df7c;
        break;
    default:
        return 1;
    }
    obj->state = next;
    return 1;
}

void obj_wait_free_then_df7c(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        obj->state = obj_action_d_pose_mode1_then_e02c;
    }
}

void obj_wait_free_then_dfa8(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        obj->state = obj_action_d_pose_mode1_then_e0b4;
    }
}

void obj_wait_free_then_dfd4(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        obj->state = obj_action_9_pose_mode1_then_e104;
    }
}

void obj_wait_free_then_e000(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        obj->state = obj_action_9_pose_mode1_then_e180;
    }
}

void obj_action_d_pose_mode1_then_e02c(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xD, 0, 0);
    pose_set_mode_one(obj);
    obj->state = obj_active_part_action_e_then_d43c;
}

void obj_action_d_pose_mode1_then_e0b4(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xD, 0, 0);
    pose_set_mode_one(obj);
    obj->state = obj_active_action_e_then_d090;
}

void obj_action_9_pose_mode1_then_e104(struct ActorObj *obj)
{
    sprite_set_action(obj, 9, 0, 0);
    pose_set_mode_one(obj);
    obj->state = obj_active_spawn_then_e1fc;
}

void obj_action_9_pose_mode1_then_e180(struct ActorObj *obj)
{
    sprite_set_action(obj, 9, 0, 0);
    pose_set_mode_one(obj);
    obj->state = obj_active_spawn_then_e278;
}

void obj_active_part_action_e_then_d43c(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, 0, 0, 0, 0x1400, 0x4CC, 0xFD, 0);
        set_slot_value(0x69, -1);
        sprite_set_action(obj, 0xE, 0, 0);
        pose_set_mode_one(obj);
        set_slot_value(0x117, -1);
        obj->state = obj_drop_land_spawn_child;
    }
}

void obj_active_action_e_then_d090(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        pose_set_mode_one(obj);
        set_slot_value(0x117, -1);
        obj->spawnLeft = 2;
        obj->state = move_states_obj_pose_active_spawn_child;
    }
}

void obj_active_spawn_then_e1fc(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj_base_effect(obj);
        set_slot_value(0x9D, -1);
        sprite_set_action(obj, 0xA, 0, 0);
        pose_set_mode_one(obj);
        obj->state = obj_active_drop_to_ground_then_cf58;
    }
}

void obj_active_spawn_then_e278(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj_base_effect(obj);
        sprite_set_action(obj, 0xA, 0, 0);
        set_slot_value(0x9D, -1);
        pose_set_mode_one(obj);
        obj->state = obj_active_action_b_then_e2b4;
    }
}

void obj_active_drop_to_ground_then_cf58(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->ground = obj->data->targetH;
        obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, 0, 0, 0, 0x1400, 0x4CC, 0xFD, 0);
        sprite_set_action(obj, 0xB, 0, 0);
        pose_set_mode_one(obj);
        obj->state = obj_drop_land_stop;
    }
}

void obj_active_action_b_then_e2b4(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        pose_set_mode_one(obj);
        obj->state = obj_active_pair_action_c_then_e2f8;
    }
}

void obj_active_pair_action_c_then_e2f8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj_spawn_aimed_child_pair(obj);
        obj->childValue = 0;
        sprite_set_action(obj, 0xC, 0, 0);
        pose_set_mode_one(obj);
        obj->state = obj_pose_active_idle_arrive_b;
    }
}

void obj_pose_active_idle_arrive_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}
