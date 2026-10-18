#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x04];
    u16 unk4;
    u8 filler6[0x0C - 0x06];
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
    s16 unkEC;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gWorld;

struct Flags77Bits
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags79Bits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ChainNode
{
    struct ChainNode *next;
    s32 x;
    s32 y;
    s32 h;
};

extern u16 gUnk_03000E80[];
extern u16 gUnk_03000E98[];

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
    u32 (*handler58)();
    s32 (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 field6C;
    u8 filler70[0x74 - 0x70];
    u8 group;
    u8 depth;
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
    u16 dx;
    s16 dy;
    s16 dh;
};

u32 sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_link_slot(struct ActorObj *obj);
u32 mt19937_next(void);
void anim_bit200_step(struct ActorPart *part, u32 value);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_list_remove(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void lookup_then_slide(struct ActorObj *obj);
void spawn_child_and_arm(struct ActorObj *obj);
void spawn_slot_sprite(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void start_motion(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void finish_when_index_zero(struct ActorObj *obj);
void wait_timer_then_place(struct ActorObj *obj);
void link_owner_then_wait(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_detach_active(struct ActorPose *pose);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void wait_counter_then_finish(struct ActorObj *obj);
void notify_when_index_zero(struct ActorObj *obj);

void reset_height_motion(struct ActorObj *obj);
void spawn_child_and_place(struct ActorObj *obj);
void place_and_move_fixed(struct ActorObj *obj);
void timer_spawn_grandchild(struct ActorObj *obj);
void obj_fall_and_split(struct ActorObj *obj);
void anim_track_slot(struct ActorObj *obj);
void move_by_speed_xy(struct ActorObj *obj);
void move_by_speed_then_land(struct ActorObj *obj);
void scroll_obj_out(struct ActorObj *obj);
u32 roll_action_chance(struct ActorObj *obj, struct ActorObj *target);
u32 bind_part_then_step(struct ActorObj *obj, u32 part);
void scroll_obj_out_and_free(struct ActorObj *obj);
void timer_then_step_2b35(struct ActorObj *obj);
void timer_30_when_idle(struct ActorObj *obj);
void count_dx_and_notify(struct ActorObj *obj);
u32 lookup_part_b_then_step(struct ActorObj *obj);
void remove_child_and_move(struct ActorObj *obj);
void timer_then_anim5(struct ActorObj *obj);
s32 arm_slot_anim_2f_then_follow(struct ActorObj *obj);
s32 arm_slot_anim_2f_then_step_2e0d(struct ActorObj *obj);
void slot_anim_2f_then_follow(struct ActorObj *obj);
void slot_anim_2f_then_step_2e0d(struct ActorObj *obj);
void slot_follow_move(struct ActorObj *obj);
void slide_to_anchor_start(struct ActorObj *obj);
void action16_when_index_zero(struct ActorObj *obj);
void begin_fx_wait(struct ActorObj *obj);
void advance_offset_then_notify(struct ActorObj *obj);
void pose_action15_then_exit(struct ActorObj *obj);
void spawn_child_at_base(struct ActorObj *obj);
void timer_then_target_from_owner(struct ActorObj *obj);
void prepare_slide_timing(struct ActorObj *obj);
void slide_step(struct ActorObj *obj);
void wait_timer_then_settle(struct ActorObj *obj);
void start_slide_to_owner(struct ActorObj *obj);
void slide_step_with_pose(struct ActorObj *obj);
void hover_bob_step(struct ActorObj *obj);
void settle_to_data_target(struct ActorObj *obj);
void step_child_chain(struct ActorObj *obj);
void step_child_chain_b(struct ActorObj *obj);

void reset_height_motion(struct ActorObj *obj)
{
    struct ActorPart *part;
    s32 flags;

    obj->posH = 0;
    sprite_set_action(obj, 5, 0x40AE, 0);
    obj->depth = obj->owner->depth;
    obj->targetX = (part = obj->part)->targetX;
    obj->targetY = part->targetY;
    obj->targetH = obj->posH;
    obj->motionSpeed = 0x200;
    flags = obj->flags79;
    flags |= 0x20;
    obj->flags79 = flags;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 8;
    obj->param9C = 2;
    obj->state = wait_timer_then_place;
}

void spawn_child_and_place(struct ActorObj *obj)
{
    struct ActorObj *child;
    struct ActorObj *slot;
    s32 zero;
    s32 flags;

    if (obj->pose->flags & 8)
    {
        slot = (struct ActorObj *)(gWorld->partners[0] + 8);
        obj_detach_active(slot->pose);
        child = obj_spawn(obj->part, 0x40B8, obj->baseX / 0x100, obj->baseY / 0x100,
                          obj->baseH / 0x100, (((u32)obj->pose->unk11 << 25) >> 31), 1);
        obj->child = child;
        child->group = obj->group;
        zero = 0;
        child->depth = obj->depth - 1;
        ((struct Flags79Bits *)&child->flags79)->bit3 = 1;
        child->state = (void (*)(struct ActorObj *))zero;
        sprite_set_action(obj, 2, 0x40B7, 0);
        sprite_set_action(obj->child, 2, 0, 0);
        effect_spawn_at(0x21F2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = count_dx_and_notify;
        obj->dx = zero;
    }
}

void place_and_move_fixed(struct ActorObj *obj)
{
    s32 flags;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0x40B7, 0);
        sprite_set_action(obj->child, 4, 0, 0);
        effect_spawn_at(0x21FB, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->targetX = 0xA000;
        obj->targetY = 0x6800;
        obj->targetH = 0xA000;
        obj->motionSpeed = 0x500;
        flags = obj->flags79;
        flags |= 0x20;
        obj->flags79 = flags;
        obj->motionStart = 0;
        start_motion(obj);
        set_slot_value(0x69, -1);
        obj->state = timer_30_when_idle;
    }
}

void timer_spawn_grandchild(struct ActorObj *obj)
{
    struct ActorObj *spawned;
    s32 flags;

    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        sprite_set_action(obj, 5, 0x40B7, 0);
        sprite_set_action(obj->child, 5, 0, 0);
        obj->param9C = 0x66;
        obj->paramA0 = 0x33;
        spawned = obj_spawn(obj->part, 0x40BB, obj->baseX / 0x100, obj->baseY / 0x100,
                            obj->baseH / 0x100, (((u32)obj->pose->unk11 << 25) >> 31), 1);
        obj->child->child = spawned;
        spawned->group = obj->group;
        spawned->depth = obj->depth + 0xA;
        ((struct Flags79Bits *)&spawned->flags79)->bit3 = 0;
        spawned->state = 0;
        obj->state = obj_fall_and_split;
    }
}

ASM_FUNC("asm/code_0807213C/obj_fall_and_split.s", void obj_fall_and_split(struct ActorObj *obj))

ASM_FUNC("asm/code_0807213C/anim_track_slot.s", void anim_track_slot(struct ActorObj *obj))

ASM_FUNC("asm/code_0807213C/move_by_speed_xy.s", void move_by_speed_xy(struct ActorObj *obj))

ASM_FUNC("asm/code_0807213C/move_by_speed_then_land.s", void move_by_speed_then_land(struct ActorObj *obj))

void scroll_obj_out(struct ActorObj *obj)
{
    s32 accel = obj->paramA0 + 0x14;
    s32 x;
    s32 px;

    obj->paramA0 = accel;
    if (obj->param9C == 0)
    {
        x = obj->posX - accel;
        obj->posX = x;
        if (x < 0)
        {
            x += 0xFF;
        }
        px = x >> 8;
        if (px > -0x20)
        {
            goto done;
        }
        obj_list_remove(obj);
    }
    else
    {
        x = obj->posX + accel;
        obj->posX = x;
        if (x < 0)
        {
            x += 0xFF;
        }
        if ((x >> 8) <= 0xFF)
        {
            goto done;
        }
    }
    obj_release(obj);
done:;
}

u32 roll_action_chance(struct ActorObj *obj, struct ActorObj *target)
{
    u8 roll;

    roll = mt19937_next() % 100;
    if (roll <= 0x32 && target->part->unkF6 != 0)
    {
        anim_bit200_step(target->part, 0xC8);
        sprite_set_action(target, 0, 0, 0);
    }
    obj->state = scroll_obj_out;
}
