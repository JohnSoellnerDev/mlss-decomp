#include "global.h"
#include "ram_routines.h"

struct OrbitNode
{
    struct OrbitNode *next;
    s32 unk4;
    s32 unk8;
    u8 filler0C[0x1B - 0x0C];
    u8 unk1B;
};

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
    u8 filler54[0x6C - 0x54];
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
    s16 spanA;
    s16 spanB;
    s32 baseY;
    s32 swing;
    u8 filler90[0x92 - 0x90];
    u16 arc;
    s32 motionX;
    s32 motionY;
    s32 height;
    u8 fillerA0[0xA8 - 0xA0];
    u32 scriptPos;
    s16 stepLen;
    u16 timer;
    u16 scriptB0;
    u16 speedZ;
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

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern s8 gEllipseStepScript[];

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

NONMATCH("asm/code_080A048C/start_script_step.s", void start_script_step(struct Body *body))
{
    s32 idx;
    s32 height;
    s32 c;
    s32 t;
    s32 v;
    u8 near;

    idx = 0;
    height = body->height;
    while (height > 0)
    {
        c = gEllipseStepScript[idx];
        idx++;
        height--;
        if (c != 0)
        {
            do
            {
                c = gEllipseStepScript[idx];
                idx++;
            } while (c != 0);
        }
    }
    body->scriptPos = idx;
    body->scriptB0 = gEllipseStepScript[idx];
    body->scriptPos = idx + 1;
    body->speedZ = ((s32)((u32 (*)(u32))gMathRoutines)(0x4C4)) / 2;
    if ((s16)body->scriptB0 < 0)
    {
        t = *(s32 *)&body->spanA;
        if (t < 0)
        {
            t += 0xFF;
        }
        near = (u8)(t >> 8);
        v = 0x80;
        if (near <= 0x80)
        {
            v = -0x80;
        }
    }
    else
    {
        t = *(s32 *)&body->spanA;
        if (t < 0)
        {
            t += 0xFF;
        }
        t = t << 16;
        v = 0x80;
        if (t < 0)
        {
            v = 0x180;
        }
    }
    body->timer = v;
    body->baseY = (s16)body->scriptB0 * 8;
    body->state = (u32)move_base_toward_target;
}
END_NONMATCH

ASM_FUNC("asm/code_080A048C/move_base_toward_target.s", void move_base_toward_target(struct Body *body))

ASM_FUNC("asm/code_080A048C/swing_then_draw.s", void swing_then_draw(void))

ASM_FUNC("asm/code_080A048C/place_members_on_orbit.s", void place_members_on_orbit(void))

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

s32 unknown_part_steps_080a1068(struct Body *body)
{
    body->state = (u32)start_fx_then_twin_chain;
    return 1;
}

s32 sub_080A1074(struct Body *body)
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

    table = gUnk_03000FF4 - (body->data->pose * 60 + 0x3C);
    body->data->poseVal = ((struct PoseTable *)table)->value2A;
    bodyA = &gUnk_03000FD8->heroA->body;
    heroB = gUnk_03000FD8->heroB;
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

void hero_pose_and_fx(struct Body *body)
{
    struct Hero *hero;
    struct Body *bodyA;
    s16 *span;

    hero = gUnk_03000FD8->heroA;
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
        ((struct SpriteMirrorByte *)&bodyA->sprite->unk11)->mirror = (span = &bodyA->spanA)[0] >= span[1];
        ((struct SpriteModeByte *)&bodyA->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_step_and_set_pose;
}

void hero_step_and_set_pose(struct Body *body)
{
    struct Hero *hero;

    hero = gUnk_03000FD8->heroA;
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

    hero = gUnk_03000FD8->heroA;
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

void hero_a_move_to_partner_target(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;
    struct Body *partner;
    s32 x;
    s32 y;
    s32 z;
    s32 dist;

    hero = gUnk_03000FD8->heroA;
    hbody = &hero->body;
    partner = body->partner;
    if (hbody->sprite->flags & 8)
    {
        set_slot_value(0x2B, -1);
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            x = partner->targetX / 0x100 + partner->ofsX;
            y = partner->targetY / 0x100 + partner->ofsY;
            z = partner->targetZ / 0x100 + partner->ofsZ;
            place_targets(hbody, x, y, z, -1);
            if ((u8 *)partner == (u8 *)partner->data + 8)
            {
                dist = partner->ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                dist = partner->unk47;
            }
            dist += partner->targetZ / 0x100;
            dist += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x5A4, (dist * dist) / 0x40 + 0x26);
            hbody->arc = dist;
            set_motion_offsets(hbody, 0x680);
        }
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 5, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 5, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        hbody->timer &= 0xFFF;
        body->state = (u32)hero_a_move_to_partner_step;
    }
}

ASM_FUNC("asm/code_080A048C/hero_a_move_to_partner_step.s", void hero_a_move_to_partner_step(struct Body *body))

void hero_a_arm_full_step(struct Body *body)
{
    struct Hero *hero;

    hero = gUnk_03000FD8->heroA;
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

void hero_a_advance_and_place(struct Body *body)
{
    struct Hero *heroA;
    struct Body *bodyA;
    s32 x;
    s32 y;

    heroA = gUnk_03000FD8->heroA;
    bodyA = &heroA->body;
    state_stack_advance_progress(bodyA);
    if (heroA->body.flags & 0x20)
    {
        if ((heroA->body.mode & 6) == 2 || (heroA->body.mode & 6) == 4)
        {
            x = bodyA->targetX / 0x100 + heroA->body.ofsX - 0x2E;
            y = bodyA->targetY / 0x100 + heroA->body.ofsY;
            place_targets(bodyA, x, y, 0, -1);
            bodyA->arc = bodyA->targetZ / 0x100 + 0x10;
            set_motion_offsets(bodyA, 0x633);
        }
        bodyA->timer &= 0xFFF;
        body->state = (u32)hero_a_step_end_fx;
    }
}

ASM_FUNC("asm/code_080A048C/hero_a_step_end_fx.s", void hero_a_step_end_fx(struct Body *body))

void hero_pair_place_finish_t3cc(struct Body *body)
{
    struct Body *bodyA;
    struct Body *bodyB;
    s16 *span;

    bodyA = &gUnk_03000FD8->heroA->body;
    bodyB = &gUnk_03000FD8->heroB->body;
    if (bodyA->sprite->flags & 8)
    {
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            place_targets(bodyA, bodyA->data->targetX / 0x100, bodyA->data->targetY / 0x100,
                          bodyA->data->targetZ / 0x100, -1);
            set_motion_start(bodyA, 0x3CC);
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
            ((struct SpriteMirrorByte *)&bodyA->sprite->unk11)->mirror = (span = &bodyA->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&bodyA->sprite->flags)->mode = 0;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            place_targets(bodyB, bodyB->data->targetX / 0x100, bodyB->data->targetY / 0x100,
                          bodyB->data->targetZ / 0x100, -1);
            set_motion_start(bodyB, 0x3CC);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->data->pose == -1)
            {
                sprite_set_action(bodyB, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&bodyB->sprite->unk11)->mirror = (span = &bodyB->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&bodyB->sprite->flags)->mode = 0;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}
