#include "global.h"
#include "ram_routines.h"

struct Enemy;

struct Owner
{
    u8 filler0[0x2C];
    struct Enemy *anchors[4];
    u8 filler3C[0x208 - 0x3C];
    u8 unk208;
};

struct Sprite
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
};

struct Enemy
{
    u8 filler0[0x2];
    u8 heading;
    u8 filler3[0xC - 0x3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x34 - 0x25];
    s32 unk34;
    s32 unk38;
    u8 filler3C[0x54 - 0x3C];
    u8 unk54;
    u8 filler55[0xF8 - 0x55];
    u16 unkF8; /* 0x800: moving towards unk2CC/unk2D0 */
    u8 fillerFA[0x100 - 0xFA];
    u16 unk100[4]; /* move speeds to pick from (unk1D0 is the pick) */
    u16 unk108[4]; /* sprite palettes going with each speed */
    u16 unk110[4]; /* jump speeds going with each speed (obj_try_start_jump) */
    u16 unk118[4]; /* jump gravities going with each speed */
    u16 unk120[4]; /* hop heights going with each speed (obj_try_start_hop), pixels */
    u16 unk128[4];
    s16 unk130[2]; /* move lengths to pick from, pixels */
    u8 filler134[0x138 - 0x134];
    s16 unk138; /* unk138-unk13E: the area's top, bottom, left and right edge, pixels */
    s16 unk13A;
    s16 unk13C;
    s16 unk13E;
    u8 filler140[0x198 - 0x140];
    void *unk198;
    u8 filler19C[0x1AC - 0x19C];
    u32 unk1AC;
    u8 filler1B0[0x1B8 - 0x1B0];
    s16 unk1B8; /* unk1B8/unk1BA: point of a move back (obj_random_pose_short), pixels */
    s16 unk1BA;
    u16 unk1BC;
    u16 unk1BE;
    s32 unk1C0;
    u16 unk1C4; /* unk1C4-unk1C8: position (pixels, z with unk18) when the last move started */
    u16 unk1C6;
    u16 unk1C8;
    u8 filler1CA[0x1CC - 0x1CA];
    s32 unk1CC; /* length of the current move */
    u16 unk1D0;
    u16 unk1D2;
    u8 filler1D4[0x1D6 - 0x1D4];
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
    u8 unk20D;
    u8 filler20E[0x216 - 0x20E];
    u8 unk216;
    u8 filler217[0x239 - 0x217];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E; /* move speed */
    u8 filler240[0x242 - 0x240];
    u16 unk242; /* unk242/unk244: velocity x/y */
    u16 unk244;
    u8 filler246[0x248 - 0x246];
    s32 unk248; /* deceleration per frame of an approach move (obj_start_hop); a jump's gravity */
    s32 unk24C; /* unk24C/unk250: height (posZ + unk18) and unk18 when the hop/jump started */
    s32 unk250;
    s32 unk254; /* speed a hop/jump started with */
    s32 unk258; /* a jump's vertical speed, less unk248 every frame (obj_jump_arc_step) */
    s32 unk25C; /* frames into the hop/jump, -1 when none is under way */
    u8 filler260[0x294 - 0x260];
    struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 unk2BC; /* unk2BC/unk2C0: move start x/y */
    s32 unk2C0;
    u8 filler2C4[0x2CC - 0x2C4];
    s32 unk2CC; /* unk2CC/unk2D0: move target x/y */
    s32 unk2D0;
    u8 filler2D4[0x2D8 - 0x2D4];
    s16 unk2D8; /* unk2D8/unk2DA: move step x/y */
    s16 unk2DA;
    u8 filler2DC[0x2DE - 0x2DC];
    u16 unk2DE; /* frames moved so far */
    u16 unk2E0; /* move duration in frames */
    u8 filler2E2[0x33C - 0x2E2];
    u8 unk33C;
};

struct Bits2B6
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Bits2B5
{
    u8 low3 : 3;
    u8 bit3 : 1; /* a hop/jump is under way */
    u8 high4 : 4;
};

struct Bits1D6
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct Bits20D
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

/* The 8-way facing in the low 3 bits of unk24, clockwise from up (screen y grows downwards). */
enum Dir8
{
    DIR8_UP,
    DIR8_UP_RIGHT,
    DIR8_RIGHT,
    DIR8_DOWN_RIGHT,
    DIR8_DOWN,
    DIR8_DOWN_LEFT,
    DIR8_LEFT,
    DIR8_UP_LEFT
};

void obj_pick_random_entry(struct Enemy *ent);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void tick_store(void);
void move_step_handler(void);
void start_linked_state(void);
void obj_apply_state_a(void *obj);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void obj_set_stage(struct Enemy *ent, u32 stage);
u16 angle_of_vector16(s32 x, s32 y);
void obj_set_or_add_pos(struct Enemy *ent, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_start_move_quarter(struct Enemy *obj, u8 mode, s32 tx, s32 ty);
void obj_move_interp(struct Enemy *ent);
void obj_step_toward_target_08138FDC(struct Enemy *obj);

void obj_random_target_b(struct Owner *owner, struct Enemy *obj, u32 *out)
{
    s32 flags;

    obj_pick_random_entry(obj);
    obj->unk1D2 = obj->unk128[rand_below(4, mt19937_next())];
    flags = (obj->unk1D6 | 2) & -2;
    obj->unk1D6 = flags;
    if (obj->unk1D2 != 0)
    {
        out[0] = (u32)tick_store;
        obj->unk1AC = out[1];
    }
    else
    {
        s32 bits = -3;
        bits &= flags;
        obj->unk1D6 = bits | 1;
        obj->unkF8 &= 0xFBFF;
    }
}

u8 advance_move_step(struct Owner *owner, struct Enemy *obj);
u8 find_active_entry(struct Owner *owner, u8 *idx);
extern const u8 gFrameAttrTable[];
/* Move handler: steps the object toward its move target and stops it where it lines up with the
   hero it follows (the active one, else the leader) on the axis it walks along (gFrameAttrTable by
   facing: 0/2 = x, 1 = y). The stop is the hero's coordinate when it lies within one step of
   unk34/unk38, else one step short of that. When the move is over (advance_move_step != 1), sets
   unk1D6 bit 0, clears bit 1 and continues with the next handler (out[1]). */
void obj_move_to_target(struct Owner *owner, struct Enemy *obj, u32 *out)
{
    u8 heroSlot;
    struct Enemy *hero;
    s32 pos;
    s32 heroPos;
    s32 step;

    if (advance_move_step(owner, obj) == 1)
    {
        obj_step_toward_target_08138FDC(obj);
        if (find_active_entry(owner, &heroSlot) == 0)
            heroSlot = ((u32)owner->unk208 << 29) >> 31;
        hero = *(struct Enemy **)((u8 *)owner + heroSlot * 4 + 0x28);
        switch (gFrameAttrTable[(((u32)obj->unk216 << 28) >> 30 << 4) + (((u32)obj->unk24 << 29) >> 29) + 8])
        {
        case 0:
        case 2:
            /* walking along x: wait until it has reached or passed the hero */
            if (obj->unk2D8 >= 0)
            {
                pos = obj->posX;
                heroPos = hero->posX;
                if (pos < heroPos)
                    return;
                step = obj->unk2D8;
            }
            else
            {
                pos = obj->posX;
                heroPos = hero->posX;
                if (pos >= heroPos)
                    return;
                step = -obj->unk2D8;
            }
            if (obj->unk34 - step < heroPos && obj->unk34 + step > heroPos)
                obj->unk2CC = heroPos;
            else
                obj->unk2CC = obj->unk34 - obj->unk2D8;
            obj->posX = obj->unk2CC;
            obj->unk2D0 = obj->posY;
            break;
        case 1:
        default:
            /* walking along y */
            if (obj->unk2DA >= 0)
            {
                pos = obj->posY;
                heroPos = hero->posY;
                if (pos < heroPos)
                    return;
                step = obj->unk2DA;
            }
            else
            {
                pos = obj->posY;
                heroPos = hero->posY;
                if (pos >= heroPos)
                    return;
                step = -obj->unk2DA;
            }
            if (obj->unk38 - step < heroPos && obj->unk38 + step > heroPos)
                obj->unk2D0 = heroPos;
            else
                obj->unk2D0 = obj->unk38 - obj->unk2DA;
            obj->unk2CC = obj->posX;
            obj->posY = obj->unk2D0;
            break;
        }
    }
    else
    {
        ((struct Bits1D6 *)&obj->unk1D6)->bit0 = 1;
        ((struct Bits1D6 *)&obj->unk1D6)->bit1 = 0;
        out[0] = out[1];
    }
}

/* Move handler: heads straight back to where the last move started (unk1C4/unk1C6, kept in
 * unk1B8/unk1BA) at a random one of the four speeds in unk100, with its palette from unk108 on the
 * sprite and in unk239. Plays the move's first frame, marks it moving (unk1D6 bit 1) and continues
 * with move_step_handler. */
void obj_random_pose_short(struct Owner *owner, struct Enemy *obj, u32 *out)
{
    s16 *backX;
    u16 *speed;
    s16 *backY;
    struct Sprite *sprite;
    u32 pick;
    u16 palette;
    u16 startPos;

    speed = obj->unk100;
    startPos = obj->unk1C4;
    backX = &obj->unk1B8;
    *backX = startPos;
    startPos = obj->unk1C6;
    backY = &obj->unk1BA;
    *backY = startPos;
    pick = rand_below(4, mt19937_next());
    sprite = obj->sprite;
    palette = obj->unk108[(u16)pick];
    obj->unk239 = palette;
    sprite->unk20 = palette;
    speed += (u16)pick;
    obj->unk23E = *speed;
    obj_start_move_quarter(obj, 0, *backX << 8, *backY << 8);
    obj_move_interp(obj);
    ((struct Bits1D6 *)&obj->unk1D6)->bit0 = 0;
    ((struct Bits1D6 *)&obj->unk1D6)->bit1 = 1;
    out[0] = (u32)move_step_handler;
}


/* Shows and activates `anchor` and puts it on top of `obj`, unless it is hidden (unk20C bit 0) or
   busy (unk33C bits 1-2). Returns whether it did. */
static inline u32 obj_try_link_anchor(struct Enemy *anchor, struct Enemy *obj)
{
    u32 lock;

    if ((anchor->unk20C & 1) != 0)
    {
        return 0;
    }
    lock = anchor->unk33C & 6;
    if (lock != 0)
    {
        return 0;
    }
    obj_set_sprite_link(anchor, 1);
    obj_set_active(anchor, 1);
    obj_set_or_add_pos(anchor, obj->posX, obj->posY + 0x100, obj->posZ + obj->unk18 + 0x1000, 1, lock);
    return 1;
}

/*
 * State: once the sprite signals its event (flags12 bit 3), apply state A, continue with
 * start_linked_state and bring out the first free of the two objects in the owner's slots after
 * this one's (owner->anchors[unk54], then [unk54 + 1]): it is shown, activated and placed on top
 * of this object (1 px lower on the screen, 16 px above its height). Then waits 120 frames
 * (unk1D2).
 */
void obj_link_sprites(struct Owner *owner, struct Enemy *obj)
{
    u8 *fields58; /* obj + 0x58, kept from the start: the timer store goes through it */

    fields58 = (u8 *)obj + 0x58;
    if ((obj->sprite->flags12 & 8) != 0)
    {
        obj_apply_state_a(obj);
        obj->unk198 = start_linked_state;
        if (!obj_try_link_anchor(*(struct Enemy **)((u8 *)owner + (obj->unk54 * 4) + 0x2C), obj))
        {
            obj_try_link_anchor(*(struct Enemy **)((u8 *)owner + (obj->unk54 * 4) + 0x30), obj);
        }
        *(u16 *)(fields58 + 0x17A) = 120; /* obj->unk1D2 */
    }
}


ASM_FUNC("asm/code_08137B08/obj_random_offset.s", void obj_random_offset(void))


ASM_FUNC("asm/code_08137B08/obj_random_offset_step.s", void obj_random_offset_step(void))


/*
 * Picks a random move: a length from unk130, a speed (and its palette) from unk100/unk108, and a new
 * facing at right angles to the current one (up or down when facing sideways, else left or right).
 * The move target is the current position moved to the area's edge in the new facing.
 */
void obj_random_facing_target(struct Enemy *obj)
{
    u8 turns[2];
    u16 *speeds;
    u16 pick;
    u32 rnd;
    u32 facing;
    struct Sprite *sprite;
    u16 palette;

    speeds = obj->unk100;
    obj->unk1CC = (s32)obj->unk130[rand_below(2, mt19937_next())] << 8;
    rnd = rand_below(4, mt19937_next());
    sprite = obj->sprite;
    pick = rnd;
    palette = obj->unk108[pick];
    sprite->unk20 = palette;
    obj->unk239 = palette;
    obj->unk23E = *(u16 *)((u8 *)speeds + (pick << 1));
    obj->unk1D0 = pick;

    facing = ((u32)obj->unk24 << 29) >> 29;
    if ((facing & 2) != 0)
    {
        turns[0] = DIR8_UP;
        turns[1] = DIR8_DOWN;
    }
    else
    {
        turns[0] = DIR8_RIGHT;
        turns[1] = DIR8_LEFT;
    }
    obj_set_stage(obj, turns[rand_below(2, mt19937_next())]);
    obj->unk2CC = obj->posX;
    obj->unk2D0 = obj->posY;

    /* the edges unk138-unk13E, read through the speed table's pointer */
    switch (((u32)obj->unk24 << 29) >> 29)
    {
    case DIR8_UP:
        obj->unk2D0 = (s32)*(s16 *)((u8 *)speeds + 0x38) << 8;
        break;
    case DIR8_RIGHT:
        obj->unk2CC = (s32)*(s16 *)((u8 *)speeds + 0x3E) << 8;
        break;
    case DIR8_DOWN:
        obj->unk2D0 = (s32)*(s16 *)((u8 *)speeds + 0x3A) << 8;
        break;
    case DIR8_LEFT:
        obj->unk2CC = (s32)*(s16 *)((u8 *)speeds + 0x3C) << 8;
        break;
    }
}


ASM_FUNC("asm/code_08137B08/obj_pick_move_vector.s", void obj_pick_move_vector(void))

ASM_FUNC("asm/code_08137B08/obj_random_orbit_offset.s", void obj_random_orbit_offset(void))


/* Starts a straight move at unk23E speed towards (targetX, targetY), relative to the current position
 * when mode is 1: records the start and target, sets the per-axis velocity (+-speed or 0), the heading
 * (0xFF when not moving), the move's length, and the moving flag 0x800 in unkF8. */
void obj_start_move_to_target(struct Enemy *obj, u8 mode, s32 targetX, s32 targetY)
{
    u8 *base;
    u8 **routines;
    u16 *flagp;
    u16 flagValue;
    s32 vx;
    s32 vy;
    s32 dx;
    s32 dy;

    base = (u8 *)obj + 0x58;
    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;

    if (mode == 1)
    {
        targetX += obj->posX;
        targetY += obj->posY;
    }
    obj->unk2BC = obj->posX;
    obj->unk2C0 = obj->posY;
    obj->unk2CC = targetX;
    obj->unk2D0 = targetY;

    if (targetX > obj->posX)
    {
        vx = obj->unk23E;
    }
    else if (targetX < obj->posX)
    {
        vx = -obj->unk23E;
    }
    else
    {
        vx = 0;
    }
    obj->unk242 = vx;
    obj->unk2D8 = vx;

    if (targetY > obj->posY)
    {
        vy = obj->unk23E;
    }
    else if (targetY < obj->posY)
    {
        vy = -obj->unk23E;
    }
    else
    {
        vy = 0;
    }
    obj->unk244 = vy;
    obj->unk2DA = vy;
    obj->unk2DE = 0;

    {
        u16 stepX;
        u16 stepY;

        stepX = *(u16 *)&obj->unk2D8;
        stepY = *(u16 *)&obj->unk2DA;
        if ((stepX | stepY) != 0)
        {
            obj->heading = (u32)((u16)angle_of_vector16(obj->unk2D8, obj->unk2DA) + 0x10) >> 8;
        }
        else
        {
            obj->heading = 0xFF;
        }
    }

    /* unk1CC = iwram_sqrt(dx * dx + dy * dy); unkF8 |= 0x800 */
    routines = &gMathRoutines;
    *(s32 *)(base + 0x174) = ((s32 (*)(s32))*routines)((obj->unk2CC - obj->unk2BC) * (obj->unk2CC - obj->unk2BC) + (obj->unk2D0 - obj->unk2C0) * (obj->unk2D0 - obj->unk2C0));
    flagp = (u16 *)(base + 0xA0);
    flagValue = *flagp | 0x800;
    *flagp = flagValue;
}


// Starts a straight move of obj to (targetX, targetY), an offset from its position when mode
// is 1, that obj_move_interp then plays out: stores the start and target, the distance in
// quarters (kept small for the interpolation's multiply), the move's length, its duration at
// obj's speed, turns obj along the move (0xFF when it doesn't move) and sets the moving flag.
void obj_start_move_quarter(struct Enemy *obj, u8 mode, s32 targetX, s32 targetY)
{
    u8 *base58;
    u8 **routines;
    u16 *moveFlags;
    u16 flagValue;
    s16 *velX;
    s16 *velY;
    s32 len;
    s32 quarterX;
    s32 quarterY;
    u16 rawVelX;
    u16 rawVelY;

    base58 = (u8 *)obj + 0x58;
    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;
    if (mode == 1)
    {
        targetX += obj->posX;
        targetY += obj->posY;
    }
    obj->unk2BC = obj->posX;
    obj->unk2C0 = obj->posY;
    obj->unk2CC = targetX;
    obj->unk2D0 = targetY;

    quarterX = (targetX - obj->posX) / 4;
    velX = (s16 *)&obj->unk242;
    *velX = quarterX;
    obj->unk2D8 = quarterX;
    quarterY = (targetY - obj->posY) / 4;
    velY = (s16 *)&obj->unk244;
    *velY = quarterY;
    obj->unk2DA = quarterY;

    // length = sqrt(x * x + y * y); duration = length / speed, both scaled back up by 4
    routines = &gMathRoutines;
    len = ((s32 (*)(s32))*routines)(*velX * *velX + *velY * *velY);
    obj->unk2E0 = ((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(len, (s16)obj->unk23E) << 2;
    obj->unk1CC = len << 2;
    obj->unk2DE = 0;
    rawVelX = *(u16 *)velX;
    rawVelY = *(u16 *)velY;
    if ((rawVelX | rawVelY) != 0)
    {
        obj->heading = (u32)((u16)angle_of_vector16(*velX, *velY) + 0x10) >> 8;
    }
    else
    {
        obj->heading = 0xFF;
    }
    moveFlags = (u16 *)(base58 + 0xA0); // &obj->unkF8
    flagValue = *moveFlags | 0x800;
    *moveFlags = flagValue;
}

/* Starts an approach move (stepped by obj_approach_step) from the current position towards (targetX,
   targetY), relative to the position when mode is 1. The speed starts at unk23E and drops by a
   deceleration (unk248, at least 1) chosen so the object comes to rest at the target: about
   speed² / (2 * distance + speed), with the distance measured in quarter units. */
void obj_start_hop(struct Enemy *obj, u8 mode, s32 targetX, s32 targetY)
{
    u8 **routines;
    s32 dy;
    s32 dist;
    s32 speed;
    s32 denom;

    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;

    if (mode == 1)
    {
        targetX += obj->posX;
        targetY += obj->posY;
    }
    obj->unk2CC = obj->posX;
    obj->unk2D0 = obj->posY;
    obj->unk2BC = targetX;
    obj->unk2C0 = targetY;

    routines = &gMathRoutines;
    targetX -= obj->posX;
    targetX = targetX * (targetX / 4) / 4;
    dy = targetY - obj->posY;
    dist = ((s32 (*)(s32))*routines)(targetX + dy * (dy / 4) / 4);

    obj->unk2D8 = obj->unk23E;
    obj->unk242 = obj->unk23E;
    speed = (s16)obj->unk23E;
    denom = speed + dist * 8;
    obj->unk248 = -((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(-speed * speed + denom / 2, denom);
    if (obj->unk248 == 0)
    {
        obj->unk248 = 1;
    }
    obj->unk2DE = 0;
    obj->unkF8 |= 0x800;
}

/* Like obj_start_move_to_target, but the recorded move start (unk2BC/unk2C0) is left at 0 and the move's
 * length is measured from the current position: starts a straight move at unk23E speed towards
 * (targetX, targetY) (relative to the current position when mode is 1), sets the per-axis velocity
 * (+-speed or 0), the heading (0xFF when not moving), the length and the moving flag 0x800 in unkF8. */
void obj_start_move_zero_anchor(struct Enemy *obj, u8 mode, s32 targetX, s32 targetY)
{
    u8 *base;
    u8 **routines;
    u16 *flagp;
    u16 flagValue;
    s32 vx;
    s32 vy;
    s32 dx;
    s32 dy;

    base = (u8 *)obj + 0x58;
    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;

    if (mode == 1)
    {
        targetX += obj->posX;
        targetY += obj->posY;
    }
    obj->unk2BC = 0;
    obj->unk2C0 = 0;
    obj->unk2CC = targetX;
    obj->unk2D0 = targetY;

    if (targetX > obj->posX)
    {
        vx = obj->unk23E;
    }
    else if (targetX < obj->posX)
    {
        vx = -obj->unk23E;
    }
    else
    {
        obj->unk2BC = 0;
        vx = 0;
    }
    obj->unk242 = vx;
    obj->unk2D8 = vx;

    if (targetY > obj->posY)
    {
        vy = obj->unk23E;
    }
    else if (targetY < obj->posY)
    {
        vy = -obj->unk23E;
    }
    else
    {
        obj->unk2C0 = 0;
        vy = 0;
    }
    obj->unk244 = vy;
    obj->unk2DA = vy;
    obj->unk2DE = 0;

    {
        u16 stepX;
        u16 stepY;

        stepX = *(u16 *)&obj->unk2D8;
        stepY = *(u16 *)&obj->unk2DA;
        if ((stepX | stepY) != 0)
        {
            obj->heading = (u32)((u16)angle_of_vector16(obj->unk2D8, obj->unk2DA) + 0x10) >> 8;
        }
        else
        {
            obj->heading = 0xFF;
        }
    }

    /* unk1CC = iwram_sqrt(dx * dx + dy * dy) from the current position; unkF8 |= 0x800 */
    routines = &gMathRoutines;
    *(s32 *)(base + 0x174) = ((s32 (*)(s32))*routines)((obj->unk2CC - obj->posX) * (obj->unk2CC - obj->posX) + (obj->unk2D0 - obj->posY) * (obj->unk2D0 - obj->posY));
    flagp = (u16 *)(base + 0xA0);
    flagValue = *flagp | 0x800;
    *flagp = flagValue;
}

/* Starts a move one step (unk23E) in the direction the object faces: sets the move step and the
   velocity per axis, restarts the frame count and sets the moving flag 0x800 in unkF8. */
void obj_velocity_from_facing(struct Enemy *obj)
{
    switch (((u32)obj->unk24 << 29) >> 29)
    {
    case DIR8_UP:
        obj->unk2D8 = 0;
        obj->unk2DA = -obj->unk23E;
        break;
    case DIR8_UP_RIGHT:
        obj->unk2D8 = obj->unk23E;
        obj->unk2DA = -obj->unk23E;
        break;
    case DIR8_RIGHT:
        obj->unk2D8 = obj->unk23E;
        obj->unk2DA = 0;
        break;
    case DIR8_DOWN_RIGHT:
        obj->unk2D8 = obj->unk23E;
        obj->unk2DA = obj->unk23E;
        break;
    case DIR8_DOWN:
        obj->unk2D8 = 0;
        obj->unk2DA = obj->unk23E;
        break;
    case DIR8_DOWN_LEFT:
        obj->unk2D8 = -obj->unk23E;
        obj->unk2DA = obj->unk23E;
        break;
    case DIR8_LEFT:
        obj->unk2D8 = -obj->unk23E;
        obj->unk2DA = 0;
        break;
    case DIR8_UP_LEFT:
        obj->unk2D8 = -obj->unk23E;
        obj->unk2DA = -obj->unk23E;
        break;
    }
    obj->unk242 = obj->unk2D8;
    obj->unk244 = obj->unk2DA;
    obj->unk2DE = 0;
    obj->unkF8 |= 0x800;
}

/* When no vertical motion is running (unk25C == -1): with a hop height for the picked speed
 * (unk120[unk1D0]), starts a hop that high over the current move's length (unk1CC) from the current
 * height, at the move speed, and sets the motion bit (unk2B5 bit 3); without one, sets unk2B6 bits
 * 6 and 7. */
void obj_try_start_hop(struct Enemy *obj)
{
    u16 hopHeight;

    hopHeight = obj->unk120[obj->unk1D0];
    if (obj->unk25C == -1)
    {
        if (hopHeight != 0)
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 0;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 0;
            obj->unk1BE = hopHeight;
            obj->unk1C0 = obj->unk1CC;
            obj->unk24C = obj->posZ + obj->unk18;
            obj->unk250 = obj->unk18;
            obj->unk254 = (s16)obj->unk23E;
            obj->unk25C = 0;
            ((struct Bits2B5 *)&obj->unk2B5)->bit3 = 1;
        }
        else
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 1;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 1;
        }
    }
}

/* When no hop/jump is under way (unk25C == -1): with a jump speed or gravity for the picked speed
 * (unk110[unk1D0] / unk118[unk1D0]), starts a jump from the current height with that vertical speed
 * and gravity (run by obj_jump_arc_step) and sets the jump bit (unk2B5 bit 3); with neither, sets
 * unk2B6 bits 6 and 7. */
void obj_try_start_jump(struct Enemy *obj)
{
    u16 speedRaw;
    u16 gravityRaw;
    s32 speed;
    s32 gravity;

    speedRaw = obj->unk110[obj->unk1D0];
    gravityRaw = obj->unk118[obj->unk1D0];
    if (obj->unk25C == -1)
    {
        speed = (s16)speedRaw;
        gravity = (s16)gravityRaw;
        if ((speed | gravity) != 0)
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 0;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 0;
            obj->unk24C = obj->posZ + obj->unk18;
            obj->unk250 = obj->unk18;
            obj->unk254 = obj->unk258 = speed;
            obj->unk248 = gravity;
            obj->unk25C = 0;
            ((struct Bits2B5 *)&obj->unk2B5)->bit3 = 1;
        }
        else
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 1;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 1;
        }
    }
}

/* Like obj_try_start_hop, for an arc spread over the current move: when no vertical motion is
 * running (unk25C == -1) and a move length (unk1CC) is set, starts one from height unk1BC whose
 * rise (unk1BE) is the current height in pixels above unk1BC, over twice the move length at half
 * the move speed, and sets the motion bit (unk2B5 bit 3); with no move, sets unk2B6 bits 6 and 7. */
void obj_flags_2b6_update(struct Enemy *obj)
{
    s32 height;
    s32 halfSpeed;

    if (obj->unk25C == -1)
    {
        if (obj->unk1CC != 0)
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 0;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 0;
            height = obj->posZ + obj->unk18;
            if (height < 0)
            {
                height += 0xFF;
            }
            height >>= 8;
            height -= obj->unk1BC;
            obj->unk1BE = height;
            obj->unk1C0 = obj->unk1CC * 2;
            obj->unk24C = (s16)obj->unk1BC << 8;
            obj->unk250 = 0;
            obj->unk254 = halfSpeed = (s16)obj->unk23E / 2;
            obj->unk25C = obj->unk1C0 / (halfSpeed * 8);
            ((struct Bits2B5 *)&obj->unk2B5)->bit3 = 1;
        }
        else
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 1;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 1;
        }
    }
}

/*
 * One frame of a straight move at a constant step, skipped while bit 5 of unk216 is set: moves the
 * object by the move step (unk2D8/unk2DA) and counts the frame (unk2DE). On each axis the object
 * snaps to the target (unk2CC/unk2D0) once it is within a pixel of it in the direction it moves, or
 * when it doesn't move on that axis.
 */
void obj_step_toward_target_08138FDC(struct Enemy *obj)
{
    s32 stepX;
    s32 stepY;

    if ((obj->unk216 & 0x20) == 0)
    {
        obj->posX += obj->unk2D8;
        obj->posY += obj->unk2DA;
        obj->unk2DE++;
        stepX = obj->unk2D8;
        if ((stepX > 0 && obj->posX + 0x100 > obj->unk2CC) || (stepX < 0 && obj->posX - 0x100 < obj->unk2CC) || stepX == 0)
        {
            obj->posX = obj->unk2CC;
        }
        stepY = obj->unk2DA;
        if ((stepY > 0 && obj->posY + 0x100 > obj->unk2D0) || (stepY < 0 && obj->posY - 0x100 < obj->unk2D0) || stepY == 0)
        {
            obj->posY = obj->unk2D0;
        }
    }
}

/* Plays one frame of the straight move obj_start_move_quarter set up (unless unk216 bit 5 holds
   it): counts the frame and puts obj at start + elapsed/duration of the distance (stored in
   quarters, so the multiply stays small), or right on the target once the duration is over. */
void obj_move_interp(struct Enemy *obj)
{
    u32 elapsed;

    if ((obj->unk216 & 0x20) == 0)
    {
        elapsed = obj->unk2DE + 1;
        obj->unk2DE = elapsed;
        if ((u16)elapsed >= obj->unk2E0)
        {
            /* arrived */
            obj->posX = obj->unk2CC;
            obj->posY = obj->unk2D0;
        }
        else
        {
            obj->posX = (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2DE * obj->unk2D8, obj->unk2E0) << 2) + obj->unk2BC;
            obj->posY = (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2DE * obj->unk2DA, obj->unk2E0) << 2) + obj->unk2C0;
        }
    }
}

NONMATCH("asm/code_08137B08/obj_approach_step.s", void obj_approach_step(struct Enemy *obj))
{
    s32 dx;
    s32 dy;
    s32 dx4;
    s32 dy4;
    s32 len;

    if ((obj->unk216 & 0x20) == 0)
    {
        obj->unk2DE++;
        dx = obj->unk2BC - obj->unk2CC;
        if (dx < 0)
        {
            dx += 3;
        }
        dx4 = dx >> 2;
        dy = obj->unk2C0 - obj->unk2D0;
        if (dy < 0)
        {
            dy += 3;
        }
        dy4 = dy >> 2;
        if ((dx4 | dy4) != 0)
        {
            len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx4 * dx4 + dy4 * dy4);
            obj->posX += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2D8 * dx4, len);
            obj->posY += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2D8 * dy4, len);
        }
        obj->unk2D8 -= obj->unk248;
        if ((s16)obj->unk2D8 > 0)
        {
            return;
        }
        ((struct Bits20D *)&obj->unk20D)->bit3 = 0;
        if (-(s16)obj->unk2D8 < (s16)obj->unk23E)
        {
            return;
        }
        obj->posX = obj->unk2CC;
        obj->posY = obj->unk2D0;
    }
}
END_NONMATCH

/*
 * One frame of an accelerating move (as started by obj_start_move_zero_anchor), skipped while
 * bit 5 of unk216 is set. Here unk2BC/unk2C0 hold the current speed: it moves the object, copies
 * it to the velocity (unk242/unk244), grows by the move step (unk2D8/unk2DA) up to 0x600 and counts
 * the frame. On each axis the object snaps to the target (unk2CC/unk2D0) once it is within a pixel
 * of it in the direction it moves, or when it doesn't move on that axis.
 */
void obj_drift_with_cap_step(struct Enemy *obj)
{
    s32 speedX;
    s32 speedY;

    if ((obj->unk216 & 0x20) == 0)
    {
        obj->posX += obj->unk2BC;
        obj->posY += obj->unk2C0;
        obj->unk242 = obj->unk2BC;
        obj->unk244 = obj->unk2C0;
        obj->unk2BC += obj->unk2D8;
        obj->unk2C0 += obj->unk2DA;
        if (obj->unk2BC > 0x600)
        {
            obj->unk2BC = 0x600;
        }
        if (obj->unk2C0 > 0x600)
        {
            obj->unk2C0 = 0x600;
        }
        obj->unk2DE++;
        speedX = obj->unk2BC;
        if ((speedX > 0 && obj->posX + 0x100 > obj->unk2CC) || (speedX < 0 && obj->posX - 0x100 < obj->unk2CC) || speedX == 0)
        {
            obj->posX = obj->unk2CC;
        }
        speedY = obj->unk2C0;
        if ((speedY > 0 && obj->posY + 0x100 > obj->unk2D0) || (speedY < 0 && obj->posY - 0x100 < obj->unk2D0) || speedY == 0)
        {
            obj->posY = obj->unk2D0;
        }
    }
}

