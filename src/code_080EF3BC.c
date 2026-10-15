#include "global.h"

extern u8 *gEffectState;
extern u8 *gUnk_03000FC0;
extern u8 *gUnk_03000FC8;
extern u8 *gUnk_03000FCC;
extern u8 gTaskTemplate[];
extern u8 gClaimKindTable[];
extern const u8 gDefaultTaskDesc[];

struct TaskCopy
{
    u32 words[4];
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

ASM_FUNC("asm/code_080EF3BC/apply_record_setting.s", void apply_record_setting(void))

ASM_FUNC("asm/code_080EF3BC/entity_setup_set_flag_bit.s", void entity_setup_set_flag_bit(void))

ASM_FUNC("asm/code_080EF3BC/set_var_from_entry.s", void set_var_from_entry(void))

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

ASM_FUNC("asm/code_080EF3BC/set_state_slot.s", void set_state_slot(void))

ASM_FUNC("asm/code_080EF3BC/step_object_effects.s", void step_object_effects(void))

ASM_FUNC("asm/code_080EF3BC/init_effect_state.s", void init_effect_state(void))

ASM_FUNC("asm/code_080EF3BC/update_object_effects.s", void update_object_effects(void))

ASM_FUNC("asm/code_080EF3BC/show_text_window.s", void show_text_window(void))

ASM_FUNC("asm/code_080EF3BC/update_effect_slots.s", void update_effect_slots(void))

ASM_FUNC("asm/code_080EF3BC/start_effect.s", void start_effect(void))

ASM_FUNC("asm/code_080EF3BC/register_effect_entry.s", void register_effect_entry(void))

ASM_FUNC("asm/code_080EF3BC/update_setup_bg_layer.s", void update_setup_bg_layer(void))

ASM_FUNC("asm/code_080EF3BC/reset_effect_state.s", void reset_effect_state(void))

ASM_FUNC("asm/code_080EF3BC/update_object_states.s", void update_object_states(void))

ASM_FUNC("asm/code_080EF3BC/step_bobbing_offset.s", void step_bobbing_offset(void))

ASM_FUNC("asm/code_080EF3BC/advance_motion_slots.s", void advance_motion_slots(void))

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

ASM_FUNC("asm/code_080EF3BC/init_effect_pool.s", void init_effect_pool(void))

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
