#include "global.h"

struct ScrollNode
{
    struct ScrollNode *next;
    s32 value;
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
    u8 filler75[0x77 - 0x75];
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 unk7A;
    u16 unk7C;
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

extern struct SpriteWorld *gUnk_03000FD8;
extern u8 gUnk_03000ED0;
extern u32 gUnk_03000F6C;
extern u8 gUnk_03000F68;
extern struct SpriteObj *gUnk_03000F60;

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

ASM_FUNC("asm/code_08078708/unlink_actor_pair.s", void unlink_actor_pair(void))

s32 update_positions_to_target(struct SpriteObj *obj);
void link_actor_if_free(struct SpriteObj *obj);
void ramp_values_then_remove(struct SpriteObj *obj);
ASM_FUNC("asm/code_08078708/spawn_child_4025_adopt.s", void spawn_child_4025_adopt(void))

ASM_FUNC("asm/code_08078708/ramp_values_then_remove.s", void ramp_values_then_remove(struct SpriteObj *obj))

ASM_FUNC("asm/code_08078708/link_actor_if_free.s", void link_actor_if_free(struct SpriteObj *obj))

ASM_FUNC("asm/code_08078708/update_positions_to_target.s", s32 update_positions_to_target(struct SpriteObj *obj))

s32 slot_step_or_remove(struct SpriteObj *obj);
s32 actor_link_anim_minus1(void);
void init_timer_slot(struct SpriteObj *obj);
ASM_FUNC("asm/code_08078708/set_anim_14_spawn_child.s", void set_anim_14_spawn_child(void))

ASM_FUNC("asm/code_08078708/tick_mode_and_remove_when_low.s", void tick_mode_and_remove_when_low(struct SpriteObj *obj))

ASM_FUNC("asm/code_08078708/slot_step_or_remove.s", s32 slot_step_or_remove(struct SpriteObj *obj))

ASM_FUNC("asm/code_08078708/timer_spawn_child_402f.s", void timer_spawn_child_402f(void))

ASM_FUNC("asm/code_08078708/pick_side_random_then_anim.s", void pick_side_random_then_anim(struct SpriteObj *obj))

u32 destroy_sprite_entry(struct SpriteEntry *ent, u32 kind);
void slot_place_and_move(struct SpriteObj *obj);
void scroll_queue_and_cull(struct SpriteObj *obj);
void scroll_and_cull(struct SpriteObj *obj);
ASM_FUNC("asm/code_08078708/spawn_child_4030_with_nodes.s", void spawn_child_4030_with_nodes(void))

ASM_FUNC("asm/code_08078708/slot_place_and_move.s", void slot_place_and_move(struct SpriteObj *obj))

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
        pa = (u8 *)gUnk_03000FD8->slots[gUnk_03000F68 + 6] + 8;
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

ASM_FUNC("asm/code_08078708/finish_when_no_target.s", void finish_when_no_target(struct SpriteObj *obj))

ASM_FUNC("asm/code_08078708/toggle_mode_until_timer_done.s", void toggle_mode_until_timer_done(void))

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

ASM_FUNC("asm/code_08078708/timer_reset_pos_and_cb.s", void timer_reset_pos_and_cb(struct SpriteObj *obj))

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
        struct SpriteObj *source = gUnk_03000FD8->slots[obj->phase + 6]->child;

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
