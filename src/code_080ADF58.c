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
    s8 *offsets;
    u8 filler0E8[0xEC - 0xE8];
    s16 poseIndex;
    u8 filler0EE[0xF4 - 0xEE];
    u8 count;
    u8 filler0F5[0xF6 - 0xF5];
    s16 unkF6;
    u8 filler0F8[0x110 - 0xF8];
    u8 unk110;
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
    s32 x;
    s32 y;
    s32 z;
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
    u8 order;
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
    u8 filler94[0x9C - 0x94];
    u32 place;
    s32 counter;
    u32 unkA4;
    s32 unkA8;
    s16 step;
    u16 timer;
    u8 filler0B0[0xB2 - 0xB0];
    u16 vel;
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

struct World
{
    u8 filler0[0x0C];
    u8 flags;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *slots[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct WorldLink *list;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

struct WorldLink
{
    struct WorldLink *next;
    u32 unk4;
    void *obj;
};

struct ObjFlags
{
    u8 filler0[0x20];
    u8 flags;
};

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gUnk_0300034C;

extern struct World *gUnk_03000FD8;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

void hero_a_active_anims(struct HeroBody *body);
void hero_b_place_from_a(struct HeroBody *body);
void hero_b_step_end_d354(struct HeroBody *body);
void hero_b_enter_side(struct HeroBody *body);
void hero_b_join_partner(struct HeroBody *body);
void sub_080B3988(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
u32 resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void obj_slot_update(struct HeroBody *obj);
void display_flush(void);
void obj_free_parts(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
struct HeroSprite *find_lowest_order_pose(struct HeroBody *body);
void pair_action_b_follow_a(struct HeroBody *body);
void pair_action_b_follow_partner(struct HeroBody *body);
void pair_action_a_hold(struct HeroBody *body);
void pair_action_a_start(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void hero_b_to_partner_slow(struct HeroBody *body);
void pair_action_pick_partner(struct HeroBody *body);
u32 mt19937_next(void);
s32 sub_081DE850(s32 a, u32 b);
void obj_release(struct HeroBody *obj);
void hero_b_to_partner_fast(struct HeroBody *body);
void pair_finish_and_rebuild(struct HeroBody *body);

ASM_FUNC("asm/code_080ADF58/pair_regroup.s", void pair_regroup(void))

ASM_FUNC("asm/code_080ADF58/pair_action_a_start.s", void pair_action_a_start(struct HeroBody *body))

ASM_FUNC("asm/code_080ADF58/pair_action_a_hold.s", void pair_action_a_hold(struct HeroBody *body))

ASM_FUNC("asm/code_080ADF58/pair_action_pick_partner.s", void pair_action_pick_partner(struct HeroBody *body))
