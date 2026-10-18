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

struct PoseMode
{
    u8 filler0[0x12];
    u8 unk12_0 : 1;
    u8 mode : 2;
    u8 unk12_3 : 1;
    u8 hold : 1;
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
            obj->step = obj->step * 3;
            obj->state = hold_decay_hold_release_a;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->step = action;
        obj->state = hold_decay_hold_idle_b;
    }
    ((struct PoseMode *)obj->pose)->hold = 1;
    obj->step++;
}

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

void hold_decay_hold_idle_b(struct Actor *obj)
{
    u32 pressed;
    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    pressed = gSys.held & obj->buttons;
    if (pressed == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 10;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->state = hold_decay_timer_start_orbit_b;
        obj->step = pressed;
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

void hold_decay_orbit_step_b(struct Actor *obj)
{
    struct Actor *child;
    u32 idx;
    s32 h;
    const s16 *table;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActionBits *)child)->mode = 1;
    child->angle += child->angleSpeed;
    idx = MATH_ROUTINE(u32 (*)(s32, s32), iwram_div)(child->angle, 360);
    table = gCosineTable;
    child->x = obj->baseX + (table[idx & 0xFF] / 64) * 24;
    h = obj->baseH + 0x1A00;
    child->h = h + (gSineTable[idx & 0xFF] / 64) * 24;
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

u32 random_jump_c(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;
    s32 (*fn)(u32);

    obj->anim = gAnim278;
    base = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(6, mt19937_next()) + 4;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = hold_decay_jump_timer_arc_d;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_d;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    obj->dh = (fn(0x006E1E40) - 0x58) / 2;
    return 1;
}

u32 random_jump_d(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;
    s32 (*fn)(u32);

    obj->anim = gAnim374;
    base = gParty;
    base += 0x3C;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(11, mt19937_next()) + 5;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = hold_decay_jump_timer_arc_c;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_c;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    obj->dh = (fn(0x005A1440) - 0x48) / 2;
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

u32 random_jump_e(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;
    s32 (*fn)(u32);

    obj->anim = gAnim278;
    base = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(5, mt19937_next()) + 7;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = hold_decay_jump_timer_arc_f;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_f;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    obj->dh = (fn(0x006E1E40) - 0x58) / 2;
    return 1;
}

u32 random_jump_f(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;
    s32 (*fn)(u32);

    obj->anim = gAnim374;
    base = gParty;
    base += 0x3C;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(11, mt19937_next()) + 5;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = hold_decay_jump_timer_arc_e;
    }
    else
    {
        obj->state = hold_decay_start_jump_arc_e;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    obj->dh = (fn(0x005A1440) - 0x48) / 2;
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

u32 random_pause_jump_c(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;
    s32 (*fn)(u32);

    obj->anim = gAnim278;
    base = gParty;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(4, mt19937_next()) + 2;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = timer_start_jump_d;
    }
    else
    {
        obj->state = charge_release_start_jump_arc_d;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    obj->dh = (fn(0x006E1E40) - 0x58) / 2;
    return 1;
}

u32 random_pause_jump_d(struct Actor *obj)
{
    s32 timer;
    u8 *base;
    struct Data *data;
    s32 (*fn)(u32);

    obj->anim = gAnim374;
    base = gParty;
    base += 0x3C;
    obj->unk77_6 = 0;
    obj->unk77_5 = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(base + 0x2A);
    timer = rand_below(11, mt19937_next()) + 5;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseMode *)obj->pose)->mode = 0;
        obj->timer = timer;
        obj->state = timer_start_jump_c;
    }
    else
    {
        obj->state = charge_release_start_jump_arc_c;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    obj->dh = (fn(0x005A1440) - 0x48) / 2;
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
