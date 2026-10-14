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
    u8 layer;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 high : 1;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

struct SpriteListItem
{
    struct SpriteListItem *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct WorldFlagBits
{
    u8 unk0 : 6;
    u8 bit6 : 1;
    u8 unk7 : 1;
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
    u8 filler0B[0x0C - 0x0B];
    u8 flags;
    u8 filler0D[0x0E - 0x0D];
    u8 enableByte;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct SpriteListItem *sprites;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

extern struct World *gUnk_03000FD8;
extern struct KeyState gUnk_0300034C;

void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void start_action_4(struct HeroBody *body);
void hero_a_pick_partner(struct HeroBody *body);
s32 resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, u32 cb);
void obj_slot_update(struct HeroBody *obj);
void obj_free_parts(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);

void hero_pair_settle(void);
void hero_a_place_at_b(struct HeroBody *body);
void hero_a_join_b(struct HeroBody *body);
void hero_b_step_frames_7954(struct HeroBody *body);
void hero_b_settle_on_partner(struct HeroBody *body);
void hero_a_follow_b(struct HeroBody *body);
void hero_pair_link_fx(struct HeroBody *body);
void hero_pair_settle_b(struct HeroBody *body);
void hero_pair_confirm_85ec(struct HeroBody *body);

void hero_pair_settle(void)
{
    struct HeroBody *a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *b = &gUnk_03000FD8->heroB->body;

    if ((b->sprite->flags & 8) != 0)
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            place_targets(a, a->data->targetX / 0x100, a->data->targetY / 0x100,
                          a->data->targetH / 0x100, -1);
            a->height = a->targetZ / 0x100 + 0x28;
            set_motion_offsets(a, 0x7CC);
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->state = 0;
            start_action_4(a);
        }
        set_slot_value(0x2B, -1);
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            place_targets(b, b->data->targetX / 0x100, b->data->targetY / 0x100,
                          b->data->targetH / 0x100, -1);
            b->height = b->targetZ / 0x100 + 0x28;
            set_motion_offsets(b, 0x7CC);
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            b->state = 0;
            start_action_4(b);
        }
    }
}

void hero_a_place_at_b(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    s32 neg;
    u8 mode;

    mode = gUnk_03000FD8->modeByte;
    gUnk_03000FD8->modeByte = mode | 0xC0;
    a = &gUnk_03000FD8->heroA->body;
    b = &gUnk_03000FD8->heroB->body;
    find_frame_entry(a, 0);
    set_slot_value(0x2B, -1);
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        neg = -a->unk47;
        place_targets(a,
                      (b->targetX / 0x100) + b->ofsX,
                      (b->targetY / 0x100) + b->ofsY,
                      (b->targetZ / 0x100) + neg, -1);
        set_motion_start(a, 0x300);
    }
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        if (a->data->poseIndex == -1)
        {
            sprite_set_action(a, 5, 0x2000, 0);
        }
        else
        {
            sprite_set_action(a, 5, 0x204D, 0);
        }
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
    }
    body->state = hero_a_join_b;
}

void hero_a_join_b(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *a = &hero_a->body;
    struct HeroBody *b = &gUnk_03000FD8->heroB->body;
    s32 x;
    s32 y;
    s32 z;

    step_motion(a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(a, 2, 0x2031, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            sprite_set_action(b, 2, 0x2060, 0);
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            place_targets(b,
                          (b->targetX / 0x100) + b->ofsX + 0x10,
                          (b->targetY / 0x100) + b->ofsY,
                          0, -1);
            b->height = b->targetZ / 0x100 + 4;
            set_motion_offsets(b, 0x600);
        }
        x = b->posX;
        a->posX = x;
        y = b->posY;
        a->posY = y;
        z = b->posZ;
        a->posZ = z;
        effect_spawn_at(0x2F7D, x / 0x100, y / 0x100, z / 0x100, a);
        b->timer &= 0xFFF;
        body->state = hero_b_step_frames_7954;
    }
}

ASM_FUNC("asm/code_080B75B4/hero_b_step_frames_7954.s", void hero_b_step_frames_7954(struct HeroBody *body))
