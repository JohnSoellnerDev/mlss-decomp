#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 cmd;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct Flags77Bits
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gUnk_03000FD8;

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
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
    u8 filler76[0x77 - 0x76];
    u8 flags77;
    u8 flags78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_release(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void start_motion(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);
void obj_index_zero_action4_exit(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 mt19937_next(void);
void obj_spawn_part_child(struct ActorObj *obj);
void obj_pose_active_move_high(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, u32 a, u32 b, u32 code);
u32 query_contact(struct ActorObj *obj, struct ActorObj *other, u32 arg);
void obj_spawn_child_by_slot(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);

u32 actor_start_action2_then_move_high(struct ActorObj *obj);
u32 actor_init_side_mode(struct ActorObj *obj);
u32 actor_default_speed(struct ActorObj *obj);
void actor_action2_then_move_high(struct ActorObj *obj);
void actor_release_when_free(struct ActorObj *obj);
void actor_action_from_a4_notify(struct ActorObj *obj);
void actor_stop_when_pose_active(struct ActorObj *obj);
void actor_action5_then_spawn_child(struct ActorObj *obj);
void actor_side_action_when_active(struct ActorObj *obj);
void actor_side_action_timer8_child(struct ActorObj *obj);
void actor_spawn_motion_child(struct ActorObj *obj);
void actor_retarget_to_owner_part(struct ActorObj *obj);
void actor_plan_arc_path(struct ActorObj *obj);
void actor_arc_move_step(struct ActorObj *obj);
void actor_effect_then_wait_index(struct ActorObj *obj);
void actor_spawn_counted_child(struct ActorObj *obj);
void actor_finish_with_child_effect(struct ActorObj *obj);
void actor_finish_child_effect_b(struct ActorObj *obj);
void actor_countdown_release_child(struct ActorObj *obj);
void actor_start_motion_to_part(struct ActorObj *obj);
u32 actor_link_partner_then_step(struct ActorObj *obj, struct ActorObj *other);
void actor_effect_then_clear_index(struct ActorObj *obj);
void actor_action_a_or_12_child(struct ActorObj *obj);
void actor_action8_or_10_timer(struct ActorObj *obj);
u32 actor_wait_for_slot(struct ActorObj *obj, u32 slot);
u32 actor_unlink_partners_and_release(struct ActorObj *obj);
void actor_countdown_release_partners(struct ActorObj *obj);
void actor_release_when_free_b(struct ActorObj *obj);
void actor_wait_index_then_timer(struct ActorObj *obj);
void actor_idle_when_pose_active(struct ActorObj *obj);
u32 actor_set_home_side1(struct ActorObj *obj);
u32 actor_set_home_side0(struct ActorObj *obj);
u32 actor_start_action2_then_action3(struct ActorObj *obj);
void actor_set_home_and_motion(struct ActorObj *obj);
void actor_action2_then_action3(struct ActorObj *obj);
void actor_action3_spawn_effect(struct ActorObj *obj);
void actor_wait_then_side_action(struct ActorObj *obj);
void actor_spawn_effect_set_cmd(struct ActorObj *obj);
void actor_spawn_lift_child(struct ActorObj *obj);
void actor_random_partner_link(struct ActorObj *obj);
void actor_linear_wave_step(struct ActorObj *obj);
u32 actor_vanish_if_slot_missing(struct ActorObj *obj, u32 slot);
u32 actor_link_and_release(struct ActorObj *obj, u32 other);
void actor_countdown_then_timer(struct ActorObj *obj);
u32 actor_link_contact_partner(struct ActorObj *obj, struct ActorObj *other, u32 arg);
u32 actor_set_speed_link(struct ActorObj *obj, u32 other);
void obj_start_effect_by_side(struct ActorObj *obj);
void obj_flip_side_walk(struct ActorObj *obj);
void obj_side_action_timer(struct ActorObj *obj);
u32 obj_pick_random_count(struct ActorObj *obj);
u32 obj_start_count_side1(struct ActorObj *obj);
u32 obj_start_count_side0(struct ActorObj *obj);
void obj_timer_action15(struct ActorObj *obj);
void obj_place_sprite_at_base(struct ActorObj *obj);
void obj_stop_clear_state(struct ActorObj *obj);
void obj_walk_by_side(struct ActorObj *obj);
void obj_pose_action15(struct ActorObj *obj);
void obj_fall_and_launch(struct ActorObj *obj);

ASM_FUNC("asm/code_0806CB50/actor_wait_then_side_action.s", void actor_wait_then_side_action(struct ActorObj *obj))

void actor_spawn_effect_set_cmd(struct ActorObj *obj)
{
    u32 id;

    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            set_slot_value(0x44, -1);
            obj->param9C = 0;
            if (obj->paramA8 == 0)
            {
                sprite_set_action(obj, 0xA, 0, 0);
                id = 0x163A;
            }
            else
            {
                sprite_set_action(obj, 0x10, 0, 0);
                id = 0x16BC;
            }
            gUnk_03000E3C = effect_from_table_obj(obj, id);
            ((struct Flags77Bits *)&obj->flags77)->bit6 = 1;
            obj->handler58 = (void (*)(struct ActorObj *))actor_set_speed_link;
            obj->handler5C = (void (*)(struct ActorObj *))actor_link_contact_partner;
            obj->pose->cmd = obj->paramA0;
            obj->state = obj_start_effect_by_side;
        }
    }
}

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct NodeModeBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

void actor_spawn_lift_child(struct ActorObj *obj)
{
    struct ActorWorld *world;
    u8 *nodeA;
    u8 *nodeB;
    struct ActorObj *child;
    void (*next)(struct ActorObj *);

    if (obj->pose->flags & 8)
    {
        world = gUnk_03000FD8;
        nodeA = world->partners[6];
        nodeB = world->partners[7];
        if (!(((struct NodeModeBits *)(nodeB + 0x7E))->mode == 1 || ((struct NodeModeBits *)(nodeA + 0x7E))->mode == 1))
        {
            sprite_set_action(obj, 0, 0, 0);
            next = 0;
        }
        else
        {
            sprite_set_action(obj, 0x16, 0, 0);
            effect_spawn_at(0x1DF0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, obj);
            child = obj_spawn(obj->part, 0x4101, obj->baseX / 256 - 0x1A, obj->baseY / 256 + 2,
                              obj->baseH / 256 + 0xB, ((u32)obj->pose->unk11 << 25) >> 31, 1);
            obj->child = child;
            ((struct Flags78Bits *)&child->flags78)->kind = 2;
            ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
            ((struct ObjFlags77 *)child)->bit5 = 0;
            child->handler68 = on_arrive;
            child->unk6C = obj->unk6C;
            child->owner = obj->owner;
            child->tag = obj->tag;
            obj_list_push_default(child);
            child->handler58 = (void (*)(struct ActorObj *))actor_link_and_release;
            child->handler60 = (void (*)(struct ActorObj *))actor_vanish_if_slot_missing;
            child->state = actor_random_partner_link;
            child->param9C = obj->paramA0;
            sprite_set_action(child, 1, 0, 0);
            child->dh = 0;
            set_slot_value(0x12B, -1);
            next = actor_countdown_then_timer;
        }
        obj->state = next;
    }
}

ASM_FUNC("asm/code_0806CB50/actor_random_partner_link.s", void actor_random_partner_link(struct ActorObj *obj))

void actor_linear_wave_step(struct ActorObj *obj)
{
    s32 xRel;
    s32 yPart;
    s32 wave;
    u16 dhu;
    s32 *hp;
    s32 *angle;

    obj->posX = obj->posX - obj->param9C;
    xRel = obj->posX - (obj->dx << 8);
    yPart = (xRel * obj->targetY) / obj->targetX;
    obj->posY = yPart + (obj->dy << 8);
    hp = &obj->targetH;
    wave = gSineTable[(u8)obj->paramA0];
    angle = &obj->paramA0;
    if (wave < 0)
    {
        wave += 0x3F;
    }
    obj->posH = *hp + ((wave >> 6) << 3);
    *angle += mt19937_next() & 7;
    if (obj->pose->flags & 8)
    {
        dhu = obj->dh;
        if (obj->dh == 0)
        {
            obj->dh = dhu + 1;
            sprite_set_action(obj, 0, 0, 0);
        }
    }
    if (obj->posX / 0x100 <= -0x20)
    {
        obj_release(obj);
    }
}

u32 actor_vanish_if_slot_missing(struct ActorObj *obj, u32 slot)
{
    u32 result = lookup_slot(obj, slot);
    if (result == 0)
    {
        effect_spawn_at(0x1DF8, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
        return 0;
    }
    return result;
}

u32 actor_link_and_release(struct ActorObj *obj, u32 other)
{
    effect_spawn_at(0x1E01, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    partner_link_update(obj, other, 0xFF);
    obj_release(obj);
    return 0;
}

void actor_countdown_then_timer(struct ActorObj *obj)
{
    void (*next)(struct ActorObj *);

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->param9C--;
        if (obj->param9C <= 0)
        {
            next = 0;
        }
        else
        {
            obj->timer = obj->paramA4;
            next = obj_timer_action15;
        }
        obj->state = next;
    }
}

ASM_FUNC("asm/code_0806CB50/actor_link_contact_partner.s", u32 actor_link_contact_partner(struct ActorObj *obj, struct ActorObj *other, u32 arg))

u32 actor_set_speed_link(struct ActorObj *obj, u32 other)
{
    obj->param9C = 1;
    partner_link_update(obj, other, 0xFF);
    return 0;
}

void obj_start_effect_by_side(struct ActorObj *obj)
{
    u32 id;

    if (obj->pose->flags & 8)
    {
        obj->pose->cmd = 0x10;
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 0xC, 0, 0);
            id = 0x1660;
        }
        else
        {
            sprite_set_action(obj, 0x12, 0, 0);
            id = 0x16E2;
        }
        gUnk_03000E3C = effect_from_table_obj(obj, id);
        obj->handler58 = (void (*)(struct ActorObj *))actor_set_speed_link;
        obj->handler5C = (void (*)(struct ActorObj *))actor_link_contact_partner;
        obj->state = obj_stop_clear_state;
    }
}

void obj_flip_side_walk(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->paramA8 = (obj->paramA8 + 1) & 1;
        obj->param9C = 0;
        obj->paramA4 = 0;
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 7, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0xD, 0, 0);
        }
        obj->state = obj_side_action_timer;
    }
}

void obj_side_action_timer(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 8, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0xE, 0, 0);
        }
        obj->timer = obj->param9C;
        set_slot_value(0x7C, -1);
        obj->state = actor_wait_then_side_action;
    }
}

u32 obj_pick_random_count(struct ActorObj *obj)
{
    s32 old = obj->param9C;

    if (obj->param9C == 0)
    {
        obj->param9C = mt19937_next() % 5;
        obj->paramA0 = 0x100;
        obj->paramA4 = old;
    }
    obj->state = obj_place_sprite_at_base;
    return 1;
}

u32 obj_start_count_side1(struct ActorObj *obj)
{
    if (obj->param9C == 0 && obj->paramA0 == 0 && obj->paramA4 == 0)
    {
        obj->param9C = 8;
        obj->paramA0 = 0x10;
        obj->paramA4 = 0;
    }
    obj->paramA8 = 1;
    obj->state = obj_walk_by_side;
    return 1;
}

u32 obj_start_count_side0(struct ActorObj *obj)
{
    if (obj->param9C == 0 && obj->paramA0 == 0 && obj->paramA4 == 0)
    {
        obj->param9C = 8;
        obj->paramA0 = 0x10;
        obj->paramA4 = 0;
    }
    obj->paramA8 = 0;
    obj->state = obj_walk_by_side;
    return 1;
}

void obj_timer_action15(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        sprite_set_action(obj, 0x15, 0, 0);
        obj->state = actor_spawn_lift_child;
    }
}

void obj_place_sprite_at_base(struct ActorObj *obj)
{
    sprite_set_action(obj, 0x14, 0, 0);
    effect_spawn_at(0x1DE6, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj->state = obj_pose_action15;
}

void obj_stop_clear_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->pose->cmd = 0x10;
        obj->state = 0;
    }
}

void obj_walk_by_side(struct ActorObj *obj)
{
    if (obj->paramA8 == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 0xD, 0, 0);
    }
    obj->state = obj_side_action_timer;
}

void obj_pose_action15(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0x15, 0, 0);
        obj->state = actor_spawn_lift_child;
    }
}

void obj_fall_and_launch(struct ActorObj *obj)
{
    struct ActorObj *a;
    struct ActorPart *partB;

    obj->posH += obj->targetX;
    obj->targetX -= obj->targetH;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        sprite_set_action(obj, 3, 0, 0);
        a = (struct ActorObj *)(gUnk_03000FD8->partners[6] + 8);
        partB = *(struct ActorPart **)(gUnk_03000FD8->partners[7] + 0x30);
        if (partB->slot >= 0 || a->part->slot >= 0)
        {
            obj->dx = 1;
            obj->targetX = 0x7800;
            obj->targetY = 0x6C00;
            obj->targetH = obj->baseH;
            obj->motionSpeed = 0x200;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionStart = 0;
            start_motion(obj);
        }
        else
        {
            obj->dx = 0;
            obj->targetX = 0x5800;
            obj->targetY = 0x6C00;
            obj->targetH = obj->baseH;
            obj->motionSpeed = 0x200;
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->motionStart = 0;
            start_motion(obj);
        }
        obj->state = obj_index_zero_action4_exit;
        set_slot_value(0x81, -1);
    }
}
