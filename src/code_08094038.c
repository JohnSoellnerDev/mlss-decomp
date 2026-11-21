#include "global.h"
#include "ram_routines.h"

/* The actor's animated sprite, stepped by sprite_step_e6a0. */
struct Pose
{
    u8 filler0[0x11];
    u8 dir;
    u8 flags;           /* see struct PoseMode */
    u8 filler13[0x20 - 0x13];
    u8 result;          /* animation speed per step */
    u8 filler21[0x23 - 0x21];
    u8 count;           /* current frame of the animation */
};

struct PoseMode
{
    u8 filler0[0x12];
    u8 unk12_0 : 1;
    u8 mode : 2;        /* at the animation's end: 0 loop, 1 hold the last frame, 2 stop */
    u8 unk12_3 : 1;     /* set when the animation has reached its end */
    u8 hold : 1;        /* freezes the animation on its current frame */
    u8 unk12_5 : 3;
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
    u8 fillerE4[0xEC - 0xE4];
    s16 slot;
    u8 fillerEE[0x111 - 0xEE];
    u8 lock;
    u8 filler112[0x114 - 0x112];
    u32 slotValue;
    u8 filler118[0x12D - 0x118];
    u8 kind;
};

struct ActionBits
{
    u8 filler0[0x76];
    u8 unk76_0 : 1;
    u8 mode : 2;
    u8 unk76_3 : 5;
};

struct Actor
{
    u8 filler0[0x8];
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    u8 filler14[0x18 - 0x14];
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct Data *data;
    u8 filler2C[0x30 - 0x2C];
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct Actor *);
    u8 filler50[0x68 - 0x50];
    void (*callback)(struct Actor *);
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 action;
    u8 phase : 3;
    u8 unk77_3 : 2;
    u8 unk77_5 : 1;
    u8 unk77_6 : 1;
    u8 unk77_7 : 1;
    u8 filler78[0x94 - 0x78];
    s32 speed;
    u8 filler98[0x9C - 0x98];
    s32 timer;
    s32 angle;
    s32 angleSpeed;
    s32 buttons;
    s16 step;
    u8 fillerAE[0xB2 - 0xAE];
    u16 dh;
};
extern u8 *gParty;
extern u8 *gWorld;
struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gSys;

void set_slot_value(u32 id, s32 value);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
extern const u8 gAnim278[];
extern const u8 gAnim374[];
void hold_decay_jump_timer_arc_a(struct Actor *obj);
void hold_decay_jump_timer_arc_b(struct Actor *obj);
void hold_decay_jump_timer_arc_c(struct Actor *obj);
void hold_decay_jump_timer_arc_d(struct Actor *obj);
void hold_decay_jump_timer_arc_e(struct Actor *obj);
void hold_decay_jump_timer_arc_f(struct Actor *obj);
s32 try_park_in_slot(struct Actor *obj);
void visible_action_204d(struct Actor *obj);
void visible_action_2000(struct Actor *obj);
void jump_arc_land(struct Actor *obj);
void hold_decay_arc_land_a(struct Actor *obj);
void hold_decay_arc_land_b(struct Actor *obj);
void hold_decay_arc_land_c(struct Actor *obj);
void hold_decay_arc_land_d(struct Actor *obj);
void hold_decay_arc_land_e(struct Actor *obj);
void charge_release_arc_land_c(struct Actor *obj);
void charge_release_arc_land_d(struct Actor *obj);
void hold_decay_finish_if_pose_visible_a(struct Actor *obj);
void hold_decay_finish_if_pose_visible_b(struct Actor *obj);
void hold_decay_finish_if_pose_visible_c(struct Actor *obj);
void hold_decay_finish_if_pose_visible_d(struct Actor *obj);
void charge_release_finish_if_pose_visible_c(struct Actor *obj);
void charge_release_finish_if_pose_visible_d(struct Actor *obj);
void hold_decay_orbit_step_a(struct Actor *obj);
void hold_decay_dispatch_slot_spawn(struct Actor *obj);
void hold_decay_orbit_step_b(struct Actor *obj);
void hold_decay_arc_rise_a(struct Actor *obj);
void hold_decay_hold_release_a(struct Actor *obj);
void hold_decay_timer_start_orbit_b(struct Actor *obj);
void step_end_action_23(struct Actor *obj);
void hold_decay_end_action_if_busy(struct Actor *obj);
void hold_decay_hold_idle_b(struct Actor *obj);
void hold_decay_arc_rise_b(struct Actor *obj);
void hold_decay_arc_rise_c(struct Actor *obj);
void hold_decay_arc_rise_d(struct Actor *obj);
void hold_decay_arc_rise_e(struct Actor *obj);
void hold_decay_arc_rise_f(struct Actor *obj);
void charge_release_arc_rise_c(struct Actor *obj);
void charge_release_arc_rise_d(struct Actor *obj);
void sprite_set_action(struct Actor *obj, u32 action, u32 part, u32 extra);
void end_action(struct Actor *obj);
void on_arrive(struct Actor *obj);
void charge_release_spawn_orbit_child_c(struct Actor *obj);
void hold_decay_spawn_orbit_child(struct Actor *obj);
u32 hold_decay_random_pause_jump_a(struct Actor *obj);
u32 hold_decay_random_pause_jump_b(struct Actor *obj);
u32 random_jump_c(struct Actor *obj);
u32 random_jump_d(struct Actor *obj);
u32 random_jump_e(struct Actor *obj);
u32 random_jump_f(struct Actor *obj);
u32 random_pause_jump_c(struct Actor *obj);
u32 random_pause_jump_d(struct Actor *obj);
void hold_decay_start_jump_arc_a(struct Actor *obj);
void hold_decay_start_jump_arc_b(struct Actor *obj);
void hold_decay_start_jump_arc_c(struct Actor *obj);
void hold_decay_start_jump_arc_d(struct Actor *obj);
void hold_decay_start_jump_arc_e(struct Actor *obj);
void hold_decay_start_jump_arc_f(struct Actor *obj);
void charge_release_start_jump_arc_c(struct Actor *obj);
void charge_release_start_jump_arc_d(struct Actor *obj);
void timer_start_jump_c(struct Actor *obj);
void timer_start_jump_d(struct Actor *obj);

/*
 * Charge wind-up: while the actor's buttons stay held, step counts up and shows charge frames 0-3
 * of animation 0x2054 every 3 steps. At step 12 the charge is complete (action 1) and
 * hold_decay_hold_idle_b takes over; letting go earlier hands step * 3 to
 * hold_decay_hold_release_a, which rewinds the animation. Any action bits end the action.
 */
void hold_decay_hold_step_a(struct Actor *obj)
{
    u32 action;

    obj->state = hold_decay_hold_step_a;
    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
    {
        sprite_set_action(obj, 0, 0x2054, 0);
    }
    if (obj->step == 3)
    {
        sprite_set_action(obj, 0, 0x2054, 1);
    }
    if (obj->step == 6)
    {
        sprite_set_action(obj, 0, 0x2054, 2);
    }
    if (obj->step == 9)
    {
        sprite_set_action(obj, 0, 0x2054, 3);
    }
    if (obj->step != 12)
    {
        if ((gSys.held & obj->buttons) == 0)
        {
            /* let go early: rewind the charge */
            obj->step = obj->step * 3;
            obj->state = hold_decay_hold_release_a;
            return;
        }
    }
    else
    {
        /* fully charged */
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->step = action;
        obj->state = hold_decay_hold_idle_b;
    }
    ((struct PoseMode *)obj->pose)->hold = 1;
    obj->step++;
}

/*
 * The button was let go before the charge finished (hold_decay_hold_step_a tripled step): counts
 * step down by 3 a frame, rewinding the charge animation (0x2054) through frames 2, 1, 0. Once
 * step drops below 0, plays action 4 once (held on its last frame) and ends the action when it
 * is done (hold_decay_end_action_if_busy).
 */
void hold_decay_hold_release_a(struct Actor *obj)
{
    u32 action;

    obj->state = hold_decay_hold_release_a;
    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xC);
        ((struct PoseMode *)obj->pose)->hold = 0;
        obj->state = hold_decay_end_action_if_busy;
        return;
    }
    if (obj->step <= 8)
    {
        if (obj->pose->count != 0)
        {
            sprite_set_action(obj, 0, 0x2054, 0);
        }
    }
    else if (obj->step <= 17)
    {
        if (obj->pose->count > 1)
        {
            sprite_set_action(obj, 0, 0x2054, 1);
        }
    }
    else if (obj->step <= 26)
    {
        if (obj->pose->count > 2)
        {
            sprite_set_action(obj, 0, 0x2054, 2);
        }
    }
    ((struct PoseMode *)obj->pose)->hold = 1;
    obj->step -= 3;
}

/* Variant b, holding: while the actor's buttons stay held, counts obj->step up (the pose stops
 * holding after step 0x1D). Letting go waits 10-14 frames in hold_decay_timer_start_orbit_b.
 * Held past step 0x2C the move ends: data kind 0x1E waits until gWorld[9]'s high nibble is 1 and
 * then plays action 4 (result 0xC0 / 12), any other kind plays action 5. */
void hold_decay_hold_idle_b(struct Actor *obj)
{
    u32 held;
    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    held = gSys.held & obj->buttons;
    if (held == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 10;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->state = hold_decay_timer_start_orbit_b;
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
                ((struct PoseMode *)obj->pose)->mode = 1;
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
            ((struct PoseMode *)obj->pose)->mode = 1;
        }
        ((struct PoseMode *)obj->pose)->hold = 0;
        obj->state = hold_decay_end_action_if_busy;
    }
    else
    {
        if (obj->step > 0x1D)
        {
            ((struct PoseMode *)obj->pose)->hold = 0;
        }
        obj->step++;
    }
}

/* One frame of orbit b: swings the child around a circle of radius 24 above obj's base point
 * (angle += angleSpeed, speed bent towards obj's facing), steps obj's animation 0x2054 through
 * frames 1-5, and at step 9 releases the child and goes on to step_end_action_23. */
void hold_decay_orbit_step_b(struct Actor *obj)
{
    struct Actor *child;
    u32 tableIdx;
    s32 centerH;
    const s16 *cosTable;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActionBits *)child)->mode = 1;
    child->angle += child->angleSpeed;
    tableIdx = MATH_ROUTINE(u32 (*)(s32, s32), iwram_div)(child->angle, 360);
    cosTable = gCosineTable;
    child->x = obj->baseX + (cosTable[tableIdx & 0xFF] / 64) * 24;
    centerH = obj->baseH + 0x1A00;
    child->h = centerH + (gSineTable[tableIdx & 0xFF] / 64) * 24;
    if (obj->pose->dir & 0x40)
        child->angleSpeed += 0x998;
    else
        child->angleSpeed -= 0x998;
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
        ((struct ActionBits *)child)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        ((struct PoseMode *)obj->pose)->hold = 0;
        obj->step = 0;
        obj->state = step_end_action_23;
        if (obj->data != NULL)
            ((struct DataLockBits *)&obj->data->lock)->locked = 1;
    }
    else
    {
        ((struct PoseMode *)obj->pose)->hold = 1;
        obj->step++;
    }
}

void step_end_action_23(struct Actor *obj)
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

void hold_decay_timer_start_orbit_b(struct Actor *obj)
{
    struct Actor *child;

    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else if (obj->timer > 0)
    {
        obj->timer--;
    }
    else
    {
        child = obj->child;
        child->angle = 0xB400;
        child->angleSpeed = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        ((struct PoseMode *)obj->pose)->hold = 1;
        obj->state = hold_decay_orbit_step_b;
    }
}

void step_end_action_19(struct Actor *obj)
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

void hold_decay_timer_start_orbit_a(struct Actor *obj)
{
    struct Actor *child;

    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else if (obj->timer > 0)
    {
        obj->timer--;
    }
    else
    {
        child = obj->child;
        child->angle = 0xB400;
        child->angleSpeed = -0x1CC;
        sprite_set_action(obj, 2, 0, 0);
        ((struct PoseMode *)obj->pose)->hold = 1;
        obj->state = hold_decay_orbit_step_a;
    }
}

void hold_decay_end_action_if_busy(struct Actor *obj)
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

u32 hold_decay_setup_slot_state(struct Actor *obj)
{
    struct Data *data;
    u32 *dst;
    u8 *base;

    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    dst = &data->slotValue;
    base = gParty;
    base += (~data->slot) * 0x3C;
    *dst = *(u16 *)(base + 0x2C);
    obj->state = hold_decay_dispatch_slot_spawn;
    obj->callback = end_action;
    return 1;
}

u32 hold_decay_dispatch_slot_pause_jump(struct Actor *obj)
{
    s16 slot = obj->data->slot;

    if (slot == -1)
    {
        hold_decay_random_pause_jump_b(obj);
    }
    else if (slot == -2)
    {
        hold_decay_random_pause_jump_a(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

void hold_decay_dispatch_slot_spawn(struct Actor *obj)
{
    if (obj->data->slot == -1)
    {
        charge_release_spawn_orbit_child_c(obj);
    }
    else
    {
        hold_decay_spawn_orbit_child(obj);
    }
}

u32 hold_decay_random_pause_jump_a(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;

    obj->anim = gAnim374;
    base = gParty;
    base += 0x3C;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(5, mt19937_next()) + 10;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = hold_decay_jump_timer_arc_a;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_a;
    }
    return 1;
}

u32 hold_decay_random_pause_jump_b(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;

    obj->anim = gAnim278;
    base = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(6, mt19937_next()) + 2;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = hold_decay_jump_timer_arc_b;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_b;
    }
    return 1;
}

void hold_decay_start_jump_arc_a(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x005A1440);
    obj->dh = (start - 0x48) / 2;
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = hold_decay_arc_rise_a;
}

void hold_decay_jump_timer_arc_a(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_decay_start_jump_arc_a(obj);
    }
}

void hold_decay_start_jump_arc_b(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x006E1E40);
    obj->dh = (start - 0x58) / 2;
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = hold_decay_arc_rise_b;
}

void hold_decay_jump_timer_arc_b(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_decay_start_jump_arc_b(obj);
    }
}

void hold_decay_arc_rise_a(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_decay_arc_land_a;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_decay_arc_rise_b(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_decay_arc_land_b;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_decay_arc_land_a(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = hold_decay_finish_if_pose_visible_a;
    }
}

void hold_decay_arc_land_b(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = hold_decay_finish_if_pose_visible_b;
    }
}

void hold_decay_finish_if_pose_visible_a(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void hold_decay_finish_if_pose_visible_b(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

/* Jump set-up for the party member in slot -1 (gParty entry 0; called by
 * dispatch_slot_random_jump_a): animation gAnim278, copies the member's u16 at +0x2A into
 * data->slotValue and waits a random 4..9 frames in action 4 (hold_decay_jump_timer_arc_d)
 * before the jump (hold_decay_start_jump_arc_d, speed -0x58). dh = (sqrt(0x6E1E40) - 0x58) / 2. */
u32 random_jump_c(struct Actor *obj)
{
    s32 delay;
    u8 *member;
    struct Data *data;
    s32 (*sqrt_fn)(u32);

    obj->anim = gAnim278;
    member = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(member + 0x2A);
    delay = rand_below(6, mt19937_next()) + 4;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0; /* loop the wait animation */
        obj->timer = delay;
        obj->state = hold_decay_jump_timer_arc_d;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_d;
    }
    sqrt_fn = (s32 (*)(u32))gMathRoutines; /* iwram_sqrt's copy */
    obj->dh = (sqrt_fn(0x006E1E40) - 0x58) / 2;
    return 1;
}

/* Jump set-up for the party member in slot -2 (gParty entry 1, 0x3C bytes each; called by
 * dispatch_slot_random_jump_a): animation gAnim374, copies the member's u16 at +0x2A into
 * data->slotValue and waits a random 5..15 frames in action 4 (hold_decay_jump_timer_arc_c)
 * before the jump (hold_decay_start_jump_arc_c, speed -0x48). dh = (sqrt(0x5A1440) - 0x48) / 2. */
u32 random_jump_d(struct Actor *obj)
{
    s32 delay;
    u8 *member;
    struct Data *data;
    s32 (*sqrt_fn)(u32);

    obj->anim = gAnim374;
    member = gParty;
    member += 0x3C;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(member + 0x2A);
    delay = rand_below(11, mt19937_next()) + 5;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0; /* loop the wait animation */
        obj->timer = delay;
        obj->state = hold_decay_jump_timer_arc_c;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_c;
    }
    sqrt_fn = (s32 (*)(u32))gMathRoutines; /* iwram_sqrt's copy */
    obj->dh = (sqrt_fn(0x005A1440) - 0x48) / 2;
    return 1;
}

void hold_decay_start_jump_arc_c(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = hold_decay_arc_rise_c;
}

void hold_decay_jump_timer_arc_c(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_decay_start_jump_arc_c(obj);
    }
}

void hold_decay_start_jump_arc_d(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = hold_decay_arc_rise_d;
}

void hold_decay_jump_timer_arc_d(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_decay_start_jump_arc_d(obj);
    }
}

u32 dispatch_slot_random_jump_a(struct Actor *obj)
{
    s16 slot = obj->data->slot;

    if (slot == -1)
    {
        random_jump_c(obj);
    }
    else if (slot == -2)
    {
        random_jump_d(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

void hold_decay_arc_rise_c(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_decay_arc_land_c;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_decay_arc_rise_d(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_decay_arc_land_d;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_decay_arc_land_c(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = hold_decay_finish_if_pose_visible_c;
    }
}

void hold_decay_arc_land_d(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = hold_decay_finish_if_pose_visible_d;
    }
}

void hold_decay_finish_if_pose_visible_c(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void hold_decay_finish_if_pose_visible_d(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

/* Jump set-up for the party member in slot -1 (gParty entry 0; called by
 * dispatch_slot_random_jump_b): animation gAnim278, copies the member's u16 at +0x2A into
 * data->slotValue and waits a random 7..11 frames in action 4 (hold_decay_jump_timer_arc_f)
 * before the jump (hold_decay_start_jump_arc_f, speed -0x58). dh = (sqrt(0x6E1E40) - 0x58) / 2. */
u32 random_jump_e(struct Actor *obj)
{
    s32 delay;
    u8 *member;
    struct Data *data;
    s32 (*sqrt_fn)(u32);

    obj->anim = gAnim278;
    member = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(member + 0x2A);
    delay = rand_below(5, mt19937_next()) + 7;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0; /* loop the wait animation */
        obj->timer = delay;
        obj->state = hold_decay_jump_timer_arc_f;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_f;
    }
    sqrt_fn = (s32 (*)(u32))gMathRoutines; /* iwram_sqrt's copy */
    obj->dh = (sqrt_fn(0x006E1E40) - 0x58) / 2;
    return 1;
}

/* Starts the jump of the hero in slot -2 (party entry 1; random_jump_e does entry 0): copies the
   party entry's u16 at 0x2A into data->slotValue, then waits a random 5-15 frames in action 4
   (hold_decay_jump_timer_arc_e) before hold_decay_start_jump_arc_e jumps off at speed -0x48.
   dh gets (sqrt(0x5A1440) - 0x48) / 2, using the RAM copy of the square root routine. */
u32 random_jump_f(struct Actor *obj)
{
    s32 delay;
    u8 *party;
    struct Data *data;
    s32 (*sqrt)(u32);

    obj->anim = gAnim374;
    party = gParty;
    party += 0x3C; /* entry 1 */
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(party + 0x2A);
    delay = rand_below(11, mt19937_next()) + 5;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = delay;
        obj->state = hold_decay_jump_timer_arc_e;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_e;
    }
    sqrt = (s32 (*)(u32))gMathRoutines;
    obj->dh = (sqrt(0x005A1440) - 0x48) / 2;
    return 1;
}

void hold_decay_start_jump_arc_e(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = hold_decay_arc_rise_e;
}

void hold_decay_jump_timer_arc_e(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_decay_start_jump_arc_e(obj);
    }
}

void hold_decay_start_jump_arc_f(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = hold_decay_arc_rise_f;
}

void hold_decay_jump_timer_arc_f(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_decay_start_jump_arc_f(obj);
    }
}

u32 dispatch_slot_random_jump_b(struct Actor *obj)
{
    s16 slot = obj->data->slot;

    if (slot == -1)
    {
        random_jump_e(obj);
    }
    else if (slot == -2)
    {
        random_jump_f(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

void hold_decay_arc_rise_e(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_decay_arc_land_e;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_decay_arc_rise_f(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = charge_release_arc_land_d;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_decay_arc_land_e(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = charge_release_finish_if_pose_visible_c;
    }
}

void charge_release_arc_land_d(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = charge_release_finish_if_pose_visible_d;
    }
}

void charge_release_finish_if_pose_visible_c(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void charge_release_finish_if_pose_visible_d(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

/* Jump set-up for the party member in slot -1 (gParty entry 0; called by
 * dispatch_slot_random_pause_jump): animation gAnim278, copies the member's u16 at +0x2A into
 * data->slotValue and waits a random 2..5 frames in action 4 (timer_start_jump_d) before the
 * jump (charge_release_start_jump_arc_d, speed -0x58). dh = (sqrt(0x6E1E40) - 0x58) / 2. */
u32 random_pause_jump_c(struct Actor *obj)
{
    s32 delay;
    u8 *member;
    struct Data *data;
    s32 (*sqrt_fn)(u32);

    obj->anim = gAnim278;
    member = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(member + 0x2A);
    delay = rand_below(4, mt19937_next()) + 2;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0; /* loop the wait animation */
        obj->timer = delay;
        obj->state = timer_start_jump_d;
    }
    else
    {
        obj->state = charge_release_start_jump_arc_d;
    }
    sqrt_fn = (s32 (*)(u32))gMathRoutines; /* iwram_sqrt's copy */
    obj->dh = (sqrt_fn(0x006E1E40) - 0x58) / 2;
    return 1;
}

/* Jump set-up for the party member in slot -2 (gParty entry 1, 0x3C bytes each; called by
 * dispatch_slot_random_pause_jump): animation gAnim374, copies the member's u16 at +0x2A into
 * data->slotValue and waits a random 5..15 frames in action 4 (timer_start_jump_c) before the
 * jump (charge_release_start_jump_arc_c, speed -0x48). dh = (sqrt(0x5A1440) - 0x48) / 2. */
u32 random_pause_jump_d(struct Actor *obj)
{
    s32 delay;
    u8 *member;
    struct Data *data;
    s32 (*sqrt_fn)(u32);

    obj->anim = gAnim374;
    member = gParty;
    member += 0x3C;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(member + 0x2A);
    delay = rand_below(11, mt19937_next()) + 5;
    if (delay != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0; /* loop the wait animation */
        obj->timer = delay;
        obj->state = timer_start_jump_c;
    }
    else
    {
        obj->state = charge_release_start_jump_arc_c;
    }
    sqrt_fn = (s32 (*)(u32))gMathRoutines; /* iwram_sqrt's copy */
    obj->dh = (sqrt_fn(0x005A1440) - 0x48) / 2;
    return 1;
}

void charge_release_start_jump_arc_c(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = charge_release_arc_rise_c;
}

void timer_start_jump_c(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        charge_release_start_jump_arc_c(obj);
    }
}

void charge_release_start_jump_arc_d(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseMode *)obj->pose)->mode = 1;
    obj->state = charge_release_arc_rise_d;
}

void timer_start_jump_d(struct Actor *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        charge_release_start_jump_arc_d(obj);
    }
}

u32 dispatch_slot_random_pause_jump(struct Actor *obj)
{
    s16 slot = obj->data->slot;

    if (slot == -1)
    {
        random_pause_jump_c(obj);
    }
    else if (slot == -2)
    {
        random_pause_jump_d(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

void charge_release_arc_rise_c(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = charge_release_arc_land_c;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void charge_release_arc_rise_d(struct Actor *obj)
{
    obj->dh += obj->speed;
    obj->h = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = jump_arc_land;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseMode *)obj->pose)->mode = 1;
            obj->phase = 1;
        }
    }
}

void charge_release_arc_land_c(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = visible_action_204d;
    }
}

void jump_arc_land(struct Actor *obj)
{
    s32 h;

    obj->dh += obj->speed;
    h = obj->baseH + (s16)obj->dh;
    obj->h = h;
    if (h <= obj->data->ground)
    {
        obj->phase = 0;
        obj->h = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 1;
        obj->state = visible_action_2000;
    }
}

void visible_action_204d(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}
