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

s32 hold_orbit_random_pause_jump_a(struct Obj *obj)
{
    struct Pose *pose;
    struct Data *data;
    u8 *list;
    u32 timer;
    void (*next)(struct Obj *);
    u32 (*ram)(u32);
    s32 q;

    obj->anim = (u32)gAnim278;
    list = gParty;
    ((struct ObjFlagBits *)&obj->flags)->switchB = 0;
    ((struct ObjFlagBits *)&obj->flags)->switchA = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(list + 0x2A);
    timer = rand_below(8, mt19937_next()) + 8;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        pose = obj->pose;
        ((struct PoseBits *)&pose->flags)->mode = 0;
        obj->timer = timer;
        next = timer_start_jump_b;
    }
    else
    {
        next = charge_release_start_jump_arc_b;
    }
    obj->state = next;
    ram = (u32 (*)(u32))gMathRoutines;
    q = ram(0x006E1E40);
    obj->dh = (q - 0x58) / 2;
    return 1;
}

s32 hold_orbit_random_pause_jump_b(struct Obj *obj)
{
    struct Pose *pose;
    struct Data *data;
    u8 *list;
    u32 timer;
    void (*next)(struct Obj *);
    u32 (*ram)(u32);
    s32 q;

    obj->anim = (u32)gAnim374;
    list = gParty;
    list += 0x3C;
    ((struct ObjFlagBits *)&obj->flags)->switchB = 0;
    ((struct ObjFlagBits *)&obj->flags)->switchA = 0;
    data = obj->data;
    data->slotValue = *(u16 *)(list + 0x2A);
    timer = rand_below(0x16, mt19937_next()) + 0xA;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        pose = obj->pose;
        ((struct PoseBits *)&pose->flags)->mode = 0;
        obj->timer = timer;
        next = timer_start_jump_a;
    }
    else
    {
        next = charge_release_start_jump_arc_a;
    }
    obj->state = next;
    ram = (u32 (*)(u32))gMathRoutines;
    q = ram(0x005A1440);
    obj->dh = (q - 0x48) / 2;
    return 1;
}

void hold_orbit_spawn_orbit_child_b(struct Obj *obj)
{
    struct Obj *child;
    struct Data *data;
    s32 x;
    s32 y;
    s32 h;
    u8 *act;

    data = obj->data;
    x = obj->baseX;
    if (x < 0)
    {
        x += 0xFF;
    }
    x >>= 8;
    y = obj->baseY;
    if (y < 0)
    {
        y += 0xFF;
    }
    y >>= 8;
    h = obj->baseH;
    if (h < 0)
    {
        h += 0xFF;
    }
    h >>= 8;
    child = obj_spawn(data, 0, x, y, h, 0, 1);
    obj->child = child;
    act = &child->action;
    *act = *act | 6;
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
    hold_charge_a(obj);
}

struct PoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 busy : 1;
    u8 rest : 3;
};

void hold_charge_a(struct Obj *obj)
{
    u32 action;

    obj->state = hold_charge_a;
    action = obj->action & 0x38;
    if (action != 0)
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
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->step = action;
        obj->state = charge_release_hold_idle_a;
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step++;
}

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

void charge_release_hold_idle_a(struct Obj *obj)
{
    u32 action;
    u32 keys;

    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    keys = gSys.held & obj->buttons;
    if (keys == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 0xA;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->state = charge_release_timer_start_orbit_a;
        obj->step = keys;
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

void charge_release_orbit_circle_a(struct Obj *obj)
{
    struct Obj *child;
    u32 index;
    s32 h;
    const s16 *table;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct PoseBits *)&child->action)->mode = 1;
    child->angle += child->velocity;
    index = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(child->angle, 0x168);
    table = gCosineTable;
    child->x = obj->baseX + (table[index & 0xFF] / 64) * 0x16;
    h = obj->baseH + 0x1600;
    child->h = h + (gSineTable[index & 0xFF] / 64) * 0x16;
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

void charge_release_spawn_orbit_child_a(struct Obj *obj)
{
    struct Obj *child;
    struct Data *data;
    s32 x;
    s32 y;
    s32 h;
    u8 *act;

    data = obj->data;
    x = obj->baseX;
    if (x < 0)
    {
        x += 0xFF;
    }
    x >>= 8;
    y = obj->baseY;
    if (y < 0)
    {
        y += 0xFF;
    }
    y >>= 8;
    h = obj->baseH;
    if (h < 0)
    {
        h += 0xFF;
    }
    h >>= 8;
    child = obj_spawn(data, 0, x, y, h, 0, 1);
    obj->child = child;
    act = &child->action;
    *act = *act | 6;
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

void charge_release_hold_step_a(struct Obj *obj)
{
    u32 action;

    obj->state = charge_release_hold_step_a;
    action = obj->action & 0x38;
    if (action != 0)
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
    if (obj->step != 0xC)
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
        obj->step = action;
        obj->state = charge_release_hold_idle_b;
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step++;
}

void hold_release_b(struct Obj *obj)
{
    u32 action;

    obj->state = hold_release_b;
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
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xC);
        ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->state = charge_release_end_action_if_busy;
        return;
    }
    if (obj->step <= 8)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2054, 0);
    }
    else if (obj->step <= 0x11)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2054, 1);
    }
    else if (obj->step <= 0x1A)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2054, 2);
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step -= 3;
}

void charge_release_hold_idle_b(struct Obj *obj)
{
    u32 action;
    u32 keys;

    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    keys = gSys.held & obj->buttons;
    if (keys == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 0xA;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->state = charge_release_timer_start_orbit_b;
        obj->step = keys;
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

void charge_release_orbit_circle_b(struct Obj *obj)
{
    struct Obj *child;
    const s16 *table;
    u32 index;
    s32 h;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct PoseBits *)&child->action)->mode = 1;
    child->angle += child->velocity;
    index = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(child->angle, 0x168);
    table = gCosineTable;
    child->x = obj->baseX + (table[index & 0xFF] / 64) * 0x18;
    h = obj->baseH + 0x1A00;
    child->h = h + (gSineTable[index & 0xFF] / 64) * 0x18;
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

void charge_release_spawn_orbit_child_c(struct Obj *obj)
{
    struct Obj *child;
    struct Data *data;
    s32 x;
    s32 y;
    s32 h;
    u8 *act;

    data = obj->data;
    x = obj->baseX;
    if (x < 0)
    {
        x += 0xFF;
    }
    x >>= 8;
    y = obj->baseY;
    if (y < 0)
    {
        y += 0xFF;
    }
    y >>= 8;
    h = obj->baseH;
    if (h < 0)
    {
        h += 0xFF;
    }
    h >>= 8;
    child = obj_spawn(data, 0, x, y, h, 0, 1);
    obj->child = child;
    act = &child->action;
    *act = *act | 6;
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
    hold_charge_b(obj);
}

void hold_charge_b(struct Obj *obj)
{
    u32 action;

    obj->state = hold_charge_b;
    action = obj->action & 0x38;
    if (action != 0)
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
            obj->state = hold_release_c;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->step = action;
        obj->state = hold_decay_hold_idle_a;
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->busy = 1;
    obj->step++;
}

void hold_release_c(struct Obj *obj)
{
    u32 action;

    obj->state = hold_release_c;
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

void hold_decay_hold_idle_a(struct Obj *obj)
{
    u32 action;
    u32 keys;

    action = obj->action & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    keys = gSys.held & obj->buttons;
    if (keys == 0)
    {
        obj->timer = rand_below(5, mt19937_next()) + 0xA;
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        obj->state = hold_decay_timer_start_orbit_a;
        obj->step = keys;
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
        obj->state = hold_decay_end_action_if_busy;
    }
    else
    {
        if (obj->step > 0x18)
            ((struct PoseFlagBits *)&obj->pose->flags)->busy = 0;
        obj->step++;
    }
}

void hold_decay_orbit_step_a(struct Obj *obj)
{
    struct Obj *child;
    const s16 *table;
    u32 index;
    s32 h;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct PoseBits *)&child->action)->mode = 1;
    child->angle += child->velocity;
    index = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(child->angle, 0x168);
    table = gCosineTable;
    child->x = obj->baseX + (table[index & 0xFF] / 64) * 0x16;
    h = obj->baseH + 0x1600;
    child->h = h + (gSineTable[index & 0xFF] / 64) * 0x16;
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

void hold_decay_spawn_orbit_child(struct Obj *obj)
{
    struct Obj *child;
    struct Data *data;
    s32 x;
    s32 y;
    s32 h;
    u8 *act;

    data = obj->data;
    x = obj->baseX;
    if (x < 0)
    {
        x += 0xFF;
    }
    x >>= 8;
    y = obj->baseY;
    if (y < 0)
    {
        y += 0xFF;
    }
    y >>= 8;
    h = obj->baseH;
    if (h < 0)
    {
        h += 0xFF;
    }
    h >>= 8;
    child = obj_spawn(data, 0, x, y, h, 0, 1);
    obj->child = child;
    act = &child->action;
    *act = *act | 6;
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
