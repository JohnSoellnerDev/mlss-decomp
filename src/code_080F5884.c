#include "global.h"
#include "ram_routines.h"

struct World
{
    u8 filler0[4];
    u8 unk4;
    u8 filler5[0x24 - 5];
    void (*callback24)(void);
    u8 filler28[0x58 - 0x28];
    u8 *slots[8];
    u8 *unk78;
    u8 *curObj;
    u8 *unk80;
    u8 *unk84;
    u8 filler88[0x248 - 0x88];
    u8 *unk248;
    s32 angle;
    u8 filler250[0x2B4 - 0x250];
    u16 unk2B4;
    u8 filler2B6[0x2BF - 0x2B6];
    u8 unk2BF;
    u8 filler2C0[0x2E0 - 0x2C0];
    u32 unk2E0[3];
    u8 filler2EC[0x2EC - 0x2EC];
    u16 unk2EC;
    u16 unk2EE;
    u8 filler2F0[0x310 - 0x2F0];
    u8 *unk310;
    u8 filler314[0x346 - 0x314];
    u8 unk346;
    u8 filler347[0x350 - 0x347];
    u8 unk350;
    u8 unk351;
    u8 filler352[0x354 - 0x352];
    u16 unk354;
    u16 unk356;
    u16 unk358;
    u16 unk35A;
    u16 unk35C;
    u16 unk35E;
};

extern struct World *gUnk_03000FD8;

struct SlotByteFD
{
    u8 filler0[0xFD];
    u8 lo : 4;
    u8 hi : 4;
};

struct OwnerState
{
    u8 filler0[0x179C];
    u8 slotCode;
};

struct Owner
{
    u8 filler0[0x2C];
    struct OwnerState *state;
};

void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, u8 *owner);
void list_splice_pair(u8 *entry, u8 *head);
s32 anim_set_motion_a(u8 *obj, u16 speed, s32 extra);
s32 anim_set_motion_b(u8 *obj, u16 speed, s32 extra);
s32 anim_set_motion_c(u8 *obj, u16 speed, s32 extra);
void obj_stop(u8 *obj);
void obj_draw_part(u8 *obj, s32 amount, s32 kind, s32 a3, s32 a4, s32 a5);
s32 obj_last_x(s32 angle);
s32 obj_last_y(s32 angle);
extern u8 *gWorkBuf;
extern u8 gUnk_02000080[];
void menu_open_setup(void);
void sub_08115F54(u8 *a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
extern u8 gClaimKindTable[];
void slot_pos_offset(u8 *obj, s32 idx, s32 *x, s32 *y, s32 z);
s32 cmd_spawn_obj(u8 *table, u32 art, u32 sub, u32 width, s32 a, s32 b, s32 c);
void state_status_release(u8 *value);
void release_marker_object(void);
void spawn_linked_children(void);
void spawn_timer_child(void);
void sprite_set_action(u8 *obj, u32 a, u32 b, u32 c);
void obj_track_target(u8 *target);
void state_update_hero_markers(void);
void anim_dispatch(u8 *obj, s32 value, u32 a, u32 b);
extern const u8 gPartTemplates[];
u32 lookup_or_alloc_13b4(u32 a, u32 key, s32 b, s32 c);
void release_block(void *block);
void place_part_in_slot(u32 effect, s32 slot, s32 x, s32 y, s32 z, s32 a, s32 b);
void noop_callback_0808261C(void);
void run_slot_script(u32 index);

ASM_FUNC("asm/code_080F5884/cmd_set_var_from_slot_field.s", void cmd_set_var_from_slot_field(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_set_field.s", void cmd_slot_set_field(void))

NONMATCH("asm/code_080F5884/cmd_slot_link_list.s", u32 cmd_slot_link_list(u32 unused0, u32 unused1, u32 *cmd))
{
    struct World *world;
    u8 *entry;
    u8 *obj;
    u8 *flags;
    s32 mode;
    s32 value;
    s32 f;
    s32 bit;

    world = gUnk_03000FD8;
    entry = world->slots[*cmd++];
    obj = entry + 8;
    mode = *cmd++;
    switch (mode)
    {
    case 0:
        flags = entry + 0x80;
        f = *flags;
        bit = (((u32)f << 29) >> 31) << 3;
        f = (f & -9) | bit;
        f = f | 4;
        *flags = f;
        value = *cmd;
        effect_spawn_at(value, *(s32 *)(obj + 0x38) / 0x100, *(s32 *)(obj + 0x3C) / 0x100,
                        *(s32 *)(obj + 0x40) / 0x100, obj);
        break;
    case 1:
        list_splice_pair(entry, world->slots[*cmd]);
        break;
    case 2:
        *(u16 *)(entry + 0x124) = *cmd;
        break;
    }
    return 1;
}
END_NONMATCH

u32 slot_cmds_cmd_slot_set_motion(u32 unused0, u32 unused1, u32 *cmd)
{
    struct World *world;
    u32 *cursor;
    u8 *entry;
    s32 mode;

    cursor = cmd;
    world = gUnk_03000FD8;
    entry = world->slots[*cursor++];
    mode = *cursor++;
    switch (mode)
    {
    case 0:
        *(u16 *)(entry + 0x11E) = *(u16 *)(entry + 0x11E) & 0xFFF3;
        anim_set_motion_a(entry, *(u16 *)cursor, 0x7FFFFFFF);
        break;
    case 1:
        *(u16 *)(entry + 0x11E) = *(u16 *)(entry + 0x11E) & 0xFFCF;
        anim_set_motion_b(entry, *(u16 *)cursor, 0x7FFFFFFF);
        break;
    case 2:
        *(u16 *)(entry + 0x11E) = *(u16 *)(entry + 0x11E) & 0xFF3F;
        anim_set_motion_c(entry, *(u16 *)cursor, 0x7FFFFFFF);
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F5884/cmd_slot_spawn_a.s", void cmd_slot_spawn_a(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_spawn_b.s", void cmd_slot_spawn_b(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_release.s", void cmd_slot_release(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_move_step.s", void cmd_slot_move_step(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_set_bits.s", void cmd_slot_set_bits(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_set_bit_pair.s", void cmd_slot_set_bit_pair(void))

ASM_FUNC("asm/code_080F5884/cmd_world_slot.s", void cmd_world_slot(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_table.s", void cmd_slot_table(void))

ASM_FUNC("asm/code_080F5884/slot_follow_source.s", void slot_follow_source(void))

ASM_FUNC("asm/code_080F5884/cmd_set_ctx_fields.s", void cmd_set_ctx_fields(void))

ASM_FUNC("asm/code_080F5884/cmd_world_scroll.s", void cmd_world_scroll(void))

NONMATCH("asm/code_080F5884/cmd_set_scroll.s", u32 cmd_set_scroll(u32 unused0, u8 *obj, u32 *cmd))
{
    struct World *world;
    u32 *cursor;
    u32 flags;
    s32 bits;
    u16 *a0;

    gUnk_03000FD8->unk354 = obj_last_x(gUnk_03000FD8->angle);
    gUnk_03000FD8->unk356 = obj_last_y(gUnk_03000FD8->angle);
    world = gUnk_03000FD8;
    cursor = cmd;
    world->unk358 = *cursor++;
    world->unk35A = *cursor++;
    flags = *cursor;
    if (flags & 0x40)
    {
        bits = flags;
        bits &= 0xF;
    }
    else
    {
        bits = flags << 4;
    }
    world->unk35C = bits;
    gUnk_03000FD8->unk35E = 0;
    a0 = (u16 *)(obj + 0xA0);
    *a0 = *a0 | (cursor[1] ? 0x500 : 0x100);
    return 0;
}
END_NONMATCH

ASM_FUNC("asm/code_080F5884/cmd_flags_show.s", void cmd_flags_show(void))

u32 list_slot_ops_cmd_world_mode(u32 unused0, u32 unused1, s32 *cmd)
{
    u8 *a;

    switch (*cmd)
    {
    case 0:
        gUnk_03000FD8->unk350 = gUnk_03000FD8->unk351 = 0x80;
        break;
    case 1:
        state_status_release(gUnk_03000FD8->unk310);
        release_marker_object();
        spawn_linked_children();
        spawn_timer_child();
        sprite_set_action(gUnk_03000FD8->curObj + 8, 0, 0, 0);
        gUnk_03000FD8->unk346 = 0xFF;
        a = gUnk_03000FD8->unk80;
        if (a != 0 && *(s16 *)(a + 0xF6) != 0)
        {
            obj_track_target(a + 8);
        }
        a = gUnk_03000FD8->unk84;
        if (a != 0 && *(s16 *)(a + 0xF6) != 0)
        {
            obj_track_target(a + 8);
        }
        gUnk_03000FD8->callback24 = state_update_hero_markers;
        break;
    case 2:
        gUnk_03000FD8->unk2B4 = 0;
        *((s8 *)gUnk_03000FD8 + 0x2C1) &= -9;
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F5884/draw_list_item.s", void draw_list_item(void))

ASM_FUNC("asm/code_080F5884/cmd_spawn_item.s", void cmd_spawn_item(void))
