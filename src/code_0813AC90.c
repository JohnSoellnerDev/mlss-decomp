#include "global.h"
#include "ram_routines.h"

struct RomBox
{
    s16 left;
    s16 bottom;
    s16 right;
    s16 top;
};

struct Box6
{
    s16 left;
    s16 right;
    s16 top;
    s16 bottom;
    s16 zTop;
    s16 zBottom;
};

struct Obj
{
    u8 filler0[0x2];
    u8 facing;
    u8 filler3[0xC - 0x3];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u8 sector;
    u8 filler25[0x28 - 0x25];
    struct Obj *entA;
    struct Obj *entB;
    u8 filler30[0x1B4 - 0x30];
    struct RomBox *romBox;
    u8 filler1B8[0x1C0 - 0x1B8];
    s32 zSpan;
    u8 filler1C4[0x1CC - 0x1C4];
    s32 moveFrames;
    u8 filler1D0[0x200 - 0x1D0];
    struct Obj *tgtA;
    struct Obj *tgtB;
    u8 filler208[0x214 - 0x208];
    u8 mode;
    u8 filler215;
    u8 unk216;
    u8 filler217[0x23E - 0x217];
    s16 speed;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 zRest;
    u8 filler24C[0x258 - 0x24C];
    s32 zVel;
    u8 filler25C[0x272 - 0x25C];
    u16 unk272;
    u16 unk274;
    u16 unk276;
    u16 unk278;
    u16 unk27A;
    u16 unk27C;
    u8 filler280[0x294 - 0x280];
    u8 *rec;
    u8 filler298[0x2BC - 0x298];
    s32 startX;
    s32 startY;
    s32 zStart;
    s32 zLimit;
    s32 targetX;
    s32 targetY;
    s32 zTarget;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 zSpeed;
    u16 steps;
};

struct State
{
    u8 filler0[0x1A0];
    u32 handler;
    u8 filler1A4[0x1AC - 0x1A4];
    u32 callback;
    u8 filler1B0[0x1D2 - 0x1B0];
    u16 timer;
    u8 filler1D4[0x1D6 - 0x1D4];
    u8 flags;
};

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct Lo3
{
    u8 lo3 : 3;
    u8 rest : 5;
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

void probe_then_move(void);
void route_or_move_ac90(void);
void route_or_move_ad10(void);
void step_move_set_state_4(void);
void step_move_set_state_8(void);
s8 wander_to_edge(struct Obj *obj, u16 steps, s32 dist);
s8 wander_diag(struct Obj *obj, s32 x, s32 y);
u32 angle_of_vector16(s32 x, s32 y);
u8 collide_boxes(struct Obj *obj, struct Obj *other, u16 param3, s8 param4, s8 param5, u16 param6, u16 param7);
void start_move_to(struct Obj *obj, s32 x, s32 y, s32 speed, ByteArg zero);
void random_cell_setup(struct Obj *obj, u32 callback);
u8 point_in_box(s16 *bounds, struct Obj *entry, s32 zMax, s32 zMin);
u32 boxes_overlap(s16 *bounds, struct Obj *a, struct Obj *b, s32 zAbove, s32 zBelow);
u32 boxes_overlap_oriented(s16 *box, struct Obj *a, struct Obj *b, s32 zAbove, s32 zBelow);
void obj_box_to_stack(s16 *out, struct Obj *obj, u16 flags, u16 index);
void load_box_oriented(s16 *out, struct Obj *obj, u32 flags, u32 offset);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u32 tile_blocks_move(struct Obj *obj, s32 x, s32 y, s32 pos, s32 span, u8 dir, u16 *flags);
extern u8 gFrameAttrTable[];
void obj_set_cell_coord(struct Obj *obj, u32 cell, u32 arg);

s8 zone_point_code(struct Obj *obj, struct Obj *zone, s32 margin)
{
    s16 *p = (s16 *)zone->romBox;
    s16 bounds[8];
    s8 result = -1;

    bounds[0] = *p++ * 0x10 - margin;
    bounds[3] = *p++ * 0x10 - margin;
    bounds[1] = *p++ * 0x10 + margin;
    bounds[2] = *p * 0x10 + margin;

    if (point_in_box(bounds, obj->entA, 0x7FFF, 0))
        result = 0;
    if (point_in_box(bounds, obj->entB, 0x7FFF, 0))
    {
        if (result == 0)
            result = 2;
        else
            result = 1;
    }
    return result;
}

s8 zone_overlap_code(struct Obj *obj, struct Obj *zone, s32 margin)
{
    s16 box[8];
    s8 pt = zone_point_code(obj, zone, margin);
    s8 result;

    if (pt == -1)
        return -1;
    if (pt == 0)
    {
        if (zone->z != obj->entA->z)
            return -1;
    }
    else if (pt == 1)
    {
        if (zone->z != obj->entB->z)
            return -1;
    }
    else if (pt == 2)
    {
        if (zone->z != obj->entA->z && zone->z != obj->entB->z)
            return -1;
    }

    result = -1;
    obj_box_to_stack(box, zone, 0x28, 8);
    box[0] = -box[0];
    box[3] = -box[3];
    if ((u8)boxes_overlap(box, zone, obj->entA, 0, 0))
        result = 0;
    if ((u8)boxes_overlap(box, zone, obj->entB, 0, 0))
    {
        if (result == 0)
            result = 2;
        else
            result = 1;
    }
    return result;
}

NONMATCH("asm/code_0813AC90/box_hit_kind.s", u32 box_hit_kind(struct Obj *owner, struct Obj *other, u16 fmt, u16 offset, u16 kind))
{
    s16 box[8];
    u32 slot;

    if (kind == 0)
        return 0;
    load_box_oriented(box, other, fmt, offset);
    switch (kind)
    {
    case 1:
        slot = 0x200;
        break;
    case 2:
        slot = 0x204;
        break;
    case 3:
        if ((u8)boxes_overlap_oriented(box, other, *(struct Obj **)((u8 *)owner + 0x200), box[4], box[5]))
            return 1;
        slot = 0x204;
        break;
    case 4:
        if (!(u8)boxes_overlap_oriented(box, other, *(struct Obj **)((u8 *)owner + 0x200), box[4], box[5]))
            return 0;
        slot = 0x204;
        break;
    default:
        return 1;
    }
    if ((u8)boxes_overlap_oriented(box, other, *(struct Obj **)((u8 *)owner + slot), box[4], box[5]))
        return 1;
    return 0;
}
END_NONMATCH

u8 probe_body_hit(struct Obj *ctx, struct Obj *obj, s32 x, s32 y, s32 mode, u8 flag, u16 *extra)
{
    s32 px;
    u32 hit;

    hit = tile_blocks_move(ctx, x, y, mode, obj->unk27A, flag, extra);
    if ((u8)hit != 0)
        return 1;
    px = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk278, 2);
    px = x - px;
    hit = tile_blocks_move(ctx, px, y - obj->unk27C, mode, obj->unk27A, flag, extra);
    if ((u8)hit != 0)
        return 1;
    px = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk278, 2);
    px = px + x;
    hit = tile_blocks_move(ctx, px, y - obj->unk27C, mode, obj->unk27A, flag, extra);
    if ((u8)hit != 0)
        return 1;
    px = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk278, 2);
    px = x - px;
    hit = tile_blocks_move(ctx, px, y, mode, obj->unk27A, flag, extra);
    if ((u8)hit != 0)
        return 1;
    px = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk278, 2);
    px = px + x;
    hit = tile_blocks_move(ctx, px, y, mode, obj->unk27A, flag, extra);
    return (u8)hit;
}

ASM_FUNC("asm/code_0813AC90/box_overlap_xy.s", void box_overlap_xy(void))

NONMATCH("asm/code_0813AC90/start_move_to.s", void start_move_to(struct Obj *obj, s32 x, s32 y, s32 speed, ByteArg zero))
{
    s32 frames;

    if (zero.value != 0)
    {
        x += obj->x;
        y += obj->y;
    }
    obj->startX = obj->x;
    obj->startY = obj->y;
    obj->targetX = x;
    obj->targetY = y;
    x -= obj->x;
    y -= obj->y;
    x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(x, 0x10);
    y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(y, 0x10);
    obj->zSpan = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(x * x + y * y) << 4;
    frames = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zSpan, speed);
    obj->moveFrames = frames;
    obj->steps = frames;
    if ((u16)frames != 0)
    {
        obj->vx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(x << 4, obj->steps);
        obj->vy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(y << 4, obj->steps);
    }
    else
    {
        obj->vy = 0;
        obj->vx = 0;
    }
}
END_NONMATCH

void start_move_to_b(struct Obj *obj, s32 z, s32 speed, ByteArg rel)
{
    s32 frames;

    if (rel.value != 0)
        z += obj->z + obj->dz;
    obj->zStart = obj->z + obj->dz;
    obj->zTarget = z;
    z -= obj->zStart;
    z = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(z, 0x10);
    obj->zSpan = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(z * z) << 4;
    frames = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zSpan, speed);
    obj->moveFrames = frames;
    obj->steps = frames;
    if ((u16)frames != 0)
        obj->zSpeed = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(z << 4, obj->steps);
    else
        obj->zSpeed = 0;
}

void start_move_to_3d(struct Obj *obj, s32 x, s32 y, s32 z, s32 speed, ByteArg zero)
{
    s32 frames;

    if (zero.value != 0)
    {
        x += obj->x;
        y += obj->y;
        z += obj->z + obj->dz;
    }
    obj->startX = obj->x;
    obj->startY = obj->y;
    obj->zStart = obj->z + obj->dz;
    obj->targetX = x;
    obj->targetY = y;
    obj->zTarget = z;
    x -= obj->x;
    y -= obj->y;
    z -= obj->z + obj->dz;
    x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(x, 0x10);
    y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(y, 0x10);
    z = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(z, 0x10);
    obj->zSpan = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(x * x + y * y + z * z) << 4;
    frames = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zSpan, speed);
    obj->moveFrames = frames;
    obj->steps = frames;
    if ((u16)frames != 0)
    {
        obj->vx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(x << 4, obj->steps);
        obj->vy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(y << 4, obj->steps);
        obj->zSpeed = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(z << 4, obj->steps);
    }
    else
    {
        obj->zSpeed = 0;
        obj->vy = 0;
        obj->vx = 0;
    }
}

ASM_FUNC("asm/code_0813AC90/wander_to_edge.s", s8 wander_to_edge(struct Obj *obj, u16 steps, s32 dist))

ASM_FUNC("asm/code_0813AC90/wander_diag.s", s8 wander_diag(struct Obj *obj, s32 x, s32 y))

ASM_FUNC("asm/code_0813AC90/collide_react.s", void collide_react(void))

NONMATCH("asm/code_0813AC90/collide_wander_diag.s", s8 collide_wander_diag(u32 unused, struct Obj *obj, struct Obj *other, u16 size, u16 speed, s8 dirA, s8 dirB, u16 extA, u16 extB))
{
    u8 code;
    s32 keep;
    u32 facing;
    s32 sector;

    keep = 1;
    code = collide_boxes(obj, other, size, dirA, dirB, extA, extB);
    if (code == 0xFF)
        goto wander;
    if (code != 2)
    {
        if (code != 8)
            goto wander;
    }
    if (code != 1)
    {
        if (code != 4)
            keep = 0;
    }
    if (keep == 0)
        goto wander;
    if (code & 2)
    {
        if (code & 1)
        {
            start_move_to(obj, size, -size, speed, (u8)1);
            code = 0;
            goto done;
        }
        if (code & 4)
        {
            start_move_to(obj, size, size, speed, (u8)1);
            code = 1;
        }
        goto done;
    }
    if ((code & 8) == 0)
        goto done;
    if (code & 1)
    {
        start_move_to(obj, -size, -size, speed, (u8)1);
        code = 3;
        goto done;
    }
    if ((code & 4) == 0)
        goto done;
    start_move_to(obj, -size, size, speed, (u8)1);
    code = 2;
    goto done;

wander:
    code = wander_diag(obj, size, speed);
done:
    if (((u16)obj->vx | (u16)obj->vy) != 0)
        facing = ((u32)(u16)angle_of_vector16(obj->vx, obj->vy) + 0x10) >> 8;
    else
        facing = 0xFF;
    obj->facing = facing;
    if (((u16)obj->vx | (u16)obj->vy) != 0)
        sector = (((u16)angle_of_vector16(obj->vx, obj->vy) + 0x1000) >> 13) & 7;
    else
        sector = -1;
    sector &= 7;
    ((struct Lo3 *)&obj->sector)->lo3 = sector;
    return (s8)code;
}
END_NONMATCH

u32 z_approach(struct Obj *obj)
{
    s32 *lim;
    s32 part;
    s32 root;
    s32 rem;
    u16 *rate;
    s32 *rest;
    s32 dz;
    s32 limit;

    part = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zTarget - obj->zStart, 4);
    if (part != 0)
    {
        root = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(part * part);
        obj->dz += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(part * obj->zSpeed, root);
    }
    rate = (u16 *)&obj->zSpeed;
    rest = &obj->zRest;
    rem = *rest;
    if (rem < 0)
        rem = -rem;
    rem = *rate - rem;
    *rate = rem;
    if (*rest > 0)
    {
        if ((s16)rem > 0)
            goto moving;
        lim = &obj->zLimit;
        dz = obj->dz;
        limit = *lim;
        if (dz > limit)
            goto moving;
    }
    else
    {
        if ((s16)rem > 0)
            goto moving;
        lim = &obj->zLimit;
        dz = obj->dz;
        limit = *lim;
        if (dz < limit)
            goto moving;
    }
    obj->dz = limit;
    return 0;
moving:
    return 1;
}

u8 collide_boxes(struct Obj *obj, struct Obj *other, u16 param3, s8 param4, s8 param5, u16 param6, u16 param7)
{
    s16 box[8];
    s32 xMin;
    s32 xMax;
    s32 yMin;
    s32 yMax;
    s32 half;
    u16 result;
    u8 roll;

    half = param3 * 0x100;
    roll = rand_below(100, mt19937_next());
    obj_box_to_stack(box, obj, param6, param7);
    xMin = box[0] * 0x100;
    xMax = box[1] * 0x100;
    yMin = box[3] * 0x100;
    yMax = box[2] * 0x100;
    result = 0;
    if ((roll << 24) < (param4 << 24))
    {
        if (obj->x >= other->x)
        {
            if (obj->x - half >= xMin)
                result = 8;
        }
        else if (obj->x + half <= xMax)
            result = 2;
    }
    if ((roll << 24) < (param5 << 24))
    {
        if (obj->y >= other->y)
        {
            if (obj->y - half >= yMin)
                result |= 1;
        }
        else if (obj->y + half <= yMax)
            result |= 4;
    }
    if (result == 0)
        return 0xff;
    return result;
}

NONMATCH("asm/code_0813AC90/random_cell_setup.s", void random_cell_setup(struct Obj *obj, u32 callback))
{
    u8 *table;
    u8 *rec;
    u16 valueA;
    s16 valueB;
    u16 valueC;
    s32 idx;
    u8 cell;

    table = (u8 *)obj + 0x100;
    idx = rand_below(2, mt19937_next());
    valueA = ((u16 *)(table + 0x30))[idx];
    idx = rand_below(4, mt19937_next());
    valueB = ((s16 *)table)[idx];
    ((void (*)(struct Obj *, u32, u32))callback)(obj, valueA, valueB);
    idx = rand_below(4, mt19937_next());
    rec = obj->rec;
    valueC = ((u16 *)(table + 8))[idx];
    rec[0x20] = valueC;
    cell = gFrameAttrTable[(((u32)obj->unk216 << 28) >> 30 << 4) + (((u32)obj->sector << 29) >> 29) + 8];
    obj_set_cell_coord(obj, cell, 0);
    ((struct Lo3 *)&obj->mode)->lo3 = 3;
}
END_NONMATCH

u32 box_contains_fixed(struct Box6 *box, struct Obj *a, struct Obj *b)
{
    s32 ax = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->x, 0x100);
    s32 ay = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->y, 0x100);
    s32 az = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->z, 0x100);
    s32 bx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->x, 0x100);
    s32 by = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->y, 0x100);
    s32 bz = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->z, 0x100);

    if (ax - box->left > bx)
        return 0;
    if (box->right + ax < bx)
        return 0;
    if (ay - box->bottom > by)
        return 0;
    if (ay + box->top < by)
        return 0;
    if (az - box->zBottom > bz)
        return 0;
    if (box->zTop + az < bz)
        return 0;
    return 1;
}

u32 distance_xyz(struct Obj *a, struct Obj *b)
{
    s32 dx = a->x - b->x;
    s32 dy = a->y - b->y;
    s32 dz = (a->z + a->dz) - (b->z + b->dz);
    return MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy + dz * dz);
}

u32 distance_xy(struct Obj *a, struct Obj *b)
{
    s32 dx = a->x - b->x;
    s32 dy = a->y - b->y;
    return MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy);
}

void tick_fire(void *ctx, struct State *st, u32 *out)
{
    if (--st->timer == 0)
    {
        ((struct Flags8 *)&st->flags)->bit0 = 1;
        ((struct Flags8 *)&st->flags)->bit1 = 0;
        *out = st->callback;
        ((void (*)(void *, struct State *, u32 *))*out)(ctx, st, out);
    }
}

u8 point_in_box(s16 *bounds, struct Obj *entry, s32 zMax, s32 zMin);
u32 boxes_overlap(s16 *bounds, struct Obj *a, struct Obj *b, s32 zAbove, s32 zBelow);
u32 boxes_overlap_oriented(s16 *box, struct Obj *a, struct Obj *b, s32 zAbove, s32 zBelow);
void obj_box_to_stack(s16 *out, struct Obj *obj, u16 flags, u16 index);
void load_box_oriented(s16 *out, struct Obj *obj, u32 flags, u32 offset);
void tick_store(void *ctx, struct State *st, u32 *out)
{
    u8 *base = (u8 *)st;
    s32 timerOff = 0x1D2;
    s32 flagsOff = 0x1D6;
    s32 cbOff = 0x1AC;
    if (--*(u16 *)(base + timerOff) == 0)
    {
        ((struct Flags8 *)(base + flagsOff))->bit0 = 1;
        ((struct Flags8 *)(base + flagsOff))->bit1 = 0;
        *out = *(u32 *)(base + cbOff);
    }
}

u32 angle_of_vector16(s32 x, s32 y);
void start_move_to(struct Obj *obj, s32 x, s32 y, s32 speed, ByteArg zero);
void box_contacts_aim_at_target(struct Obj *obj, struct Obj *state, u32 *out)
{
    u8 *behavior = (u8 *)state + 0x58;
    struct Obj *target = obj->tgtA;
    s32 dx = target->x - state->x;
    s32 dy = target->y - state->y;
    u32 facing;

    if ((dx | dy) != 0)
        facing = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    else
        facing = 0xFF;
    state->facing = facing;
    start_move_to(state, target->x, target->y, state->speed, (u8)0);
    *(s32 *)(behavior + 0x184) = state->x;
    *(s32 *)(behavior + 0x188) = state->y;
    ((struct Lo3 *)&state->mode)->lo3 = 3;
    *out = (u32)step_move_set_state_8;
}

void start_wander(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)probe_then_move;
}

void start_wander_edge_c888(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)route_or_move_ad10;
}

void start_wander_diag_c8f8(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_diag);
    *out = (u32)route_or_move_ac90;
}

void start_wander_edge_c8f8(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)route_or_move_ac90;
}

void start_wander_diag_b(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_diag);
    *out = (u32)step_move_set_state_4;
}

void start_wander_b(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)step_move_set_state_4;
}

void run_state_handler(struct Obj *obj, struct State *state)
{
    u32 *slot = &state->handler;
    ((void (*)(struct Obj *, struct State *, u32 *))*slot)(obj, state, slot);
}

void start_z_move(struct Obj *obj, s32 z, u16 speed, u8 rel, s32 zLimit)
{
    s32 d;
    s32 raw;

    if (rel != 0)
        z += obj->z + obj->dz;
    obj->zTarget = z;
    obj->zStart = obj->z + obj->dz;
    obj->zLimit = zLimit;
    obj->zSpan = obj->zTarget - obj->zStart;
    obj->zSpeed = speed;
    d = speed + 2 * obj->zSpan;
    raw = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(-speed * speed + ((d + (s32)((u32)d >> 31)) >> 1), d);
    obj->zRest = -raw;
    if (obj->zRest == 0)
        obj->zRest = 1;
}

u32 accum_step_check(struct Obj *obj)
{
    u32 moving;
    obj->dz += obj->zVel;
    if (obj->dz > 0)
    {
        obj->zVel -= obj->zRest;
        moving = 1;
    }
    else
        moving = 0;
    return moving;
}

void set_z_motion(struct Obj *obj, s32 zVel, s32 zRest)
{
    obj->zVel = zVel;
    obj->zRest = zRest;
}

u32 sub_0813C27C(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xy(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xy_b(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xy_c(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xyz(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->x += obj->vx;
    obj->y += obj->vy;
    obj->dz += obj->zSpeed;
    obj->steps--;
    return obj->steps != 0;
}

u32 pos_step_countdown_running(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->dz += obj->zSpeed;
    obj->steps--;
    return obj->steps != 0;
}

u32 step_pos_xy_plain(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->x += obj->vx;
    obj->y += obj->vy;
    obj->steps--;
    return obj->steps != 0;
}
