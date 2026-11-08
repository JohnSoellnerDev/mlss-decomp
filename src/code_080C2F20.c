#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 unk06;
    u8 filler08[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
};

struct HeroLayer
{
    struct HeroLayer *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
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
    u8 filler0F8[0x111 - 0xF8];
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
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
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
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

extern struct World *gWorld;
extern u8 gSys[];

void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void hero_pair_place_and_finish_alt(struct HeroBody *body);
void hero_pair_place_and_finish_alt2(struct HeroBody *body);
void hero_b_sprite_active_anim3_to_3bec(struct HeroBody *body);
void hero_a_step_window_2fec(struct HeroBody *body);
void hero_a_walk_to_partner_3cb8(struct HeroBody *body);
void hero_a_active_fx_anim_ready(struct HeroBody *body);
void hero_a_active_fx_anim_ready_alt(struct HeroBody *body);
void hero_a_active_link_place_finish_alt(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 value);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void state_stack_advance_progress(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 kind, s32 x, s32 y, s32 arg5);
void body_set_pose(u32 kind, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void find_frame_entry(struct HeroBody *body, u32 index);
void hero_a_place_on_partner_offset(struct HeroBody *body);
void hero_a_step_sprite_anim5_to_3754(struct HeroBody *body);

struct WorldModeBits
{
    u8 lo4 : 4;
    u8 f45 : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct WorldMode2Bits
{
    u8 lo2 : 2;
    u8 g23 : 2;
    u8 hi4 : 4;
} __attribute__((packed));

static inline void world_mode_set_bit5(void)
{
    struct WorldModeBits *bits = (struct WorldModeBits *)&gWorld->modeByte;
    bits->f45 = bits->f45 | 2;
}

static inline u32 world_mode_bit2(void)
{
    return (((struct WorldMode2Bits *)&gWorld->modeByte)->g23 & ~2) != 0;
}

static inline void set_timer_mode(struct HeroBody *body, u32 mode)
{
    body->timer = (body->timer & 0xFFF) | (mode << 12);
}

void hero_b_visible_anim_b_rate_a(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xB, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xA);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_window_2fec;
    }
}

NONMATCH("asm/code_080C2F20/hero_a_step_window_2fec.s", void hero_a_step_window_2fec(struct HeroBody *body))
{
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    struct HeroBody *p;
    void (*fn)(struct HeroBody *);
    u32 (*divide)(u32, u32);
    s32 end;
    s32 rate;
    s32 start;
    s32 mid;
    s32 lo;
    s32 in_start;
    s32 mid_c;
    s32 end_c;
    s32 limit;
    s32 reached;
    s32 end_w;
    s32 in_end;
    s32 mode_now;
    s32 end_t;
    s32 cap;
    s32 reached_t;
    s32 none;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 height;
    s32 m;

    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    state_stack_advance_progress(body_a);

    rate = body_a->rate;
    start = end - rate * 10;
    mid = end - (rate << 3);
    end = end + (rate << 1);
    if (body_a->progress < end)
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;

    if ((body_a->timer >> 12) != 0)
        goto tail;

    lo = start;
    if (lo < 0)
        lo = 0;
    if (lo > 0x7FFF)
        lo = 0x7FFF;
    if (body_a->step > lo)
        goto no_in_start;
    in_start = 1;
    if (body_a->progress >= lo)
        goto done_in_start;
no_in_start:
    in_start = 0;
done_in_start:
    if (in_start)
    {
        if ((gWorld->view & 8) != 0)
        {
            p = body_a;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x33;
                    p = p->child;
                } while (p != 0);
            }
            p = body_b;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x33;
                    p = p->child;
                } while (p != 0);
            }
            p = partner;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x33;
                    p = p->child;
                } while (p != 0);
            }
            p = gWorld->layers;
            if (p != 0)
            {
                do
                {
                    if (p->sprite != 0)
                        p->sprite->layer = 3;
                    p = p->next;
                } while (p != 0);
            }
        }
        else
        {
            p = body_a;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = body_b;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = partner;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = gWorld->layers;
            if (p != 0)
            {
                do
                {
                    if (p->sprite != 0)
                        p->sprite->layer = 16;
                    p = p->next;
                } while (p != 0);
            }
        }
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    mid_c = mid;
    end_c = end;
    if (mid_c < 0)
        mid_c = 0;
    if (end_c < 0)
        end_c = 0;
    if (mid_c > 0x4000)
        mid_c = 0x4000;
    if (end_c > 0x7FFF)
        end_c = 0x7FFF;
    if (end_c < mid_c)
        mid_c = end_c;
    if (body_a->progress < mid_c)
        goto no_reached;
    reached = 1;
    if (body_a->step <= end_c)
        goto done_reached;
no_reached:
    reached = 0;
done_reached:
    if (reached)
    {
        set_body_part(body_b, 2, 0x14, 0, 0);
        if ((((struct SysKeys3E *)gSys)->keys & 2) != 0)
        {
            body_set_pose(1, body_b, 0, -40);
            obj_free_parts(body_b);
            world_mode_set_bit5();
            if (!world_mode_bit2())
            {
                if ((gWorld->view & 8) != 0)
                {
                    p = body_a;
                    if (p != 0)
                    {
                        do
                        {
                            p->frame = 0x100;
                            p = p->child;
                        } while (p != 0);
                    }
                    p = body_b;
                    if (p != 0)
                    {
                        do
                        {
                            p->frame = 0x100;
                            p = p->child;
                        } while (p != 0);
                    }
                    p = partner;
                    if (p != 0)
                    {
                        do
                        {
                            p->frame = 0x100;
                            p = p->child;
                        } while (p != 0);
                    }
                    p = gWorld->layers;
                    if (p != 0)
                    {
                        do
                        {
                            if (p->sprite != 0)
                                p->sprite->layer = 16;
                            p = p->next;
                        } while (p != 0);
                    }
                }
                else
                {
                    p = body_a;
                    if (p != 0)
                    {
                        do
                        {
                            p->frame = 0x100;
                            p = p->child;
                        } while (p != 0);
                    }
                    p = body_b;
                    if (p != 0)
                    {
                        do
                        {
                            p->frame = 0x100;
                            p = p->child;
                        } while (p != 0);
                    }
                    p = partner;
                    if (p != 0)
                    {
                        do
                        {
                            p->frame = 0x100;
                            p = p->child;
                        } while (p != 0);
                    }
                    p = gWorld->layers;
                    if (p != 0)
                    {
                        do
                        {
                            if (p->sprite != 0)
                                p->sprite->layer = 16;
                            p = p->next;
                        } while (p != 0);
                    }
                }
                display_flush();
            }
            body_a->timer = (body_a->timer & 0xFFF) | 0x2000;
        }
    }

    if ((body_a->timer >> 12) != 0)
        goto tail;

    end_w = end;
    if (end_w < 0)
        end_w = 0;
    if (end_w > 0x7FFF)
        end_w = 0x7FFF;
    if (body_a->step > end_w)
        goto no_in_end;
    in_end = 1;
    if (body_a->progress >= end_w)
        goto done_in_end;
no_in_end:
    in_end = 0;
done_in_end:
    if (in_end)
    {
        world_mode_set_bit5();
        body_a->timer = (body_a->timer & 0xFFF) | 0x1000;
        if ((gWorld->view & 8) != 0)
        {
            p = body_a;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = body_b;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = partner;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = gWorld->layers;
            if (p != 0)
            {
                do
                {
                    if (p->sprite != 0)
                        p->sprite->layer = 16;
                    p = p->next;
                } while (p != 0);
            }
        }
        else
        {
            p = body_a;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = body_b;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = partner;
            if (p != 0)
            {
                do
                {
                    p->frame = 0x100;
                    p = p->child;
                } while (p != 0);
            }
            p = gWorld->layers;
            if (p != 0)
            {
                do
                {
                    if (p->sprite != 0)
                        p->sprite->layer = 16;
                    p = p->next;
                } while (p != 0);
            }
        }
        display_flush();
        obj_free_parts(body_b);
        goto tail;
    }
    if (body_a->progress > end)
        goto tail;
    if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
        goto tail;
    world_mode_set_bit5();
    if (world_mode_bit2())
        goto tail;
    body_a->timer = (body_a->timer & 0xFFF) | 0x1000;
    if ((gWorld->view & 8) != 0)
    {
        p = body_a;
        if (p != 0)
        {
            do
            {
                p->frame = 0x100;
                p = p->child;
            } while (p != 0);
        }
        p = body_b;
        if (p != 0)
        {
            do
            {
                p->frame = 0x100;
                p = p->child;
            } while (p != 0);
        }
        p = partner;
        if (p != 0)
        {
            do
            {
                p->frame = 0x100;
                p = p->child;
            } while (p != 0);
        }
        p = gWorld->layers;
        if (p != 0)
        {
            do
            {
                if (p->sprite != 0)
                    p->sprite->layer = 16;
                p = p->next;
            } while (p != 0);
        }
    }
    else
    {
        p = body_a;
        if (p != 0)
        {
            do
            {
                p->frame = 0x100;
                p = p->child;
            } while (p != 0);
        }
        p = body_b;
        if (p != 0)
        {
            do
            {
                p->frame = 0x100;
                p = p->child;
            } while (p != 0);
        }
        p = partner;
        if (p != 0)
        {
            do
            {
                p->frame = 0x100;
                p = p->child;
            } while (p != 0);
        }
        p = gWorld->layers;
        if (p != 0)
        {
            do
            {
                if (p->sprite != 0)
                    p->sprite->layer = 16;
                p = p->next;
            } while (p != 0);
        }
    }
    display_flush();
    obj_free_parts(body_b);
    body_set_pose(6, body_b, 0, -40);

tail:
    mode_now = body_a->timer >> 12;
    if (mode_now != 0)
    {
        cap = 0x4000;
        end_t = end;
        if (end_t < 0)
            end_t = 0;
        if (cap > 0x4000)
            cap = 0x4000;
        if (end_t > 0x7FFF)
            end_t = 0x7FFF;
        if (end_t < cap)
            cap = end_t;
        if (body_a->progress < cap)
            goto no_reached_t;
        reached_t = 1;
        if (body_a->step <= end_t)
            goto done_reached_t;
no_reached_t:
        reached_t = 0;
done_reached_t:
        if (reached_t && mode_now > 1)
        {
            body_a->timer = body_a->timer & 0xFFF;
            fn = hero_a_place_on_partner_offset;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if ((body_a->flags & 0x20) == 0)
        return;
    none = -1;
    set_slot_value(0x7D, none);
    m = body_a->mode & 6;
    if (m == 2 || m == 4)
    {
        target_x = partner->targetX / 0x100 + partner->ofsX - 0xA;
        target_y = partner->targetY / 0x100 + partner->ofsY + 3;
        target_z = partner->targetZ / 0x100 + partner->ofsZ - 0x15;
        place_targets(body_a, target_x, target_y, target_z, none);
        if ((u8 *)partner == (u8 *)partner->data + 8)
        {
            height = partner->ofsZ;
        }
        else
        {
            find_frame_entry(partner, 0);
            height = partner->unk47;
        }
        height += partner->targetZ / 0x100;
        divide = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
        height += divide(0x19, height * height / 0x40 - 5);
        body_a->height = height;
        set_motion_offsets(body_a, 0x600);
    }

    m = body_a->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(body_a, 4, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    body_a->timer = body_a->timer & 0xFFF;
    body->state = hero_a_step_sprite_anim5_to_3754;
}
END_NONMATCH
