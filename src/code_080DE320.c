#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 busy;
    u8 filler22[0x23 - 0x22];
    u8 record23;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 partner;
    u8 filler0EE[0xF6 - 0xEE];
    s16 cursorAX;
    s16 cursorAY;
    u8 filler0FA[0xFC - 0xFA];
    s16 cursorBX;
    s16 cursorBY;
    u8 filler100[0x113 - 0x100];
    u8 heldItem; /* 0x80 | id of the item stolen by obj_random_slot_link_then_next, 0xFF none */
    u8 filler114[0x130 - 0x114];
};

struct ActorPoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ItemEntry
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 filler3[0x10 - 0x3];
};

/* The item counts in gParty, indexed by item id. */
struct ItemCounts
{
    u8 filler0[0x8A];
    u8 count[1];
};

#define ITEM_COUNT_UNUSABLE 0xFF /* the count of an item that can't be used */

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u32 unk4;
    u32 unk8;
    u32 unkC;
};

struct ActorObj
{
    u8 filler0[0x04];
    struct SpriteNode *node4;
    struct ActorPose *pose;
    struct SpriteNode *node0C;
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x22 - 0x1C];
    u16 fxLife;
    u8 filler24[0x28 - 0x24];
    struct ActorPart *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x47 - 0x44];
    s8 unk47; /* frame box height, set by find_frame_entry */
    u8 filler48[0x4C - 0x48];
    void (*state)(struct ActorObj *);
    u8 filler50[0x54 - 0x50];
    void (*savedState)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u16 progress;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x92 - 0x90];
    s16 unk92; /* height of a jump arc set up by place_targets / set_motion_offsets */
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 spawnLeft;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
    u8 filler0B4[0xC3 - 0xB4];
    s8 unkC3; /* offset (pixels) of the point others aim at: X, Y, height */
    s8 unkC4;
    s8 unkC5;
};

struct ObjFlags79Bit0
{
    u8 filler0[0x79];
    u8 bit0 : 1;
    u8 rest : 7;
};

struct ObjFlags79
{
    u8 filler0[0x79];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct ObjFlags76
{
    u8 filler0[0x76];
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct FxTask
{
    u8 filler0[4];
    void *unk4;
    void *unk8;
    u8 filler0C[0x10 - 0x0C];
    u16 counter;
    u8 filler12[0x18 - 0x12];
    u16 unk18;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FxRec
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 0x4];
    u16 unkE;
};

struct FxBit12
{
    u8 filler0[0x12];
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct FxBits1F
{
    u8 filler0[0x1F];
    u8 a : 2;
    u8 b : 2;
    u8 c : 2;
    u8 d : 2;
};

u32 is_state_busy(void);
void release_obj_131c(void *p);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void on_arrive(struct ActorObj *obj);
void obj_active_part_action_e_then_d43c(struct ActorObj *obj);
void obj_active_action_e_then_d090(struct ActorObj *obj);
void obj_active_pair_action_c_then_e2f8(struct ActorObj *obj);
void obj_pose_active_idle_arrive_b(struct ActorObj *obj);
void obj_float_offscreen_finish(struct ActorObj *obj);
void move_states_obj_pose_active_spawn_child(struct ActorObj *obj);
void set_slot_value(u32 id, s32 value);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_active_drop_to_ground_then_cf58(struct ActorObj *obj);
void obj_active_action_b_then_e2b4(struct ActorObj *obj);
void obj_drop_land_stop(struct ActorObj *obj);
void obj_drop_land_spawn_child(struct ActorObj *obj);
void obj_arrive_toggle_pose(struct ActorObj *obj);
void obj_timer_release_child_start_motion(struct ActorObj *obj);
void obj_partner_action_timer(struct ActorObj *obj);
void obj_timed_bounce_or_finish(struct ActorObj *obj);
void obj_begin_fall_to_owner(struct ActorObj *obj);
u32 obj_slot_found_recoil(struct ActorObj *obj, u32 slot);
void obj_settle_step_land_slot69(struct ActorObj *obj);
extern u8 *gWorld;
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void place_targets(struct ActorObj *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct ActorObj *obj, u32 offsets);
s32 find_frame_entry(struct ActorObj *obj, s32 index);
void obj_list_push_default(struct ActorObj *obj);
u32 link_slot_lookup_clear_11c(struct ActorObj *obj, u32 a, u32 b);
void obj_action8_slot126_timer(struct ActorObj *obj);
u32 obj_random_slot_link_then_next(struct ActorObj *obj, struct ActorObj *arg);
void change_item_count(u32 item, u32 count);
void obj_lower_height_512(struct ActorObj *obj);
extern u8 *gParty;
u32 mt19937_next(void);
void start_motion(struct ActorObj *obj);
u32 rand_below(u32 n, u32 r);
void sound_stop_song(u32 song);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_release(struct ActorObj *obj);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void *task_create_list_4c(void (*fn)(struct FxTask *), void *obj);
void fx_task_tick_release(struct FxTask *task);
void obj_spawn_aimed_child_pair(struct ActorObj *obj);
void obj_child_trail_update(struct ActorObj *obj);
void obj_begin_fall_to_owner_alt(struct ActorObj *obj);
struct SpriteNode *sprite_node_take(void);
void obj_list_push_arg(struct ActorObj *obj, u32 arg);
void obj_fall_spawn_fx_offscreen(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_active_spawn_then_e1fc(struct ActorObj *obj);
void obj_active_spawn_then_e278(struct ActorObj *obj);
void obj_slot_spawn_child_anim9(struct ActorObj *obj);
void ensure_code_entry(u32 code);
extern struct ItemEntry gItemTable[];
extern const u16 gEffectCodeTable[];
void step_motion(struct ActorObj *obj);
s32 menu_cursor_pos(s32 x, s32 y, u32 n);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
s32 anim_set_motion_a(struct ActorPart *data, u16 value, s32 extra);
s32 anim_set_motion_b(struct ActorPart *data, u16 value, s32 extra);
s32 anim_bit400_step(struct ActorPart *data, s32 value);
s32 anim_chance_start(struct ActorPart *data, s32 value);
void anim_stop_mode(struct ActorPart *data);
void anim_bit100_release(struct ActorPart *data);
void anim_end_mode(struct ActorPart *data);
void anim_bit8_scale10a(struct ActorPart *data);
void anim_bit20_scale10c(struct ActorPart *data);
void anim_bit80_scale11c(struct ActorPart *data);
void anim_clear_high_bits(struct ActorPart *data);

static inline u16 entry_value_at(s32 k)
{
    return *(u16 *)(*(u8 **)(gWorld + k) + 8);
}

static inline struct ActorObj *obj_spawn_entry_child(struct ActorObj *obj, u8 *entry, u32 base)
{
    base += entry[2];
    return obj_spawn_linked(obj, base, 0);
}

static inline void pose_set_mode_one(struct ActorObj *obj)
{
    ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
}

static inline void obj_base_effect(struct ActorObj *obj)
{
    effect_spawn_at(0xCAC, obj->baseX / 0x100 - 0x14, obj->baseY / 0x100 + 1, obj->baseH / 0x100 + 0x10, obj->owner);
}

static inline void obj_set_pair_target(struct ActorObj *obj)
{
    struct ActorPart *b;
    struct ActorPart *a;
    struct ActorPart *data;

    data = obj->owner->data;
    if (data->partner == -1)
    {
        a = data;
        b = data + 1;
    }
    else
    {
        a = data - 1;
        b = data;
    }
    if (a->partner != 0)
    {
        if (b->partner != 0)
        {
            obj->targetX = (a->targetX + b->targetX) / 2;
            obj->targetY = (a->targetY + b->targetY) / 2;
        }
        else
        {
            obj->targetX = a->targetX;
            obj->targetY = a->targetY;
        }
    }
    else
    {
        obj->targetX = b->targetX;
        obj->targetY = b->targetY;
    }
    obj->targetH = obj->owner->data->targetH;
    obj->targetX += 0x4000;
    ((struct ObjFlags79 *)obj)->bit5 = 1;
}

void obj_wait_free_then_df7c(struct ActorObj *obj);
void obj_wait_free_then_dfa8(struct ActorObj *obj);
void obj_wait_free_then_dfd4(struct ActorObj *obj);
void obj_wait_free_then_e000(struct ActorObj *obj);
void obj_action_d_pose_mode1_then_e02c(struct ActorObj *obj);
void obj_action_d_pose_mode1_then_e0b4(struct ActorObj *obj);
void obj_action_9_pose_mode1_then_e104(struct ActorObj *obj);
void obj_action_9_pose_mode1_then_e180(struct ActorObj *obj);
void obj_rebuild_after_motion(struct ActorObj *obj);

/*
 * Moves the object to the two heroes: its target is the midpoint of both heroes' targets (or the one
 * hero's whose partner field is set, when only one is), 0x40 px further along x, at the owner's
 * height. Starts the move at paramA0 px per frame, plays slot 0x11C and action 2, then waits in
 * obj_partner_action_timer.
 */
void obj_pair_midpoint_move_then_f3b4(struct ActorObj *obj)
{
    struct ActorPart *heroB;
    struct ActorPart *heroA;
    struct ActorPart *part;
    /* register variables (and the dead `if` below) keep the original's register use */
    register s32 *pTargetX asm("r3");
    register s32 *pLast asm("r2");
    register s32 *pTargetH asm("r1");

    part = obj->owner->data;
    /* the hero parts sit side by side; the first one's partner field is -1 */
    if (part->partner == -1)
    {
        heroA = part;
        heroB = part + 1;
    }
    else
    {
        heroA = part - 1;
        heroB = part;
    }
    if (heroA->partner != 0)
    {
        if (heroB->partner != 0)
        {
            pTargetX = &obj->targetX;
            *pTargetX = (heroA->targetX + heroB->targetX) / 2;
            pLast = &obj->targetY;
            *pLast = (heroA->targetY + heroB->targetY) / 2;
        }
        else
        {
            pLast = &obj->targetX;
            *pLast = heroA->targetX;
            obj->targetY = heroA->targetY;
            pTargetX = pLast;
        }
    }
    else
    {
        pLast = &obj->targetX;
        *pLast = heroB->targetX;
        obj->targetY = heroB->targetY;
        pTargetX = pLast;
    }
    pTargetH = &obj->targetH;
    *pTargetH = obj->owner->data->targetH;
    *pTargetX += 0x4000;
    if (pTargetH == pLast)
        pTargetH = NULL;
    ((struct ObjFlags79 *)obj)->bit5 = 1;
    obj->motionSpeed = obj->paramA0 << 8;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x11C, -1);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_partner_action_timer;
}

/*
 * Item thief, waiting to strike: counts the timer down, then joins the active object list with
 * obj_random_slot_link_then_next (steal a mushroom) as its hit handler and link_slot_lookup_clear_11c
 * (stop song 0x11C) as its slot lookup, dashes at its owner's target point (speed 0x199) playing
 * action 7 and song 0x11C (set_slot_value), and waits for arrival in obj_action8_slot126_timer.
 */
void obj_timer_move_to_owner_then_f358(struct ActorObj *obj)
{
    struct ActorPart *victim;
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        obj_list_push_default(obj);
        obj->handler58 = (void (*)(struct ActorObj *))obj_random_slot_link_then_next;
        obj->handler60 = (void (*)(struct ActorObj *))link_slot_lookup_clear_11c;
        obj->targetX = (victim = obj->owner->data)->targetX;
        obj->targetY = victim->targetY;
        obj->targetH = victim->targetH;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x199;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 7, 0, 0);
        set_slot_value(0x11C, -1);
        obj->state = obj_action8_slot126_timer;
    }
}

/* Counts the timer down; when it runs out: stops song 0x126, clears the owner's state and moves to
 * 0x40 px right of the owner's pair of parts (their midpoint, or the one that has a partner) at speed
 * 0x200, playing action 2 with the pose mirrored, and goes on to obj_slot_spawn_child_anim9. If an item
 * slot was taken (unk113 = 0x80 | slot), spawnLeft gets the object id for that item's kind. */
void obj_timer_pair_move_then_e640(struct ActorObj *obj)
{
    struct ActorPart *partB;
    struct ActorPart *partA;
    struct ActorPart *data;
    struct ActorPose *pose;
    u8 *itemCode;
    s32 timer = obj->timer - 1;
    s32 poseByte;
    s32 mirror;
    s32 value;
    u32 code;
    register s32 *targetX asm("r3");
    register s32 *dst asm("r2");
    register s32 *targetH asm("r1");

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        sound_stop_song(0x126);
        obj->owner->state = 0;
        data = obj->owner->data;
        if (data->partner == -1)
        {
            partA = data;
            partB = data + 1;
        }
        else
        {
            partA = data - 1;
            partB = data;
        }
        if (partA->partner != 0)
        {
            if (partB->partner != 0)
            {
                targetX = &obj->targetX;
                *targetX = (partA->targetX + partB->targetX) / 2;
                dst = &obj->targetY;
                *dst = (partA->targetY + partB->targetY) / 2;
            }
            else
            {
                dst = &obj->targetX;
                *dst = partA->targetX;
                obj->targetY = partA->targetY;
                targetX = dst;
            }
        }
        else
        {
            dst = &obj->targetX;
            *dst = partB->targetX;
            obj->targetY = partB->targetY;
            targetX = dst;
        }
        targetH = &obj->targetH;
        *targetH = obj->owner->data->targetH;
        *targetX += 0x4000;
        /* never true: keeps the register vars live as the original's */
        if (targetH == dst)
            targetH = 0;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        /* flip the pose's mirror bit (byte11 bit 6) */
        pose = obj->pose;
        poseByte = pose->byte11;
        mirror = ((u32)poseByte << 25) >> 31;
        mirror = mirror ^ 1;
        mirror = mirror << 6;
        value = -0x41;
        value &= poseByte;
        value |= mirror;
        pose->byte11 = value;
        obj->state = obj_slot_spawn_child_anim9;
        itemCode = &obj->data->heldItem;
        code = *itemCode;
        if ((code & 0x80) && code != 0xFF)
        {
            u32 slotMask = 0x7F;
            s32 *spawnId = &obj->spawnLeft;
            u8 *table = (u8 *)gItemTable;
            s32 id = (table[(slotMask & *(volatile u8 *)itemCode) * 16 + 2] & 7) + 0x4188;

            *spawnId = id;
            ensure_code_entry(id);
        }
    }
}

/* Item thief, after running off with its loot (unless a message is up, and only for index 0):
 * if obj_random_slot_link_then_next got nothing (heldItem 0xFF), clears heldItem and plays action
 * 0x13; otherwise plays action 9 and sound 0xA3 (set_slot_value) and spawns the stolen item's sprite
 * (spawnLeft, set by obj_timer_pair_move_then_e640) above its head, facing the same way. Then waits
 * 0x30 frames in obj_timer_release_child_start_motion, which drops the sprite and walks back. */
void obj_slot_spawn_child_anim9(struct ActorObj *obj)
{
    struct ActorObj *itemSprite;

    if (is_state_busy())
    {
        return;
    }
    if (obj->index != 0)
    {
        return;
    }
    if (obj->data->heldItem == 0xFF)
    {
        /* index is 0 here: nothing held any more */
        obj->data->heldItem = obj->index;
        sprite_set_action(obj, 0x13, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 9, 0, 0);
        set_slot_value(0xA3, -1);
        itemSprite = obj_spawn(obj->data, *(u16 *)&obj->spawnLeft, obj->baseX / 0x100 + 2, obj->baseY / 0x100 + 1,
                               obj->baseH / 0x100 + 0x27, (((u32)obj->pose->byte11 << 25) >> 31), 1);
        obj->child = itemSprite;
        ((struct ObjFlags79Bit0 *)itemSprite)->bit0 = 1;
        itemSprite->owner = obj->owner;
        itemSprite->byte74 = 0xFF;
        sprite_set_action(itemSprite, 0, 0, 0);
    }
    obj->timer = 0x30;
    obj->state = obj_timer_release_child_start_motion;
}

void obj_timer_release_child_start_motion(struct ActorObj *obj)
{
    struct ActorPart *data;
    s32 t = obj->timer - 1;

    obj->timer = t;
    if ((s16)t < 0)
    {
        if (obj->child != 0)
        {
            obj_release(obj->child);
            obj->child = 0;
        }
        obj->targetX = (data = obj->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_arrive_toggle_pose;
    }
}

/* Hit handler of an item thief: steals one of the party's mushrooms (item ids 0-2) and holds it
 * in data->heldItem (0x80 | id, or 0xFF when the party has none). The pick is weighted: each
 * owned kind replaces the earlier pick with odds weight / (sum of weights so far), weights 70, 25
 * and 5. Then it runs the owner's handler68 and drops the hit object back to the ground. */
u32 obj_random_slot_link_then_next(struct ActorObj *obj, struct ActorObj *hit)
{
    s32 item = -1;
    u32 totalWeight = 0;
    u8 count;

    count = ((struct ItemCounts *)gParty)->count[0];
    if (count != 0 && count != ITEM_COUNT_UNUSABLE)
    {
        totalWeight = 70;
        if (rand_below(totalWeight, mt19937_next()) < 70)
        {
            item = 0;
        }
    }
    count = ((struct ItemCounts *)gParty)->count[1];
    if (count != 0 && count != ITEM_COUNT_UNUSABLE)
    {
        totalWeight += 25;
        if (rand_below(totalWeight, mt19937_next()) < 25)
        {
            item = 1;
        }
    }
    count = ((struct ItemCounts *)gParty)->count[2];
    if (count != 0 && count != ITEM_COUNT_UNUSABLE)
    {
        totalWeight += 5;
        if (rand_below(totalWeight, mt19937_next()) < 5)
        {
            item = 2;
        }
    }
    if (item != -1)
    {
        change_item_count((u8)item, -1);
        obj->data->heldItem = item | 0x80;
    }
    else
    {
        obj->data->heldItem = 0xFF;
    }
    if (obj->owner->handler68 != 0)
    {
        obj->owner->handler68(obj->owner);
    }
    hit->state = obj_lower_height_512;
    return 1;
}

/* Spawns the sprite of the item in use (kind 0x4188 + the item's sprite index) at this obj,
   raised by its aim height, for the same owner and speed. When the sprite's mode is 1 or 2 it
   jumps to the owner's aim point, its arc height worked out from the owner's height (the Z offset
   for a hero's own body, else its frame box height). On landing, obj_rebuild_after_motion applies
   the item's effect to the owner. */
void obj_spawn_linked_child_jump(struct ActorObj *obj)
{
    u8 *world = gWorld;
    struct ActorObj *receiver = obj->owner;
    struct ActorObj *itemObj;
    u8 *item;
    s32 height;
    s32 heightSq;

    item = *(u8 **)(world + 0x2DC); /* the item being used */
    itemObj = obj_spawn_linked(obj, (u16)(0x4188 + item[2]), 0);
    itemObj->baseH += (s8)obj->unkC5 << 8;
    itemObj->posX = itemObj->baseX;
    itemObj->posY = itemObj->baseY;
    itemObj->posH = itemObj->baseH;
    ((struct Flags78Bits *)&itemObj->dir)->kind = 1;
    itemObj->owner = obj->owner;
    itemObj->motionSpeed = obj->motionSpeed;
    if (((struct ObjFlags76 *)itemObj)->mode == 1 || ((struct ObjFlags76 *)itemObj)->mode == 2)
    {
        place_targets(itemObj, receiver->baseX / 0x100 + (s8)receiver->unkC3,
                      receiver->baseY / 0x100 + (s8)receiver->unkC4,
                      receiver->baseH / 0x100 + (s8)receiver->unkC5, -1);
        /* a hero's own body (embedded at its data + 8) uses its Z offset */
        if (receiver == (struct ActorObj *)((u8 *)receiver->data + 8))
        {
            height = (s8)receiver->unkC5;
        }
        else
        {
            find_frame_entry(receiver, 0);
            height = (s8)receiver->unk47;
        }
        height += receiver->baseH / 0x100;
        heightSq = height * height;
        height += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x640, heightSq / 0x40 + 0x28);
        itemObj->unk92 = height;
        set_motion_offsets(itemObj, 0x480);
    }
    itemObj->state = obj_rebuild_after_motion;
}

NONMATCH("asm/code_080DE320/obj_rebuild_after_motion.s", void obj_rebuild_after_motion(struct ActorObj *obj))
{
    struct ActorObj *owner = obj->owner;
    s32 dx;
    s32 cursor;
    s32 t;
    u8 **pw = &gWorld;

    step_motion(obj);
    if (((struct ObjFlags79 *)obj)->bit5)
    {
        effect_spawn_at(gEffectCodeTable[(*pw)[0x349]], owner->posX / 0x100, owner->posY / 0x100 + 2,
                        owner->posH / 0x100 - 2, owner);
        t = (*(u8 **)((*pw) + 0x2DC))[0] - 1;
        switch (t)
        {
        case 0:
            cursor = menu_cursor_pos(owner->data->cursorAX, owner->data->cursorAY, entry_value_at(0x2DC));
            popup_digit_spawn(cursor - owner->data->cursorAX, owner->baseX / 0x100, owner->baseY / 0x100, owner->unkC5, 1);
            owner->data->cursorAX = cursor;
            break;
        case 1:
            anim_stop_mode(owner->data);
            anim_bit100_release(owner->data);
            anim_end_mode(owner->data);
            anim_bit8_scale10a(owner->data);
            anim_bit20_scale10c(owner->data);
            anim_bit80_scale11c(owner->data);
            anim_clear_high_bits(owner->data);
            break;
        case 10:
            cursor = menu_cursor_pos(owner->data->cursorBX, owner->data->cursorBY, entry_value_at(0x2DC));
            popup_digit_spawn(cursor - owner->data->cursorBX, owner->baseX / 0x100, owner->baseY / 0x100, owner->unkC5, 1);
            owner->data->cursorBX = cursor;
            break;
        case 11:
        {

            cursor = menu_cursor_pos(owner->data->cursorBX, owner->data->cursorBY, entry_value_at(0x2DC));
            popup_digit_spawn(cursor - owner->data->cursorBX, owner->baseX / 0x100 - 0xC, owner->baseY / 0x100 - 4, owner->unkC5, 1);
            owner->data->cursorBX = cursor;
            cursor = menu_cursor_pos(owner->data->cursorAX, owner->data->cursorAY, entry_value_at(0x2DC));
            popup_digit_spawn(cursor - owner->data->cursorAX, owner->baseX / 0x100 + 0xC, owner->baseY / 0x100 + 4, owner->unkC5, 1);
            owner->data->cursorAX = cursor;
            break;
        }
        case 12:
            effect_spawn_at(0xCB, owner->posX / 0x100, owner->posY / 0x100 + 2, owner->posH / 0x100 - 2, owner);
            anim_set_motion_a(owner->data, (u16)(entry_value_at(0x2DC) + 0x100), 0xC8);
            anim_bit400_step(owner->data, 0xC8);
            break;
        case 13:
            effect_spawn_at(0xCB, owner->posX / 0x100, owner->posY / 0x100 + 2, owner->posH / 0x100 - 2, owner);
            anim_set_motion_b(owner->data, (u16)(entry_value_at(0x2DC) + 0x100), 0xC8);
            anim_chance_start(owner->data, 0xC8);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            obj_release(obj);
            break;
        }
        obj_release(obj);
    }
}
END_NONMATCH
