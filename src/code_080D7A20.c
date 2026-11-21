#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
};

#define HERO_SPRITE_ANIM_DONE 8 // HeroSprite.flags: the current animation has finished

// HeroBody.mode: the body is shown and can be animated.
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

struct HeroLayer
{
    struct HeroLayer *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 flip : 1;
    u8 unk7 : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 high : 1;
};

struct HeroSpan
{
    s16 a;
    s16 b;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 displayTimer;
    u8 filler0F8[0x110 - 0xF8];
    u8 flags110;
    u8 flags111;
    u8 filler112[0x114 - 0x112];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
    s16 prevProgress;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler0C5[0xCA - 0xC5];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x0C - 0x0B];
    u8 view;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroLayer *layers;
};

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;
extern const u8 gRoom149Layer1[];

void hero_b_start_step_14th(struct HeroBody *body);
void hero_b_active_partner_anim_place(struct HeroBody *body);
void hero_b_step_window_d72e4(struct HeroBody *body);
void hero_b_link_list_objs(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void obj_release(struct HeroBody *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void state_stack_advance_progress(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 value);
void hero_b_sprite_idle_start_step10(struct HeroBody *body);
void hero_b_link_partner_child_fx(struct HeroBody *body);
void hero_pair_active_place_both(struct HeroBody *body);
void hero_b_step_d8164(struct HeroBody *body);
void hero_b_copy_pos(struct HeroBody *body);
void hero_b_active_anim_place(struct HeroBody *body);
void hero_pair_spawn_fx_anim_next(struct HeroBody *body);
void hero_b_active_place_at_partner(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_sprite_active_start_step16(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void hero_pair_active_place_a_finish(struct HeroBody *body);

/* Advances hero B's state-stack progress. When its motion ends: plays sound slot 0x11F, hero B (if on
 * the field) and its child take action 0xE, and hero B walks to 10 px left of the partner's target
 * point (plus the partner's offset) at speed 0x300; then the child-carrying step follows. */
void hero_b_active_place_at_partner(struct HeroBody *body)
{
    struct HeroBody *heroB = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(heroB);
    if ((heroB->flags & 0x20) != 0)
    {
        set_slot_value(0x11F, -1);
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((heroB->mode & 6) == 2 || (heroB->mode & 6) == 4)
        {
            sprite_set_action(heroB, 0xE, 0x2066, 0);
            ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 0;
        }
        if (heroB->child != 0)
        {
            sprite_set_action(heroB->child, 0xE, 0x2095, 0);
            ((struct HeroSpriteBits *)&heroB->child->sprite->flags)->mode = 0;
        }
        if ((heroB->mode & 6) == 2 || (heroB->mode & 6) == 4)
        {
            place_targets(heroB, partner->targetX / 0x100 + partner->ofsX - 10,
                          partner->targetY / 0x100 + partner->ofsY, 0, -1);
            set_motion_start(heroB, 0x300);
        }
        body->state = hero_b_link_partner_child_fx;
    }
}

// State step for hero B carrying its child object: the child follows hero B while it moves. When the
// motion ends it releases the child, resolves the contact with the partner, spawns effect 0x6C4 on
// the partner if it is still showing (or flag bit 1 of +0x111), and both switch to action 0xF.
// The contact flags word is a ROM constant the symbols take for gRoom149Layer1 + 0x40D.
void hero_b_link_partner_child_fx(struct HeroBody *body)
{
    struct HeroBody *heroB = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;

    step_motion(heroB);
    heroB->child->posX = heroB->posX;
    heroB->child->posY = heroB->posY;
    heroB->child->posZ = heroB->posZ;
    if ((heroB->flags & 0x20) != 0)
    {
        set_slot_value(0x55, -1);
        obj_release(heroB->child);
        ((struct HeroCcBits *)&heroB->cc)->low = 0;
        heroB->scale = 0xE6;
        resolve_contact_action(heroB, partner, 0, 0, gRoom149Layer1 + 0x40D);
        heroB->count++;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            x = partner->targetX / 0x100 + partner->ofsX;
            y = partner->targetY / 0x100 + partner->ofsY;
            effect_spawn_at(0x6C4, x, y, 0, partner);
        }
        if ((heroB->mode & 6) == 2 || (heroB->mode & 6) == 4)
        {
            sprite_set_action(heroB, 0xF, 0x2066, 0);
            ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 1;
        }
        if (heroB->child != 0)
        {
            sprite_set_action(heroB->child, 0xF, 0x2095, 0);
            ((struct HeroSpriteBits *)&heroB->child->sprite->flags)->mode = 0;
        }
        body->state = hero_b_sprite_active_start_step16;
    }
}

void hero_b_sprite_active_start_step16(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_pair_active_place_a_finish;
    }
}

/* Advances hero B's state-stack progress. When its motion ends: plays sound slot 0x53, and each
   hero in turn (A, then B) walks back to its home spot (its data's target, in 1/256 pixels), takes
   its idle action facing along its span, and clears its state to enter the finish rebuild. */
void hero_pair_active_place_a_finish(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            data = body_a->data;
            place_targets(body_a, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_a->sprite;
                mirror = 0;
                span = &body_a->span.a;
                if (span[0] >= span[1])
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_b->sprite;
                mirror = 0;
                span = &body_b->span.a;
                if (span[0] >= span[1])
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
    }
}


/* Pair-move step: spawns effect 0x2FE6 at body's position (in pixels), hero A (if on the field)
   takes action 2 (animation 0x2024) and hero B action 0xC (0x2066), both with sprite mode 1, and
   B's child takes action 0xC (0x2095). The child's sprite mode then ends up 1 either way (the game
   writes it even when B has no child). hero_b_sprite_idle_start_step10 follows. */
void hero_pair_spawn_fx_anim_next(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    effect_spawn_at(0x2FE6, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
    /* mode bits 1-2 are 2 or 4 while the body is on the field */
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 2, 0x2024, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0xC, 0x2066, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if (body_b->child != NULL)
    {
        sprite_set_action(body_b->child, 0xC, 0x2095, 0);
        ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
    }
    ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 1;
    body->state = hero_b_sprite_idle_start_step10;
}

/* Once hero A's animation finishes: hero A goes back to its unmirrored idle action (0), hero B
 * starts a 10-frame progress run (rate 0x4000 / 10), and hero_b_active_partner_anim_place takes over. */
void hero_b_sprite_idle_start_step10(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 10);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_active_partner_anim_place;
    }
}

/* Pair-move state: advances hero B's motion. When it ends (flag 0x20): plays sound slot 0x41,
   hero B (if on the field) and its child take action 0xD, hero B walks to the partner's target
   spot plus its offsets (in pixels) at speed 0x300, the low 12 bits of B's timer are cleared and
   hero_b_step_d8164 takes over. */
void hero_b_active_partner_anim_place(struct HeroBody *body)
{
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x41, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xD, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (body_b->child != NULL)
        {
            sprite_set_action(body_b->child, 0xD, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = partner->targetX / 0x100;
            x += partner->ofsX;
            y = partner->targetY / 0x100;
            y += partner->ofsY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x300);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_d8164;
    }
}
