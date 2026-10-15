#include "global.h"

struct OwnerState
{
    u8 filler0[0x888];
    u32 handles[0x46];
    u8 cur9A0;
    u8 mode9A1;
    u8 busy9A2;
    u8 flags9A3;
};

struct Slot
{
    u8 *obj;
    u8 filler4[0x14 - 4];
    s32 xFix;
    s32 yFix;
    u8 filler1C[0x3A - 0x1C];
    u16 scale;
    u8 flags;
    u8 unk3D;
    u8 filler3E[0x40 - 0x3E];
};

struct ObjList
{
    u8 filler0[0x158];
    struct Slot *slots;
};

struct Owner
{
    u8 filler0[0x14];
    struct ObjList *list;
    u32 type;
    u8 filler1C[0x20 - 0x1C];
    u32 aux;
    struct OwnerState *state;
    u8 *entry;
};

extern u8 *gUnk_03000FC0;
extern u8 gOwnerTypeCreateDestroy[];
extern u8 gStateTag[];
extern u32 gTextExtraFlagPtrs[];
extern u32 gTextCountPtrs[];
extern u32 gTextKindPtrs[];
extern u32 gTextModePtrs[];
extern u32 gTextHalfPtrs[];
extern u8 *gEffectState;
extern u8 *gMemRoutines;
extern u8 gUnk_03000C80[];
extern u8 gClaimKindTable[];

void clear_command_records(struct Owner *owner);
void owner_reset(struct Owner *owner);
void owner_release_handles(struct Owner *owner);
void clear_slot_rect(u8 *rect, u32 n);
void copy_map_rects(u8 *rects, u32 n);
void task_insert(struct Owner *owner, u32 prio, u32 arg);
void task_remove(struct Owner *owner, u32 arg);
u8 *heap_alloc_raw(u32 a, u32 size, u8 *tag);
void heap_free(u8 *p);
void free_ptr_pair(u8 *p, u32 n);
void release_obj_parts(u8 *p, u32 n);
void unpack_stream_values(u8 *a, u32 *stream, u32 *p1, u32 *p2, u32 *p3);
void tween_move_to(struct ObjList *list, u32 index, u32 flags, u32 one, u32 v1, u32 v2, s32 v3);
void release_obj_131c(u32 handle);
u32 claim_table_slot(u32 table, u32 a, u32 b, u32 c, s32 d, s32 e, u32 f);
void update_slot_bounds(u32 table, u32 index);

struct SprBits11
{
    u8 lo : 6;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct FlagBit0
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct SprBits12
{
    u8 lo : 4;
    u8 b4 : 1;
    u8 hi : 3;
};

// Sets the mirror bit (bit 6 of the sprite byte at +0x11).
static inline void spr_set_b6(u8 *spr, u32 flag)
{
    ((struct SprBits11 *)(spr + 0x11))->b6 = flag;
}

void set_tween_anim(struct ObjList *list, u32 a, u32 b, u32 c);
void slot_load_object(struct Owner *owner, u32 index);
void slot_store_object(struct Owner *owner, u32 index);
void ensure_tween_sprite(struct ObjList *list, u32 a, u32 b);
u8 flags_vars_set_obj_state(u8 *owner1C, u32 a2, u8 *obj, u32 b, u32 c);
u32 sub_08122768(struct ObjList *list);
u32 tween_step(struct ObjList *list, u32 id);
void start_sub_obj(struct ObjList *list);
void start_sub_obj_display(struct ObjList *list);
void release_obj_slots(struct ObjList *list);
void update_sprite_obj(struct ObjList *list);
u32 restore_obj_word0(u8 *obj, u32 arg);
void reset_obj_params_b(u8 *obj, u8 *state, u32 arg, u32 zero, u32 one, u32 ff);
u32 record_advance(struct Owner *owner, u8 *rec);
u32 record_dispatch(struct Owner *owner, u8 *rec, u32 *buf);
void decode_stream_run(u8 *rec, u32 *buf, u8 *block, u8 *p3, u8 *p4, u32 advanced);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);


ASM_FUNC("asm/code_080FA598/cmd_set_value.s", void cmd_set_value(void))

ASM_FUNC("asm/code_080FA598/cmd_slot_value_by_kind.s", void cmd_slot_value_by_kind(void))

ASM_FUNC("asm/code_080FA598/cmd_slot_op.s", void cmd_slot_op(void))

u32 cmd_slot_field(struct Owner *owner, u32 unused, u32 *data)
{
    u8 index = *data++;
    u32 kind = *data++;

    switch (kind)
    {
    case 0:
    {
        u32 value = *data;

        if (value & 0x40)
            owner->list->slots[index].scale = (value & 0xF) << 4;
        else
            owner->list->slots[index].scale = value << 8;
        break;
    }
    case 1:
    {
        u32 value = *data;

        if (value & 0x40)
        {
            u8 *obj = owner->list->slots[index].obj;
            u32 low = value & 0xF;

            obj[0x20] = low;
        }
        else
        {
            u8 *obj = owner->list->slots[index].obj;
            u32 high = value << 4;

            obj[0x20] = high;
        }
        break;
    }
    }
    return 1;
}

u8 cmd_place_object(struct Owner *owner, u32 a2, u32 *args)
{
    u8 *spr;

    slot_load_object(owner, (u8)args[0]);
    ensure_tween_sprite(owner->list, (u8)args[0], args[1]);
    set_tween_anim(owner->list, (u8)args[0], args[2], 0);
    slot_store_object(owner, (u8)args[0]);
    spr = owner->list->slots[args[0]].obj;
    ((struct SprBits12 *)(spr + 0x12))->b4 = 0;
    spr_set_b6(spr, args[3]);
    return flags_vars_set_obj_state((u8 *)owner + 0x1C, a2, spr, (u8)args[4], 0);
}

ASM_FUNC("asm/code_080FA598/cmd_start_move_list.s", void cmd_start_move_list(void))

ASM_FUNC("asm/code_080FA598/cmd_set_position.s", void cmd_set_position(void))

ASM_FUNC("asm/code_080FA598/cmd_place_sprite.s", void cmd_place_sprite(void))

ASM_FUNC("asm/code_080FA598/cmd_aux_entry.s", void cmd_aux_entry(void))

u32 cmd_latch_when_ready(struct Owner *owner, u32 *cmd)
{
    if ((u8)sub_08122768(owner->list) != 0)
    {
        cmd[0] = cmd[5];
        return 0;
    }
    return 1;
}

u32 cmd_list_toggle(struct Owner *owner, u32 unused, u32 *data)
{
    if (*data & 1)
        start_sub_obj(owner->list);
    else
        start_sub_obj_display(owner->list);
    return 1;
}

u32 stub_return_0(struct ObjList *list);

u32 unknown_thumb_080faf3a(struct Owner *owner, u32 *cmd)
{
    if ((u8)stub_return_0(owner->list) != 1)
    {
        cmd[0] = cmd[5];
        return 0;
    }
    owner->state->mode9A1 = 2;
    return 1;
}

ASM_FUNC("asm/code_080FA598/cmd_start_move.s", void cmd_start_move(void))

u32 cmd_list_mode(struct Owner *owner, u32 unused, u32 *data)
{
    switch (*data)
    {
    case 0:
        release_obj_slots(owner->list);
        break;
    case 1:
        update_sprite_obj(owner->list);
        break;
    }
    return 1;
}

u8 cmd_set_mode_1(struct Owner *owner, u32 arg)
{
    owner->state->mode9A1 = 1;
    return restore_obj_word0((u8 *)owner + 0x1C, arg);
}

u32 cmd_latch_when_free(struct Owner *owner, u32 *cmd)
{
    u32 own = ((u32)owner->state->busy9A2 << 31) >> 31;

    if ((gUnk_03000C80[0] | own) != 0)
    {
        cmd[0] = cmd[5];
        return 0;
    }
    return 1;
}

ASM_FUNC("asm/code_080FA598/cmd_push_handle.s", void cmd_push_handle(void))

u32 cmd_set_object_flags_list(struct Owner *owner, u32 unused, u32 *cmd)
{
    set_tween_anim(owner->list, (u8)cmd[0], owner->list->slots[cmd[0]].obj[0x24], cmd[1]);
    spr_set_b6(owner->list->slots[cmd[0]].obj, cmd[2]);
    owner->list->slots[cmd[0]].obj[0x12] |= 0x10;
    return 1;
}

u32 cmd_set_obj_flags_list2(struct Owner *owner, u32 unused, u32 *cmd)
{
    set_tween_anim(owner->list, (u8)cmd[0], cmd[1], cmd[2]);
    spr_set_b6(owner->list->slots[cmd[0]].obj, cmd[3]);
    owner->list->slots[cmd[0]].obj[0x12] |= 0x10;
    return 1;
}

u8 cmd_obj_op_ae49(struct Owner *owner, u32 a2, u32 *cmd)
{
    u8 *obj;

    set_tween_anim(owner->list, (u8)cmd[0], cmd[1], 0);
    obj = owner->list->slots[cmd[0]].obj;
    spr_set_b6(obj, cmd[2]);
    ((struct SprBits12 *)(obj + 0x12))->b4 = 0;
    return flags_vars_set_obj_state((u8 *)owner + 0x1C, a2, obj, (u8)cmd[3], 0);
}

u32 cmd_place_slot_flags(struct Owner *owner, u32 unused, u32 *cmd)
{
    slot_load_object(owner, (u8)cmd[0]);
    ensure_tween_sprite(owner->list, (u8)cmd[0], cmd[1]);
    set_tween_anim(owner->list, (u8)cmd[0], cmd[2], cmd[3]);
    slot_store_object(owner, (u8)cmd[0]);
    spr_set_b6(owner->list->slots[cmd[0]].obj, cmd[4]);
    owner->list->slots[cmd[0]].obj[0x12] |= 0x10;
    return 1;
}

u32 cmd_set_pos_for_current(struct Owner *owner, u32 unused, u32 *cmd)
{
    u8 index = owner->state->cur9A0 - 1;

    set_tween_anim(owner->list, index, cmd[0], cmd[1]);
    spr_set_b6(owner->list->slots[index].obj, cmd[2]);
    owner->list->slots[index].obj[0x12] |= 0x10;
    return 1;
}

u8 cmd_object_op_cur(struct Owner *owner, u32 a2, u32 *cmd)
{
    u8 index = owner->state->cur9A0 - 1;
    u8 *obj;

    set_tween_anim(owner->list, index, cmd[0], 0);
    obj = owner->list->slots[index].obj;
    ((struct SprBits12 *)(obj + 0x12))->b4 = 0;
    spr_set_b6(obj, cmd[1]);
    return flags_vars_set_obj_state((u8 *)owner + 0x1C, a2, obj, (u8)cmd[2], 0);
}

u32 cmd_place_obj_sync(struct Owner *owner, u32 unused, u32 *cmd)
{
    u8 index = owner->state->cur9A0 - 1;

    slot_load_object(owner, index);
    ensure_tween_sprite(owner->list, index, cmd[0]);
    set_tween_anim(owner->list, index, cmd[1], cmd[2]);
    slot_store_object(owner, index);
    spr_set_b6(owner->list->slots[index].obj, cmd[3]);
    owner->list->slots[index].obj[0x12] |= 0x10;
    return 1;
}

u8 cmd_place_obj_flags3(struct Owner *owner, u32 a2, u32 *cmd)
{
    u8 index = owner->state->cur9A0 - 1;
    u8 *obj;

    slot_load_object(owner, index);
    ensure_tween_sprite(owner->list, index, cmd[0]);
    set_tween_anim(owner->list, index, cmd[1], 0);
    slot_store_object(owner, index);
    obj = owner->list->slots[index].obj;
    ((struct SprBits12 *)(obj + 0x12))->b4 = 0;
    spr_set_b6(obj, cmd[2]);
    return flags_vars_set_obj_state((u8 *)owner + 0x1C, a2, obj, (u8)cmd[3], 0);
}

u32 cmd_obj_setup_global(struct Owner *owner, u32 unused, u32 *cmd)
{
    reset_obj_params_b((u8 *)owner + 0x1C, gEffectState + 0x1F8, cmd[1], 0, 1, 0xFF);
    return 1;
}

u32 cmd_reset_next_record(struct Owner *owner, u32 unused, u32 *cmd)
{
    u32 idx = cmd[0];
    u8 *rec = (u8 *)owner->state + idx * 0xA8;
    u8 *next = rec + 0xA8;
    u8 *obj = owner->list->slots[idx].obj;
    s32 v;

    *(u16 *)(rec + 0x148) = 0;
    *(u32 *)next = *(u32 *)(next + 4);
    v = obj[0x12];
    v = (-7 & v) | 2;
    obj[0x12] = v;
    return 1;
}

ASM_FUNC("asm/code_080FA598/cmd_set_slot_flag.s", void cmd_set_slot_flag(void))

u32 cmd_find_record(struct Owner *owner, u32 *cmd, u32 *key_ptr, u8 key)
{
    u8 *rec = (u8 *)owner->state + 0xA8;
    u32 start = 12;
    s16 remaining;

    if (key_ptr[0] == 0)
    {
        rec += key_ptr[1] * 0xA8;
        start = 1;
    }
    remaining = start;
    if (remaining != 0)
    {
        do
        {
            if ((*(u16 *)(rec + 0xA0) & 1) != 0 && rec[0xA4] == key)
            {
                cmd[0] = cmd[5];
                return 0;
            }
            remaining--;
            rec += 0xA8;
        } while (remaining > 0);
    }
    return 1;
}

ASM_FUNC("asm/code_080FA598/cmd_set_next_flag4.s", void cmd_set_next_flag4(void))

// Runs the record while bit 0 of its flags (+0xA0) is set.
u32 run_record(struct Owner *owner, u8 *rec)
{
    u32 buf[13];
    u32 advanced;

    if (((rec[0xA0] ^ 1) & 1) != 0)
        return 0;
    while ((advanced = (u8)record_advance(owner, rec)) == 0)
    {
        decode_stream_run(rec, buf, gUnk_03000FC0, (u8 *)owner->state + 0x908,
                          (u8 *)owner->state + 0x99C, advanced);
        if ((u8)record_dispatch(owner, rec, buf) == 0)
            break;
    }
    return 1;
}

void owner_release_handles(struct Owner *owner)
{
    s16 remaining = owner->state->busy9A2 >> 1;
    u32 *slot = owner->state->handles;

    if (remaining != 0)
    {
        do
        {
            if (*slot != 0)
            {
                release_obj_131c(*slot);
                *slot = 0;
            }
            remaining--;
            slot++;
        } while (remaining > 0);
    }
    owner->state->busy9A2 = owner->state->busy9A2 & 1;
    ((struct FlagBit0 *)&owner->state->busy9A2)->b0 = 0;
}

void clear_command_records(struct Owner *owner)
{
    s16 remaining = 13;
    u8 *rec = (u8 *)owner->state;

    do
    {
        ((void (*)(u32, u8 *, u32))gMemRoutines)(0, rec, 0xA8);
        rec += 0xA8;
        remaining--;
    } while (remaining > 0);
}

ASM_FUNC("asm/code_080FA598/tick_slot_timer.s", void tick_slot_timer(void))

void toggle_on_motion_done(struct Owner *owner, u8 *state, u8 id)
{
    if ((u8)tween_step(owner->list, id) == 1)
        *(u16 *)(state + 0xA0) ^= 2;
}

u8 state_mode(struct Owner *owner)
{
    return owner->state->mode9A1;
}

void owner_stop(struct Owner *owner)
{
    u32 *word;

    owner_reset(owner);
    owner_release_handles(owner);
    word = (u32 *)(gUnk_03000FC0 + 0x100);
    *word &= -2;
}

void clear_slot_marks(struct Owner *owner)
{
    if (owner->state->flags9A3 & 1)
    {
        s16 remaining = 12;
        struct Slot *slot = owner->list->slots;

        do
        {
            if (slot->obj != 0)
                slot->obj[0x20] = 0;
            slot++;
            remaining--;
        } while (remaining > 0);
    }
    clear_command_records(owner);
}

void unknown_thumb_080fb7b4(struct Owner *owner)
{
    clear_command_records(owner);
    owner_reset(owner);
}

void begin_state_mode(struct Owner *owner, u32 arg)
{
    reset_obj_params_b((u8 *)owner + 0x1C, (u8 *)owner->state, arg, 0, 1, 0xFF);
    owner->state->mode9A1 = 2;
}

void unknown_thumb_080fb7f8(struct Owner *owner)
{
    clear_slot_rect(owner->entry, 4);
    copy_map_rects(owner->entry, 0x10);
}

void owner_destroy(struct Owner *owner, u32 arg)
{
    u8 *flag;

    owner->type = (u32)gOwnerTypeCreateDestroy;
    owner_stop(owner);
    if (owner->aux != 0)
    {
        free_ptr_pair((u8 *)owner->aux, 3);
        owner->aux = 0;
    }
    if (owner->entry != 0)
    {
        release_obj_parts(owner->entry, 3);
        owner->entry = 0;
    }
    heap_free((u8 *)owner->state);
    flag = gUnk_03000FC0;
    *(u32 *)(flag + 0x55C) = gTextExtraFlagPtrs[0];
    *(u32 *)(flag + 0x560) = gTextCountPtrs[0];
    *(u32 *)(flag + 0x564) = gTextKindPtrs[0];
    *(u32 *)(flag + 0x568) = gTextModePtrs[0];
    *(u32 *)(flag + 0x56C) = gTextHalfPtrs[0];
    task_remove(owner, arg);
}

struct Owner *owner_create(struct Owner *owner, u32 kind, u32 arg)
{
    u8 *state;
    u8 *flag;

    task_insert(owner, (u8)kind, arg);
    owner->type = (u32)gOwnerTypeCreateDestroy;
    owner->entry = 0;
    owner->aux = 0;
    state = heap_alloc_raw(1, 0x9A8, gStateTag);
    owner->state = (struct OwnerState *)state;
    ((void (*)(u32, u8 *, u32))gMemRoutines)(0, state, 0x9A8);
    flag = gUnk_03000FC0;
    *(u32 *)(flag + 0x55C) = gTextExtraFlagPtrs[2];
    *(u32 *)(flag + 0x560) = gTextCountPtrs[2];
    *(u32 *)(flag + 0x564) = gTextKindPtrs[2];
    *(u32 *)(flag + 0x568) = gTextModePtrs[2];
    *(u32 *)(flag + 0x56C) = gTextHalfPtrs[2];
    return owner;
}

ASM_FUNC("asm/code_080FA598/load_object_def.s", void load_object_def(void))

ASM_FUNC("asm/code_080FA598/init_object_from_def.s", void init_object_from_def(void))
