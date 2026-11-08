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

struct WorldModeBits
{
    u8 bit0 : 1;
    u8 mid : 3;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 high : 2;
};

struct HeroTimerBits
{
    u16 low : 12;
    u16 mode : 4;
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
    u8 filler0[0x08];
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
    u16 rate;
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

static inline void chain_frames(struct HeroBody *p, u16 frame)
{
    for (; p != 0; p = p->link)
        p->frame = frame;
}

static inline void sprite_frames(u8 frame)
{
    struct SpriteNode *node;

    for (node = gWorld->sprites; node != 0; node = node->next)
        if (node->sprite != 0)
            node->sprite->frame = frame;
}

static inline void set_timer_mode(u16 *timer, u32 mode)
{
    ((struct HeroTimerBits *)timer)->mode = mode;
}

static inline void world_mode_set_bit4(void)
{
    struct WorldModeBits *m = (struct WorldModeBits *)&gWorld->mode;

    m->bit0 = 0;
    m->bit4 = 1;
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

NONMATCH("asm/code_080A7580/hero_pair_place_then_a_clear.s", void hero_pair_place_then_a_clear(struct HeroBody *body))
{
    struct World *world_mode;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    u8 *pose;
    s16 *span;
    s32 x;
    s32 y;
    s32 z;
    u32 flag_a;
    u32 flag_b;
    u32 flag_bit;
    u32 bits;
    u32 mask;
    u32 t;

    gWorld->unkE = 0;
    world_mode = gWorld;
    pose = gParty;
    t = (*(u32 *)(pose + 8) << 14) >> 26;
    mask = 1;
    flag_a = mask & t;
    flag_b = pose[0xB] >> 6;
    flag_bit = (((pose[0xC] & 0xF) << 2) | flag_b) & mask;
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
END_NONMATCH

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

NONMATCH("asm/code_080A7580/hero_a_step_wait_press.s", void hero_a_step_wait_press(struct HeroBody *body))
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    u16 *timer_a;
    s16 *step_ptr;
    s8 *ofs_ptr;
    u8 *flags_ptr;
    s32 dist_c;
    s32 dist_8;
    s32 end_limit;
    s32 rate;
    s32 dist;
    s32 limit_c;
    s32 limit_end;
    s32 reach;
    s32 mode;
    s32 end_c;
    s32 step_done;
    s32 reached;
    s32 limit_fx;
    s32 end_fx;
    s32 ofs_z;
    s32 z_a;
    s32 flag_a;
    s32 flag_c;
    s32 flag_full;
    u32 *flag9c;

    dist_c = 0x4000;
    step_motion(body_a);
    rate = (s16)body_a->rate;
    dist_8 = dist_c - (rate << 3);
    dist_c = dist_c - (rate << 2);
    end_limit = (((rate << 1) + rate) << 1) + 0x4000;
    if (body_a->progress < end_limit)
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;

    timer_a = &body_a->timer;
    step_ptr = &body_a->step;
    ofs_ptr = &body_b->ofsZ;
    flags_ptr = &body_a->flags;
    if ((*timer_a >> 12) == 0)
    {
        dist = dist_8;
        if (dist < 0)
            dist = 0;
        if (dist > 0x7FFF)
            dist = 0x7FFF;
        step_done = 0;
        if (!(*step_ptr > dist))
        {
            if (body_a->progress >= dist)
                step_done = 1;
        }

        if (step_done)
        {
            if (gWorld->display & 8)
            {
                chain_frames(body_a, 0x33);
                chain_frames(body_b, 0x33);
                chain_frames(partner, 0x33);
                sprite_frames(3);
            }
            else
            {
                chain_frames(body_a, 0x100);
                chain_frames(body_b, 0x100);
                chain_frames(partner, 0x100);
                sprite_frames(0x10);
            }
            sprite_objects_draw_obj_tiles(0x4C, 0xC);
        }

        limit_c = dist_c;
        limit_end = end_limit;
        if (limit_c < 0)
            limit_c = 0;
        if (limit_end < 0)
            limit_end = 0;
        if (limit_c > 0x4000)
            limit_c = 0x4000;
        if (limit_end > 0x7FFF)
            limit_end = 0x7FFF;
        reach = limit_c;
        if (limit_end < limit_c)
            reach = limit_end;

        reached = 0;
        if (body_a->progress >= reach)
        {
            if (*step_ptr <= limit_end)
                reached = 1;
        }

        if (reached)
        {
            set_body_part(body_a, 1, 0x18, 0, 0);
            if ((gSys.pressed & 1) != 0)
            {
                body_set_pose(3, body_a, 0x2E, 0);
                obj_free_parts(body_a);
                world_mode_set_bit4();
                if ((gWorld->mode & 8) == 0)
                {
                    if (gWorld->display & 8)
                    {
                        chain_frames(body_a, 0x100);
                        chain_frames(body_b, 0x100);
                        chain_frames(partner, 0x100);
                        sprite_frames(0x10);
                    }
                    else
                    {
                        chain_frames(body_a, 0x100);
                        chain_frames(body_b, 0x100);
                        chain_frames(partner, 0x100);
                        sprite_frames(0x10);
                    }
                    display_flush();
                }
                set_timer_mode(timer_a, 2);
            }
        }

        if ((*timer_a >> 12) != 0)
            goto effect;

        end_c = end_limit;
        if (end_c < 0)
            end_c = 0;
        if (end_c > 0x7FFF)
            end_c = 0x7FFF;
        if (*step_ptr > end_c)
            goto check_progress;
        if (body_a->progress < end_c)
            goto check_progress;

        world_mode_set_bit4();
        set_timer_mode(timer_a, 1);
        if (gWorld->display & 8)
        {
            chain_frames(body_a, 0x100);
            chain_frames(body_b, 0x100);
            chain_frames(partner, 0x100);
            sprite_frames(0x10);
        }
        else
        {
            chain_frames(body_a, 0x100);
            chain_frames(body_b, 0x100);
            chain_frames(partner, 0x100);
            sprite_frames(0x10);
        }
        display_flush();
        obj_free_parts(body_a);
        goto effect;

    check_progress:
        if (body_a->progress > end_limit)
            goto effect;
        if ((gSys.pressed & 1) != 0)
            goto press_on;
        if ((gSys.pressed & 3) != 0)
            goto press_on;
        goto effect;

    press_on:
        world_mode_set_bit4();
        if (gWorld->mode & 8)
            goto effect;
        set_timer_mode(timer_a, 1);
        if (gWorld->display & 8)
        {
            chain_frames(body_a, 0x100);
            chain_frames(body_b, 0x100);
            chain_frames(partner, 0x100);
            sprite_frames(0x10);
        }
        else
        {
            chain_frames(body_a, 0x100);
            chain_frames(body_b, 0x100);
            chain_frames(partner, 0x100);
            sprite_frames(0x10);
        }
        display_flush();
        obj_free_parts(body_a);
        body_set_pose(6, body_a, 0x2E, 0);
    }

effect:
    mode = *timer_a >> 12;
    if (mode == 0)
        goto abschluss;

    limit_fx = 0x4000;
    end_fx = end_limit;
    if (end_fx < 0)
        end_fx = 0;
    if (limit_fx > 0x4000)
        limit_fx = 0x4000;
    if (end_fx > 0x7FFF)
        end_fx = 0x7FFF;
    if (end_fx < limit_fx)
        limit_fx = end_fx;

    reached = 0;
    if (body_a->progress >= limit_fx)
    {
        if (*step_ptr <= end_fx)
            reached = 1;
    }
    if (!reached)
        goto abschluss;
    if (mode != 2)
        goto abschluss;

    flag9c = &body_a->unk9C;
    if (*flag9c == 0)
    {
        effect_spawn_at(0x2F7D, body_a->x / 0x100, body_a->y / 0x100, body_a->z / 0x100, body_a);
    }
    *flag9c = 1;

abschluss:
    dist = dist_8;
    if (dist < 0)
        dist = 0;
    if (dist > 0x7FFF)
        dist = 0x7FFF;
    flag_a = 0;
    if (!(*step_ptr > dist))
    {
        if (body_a->progress >= dist)
            flag_a = 1;
    }
    if (flag_a)
        set_cur_flags_both(0x40000000);

    dist = dist_c;
    if (dist < 0)
        dist = 0;
    if (dist > 0x7FFF)
        dist = 0x7FFF;
    flag_c = 0;
    if (!(*step_ptr > dist))
    {
        if (body_a->progress >= dist)
            flag_c = 1;
    }
    if (flag_c)
        set_cur_flags_both(0x40000000);

    flag_full = 0;
    if (!(*step_ptr > 0x4000))
    {
        if (body_a->progress >= 0x4000)
            flag_full = 1;
    }
    if (flag_full)
    {
        set_slot_value(0x7C, -1);
        sprite_set_action(body_a, 1, 0x2030, 0);
        body_a->sprite->flags |= 0x10;
    }

    ofs_z = *ofs_ptr - 9;
    z_a = body_a->z;
    if (z_a < body_a->targetZ)
    {
        if (z_a < body_b->z + (ofs_z << 8))
        {
            body_b->sprite->scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(z_a, ofs_z);
        }
    }

    if (*flags_ptr & 0x20)
        body->state = hero_b_sprite_grow_a_place;
}
END_NONMATCH
