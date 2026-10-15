#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags12;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct PartBits
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

struct Unk11Bits
{
    u8 low6 : 6;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct HighBitByte
{
    u8 low7 : 7;
    u8 high : 1;
};

struct ModeBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct PoseBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorInfo
{
    u8 filler0[0x1C];
    u8 flags1C;
    u8 flags1D;
};

struct ActorPart
{
    struct ActorPart *link;
    u8 filler0[0xB0 - 4];
    s32 unkB0;
    u8 filler1[0xD8 - 0xB4];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler2[0xEC - 0xE4];
    s16 slot;
    u8 filler3[0xF0 - 0xEE];
    struct ActorInfo *info;
    u8 filler3b[0xF6 - 0xF4];
    s16 unkF6;
    u8 filler4[0x111 - 0xF8];
    u8 unk111;
};

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x24 - 0x1C];
    u16 unk24;
    u8 filler26[0x28 - 0x26];
    struct ActorPart *part;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    void (*next)(struct ActorObj *);
    u8 filler54[0x58 - 0x54];
    u32 (*handler58)(struct ActorObj *, u32, u32);
    u8 filler5C[0x60 - 0x5C];
    u32 (*handler60)(struct ActorObj *, u32, u32);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
    u8 mode;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    s16 progress;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x92 - 0x90];
    s16 unk92;
    s32 partHandle;
    s32 limitH;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    struct ActorObj *partner;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
    u8 filler0B4[0xC3 - 0xB4];
    s8 offsetC3;
    s8 offsetC4;
    s8 offsetC5;
    u8 fillerC6[0xC8 - 0xC6];
};

struct Hero
{
    u8 filler0[0x08];
    struct ActorObj body;
    u8 filler0D0[0xD2 - 0xD0];
    u16 unkD2;
    u8 unkD4;
    u8 filler0D5[0x110 - 0xD5];
    u8 unk110;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gUnk_03000FD8;
extern u8 gRoom149Layer0[];

void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
u32 lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u8 step_motion(struct ActorObj *obj);
void step_motion_checked(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
u32 sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void wait_for_index(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void motion_timed(struct ActorObj *obj, u32 art, u32 wert);
s32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
struct Hero *place_from_table(s32 entry, u32 id, u32 flag);
void obj_attach(struct ActorObj *obj, struct ActorObj *owner, u32 flag);
void obj_release(struct ActorObj *obj);
void owner_moves_obj_prepare_b(struct ActorObj *obj);
void obj_retarget_c(struct ActorObj *obj);
void obj_retarget_a(struct ActorObj *obj);
void hero_b_link_b(struct ActorObj *obj);
void hero_b_active_a(struct ActorObj *obj);
void hero_b_flagged_a(struct ActorObj *obj);
void heroes_step(struct ActorObj *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
void place_targets(struct ActorObj *obj, s32 x, s32 y, s32 h, s32 flag);
void set_motion_offsets(struct ActorObj *obj, u32 value);
void state_stack_advance_progress(struct ActorObj *obj);
void hero_pair_active_finish_both(struct ActorObj *obj);
void hero_b_follow_child_pos_then_next(struct ActorObj *obj);
void hero_b_link_a(struct ActorObj *obj);
u32 obj_start_move_if_zero(struct ActorObj *obj, u32 slot);
void obj_pose_update_if_active(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void obj_stop_b(struct ActorObj *obj);
s32 resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void enter_finish_rebuild(struct ActorObj *obj);
void set_motion_start(struct ActorObj *obj, u32 value);
void hero_b_start_step32_then_next(struct ActorObj *obj);
void hero_b_step16_when_a_free(struct ActorObj *obj);

void obj_start_move_b(struct ActorObj *obj);
void obj_spawn_part(struct ActorObj *obj);
void obj_move_clamp_a(struct ActorObj *obj);
void obj_move_clamp_b(struct ActorObj *obj);
void obj_start_move_a(struct ActorObj *obj);
void obj_start_move_c(struct ActorObj *obj);
void obj_timed_move_a(struct ActorObj *obj);
void obj_timed_move_b(struct ActorObj *obj);
void obj_wait_index_a(struct ActorObj *obj);
u32 obj_lookup_slot(struct ActorObj *obj, u32 a, u32 b);
u32 obj_lookup_part(struct ActorObj *obj, u32 a, u32 b);
void obj_handoff_b(struct ActorObj *obj);

void hero_b_partner_place_height(struct ActorObj *obj)
{
    struct ActorObj *body = &gUnk_03000FD8->heroB->body;
    struct ActorObj *partner = obj->owner;
    u32 mode;

    state_stack_advance_progress(body);
    if (partner->partner != 0 && (partner->baseH << 6) < body->offsetC5 * body->progress)
    {
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
        {
            partner->state = hero_b_link_a;
        }
    }
    if (body->flags79 & 0x20)
    {
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            place_targets(body, partner->baseX / 0x100 + partner->offsetC3 - 0x18,
                          partner->baseY / 0x100 + partner->offsetC4, 0, -1);
            body->unk92 = 0x20;
            set_motion_offsets(body, 0x600);
        }
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            sprite_set_action(body, 0x13, 0x2066, 0);
            ((struct PoseBits *)&body->pose->flags12)->mode = 1;
        }
        if (body->child != 0)
        {
            sprite_set_action(body->child, 0x13, 0x2095, 0);
            ((struct PoseBits *)&body->child->pose->flags12)->mode = 0;
        }
        obj->state = hero_b_follow_child_pos_then_next;
    }
}

void hero_b_follow_child_pos_then_next(struct ActorObj *obj)
{
    struct ActorObj *body = &gUnk_03000FD8->heroB->body;
    struct ActorObj *owner = obj->owner;
    u32 mode;

    step_motion(body);
    body->child->posX = body->posX;
    body->child->posY = body->posY;
    body->child->posH = body->posH;
    if (owner->partner != 0 && owner->baseH < body->posH + body->offsetC5)
    {
        mode = owner->mode & 6;
        if (mode == 2 || mode == 4)
        {
            owner->state = hero_b_link_a;
        }
    }
    if (body->flags79 & 0x20)
    {
        set_slot_value(0x65, -1);
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            sprite_set_action(body, 0, 0x2005, 0);
            ((struct PoseBits *)&body->pose->flags12)->mode = 1;
        }
        obj->state = hero_pair_active_finish_both;
    }
}

void hero_pair_active_finish_both(struct ActorObj *obj)
{
    struct Hero *heroA;
    struct ActorObj *bodyA;
    struct ActorObj *bodyB;
    struct ActorPart *part;
    s16 *span;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;

    heroA = gUnk_03000FD8->heroA;
    bodyA = &heroA->body;
    bodyB = &gUnk_03000FD8->heroB->body;
    if (bodyB->pose->flags12 & 8)
    {
        obj_release(bodyB->child);
        set_slot_value(0x53, -1);
        if ((heroA->body.mode & 6) == 2 || (heroA->body.mode & 6) == 4)
        {
            part = bodyA->part;
            t = part->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = part->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = part->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyA, px, py, pz, -1);
            set_motion_start(bodyA, 0x400);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            if (bodyA->part->slot == -1)
            {
                sprite_set_action(bodyA, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyA, 0, 0x204F, 0);
            }
            ((struct Unk11Bits *)&bodyA->pose->unk11)->b6 = (span = (s16 *)&bodyA->targetX)[0] >= span[1];
            ((struct PoseBits *)&bodyA->pose->flags12)->mode = 0;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            part = bodyB->part;
            t = part->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = part->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = part->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyB, px, py, pz, -1);
            set_motion_start(bodyB, 0x400);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->part->slot == -1)
            {
                sprite_set_action(bodyB, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, 0x204F, 0);
            }
            ((struct Unk11Bits *)&bodyB->pose->unk11)->b6 = (span = (s16 *)&bodyB->targetX)[0] >= span[1];
            ((struct PoseBits *)&bodyB->pose->flags12)->mode = 0;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}

ASM_FUNC("asm/code_080D8884/hero_b_link_a.s", void hero_b_link_a(struct ActorObj *obj))

void hero_b_unlink_a(struct ActorObj *obj)
{
    struct ActorObj *body = &gUnk_03000FD8->heroB->body;
    struct ActorObj *owner = obj->owner;
    u32 mode;

    set_slot_value(0x15E, -1);
    obj_release(body->child);
    mode = owner->mode & 6;
    if (mode == 2 || mode == 4)
    {
        owner->state = hero_b_link_b;
    }
    effect_spawn_at(0x6AF, body->posX / 0x100, body->posY / 0x100, body->posH / 0x100, body);
    body->tag = 0;
    body->counter = 0;
    mode = body->mode & 6;
    if (mode == 2 || mode == 4)
    {
        sprite_set_action(body, 0x10, 0x2066, 0);
        ((struct PoseBits *)&body->pose->flags12)->mode = 1;
    }
    if (body->child != 0)
    {
        sprite_set_action(body->child, 0x10, 0x2095, 0);
        ((struct PoseBits *)&body->child->pose->flags12)->mode = 0;
    }
    obj->state = hero_b_active_a;
}

void hero_b_active_a(struct ActorObj *obj)
{
    struct ActorObj *body = &gUnk_03000FD8->heroB->body;
    u32 mode;
    u16 v;

    if (body->pose->flags12 & 8)
    {
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            sprite_set_action(body, 0x11, 0x2066, 0);
            ((struct PoseBits *)&body->pose->flags12)->mode = 1;
        }
        if (body->child != 0)
        {
            sprite_set_action(body->child, 0x11, 0x2095, 0);
            ((struct PoseBits *)&body->child->pose->flags12)->mode = 0;
        }
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            place_targets(body, body->baseX / 0x100 + body->offsetC3 - 0x18,
                          body->baseY / 0x100 + body->offsetC4, 0, -1);
            v = 0x30;
            body->unk92 = v;
            set_motion_offsets(body, 0x600);
        }
        obj->state = hero_b_flagged_a;
    }
}

void hero_b_flagged_a(struct ActorObj *obj)
{
    struct ActorObj *body = &gUnk_03000FD8->heroB->body;
    u32 mode;

    step_motion(body);
    if (body->flags79 & 0x20)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, body->posX / 0x100, body->posY / 0x100, body->posH / 0x100, body);
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            if (body->part->slot == -1)
            {
                sprite_set_action(body, 8, 0x2000, 0);
            }
            else
            {
                sprite_set_action(body, 8, 0x204D, 0);
            }
            ((struct PoseBits *)&body->pose->flags12)->mode = 1;
        }
        ((struct Unk11Bits *)&body->pose->unk11)->b6 = 1;
        obj->state = heroes_step;
    }
}

void heroes_step(struct ActorObj *obj)
{
    struct Hero *heroA;
    struct ActorObj *bodyA;
    struct ActorObj *bodyB;
    struct ActorPart *part;
    s16 *span;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;

    heroA = gUnk_03000FD8->heroA;
    bodyA = &heroA->body;
    bodyB = &gUnk_03000FD8->heroB->body;
    if (bodyB->pose->flags12 & 8)
    {
        set_slot_value(0x53, -1);
        if ((heroA->body.mode & 6) == 2 || (heroA->body.mode & 6) == 4)
        {
            part = bodyA->part;
            t = part->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = part->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = part->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyA, px, py, pz, -1);
            set_motion_start(bodyA, 0x400);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            if (bodyA->part->slot == -1)
            {
                sprite_set_action(bodyA, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyA, 0, 0x204F, 0);
            }
            ((struct Unk11Bits *)&bodyA->pose->unk11)->b6 = (span = (s16 *)&bodyA->targetX)[0] >= span[1];
            ((struct PoseBits *)&bodyA->pose->flags12)->mode = 0;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            part = bodyB->part;
            t = part->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = part->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = part->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyB, px, py, pz, -1);
            set_motion_start(bodyB, 0x400);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->part->slot == -1)
            {
                sprite_set_action(bodyB, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, 0x204F, 0);
            }
            ((struct Unk11Bits *)&bodyB->pose->unk11)->b6 = (span = (s16 *)&bodyB->targetX)[0] >= span[1];
            ((struct PoseBits *)&bodyB->pose->flags12)->mode = 0;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}

ASM_FUNC("asm/code_080D8884/hero_b_link_b.s", void hero_b_link_b(struct ActorObj *obj))

void obj_stop_a(struct ActorObj *obj)
{
    struct ActorObj *owner = obj->owner;
    step_motion(obj);
    if (obj->flags79 & 0x20)
    {
        set_slot_value(0x2E, -1);
        obj->state = 0;
        ((struct PartBits *)&owner->part->unk111)->b1 = 0;
        if (owner->part->unkF6 == 0)
        {
            obj_stop(owner);
        }
        else
        {
            owner->state = 0;
        }
    }
}

void obj_stop_b(struct ActorObj *obj)
{
    struct ActorObj *owner = obj->owner;

    step_motion(obj);
    if (obj->flags79 & 0x20)
    {
        set_slot_value(0x2E, -1);
        obj->state = 0;
        ((struct PartBits *)&owner->part->unk111)->b1 = 0;
        if (owner->part->unkF6 == 0)
        {
            obj_stop(owner);
        }
        else
        {
            owner->state = 0;
        }
    }
}

void hero_b_copy_pos(struct ActorObj *obj)
{
    struct Hero *heroB = gUnk_03000FD8->heroB;
    struct ActorObj *body = &heroB->body;

    step_motion_checked(body);
    body->child->posX = body->posX;
    body->child->posY = body->posY;
    body->child->posH = body->posH;
    if (heroB->body.flags79 & 0x20)
    {
        obj->state = hero_b_start_step32_then_next;
    }
}

void hero_b_end_a(struct ActorObj *obj)
{
    struct ActorObj *body = &gUnk_03000FD8->heroB->body;
    u32 mode;

    step_motion(body);
    if (body->flags79 & 0x20)
    {
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            sprite_set_action(body, 0, -1, 0);
            ((struct Unk11Bits *)&body->pose->unk11)->b6 = 0;
            ((struct PoseBits *)&body->pose->flags12)->mode = 0;
        }
        obj->state = hero_b_step16_when_a_free;
    }
}

void obj_move_clamp_a(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0x104 / 0x100;
    obj->dy = obj->dy * 0x104 / 0x100;
    obj->dh += obj->partHandle;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->part->targetH)
    {
        obj->posH = obj->part->targetH;
        obj->timer = obj->param9C;
        motion_timed(obj, 3, 0);
        obj->next = obj_start_move_a;
    }
}

void obj_start_move_a(struct ActorObj *obj)
{
    set_slot_value(0x81, -1);
    obj->targetX = obj->owner->part->targetX;
    obj->targetY = obj->owner->part->targetY;
    obj->targetH = obj->owner->part->targetH;
    obj->targetH = obj->baseH;
    ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
    obj->motionSpeed = 0x200;
    obj->motionStart = 0;
    start_motion(obj);
    sprite_set_action(obj, 4, 0, 0);
    ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
    obj->timer = 0x10;
    obj->state = obj_timed_move_a;
}

void obj_timed_move_a(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            obj_list_remove(obj);
            set_slot_value(0x81, -1);
            obj->targetX = obj->baseX + 0x2800;
            obj->targetY = obj->baseY;
            obj->targetH = obj->baseH;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 5, 0, 0);
            obj->state = obj_wait_index_a;
        }
    }
}

void obj_wait_index_a(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x81);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = wait_for_index;
    }
}

u32 part_step_obj_start_move_b(struct ActorObj *obj)
{
    obj->state = obj_start_move_b;
    return 1;
}

void obj_start_move_b(struct ActorObj *obj)
{
    set_slot_value(0x81, -1);
    obj->targetX = obj->owner->part->targetX + 0x2800;
    obj->targetY = obj->owner->part->targetY;
    obj->targetH = obj->owner->part->targetH;
    ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
    obj->motionSpeed = 0x200;
    obj->motionStart = 0;
    start_motion(obj);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_spawn_part;
}

void obj_spawn_part(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        sound_stop_song(0x81);
        set_slot_value(0x30, -1);
        obj->partHandle = jump_velocity(&obj->dx, &obj->dy, &obj->dh, 0, 0, 0, 0x1400, 0x4CC, 0x104, index);
        sprite_set_action(obj, 3, 0, 0);
        obj_list_push_default(obj);
        obj->handler58 = obj_lookup_part;
        obj->handler60 = obj_lookup_slot;
        obj->state = obj_move_clamp_a;
    }
}

u32 obj_lookup_slot(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x81);
    return lookup_link_slot(obj, a, b);
}

u32 obj_lookup_part(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x81);
    return partner_link_update(obj, a, b);
}

ASM_FUNC("asm/code_080D8884/obj_spawn_child_a.s", void obj_spawn_child_a(void))


void obj_retarget_a(struct ActorObj *obj)
{
    struct ActorPart *part;
    s32 targetH;
    s32 halfX;
    s32 halfY;

    obj->dh += obj->partHandle;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->limitH)
    {
        obj->posH = obj->limitH;
        set_slot_value(0x65, -1);
        part = obj->owner->part;
        targetH = part->targetH;
        obj->limitH = targetH;
        halfX = (part->targetX - obj->baseX) / 2;
        halfY = (part->targetY - obj->baseY) / 2;
        obj->partHandle = jump_velocity(&obj->dx, &obj->dy, &obj->dh, halfX, halfY, targetH - obj->baseH,
                                        0x1400, 0x4CC, 0xFD, 0);
        obj->state = obj_handoff_b;
    }
}

void obj_handoff_b(struct ActorObj *obj)
{
    struct Hero *hero;

    obj->dh += obj->partHandle;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->limitH)
    {
        obj->posH = obj->limitH;
        hero = place_from_table(obj->paramA4, 0x16, 1);
        obj_attach(&hero->body, obj->owner, 0);
        ((struct HighBitByte *)&hero->unk110)->high = 1;
        sprite_set_action(&hero->body, 0, -1, 0);
        hero->body.posX = obj->posX;
        hero->body.posY = obj->posY;
        hero->body.posH = obj->posH;
        hero->body.state = owner_moves_obj_prepare_b;
        obj->part->unkB0 = 1;
        obj_release(obj);
    }
}

void obj_move_clamp_b(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0x104 / 0x100;
    obj->dy = obj->dy * 0x104 / 0x100;
    obj->dh += obj->partHandle;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->limitH)
    {
        obj->posH = obj->limitH;
        obj->timer = obj->param9C;
        motion_timed(obj, 3, 0);
        obj->next = obj_start_move_c;
    }
}

void obj_start_move_c(struct ActorObj *obj)
{
    obj->targetX = obj->owner->part->targetX;
    obj->targetY = obj->owner->part->targetY;
    obj->targetH = obj->owner->part->targetH;
    obj->targetH = obj->baseH;
    ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
    obj->motionSpeed = 0x200;
    obj->motionStart = 0;
    start_motion(obj);
    sprite_set_action(obj, 4, 0, 0);
    obj->timer = 0x10;
    obj->state = obj_timed_move_b;
}

void obj_timed_move_b(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            obj_list_remove(obj);
            obj->targetX = obj->baseX + 0x2800;
            obj->targetY = obj->baseY;
            obj->targetH = obj->baseH;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 5, 0, 0);
            obj->state = wait_for_index;
        }
    }
}

ASM_FUNC("asm/code_080D8884/owner_moves_obj_spawn_child_b.s", void owner_moves_obj_spawn_child_b(void))

void obj_retarget_b(struct ActorObj *obj)
{
    struct ActorPart *part;
    s32 targetH;
    s32 halfX;
    s32 halfY;

    obj->dh += obj->partHandle;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->limitH)
    {
        obj->posH = obj->limitH;
        set_slot_value(0x65, -1);
        part = obj->owner->part;
        targetH = part->targetH;
        obj->limitH = targetH;
        halfX = (part->targetX - obj->baseX) / 2;
        halfY = (part->targetY - obj->baseY) / 2;
        obj->partHandle = jump_velocity(&obj->dx, &obj->dy, &obj->dh, halfX, halfY, targetH - obj->baseH,
                                        0x2800, 0x4CC, 0xFD, 0);
        obj->part->unkB0 = 1;
        obj->state = obj_retarget_c;
    }
}
