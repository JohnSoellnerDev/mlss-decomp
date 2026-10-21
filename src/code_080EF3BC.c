#include "global.h"

extern u8 *gEffectState;
extern u8 *gUnk_03000FC0;
extern u8 *gUnk_03000FC8;
extern u8 *gUnk_03000FCC;
extern u8 *gCurObj;
extern const u8 gEffectPoolTag[];
extern u8 gTaskTemplate[];
extern u8 gClaimKindTable[];
extern const u8 gDefaultTaskDesc[];

struct TaskCopy
{
    u32 words[4];
};

struct EffectPoolBits
{
    u8 lo:2;
    u8 mode:4;
    u8 hi:2;
};

struct EffectSlotBits
{
    u8 flag:1;
    u8 rest:7;
};

struct EffectStateMask
{
    u8 filler0[0x54A];
    u16 slotMask;
};

void clear_slot_rect(u8 *rect, u32 n);
void copy_map_rects(u8 *rects, u32 n);
void heap_free(u8 *p);
void release_obj_parts(u8 *p, u32 n);
void free_ptr_pair(u8 *p, u32 n);
void reset_obj_params_b(u8 *obj, u8 *state, u32 arg, u32 zero, u32 one, u32 ff);
void obj_finish_move_step(void *obj);
void obj_stop_motion(void *obj);
void obj_apply_state_a(void *obj);
void set_var(u8 *vars, void *dst, u8 *buf, u32 zero, s32 e, u32 f);
void register_effect_slot(u32 first, u8 *slot, u32 *args);
void text_window_close(void);
void entity_camera_offset(u8 *list, u32 index, s32 *x, s32 *y);
u32 claim_table_slot(u8 *table, u32 a, u32 b, u32 c, s32 d, s32 e, u32 f);
void update_slot_bounds(u8 *pool, u32 slot);
void heap_free_b(void *p);
void task_remove(void *node, u32 kind);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *init_slot_table(void *state);

u32 claim_effect_slot(u8 *owner, u32 unused, u32 *cmds)
{
    s32 x;
    s32 y;
    u32 index = cmds[0];
    u8 *list = *(u8 **)(owner + 0x14);
    u8 *obj = *(u8 **)(list + index * 4 + 0x28);
    u32 slot;

    y = 0;
    x = 0;
    if (cmds[2] != 0)
    {
        entity_camera_offset(list, index, &x, &y);
        y = y - obj[0x283];
    }
    if (cmds[5] == 0xFF)
    {
        cmds[5] = gClaimKindTable[cmds[1]];
    }
    slot = (u8)claim_table_slot(gUnk_03000FC8, (u8)cmds[0], (u8)cmds[1], (u8)cmds[2],
                                *(s16 *)((u8 *)cmds + 0xC), *(s16 *)((u8 *)cmds + 0x10), (u8)cmds[5]);
    if (slot != 0x3F)
    {
        u8 *pool = gUnk_03000FC8;
        u8 *entry = *(u8 **)pool + (slot << 4);

        *(s16 *)(entry + 8) = x;
        *(s16 *)(entry + 0xA) = y;
        update_slot_bounds(pool, slot);
    }
    return 1;
}

struct AnchorBits4 { u8 lo4 : 4; u8 b4 : 1; u8 top : 3; };
struct AnchorBits3 { u8 lo3 : 3; u8 b3 : 1; u8 top : 4; };
void set_sub_pair_mode(u8 *target, u32 a, u32 b, u32 c);
void follow_anchor(u8 *target, u32 arg);

u32 apply_anchor_command(u8 *obj, u8 *dst, u32 *cmds)
{
    u8 *list = *(u8 **)(obj + 0x14);
    u32 mode;

    if ((u8)(*(u8 *)(list + 0x12A) - 3) <= 2)
    {
        *(u32 *)dst = *(u32 *)(dst + 0x14);
        return 0;
    }
    mode = cmds[0];
    if (mode > 7)
    {
        return 1;
    }
    switch (mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        set_sub_pair_mode(*(u8 **)(obj + 0x14), mode & 1, ((s32)mode >> 1) & 1, 0);
        break;
    case 4:
        follow_anchor(*(u8 **)(obj + 0x14), 0);
        break;
    case 5:
        ((struct AnchorBits4 *)(*(u8 **)(obj + 0x14) + 0x208))->b4 = 0;
        /* fall through */
    case 6:
    {
        u8 *target = *(u8 **)(obj + 0x14);
        u32 bit = cmds[0] - 5;
        ((struct AnchorBits3 *)(target + 0x208))->b3 = bit & 1;
        break;
    }
    case 7:
        follow_anchor(*(u8 **)(obj + 0x14), 1);
        break;
    }
    return 1;
}

struct Bit1Bits { u8 lo1 : 1; u8 b1 : 1; u8 rest : 6; };
u32 pair_slot_kind(u8 *list);
u32 sub_0803C454(u8 *list);
u32 sub_0803C4EC(u8 *list);

u32 check_object_condition(u8 *obj, u32 *dst, u32 *cmds)
{
    u32 op = *cmds++;

    if (op > 0xB)
    {
        return 1;
    }
    switch (op)
    {
    case 0:
        if (pair_slot_kind(*(u8 **)(obj + 0x14)) != 0)
        {
            *dst = *cmds;
        }
        break;
    case 1:
        if (pair_slot_kind(*(u8 **)(obj + 0x14)) > 1)
        {
            *dst = *cmds;
        }
        break;
    case 2:
        if (pair_slot_kind(*(u8 **)(obj + 0x14)) == 2)
        {
            *dst = *cmds;
        }
        break;
    case 3:
        if (pair_slot_kind(*(u8 **)(obj + 0x14)) == 4)
        {
            *dst = *cmds;
        }
        break;
    case 4:
        if (pair_slot_kind(*(u8 **)(obj + 0x14)) == 3)
        {
            *dst = *cmds;
        }
        break;
    case 5:
        if (*(u8 *)(*(u8 **)(*(u8 **)(*(u8 **)(obj + 0x14) + 0x200) + 0x368)) == 9
            || *(u8 *)(*(u8 **)(*(u8 **)(*(u8 **)(obj + 0x14) + 0x204) + 0x368)) == 9)
        {
            *dst = *cmds;
        }
        break;
    case 6:
        if ((*(u8 *)(*(u8 **)(obj + 0x14) + 0xF8) & 2) != 0)
        {
            *dst = *cmds;
        }
        ((struct Bit1Bits *)(*(u8 **)(obj + 0x14) + 0xF8))->b1 = 0;
        break;
    case 7:
        if ((*(u8 *)(*(u8 **)(obj + 0x14) + 0x2B8) & 0x40) != 0)
        {
            *dst = *cmds;
        }
        break;
    case 8:
        if ((u8)sub_0803C454(*(u8 **)(obj + 0x14)) != 0)
        {
            *dst = *cmds;
        }
        break;
    case 9:
        if ((u8)sub_0803C4EC(*(u8 **)(obj + 0x14)) != 0)
        {
            *dst = *cmds;
        }
        break;
    case 10:
        if ((*(u8 *)(*(u8 **)(obj + 0x14) + 0xF8) & 4) != 0)
        {
            *dst = *cmds;
        }
        break;
    case 11:
        if ((*(u8 *)(*(u8 **)(obj + 0x14) + 0xF8) & 4) == 0)
        {
            *dst = *cmds;
        }
        break;
    }
    return 1;
}

extern u8 *gMemRoutines;
extern const u8 gAreaEntryTable[];
extern u8 *gRecordPtrTable[];
void set_flag(u32 a, u32 b, u32 value, u32 flag);

ASM_FUNC("asm/code_080EF3BC/apply_record_setting.s", void apply_record_setting(void))

struct FlagBits7 { u8 lo7 : 7; u8 b7 : 1; };
struct FlagBits6 { u8 lo6 : 6; u8 b6 : 1; u8 top : 1; };
struct FlagBits5 { u8 lo5 : 5; u8 b5 : 1; u8 top : 2; };

static inline void set_flag_bit5(u8 *p, u32 v)
{
    ((struct FlagBits5 *)p)->b5 = v;
}

static inline void set_flag_bit6(u8 *p, u32 v)
{
    ((struct FlagBits6 *)p)->b6 = v;
}

static inline void set_flag_bit7(u8 *p, u32 v)
{
    ((struct FlagBits7 *)p)->b7 = v;
}

NONMATCH("asm/code_080EF3BC/entity_setup_set_flag_bit.s", u32 entity_setup_set_flag_bit(u8 *obj, u32 unused, u32 *cmds))
{
    u8 *list = *(u8 **)(obj + 0x14);
    u32 idx;
    u32 off;
    u8 *slots;
    u8 *entry;
    s32 kind;

    idx = *cmds++;
    off = idx << 2;
    slots = list + 0x28;
    entry = *(u8 **)(slots + off);
    kind = *cmds++;

    switch (kind)
    {
    case 0:
    {
        u32 value = *cmds;
        set_flag_bit7(entry + 0x351, value);
        break;
    }
    case 1:
    {
        u32 value = *cmds;
        set_flag_bit6(list + 0x209, value);
        break;
    }
    case 2:
    {
        u32 value = *cmds;
        set_flag_bit5(list + 0x20B, value);
        break;
    }
    case 3:
    {
        u8 *bits = list + 0x26A;
        set_flag_bit6(bits, *(u8 *)cmds);
        break;
    }
    }
    return 1;
}
END_NONMATCH

u32 set_var_from_entry(u8 *owner, u8 *dst, s16 *cmd)
{
    switch (*(u32 *)((u8 *)cmd + 4))
    {
    case 0:
        set_var(gEffectState + 0x4B0, dst + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(u16 *)(*(u8 **)(owner + 0x14) + 0x124));
        break;
    case 1:
        set_var(gEffectState + 0x4B0, dst + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(s16 *)(*(u8 **)(owner + 0x14) + 0x27E));
        break;
    case 2:
        set_var(gEffectState + 0x4B0, dst + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(s16 *)(*(u8 **)(owner + 0x14) + 0x280));
        break;
    case 3:
        set_var(gEffectState + 0x4B0, dst + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(u8 *)(*(u8 **)(owner + 0x14) + 0x26B) >> 5);
        break;
    case 4:
        set_var(gEffectState + 0x4B0, dst + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(s16 *)(*(u8 **)(owner + 0x14) + 0x26E));
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080EF3BC/set_var_from_source.s", void set_var_from_source(void))

u32 set_state_rect(u32 unused0, u32 unused1, u32 *cmds)
{
    u8 **state = &gEffectState;
    u32 index = *cmds++;
    u8 *entry = *state + (index * 0x2C + 0x320);
    u32 flags = *cmds++;
    u8 *rect = entry + (((flags & 1) << 4) + 0xC);

    if (flags & 2)
    {
        cmds[0] <<= 4;
        cmds[1] <<= 4;
        cmds[2] <<= 4;
        cmds[3] <<= 4;
    }
    *(u16 *)rect = *cmds++;
    *(u16 *)(rect + 6) = *cmds++;
    *(u16 *)(rect + 2) = *cmds++;
    *(u16 *)(rect + 4) = *cmds;
    return 1;
}

u32 set_state_rect_ext(u32 unused0, u32 unused1, u32 *cmds)
{
    u8 **state = &gEffectState;
    u32 index = *cmds++;
    u8 *entry = *state + (index * 0x2C + 0x320);
    u32 flags = *cmds++;
    u8 *target = entry + (((flags & 1) << 4) + 0xC);

    if (flags & 2)
    {
        cmds[0] <<= 4;
        cmds[1] <<= 4;
        cmds[2] <<= 4;
        cmds[3] <<= 4;
        cmds[4] <<= 4;
        cmds[5] <<= 4;
    }
    *(u16 *)target = *cmds++;
    *(u16 *)(target + 6) = *cmds++;
    *(u16 *)(target + 8) = *cmds++;
    *(u16 *)(target + 2) = *cmds++;
    *(u16 *)(target + 4) = *cmds++;
    *(u16 *)(target + 0xA) = *cmds;
    return 1;
}

u32 set_state_slot(u32 unused0, u32 value, u32 unused1, u32 *cmds)
{
    u8 *state = gEffectState;
    u8 *entry;

    ((struct EffectStateMask *)state)->slotMask |= 1 << *cmds;
    entry = state + (*cmds++ * 0x2C + 0x320);
    entry[9] = *cmds++;
    entry[0xA] = *cmds++;
    ((struct EffectSlotBits *)(entry + 0xB))->flag = *cmds++;
    entry[8] = *cmds++;
    *(u32 *)(entry + 4) = *cmds;
    if (value != 0)
    {
        *(u32 *)entry = value;
    }
    return 1;
}

void step_status_1f8(u8 **obj);
void sprites_obj_step_move(u8 *list);
void decode_stream_run(u8 *record, u32 *buf, u8 *gfx, u8 *vars, u8 *extra, u32 kind);
u32 obj_command_dispatch(u8 *owner, u32 zero, u8 *record, u32 *buf);
u32 step_effect_slot(u8 *owner, u32 zero, u8 *record);

NONMATCH("asm/code_080EF3BC/step_object_effects.s", void step_object_effects(u8 **obj))
{
    u8 *counter = gEffectState + 0x548;
    u8 *record;
    u8 *owner;
    u32 flags;
    u32 buf[13];
    u32 kind;

    step_status_1f8(obj);
    *counter = 3;
    record = gEffectState + 0x1F8;
    flags = *(u16 *)(gEffectState + 0x298);
    if ((flags & 0x100) != 0)
    {
        sprites_obj_step_move(*(u8 **)(*obj + 0x14));
        return;
    }
    if ((flags & 0x14) != 0)
    {
        return;
    }
    owner = *obj;
    if ((((u16)flags ^ 1) & 1) != 0)
    {
        return;
    }
    kind = (u8)step_effect_slot(owner, 0, record);
    while (kind == 0)
    {
        decode_stream_run(record, buf, gUnk_03000FC0, gEffectState + 0x4B0, gUnk_03000FC0 + 0x38C, kind);
        if ((u8)obj_command_dispatch(owner, 0, record, buf) == 0)
        {
            break;
        }
        kind = (u8)step_effect_slot(owner, 0, record);
    }
}
END_NONMATCH

extern const u8 gEffectStateTag[];
extern u32 gTextExtraFlagPtrs[];
extern u32 gTextCountPtrs[];
extern u32 gTextKindPtrs[];
extern u32 gTextModePtrs[];
extern u32 gTextHalfPtrs[];
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void task_insert(void *node, u8 kind, u32 data);
u8 *init_effect_state(u8 *owner, u32 flag, u32 kind, u32 arg)
{
    u32 task[4];
    u8 *node;
    u8 *record;
    u8 *dst;
    u8 *base;
    u8 *state;
    u8 *pool;
    u8 k;

    k = kind;
    if (flag != 0)
    {
        node = owner + 4;
        *(u8 **)owner = node;
        task_insert(node, k, arg);
        *(const u8 **)(node + 0x18) = gDefaultTaskDesc;
    }
    record = *(u8 **)owner;
    *(u8 **)(record + 0x18) = gTaskTemplate;
    if (flag == 0)
    {
        *(struct TaskCopy *)task = *(struct TaskCopy *)gTaskTemplate;
        *(u8 **)(record + 0x18) = (u8 *)task;
        dst = (u8 *)task;
        base = record - 4;
        *(u16 *)(dst + 8) = (u16)((u32)owner - (u32)base + *(u16 *)(gTaskTemplate + 8));
    }
    gEffectState = heap_alloc_raw(1, 0x560, gEffectStateTag);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gEffectState, 0x560);
    state = gUnk_03000FC0;
    *(u32 *)(state + 0x55C) = gTextExtraFlagPtrs[0];
    *(u32 *)(state + 0x560) = gTextCountPtrs[0];
    *(u32 *)(state + 0x564) = gTextKindPtrs[0];
    *(u32 *)(state + 0x568) = gTextModePtrs[0];
    *(u32 *)(state + 0x56C) = gTextHalfPtrs[0];
    gUnk_03000FCC = 0;
    gUnk_03000FC8 = 0;
    pool = gCurObj;
    pool[0x27] = 0;
    pool[0x26] = 0;
    return owner;
}

void update_object_states(u8 **task);
void step_bobbing_offset(u8 **task);
void advance_motion_slots(u8 **task);
ASM_FUNC("asm/code_080EF3BC/update_object_effects.s", void update_object_effects(void))

ASM_FUNC("asm/code_080EF3BC/show_text_window.s", void show_text_window(void))

u32 slot_countdown(u8 *pool, u32 idx);

NONMATCH("asm/code_080EF3BC/update_effect_slots.s", void update_effect_slots(u8 **task))
{
    u8 *slot;
    s16 i;
    s32 x;
    s32 y;
    u8 idx;

    i = 0;
    slot = *(u8 **)gUnk_03000FC8;
    for (; i <= 3; i++, slot += 0x10)
    {
        idx = i;
        if ((u8)slot_countdown(gUnk_03000FC8, idx) != 0)
        {
            if (slot[0xE] != 0)
            {
                u8 *list = *(u8 **)(*task + 0x14);
                u32 off = slot[0xC] << 2;
                u8 *base = list + 0x28;
                u8 *z = *(u8 **)(base + off);
                entity_camera_offset(list, slot[0xC], &x, &y);
                y = y - z[0x283];
                *(s16 *)(slot + 8) = x;
                *(s16 *)(slot + 0xA) = y;
            }
            update_slot_bounds(gUnk_03000FC8, idx);
        }
    }
}
END_NONMATCH

void apply_key_filter(u8 *list);
void start_effect(u8 **owner, u32 arg)
{
    u8 *list;
    u8 **ptr;
    s16 count;
    u8 *sub;

    ((s8 *)gEffectState)[0x55A] &= -3;
    ((s8 *)gEffectState)[0x55A] &= -0x39;
    {
        u8 *state = gEffectState;
        *(u16 *)(state + 0x558) = 0;
        *(u16 *)(state + 0x556) = 0;
        *(u16 *)(state + 0x554) = 0;
        state[0x549] = 0x10;
    }
    apply_key_filter(*(u8 **)(*owner + 0x14));
    list = *(u8 **)(*owner + 0x14);
    count = list[0x10B];
    ptr = (u8 **)(list + 0x28);
    if (count != 0)
    {
        do
        {
            reset_obj_params_b(*owner + 0x1C, *ptr + 0x58, *(u32 *)(*ptr + 0x228), 0, 1, 0xFF);
            ptr++;
            count--;
        } while (count > 0);
    }
    sub = *(u8 **)(*owner + 0x18);
    ((void (*)(u8 *))(*(u32 *)(sub + 0xC)))(*owner + *(s16 *)(sub + 8));
    reset_obj_params_b(*owner + 0x1C, gEffectState, arg, 0, 1, 0xFF);
}

ASM_FUNC("asm/code_080EF3BC/register_effect_entry.s", void register_effect_entry(void))

ASM_FUNC("asm/code_080EF3BC/update_setup_bg_layer.s", void update_setup_bg_layer(void))

ASM_FUNC("asm/code_080EF3BC/reset_effect_state.s", void reset_effect_state(void))

ASM_FUNC("asm/code_080EF3BC/update_object_states.s", void update_object_states(u8 **task))

ASM_FUNC("asm/code_080EF3BC/step_bobbing_offset.s", void step_bobbing_offset(u8 **task))

ASM_FUNC("asm/code_080EF3BC/advance_motion_slots.s", void advance_motion_slots(u8 **task))

void free_effect_state(void)
{
    heap_free(*(u8 **)(gEffectState + 0x4AC));
    *(u32 *)(gEffectState + 0x4AC) = 0;
    if (gUnk_03000FCC != 0)
    {
        release_obj_parts(gUnk_03000FCC, 3);
        gUnk_03000FCC = 0;
    }
    if (gUnk_03000FC8 != 0)
    {
        free_ptr_pair(gUnk_03000FC8, 3);
        gUnk_03000FC8 = 0;
    }
}

void init_effect_pool(u32 unused, u16 value, u8 flags)
{
    u8 *pool = gCurObj;

    *(u16 *)(pool + 0x20) = value;
    ((struct EffectPoolBits *)(pool + 0x28))->mode = flags;
    gUnk_03000FC8 = init_slot_table(heap_alloc2(4, 0, gEffectPoolTag, 0));
}

void unknown_thumb_080f0a54(u8 **owner, u8 idx, u32 arg)
{
    u8 *table = *(u8 **)(*owner + 0x14);
    u8 *ent = *(u8 **)(table + idx * 4 + 0x28);
    u8 *state = ent + 0x58;
    u16 *flags = (u16 *)(ent + 0xF8);

    if (*flags & 2)
    {
        obj_finish_move_step(ent);
    }
    if (*flags & 0x40)
    {
        obj_stop_motion(ent);
    }
    if (*flags & 0x20)
    {
        obj_apply_state_a(ent);
    }
    reset_obj_params_b(*owner + 0x1C, state, arg, 0, 1, 0xFF);
}

void start_effect_slot(u32 *owner, u32 value, u8 flag, u16 word)
{
    u8 **pstate = &gEffectState;
    u8 *state = *pstate;
    u32 la[2];

    if (*(u16 *)(state + 0x148) & 1)
    {
        if (*(u16 *)(state + 0x1F0) & 1)
        {
            return;
        }
        if ((*(u8 *)(state + 0x55A) & 1) == 0)
        {
            return;
        }
    }
    la[0] = 2;
    la[1] = value;
    set_var(state + 0x4B0, 0, gUnk_03000FC0 + 0x38C, 0, 0x2F, flag);
    register_effect_slot(*owner, *pstate + 0xA8, la);
    *(u16 *)(*pstate + 0x54C) = word;
}

void refresh_bg_layer(void)
{
    clear_slot_rect(gUnk_03000FCC, 4);
    copy_map_rects(gUnk_03000FCC, 0x10);
}

void teardown_effect_task(u8 *obj, u32 flags)
{
    u8 *record = *(u8 **)obj;
    u32 task[4];
    u8 *owner;
    u8 *base;
    u8 *dst;

    *(u8 **)(record + 0x18) = gTaskTemplate;
    if (flags == 0)
    {
        *(struct TaskCopy *)task = *(struct TaskCopy *)gTaskTemplate;
        *(u8 **)(record + 0x18) = (u8 *)task;
        dst = (u8 *)task;
        base = record - 4;
        *(u16 *)(dst + 8) = (u16)((u32)obj - (u32)base + *(u16 *)(gTaskTemplate + 8));
    }

    text_window_close();
    heap_free(gEffectState);
    if (flags & 2)
    {
        owner = *(u8 **)obj;
        *(const u8 **)(owner + 0x18) = gDefaultTaskDesc;
        task_remove(owner, 0);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}
