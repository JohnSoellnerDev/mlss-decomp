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
    s16 power;        /* attack stat used by the contact damage formula */
    s16 defense;      /* defense stat used by the contact damage formula */
    u8 filler106[0x10A - 0x106];
    u16 powerScale;   /* 8.8 multiplier on power (0x100 = 1.0) */
    u16 defenseScale; /* 8.8 multiplier on defense */
    u8 filler10E[0x114 - 0x10E];
    s32 damageScale;  /* 8.8 multiplier on the damage dealt, used only above 1.0 */
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
    void (*handler54)(struct ActorObj *);
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

/* The battle stats in an actor's part record (see code_0806F354.c), as the damage formula reads them. */
struct PartStats
{
    u8 filler0[0x102];
    s16 pow;          /* attack */
    s16 defense;      /* subtracted in the damage formula */
    u8 filler106[0x10A - 0x106];
    u16 powScale;     /* attack multiplier, 0x100 = 1.0 */
    u16 defScale;     /* defense multiplier, 0x100 = 1.0 */
    u8 filler10E[0x114 - 0x10E];
    u32 damageScale;  /* damage multiplier, 0x100 = 1.0; only used above 1.0 */
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

/* Moves toward the bros: the owner's part is one of the two bro records, which lie 0x130 bytes
   apart (slot -1 first, slot -2 second). Targets the midpoint of their targets when both have a
   slot, else the base point of the one that does, paramA4 px to the right and 2 px down, at the
   owner's part's height. Then starts the motion (speed 0x600) with action 2 and waits in
   start_motion_by_owner_slot. */
void set_target_from_records(struct ActorObj *obj)
{
    struct ActorObj *owner = obj->owner;
    struct ActorPart *part = owner->part;
    struct ActorPart *second;
    struct ActorPart *first;

    if (part->slot == -1)
    {
        first = part;
        second = (struct ActorPart *)((u8 *)part + 0x130);
    }
    else if (part->slot == -2)
    {
        first = (struct ActorPart *)((u8 *)part - 0x130);
        second = part;
    }
    if (first->slot != 0)
    {
        if (second->slot != 0)
        {
            obj->targetX = (first->targetX + second->targetX) / 2 + obj->paramA4 * 0x100;
            obj->targetY = (first->targetY + second->targetY) / 2 + 0x200;
        }
        else
        {
            obj->targetX = first->baseX + obj->paramA4 * 0x100;
            obj->targetY = first->baseY + 0x200;
        }
    }
    else
    {
        obj->targetX = second->baseX + obj->paramA4 * 0x100;
        obj->targetY = second->baseY + 0x200;
    }
    obj->targetH = owner->part->targetH;
    *((u8 *)obj + 0x79) |= 0x20; /* flags79 bit 5 */
    obj->motionSpeed = 0x600;
    obj->motionStart = 0;
    start_motion(obj);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = start_motion_by_owner_slot;
}

NONMATCH("asm/code_0808962C/spawn_child_with_offsets.s", void spawn_child_with_offsets(struct ActorObj *obj))
{
    struct ActorObj *owner;
    struct ActorObj *child;
    struct ActorPart *ownerPart;
    s32 spriteType;
    s32 ownerSlot;
    s32 tileX;
    s32 tileY;
    s32 tileH;
    s32 len;
    s32 dx;
    s32 v1;

    if (obj->pose->flags & 8)
    {
        if (obj->part->slot == 0x10)
        {
            ownerSlot = obj->owner->part->slot;
            spriteType = 0x4060;
        }
        else
        {
            ownerSlot = obj->owner->part->slot;
            spriteType = 0x4063;
        }
        if (ownerSlot == -1)
        {
            spriteType--;
        }
        child = obj_spawn(obj->part, spriteType, obj->baseX / 0x100 - 0xA, obj->baseY / 0x100,
                          obj->baseH / 0x100 + 0xD, ((u32)((u32)obj->pose->unk11 << 25)) >> 31, 1);
        obj->child = child;
        child->flags78 = (child->flags78 & 0x1F) | 0x20;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        child->state = offset_and_move_child;
        child->handler68 = on_arrive;
        child->handler54 = hit_react;
        child->unk6C = obj->unk6C;
        obj_list_push_default(child);
        child->handler5C = (void (*)(struct ActorObj *))contact_step_79e4;
        child->handler60 = (void (*)(struct ActorObj *))contact_step_75bc;
        child->param9C = obj->param9C;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->paramA8 = obj->paramA8;
        owner = obj->owner;
        child->owner = owner;
        ownerPart = owner->part;
        tileX = (ownerPart->targetX - child->baseX) / 4;
        tileY = (ownerPart->targetY - child->baseY) / 4;
        tileH = (ownerPart->targetH - child->baseH + 0x800) / 4;
        len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(tileX * tileX + tileY * tileY + tileH * tileH);
        child->dx = obj->param9C;
        dx = child->dx;
        v1 = dx + (len << 3);
        child->dy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx * -dx + v1 / 2, v1);
        set_slot_value(0x89, -1);
        sprite_set_action(obj, 5, 0, 0);
        obj->unk6C = 0;
        obj->state = 0;
        obj->handler68 = 0;
    }
}
END_NONMATCH

/* State of the child spawned by spawn_child_with_offsets: each frame its distance (dx, 24.8)
 * changes by dy, and it is placed that far from its base along the line from its part's base
 * (5 px higher) to the owner's part's target. When dx reaches -param9C it hands unk6C and
 * on_arrive back to its spawner (the object at part + 8), which plays action 6 and waits in
 * wait_pose_then_index, and the child is released. */
void offset_and_move_child(struct ActorObj *obj)
{
    struct ActorPart *part = obj->part;
    struct ActorObj *owner = obj->owner;
    s16 stepX;
    s16 stepY;
    s16 stepH;
    s32 heightOffset = 0x500;

    obj->dx = (u16)obj->dx + (u16)obj->dy;
    move_step(&stepX, &stepY, &stepH,
              owner->part->targetX - part->baseX,
              owner->part->targetY - part->baseY,
              owner->part->targetH - (part->baseH + heightOffset),
              obj->dx);
    obj->posX = obj->baseX + stepX;
    obj->posY = obj->baseY + stepY;
    obj->posH = obj->baseH + stepH;
    if (obj->dx <= -obj->param9C)
    {
        struct ActorObj *spawner = (struct ActorObj *)((u8 *)part + 8);

        spawner->state = (void (*)(struct ActorObj *))wait_pose_then_index;
        spawner->handler68 = on_arrive;
        spawner->unk6C = obj->unk6C;
        sprite_set_action(spawner, 6, 0, 0);
        obj->unk6C = 0;
        obj_release(obj);
    }
}

/* Contact handler: when other hits body (query_contact returns 0), deals other's damage to body:
   (pow * 0.4 * powScale - defense * 0.2 * defScale) / 2, times other's damageScale when it's above
   1.0, at least 1. Then body reacts to the hit for 8 frames. Returns query_contact's result. */
s32 contact_step_79e4(struct ActorObj *body, struct ActorObj *other, s32 kind)
{
    s32 contact = query_contact(body, (u32)other, kind);

    if (contact == 0)
    {
        struct PartStats *attacker = (struct PartStats *)other->part;
        s32 damageScale = attacker->damageScale;
        s32 pow = attacker->pow;
        u16 powScale = attacker->powScale;
        struct PartStats *defender = (struct PartStats *)body->part;
        s32 defense = defender->defense;
        u16 defScale = defender->defScale;
        s32 product;
        s32 damage;
        s32 defense51;

        damage = pow * 0x66;
        product = damage * powScale;
        if (product < 0)
        {
            product += 0xFF;
        }
        damage = product >> 8;
        defense51 = defense * 3;
        defense51 = defense51 + (defense51 << 4);
        product = defScale * defense51;
        if (product < 0)
        {
            product += 0xFF;
        }
        damage -= product >> 8;
        damage /= 2;
        if (damageScale > 0x100)
        {
            s32 scaled = damageScale * damage;

            damage = scaled;
            if (damage < 0)
            {
                scaled = damage + 0xFFFF;
            }
            damage = scaled >> 16;
        }
        else
        {
            s32 scaled = damage;

            if (damage < 0)
            {
                scaled += 0xFF;
            }
            damage = scaled >> 8;
        }
        if (damage <= 1)
        {
            damage = 1;
        }
        apply_partner_effect(other, body, damage, 0, other->posX / 0x100, other->posH / 0x100, 0);
        hit_react(body);
        body->timer = 8;
    }
    return contact;
}

/* Contact handler: unless lookup_slot reports an existing contact, deals contact damage from other
   to obj. Damage = (other's power * 0.4 * powerScale - obj's defense * 0.2 * defenseScale) / 2 in
   8.8, times other's damageScale when that is above 1.0, at least 1. Then obj reacts to the hit
   and its timer is set to 8. Returns lookup_slot's result. */
u32 contact_step_75bc(struct ActorObj *obj, struct ActorObj *other)
{
    s32 contact = lookup_slot(obj, (u32)other);

    if (contact == 0)
    {
        struct ActorPart *attacker = other->part;
        s32 damageScale = attacker->damageScale;
        s32 power = attacker->power;
        u16 powerScale = attacker->powerScale;
        struct ActorPart *target = obj->part;
        s32 defense = target->defense;
        u16 defenseScale = target->defenseScale;
        s32 product;
        s32 damage;
        s32 defense51;

        damage = power * 0x66; /* 0x66 / 256 = 0.4 */
        product = damage * powerScale;
        if (product < 0)
        {
            product += 0xFF;
        }
        damage = product >> 8;
        defense51 = defense * 3;
        defense51 = defense51 + (defense51 << 4); /* 51 / 256 = 0.2 */
        product = defenseScale * defense51;
        if (product < 0)
        {
            product += 0xFF;
        }
        damage -= product >> 8;
        damage /= 2;
        if (damageScale > 0x100)
        {
            s32 scaled = damageScale * damage;

            damage = scaled;
            if (damage < 0)
            {
                scaled = damage + 0xFFFF;
            }
            damage = scaled >> 16;
        }
        else
        {
            s32 scaled = damage;

            if (damage < 0)
            {
                scaled += 0xFF;
            }
            damage = scaled >> 8;
        }
        if (damage <= 1)
        {
            damage = 1;
        }
        apply_partner_effect(other, obj, damage, 0, other->posX / 0x100, other->posH / 0x100, 0);
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

/*
 * State after landing: counts the timer down, then either hops in place (while the hop
 * counter unk98 is negative; bounce_step counts it up and comes back here) or jumps to the
 * point halfway between its start and the owner's part target, playing sound 0x8D.
 */
void wait_then_spawn_part(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer <= 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        if (obj->unk98 < 0)
        {
            s32 hopDivisor;

            /* small hop in place: rise at 0x80, gravity -0x3BC0 / 0x880 */
            obj->dh = 0x80;
            hopDivisor = 0x880;
            obj->acc94 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(-0x3BC0, hopDivisor);
            obj->state = bounce_step;
            set_slot_value(0x8D, -1);
        }
        else
        {
            struct ActorObj *owner;
            struct ActorPart *ownerPart;

            obj_list_push_default(obj);
            owner = obj->owner;
            obj->targetX = ((ownerPart = owner->part)->targetX + obj->baseX) / 2 + 0x100;
            obj->targetY = (ownerPart->targetY + obj->baseY) / 2;
            obj->targetH = (ownerPart->targetH + obj->baseH) / 2;
            obj->acc94 = jump_velocity(&obj->dx, &obj->dy, &obj->dh, obj->targetX - obj->baseX,
                                       obj->targetY - obj->baseY, obj->targetH - obj->baseH,
                                       0x400, 0x200, 0x100, 0);
            set_slot_value(0x8D, -1);
            obj->state = drift_settle;
        }
    }
}
