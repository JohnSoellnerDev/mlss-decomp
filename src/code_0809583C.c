#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    s16 screenY;
    u8 filler2[0x11 - 0x02];
    u8 mirror;
    u8 flags;
};

#define HERO_SPRITE_ANIM_DONE 8 // HeroSprite.flags: the current animation has finished

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
    s16 cursorX;  // HP
    s16 cursorY;  // max HP
    u8 filler0FA[0xFC - 0xFA];
    s16 cursorX2; // SP
    s16 cursorY2; // max SP
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

// The item being used in battle.
struct ActionEntry
{
    u8 type;   // ITEM_TYPE_*
    u8 filler1;
    u8 kind;
    u8 filler3[0x05 - 0x03];
    u8 switchByte;
    u8 filler6[0x08 - 0x06];
    u16 menuKind; // amount: points, or one of AMOUNT_* (see menu_cursor_pos)
};

// ActionEntry.type
#define ITEM_TYPE_HP      1  // restores HP
#define ITEM_TYPE_CURE    2  // cures every status ailment
#define ITEM_TYPE_SP      11 // restores SP
#define ITEM_TYPE_HP_SP   12 // restores both
#define ITEM_TYPE_BOOST_A 13 // raises a stat for a while (anim_set_motion_a)
#define ITEM_TYPE_BOOST_B 14 // raises a stat for a while (anim_set_motion_b)

// menu_cursor_pos(points, max, amount) adds amount to points, at most max, or for these amounts
// sets them to a part of max (rounded up).
#define AMOUNT_FULL    0xFFFF
#define AMOUNT_HALF    0xFFFE
#define AMOUNT_QUARTER 0xFFFD

// HeroData.slot of the brothers: ~ their index (in World.list and the byte table at World + 0x34E)
#define SLOT_HERO_A -1
#define SLOT_HERO_B -2

// HeroBody.mode bits 1-2
#define HERO_MODE_MASK     6
#define HERO_MODE_ON_FIELD (1 << 1) // standing in battle, can be targeted

// HeroData.input: statuses that keep a brother from reacting
#define HERO_CANT_ACT 0x300

// sprite_set_action animation sets of the brothers
#define ANIM_SET_HERO_A 0x2022
#define ANIM_SET_HERO_B 0x2051

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

struct Flags77Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 hi1 : 1;
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
    u8 filler2E0[0x34E - 0x2E0];
    u8 heroEffect[2]; // per hero (~HeroData.slot): index into gEffectCodeTable
};

extern struct World *gWorld;
extern u8 *gParty;
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
void release_partner_when_visible(struct HeroBody *body);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void anim_stop_mode(struct HeroData *data);
void anim_bit100_release(struct HeroData *data);
void anim_end_mode(struct HeroData *data);
void anim_bit8_scale10a(struct HeroData *data);
void anim_bit20_scale10c(struct HeroData *data);
void anim_bit80_scale11c(struct HeroData *data);
void anim_clear_high_bits(struct HeroData *data);
s32 anim_set_motion_a(struct HeroData *data, u16 value, s32 extra);
s32 anim_set_motion_b(struct HeroData *data, u16 value, s32 extra);
s32 anim_bit400_step(struct HeroData *data, s32 value);
s32 anim_chance_start(struct HeroData *data, s32 value);
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
u32 rand_below(u32 range, u32 seed);
s32 contact_check(struct HeroBody *body, struct HeroBody *other);
u32 partner_link_hook(struct HeroBody *body, u32 a, u32 b);

void pair_children_end(struct HeroBody *body);

void visible_action_2000(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0x2000, 0);
        body->state = 0;
    }
}

/* Links the body to the partner that touched it (lookup_part sets body->partner first): flags the
 * world as moving, runs the body's leave callback (keeping its partner and anim), puts it back on the
 * object list with the partner's contact handlers, resets its jump, rate and side, and waits in
 * partner_wait_after_connect. Returns 1. */
u32 actor_init_linked(struct HeroBody *body)
{
    struct HeroBody *partner;
    void *anim;

    ((struct WorldFlagBits *)&gWorld->flags)->moving = 1;
    body->link = 0;
    partner = body->partner;
    anim = body->anim;
    if (body->callback != 0)
    {
        body->callback(body);
    }
    body->partner = partner;
    body->anim = anim;
    obj_list_push_default(body);
    ((struct Flags77Bits *)&body->flags77)->bit5 = 0;
    ((struct Flags77Bits *)&body->flags77)->bit6 = 0;
    body->eventA = partner_link_hook;
    body->eventB = contact_check;
    body->eventC = contact_check;
    body->eventD = contact_check;
    body->jumpInit = 0x1AA8;
    body->rate = 0x200;
    body->side = 0;
    /* the test only reads the partner's slot; both arms store the same value */
    if (partner->data->slot == -1)
    {
        partner->dh = 0x300;
    }
    else
    {
        partner->dh = 0x300;
    }
    body->resume = NULL;
    body->state = partner_wait_after_connect;
    body->scale = 0x100;
    return 1;
}

// Picks a random partner among the world's six bodies: one in mode 4, not this body, and lower
// than its candidateLimit (in px), each with the same chance (reservoir sampling). Heads to the
// partner's target, or 100 px right of its own target when there is none, at the current rate,
// which then grows by 0x4C up to 0x380. Starts action 0xA and waits 10 frames in partner_move_a.
void pick_random_partner(struct HeroBody *body)
{
    struct HeroBody *partner = 0;
    struct HeroBody *candidate;
    s32 count = 0;
    s32 slot;
    s32 next;

    /* two list slots per pass */
    for (slot = 0; slot < 6; slot += 2)
    {
        candidate = &gWorld->list[slot]->body;
        if ((candidate->mode & 6) == 4 && candidate != body)
        {
            if (candidate->posZ < ((s32)body->candidateLimit << 8))
            {
                /* keep the count-th candidate with chance 1/count */
                count++;
                if (rand_below(count, mt19937_next()) == 0)
                {
                    partner = candidate;
                }
            }
        }
        next = slot + 1;
        candidate = &gWorld->list[next]->body;
        if ((candidate->mode & 6) == 4 && candidate != body)
        {
            if (candidate->posZ < ((s32)body->candidateLimit << 8))
            {
                count++;
                if (rand_below(count, mt19937_next()) == 0)
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
        place_targets(body, body->targetX / 0x100 + 100, body->targetY / 0x100, 0, -1);
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

// Flies toward the partner chosen by pick_random_partner, leaving trail effect 0x1CF0
// every 10 frames, and stops once it leaves the screen vertically. When it hits the
// partner it shows the hit (effect 0x1D00) and bounces back toward hero A or B (switching
// to the other hero when that one is in mode 2), 0x4C faster (up to 0x380) and mirrored,
// then keeps flying in move_until_offscreen.
void partner_move_a(struct HeroBody *body)
{
    struct HeroBody *heroA;
    struct HeroBody *heroB;
    struct HeroBody *partner;
    struct World *world = gWorld;
    s32 screenY;
    u16 savedC8;

    heroA = &world->heroA->body;
    heroB = &world->heroB->body;
    partner = body->partner;

    step_motion_d(body);
    if ((s16)--body->timer < 0)
    {
        effect_spawn_at(0x1CF0, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
        body->timer = 10;
    }
    screenY = body->sprite->screenY;
    if (screenY + body->edgeTop <= 0 || screenY + body->edgeBottom > 0xEF)
    {
        body->state = 0;
        ((struct WorldFlagBits *)&gWorld->flags)->moving = 0;
        return;
    }
    if (search_frame_pairs(body, partner) == 0)
    {
        return;
    }
    savedC8 = body->unkC8;
    sprite_objects_obj_show_at(body, partner, (body->link << 1) + 6);
    body->unkC8 = savedC8;
    effect_spawn_at(0x1D00, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
    if (body->side == 0)
    {
        if ((heroB->mode & 6) == 2)
        {
            body->side = 1;
        }
    }
    else
    {
        if ((heroA->mode & 6) == 2)
        {
            body->side = 0;
        }
    }
    partner = (body->side == 0) ? heroA : heroB;
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
        ((struct WorldFlagBits *)&gWorld->flags)->moving = 0;
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
    u8 **party = &gParty;
    struct HeroData *data = body->data;
    struct HeroBody *partner;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    void *anim;
    u8 *entry;

    entry = *party - (data->slot * 0x3C + 0x3C);
    data->slotValue = *(u16 *)(entry + 0x2A);
    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
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

NONMATCH("asm/code_0809583C/pair_place_targets.s", void pair_place_targets(struct HeroBody *body))
{
    s32 mask;
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    s16 *span;
    u8 *mode_a;
    s32 mode;

    mask = 6;
    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    body_b = &gWorld->heroB->body;
    mode_a = &hero_a->body.mode;
    if ((*mode_a & 6) == 2)
    {
        place_targets(body_a, body_a->data->targetX / 0x100, body_a->data->targetY / 0x100, body_a->data->targetH / 0x100, -1);
        set_motion_start(body_a, 0x200);
        mode = *mode_a & 6;
        if (mode == 2 || mode == 4)
        {
            if (body_a->data->slot == -1)
            {
                sprite_set_action(body_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_a, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorBits *)&body_a->sprite->mirror)->flip = (span = &body_a->span.a)[0] >= span[1];
            ((struct SpriteModeBits *)&body_a->sprite->flags)->mode = 0;
        }
        mode = *mode_a & mask;
        if (mode == 2 || mode == 4)
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
    }
    if ((body_b->mode & 6) == 2)
    {
        place_targets(body_b, body_b->data->targetX / 0x100, body_b->data->targetY / 0x100, body_b->data->targetH / 0x100, -1);
        set_motion_start(body_b, 0x200);
        mode = body_b->mode & 6;
        if (mode == 2 || mode == 4)
        {
            if (body_b->data->slot == -1)
            {
                sprite_set_action(body_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_b, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorBits *)&body_b->sprite->mirror)->flip = (span = &body_b->span.a)[0] >= span[1];
            ((struct SpriteModeBits *)&body_b->sprite->flags)->mode = 0;
        }
        mode = body_b->mode & 6;
        if (mode == 2 || mode == 4)
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
    }
    body->resume = hold_roll_handle;
}
END_NONMATCH

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
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct ActionEntry *entry;
    s32 action;

    state_stack_advance_progress(body);
    if (body->flags79 & 0x20)
    {
        entry = gWorld->curEntry;
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

NONMATCH("asm/code_0809583C/pair_choose_partner.s", void pair_choose_partner(struct HeroBody *body))
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *hero_b = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;
    struct HeroBody *child;
    u8 *mode;
    s32 count;
    s32 action;
    s32 kind;

    if (body->sprite->flags & 8)
    {
        count = 0;
        mode = &body->mode;
        for (; count < 2; count++)
        {
            child = obj_spawn_linked(body, 0x4188 + gWorld->curEntry->kind, 0);
            child->flags78 = (child->flags78 & 0x1F) | 0x20;
            child->posX += 0x1000;
            child->targetX = child->posX;
            child->targetY = child->posY;
            child->posZ += 0x1000;
            child->targetZ = child->posZ;
            if (gWorld->curEntry->switchByte == 0)
            {
                break;
            }
            partner = (partner == hero_a) ? hero_b : hero_a;
            body->partner = partner;
            if ((partner->mode & 6) != 2)
            {
                break;
            }
        }

        kind = gWorld->curEntry->kind;
        switch (kind)
        {
        case 0xD:
        case 0xE:
            set_slot_value(0x128, -1);
            break;
        default:
            set_slot_value(0x128, -1);
            break;
        }

        if (gWorld->curEntry->switchByte != 0)
        {
            if ((*mode & 6) == 2)
            {
                action = 0x2051;
                if (body == hero_a)
                {
                    action -= 0x2F;
                }
                sprite_set_action(body, 2, action, 0);
                ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
            }
            partner = (body == hero_a) ? hero_b : hero_a;
        }
        else if (body == partner)
        {
            if ((*mode & 6) == 2)
            {
                action = 0x2051;
                if (body == hero_a)
                {
                    action -= 0x2F;
                }
                sprite_set_action(body, 2, action, 0);
                ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
            }
        }
        else
        {
            if ((*mode & 6) == 2)
            {
                action = 0x2051;
                if (body == hero_a)
                {
                    action -= 0x2F;
                }
                sprite_set_action(body, 5, action, 0);
                ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
            }
        }

        if ((partner->data->input & 0x300) == 0 && (partner->mode & 6) == 2)
        {
            action = 0x2051;
            if (partner == hero_a)
            {
                action -= 0x2F;
            }
            sprite_set_action(partner, 9, action, 0);
            ((struct SpriteModeBits *)&partner->sprite->flags)->mode = 1;
        }

        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            body->handle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
            body->phase = 0;
            ((struct ActiveBits *)&body->flags79)->active = 0;
        }
        body->state = partner_chase_pair_step;
    }
}
END_NONMATCH

/*
 * Advances the body's state stack; when its step ends (flag 0x20), sends every child in its
 * chain jumping onto the child's partner (or onto this body when that is the partner): a
 * target 4 right and 8 below the partner's target plus its offset, an arc height from the
 * partner's height (its Z offset for a hero's own body, else its frame box height) and its
 * target Z. Each later child in the chain gets a larger handle value, then waits in
 * partner_step_wait.
 */
void partner_chase_pair_step(struct HeroBody *body)
{
    struct HeroBody *child;
    struct HeroBody *partner;
    s32 order;
    s32 height;

    state_stack_advance_progress(body);
    if (body->flags79 & 0x20)
    {
        order = 0;
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
                        /* a hero's own body (embedded at Hero + 8) uses its Z offset */
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
                /* handle / (1 - order * handle * 18 / 0x4000), 1.14 fixed point */
                child->handle = MATH_ROUTINE(s32 (*)(s32, s32, s32), iwram_div)((s16)child->handle << 14, 0x4000 - order++ * (s16)child->handle * 18, 1);
                child->state = partner_step_wait;
                child = child->child;
            } while (child != NULL);
        }
        body->state = NULL;
    }
}

/* Moves the used item (body) on towards the brother it is for; when it arrives it plays sound
   0x9F, its sprite is detached, the target (if he can react and is on the field) plays his
   catch action 8, and the item goes on to take effect in state_partner_action8. */
void partner_step_wait(struct HeroBody *body)
{
    struct World **world = &gWorld;
    struct HeroBody *hero_a;
    struct HeroBody *target;
    s32 action;
    s32 kind;

    hero_a = &(*world)->heroA->body;
    target = body->partner;
    step_motion(body);
    if (body->flags79 & 0x20)
    {
        kind = (*world)->curEntry->kind;
        switch (kind)
        {
        case 0xD:
        case 0xE:
            set_slot_value(0x9F, -1);
            break;
        default:
            set_slot_value(0x9F, -1);
            break;
        }
        obj_detach_active(body->sprite);
        if ((target->data->input & HERO_CANT_ACT) == 0 && (target->mode & HERO_MODE_MASK) == HERO_MODE_ON_FIELD)
        {
            action = ANIM_SET_HERO_B;
            if (target == hero_a)
            {
                action -= ANIM_SET_HERO_B - ANIM_SET_HERO_A;
            }
            sprite_set_action(target, 8, action, 0);
            ((struct SpriteModeBits *)&target->sprite->flags)->mode = 1;
        }
        body->state = state_partner_action8;
    }
}

/* Once the used item (body) reaches the brother it is for: the user stops his throwing pose, the
   target cheers (if he can), the item's effect appears on him and the item takes effect: HP or
   SP restored with the points popping up, ailments cured, or a stat raised for 200 frames. */
void state_partner_action8(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *target = body->partner;
    struct HeroBody *user;
    s32 action;
    s32 points;
    u16 effect;
    const u16 *effects;
    u8 *table;
    s32 index;

    if (target->sprite->flags & 8)
    {
        user = &((struct Hero *)body->data)->body;
        if (((struct ActionModeBits *)&user->mode)->mode == 1 ||
            ((struct ActionModeBits *)&user->mode)->mode == 2)
        {
            sprite_set_action(user, -1, -1, 0);
            ((struct SpriteModeBits *)&user->sprite->flags)->mode = 0;
        }

        if ((target->data->input & 0x300) == 0)
        {
            switch (gWorld->curEntry->type)
            {
            case ITEM_TYPE_HP:
            case ITEM_TYPE_SP:
            case ITEM_TYPE_HP_SP:
                if (target->data->slot == SLOT_HERO_A)
                {
                    set_slot_value(0xEB, -1);
                }
                if (target->data->slot == SLOT_HERO_B)
                {
                    set_slot_value(0xFD, -1);
                }
                if (((struct ActionModeBits *)&target->mode)->mode == 1)
                {
                    // the same action in each brother's animation set
                    action = 0x2051;
                    if (target == hero_a)
                    {
                        action -= 0x2F;
                    }
                    sprite_set_action(target, 0xB, action, 0);
                    ((struct SpriteModeBits *)&target->sprite->flags)->mode = 1;
                }
                break;
            case ITEM_TYPE_CURE:
                if (target->data->slot == SLOT_HERO_A)
                {
                    set_slot_value(0xEB, -1);
                }
                if (target->data->slot == SLOT_HERO_B)
                {
                    set_slot_value(0xFD, -1);
                }
                break;
            case ITEM_TYPE_BOOST_A:
            case ITEM_TYPE_BOOST_B:
                set_slot_value(0x9C, -1);
                if (((struct ActionModeBits *)&target->mode)->mode == 1)
                {
                    action = 0x2051;
                    if (target == hero_a)
                    {
                        action -= 0x2F;
                    }
                    sprite_set_action(target, 0xA, action, 0);
                    ((struct SpriteModeBits *)&target->sprite->flags)->mode = 1;
                }
                break;
            default:
                break;
            }
        }

        effects = gEffectCodeTable;
        index = ~gWorld->parts->slot;
        table = (u8 *)gWorld + 0x34E;
        effect = effects[table[index]];
        effect_spawn_at(effect, target->posX / 0x100, target->posY / 0x100 + 2,
                        target->posZ / 0x100 - 2, target);

        switch (gWorld->curEntry->type)
        {
        case ITEM_TYPE_HP:
            points = menu_cursor_pos(target->data->cursorX, target->data->cursorY,
                                     gWorld->curEntry->menuKind);
            popup_digit_spawn(points - target->data->cursorX, target->targetX / 0x100,
                              target->targetY / 0x100, (s8)target->ofsZ, 1);
            target->data->cursorX = points;
            break;
        case ITEM_TYPE_CURE:
            anim_stop_mode(target->data);
            anim_bit100_release(target->data);
            anim_end_mode(target->data);
            anim_bit8_scale10a(target->data);
            anim_bit20_scale10c(target->data);
            anim_bit80_scale11c(target->data);
            anim_clear_high_bits(target->data);
            break;
        case ITEM_TYPE_SP:
            points = menu_cursor_pos(target->data->cursorX2, target->data->cursorY2,
                                     gWorld->curEntry->menuKind);
            popup_digit_spawn(points - target->data->cursorX2, target->targetX / 0x100,
                              target->targetY / 0x100, (s8)target->ofsZ, 1);
            target->data->cursorX2 = points;
            break;
        case ITEM_TYPE_HP_SP:
            points = menu_cursor_pos(target->data->cursorX2, target->data->cursorY2,
                                     gWorld->curEntry->menuKind);
            popup_digit_spawn(points - target->data->cursorX2, target->targetX / 0x100 - 0xC,
                              target->targetY / 0x100 - 4, (s8)target->ofsZ, 1);
            target->data->cursorX2 = points;
            points = menu_cursor_pos(target->data->cursorX, target->data->cursorY,
                                     gWorld->curEntry->menuKind);
            popup_digit_spawn(points - target->data->cursorX, target->targetX / 0x100 + 0xC,
                              target->targetY / 0x100 + 4, (s8)target->ofsZ, 1);
            target->data->cursorX = points;
            break;
        case ITEM_TYPE_BOOST_A:
            effect_spawn_at(0xCB, target->posX / 0x100, target->posY / 0x100 + 2,
                            target->posZ / 0x100 - 2, target);
            anim_set_motion_a(target->data, (u16)(gWorld->curEntry->menuKind + 0x100), 200);
            anim_bit400_step(target->data, 200);
            break;
        case ITEM_TYPE_BOOST_B:
            effect_spawn_at(0xCB, target->posX / 0x100, target->posY / 0x100 + 2,
                            target->posZ / 0x100 - 2, target);
            anim_set_motion_b(target->data, (u16)(gWorld->curEntry->menuKind + 0x100), 200);
            anim_chance_start(target->data, 200);
            break;
        default:
            obj_release(body);
            break;
        }
        body->state = release_partner_when_visible;
    }
}

/* Starts a hero's jump over to its partner's side: re-poses it (pose 3) if it is on the field, jumps
   it (speed 0x400, 0x28 px high) to (0x36, 0x5C) px when the partner is hero A, else to (0x28, 0x7C),
   plays the jump sound 0x2B and waits for the landing in step_motion_arrive. */
void hero_start_move(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *partner = body->partner;
    s32 action;

    if ((body->mode & HERO_MODE_MASK) == HERO_MODE_ON_FIELD)
    {
        // the same action in each brother's animation set
        action = ANIM_SET_HERO_B + 0x17;
        if (body == hero_a)
        {
            action -= ANIM_SET_HERO_B - ANIM_SET_HERO_A;
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
    struct HeroBody *hero_a = &gWorld->heroA->body;
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

/* Once the item's (child's) appearing animation has finished: plays sound 0x9F, gives the user
   (body) its pose 5 if it is on the field, the brother it is for (partner) its pose 1 and the item
   its flight animation, then the item flies to him (child_launch) and the user goes on to
   pose_switch. */
void child_visible_check(struct HeroBody *user)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    s32 action;

    if (user->child->sprite->flags & HERO_SPRITE_ANIM_DONE)
    {
        set_slot_value(0x9F, -1);
        if ((user->mode & HERO_MODE_MASK) == HERO_MODE_ON_FIELD)
        {
            // the same action in each brother's animation set
            action = ANIM_SET_HERO_B + 1;
            if (user == hero_a)
            {
                action -= ANIM_SET_HERO_B - ANIM_SET_HERO_A;
            }
            sprite_set_action(user, 5, action, 0);
            ((struct SpriteModeBits *)&user->sprite->flags)->mode = 1;
        }
        // the partner is the other brother: it uses the other animation set
        sprite_set_action(user->partner, 1, user == hero_a ? ANIM_SET_HERO_B + 1 : ANIM_SET_HERO_A + 1, 0);
        ((struct SpriteModeBits *)&user->partner->sprite->flags)->mode = 1;
        sprite_set_action(user->child, 1, 0x208C, 0);
        user->child->state = child_launch;
        user->state = pose_switch;
    }
}

/* Once the hero's animation ends: plays sound 0x2B, re-poses it (pose 6) if it is in mode 2, and
   swaps the two heroes' spots: hero A starts at (0x28, 0x7C) px and moves to (0x36, 0x5C), the other
   hero the other way round. Then it waits for the move to end (wait_motion_end). */
void pose_switch(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
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

/* State of the item object thrown at a hero (set by child_visible_check): once its animation
   ends, plays the acting hero's effect on the target, poses the target (pose 2), removes the
   item and has the target wait 0x18 frames before the item takes effect (menu_cursor_follow). */
void child_launch(struct HeroBody *item)
{
    struct World *world = gWorld;
    struct HeroBody *hero_a = &world->heroA->body;
    struct HeroBody *target = item->partner;
    s32 action;
    s32 heroIndex;
    const u16 *effects;
    u8 *heroEffect;
    u16 effect;

    if (item->sprite->flags & 8)
    {
        effects = gEffectCodeTable;
        heroIndex = ~world->parts->slot;
        heroEffect = world->heroEffect;
        effect = effects[heroEffect[heroIndex]];
        effect_spawn_at(effect, target->posX / 0x100, target->posY / 0x100 + 2, target->posZ / 0x100 - 2, target);
        action = 0x2052;
        if (target == hero_a)
        {
            action -= 0x2F;
        }
        sprite_set_action(target, 2, action, 0);
        ((struct SpriteModeBits *)&target->sprite->flags)->mode = 1;
        obj_release(item);
        target->step = 0x18;
        target->state = menu_cursor_follow;
    }
}

s32 menu_cursor_pos(s32 x, s32 y, u32 n);
// After the hero's wait (step) runs out: applies the item in use to its HP (with AMOUNT_FULL, to SP as
// well), puts its battle part back in its slot (keeping its home spot and partBits bit 7), plays
// sound 0x2B and sends the hero back to its place in the line-up (land_at_target_height).
void menu_cursor_follow(struct HeroBody *body)
{
    s16 *points;
    struct HeroBody *hero_a;
    struct HeroData *data;
    struct ActionEntry *item;
    s32 savedBit7;
    s32 homeX;
    s32 homeY;
    s32 homeH;
    s32 placeX;
    s32 placeY;

    hero_a = &gWorld->heroA->body;
    if (body->step > 0)
    {
        body->step--;
        return;
    }
    data = body->data;
    item = gWorld->curEntry;
    if (item->menuKind == AMOUNT_FULL)
    {
        points = &data->cursorX;
        *points = menu_cursor_pos(*points, data->cursorY, item->menuKind);
        points = &data->cursorX2;
        *points = menu_cursor_pos(*points, data->cursorY2, gWorld->curEntry->menuKind);
    }
    else
    {
        points = &data->cursorX;
        *points = menu_cursor_pos(*points, data->cursorY, item->menuKind);
    }

    // place_part_in_slot resets these, so keep them across the call.
    savedBit7 = data->partBits >> 7;
    homeX = data->targetX;
    homeY = data->targetY;
    homeH = data->targetH;
    place_part_in_slot(data->effect, data->slot, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, 0, 1);
    data->targetX = homeX;
    data->targetY = homeY;
    data->targetH = homeH;
    ((struct PartBitsFlags *)&data->partBits)->bit7 = savedBit7;

    ((struct WorldInputBits *)&gWorld->input)->lo2 = 0;
    set_slot_value(0x2B, -1);
    sprite_set_action(body, 5, -1, 0);
    ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
    placeX = (body == hero_a) ? 0x36 : 0x28;
    placeY = (body == hero_a) ? 0x5C : 0x7C;
    place_targets(body, placeX, placeY, 0, 0x100);
    set_step_and_offsets(body, 0x300, 0x20);
    body->state = land_at_target_height;
}

NONMATCH("asm/code_0809583C/pair_partner_swap.s", void pair_partner_swap(struct HeroBody *body))
{
    struct World *world = gWorld;
    struct HeroBody *hero_a = &world->heroA->body;
    struct HeroBody *hero_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    struct HeroBody *child;
    struct HeroBody *list;
    s32 action;
    s32 step;

    if (body->sprite->flags & 8)
    {
        step = 0x1000;
        child = obj_spawn_linked(body, 0, 0);
        child->flags78 = (child->flags78 & 0x1F) | 0x20;
        child->posX += step;
        child->targetX = child->posX;
        child->targetY = child->posY;
        child->posZ += step;
        child->targetZ = child->posZ;
        if (gWorld->curEntry->switchByte != 0)
        {
            partner = (partner == hero_a) ? hero_b : hero_a;
            body->partner = partner;
            if ((partner->mode & 6) == 2)
            {
                struct HeroBody *child;

                child = obj_spawn_linked(body, 0, 0);
                child->flags78 = (child->flags78 & 0x1F) | 0x20;
                child->posX += step;
                child->targetX = child->posX;
                child->targetY = child->posY;
                child->posZ += step;
                child->targetZ = child->posZ;
                if (gWorld->curEntry->switchByte != 0)
                {
                    partner = (partner == hero_a) ? hero_b : hero_a;
                    body->partner = partner;
                    (void)(partner->mode & 6);
                }
            }
        }

        list = body->child;
        while (list->child != NULL)
        {
            list = list->child;
        }
        if (body == partner)
        {
            if ((body->mode & 6) == 2)
            {
                action = 0x2051;
                if (body == hero_a)
                {
                    action -= 0x2F;
                }
                sprite_set_action(body, 3, action, 0);
                ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
            }
            sprite_set_action(list, 3, 0, 0);
            if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
            {
                body->handle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x18);
                body->phase = 0;
                ((struct ActiveBits *)&body->flags79)->active = 0;
            }
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
                sprite_set_action(body, 4, action, 0);
                ((struct SpriteModeBits *)&body->sprite->flags)->mode = 1;
            }
            if ((partner->data->input & 0x300) == 0 && (partner->mode & 6) == 2)
            {
                action = 0x2051;
                if (partner == hero_a)
                {
                    action -= 0x2F;
                }
                sprite_set_action(partner, 9, action, 0);
                ((struct SpriteModeBits *)&partner->sprite->flags)->mode = 1;
            }
            sprite_set_action(list, 4, 0, 0);
            if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
            {
                body->handle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
                body->phase = 0;
                ((struct ActiveBits *)&body->flags79)->active = 0;
            }
        }
        body->state = pair_children_end;
    }
}
END_NONMATCH
