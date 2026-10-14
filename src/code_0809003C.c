#include "global.h"
#include "ram_routines.h"

struct LockBits
{
    u8 low : 2;
    u8 bit2 : 1;
    u8 high : 5;
};

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gUnk_0300034C;

struct ActionBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActionBit6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ChildFlagBits
{
    u8 first : 1;
    u8 rest : 7;
};

struct WorldMode
{
    u8 filler0[0x9];
    u8 mode;
};

extern struct WorldMode *gUnk_03000FD8;

struct PoseHoldBit
{
    u8 low : 4;
    u8 hold : 1;
    u8 high : 3;
};

struct PoseFlagBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct FlagsSwitch
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct FlagsPhase
{
    u8 phase : 3;
    u8 rest : 5;
};

struct HoldPose
{
    u8 filler0[0x11];
    u8 turn;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 result;
    u8 filler21[0x23 - 0x21];
    u8 count;
};

struct HoldData
{
    u8 filler0[0xE0];
    s32 ground;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
    u8 filler0EE[0x111 - 0xEE];
    u8 lock;
    u8 filler112[0x114 - 0x112];
    u32 slotValue;
    u8 filler118[0x12D - 0x118];
    u8 kind;
};

struct HoldActor
{
    u8 filler0[0x8];
    struct HoldPose *pose;
    u8 filler0C[0x10 - 0xC];
    s32 childX;
    u8 filler14[0x18 - 0x14];
    s32 height;
    u8 filler1C[0x28 - 0x1C];
    struct HoldData *data;
    u8 filler2C[0x30 - 0x2C];
    struct HoldActor *child;
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
    void (*state)(struct HoldActor *obj);
    u8 filler50[0x68 - 0x50];
    void (*callback)(struct HoldActor *obj);
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 action;
    u8 flags;
    u8 filler78[0x79 - 0x78];
    u8 childFlags;
    u8 filler7A[0x84 - 0x7A];
    s32 targetX;
    u8 filler88[0x8C - 0x88];
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 speed;
    u8 filler98[0x9C - 0x98];
    s32 timer;
    s32 childAngle;
    s32 childSpeed;
    u32 buttons;
    u16 step;
    u8 filler0AE[0xB2 - 0xAE];
    u16 dh;
};

void sprite_set_action(struct HoldActor *obj, u32 action, u32 arg, u32 extra);
void hold_orbit_start_jump_arc_a(struct HoldActor *obj);
void hold_orbit_start_jump_arc_b(struct HoldActor *obj);
void jump_orbit_spawn_orbit_child_c(struct HoldActor *obj);
void hold_orbit_spawn_orbit_child_a(struct HoldActor *obj);
void end_action(struct HoldActor *obj);
void on_arrive(struct HoldActor *obj);
void random_jump_setup_list_slot_a(struct HoldActor *obj);
void random_jump_setup_list_slot_b(struct HoldActor *obj);
s32 slot_begin_a(struct HoldActor *obj);
s32 slot_begin_b(struct HoldActor *obj);
s32 slot_begin_c(struct HoldActor *obj);
s32 slot_begin_d(struct HoldActor *obj);
void timer_wait_a(struct HoldActor *obj);
void timer_wait_b(struct HoldActor *obj);
void hold_orbit_start_jump_arc_c(struct HoldActor *obj);
void hold_orbit_start_jump_arc_d(struct HoldActor *obj);
void dispatch_slot_orbit(struct HoldActor *obj);
void set_slot_value(u32 id, s32 value);
void hold_orbit_arc_rise_a(struct HoldActor *obj);
void hold_orbit_arc_rise_b(struct HoldActor *obj);
void hold_orbit_arc_rise_c(struct HoldActor *obj);
void hold_orbit_arc_rise_d(struct HoldActor *obj);
void jump_orbit_arc_land_c(struct HoldActor *obj);
void jump_orbit_arc_land_d(struct HoldActor *obj);
void hold_orbit_arc_land_a(struct HoldActor *obj);
void hold_orbit_arc_land_b(struct HoldActor *obj);
void hold_orbit_arc_land_c(struct HoldActor *obj);
void hold_orbit_arc_land_d(struct HoldActor *obj);
u8 try_park_in_slot(struct HoldActor *obj);
void fall_a(struct HoldActor *obj);
void fall_b(struct HoldActor *obj);
struct HoldActor *obj_spawn(struct HoldData *data, u32 kind, s32 x, s32 y, s32 h, s32 a, s32 b);
void orbit_step_e(struct HoldActor *obj);
void orbit_start_a(struct HoldActor *obj);
extern const u8 gAnim284[];
void orbit_wait_c(struct HoldActor *obj);
void hold_wait_a(struct HoldActor *obj);
void hold_countdown_a(struct HoldActor *obj);
void hold_wait_b(struct HoldActor *obj);
void hold_release(struct HoldActor *obj);
void hold_orbit_circle_a(struct HoldActor *obj);
void hold_orbit_circle_b(struct HoldActor *obj);
void orbit_end_wait_a(struct HoldActor *obj);
void orbit_end_wait_b(struct HoldActor *obj);
extern u8 *gUnk_03000FF4;
extern const u8 gAnim278[];
extern const u8 gAnim374[];
void jump_orbit_finish_if_pose_visible_c(struct HoldActor *obj);
void jump_orbit_finish_if_pose_visible_d(struct HoldActor *obj);
void hold_orbit_finish_if_pose_visible_a(struct HoldActor *obj);
void hold_orbit_finish_if_pose_visible_b(struct HoldActor *obj);
void hold_orbit_finish_if_pose_visible_c(struct HoldActor *obj);
void hold_orbit_finish_if_pose_visible_d(struct HoldActor *obj);
void hold_orbit_finish_if_pose_visible_e(struct HoldActor *obj);
void finish_if_pose_visible_f(struct HoldActor *obj);

void hold_countdown_a(struct HoldActor *obj)
{
    obj->state = hold_countdown_a;
    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    if ((s16)obj->step < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 6);
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 0;
        obj->state = hold_release;
        return;
    }
    else if ((s16)obj->step <= 1)
    {
        if (obj->pose->count != 0)
        {
            sprite_set_action(obj, 0, 0x2054, 0);
        }
    }
    else if ((s16)obj->step <= 3)
    {
        if (obj->pose->count > 1)
        {
            sprite_set_action(obj, 0, 0x2054, 1);
        }
    }
    else if ((s16)obj->step <= 5)
    {
        if (obj->pose->count > 2)
        {
            sprite_set_action(obj, 0, 0x2054, 2);
        }
    }
    ((struct PoseHoldBit *)&obj->pose->flags)->hold = 1;
    obj->step -= 2;
}

void hold_wait_b(struct HoldActor *obj)
{
    struct HoldActor *child;
    u32 keys;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    keys = gUnk_0300034C.held & obj->buttons;
    if (keys == 0)
    {
        obj->timer = 1;
        child = obj->child;
        child->childAngle = 0xB400;
        child->childSpeed = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 1;
        obj->state = hold_orbit_circle_b;
        obj->step = keys;
        return;
    }
    if ((s16)obj->step > 0x59)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gUnk_03000FD8->mode & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 6);
            }
            else
            {
                obj->step = 0x5A;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 0;
        obj->state = hold_release;
        return;
    }
    if ((s16)obj->step > 0x3B)
    {
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 0;
    }
    obj->step++;
}

void hold_orbit_circle_b(struct HoldActor *obj)
{
    struct HoldActor *child;
    u32 idx;
    s32 base;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActionBits *)&child->action)->mode = 1;
    child->childAngle += child->childSpeed;
    child->childX = obj->baseX + 24 * (gCosineTable[idx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->childAngle, 0x168) & 0xFF] / 64);
    base = obj->baseH + 0x1A00;
    child->height = base + 24 * (gSineTable[idx] / 64);
    if (obj->pose->turn & 0x40)
    {
        child->childSpeed += 0x998;
    }
    else
    {
        child->childSpeed += -0x998;
    }
    if ((s16)obj->step == 3)
    {
        sprite_set_action(obj, 2, 0x2054, 1);
    }
    if ((s16)obj->step == 5)
    {
        sprite_set_action(obj, 2, 0x2054, 2);
    }
    if ((s16)obj->step == 6)
    {
        sprite_set_action(obj, 2, 0x2054, 3);
    }
    if ((s16)obj->step == 7)
    {
        sprite_set_action(obj, 2, 0x2054, 4);
    }
    if ((s16)obj->step == 8)
    {
        sprite_set_action(obj, 2, 0x2054, 5);
    }
    if ((s16)obj->step == 9)
    {
        ((struct ActionBits *)&child->action)->mode = 3;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = orbit_end_wait_b;
        if (obj->data != 0)
        {
            ((struct LockBits *)&obj->data->lock)->bit2 = 1;
        }
    }
    else
    {
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 1;
        obj->step++;
    }
}

void orbit_end_wait_b(struct HoldActor *obj)
{
    u16 step;

    if (obj->data != 0)
    {
        ((struct LockBits *)&obj->data->lock)->bit2 = 0;
    }
    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else
    {
        step = obj->step;
        obj->step = step + 1;
        if ((s16)step > 0x17)
        {
            end_action(obj);
        }
    }
}

void orbit_end_wait_a(struct HoldActor *obj)
{
    u16 step;

    if (obj->data != 0)
    {
        ((struct LockBits *)&obj->data->lock)->bit2 = 0;
    }
    if (obj->action & 0x38)
    {
        end_action(obj);
    }
    else
    {
        step = obj->step;
        obj->step = step + 1;
        if ((s16)step > 0x13)
        {
            end_action(obj);
        }
    }
}

void hold_release(struct HoldActor *obj)
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

s32 list_slot_start(struct HoldActor *obj)
{
    u32 *slotValue;
    u8 *entry;

    ((struct FlagsSwitch *)&obj->flags)->bit6 = 0;
    ((struct FlagsSwitch *)&obj->flags)->bit5 = 0;
    slotValue = &obj->data->slotValue;
    entry = gUnk_03000FF4;
    entry += (~obj->data->slot) * 0x3C;
    *slotValue = *(u16 *)(entry + 0x2C);
    obj->state = dispatch_slot_orbit;
    obj->callback = end_action;
    return 1;
}

s32 dispatch_slot_hold(struct HoldActor *obj)
{
    if (obj->data->slot == -1)
    {
        slot_begin_b(obj);
    }
    else if (obj->data->slot == -2)
    {
        slot_begin_a(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

void dispatch_slot_orbit(struct HoldActor *obj)
{
    if (obj->data->slot == -1)
    {
        jump_orbit_spawn_orbit_child_c(obj);
    }
    else
    {
        hold_orbit_spawn_orbit_child_a(obj);
    }
}

s32 slot_begin_a(struct HoldActor *obj)
{
    u8 *entry;

    obj->anim = gAnim374;
    entry = gUnk_03000FF4 + 0x3C;
    ((struct FlagsSwitch *)&obj->flags)->bit6 = 0;
    ((struct FlagsSwitch *)&obj->flags)->bit5 = 0;
    obj->data->slotValue = *(u16 *)(entry + 0x2A);
    sprite_set_action(obj, 4, 0x204D, 0);
    obj->pose->result = 0x10;
    obj->timer = 6;
    obj->state = timer_wait_a;
    return 1;
}

s32 slot_begin_b(struct HoldActor *obj)
{
    u8 *entry;

    obj->anim = gAnim278;
    entry = gUnk_03000FF4;
    ((struct FlagsSwitch *)&obj->flags)->bit6 = 0;
    ((struct FlagsSwitch *)&obj->flags)->bit5 = 0;
    obj->data->slotValue = *(u16 *)(entry + 0x2A);
    sprite_set_action(obj, 4, 0x2000, 0);
    obj->pose->result = 0x30;
    obj->timer = 2;
    obj->state = timer_wait_b;
    return 1;
}

void timer_wait_a(struct HoldActor *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if (timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_orbit_start_jump_arc_a(obj);
    }
}

void timer_wait_b(struct HoldActor *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if (timer <= 0)
    {
        obj->pose->result = 0x10;
        hold_orbit_start_jump_arc_b(obj);
    }
}

void hold_orbit_start_jump_arc_a(struct HoldActor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x002E4559);
    obj->dh = (start - 0x25) / 2;
    obj->speed = -0x25;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = hold_orbit_arc_rise_a;
}

void hold_orbit_start_jump_arc_b(struct HoldActor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x00318790);
    obj->dh = (start - 0x2C) / 2;
    obj->speed = -0x2C;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = hold_orbit_arc_rise_b;
}

void hold_orbit_arc_rise_a(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_orbit_arc_land_a;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct FlagsPhase *)&obj->flags)->phase = 1;
        }
    }
}

void hold_orbit_arc_rise_b(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_orbit_arc_land_b;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct FlagsPhase *)&obj->flags)->phase = 1;
        }
    }
}

void hold_orbit_arc_land_a(struct HoldActor *obj)
{
    s32 height;

    obj->dh = obj->dh + obj->speed;
    height = obj->baseH + (s16)obj->dh;
    obj->height = height;
    if (height <= obj->data->ground)
    {
        ((struct FlagsPhase *)&obj->flags)->phase = 0;
        obj->height = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        obj->state = hold_orbit_finish_if_pose_visible_a;
    }
}

void hold_orbit_arc_land_b(struct HoldActor *obj)
{
    s32 height;

    obj->dh = obj->dh + obj->speed;
    height = obj->baseH + (s16)obj->dh;
    obj->height = height;
    if (height <= obj->data->ground)
    {
        ((struct FlagsPhase *)&obj->flags)->phase = 0;
        obj->height = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        obj->state = hold_orbit_finish_if_pose_visible_b;
    }
}

void hold_orbit_finish_if_pose_visible_a(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void hold_orbit_finish_if_pose_visible_b(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

void enter_fall_a(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if (gUnk_0300034C.held & 1)
    {
        obj->speed = obj->speed + ((-0x7A - obj->speed) * 10) / 256;
    }
    else
    {
        obj->speed = -0x7A;
    }
    if ((s16)obj->dh <= 0)
    {
        sprite_set_action(obj, 6, 0x2000, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        ((struct FlagsPhase *)&obj->flags)->phase = 1;
        obj->state = fall_a;
    }
}

void fall_a(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if (gUnk_0300034C.held & 1)
    {
        obj->speed = obj->speed + ((-0x7A - obj->speed) * 10) / 256;
    }
    else
    {
        obj->speed = -0x7A;
    }
    if (obj->height <= obj->data->ground)
    {
        ((struct FlagsPhase *)&obj->flags)->phase = 0;
        obj->height = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        obj->state = hold_orbit_finish_if_pose_visible_d;
    }
}

void enter_fall_b(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if (gUnk_0300034C.held & 2)
    {
        obj->speed = obj->speed + ((-0x7A - obj->speed) * 10) / 256;
    }
    else
    {
        obj->speed = -0x7A;
    }
    if ((s16)obj->dh <= 0)
    {
        sprite_set_action(obj, 6, 0x204D, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        ((struct FlagsPhase *)&obj->flags)->phase = 1;
        obj->state = fall_b;
    }
}

void fall_b(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if (gUnk_0300034C.held & 2)
    {
        obj->speed = obj->speed + ((-0x7A - obj->speed) * 10) / 256;
    }
    else
    {
        obj->speed = -0x7A;
    }
    if (obj->height <= obj->data->ground)
    {
        ((struct FlagsPhase *)&obj->flags)->phase = 0;
        obj->height = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        obj->state = hold_orbit_finish_if_pose_visible_c;
    }
}

void hold_orbit_finish_if_pose_visible_c(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void hold_orbit_finish_if_pose_visible_d(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

s32 dispatch_slot_jump(struct HoldActor *obj)
{
    if (obj->data->slot == -1)
    {
        slot_begin_d(obj);
    }
    else if (obj->data->slot == -2)
    {
        slot_begin_c(obj);
    }
    obj->callback = on_arrive;
    return 1;
}

s32 slot_begin_c(struct HoldActor *obj)
{
    u8 *entry;

    obj->anim = gAnim374;
    entry = gUnk_03000FF4 + 0x3C;
    ((struct FlagsSwitch *)&obj->flags)->bit6 = 0;
    ((struct FlagsSwitch *)&obj->flags)->bit5 = 0;
    obj->data->slotValue = *(u16 *)(entry + 0x2A);
    obj->state = hold_orbit_start_jump_arc_c;
    return 1;
}

s32 slot_begin_d(struct HoldActor *obj)
{
    u8 *entry;

    obj->anim = gAnim278;
    entry = gUnk_03000FF4;
    ((struct FlagsSwitch *)&obj->flags)->bit6 = 0;
    ((struct FlagsSwitch *)&obj->flags)->bit5 = 0;
    obj->data->slotValue = *(u16 *)(entry + 0x2A);
    obj->state = hold_orbit_start_jump_arc_d;
    return 1;
}

void hold_orbit_start_jump_arc_c(struct HoldActor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x003F1440);
    obj->dh = (start - 0x48) / 2;
    obj->speed = -0x48;
    sprite_set_action(obj, 5, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = hold_orbit_arc_rise_c;
}

void hold_orbit_start_jump_arc_d(struct HoldActor *obj)
{
    s32 start;
    s32 (*fn)(u32);

    set_slot_value(0x2B, -1);
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x004D1E40);
    obj->dh = (start - 0x58) / 2;
    obj->speed = -0x58;
    sprite_set_action(obj, 5, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = hold_orbit_arc_rise_d;
}

void hold_orbit_arc_rise_c(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_orbit_arc_land_c;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct FlagsPhase *)&obj->flags)->phase = 1;
        }
    }
}

void hold_orbit_arc_rise_d(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = hold_orbit_arc_land_d;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct FlagsPhase *)&obj->flags)->phase = 1;
        }
    }
}

void hold_orbit_arc_land_c(struct HoldActor *obj)
{
    s32 height;

    obj->dh = obj->dh + obj->speed;
    height = obj->baseH + (s16)obj->dh;
    obj->height = height;
    if (height <= obj->data->ground)
    {
        ((struct FlagsPhase *)&obj->flags)->phase = 0;
        obj->height = obj->data->ground;
        sprite_set_action(obj, 8, 0x204D, 0);
        obj->state = hold_orbit_finish_if_pose_visible_e;
    }
}

void hold_orbit_arc_land_d(struct HoldActor *obj)
{
    s32 height;

    obj->dh = obj->dh + obj->speed;
    height = obj->baseH + (s16)obj->dh;
    obj->height = height;
    if (height <= obj->data->ground)
    {
        ((struct FlagsPhase *)&obj->flags)->phase = 0;
        obj->height = obj->data->ground;
        sprite_set_action(obj, 8, 0x2000, 0);
        obj->state = finish_if_pose_visible_f;
    }
}

void hold_orbit_finish_if_pose_visible_e(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void finish_if_pose_visible_f(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

s32 dispatch_slot_list(struct HoldActor *obj)
{
    if (obj->data->slot == -1)
    {
        random_jump_setup_list_slot_b(obj);
    }
    else if (obj->data->slot == -2)
    {
        random_jump_setup_list_slot_a(obj);
    }
    obj->callback = on_arrive;
    return 1;
}
