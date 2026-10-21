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

struct SpriteMode12
{
    u8 bit0 : 1;
    u8 mode : 2;
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
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x239 - 0x217];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    u16 unk242;
    u16 unk244;
    u8 filler246[0x248 - 0x246];
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
struct Partner *owner_entry(struct Owner *owner, u16 index);
void obj_start_move_zero_anchor(struct Enemy *obj, u32 one, u32 a, u32 b);
void obj_drift_with_cap_step(struct Enemy *ent);
void enemy_setup_wait_b(struct Owner *owner, struct Enemy *ent);
void enemy_after_contact(void);
extern const u8 gFrameAttrTable[];
void reset_record_fields(struct Enemy *ent);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void run_state_handler(void);
void enemy_enter_queued_state(struct Owner *owner, struct Enemy *ent);
void enemy_chase_owner_step(struct Owner *owner, struct Enemy *ent);
void enemy_setup_idle(void);
void enemy_wait_timer(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void enemy_approach_bro(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void load_box_oriented(s16 *box, struct Enemy *ent, u32 a, u32 b);
u8 step_pos_xy(struct Enemy *ent);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void tick_store(void);
void enemy_idle_runner(struct Owner *owner, struct Enemy *ent);
void enemy_idle_update(void);
void obj_find_active_state(void);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
u8 boxes_overlap_oriented(s16 *box, struct Enemy *ent, void *entry, s32 zAbove, s32 zBelow);

void enemy_start_random_move(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    struct Sprite **spr = &ent->sprite;

    if (((*spr)->flags12 & 8) != 0)
    {
        u8 *table = (u8 *)ent + 0x100;
        struct SensorBox *sensor = ent->sensor;
        struct Sprite *s;
        u32 roll;
        u32 off;
        u8 *kinds;
        u16 kind;

        ((struct Mode3 *)&ent->unk214)->mode = 3;
        roll = rand_below(4, mt19937_next());
        s = *spr;
        off = (u16)roll << 1;
        kinds = (u8 *)ent + 0x108;
        kind = *(u16 *)(kinds + off);
        ent->unk239 = kind;
        s->unk20 = kind;
        ent->unk23E = *(u16 *)(table + off);
        ((struct Flags8 *)&ent->unk20C)->bit5 = 1;
        ((struct Flags8 *)&ent->unk20C)->bit7 = 1;
        ((struct Flags8 *)&ent->unk20C)->bit6 = 1;
        ((struct Flags8 *)&ent->unk210)->bit4 = 1;
        {
            s32 sx = sensor->unk18;
            ent->unk1B8 = sx << 4;
        }
        {
            s32 sy = sensor->unk1A;
            ent->unk1BA = sy << 4;
        }
        {
            s32 sz = sensor->unk1C;
            ent->unk1BC = sz << 4;
        }
        obj_start_move_quarter(ent, 1, ent->unk1B8 << 8, ent->unk1BA << 8);
        obj_move_interp(ent);
        *state_out = (u32)enemy_check_flags;
    }
}

void enemy_wait_spot_owner(struct Owner *owner, struct Enemy *ent)
{
    u8 *slot = (u8 *)ent + 0x1A0;
    void (*fn)(struct Owner *, struct Enemy *, u8 *) = *(void (**)(struct Owner *, struct Enemy *, u8 *))slot;
    u8 idx;

    fn(owner, ent, slot);
    if (ent->unk1D4 == 0)
    {
        if (find_active_entry(owner, &idx) != 0)
        {
            struct SensorBox *sensor = ent->sensor;
            void *entry = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
            s16 box[8];
            s32 sv;

            box[0] = ((sv = sensor->unk10) << 4) + 8;
            box[3] = ((sv = sensor->unk12) << 4) + 8;
            box[1] = ((sv = sensor->unk14) << 4) + 8;
            box[2] = ((sv = sensor->unk16) << 4) + 8;
            if (point_in_box(box, entry, 0x7FFF, 0) != 0)
            {
                struct RecBox *rec = ent->rec;

                box[0] = rec->unk18 << 4;
                box[2] = rec->unk19 << 4;
                box[4] = rec->unk1A << 4;
                box[1] = rec->unk1B << 4;
                box[3] = rec->unk1C << 4;
                box[5] = rec->unk1D << 4;
                if (boxes_overlap_oriented(box, ent, entry, box[4], box[5]) != 0)
                {
                    ent->unk198 = (void *)enemy_enter_queued_state;
                    ent->unk19C = (void *)enemy_chase_owner_step;
                }
            }
        }
    }
    else
    {
        ent->unk1D4--;
    }
}

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

NONMATCH("asm/code_081442FC/enemy_start_move.s", void enemy_start_move(struct Owner *owner, struct Enemy *ent, u32 *state_out))
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *table = (u8 *)ent + 0x100;
    u8 side;
    u8 idx;
    u8 active;
    struct Partner *bro;
    u32 speed;
    u32 off;
    u32 slot;
    s32 dir;
    u32 cell;

    side = rand_below(4, mt19937_next());
    if (find_active_entry(owner, &idx) != 0)
    {
        active = idx;
    }
    else
    {
        active = owner->flags208.bit1;
    }
    bro = *(struct Partner **)((u8 *)owner + active * 4 + 0x28);
    slot = rand_below(2, mt19937_next());
    speed = *(u16 *)(table + (slot << 1) + 0x30);
    off = side << 1;
    collide_react(owner, ent, bro, speed, *(u16 *)(table + off), *(s8 *)(table + 0x40), *(s8 *)(table + 0x41), 0x22, 0);
    if ((ent->unk242 | ent->unk244) != 0)
    {
        dir = (((s32)((u16)angle_of_vector16((s16)ent->unk242, (s16)ent->unk244) + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir = -1;
    }
    ((struct Mode3 *)&ent->unk24)->mode = dir;
    ((struct Mode3 *)&ent->unk214)->mode = 3;
    {
        int b = ent->sprite->flags12;

        b = b & -7;
        ent->sprite->flags12 = b;
    }
    ent->sprite->unk20 = *(u16 *)(table + off + 8);
    cell = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    obj_set_cell_coord(ent, cell, 0);
    *(u16 *)(ai + 0x178) = side;
    *state_out = (u32)enemy_walk;
}
END_NONMATCH

void enemy_walk(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct RecBox *rec = ent->rec;

    if (ent->unk1D4 == 0)
    {
        s16 box[8];
        u8 idx;
        u16 v;

        if (find_active_entry(owner, &idx) != 0)
        {
            struct Partner *bro = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);

            load_box_oriented(box, ent, 0x11, 0x1C);
            if (boxes_overlap_oriented(box, ent, bro, box[4], box[5]) != 0)
            {
                ent->unk1D2 = *(u8 *)(ent->unk1D0 + (u32)rec + 0x26);
                ent->sprite->unk20 = *(u8 *)(ent->unk1D0 + (u32)rec + 0x22);
                ((struct Mode3 *)&ent->unk214)->mode = 1;
                *state_out = (u32)enemy_wait_timer;
                return;
            }
        }
        v = *(u16 *)((u8 *)rec + 0x1A);
        *(u16 *)(ai + 0x17C) = v;
    }
    else
    {
        ent->unk1D4--;
    }
    if (step_pos_xy(ent) == 0)
    {
        u32 pick = rand_below(4, mt19937_next());
        u32 off = pick << 1;
        u8 *cell = ai + 0xD0;
        u16 v = *(u16 *)(cell + off);

        *(u16 *)(ai + 0x17A) = v;
        *state_out = (u32)tick_store;
        state_out[3] = (u32)enemy_start_move;
    }
}

NONMATCH("asm/code_081442FC/enemy_wait_timer.s", void enemy_wait_timer(struct Owner *owner, struct Enemy *ent, u32 *state_out))
{
    u8 *table = (u8 *)ent + 0x100;
    u8 idx;

    if (find_active_entry(owner, &idx) == 0)
    {
        u32 pick = rand_below(4, mt19937_next());
        u16 v = *(u16 *)(table + (pick << 1) + 0x28);

        ent->unk1D2 = v;
        *state_out = (u32)tick_store;
        state_out[3] = (u32)enemy_start_move;
    }
    else
    {
        s32 left = ent->unk1D2 - 1;

        ent->unk1D2 = left;
        if ((u16)left == 0)
        {
            ent->unk1D0 = rand_below(4, mt19937_next());
            *state_out = (u32)enemy_approach_bro;
        }
        else
        {
            struct Partner *bro = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
            s32 (*div)(s32, s32) = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
            s32 dx = bro->posX - ent->posX;
            s32 dy = bro->posY - ent->posY;
            s32 dir;
            s32 cell;

            if ((dx | dy) != 0)
            {
                dir = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
            }
            else
            {
                dir = -1;
            }
            idx = div(dir, 2);
            cell = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
            if (cell != idx)
            {
                obj_set_cell_coord(ent, idx, ent->sprite->unk23);
            }
        }
    }
}
END_NONMATCH

NONMATCH("asm/code_081442FC/enemy_approach_bro.s", void enemy_approach_bro(struct Owner *owner, struct Enemy *ent, u32 *state_out))
{
    u8 *hit = (u8 *)ent->rec;
    u8 *ai = (u8 *)ent + 0x58;
    struct Enemy *body = ent->body;
    u8 idx;
    u8 active;
    struct Partner *bro;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;

    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
    obj_set_or_add_pos(body, ent->posX, ent->posY, ent->unk14 + ent->unk18 + 0x1800, 1, 0);
    if (probe_body_hit(owner, body, body->posX, body->posY, body->unk14 + body->unk18, 1, 0) != 0)
    {
        enemy_start_move(owner, ent, state_out);
        return;
    }
    if (find_active_entry(owner, &idx) != 0)
    {
        active = idx;
    }
    else
    {
        active = owner->flags208.bit1;
    }
    bro = *(struct Partner **)((u8 *)owner + active * 4 + 0x28);
    dx = bro->posX - ent->posX;
    dy = bro->posY - ent->posY;
    ent->unk248 = *(u16 *)(hit + (*(u16 *)(ai + 0x178) << 1) + 0x36);
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
        ent->sprite->unk21 = 4;
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = (u32)dx >> 31;
    }
    else
    {
        ent->sprite->unk21 = (dy < 0) ? 3 : 5;
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 0;
    }
    obj_refresh_sub_attr(ent);
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ((struct SpriteMode12 *)&ent->sprite->flags12)->mode = 1;
    ent->sprite->unk20 = *(u8 *)(hit + *(u16 *)(ai + 0x178) + 0x2A);
    *state_out = (u32)enemy_ready_check;
}
END_NONMATCH

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

NONMATCH("asm/code_081442FC/enemy_idle_runner.s", void enemy_idle_runner(struct Owner *owner, struct Enemy *ent))
{
    u8 sel;
    u8 *ai = (u8 *)ent + 0x58;
    u8 *body_ai = (u8 *)ent->body + 0x58;
    struct Partner *entry;
    s32 dx;
    s32 dy;
    s32 dir;
    void (*fn)(struct Owner *, struct Enemy *, u8 *);
    u8 *slot;
    u8 *body_slot;

    if (find_active_entry(owner, &sel) == 0)
    {
        sel = owner->flags208.bit2;
    }
    entry = *(struct Partner **)((u8 *)owner + sel * 4 + 0x28);
    dx = entry->posX - ent->posX;
    dy = entry->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        dir = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir = 0xFFFF;
    }
    if (dir != (s32)(((u32)ent->unk24 << 29) >> 29))
    {
        obj_set_stage(ent, (u8)dir);
    }
    slot = ai + 0x148;
    fn = *(void (**)(struct Owner *, struct Enemy *, u8 *))slot;
    fn(owner, ent, slot);
    body_slot = body_ai + 0x148;
    fn = *(void (**)(struct Owner *, struct Enemy *, u8 *))body_slot;
    if (fn != 0)
    {
        fn(owner, ent, body_slot);
    }
    if ((ai[0x17E] & 1) != 0)
    {
        s8 *rec = *(s8 **)(ai + 0x158);
        s16 box[8];

        box[0] = rec[0x34] << 4;
        box[2] = rec[0x35] << 4;
        box[4] = rec[0x36] << 4;
        box[1] = rec[0x37] << 4;
        box[3] = rec[0x38] << 4;
        box[5] = rec[0x39] << 4;
        if (boxes_overlap_oriented(box, ent, entry, box[4], box[5]) != 0)
        {
            *(void **)(ai + 0x140) = (void *)enemy_idle_update;
            *(void **)slot = (void *)obj_find_active_state;
            *(void **)(ai + 0x14C) = (void *)obj_find_active_state;
        }
        else
        {
            ((struct Mode3 *)&ent->unk214)->mode = 3;
            *(void **)(ai + 0x140) = (void *)enemy_setup_wait_b;
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_081442FC/enemy_idle_update.s", void enemy_idle_update(void))

ASM_FUNC("asm/code_081442FC/enemy_timer_step.s", void enemy_timer_step(void))

void enemy_body_contact(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    struct Enemy *body = ent->body;
    s32 kind;
    u32 lim_a;
    u32 lim_b;

    owner_entry(owner, 0xFF);
    if ((body->unk20C & 1) == 0)
    {
        *state_out = 0;
        return;
    }
    if (probe_body_hit(owner, body, body->posX, body->posY, body->unk14 + body->unk18, 1, 0) != 0)
    {
        obj_set_sprite_link(body, 0);
        obj_set_active(body, 0);
        ((struct Mode3 *)&ent->unk214)->mode = 3;
        ent->unk198 = (void *)enemy_setup_wait_b;
        *state_out = 0;
        return;
    }
    kind = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8];
    switch (kind)
    {
    case 0:
        lim_a = 0;
        lim_b = 0x80000000;
        break;
    case 1:
        lim_a = 0x03FFFFFF;
        lim_b = 0;
        break;
    case 2:
        lim_a = 0;
        lim_b = 0x03FFFFFF;
        break;
    case 3:
        lim_a = 0x80000000;
        lim_b = 0;
        break;
    }
    ((struct Flags8 *)&body->unk20D)->bit3 = 1;
    obj_start_move_zero_anchor(body, 1, lim_a, lim_b);
    obj_drift_with_cap_step(body);
    *state_out = (u32)enemy_after_contact;
}

ASM_FUNC("asm/code_081442FC/enemy_push_by_mode.s", void enemy_push_by_mode(void))
