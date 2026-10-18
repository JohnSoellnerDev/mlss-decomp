#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
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

struct PoseFlagBits
{
    u16 unk0 : 5;
    u16 field5 : 5;
    u16 unk10 : 6;
};

struct PoseRecord
{
    u8 filler0[0x46];
    u8 byte46;
    u8 filler47;
    struct PoseFlagBits word48;
};

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
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
    struct HeroBody *next;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
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
    u16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
    u16 timer;
    s16 jitterX;
    s16 jitterY;
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

struct HeroNode
{
    struct HeroNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 flagsC;
    u8 filler0D;
    u8 byte0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroNode *nodes;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gSys[];

void hero_b_set_rate_if_idle(struct HeroBody *body);
void hero_b_visible_anim_b_rate_a_alt(struct HeroBody *body);
void hero_a_active_link_place_finish(struct HeroBody *body);
void hero_b_visible_anim_b_rate_a(struct HeroBody *body);
void hero_pair_place_and_finish(struct HeroBody *body);
void hero_pair_b_active_slot_anim_next(struct HeroBody *body);
void hero_a_active_step_fx_anim_alt(struct HeroBody *body);
void hero_a_jitter_step_alt_7d58(struct HeroBody *body);
void hero_a_sprite_active_place_target_alt(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_a_pair_step_83f8(struct HeroBody *body);
void hero_a_clear_partner_flag_anim(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void hero_pair_pose_setup_place_finish(struct HeroBody *body);
void jitter_place_a_hero_pair_place_both_finish_alt(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void set_slot_value(u32 id, s32 value);
void state_stack_advance_progress(struct HeroBody *body);
void hero_b_visible_set_rate_20(struct HeroBody *body);
void hero_b_wait_idle(struct HeroBody *body);
void enter_finish_flag(struct HeroBody *body);
void hero_b_step_c94a0(struct HeroBody *body);
void obj_stop(struct HeroBody *obj);
void schedule_pose_task(struct HeroBody *obj);
void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);

void hero_a_place_near_partner_if_b_active(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    state_stack_advance_progress(body_b);
    step_motion(body_a);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX - 10;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY + 3;
            tz = partner->targetZ;
            if (tz < 0)
                tz += 0xFF;
            z = (tz >> 8) + partner->ofsZ - 0x15;
            place_targets(body_a, x, y, z, -1);
            set_motion_start(body_a, 0x600);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 4, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_jitter_step_alt_7d58;
    }
}

ASM_FUNC("asm/code_080C7C58/hero_a_jitter_step_alt_7d58.s", void hero_a_jitter_step_alt_7d58(struct HeroBody *body))

ASM_FUNC("asm/code_080C7C58/hero_a_pair_step_83f8.s", void hero_a_pair_step_83f8(struct HeroBody *body))
