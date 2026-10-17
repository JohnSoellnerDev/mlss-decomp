#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    s16 screenY;
    u8 filler2[0x11 - 0x02];
    u8 mirror;
    u8 flags;
};

struct SpriteMirrorBits
{
    u8 unk0 : 6;
    u8 flip : 1;
    u8 unk7 : 1;
};

struct SpriteModeBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct ActionModeBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 action : 3;
    u8 unk6 : 2;
};

struct ActiveBits
{
    u8 lo5 : 5;
    u8 active : 1;
    u8 hi2 : 2;
};

struct WorldFlagBits
{
    u8 low : 4;
    u8 moving : 1;
    u8 high : 3;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
    u8 filler0EE[0xF4 - 0xEE];
    u8 effect;
    s16 cursorX;
    s16 cursorY;
    u8 filler0FA[0xFC - 0xFA];
    s16 cursorX2;
    s16 cursorY2;
    u8 filler100[0x110 - 0x100];
    u8 partBits;
    u8 filler111[0x114 - 0x111];
    u32 slotValue;
    u8 filler118[0x11E - 0x118];
    u16 input;
};

struct HeroSpan
{
    s16 a;
    s16 b;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    s16 rate;
    s16 side;
    u16 timer;
    u8 filler22[0x24 - 0x22];
    u16 pair;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s8 edgeBottom;
    s8 edgeTop;
    u8 filler46;
    s8 candidateLimit;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    void *resume;
    u8 filler54[0x58 - 0x54];
    void *eventA;
    void *eventB;
    void *eventC;
    void *eventD;
    void (*callback)(struct HeroBody *);
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 handle;
    u16 scale;
    u16 phase;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 unk92;
    s32 speed;
    u8 filler98[0xA8 - 0x98];
    u32 link;
    s16 step;
    u8 filler0AE[0xB2 - 0xAE];
    u16 dh;
    u8 filler0B4[0xB8 - 0xB4];
    u16 jumpInit;
    u8 filler0BA[0xC3 - 0xBA];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xC8 - 0xC6];
    u16 unkC8;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

struct ActionEntry
{
    u8 type;
    u8 filler1;
    u8 kind;
    u8 filler3[0x05 - 0x03];
    u8 switchByte;
    u8 filler6[0x08 - 0x06];
    u16 menuKind;
};

struct PartBitsFlags
{
    u8 low7 : 7;
    u8 bit7 : 1;
};

struct Flags78Bits
{
    u8 lo5 : 5;
    u8 hi3 : 3;
};

struct WorldInputBits
{
    u8 lo2 : 2;
    u8 hi6 : 6;
};

struct World
{
    u8 filler0[0x0C];
    u8 flags;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x7C - 0x78];
    struct HeroData *parts;
    u8 filler80[0x2C0 - 0x80];
    u8 input;
    u8 filler2C1[0x2DC - 0x2C1];
    struct ActionEntry *curEntry;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern const u16 gEffectCodeTable[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
u8 step_motion(struct HeroBody *body);
void step_motion_d(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *obj, u32 value);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_step_and_offsets(struct HeroBody *body, u32 step, u32 offsets);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void schedule_pose_task(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void partner_link_update(struct HeroBody *body, u32 a, u32 b);
void obj_list_push_default(struct HeroBody *body);
void obj_release(struct HeroBody *body);
void obj_detach_active(struct HeroSprite *sprite);
void state_partner_action8(struct HeroBody *body);
void child_launch(struct HeroBody *body);
void menu_cursor_follow(struct HeroBody *body);
void partner_step_wait(struct HeroBody *body);
void partner_chase_pair_step(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void pose_switch(struct HeroBody *body);
void wait_motion_end(struct HeroBody *body);
s32 query_contact(struct HeroBody *a, struct HeroBody *b);
void visible_action_8(struct HeroBody *body);
void move_progress_check(struct HeroBody *body);
void pair_choose_partner(struct HeroBody *body);
void pair_place_targets(struct HeroBody *body);
void hero_start_move(struct HeroBody *body);
void step_motion_arrive(struct HeroBody *body);
void pair_countdown_spawn(struct HeroBody *body);
void spawn_marker_child(struct HeroBody *body);
void pick_random_partner(struct HeroBody *body);
void visible_action_9(struct HeroBody *body);
void noop_after_visible_action_9(struct HeroBody *body);
void partner_wait_after_connect(struct HeroBody *body);
void partner_move_a(struct HeroBody *body);
void move_until_offscreen(struct HeroBody *body);
void hold_roll_handle(struct HeroBody *body);
void land_at_target_height(struct HeroBody *body);
s32 menu_cursor_pos(s32 x, s32 y, u32 kind);
void place_part_in_slot(u32 effect, s32 slot, s32 x, s32 y, s32 z, s32 a, s32 b);
void enter_finish_rebuild(struct HeroBody *body);
u32 search_frame_pairs(struct HeroBody *body, struct HeroBody *partner);
void sprite_objects_obj_show_at(struct HeroBody *body, struct HeroBody *partner, u32 arg);
u32 mt19937_next(void);
u32 sub_081DE850(u32 range, u32 seed);
s32 contact_check(struct HeroBody *body, struct HeroBody *other);
u32 partner_link_hook(struct HeroBody *body, u32 a, u32 b);

void visible_action_2000(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0x2000, 0);
        body->state = 0;
    }
}

ASM_FUNC("asm/code_0809583C/actor_init_linked.s", u32 actor_init_linked(struct HeroBody *body))

void pick_random_partner(struct HeroBody *body)
{
    struct HeroBody *partner = 0;
    struct HeroBody *candidate;
    s32 count = 0;
    s32 slot;
    s32 index;

    for (slot = 0; slot < 6; slot += 2)
    {
        candidate = &gUnk_03000FD8->list[slot]->body;
        if ((candidate->mode & 6) == 4 && candidate != body)
        {
            if (candidate->posZ < ((s32)body->candidateLimit << 8))
            {
                count++;
                if (sub_081DE850(count, mt19937_next()) == 0)
                {
                    partner = candidate;
                }
            }
        }
        index = slot + 1;
        candidate = &gUnk_03000FD8->list[index]->body;
        if ((candidate->mode & 6) == 4 && candidate != body)
        {
            if (candidate->posZ < ((s32)body->candidateLimit << 8))
            {
                count++;
                if (sub_081DE850(count, mt19937_next()) == 0)
                {
                    partner = candidate;
                }
            }
        }
    }
    body->partner = partner;
    if (partner != 0)
    {
        place_targets(body, partner->targetX / 0x100, partner->targetY / 0x100, 0, -1);
    }
    else
    {
        place_targets(body, body->targetX / 0x100 + 0x64, body->targetY / 0x100, 0, -1);
    }
    set_motion_start(body, body->rate);
    body->rate += 0x4C;
    if (body->rate > 0x380)
    {
        body->rate = 0x380;
    }
    sprite_set_action(body, 0xA, 0, 0);
    ((struct SpriteMirrorBits *)&body->sprite->mirror)->flip = 0;
    body->timer = 10;
    body->state = partner_move_a;
}

void partner_move_a(struct HeroBody *body)
{
    struct HeroBody *hero_a;
    struct HeroBody *hero_b;
    struct HeroBody *partner;
    struct World *world = gUnk_03000FD8;
    s32 y;
    u16 saved;

    hero_a = &world->heroA->body;
    hero_b = &world->heroB->body;
    partner = body->partner;

    step_motion_d(body);
    if ((s16)--body->timer < 0)
    {
        effect_spawn_at(0x1CF0, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
        body->timer = 10;
    }
    y = body->sprite->screenY;
    if (y + body->edgeTop <= 0 || y + body->edgeBottom > 0xEF)
    {
        body->state = 0;
        ((struct WorldFlagBits *)&gUnk_03000FD8->flags)->moving = 0;
        return;
    }
    if (search_frame_pairs(body, partner) == 0)
    {
        return;
    }
    saved = body->unkC8;
    sprite_objects_obj_show_at(body, partner, (body->link << 1) + 6);
    body->unkC8 = saved;
    effect_spawn_at(0x1D00, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
    if (body->side == 0)
    {
        if ((hero_b->mode & 6) == 2)
        {
            body->side = 1;
        }
    }
    else
    {
        if ((hero_a->mode & 6) == 2)
        {
            body->side = 0;
        }
    }
    partner = (body->side == 0) ? hero_a : hero_b;
    body->partner = partner;
    if (partner != 0)
    {
        place_targets(body, partner->targetX / 0x100, partner->targetY / 0x100, 0, -1);
    }
    else
    {
        place_targets(body, body->targetX / 0x100 - 0x64, body->targetY / 0x100, 0, -1);
    }
    set_motion_start(body, body->rate);
    body->rate += 0x4C;
    if (body->rate > 0x380)
    {
        body->rate = 0x380;
    }
    ((struct SpriteMirrorBits *)&body->sprite->mirror)->flip = 1;
    body->timer = 10;
    body->state = move_until_offscreen;
}

void move_until_offscreen(struct HeroBody *body)
{
    s32 y;

    step_motion_d(body);
    if ((s16)--body->timer < 0)
    {
        effect_spawn_at(0x1CE7, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
        body->timer = 10;
    }
    y = body->sprite->screenY;
    if (y + body->edgeTop <= 0 || y + body->edgeBottom > 0xEF)
    {
        body->state = 0;
        ((struct WorldFlagBits *)&gUnk_03000FD8->flags)->moving = 0;
    }
}

void partner_wait_after_connect(struct HeroBody *body)
{
    body->state = visible_action_8;
}

s32 contact_check(struct HeroBody *body, struct HeroBody *other)
{
    s32 result;
    s16 rate;
    s32 speed;

    result = query_contact(body, other);
    if (result != 0)
    {
        return result;
    }
    if (other->data->slot == -1)
    {
        body->side = 0;
    }
    else
    {
        body->side = 1;
    }
    if (other->targetX < body->targetX)
    {
        body->state = pick_random_partner;
    }
    else
    {
        rate = body->rate;
        speed = rate + 0x4C;
        if (speed > 0x380)
            speed = 0x380;
        set_motion_start(body, rate + 0x4C);
    }
    body->link++;
    ((struct ActionModeBits *)&body->mode)->action = 0;
    return -2;
}

u32 partner_link_hook(struct HeroBody *body, u32 a, u32 b)
{
    partner_link_update(body, a, b);
    effect_spawn_at(0x1CF9, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
    return 0;
}

void visible_action_8(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 8, 0, 0);
        body->state = visible_action_9;
    }
}

void visible_action_9(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 9, 0, 0);
        ((struct SpriteModeBits *)&body->sprite->flags)->mode = 0;
        body->state = noop_after_visible_action_9;
    }
}

void noop_after_visible_action_9(struct HeroBody *body)
{
}

u32 pair_begin(struct HeroBody *body)
{
    u8 **party = &gUnk_03000FF4;
    struct HeroData *data = body->data;
    struct HeroBody *partner;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    void *anim;
    u8 *entry;

    entry = *party - (data->slot * 0x3C + 0x3C);
    data->slotValue = *(u16 *)(entry + 0x2A);
    body_a = &gUnk_03000FD8->heroA->body;
    body_b = &gUnk_03000FD8->heroB->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->resume = 0;
    body_b->resume = 0;
    body_a->resume = 0;
    partner->scale = 0x100;
    body_b->scale = 0x100;
    body_a->scale = 0x100;
    body->state = pair_place_targets;
    body_b->pair = 0;
    body_a->pair = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_0809583C/pair_place_targets.s", void pair_place_targets(struct HeroBody *body))

void hold_roll_handle(struct HeroBody *body)
{
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        body->handle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body->phase = 0;
        ((struct ActiveBits *)&body->flags79)->active = 0;
    }
    body->state = move_progress_check;
}

void move_progress_check(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gUnk_03000FD8->heroA->body;
    struct ActionEntry *entry;
    s32 action;

    state_stack_advance_progress(body);
    if (body->flags79 & 0x20)
    {
        entry = gUnk_03000FD8->curEntry;
        if (entry->type == 3)
        {
            hero_start_move(body);
        }
        else if (entry->kind == 2)
        {
            spawn_marker_child(body);
        }
        else
        {
            if ((body->mode & 6) == 2)
            {
                action = 0x2051;
                if (body == hero_a)
                {
                    action -= 0x2F;
                }
                sprite_set_action(body, 0, action, 0);
                ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
            }
            body->state = pair_choose_partner;
        }
    }
}

ASM_FUNC("asm/code_0809583C/pair_choose_partner.s", void pair_choose_partner(struct HeroBody *body))

void partner_chase_pair_step(struct HeroBody *body)
{
    struct HeroBody *child;
    struct HeroBody *partner;
    s32 i;
    s32 height;

    state_stack_advance_progress(body);
    if (body->flags79 & 0x20)
    {
        i = 0;
        child = body->child;
        if (child != NULL)
        {
            do
            {
                partner = child->partner;
                if (body == partner)
                {
                    if ((child->mode & 6) == 2 || (child->mode & 6) == 4)
                    {
                        place_targets(child, body->targetX / 0x100 + body->ofsX + 4, body->targetY / 0x100 + body->ofsY, body->targetZ / 0x100 + body->ofsZ - 8, -1);
                        if (body == (struct HeroBody *)((u8 *)body->data + 8))
                        {
                            height = body->ofsZ;
                        }
                        else
                        {
                            find_frame_entry(body, 0);
                            height = body->candidateLimit;
                        }
                        height += partner->targetZ / 0x100;
                        height += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x640, (height * height) / 0x40 + 0x28);
                        child->unk92 = height;
                        set_motion_offsets(child, 0x480);
                    }
                }
                else
                {
                    if ((child->mode & 6) == 2 || (child->mode & 6) == 4)
                    {
                        place_targets(child, partner->targetX / 0x100 + partner->ofsX + 4, partner->targetY / 0x100 + partner->ofsY, partner->targetZ / 0x100 + partner->ofsZ - 8, -1);
                        if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
                        {
                            height = partner->ofsZ;
                        }
                        else
                        {
                            find_frame_entry(partner, 0);
                            height = partner->candidateLimit;
                        }
                        height += partner->targetZ / 0x100;
                        height += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x640, (height * height) / 0x40 + 0x28);
                        child->unk92 = height;
                        set_motion_offsets(child, 0x480);
                    }
                }
                child->handle = MATH_ROUTINE(s32 (*)(s32, s32, s32), iwram_div)((s16)child->handle << 14, 0x4000 - i++ * (s16)child->handle * 18, 1);
                child->state = partner_step_wait;
                child = child->child;
            } while (child != NULL);
        }
        body->state = NULL;
    }
}

NONMATCH("asm/code_0809583C/partner_step_wait.s", void partner_step_wait(struct HeroBody *body))
{
    struct World **world = &gUnk_03000FD8;
    struct HeroBody *hero_a;
    struct HeroBody *partner;
    s32 action;
    s32 kind;

    hero_a = &(*world)->heroA->body;
    partner = body->partner;
    step_motion(body);
    if (body->flags79 & 0x20)
    {
        kind = (*world)->curEntry->kind;
        if (kind > 0xE)
        {
            set_slot_value(0x9F, -1);
        }
        else if (kind < 0xD)
        {
            set_slot_value(0x9F, -1);
        }
        else
        {
            set_slot_value(0x9F, -1);
        }
        obj_detach_active(body->sprite);
        if ((partner->data->input & 0x300) == 0 && (partner->mode & 6) == 2)
        {
            action = 0x2051;
            if (partner == hero_a)
            {
                action -= 0x2F;
            }
            sprite_set_action(partner, 8, action, 0);
            ((struct SpriteModeBits *)&partner->sprite->flags)->mode = 1;
        }
        body->state = state_partner_action8;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0809583C/state_partner_action8.s", void state_partner_action8(struct HeroBody *body))

void hero_start_move(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *partner = body->partner;
    s32 action;

    if ((body->mode & 6) == 2)
    {
        action = 0x2068;
        if (body == hero_a)
        {
            action -= 0x2F;
        }
        sprite_set_action(body, 3, action, 0);
        ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
    }
    ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
    place_targets(body, partner == hero_a ? 0x36 : 0x28, partner == hero_a ? 0x5C : 0x7C, 0, 0x100);
    set_step_and_offsets(body, 0x400, 0x28);
    set_slot_value(0x2B, -1);
    body->state = step_motion_arrive;
}

void step_motion_arrive(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *partner = body->partner;
    s32 action;

    if (step_motion(body) == 0)
    {
        if ((body->mode & 6) == 2)
        {
            action = 0x2052;
            if (body == hero_a)
            {
                action -= 0x2F;
            }
            sprite_set_action(body, 4, action, 0);
            ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
        }
        body->posX = partner->posX;
        body->posY = partner->posY;
        body->posZ = partner->posZ;
        body->step = 0x18;
        set_slot_value(0xAE, -1);
        body->state = pair_countdown_spawn;
    }
}

void child_visible_check(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gUnk_03000FD8->heroA->body;
    s32 action;

    if (body->child->sprite->flags & 8)
    {
        set_slot_value(0x9F, -1);
        if ((body->mode & 6) == 2)
        {
            action = 0x2052;
            if (body == hero_a)
            {
                action -= 0x2F;
            }
            sprite_set_action(body, 5, action, 0);
            ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
        }
        sprite_set_action(body->partner, 1, body == hero_a ? 0x2023 + 0x2F : 0x2023, 0);
        ((struct SpriteModeBits *)&body->partner->sprite->flags)->mode = 1;
        sprite_set_action(body->child, 1, 0x208C, 0);
        body->child->state = child_launch;
        body->state = pose_switch;
    }
}

void pose_switch(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gUnk_03000FD8->heroA->body;
    s32 action;

    if (body->sprite->flags & 8)
    {
        set_slot_value(0x2B, -1);
        if ((body->mode & 6) == 2)
        {
            action = 0x2068;
            if (body == hero_a)
            {
                action -= 0x2F;
            }
            sprite_set_action(body, 6, action, 0);
            ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
        }
        body->posX = (body == hero_a) ? 0x2800 : 0x3600;
        body->targetX = body->posX;
        body->posY = (body == hero_a) ? 0x7C00 : 0x5C00;
        body->targetY = body->posY;
        place_targets(body, (body == hero_a) ? 0x36 : 0x28, (body == hero_a) ? 0x5C : 0x7C, 0, 0x100);
        set_step_and_offsets(body, 0x400, 0x28);
        body->state = wait_motion_end;
    }
}

void child_launch(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *hero_a = &world->heroA->body;
    struct HeroBody *partner = body->partner;
    s32 action;
    s32 index;
    const u16 *effects;
    u8 *table;
    u16 effect;

    if (body->sprite->flags & 8)
    {
        effects = gEffectCodeTable;
        index = ~world->parts->slot;
        table = (u8 *)world + 0x34E;
        effect = effects[table[index]];
        effect_spawn_at(effect, partner->posX / 0x100, partner->posY / 0x100 + 2, partner->posZ / 0x100 - 2, partner);
        action = 0x2052;
        if (partner == hero_a)
        {
            action -= 0x2F;
        }
        sprite_set_action(partner, 2, action, 0);
        ((struct SpriteModeBits *)&partner->sprite->flags)->mode = 1;
        obj_release(body);
        partner->step = 0x18;
        partner->state = menu_cursor_follow;
    }
}

s32 menu_cursor_pos(s32 x, s32 y, u32 n);
ASM_FUNC("asm/code_0809583C/menu_cursor_follow.s", void menu_cursor_follow(struct HeroBody *body))

ASM_FUNC("asm/code_0809583C/pair_partner_swap.s", void pair_partner_swap(struct HeroBody *body))
