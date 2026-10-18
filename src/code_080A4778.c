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

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x114 - 0xEE];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x24 - 0x0C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    u8 filler30[0x38 - 0x30];
    s32 targetX;
    s32 targetY;
    u8 filler40[0x4C - 0x40];
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
    u8 filler88[0xAC - 0x88];
    s16 step;
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
    u8 filler0B[0x70 - 0x0B];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gSys[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void schedule_pose_task(struct HeroBody *obj);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);

void hero_a_sprite_step_2(struct HeroBody *body);
void hero_a_begin_slow_step(struct HeroBody *body);
void hero_a_link_to_partner(struct HeroBody *body);
void hero_a_step_once(struct HeroBody *body);
void hero_b_begin_step(struct HeroBody *body);
void hero_b_step_with_input(struct HeroBody *body);
void hero_a_step_with_input(struct HeroBody *body);
void hero_pair_place_a_then_b(struct HeroBody *body);
void hero_pair_place_finish(struct HeroBody *body);
void pair_meet_hero_pair_place_finish_b(struct HeroBody *body);
void hero_b_show_anim_5(struct HeroBody *body);
void hero_a_show_anim_5(struct HeroBody *body);
void hero_b_link_to_partner_b(struct HeroBody *body);
void hero_a_link_to_partner_b(struct HeroBody *body);
void hero_b_go_to_partner(struct HeroBody *body);
void hero_a_show_anim_0(struct HeroBody *body);
void hero_b_show_anim_0(struct HeroBody *body);
void hero_b_begin_step_sprite_15(struct HeroBody *body);
void hero_b_sprite_step_20(struct HeroBody *body);
void hero_b_prepare_step(struct HeroBody *body);
void hero_b_begin_slow_step(struct HeroBody *body);
void hero_a_step_sprite_18(struct HeroBody *body);
void hero_a_prepare_step(struct HeroBody *body);
void hero_a_begin_step_sprite_15(struct HeroBody *body);

ASM_FUNC("asm/code_080A4778/hero_b_step_with_input.s", void hero_b_step_with_input(struct HeroBody *body))
