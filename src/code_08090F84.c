#include "global.h"
#include "ram_routines.h"

/* The actor's animated sprite, stepped by sprite_step_e6a0. */
struct Pose
{
    u8 filler0[0x11];
    u8 turn;
    u8 unk12_0 : 1;
    u8 mode : 2;        /* at the animation's end: 0 loop, 1 hold the last frame, 2 stop */
    u8 visible : 1;     /* set when the animation has reached its end */
    u8 hold : 1;        /* freezes the animation on its current frame */
    u8 unk12_5 : 3;
    u8 filler13[0x20 - 0x13];
    u8 result;          /* animation speed per step */
    u8 filler21[0x23 - 0x21];
    u8 count;           /* current frame of the animation */
};

struct Actor
{
    u8 filler0[0x08];
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    u8 filler14[0x18 - 0x14];
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    u8 *data;
    u8 filler2C[0x30 - 0x2C];
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
    void *state;
    u8 filler50[0x68 - 0x50];
    void *callback;
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 flags76;
    u8 phase : 3;
    u8 unk77_3 : 2;
    u8 switch5 : 1;
    u8 switch6 : 1;
    u8 unk77_7 : 1;
    u8 filler78[0x79 - 0x78];
    u8 visible79 : 1;
    u8 unk79_1 : 7;
    u8 filler7A[0x84 - 0x7A];
    s32 targetX;
    u8 filler88[0x8C - 0x88];
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 speed;
    u8 filler98[0x9C - 0x98];
    s32 timer;
    s32 angle;
    s32 angleSpeed;
    u32 buttons;
    s16 step;
    u8 filler0AE[0xB2 - 0xAE];
    s16 dh;
};

extern const u8 gAnim278[];
extern const u8 gAnim284[];
extern const u8 gAnim374[];
extern u8 *gParty;
extern u8 gSys[];
extern u8 *gWorld;

#define SYS_HELD_KEYS 0x3C      /* gSys: u16, the keys held this frame */
#define ACTOR_ACTION_MASK 0x38  /* Actor.flags76 bits 3-5: set when the action must end */
#define ANIM_ORBIT_CHARGE 0x2025 /* sprite_set_action arg: the orbit's charge animation (frames 0-3) */
#define MARIO_ANIM_BASE 0x2000   /* sprite_set_action arg: Mario's battle animation set */
#define LUIGI_ANIM_BASE 0x204D   /* sprite_set_action arg: Luigi's battle animation set */
#define PARTY_HERO_SIZE 0x3C     /* one hero's record in gParty: Mario's at 0, Luigi's at 0x3C */
#define HERO_SLOT_MARIO -1       /* ActorData + 0xEC: which hero the actor is */
#define HERO_SLOT_LUIGI -2

/* sprite_set_action actions of a hero's jump */
#define HERO_ACTION_STAND 0
#define HERO_ACTION_WIND_UP 4    /* crouch before the jump */
#define HERO_ACTION_JUMP 5
#define HERO_ACTION_FALL 6
#define HERO_ACTION_LAND 8

/* Pose.mode */
#define POSE_MODE_LOOP 0
#define POSE_MODE_HOLD_LAST 1

void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct Actor *obj, u32 action, u32 arg, u32 extra);
s32 try_park_in_slot(struct Actor *obj);
void hold_orbit_arc_land_e(struct Actor *obj);
void finish_if_pose_visible_g(struct Actor *obj);
void on_arrive(struct Actor *obj);
void end_action(struct Actor *obj);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
struct ActionBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct ActorData
{
    u8 filler0[0x111];
    u8 unk111_0 : 2;
    u8 lock : 1;
    u8 unk111_3 : 5;
};

struct Actor *obj_spawn(u8 *data, u32 kind, s32 x, s32 y, s32 h, s32 a, s32 b);

/* prototypes */
u32 random_jump_setup_list_slot_a(struct Actor *obj);
u32 random_jump_setup_list_slot_b(struct Actor *obj);
void random_jump_start_jump_arc_a(struct Actor *obj);
void random_jump_start_jump_arc_b(struct Actor *obj);
void random_jump_arc_rise_a(struct Actor *obj);
void random_jump_arc_rise_b(struct Actor *obj);
void random_jump_arc_land_a(struct Actor *obj);
void random_jump_arc_land_b(struct Actor *obj);
void random_jump_finish_if_pose_visible_a(struct Actor *obj);
void random_jump_finish_if_pose_visible_b(struct Actor *obj);
u32 dispatch_slot_begin_hold(struct Actor *obj);
u32 begin_hold_a(struct Actor *obj);
u32 begin_hold_b(struct Actor *obj);
void random_jump_timer_arc_c(struct Actor *obj);
void random_jump_timer_arc_d(struct Actor *obj);
void random_jump_start_jump_arc_c(struct Actor *obj);
void random_jump_start_jump_arc_d(struct Actor *obj);
void random_jump_arc_rise_c(struct Actor *obj);
void random_jump_arc_rise_d(struct Actor *obj);
void random_jump_arc_land_c(struct Actor *obj);
void random_jump_arc_land_d(struct Actor *obj);
void random_jump_finish_if_pose_visible_c(struct Actor *obj);
void random_jump_finish_if_pose_visible_d(struct Actor *obj);
u32 random_jump_a(struct Actor *obj);
u32 random_jump_b(struct Actor *obj);
void random_jump_spawn_orbit_child_a(struct Actor *obj);
void random_jump_orbit_step_a(struct Actor *obj);
void random_jump_orbit_step_b(struct Actor *obj);
void random_jump_orbit_hold_a(struct Actor *obj);
void random_jump_orbit_circle_a(struct Actor *obj);
void random_jump_spawn_orbit_child_b(struct Actor *obj);
void random_jump_orbit_step_c(struct Actor *obj);
void random_jump_orbit_step_d(struct Actor *obj);
void random_jump_orbit_hold_b(struct Actor *obj);
void random_jump_orbit_circle_b(struct Actor *obj);
void random_jump_orbit_end_b(struct Actor *obj);
void orbit_timer_b(struct Actor *obj);
void random_jump_orbit_end_a(struct Actor *obj);
void orbit_timer_a(struct Actor *obj);
void random_jump_end_action_if_pose_visible(struct Actor *obj);
u32 random_jump_setup_slot_spawn(struct Actor *obj);
void random_jump_start_jump_arc_f(struct Actor *obj);
void random_jump_timer_arc_f(struct Actor *obj);
void random_jump_start_jump_arc_e(struct Actor *obj);
void random_jump_timer_arc_e(struct Actor *obj);
u32 random_jump_dispatch_slot_random_jump(struct Actor *obj);
void random_jump_dispatch_slot_spawn(struct Actor *obj);
void hold_orbit_arc_rise_f(struct Actor *obj);
void hold_orbit_arc_rise_e(struct Actor *obj);
void arc_land_f(struct Actor *obj);

u32 random_jump_setup_list_slot_a(struct Actor *obj)
{
    u8 *entry;

    obj->anim = gAnim374;
    entry = gParty + 0x3C;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(entry + 0x2A);
    obj->state = random_jump_start_jump_arc_a;
    return 1;
}

u32 random_jump_setup_list_slot_b(struct Actor *obj)
{
    u8 *entry;

    obj->anim = gAnim278;
    entry = gParty;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(entry + 0x2A);
    obj->state = random_jump_start_jump_arc_b;
    return 1;
}

void random_jump_start_jump_arc_a(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x0096F390);
    obj->dh = (start - 0xAC) / 2;
    obj->speed = -0xAC;
    sprite_set_action(obj, 5, 0x204D, 0);
    obj->pose->mode = 1;
    obj->state = random_jump_arc_rise_a;
}

void random_jump_start_jump_arc_b(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x00A50A10);
    obj->dh = (start - 0xBC) / 2;
    obj->speed = -0xBC;
    sprite_set_action(obj, 5, 0x2000, 0);
    obj->pose->mode = 1;
    obj->state = random_jump_arc_rise_b;
}

void random_jump_arc_rise_a(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj->state = random_jump_arc_land_a;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            obj->pose->mode = 1;
            obj->phase = 1;
        }
    }
}

void random_jump_arc_rise_b(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj->state = random_jump_arc_land_b;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            obj->pose->mode = 1;
            obj->phase = 1;
        }
    }
}

void random_jump_arc_land_a(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->h <= *(s32 *)(obj->data + 0xE0))
    {
        obj->phase = 0;
        obj->h = *(s32 *)(obj->data + 0xE0);
        sprite_set_action(obj, 8, 0x204D, 0);
        obj->state = random_jump_finish_if_pose_visible_a;
    }
}

void random_jump_arc_land_b(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->h <= *(s32 *)(obj->data + 0xE0))
    {
        obj->phase = 0;
        obj->h = *(s32 *)(obj->data + 0xE0);
        sprite_set_action(obj, 8, 0x2000, 0);
        obj->state = random_jump_finish_if_pose_visible_b;
    }
}

void random_jump_finish_if_pose_visible_a(struct Actor *obj)
{
    if (obj->pose->visible)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void random_jump_finish_if_pose_visible_b(struct Actor *obj)
{
    if (obj->pose->visible)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

u32 dispatch_slot_begin_hold(struct Actor *obj)
{
    s16 slot;

    slot = *(s16 *)(obj->data + 0xEC);
    if (slot == -1)
    {
        begin_hold_b(obj);
    }
    else if (slot == -2)
    {
        begin_hold_a(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

u32 begin_hold_a(struct Actor *obj)
{
    u8 *entry;

    obj->anim = gAnim374;
    entry = gParty + 0x3C;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(entry + 0x2A);
    sprite_set_action(obj, 4, 0x204D, 0);
    obj->pose->result = 0x60;
    obj->timer = 1;
    obj->state = random_jump_timer_arc_c;
    return 1;
}

u32 begin_hold_b(struct Actor *obj)
{
    u8 *entry;

    obj->anim = gAnim278;
    entry = gParty;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(entry + 0x2A);
    sprite_set_action(obj, 4, 0x2000, 0);
    obj->pose->result = 0x60;
    obj->timer = 1;
    obj->state = random_jump_timer_arc_d;
    return 1;
}

void random_jump_timer_arc_c(struct Actor *obj)
{
    obj->timer = obj->timer - 1;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        random_jump_start_jump_arc_c(obj);
    }
}

void random_jump_timer_arc_d(struct Actor *obj)
{
    obj->timer = obj->timer - 1;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        random_jump_start_jump_arc_d(obj);
    }
}

void random_jump_start_jump_arc_c(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x0025A739);
    obj->dh = (start - 0x2B) / 2;
    obj->speed = -0x2B;
    sprite_set_action(obj, 5, 0x204D, 0);
    obj->pose->mode = 1;
    obj->state = random_jump_arc_rise_c;
}

void random_jump_start_jump_arc_d(struct Actor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x002928A1);
    obj->dh = (start - 0x2F) / 2;
    obj->speed = -0x2F;
    sprite_set_action(obj, 5, 0x2000, 0);
    obj->pose->mode = 1;
    obj->state = random_jump_arc_rise_d;
}

void random_jump_arc_rise_c(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj->state = random_jump_arc_land_c;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            obj->pose->mode = 1;
            obj->phase = 1;
        }
    }
}

void random_jump_arc_rise_d(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj->state = random_jump_arc_land_d;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            obj->pose->mode = 1;
            obj->phase = 1;
        }
    }
}

void random_jump_arc_land_c(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->h <= *(s32 *)(obj->data + 0xE0))
    {
        obj->phase = 0;
        obj->h = *(s32 *)(obj->data + 0xE0);
        sprite_set_action(obj, 8, 0x204D, 0);
        obj->state = random_jump_finish_if_pose_visible_c;
    }
}

void random_jump_arc_land_d(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->h <= *(s32 *)(obj->data + 0xE0))
    {
        obj->phase = 0;
        obj->h = *(s32 *)(obj->data + 0xE0);
        sprite_set_action(obj, 8, 0x2000, 0);
        obj->state = random_jump_finish_if_pose_visible_d;
    }
}

void random_jump_finish_if_pose_visible_c(struct Actor *obj)
{
    if (obj->pose->visible)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void random_jump_finish_if_pose_visible_d(struct Actor *obj)
{
    if (obj->pose->visible)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

/*
 * Mario's jump after a random wait (slot HERO_SLOT_MARIO of random_jump_dispatch_slot_random_jump):
 * switches to Mario's animation table (gAnim278), copies the u16 at 0x2A of his gParty record into
 * his ActorData + 0x114, then crouches in a loop for 4-9 frames (random_jump_timer_arc_e) before
 * random_jump_start_jump_arc_e takes off. The arc starts (isqrt(0x689E40) - 0x58) / 2 above its base.
 * Returns 1 (handled).
 */
u32 random_jump_a(struct Actor *obj)
{
    s32 waitFrames;
    s32 root;
    s32 (*isqrt)(u32);
    u8 *hero;

    obj->anim = gAnim278;
    hero = gParty;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(hero + 0x2A);
    waitFrames = rand_below(6, mt19937_next()) + 4;
    if (waitFrames != 0)
    {
        sprite_set_action(obj, HERO_ACTION_WIND_UP, MARIO_ANIM_BASE, 0);
        obj->pose->mode = POSE_MODE_LOOP;
        obj->timer = waitFrames;
        obj->state = random_jump_timer_arc_e;
    }
    else
    {
        obj->state = random_jump_start_jump_arc_e;
    }
    isqrt = (s32 (*)(u32))gMathRoutines; /* gMathRoutines starts with the integer square root */
    root = isqrt(0x00689E40);
    obj->dh = (root - 0x58) / 2;
    return 1;
}

/*
 * Luigi's jump after a random wait (slot HERO_SLOT_LUIGI of random_jump_dispatch_slot_random_jump):
 * switches to Luigi's animation table (gAnim374), copies the u16 at 0x2A of his gParty record into
 * his ActorData + 0x114, then crouches in a loop for 10-31 frames (random_jump_timer_arc_f) before
 * random_jump_start_jump_arc_f takes off. The arc starts (isqrt(0x559440) - 0x48) / 2 above its base.
 * Returns 1 (handled).
 */
u32 random_jump_b(struct Actor *obj)
{
    s32 waitFrames;
    s32 root;
    s32 (*isqrt)(u32);
    u8 *hero;

    obj->anim = gAnim374;
    hero = gParty + PARTY_HERO_SIZE;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(hero + 0x2A);
    waitFrames = rand_below(22, mt19937_next()) + 10;
    if (waitFrames != 0)
    {
        sprite_set_action(obj, HERO_ACTION_WIND_UP, LUIGI_ANIM_BASE, 0);
        obj->pose->mode = POSE_MODE_LOOP;
        obj->timer = waitFrames;
        obj->state = random_jump_timer_arc_f;
    }
    else
    {
        obj->state = random_jump_start_jump_arc_f;
    }
    isqrt = (s32 (*)(u32))gMathRoutines; /* gMathRoutines starts with the integer square root */
    root = isqrt(0x00559440);
    obj->dh = (root - 0x48) / 2;
    return 1;
}

// Starts the random-jump orbit move: spawns the orbiting child at obj's base point (in whole
// pixels), in action mode 3 with its box bytes (unk44-unk49: -10, 8, -10, 10, -12, 4), phase 2,
// aiming at obj's base X and its own base height, with animation gAnim284, on_arrive as its
// callback and visible. Then resets the step, clears slot value 0x7C and runs the first wind-up
// frame (random_jump_orbit_step_a).
void random_jump_spawn_orbit_child_a(struct Actor *obj)
{
    struct Actor *child;
    u8 *act;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    act = (u8 *)&child->flags76;
    *act = *act | 6; // action mode 3
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xF4;
    child->unk49 = 4;
    child->phase = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    act = (u8 *)&child->flags76;
    *act = *act | 0x40;
    child->callback = on_arrive;
    child->visible79 = 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    random_jump_orbit_step_a(obj);
}

/* Orbit a, charging: while the actor's buttons stay held, step counts up and the charge animation
 * moves on a frame every 3 steps; at step 12 action 1 plays and random_jump_orbit_hold_a takes
 * over. Letting go earlier hands step * 2 to random_jump_orbit_step_b, which rewinds the charge.
 * Any action bits end the action. */
void random_jump_orbit_step_a(struct Actor *obj)
{
    u32 action;
    u8 *sys;

    obj->state = random_jump_orbit_step_a;
    action = obj->flags76 & ACTOR_ACTION_MASK;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
        sprite_set_action(obj, 0, ANIM_ORBIT_CHARGE, 0);
    if (obj->step == 3)
        sprite_set_action(obj, 0, ANIM_ORBIT_CHARGE, 1);
    if (obj->step == 6)
        sprite_set_action(obj, 0, ANIM_ORBIT_CHARGE, 2);
    if (obj->step == 9)
        sprite_set_action(obj, 0, ANIM_ORBIT_CHARGE, 3);
    if (obj->step != 12)
    {
        sys = gSys;
        if ((*(u16 *)(sys + SYS_HELD_KEYS) & obj->buttons) == 0)
        {
            obj->step *= 2;
            obj->state = random_jump_orbit_step_b;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->pose->mode = 0;
        obj->step = action; /* 0 */
        obj->state = random_jump_orbit_hold_a;
    }
    obj->pose->hold = 1;
    obj->step++;
}

/*
 * The button was let go before the charge finished (random_jump_orbit_step_a doubled step):
 * counts step down by 3 a frame, rewinding the charge animation through frames 2, 1, 0. Once
 * step drops below 0, plays action 4 once (held on its last frame) and ends the action when it
 * is done (random_jump_end_action_if_pose_visible).
 */
void random_jump_orbit_step_b(struct Actor *obj)
{
    u8 *poseFlags;

    obj->state = random_jump_orbit_step_b;
    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    if (obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->mode = 1;
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xA);
        obj->pose->hold = 0;
        obj->state = random_jump_end_action_if_pose_visible;
    }
    else
    {
        if (obj->step <= 5)
        {
            if (obj->pose->count != 0)
            {
                sprite_set_action(obj, 0, 0x2025, 0);
            }
        }
        else if (obj->step <= 11)
        {
            if (obj->pose->count > 1)
            {
                sprite_set_action(obj, 0, 0x2025, 1);
            }
        }
        else if (obj->step <= 17)
        {
            if (obj->pose->count > 2)
            {
                sprite_set_action(obj, 0, 0x2025, 2);
            }
        }
        /* pose->hold = 1 */
        poseFlags = (u8 *)obj->pose + 0x12;
        *poseFlags = *poseFlags | 0x10;
        obj->step = obj->step - 3;
    }
}

// Charge state after random_jump_orbit_step_a: while the attack's button (obj->buttons, matched against
// the held keys at gSys + 0x3C) stays down, step counts up and the pose's hold flag goes off after 24
// frames. Letting go waits a random 10..31 frames (orbit_timer_a) before the orbit starts. Held past 49
// frames the charge fails (action 5); data kind 0x1E instead waits there until gWorld[9]'s high nibble
// is 1, then cancels with action 4.
void random_jump_orbit_hold_a(struct Actor *obj)
{
    u32 held;
    u8 *sys;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    sys = gSys;
    held = *(u16 *)(sys + 0x3C) & obj->buttons;
    if (held == 0)
    {
        obj->timer = rand_below(0x16, mt19937_next()) + 10;
        sprite_set_action(obj, 6, 0, 0);
        obj->pose->mode = 0;
        obj->state = orbit_timer_a;
        obj->step = 0;
        return;
    }
    if (obj->step > 49)
    {
        if (*(u8 *)(obj->data + 0x12D) == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                obj->pose->mode = 1;
                obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 10);
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
            obj->pose->mode = 1;
        }
        obj->pose->hold = 0;
        obj->state = random_jump_end_action_if_pose_visible;
        return;
    }
    if (obj->step > 24)
    {
        obj->pose->hold = 0;
    }
    obj->step = obj->step + 1;
}

/* One frame of orbit a: swings the child around a circle of radius 22 above obj's base point
 * (angle += angleSpeed, speed bent towards obj's facing), steps obj's animation 0x2025 through
 * frames 1-5, and at step 7 releases the child and goes on to random_jump_orbit_end_a. */
void random_jump_orbit_circle_a(struct Actor *obj)
{
    struct Actor *child;
    u32 tableIdx;
    s32 centerH;
    const s16 *cosTable;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActionBits *)&child->flags76)->mode = 1;
    child->angle += child->angleSpeed;
    tableIdx = MATH_ROUTINE(u32 (*)(s32, s32), iwram_div)(child->angle, 360);
    cosTable = gCosineTable;
    child->posX = obj->baseX + (cosTable[tableIdx & 0xFF] / 64) * 22;
    centerH = obj->baseH + 0x1600;
    child->h = centerH + (gSineTable[tableIdx & 0xFF] / 64) * 22;
    if (obj->pose->turn & 0x40)
        child->angleSpeed += 0xC00;
    else
        child->angleSpeed -= 0xC00;
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
        ((struct ActionBits *)&child->flags76)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->hold = 0;
        obj->pose->mode = 0;
        obj->step = 0;
        obj->state = random_jump_orbit_end_a;
        if (obj->data != NULL)
            ((struct ActorData *)obj->data)->lock = 1;
    }
    else
    {
        obj->pose->hold = 1;
        obj->step++;
    }
}

// Starts the second random-jump orbit move: spawns the orbiting child at obj's base point (in whole
// pixels), in action mode 3 with its box bytes (unk44-unk49: -10, 8, -10, 10, -12, 4), phase 2,
// aiming at obj's base X and its own base height, with animation gAnim284, on_arrive as its
// callback and visible. Then resets the step, clears slot value 0x7C and runs the first wind-up
// frame (random_jump_orbit_step_c).
void random_jump_spawn_orbit_child_b(struct Actor *obj)
{
    struct Actor *child;
    u8 *act;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    act = (u8 *)&child->flags76;
    *act = *act | 6; // action mode 3
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xF4;
    child->unk49 = 4;
    child->phase = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    act = (u8 *)&child->flags76;
    *act = *act | 0x40;
    child->callback = on_arrive;
    child->visible79 = 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    random_jump_orbit_step_c(obj);
}

/* Orbit b wind-up: while the actor's buttons (the held keys at gSys + 0x3C) stay down, step counts
 * up and shows wind-up frames 0-3 of animation 0x2054 every 2 steps. At step 8 the wind-up is
 * complete and random_jump_orbit_hold_b takes over; letting go earlier hands step * 3 to
 * random_jump_orbit_step_d, which winds the animation back down. Any action bits end the action. */
void random_jump_orbit_step_c(struct Actor *obj)
{
    u32 action;
    u8 *sys;

    obj->state = random_jump_orbit_step_c;
    action = obj->flags76 & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
        sprite_set_action(obj, 0, 0x2054, 0);
    if (obj->step == 2)
        sprite_set_action(obj, 0, 0x2054, 1);
    if (obj->step == 4)
        sprite_set_action(obj, 0, 0x2054, 2);
    if (obj->step == 6)
        sprite_set_action(obj, 0, 0x2054, 3);
    if (obj->step != 8)
    {
        sys = gSys;
        if ((*(u16 *)(sys + 0x3C) & obj->buttons) == 0)
        {
            /* let go early: wind back down */
            obj->step *= 3;
            obj->state = random_jump_orbit_step_d;
            return;
        }
    }
    else
    {
        /* fully wound up */
        sprite_set_action(obj, 1, 0, 0);
        obj->pose->mode = 0;
        obj->step = action;
        obj->state = random_jump_orbit_hold_b;
    }
    obj->pose->hold = 1;
    obj->step++;
}

/* Orbit b, button let go during the wind-up (random_jump_orbit_step_c multiplied step by 3): plays
 * the wind-up frames of animation 0x2054 back to frame 0, two steps a frame, then cancels with
 * action 4 (speed 0xC0 / 12) and ends once that pose has finished. */
void random_jump_orbit_step_d(struct Actor *obj)
{
    u8 *poseFlags;

    obj->state = random_jump_orbit_step_d;
    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    if (obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->mode = 1;
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 12);
        obj->pose->hold = 0;
        obj->state = random_jump_end_action_if_pose_visible;
    }
    else
    {
        if (obj->step <= 5)
        {
            if (obj->pose->count != 0)
                sprite_set_action(obj, 0, 0x2054, 0);
        }
        else if (obj->step <= 11)
        {
            if (obj->pose->count > 1)
                sprite_set_action(obj, 0, 0x2054, 1);
        }
        else if (obj->step <= 17)
        {
            if (obj->pose->count > 2)
                sprite_set_action(obj, 0, 0x2054, 2);
        }
        /* pose->hold = 1 */
        poseFlags = (u8 *)obj->pose + 0x12;
        *poseFlags = *poseFlags | 0x10;
        obj->step = obj->step - 2;
    }
}

/* Orbit b, holding: while the actor's buttons stay held, counts obj->step up (the pose stops
 * holding after step 0x17). Letting go waits 8-13 frames in orbit_timer_b, then swings the child.
 * Held past step 0x26 the jump ends instead: data kind 0x1E waits until gWorld[9]'s high nibble is
 * 1 and then plays action 4 (result 0xC0 / 12), any other kind plays action 5. */
void random_jump_orbit_hold_b(struct Actor *obj)
{
    u32 held;
    u8 *sys;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    sys = gSys;
    held = *(u16 *)(sys + 0x3C) & obj->buttons;
    if (held == 0)
    {
        obj->timer = rand_below(6, mt19937_next()) + 8;
        sprite_set_action(obj, 6, 0, 0);
        obj->pose->mode = 0;
        obj->state = orbit_timer_b;
        obj->step = 0;
        return;
    }
    if (obj->step > 0x26)
    {
        if (*(u8 *)(obj->data + 0x12D) == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                obj->pose->mode = 1;
                obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xC);
            }
            else
            {
                obj->step = 0x27;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
            obj->pose->mode = 1;
        }
        obj->pose->hold = 0;
        obj->state = random_jump_end_action_if_pose_visible;
        return;
    }
    if (obj->step > 0x17)
    {
        obj->pose->hold = 0;
    }
    obj->step = obj->step + 1;
}

/* One frame of orbit b: swings the child around a circle of radius 24 above obj's base point
 * (angle += angleSpeed, speed bent towards obj's facing), steps obj's animation 0x2054 through
 * frames 1-5, and at step 9 releases the child and goes on to random_jump_orbit_end_b. */
void random_jump_orbit_circle_b(struct Actor *obj)
{
    struct Actor *child;
    u32 tableIdx;
    s32 centerH;
    const s16 *cosTable;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActionBits *)&child->flags76)->mode = 1;
    child->angle += child->angleSpeed;
    tableIdx = MATH_ROUTINE(u32 (*)(s32, s32), iwram_div)(child->angle, 360);
    cosTable = gCosineTable;
    child->posX = obj->baseX + (cosTable[tableIdx & 0xFF] / 64) * 24;
    centerH = obj->baseH + 0x1A00;
    child->h = centerH + (gSineTable[tableIdx & 0xFF] / 64) * 24;
    if (obj->pose->turn & 0x40)
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
        ((struct ActionBits *)&child->flags76)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0;
        obj->pose->hold = 0;
        obj->step = 0;
        obj->state = random_jump_orbit_end_b;
        if (obj->data != NULL)
            ((struct ActorData *)obj->data)->lock = 1;
    }
    else
    {
        obj->pose->hold = 1;
        obj->step++;
    }
}

void random_jump_orbit_end_b(struct Actor *obj)
{
    struct ActorData *data;
    u16 old;

    data = (struct ActorData *)obj->data;
    if (data != 0)
    {
        data->lock = 0;
    }
    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    old = obj->step;
    obj->step = old + 1;
    if ((s16)old > 0x17)
    {
        end_action(obj);
    }
}

void orbit_timer_b(struct Actor *obj)
{
    struct Actor *child;
    u8 *p;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    if (obj->timer > 0)
    {
        obj->timer = obj->timer - 1;
        return;
    }
    child = obj->child;
    child->angle = 0xB400;
    child->angleSpeed = 0xA00;
    sprite_set_action(obj, 2, 0, 0);
    p = (u8 *)obj->pose + 0x12;
    *p = *p | 0x10;
    obj->state = random_jump_orbit_circle_b;
}

void random_jump_orbit_end_a(struct Actor *obj)
{
    struct ActorData *data;
    u16 old;

    data = (struct ActorData *)obj->data;
    if (data != 0)
    {
        data->lock = 0;
    }
    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    old = obj->step;
    obj->step = old + 1;
    if ((s16)old > 0x13)
    {
        end_action(obj);
    }
}

void orbit_timer_a(struct Actor *obj)
{
    struct Actor *child;
    u8 *p;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    if (obj->timer > 0)
    {
        obj->timer = obj->timer - 1;
        return;
    }
    child = obj->child;
    child->angle = 0xB400;
    child->angleSpeed = -0x1CC;
    sprite_set_action(obj, 2, 0, 0);
    p = (u8 *)obj->pose + 0x12;
    *p = *p | 0x10;
    obj->state = random_jump_orbit_circle_a;
}

void random_jump_end_action_if_pose_visible(struct Actor *obj)
{
    if (obj->flags76 & 0x38)
    {
        end_action(obj);
    }
    else if (obj->pose->visible)
    {
        end_action(obj);
    }
}

u32 random_jump_setup_slot_spawn(struct Actor *obj)
{
    s32 index;
    u32 *dst;
    u8 *entry;

    obj->switch6 = 0;
    obj->switch5 = 0;
    dst = (u32 *)(obj->data + 0x114);
    entry = gParty;
    index = ~*(s16 *)(obj->data + 0xEC);
    entry = entry + 0x3C * index;
    *dst = *(u16 *)(entry + 0x2C);
    obj->state = random_jump_dispatch_slot_spawn;
    obj->callback = end_action;
    return 1;
}

void random_jump_start_jump_arc_f(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    obj->pose->mode = 1;
    obj->state = hold_orbit_arc_rise_f;
}

void random_jump_timer_arc_f(struct Actor *obj)
{
    obj->timer = obj->timer - 1;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        random_jump_start_jump_arc_f(obj);
    }
}

void random_jump_start_jump_arc_e(struct Actor *obj)
{
    set_slot_value(0x2B, -1);
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    obj->pose->mode = 1;
    obj->state = hold_orbit_arc_rise_e;
}

void random_jump_timer_arc_e(struct Actor *obj)
{
    obj->timer = obj->timer - 1;
    if (obj->timer <= 0)
    {
        obj->pose->result = 0x10;
        random_jump_start_jump_arc_e(obj);
    }
}

u32 random_jump_dispatch_slot_random_jump(struct Actor *obj)
{
    s16 slot;

    slot = *(s16 *)(obj->data + 0xEC);
    if (slot == -1)
    {
        random_jump_a(obj);
    }
    else if (slot == -2)
    {
        random_jump_b(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

void random_jump_dispatch_slot_spawn(struct Actor *obj)
{
    if (*(s16 *)(obj->data + 0xEC) == -1)
    {
        random_jump_spawn_orbit_child_a(obj);
    }
    else
    {
        random_jump_spawn_orbit_child_b(obj);
    }
}

void hold_orbit_arc_rise_f(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj->state = arc_land_f;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            obj->pose->mode = 1;
            obj->phase = 1;
        }
    }
}

void hold_orbit_arc_rise_e(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj->state = hold_orbit_arc_land_e;
        if ((u8)try_park_in_slot(obj) == 0)
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            obj->pose->mode = 1;
            obj->phase = 1;
        }
    }
}

void arc_land_f(struct Actor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->h = obj->baseH + obj->dh;
    if (obj->h <= *(s32 *)(obj->data + 0xE0))
    {
        obj->phase = 0;
        obj->h = *(s32 *)(obj->data + 0xE0);
        sprite_set_action(obj, 8, 0x204D, 0);
        obj->pose->mode = 1;
        obj->state = finish_if_pose_visible_g;
    }
}
