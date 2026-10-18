#include "global.h"

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
void obj_start_walk(void *owner, struct Obj *obj, u16 mode, u8 flag);
void obj_after_run(void);
void obj_set_sprite_link(struct Obj *obj, u32 link);
void obj_set_or_add_pos(struct Obj *obj, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_set_active(struct Obj *obj, u32 on);
void sprite_step_e6a0(void *sprite);
extern const u8 gFrameAttrTable[];
u8 find_active_entry(void *owner, u8 *idx);
s32 zone_point_code(void *parent, struct Obj *obj, s32 zero);
void obj_set_cell_coord(struct Obj *obj, s32 cell, u32 zero);
void obj_halt(struct Obj *obj, u8 flag);
void obj_set_motion_a_08046AA4(void *obj, u32 a, u32 b, u32 c, u32 d);
void obj_advance_axes(void *obj);
void obj_clear_flags_2b5_30(void *obj);
u8 obj_flags_2b5_30_set(void *obj);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void zone_move_handler(void);
void zone_entry_handler(void);
void run_state_handler(void);
void start_wander_b(void);
void obj_seek_entry(void);

ASM_FUNC("asm/code_0814F8E4/zone_entry_handler.s", void zone_entry_handler(void))


ASM_FUNC("asm/code_0814F8E4/enter_zone_move.s", void enter_zone_move(void))

void enter_zone_entry(s32 unused, struct Obj *obj, void **slot)
{
    u16 *table;
    u8 moving;
    u32 rnd;
    u32 pick;
    u16 step;

    table = (u16 *)obj->tbl;
    if ((obj->flags1D6 & 4) == 0)
    {
        rnd = mt19937_next();
        pick = rand_below(4, rnd);
        step = table[pick + 20];
        obj_set_motion_a_08046AA4(obj, 2, 0, 0x10, step);
        obj_advance_axes(obj);
        ((struct Bits *)&obj->flags1D6)->b2 = 1;
    }
    moving = obj_flags_2b5_30_set(obj);
    if (moving == 0)
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
        *slot = (void *)zone_entry_handler;
    }
}


ASM_FUNC("asm/code_0814F8E4/load_anim_tables_plain.s", void load_anim_tables_plain(void))

ASM_FUNC("asm/code_0814F8E4/obj_target_in_reach.s", void obj_target_in_reach(void))

ASM_FUNC("asm/code_0814F8E4/obj_seek_entry.s", void obj_seek_entry(void))

void obj_run_step(void *owner, struct Obj *obj, void **handler_slot)
{
    u16 *phase;
    struct Cell *cell;

    phase = &obj->phase1D4;
    switch (*phase)
    {
    case 0:
        cell = obj->cell;
        if ((cell->flags12 & 8) != 0)
        {
            obj_start_walk(owner, obj, 1, 1);
            *phase = *phase + 1;
        }
        break;
    case 1:
        cell = obj->cell;
        if ((cell->flags12 & 8) != 0)
        {
            obj_start_walk(owner, obj, 2, 1);
            *phase = *phase + 1;
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
    *(u32 *)&obj->flags20C = (*(u32 *)&obj->flags20C & 0xFFFC7FFF) | 0x8000;
    ((struct Bits *)&obj->flags20F)->b2 = 1;
    rec = obj->animTbl;
    cpu_set(rec + 0x1C, (u8 *)obj + 0x110, 4);
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


ASM_FUNC("asm/code_0814F8E4/obj_start_walk.s", void obj_start_walk(void *owner, struct Obj *obj, u16 mode, u8 flag))

ASM_FUNC("asm/code_0814F8E4/obj_place_linked.s", void obj_place_linked(void))


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
