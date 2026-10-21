#include "global.h"
#include "ram_routines.h"

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

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
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
extern s32 gUnk_03000EB0[];
extern s32 gUnk_03000EB8[];
extern s32 gUnk_03000EC0[];

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
        actorA->entry->flags &= ~0x10;
    }
    if (actorB->callback == actor_link_anim_steps)
    {
        actorB->callback = gUnk_03000ED8;
        actorB->entry->flags &= ~0x10;
    }
    gUnk_03000F5C = 0;
    gUnk_03000F5D = 0;
    child = actorA->child;
    while (child != 0)
    {
        if (child->callback == actor_link_anim_steps)
        {
            u8 n;

            child->entry->flags &= ~0x10;
            n = gUnk_03000F5C;
            gUnk_03000F5C = n + 1;
            child->callback = gUnk_03000EDC[n];
        }
        child = child->child;
    }
    child = actorB->child;
    while (child != 0)
    {
        if (child->callback == actor_link_anim_steps)
        {
            u8 n;

            child->entry->flags &= ~0x10;
            n = gUnk_03000F5D;
            gUnk_03000F5D = n + 1;
            child->callback = gUnk_03000F1C[n];
        }
        child = child->child;
    }
    obj->callback = 0;
}
END_NONMATCH

s32 update_positions_to_target(struct SpriteObj *obj);
void link_actor_if_free(struct SpriteObj *obj);
void ramp_values_then_remove(struct SpriteObj *obj);
NONMATCH("asm/code_08078708/spawn_child_4025_adopt.s", void spawn_child_4025_adopt(struct SpriteObj *obj))
{
    struct SpriteObj *part;
    struct SpriteObj *partBody;
    struct SpriteObj *child;

    obj->timer2 += 0x400;
    update_positions_to_target(obj);
    part = obj->mainObj;
    partBody = (struct SpriteObj *)((u8 *)part + 8);
    child = obj_spawn(part, 0x4025, gUnk_03000EB0[obj->mode] / 0x100, gUnk_03000EB8[obj->mode] / 0x100,
                      gUnk_03000EC0[obj->mode] / 0x100, 0, 1);
    sprite_set_action(child, 1, 0, 0);
    child->unk74 = 0xFF;
    {
        u32 flags = child->flags79;

        child->flags79 = flags | 1;
    }
    {
        u32 flags = child->flags76;

        child->flags76 = flags | 6;
    }
    child->owner = partBody->owner;
    child->unk6C = partBody->unk6C;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
    child->entry->flags &= ~6;
    child->callback = link_actor_if_free;
    child->timer = 0x1E;
    child->child = obj->child;
    child->mode = obj->mode;
    child->unkA0 = obj->unkA0;
    child->child = obj->child;
    obj->child = child;
    obj->timer = 0x28;
    if (obj->mode == 0)
    {
        if (obj->unkA0 == 0 || obj->unkA0 == 2)
            child->unk10 = 0x10000;
    }
    else
    {
        if (obj->unkA0 <= 2)
        {
            if (obj->unkA0 >= 1)
                child->unk10 = 0x10000;
        }
    }
    obj->callback = ramp_values_then_remove;
}
END_NONMATCH

static inline s32 scale_by_18(s32 h)
{
    return ((h << 3) + h) << 1;
}

NONMATCH("asm/code_08078708/ramp_values_then_remove.s", void ramp_values_then_remove(struct SpriteObj *obj))
{
    s32 t;
    s32 resX;
    s32 resY;
    u32 link;
    u16 *oam;

    update_positions_to_target(obj);
    t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 18)
    {
        resX = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scale_by_18(gUnk_03000EC8[obj->mode]), (s16)obj->timer);
        resY = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scale_by_18(gUnk_03000ECC[obj->mode]), (s16)obj->timer);
        link = (((u32)gSys[0x888] << 28) >> 31) << 10;
        oam = (u16 *)(gSys + 0x80 + link + (((u32)obj->desc->order << 27) >> 22));
        oam[3] = resX;
        oam[7] = resY;
        oam[11] = -resY;
        oam[15] = resX;
        if ((s16)obj->timer == 1)
            obj_release(obj);
    }
}
END_NONMATCH
NONMATCH("asm/code_08078708/link_actor_if_free.s", void link_actor_if_free(struct SpriteObj *obj))
{
    struct SpriteObj *linkA;
    struct SpriteObj *linkB;
    struct SpriteObj *actorA;
    struct SpriteObj *actorB;
    struct SpriteObj *partner;
    s32 a0;
    s32 t;

    linkA = gWorld->slots[6];
    actorA = (struct SpriteObj *)((u8 *)linkA + 8);
    linkB = gWorld->slots[7];
    actorB = (struct SpriteObj *)((u8 *)linkB + 8);
    if (obj->mode == 0)
    {
        a0 = obj->unkA0;
        if (a0 == 0 || a0 == 2)
        {
            if ((linkA->flags7E & 0x80) || actorA->unk18 <= 0x1000)
                goto timer;
            partner_link_update(obj, actorA, 0xFF);
            goto timer;
        }
        if ((linkA->flags7E & 0x80) || actorA->unk18 != 0)
            goto timer;
        partner_link_update(obj, actorA, 0xFF);
        goto timer;
    }
    a0 = obj->unkA0;
    if (a0 > 2)
        goto out_of_range;
    if (a0 < 1)
        goto out_of_range;
    if ((linkB->flags7E & 0x80) || actorB->unk18 <= 0x1000)
        goto timer;
    partner_link_update(obj, actorB, 0xFF);
    goto timer;
out_of_range:
    if (!(actorB->flags76 & 0x80) && actorB->unk18 == 0)
        partner_link_update(obj, actorB, 0xFF);
timer:
    t = obj->timer;
    obj->timer = t - 1;
    if ((s16)t <= 0)
        obj_release(obj);
}
END_NONMATCH

ASM_FUNC("asm/code_08078708/update_positions_to_target.s", s32 update_positions_to_target(struct SpriteObj *obj))

u32 lookup_slot(struct SpriteObj *obj, u32 slot);
s32 slot_step_or_remove(struct SpriteObj *obj, u32 slot);
s32 actor_link_anim_minus1(void);
void init_timer_slot(struct SpriteObj *obj);
NONMATCH("asm/code_08078708/set_anim_14_spawn_child.s", void set_anim_14_spawn_child(struct SpriteObj *obj))
{
    struct SpriteObj *child;

    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0x14, 0, 0);
        effect_spawn_at(0x29DB, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        child = obj_spawn(obj->mainObj, 0x402B, obj->unk38 / 0x100 - 0x10, obj->unk3C / 0x100 + 2,
                          obj->unk40 / 0x100 + 0x50, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
        obj->child = child;
        child->flags78 = (child->flags78 & 0x1F) | 0x80;
        ((struct ObjFlags77 *)child)->bit6 = 0;
        ((struct ObjFlags77 *)child)->bit5 = 0;
        child->unk68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->unk58 = (void *)actor_link_anim_minus1;
        child->unk60 = (void (*)(struct SpriteObj *))slot_step_or_remove;
        child->unk84 = obj->owner->mainObj->unkD8;
        child->unk88 = obj->owner->mainObj->unkDC;
        child->unk8C = 0xC000;
        {
            u32 flags = child->flags79;

            child->flags79 = flags | 0x20;
        }
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
END_NONMATCH

void tick_mode_and_remove_when_low(struct SpriteObj *obj)
{
    s32 count;
    s32 scaled;

    count = obj->unk8C - 1;
    obj->unk8C = count;
    if (count <= 0)
    {
        effect_spawn_at(0x29E8, obj->unk38 / 0x100, obj->unk3C / 0x100, 0, obj);
        obj->unk8C = 0x10;
    }
    obj->mode += 0x4C;
    if ((s32)obj->mode > 0x200)
    {
        obj->mode = 0x200;
    }
    obj->unk18 = obj->unk18 - obj->mode;
    if ((s32)obj->unk18 <= 0x2000)
    {
        obj_list_remove(obj);
        scaled = ((s32)obj->unkB2 << 1) + obj->unkB2;
        scaled = scaled + (scaled << 4);
        if (scaled <= 0x100)
        {
            obj->mainObj->unk124 = 0x100 - scaled;
        }
        else
        {
            obj->mainObj->unk124 = 0;
        }
        partner_link_update(obj, obj->owner, 0xFF);
        set_slot_value(0x3B, -1);
        obj->mainObj->unk124 = 0x100;
        effect_spawn_at(0x2A0C, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj->callback = remove_object_b;
    }
}

NONMATCH("asm/code_08078708/slot_step_or_remove.s", s32 slot_step_or_remove(struct SpriteObj *obj, u32 slot))
{
    s32 found;
    s32 t;
    s32 phase;
    u32 kind;

    found = lookup_slot(obj, slot);
    if (found != 0)
        return found;
    t = obj->timer - 1;
    obj->timer = t;
    if ((s16)t <= 0)
    {
        obj->timer = 1;
        obj->unkB2++;
        phase = (s16)obj->timer2;
        switch (phase)
        {
        case 0:
            kind = 0x402C;
            break;
        case 1:
            kind = 0x402D;
            break;
        case 2:
            kind = 0x402E;
            break;
        default:
            goto none;
        }
        sprite_set_action(obj, kind, 0, 0);
        effect_spawn_at(0x29F1, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj->timer2++;
        goto tail;
    none:
        effect_spawn_at(0x2A32, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj_release(obj);
        set_slot_value(0x3B, -1);
        return 0;
    }
    else
    {
        effect_spawn_at(0x29F1, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
    }
tail:
    obj->mode = 0xFFFFF900;
    obj->callback = tick_mode_and_remove_when_low;
    return -1;
}
END_NONMATCH

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
            effect_spawn_at(0x2A61, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
            child = obj_spawn(obj->mainObj, 0x402F, obj->unk38 / 0x100 - 0x10, obj->unk3C / 0x100 + 2,
                              obj->unk40 / 0x100 + 0x30, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
            child->child = obj->child;
            obj->child = child;
            child->flags78 = (child->flags78 & 0x1F) | 0x20;
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

void pick_side_random_then_anim(struct SpriteObj *obj)
{
    u8 tries;
    u8 parity;

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
            parity = mt19937_next() & 1;
            goto body;
        check:
            if (tries > 2)
                goto side0;
        body:
            tries = (u8)(tries + 1);
            parity = (u8)(parity + 1);
            parity &= 1;
            if (gWorld->slots[parity + 6]->child->unkF6 == 0)
                goto check;
            if (tries <= 2)
                goto store;
        side0:
            parity = 0;
        store:
            obj->phase = parity;
            sprite_set_action(obj, parity + 3, 0, 0);
            obj->callback = snap_to_world_slot;
        }
        set_slot_value(0x119, -1);
    }
}u32 destroy_sprite_entry(struct SpriteEntry *ent, u32 kind);
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
        child->flags79 |= 1;
        child->unk74 = obj->unk74;
        child->callback = scroll_queue_and_cull;
        child->unkB2 = 0x2222;
        node = sprite_node_take();
        child->desc = (struct SpriteDesc *)node;
        node->unk4 = child->entry;
        node->unk8 = &child->list;
        pool = ((u8 *)gWorld)[7];
        count = (pool << 27) >> 27;
        ((u8 *)gWorld)[7] = (pool & 0xE0) | ((count - 1) & 0x1F);
        node->unkC = (node->unkC & 0xE0) | count;
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

    found = lookup_slot(obj, other);
    if (found != 0)
        return found;
    gUnk_03000F64 = other;
    if ((s16)((struct SpriteObj *)other)->mainObj->unkEC == -1)
        gUnk_03000F68 = found;
    else
        gUnk_03000F68 = 1;
    child = (struct SpriteObj *)obj->mainObj->unk38;
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
    part = obj->mainObj;
    walk = *(struct SpriteObj **)part;
    slotBase = (s16)part->unkEC;
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
    obj->unk84 = part->unkD8 + (gLinkOffsetX[idx >> 2] << 8);
    obj->unk88 = part->unkDC;
    obj->unk8C = part->unkE0 + (gLinkOffsetH[idx >> 2] << 8);
    {
        u32 flags = obj->flags79;

        obj->flags79 = flags | 0x20;
    }
    obj->unk7C = 0x200;
    obj->unk7A = 0;
    start_motion(obj);
    obj->timer = 8;
    obj->callback = set_pose_halfword_and_link;
    return 0;
}
END_NONMATCH

void remove_object(struct SpriteObj *obj);
void resolve_contact_action(void *owner, void *obj, s32 a, s32 b, u32 code);
void set_pose_halfword_and_link(struct SpriteObj *obj)
{
    struct SpriteObj *partner;
    u8 *pa;

    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        effect_spawn_at(0x2A86, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj->timer = 8;
    }
    obj->entry->unkC += 0x1200;
    if (obj->target == 0)
    {
        pa = (u8 *)gWorld->slots[gUnk_03000F68 + 6] + 8;
        if (gUnk_03000F60 != NULL)
        {
            partner = gUnk_03000F60;
            resolve_contact_action(pa, partner, partner->unk10 / 0x100, partner->unk18 / 0x100,
                                   (((struct SpriteObj *)pa)->mainObj->unkEC != -1) ? 0x13 : 0x12);
        }
        set_slot_value(0xCC, -1);
        effect_spawn_at(0x2A76, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        obj->callback = remove_object;
    }
}

void finish_when_no_target(struct SpriteObj *obj);
void anim0_clear_cb_if_active(struct SpriteObj *obj);
void set_anim_10_spawn_child_random(struct SpriteObj *obj)
{
    struct SpriteObj *child;
    u32 rnd;

    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0x10, 0, 0);
        set_slot_value(0x114, -1);
        effect_spawn_at(0x2A94, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        child = obj_spawn(obj->mainObj, 0x4028, obj->unk38 / 0x100 - 0x10, obj->unk3C / 0x100 + 2,
                          obj->unk40 / 0x100 + 0x40, ((struct PoseBits6 *)&obj->entry->unk11)->bit6, 1);
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
        rnd = mt19937_next() & 1;
        gUnk_03000F6C = rnd;
        if (rnd == 0)
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

NONMATCH("asm/code_08078708/finish_when_no_target.s", void finish_when_no_target(struct SpriteObj *obj))
{
    if (obj->target == 0)
    {
        effect_spawn_at(0x2A9D, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        sprite_set_action(obj, 0, 0x4029, 0);
        ((struct Flags79Bit0 *)&obj->flags79)->bit0 = 0;
        obj->flags78 = (obj->flags78 & 0x1F) | 0x20;
        obj->unk18 = 0x1800;
        obj->callback = 0;
        gUnk_03000ED0 = 1;
        set_slot_value(0x55, -1);
    }
}
END_NONMATCH

NONMATCH("asm/code_08078708/toggle_mode_until_timer_done.s", void toggle_mode_until_timer_done(struct SpriteObj *obj))
{
    s32 t;
    s32 y;
    s32 *pos;

    if (obj->entry->flags & 8)
    {
        sprite_set_action(obj, 0, 0x4029, 0);
        t = obj->unkA0 - 1;
        obj->unkA0 = t;
        if (t > 0)
        {
            obj->mode = (obj->mode + 1) & 1;
            if (obj->mode == 0)
            {
                obj->unk84 = 0x7800;
                pos = &obj->unk88;
                y = 0x8000;
            }
            else
            {
                obj->unk84 = 0x7000;
                pos = &obj->unk88;
                y = 0x6000;
            }
            *pos = y;
            pos++;
            *pos = 0x1800;
            obj->flags79 |= 0x20;
            obj->unk7C = 0x300;
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
END_NONMATCH
void start_anim1_if_no_target(struct SpriteObj *obj);
ASM_FUNC("asm/code_08078708/spawn_child_4059_moving.s", void spawn_child_4059_moving(void))

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

NONMATCH("asm/code_08078708/timer_reset_pos_and_cb.s", void timer_reset_pos_and_cb(struct SpriteObj *obj))
{
    s32 y;
    s32 *pos;

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
                pos = &obj->unk88;
                y = 0x8000;
            }
            else
            {
                obj->unk84 = 0x7000;
                pos = &obj->unk88;
                y = 0x6000;
            }
            *pos = y;
            pos++;
            *pos = 0x1800;
            obj->flags79 |= 0x20;
            obj->unk7C = 0x300;
            obj->unk7A = 0;
            start_motion(obj);
            obj->callback = end_without_target;
        }
    }
}
END_NONMATCH

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
