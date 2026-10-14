#include "global.h"

struct ObjList
{
    u8 filler0[0x28];
    void *entries[(0x208 - 0x28) / 4];
    u8 unk208;
    u8 filler209[0x25C - 0x209];
    u32 unk25C;
    u8 filler260[0x304 - 0x260];
    void *table304;
};

struct OwnerObj
{
    u8 filler0[0x14];
    struct ObjList *list;
};

struct ObjEntry
{
    u8 filler0[0x25C];
    u32 unk25C;
};

struct ObjCmd
{
    u32 index;
    u16 value;
};

void call_9a20_pair(struct ObjList *list, u32 mode);
void process_obj_item_list(struct ObjList *list);
void obj_request_entry(void *obj, u32 request);
void obj_set_state(void *obj, u32 state);
void obj_apply_state_a(void *obj);
void obj_commit_sprite(void *obj);
void obj_activate_sprite(void *obj);
void create_state_sprite(void *table, u32 value);
void map_anim2_start_0805BCCC(void *table, u32 value);
void map_layer3_entry(void *table, u32 value);
void set_state_sprite_value(void *table, u32 a, u32 b);
u32 sub_0803C51C(struct ObjList *list);
u32 obj_flag_291_bit0(struct ObjList *list);
u32 object_mode_check(struct ObjList *list);
void set_obj_12c_field(struct ObjList *list, s32 value);
void set_obj_flags_12c_bit0(struct ObjList *list);
void set_obj_fc_state(struct ObjList *list, s32 value);
void call_7380_for_state(struct ObjList *list, s32 value);
void set_scene_loaded(struct ObjList *list);
void set_scene_flag_20(struct ObjList *list);
void obj_set_sprite_slot(void *obj, u32 slot);
void obj_release_entry(void *obj, u32 flag);
void obj_entry_release(void *obj);
void obj_entry_activate(void *obj);
void set_sub_pair_action(struct ObjList *list, u32 a, u32 b);
void reset_obj_fc(struct ObjList *list, u32 b);
void finish_obj_fc(struct ObjList *list);
void obj_set_move_params(void *obj, u32 a, u32 b, s32 c, u32 d);
void set_var(u32 a, void *b, void *c, u32 d, s32 e, u32 f);
void set_object_mode(struct ObjList *list, u32 mode, u32 a, u32 packed, u32 x, u32 y);
void set_flag(u32 a, u32 b, u32 c, u32 d);
void obj_set_motion_a_08046AA4(void *obj, u32 a, u32 b, s32 c, u32 d);
void obj_set_motion_b(void *obj, u32 a, u32 b, s32 c, u32 d);
u32 build_menu_list(u32 a, u32 b, u32 c);
u32 char_menu_list(u32 a, u32 b, u32 c);
u32 obj_has_pending_bits(struct ObjList *list);
void tick_obj_timer(void *timer, void *state);
u32 contacts_obj_start_move(void *a, u32 b, u32 c, s32 d, s32 e, s32 f, u32 g);
void obj_set_move(void *obj, u32 v, u32 b);
void obj_set_limit(void *obj, s32 v);
void obj_stop_motion(void *obj);
void set_flag_a0_2(void *a, void *b, u8 *p, u32 c);
void *tile_ptr_at(void *table, s32 x, s32 y);
void obj_set_or_add_pos(void *obj, s32 a, s32 b, s32 c, u32 d, u32 e);
void camera_track_entity(void *list, u32 field, u32 zero);
void swap_sub_pair(struct ObjList *list, s32 a);
void start_sub_pair_step(struct ObjList *list, s32 a);
void set_208_bit1_copy_100(struct ObjList *list);
void clear_208_bit1_set_351_bit6(struct ObjList *list);
void pair_start_state_27(struct ObjList *list);
void pair_start_state_2a(struct ObjList *list);
u32 sub_0803C4B4(struct ObjList *list);
u32 sub_0803C47C(struct ObjList *list);

typedef u32 (*KindHandler)(struct ObjList *list, void *slot);
extern const u32 gKindHandlerTable[];

extern u8 *gUnk_03000FD0;
extern u8 *gUnk_03000FC0;
extern u8 gUnk_03000DD0[];
extern const u8 gLayer3Table[];

ASM_FUNC("asm/code_080F0C04/step_status_1f8.s", void step_status_1f8(void))

ASM_FUNC("asm/code_080F0C04/task_slot_release.s", void task_slot_release(void))

ASM_FUNC("asm/code_080F0C04/task_slot_attach.s", void task_slot_attach(void))

ASM_FUNC("asm/code_080F0C04/set_flag_a0_2.s", void set_flag_a0_2(void *a, void *b, u8 *p, u32 c))

u32 dispatch_kind_handler(struct OwnerObj *owner, u32 value, u32 *dst, u32 *block)
{
    u32 *slot = (u32 *)(gUnk_03000FD0 + 0x480);
    KindHandler *table;
    u32 result;
    if (value != 0)
        *slot = value;
    table = (KindHandler *)gKindHandlerTable;
    result = table[block[0]](owner->list, slot);
    if ((u8)result == block[1])
        *dst = block[2];
    return 1;
}

u32 clear_effect_bit_54a(void *unused0, void *unused1, void *unused2, u32 *block)
{
    u32 flag = *block++;
    if (flag == 0)
    {
        u16 *flags = (u16 *)((u8 *)gUnk_03000FD0 + 0x54A);
        u32 mask = 1 << *block;
        *flags &= ~mask;
    }
    return 1;
}

ASM_FUNC("asm/code_080F0C04/setup_effect_record.s", void setup_effect_record(void))

ASM_FUNC("asm/code_080F0C04/set_var_from_halfwords.s", void set_var_from_halfwords(void))

u32 set_var_from_point(struct OwnerObj *owner, u8 *dst, s32 *pt, u32 value)
{
    u8 *vars;
    void *tile;
    u8 *target;
    if (pt[1] != 0)
    {
        pt[2] >>= 4;
        pt[3] >>= 4;
    }
    tile =tile_ptr_at(owner->list->table304, ((s16 *)pt)[4], ((s16 *)pt)[6]);
    target = dst + 0x18;
    vars = gUnk_03000FC0 + 0x38C;
    set_var(value, target, vars, 0, ((s16 *)pt)[0], (u32)tile);
    return 1;
}

ASM_FUNC("asm/code_080F0C04/cmd_set_var_direction.s", void cmd_set_var_direction(void))

u32 process_list_if_idle(struct OwnerObj *owner, void *unused, u32 *flag_ref)
{
    if (*flag_ref == 0)
        process_obj_item_list(owner->list);
    return 1;
}

u32 list_query_copy_word(struct OwnerObj *owner, u32 *dst)
{
    u32 result;
    if ((u8)sub_0803C51C(owner->list) == 0)
    {
        result = 1;
    }
    else
    {
        dst[0] = dst[5];
        result = 0;
    }
    return result;
}

u32 list_probe_copy_word(struct OwnerObj *owner, u32 *dst)
{
    u32 result;
    if ((u8)obj_flag_291_bit0(owner->list) == 0)
    {
        result = 1;
    }
    else
    {
        dst[0] = dst[5];
        result = 0;
    }
    return result;
}

u32 obj_fc_command(struct OwnerObj *owner, void *unused, u32 *block)
{
    s32 kind = *block++;
    switch (kind)
    {
    case 0:
        set_sub_pair_action(owner->list, *block, 0);
        break;
    case 1:
        reset_obj_fc(owner->list, 0);
        break;
    case 2:
        finish_obj_fc(owner->list);
        break;
    }
    return 1;
}

u32 obj_mode_notify(struct OwnerObj *owner, void *unused, u32 *cmd)
{
    u32 mode = *cmd;
    if (mode != 5)
        call_9a20_pair(owner->list, mode);
    return 1;
}

ASM_FUNC("asm/code_080F0C04/obj_list_command.s", void obj_list_command(void))

u32 obj_mode_dispatch(struct OwnerObj *owner, u32 *dst, s32 *cmd)
{
    s32 v = *cmd;
    s32 hi = (v >> 1) & 0xF;
    switch (hi)
    {
    case 0:
        switch (v & 1)
        {
        case 0:
            pair_start_state_27(owner->list);
            break;
        case 1:
            if ((u8)sub_0803C4B4(owner->list) != 0)
            {
                dst[0] = dst[5];
                return 0;
            }
            break;
        }
        break;
    case 1:
        switch (hi & v)
        {
        case 0:
            pair_start_state_2a(owner->list);
            break;
        case 1:
            if ((u8)sub_0803C47C(owner->list) != 0)
            {
                dst[0] = dst[5];
                return 0;
            }
            break;
        }
        break;
    }
    return 1;
}

u32 obj_bit4_copy_word(struct OwnerObj *owner, u32 *dst)
{
    u32 result;
    if (owner->list->unk208 & 0x10)
    {
        result = 1;
    }
    else
    {
        dst[0] = dst[5];
        result = 0;
    }
    return result;
}

ASM_FUNC("asm/code_080F0C04/obj_set_bit0_208.s", void obj_set_bit0_208(void))

u32 obj_status_check_copy(void *unused0, u32 *dst, u32 *block)
{
    u32 r = (u8)build_menu_list(0, 3, 0);
    u32 match;
    u32 expect;
    if (r == 0)
    {
        r = (u8)char_menu_list(0, 0, 1);
        if (r == 0)
            r = (u8)char_menu_list(1, 0, 1);
    }
    match = 0;
    if (r == 0)
        match = 1;
    expect = *block++;
    if (match == expect)
        *dst = *block;
    return 1;
}

u32 scene_mode_command(struct OwnerObj *owner, void *unused, u32 *block)
{
    switch (*block)
    {
    case 0:
        set_scene_loaded(owner->list);
        break;
    case 1:
        set_scene_flag_20(owner->list);
        break;
    }
    return 1;
}

u32 obj_12c_command(struct OwnerObj *owner, void *unused, s32 *block)
{
    s32 value = *block;
    if (value != -0x20)
        set_obj_12c_field(owner->list, value);
    else
        set_obj_flags_12c_bit0(owner->list);
    return 0;
}

ASM_FUNC("asm/code_080F0C04/obj_fc_state_command.s", void obj_fc_state_command(void))

u32 obj_mode_check_copy(struct OwnerObj *owner, u32 *dst)
{
    u32 result;
    if ((u8)object_mode_check(owner->list) != 1)
    {
        result = 1;
    }
    else
    {
        dst[0] = dst[5];
        result = 0;
    }
    return result;
}

u32 set_object_mode_block(struct OwnerObj *owner, void *unused, u32 *block)
{
    struct ObjList *list = owner->list;
    u32 first = block[0];
    u32 mode = 1;
    u32 a;
    u32 packed;
    if (first != 0)
        mode = 2;
    a = block[1];
    packed = block[2];
    packed |= block[3] << 5;
    packed |= block[4] << 10;
    packed <<= 16;
    packed >>= 16;
    set_object_mode(list, mode, a, packed, ((u16 *)block)[10], ((u16 *)block)[12]);
    return 1;
}

u32 set_object_mode_limit(struct OwnerObj *owner, void *unused, u32 *block)
{
    u32 v = block[0];
    u32 flag2 = v & 2;
    u32 flag1;
    u32 limit = 0;
    u32 mode;
    struct ObjList *list;
    if (flag2)
        limit = 0x7FFF;
    list = owner->list;
    flag1 = v & 1;
    mode = 1;
    if (flag1)
        mode = 2;
    set_object_mode(list, mode, block[1], limit, ((u16 *)block)[4], ((u16 *)block)[6]);
    return 1;
}

ASM_FUNC("asm/code_080F0C04/obj_display_command.s", void obj_display_command(void))

ASM_FUNC("asm/code_080F0C04/obj_set_bit2_f9.s", void obj_set_bit2_f9(void))

ASM_FUNC("asm/code_080F0C04/set_flag_from_row.s", void set_flag_from_row(void))

ASM_FUNC("asm/code_080F0C04/effect_idle_copy_word.s", void effect_idle_copy_word(void))

u32 effect_flag_55f_set(void)
{
    u8 *flags = gUnk_03000FD0 + 0x55F;
    u32 v = *flags;
    v = (v & -0x61) | 0x20;
    *flags = v;
    return 1;
}

u32 call_table_two_bytes(struct OwnerObj *owner, void *unused, u8 *args)
{
    set_state_sprite_value(owner->list->table304, args[0], args[4]);
    return 1;
}

u32 call_table_byte(struct OwnerObj *owner, void *unused, u8 *args)
{
    create_state_sprite(owner->list->table304, args[0]);
    return 1;
}

u32 call_table_half_a(struct OwnerObj *owner, void *unused, u16 *args)
{
    map_anim2_start_0805BCCC(owner->list->table304, args[0]);
    return 0;
}

u32 call_table_half_b(struct OwnerObj *owner, void *unused, u16 *args)
{
    map_layer3_entry(owner->list->table304, args[0]);
    return 0;
}

ASM_FUNC("asm/code_080F0C04/find_motion_slot.s", void find_motion_slot(void))

ASM_FUNC("asm/code_080F0C04/start_move_to_target.s", void start_move_to_target(void))

ASM_FUNC("asm/code_080F0C04/write_packed_entry.s", void write_packed_entry(void))

ASM_FUNC("asm/code_080F0C04/start_move_params.s", void start_move_params(void))

ASM_FUNC("asm/code_080F0C04/reinit_object_block.s", void reinit_object_block(void))

ASM_FUNC("asm/code_080F0C04/reinit_object_block_scaled.s", void reinit_object_block_scaled(void))

ASM_FUNC("asm/code_080F0C04/store_param_block.s", void store_param_block(void))

ASM_FUNC("asm/code_080F0C04/store_scaled_pair.s", void store_scaled_pair(void))

ASM_FUNC("asm/code_080F0C04/obj_flags_command.s", void obj_flags_command(void))

u32 set_move_params_block(void *unused0, void *obj, void *unused2, s32 *block)
{
    s32 v = block[0];
    u32 a = ((v >> 1) & 1) + 1;
    u32 b = ((u16 *)block)[2];
    s32 sign = (v & 1) ? -1 : 1;
    obj_set_move_params(obj, a, b, sign, ((u16 *)block)[4]);
    return 1;
}

u32 set_motion_channels(void *unused0, void *obj, void *unused2, s32 *block)
{
    s32 v = block[0];
    if (v & 4)
        obj_set_motion_a_08046AA4(obj, ((v >> 1) & 1) + 1, v & 1, ((s16 *)block)[2], ((u16 *)block)[4]);
    v = block[0];
    if (v & 8)
        obj_set_motion_b(obj, ((v >> 1) & 1) + 1, v & 1, ((s16 *)block)[2], ((u16 *)block)[4]);
    return 1;
}

u32 obj_top_bit_check(struct OwnerObj *owner, u32 *dst, u32 *block)
{
    u32 index = *block++;
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    struct ObjEntry *obj = *(struct ObjEntry **)(base + 0x28);
    u32 bit = (~obj->unk25C) >> 31;
    u32 expect = *block++;
    if (bit == expect)
        *dst = *block;
    return 1;
}

u32 obj_top_bit_store(void *unused0, struct ObjEntry *obj, u32 *dst, u32 *block)
{
    u32 bit = (~obj->unk25C) >> 31;
    u32 expect = *block++;
    if (bit == expect)
        *dst = *block;
    return 1;
}

ASM_FUNC("asm/code_080F0C04/obj_motion_command.s", void obj_motion_command(void))

u32 probe_object_entry(struct OwnerObj *owner, void *dst, void *third, s32 *block)
{
    u32 index = block[0];
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    s32 *obj = *(s32 **)(base + 0x28);
    u32 result;
    u32 hit = contacts_obj_start_move(dst, 2, 0, (obj[3] >> 8) + block[1], (obj[4] >> 8) + block[2], (obj[5] >> 8) + block[3], 1);
    if ((u8)hit == 0)
    {
        result = 1;
    }
    else
    {
        set_flag_a0_2(owner, dst, third, 0);
        result = 0;
    }
    return result;
}

u32 place_object_track(struct OwnerObj *owner, void *obj, void *unused2, s32 *block)
{
    u32 index = block[0];
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    s32 *o2 = *(s32 **)(base + 0x28);
    obj_set_or_add_pos(obj, o2[3], o2[4], o2[5] + o2[6], 1, 0);
    {
        u32 cur = (u32)owner->list;
        if (*(u8 *)(cur + 0xF8) & 0x40)
        {
            u32 half = *(u16 *)(cur + 0xFA);
            u32 field = (half << 22) >> 27;
            if (field == *((u8 *)obj + 0x54))
                camera_track_entity((void *)cur, field, 0);
        }
    }
    return 0;
}

ASM_FUNC("asm/code_080F0C04/place_object_scaled.s", void place_object_scaled(void))

u32 probe_object_entry_plain(struct OwnerObj *owner, void *dst, void *third, s32 *block)
{
    u32 index = block[0];
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    s32 *obj = *(s32 **)(base + 0x28);
    u32 result;
    u32 hit = contacts_obj_start_move(dst, 1, 0, obj[3] >> 8, obj[4] >> 8, obj[5] >> 8, 0);
    if ((u8)hit == 0)
    {
        result = 1;
    }
    else
    {
        set_flag_a0_2(owner, dst, third, 0);
        result = 0;
    }
    return result;
}

ASM_FUNC("asm/code_080F0C04/unpack_and_set_flags.s", void unpack_and_set_flags(void))

ASM_FUNC("asm/code_080F0C04/scale_and_probe_block.s", void scale_and_probe_block(void))

u32 obj_entry_command(struct OwnerObj *owner, void *unused, u32 *block)
{
    u32 index = *block++;
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    void *obj = *(void **)(base + 0x28);
    s32 value = *block;
    switch (value)
    {
    case 0:
        obj_release_entry(obj, 1);
        break;
    case 1:
        obj_entry_release(obj);
        break;
    case 2:
        obj_entry_activate(obj);
        break;
    }
    return 1;
}

u32 obj_request_command(struct OwnerObj *owner, void *unused, u32 *block)
{
    u32 index = *block++;
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    obj_request_entry(*(void **)(base + 0x28), *(u16 *)block);
    return 1;
}

u32 obj_entry_mode(void *unused0, void *obj, void *unused2, s32 *mode)
{
    s32 value = *mode;
    switch (value)
    {
    case 0:
        obj_release_entry(obj, 1);
        break;
    case 1:
        obj_entry_release(obj);
        break;
    case 2:
        obj_entry_activate(obj);
        break;
    }
    return 1;
}

u32 obj_request_halfword(void *unused0, void *obj, void *unused2, u16 *block)
{
    obj_request_entry(obj, *block);
    return 1;
}

ASM_FUNC("asm/code_080F0C04/obj_state_entry.s", void obj_state_entry(void))

ASM_FUNC("asm/code_080F0C04/obj_state_direct.s", void obj_state_direct(void))

ASM_FUNC("asm/code_080F0C04/obj_reset_command.s", void obj_reset_command(void))

ASM_FUNC("asm/code_080F0C04/obj_state_entry_plain.s", void obj_state_entry_plain(void))

ASM_FUNC("asm/code_080F0C04/obj_routine_entry.s", void obj_routine_entry(void))

ASM_FUNC("asm/code_080F0C04/cmd_place_list_entry.s", void cmd_place_list_entry(void))

u32 obj_set_state_entry(struct OwnerObj *owner, void *unused, u32 *block)
{
    u32 index = *block++;
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    obj_set_state(*(void **)(base + 0x28), *(u8 *)block);
    return 1;
}

u32 cmd_apply_list_entry(struct OwnerObj *owner, void *unused, u32 *cmd)
{
    u32 index = cmd[0];
    u32 list = (u32)owner->list;
    u32 base = index * 4 + list;
    obj_apply_state_a(*(void **)(base + 0x28));
    return 1;
}

ASM_FUNC("asm/code_080F0C04/obj_sub_pair_command.s", void obj_sub_pair_command(void))

ASM_FUNC("asm/code_080F0C04/obj_state_read_command.s", void obj_state_read_command(void))

ASM_FUNC("asm/code_080F0C04/cmd_set_obj_state.s", void cmd_set_obj_state(void))

ASM_FUNC("asm/code_080F0C04/cmd_place_object_toggle.s", void cmd_place_object_toggle(void))

ASM_FUNC("asm/code_080F0C04/cmd_set_obj_state_b.s", void cmd_set_obj_state_b(void))

u32 cmd_commit_or_activate(void *unused0, void *obj, void *unused2, u32 *kind)
{
    switch (*kind)
    {
    case 0:
        obj_commit_sprite(obj);
        break;
    case 1:
        obj_activate_sprite(obj);
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F0C04/cmd_toggle_flag.s", void cmd_toggle_flag(void))

ASM_FUNC("asm/code_080F0C04/cmd_set_sprite_slot.s", void cmd_set_sprite_slot(void))

ASM_FUNC("asm/code_080F0C04/cmd_set_obj_bit.s", void cmd_set_obj_bit(void))

u32 copy_word(void *unused, u32 *dst, u32 *src)
{
    *dst = *src;
    return 1;
}

u32 cmd_copy_if_equal(void *unused0, u32 *dst, u32 *block)
{
    u32 cur = *(u16 *)((u8 *)gUnk_03000FD0 + 0x54C);
    u32 expect = *block++;
    if (cur == expect)
        *dst = *block;
    return 1;
}

u32 cmd_set_var_from_flag(void *unused0, u8 *dst, s32 *block, u32 value)
{
    s32 kind = block[1];
    u8 *p;
    u8 *vars;
    u8 *target;
    switch (kind)
    {
    case 0:
        p = gUnk_03000FD0;
        break;
    case 1:
        p = gUnk_03000FD0 + 0x1F8;
        break;
    case 2:
        p = gUnk_03000FD0 + 0xA8;
        break;
    case 3:
        p = gUnk_03000FD0 + 0x150;
        break;
    }
    target = dst + 0x18;
    vars = gUnk_03000FC0 + 0x38C;
    set_var(value, target, vars, 0, ((s16 *)block)[0], *(u16 *)(p + 0xA0) & 1);
    return 1;
}

u32 cmd_copy_if_flag(struct OwnerObj *owner, u32 *dst, s32 *block)
{
    u8 *base = (u8 *)owner;
    s32 kind = *block++;
    switch (kind)
    {
    case 0:
        base = gUnk_03000FD0;
        break;
    case 1:
        base = gUnk_03000FD0 + 0x1F8;
        break;
    case 2:
        base = gUnk_03000FD0 + 0xA8;
        break;
    case 3:
        base = gUnk_03000FD0 + 0x150;
        break;
    }
    if (*(u16 *)(base + 0xA0) & 1)
        *dst = *block;
    return 1;
}
