#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0x12];
    u8 unk12_0 : 1;
    u8 mode : 2;
    u8 visible : 1;
    u8 hold : 1;
    u8 unk12_5 : 1;
    u8 unk12_6 : 2;
    u8 filler13[0x21 - 0x13];
    u8 kind;
};

struct SpriteNode
{
    struct SpriteNode *next;
    s32 x;
    s32 y;
    s32 z;
    s16 vx;
    s16 vy;
    s16 vz;
};

struct SlotList
{
    u8 filler0[0x8];
    struct SpriteNode *head;
};

struct Data
{
    u8 filler0[0xEC];
    s16 slot;
    u8 fillerEE[0xF5 - 0xEE];
    s8 unkF5;
    u8 fillerF6[0x106 - 0xF6];
    u16 rate;
    s16 unk108;
    u8 filler10A[0x114 - 0x10A];
    u32 slotValue;
    u8 filler118[0x11E - 0x118];
    u16 input;
};

struct Actor
{
    u8 filler0[0x4];
    struct SlotList *slots;
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    u8 filler14[0x18 - 0x14];
    s32 z;
    s16 rate;
    u16 unk1E;
    u16 timer;
    u8 filler22[0x24 - 0x22];
    u16 pair;
    u8 filler26[0x28 - 0x26];
    struct Data *data;
    struct Actor *partner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 candidateLimit;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct Actor *);
    void (*resume)(struct Actor *);
    u8 filler54[0x6C - 0x54];
    const u8 *anim;
    u8 filler70[0x75 - 0x70];
    u8 count;
    u8 flags76;
    u8 filler77;
    u8 flags78;
    u8 flags79;
    u16 handle;
    u16 scale;
    u16 phase;
    u8 filler80[0x92 - 0x80];
    s16 unk92;
    s32 speedLo;
    s32 speedHi;
    s32 time;
    s32 clock;
    s32 repeats;
    u8 fillerA8[0xAC - 0xA8];
    s16 step;
    u8 fillerAE[0xC3 - 0xAE];
    u8 offX;
    u8 offY;
    u8 offZ;
};

struct Hero
{
    u8 filler0[0x08];
    struct Actor body;
};

struct Partner
{
    u8 filler0[0x7E];
    u8 flags7E;
    u8 filler7F[0xF5 - 0x7F];
    s8 unkF5;
    u8 fillerF6[0x106 - 0xF6];
    s16 unk106;
    s16 unk108;
};

struct World
{
    u8 filler0[0x58];
    struct Partner *partners[6];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x7C - 0x78];
    struct Hero *unk7C;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 *gWorkBuf;

void sprite_set_action(struct Actor *obj, u32 action, u32 part, u32 extra);
u8 step_motion(struct Actor *obj);
void obj_release(void *obj);
void obj_detach_active(struct Pose *pose);
void state_stack_advance_progress(struct Actor *obj);
void sprite_node_take_link(struct Actor *obj);
struct Actor *obj_spawn(struct Data *data, u32 kind, s32 x, s32 y, s32 h, s32 a, s32 b);
void pair_partner_swap(struct Actor *obj);
void set_slot_value(u32 id, s32 value);
struct Actor *obj_spawn_linked(struct Actor *parent, u32 kind, u32 value);
void child_visible_check(struct Actor *obj);

void pair_children_end(struct Actor *obj);
void child_hold_settle(struct Actor *obj);
void spawn_marker_child(struct Actor *obj);
void land_at_target_height(struct Actor *obj);
void wait_motion_end(struct Actor *obj);
void pair_countdown_spawn(struct Actor *obj);
void release_partner_when_visible(struct Actor *obj);
void motion_done_start_wait(struct Actor *obj);
void wait_countdown_heroes_anim(struct Actor *obj);
void hero_a_anim_when_visible(struct Actor *obj);
void hero_b_anim_and_mode(struct Actor *obj);
void heroes_release_anim_when_visible(struct Actor *obj);
u32 pair_begin_setup(struct Actor *obj);
void state_pair_setup_motion(struct Actor *obj);
void pair_end_heroes(struct Actor *obj);
void spawn_child_on_visible(struct Actor *obj);
void hero_random_walk(struct Actor *obj);
void pair_finish_handle(struct Actor *obj);
void pair_repeat_countdown(struct Actor *obj);
void start_random_handle(struct Actor *obj);
void sprite_nodes_fall(struct Actor *obj);
void spawn_fall_particle(struct Actor *obj);
void pair_switch_wait(struct Actor *obj);
void hero_random_walk_reset(struct Actor *obj);
void hero_detach_wait(struct Actor *obj);
void hero_motion_done(struct Actor *obj);
void child_release_when_empty(struct Actor *obj);
u32 pair_begin_setup_b(struct Actor *obj);
void pair_sum_offsets(struct Actor *obj);
void schedule_pose_task(struct Actor *partner);
void follow_focus_settle(struct Actor *obj);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void obj_free_parts(struct Actor *obj);
void place_targets(struct Actor *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct Actor *obj, u32 value);


ASM_FUNC("asm/code_08097238/pair_children_end.s", void pair_children_end(struct Actor *obj))

ASM_FUNC("asm/code_08097238/child_hold_settle.s", void child_hold_settle(struct Actor *obj))

void spawn_marker_child(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *marker;

    if ((obj->flags76 & 6) == 2) {
        u32 action = 0x2051;

        if (obj == heroA) {
            action -= 0x2F;
        }
        sprite_set_action(obj, 1, action, 0);
        obj->pose->mode = 1;
    }
    marker = obj_spawn_linked(obj, 0, 0);
    sprite_set_action(marker, 1, 0x208D, 0);
    marker->flags78 = (marker->flags78 & 0x1F) | 0x20;
    obj->state = pair_partner_swap;
}

void land_at_target_height(struct Actor *obj)
{
    step_motion(obj);
    if (obj->targetZ >= obj->z) {
        sprite_set_action(obj, 6, -1, 0);
        obj->pose->mode = 1;
        obj->state = motion_done_start_wait;
    }
}

void wait_motion_end(struct Actor *obj)
{
    u8 still = step_motion(obj);
    if (still == 0) {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
    }
}

void pair_countdown_spawn(struct Actor *obj)
{
    u16 step = obj->step;

    if ((s16)obj->step > 0) {
        obj->step = step - 1;
    } else {
        obj_spawn_linked(obj, 0x208C, 0);
        obj->child->count = obj->count - 1;
        sprite_set_action(obj->child, 0, 0, 0);
        obj->state = child_visible_check;
    }
}

void release_partner_when_visible(struct Actor *obj)
{
    struct Actor *partner = obj->partner;

    if (partner->pose->visible) {
        u32 mode = partner->flags76 & 6;

        if (mode == 2 || mode == 4) {
            sprite_set_action(partner, -1, -1, 0);
            partner->pose->mode = 0;
        }
        obj_release(obj);
    }
}

void motion_done_start_wait(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;

    if (step_motion(obj) == 0) {
        u32 action = 0x2050;

        if (obj == heroA) {
            action -= 0x4D;
        }
        sprite_set_action(obj, 0, action, 0);
        obj->pose->mode = 0;
        obj->step = 0x3C;
        obj->state = wait_countdown_heroes_anim;
    }
}

void wait_countdown_heroes_anim(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;
    u16 step = obj->step;

    if ((s16)obj->step > 0) {
        obj->step = step - 1;
    } else {
        sprite_set_action(heroA, 7, 0x2039, 0);
        sprite_set_action(heroB, 7, 0x2068, 0);
        obj->pose->mode = 1;
        obj->state = hero_a_anim_when_visible;
    }
}

void hero_a_anim_when_visible(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;

    if (obj->pose->visible) {
        set_slot_value(6, -1);
        sprite_set_action(heroA, 8, 0x2039, 0);
        heroA->pose->mode = 1;
        obj->state = hero_b_anim_and_mode;
    }
}

void hero_b_anim_and_mode(struct Actor *obj)
{
    struct Actor *heroB = &gWorld->heroB->body;

    set_slot_value(0x18, -1);
    sprite_set_action(heroB, 8, 0x2068, 0);
    heroB->pose->mode = 1;
    obj->state = heroes_release_anim_when_visible;
}

void heroes_release_anim_when_visible(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;

    if (obj->pose->visible) {
        sprite_set_action(heroA, -1, -1, 0);
        sprite_set_action(heroB, -1, -1, 0);
        obj->state = 0;
    }
}

u32 pair_begin_setup(struct Actor *obj)
{
    u8 *entry = gParty - (obj->data->slot * 0x3C + 0x3C);
    struct Hero *heroA;
    struct Hero *heroB;
    struct Actor *bodyA;
    struct Actor *bodyB;
    struct Actor *partner;
    const u8 *anim;

    obj->data->slotValue = *(u16 *)(entry + 0x2A);
    heroA = gWorld->heroA;
    bodyA = &heroA->body;
    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    partner = obj->partner;
    bodyB->partner = partner;
    bodyA->partner = partner;
    anim = obj->anim;
    bodyB->anim = anim;
    bodyA->anim = anim;
    partner->resume = 0;
    bodyB->resume = 0;
    bodyA->resume = 0;
    partner->scale = 0x100;
    heroB->body.scale = 0x100;
    heroA->body.scale = 0x100;
    obj->state = state_pair_setup_motion;
    bodyB->pair = 0;
    bodyA->pair = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_08097238/state_pair_setup_motion.s", void state_pair_setup_motion(struct Actor *obj))

void pair_end_heroes(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;
    struct Hero *hero;
    u8 mode;

    if (obj == heroA) {
        sprite_set_action(obj, 1, 0x2027, 0);
    }
    if (obj == heroB) {
        sprite_set_action(obj, 1, 0x2056, 0);
    }
    obj->pose->mode = 1;
    hero = gWorld->unk7C;
    if (obj == &hero->body) {
        mode = hero->body.flags76 & 6;
        if (mode == 2 || mode == 4) {
            place_targets(obj, obj->targetX / 0x100 + (s8)hero->body.offX + 10,
                          obj->targetY / 0x100 + (s8)hero->body.offY, 0, -1);
            hero->body.unk92 = obj->targetZ / 0x100 + 0xe;
            set_motion_offsets(obj, 0x200);
        }
    } else {
        mode = obj->flags76 & 6;
        if (mode == 2 || mode == 4) {
            place_targets(obj, obj->targetX / 0x100 + (s8)obj->offX + 0xe,
                          obj->targetY / 0x100 + (s8)obj->offY, 0, -1);
            obj->unk92 = obj->targetZ / 0x100 + 0xe;
            set_motion_offsets(obj, 0x200);
        }
    }
    set_slot_value(0x2b, -1);
    obj->state = hero_motion_done;
}

void spawn_child_on_visible(struct Actor *obj)
{
    struct Actor *heroB = &gWorld->heroB->body;
    struct Actor *child;
    u32 mode;

    if (obj->pose->visible) {
        child = obj_spawn(heroB->data, 0x2089, 0, -0x40, -1, 0, 1);
        sprite_set_action(child, 0, 0, 0);
        child->flags78 = child->flags78 & 0x1F;
        obj->child = child;
        sprite_node_take_link(child);
        mode = child->flags76 & 6;
        if (mode == 2 || mode == 4) {
            child->state = sprite_nodes_fall;
        }
        hero_random_walk_reset(obj);
    }
}

ASM_FUNC("asm/code_08097238/hero_random_walk.s", void hero_random_walk(struct Actor *obj))

void pair_finish_handle(struct Actor *obj)
{
    s32 n;
    u8 bits;

    obj_free_parts(obj);
    if (obj == &gWorld->heroA->body)
    {
        sprite_set_action(obj, 3, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 3, 0, 0);
    }
    obj->pose->mode = 1;
    set_slot_value(0x2e, -1);
    bits = obj->flags76 & 6;
    if (bits == 2 || bits == 4)
    {
        n = rand_below(3, mt19937_next());
        obj->handle = *(s32 *)(gWorkBuf + 0x5480) + n * *(s32 *)(gWorkBuf + 0x5484) != 0
            ? MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, *(s32 *)(gWorkBuf + 0x5480) + rand_below(3, mt19937_next()) * *(s32 *)(gWorkBuf + 0x5484))
            : 0x4000;
        obj->phase = 0;
        *((s8 *)obj + 0x79) &= -0x21;
    }
    obj->clock = 0;
    obj->repeats = *(s32 *)(gWorkBuf + 0x5488);
    obj->state = pair_repeat_countdown;
}

ASM_FUNC("asm/code_08097238/pair_repeat_countdown.s", void pair_repeat_countdown(struct Actor *obj))

ASM_FUNC("asm/code_08097238/start_random_handle.s", void start_random_handle(struct Actor *obj))

s32 rand_below(s32 range, u32 seed);
u32 mt19937_next(void);
extern u8 *gWorkBuf;
ASM_FUNC("asm/code_08097238/sprite_nodes_fall.s", void sprite_nodes_fall(struct Actor *obj))

ASM_FUNC("asm/code_08097238/spawn_fall_particle.s", void spawn_fall_particle(struct Actor *obj))

void pair_switch_wait(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;

    state_stack_advance_progress(obj);
    if (obj->flags79 & 0x20) {
        if (obj == heroA && heroB->state == hero_random_walk) {
            heroB->state = pair_finish_handle;
        }
        if (obj == heroB && heroA->state == hero_random_walk) {
            heroA->state = pair_finish_handle;
        }
        obj->state = pair_end_heroes;
    }
}

void hero_random_walk_reset(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;

    if (obj == heroA) {
        sprite_set_action(obj, 1, 0x2027, 0);
    }
    if (obj == heroB) {
        sprite_set_action(obj, 1, 0x2056, 0);
    }
    obj->pose->mode = 0;
    obj->time = obj->targetX;
    obj->clock = 0;
    obj->handle = 0;
    obj->state = hero_random_walk;
    set_slot_value(0x3F, -1);
}

void hero_detach_wait(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;

    step_motion(obj);
    if (obj->flags79 & 0x20) {
        obj_detach_active(obj->pose);
        if (obj == heroA && !heroB->pose->unk12_5) {
            if (heroB->child != 0) {
                hero_random_walk_reset(heroB);
            } else {
                pair_end_heroes(heroB);
            }
        }
        if (obj == heroB && !heroA->pose->unk12_5) {
            if (heroA->child != 0) {
                hero_random_walk_reset(heroA);
            } else {
                pair_end_heroes(heroA);
            }
        }
        obj->state = child_release_when_empty;
    }
}

void hero_motion_done(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;

    step_motion(obj);
    if (obj->flags79 & 0x20) {
        if (obj == heroA) {
            sprite_set_action(obj, 0, 0, 0);
        }
        if (obj == heroB) {
            sprite_set_action(obj, 0, 0, 0);
        }
        obj->state = spawn_child_on_visible;
    }
}

void child_release_when_empty(struct Actor *obj)
{
    if (obj->child->slots->head->next == 0) {
        obj_release(obj->child);
        obj->child = 0;
        obj->state = 0;
    }
}

u32 pair_begin_setup_b(struct Actor *obj)
{
    u8 *entry = gParty - (obj->data->slot * 0x3C + 0x3C);
    struct Hero *heroA;
    struct Hero *heroB;
    struct Actor *bodyA;
    struct Actor *bodyB;
    struct Actor *partner;
    const u8 *anim;

    obj->data->slotValue = *(u16 *)(entry + 0x2A);
    heroA = gWorld->heroA;
    bodyA = &heroA->body;
    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    partner = obj->partner;
    bodyB->partner = partner;
    bodyA->partner = partner;
    anim = obj->anim;
    bodyB->anim = anim;
    bodyA->anim = anim;
    partner->resume = 0;
    bodyB->resume = 0;
    bodyA->resume = 0;
    partner->scale = 0x100;
    heroB->body.scale = 0x100;
    heroA->body.scale = 0x100;
    obj->state = pair_sum_offsets;
    bodyB->pair = 0;
    bodyA->pair = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_08097238/pair_sum_offsets.s", void pair_sum_offsets(struct Actor *obj))

void place_targets(struct Actor *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct Actor *obj, u32 value);
void end_hero_follow(struct Actor *obj);
void follow_focus_settle(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;
    struct Actor *focus;
    s32 mode;

    if (obj == heroA) {
        sprite_set_action(obj, 1, 0x2027, 0);
    }
    if (obj == heroB) {
        sprite_set_action(obj, 1, 0x2056, 0);
    }
    obj->pose->mode = 1;
    focus = &gWorld->unk7C->body;
    if (obj == focus) {
        mode = focus->flags76 & 6;
        if (mode == 2 || mode == 4) {
            s32 col = obj->targetX / 0x100 + (s8)focus->offX + 10;
            s32 row = obj->targetY / 0x100 + (s8)focus->offY;
            s16 z;

            place_targets(obj, col, row, 0, -1);
            z = obj->targetZ / 0x100 + 14;
            *(s16 *)((u8 *)focus + 0x92) = z;
            set_motion_offsets(obj, 0x200);
        }
    } else {
        mode = obj->flags76 & 6;
        if (mode == 2 || mode == 4) {
            s32 col = obj->targetX / 0x100 + (s8)obj->offX + 14;
            s32 row = obj->targetY / 0x100 + (s8)obj->offY;
            s16 z;

            place_targets(obj, col, row, 0, -1);
            z = obj->targetZ / 0x100 + 14;
            *(s16 *)((u8 *)obj + 0x92) = z;
            set_motion_offsets(obj, 0x200);
        }
    }
    obj->state = end_hero_follow;
}
