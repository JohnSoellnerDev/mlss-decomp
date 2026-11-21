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
    u8 unk54;           // its own index in Owner.party
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
    u8 unk20C;          // bit 0: active (obj_set_active)
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;          // bit 1: sprite linked (obj_set_sprite_link)
    u8 filler210[0x214 - 0x210];
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x242 - 0x217];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u8 filler24C[0x272 - 0x24C];
    u16 width;
    u16 unk274;
    u16 height;
    u8 filler278[0x290 - 0x278];
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
void enemy_run_move(struct Owner *owner, struct Enemy *ent, EnemyFn *out);
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
void enemy_kick_after_probe(struct Owner *owner, struct Enemy *ent, EnemyFn *out);
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);
u8 z_approach(struct Enemy *ent);
u8 step_pos_xy_plain(struct Enemy *ent);
void load_box_oriented(s16 *out, struct Enemy *obj, u32 flags, u32 offset);
u8 boxes_overlap_oriented(s16 *box, struct Enemy *a, struct Enemy *b, s32 zAbove, s32 zBelow);


ASM_FUNC("asm/code_0813F77C/enemy_hop_land_on_partner.s", void enemy_hop_land_on_partner(void))

ASM_FUNC("asm/code_0813F77C/enemy_hop_land_next.s", void enemy_hop_land_next(void))

// Starts the enemy's fall after a hop: movement mode 1 with its falling cell (the movement cell
// + 4), the falling flag (0x33E bit 3), the record's sprite value for its index (rec + 0x44), a
// fall time picked at random from the record's 4 (rec + 0x48), the record's starting fall speed
// (rec + 0x4C) and acceleration (rec + 0x54); then falls in enemy_hop_fall_then_next.
void enemy_hop_prepare_fall(u32 owner, struct Enemy *ent, EnemyFn *out)
{
    u8 *rec;
    struct Sprite *sprite;
    u32 cell;
    s32 pick;

    rec = ent->rec;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    cell = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 4;
    obj_set_cell_coord(ent, cell, 0);
    ((struct Bit3 *)&ent->unk33E)->bit3 = 1;
    sprite = ent->sprite;
    sprite->unk20 = rec[ent->index + 0x44];
    pick = rand_below(4, mt19937_next());
    ent->timerB = ent->rec[pick + 0x48];
    ent->zspeed = *(u16 *)(ent->rec + 0x4C);
    ent->accel = ent->rec[0x54];
    *out = enemy_hop_fall_then_next;
}

// Starts the enemy moving straight up or down to 0x3000 above the floor of the tile it is on (the
// tile's upper floor when it has one), at one of the record's 4 speeds (rec + 0x58, picked at
// random), with movement mode 4 and its movement cell; then goes on to enemy_after_fall_check_box.
void enemy_hop_fall_to_tile(u32 r0, struct Enemy *ent, EnemyFn *out)
{
    u8 zero = 0;
    s32 floorH;
    s32 layerTop;
    s32 upperFloorH;
    s32 pick;
    u16 speed;
    u32 cell;

    obj_probe_tile_heights(ent, ent->x / 0x1000, ent->y / 0x1000, &floorH, &layerTop, &upperFloorH);
    if (upperFloorH != -1)
    {
        floorH = upperFloorH;
    }
    pick = rand_below(4, mt19937_next());
    speed = ((u16 *)(ent->rec + 0x58))[pick];
    start_move_to_3d(ent, ent->x, ent->y, floorH + 0x3000, speed, zero);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    cell = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(ent, cell, 0);
    *out = enemy_after_fall_check_box;
}

// Turns the enemy toward its home (0xFF when it is already there) and starts moving it home at
// one of the record's 4 speeds (rec + 0x64, picked at random), with movement mode 4, its
// movement cell and the record's sprite value for its index; then waits in enemy_wait_after_step_c.
void enemy_hop_return_home(u32 r0, struct Enemy *ent, EnemyFn *out)
{
    // rec, index and homeX/homeY read through ent + 0x58, as the original does.
    u8 *base58 = (u8 *)ent + 0x58;
    u8 *rec;
    s32 dx;
    s32 dy;
    u32 facing;
    s32 pick;
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
    pick = rand_below(4, mt19937_next());
    speed = ((u16 *)(*(u8 **)(base58 + 0x158) + 0x64))[pick];
    start_move_to(ent, *(s32 *)(base58 + 0x184), *(s32 *)(base58 + 0x188), speed, zero);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    cell = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(ent, cell, 0);
    ent->sprite->unk20 = rec[*(u16 *)(base58 + 0x178) + 0x60];
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

// Starts a hop toward the partner: picks the active party member (or, if none is active, the one
// flags208 bit 1 selects), turns the enemy's cell to the quarter of the 8-way direction toward it,
// sets the rise target to the record's hop height (rec + 0x24, whole units) above the current
// height, and hands over to enemy_fall_step with movement mode 1.
void enemy_hop_face_partner(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    u8 *rec = ent->rec;
    u8 activeIdx;
    struct Enemy *partner;
    s32 (*divide)(s32, s32);
    s32 dx;
    s32 dy;
    s32 dir8;

    // The party slots are indexed by byte offset, as the original does.
    if (find_active_entry(owner, &activeIdx) != 0)
        partner = *(struct Enemy **)((u8 *)owner + activeIdx * 4 + 0x28);
    else
        partner = *(struct Enemy **)((u8 *)owner + (((u32)owner->flags208 << 30) >> 31) * 4 + 0x28);
    divide = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    dx = partner->x - ent->x;
    dy = partner->y - ent->y;
    if (dx | dy)
        dir8 = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
    else
        dir8 = -1;
    obj_set_cell_coord(ent, (u8)divide(dir8, 2), 0);
    ent->tgtZ = ent->z + ent->dz + (*(u16 *)(rec + 0x24) << 12);
    ent->sprite->unk20 = 0x10;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    *out = enemy_fall_step;
}

// Sets an enemy up to jump off its sibling, the party member its sensor names (signed byte 0xA:
// an offset from the enemy's own party index). It clears 0x33E bit 4 and unlinks its sprite. With
// the sibling inactive the enemy is switched off too; otherwise it sets 0x20F bit 2, clears 0x20D
// bit 2 and waits (enemy_sibling_noop_state) to run its jump in enemy_run_move.
void enemy_group_sibling_setup(struct Owner *owner, struct Enemy *ent)
{
    struct Enemy *sibling;
    u16 *sensor;
    u32 selfIdx;
    s32 siblingOffset;

    ((struct Bit4 *)&ent->unk33E)->bit4 = 0;
    sensor = ent->sensor;
    selfIdx = ent->unk54;
    siblingOffset = (s8)((u8 *)sensor)[0xA];
    // owner->party[selfIdx + siblingOffset]
    sibling = *(struct Enemy **)((siblingOffset << 2) + ((selfIdx << 2) + (u32)owner) + 0x28);
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
        ent->nextHandler = (EnemyFn)enemy_run_move;
    }
}

// Run step of an enemy jumping off its sibling. While it still has frames to run, a step that
// would carry it into one of its sensor's blocking boxes (sensor byte 0xB of them) is taken
// back and the run stops. The run also stops on a blocking tile (flag 0x80) at its leading
// corner. It then moves in X/Y and in Z (z_approach); once it has landed, it goes back to
// cell 0 and enemy_kick_after_probe picks what comes next.
void enemy_run_move(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    ByteArg zero = { 0 };
    ByteArg one = { 1 };
    s16 box[8];
    u8 k;
    u16 *sensor;
    s32 tileA;
    s32 tileB;
    s32 tileC;
    s32 nextX;
    s32 nextY;
    s32 (*div)(s32, s32);
    s32 halfWidth;
    s32 cornerDx;
    s32 cornerDy;
    s32 cornerX;
    s32 cornerY;
    u8 *tile;
    u8 stillMoving;
    u16 vxBits;

    sensor = ent->sensor;
    nextX = ent->x + ent->vx;
    nextY = ent->y + ent->vy;
    if (ent->frames != 0)
    {
        // Box k: [0] left, [1] right, [2] bottom, [3] top, in whole units.
        for (k = 0; k < ((u8 *)sensor)[0xB]; k++)
        {
            obj_box_to_stack(box, ent, 0x22, (((u32)k << 19) + 0xC0000) >> 16);
            if (!(ent->x >= (box[0] << 8) && ent->x <= (box[1] << 8) && ent->y >= (box[3] << 8) && ent->y <= (box[2] << 8)))
            {
                if (nextX >= (box[0] << 8) && nextX <= (box[1] << 8) && nextY >= (box[3] << 8) && nextY <= (box[2] << 8))
                {
                    ent->x -= ent->vx;
                    ent->y -= ent->vy;
                    ent->frames = 0;
                }
            }
        }
    }

    // The corner of the collision box that leads in the direction of motion.
    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    halfWidth = div(ent->width, 2);
    if (ent->vx <= 0)
    {
        cornerDx = 0;
        if (ent->vx < 0)
        {
            cornerDx = -halfWidth;
        }
    }
    else
    {
        cornerDx = halfWidth;
    }
    if (ent->vy <= 0)
    {
        cornerDy = -ent->height;
    }
    else
    {
        cornerDy = 0;
    }
    cornerX = ent->x + ent->vx + cornerDx;
    cornerY = ent->y + ent->vy + cornerDy;
    vxBits = *(u16 *)&ent->vx;
    if ((vxBits | *(u16 *)&ent->vy) != 0)
    {
        tile = tile_query(ent->map, cornerX, cornerY, &tileA, &tileB, &tileC, zero, one);
        if ((*tile & 0x80) != 0)
        {
            ent->vx = 0;
            ent->vy = 0;
        }
    }
    step_pos_xy_plain(ent);
    stillMoving = z_approach(ent);
    if (stillMoving == 0)
    {
        ((struct Bit6 *)&ent->unk20D)->bit6 = 1;
        ((struct Bit2 *)&ent->unk20E)->bit2 = 0;
        obj_set_cell_coord(ent, 0, 0);
        enemy_kick_after_probe(owner, ent, out);
    }
}

NONMATCH("asm/code_0813F77C/enemy_kick_after_probe.s", void enemy_kick_after_probe(struct Owner *owner, struct Enemy *ent, EnemyFn *out))
{
    u8 found;
    s32 h0;
    s32 h1;
    s32 h2;
    u8 *tile;
    u8 *table;
    struct Enemy *partner;
    u32 idx;
    u16 strength;
    u16 offset;

    table = (u8 *)ent + 0x100;
    tile = obj_probe_tile_heights(ent, ent->x / 0x1000, ent->y / 0x1000, &h0, &h1, &h2);
    if ((*tile & 0x80) != 0)
    {
        ((struct Bit5 *)&ent->unk1D6)->bit5 = 1;
        return;
    }
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    if (find_active_entry(owner, &found) != 0)
    {
        idx = found;
    }
    else
    {
        idx = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
    strength = ((u16 *)table)[rand_below(4, mt19937_next())];
    offset = ((u16 *)table)[rand_below(2, mt19937_next()) + 0x18];
    collide_react(owner, ent, partner, offset, strength, (s8)table[0x40], (s8)table[0x41], 0x22, 0);
    *out = enemy_run_until_contact;
}
END_NONMATCH



ASM_FUNC("asm/code_0813F77C/enemy_run_until_contact.s", void enemy_run_until_contact(void))

// Starts an enemy's approach on its partner: picks one of the record's four rise heights and
// speeds at random and starts the Z move, turns to face the active party member (or the leader
// when none is active), and hands over to enemy_approach_partner.
void enemy_start_approach(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    u8 found;
    u16 choice;
    u8 *rec;
    u32 partnerIdx;
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
    choice = ent->index;
    rec = ent->rec;
    // rec + 0x22: u16 rise heights[4], rec + 0x2A: u16 rise speeds[4].
    entry = (u8 *)((u32)(choice * 2) + (u32)rec);
    speed = *(u16 *)(entry + 0x2A);
    z = ent->z + ent->dz + *(u16 *)(entry + 0x22);
    start_z_move(ent, z, speed, 0, 0);
    if (find_active_entry(owner, &found) != 0)
    {
        partnerIdx = found;
    }
    else
    {
        partnerIdx = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = *(struct Enemy **)((u8 *)owner + partnerIdx * 4 + 0x28);
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
