#include "global.h"
#include "ram_routines.h"

struct OrbitNode
{
    struct OrbitNode *next;
    s32 unk4; /* X (8.8 fixed point) */
    s32 unk8; /* Y */
    u8 filler0C[0x1B - 0x0C];
    u8 unk1B; /* bit 7 (OrbitFlagBits) toggled on every 5th node by place_members_on_orbit */
};

struct OrbitFlagBits
{
    u8 lo7 : 7;
    u8 bit7 : 1;
} __attribute__((packed));

struct SpriteEntry
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[0x11 - 8];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
};

struct BodyData
{
    u8 filler0[0x38];
    struct Body *leader;
    u8 filler3C[0xD8 - 0x3C];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler0E4[0xEC - 0xE4];
    s16 pose;
    u8 filler0EE[0x114 - 0xEE];
    u32 poseVal;
    u8 filler118[0x11E - 0x118];
    u16 flags;
};

struct SpriteMirrorByte
{
    u8 lo6 : 6;
    u8 mirror : 1;
    u8 hi : 1;
};

struct SpriteModeByte
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 active : 1;
    u8 hi : 4;
};

struct BodyFlagBits
{
    u8 lo5 : 5;
    u8 active : 1;
    u8 hi : 2;
};

struct Body
{
    u8 filler0[8];
    struct SpriteEntry *sprite;
    struct OrbitNode *nodes;
    s32 x;
    s32 y;
    s32 z;
    u16 moveProgress;
    u8 filler1E[0x24 - 0x1E];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct BodyData *data;
    struct Body *partner;
    struct Body *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    u32 state;
    u32 slot50;
    u8 filler54[0x58 - 0x54];
    u32 slot58;
    u8 filler5C[0x6C - 0x5C];
    u32 anim;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 mode;
    u8 unk77;
    u8 unk78;
    u8 flags;
    u16 rate;
    u16 speed;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA; /* motion start X in pixels (place_targets packs the current X here) */
    s16 spanB; /* motion target X in pixels */
    s32 baseY;      /* orbiting body: angular speed (8.8 fixed point) */
    s32 swing;      /* orbiting body: orbit radius (8.8 fixed point) */
    u8 filler90[0x92 - 0x90];
    u16 arc;
    /* jump arc set by set_motion_offsets: height = peak - (arcSpan * progress / 0x4000 - arcRise)^2 / 0x100 */
    s32 arcSpan;
    s32 arcRise;
    s32 height;
    u8 fillerA0[0xA8 - 0xA0];
    u32 scriptPos;  /* orbiting body: next byte of gEllipseStepScript */
    s16 prevProgress; /* progress before the last step_motion */
    u16 timer;      /* orbiting body: angle (whole units) where the current script step ends */
    u16 scriptB0;   /* orbiting body: current script step (s8; its sign is the turn direction, 0 ends) */
    u16 speedZ;     /* orbiting body: radius (whole units) the orbit closes in to */
    u8 fillerB4[0xC3 - 0xB4];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 fillerC6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[8];
    struct Body body;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gEllipseStepScript[];

/* An orbiting body's angle (8.8 fixed point, 256 = a full turn), kept in spanA/spanB. */
#define BODY_ORBIT_ANGLE(body) (*(s32 *)&(body)->spanA)

struct PoseTable
{
    u8 filler0[0x2A];
    u16 value2A;
};

void sprite_set_action(struct Body *body, u32 action, s32 arg, u32 extra);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct Body *owner);
u32 partner_link_update(struct Body *body, struct Body *partner, u32 mode);
void start_fx_then_twin_chain(struct Body *body);
void start_fx_then_part_chain(struct Body *body);
void hero_a_advance_and_place(struct Body *body);
void build_part_chain(struct Body *body);
void build_twin_part_chain(struct Body *body);
void orbit_parts_step(struct Body *body);
void set_motion_from_value(struct Body *body, u32 value);
void set_slot_value(u32 id, s32 value);
u32 step_motion(struct Body *body);
void obj_release(struct Body *body);
void state_stack_advance_progress(struct Body *body);
void hero_a_move_to_partner_target(struct Body *body);
void hero_advance_and_pose(struct Body *body);
void hero_pose_and_fx(struct Body *body);
void hero_a_step_end_fx(struct Body *body);
void move_base_toward_target(struct Body *body);
void enter_finish_rebuild(struct Body *body);
void hero_a_move_to_partner_step(struct Body *body);
void find_frame_entry(struct Body *body, u32 index);
void hero_step_and_set_pose(struct Body *body);
void hero_pair_place_finish_t3cc(struct Body *body);
void set_motion_start(struct Body *body, u32 speed);
void sprite_set_action(struct Body *body, u32 action, s32 arg, u32 extra);
void place_targets(struct Body *body, s32 x, s32 y, s32 z, s32 flag);
void set_motion_offsets(struct Body *body, u32 a);
void schedule_pose_task(struct Body *body);

ASM_FUNC("asm/code_080A048C/place_chain_nodes.s", void place_chain_nodes(void))

ASM_FUNC("asm/code_080A048C/build_twin_part_chain.s", void build_twin_part_chain(struct Body *body))

/* Starts an orbiting body's spiral-in: gEllipseStepScript holds 0-terminated scripts and body->height
   picks one. Reads its first step (its sign is the turn direction), sets the radius to close in to
   (sqrt(0x4C4) / 2 = 17), the angle where the step ends (the next half turn in the turn direction) and
   the angular speed (step * 8), then hands over to move_base_toward_target. */
void start_script_step(struct Body *body)
{
    s32 scriptsToSkip;
    u32 pos;
    s32 step;
    s32 byte;
    s32 root;
    u8 angle;
    s32 signedAngle;
    register s16 stepEnd asm("r1");

    pos = 0;
    scriptsToSkip = body->height;
    if (scriptsToSkip > 0)
    {
        for (;;)
        {
            step = (s8)gEllipseStepScript[pos];
            pos++;
            scriptsToSkip--;
            if (step != 0)
            {
                do
                {
                    byte = (s8)gEllipseStepScript[pos];
                    pos++;
                } while (byte != 0);
            }
            if (scriptsToSkip <= 0)
            {
                break;
            }
        }
    }
    body->scriptPos = pos;
    body->scriptB0 = (s8)gEllipseStepScript[pos];
    body->scriptPos = pos + 1;
    /* gMathRoutines + 0 is the square root */
    root = ((s32 (*)(s32))gMathRoutines)(0x4C4);
    body->speedZ = root / 2;
    if ((s16)body->scriptB0 < 0)
    {
        angle = BODY_ORBIT_ANGLE(body) / 0x100;
        stepEnd = 0x80;
        if (angle <= 0x80)
            stepEnd = -0x80;
        body->timer = stepEnd;
    }
    else
    {
        signedAngle = (s8)(BODY_ORBIT_ANGLE(body) / 0x100);
        stepEnd = 0x80;
        if (signedAngle < 0)
            stepEnd = 0x180;
        body->timer = stepEnd;
    }
    body->baseY = (s16)body->scriptB0 << 3;
    body->state = (u32)move_base_toward_target;
}

void obj_list_push_default(struct Body *obj);
void sound_stop_song(u32 song);
u32 link_to_partner_fx(struct Body *body, struct Body *partner, u32 mode);
void swing_then_draw(struct Body *body);
/*
 * Orbiting body, spiral-in phase: eases the angular speed toward the script step's (step * 16) and
 * the radius down toward speedZ; once the radius gets there, the body and its child become
 * collidable (link_to_partner_fx) and sound 0x10F starts. The sprite grows from 0 to full size
 * (min 4) as the radius closes in, and the body is placed on an ellipse around (0x2F, 0x6C),
 * stretched 2.5x horizontally. Each time the angle passes the step's end, the next script step is
 * read (once the radius has settled) and the end moves a full turn on; step 0 stops the sound and
 * switches to swing_then_draw.
 */
void move_base_toward_target(struct Body *body)
{
    s32 scale;
    s32 angle;
    s32 trig;
    s32 product;
    s32 offsetX;
    s32 stretchedX;
    struct SpriteEntry *sprite;

    body->baseY = (body->baseY * 0xF8 + (s16)body->scriptB0 * 0x80) / 0x100;
    if (body->swing != (s16)body->speedZ << 8)
    {
        body->swing = (body->swing * 0xF8 + (s16)body->speedZ * 0x800) / 0x100;
        if (body->swing - ((s16)body->speedZ << 8) < 0x100)
        {
            body->swing = (s16)body->speedZ << 8;
        }
        if (body->swing == (s16)body->speedZ << 8)
        {
            obj_list_push_default(body);
            obj_list_push_default(body->child);
            body->child->slot58 = (u32)link_to_partner_fx;
            body->slot58 = (u32)link_to_partner_fx;
            set_slot_value(0x10F, -1);
        }
    }

    /* the sprite grows as the radius closes in */
    scale = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x5000 - body->swing, 0x50 - (s16)body->speedZ);
    sprite = body->sprite;
    scale = (scale < 4) ? 4 : scale;
    sprite->unk6 = scale;
    sprite->unk4 = scale;

    /* turn, and place the body on the ellipse */
    BODY_ORBIT_ANGLE(body) += body->baseY;
    angle = BODY_ORBIT_ANGLE(body);
    trig = gCosineTable[(angle / 0x100) & 0xFF] / 0x40;
    product = body->swing * trig;
    if (product < 0)
    {
        product += 0xFF;
    }
    offsetX = product >> 8;
    stretchedX = ((offsetX << 2) + offsetX) << 7;
    if (stretchedX < 0)
    {
        stretchedX += 0xFF;
    }
    body->x = 0x2F00 - (stretchedX >> 8);
    trig = gSineTable[(BODY_ORBIT_ANGLE(body) / 0x100) & 0xFF] / 0x40;
    product = body->swing * trig;
    body->y = 0x6C00 - (product / 0x100);

    /* wait until the angle passes the current step's end */
    if ((s16)body->scriptB0 < 0)
    {
        if ((s16)body->timer << 8 < BODY_ORBIT_ANGLE(body))
            return;
    }
    else
    {
        if (BODY_ORBIT_ANGLE(body) < (s16)body->timer << 8)
            return;
    }
    if (body->swing == (s16)body->speedZ << 8)
    {
        body->scriptB0 = (s8)gEllipseStepScript[body->scriptPos++];
    }
    if ((s16)body->scriptB0 > 0)
    {
        body->timer += 0x100;
    }
    else if ((s16)body->scriptB0 < 0)
    {
        body->timer -= 0x100;
    }
    else
    {
        sound_stop_song(0x10F);
        body->timer = 0;
        body->state = (u32)swing_then_draw;
    }
}

/*
 * Orbiting body, spiral-out phase (after move_base_toward_target's script ends): the radius grows
 * by a step that speeds up every frame (timer, +0x33 per frame) while the body keeps turning at its
 * last angular speed. The sprite shrinks back down (min 4), and the body is placed on the same
 * ellipse around (0x2F, 0x6C). Once the radius reaches 0x50 pixels, the body, its child and the
 * child's child are released.
 */
void swing_then_draw(struct Body *body)
{
    s32 radius;
    s32 scale;
    s32 angle;
    s32 trig;
    s32 product;
    s32 offsetX;
    s32 stretchedX;
    struct SpriteEntry *sprite;

    /* timer is the outward speed in this phase */
    body->timer += 0x33;
    radius = body->swing + (s16)body->timer;
    body->swing = radius;
    if (radius <= 0x4FFF)
    {
        /* the sprite shrinks as the radius grows */
        scale = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x5000 - radius, 0x50 - (s16)body->speedZ);
        sprite = body->sprite;
        scale = (scale < 4) ? 4 : scale;
        sprite->unk6 = scale;
        sprite->unk4 = scale;

        /* turn, and place the body on the ellipse */
        BODY_ORBIT_ANGLE(body) += body->baseY;
        angle = BODY_ORBIT_ANGLE(body);
        trig = gCosineTable[(angle / 0x100) & 0xFF] / 0x40;
        product = body->swing * trig;
        if (product < 0)
        {
            product += 0xFF;
        }
        offsetX = product >> 8;
        stretchedX = ((offsetX << 2) + offsetX) << 7;
        if (stretchedX < 0)
        {
            stretchedX += 0xFF;
        }
        body->x = 0x2F00 - (stretchedX >> 8);
        trig = gSineTable[(BODY_ORBIT_ANGLE(body) / 0x100) & 0xFF] / 0x40;
        product = body->swing * trig;
        body->y = 0x6C00 - (product / 0x100);
    }
    else
    {
        obj_release(body->child->child);
        obj_release(body->child);
        obj_release(body);
    }
}

static inline void copy_sprite_unk4(struct Body *body, struct Body *head)
{
    struct SpriteEntry *bodySpr;
    struct SpriteEntry *childSpr;
    struct SpriteEntry *headSpr;

    bodySpr = body->sprite;
    childSpr = body->child->sprite;
    headSpr = head->sprite;
    childSpr->unk4 = headSpr->unk4;
    bodySpr->unk4 = headSpr->unk4;
}

static inline void copy_sprite_unk6(struct Body *body, struct Body *head)
{
    struct SpriteEntry *bodySpr;
    struct SpriteEntry *childSpr;
    struct SpriteEntry *headSpr;

    bodySpr = body->sprite;
    childSpr = body->child->sprite;
    headSpr = head->sprite;
    childSpr->unk6 = headSpr->unk6;
    bodySpr->unk6 = headSpr->unk6;
}

/*
 * Orbiting pair, follower's update: shifts the child's chain of trail nodes one step (each node
 * takes the next one's position; node 9 restarts at the leader's target, node 19 at this body's
 * target), toggles bit 7 of every 5th node, then puts the body half a turn round the leader's
 * ellipse (centre (0x2F, 0x6C), stretched 2.5x horizontally) at the leader's height, its sprites
 * scaled like the leader's.
 */
void place_members_on_orbit(struct Body *body)
{
    u32 n;
    struct Body *leader;
    struct OrbitNode *node;
    s32 *radius;
    s32 *angle;
    const s16 *cosTab;
    s32 a;
    s32 trig;
    s32 product;
    s32 offsetX;
    s32 stretchedX;

    leader = body->data->leader;
    n = 0;
    node = (struct OrbitNode *)((u8 *)body->child + 0xC);
    while (node != NULL)
    {
        /* n % 5 */
        if (MATH_ROUTINE(u32 (*)(u32, u32), iwram_math_08199424)(n, 5) == 0)
        {
            ((struct OrbitFlagBits *)&node->unk1B)->bit7 = ((struct OrbitFlagBits *)&node->unk1B)->bit7 ^ 1;
        }
        if (n == 9)
        {
            node->unk4 = leader->targetX;
            node->unk8 = leader->targetY;
        }
        else if (n == 19)
        {
            node->unk4 = body->targetX;
            node->unk8 = body->targetY;
        }
        else
        {
            node->unk4 = node->next->unk4;
            node->unk8 = node->next->unk8;
        }
        node = node->next;
        n++;
    }

    /* the leader's angle plus half a turn */
    radius = &leader->swing;
    cosTab = gCosineTable;
    angle = &BODY_ORBIT_ANGLE(leader);
    a = *angle;
    if (a < 0)
    {
        a += 0xFF;
    }
    trig = cosTab[((a >> 8) + 0x80) & 0xFF] / 0x40;
    product = *radius * trig;
    if (product < 0)
    {
        product += 0xFF;
    }
    offsetX = product >> 8;
    stretchedX = ((offsetX << 2) + offsetX) << 7;
    if (stretchedX < 0)
    {
        stretchedX += 0xFF;
    }
    body->x = 0x2F00 - (stretchedX >> 8);
    trig = gSineTable[((*angle / 0x100) + 0x80) & 0xFF] / 0x40;
    product = *radius * trig;
    body->y = 0x6C00 - (product / 0x100);
    body->z = leader->z;
    copy_sprite_unk4(body, leader);
    copy_sprite_unk6(body, leader);
}

u32 link_to_partner_fx(struct Body *body, struct Body *partner, u32 mode)
{
    effect_spawn_at(0x2ADB, body->x / 0x100, body->y / 0x100, body->z / 0x100, partner);
    return partner_link_update(body, partner, mode);
}

void end_anim_when_active(struct Body *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0, 0);
        body->state = 0;
    }
}

void end_anim_when_active_b(struct Body *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0, 0);
        body->state = 0;
    }
}

void settle_members_then_orbit(struct Body *body)
{
    u32 movedBody;
    u32 movedChild;
    s32 negBody;
    s32 negChild;

    body->moveProgress = 0;
    movedBody = step_motion(body);
    movedChild = step_motion(body->child);
    negBody = -(u8)movedBody;
    negChild = -(u8)movedChild;
    if ((negChild | negBody) >= 0)
    {
        *(s32 *)&body->spanA = 0xD800;
        *(s32 *)&body->child->spanA = 0x2800;
        set_motion_from_value(body, 0x3C);
        body->state = (u32)orbit_parts_step;
    }
}

s32 set_state_fx_twin_chain(struct Body *body)
{
    body->state = (u32)start_fx_then_twin_chain;
    return 1;
}

s32 set_state_fx_part_chain(struct Body *body)
{
    body->state = (u32)start_fx_then_part_chain;
    return 1;
}

void start_fx_then_twin_chain(struct Body *body)
{
    sprite_set_action(body, 0x20, 0, 0);
    ((struct SpriteModeByte *)&body->sprite->flags)->mode = 1;
    effect_spawn_at(0x2AC7, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
    set_slot_value(0xD5, -1);
    body->state = (u32)build_twin_part_chain;
}

void start_fx_then_part_chain(struct Body *body)
{
    sprite_set_action(body, 0x18, 0, 0);
    ((struct SpriteModeByte *)&body->sprite->flags)->mode = 0;
    effect_spawn_at(0x2AAF, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
    body->state = (u32)build_part_chain;
}

u32 pair_heroes_with_partner(struct Body *body)
{
    struct Hero *heroB;
    struct Body *bodyA;
    struct Body *bodyB;
    struct Body *partner;
    u8 *table;
    u32 anim;

    table = gParty - (body->data->pose * 60 + 0x3C);
    body->data->poseVal = ((struct PoseTable *)table)->value2A;
    bodyA = &gWorld->heroA->body;
    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    partner = body->partner;
    bodyB->partner = partner;
    bodyA->partner = partner;
    anim = body->anim;
    bodyB->anim = anim;
    bodyA->anim = anim;
    partner->slot50 = 0;
    bodyB->slot50 = 0;
    bodyA->slot50 = 0;
    partner->speed = 0x100;
    bodyB->speed = 0x100;
    bodyA->speed = 0x100;
    body->state = (u32)hero_pose_and_fx;
    bodyB->count = 0;
    bodyA->count = 0;
    schedule_pose_task(partner);
    return 1;
}

/*
 * Cutscene step: sets the body's playback speed from its data flags (0x400: 0x199
 * with sprite frame 25, 0x800: 0x66 with frame 6; 0x100 is normal), spawns effect 1
 * on hero A and walks hero A to 24 px right of its data target at speed 0x300, with
 * its walk action (0x2002 with no pose, else 0x204F) and facing the way it walks.
 * Hero A is only touched while its mode is 2 or 4. Goes on to hero_step_and_set_pose.
 */
void hero_pose_and_fx(struct Body *body)
{
    struct Hero *hero;
    struct Body *bodyA;
    s16 *move_x;

    hero = gWorld->heroA;
    bodyA = &hero->body;
    if (body->data->flags & 0x400)
    {
        body->speed = 0x199;
        body->sprite->frame = 0x19;
    }
    if (body->data->flags & 0x800)
    {
        body->speed = 0x66;
        body->sprite->frame = 6;
    }
    effect_spawn_at(1, bodyA->x / 0x100, bodyA->y / 0x100, bodyA->z / 0x100, bodyA);
    if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
    {
        place_targets(bodyA, bodyA->data->targetX / 0x100 + 0x18, bodyA->data->targetY / 0x100,
                      bodyA->data->targetZ / 0x100, -1);
        set_motion_start(bodyA, 0x300);
    }
    if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
    {
        if (bodyA->data->pose == -1)
        {
            sprite_set_action(bodyA, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(bodyA, 0, 0x204F, 0);
        }
        /* face left when the walk goes towards smaller X */
        ((struct SpriteMirrorByte *)&bodyA->sprite->unk11)->mirror = (move_x = &bodyA->spanA)[0] >= move_x[1];
        ((struct SpriteModeByte *)&bodyA->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_step_and_set_pose;
}

void hero_step_and_set_pose(struct Body *body)
{
    struct Hero *hero;

    hero = gWorld->heroA;
    step_motion(&hero->body);
    if (hero->body.flags & 0x20)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            hero->body.progress = 0;
            ((struct BodyFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = (u32)hero_advance_and_pose;
    }
}

void hero_advance_and_pose(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;

    hero = gWorld->heroA;
    hbody = &hero->body;
    state_stack_advance_progress(hbody);
    if (hero->body.flags & 0x20)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 4, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 4, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        body->state = (u32)hero_a_move_to_partner_target;
    }
}

/*
 * Once hero A's animation has finished: plays sound 0x2B and sends hero A on an
 * arc onto its target (higher the higher the target stands), in its jump pose,
 * then hands over to hero_a_move_to_partner_step.
 */
void hero_a_move_to_partner_target(struct Body *body)
{
    struct Hero *hero;
    struct Body *heroA;
    struct Body *target;
    s32 x;
    s32 y;
    s32 z;
    s32 height;

    hero = gWorld->heroA;
    heroA = &hero->body;
    target = body->partner;
    /* sprite flags bit 3: the animation has finished */
    if (heroA->sprite->flags & 8)
    {
        set_slot_value(0x2B, -1);
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
        {
            x = target->targetX / 0x100 + target->ofsX;
            y = target->targetY / 0x100 + target->ofsY;
            z = target->targetZ / 0x100 + target->ofsZ;
            place_targets(heroA, x, y, z, -1);
            /* the target's own body uses its offset, a part its current frame */
            if ((u8 *)target == (u8 *)target->data + 8)
            {
                height = target->ofsZ;
            }
            else
            {
                find_frame_entry(target, 0);
                height = target->unk47;
            }
            height += target->targetZ / 0x100;
            /* the jump's peak: the target's height plus 0x5A4 / (height^2 / 0x40 + 0x26) */
            height += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x5A4, (height * height) / 0x40 + 0x26);
            heroA->arc = height;
            set_motion_offsets(heroA, 0x680);
        }
        if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
        {
            if (heroA->data->pose == -1)
            {
                sprite_set_action(heroA, 5, 0x2000, 0);
            }
            else
            {
                sprite_set_action(heroA, 5, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&heroA->sprite->flags)->mode = 1;
        }
        heroA->timer &= 0xFFF;
        body->state = (u32)hero_a_move_to_partner_step;
    }
}

ASM_FUNC("asm/code_080A048C/hero_a_move_to_partner_step.s", void hero_a_move_to_partner_step(struct Body *body))

void hero_a_arm_full_step(struct Body *body)
{
    struct Hero *hero;

    hero = gWorld->heroA;
    if (hero->body.sprite->flags & 8)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = 0x4000;
            hero->body.progress = 0;
            ((struct BodyFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = (u32)hero_a_advance_and_place;
    }
}

/* State handler: advances hero A's progress; once it is done (flag 0x20), hero A (if on the field)
 * jumps to 0x2E px left of its own target X/Y plus its offsets, at height 0, in an arc 0x10 above
 * its target height (speed 0x633), and hero_a_step_end_fx takes over. */
void hero_a_advance_and_place(struct Body *body)
{
    struct Hero *hero;
    struct Body *heroA;
    s32 x;
    s32 y;

    hero = gWorld->heroA;
    heroA = &hero->body;
    state_stack_advance_progress(heroA);
    if (hero->body.flags & 0x20)
    {
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            x = heroA->targetX / 0x100 + hero->body.ofsX - 0x2E;
            y = heroA->targetY / 0x100 + hero->body.ofsY;
            place_targets(heroA, x, y, 0, -1);
            heroA->arc = heroA->targetZ / 0x100 + 0x10;
            set_motion_offsets(heroA, 0x633);
        }
        heroA->timer &= 0xFFF;
        body->state = (u32)hero_a_step_end_fx;
    }
}

/*
 * State handler: moves hero A along its jump arc. In the frame the arc passes its
 * apex, hero A switches to its falling pose (action 6); once it has landed it plays
 * sound 0x2E, takes its landing pose, spawns effect 0x33 where it stands and hands
 * over to hero_pair_place_finish_t3cc.
 */
void hero_a_step_end_fx(struct Body *body)
{
    struct Hero *hero;
    struct Body *heroA;
    s32 passedApex;
    register s32 apex asm("r2");
    s32 progress;
    s32 span;
    s32 rise;

    hero = gWorld->heroA;
    heroA = &hero->body;
    step_motion(heroA);
    span = hero->body.arcSpan;
    rise = hero->body.arcRise;
    /* the arc is highest where arcSpan * progress / 0x4000 == arcRise */
    apex = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(rise << 14, span);
    if (hero->body.prevProgress <= apex)
    {
        progress = hero->body.progress;
        passedApex = 1;
        if (progress >= apex)
            goto done;
    }
    passedApex = 0;
done:
    if (passedApex)
    {
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
        {
            if (heroA->data->pose == -1)
            {
                sprite_set_action(heroA, 6, 0x2000, 0);
            }
            else
            {
                sprite_set_action(heroA, 6, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&heroA->sprite->flags)->mode = 1;
        }
    }
    /* flags bit 5: step_motion has reached the end of the motion */
    if (heroA->flags & 0x20)
    {
        set_slot_value(0x2E, -1); /* plays sound 0x2E */
        if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
        {
            sprite_set_action(heroA, 0, 0x2004, 0);
            ((struct SpriteModeByte *)&heroA->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0x33, heroA->x / 0x100, heroA->y / 0x100, heroA->z / 0x100, heroA);
        body->state = (u32)hero_pair_place_finish_t3cc;
    }
}

/*
 * State handler: once hero A's sprite animation has finished, send both heroes (A first,
 * then B) walking back to the home positions stored in their BodyData at speed 0x3CC.
 * Each hero on the field gets the walk action (0x204F if it has a pose, else 0x2002),
 * faces the way it walks, and then hands over to the finish-rebuild state.
 */
void hero_pair_place_finish_t3cc(struct Body *body)
{
    struct Body *hero_a;
    struct Body *hero_b;
    s16 *move_x;

    hero_a = &gWorld->heroA->body;
    hero_b = &gWorld->heroB->body;
    if (hero_a->sprite->flags & 8)
    {
        if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
        {
            place_targets(hero_a, hero_a->data->targetX / 0x100, hero_a->data->targetY / 0x100,
                          hero_a->data->targetZ / 0x100, -1);
            set_motion_start(hero_a, 0x3CC);
        }
        if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
        {
            if (hero_a->data->pose == -1)
            {
                sprite_set_action(hero_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(hero_a, 0, 0x204F, 0);
            }
            /* face left when the walk goes towards smaller X */
            ((struct SpriteMirrorByte *)&hero_a->sprite->unk11)->mirror = (move_x = &hero_a->spanA)[0] >= move_x[1];
            ((struct SpriteModeByte *)&hero_a->sprite->flags)->mode = 0;
        }
        if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
        {
            hero_a->state = 0;
            enter_finish_rebuild(hero_a);
        }
        if ((hero_b->mode & 6) == 2 || (hero_b->mode & 6) == 4)
        {
            place_targets(hero_b, hero_b->data->targetX / 0x100, hero_b->data->targetY / 0x100,
                          hero_b->data->targetZ / 0x100, -1);
            set_motion_start(hero_b, 0x3CC);
        }
        if ((hero_b->mode & 6) == 2 || (hero_b->mode & 6) == 4)
        {
            if (hero_b->data->pose == -1)
            {
                sprite_set_action(hero_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(hero_b, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&hero_b->sprite->unk11)->mirror = (move_x = &hero_b->spanA)[0] >= move_x[1];
            ((struct SpriteModeByte *)&hero_b->sprite->flags)->mode = 0;
        }
        if ((hero_b->mode & 6) == 2 || (hero_b->mode & 6) == 4)
        {
            hero_b->state = 0;
            enter_finish_rebuild(hero_b);
        }
    }
}
