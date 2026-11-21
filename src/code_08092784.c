#include "global.h"
#include "ram_routines.h"

struct PoseBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct Pose
{
    u8 filler0[0x11];
    u8 turn;
    u8 flags;
    u8 filler13[0x0D];
    u8 result;
    u8 filler21[0x02];
    u8 count;
};

struct ObjFlagBits
{
    u8 phase : 3;
    u8 unk3 : 2;
    u8 switchA : 1;
    u8 switchB : 1;
    u8 unk7 : 1;
};

struct DataLockBits
{
    u8 unk0 : 2;
    u8 locked : 1;
    u8 unk3 : 5;
};

struct Data
{
    u8 filler0[0xE0];
    s32 ground;
    u8 fillerE4[0x08];
    s16 slot;
    u8 fillerEE[0x23];
    u8 lock;
    u8 fillerLock[0x02];
    u32 slotValue;
    u8 filler118[0x15];
    u8 kind;
};

struct Obj
{
    u8 filler0[0x08];
    struct Pose *pose;
    u8 filler0C[0x04];
    s32 x;
    u8 filler14[0x04];
    s32 h;
    u8 filler1C[0x0C];
    struct Data *data;
    u8 filler2C[0x04];
    struct Obj *child;
    u8 filler34[0x04];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 shape[0x06];
    u8 filler4A[0x02];
    void (*state)(struct Obj *);
    u8 filler50[0x18];
    u32 callback;
    u32 anim;
    u8 filler70[0x06];
    u8 action;
    u8 flags;
    u8 filler78;
    u8 flags79;
    u8 filler7A[0x0A];
    u32 target;
    u8 filler88[0x04];
    u32 target2;
    u8 filler90[0x04];
    s32 speed;
    u8 filler98[0x04];
    u32 timer;
    u32 angle;
    s32 velocity;
    u32 buttons;
    s16 step;
    u8 fillerAE[0x04];
    u16 dh;
};

extern u8 *gParty;
extern u8 *gWorld;
extern u8 gAnim278[];
extern u8 gAnim374[];
extern u8 gAnim284[];
struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gSys;

u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void sprite_set_action(struct Obj *obj, u32 action, u32 part, u32 extra);
void set_slot_value(u32 id, u32 value);
void end_action(struct Obj *obj);
void on_arrive(void);
u8 try_park_in_slot(struct Obj *obj);
struct Obj *obj_spawn(struct Data *data, u32 kind, s32 x, s32 y, s32 z, u32 extra, u32 extra2);
void hold_decay_hold_step_a(struct Obj *obj);

void hold_orbit_arc_land_e(struct Obj *obj);
void finish_if_pose_visible_g(struct Obj *obj);
void finish_if_pose_visible_h(struct Obj *obj);
s32 hold_orbit_random_pause_jump_a(struct Obj *obj);
s32 hold_orbit_random_pause_jump_b(struct Obj *obj);
void hold_orbit_spawn_orbit_child_b(struct Obj *obj);
void hold_charge_a(struct Obj *obj);
void charge_release_hold_release_a(struct Obj *obj);
void charge_release_hold_idle_a(struct Obj *obj);
void charge_release_orbit_circle_a(struct Obj *obj);
void charge_release_spawn_orbit_child_a(struct Obj *obj);
void charge_release_hold_step_a(struct Obj *obj);
void hold_release_b(struct Obj *obj);
void charge_release_hold_idle_b(struct Obj *obj);
void charge_release_orbit_circle_b(struct Obj *obj);
void step_end_action_a(struct Obj *obj);
void charge_release_timer_start_orbit_b(struct Obj *obj);
void step_end_action_b(struct Obj *obj);
void charge_release_timer_start_orbit_a(struct Obj *obj);
void charge_release_end_action_if_busy(struct Obj *obj);
s32 charge_release_setup_slot_state(struct Obj *obj);
void charge_release_start_jump_arc_a(struct Obj *obj);
void timer_start_jump_a(struct Obj *obj);
void charge_release_start_jump_arc_b(struct Obj *obj);
void timer_start_jump_b(struct Obj *obj);
s32 charge_release_dispatch_slot_random_jump(struct Obj *obj);
void charge_release_dispatch_slot_spawn(struct Obj *obj);
void charge_release_arc_rise_a(struct Obj *obj);
void charge_release_arc_rise_b(struct Obj *obj);
void charge_release_arc_land_a(struct Obj *obj);
void charge_release_arc_land_b(struct Obj *obj);
void charge_release_finish_if_pose_visible_a(struct Obj *obj);
void charge_release_finish_if_pose_visible_b(struct Obj *obj);
void charge_release_spawn_orbit_child_c(struct Obj *obj);
void hold_charge_b(struct Obj *obj);
void hold_release_c(struct Obj *obj);
void hold_decay_hold_idle_a(struct Obj *obj);
void hold_decay_orbit_step_a(struct Obj *obj);
void hold_decay_spawn_orbit_child(struct Obj *obj);
void hold_decay_end_action_if_busy(struct Obj *obj);
void hold_decay_timer_start_orbit_a(struct Obj *obj);
void step_end_action_19(struct Obj *obj);

void hold_orbit_arc_land_e(struct Obj *obj)
{
    s32 h;
    struct Pose *pose;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        ((struct ObjFlagBits *)&obj->flags)->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        pose = obj->pose;
        ((struct PoseBits *)&pose->flags)->mode = 1;
        obj->state = finish_if_pose_visible_h;
    }
}

void finish_if_pose_visible_g(struct Obj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void finish_if_pose_visible_h(struct Obj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

/* Starts the jump of the hero in slot -1 (party entry 0; _b is entry 1): copies the party entry's
   u16 at 0x2A into data->slotValue, then waits a random 8-15 frames in action 4
   (timer_start_jump_b) before charge_release_start_jump_arc_b jumps off at speed -0x58. dh gets
   (sqrt(0x6E1E40) - 0x58) / 2, using the RAM copy of the square root routine. */
s32 hold_orbit_random_pause_jump_a(struct Obj *obj)
{
    struct Pose *pose;
    struct Data *data;
    u8 *party;
    u32 delay;
    void (*next)(struct Obj *);
    u32 (*sqrt)(u32);
    s32 root;

    obj->anim = (u32)gAnim278;
    party = gParty; /* entry 0 */
    ((struct ObjFlagBits *)&obj->flags)->switchB = 0;
    ((struct ObjFlagBits *)&obj->flags)->switchA = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(party + 0x2A);
    delay = rand_below(8, mt19937_next()) + 8;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        pose = obj->pose;
        ((struct PoseBits *)&pose->flags)->mode = 0;
        obj->timer = delay;
        next = timer_start_jump_b;
    }
    else
    {
        next = charge_release_start_jump_arc_b;
    }
    obj->state = next;
    sqrt = (u32 (*)(u32))gMathRoutines;
    root = sqrt(0x006E1E40);
    obj->dh = (root - 0x58) / 2;
    return 1;
}

/* Starts the jump of the hero in slot -2 (party entry 1; _a is entry 0): copies the party entry's
   u16 at 0x2A into data->slotValue, then waits a random 10-31 frames in action 4
   (timer_start_jump_a) before charge_release_start_jump_arc_a jumps off at speed -0x48. dh gets
   (sqrt(0x5A1440) - 0x48) / 2, using the RAM copy of the square root routine. */
s32 hold_orbit_random_pause_jump_b(struct Obj *obj)
{
    struct Pose *pose;
    struct Data *data;
    u8 *party;
    u32 delay;
    void (*next)(struct Obj *);
    u32 (*sqrt)(u32);
    s32 root;

    obj->anim = (u32)gAnim374;
    party = gParty;
    party += 0x3C; /* entry 1 */
    ((struct ObjFlagBits *)&obj->flags)->switchB = 0;
    ((struct ObjFlagBits *)&obj->flags)->switchA = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(party + 0x2A);
    delay = rand_below(0x16, mt19937_next()) + 0xA;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        pose = obj->pose;
        ((struct PoseBits *)&pose->flags)->mode = 0;
        obj->timer = delay;
        next = timer_start_jump_a;
    }
    else
    {
        next = charge_release_start_jump_arc_a;
    }
    obj->state = next;
    sqrt = (u32 (*)(u32))gMathRoutines;
    root = sqrt(0x005A1440);
    obj->dh = (root - 0x48) / 2;
    return 1;
}

/* Start of hold orbit b: spawns the child the actor holds at its base position in pixels (action
 * mode 3, hit box x -10/8, y -10/10, h -12/4, animation gAnim284, on_arrive when it lands), plays
 * sound 0x7C and runs the charge wind-up, hold_charge_a, from step 0. */
void hold_orbit_spawn_orbit_child_b(struct Obj *obj)
{
    struct Obj *child;
    struct Data *data;
    s32 x;
    s32 y;
    s32 h;
    u8 *flags;

    data = obj->data;
    x = obj->baseX / 256;
    y = obj->baseY / 256;
    h = obj->baseH / 256;
    child = obj_spawn(data, 0, x, y, h, 0, 1);
    obj->child = child;
    flags = &child->action;
    *flags = *flags | 6; /* action mode 3 */
    child->shape[0] = -10;
    child->shape[1] = 8;
    child->shape[2] = -10;
    child->shape[3] = 10;
    child->shape[4] = -12;
    child->shape[5] = 4;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->target = obj->baseX;
    child->target2 = child->baseH;
    child->anim = (u32)gAnim284;
    *flags = *flags | 0x40;
    child->callback = (u32)on_arrive;
    flags = &child->flags79;
    *flags = *flags | 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    hold_charge_a(obj);
}

/* The pose (animated sprite) flags at +0x12, read by sprite_step_e6a0. */
struct PoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;        /* at the animation's end: 0 loop, 1 hold the last frame, 2 stop */
    u8 bit3 : 1;        /* set when the animation has reached its end */
    u8 busy : 1;        /* freezes the animation on its current frame */
    u8 rest : 3;
};

/* obj->action bits that interrupt a move in progress (it then calls end_action) */
#define OBJ_ACTION_INTERRUPT 0x38

/* sprite_set_action animation of the charge wind-up (frames 0-3) */
#define ANIM_CHARGE_WINDUP 0x2025

/*
 * Charge a, winding up: while the attack's buttons stay held, shows frames 0-3 of sprite 0x2025
 * (one every 2 frames, the pose frozen on it). At step 8 it is fully charged: action 1 loops and
 * charge_release_hold_idle_a takes over. Let go earlier: step is doubled and
 * charge_release_hold_release_a rewinds the charge. Interrupt bits end the action.
 */
void hold_charge_a(struct Obj *obj)
{
    u32 interrupt;

    obj->state = hold_charge_a;
    interrupt = obj->action & OBJ_ACTION_INTERRUPT;
    if (interrupt != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
        sprite_set_action(obj, 0, 0x2025, 0);
    if (obj->step == 2)
        sprite_set_action(obj, 0, 0x2025, 1);
    if (obj->step == 4)
        sprite_set_action(obj, 0, 0x2025, 2);
    if (obj->step == 6)
        sprite_set_action(obj, 0, 0x2025, 3);
    if (obj->step != 8)
    {
        if ((gSys.held & obj->buttons) == 0)
        {
            obj->step *= 2;
            obj->state = charge_release_hold_release_a;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0; /* loop */
        obj->step = interrupt; /* 0: counts the frames held from now on */
        obj->state = charge_release_hold_idle_a;
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step++;
}

/*
 * The button was let go before the charge finished (hold_charge_a doubled step): counts step
 * down by 2 a frame, rewinding the charge animation through frames 2, 1, 0. Once step drops
 * below 0, plays action 4 once (held on its last frame) and ends the action when it is done
 * (charge_release_end_action_if_busy).
 */
void charge_release_hold_release_a(struct Obj *obj)
{
    u32 action;

    obj->state = charge_release_hold_release_a;
    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 8);
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = charge_release_end_action_if_busy;
        return;
    }
    if (obj->step <= 3)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2025, 0);
    }
    else if (obj->step <= 7)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2025, 1);
    }
    else if (obj->step <= 11)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2025, 2);
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step -= 2;
}

/* Fully charged (after hold_charge_a), button still held; step counts the frames held. Interrupted
   (action bits 0x38): ends the action. Button released: waits 10-14 frames with action 6, then
   charge_release_timer_start_orbit_a swings the child. Held past 49 frames: kind 0x1E plays
   action 4 (result 0xC0 / 8) only while gWorld[9]'s high nibble is 1 and otherwise keeps waiting;
   other kinds play action 5. Either way it ends with charge_release_end_action_if_busy. The pose's
   busy bit is cleared after 25 frames held. */
void charge_release_hold_idle_a(struct Obj *obj)
{
    u32 action;
    u32 held;

    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    held = gSys.held & obj->buttons;
    if (held == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 0xA;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->state = charge_release_timer_start_orbit_a;
        obj->step = held;
        return;
    }
    if (obj->step > 0x31)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 8);
            }
            else
            {
                obj->step = 0x32;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = charge_release_end_action_if_busy;
    }
    else
    {
        if (obj->step > 0x18)
            ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->step++;
    }
}

/* One frame of orbit a: swings the child around a circle of radius 22 above obj's base point
 * (angle += velocity, velocity bent towards obj's facing), steps obj's animation 0x2025 through
 * frames 1-5, and at step 7 releases the child and goes on to step_end_action_b. */
void charge_release_orbit_circle_a(struct Obj *obj)
{
    struct Obj *child;
    u32 tableIdx;
    s32 centerH;
    const s16 *cosTable;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct PoseBits *)&child->action)->mode = 1;
    child->angle += child->velocity;
    tableIdx = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(child->angle, 360);
    cosTable = gCosineTable;
    child->x = obj->baseX + (cosTable[tableIdx & 0xFF] / 64) * 22;
    centerH = obj->baseH + 0x1600;
    child->h = centerH + (gSineTable[tableIdx & 0xFF] / 64) * 22;
    if ((obj->pose->turn & 0x40) != 0)
        child->velocity += 0xC00;
    else
        child->velocity -= 0xC00;
    if (obj->step == 2)
        sprite_set_action(obj, 2, 0x2025, 1);
    if (obj->step == 3)
        sprite_set_action(obj, 2, 0x2025, 2);
    if (obj->step == 4)
        sprite_set_action(obj, 2, 0x2025, 3);
    if (obj->step == 5)
        sprite_set_action(obj, 2, 0x2025, 4);
    if (obj->step == 6)
        sprite_set_action(obj, 2, 0x2025, 5);
    if (obj->step == 7)
    {
        ((struct PoseBits *)&child->action)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->step = 0;
        obj->state = step_end_action_b;
        if (obj->data != 0)
            ((struct DataLockBits *)&obj->data->lock)->locked = 1;
    }
    else
    {
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
        obj->step++;
    }
}

// Starts the charge-release move: spawns the orbiting child at obj's base point (in whole
// pixels), in action mode 3 with its box bytes (shape: -10, 8, -10, 10, -12, 4), phase 2, aiming
// (target, target2) at obj's base X and its own base height, with animation gAnim284 and
// on_arrive as its callback. Then resets the step, clears slot value 0x7C and runs the first
// wind-up frame (charge_release_hold_step_a).
void charge_release_spawn_orbit_child_a(struct Obj *obj)
{
    struct Obj *child;
    u8 *act;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    act = &child->action;
    *act = *act | 6; // action mode 3
    child->shape[0] = 0xF6;
    child->shape[1] = 8;
    child->shape[2] = 0xF6;
    child->shape[3] = 0xA;
    child->shape[4] = 0xF4;
    child->shape[5] = 4;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->target = obj->baseX;
    child->target2 = child->baseH;
    child->anim = (u32)gAnim284;
    *act = *act | 0x40;
    child->callback = (u32)on_arrive;
    act = &child->flags79;
    *act = *act | 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    charge_release_hold_step_a(obj);
}

/* Charge b, winding up while the actor's buttons are held: steps animation 0x2054 through frames
 * 0-3 every 3 frames. Let go early: step becomes frames held * 3 and hold_release_b rewinds it.
 * Fully wound (step 12): plays action 1 looping and goes on to charge_release_hold_idle_b.
 * Action bits OBJ_ACTION_INTERRUPT end the move. */
void charge_release_hold_step_a(struct Obj *obj)
{
    u32 interrupt;

    obj->state = charge_release_hold_step_a;
    interrupt = obj->action & OBJ_ACTION_INTERRUPT;
    if (interrupt != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
        sprite_set_action(obj, 0, 0x2054, 0);
    if (obj->step == 3)
        sprite_set_action(obj, 0, 0x2054, 1);
    if (obj->step == 6)
        sprite_set_action(obj, 0, 0x2054, 2);
    if (obj->step == 9)
        sprite_set_action(obj, 0, 0x2054, 3);
    if (obj->step != 12)
    {
        if ((gSys.held & obj->buttons) == 0)
        {
            obj->step *= 3;
            obj->state = hold_release_b;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->step = interrupt; /* 0 */
        obj->state = charge_release_hold_idle_b;
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step++;
}

/* Charge b let go before it was fully wound up (charge_release_hold_step_a sets step = frames
 * held * 3): plays animation 0x2054's wind-up frames 2, 1, 0 back in reverse, 3 steps per frame,
 * while the pose has that many frames. Once step drops below 0, plays action 4 (result 0xC0 / 12)
 * and ends with charge_release_end_action_if_busy. Action bits 0x38 interrupt it. */
void hold_release_b(struct Obj *obj)
{
    u32 interrupt;

    obj->state = hold_release_b;
    interrupt = obj->action & 0x38;
    if (interrupt != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 12);
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = charge_release_end_action_if_busy;
        return;
    }
    if (obj->step <= 8)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2054, 0);
    }
    else if (obj->step <= 17)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2054, 1);
    }
    else if (obj->step <= 26)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2054, 2);
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step -= 3;
}

/* Charge b, holding: while the actor's buttons stay held, counts obj->step up (the pose stops
 * being busy after step 0x1D). Letting go waits 10-14 frames in charge_release_timer_start_orbit_b.
 * Held past step 0x2C the move ends: data kind 0x1E waits until gWorld[9]'s high nibble is 1 and
 * then plays action 4 (result 0xC0 / 12), any other kind plays action 5. */
void charge_release_hold_idle_b(struct Obj *obj)
{
    u32 action;
    u32 held;

    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    held = gSys.held & obj->buttons;
    if (held == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 0xA;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->state = charge_release_timer_start_orbit_b;
        obj->step = held;
        return;
    }
    if (obj->step > 0x2C)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xC);
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
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = charge_release_end_action_if_busy;
    }
    else
    {
        if (obj->step > 0x1D)
            ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->step++;
    }
}

/* One frame of orbit b: swings the child around a circle of radius 24 above obj's base point
 * (angle += velocity, velocity bent towards obj's facing), steps obj's animation 0x2054 through
 * frames 1-5, and at step 9 releases the child and goes on to step_end_action_a. */
void charge_release_orbit_circle_b(struct Obj *obj)
{
    struct Obj *child;
    const s16 *cosTable;
    u32 tableIdx;
    s32 centerH;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct PoseBits *)&child->action)->mode = 1;
    child->angle += child->velocity;
    tableIdx = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(child->angle, 360);
    cosTable = gCosineTable;
    child->x = obj->baseX + (cosTable[tableIdx & 0xFF] / 64) * 24;
    centerH = obj->baseH + 0x1A00;
    child->h = centerH + (gSineTable[tableIdx & 0xFF] / 64) * 24;
    if ((obj->pose->turn & 0x40) != 0)
        child->velocity += 0x998;
    else
        child->velocity -= 0x998;
    if (obj->step == 3)
        sprite_set_action(obj, 2, 0x2054, 1);
    if (obj->step == 5)
        sprite_set_action(obj, 2, 0x2054, 2);
    if (obj->step == 6)
        sprite_set_action(obj, 2, 0x2054, 3);
    if (obj->step == 7)
        sprite_set_action(obj, 2, 0x2054, 4);
    if (obj->step == 8)
        sprite_set_action(obj, 2, 0x2054, 5);
    if (obj->step == 9)
    {
        ((struct PoseBits *)&child->action)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->step = 0;
        obj->state = step_end_action_a;
        if (obj->data != 0)
            ((struct DataLockBits *)&obj->data->lock)->locked = 1;
    }
    else
    {
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
        obj->step++;
    }
}

void step_end_action_a(struct Obj *obj)
{
    struct Data *data;
    u16 old;

    data = obj->data;
    if (data != 0)
    {
        ((struct DataLockBits *)&data->lock)->locked = 0;
    }
    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else
    {
        old = obj->step;
        obj->step = old + 1;
        if ((s16)old > 0x17)
        {
            end_action(obj);
        }
    }
}

void charge_release_timer_start_orbit_b(struct Obj *obj)
{
    struct Pose *pose;
    s32 flags;

    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else if ((s32)obj->timer > 0)
    {
        obj->timer--;
    }
    else
    {
        obj->child->angle = 0xB400;
        obj->child->velocity = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        pose = obj->pose;
        flags = pose->flags;
        flags |= 0x10;
        pose->flags = flags;
        obj->state = charge_release_orbit_circle_b;
    }
}

void step_end_action_b(struct Obj *obj)
{
    struct Data *data;
    u16 old;

    data = obj->data;
    if (data != 0)
    {
        ((struct DataLockBits *)&data->lock)->locked = 0;
    }
    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else
    {
        old = obj->step;
        obj->step = old + 1;
        if ((s16)old > 0x13)
        {
            end_action(obj);
        }
    }
}

void charge_release_timer_start_orbit_a(struct Obj *obj)
{
    struct Pose *pose;
    s32 flags;

    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else if ((s32)obj->timer > 0)
    {
        obj->timer--;
    }
    else
    {
        obj->child->angle = 0xB400;
        obj->child->velocity = -0x1CC;
        sprite_set_action(obj, 2, 0, 0);
        pose = obj->pose;
        flags = pose->flags;
        flags |= 0x10;
        pose->flags = flags;
        obj->state = charge_release_orbit_circle_a;
    }
}

void charge_release_end_action_if_busy(struct Obj *obj)
{
    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else if (obj->pose->flags & 8)
    {
        end_action(obj);
    }
}

s32 charge_release_setup_slot_state(struct Obj *obj)
{
    struct Data *data;
    u32 *slotValue;
    u8 *list;
    u8 *entry;
    s32 index;

    ((struct ObjFlagBits *)&obj->flags)->switchB = 0;
    ((struct ObjFlagBits *)&obj->flags)->switchA = 0;
    data = obj->data;
    slotValue = &data->slotValue;
    list = gParty;
    index = ~data->slot;
    entry = list + index * 0x3C;
    *slotValue = *(u16 *)(entry + 0x2C);
    obj->state = charge_release_dispatch_slot_spawn;
    obj->callback = (u32)end_action;
    return 1;
}

void charge_release_start_jump_arc_a(struct Obj *obj)
{
    struct Pose *pose;

    set_slot_value(0x2B, -1);
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    pose = obj->pose;
    ((struct PoseBits *)&pose->flags)->mode = 1;
    obj->state = charge_release_arc_rise_a;
}

void timer_start_jump_a(struct Obj *obj)
{
    s32 value;

    value = obj->timer - 1;
    obj->timer = value;
    if (value <= 0)
    {
        obj->pose->result = 0x10;
        charge_release_start_jump_arc_a(obj);
    }
}

void charge_release_start_jump_arc_b(struct Obj *obj)
{
    struct Pose *pose;

    set_slot_value(0x2B, -1);
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    pose = obj->pose;
    ((struct PoseBits *)&pose->flags)->mode = 1;
    obj->state = charge_release_arc_rise_b;
}

void timer_start_jump_b(struct Obj *obj)
{
    s32 value;

    value = obj->timer - 1;
    obj->timer = value;
    if (value <= 0)
    {
        obj->pose->result = 0x10;
        charge_release_start_jump_arc_b(obj);
    }
}

s32 charge_release_dispatch_slot_random_jump(struct Obj *obj)
{
    s32 slot;

    slot = obj->data->slot;
    if (slot == -1)
    {
        hold_orbit_random_pause_jump_a(obj);
    }
    else if (slot == -2)
    {
        hold_orbit_random_pause_jump_b(obj);
    }
    obj->callback = (u32)on_arrive;
    return 1;
}

void charge_release_dispatch_slot_spawn(struct Obj *obj)
{
    if (obj->data->slot == -1)
    {
        hold_orbit_spawn_orbit_child_b(obj);
    }
    else
    {
        charge_release_spawn_orbit_child_a(obj);
    }
}

void charge_release_arc_rise_a(struct Obj *obj)
{
    struct Pose *pose;

    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = charge_release_arc_land_a;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            pose = obj->pose;
            ((struct PoseBits *)&pose->flags)->mode = 1;
            ((struct ObjFlagBits *)&obj->flags)->phase = 1;
        }
    }
}

void charge_release_arc_rise_b(struct Obj *obj)
{
    struct Pose *pose;

    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = charge_release_arc_land_b;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            pose = obj->pose;
            ((struct PoseBits *)&pose->flags)->mode = 1;
            ((struct ObjFlagBits *)&obj->flags)->phase = 1;
        }
    }
}

void charge_release_arc_land_a(struct Obj *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        ((struct ObjFlagBits *)&obj->flags)->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        obj->state = charge_release_finish_if_pose_visible_a;
    }
}

void charge_release_arc_land_b(struct Obj *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        ((struct ObjFlagBits *)&obj->flags)->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        obj->state = charge_release_finish_if_pose_visible_b;
    }
}

void charge_release_finish_if_pose_visible_a(struct Obj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void charge_release_finish_if_pose_visible_b(struct Obj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

/* Start of charge-release orbit c: spawns the child the actor holds at its base position in pixels
 * (action mode 3, hit box x -10/8, y -10/10, h -12/4, animation gAnim284, on_arrive when it lands),
 * clears slot value 0x7C and runs the charge wind-up, hold_charge_b, from step 0. */
void charge_release_spawn_orbit_child_c(struct Obj *obj)
{
    struct Obj *child;
    struct Data *data;
    s32 x;
    s32 y;
    s32 h;
    u8 *flags;

    data = obj->data;
    x = obj->baseX / 256;
    y = obj->baseY / 256;
    h = obj->baseH / 256;
    child = obj_spawn(data, 0, x, y, h, 0, 1);
    obj->child = child;
    flags = &child->action;
    *flags = *flags | 6; /* action mode 3 */
    child->shape[0] = -10;
    child->shape[1] = 8;
    child->shape[2] = -10;
    child->shape[3] = 10;
    child->shape[4] = -12;
    child->shape[5] = 4;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->target = obj->baseX;
    child->target2 = child->baseH;
    child->anim = (u32)gAnim284;
    *flags = *flags | 0x40;
    child->callback = (u32)on_arrive;
    flags = &child->flags79;
    *flags = *flags | 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    hold_charge_b(obj);
}

/*
 * Charge b, winding up (same as hold_charge_a): while the attack's buttons stay held, shows frames
 * 0-3 of the wind-up animation, one every 2 frames, the pose frozen on it. At step 8 it is fully
 * charged: action 1 loops and hold_decay_hold_idle_a takes over. Let go earlier: step is doubled
 * and hold_release_c rewinds the charge. Interrupt bits end the action.
 */
void hold_charge_b(struct Obj *obj)
{
    u32 interrupt;

    obj->state = hold_charge_b;
    interrupt = obj->action & OBJ_ACTION_INTERRUPT;
    if (interrupt != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
        sprite_set_action(obj, 0, ANIM_CHARGE_WINDUP, 0);
    if (obj->step == 2)
        sprite_set_action(obj, 0, ANIM_CHARGE_WINDUP, 1);
    if (obj->step == 4)
        sprite_set_action(obj, 0, ANIM_CHARGE_WINDUP, 2);
    if (obj->step == 6)
        sprite_set_action(obj, 0, ANIM_CHARGE_WINDUP, 3);
    if (obj->step != 8)
    {
        if ((gSys.held & obj->buttons) == 0)
        {
            obj->step *= 2;
            obj->state = hold_release_c;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0; /* loop */
        obj->step = interrupt; /* 0: counts the frames held from now on */
        obj->state = hold_decay_hold_idle_a;
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step++;
}

/* Button let go during hold_charge_b's wind-up (which doubled step): plays the wind-up frames of
 * animation 0x2025 back to frame 0, two steps a frame, then cancels with action 4 (speed 0xC0 / 8)
 * and ends the action once that pose is done (hold_decay_end_action_if_busy). */
void hold_release_c(struct Obj *obj)
{
    u32 cancelBits;

    obj->state = hold_release_c;
    cancelBits = obj->action & 0x38;
    if (cancelBits != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 8);
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = hold_decay_end_action_if_busy;
        return;
    }
    if (obj->step <= 3)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2025, 0);
    }
    else if (obj->step <= 7)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2025, 1);
    }
    else if (obj->step <= 11)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2025, 2);
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step -= 2;
}

// Charge state after hold_charge_b: while the attack's button stays held, step counts up and the pose's
// busy flag goes off after 24 frames. Letting go waits a random 10..14 frames
// (hold_decay_timer_start_orbit_a) before the orbit starts. Held past 49 frames the charge fails
// (action 5); data kind 0x1E instead waits there until gWorld[9]'s high nibble is 1, then cancels with
// action 4.
void hold_decay_hold_idle_a(struct Obj *obj)
{
    u32 action;
    u32 held;

    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    held = gSys.held & obj->buttons;
    if (held == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 10;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->state = hold_decay_timer_start_orbit_a;
        obj->step = held;
        return;
    }
    if (obj->step > 49)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 8);
            }
            else
            {
                obj->step = 50;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = hold_decay_end_action_if_busy;
    }
    else
    {
        if (obj->step > 24)
            ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->step++;
    }
}

/* One frame of the hold-decay orbit: swings the child around a circle of radius 22 above obj's base
 * point (angle += velocity, velocity bent towards obj's facing), steps obj's animation 0x2025 through
 * frames 1-5, and at step 7 releases the child and goes on to step_end_action_19. Same as
 * charge_release_orbit_circle_a apart from the next state. */
void hold_decay_orbit_step_a(struct Obj *obj)
{
    struct Obj *child;
    const s16 *cosTable;
    u32 tableIdx;
    s32 centerH;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct PoseBits *)&child->action)->mode = 1;
    child->angle += child->velocity;
    tableIdx = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(child->angle, 360);
    cosTable = gCosineTable;
    child->x = obj->baseX + (cosTable[tableIdx & 0xFF] / 64) * 22;
    centerH = obj->baseH + 0x1600;
    child->h = centerH + (gSineTable[tableIdx & 0xFF] / 64) * 22;
    if ((obj->pose->turn & 0x40) != 0)
        child->velocity += 0xC00;
    else
        child->velocity -= 0xC00;
    if (obj->step == 2)
        sprite_set_action(obj, 2, 0x2025, 1);
    if (obj->step == 3)
        sprite_set_action(obj, 2, 0x2025, 2);
    if (obj->step == 4)
        sprite_set_action(obj, 2, 0x2025, 3);
    if (obj->step == 5)
        sprite_set_action(obj, 2, 0x2025, 4);
    if (obj->step == 6)
        sprite_set_action(obj, 2, 0x2025, 5);
    if (obj->step == 7)
    {
        ((struct PoseBits *)&child->action)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->step = 0;
        obj->state = step_end_action_19;
        if (obj->data != 0)
            ((struct DataLockBits *)&obj->data->lock)->locked = 1;
    }
    else
    {
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
        obj->step++;
    }
}

// Starts the hold-decay move: spawns the orbiting child at obj's base point (in whole pixels),
// in action mode 3 with its box bytes (shape: -10, 8, -10, 10, -12, 4), phase 2, aiming
// (target, target2) at obj's base X and its own base height, with animation gAnim284 and
// on_arrive as its callback. Then resets the step, clears slot value 0x7C and runs the first
// wind-up frame (hold_decay_hold_step_a).
void hold_decay_spawn_orbit_child(struct Obj *obj)
{
    struct Obj *child;
    u8 *act;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    act = &child->action;
    *act = *act | 6; // action mode 3
    child->shape[0] = 0xF6;
    child->shape[1] = 8;
    child->shape[2] = 0xF6;
    child->shape[3] = 0xA;
    child->shape[4] = 0xF4;
    child->shape[5] = 4;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->target = obj->baseX;
    child->target2 = child->baseH;
    child->anim = (u32)gAnim284;
    *act = *act | 0x40;
    child->callback = (u32)on_arrive;
    act = &child->flags79;
    *act = *act | 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    hold_decay_hold_step_a(obj);
}
