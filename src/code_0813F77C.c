#include "global.h"
#include "ram_routines.h"

struct Enemy;

struct Owner
{
    u8 filler0[0x28];
    struct Enemy *party[4];
    u8 filler38[0x208 - 0x38];
    u8 flags208;
};

struct DirBits
{
    u8 dir : 3;
    u8 rest : 5;
};

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

struct Bit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct Bit3
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

struct Bit6
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct Bit5
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct Bit2
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 hi5 : 5;
};

typedef void (*EnemyFn)(void);

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct Sprite
{
    u8 filler0[0x12];
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
};

struct Enemy
{
    u8 filler0[2];
    u8 angle;
    u8 filler3[0xC - 3];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x54 - 0x25];
    u8 unk54;
    u8 filler55[0x198 - 0x55];
    EnemyFn state;
    EnemyFn unk19C;
    EnemyFn handler;
    EnemyFn nextHandler;
    u8 filler1A8[0x1B0 - 0x1A8];
    u8 *rec;
    u16 *sensor;
    u8 filler1B8[0x1D0 - 0x1B8];
    u16 index;
    u16 timerB;
    u16 timer;
    u8 unk1D6;
    u8 filler1D7[0x1D8 - 0x1D7];
    u16 unk1D8;
    u8 filler1DA[0x1DC - 0x1DA];
    s32 homeX;
    s32 homeY;
    u8 filler1E4[0x20C - 0x1E4];
    u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 filler210[0x214 - 0x210];
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x242 - 0x217];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u8 filler24C[0x290 - 0x24C];
    u8 *map;
    struct Sprite *sprite;
    u8 filler298[0x2D4 - 0x298];
    u32 tgtZ;
    u8 filler2D8[0x2DC - 0x2D8];
    u16 zspeed;
    u16 frames;
    u8 filler2E0[0x33E - 0x2E0];
    u8 unk33E;
};

u32 mt19937_next(void);
s32 rand_below(s32 n, u32 value);
void obj_set_sprite_link(struct Enemy *obj, u32 value);
void obj_set_active(struct Enemy *obj, u32 value);
void obj_set_cell_coord(struct Enemy *obj, u32 cell, u32 arg);
void enemy_sibling_noop_state(void);
void enemy_run_move(void);
void enemy_hop_fall_then_next(void);
void enemy_after_fall_check_box(void);
void enemy_fall_step(void);
void enemy_wait_after_step_c(void);
void enemy_approach_partner(void);
void enemy_run_until_contact(void);
void collide_react(struct Owner *owner, struct Enemy *ent, struct Enemy *bro, u32 speed, u32 a, s32 b, s32 c, u32 d, u32 e);
void start_z_move(struct Enemy *obj, s32 z, u16 speed, u8 rel, s32 zLimit);
void start_move_to(struct Enemy *ent, s32 x, s32 y, u32 speed, ByteArg zero);
u16 angle_of_vector16(s32 x, s32 y);
u8 *obj_probe_tile_heights(struct Enemy *obj, s32 x, s32 y, s32 *h0, s32 *h1, s32 *h2);
void start_move_to_3d(struct Enemy *obj, s32 x, s32 y, s32 z, u16 speed, ByteArg flag);
extern const u8 gFrameAttrTable[];

u8 find_active_entry(struct Owner *owner, u8 *idx);
void obj_box_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index);
u8 point_in_box(s16 *bounds, struct Enemy *obj, s32 zMaxIn, s32 zMinIn);
void load_box_oriented(s16 *out, struct Enemy *obj, u32 flags, u32 offset);
u8 boxes_overlap_oriented(s16 *box, struct Enemy *a, struct Enemy *b, s32 zAbove, s32 zBelow);

ASM_FUNC("asm/code_0813F77C/enemy_hop_land_on_partner.s", void enemy_hop_land_on_partner(void))

ASM_FUNC("asm/code_0813F77C/enemy_hop_land_next.s", void enemy_hop_land_next(void))

void enemy_hop_prepare_fall(u32 r0, struct Enemy *ent, EnemyFn *out)
{
    u8 *rec;
    struct Sprite *sprite;
    u32 cell;
    s32 idx;

    rec = ent->rec;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    cell = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 4;
    obj_set_cell_coord(ent, cell, 0);
    ((struct Bit3 *)&ent->unk33E)->bit3 = 1;
    sprite = ent->sprite;
    sprite->unk20 = rec[ent->index + 0x44];
    idx = rand_below(4, mt19937_next());
    ent->timerB = ent->rec[idx + 0x48];
    ent->zspeed = *(u16 *)(ent->rec + 0x4C);
    ent->accel = ent->rec[0x54];
    *out = enemy_hop_fall_then_next;
}

void enemy_hop_fall_to_tile(u32 r0, struct Enemy *ent, EnemyFn *out)
{
    u8 zero = 0;
    s32 h0;
    s32 h1;
    s32 h2;
    s32 idx;
    u16 speed;
    u32 cell;

    obj_probe_tile_heights(ent, ent->x / 0x1000, ent->y / 0x1000, &h0, &h1, &h2);
    if (h2 != -1)
    {
        h0 = h2;
    }
    idx = rand_below(4, mt19937_next());
    speed = ((u16 *)(ent->rec + 0x58))[idx];
    start_move_to_3d(ent, ent->x, ent->y, h0 + 0x3000, speed, zero);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    cell = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(ent, cell, 0);
    *out = enemy_after_fall_check_box;
}

void enemy_hop_return_home(u32 r0, struct Enemy *ent, EnemyFn *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *rec;
    s32 dx;
    s32 dy;
    u32 facing;
    s32 idx;
    u16 speed;
    u8 zero;
    u32 cell;

    rec = ent->rec;
    dx = ent->homeX - ent->x;
    dy = ent->homeY - ent->y;
    if ((dx | dy) != 0)
    {
        facing = ((u32)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        facing = 0xFF;
    }
    zero = 0;
    ent->angle = facing;
    idx = rand_below(4, mt19937_next());
    speed = ((u16 *)(*(u8 **)(ai + 0x158) + 0x64))[idx];
    start_move_to(ent, *(s32 *)(ai + 0x184), *(s32 *)(ai + 0x188), speed, zero);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    cell = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(ent, cell, 0);
    ent->sprite->unk20 = rec[*(u16 *)(ai + 0x178) + 0x60];
    *out = enemy_wait_after_step_c;
}

u32 enemy_partner_in_sensor(struct Owner *owner, struct Enemy *ent)
{
    u8 idx;
    s16 box[8];
    struct Enemy *partner;

    if (find_active_entry(owner, &idx) == 0)
    {
        return 0;
    }
    partner = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
    if (ent->sensor[4] != 0)
    {
        obj_box_to_stack(box, ent, 0x12, 0);
        if (point_in_box(box, partner, 0x7FFF, 0) != 0)
        {
            return 0;
        }
    }
    load_box_oriented(box, ent, 0x11, 0x1C);
    if (boxes_overlap_oriented(box, ent, partner, box[4], box[5]) != 0)
    {
        return 1;
    }
    return 0;
}

u32 enemy_edge_limit_check(u32 r0, struct Enemy *ent, s32 a2, s32 a3, s32 z)
{
    u32 dir = ((u32)ent->unk24 << 29) >> 29;

    switch (dir)
    {
    case 2:
        if (a2 != 0 || a3 != 0 || z != 0x7FFFFFFF)
        {
            return 1;
        }
    ret0:
        return 0;
    case 0:
    case 4:
        goto common;
    case 6:
        if (a2 == 0x7FFFFFFF && a3 == -1 && z == a3)
        {
            goto ret0;
        }
        goto common;
    default:
        return 1;
    }
common:
    if (a2 == 0 && a3 == 0 && z == 0x7FFFFFFF)
    {
        goto ret0;
    }
    if (a2 != 0x2000 || a3 != -1 || z != a3)
    {
        return 1;
    }
    if (ent->x <= 0x5000)
    {
        goto ret0;
    }
    return 1;
}

NONMATCH("asm/code_0813F77C/enemy_hop_face_partner.s", void enemy_hop_face_partner(struct Owner *owner, struct Enemy *ent, EnemyFn *out))
{
    u8 found;
    s32 idx;
    u8 *rec;
    struct Enemy *partner;
    s32 (*div)(s32, s32);
    s32 dx;
    s32 dy;
    s32 dir;
    u8 cell;

    rec = ent->rec;
    if (find_active_entry(owner, &found) != 0)
    {
        idx = found;
    }
    else
    {
        idx = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    dx = partner->x - ent->x;
    dy = partner->y - ent->y;
    if ((dx | dy) != 0)
    {
        dir = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
    }
    else
    {
        dir = -1;
    }
    cell = div(dir, 2);
    obj_set_cell_coord(ent, cell, 0);
    ent->tgtZ = ent->z + ent->dz + (*(u16 *)(rec + 0x24) << 12);
    ent->sprite->unk20 = 0x10;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    *out = enemy_fall_step;
}
END_NONMATCH

void enemy_group_sibling_setup(struct Owner *owner, struct Enemy *ent)
{
    struct Enemy *sibling;
    u16 *sensor;
    u32 group;
    s32 span;

    ((struct Bit4 *)&ent->unk33E)->bit4 = 0;
    sensor = ent->sensor;
    group = ent->unk54;
    span = (s8)((u8 *)sensor)[0xA];
    sibling = *(struct Enemy **)((span << 2) + ((group << 2) + (u32)owner) + 0x28);
    if ((sibling->unk20C & 1) == 0)
    {
        obj_set_sprite_link(ent, 0);
        obj_set_active(ent, 0);
    }
    else
    {
        ((struct Bit2 *)&ent->unk20F)->bit2 = 1;
        obj_set_sprite_link(ent, 0);
        ((struct Bit2 *)&ent->unk20D)->bit2 = 0;
        ent->state = enemy_sibling_noop_state;
        ent->nextHandler = enemy_run_move;
    }
}

ASM_FUNC("asm/code_0813F77C/enemy_run_move.s", void enemy_run_move(void))

ASM_FUNC("asm/code_0813F77C/enemy_kick_after_probe.s", void enemy_kick_after_probe(void))


ASM_FUNC("asm/code_0813F77C/enemy_run_until_contact.s", void enemy_run_until_contact(void))

NONMATCH("asm/code_0813F77C/enemy_start_approach.s", void enemy_start_approach(struct Owner *owner, struct Enemy *ent, EnemyFn *out))
{
    u8 found;
    u32 pidx;
    u8 *entry;
    u16 speed;
    s32 z;
    struct Enemy *partner;
    s32 dx;
    s32 dy;
    s32 dir;
    s32 cell;

    ((struct Bit4 *)&ent->unk1D6)->bit4 = 0;
    ((struct Bit6 *)&ent->unk20D)->bit6 = 0;
    ((struct Bit2 *)&ent->unk20F)->bit2 = 1;
    ent->index = rand_below(4, mt19937_next());
    entry = (u8 *)ent->rec + ent->index * 2;
    speed = *(u16 *)(entry + 0x2A);
    z = ent->z + ent->dz + *(u16 *)(entry + 0x22);
    start_z_move(ent, z, speed, 0, 0);
    if (find_active_entry(owner, &found) != 0)
    {
        pidx = found;
    }
    else
    {
        pidx = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = *(struct Enemy **)((u8 *)owner + pidx * 4 + 0x28);
    dx = partner->x - ent->x;
    dy = partner->y - ent->y;
    if ((dx | dy) != 0)
    {
        dir = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
        cell = dir >> 1;
    }
    else
    {
        cell = 0;
    }
    obj_set_cell_coord(ent, cell, 7);
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    *out = enemy_approach_partner;
}
END_NONMATCH
