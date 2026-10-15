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

extern struct World *gUnk_03000FD8;

void sprite_set_action(struct Body *body, u32 action, s32 arg, u32 extra);
void hero_b_advance_and_place(struct Body *body);
void set_slot_value(u32 id, s32 value);
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
void hero_b_begin_step_rate2(struct Body *body);
void hero_b_advance_anim4(struct Body *body);
void hero_b_move_to_partner_target(struct Body *body);
void hero_b_advance_and_place(struct Body *body);
void hero_b_step_end_fx(struct Body *body);
void hero_b_link_when_ready(struct Body *body);
void hero_b_begin_step_rate6(struct Body *body);
void hero_b_place_at_data_target(struct Body *body);
void hero_pair_meet(struct Body *body);

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

    hbody = &gUnk_03000FD8->heroA->body;
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

    hero = gUnk_03000FD8->heroA;
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

    hero = gUnk_03000FD8->heroA;
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

ASM_FUNC("asm/code_080A1DC8/hero_pair_step_and_place.s", void hero_pair_step_and_place(struct Body *body))

void hero_a_end_anim_clear(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;

    hero = gUnk_03000FD8->heroA;
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

ASM_FUNC("asm/code_080A1DC8/hero_pair_setup_pose.s", void hero_pair_setup_pose(void))
ASM_FUNC("asm/code_080A1DC8/hero_b_place_with_effect.s", void hero_b_place_with_effect(void))
void hero_b_begin_step_rate2(struct Body *body)
{
    struct Hero *hero;

    hero = gUnk_03000FD8->heroB;
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

    hero = gUnk_03000FD8->heroB;
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

ASM_FUNC("asm/code_080A1DC8/hero_b_move_to_partner_target.s", void hero_b_move_to_partner_target(struct Body *body))
ASM_FUNC("asm/code_080A1DC8/hero_b_move_to_partner_step.s", void hero_b_move_to_partner_step(void))

void hero_b_arm_full_step(struct Body *body)
{
    struct Hero *hero;

    hero = gUnk_03000FD8->heroB;
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

    hero = gUnk_03000FD8->heroB;
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

ASM_FUNC("asm/code_080A1DC8/hero_b_step_end_fx.s", void hero_b_step_end_fx(struct Body *body))
ASM_FUNC("asm/code_080A1DC8/hero_pair_place_both_finish_t399.s", void hero_pair_place_both_finish_t399(void))

void hero_b_link_when_ready(struct Body *body)
{
    struct Body *hbody;
    struct Body *partner;
    s32 linked;

    hbody = &gUnk_03000FD8->heroB->body;
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

    hero = gUnk_03000FD8->heroB;
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

    hero = gUnk_03000FD8->heroB;
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

ASM_FUNC("asm/code_080A1DC8/hero_pair_meet.s", void hero_pair_meet(struct Body *body))

void hero_b_release(struct Body *body)
{
    struct Hero *hero;
    struct Body *hbody;

    hero = gUnk_03000FD8->heroB;
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

ASM_FUNC("asm/code_080A1DC8/hero_pair_start.s", void hero_pair_start(void))
ASM_FUNC("asm/code_080A1DC8/hero_a_go_to_partner.s", void hero_a_go_to_partner(void))