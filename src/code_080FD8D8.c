#include "global.h"
#include "ram_routines.h"

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

struct ObjFlags122
{
    u8 filler0[0x122];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct ObjMode7E
{
    u8 filler0[0x7E];
    u8 lo : 1;
    u8 mode : 2;
    u8 hi : 5;
};

struct FigFlags2C0
{
    u8 filler0[0x2C0];
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct ObjFlags11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
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
extern u8 gSys[];
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];
extern u8 gUnk_02000440[];
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
void refresh_sprite_state(void);
void noop_stub_080826AC(void);
void world_objects_tick(void);
void state_menu_open_active(void);
void state_select_targets(void);
void create_status_objects(void);
void hero_select_begin(void);

NONMATCH("asm/code_080FD8D8/state_start.s", void state_start(struct StateOwner *owner))
{
    u32 left;
    u8 *obj;
    u16 *status;
    u32 cb;
    s32 unk18;

    obj = gWorld->cur;
    sprite_load_active_tables();
    unk18 = owner->unk18;
    cb = (u32)state_wait_menu;
    if (unk18 != 0)
        cb = (u32)state_ready_check;
    owner->callback = cb;
    ((struct ObjFlags122 *)obj)->bit5 = 0;
    status = (u16 *)(obj + 0x11E);
    if (*status & 0x100)
    {
        left = ((struct ObjCounter121 *)obj)->counter - 1;
        ((struct ObjCounter121 *)obj)->counter = left;
        if ((left & 7) == 0)
        {
            anim_bit100_release(obj);
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)state_call_saved_if_flag;
        }
    }
    else if (*status & 0x200)
    {
        left = ((struct ObjCounter121 *)obj)->counter - 1;
        ((struct ObjCounter121 *)obj)->counter = left;
        if ((left & 7) == 0)
        {
            place_and_start_action5(obj + 8, 0x10, 0x500);
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)state_call_active_callback;
            *status &= 0xFCFF;
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

void state_camera(struct StateOwner *owner)
{
    u8 *obj;
    u8 *fig1;
    u8 *fig2;
    u32 ready;
    s32 mask;
    s32 off;
    void (*fn)(struct StateOwner *);

    obj = gWorld->cur;
    ready = is_state_busy();
    if (ready != 0)
        return;
    if (((struct ObjMode7E *)obj)->mode == 2)
    {
        if (*(u32 *)(obj + 0xE8) != 0)
        {
            fig1 = gWorld->fig[0];
            fig2 = gWorld->fig[1];
            if (fig1 != 0 && ((struct ObjMode7E *)fig1)->mode == 1)
                *(u32 *)(fig1 + 0x74) = ready;
            if (fig2 != 0 && ((struct ObjMode7E *)fig2)->mode == 1)
                *(u32 *)(fig2 + 0x74) = 0;
            ((struct FigFlags2C0 *)gWorld)->bit4 = 0;
            noop_stub_080826AC();
            *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_menu_open_active;
            owner->callback = (u32)state_menu_open_active;
            off = 0x122;
            mask = ~0x40;
            *(gWorld->list[0] + off) &= mask;
            *(gWorld->list[1] + off) &= mask;
            *(gWorld->list[2] + off) &= mask;
            *(gWorld->list[3] + off) &= mask;
            *(gWorld->list[4] + off) &= mask;
            *(gWorld->list[5] + off) &= mask;
            *(gWorld->list[6] + off) &= mask;
            *(gWorld->list[7] + off) &= mask;
            ((struct FigFlags2C0 *)gWorld)->bit3 = 1;
        }
        else
        {
            owner->callback = (u32)world_objects_tick;
        }
    }
    else
    {
        refresh_sprite_state();
        fn = (void (*)(struct StateOwner *))state_select_targets;
        owner->callback = (u32)fn;
        fn(owner);
    }
}

NONMATCH("asm/code_080FD8D8/state_message.s", void state_message(struct StateOwner *owner))
{
    struct FigWorld *world;
    u8 *fig;
    u8 *pos;
    u32 text;
    u32 bit;
    u32 mode;
    u32 slot;

    if (is_state_busy())
        return;
    if (gWorld->unkB & 4)
        return;
    slot = (gWorld->flags2C0 & 4) ? 3 : 0x15;
    set_slot_value(slot, -1);
    world = gWorld;
    mode = world->flags2C0;
    fig = world->fig[1 ^ ((mode << 29) >> 31)];
    pos = fig + 8;
    text = 0x2068;
    bit = 4;
    if (mode & bit)
        text -= 0x2F;
    sprite_set_action((struct FigPart *)pos, 2, text, 0);
    ((struct FigBits *)&gWorld->flags2C0)->mode = 2;
    owner->callback = (u32)state_fig_active_call;
}
END_NONMATCH

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


NONMATCH("asm/code_080FD8D8/figure_sync_step.s", void figure_sync_step(struct FigObj *obj))
{
    struct FigWorld *world;
    struct FigPart *part_other;
    struct FigPart *part_active;
    u8 *fig_other;
    u8 *fig_active;
    u32 shifted;
    u32 text;
    u32 v;
    u32 addr;
    u32 addr2;
    u32 addr3;

    world = gWorld;
    shifted = (u32)world->flags2C0 << 29;
    fig_other = world->fig[1 ^ (shifted >> 31)];
    fig_active = world->fig[shifted >> 31];
    part_other = (struct FigPart *)(fig_other + 8);
    part_active = (struct FigPart *)(fig_active + 8);
    if (part_other->unk8[0x23] != 0)
    {
        v = fig_active[0x7D] + 1;
        fig_other[0x7D] = v;
    }
    if (part_other->unk8[0x12] & 8)
    {
        gWorld->flags0C |= 0x80;
        text = 0x2069;
        if (gWorld->flags2C0 & 4)
            text -= 0x2F;
        sprite_set_action(part_other, 0, text, 0);
        text = 0x203B;
        if (gWorld->flags2C0 & 4)
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
    u8 *other_obj;
    u8 *active_obj;
    u32 shifted;
    u32 text;

    world = gWorld;
    shifted = (u32)world->flags2C0 << 29;
    part_other = (struct FigPart *)(world->fig[1 ^ (shifted >> 31)] + 8);
    part_active = (struct FigPart *)(world->fig[shifted >> 31] + 8);
    world->flags0C &= 0x7F;
    text = 0x2068;
    if (gWorld->flags2C0 & 4)
        text -= 0x2F;
    sprite_set_action(part_other, 5, text, 0);
    text = 0x2039;
    if (gWorld->flags2C0 & 4)
        text += 0x2F;
    sprite_set_action(part_active, 0xA, text, 0);
    other_obj = part_other->unk8;
    active_obj = part_active->unk8;
    ((struct ObjMode12 *)active_obj)->mode = 1;
    ((struct ObjMode12 *)other_obj)->mode = 1;
    part_active->unk10 -= 0x1600;
    owner->callback = (u32)figure_pair_place;
}
END_NONMATCH

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

NONMATCH("asm/code_080FD8D8/figure_status_enter.s", void figure_status_enter(struct StateOwner *owner))
{
    struct FigWorld *world;
    struct FigWorld *world2;
    struct FigWorld **pworld;
    u8 *fig;
    u8 *fig1;
    u8 *fig2;
    u32 idx;
    u32 entry;
    void (*copy)(u8 *, u8 *, u32);

    world = gWorld;
    fig1 = world->fig[0];
    if (fig1 != 0 && (fig1[0x81] & 0x10) != 0)
        return;
    if (world->fig[1] != 0 && (world->fig[1][0x81] & 0x10) != 0)
        return;
    fig = fig1;
    if (fig != 0 && ((struct ObjMode7E *)fig)->mode == 1)
    {
        if (world->cur == fig)
            sprite_set_action((struct FigPart *)(fig + 8), 1, -1, 0);
        else
            sprite_set_action((struct FigPart *)(fig + 8), -1, -1, 0);
        ((struct ObjFlags11 *)((struct FigPart *)fig)->unk10)->bit6 = 0;
    }
    pworld = &gWorld;
    world2 = *pworld;
    fig2 = world2->fig[1];
    if (fig2 != 0 && ((struct ObjMode7E *)fig2)->mode == 1)
    {
        if (world2->cur == fig2)
            sprite_set_action((struct FigPart *)(fig2 + 8), 1, -1, 0);
        else
            sprite_set_action((struct FigPart *)(fig2 + 8), -1, -1, 0);
        ((struct ObjFlags11 *)((struct FigPart *)fig2)->unk10)->bit6 = 0;
    }
    copy = (void (*)(u8 *, u8 *, u32))(gMemRoutines + ((u32)iwram_copy - (u32)iwram_fill));
    idx = ((gSys[0x888] & 6) != 0) ? 0x20 : 0x1E;
    entry = *(u16 *)((u8 *)gGlyphIndexTable + idx);
    copy((u8 *)gMapTable + gMapTable[entry], gUnk_02000440, 0x20);
    *(u16 *)gSys = *(u16 *)gSys | 0x4000;
    create_status_objects();
    owner->callback = (u32)hero_select_begin;
}
END_NONMATCH
ASM_FUNC("asm/code_080FD8D8/hero_select_begin.s", void hero_select_begin(void))
