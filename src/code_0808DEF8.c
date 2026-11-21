#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0x11];
    u8 dir;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 result;
    u8 filler21[0x23 - 0x21];
    u8 count;
};

struct Data
{
    u8 filler0[0xE0];
    s32 ground;
    u8 fillerE4[0xEC - 0xE4];
    s16 slot;
    u8 fillerEE[0x111 - 0xEE];
    u8 lock;
    u8 filler112[0x114 - 0x112];
    u32 slotValue;
    u8 filler118[0x12D - 0x118];
    u8 kind;
};

struct Actor
{
    u8 filler0[0x08];
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 unk10; /* x position */
    u8 filler14[0x18 - 0x14];
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct Data *data;
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
    u8 filler50[0x68 - 0x50];
    void (*callback)(struct Actor *);
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 action;
    u8 flags;
    u8 filler78;
    u8 flags79;
    u8 filler7A[0x84 - 0x7A];
    s32 targetX;
    u8 filler88[0x8C - 0x88];
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 speed;
    u8 filler98[0x9C - 0x98];
    s32 timer;
    s32 unkA0; /* orbiting child: angle, sine table index = unkA0 / 360 (0xB400 = half a turn) */
    s32 unkA4; /* orbiting child: angle speed added to unkA0 each frame */
    u32 keyMask;
    u16 step;
    u8 fillerAE[0xB2 - 0xAE];
    s16 dh;
};

struct ActBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 kind : 3;
    u8 unk6 : 2;
};

#define ACT_MODE_MASK 0x06 // ActBits.mode
#define ACT_KIND_MASK 0x38 // ActBits.kind: set when the action is interrupted

struct ObjFlagBits
{
    u8 phase : 3;
    u8 unk3 : 2;
    u8 switchA : 1;
    u8 switchB : 1;
    u8 unk7 : 1;
};

struct PoseFlagBits
{
    u8 unk0_0 : 1;
    u8 mode : 2;
    u8 unk0_3 : 1;
    u8 hold : 1;
    u8 unk0_5 : 3;
};

#define POSE_HOLD_MASK 0x10 // PoseFlagBits.hold: keep the current animation frame

struct PoseDirBits
{
    u8 unk0_0 : 6;
    u8 turn : 1;
    u8 unk0_7 : 1;
};

struct DataLockBits
{
    u8 unk0 : 2;
    u8 locked : 1;
    u8 unk3 : 5;
};

struct ListEntry
{
    u8 filler0[0x2A];
    u16 value;
    u16 value2;
};

extern u8 *gParty;
extern u8 *gWorld;
struct SysState
{
    u8 filler0[0x3C];
    u16 keysHeld;
};

extern struct SysState gSys;
extern const u8 gAnim284[];
extern const u8 gAnim278[];
extern const u8 gAnim374[];

void set_slot_value(u32 id, s32 value);
void obj_release(struct Actor *obj);
void on_arrive(struct Actor *obj);

void end_action(struct Actor *obj);
s32 pause_jump_a(struct Actor *obj);
s32 pause_jump_b(struct Actor *obj);
struct Actor *obj_spawn(struct Data *data, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void jump_orbit_spawn_orbit_child_a(struct Actor *obj);
void jump_orbit_spawn_orbit_child_b(struct Actor *obj);
void jump_orbit_step_a(struct Actor *obj);
void jump_orbit_step_b(struct Actor *obj);
void jump_orbit_hold_a(struct Actor *obj);
void jump_orbit_end_a(struct Actor *obj);
void jump_orbit_end_b(struct Actor *obj);
void finish_orbit_b(struct Actor *obj);
void orbit_position_b(struct Actor *obj);
void end_action_if_done(struct Actor *obj);
void set_cur_flags_both(u32 value);
void jump_orbit_step_d(struct Actor *obj);
void jump_orbit_end_action_if_pose_visible(struct Actor *obj);
void jump_orbit_circle_a(struct Actor *obj);
void jump_orbit_circle_b(struct Actor *obj);
void jump_orbit_hold_b(struct Actor *obj);
void jump_orbit_step_c(struct Actor *obj);
void jump_orbit_start_jump_arc_a(struct Actor *obj);
s32 poll_action_end(struct Actor *obj);
void jump_arc_fall_a(struct Actor *obj);
void jump_arc_fall_b(struct Actor *obj);
void finish_fall(struct Actor *obj);
void jump_orbit_finish_if_pose_visible_e(struct Actor *obj);
void jump_orbit_arc_rise_a(struct Actor *obj);
void jump_orbit_arc_rise_b(struct Actor *obj);
void jump_orbit_arc_land_a(struct Actor *obj);
void jump_orbit_arc_land_b(struct Actor *obj);
u8 try_park_in_slot(struct Actor *obj);
void jump_orbit_arc_rise_c(struct Actor *obj);
void jump_orbit_arc_rise_d(struct Actor *obj);
void jump_orbit_start_jump_arc_b(struct Actor *obj);
void jump_orbit_jump_timer_arc_b(struct Actor *obj);
void jump_orbit_dispatch_slot_spawn(struct Actor *obj);
void jump_orbit_jump_timer_arc_a(struct Actor *obj);
void jump_orbit_finish_if_pose_visible_a(struct Actor *obj);
void jump_orbit_finish_if_pose_visible_b(struct Actor *obj);
void jump_orbit_start_jump_arc_c(struct Actor *obj);
void jump_orbit_start_jump_arc_d(struct Actor *obj);
void sprite_set_action(struct Actor *obj, u32 action, u32 arg, u32 extra);
void orbit_slots_spawn_orbit_child(struct Actor *obj);
void orbit_slots_spawn_orbit_child_b(struct Actor *obj);

static inline void clear_switches(struct Actor *obj)
{
    ((struct ObjFlagBits *)&obj->flags)->switchB = 0;
    ((struct ObjFlagBits *)&obj->flags)->switchA = 0;
}

/* Waits while the button (keyMask) is held. On release it starts orbit b: the child starts half a
 * turn round at angle speed 0xA00 and obj goes on to orbit_position_b. Held for more than 0x2C
 * frames the move fails: kind 0x1E waits there unless world byte 9 has 1 in its upper nibble
 * (then action 4 with pose result 0xC0 / 12), any other kind takes action 5 and sets the both-heroes
 * flag 0x40000000. Past 0x1D frames the pose's hold bit is cleared. */
void hold_then_orbit_b(struct Actor *obj)
{
    u32 held;
    s16 *holdFrames;

    if (poll_action_end(obj) != 0)
        return;
    held = gSys.keysHeld & obj->keyMask;
    if (held == 0)
    {
        obj->timer = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        *(u8 *)&obj->pose->flags |= 0x10; // PoseFlagBits.hold
        obj->state = orbit_position_b;
        obj->step = held;
        return;
    }
    holdFrames = (s16 *)&obj->step;
    if ((s16)*holdFrames > 0x2C)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 0xC);
                ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
                obj->state = end_action_if_done;
            }
            else
            {
                obj->step = 0x2D;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
            obj->state = end_action_if_done;
            set_cur_flags_both(0x40000000);
        }
        return;
    }
    if ((s16)*holdFrames > 0x1D)
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->step = obj->step + 1;
}

/* One frame of orbit b: swings the child around a circle of radius 24 above obj's base point
 * (child angle unkA0 += its speed unkA4, the speed bent towards obj's facing), steps obj's
 * animation 0x2054 through frames 1-5, and at step 9 releases the child (action mode 3), locks
 * obj's data and goes on to finish_orbit_b. */
void orbit_position_b(struct Actor *obj)
{
    struct Actor *child;
    const s16 *cosTable;
    s16 *stepp;
    u32 tableIdx;
    s32 angle;
    s32 centerH;

    if (poll_action_end(obj) != 0)
        return;
    child = obj->child;
    ((struct ActBits *)&child->action)->mode = 1;
    child->unkA0 += child->unkA4;
    angle = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->unkA0, 360);
    cosTable = gCosineTable;
    tableIdx = angle & 0xFF;
    child->unk10 = obj->baseX + (cosTable[tableIdx] / 64) * 24;
    centerH = obj->baseH + 0x1A00;
    child->h = centerH + (gSineTable[tableIdx] / 64) * 24;
    if (obj->pose->dir & 0x40)
        child->unkA4 += 0x998;
    else
        child->unkA4 += -0x998;
    stepp = (s16 *)&obj->step;
    if (*stepp == 3)
        sprite_set_action(obj, 2, 0x2054, 1);
    if (*stepp == 5)
        sprite_set_action(obj, 2, 0x2054, 2);
    if (*stepp == 6)
        sprite_set_action(obj, 2, 0x2054, 3);
    if (*stepp == 7)
        sprite_set_action(obj, 2, 0x2054, 4);
    if (*stepp == 8)
        sprite_set_action(obj, 2, 0x2054, 5);
    if (*stepp == 9)
    {
        *(u8 *)&child->action |= 6; // ActBits.mode = 3
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = finish_orbit_b;
        if (obj->data != NULL)
            *(u8 *)&obj->data->lock |= 4; // DataLockBits.locked
        return;
    }
    *(u8 *)&obj->pose->flags |= 0x10; // PoseFlagBits.hold
    obj->step = obj->step + 1;
}


static inline void finish_pose_and_action(struct Actor *obj)
{
    ((struct PoseDirBits *)&obj->pose->dir)->turn = 0;
    ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->pose->result = 0x10;
    sprite_set_action(obj, 0, -1, 0);
}

/* After orbit_position_b has thrown the child: unlocks obj's data again, then, once 28 more
 * frames have passed (unless poll_action_end ends it first), ends the action like
 * actor_end_action: drops the child, resets the pose and returns to action 0. */
void finish_orbit_b(struct Actor *obj)
{
    s32 ended;
    u16 step;

    if (obj->data != NULL)
        ((struct DataLockBits *)&obj->data->lock)->locked = 0;
    ended = poll_action_end(obj);
    if (ended == 0)
    {
        step = obj->step;
        obj->step = step + 1;
        if ((s16)step > 0x1B)
        {
            if (((struct ActBits *)&obj->action)->mode == 1)
            {
                if (obj->data != NULL)
                    ((struct DataLockBits *)&obj->data->lock)->locked = 0;
                if (obj->child != NULL)
                {
                    obj_release(obj->child);
                    obj->child = NULL;
                }
                finish_pose_and_action(obj);
                obj->state = NULL;
            }
        }
    }
}

// Rising half of hero A's jump (button A; enter_jump_arc_fall_b is hero B's with B): the rise above
// baseH (dh) moves by the vertical speed each frame, and holding A eases the speed toward the fall
// speed -0x7A by 10/256 of the gap per frame (a higher jump); without it the speed is -0x7A at once.
// Once back down at baseH it takes the falling action 6 (images 0x2000) in play mode 1, sets flag
// phase 1 and falls the rest of the way in jump_arc_fall_a.
void enter_jump_arc_fall_a(struct Actor *obj)
{
    u16 *rise;
    s32 *vspeed;
    s32 h;
    s32 next;
    s32 gap;

    rise = (u16 *)&obj->dh;
    vspeed = &obj->speed;
    obj->dh = *rise + *vspeed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gSys.keysHeld & A_BUTTON)
    {
        gap = -0x7A - *vspeed;
        next = *vspeed + (gap * 10) / 0x100;
    }
    else
        next = -0x7A;
    *vspeed = next;
    if (obj->dh > 0)
        return;
    sprite_set_action(obj, 6, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    ((struct ObjFlagBits *)&obj->flags)->phase = 1;
    obj->state = jump_arc_fall_a;
}

// Falling half of hero A's jump (images 0x2000, button A; jump_arc_fall_b is hero B's with B):
// the rise above baseH (dh) moves by the vertical speed each frame. Holding A eases the speed
// toward the fall speed -0x7A by 10/256 of the gap per frame, a floatier fall; without it the
// speed is -0x7A at once. On reaching the ground it lands there with action 8 in play mode 1 and
// waits in jump_orbit_finish_if_pose_visible_e.
void jump_arc_fall_a(struct Actor *obj)
{
    u16 *rise;
    s32 *vspeed;
    s32 h;
    s32 next;
    s32 gap;

    rise = (u16 *)&obj->dh;
    vspeed = &obj->speed;
    obj->dh = *rise + *vspeed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gSys.keysHeld & A_BUTTON)
    {
        gap = -0x7A - *vspeed;
        next = *vspeed + (gap * 10) / 0x100;
    }
    else
        next = -0x7A;
    *vspeed = next;
    if (obj->h > obj->data->ground)
        return;
    ((struct ObjFlagBits *)&obj->flags)->phase = 0;
    obj->h = obj->data->ground;
    sprite_set_action(obj, 8, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = jump_orbit_finish_if_pose_visible_e;
}

// Rising half of hero B's jump (button B; enter_jump_arc_fall_a is hero A's with A): the rise above
// baseH (dh) moves by the vertical speed each frame, and holding B eases the speed toward the fall
// speed -0x7A by 10/256 of the gap per frame (a higher jump); without it the speed is -0x7A at once.
// Once back down at baseH it takes the falling action 6 (images 0x204D) in play mode 1, sets flag
// phase 1 and falls the rest of the way in jump_arc_fall_b.
void enter_jump_arc_fall_b(struct Actor *obj)
{
    u16 *rise;
    s32 *vspeed;
    s32 h;
    s32 next;
    s32 gap;

    rise = (u16 *)&obj->dh;
    vspeed = &obj->speed;
    obj->dh = *rise + *vspeed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gSys.keysHeld & B_BUTTON)
    {
        gap = -0x7A - *vspeed;
        next = *vspeed + (gap * 10) / 0x100;
    }
    else
        next = -0x7A;
    *vspeed = next;
    if (obj->dh > 0)
        return;
    sprite_set_action(obj, 6, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    ((struct ObjFlagBits *)&obj->flags)->phase = 1;
    obj->state = jump_arc_fall_b;
}

// Falling half of hero B's jump (images 0x204D, button B; jump_arc_fall_a is hero A's with A):
// the rise above baseH (dh) moves by the vertical speed each frame. Holding B eases the speed
// toward the fall speed -0x7A by 10/256 of the gap per frame, a floatier fall; without it the
// speed is -0x7A at once. On reaching the ground it lands there with action 8 in play mode 1 and
// waits in finish_fall.
void jump_arc_fall_b(struct Actor *obj)
{
    u16 *rise;
    s32 *vspeed;
    s32 h;
    s32 next;
    s32 gap;

    rise = (u16 *)&obj->dh;
    vspeed = &obj->speed;
    obj->dh = *rise + *vspeed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gSys.keysHeld & B_BUTTON)
    {
        gap = -0x7A - *vspeed;
        next = *vspeed + (gap * 10) / 0x100;
    }
    else
        next = -0x7A;
    *vspeed = next;
    if (obj->h > obj->data->ground)
        return;
    ((struct ObjFlagBits *)&obj->flags)->phase = 0;
    obj->h = obj->data->ground;
    sprite_set_action(obj, 8, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = finish_fall;
}

void finish_fall(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void jump_orbit_finish_if_pose_visible_e(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

void dispatch_slot_spawn_ext(struct Actor *obj)
{
    if (obj->data->slot == -1)
        orbit_slots_spawn_orbit_child(obj);
    else
        orbit_slots_spawn_orbit_child_b(obj);
}

void end_action(struct Actor *obj)
{
    if (((struct ActBits *)&obj->action)->mode == 1)
    {
        if (obj->data != NULL)
            ((struct DataLockBits *)&obj->data->lock)->locked = 0;
        if (obj->child != NULL)
        {
            obj_release(obj->child);
            obj->child = NULL;
        }
        ((struct PoseDirBits *)&obj->pose->dir)->turn = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->pose->result = 0x10;
        sprite_set_action(obj, 0, -1, 0);
        obj->state = 0;
    }
}

s32 jump_orbit_setup_list_slot_a(struct Actor *obj)
{
    struct ListEntry *entry;

    obj->anim = gAnim374;
    entry = (struct ListEntry *)(gParty + 0x3C);
    clear_switches(obj);
    obj->data->slotValue = entry->value;
    obj->state = jump_orbit_start_jump_arc_a;
    return 1;
}

s32 jump_orbit_setup_list_slot_b(struct Actor *obj)
{
    u8 *list;
    s32 start;
    s32 (*fn)(s32);

    obj->anim = gAnim278;
    list = gParty;
    clear_switches(obj);
    obj->data->slotValue = ((struct ListEntry *)list)->value;
    obj->state = jump_orbit_start_jump_arc_b;
    fn = (s32 (*)(s32))gMathRoutines;
    start = fn(0x006E1E40);
    obj->dh = (start - 0x58) / 2;
    return 1;
}

void jump_orbit_start_jump_arc_a(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(s32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(s32))gMathRoutines;
    start = fn(0x005A1440);
    obj->dh = (start - 0x48) / 2;
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = jump_orbit_arc_rise_a;
}

void jump_orbit_start_jump_arc_b(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = jump_orbit_arc_rise_b;
}

void jump_orbit_arc_rise_a(struct Actor *obj)
{
    u16 *dhp;
    s32 speed;
    s32 h;

    dhp = (u16 *)&obj->dh;
    speed = obj->speed;
    obj->dh = *dhp + speed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (obj->dh > 0)
        return;
    obj->state = jump_orbit_arc_land_a;
    if (try_park_in_slot(obj) == 0)
    {
        sprite_set_action(obj, 6, 0x204D, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        ((struct ObjFlagBits *)&obj->flags)->phase = 1;
    }
}

void jump_orbit_arc_rise_b(struct Actor *obj)
{
    u16 *dhp;
    s32 speed;
    s32 h;

    dhp = (u16 *)&obj->dh;
    speed = obj->speed;
    obj->dh = *dhp + speed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (obj->dh > 0)
        return;
    obj->state = jump_orbit_arc_land_b;
    if (try_park_in_slot(obj) == 0)
    {
        sprite_set_action(obj, 6, 0x2000, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        ((struct ObjFlagBits *)&obj->flags)->phase = 1;
    }
}

void jump_orbit_arc_land_a(struct Actor *obj)
{
    u16 *dhp;
    s32 speed;
    s32 h;

    dhp = (u16 *)&obj->dh;
    speed = obj->speed;
    *dhp = *dhp + speed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        ((struct ObjFlagBits *)&obj->flags)->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = jump_orbit_finish_if_pose_visible_a;
    }
}

void jump_orbit_arc_land_b(struct Actor *obj)
{
    u16 *dhp;
    s32 speed;
    s32 h;

    dhp = (u16 *)&obj->dh;
    speed = obj->speed;
    *dhp = *dhp + speed;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        ((struct ObjFlagBits *)&obj->flags)->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = jump_orbit_finish_if_pose_visible_b;
    }
}

void jump_orbit_finish_if_pose_visible_a(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void jump_orbit_finish_if_pose_visible_b(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

// Starts the jump orbit move: spawns the orbiting child at obj's base point (in whole pixels),
// in action mode 3 with its box bytes (unk44-unk49: -10, 8, -10, 10, -3, 3), phase 2, aiming at
// obj's base X and its own base height, with animation gAnim284 and on_arrive as its callback.
// Then resets the step, clears slot value 0x7C and runs the first wind-up frame (jump_orbit_step_a).
void jump_orbit_spawn_orbit_child_a(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, 0, 1);
    obj->child = child;
    *(u8 *)&child->action |= ACT_MODE_MASK; // ActBits.mode = 3
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xFD;
    child->unk49 = 3;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    *(u8 *)&child->action |= 0x40;
    child->callback = on_arrive;
    *(u8 *)&child->flags79 |= 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    jump_orbit_step_a(obj);
}

/* Wind-up of jump orbit a while the button (keyMask) is held: steps animation 0x2025 through frames
 * 0-3 every 5 frames with the pose held. Fully wound up (20 frames) it takes action 1 and waits in
 * jump_orbit_hold_a; let go earlier, step becomes frames * 4 and jump_orbit_step_b winds it back
 * down. An interrupted action ends it at once. */
void jump_orbit_step_a(struct Actor *obj)
{
    s16 *windup;
    s32 kind;

    obj->state = jump_orbit_step_a;
    kind = obj->action & ACT_KIND_MASK;
    if (kind != 0)
    {
        end_action(obj);
        return;
    }
    windup = (s16 *)&obj->step;
    if ((s16)*windup == 0)
        sprite_set_action(obj, 0, 0x2025, 0);
    if ((s16)*windup == 5)
        sprite_set_action(obj, 0, 0x2025, 1);
    if ((s16)*windup == 10)
        sprite_set_action(obj, 0, 0x2025, 2);
    if ((s16)*windup == 15)
        sprite_set_action(obj, 0, 0x2025, 3);
    if ((s16)*windup != 20)
    {
        if ((gSys.keysHeld & obj->keyMask) == 0)
        {
            *windup = (s16)*windup * 4;
            obj->state = jump_orbit_step_b;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        *windup = kind; /* 0: jump_orbit_hold_a counts hold frames from here */
        obj->state = jump_orbit_hold_a;
    }
    *(u8 *)&obj->pose->flags |= POSE_HOLD_MASK;
    obj->step = obj->step + 1;
}

/* The button was let go while jump_orbit_step_a was charging (step = charge * 4): winds the charge
 * back down by 5 a frame, stepping animation 0x2025 back to frame 2, 1 and 0 as step falls below
 * 0x3C, 0x28 and 0x14, with the pose held. Once step is below 0 it takes action 4 (pose result
 * 0xC0 / 8), lets the pose go and ends in jump_orbit_end_action_if_pose_visible. An interrupted
 * action ends it at once. */
void jump_orbit_step_b(struct Actor *obj)
{
    s16 *step;

    obj->state = jump_orbit_step_b;
    if (obj->action & ACT_KIND_MASK)
    {
        end_action(obj);
        return;
    }
    step = (s16 *)&obj->step;
    if ((s16)*step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 8);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = jump_orbit_end_action_if_pose_visible;
        return;
    }
    if ((s16)*step <= 0x13)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2025, 0);
    }
    else if ((s16)*step <= 0x27)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2025, 1);
    }
    else if ((s16)*step <= 0x3B)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2025, 2);
    }
    *(u8 *)&obj->pose->flags |= 0x10; /* PoseFlagBits.hold */
    obj->step = obj->step - 5;
}

/* Waits while the button (keyMask) is held. On release it starts jump orbit a: the child starts
 * half a turn round at angle speed -0x1CC and obj goes on to jump_orbit_circle_a. Held for more
 * than 0x17 frames the move fails: kind 0x1E waits there unless world byte 9 has 1 in its upper
 * nibble (then action 4 with pose result 0xC0 / 8), any other kind takes action 5. Past 0x0B
 * frames the pose's hold bit is cleared. */
void jump_orbit_hold_a(struct Actor *obj)
{
    u32 held;
    s16 *holdFrames;

    if (obj->action & ACT_KIND_MASK)
    {
        end_action(obj);
        return;
    }
    held = gSys.keysHeld & obj->keyMask;
    if (held == 0)
    {
        obj->timer = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = -0x1CC;
        sprite_set_action(obj, 2, 0, 0);
        *(u8 *)&obj->pose->flags |= 0x10; // PoseFlagBits.hold
        obj->state = jump_orbit_circle_a;
        obj->step = held;
        return;
    }
    holdFrames = (s16 *)&obj->step;
    if ((s16)*holdFrames > 0x17)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 8);
            }
            else
            {
                obj->step = 0x18;
                return;
            }
        }
        else
            sprite_set_action(obj, 5, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = jump_orbit_end_action_if_pose_visible;
        return;
    }
    if ((s16)*holdFrames > 0x0B)
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->step = obj->step + 1;
}

/* One frame of jump orbit a: swings the child around a circle of radius 22 above obj's base
 * point (child angle unkA0 += its speed unkA4, the speed bent by 0xC00 towards obj's facing),
 * steps obj's animation 0x2025 through frames 1-5, and at step 7 releases the child (action
 * mode 3), locks obj's data and goes on to jump_orbit_end_a. */
void jump_orbit_circle_a(struct Actor *obj)
{
    struct Actor *child;
    const s16 *cosTable;
    s16 *stepp;
    u32 tableIdx;
    s32 angle;
    s32 centerH;

    if (obj->action & ACT_KIND_MASK)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActBits *)&child->action)->mode = 1;
    child->unkA0 += child->unkA4;
    angle = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->unkA0, 360);
    cosTable = gCosineTable;
    tableIdx = angle & 0xFF;
    child->unk10 = obj->baseX + (cosTable[tableIdx] / 64) * 22;
    centerH = obj->baseH + 0x1600;
    child->h = centerH + (gSineTable[tableIdx] / 64) * 22;
    if (obj->pose->dir & 0x40)
        child->unkA4 += 0xC00;
    else
        child->unkA4 += -0xC00;
    stepp = (s16 *)&obj->step;
    if (*stepp == 2)
        sprite_set_action(obj, 2, 0x2025, 1);
    if (*stepp == 3)
        sprite_set_action(obj, 2, 0x2025, 2);
    if (*stepp == 4)
        sprite_set_action(obj, 2, 0x2025, 3);
    if (*stepp == 5)
        sprite_set_action(obj, 2, 0x2025, 4);
    if (*stepp == 6)
        sprite_set_action(obj, 2, 0x2025, 5);
    if (*stepp == 7)
    {
        *(u8 *)&child->action |= ACT_MODE_MASK; // ActBits.mode = 3: release the child
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = jump_orbit_end_a;
        if (obj->data != NULL)
            *(u8 *)&obj->data->lock |= 4; // DataLockBits.locked
        return;
    }
    *(u8 *)&obj->pose->flags |= 0x10; // PoseFlagBits.hold
    obj->step = obj->step + 1;
}

/* Start of jump orbit b: spawns the child the actor holds at its base position (action mode 3, hit
 * box x -10/8, y -10/10, h -3/3, target x/h = unk84/unk8C, animation gAnim284, on_arrive when it
 * lands), plays sound 0x7C and runs the wind-up, jump_orbit_step_c, from step 0. */
void jump_orbit_spawn_orbit_child_b(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, 0, 1);
    obj->child = child;
    *(u8 *)&child->action |= ACT_MODE_MASK; // ActBits.mode = 3
    child->unk44 = -10;
    child->unk45 = 8;
    child->unk46 = -10;
    child->unk47 = 10;
    child->unk48 = -3;
    child->unk49 = 3;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    *(u8 *)&child->action |= 0x40;
    child->callback = on_arrive;
    *(u8 *)&child->flags79 |= 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    jump_orbit_step_c(obj);
}

/* Wind-up of jump orbit b while the button (keyMask) is held: steps animation 0x2054 through frames
 * 0-3 every 8 frames with the pose held. Fully wound up (32 frames) it takes action 1 and waits in
 * jump_orbit_hold_b; let go earlier, step becomes frames * 6 and jump_orbit_step_d winds it back
 * down. An interrupted action ends it at once. */
void jump_orbit_step_c(struct Actor *obj)
{
    s16 *windup;
    s32 kind;

    obj->state = jump_orbit_step_c;
    kind = obj->action & ACT_KIND_MASK;
    if (kind != 0)
    {
        end_action(obj);
        return;
    }
    windup = (s16 *)&obj->step;
    if ((s16)*windup == 0)
        sprite_set_action(obj, 0, 0x2054, 0);
    if ((s16)*windup == 8)
        sprite_set_action(obj, 0, 0x2054, 1);
    if ((s16)*windup == 16)
        sprite_set_action(obj, 0, 0x2054, 2);
    if ((s16)*windup == 24)
        sprite_set_action(obj, 0, 0x2054, 3);
    if ((s16)*windup != 32)
    {
        if ((gSys.keysHeld & obj->keyMask) == 0)
        {
            *windup = (s16)*windup * 6;
            obj->state = jump_orbit_step_d;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        *windup = kind; /* 0: jump_orbit_hold_b counts hold frames from here */
        obj->state = jump_orbit_hold_b;
    }
    *(u8 *)&obj->pose->flags |= POSE_HOLD_MASK;
    obj->step = obj->step + 1;
}

/* Let-go state of jump_orbit_step_c's wind-up (step = wind-up frames * 6): plays animation 0x2054
   back down, frame 2, 1 then 0 (each only if pose->count has it), as step counts down by 8 a
   frame. Past 0 the move is cancelled with action 4 (pose result 0xC0 / 12) and
   jump_orbit_end_action_if_pose_visible takes over. An action kind (action bits 3-5) ends it. */
void jump_orbit_step_d(struct Actor *obj)
{
    s16 *pStep;

    obj->state = jump_orbit_step_d;
    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    pStep = (s16 *)&obj->step;
    if ((s16)*pStep < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 0xC);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = jump_orbit_end_action_if_pose_visible;
        return;
    }
    if ((s16)*pStep <= 0x2F)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2054, 0);
    }
    else if ((s16)*pStep <= 0x5F)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2054, 1);
    }
    else if ((s16)*pStep <= 0x8F)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2054, 2);
    }
    *(u8 *)&obj->pose->flags |= 0x10; /* PoseFlagBits.hold */
    obj->step = obj->step - 8;
}

/* Waits while the button (keyMask) is held. On release it starts jump orbit b: the child starts
 * half a turn round at angle speed 0xA00 and obj goes on to jump_orbit_circle_b. Held for more
 * than 0x15 frames the move fails: kind 0x1E waits there unless world byte 9 has 1 in its upper
 * nibble (then action 4 with pose result 0xC0 / 12), any other kind takes action 5. Past 0x0E
 * frames the pose's hold bit is cleared. */
void jump_orbit_hold_b(struct Actor *obj)
{
    u32 held;
    s16 *holdFrames;

    if (obj->action & ACT_KIND_MASK)
    {
        end_action(obj);
        return;
    }
    held = gSys.keysHeld & obj->keyMask;
    if (held == 0)
    {
        obj->timer = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        *(u8 *)&obj->pose->flags |= 0x10; // PoseFlagBits.hold
        obj->state = jump_orbit_circle_b;
        obj->step = held;
        return;
    }
    holdFrames = (s16 *)&obj->step;
    if ((s16)*holdFrames > 0x15)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 0xC);
            }
            else
            {
                obj->step = 0x16;
                return;
            }
        }
        else
            sprite_set_action(obj, 5, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = jump_orbit_end_action_if_pose_visible;
        return;
    }
    if ((s16)*holdFrames > 0x0E)
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->step = obj->step + 1;
}


/* One frame of orbit b: swings the child around a circle of radius 24 above obj's base point
 * (angle += angle speed, the speed bent towards obj's facing), steps obj's animation 0x2054
 * through frames 1-5, and at step 9 releases the child and goes on to jump_orbit_end_b. */
void jump_orbit_circle_b(struct Actor *obj)
{
    struct Actor *child;
    const s16 *cosTable;
    s16 *stepp;
    u32 tableIdx;
    s32 angleIdx;
    s32 centerH;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActBits *)&child->action)->mode = 1;
    child->unkA0 += child->unkA4;
    angleIdx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->unkA0, 360);
    cosTable = gCosineTable;
    tableIdx = angleIdx & 0xFF;
    child->unk10 = obj->baseX + (cosTable[tableIdx] / 64) * 24;
    centerH = obj->baseH + 0x1A00;
    child->h = centerH + (gSineTable[tableIdx] / 64) * 24;
    if (obj->pose->dir & 0x40)
        child->unkA4 += 0x998;
    else
        child->unkA4 += -0x998;
    stepp = (s16 *)&obj->step;
    if (*stepp == 3)
        sprite_set_action(obj, 2, 0x2054, 1);
    if (*stepp == 5)
        sprite_set_action(obj, 2, 0x2054, 2);
    if (*stepp == 6)
        sprite_set_action(obj, 2, 0x2054, 3);
    if (*stepp == 7)
        sprite_set_action(obj, 2, 0x2054, 4);
    if (*stepp == 8)
        sprite_set_action(obj, 2, 0x2054, 5);
    if (*stepp == 9)
    {
        *(u8 *)&child->action |= 6; /* ActBits.mode = 3: released */
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = jump_orbit_end_b;
        if (obj->data != NULL)
            *(u8 *)&obj->data->lock |= 4; /* DataLockBits.locked = 1 */
        return;
    }
    *(u8 *)&obj->pose->flags |= 0x10; /* PoseFlagBits.hold = 1 */
    obj->step = obj->step + 1;
}


void jump_orbit_end_b(struct Actor *obj)
{
    u16 step;

    if (obj->data != NULL)
        ((struct DataLockBits *)&obj->data->lock)->locked = 0;
    if (obj->action & 0x38)
        end_action(obj);
    else
    {
        step = obj->step;
        obj->step = step + 1;
        if ((s16)step > 0x17)
            end_action(obj);
    }
}

void jump_orbit_end_a(struct Actor *obj)
{
    u16 step;

    if (obj->data != NULL)
        ((struct DataLockBits *)&obj->data->lock)->locked = 0;
    if (obj->action & 0x38)
        end_action(obj);
    else
    {
        step = obj->step;
        obj->step = step + 1;
        if ((s16)step > 0x13)
            end_action(obj);
    }
}

void jump_orbit_end_action_if_pose_visible(struct Actor *obj)
{
    if (obj->action & 0x38)
        end_action(obj);
    else if (obj->pose->flags & 8)
        end_action(obj);
}

s32 jump_orbit_setup_slot_spawn(struct Actor *obj)
{
    u32 *value;
    u8 *list;
    struct ListEntry *entry;

    clear_switches(obj);
    value = &obj->data->slotValue;
    list = gParty;
    entry = (struct ListEntry *)(list + (~obj->data->slot) * 0x3C);
    *value = entry->value2;
    obj->state = jump_orbit_dispatch_slot_spawn;
    obj->callback = end_action;
    return 1;
}

s32 jump_orbit_dispatch_slot_pause_jump(struct Actor *obj)
{
    s16 slot = obj->data->slot;

    if (slot == -1)
        pause_jump_a(obj);
    else if (slot == -2)
        pause_jump_b(obj);
    obj->callback = on_arrive;
    return 1;
}

void jump_orbit_dispatch_slot_spawn(struct Actor *obj)
{
    if (obj->data->slot == -1)
        jump_orbit_spawn_orbit_child_a(obj);
    else
        jump_orbit_spawn_orbit_child_b(obj);
}

s32 pause_jump_b(struct Actor *obj)
{
    struct ListEntry *entry;

    obj->anim = gAnim374;
    entry = (struct ListEntry *)(gParty + 0x3C);
    clear_switches(obj);
    obj->data->slotValue = entry->value;
    sprite_set_action(obj, 4, 0x204D, 0);
    obj->pose->result = 0x30;
    obj->timer = 2;
    obj->state = jump_orbit_jump_timer_arc_a;
    return 1;
}

s32 pause_jump_a(struct Actor *obj)
{
    struct ListEntry *entry;

    obj->anim = gAnim278;
    entry = (struct ListEntry *)gParty;
    clear_switches(obj);
    obj->data->slotValue = entry->value;
    sprite_set_action(obj, 4, 0x2000, 0);
    obj->pose->result = 0x30;
    obj->timer = 2;
    obj->state = jump_orbit_jump_timer_arc_b;
    return 1;
}

void jump_orbit_jump_timer_arc_a(struct Actor *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if (timer <= 0)
    {
        obj->pose->result = 0x10;
        jump_orbit_start_jump_arc_c(obj);
    }
}

void jump_orbit_jump_timer_arc_b(struct Actor *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if (timer <= 0)
    {
        obj->pose->result = 0x10;
        jump_orbit_start_jump_arc_d(obj);
    }
}

void jump_orbit_start_jump_arc_c(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(s32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(s32))gMathRoutines;
    start = fn(0x00B45100);
    obj->dh = (start - 0x90) / 2;
    obj->speed = -0x90;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = jump_orbit_arc_rise_c;
}

void jump_orbit_start_jump_arc_d(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(s32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(s32))gMathRoutines;
    start = fn(0x00DC7900);
    obj->dh = (start - 0xB0) / 2;
    obj->speed = -0xB0;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = jump_orbit_arc_rise_d;
}
