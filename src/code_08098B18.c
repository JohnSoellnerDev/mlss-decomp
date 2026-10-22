#include "global.h"
#include "ram_routines.h"

struct TileNode
{
    struct TileNode *next;
    s32 x;
    s32 y;
    s32 z;
};

struct TileList
{
    u8 filler0[0x0C];
    struct TileNode first;
};

struct HeroSprite
{
    s16 unk0;
    s16 unk2;
    u8 filler4[0x12 - 0x04];
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 unk21;
    u8 filler22[0x30 - 0x22];
    struct TileList *tiles;
};

struct HeroBody;
struct Hero;

struct HeroData
{
    struct Hero *first;
    u8 filler4[0xD8 - 0x04];
    s32 unkD8;
    s32 unkDC;
    u8 filler0E0[0x106 - 0xE0];
    s16 unk106;
    s16 unk108;
    u8 filler10A[0x110 - 0x10A];
    u8 unk110;
};

struct ChildNode
{
    struct ChildNode *next;
};

struct ChildList
{
    u8 filler0[0x04];
    struct HeroSprite *sprite;
    struct ChildNode *head;
};

struct HeroBody
{
    u8 filler0[0x04];
    struct ChildList *list;
    struct HeroSprite *sprite;
    struct ChildNode *nodes;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x1E - 0x1C];
    s16 level;
    s16 unk20;
    u8 filler22[0x28 - 0x22];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u8 filler50[0x5C - 0x50];
    void (*unk5C)(struct HeroBody *);
    u32 unk60;
    u8 filler64[0x6C - 0x64];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 flags77;
    u8 unk78;
    u8 flags;
    u16 handle;
    u8 filler7C[0x7E - 0x7C];
    u16 unk7E;
    u8 filler80[0x84 - 0x80];
    s32 centerX;
    s32 centerY;
    u8 filler8C[0x9C - 0x8C];
    s32 phase;
    s32 clock;
    s32 repeats;
    u8 filler0A8[0xAC - 0xA8];
    s16 step;
    u16 timerE;
};

struct HeroFlag0Bits
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroFlags77Bits
{
    u8 unk0 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 unk7 : 1;
};

struct Hero
{
    struct Hero *next;
    u8 filler4[0x04];
    struct HeroBody body;
};

struct Part
{
    u8 filler0[0x14];
    u16 flags;
};

struct Entry
{
    u8 filler0[0x10];
    struct Part *part;
};

struct World
{
    u8 filler0[0x0C];
    u8 flags;
    u8 filler0D[0x58 - 0x0D];
    struct Entry *entries[6];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *focus;
};

struct LayerCtx
{
    u8 filler0[0x0C];
    u16 cells[16];
};

extern struct World *gWorld;
extern u8 gSys[];
extern struct LayerCtx *gModelCtx;
extern u8 *gWorkBuf;
extern u8 gUnk_02000460[];

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

static inline void *focus_obj(void)
{
    return gWorld->focus;
}

void sprite_set_action(struct HeroBody *body, u32 action, u32 arg, u32 extra);
void on_arrive(struct HeroBody *body);
void obj_release(void *obj);
void step_motion(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void set_slot_value(u32 id, s32 value);
struct HeroBody *obj_spawn(void *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void sprite_node_take_link(struct HeroBody *obj);
void sprite_nodes_fall(struct HeroBody *body);
s32 step_vector(u16 *outX, u16 *outY, s32 dx, s32 dy, s32 speed);
void obj_free_parts(struct HeroBody *body);
void obj_list_remove(void *obj);
void sound_stop_song(u32 song);
void refresh_slot_flags(void);
void place_layer_tile(s32 x, s32 y, s32 z, s32 flag, u32 tile);

void reset_follow_state(struct HeroBody *body);
void follow_focus_end(struct HeroBody *body);
void drop_child_when_empty(struct HeroBody *body);
struct ChildList *sprite_node_take(void);
struct ChildNode *sprite_node_alloc(s32 x, s32 y, s32 z);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void spawn_fall_particle(struct HeroBody *body);
void follow_focus_wait(struct HeroBody *body);
void start_parts_after_16(struct HeroBody *body);
void orbit_nodes_a(struct HeroBody *body);
void drop_to_focus(struct HeroBody *body);
void drop_to_focus_b(struct HeroBody *body);
void orbit_nodes_b(struct HeroBody *body);
void orbit_nodes_c(struct HeroBody *body);
struct TileNode *sprite_node_release(struct TileNode *node);
void partner_link_update(struct HeroBody *body, struct HeroBody *other, u32 mode);
struct Hero *active_candidate(struct HeroData *data);
void pose_update_if_active_a(struct HeroBody *body);
void pose_update_if_active_b(struct HeroBody *body);
void pose_update_if_active_c(struct HeroBody *body);
void start_parts_after_32(struct HeroBody *body);
void follow_spawn_child(struct HeroBody *body);
void follow_focus_x(struct HeroBody *body);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void release_other_hero(struct HeroBody *body);
void prepare_follow_focus(struct HeroBody *body);
void follow_focus_settle(struct HeroBody *body);
void set_anim_12(struct HeroBody *body);
void anim_13_fx_then_spawn(struct HeroBody *body);
void set_anim_0f(struct HeroBody *body);
void anim_10_fx_then_child(struct HeroBody *body);
void set_anim_0c(struct HeroBody *body);
void anim_0d_fx_then_spawn_c(struct HeroBody *body);
void focus_follow_spawn_orbit_child_a(struct HeroBody *body);
void focus_follow_spawn_orbit_child_b(struct HeroBody *body);
void focus_follow_spawn_orbit_child_c(struct HeroBody *body);

void follow_spawn_child(struct HeroBody *body)
{
    struct HeroBody *hero_b = &gWorld->heroB->body;
    struct HeroBody *child;
    s32 mode;

    if (body->sprite->flags & 8)
    {
        child = obj_spawn(hero_b->data, 0x2089, 0, -0x40, -1, 0, 1);
        sprite_set_action(child, 0, 0, 0);
        child->unk78 &= 0x1F;
        body->child = child;
        sprite_node_take_link(child);
        mode = child->mode & 6;
        if (mode == 2 || mode == 4)
        {
            child->state = sprite_nodes_fall;
        }
        reset_follow_state(body);
    }
}

void follow_focus_x(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *hero_a = &world->heroA->body;
    struct HeroBody *hero_b = &world->heroB->body;
    s32 roll;
    s32 step;
    s32 target;

    {
        s32 wflags = world->flags;

        world->flags = wflags | 0x10;
    }
    if (body == hero_a)
        set_body_part(body, 1, 0, -0x24, 1);
    else
        set_body_part(body, 2, 0, -0x24, 1);

    roll = rand_below(4, mt19937_next()) + 0x10;
    if ((u32)roll < (u32)body->clock)
    {
        body->clock = 0;
        spawn_fall_particle(body);
    }
    body->clock++;

    if ((body == hero_a && (((struct SysKeys3E *)gSys)->keys & 1) != 0)
        || (body == hero_b && (((struct SysKeys3E *)gSys)->keys & 2) != 0))
    {
        body->phase -= 0x400;
        if (body->sprite->unk21 != 2)
            sprite_set_action(body, 2, 0, 0);
    }
    if (body->sprite->flags & 8)
        sprite_set_action(body, 1, 0, 0);
    body->phase -= 0x19;

    step = (s32)(((double)(body->data->unk106 / ((body->unk20 * 0x99) / 0x100)) + 1.2) * 256.0);
    body->posX -= step;
    if (body->posX < body->phase)
        body->posX = body->phase;

    target = (s16)obj_last_x(focus_obj()) * 0x100 + 0xC00;
    if (target >= body->posX)
    {
        body->posX = target;
        obj_free_parts(body);
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            s32 y0 = body->sprite->unk2;
            s32 x = (s16)obj_last_x(focus_obj()) - 0x30;
            s32 y = (s16)obj_last_y(focus_obj()) + y0;

            place_targets(body, x, y, 0, -1);
            set_motion_start(body, 0x200);
        }
        body->state = release_other_hero;
    }
}

void prepare_follow_focus(struct HeroBody *body)
{
    struct FollowFlagBits
    {
        u8 unk0 : 5;
        u8 bit5 : 1;
        u8 unk6 : 2;
    };

    obj_free_parts(body);
    if (body == &gWorld->heroA->body)
    {
        sprite_set_action(body, 3, 0, 0);
    }
    else
    {
        sprite_set_action(body, 3, 0, 0);
    }
    ((struct HeroSpriteBits *)&body->sprite->flags)->mode = 1;
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        s32 value = body->data->unk108 + 0x64;

        body->handle = (value != body->level)
            ? MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, value - body->level)
            : 0x4000;
        body->unk7E = 0;
        ((struct FollowFlagBits *)&body->flags)->bit5 = 0;
    }
    body->clock = 0;
    body->repeats = rand_below(3, mt19937_next()) + 2;
    body->state = follow_focus_wait;
}

static inline void set_hero_anim(struct HeroBody *body, u32 anim)
{
    sprite_set_action(body, 1, anim, 0);
}

void follow_focus_wait(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *hero_b = &gWorld->heroB->body;
    s32 clock;

    state_stack_advance_progress(body);
    if (body->repeats != 0)
    {
        clock = body->clock - 1;
        body->clock = clock;
        if (clock <= 0)
        {
            body->clock = rand_below(4, mt19937_next()) + 4;
            body->repeats--;
            spawn_fall_particle(body);
        }
    }
    if (body->flags & 0x20)
    {
        if (body == hero_a)
        {
            if (hero_b->state == follow_focus_x)
            {
                hero_b->state = prepare_follow_focus;
            }
        }
        if (body == hero_b)
        {
            if (hero_a->state == follow_focus_x)
            {
                hero_a->state = prepare_follow_focus;
            }
        }
        {
            struct World *world = gWorld;
            struct HeroBody *hero_a = &world->heroA->body;
            struct HeroBody *hero_b = &world->heroB->body;

            if (body == hero_a)
            {
                set_hero_anim(body, 0x2027);
            }
            if (body == hero_b)
            {
                set_hero_anim(body, 0x2056);
            }
        }
        ((struct HeroSpriteBits *)&body->sprite->flags)->mode = 0;
        body->phase = body->targetX;
        body->clock = 0;
        body->handle = 0;
        body->state = follow_focus_x;
    }
}

void set_follow_handle(struct HeroBody *body)
{
    struct FollowFlagBits
    {
        u8 unk0 : 5;
        u8 bit5 : 1;
        u8 unk6 : 2;
    };

    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        s32 value = body->data->unk108 + 0x64;

        body->handle = (value != body->level)
            ? MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, value - body->level)
            : 0x4000;
        body->unk7E = 0;
        ((struct FollowFlagBits *)&body->flags)->bit5 = 0;
    }
    body->state = follow_focus_end;
}

void follow_focus_end(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *hero_b = &gWorld->heroB->body;

    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        if (body == hero_a)
        {
            if (hero_b->state == follow_focus_x)
            {
                hero_b->state = prepare_follow_focus;
            }
        }
        if (body == hero_b)
        {
            if (hero_a->state == follow_focus_x)
            {
                hero_a->state = prepare_follow_focus;
            }
        }
        body->state = follow_focus_settle;
    }
}

void reset_follow_state(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *hero_b = &gWorld->heroB->body;

    if (body == hero_a)
    {
        sprite_set_action(body, 1, 0x2027, 0);
    }
    if (body == hero_b)
    {
        sprite_set_action(body, 1, 0x2056, 0);
    }
    ((struct HeroSpriteBits *)&body->sprite->flags)->mode = 0;
    body->phase = body->targetX;
    body->clock = 0;
    body->handle = 0;
    body->state = follow_focus_x;
}

void release_other_hero(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *hero_b = &gWorld->heroB->body;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body == hero_a)
        {
            if (hero_b->state != 0)
            {
                if (hero_b->child != 0)
                {
                    reset_follow_state(hero_b);
                }
                else
                {
                    follow_focus_settle(hero_b);
                }
            }
        }
        if (body == hero_b)
        {
            if (hero_a->state != 0)
            {
                if (hero_a->child != 0)
                {
                    reset_follow_state(hero_a);
                }
                else
                {
                    follow_focus_settle(hero_a);
                }
            }
        }
        body->state = drop_child_when_empty;
    }
}

void end_hero_follow(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *hero_b = &gWorld->heroB->body;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body == hero_a)
        {
            sprite_set_action(body, 0, 0, 0);
        }
        if (body == hero_b)
        {
            sprite_set_action(body, 0, 0, 0);
        }
        body->state = follow_spawn_child;
    }
}

void drop_child_when_empty(struct HeroBody *body)
{
    struct HeroBody *child = body->child;
    struct ChildNode *head = child->list->head;
    struct ChildNode *next = head->next;

    if (next == 0)
    {
        obj_release(child);
        body->child = (struct HeroBody *)next;
        body->state = (void (*)(struct HeroBody *))next;
    }
}

static inline void push_sprite_node(struct HeroBody *child)
{
    struct ChildNode *node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);

    node->next = child->nodes;
    child->nodes = node;
}

void focus_follow_spawn_orbit_child_a(struct HeroBody *body)
{
    struct HeroBody *child;

    if (body->sprite->flags & 8)
    {
        set_slot_value(0x8B, -1);
        set_slot_value(0x8C, -1);
        sprite_set_action(body, 0x14, 0, 0);
        effect_spawn_at(0x28DF, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = obj_spawn(body->data, 0x403D, body->targetX / 0x100 - 0x32, body->targetY / 0x100, body->targetZ / 0x100, 1, 1);
        body->child = child;
        ((struct HeroFlag0Bits *)&child->flags)->bit0 = 1;
        child->phase = 0;
        child->list = sprite_node_take();
        child->list->sprite = child->sprite;
        child->list->head = (struct ChildNode *)&child->nodes;
        child->anim = body->anim;
        child->partner = body->partner;
        child->centerX = child->posX;
        child->centerY = child->posY;
        child->timerE = 0;
        push_sprite_node(child);
        push_sprite_node(child);
        push_sprite_node(child);
        push_sprite_node(child);
        child->state = orbit_nodes_a;
        body->state = pose_update_if_active_a;
    }
}

ASM_FUNC("asm/code_08098B18/orbit_nodes_a.s", void orbit_nodes_a(struct HeroBody *body))

void pose_update_if_active_a(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        on_arrive(body);
    }
}

u32 enter_set_anim_12(struct HeroBody *body)
{
    body->state = set_anim_12;
    return 1;
}

void set_anim_12(struct HeroBody *body)
{
    sprite_set_action(body, 0x12, 0, 0);
    body->state = anim_13_fx_then_spawn;
}

void anim_13_fx_then_spawn(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0x13, 0, 0);
        effect_spawn_at(0x2841, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        set_slot_value(0x10C, -1);
        body->state = focus_follow_spawn_orbit_child_a;
    }
}

void focus_follow_spawn_orbit_child_b(struct HeroBody *body)
{
    struct HeroBody *child;
    s32 i;

    if (body->sprite->flags & 8)
    {
        set_slot_value(0x8B, -1);
        set_slot_value(0x8C, -1);
        sprite_set_action(body, 0x11, 0, 0);
        effect_spawn_at(0x28C6, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = obj_spawn(body->data, 0x403C, body->targetX / 0x100 - 0x32, body->targetY / 0x100, body->targetZ / 0x100, 1, 1);
        body->child = child;
        child->phase = 0;
        child->list = sprite_node_take();
        child->list->sprite = child->sprite;
        child->list->head = (struct ChildNode *)&child->nodes;
        child->anim = body->anim;
        child->partner = body->partner;
        child->centerX = child->posX;
        child->centerY = child->posY;
        child->timerE = 0;
        for (i = 0; i < 8; i++)
            push_sprite_node(child);
        child->state = orbit_nodes_b;
        body->state = pose_update_if_active_b;
    }
}

struct TileNode *sprite_node_release(struct TileNode *node);
void partner_link_update(struct HeroBody *body, struct HeroBody *other, u32 mode);
struct Hero *active_candidate(struct HeroData *data);
ASM_FUNC("asm/code_08098B18/orbit_nodes_b.s", void orbit_nodes_b(struct HeroBody *body))

void pose_update_if_active_b(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        on_arrive(body);
    }
}

u32 enter_set_anim_0f(struct HeroBody *body)
{
    body->state = set_anim_0f;
    return 1;
}

void set_anim_0f(struct HeroBody *body)
{
    sprite_set_action(body, 0xF, 0, 0);
    body->state = anim_10_fx_then_child;
}

void anim_10_fx_then_child(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0x10, 0, 0);
        effect_spawn_at(0x2829, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        set_slot_value(0x10C, -1);
        body->state = focus_follow_spawn_orbit_child_b;
    }
}

void focus_follow_spawn_orbit_child_c(struct HeroBody *body)
{
    struct HeroBody *child;
    s32 i;

    if (body->sprite->flags & 8)
    {
        set_slot_value(0x8B, -1);
        set_slot_value(0x8C, -1);
        sprite_set_action(body, 0xE, 0, 0);
        effect_spawn_at(0x28AD, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = obj_spawn(body->data, 0x403C, body->targetX / 0x100 - 0x32, body->targetY / 0x100, body->targetZ / 0x100, 1, 1);
        body->child = child;
        child->phase = 0;
        child->list = sprite_node_take();
        child->list->sprite = child->sprite;
        child->list->head = (struct ChildNode *)&child->nodes;
        child->anim = body->anim;
        child->partner = body->partner;
        child->centerX = child->posX;
        child->centerY = child->posY;
        child->timerE = 0;
        for (i = 0; i < 8; i++)
            push_sprite_node(child);
        child->state = orbit_nodes_c;
        body->state = pose_update_if_active_c;
    }
}

ASM_FUNC("asm/code_08098B18/orbit_nodes_c.s", void orbit_nodes_c(struct HeroBody *body))

void pose_update_if_active_c(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        on_arrive(body);
    }
}

u32 enter_set_anim_0c(struct HeroBody *body)
{
    body->state = set_anim_0c;
    return 1;
}

void set_anim_0c(struct HeroBody *body)
{
    sprite_set_action(body, 0xC, 0, 0);
    body->state = anim_0d_fx_then_spawn_c;
}

void anim_0d_fx_then_spawn_c(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0xD, 0, 0);
        effect_spawn_at(0x2811, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        set_slot_value(0x10C, -1);
        body->state = focus_follow_spawn_orbit_child_c;
    }
}

void focus_follow_step_toward_partner(struct HeroBody *body)
{
    u16 *step = (u16 *)&body->step;
    s16 out[2];
    s16 *outY;
    struct HeroBody *partner;
    s32 startX;
    s32 dist;
    s32 dx;
    s32 dy;

    *step += 1;
    outY = &out[1];
    partner = body->partner;
    startX = body->centerX - 0x5000;
    dx = partner->targetX - startX;
    dy = partner->targetY - body->centerY;
    dist = step_vector((u16 *)&out[0], (u16 *)outY, dx, dy, (s16)*step << 7);
    body->posX = body->centerX + (out[0] << 2);
    body->posY = body->centerY + (*outY << 2);
    if (((s16)*step << 7) >= dist)
    {
        set_slot_value(0xD5, -1);
        sprite_set_action(body, 3, 0, 0);
        *step = 0;
        partner = body->partner;
        body->posX = partner->targetX + 0x5000;
        body->posY = partner->targetY;
        body->state = start_parts_after_32;
    }
}

ASM_FUNC("asm/code_08098B18/start_parts_after_32.s", void start_parts_after_32(struct HeroBody *body))

s32 obj_last_x(void *obj);
void drop_to_focus(struct HeroBody *body)
{
    struct Hero *node;
    struct HeroBody *part;
    u16 *counter;
    s32 limit;
    s32 mode;
    s32 clear;
    s32 zero;

    body->posX -= 0x2CC;
    counter = (u16 *)&body->step;
    *counter += 1;
    if ((s16)*counter > 5)
    {
        *counter = 0;
        effect_spawn_at(0xD1D, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
    }
    if (body->sprite->unk0 < -0x30)
    {
        body->posX = ((s32)(s16)obj_last_x(gWorld->focus) << 8) + 0x12000;
        body->posY = body->data->unkDC;
        node = body->data->first;
        zero = 0;
        clear = ~0x40;
        for (;;)
        {
            part = &node->body;
            obj_list_remove(part);
            mode = part->mode;
            mode &= clear;
            part->mode = mode;
            part->partner = (struct HeroBody *)zero;
            part->anim = (void *)zero;
            if ((void *)node == (void *)body->data)
            {
                break;
            }
            node = node->next;
        }
    }
    if (body->targetX >= body->data->unkD8)
    {
        limit = body->data->unkD8;
        if (limit >= body->posX)
        {
            body->posX = limit;
            sprite_set_action(body, -1, 0, 0);
            sound_stop_song(0x11A);
            body->state = 0;
        }
    }
}

extern u8 gSys[];
ASM_FUNC("asm/code_08098B18/unknown_code_08099f58.s", void unknown_code_08099f58(s32 a, s32 b))

void approach_partner(struct HeroBody *body)
{
    u16 *counter = (u16 *)&body->step;
    s16 out[2];
    s16 *outY;
    struct HeroBody *partner;
    s32 startX;
    s32 limit;
    s32 dx;
    s32 dy;

    *counter += 1;
    outY = &out[1];
    partner = body->partner;
    startX = body->centerX - 0x5000;
    dx = partner->targetX - startX;
    dy = partner->targetY - body->centerY;
    limit = step_vector((u16 *)&out[0], (u16 *)outY, dx, dy, (s16)*counter << 7);
    body->posX = body->centerX + (out[0] << 2);
    body->posY = body->centerY + (*outY << 2);
    if (((s16)*counter << 7) >= limit)
    {
        set_slot_value(0xD5, -1);
        sprite_set_action(body, 3, 0, 0);
        *counter = 0;
        partner = body->partner;
        body->posX = partner->targetX + 0x5000;
        body->posY = partner->targetY;
        body->state = start_parts_after_16;
    }
}

ASM_FUNC("asm/code_08098B18/start_parts_after_16.s", void start_parts_after_16(struct HeroBody *body))

void drop_to_focus_b(struct HeroBody *body)
{
    struct Hero *node;
    struct HeroBody *part;
    u16 *counter;
    s32 limit;
    s32 mode;
    s32 clear;
    s32 zero;

    body->posX -= 0x2CC;
    counter = (u16 *)&body->step;
    *counter += 1;
    if ((s16)*counter > 5)
    {
        *counter = 0;
        effect_spawn_at(0xD1D, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
    }
    if (body->sprite->unk0 < -0x30)
    {
        body->posX = ((s32)(s16)obj_last_x(gWorld->focus) << 8) + 0x12000;
        body->posY = body->data->unkDC;
        node = body->data->first;
        zero = 0;
        clear = ~0x40;
        for (;;)
        {
            part = &node->body;
            obj_list_remove(part);
            mode = part->mode;
            mode &= clear;
            part->mode = mode;
            part->partner = (struct HeroBody *)zero;
            part->anim = (void *)zero;
            if ((void *)node == (void *)body->data)
            {
                break;
            }
            node = node->next;
        }
    }
    if (body->targetX >= body->data->unkD8)
    {
        limit = body->data->unkD8;
        if (limit >= body->posX)
        {
            body->posX = limit;
            sprite_set_action(body, -1, 0, 0);
            sound_stop_song(0x11A);
            body->state = 0;
        }
    }
}

NONMATCH("asm/code_08098B18/update_layer_tiles.s", void update_layer_tiles(struct HeroBody *body))
{
    struct HeroSprite *sprite = body->sprite;
    struct TileNode *tile;
    u32 dest;
    s32 i;

    if (sprite->tiles != 0)
    {
        refresh_slot_flags();
        dest = 0x200;
        tile = &sprite->tiles->first;
        while (tile != 0)
        {
            place_layer_tile(tile->x / 0x100, tile->y / 0x100, tile->z / 0x100, 0, dest);
            dest += 2;
            tile = tile->next;
        }
    }
    else
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gWorkBuf + 0x3CD0, gUnk_02000460, 0x20);
        *(u16 *)gSys |= 0x8000;
        gModelCtx->cells[15] = gModelCtx->cells[14];
        gModelCtx->cells[14] = gModelCtx->cells[13];
        for (i = 0; i < 15; i++)
        {
            if (gModelCtx->cells[i] == 0x1FFF)
            {
                gModelCtx->cells[i] = 0xFFFF;
            }
        }
        body->list = 0;
    }
}
END_NONMATCH
