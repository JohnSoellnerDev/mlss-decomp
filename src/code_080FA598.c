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

u32 cmd_set_mode2_or_latch(struct Owner *owner, u32 *cmd)
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
