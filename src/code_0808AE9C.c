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

// The template record starts 8 bytes into the data record and has the layout of an actor.
struct Template
{
    u8 filler0[0x2C];
    struct Actor *owner;
    u8 filler30[0x6C - 0x30];
    const u8 *anim;
};

struct Data
{
    u8 filler0[0x7F];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
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

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

struct ActionBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 hi5 : 5;
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

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

extern u8 gSys[];

// The rotation entry of the object's sprite: sys flag bit 3 selects the bank, the OAM index the entry.
static inline struct AffineSet *oam_affine_group_of(struct Actor *obj)
{
    u32 bank = ((u32)gSys[0x888] << 28) >> 31;
    u32 off;
    bank <<= 10;
    off = (obj->oam->index << 5) + (u32)(gSys + 0x80);
    return (struct AffineSet *)(bank + off);
}

// A spawned part takes over bits 5 and 6 of the template flags.
static inline void copy_template_flags(struct Actor *part, struct Data *data)
{
    ((struct ObjFlags77 *)part)->bit6 = data->bit6;
    ((struct ObjFlags77 *)part)->bit5 = data->bit5;
}

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
void settle_aim_then_finish(struct Actor *obj);
void settle_aim_then_remove(struct Actor *obj);
void pose_update_if_visible_e(struct Actor *obj);
void *sprite_node_alloc(s32 x, s32 y, s32 h);
void walk_then_spawn_home(struct Actor *obj);
void place_effect_and_remove(struct Actor *obj);
void pose_update_if_visible_b(struct Actor *obj);
void part_landing_step(struct Actor *obj);
void pose_update_if_visible_c(struct Actor *obj);
void pose_update_if_visible_d(struct Actor *obj);
extern u8 *gWorld;
void *sprite_node_alloc(s32 x, s32 y, s32 h);
void sprite_node_take_link(struct Actor *obj);
void arrive_when_timer_done(struct Actor *obj);

void spawn_child_4068_sprite(struct Actor *obj)
{
    struct Actor *child;
    struct Actor *owner;
    s32 x;
    s32 y;
    s32 h;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = pose_update_if_visible_e;
        set_slot_value(0x85, -1);
        x = obj->x / 0x100;
        y = obj->y / 0x100;
        h = obj->h / 0x100;
        owner = obj->owner;
        child = obj_spawn(obj->data, 0x4068, x - 0xC, y, h + 0x32, 1, 1);
        effect_spawn_at(0xC52, x, y, h, obj);
        obj->child = child;
        child->listSlot = 0xFF;
        ((struct Flags78Bits *)&child->flags78)->kind = 1;
        child->timer = obj->timer;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->buttons = obj->buttons;
        child->step = 0;
        child->targetX = owner->data->x - 0x2000;
        child->targetY = owner->data->y - 0x400;
        child->targetH = owner->data->h + 0x1000;
        ((struct Flags79Bits *)&child->flags79)->rebuild = 1;
        child->motionSpeed = 0x400;
        child->motionStart = 0;
        start_motion(child);
        child->anim = obj->anim;
        obj_list_push_arg(child, 0);
        child->state = place_effect_and_remove;
    }
}

void spawn_child_4068_sprite_b(struct Actor *obj)
{
    struct Actor *child;
    struct Actor *owner;
    s32 x;
    s32 y;
    s32 h;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = pose_update_if_visible_d;
        set_slot_value(0x85, -1);
        x = obj->x / 0x100;
        y = obj->y / 0x100;
        h = obj->h / 0x100;
        owner = obj->owner;
        child = obj_spawn(obj->data, 0x4068, x - 0xA, y, h + 0x28, 1, 1);
        effect_spawn_at(0xC52, x, y, h, obj);
        obj->child = child;
        child->listSlot = 0xFF;
        ((struct Flags78Bits *)&child->flags78)->kind = 1;
        child->timer = obj->timer;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->buttons = obj->buttons;
        child->step = 0;
        child->targetX = owner->data->x - 0x2000;
        child->targetY = owner->data->y + 0x400;
        child->targetH = owner->data->h + 0x1200;
        ((struct Flags79Bits *)&child->flags79)->rebuild = 1;
        child->motionSpeed = 0x400;
        child->motionStart = 0;
        start_motion(child);
        child->anim = obj->anim;
        obj_list_push_arg(child, 0);
        child->state = place_effect_and_remove;
    }
}

void spawn_child_4068_sprite_c(struct Actor *obj)
{
    struct Actor *child;
    struct Actor *owner;
    s32 x;
    s32 y;
    s32 h;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = pose_update_if_visible_c;
        set_slot_value(0x85, -1);
        x = obj->x / 0x100;
        y = obj->y / 0x100;
        h = obj->h / 0x100;
        owner = obj->owner;
        child = obj_spawn(obj->data, 0x4068, x - 0xC, y, h + 0x32, 1, 1);
        effect_spawn_at(0xC59, x, y, h, obj);
        obj->child = child;
        child->listSlot = 0xFF;
        ((struct Flags78Bits *)&child->flags78)->kind = 1;
        child->timer = obj->timer;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->buttons = obj->buttons;
        child->step = 0;
        child->targetX = owner->data->x + (obj->timer << 8);
        child->targetY = owner->data->y + (obj->paramA0 << 8);
        child->targetH = owner->data->h + (obj->paramA4 << 8);
        ((struct Flags79Bits *)&child->flags79)->rebuild = 1;
        child->motionSpeed = 0x300;
        child->motionStart = 0;
        start_motion(child);
        child->anim = obj->anim;
        obj_list_push_arg(child, 0);
        child->state = place_effect_and_remove;
    }
}

void spawn_child_4068_sprite_d(struct Actor *obj)
{
    struct Actor *child;
    struct Actor *owner;
    s32 x;
    s32 y;
    s32 h;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = pose_update_if_visible_b;
        set_slot_value(0x85, -1);
        x = obj->x / 0x100;
        y = obj->y / 0x100;
        h = obj->h / 0x100;
        owner = obj->owner;
        child = obj_spawn(obj->data, 0x4068, x - 0xA, y, h + 0x16, 1, 1);
        effect_spawn_at(0xC59, x, y, h, obj);
        obj->child = child;
        child->listSlot = 0xFF;
        ((struct Flags78Bits *)&child->flags78)->kind = 1;
        child->timer = obj->timer;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->buttons = obj->buttons;
        child->step = 0;
        child->targetX = owner->data->x + (obj->timer << 8);
        child->targetY = owner->data->y + (obj->paramA0 << 8);
        child->targetH = owner->data->h + (obj->paramA4 << 8);
        ((struct Flags79Bits *)&child->flags79)->rebuild = 1;
        child->motionSpeed = 0x300;
        child->motionStart = 0;
        start_motion(child);
        child->anim = obj->anim;
        obj_list_push_arg(child, 0);
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        child->state = place_effect_and_remove;
    }
}

ASM_FUNC("asm/code_0808AE9C/travel_aim_at_target.s", s32 travel_aim_at_target(struct Actor *obj))

void travel_then_spawn_home(struct Actor *obj);
ASM_FUNC("asm/code_0808AE9C/wait_then_spawn_traveler.s", void wait_then_spawn_traveler(void))

void travel_then_spawn_home(struct Actor *obj)
{
    struct Data *data;
    struct Template *tpl;
    struct Actor *part;
    s32 length;
    u16 *progress;

    progress = &obj->dx;
    *progress = (obj->timer << 7) + *progress;
    length = travel_aim_at_target(obj);
    if (((s16)*progress << 1) >= length)
    {
        data = obj->data;
        tpl = (struct Template *)((u8 *)data + 8);
        part = obj_spawn(data, 0x4066, data->spawnX / 0x100, data->spawnY / 0x100, data->spawnH / 0x100, 0, 1);
        sprite_set_action(part, 1, 0, 0);
        part->listSlot = 0xFF;
        ((struct Flags79Bit0 *)&part->flags79)->bit0 = 1;
        ((struct ActionBits *)&part->action)->mode = 3;
        part->owner = tpl->owner;
        part->anim = tpl->anim;
        obj_list_push_arg(part, 0);
        copy_template_flags(part, data);
        part->pose->mode = 0;
        part->state = wait_then_remove_part;
        part->step = 6;
        obj->child = part;
        obj->step = 0x10;
        obj->state = settle_aim_then_finish;
    }
}

void settle_aim_then_finish(struct Actor *obj)
{
    u16 *counter;
    s32 ox;
    s32 oy;
    struct AffineSet *group;

    travel_aim_at_target(obj);
    counter = &obj->step;
    *counter = *counter - 1;
    if ((s16)*counter <= 0x12)
    {
        ox = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((obj->data->dirX << 3) + obj->data->dirX) << 1, (s16)*counter);
        oy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((obj->data->dirY << 3) + obj->data->dirY) << 1, (s16)*counter);
        group = oam_affine_group_of(obj);
        group->pa = ox;
        group->pb = oy;
        group->pc = -oy;
        group->pd = ox;
        if ((s16)*counter == 1)
        {
            obj->data->mark = 0;
            sound_stop_song(0x5E);
            obj_release(obj);
        }
    }
}

ASM_FUNC("asm/code_0808AE9C/wait_then_spawn_walker.s", void wait_then_spawn_walker(void))

void walk_then_spawn_home(struct Actor *obj)
{
    struct Data *data;
    struct Template *tpl;
    struct Actor *part;
    s32 length;

    obj->dx += 0x300;
    length = travel_aim_at_target(obj);
    if (((s16)obj->dx << 1) < length)
        return;
    data = obj->data;
    tpl = (struct Template *)((u8 *)data + 8);
    part = obj_spawn(data, 0x4066, data->spawnX / 0x100, data->spawnY / 0x100, data->spawnH / 0x100, 0, 1);
    sprite_set_action(part, 1, 0, 0);
    part->pose->mode = 0;
    part->listSlot = 0xFF;
    part->owner = tpl->owner;
    ((struct Flags79Bit0 *)&part->flags79)->bit0 = 1;
    part->anim = tpl->anim;
    obj_list_push_arg(part, 0);
    copy_template_flags(part, data);
    part->step = 0x3C;
    part->state = wait_then_start_motion;
    obj->child = part;
    obj->step = 0;
    obj->state = settle_aim_then_remove;
}

void settle_aim_then_remove(struct Actor *obj)
{
    u16 *counter;
    u16 before;
    s32 ox;
    s32 oy;
    struct AffineSet *group;

    travel_aim_at_target(obj);
    counter = &obj->step;
    before = *counter;
    if (*(s16 *)counter != 0)
    {
        *counter = before - 1;
        ox = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->data->dirX << 4, (s16)*counter);
        oy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->data->dirY << 4, (s16)*counter);
        group = oam_affine_group_of(obj);
        group->pa = ox;
        group->pb = oy;
        group->pc = -oy;
        group->pd = ox;
        if ((s16)*counter <= 1)
        {
            sound_stop_song(0x5E);
            obj->data->mark = 0;
            obj_release(obj->child);
            obj_release(obj);
        }
    }
}

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

NONMATCH("asm/code_0808AE9C/set_action_by_mode.s", void set_action_by_mode(struct Actor *obj))
{
    u16 wait;

    set_slot_value(0xAF, -1);
    switch (obj->mode)
    {
    case 1:
        sprite_set_action(obj, 7, 0, 0);
        break;
    case 0:
    case 2:
        sprite_set_action(obj, 3, 0, 0);
        break;
    case 3:
        sprite_set_action(obj, 7, 0, 0);
        break;
    }
    obj->pose->mode = 0;
    if (obj->mode == 0 || obj->mode == 1 || obj->mode == 2 || obj->mode == 3)
        wait = 0x18;
    obj->step = wait;
    obj->state = wait_then_set_action;
}
END_NONMATCH

void part_jump_setup(struct Actor *obj);
ASM_FUNC("asm/code_0808AE9C/spawn_part_by_mode.s", void spawn_part_by_mode(void))

ASM_FUNC("asm/code_0808AE9C/part_jump_setup.s", void part_jump_setup(struct Actor *obj))

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
