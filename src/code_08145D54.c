#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
    u8 unk21;
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

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

struct Partner
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    u8 filler14[0x242 - 0x14];
    s16 unk242;
    s16 unk244;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x208 - 0x38];
    u8 unk208;
};

struct Field12
{
    u8 b0 : 1;
    u8 mid : 2;
    u8 rest : 5;
};

struct Sensor
{
    u8 filler0[0x8];
    u8 unk8;
    u8 unk9;
};

struct Ai
{
    u8 filler0[0xD0];
    u16 unkD0[4];
    u8 filler1D8[0x14C - 0xD8];
    void *handler_next;
    u8 filler150[0x178 - 0x150];
    u16 unk178;
    u16 unk17A;
    u16 unk17C;
    u8 unk17E;
};

struct StateOut
{
    void *state;
    void *unk4;
    void *unk8;
    void *unkC;
};

struct Enemy
{
    u8 filler0[0x2];
    u8 unk2;
    u8 filler3[0xC - 0x3];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 unk25;
    u8 filler26[0x58 - 0x26];
    u8 filler58[0x128 - 0x58];
    u16 unk128[4];
    u8 filler130[0x198 - 0x130];
    void *unk198;
    void *unk19C;
    void *unk1A0[2];
    u8 filler1A8[0x1B0 - 0x1A8];
    u8 *unk1B0;
    struct Sensor *unk1B4;
    u8 filler1B8[0x1D0 - 0x1B8];
    u16 unk1D0;
    u16 unk1D2;
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 unk1DC;
    s32 unk1E0;
    s32 unk1E4;
    s32 unk1E8;
    u8 filler1EC[0x20C - 0x1EC];
    u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x214 - 0x211];
    u8 unk214;
    u8 unk215;
    u8 unk216;
    u8 filler217[0x242 - 0x217];
    s16 unk242;
    s16 unk244;
    u8 filler246[0x294 - 0x246];
    struct Sprite *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 unk2B6;
    u8 filler2B7[0x2DC - 0x2B7];
    u16 unk2DC;
    u16 unk2DE;
    u8 filler2E0[0x33D - 0x2E0];
    u8 unk33D;
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *body;
};

void run_state_handler(void);
void enemy_push_by_mode(void);
void tick_store(void);
void start_move_to(struct Enemy *ent, s32 x, s32 y, u32 z, u8 a);
void start_z_move(struct Enemy *ent, s32 z, u32 a, u32 b, u32 c);
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void enemy_align_body_to_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_start_guard(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_wander_to_edge(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void obj_box_to_stack(s16 *box, struct Enemy *ent, u32 a, u32 b);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
void sprite_step_e6a0(void *sprite);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void enemy_idle_runner(struct Owner *owner, struct Enemy *ent);
void enemy_wait_b_runner(struct Owner *owner, struct Enemy *ent);
void obj_random_target_b(struct Owner *owner, struct Enemy *ent);
void setup_random_offset_state(struct Owner *owner, struct Enemy *ent);
void load_record_fields(struct Enemy *ent, u32 mode);
void load_record_fields_scaled(struct Enemy *ent, u32 mode);
void load_hit_box_words(struct Enemy *ent, u32 mode);
void set_timer_from_table(struct Enemy *ent, u32 index);
void reset_record_fields(struct Enemy *ent);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void obj_refresh_sub_attr(struct Enemy *ent);
u8 z_approach(struct Enemy *ent);
void obj_drift_with_cap_step(struct Enemy *ent);
u8 step_pos_xyz(struct Enemy *ent);
u8 step_pos_xy_b(struct Enemy *ent);
u8 step_pos_xy_c(struct Enemy *ent);
u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 seed);
u32 angle_of_vector16(s32 x, s32 y);
u8 find_active_entry(struct Owner *owner, u8 *idx);
u8 box_hit_kind(struct Owner *owner, struct Enemy *ent, u32 a, u32 b, u32 c);
// u8 stack argument the original passes unpromoted (strb/ldrb on the stack slot)
struct ByteArg
{
    u8 val;
} __attribute__((packed));

void start_move_to_3d(struct Enemy *ent, s32 x, s32 y, s32 z, u32 arg4, struct ByteArg arg5);

void enemy_wait_sense(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_pick_random_index(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_chase_companion(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_face_idle(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_face_companion(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_start_chase(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_start_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_wait_count(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
u8 companion_in_sight(struct Owner *owner, struct Enemy *ent);
void enemy_start_move_3d(struct Enemy *ent, s32 x, s32 y, s32 z, u32 arg4, struct ByteArg arg5, struct StateOut *state_out, void *handler_next);
void enemy_approach_z(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void body_contact_enemy_reset_idle(struct Owner *owner, struct Enemy *ent);
void enemy_after_contact(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_run_body_handler(struct Owner *owner, struct Enemy *ent);
void enemy_setup_idle(struct Owner *owner, struct Enemy *ent);
void enemy_setup_wait_b(struct Owner *owner, struct Enemy *ent);
void enemy_stop_tracking_b(struct Owner *owner, struct Enemy *ent);
void body_contact_enemy_move_3d_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_seek_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_guard_aim(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_begin_wait(struct Owner *owner, struct Enemy *ent);
void enemy_wait_scan(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_start_run(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_run_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_chase_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_run_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_face_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_align_body_to_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);

ASM_FUNC("asm/code_08145D54/enemy_wait_sense.s", void enemy_wait_sense(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

void enemy_pick_random_index(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 *rec = ent->unk1B0;

    ent->unk1D0 = sub_081DE850(4, mt19937_next());
    ent->unk1D2 = rec[ent->unk1D0 + 0x24];
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ent->sprite->unk21 = 4;
    obj_refresh_sub_attr(ent);
    ent->sprite->unk20 = 0x10;
    state_out->state = enemy_chase_companion;
}

ASM_FUNC("asm/code_08145D54/enemy_chase_companion.s", void enemy_chase_companion(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

void enemy_face_idle(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    s32 cmd;

    ((struct Mode3 *)&ent->unk214)->mode = 4;
    cmd = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (cmd)
    {
    case 0:
        ent->sprite->unk21 = 4;
        break;
    case 3:
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 1;
    case 1:
        ent->sprite->unk21 = 7;
        break;
    case 2:
        ent->sprite->unk21 = 0xA;
        break;
    }
    ((struct Field12 *)&ent->sprite->unk12)->mid = 1;
    obj_refresh_sub_attr(ent);
    state_out->state = enemy_face_companion;
}

void enemy_face_companion(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 idx;
    struct Partner *partner;
    s32 dx;
    s32 dy;
    s32 dir;
    s32 cmd;

    if (companion_in_sight(owner, ent) == 0)
    {
        state_out->state = enemy_push_by_mode;
        return;
    }
    if (find_active_entry(owner, &idx) == 0)
    {
        state_out->state = enemy_push_by_mode;
        return;
    }
    if ((ent->sprite->unk12 & 8) == 0)
    {
        return;
    }
    if (ent->sprite->unk21 == 5 || ent->sprite->unk21 == 8 || ent->sprite->unk21 == 0xB)
    {
        state_out->state = enemy_start_chase;
        ((struct Mode3 *)&ent->unk214)->mode = 0;
        return;
    }
    partner = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
    dx = partner->posX - ent->unkC;
    dy = partner->posY - ent->unk10;
    if (dx | dy)
    {
        dir = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
    }
    else
    {
        dir = -1;
    }
    ((struct Mode3 *)&ent->unk24)->mode = dir;
    ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 0;
    cmd = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (cmd)
    {
    case 0:
        ent->sprite->unk21 = 5;
        break;
    case 3:
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 1;
    case 1:
        ent->sprite->unk21 = 8;
        break;
    case 2:
        ent->sprite->unk21 = 0xB;
        break;
    }
    obj_refresh_sub_attr(ent);
    ((struct Field12 *)&ent->sprite->unk12)->mid = 1;
}

ASM_FUNC("asm/code_08145D54/enemy_start_chase.s", void enemy_start_chase(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

void enemy_start_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai;
    s32 cmd;

    ai = (struct Ai *)&ent->filler58;
    ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 0;
    cmd = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (cmd)
    {
    case 0:
        ent->sprite->unk21 = 3;
        break;
    case 3:
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 1;
    case 1:
        ent->sprite->unk21 = 6;
        break;
    case 2:
        ent->sprite->unk21 = 9;
        break;
    }
    obj_load_record(ent->sprite, ent->sprite->unk21, -1, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ai->unk17A = 0x14;
    state_out->state = enemy_wait_count;
}


void enemy_wait_count(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 *ai = ent->unk1B0;
    s16 box[8];
    u16 arg4;
    struct ByteArg flag;

    ent->unk1D2--;
    if (ent->unk1D2 == 0)
    {
        ((struct Mode3 *)&ent->unk214)->mode = 4;
        obj_set_cell_coord(ent, MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2), 0);
        obj_box_to_stack(box, ent, 0x22, 0);
        ent->unk1D2 = ai[ent->unk1D0 + 0x40];
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            state_out->state = tick_store;
            state_out->unkC = enemy_push_by_mode;
        }
        else
        {
            arg4 = *(u16 *)(ai + ent->unk1D0 * 2 + 0x38);
            ent->sprite->unk20 = ai[ent->unk1D0 + 0x34];
            ((struct Mode3 *)&ent->unk214)->mode = 3;
            flag.val = 0;
            enemy_start_move_3d(ent, ent->unk1DC, ent->unk1E0, ent->unk14, arg4, flag, state_out, tick_store);
            state_out->unkC = enemy_push_by_mode;
        }
    }
}

u8 companion_in_sight(struct Owner *owner, struct Enemy *ent)
{
    u8 idx;
    struct Sensor *sensor = ent->unk1B4;
    struct Partner *party;
    u8 s;

    if (sensor->unk8 == 0)
    {
        return 1;
    }
    {
        u32 k = find_active_entry(owner, &idx) ? idx : ((u32)owner->unk208 << 30) >> 31;
        party = *(struct Partner **)((u8 *)owner + k * 4 + 0x28);
    }
    s = sensor->unk9;
    if (((s ^ 1) & 1) != 0 && party->posY - ent->unk10 <= 0)
    {
        return 0;
    }
    if ((s & 2) == 0 && party->posX - ent->unkC >= 0)
    {
        return 0;
    }
    if ((s & 4) == 0 && party->posY - ent->unk10 >= 0)
    {
        return 0;
    }
    if (s & 8)
    {
        return 1;
    }
    if (party->posX - ent->unkC > 0)
    {
        return 1;
    }
    return 0;
}

void enemy_start_move_3d(struct Enemy *ent, s32 x, s32 y, s32 z, u32 arg4, struct ByteArg arg5, struct StateOut *state_out, void *handler_next)
{
    struct Ai *ai;

    ai = (struct Ai *)&ent->filler58;
    start_move_to_3d(ent, x, y, z, arg4, arg5);
    if ((ent->unk242 | ent->unk244) != 0)
    {
        ent->unk2 = ((u32)(u16)angle_of_vector16(ent->unk242, ent->unk244) + 0x10) >> 8;
    }
    else
    {
        ent->unk2 = 0xff;
    }
    state_out->state = body_contact_enemy_move_3d_step;
    ai->handler_next = handler_next;
}

void enemy_approach_z(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    s32 unused[4];

    ent->unkC += ent->unk242;
    ent->unk10 += ent->unk244;
    if (z_approach(ent) == 0)
    {
        ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
        state_out->state = enemy_start_wait;
    }
}

void body_contact_enemy_reset_idle(struct Owner *owner, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk33E)->bit4 = 1;
    reset_record_fields(ent);
    ent->unk1D4 = 0;
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_push_by_mode;
}

void enemy_after_contact(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Enemy *body = ent->body;

    if ((body->unk20C & 1) == 0)
    {
        state_out->state = 0;
        return;
    }
    if (body->sprite->unk12 & 0x10)
    {
        ((struct Mode3 *)&body->unk214)->mode = 0;
    }
    if ((((u32)body->unk216 << 26) >> 31 | ((u32)body->unk25 << 25) >> 28) != 0)
    {
        ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
        obj_set_sprite_link(body, 0);
        obj_set_active(body, 0);
        state_out->state = 0;
        return;
    }
    obj_drift_with_cap_step(body);
}

void enemy_run_body_handler(struct Owner *owner, struct Enemy *ent)
{
    void **slot = ent->body->unk1A0;
    void (*fn)(struct Owner *, struct Enemy *, void **) = (void (*)(struct Owner *, struct Enemy *, void **))slot[0];

    if (fn != NULL)
    {
        fn(owner, ent, slot);
    }
    if (ent->sprite->unk12 & 8)
    {
        ((struct Mode3 *)&ent->unk214)->mode = 3;
        ent->unk198 = enemy_setup_wait_b;
    }
}

void enemy_setup_idle(struct Owner *owner, struct Enemy *ent)
{
    void (*next)(struct Owner *, struct Enemy *);

    load_record_fields(ent, 0xF);
    ent->unk198 = enemy_idle_runner;
    ent->unk1A0[0] = obj_random_target_b;
    ent->unk1A0[1] = obj_random_target_b;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    next = ent->unk198;
    next(owner, ent);
}

void enemy_setup_wait_b(struct Owner *owner, struct Enemy *ent)
{
    void (*wait)(struct Owner *, struct Enemy *);

    load_record_fields_scaled(ent, 0);
    load_hit_box_words(ent, 0);
    set_timer_from_table(ent, 0xB);
    ent->unk198 = enemy_wait_b_runner;
    ent->unk1A0[0] = setup_random_offset_state;
    ent->unk1A0[1] = setup_random_offset_state;
    wait = enemy_wait_b_runner;
    wait(owner, ent);
}

void enemy_stop_tracking_b(struct Owner *owner, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk33E)->bit4 = 1;
    ((struct Flags8 *)&ent->body->unk20D)->bit2 = 0;
    ((struct Flags8 *)&ent->unk2B6)->bit6 = 1;
    ent->body->unk1A0[0] = 0;
    ((struct Flags8 *)&ent->unk33D)->bit3 = 1;
    enemy_setup_wait_b(owner, ent);
}

void body_contact_enemy_move_3d_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    if (step_pos_xyz(ent) == 0)
    {
        state_out->state = ent->unk1A0[1];
    }
}

ASM_FUNC("asm/code_08145D54/enemy_seek_partner.s", void enemy_seek_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

ASM_FUNC("asm/code_08145D54/enemy_guard_aim.s", void enemy_guard_aim(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

void enemy_begin_wait(struct Owner *owner, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk33E)->bit4 = 1;
    ent->unk1DC = ent->unkC;
    ent->unk1E0 = ent->unk10;
    ent->unk1E4 = ent->unk14;
    ent->unk1E8 = ent->unk18;
    ((struct Flags8 *)&ent->unk33D)->bit3 = 1;
    ent->unk1D0 = sub_081DE850(4, mt19937_next());
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ent->sprite->unk20 = ent->unk1B0[ent->unk1D0 + 6];
    ent->unk1D2 = 0;
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_wait_scan;
}

ASM_FUNC("asm/code_08145D54/enemy_wait_scan.s", void enemy_wait_scan(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

ASM_FUNC("asm/code_08145D54/enemy_start_run.s", void enemy_start_run(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

ASM_FUNC("asm/code_08145D54/enemy_run_step.s", void enemy_run_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

ASM_FUNC("asm/code_08145D54/enemy_chase_partner.s", void enemy_chase_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

ASM_FUNC("asm/code_08145D54/enemy_run_wait.s", void enemy_run_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

void enemy_face_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    u8 *rec = ent->unk1B0;
    u8 idx;
    u32 index;
    struct Partner *partner;
    s32 dx;
    s32 dy;
    s32 dir;
    u32 (*math)(u32, u32);

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->unk208 << 30) >> 31;
    }
    partner = *(struct Partner **)((u8 *)owner + index * 4 + 0x28);
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ent->sprite->unk20 = rec[ai->unk178 + 0x12];
    math = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
    dx = partner->posX - ent->unkC;
    dy = partner->posY - ent->unk10;
    if (dx | dy)
    {
        u16 angle = angle_of_vector16(dx, dy);
        dir = (((s32)(angle + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir = -1;
    }
    {
        u32 facing = math(dir, 2);
        obj_set_cell_coord(ent, facing + 4, 0);
    }
    ai->unk17A = rec[ai->unk178 + 0x16];
    state_out->state = tick_store;
    state_out->unkC = enemy_align_body_to_partner;
}

ASM_FUNC("asm/code_08145D54/enemy_align_body_to_partner.s", void enemy_align_body_to_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

