#include "global.h"
#include "ram_routines.h"

struct Bits3
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};

struct Bits
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

#define SPRITE_FLAG_ANIM_DONE 0x08 /* Cell.flags12: the current animation has finished */

struct Cell
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
    u8 filler21[0x2C - 0x21];
    s16 unk2C;
};

struct Mode12
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct Obj
{
    u8 filler0[0xC];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x8];
    u8 flags24;
    u8 filler25[0xDB];
    s16 tbl[0x18];
    u8 filler130[0x68];
    void *handlerA;
    u8 filler19C[0x4];
    void *handlerB;
    void *handlerC;
    u8 filler1A8[0x8];
    s8 *animTbl;
    struct Sensor *sensor;
    u8 filler1B8[0x18];
    u16 index1D0;
    u8 filler1D2[0x2];
    u16 phase1D4;
    u8 flags1D6;
    u8 filler1D7[0x5];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDz;
    u8 filler1EC[0x20];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 filler210[0x4];
    u8 flags214;
    u8 filler215[0x1];
    u8 flags216;
    u8 filler217[0x27];
    u16 speed23E;
    u8 filler240[0x8];
    s32 step248;
    u8 filler24C[0x48];
    struct Cell *cell;
    u8 filler298[0x24];
    s32 tbl2BC;
    u8 filler2C0[0x38];
    u16 hop2F8;
    u8 filler2FA[0x43];
    u8 flags33D;
    u8 flags33E;
    u8 filler33F[0x5];
    struct Obj *linked;
};

struct Sensor
{
    u8 filler0[0x18];
    u16 h18;
    u16 h1A;
};

struct Spr
{
    u8 filler0[0x11];
    u8 flags11;
    u8 filler12[0x21 - 0x12];
    u8 record21;
};

struct Owner
{
    u8 filler0[0x28];
    struct Obj *entries[4];
    u8 filler38[0x208 - 0x38];
    u8 flags208;
};

void reset_record_fields(struct Obj *obj);
void cpu_set(void *src, void *dst, u32 control);
void obj_init_from_link(struct Obj *link);
void obj_start_walk(struct Owner *owner, struct Obj *obj, u16 mode, u8 flag);
void obj_after_run(void);
void obj_set_sprite_link(struct Obj *obj, u32 link);
void obj_set_or_add_pos(struct Obj *obj, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_set_active(struct Obj *obj, u32 on);
void sprite_step_e6a0(void *sprite);
/* gFrameAttrTable[kind * 16 + direction + 8]: the way an object of that kind faces */
extern const u8 gFrameAttrTable[];

enum Facing
{
    FACING_UP,
    FACING_RIGHT,
    FACING_DOWN,
    FACING_LEFT
};

u8 find_active_entry(void *owner, u8 *idx);
s32 zone_point_code(void *parent, struct Obj *obj, s32 zero);
void obj_set_cell_coord(struct Obj *obj, s32 cell, u32 zero);
void obj_halt(struct Obj *obj, u8 flag);
/* Sprite tween modes (see code_08045F18.c) */
enum TweenMode
{
    TWEEN_STEP = 1,  /* add a step every frame until the target */
    TWEEN_TIMED = 2, /* reach the target after a number of frames */
};

/* Tweens the sprite's horizontal scale (8.8, 0x100 = normal) to target */
void obj_set_motion_a_08046AA4(void *obj, u32 mode, u32 relative, u32 target, u32 frames);
/* Advances every running sprite tween by a frame */
void obj_advance_axes(void *obj);
/* Stops the horizontal scale tween */
void obj_clear_flags_2b5_30(void *obj);
/* Nonzero while the horizontal scale tween runs */
u8 obj_flags_2b5_30_set(void *obj);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void zone_move_handler(void);
void zone_entry_handler(struct Owner *parent, struct Obj *obj);
void run_state_handler(void);
void start_wander_b(void);
void obj_seek_entry(struct Owner *owner, struct Obj *obj, void **slot);
void obj_run_step(void *owner, struct Obj *obj, void **handler_slot);
u32 obj_near(void *obj, void *other);
u16 angle_of_vector16(s32 x, s32 y);
u8 probe_body_hit(struct Owner *owner, struct Obj *body, s32 x, s32 y, s32 z, u32 a, u32 b);
u8 step_pos_xy_c(struct Obj *obj);
void wander_to_edge(struct Obj *obj, u16 a, s16 b);

NONMATCH("asm/code_0814F8E4/zone_entry_handler.s", void zone_entry_handler(struct Owner *parent, struct Obj *obj))
{
    u16 *table;
    struct Sensor *sensor;
    struct Obj *entry;
    u16 *ea;
    s32 *tbl2;
    u8 idx;
    s8 zone;
    s32 pick;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;
    u32 rnd;
    u32 pick2;
    u32 face;
    s32 four;
    struct Bits3 *fb;

    table = (u16 *)obj->tbl;
    sensor = obj->sensor;
    zone = zone_point_code(parent, obj, 0);
    if (zone != -1)
    {
        if (find_active_entry(parent, &idx) != 0)
            pick = idx;
        else
            pick = ((u32)parent->flags208 << 30) >> 31;
        entry = *(struct Obj **)((pick << 2) + (u32)parent + 0x28);
        if ((sensor->h18 << 8) <= entry->z + entry->dz && (sensor->h1A << 8) >= entry->z + entry->dz)
        {
            rnd = mt19937_next();
            pick2 = rand_below(4, rnd);
            tbl2 = &obj->tbl2BC;
            ea = &table[(u8)pick2];
            obj->speed23E = ea[0];
            *tbl2 = (s16)ea[0];
            obj->hop2F8 = ea[8];
            obj->step248 = (s16)ea[12];
            dx = entry->x - obj->x;
            dy = entry->y - obj->y;
            ax = dx;
            if (dx < 0)
                ax = -dx;
            ay = dy;
            if (dy < 0)
                ay = -dy;
            if (ax > ay)
            {
                ((struct Spr *)obj->cell)->record21 = 1;
                ((struct Bits *)&((struct Spr *)obj->cell)->flags11)->b6 = (u32)dx >> 31;
                ((struct Bits3 *)&obj->flags24)->lo3 = dx < 0 ? 6 : 2;
            }
            else
            {
                ((struct Spr *)obj->cell)->record21 = ((~dy) >> 31) & 2;
                four = 4;
                fb = (struct Bits3 *)&obj->flags24;
                fb->lo3 = ((~dy) >> 31) & four;
            }
            obj_load_record(obj->cell, ((struct Spr *)obj->cell)->record21, -1, 0, 0);
            face = ((u32)obj->flags24 << 29) >> 30;
            obj_set_cell_coord(obj, face, 0);
            obj_halt(obj, 0);
        }
    }
}
END_NONMATCH


void enter_zone_move(s32 unused, struct Obj *obj, void **slot)
{
    u16 *table;
    u8 moving;
    u32 rnd;
    u32 pick;
    u16 step;

    {
        register u32 ofs asm("r0") = 0x100;

        table = (u16 *)((u8 *)obj + ofs);
    }
    if ((obj->flags1D6 & 4) == 0)
    {
        obj_set_sprite_link(obj, 1);
        rnd = mt19937_next();
        pick = rand_below(4, rnd);
        step = table[pick + 20];
        obj_set_motion_a_08046AA4(obj, 2, 0, 0x100, step);
        obj_advance_axes(obj);
        ((struct Bits *)&obj->flags1D6)->b2 = 1;
    }
    moving = obj_flags_2b5_30_set(obj);
    if (moving == 0)
    {
        obj_clear_flags_2b5_30(obj);
        ((struct Bits *)&obj->flags20C)->b7 = 1;
        ((struct Bits *)&obj->flags20C)->b6 = 1;
        ((struct Bits *)&obj->flags20C)->b5 = 1;
        ((struct Bits *)&obj->flags1D6)->b2 = 0;
        *slot = (void *)zone_move_handler;
    }
}

/*
 * State that sends the object home: squash its sprite to 1/16 width over one
 * of four random durations, then warp it back to its home position and leave
 * it hidden there, waiting in zone_entry_handler.
 */
void enter_zone_entry(s32 unused, struct Obj *obj, void **handler_slot)
{
    u16 *params;
    u8 squashing;
    u32 rnd;
    u32 choice;
    u16 frames;

    params = (u16 *)obj->tbl;
    if ((obj->flags1D6 & 4) == 0) /* squash not started yet */
    {
        rnd = mt19937_next();
        choice = rand_below(4, rnd);
        frames = params[choice + 20];
        obj_set_motion_a_08046AA4(obj, TWEEN_TIMED, 0, 0x10, frames);
        obj_advance_axes(obj);
        ((struct Bits *)&obj->flags1D6)->b2 = 1;
    }
    squashing = obj_flags_2b5_30_set(obj);
    if (squashing == 0)
    {
        obj_clear_flags_2b5_30(obj);
        obj_set_sprite_link(obj, 0);
        obj->x = obj->homeX;
        obj->y = obj->homeY;
        obj->z = obj->homeZ;
        obj->dz = obj->homeDz;
        obj->index1D0 = *(u16 *)(obj->animTbl + 0x18);
        ((struct Bits *)&obj->flags20C)->b7 = 0;
        ((struct Bits *)&obj->flags20C)->b6 = 0;
        ((struct Bits *)&obj->flags20C)->b5 = 0;
        ((struct Bits *)&obj->flags1D6)->b2 = 0;
        *handler_slot = (void *)zone_entry_handler;
    }
}


void load_anim_tables_plain(struct Obj *obj)
{
    register s16 *tbl asm("r4");
    s8 *rec;

    tbl = obj->tbl;
    rec = obj->animTbl;
    cpu_set(rec, tbl, 4);
    rec += 8;
    tbl[4] = rec[0];
    tbl[5] = rec[1];
    rec += 2;
    tbl[6] = rec[0];
    rec += 1;
    tbl[7] = rec[0];
    rec += 1;
    cpu_set(rec, (u8 *)obj + 0x110, 4);
    rec += 8;
    tbl[20] = rec[0];
    tbl[21] = rec[1];
    rec += 2;
    tbl[22] = rec[0];
    tbl[23] = rec[1];
}

ASM_FUNC("asm/code_0814F8E4/obj_target_in_reach.s", void obj_target_in_reach(void))

/*
 * Seek handler: when an active owner entry is near, pick a point beside obj
 * from the direction to it (on an odd roll of 0-99 by quadrant = 8-way
 * direction / 2, otherwise by the 8-way direction itself; only cases 0-3 set
 * a point) and probe it 3 px above obj's height. If nothing is in the way,
 * start walking and hand over to obj_run_step. Otherwise keep stepping, and
 * when the step ends wander toward an edge picked at random from obj->tbl.
 */
void obj_seek_entry(struct Owner *owner, struct Obj *obj, void **slot)
{
    struct Obj *target;
    u8 *base58;
    u16 *table;
    u8 idx;
    s32 roll;
    s32 dir;
    s32 quadrant;
    s32 (*div)(s32, s32);
    s32 dx;
    s32 dy;
    s32 aimX;
    s32 aimY;
    s32 z;
    u8 blocked;
    u8 stepped;
    s32 pickA;
    s32 pickB;
    u16 wanderA;
    s16 wanderB;

    base58 = (u8 *)obj + 0x58;
    table = (u16 *)obj->tbl;
    if (find_active_entry(owner, &idx) != 0)
    {
        /* owner->entries[idx] */
        target = *(struct Obj **)((idx << 2) + (u8 *)owner + 0x28);
        if ((u8)obj_near(obj, target) != 0)
        {
            roll = rand_below(100, mt19937_next());
            if ((roll & 1) != 0)
            {
                div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
                dx = target->x - obj->x;
                dy = target->y - obj->y;
                if ((dx | dy) != 0)
                    dir = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
                else
                    dir = -1;
                quadrant = div(dir, 2);
                switch (quadrant)
                {
                case 0:
                    aimX = obj->x - 0x600;
                    aimY = obj->y - 0x1500;
                    break;
                case 1:
                    aimX = obj->x + 0x1200;
                    aimY = obj->y - 0x600;
                    break;
                case 2:
                    aimX = obj->x + 0x500;
                    aimY = obj->y;
                    break;
                case 3:
                    aimX = obj->x - 0x1200;
                    aimY = obj->y;
                    break;
                }
            }
            else
            {
                dx = target->x - obj->x;
                dy = target->y - obj->y;
                if ((dx | dy) != 0)
                    dir = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
                else
                    dir = -1;
                switch (dir)
                {
                case 0:
                    aimX = obj->x - 0x600;
                    aimY = obj->y - 0x1500;
                    break;
                case 1:
                    aimX = obj->x + 0x1200;
                    aimY = obj->y;
                    break;
                case 2:
                    aimX = obj->x + 0x500;
                    aimY = obj->y + 0xA00;
                    break;
                case 3:
                    aimX = obj->x - 0x1200;
                    aimY = obj->y;
                    break;
                }
            }
            z = obj->z + obj->dz + 0x300;
            blocked = probe_body_hit(owner, obj->linked, aimX, aimY, z, 1, 0);
            if (blocked == 0)
            {
                obj_start_walk(owner, obj, 0, 1);
                *(u16 *)(base58 + 0x17C) = blocked; /* obj->phase1D4 = 0 */
                *slot = (void *)obj_run_step;
                return;
            }
        }
    }
    stepped = step_pos_xy_c(obj);
    if (stepped == 0)
    {
        pickA = rand_below(2, mt19937_next());
        wanderA = table[pickA + 0x18];
        pickB = rand_below(4, mt19937_next());
        wanderB = ((s16 *)table)[pickB];
        wander_to_edge(obj, wanderA, wanderB);
    }
}

/* Run handler: plays the three stages of the run animation, starting the next stage each time
 * the cell's animation finishes. Once the last stage's cell counter passes 95, clears the run
 * flags on obj and its linked object and hands over to obj_after_run. */
void obj_run_step(void *owner, struct Obj *obj, void **handler_slot)
{
    u16 *stage;
    struct Cell *cell;

    stage = &obj->phase1D4;
    switch (*stage)
    {
    case 0:
        cell = obj->cell;
        if ((cell->flags12 & SPRITE_FLAG_ANIM_DONE) != 0)
        {
            obj_start_walk(owner, obj, 1, 1);
            *stage = *stage + 1;
        }
        break;
    case 1:
        cell = obj->cell;
        if ((cell->flags12 & SPRITE_FLAG_ANIM_DONE) != 0)
        {
            obj_start_walk(owner, obj, 2, 1);
            *stage = *stage + 1;
        }
        break;
    case 2:
        cell = obj->cell;
        if (cell->unk2C > 95)
        {
            ((struct Bits *)&obj->flags20D)->b2 = 0;
            ((struct Bits *)&obj->linked->flags20D)->b2 = 0;
            ((struct Bits3 *)&obj->flags214)->lo3 = 0;
            *handler_slot = (void *)obj_after_run;
        }
        break;
    }
}

ASM_FUNC("asm/code_0814F8E4/obj_after_run.s", void obj_after_run(void))

/*
 * Puts the object at its linked object's position (z + dz as its height), loads cell
 * record 0xC, sets its flags, and fills its parameter table from the second half of its
 * animation record (load_anim_tables_plain loads the first half):
 *   tbl[8..11] <- rec[0x1C..0x23]   tbl[12..15] <- rec[0x24..0x27]
 *   tbl[4..7]  <- rec[0x28..0x2B]   tbl[20..23] <- rec[0x2C..0x2F]
 *   tbl[0..3]  <- rec[0x30..0x37]
 * (cpu_set with control 4 copies four halfwords; the single bytes are signed.)
 */
void obj_init_from_link(struct Obj *obj)
{
    s16 *tbl;
    s8 *rec;

    tbl = obj->tbl;
    obj_set_or_add_pos(obj, obj->linked->x, obj->linked->y, obj->linked->z + obj->linked->dz, 1, 0);
    ((struct Bits3 *)&obj->flags214)->lo3 = 4;
    obj_load_record(obj->cell, 0xC, -1, 0, 0);
    ((struct Bits *)&obj->flags20D)->b3 = 1;
    ((struct Bits *)&obj->flags20D)->b5 = 0;
    ((struct Bits *)&obj->flags20D)->b6 = 0;
    ((struct Bits *)&obj->flags20E)->b2 = 0;
    /* the 3-bit field at bits 15-17 of the flags word = 1 */
    *(u32 *)&obj->flags20C = (*(u32 *)&obj->flags20C & 0xFFFC7FFF) | 0x8000;
    ((struct Bits *)&obj->flags20F)->b2 = 1;

    rec = obj->animTbl;
    cpu_set(rec + 0x1C, &obj->tbl[8], 4);
    tbl[12] = rec[0x24];
    tbl[13] = rec[0x25];
    tbl[14] = rec[0x26];
    tbl[15] = rec[0x27];
    tbl[4] = rec[0x28];
    tbl[5] = rec[0x29];
    tbl[6] = rec[0x2A];
    tbl[7] = rec[0x2B];
    tbl[20] = rec[0x2C];
    tbl[21] = rec[0x2D];
    tbl[22] = rec[0x2E];
    tbl[23] = rec[0x2F];
    cpu_set(rec + 0x30, tbl, 4);
}


// Starts one stage (mode 0, 1 or 2) of the object's run animation: picks one of the four
// random values in tbl[4..7] for the cell, and if face_target is set first turns the
// object toward the owner's active entry (directions in flags24: 0 up, 2 right, 4 down,
// 6 left). The frame comes from gFrameAttrTable by kind (flags216) and direction, plus 4/8/12.
void obj_start_walk(struct Owner *owner, struct Obj *obj, u16 mode, u8 face_target)
{
    struct Obj *target;
    u16 *params;
    u8 activeIdx;
    s32 pick;
    s32 targetIdx;
    s32 dx;
    s32 dy;
    s32 absDx;
    s32 absDy;
    u32 frame;
    s32 down;
    struct Bits3 *dirBits;

    params = (u16 *)obj->tbl;
    pick = rand_below(4, mt19937_next());
    obj->cell->unk20 = params[pick + 4];
    ((struct Bits3 *)&obj->flags214)->lo3 = 1;
    if (face_target != 0)
    {
        if (find_active_entry(owner, &activeIdx) != 0)
            targetIdx = activeIdx;
        else
            targetIdx = ((u32)owner->flags208 << 30) >> 31;
        target = *(struct Obj **)((targetIdx << 2) + (u32)owner + 0x28);
        dx = target->x - obj->x;
        dy = target->y - obj->y;
        absDx = dx;
        if (dx < 0)
            absDx = -dx;
        absDy = dy;
        if (dy < 0)
            absDy = -dy;
        if (absDx > absDy)
        {
            /* face left or right, mirrored when the target is to the left */
            ((struct Spr *)obj->cell)->record21 = 1;
            ((struct Bits *)&((struct Spr *)obj->cell)->flags11)->b6 = (u32)dx >> 31;
            ((struct Bits3 *)&obj->flags24)->lo3 = dx < 0 ? 6 : 2;
        }
        else
        {
            /* face down (dy >= 0) or up */
            ((struct Spr *)obj->cell)->record21 = ((~dy) >> 31) & 2;
            down = 4;
            dirBits = (struct Bits3 *)&obj->flags24;
            dirBits->lo3 = ((~dy) >> 31) & down;
        }
    }
    switch (mode)
    {
    case 0:
        frame = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4) + (((u32)obj->flags24 << 29) >> 29) + 8];
        frame += 4;
        break;
    case 1:
        frame = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4) + (((u32)obj->flags24 << 29) >> 29) + 8];
        frame += 8;
        break;
    case 2:
        frame = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4) + (((u32)obj->flags24 << 29) >> 29) + 8];
        frame += 0xC;
        break;
    default:
        return;
    }
    obj_set_cell_coord(obj, frame, 0);
    ((struct Mode12 *)&obj->cell->flags12)->mode = 1;
}

/* Shows the object's linked object in front of it, by the way it faces (gFrameAttrTable), 6 pixels
   above it, with animation 0xC (alt set; slightly different offsets) or 0xD. Coordinates are in
   1/256 pixels. */
void obj_place_linked(struct Obj *obj, u8 alt)
{
    struct Obj *linked;
    s32 x;
    s32 y;
    s32 facing;

    linked = obj->linked;
    if (alt != 0)
    {
        facing = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4) + (((u32)obj->flags24 << 29) >> 29) + 8];
        switch (facing)
        {
        case FACING_RIGHT:
            x = obj->x + 0x1200;
            y = obj->y - 0x600;
            break;
        case FACING_UP:
            x = obj->x - 0x600;
            y = obj->y - 0x1500;
            break;
        case FACING_DOWN:
            x = obj->x + 0x500;
            y = obj->y;
            break;
        case FACING_LEFT:
            x = obj->x - 0x1200;
            y = obj->y;
            break;
        }
    }
    else
    {
        facing = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4) + (((u32)obj->flags24 << 29) >> 29) + 8];
        switch (facing)
        {
        case FACING_UP:
            x = obj->x - 0x600;
            y = obj->y - 0x1500;
            break;
        case FACING_RIGHT:
            x = obj->x + 0x1200;
            y = obj->y;
            break;
        case FACING_DOWN:
            x = obj->x + 0x500;
            y = obj->y + 0xA00;
            break;
        case FACING_LEFT:
            x = obj->x - 0x1200;
            y = obj->y;
            break;
        }
    }
    obj_set_or_add_pos(linked, x, y, obj->z + obj->dz + 0x600, 1, 0);
    if (alt != 0)
    {
        obj_load_record(linked->cell, 0xC, -1, 0, 0);
    }
    else
    {
        obj_load_record(linked->cell, 0xD, -1, 0, alt);
    }
    sprite_step_e6a0(linked->cell);
    obj_set_active(linked, 1);
    obj_set_sprite_link(linked, 1);
}


u32 depth_scale_a(s16 depth)
{
    if (depth <= 208)
        return 0x400;
    if (depth <= 256)
        return 0x600;
    if (depth <= 304)
        return 0x600;
    if (depth <= 352)
        return 0x700;
    if (depth <= 400)
        return 0x800;
    if (depth <= 448)
        return 0x800;
    if (depth <= 512)
        return 0xA00;
    return 0;
}

u32 depth_scale_b(s16 depth)
{
    if (depth <= 208)
        return 0x500;
    if (depth <= 256)
        return 0x700;
    if (depth <= 304)
        return 0x900;
    if (depth <= 352)
        return 0xB00;
    if (depth <= 400)
        return 0xC00;
    if (depth <= 448)
        return 0xD00;
    if (depth <= 512)
        return 0xF00;
    return 0;
}

u32 depth_scale_c(s16 depth)
{
    if (depth <= 208)
        return 0x800;
    if (depth <= 256)
        return 0xA00;
    if (depth <= 304)
        return 0xA00;
    if (depth <= 352)
        return 0xC00;
    if (depth <= 400)
        return 0xC00;
    if (depth <= 448)
        return 0x1200;
    if (depth <= 512)
        return 0x1400;
    return 0;
}

u32 kind_value(s8 kind)
{
    if (kind == 0)
        return 0x100;
    if (kind == 1)
        return 0x180;
    if (kind > 1)
        return 0x160;
    return 0x10;
}

void obj_box_mirror_to_stack(s16 *out, void *obj, u32 flags, u32 index);
u32 boxes_overlap(s16 *bounds, void *a, void *b, s32 zAbove, s32 zBelow);

u32 obj_near(void *obj, void *other)
{
    s16 box[8];
    u32 overlap;

    obj_box_mirror_to_stack(box, obj, 0x28, 8);
    overlap = boxes_overlap(box, obj, other, 0x30, 0x30);
    if ((overlap << 24) != 0)
        overlap = 1;
    return overlap;
}

void obj_init_ai(s32 unused, struct Obj *obj)
{
    struct Obj **link;

    ((struct Bits *)&obj->flags33E)->b4 = 1;
    link = &obj->linked;
    ((struct Bits *)&(*link)->flags20D)->b2 = 0;
    reset_record_fields(obj);
    obj_init_from_link(*link);
    ((struct Bits *)&obj->flags33D)->b3 = 1;
    obj->index1D0 = 0;
    ((struct Bits *)&obj->flags1D6)->b0 = 0;
    obj->handlerA = (void *)run_state_handler;
    obj->handlerB = (void *)start_wander_b;
    obj->handlerC = (void *)obj_seek_entry;
}
