#include "global.h"

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
    u8 filler0[0x1A];
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
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x208 - 0x38];
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
    u8 filler25[0x52 - 0x25];
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
    u8 filler246[0x27A - 0x246];
    u16 unk27A;
    u8 filler27C[0x283 - 0x27C];
    u8 unk283;
    u8 filler284[0x294 - 0x284];
    struct Sprite *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 unk2B6;
    u8 filler2B7[0x2DC - 0x2B7];
    s16 zspeed;
    u16 runLeft;
    u8 filler2E0[0x2EC - 0x2E0];
    s32 unk2EC;
    u8 filler2F0[0x2F4 - 0x2F0];
    u32 unk2F4;
    s16 unk2F8;
    u8 filler2FA[0x338 - 0x2FA];
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
u8 boxes_overlap_oriented(s16 *box, struct Enemy *ent, void *entry, s32 zAbove, s32 zBelow);
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
s32 sub_081DE850(s32 n, u32 seed);
void enemy_jump_partner(void);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void enemy_follow_or_hop(void);

ASM_FUNC("asm/code_08149500/bump_attack_enemy_move_3d_step.s", void bump_attack_enemy_move_3d_step(void))

ASM_FUNC("asm/code_08149500/enemy_place_body_quadrant.s", void enemy_place_body_quadrant(void))

ASM_FUNC("asm/code_08149500/enemy_touches_companion_oriented.s", void enemy_touches_companion_oriented(void))

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
    if (boxes_overlap_oriented(box, ent, entry, box[4], box[5]))
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

ASM_FUNC("asm/code_08149500/bump_attack_enemy_reset_idle.s", void bump_attack_enemy_reset_idle(void))

void clear_obj_flags(struct Enemy *obj)
{
    ((struct Flags8 *)&obj->unk20D)->bit3 = 0;
    ((struct Flags8 *)&obj->unk20C)->bit6 = 0;
    ((struct Flags8 *)&obj->unk20C)->bit7 = 0;
    ((struct Flags8 *)&obj->unk20C)->bit5 = 0;
    ((struct Flags8 *)&obj->unk20F)->bit2 = 0;
}

ASM_FUNC("asm/code_08149500/enemy_fall_end_state.s", void enemy_fall_end_state(void))

ASM_FUNC("asm/code_08149500/enemy_fall_decay.s", void enemy_fall_decay(void))

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
        idx = sub_081DE850(4, mt19937_next());
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

ASM_FUNC("asm/code_08149500/enemy_move_axis.s", void enemy_move_axis(void))

ASM_FUNC("asm/code_08149500/enemy_fall_landing_step.s", void enemy_fall_landing_step(void))

ASM_FUNC("asm/code_08149500/enemy_hop_to_partner.s", void enemy_hop_to_partner(void))

ASM_FUNC("asm/code_08149500/enemy_state_step_xyz.s", void enemy_state_step_xyz(void))

ASM_FUNC("asm/code_08149500/enemy_update_draw_flags.s", void enemy_update_draw_flags(void))

ASM_FUNC("asm/code_08149500/set_box_flag_1d8.s", void set_box_flag_1d8(void))

ASM_FUNC("asm/code_08149500/enemy_start_fall.s", void enemy_start_fall(void))

ASM_FUNC("asm/code_08149500/fall_landing_pick_1d8.s", void fall_landing_pick_1d8(void))

ASM_FUNC("asm/code_08149500/enemy_seek_partner_axis.s", void enemy_seek_partner_axis(void))

ASM_FUNC("asm/code_08149500/enemy_check_owner_box.s", void enemy_check_owner_box(void))

ASM_FUNC("asm/code_08149500/enemy_start_fall_alt.s", void enemy_start_fall_alt(void))

ASM_FUNC("asm/code_08149500/enemy_seek_partner_axis_alt.s", void enemy_seek_partner_axis_alt(void))

ASM_FUNC("asm/code_08149500/entry_box_check.s", void entry_box_check(void))

ASM_FUNC("asm/code_08149500/enemy_pick_entry_sprite.s", void enemy_pick_entry_sprite(void))

ASM_FUNC("asm/code_08149500/enemy_follow_or_hop.s", void enemy_follow_or_hop(void))

ASM_FUNC("asm/code_08149500/enemy_aim_at_entry.s", void enemy_aim_at_entry(void))

ASM_FUNC("asm/code_08149500/enemy_tile_move.s", void enemy_tile_move(void))
