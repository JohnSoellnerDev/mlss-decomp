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

ASM_FUNC("asm/code_080739F0/spawn_orbit_chain.s", void spawn_orbit_chain(void))

ASM_FUNC("asm/code_080739F0/rotate_duo_step.s", void rotate_duo_step(void))

ASM_FUNC("asm/code_080739F0/spawn_child_chain.s", void spawn_child_chain(void))

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

void orbit_step_action1(struct ActorObj *obj)
{
    struct ActorPart *part = obj->part;
    s32 radius = part->radiusB0;
    s32 *tx = &obj->targetX;
    s16 *tab = gCosineTable;
    s32 *phase = &obj->paramA4;
    s32 *ty;
    s32 ph;
    s32 s;
    s32 sv;
    s32 next;
    ph = *phase;
    if (ph < 0) {
        ph += 0xFF;
    }
    ph >>= 8;
    s = tab[ph & 0xFF];
    if (s < 0) {
        s += 0x3F;
    }
    obj->posX = *tx + (s >> 6) * radius;
    ty = &obj->targetY;
    tab = gSineTable;
    ph = *phase;
    if (ph < 0) {
        ph += 0xFF;
    }
    ph >>= 8;
    s = tab[ph & 0xFF];
    if (s < 0) {
        s += 0x3F;
    }
    sv = s >> 6;
    obj->posY = *ty - sv * (radius / 3);
    if (part->countA8 != 0) {
        next = *phase + 0x100;
    } else {
        next = *phase + 0xE0;
    }
    *phase = next;
    if (obj->pose->flags & 8) {
        sprite_set_action(obj, 1, 0, 0);
        obj->state = orbit_step_action2;
    }
}

void orbit_step_action2(struct ActorObj *obj)
{
    struct ActorPart *part = obj->part;
    s32 radius = part->radiusB0;
    s32 *tx = &obj->targetX;
    s16 *tab = gCosineTable;
    s32 *phase = &obj->paramA4;
    s32 *ty;
    s32 ph;
    s32 s;
    s32 sv;
    s32 next;
    ph = *phase;
    if (ph < 0) {
        ph += 0xFF;
    }
    ph >>= 8;
    s = tab[ph & 0xFF];
    if (s < 0) {
        s += 0x3F;
    }
    obj->posX = *tx + (s >> 6) * radius;
    ty = &obj->targetY;
    tab = gSineTable;
    ph = *phase;
    if (ph < 0) {
        ph += 0xFF;
    }
    ph >>= 8;
    s = tab[ph & 0xFF];
    if (s < 0) {
        s += 0x3F;
    }
    sv = s >> 6;
    obj->posY = *ty - sv * (radius / 3);
    if (part->countA8 != 0) {
        next = *phase + 0x100;
    } else {
        next = *phase + 0xE0;
    }
    *phase = next;
    if (obj->part->radiusB0 == 0) {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = orbit_step_finish;
    }
}

void orbit_step_finish(struct ActorObj *obj)
{
    s32 *tx = &obj->targetX;
    s16 *tab = gCosineTable;
    s32 *phase = &obj->paramA4;
    s32 *ty;
    s32 ph;
    s32 s;
    s32 k;
    s32 k2;
    s32 next;
    k = 0x90;
    k2 = 0x30;
    ph = *phase;
    if (ph < 0) {
        ph += 0xFF;
    }
    ph >>= 8;
    s = tab[ph & 0xFF];
    if (s < 0) {
        s += 0x3F;
    }
    obj->posX = *tx + (s >> 6) * k;
    ty = &obj->targetY;
    tab = gSineTable;
    ph = *phase;
    if (ph < 0) {
        ph += 0xFF;
    }
    ph >>= 8;
    s = tab[ph & 0xFF];
    if (s < 0) {
        s += 0x3F;
    }
    obj->posY = *ty - (s >> 6) * k2;
    if (obj->part->countA8 != 0) {
        next = *phase + 0x100;
    } else {
        next = *phase + 0xE0;
    }
    *phase = next;
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

NONMATCH("asm/code_080739F0/action_by_owner_slot_then_wait.s", void action_by_owner_slot_then_wait(struct ActorObj *obj))
{
    u32 action;
    if (obj->owner->part->slot == -1) {
        action = 0xC;
    } else {
        action = 0xD;
    }
    sprite_set_action(obj, action, 0, 0);
    obj->timer = 4;
    obj->state = wait_timer_then_settle;
}
END_NONMATCH

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

NONMATCH("asm/code_080739F0/place_parts_by_phase_then_wait.s", void place_parts_by_phase_then_wait(struct ActorObj *obj))
{
    s32 t;
    s32 wait;
    t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0) {
        if (obj->phase == 0 || obj->phase == 2) {
            effect_spawn_at(0x242B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        if ((u16)(obj->phase - 1) <= 1) {
            effect_spawn_at(0x2434, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->timer = 8;
    }
    if (obj->pose->flags & 8) {
        wait = obj->targetX - 1;
        obj->targetX = wait;
        if (wait <= 0) {
            s32 action;
            sound_stop_song(0x6C);
            switch (obj->phase) {
            case 0:
                action = 4;
                break;
            case 1:
                action = 8;
                break;
            default:
                action = 0xC;
                break;
            }
            sprite_set_action(obj, action, 0, 0);
            obj->state = spawn_phase_child;
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080739F0/spawn_phase_child.s", void spawn_phase_child(void))
