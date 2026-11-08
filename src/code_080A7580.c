#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0x06 - 0x04];
    u16 scale;
    u8 filler8[0x0E - 0x08];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirrorFlags;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
};

struct HeroNode
{
    u16 x;
    u16 y;
    u8 filler4[0x0E - 0x04];
    u16 z;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 mirror : 1;
    u8 unk7 : 1;
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
    u8 bit7 : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 pose;
    u8 fillerEE[0x114 - 0xEE];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags;
};

struct HeroBody
{
    struct HeroBody *next;
    u8 filler04[0x08 - 0x04];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *link;
    struct HeroNode *node;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0x9C - 0x94];
    u32 unk9C;
    u8 filler_A0[0xAC - 0xA0];
    s16 step;
    u16 timer;
    u8 filler_B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler_C6[0xCA - 0xC6];
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
    u8 mode;
    u8 filler0B[0x0C - 0x0B];
    u8 display;
    u8 filler0D[0x0E - 0x0D];
    u8 unkE;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct SpriteNode *sprites;
};

extern struct World *gWorld;
extern u8 *gParty;

/* Party record: five 6-bit flag sets from bit 12 of the word at +8; the fourth one straddles
   the two words. */
struct PartyFlags
{
    u8 filler0[0x08];
    u32 unk8 : 12;
    u32 set0 : 6;
    u32 set1 : 6;
    u32 set2 : 6;
    u32 set3 : 6 __attribute__((packed));
    u32 set4 : 6;
};

extern struct KeyState gSys;

void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct HeroBody *owner);
void hero_b_sprite_grow_a_place(struct HeroBody *body);

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

static inline void world_mode_or1(void)
{
    ((struct HeroWorldModeBits *)&gWorld->mode)->mode |= 1;
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

/* Window frame values for the three body chains, frame 3 for every listed sprite. */
static inline void frames_win(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->link;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->link;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->link;
    }
    p = (struct HeroBody *)gWorld->sprites;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 3;
        p = p->next;
    }
}

/* Full frame values for the three body chains, frame 0x10 for every listed sprite. */
static inline void frames_full(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->link;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->link;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->link;
    }
    p = (struct HeroBody *)gWorld->sprites;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 0x10;
        p = p->next;
    }
}

void state_stack_advance_progress(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void hero_b_sprite_step_15_6d00(struct HeroBody *body);
void hero_b_sprite_step_18_70e8(struct HeroBody *body);
void hero_b_ready_step_6d78(struct HeroBody *body);
void hero_b_ready_step_7160(struct HeroBody *body);
void hero_a_active_anims_pair(struct HeroBody *body);
void hero_b_gate_place_pair_6e34(struct HeroBody *body);
void hero_pair_place_finish_on_b_active(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_release_node(void *node);
void schedule_pose_task(struct HeroBody *partner);
void hero_pair_place_then_a_clear(struct HeroBody *body);
void hero_b_active_link_to_partner(struct HeroBody *body);
void hero_b_active_link_to_partner_alt(struct HeroBody *body);
void hero_a_sprite_place_at_b(struct HeroBody *body);
void hero_a_step_wait_press(struct HeroBody *body);
void hero_a_active_clear_anim(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void sprite_entry_update(struct HeroNode *node);
void hero_b_drop_node_702c(struct HeroBody *body);
void hero_b_drop_node_6c40(struct HeroBody *body);

/* Mode bits 6-7 of the world from the party flags, then both heroes to their targets with the
   pose animation and mirror; hero B's state ends, the body goes on to clearing hero A's animation. */
void hero_pair_place_then_a_clear(struct HeroBody *body)
{
    struct World *world_mode;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    s16 *span;
    s32 x;
    s32 y;
    s32 z;
    u8 flag_a;
    u32 flag_bit;
    u32 bits;

    gWorld->unkE = 0;
    world_mode = gWorld;
    flag_a = ((struct PartyFlags *)gParty)->set0 & 1;
    flag_bit = ((struct PartyFlags *)gParty)->set3 & 1;
    bits = (flag_a + (flag_bit << 1)) << 6;
    world_mode->mode = (world_mode->mode & 0x3F) | bits;

    body_a = &gWorld->heroA->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    set_slot_value(0x53, -1);

    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        data = body_b->data;
        x = data->targetX / 0x100 + 0x14;
        y = data->targetY / 0x100 - 0x10;
        z = data->targetH / 0x100;
        place_targets(body_b, x, y, z, -1);
        set_motion_start(body_b, 0x300);
    }

    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        if (body_b->data->pose == -1)
        {
            sprite_set_action(body_b, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(body_b, 0, 0x204F, 0);
        }
        ((struct HeroMirrorBits *)&body_b->sprite->mirrorFlags)->mirror = (span = &body_b->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }

    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->state = 0;
        enter_finish_rebuild(body_b);
    }

    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        data = body_a->data;
        x = data->targetX / 0x100 - 0x22;
        y = data->targetY / 0x100 + 0x10;
        z = data->targetH / 0x100;
        place_targets(body_a, x, y, z, -1);
        set_motion_start(body_a, 0x400);
    }

    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        if (body_a->data->pose == -1)
        {
            sprite_set_action(body_a, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(body_a, 0, 0x204F, 0);
        }
        ((struct HeroMirrorBits *)&body_a->sprite->mirrorFlags)->mirror = (span = &body_a->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }

    body->state = hero_a_active_clear_anim;
}

void hero_a_step_16_b_idle_alt(struct HeroBody *body)
{
    struct HeroBody *a = &gWorld->heroA->body;

    if (gWorld->heroB->body.state == 0)
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            a->progress = 0;
            ((struct HeroFlagBits *)&a->flags)->active = 0;
        }
        body->state = hero_a_active_anims_pair;
    }
}

void hero_a_active_anims_pair(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *a = &hero->body;
    struct HeroBody *b = &gWorld->heroB->body;

    state_stack_advance_progress(a);
    if ((a->flags & 0x20) != 0)
    {
        set_slot_value(0x88, -1);
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(a, 0, 0x202F, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            sprite_set_action(b, 0, 0x205F, 0);
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
        }
        body->state = hero_a_sprite_place_at_b;
    }
}

void hero_a_sprite_place_at_b(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    s32 x;
    s32 y;
    s32 z;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            x = body_b->targetX / 0x100 + body_b->ofsX;
            y = body_b->targetY / 0x100 + body_b->ofsY;
            z = body_b->targetZ / 0x100 + body_b->ofsZ - 0xC;
            place_targets(body_a, x, y, z, -1);
            {
                u16 h = 0x2F;
                hero_a->body.height = h;
            }
            set_motion_offsets(body_a, 0x466);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xE, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body_a->unk9C = 0;
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_wait_press;
    }
}

/* Hero A waits for the A button while stepping: frame values at the marks before the end of the
   step, pose 3 on a press inside the window, an effect at the end mark, flags and the sprite
   action at the full step, then the B sprite scale from the height difference. */
void hero_a_step_wait_press(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 lead8;
    s32 near;
    s32 full;
    s32 rate;
    s32 end;
    s32 tmode;
    s32 ofs;

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    near = 0x4000;
    full = 0x4000;
    step_motion(a);
    rate = a->rate;
    lead8 = near - (rate << 3);
    near -= rate << 2;
    end = (((rate << 1) + rate) << 1) + 0x4000;

    if (a->progress < end)
    {
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, lead8))
    {
        if ((gWorld->display & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, near, end))
    {
        set_body_part(a, 1, 0x18, 0, 0);
        if ((gSys.pressed & 1) != 0)
        {
            body_set_pose(3, a, 0x2E, 0);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct HeroWorldModeBits *)&gWorld->mode)->m2 & ~1) == 0)
            {
                if ((gWorld->display & 8) != 0)
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

    if (step_reached(a, end))
    {
        world_mode_or1();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->display & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto after_zero;
    }

    if (a->progress > end)
        goto after_zero;

    if ((gSys.pressed & 1) == 0)
    {
        if ((gSys.pressed & 3) == 0)
            goto after_zero;
    }

    world_mode_or1();
    if ((((struct HeroWorldModeBits *)&gWorld->mode)->m2 & ~1) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->display & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 0x2E, 0);

after_zero:
    tmode = a->timer >> 12;
    if (tmode != 0 && step_in_window(a, full, end) && tmode == 2)
    {
        if (a->unk9C == 0)
            effect_spawn_at(0x2F7D, a->x / 0x100, a->y / 0x100, a->z / 0x100, a);
        a->unk9C = 1;
    }

    if (step_reached(a, lead8))
        set_cur_flags_both(0x40000000);
    if (step_reached(a, near))
        set_cur_flags_both(0x40000000);
    if (step_reached(a, 0x4000))
    {
        set_slot_value(0x7C, -1);
        sprite_set_action(a, 1, 0x2030, 0);
        {
            u8 sprFlags = a->sprite->flags;
            a->sprite->flags = sprFlags | 0x10;
        }
    }

    ofs = b->ofsZ - 9;
    if (a->z < a->targetZ && a->z < b->z + (ofs << 8))
        b->sprite->scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->z, ofs);

    if ((a->flags & 0x20) != 0)
        body->state = hero_b_sprite_grow_a_place;
}
