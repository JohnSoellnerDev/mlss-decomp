#include "global.h"
#include "ram_routines.h"

typedef void (*EnemyFn)();

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct Record
{
    u8 filler0[0x1A];
    u16 unk1A;
    u8 filler1C[0x22 - 0x1C];
    u8 unk22;  /* hover enemies: bob amplitude */
    u8 unk23;  /* hover enemies: bob speed */
    u16 unk24; /* hover enemies: hover height above the ground, 16 px units */
    u16 unk26; /* hover enemies: how fast the hover height is reached (24.8 per frame) */
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

// An Enemy seen from its behaviour block at +0x58 (the same memory as Enemy from 0x58 on).
struct Behavior
{
    u8 filler0[0x158];
    struct Record *rec; // Enemy.rec
    u8 filler15C[0x178 - 0x15C];
    u16 index;  // Enemy.index: random variant 0-3 picked for the current move
    u16 timerB; // Enemy.unk1D2
    u16 holdTimer; // Enemy.unk1D4: frames left in timed_step_state's hold
    u8 filler17E[0x184 - 0x17E];
    s32 homeX; // Enemy.homeX: where the current move started
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
};

struct Enemy
{
    u8 filler0[0x2];
    u8 heading; // 256-step direction it moves in (0xFF: none)
    u8 filler3[0xC - 0x3];
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
    s32 unk2EC; /* hover: z the bob centres on (24.8), drifts towards posZ + Record.unk24 * 16 px */
    u8 filler2F0[0x2F4 - 0x2F0];
    u32 unk2F4; /* hover: bob phase, 256 steps per turn */
    u16 unk2F8; /* hover: bob speed, from Record.unk23 */
    u8 filler2FA[0x33E - 0x2FA];
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *body;
};

/* Where each table of EnemyMoveTable.words starts; a move adds a random pick to it. */
enum EnemyMoveWord
{
    MOVE_SPEED = 0,       /* [4] move speed */
    MOVE_ANIM_SPEED = 4,  /* [4] sprite animation speed while moving */
    MOVE_WAIT_TIME = 0x14, /* [4] frames to wait between moves */
    MOVE_DIST = 0x18      /* [2] move distance in pixels */
};

/* The enemy's movement parameters, seen from Enemy+0x100. */
struct EnemyMoveTable
{
    u16 words[0x20];
    u8 chanceX; /* +0x40: percent chance of stepping along X (collide_boxes takes it as s8) */
    u8 chanceY; /* +0x41: same for Y */
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
void start_move_to_3d(struct Enemy *obj, s32 x, s32 y, s32 z, u32 speed, ByteArg flag);
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);
u32 position_blocked(struct Owner *owner, struct Enemy *obj, u32 x, u32 y, u32 z);
void obj_random_orbit_offset(struct Enemy *obj, struct Partner *partner);
void obj_settle_on_tiles(struct Enemy *obj, u32 flag);
void enemy_hop_prepare_fall(void);
void enemy_hop_fall_then_next(void);
void enemy_xyz_step_done(void);
void enemy_hop_land_on_partner(void);
void enemy_hop_land_next(void);
void enemy_hop_fall_to_tile(void);
void enemy_after_fall_check_box(void);
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


/* Enemy state set by timer_overlap_state_c after a party member touched its hit box: if tiles block
 * the enemy's body where it stands, it drops the body's sprite, resets its cell and goes back to
 * enter_state_dbc0; otherwise it moves on toward the active party member (obj_state_run_advance),
 * with enter_state_dbc0 as the state after that. */
void child_box_state(void *ctx, struct Enemy *obj)
{
    struct Enemy *body = obj->body;
    u8 blocked;

    owner_entry(ctx, 0xFF);
    blocked = probe_body_hit(ctx, body, body->posX, body->posY, body->posZ + body->unk18, 1, 0);
    if (blocked != 0)
    {
        obj_set_sprite_link(body, 0);
        obj_set_active(body, 0);
        ((struct Flags214 *)&obj->unk214)->low3 = 3;
        obj_set_cell_coord(obj, ((u32)obj->unk24 << 29) >> 30, 0);
        obj->unk198 = enter_state_dbc0; /* state */
    }
    else
    {
        move_toward_partner(ctx, obj, obj->unk1A0);
        obj->unk198 = obj_state_run_advance; /* state */
        obj->unk19C = enter_state_dbc0;      /* state once the move is done */
    }
}


static inline s32 half_dist(s32 d)
{
    s32 t = d;
    s32 prod;

    if (d < 0)
    {
        t = d + 3;
    }
    prod = d * (t >> 2);
    if (prod < 0)
    {
        prod += 3;
    }
    return prod >> 2;
}

NONMATCH("asm/code_0813DE50/move_toward_partner.s", void move_toward_partner(void *ctx, struct Enemy *obj, EnemyFn *slot))
{
    struct Enemy *sprite;
    struct Partner *partner;
    u32 rnd;
    s32 dx;
    s32 dy;
    s32 px;
    s32 py;
    s32 dist;
    s32 idx;
    s32 sign;
    s32 ux;
    s32 tx;
    s32 ty;
    sprite = obj->body;
    partner = owner_entry(ctx, 0xFF);
    dx = partner->posX - sprite->posX;
    px = half_dist(dx);
    dy = partner->posY - sprite->posY;
    py = half_dist(dy);
    dist = ((s32 (*)(s32))gMathRoutines)(px + py);
    rnd = mt19937_next();
    idx = rand_below(2, rnd);
    sign = *(s16 *)((u8 *)sprite + 0x130 + (idx << 1));

    ux = ((partner->posX - sprite->posX) << 6) / dist;
    tx = ux * sign + sprite->posX;
    ty = (((partner->posY - sprite->posY) << 6) / dist) * sign + sprite->posY;

    obj_start_hop(sprite, 0, tx, ty);
    obj_approach_step(sprite);

    ((struct Flags20D *)&sprite->unk20D)->bit3 = 1;
    *(u32 *)&sprite->unk20C &= 0xFFFC7FFF;
    *(u32 *)&sprite->unk20C |= 0x8000;
    ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 0;
    ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 1;
    *slot = bounce_and_check;
}
END_NONMATCH


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

/* Enemy seek state: runs its current sub-state, then, once the unk1D4 countdown is over, looks for an
 * active party member inside the enemy's zone (its box words 4-7, in tiles) that also overlaps its
 * record's hit box. When one is found the enemy starts probe_contact_move with place_child_by_stage
 * as its sub-state; until then the countdown goes down by one a frame. */
void seek_partner(void *ctx, struct Enemy *obj)
{
    EnemyFn *subState = obj->unk1A0;
    EnemyFn run = subState[0];
    struct ZoneLocals loc;
    struct Partner *partner;
    struct Record *rec;
    s16 *zone;
    s16 *bounds;

    run(ctx, obj, subState);
    if (obj->unk1D4 == 0)
    {
        if (find_active_entry(ctx, &loc.idx) == 0)
        {
            return;
        }
        partner = *(struct Partner **)((u8 *)ctx + 0x28 + (loc.idx << 2)); /* ctx->party[loc.idx] */
        /* the zone: tile coordinates to pixel centres */
        zone = obj->box;
        bounds = loc.box;
        bounds[0] = zone[4] * 0x10 + 8;
        bounds[3] = zone[5] * 0x10 + 8;
        bounds[1] = zone[6] * 0x10 + 8;
        bounds[2] = zone[7] * 0x10 + 8;
        if (point_in_box(bounds, partner, 0x7FFF, 0) == 0)
        {
            return;
        }
        /* the record's hit box, 6 signed bytes in tiles; [4] and [5] are its z extent */
        rec = obj->rec;
        bounds[0] = ((s8 *)rec)[0x18] * 0x10;
        bounds[2] = ((s8 *)rec)[0x19] * 0x10;
        bounds[4] = ((s8 *)rec)[0x1A] * 0x10;
        bounds[1] = ((s8 *)rec)[0x1B] * 0x10;
        bounds[3] = ((s8 *)rec)[0x1C] * 0x10;
        bounds[5] = ((s8 *)rec)[0x1D] * 0x10;
        if (boxes_overlap_oriented(bounds, obj, partner, bounds[4], bounds[5]) == 0)
        {
            return;
        }
        ((struct Flags1D6 *)&obj->unk1D6)->bit0 = 0;
        ((struct Flags1D6 *)&obj->unk1D6)->bit1 = 1;
        obj->unk198 = obj_state_run_advance;
        obj->unk19C = probe_contact_move;
        *subState = place_child_by_stage;
        return;
    }
    obj->unk1D4--;
}

/* Enemy state set by seek_partner once a party member came into reach: while the enemy's body touches
 * nothing and an active party member is still inside the enemy's zone (its box words 4-7, in tiles)
 * and overlaps its record's hit box, keeps moving toward that member; otherwise it drops the body's
 * sprite, resets the cell and goes back to seeking. Either way enter_seek_state comes next. */
void probe_contact_move(struct Owner *owner, struct Enemy *ent)
{
    u8 *behavior = (u8 *)ent + 0x58;
    struct Enemy *body = ent->body;
    struct ZoneLocals loc;
    struct Partner *partner;
    struct Record *rec;
    s16 *zone;
    s16 *bounds;

    owner_entry(owner, 0xFF);
    if (probe_body_hit(owner, body, body->posX, body->posY, body->posZ + body->unk18, 1, 0) == 0)
    {
        if (find_active_entry(owner, &loc.idx) != 0)
        {
            partner = *(struct Partner **)((u8 *)owner + 0x28 + (loc.idx << 2)); /* owner->party[loc.idx] */
            /* the zone: tile coordinates to pixel centres */
            zone = ent->box;
            bounds = loc.box;
            bounds[0] = zone[4] * 0x10 + 8;
            bounds[3] = zone[5] * 0x10 + 8;
            bounds[1] = zone[6] * 0x10 + 8;
            bounds[2] = zone[7] * 0x10 + 8;
            if (point_in_box(bounds, partner, 0x7FFF, 0) != 0)
            {
                /* the record's hit box, 6 signed bytes in tiles; [4] and [5] are its z extent */
                rec = ent->rec;
                bounds[0] = ((s8 *)rec)[0x18] * 0x10;
                bounds[2] = ((s8 *)rec)[0x19] * 0x10;
                bounds[4] = ((s8 *)rec)[0x1A] * 0x10;
                bounds[1] = ((s8 *)rec)[0x1B] * 0x10;
                bounds[3] = ((s8 *)rec)[0x1C] * 0x10;
                bounds[5] = ((s8 *)rec)[0x1D] * 0x10;
                if (boxes_overlap_oriented(bounds, ent, partner, bounds[4], bounds[5]) != 0)
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
    *(EnemyFn *)(behavior + 0x140) = enter_seek_state; /* ent->unk198 */
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


/* Enemy sub-state: looks for the active party member inside a box of +-0x2000 around the enemy
 * (built from posX/posY in 8.8 fixed point but stored as s16, while point_in_box compares pixels).
 * Found: sets unk239 to 0x7F, a 120-frame countdown (unk1D4), marks the enemy busy (unk1D6 bit 1)
 * and continues with partner_seek_countdown_then_state; otherwise with setup_random_offset_state. */
void enemy_check_entry_in_box(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    s16 box[8];
    u8 slot;
    u16 zero;
    struct Partner *partner;

    if (find_active_entry(ctx, &slot) != 0)
    {
        partner = *(struct Partner **)((u8 *)ctx + 0x28 + (slot << 2)); /* ctx->party[slot] */
        zero = 0;
        cpu_set(&zero, box, 0x01000008); /* fill 8 halfwords with 0 */
        box[0] = obj->posX - 0x2000; /* left */
        box[3] = obj->posY - 0x2000; /* top */
        box[1] = obj->posX + 0x2000; /* right */
        box[2] = obj->posY + 0x2000; /* bottom */
        if (point_in_box(box, partner, 0x7FFF, 0) != 0)
        {
            goto found;
        }
    }
    *out = setup_random_offset_state;
    return;
found:
    *out = partner_seek_countdown_then_state;
    obj->unk239 = 0x7F;
    obj->unk1D4 = 120;
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

NONMATCH("asm/code_0813DE50/tile_step_probe.s", void tile_step_probe(struct Owner *ctx, struct Enemy *body, EnemyFn *out))
{
    struct Partner *partner;
    u8 *ai;
    u16 hit16;
    u8 flag;
    u8 *map;
    s32 zero;
    s32 height;
    s32 outB;
    s32 outC;
    s32 t;
    s32 qx;
    s32 qy;
    s32 dist;
    s32 x;
    s32 y;
    u8 hit;
    u8 *table;
    u8 subHit;
    struct Sprite *sprite;

    ai = (u8 *)body + 0x58;
    partner = owner_entry(ctx, 0xFF);
    obj_random_orbit_offset(body, partner);
    zero = 0;
    hit16 = zero;

    t = (body->vecX << 8) - body->posX;
    if (t < 0)
    {
        t += 3;
    }
    qx = t >> 2;
    t = (body->vecY << 8) - body->posY;
    if (t < 0)
    {
        t += 3;
    }
    qy = t >> 2;
    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(qx * qx + qy * qy);
    *(s32 *)((u8 *)body + 0x1CC) = dist << 2;
    body->unk1D2 = body->unk128[body->index];

    x = body->vecX << 8;
    body->posX = x;
    y = body->vecY << 8;
    body->posY = y;
    body->posZ = -1;
    body->unk18 = zero;

    map = *(u8 **)((u8 *)ctx + 0x304);
    flag = 1;
    tile_query(map, x, y, &height, &outB, &outC, (u8)0, flag);
    body->posZ = height;
    hit = probe_body_hit(ctx, body, body->posX, body->posY, height + body->unk18, 1, &hit16);
    if ((u8)hit == 0)
    {
        table = *(u8 **)((u8 *)body + 0x338) + 0x68;
        subHit = (*(u8 (**)(struct Enemy *))(table + 4))((struct Enemy *)((u8 *)body + *(s16 *)table));
        if ((u8)subHit == 0)
        {
            if (((*(u8 *)&hit16 ^ 1) & 1) != 0)
            {
                if ((u8)position_blocked(ctx, body, body->posX, body->posY, body->posZ + body->unk18) == 0)
                {
                    obj_set_cell_coord(body, 0xC, 0);
                    ((struct Flags214 *)&body->unk214)->low3 = flag;
                    sprite = body->sprite;
                    ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
                    obj_set_sprite_link(body, 1);
                    obj_settle_on_tiles(body, 1);
                    *(u8 *)((u8 *)body + 0x217) |= flag;
                    body->unk20E |= 4;
                    *out = face_partner;
                    return;
                }
            }
        }
    }
    if (*(u16 *)(ai + 0x17A) != 0)
    {
        *out = tick_store;
    }
    else
    {
        out[0] = out[1];
        ((struct Flags1D6 *)(ai + 0x17E))->bit0 = 1;
        ((struct Flags1D6 *)(ai + 0x17E))->bit1 = 0;
    }
}
END_NONMATCH






/* Once the current animation has ended, turns the enemy towards the active party member
 * (8-way facing in the low 3 bits of unk24), plays the matching 4-way cell and hands over to
 * timed_step_state, running it at once. owner_entry(ctx, 0xFF) is the active party member. */
void face_partner(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Sprite *sprite = obj->sprite;
    EnemyFn next;

    if ((sprite->unk12 & 8) != 0)
    {
        struct Behavior *ai = (struct Behavior *)((u8 *)obj + 0x58);
        struct Partner *partner = owner_entry(ctx, 0xFF);
        s32 dx = partner->posX - obj->posX;
        s32 dy = partner->posY - obj->posY;
        u32 facing;
        u8 *flags;

        if ((dx | dy) != 0)
        {
            /* round the 16-bit angle to the nearest of 8 directions */
            s32 angle = angle_of_vector16(dx, dy) + 0x1000;

            facing = (angle >> 13) & 7;
        }
        else
        {
            facing = -1;
        }
        flags = &obj->unk24;
        facing &= 7;
        {
            s32 old = *flags;
            s32 value = -8;

            value &= old;
            value |= facing;
            *flags = value;
            obj_set_cell_coord(obj, ((u32)value << 29) >> 30, 0);
        }
        flags = &obj->unk20C;
        {
            s32 bits = *flags;

            bits |= 0x20;
            bits |= 0x40;
            bits |= 0x80;
            *flags = bits;
        }
        flags = &obj->unk210;
        *flags = *flags | 0x10;
        ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
        ((struct Flags12b *)&sprite->unk12)->bit3 = 0;
        ai->holdTimer = 0;
        next = timed_step_state;
        *out = next;
        next(ctx, obj, out);
    }
}









/*
 * Enemy wind-up/hold state. While the wind-up animation plays, unk274 is set from the
 * sprite's progress (unk2C). Once it ends (sprite flag 8), the hold animation (cell + 4)
 * loops for a random time picked from the four u16 at Enemy+0x136; when that runs out
 * (or the pick is 0), the closing animation (cell + 8) plays once and finish_state takes over.
 */
void timed_step_state(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Sprite *sprite = obj->sprite;
    u16 *holdTimer = &obj->unk1D4;
    EnemyFn next;
    s32 pick;
    s32 pickOffset;
    s32 timeLeft;
    u8 *holdTimes;

    timeLeft = *holdTimer;
    if (timeLeft == 0)
    {
        if ((sprite->unk12 & 8) != 0)
        {
            obj_set_cell_coord(obj, (((u32)obj->unk24 << 29) >> 30) + 4, 0);
            pick = rand_below(4, mt19937_next());
            pickOffset = pick << 1;
            holdTimes = (u8 *)obj + 0x136;
            *holdTimer = *(u16 *)(holdTimes + pickOffset);
            ((struct Flags12b *)&sprite->unk12)->bit3 = 0;
            if (*holdTimer != 0)
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
        s32 left = timeLeft - 1;

        *holdTimer = left;
        if ((left << 16) != 0)
        {
            return;
        }
    }
    obj_set_cell_coord(obj, (((u32)obj->unk24 << 29) >> 30) + 8, 0);
    ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
    ((struct Flags12b *)&sprite->unk12)->bit3 = 0;
    next = finish_state;
    *out = next;
    next(ctx, obj, out);
}



/*
 * Closing part of timed_step_state's wind-up/hold/close move: while the closing animation
 * plays, unk274 follows the sprite's progress (0xC past 0x40; past 0x80 it is 4 and the move's
 * flags are cleared). Once the animation ends (sprite flag 8) the sprite is unlinked, and the
 * enemy either goes to tick_store (unk1D2 set) or returns to the state saved in state[1].
 */
void finish_state(void *ctx, struct Enemy *enemy, EnemyFn *state)
{
    struct Sprite *sprite = enemy->sprite;

    if ((sprite->unk12 & 8) != 0)
    {
        ((struct Flags12 *)&sprite->unk12)->mid2 = 1;
        ((struct Flags20D *)&enemy->unk20E)->bit2 = 0;
        obj_set_sprite_link(enemy, 0);
        if (enemy->unk1D2 != 0)
        {
            *state = tick_store;
        }
        else
        {
            state[0] = state[1];
            enemy->unk0F8 &= 0xFBFF;
            ((struct Flags1D6 *)&enemy->unk1D6)->bit0 = 1;
            ((struct Flags1D6 *)&enemy->unk1D6)->bit1 = 0;
        }
    }
    else if (sprite->unk2C > 0x7F)
    {
        ((struct Flags20C *)&enemy->unk20C)->bit5 = 0;
        ((struct Flags20C *)&enemy->unk20C)->bit6 = 0;
        ((struct Flags20C *)&enemy->unk20C)->bit7 = 0;
        ((struct Flags33E *)&enemy->unk210)->bit4 = 0;
        enemy->unk274 = 4;
    }
    else if (sprite->unk2C > 0x3F)
    {
        enemy->unk274 = 0xC;
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




/* Move state: picks a random animation speed, then has collide_react step the enemy a random
 * distance at a random speed near the active partner (or the default one), along an axis the
 * partner's box leaves free. Next: step_with_box_check. */
void random_collide(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    struct EnemyMoveTable *moves;
    u8 activeIdx;
    s32 pick;
    s32 distPick;
    u16 speed;
    u16 dist;
    u8 index;

    moves = (struct EnemyMoveTable *)((u8 *)ent + 0x100);
    pick = rand_below(4, mt19937_next());
    ent->sprite->unk20 = (moves->words + pick)[MOVE_ANIM_SPEED];
    ((struct Flags214 *)&ent->unk214)->low3 = 4;
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 0;
    pick = rand_below(4, mt19937_next());
    speed = (moves->words + pick)[MOVE_SPEED];
    distPick = rand_below(2, mt19937_next());
    dist = (moves->words + distPick)[MOVE_DIST];
    if (find_active_entry(owner, &activeIdx) != 0)
    {
        index = activeIdx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    collide_react(owner, ent, ((struct Partner **)((u8 *)owner + 0x28))[index], dist, speed, (s8)moves->chanceX, (s8)moves->chanceY, 0x22, 0);
    *out = step_with_box_check;
}



/* Move state after random_collide: steps the enemy along its move. Every Record.unk1A frames it looks
 * for the active partner in its sight box (and, when the enemy has a second box, in that one too) and
 * goes to load_record_state if it finds one. When the move ends it waits a random MOVE_WAIT_TIME in
 * tick_store, then moves again with random_collide. */
void step_with_box_check(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    u16 *moves = ent->wanderDist;
    struct Record *rec = ent->rec;
    struct Partner *partner;
    s16 box[8];
    u16 checkInterval;
    u16 waitTime;

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
        checkInterval = rec->unk1A;
        ai->holdTimer = checkInterval;
    } else {
        ent->unk1D4 = ent->unk1D4 - 1;
    }
    if (step_pos_xy(ent) == 0) {
        s32 pick = rand_below(4, mt19937_next());
        waitTime = moves[pick + MOVE_WAIT_TIME];
        ai->timerB = waitTime;
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

// Heads back to where the current move started (ai->homeX/homeY): sets the 256-step heading
// and the 4-way facing cell toward it, the animation speed (record byte table at +0x36) and
// the move speed (record u16 table at +0x3A) of the move's variant, and starts the move.
// Next step: enemy_box_point_then_collide.
void aim_and_start_move(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)obj + 0x58);
    u8 *rec = (u8 *)obj->rec;
    s32 dx = obj->homeX - obj->posX;
    s32 dy = obj->homeY - obj->posY;
    u32 heading;
    s32 toHomeX;
    s32 toHomeY;
    s32 dir8;
    s32 cell;
    u32 speed;
    s32 (*divFn)(s32, s32);
    u8 zero = 0;

    if ((dx | dy) != 0)
    {
        u32 angle = angle_of_vector16(dx, dy);

        heading = (angle + 0x10) >> 8;
    }
    else
    {
        heading = 0xFF;
    }
    obj->heading = heading;
    obj->sprite->unk20 = ((u8 *)(rec + 0x36))[ai->index];
    ((struct Flags214 *)&obj->unk214)->low3 = 4;
    ((struct Flags12 *)&obj->sprite->unk12)->mid2 = 0;
    divFn = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    toHomeX = ai->homeX - obj->posX;
    toHomeY = ai->homeY - obj->posY;
    if ((toHomeX | toHomeY) != 0)
    {
        // nearest of 8 directions
        s32 angle = (u16)angle_of_vector16(toHomeX, toHomeY) + 0x1000;

        dir8 = angle >> 13;
        dir8 &= 7;
    }
    else
    {
        dir8 = -1;
    }
    cell = divFn(dir8, 2);
    obj_set_cell_coord(obj, cell, 0);
    speed = ((u16 *)(rec + 0x3A))[ai->index];
    start_move_to(obj, ai->homeX, ai->homeY, speed, zero);
    *out = enemy_box_point_then_collide;
}


/* Enemy entry state for hovering hoppers: sets flag bit 4 of unk33E, reloads the record fields and
 * starts the hover bob at the current height (phase 0, bob speed Record.unk23). Parameter word 5
 * of the enemy's box data picks the mode: 1 stands on the ground (run_state_handler with
 * enemy_land_reload_sprite), anything else hovers and hops (enemy_hop_state_runner with
 * enemy_hop_start_kick). */
void init_state_from_box(void *ctx, struct Enemy *obj)
{
    struct Record *rec = obj->rec;
    s16 *params = obj->box;
    u8 *flags;

    flags = &obj->unk33E;
    *flags = *flags | 0x10;
    reset_record_fields(obj);
    /* hover base: ground z plus the current height above it */
    obj->unk2EC = obj->posZ + obj->unk18;
    obj->unk2F4 = 0;
    obj->unk2F8 = rec->unk23;
    obj->unk1D4 = 0;
    if (((u16 *)params)[5] == 1)
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

/* Enemy main state for hovering hoppers: runs the current sub-state, then (unless it is falling,
   jumping or landing) bobs the enemy: its height above the ground is the hover base plus
   amplitude * sin(phase) / 64, the phase advancing with the sprite's animation speed, and the hover
   base drifts towards Record.unk24 * 16 px above posZ by Record.unk26 per frame. */
void enemy_hop_state_runner(u8 *owner, struct Enemy *ent)
{
    struct Record *rec = ent->rec;
    EnemyFn *subState = ent->unk1A0;
    EnemyFn current;
    u8 amplitude;
    s32 *hoverZ;
    s32 baseHeight;
    s32 bob;
    s32 phaseStep;
    s32 bobSpeed;
    s8 animSpeed;
    u16 height;
    u16 rate;
    s32 targetHeight;

    (*subState)(owner, ent, subState);
    current = *subState;
    if (current == enemy_hop_prepare_fall || current == enemy_hop_fall_then_next || current == enemy_hop_jump_to_partner
        || current == enemy_xyz_step_done || current == enemy_hop_land_on_partner || current == enemy_hop_land_next
        || current == enemy_hop_fall_to_tile || current == enemy_after_fall_check_box)
    {
        return;
    }
    amplitude = rec->unk22;
    hoverZ = &ent->unk2EC;
    baseHeight = *hoverZ - ent->posZ;
    bob = gSineTable[ent->unk2F4 & 0xFF] / 0x40;
    ent->unk18 = baseHeight + amplitude * bob;
    bobSpeed = (s16)ent->unk2F8;
    animSpeed = ent->sprite->unk20;
    phaseStep = animSpeed * bobSpeed + 8;
    ent->unk2F4 = ((phaseStep / 0x10) + ent->unk2F4) & 0xFF;
    height = rec->unk24;
    rate = rec->unk26;
    targetHeight = height << 12;
    if (*hoverZ - ent->posZ > targetHeight)
    {
        *hoverZ -= rate;
        if (*hoverZ - ent->posZ <= targetHeight)
            *hoverZ = ent->posZ + targetHeight;
    }
    else if (*hoverZ - ent->posZ < targetHeight)
    {
        *hoverZ += rate;
        if (*hoverZ - ent->posZ >= targetHeight)
            *hoverZ = ent->posZ + targetHeight;
    }
}

extern const u8 gFrameAttrTable[];
void enemy_hop_wait_partner(u8 *owner, struct Enemy *ent, EnemyFn *out);
/* Hop state: starts a random step near the active partner (or the default one). Picks a
 * random animation speed, sets the sprite cell for the enemy's facing sector, then has
 * collide_react step it a random distance at a random speed along an axis the partner's
 * box leaves free. Next: enemy_hop_wait_partner. */
void enemy_hop_start_kick(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    struct EnemyMoveTable *moves;
    s32 pick;
    u16 speed;
    u16 dist;
    u8 activeIdx;
    u8 index;
    struct Partner *partner;

    moves = (struct EnemyMoveTable *)((u8 *)ent + 0x100);
    pick = rand_below(4, mt19937_next());
    ent->sprite->unk20 = (moves->words + pick)[MOVE_ANIM_SPEED];
    ((struct Flags214 *)&ent->unk214)->low3 = 4;
    ((struct Flags12 *)&ent->sprite->unk12)->mid2 = 0;
    ((struct Flags214 *)&ent->unk214)->low3 = 4;
    obj_set_cell_coord(ent, gFrameAttrTable[(((u32)ent->unk216 << 28) >> 30) * 0x10 + (((u32)ent->unk24 << 29) >> 29) + 8], 0);
    if (find_active_entry(owner, &activeIdx) != 0)
    {
        index = activeIdx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = *(struct Partner **)((u8 *)owner + index * 4 + 0x28);
    pick = rand_below(4, mt19937_next());
    speed = (moves->words + pick)[MOVE_SPEED];
    pick = rand_below(2, mt19937_next());
    dist = (moves->words + pick)[MOVE_DIST];
    collide_react(owner, ent, partner, dist, speed, (s8)moves->chanceX, (s8)moves->chanceY, 0x22, 0);
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

// Hop state: jump at the active partner (or the default one). Picks a random variant 0-3
// for the jump height and speed, starts the jump, faces the partner along the longer
// axis, sets the jump cell and animation speed, and remembers where the jump started.
void enemy_hop_jump_to_partner(struct Owner *owner, struct Enemy *ent, EnemyFn *out)
{
    u8 one = 1;
    u16 variant;
    u16 variant2;
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    struct Record *rec = ent->rec;
    u8 activeIdx;
    u32 partnerIdx;
    struct Partner *partner;
    s32 partnerTop;
    s32 entTop;
    s32 jumpHeight;
    s32 jumpDz;
    s32 dx;
    s32 dy;
    s32 absDx;
    s32 absDy;
    struct Sprite *sprite;
    u8 cell;
    u16 speed;

    if (find_active_entry(owner, &activeIdx) != 0)
    {
        partnerIdx = activeIdx;
    }
    else
    {
        partnerIdx = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = *(struct Partner **)((u8 *)owner + 0x28 + (partnerIdx << 2));

    ai->index = rand_below(4, mt19937_next());
    variant = ai->index;
    jumpHeight = *(u8 *)((u8 *)ai->rec + (variant + 0x34));

    // Jump high enough to clear the partner's top (posZ + unk18 is the height above ground).
    partnerTop = partner->posZ + partner->unk18;
    entTop = ent->posZ + ent->unk18;
    if (entTop > partnerTop)
    {
        jumpDz = jumpHeight << 12;
    }
    else
    {
        jumpDz = partnerTop + (jumpHeight << 12) - entTop;
    }

    variant2 = ai->index;
    speed = *(u16 *)((u8 *)ai->rec + (variant2 * 2) + 0x2C);
    start_move_to_3d(ent, 0, 0, jumpDz, speed, one);

    dx = partner->posX - ent->posX;
    absDx = dx;
    if (dx < 0)
    {
        absDx = -dx;
    }
    dy = partner->posY - ent->posY;
    absDy = dy;
    if (dy < 0)
    {
        absDy = -dy;
    }

    // Facing (low 3 bits of unk24): 0 = -y, 2 = +x, 4 = +y, 6 = -x.
    if (absDx > absDy)
    {
        if (dx >= 0)
        {
            ((struct Flags12b *)&ent->unk24)->low3 = 2;
        }
        else
        {
            ((struct Flags12b *)&ent->unk24)->low3 = 6;
        }
    }
    else if (dy >= 0)
    {
        ((struct Flags12b *)&ent->unk24)->low3 = 4;
    }
    else
    {
        ((struct Flags12b *)&ent->unk24)->low3 = 0;
    }

    ((struct Flags214 *)&ent->unk214)->low3 = 1;
    cell = gFrameAttrTable[(((u32)ent->unk216 << 28) >> 30) * 0x10 + (((u32)ent->unk24 << 29) >> 29) + 8];
    obj_set_cell_coord(ent, cell, 0);

    sprite = ent->sprite;
    sprite->unk20 = *(u8 *)((u8 *)rec + 0x28 + ai->index);

    ai->homeX = ent->posX;
    ai->homeY = ent->posY;
    ai->homeZ = ent->posZ;
    ai->homeDZ = ent->unk18;
    *out = enemy_xyz_step_done;
}
