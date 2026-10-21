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
    s16 unk242;
    s16 unk244;
    u8 filler246[0x248 - 0x246];
    s32 unk248;
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

void fall_landing_entry(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    u8 idx;
    struct Enemy *partner;

    obj->dz = obj->dz - obj->unk2DC;
    if (obj->unk24 & 0x18000)
    {
        if ((u8)entry_box_check(parent, obj))
        {
            *slot = (u32)enemy_seek_partner_axis_alt;
        }
        else
        {
            *slot = (u32)start_axis_wander;
        }
        return;
    }
    if (!(u8)find_active_entry(parent, &idx))
    {
        ((struct Flags8 *)&obj->unk1D8)->bit0 = 0;
        return;
    }
    partner = *(struct Enemy **)((u8 *)parent + idx * 4 + 0x28);
    if (obj->posZ + obj->dz > partner->posZ + partner->dz + 0x2D00)
    {
        return;
    }
    if ((u8)entry_box_check(parent, obj))
    {
        *slot = (u32)enemy_seek_partner_axis_alt;
    }
    else
    {
        *slot = (u32)start_axis_wander;
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
NONMATCH("asm/code_0814CB10/random_cell_walk.s", void random_cell_walk(struct Enemy *parent, struct Enemy *obj, u32 *slot))
{
    u16 margin;
    u8 *table;
    s16 frames;
    u8 coord;
    u32 ang;

    table = (u8 *)obj + 0x100;
    margin = ((u16 *)(table + 0x30))[rand_below(2, mt19937_next())];
    frames = ((s16 *)table)[rand_below(4, mt19937_next())];
    wander_to_edge(obj, margin, frames);
    obj->cell->unk20 = ((u16 *)(table + 8))[rand_below(4, mt19937_next())];
    coord = gFrameAttrTable[((((u32)obj->unk216 << 28) >> 30) << 4) + (((u32)*(u8 *)&obj->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(obj, coord, 0);
    ((struct BitsMode *)&obj->unk214)->low = 3;
    if ((u16)obj->unk242 | (u16)obj->unk244)
    {
        ang = (u16)angle_of_vector16(obj->unk242, obj->unk244);
        obj->unk2 = (ang + 0x10) >> 8;
    }
    else
    {
        obj->unk2 = 0xFF;
    }
    *slot = (u32)entry_overlap_or_zone;
}
END_NONMATCH

NONMATCH("asm/code_0814CB10/entry_overlap_or_zone.s", void entry_overlap_or_zone(struct Enemy *owner, struct Enemy *obj, u32 *slot))
{
    s16 box[8];
    u8 idx;
    struct Enemy *partner;
    u8 **tab;
    u8 *box_tab;

    tab = &obj->unk1B0;
    if ((u8)find_active_entry(owner, &idx))
    {
        partner = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
        box_tab = *tab;
        box[0] = (s8)box_tab[0x28] << 4;
        box[2] = (s8)box_tab[0x29] << 4;
        box[4] = (s8)box_tab[0x2A] << 4;
        box[1] = (s8)box_tab[0x2B] << 4;
        box[3] = (s8)box_tab[0x2C] << 4;
        box[5] = (s8)box_tab[0x2D] << 4;
        if ((u8)boxes_overlap_oriented(box, obj, partner, box[4], box[5]))
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
END_NONMATCH

ASM_FUNC("asm/code_0814CB10/aim_at_entry_move.s", void aim_at_entry_move(struct Enemy *owner, struct Enemy *obj, u32 *slot))

NONMATCH("asm/code_0814CB10/entry_overlap_flags_zone.s", void entry_overlap_flags_zone(struct Enemy *owner, struct Enemy *obj, u32 *slot))
{
    s16 box[8];
    u8 idx;
    struct Enemy *partner;
    u8 *beh;
    u8 *box_tab;

    beh = (u8 *)obj + 0x58;
    if ((u8)find_active_entry(owner, &idx))
    {
        partner = *(struct Enemy **)((u8 *)owner + idx * 4 + 0x28);
        box_tab = obj->unk1B0;
        box[0] = (s8)box_tab[0x28] << 4;
        box[2] = (s8)box_tab[0x29] << 4;
        box[4] = (s8)box_tab[0x2A] << 4;
        box[1] = (s8)box_tab[0x2B] << 4;
        box[3] = (s8)box_tab[0x2C] << 4;
        box[5] = (s8)box_tab[0x2D] << 4;
        if ((u8)boxes_overlap_oriented(box, obj, partner, box[4], box[5]))
        {
            ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
            *slot = (u32)face_entry;
            return;
        }
    }
    if ((((u32)*(u8 *)((u8 *)obj + 0x4D) << 25) >> 28) & 0xA)
    {
        ((struct Flags8 *)(beh + 0x17E))->bit2 = 1;
    }
    if ((((u32)*(u8 *)((u8 *)obj + 0x4D) << 25) >> 28) & 5)
    {
        ((struct Flags8 *)(beh + 0x17E))->bit3 = 1;
    }
    if (*(beh + 0x17E) & 0xC)
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
END_NONMATCH

void ai_face_target_tick(struct Enemy *parent, struct Enemy *obj, u32 *slot);
ASM_FUNC("asm/code_0814CB10/face_entry.s", void face_entry(struct Enemy *parent, struct Enemy *obj, u32 *slot))

void ai_start_dash(struct Enemy *unused, struct Enemy *parent, u32 *slot);
void ai_start_hop(struct Enemy *parent, struct Enemy *obj, u32 *slot);
ASM_FUNC("asm/code_0814CB10/ai_face_target_tick.s", void ai_face_target_tick(struct Enemy *parent, struct Enemy *obj, u32 *slot))

void ai_start_dash(struct Enemy *unused, struct Enemy *obj, u32 *slot)
{
    struct Enemy *child;
    u8 mode;
    u16 speed;
    s32 roll;
    s32 x;
    s32 y;
    s32 z;
    s32 spare[4];

    child = obj->unk344;
    mode = gFrameAttrTable[((((u32)obj->unk216 << 28) >> 30) << 4) + (((u32)*(u8 *)&obj->unk24 << 29) >> 29) + 8];
    roll = rand_below(2, mt19937_next());
    speed = ((u16 *)(obj->unk1B0 + 0x30))[roll];
    ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
    switch (mode)
    {
    case 0:
        child->unk242 = mode;
        child->unk244 = -speed;
        break;
    case 1:
        child->unk242 = speed;
        child->unk244 = 0;
        break;
    case 2:
        child->unk242 = 0;
        child->unk244 = speed;
        break;
    case 3:
        child->unk242 = -speed;
        child->unk244 = 0;
        break;
    default:
        return;
    }
    obj_set_active(child, 1);
    obj_set_sprite_link(child, 1);
    x = obj->posX;
    y = obj->posY;
    switch (mode)
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
void ai_dash_step(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct Enemy *child;
    struct VtblEntry *vt;
    u8 res;

    child = obj->unk344;
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk282, 2);
    if (obj->cell->unk12 & 8)
    {
        ((struct BitsMode *)&obj->unk214)->low = 0;
    }
    child->posX += child->unk242;
    child->posY += child->unk244;
    if (!(child->unk216 & 0x20))
    {
        vt = (struct VtblEntry *)((u8 *)child->unk338 + 0x68);
        res = ((u8 (*)(void *))vt->fn)((u8 *)child + vt->off);
        if (!res && !(child->unk24 & 0x18000) && (*(u32 *)((u8 *)child + 0x20C) & (u32)gUnk_02000001) == (u32)gUnk_02000001)
        {
            return;
        }
    }
    ((struct Flags8 *)&obj->unk20D)->bit2 = 1;
    obj_set_active(child, 0);
    obj_set_sprite_link(child, 0);
    *slot = slot[1];
}

void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void ai_airborne(struct Enemy *parent, struct Enemy *obj, u32 *slot);
NONMATCH("asm/code_0814CB10/ai_start_hop.s", void ai_start_hop(struct Enemy *parent, struct Enemy *obj, u32 *slot))
{
    struct Enemy *child;
    u8 *beh;
    u8 *table;
    s32 roll;
    s32 dx;
    s32 x0;
    s32 x;
    s32 y;
    s32 z;

    beh = (u8 *)obj + 0x58;
    child = obj->unk344;
    ((struct Flags8 *)&obj->unk20D)->bit2 = 0;
    obj_set_active(child, 1);
    obj_set_sprite_link(child, 1);
    if (obj->cell->unk11 & 0x40)
    {
        x0 = obj->posX;
        dx = -0x300;
    }
    else
    {
        x0 = obj->posX;
        dx = 0x300;
    }
    x = x0 + dx;
    y = obj->posY;
    z = obj->posZ + 0x1900;
    z += obj->dz;
    obj_set_or_add_pos(child, x, y, z, 1, 0);
    child->unk272[3] = 0x1000;
    child->unk272[0] = 0x1000;
    child->unk272[4] = 0xA00;
    child->unk272[1] = 0xA00;
    child->unk272[5] = 0x800;
    child->unk272[2] = 0x800;
    roll = rand_below(2, mt19937_next());
    table = *(u8 **)(beh + 0x158);
    child->unk2DC = ((u16 *)(table + 0x38))[roll];
    ((struct CellMode12 *)&obj->cell->unk12)->mode = 1;
    obj_load_record(child->cell, 0xE, -1, 0, 0);
    ((struct Flags8 *)&obj->unk20E)->bit2 = 1;
    *slot = (u32)ai_airborne;
}
END_NONMATCH

void ai_leap_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void ai_airborne(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct Enemy *child;
    struct Enemy *tgt_a;
    struct Enemy *tgt_b;
    struct Enemy *higher;
    struct Cell *cell;
    u8 *beh;
    u8 *table;
    s32 za;
    s32 zb;
    s32 timer;
    s32 roll;

    beh = (u8 *)obj + 0x58;
    child = obj->unk344;
    tgt_a = parent->unk200;
    za = tgt_a->posZ + tgt_a->dz;
    tgt_b = parent->unk204;
    zb = tgt_b->posZ + tgt_b->dz;
    higher = tgt_b;
    if (za >= zb)
    {
        higher = tgt_a;
    }
    child->dz = child->dz + child->unk2DC;
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
    if ((child->unk216 & 0x20)
        || (((u8 *)child)[0x25] & 0x78)
        || (*(u32 *)((u8 *)child + 0x20C) & (u32)gUnk_02000001) != (u32)gUnk_02000001
        || child->posZ + child->dz >= higher->posZ + higher->dz + 0xF000
        || *(s16 *)((u8 *)child + 0x52) < 0
        || *(s16 *)((u8 *)child + 0x52) > 0x9F)
    {
        obj_set_sprite_link(child, 0);
        roll = rand_below(2, mt19937_next());
        table = *(u8 **)(beh + 0x158);
        timer = ((u8 *)(table + 0x3C))[roll];
        *(u16 *)(beh + 0x17A) = timer;
        ((struct BitsMode *)&obj->unk214)->low = 0;
        *slot = (u32)tick_store;
        ((u32 *)slot)[3] = (u32)ai_leap_to_target;
    }
}

void ai_hop_fall(u32 unused, struct Enemy *obj, u32 *slot);
NONMATCH("asm/code_0814CB10/ai_leap_to_target.s", void ai_leap_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot))
{
    struct Enemy *child;
    struct Enemy *target;
    s16 *vxp;
    s16 *vyp;
    s32 *p248;
    u8 idx;
    s32 h0;
    s32 h1;
    s32 h2;
    u8 *beh;
    u8 *table;
    s32 tile_x;
    s32 tile_y;
    s32 aim_x;
    s32 aim_y;
    s32 new_y;
    s32 dz;
    s32 roll;

    beh = (u8 *)obj + 0x58;
    child = obj->unk344;
    if ((u8)find_active_entry(parent, &idx))
    {
        target = *(struct Enemy **)((u8 *)parent + idx * 4 + 0x28);
        vxp = (s16 *)((u8 *)target + 0x242);
        aim_x = target->posX + (((*vxp << 1) + *vxp) << 3);
        if (aim_x < 0)
        {
            aim_x += 0xFFF;
        }
        tile_x = aim_x >> 12;
        vyp = (s16 *)((u8 *)target + 0x244);
        aim_y = target->posY + (((*vyp << 1) + *vyp) << 3);
        if (aim_y < 0)
        {
            aim_y += 0xFFF;
        }
        tile_y = aim_y >> 12;
        obj_probe_tile_heights(obj, tile_x, tile_y, &h0, &h1, &h2);
        if (h0 == 0x7FFFFFFF || (h1 != 0x7FFFFFFF && h2 == 0x7FFFFFFF))
        {
            child->posX = target->posX - (((*vxp << 1) + *vxp) << 3);
            new_y = target->posY - (((*vyp << 1) + *vyp) << 3);
        }
        else
        {
            child->posX = target->posX + (((target->unk242 << 1) + target->unk242) << 3);
            new_y = target->posY + (((target->unk244 << 1) + target->unk244) << 3);
        }
        child->posY = new_y;
        child->posZ = target->posZ;
        dz = 0xF000;
    }
    else
    {
        child->posX = obj->posX + 0x2000;
        child->posY = obj->posY;
        child->posZ = obj->posZ;
        dz = obj->dz + 0xF000;
    }
    child->dz = dz;
    obj_set_sprite_link(child, 1);
    roll = rand_below(2, mt19937_next());
    p248 = (s32 *)((u8 *)child + 0x248);
    table = *(u8 **)(beh + 0x158);
    *p248 = ((u16 *)(table + 0x3E))[roll];
    roll = rand_below(2, mt19937_next());
    table = *(u8 **)(beh + 0x158);
    child->unk2DC = ((u8 *)(table + 0x42))[roll];
    *slot = (u32)ai_hop_fall;
}
END_NONMATCH

void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void ai_hop_fall(u32 unused, struct Enemy *obj, u32 *slot)
{
    struct Enemy *child;
    struct Vtbl80 *vt;
    u16 active;

    child = obj->unk344;
    if (child->unk20C_0 & 1)
    {
        child->dz = child->dz - child->unk248;
        child->unk248 += child->unk2DC;
        if (child->dz > 0)
        {
            return;
        }
        child->dz = 0;
        obj_set_active(child, 0);
        vt = (struct Vtbl80 *)((u8 *)child->unk338 + 0x80);
        vt->fn((u8 *)child + vt->off, 2);
    }
    else
    {
        active = child->unk212 & 0x180;
        if (active != 0)
        {
            return;
        }
        ((struct Flags8 *)&obj->unk20D)->bit2 = 1;
        child->posX = obj->posX;
        child->posY = obj->posY;
        child->posZ = obj->posZ;
        child->dz = obj->dz;
        ((struct Seq212 *)&child->unk212)->active = 0;
        obj->unk2DC = active;
        ((struct Flags8 *)&obj->unk20E)->bit2 = 0;
        obj_set_sprite_link(child, 0);
        *slot = slot[2];
    }
}
