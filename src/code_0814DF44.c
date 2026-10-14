#include "global.h"
#include "ram_routines.h"

struct Sensor
{
    u8 filler0[0x8];
    s8 flag8;
    s8 kind;
};

struct Sprite
{
    u8 filler0[0x11];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 anim;
    u8 record;
    u8 filler22;
    u8 state;
    u8 filler24[0x2A - 0x24];
    s16 timer;
};

struct FlagBits
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

struct StateBits
{
    u8 low3 : 3;
    u8 high5 : 5;
};

struct SpriteFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct CompanionObj
{
    u8 filler0[0x2];
    u8 angle;
    u8 filler3[0xC - 0x3];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x28 - 0x1C];
    struct CompanionObj *entries[2];
    u8 filler30[0x100 - 0x30];
    s16 tbl[0x18];
    u8 filler130[0x198 - 0x130];
    u32 altHandler;
    u8 filler19C[0x1A0 - 0x19C];
    u32 handler;
    u32 handler1A4;
    u32 handler1A8;
    u32 nextHandler;
    u8 *animTable;
    struct Sensor *sensor;
    u8 filler1B8[0x1BE - 0x1B8];
    s16 variant;
    u8 filler1C0[0x1D0 - 0x1C0];
    u16 index1D0;
    u16 timer;
    u8 filler1D4[0x1D6 - 0x1D4];
    u8 flags1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 homeX;
    s32 homeY;
    u8 filler1E4[0x20C - 0x1E4];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 filler210[0x214 - 0x210];
    u8 state;
    u8 filler215[0x272 - 0x215];
    u16 size272;
    u16 size274;
    u16 size276;
    u16 size278;
    u16 size27A;
    u16 size27C;
    u8 filler27E[0x294 - 0x27E];
    struct Sprite *sprite;
    u8 filler298[0x33D - 0x298];
    u8 flags33D;
    u8 flags33E;
    u8 filler33F[0x344 - 0x33F];
    struct CompanionObj *child;
    u8 filler348[0x352 - 0x348];
    u8 flags352;
};

u32 mt19937_next(void);
u32 sub_081DE850(u32 range, u32 rnd);
u32 point_in_box(void *box, struct CompanionObj *obj, u32 limit, u32 zero);
void obj_box_to_stack(void *box, struct CompanionObj *obj, u32 extent, u32 zero);
u8 step_pos_xy_plain(struct CompanionObj *obj);
u8 step_pos_xy_c(struct CompanionObj *obj);
void obj_load_record(struct Sprite *spr, u32 record, u32 loadFlags, u32 arg3, u32 arg4);
void wander_to_edge(struct CompanionObj *obj, u32 a, u32 b);
void wander_diag(struct CompanionObj *obj, u32 a, u32 b);
void reset_record_fields(struct CompanionObj *obj);
void snap_to_linked(struct CompanionObj *obj);
void run_state_handler(void);
void face_entry(void);
void random_cell_walk(void);
void pick_next_handler(struct CompanionObj *parent, struct CompanionObj *obj, u32 *slot);
void ai_choose_move_target(void);
void ai_hold_at_home(void);
void run_e93c_then_handler(void *parent, struct CompanionObj *obj, u32 *slot);
u32 cpu_set(void *src, void *dst, u32 control);
void sprite_step_e6a0(struct Sprite *spr);
void tick_store(void);
void obj_load_anim_table(struct CompanionObj *obj);
void ai_seek_target(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot);
void ai_check_flags_c0(void *parent, struct CompanionObj *obj, u32 *slot);
u8 find_active_entry(struct CompanionObj *owner, u8 *idx);
u8 obj_flags_2b5_30_set(void *obj);
u8 obj_flags_2b5_c0_set(void *obj);
void obj_set_motion_a_08046AA4(void *obj, u32 a, u32 b, u32 c, u32 d);
void obj_set_motion_b(void *obj, u32 a, u32 b, u32 c, u32 d);
void obj_advance_axes(void *obj);
void obj_clear_flags_2b5_30(void *obj);
void obj_clear_flags_2b5_c0(void *obj);
void obj_set_sprite_link(void *obj, u32 v);
void obj_roll_spawn(void);
void obj_load_kind_record(void);
void random_pose(void);
void entry_switch_handler(void);
u32 entry_within_reach(struct CompanionObj *obj, struct CompanionObj *other);

void obj_enter_ai(void *parent, struct CompanionObj *obj)
{
    ((struct FlagBits *)&obj->flags33E)->bit4 = 0;
    obj_load_anim_table(obj);
    obj_load_record(obj->sprite, 9, -1, 0, 0);
    obj_set_motion_a_08046AA4(obj, 2, 0, 0x10, 1);
    obj_set_motion_b(obj, 2, 0, 0x10, 1);
    obj_advance_axes(obj);
    obj_clear_flags_2b5_30(obj);
    obj_clear_flags_2b5_c0(obj);
    obj_set_sprite_link(obj, 0);
    ((struct FlagBits *)&obj->flags20C)->bit7 = 0;
    ((struct FlagBits *)&obj->flags20C)->bit6 = 0;
    ((struct FlagBits *)&obj->flags20C)->bit5 = 0;
    ((struct FlagBits *)&obj->flags20D)->bit2 = 0;
    obj->altHandler = (u32)run_state_handler;
    obj->handler = (u32)ai_seek_target;
}

void ai_seek_target(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot)
{
    u8 box[0x10];
    u8 idx;
    struct CompanionObj *target;
    u8 *p;
    u16 anim;

    if (find_active_entry(owner, &idx) != 0)
    {
        target = (struct CompanionObj *)((u32 *)owner)[idx + 10];
        obj_box_to_stack(box, obj, 0x22, 0x10);
        if ((u8)point_in_box(box, target, 0x7FFF, 0) != 0)
        {
            u32 roll;
            u32 step;

            obj_set_sprite_link(obj, 1);
            roll = sub_081DE850(4, mt19937_next());
            p = (u8 *)(roll + (u32)obj->animTable) + 0x20;
            step = 0x100;
            anim = *(s8 *)p;
            obj_set_motion_a_08046AA4(obj, 2, 0, step, anim);
            obj_set_motion_b(obj, 2, 0, step, anim);
            obj_advance_axes(obj);
            *slot = (u32)ai_check_flags_c0;
        }
    }
}

void ai_check_flags_c0(void *parent, struct CompanionObj *obj, u32 *slot)
{
    struct FlagBits *fb;
    u8 set;
    u8 flags;
    u32 bit2;
    u32 bit3;

    set = obj_flags_2b5_30_set(obj);
    if (set == 0)
    {
        obj_clear_flags_2b5_30(obj);
        ((struct FlagBits *)&obj->flags1D6)->bit2 = 1;
    }
    set = obj_flags_2b5_c0_set(obj);
    fb = (struct FlagBits *)&obj->flags1D6;
    if (set == 0)
    {
        obj_clear_flags_2b5_c0(obj);
        fb->bit3 = 1;
    }
    flags = obj->flags1D6;
    bit2 = ((u32)flags << 29) >> 31;
    bit3 = ((u32)flags << 28) >> 31;
    if (bit3 & bit2)
    {
        ((struct FlagBits *)&obj->flags20C)->bit5 = 1;
        ((struct FlagBits *)&obj->flags20C)->bit6 = 1;
        ((struct FlagBits *)&obj->flags20C)->bit7 = 1;
        ((struct FlagBits *)&obj->flags20D)->bit2 = 1;
        fb->bit3 = 0;
        fb->bit2 = 0;
        obj->size272 = 0xE00;
        obj->size274 = 0x100;
        obj->size276 = 0x800;
        *slot = (u32)ai_choose_move_target;
    }
}

ASM_FUNC("asm/code_0814DF44/ai_choose_move_target.s", void ai_choose_move_target(void))

ASM_FUNC("asm/code_0814DF44/ai_move_or_return.s", void ai_move_or_return(void))

ASM_FUNC("asm/code_0814DF44/ai_aim_and_strike.s", void ai_aim_and_strike(void))

void ai_set_box_for_state(void *parent, struct CompanionObj *obj, u32 *slot)
{
    u8 st;

    st = obj->sprite->state;
    if (st > 1)
    {
        obj->size272 = 0xC00;
        obj->size274 = 0xC00;
        obj->size276 = 0x800;
    }
    else if (st != 0)
    {
        obj->size272 = 0xC00;
        obj->size274 = 0x800;
        obj->size276 = 0x800;
    }
    if ((obj->sprite->flags & 8) != 0)
    {
        ((struct StateBits *)&obj->state)->low3 = 0;
        *slot = (u32)run_e93c_then_handler;
    }
}

ASM_FUNC("asm/code_0814DF44/ai_hold_at_home.s", void ai_hold_at_home(void))

void obj_load_anim_table(struct CompanionObj *obj)
{
    s16 *tbl;
    s8 *p;

    tbl = obj->tbl;
    p = (s8 *)obj->animTable;
    cpu_set(p, tbl, 4);
    p += 8;
    cpu_set(p, &obj->tbl[4], 4);
    p += 8;
    tbl[8] = p[0];
    tbl[9] = p[1];
    p += 2;
    tbl[10] = p[0];
    p++;
    tbl[11] = p[0];
    p++;
    tbl[12] = p[0];
    p++;
    tbl[13] = p[0];
    p++;
    tbl[14] = p[0];
    p++;
    tbl[15] = p[0];
    p++;
    tbl[16] = p[0];
    p++;
    tbl[17] = p[0];
    p++;
    tbl[18] = p[0];
    p++;
    tbl[19] = p[0];
    p++;
    tbl[20] = p[0];
    p++;
    tbl[21] = p[0];
    p++;
    tbl[22] = p[0];
    p++;
    tbl[23] = p[0];
}

ASM_FUNC("asm/code_0814DF44/ai_set_facing_from_velocity.s", void ai_set_facing_from_velocity(void))

void ai_reload_sprite_mirrored(void *parent, struct CompanionObj *obj)
{
    struct Sprite **sprp;
    u32 zero;

    sprp = &obj->sprite;
    zero = 0;
    obj_load_record(*sprp, (*sprp)->record, 5, 0, zero);
    ((struct StateBits *)&obj->state)->low3 = 1;
    ((struct SpriteFlagBits *)&(*sprp)->flags)->mode = 1;
    (*sprp)->anim = -(*sprp)->anim;
}

ASM_FUNC("asm/code_0814DF44/ai_set_anim_by_mode.s", void ai_set_anim_by_mode(void))

void ai_wander_random(void *parent, struct CompanionObj *obj, u32 *slot)
{
    if ((sub_081DE850(100, mt19937_next()) & 1) != 0)
        wander_to_edge(obj, 0x10, 0x100);
    else
        wander_diag(obj, 0x10, 0x100);
    ((struct StateBits *)&obj->state)->low3 = 1;
    *slot = (u32)pick_next_handler;
}

u32 obj_inside_area(struct CompanionObj *obj)
{
    u8 box[0x10];
    u32 inside;

    obj_box_to_stack(box, obj, 0x22, 0);
    inside = point_in_box(box, obj, 0x7FFF, 0);
    if ((u8)inside != 0)
        inside = 1;
    return inside;
}

void run_e93c_then_handler(void *parent, struct CompanionObj *obj, u32 *slot)
{
    ai_reload_sprite_mirrored(parent, obj);
    *slot = (u32)ai_hold_at_home;
}

void step_xy_or_handler(void *parent, struct CompanionObj *obj, u32 *slot)
{
    if (step_pos_xy_plain(obj) == 0)
        *slot = (u32)ai_choose_move_target;
}

void entity_mode_42_noop(void)
{
}

void setup_linked_d3cd(void *parent, struct CompanionObj *obj)
{
    ((struct FlagBits *)&obj->flags33E)->bit4 = 1;
    reset_record_fields(obj);
    ((struct FlagBits *)&obj->flags33D)->bit3 = 1;
    snap_to_linked(obj);
    obj->altHandler = (u32)run_state_handler;
    obj->handler = (u32)face_entry;
    obj->handler1A4 = (u32)face_entry;
    obj->handler1A8 = (u32)face_entry;
}

void setup_linked_ced5(void *parent, struct CompanionObj *obj)
{
    ((struct FlagBits *)&obj->flags33E)->bit4 = 1;
    reset_record_fields(obj);
    ((struct FlagBits *)&obj->flags33D)->bit3 = 1;
    snap_to_linked(obj);
    obj->altHandler = (u32)run_state_handler;
    obj->handler = (u32)random_cell_walk;
    obj->handler1A4 = (u32)random_cell_walk;
    obj->handler1A8 = (u32)random_cell_walk;
}

void pick_next_handler(struct CompanionObj *parent, struct CompanionObj *obj, u32 *slot)
{
    if ((parent->entries[0]->flags20C & 1) != 0 && (parent->entries[1]->flags20C & 1) != 0)
        *slot = (u32)ai_choose_move_target;
    else if (step_pos_xy_c(obj) == 0)
        *slot = (u32)ai_wander_random;
}

ASM_FUNC("asm/code_0814DF44/enter_owner_follow.s", void enter_owner_follow(void))

ASM_FUNC("asm/code_0814DF44/companion_chase_tick.s", void companion_chase_tick(void))

ASM_FUNC("asm/code_0814DF44/entry_switch_handler.s", void entry_switch_handler(void))

ASM_FUNC("asm/code_0814DF44/random_pose.s", void random_pose(void))

ASM_FUNC("asm/code_0814DF44/follow_entry_axis.s", void follow_entry_axis(void))

ASM_FUNC("asm/code_0814DF44/load_anim_tables.s", void load_anim_tables(struct CompanionObj *obj))

void enter_idle_wait(struct CompanionObj *obj, u32 *slot)
{
    struct Sprite **sprp;
    s16 *tbl;

    tbl = obj->tbl;
    ((struct FlagBits *)&obj->flags20D)->bit2 = 1;
    sprp = &obj->sprite;
    ((struct SpriteFlagBits *)&(*sprp)->flags)->mode = 0;
    ((struct StateBits *)&obj->state)->low3 = 1;
    obj_load_record(*sprp, obj->variant, -1, 0, 0);
    sprite_step_e6a0(*sprp);
    obj->timer = ((u16 *)tbl)[sub_081DE850(4, mt19937_next()) + 20];
    *slot = (u32)tick_store;
    slot[3] = (u32)entry_switch_handler;
}

void init_size_and_pose(struct CompanionObj *obj)
{
    struct Sprite **sprp;

    ((struct StateBits *)&obj->state)->low3 = 4;
    sprp = &obj->sprite;
    (*sprp)->anim = 0x10;
    obj_load_record(*sprp, 8, -1, 0, 0);
    sprite_step_e6a0(*sprp);
    obj->size278 = 0x800;
    obj->size272 = 0x800;
    obj->size27A = 0x600;
    obj->size274 = 0x600;
    obj->size27C = 0x600;
    obj->size276 = 0x600;
    ((struct FlagBits *)&obj->flags20D)->bit3 = 1;
    ((struct FlagBits *)&obj->flags20D)->bit5 = 0;
    ((struct FlagBits *)&obj->flags20D)->bit2 = 0;
    ((struct FlagBits *)&obj->flags20E)->bit2 = 0;
    ((struct FlagBits *)&obj->flags20F)->bit2 = 1;
}

ASM_FUNC("asm/code_0814DF44/spawn_random_offset.s", void spawn_random_offset(void))

ASM_FUNC("asm/code_0814DF44/box_fx8_div_and_dispatch.s", void box_fx8_div_and_dispatch(void))

ASM_FUNC("asm/code_0814DF44/entry_within_reach.s", u32 entry_within_reach(struct CompanionObj *obj, struct CompanionObj *other))

ASM_FUNC("asm/code_0814DF44/zone_move_handler.s", void zone_move_handler(void))
