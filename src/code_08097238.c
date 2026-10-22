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
    child->unk92 = offZ;
    set_motion_offsets(child, 0x480);
}

void pair_children_end(struct Actor *obj)
{
    struct Actor *child;

    state_stack_advance_progress(obj);
    if (obj->flags79 & 0x20) {
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
    inWindow = obj->step <= q && (s16)obj->phase >= q;
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

static inline void slot_action_one(struct Actor *obj, u32 action)
{
    sprite_set_action(obj, 1, action, 0);
}

static inline void slot_set_minus_one(u32 id)
{
    set_slot_value(id, -1);
}

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
    if (obj->flags79 & 0x20) {
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
                slot_action_one(obj, 0x2027);
            }
            if (obj == curB) {
                slot_action_one(obj, 0x2056);
            }
        }
        obj->pose->mode = 0;
        obj->time = obj->targetX;
        obj->clock = 0;
        obj->handle = 0;
        obj->state = hero_random_walk;
        slot_set_minus_one(0x3F);
    }
}

void start_random_handle(struct Actor *obj)
{
    u8 bits = obj->flags76 & 6;

    if (bits == 2 || bits == 4) {
        s32 n = rand_below(3, mt19937_next());

        obj->handle = *(s32 *)(gWorkBuf + 0x5480) + n * *(s32 *)(gWorkBuf + 0x5484) != 0
            ? MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, *(s32 *)(gWorkBuf + 0x5480) + rand_below(3, mt19937_next()) * *(s32 *)(gWorkBuf + 0x5484))
            : 0x4000;
        obj->phase = 0;
        *((s8 *)obj + 0x79) &= -0x21;
    }
    obj->state = pair_switch_wait;
}

s32 rand_below(s32 range, u32 seed);
u32 mt19937_next(void);
extern u8 *gWorkBuf;
struct SpriteNode *sprite_node_release(struct SpriteNode *node);

NONMATCH("asm/code_08097238/sprite_nodes_fall.s", void sprite_nodes_fall(struct Actor *obj))
{
    struct SpriteNode *link = obj->slots->head;
    struct SpriteNode *node;

    for (node = link->next; node != 0; node = link->next) {
        s32 z;

        node->x += node->vx;
        node->y += node->vy;
        z = node->z += (s16)node->vz;
        node->vz -= 0x66;
        if (z > 0) {
            link = node;
        } else {
            node->z = 0;
            node->vx /= 2;
            node->vy /= 2;
            node->vz = -(s16)node->vz / 2;
            if ((s16)node->vz <= 0xFF) {
                link->next = sprite_node_release(node);
            }
        }
    }
}
END_NONMATCH

struct SpriteNode *sprite_node_alloc_default(void);

void spawn_fall_particle(struct Actor *obj)
{
    s32 speedBase = 0x180;
    u16 *counter = (u16 *)(gParty + 0x88);

    if (*counter != 0) {
        struct SpriteNode *node;
        struct SpriteNode *sentinel;

        (*counter)--;
        node = sprite_node_alloc_default();
        node->x = obj->targetX + 0x600;
        node->y = obj->targetY;
        node->z = obj->targetZ + ((s8)obj->offZ - 0x16) * 0x100;
        node->vx = rand_below(speedBase, mt19937_next());
        node->vx += speedBase;
        node->vy = 0x4C - rand_below(0x99, mt19937_next());
        node->vz = rand_below(speedBase, mt19937_next());
        node->vz += 0x280;
        sentinel = obj->child->slots->head;
        node->next = sentinel->next;
        sentinel->next = node;
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
