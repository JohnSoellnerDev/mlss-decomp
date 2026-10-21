#include "global.h"
#include "ram_routines.h"

struct Flags8
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

struct Mode2
{
    u8 low : 2;
    u8 mode : 2;
    u8 rest : 4;
};

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    s8 unk20;
    u8 unk21;
    u8 filler22[0x23 - 0x22];
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    s16 unk2A;
};

struct Record
{
    u8 filler0[0x14];
    u16 unk14[3];
    u16 unk1A;
    u8 filler1C[0x2E - 0x1C];
    u16 unk2E;
    u16 unk30;
    u16 unk32;
    u16 unk34;
    u16 speeds[4];
    u8 unk3E;
};

struct Partner
{
    u8 filler0[4];
    s16 unk4;
    u8 filler6[0xC - 6];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x200 - 0x38];
    struct Partner *unk200[2];
    u8 unk208;
};

struct Enemy
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x34 - 0x25];
    s32 unk34;
    s32 unk38;
    u8 filler3C[0x52 - 0x3C];
    u16 unk52;
    u8 filler54[0x100 - 0x54];
    u16 wanderDist[4];
    u8 filler108[0x130 - 0x108];
    u16 wanderSteps[2];
    u8 filler134[0x198 - 0x134];
    void *unk198;
    void *unk19C;
    void *unk1A0[2];
    u8 filler1A8[0x1B0 - 0x1A8];
    struct Record *rec;
    s16 *sensor;
    u8 filler1B8[0x1BA - 0x1B8];
    u16 unk1BA;
    u8 filler1BC[0x1D0 - 0x1BC];
    u16 index;
    u16 unk1D2;
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x1D8 - 0x1D7];
    u16 unk1D8;
    u8 filler1DA[0x1DC - 0x1DA];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x20C - 0x1EC];
    u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x214 - 0x211];
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x23B - 0x217];
    u8 unk23B;
    u8 filler23C[0x23E - 0x23C];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x276 - 0x246];
    u16 unk276;
    u8 filler278[0x27A - 0x278];
    u16 unk27A;
    u8 filler27C[0x283 - 0x27C];
    u8 unk283;
    u8 filler284[0x294 - 0x284];
    struct Sprite *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 unk2B6;
    u8 filler2B7[0x2C4 - 0x2B7];
    s32 unk2C4;
    u8 filler2C8[0x2D4 - 0x2C8];
    s32 unk2D4;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 zspeed;
    u16 runLeft;
    u8 filler2E0[0x2E4 - 0x2E0];
    s32 unk2E4;
    s32 unk2E8;
    s32 unk2EC;
    u32 unk2F0;
    u32 unk2F4;
    s16 unk2F8;
    u8 filler2FA[0x30A - 0x2FA];
    u16 unk30A;
    u16 unk30C;
    s16 unk30E;
    s16 unk310;
    u8 filler312[0x328 - 0x312];
    s32 unk328;
    s32 unk32C;
    u8 filler330[0x338 - 0x330];
    void *unk338;
    u8 filler33C[0x33D - 0x33C];
    u8 unk33D;
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *body;
};

void obj_set_active(struct Enemy *ent, u32 zero);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void enemy_move_checked(void);
void enemy_check_axis(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void load_box_oriented(s16 *box, struct Enemy *ent, u32 a, u32 b);
u32 boxes_overlap_oriented(s16 *box, struct Enemy *ent, void *entry, s32 zAbove, s32 zBelow);
void reset_record_fields(struct Enemy *ent);
void run_state_handler(void);
void enemy_collide_settle(void);
void enemy_start_bump(void);
void enemy_hop_3d(void);
void tick_store(void);
u8 step_pos_xyz(struct Enemy *ent);
u8 z_approach(struct Enemy *ent);
void enemy_deactivate_body(struct Enemy *body);
void enemy_zone_pick(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
s8 zone_point_code(struct Owner *owner, struct Enemy *ent, u32 zero);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
void enemy_jump_partner(void);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void enemy_follow_or_hop(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void behavior_fall_step(void);
void enemy_update_landing(void);
void enemy_move_axis(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void enemy_hop_to_partner(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void enemy_fall_landing_step(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void start_move_to_3d(struct Enemy *obj, s32 x, s32 y, s32 z, u16 speed, u8 flag);
extern u8 *gMathRoutines;
void wander_to_edge(void *obj, u32 a, s32 b);
void start_move_2d(struct Enemy *obj, s32 x, s32 y, u16 frames, u8 flag);
void walk_step_z_plain(void);
void walk_step_z(void);
void advance_and_step_z(void);
void advance_and_step_z_alt(void);
void start_axis_move(struct Enemy *ent, s32 a, s32 b, s32 c);
void obj_set_cell_coord(void *ent, u32 cell, u32 arg);
void collide_react(struct Owner *owner, struct Enemy *ent, void *bro, u32 kick, u32 speed, s32 offX, s32 offY, u32 a, u32 b);
void set_box_flag_1d8(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void enemy_aim_at_entry(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void enemy_seek_partner_axis(struct Owner *owner, struct Enemy *ent, u32 *stateOut);
void start_axis_wander_alt(void);
void sprite_step_e6a0(void *spr);
extern const u8 gFrameAttrTable[];
void obj_set_or_add_pos(void *obj, s32 a, s32 b, s32 c, u32 d, u32 e);
void start_handler_acfd(void);
void fall_landing_active(void);
void fall_landing(void);
u8 point_in_box(s16 *bounds, struct Partner *obj, s32 zMaxIn, s32 zMinIn);

ASM_FUNC("asm/code_08149500/bump_attack_enemy_move_3d_step.s", void bump_attack_enemy_move_3d_step(void))

void enemy_place_body_quadrant(struct Enemy *ent)
{
    struct Enemy *body;
    u8 v;

    body = ent->body;
    v = gFrameAttrTable[((((u32)body->unk216 << 28) >> 30) << 4) + (((u32)body->unk24 << 29) >> 29) + 8];
    switch (v)
    {
    case 0:
        obj_set_or_add_pos(ent, body->posX, body->posY - ent->unk276, body->posZ + body->unk18 + 0x900, 1, 0);
        break;
    case 1:
        obj_set_or_add_pos(ent, body->posX + 0xA00, body->posY + 0x500, body->posZ + body->unk18 + 0xF00, 1, 0);
        break;
    case 2:
        obj_set_or_add_pos(ent, body->posX, body->posY + 0x300, body->posZ + body->unk18 + 0x900, 1, 0);
        break;
    case 3:
        obj_set_or_add_pos(ent, body->posX - 0xA00, body->posY + 0x500, body->posZ + body->unk18 + 0xF00, 1, 0);
        break;
    }
}

u32 enemy_touches_companion_oriented(struct Owner *owner, struct Enemy *ent)
{
    s16 box[8];
    u8 idx;
    struct Partner *entry;
    u32 hit;

    if (find_active_entry(owner, &idx) == 0)
    {
        return 0;
    }
    entry = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
    load_box_oriented(box, ent, 0x11, 0x1C);
    hit = boxes_overlap_oriented(box, ent, entry, box[4], box[5]);
    if ((u8)hit)
    {
        hit = 1;
    }
    return hit;
}

void enemy_go_dormant_b(struct Owner *owner, struct Enemy *ent)
{
    struct Enemy *body;

    {
        u8 *p = &ent->unk33E;
        *p = *p | 0x10;
    }
    body = ent->body;
    ((struct Flags8 *)&body->unk20D)->bit2 = 0;
    reset_record_fields(ent);
    enemy_deactivate_body(ent->body);
    ent->unk1D4 = 0;
    {
        u8 *p = &ent->unk33D;
        *p = *p | 8;
    }
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_collide_settle;
}

u8 enemy_companion_hit(struct Owner *owner, struct Enemy *ent)
{
    s16 box[8];
    u8 idx;
    struct Partner *entry;

    if (find_active_entry(owner, &idx) == 0)
    {
        return 0;
    }
    entry = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
    load_box_oriented(box, ent, 0x11, 0x1C);
    if ((u8)boxes_overlap_oriented(box, ent, entry, box[4], box[5]))
    {
        return 1;
    }
    return 0;
}

void enemy_z_fall_step(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    struct Record *rec;
    u8 done;

    rec = ent->rec;
    done = step_pos_xyz(ent);
    if (done == 0)
    {
        ent->unk2EC = ent->posZ + ent->unk18;
        ent->unk2F4 = 0;
        ent->unk1D2 = *(u8 *)(ent->index + (u32)rec + 0x52);
        stateOut[0] = (u32)tick_store;
        stateOut[3] = (u32)enemy_start_bump;
    }
}

void enemy_fall_pos(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    u8 done;

    ent->posX = ent->posX + ent->vx;
    ent->posY = ent->posY + ent->vy;
    done = z_approach(ent);
    if (done == 0)
    {
        ent->unk2EC = ent->posZ + ent->unk18;
        ent->unk2F4 = 0;
        stateOut[0] = (u32)enemy_hop_3d;
    }
}

void bump_attack_enemy_reset_idle(struct Owner *owner, struct Enemy *ent)
{
    {
        u8 *p = &ent->unk33E;
        *p = *p | 0x10;
    }
    reset_record_fields(ent);
    ent->unk2EC = ent->posZ + ent->unk18;
    ent->unk2F4 = 0;
    {
        u16 v = 0x10;
        ent->unk2F8 = v;
    }
    ent->unk1D4 = 0;
    ent->unk198 = enemy_update_landing;
    ent->unk1A0[0] = enemy_start_bump;
}

void clear_obj_flags(struct Enemy *obj)
{
    ((struct Flags8 *)&obj->unk20D)->bit3 = 0;
    ((struct Flags8 *)&obj->unk20C)->bit6 = 0;
    ((struct Flags8 *)&obj->unk20C)->bit7 = 0;
    ((struct Flags8 *)&obj->unk20C)->bit5 = 0;
    ((struct Flags8 *)&obj->unk20F)->bit2 = 0;
}

struct Mode2At1
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

void enemy_fall_end_state(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ((struct Mode2At1 *)&ent->sprite->unk12)->mode = 0;
    ent->zspeed = ent->rec->speeds[0];
    *stateOut = (u32)enemy_check_axis;
}

void enemy_fall_decay(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    struct Enemy *body;

    ent->unk18 = ent->unk18 - ent->zspeed;
    body = ent->body;
    body->unk1BA = body->unk52;
    ent->zspeed += ent->unk1D2;
    if (ent->unk18 <= 0)
    {
        ent->unk18 = 0;
        *stateOut = (u32)enemy_fall_end_state;
    }
}

void enemy_timer_expire(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    struct Enemy *body;
    s32 timer;

    body = ent->body;
    body->unk23B = 0xF0;
    timer = body->unk1D4 + 1;
    body->unk1D4 = timer;
    if ((u16)timer > 0x3B)
    {
        obj_set_active(ent->body, 0);
        obj_set_sprite_link(ent->body, 0);
        *stateOut = (u32)enemy_move_checked;
    }
}

void enemy_zone_pick(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    s8 zone;
    u8 idx;

    zone = zone_point_code(owner, ent, 0);
    if (zone != -1)
    {
        idx = rand_below(4, mt19937_next());
        ent->unk1D2 = *(u8 *)(idx + (u32)ent->rec + 0x2A);
        ent->index = idx;
        ((struct Mode3 *)&ent->unk214)->mode = 0;
        *stateOut = (u32)enemy_jump_partner;
    }
}

void enemy_deactivate_body(struct Enemy *body)
{
    obj_set_active(body, 0);
    obj_set_sprite_link(body, 0);
    ((struct Flags8 *)&body->unk20D)->bit5 = 0;
    ((struct Flags8 *)&body->unk20D)->bit6 = 0;
    ((struct Flags8 *)&body->unk20D)->bit3 = 1;
    {
        u8 *p = &body->unk20E;
        *p = *p | 4;
    }
    *(u32 *)((u8 *)body + 0x20C) = (*(u32 *)((u8 *)body + 0x20C) & 0xFFFC7FFF) | 0x8000;
    {
        u8 *p = &body->unk20F;
        *p = *p | 4;
    }
    obj_load_record(body->sprite, 0xF, -1, 0, 0);
}

void enemy_check_axis(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    s32 v;

    ent->unk18 = ent->unk18 + ent->zspeed;
    v = *(s16 *)&ent->unk52;
    if (v < -ent->unk283)
    {
        ((struct Flags8 *)&ent->unk20C)->bit7 = 0;
        ((struct Flags8 *)&ent->unk20C)->bit6 = 0;
        ((struct Flags8 *)&ent->unk20C)->bit5 = 0;
        obj_set_sprite_link(ent, 0);
        ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
        *stateOut = (u32)enemy_zone_pick;
    }
}

void enemy_move_axis(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    u8 *ai;
    u8 *tbl;
    s16 *sensor;
    u32 value;
    u32 part;
    u32 sensorVal;
    u8 idx;
    u8 pick;
    struct Partner *target;

    ai = (u8 *)ent + 0x58;
    tbl = (u8 *)ent + 0x100;
    sensor = ent->sensor;
    ent->index = rand_below(4, mt19937_next());
    value = *(u16 *)(tbl + ent->index * 2);
    part = *(u16 *)(tbl + rand_below(2, mt19937_next()) * 2 + 0x30);
    sensorVal = (u16)sensor[5];
    if (find_active_entry(owner, &idx) != 0)
    {
        pick = idx;
    }
    else
    {
        pick = (((u32)owner->unk208) << 30) >> 31;
    }
    target = *(struct Partner **)((u8 *)owner + pick * 4 + 0x28);
    collide_react(owner, ent, target, part, value, (s8)tbl[0x40], (s8)tbl[0x41], 0x22, 0);
    start_axis_move(ent, ent->posZ + (sensorVal << 8), ent->runLeft, 0);
    obj_set_cell_coord(ent, 0, 2);
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ((struct Mode3 *)&ent->unk24)->mode = 0;
    ent->sprite->unk20 = *(u16 *)(tbl + *(u16 *)(ai + 0x178) * 2 + 8);
    ((struct Flags8 *)(ai + 0x17E))->bit0 = 0;
    *stateOut = (u32)enemy_fall_landing_step;
}

void enemy_fall_landing_step(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    u8 *tbl;
    s16 *sensor;
    u8 done;

    tbl = (u8 *)ent + 0x100;
    sensor = ent->sensor;
    if (((struct Flags8 *)&ent->unk1D6)->bit0 == 0)
    {
        if (ent->sprite->unk23 > 1)
        {
            ((struct Mode3 *)&ent->unk214)->mode = 0;
        }
        done = step_pos_xyz(ent);
        if (done == 0)
        {
            ent->unk2D4 = (u16)sensor[4] << 8;
            ent->zspeed = *(u16 *)(tbl + ent->index * 2 + 0x28);
            ((struct Mode3 *)&ent->unk214)->mode = 1;
            ((struct Mode3 *)&ent->unk24)->mode = 0;
            ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        }
    }
    else
    {
        if (((struct Flags8 *)&ent->sprite->unk12)->bit3)
        {
            ((struct Mode3 *)&ent->unk214)->mode = 0;
        }
        ent->unk18 = ent->unk18 - ent->zspeed;
        if (ent->unk18 <= ent->unk2D4)
        {
            ent->unk18 = ent->unk2D4;
            if (rand_below(0x64, mt19937_next()) & 1)
            {
                *stateOut = (u32)enemy_move_axis;
            }
            else
            {
                *stateOut = (u32)enemy_hop_to_partner;
            }
        }
    }
}

ASM_FUNC("asm/code_08149500/enemy_hop_to_partner.s", void enemy_hop_to_partner(struct Owner *owner, struct Enemy *ent, u32 *stateOut))

void enemy_state_step_xyz(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    struct Record *rec;
    s16 *sensor;
    u8 done;

    rec = ent->rec;
    sensor = ent->sensor;
    if (((struct Flags8 *)&ent->unk1D6)->bit0 == 0)
    {
        if (ent->sprite->unk23 > 1)
        {
            ((struct Mode3 *)&ent->unk214)->mode = 0;
        }
        done = step_pos_xyz(ent);
        if (done == 0)
        {
            ent->unk2D4 = (u16)sensor[4] << 8;
            ent->zspeed = *(u16 *)((u8 *)rec + ent->index * 2 + 0x32);
            ((struct Mode3 *)&ent->unk214)->mode = 1;
            ((struct Mode3 *)&ent->unk24)->mode = 0;
            ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        }
    }
    else
    {
        if (((struct Flags8 *)&ent->sprite->unk12)->bit3)
        {
            ((struct Mode3 *)&ent->unk214)->mode = 0;
        }
        ent->unk18 = ent->unk18 - ent->zspeed;
        if (ent->unk18 <= ent->unk2D4)
        {
            ent->unk18 = ent->unk2D4;
            *stateOut = (u32)enemy_move_axis;
        }
    }
}

NONMATCH("asm/code_08149500/enemy_update_draw_flags.s", void enemy_update_draw_flags(struct Owner *owner, struct Enemy *ent))
{
    u8 *ai;
    struct Sprite *spr;
    u8 *flags;
    u8 idx;

    ai = (u8 *)ent + 0x58;
    spr = ent->sprite;
    flags = (u8 *)&ent->unk1D8;
    spr->unk20 = (*flags << 30) >> 31 ? 0x40 : 0x20;
    set_box_flag_1d8(owner, ent, (u32 *)ent->unk1A0);
    if ((((*flags << 29) >> 31) == 0 && ((*flags << 30) >> 31) == 0 && owner->party[0]->unk4 == 4) || owner->party[1]->unk4 == 4)
    {
        if (find_active_entry(owner, &idx) != 0)
        {
            ((struct Flags8 *)(ai + 0x180))->bit1 = 1;
        }
    }
    if (owner->unk200[0]->unk18 == 0 && owner->unk200[1]->unk18 == 0)
    {
        ((struct Flags8 *)(ai + 0x180))->bit1 = 0;
    }
    ((void (*)(struct Owner *, struct Enemy *))(*(void **)(ai + 0x148)))(owner, ent);
}
END_NONMATCH

void set_box_flag_1d8(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    s16 *sensor;
    s16 box[8];
    u8 idx;
    struct Partner *entry;

    sensor = ent->sensor;
    if (find_active_entry(owner, &idx) == 0)
    {
        ((struct Flags8 *)&ent->unk1D8)->bit0 = 0;
    }
    else
    {
        entry = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
        box[0] = *sensor * 0x10 - 0x20;
        sensor++;
        box[3] = *sensor * 0x10 - 0x20;
        sensor++;
        box[1] = *sensor * 0x10 + 0x20;
        box[2] = sensor[1] * 0x10 + 0x20;
        if (point_in_box(box, entry, 0x7FFF, 0) == 0)
        {
            ((struct Flags8 *)&ent->unk1D8)->bit0 = 0;
        }
        else
        {
            ((struct Flags8 *)&ent->unk1D8)->bit0 = 1;
        }
    }
}

NONMATCH("asm/code_08149500/enemy_start_fall.s", void enemy_start_fall(struct Owner *owner, struct Enemy *ent, u32 *stateOut))
{
    u8 *ai;
    u8 *roll;
    u8 dice[2];
    u8 threshold;
    u8 rollValue;

    ai = (u8 *)ent + 0x58;
    ent->unk2C4 = ent->posZ + ent->unk18;
    dice[0] = 3;
    roll = &dice[1];
    *roll = rand_below(0x64, mt19937_next());
    threshold = dice[0];
    rollValue = *roll;
    if ((s8)rollValue - (s8)threshold < 0)
    {
        ent->unk2D4 = 0x1800;
    }
    else
    {
        ent->unk2D4 = ent->posZ + ent->unk18 - 0x2400;
    }
    ent->zspeed = 0x100;
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ((struct Flags8 *)(ai + 0x17E))->bit0 = 0;
    ((struct Flags8 *)(ai + 0x17E))->bit1 = 1;
    *stateOut = (u32)fall_landing_active;
}
END_NONMATCH

void fall_landing_pick_1d8(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    u8 idx;
    struct Partner *entry;

    ent->unk18 = ent->unk18 - ent->zspeed;
    if ((*(u32 *)&ent->unk24 & 0x18000) != 0)
    {
        if ((((u32)*(u8 *)&ent->unk1D8) << 31) != 0 && (s32)(((u32)*(u8 *)&ent->unk1D8) << 30) < 0
            && find_active_entry(owner, &idx) != 0)
        {
            *stateOut = (u32)enemy_seek_partner_axis;
            return;
        }
        *stateOut = (u32)start_axis_wander_alt;
        return;
    }
    if (find_active_entry(owner, &idx) == 0)
    {
        ((struct Flags8 *)&ent->unk1D8)->bit0 = 0;
        return;
    }
    entry = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
    if (ent->posZ + ent->unk18 > entry->posZ + entry->unk18 + 0x2D00)
    {
        return;
    }
    if ((((u32)*(u8 *)&ent->unk1D8) << 31) != 0 && (s32)(((u32)*(u8 *)&ent->unk1D8) << 30) < 0)
    {
        *stateOut = (u32)enemy_seek_partner_axis;
    }
    else
    {
        *stateOut = (u32)start_axis_wander_alt;
    }
}

ASM_FUNC("asm/code_08149500/enemy_seek_partner_axis.s", void enemy_seek_partner_axis(struct Owner *owner, struct Enemy *ent, u32 *stateOut))

ASM_FUNC("asm/code_08149500/enemy_check_owner_box.s", void enemy_check_owner_box(void))

NONMATCH("asm/code_08149500/enemy_start_fall_alt.s", void enemy_start_fall_alt(struct Owner *owner, struct Enemy *ent, u32 *stateOut))
{
    u8 *ai;
    u8 *roll;
    u8 dice[2];
    u8 threshold;
    u8 rollValue;

    ai = (u8 *)ent + 0x58;
    ent->unk2C4 = ent->posZ + ent->unk18;
    dice[0] = 3;
    roll = &dice[1];
    *roll = rand_below(0x64, mt19937_next());
    threshold = dice[0];
    rollValue = *roll;
    if ((s8)rollValue - (s8)threshold < 0)
    {
        ent->unk2D4 = 0x1800;
    }
    else
    {
        ent->unk2D4 = ent->posZ + ent->unk18 - 0x2400;
    }
    ent->zspeed = 0x100;
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ((struct Flags8 *)(ai + 0x17E))->bit0 = 0;
    ((struct Flags8 *)(ai + 0x17E))->bit1 = 1;
    *stateOut = (u32)fall_landing;
}
END_NONMATCH

ASM_FUNC("asm/code_08149500/enemy_seek_partner_axis_alt.s", void enemy_seek_partner_axis_alt(struct Owner *owner, struct Enemy *ent, u32 *stateOut))

u8 entry_box_check(struct Owner *owner, struct Enemy *ent)
{
    s16 *sensor;
    s16 box[8];
    u8 idx;
    struct Partner *entry;
    s16 px;
    s16 py;

    sensor = ent->sensor;
    if ((((u32)*(u8 *)&ent->unk1D8) << 31) == 0)
    {
        return 0;
    }
    if (find_active_entry(owner, &idx) == 0)
    {
        return 0;
    }
    if ((s32)(((u32)*(u8 *)&ent->unk1D8) << 30) >= 0)
    {
        return 0;
    }
    entry = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
    px = ent->posX / 0x100;
    py = ent->posY / 0x100;
    box[0] = px - sensor[8] * 0x10;
    box[3] = py - sensor[9] * 0x10;
    box[1] = px + sensor[10] * 0x10;
    box[2] = py + sensor[11] * 0x10;
    return (u8)point_in_box(box, entry, 0x7FFF, 0) == 1;
}

void enemy_pick_entry_sprite(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    struct Record *rec;

    rec = ent->rec;
    ent->index = rand_below(4, mt19937_next());
    obj_set_sprite_link(ent, 1);
    ((struct Mode2At1 *)&ent->sprite->unk12)->mode = 1;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    obj_load_record(ent->sprite, 1, -1, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ent->sprite->unk20 = *((u8 *)rec + ent->index);
    ((struct Flags8 *)&ent->unk20C)->bit7 = 0;
    ((struct Flags8 *)&ent->unk20C)->bit6 = 0;
    ((struct Flags8 *)&ent->unk20C)->bit5 = 0;
    ((struct Flags8 *)&ent->unk1D6)->bit4 = 1;
    stateOut[0] = (u32)enemy_follow_or_hop;
    stateOut[1] = (u32)start_handler_acfd;
}

ASM_FUNC("asm/code_08149500/enemy_follow_or_hop.s", void enemy_follow_or_hop(struct Owner *owner, struct Enemy *ent, u32 *stateOut))

void enemy_aim_at_entry(struct Owner *owner, struct Enemy *ent, u32 *stateOut)
{
    u8 *ai;
    struct Record *rec;
    s16 *sensor;
    u8 idx;
    u8 pick;
    u32 shifted;
    struct Partner *entry;

    ai = (u8 *)ent + 0x58;
    rec = ent->rec;
    sensor = ent->sensor;
    if (find_active_entry(owner, &idx) != 0)
    {
        pick = idx;
    }
    else
    {
        pick = (((u32)owner->unk208) << 30) >> 31;
    }
    entry = *(struct Partner **)((u8 *)owner + pick * 4 + 0x28);
    ent->unk2C4 = ent->posZ + ent->unk18;
    shifted = (u16)sensor[4];
    ent->unk2D4 = entry->posZ + (shifted << 8);
    ent->zspeed = *(u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2 + 0x14);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    *stateOut = (u32)behavior_fall_step;
}

ASM_FUNC("asm/code_08149500/enemy_tile_move.s", void enemy_tile_move(void))
