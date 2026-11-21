#include "global.h"
#include "ram_routines.h"

struct HalfBit0
{
    u16 bit0 : 1;
    u16 hi : 15;
};

struct HalfBit6
{
    u16 lo : 6;
    u16 bit6 : 1;
    u16 hi : 9;
};

struct PoseBits11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

struct Pose
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10;
    u8 flags11;
    u8 flags12;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F;
    u8 amount20;
    u8 waitFlag21;
          u8 filler22;
      u8 unk23;
      u8 filler24[0xBA - 0x24];
              u16 unkBA;
};

#define POSE_FLIPPED 0x40 // Pose.flags11: drawn mirrored horizontally
#define POSE_FREE    0x20 // Pose.flags12: the record has been released


struct Rec
{
    struct Rec *ringNext;
          u8 filler4[0x10 - 0x4];
      struct Pose *pose10;
      u8 filler14[0x38 - 0x14];
      struct Actor *field38;
          u8 filler3C[0x48 - 0x3C];
      s32 unk48;
      void (*state)(struct Actor *);
      u8 filler50[0x78 - 0x50];
      u16 arrive78;
    u8 filler7A[0x7C - 0x7A];
    u8 unk7C;
    u8 unk7D;
    u8 flags7E;
    u8 filler7F[0x81 - 0x7F];
    u8 flags81;
    u8 filler82[0xD8 - 0x82];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 slot;
          u8 fillerEE[0xF0 - 0xEE];
      struct SlotTable *table;
      u8 fillerF4[0xF6 - 0xF4];
      s16 timer;
      u8 filler0F8[0x110 - 0xF8];
      u8 status110;
      u8 status111;
};

/* one entry of a record's slot table; an actor attached to it runs its routine */
struct SlotEntry
{
    u8 filler0[0x4];
    void (*routine4)(struct Actor *); /* run by poll_link_routine once the state isn't busy */
    u8 flags8;          /* bits 5, 6: cleared ones set the actor's flags77 bits 6, 5 */
    u8 filler9[0xE - 0x9];
    s16 offE;
    s16 off10;
    u16 unk12;
};

struct SlotTable
{
    u8 filler0[0xC];
    struct SlotEntry *entries;
};

struct CandMgr
{
    u8 filler0[0x8];
    u8 status08;
    u8 filler09[0xC - 0x9];
    u8 flags0C;
    u8 filler0D[0x58 - 0x0D];
    struct Rec *slotTab[6];
    struct Rec *candA;
    struct Rec *candB;
          struct Rec *list78;
      struct Rec *cand7C;
    u8 filler80[0x84 - 0x80];
    s32 unk84;
    u8 filler88[0x2BF - 0x88];
    u8 unk2BF;
    u8 filler2C0[0x2F9 - 0x2C0];
    u8 unk2F9; /* 1: a contacted part waits for its pose instead of linking to the partner */
};

struct Actor
{
    u8 filler0[0x4];
    void (*routine4)(struct Actor *);
    struct Pose *pose;
          u8 filler0C[0x10 - 0x0C];
      s32 posX;
      s32 posY;
      s32 posZ;
      u8 filler1C[0x28 - 0x1C];
      struct Rec *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 unk44;
        u8 unk45;
        u8 unk46;
        u8 unk47;
        u8 unk48;
        u8 unk49;
        u8 filler4A[0x4C - 0x4A];
    void (*state)(struct Actor *);
    void (*savedState)(struct Actor *);
    s32 field54;
    s32 field58;
        s32 field5C;
        s32 field60;
        s32 field64;
    struct Actor *activeNext;
    struct Actor *link;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 flags77;
    u8 flags78;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s16 progress;
    u32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
                  u8 filler90[0x94 - 0x90];
        s32 unk94;
        s32 unk98;
        s32 unk9C;
        s32 unkA0;
        s32 unkA4;
      s32 waitCount;
      u16 timer;
      s16 offX;
      s16 offY;
      s16 offZ;
      u8 fillerB4[0xC8 - 0xB4];
      u16 unkC8;
      u16 unkCA;
      u8 unkCC;
      u8 fillerCD[0xF0 - 0xCD];
    void *dataPtr;
    u8 counter;
};

void on_arrive(struct Actor *obj);
void poll_link_routine(struct Actor *obj);

struct PoseBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct CandBits
{
    u8 low : 4;
    u8 high : 4;
};

struct PoseFlags11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FlagBits81
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
};

extern struct CandMgr *gWorld;
extern const u16 gActionCodes[];

void place_part_sprite(struct Actor *obj);
void effect_follow(struct Actor *obj);
s32 is_state_busy(void);
void release_object(void *obj);
void task_list_remove(struct Actor *obj);
void pair_begin_setup(void *p);
void pair_begin_setup_b(void *p);
void place_targets(struct Actor *obj, s32 x, s32 y, s32 z, s32 w);
void set_step_and_offsets(struct Actor *obj, s32 a, s32 b);
void start_action_5(struct Actor *obj);
void sprite_set_action(struct Actor *obj, s32 a, s32 b, s32 c);
void task_create_list_4c(void (*fn)(struct Actor *), void *obj);
void task_release_obj(struct Actor *obj);
void noop_state_080883dc(struct Actor *obj);
void obj_step_velocity(struct Actor *obj);
void set_targets_from_record(struct Actor *obj);
void noop_state_08087c7c(struct Actor *obj);
void obj_stop_step(struct Actor *obj);
void obj_list_remove(struct Actor *obj);
void set_cur_flags_both(s32 flags);
void start_motion(struct Actor *obj);
void link_index_state(struct Actor *obj);
void mark_candidate_pair(struct Actor *obj);
void stop_unless_flagged(struct Actor *obj);
void timed_motion_done(struct Actor *obj);
void clear_candidate_pair(struct Actor *obj);
void *find_lowest_order_pose(struct Actor *obj);
void finish_step_and_place(struct Actor *obj);
void obj_place_step(struct Actor *obj);
void set_slot_value(s32 a, s32 b);
s32 resolve_contact_action(struct Actor *a, struct Actor *b, s32 c, s32 d, s32 e);
void wait_pose_flag(struct Actor *obj);
void set_motion_start(struct Actor *obj, s32 a);
void enter_finish_rebuild(struct Actor *obj);
void obj_release(struct Actor *obj);
u32 arc_tan2(s32 x, s32 y);
extern const s16 gOffsetRows[];
struct Actor *obj_spawn(struct Rec *rec, s32 type, s32 x, s32 y, s32 h, s32 a, s32 b);
void obj_list_push_default(struct Actor *obj);
void run_sequence_of_obj(struct Actor *obj);
void actor_init_linked(struct Actor *obj);

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct StatusBits110
{
    u8 low5 : 5;
    u8 high3 : 3;
} __attribute__((packed));

struct LinkBitsCC
{
    u8 low7 : 7;
    u8 bit7 : 1;
} __attribute__((packed));

void sub_08087FCC(struct Actor *obj, s32 divisor);

/*
 * Starts a jump to (x, y, z), in pixels, that peaks at height peakH. A peak
 * below the landing or the current height makes it a straight move. Otherwise
 * unk94 / unk98 get the square roots of the fall and rise (the arc's shape for
 * the height curve). The move takes 0x20 frames (sub_08087FCC sets the speed).
 */
void sub_080865F8(struct Actor *obj, s32 x, s32 y, s32 z, s32 peakH)
{
    s32 *arcTotal;
    s32 *arcRise;
    s32 startZ;
    s32 top;
    s32 fallRoot;
    s32 riseRoot;
    s32 flip;

    x <<= 8;
    y = y * 0x100;
    z <<= 8;
    peakH <<= 8;
    obj->targetX = x;
    obj->targetY = y;
    obj->offX = (x - obj->posX) / 4;
    obj->offY = (y - obj->posY) / 4;
    if (peakH < z || peakH < obj->posZ)
    {
        /* no arc: straight to the landing height */
        obj->targetH = z;
        obj->offZ = (z - obj->posZ) / 4;
        obj->unk94 = 0;
    }
    else
    {
        obj->targetH = peakH;
        obj->offZ = 0;
        arcTotal = &obj->unk94;
        arcRise = &obj->unk98;
        startZ = obj->posZ;
        top = obj->targetH;
        flip = 0;
        if (top < startZ)
        {
            if (top < z)
            {
                top = startZ + z - top;
                flip = 1;
            }
            else
                top = startZ;
        }
        else if (top < z)
            top = z;
        /* gMathRoutines starts with the integer square root */
        fallRoot = ((s32 (*)(s32))gMathRoutines)((top - z) << 8);
        riseRoot = ((s32 (*)(s32))gMathRoutines)((top - startZ) << 8);
        *arcTotal = riseRoot + fallRoot;
        *arcRise = riseRoot;
        if (flip)
        {
            *arcTotal = -*arcTotal;
            *arcRise = -*arcRise;
        }
    }
    obj->progress = 0x4000;
    sub_08087FCC(obj, 0x20);
}

ASM_FUNC("asm/code_080865F8/contact_face.s", void contact_face(void))

/* Springs the object toward its record's target on x and h: the distance from the base point / 32
 * is added to the velocity (offX/offZ), which is then damped to 0xF2/0x100 and added to the
 * position. Once velocity and pull are both small (v*v + a*a <= 0x40 on each axis) it snaps to the
 * target and stops. */
void obj_step_velocity(struct Actor *obj)
{
    struct Rec *rec = obj->data;
    s32 distX = rec->targetX - obj->baseX;
    s32 distH;
    s32 pullX;
    s32 pullH;

    if (distX < 0)
        distX += 0x1F;
    pullX = distX >> 5;
    distH = rec->targetH - obj->baseH;
    if (distH < 0)
        distH += 0x1F;
    pullH = distH >> 5;
    *(u16 *)&obj->offX += pullX;
    *(u16 *)&obj->offZ += pullH;
    obj->offX = (obj->offX * 0xF2) / 0x100;
    obj->offZ = (obj->offZ * 0xF2) / 0x100;
    obj->posX += obj->offX;
    obj->posZ += obj->offZ;
    if (obj->offX * obj->offX + pullX * pullX <= 0x40 && obj->offZ * obj->offZ + pullH * pullH <= 0x40)
    {
        obj->posX = obj->data->targetX;
        obj->posZ = obj->data->targetH;
        obj->state = NULL;
    }
}

ASM_FUNC("asm/code_080865F8/obj_attach.s", void obj_attach(void))

/*
 * Attaches an actor in mode 4 (bits 1-2 of mode) to entry `index` of its
 * record's slot table, under `owner`: takes the entry's flags and offsets,
 * records the index in the record's status, sets the arrive / retarget
 * callbacks, starts the actor's sequence and then waits to run the entry's
 * routine (poll_link_routine).
 */
void obj_attach_entry(struct Actor *obj, struct Actor *owner, s32 index)
{
    struct SlotEntry *ent;
    s32 cleared;
    u8 heldCC;

    if ((obj->mode & 6) != 4)
        return;
    *((u8 *)gWorld + 0x2BF) |= 1; /* gWorld->unk2BF */
    obj->owner = owner;
    ((struct ObjFlags77 *)obj)->bit3 = 0;
    ((struct ObjFlags77 *)obj)->bit4 = 0;
    ent = &obj->data->table->entries[index];
    /* entry flag bits 5 and 6 that are clear */
    cleared = (ent->flags8 & 0x60) ^ 0x60;
    ((struct ObjFlags77 *)obj)->bit6 = (cleared >> 5) & 1;
    ((struct ObjFlags77 *)obj)->bit5 = (cleared >> 6) & 1;
    obj->link = (struct Actor *)ent;
    ((struct StatusBits110 *)&obj->data->status110)->low5 = index;
    obj->activeNext = (struct Actor *)on_arrive;
    obj->field54 = (s32)set_targets_from_record;
    obj->unk9C = ent->offE;
    obj->unkA0 = ent->off10;
    /* low 15 bits, sign-extended */
    obj->unkA4 = ((s32)(ent->unk12 << 17)) >> 17;
    heldCC = obj->unkCC & 0x80;
    if (heldCC != 0)
    {
        ((struct LinkBitsCC *)&obj->unkCC)->bit7 = 0;
    }
    else
    {
        obj->unkC8 = heldCC;
        ((struct LinkBitsCC *)&obj->unkCC)->low7 = 0;
        obj->unkCA = heldCC;
    }
    run_sequence_of_obj(obj);
    obj->state = poll_link_routine;
}

void obj_arrive_check(struct Actor *obj);
ASM_FUNC("asm/code_080865F8/obj_track_target.s", void obj_track_target(struct Actor *obj))

/* State run while an actor walks to its record's target (obj_track_target sets the step
   offX/offY/offZ): places it one step from its base, and once that step gets it no closer to the
   target (squared distances in 1/16 units, so they don't overflow) snaps it onto the target,
   stops its walk action, unflips its pose and returns to the state it had saved. */
void obj_arrive_check(struct Actor *obj)
{
    struct Rec *rec;
    s32 nextX;
    s32 nextY;
    s32 nextH;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    s32 nextDx;
    s32 nextDy;
    s32 nextDh;
    s32 baseDx;
    s32 baseDy;
    s32 baseDh;
    s32 nextDist2;
    s32 baseDist2;

    nextX = obj->baseX + obj->offX;
    obj->posX = nextX;
    nextY = obj->baseY + obj->offY;
    obj->posY = nextY;
    nextH = obj->baseH + obj->offZ;
    obj->posZ = nextH;
    rec = obj->data;
    targetX = rec->targetX;
    nextDx = (nextX - targetX) / 16;
    targetY = rec->targetY;
    nextDy = (nextY - targetY) / 16;
    targetH = rec->targetH;
    nextDh = (nextH - targetH) / 16;
    baseDx = (obj->baseX - targetX) / 16;
    baseDy = (obj->baseY - targetY) / 16;
    baseDh = (obj->baseH - targetH) / 16;
    nextDist2 = nextDx * nextDx + nextDy * nextDy + nextDh * nextDh;
    baseDist2 = baseDx * baseDx + baseDy * baseDy + baseDh * baseDh;
    if (nextDist2 >= baseDist2)
    {
        obj->posX = targetX;
        obj->posY = rec->targetY;
        obj->posZ = rec->targetH;
        sprite_set_action(obj, -1, -1, 0);
        /* bit 6: flipped horizontally (obj_track_target sets it when walking left) */
        ((struct PoseBits11 *)&obj->pose->flags11)->bit6 = 0;
        obj->field58 = 0;
        obj->state = obj->savedState;
        obj->savedState = NULL;
    }
}

s32 obj_stop(struct Actor *obj)
{
    s32 mode = obj->mode & 6;
    struct Rec *owner;
    u8 busy;

    if (mode == 6)
        return 0;
    if (mode == 2)
        return 0;
    owner = obj->data;
    if (owner == NULL)
        return 0;
    busy = owner->status111 & 2;
    if (busy != 0)
        return 0;
    obj->state = noop_state_08087c7c;
    obj->index = busy;
    task_create_list_4c(obj_stop_step, owner);
    {
        s32 status = owner->status111;

        owner->status111 = status | 1;
    }
    release_object(obj->data);
    return 1;
}

void obj_track_target(struct Actor *obj);
void start_placement(struct Actor *obj);
void obj_place_start(struct Actor *obj);
/* Starts a hero's placement and nudges its partner. The pair is (first, second) = the slot -2 hero
   and the slot -1 hero (data->slot); the partner is gWorld->slotTab[slot + 8] (the hero pointers at
   +0x70/+0x74, each one's actor at +8), or NULL while gWorld->unk84 is 0. If the partner is in
   mode 2 it walks to its target (obj_track_target) when unk48 <= 0, else starts its placement when
   gWorld->flags0C bit 6 is set. Then clears that bit and moves on to obj_place_start. */
void obj_slot_update(struct Actor *obj)
{
    s16 *slot = &obj->data->slot;
    struct Rec *first;
    struct Rec *second;

    if (*slot == -1)
    {
        second = obj->data;
        if (gWorld->unk84 != 0)
            first = gWorld->slotTab[*slot + 8];
        else
            first = NULL;
    }
    else
    {
        first = obj->data;
        if (gWorld->unk84 != 0)
            second = gWorld->slotTab[*slot + 8];
        else
            second = NULL;
    }
    // The partner is whichever of the two this actor isn't.
    if (obj->data == second)
    {
        if ((first->flags7E & 6) == 2)
        {
            if (first->unk48 <= 0)
                obj_track_target((struct Actor *)((u8 *)first + 8));
            else if (gWorld->flags0C & 0x40)
                ((struct Actor *)((u8 *)first + 8))->state = start_placement;
        }
    }
    else
    {
        if ((second->flags7E & 6) == 2)
        {
            if (second->unk48 <= 0)
                obj_track_target((struct Actor *)((u8 *)second + 8));
            else if (gWorld->flags0C & 0x40)
                ((struct Actor *)((u8 *)second + 8))->state = start_placement;
        }
    }
    ((struct PoseFlags11 *)&gWorld->flags0C)->bit6 = 0;
    obj->state = obj_place_start;
}

/*
 * Spawns a follower for obj from the gOffsetRows row at halfword `row`: the row's first 6 values go to
 * unk44-unk49, and after them come x/y/z offsets per animation frame. The follower stays at obj's
 * current position plus the offset for obj's frame (mirrored with obj's flip) while obj keeps playing
 * the same animation (effect_follow). It is placed right away, and obj takes its field58-field64.
 */
struct Actor *effect_from_table_obj(struct Actor *obj, s32 row)
{
    struct Actor *follower;
    void (*follow)(struct Actor *);
    const u16 *header;
    struct Pose *followerPose;

    follower = obj_spawn(obj->data, 0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, 0, 1);
    header = &((const u16 *)gOffsetRows)[row];
    ((struct HalfBit6 *)&follower->mode)->bit6 = 1;
    follower->unk44 = header[0];
    follower->unk45 = header[1];
    follower->unk46 = header[2];
    follower->unk47 = header[3];
    follower->unk48 = header[4];
    follower->unk49 = header[5];
    follower->owner = obj->owner;
    follower->targetX = obj->posX;
    follower->targetY = obj->posY;
    follower->targetH = obj->posZ;
    follower->motionSpeed = obj->pose->waitFlag21; /* the animation it follows */
    follower->child = obj;
    follower->motionStart = row;
    followerPose = follower->pose;
    {
        register u32 flags asm("r0") = obj->pose->flags11;
        ((struct PoseBits11 *)&followerPose->flags11)->bit6 = (flags << 25) >> 31;
    }
    ((struct HalfBit0 *)&follower->flags)->bit0 = 1;
    follower->link = obj->link;
    follow = effect_follow;
    follower->state = follow;
    obj_list_push_default(follower);
    follow(follower);
    follower->baseX = follower->posX;
    follower->baseY = follower->posY;
    follower->baseH = follower->posZ;
    obj->field58 = follower->field58;
    obj->field5C = follower->field5C;
    obj->field60 = follower->field60;
    obj->field64 = follower->field64;
    return follower;
}

/* State of a follower made by effect_from_table_obj: released once the object it follows
 * (child) is gone, plays another animation, or no other live object is left in
 * gWorld->list78. Otherwise placed at its anchor (targetX/Y/H) plus the gOffsetRows offset for
 * the followed pose's current frame (pose->unk23), mirrored in x when that pose is flipped. */
void effect_follow(struct Actor *obj)
{
    struct Pose *pose;
    struct Rec *cur;
    const s16 *offset;
    s32 anchorX;
    s32 x;
    s32 frameOff;
    const u8 *row;

    pose = obj->child->pose;
    if (pose == NULL || pose->waitFlag21 != obj->motionSpeed || (pose->flags12 & POSE_FREE) != 0)
    {
        obj_release(obj);
        return;
    }
    cur = gWorld->list78;
    for (;;)
    {
        if (cur == NULL)
        {
            obj_release(obj);
            return;
        }
        if (cur != (struct Rec *)obj && cur->state != NULL)
            break;
        cur = cur->ringNext;
    }
    /* the row starts with a 6-halfword header, then (x, y, z) per frame */
    row = (const u8 *)gOffsetRows + (obj->motionStart << 1);
    frameOff = pose->unk23 * 6 + 12;
    offset = (const s16 *)(row + frameOff);
    anchorX = obj->targetX;
    if ((pose->flags11 & POSE_FLIPPED) != 0)
        x = anchorX - (offset[0] << 8);
    else
        x = anchorX + (offset[0] << 8);
    obj->posX = x;
    obj->posY = obj->targetY + (offset[1] << 8);
    obj->posZ = obj->targetH + (offset[2] << 8);
}

/* Starts a hero's hop to its placement target (data->targetX/Y) in three jumps: plays action
   gActionCodes[5] (the next code for the slot -2 hero), aims the first jump a third of the way from
   base to target at speed 0x700 and 24 px above baseH, and leaves the other two to obj_place_step. */
void obj_place_start(struct Actor *obj)
{
    s32 action = gActionCodes[5];
    s32 x;
    s32 y;

    if (obj->data->slot == -2)
        action += 1;
    sprite_set_action(obj, 0, action, 0);
    ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
    obj->waitCount = 3;
    x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetX + obj->baseX * 2) / 0x100, 3);
    y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetY + obj->baseY * 2) / 0x100, 3);
    place_targets(obj, x, y, 0, 0x100);
    set_step_and_offsets(obj, 0x700, (obj->baseH / 0x100) + 0x18);
    obj->waitCount--;
    obj->state = obj_place_step;
}

u8 step_motion(struct Actor *obj);
void timed_wait_step(struct Actor *obj);
/* One frame of the placement hops started by obj_place_start: when a jump lands and jumps are left,
   aims the next one 1/n of the way from base to data->targetX/Y (n = jumps left) at speed 0x700 and
   20 px above baseH; after the last one, waits 20 frames in timed_wait_step. */
void obj_place_step(struct Actor *obj)
{
    s32 jumpsLeft;
    s32 *count;
    s32 x;
    s32 y;

    if (step_motion(obj) == 0)
    {
        count = &obj->waitCount;
        jumpsLeft = *count;
        if (jumpsLeft != 0)
        {
            x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetX + obj->baseX * (jumpsLeft - 1)) / 0x100, jumpsLeft);
            y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetY + obj->baseY * (jumpsLeft - 1)) / 0x100, jumpsLeft);
            place_targets(obj, x, y, 0, 0x100);
            set_step_and_offsets(obj, 0x700, (obj->baseH / 0x100) + 0x14);
            *count = *count - 1;
        }
        else
        {
            *count = 0x14;
            obj->state = timed_wait_step;
        }
    }
}

/* State after the placement hops: once the pose's animation ends, sends the hero (speed 0x400) to its
   record's target point, plays its stand action (0x2002 for the slot -1 hero, else 0x204F) mirrored
   (PoseFlags11.bit6 is POSE_FLIPPED), and runs the move through enter_finish_rebuild with no state
   to return to. */
void obj_finish_placement(struct Actor *obj)
{
    struct Rec *hero;
    s32 action;

    if ((obj->pose->flags12 & 8) != 0)
    {
        hero = obj->data;
        place_targets(obj, hero->targetX / 0x100, hero->targetY / 0x100, hero->targetH / 0x100, 0x100);
        set_motion_start(obj, 0x400);
        action = (hero->slot == -1) ? 0x2002 : 0x204F;
        sprite_set_action(obj, 0, action, 0);
        ((struct PoseFlags11 *)&obj->pose->flags11)->bit6 = 1;
        ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
        obj->state = NULL;
        enter_finish_rebuild(obj);
    }
}

/* inlined copy of query_contact (defined further down) */
static inline s32 query_contact_inline(struct Actor *obj, struct Actor *other, s32 kind)
{
    struct CandMgr *mgr;

    if (kind != 3)
        return 1;
    if ((obj->mode & 6) == 4 && obj->link != NULL && (((u8 *)obj->link)[9] & 0x80) == 0)
        return 1;
    set_slot_value(0x2A, -1);
    if (other->data->slot == -1)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 1;
    }
    if (other->data->slot == -2)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 4;
    }
    return 0;
}

/* Contact handler: when a partner touches the object with contact kind 3, records the contact
   (query_contact), resolves the partner's action (0x04021010 for slot -1, 0x04021011 otherwise)
   and, if the object is its record's main actor or one of the parts chained from it, links it to
   the partner (actor_init_linked), or makes it wait for its pose while gWorld->unk2F9 is 1.
   Returns query_contact's or the action's result when they handle it, -1 once linked, else 0. */
s32 lookup_part(struct Actor *obj, struct Actor *partner, s32 kind)
{
    struct Rec *rec;
    struct Actor *part;
    s32 result;
    u8 isPart;

    result = query_contact_inline(obj, partner, kind);
    if (result != 0)
        return result;
    result = resolve_contact_action(partner, obj, 0, 0, (partner->data->slot == -1) ? 0x04021010 : 0x04021011);
    if (result > 0)
        return result;
    rec = obj->data;
    if (rec == NULL)
        return 0;
    isPart = 0;
    /* not the record's main actor (embedded at +8): look for it in the part chain */
    if ((void *)obj != (u8 *)rec + 8)
    {
        part = rec->field38;
        if (part != NULL)
        {
            if (obj == part)
                goto link;
            do
            {
                part = part->child;
                if (part == NULL)
                    goto check;
            } while (obj != part);
            isPart = 1;
        }
    check:
        if (!isPart)
            return 0;
    }
link:
    obj->index = 0;
    if (gWorld->unk2F9 == 1)
    {
        obj->state = wait_pose_flag;
        return 0;
    }
    obj->owner = partner;
    actor_init_linked(obj);
    return -1;
}

/* Contact check that only counts hits from the front: returns 1 when `partner` isn't its record's main
   actor, -1 when `obj` lies outside the half-turn in front of the partner's facing angle (partner->unkA0,
   degrees in 8.8; flipped by half a turn when the partner's pose is mirrored), and otherwise records the
   contact (slot value 0x7D, gWorld's status nibble 1 for slot -1, 4 for slot -2) and returns 0. */
s32 lookup_slot(struct Actor *obj, struct Actor *partner)
{
    s32 partnerX;
    s32 partnerH;
    s32 objX;
    s32 objH;
    s32 angle;
    s32 facing;
    s32 wrapped;
    struct CandMgr *mgr;

    if (partner != partner->data->field38)
        return 1;
    partnerX = partner->posX / 0x100;
    partnerH = (-partner->posZ) / 0x100;
    objX = obj->posX / 0x100;
    objH = (-obj->posZ) / 0x100;
    angle = (u16)arc_tan2((s16)(objX - partnerX), (s16)(objH - partnerH));
    /* degrees (8.8) to a 16-bit binary angle */
    wrapped = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((-partner->unkA0) << 8, 360);
    facing = wrapped & 0xFFFF;
    if ((partner->data->pose10->flags11 & 0x40) != 0)
        angle = (angle + 0x8000) & 0xFFFF;
    if (angle < facing)
        angle += 0xFFFF;
    if (angle - facing > 0x8000)
        return -1;
    set_slot_value(0x7D, -1);
    if (partner->data->slot == -1)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 1;
    }
    if (partner->data->slot == -2)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 4;
    }
    return 0;
}

void *find_lowest_order_pose(struct Actor *obj)
{
    struct Pose *best = obj->pose;
    struct Actor *child;
    struct Rec *head;
    struct Rec *rec;
    struct Rec *next;

    for (child = obj->child; child != NULL; child = child->child)
    {
        if ((child->flags & 8) && child->pose->unkE < best->unkE)
            best = child->pose;
    }
    head = obj->data;
    rec = head->ringNext;
    if (rec != NULL)
    {
        while (rec != head)
        {
            if (rec->pose10->unkE < best->unkE)
                best = rec->pose10;
            child = rec->field38;
            next = rec->ringNext;
            for (; child != NULL; child = child->child)
            {
                if ((child->flags & 8) && child->pose->unkE < best->unkE)
                    best = child->pose;
            }
            rec = next;
        }
    }
    return best;
}

NONMATCH("asm/code_080865F8/arbitrate_candidates.s", void arbitrate_candidates(struct Actor *obj))
{
    struct CandMgr *mgr;
    struct Pose *op;
    struct Actor *a;
    struct Actor *b;
    struct Rec *ca;
    struct Rec *cb;
    struct Pose *w;
    s32 at;
    s32 bt;
    s32 mask;

    op = obj->pose;
    mgr = gWorld;
    ca = mgr->candA;
    a = (struct Actor *)((u8 *)ca + 8);
    cb = mgr->candB;
    b = (struct Actor *)((u8 *)cb + 8);
    if ((a->mode & 6) == 2)
        a->pose->unkE = op->unkE - 2;
    if ((b->mode & 6) == 2)
        b->pose->unkE = op->unkE - 2;
    if ((a->mode & 6) == 2)
    {
        if ((b->mode & 6) == 2)
        {
            at = (s8)ca->unk7C + (s8)ca->unk7D;
            bt = (s8)cb->unk7C + (s8)cb->unk7D;
            if (at < bt)
                w = b->pose;
            else if (bt < at)
                w = a->pose;
            else if (a->posY < b->posY)
                w = a->pose;
            else
                w = b->pose;
            w->unkE += 1;
        }
        if ((a->mode & 6) == 2 && a->state != NULL)
            return;
    }
    if ((b->mode & 6) == 2 && b->state != NULL)
        return;
    *((u8 *)a + 0x74) = 0;
    *((u8 *)a + 0x75) = 0;
    *((u8 *)b + 0x74) = 0;
    *((u8 *)b + 0x75) = 0;
    mask = -5;
    a->flags &= mask;
    b->flags &= mask;
    obj->routine4 = NULL;
}
END_NONMATCH

void clear_candidate_pair(struct Actor *obj)
{
    struct CandMgr *mgr = gWorld;
    struct Rec *a = mgr->candA;
    struct Actor *objA = (struct Actor *)((u8 *)a + 8);
    struct Rec *b = mgr->candB;
    struct Actor *objB = (struct Actor *)((u8 *)b + 8);

    if ((a->flags7E & 6) == 2 && objA->state != NULL)
        return;
    if ((b->flags7E & 6) == 2 && objB->state != NULL)
        return;
    a->unk7C = 0;
    a->unk7D = 0;
    b->unk7C = 0;
    b->unk7D = 0;
    ((struct FlagBits81 *)&a->flags81)->bit2 = 0;
    ((struct FlagBits81 *)&b->flags81)->bit2 = 0;
    obj->routine4 = NULL;
}

void schedule_pose_task(struct Actor *obj)
{
    struct Rec *rec;
    struct Actor *child;
    s32 flags;

    if (obj == NULL)
        return;
    flags = 0;
    rec = obj->data;
    if (rec->ringNext == NULL)
        flags = 1;
    else if (rec->ringNext == rec)
        flags = 1;
    child = obj->child;
    for (;;)
    {
        if (child == NULL)
        {
            flags |= 2;
            break;
        }
        if (child->flags & 8)
            break;
        child = child->child;
    }
    if (flags == 3)
        task_create_list_4c(clear_candidate_pair, obj);
    else
        task_create_list_4c(mark_candidate_pair, find_lowest_order_pose(obj));
}

s32 state_clear_08087958(struct Actor *obj)
{
    obj->state = NULL;
    return 1;
}

s32 lookup_slot();
s32 lookup_link_slot(struct Actor *a, struct Actor *b)
{
    struct Rec *rec;
    struct Actor *cur;
    s32 result;
    u8 found;

    result = lookup_slot(a, b);
    if (result != 0)
        return result;
    result = resolve_contact_action(b, a, 0, 0, (b->data->slot == -1) ? 0x8032012 : 0x8032013);
    if (result > 0)
        return result;
    rec = a->data;
    if (rec != NULL)
    {
        found = 0;
        if ((void *)a != (u8 *)rec + 8)
        {
            cur = rec->field38;
            if (cur != NULL)
            {
                if (a == cur)
                    goto set;
                do
                {
                    cur = cur->child;
                    if (cur == NULL)
                        goto check;
                } while (a != cur);
                found = 1;
            }
        check:
            if (!found)
                return 0;
        }
    set:
        a->index = 0;
        a->state = wait_pose_flag;
    }
    return 0;
}

s32 query_contact(struct Actor *obj, struct Actor *other, s32 kind)
{
    struct CandMgr *mgr;

    if (kind != 3)
        return 1;
    if ((obj->mode & 6) == 4 && obj->link != NULL && (((u8 *)obj->link)[9] & 0x80) == 0)
        return 1;
    set_slot_value(0x2A, -1);
    if (other->data->slot == -1)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 1;
    }
    if (other->data->slot == -2)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 4;
    }
    return 0;
}

void start_placement(struct Actor *obj)
{
    s32 flags;

    place_targets(obj, (obj->baseX / 0x100) - 0x40, obj->baseY / 0x100, 0, 0x100);
    set_step_and_offsets(obj, 0x700, (obj->baseH / 0x100) + 0x18);
    sprite_set_action(obj, 0, gActionCodes[0] + ~obj->data->slot, 0);
    flags = obj->pose->flags12;
    obj->pose->flags12 = flags | 0x10;
    obj->state = finish_step_and_place;
}

void timed_wait_step(struct Actor *obj)
{
    s32 timer = obj->data->timer;
    void (*next)(struct Actor *);

    if (timer == 0)
    {
        s32 status = obj->data->status111;

        obj->data->status111 = status | 1;
        task_create_list_4c(task_release_obj, obj);
        obj->waitCount = timer;
        next = noop_state_080883dc;
    }
    else
    {
        if (obj->waitCount > 0)
        {
            obj->waitCount--;
            return;
        }
        sprite_set_action(obj, 0, -1, 0);
        ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
        next = NULL;
    }
    obj->state = next;
}

void place_and_start_action5(struct Actor *obj, s32 a, s32 b)
{
    place_targets(obj, obj->baseX / 0x100, obj->baseY / 0x100, 0, 0x100);
    set_step_and_offsets(obj, b, a);
    obj->state = NULL;
    start_action_5(obj);
}

void candidate_dispatch(void)
{
    struct CandMgr *mgr = gWorld;

    if (mgr->flags0C & 0x80)
        pair_begin_setup_b((u8 *)mgr->cand7C + 8);
    else
        pair_begin_setup((u8 *)mgr->cand7C + 8);
}

void release_object_data(struct Actor *obj)
{
    if (obj != NULL)
    {
        release_object(obj);
        task_list_remove(obj);
        if (obj->counter <= 5)
            obj->dataPtr = NULL;
    }
}

void motion_timed(struct Actor *obj, s32 a, s32 w)
{
    struct Pose *pose;

    sprite_set_action(obj, a, 0, 0);
    pose = obj->pose;
    if (w != 0)
    {
        w = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(w << 4, (s16)obj->timer);
        if (w <= 0)
            w = 1;
        if (w > 0x7E)
            w = 0x7F;
        pose->amount20 = w;
        ((struct PoseBits *)&pose->flags12)->mode = 1;
    }
    else
    {
        ((struct PoseBits *)&pose->flags12)->mode = 0;
    }
    obj->state = timed_motion_done;
}

void obj_stop_step(struct Actor *obj)
{
    obj->pose->unkBA = 0;
    obj->routine4 = place_part_sprite;
}

void noop_state_08087c7c(struct Actor *obj)
{
}

void obj_release(struct Actor *obj);
extern const s16 gOffsetRows[];
s32 process_record_ring(struct Actor *obj)
{
    register struct Rec *next asm("r1");
    struct Rec *rec = obj->data;
    register struct Rec *cur asm("r4");
    if ((next = rec->ringNext) != NULL)
    {
        do {
            cur = next;
            if (cur->status111 & 2)
                task_create_list_4c(stop_unless_flagged, cur);
            else
                obj_stop((struct Actor *)((u8 *)cur + 8));
        } while ((next = cur->ringNext) != rec);
    }
    if (rec->status111 & 2)
        task_create_list_4c(stop_unless_flagged, rec);
    else
        obj_stop(obj);
    return 1;
}

void set_targets_from_record(struct Actor *obj)
{
    obj->targetX = obj->data->targetX;
    obj->targetY = obj->data->targetY;
    obj->targetH = obj->data->targetH;
    {
        u8 *flagsPtr = &obj->flags;

        *flagsPtr |= 0x20;
    }
    obj->motionSpeed = 0x800;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = link_index_state;
}

void wait_for_index(struct Actor *obj)
{
    if (obj->index == 0)
        obj->state = set_targets_from_record;
}

void wait_pose_flag(struct Actor *obj)
{
    if (obj->pose->waitFlag21 == 0)
        obj->state = set_targets_from_record;
}

void on_arrive(struct Actor *obj)
{
    struct Rec *rec = obj->data;

    if (rec != NULL && obj == (struct Actor *)((u8 *)rec + 8) && obj->pose != NULL)
    {
        rec->arrive78 = 0;
        sprite_set_action(obj, -1, -1, 0);
    }
    obj_list_remove(obj);
    obj->activeNext = NULL;
    obj->field54 = 0;
    obj->state = NULL;
    obj->link = NULL;
    obj->owner = NULL;
    if ((obj->mode & 6) == 2)
        set_cur_flags_both(0x40000);
}

void poll_link_routine(struct Actor *obj)
{
    if (is_state_busy() == 0)
        obj->link->routine4(obj);
}

void candidates_countdown_then_state(struct Actor *obj)
{
    u16 t = obj->timer;

    if ((s16)obj->timer > 0)
        obj->timer = t - 1;
    else
        obj->state = obj_step_velocity;
}
