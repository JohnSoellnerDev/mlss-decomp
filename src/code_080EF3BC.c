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

ASM_FUNC("asm/code_080EF3BC/apply_anchor_command.s", void apply_anchor_command(void))

ASM_FUNC("asm/code_080EF3BC/check_object_condition.s", void check_object_condition(void))

extern u8 *gMemRoutines;
ASM_FUNC("asm/code_080EF3BC/apply_record_setting.s", void apply_record_setting(void))

ASM_FUNC("asm/code_080EF3BC/entity_setup_set_flag_bit.s", void entity_setup_set_flag_bit(void))

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

ASM_FUNC("asm/code_080EF3BC/step_object_effects.s", void step_object_effects(void))

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

ASM_FUNC("asm/code_080EF3BC/update_effect_slots.s", void update_effect_slots(void))

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
