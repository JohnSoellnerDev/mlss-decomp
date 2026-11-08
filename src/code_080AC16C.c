#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 scale;
    u8 filler08[0x11 - 0x08];
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

struct HeroSpriteFlag4Bits
{
    u8 unk0 : 4;
    u8 bit4 : 1;
    u8 unk5 : 3;
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
    s16 flagsF6;
    u8 filler0F8[0x111 - 0xF8];
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
    struct HeroBody *chain;
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
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
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
    u8 filler0B;
    u8 flags0C;
    u8 filler0D;
    u8 enableByte;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct WorldNode *nodes;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gParty;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void schedule_pose_task(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
s32 resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, u32 cb);
void obj_slot_update(struct HeroBody *obj);

void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_step_16_b_idle_alt(struct HeroBody *body);
void hero_b_active_place_at_a_pose(struct HeroBody *body);

void hero_a_link_to_partner_fx(struct HeroBody *body);
void hero_a_active_set_targets(struct HeroBody *body);
void hero_a_active_fx_mirror(struct HeroBody *body);
void hero_pair_place_finish_sprite(struct HeroBody *body);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_active_step_12(struct HeroBody *body);
void hero_a_active_follow_b_aux(struct HeroBody *body);
void hero_a_step_threshold_c16c(struct HeroBody *body);
void hero_a_sprite_set_targets(struct HeroBody *body);
void hero_a_step_stage_fx(struct HeroBody *body);
void active_targets_hero_pair_place_finish_b(struct HeroBody *body);

void hero_b_pose_anim(struct HeroBody *body);
void noop_state_080acec0(struct HeroBody *body);
void hero_b_on_object(struct HeroBody *body);

struct WorldNode
{
    struct WorldNode *next;
    u8 filler4[0x08 - 0x04];
    struct WorldElem *elem;
};

struct WorldElem
{
    u8 filler0[0x20];
    u8 value;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

extern u8 gSys[];

struct StepWorldMode
{
    u8 low2 : 2;
    u8 mid2 : 2;
    u8 mode : 2;
    u8 high2 : 2;
} __attribute__((packed));

struct StepFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
} __attribute__((packed));

struct StepSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
} __attribute__((packed));

struct StepCcBits
{
    u8 low : 7;
    u8 high : 1;
} __attribute__((packed));

void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
u32 step_motion_scaled(struct HeroBody *body);

static inline void world_mode_or(u32 v)
{
    struct StepWorldMode *m = (struct StepWorldMode *)&gWorld->modeByte;
    m->mode = m->mode | v;
}

static inline void set_timer_mode(struct HeroBody *b, u32 m)
{
    b->timer = (b->timer & 0xFFF) | (m << 12);
}

NONMATCH("asm/code_080AC16C/hero_a_step_threshold_c16c.s", void hero_a_step_threshold_c16c(struct HeroBody *body))
{
    s32 thrHi;
    struct Hero *heroA;
    struct HeroBody *bodyA;
    struct HeroBody *bodyB;
    struct HeroBody *partner;
    s32 rate;
    s32 thrNear;
    s32 thrLo;
    s32 nearLim;
    s32 nearFlag;
    s32 stepLo;
    s32 stepHi;
    s32 farLim;
    s32 fullLim;
    s32 hi;
    s32 mode;
    s32 linkArg;
    s32 link;
    s32 x;
    s32 y;
    s32 z;

    heroA = gWorld->heroA;
    bodyA = &heroA->body;
    bodyB = &gWorld->heroB->body;
    partner = body->partner;
    thrHi = 0x4000;
    step_motion_scaled(bodyA);
    rate = (s16)bodyA->rate;
    thrNear = thrHi - (((rate << 1) + rate) << 1);
    thrLo = thrHi - (rate << 2);
    thrHi += rate << 1;

    if (bodyA->progress < thrHi)
        ((struct StepFlagBits *)&bodyA->flags)->active = 0;

    if ((bodyA->timer >> 12) != 0)
        goto tail;

    nearLim = thrNear;
    if (nearLim < 0)
        nearLim = 0;
    if (nearLim > 0x7FFF)
        nearLim = 0x7FFF;
    nearFlag = 0;
    if (bodyA->step <= nearLim)
    {
        if (bodyA->progress >= nearLim)
            nearFlag = 1;
    }
    if (nearFlag)
    {
        if (gWorld->flags0C & 8)
        {
        {
            struct HeroBody *p = bodyA;
            while (p != 0)
            {
                p->frame = 0x33;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = bodyB;
            while (p != 0)
            {
                p->frame = 0x33;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = partner;
            while (p != 0)
            {
                p->frame = 0x33;
                p = p->chain;
            }
        }
        {
            struct WorldNode *n = gWorld->nodes;
            while (n != 0)
            {
                if (n->elem != 0)
                    n->elem->value = 3;
                n = n->next;
            }
        }
        }
        else
        {
        {
            struct HeroBody *p = bodyA;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = bodyB;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = partner;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct WorldNode *n = gWorld->nodes;
            while (n != 0)
            {
                if (n->elem != 0)
                    n->elem->value = 0x10;
                n = n->next;
            }
        }
        }
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    stepLo = thrLo;
    stepHi = thrHi;
    if (stepLo < 0)
        stepLo = 0;
    if (thrHi < 0)
        stepHi = 0;
    if (stepLo > 0x4000)
        stepLo = 0x4000;
    if (stepHi > 0x7FFF)
        stepHi = 0x7FFF;
    if (stepHi < stepLo)
        stepLo = stepHi;

    if (bodyA->progress >= stepLo && bodyA->step <= stepHi)
    {
        set_body_part(bodyA, 1, 0x18, 0, 0);
        if (((struct SysKeys3E *)gSys)->keys & 1)
        {
            body_set_pose(2, bodyA, 0x2E, 0);
            obj_free_parts(bodyA);
            world_mode_or(1);
            if ((((struct StepWorldMode *)&gWorld->modeByte)->mid2 & 2) == 0)
            {
                if (gWorld->flags0C & 8)
                {
                    {
                        struct HeroBody *p = bodyA;
                        while (p != 0)
                        {
                            p->frame = 0x100;
                            p = p->chain;
                        }
                    }
                    {
                        struct HeroBody *p = bodyB;
                        while (p != 0)
                        {
                            p->frame = 0x100;
                            p = p->chain;
                        }
                    }
                    {
                        struct HeroBody *p = partner;
                        while (p != 0)
                        {
                            p->frame = 0x100;
                            p = p->chain;
                        }
                    }
                    {
                        struct WorldNode *n = gWorld->nodes;
                        while (n != 0)
                        {
                            if (n->elem != 0)
                                n->elem->value = 0x10;
                            n = n->next;
                        }
                    }
                }
                else
                {
                    {
                        struct HeroBody *p = bodyA;
                        while (p != 0)
                        {
                            p->frame = 0x100;
                            p = p->chain;
                        }
                    }
                    {
                        struct HeroBody *p = bodyB;
                        while (p != 0)
                        {
                            p->frame = 0x100;
                            p = p->chain;
                        }
                    }
                    {
                        struct HeroBody *p = partner;
                        while (p != 0)
                        {
                            p->frame = 0x100;
                            p = p->chain;
                        }
                    }
                    {
                        struct WorldNode *n = gWorld->nodes;
                        while (n != 0)
                        {
                            if (n->elem != 0)
                                n->elem->value = 0x10;
                            n = n->next;
                        }
                    }
                }
                display_flush();
            }
            set_timer_mode(bodyA, 2);
        }
    }

    if ((bodyA->timer >> 12) != 0)
        goto tail;

    farLim = thrHi;
    if (farLim < 0)
        farLim = 0;
    if (farLim > 0x7FFF)
        farLim = 0x7FFF;
    if (bodyA->step <= farLim && bodyA->progress >= farLim)
    {
        world_mode_or(1);
        set_timer_mode(bodyA, 1);
        if (gWorld->flags0C & 8)
        {
            {
                struct HeroBody *p = bodyA;
                while (p != 0)
                {
                    p->frame = 0x100;
                    p = p->chain;
                }
            }
            {
                struct HeroBody *p = bodyB;
                while (p != 0)
                {
                    p->frame = 0x100;
                    p = p->chain;
                }
            }
            {
                struct HeroBody *p = partner;
                while (p != 0)
                {
                    p->frame = 0x100;
                    p = p->chain;
                }
            }
            {
                struct WorldNode *n = gWorld->nodes;
                while (n != 0)
                {
                    if (n->elem != 0)
                        n->elem->value = 0x10;
                    n = n->next;
                }
            }
        }
        else
        {
            {
                struct HeroBody *p = bodyA;
                while (p != 0)
                {
                    p->frame = 0x100;
                    p = p->chain;
                }
            }
            {
                struct HeroBody *p = bodyB;
                while (p != 0)
                {
                    p->frame = 0x100;
                    p = p->chain;
                }
            }
            {
                struct HeroBody *p = partner;
                while (p != 0)
                {
                    p->frame = 0x100;
                    p = p->chain;
                }
            }
            {
                struct WorldNode *n = gWorld->nodes;
                while (n != 0)
                {
                    if (n->elem != 0)
                        n->elem->value = 0x10;
                    n = n->next;
                }
            }
        }
        display_flush();
        obj_free_parts(bodyA);
        goto tail;
    }

    if (bodyA->progress > thrHi)
        goto tail;
    if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
        goto tail;
    world_mode_or(1);
    if (((struct StepWorldMode *)&gWorld->modeByte)->mid2 & 2)
        goto tail;
    set_timer_mode(bodyA, 1);
    if (gWorld->flags0C & 8)
    {
        {
            struct HeroBody *p = bodyA;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = bodyB;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = partner;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct WorldNode *n = gWorld->nodes;
            while (n != 0)
            {
                if (n->elem != 0)
                    n->elem->value = 0x10;
                n = n->next;
            }
        }
    }
    else
    {
        {
            struct HeroBody *p = bodyA;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = bodyB;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct HeroBody *p = partner;
            while (p != 0)
            {
                p->frame = 0x100;
                p = p->chain;
            }
        }
        {
            struct WorldNode *n = gWorld->nodes;
            while (n != 0)
            {
                if (n->elem != 0)
                    n->elem->value = 0x10;
                n = n->next;
            }
        }
    }
    display_flush();
    obj_free_parts(bodyA);
    body_set_pose(6, bodyA, 0x2E, 0);

tail:
    mode = bodyA->timer >> 12;
    if (mode != 0)
    {
        hi = thrHi;
        if (hi < 0)
            hi = 0;
        fullLim = 0x4000;
        if (fullLim > 0x4000)
            fullLim = 0x4000;
        if (hi > 0x7FFF)
            hi = 0x7FFF;
        if (hi < fullLim)
            fullLim = hi;
        if (bodyA->progress >= fullLim && bodyA->step <= hi)
        {
            linkArg = (mode > 1) ? 0x040B1888 : 0x040B1788;
            ((struct StepCcBits *)&bodyA->cc)->high = 0;
            bodyA->scale = 0x100;
            link = resolve_contact_action(bodyA, partner, 0, 0, linkArg);
            bodyA->count++;
            if (!(linkArg & 0x10000) && link == 1)
            {
                obj_slot_update(bodyA);
                return;
            }
            x = bodyA->posX / 0x100;
            y = bodyA->posY / 0x100;
            z = bodyA->posZ / 0x100;
            if (mode > 1)
                effect_spawn_at(0x2E47, x, y, z, partner);
            else
                effect_spawn_at(0x70B, x, y, z, partner);
            set_timer_mode(bodyA, 0);
            ((struct StepFlagBits *)&bodyA->flags)->active = 1;
        }
    }

    if (((struct StepFlagBits *)&bodyA->flags)->active == 0)
        return;
    x = bodyA->posX / 0x100;
    y = bodyA->posY / 0x100;
    z = bodyA->posZ / 0x100;
    effect_spawn_at(0x2FF9, x, y, z, bodyA);
    mode = bodyA->mode & 6;
    if (mode == 2 || mode == 4)
    {
        sprite_set_action(bodyA, 0xB, 0x2030, 0);
        ((struct StepSpriteBits *)&bodyA->sprite->flags)->mode = 0;
    }
    mode = bodyB->mode & 6;
    if (mode == 2 || mode == 4)
    {
        sprite_set_action(bodyB, 0, 0x204D, 0);
        ((struct StepSpriteBits *)&bodyB->sprite->flags)->mode = 0;
    }
    body->state = hero_a_sprite_set_targets;
}
END_NONMATCH

void hero_a_sprite_set_targets(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a,
                          (body_a->targetX / 0x100) + body_a->ofsX - 0x3C,
                          (body_a->targetY / 0x100) + body_a->ofsY, 0, -1);
            body_a->height = (body_a->targetZ / 0x100) + 0x28;
            set_motion_offsets(body_a, 0x500);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xA, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_step_stage_fx;
    }
}

NONMATCH("asm/code_080AC16C/hero_a_step_stage_fx.s", void hero_a_step_stage_fx(struct HeroBody *body))
{
    struct HeroBody *body_a;
    struct Hero *hero_a = gWorld->heroA;
    s32 stage;
    s32 limit;

    body_a = &hero_a->body;
    step_motion(body_a);
    limit = 0x1555;
    if (hero_a->body.step > limit)
    {
        stage = 0;
    }
    else
    {
        if (hero_a->body.progress < limit)
        {
            stage = 0;
        }
        else
        {
            stage = 1;
        }
    }
    if (stage != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 9, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
    }
    limit = 0x2AAA;
    if (body_a->step > limit)
    {
        stage = 0;
    }
    else
    {
        if (body_a->progress < limit)
        {
            stage = 0;
        }
        else
        {
            stage = 1;
        }
    }
    if (stage != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 3, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
    }
    if (body_a->flags & 0x20)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = active_targets_hero_pair_place_finish_b;
    }
}
END_NONMATCH
