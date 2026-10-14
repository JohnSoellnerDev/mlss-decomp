#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirrorFlags;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
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
    struct HeroBody *next;
    struct HeroNode *node;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
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
    u16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
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
    u8 filler_D8[0x110 - 0xD8];
    u8 flags110;
};

struct World
{
    u8 filler0[0x0A];
    u8 mode;
    u8 filler0B[0x0C - 0x0B];
    u8 flags;
    u8 filler0D[0x0E - 0x0D];
    u8 unk0E;
    u8 filler0F[0x58 - 0x0F];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void step_motion_checked(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void schedule_pose_task(struct HeroBody *partner);
void hero_a_stop_anim_if_active(struct HeroBody *body);
void hero_b_stop_anim_if_active(struct HeroBody *body);
void hero_b_anim_on_sprite(struct HeroBody *body);
void hero_b_begin_step_16(struct HeroBody *body);
void hero_a_go_to_b_target(struct HeroBody *body);
void hero_a_active_anim_24(struct HeroBody *body);
void hero_pair_place_mirror(struct HeroBody *body);
void hero_a_move_ba294(struct HeroBody *body);
void hero_a_place_at_target(struct HeroBody *body);
void hero_a_pair_finish(void);
void hero_a_approach_91e0(struct HeroBody *body);
void hero_a_step_slot_anim(struct HeroBody *body);
void hero_b_step_slot_anim(struct HeroBody *body);
u32 mt19937_next(void);
s32 sub_081DE850(s32 range, u32 seed);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

ASM_FUNC("asm/code_080B9F40/hero_pair_place_mirror.s", void hero_pair_place_mirror(struct HeroBody *body))

void hero_a_rate_wait(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *a = &world->heroA->body;
    struct HeroBody *b = &world->heroB->body;
    u32 next = (u32)b->state;

    if (next == 0)
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            a->progress = next;
            ((struct HeroFlagBits *)&a->flags)->active = 0;
        }
        body->state = hero_a_active_anim_24;
    }
}

void hero_a_rate_anim(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroA;
    struct HeroBody *a = &hero->body;

    if ((a->sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(a, 1, 0x2024, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            a->progress = 0;
            ((struct HeroFlagBits *)&a->flags)->active = 0;
        }
        a->timer &= 0xFFF;
        body->state = hero_a_move_ba294;
    }
}

ASM_FUNC("asm/code_080B9F40/hero_a_move_ba294.s", void hero_a_move_ba294(struct HeroBody *body))
