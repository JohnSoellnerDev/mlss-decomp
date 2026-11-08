#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x0E];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
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
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 unk111;
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
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 speed;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xAC - 0x94];
    s16 prevProgress;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

struct WorldModeBits
{
    u8 low4 : 4;
    u8 mid : 2;
    u8 high : 2;
};

struct DrawNode
{
    struct DrawNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x0C - 0x0B];
    u8 view;
    u8 filler0D[0x0E - 0x0D];
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct DrawNode *nodes;
    u8 filler7C[0x24C - 0x7C];
    void *focus24C;
};

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;

void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void obj_release(void *obj);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void schedule_pose_task(struct HeroBody *obj);
void state_stack_advance_progress(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);

void hero_a_step16_when_b_idle(struct HeroBody *body);
void hero_pair_place_both_finish_sprite(struct HeroBody *body);
void hero_pair_place_both_finish_sprite_alt(struct HeroBody *body);
void hero_pair_place_both_tracked(struct HeroBody *body);
void hero_b_spawn_child_chain(struct HeroBody *body);
void hero_b_place_at_a_height(struct HeroBody *body);
void hero_b_link_partner_step16(struct HeroBody *body);
void hero_b_step_keys_d4a18(struct HeroBody *body);
void hero_a_mode_place_finish(struct HeroBody *body);
void hero_b_window_d5114(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_step_window_d3c38(struct HeroBody *body);
void hero_b_end_a(struct HeroBody *body);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void hero_pair_place_both_finish_adv(struct HeroBody *body);

/* Speed of a body and of every body after it in the chain. */
static inline void chain_speed(struct HeroBody *head, u16 speed)
{
    struct HeroBody *link = head;
    if (head != 0) {
        do {
            link->speed = speed;
            link = link->child;
        } while (link != 0);
    }
}

/* Layer of every sprite in the display list. */
static inline void set_layers(u8 layer)
{
    struct DrawNode *node = gWorld->nodes;
    if (node != 0) {
        do {
            if (node->sprite != 0) {
                node->sprite->layer = layer;
            }
            node = node->next;
        } while (node != 0);
    }
}

static inline void speed_layers(struct HeroBody *a, struct HeroBody *b, struct HeroBody *p, u16 speed, u8 layer)
{
    chain_speed(a, speed);
    chain_speed(b, speed);
    chain_speed(p, speed);
    set_layers(layer);
}

static inline void timer_set_mode(struct HeroBody *b, u32 nibble)
{
    b->timer = (b->timer & 0xFFF) | (nibble << 12);
}

static inline void timer_clear_mode(struct HeroBody *b)
{
    b->timer = b->timer & 0xFFF;
}

NONMATCH("asm/code_080D4A18/hero_b_step_keys_d4a18.s", void hero_b_step_keys_d4a18(struct HeroBody *body))
{
    struct Hero *hb;
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    struct HeroBody *next;
    s16 *progp;
    s16 *prevp;
    s16 *ratep;
    u16 *timerp;
    s32 limit;
    s32 budget;
    s32 budgetCopy;
    s32 lim;
    s32 cap;
    s32 edge;
    s32 reached;
    s32 atEdge;
    s32 nib;
    s32 mode;
    u16 keys;
    void (*fn)(struct HeroBody *);

    limit = 0x4000;
    a = &gWorld->heroA->body;
    hb = gWorld->heroB;
    b = &hb->body;
    partner = body->partner;
    step_motion(b);

    next = b->child;
    next->posX = b->posX;
    next->posY = b->posY;
    next->posZ = b->posZ;

    if ((gWorld->modeByte & 0xC0) == 0) {
        goto L50E4;
    }

    ratep = &hb->body.rate;
    budget = 0x4000 + (-(*ratep * 26));
    budgetCopy = budget;
    limit = limit - *ratep * 14;
    progp = &hb->body.progress;
    if (!(*progp >= limit)) {
        ((struct HeroFlagBits *)&b->flags)->active = 0;
    }
    timerp = &hb->body.timer;
    if ((*timerp >> 12) != 0) {
        goto L5038;
    }

    if ((gWorld->modeByte & 0xC0) == 0) {
        goto L4BDC;
    }
    prevp = &hb->body.prevProgress;
    lim = budget;
    if (lim < 0) {
        lim = 0;
    }
    if (lim > 0x7FFF) {
        lim = 0x7FFF;
    }
    reached = 0;
    if (!(*prevp > lim)) {
        if (*progp >= lim) {
            reached = 1;
        }
    }
    if (!reached) {
        goto L4BDC;
    }

    if (!(gWorld->view & 8)) {
        goto L4B78;
    }
    speed_layers(a, b, partner, 0x4C, 4);
    goto L4BD4;
L4B78:
    speed_layers(a, b, partner, 0x100, 0x10);
L4BD4:
    sprite_objects_draw_obj_tiles(0x4C, 0xC);

L4BDC:
    edge = budgetCopy;
    cap = limit;
    if (edge < 0) {
        edge = 0;
    }
    if (limit < 0) {
        cap = 0;
    }
    if (edge > 0x4000) {
        edge = 0x4000;
    }
    if (cap > 0x7FFF) {
        cap = 0x7FFF;
    }
    if (cap < edge) {
        edge = cap;
    }
    atEdge = 0;
    if (!(*progp < edge)) {
        if (*prevp <= cap) {
            atEdge = 1;
        }
    }
    if (!atEdge) {
        goto L4D9E;
    }

    mode = gWorld->modeByte & 0xC0;
    if (mode == 0x80) {
        set_body_part(a, 1, -22, 0, 0);
        goto L4C68;
    }
    if (mode != 0x40) {
        goto L4C68;
    }
    set_body_part(a, 0xFF000000, -22, 0, 0);

L4C68:
    if (!(gSys.pressed & 1)) {
        goto L4D9E;
    }
    body_set_pose(7, a, 8, -40);
    obj_free_parts(a);
    ((struct WorldModeBits *)&gWorld->modeByte)->mid |= 1;
    if (gWorld->modeByte & 8) {
        goto L4D8A;
    }
    if (gWorld->view & 8) {
        speed_layers(a, b, partner, 0x100, 0x10);
    } else {
        speed_layers(a, b, partner, 0x100, 0x10);
    }
    display_flush();

L4D8A:
    timer_set_mode(b, 2);

L4D9E:
    if ((*timerp >> 12) != 0) {
        goto L5038;
    }

    lim = limit;
    if (lim < 0) {
        lim = 0;
    }
    if (lim > 0x7FFF) {
        lim = 0x7FFF;
    }
    reached = 0;
    if (!(*prevp > lim)) {
        if (*progp >= lim) {
            reached = 1;
        }
    }
    if (!reached) {
        goto L4EE8;
    }

    ((struct WorldModeBits *)&gWorld->modeByte)->mid |= 1;
    timer_set_mode(b, 1);
    if (gWorld->view & 8) {
        speed_layers(a, b, partner, 0x100, 0x10);
    } else {
        speed_layers(a, b, partner, 0x100, 0x10);
    }
    display_flush();
    obj_free_parts(a);
    goto L5038;

L4EE8:
    if (*progp > limit) {
        goto L5038;
    }
    keys = gSys.pressed;
    if (!(keys & 1)) {
        if (!(keys & 3)) {
            goto L5038;
        }
    }

    ((struct WorldModeBits *)&gWorld->modeByte)->mid |= 1;
    if (gWorld->modeByte & 8) {
        goto L5038;
    }
    timer_set_mode(b, 1);
    if (gWorld->view & 8) {
        speed_layers(a, b, partner, 0x100, 0x10);
    } else {
        speed_layers(a, b, partner, 0x100, 0x10);
    }

L5020:
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 8, -40);

L5038:
    nib = *timerp >> 12;
    if (nib == 0) {
        goto L50B4;
    }
    lim = budget;
    if (lim < 0) {
        lim = 0;
    }
    cap = limit;
    if (cap < 0) {
        cap = 0;
    }
    if (lim > 0x4000) {
        lim = 0x4000;
    }
    if (cap > 0x7FFF) {
        cap = 0x7FFF;
    }
    if (cap < lim) {
        lim = cap;
    }
    atEdge = 0;
    if (!(*progp < lim)) {
        if (*prevp <= cap) {
            atEdge = 1;
        }
    }
    if (!atEdge) {
        goto L50B4;
    }
    if (nib <= 1) {
        goto L50B4;
    }
    timer_clear_mode(b);
    body->state = hero_a_mode_place_finish;
    fn = hero_a_mode_place_finish;
    fn(body);
    return;

L50B4:
    lim = limit;
    if (lim < 0) {
        lim = 0;
    }
    if (lim > 0x7FFF) {
        lim = 0x7FFF;
    }
    reached = 0;
    if (!(*prevp > lim)) {
        if (*progp >= lim) {
            reached = 1;
        }
    }
    if (!reached) {
        return;
    }

L50E4:
    timer_clear_mode(b);
    body->state = hero_b_window_d5114;
}
END_NONMATCH
