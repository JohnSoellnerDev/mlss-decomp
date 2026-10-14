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
};

struct Part
{
    u8 filler0[0xD8];
    s32 x;
    s32 y;
    s32 h;
};

struct Data
{
    u8 filler0[0x7F];
    u8 flags7F;
    u8 filler80[0x8C - 0x80];
    s32 spawnX;
    s32 spawnY;
    s32 spawnH;
    u8 filler98[0xB4 - 0x98];
    u16 mark;
    s16 dirX;
    s16 dirY;
    u8 fillerBA[0xD8 - 0xBA];
    s32 x;
    s32 y;
    s32 h;
    u8 fillerE4[0xEC - 0xE4];
    s16 partNo;
};

struct Flags79Bits
{
    u8 unk0 : 5;
    u8 rebuild : 1;
    u8 unk6 : 2;
};

struct Counter
{
    u8 filler0[0x7];
    u8 cnt : 5;
    u8 unk7_5 : 3;
};

struct Oam
{
    u8 filler0[0xC];
    u8 index : 5;
    u8 unkC_5 : 3;
};

struct AffineSet
{
    u8 filler0[0x6];
    volatile u16 pa;
    u8 filler8[0xE - 0x8];
    volatile u16 pb;
    u8 filler10[0x16 - 0x10];
    volatile u16 pc;
    u8 filler18[0x1E - 0x18];
    volatile u16 pd;
};

struct AffineFlags
{
    u8 unk0 : 3;
    u8 bank : 1;
    u8 unk4 : 4;
};

struct Actor
{
    u8 filler0[0x04];
    struct Oam *oam;
    struct Pose *pose;
    void **nodes;
    s32 x;
    s32 y;
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct Data *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct Actor *);
    u8 filler50[0x6C - 0x50];
    const u8 *anim;
    u8 filler70[0x74 - 0x70];
    u8 listSlot;
    u8 unk75;
    u8 action;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 motionStart;
    u16 motionSpeed;
    u8 filler7E[0x84 - 0x7E];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 speed;
    s32 mode;
    s32 timer;
    s32 paramA0;
    s32 paramA4;
    s32 buttons;
    u16 step;
    u16 dx;
    s16 dy;
    u16 dh;
};

void start_motion(struct Actor *obj);
void sub_0808C7C8(struct Actor *obj);
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct Actor *obj, u32 action, u32 part, u32 extra);
void wait_then_set_action(struct Actor *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct Actor *owner);
void sound_stop_song(u32 id);
void obj_release(struct Actor *obj);
void wait_then_start_motion(struct Actor *obj);
s32 travel_aim_at_target(struct Actor *obj);
struct Actor *obj_spawn(struct Data *data, u32 kind, s32 x, s32 y, s32 h, s32 a, s32 b);
void obj_list_push_arg(struct Actor *obj, u32 arg);
void wait_then_remove_part(struct Actor *obj);
void pose_update_if_visible_e(struct Actor *obj);
void place_effect_and_remove(struct Actor *obj);
void pose_update_if_visible_b(struct Actor *obj);
void pose_update_if_visible_c(struct Actor *obj);
void pose_update_if_visible_d(struct Actor *obj);
extern u8 gUnk_0300034C[];
extern u8 *gUnk_03000FD8;
void *sprite_node_alloc(s32 x, s32 y, s32 h);
void sprite_node_take_link(struct Actor *obj);
void arrive_when_timer_done(struct Actor *obj);

ASM_FUNC("asm/code_0808AE9C/spawn_child_4068_sprite.s", void spawn_child_4068_sprite(void))

ASM_FUNC("asm/code_0808AE9C/spawn_child_4068_sprite_b.s", void spawn_child_4068_sprite_b(void))

ASM_FUNC("asm/code_0808AE9C/spawn_child_4068_sprite_c.s", void spawn_child_4068_sprite_c(void))

ASM_FUNC("asm/code_0808AE9C/spawn_child_4068_sprite_d.s", void spawn_child_4068_sprite_d(void))

ASM_FUNC("asm/code_0808AE9C/travel_aim_at_target.s", s32 travel_aim_at_target(struct Actor *obj))

ASM_FUNC("asm/code_0808AE9C/wait_then_spawn_traveler.s", void wait_then_spawn_traveler(void))

ASM_FUNC("asm/code_0808AE9C/travel_then_spawn_home.s", void travel_then_spawn_home(void))

ASM_FUNC("asm/code_0808AE9C/settle_aim_then_finish.s", void settle_aim_then_finish(void))

ASM_FUNC("asm/code_0808AE9C/wait_then_spawn_walker.s", void wait_then_spawn_walker(void))

ASM_FUNC("asm/code_0808AE9C/walk_then_spawn_home.s", void walk_then_spawn_home(void))

ASM_FUNC("asm/code_0808AE9C/settle_aim_then_remove.s", void settle_aim_then_remove(void))

void wait_then_start_motion(struct Actor *obj)
{
    u16 before = obj->step;
    obj->step = before - 1;
    if ((s16)before <= 0)
    {
        struct Part *entry;
        if (obj->owner->data->partNo == -2)
            entry = (struct Part *)((u8 *)obj->owner->data - 0x130);
        else
            entry = (struct Part *)((u8 *)obj->owner->data + 0x130);
        obj->targetX = entry->x;
        obj->targetY = entry->y;
        obj->targetH = entry->h;
        ((struct Flags79Bits *)&obj->flags79)->rebuild = 1;
        obj->motionSpeed = 0x100;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = sub_0808C7C8;
    }
}

ASM_FUNC("asm/code_0808AE9C/set_action_by_mode.s", void set_action_by_mode(void))

ASM_FUNC("asm/code_0808AE9C/spawn_part_by_mode.s", void spawn_part_by_mode(void))

ASM_FUNC("asm/code_0808AE9C/part_jump_setup.s", void part_jump_setup(void))

ASM_FUNC("asm/code_0808AE9C/jump_arc_step.s", void jump_arc_step(void))

void part_landing_step(struct Actor *obj)
{
    s32 x;
    s32 y;
    s32 h;

    obj->dh += obj->speed;
    x = obj->x + (s16)obj->dx;
    obj->x = x;
    y = obj->y + obj->dy;
    obj->y = y;
    h = obj->h + (s16)obj->dh;
    obj->h = h;
    if (obj->targetH >= h)
    {
        effect_spawn_at(0xC67, x / 256, y / 256, h / 256, obj);
        sound_stop_song(0x11A);
        set_slot_value(0xAD, -1);
        obj_release(obj);
    }
}
