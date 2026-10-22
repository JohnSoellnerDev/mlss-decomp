#include "global.h"
#include "ram_routines.h"

struct ActorSprite
{
    s16 unk0;
    u8 filler2[0x12 - 0x2];
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 kind;
};

struct SpriteFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ActorBody;

struct SpriteNode
{
    struct SpriteNode *next;
    struct ActorSprite *sprite;
    struct SpriteNode **link;
};

struct ActorFlagBits
{
    u8 low : 5;
    u8 active : 1;
    u8 high : 2;
};

struct ActorFlagByte
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ActorData
{
    u8 filler0[0x38];
    struct ActorBody *members;
    u8 filler3C[0xF6 - 0x3C];
    s16 effect;
};

struct ActorBody
{
    u8 filler0[0x04];
    struct SpriteNode *node;
    struct ActorSprite *sprite;
    struct SpriteNode *parts;
    s32 x;
    s32 y;
    s32 z;
    s16 timerA;
    s16 timerB;
    s16 timerC;
    u8 filler22[0x28 - 0x22];
    struct ActorData *data;
    struct ActorBody *partner;
    struct ActorBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x45 - 0x44];
    s8 unk45;
    u8 filler46[0x4C - 0x46];
    void (*state)(struct ActorBody *body);
    u32 unk50;
    void (*unk54)(struct ActorBody *body);
    u32 unk58;
    void (*unk5C)(struct ActorBody *body);
    void (*unk60)(struct ActorBody *body);
    u8 filler64[0x6C - 0x64];
    s32 unk6C;
    u8 filler70[0x75 - 0x70];
    u8 index;
    u8 mode;
    u8 filler77;
    u8 unk78;
    u8 flags;
    u16 handle;
    u16 unk7C;
    u16 handleB;
    u8 filler80[0x84 - 0x80];
    s32 baseX;
    s32 baseY;
    u8 filler8C[0x9C - 0x8C];
    u32 timeLink;
    u32 unkA0;
    u32 unkA4;
    s32 link;
    u8 fillerAC[0xAE - 0xAC];
    u16 vspeed;
};

struct Hero
{
    u8 filler0[0x08];
    struct ActorBody body;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *focus;
};

extern struct World *gWorld;

static inline void *world_focus(struct World *world)
{
    return world->focus;
}

void obj_release(void *obj);
void release_object(void *obj);
void task_list_remove(void *obj);
void sprite_set_action(struct ActorBody *body, u32 action, u32 arg, u32 extra);
void step_motion_d(struct ActorBody *body);
void apply_pose_to_parts(struct ActorBody *body);
void set_slot_value(u32 id, s32 value);
void state_stack_advance_progress(struct ActorBody *body);
void partner_link_update(struct ActorBody *body, struct ActorBody *partner, u32 mask);
void place_targets(struct ActorBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct ActorBody *body, u32 speed);
void start_anim_by_hero(struct ActorBody *body);
void fx_anim8(struct ActorBody *body);
void set_anim_0b_mode1(struct ActorBody *body);
void anim_0c_fx_at_pos(struct ActorBody *body);
void start_child_chain(struct ActorBody *body);
void spawn_five_children(struct ActorBody *body);
void anim_stop_if_sprite_active(struct ActorBody *body);
void draw_if_visible_b(struct ActorBody *body);
void draw_if_visible_c(struct ActorBody *body);
void draw_if_visible_d(struct ActorBody *body);
void fx_at_partner_a(struct ActorBody *body);
void fx_at_partner_b(struct ActorBody *body);
void fx_at_partner_link(struct ActorBody *body);
void float_above_focus(struct ActorBody *body);
void start_anim10(struct ActorBody *body);
void start_pose_4(struct ActorBody *body);
void start_pose_2(struct ActorBody *body);
void stop_after_index_bump(struct ActorBody *body);
void place_beside_partner(struct ActorBody *body);
void rise_anim1(struct ActorBody *body);
void link_anim3_a(struct ActorBody *body);
void link_anim3_b(struct ActorBody *body);
void start_anim1_b(struct ActorBody *body);
void start_anim9(struct ActorBody *body);
void start_wander_timers(struct ActorBody *body);
void attach_to_heroes(struct ActorBody *body);
void choose_hero_target(struct ActorBody *body);
void start_anim_by_partner(struct ActorBody *body);
struct ActorBody *obj_spawn_linked(struct ActorBody *body, u32 art, struct ActorBody *partner);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct ActorBody *owner);
void step_motion(struct ActorBody *body);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void follow_focus_while_effect(struct ActorBody *body);
void place_on_focus_a(struct ActorBody *body);
void place_on_focus_b(struct ActorBody *body);
void place_on_focus_c(struct ActorBody *body);
void release_fx_a(struct ActorBody *body);
void release_fx_b(struct ActorBody *body);
void remove_when_expired(struct ActorBody *body);
void start_anim3_c(struct ActorBody *body);
void fx_only_a(struct ActorBody *body);
void fx_only_b(struct ActorBody *body);
void fx_only_c(struct ActorBody *body);
void check_offscreen_b(struct ActorBody *body);
void check_offscreen(struct ActorBody *body);
void obj_list_push_arg(struct ActorBody *obj, u32 arg);
void obj_detach_active(struct ActorSprite *sprite);
void wander_step(struct ActorBody *body);
void set_focus_pos_a(struct ActorBody *body);
struct ActorBody *obj_spawn(struct ActorData *data, u32 art, s32 x, s32 y, s32 z, u32 a, u32 b);
void *sprite_node_take(void);
void *sprite_node_alloc(s32 x, s32 y, s32 h);
void orbit_parts_falling(struct ActorBody *body);
void pose_update_b(struct ActorBody *body);
void set_focus_pos_b(struct ActorBody *body);
void set_focus_pos_c(struct ActorBody *body);
void free_data_done(struct ActorBody *body);
void rise_stop_b(struct ActorBody *body);
void wait_child_idle(struct ActorBody *body);
void step_motion_scaled(struct ActorBody *body);
void enter_start_pose_4(struct ActorBody *body);
void clear_state_if_sprite_active(struct ActorBody *body);
extern const s8 gChildChainOffsetTable[];
u32 mt19937_next(void);
void approach_contact(struct ActorBody *body);
void approach_slot(struct ActorBody *body);
void obj_list_push_default(struct ActorBody *obj);
void set_motion_speed_aux(struct ActorBody *body, u32 speed, u32 aux);
void draw_while_group_busy(struct ActorBody *body);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);

void release_fx_a(struct ActorBody *body)
{
    struct World **pw = &gWorld;
    struct ActorBody *partner = body->partner;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (partner != NULL)
        {
            obj_list_push_arg(body, 0);
        }
        if (body->data->effect != 0)
        {
            s32 x = (s16)obj_last_x(world_focus(*pw)) + 0x36;
            s32 y = (s16)obj_last_y(world_focus(*pw)) + 0x5c;
            place_targets(body, x, y, 10, -1);
            set_motion_start(body, 0x2c0);
        }
        body->state = check_offscreen_b;
    }
}

void set_focus_pos_b(struct ActorBody *body)
{
    s32 x;
    s32 y;
    s32 v;
    s32 minusOne;

    v = (s16)obj_last_x(gWorld->focus) * 0x100 + 0x10000;
    body->x = v;
    body->targetX = v;
    body->z = 0x7800;
    body->targetZ = 0x7800;
    if (body->data->effect != 0)
    {
        x = (s16)obj_last_x(gWorld->focus) + 0xD2;
        y = (s16)obj_last_y(gWorld->focus) + 0x7C;
        place_targets(body, x, y, 0x3C, -1);
        set_motion_start(body, 0x140);
    }
    minusOne = -1;
    set_slot_value(0x88, minusOne);
    set_slot_value(0x8D, minusOne);
    body->state = fx_only_b;
}

void release_fx_b(struct ActorBody *body)
{
    struct World **pw = &gWorld;
    struct ActorBody *partner = body->partner;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (partner != NULL)
        {
            obj_list_push_arg(body, 0);
        }
        if (body->data->effect != 0)
        {
            s32 x = (s16)obj_last_x(world_focus(*pw)) + 0x28;
            s32 y = (s16)obj_last_y(world_focus(*pw)) + 0x7c;
            place_targets(body, x, y, 10, -1);
            set_motion_start(body, 0x2c0);
        }
        body->state = check_offscreen;
    }
}

void set_focus_pos_c(struct ActorBody *body)
{
    s32 x;
    s32 y;
    s32 v;
    s32 minusOne;

    v = (s16)obj_last_x(gWorld->focus) * 0x100 + 0x10000;
    body->x = v;
    body->targetX = v;
    body->z = 0x7800;
    body->targetZ = 0x7800;
    if (body->data->effect != 0)
    {
        x = (s16)obj_last_x(gWorld->focus) + 0xD9;
        y = (s16)obj_last_y(gWorld->focus) + 0x6C;
        place_targets(body, x, y, 0x3C, -1);
        set_motion_start(body, 0xC0);
    }
    minusOne = -1;
    set_slot_value(0x88, minusOne);
    set_slot_value(0x8D, minusOne);
    body->state = fx_only_a;
}

void check_offscreen(struct ActorBody *body);
void fx_only_a(struct ActorBody *body)
{
    struct World **pw = &gWorld;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            s32 x = (s16)obj_last_x(world_focus(*pw)) + 0xb1;
            s32 y = (s16)obj_last_y(world_focus(*pw)) + 0x6c;
            place_targets(body, x, y, 0x18, -1);
            set_motion_start(body, 0x100);
        }
        body->state = follow_focus_while_effect;
    }
}

void check_offscreen(struct ActorBody *body)
{
    step_motion_d(body);
    if (body->sprite->unk0 + body->unk45 <= 0)
    {
        obj_release(body);
    }
}

void fx_only_b(struct ActorBody *body)
{
    struct World **world = &gWorld;
    s32 x;
    s32 y;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            x = (s16)obj_last_x(world_focus(*world)) + 0xaa;
            y = (s16)obj_last_y(world_focus(*world)) + 0x7c;
            place_targets(body, x, y, 0x18, -1);
            set_motion_start(body, 0x180);
        }
        body->state = place_on_focus_a;
    }
}

void check_offscreen_b(struct ActorBody *body)
{
    step_motion_d(body);
    if (body->sprite->unk0 + body->unk45 <= 0)
    {
        obj_release(body);
    }
}

void fx_only_c(struct ActorBody *body)
{
    struct World **world = &gWorld;
    s32 x;
    s32 y;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            x = (s16)obj_last_x(world_focus(*world)) + 0xb8;
            y = (s16)obj_last_y(world_focus(*world)) + 0x5c;
            place_targets(body, x, y, 0x18, -1);
            set_motion_start(body, 0x180);
        }
        body->state = place_on_focus_b;
    }
}

void free_data_done(struct ActorBody *body)
{
    if (body->child == 0)
    {
        release_object(body->data);
        task_list_remove(body->data);
    }
}

u32 part_step_set_anim_0b_mode1(struct ActorBody *body)
{
    body->unk50 = 0;
    body->state = set_anim_0b_mode1;
    body->unk7C = 0x100;
    return 1;
}

void follow_focus_while_effect(struct ActorBody *body)
{
    struct World **world = &gWorld;
    s32 x;
    s32 y;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            x = (s16)obj_last_x(world_focus(*world)) + 0x89;
            y = (s16)obj_last_y(world_focus(*world)) + 0x6c;
            place_targets(body, x, y, 10, -1);
            set_motion_start(body, 0x140);
        }
        body->state = place_on_focus_c;
    }
}

void place_on_focus_a(struct ActorBody *body)
{
    struct World **world = &gWorld;
    s32 x;
    s32 y;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            x = (s16)obj_last_x(world_focus(*world)) + 0x82;
            y = (s16)obj_last_y(world_focus(*world)) + 0x7c;
            place_targets(body, x, y, 10, -1);
            set_motion_start(body, 0x200);
        }
        body->state = release_fx_b;
    }
}

void place_on_focus_b(struct ActorBody *body)
{
    struct World **world = &gWorld;
    s32 x;
    s32 y;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            x = (s16)obj_last_x(world_focus(*world)) + 0x90;
            y = (s16)obj_last_y(world_focus(*world)) + 0x5c;
            place_targets(body, x, y, 10, -1);
            set_motion_start(body, 0x200);
        }
        body->state = release_fx_a;
    }
}

void set_anim_0b_mode1(struct ActorBody *body)
{
    sprite_set_action(body, 0xB, 0x40BF, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
    body->state = anim_0c_fx_at_pos;
}

void place_on_focus_c(struct ActorBody *body)
{
    struct World **world = &gWorld;
    s32 x;
    s32 y;

    step_motion(body);
    if (body->flags & 0x20)
    {
        if (body->data->effect != 0)
        {
            x = (s16)obj_last_x(world_focus(*world)) + 0x2F;
            y = (s16)obj_last_y(world_focus(*world)) + 0x6C;
            place_targets(body, x, y, 10, -1);
            set_motion_start(body, 0x14C);
        }
        body->state = remove_when_expired;
    }
}

void anim_0c_fx_at_pos(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 0xC, 0x40BF, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        set_slot_value(0xD5, -1);
        effect_spawn_at(0x1B78, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
        body->state = start_wander_timers;
    }
}

void remove_when_expired(struct ActorBody *body)
{
    step_motion_d(body);
    if (body->sprite->unk0 + body->unk45 <= 0)
    {
        obj_release(body);
    }
}

void spawn_five_children(struct ActorBody *body)
{
    struct ActorBody *child;

    child = obj_spawn_linked(body, 0x40EA, 0);
    child->flags |= 1;
    child->flags |= 8;
    child->index = body->index - 1;
    child = obj_spawn_linked(body, 0x40EB, 0);
    child->flags |= 1;
    child->flags |= 8;
    child->index = body->index - 2;
    child = obj_spawn_linked(body, 0x40EC, 0);
    child->flags |= 1;
    child->flags |= 8;
    child->index = body->index - 3;
    child = obj_spawn_linked(body, 0x40ED, 0);
    child->flags |= 1;
    child->flags |= 8;
    child->index = body->index - 4;
    child = obj_spawn_linked(body, 0x40EE, 0);
    child->flags |= 1;
    child->flags |= 8;
    child->index = body->index - 5;
}

NONMATCH("asm/code_0809C68C/start_child_chain.s", void start_child_chain(struct ActorBody *body))
{
    s32 animId = 0x40EF;
    struct ActorBody *child;
    const s8 *table;
    const s8 *cursor;
    const s8 *q;
    const s8 *t1p;
    s32 offset;
    s32 b1;
    s32 b2;
    s32 b3;
    s32 x;
    s32 y;
    s32 z;
    u32 *srcTime;
    u32 *srcA0;
    u32 *srcA4;

    if (body->sprite->flags & 0x8)
    {
        if (body->child == NULL)
        {
            body->state = enter_start_pose_4;
        }
        else
        {
            sprite_set_action(body, 3, -1, 0);
            ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
            apply_pose_to_parts(body);
            child = body->child;
            srcTime = &body->timeLink;
            srcA0 = &body->unkA0;
            srcA4 = &body->unkA4;
            table = gChildChainOffsetTable;
            cursor = table;
            offset = 0;
            while (child->child != NULL && child->child->state == NULL)
            {
                cursor += 3;
                offset += 3;
                child = child->child;
            }
            b1 = cursor[0];
            t1p = table + 1;
            q = offset + t1p;
            b2 = q[0];
            b3 = cursor[2];
            x = child->x + (b1 << 8);
            child->x = x;
            y = child->y + (b2 << 8);
            child->y = y;
            z = child->z + (b3 << 8);
            child->z = z;
            effect_spawn_at(0xE82, x / 0x100, y / 0x100, z / 0x100, child);
            set_slot_value(0x86, -1);
            child->flags &= ~8;
            sprite_set_action(child, 0, animId, 0);
            ((struct SpriteFlagBits *)&child->sprite->flags)->mode = 0;
            child->partner = body->partner;
            child->timeLink = *srcTime;
            child->unkA0 = *srcA0;
            child->unkA4 = *srcA4;
            child->unk6C = body->unk6C;
            child->flags &= ~1;
            child->unk78 = (child->unk78 & 0x1F) | 0x40;
            child->unk50 = 0;
            child->state = place_beside_partner;
            child->unk7C = 0x100;
            body->state = clear_state_if_sprite_active;
        }
    }
}
END_NONMATCH

void set_motion_speed_aux(struct ActorBody *body, u32 speed, u32 arg);
void float_above_focus(struct ActorBody *body)
{
    s32 focusX;
    s32 x;
    s32 dx;

    step_motion(body);
    if (body->link++ > 5)
    {
        body->link = 0;
        effect_spawn_at(0xE89, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
    }
    if (body->flags & 0x20)
    {
        obj_list_push_default(body);
        body->unk5C = approach_contact;
        body->unk60 = approach_slot;
        body->vspeed = 0xFE00;
        if (body->data->effect != 0)
        {
            focusX = (s16)obj_last_x(world_focus(gWorld));
            dx = focusX - body->targetX / 0x100;
            x = body->targetX / 0x100 + dx;
            place_targets(body, x, body->targetY / 0x100, body->targetZ / 0x100, -1);
            set_motion_speed_aux(body, 0x100, 0xC);
        }
        body->state = draw_while_group_busy;
    }
}

void draw_while_group_busy(struct ActorBody *body)
{
    struct ActorBody *member;

    step_motion_scaled(body);
    if (body->link++ > 5)
    {
        body->link = 0;
        effect_spawn_at(0xE89, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
    }
    if (body->flags & 0x20)
    {
        member = body->data->members;
        while (1)
        {
            if (member == 0)
            {
                sprite_set_action((struct ActorBody *)((u8 *)body->data + 8), 0, 0, 0);
                break;
            }
            if (member != body && member->state != 0)
            {
                break;
            }
            member = member->child;
        }
        obj_release(body);
    }
}

void enter_start_pose_4(struct ActorBody *body)
{
    body->state = start_pose_4;
}

u32 check_child_gone(struct ActorBody *body)
{
    if (body->child != 0)
    {
        return 0;
    }
    body->state = enter_start_pose_4;
    return 1;
}

void place_beside_partner(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;

    if (body->data->effect != 0)
    {
        place_targets(body, partner->targetX / 0x100 + 0x48, partner->targetY / 0x100, 0x14, -1);
        set_motion_start(body, 0x200);
    }
    body->link = 0;
    body->state = float_above_focus;
}

void clear_state_if_sprite_active(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        body->state = 0;
    }
}

u32 enter_start_pose_2(struct ActorBody *body)
{
    body->state = start_pose_2;
    return 1;
}

u32 enter_place_beside_partner(struct ActorBody *body)
{
    body->unk50 = 0;
    body->state = place_beside_partner;
    body->unk7C = 0x100;
    return 1;
}

void enter_stop_after_index_bump(struct ActorBody *body)
{
    body->unk54 = stop_after_index_bump;
}

void start_pose_4(struct ActorBody *body)
{
    s32 minusOne;

    spawn_five_children(body);
    minusOne = -1;
    sprite_set_action(body, 4, minusOne, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
    apply_pose_to_parts(body);
    set_slot_value(0x122, minusOne);
    body->state = anim_stop_if_sprite_active;
}

void start_pose_2(struct ActorBody *body)
{
    sprite_set_action(body, 2, -1, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
    body->state = start_child_chain;
}

void stop_after_index_bump(struct ActorBody *body)
{
    body->index += 5;
    spawn_five_children(body);
    sprite_set_action(body, 0, 0, 0);
    body->state = 0;
}

void anim_stop_if_sprite_active(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 0, 0, 0);
        body->state = 0;
    }
}

void float_parts_spawn_part_child(struct ActorBody *body)
{
    struct ActorBody *child;
    struct SpriteNode *node;
    struct SpriteNode *head;

    if (body->sprite->flags & 8)
    {
        set_slot_value(0x8B, -1);
        set_slot_value(0x8C, -1);
        sprite_set_action(body, 7, 0, 0);
        effect_spawn_at(0x1E47, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = obj_spawn(body->data, 0x410C, body->targetX / 0x100 - 0x15, body->targetY / 0x100 + 1, body->targetZ / 0x100, 1, 1);
        child->child = body->child;
        body->child = child;
        *((u8 *)child + 0x79) |= 1;
        child->timeLink = 0;
        child->node = sprite_node_take();
        child->node->sprite = child->sprite;
        child->node->link = &child->parts;
        child->unk6C = body->unk6C;
        child->partner = body->partner;
        child->baseX = child->x;
        child->baseY = child->y;
        child->vspeed = 0x14;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        head = child->parts;
        node->next = head;
        child->parts = node;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        head = child->parts;
        node->next = head;
        child->parts = node;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        head = child->parts;
        node->next = head;
        child->parts = node;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        head = child->parts;
        node->next = head;
        child->parts = node;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        head = child->parts;
        node->next = head;
        child->parts = node;
        child->state = orbit_parts_falling;
        body->state = pose_update_b;
    }
}
