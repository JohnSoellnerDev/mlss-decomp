#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0x11];
    u8 turn;
    u8 unk12_0 : 1;
    u8 mode : 2;
    u8 visible : 1;
    u8 hold : 1;
    u8 unk12_5 : 3;
    u8 filler13[0x20 - 0x13];
    u8 result;
    u8 filler21[0x23 - 0x21];
    u8 count;
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
extern u8 *gUnk_03000FF4;
extern u8 gUnk_0300034C[];
extern u8 *gUnk_03000FD8;

void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct Actor *obj, u32 action, u32 arg, u32 extra);
s32 try_park_in_slot(struct Actor *obj);
void hold_orbit_arc_land_e(struct Actor *obj);
void finish_if_pose_visible_g(struct Actor *obj);
void on_arrive(struct Actor *obj);
void end_action(struct Actor *obj);
u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 seed);
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
    entry = gUnk_03000FF4 + 0x3C;
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
    entry = gUnk_03000FF4;
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
    entry = gUnk_03000FF4 + 0x3C;
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
    entry = gUnk_03000FF4;
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

u32 random_jump_a(struct Actor *obj)
{
    s32 timer;
    s32 start;
    s32 (*fn)(u32);
    u8 *entry;

    obj->anim = gAnim278;
    entry = gUnk_03000FF4;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(entry + 0x2A);
    timer = sub_081DE850(6, mt19937_next()) + 4;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x2000, 0);
        obj->pose->mode = 0;
        obj->timer = timer;
        obj->state = random_jump_timer_arc_e;
    }
    else
    {
        obj->state = random_jump_start_jump_arc_e;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x00689E40);
    obj->dh = (start - 0x58) / 2;
    return 1;
}

u32 random_jump_b(struct Actor *obj)
{
    s32 timer;
    s32 start;
    s32 (*fn)(u32);
    u8 *entry;

    obj->anim = gAnim374;
    entry = gUnk_03000FF4 + 0x3C;
    obj->switch6 = 0;
    obj->switch5 = 0;
    *(u32 *)(obj->data + 0x114) = *(u16 *)(entry + 0x2A);
    timer = sub_081DE850(0x16, mt19937_next()) + 10;
    if (timer != 0)
    {
        sprite_set_action(obj, 4, 0x204D, 0);
        obj->pose->mode = 0;
        obj->timer = timer;
        obj->state = random_jump_timer_arc_f;
    }
    else
    {
        obj->state = random_jump_start_jump_arc_f;
    }
    fn = (s32 (*)(u32))gMathRoutines;
    start = fn(0x00559440);
    obj->dh = (start - 0x48) / 2;
    return 1;
}

void random_jump_spawn_orbit_child_a(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    { u8 *p; p = (u8 *)&child->flags76; *p = *p | 6; }
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
    { u8 *p; p = (u8 *)&child->flags76; *p = *p | 0x40; }
    child->callback = on_arrive;
    child->visible79 = 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    random_jump_orbit_step_a(obj);
}

void random_jump_orbit_step_a(struct Actor *obj)
{
    u32 action;
    u8 *input;

    obj->state = random_jump_orbit_step_a;
    action = obj->flags76 & 0x38;
    if (action != 0)
    {
        end_action(obj);
        return;
    }
    if (obj->step == 0)
        sprite_set_action(obj, 0, 0x2025, 0);
    if (obj->step == 3)
        sprite_set_action(obj, 0, 0x2025, 1);
    if (obj->step == 6)
        sprite_set_action(obj, 0, 0x2025, 2);
    if (obj->step == 9)
        sprite_set_action(obj, 0, 0x2025, 3);
    if (obj->step != 12)
    {
        input = gUnk_0300034C;
        if ((*(u16 *)(input + 0x3C) & obj->buttons) == 0)
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
        obj->step = action;
        obj->state = random_jump_orbit_hold_a;
    }
    obj->pose->hold = 1;
    obj->step++;
}

void random_jump_orbit_step_b(struct Actor *obj)
{
    u8 *p;

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
        p = (u8 *)obj->pose + 0x12;
        *p = *p | 0x10;
        obj->step = obj->step - 3;
    }
}

void random_jump_orbit_hold_a(struct Actor *obj)
{
    u32 held;
    u8 *input;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    input = gUnk_0300034C;
    held = *(u16 *)(input + 0x3C) & obj->buttons;
    if (held == 0)
    {
        obj->timer = sub_081DE850(0x16, mt19937_next()) + 10;
        sprite_set_action(obj, 6, 0, 0);
        obj->pose->mode = 0;
        obj->state = orbit_timer_a;
        obj->step = 0;
        return;
    }
    if (obj->step > 0x31)
    {
        if (*(u8 *)(obj->data + 0x12D) == 0x1E)
        {
            if ((gUnk_03000FD8[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                obj->pose->mode = 1;
                obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xA);
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
            obj->pose->mode = 1;
        }
        obj->pose->hold = 0;
        obj->state = random_jump_end_action_if_pose_visible;
        return;
    }
    if (obj->step > 0x18)
    {
        obj->pose->hold = 0;
    }
    obj->step = obj->step + 1;
}

ASM_FUNC("asm/code_08090F84/random_jump_orbit_circle_a.s", void random_jump_orbit_circle_a(struct Actor *obj))

void random_jump_spawn_orbit_child_b(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    { u8 *p; p = (u8 *)&child->flags76; *p = *p | 6; }
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
    { u8 *p; p = (u8 *)&child->flags76; *p = *p | 0x40; }
    child->callback = on_arrive;
    child->visible79 = 1;
    obj->step = 0;
    set_slot_value(0x7C, -1);
    random_jump_orbit_step_c(obj);
}

void random_jump_orbit_step_c(struct Actor *obj)
{
    u32 action;
    u8 *input;

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
        input = gUnk_0300034C;
        if ((*(u16 *)(input + 0x3C) & obj->buttons) == 0)
        {
            obj->step *= 3;
            obj->state = random_jump_orbit_step_d;
            return;
        }
    }
    else
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->pose->mode = 0;
        obj->step = action;
        obj->state = random_jump_orbit_hold_b;
    }
    obj->pose->hold = 1;
    obj->step++;
}

void random_jump_orbit_step_d(struct Actor *obj)
{
    u8 *p;

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
        obj->pose->result = MATH_ROUTINE(u8 (*)(u32, u32), iwram_div)(0xC0, 0xC);
        obj->pose->hold = 0;
        obj->state = random_jump_end_action_if_pose_visible;
    }
    else
    {
        if (obj->step <= 5)
        {
            if (obj->pose->count != 0)
            {
                sprite_set_action(obj, 0, 0x2054, 0);
            }
        }
        else if (obj->step <= 11)
        {
            if (obj->pose->count > 1)
            {
                sprite_set_action(obj, 0, 0x2054, 1);
            }
        }
        else if (obj->step <= 17)
        {
            if (obj->pose->count > 2)
            {
                sprite_set_action(obj, 0, 0x2054, 2);
            }
        }
        p = (u8 *)obj->pose + 0x12;
        *p = *p | 0x10;
        obj->step = obj->step - 2;
    }
}

void random_jump_orbit_hold_b(struct Actor *obj)
{
    u32 held;
    u8 *input;

    if (obj->flags76 & 0x38)
    {
        end_action(obj);
        return;
    }
    input = gUnk_0300034C;
    held = *(u16 *)(input + 0x3C) & obj->buttons;
    if (held == 0)
    {
        obj->timer = sub_081DE850(6, mt19937_next()) + 8;
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
            if ((gUnk_03000FD8[9] & 0xF0) == 0x10)
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

ASM_FUNC("asm/code_08090F84/random_jump_orbit_circle_b.s", void random_jump_orbit_circle_b(struct Actor *obj))

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
    entry = gUnk_03000FF4;
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
