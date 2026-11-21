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
void jump_arc_step(struct Actor *obj);
void pose_update_if_visible_a(struct Actor *obj);

// Once the current animation has ended: switch to action 5, play sound 0x85 and launch a shot
// (object 0x4068, with effect 0xC52) from just in front of the actor toward a fixed point near
// the owner's target position (32 px behind, 4 px up in y, 16 px higher). The shot bursts into an
// effect when it arrives (place_effect_and_remove).
void spawn_child_4068_sprite(struct Actor *obj)
{
    struct Actor *shot;
    struct Actor *owner;
    s32 pixelX;
    s32 pixelY;
    s32 pixelH;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = pose_update_if_visible_e;
        set_slot_value(0x85, -1);

        // Positions are 24.8 fixed point; spawning works in whole pixels.
        pixelX = obj->x / 0x100;
        pixelY = obj->y / 0x100;
        pixelH = obj->h / 0x100;
        owner = obj->owner;
        shot = obj_spawn(obj->data, 0x4068, pixelX - 12, pixelY, pixelH + 50, 1, 1);
        effect_spawn_at(0xC52, pixelX, pixelY, pixelH, obj);
        obj->child = shot;

        shot->listSlot = 0xFF;
        ((struct Flags78Bits *)&shot->flags78)->kind = 1;
        shot->timer = obj->timer;
        shot->paramA0 = obj->paramA0;
        shot->paramA4 = obj->paramA4;
        shot->buttons = obj->buttons;
        shot->step = 0;

        shot->targetX = owner->data->x - 0x2000;
        shot->targetY = owner->data->y - 0x400;
        shot->targetH = owner->data->h + 0x1000;
        ((struct Flags79Bits *)&shot->flags79)->rebuild = 1;
        shot->motionSpeed = 0x400;
        shot->motionStart = 0;
        start_motion(shot);

        shot->anim = obj->anim;
        obj_list_push_arg(shot, 0);
        shot->state = place_effect_and_remove;
    }
}

// Once the current animation has ended: switch to action 9, play sound 0x85 and launch a shot
// (object 0x4068, with effect 0xC52) from just in front of the actor toward a fixed point near
// the owner's target position (32 px behind, 4 px down in y, 18 px higher). The shot bursts into an
// effect when it arrives (place_effect_and_remove).
void spawn_child_4068_sprite_b(struct Actor *obj)
{
    struct Actor *shot;
    struct Actor *owner;
    s32 pixelX;
    s32 pixelY;
    s32 pixelH;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = pose_update_if_visible_d;
        set_slot_value(0x85, -1);

        // Positions are 24.8 fixed point; spawning works in whole pixels.
        pixelX = obj->x / 0x100;
        pixelY = obj->y / 0x100;
        pixelH = obj->h / 0x100;
        owner = obj->owner;
        shot = obj_spawn(obj->data, 0x4068, pixelX - 10, pixelY, pixelH + 40, 1, 1);
        effect_spawn_at(0xC52, pixelX, pixelY, pixelH, obj);
        obj->child = shot;

        shot->listSlot = 0xFF;
        ((struct Flags78Bits *)&shot->flags78)->kind = 1;
        shot->timer = obj->timer;
        shot->paramA0 = obj->paramA0;
        shot->paramA4 = obj->paramA4;
        shot->buttons = obj->buttons;
        shot->step = 0;

        shot->targetX = owner->data->x - 0x2000;
        shot->targetY = owner->data->y + 0x400;
        shot->targetH = owner->data->h + 0x1200;
        ((struct Flags79Bits *)&shot->flags79)->rebuild = 1;
        shot->motionSpeed = 0x400;
        shot->motionStart = 0;
        start_motion(shot);

        shot->anim = obj->anim;
        obj_list_push_arg(shot, 0);
        shot->state = place_effect_and_remove;
    }
}

// Once the current animation has ended: switch to action 5, play sound 0x85 and launch a shot
// (object 0x4068, with effect 0xC59) from just in front of the actor toward the owner's target
// position, offset by (timer, paramA0, paramA4) pixels. The shot bursts into an effect when it
// arrives (place_effect_and_remove).
void spawn_child_4068_sprite_c(struct Actor *obj)
{
    struct Actor *shot;
    struct Actor *owner;
    s32 pixelX;
    s32 pixelY;
    s32 pixelH;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = pose_update_if_visible_c;
        set_slot_value(0x85, -1);

        // Positions are 24.8 fixed point; spawning works in whole pixels.
        pixelX = obj->x / 0x100;
        pixelY = obj->y / 0x100;
        pixelH = obj->h / 0x100;
        owner = obj->owner;
        shot = obj_spawn(obj->data, 0x4068, pixelX - 12, pixelY, pixelH + 50, 1, 1);
        effect_spawn_at(0xC59, pixelX, pixelY, pixelH, obj);
        obj->child = shot;

        shot->listSlot = 0xFF;
        ((struct Flags78Bits *)&shot->flags78)->kind = 1;
        shot->timer = obj->timer;
        shot->paramA0 = obj->paramA0;
        shot->paramA4 = obj->paramA4;
        shot->buttons = obj->buttons;
        shot->step = 0;

        // Fly to the owner's target, offset by the pixel offsets kept in timer/paramA0/paramA4.
        shot->targetX = owner->data->x + (obj->timer << 8);
        shot->targetY = owner->data->y + (obj->paramA0 << 8);
        shot->targetH = owner->data->h + (obj->paramA4 << 8);
        ((struct Flags79Bits *)&shot->flags79)->rebuild = 1;
        shot->motionSpeed = 0x300;
        shot->motionStart = 0;
        start_motion(shot);

        shot->anim = obj->anim;
        obj_list_push_arg(shot, 0);
        shot->state = place_effect_and_remove;
    }
}

// Once the current animation has ended: switch to action 9, play sound 0x85 and launch a shot
// (object 0x4068) from just in front of the actor toward the owner's target position, offset by
// (timer, paramA0, paramA4) pixels. The shot inherits the actor's flags77 bits 5 and 6 and bursts
// into an effect when it arrives (place_effect_and_remove).
void spawn_child_4068_sprite_d(struct Actor *obj)
{
    struct Actor *shot;
    struct Actor *owner;
    s32 pixelX;
    s32 pixelY;
    s32 pixelH;

    if (obj->pose->visible)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = pose_update_if_visible_b;
        set_slot_value(0x85, -1);

        // Positions are 24.8 fixed point; spawning works in whole pixels.
        pixelX = obj->x / 0x100;
        pixelY = obj->y / 0x100;
        pixelH = obj->h / 0x100;
        owner = obj->owner;
        shot = obj_spawn(obj->data, 0x4068, pixelX - 10, pixelY, pixelH + 22, 1, 1);
        effect_spawn_at(0xC59, pixelX, pixelY, pixelH, obj);
        obj->child = shot;

        shot->listSlot = 0xFF;
        ((struct Flags78Bits *)&shot->flags78)->kind = 1;
        shot->timer = obj->timer;
        shot->paramA0 = obj->paramA0;
        shot->paramA4 = obj->paramA4;
        shot->buttons = obj->buttons;
        shot->step = 0;

        // Fly to the owner's target, offset by the pixel offsets kept in timer/paramA0/paramA4.
        shot->targetX = owner->data->x + (obj->timer << 8);
        shot->targetY = owner->data->y + (obj->paramA0 << 8);
        shot->targetH = owner->data->h + (obj->paramA4 << 8);
        ((struct Flags79Bits *)&shot->flags79)->rebuild = 1;
        shot->motionSpeed = 0x300;
        shot->motionStart = 0;
        start_motion(shot);

        shot->anim = obj->anim;
        obj_list_push_arg(shot, 0);
        ((struct ObjFlags77 *)shot)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)shot)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        shot->state = place_effect_and_remove;
    }
}

ASM_FUNC("asm/code_0808AE9C/travel_aim_at_target.s", s32 travel_aim_at_target(struct Actor *obj))

void travel_then_spawn_home(struct Actor *obj);
NONMATCH("asm/code_0808AE9C/wait_then_spawn_traveler.s", void wait_then_spawn_traveler(struct Actor *obj))
{
    u16 *counter;
    u16 before;
    struct Actor *traveler;
    struct Actor *owner;
    struct Data *ownerData;
    void **node;
    void **link;
    s32 *targetX;
    s32 *targetY;
    s32 *targetH;
    s32 left;
    s32 step = 4;
    u32 count;

    counter = &obj->step;
    before = *counter;
    *counter = before - 1;
    if ((s16)before <= 0)
    {
        traveler = obj_spawn(obj->data, 0x4066, (obj->x / 0x100) - 0xA, obj->y / 0x100, (obj->h / 0x100) + 0x18, ((u32)obj->pose->turn << 25) >> 31, 1);
        obj->child = traveler;
        traveler->dx = 0;
        ((struct Flags79Bit0 *)&traveler->flags79)->bit0 = 1;
        traveler->targetX = traveler->baseX;
        traveler->targetY = traveler->baseY;
        traveler->targetH = traveler->baseH;
        traveler->listSlot = 0;
        ((struct ActionBits *)&traveler->action)->mode = 3;
        traveler->owner = obj->owner;
        traveler->timer = obj->timer;
        traveler->paramA0 = obj->paramA0;
        traveler->paramA4 = obj->paramA4;
        traveler->buttons = obj->buttons;
        sprite_node_take_link(traveler);

        count = ((struct Counter *)gWorld)->cnt;
        ((struct Counter *)gWorld)->cnt = count - 1;
        ((struct Oam *)traveler->oam)->index = count;

        targetX = &obj->targetX;
        targetY = &obj->targetY;
        targetH = &obj->targetH;

        node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
        traveler->nodes = node;
        link = node;
        node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
        *link = node;
        link = node;
        node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
        *link = node;
        link = node;
        left = 7;
        do
        {
            node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
            *link = node;
            link = node;
            node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
            *link = node;
            link = node;
            node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
            *link = node;
            link = node;
            node = sprite_node_alloc(traveler->x, traveler->y, traveler->h);
            *link = node;
            link = node;
            left -= step;
        } while (left >= 0);
        *link = 0;
        traveler->state = travel_then_spawn_home;

        owner = obj->owner;
        ownerData = owner->data;
        *targetX = ownerData->x;
        *targetY = ownerData->y;
        *targetH = ownerData->h;
        sprite_set_action(obj, 6, 0, 0);
        obj->state = arrive_when_timer_done;
        *counter = 6;
        set_slot_value(0x5C, -1);
        set_slot_value(0x5E, -1);
    }
}
END_NONMATCH

// Moves the traveler along its path to the target (travel_aim_at_target), timer / 2 px a frame
// (dx counts the distance). Once it has covered the path, spawns a copy of its data's template
// (kind 0x4066, action 1) at the data's spawn point, which waits 6 frames and is removed
// (wait_then_remove_part); the traveler itself shrinks away over 0x10 frames (settle_aim_then_finish).
void travel_then_spawn_home(struct Actor *obj)
{
    struct Data *data;
    struct Template *template;
    struct Actor *spawned;
    s32 pathLength;
    u16 *distance;

    distance = &obj->dx;
    *distance = (obj->timer << 7) + *distance;
    pathLength = travel_aim_at_target(obj);
    if (((s16)*distance << 1) >= pathLength)
    {
        data = obj->data;
        template = (struct Template *)((u8 *)data + 8);
        spawned = obj_spawn(data, 0x4066, data->spawnX / 0x100, data->spawnY / 0x100, data->spawnH / 0x100, 0, 1);
        sprite_set_action(spawned, 1, 0, 0);
        spawned->listSlot = 0xFF;
        ((struct Flags79Bit0 *)&spawned->flags79)->bit0 = 1;
        ((struct ActionBits *)&spawned->action)->mode = 3;
        spawned->owner = template->owner;
        spawned->anim = template->anim;
        obj_list_push_arg(spawned, 0);
        copy_template_flags(spawned, data);
        spawned->pose->mode = 0;
        spawned->state = wait_then_remove_part;
        spawned->step = 6;
        obj->child = spawned;
        obj->step = 0x10;
        obj->state = settle_aim_then_finish;
    }
}

// The traveler's last state: keeps it on its path (travel_aim_at_target) while it shrinks away,
// its affine matrix the data's direction (dirX, dirY) rotation scaled by 18 / step as step counts
// down from 0x10. At step 1 it clears data->mark, stops sound 0x5E and removes itself.
void settle_aim_then_finish(struct Actor *obj)
{
    u16 *step;
    s32 cosScaled;
    s32 sinScaled;
    struct AffineSet *group;

    travel_aim_at_target(obj);
    step = &obj->step;
    *step = *step - 1;
    if ((s16)*step <= 0x12)
    {
        cosScaled = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->data->dirX * 18, (s16)*step);
        sinScaled = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->data->dirY * 18, (s16)*step);
        group = oam_affine_group_of(obj);
        group->pa = cosScaled;
        group->pb = sinScaled;
        group->pc = -sinScaled;
        group->pd = cosScaled;
        if ((s16)*step == 1)
        {
            obj->data->mark = 0;
            sound_stop_song(0x5E);
            obj_release(obj);
        }
    }
}

ASM_FUNC("asm/code_0808AE9C/wait_then_spawn_walker.s", void wait_then_spawn_walker(struct Actor *obj))

// Walks the object 3 px a frame (dx counts the distance) along its path to the target
// (travel_aim_at_target). Once it has covered the path, spawns a copy of its data's template
// (kind 0x4066, action 1) at the data's spawn point, which waits 60 frames and then moves to the
// partner (wait_then_start_motion); the walker itself shrinks away (settle_aim_then_remove).
void walk_then_spawn_home(struct Actor *obj)
{
    struct Data *data;
    struct Template *template;
    struct Actor *spawned;
    s32 pathLength;

    obj->dx += 0x300;
    pathLength = travel_aim_at_target(obj);
    if (((s16)obj->dx << 1) < pathLength)
        return;
    data = obj->data;
    template = (struct Template *)((u8 *)data + 8);
    spawned = obj_spawn(data, 0x4066, data->spawnX / 0x100, data->spawnY / 0x100, data->spawnH / 0x100, 0, 1);
    sprite_set_action(spawned, 1, 0, 0);
    spawned->pose->mode = 0;
    spawned->listSlot = 0xFF;
    spawned->owner = template->owner;
    ((struct Flags79Bit0 *)&spawned->flags79)->bit0 = 1;
    spawned->anim = template->anim;
    obj_list_push_arg(spawned, 0);
    copy_template_flags(spawned, data);
    spawned->step = 60;
    spawned->state = wait_then_start_motion;
    obj->child = spawned;
    obj->step = 0;
    obj->state = settle_aim_then_remove;
}

// The walker's last state (after walk_then_spawn_home): keeps it on its path (travel_aim_at_target).
// While step is non-zero (walk_then_spawn_home leaves it at 0 until it is set) it counts step down and
// shrinks away, its affine matrix the data's direction (dirX, dirY) rotation scaled by 16 / step.
// At step 1 it stops sound 0x5E, clears data->mark and removes the object it spawned and itself.
void settle_aim_then_remove(struct Actor *obj)
{
    u16 *step;
    u16 before;
    s32 cosScaled;
    s32 sinScaled;
    struct AffineSet *group;

    travel_aim_at_target(obj);
    step = &obj->step;
    before = *step;
    if (*(s16 *)step != 0)
    {
        *step = before - 1;
        cosScaled = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->data->dirX << 4, (s16)*step);
        sinScaled = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->data->dirY << 4, (s16)*step);
        group = oam_affine_group_of(obj);
        group->pa = cosScaled;
        group->pb = sinScaled;
        group->pc = -sinScaled;
        group->pd = cosScaled;
        if ((s16)*step <= 1)
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

void set_action_by_mode(struct Actor *obj)
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
    switch (obj->mode)
    {
    case 1:
        wait = 0x18;
        break;
    case 0:
    case 2:
        wait = 0x18;
        break;
    case 3:
        wait = 0x18;
        break;
    }
    obj->step = wait;
    obj->state = wait_then_set_action;
}

void part_jump_setup(struct Actor *obj);
/* Once the actor's pose is visible: plays its action for its mode, then spawns part 0x405D to the actor's
 * left (offsets depend on the mode), hands it the actor's parameters and starts its jump. */
void spawn_part_by_mode(struct Actor *obj)
{
    s32 nearXOff = 0x1B;
    s32 xOff;
    s32 yOff;
    s32 x;
    s32 y;
    struct Actor *part;
    struct Actor *owner;

    if (obj->pose->visible)
    {
        switch (obj->mode)
        {
        case 1:
            sprite_set_action(obj, 9, 0, 0);
            break;
        case 0:
        case 2:
            sprite_set_action(obj, 5, 0, 0);
            break;
        case 3:
            sprite_set_action(obj, 9, 0, 0);
            break;
        }
        obj->state = pose_update_if_visible_a;
        set_slot_value(0x11A, -1);
        owner = obj->owner;
        switch (obj->mode)
        {
        case 1:
            xOff = nearXOff;
            break;
        case 0:
        case 2:
            xOff = 0x35;
            break;
        case 3:
            xOff = 0x1B;
            break;
        }
        x = (obj->x / 0x100) - xOff;
        switch (obj->mode)
        {
        case 1:
            yOff = -5;
            break;
        case 0:
        case 2:
            yOff = -9;
            break;
        case 3:
            yOff = -5;
            break;
        }
        y = (obj->y / 0x100) + yOff;
        part = obj_spawn(obj->data, 0x405D, x, y, obj->h / 0x100, 1, 1);
        obj->child = part;
        part->listSlot = 0xFF;
        ((struct Flags78Bits *)&part->flags78)->kind = 1;
        part->timer = obj->timer;
        part->paramA0 = obj->paramA0;
        part->paramA4 = obj->paramA4;
        part->buttons = obj->buttons;
        part->anim = obj->anim;
        part->mode = obj->mode;
        part->owner = owner;
        part_jump_setup(part);
    }
}

ASM_FUNC("asm/code_0808AE9C/part_jump_setup.s", void part_jump_setup(struct Actor *obj))

ASM_FUNC("asm/code_0808AE9C/jump_arc_step.s", void jump_arc_step(struct Actor *obj))

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
