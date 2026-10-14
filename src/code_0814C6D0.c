#include "global.h"

struct Cell
{
    u8 filler0[0x12];
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
    u8 filler110[0x130 - 0x110];
    u16 unk130[2];
    u8 filler134[0x198 - 0x134];
    u32 unk198;
    u8 filler19C[0x1A0 - 0x19C];
    u32 unk1A0;
    u32 unk1A4;
    u8 filler1A8[0x1D4 - 0x1A8];
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
    u8 filler204[0x20C - 0x204];
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
    u8 filler24C[0x294 - 0x24C];
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

struct Bits33E
{
    u8 lo : 4;
    u8 b4 : 1;
    u8 rest : 3;
};

void wander_to_edge(struct Enemy *ent, s32 steps, s32 dist);
void start_axis_move(struct Enemy *ent, s32 a, s32 b, s32 c);
s32 advance_move(struct Enemy *ent);
s32 step_z(struct Enemy *ent);
u8 step_pos_xy_c(struct Enemy *ent);
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
void start_axis_wander(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void start_axis_wander_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_state(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_active_state(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void start_fall_to_target(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void start_fall_to_target_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_entry(void);
void fall_landing_check_1d8(struct Enemy *parent, struct Enemy *obj, u32 *slot);
void fall_landing_check_1d8_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot);

void entity_mode_36(struct Enemy *parent, struct Enemy *obj)
{
    ((struct Bits33E *)&obj->unk33E)->b4 = 0;
    obj->homeX = obj->posX;
    obj->homeY = obj->posY;
    obj->homeZ = obj->posZ;
    obj->homeDZ = obj->dz;
    obj->unk198 = (u32)run_state_handler;
    obj->unk1A0 = (u32)reset_to_home;
    obj->unk1A4 = 0;
}

void advance_and_step_z(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    if (obj->cell->unk23 == 2)
    {
        ((struct BitsMode *)&obj->unk214)->low = 5;
    }
    advance_move(obj);
    if ((step_z(obj) << 24) == 0)
    {
        ((struct Bits1D8 *)&obj->unk1D8)->b2 = 0;
        *slot = (u32)start_fall_to_target;
    }
}

ASM_FUNC("asm/code_0814C6D0/fall_landing.s", void fall_landing(void))

void walk_step_z(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    if (obj->cell->unk23 == 2)
    {
        if (obj->cell->unk2A <= obj->cell->unk20)
        {
            ((struct BitsMode *)&obj->unk214)->low = 5;
        }
    }
    step_pos_xy_c(obj);
    if ((step_z(obj) << 24) == 0)
    {
        *slot = (u32)enemy_start_fall_alt;
    }
}

void start_axis_wander(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct BitsFlag1D6 *bits;

    wander_to_edge(obj, 0x40, 0x200);
    start_axis_move(obj, 0x6C00, 0, 1);
    ((struct BitsMode *)&obj->unk214)->low = 4;
    bits = (struct BitsFlag1D6 *)&obj->unk1D6;
    bits->b0 = 0;
    bits->b1 = 1;
    *slot = (u32)walk_step_z;
}

void setup_idle_handlers(struct Enemy *parent, struct Enemy *obj)
{
    u8 *p = (u8 *)obj;
    s32 flags;

    flags = *(p + 0x33E);
    flags |= 0x10;
    *(p + 0x33E) = flags;
    load_record_fields(obj, 0);
    load_hit_box_words(obj, 0);
    *(*(u8 **)(p + 0x294) + 0x23) = 0;
    *(*(u8 **)(p + 0x294) + 0x20) = 0x20;
    *(u32 *)(p + 0x198) = (u32)enemy_check_owner_box;
    *(u32 *)(p + 0x1A0) = (u32)start_axis_wander;
}

void advance_and_step_z_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    if (obj->cell->unk23 == 2)
    {
        ((struct BitsMode *)&obj->unk214)->low = 5;
    }
    advance_move(obj);
    if ((step_z(obj) << 24) == 0)
    {
        ((struct Bits1D8 *)&obj->unk1D8)->b2 = 0;
        *slot = (u32)start_fall_to_target_alt;
    }
}

void fall_landing_active(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct
    {
        s16 box[8];
        u8 idx;
    } local;
    s32 dz;

    if (obj->cell->unk12 & 8)
    {
        ((struct BitsMode *)&obj->unk214)->low = 0;
    }
    dz = obj->dz - obj->unk2DC;
    obj->dz = dz;
    if (obj->unk24 & 0x18000)
    {
        if ((obj->unk1D8 << 31) != 0 && (s32)(obj->unk1D8 << 30) < 0
            && (find_active_entry(parent, &local.idx) << 24))
        {
            *slot = (u32)enemy_seek_partner_axis;
        }
        else
        {
            *slot = (u32)start_axis_wander_alt;
        }
    }
    else if (obj->posZ + dz <= obj->unk2D4)
    {
        *slot = (u32)fall_landing_active_state;
    }
}

void walk_step_z_plain(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    if (obj->cell->unk23 == 2)
    {
        ((struct BitsMode *)&obj->unk214)->low = 5;
    }
    step_pos_xy_c(obj);
    if ((step_z(obj) << 24) == 0)
    {
        *slot = (u32)enemy_start_fall;
    }
}

void start_axis_wander_alt(struct Enemy *parent, struct Enemy *obj, u32 *slot)
{
    struct BitsFlag1D6 *bits;

    wander_to_edge(obj, 0x40, 0x200);
    start_axis_move(obj, 0x6C00, 0, 1);
    ((struct BitsMode *)&obj->unk214)->low = 4;
    bits = (struct BitsFlag1D6 *)&obj->unk1D6;
    bits->b0 = 0;
    bits->b1 = 1;
    *slot = (u32)walk_step_z_plain;
}

void reset_phase_handlers(struct Enemy *parent, struct Enemy *obj)
{
    u8 *p = (u8 *)obj;
    s32 flags;

    flags = *(p + 0x33E);
    flags |= 0x10;
    *(p + 0x33E) = flags;
    reset_record_fields(obj);
    *(u16 *)(p + 0x1D4) = 0;
    *(u32 *)(p + 0x198) = (u32)run_state_handler;
    *(u32 *)(p + 0x1A0) = (u32)enemy_move_axis;
}

ASM_FUNC("asm/code_0814C6D0/hit_active_entry.s", void hit_active_entry(void))

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

ASM_FUNC("asm/code_0814C6D0/fall_landing_entry.s", void fall_landing_entry(void))

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

ASM_FUNC("asm/code_0814C6D0/snap_to_linked.s", void snap_to_linked(void))

ASM_FUNC("asm/code_0814C6D0/random_cell_walk.s", void random_cell_walk(void))

ASM_FUNC("asm/code_0814C6D0/entry_overlap_or_zone.s", void entry_overlap_or_zone(void))

ASM_FUNC("asm/code_0814C6D0/aim_at_entry_move.s", void aim_at_entry_move(void))

ASM_FUNC("asm/code_0814C6D0/entry_overlap_flags_zone.s", void entry_overlap_flags_zone(void))

ASM_FUNC("asm/code_0814C6D0/face_entry.s", void face_entry(void))

ASM_FUNC("asm/code_0814C6D0/ai_face_target_tick.s", void ai_face_target_tick(void))

ASM_FUNC("asm/code_0814C6D0/ai_start_dash.s", void ai_start_dash(void))

ASM_FUNC("asm/code_0814C6D0/ai_dash_step.s", void ai_dash_step(void))

ASM_FUNC("asm/code_0814C6D0/ai_start_hop.s", void ai_start_hop(void))

ASM_FUNC("asm/code_0814C6D0/ai_airborne.s", void ai_airborne(void))

ASM_FUNC("asm/code_0814C6D0/ai_leap_to_target.s", void ai_leap_to_target(void))

ASM_FUNC("asm/code_0814C6D0/ai_hop_fall.s", void ai_hop_fall(void))
