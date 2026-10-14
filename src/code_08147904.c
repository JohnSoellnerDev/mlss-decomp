#include "global.h"
#include "ram_routines.h"

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

struct SpriteBits12
{
    u8 bit0 : 1;
    u8 field1 : 2;
    u8 rest : 5;
};

struct OwnerFlags
{
    u8 unk0 : 2;
    u8 bit2 : 1;
    u8 unk3 : 5;
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
    struct OwnerFlags flags208;
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

struct Enemy
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x100 - 0x25];
    u16 wanderDist[4];
    u8 filler108[0x130 - 0x108];
    u16 wanderSteps[2];
    u8 filler134[0x198 - 0x134];
    void *unk198;
    void *unk19C;
    void *unk1A0[2];
    u8 filler1A8[0x1B0 - 0x1A8];
    struct Record *rec;
    u8 filler1B4[0x1D0 - 0x1B4];
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
    u8 filler1EC[0x20D - 0x1EC];
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x214 - 0x211];
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x23E - 0x217];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x27A - 0x246];
    u16 unk27A;
    u8 filler27C[0x294 - 0x27C];
    struct Sprite *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 unk2B6;
    u8 filler2B7[0x2DC - 0x2B7];
    u16 speed;
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

u8 step_pos_xy_plain(struct Enemy *ent);
void reset_record_fields(struct Enemy *ent);
u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 value);
void run_state_handler(void);
void tick_store(void);
void enemy_move_home(void);
void enemy_home_step(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_wander_to_edge(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_seek_partner(void);
void enemy_guard_aim(void);
void enemy_step_wait(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_hop_start(void);
void enemy_aim_and_move(void);
void obj_box_mirror_to_stack(s16 *box, struct Enemy *ent, u32 flags, u32 index);
u8 boxes_overlap(s16 *box, struct Enemy *ent, struct Partner *other, s32 zAbove, s32 zBelow);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void load_record_fields(struct Enemy *ent, u32 mode);
void load_hit_box_words(struct Enemy *ent, u32 mode);
void clear_obj_flags(void *obj);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void enemy_zone_pick(void);
extern const u8 gFrameAttrTable[];
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void enemy_face_owner(void);
void enemy_place_on_owner(void);
void enemy_wait_scan(void);
void enemy_fall_end_state(void);
void enemy_fall_decay(void);
void enemy_fall_pos(void);
void enemy_bump_wait(void);
void enemy_collide_settle(void);
void enemy_start_bump(void);
void enemy_idle_walk(void);
s8 zone_point_code(struct Owner *owner, struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void enemy_start_run(void);
void enemy_chase_partner(void);
void seek_run_enemy_return_home(struct Owner *owner, struct Enemy *ent, u32 *out);
u8 enemy_companion_hit(struct Owner *owner, struct Enemy *ent);
void enemy_pose_a(struct Owner *owner, struct Enemy *ent, u32 *out);
u8 enemy_touches_companion_oriented(struct Owner *owner, struct Enemy *ent);
u8 step_pos_xy_c(struct Enemy *ent);
void obj_refresh_sub_attr(struct Enemy *ent);
void collide_react(struct Owner *owner, struct Enemy *ent, void *target, s32 part, s32 speed, s32 a, s32 b, s32 c, s32 zero);
extern const s16 gSineTable[];
u8 tile_blocks_move(struct Owner *owner, s32 x, s32 y, s32 z, s32 w, s32 one, s32 zero);
void enemy_run_return(struct Owner *owner, struct Enemy *ent, u32 *out);
void wander_to_edge(struct Enemy *ent, s32 steps, s32 dist);
void obj_set_or_add_pos(struct Enemy *ent, s32 x, s32 y, s32 z, u32 zero0, u32 zero1);

void enemy_body_step(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Enemy *body = ent->body;
    s32 t;

    if ((ent->unk1D6 & 0x10) == 0)
    {
        t = ent->unk1D2 - 1;
        ent->unk1D2 = t;
        if ((t << 16) == 0)
        {
            obj_set_cell_coord(ent, MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((u32)ent->unk24 << 29 >> 29, 2), 0);
            ((struct Flags8 *)&ent->unk1D6)->bit4 = 1;
        }
    }
    body->posX = body->posX + body->vx;
    body->posY = body->posY + body->vy;
    if ((body->unk20F & 2) != 0 && (body->unk216 & 0x20) == 0 && (*((u8 *)body + 0x20C) & 1) != 0)
    {
        u8 *ctl = (u8 *)body->unk338 + 0x68;
        s32 off = *(s16 *)ctl;

        if (((u8 (*)(struct Enemy *))(*(u32 *)(ctl + 4)))((struct Enemy *)((u8 *)body + off)) == 0)
        {
            return;
        }
    }
    obj_set_cell_coord(ent, MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((u32)ent->unk24 << 29 >> 29, 2), 0);
    obj_set_active(body, 0);
    obj_set_sprite_link(body, 0);
    *(u16 *)(ai + 0x17A) = 1;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
    out[0] = (u32)tick_store;
    out[3] = (u32)enemy_start_run;
    if (zone_point_code(owner, ent, 0) != -1)
    {
        out[2] = (u32)enemy_chase_partner;
    }
    else
    {
        out[2] = (u32)seek_run_enemy_return_home;
    }
}

void enemy_run_return(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    s32 step;

    ent->unk1D4 = ent->unk1D4 + 1;
    if (ent->unk1D4 > 1)
    {
        ent->unk1D2 = ent->unk1D2 + 1;
        if ((ent->unk1D6 & 4) != 0)
        {
            obj_set_sprite_link(ent, 0);
            ent->vx = ent->speed;
            step = ent->vx;
        }
        else
        {
            obj_set_sprite_link(ent, 1);
            ent->vx = -ent->speed;
            step = ent->vx;
        }
        ent->posX = ent->posX + step;
        if (*(u16 *)(ai + 0x17A) > 1)
        {
            if (((struct Flags8 *)(ai + 0x17E))->bit2)
            {
                ((struct Flags8 *)(ai + 0x17E))->bit2 = 0;
            }
            else
            {
                ((struct Flags8 *)(ai + 0x17E))->bit2 = 1;
            }
            *(u16 *)(ai + 0x17A) = 0;
        }
        *(u16 *)(ai + 0x17C) = 0;
    }
    {
        s32 left = ent->runLeft - 1;

        ent->runLeft = left;
        if ((left << 16) == 0)
        {
            obj_set_sprite_link(ent, 1);
            ((struct Flags8 *)((u8 *)ent + 0x20C))->bit5 = 1;
            ((struct Flags8 *)((u8 *)ent + 0x20C))->bit7 = 1;
            ((struct Flags8 *)((u8 *)ent + 0x20C))->bit6 = 1;
            ((struct Flags8 *)&ent->unk210)->bit4 = 1;
            ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
            ((struct Mode3 *)&ent->unk214)->mode = 1;
            {
                struct Sprite *sprite = ent->sprite;
                u32 index = *(u16 *)(ai + 0x178);
                u32 rec = *(u32 *)(ai + 0x158);

                sprite->unk20 = *((u8 *)(index + rec) + 6);
            }
            out[0] = (u32)enemy_wait_scan;
        }
    }
}

void enemy_clear_body_flags(struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk20E)->bit2 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit3 = 1;
    ((struct Flags8 *)&ent->unk20D)->bit5 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit6 = 0;
    *(u32 *)((u8 *)ent + 0x20C) = *(u32 *)((u8 *)ent + 0x20C) & 0xFFFC7FFF;
    ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit7 = 1;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit6 = 1;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit5 = 1;
    ((struct Flags8 *)&ent->unk20E)->bit2 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
}

void seek_run_enemy_return_home(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    obj_set_or_add_pos(ent, ent->homeX, ent->homeY, ent->homeZ + ent->homeDZ, 0, 0);
    ent->speed = 0x200;
    ent->runLeft = 0x3C;
    out[0] = (u32)enemy_run_return;
}

ASM_FUNC("asm/code_08147904/enemy_move_to_partner.s", void enemy_move_to_partner(void))

void enemy_start_guard(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;
    s32 pick = sub_081DE850(4, mt19937_next());

    u8 *slot = (u8 *)(pick + (u32)rec);

    ent->unk1D4 = slot[0x1E];
    {
        u8 *p = &ent->unk1D6;
        *p = *p | 0x10;
    }
    *(u32 *)((u8 *)ent + 0x20C) = (*(u32 *)((u8 *)ent + 0x20C) & 0xFFFC7FFF) | 0x8000;
    {
        u8 *p = &ent->unk20F;
        *p = *p | 4;
    }
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    out[0] = (u32)enemy_guard_aim;
}

void enemy_wander_to_edge(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u16 *table = ent->wanderDist;
    s32 pick1 = sub_081DE850(4, mt19937_next());
    u16 dist = table[pick1];
    s32 pick2 = sub_081DE850(2, mt19937_next());
    u16 steps = *(u16 *)((u8 *)table + pick2 * 2 + 0x30);

    wander_to_edge(ent, steps, dist);
    ((struct Mode3 *)&ent->unk214)->mode = 3;
    out[0] = (u32)enemy_seek_partner;
}

void enemy_go_idle(struct Owner *owner, struct Enemy *ent)
{
    {
        u8 *p = &ent->unk33E;
        *p = *p | 0x10;
    }
    reset_record_fields(ent);
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_wander_to_edge;
}

void enemy_step_wait(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    if (step_pos_xy_plain(ent) == 0)
    {
        out[0] = (u32)enemy_move_home;
    }
}

ASM_FUNC("asm/code_08147904/enemy_move_home.s", void enemy_move_home(void))

void enemy_home_step(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;

    if (step_pos_xy_plain(ent) == 0)
    {
        ent->unk1D2 = rec->unk3E;
        out[0] = (u32)tick_store;
        out[3] = (u32)enemy_wander_to_edge;
    }
}

void enemy_release_body(struct Owner *owner, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk33E)->bit4 = 0;
    load_record_fields(ent, 0);
    load_hit_box_words(ent, 0);
    clear_obj_flags(ent->body);
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit7 = 0;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit6 = 0;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit5 = 0;
    obj_set_sprite_link(ent, 0);
    ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
    ((struct Flags8 *)&ent->unk20E)->bit2 = 1;
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_zone_pick;
}

ASM_FUNC("asm/code_08147904/enemy_jump_partner.s", void enemy_jump_partner(void))

ASM_FUNC("asm/code_08147904/enemy_move_checked.s", void enemy_move_checked(void))

ASM_FUNC("asm/code_08147904/enemy_update_landing.s", void enemy_update_landing(void))

ASM_FUNC("asm/code_08147904/enemy_start_bump.s", void enemy_start_bump(void))

ASM_FUNC("asm/code_08147904/enemy_bump_wait.s", void enemy_bump_wait(void))

ASM_FUNC("asm/code_08147904/enemy_aim_and_move.s", void enemy_aim_and_move(void))

ASM_FUNC("asm/code_08147904/enemy_move_to_target.s", void enemy_move_to_target(void))

ASM_FUNC("asm/code_08147904/enemy_hop_start.s", void enemy_hop_start(void))

ASM_FUNC("asm/code_08147904/enemy_hop_3d.s", void enemy_hop_3d(void))

ASM_FUNC("asm/code_08147904/enemy_collide_settle.s", void enemy_collide_settle(void))

ASM_FUNC("asm/code_08147904/enemy_idle_walk.s", void enemy_idle_walk(void))

void enemy_pose_a(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;
    s32 pick = sub_081DE850(4, mt19937_next());
    u16 *pindex = &ent->index;

    *pindex = pick;
    obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 4, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 1;
    ent->sprite->unk20 = *((u8 *)(*pindex + (u32)rec) + 0x22);
    ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
    out[0] = (u32)enemy_face_owner;
}

ASM_FUNC("asm/code_08147904/enemy_face_owner.s", void enemy_face_owner(void))

ASM_FUNC("asm/code_08147904/enemy_place_on_owner.s", void enemy_place_on_owner(void))

ASM_FUNC("asm/code_08147904/enemy_after_step_906c.s", void enemy_after_step_906c(void))
