#include "global.h"
#include "ram_routines.h"

struct ByteBit0
{
    u8 bit0 : 1;
    u8 hi : 7;
};

struct ActorSprite
{
    s16 unk0;
    u8 filler2[0x6 - 0x2];
    s16 unk6;
    u8 filler8[0xC - 0x8];
    s16 unkC;
    u8 filler[0x11 - 0xE];
    u8 unk11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 kind;
};

struct PartNode
{
    struct PartNode *next;
    s32 x;
    s32 y;
    s32 z;
    u8 filler10[0x1B - 0x10];
    u8 flags1B;
};

struct SpriteNode
{
    struct SpriteNode *next;
    struct ActorSprite *sprite;
    struct SpriteNode **link;
};

struct ActorData;
struct ActorBody;

typedef void (*BodyFunc)(struct ActorBody *body);

struct ActorBody
{
    u8 filler0[0x04];
    struct SpriteNode *node;
    struct ActorSprite *sprite;
    struct SpriteNode *parts;
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x28 - 0x1C];
    struct ActorData *data;
    struct ActorBody *partner;
    struct ActorBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    BodyFunc state;
    u8 filler50[0x54 - 0x50];
    BodyFunc subState;
    BodyFunc unk58;
    u8 filler5C[0x6C - 0x5C];
    s32 unk6C;
    u8 filler70[0x75 - 0x70];
    u8 index;
    u8 mode;
    u8 unk77;
    u8 filler78;
    u8 flags;
    u8 filler7A[0x84 - 0x7A];
    s32 baseX;
    s32 baseY;
    u8 filler8C[0x8E - 0x8C];
    s16 unk8E;
    u8 filler90[0x9C - 0x90];
    s32 height;
    u8 filler_A0[0xA8 - 0xA0];
    s32 tick;
    u16 wait;
    s16 vspeed;
    u8 fillerB0[0xB2 - 0xB0];
    u16 speed;
};

struct SpriteByte11Bits
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
};

struct UnkFlagBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct SpriteFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ModeBits
{
    u8 low : 3;
    u8 mode : 3;
    u8 high : 2;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

struct World
{
    u8 filler0[0x58];
    u8 *partners[8];
    u8 filler78[0x24C - 0x78];
    void *focus24C;
};

extern struct World *gWorld;

struct ActorData
{
    u8 filler0[0x08];
    u8 filler08[0x18 - 0x08];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 filler24[0x38 - 0x24];
    struct ActorBody *members;
    u8 filler3C[0x44 - 0x3C];
    s32 unk44;
    u8 filler48[0x7E - 0x48];
    u8 mode7E;
    u8 filler7F[0xB0 - 0x7F];
    s32 childCount;
    u8 fillerB4[0xEC - 0xB4];
    s16 poseOfs;
    u8 fillerEE[0xF6 - 0xEE];
    s16 effect;
    u8 fillerF8[0x113 - 0xF8];
    u8 flag113;
};

void on_arrive(struct ActorBody *body);
void sprite_set_action(struct ActorBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorBody *owner);
struct ActorBody *obj_spawn_linked(struct ActorBody *parent, u32 kind, u32 value);
void float_parts_spawn_part_child(struct ActorBody *body);
void spawn_small_child(struct ActorBody *body);
void start_anim4_fx_a(struct ActorBody *body);
void start_anim4_fx_b(struct ActorBody *body);
void spawn_child_and_stop(struct ActorBody *body);
void start_pair_anims_a(struct ActorBody *body);
void start_pair_anims_b(struct ActorBody *body);
void pair_wait_then_spawn_small(struct ActorBody *body);
void pair_wait_then_spawn_part(struct ActorBody *body);
void approach_heroes_step(struct ActorBody *body);
void arrange_child_nodes(struct ActorBody *body);
void slide_and_settle(struct ActorBody *body);
void obj_release(struct ActorBody *obj);
void body_fall_and_land(struct ActorBody *body);
void timed_child_effects(struct ActorBody *body);
struct SpriteNode *sprite_node_take(void);
struct SpriteNode *sprite_node_alloc(s32 x, s32 y, s32 z);
struct ActorBody *obj_spawn(struct ActorData *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void orbit_parts_gentle(struct ActorBody *body);
void pose_update_a(struct ActorBody *body);
void focus_follow_fall_step(struct ActorBody *body);
void step_motion_or_remove(struct ActorBody *body);
void obj_list_remove(struct ActorBody *obj);
void spawn_twelve_parts(struct ActorBody *body);
void pose_offset_place(struct ActorBody *body);
void start_pair_motion(struct ActorBody *body);
void anim_b_then_wait(struct ActorBody *body);
void wait_for_index(struct ActorBody *body);
u32 partner_link_update(struct ActorBody *obj, struct ActorBody *partner, u32 mode);
void spawn_pending_child(struct ActorBody *body);
extern u8 gSys[];
void lower_height_then_slide(struct ActorBody *body);
void obj_detach_active(struct ActorSprite *pose);
void place_targets(struct ActorBody *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct ActorBody *obj, s32 a);
void obj_list_push_default(struct ActorBody *obj);
s32 tick_timer_or_remove(struct ActorBody *body, struct ActorBody *other);
void countdown_then_next_state(struct ActorBody *body);
extern s8 gPairStartByte;
extern const s8 gPairCountTable[];
u32 step_motion(struct ActorBody *obj);
void hero_helper_spawn(struct ActorBody *body);
void place_by_heroes(struct ActorBody *body);
void hero_pair_link(struct ActorBody *body);
void step_then_effect(struct ActorBody *body);
void sound_stop_song(u32 song);

ASM_FUNC("asm/code_0809D474/orbit_parts_falling.s", void orbit_parts_falling(void))

NONMATCH("asm/code_0809D474/spawn_small_child.s", void spawn_small_child(struct ActorBody *body))
{
    struct ActorBody *child;
    struct SpriteNode *node;
    s32 minusOne;

    if ((body->sprite->flags & 8) != 0)
    {
        minusOne = -1;
        set_slot_value(0x8B, minusOne);
        set_slot_value(0x8C, minusOne);
        sprite_set_action(body, 7, 0, 0);
        effect_spawn_at(0x1E47, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = obj_spawn(body->data, 0x403D, body->targetX / 0x100 - 0x15, body->targetY / 0x100 + 1,
                          body->targetZ / 0x100, 1, 1);
        child->child = body->child;
        body->child = child;
        ((struct ByteBit0 *)&child->flags)->bit0 = 1;
        child->height = 0;
        child->node = sprite_node_take();
        child->node->sprite = child->sprite;
        child->node->link = &child->parts;
        child->unk6C = body->unk6C;
        child->partner = body->partner;
        child->baseX = child->x;
        child->baseY = child->y;
        child->vspeed = 2;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        node->next = child->parts;
        child->parts = node;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        node->next = child->parts;
        child->parts = node;
        node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
        node->next = child->parts;
        child->parts = node;
        child->state = orbit_parts_gentle;
        body->state = pose_update_a;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0809D474/orbit_parts_gentle.s", void orbit_parts_gentle(struct ActorBody *body))

void pose_update_a(struct ActorBody *body)
{
    if (body->sprite->flags & 8)
    {
        on_arrive(body);
    }
}

s32 enter_start_anim4_fx_a(struct ActorBody *body)
{
    body->state = start_anim4_fx_a;
    return 1;
}

void pose_update_b(struct ActorBody *body)
{
    if (body->sprite->flags & 8)
    {
        on_arrive(body);
    }
}

s32 enter_start_anim4_fx_b(struct ActorBody *body)
{
    body->state = start_anim4_fx_b;
    return 1;
}

void set_sub_state_spawn_child(struct ActorBody *body)
{
    body->subState = spawn_child_and_stop;
}

void start_anim4_fx_a(struct ActorBody *body)
{
    set_slot_value(0xAF, -1);
    sprite_set_action(body, 4, 0, 0);
    effect_spawn_at(0x1E3D, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
    body->state = start_pair_anims_a;
}

void start_anim4_fx_b(struct ActorBody *body)
{
    set_slot_value(0xAF, -1);
    sprite_set_action(body, 4, 0, 0);
    effect_spawn_at(0x1E3D, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
    body->state = start_pair_anims_b;
}

void spawn_child_and_stop(struct ActorBody *body)
{
    struct ActorBody *child = obj_spawn_linked(body, 0x4109, 0);
    u8 *fp;
    s32 flags;

    fp = &child->flags;
    flags = *fp;
    flags |= 1;
    flags |= 8;
    *fp = flags;
    child->index = 0xFF;
    sprite_set_action(body, 0, 0, 0);
    body->state = 0;
}

void start_pair_anims_a(struct ActorBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0x410A, 0);
        sprite_set_action(body->child, 0, 0x410B, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
        ((struct SpriteFlagBits *)&body->child->sprite->flags)->mode = 0;
        body->wait = 0x32;
        body->state = pair_wait_then_spawn_small;
    }
}

void start_pair_anims_b(struct ActorBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0x410A, 0);
        sprite_set_action(body->child, 0, 0x410B, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
        ((struct SpriteFlagBits *)&body->child->sprite->flags)->mode = 0;
        body->wait = 0;
        body->state = pair_wait_then_spawn_part;
    }
}

void pair_wait_then_spawn_small(struct ActorBody *body)
{
    u16 wait = body->wait;

    if ((s16)body->wait > 0)
    {
        body->wait = wait - 1;
    }
    else
    {
        sprite_set_action(body, 0, 0x4108, 0);
        sprite_set_action(body->child, 6, 0x4109, 0);
        body->state = spawn_small_child;
    }
}

void pair_wait_then_spawn_part(struct ActorBody *body)
{
    u16 wait = body->wait;

    if ((s16)body->wait > 0)
    {
        body->wait = wait - 1;
    }
    else
    {
        sprite_set_action(body, 0, 0x4108, 0);
        sprite_set_action(body->child, 6, 0x4109, 0);
        body->state = float_parts_spawn_part_child;
    }
}

void place_by_heroes(struct ActorBody *body)
{
    struct ActorBody *heroA;
    struct ActorBody *heroB;
    struct ActorBody *partner;
    s32 y;
    s32 sum;

    heroA = (struct ActorBody *)(gWorld->partners[6] + 8);
    heroB = (struct ActorBody *)(gWorld->partners[7] + 8);
    sprite_set_action(body, 8, 0, 0);
    if ((6 & heroA->mode) != 2)
    {
        y = heroB->targetY / 0x100;
    }
    else if ((6 & heroB->mode) != 2)
    {
        y = heroA->targetY / 0x100;
    }
    else
    {
        sum = heroA->targetY + heroB->targetY;
        if (sum < 0)
        {
            sum += 0x1FF;
        }
        y = sum >> 9;
    }
    partner = body->partner;
    place_targets(body, partner->targetX / 0x100 + 0x38, y, 0, 0x100);
    set_motion_start(body, 0x180);
    set_slot_value(0x99, -1);
    body->state = step_then_effect;
}

ASM_FUNC("asm/code_0809D474/hero_helper_spawn.s", void hero_helper_spawn(struct ActorBody *body))

void hero_pair_link(struct ActorBody *body)
{
    struct ActorBody *heroA;
    struct ActorBody *heroB;
    struct ActorBody *hero;
    struct ActorBody *child;
    s32 wait;
    s32 i;

    wait = body->wait;
    wait--;
    body->wait = wait;
    if ((s16)wait > 0)
    {
        heroA = (struct ActorBody *)(gWorld->partners[6] + 8);
        heroB = (struct ActorBody *)(gWorld->partners[7] + 8);
        if ((6 & heroA->mode) == 2 && heroA->child->state != 0)
        {
            return;
        }
        if ((6 & heroB->mode) == 2 && heroB->child->state != 0)
        {
            return;
        }
    }
    sprite_set_action(body, 0xB, 0, 0);
    for (i = 6; i <= 7; i++)
    {
        hero = (struct ActorBody *)(gWorld->partners[i] + 8);
        if ((6 & hero->mode) == 2)
        {
            ((struct ModeBits *)&hero->mode)->mode = 0;
            child = hero->child;
            if (child != 0)
            {
                if (child->sprite->unk6 == 0x100)
                {
                    effect_spawn_at(0xE8, child->targetX / 0x100, child->targetY / 0x100, child->targetZ / 0x100, hero);
                    partner_link_update(body, hero, 0xFF);
                }
                else
                {
                    effect_spawn_at(0xDF, child->targetX / 0x100, child->targetY / 0x100, child->targetZ / 0x100, hero);
                }
                if (hero->child != 0)
                {
                    obj_release(hero->child);
                }
            }
        }
    }
    body->state = wait_for_index;
}

void slide_and_settle(struct ActorBody *body)
{
    struct ActorData *data;
    u16 value;
    s32 limit;
    u32 flagMask;

    body->x = body->data->unk18;
    body->y = body->data->unk1C;
    body->z = body->data->unk20;
    body->sprite->unk6 += 4;
    value = body->sprite->unk6;
    if ((s16)body->sprite->unk6 > (limit = 0x100))
    {
        body->sprite->unk6 = limit;
        body->state = 0;
        return;
    }
    data = body->data;
    flagMask = (data->poseOfs == -1) ? 1 : 2;
    if ((((struct SysKeys3E *)gSys)->keys & flagMask) == 0)
    {
        return;
    }
    if ((*(u16 *)((u8 *)data + 0x11E) & 0x300) != 0)
    {
        return;
    }
    body->sprite->unk6 = value - 0x18;
    if ((s16)body->sprite->unk6 <= 0)
    {
        body->sprite->unk6 = 2;
    }
    effect_spawn_at(0xDF, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
    place_targets(body, body->x / 0x100, body->y / 0x100, body->z / 0x100, limit);
    ((struct ModeBits *)&body->data->mode7E)->mode = 0;
    sprite_set_action((struct ActorBody *)((u8 *)body->data + 8), 5, 0, 0);
    ((struct ModeBits *)&body->data->mode7E)->mode = 1;
    body->data->unk20 = (body->z += 0x200);
    body->state = lower_height_then_slide;
}
void lower_height_then_slide(struct ActorBody *body)
{
    s32 h;
    struct ActorData *data;

    h = body->z - 0xCC;
    if (h < 0)
        h = 0;
    data = body->data;
    body->z = h;
    data->unk20 = h;
    if (h == 0)
    {
        ((struct ModeBits *)&data->mode7E)->mode = 0;
        sprite_set_action((struct ActorBody *)((u8 *)body->data + 8), 0, 0, 0);
        ((struct ModeBits *)&body->data->mode7E)->mode = 1;
        body->state = slide_and_settle;
    }
}


void step_then_effect(struct ActorBody *body)
{
    if ((u8)step_motion(body) == 0)
    {
        sprite_set_action(body, 9, 0, 0);
        set_slot_value(0x118, -1);
        set_slot_value(0x127, -1);
        effect_spawn_at(0x1E8A, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        body->wait = 0x96;
        body->state = hero_helper_spawn;
    }
}


NONMATCH("asm/code_0809D474/reset_heroes_mode.s", void reset_heroes_mode(struct ActorBody *body))
{
    s32 heroFirst = 6;
    struct ActorBody *hero;
    s32 i;
    s32 keep;

    keep = ~0x38;
    for (i = heroFirst; i <= 7; i++)
    {
        hero = (struct ActorBody *)(gWorld->partners[i] + 8);
        if ((heroFirst & hero->mode) == 2)
        {
            hero->mode = (hero->mode & keep) | 8;
        }
    }
    body->state = place_by_heroes;
}
END_NONMATCH


void spawn_hero_children(struct ActorBody *body)
{
    struct ActorBody *heroBody;
    struct ActorBody *child;
    u8 *fp;
    s32 flags;
    s32 i;

    body->data->childCount = 0;
    for (i = 6; i <= 7; i++)
    {
        heroBody = (struct ActorBody *)(gWorld->partners[i] + 8);
        if ((heroBody->mode & 6) == 2)
        {
            child = obj_spawn_linked(body, 0x4147, 0);
            sprite_set_action(child, 2, 0, 0);
            ((struct SpriteFlagBits *)&child->sprite->flags)->mode = 0;
            fp = &child->flags;
            flags = *fp;
            flags |= 1;
            *fp = flags;
            child->index = heroBody->index - 1;
            child->x = heroBody->targetX;
            child->y = heroBody->targetY;
            child->z = 0xA000;
            child->partner = heroBody;
            child->state = pose_offset_place;
            body->data->childCount++;
        }
    }
    body->data->flag113 = 1;
    sprite_set_action(body, 7, 0, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
    body->state = spawn_pending_child;
}

void spawn_pending_child(struct ActorBody *body)
{
    struct ActorBody *child;
    s32 pending;
    u8 *fp;
    s32 flags;

    pending = body->data->childCount;
    if (pending == 0)
    {
        child = obj_spawn_linked(body, 0x8031, 0);
        fp = &child->flags;
        flags = *fp;
        flags |= 1;
        *fp = flags;
        child->index = body->index - 1;
        child->x += 0x2000;
        child->z += 0x2000;
        sprite_set_action(child, 5, 0, 0);
        ((struct SpriteByte11Bits *)&child->sprite->unk11)->bit6 = 0;
        sprite_set_action(body, 8, 0, 0);
        body->wait = pending;
        body->tick = 0x14;
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
        body->state = timed_child_effects;
    }
}

void timed_child_effects(struct ActorBody *body)
{
    s32 wait;

    if (body->tick > 0)
    {
        body->tick--;
        if (body->tick == 0)
        {
            body->tick = 0x14;
            effect_spawn_at(0x227F, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        }
    }
    wait = body->wait + 1;
    body->wait = wait;
    if ((s16)wait > 0x31)
    {
        body->wait = 0;
        if (body->child->sprite->kind != 0)
        {
            sprite_set_action(body->child, body->child->sprite->kind - 1, 0, 0);
            set_slot_value(0x10A, -1);
        }
        else
        {
            obj_detach_active(body->child->sprite);
            effect_spawn_at(0x2291, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
            sprite_set_action(body, 9, 0, 0);
            ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
            body->state = spawn_twelve_parts;
        }
    }
}

void spawn_twelve_parts(struct ActorBody *body)
{
    struct ActorBody *child;
    struct PartNode *prev;
    struct PartNode *node;
    struct SpriteNode *taken;
    u8 *fp;
    s32 flags;
    s32 i;

    if ((body->sprite->flags & 8) != 0)
    {
        effect_spawn_at(0x229B, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = body->child;
        child->x = body->x;
        child->y = body->y;
        child->z = body->z;
        sprite_set_action(child, 0, 0x4146, 0);
        ((struct SpriteFlagBits *)&child->sprite->flags)->mode = 1;
        child->state = start_pair_motion;
        set_slot_value(0xD2, -1);
        child = obj_spawn_linked(child, 0x4145, 0);
        fp = &child->flags;
        flags = *fp;
        flags |= 1;
        *fp = flags;
        sprite_set_action(child, 0, 0, 0);
        ((struct SpriteFlagBits *)&child->sprite->flags)->mode = 1;
        prev = (struct PartNode *)&child->parts;
        taken = sprite_node_take();
        child->node = taken;
        taken->sprite = child->sprite;
        taken->link = &child->parts;
        i = 0xB;
        do
        {
            node = (struct PartNode *)sprite_node_alloc(child->x, child->y, child->z);
            prev->next = node;
            prev = node;
            node = (struct PartNode *)sprite_node_alloc(child->x, child->y, child->z);
            prev->next = node;
            prev = node;
            node = (struct PartNode *)sprite_node_alloc(child->x, child->y, child->z);
            prev->next = node;
            prev = node;
            node = (struct PartNode *)sprite_node_alloc(child->x, child->y, child->z);
            prev->next = node;
            prev = node;
            node = (struct PartNode *)sprite_node_alloc(child->x, child->y, child->z);
            prev->next = node;
            prev = node;
            node = (struct PartNode *)sprite_node_alloc(child->x, child->y, child->z);
            prev->next = node;
            prev = node;
            i -= 6;
        } while (i >= 0);
        prev->next = 0;
        sprite_set_action(body, 0xA, 0, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
        body->state = anim_b_then_wait;
    }
}

void pose_offset_place(struct ActorBody *body)
{
    place_targets(body, body->targetX / 0x100, body->targetY / 0x100,
                  (body->partner->data->poseOfs == -1) ? 0x22 : 0x26, 0x100);
    set_motion_start(body, 0x300);
    set_slot_value(0xA3, -1);
    obj_list_push_default(body);
    body->unk58 = (BodyFunc)tick_timer_or_remove;
    ((struct UnkFlagBits *)&body->unk77)->bit6 = 0;
    ((struct UnkFlagBits *)&body->unk77)->bit5 = 0;
    body->vspeed = body->height;
    sprite_set_action(body, 0, 0, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
    body->state = focus_follow_fall_step;
}

void focus_follow_fall_step(struct ActorBody *body)
{
    u8 done;

    done = step_motion(body);
    if (body->tick != 0)
    {
        body->speed = body->speed - 0xA0;
        body->tick = body->tick + (s16)body->speed;
        if (!(body->tick > 0))
        {
            body->tick = 0;
            body->speed = 0;
            sprite_set_action(body, 0, 0, 0);
        }
        body->z += body->tick;
    }
    if (done == 0)
    {
        body->data->childCount--;
        body->state = body_fall_and_land;
    }
}

void body_fall_and_land(struct ActorBody *body)
{
    struct ActorBody *partner;
    s16 *thr;

    thr = (s16 *)((u8 *)body + 0x8C);
    if (body->targetZ > ((s32)thr[1] << 8) || (s16)body->speed > 0)
    {
        if (body->tick != 0)
        {
            body->z += body->tick;
            body->tick = 0;
        }
        body->speed = body->speed - 0xA0;
        body->z += (s16)body->speed;
        if (body->z <= ((s32)thr[1] << 8))
        {
            body->speed = 0;
            body->z = (s32)thr[1] << 8;
            sprite_set_action(body, 0, 0, 0);
        }
    }
    partner = body->partner;
    if ((partner->mode & 6) == 2 && partner->sprite->kind == 8)
    {
        sprite_set_action(partner, 0, 0, 0);
        partner->state = 0;
    }
    if (body->data->flag113 == 0)
    {
        obj_list_remove(body);
        place_targets(body, body->targetX / 0x100, body->targetY / 0x100, 0xA0, 0x100);
        set_motion_start(body, 0x400);
        sprite_set_action(body, 2, 0, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
        set_slot_value(0xA3, -1);
        body->state = step_motion_or_remove;
    }
}

s32 tick_timer_or_remove(struct ActorBody *body, struct ActorBody *other)
{
    u16 timer;

    other->speed = 0;
    timer = body->vspeed;
    if ((s16)body->vspeed > 0)
    {
        body->vspeed = timer - 1;
    }
    if ((s16)body->vspeed == 0)
    {
        effect_spawn_at(0x22AB, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, 0);
        set_slot_value(0x55, -1);
        obj_release(body);
        return -2;
    }
    body->speed = 0x300;
    body->tick = 0x300;
    sprite_set_action(body, 1, 0, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
    set_slot_value(0x47, -1);
    return 0;
}

void start_pair_motion(struct ActorBody *body)
{
    struct ActorBody *child;
    u16 *speed;
    BodyFunc next;

    speed = &body->speed;
    *speed = 0;
    body->tick = gPairStartByte << 8;
    arrange_child_nodes(body);
    if (body->child->sprite->flags & 8)
    {
        sprite_set_action(body, 1, 0, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
        child = body->child;
        sprite_set_action(child, 1, 0, 0);
        ((struct SpriteFlagBits *)&child->sprite->flags)->mode = 0;
        next = countdown_then_next_state;
        body->state = next;
        body->wait = gPairCountTable[(s16)*speed];
        next(body);
    }
}

void arrange_child_or_remove(struct ActorBody *body);
ASM_FUNC("asm/code_0809D474/approach_heroes_step.s", void approach_heroes_step(struct ActorBody *body))

ASM_FUNC("asm/code_0809D474/arrange_child_nodes.s", void arrange_child_nodes(struct ActorBody *body))
void arrange_child_or_remove(struct ActorBody *body)
{
    arrange_child_nodes(body);
    if (body->child->sprite->flags & 8)
    {
        sound_stop_song(0xD2);
        obj_release(body->child);
        obj_release(body);
    }
}


void countdown_then_next_state(struct ActorBody *body)
{
    u16 wait = body->wait;

    if ((s16)body->wait > 0)
    {
        body->wait = wait - 1;
    }
    else
    {
        body->state = approach_heroes_step;
    }
}

