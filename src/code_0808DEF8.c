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
    u8 filler0[0xEC];
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
    u8 filler0C[0x18 - 0x0C];
    s16 h;
    u8 filler1A[0x28 - 0x1A];
    struct Data *data;
    struct Actor *owner;
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
    u8 flags;
    u8 filler78;
    u8 flags79;
    u8 filler7A[0x94 - 0x7A];
    s32 speed;
    u8 filler98[0x9C - 0x98];
    s32 timer;
    u8 filler[0xAC - 0xA0];
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
extern const u8 gAnim278[];
extern const u8 gAnim374[];

void set_slot_value(u32 id, s32 value);
void obj_release(struct Actor *obj);
void on_arrive(struct Actor *obj);

void jump_orbit_start_jump_arc_c(struct Actor *obj);
void jump_orbit_start_jump_arc_d(struct Actor *obj);
void sprite_set_action(struct Actor *obj, u32 action, u32 arg, u32 extra);
void orbit_slots_spawn_orbit_child(struct Actor *obj);
void orbit_slots_spawn_orbit_child_b(struct Actor *obj);

ASM_FUNC("asm/code_0808DEF8/hold_then_orbit_b.s", void hold_then_orbit_b(void))

ASM_FUNC("asm/code_0808DEF8/orbit_position_b.s", void orbit_position_b(void))

ASM_FUNC("asm/code_0808DEF8/finish_orbit_b.s", void finish_orbit_b(void))

ASM_FUNC("asm/code_0808DEF8/enter_jump_arc_fall_a.s", void enter_jump_arc_fall_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_arc_fall_a.s", void jump_arc_fall_a(void))

ASM_FUNC("asm/code_0808DEF8/enter_jump_arc_fall_b.s", void enter_jump_arc_fall_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_arc_fall_b.s", void jump_arc_fall_b(void))

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

ASM_FUNC("asm/code_0808DEF8/end_action.s", void end_action(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_setup_list_slot_a.s", void jump_orbit_setup_list_slot_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_setup_list_slot_b.s", void jump_orbit_setup_list_slot_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_start_jump_arc_a.s", void jump_orbit_start_jump_arc_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_start_jump_arc_b.s", void jump_orbit_start_jump_arc_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_arc_rise_a.s", void jump_orbit_arc_rise_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_arc_rise_b.s", void jump_orbit_arc_rise_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_arc_land_a.s", void jump_orbit_arc_land_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_arc_land_b.s", void jump_orbit_arc_land_b(void))

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

ASM_FUNC("asm/code_0808DEF8/jump_orbit_spawn_orbit_child_a.s", void jump_orbit_spawn_orbit_child_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_step_a.s", void jump_orbit_step_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_step_b.s", void jump_orbit_step_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_hold_a.s", void jump_orbit_hold_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_circle_a.s", void jump_orbit_circle_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_spawn_orbit_child_b.s", void jump_orbit_spawn_orbit_child_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_step_c.s", void jump_orbit_step_c(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_step_d.s", void jump_orbit_step_d(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_hold_b.s", void jump_orbit_hold_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_circle_b.s", void jump_orbit_circle_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_end_b.s", void jump_orbit_end_b(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_end_a.s", void jump_orbit_end_a(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_end_action_if_pose_visible.s", void jump_orbit_end_action_if_pose_visible(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_setup_slot_spawn.s", void jump_orbit_setup_slot_spawn(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_dispatch_slot_pause_jump.s", void jump_orbit_dispatch_slot_pause_jump(void))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_dispatch_slot_spawn.s", void jump_orbit_dispatch_slot_spawn(void))

ASM_FUNC("asm/code_0808DEF8/pause_jump_b.s", void pause_jump_b(void))

ASM_FUNC("asm/code_0808DEF8/pause_jump_a.s", void pause_jump_a(void))

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

ASM_FUNC("asm/code_0808DEF8/jump_orbit_start_jump_arc_c.s", void jump_orbit_start_jump_arc_c(struct Actor *obj))

ASM_FUNC("asm/code_0808DEF8/jump_orbit_start_jump_arc_d.s", void jump_orbit_start_jump_arc_d(struct Actor *obj))
