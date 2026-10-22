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
    u8 filler0EE[0xF6 - 0xEE];
    s16 cursorAX;
    s16 cursorAY;
    u8 filler0FA[0xFC - 0xFA];
    s16 cursorBX;
    s16 cursorBY;
    u8 filler100[0x113 - 0x100];
    u8 unk113;
    u8 filler114[0x130 - 0x114];
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

struct ObjFlags76
{
    u8 filler0[0x76];
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
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
extern u8 *gWorld;
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
extern u8 *gParty;
u32 mt19937_next(void);
void start_motion(struct ActorObj *obj);
u32 rand_below(u32 n, u32 r);
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
void obj_slot_spawn_child_anim9(struct ActorObj *obj);
void ensure_code_entry(u32 code);
extern u8 gItemTable[];

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
void obj_rebuild_after_motion(struct ActorObj *obj);

NONMATCH("asm/code_080DE320/obj_pair_midpoint_move_then_f3b4.s", void obj_pair_midpoint_move_then_f3b4(struct ActorObj *obj))
{
    struct ActorPart *data = obj->owner->data;
    struct ActorPart *a;
    struct ActorPart *b;

    if (data->partner == -1)
    {
        a = data;
        b = data + 1;
    }
    else
    {
        a = data - 1;
        b = data;
    }
    if (a->partner != 0)
    {
        if (b->partner != 0)
        {
            obj->targetX = (a->targetX + b->targetX) / 2;
            obj->targetY = (a->targetY + b->targetY) / 2;
        }
        else
        {
            obj->targetX = a->targetX;
            obj->targetY = a->targetY;
        }
    }
    else
    {
        obj->targetX = b->targetX;
        obj->targetY = b->targetY;
    }
    obj->targetH = obj->owner->data->targetH;
    obj->targetX += 0x4000;
    ((struct ObjFlags79 *)obj)->bit5 = 1;
    obj->motionSpeed = obj->paramA0 << 8;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x11C, -1);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_partner_action_timer;
}
END_NONMATCH

void obj_timer_move_to_owner_then_f358(struct ActorObj *obj)
{
    struct ActorPart *data;
    s32 t = obj->timer - 1;

    obj->timer = t;
    if ((s16)t < 0)
    {
        obj_list_push_default(obj);
        obj->handler58 = (void (*)(struct ActorObj *))obj_random_slot_link_then_next;
        obj->handler60 = (void (*)(struct ActorObj *))link_slot_lookup_clear_11c;
        obj->targetX = (data = obj->owner->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x199;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 7, 0, 0);
        set_slot_value(0x11C, -1);
        obj->state = obj_action8_slot126_timer;
    }
}

NONMATCH("asm/code_080DE320/obj_timer_pair_move_then_e640.s", void obj_timer_pair_move_then_e640(struct ActorObj *obj))
{
    s32 kc = 0x41;
    struct ActorPart *data;
    struct ActorPart *a;
    struct ActorPart *b;
    struct ActorPose *pose;
    u8 *code;
    u8 *entry;
    s32 t = obj->timer - 1;
    s32 flags;
    s32 bit;

    obj->timer = t;
    if ((s16)t < 0)
    {
        sound_stop_song(0x126);
        obj->owner->state = 0;
        data = obj->owner->data;
        if (data->partner == -1)
        {
            a = data;
            b = data + 1;
        }
        else
        {
            a = data - 1;
            b = data;
        }
        if (a->partner != 0)
        {
            if (b->partner != 0)
            {
                obj->targetX = (a->targetX + b->targetX) / 2;
                obj->targetY = (a->targetY + b->targetY) / 2;
            }
            else
            {
                obj->targetX = a->targetX;
                obj->targetY = a->targetY;
            }
        }
        else
        {
            obj->targetX = b->targetX;
            obj->targetY = b->targetY;
        }
        obj->targetH = obj->owner->data->targetH;
        obj->targetX += 0x4000;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        pose = obj->pose;
        flags = pose->byte11;
        bit = ((u32)flags << 25) >> 31;
        bit = bit ^ 1;
        bit = bit << 6;
        flags = -kc & flags;
        flags = flags | bit;
        pose->byte11 = flags;
        obj->state = obj_slot_spawn_child_anim9;
        code = &obj->data->unk113;
        if (*code & 0x80)
        {
            if (*code != 0xFF)
            {
                entry = gItemTable + ((*code & 0x7F) << 4);
                obj->spawnLeft = (entry[2] & 7) + 0x4188;
                ensure_code_entry(obj->spawnLeft);
            }
        }
    }
}
END_NONMATCH

void obj_slot_spawn_child_anim9(struct ActorObj *obj)
{
    struct ActorObj *child;

    if (is_state_busy())
    {
        return;
    }
    if (obj->index != 0)
    {
        return;
    }
    if (obj->data->unk113 == 0xFF)
    {
        obj->data->unk113 = obj->index;
        sprite_set_action(obj, 0x13, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 9, 0, 0);
        set_slot_value(0xA3, -1);
        child = obj_spawn(obj->data, *(u16 *)&obj->spawnLeft, obj->baseX / 0x100 + 2, obj->baseY / 0x100 + 1,
                          obj->baseH / 0x100 + 0x27, (((u32)obj->pose->byte11 << 25) >> 31), 1);
        obj->child = child;
        ((struct ObjFlags79Bit0 *)child)->bit0 = 1;
        child->owner = obj->owner;
        child->byte74 = 0xFF;
        sprite_set_action(child, 0, 0, 0);
    }
    obj->timer = 0x30;
    obj->state = obj_timer_release_child_start_motion;
}

void obj_timer_release_child_start_motion(struct ActorObj *obj)
{
    struct ActorPart *data;
    s32 t = obj->timer - 1;

    obj->timer = t;
    if ((s16)t < 0)
    {
        if (obj->child != 0)
        {
            obj_release(obj->child);
            obj->child = 0;
        }
        obj->targetX = (data = obj->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_arrive_toggle_pose;
    }
}

u32 obj_random_slot_link_then_next(struct ActorObj *obj, struct ActorObj *arg)
{
    s32 slot = -1;
    u32 base = 0;
    u8 flag;

    flag = gParty[0x8A];
    if (flag != 0 && flag != 0xFF)
    {
        base = 0x46;
        if (rand_below(base, mt19937_next()) <= 0x45)
        {
            slot = 0;
        }
    }
    flag = gParty[0x8B];
    if (flag != 0 && flag != 0xFF)
    {
        base += 0x19;
        if (rand_below(base, mt19937_next()) <= 0x18)
        {
            slot = 1;
        }
    }
    flag = gParty[0x8C];
    if (flag != 0 && flag != 0xFF)
    {
        base += 5;
        if (rand_below(base, mt19937_next()) <= 4)
        {
            slot = 2;
        }
    }
    if (slot != -1)
    {
        change_item_count((u8)slot, -1);
        obj->data->unk113 = slot | -0x80;
    }
    else
    {
        obj->data->unk113 = 0xFF;
    }
    if (obj->owner->handler68 != 0)
    {
        obj->owner->handler68(obj->owner);
    }
    arg->state = obj_lower_height_512;
    return 1;
}

NONMATCH("asm/code_080DE320/obj_spawn_linked_child_jump.s", void obj_spawn_linked_child_jump(struct ActorObj *obj))
{
    u8 *world = gWorld;
    struct ActorObj *partner = obj->owner;
    struct ActorObj *child;
    u8 *entry;
    s32 ofs;
    s32 prod;

    entry = *(u8 **)(world + 0x2DC);
    child = obj_spawn_linked(obj, entry[2] + 0x4188, 0);
    child->baseH += (s8)obj->unkC5 << 8;
    child->posX = child->baseX;
    child->posY = child->baseY;
    child->posH = child->baseH;
    ((struct Flags78Bits *)&child->dir)->kind = 1;
    child->owner = obj->owner;
    child->motionSpeed = obj->motionSpeed;
    if (((struct ObjFlags76 *)child)->mode == 1 || ((struct ObjFlags76 *)child)->mode == 2)
    {
        place_targets(child, partner->baseX / 0x100 + (s8)partner->unkC3,
                      partner->baseY / 0x100 + (s8)partner->unkC4,
                      partner->baseH / 0x100 + (s8)partner->unkC5, -1);
        if (partner == (struct ActorObj *)((u8 *)partner->data + 8))
        {
            ofs = (s8)partner->unkC5;
        }
        else
        {
            find_frame_entry(partner, 0);
            ofs = (s8)partner->unk47;
        }
        ofs += partner->baseH / 0x100;
        prod = ofs * ofs;
        ofs += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x640, prod / 0x40 + 0x28);
        child->unk92 = ofs;
        set_motion_offsets(child, 0x480);
    }
    child->state = obj_rebuild_after_motion;
}
END_NONMATCH

ASM_FUNC("asm/code_080DE320/obj_rebuild_after_motion.s", void obj_rebuild_after_motion(struct ActorObj *obj))
