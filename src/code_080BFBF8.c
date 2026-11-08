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

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x111 - 0xEE];
    u8 flags111;
    u8 filler112[0x114 - 0x112];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    struct HeroBody *next;
    u8 filler04[0x08 - 0x04];
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
    struct HeroBody *chain;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 unk92;
    u32 wideLo;
    u32 wideHi;
    u8 filler9C[0xA8 - 0x9C];
    u32 link;
    s16 step;
    u16 timer;
    s16 randX;
    s16 randY;
    u8 filler0B4[0xC3 - 0xB4];
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

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x0C - 0x0B];
    u8 flags0C;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
    u8 filler7C[0x2BF - 0x7C];
    u8 byte2BF;
};

extern struct World *gWorld;
extern u8 gSys[];
extern const s8 gHandOverJitterStart[];
extern s8 gHandOverDriftRange[];
extern const u8 gHandOverLimitTable[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void start_action_5(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void schedule_pose_task(struct HeroBody *obj);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
void owner_take_link_state(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, u32 x, u32 y);
void hero_b_shift_place_pose_next(struct HeroBody *body);
void hero_b_place_last_pose(struct HeroBody *body);
void hero_a_join_partner_next(struct HeroBody *body);
void hero_a_sprite_place_partner(struct HeroBody *body);
void hero_pair_place_finish_a_sprite(struct HeroBody *body);
void hero_b_sprite_place_mirror(struct HeroBody *body);
void hero_pair_place_finish_b_sprite(struct HeroBody *body);

void hero_a_hand_over(struct HeroBody *body);
void hero_b_after_hand_over(struct HeroBody *body);
void hero_a_active_setup(struct HeroBody *body);
void hero_b_take_a_position(struct HeroBody *body);
void hero_b_visible_place(struct HeroBody *body);
void link_partner_and_hero_b(struct HeroBody *body);
void hero_b_follow_partner(struct HeroBody *body);
void hero_b_step_and_release(struct HeroBody *body);
void hero_pair_timer_step_06f0(struct HeroBody *body);
void hero_a_effect_link_rate_next(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_a_visible_place_finish(struct HeroBody *body);
void hero_b_active_place_and_finish(struct HeroBody *body);
void hero_b_start_anim_f(struct HeroBody *body);
void step_end_and_partner_next(struct HeroBody *body);
void hero_b_wait_visible_anim_e(struct HeroBody *body);
void hero_b_fx_and_end(struct HeroBody *body);
void hero_a_wait_visible_anim_33(struct HeroBody *body);
void hero_b_anim_c_then_clear(struct HeroBody *body);
void hero_a_wait_visible_anim_24(struct HeroBody *body);
void hero_b_active_fx(struct HeroBody *body);

void hero_a_hand_over(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        partner->link = 0;
        set_slot_value(0x30, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 5, 0x2000, 0);
            else
                sprite_set_action(body_a, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            s32 x = body_a->targetX / 0x100 + body_a->ofsX;
            s32 y = body_a->targetY / 0x100 + body_a->ofsY;
            u16 offset;
            place_targets(body_a, x, y, 8, -1);
            offset = 0x18;
            body_a->unk92 = offset;
            set_motion_offsets(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xF);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            s32 x = body_a->targetX / 0x100 + body_a->ofsX;
            s32 y = body_a->targetY / 0x100 + body_a->ofsY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xC);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_after_hand_over;
    }
}

void hero_b_after_hand_over(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    step_motion(body_a);
    step_motion(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xA, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = 1;
        body->state = hero_a_active_setup;
    }
}

void hero_a_active_setup(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroData *data;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    step_motion(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2033, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0x10, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = 0;
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + 0x10;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + 0x18;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = (th >> 8) + 8;
            place_targets(body_a, x, y, h, -1);
            set_motion_start(body_a, 0x180);
        }
        body->state = hero_b_take_a_position;
    }
}

void hero_b_take_a_position(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroData *data;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    step_motion(body_a);
    body_b->posX = body_a->posX;
    body_b->posY = body_a->posY;
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x2b, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_a, x, y, h, -1);
            th = body_a->data->targetH;
            if (th < 0)
                th += 0xFF;
            body_a->unk92 = (th >> 8) + 0x20;
            set_motion_offsets(body_a, 0x500);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            start_action_5(body_a);
        }
        body = body_b;
        set_slot_value(0x38, -1);
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            sprite_set_action(body, 0x11, 0x2062, 0);
            body->sprite->flags = ((*(s8 *)&body->sprite->flags) & -7) | 2;
        }
        body_b->unk74 = 0;
        body_b->unk75 = 0;
        body->state = hero_b_visible_place;
    }
}

void hero_b_visible_place(struct HeroBody *body)
{
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroData *data;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_b, x, y, h, -1);
            th = body_b->data->targetH;
            if (th < 0)
                th += 0xFF;
            body_b->unk92 = (th >> 8) + 0x20;
            set_motion_offsets(body_b, 0x500);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            start_action_5(body_b);
        }
    }
}

void link_partner_and_hero_b(struct HeroBody *body)
{
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;

    partner->link = 1;
    if (partner->targetZ == 0)
    {
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
        {
            partner->state = hero_a_join_partner_next;
        }
    }
    effect_spawn_at(0x5DF, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 4, 0x2062, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body_b->unk74 = 0;
    body_b->unk75 = 0;
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = hero_b_follow_partner;
}

static inline s32 rand_span(s32 lo, s32 hi)
{
    return rand_below(hi - lo, mt19937_next());
}

/* Hero B follows the partner: the partner may join when it is low enough, hero B is placed on the
   partner's target and takes its pose, hero A gets its jitter, then the timer decides the next state. */
NONMATCH("asm/code_080BFBF8/hero_b_follow_partner.s", void hero_b_follow_partner(struct HeroBody *body))
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    u16 count;
    u8 limit;

    state_stack_advance_progress(body_b);
    if (partner->link != 0)
    {
        if ((partner->targetZ << 6) < body_b->ofsZ * body_b->progress)
        {
            if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
                partner->state = hero_a_join_partner_next;
        }
    }
    if ((body_b->flags & 0x20) == 0)
        return;
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        place_targets(body_b, partner->targetX / 0x100 + partner->ofsX, partner->targetY / 0x100 + partner->ofsY, 0, -1);
        body_b->unk92 = 0x20;
        set_motion_offsets(body_b, 0x600);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        if (body_b->data->poseIndex == -1)
            sprite_set_action(body_b, 5, 0x2000, 0);
        else
            sprite_set_action(body_b, 5, 0x204D, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 6, 0x2025, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    count = body_a->timer & 0xFFF;
    if (count <= 2)
    {
        body_a->randX = gHandOverJitterStart[count * 2];
        body_a->randY = gHandOverJitterStart[(body_a->timer & 0xFFF) * 2 + 1];
    }
    else
    {
        if (gHandOverDriftRange[0] < gHandOverDriftRange[2])
            body_a->randX = gHandOverDriftRange[0] + rand_span(gHandOverDriftRange[0], gHandOverDriftRange[2]);
        else
            body_a->randX = gHandOverDriftRange[2] + rand_span(gHandOverDriftRange[2], gHandOverDriftRange[0]);
        if (gHandOverDriftRange[1] < gHandOverDriftRange[3])
            body_a->randY = gHandOverDriftRange[1] + rand_span(gHandOverDriftRange[1], gHandOverDriftRange[3]);
        else
            body_a->randY = gHandOverDriftRange[3] + rand_span(gHandOverDriftRange[3], gHandOverDriftRange[1]);
    }
    body_a->timer++;
    limit = gHandOverLimitTable[gWorld->byte2BF >> 6];
    if (limit != 0 && limit <= (body_a->timer & 0xFFF))
    {
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
            body->state = hero_b_active_place_and_finish;
    }
    else
        body->state = hero_b_step_and_release;
}
END_NONMATCH

/* Hero B steps until the ratio of its wide counters is reached, releases the partner link and
   takes its pose; the keys hand over to the active place, and at the end hero A gets its step rate. */
void hero_b_step_and_release(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    u32 lo;
    u32 hi;
    u32 *link;
    s32 reached;
    s32 ratio;
    s32 z;
    s32 t;

    step_motion(body_b);
    lo = hero_b->body.wideLo;
    hi = hero_b->body.wideHi;
    ratio = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(hi << 14, lo);
    if (hero_b->body.step > ratio)
        reached = 0;
    else
    {
        t = hero_b->body.progress;
        reached = 1;
        if (t < ratio)
            reached = 0;
    }
    link = &partner->link;
    if (reached != 0)
    {
        if (*link != 0)
        {
            *link = 0;
            body_set_pose(6, body_b, 0x2E, 8);
            ((struct HeroDataFlagBits *)&partner->data->flags111)->bit1 = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 6, 0x2000, 0);
            else
                sprite_set_action(body_b, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if (*link != 0)
    {
        z = body_b->posZ + body_b->ofsZ;
        if (partner->targetZ < z)
        {
            if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
                partner->state = hero_a_join_partner_next;
        }
    }
    if ((((struct SysKeys3E *)gSys)->keys & 3) != 0)
    {
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
            body->state = hero_b_active_place_and_finish;
        set_slot_value(0xD0, -1);
    }
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xC);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body_a->timer &= 0xFFF;
        body->state = hero_pair_timer_step_06f0;
    }
}





/* World mode bits 4-5: set bit 4. */
static inline void world_mode_or1(void)
{
    ((struct HeroWorldModeBits *)&gWorld->modeByte)->mode |= 1;
}

/* Step length within x and progress at least x (x clamped to 0..0x7FFF). */
static inline u32 step_reached(struct HeroBody *a, s32 x)
{
    u32 r;
    s32 t;

    if (x < 0)
        x = 0;
    if (x > 0x7FFF)
        x = 0x7FFF;
    if (a->step <= x)
    {
        t = a->progress;
        r = 1;
        if (t >= x)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Progress at least lo and step length within hi (both clamped, lo at most hi). */
static inline u32 step_in_window(struct HeroBody *a, s32 lo, s32 hi)
{
    u32 r;
    s32 t;

    if (lo < 0)
        lo = 0;
    if (hi < 0)
        hi = 0;
    if (lo > 0x4000)
        lo = 0x4000;
    if (hi > 0x7FFF)
        hi = 0x7FFF;
    if (hi < lo)
        lo = hi;
    if (a->progress >= lo)
    {
        t = a->step;
        r = 1;
        if (t <= hi)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Window frame values for the three body chains, layer 4 for every listed sprite. */
static inline void frames_win(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x40;
        p = p->chain;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x40;
        p = p->chain;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x40;
        p = p->chain;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 4;
        p = p->next;
    }
}

/* Full frame values for the three body chains, layer 0x10 for every listed sprite. */
static inline void frames_full(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->chain;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->chain;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->chain;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 0x10;
        p = p->next;
    }
}

/* Hero pair timer step: frame and layer values at the marks of hero A's step, hero A's pose on the
   A key, the timer phase, then the next states of both heroes. */
void hero_pair_timer_step_06f0(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 x;
    s32 x2;
    s32 y;
    s32 rate;
    s32 lower;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    x = 0x4000;
    y = 0x4000;
    state_stack_advance_progress(a);
    rate = a->rate;
    lower = x - rate * 12;
    x += rate * a->randX;
    x2 = x;
    y += rate * a->randY;

    if (a->progress < y)
    {
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, lower))
    {
        if ((gWorld->flags0C & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, x, y))
    {
        set_body_part(a, 1, -0x18, 0, 0);
        if ((((struct SysKeys3E *)gSys)->keys & 1) != 0)
        {
            body_set_pose(1, a, -0x2A, 0);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) == 0)
            {
                if ((gWorld->flags0C & 8) != 0)
                    frames_full(a, b, partner);
                else
                    frames_full(a, b, partner);
                display_flush();
            }
            a->timer = (a->timer & 0xFFF) | 0x2000;
        }
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, y))
    {
        world_mode_or1();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->flags0C & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto after_zero;
    }

    if (a->progress > y)
        goto after_zero;

    if ((((struct SysKeys3E *)gSys)->keys & 1) == 0)
    {
        if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
            goto after_zero;
    }

    world_mode_or1();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->flags0C & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, -0x2A, 0);

after_zero:
    {
        s32 tmode = a->timer >> 12;
        if (tmode != 0 && step_in_window(a, x2, y) && tmode > 1)
        {
            a->timer &= 0xFFF;
            fn = hero_a_effect_link_rate_next;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if (step_reached(a, x))
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            sprite_set_action(a, 1, 0x2025, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
    }

    if ((a->flags & 0x20) == 0)
        return;

    partner->link = 0;
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        sprite_set_action(b, 0xF, 0x2062, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        b->state = hero_b_start_anim_f;
    body = a;
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        sprite_set_action(body, 9, 0x2024, 0);
        ((struct HeroSpriteBits *)&body->sprite->flags)->mode = 1;
    }
    body->state = hero_a_visible_place_finish;
}
