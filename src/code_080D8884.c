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

struct Unk111Bit3
{
    u8 low3 : 3;
    u8 b3 : 1;
    u8 high4 : 4;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 high3 : 3;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
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
    u8 flags1C; // 0x80: the root part of a linked actor
    u8 flags1D; // 0x10: can't be knocked back
};

struct ActorPart
{
    struct ActorPart *link; // next part of a multi-part actor
    u8 filler0[0x84 - 4];
    u16 unk84;  // motion speed copied from the part that was hit
    u8 filler0B[0xB0 - 0x86];
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
    s16 unkF6;  // 0 once the actor is finished (obj_stop_a stops it then)
    u8 filler4[0x111 - 0xF8];
    u8 unk111;  // bit 1: being knocked back, bit 3: hit by hero B's first hit
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
    u16 unkD2;  // the current hit's value, for resolve_contact_action
    u8 unkD4;   // low 7 bits: the current hit's kind
    u8 filler0D5[0x110 - 0xD5];
    u8 unk110;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

/* Actor mode bits 1-2: 2 or 4 means the actor is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* ActorPose.flags12 bit set once the pose's current animation has played out. */
#define HERO_SPRITE_ANIM_DONE 8

/* ActorObj.flags79 bit set once the current motion (or state-stack progress) has finished. */
#define HERO_FLAG_MOTION_DONE 0x20

/* sprite_set_action arguments for a hero standing still, without and with a pose. */
#define HERO_ACTION_STAND 0x2002
#define HERO_ACTION_STAND_POSED 0x204F

extern struct World *gWorld;
extern u8 gRoom149Layer0[];
extern u8 gText[];

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
void obj_retarget_b(struct ActorObj *obj);
void obj_pose_update_b(struct ActorObj *obj);
void obj_stop_a(struct ActorObj *obj);
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

/*
 * Hero B state: advances B's progress and, once obj's owner has a partner and B has risen above the
 * owner's base height (progress * B's height offset), links the owner (hero_b_link_a). When B's
 * motion is done, B jumps in an arc 0x20 high (speed 0x600) to the owner's base spot plus its
 * offsets (24 pixels to the left), B and its child take action 0x13 (0x2066 and 0x2095) and obj
 * moves on to hero_b_follow_child_pos_then_next.
 */
void hero_b_partner_place_height(struct ActorObj *obj)
{
    struct ActorObj *body = &gWorld->heroB->body;
    struct ActorObj *owner = obj->owner;
    u32 mode;

    state_stack_advance_progress(body);
    if (owner->partner != 0 && (owner->baseH << 6) < body->offsetC5 * body->progress)
    {
        if (HERO_MODE_ACTIVE(owner->mode))
        {
            owner->state = hero_b_link_a;
        }
    }
    if (body->flags79 & HERO_FLAG_MOTION_DONE)
    {
        mode = body->mode & 6;
        if (mode == 2 || mode == 4)
        {
            place_targets(body, owner->baseX / 0x100 + owner->offsetC3 - 24,
                          owner->baseY / 0x100 + owner->offsetC4, 0, -1);
            body->unk92 = 0x20; /* arc height */
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

/*
 * Hero B state: steps B's jump while carrying its child along (the child takes B's position).
 * Once obj's owner has a partner and B is above the owner's base height, links the owner
 * (hero_b_link_a). When the jump has landed, plays sound 0x65, B takes action 0 (0x2005) and
 * obj moves on to hero_pair_active_finish_both.
 */
void hero_b_follow_child_pos_then_next(struct ActorObj *obj)
{
    struct ActorObj *heroB = &gWorld->heroB->body;
    struct ActorObj *owner = obj->owner;
    u32 mode;

    step_motion(heroB);
    heroB->child->posX = heroB->posX;
    heroB->child->posY = heroB->posY;
    heroB->child->posH = heroB->posH;
    if (owner->partner != 0 && owner->baseH < heroB->posH + heroB->offsetC5)
    {
        mode = owner->mode & 6;
        if (mode == 2 || mode == 4)
        {
            owner->state = hero_b_link_a;
        }
    }
    if (heroB->flags79 & HERO_FLAG_MOTION_DONE)
    {
        set_slot_value(0x65, -1);
        mode = heroB->mode & 6;
        if (mode == 2 || mode == 4)
        {
            sprite_set_action(heroB, 0, 0x2005, 0);
            ((struct PoseBits *)&heroB->pose->flags12)->mode = 1;
        }
        obj->state = hero_pair_active_finish_both;
    }
}

/*
 * Hero pair state: once hero B's pose has finished (pose flag 8), releases B's child, clears slot
 * 0x53 and, for hero A and then hero B (each only in mode 2 or 4), sends the hero to its part's
 * target position (8.8 fixed point, speed 0x400), sets the standing action (0x2002, or 0x204F
 * when it has a slot), faces it along the two halves of its targetX word, clears its pose mode
 * and hands it to enter_finish_rebuild.
 */
void hero_pair_active_finish_both(struct ActorObj *obj)
{
    struct Hero *heroA;
    struct ActorObj *bodyA;
    struct ActorObj *bodyB;
    struct ActorPart *part;
    s16 *facing;
    s32 fixed;
    s32 x;
    s32 y;
    s32 h;

    heroA = gWorld->heroA;
    bodyA = &heroA->body;
    bodyB = &gWorld->heroB->body;
    if (bodyB->pose->flags12 & 8)
    {
        obj_release(bodyB->child);
        set_slot_value(0x53, -1);

        /* hero A */
        if ((heroA->body.mode & 6) == 2 || (heroA->body.mode & 6) == 4)
        {
            part = bodyA->part;
            fixed = part->targetX;
            if (fixed < 0)
                fixed += 0xff;
            x = fixed >> 8;
            fixed = part->targetY;
            if (fixed < 0)
                fixed += 0xff;
            y = fixed >> 8;
            fixed = part->targetH;
            if (fixed < 0)
                fixed += 0xff;
            h = fixed >> 8;
            place_targets(bodyA, x, y, h, -1);
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
            ((struct Unk11Bits *)&bodyA->pose->unk11)->b6 = (facing = (s16 *)&bodyA->targetX)[0] >= facing[1];
            ((struct PoseBits *)&bodyA->pose->flags12)->mode = 0;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }

        /* hero B */
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            part = bodyB->part;
            fixed = part->targetX;
            if (fixed < 0)
                fixed += 0xff;
            x = fixed >> 8;
            fixed = part->targetY;
            if (fixed < 0)
                fixed += 0xff;
            y = fixed >> 8;
            fixed = part->targetH;
            if (fixed < 0)
                fixed += 0xff;
            h = fixed >> 8;
            place_targets(bodyB, x, y, h, -1);
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
            ((struct Unk11Bits *)&bodyB->pose->unk11)->b6 = (facing = (s16 *)&bodyB->targetX)[0] >= facing[1];
            ((struct PoseBits *)&bodyB->pose->flags12)->mode = 0;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}

/* Hero B's hit lands on obj: unless obj's actor can't be knocked back, marks its part as hit by B
 * and knocked back, resolves the hit (kind 0x46, value 0xCC), counts it on B and, while the actor
 * is still alive or knocked back, spawns effect 0x6C4 at it. Then the actor's root part (obj, or
 * for a multi-part actor the part flagged as root, which takes obj's motion speed) jumps back to
 * its target position in an arc 0x30 above it and stops in obj_stop_b. */
void hero_b_link_a(struct ActorObj *obj)
{
    struct Hero *hero = gWorld->heroB;
    struct ActorObj *body = &hero->body;
    struct ActorObj *hitObj = obj;
    struct ActorPart *part;
    struct ActorPart *head;

    if (obj->part->info->flags1D & 0x10)
    {
        obj->state = 0;
        return;
    }
    ((struct Unk111Bit3 *)&obj->part->unk111)->b3 = 1;
    ((struct PartBits *)&obj->part->unk111)->b1 = 1;
    ((struct HighBitByte *)&hero->unkD4)->low7 = 0x46;
    hero->unkD2 = 0xCC;
    resolve_contact_action(body, obj, 0, 0, (u32)(gRoom149Layer0 + 0x59));
    body->unk24++;
    head = obj->part;
    if (head->unkF6 > 0 || (head->unk111 & 2) != 0)
    {
        s32 x = obj->baseX / 0x100 + obj->offsetC3;
        s32 y = obj->baseY / 0x100 + obj->offsetC4;
        effect_spawn_at(0x6C4, x, y, 0, obj);
    }
    obj->partner = 0;
    head = obj->part;
    if (head->link != 0)
    {
        struct ActorPart *root;
        obj->state = 0;
        root = head;
        while ((root->info->flags1C & 0x80) == 0)
        {
            root = root->link;
        }
        obj = (struct ActorObj *)((u8 *)root + 8);
        root->unk84 = hitObj->motionSpeed;
    }
    obj->owner = hitObj;
    if (HERO_MODE_ACTIVE(obj->mode))
    {
        part = obj->part;
        place_targets(obj, part->targetX / 0x100, part->targetY / 0x100, part->targetH / 0x100, -1);
        obj->unk92 = obj->part->targetH / 0x100 + 0x30;
        set_motion_offsets(obj, 0x600);
    }
    obj->state = obj_stop_b;
}

/* Ends hero B's link with the actor obj is attached to: plays sound 0x15E, releases B's child object,
   switches the owner (while active) to hero_b_link_b, spawns effect 0x6AF at B, clears B's tag and
   counter bytes, starts action 0x10 on B (0x2066) and its child (0x2095), and moves on to
   hero_b_active_a. */
void hero_b_unlink_a(struct ActorObj *obj)
{
    struct ActorObj *heroB = &gWorld->heroB->body;
    struct ActorObj *owner = obj->owner;
    u32 mode;

    set_slot_value(0x15E, -1);
    obj_release(heroB->child);
    mode = owner->mode & 6;
    if (mode == 2 || mode == 4)
    {
        owner->state = hero_b_link_b;
    }
    effect_spawn_at(0x6AF, heroB->posX / 0x100, heroB->posY / 0x100, heroB->posH / 0x100, heroB);
    heroB->tag = 0;
    heroB->counter = 0;
    mode = heroB->mode & 6;
    if (mode == 2 || mode == 4)
    {
        sprite_set_action(heroB, 0x10, 0x2066, 0);
        ((struct PoseBits *)&heroB->pose->flags12)->mode = 1;
    }
    if (heroB->child != 0)
    {
        sprite_set_action(heroB->child, 0x10, 0x2095, 0);
        ((struct PoseBits *)&heroB->child->pose->flags12)->mode = 0;
    }
    obj->state = hero_b_active_a;
}

/*
 * Hero B state: once B's pose animation has played out, B (while active) and its child take
 * action 0x11 (0x2066 and 0x2095), B jumps in an arc 0x30 high (speed 0x600) to its own base
 * spot plus its offsets (24 pixels to the left), and obj moves on to hero_b_flagged_a.
 */
void hero_b_active_a(struct ActorObj *obj)
{
    struct ActorObj *body = &gWorld->heroB->body;
    u32 mode;
    u16 arcHeight;

    if (body->pose->flags12 & HERO_SPRITE_ANIM_DONE)
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
            place_targets(body, body->baseX / 0x100 + body->offsetC3 - 24,
                          body->baseY / 0x100 + body->offsetC4, 0, -1);
            arcHeight = 0x30;
            body->unk92 = arcHeight;
            set_motion_offsets(body, 0x600);
        }
        obj->state = hero_b_flagged_a;
    }
}

/*
 * Hero B state: moves B along its jump (from hero_b_active_a). Once it has landed, plays sound
 * 0x2E and spawns effect 0x1E at B, gives B (while active) action 8 (0x2000, or 0x204D for the
 * other hero's sprite set), turns it to face the other way (pose bit 6), and moves obj on to
 * heroes_step.
 */
void hero_b_flagged_a(struct ActorObj *obj)
{
    struct ActorObj *body = &gWorld->heroB->body;
    u32 mode;

    step_motion(body);
    if (body->flags79 & HERO_FLAG_MOTION_DONE)
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

/*
 * Hero pair state: once hero B's pose animation has played out, plays sound 0x53 and sends hero A
 * and then hero B (each only while active) to the rest position in its part (8.8 fixed point, speed
 * 0x400), gives it the standing action, faces it along its move, clears its pose mode and hands it
 * to enter_finish_rebuild.
 */
void heroes_step(struct ActorObj *obj)
{
    struct Hero *heroA;
    struct ActorObj *bodyA;
    struct ActorObj *bodyB;
    struct ActorPart *part;
    s16 *span;
    s32 fixed;
    s32 x;
    s32 y;
    s32 h;

    heroA = gWorld->heroA;
    bodyA = &heroA->body;
    bodyB = &gWorld->heroB->body;
    if (bodyB->pose->flags12 & HERO_SPRITE_ANIM_DONE)
    {
        set_slot_value(0x53, -1);

        /* hero A */
        if (HERO_MODE_ACTIVE(heroA->body.mode))
        {
            part = bodyA->part;
            fixed = part->targetX;
            if (fixed < 0)
                fixed += 0xff;
            x = fixed >> 8;
            fixed = part->targetY;
            if (fixed < 0)
                fixed += 0xff;
            y = fixed >> 8;
            fixed = part->targetH;
            if (fixed < 0)
                fixed += 0xff;
            h = fixed >> 8;
            place_targets(bodyA, x, y, h, -1);
            set_motion_start(bodyA, 0x400);
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            if (bodyA->part->slot == -1)
            {
                sprite_set_action(bodyA, 0, HERO_ACTION_STAND, 0);
            }
            else
            {
                sprite_set_action(bodyA, 0, HERO_ACTION_STAND_POSED, 0);
            }
            /* mirror unless the move goes right (low half of targetX = start x, high half = target x) */
            ((struct Unk11Bits *)&bodyA->pose->unk11)->b6 = (span = (s16 *)&bodyA->targetX)[0] >= span[1];
            ((struct PoseBits *)&bodyA->pose->flags12)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }

        /* hero B */
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            part = bodyB->part;
            fixed = part->targetX;
            if (fixed < 0)
                fixed += 0xff;
            x = fixed >> 8;
            fixed = part->targetY;
            if (fixed < 0)
                fixed += 0xff;
            y = fixed >> 8;
            fixed = part->targetH;
            if (fixed < 0)
                fixed += 0xff;
            h = fixed >> 8;
            place_targets(bodyB, x, y, h, -1);
            set_motion_start(bodyB, 0x400);
        }
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            if (bodyB->part->slot == -1)
            {
                sprite_set_action(bodyB, 0, HERO_ACTION_STAND, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, HERO_ACTION_STAND_POSED, 0);
            }
            ((struct Unk11Bits *)&bodyB->pose->unk11)->b6 = (span = (s16 *)&bodyB->targetX)[0] >= span[1];
            ((struct PoseBits *)&bodyB->pose->flags12)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}

// Hero B's second hit on target: resolves the hit (kind 0x48, value 0xF3), counts it and
// spawns the hit effect. Unless the target can't be knocked back, its root part is then
// knocked back home in an arc, and obj_stop_a waits for it to land.
void hero_b_link_b(struct ActorObj *target)
{
    struct Hero *hero = gWorld->heroB;
    struct ActorObj *body = &hero->body;
    struct ActorObj *hitObj = target;
    struct ActorPart *part;
    struct ActorPart *head;

    if (target->part->info->flags1D & 0x10)
    {
        ((struct HighBitByte *)&hero->unkD4)->low7 = 0x48;
        hero->unkD2 = 0xF3;
        resolve_contact_action(body, target, 0, 0, (u32)(gText + 0x5F065));
        body->unk24++;
        head = target->part;
        if (head->unkF6 > 0 || (head->unk111 & 2) != 0)
        {
            s32 x = target->baseX / 0x100 + target->offsetC3;
            s32 y = target->baseY / 0x100 + target->offsetC4;
            effect_spawn_at(0x6C4, x, y, 0, target);
        }
        target->state = 0;
        return;
    }
    ((struct PartBits *)&target->part->unk111)->b1 = 1;
    ((struct HighBitByte *)&hero->unkD4)->low7 = 0x48;
    hero->unkD2 = 0xF3;
    resolve_contact_action(body, target, 0, 0, (u32)(gText + 0x5F065));
    body->unk24++;
    head = target->part;
    if (head->unkF6 > 0 || (head->unk111 & 2) != 0)
    {
        s32 x = target->baseX / 0x100 + target->offsetC3;
        s32 y = target->baseY / 0x100 + target->offsetC4;
        effect_spawn_at(0x6C4, x, y, 0, target);
    }

    /* A multi-part actor moves as a whole: knock back its root part instead. */
    if (target->part->link != 0)
    {
        struct ActorPart *walk;
        target->state = 0;
        walk = target->part;
        while ((walk->info->flags1C & 0x80) == 0)
        {
            walk = walk->link;
        }
        target = (struct ActorObj *)((u8 *)walk + 8);
        walk->unk84 = hitObj->motionSpeed;
    }
    target->owner = hitObj;
    if ((target->mode & 6) == 2 || (target->mode & 6) == 4)
    {
        part = target->part;
        place_targets(target, part->targetX / 0x100, part->targetY / 0x100, part->targetH / 0x100, -1);
        target->unk92 = target->part->targetH / 0x100 + 0x40;
        set_motion_offsets(target, 0x580);
    }
    target->state = obj_stop_a;
}

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
    struct Hero *heroB = gWorld->heroB;
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
    struct ActorObj *body = &gWorld->heroB->body;
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

/* Moves the object one frame along the jump obj_spawn_part started: the horizontal offsets from
   its base grow by 1/64 a frame and dh by the jump's per-frame change (partHandle). Once it comes
   down to its part's target height it lands there, waits param9C frames (motion_timed) and then
   goes on with obj_start_move_a. */
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

/* Once the current animation is done (pose flag 8), spawns a 0x4070 child 12 pixels above this object,
   wired like this one (owner, partner, params), and sends it on a jump a third of the way toward the
   owner's part target; obj_retarget_a takes it from there. This object then plays its follow-up action
   and waits in obj_pose_update_if_active. */
void obj_spawn_child_a(struct ActorObj *obj)
{
    struct ActorObj *child;
    struct ActorPart *ownerPart;
    s16 *dxp;
    s16 *dyp;
    s16 *dhp;
    s32 *limitp;
    s32 thirdX;
    s32 thirdY;

    if ((obj->pose->flags12 & 8) != 0)
    {
        child = obj_spawn(obj->part, 0x4070, obj->baseX / 0x100, obj->baseY / 0x100,
                          obj->baseH / 0x100 + 0xC, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        ((struct Flags78Bits *)&child->flags78)->high3 = 1;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        child->state = obj_retarget_a;
        child->handler68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->handler60 = (u32 (*)(struct ActorObj *, u32, u32))obj_start_move_if_zero;
        child->param9C = obj->param9C;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->partner = obj->partner;
        child->tag = 0xFF;
        sprite_set_action(child, 2, 0, 0);
        /* the child lands at the owner's part height; jump a third of the way there */
        limitp = &child->limitH;
        ownerPart = obj->owner->part;
        *limitp = ownerPart->targetH;
        dxp = &child->dx;
        dyp = &child->dy;
        dhp = &child->dh;
        thirdX = (ownerPart->targetX - child->baseX) / 3;
        thirdY = (ownerPart->targetY - child->baseY) / 3;
        child->partHandle = jump_velocity(dxp, dyp, dhp, thirdX, thirdY,
                                          *limitp - child->baseH, 0x3C00, 0x4CC, 0xFD, 0);
        obj->unk6C = 0;
        sprite_set_action(obj, (obj->owner->part->slot == -1) ? 5 : 9, 0, 0);
        ((struct PoseBits *)&obj->pose->flags12)->mode = 1;
        obj->state = obj_pose_update_if_active;
    }
}


/* Moves the jumping child of obj_spawn_child_a one frame (partHandle is the per-frame change of dh
 * that jump_velocity gave). When it comes down to limitH it bounces with sound 0x65 (set_slot_value):
 * a second jump (0x1400) half way toward the owner's part target, landing at that target's height,
 * and hands over to obj_handoff_b. Like obj_retarget_b, with a lower jump. */
void obj_retarget_a(struct ActorObj *obj)
{
    struct ActorPart *ownerPart;
    s32 landH;
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
        ownerPart = obj->owner->part;
        landH = ownerPart->targetH;
        obj->limitH = landH;
        halfX = (ownerPart->targetX - obj->baseX) / 2;
        halfY = (ownerPart->targetY - obj->baseY) / 2;
        obj->partHandle = jump_velocity(&obj->dx, &obj->dy, &obj->dh, halfX, halfY, landH - obj->baseH,
                                        0x1400, 0x4CC, 0xFD, 0);
        obj->state = obj_handoff_b;
    }
}

/* Moves the jumping child one frame on the second jump obj_retarget_a started. When it comes down to
 * limitH it hands over to a real actor: places entry paramA4 of table 0x16, attaches it to the owner,
 * puts it in its default action at the landing spot with owner_moves_obj_prepare_b as its state,
 * marks the part (unkB0) and releases itself. */
void obj_handoff_b(struct ActorObj *obj)
{
    struct Hero *spawned;

    obj->dh += obj->partHandle;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->limitH)
    {
        obj->posH = obj->limitH;
        spawned = place_from_table(obj->paramA4, 0x16, 1);
        obj_attach(&spawned->body, obj->owner, 0);
        ((struct HighBitByte *)&spawned->unk110)->high = 1;
        sprite_set_action(&spawned->body, 0, -1, 0);
        spawned->body.posX = obj->posX;
        spawned->body.posY = obj->posY;
        spawned->body.posH = obj->posH;
        spawned->body.state = owner_moves_obj_prepare_b;
        obj->part->unkB0 = 1;
        obj_release(obj);
    }
}

/* Moves the object one frame along the jump obj_create_part_then_action3 started: the horizontal
   offsets from its base grow by 1/64 a frame and dh by the jump's per-frame change (partHandle).
   Once it comes down to limitH (its data's target height) it lands there, waits param9C frames
   (motion_timed) and then goes on with obj_start_move_c. Like obj_move_clamp_a. */
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

/* Once the current animation is done (pose flag 8), spawns a 0x4070 child 12 pixels above this object,
   wired like this one (owner, partner, params), and sends it on a jump a third of the way toward the
   owner's part target; obj_retarget_b takes it from there. This object then plays its follow-up action
   and waits in obj_pose_update_b. Unlike obj_spawn_child_a, the jump height is measured from this
   object's limitH, not the child's. */
void owner_moves_obj_spawn_child_b(struct ActorObj *obj)
{
    struct ActorObj *child;
    struct ActorPart *ownerPart;
    s16 *dxp;
    s16 *dyp;
    s16 *dhp;
    s32 *limitp;
    s32 thirdX;
    s32 thirdY;

    if ((obj->pose->flags12 & 8) != 0)
    {
        child = obj_spawn(obj->part, 0x4070, obj->baseX / 0x100, obj->baseY / 0x100,
                          obj->baseH / 0x100 + 0xC, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        ((struct Flags78Bits *)&child->flags78)->high3 = 1;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        child->state = obj_retarget_b;
        child->handler68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->handler60 = (u32 (*)(struct ActorObj *, u32, u32))obj_start_move_if_zero;
        child->param9C = obj->param9C;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->partner = obj->partner;
        child->tag = 0xFF;
        sprite_set_action(child, 2, 0, 0);
        /* the child lands at the owner's part height; jump a third of the way there */
        limitp = &child->limitH;
        ownerPart = obj->owner->part;
        *limitp = ownerPart->targetH;
        dxp = &child->dx;
        dyp = &child->dy;
        dhp = &child->dh;
        thirdX = (ownerPart->targetX - child->baseX) / 3;
        thirdY = (ownerPart->targetY - child->baseY) / 3;
        child->partHandle = jump_velocity(dxp, dyp, dhp, thirdX, thirdY,
                                          obj->limitH - child->baseH, 0x3C00, 0x4CC, 0xFD, 0);
        obj->unk6C = 0;
        sprite_set_action(obj, (obj->owner->part->slot == -1) ? 5 : 9, 0, 0);
        ((struct PoseBits *)&obj->pose->flags12)->mode = 1;
        obj->state = obj_pose_update_b;
    }
}

/* Moves the jumping child one frame (partHandle is the per-frame change of dh that jump_velocity gave).
   When it comes down to limitH it bounces with sound 0x65: a second, higher jump half way toward the
   owner's part target, landing at that target's height, then marks the part (unkB0) and hands over to
   obj_retarget_c. Like obj_retarget_a, with a 0x2800 jump instead of 0x1400. */
void obj_retarget_b(struct ActorObj *obj)
{
    struct ActorPart *ownerPart;
    s32 landH;
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
        ownerPart = obj->owner->part;
        landH = ownerPart->targetH;
        obj->limitH = landH;
        halfX = (ownerPart->targetX - obj->baseX) / 2;
        halfY = (ownerPart->targetY - obj->baseY) / 2;
        obj->partHandle = jump_velocity(&obj->dx, &obj->dy, &obj->dh, halfX, halfY, landH - obj->baseH,
                                        0x2800, 0x4CC, 0xFD, 0);
        obj->part->unkB0 = 1;
        obj->state = obj_retarget_c;
    }
}
