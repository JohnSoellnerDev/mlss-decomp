#include "global.h"
#include "ram_routines.h"

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

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
    s16 unk10;          /* unk10..unk16: the enemy's zone, left, top, right, bottom in 16 px tiles */
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
    u8 unk24; /* bits 0-2 (struct Mode3): facing direction, 0-7 */
    u8 filler25[0x198 - 0x25];
    void *unk198;       /* AI state run each frame */
    void *unk19C;       /* state enemy_enter_queued_state switches to */
    void *unk1A0[2];    /* movement handler, called by the state with &unk1A0 */
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
s8 collide_react(struct Owner *owner, struct Enemy *ent, struct Partner *bro, u32 speed, u32 a, s32 b, s32 c, u32 d, u32 e);
s8 collide_wander_diag(u32 unused, struct Enemy *obj, struct Partner *other, u16 size, u16 speed, s8 dirA, s8 dirB, u16 extA, u16 extB);
void enemy_wait_sense(void);
u16 angle_of_vector16(s32 x, s32 y);
void enemy_wait_spot_owner(struct Owner *owner, struct Enemy *ent);
void enemy_chase_step(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void enemy_move_body(void);
void enemy_body_contact(struct Owner *owner, struct Enemy *ent, u32 *state_out);
void enemy_timer_step(void);
void enemy_run_body_handler(void);
void obj_box_to_stack(s16 *box, struct Enemy *ent, u32 a, u32 b);
void set_half_23e(void *obj, s16 v);
void move_step_handler(void);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void start_move_to_3d(struct Enemy *obj, s32 x, s32 y, s32 z, u16 speed, ByteArg zero);
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
void enemy_idle_update(struct Owner *owner, struct Enemy *ent);
void obj_find_active_state(void);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
u8 boxes_overlap_oriented(s16 *box, struct Enemy *ent, void *entry, s32 zAbove, s32 zBelow);

/* Once the sprite's current animation has finished (flags12 bit 3): picks one of four moves at
 * random, its kind from the u16 table at Enemy+0x108 (into unk239 and sprite->unk20) and its
 * speed from the table at Enemy+0x100 (into unk23E), sets the moving flags and starts moving to
 * the sensor's point (unk18/1A/1C, in 16 px tiles; unk1B8-1BC get it in pixels).
 * enemy_check_flags then runs the move. */
void enemy_start_random_move(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    struct Sprite **spritep = &ent->sprite;

    if (((*spritep)->flags12 & 8) != 0)
    {
        u8 *speeds = (u8 *)ent + 0x100;
        struct SensorBox *sensor = ent->sensor;
        struct Sprite *sprite;
        u32 move;
        u32 offset;
        u8 *kinds;
        u16 kind;

        ((struct Mode3 *)&ent->unk214)->mode = 3;
        move = rand_below(4, mt19937_next());
        sprite = *spritep;
        offset = (u16)move << 1;
        kinds = (u8 *)ent + 0x108;
        kind = *(u16 *)(kinds + offset);
        ent->unk239 = kind;
        sprite->unk20 = kind;
        ent->unk23E = *(u16 *)(speeds + offset);
        ((struct Flags8 *)&ent->unk20C)->bit5 = 1;
        ((struct Flags8 *)&ent->unk20C)->bit7 = 1;
        ((struct Flags8 *)&ent->unk20C)->bit6 = 1;
        ((struct Flags8 *)&ent->unk210)->bit4 = 1;
        {
            s32 tileX = sensor->unk18;
            ent->unk1B8 = tileX << 4;
        }
        {
            s32 tileY = sensor->unk1A;
            ent->unk1BA = tileY << 4;
        }
        {
            s32 tileZ = sensor->unk1C;
            ent->unk1BC = tileZ << 4;
        }
        obj_start_move_quarter(ent, 1, ent->unk1B8 << 8, ent->unk1BA << 8);
        obj_move_interp(ent);
        *state_out = (u32)enemy_check_flags;
    }
}

/*
 * Wait state (enemy_setup_wait): runs the movement handler and counts unk1D4 down. Once it is 0,
 * if the active party member stands inside the enemy's zone (sensor) and overlaps its hit box
 * (rec[0x18..0x1D]), the enemy spots it: enemy_enter_queued_state, then enemy_chase_owner_step.
 */
void enemy_wait_spot_owner(struct Owner *owner, struct Enemy *ent)
{
    u8 *handlerSlot = (u8 *)ent + 0x1A0;
    void (*handler)(struct Owner *, struct Enemy *, u8 *) = *(void (**)(struct Owner *, struct Enemy *, u8 *))handlerSlot;
    u8 activeIdx;

    handler(owner, ent, handlerSlot);
    if (ent->unk1D4 == 0)
    {
        if (find_active_entry(owner, &activeIdx) != 0)
        {
            struct SensorBox *zone = ent->sensor;
            void *target = *(struct Partner **)((u8 *)owner + activeIdx * 4 + 0x28);
            s16 box[8];
            s32 tile;

            /* zone tiles to tile centres in pixels: left, right, bottom, top */
            box[0] = ((tile = zone->unk10) << 4) + 8;
            box[3] = ((tile = zone->unk12) << 4) + 8;
            box[1] = ((tile = zone->unk14) << 4) + 8;
            box[2] = ((tile = zone->unk16) << 4) + 8;
            if (point_in_box(box, target, 0x7FFF, 0) != 0)
            {
                struct RecBox *rec = ent->rec;

                /* hit box bytes: x0, y0, z above, x1, y1, z below */
                box[0] = rec->unk18 << 4;
                box[2] = rec->unk19 << 4;
                box[4] = rec->unk1A << 4;
                box[1] = rec->unk1B << 4;
                box[3] = rec->unk1C << 4;
                box[5] = rec->unk1D << 4;
                if (boxes_overlap_oriented(box, ent, target, box[4], box[5]) != 0)
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

// Chase step, set by enemy_wait_spot_owner once the enemy has spotted a bro: turns the
// enemy toward the active bro (one of 8 directions), runs its movement handler (0x1A0),
// and when bit 0 of 0x1D6 is set checks the bro again. Inside the sensor area and the
// record's attack box (rec + 0x34) it engages (enemy_engage); otherwise it gives up and
// goes back to waiting (enemy_setup_wait). Offsets below are from ai = ent + 0x58.
void enemy_chase_owner_step(struct Owner *owner, struct Enemy *ent)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 leadIdx;
    struct Partner *bro;
    s32 dx;
    s32 dy;
    s32 dir;
    void (*moveFn)(struct Owner *, struct Enemy *, u8 *);
    u8 *moveSlot;

    if (find_active_entry(owner, &leadIdx) == 0)
    {
        leadIdx = owner->flags208.bit2;
    }
    bro = *(struct Partner **)((u8 *)owner + leadIdx * 4 + 0x28);
    dx = bro->posX - ent->posX;
    dy = bro->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        /* round the angle to the nearest of 8 directions */
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
    moveSlot = ai + 0x148;
    moveFn = *(void (**)(struct Owner *, struct Enemy *, u8 *))moveSlot;
    moveFn(owner, ent, moveSlot);
    if ((ai[0x17E] & 1) != 0)
    {
        struct SensorBox *sensor = *(struct SensorBox **)(ai + 0x15C);
        s16 box[8];
        s32 sv;

        bro = *(struct Partner **)((u8 *)owner + leadIdx * 4 + 0x28);
        box[0] = ((sv = sensor->unk10) << 4) + 8;
        box[3] = ((sv = sensor->unk12) << 4) + 8;
        box[1] = ((sv = sensor->unk14) << 4) + 8;
        box[2] = ((sv = sensor->unk16) << 4) + 8;
        if (point_in_box(box, bro, 0x7FFF, 0) != 0)
        {
            s8 *rec = *(s8 **)(ai + 0x158);
            s8 *attackBox = rec + 0x34;

            ((struct Mode3 *)&ent->unk214)->mode = 3;
            box[0] = *attackBox << 4;
            attackBox++;
            box[2] = *attackBox << 4;
            attackBox++;
            box[4] = *attackBox << 4;
            attackBox++;
            box[1] = *attackBox << 4;
            attackBox++;
            box[3] = *attackBox << 4;
            attackBox++;
            box[5] = *attackBox << 4;
            if (boxes_overlap_oriented(box, ent, bro, box[4], box[5]) != 0)
            {
                *(void **)(ai + 0x140) = (void *)obj_state_run_advance;
                *(void **)(ai + 0x144) = (void *)enemy_setup_wait;
                *(void **)moveSlot = (void *)enemy_engage;
                return;
            }
        }
        *(void **)(ai + 0x140) = (void *)enemy_setup_wait;
    }
}

/* Puts the enemy to rest: marks it (unk33E bit 4, unk33D bit 3), clears its record fields and its
 * timer, resets its body's state bits (unk20D bit 2 off, bit 3 on, bits 5-6 off; mode unk214 = 0;
 * unk210 bit 3 off; the 3-bit field at bits 15-17 of the word at +0x20C = 1; unk20F bit 2 on),
 * unlinks the body's sprite and deactivates it. The AI then runs run_state_handler with
 * enemy_start_move as its movement handler. */
void enemy_go_dormant(struct Owner *owner, struct Enemy *ent)
{
    struct Enemy *body = ent->body;

    {
        u8 *flags = &ent->unk33E;
        *flags = *flags | 0x10;
    }
    reset_record_fields(ent);
    ent->unk1D4 = 0;
    {
        int bits = ent->unk33D;
        bits = bits | 0x08;
        ent->unk33D = bits;
    }
    ((struct Flags8 *)&body->unk20D)->bit2 = 0;
    ((struct Mode3 *)&body->unk214)->mode = 0;
    ((struct Flags8 *)&body->unk210)->bit3 = 0;
    {
        int bits = body->unk20D;
        bits = bits | 0x08;
        bits = bits & ~0x20;
        bits = bits & ~0x40;
        body->unk20D = bits;
    }
    *(u32 *)((u8 *)body + 0x20C) = (*(u32 *)((u8 *)body + 0x20C) & ~(7 << 15)) | (1 << 15);
    {
        u8 *flags = &body->unk20F;
        *flags = *flags | 0x04;
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

/* Walk state (set by enemy_start_move). Every unk1D4 frames (reloaded from rec + 0x1A) it looks for the
 * active bro in its sight box (rec + 0x1C): a bro in sight ends the walk, and the enemy waits
 * rec[0x26 + attack] frames with animation rec[0x22 + attack] (enemy_wait_timer, attack = unk1D0).
 * When the walk's steps run out it pauses for one of its 4 wait times (ent + 0x128) and walks again. */
void enemy_walk(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct RecBox *rec = ent->rec;

    if (ent->unk1D4 == 0)
    {
        s16 sightBox[8];
        u8 broIdx;
        u16 interval;

        if (find_active_entry(owner, &broIdx) != 0)
        {
            struct Partner *bro = *(struct Partner **)((u8 *)owner + broIdx * 4 + 0x28);

            load_box_oriented(sightBox, ent, 0x11, 0x1C);
            if (boxes_overlap_oriented(sightBox, ent, bro, sightBox[4], sightBox[5]) != 0)
            {
                ent->unk1D2 = *(u8 *)(ent->unk1D0 + (u32)rec + 0x26);
                ent->sprite->unk20 = *(u8 *)(ent->unk1D0 + (u32)rec + 0x22);
                ((struct Mode3 *)&ent->unk214)->mode = 1;
                *state_out = (u32)enemy_wait_timer;
                return;
            }
        }
        interval = *(u16 *)((u8 *)rec + 0x1A);
        *(u16 *)(ai + 0x17C) = interval; /* unk1D4 */
    }
    else
    {
        ent->unk1D4--;
    }
    if (step_pos_xy(ent) == 0)
    {
        u32 pick = rand_below(4, mt19937_next());
        u32 off = pick << 1;
        u8 *waitTimes = ai + 0xD0; /* ent + 0x128 */
        u16 wait = *(u16 *)(waitTimes + off);

        *(u16 *)(ai + 0x17A) = wait; /* unk1D2 */
        *state_out = (u32)tick_store;
        state_out[3] = (u32)enemy_start_move;
    }
}

/*
 * Enemy state after enemy_walk found a bro in front of it: counts the wait timer unk1D2 down while
 * turning to face the active bro (4 facings, cell = direction / 2). When it runs out the enemy picks
 * a random attack (unk1D0, 0-3) and goes to enemy_approach_bro. With no active bro left it waits a
 * random one of its 4 wait times (u16s at ent + 0x128) in tick_store, then walks on (enemy_start_move).
 */
void enemy_wait_timer(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    u8 *params = (u8 *)ent + 0x100;
    u8 broIdx;
    u8 found;
    register struct Owner *own asm("r6");

    found = find_active_entry(owner, &broIdx);
    own = owner;
    if (found == 0)
    {
        ent->unk1D2 = ((u16 *)params)[rand_below(4, mt19937_next()) + 20];
        *state_out = (u32)tick_store;
        state_out[3] = (u32)enemy_start_move;
    }
    else if (--ent->unk1D2 == 0)
    {
        ent->unk1D0 = rand_below(4, mt19937_next());
        *state_out = (u32)enemy_approach_bro;
    }
    else
    {
        struct Partner *bro = *(struct Partner **)((u8 *)own + broIdx * 4 + 0x28);
        u8 *facing = &broIdx;
        s32 (*div)(s32, s32) = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
        s32 dx = bro->posX - ent->posX;
        s32 dy = bro->posY - ent->posY;
        s32 dir;

        if ((dx | dy) != 0)
        {
            /* round the angle to the nearest of 8 directions */
            dir = (((s32)((u16)angle_of_vector16(dx, dy) + 0x1000)) >> 13) & 7;
        }
        else
        {
            dir = -1;
        }
        /* broIdx's slot is reused for the new facing */
        *facing = div(dir, 2);
        if (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2) != broIdx)
        {
            obj_set_cell_coord(ent, broIdx, ent->sprite->unk23);
        }
    }
}

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

NONMATCH("asm/code_081442FC/enemy_chase_step.s", void enemy_chase_step(struct Owner *owner, struct Enemy *ent, u32 *state_out))
{
    u8 zero = 0;
    u8 idx;
    u8 active;
    u8 *ai = (u8 *)ent + 0x58;
    u8 *hit = (u8 *)ent->rec;
    struct Enemy *body = ent->body;
    struct Partner *bro;
    u8 *ctx;
    u8 *cb;
    s32 mirrorX;
    s32 mirrorY;
    s32 zBase;
    s32 zOff;
    s32 zTarget;
    s32 dx;
    s32 dy;
    s32 absDx;
    s32 absDy;
    u16 speed;

    if (find_active_entry(owner, &idx) != 0)
    {
        active = idx;
    }
    else
    {
        active = owner->flags208.bit1;
    }
    bro = *(struct Partner **)((u8 *)owner + active * 4 + 0x28);
    mirrorX = bro->posX + (bro->posX - ent->posX);
    mirrorY = bro->posY + (bro->posY - ent->posY);
    ctx = *(u8 **)((u8 *)owner + 0x200);
    if (*(s16 *)(ctx + 4) == 0x55)
    {
        cb = *(u8 **)((u8 *)owner + 0x204);
        zBase = *(s32 *)(cb + 0x14) + *(s32 *)(cb + 0x18);
        zOff = 0x3600;
    }
    else
    {
        zBase = *(s32 *)(ctx + 0x14);
        zOff = 0x1800;
    }
    zTarget = zBase + zOff;
    speed = *(u16 *)(hit + (*(u16 *)(ai + 0x178) << 1) + 0x2E);
    dx = mirrorX - ent->posX;
    dy = mirrorY - ent->posY;
    start_move_to_3d(body, mirrorX, mirrorY, zTarget, speed, zero);
    absDx = dx;
    if (absDx < 0)
    {
        absDx = -absDx;
    }
    absDy = dy;
    if (absDy < 0)
    {
        absDy = -absDy;
    }
    if (absDx > absDy)
    {
        ent->sprite->unk21 = 4;
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = (u32)dx >> 31;
    }
    else
    {
        struct Sprite *s = ent->sprite;
        s32 anim = 3;

        if (dy >= 0)
        {
            anim = 5;
        }
        s->unk21 = anim;
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 0;
    }
    obj_load_record(ent->sprite, ent->sprite->unk21, 1, 0, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    body->sprite->unk21 = ent->sprite->unk21 + 3;
    {
        struct Sprite *bs = body->sprite;
        s32 src = ent->sprite->unk11;
        s32 b = bs->unk11;

        b = b & -0x41;
        b = b | (src & 0x40);
        bs->unk11 = b;
    }
    obj_refresh_sub_attr(body);
    obj_set_active(body, 1);
    obj_set_sprite_link(body, 1);
    *state_out = (u32)enemy_move_body;
}
END_NONMATCH

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

/* Wait state set by enemy_setup_wait_b: runs the enemy's movement handler (ai + 0x148, i.e. unk1A0)
 * and its body's, if it has one, and counts unk1D4 down. Once it is 0, an active bro that overlaps
 * the enemy's hit box (rec bytes 0x18..0x1D) sends it to enemy_setup_idle, which goes on to
 * enemy_idle_runner. */
void enemy_wait_b_runner(struct Owner *owner, struct Enemy *ent)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *bodyAi = (u8 *)ent->body + 0x58;
    void (*handler)(struct Owner *, struct Enemy *, u8 *);
    u8 *handlerSlot;
    s32 timer;

    handlerSlot = ai + 0x148;
    handler = *(void (**)(struct Owner *, struct Enemy *, u8 *))handlerSlot;
    handler(owner, ent, handlerSlot);
    handlerSlot = bodyAi + 0x148;
    handler = *(void (**)(struct Owner *, struct Enemy *, u8 *))handlerSlot;
    if (handler != 0)
    {
        handler(owner, ent, handlerSlot);
    }
    timer = ent->unk1D4;
    if (timer == 0)
    {
        u8 activeIdx[4];
        s16 box[8];

        if (find_active_entry(owner, activeIdx) != 0)
        {
            struct RecBox *rec = ent->rec;
            void *bro = *(void **)((u8 *)owner + 0x28 + (activeIdx[0] << 2));

            /* hit box bytes: x0, y0, z above, x1, y1, z below */
            box[0] = rec->unk18 << 4;
            box[2] = rec->unk19 << 4;
            box[4] = rec->unk1A << 4;
            box[1] = rec->unk1B << 4;
            box[3] = rec->unk1C << 4;
            box[5] = rec->unk1D << 4;
            if (boxes_overlap_oriented(box, ent, bro, box[4], box[5]) != 0)
            {
                ent->unk198 = (void *)enemy_setup_idle;
                ent->unk19C = (void *)enemy_idle_runner;
            }
        }
    }
    else
    {
        ent->unk1D4 = timer - 1;
    }
}

/* AI state of an idle enemy: turns it to face the active bro (one of 8 directions), runs its own
 * per-frame handler (ai + 0x148) and its body's. Once flag bit 0 (ai + 0x17E) is set, it goes on
 * to enemy_idle_update if the bro is inside its sight box (the record's 6 bytes at 0x34, in 16ths
 * of a pixel), otherwise it waits (mode 3, enemy_setup_wait_b). */
void enemy_idle_runner(struct Owner *owner, struct Enemy *ent)
{
    u8 broIndex;
    u8 *ai = (u8 *)ent + 0x58;
    u8 *bodyAi = (u8 *)ent->body + 0x58;
    struct Partner *bro;
    s32 dx;
    s32 dy;
    s32 dir;
    void (*handler)(struct Owner *, struct Enemy *, u8 *);
    u8 *handlerSlot;
    u8 *bodySlot;

    if (find_active_entry(owner, &broIndex) == 0)
    {
        broIndex = owner->flags208.bit2;
    }
    bro = *(struct Partner **)((u8 *)owner + broIndex * 4 + 0x28);
    dx = bro->posX - ent->posX;
    dy = bro->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        /* round the 16-bit angle to the nearest eighth of a turn */
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
    handlerSlot = ai + 0x148;
    handler = *(void (**)(struct Owner *, struct Enemy *, u8 *))handlerSlot;
    handler(owner, ent, handlerSlot);
    bodySlot = bodyAi + 0x148;
    handler = *(void (**)(struct Owner *, struct Enemy *, u8 *))bodySlot;
    if (handler != 0)
    {
        handler(owner, ent, bodySlot);
    }
    if ((ai[0x17E] & 1) != 0)
    {
        s8 *record = *(s8 **)(ai + 0x158);
        s16 sightBox[8];

        s8 *boxBytes = record + 0x34;
        sightBox[0] = *boxBytes << 4;
        boxBytes++;
        sightBox[2] = *boxBytes << 4;
        boxBytes++;
        sightBox[4] = *boxBytes << 4;
        boxBytes++;
        sightBox[1] = *boxBytes << 4;
        boxBytes++;
        sightBox[3] = *boxBytes << 4;
        boxBytes++;
        sightBox[5] = *boxBytes << 4;
        if (boxes_overlap_oriented(sightBox, ent, bro, sightBox[4], sightBox[5]) != 0)
        {
            *(void **)(ai + 0x140) = (void *)enemy_idle_update;
            *(void **)handlerSlot = (void *)obj_find_active_state;
            *(void **)(ai + 0x14C) = (void *)obj_find_active_state;
        }
        else
        {
            ((struct Mode3 *)&ent->unk214)->mode = 3;
            *(void **)(ai + 0x140) = (void *)enemy_setup_wait_b;
        }
    }
}

/* AI state while a bro is in sight (entered from enemy_idle_runner). Runs the enemy's per-frame
   handler while it is inside its area box (else flags it as out of range), and the body's.
   Out of range (flag bit 0): back at its home spot it settles (cell from gFrameAttrTable, mode
   0x10, then enemy_timer_step / enemy_run_body_handler); otherwise it waits the record's time
   (mode 3, enemy_setup_wait_b) or goes back to enemy_setup_idle, and runs that state at once.
   In range: it falls back to enemy_setup_idle once the active bro leaves its sight box (the
   record's 6 bytes at 0x34, in 16ths of a pixel).
   AI block (ent + 0x58): +0x140 state, +0x144 next state, +0x148 per-frame handler,
   +0x158 record, +0x17A wait timer, +0x17E flags. */
void enemy_idle_update(struct Owner *owner, struct Enemy *ent)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *bodyAi = (u8 *)ent->body + 0x58;
    s16 areaBox[8];

    obj_box_to_stack(areaBox, ent, 0x22, 0);
    if (point_in_box(areaBox, ent, 0x7FFF, 0) != 0)
    {
        void (*handler)(struct Owner *, struct Enemy *, u8 *) = *(void (**)(struct Owner *, struct Enemy *, u8 *))((u8 *)ent + 0x1A0);

        handler(owner, ent, (u8 *)ent + 0x1A0);
    }
    else
    {
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
    }
    {
        u8 *bodySlot = bodyAi + 0x148;
        void (*bodyHandler)(struct Owner *, struct Enemy *, u8 *) = *(void (**)(struct Owner *, struct Enemy *, u8 *))bodySlot;

        if (bodyHandler != 0)
        {
            bodyHandler(owner, ent, bodySlot);
        }
    }
    if ((ai[0x17E] & 1) != 0)
    {
        /* out of range: settle at home, or wait / go idle */
        if (*(s32 *)((u8 *)ent + 0x2CC) == ent->posX
            && *(s32 *)((u8 *)ent + 0x2D0) == ent->posY
            && (*((u8 *)ent->body + 0x20C) & 1) == 0)
        {
            obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 4, 0);
            flags_obj_set_mode(ent, 0x10);
            *(void **)(ai + 0x140) = (void *)enemy_timer_step;
            *(void **)(ai + 0x144) = (void *)enemy_run_body_handler;
        }
        else
        {
            if (((((u32)*((u8 *)ent + 0x213) << 27) >> 28) & 3) == 0)
            {
                u8 *rec;
                u16 waitTime;

                ((struct Mode3 *)&ent->unk214)->mode = 3;
                rec = *(u8 **)(ai + 0x158);
                waitTime = *(u16 *)(rec + 0x16);
                *(u16 *)(ai + 0x17A) = waitTime;
                if (*(u16 *)(rec + 0x16) != 0)
                {
                    *(void **)(ai + 0x140) = (void *)obj_state_run_advance;
                    *(void **)(ai + 0x148) = (void *)tick_store;
                    *(void **)(ai + 0x144) = (void *)enemy_setup_wait_b;
                }
                else
                {
                    *(void **)(ai + 0x140) = (void *)enemy_setup_wait_b;
                }
            }
            else
            {
                *(void **)(ai + 0x140) = (void *)enemy_setup_idle;
            }
        }
        {
            void (*state)(struct Owner *, struct Enemy *) = *(void (**)(struct Owner *, struct Enemy *))(ai + 0x140);

            state(owner, ent);
        }
    }
    else
    {
        /* in range: lose interest once the active bro leaves the sight box */
        s16 sightBox[8];
        u8 broIndex;
        struct Partner *bro;
        s8 *boxBytes = *(s8 **)(ai + 0x158) + 0x34;

        if (find_active_entry(owner, &broIndex) == 0)
        {
            broIndex = owner->flags208.bit2;
        }
        bro = *(struct Partner **)((u8 *)owner + broIndex * 4 + 0x28);
        sightBox[0] = *boxBytes << 4;
        boxBytes++;
        sightBox[2] = *boxBytes << 4;
        boxBytes++;
        sightBox[4] = *boxBytes << 4;
        boxBytes++;
        sightBox[1] = *boxBytes << 4;
        boxBytes++;
        sightBox[3] = *boxBytes << 4;
        boxBytes++;
        sightBox[5] = *boxBytes << 4;
        if (boxes_overlap_oriented(sightBox, ent, bro, sightBox[4], sightBox[5]) == 0)
        {
            *(void **)(ai + 0x140) = (void *)enemy_setup_idle;
        }
    }
}

ASM_FUNC("asm/code_081442FC/enemy_timer_step.s", void enemy_timer_step(void))

/* AI state: if the enemy's body is not moving (body flag 0x20C bit 0 clear) there is no next
   state. If the body hits something it is unlinked and stopped and the enemy waits
   (mode 3, enemy_setup_wait_b). Otherwise the body starts moving as far as it can in the
   direction gFrameAttrTable gives for the enemy's row and facing (0 up, 1 right, 2 down,
   3 left), and the next state is enemy_after_contact. */
void enemy_body_contact(struct Owner *owner, struct Enemy *ent, u32 *state_out)
{
    struct Enemy *body = ent->body;
    s32 dir;
    u32 moveX;
    u32 moveY;

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
        ent->unk198 = (void *)enemy_setup_wait_b; /* AI state (ai + 0x140) */
        *state_out = 0;
        return;
    }
    dir = gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8];
    /* a move relative to the body, out to the edge of the coordinate range */
    switch (dir)
    {
    case 0:
        moveX = 0;
        moveY = 0x80000000;
        break;
    case 1:
        moveX = 0x03FFFFFF;
        moveY = 0;
        break;
    case 2:
        moveX = 0;
        moveY = 0x03FFFFFF;
        break;
    case 3:
        moveX = 0x80000000;
        moveY = 0;
        break;
    }
    ((struct Flags8 *)&body->unk20D)->bit3 = 1;
    obj_start_move_zero_anchor(body, 1, moveX, moveY);
    obj_drift_with_cap_step(body);
    *state_out = (u32)enemy_after_contact;
}

ASM_FUNC("asm/code_081442FC/enemy_push_by_mode.s", void enemy_push_by_mode(void))
