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
    s16 unkEC;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gUnk_03000FD8;

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
    u8 filler50[0x60 - 0x50];
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x74 - 0x64];
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
s32 unknown_part_steps_08072d24(struct ActorObj *obj);
s32 sub_08072D30(struct ActorObj *obj);
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

ASM_FUNC("asm/code_08072FA0/spawn_child_at_base.s", void spawn_child_at_base(struct ActorObj *obj))

void timer_then_target_from_owner(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        struct ActorPart *part;
        s32 flags;

        sprite_set_action(obj, 1, 0, 0);
        obj->targetX = (part = obj->owner->part)->targetX;
        obj->targetY = part->targetY;
        obj->targetH = part->targetH;
        flags = obj->flags79;
        flags |= 0x20;
        obj->flags79 = flags;
        obj->motionSpeed = 0x280;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = finish_when_index_zero;
    }
}

ASM_FUNC("asm/code_08072FA0/prepare_slide_timing.s", void prepare_slide_timing(struct ActorObj *obj))

ASM_FUNC("asm/code_08072FA0/slide_step.s", void slide_step(struct ActorObj *obj))

void wait_timer_then_settle(struct ActorObj *obj)
{
    s32 flags;

    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sprite_set_action(obj, 4, 0, 0);
            obj->targetX = obj->posX;
            obj->targetY = obj->posY;
            obj->targetH = obj->posH + 0x8000;
            flags = obj->flags79;
            flags |= 0x20;
            obj->flags79 = flags;
            obj->motionSpeed = 0x400;
            obj->motionStart = 0;
            start_motion(obj);
            set_slot_value(0xD3, -1);
            obj->state = notify_when_index_zero;
        }
    }
}

void start_slide_to_owner(struct ActorObj *obj)
{
    struct ActorPart *part;
    s32 dX;
    s32 dY;
    s32 dur;
    s32 q;

    sprite_set_action(obj, 5, 0, 0);
    obj->targetH = obj->param9C;
    obj->dx = obj->baseX / 0x100;
    obj->dy = obj->baseY / 0x100;
    obj->dh = obj->baseH / 0x100;
    part = obj->owner->part;
    dX = part->targetX - ((s16)obj->dx << 8);
    dY = part->targetY - ((s16)obj->dy << 8);
    dur = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dX * dX + dY * dY) / 0x300;
    q = ((s16)obj->dh << 8) / (dur * dur);
    if (q == 1)
    {
        q = 2;
    }
    obj->param9C = q;
    obj->paramA0 = dur;
    obj->paramA4 = dX;
    obj->paramA8 = dY;
    obj->timer = 0;
    ((struct Flags77Bits *)&obj->flags77)->bit6 = 0;
    ((struct Flags77Bits *)&obj->flags77)->bit5 = 0;
    obj_list_push_default(obj);
    obj->handler60 = lookup_then_slide;
    obj->state = slide_step_with_pose;
    obj->targetX = 5;
}

void slide_step_with_pose(struct ActorObj *obj)
{
    struct ActorPose *pose;
    s32 d;

    obj->targetX--;
    if (obj->targetX <= 0)
    {
        effect_spawn_at(0x2276, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->targetX = 5;
    }
    pose = obj->pose;
    pose->unkC = (-((s16)obj->timer << 14)) / obj->paramA0;
    obj->posX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    d = (s16)obj->timer - obj->paramA0;
    obj->posH = (d * d) * obj->param9C;
    obj->timer++;
    if ((s16)obj->timer > obj->paramA0)
    {
        obj->timer = 0xC000;
        obj->paramA0 = 0;
        obj->param9C = obj->targetH;
        obj->state = hover_bob_step;
    }
}

ASM_FUNC("asm/code_08072FA0/hover_bob_step.s", void hover_bob_step(struct ActorObj *obj))

void settle_to_data_target(struct ActorObj *obj)
{
    struct ActorPart *part;
    s32 index;
    s32 flags;

    obj->timer += 0x800;
    obj->pose->unkC = obj->timer;
    index = obj->index;
    if (index == 0)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->pose->unkC = 0x8000;
        obj->targetX = (part = obj->part)->targetX;
        obj->targetY = part->targetY;
        obj->targetH = part->targetH + 0x2000;
        flags = obj->flags79;
        flags |= 0x20;
        obj->flags79 = flags;
        obj->motionSpeed = 0x500;
        obj->motionStart = index;
        start_motion(obj);
        set_slot_value(0x8B, -1);
        obj->state = link_owner_then_wait;
    }
}

void step_child_chain(struct ActorObj *obj)
{
    struct ChainNode *node = (struct ChainNode *)((u8 *)obj + 0x0C);
    s32 dX = ((s16)obj->dx << 8) - obj->targetX;
    s32 dY = ((s16)obj->dy << 8) - obj->targetY;
    s32 dH = (((s16)obj->dh + 8) << 8) - obj->targetH;
    s32 unit = 0x100;
    s32 i = 0;

    while (node != 0)
    {
        if (gUnk_03000E80[i] != 0)
        {
            s32 v;

            node->x = obj->targetX + (gUnk_03000E80[i] * dX) / unit;
            node->y = obj->targetY + (gUnk_03000E80[i] * dY) / unit;
            node->h = obj->targetH + (gUnk_03000E80[i] * dH) / unit;
            v = gUnk_03000E80[i] + 0xC;
            gUnk_03000E80[i] = v;
            if ((u16)v > 0xFF)
            {
                gUnk_03000E80[i] = 0;
                obj->timer = 1;
            }
        }
        else
        {
            node->x = 0x10000;
            node->y = 0x10000;
            node->h = 0x10000;
        }
        node = node->next;
        i++;
    }
}

void step_child_chain_b(struct ActorObj *obj)
{
    struct ChainNode *node = (struct ChainNode *)((u8 *)obj + 0x0C);
    s32 dX = ((s16)obj->dx << 8) - obj->targetX;
    s32 dY = ((s16)obj->dy << 8) - obj->targetY;
    s32 dH = (((s16)obj->dh + 8) << 8) - obj->targetH;
    s32 unit = 0x100;
    s32 i = 0;

    while (node != 0)
    {
        if (gUnk_03000E98[i] != 0)
        {
            s32 v;

            node->x = obj->targetX + (gUnk_03000E98[i] * dX) / unit;
            node->y = obj->targetY + (gUnk_03000E98[i] * dY) / unit;
            node->h = obj->targetH + (gUnk_03000E98[i] * dH) / unit;
            v = gUnk_03000E98[i] + 0xC;
            gUnk_03000E98[i] = v;
            if ((u16)v > 0xFF)
            {
                gUnk_03000E98[i] = 0;
                obj->timer = 1;
            }
        }
        else
        {
            node->x = 0x10000;
            node->y = 0x10000;
            node->h = 0x10000;
        }
        node = node->next;
        i++;
    }
}
