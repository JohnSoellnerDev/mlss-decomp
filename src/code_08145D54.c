#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;           // bit 6: flipped horizontally
    u8 unk12;           // bits 1-2: play mode, bit 3: the animation has ended
    u8 filler13[0x20 - 0x13];
    u8 unk20;
    u8 unk21;           // current animation (set by obj_load_record)
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

/* Mode3 as a one-byte value (copied whole, edited in a register) */
struct Mode3Byte
{
    u8 mode : 3;
    u8 rest : 5;
} __attribute__((packed));

struct Partner
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    u8 filler14[0x242 - 0x14];
    s16 unk242;         // x speed per frame
    s16 unk244;         // y speed per frame
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
    /* the Ai view starts at enemy + 0x58: these are the enemy's unk1D0..unk1D6 */
    u16 unk178;
    u16 unk17A; /* = unk1D2: turn / step counter */
    u16 unk17C; /* = unk1D4: frame counter */
    u8 unk17E;  /* = unk1D6: bit 2 spotted (enemy_wait_scan), run direction (enemy_run_*) */
    u8 filler17F[0x184 - 0x17F];
    s32 unk184;
    s32 unk188;
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
    s32 unkC;           // x
    s32 unk10;          // y
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;           // bits 0-2: facing, 0-7 in 45 degree steps
    u8 unk25;
    u8 filler26[0x58 - 0x26];
    u8 filler58[0x128 - 0x58];
    u16 unk128[4];      // wait times after a move (Ai.unkD0), one picked at random
    u8 filler130[0x198 - 0x130];
    void *unk198;
    void *unk19C;
    void *unk1A0[2];
    u8 filler1A8[0x1B0 - 0x1A8];
    u8 *unk1B0;
    struct Sensor *unk1B4;
    u8 filler1B8[0x1D0 - 0x1B8];
    u16 unk1D0;         // random variant 0-3: indexes the record's per-variant bytes
    u16 unk1D2;         // turn / step counter
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 unk1DC;         // home x (enemy_begin_wait keeps unkC..unk18 here)
    s32 unk1E0;         // home y
    s32 unk1E4;         // home unk14
    s32 unk1E8;         // home unk18
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
    s16 unk242;         // x speed per frame (set by start_move_to)
    s16 unk244;         // y speed per frame
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
s32 rand_below(s32 n, u32 seed);
u32 angle_of_vector16(s32 x, s32 y);
u8 find_active_entry(struct Owner *owner, u8 *idx);
/* Which of the owner's two targets box_hit_kind tests the hit box against (see code_0813AC90.c). */
enum BoxHitKind
{
    BOX_HIT_NONE = 0,   /* never hits */
    BOX_HIT_A = 1,
    BOX_HIT_B = 2,
    BOX_HIT_EITHER = 3, /* one of the two */
    BOX_HIT_BOTH = 4
};
u8 box_hit_kind(struct Owner *owner, struct Enemy *ent, u32 boxFmt, u32 boxOffset, u32 kind);
// u8 stack argument the original passes unpromoted (strb/ldrb on the stack slot)
struct ByteArg
{
    u8 val;
} __attribute__((packed));

void start_move_to_3d(struct Enemy *ent, s32 x, s32 y, s32 z, u32 arg4, struct ByteArg arg5);
void start_move_to(struct Enemy *ent, s32 x, s32 y, u32 z, struct ByteArg arg4);
void obj_set_or_add_pos(struct Enemy *ent, s32 x, s32 y, s32 z, u32 arg5, u32 arg6);
void enemy_clear_body_flags(struct Enemy *ent);
void enemy_body_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);

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
void enemy_move_to_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_wait_scan(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_start_run(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_run_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_chase_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_run_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_face_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);
void enemy_align_body_to_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out);

/* Enemy state while it moves: once the move is done it waits one of its 4 wait times (unk128,
   picked at random; tick_store counts unk1D2 down), then goes on with enemy_push_by_mode. Every
   rec[0x1C] frames (counted in unk1D4) it checks the 0x11/0x1E box for either target: with one in
   it and in sight (companion_in_sight) it goes to enemy_pick_random_index to start the chase. */
void enemy_wait_sense(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 *ai = (u8 *)ent + 0x58;
    u8 *rec = ent->unk1B0;
    s32 unused[4];

    if (step_pos_xy_b(ent) == 0)
    {
        u32 pick = rand_below(4, mt19937_next());

        ent->unk1D2 = ((u16 *)(ai + 0xD0))[pick]; // ent->unk128[pick]
        state_out->state = tick_store;
        state_out->unkC = enemy_push_by_mode;
    }
    if (ent->unk1D4 == 0)
    {
        if (box_hit_kind(owner, ent, 0x11, 0x1E, BOX_HIT_EITHER) != 0)
        {
            if (companion_in_sight(owner, ent) == 0)
            {
                return;
            }
            state_out->state = enemy_pick_random_index;
        }
        ent->unk1D4 = *(u16 *)(rec + 0x1C); // sense interval
    }
    else
    {
        ent->unk1D4--;
    }
}

void enemy_pick_random_index(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 *rec = ent->unk1B0;

    ent->unk1D0 = rand_below(4, mt19937_next());
    ent->unk1D2 = rec[ent->unk1D0 + 0x24];
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ent->sprite->unk21 = 4;
    obj_refresh_sub_attr(ent);
    ent->sprite->unk20 = 0x10;
    state_out->state = enemy_chase_companion;
}

ASM_FUNC("asm/code_08145D54/enemy_chase_companion.s", void enemy_chase_companion(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

/* Enemy state: sets body mode 4, plays the idle animation for the way the enemy faces (4 down,
   7 side, 0xA up; the other side is the side one flipped) in play mode 1, then goes on with
   enemy_face_companion. */
void enemy_face_idle(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    s32 quadrant;

    ((struct Mode3 *)&ent->unk214)->mode = 4;
    quadrant = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (quadrant)
    {
    case 0:
        ent->sprite->unk21 = 4;
        break;
    case 3:
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 1;
        // fall through: the side animation, flipped
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

/* Enemy state: turns to face the active party member. Goes back to enemy_push_by_mode when that
   member is out of sight or gone; otherwise waits for the current animation to end. When the
   enemy is already in a facing animation (5, 8 or 0xB) it starts the chase, else it sets its
   facing to the direction of the member (8 directions) and plays the matching facing animation
   (down, side, up; the other side is the side one flipped). */
void enemy_face_companion(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 memberIndex;
    struct Partner *member;
    s32 dx;
    s32 dy;
    s32 facing;
    s32 quadrant;

    if (companion_in_sight(owner, ent) == 0)
    {
        state_out->state = enemy_push_by_mode;
        return;
    }
    if (find_active_entry(owner, &memberIndex) == 0)
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
    member = *(struct Partner **)((u8 *)owner + memberIndex * 4 + 0x28); // owner->party[memberIndex]
    dx = member->posX - ent->unkC;
    dy = member->posY - ent->unk10;
    if (dx | dy)
    {
        // Round the angle to the nearest of 8 directions.
        facing = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
    }
    else
    {
        facing = -1;
    }
    ((struct Mode3 *)&ent->unk24)->mode = facing;
    ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 0;
    quadrant = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (quadrant)
    {
    case 0:
        ent->sprite->unk21 = 5;
        break;
    case 3:
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 1;
        // fall through: the side animation, flipped
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

/* Enemy state after enemy_face_companion: leaps at the targeted party member (the active one,
   else the one bit 1 of the owner's unk208 picks), aiming where it will be in 6 frames, and up to
   the record's height for the leap index unk1D0 (the byte at 0x28 + index) above its own z, at
   the record's speed for that index (the u16 at 0x2C + index * 2). It faces the way it moves,
   remembers where it started (ai unk184/unk188) and goes on with enemy_approach_z. */
void enemy_start_chase(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)&ent->filler58; /* ai->unk178 is ent->unk1D0 */
    u8 *rec = ent->unk1B0;
    u8 activeIndex;
    u32 memberIndex;
    struct Partner *member;
    struct ByteArg zero = { 0 };
    u16 speed;
    u8 height;
    s32 targetZ;
    s32 offVX;
    s32 offVY;
    s32 targetX;
    s32 targetY;

    memberIndex = find_active_entry(owner, &activeIndex) ? activeIndex : ((u32)owner->unk208 << 30) >> 31;
    member = *(struct Partner **)((u8 *)owner + memberIndex * 4 + 0x28); /* owner->party[memberIndex] */
    speed = ((u16 *)(rec + 0x2C))[ai->unk178];
    /* the member's position 6 frames ahead (its speed is at 0x242/0x244) */
    offVX = 0x242;
    targetX = member->posX + *(s16 *)((u8 *)member + offVX) * 6;
    offVY = 0x244;
    targetY = member->posY + *(s16 *)((u8 *)member + offVY) * 6;
    start_move_to(ent, targetX, targetY, speed, zero);
    height = rec[ai->unk178 + 0x28];
    targetZ = ent->unk14 + ent->unk18 + (height << 8);
    start_z_move(ent, targetZ, speed, 0, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    /* heading 0-255, rounded; 0xFF when it doesn't move */
    if ((ent->unk242 | ent->unk244) != 0)
    {
        ent->unk2 = ((u32)(u16)angle_of_vector16(ent->unk242, ent->unk244) + 0x10) >> 8;
    }
    else
    {
        ent->unk2 = 0xff;
    }
    ai->unk184 = ent->unkC;
    ai->unk188 = ent->unk10;
    ((struct Flags8 *)&ent->unk20F)->bit2 = 1;
    state_out->state = enemy_approach_z;
}

/* Enemy state: plays the standing animation for the way the enemy faces (its 8-way facing in the low
   3 bits of unk24, halved to 4 quadrants: 0 -> anim 3, 1 -> 6, 2 -> 9, 3 -> 6 mirrored), sets the
   wait timer (unk1D2) to 20 frames and goes on with enemy_wait_count. */
void enemy_start_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai;
    s32 quadrant;

    ai = (struct Ai *)&ent->filler58;
    ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 0; /* not mirrored */
    quadrant = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (quadrant)
    {
    case 0:
        ent->sprite->unk21 = 3;
        break;
    case 3:
        ((struct Flags8 *)&ent->sprite->unk11)->bit6 = 1;
        // fall through: the side animation, mirrored
    case 1:
        ent->sprite->unk21 = 6;
        break;
    case 2:
        ent->sprite->unk21 = 9;
        break;
    }
    obj_load_record(ent->sprite, ent->sprite->unk21, -1, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ai->unk17A = 20; /* wait timer */
    state_out->state = enemy_wait_count;
}


/* Enemy state after enemy_start_wait: counts the wait timer unk1D2 down. When it runs out the
   enemy turns to its facing's cell, reloads the timer from its record (the byte at 0x40 + index
   unk1D0) and checks the 0x22 box in front of it: with something in it, it goes to tick_store;
   otherwise it moves back to (unk1DC, unk1E0) at the record's speed for that index (the u16 at
   0x38 + index * 2), with sprite byte 0x20 from the byte at 0x34 + index. */
void enemy_wait_count(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 *rec = ent->unk1B0;
    s16 box[8];
    u16 speed;
    struct ByteArg zero;

    ent->unk1D2--;
    if (ent->unk1D2 == 0)
    {
        ((struct Mode3 *)&ent->unk214)->mode = 4;
        obj_set_cell_coord(ent, MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2), 0);
        obj_box_to_stack(box, ent, 0x22, 0);
        ent->unk1D2 = rec[ent->unk1D0 + 0x40];
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            state_out->state = tick_store;
            state_out->unkC = enemy_push_by_mode;
        }
        else
        {
            speed = *(u16 *)(rec + ent->unk1D0 * 2 + 0x38);
            ent->sprite->unk20 = rec[ent->unk1D0 + 0x34];
            ((struct Mode3 *)&ent->unk214)->mode = 3;
            zero.val = 0;
            enemy_start_move_3d(ent, ent->unk1DC, ent->unk1E0, ent->unk14, speed, zero, state_out, tick_store);
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

NONMATCH("asm/code_08145D54/enemy_seek_partner.s", void enemy_seek_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    u8 *table;
    u8 idx;
    struct Partner *partner;
    s16 box[8];
    u8 hit;

    if (*(u16 *)((u8 *)ent->unk1B4 + 8) != 0)
    {
        u32 k = find_active_entry(owner, &idx) ? idx : ((u32)owner->unk208 << 30) >> 31;
        partner = *(struct Partner **)((u8 *)owner + k * 4 + 0x28);
        obj_box_to_stack(box, ent, 0x22, 0xA);
        hit = point_in_box(box, partner, 0x7FFF, 0);
    }
    else
    {
        hit = box_hit_kind(owner, ent, 0x11, 0x18, 3);
    }
    table = (u8 *)ent + 0x100;
    if (hit != 0)
    {
        state_out->state = enemy_start_guard;
    }
    else if (step_pos_xy_c(ent) == 0)
    {
        u32 pick = rand_below(4, mt19937_next());

        ai->unk17A = *(u16 *)(table + 0x28 + pick * 2);
        state_out->state = tick_store;
        state_out->unkC = enemy_wander_to_edge;
    }
}
END_NONMATCH

/* AI state: hop in place facing the hero. Each time a hop has landed (bit 4 of unk1D6, set
 * here once z_approach ends) and hops are left (unk1D4, also seen as ai->unk17C), turn towards
 * the active hero (or the one owner->unk208 picks) and start another hop to a random height at
 * a random speed from the record. With no hops left, go on with enemy_move_to_partner. */
void enemy_guard_aim(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    u8 *rec = ent->unk1B0;
    u8 facing;
    u32 (*div)(u32, u32);
    u8 *facingOut;
    struct Partner *hero;
    s32 dx;
    s32 dy;
    s32 dir8;

    if ((ent->unk1D6 & 0x10) != 0)
    {
        if (ent->unk1D4 != 0)
        {
            u32 heroIndex = find_active_entry(owner, &facing) ? facing : ((u32)owner->unk208 << 30) >> 31;
            hero = *(struct Partner **)((u8 *)owner + heroIndex * 4 + 0x28);
            facingOut = &facing;
            div = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
            /* unkC, unk10: ent's x and y */
            dx = hero->posX - ent->unkC;
            dy = hero->posY - ent->unk10;
            if (dx | dy)
            {
                /* one of 8 directions, each centred on its angle */
                dir8 = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
            }
            else
            {
                dir8 = -1;
            }
            *facingOut = div(dir8, 2);
            if (MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2) != facing)
            {
                obj_set_cell_coord(ent, facing, 0);
            }
            {
                u8 height = rec[rand_below(4, mt19937_next()) + 0x22];
                u16 speed = ((u16 *)rec)[rand_below(4, mt19937_next()) + 0x13];
                start_z_move(ent, height << 8, speed, 0, 0);
            }
            ai->unk17C--;
            ((struct Flags8 *)&ai->unk17E)->bit4 = 0;
        }
        else
        {
            ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
            state_out->state = enemy_move_to_partner;
        }
    }
    else
    {
        if (z_approach(ent) == 0)
        {
            ((struct Flags8 *)&ent->unk1D6)->bit4 = 1;
        }
    }
}

/* Starts the enemy's waiting behaviour: remembers where it stands now as its home position,
 * picks a random variant 0-3 (its sprite takes the record's byte 6 + variant), clears the turn
 * timer and runs enemy_wait_scan through run_state_handler. */
void enemy_begin_wait(struct Owner *owner, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk33E)->bit4 = 1;
    ent->unk1DC = ent->unkC;
    ent->unk1E0 = ent->unk10;
    ent->unk1E4 = ent->unk14;
    ent->unk1E8 = ent->unk18;
    ((struct Flags8 *)&ent->unk33D)->bit3 = 1;
    ent->unk1D0 = rand_below(4, mt19937_next());
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ent->sprite->unk20 = ent->unk1B0[ent->unk1D0 + 6];
    ent->unk1D2 = 0;
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_wait_scan;
}

/* AI state: stand and look around. Every 30 frames turn to a random one of the 8 directions (low
 * bits of unk24; the sprite shows dir / 2). When the hero is inside the sight box (box kind 0x11),
 * face the active hero (or the one owner->unk208 picks) and set the "spotted" bit 2 of unk1D6
 * (ai->unk17E is the same byte); the next frame sees it and starts the run, then the chase. */
void enemy_wait_scan(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    u8 heroSlot;
    u8 facing;
    u32 randomFacing;
    u32 (*div)(u32, u32);
    struct Partner *hero;
    s32 dx;
    s32 dy;
    s32 dir8;
    u16 *turnTimer;

    if ((ent->unk1D6 & 4) != 0)
    {
        ((struct Flags8 *)&ent->unk1D6)->bit2 = 0;
        state_out->state = enemy_start_run;
        state_out->unk8 = enemy_chase_partner;
        return;
    }
    turnTimer = &ent->unk1D2;
    if (*turnTimer != 0)
    {
        *turnTimer = *turnTimer - 1;
    }
    else
    {
        ((struct Mode3Byte *)&ent->unk24)->mode = rand_below(8, mt19937_next());
        randomFacing = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((struct Mode3Byte *)&ent->unk24)->mode, 2);
        obj_set_cell_coord(ent, randomFacing, 0);
        *turnTimer = 30;
    }
    if (box_hit_kind(owner, ent, 0x11, 0, 3) != 0)
    {
        u32 heroIndex = find_active_entry(owner, &heroSlot) ? heroSlot : ((u32)owner->unk208 << 30) >> 31;
        hero = *(struct Partner **)((u8 *)owner + heroIndex * 4 + 0x28);
        div = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
        /* unkC, unk10: ent's x and y */
        dx = hero->posX - ent->unkC;
        dy = hero->posY - ent->unk10;
        if (dx | dy)
        {
            /* one of 8 directions, each centred on its angle */
            dir8 = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
        }
        else
        {
            dir8 = -1;
        }
        facing = div(dir8, 2);
        if (MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2) != facing)
        {
            obj_set_cell_coord(ent, facing, 0);
        }
        ((struct Flags8 *)&ai->unk17E)->bit2 = 1;
    }
}

void enemy_start_run(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    u8 *rec = ent->unk1B0;
    s32 flags;
    u8 low;

    ent->unk2DC = 0x200;
    ent->unk1D0 = rand_below(4, mt19937_next());
    ent->unk2DE = rec[ent->unk1D0 + 0xE];
    low = ent->unk20C;
    flags = -0x21;
    flags &= low;
    flags &= 0x7F;
    flags &= -0x41;
    ent->unk20C = flags;
    ((struct Flags8 *)&ent->unk210)->bit4 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
    state_out->state = enemy_run_step;
}

/*
 * State set by enemy_start_run: shakes the enemy on the spot like enemy_run_wait. Every second
 * frame it steps unk2DC (x speed) right while bit 2 of unk1D6 is set (sprite link 0) or left
 * otherwise (link 1), and flips that bit after every two steps. When the unk2DE frames picked by
 * enemy_start_run are over, it faces right again (link 0) and goes back to state_out->unk8.
 */
void enemy_run_step(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    s32 step;
    s32 frames;
    s32 left;

    frames = ent->unk1D4 + 1;
    ent->unk1D4 = frames;
    if ((u16)frames > 1)
    {
        ent->unk1D2++;
        if (ent->unk1D6 & 4)
        {
            obj_set_sprite_link(ent, 0);
            ent->unk242 = ent->unk2DC;
            step = ent->unk242;
        }
        else
        {
            obj_set_sprite_link(ent, 1);
            ent->unk242 = -ent->unk2DC;
            step = ent->unk242;
        }
        ent->unkC += step;
        /* after two steps one way, turn round */
        if (ai->unk17A > 1)
        {
            u8 *dirFlags = &ai->unk17E;
            u8 oldFlags = *dirFlags;
            s32 newFlags;

            if (oldFlags & 4)
            {
                newFlags = oldFlags & ~4;
            }
            else
            {
                newFlags = oldFlags | 4;
            }
            *dirFlags = newFlags;
            ai->unk17A = 0;
        }
        ai->unk17C = 0;
    }
    left = ent->unk2DE - 1;
    ent->unk2DE = left;
    if ((u16)left == 0)
    {
        obj_set_sprite_link(ent, 0);
        state_out->state = state_out->unk8;
    }
}

ASM_FUNC("asm/code_08145D54/enemy_chase_partner.s", void enemy_chase_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))

/*
 * Shakes the enemy on the spot: every second frame it steps unk2DC (x speed) right while bit 2 of
 * unk1D6 is set (sprite link 0) or left otherwise (link 1), and flips that bit after every two steps.
 * When the unk2DE frames are over it faces left (link 1), sets again the flag bits enemy_start_run
 * cleared and goes on to enemy_face_partner.
 */
void enemy_run_wait(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    s32 step;
    s32 frames;
    s32 left;

    frames = ent->unk1D4 + 1;
    ent->unk1D4 = frames;
    if ((u16)frames > 1)
    {
        ent->unk1D2++;
        if (ent->unk1D6 & 4)
        {
            obj_set_sprite_link(ent, 0);
            ent->unk242 = ent->unk2DC;
            step = ent->unk242;
        }
        else
        {
            obj_set_sprite_link(ent, 1);
            ent->unk242 = -ent->unk2DC;
            step = ent->unk242;
        }
        ent->unkC += step;
        /* after two steps one way, turn round */
        if (ai->unk17A > 1)
        {
            u8 *dirFlags = &ai->unk17E;
            u8 oldFlags = *dirFlags;
            s32 newFlags;

            if (oldFlags & 4)
            {
                newFlags = oldFlags & ~4;
            }
            else
            {
                newFlags = oldFlags | 4;
            }
            *dirFlags = newFlags;
            ai->unk17A = 0;
        }
        ai->unk17C = 0;
    }
    left = ent->unk2DE - 1;
    ent->unk2DE = left;
    if ((u16)left == 0)
    {
        obj_set_sprite_link(ent, 1);
        ((struct Flags8 *)&ent->unk20C)->bit5 = 1;
        ((struct Flags8 *)&ent->unk20C)->bit7 = 1;
        ((struct Flags8 *)&ent->unk20C)->bit6 = 1;
        ((struct Flags8 *)&ent->unk210)->bit4 = 1;
        ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
        state_out->state = enemy_face_partner;
    }
}

/*
 * After the shake (enemy_run_wait): turn to face the active hero (or the one owner->unk208 picks),
 * showing the facing's second cell set (facing + 4), take the sprite value and the wait time for the
 * current variant (ai->unk178) from the record, and wait that long before enemy_align_body_to_partner.
 */
void enemy_face_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out)
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    u8 *rec = ent->unk1B0;
    u8 heroSlot;
    u32 heroIndex;
    struct Partner *hero;
    s32 dx;
    s32 dy;
    s32 dir8;
    u32 (*div)(u32, u32);

    if (find_active_entry(owner, &heroSlot) != 0)
    {
        heroIndex = heroSlot;
    }
    else
    {
        heroIndex = ((u32)owner->unk208 << 30) >> 31;
    }
    hero = *(struct Partner **)((u8 *)owner + heroIndex * 4 + 0x28); /* owner->party[heroIndex] */
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ent->sprite->unk20 = rec[ai->unk178 + 0x12];
    div = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
    /* unkC, unk10: ent's x and y */
    dx = hero->posX - ent->unkC;
    dy = hero->posY - ent->unk10;
    if (dx | dy)
    {
        /* one of 8 directions, each centred on its angle */
        u16 angle = angle_of_vector16(dx, dy);
        dir8 = (((s32)(angle + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir8 = -1;
    }
    {
        u32 facing = div(dir8, 2);
        obj_set_cell_coord(ent, facing + 4, 0);
    }
    /* frames to wait: tick_store counts ai->unk17A down, then switches to state_out->unkC */
    ai->unk17A = rec[ai->unk178 + 0x16];
    state_out->state = tick_store;
    state_out->unkC = enemy_align_body_to_partner;
}

NONMATCH("asm/code_08145D54/enemy_align_body_to_partner.s", void enemy_align_body_to_partner(struct Owner *owner, struct Enemy *ent, struct StateOut *state_out))
{
    struct Ai *ai = (struct Ai *)((u8 *)ent + 0x58);
    struct Enemy *body = ent->body;
    u8 *rec = ent->unk1B0;
    u8 found_idx;
    struct Partner *target;
    s32 cell;
    u32 k;
    u32 flip;
    u8 is_rec_b;
    s32 ax;
    s32 ay;

    is_rec_b = 0;
    if (find_active_entry(owner, &found_idx) != 0)
    {
        k = found_idx;
    }
    else
    {
        k = ((u32)owner->unk208 << 30) >> 31;
    }
    target = owner->party[k];
    cell = (u8)MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);

    enemy_clear_body_flags(body);
    obj_set_active(body, 1);
    obj_set_sprite_link(body, 1);

    switch (cell)
    {
    case 0:
        if (!(target->posY - ent->unk10 < -0x1000))
        {
            obj_set_active(body, 0);
            obj_set_sprite_link(body, 0);
            state_out->state = enemy_start_run;
            return;
        }
        obj_set_or_add_pos(body, ent->unkC, ent->unk10, ent->unk14 + ent->unk18 + 0x1400, 1, cell);
        break;
    case 1:
        if (!(target->posX - ent->unkC > 0x1000))
        {
            obj_set_active(body, 0);
            obj_set_sprite_link(body, 0);
            state_out->state = enemy_start_run;
            return;
        }
        obj_set_or_add_pos(body, ent->unkC + 0xA00, ent->unk10, ent->unk14 + ent->unk18 + 0x1400, cell, 0);
        break;
    case 2:
        if (!(target->posY - ent->unk10 > 0x1000))
        {
            obj_set_active(body, 0);
            obj_set_sprite_link(body, 0);
            state_out->state = enemy_start_run;
            return;
        }
        obj_set_or_add_pos(body, ent->unkC, ent->unk10, ent->unk14 + ent->unk18 + 0x1400, 1, 0);
        break;
    case 3:
        if (!(target->posX - ent->unkC < -0x1000))
        {
            obj_set_active(body, 0);
            obj_set_sprite_link(body, 0);
            state_out->state = enemy_start_run;
            return;
        }
        obj_set_or_add_pos(body, ent->unkC - 0xA00, ent->unk10, ent->unk14 + ent->unk18 + 0x1400, 1, 0);
        break;
    default:
        break;
    }

    obj_set_cell_coord(ent, cell + 8, 0);
    ((struct Mode3 *)&body->unk214)->mode = 1;
    {
        u16 spd = *(u16 *)(rec + ai->unk178 * 2 + 0x1A);
        struct ByteArg zero;
        zero.val = 0;
        start_move_to(body, target->posX, target->posY, spd, zero);
    }

    if (cell == 0)
    {
        ax = body->unk242;
        if (ax < 0)
        {
            ax = -ax;
        }
        ay = body->unk244;
        if (ay < 0)
        {
            ay = -ay;
        }
        if (ax < ay)
        {
            obj_load_record(body->sprite, 9, -1, 0, cell);
            *(u16 *)((u8 *)body + 0x278) = 0x10;
            *(u16 *)((u8 *)body + 0x272) = 0x10;
            *(u16 *)((u8 *)body + 0x27A) = 6;
        }
        else
        {
            obj_load_record(body->sprite, 0xB, -1, 0, cell);
            flip = (u32)body->unk242 >> 31;
            is_rec_b = 1;
        }
    }
    else if (cell == 1)
    {
        ax = body->unk242;
        if (ax < 0)
        {
            ax = -ax;
        }
        ay = body->unk244;
        if (ay < 0)
        {
            ay = -ay;
        }
        if (ax <= ay)
        {
            obj_load_record(body->sprite, 0xB, -1, 0, 0);
            flip = (u32)(~body->unk244) >> 31;
            is_rec_b = 1;
        }
        else
        {
            obj_load_record(body->sprite, 0xA, -1, 0, 0);
            *(u16 *)((u8 *)body + 0x278) = 6;
            *(u16 *)((u8 *)body + 0x272) = 6;
            *(u16 *)((u8 *)body + 0x27A) = 6;
        }
    }
    else if (cell == 2)
    {
        ax = body->unk242;
        if (ax < 0)
        {
            ax = -ax;
        }
        ay = body->unk244;
        if (ay < 0)
        {
            ay = -ay;
        }
        if (ax < ay)
        {
            obj_load_record(body->sprite, 9, -1, 0, 0);
            *(u16 *)((u8 *)body + 0x278) = 0x10;
            *(u16 *)((u8 *)body + 0x272) = 0x10;
            *(u16 *)((u8 *)body + 0x27A) = 6;
        }
        else
        {
            obj_load_record(body->sprite, 0xB, -1, 0, 0);
            flip = (u32)(~body->unk242) >> 31;
            is_rec_b = 1;
        }
    }
    else
    {
        ax = body->unk242;
        if (ax < 0)
        {
            ax = -ax;
        }
        ay = body->unk244;
        if (ay < 0)
        {
            ay = -ay;
        }
        if (ax > ay)
        {
            obj_load_record(body->sprite, 0xA, -1, 0, 0);
            *(u16 *)((u8 *)body + 0x278) = 6;
            *(u16 *)((u8 *)body + 0x272) = 6;
            *(u16 *)((u8 *)body + 0x27A) = 6;
        }
        else
        {
            obj_load_record(body->sprite, 0xB, -1, 0, 0);
            flip = (u32)body->unk244 >> 31;
            is_rec_b = 1;
        }
    }

    if (is_rec_b)
    {
        ((struct Flags8 *)&body->sprite->unk11)->bit6 = flip;
    }
    else
    {
        *(u16 *)((u8 *)body + 0x274) = 6;
        *(u16 *)((u8 *)body + 0x27C) = 6;
        *(u16 *)((u8 *)body + 0x276) = 6;
    }

    sprite_step_e6a0(body->sprite);
    ai->unk17A = 0xF;
    ((struct Flags8 *)&ai->unk17E)->bit4 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
    state_out->state = enemy_body_step;
}
END_NONMATCH
