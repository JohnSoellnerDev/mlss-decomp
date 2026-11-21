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
};

struct Enemy
{
    u8 filler0[2];
    u8 unk2;
    u8 filler3[0xC - 3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u32 unk24;
    struct Enemy *unk28[0x10];
    u8 filler68[0x100 - 0x68];
    s16 unk100[4];
    u16 unk108[4];
    struct Cell *unk110;
    u8 filler114[0x130 - 0x114];
    u16 unk130[2];
    u8 filler134[0x198 - 0x134];
    u32 unk198;
    u8 filler19C[0x1A0 - 0x19C];
    u32 unk1A0;
    u32 unk1A4;
    u8 filler1A8[0x1B0 - 0x1A8];
    u8 *unk1B0;
    u8 filler1B4[0x1D4 - 0x1B4];
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7;
    u8 unk1D8;
    u8 filler1D9[0x1DC - 0x1D9];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x200 - 0x1EC];
    struct Enemy *unk200;
    struct Enemy *unk204;
    u8 unk208;
    u8 filler209[0x20C - 0x209];
    u8 unk20C_0;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 filler210[0x212 - 0x210];
    u16 unk212;
    u8 unk214;
    u8 filler215;
    u8 unk216;
    u8 filler217[0x242 - 0x217];
    s16 vx; /* added to posX each frame */
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 unk248; /* ai_hop_fall: fall speed, taken from dz each frame */
    u8 filler24C[0x272 - 0x24C];
    u16 unk272[6];
    u8 filler27E[0x282 - 0x27E];
    u8 unk282;
    u8 filler283[0x294 - 0x283];
    struct Cell *cell;
    u8 filler298[0x2C4 - 0x298];
    s32 unk2C4;
    u8 filler2C8[0x2D4 - 0x2C8];
    s32 unk2D4;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 unk2DC;
    u8 filler2DE[0x338 - 0x2DE];
    void *unk338;
    u8 filler33C[0x33E - 0x33C];
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *unk344;
};

struct BitsMode
{
    u8 low : 3;
    u8 high : 5;
};

struct BitsFlag4D
{
    u8 low : 3;
    u8 mid : 4;
    u8 top : 1;
};

struct BitsFlag1D6
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

struct Bits1D8
{
    u8 lo : 2;
    u8 b2 : 1;
    u8 rest : 5;
};

struct Seq212
{
    u16 lo7 : 7;
    u16 active : 2;
    u16 rest : 7;
};

struct VtblEntry
{
    s16 off;
    u8 filler2[2];
    u32 fn;
};

struct Vtbl80
{
    s16 off;
    u8 filler2[2];
    void (*fn)(void *, u32);
};

struct Bits33E
{
    u8 lo : 4;
    u8 b4 : 1;
    u8 rest : 3;
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

struct CellMode12
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

void obj_set_or_add_pos(struct Enemy *obj, u32 dx, u32 dy, u32 dz, u32 copy, u32 add);
void obj_box_to_stack(s16 *box, struct Enemy *ent, u32 a, u32 b);
u8 point_in_box(s16 *box, struct Enemy *obj, s32 x, s32 y);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u32 angle_of_vector16(s32 x, s32 y);
void enemy_pick_entry_sprite(void);
void wander_to_edge(struct Enemy *ent, s32 steps, s32 dist);
void start_axis_move(struct Enemy *ent, s32 a, s32 b, s32 c);
s32 advance_move(struct Enemy *ent);
s32 step_z(struct Enemy *ent);
u8 step_pos_xy_c(struct Enemy *ent);
u8 step_pos_xy_plain(struct Enemy *ent);
void load_record_fields(struct Enemy *ent, u32 mode);
void load_hit_box_words(struct Enemy *ent, u32 mode);
void reset_record_fields(struct Enemy *ent);
void run_state_handler(void);
void enemy_move_axis(void);
void enemy_check_owner_box(void);
void enemy_start_fall(void);
void enemy_start_fall_alt(void);
void reset_to_home(void);
void fall_landing_pick_1d8(void);
s32 find_active_entry(struct Enemy *owner, u8 *idx);
void enemy_seek_partner_axis(void);
void enemy_seek_partner_axis_alt(void);
u8 entry_box_check(struct Enemy *parent, struct Enemy *obj);
void start_axis_wander(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void start_axis_wander_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_state(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_active_state(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void start_fall_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void start_fall_to_target_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_entry(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_check_1d8(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_check_1d8_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void obj_set_active(struct Enemy *ent, u32 zero);
void tick_store(void);
u8 *obj_probe_tile_heights(struct Enemy *obj, s32 x, s32 y, s32 *h0, s32 *h1, s32 *h2);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void ai_dash_step(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void flags_obj_set_mode(struct Enemy *ent, u32 mode);
extern u8 gUnk_02000001[];

void hit_active_entry(struct Enemy *owner, struct Enemy *ent, u32 *out)
{
    s16 box[8];
    u8 idx;
    struct Enemy *partner;
    s32 v;

    if ((u8)find_active_entry(owner, &idx))
    {
        partner = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
        obj_box_to_stack(box, ent, 0x22, 0);
        v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(ent->posZ + ent->dz, 0x100);
        if (point_in_box(box, partner, (v * 0x10000 + 0x200000) >> 16, (v * 0x10000 - 0x200000) >> 16))
        {
            *out = (u32)enemy_pick_entry_sprite;
        }
    }
}

void start_fall_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct BitsFlag1D6 *bits;

    obj->unk2C4 = obj->posZ + obj->dz;
    obj->unk2D4 = parent->unk200->posZ + parent->unk200->dz;
    obj->unk2DC = 0x300;
    ((struct BitsMode *)&obj->unk214)->low = 4;
    bits = (struct BitsFlag1D6 *)&obj->unk1D6;
    bits->b0 = 0;
    bits->b1 = 1;
    *slot = (u32)fall_landing_check_1d8;
}

void fall_landing_state(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct BitsFlag1D6 *bits = (struct BitsFlag1D6 *)&obj->unk1D6;

    bits->b0 = 0;
    bits->b1 = 1;
    *slot = (u32)fall_landing_entry;
}

void start_fall_to_target_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct BitsFlag1D6 *bits;

    obj->unk2C4 = obj->posZ + obj->dz;
    obj->unk2D4 = parent->unk200->posZ + parent->unk200->dz;
    obj->unk2DC = 0x300;
    ((struct BitsMode *)&obj->unk214)->low = 4;
    bits = (struct BitsFlag1D6 *)&obj->unk1D6;
    bits->b0 = 0;
    bits->b1 = 1;
    *slot = (u32)fall_landing_check_1d8_alt;
}

void fall_landing_active_state(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct BitsFlag1D6 *bits = (struct BitsFlag1D6 *)&obj->unk1D6;

    bits->b0 = 0;
    bits->b1 = 1;
    *slot = (u32)fall_landing_pick_1d8;
}

void fall_landing_check_1d8(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    s32 dz;

    if (obj->cell->unk12 & 8)
    {
        ((struct BitsMode *)&obj->unk214)->low = 0;
    }
    dz = obj->dz - obj->unk2DC;
    obj->dz = dz;
    if (!(obj->unk24 & 0x18000) && obj->posZ + dz > obj->unk2D4)
    {
        return;
    }
    if ((obj->unk1D8 << 31) != 0)
    {
        if ((s32)(obj->unk1D8 << 30) < 0)
        {
            *slot = (u32)enemy_seek_partner_axis_alt;
        }
        else
        {
            *slot = (u32)start_axis_wander;
        }
    }
    else
    {
        *slot = (u32)start_axis_wander;
    }
}

/* AI handler while the enemy falls (dz drops by unk2DC each frame). It lands once unk24 has a bit of
 * 0x18000 set, or once it is no more than 0x2D pixels above the owner's active hero; it then chases
 * that hero if it is in its sensor box (entry_box_check) or else wanders. With no active hero it
 * clears its hero-in-area flag (unk1D8 bit 0) and keeps falling. */
void fall_landing_entry(struct Enemy *owner, struct Enemy *obj, u32 *nextHandler)
{
    u8 heroIdx;
    struct Enemy *hero;

    obj->dz = obj->dz - obj->unk2DC;
    if (obj->unk24 & 0x18000)
    {
        if ((u8)entry_box_check(owner, obj))
        {
            *nextHandler = (u32)enemy_seek_partner_axis_alt;
        }
        else
        {
            *nextHandler = (u32)start_axis_wander;
        }
        return;
    }
    if (!(u8)find_active_entry(owner, &heroIdx))
    {
        ((struct Flags8 *)&obj->unk1D8)->bit0 = 0;
        return;
    }
    hero = *(struct Enemy **)((u8 *)owner + heroIdx * 4 + 0x28); /* owner->unk28[heroIdx] */
    if (obj->posZ + obj->dz > hero->posZ + hero->dz + 0x2D00)
    {
        return;
    }
    if ((u8)entry_box_check(owner, obj))
    {
        *nextHandler = (u32)enemy_seek_partner_axis_alt;
    }
    else
    {
        *nextHandler = (u32)start_axis_wander;
    }
}

void fall_landing_check_1d8_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    s32 dz;

    if (obj->cell->unk12 & 8)
    {
        ((struct BitsMode *)&obj->unk214)->low = 0;
    }
    dz = obj->dz - obj->unk2DC;
    obj->dz = dz;
    if (!(obj->unk24 & 0x18000) && obj->posZ + dz > obj->unk2D4)
    {
        return;
    }
    if ((obj->unk1D8 << 31) != 0)
    {
        if ((s32)(obj->unk1D8 << 30) < 0)
        {
            *slot = (u32)enemy_seek_partner_axis;
        }
        else
        {
            *slot = (u32)start_axis_wander_alt;
        }
    }
    else
    {
        *slot = (u32)start_axis_wander_alt;
    }
}

void obj_set_or_add_pos(struct Enemy *obj, u32 dx, u32 dy, u32 dz, u32 copy, u32 add);
void obj_box_to_stack(s16 *box, struct Enemy *ent, u32 a, u32 b);
u8 point_in_box(s16 *box, struct Enemy *obj, s32 x, s32 y);
void enemy_pick_entry_sprite(void);
void snap_to_linked(struct Enemy *ent)
{
    struct Enemy *obj;

    obj = ent->unk344;
    obj_set_or_add_pos(obj, ent->posX, ent->posY, ent->posZ, 0, 0);
    ((struct Flags8 *)&obj->unk20E)->bit2 = 0;
    ((struct Flags8 *)&obj->unk20D)->bit3 = 1;
    ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
    ((struct Flags8 *)&obj->unk20D)->bit5 = 0;
    ((struct Flags8 *)&obj->unk20D)->bit6 = 0;
    *(u32 *)((u8 *)obj + 0x20C) = (*(u32 *)((u8 *)obj + 0x20C) & 0xFFFC7FFF) | 0x8000;
    {
        u8 *p = &obj->unk20F;
        *p = *p | 4;
    }
}

extern const u8 gFrameAttrTable[];
void obj_set_cell_coord(struct Enemy *ent, u32 a, u32 b);
void entry_overlap_or_zone(struct Enemy *owner, struct Enemy *obj, u32 *slot);
void face_entry(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void random_cell_walk(struct Enemy *parent, struct Enemy *obj, u32 *slot);
u8 boxes_overlap_oriented(s16 *box, struct Enemy *a, struct Enemy *b, s32 zAbove, s32 zBelow);
s8 zone_point_code(struct Enemy *parent, struct Enemy *obj, s32 zero);
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
extern const u8 gFrameAttrTable[];
void aim_at_entry_move(struct Enemy *owner, struct Enemy *obj, u32 *slot);
void entry_overlap_flags_zone(struct Enemy *owner, struct Enemy *obj, u32 *slot);
/* Enemy AI step: starts a random walk. wander_to_edge picks a direction that keeps the enemy inside its
   bounds, with a walk distance from the two at 0x130 and a speed from the four at 0x100; cell->unk20 gets
   one of the four values at 0x108. The sprite frame follows the facing (gFrameAttrTable), unk2 becomes
   the direction of travel (angle >> 8, 0xFF when standing still), and the next step is
   entry_overlap_or_zone. */
void random_cell_walk(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    u8 *params;
    u8 frame;
    u32 angle;
    u16 *vx;
    u16 *vy;
    u16 vxValue;

    params = (u8 *)obj + 0x100;
    {
        register u16 distance asm("r4") = ((u16 *)(params + 0x30))[rand_below(2, mt19937_next())];

        wander_to_edge(obj, distance, ((s16 *)params)[rand_below(4, mt19937_next())]);
    }
    obj->cell->unk20 = ((u16 *)(params + 8))[rand_below(4, mt19937_next())];
    /* facing (low 3 bits of unk24) and bits 2-3 of unk216 pick the frame */
    frame = gFrameAttrTable[((((u32)obj->unk216 << 28) >> 30) << 4) + (((u32)*(u8 *)&obj->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(obj, frame, 0);
    ((struct BitsMode *)&obj->unk214)->low = 3;
    vx = &obj->vx;
    vy = &obj->vy;
    vxValue = *vx;
    if (vxValue | *vy)
    {
        angle = (u16)angle_of_vector16(obj->vx, obj->vy);
        obj->unk2 = (angle + 0x10) >> 8;
    }
    else
    {
        obj->unk2 = 0xFF;
    }
    *slot = (u32)entry_overlap_or_zone;
}

/* Enemy AI step while walking (set by random_cell_walk). If the owner's active entry (a hero)
   overlaps the enemy's reach box (six s8 values at unk1B0 + 0x28, in 16ths), the enemy turns to
   face it (face_entry). Otherwise, once the walk step is done (step_pos_xy_c returns 0), it aims
   at the entry if it is inside a zone, else picks a new random walk. */
void entry_overlap_or_zone(struct Enemy *owner, struct Enemy *obj, u32 *slot)
{
    s16 box[8];
    u8 idx;
    struct Enemy *target;
    u8 **paramsPtr;
    u8 *params;

    paramsPtr = &obj->unk1B0;
    if ((u8)find_active_entry(owner, &idx))
    {
        target = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
        params = *paramsPtr;
        {
            s32 v = params[0x28];
            box[0] = (s8)v << 4;
        }
        box[2] = (s8)params[0x29] << 4;
        box[4] = (s8)params[0x2A] << 4;
        box[1] = (s8)params[0x2B] << 4;
        box[3] = (s8)params[0x2C] << 4;
        box[5] = (s8)params[0x2D] << 4;
        if ((u8)boxes_overlap_oriented(box, obj, target, box[4], box[5]))
        {
            *slot = (u32)face_entry;
            return;
        }
    }
    if ((u8)step_pos_xy_c(obj))
    {
        return;
    }
    if (zone_point_code(owner, obj, 0) != -1)
    {
        *slot = (u32)aim_at_entry_move;
    }
    else
    {
        *slot = (u32)random_cell_walk;
    }
}

ASM_FUNC("asm/code_0814CB10/aim_at_entry_move.s", void aim_at_entry_move(struct Enemy *owner, struct Enemy *obj, u32 *slot))

/* Enemy AI step while walking. If the owner's active entry (a hero) overlaps the enemy's reach box
   (six s8 values at unk1B0 + 0x28, in 16ths), it clears unk20D bit 2 and turns to face it
   (face_entry). Otherwise the collision bits at byte 0x4D (bits 3-6) are latched into unk1D6
   bits 2/3; either one set picks a new random walk. When the walk step is done
   (step_pos_xy_plain returns 0), it aims at the entry if it is inside a zone, else walks again. */
void entry_overlap_flags_zone(struct Enemy *owner, struct Enemy *obj, u32 *slot)
{
    s16 box[8];
    u8 idx;
    struct Enemy *target;
    u8 *base; /* base + 0x17E is obj->unk1D6 */
    u8 *params;

    base = (u8 *)obj + 0x58;
    if ((u8)find_active_entry(owner, &idx))
    {
        target = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
        params = obj->unk1B0;
        {
            s32 v = params[0x28];
            box[0] = (s8)v << 4;
        }
        box[2] = (s8)params[0x29] << 4;
        box[4] = (s8)params[0x2A] << 4;
        box[1] = (s8)params[0x2B] << 4;
        box[3] = (s8)params[0x2C] << 4;
        box[5] = (s8)params[0x2D] << 4;
        if ((u8)boxes_overlap_oriented(box, obj, target, box[4], box[5]))
        {
            ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
            *slot = (u32)face_entry;
            return;
        }
    }
    if ((((u32)*(u8 *)((u8 *)obj + 0x4D) << 25) >> 28) & 0xA)
    {
        ((struct Flags8 *)(base + 0x17E))->bit2 = 1;
    }
    if ((((u32)*(u8 *)((u8 *)obj + 0x4D) << 25) >> 28) & 5)
    {
        ((struct Flags8 *)(base + 0x17E))->bit3 = 1;
    }
    if (*(base + 0x17E) & 0xC)
    {
        *slot = (u32)random_cell_walk;
        return;
    }
    if ((u8)step_pos_xy_plain(obj))
    {
        return;
    }
    if (zone_point_code(owner, obj, 0) != -1)
    {
        *slot = (u32)aim_at_entry_move;
    }
    else
    {
        *slot = (u32)random_cell_walk;
    }
}

void ai_face_target_tick(struct Enemy *parent, struct Enemy *obj, u32 *slot);
NONMATCH("asm/code_0814CB10/face_entry.s", void face_entry(struct Enemy *parent, struct Enemy *obj, u32 *slot))
{
    struct Enemy *entry;
    u8 *beh;
    u8 idx;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;
    s32 mode;
    u32 scaled;
    u32 ang;

    beh = (u8 *)obj + 0x58;
    if (!(u8)find_active_entry(parent, &idx))
    {
        idx = ((u32)parent->unk208 << 30) >> 31;
    }
    entry = *(struct Enemy **)((u8 *)parent + idx * 4 + 0x28);
    flags_obj_set_mode(obj, 0x28);
    dx = entry->posX - obj->posX;
    dy = entry->posY - obj->posY;
    ax = dx;
    if (ax < 0)
    {
        ax = -ax;
    }
    ay = dy;
    if (ay < 0)
    {
        ay = -ay;
    }
    if (ax > ay)
    {
        mode = 2;
        if (dx < 0)
        {
            mode = 6;
        }
        ((struct BitsMode *)&obj->unk24)->low = mode;
    }
    else
    {
        ((struct BitsMode *)&obj->unk24)->low = (~dy >> 31) & 4;
    }
    scaled = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)*(u8 *)&obj->unk24 << 29) >> 29, 2);
    obj_set_cell_coord(obj, scaled + 4, 0);
    ((struct BitsMode *)&obj->unk214)->low = 1;
    dx = entry->posX - obj->posX;
    dy = entry->posY - obj->posY;
    if ((dx | dy) != 0)
    {
        ang = (u16)angle_of_vector16(dx, dy);
        obj->unk2 = (ang + 0x10) >> 8;
    }
    else
    {
        obj->unk2 = 0xFF;
    }
    *(u16 *)(beh + 0x17A) = 6;
    *(u16 *)(beh + 0x17C) = 4;
    *slot = (u32)ai_face_target_tick;
}
END_NONMATCH

void ai_start_dash(struct Enemy *unused, struct Enemy *parent, u32 *slot);
void ai_start_hop(struct Enemy *parent, struct Enemy *obj, u32 *slot);
ASM_FUNC("asm/code_0814CB10/ai_face_target_tick.s", void ai_face_target_tick(struct Enemy *parent, struct Enemy *obj, u32 *slot))

/* Launches obj's child object (obj->unk344) in obj's facing direction: gives it a speed picked at
 * random from obj's table, places it in front of obj, then hands over to ai_dash_step, which moves
 * it until it hits something. */
void ai_start_dash(struct Enemy *unused, struct Enemy *obj, u32 *slot)
{
    struct Enemy *child;
    u8 dir;
    u16 speed;
    s32 pick;
    s32 x;
    s32 y;
    s32 z;
    s32 pad[4];

    child = obj->unk344;
    /* facing (low 3 bits of unk24) to one of four directions: 0 up, 1 right, 2 down, 3 left */
    dir = gFrameAttrTable[((((u32)obj->unk216 << 28) >> 30) << 4) + (((u32)*(u8 *)&obj->unk24 << 29) >> 29) + 8];
    pick = rand_below(2, mt19937_next());
    speed = ((u16 *)(obj->unk1B0 + 0x30))[pick];
    ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
    switch (dir)
    {
    case 0:
        child->vx = dir;
        child->vy = -speed;
        break;
    case 1:
        child->vx = speed;
        child->vy = 0;
        break;
    case 2:
        child->vx = 0;
        child->vy = speed;
        break;
    case 3:
        child->vx = -speed;
        child->vy = 0;
        break;
    default:
        return;
    }
    obj_set_active(child, 1);
    obj_set_sprite_link(child, 1);
    x = obj->posX;
    y = obj->posY;
    switch (dir)
    {
    case 0:
        y = y - 0x100;
        break;
    case 1:
        x = x + 0x1500;
        break;
    case 2:
        y = y + 0x800;
        break;
    case 3:
        x = x - 0x1500;
        break;
    }
    z = obj->posZ + 0x600;
    z += obj->dz;
    obj_set_or_add_pos(child, x, y, z, 1, 0);
    child->unk272[3] = 0x800;
    child->unk272[0] = 0x800;
    child->unk272[4] = 0x800;
    child->unk272[1] = 0x800;
    child->unk272[5] = 0x800;
    child->unk272[2] = 0x800;
    obj_load_record(child->cell, 0xD, -1, 0, 0);
    ((struct CellMode12 *)&obj->cell->unk12)->mode = 1;
    *slot = (u32)ai_dash_step;
}
/* AI state after ai_start_dash: moves obj's projectile (obj->unk344) by its speed each frame
 * (obj's state bits clear once obj's cell flags the end of its animation). When the projectile is
 * hidden, its hit check (method at +0x68 of its class) reports a hit, it is flagged in unk24 or it
 * leaves the active layers, it is hidden and unlinked, obj's unk20D bit 2 is set and the AI goes
 * on to the next state (slot[1]). */
void ai_dash_step(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct Enemy *shot;
    struct VtblEntry *method;
    u8 hit;

    shot = obj->unk344;
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk282, 2); /* result unused */
    if (obj->cell->unk12 & 8)
    {
        ((struct BitsMode *)&obj->unk214)->low = 0;
    }
    shot->posX += shot->vx;
    shot->posY += shot->vy;
    if (!(shot->unk216 & 0x20))
    {
        method = (struct VtblEntry *)((u8 *)shot->unk338 + 0x68);
        hit = ((u8 (*)(void *))method->fn)((u8 *)shot + method->off);
        /* 0x20C: the layers it is on */
        if (!hit && !(shot->unk24 & 0x18000) && (*(u32 *)((u8 *)shot + 0x20C) & (u32)gUnk_02000001) == (u32)gUnk_02000001)
        {
            return;
        }
    }
    ((struct Flags8 *)&obj->unk20D)->bit2 = 1;
    obj_set_active(shot, 0);
    obj_set_sprite_link(shot, 0);
    *slot = slot[1];
}

void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void ai_airborne(struct Enemy *parent, struct Enemy *obj, u32 *slot);
/* AI state: launches obj's projectile (obj->unk344) straight up. It is shown and linked, placed 3 px
 * in front of obj (behind when obj's cell is flipped) and 0x19 above it, given a rise speed picked at
 * random from obj's parameter table (u16s at +0x38) and record 0xE; obj's cell plays mode 1 and
 * ai_airborne follows the projectile from there. */
void ai_start_hop(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct Enemy *shot;
    u8 *body;
    u8 *params;
    s32 roll;
    s32 offsetX;
    s32 x;
    s32 y;
    s32 z;

    body = (u8 *)obj + 0x58;
    shot = obj->unk344;
    ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
    obj_set_active(shot, 1);
    obj_set_sprite_link(shot, 1);
    if (obj->cell->unk11 & 0x40)
    {
        offsetX = -0x300;
        x = obj->posX + offsetX;
    }
    else
    {
        offsetX = 0x300;
        x = obj->posX + offsetX;
    }
    y = obj->posY;
    z = obj->posZ + 0x1900;
    z += obj->dz;
    obj_set_or_add_pos(shot, x, y, z, 1, 0);
    shot->unk272[3] = 0x1000;
    shot->unk272[0] = 0x1000;
    shot->unk272[4] = 0xA00;
    shot->unk272[1] = 0xA00;
    shot->unk272[5] = 0x800;
    shot->unk272[2] = 0x800;
    roll = rand_below(2, mt19937_next());
    params = *(u8 **)(body + 0x158);
    shot->unk2DC = ((u16 *)(params + 0x38))[roll];
    ((struct CellMode12 *)&obj->cell->unk12)->mode = 1;
    obj_load_record(shot->cell, 0xE, -1, 0, 0);
    ((struct Flags8 *)&obj->unk20E)->bit2 = 1;
    *slot = (u32)ai_airborne;
}

void ai_leap_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot);
/* AI state after ai_start_hop: obj's projectile (obj->unk344) rises by its unk2DC each frame, and
 * obj's cell gets state 6 once its timer runs out. When the projectile is gone (hidden, flagged in
 * byte 0x25, off the active layers, 0xF0 above the higher of the two heroes, or off screen
 * vertically), it is unlinked, a random wait from obj's parameter table (u8s at +0x3C) is set, and
 * after the wait ai_leap_to_target drops it on a hero. */
void ai_airborne(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct Enemy *shot;
    struct Enemy *heroA;
    struct Enemy *heroB;
    struct Enemy *higher;
    struct Cell *cell;
    u8 *body;
    u8 *params;
    s32 heroAZ;
    s32 heroBZ;
    s32 timer;
    s32 roll;

    body = (u8 *)obj + 0x58;
    shot = obj->unk344;
    heroA = parent->unk200;
    heroAZ = heroA->posZ + heroA->dz;
    heroB = parent->unk204;
    heroBZ = heroB->posZ + heroB->dz;
    higher = heroB;
    if (heroAZ >= heroBZ)
    {
        higher = heroA;
    }
    shot->dz = shot->dz + shot->unk2DC;
    cell = obj->cell;
    if (cell->unk23 > 4)
    {
        timer = cell->unk2A - (s8)cell->unk20;
        if (timer <= 0)
        {
            cell->unk23 = 6;
            ((struct BitsMode *)&obj->unk214)->low = 0;
        }
    }
    /* 0x20C: the layers it is on, 0x52: its screen y */
    if ((shot->unk216 & 0x20)
        || (((u8 *)shot)[0x25] & 0x78)
        || (*(u32 *)((u8 *)shot + 0x20C) & (u32)gUnk_02000001) != (u32)gUnk_02000001
        || shot->posZ + shot->dz >= higher->posZ + higher->dz + 0xF000
        || *(s16 *)((u8 *)shot + 0x52) < 0
        || *(s16 *)((u8 *)shot + 0x52) > 0x9F)
    {
        obj_set_sprite_link(shot, 0);
        roll = rand_below(2, mt19937_next());
        params = *(u8 **)(body + 0x158);
        timer = ((u8 *)(params + 0x3C))[roll];
        *(u16 *)(body + 0x17A) = timer;
        ((struct BitsMode *)&obj->unk214)->low = 0;
        *slot = (u32)tick_store;
        ((u32 *)slot)[3] = (u32)ai_leap_to_target;
    }
}

void ai_hop_fall(u32 unused, struct Enemy *obj, u32 *slot);
/* AI state: drop obj's projectile (obj->unk344) on the active hero. It starts 0xF0 above the spot
 * the hero will reach in 24 frames, or the spot 24 frames behind when there's no floor to land on
 * ahead; with no active hero, 0x20 to obj's right. Fall speed and gravity are picked at random
 * from obj's parameter table, then ai_hop_fall runs the fall. */
void ai_leap_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct Enemy *shot;
    struct Enemy *target;
    s16 *vxp;
    s16 *vyp;
    u8 idx;
    s32 floorZ;
    s32 lowZ;
    s32 highZ;
    u8 *body;
    u8 *params;
    s32 tileX;
    s32 tileY;
    s32 aimX;
    s32 aimY;
    s32 dz;
    s32 roll;
    s32 roll2;

    body = (u8 *)obj + 0x58;
    shot = obj->unk344;
    if ((u8)find_active_entry(parent, &idx))
    {
        target = *(struct Enemy **)((u8 *)parent + idx * 4 + 0x28);
        vxp = &target->vx;
        aimX = target->posX + (((*vxp << 1) + *vxp) << 3);
        if (aimX < 0)
        {
            aimX += 0xFFF;
        }
        tileX = aimX >> 12;
        vyp = &target->vy;
        aimY = target->posY + (((*vyp << 1) + *vyp) << 3);
        if (aimY < 0)
        {
            aimY += 0xFFF;
        }
        tileY = aimY >> 12;
        obj_probe_tile_heights(obj, tileX, tileY, &floorZ, &lowZ, &highZ);
        if (floorZ == 0x7FFFFFFF || (lowZ != 0x7FFFFFFF && highZ == 0x7FFFFFFF))
        {
            shot->posX = target->posX - (((*vxp << 1) + *vxp) << 3);
            shot->posY = target->posY - (((*vyp << 1) + *vyp) << 3);
        }
        else
        {
            shot->posX = target->posX + (((target->vx << 1) + target->vx) << 3);
            shot->posY = target->posY + (((target->vy << 1) + target->vy) << 3);
        }
        shot->posZ = target->posZ;
        dz = 0xF000;
    }
    else
    {
        shot->posX = obj->posX + 0x2000;
        shot->posY = obj->posY;
        shot->posZ = obj->posZ;
        dz = obj->dz + 0xF000;
    }
    shot->dz = dz;
    obj_set_sprite_link(shot, 1);
    roll = rand_below(2, mt19937_next());
    /* obj->unk1B0: u16 fall speeds at +0x3E, u8 gravities at +0x42 */
    shot->unk248 = ((u16 *)(*(u8 **)(body + 0x158) + 0x3E))[roll];
    roll2 = rand_below(2, mt19937_next());
    params = *(u8 **)(body + 0x158);
    shot->unk2DC = ((u8 *)(params + 0x42))[roll2];
    *slot = (u32)ai_hop_fall;
}

void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
/* AI state after ai_leap_to_target: while obj's projectile (obj->unk344) is shown, it falls by its
 * fall speed (unk248) each frame, the speed growing by its gravity (unk2DC); on reaching the
 * ground it is hidden and gets event 2 through its vtable's entry at +0x80. Once it is hidden and
 * its sequence (unk212 bits 7-8) has ended, it goes back to obj's position, is unlinked, and the
 * AI goes on to the state saved in slot[2]. */
void ai_hop_fall(u32 unused, struct Enemy *obj, u32 *slot)
{
    struct Enemy *shot;
    struct Vtbl80 *vt;
    u16 sequence;

    shot = obj->unk344;
    if (shot->unk20C_0 & 1)
    {
        shot->dz = shot->dz - shot->unk248;
        shot->unk248 += shot->unk2DC;
        if (shot->dz > 0)
        {
            return;
        }
        shot->dz = 0;
        obj_set_active(shot, 0);
        vt = (struct Vtbl80 *)((u8 *)shot->unk338 + 0x80);
        vt->fn((u8 *)shot + vt->off, 2);
    }
    else
    {
        sequence = shot->unk212 & 0x180;
        if (sequence != 0)
        {
            return;
        }
        ((struct Flags8 *)&obj->unk20D)->bit2 = 1;
        shot->posX = obj->posX;
        shot->posY = obj->posY;
        shot->posZ = obj->posZ;
        shot->dz = obj->dz;
        ((struct Seq212 *)&shot->unk212)->active = 0;
        obj->unk2DC = sequence;
        ((struct Flags8 *)&obj->unk20E)->bit2 = 0;
        obj_set_sprite_link(shot, 0);
        *slot = slot[2];
    }
}
