#include "global.h"

struct FigWorld
{
    u8 filler0[0xB];
    u8 unkB;
    u8 flags0C;
    u8 filler0D[0x40 - 0xD];
    u32 unk40;
    u8 filler44[0x58 - 0x44];
    u8 *list[8];
    u8 filler78[0x7C - 0x78];
    u8 *cur;
    u8 *fig[2];
    u8 filler88[0x2C0 - 0x88];
    u8 flags2C0;
};

struct FigPart
{
    u8 filler0[8];
    u8 *unk8;
    u8 filler0C[4];
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u8 filler1C[0x28 - 0x1C];
    u8 *unk28;
};

struct FigBits
{
    u8 mode : 2;
    u8 bit2 : 1;
};

struct StateOwner
{
    u8 filler0[4];
    u32 callback;
    u8 filler8[0x12 - 8];
    u16 unk12;
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    s16 unk18;
};

struct ObjCounter121
{
    u8 filler0[0x121];
    u8 lo4 : 4;
    u8 counter : 3;
    u8 hi : 1;
};

struct Owner;

struct ObjMode12
{
    u8 filler0[0x12];
    u8 lo : 1;
    u8 mode : 2;
    u8 hi : 5;
};

struct FigObj
{
    u8 filler0[4];
    u32 unk4;
    u32 unk8;
    u8 filler0C[0x10 - 0xC];
    u16 unk10;
};

extern struct FigWorld *gUnk_03000FD8;
extern u8 *gWorkBuf;
u8 step_motion(struct FigPart *part);
void sprite_set_action(struct FigPart *part, u32 action, u32 arg, u32 extra);
u8 is_state_busy(void);
void set_slot_value(u32 slot, u32 value);
void state_fig_active_call(void);
void state_update_bits(void);
void state_obj_wait_then_select(struct Owner *owner);
void sprite_load_active_tables(void);
void hero_random_offset(u8 *obj);
void figure_pair_step(struct FigObj *obj);
void anim_bit100_release(u8 *obj);
void place_and_start_action5(u8 *obj, u32 a, u32 b);
void state_call_saved_if_flag(void);
void state_call_active_callback(void);
void figure_pair_place(struct StateOwner *owner);
void draw_obj_pose(u8 *obj);
void release_slot_objects(void);
void place_targets(u8 *obj, u32 a, u32 b, u32 c, u32 d);
void set_step_and_offsets(u8 *obj, u32 a, u32 b);
void state_show_figs(struct StateOwner *owner);
void state_wait_menu(struct StateOwner *owner);
void state_ready_check(struct StateOwner *owner);
void figure_pair_begin(struct StateOwner *owner);
void state_select_by_status(struct StateOwner *owner);

static inline u32 counter121_dec(u8 *obj)
{
    struct ObjCounter121 *c = (struct ObjCounter121 *)(obj + 0x121);
    u32 left = c->counter - 1;
    c->counter = left;
    return left;
}

NONMATCH("asm/code_080FD8D8/state_start.s", void state_start(struct StateOwner *owner))
{
    u8 *obj;

    obj = gUnk_03000FD8->cur;
    sprite_load_active_tables();
    owner->callback = owner->unk18 != 0 ? (u32)state_ready_check : (u32)state_wait_menu;
    obj[0x122] &= -0x21;
    if (*(u16 *)(obj + 0x11E) & 0x100)
    {
        if ((counter121_dec(obj) & 7) == 0)
        {
            anim_bit100_release(obj);
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)state_call_saved_if_flag;
        }
    }
    else if (*(u16 *)(obj + 0x11E) & 0x200)
    {
        if ((counter121_dec(obj) & 7) == 0)
        {
            place_and_start_action5(obj + 8, 0x10, 0x500);
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)state_call_active_callback;
            *(u16 *)(obj + 0x11E) &= 0xFCFF;
        }
    }
}
END_NONMATCH

void state_ready_check(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *fig1;
    u8 *fig2;
    void (*fn)(struct StateOwner *);

    world = gUnk_03000FD8;
    if (*(s16 *)(world->cur + 0xEC) < 0 && (world->flags0C & 0x80))
    {
        fig1 = world->fig[0] + 0x11E;
        fig2 = world->fig[1] + 0x11E;
        if (((*(u16 *)fig1 | *(u16 *)fig2) & 0x100) == 0)
        {
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)figure_pair_begin;
            return;
        }
    }
    owner->unk12 = 2;
    owner->unk14 = 2;
    fn = state_select_by_status;
    owner->callback = (u32)fn;
    *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_wait_menu;
    fn(owner);
}

ASM_FUNC("asm/code_080FD8D8/state_move.s", void state_move(void))

void state_wait_menu(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *obj;
    u8 *fig1;
    u8 *fig2;

    world = gUnk_03000FD8;
    obj = world->cur;
    if (*(s16 *)(obj + 0xEC) < 0 && (world->flags0C & 0x80))
    {
        fig1 = world->fig[0] + 0x11E;
        fig2 = world->fig[1] + 0x11E;
        if (((*(u16 *)fig1 | *(u16 *)fig2) & 0x100) == 0)
        {
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)figure_pair_begin;
            return;
        }
    }
    sprite_load_active_tables();
    if ((*(u16 *)(obj + 0x11E) & 3) != 0 && (*(u8 *)(obj + 0x120) & 7) != 1)
    {
        obj += 8;
        hero_random_offset(obj);
        draw_obj_pose(obj);
        owner->callback = (u32)state_obj_wait_then_select;
        return;
    }
    owner->callback = (u32)state_update_bits;
}

ASM_FUNC("asm/code_080FD8D8/state_update_bits.s", void state_update_bits(void))

ASM_FUNC("asm/code_080FD8D8/state_camera.s", void state_camera(void))

NONMATCH("asm/code_080FD8D8/state_message.s", void state_message(struct StateOwner *owner))
{
    struct FigWorld *world;
    struct FigPart *part;
    u32 bit;
    u32 mode;
    u32 slot;
    u32 sel;
    u32 text;

    if (is_state_busy())
        return;
    bit = 4;
    world = gUnk_03000FD8;
    if (bit & world->unkB)
        return;
    mode = bit & world->flags2C0;
    slot = 0x15;
    if (mode != 0)
        slot = 3;
    set_slot_value(slot, -1);
    world = gUnk_03000FD8;
    sel = 1;
    sel ^= ((u32)world->flags2C0 << 29) >> 31;
    part = (struct FigPart *)(world->fig[sel] + 8);
    text = 0x2068;
    if (bit & world->flags2C0)
        text = 0x2039;
    sprite_set_action(part, 2, text, 0);
    ((struct FigBits *)&gUnk_03000FD8->flags2C0)->mode = 2;
    owner->callback = (u32)state_fig_active_call;
}
END_NONMATCH

NONMATCH("asm/code_080FD8D8/state_setup_menu.s", void state_setup_menu(struct StateOwner *owner))
{
    struct FigWorld *world;
    u8 *menu;
    u8 flags;
    u32 bit;
    u32 extent;
    u32 offset;
    u32 text;
    void (*fn)(struct StateOwner *);

    flags = gUnk_03000FD8->unkB;
    bit = 4;
    if (bit & flags)
        return;
    release_slot_objects();
    world = gUnk_03000FD8;
    if (bit & world->flags2C0)
    {
        menu = world->fig[0] + 8;
        extent = 0x28;
        offset = 0x7C;
        text = 0x2039;
    }
    else
    {
        menu = world->fig[1] + 8;
        extent = 0x36;
        offset = 0x5C;
        text = 0x2068;
    }
    sprite_set_action((struct FigPart *)menu, 3, text, 0);
    place_targets(menu, extent, offset, 0, 0x100);
    set_step_and_offsets(menu, 0x400, 0x20);
    fn = state_show_figs;
    owner->callback = (u32)fn;
    fn(owner);
    set_slot_value(0x2B, -1);
}
END_NONMATCH

NONMATCH("asm/code_080FD8D8/figure_sync_step.s", void figure_sync_step(struct FigObj *obj))
{
    struct FigWorld *world;
    struct FigPart *part_other;
    struct FigPart *part_active;
    u8 *fig_other;
    u8 *fig_active;
    u32 shifted;
    u32 sel;
    u32 text;
    u32 addr;
    u32 addr2;
    u32 addr3;

    world = gUnk_03000FD8;
    shifted = (u32)world->flags2C0 << 29;
    sel = 1;
    sel ^= shifted >> 31;
    fig_other = world->fig[sel];
    fig_active = world->fig[shifted >> 31];
    part_other = (struct FigPart *)(fig_other + 8);
    part_active = (struct FigPart *)(fig_active + 8);
    if (part_other->unk8[0x23] != 0)
        fig_other[0x7D] = fig_active[0x7D] + 1;
    if (part_other->unk8[0x12] & 8)
    {
        gUnk_03000FD8->flags0C |= 0x80;
        text = 0x2069;
        if (gUnk_03000FD8->flags2C0 & 4)
            text -= 0x2F;
        sprite_set_action(part_other, 0, text, 0);
        text = 0x203B;
        if (gUnk_03000FD8->flags2C0 & 4)
            text += 0x2F;
        sprite_set_action(part_active, 0, text, 0);
        addr = (u32)part_other->unk28 + 0xD8;
        *(u32 *)addr = part_other->unk10;
        addr2 = addr + 4;
        *(u32 *)addr2 = part_other->unk14;
        addr3 = addr2 + 4;
        *(u32 *)addr3 = part_other->unk18;
        obj->unk4 = obj->unk8;
    }
}
END_NONMATCH

NONMATCH("asm/code_080FD8D8/figure_pair_begin.s", void figure_pair_begin(struct StateOwner *owner))
{
    struct FigWorld *world;
    struct FigPart *part_other;
    struct FigPart *part_active;
    u32 shifted;
    u32 bit;
    u32 four;
    u32 text;

    world = gUnk_03000FD8;
    shifted = (u32)world->flags2C0 << 29;
    bit = shifted >> 31;
    part_other = (struct FigPart *)(world->fig[1 ^ bit] + 8);
    part_active = (struct FigPart *)(world->fig[bit] + 8);
    world->flags0C &= 0x7F;
    four = 4;
    text = 0x2068;
    if (four & gUnk_03000FD8->flags2C0)
        text -= 0x2F;
    sprite_set_action(part_other, 5, text, 0);
    text = 0x2039;
    if (four & gUnk_03000FD8->flags2C0)
        text += 0x2F;
    sprite_set_action(part_active, 0xA, text, 0);
    ((struct ObjMode12 *)part_active->unk8)->mode = 1;
    ((struct ObjMode12 *)part_other->unk8)->mode = 1;
    part_active->unk10 -= 0x1600;
    owner->callback = (u32)figure_pair_place;
}
END_NONMATCH

NONMATCH("asm/code_080FD8D8/figure_pair_place.s", void figure_pair_place(struct StateOwner *owner))
{
    struct FigWorld *world;
    struct FigPart *part;
    u8 *other;
    u32 shifted;
    u32 sel;
    u32 extent;
    u32 offset;
    u8 mode;
    void (*fn)(struct StateOwner *);

    world = gUnk_03000FD8;
    shifted = (u32)world->flags2C0 << 29;
    sel = 1;
    sel ^= shifted >> 31;
    part = (struct FigPart *)(world->fig[sel] + 8);
    if (part->unk8[0x12] & 8)
    {
        other = world->fig[shifted >> 31] + 8;
        sprite_set_action(part, 6, 0, 0);
        sprite_set_action((struct FigPart *)other, 1, 0, 0);
        mode = gUnk_03000FD8->flags2C0 & 4;
        extent = 0x28;
        if (mode != 0)
            extent = 0x36;
        offset = 0x7C;
        if (mode != 0)
            offset = 0x5C;
        place_targets((u8 *)part, extent, offset, 0, 0x100);
        set_step_and_offsets((u8 *)part, 0x400, 0x20);
        fn = (void (*)(struct StateOwner *))figure_pair_step;
        owner->callback = (u32)fn;
        fn(owner);
        set_slot_value(0x2B, -1);
    }
}
END_NONMATCH

void figure_pair_step(struct FigObj *obj)
{
    struct FigWorld *world;
    struct FigPart *part;
    u32 addr;
    u32 addr2;
    u32 addr3;
    u32 sel;
    u32 bit;

    world = gUnk_03000FD8;
    bit = ((u32)world->flags2C0 << 29) >> 31;
    sel = 1;
    sel ^= bit;
    part = (struct FigPart *)(world->fig[sel] + 8);
    if (!step_motion(part))
    {
        sprite_set_action(part, 0, -1, 0);
        addr = (u32)part->unk28 + 0xD8;
        *(u32 *)addr = part->unk10;
        addr2 = addr + 4;
        *(u32 *)addr2 = part->unk14;
        addr3 = addr2 + 4;
        *(u32 *)addr3 = part->unk18;
        ((struct FigBits *)&gUnk_03000FD8->flags2C0)->mode = 2;
        obj->unk4 = *(u32 *)(gWorkBuf + 0x8E58);
        obj->unk10 = 0xA;
    }
}

ASM_FUNC("asm/code_080FD8D8/state_select_targets.s", void state_select_targets(void))

ASM_FUNC("asm/code_080FD8D8/figure_vectors_setup.s", void figure_vectors_setup(void))

ASM_FUNC("asm/code_080FD8D8/figure_status_enter.s", void figure_status_enter(void))

ASM_FUNC("asm/code_080FD8D8/hero_select_begin.s", void hero_select_begin(void))
