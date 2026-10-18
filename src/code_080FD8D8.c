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

extern struct FigWorld *gWorld;
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
s32 signed_byte_e4(void *part);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void state_restore_saved_when_flag(void);

ASM_FUNC("asm/code_080FD8D8/state_start.s", void state_start(void))

void state_ready_check(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *fig1;
    u8 *fig2;
    void (*fn)(struct StateOwner *);

    world = gWorld;
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

void state_move(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *obj;
    u8 *fixed;
    s32 mode;

    world = gWorld;
    if (world->unk40 != 0)
        return;
    obj = world->cur;
    fixed = obj + 8;
    mode = owner->unk18;
    switch (mode)
    {
    case 1:
        popup_digit_spawn((s16)owner->unk12, *(s32 *)(fixed + 0x38) / 0x100, *(s32 *)(fixed + 0x3C) / 0x100, signed_byte_e4(obj), mode);
        break;
    case 2:
        popup_digit_spawn((s16)owner->unk14, *(s32 *)(fixed + 0x38) / 0x100, *(s32 *)(fixed + 0x3C) / 0x100, signed_byte_e4(obj), 1);
        break;
    case 3:
        popup_digit_spawn((s16)owner->unk12, *(s32 *)(fixed + 0x38) / 0x100 + 0xC, *(s32 *)(fixed + 0x3C) / 0x100 + 4, signed_byte_e4(obj), 1);
        popup_digit_spawn((s16)owner->unk14, *(s32 *)(fixed + 0x38) / 0x100 - 0xC, *(s32 *)(fixed + 0x3C) / 0x100 - 4, signed_byte_e4(obj), 1);
        break;
    }
    if (owner->unk18 & 1)
    {
        *(u16 *)(obj + 0xF6) += owner->unk12;
        if ((s16)*(u16 *)(obj + 0xF6) > *(s16 *)(obj + 0xF8))
            *(u16 *)(obj + 0xF6) = *(u16 *)(obj + 0xF8);
    }
    if (owner->unk18 & 2)
    {
        *(u16 *)(obj + 0xFC) += owner->unk14;
        if ((s16)*(u16 *)(obj + 0xFC) > *(s16 *)(obj + 0xFE))
            *(u16 *)(obj + 0xFC) = *(u16 *)(obj + 0xFE);
    }
    owner->callback = (u32)state_restore_saved_when_flag;
}

void state_wait_menu(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *obj;
    u8 *fig1;
    u8 *fig2;

    world = gWorld;
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

ASM_FUNC("asm/code_080FD8D8/state_message.s", void state_message(void))

void state_setup_menu(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *menu;
    u32 extent;
    u32 offset;
    u32 text;
    void (*fn)(struct StateOwner *);

    if (gWorld->unkB & 4)
        return;
    release_slot_objects();
    world = gWorld;
    if (world->flags2C0 & 4)
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


ASM_FUNC("asm/code_080FD8D8/figure_sync_step.s", void figure_sync_step(void))

ASM_FUNC("asm/code_080FD8D8/figure_pair_begin.s", void figure_pair_begin(struct StateOwner *owner))

NONMATCH("asm/code_080FD8D8/figure_pair_place.s", void figure_pair_place(struct StateOwner *owner))
{
    struct FigWorld *world;
    struct FigPart *part;
    struct FigPart *part_active;
    u32 shifted;
    u32 extent;
    u32 offset;
    u8 mode;
    void (*fn)(struct StateOwner *);

    world = gWorld;
    shifted = (u32)world->flags2C0 << 29;
    part = (struct FigPart *)(world->fig[1 ^ (shifted >> 31)] + 8);
    if (part->unk8[0x12] & 8)
    {
        part_active = (struct FigPart *)(world->fig[shifted >> 31] + 8);
        sprite_set_action(part, 6, 0, 0);
        sprite_set_action(part_active, 1, 0, 0);
        mode = gWorld->flags2C0 & 4;
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

    world = gWorld;
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
        ((struct FigBits *)&gWorld->flags2C0)->mode = 2;
        obj->unk4 = *(u32 *)(gWorkBuf + 0x8E58);
        obj->unk10 = 0xA;
    }
}

ASM_FUNC("asm/code_080FD8D8/state_select_targets.s", void state_select_targets(void))

ASM_FUNC("asm/code_080FD8D8/figure_vectors_setup.s", void figure_vectors_setup(void))

ASM_FUNC("asm/code_080FD8D8/figure_status_enter.s", void figure_status_enter(void))
ASM_FUNC("asm/code_080FD8D8/hero_select_begin.s", void hero_select_begin(void))
