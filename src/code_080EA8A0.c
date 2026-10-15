#include "global.h"

struct ScriptObj
{
    u32 word0;
    u32 word4;
    u32 word8;
    u32 wordC;
    u32 word10;
    u8 filler14[0x58 - 0x14];
    u32 entries[4];
    u8 filler68[0x98 - 0x68];
    u32 word98;
    u32 word9C;
    u16 halfA0;
    u16 halfA2;
    u8 byteA4;
    u8 byteA5;
    u8 countA6;
};

struct IdleRequest
{
    u32 flag;
    u16 a;
    u8 filler6[2];
    u16 b;
    u8 fillerA[2];
    u32 c;
};

struct RampRequest
{
    u32 flag;
    u8 byte4;
    u8 filler5[3];
    u8 byte8;
};

struct CmpRecord
{
    u8 op;
    u8 filler1[3];
    u32 a;
    u32 value;
};

struct CmpMatchRecord
{
    u8 op;
    u8 filler1[3];
    u32 a;
    u32 b;
    u32 expect;
    u32 value;
};

struct BranchState
{
    u32 word0;
    u8 filler4[0x10];
    u32 word14;
};

struct ListEntry
{
    u8 filler0[0xC];
    u8 id;
    u8 filler1[2];
    u8 active;
};

struct KeyGroupRecord
{
    s16 index;
    u8 filler2[2];
    u16 group;
};

extern u8 *gUnk_03000FB8;
void set_link_slot_mask(u32 mask);
void serial_schedule(u32 a, u32 b, u32 c);
void set_ramp0_target(u32 a, u32 b);
extern u8 gUnk_0300034C[];
extern u8 *gUnk_03000D44;
u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 value);
u32 compare_op(void *ctx, u32 op, u32 a, u32 b);
void set_var(u32 a, void *b, void *c, u32 d, s32 e, u32 f);
void poll_keys(void);
void slot_obj_clear(struct ListEntry **list, u8 index);

ASM_FUNC("asm/code_080EA8A0/set_obj_flag.s", void set_obj_flag(void))

ASM_FUNC("asm/code_080EA8A0/cmd_update_and_store.s", void cmd_update_and_store(void))

ASM_FUNC("asm/code_080EA8A0/cmd_read_store.s", void cmd_read_store(void))

ASM_FUNC("asm/code_080EA8A0/counter_step_and_branch.s", void counter_step_and_branch(void))

void set_obj_params_a(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    obj->word8 = a;
    obj->wordC = b;
    obj->halfA2 = s5;
    obj->byteA5 = s6;
}

void set_obj_params_b(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    obj->word0 = a;
    obj->word4 = b;
    obj->word10 = 0;
    obj->halfA0 = s5;
    obj->byteA4 = s6;
}

void clear_obj_fields(void *ctx, struct ScriptObj *obj, struct ScriptObj *flags)
{
    if ((obj->halfA0 & 8) != 0)
    {
        flags->halfA0 &= 0xFFFB;
    }
    obj->wordC = 0;
    obj->word8 = 0;
    obj->byteA5 = 0;
    obj->halfA2 = 0;
}

void reset_obj_params_a(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    clear_obj_fields(ctx, obj, (struct ScriptObj *)a);
    set_obj_params_a(ctx, obj, a, b, s5, s6);
}

void reset_obj_params_b(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    clear_obj_fields(ctx, obj, (struct ScriptObj *)a);
    set_obj_params_b(ctx, obj, a, b, s5, s6);
}

ASM_FUNC("asm/code_080EA8A0/flags_vars_set_obj_state.s", void flags_vars_set_obj_state(void))

ASM_FUNC("asm/code_080EA8A0/direction_octant.s", void direction_octant(void))

ASM_FUNC("asm/code_080EA8A0/unpack_stream_values.s", void unpack_stream_values(void))

void tick_obj_timer(void *ctx, struct ScriptObj *obj)
{
    s32 t = obj->word9C - 1;

    obj->word9C = t;
    if (t == 0)
    {
        obj->halfA0 &= 0xFFEF;
    }
}

ASM_FUNC("asm/code_080EA8A0/compare_op.s", u32 compare_op(void *ctx, u32 op, u32 a, u32 b))

ASM_FUNC("asm/code_080EA8A0/cmd_set_var_lang.s", void cmd_set_var_lang(void))

u32 request_if_idle(void *ctx, void *unused, u32 *rec)
{
    u32 flag = *rec++;

    if (flag == 0)
    {
        serial_schedule(((u16 *)rec)[0], ((u16 *)rec)[2], rec[2]);
    }
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/dispatch_obj_mode_call.s", void dispatch_obj_mode_call(void))

u32 set_obj_if_cmp(void *ctx, struct ScriptObj *obj, struct CmpRecord *rec)
{
    if ((u8)compare_op(ctx, rec->op, rec->a, obj->word98))
    {
        obj->word0 = rec->value;
    }
    return 1;
}

u32 pop_obj_entry(void *ctx, struct ScriptObj *obj)
{
    u8 *count = &obj->countA6;
    u32 idx;

    *count = *count - 1;
    idx = obj->countA6;
    *(u32 *)((idx << 2) + (u32)obj + 0x58) = 0;
    return 1;
}

u32 set_obj_word_98(void *ctx, struct ScriptObj *obj, u32 *src)
{
    obj->word98 = *src;
    return 1;
}

u32 push_obj_entry(void *ctx, struct ScriptObj *obj, u32 *src)
{
    *(u32 *)((u8 *)obj + 0x58 + (obj->countA6 << 2)) = *src;
    obj->countA6++;
    return 1;
}

u32 pick_random_entry(void *ctx, u32 *out, u32 *table)
{
    s32 key = table[0];
    u32 rnd = mt19937_next();
    s32 index = sub_081DE850(key, rnd);
    u32 entry = table[index + 1];

    if (entry != 0)
    {
        *out = entry;
    }
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/random_obj_placement.s", void random_obj_placement(void))

u32 call_b0c0(void *ctx, void *unused, u16 *rec)
{
    set_link_slot_mask(*rec);
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/copy_on_key_match_a.s", void copy_on_key_match_a(void))

ASM_FUNC("asm/code_080EA8A0/copy_on_key_match_b.s", void copy_on_key_match_b(void))

ASM_FUNC("asm/code_080EA8A0/set_var_from_keys.s", u32 set_var_from_keys(void *ctx, struct ScriptObj *obj, s16 *src, u32 mid))

ASM_FUNC("asm/code_080EA8A0/set_var_from_key_group.s", u32 set_var_from_key_group(void *ctx, struct ScriptObj *obj, struct KeyGroupRecord *src, u32 mid))

ASM_FUNC("asm/code_080EA8A0/set_key_ignore_mask.s", u32 set_key_ignore_mask(void *ctx, void *unused, u32 *src))

ASM_FUNC("asm/code_080EA8A0/set_key_mode_bits.s", u32 set_key_mode_bits(void *ctx, void *unused, u8 *src))

ASM_FUNC("asm/code_080EA8A0/set_shadow_bits.s", u32 set_shadow_bits(void *ctx, void *unused, u32 *src))

u32 copy_if_cmp_match(void *ctx, u32 *out, struct CmpMatchRecord *rec)
{
    u8 res = compare_op(ctx, rec->op, rec->a, rec->b);

    if (res == rec->expect)
    {
        *out = rec->value;
    }
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/binary_op_and_set_var.s", void binary_op_and_set_var(void))

ASM_FUNC("asm/code_080EA8A0/cmd_store_obj_field.s", void cmd_store_obj_field(void))

ASM_FUNC("asm/code_080EA8A0/store_obj_byte_to_var.s", void store_obj_byte_to_var(void))

ASM_FUNC("asm/code_080EA8A0/set_cur_obj_26_27.s", void set_cur_obj_26_27(void))

u32 set_cur_obj_29(void *ctx, void *unused, u32 *args)
{
    u8 *cur;
    u8 value;

    if (*args == 0)
    {
        cur = gUnk_03000D44 + 0x29;
        value = 1;
    }
    else
    {
        cur = gUnk_03000D44 + 0x29;
        value = 0xFF;
    }
    *cur = value;
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/cur_obj_bbf8.s", void cur_obj_bbf8(void))

u32 stub_return_1_080EB624(void)
{
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/branch_if_list_entry.s", u32 branch_if_list_entry(void *ctx, struct ListEntry **list, struct BranchState *state, u32 *id))

u32 stub_return_1_b(void)
{
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/clear_list_entry.s", u32 clear_list_entry(void *ctx, struct ListEntry **list, u32 *unused, u32 *id))

u32 set_var_from_text_byte(void *ctx, struct ScriptObj *obj, s16 *src, u32 mid)
{
    set_var(mid, (u8 *)obj + 0x18, 0, 0, *src, gUnk_03000FB8[0x30]);
    return 1;
}

u32 store_if_text_byte(void *ctx, u32 *dst, u32 *args)
{
    if (gUnk_03000FB8[0x30] == args[0])
    {
        *dst = args[1];
    }
    return 1;
}

u32 copy_if_slot_bit(void *ctx, struct ScriptObj *obj, u32 *dst, u32 *args)
{
    u8 *state = (u8 *)obj->word4;
    s32 bits = state[0x1B2];

    if (((bits >> *args++) & 1) != 0)
    {
        *dst = *args;
    }
    return 1;
}

u32 set_ramp0_if_zero(void *ctx, void *unused, u32 *rec)
{
    u32 flag = *rec++;

    if (flag == 0)
    {
        set_ramp0_target(((u8 *)rec)[4], ((u8 *)rec)[0]);
    }
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/slot_value_cmd.s", void slot_value_cmd(void))

ASM_FUNC("asm/code_080EA8A0/set_slot_param.s", void set_slot_param(void))

ASM_FUNC("asm/code_080EA8A0/slot_target_cmd.s", void slot_target_cmd(void))

u32 set_text_window_32(void *ctx, void *unused, u32 *src)
{
    u8 *p = gUnk_03000FB8;
    u32 v = *src;

    p[0x32] = v;
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/bump_or_branch_record.s", void bump_or_branch_record(void))

ASM_FUNC("asm/code_080EA8A0/map_rects_cmd.s", void map_rects_cmd(void))

u32 branch_if_state_bit(void *ctx, struct ScriptObj *obj, struct BranchState *state, u32 *args)
{
    u8 *block = (u8 *)obj->word4;
    s32 bits = block[0x1B4];

    if (((bits >> *args) & 1) != 0)
    {
        state->word0 = state->word14;
        return 0;
    }
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/draw_text_item_raw.s", void draw_text_item_raw(void))

ASM_FUNC("asm/code_080EA8A0/unknown_thumb_080eb9e0.s", void unknown_thumb_080eb9e0(void))

u32 set_obj_word9c_flag(void *ctx, struct ScriptObj *obj, u32 *src)
{
    u32 result;

    obj->word9C = *src;
    if (obj->word9C != 0)
    {
        obj->halfA0 |= 0x10;
        result = 0;
    }
    else
    {
        result = 1;
    }
    return result;
}

u32 set_obj_word0_saved(void *ctx, struct ScriptObj *obj, u32 *rec)
{
    if (rec[0] == 1)
    {
        obj->word10 = obj->word0;
    }
    obj->word0 = rec[1];
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/restore_obj_word0.s", u32 restore_obj_word0(void *ctx, struct ScriptObj *obj))

u32 reset_cursor(void *ctx, struct ScriptObj *obj)
{
    obj->word4 = obj->word0;
    obj->word10 = 0;
    return 1;
}

ASM_FUNC("asm/code_080EA8A0/update_owner_objects.s", void update_owner_objects(void))

ASM_FUNC("asm/code_080EA8A0/update_obj_sub_state.s", void update_obj_sub_state(void))

ASM_FUNC("asm/code_080EA8A0/set_obj_mode.s", void set_obj_mode(void))

ASM_FUNC("asm/code_080EA8A0/step_effect_slot.s", void step_effect_slot(void))
