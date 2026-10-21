#include "global.h"
#include "ram_routines.h"

struct BodyData
{
    u8 filler0[0xD8];
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

struct SpriteEntry
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
};

struct Body
{
    u8 filler0[8];
    struct SpriteEntry *sprite;
    u8 filler0C[4];
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct BodyData *data;
    struct Body *partner;
    u8 filler30[0x38 - 0x30];
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
    u8 filler88[0x92 - 0x88];
    u16 arc;
    s32 motionX;
    s32 motionY;
    u8 filler9C[0xAC - 0x9C];
    s16 stepLen;
    u16 timer;
    u8 fillerB0[0xB2 - 0xB0];
    u16 speedZ;
    u8 fillerB4[0xC3 - 0xB4];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 fillerC6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
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

void sprite_set_action(struct Body *body, u32 action, s32 arg, u32 extra);
void hero_b_advance_and_place(struct Body *body);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct Body *owner);
void enter_finish_rebuild(struct Body *body);
void obj_slot_update(struct Body *body);
void set_motion_offsets(struct Body *body, u32 a);
s32 resolve_contact_action(struct Body *a, struct Body *b, u32 c, u32 d, u32 e);
struct CcBits
{
    u8 lo7 : 7;
    u8 top : 1;
};
void hero_a_link_when_ready(struct Body *body);
void hero_a_begin_step_rate6(struct Body *body);
void hero_a_place_at_data_target(struct Body *body);
void hero_pair_step_and_place(struct Body *body);
void hero_a_end_anim_clear(struct Body *body);
void hero_b_release(struct Body *body);
void hero_b_begin_step_rate2(struct Body *body);
void hero_b_advance_anim4(struct Body *body);
void hero_b_move_to_partner_target(struct Body *body);
void hero_b_advance_and_place(struct Body *body);
void hero_b_step_end_fx(struct Body *body);
void hero_b_link_when_ready(struct Body *body);
void hero_b_begin_step_rate6(struct Body *body);
void hero_b_place_at_data_target(struct Body *body);
void hero_pair_meet(struct Body *body);
void hero_pair_place_both_finish_t399(void);

void step_motion(struct Body *body);
void state_stack_advance_progress(struct Body *body);
void step_motion_then_end(struct Body *body);
void enter_body_speed_z_step(struct Body *body);
void enter_body_fall_with_speed(struct Body *body);
void hero_a_begin_step(struct Body *body);
void body_step_then_end(struct Body *body);
void place_targets(struct Body *body, s32 x, s32 y, s32 z, s32 flag);
void set_motion_start(struct Body *body, u32 speed);
void body_fall_with_speed(struct Body *body);
void body_speed_z_step(struct Body *body);

void hero_a_link_when_ready(struct Body *body)
{
    struct Body *hbody;
    struct Body *partner;
    s32 linked;

    hbody = &gWorld->heroA->body;
    partner = body->partner;
    if (partner->state == 0 && partner->targetZ > 0x800)
    {
        partner->partner = body;
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
        {
            partner->state = (u32)enter_body_speed_z_step;
        }
    }
    set_slot_value(0x63, -1);
    ((struct CcBits *)&hbody->cc)->lo7 = 0;
    hbody->scale = 0x100;
    linked = resolve_contact_action(hbody, partner, 0, 0, 0x040A1100);
    hbody->count++;
    if (linked == 1)
    {
        obj_slot_update(hbody);
    }
    else
    {
        body->state = (u32)hero_a_begin_step_rate6;
    }
}

void hero_a_begin_step_rate6(struct Body *body)
{
    struct Hero *hero;

    hero = gWorld->heroA;
    if (hero->body.sprite->flags & 8)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 6);
            hero->body.progress = 0;
            ((struct BodyFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = (u32)hero_a_place_at_data_target;
    }
}

void hero_a_place_at_data_target(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;
    struct BodyData *data;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;

    hero = gWorld->heroA;
    hbody = &hero->body;
    state_stack_advance_progress(hbody);
    if (hero->body.flags & 0x20)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            data = hbody->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(hbody, px, py, pz, -1);
            t = hbody->targetZ;
            if (t < 0)
                t += 0xff;
            hbody->arc = (t >> 8) + 0x16;
            set_motion_offsets(hbody, 0x833);
        }
        hbody->timer &= 0xFFF;
        body->state = (u32)hero_pair_step_and_place;
    }
}

NONMATCH("asm/code_080A1DC8/hero_pair_step_and_place.s", void hero_pair_step_and_place(struct Body *body))
{
    struct Hero *hero;
    struct Body *hbody;
    struct Body *bodyB;
    struct BodyData *data;
    s32 ratio;
    s32 mx;
    s32 my;
    s32 reached;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s16 *span;

    hero = gWorld->heroA;
    hbody = &hero->body;
    bodyB = &gWorld->heroB->body;
    step_motion(hbody);
    mx = hero->body.motionX;
    my = hero->body.motionY;
    ratio = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(my << 14, mx);
    reached = 0;
    if (hero->body.stepLen <= ratio)
    {
        reached = 1;
        if (hero->body.progress < ratio)
            reached = 0;
    }
    if (reached)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 6, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 6, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            data = bodyB->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyB, px, py, pz, -1);
            set_motion_start(bodyB, 0x400);
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
    if (hbody->flags & 0x20)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 8, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 8, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        px = hbody->x;
        if (px < 0)
            px += 0xff;
        px >>= 8;
        py = hbody->y;
        if (py < 0)
            py += 0xff;
        py >>= 8;
        pz = hbody->z;
        if (pz < 0)
            pz += 0xff;
        pz >>= 8;
        effect_spawn_at(0x1E, px, py, pz, hbody);
        body->state = (u32)hero_a_end_anim_clear;
    }
}
END_NONMATCH

void hero_a_end_anim_clear(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;

    hero = gWorld->heroA;
    hbody = &hero->body;
    if (hbody->sprite->flags & 8)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            sprite_set_action(hbody, 0, -1, 0);
            ((struct SpriteMirrorByte *)&hbody->sprite->unk11)->mirror = 0;
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
        }
        body->state = 0;
        body->unk74 = 0;
    }
}

void body_speed_z_step(struct Body *body)
{
    struct BodyData *data;
    struct Body *partner;
    s32 px;
    s32 py;
    s32 pz;
    s32 t;

    partner = body->partner;
    body->speedZ += 0x26;
    if ((s16)body->speedZ < 0)
    {
        body->z += (s16)body->speedZ;
        if (partner->targetZ == partner->z)
        {
            partner->z = partner->targetZ + (s16)body->speedZ;
        }
        return;
    }
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        data = body->data;
        t = data->targetX;
        if (t < 0)
            t += 0xff;
        px = t >> 8;
        t = data->targetY;
        if (t < 0)
            t += 0xff;
        py = t >> 8;
        t = data->targetZ;
        if (t < 0)
            t += 0xff;
        pz = t >> 8;
        place_targets(body, px, py, pz, -1);
        set_motion_start(body, 0x100);
    }
    body->state = (u32)step_motion_then_end;
}

void step_motion_then_end(struct Body *body)
{
    step_motion(body);
    if (body->flags & 0x20)
    {
        body->state = 0;
        body->unk74 = 0;
    }
}

void enter_body_speed_z_step(struct Body *body)
{
    body->speedZ = 0xFE4D;
    body->state = (u32)body_speed_z_step;
}

extern u8 *gParty;
void schedule_pose_task(struct Body *body);
void hero_b_place_with_effect(struct Body *body);
u32 hero_pair_setup_pose(struct Body *body)
{
    u8 **party = &gParty;
    struct BodyData *data = body->data;
    struct Body *partner;
    struct Body *body_a;
    struct Body *body_b;
    u32 anim;
    u8 *entry;

    entry = *party - (data->pose * 0x3C + 0x3C);
    data->poseVal = *(u16 *)(entry + 0x2A);
    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->slot50 = 0;
    body_b->slot50 = 0;
    body_a->slot50 = 0;
    partner->speed = 0x100;
    body_b->speed = 0x100;
    body_a->speed = 0x100;
    body->state = (u32)hero_b_place_with_effect;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}
void hero_b_place_with_effect(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;
    struct BodyData *data;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s16 *span;

    hero = gWorld->heroB;
    hbody = &hero->body;
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
    px = hbody->x;
    if (px < 0)
        px += 0xff;
    px >>= 8;
    py = hbody->y;
    if (py < 0)
        py += 0xff;
    py >>= 8;
    pz = hbody->z;
    if (pz < 0)
        pz += 0xff;
    pz >>= 8;
    effect_spawn_at(1, px, py, pz, hbody);
    if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
    {
        data = hbody->data;
        t = data->targetX;
        if (t < 0)
            t += 0xff;
        px = (t >> 8) + 0x23;
        t = data->targetY;
        if (t < 0)
            t += 0xff;
        py = t >> 8;
        t = data->targetZ;
        if (t < 0)
            t += 0xff;
        pz = t >> 8;
        place_targets(hbody, px, py, pz, -1);
        set_motion_start(hbody, 0x200);
    }
    if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
    {
        if (hbody->data->pose == -1)
        {
            sprite_set_action(hbody, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(hbody, 0, 0x204F, 0);
        }
        ((struct SpriteMirrorByte *)&hbody->sprite->unk11)->mirror = (span = &hbody->spanA)[0] >= span[1];
        ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_b_begin_step_rate2;
}
void hero_b_begin_step_rate2(struct Body *body)
{
    struct Hero *hero;

    hero = gWorld->heroB;
    step_motion(&hero->body);
    if (hero->body.flags & 0x20)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            hero->body.progress = 0;
            ((struct BodyFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = (u32)hero_b_advance_anim4;
    }
}

void hero_b_advance_anim4(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;

    hero = gWorld->heroB;
    hbody = &hero->body;
    state_stack_advance_progress(hbody);
    if (hero->body.flags & 0x20)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
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
        body->state = (u32)hero_b_move_to_partner_target;
    }
}

void find_frame_entry(struct Body *body, u32 index);
void hero_b_move_to_partner_step(struct Body *body);
void hero_b_move_to_partner_target(struct Body *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct Body *body_b = &hero_b->body;
    struct Body *partner = body->partner;
    s32 t;
    s32 x;
    s32 y;
    s32 z;
    s32 h;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = partner->targetX / 0x100 + partner->ofsX;
            y = partner->targetY / 0x100 + partner->ofsY;
            z = partner->targetZ / 0x100 + partner->ofsZ;
            place_targets(body_b, x, y, z, -1);
            if (partner == (struct Body *)((u8 *)partner->data + 8))
            {
                h = partner->ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                h = partner->unk47;
            }
            t = partner->targetZ;
            if (t < 0)
                t += 0xFF;
            h += t >> 8;
            t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x6E4, (h * h) / 64 + 0x2A);
            h += t;
            body_b->arc = h;
            set_motion_offsets(body_b, 0x680);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct SpriteModeByte *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = (u32)hero_b_move_to_partner_step;
    }
}
ASM_FUNC("asm/code_080A1DC8/hero_b_move_to_partner_step.s", void hero_b_move_to_partner_step(struct Body *body))

void hero_b_arm_full_step(struct Body *body)
{
    struct Hero *hero;

    hero = gWorld->heroB;
    if (hero->body.sprite->flags & 8)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = 0x4000;
            hero->body.progress = 0;
            ((struct BodyFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = (u32)hero_b_advance_and_place;
    }
}

void hero_b_advance_and_place(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;
    s32 t;
    s32 px;
    s32 py;

    hero = gWorld->heroB;
    hbody = &hero->body;
    state_stack_advance_progress(hbody);
    if (hero->body.flags & 0x20)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            t = hbody->targetX;
            if (t < 0)
                t += 0xff;
            px = (t >> 8) + hero->body.ofsX - 0x38;
            py = hbody->targetY;
            if (py < 0)
                py += 0xff;
            py >>= 8;
            py += hero->body.ofsY;
            place_targets(hbody, px, py, 0, -1);
            t = hbody->targetZ;
            if (t < 0)
                t += 0xff;
            hero->body.arc = (t >> 8) + 0x10;
            set_motion_offsets(hbody, 0x600);
        }
        hbody->timer &= 0xFFF;
        body->state = (u32)hero_b_step_end_fx;
    }
}

NONMATCH("asm/code_080A1DC8/hero_b_step_end_fx.s", void hero_b_step_end_fx(struct Body *body))
{
    struct Hero *hero;
    struct Body *hbody;
    s32 ratio;
    s32 mx;
    s32 my;
    s32 reached;
    s32 px;
    s32 py;
    s32 pz;

    hero = gWorld->heroB;
    hbody = &hero->body;
    step_motion(hbody);
    mx = hero->body.motionX;
    my = hero->body.motionY;
    ratio = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(my << 14, mx);
    if (hero->body.stepLen > ratio)
        reached = 0;
    else
    {
        reached = 1;
        if (hero->body.progress < ratio)
            reached = 0;
    }
    if (reached)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 6, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 6, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
    }
    if (hbody->flags & 0x20)
    {
        set_slot_value(0x2E, -1);
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            sprite_set_action(hbody, 0, 0x2005, 0);
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        px = hbody->x;
        if (px < 0)
            px += 0xff;
        px >>= 8;
        py = hbody->y;
        if (py < 0)
            py += 0xff;
        py >>= 8;
        pz = hbody->z;
        if (pz < 0)
            pz += 0xff;
        pz >>= 8;
        effect_spawn_at(0x33, px, py, pz, hbody);
        body->state = (u32)hero_pair_place_both_finish_t399;
    }
}
END_NONMATCH
void hero_pair_place_both_finish_t399(void)
{
    struct Hero *heroA;
    struct Hero *heroB;
    struct Body *bodyA;
    struct Body *bodyB;
    struct BodyData *data;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s16 *span;

    heroA = gWorld->heroA;
    bodyA = &heroA->body;
    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    if (bodyB->sprite->flags & 8)
    {
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            data = bodyA->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyA, px, py, pz, -1);
            set_motion_start(bodyA, 0x399);
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
            data = bodyB->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyB, px, py, pz, -1);
            set_motion_start(bodyB, 0x399);
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

void hero_b_link_when_ready(struct Body *body)
{
    struct Body *hbody;
    struct Body *partner;
    s32 linked;

    hbody = &gWorld->heroB->body;
    partner = body->partner;
    if (partner->state == 0 && partner->targetZ > 0x800)
    {
        partner->partner = body;
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
        {
            partner->state = (u32)enter_body_fall_with_speed;
        }
    }
    set_slot_value(0x63, -1);
    ((struct CcBits *)&hbody->cc)->lo7 = 0;
    hbody->scale = 0x100;
    linked = resolve_contact_action(hbody, partner, 0, 0, 0x040A1101);
    hbody->count++;
    if (linked == 1)
    {
        obj_slot_update(hbody);
    }
    else
    {
        body->state = (u32)hero_b_begin_step_rate6;
    }
}

void hero_b_begin_step_rate6(struct Body *body)
{
    struct Hero *hero;

    hero = gWorld->heroB;
    if (hero->body.sprite->flags & 8)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 6);
            hero->body.progress = 0;
            ((struct BodyFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = (u32)hero_b_place_at_data_target;
    }
}

void hero_b_place_at_data_target(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;
    struct BodyData *data;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;

    hero = gWorld->heroB;
    hbody = &hero->body;
    state_stack_advance_progress(hbody);
    if (hero->body.flags & 0x20)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            data = hbody->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(hbody, px, py, pz, -1);
            t = hbody->targetZ;
            if (t < 0)
                t += 0xff;
            hbody->arc = (t >> 8) + 0x1A;
            set_motion_offsets(hbody, 0x880);
        }
        hbody->timer &= 0xFFF;
        body->state = (u32)hero_pair_meet;
    }
}

NONMATCH("asm/code_080A1DC8/hero_pair_meet.s", void hero_pair_meet(struct Body *body))
{
    struct Hero *hero;
    struct Body *hbody;
    struct Body *bodyA;
    struct BodyData *data;
    s32 ratio;
    s32 mx;
    s32 my;
    s32 reached;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s16 *span;

    bodyA = &gWorld->heroA->body;
    hero = gWorld->heroB;
    hbody = &hero->body;
    step_motion(hbody);
    mx = hero->body.motionX;
    my = hero->body.motionY;
    ratio = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(my << 14, mx);
    reached = 0;
    if (hero->body.stepLen <= ratio)
    {
        reached = 1;
        if (hero->body.progress < ratio)
            reached = 0;
    }
    if (reached)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 6, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 6, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            data = bodyA->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyA, px, py, pz, -1);
            set_motion_start(bodyA, 0x400);
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
    }
    if (hbody->flags & 0x20)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            if (hbody->data->pose == -1)
            {
                sprite_set_action(hbody, 8, 0x2000, 0);
            }
            else
            {
                sprite_set_action(hbody, 8, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        px = hbody->x;
        if (px < 0)
            px += 0xff;
        px >>= 8;
        py = hbody->y;
        if (py < 0)
            py += 0xff;
        py >>= 8;
        pz = hbody->z;
        if (pz < 0)
            pz += 0xff;
        pz >>= 8;
        effect_spawn_at(0x1E, px, py, pz, hbody);
        body->state = (u32)hero_b_release;
    }
}
END_NONMATCH

void hero_b_release(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;

    hero = gWorld->heroB;
    hbody = &hero->body;
    if (hbody->sprite->flags & 8)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            sprite_set_action(hbody, 0, -1, 0);
            ((struct SpriteMirrorByte *)&hbody->sprite->unk11)->mirror = 0;
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
        }
        body->state = 0;
        body->unk74 = 0;
    }
}

void body_fall_with_speed(struct Body *body)
{
    struct BodyData *data;
    struct Body *partner;
    s32 px;
    s32 py;
    s32 pz;
    s32 t;

    partner = body->partner;
    body->speedZ += 0x26;
    if ((s16)body->speedZ < 0)
    {
        body->z += (s16)body->speedZ;
        if (partner->targetZ == partner->z)
        {
            partner->z = partner->targetZ + (s16)body->speedZ;
        }
        return;
    }
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        data = body->data;
        t = data->targetX;
        if (t < 0)
            t += 0xff;
        px = t >> 8;
        t = data->targetY;
        if (t < 0)
            t += 0xff;
        py = t >> 8;
        t = data->targetZ;
        if (t < 0)
            t += 0xff;
        pz = t >> 8;
        place_targets(body, px, py, pz, -1);
        set_motion_start(body, 0x100);
    }
    body->state = (u32)body_step_then_end;
}

void body_step_then_end(struct Body *body)
{
    step_motion(body);
    if (body->flags & 0x20)
    {
        body->state = 0;
        body->unk74 = 0;
    }
}

void enter_body_fall_with_speed(struct Body *body)
{
    body->speedZ = 0xFE4D;
    body->state = (u32)body_fall_with_speed;
}

void hero_a_go_to_partner(struct Body *body);
u32 hero_pair_start(struct Body *body)
{
    u8 **party = &gParty;
    struct BodyData *data = body->data;
    struct Body *partner;
    struct Body *body_a;
    struct Body *body_b;
    u32 anim;
    u8 *entry;

    entry = *party - (data->pose * 0x3C + 0x3C);
    data->poseVal = *(u16 *)(entry + 0x2A);
    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->slot50 = 0;
    body_b->slot50 = 0;
    body_a->slot50 = 0;
    partner->speed = 0x100;
    body_b->speed = 0x100;
    body_a->speed = 0x100;
    body->state = (u32)hero_a_go_to_partner;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}
void hero_a_go_to_partner(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;
    struct Body *partner;
    s32 t;
    s32 x;
    s32 y;
    s32 mask;
    s16 *span;

    hero = gWorld->heroA;
    hbody = &hero->body;
    partner = body->partner;
    if (body->data->flags & 0x400)
    {
        body->speed = 0xCC;
        body->sprite->frame = 0xC;
    }
    if (body->data->flags & 0x800)
    {
        body->speed = 0x199;
        body->sprite->frame = 0x19;
    }
    set_slot_value(0x3F, -1);
    if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
    {
        t = partner->targetX;
        if (t < 0)
            t += 0xff;
        x = (t >> 8) + partner->ofsX - 0x20;
        y = partner->targetY;
        if (y < 0)
            y += 0xff;
        y >>= 8;
        y += partner->ofsY;
        place_targets(hbody, x, y, 0, -1);
        set_motion_start(hbody, 0x300);
    }
    if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
    {
        if (hbody->data->pose == -1)
        {
            sprite_set_action(hbody, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(hbody, 0, 0x204F, 0);
        }
        ((struct SpriteMirrorByte *)&hbody->sprite->unk11)->mirror = (span = &hbody->spanA)[0] >= span[1];
        ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_a_begin_step;
}