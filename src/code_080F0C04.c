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

struct Bit0Only
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct Bit2Field
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 rest : 5;
};

struct SpriteHdr
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
};

struct Bits1to4
{
    u8 b0 : 1;
    u8 f : 4;
    u8 rest : 3;
};

struct SpriteBits12
{
    u8 bit0 : 1;
    u8 field1 : 2;
    u8 rest : 5;
};

struct Bit6Byte
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 rest : 1;
};

void obj_set_state(void *obj, u32 state);
void obj_sync_sub_slot(void *obj, u32 value);
void obj_set_cell_coord(void *ent, u32 cell, u32 arg);

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

struct TaskNode
{
    u8 filler0[0x18];
    void *desc;
};

struct TaskDesc
{
    u32 unk0;
    u32 unk4;
    u16 offset8;
    u16 unkA;
    void *callback;
};

extern const struct TaskDesc gTaskDescA;
extern const struct TaskDesc gDefaultTaskDesc;
extern void *gUnk_03000FD4;

void task_remove(void *node, u32 mode);
void task_insert(void *node, u32 kind, u32 data);
void heap_free_b(void *p);

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
void shift_sub_pair_frames(struct ObjList *list);
void reset_sub_pair(struct ObjList *list);
void clear_sub_pair(struct ObjList *list);
void probe_sub_pair_target(struct ObjList *list, u32 level);
void apply_test_level(struct ObjList *list);
void advance_sub_a_state(struct ObjList *list);
void set_sub_pair_state_b2(struct ObjList *list);
void pair_set_mode(struct ObjList *list, u32 mode, u32 flag);
u32 direction_octant(void *root, s32 x0, s32 y0, s32 x1, s32 y1);
void set_half_23e(void *obj, s16 v);
void unpack_stream_values(void *src, s32 *block, s32 *a, s32 *b, s32 *c);
void entity_camera_offset(void *list, u32 mode, s32 *x, s32 *y);
void sprites_obj_set_move_target(void *list, u32 type, u32 one, s32 x, s32 y, s32 z);
void entity_camera_offset(void *list, u32 mode, s32 *x, s32 *y);
void object_setup_wipe(void *list, u32 mode, u32 a, s32 x, s32 y);

typedef u32 (*KindHandler)(struct ObjList *list, void *slot);
extern const u32 gKindHandlerTable[];

extern u8 *gEffectState;
extern u8 *gUnk_03000FC0;
extern u8 gUnk_03000DD0[];
extern const u8 gLayer3Table[];

void obj_clear_flags_2b5_30(void *obj);
void obj_clear_flags_2b5_c0(void *obj);
void obj_clear_flags_2b6_30(void *obj);
u8 obj_flags_2b5_30_set(void *obj);
u8 obj_flags_2b5_c0_set(void *obj);
u8 obj_flags_2b6_30_set(void *obj);

u8 flags_vars_set_obj_state(void *root, void *dst, void *obj, u32 state, u32 zero);

typedef void (*ObjMethod)(void *self, u32 arg);

struct MethodEntry
{
    s16 offset;
    u16 unk2;
    ObjMethod fn;
};

struct SubObj
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x21 - 0x13];
    u8 unk21;
};

struct Entity
{
    u8 filler0[0x214];
    u8 unk214;
    u8 unk215;
    u8 filler216[0x294 - 0x216];
    struct SubObj *sub294;
    u8 filler298[0x338 - 0x298];
    u8 *unk338;
};

struct ListEntryArgs
{
    u32 index;
    u8 unk4;
    u8 filler5[3];
    u32 unk8;
    u8 unkC;
    u8 filler0D[3];
    u32 unk10;
};

void step_status_1f8(struct OwnerObj **self)
{
    u8 *state = gEffectState;
    u8 *timer = state + 0x1f8;
    u16 *flags = (u16 *)(timer + 0xa0);
    if ((*flags & 0x100) && (u8)obj_has_pending_bits((*self)->list) == 0)
        *flags ^= 0x100;
    if (*(u16 *)(timer + 0xa0) & 0x10)
        tick_obj_timer((u8 *)*self + 0x1c, timer);
}

void task_slot_release(u8 *slot, u32 flags)
{
    struct TaskNode *node = *(struct TaskNode **)slot;
    node->desc = (void *)&gTaskDescA;
    if (flags == 0)
    {
        struct TaskDesc copy = gTaskDescA;
        u16 *off;
        u8 *base;
        node->desc = &copy;
        off = (u16 *)&copy;
        base = (u8 *)node - 4;
        off[4] = (u16)(slot - base + gTaskDescA.offset8);
    }
    if (flags & 2)
    {
        (*(struct TaskNode **)slot)->desc = (void *)&gDefaultTaskDesc;
        task_remove(*(struct TaskNode **)slot, 0);
    }
    if (flags & 1)
        heap_free_b(slot);
}

void *task_slot_attach(u8 *slot, u32 attach, u32 kind, u32 data)
{
    u8 kind_byte = kind;
    if (attach != 0)
    {
        struct TaskNode *node = (struct TaskNode *)(slot + 4);
        *(struct TaskNode **)slot = node;
        task_insert(node, kind_byte, data);
        node->desc = (void *)&gDefaultTaskDesc;
    }
    (*(struct TaskNode **)slot)->desc = (void *)&gTaskDescA;
    if (attach == 0)
    {
        struct TaskDesc copy = gTaskDescA;
        u16 *off;
        u8 *base;
        (*(struct TaskNode **)slot)->desc = &copy;
        off = (u16 *)&copy;
        base = (u8 *)*(struct TaskNode **)slot - 4;
        off[4] = (u16)(slot - base + gTaskDescA.offset8);
    }
    gUnk_03000FD4 = slot;
    return slot;
}

void set_flag_a0_2(void *a, void *b, u8 *p, u32 c)
{
    struct { u8 pad[0xA0]; u16 flags; } *obj = (void *)p;
    obj->flags |= 2;
}

u32 dispatch_kind_handler(struct OwnerObj *owner, u32 value, u32 *dst, u32 *block)
{
    u32 *slot = (u32 *)(gEffectState + 0x480);
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
        u16 *flags = (u16 *)((u8 *)gEffectState + 0x54A);
        u32 mask = 1 << *block;
        *flags &= ~mask;
    }
    return 1;
}

ASM_FUNC("asm/code_080F0C04/setup_effect_record.s", void setup_effect_record(void))

u32 set_var_from_halfwords(struct OwnerObj *owner, u8 *dst, s16 *args, u32 value)
{
    u8 *target = dst + 0x18;
    u8 *vars = gUnk_03000FC0 + 0x38C;
    set_var(value, target, vars, 0, args[0], *(u16 *)((u8 *)owner->list + 0x11E));
    return 1;
}

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

struct ObjPos
{
    u8 filler0[0xC];
    s32 x;
    s32 y;
};

struct VarDirCmd
{
    s16 var;
    u16 unk2;
    u32 flag;
    u32 from;
    u32 to;
};

u32 cmd_set_var_direction(struct OwnerObj *owner, u8 *dst, struct VarDirCmd *cmd, u32 var_mid)
{
    struct ObjPos *from = (struct ObjPos *)owner->list->entries[cmd->from];
    struct ObjPos *to = (struct ObjPos *)owner->list->entries[cmd->to];
    u32 dir;
    u8 *target;
    u8 *vars;
    if (cmd->flag == 0)
    {
        dir = (u8)direction_octant((u8 *)owner + 0x1C, from->x, from->y, to->x, to->y);
    }
    target = dst + 0x18;
    vars = gUnk_03000FC0 + 0x38C;
    set_var(var_mid, target, vars, 0, cmd->var, dir);
    return 1;
}

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

u32 obj_list_command(struct OwnerObj *owner, void *unused, u32 *cmd)
{
    u32 mode = *cmd;
    switch (mode)
    {
    case 5:
        shift_sub_pair_frames(owner->list);
        break;
    case 6:
        reset_sub_pair(owner->list);
        break;
    case 7:
        clear_sub_pair(owner->list);
        break;
    case 8:
        probe_sub_pair_target(owner->list, 1);
        break;
    case 9:
        apply_test_level(owner->list);
        break;
    case 10:
        advance_sub_a_state(owner->list);
        break;
    case 11:
        set_sub_pair_state_b2(owner->list);
        break;
    default:
        pair_set_mode(owner->list, mode, 0);
        break;
    }
    return 1;
}

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

u32 obj_set_bit0_208(struct OwnerObj *owner, u32 unused, u32 *flag)
{
    struct ObjList *list = owner->list;
    u32 v = *flag;
    ((struct Bit0Only *)&list->unk208)->b0 = v;
    return 1;
}

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

u32 obj_set_bit2_f9(struct OwnerObj *owner, u32 unused, u32 *flags)
{
    if ((((s32)*flags >> 1) & 1) == 0)
    {
        ((struct Bit2Field *)((u8 *)owner->list + 0xF9))->b2 = *flags & 1;
    }
    return 1;
}

u32 set_flag_from_row(void *unused0, void *unused1, s32 *block)
{
    const u8 *table = gLayer3Table;
    u8 *entry;
    u8 *row;
    u32 index;
    u32 h;
    u32 value;
    entry = (u8 *)table + (gUnk_03000DD0[0xA] << 3);
    index = block[0];
    row = *(u8 **)entry;
    h = *(u16 *)(row + index * 20 + 0x10);
    h = h << 17;
    h = h >> 1;
    value = h + (0xE4 << 19);
    value = value >> 16;
    set_flag(0, 0, value, *((u8 *)block + 4));
    return 1;
}

u32 effect_idle_copy_word(void *unused, u32 *dst)
{
    if ((*(u16 *)((u8 *)gEffectState + 0x55E) & 0x603F) != 0)
    {
        dst[0] = dst[5];
        return 0;
    }
    return 1;
}

u32 effect_flag_55f_set(void)
{
    u8 *flags = gEffectState + 0x55F;
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

void sprites_obj_set_move_target(void *list, u32 type, u32 one, s32 x, s32 y, s32 z);
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

u32 place_object_scaled(struct OwnerObj *owner, u8 *obj, void *unused2, s32 *block)
{
    s32 flags = block[0];
    s32 match;
    if (flags != 0)
    {
        s32 x = block[1] << 4;
        block[1] = x;
        block[2] <<= 4;
        block[3] <<= 4;
        if ((flags & 0xF) == 0)
        {
            block[1] = x + 8;
            block[2] = (block[2] >> 4 << 4) + 8;
        }
    }
    obj_set_or_add_pos(obj, block[1] << 8, block[2] << 8, block[3] << 8, block[5], block[6]);
    match = 0;
    if (*((u8 *)owner->list + 0xF8) & 0x40)
    {
        u32 field = ((u32)*(u16 *)((u8 *)owner->list + 0xFA) << 22) >> 27;
        if (field == *(obj + 0x54))
            match = 1;
    }
    if (match & block[5])
        camera_track_entity(owner->list, *(obj + 0x54), 0);
    return 0;
}

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

u32 unpack_and_set_flags(struct OwnerObj *owner, void *obj, void *third, s32 *block)
{
    s32 v0;
    s32 v1;
    s32 v2;
    s32 flags;
    unpack_stream_values((u8 *)owner + 0x1C, block, &v0, &v1, &v2);
    block += 3;
    flags = *block++;
    if ((flags & 3) == 1)
    {
        s32 x = *block;
        *block = (x & 0x40) ? ((x & 0xF) << 4) : (x << 8);
        if (*block != 0)
            set_half_23e(obj, (s16)*block);
    }
    contacts_obj_start_move(obj, flags & 3, 1, v0, v1, v2, ((u16 *)block)[0]);
    set_flag_a0_2(owner, obj, third, 0);
    return 0;
}

u32 scale_and_probe_block(struct OwnerObj *owner, void *dst, void *third, s32 *block)
{
    u32 hit;
    u32 result;
    if (block[0] != 0)
    {
        block[2] <<= 4;
        block[3] <<= 4;
        block[4] <<= 4;
    }
    if (((block[1] >> 1) & 3) == 1)
    {
        s32 x = block[5];
        block[5] = (x & 0x40) ? ((x & 0xF) << 4) : (x << 8);
        if (block[5] != 0)
            set_half_23e(dst, (s16)block[5]);
    }
    hit = contacts_obj_start_move(dst, (block[1] >> 1) & 3, block[1] & 1, block[2], block[3], block[4], ((u16 *)block)[10]);
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

NONMATCH("asm/code_080F0C04/obj_reset_command.s", u32 obj_reset_command(struct OwnerObj *owner, u32 unused, u32 *block))
{
    u8 *obj;
    struct SpriteHdr *sub;

    obj = *(u8 **)((u8 *)owner->list + (block[0] << 2) + 0x28);
    obj_set_state(obj, 0);
    obj_sync_sub_slot(obj, 0);
    obj_set_cell_coord(obj, block[1], block[2]);
    obj[0x214] &= 0x7F;
    sub = *(struct SpriteHdr **)(obj + 0x294);
    ((struct SpriteBits12 *)&sub->unk12)->field1 = 0;
    sub = *(struct SpriteHdr **)(obj + 0x294);
    ((struct Bit6Byte *)&sub->unk11)->bit6 = block[3] ^ (((u32)sub->unk11 << 25) >> 31);
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080F0C04/obj_state_entry_plain.s", void obj_state_entry_plain(void))

u8 flags_vars_set_obj_state(void *root, void *dst, void *obj, u32 state, u32 zero);
ASM_FUNC("asm/code_080F0C04/obj_routine_entry.s", void obj_routine_entry(void))

u32 cmd_place_list_entry(struct OwnerObj *owner, void *unused, struct ListEntryArgs *args)
{
    struct Entity *obj;
    u32 index = args->index;
    struct ObjList *list = owner->list;
    obj = *(struct Entity **)((u8 *)list + 0x28 + (index << 2));
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, 0);
    if (args->unk10 != 0)
    {
        struct MethodEntry *m = (struct MethodEntry *)(obj->unk338 + 0x98);
        m->fn((u8 *)obj + m->offset, args->unk4);
    }
    else
    {
        struct MethodEntry *m = (struct MethodEntry *)(obj->unk338 + 0x90);
        m->fn((u8 *)obj + m->offset, args->unk4);
    }
    obj_set_cell_coord(obj, args->unk8, 0);
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, (u8 *)obj + 0x58, obj->sub294, (u8)args->unk10, (u8)args->unkC);
}

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

u32 cmd_set_obj_state_b(u8 *owner, u8 *obj, u32 *dst, u32 *args)
{
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, args[3]);
    if (args[3] != 0)
    {
        u8 *m = *(u8 **)(obj + 0x338) + 0x98;

        ((void (*)(void *, u32))*(void **)(m + 4))(obj + *(s16 *)m, ((u8 *)args)[0]);
    }
    else
    {
        u8 *m = *(u8 **)(obj + 0x338) + 0x90;

        ((void (*)(void *, u32))*(void **)(m + 4))(obj + *(s16 *)m, ((u8 *)args)[0]);
    }
    obj_set_cell_coord(obj, args[1], 0);
    return (u8)flags_vars_set_obj_state(owner + 0x1C, dst, *(void **)(obj + 0x294), ((u8 *)args)[0xC], ((u8 *)args)[8]);
}

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

void entity_camera_offset(void *list, u32 mode, s32 *x, s32 *y);
void object_setup_wipe(void *list, u32 mode, u32 a, s32 x, s32 y);
ASM_FUNC("asm/code_080F0C04/cmd_set_sprite_slot.s", void cmd_set_sprite_slot(void))

void obj_clear_flags_2b5_30(void *obj);
void obj_clear_flags_2b5_c0(void *obj);
void obj_clear_flags_2b6_30(void *obj);
u8 obj_flags_2b5_30_set(void *obj);
u8 obj_flags_2b5_c0_set(void *obj);
u8 obj_flags_2b6_30_set(void *obj);
ASM_FUNC("asm/code_080F0C04/cmd_set_obj_bit.s", void cmd_set_obj_bit(void))

u32 copy_word(void *unused, u32 *dst, u32 *src)
{
    *dst = *src;
    return 1;
}

u32 cmd_copy_if_equal(void *unused0, u32 *dst, u32 *block)
{
    u32 cur = *(u16 *)((u8 *)gEffectState + 0x54C);
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
        p = gEffectState;
        break;
    case 1:
        p = gEffectState + 0x1F8;
        break;
    case 2:
        p = gEffectState + 0xA8;
        break;
    case 3:
        p = gEffectState + 0x150;
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
        base = gEffectState;
        break;
    case 1:
        base = gEffectState + 0x1F8;
        break;
    case 2:
        base = gEffectState + 0xA8;
        break;
    case 3:
        base = gEffectState + 0x150;
        break;
    }
    if (*(u16 *)(base + 0xA0) & 1)
        *dst = *block;
    return 1;
}
