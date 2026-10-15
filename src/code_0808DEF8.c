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
    s32 unk10;
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
    s32 unk84;
    u8 filler88[0x8C - 0x88];
    s32 unk8C;
    u8 filler90[0x94 - 0x90];
    s32 speed;
    u8 filler98[0x9C - 0x98];
    s32 timer;
    s32 unkA0;
    s32 unkA4;
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

extern u8 *gUnk_03000FF4;
extern u8 *gUnk_03000FD8;
struct SysState
{
    u8 filler0[0x3C];
    u16 keysHeld;
};

extern struct SysState gUnk_0300034C;
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

void hold_then_orbit_b(struct Actor *obj)
{
    u32 keys;
    s16 *stepp;

    if (poll_action_end(obj) != 0)
        return;
    keys = gUnk_0300034C.keysHeld & obj->keyMask;
    if (keys == 0)
    {
        obj->timer = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        *(u8 *)&obj->pose->flags |= 0x10;
        obj->state = orbit_position_b;
        obj->step = keys;
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp > 0x2C)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gUnk_03000FD8[9] & 0xF0) == 0x10)
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
    if ((s16)*stepp > 0x1D)
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->step = obj->step + 1;
}

void orbit_position_b(struct Actor *obj)
{
    struct Actor *child;
    const s16 *cosp;
    s16 *stepp;
    u32 idx;
    s32 angle;
    s32 height;

    if (poll_action_end(obj) != 0)
        return;
    child = obj->child;
    ((struct ActBits *)&child->action)->mode = 1;
    child->unkA0 += child->unkA4;
    angle = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->unkA0, 0x168);
    cosp = gCosineTable;
    idx = angle & 0xFF;
    child->unk10 = obj->baseX + (cosp[idx] / 0x40) * 0x18;
    height = obj->baseH + 0x1A00;
    child->h = height + (gSineTable[idx] / 0x40) * 0x18;
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
        *(u8 *)&child->action |= 6;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = finish_orbit_b;
        if (obj->data != NULL)
            *(u8 *)&obj->data->lock |= 4;
        return;
    }
    *(u8 *)&obj->pose->flags |= 0x10;
    obj->step = obj->step + 1;
}


static inline void finish_pose_and_action(struct Actor *obj)
{
    ((struct PoseDirBits *)&obj->pose->dir)->turn = 0;
    ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->pose->result = 0x10;
    sprite_set_action(obj, 0, -1, 0);
}

void finish_orbit_b(struct Actor *obj)
{
    s32 busy;
    u16 step;

    if (obj->data != NULL)
        ((struct DataLockBits *)&obj->data->lock)->locked = 0;
    busy = poll_action_end(obj);
    if (busy == 0)
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
                    obj->child = (struct Actor *)busy;
                }
                finish_pose_and_action(obj);
                obj->state = (void (*)(struct Actor *))busy;
            }
        }
    }
}

void enter_jump_arc_fall_a(struct Actor *obj)
{
    u16 *dhp;
    s32 *speedp;
    s32 h;
    s32 next;
    s32 t;

    dhp = (u16 *)&obj->dh;
    speedp = &obj->speed;
    obj->dh = *dhp + *speedp;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gUnk_0300034C.keysHeld & 1)
    {
        t = -0x7A - *speedp;
        next = *speedp + (t * 10) / 0x100;
    }
    else
        next = -0x7A;
    *speedp = next;
    if (obj->dh > 0)
        return;
    sprite_set_action(obj, 6, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    ((struct ObjFlagBits *)&obj->flags)->phase = 1;
    obj->state = jump_arc_fall_a;
}

void jump_arc_fall_a(struct Actor *obj)
{
    u16 *dhp;
    s32 *speedp;
    s32 h;
    s32 next;
    s32 t;

    dhp = (u16 *)&obj->dh;
    speedp = &obj->speed;
    obj->dh = *dhp + *speedp;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gUnk_0300034C.keysHeld & 1)
    {
        t = -0x7A - *speedp;
        next = *speedp + (t * 10) / 0x100;
    }
    else
        next = -0x7A;
    *speedp = next;
    if (obj->h > obj->data->ground)
        return;
    ((struct ObjFlagBits *)&obj->flags)->phase = 0;
    obj->h = obj->data->ground;
    sprite_set_action(obj, 8, 0x2000, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = jump_orbit_finish_if_pose_visible_e;
}

void enter_jump_arc_fall_b(struct Actor *obj)
{
    u16 *dhp;
    s32 *speedp;
    s32 h;
    s32 next;
    s32 t;

    dhp = (u16 *)&obj->dh;
    speedp = &obj->speed;
    obj->dh = *dhp + *speedp;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gUnk_0300034C.keysHeld & 2)
    {
        t = -0x7A - *speedp;
        next = *speedp + (t * 10) / 0x100;
    }
    else
        next = -0x7A;
    *speedp = next;
    if (obj->dh > 0)
        return;
    sprite_set_action(obj, 6, 0x204D, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    ((struct ObjFlagBits *)&obj->flags)->phase = 1;
    obj->state = jump_arc_fall_b;
}

void jump_arc_fall_b(struct Actor *obj)
{
    u16 *dhp;
    s32 *speedp;
    s32 h;
    s32 next;
    s32 t;

    dhp = (u16 *)&obj->dh;
    speedp = &obj->speed;
    obj->dh = *dhp + *speedp;
    h = obj->baseH + obj->dh;
    obj->h = h;
    if (gUnk_0300034C.keysHeld & 2)
    {
        t = -0x7A - *speedp;
        next = *speedp + (t * 10) / 0x100;
    }
    else
        next = -0x7A;
    *speedp = next;
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
    entry = (struct ListEntry *)(gUnk_03000FF4 + 0x3C);
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
    list = gUnk_03000FF4;
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

void jump_orbit_spawn_orbit_child_a(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, 0, 1);
    obj->child = child;
    *(u8 *)&child->action |= 6;
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xFD;
    child->unk49 = 3;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->unk84 = obj->baseX;
    child->unk8C = child->baseH;
    child->anim = gAnim284;
    *(u8 *)&child->action |= 0x40;
    child->callback = on_arrive;
    *(u8 *)&child->flags79 |= 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    jump_orbit_step_a(obj);
}

void jump_orbit_step_a(struct Actor *obj)
{
    s16 *stepp;
    s32 kind;

    obj->state = jump_orbit_step_a;
    kind = obj->action & 0x38;
    if (kind != 0)
    {
        end_action(obj);
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp == 0)
        sprite_set_action(obj, 0, 0x2025, 0);
    if ((s16)*stepp == 5)
        sprite_set_action(obj, 0, 0x2025, 1);
    if ((s16)*stepp == 10)
        sprite_set_action(obj, 0, 0x2025, 2);
    if ((s16)*stepp == 15)
        sprite_set_action(obj, 0, 0x2025, 3);
    if ((s16)*stepp != 20)
    {
        if ((gUnk_0300034C.keysHeld & obj->keyMask) == 0)
        {
            *stepp = (s16)*stepp * 4;
            obj->state = jump_orbit_step_b;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        *stepp = kind;
        obj->state = jump_orbit_hold_a;
    }
    *(u8 *)&obj->pose->flags |= 0x10;
    obj->step = obj->step + 1;
}

void jump_orbit_step_b(struct Actor *obj)
{
    s16 *stepp;

    obj->state = jump_orbit_step_b;
    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 8);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = jump_orbit_end_action_if_pose_visible;
        return;
    }
    if ((s16)*stepp <= 0x13)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2025, 0);
    }
    else if ((s16)*stepp <= 0x27)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2025, 1);
    }
    else if ((s16)*stepp <= 0x3B)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2025, 2);
    }
    *(u8 *)&obj->pose->flags |= 0x10;
    obj->step = obj->step - 5;
}

void jump_orbit_hold_a(struct Actor *obj)
{
    u32 keys;
    s16 *stepp;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    keys = gUnk_0300034C.keysHeld & obj->keyMask;
    if (keys == 0)
    {
        obj->timer = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = -0x1CC;
        sprite_set_action(obj, 2, 0, 0);
        *(u8 *)&obj->pose->flags |= 0x10;
        obj->state = jump_orbit_circle_a;
        obj->step = keys;
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp > 0x17)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gUnk_03000FD8[9] & 0xF0) == 0x10)
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
    if ((s16)*stepp > 0x0B)
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->step = obj->step + 1;
}

void jump_orbit_circle_a(struct Actor *obj)
{
    struct Actor *child;
    const s16 *cosp;
    s16 *stepp;
    u32 idx;
    s32 angle;
    s32 height;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActBits *)&child->action)->mode = 1;
    child->unkA0 += child->unkA4;
    angle = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->unkA0, 0x168);
    cosp = gCosineTable;
    idx = angle & 0xFF;
    child->unk10 = obj->baseX + (cosp[idx] / 0x40) * 0x16;
    height = obj->baseH + 0x1600;
    child->h = height + (gSineTable[idx] / 0x40) * 0x16;
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
        *(u8 *)&child->action |= 6;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = jump_orbit_end_a;
        if (obj->data != NULL)
            *(u8 *)&obj->data->lock |= 4;
        return;
    }
    *(u8 *)&obj->pose->flags |= 0x10;
    obj->step = obj->step + 1;
}

void jump_orbit_spawn_orbit_child_b(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, 0, 1);
    obj->child = child;
    *(u8 *)&child->action |= 6;
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xFD;
    child->unk49 = 3;
    ((struct ObjFlagBits *)&child->flags)->phase = 2;
    child->unk84 = obj->baseX;
    child->unk8C = child->baseH;
    child->anim = gAnim284;
    *(u8 *)&child->action |= 0x40;
    child->callback = on_arrive;
    *(u8 *)&child->flags79 |= 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    jump_orbit_step_c(obj);
}

void jump_orbit_step_c(struct Actor *obj)
{
    s16 *stepp;
    s32 kind;

    obj->state = jump_orbit_step_c;
    kind = obj->action & 0x38;
    if (kind != 0)
    {
        end_action(obj);
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp == 0)
        sprite_set_action(obj, 0, 0x2054, 0);
    if ((s16)*stepp == 8)
        sprite_set_action(obj, 0, 0x2054, 1);
    if ((s16)*stepp == 0x10)
        sprite_set_action(obj, 0, 0x2054, 2);
    if ((s16)*stepp == 0x18)
        sprite_set_action(obj, 0, 0x2054, 3);
    if ((s16)*stepp != 0x20)
    {
        if ((gUnk_0300034C.keysHeld & obj->keyMask) == 0)
        {
            *stepp = (s16)*stepp * 6;
            obj->state = jump_orbit_step_d;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        *stepp = kind;
        obj->state = jump_orbit_hold_b;
    }
    *(u8 *)&obj->pose->flags |= 0x10;
    obj->step = obj->step + 1;
}

void jump_orbit_step_d(struct Actor *obj)
{
    s16 *stepp;

    obj->state = jump_orbit_step_d;
    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0xC0, 0xC);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = jump_orbit_end_action_if_pose_visible;
        return;
    }
    if ((s16)*stepp <= 0x2F)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2054, 0);
    }
    else if ((s16)*stepp <= 0x5F)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2054, 1);
    }
    else if ((s16)*stepp <= 0x8F)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2054, 2);
    }
    *(u8 *)&obj->pose->flags |= 0x10;
    obj->step = obj->step - 8;
}

void jump_orbit_hold_b(struct Actor *obj)
{
    u32 keys;
    s16 *stepp;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    keys = gUnk_0300034C.keysHeld & obj->keyMask;
    if (keys == 0)
    {
        obj->timer = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = 0xA00;
        sprite_set_action(obj, 2, 0, 0);
        *(u8 *)&obj->pose->flags |= 0x10;
        obj->state = jump_orbit_circle_b;
        obj->step = keys;
        return;
    }
    stepp = (s16 *)&obj->step;
    if ((s16)*stepp > 0x15)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gUnk_03000FD8[9] & 0xF0) == 0x10)
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
    if ((s16)*stepp > 0x0E)
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    obj->step = obj->step + 1;
}


void jump_orbit_circle_b(struct Actor *obj)
{
    struct Actor *child;
    const s16 *cosp;
    s16 *stepp;
    u32 idx;
    s32 angle;
    s32 height;

    if (obj->action & 0x38)
    {
        end_action(obj);
        return;
    }
    child = obj->child;
    ((struct ActBits *)&child->action)->mode = 1;
    child->unkA0 += child->unkA4;
    angle = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(child->unkA0, 0x168);
    cosp = gCosineTable;
    idx = angle & 0xFF;
    child->unk10 = obj->baseX + (cosp[idx] / 0x40) * 0x18;
    height = obj->baseH + 0x1A00;
    child->h = height + (gSineTable[idx] / 0x40) * 0x18;
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
        *(u8 *)&child->action |= 6;
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->step = 0;
        obj->state = jump_orbit_end_b;
        if (obj->data != NULL)
            *(u8 *)&obj->data->lock |= 4;
        return;
    }
    *(u8 *)&obj->pose->flags |= 0x10;
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
    list = gUnk_03000FF4;
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
    entry = (struct ListEntry *)(gUnk_03000FF4 + 0x3C);
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
    entry = (struct ListEntry *)gUnk_03000FF4;
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
