#include "global.h"
#include "ram_routines.h"

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct Sprite
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 frame20;
    u8 unk21;
    u8 filler22[0x2C - 0x22];
    s16 unk2C;
};

struct Flags20D
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Bits2
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Word20C
{
    u32 low15 : 15;
    u32 f15 : 3;
    u32 rest : 14;
};

struct Flag210
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Home
{
    u8 filler0[0x178];
    u16 index;
    u8 filler17A[0x184 - 0x17A];
    s32 homeX;
    s32 homeY;
};

struct Bits1
{
    u8 bit0 : 1;
    u8 field1 : 2;
    u8 rest : 5;
};

struct Actor;

struct Owner
{
    u8 filler0[0x28];
    void *entries[4];
    u8 filler38[0x200 - 0x38];
    struct Actor *tgtA;
    struct Actor *tgtB;
    u8 flags208;
};

struct Rec
{
    u8 filler0[0x23];
    u8 unk23;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 filler28[0x38 - 0x28];
    u16 unk38;
};

struct Actor
{
    u8 filler0[0x2];
    u8 angle02;
    u8 filler3[0xC - 0x3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 dz;
    u8 filler1C[0x100 - 0x1C];
    u16 table[0x20];
    u8 filler140[0x198 - 0x140];
    void *state;
    void *stateNext;
    void *handler;
    void *handlerNext;
    void *prep;
    void *callback;
    struct Rec *rec;
    s16 *sensor;
    s16 vecX;
    s16 vecY;
    s16 vecZ;
    u8 filler1BE[0x1D0 - 0x1BE];
    u16 index;
    u16 timerB;
    u16 timer;
    u8 flags1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDz;
    u8 filler1EC[0x20D - 0x1EC];
    u8 flags20D;
    u8 flags20E;
    u8 filler20F[0x210 - 0x20F];
    u8 flags210;
    u8 filler211[0x214 - 0x211];
    u8 mode;
    u8 filler215[0x239 - 0x215];
    u8 byte239;
    u8 filler23A[0x23E - 0x23A];
    s16 speed2;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u8 filler24C[0x274 - 0x24C];
    u16 scale;
    u8 filler276[0x294 - 0x276];
    struct Sprite *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 flags2B6;
    u8 filler2B7[0x2D4 - 0x2B7];
    s32 tgtZ;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 zspeed;
    u8 filler2DE[0x2EC - 0x2DE];
    s32 landZ;
    u8 filler2F0[0x2F4 - 0x2F0];
    u32 phase;
    s16 hopStep;
    u8 filler2FA[0x33E - 0x2FA];
    u8 flags33E;
};

struct Flag33E
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Flags1D6
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bits3
{
    u8 field0 : 3;
    u8 rest : 5;
};

struct Flag2B6
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 rest : 1;
};

u8 step_pos_xyz(struct Actor *ent);
void enemy_hop_land_on_partner(void);
void enemy_start_partner_walk(void);
void enemy_collide_random_kick(void);
void enemy_partner_in_box_check(struct Owner *owner, struct Actor *ent, void **out);
void enemy_sprite_scale_check(void *owner, struct Actor *ent, void **out);
void enemy_state_wander_run(void);
void enemy_contact_check_state(struct Owner *owner, struct Actor *ent);
void enemy_begin_motion(void *owner, struct Actor *ent, void **out);
u8 step_pos_xy_c(struct Actor *ent);
void collide_react(struct Owner *owner, struct Actor *ent, struct Actor *bro, u32 kick, u32 speed, s32 offX, s32 offY, u32 a, u32 b);
void enemy_run_move(void);
void load_record_fields(struct Actor *ent, u32 mode);
void load_hit_box_words(struct Actor *ent, u32 mode);
void setup_random_offset_state(void);
void obj_random_target_b(void);
void aim_and_start_move(void);
void obj_set_cell_coord(struct Actor *ent, u32 cell, u32 arg);
void obj_set_sprite_link(struct Actor *ent, u32 zero);
void start_move_to_3d(struct Actor *ent, s32 x, s32 y, s32 z, s32 speed, ByteArg zero);
void enemy_after_step_box_check(void *owner, struct Actor *ent, void **out);
void enemy_load_frame_wait(void *owner, struct Actor *ent, void **out);
void enemy_enter_next_state(void *owner, struct Actor *ent);
void enemy_contact_state_handler(void *owner, struct Actor *ent, void **out);
void enemy_run_handler_advance(void *owner, struct Actor *ent);
void link_to_owner_entry(void);
void load_record_fields_scaled(struct Actor *ent, u32 mode);
void obj_state_run_advance(void);
void enemy_setup_wait(void);
void enemy_end_countdown(void);
void enemy_wait_after_step(void *owner, struct Actor *ent, void **out);
void enemy_wait_after_step_b(void *owner, struct Actor *ent, void **out);
u16 angle_of_vector16(s32 x, s32 y);
void start_move_to(struct Actor *ent, s32 x, s32 y, s32 speed, ByteArg zero);
u8 obj_flags_2b5_30_set(struct Actor *ent);
void enemy_timer_then_cell(void);
void enemy_wait_then_wander(void);
void enemy_hop_face_partner(void);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void wander_to_edge(struct Actor *ent, s32 steps, s32 dist);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void load_box_oriented(s16 *box, struct Actor *ent, u32 a, u32 b);
u8 boxes_overlap_oriented(s16 *box, struct Actor *ent, void *entry, s32 zAbove, s32 zBelow);
void enemy_hop_state_runner(void);
void enemy_hop_start_kick(void);
void run_state_handler(void);
void reset_record_fields(struct Actor *ent);
void sprite_step_e6a0(void *spr);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_set_motion_a_08046AA4(void *obj, u32 a, u32 b, s32 c, u32 d);
void obj_advance_axes(void *obj);
void tick_store(void);
void start_wander_b(void);
void enemy_hop_return_home(void);
void enemy_wander_random_edge(void *owner, struct Actor *ent, void **out);
void enemy_collide_then_idle();
void enemy_start_motion_if_free(void *owner, struct Actor *ent, void **out);
void enemy_kick_after_probe(void);
void random_collide(void);
void load_record_state_b(void);
void start_wander(void);
u8 step_pos_xy_plain(struct Actor *ent);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
void obj_box_to_stack(s16 *box, struct Actor *ent, u32 a, u32 b);
void contact_states_enemy_return_home(void *owner, struct Actor *ent, void **out);

void enemy_enter_contact_state(void *owner, struct Actor *ent);

void enemy_start_return_home(void *owner, struct Actor *ent, void **out)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *rec = (u8 *)ent->rec;
    s32 dx;
    s32 dy;
    s32 facing;
    s32 (*div)(s32, s32);
    u32 cell;
    u16 speed;
    u8 zero = 0;

    ((struct Bits3 *)&ent->mode)->field0 = 4;
    ent->sprite->frame20 = rec[ent->index + 0x42];
    dx = ent->homeX - ent->posX;
    dy = ent->homeY - ent->posY;
    if ((dx | dy) != 0)
    {
        ent->angle02 = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        ent->angle02 = 0xFF;
    }
    div = (s32 (*)(s32, s32))(gMathRoutines + ((u32)iwram_div - (u32)iwram_sqrt));
    dx = *(s32 *)(ai + 0x184) - ent->posX;
    dy = *(s32 *)(ai + 0x188) - ent->posY;
    if ((dx | dy) != 0)
    {
        facing = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
    }
    else
    {
        facing = -1;
    }
    cell = div(facing, 2);
    obj_set_cell_coord(ent, cell, 0);
    speed = *(u16 *)(rec + *(u16 *)(ai + 0x178) * 2 + 0x46);
    start_move_to(ent, *(s32 *)(ai + 0x184), *(s32 *)(ai + 0x188), speed, zero);
    *out = enemy_wait_after_step_b;
}


void enemy_idle_handler(void);
void enemy_engage_setup(struct Owner *owner, struct Actor *ent, void **out);
void enemy_collide_then_idle(struct Owner *owner, struct Actor *ent, void **out)
{
    u16 *table = ent->table;
    u8 *rec = (u8 *)ent->rec;
    u8 idx;
    u32 index;
    u32 speed;
    u32 kick;
    u8 offX;
    u8 offY;
    s32 flag;
    struct Actor *partner;
    s32 cell;

    ent->index = rand_below(4, mt19937_next());
    if (((struct Flags1D6 *)&ent->flags1D6)->bit4)
    {
        ((struct Bits3 *)&ent->mode)->field0 = 2;
        ent->sprite->frame20 = rec[ent->index + 0x28];
        speed = *(u16 *)(rec + ent->index * 2 + 0x2C);
        kick = rec[rand_below(2, mt19937_next()) + 0x34];
        offX = rec[0x36];
        offY = rec[0x37];
        flag = 4;
    }
    else
    {
        ((struct Bits3 *)&ent->mode)->field0 = 4;
        ent->sprite->frame20 = table[ent->index + 4];
        speed = table[ent->index];
        kick = table[rand_below(2, mt19937_next()) + 0x18];
        offX = ent->filler140[0];
        offY = ent->filler140[1];
        flag = 0;
    }
    if (find_active_entry(owner, &idx))
    {
        index = idx;
    }
    else
    {
        index = (u32)(owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)owner)[index + 10];
    collide_react(owner, ent, partner, kick, speed, (s8)offX, (s8)offY, 0x22, 0);
    cell = ((s32 (*)(s32, s32))(gMathRoutines + ((u32)iwram_div - (u32)iwram_sqrt)))((u32)(ent->filler1C[0x24 - 0x1C] << 29) >> 29, 2);
    obj_set_cell_coord(ent, cell + (s8)flag, 0);
    *out = enemy_idle_handler;
}

ASM_FUNC("asm/code_08142A38/enemy_idle_handler.s", void enemy_idle_handler(void))

void enemy_charge_handler(struct Owner *owner, struct Actor *ent, void **out);
void enemy_sway_then_check(void *owner, struct Actor *ent, void **out);
NONMATCH("asm/code_08142A38/enemy_engage_setup.s", void enemy_engage_setup(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *rec = (u8 *)ent->rec;
    u8 idx;
    u32 index;
    struct Actor *partner;
    s32 dx;
    s32 dy;
    s32 facing;
    u8 *dp;
    u32 dir;
    u32 cell;

    if (find_active_entry(owner, &idx))
    {
        index = idx;
    }
    else
    {
        index = (u32)(owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)owner)[index + 10];
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    ent->byte239 = rec[*(u16 *)(ai + 0x178) + 0x40];
    dx = partner->posX - ent->posX;
    dy = partner->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        facing = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
    }
    else
    {
        facing = -1;
    }
    dp = &ent->filler1C[0x24 - 0x1C];
    facing &= 7;
    dir = (*dp & -8) | facing;
    *dp = dir;
    cell = ((s32 (*)(s32, s32))(gMathRoutines + ((u32)iwram_div - (u32)iwram_sqrt)))((u32)(dir << 29) >> 29, 2);
    obj_set_cell_coord(ent, cell, 0);
    {
        u8 tv = rec[*(u16 *)(ai + 0x178) + 0x3C];

        *(u16 *)(ai + 0x17A) = tv;
    }
    *(s32 *)(ai + 0x184) = ent->posX;
    *(s32 *)(ai + 0x188) = ent->posY;
    *out = enemy_charge_handler;
}
END_NONMATCH

NONMATCH("asm/code_08142A38/enemy_charge_handler.s", void enemy_charge_handler(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 *dp;
    u8 zero = 0;
    u8 *ai = (u8 *)ent + 0x58;
    u8 *rec = (u8 *)ent->rec;
    s16 box[8];
    u8 idx;
    struct Actor *partner;
    s32 dx;
    s32 dy;
    s32 facing;
    u32 cell;
    u16 speed;
    u32 dir;

    if (ent->timerB != 0)
    {
        s32 t = ent->timerB - 1;

        ent->timerB = t;
        if ((t << 16) != 0)
        {
            return;
        }
        if (!find_active_entry(owner, &idx))
        {
            out[0] = enemy_sway_then_check;
            return;
        }
        partner = ((struct Actor **)owner)[idx + 10];
        ((struct Bits3 *)&ent->mode)->field0 = 1;
        dx = partner->posX - ent->posX;
        dy = partner->posY - ent->posY;
        if ((dx | dy) != 0)
        {
            facing = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
        }
        else
        {
            facing = -1;
        }
        dp = &ent->filler1C[0x24 - 0x1C];
        facing &= 7;
        dir = (*dp & -8) | facing;
        *dp = dir;
        cell = ((s32 (*)(s32, s32))(gMathRoutines + ((u32)iwram_div - (u32)iwram_sqrt)))((u32)(dir << 29) >> 29, 2);
        obj_set_cell_coord(ent, cell, 0);
        speed = *(u16 *)(rec + *(u16 *)(ai + 0x178) * 2 + 0x44);
        start_move_to(ent, partner->posX + partner->vx * 20, partner->posY + partner->vy * 20, speed, zero);
        return;
    }
    if (!step_pos_xy_plain(ent))
    {
        obj_box_to_stack(box, ent, 0x12, 0);
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            ent->timerB = rec[*(u16 *)(ai + 0x178) + 0x58];
            out[0] = tick_store;
            out[3] = start_wander_b;
            out[1] = enemy_collide_then_idle;
        }
        else
        {
            out[0] = contact_states_enemy_return_home;
        }
    }
}
END_NONMATCH

void enemy_sway_then_check(void *owner, struct Actor *ent, void **out)
{
    u8 **routines = &gMathRoutines;
    s16 box[8];

    ent->sprite->frame20 = ((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(*(s16 *)&ent->filler2FA, 0x100);
    *(s16 *)&ent->filler2FA += *(u16 *)&ent->filler2FA[0x30A - 0x2FA];
    if (--ent->timerB == 0)
    {
        s8 *mode = (s8 *)&ent->mode;
        *mode = (*mode & -8) | 1;
        obj_set_cell_coord(ent, ((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))((u32)(ent->filler1C[0x24 - 0x1C] << 29) >> 29, 2), 0);
        obj_box_to_stack(box, ent, 0x22, 0);
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            out[0] = start_wander_b;
            out[1] = enemy_collide_then_idle;
        }
        else
        {
            out[0] = contact_states_enemy_return_home;
        }
    }
}

void contact_states_enemy_return_home(void *owner, struct Actor *ent, void **out)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *rec = (u8 *)ent->rec;
    s32 dx;
    s32 dy;
    s32 facing;
    s32 (*div)(s32, s32);
    u32 cell;
    u16 speed;
    u8 zero = 0;

    ((struct Bits3 *)&ent->mode)->field0 = 4;
    ent->sprite->frame20 = rec[ent->index + 0x4C];
    dx = ent->homeX - ent->posX;
    dy = ent->homeY - ent->posY;
    if ((dx | dy) != 0)
    {
        ent->angle02 = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        ent->angle02 = 0xFF;
    }
    div = (s32 (*)(s32, s32))(gMathRoutines + ((u32)iwram_div - (u32)iwram_sqrt));
    dx = *(s32 *)(ai + 0x184) - ent->posX;
    dy = *(s32 *)(ai + 0x188) - ent->posY;
    if ((dx | dy) != 0)
    {
        facing = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
    }
    else
    {
        facing = -1;
    }
    cell = div(facing, 2);
    obj_set_cell_coord(ent, cell, 0);
    speed = *(u16 *)(rec + *(u16 *)(ai + 0x178) * 2 + 0x50);
    start_move_to(ent, *(s32 *)(ai + 0x184), *(s32 *)(ai + 0x188), speed, zero);
    *out = enemy_wait_after_step;
}


void enemy_wait_after_step(void *owner, struct Actor *ent, void **out)
{
    u8 *rec = (u8 *)ent->rec;

    if (!step_pos_xy_plain(ent))
    {
        ent->timerB = rec[ent->index + 0x58];
        out[0] = tick_store;
        out[3] = start_wander_b;
        out[1] = enemy_collide_then_idle;
    }
}

void enemy_begin_motion(void *owner, struct Actor *ent, void **out)
{
    struct Rec *rec = ent->rec;

    obj_set_motion_a_08046AA4(ent, 2, 0, rec->unk24, rec->unk26);
    obj_advance_axes(ent);
    ((struct Flags1D6 *)&ent->flags1D6)->bit0 = 0;
    out[0] = enemy_start_motion_if_free;
}

void contact_states_enemy_reset_idle(void *owner, struct Actor *ent, void **out)
{
    ((struct Flag33E *)&ent->flags33E)->bit4 = 1;
    reset_record_fields(ent);
    ent->timer = 0;
    ((struct Flags1D6 *)&ent->flags1D6)->bit4 = 0;
    ent->homeDz = 0;
    ent->state = run_state_handler;
    ent->handler = enemy_collide_then_idle;
}

u8 find_touching_partner(struct Owner *owner, struct Actor *ent, struct Actor **out)
{
    u8 idx;
    s16 box[8];
    struct Actor *partner;

    if (!find_active_entry(owner, &idx))
    {
        return 0;
    }
    partner = ((struct Actor **)owner)[idx + 10];
    load_box_oriented(box, ent, 0x11, 0x1C);
    if (!boxes_overlap_oriented(box, ent, partner, box[4], box[5]))
    {
        return 0;
    }
    if (out)
    {
        *out = partner;
    }
    return 1;
}

void enemy_wait_after_step_b(void *owner, struct Actor *ent, void **out)
{
    u8 *rec = (u8 *)ent->rec;

    if (!step_pos_xy_plain(ent))
    {
        ent->timerB = rec[ent->index + 0x4E];
        out[0] = tick_store;
        out[3] = start_wander_b;
        out[1] = enemy_collide_random_kick;
    }
}

void enemy_wait_sprite_timer(void *owner, struct Actor *ent, void **out)
{
    if (ent->sprite->flags12 & 8)
    {
        s32 t = ent->timerB - 1;

        ent->timerB = t;
        if ((t << 16) == 0)
        {
            out[0] = enemy_start_partner_walk;
        }
    }
}

void enemy_reset_state_b(void *owner, struct Actor *ent, void **out)
{
    ((struct Flag33E *)&ent->flags33E)->bit4 = 1;
    reset_record_fields(ent);
    ent->state = run_state_handler;
    ent->handler = enemy_collide_random_kick;
}

void enemy_z_step_done_wait(void *owner, struct Actor *ent, void **out)
{
    u8 *rec = (u8 *)ent->rec;
    u8 flag = step_pos_xyz(ent);

    if (!flag)
    {
        ent->landZ = ent->posZ + ent->dz;
        ent->phase = flag;
        ent->timerB = rec[ent->index + 0x44];
        out[0] = tick_store;
        out[3] = start_wander_b;
        out[1] = enemy_wander_random_edge;
    }
}

void enemy_move_home_3d(void *owner, struct Actor *ent, void **out)
{
    u8 *rec = (u8 *)ent->rec;
    u16 speed;
    u8 zero;

    ent->sprite->frame20 = rec[ent->index + 0x38];
    speed = *(u16 *)(rec + ent->index * 2 + 0x3C);
    zero = 0;
    start_move_to_3d(ent, ent->homeX, ent->homeY, ent->homeZ + ent->homeDz, speed, zero);
    out[0] = enemy_z_step_done_wait;
}

void enemy_wander_random_edge(void *owner, struct Actor *ent, void **out)
{
    u16 *table = ent->table;
    s32 pick = rand_below(4, mt19937_next());
    u16 dist = table[pick];
    s32 pick2 = rand_below(2, mt19937_next());

    wander_to_edge(ent, table[pick2 + 0x18], dist);
    out[0] = enemy_wait_then_wander;
}

void enemy_start_wander_setup(void *owner, struct Actor *ent, void **out)
{
    struct Rec *rec = ent->rec;

    ((struct Flag33E *)&ent->flags33E)->bit4 = 1;
    reset_record_fields(ent);
    ent->landZ = ent->posZ + ent->dz;
    ent->phase = 0;
    ent->hopStep = rec->unk23;
    ent->timer = 0;
    ent->state = enemy_state_wander_run;
    ent->handler = enemy_wander_random_edge;
}

void enemy_rand_setup(struct Actor *ent)
{
    struct Rec **slot = &ent->rec;
    u8 *rec = (u8 *)*slot;

    ent->index = rand_below(4, mt19937_next());
    ent->sprite->frame20 = rec[ent->index + 0x1A];
    ent->timerB = ((u8 *)*slot)[ent->index + 0x16];
    ent->handler = enemy_timer_then_cell;
}

u8 enemy_sensor_hit(struct Actor *ent, struct Actor *target)
{
    u8 *boxes = (u8 *)ent->sensor;
    u8 count = boxes[0xB];
    u8 i;
    s16 box[8];

    for (i = 0; i < count; i++)
    {
        obj_box_to_stack(box, ent, 0x22, ((((u32)i << 19) + 0xC0000) >> 16));
        if (point_in_box(box, target, 0x7FFF, 0))
        {
            return 1;
        }
    }
    return 0;
}

void enemy_sibling_noop_state(void)
{
}

void enemy_activate(void *owner, struct Actor *ent, void **out)
{
    reset_record_fields(ent);
    ((struct Flags20D *)&ent->flags20D)->bit2 = 0;
    ((struct Flag210 *)&ent->flags210)->bit4 = 1;
    ent->timer = 0;
    obj_set_sprite_link(ent, 1);
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    obj_load_record(ent->sprite, 3, -1, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ent->state = run_state_handler;
    ent->handler = enemy_run_move;
}

void enemy_reset_timer(void *owner, struct Actor *ent, void **out)
{
    ((struct Flag33E *)&ent->flags33E)->bit4 = 1;
    reset_record_fields(ent);
    ent->timer = 0;
    ent->state = run_state_handler;
    ent->handler = enemy_kick_after_probe;
}

void enemy_fall_step(void *owner, struct Actor *ent, void **out)
{
    ent->dz += 0x100;
    if (ent->posZ + ent->dz >= ent->tgtZ)
    {
        ent->state = enemy_hop_state_runner;
        out[0] = enemy_hop_start_kick;
    }
}

void enemy_land_reload_sprite(void *owner, struct Actor *ent, void **out)
{
    obj_load_record(ent->sprite, ent->sprite->unk21, 2, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ent->dz = 0;
    out[0] = enemy_partner_in_box_check;
}

void enemy_wait_after_step_c(void *owner, struct Actor *ent, void **out)
{
    u8 *rec = (u8 *)ent->rec;

    if (!step_pos_xy_plain(ent))
    {
        ent->timerB = rec[ent->index + 0x6C];
        out[0] = tick_store;
        out[3] = start_wander_b;
        out[1] = enemy_hop_start_kick;
    }
}

void enemy_after_fall_check_box(void *owner, struct Actor *ent, void **out)
{
    s16 box[8];
    u8 flag = step_pos_xyz(ent);

    if (!flag)
    {
        ent->landZ = ent->posZ + ent->dz;
        ent->phase = flag;
        obj_box_to_stack(box, ent, 0x22, 0);
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            out[0] = start_wander_b;
            out[1] = enemy_hop_start_kick;
        }
        else
        {
            out[0] = enemy_hop_return_home;
        }
    }
}

struct Behavior
{
    u8 filler0[0x17A];
    u16 timerB;
};

void enemy_hop_fall_then_next(void *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);

    if (ent->dz > 0)
    {
        ent->dz -= ent->zspeed;
        if (ent->dz <= 0)
        {
            ent->dz = 0;
        }
        else
        {
            ent->zspeed += ent->accel;
        }
    }
    {
        s32 t = ai->timerB - 1;

        ai->timerB = t;
        if ((t << 16) == 0)
        {
            ((struct Flag33E *)&ent->flags33E)->bit3 = 0;
            out[0] = out[1];
        }
    }
}

void enemy_xyz_step_done(void *unused, struct Actor *ent, void **out)
{
    if (!step_pos_xyz(ent))
    {
        *out = enemy_hop_land_on_partner;
    }
}

u8 find_partner_in_box(struct Owner *owner, struct Actor *ent, struct Actor **out)
{
    u8 idx;
    s16 box[8];
    struct Actor *partner;

    if (!find_active_entry(owner, &idx))
    {
        return 0;
    }
    partner = ((struct Actor **)owner)[idx + 10];
    if (out)
    {
        *out = partner;
    }
    load_box_oriented(box, ent, 0x11, 0x1C);
    if (boxes_overlap_oriented(box, ent, partner, box[4], box[5]))
    {
        return 1;
    }
    return 0;
}

void enemy_box_point_then_collide(void *owner, struct Actor *ent, void **out)
{
    s16 box[8];

    obj_box_to_stack(box, ent, 0x22, 0);
    if (!point_in_box(box, ent, box[4], box[5]))
    {
        if (step_pos_xy_plain(ent))
        {
            return;
        }
    }
    out[0] = random_collide;
}

NONMATCH("asm/code_08142A38/enemy_dash_to_owner_target.s", void enemy_dash_to_owner_target(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 zero;
    struct Actor *target = owner->tgtA;
    u16 speed;

    ent->homeX = ent->posX;
    ent->homeY = ent->posY;
    speed = *(u16 *)((u8 *)ent->rec + ent->index * 2 + 0x2A);
    zero = 0;
    start_move_to(ent, target->posX + (target->vx << 4), target->posY + (target->vy << 4), speed, zero);
    *out = enemy_after_step_box_check;
}
END_NONMATCH

void enemy_wait_sprite_flag_then_load(void *owner, struct Actor *ent, void **out)
{
    if (ent->sprite->flags12 & 8)
    {
        ent->scale = 0x600;
        ((struct Bits3 *)&ent->mode)->field0 = 0;
        out[0] = load_record_state_b;
    }
}

void enemy_reset_collide(void *owner, struct Actor *ent, void **out)
{
    ((struct Flag33E *)&ent->flags33E)->bit4 = 1;
    reset_record_fields(ent);
    ent->timer = 0;
    ent->state = run_state_handler;
    ent->handler = random_collide;
}

void entity_mode_46_noop(void)
{
}

void enemy_set_default_handlers(void *owner, struct Actor *ent)
{
    ent->timerB = 0x1E;
    ent->handler = tick_store;
    ent->prep = start_wander_b;
    ent->speed2 = 0x200;
    ent->callback = start_wander;
}

void enemy_start_motion_if_free(void *owner, struct Actor *ent, void **out)
{
    struct Rec *rec = ent->rec;

    if (!((struct Flags1D6 *)&ent->flags1D6)->bit0)
    {
        if (!obj_flags_2b5_30_set(ent))
        {
            obj_set_motion_a_08046AA4(ent, 2, 0, 0x100, rec->unk27);
            obj_advance_axes(ent);
            ((struct Flags1D6 *)&ent->flags1D6)->bit0 = 1;
        }
    }
    else
    {
        if (!obj_flags_2b5_30_set(ent))
        {
            ent->timerB = rec->unk38;
            out[0] = enemy_collide_then_idle;
        }
    }
}

void enemy_partner_in_box_check(struct Owner *owner, struct Actor *ent, void **out)
{
    u8 idx;
    s16 box[8];
    struct Actor *partner;

    if (find_active_entry(owner, &idx))
    {
        partner = ((struct Actor **)owner)[idx + 10];
        obj_box_to_stack(box, ent, 0x22, 0xC);
        if (point_in_box(box, partner, 0x7FFF, 0))
        {
            out[0] = enemy_hop_face_partner;
        }
    }
}

void enemy_after_step_box_check(void *owner, struct Actor *ent, void **out)
{
    s16 box[8];

    if (!step_pos_xy_plain(ent))
    {
        obj_box_to_stack(box, ent, 0x22, 0);
        ent->timerB = (s8)((u8 *)ent->rec)[0x42];
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            out[0] = tick_store;
            out[3] = enemy_load_frame_wait;
            out[2] = start_wander_b;
            out[1] = random_collide;
        }
        else
        {
            out[0] = tick_store;
            out[3] = enemy_load_frame_wait;
            out[2] = aim_and_start_move;
        }
    }
}

void enemy_load_frame_wait(void *owner, struct Actor *ent, void **out)
{
    u8 *rec = (u8 *)ent->rec;

    ent->sprite->frame20 = rec[ent->index + 0x32];
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    ((struct Bits1 *)&ent->sprite->flags12)->field1 = 1;
    obj_load_record(ent->sprite, 5, 0, 0, 0);
    sprite_step_e6a0(ent->sprite);
    out[0] = enemy_sprite_scale_check;
}

void enemy_sprite_scale_check(void *owner, struct Actor *ent, void **out)
{
    if (ent->sprite->unk2C > 0x18F)
    {
        ent->scale = 0x1200;
        obj_load_record(ent->sprite, 5, 7, 0, 0);
        sprite_step_e6a0(ent->sprite);
        ((struct Bits3 *)&ent->mode)->field0 = 0;
        out[0] = out[2];
    }
}

void enemy_contact_check_state(struct Owner *owner, struct Actor *ent)
{
    void **slot = &ent->handler;
    u8 idx;
    struct Actor *partner;
    s8 *rec;

    ((void (*)(void *, struct Actor *, void **))*slot)(owner, ent, slot);
    if (!find_active_entry(owner, &idx))
    {
        return;
    }
    partner = ((struct Actor **)owner)[idx + 10];
    rec = (s8 *)ent->rec;
    {
        s16 box[8];

        box[0] = rec[0x1A] << 4;
        box[2] = rec[0x1B] << 4;
        box[4] = rec[0x1C] << 4;
        box[1] = rec[0x1D] << 4;
        box[3] = rec[0x1E] << 4;
        box[5] = rec[0x1F] << 4;
        if (boxes_overlap_oriented(box, ent, partner, box[4], box[5]))
        {
            ent->state = enemy_enter_next_state;
            ent->stateNext = enemy_contact_state_handler;
        }
    }
}

void enemy_contact_state_handler(void *owner, struct Actor *ent, void **out)
{
    u8 *ai = (u8 *)ent + 0x58;
    void **slot = &ent->handler;
    u8 idx;
    struct Actor *partner;

    ((void (*)(void *, struct Actor *, void **))*slot)(owner, ent, slot);
    if (!(ent->flags1D6 & 1))
    {
        return;
    }
    if (find_active_entry(owner, &idx))
    {
        partner = ((struct Actor **)owner)[idx + 10];
        {
            s16 box[8];
            u8 *rec;
            u8 *f36;

            rec = (u8 *)ent->rec;

            f36 = rec + 0x36;
            box[0] = (s8)*f36 << 4;
            box[2] = (s8)rec[0x37] << 4;
            box[4] = (s8)rec[0x38] << 4;
            box[1] = (s8)rec[0x39] << 4;
            box[3] = (s8)rec[0x3A] << 4;
            box[5] = (s8)rec[0x3B] << 4;
            if (boxes_overlap_oriented(box, ent, partner, box[4], box[5]))
            {
                ent->state = enemy_run_handler_advance;
                ent->stateNext = enemy_enter_contact_state;
                *slot = link_to_owner_entry;
                ent->handlerNext = link_to_owner_entry;
                return;
            }
        }
    }
    *(void **)(ai + 0x140) = enemy_enter_contact_state;
}

void enemy_run_handler_advance(void *owner, struct Actor *ent)
{
    void **slot = &ent->handler;
    u8 flags;

    ((void (*)(void *, struct Actor *, void **))*slot)(owner, ent, slot);
    flags = ent->flags1D6;
    if (!(flags & 1))
    {
        u32 bit2 = ((u32)flags << 29) >> 31;

        if (((((u32)flags << 28) >> 31) & bit2) == 0)
        {
            return;
        }
    }
    ent->state = ent->stateNext;
}

void enemy_enter_next_state(void *owner, struct Actor *ent)
{
    load_record_fields(ent, 0x10);
    ent->state = ent->stateNext;
    ent->handler = obj_random_target_b;
    ent->handlerNext = obj_random_target_b;
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    obj_set_cell_coord(ent, 4, 0);
    ((void (*)(void *, struct Actor *))ent->state)(owner, ent);
}

void enemy_enter_contact_state(void *owner, struct Actor *ent)
{
    load_record_fields(ent, 0);
    load_hit_box_words(ent, 0);
    ((struct Bits3 *)&ent->mode)->field0 = 3;
    ent->state = enemy_contact_check_state;
    ent->handler = setup_random_offset_state;
    ent->handlerNext = setup_random_offset_state;
    enemy_contact_check_state(owner, ent);
}

void enemy_mark_and_enter_contact(void *owner, struct Actor *ent)
{
    ((struct Flag33E *)&ent->flags33E)->bit4 = 1;
    ((struct Flag2B6 *)&ent->flags2B6)->bit6 = 1;
    enemy_enter_contact_state(owner, ent);
}

void enemy_contact_engage(struct Owner *owner, struct Actor *ent)
{
    u8 idx;
    struct Actor *partner;
    void (*fn)(void *, struct Actor *);

    if (!find_active_entry(owner, &idx))
    {
        return;
    }
    partner = ((struct Actor **)owner)[idx + 10];
    {
        s16 box[8];
        s16 *sensor = ent->sensor;

        s32 k;

        box[0] = ((k = *(s16 *)((u8 *)sensor + 8)) << 4) + 8;
        box[3] = ((k = *(s16 *)((u8 *)sensor + 0xA)) << 4) + 8;
        box[1] = ((k = *(s16 *)((u8 *)sensor + 0xC)) << 4) + 8;
        box[2] = ((k = *(s16 *)((u8 *)sensor + 0xE)) << 4) + 8;
        if (!point_in_box(box, partner, 0x7FFF, 0))
        {
            return;
        }
    }
    load_record_fields_scaled(ent, 0);
    *(u32 *)((u8 *)ent + 0x20C) = (*(u32 *)((u8 *)ent + 0x20C) & 0xFFFC7FFF) | 0x8000;
    {
        s32 t = ent->flags20D;

        ent->flags20D = t | 0x40;
    }
    ent->dz += 1;
    {
        s32 t = ent->flags20E;

        ent->flags20E = t | 4;
    }
    ((struct Flags1D6 *)&ent->flags1D6)->bit1 = 1;
    ((struct Flags1D6 *)&ent->flags1D6)->bit0 = 0;
    ent->state = fn = (void (*)(void *, struct Actor *))obj_state_run_advance;
    ent->stateNext = enemy_setup_wait;
    ent->handler = enemy_end_countdown;
    ent->handlerNext = enemy_end_countdown;
    fn(owner, ent);
}
