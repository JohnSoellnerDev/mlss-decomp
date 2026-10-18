#include "global.h"
#include "ram_routines.h"

typedef void (*EnemyFn)();

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct Record
{
    u8 filler0[0x1A];
    u16 unk1A;
    u8 filler1C[0x22 - 0x1C];
    u8 unk22;
    u8 unk23;
    u16 unk24;
    u16 unk26;
    u8 filler28[0x36 - 0x28];
    s16 unk36;
};

struct Partner
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x208 - 0x38];
    u8 flags208;
};

struct OwnerBits
{
    u8 low2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Sprite
{
    u8 filler0[0x12];
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    s8 unk20;
    u8 unk21;
    u8 filler22[0x23 - 0x22];
    u8 unk23;
    u8 filler24[0x2C - 0x24];
    s16 unk2C;
};

struct Flags12
{
    u8 bit0 : 1;
    u8 mid2 : 2;
    u8 rest : 5;
};

struct Flags12b
{
    u8 low3 : 3;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct Flags24
{
    u8 bit0 : 1;
    u8 cell : 2;
    u8 rest : 5;
};

struct Flags1D6
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct Flags214
{
    u8 low3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 high2 : 2;
};

struct Flags33E
{
    u8 low4 : 4;
    u8 bit4 : 1;
    u8 high3 : 3;
};

struct Flags20C
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags210
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high2 : 2;
};

struct Record;

struct Behavior
{
    u8 filler0[0x17A];
    u16 timerB;
};

struct Enemy
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 unk25;
    u8 filler26[0x34 - 0x26];
    s32 prevX;
    s32 prevY;
    u8 filler3C[0xF8 - 0x3C];
    u16 unk0F8;
    u8 filler0FA[0x100 - 0xFA];
    u16 wanderDist[4];
    u16 unk108[4];
    u8 filler110[0x128 - 0x110];
    u16 unk128[4];
    u16 wanderSteps[2];
    u16 linkIndex;
    u8 filler136[0x138 - 0x136];
    s16 hitBottom;
    s16 hitTop;
    s16 hitLeft;
    s16 hitRight;
    u8 filler140[0x198 - 0x140];
    EnemyFn unk198;
    EnemyFn unk19C;
    EnemyFn unk1A0[2];
    u8 filler1A8[0x1AC - 0x1A8];
    EnemyFn unk1AC;
    struct Record *rec;
    s16 *box;
    s16 vecX;
    s16 vecY;
    u8 filler1BC[0x1BE - 0x1BC];
    s16 hopValue;
    u32 hopDist;
    u8 filler1C4[0x1D0 - 0x1C4];
    u16 index;
    u16 unk1D2;
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x1D8 - 0x1D7];
    u16 unk1D8;
    u8 filler1DA[0x1DC - 0x1DA];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x20C - 0x1EC];
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
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u32 hopZ0;
    u8 filler250[0x254 - 0x250];
    u32 hopSpeed0;
    s32 jumpVz;
    s32 jumpFrames;
    u8 filler260[0x274 - 0x260];
    u16 unk274;
    u8 filler276[0x294 - 0x276];
    struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 orbitCenX;
    s32 orbitCenY;
    u8 filler2C4[0x2C8 - 0x2C4];
    s32 orbitCenZ;
    s32 orbitRadX;
    s32 orbitRadY;
    u8 filler2D4[0x2D8 - 0x2D4];
    u16 unk2D8;
    u8 filler2DA[0x2EC - 0x2DA];
    s32 unk2EC;
    u8 filler2F0[0x2F4 - 0x2F0];
    u32 unk2F4;
    u16 unk2F8;
    u8 filler2FA[0x33E - 0x2FA];
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *body;
};

void load_record_fields_scaled(struct Enemy *obj, s16 index);
void load_hop_entry(struct Enemy *ent, s16 index);
void load_hit_box_words(struct Enemy *obj, s16 index);
void set_timer_from_table(struct Enemy *obj, s16 index);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void setup_random_offset_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_hop_phase(void *ctx, struct Enemy *obj);
void enter_state_c188(void *ctx, struct Enemy *obj);
void enter_hop_phase(void *ctx, struct Enemy *obj);
void obj_sub_state_then_flag(void *ctx, struct Enemy *obj);
void refresh_cell_if_changed(void *ctx, struct Enemy *obj);
void obj_approach_step(struct Enemy *obj);
u8 advance_move_step(void *ctx, struct Enemy *obj);
void obj_begin_glide(void);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_step_e6a0(struct Sprite *spr);
void tick_store(void);
void enemy_dash_to_owner_target(void);
void reset_record_fields(struct Enemy *obj);
u8 find_active_entry(struct Owner *owner, u8 *idx);
u8 point_in_box(s16 *bounds, struct Partner *partner, s32 zMax, s32 zMin);
void tile_step_probe(struct Owner *ctx, struct Enemy *body, EnemyFn *out);
void face_partner(void *ctx, struct Enemy *obj, EnemyFn *out);
void enemy_land_reload_sprite(void);
void enemy_hop_start_kick(struct Owner *owner, struct Enemy *ent, EnemyFn *out);
void obj_start_hop(struct Enemy *ent, u32 mode, s32 x, s32 y);
void bounce_and_check(void *ctx, struct Enemy *obj, EnemyFn *out);
void enemy_hop_state_runner(u8 *owner, struct Enemy *ent);
void enemy_wait_sprite_flag_then_load(void);
u8 find_partner_in_box(void *ctx, struct Enemy *obj, struct Partner **found);
void obj_box_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index);
u8 step_pos_xy(struct Enemy *ent);
void load_record_state(u8 *owner, struct Enemy *ent, EnemyFn *out);
void random_collide(struct Owner *owner, struct Enemy *obj, EnemyFn *out);
void step_with_box_check(void *ctx, struct Enemy *ent, EnemyFn *out);
void enemy_hop_jump_to_partner(struct Owner *owner, struct Enemy *ent, EnemyFn *out);
u8 enemy_partner_in_sensor(void *ctx, struct Enemy *ent);
u8 step_pos_xy_c(struct Enemy *ent);
void collide_react(struct Owner *owner, struct Enemy *ent, struct Partner *bro, u32 speed, u32 a, s32 b, s32 c, u32 d, u32 e);
struct Partner *owner_entry(struct Owner *owner, u16 index);
u16 angle_of_vector16(s32 x, s32 y);
void timed_step_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void finish_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void move_toward_partner(void *ctx, struct Enemy *obj, EnemyFn *slot);
u8 probe_body_hit(struct Owner *owner, struct Enemy *body, s32 x, s32 y, s32 z, u32 a, u16 *b);
void obj_state_run_advance(void);
void partner_seek_countdown_then_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void cpu_set(const void *src, void *dst, u32 ctl);
void load_record_words_128(struct Enemy *obj, s16 index);
s32 rand_below(s32 n, u32 value);
u32 mt19937_next(void);


void timer_overlap_state(void);
void timer_overlap_state_b(void);
void timer_overlap_state_c(void);
void run_state_handler(void);
void obj_random_pose_short(void);
void obj_random_target_b(void);
void seek_partner(void *ctx, struct Enemy *obj);
void probe_zone_point(void *ctx, struct Enemy *obj, EnemyFn *out);
void partner_seek_after_countdown(void *ctx, struct Enemy *obj);
void enter_state_d8f4(void *ctx, struct Enemy *obj);
void enter_state_d6dc(void *ctx, struct Enemy *obj);
void enter_state_dbc0(void *ctx, struct Enemy *obj);
void enter_seek_state(void *ctx, struct Enemy *obj);
void load_part_offsets(struct Enemy *obj, s16 index);




struct Flags20D
{
    u8 low2 : 2;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 high4 : 4;
};


void child_box_state(void *ctx, struct Enemy *obj)
{
    struct Enemy *body = obj->body;
    u8 hit;

    owner_entry(ctx, 0xFF);
    hit = probe_body_hit(ctx, body, body->posX, body->posY, body->posZ + body->unk18, 1, 0);
    if (hit != 0)
    {
        obj_set_sprite_link(body, 0);
        obj_set_active(body, 0);
        ((struct Flags214 *)&obj->unk214)->low3 = 3;
        obj_set_cell_coord(obj, ((u32)obj->unk24 << 29) >> 30, 0);
        obj->unk198 = enter_state_dbc0;
    }
    else
    {
        move_toward_partner(ctx, obj, obj->unk1A0);
        obj->unk198 = obj_state_run_advance;
        obj->unk19C = enter_state_dbc0;
    }
}


ASM_FUNC("asm/code_0813DE50/move_toward_partner.s", void move_toward_partner(void *ctx, struct Enemy *obj, EnemyFn *slot))


void bounce_and_check(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Enemy *body = obj->body;

    if ((body->unk25 & 0x78) != 0)
    {
        body->unk2D8 += body->accel;
        {
            s16 v = body->unk2D8;
            body->unk2D8 = -v;
        }
        ((struct Flags20D *)&body->unk20D)->bit3 = 0;
    }
    if (advance_move_step(ctx, body) == 1)
    {
        obj_approach_step(body);
    }
    else
    {
        *out = refresh_cell_if_changed;
        ((struct Flags20D *)&body->unk20E)->bit2 = 0;
    }
}

void probe_contact_move(struct Owner *owner, struct Enemy *ent);
struct ZoneLocals
{
    u8 idx;
    u8 filler1[3];
    s16 box[8];
};

u8 boxes_overlap_oriented(s16 *box, struct Enemy *a, struct Partner *b, s32 zAbove, s32 zBelow);
void place_child_by_stage();

void seek_partner(void *ctx, struct Enemy *obj)
{
    EnemyFn *slot = obj->unk1A0;
    EnemyFn handler = slot[0];
    struct ZoneLocals loc;
    struct Partner *partner;
    struct Record *rec;
    s16 *zone;
    s16 *bx;

    handler(ctx, obj, slot);
    if (obj->unk1D4 == 0)
    {
        if (find_active_entry(ctx, &loc.idx) == 0)
        {
            return;
        }
        partner = *(struct Partner **)((u8 *)ctx + 0x28 + (loc.idx << 2));
        zone = obj->box;
        bx = loc.box;
        bx[0] = zone[4] * 0x10 + 8;
        bx[3] = zone[5] * 0x10 + 8;
        bx[1] = zone[6] * 0x10 + 8;
        bx[2] = zone[7] * 0x10 + 8;
        if (point_in_box(bx, partner, 0x7FFF, 0) == 0)
        {
            return;
        }
        rec = obj->rec;
        bx[0] = ((s8 *)rec)[0x18] * 0x10;
        bx[2] = ((s8 *)rec)[0x19] * 0x10;
        bx[4] = ((s8 *)rec)[0x1A] * 0x10;
        bx[1] = ((s8 *)rec)[0x1B] * 0x10;
        bx[3] = ((s8 *)rec)[0x1C] * 0x10;
        bx[5] = ((s8 *)rec)[0x1D] * 0x10;
        if (boxes_overlap_oriented(bx, obj, partner, bx[4], bx[5]) == 0)
        {
            return;
        }
        ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 0;
        ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 1;
        obj->unk198 = obj_state_run_advance;
        obj->unk19C = probe_contact_move;
        *slot = place_child_by_stage;
        return;
    }
    obj->unk1D4--;
}

void probe_contact_move(struct Owner *owner, struct Enemy *ent)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Enemy *body = ent->body;
    struct ZoneLocals loc;
    struct Partner *partner;
    struct Record *rec;
    s16 *zone;
    s16 *bx;

    owner_entry(owner, 0xFF);
    if (probe_body_hit(owner, body, body->posX, body->posY, body->posZ + body->unk18, 1, 0) == 0)
    {
        if (find_active_entry(owner, &loc.idx) != 0)
        {
            partner = *(struct Partner **)((u8 *)owner + 0x28 + (loc.idx << 2));
            zone = ent->box;
            bx = loc.box;
            bx[0] = zone[4] * 0x10 + 8;
            bx[3] = zone[5] * 0x10 + 8;
            bx[1] = zone[6] * 0x10 + 8;
            bx[2] = zone[7] * 0x10 + 8;
            if (point_in_box(bx, partner, 0x7FFF, 0) != 0)
            {
                rec = ent->rec;
                bx[0] = ((s8 *)rec)[0x18] * 0x10;
                bx[2] = ((s8 *)rec)[0x19] * 0x10;
                bx[4] = ((s8 *)rec)[0x1A] * 0x10;
                bx[1] = ((s8 *)rec)[0x1B] * 0x10;
                bx[3] = ((s8 *)rec)[0x1C] * 0x10;
                bx[5] = ((s8 *)rec)[0x1D] * 0x10;
                if (boxes_overlap_oriented(bx, ent, partner, bx[4], bx[5]) != 0)
                {
                    move_toward_partner(owner, ent, ent->unk1A0);
                    ent->unk198 = obj_state_run_advance;
                    ent->unk19C = enter_seek_state;
                    return;
                }
            }
        }
    }
    obj_set_sprite_link(body, 0);
    obj_set_active(body, 0);
    ((struct Flags214 *)&ent->unk214)->low3 = 3;
    obj_set_cell_coord(ent, ((u32)ent->unk24 << 29) >> 30, 0);
    *(EnemyFn *)(ai + 0x140) = enter_seek_state;
}


void enter_seek_state(void *ctx, struct Enemy *obj)
{
    EnemyFn run;

    load_record_fields_scaled(obj, 0);
    load_hit_box_words(obj, 0);
    set_timer_from_table(obj, 0xB);
    load_record_fields_scaled(obj->body, 0xF);
    obj->unk198 = seek_partner;
    obj->unk1A0[0] = setup_random_offset_state;
    obj->unk1A0[1] = setup_random_offset_state;
    run = seek_partner;
    run(ctx, obj);
}


void set_flags_enter_seek(void *ctx, struct Enemy *obj)
{
    u8 *p = &obj->unk33E;
    struct Flags20D *body;

    *p = *p | 0x10;
    body = (struct Flags20D *)&obj->body->unk20D;
    body->bit2 = 0;
    p = &obj->unk2B6;
    *p = *p | 0x40;
    enter_seek_state(ctx, obj);
}


void refresh_cell_if_changed(void *ctx, struct Enemy *obj)
{
    struct Enemy *body = obj->body;
    u8 *p24;
    u8 sp23 = body->sprite->unk23;

    p24 = &obj->unk24;
    if (sp23 == (((u32)*p24 << 29) >> 30))
    {
        u8 *p = &obj->unk20D;

        *p = *p | 4;
        obj_set_sprite_link(body, 0);
        obj_set_active(body, 0);
        *(u32 *)&body->unk20C &= 0xFFFC7FFF;
        ((struct Flags214 *)&obj->unk214)->low3 = 3;
        obj_set_cell_coord(obj, (((u32)*p24 << 29) >> 30), 0);
        ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 1;
        ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 0;
    }
}


void enter_state_dbc0(void *ctx, struct Enemy *obj)
{
    EnemyFn run;

    load_record_fields_scaled(obj, 0);
    load_hit_box_words(obj, 0);
    set_timer_from_table(obj, 0xB);
    load_record_fields_scaled(obj->body, 0xF);
    obj->unk198 = timer_overlap_state_c;
    obj->unk1A0[0] = setup_random_offset_state;
    obj->unk1A0[1] = setup_random_offset_state;
    run = timer_overlap_state_c;
    run(ctx, obj);
}


void set_flags_enter_dbc0(void *ctx, struct Enemy *obj)
{
    u8 *p = &obj->unk33E;

    *p = *p | 0x10;
    ((struct Flags20D *)&obj->body->unk20D)->bit2 = 0;
    ((struct Flags20D *)&obj->body->unk20D)->bit3 = 1;
    p = &obj->unk2B6;
    *p = *p | 0x40;
    enter_state_dbc0(ctx, obj);
}


void enter_state_d8f4(void *ctx, struct Enemy *obj)
{
    EnemyFn run;

    load_record_fields_scaled(obj, 0);
    load_hit_box_words(obj, 0);
    set_timer_from_table(obj, 0xB);
    obj->unk198 = timer_overlap_state_b;
    obj->unk1A0[0] = setup_random_offset_state;
    obj->unk1A0[1] = setup_random_offset_state;
    run = timer_overlap_state_b;
    run(ctx, obj);
}




void set_flags_enter_d8f4(void *ctx, struct Enemy *obj)
{
    u8 *p = &obj->unk33E;

    *p = *p | 0x10;
    p = &obj->unk2B6;
    *p = *p | 0x40;
    enter_state_d8f4(ctx, obj);
}


void random_branch_state(void *ctx, struct Enemy *obj)
{
    EnemyFn *slot = obj->unk1A0;
    EnemyFn sub = slot[0];

    sub(ctx, obj, slot);
    if ((obj->unk1D6 & 1) != 0)
    {
        s32 roll = rand_below(0x64, mt19937_next());

        if ((u32)roll < (u32)obj->rec->unk36)
        {
            load_record_words_128(obj, 0x1C);
            slot[0] = obj_begin_glide;
            obj->unk1A0[1] = obj_begin_glide;
            obj->unk198 = obj_sub_state_then_flag;
        }
        else
        {
            obj->unk198 = obj->unk19C;
        }
    }
}



void advance_to_next_state(void *ctx, struct Enemy *obj)
{
    load_record_fields_scaled(obj, 0xF);
    obj->unk198 = obj->unk19C;
    obj->unk1A0[0] = obj_random_target_b;
    obj->unk1A0[1] = obj_random_target_b;
    ((struct Flags214 *)&obj->unk214)->low3 = 1;
    obj->unk198(ctx, obj);
}


void enter_state_d6dc(void *ctx, struct Enemy *obj)
{
    EnemyFn run;

    load_record_fields_scaled(obj, 0);
    load_hit_box_words(obj, 0);
    set_timer_from_table(obj, 0xB);
    obj->unk198 = timer_overlap_state;
    obj->unk1A0[0] = setup_random_offset_state;
    obj->unk1A0[1] = setup_random_offset_state;
    run = timer_overlap_state;
    run(ctx, obj);
}




void set_flags_enter_d6dc(void *ctx, struct Enemy *obj)
{
    u8 *p = &obj->unk33E;

    *p = *p | 0x10;
    p = &obj->unk2B6;
    *p = *p | 0x40;
    enter_state_d6dc(ctx, obj);
}



void obj_sub_state_then_flag(void *ctx, struct Enemy *obj)
{
    EnemyFn *slot = obj->unk1A0;
    EnemyFn fn = slot[0];

    fn(ctx, obj, slot);
    if (obj->unk1D6 & 1)
    {
        load_record_fields_scaled(obj, 0x20);
        slot[0] = obj_random_pose_short;
        obj->unk1A0[1] = obj_random_pose_short;
        obj->unk198 = obj_state_run_advance;
    }
}


void enemy_check_entry_in_box(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    s16 box[8];
    u8 idx;
    u16 zero;
    struct Partner *partner;

    if (find_active_entry(ctx, &idx) != 0)
    {
        partner = *(struct Partner **)((u8 *)ctx + 0x28 + (idx << 2));
        zero = 0;
        cpu_set(&zero, box, 0x01000008);
        box[0] = obj->posX - 0x2000;
        box[3] = obj->posY - 0x2000;
        box[1] = obj->posX + 0x2000;
        box[2] = obj->posY + 0x2000;
        if (point_in_box(box, partner, 0x7FFF, 0) != 0)
        {
            goto hit;
        }
    }
    *out = setup_random_offset_state;
    return;
hit:
    *out = partner_seek_countdown_then_state;
    obj->unk239 = 0x7F;
    obj->unk1D4 = 0x78;
    ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 0;
    ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 1;
}


void probe_zone_point(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    s16 *zone = obj->box;
    struct ZoneLocals loc;
    struct Partner *partner;
    s16 *bx;

    if (find_active_entry(ctx, &loc.idx) != 0)
    {
        partner = *(struct Partner **)((u8 *)ctx + 0x28 + (loc.idx << 2));
        bx = loc.box;
        bx[0] = *zone * 0x10 + 8;
        zone++;
        bx[3] = *zone * 0x10 + 8;
        zone++;
        bx[1] = *zone * 0x10 + 8;
        bx[2] = zone[1] * 0x10 + 8;
        if (point_in_box(bx, partner, 0x7FFF, 0) != 0)
        {
            *out = tile_step_probe;
            ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 0;
            ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 1;
        }
    }
}

ASM_FUNC("asm/code_0813DE50/tile_step_probe.s", void tile_step_probe(struct Owner *ctx, struct Enemy *body, EnemyFn *out))






NONMATCH("asm/code_0813DE50/face_partner.s", void face_partner(void *ctx, struct Enemy *obj, EnemyFn *out))
{
    struct Sprite *sprite = obj->sprite;
    EnemyFn run;

    if ((sprite->unk12 & 8) != 0)
    {
        u8 *ai = (u8 *)obj + 0x58;
        struct Partner *partner = owner_entry(ctx, 0xFF);
        s32 dx = partner->posX - obj->posX;
        s32 dy = partner->posY - obj->posY;
        u32 dir;
        u8 *p;

        if ((dx | dy) != 0)
        {
            s32 t = angle_of_vector16(dx, dy) + 0x1000;

            dir = (t >> 13) & 7;
        }
        else
        {
            dir = -1;
        }
        p = &obj->unk24;
        dir &= 7;
        {
            s32 cell = *p;

            cell = -8 & cell;
            cell |= dir;
            *p = cell;
            obj_set_cell_coord(obj, ((u32)cell << 29) >> 30, 0);
        }
        p = &obj->unk20C;
        {
            s32 bits = *p;

            bits |= 0x20;
            bits |= 0x40;
            bits |= 0x80;
            *p = bits;
        }
        p = &obj->unk210;
        *p = *p | 0x10;
        ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
        ((struct Flags12b *)&sprite->unk12)->bit3 = 0;
        *(u16 *)(ai + 0x17C) = 0;
        run = timed_step_state;
        *out = run;
        run(ctx, obj, out);
    }
}
END_NONMATCH









void timed_step_state(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Sprite *sprite = obj->sprite;
    u16 *timer = &obj->unk1D4;
    EnemyFn run;
    s32 roll;
    s32 off;
    s32 t;
    u8 *base;

    t = *timer;
    if (t == 0)
    {
        if ((sprite->unk12 & 8) != 0)
        {
            obj_set_cell_coord(obj, (((u32)obj->unk24 << 29) >> 30) + 4, 0);
            roll = rand_below(4, mt19937_next());
            off = roll << 1;
            base = (u8 *)obj + 0x136;
            *timer = *(u16 *)(base + off);
            ((struct Flags12b *)&sprite->unk12)->bit3 = 0;
            if (*timer != 0)
            {
                ((struct Flags12 *)&sprite->unk12)->mid2 = 0;
                return;
            }
        }
        else
        {
            if (sprite->unk2C > 0x7F)
            {
                obj->unk274 = 0x14;
            }
            else if (sprite->unk2C > 0x3F)
            {
                obj->unk274 = 0xC;
            }
            return;
        }
    }
    else
    {
        s32 left = t - 1;

        *timer = left;
        if ((left << 16) != 0)
        {
            return;
        }
    }
    obj_set_cell_coord(obj, (((u32)obj->unk24 << 29) >> 30) + 8, 0);
    ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
    ((struct Flags12b *)&sprite->unk12)->bit3 = 0;
    run = finish_state;
    *out = run;
    run(ctx, obj, out);
}



void finish_state(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Sprite *sprite = obj->sprite;

    if ((sprite->unk12 & 8) != 0)
    {
        ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
        ((struct Flags20D *)&obj->unk20E)->bit2 = 0;
        obj_set_sprite_link(obj, 0);
        if (obj->unk1D2 != 0)
        {
            *out = tick_store;
        }
        else
        {
            out[0] = out[1];
            obj->unk0F8 &= 0xFBFF;
            ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 1;
            ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 0;
        }
    }
    else if (sprite->unk2C > 0x7F)
    {
        ((struct Flags20C *)&obj->unk20C)->bit5 = 0;
        ((struct Flags20C *)&obj->unk20C)->bit6 = 0;
        ((struct Flags20C *)&obj->unk20C)->bit7 = 0;
        ((struct Flags33E *)&obj->unk210)->bit4 = 0;
        obj->unk274 = 4;
    }
    else if (sprite->unk2C > 0x3F)
    {
        obj->unk274 = 0xC;
    }
}


void reset_move_state(void *ctx, struct Enemy *obj)
{
    ((struct Flags33E *)&obj->unk33E)->bit4 = 0;
    ((struct Flags20D *)&obj->unk20D)->bit2 = 0;
    ((struct Flags210 *)&obj->unk210)->bit5 = 1;
    ((struct Flags20C *)&obj->unk20C)->bit5 = 0;
    ((struct Flags20C *)&obj->unk20C)->bit6 = 0;
    ((struct Flags20C *)&obj->unk20C)->bit7 = 0;
    obj->unk274 = 4;
    obj_set_sprite_link(obj, 0);
    enter_state_c188(ctx, obj);
}



void partner_seek_countdown_then_state(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    s32 left = obj->unk1D4 - 1;

    obj->unk1D4 = left;
    if ((left << 16) == 0)
    {
        obj->unk239 = 0x7F;
        *out = partner_seek_after_countdown;
    }
}

void set_flag_0x40_then_call(void *ctx, struct Enemy *obj)
{
    s32 flags = obj->unk2B6;
    obj->unk2B6 = 0x40 | flags;
    enter_hop_phase(ctx, obj);
}


void enter_state_c188(void *ctx, struct Enemy *obj)
{
    EnemyFn run;

    load_record_fields_scaled(obj, 0);
    load_part_offsets(obj, 9);
    obj->unk198 = run_state_handler;
    obj->unk1A0[0] = probe_zone_point;
    obj->unk1A0[1] = probe_zone_point;
    obj->unk1AC = probe_zone_point;
    run = run_state_handler;
    run(ctx, obj);
}

void partner_seek_after_countdown(void *ctx, struct Enemy *obj)
{
    obj->posY -= 0x40;
}

void enter_hop_phase(void *ctx, struct Enemy *obj)
{
    EnemyFn run;

    load_record_fields_scaled(obj, 0);
    load_hop_entry(obj, 0xB);
    load_hit_box_words(obj, 0);
    obj->unk198 = obj_state_run_hop_phase;
    obj->unk1A0[0] = setup_random_offset_state;
    obj->unk1A0[1] = setup_random_offset_state;
    ((struct Flags214 *)&obj->unk214)->low3 = 4;
    run = obj_state_run_hop_phase;
    run(ctx, obj);
}




void random_collide(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    u8 *tbl;
    u8 idx;
    s32 pick;
    s32 pick2;
    u16 speed;
    u16 kick;
    u8 index;

    tbl = (u8 *)ent + 0x100;
    pick = rand_below(4, mt19937_next());
    ent->sprite->unk20 = *(u16 *)(tbl + pick * 2 + 8);
    ((struct Flags214 *)&ent->unk214)->low3 = 4;
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 0;
    pick = rand_below(4, mt19937_next());
    speed = *(u16 *)(tbl + pick * 2);
    pick2 = rand_below(2, mt19937_next());
    kick = *(u16 *)(tbl + pick2 * 2 + 0x30);
    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    collide_react(owner, ent, ((void **)((u8 *)owner + 0x28))[index], kick, speed, (s8)tbl[0x40], (s8)tbl[0x41], 0x22, 0);
    *out = step_with_box_check;
}



void step_with_box_check(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    u16 *table = ent->wanderDist;
    struct Record *rec = ent->rec;
    struct Partner *partner;
    s16 box[8];
    u16 reload;
    u16 pick_val;

    if (ent->unk1D4 == 0) {
        if (find_partner_in_box(ctx, ent, &partner)) {
            if (((u16 *)ent->box)[4] == 0) {
                out[0] = load_record_state;
                return;
            }
            obj_box_to_stack(box, ent, 0x12, 10);
            if (point_in_box(box, partner, 0x7FFF, 0)) {
                out[0] = load_record_state;
                return;
            }
        }
        reload = rec->unk1A;
        *(u16 *)(ai + 0x17C) = reload;
    } else {
        ent->unk1D4 = ent->unk1D4 - 1;
    }
    if (step_pos_xy(ent) == 0) {
        s32 pick = rand_below(4, mt19937_next());
        pick_val = table[pick + 0x14];
        *(u16 *)(ai + 0x17A) = pick_val;
        out[0] = tick_store;
        out[3] = random_collide;
    }
}


void load_record_state(u8 *owner, struct Enemy *ent, EnemyFn *out)
{
    struct Record *rec = ent->rec;

    ent->index = rand_below(4, mt19937_next());
    ent->sprite->unk20 = ((u8 *)rec)[ent->index + 0x22];
    ((struct Flags214 *)&ent->unk214)->low3 = 1;
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 1;
    obj_load_record(ent->sprite, 3, 0, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 1;
    out[0] = enemy_wait_sprite_flag_then_load;
}


void load_record_state_b(u8 *owner, struct Enemy *ent, EnemyFn *out)
{
    struct Record *rec = ent->rec;

    ent->sprite->unk20 = ((u8 *)rec)[ent->index + 0x22];
    ((struct Flags214 *)&ent->unk214)->low3 = 1;
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 0;
    obj_load_record(ent->sprite, 4, 0, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ent->unk1D2 = ((s8 *)rec)[ent->index + 0x26];
    out[0] = tick_store;
    out[3] = enemy_dash_to_owner_target;
}

void start_move_to(struct Enemy *obj, u32 x, u32 y, u32 speed, ByteArg flag);
void enemy_box_point_then_collide();

NONMATCH("asm/code_0813DE50/aim_and_start_move.s", void aim_and_start_move(void *ctx, struct Enemy *obj, EnemyFn *out))
{
    u8 *zone = (u8 *)obj + 0x58;
    u8 *tab = (u8 *)obj->rec;
    s32 dx = obj->homeX - obj->posX;
    s32 dy = obj->homeY - obj->posY;
    u32 dir;
    s32 dx2;
    s32 dy2;
    s32 dir2;
    s32 cell;
    u32 speed;
    s32 (*divFn)(s32, s32);

    if ((dx | dy) != 0)
    {
        u32 a = angle_of_vector16(dx, dy);

        dir = (a + 0x10) >> 8;
    }
    else
    {
        dir = 0xFF;
    }
    ((u8 *)obj)[2] = dir;
    obj->sprite->unk20 = ((u8 *)(tab + 0x36))[*(u16 *)(zone + 0x178)];
    ((struct Flags214 *)&obj->unk214)->low3 = 4;
    ((struct Flags12 *)&obj->sprite->unk12)->mid2 = 0;
    divFn = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    dx2 = *(s32 *)(zone + 0x184) - obj->posX;
    dy2 = *(s32 *)(zone + 0x188) - obj->posY;
    if ((dx2 | dy2) != 0)
    {
        s32 t = (u16)angle_of_vector16(dx2, dy2) + 0x1000;

        dir2 = t >> 13;
        dir2 &= 7;
    }
    else
    {
        dir2 = -1;
    }
    cell = divFn(dir2, 2);
    obj_set_cell_coord(obj, cell, 0);
    speed = ((u16 *)(tab + 0x3A))[*(u16 *)(zone + 0x178)];
    start_move_to(obj, *(s32 *)(zone + 0x184), *(s32 *)(zone + 0x188), speed, (u8)0);
    *out = enemy_box_point_then_collide;
}
END_NONMATCH


void init_state_from_box(void *ctx, struct Enemy *obj)
{
    struct Record *rec = obj->rec;
    s16 *box = obj->box;
    u8 *p;

    p = &obj->unk33E;
    *p = *p | 0x10;
    reset_record_fields(obj);
    obj->unk2EC = obj->posZ + obj->unk18;
    obj->unk2F4 = 0;
    obj->unk2F8 = rec->unk23;
    obj->unk1D4 = 0;
    if (((u16 *)box)[5] == 1)
    {
        obj->unk198 = run_state_handler;
        obj->unk1A0[0] = enemy_land_reload_sprite;
    }
    else
    {
        obj->unk198 = enemy_hop_state_runner;
        obj->unk1A0[0] = enemy_hop_start_kick;
    }
}

ASM_FUNC("asm/code_0813DE50/enemy_hop_state_runner.s", void enemy_hop_state_runner(u8 *owner, struct Enemy *ent))

extern const u8 gFrameAttrTable[];
void enemy_hop_wait_partner(u8 *owner, struct Enemy *ent, EnemyFn *out);
void enemy_hop_start_kick(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    u8 *tbl;
    s32 pick;
    u16 speed;
    u16 kick;
    u8 idx;
    u8 index;
    struct Partner *party;

    tbl = (u8 *)ent + 0x100;
    pick = rand_below(4, mt19937_next());
    ent->sprite->unk20 = *(u16 *)(tbl + pick * 2 + 8);
    ((struct Flags214 *)&ent->unk214)->low3 = 4;
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 0;
    ((struct Flags214 *)&ent->unk214)->low3 = 4;
    obj_set_cell_coord(ent, gFrameAttrTable[(((u32)ent->unk216 << 28) >> 30) * 0x10 + (((u32)ent->unk24 << 29) >> 29) + 8], 0);
    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    party = *(struct Partner **)((u8 *)owner + index * 4 + 0x28);
    pick = rand_below(4, mt19937_next());
    speed = *(u16 *)(tbl + pick * 2);
    pick = rand_below(2, mt19937_next());
    kick = *(u16 *)(tbl + pick * 2 + 0x30);
    collide_react(owner, ent, party, kick, speed, (s8)tbl[0x40], (s8)tbl[0x41], 0x22, 0);
    *out = enemy_hop_wait_partner;
}


void enemy_hop_wait_partner(u8 *owner, struct Enemy *ent, EnemyFn *out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    u16 *table = ent->wanderDist;
    struct Record *rec = ent->rec;
    u16 *timer = &ent->unk1D4;
    s32 t = *timer;
    s32 next;

    if (t == 0)
    {
        if (enemy_partner_in_sensor(owner, ent))
        {
            out[0] = enemy_hop_jump_to_partner;
            return;
        }
        next = rec->unk1A;
    }
    else
    {
        next = t - 1;
    }
    *timer = next;
    if (step_pos_xy_c(ent))
    {
        return;
    }
    {
        u32 rnd = mt19937_next();
        s32 pick = rand_below(4, rnd);
        ai->timerB = table[pick + 0x14];
    }
    out[0] = tick_store;
    out[3] = enemy_hop_start_kick;
}

s32 rand_below(s32 range, u32 seed);

ASM_FUNC("asm/code_0813DE50/enemy_hop_jump_to_partner.s", void enemy_hop_jump_to_partner(struct Owner *owner, struct Enemy *ent, EnemyFn *out))
