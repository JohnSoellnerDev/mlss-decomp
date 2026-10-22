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

extern struct KeyState gSys;

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

extern struct WorldMode *gWorld;

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
extern u8 *gParty;
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

void jump_orbit_arc_rise_c(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = jump_orbit_arc_land_c;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct FlagsPhase *)&obj->flags)->phase = 1;
        }
    }
}

void jump_orbit_arc_rise_d(struct HoldActor *obj)
{
    obj->dh = obj->dh + obj->speed;
    obj->height = obj->baseH + (s16)obj->dh;
    if ((s16)obj->dh <= 0)
    {
        obj->state = jump_orbit_arc_land_d;
        if (!try_park_in_slot(obj))
        {
            sprite_set_action(obj, 6, 0x2000, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            ((struct FlagsPhase *)&obj->flags)->phase = 1;
        }
    }
}

void jump_orbit_arc_land_c(struct HoldActor *obj)
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
        obj->state = jump_orbit_finish_if_pose_visible_c;
    }
}

void jump_orbit_arc_land_d(struct HoldActor *obj)
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
        obj->state = jump_orbit_finish_if_pose_visible_d;
    }
}

void jump_orbit_finish_if_pose_visible_c(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x204D, 0);
        obj->state = 0;
    }
}

void jump_orbit_finish_if_pose_visible_d(struct HoldActor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0x2000, 0);
        obj->state = 0;
    }
}

void jump_orbit_spawn_orbit_child_c(struct HoldActor *obj)
{
    struct HoldActor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, 0, 1);
    obj->child = child;
    ((struct ActionBits *)&child->action)->mode = 3;
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xFD;
    child->unk49 = 3;
    ((struct FlagsPhase *)&child->flags)->phase = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    ((struct ActionBit6 *)&child->action)->bit6 = 1;
    child->callback = on_arrive;
    ((struct ChildFlagBits *)&child->childFlags)->first = 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    orbit_step_e(obj);
}

NONMATCH("asm/code_0808F758/orbit_step_e.s", void orbit_step_e(struct HoldActor *obj))
{
    s32 busy;
    u8 act;

    obj->state = orbit_step_e;
    act = obj->action;
    busy = 0x38;
    busy &= act;
    if (busy != 0)
    {
        end_action(obj);
    }
    else
    {
        if ((s16)obj->step == 0)
        {
            sprite_set_action(obj, 0, 0x2025, 0);
        }
        if ((s16)obj->step == 1)
        {
            sprite_set_action(obj, 0, 0x2025, 1);
        }
        if ((s16)obj->step == 2)
        {
            sprite_set_action(obj, 0, 0x2025, 2);
        }
        if ((s16)obj->step == 3)
        {
            sprite_set_action(obj, 0, 0x2025, 3);
        }
        if ((s16)obj->step != 4)
        {
            if ((gSys.held & obj->buttons) == 0)
            {
                obj->state = orbit_wait_c;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 1, 0, 0);
            obj->step = busy;
            obj->state = hold_wait_a;
        }
        ((struct PoseHoldBit *)&obj->pose->flags)->hold = 1;
        obj->step++;
    }
}
END_NONMATCH
