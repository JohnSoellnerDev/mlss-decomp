#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    s8 unk20;
    u8 unk21;
    u8 filler22;
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    s16 unk2A;
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

struct OwnerFlags
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 unk3 : 5;
};

struct Partner
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x208 - 0x38];
    struct OwnerFlags flags208;
};

struct RecBox
{
    u8 filler0[0x18];
    s8 unk18;
    s8 unk19;
    s8 unk1A;
    s8 unk1B;
    s8 unk1C;
    s8 unk1D;
};

struct SensorBox
{
    u8 filler0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
};

struct WaitScratch
{
    u8 idx;
    u8 pad1[3];
    s16 box[8];
};

struct WalkScratch
{
    s16 box[8];
    u8 idx;
};

struct Enemy
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x198 - 0x25];
    void *unk198;
    void *unk19C;
    void *unk1A0[2];
    u8 filler1A8[0x1B0 - 0x1A8];
    struct RecBox *rec;
    struct SensorBox *sensor;
    s16 unk1B8;
    s16 unk1BA;
    s16 unk1BC;
    u8 filler1BE[0x1D0 - 0x1BE];
    u16 unk1D0;
    u16 unk1D2;
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x214 - 0x211];
    u8 unk214;
    u8 filler215[0x239 - 0x215];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E;
    u8 filler240[0x248 - 0x240];
    s32 unk248;
    u8 filler24C[0x294 - 0x24C];
    struct Sprite *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 unk2B6;
    u8 filler2B7[0x33D - 0x2B7];
    u8 unk33D;
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *body;
};

void enemy_start_random_move(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void enemy_check_flags(struct Owner *owner, struct Enemy *ent);
void enemy_ready_check(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void enemy_engage(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void enemy_setup_wait(struct Owner *owner, struct Enemy *ent);
void obj_state_run_advance(void);
void obj_set_stage(struct Enemy *ent, u32 stage);
void obj_refresh_sub_attr(struct Enemy *ent);
u8 probe_body_hit(struct Owner *owner, struct Enemy *body, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_set_or_add_pos(struct Enemy *ent, s32 x, s32 y, s32 z, u32 a, u32 b);
void enemy_walk(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void collide_react(struct Owner *owner, struct Enemy *ent, struct Partner *bro, u32 speed, u32 a, s32 b, s32 c, u32 d, u32 e);
u16 angle_of_vector16(s32 x, s32 y);
void enemy_wait_spot_owner(struct Owner *owner, struct Enemy *ent);
void enemy_chase_step(void);
void move_step_handler(void);
void obj_random_target_b(void);
void setup_random_offset_state(void);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void load_record_fields(struct Enemy *ent, u32 mode);
void load_hit_box_words(struct Enemy *ent, u32 mode);
void set_timer_from_table(struct Enemy *ent, u32 index);
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void flags_obj_set_mode(struct Enemy *ent, u32 mode);
void obj_move_interp(struct Enemy *ent);
u8 advance_move_step(struct Owner *owner, struct Enemy *ent);
void obj_start_move_quarter(struct Enemy *ent, u32 zero, s32 x, s32 y);
void enemy_contact_engage(struct Owner *owner, struct Enemy *ent);
void enemy_start_move(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void reset_record_fields(struct Enemy *ent);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void run_state_handler(void);
void enemy_enter_queued_state(struct Owner *owner, struct Enemy *ent);
void enemy_chase_owner_step(struct Owner *owner, struct Enemy *ent);
void enemy_setup_idle(void);
void enemy_wait_timer(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void load_box_oriented(s16 *box, struct Enemy *ent, u32 a, u32 b);
u8 step_pos_xy(struct Enemy *ent);
u32 mt19937_next(void);
u32 sub_081DE850(u32 range, u32 rnd);
void tick_store(void);
void enemy_idle_runner(void);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
u8 boxes_overlap_oriented(s16 *box, struct Enemy *ent, void *entry, s32 zAbove, s32 zBelow);

ASM_FUNC("asm/code_081442FC/enemy_start_random_move.s", void enemy_start_random_move(struct Owner *owner, struct Enemy *ent, u32 *state_out))

ASM_FUNC("asm/code_081442FC/enemy_wait_spot_owner.s", void enemy_wait_spot_owner(struct Owner *owner, struct Enemy *ent))

ASM_FUNC("asm/code_081442FC/enemy_chase_owner_step.s", void enemy_chase_owner_step(struct Owner *owner, struct Enemy *ent))

void enemy_go_dormant(struct Owner *owner, struct Enemy *ent)
{
    struct Enemy *body = ent->body;

    {
        u8 *p = &ent->unk33E;
        *p = *p | 0x10;
    }
    reset_record_fields(ent);
    ent->unk1D4 = 0;
    {
        int b = ent->unk33D;
        b = b | 8;
        ent->unk33D = b;
    }
    ((struct Flags8 *)&body->unk20D)->bit2 = 0;
    ((struct Mode3 *)&body->unk214)->mode = 0;
    ((struct Flags8 *)&body->unk210)->bit3 = 0;
    {
        int b = body->unk20D;
        b = b | 8;
        b = b & -33;
        b = b & -65;
        body->unk20D = b;
    }
    *(u32 *)((u8 *)body + 0x20C) = (*(u32 *)((u8 *)body + 0x20C) & 0xFFFC7FFF) | 0x8000;
    {
        u8 *p = &body->unk20F;
        *p = *p | 4;
    }
    obj_set_sprite_link(body, 0);
    obj_set_active(body, 0);
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_start_move;
}

ASM_FUNC("asm/code_081442FC/enemy_start_move.s", void enemy_start_move(struct Owner *owner, struct Enemy *ent, u32 *state_out))

ASM_FUNC("asm/code_081442FC/enemy_walk.s", void enemy_walk(struct Owner *owner, struct Enemy *ent, u32 *state_out))

ASM_FUNC("asm/code_081442FC/enemy_wait_timer.s", void enemy_wait_timer(struct Owner *owner, struct Enemy *ent, u32 *state_out))

ASM_FUNC("asm/code_081442FC/enemy_approach_bro.s", void enemy_approach_bro(struct Owner *owner, struct Enemy *ent, u32 *state_out))

ASM_FUNC("asm/code_081442FC/enemy_chase_step.s", void enemy_chase_step(void))

ASM_FUNC("asm/code_081442FC/enemy_move_body.s", void enemy_move_body(void))

void enemy_ready_check(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    if (ent->sprite->unk23 == 0)
    {
        if (ent->sprite->unk2A <= ent->sprite->unk20)
        {
            *state_out = (u32)enemy_chase_step;
        }
    }
}

void enemy_engage(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    u8 idx;

    if (find_active_entry(owner, &idx) == 0)
    {
        idx = owner->flags208.bit2;
    }
    {
        struct Partner *bro = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
        obj_start_move_quarter(ent, 0, bro->posX, bro->posY);
    }
    obj_move_interp(ent);
    ((struct Flags8 *)&ent->unk1D6)->bit1 = 1;
    ((struct Flags8 *)&ent->unk1D6)->bit0 = 0;
    *state_out = (u32)move_step_handler;
}

void enemy_enter_queued_state(struct Owner *owner, struct Enemy *ent)
{
    void (*next)(struct Owner *, struct Enemy *);

    load_record_fields(ent, 0xF);
    ent->unk198 = ent->unk19C;
    ent->unk1A0[0] = obj_random_target_b;
    ent->unk1A0[1] = obj_random_target_b;
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    next = ent->unk198;
    next(owner, ent);
}

void enemy_setup_wait(struct Owner *owner, struct Enemy *ent)
{
    void (*wait)(struct Owner *, struct Enemy *);

    load_record_fields(ent, 0);
    load_hit_box_words(ent, 0);
    set_timer_from_table(ent, 0xB);
    ent->unk198 = (void *)enemy_wait_spot_owner;
    ent->unk1A0[0] = setup_random_offset_state;
    ent->unk1A0[1] = setup_random_offset_state;
    wait = enemy_wait_spot_owner;
    wait(owner, ent);
}

void enemy_check_flags(struct Owner *owner, struct Enemy *ent)
{
    if (advance_move_step(owner, ent) == 1)
    {
        obj_move_interp(ent);
    }
    else
    {
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
        ((struct Flags8 *)&ent->unk20D)->bit3 = 1;
        ((struct Flags8 *)&ent->unk210)->bit3 = 1;
        ((struct Flags8 *)&ent->unk210)->bit6 = 1;
    }
}

void enemy_end_countdown(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    if (ent->unk18 <= 0)
    {
        ent->unk18 = 0;
        ((struct Flags8 *)&ent->unk20E)->bit2 = 0;
        obj_set_cell_coord(ent, 6, 0);
        flags_obj_set_mode(ent, 0x10);
        ((struct Mode3 *)&ent->unk214)->mode = 1;
        *state_out = (u32)enemy_start_random_move;
    }
}

void enemy_stop_tracking(struct Owner *owner, struct Enemy *ent)
{
    {
        u8 *p = &ent->unk33E;
        *p = *p | 0x10;
    }
    obj_set_cell_coord(ent, 5, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    ((struct Flags8 *)&ent->unk210)->bit3 = 0;
    ((struct Flags8 *)&ent->unk210)->bit6 = 0;
    ((struct Flags8 *)&ent->unk2B6)->bit6 = 1;
    enemy_contact_engage(owner, ent);
}

ASM_FUNC("asm/code_081442FC/enemy_wait_b_runner.s", void enemy_wait_b_runner(struct Owner *owner, struct Enemy *ent))

ASM_FUNC("asm/code_081442FC/enemy_idle_runner.s", void enemy_idle_runner(void))

ASM_FUNC("asm/code_081442FC/enemy_idle_update.s", void enemy_idle_update(void))

ASM_FUNC("asm/code_081442FC/enemy_timer_step.s", void enemy_timer_step(void))

ASM_FUNC("asm/code_081442FC/enemy_body_contact.s", void enemy_body_contact(void))

ASM_FUNC("asm/code_081442FC/enemy_push_by_mode.s", void enemy_push_by_mode(void))
