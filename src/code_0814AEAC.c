#include "global.h"
#include "ram_routines.h"

struct Cell
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    s8 unk20;
    u8 filler21[0x23 - 0x21];
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    s16 unk2A;
    s16 unk2C;
};

struct Sensor
{
    u8 filler0[8];
    u16 unk8;
};

struct Obj
{
    u8 filler0[0xC];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u32 flags24;
    struct Obj *entries[3];
    s32 unk34;
    u8 filler38[0x198 - 0x38];
    u32 altHandler;
    u8 filler19C[0x1B0 - 0x19C];
    u8 *boxTab;
    struct Sensor *sensor;
    u8 filler1B8[0x1C0 - 0x1B8];
    s32 zScale;
    u8 filler1C4[0x1D0 - 0x1C4];
    u16 index;
    u16 timer;
    u16 phase;
    u8 flags1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x200 - 0x1EC];
    u32 tgtA;
    u8 filler204[0x208 - 0x204];
    u8 flags208;
    u8 filler209[0x20C - 0x209];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 filler210[0x214 - 0x210];
    u8 mode;
    u8 filler215[0x23E - 0x215];
    u16 speed2;
    u8 filler240[0x242 - 0x240];
    u16 vx;
    u8 filler244[0x248 - 0x244];
    u32 stepLen;
    u8 filler24C[0x294 - 0x24C];
    struct Cell *cell;
    u8 filler298[0x2C4 - 0x298];
    s32 zNext;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 unk2CC;
    u8 filler2D0[0x2D4 - 0x2D0];
    s32 zNextAdj;
    u8 filler2D8[0x2DC - 0x2D8];
    u16 zspeed;
    u16 counter2DE;
    u8 filler2E0[0x33E - 0x2E0];
    u8 flags33E;
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

struct ModeBits
{
    u8 low3 : 3;
    u8 rest : 5;
};

struct Bit2Byte
{
    u8 low2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

typedef void (*StateFn)(struct Obj *parent, struct Obj *obj, u32 *slot);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

u8 step_pos_xy_plain(struct Obj *obj);
u8 step_pos_xyz(struct Obj *obj);
void obj_set_sprite_link(struct Obj *obj, u32 value);
void obj_set_or_add_pos(struct Obj *obj, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_box_mirror_to_stack(s16 *dst, struct Obj *other, u32 a, u32 b);
u32 boxes_overlap(s16 *bounds, struct Obj *a, struct Obj *b, s32 zAbove, s32 zBelow);
void obj_box_to_stack(s16 *box, struct Obj *obj, u32 extent, u32 zero);
u32 point_in_box(s16 *box, struct Obj *obj, u32 limit, u32 zero);
void enemy_box_reset(struct Obj *parent, struct Obj *obj, u32 *slot);
u32 target_in_box(struct Obj *obj, struct Obj *other);
void enemy_tile_move(void);
void hit_active_entry(void);
void set_state(struct Obj *parent, struct Obj *obj, u8 state);
void set_state_timers(struct Obj *parent, struct Obj *obj, u8 state);
void behavior_set_obj_state(struct Obj *parent, struct Obj *obj, u8 state);
void state_change_hook(struct Obj *parent, struct Obj *obj, u8 state);
u32 position_blocked(struct Obj *parent, struct Obj *obj, u32 x, u32 y, u32 z);
void reset_record_fields(struct Obj *obj);
u32 mt19937_next(void);
u8 step_pos_xy_c(struct Obj *obj);
void wander_to_edge(struct Obj *obj, u16 a, s16 b);
void tick_store(void);
s32 rand_below(s32 range, u32 seed);
void run_state_handler(void);
void enemy_hop_phase(struct Obj *parent, struct Obj *obj, u32 *slot);
void set_idle_slot(struct Obj *parent, struct Obj *obj, u32 *slot);
void reset_to_home(struct Obj *parent, struct Obj *obj, u32 *slot);
void enemy_pose_move(struct Obj *parent, struct Obj *obj, u32 *slot);
void behavior_fall_step(struct Obj *parent, struct Obj *obj, u32 *slot);

u8 find_active_entry(struct Obj *owner, u8 *idx);
NONMATCH("asm/code_0814AEAC/enemy_box_reset.s", void enemy_box_reset(struct Obj *parent, struct Obj *obj, u32 *slot))
{
    s16 box[8];
    u8 idx;
    u8 *ai;
    u8 *tab;
    struct Sensor *sensor;
    struct Obj *partner;
    u32 ownerIdx;
    u16 timerIdx;

    ai = (u8 *)obj + 0x58;
    tab = obj->boxTab;
    sensor = obj->sensor;
    if (find_active_entry(parent, &idx) != 0)
    {
        ownerIdx = idx;
    }
    else
    {
        ownerIdx = ((u32)parent->flags208 << 30) >> 31;
    }
    partner = *(struct Obj **)((u8 *)parent + ownerIdx * 4 + 0x28);
    obj->zNext = obj->z + obj->dz;
    obj->zNextAdj = partner->z + (sensor->unk8 << 8);
    timerIdx = *(u16 *)(ai + 0x178);
    obj->zspeed = *(u16 *)(tab + timerIdx * 2 + 0x2C);
    ((struct ModeBits *)&obj->mode)->low3 = 4;
    slot[0] = (u32)behavior_fall_step;
    obj_box_to_stack(box, obj, 0x12, 0);
    if ((u8)point_in_box(box, obj, 0x7FFF, 0) != 0)
    {
        slot[1] = (u32)enemy_tile_move;
    }
    else
    {
        slot[1] = (u32)enemy_pose_move;
    }
}
END_NONMATCH

void enemy_axis_limit_step(struct Obj *parent, struct Obj *obj, u32 *slot);
void start_drop_slot(struct Obj *parent, struct Obj *obj, u32 *slot);
void start_move_to_3d(struct Obj *obj, s32 x, s32 y, s32 z, u16 speed, u8 flag);
ASM_FUNC("asm/code_0814AEAC/enemy_pose_move.s", void enemy_pose_move(struct Obj *parent, struct Obj *obj, u32 *slot))
void start_vertical_move(struct Obj *parent, struct Obj *obj, u32 *slot);
void enemy_axis_limit_step(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    s16 box[8];
    s16 sVar1;
    s16 sVar2;
    u8 cVar5;
    s16 sVar6;
    s16 sVar7;
    u16 uVar9;
    s32 iVar8;

    iVar8 = obj->homeX;
    if (iVar8 < 0)
    {
        iVar8 += 0xff;
    }
    sVar2 = (u32)iVar8 >> 8;
    iVar8 = obj->homeY;
    if (iVar8 < 0)
    {
        iVar8 += 0xff;
    }
    sVar1 = (u32)iVar8 >> 8;
    sVar6 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->x, 0x100);
    sVar7 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->y, 0x100);
    uVar9 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(*(u16 *)((u8 *)obj + 0x278), 0x200);
    if (obj->cell->unk23 == 2)
    {
        *(s8 *)&obj->mode = (*(s8 *)&obj->mode & -8) | 5;
    }
    cVar5 = step_pos_xyz(obj);
    box[0] = sVar2 - 0x19;
    box[3] = sVar1 - 0x13;
    box[1] = sVar2 + 0x19;
    box[2] = sVar1 + 0x13;
    if (slot[1] != (u32)start_vertical_move
        && sVar6 + uVar9 >= box[0]
        && sVar6 - uVar9 <= box[1]
        && sVar7 >= box[3]
        && sVar7 - MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(*(u16 *)((u8 *)obj + 0x27C), 0x100) <= box[2])
    {
        slot[1] = (u32)start_vertical_move;
    }
    if (cVar5 == 0)
    {
        slot[0] = slot[1];
    }
}
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_step_e6a0(void *spr);
void enemy_hop_phase(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    u8 *tab;
    struct Cell *cell;
    StateFn fn;
    s32 dz;

    tab = obj->boxTab;
    if (obj->phase != 0)
    {
        dz = obj->dz - (s16)obj->zspeed;
        obj->dz = dz;
        if (obj->z + dz <= obj->zNextAdj)
        {
            *(s8 *)&obj->mode = (*(s8 *)&obj->mode & -8) | 1;
            cell = obj->cell;
            *(s8 *)&cell->unk12 = (*(s8 *)&cell->unk12 & -7) | 2;
            obj_load_record(obj->cell, 2, -1, 0, 0);
            sprite_step_e6a0(obj->cell);
            obj->cell->unk20 = tab[obj->index + 0x4C];
            ((struct Flags8 *)&obj->flags20C)->bit7 = 0;
            ((struct Flags8 *)&obj->flags20C)->bit6 = 0;
            ((struct Flags8 *)&obj->flags20C)->bit5 = 0;
            obj->phase = 0;
        }
    }
    else
    {
        if (obj->cell->unk2C > 0x84F)
        {
            *(s8 *)&obj->mode &= -8;
            fn = set_idle_slot;
            slot[0] = (u32)fn;
            slot[1] = (u32)reset_to_home;
            fn(parent, obj, slot);
        }
    }
}

u32 angle_of_vector16(s32 x, s32 y);
s32 zone_point_code(void *parent, struct Obj *obj, s32 zero);
void start_move_to(struct Obj *obj, s32 x, s32 y, s32 speed, ByteArg zero);
void enemy_approach_target_step(struct Obj *parent, struct Obj *obj)
{
    u8 idx;
    struct Obj *partner;
    u16 speed;
    s32 dx;
    s32 dy;
    u8 *ai;
    u8 *tab;
    u16 timerIdx;
    u32 v;
    u8 zero;

    ai = (u8 *)obj + 0x58;
    if (find_active_entry(parent, &idx) == 0 || (s8)zone_point_code(parent, obj, 0) == -1)
    {
        set_state(parent, obj, 0);
        return;
    }
    if ((s16)obj->sensor->unk8 != 0 && (u8)target_in_box(parent, obj) != 0)
    {
        if (((s16 *)parent->tgtA)[2] == 2 || ((s16 *)*(u32 *)((u8 *)parent + 0x204))[2] == 2)
        {
            set_state(parent, obj, 2);
            return;
        }
    }
    partner = *(struct Obj **)((u8 *)parent + idx * 4 + 0x28);
    timerIdx = *(u16 *)(ai + 0x178);
    tab = *(u8 **)(ai + 0x158);
    speed = *(u16 *)(tab + timerIdx * 2 + 0x28);
    dx = partner->x - obj->x;
    dy = partner->y - obj->y;
    if ((dx | dy) != 0)
    {
        v = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        v = 0xFF;
    }
    zero = 0;
    *((u8 *)obj + 2) = v;
    start_move_to(obj, partner->x, partner->y, speed, zero);
    step_pos_xy_plain(obj);
}

void enemy_land_wait_sprite(struct Obj *parent, struct Obj *obj)
{
    struct Cell *cell = obj->cell;
    s8 *flags;
    u8 *p;

    if (obj->phase != 0)
    {
        if (cell->unk23 > 8)
        {
            flags = (s8 *)&obj->flags20C;
            *flags &= -0x21;
            *flags &= 0x7F;
            *flags &= -0x41;
        }
        if (cell->unk23 > 0xD && cell->unk2A - cell->unk20 <= 0)
        {
            p = &obj->flags20C;
            *p |= 0x20;
            *p |= 0x80;
            *p |= 0x40;
            state_change_hook(parent, obj, 3);
            obj->phase = 0;
        }
    }
    else
    {
        if (cell->unk23 > 1 && cell->unk2A - cell->unk20 <= 0)
        {
            set_state(parent, obj, 4);
        }
    }
}

NONMATCH("asm/code_0814AEAC/aim_ahead_step.s", void aim_ahead_step(struct Obj *parent, struct Obj *obj))
{
    s32 q;
    s32 rootOut;
    s32 scaled;
    s32 tgt;
    s32 vx;
    s32 tvx;
    s32 vy;
    s32 tvy;

    q = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zNextAdj - obj->zNext, 4);
    if (q != 0)
    {
        rootOut = ((s32 (*)(s32, s32))gMathRoutines)(q * q, (s32)gMathRoutines);
        scaled = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(q * (s16)obj->zspeed, rootOut);
        obj->dz += scaled;
    }
    obj->zspeed -= obj->stepLen;
    if ((s16)obj->zspeed < 0)
    {
        if (obj->flags1D6 & 0x10)
        {
            state_change_hook(parent, obj, 6);
            *(s8 *)&obj->flags1D6 &= -0x11;
        }
        if (obj->dz <= 0x400)
        {
            obj->dz = 0;
            *(s8 *)&obj->flags20F &= -5;
            if ((u8)position_blocked(parent, obj, obj->x, obj->y, obj->z + obj->dz) != 0)
            {
                set_state(parent, obj, 0xC);
            }
            else
            {
                set_state(parent, obj, 7);
            }
            return;
        }
    }
    tgt = parent->tgtA;
    vx = *(s16 *)((u8 *)obj + 0x242);
    tvx = *(s16 *)((u8 *)tgt + 0x242);
    if (tvx < 0)
    {
        tvx += 3;
    }
    obj->x += vx + (tvx >> 2);
    vy = *(s16 *)((u8 *)obj + 0x244);
    tvy = *(s16 *)((u8 *)tgt + 0x244);
    if (tvy < 0)
    {
        tvy += 3;
    }
    obj->y += vy + (tvy >> 2);
}
END_NONMATCH

void wait_then_box_check(struct Obj *parent, struct Obj *obj)
{
    s16 box[8];
    struct Cell *cell = obj->cell;
    s8 *flags;
    s8 *mode;

    if (obj->phase != 0)
    {
        if (cell->unk23 > 2)
        {
            flags = (s8 *)&obj->flags20C;
            *flags &= -0x21;
            *flags &= 0x7F;
            *flags &= -0x41;
        }
        if (cell->unk23 > 3 && cell->unk2A - cell->unk20 <= 0)
        {
            state_change_hook(parent, obj, 8);
            obj->phase = 0;
        }
    }
    else
    {
        if (cell->unk23 != 0)
        {
            u8 *p = &obj->flags20C;
            *p |= 0x20;
            *p |= 0x80;
            *p |= 0x40;
        }
        if (cell->unk23 > 2 && cell->unk2A - cell->unk20 <= 0)
        {
            mode = (s8 *)&obj->mode;
            *mode &= -8;
            obj_box_to_stack(box, obj, 0x22, 0);
            if ((u8)point_in_box(box, obj, 0x7FFF, 0))
            {
                set_state(parent, obj, 0);
            }
            else
            {
                set_state(parent, obj, 9);
            }
        }
    }
}

void idle_wander(struct Obj *parent, struct Obj *obj)
{
    u16 *tab = (u16 *)((u8 *)obj + 0x100);
    u16 *timer = &obj->timer;
    u16 speed;

    if (*timer == 0)
    {
        if ((u8)target_in_box(parent, obj))
        {
            set_state(parent, obj, 1);
            return;
        }
        *timer = 0x28;
    }
    else
    {
        (*timer)--;
    }
    if (!step_pos_xy_c(obj))
    {
        speed = tab[rand_below(2, mt19937_next()) + 0x18];
        wander_to_edge(obj, speed, ((s16 *)tab)[rand_below(4, mt19937_next())]);
    }
}

void chase_active_entry(struct Obj *parent, struct Obj *obj)
{
    s16 box[8];
    u8 idx;
    u8 *ai;
    u8 *tab;
    u16 timerIdx;
    struct Obj *partner;
    u16 speed;
    s32 dx;
    s32 dy;
    u32 v;
    u8 zero;

    ai = (u8 *)obj + 0x58;
    if (find_active_entry(parent, &idx) != 0)
    {
        goto found;
    }
    obj_box_to_stack(box, obj, 0x22, 0);
    if ((u8)point_in_box(box, obj, 0x7FFF, 0) != 0)
    {
        goto to_idle;
    }
    goto to_wait;
found:
    obj_box_to_stack(box, obj, 0x22, 0);
    if ((u8)point_in_box(box, obj, 0x7FFF, 0) != 0)
    {
        goto chase;
    }
to_wait:
    set_state(parent, obj, 9);
    return;
chase:
    if ((*((u8 *)parent + 0x246) & 0x1C) != 0x0C)
    {
        if ((s16)obj->sensor->unk8 != 0 && (u8)target_in_box(parent, obj) != 0)
        {
            set_state(parent, obj, 2);
            return;
        }
    }
    partner = *(struct Obj **)((u8 *)parent + idx * 4 + 0x28);
    timerIdx = *(u16 *)(ai + 0x178);
    tab = *(u8 **)(ai + 0x158);
    speed = *(u16 *)(tab + timerIdx * 2 + 0x28);
    dx = partner->x - obj->x;
    dy = partner->y - obj->y;
    if ((dx | dy) != 0)
    {
        v = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        v = 0xFF;
    }
    zero = 0;
    *((u8 *)obj + 2) = v;
    start_move_to(obj, partner->x, partner->y, speed, zero);
    if ((u8)position_blocked(parent, obj, obj->x + *(s16 *)((u8 *)obj + 0x242),
                             obj->y + *(s16 *)((u8 *)obj + 0x244), obj->z + obj->dz) != 0)
    {
to_idle:
        set_state(parent, obj, 0);
    }
    else
    {
        step_pos_xy_plain(obj);
    }
}
u32 position_blocked(struct Obj *parent, struct Obj *obj, u32 x, u32 y, u32 z);
NONMATCH("asm/code_0814AEAC/sidestep_drift.s", void sidestep_drift(struct Obj *parent, struct Obj *obj))
{
    u8 *beh;
    s32 frames;
    s32 nextX;
    u32 f;

    beh = (u8 *)obj + 0x58;
    frames = obj->counter2DE + 1;
    obj->counter2DE = frames;
    if ((u16)frames > 3)
    {
        if (obj->flags1D6 & 4)
        {
            obj->x += 0x400;
        }
        else
        {
            obj->x -= 0x400;
        }
        nextX = obj->x;
        obj->vx = obj->x - obj->unk34;
        obj->counter2DE = 0;
        if (*(u16 *)(beh + 0x17C) <= 0xFFFE)
        {
            (*(u16 *)(beh + 0x17C))++;
        }
        f = *(beh + 0x17E);
        ((struct Bit2Byte *)(beh + 0x17E))->bit2 = ((f >> 2) ^ 1) & 1;
    }
    if (*(u16 *)(beh + 0x17C) > 0x1D)
    {
        if ((u8)position_blocked(parent, obj, obj->x, obj->y, obj->z + obj->dz) == 0)
        {
            set_state(parent, obj, 0xD);
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0814AEAC/hop_fall.s", void hop_fall(struct Obj *parent, struct Obj *obj))

void vertical_step(struct Obj *parent, struct Obj *obj);
void walk_step_or_restart(struct Obj *parent, struct Obj *obj);
ASM_FUNC("asm/code_0814AEAC/behavior_set_obj_state.s", void behavior_set_obj_state(struct Obj *parent, struct Obj *obj, u8 state))

void sprite_step_e6a0(void *obj);
ASM_FUNC("asm/code_0814AEAC/state_change_hook.s", void state_change_hook(struct Obj *parent, struct Obj *obj, u8 state))
void set_state_timers(struct Obj *parent, struct Obj *obj, u8 state)
{
    if (state == 0)
    {
        obj->timer = 0x50;
    }
    else if (state == 1)
    {
        obj->timer = 0x41;
    }
    else if (state == 2)
    {
        obj->phase = 1;
    }
    else if (state == 3)
    {
    }
    else if (state == 4)
    {
        ((struct Flags8 *)&obj->flags1D6)->bit4 = 1;
    }
    else if (state == 5)
    {
    }
    else if (state == 6)
    {
    }
    else if (state == 7)
    {
        obj->phase = 1;
    }
    else if (state == 8)
    {
    }
    else if (state == 9)
    {
    }
    else if (state == 10)
    {
    }
    else if (state == 11)
    {
    }
    else if (state == 12)
    {
    }
    else if (state == 0xD)
    {
        obj->phase = 0;
    }
}

void set_state(struct Obj *parent, struct Obj *obj, u8 state)
{
    set_state_timers(parent, obj, state);
    behavior_set_obj_state(parent, obj, state);
    state_change_hook(parent, obj, state);
}

u32 target_in_box(struct Obj *obj, struct Obj *other)
{
    s16 box[8];
    u32 hit;

    obj_box_mirror_to_stack(box, other, 0x24, 0xA);
    hit = boxes_overlap(box, other, (struct Obj *)obj->tgtA, 0x30, 0x30);
    if ((hit << 24) != 0)
    {
        hit = 1;
    }
    return hit;
}

void vertical_step(struct Obj *parent, struct Obj *obj)
{
    s32 scale;
    s32 dzPart;

    scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zNextAdj - obj->zNext, 4);
    if (scale != 0)
    {
        dzPart = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(scale * scale);
        obj->dz += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scale * (s16)obj->zspeed, dzPart);
    }
    obj->zspeed -= obj->stepLen;
    if ((s16)obj->zspeed < 0 && obj->dz <= 0)
    {
        obj->dz = 0;
        set_state(parent, obj, 0xC);
    }
}

void walk_step_or_restart(struct Obj *parent, struct Obj *obj)
{
    if (!step_pos_xy_plain(obj))
    {
        set_state(parent, obj, 0);
    }
}

void save_home_and_wander(struct Obj *parent, struct Obj *obj)
{
    u32 flags;

    flags = obj->flags33E;
    flags |= 0x10;
    obj->flags33E = flags;
    reset_record_fields(obj);
    ((struct Bit2Byte *)&obj->flags20D)->bit2 = 0;
    obj->homeX = obj->x;
    obj->homeY = obj->y;
    obj->homeZ = obj->z;
    obj->homeDZ = obj->dz;
    set_state(parent, obj, 0);
    obj->altHandler = (u32)run_state_handler;
    obj->index = rand_below(4, mt19937_next());
}
void tick_store(void);
u8 step_pos_xy_c(struct Obj *obj);
void wander_to_edge(struct Obj *obj, u16 a, s16 b);
void set_idle_slot(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    u8 *box = obj->boxTab;

    obj->timer = box[obj->index + 0x50];
    obj_set_sprite_link(obj, 0);
    ((struct Bit2Byte *)&obj->flags20E)->bit2 = 0;
    slot[0] = (u32)tick_store;
    slot[3] = slot[1];
}

void start_vertical_move(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    u8 *boxTab;

    boxTab = obj->boxTab;
    obj->zNext = obj->z + obj->dz;
    obj->zNextAdj = obj->homeZ + obj->homeDZ;
    obj->zspeed = *(u16 *)(boxTab + obj->index * 2 + 0x44);
    obj->phase = 1;
    slot[0] = (u32)enemy_hop_phase;
}

void start_drop_slot(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    u8 *boxTab;

    boxTab = obj->boxTab;
    obj->zNext = obj->z + obj->dz;
    obj->zNextAdj = obj->z + obj->dz - 0x3000;
    obj->zspeed = *(u16 *)(boxTab + obj->index * 2 + 0x44);
    ((struct ModeBits *)&obj->mode)->low3 = 4;
    slot[0] = (u32)behavior_fall_step;
    slot[1] = (u32)enemy_pose_move;
}

void behavior_fall_step(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    s32 dz;

    if (obj->cell->unk12 & 8)
    {
        ((struct ModeBits *)&obj->mode)->low3 = 0;
    }
    dz = obj->dz - (s16)obj->zspeed;
    obj->dz = dz;
    if ((obj->flags24 & 0x18000) == 0)
    {
        if (obj->z + dz > obj->zNextAdj)
        {
            return;
        }
    }
    slot[0] = slot[1];
}

void step_xyz_or_set_handler(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    if (obj->cell->unk23 == 2)
    {
        ((struct ModeBits *)&obj->mode)->low3 = 5;
    }
    if (!step_pos_xyz(obj))
    {
        slot[0] = (u32)enemy_box_reset;
    }
}

void start_handler_acfd(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    StateFn fn;
    u32 flags;

    flags = obj->flags20E;
    flags |= 4;
    obj->flags20E = flags;
    fn = (StateFn)enemy_tile_move;
    slot[0] = (u32)fn;
    fn(parent, obj, slot);
}

void reset_to_home(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    ((struct ModeBits *)&obj->mode)->low3 = 0;
    obj_set_sprite_link(obj, 0);
    obj_set_or_add_pos(obj, obj->homeX, obj->homeY, obj->homeZ + obj->homeDZ, 0, 0);
    slot[0] = (u32)hit_active_entry;
}
