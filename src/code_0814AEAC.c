#include "global.h"
#include "ram_routines.h"

/* The enemy's animated sprite, stepped by sprite_step_e6a0. */
struct Cell
{
    u8 filler0[0x11];
    u8 flip;            /* bit 6: mirrored horizontally, bit 7: vertically */
    u8 unk12;           /* bits 1-2: at the end, 0 loop, 1 hold the last frame, 2 stop; bit 3: end reached */
    u8 filler13[0x20 - 0x13];
    s8 unk20;           /* animation speed per step (sign: direction) */
    u8 filler21[0x23 - 0x21];
    u8 unk23;           /* current frame of the animation */
    u8 filler24[0x2A - 0x24];
    s16 unk2A;          /* time left on the current frame */
    s16 unk2C;          /* time since the animation started */
};

struct Sensor
{
    u8 filler0[8];
    u16 unk8;
};

struct Obj
{
    u8 filler0[0x2];
    u8 heading;         /* facing, 0-255 */
    u8 filler3[0xC - 0x3];
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
    u32 tgtB;
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
    u16 vy;
    u8 filler246[0x248 - 0x246];
    u32 stepLen;
    u8 filler24C[0x278 - 0x24C];
    u16 width;          /* body size, 8.8 fixed point */
    u8 filler27A[0x27C - 0x27A];
    u16 height;
    u8 filler27E[0x294 - 0x27E];
    struct Cell *cell;
    u8 filler298[0x2C4 - 0x298];
    s32 zNext;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 moveTargetX;    /* where the current straight-line move ends */
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

struct CellBits12
{
    u8 b0 : 1;
    u8 mode : 2;
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
void obj_set_cell_coord(struct Obj *obj, u32 cell, u32 arg);
u32 position_blocked(struct Obj *parent, struct Obj *obj, u32 x, u32 y, u32 z);
void reset_record_fields(struct Obj *obj);
u32 mt19937_next(void);
u8 step_pos_xy_c(struct Obj *obj);
void wander_to_edge(struct Obj *obj, u16 a, s32 b);
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
void start_move_to_3d(struct Obj *obj, s32 x, s32 y, s32 z, u16 speed, ByteArg flag);
NONMATCH("asm/code_0814AEAC/enemy_pose_move.s", void enemy_pose_move(struct Obj *parent, struct Obj *obj, u32 *slot))
{
    u8 *ai;
    u8 *rec;
    u16 *timer;
    s32 dy;
    s32 dx;
    s32 dist;
    s32 xStep;
    s32 yStep;
    s32 zdz;
    s32 homeZ;
    s32 zTarget;
    u8 frame;
    u16 speed;
    u8 one;
    s32 hoverOffset;

    hoverOffset = -0x2000;
    ai = (u8 *)obj + 0x58;
    rec = obj->boxTab;
    dx = obj->homeX - obj->x;
    dy = obj->homeY - obj->y;
    dist = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy), 0x100);
    frame = rec[obj->index + 0x40];
    xStep = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx, dist) * frame;
    yStep = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dy, dist) * frame;
    zdz = obj->z + obj->dz;
    homeZ = obj->homeZ + obj->homeDZ;
    if (zdz > homeZ)
    {
        zTarget = 0x2000;
    }
    else
    {
        zTarget = homeZ - (zdz + hoverOffset);
    }
    timer = (u16 *)(ai + 0x178);
    speed = *(u16 *)(rec + *timer * 2 + 0x38);
    one = 1;
    start_move_to_3d(obj, xStep, yStep, zTarget, speed, one);
    ((struct ModeBits *)&obj->mode)->low3 = 4;
    obj->cell->unk20 = rec[*timer + 0x34];
    slot[0] = (u32)enemy_axis_limit_step;
    slot[1] = (u32)start_drop_slot;
}
END_NONMATCH
void start_vertical_move(struct Obj *parent, struct Obj *obj, u32 *slot);
/*
 * Steps the enemy along its move back home (enemy_pose_move started it). Once its
 * body (width x height, x centred, y at its feet) overlaps a 50 x 38 px box around the
 * home position, the state after the move becomes start_vertical_move (back to the
 * home height). When the move ends, the slot goes on to that next state.
 */
void enemy_axis_limit_step(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    s16 homeBox[8]; /* [0] left, [1] right, [2] bottom, [3] top, in pixels */
    s16 homeY;
    s16 homeX;
    u8 moving;
    s16 x;
    s16 y;
    u16 halfWidth;
    s32 pos;

    pos = obj->homeX;
    if (pos < 0)
    {
        pos += 0xff;
    }
    homeX = (u32)pos >> 8;
    pos = obj->homeY;
    if (pos < 0)
    {
        pos += 0xff;
    }
    homeY = (u32)pos >> 8;
    x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->x, 0x100);
    y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->y, 0x100);
    halfWidth = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->width, 0x200);
    if (obj->cell->unk23 == 2)
    {
        /* mode bits 0-2 = 5 */
        *(s8 *)&obj->mode = (*(s8 *)&obj->mode & -8) | 5;
    }
    moving = step_pos_xyz(obj);
    homeBox[0] = homeX - 0x19;
    homeBox[3] = homeY - 0x13;
    homeBox[1] = homeX + 0x19;
    homeBox[2] = homeY + 0x13;
    if (slot[1] != (u32)start_vertical_move
        && x + halfWidth >= homeBox[0]
        && x - halfWidth <= homeBox[1]
        && y >= homeBox[3]
        && y - MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->height, 0x100) <= homeBox[2])
    {
        slot[1] = (u32)start_vertical_move;
    }
    if (moving == 0)
    {
        slot[0] = slot[1];
    }
}
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_step_e6a0(void *spr);
/*
 * Phase 1 (from start_vertical_move): sinks the enemy by zspeed each frame until it is back at
 * its home height, then plays the landing animation (record 2, hold the last frame) and goes to
 * phase 0. Phase 0: once the landing animation has run long enough, waits (set_idle_slot) and
 * then puts the enemy back home (reset_to_home).
 */
void enemy_hop_phase(struct Obj *parent, struct Obj *obj, u32 *slot)
{
    u8 *rec;
    struct Cell *sprite;
    StateFn next;
    s32 dz;

    rec = obj->boxTab;
    if (obj->phase != 0)
    {
        dz = obj->dz - (s16)obj->zspeed;
        obj->dz = dz;
        if (obj->z + dz <= obj->zNextAdj)
        {
            /* mode bits 0-2 = 1, animation end mode = 1 (hold the last frame) */
            *(s8 *)&obj->mode = (*(s8 *)&obj->mode & -8) | 1;
            sprite = obj->cell;
            *(s8 *)&sprite->unk12 = (*(s8 *)&sprite->unk12 & -7) | 2;
            obj_load_record(obj->cell, 2, -1, 0, 0);
            sprite_step_e6a0(obj->cell);
            obj->cell->unk20 = rec[obj->index + 0x4C];
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
            next = set_idle_slot;
            slot[0] = (u32)next;
            slot[1] = (u32)reset_to_home;
            next(parent, obj, slot);
        }
    }
}

u32 angle_of_vector16(s32 x, s32 y);
s32 zone_point_code(void *parent, struct Obj *obj, s32 zero);
void start_move_to(struct Obj *obj, s32 x, s32 y, s32 speed, ByteArg zero);
/*
 * Walks the enemy toward the active brother. Gives up (state 0) when there is no
 * active brother or neither brother is in the enemy's zone; goes to state 2 when the
 * zone is armed, the target is in reach and either target is in state 2.
 */
void enemy_approach_target_step(struct Obj *parent, struct Obj *obj)
{
    u8 activeIdx;
    struct Obj *bro;
    u16 speed;
    s32 dx;
    s32 dy;
    u8 *ai;
    u8 *tab;
    u16 index;
    u32 heading;
    u8 zero;

    ai = (u8 *)obj + 0x58;
    if (find_active_entry(parent, &activeIdx) == 0 || (s8)zone_point_code(parent, obj, 0) == -1)
    {
        set_state(parent, obj, 0);
        return;
    }
    if ((s16)obj->sensor->unk8 != 0 && (u8)target_in_box(parent, obj) != 0)
    {
        if (((s16 *)parent->tgtA)[2] == 2 || ((s16 *)parent->tgtB)[2] == 2)
        {
            set_state(parent, obj, 2);
            return;
        }
    }
    bro = *(struct Obj **)((u8 *)parent + activeIdx * 4 + 0x28);
    index = *(u16 *)(ai + 0x178);   /* obj->index */
    tab = *(u8 **)(ai + 0x158);     /* obj->boxTab */
    speed = *(u16 *)(tab + index * 2 + 0x28);
    dx = bro->x - obj->x;
    dy = bro->y - obj->y;
    if ((dx | dy) != 0)
    {
        heading = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        heading = 0xFF;
    }
    zero = 0;
    obj->heading = heading;
    start_move_to(obj, bro->x, bro->y, speed, zero);
    step_pos_xy_plain(obj);
}

/* State 2, the wind-up before the hop: plays out the sprite's animation. In phase 1 the flags20C
 * bits 5-7 are cleared from animation frame 9 on, and when frame 14 or later ends they are set
 * again, state hook 3 fires and phase 0 starts. In phase 0, once frame 2 or later ends, the object
 * goes to state 4 (the hop toward tgtA). */
void enemy_land_wait_sprite(struct Obj *parent, struct Obj *obj)
{
    struct Cell *cell = obj->cell;
    s8 *clearFlags;
    u8 *setFlags;

    if (obj->phase != 0)
    {
        if (cell->unk23 > 8)
        {
            clearFlags = (s8 *)&obj->flags20C;
            *clearFlags &= ~0x20;
            *clearFlags &= 0x7F;
            *clearFlags &= ~0x40;
        }
        /* the frame ends this step */
        if (cell->unk23 > 13 && cell->unk2A - cell->unk20 <= 0)
        {
            setFlags = &obj->flags20C;
            *setFlags |= 0x20;
            *setFlags |= 0x80;
            *setFlags |= 0x40;
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

// Hop toward the target, one frame: the height (dz) moves by zspeed toward zNextAdj and zspeed
// drops by stepLen. Once falling: fire state hook 6 (once, flags1D6 bit 4), and on landing
// (dz <= 4 px) go to state 0xC when the spot is blocked, else state 7. Meanwhile x/y move by the
// object's own velocity plus a quarter of the target's, so it leads a moving target.
void aim_ahead_step(struct Obj *parent, struct Obj *obj)
{
    s32 dir;
    s32 absDir;
    s32 rise;
    s32 target;
    s32 vx;
    s32 targetVx;
    s32 vy;
    s32 targetVy;

    dir = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zNextAdj - obj->zNext, 4);
    if (dir != 0)
    {
        /* zspeed, signed toward zNextAdj */
        absDir = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dir * dir);
        rise = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dir * (s16)obj->zspeed, absDir);
        obj->dz += rise;
    }
    obj->zspeed -= obj->stepLen;
    if ((s16)obj->zspeed < 0)
    {
        if (obj->flags1D6 & 0x10)
        {
            state_change_hook(parent, obj, 6);
            *(s8 *)&obj->flags1D6 &= ~0x10;
        }
        if (obj->dz <= 0x400)
        {
            /* landed */
            obj->dz = 0;
            *(s8 *)&obj->flags20F &= ~4;
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
    vx = *(s16 *)((u8 *)obj + 0x242);
    target = parent->tgtA;
    targetVx = *(s16 *)((u8 *)target + 0x242);
    if (targetVx < 0)
    {
        targetVx += 3;
    }
    obj->x += vx + (targetVx >> 2);
    vy = *(s16 *)((u8 *)obj + 0x244);
    targetVy = *(s16 *)((u8 *)target + 0x244);
    if (targetVy < 0)
    {
        targetVy += 3;
    }
    obj->y += vy + (targetVy >> 2);
}


/* State 7, after a hop lands: plays out the landing animation. In phase 1 the flags20C bits 5-7
 * are cleared from animation frame 3 on, and when frame 4 or later ends the sprite is set up for
 * state 8 and phase 0 starts. In phase 0 the bits are set again from frame 1 on; when frame 3 or
 * later ends, the mode's low 3 bits are cleared and the object goes to state 0 (idle) if
 * point_in_box finds it in its 0x22 box, else to state 9 (walk back home). */
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
            *flags &= ~0x20;
            *flags &= 0x7F;
            *flags &= ~0x40;
        }
        /* the frame ends this step */
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
            u8 *setFlags = &obj->flags20C;
            *setFlags |= 0x20;
            *setFlags |= 0x80;
            *setFlags |= 0x40;
        }
        if (cell->unk23 > 2 && cell->unk2A - cell->unk20 <= 0)
        {
            mode = (s8 *)&obj->mode;
            *mode &= ~7;
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

/* Chase state: while the object stays inside its territory (the box sensor record 0 gives, in
   pixels) it heads for the active hero at its record's speed for obj->index, going to state 2 when
   the hero is in its sensor box (sensor->unk8 set, not while the owner's byte 0x246 mode is 3) and
   to state 0 when the next step is blocked. Outside the territory it goes to state 9; with no
   active hero, to state 0 inside it. */
void chase_active_entry(struct Obj *parent, struct Obj *obj)
{
    s16 box[8];
    u8 heroSlot;
    u8 *ai;
    u8 *rec;
    u16 speedIdx;
    struct Obj *hero;
    u16 speed;
    s32 dx;
    s32 dy;
    u32 heading;
    u8 zero;

    ai = (u8 *)obj + 0x58;
    if (find_active_entry(parent, &heroSlot) != 0)
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
    hero = *(struct Obj **)((u8 *)parent + heroSlot * 4 + 0x28);
    speedIdx = *(u16 *)(ai + 0x178); /* obj->index */
    rec = *(u8 **)(ai + 0x158);      /* obj->boxTab */
    speed = *(u16 *)(rec + speedIdx * 2 + 0x28);
    dx = hero->x - obj->x;
    dy = hero->y - obj->y;
    if ((dx | dy) != 0)
    {
        heading = ((u32)(u16)angle_of_vector16(dx, dy) + 0x10) >> 8;
    }
    else
    {
        heading = 0xFF;
    }
    zero = 0;
    *((u8 *)obj + 2) = heading;
    start_move_to(obj, hero->x, hero->y, speed, zero);
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
/*
 * Shakes the object sideways: every 4th frame it jumps 4 pixels right or left (bit 2 of flags1D6,
 * flipped each time) and counts the shake in phase. After 30 shakes it switches to state 0xD as soon
 * as its position isn't blocked.
 */
void sidestep_drift(struct Obj *parent, struct Obj *obj)
{
    u8 *behavior;
    s32 frames;

    behavior = (u8 *)obj + 0x58;
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
        obj->vx = obj->x - obj->unk34;
        obj->counter2DE = 0;
        /* behavior + 0x17C is obj->phase: the shake count, saturating */
        if (*(u16 *)(behavior + 0x17C) <= 0xFFFE)
        {
            (*(u16 *)(behavior + 0x17C))++;
        }
        /* behavior + 0x17E is obj->flags1D6: flip the shake direction */
        ((struct Flags8 *)(behavior + 0x17E))->bit2 = (behavior[0x17E] >> 2) ^ 1;
    }
    if (*(u16 *)(behavior + 0x17C) > 0x1D)
    {
        if ((u8)position_blocked(parent, obj, obj->x, obj->y, obj->z + obj->dz) == 0)
        {
            set_state(parent, obj, 0xD);
        }
    }
}

NONMATCH("asm/code_0814AEAC/hop_fall.s", void hop_fall(struct Obj *parent, struct Obj *obj))
{
    s32 startSpeed = 0x200;
    s32 t;
    s32 q;
    s32 scale;
    s32 dzPart;

    if (obj->phase == 0)
    {
        obj->zNextAdj = obj->z + 0xA00;
        obj->zNext = obj->z + obj->dz;
        obj->zScale = obj->zNextAdj - obj->zNext;
        obj->zspeed = startSpeed;
        obj->speed2 = startSpeed;
        t = obj->zScale * 2 + startSpeed;
        q = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((t >> 1) - 0x40000, t);
        obj->stepLen = -q;
        if (obj->stepLen == 0)
        {
            obj->stepLen = 1;
        }
        obj->phase = 1;
        return;
    }
    if (obj->phase != 1)
    {
        return;
    }
    scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zNextAdj - obj->zNext, 4);
    if (scale != 0)
    {
        dzPart = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(scale * scale);
        obj->dz += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scale * (s16)obj->zspeed, dzPart);
    }
    obj->zspeed -= obj->stepLen;
    if ((s16)obj->zspeed < 0 && obj->dz <= 0x400)
    {
        obj->dz = 0;
        ((struct Bit2Byte *)&obj->flags20F)->bit2 = 0;
        if ((u8)position_blocked(parent, obj, obj->x, obj->y, obj->z + obj->dz) != 0)
        {
            set_state(parent, obj, 0xC);
        }
        else
        {
            set_state(parent, obj, 7);
        }
    }
}
END_NONMATCH

void vertical_step(struct Obj *parent, struct Obj *obj);
void walk_step_or_restart(struct Obj *parent, struct Obj *obj);
u32 distance_xy(struct Obj *a, struct Obj *b);
void hop_fall(struct Obj *parent, struct Obj *obj);
NONMATCH("asm/code_0814AEAC/behavior_set_obj_state.s", void behavior_set_obj_state(struct Obj *parent, struct Obj *obj, u8 state))
{
    s32 chaseInit = 0x580;
    u16 *tab;
    s32 chaseSpeed;
    s32 zs;
    s32 dist;
    u32 pick;
    u16 frames;
    u16 margin;
    u16 speed;
    u32 *tgtp;
    struct Obj *target;

    tab = (u16 *)((u8 *)obj + 0x100);
    if (state == 0)
    {
        *((u8 *)obj + 0x210) |= 0x20;
        obj->timer = 0x50;
        frames = tab[rand_below(4, mt19937_next())];
        margin = tab[rand_below(2, mt19937_next()) + 0x18];
        wander_to_edge(obj, margin, frames);
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)idle_wander;
    }
    else if (state == 1)
    {
        *((u8 *)obj + 0x210) |= 0x20;
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)chase_active_entry;
    }
    else if (state == 2)
    {
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)enemy_land_wait_sprite;
    }
    else if (state == 4)
    {
        u8 zero;

        chaseSpeed = chaseInit;
        tgtp = &parent->tgtA;
        zero = 0;
        target = (struct Obj *)*tgtp;
        start_move_to(obj, target->x, target->y, chaseSpeed, zero);
        target = (struct Obj *)*tgtp;
        obj->vx = target->x + (s16)target->vx * 6 - obj->x;
        obj->vy = target->y + (s16)target->vy * 6 - obj->y;
        distance_xy(obj, target);
        obj->vx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((s16)obj->vx, 0x20);
        obj->vy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((s16)obj->vy, 0x20);
        obj->zNextAdj = obj->z + obj->dz + 0x2800;
        obj->zNext = obj->z + obj->dz;
        obj->zScale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zNextAdj - obj->zNext, 1);
        obj->zspeed = chaseSpeed;
        obj->speed2 = chaseSpeed;
        zs = obj->zScale * 2 + chaseSpeed;
        dist = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((zs >> 1) - 0x1E4000, zs);
        obj->stepLen = -dist;
        if (obj->stepLen == 0)
        {
            obj->stepLen = 1;
        }
        *((u8 *)obj + 0x20F) |= 4;
        state_change_hook(parent, obj, 4);
        pick = rand_below(0x64, mt19937_next());
        if (pick > 0x13)
        {
            *(u32 *)((u8 *)obj + 0x1A0) = (u32)aim_ahead_step;
        }
        else
        {
            *(u32 *)((u8 *)obj + 0x1A0) = (u32)vertical_step;
        }
    }
    else if (state == 7)
    {
        *((u8 *)obj + 0x210) |= 0x20;
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)wait_then_box_check;
    }
    else if (state == 9)
    {
        u8 zero;

        pick = rand_below(4, mt19937_next());
        speed = tab[pick];
        zero = 0;
        start_move_to(obj, obj->homeX, obj->homeY, speed, zero);
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)walk_step_or_restart;
    }
    else if (state == 0xA)
    {
        pick = rand_below(4, mt19937_next());
        obj->timer = tab[pick + 0x14];
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)tick_store;
        *(u32 *)((u8 *)obj + 0x1AC) = (u32)idle_wander;
    }
    else if (state == 0xB)
    {
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)vertical_step;
    }
    else if (state == 0xC)
    {
        *((s8 *)obj + 0x210) &= -0x21;
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)sidestep_drift;
    }
    else if (state == 0xD)
    {
        *((s8 *)obj + 0x210) &= -0x21;
        *(u32 *)((u8 *)obj + 0x1A0) = (u32)hop_fall;
    }
}
END_NONMATCH

void sprite_step_e6a0(void *obj);
/* Sets up obj's sprite (animation, facing, flip) for the state it has just entered; see set_state. */
void state_change_hook(struct Obj *parent, struct Obj *obj, u8 state)
{
    u8 *base58 = (u8 *)obj + 0x58; /* homeX/homeY are read through this in state 9 */
    s32 facing4;
    s32 angle;
    s32 dx;
    s32 dy;
    u32 facing8;

    if (state == 0)
    {
        u16 *choices = (u16 *)((u8 *)obj + 0x100);
        s32 pick = rand_below(4, mt19937_next());

        obj->cell->unk20 = choices[pick + 4];
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 0;
        ((struct ModeBits *)&obj->mode)->low3 = 4;
    }
    else if (state == 1)
    {
        /* face the parent's target */
        struct Obj *target;
        s32 targetDx;
        s32 targetDy;

        obj->cell->unk20 = 0x30;
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 0;
        ((struct ModeBits *)&obj->mode)->low3 = 4;
        target = (struct Obj *)parent->tgtA;
        targetDx = target->x - obj->x;
        targetDy = target->y - obj->y;
        if ((targetDx | targetDy) != 0)
        {
            angle = (u16)angle_of_vector16(targetDx, targetDy) + 0x1000;
            facing4 = ((angle >> 13) & 7) >> 1;
        }
        else
        {
            facing4 = 0;
        }
        obj_set_cell_coord(obj, facing4, 0);
    }
    else if (state == 2)
    {
        obj->cell->unk20 = 0x31;
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 1;
        obj_load_record(obj->cell, 3, 0, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 3)
    {
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 1;
        if (obj->moveTargetX >= obj->x)
        {
            ((struct Flags8 *)&obj->cell->flip)->bit6 = 0;
        }
        else
        {
            ((struct Flags8 *)&obj->cell->flip)->bit6 = 1;
        }
        obj_load_record(obj->cell, 4, 0, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 4)
    {
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 0;
        obj_load_record(obj->cell, 5, 0, 0, 0);
        if (obj->moveTargetX >= obj->x)
        {
            ((struct Flags8 *)&obj->cell->flip)->bit6 = 0;
        }
        else
        {
            ((struct Flags8 *)&obj->cell->flip)->bit6 = 1;
        }
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 5)
    {
    }
    else if (state == 6)
    {
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 0;
        obj_load_record(obj->cell, 7, 0, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 7)
    {
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 1;
        obj_load_record(obj->cell, 8, 0, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 8)
    {
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 1;
        obj_load_record(obj->cell, 9, 0, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 9)
    {
        /* head and face back towards home */
        s32 homeDx = obj->homeX - obj->x;
        s32 homeDy = obj->homeY - obj->y;
        u32 heading;

        if ((homeDx | homeDy) != 0)
        {
            heading = ((u32)(u16)angle_of_vector16(homeDx, homeDy) + 0x10) >> 8;
        }
        else
        {
            heading = 0xFF;
        }
        obj->heading = heading;
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 0;
        ((struct ModeBits *)&obj->mode)->low3 = 4;
        dx = *(s32 *)(base58 + 0x184) - obj->x;
        dy = *(s32 *)(base58 + 0x188) - obj->y;
        if ((dx | dy) != 0)
        {
            angle = (u16)angle_of_vector16(dx, dy) + 0x1000;
            facing4 = ((angle >> 13) & 7) >> 1;
        }
        else
        {
            facing4 = 0;
        }
        obj_set_cell_coord(obj, facing4, 0);
    }
    else if (state == 0xA)
    {
        /* walk animation in the direction of the velocity */
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 0;
        ((struct ModeBits *)&obj->mode)->low3 = 1;
        if ((s16)obj->vx | (s16)obj->vy)
        {
            angle = (u16)angle_of_vector16((s16)obj->vx, (s16)obj->vy) + 0x1000;
            facing8 = (angle >> 13) & 7;
        }
        else
        {
            facing8 = -1;
        }
        obj_load_record(obj->cell, facing8, -1, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
    else if (state == 0xB)
    {
    }
    else if (state == 0xC)
    {
    }
    else if (state == 0xD)
    {
        obj->cell->unk20 = 0x11;
        ((struct CellBits12 *)&obj->cell->unk12)->mode = 1;
        ((struct ModeBits *)&obj->mode)->low3 = 1;
        obj_load_record(obj->cell, 5, 0, 0, 0);
        sprite_step_e6a0(obj->cell);
    }
}

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

/* Hop straight up and down, one frame (behavior states 4 and 0xB; aim_ahead_step is the hop that also
 * moves x/y): the height (dz) moves by zspeed toward zNextAdj and zspeed drops by stepLen. Once it is
 * falling and back down on the ground (dz <= 0) it lands and goes to state 0xC. */
void vertical_step(struct Obj *parent, struct Obj *obj)
{
    s32 dir;
    s32 absDir;

    dir = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->zNextAdj - obj->zNext, 4);
    if (dir != 0)
    {
        /* zspeed, signed toward zNextAdj */
        absDir = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dir * dir);
        obj->dz += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dir * (s16)obj->zspeed, absDir);
    }
    obj->zspeed -= obj->stepLen;
    if ((s16)obj->zspeed < 0 && obj->dz <= 0)
    {
        /* landed */
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
void wander_to_edge(struct Obj *obj, u16 a, s32 b);
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
