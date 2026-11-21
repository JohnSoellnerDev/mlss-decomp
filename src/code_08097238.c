#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0x2];
    s16 unk2;
    u8 filler4[0x12 - 0x4];
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
    u16 vz;
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
    s16 rate;
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
    s16 timer;
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
    s16 arcHeight; /* peak of the jump arc to the target, read by set_motion_offsets */
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

// Actor.flags79: the motion has finished (state_stack_advance_progress went past 0x3FFF).
#define ACTOR_MOTION_DONE 0x20

// Animations heroA and heroB play when they go back to hero_random_walk (sprite_set_action mode 1).
#define HERO_A_WALK_ANIM 0x2027
#define HERO_B_WALK_ANIM 0x2056

// Sound played (set_slot_value queues it) when a hero goes back to hero_random_walk.
#define SOUND_PAIR_END 0x3F

// Pair motion tuning in the work buffer: a motion lasts PAIR_FRAMES_BASE plus 0-2 times
// PAIR_FRAMES_STEP frames and drops PAIR_PARTICLE_COUNT falling particles.
#define PAIR_FRAMES_BASE (*(s32 *)(gWorkBuf + 0x5480))
#define PAIR_FRAMES_STEP (*(s32 *)(gWorkBuf + 0x5484))
#define PAIR_PARTICLE_COUNT (*(s32 *)(gWorkBuf + 0x5488))

// Actor.handle for a motion of `frames` frames: the progress added per frame, 0x4000 being the
// whole motion (see state_stack_advance_progress). Evaluates `frames` twice.
#define MOTION_RATE(frames) \
    ((frames) != 0 ? MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, (frames)) : 0x4000)

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
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

struct MapEntry
{
    u8 filler0[0x2];
    u8 kind;
};

struct World
{
    u8 filler0[0xC];
    u8 flags;
    u8 filler0D[0x58 - 0xD];
    struct Partner *partners[6];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x7C - 0x78];
    struct Hero *unk7C;
    u8 filler80[0x24C - 0x80];
    void *unk24C;
    u8 filler250[0x2DC - 0x250];
    struct MapEntry *curEntry;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 *gWorkBuf;
extern u8 gSys[];

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
void state_partner_action8(struct Actor *obj);
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


s32 find_frame_entry(struct Actor *obj, s32 index);

static inline void child_place(struct Actor *child, struct Actor *src, struct Actor *partner)
{
    s32 px = src->targetX / 0x100 + (s8)src->offX + 4;
    s32 py = src->targetY / 0x100 + (s8)src->offY;
    s32 offZ;

    place_targets(child, px, py, src->targetZ / 0x100 + (s8)src->offZ - 8, -1);
    if (src == (struct Actor *)((u8 *)src->data + 8)) {
        offZ = (s8)src->offZ;
    } else {
        find_frame_entry(src, 0);
        offZ = (s8)src->candidateLimit;
    }
    offZ += partner->targetZ / 0x100;
    offZ += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x640, (offZ * offZ) / 0x40 + 0x28);
    child->arcHeight = offZ;
    set_motion_offsets(child, 0x480);
}

// Waits for the pair's motion to finish, then releases the last actor of the child chain and sends
// every other child to its next pose: each one is placed relative to its partner (to this actor when
// it is the partner) and left to child_hold_settle. This actor's state ends there.
void pair_children_end(struct Actor *obj)
{
    struct Actor *child;

    state_stack_advance_progress(obj);
    if (obj->flags79 & ACTOR_MOTION_DONE) {
        struct Actor *last = obj->child;

        while (last->child != 0) {
            last = last->child;
        }
        obj_release(last);
        set_slot_value(0xAE, -1);
        child = obj->child;
        while (child != 0) {
            struct Actor *partner = child->partner;

            if ((child->flags76 & 6) == 2 || (child->flags76 & 6) == 4) {
                sprite_set_action(child, 1, gWorld->curEntry->kind + 0x4188, 0);
                child->pose->mode = 1;
            }
            if (obj == partner) {
                if ((child->flags76 & 6) == 2 || (child->flags76 & 6) == 4) {
                    child_place(child, obj, partner);
                }
            } else {
                if ((child->flags76 & 6) == 2 || (child->flags76 & 6) == 4) {
                    child_place(child, partner, partner);
                }
            }
            child->state = child_hold_settle;
            child = child->child;
        }
        obj->state = 0;
    }
}

NONMATCH("asm/code_08097238/child_hold_settle.s", void child_hold_settle(struct Actor *obj))
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *partner = obj->partner;
    s32 inWindow;
    s32 q;
    s32 lo;
    s32 hi;

    step_motion(obj);
    lo = obj->speedLo;
    hi = obj->speedHi;
    q = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(hi << 14, lo);
    if (obj->step > q) {
        inWindow = 0;
    } else {
        inWindow = 1;
        if ((s16)obj->phase < q) {
            inWindow = 0;
        }
    }
    if (inWindow) {
        if ((obj->flags76 & 6) == 2 || (obj->flags76 & 6) == 4) {
            sprite_set_action(obj, 2, 0, 0);
            obj->pose->mode = 1;
        }
    }
    if (obj->flags79 & 0x20) {
        struct Actor *dataBody;

        set_slot_value(0x9F, -1);
        obj_detach_active(obj->pose);
        dataBody = (struct Actor *)((u8 *)obj->data + 8);
        if ((dataBody->flags76 & 6) == 2 || (dataBody->flags76 & 6) == 4) {
            sprite_set_action(dataBody, -1, -1, 0);
            dataBody->pose->mode = 0;
        }
        if ((partner->data->input & 0x300) == 0 && (partner->flags76 & 6) == 2) {
            u32 action = 0x2051;

            if (partner == heroA) {
                action -= 0x2F;
            }
            sprite_set_action(partner, 8, action, 0);
            partner->pose->mode = 1;
        }
        obj->state = state_partner_action8;
    }
}
END_NONMATCH

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

/* Ends a hero's pair move: sets hero A's (0x2027) or hero B's (0x2056) action, sets its pose mode to
   1 and, when the hero is in mode 2 or 4, sends it to its target position (whole pixels, plus its
   offsets, 10 to the right for the World.unk7C hero and 14 for the other), 14 pixels up, at speed
   0x200. Then sets slot 0x2B and waits in hero_motion_done. */
void pair_end_heroes(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;
    struct Hero *hero7C;
    u8 mode;

    if (obj == heroA) {
        sprite_set_action(obj, 1, 0x2027, 0);
    }
    if (obj == heroB) {
        sprite_set_action(obj, 1, 0x2056, 0);
    }
    obj->pose->mode = 1;
    hero7C = gWorld->unk7C;
    if (obj == &hero7C->body) {
        mode = hero7C->body.flags76 & 6;
        if (mode == 2 || mode == 4) {
            place_targets(obj, obj->targetX / 0x100 + (s8)hero7C->body.offX + 10,
                          obj->targetY / 0x100 + (s8)hero7C->body.offY, 0, -1);
            hero7C->body.arcHeight = obj->targetZ / 0x100 + 0xe;
            set_motion_offsets(obj, 0x200);
        }
    } else {
        mode = obj->flags76 & 6;
        if (mode == 2 || mode == 4) {
            place_targets(obj, obj->targetX / 0x100 + (s8)obj->offX + 0xe,
                          obj->targetY / 0x100 + (s8)obj->offY, 0, -1);
            obj->arcHeight = obj->targetZ / 0x100 + 0xe;
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

void set_body_part(struct Actor *obj, u32 art, s32 x, s32 y, u32 extra);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void set_motion_start(struct Actor *obj, u32 speed);

static inline void *walk_focus(void)
{
    return gWorld->unk24C;
}

ASM_FUNC("asm/code_08097238/hero_random_walk.s", void hero_random_walk(struct Actor *obj))

// Ends a hero's pair move: frees its parts, plays action 3 and, for flags76 modes 2 and 4,
// restarts its motion with a random duration (MOTION_RATE draws the random duration twice:
// once for the zero check, once for the division). Then counts down the falling particles.
void pair_finish_handle(struct Actor *obj)
{
    u8 mode;

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
    mode = obj->flags76 & 6;
    if (mode == 2 || mode == 4)
    {
        obj->handle = MOTION_RATE(PAIR_FRAMES_BASE + rand_below(3, mt19937_next()) * PAIR_FRAMES_STEP);
        obj->phase = 0;
        *(s8 *)&obj->flags79 &= ~ACTOR_MOTION_DONE;
    }
    obj->clock = 0;
    obj->repeats = PAIR_PARTICLE_COUNT;
    obj->state = pair_repeat_countdown;
}

// Plays animation `anim` on the hero's sprite (sprite_set_action mode 1).
static inline void hero_play_anim(struct Actor *obj, u32 anim)
{
    sprite_set_action(obj, 1, anim, 0);
}

// Plays sound `id` at the current volume.
static inline void play_sound(u32 id)
{
    set_slot_value(id, -1);
}

// State after pair_finish_handle: runs the motion, dropping a coin (spawn_fall_particle) every
// 2-5 frames until `repeats` runs out. When the motion is done, hands the partner (if it is
// walking) to pair_finish_handle, and sends this hero back to hero_random_walk.
void pair_repeat_countdown(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;

    state_stack_advance_progress(obj);
    if (obj->repeats != 0) {
        if (--obj->clock <= 0) {
            obj->clock = rand_below(4, mt19937_next()) + 2;
            obj->repeats--;
            spawn_fall_particle(obj);
        }
    }
    if (obj->flags79 & ACTOR_MOTION_DONE) {
        if (obj == heroA && heroB->state == hero_random_walk) {
            heroB->state = pair_finish_handle;
        }
        if (obj == heroB && heroA->state == hero_random_walk) {
            heroA->state = pair_finish_handle;
        }
        {
            struct Actor *curA = &gWorld->heroA->body;
            struct Actor *curB = &gWorld->heroB->body;

            if (obj == curA) {
                hero_play_anim(obj, HERO_A_WALK_ANIM);
            }
            if (obj == curB) {
                hero_play_anim(obj, HERO_B_WALK_ANIM);
            }
        }
        obj->pose->mode = 0;
        obj->time = obj->targetX;
        obj->clock = 0;
        obj->handle = 0;
        obj->state = hero_random_walk;
        play_sound(SOUND_PAIR_END);
    }
}

// When the actor is in pair mode 2 or 4 (flags76 & 6), starts a new motion lasting
// PAIR_FRAMES_BASE plus 0-2 times PAIR_FRAMES_STEP frames; then waits in pair_switch_wait.
void start_random_handle(struct Actor *obj)
{
    u8 mode = obj->flags76 & 6;

    if (mode == 2 || mode == 4) {
        obj->handle = MOTION_RATE(PAIR_FRAMES_BASE + rand_below(3, mt19937_next()) * PAIR_FRAMES_STEP);
        obj->phase = 0;
        *(s8 *)&obj->flags79 &= ~ACTOR_MOTION_DONE;
    }
    obj->state = pair_switch_wait;
}

s32 rand_below(s32 range, u32 seed);
u32 mt19937_next(void);
extern u8 *gWorkBuf;
struct SpriteNode *sprite_node_release(struct SpriteNode *node);

void sprite_nodes_fall(struct Actor *obj)
{
  s32 z;
  struct SpriteNode *link = obj->slots->head;
  struct SpriteNode *node;
  next:
  node = link->next;

  if (node == 0)
  {
    return;
  }
  node->x += node->vx;
  node->y += node->vy;
  z = (node->z += (s16) node->vz);
  node->vz -= 0x66;
  if (((z - 1) + 1) > 0)
  {
    link = node;
    goto next;
  }
  node->z = 0;
  node->vx /= 2;
  node->vy /= 2;
  node->vz = (-((s16) node->vz)) / 2;
  if (((s16) node->vz) <= 0xFF)
  {
    node->z = 0;
    link->next = sprite_node_release(node);
  }
  goto next;
}

struct SpriteNode *sprite_node_alloc_default(void);

/*
 * Drops one coin: if the party has any coins left (the u16 at gParty + 0x88), takes one and throws
 * a coin sprite out from 6 px right of obj's target point, at its height offset minus 0x16: x speed
 * 0x180-0x2FF, y speed -0x4C..0x4C, rising at 0x280-0x3FF (sprite_nodes_fall makes it fall and
 * bounce). The coin joins the list of obj's child, and the coin sound (0x2C) plays.
 */
void spawn_fall_particle(struct Actor *obj)
{
    s32 speedBase = 0x180;
    u16 *coins = (u16 *)(gParty + 0x88);

    if (*coins != 0) {
        struct SpriteNode *coin;
        struct SpriteNode *head;

        (*coins)--;
        coin = sprite_node_alloc_default();
        coin->x = obj->targetX + 0x600;
        coin->y = obj->targetY;
        coin->z = obj->targetZ + ((s8)obj->offZ - 0x16) * 0x100;
        coin->vx = rand_below(speedBase, mt19937_next());
        coin->vx += speedBase;
        coin->vy = 0x4C - rand_below(0x99, mt19937_next());
        coin->vz = rand_below(speedBase, mt19937_next());
        coin->vz += 0x280;
        head = obj->child->slots->head;
        coin->next = head->next;
        head->next = coin;
        set_slot_value(0x2C, -1);
    }
}

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

void set_follow_handle(struct Actor *obj);

NONMATCH("asm/code_08097238/pair_sum_offsets.s", void pair_sum_offsets(struct Actor *obj))
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;
    s32 count = 0;
    s32 sumX = 0;
    s32 sumW = 0;
    s32 sumR = 0;
    s32 i;
    s32 rate;
    s32 timer;
    s32 timer2;
    u8 *ctx;

    for (i = 0; i < 6; i++) {
        struct Partner *p = gWorld->partners[i];

        if ((p->flags7E & 6) == 4) {
            sumX += p->unkF5;
            sumW += p->unk108;
            sumR += p->unk106;
            count++;
        }
    }
    rate = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sumX, count);
    heroB->rate = rate;
    heroA->rate = rate;
    timer = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sumW, count);
    heroB->unk1E = timer;
    heroA->unk1E = timer;
    timer2 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sumR, count);
    heroB->timer = timer2;
    heroA->timer = timer2;
    ctx = gWorkBuf;
    *(s32 *)(ctx + 0x5470) = 0;
    ctx = gWorkBuf;
    *(s32 *)(ctx + 0x5474) = -0x14;
    *(s32 *)(ctx + 0x5478) = 0xB3;
    *(s32 *)(ctx + 0x547C) = obj->data->unkF5 - obj->rate;
    if (obj == heroA && (heroB->flags76 & 6) == 2) {
        heroB->state = set_follow_handle;
    }
    if (obj == heroB && (heroA->flags76 & 6) == 2) {
        heroA->state = set_follow_handle;
    }
    obj->state = follow_focus_settle;
}
END_NONMATCH

void place_targets(struct Actor *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct Actor *obj, u32 value);
void end_hero_follow(struct Actor *obj);
/* Ends a hero's follow move: sets hero A's (0x2027) or hero B's (0x2056) action and pose mode 1; when
   the hero is in mode 2 or 4, it jumps to its target (whole pixels plus its offsets, 10 to the right
   for the World.unk7C hero and 14 for the other) with an arc 14 pixels high, at speed 0x200. Then
   waits in end_hero_follow. */
void follow_focus_settle(struct Actor *obj)
{
    struct Actor *heroA = &gWorld->heroA->body;
    struct Actor *heroB = &gWorld->heroB->body;
    struct Actor *hero7C;
    s32 mode;

    if (obj == heroA) {
        sprite_set_action(obj, 1, 0x2027, 0);
    }
    if (obj == heroB) {
        sprite_set_action(obj, 1, 0x2056, 0);
    }
    obj->pose->mode = 1;
    hero7C = &gWorld->unk7C->body;
    if (obj == hero7C) {
        mode = hero7C->flags76 & 6;
        if (mode == 2 || mode == 4) {
            s32 x = obj->targetX / 0x100 + (s8)hero7C->offX + 10;
            s32 y = obj->targetY / 0x100 + (s8)hero7C->offY;
            s16 height;

            place_targets(obj, x, y, 0, -1);
            height = obj->targetZ / 0x100 + 14;
            hero7C->arcHeight = height;
            set_motion_offsets(obj, 0x200);
        }
    } else {
        mode = obj->flags76 & 6;
        if (mode == 2 || mode == 4) {
            s32 x = obj->targetX / 0x100 + (s8)obj->offX + 14;
            s32 y = obj->targetY / 0x100 + (s8)obj->offY;
            s16 height;

            place_targets(obj, x, y, 0, -1);
            height = obj->targetZ / 0x100 + 14;
            obj->arcHeight = height;
            set_motion_offsets(obj, 0x200);
        }
    }
    obj->state = end_hero_follow;
}
