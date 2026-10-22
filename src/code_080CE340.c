#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x02];
    s16 ofsY;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
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

struct HeroChildFlagBits
{
    u8 bit0 : 1;
    u8 unk1 : 1;
    u8 bit2 : 1;
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

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 partnerLinked : 1;
    u8 unk2 : 6;
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
    u8 filler70[0x75 - 0x70];
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xA0 - 0x94];
    u32 unkA0;
    u8 filler0A4[0xA8 - 0xA4];
    u32 unkA8;
    s16 step;
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
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gParty;

void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void hero_pair_place_partner_height(struct HeroBody *body);
void hero_a_child_fade_b_step30(struct HeroBody *body);
void hero_b_follow_tracked(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_b_child_follow_partner(struct HeroBody *body);
void hero_b_match_partner(struct HeroBody *body);
void hero_b_partner_link_fx(struct HeroBody *body);
void hero_b_finish_child_anim9(struct HeroBody *body);
void hero_a_release_body_cf9f4(struct HeroBody *body);
void hero_a_child_window_eb9c(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void hero_b_active_link_partner_anchor(struct HeroBody *body);
void hero_b_finish_child_anim9_alt(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, const u8 *e);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
extern const u8 gUnk_0800E210[];
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);

static inline void *tracked_obj(void)
{
    return gWorld->trackedObj;
}

void hero_a_child_place_on_b(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    u16 height;
    struct HeroBody *child;

    step_motion(body_a->child);
    if ((body_a->child->flags & 0x20) != 0)
    {
        set_slot_value(0x9C, -1);
        if ((body_a->child->mode & 6) == 2 || (body_a->child->mode & 6) == 4)
        {
            place_targets(body_a->child, body_b->targetX / 0x100 + body_b->ofsX + 6,
                          body_b->targetY / 0x100 + body_b->ofsY, 0xC, -1);
            child = body_a->child;
            height = 0x28;
            child->height = height;
            set_motion_offsets(body_a->child, 0x500);
        }
        body->state = hero_a_child_fade_b_step30;
    }
}

void hero_a_child_fade_b_step30(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    step_motion(body_a->child);
    if ((body_a->child->flags & 0x20) != 0)
    {
        sprite_set_action(body_a->child, 0, 0, 0);
        ((struct HeroChildFlagBits *)&body_a->child->flags)->bit0 = 0;
        ((struct HeroSpriteBits *)&body_a->child->sprite->flags)->mode = 0;
        body_a->child->unk75 = body_b->unk75 - 1;
        set_slot_value(0x104, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x2065, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x1E);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_follow_tracked;
    }
}

void hero_b_follow_tracked(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 x;
    s32 y;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0xA7, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 1, 0x2065, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            s32 offY = body_b->sprite->ofsY;
            x = (s16)obj_last_x(tracked_obj()) - 0x10;
            y = (s16)obj_last_y(tracked_obj()) + offY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x380);
        }
        body->state = hero_b_child_follow_partner;
    }
}
