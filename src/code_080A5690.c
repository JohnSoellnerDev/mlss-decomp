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

struct HeroNode
{
    u16 x;
    u16 y;
    u8 filler4[0x0E - 0x04];
    u16 z;
};

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gUnk_0300034C;

// Soft-float constants as the helpers receive them: the high word in the first register.
#define F64(hi, lo) ((((u64)(lo)) << 32) | (hi))

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
    u8 filler0C[0x24 - 0x0C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    u8 filler30[0x34 - 0x30];
    struct HeroNode *node;
    s32 targetX;
    s32 targetY;
    u8 filler40[0x4C - 0x40];
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
    u8 filler88[0xAC - 0x88];
    s16 step;
    u16 timer;
    u8 filler_B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler_C5[0xCA - 0xC5];
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
    u8 filler0B[0x70 - 0x0B];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern const u8 gPartShapeRow02[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void schedule_pose_task(struct HeroBody *partner);
void hero_b_begin_step_sprite(struct HeroBody *body);
void hero_b_sprite_step_2(struct HeroBody *body);
void hero_b_pose_to_partner_rand_6464(struct HeroBody *body);
void hero_a_sprite_step_18_5fcc(struct HeroBody *body);
void hero_a_begin_step_sprite_15_b(struct HeroBody *body);
void hero_a_prepare_step_b(struct HeroBody *body);
void hero_a_ready_step_6044(struct HeroBody *body);
void hero_a_go_to_partner_random(struct HeroBody *body);
void hero_a_spawn_node(struct HeroBody *body);
void hero_a_link_partner_6364(struct HeroBody *body);
void hero_a_link_partner_62f8(struct HeroBody *body);
void hero_a_advance_place_pair_5d20(struct HeroBody *body);
void hero_a_advance_place_pair_6100(struct HeroBody *body);
void hero_a_node_step_with_input(struct HeroBody *body);
void hero_a_drop_node_5f18(struct HeroBody *body);
void hero_a_release_node(struct HeroBody *body);
void sprite_entry_update(struct HeroNode *node);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
u32 mt19937_next(void);
s32 sub_081DE850(s32 range, u32 seed);
u64 __floatsidf(s32 n);
u64 __adddf3(u64 a, u64 b);
u64 __muldf3(u64 a, u64 b);
s32 __fixdfsi(u64 a);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void hero_b_node_input_step_67a8(struct HeroBody *body);
void hero_b_spawn_node_66c8(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void *spawn_obj_by_id(u32 id, u32 a, u32 b, u32 c);
void hero_pair_place_b_then_a(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void obj_release_node(void *node);

ASM_FUNC("asm/code_080A5690/hero_a_node_step_with_input.s", void hero_a_node_step_with_input(struct HeroBody *body))

ASM_FUNC("asm/code_080A5690/hero_a_node_step_with_input_b.s", void hero_a_node_step_with_input_b(void))
