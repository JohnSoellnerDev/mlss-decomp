#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xA8];
    s32 countA8;
    u8 fillerAC[0xB0 - 0xAC];
    s32 radiusB0;
    u8 fillerB4[0xD8 - 0xB4];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
};

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x28 - 0x1C];
    struct ActorPart *part;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)();
    u8 filler50[0x60 - 0x50];
    u32 (*handler60)();
    u8 filler64[0x68 - 0x64];
    void (*callback68)();
    s32 copy6C;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 flags77;
    u8 flags78;
    u8 flags;
    s16 motionStart;
    u16 speed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 scale;
    s32 paramA0;
    s32 paramA4;
    s32 count;
    u16 timer;
    s16 phase;
    s16 variant;
};

struct ObjFlagBits
{
    u8 lo5 : 5;
    u8 b5 : 1;
    u8 rest : 2;
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

struct SceneWorld
{
    u8 filler0[0x70];
    u8 *cand70;
    u8 *cand74;
};

extern struct SceneWorld *gWorld;
extern u16 gUnk_03000E80[];
extern u16 gUnk_03000E98[];
extern s16 gSineTable[];
extern s16 gCosineTable[];

void action2_then_spawn_child();
void action4_effect_then_chain();
void action6_effect_event();
void action_by_owner_slot_then_wait();
void obj_place_part_then_step();
void wait_child_then_next();
void spawn_child_at_base();
void start_slide_to_owner();
void timer_then_target_from_owner();
void wait_child_then_count();
void wait_scene_objects_then_timer();
void prepare_slide_timing();
void spawn_child_chain();
void spawn_orbit_chain();
void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 arg, u32 extra);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 lookup_slot(struct ActorObj *obj);
u32 mt19937_next(void);
void set_slot_value(u32 id, s32 value);
void set_slot_state(u32 state);
void sound_stop_song(u32 song);
void start_motion(struct ActorObj *obj);
void partner_link_update(struct ActorObj *a, struct ActorObj *b, u32 flags);
void wait_timer_then_settle();
void wait_index_then_timer(struct ActorObj *obj);
void wait_timer_then_stop(struct ActorObj *obj);
void finish_child_chain(struct ActorObj *obj);
void timer_down_then_step(struct ActorObj *obj);
void spawn_phase_child();
void orbit_step_action2(struct ActorObj *obj);
void orbit_step_finish();
void orbit_step_action1(struct ActorObj *obj);
void count_frames_then_timer(struct ActorObj *obj);
u32 lookup_then_finish(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void on_arrive();
void settle_to_data_target();
void step_child_chain(struct ActorObj *obj);
void step_child_chain_b(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void stop_when_pose_active(struct ActorObj *obj);
void timer_then_wait_child(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);

ASM_FUNC("asm/code_080739F0/spawn_orbit_chain.s", void spawn_orbit_chain(void))

ASM_FUNC("asm/code_080739F0/rotate_duo_step.s", void rotate_duo_step(void))

/*
 * State: once the pose has finished (flag 8), switch to action 5, spawn
 * effect 0x22E8 at the part's target and build a linked chain of obj->scale
 * orbiting children (type 0x414E) 10 pixels above the base, each one's
 * phase spread evenly around the circle; then count 0x30 frames.
 */
void spawn_child_chain(struct ActorObj *obj)
{
    struct ActorObj *link;
    struct ActorObj *next;
    s32 *chainLen;
    s32 i;

    if (obj->pose->flags & 8) {
        sprite_set_action(obj, 5, 0, 0);
        set_slot_value(0x116, -1);
        set_slot_value(0xD5, -1);
        effect_spawn_at(0x22E8, obj->part->targetX / 0x100, obj->part->targetY / 0x100,
                        obj->part->targetH / 0x100, obj);
        chainLen = &obj->scale;

        /* first link: hangs off obj */
        link = obj_spawn(obj->part, 0x414E, obj->baseX / 0x100, obj->baseY / 0x100 - 0xA,
                         obj->baseH / 0x100, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = link;
        ((struct Flags78Bits *)&link->flags78)->kind = 1;
        link->byte74 = obj->byte74;
        link->owner = obj->owner;
        ((struct ObjFlags77 *)link)->bit6 = 0;
        ((struct ObjFlags77 *)link)->bit5 = 0;
        link->callback68 = on_arrive;
        link->copy6C = obj->copy6C;
        link->scale = 0;
        link->paramA4 = 0;
        obj_list_push_default(link);
        link->state = orbit_step_action1;
        link->handler60 = lookup_then_finish;
        link->targetX = obj->baseX;
        link->targetY = obj->baseY - 0xA00;

        /* the rest: each hangs off the previous link */
        for (i = 1; i < *chainLen; i++) {
            next = obj_spawn(obj->part, 0x414E, obj->baseX / 0x100, obj->baseY / 0x100 - 0xA,
                             obj->baseH / 0x100, ((u32)obj->pose->unk11 << 25) >> 31, 1);
            link->child = next;
            link = next;
            link->flags78 = (link->flags78 & 0x1F) | 0x20; /* kind = 1 */
            link->byte74 = obj->byte74;
            link->owner = obj->owner;
            ((struct ObjFlags77 *)link)->bit6 = 0;
            ((struct ObjFlags77 *)link)->bit5 = 0;
            link->callback68 = on_arrive;
            link->copy6C = obj->copy6C;
            link->scale = i;
            link->paramA4 = ((i << 8) / *chainLen) << 8;
            obj_list_push_default(link);
            link->state = orbit_step_action1;
            link->handler60 = lookup_then_finish;
            link->targetX = obj->baseX;
            link->targetY = obj->baseY - 0xA00;
        }
        link->child = 0;
        obj->paramA0 = 0;
        obj->count = 0x30;
        obj->state = count_frames_then_timer;
    }
}

void wait_scene_objects_then_timer(struct ActorObj *obj)
{
    struct SceneWorld *scene = gWorld;
    u8 *first = scene->cand70;
    u8 *second = scene->cand74;
    s32 t;
    if ((first[0x7E] & 6) != 2) {
        if ((second[0x7E] & 6) != 2) {
            obj->timer = 0;
        }
    }
    t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0) {
        obj->count = 0;
        obj->timer = 0x3C;
        obj->state = timer_down_then_step;
    }
}

/*
 * State of an orbiting chain link (spawn_child_chain): circles its centre (targetX/targetY) on a
 * flattened ellipse, part->radiusB0 wide and a third of that high. paramA4 is the angle in 1/256
 * steps of a 256-step turn; it goes on by one step a frame while part->countA8 is set, else by
 * 0xE0. Once the pose has finished (flag 8) it takes action 1 and orbit_step_action2 takes over.
 */
void orbit_step_action1(struct ActorObj *obj)
{
    struct ActorPart *part = obj->part;
    s32 radius = part->radiusB0;
    s32 *centerX = &obj->targetX;
    s16 *table = gCosineTable;
    s32 *angle = &obj->paramA4;
    s32 *centerY;
    s32 step;
    s32 value;
    s32 sine;
    s32 nextAngle;
    step = *angle;
    if (step < 0) {
        step += 0xFF;
    }
    step >>= 8;
    value = table[step & 0xFF];
    if (value < 0) {
        value += 0x3F;
    }
    obj->posX = *centerX + (value >> 6) * radius;
    centerY = &obj->targetY;
    table = gSineTable;
    step = *angle;
    if (step < 0) {
        step += 0xFF;
    }
    step >>= 8;
    value = table[step & 0xFF];
    if (value < 0) {
        value += 0x3F;
    }
    sine = value >> 6;
    obj->posY = *centerY - sine * (radius / 3);
    if (part->countA8 != 0) {
        nextAngle = *angle + 0x100;
    } else {
        nextAngle = *angle + 0xE0;
    }
    *angle = nextAngle;
    if (obj->pose->flags & 8) {
        sprite_set_action(obj, 1, 0, 0);
        obj->state = orbit_step_action2;
    }
}

/*
 * State of an orbiting chain link after orbit_step_action1: keeps circling its centre
 * (targetX/targetY) on the same flattened ellipse, part->radiusB0 wide and a third of that high,
 * the angle (paramA4) going on by 0x100 a frame while part->countA8 is set, else by 0xE0. Once
 * the part's radius is down to 0 it takes action 2 and orbit_step_finish takes over.
 */
void orbit_step_action2(struct ActorObj *obj)
{
    struct ActorPart *part = obj->part;
    s32 radius = part->radiusB0;
    s32 *centerX = &obj->targetX;
    s16 *table = gCosineTable;
    s32 *angle = &obj->paramA4;
    s32 *centerY;
    s32 step;
    s32 value;
    s32 sine;
    s32 nextAngle;
    step = *angle;
    if (step < 0) {
        step += 0xFF;
    }
    step >>= 8;
    value = table[step & 0xFF];
    if (value < 0) {
        value += 0x3F;
    }
    obj->posX = *centerX + (value >> 6) * radius;
    centerY = &obj->targetY;
    table = gSineTable;
    step = *angle;
    if (step < 0) {
        step += 0xFF;
    }
    step >>= 8;
    value = table[step & 0xFF];
    if (value < 0) {
        value += 0x3F;
    }
    sine = value >> 6;
    obj->posY = *centerY - sine * (radius / 3);
    if (part->countA8 != 0) {
        nextAngle = *angle + 0x100;
    } else {
        nextAngle = *angle + 0xE0;
    }
    *angle = nextAngle;
    if (obj->part->radiusB0 == 0) {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = orbit_step_finish;
    }
}

/*
 * Last state of an orbiting chain link (after orbit_step_action2, action 2): circles its centre
 * (targetX/targetY) on a fixed ellipse 0x90 wide and 0x30 high, the angle (paramA4) going on by
 * 0x100 a frame while part->countA8 is set, else by 0xE0. Once the pose has finished (flag 8) the
 * link is released.
 */
void orbit_step_finish(struct ActorObj *obj)
{
    s32 *centerX = &obj->targetX;
    s16 *table = gCosineTable;
    s32 *angle = &obj->paramA4;
    s32 *centerY;
    s32 step;
    s32 value;
    s32 radiusX;
    s32 radiusY;
    s32 nextAngle;
    radiusX = 0x90;
    radiusY = 0x30;
    step = *angle;
    if (step < 0) {
        step += 0xFF;
    }
    step >>= 8;
    value = table[step & 0xFF];
    if (value < 0) {
        value += 0x3F;
    }
    obj->posX = *centerX + (value >> 6) * radiusX;
    centerY = &obj->targetY;
    table = gSineTable;
    step = *angle;
    if (step < 0) {
        step += 0xFF;
    }
    step >>= 8;
    value = table[step & 0xFF];
    if (value < 0) {
        value += 0x3F;
    }
    obj->posY = *centerY - (value >> 6) * radiusY;
    if (obj->part->countA8 != 0) {
        nextAngle = *angle + 0x100;
    } else {
        nextAngle = *angle + 0xE0;
    }
    *angle = nextAngle;
    if (obj->pose->flags & 8) {
        obj_release(obj);
    }
}

u32 lookup_then_finish(struct ActorObj *obj)
{
    u32 found = lookup_slot(obj);
    if (found == 0) {
        effect_spawn_at(0x22F0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
        return 0;
    }
    return found;
}

void timer_down_then_step(struct ActorObj *obj)
{
    s32 t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0) {
        obj->state = action6_effect_event;
    }
}

void count_frames_then_timer(struct ActorObj *obj)
{
    s32 n = obj->count + 1;
    obj->count = n;
    if (n > 0x71) {
        obj->count = 0x72;
        obj->timer = 0x72 + 0xBA;
        obj->state = wait_child_then_next;
    }
}

void chain_then_wait_counters(struct ActorObj *obj)
{
    struct ActorObj *grandchild = obj->child->child;
    u8 nonzero;
    s32 i;
    step_child_chain(obj->child);
    step_child_chain_b(grandchild);
    nonzero = 0;
    for (i = 0; i < 12; i++)
    {
        if (gUnk_03000E80[i] != 0)
            nonzero++;
        if (gUnk_03000E98[i] != 0)
            nonzero++;
    }
    if (nonzero == 0)
        obj->state = finish_child_chain;
}

void link_owner_then_wait(struct ActorObj *obj)
{
    s32 index = obj->index;
    struct ActorObj *owner;
    if (index == 0) {
        obj->pose->unkC = index;
        owner = obj->owner;
        resolve_contact_action(owner, obj, obj->posX / 0x100, obj->posH / 0x100,
                               owner->part->slot != -1 ? 0x13 : 0x12);
        obj->posH = obj->part->targetH;
        sprite_set_action(obj, 1, 0, 0);
        obj->timer = 0x1E;
        obj->state = wait_timer_then_stop;
    }
}

u32 lookup_then_slide(struct ActorObj *obj)
{
    u32 found = lookup_slot(obj);
    u32 result;
    if (found == 0) {
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->targetH = 0xC000;
        ((struct ObjFlagBits *)&obj->flags)->b5 = 1;
        obj->speed = 0x500;
        obj->motionStart = found;
        start_motion(obj);
        obj->timer = found;
        set_slot_value(0xA7, -1);
        obj->state = settle_to_data_target;
        result = 0;
    } else {
        result = found;
    }
    return result;
}

void notify_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0) {
        set_slot_value(0x10E, -1);
        obj->state = start_slide_to_owner;
    }
}

u32 lookup_then_notify(struct ActorObj *obj)
{
    u32 found = lookup_slot(obj);
    if (found == 0) {
        obj->state = prepare_slide_timing;
        set_slot_value(0xAD, -1);
        return 0;
    }
    return found;
}

u32 notify_effect_unlink_finish(struct ActorObj *obj)
{
    struct ActorPart *part;
    set_slot_value(0xAD, -1);
    part = obj->owner->part;
    effect_spawn_at(0x226E, part->targetX / 0x100, part->targetY / 0x100, part->targetH / 0x100, obj);
    partner_link_update(obj, obj->owner, 0xFF);
    obj->state = 0;
    obj_release(obj);
    return 0;
}

void wait_counter_then_finish(struct ActorObj *obj)
{
    s32 w = obj->targetX - 1;
    obj->targetX = w;
    if (w <= 0) {
        obj->state = 0;
        obj_release(obj);
    }
}

void finish_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0) {
        obj_release(obj);
    }
}

void settle_to_owner_data(struct ActorObj *obj)
{
    s32 index = obj->index;
    if (index == 0) {
        s32 *dst = &obj->targetX;
        struct ActorPart *part = obj->owner->part;
        *dst = part->targetX;
        obj->targetY = part->targetY;
        obj->targetH = part->targetH + 0x8000;
        ((struct ObjFlagBits *)&obj->flags)->b5 = 1;
        obj->speed = 0x300;
        obj->motionStart = index;
        start_motion(obj);
        obj->state = wait_index_then_timer;
    }
}

void pose_action0_then_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8) {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

u32 unknown_part_steps_08074c8e(struct ActorObj *obj)
{
    obj->state = action4_effect_then_chain;
    return 1;
}

u32 random_scale_then_next(struct ActorObj *obj)
{
    if (obj->scale == 0) {
        obj->scale = mt19937_next() & 3;
    }
    obj->state = obj_place_part_then_step;
    return 1;
}

u32 init_counter_then_next(struct ActorObj *obj)
{
    if (obj->scale == 0) {
        obj->scale = 5;
    }
    obj->state = action_by_owner_slot_then_wait;
    return 1;
}

u32 unknown_part_steps_08074ce4(struct ActorObj *obj)
{
    obj->state = action2_then_spawn_child;
    return 1;
}

void action6_effect_event(struct ActorObj *obj)
{
    sprite_set_action(obj, 6, 0, 0);
    effect_spawn_at(0x22E2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    sound_stop_song(0x116);
    obj->state = stop_when_pose_active;
}

void wait_child_then_next(struct ActorObj *obj)
{
    s32 t;
    if (obj->child == 0) {
        sound_stop_song(0xD5);
        obj->state = action6_effect_event;
        return;
    }
    t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0) {
        set_slot_state(1);
        obj->paramA0 = 1;
        obj->timer = 0xB4;
        obj->state = timer_then_wait_child;
    }
}

void action4_effect_then_chain(struct ActorObj *obj)
{
    sprite_set_action(obj, 4, 0, 0);
    effect_spawn_at(0x22DC, obj->part->targetX / 0x100, obj->part->targetY / 0x100, obj->part->targetH / 0x100, obj);
    obj->state = spawn_child_chain;
}

void finish_child_chain(struct ActorObj *obj)
{
    struct ActorObj *c1 = obj->child;
    struct ActorObj *c2 = c1->child;
    struct ActorObj *c3 = c2->child;
    obj_release(c3->child);
    obj_release(c3);
    obj_release(c2);
    obj_release(c1);
    sound_stop_song(0x5E);
    obj->state = 0;
}

void obj_place_part_then_step(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    effect_spawn_at(0x22C0, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj->state = spawn_orbit_chain;
}

void wait_timer_then_stop(struct ActorObj *obj)
{
    s32 t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0) {
        obj->state = 0;
    }
}

void action_by_owner_slot_then_wait(struct ActorObj *obj)
{
    u16 *tm;
    if (obj->owner->part->slot == -1) {
        sprite_set_action(obj, 0xC, 0, 0);
        tm = &obj->timer;
        *tm = 4;
        obj->state = wait_timer_then_settle;
    } else {
        sprite_set_action(obj, 0xD, 0, 0);
        tm = &obj->timer;
        *tm = 4;
        obj->state = wait_timer_then_settle;
    }
}

void wait_index_then_timer(struct ActorObj *obj)
{
    if (obj->index == 0) {
        obj->timer = 0x10;
        obj->state = timer_then_target_from_owner;
    }
}

void action2_then_spawn_child(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->state = spawn_child_at_base;
}

void stop_when_pose_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 8) {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void timer_then_wait_child(struct ActorObj *obj)
{
    s32 t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0) {
        obj->state = wait_child_then_count;
    }
}

void wait_child_then_count(struct ActorObj *obj)
{
    s32 n;
    if (obj->child == 0) {
        obj->state = action6_effect_event;
        return;
    }
    n = obj->count + 1;
    obj->count = n;
    if (n > 0x8F) {
        obj->timer = 0x12C;
        obj->state = wait_scene_objects_then_timer;
    }
}

/* State: every 8 frames spawns the phase's effect at the base (phase 0: 0x242B, phase 1: 0x2434,
   phase 2: both). Once the pose's animation has finished, counts targetX down; at 0 stops song
   0x6C, plays the phase's action (4, 8 or 0xC) and goes on to spawn_phase_child. */
void place_parts_by_phase_then_wait(struct ActorObj *obj)
{
    s32 timer;
    s32 wait;
    timer = obj->timer - 1;
    obj->timer = timer;
    if ((s16)timer <= 0) {
        if (obj->phase == 0 || obj->phase == 2) {
            effect_spawn_at(0x242B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        /* phase 1 or 2 */
        if ((u16)(obj->phase - 1) <= 1) {
            effect_spawn_at(0x2434, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->timer = 8;
    }
    if (obj->pose->flags & 8) {
        wait = obj->targetX - 1;
        obj->targetX = wait;
        if (wait <= 0) {
            sound_stop_song(0x6C);
            switch (obj->phase) {
            case 0:
                sprite_set_action(obj, 4, 0, 0);
                break;
            case 1:
                sprite_set_action(obj, 8, 0, 0);
                break;
            default:
                sprite_set_action(obj, 0xC, 0, 0);
                break;
            }
            obj->state = spawn_phase_child;
        }
    }
}

ASM_FUNC("asm/code_080739F0/spawn_phase_child.s", void spawn_phase_child(void))
