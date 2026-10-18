#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 cmd;
    u8 unk21;
};

struct PoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct Flags77Bits
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ActorPart
{
    u8 filler0[0x40];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler1[0xB4 - 0x4C];
    u16 unkB4;
    u8 filler2[0xD8 - 0xB6];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler3[0xEC - 0xE4];
    s16 slot;
    u8 filler4[0xF6 - 0xEE];
    s16 timerF6;
    u8 filler0F8[0x102 - 0xF8];
    s16 unk102;
    s16 unk104;
    u8 filler106[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
    u8 filler10E[0x114 - 0x10E];
    s32 unk114;
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
    void (*savedState)(struct ActorObj *);
    u8 filler54[0x58 - 0x54];
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
    u8 filler90[0x94 - 0x90];
    s32 acc94;
    s32 unk98;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

struct BodyData
{
    u8 filler0[0x102];
    s16 unk102;
    s16 unk104;
    u8 filler106[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
    u8 filler10E[0x114 - 0x10E];
    u32 unk114;
};

void apply_partner_effect(struct ActorObj *a, struct ActorObj *b, s32 strength, s32 c, s32 x, s32 z, s32 d);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
u32 lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void motion_timed(struct ActorObj *obj, u32 kind, u32 value);
void obj_list_push_default(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void spawn_pose_child(struct ActorObj *obj);
void setup_motion_from_owner(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void begin_countdown(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void start_jump(struct ActorObj *obj);
void set_cur_flags_both(s32 flags);
void await_child_pose(struct ActorObj *obj);
void turn_and_arrive(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void approach_target(struct ActorObj *obj);
s32 move_step(s16 *outX, s16 *outY, s16 *outH, s32 dx, s32 dy, s32 dh, s32 speed);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
u32 query_contact(struct ActorObj *obj, u32 other, u32 kind);
void fly_step_cull(struct ActorObj *obj);

void timed_move_3800(struct ActorObj *obj);
void set_target_from_records(struct ActorObj *obj);
void spawn_child_with_offsets(struct ActorObj *obj);
void offset_and_move_child(struct ActorObj *obj);
s32 contact_step_79e4(struct ActorObj *body, struct ActorObj *other, s32 arg);
u32 contact_step_75bc(struct ActorObj *obj, struct ActorObj *other);
void tick_timer_then_move(struct ActorObj *obj);
void setup_motion_with_callbacks(struct ActorObj *obj);
void timed_move_3400(struct ActorObj *obj);
void drift_step(struct ActorObj *obj);
void wait_then_spawn_part(struct ActorObj *obj);
void drift_settle(struct ActorObj *obj);
void wait_then_spawn_part_short(struct ActorObj *obj);
void drift_settle_short(struct ActorObj *obj);
void count_down_then_idle(struct ActorObj *obj);
void bounce_step(struct ActorObj *obj);
s32 part_step_start_drift(struct ActorObj *obj);
u32 find_link_slot_6c(struct ActorObj *obj, u32 a, u32 b);
u32 find_part_6c(struct ActorObj *obj, u32 a, u32 b);
u32 link_pair_6c(struct ActorObj *obj, u32 a, u32 b);
s32 enter_start_move_3400(struct ActorObj *obj);
void height_step_finish(struct ActorObj *obj);
void hit_react(struct ActorObj *obj);
void wait_pose_then_index(struct ActorObj *obj);
void start_motion_by_owner_slot(struct ActorObj *obj);
s32 part_step_set_target(struct ActorObj *obj);
s32 part_step_start_move_3800(struct ActorObj *obj);
s32 part_step_start_move_2800(struct ActorObj *obj);
u32 approach_slot(struct ActorObj *obj, u32 other);
u32 approach_contact(struct ActorObj *obj, u32 other, u32 kind);
void slide_x_timed(struct ActorObj *obj);
void on_arrive_if_posed(struct ActorObj *obj);
s32 arm_spawn_after_motion(struct ActorObj *obj);
s32 part_step_start_move_3000(struct ActorObj *obj);
s32 part_step_move_toward_owner(struct ActorObj *obj);
s32 part_step_clear_state(struct ActorObj *obj);
s32 part_step_noop(struct ActorObj *obj);
s32 part_step_prep_idle(struct ActorObj *obj);
void start_drift(struct ActorObj *obj);
void start_move_3400(struct ActorObj *obj);
void owner_slot_actions(struct ActorObj *obj);
void start_move_3800(struct ActorObj *obj);
void start_move_2800(struct ActorObj *obj);
void fly_step_cull(struct ActorObj *obj);
void slide_left_cull(struct ActorObj *obj);
void start_move_3000(struct ActorObj *obj);
void move_toward_owner(struct ActorObj *obj);
void state_clear_0808a8c8(struct ActorObj *obj);
void noop_state_b(struct ActorObj *obj);
void prep_idle(struct ActorObj *obj);
void arm_timed_motion_art3(struct ActorObj *obj);
void arm_callbacks_then_jump(struct ActorObj *obj);
void arm_callbacks_then_countdown(struct ActorObj *obj);
u32 find_link_slot_81(struct ActorObj *obj, u32 a, u32 b);
u32 find_part_81(struct ActorObj *obj, u32 a, u32 b);
u32 link_pair_81(struct ActorObj *obj, u32 a, u32 b);
void arm_timed_motion_art6(struct ActorObj *obj);
void noop_state(struct ActorObj *obj);
void prepare_jump(struct ActorObj *obj);
void spawn_child_405a(struct ActorObj *obj);
void finish_child_and_move(struct ActorObj *obj);
void spawn_child_4059(struct ActorObj *obj);
void spawn_child_4059_b(struct ActorObj *obj);
void release_then_move(struct ActorObj *obj);

void timed_move_3800(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        s32 timer = obj->timer - 1;

        obj->timer = timer;
        if ((s16)timer <= 0)
        {
            obj_list_remove(obj);
            sound_stop_song(0x81);
            obj->targetX = obj->baseX + 0x3800;
            obj->targetY = obj->baseY;
            obj->targetH = obj->baseH;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 2, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
            obj->state = wait_for_index;
        }
    }
}

void set_target_from_records(struct ActorObj *obj)
{
    struct ActorObj *owner = obj->owner;
    struct ActorPart *data = owner->part;
    struct ActorPart *a;
    struct ActorPart *b;

    if (data->slot == -1)
    {
        b = data;
        a = (struct ActorPart *)((u8 *)data + 0x130);
    }
    else if (data->slot == -2)
    {
        b = (struct ActorPart *)((u8 *)data - 0x130);
        a = data;
    }
    if (b->slot != 0)
    {
        if (a->slot != 0)
        {
            obj->targetX = (b->targetX + a->targetX) / 2 + obj->paramA4 * 0x100;
            obj->targetY = (b->targetY + a->targetY) / 2 + 0x200;
        }
        else
        {
            obj->targetX = b->baseX + obj->paramA4 * 0x100;
            obj->targetY = b->baseY + 0x200;
        }
    }
    else
    {
        obj->targetX = a->baseX + obj->paramA4 * 0x100;
        obj->targetY = a->baseY + 0x200;
    }
    obj->targetH = owner->part->targetH;
    *((u8 *)obj + 0x79) |= 0x20;
    obj->motionSpeed = 0x600;
    obj->motionStart = 0;
    start_motion(obj);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = start_motion_by_owner_slot;
}

ASM_FUNC("asm/code_0808962C/spawn_child_with_offsets.s", void spawn_child_with_offsets(struct ActorObj *obj))

ASM_FUNC("asm/code_0808962C/offset_and_move_child.s", void offset_and_move_child(struct ActorObj *obj))

s32 contact_step_79e4(struct ActorObj *body, struct ActorObj *other, s32 arg)
{
    s32 contact = query_contact(body, (u32)other, arg);

    if (contact == 0)
    {
        struct BodyData *otherData = (struct BodyData *)other->part;
        s32 otherWord = otherData->unk114;
        s32 otherW = otherData->unk102;
        u16 otherH = otherData->unk10A;
        struct BodyData *bodyData = (struct BodyData *)body->part;
        s32 bodyW = bodyData->unk104;
        u16 bodyH = bodyData->unk10C;
        s32 otherProd;
        s32 strength;
        s32 bodyScale;

        strength = otherW * 0x66;
        otherProd = strength * otherH;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength = otherProd >> 8;
        bodyScale = bodyW * 3;
        bodyScale = bodyScale + (bodyScale << 4);
        otherProd = bodyH * bodyScale;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength -= otherProd >> 8;
        strength /= 2;
        if (otherWord > 0x100)
        {
            s32 scaled = otherWord * strength;

            strength = scaled;
            if (strength < 0)
            {
                scaled = strength + 0xFFFF;
            }
            strength = scaled >> 16;
        }
        else
        {
            s32 scaled = strength;

            if (strength < 0)
            {
                scaled += 0xFF;
            }
            strength = scaled >> 8;
        }
        if (strength <= 1)
        {
            strength = 1;
        }
        apply_partner_effect(other, body, strength, 0, other->posX / 0x100, other->posH / 0x100, 0);
        hit_react(body);
        *(u16 *)((u8 *)body + 0xAC) = 8;
    }
    return contact;
}

u32 contact_step_75bc(struct ActorObj *obj, struct ActorObj *other)
{
    s32 contact = lookup_slot(obj, (u32)other);

    if (contact == 0)
    {
        struct ActorPart *otherPart = other->part;
        s32 otherWord = otherPart->unk114;
        s32 otherW = otherPart->unk102;
        u16 otherH = otherPart->unk10A;
        struct ActorPart *objPart = obj->part;
        s32 objW = objPart->unk104;
        u16 objH = objPart->unk10C;
        s32 otherProd;
        s32 strength;
        s32 objScale;

        strength = otherW * 0x66;
        otherProd = strength * otherH;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength = otherProd >> 8;
        objScale = objW * 3;
        objScale = objScale + (objScale << 4);
        otherProd = objH * objScale;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength -= otherProd >> 8;
        strength /= 2;
        if (otherWord > 0x100)
        {
            s32 scaled = otherWord * strength;

            strength = scaled;
            if (strength < 0)
            {
                scaled = strength + 0xFFFF;
            }
            strength = scaled >> 16;
        }
        else
        {
            s32 scaled = strength;

            if (strength < 0)
            {
                scaled += 0xFF;
            }
            strength = scaled >> 8;
        }
        if (strength <= 1)
        {
            strength = 1;
        }
        apply_partner_effect(other, obj, strength, 0, other->posX / 0x100, other->posH / 0x100, 0);
        hit_react(obj);
        obj->timer = 8;
    }
    return contact;
}

void tick_timer_then_move(struct ActorObj *obj)
{
    u16 t = obj->timer;

    if ((s16)obj->timer != 0)
    {
        obj->timer = t - 1;
    }
    else
    {
        u8 flag = obj->pose->unk21;

        if (flag == 0)
        {
            struct ActorPart *part = obj->part;

            obj->targetX = part->baseX - 0xA00;
            obj->targetY = part->baseY;
            obj->targetH = part->baseH + 0xD00;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = flag;
            start_motion(obj);
            obj->dy = part->baseH + 0x680;
            obj->dh = flag;
            obj->state = height_step_finish;
        }
    }
}

void setup_motion_with_callbacks(struct ActorObj *obj)
{
    struct ActorObj *owner = obj->owner;

    obj->targetX = owner->part->targetX;
    obj->targetY = owner->part->targetY;
    obj->targetH = obj->baseH;
    ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
    obj->motionSpeed = 0x200;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 0xA;
    obj_list_push_default(obj);
    obj->handler58 = (void (*)(struct ActorObj *))link_pair_6c;
    obj->handler5C = (void (*)(struct ActorObj *))find_part_6c;
    obj->handler60 = (void (*)(struct ActorObj *))find_link_slot_6c;
    sprite_set_action(obj, 4, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->state = timed_move_3400;
}

void timed_move_3400(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        s32 timer = obj->timer - 1;

        obj->timer = timer;
        if ((s16)timer <= 0)
        {
            sound_stop_song(0x6C);
            obj_list_remove(obj);
            obj->targetX = obj->baseX + 0x3400;
            obj->targetY = obj->baseY;
            obj->targetH = obj->baseH;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 2, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
            obj->state = wait_for_index;
        }
    }
}

void drift_step(struct ActorObj *obj)
{
    obj->dh += obj->acc94;
    obj->posX += obj->dx;
    obj->posY += obj->dy;
    obj->posH += obj->dh;
    if (obj->targetH >= obj->posH)
    {
        obj->posX = obj->targetX;
        obj->posY = obj->targetY;
        obj->posH = obj->targetH;
        obj->pose->cmd = 0x10;
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 0x20;
        obj->unk98 = 0;
        obj->state = wait_then_spawn_part;
    }
}

ASM_FUNC("asm/code_0808962C/wait_then_spawn_part.s", void wait_then_spawn_part(struct ActorObj *obj))
