#include "global.h"
#include "ram_routines.h"
#include "pair_shot.h"

struct ScrollNode
{
    struct ScrollNode *next;
    s32 value;
};

struct SpriteNode
{
    struct SpriteNode *next;
    struct SpriteEntry *unk4;
    struct ScrollNode **unk8;
    u8 unkC;
};

struct SpriteObj;

struct SpriteEntry
{
    u8 filler0[0xC];
    u16 unkC;
    u8 filler0E[0x11 - 0xE];
    u8 unk11;
    u8 flags;
    u8 filler13[0x28 - 0x13];
    struct SpriteObj *owner;
    u8 filler2C[0x38 - 0x2C];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
};

struct SpriteDesc
{
    u8 filler0[0xC];
    u8 order;
};

struct SpriteObj
{
    u8 filler0[0x4];
    struct SpriteDesc *desc;
    struct SpriteEntry *entry;
    struct ScrollNode *list;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x20 - 0x1C];
    s32 unk20;
    u8 filler24[0x28 - 0x24];
    struct SpriteObj *mainObj;
    struct SpriteObj *owner;
    struct SpriteObj *child;
    u8 filler34[0x38 - 0x34];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x4C - 0x44];
    void (*callback)(struct SpriteObj *);
    u8 filler50[0x58 - 0x50];
    void *unk58;
    u8 filler5C[0x60 - 0x5C];
    void (*unk60)(struct SpriteObj *);
    u8 filler64[0x68 - 0x64];
    void (*unk68)(struct SpriteObj *);
    s32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 filler75;
    u8 flags76;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 unk7A;
    u16 unk7C;
    u8 flags7E;
    u8 filler7F;
    u32 target;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    u8 filler90[0x9C - 0x90];
    u32 mode;
    s32 unkA0;
    s32 phaseTimer;
    s32 phase;
    u16 timer;
    u16 timer2;
    u8 filler0B0[0xB2 - 0xB0];
    s16 unkB2;
    u8 filler0B4[0xD8 - 0xB4];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    u8 filler0E4[0xEC - 0xE4];
    s16 unkEC;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x113 - 0xF8];
    u8 unk113;
    u8 filler114[0x124 - 0x114];
    u16 unk124;
};

struct EntryBits12
{
    u8 b0 : 1;
    u8 b12 : 2;
    u8 rest : 5;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct EntryBit4
{
    u8 low4 : 4;
    u8 bit4 : 1;
    u8 high3 : 3;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct SpriteWorld
{
    u8 filler0[0x58];
    struct SpriteObj *slots[8];
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct SpriteWorld *gWorld;
extern u8 gUnk_03000ED0;
extern u32 gUnk_03000F6C;
extern u8 gUnk_03000F68;
extern struct SpriteObj *gUnk_03000F60;
extern s16 gUnk_03000EC8[];
extern s16 gUnk_03000ECC[];
extern u8 gSys[];
extern u32 gUnk_03000F64;
extern s32 gLinkOffsetX[];
extern s32 gLinkOffsetH[];

extern void (*gUnk_03000ED4)(struct SpriteObj *);
extern void (*gUnk_03000ED8)(struct SpriteObj *);
extern u8 gUnk_03000F5C;
extern u8 gUnk_03000F5D;
extern void (*gUnk_03000EDC[])(struct SpriteObj *);
extern void (*gUnk_03000F1C[])(struct SpriteObj *);
void actor_link_anim_steps(struct SpriteObj *obj);
void set_slot_fields(u32 idx, u8 level, u8 period);

void sprite_set_action(struct SpriteObj *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void obj_release(void *obj);
void obj_list_remove(void *obj);
void obj_list_push_default(struct SpriteObj *obj);
void remove_if_active(struct SpriteObj *obj);
void set_anim_3_or_8(struct SpriteObj *obj);
void check_and_place(struct SpriteObj *obj);
void start_motion(struct SpriteObj *obj);
void anim_from_phase_plus5(struct SpriteObj *obj);
void on_arrive(struct SpriteObj *obj);
struct SpriteObj *obj_spawn(struct SpriteObj *parent, u32 kind, s32 x, s32 y, s32 z, u32 flag, u32 one);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, void *ent);
void partner_link_update(void *ent, void *kind, u32 arg);
u32 mt19937_next(void);
struct SpriteNode *sprite_node_take(void);
struct SpriteNode *sprite_node_alloc(s32 x, s32 y, s32 z);

void mode_counter_tick(struct SpriteObj *obj);
void random_spawn_pos(struct SpriteObj *obj);
void snap_to_world_slot(struct SpriteObj *obj);
void start_timer_anim4(struct SpriteObj *obj);
void end_without_target(struct SpriteObj *obj);
void set_anim_by_alt_flag(struct SpriteObj *obj);
void set_cb_if_no_target(struct SpriteObj *obj);
void set_anim1_mode4(struct SpriteObj *obj);
void pick_side_random_then_anim(struct SpriteObj *obj);
void timer_reset_pos_and_cb(struct SpriteObj *obj);
void tick_mode_and_remove_when_low(struct SpriteObj *obj);
void remove_object_b(struct SpriteObj *obj);
void anim0_clear_cb_if_active_b(struct SpriteObj *obj);
void anim0_clear_cb_if_active_c(struct SpriteObj *obj);

NONMATCH("asm/code_08078708/unlink_actor_pair.s", void unlink_actor_pair(struct SpriteObj *obj))
{
    s32 t;
    u16 *phase;
    struct SpriteObj *actorA;
    struct SpriteObj *actorB;
    struct SpriteObj *child;

    phase = &obj->timer2;
    t = *phase - 1;
    *phase = t;
    if ((s16)t <= 0)
    {
        set_slot_value(0x54, -1);
        *phase = 0xA;
    }
    if (obj->child != 0)
        return;
    set_slot_fields(1, 0xFF, 0);
    ((u8 *)gWorld->trackedObj)[0x770] = 0;
    actorA = (struct SpriteObj *)((u8 *)gWorld->slots[6] + 8);
    actorB = (struct SpriteObj *)((u8 *)gWorld->slots[7] + 8);
    if (actorA->callback == actor_link_anim_steps)
    {
        actorA->callback = gUnk_03000ED4;
        ((struct EntryBit4 *)&actorA->entry->flags)->bit4 = 0;
    }
    if (actorB->callback == actor_link_anim_steps)
    {
        actorB->callback = gUnk_03000ED8;
        ((struct EntryBit4 *)&actorB->entry->flags)->bit4 = 0;
    }
    gUnk_03000F5C = 0;
    gUnk_03000F5D = 0;
    child = actorA->child;
    while (child != 0)
    {
        if (child->callback == actor_link_anim_steps)
        {
            ((struct EntryBit4 *)&child->entry->flags)->bit4 = 0;
            child->callback = gUnk_03000EDC[gUnk_03000F5C++];
        }
        child = child->child;
    }
    child = actorB->child;
    while (child != 0)
    {
        if (child->callback == actor_link_anim_steps)
        {
            ((struct EntryBit4 *)&child->entry->flags)->bit4 = 0;
            child->callback = gUnk_03000F1C[gUnk_03000F5D++];
        }
        child = child->child;
    }
    obj->callback = 0;
}
END_NONMATCH

s32 update_positions_to_target(struct SpriteObj *obj);
void link_actor_if_free(struct SpriteObj *obj);
void ramp_values_then_remove(struct SpriteObj *obj);
// State of a fired pair shot (mode: which hero it flies at, unkA0: the PairShotPattern): moves
// it on, then spawns a second 0x4025 at the shot's target that takes over the shot's index,
// pattern, owner and child list and checks for 30 frames whether its hero is hit
// (link_actor_if_free); for a shot that flies past its hero, it sets the new object's unk10 to
// 0x10000. The shot then runs ramp_values_then_remove for 40 frames.
void spawn_child_4025_adopt(struct SpriteObj *obj)
{
    struct SpriteObj *parent;
    struct SpriteObj *parentBody;
    struct SpriteObj *hitCheck;

    obj->timer2 += 0x400;
    update_positions_to_target(obj);
    parent = obj->mainObj;
    parentBody = (struct SpriteObj *)((u8 *)parent + 8);
    hitCheck = obj_spawn(parent, 0x4025, gUnk_03000EB0[obj->mode] / 0x100, gUnk_03000EB8[obj->mode] / 0x100,
                         gUnk_03000EC0[obj->mode] / 0x100, 0, 1);
    sprite_set_action(hitCheck, 1, 0, 0);
    hitCheck->unk74 = 0xFF;
    {
        u32 flags = hitCheck->flags79;

        hitCheck->flags79 = flags | 1;
    }
    {
        u32 flags = hitCheck->flags76;

        hitCheck->flags76 = flags | 6;
    }
    hitCheck->owner = parentBody->owner;
    hitCheck->unk6C = parentBody->unk6C;
    ((struct ObjFlags77 *)hitCheck)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)hitCheck)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
    ((struct EntryBits12 *)&hitCheck->entry->flags)->b12 = 0;
    hitCheck->callback = link_actor_if_free;
    hitCheck->timer = 30;
    hitCheck->child = obj->child;
    hitCheck->mode = obj->mode;
    hitCheck->unkA0 = obj->unkA0;
    hitCheck->child = obj->child;
    obj->child = hitCheck;
    obj->timer = 40;
    if (obj->mode == 0)
    {
        if (obj->unkA0 == PAIR_SHOT_PAST_A || obj->unkA0 == PAIR_SHOT_PAST_BOTH)
            hitCheck->unk10 = 0x10000;
    }
    else
    {
        switch (obj->unkA0)
        {
        case PAIR_SHOT_PAST_B:
        case PAIR_SHOT_PAST_BOTH:
            hitCheck->unk10 = 0x10000;
            break;
        }
    }
    obj->callback = ramp_values_then_remove;
}

static inline s32 scale_by_18(s32 h)
{
    return ((h << 3) + h) << 1;
}

/* State of a pair shot after it hit: keeps moving it on and counts its timer down; over the last
 * 18 frames it shrinks the impact sprite to nothing by writing its OAM affine matrix (in the
 * current half of the shadow OAM at gSys + 0x80) as the shot's base matrix (gUnk_03000EC8:
 * PA/PD, gUnk_03000ECC: PB/-PC) times 18 / timer, and releases the shot on its last frame. */
void ramp_values_then_remove(struct SpriteObj *obj)
{
    s32 left;
    s32 pa;
    s32 pb;
    u32 bufOffset;
    u16 *oam;
    s32 pc;

    update_positions_to_target(obj);
    left = obj->timer - 1;
    obj->timer = left;
    if ((s16)left <= 18)
    {
        pa = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scale_by_18(gUnk_03000EC8[obj->mode]), (s16)obj->timer);
        pb = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scale_by_18(gUnk_03000ECC[obj->mode]), (s16)obj->timer);
        /* gSys[0x888] bit 3 picks the shadow OAM half; each affine group is 0x20 bytes */
        bufOffset = (((u32)gSys[0x888] << 28) >> 31) << 10;
        oam = (u16 *)(gSys + 0x80 + bufOffset + (((u32)obj->desc->order << 27) >> 22));
        (void)*(volatile u16 *)&oam[3];
        oam[3] = pa;
        (void)*(volatile u16 *)&oam[7];
        oam[7] = pb;
        pc = -pb;
        (void)*(volatile u16 *)&oam[11];
        oam[11] = pc;
        (void)*(volatile u16 *)&oam[15];
        oam[15] = pa;
        if ((s16)obj->timer == 1)
            obj_release(obj);
    }
}
/* State of the hit check a pair shot spawns (spawn_child_4025_adopt), for `timer` frames: hits
 * its hero (world slot 6 for mode 0, slot 7 for mode 1) through partner_link_update unless the
 * hero's flags7E bit 7 is set. A shot that flies past its hero (PairShotPattern) hits only a hero
 * higher than 0x1000 (jumped into it), any other shot only a hero on the ground (height 0).
 * Releases itself once the timer runs out. */
void link_actor_if_free(struct SpriteObj *obj)
{
    struct SpriteObj *heroA;
    struct SpriteObj *heroB;
    struct SpriteObj *bodyA;
    struct SpriteObj *bodyB;
    s32 pattern;
    s32 left;

    heroA = gWorld->slots[6];
    bodyA = (struct SpriteObj *)((u8 *)heroA + 8);
    heroB = gWorld->slots[7];
    bodyB = (struct SpriteObj *)((u8 *)heroB + 8);
    if (obj->mode == 0)
    {
        pattern = obj->unkA0;
        if (pattern == PAIR_SHOT_PAST_A || pattern == PAIR_SHOT_PAST_BOTH)
        {
            if ((heroA->flags7E & 0x80) || bodyA->unk18 <= 0x1000)
                goto count_down;
            partner_link_update(obj, bodyA, 0xFF);
            goto count_down;
        }
        if ((heroA->flags7E & 0x80) || bodyA->unk18 != 0)
            goto count_down;
        partner_link_update(obj, bodyA, 0xFF);
        goto count_down;
    }
    pattern = obj->unkA0;
    switch (pattern)
    {
    case PAIR_SHOT_PAST_B:
    case PAIR_SHOT_PAST_BOTH:
        if ((heroB->flags7E & 0x80) || bodyB->unk18 <= 0x1000)
            goto count_down;
        partner_link_update(obj, bodyB, 0xFF);
        goto count_down;
    default:
        goto hits_on_ground;
    }
hits_on_ground:
    /* bodyB->flags76 is the same byte as heroB->flags7E */
    if (!(bodyB->flags76 & 0x80) && bodyB->unk18 == 0)
        partner_link_update(obj, bodyB, 0xFF);
count_down:
    left = obj->timer;
    obj->timer = left - 1;
    if ((s16)left <= 0)
        obj_release(obj);
}

ASM_FUNC("asm/code_08078708/update_positions_to_target.s", s32 update_positions_to_target(struct SpriteObj *obj))

u32 lookup_slot(struct SpriteObj *obj, u32 slot);
s32 slot_step_or_remove(struct SpriteObj *obj, u32 slot);
s32 actor_link_anim_minus1(void);
void init_timer_slot(struct SpriteObj *obj);
/* Once the object's animation is done (entry flag 8): starts action 0x14, spawns effect 0x29DB at
 * it and spawns child 0x402B (0x10 to the left, 2 below and 0x50 above it, mirrored like it). The
 * child starts moving (speed 0x200) to the owner's main object's target (0xD8/0xDC) at height
 * 0xC000 and goes on with init_timer_slot; then sets slot 0x86 and goes on with
 * anim0_clear_cb_if_active_c. */
void set_anim_14_spawn_child(struct SpriteObj *obj)
{
    struct SpriteObj *child;

    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0x14, 0, 0);
        /* unk38/unk3C/unk40: position, 24.8 fixed point; entry->unk11 bit 6: mirrored */
        effect_spawn_at(0x29DB, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        child = obj_spawn(obj->mainObj, 0x402B, obj->unk38 / 0x100 - 0x10, obj->unk3C / 0x100 + 2,
                          obj->unk40 / 0x100 + 0x50, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
        obj->child = child;
        ((struct EntryBit4 *)&child->flags78)->high3 = 4;
        ((struct ObjFlags77 *)child)->bit6 = 0;
        ((struct ObjFlags77 *)child)->bit5 = 0;
        child->unk68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->unk58 = (void *)actor_link_anim_minus1;
        child->unk60 = (void (*)(struct SpriteObj *))slot_step_or_remove;
        /* where the child flies to */
        child->unk84 = obj->owner->mainObj->unkD8;
        child->unk88 = obj->owner->mainObj->unkDC;
        child->unk8C = 0xC000;
        ((struct Flags79Bits *)&child->flags79)->bit5 = 1;
        child->unk7C = 0x200;
        child->unk7A = 0;
        start_motion(child);
        child->unk74 = obj->unk74;
        child->callback = init_timer_slot;
        child->timer2 = 0;
        set_slot_value(0x86, -1);
        obj->callback = anim0_clear_cb_if_active_c;
    }
}

/* Falling state of the shot set_anim_14_spawn_child spawns: every 16 frames (unk8C) puts effect
 * 0x29E8 on the ground below it, speeds its fall (mode, +0x4C a frame up to 0x200) and lowers it
 * (unk18). Once it is down to 0x20 it hits: the owner's partner link is applied with the main
 * object's unk124 cut by 51/256 for each time it was knocked back (unkB2, at least 0), then
 * unk124 goes back to 0x100, sound 0x3B and effect 0x2A0C play and it is removed next frame. */
void tick_mode_and_remove_when_low(struct SpriteObj *shot)
{
    s32 count;
    s32 cut;

    count = shot->unk8C - 1;
    shot->unk8C = count;
    if (count <= 0)
    {
        effect_spawn_at(0x29E8, shot->unk38 / 0x100, shot->unk3C / 0x100, 0, shot);
        shot->unk8C = 0x10;
    }
    shot->mode += 0x4C;
    if ((s32)shot->mode > 0x200)
    {
        shot->mode = 0x200;
    }
    shot->unk18 = shot->unk18 - shot->mode;
    if ((s32)shot->unk18 <= 0x2000)
    {
        obj_list_remove(shot);
        /* cut = hits * 51 */
        cut = ((s32)shot->unkB2 << 1) + shot->unkB2;
        cut = cut + (cut << 4);
        if (cut <= 0x100)
        {
            shot->mainObj->unk124 = 0x100 - cut;
        }
        else
        {
            shot->mainObj->unk124 = 0;
        }
        partner_link_update(shot, shot->owner, 0xFF);
        set_slot_value(0x3B, -1);
        shot->mainObj->unk124 = 0x100;
        effect_spawn_at(0x2A0C, shot->unk38 / 0x100, shot->unk3C / 0x100, shot->unk40 / 0x100, shot);
        shot->callback = remove_object_b;
    }
}

/*
 * Hit handler (unk60) of the shot set_anim_14_spawn_child spawns. lookup_slot decides whether the
 * hero that touched it faces it; if not, its result (1 or -1) is returned unchanged. Otherwise the
 * hero knocks the shot back up (vertical speed -0x700, falling in tick_mode_and_remove_when_low)
 * with spark 0x29F1. Once the hit timer has run out, each hit also counts in unkB2 and shrinks the
 * shot (sprites 0x402C, 0x402D, 0x402E); the fourth one bursts it (effect 0x2A32, sound 0x3B)
 * and releases it, returning 0. Returns -1 while it lives on.
 */
s32 slot_step_or_remove(struct SpriteObj *shot, u32 hero)
{
    s32 result;
    s32 t;

    result = lookup_slot(shot, hero);
    if (result != 0)
        return result;
    t = shot->timer - 1;
    shot->timer = t;
    if ((s16)t <= 0)
    {
        shot->timer = 1;
        shot->unkB2++;
        switch ((s16)shot->timer2)
        {
        case 0:
            sprite_set_action(shot, 0, 0x402C, 0);
            effect_spawn_at(0x29F1, shot->unk38 / 0x100, shot->unk3C / 0x100, shot->unk40 / 0x100, shot);
            break;
        case 1:
            sprite_set_action(shot, 0, 0x402D, 0);
            effect_spawn_at(0x29F1, shot->unk38 / 0x100, shot->unk3C / 0x100, shot->unk40 / 0x100, shot);
            break;
        case 2:
            sprite_set_action(shot, 0, 0x402E, 0);
            effect_spawn_at(0x29F1, shot->unk38 / 0x100, shot->unk3C / 0x100, shot->unk40 / 0x100, shot);
            break;
        default:
            effect_spawn_at(0x2A32, shot->unk38 / 0x100, shot->unk3C / 0x100, shot->unk40 / 0x100, shot);
            obj_release(shot);
            set_slot_value(0x3B, -1);
            return 0;
        }
        shot->timer2++;
    }
    else
    {
        effect_spawn_at(0x29F1, shot->unk38 / 0x100, shot->unk3C / 0x100, shot->unk40 / 0x100, shot);
    }
    shot->mode = -0x700;
    shot->callback = tick_mode_and_remove_when_low;
    return -1;
}

/* Every 0x50 frames (mode counts the frames down) while volleys are left (timer, set to 6 by
 * anim16_timer6_mode0): plays sound 0xD5, spawns effect 0x2A61 at the object and spawns child
 * 0x402F (0x10 to the left, 2 below and 0x30 above it, mirrored like it), pushed onto the object's
 * child list, which goes on with set_anim1_mode4. With none left: starts action 0x17 and goes on
 * with anim0_clear_cb_if_active_b. */
void timer_spawn_child_402f(struct SpriteObj *obj)
{
    s32 t;
    struct SpriteObj *child;

    t = obj->mode - 1;
    obj->mode = t;
    if (t <= 0)
    {
        obj->mode = 0x50;
        t = obj->timer - 1;
        obj->timer = t;
        if ((s16)t >= 0)
        {
            set_slot_value(0xD5, -1);
            /* unk38/unk3C/unk40: position, 24.8 fixed point; entry->unk11 bit 6: mirrored */
            effect_spawn_at(0x2A61, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
            child = obj_spawn(obj->mainObj, 0x402F, obj->unk38 / 0x100 - 0x10, obj->unk3C / 0x100 + 2,
                              obj->unk40 / 0x100 + 0x30, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
            child->child = obj->child;
            obj->child = child;
            ((struct EntryBit4 *)&child->flags78)->high3 = 1;
            ((struct ObjFlags77 *)child)->bit6 = 0;
            ((struct ObjFlags77 *)child)->bit5 = 0;
            child->unk68 = on_arrive;
            child->unk6C = obj->unk6C;
            child->owner = obj->owner;
            child->unk74 = obj->unk74;
            child->callback = set_anim1_mode4;
            child->unkB2 = 0x1111;
            child->timer = 3;
        }
        else
        {
            sprite_set_action(obj, 0x17, 0, 0);
            obj->callback = anim0_clear_cb_if_active_b;
        }
    }
}

// Counts mode down. At zero: while timer is left, plays action 2 and picks a new random spot
// (random_spawn_pos); when timer runs out, picks one of the two heroes (world slots 6/7), starting
// at a random one and taking the first whose child has unkF6 set (hero 0 if none in 3 tries),
// plays action 3 + hero and snaps to that hero (snap_to_world_slot). Plays sound slot 0x119 either way.
void pick_side_random_then_anim(struct SpriteObj *obj)
{
    u8 tries;
    u8 hero;

    if ((s32)--obj->mode <= 0)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            sprite_set_action(obj, 2, 0, 0);
            obj->callback = random_spawn_pos;
        }
        else
        {
            tries = 0;
            hero = mt19937_next() & 1;
            do
            {
                tries = (u8)(tries + 1);
                hero = (u8)(hero + 1);
                hero &= 1;
            } while (gWorld->slots[hero + 6]->child->unkF6 == 0 && tries <= 2);
            if (tries > 2)
                hero = 0;
            obj->phase = hero;
            sprite_set_action(obj, hero + 3, 0, 0);
            obj->callback = snap_to_world_slot;
        }
        set_slot_value(0x119, -1);
    }
}
u32 destroy_sprite_entry(struct SpriteEntry *ent, u32 kind);
u32 slot_place_and_move(struct SpriteObj *obj, u32 other);
void set_pose_halfword_and_link(struct SpriteObj *obj);
void scroll_queue_and_cull(struct SpriteObj *obj);
void scroll_and_cull(struct SpriteObj *obj);
NONMATCH("asm/code_08078708/spawn_child_4030_with_nodes.s", void spawn_child_4030_with_nodes(struct SpriteObj *obj))
{
    s32 t;
    u16 *wait;
    u32 pool;
    u32 count;
    struct SpriteObj *child;
    struct SpriteNode *node;
    struct SpriteNode *n2;
    struct SpriteNode *n3;

    wait = &obj->timer;
    t = *wait - 1;
    *wait = t;
    if ((s16)t <= 0)
    {
        obj_list_push_default(obj);
        obj->unk58 = (void *)destroy_sprite_entry;
        obj->unk60 = (void (*)(struct SpriteObj *))slot_place_and_move;
        child = obj_spawn(obj->mainObj, 0x4030, obj->unk38 / 0x100, obj->unk3C / 0x100 - 2,
                          obj->unk40 / 0x100, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
        sprite_set_action(child, obj->phase, 0, 0);
        child->child = obj->child;
        obj->child = child;
        {
            u32 flags = child->flags79;

            child->flags79 = flags | 1;
        }
        child->unk74 = obj->unk74;
        child->callback = scroll_queue_and_cull;
        child->unkB2 = 0x2222;
        node = sprite_node_take();
        child->desc = (struct SpriteDesc *)node;
        node->unk4 = child->entry;
        node->unk8 = &child->list;
        pool = ((u8 *)gWorld)[7];
        count = (pool << 27) >> 27;
        ((u8 *)gWorld)[7] = (pool & -0x20) | ((count - 1) & 0x1F);
        node->unkC = (node->unkC & -0x20) | count;
        child->list = (struct ScrollNode *)sprite_node_alloc(child->unk10, child->unk14, child->unk18);
        n2 = sprite_node_alloc(child->unk10, child->unk14, child->unk18);
        ((struct SpriteNode *)child->list)->next = n2;
        n3 = sprite_node_alloc(child->unk10, child->unk14, child->unk18);
        n2->next = n3;
        n3->next = 0;
        obj->callback = scroll_and_cull;
    }
}
END_NONMATCH

NONMATCH("asm/code_08078708/slot_place_and_move.s", u32 slot_place_and_move(struct SpriteObj *obj, u32 other))
{
    u32 found;
    u32 rnd;
    s32 i;
    s32 pick;
    s32 seek;
    s32 slotBase;
    s32 v6;
    s32 v0;
    s32 idx;
    struct SpriteObj *part;
    struct SpriteObj *child;
    struct SpriteObj *walk;
    s32 *p84;
    s32 *p88;
    u8 *f79;
    u16 *p7c;
    u16 *pwait;
    s32 *p8c;
    u16 *p7a;

    found = lookup_slot(obj, other);
    if (found != 0)
        return found;
    gUnk_03000F64 = other;
    if ((s16)((struct SpriteObj *)other)->mainObj->unkEC == -1)
        gUnk_03000F68 = found;
    else
        gUnk_03000F68 = 1;
    child = (struct SpriteObj *)obj->mainObj->unk38;
    p84 = &obj->unk84;
    p88 = &obj->unk88;
    p8c = &obj->unk8C;
    f79 = &obj->flags79;
    p7c = &obj->unk7C;
    p7a = &obj->unk7A;
    pwait = &obj->timer;
    while (child != 0)
    {
        if (child->unkB2 == 0x2222)
            obj_release(child);
        child = child->child;
    }
    effect_spawn_at(0x2A6E, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
    rnd = mt19937_next();
    pick = (rnd & 3) + 1;
    gUnk_03000F60 = 0;
    walk = *(struct SpriteObj **)obj->mainObj;
    slotBase = (s16)obj->mainObj->unkEC;
    seek = slotBase + 1;
    for (i = 0; i < pick; i++)
    {
        do
        {
            walk = *(struct SpriteObj **)walk;
            v6 = (s16)walk->unkEC;
            v0 = (s16)walk->unkF6;
        } while (v0 == 0 || v6 == seek);
    }
    gUnk_03000F60 = (struct SpriteObj *)((u8 *)walk + 8);
    idx = ((u32)(v6 - slotBase) << 24) >> 22;
    part = obj->mainObj;
    *p84 = part->unkD8 + (gLinkOffsetX[idx >> 2] << 8);
    *p88 = part->unkDC;
    *p8c = part->unkE0 + (gLinkOffsetH[idx >> 2] << 8);
    *f79 |= 0x20;
    *p7c = 0x200;
    *p7a = 0;
    start_motion(obj);
    *pwait = 8;
    obj->callback = set_pose_halfword_and_link;
    return 0;
}
END_NONMATCH

void remove_object(struct SpriteObj *obj);
void resolve_contact_action(void *owner, void *obj, s32 a, s32 b, u32 code);
/*
 * Each frame of the shot's flight (slot_place_and_move aimed it at gUnk_03000F60): leaves trail
 * effect 0x2A86 every 8 frames and spins the sprite. When the move ends it hits the target
 * (resolve_contact_action, code 0x13 or 0x12 depending on the shooter's slot), plays sound 0xCC,
 * spawns impact effect 0x2A76 and removes itself.
 */
void set_pose_halfword_and_link(struct SpriteObj *obj)
{
    struct SpriteObj *target;
    u8 *shooter;

    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        effect_spawn_at(0x2A86, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj->timer = 8;
    }
    obj->entry->unkC += 0x1200; /* rotation */
    if (obj->target == 0)
    {
        shooter = (u8 *)gWorld->slots[gUnk_03000F68 + 6] + 8;
        if (gUnk_03000F60 != NULL)
        {
            target = gUnk_03000F60;
            resolve_contact_action(shooter, target, target->unk10 / 0x100, target->unk18 / 0x100,
                                   (((struct SpriteObj *)shooter)->mainObj->unkEC != -1) ? 0x13 : 0x12);
        }
        set_slot_value(0xCC, -1);
        effect_spawn_at(0x2A76, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj->callback = remove_object;
    }
}

void finish_when_no_target(struct SpriteObj *obj);
void anim0_clear_cb_if_active(struct SpriteObj *obj);
/*
 * Once the current animation ends (entry flag 8): action 0x10, sound 0x114 and effect 0x2A94, then
 * spawns object 0x4028 16 px left of, 2 px behind and 64 px above this one as its first child. The
 * child moves (speed 0x300) to one of two spots picked at random; the pick stays in gUnk_03000F6C
 * (0: 120,128 / 1: 112,96, both at height 40) and chooses the animations that follow.
 */
void set_anim_10_spawn_child_random(struct SpriteObj *obj)
{
    struct SpriteObj *child;
    u32 side;

    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0x10, 0, 0);
        set_slot_value(0x114, -1);
        effect_spawn_at(0x2A94, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        child = obj_spawn(obj->mainObj, 0x4028, obj->unk38 / 0x100 - 16, obj->unk3C / 0x100 + 2,
                          obj->unk40 / 0x100 + 64, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
        child->child = obj->child;
        obj->child = child;
        ((struct Flags79Bit0 *)&child->flags79)->bit0 = 1;
        child->unk74 = obj->unk74;
        child->callback = finish_when_no_target;
        ((struct ObjFlags77 *)child)->bit6 = 0;
        ((struct ObjFlags77 *)child)->bit5 = 0;
        child->unk68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        side = mt19937_next() & 1;
        gUnk_03000F6C = side;
        if (side == 0)
        {
            child->unk84 = 0x7800;
            child->unk88 = 0x8000;
            child->unk8C = 0x2800;
        }
        else
        {
            child->unk84 = 0x7000;
            child->unk88 = 0x6000;
            child->unk8C = 0x2800;
        }
        ((struct Flags79Bits *)&child->flags79)->bit5 = 1;
        child->unk7C = 0x300;
        child->unk7A = 0;
        start_motion(child);
        obj->callback = anim0_clear_cb_if_active;
    }
}

void finish_when_no_target(struct SpriteObj *obj)
{
    if (obj->target == 0)
    {
        effect_spawn_at(0x2A9D, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        sprite_set_action(obj, 0, 0x4029, 0);
        ((struct Flags79Bit0 *)&obj->flags79)->bit0 = 0;
        obj->flags78 = (obj->flags78 & 0x1F) | 0x20;
        if (gUnk_03000F6C == 0)
            obj->unk18 = 0x1800;
        else
            obj->unk18 = 0x1800;
        obj->callback = 0;
        gUnk_03000ED0 = 1;
        set_slot_value(0x55, -1);
    }
}

/* Once the current animation has finished: back to action 0 (sprite 0x4029) and one hop fewer
 * (unkA0). While hops are left it switches sides (mode) and moves at speed 0x300 to that side's
 * point, 24 px up: side 0 (120, 128) px, side 1 (112, 96) px; sound 0xD7, then
 * set_cb_if_no_target waits for the arrival. With no hops left the object stops (no callback). */
void toggle_mode_until_timer_done(struct SpriteObj *obj)
{
    s32 hopsLeft;

    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0x4029, 0);
        hopsLeft = obj->unkA0 - 1;
        obj->unkA0 = hopsLeft;
        if (hopsLeft > 0)
        {
            obj->mode = (obj->mode + 1) & 1;
            if (obj->mode == 0)
            {
                obj->unk84 = 0x7800;
                obj->unk88 = 0x8000;
                obj->unk8C = 0x1800;
            }
            else
            {
                obj->unk84 = 0x7000;
                obj->unk88 = 0x6000;
                obj->unk8C = 0x1800;
            }
            /* unk84/88/8C are an absolute point, not an offset */
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->unk7C = 0x300; /* speed */
            obj->unk7A = 0;
            start_motion(obj);
            obj->callback = set_cb_if_no_target;
            set_slot_value(0xD7, -1);
        }
        else
        {
            obj->callback = 0;
        }
    }
}
void start_anim1_if_no_target(struct SpriteObj *obj);
/*
 * Spawns object 0x4059 16 px left of, 4 px behind and 56 px above this one as its first child and
 * sends it (speed 0x400) to the ground at spot `phase` of a line of spots from (64, 80) px, each
 * 4.8 px further left and 11.2 px further down; on side 1 (gUnk_03000F6C) the line runs backwards
 * (5 - phase). On arrival the child plays action 1 (start_anim1_if_no_target).
 */
void spawn_child_4059_moving(struct SpriteObj *obj)
{
    s32 stepX;
    s32 stepY;
    s32 spot;
    s32 destX;
    s32 destY;
    struct SpriteObj *child;

    spot = obj->phase;
    if (gUnk_03000F6C != 0)
    {
        spot = 5 - spot;
    }
    stepX = -0x4CC;
    stepY = 0xB33;
    destX = (stepX * spot) + 0x4000;
    destY = spot;
    destY = (destY * stepY) + 0x5000;
    child = obj_spawn(obj->mainObj, 0x4059, (obj->unk38 / 0x100) - 16, (obj->unk3C / 0x100) + 4,
                      (obj->unk40 / 0x100) + 56, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
    child->child = obj->child;
    obj->child = child;
    ((struct EntryBit4 *)&child->flags78)->high3 = 1;
    child->unk74 = obj->unk74;
    child->callback = start_anim1_if_no_target;
    child->unk84 = destX;
    child->unk88 = destY;
    child->unk8C = 0;
    ((struct Flags79Bits *)&child->flags79)->bit5 = 1;
    child->unk7C = 0x400;
    child->unk7A = 0;
    start_motion(child);
    ((struct ObjFlags77 *)child)->bit6 = 0;
    ((struct ObjFlags77 *)child)->bit5 = 0;
    child->unk68 = on_arrive;
    child->unk6C = obj->unk6C;
    child->owner = obj->owner;
    obj_list_push_default(child);
}

/*
 * Once the current animation ends: plays action 0x11 and waits in mode_counter_tick. Its child (if
 * any) flies at speed 0x300 to 28 px right of the owner's main object (unk84/88/8C: destination,
 * flags79 bit 5: moving); timer2 says whether that object has a slot (unkEC != -1), and on arrival
 * the child goes on with start_timer_anim4. With no child the callback is cleared.
 */
void start_child_at_main(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0x11, 0, 0);
        obj->callback = mode_counter_tick;
        obj->mode = 0;
        if (obj->child == 0)
        {
            obj->callback = 0;
        }
        else
        {
            struct SpriteObj *child = obj->child;
            struct SpriteObj *mainObj;

            child->unk84 = (mainObj = obj->owner->mainObj)->unkD8 + 0x1C00;
            child->unk88 = mainObj->unkDC;
            child->unk8C = 0;
            {
                u32 flags = child->flags79;

                child->flags79 = flags | 0x20;
            }
            child->unk7C = 0x300;
            child->unk7A = 0;
            start_motion(child);
            if (obj->owner->mainObj->unkEC == -1)
                child->timer2 = 0;
            else
                child->timer2 = 1;
            child->callback = start_timer_anim4;
        }
    }
}

/* After `timer` more animation loops at the owner's main object: leaves the active object list,
 * goes back to action 0 and flies (speed 0x300) back to the side spot it came from, 24 px up
 * (side gUnk_03000F6C: 0 = (120, 128) px, 1 = (112, 96) px); end_without_target waits for the arrival. */
void timer_reset_pos_and_cb(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            obj_list_remove(obj);
            sprite_set_action(obj, 0, 0, 0);
            if (gUnk_03000F6C == 0)
            {
                obj->unk84 = 0x7800;
                obj->unk88 = 0x8000;
                obj->unk8C = 0x1800;
            }
            else
            {
                obj->unk84 = 0x7000;
                obj->unk88 = 0x6000;
                obj->unk8C = 0x1800;
            }
            /* unk84/88/8C are an absolute point, not an offset */
            ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
            obj->unk7C = 0x300; /* speed */
            obj->unk7A = 0;
            start_motion(obj);
            obj->callback = end_without_target;
        }
    }
}

void end_without_target(struct SpriteObj *obj)
{
    if (obj->target == 0)
    {
        struct SpriteObj *main;

        obj->callback = 0;
        main = obj->mainObj;
        main->phaseTimer = 2;
        main->unk113 = 2;
    }
}

void start_timer_anim4(struct SpriteObj *obj)
{
    if (obj->target == 0)
    {
        sprite_set_action(obj, 4, 0x4029, 0);
        set_slot_value(0xE6, -1);
        obj->timer = 4;
        obj->callback = timer_reset_pos_and_cb;
        obj_list_push_default(obj);
        obj->unk60 = check_and_place;
    }
}

void mode_counter_tick(struct SpriteObj *obj)
{
    if (obj->mode != 0)
    {
        if (obj->mode == 1)
            gUnk_03000ED0 = 0;
        else
            gUnk_03000ED0++;
        obj->callback = 0;
    }
}

void start_anim1_if_no_target(struct SpriteObj *obj)
{
    if (obj->target == 0)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->callback = remove_if_active;
        set_slot_value(0xAD, -1);
    }
}

void set_cb_if_no_target(struct SpriteObj *obj)
{
    if (obj->target == 0)
    {
        obj->callback = set_anim_by_alt_flag;
    }
}

void set_anim_by_alt_flag(struct SpriteObj *obj)
{
    if (gUnk_03000F6C == 0)
        sprite_set_action(obj, 2, 0x4029, 0);
    else
        sprite_set_action(obj, 7, 0x4029, 0);
    obj->callback = set_anim_3_or_8;
}

void anim0_clear_cb_if_active(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->callback = 0;
    }
}

void remove_object(struct SpriteObj *obj)
{
    obj_release(obj);
}

u32 destroy_sprite_entry(struct SpriteEntry *ent, u32 kind)
{
    struct SpriteObj *child;

    effect_spawn_at(0x2A7E, ent->unk38 / 0x100, ent->unk3C / 0x100, ent->unk40 / 0x100, ent);
    partner_link_update(ent, (void *)kind, 0xFF);
    obj_list_remove(ent);
    obj_release(ent);
    child = (struct SpriteObj *)ent->owner->unk38;
    while (child != 0)
    {
        if (child->unkB2 == 0x2222)
            obj_release(child);
        child = child->child;
    }
    return 0;
}

void scroll_queue_and_cull(struct SpriteObj *obj)
{
    struct ScrollNode *node;
    s32 carry = obj->unk10;

    obj->unk10 = carry - 0x280;
    node = obj->list;
    if (node != 0)
    {
        do
        {
            s32 prev = node->value;

            node->value = carry;
            carry = prev;
            node = node->next;
        } while (node != 0);
    }
    {
        s32 y = obj->unk10;

        if (y < 0)
            y += 0xFF;
        y >>= 8;
        if (y <= -0x28)
            obj_release(obj);
    }
}

void scroll_and_cull(struct SpriteObj *obj)
{
    s32 x = obj->unk10 + -0x280;

    obj->unk10 = x;
    if (x / 0x100 <= -0x20)
    {
        obj_release(obj);
    }
}

void snap_to_world_slot(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        struct SpriteObj *source = gWorld->slots[obj->phase + 6]->child;

        obj->unk10 = source->unkD8 + 0x3800;
        obj->unk14 = source->unkDC + 0x200;
        obj->unk18 = source->unkE0;
        sprite_set_action(obj, obj->phase + 8, 0, 0);
        obj->callback = anim_from_phase_plus5;
    }
}

void random_spawn_pos(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        s32 x;
        s32 y;
        s32 z;
        s32 keepSearching = 1;

        do
        {
            x = ((mt19937_next() % 7) + 1) << 13;
            y = ((mt19937_next() & 7) + 4) << 12;
            z = ((mt19937_next() & 3) + 2) << 12;
            if (x > 0x5800)
                break;
            if (z > y - 0x3000)
                keepSearching = 0;
        } while (keepSearching);
        obj->unk10 = x;
        obj->unk14 = y;
        obj->unk18 = z;
        sprite_set_action(obj, 0, 0, 0);
        obj->callback = set_anim1_mode4;
    }
}

void set_anim1_mode4(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->mode = 4;
        obj->callback = pick_side_random_then_anim;
    }
}

void anim0_clear_cb_if_active_b(struct SpriteObj *obj)
{
    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->callback = 0;
    }
}

s32 actor_link_anim_minus1(void)
{
    return -1;
}

void remove_object_b(struct SpriteObj *obj)
{
    obj_release(obj);
}

void init_timer_slot(struct SpriteObj *obj)
{
    u32 target = obj->target;

    if (target == 0)
    {
        obj->mode = 0x100;
        obj->unk8C = 0x10;
        obj->timer = 1;
        obj->unkB2 = target;
        obj->callback = tick_mode_and_remove_when_low;
        set_slot_value(0x9C, -1);
    }
}
