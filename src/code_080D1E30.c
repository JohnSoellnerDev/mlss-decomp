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
    u8 filler0EE[0xF6 - 0xEE];
    s16 displayTimer;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
    u8 filler112[0x114 - 0x112];
    u32 poseVal;
    u8 filler118[0x11E - 0x118];
    u16 flags;
};

struct DataConnectBits
{
    u8 lo1 : 1;
    u8 connect : 1;
    u8 hi : 6;
};

struct CcBits
{
    u8 lo7 : 7;
    u8 top : 1;
};

struct SpriteEntry
{
    u8 filler0[6];
    u16 scale;
    u8 filler8[0x11 - 0x8];
    u8 unk11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 anim;
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

void obj_release(struct Body *obj);
void sprite_set_action(struct Body *body, u32 action, s32 arg, u32 extra);
void state_stack_advance_progress(struct Body *body);
void step_motion(struct Body *body);
void set_slot_value(u32 id, s32 value);
void hero_body_blend_to_a(struct Body *body);
void step_motion_scaled(struct Body *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct Body *owner);
void place_targets(struct Body *body, s32 x, s32 y, s32 z, s32 flag);
void set_motion_offsets(struct Body *body, u32 a);
void hero_pair_place_both_rebuild(void);
void body_place_at_midpoint_arc(struct Body *body);
void body_fx_then_place_on_partner(struct Body *body);
void hero_a_place_own_target_anim10(struct Body *body);
void hero_a_step_scaled_fx(struct Body *body);
void hero_a_step_scaled_fx_pose(struct Body *body);
void hero_pair_place_both_rebuild_alt(void);
void enter_finish_rebuild(struct Body *body);
void set_motion_start(struct Body *body, u32 speed);
void set_motion_speed_aux(struct Body *obj, u32 value, s32 aux);
s32 find_frame_entry(struct Body *obj, s32 index);
void hero_a_place_at_child_target(struct Body *body);
void hero_a_step_window_20e8(void);
void body_link_partner_fx_slot55(struct Body *body);
void body_link_partner_fx_slot9d(struct Body *body);
void body_finish_object(struct Body *body);
void hero_a_step_window_20e8(void);
s32 resolve_contact_action(struct Body *a, struct Body *b, u32 c, u32 d, u32 e);

void hero_a_place_at_b_target(struct Body *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct Body *body_a = &heroA->body;
    struct Body *body_b = &gWorld->heroB->body;
    s32 arc;

    set_slot_value(0x2B, -1);
    if ((heroA->body.mode & 6) == 2 || (heroA->body.mode & 6) == 4)
    {
        if (body_a->data->pose == -1)
        {
            sprite_set_action(body_a, 5, 0x2000, 0);
        }
        else
        {
            sprite_set_action(body_a, 5, 0x204D, 0);
        }
        ((struct SpriteModeByte *)&body_a->sprite->flags)->mode = 1;
    }
    ((struct SpriteMirrorByte *)&body_a->sprite->unk11)->mirror = 1;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        place_targets(body_a, body_b->targetX / 0x100 + body_b->ofsX, body_b->targetY / 0x100 + body_b->ofsY,
                      body_b->targetZ / 0x100 + body_b->ofsZ - 0x14, -1);
        if (body_b == (struct Body *)((u8 *)body_b->data + 8))
        {
            arc = body_b->ofsZ;
        }
        else
        {
            find_frame_entry(body_b, 0);
            arc = body_b->unk47;
        }
        arc += body_b->targetZ / 0x100;
        arc += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(4, arc * arc / 0x40 + 2);
        body_a->arc = arc;
        set_motion_offsets(body_a, 0x500);
    }
    body->state = (u32)hero_a_place_at_child_target;
}

void hero_a_place_at_child_target(struct Body *body)
{
    struct Hero *heroB;
    struct Body *body_a;
    struct Body *body_b;
    s32 k;
    s32 za;
    s32 x;
    s32 y;
    s32 z;
    s32 tz;
    s32 t;

    body_a = &gWorld->heroA->body;
    heroB = gWorld->heroB;
    body_b = &heroB->body;
    step_motion(body_a);
    k = (s8)heroB->body.ofsZ - 4;
    za = body_a->z;
    if (za < body_a->targetZ)
    {
        if (za < body_b->z + (k << 8))
        {
            body_b->sprite->scale = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(za, k);
        }
    }
    if (body_a->flags & 0x20)
    {
        set_slot_value(0x2B, -1);
        ((struct SpriteMirrorByte *)&body_a->sprite->unk11)->mirror = 0;
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            t = body_a->child->targetX;
            if (t < 0)
                t += 0xff;
            x = (t >> 8) + body_a->child->ofsX - 0x14;
            t = body_a->child->targetY;
            if (t < 0)
                t += 0xff;
            y = (t >> 8) + body_a->child->ofsY;
            tz = body_a->child->targetZ;
            if (tz < 0)
                tz += 0xff;
            z = tz >> 8;
            place_targets(body_a, x, y, z, -1);
            t = body_a->child->targetZ;
            if (t < 0)
                t += 0xff;
            body_a->arc = (t >> 8) + 2;
            set_motion_offsets(body_a, 0x500);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 7, 0x2036, 0);
            ((struct SpriteModeByte *)&body_a->sprite->flags)->mode = 1;
        }
        body_a->timer &= 0xFFF;
        body->state = (u32)hero_a_step_window_20e8;
    }
}

ASM_FUNC("asm/code_080D1E30/hero_a_step_window_20e8.s", void hero_a_step_window_20e8(void))

void hero_a_place_own_target_anim11(struct Body *body)
{
    struct Body *hbody;
    s32 px;
    s32 py;

    hbody = &gWorld->heroA->body;
    if (hbody->sprite->flags & 8)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            px = hbody->targetX;
            if (px < 0)
                px += 0xff;
            px = px >> 8;
            px = px + hbody->ofsX;
            py = hbody->targetY;
            if (py < 0)
                py += 0xff;
            py = py >> 8;
            py = py + hbody->ofsY;
            place_targets(hbody, px, py, 0, -1);
            set_motion_speed_aux(hbody, 0, 0x66);
        }
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            sprite_set_action(hbody, 0xB, 0x2036, 0);
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        body->state = (u32)hero_a_step_scaled_fx;
    }
}

void hero_a_step_scaled_fx(struct Body *body)
{
    struct Body *hbody;
    s32 px;
    s32 py;
    s32 pz;

    hbody = &gWorld->heroA->body;
    step_motion_scaled(hbody);
    if (hbody->flags & 0x20)
    {
        set_slot_value(0x2E, -1);
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
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            sprite_set_action(hbody, 0xC, 0x2036, 0);
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
        }
        body->state = (u32)hero_pair_place_both_rebuild;
    }
}

void hero_pair_place_both_rebuild(void)
{
    struct Body *body_a;
    struct Body *body_b;
    struct BodyData *data;
    s16 *span;
    s32 t;
    s32 x;
    s32 y;
    s32 z;

    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    if (body_a->sprite->flags & 8)
    {
        set_slot_value(0x53, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            y = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            z = t >> 8;
            place_targets(body_b, x, y, z, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
            {
                sprite_set_action(body_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_b, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&body_b->sprite->unk11)->mirror = (span = &body_b->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            y = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            z = t >> 8;
            place_targets(body_a, x, y, z, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
            {
                sprite_set_action(body_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_a, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&body_a->sprite->unk11)->mirror = (span = &body_a->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

void body_place_at_midpoint(struct Body *body)
{
    s32 mx;
    s32 my;
    s32 t;
    s32 sy;
    s32 tz;
    struct Body *partner;

    partner = body->partner;
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        t = partner->targetX + body->targetX;
        if (t < 0)
            t += 0xff;
        mx = ((t >> 8) + partner->ofsX) / 2;
        sy = partner->targetY + body->targetY;
        if (sy < 0)
            sy += 0xff;
        my = ((sy >> 8) + partner->ofsY) / 2;
        place_targets(body, mx, my, 0, -1);
        tz = body->targetZ;
        if (tz < 0)
            tz += 0xff;
        body->arc = (tz >> 8) + 2;
        set_motion_offsets(body, 0x300);
    }
    body->state = (u32)body_place_at_midpoint_arc;
}

void body_place_at_midpoint_arc(struct Body *body)
{
    s32 height;
    struct Body *partner;
    s32 t;
    s32 sy;
    s32 pz;
    s32 tz;
    s32 ofsZ;
    s32 mx;
    s32 my;
    s32 arc;

    partner = body->partner;
    step_motion(body);
    if (body->flags & 0x20)
    {
        set_slot_value(0x9C, -1);
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            t = partner->targetX + body->targetX;
            if (t < 0)
                t += 0xff;
            mx = ((t >> 8) + partner->ofsX) / 2;
            sy = partner->targetY + body->targetY;
            if (sy < 0)
                sy += 0xff;
            my = ((sy >> 8) + partner->ofsY) / 2;
            place_targets(body, mx, my, 0, -1);
            pz = partner->z;
            if (pz < 0)
                pz += 0xff;
            pz >>= 8;
            ofsZ = partner->ofsZ;
            arc = (0x50 - (pz + ofsZ)) / 2;
            if (partner == (struct Body *)((u8 *)partner->data + 8))
            {
                height = ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                height = partner->unk47;
            }
            tz = partner->targetZ;
            if (tz < 0)
                tz += 0xff;
            height += tz >> 8;
            height += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(arc * arc, arc + height * height / 0x40);
            body->arc = height;
            set_motion_offsets(body, 0x500);
        }
        body->state = (u32)body_link_partner_fx_slot55;
    }
}

void body_link_partner_fx_slot55(struct Body *body)
{
    struct Body *partner;
    s32 y;
    s32 x;

    partner = body->partner;
    step_motion(body);
    if (body->flags & 0x20)
    {
        ((struct DataConnectBits *)&partner->data->flags111)->connect = 0;
        set_slot_value(0x55, -1);
        ((struct CcBits *)&body->cc)->lo7 = 0x19;
        body->scale = 0x100;
        resolve_contact_action(body, partner, 0, 0, 0x0423448C);
        body->count++;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2))
        {
            switch (body->sprite->anim - 3)
            {
            case 0:
            case 2:
            case 4:
            case 6:
            case 8:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0x7D, x, y, 0, partner);
                break;
            default:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0, x, y, 0, partner);
                break;
            }
        }
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            body->state = (u32)body_finish_object;
        }
    }
}

void hero_a_child_next_state_anim(struct Body *body)
{
    struct Body *hbody;
    struct Body *child;

    hbody = &gWorld->heroA->body;
    set_slot_value(0x85, -1);
    child = hbody->child;
    if ((child->mode & 6) == 2 || (child->mode & 6) == 4)
    {
        child->state = (u32)body_fx_then_place_on_partner;
    }
    if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
    {
        sprite_set_action(hbody, 9, 0x2036, 0);
        ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_a_place_own_target_anim10;
}

void hero_a_place_own_target_anim10(struct Body *body)
{
    struct Body *hbody;
    s32 px;
    s32 py;

    hbody = &gWorld->heroA->body;
    if (hbody->sprite->flags & 8)
    {
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            px = hbody->targetX;
            if (px < 0)
                px += 0xff;
            px = px >> 8;
            px = px + hbody->ofsX;
            py = hbody->targetY;
            if (py < 0)
                py += 0xff;
            py = py >> 8;
            py = py + hbody->ofsY;
            place_targets(hbody, px, py, 0, -1);
            set_motion_speed_aux(hbody, 0, 0x66);
        }
        if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
        {
            sprite_set_action(hbody, 0xA, 0x2036, 0);
            ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 1;
        }
        body->state = (u32)hero_a_step_scaled_fx_pose;
    }
}

void hero_a_step_scaled_fx_pose(struct Body *body)
{
    struct Body *hbody;
    s32 px;
    s32 py;
    s32 pz;

    hbody = &gWorld->heroA->body;
    step_motion_scaled(hbody);
    if (hbody->flags & 0x20)
    {
        set_slot_value(0x65, -1);
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
        body->state = (u32)hero_pair_place_both_rebuild_alt;
    }
}

void hero_pair_place_both_rebuild_alt(void)
{
    struct Body *body_a;
    struct Body *body_b;
    struct BodyData *data;
    s16 *span;
    s32 t;
    s32 x;
    s32 y;
    s32 z;

    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    if (body_a->sprite->flags & 8)
    {
        set_slot_value(0x53, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            y = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            z = t >> 8;
            place_targets(body_b, x, y, z, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
            {
                sprite_set_action(body_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_b, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&body_b->sprite->unk11)->mirror = (span = &body_b->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            y = t >> 8;
            t = data->targetZ;
            if (t < 0)
                t += 0xff;
            z = t >> 8;
            place_targets(body_a, x, y, z, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
            {
                sprite_set_action(body_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_a, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&body_a->sprite->unk11)->mirror = (span = &body_a->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

NONMATCH("asm/code_080D1E30/body_fx_then_place_on_partner.s", void body_fx_then_place_on_partner(struct Body *body))
{
    struct Body *partner;
    s32 px;
    s32 py;
    s32 pz;
    s32 t;
    s32 mx;
    s32 my;
    s32 mz;

    partner = body->partner;
    switch (body->sprite->anim - 3)
    {
    case 0:
    case 2:
    case 4:
    case 6:
    case 8:
        px = body->x;
        if (px < 0)
            px += 0xff;
        px >>= 8;
        py = body->y;
        if (py < 0)
            py += 0xff;
        py >>= 8;
        pz = body->z;
        if (pz < 0)
            pz += 0xff;
        pz >>= 8;
        effect_spawn_at(0x3002, px, py, pz, body);
        break;
    default:
        px = body->x;
        if (px < 0)
            px += 0xff;
        px >>= 8;
        py = body->y;
        if (py < 0)
            py += 0xff;
        py >>= 8;
        pz = body->z;
        if (pz < 0)
            pz += 0xff;
        pz >>= 8;
        effect_spawn_at(0, px, py, pz, body);
        break;
    }
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        t = partner->targetX;
        if (t < 0)
            t += 0xff;
        mx = (t >> 8) + partner->ofsX - 8;
        t = partner->targetY;
        if (t < 0)
            t += 0xff;
        my = (t >> 8) + partner->ofsY;
        t = partner->targetZ;
        if (t < 0)
            t += 0xff;
        mz = (t >> 8) + partner->ofsZ - 8;
        place_targets(body, mx, my, mz, -1);
        set_motion_speed_aux(body, 0x500, 0x33);
    }
    body->state = (u32)body_link_partner_fx_slot9d;
}
END_NONMATCH

void body_link_partner_fx_slot9d(struct Body *body)
{
    struct Body *partner;
    s32 y;
    s32 x;

    partner = body->partner;
    step_motion_scaled(body);
    if (body->flags & 0x20)
    {
        ((struct DataConnectBits *)&partner->data->flags111)->connect = 0;
        set_slot_value(0x9D, -1);
        ((struct CcBits *)&body->cc)->lo7 = 0x5F;
        body->scale = 0x100;
        resolve_contact_action(body, partner, 0, 0, 0x0423458C);
        body->count++;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2))
        {
            switch (body->sprite->anim - 3)
            {
            case 0:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0x2EAC, x, y, 0, partner);
                break;
            case 2:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0x2EC5, x, y, 0, partner);
                break;
            case 4:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0x2EE3, x, y, 0, partner);
                break;
            case 6:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0x2F01, x, y, 0, partner);
                break;
            case 8:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0x2F1F, x, y, 0, partner);
                break;
            default:
                x = partner->targetX;
                if (x < 0)
                    x += 0xff;
                x >>= 8;
                x += partner->ofsX;
                y = partner->targetY;
                if (y < 0)
                    y += 0xff;
                y >>= 8;
                y += partner->ofsY;
                effect_spawn_at(0, x, y, 0, partner);
                break;
            }
        }
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            body->state = (u32)body_finish_object;
        }
    }
}

void body_finish_object(struct Body *body)
{
    obj_release(body);
}

void hero_a_child_remove(void)
{
    obj_release(gWorld->heroA->body.child);
}

void body_advance_end_if_active(struct Body *body)
{
    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        body->state = 0;
    }
}

void body_anim_next_then_end(struct Body *body)
{
    struct SpriteEntry *sprite;

    sprite = body->sprite;
    if (sprite->flags & 8)
    {
        sprite_set_action(body, sprite->anim + 1, 0, 0);
        ((struct SpriteModeByte *)&body->sprite->flags)->mode = 0;
        body->state = 0;
    }
}

void body_step_if_near_hero_a(struct Body *body)
{
    struct Body *hbody;
    s32 ax;

    hbody = &gWorld->heroA->body;
    ax = hbody->x;
    if (body->x < ax - 0x800
        || (hbody->z + (hbody->ofsZ << 8) < body->z && ax + 0x800 < body->x))
    {
        step_motion(body);
        return;
    }
    set_slot_value(0x9C, -1);
    body->state = (u32)hero_body_blend_to_a;
}
