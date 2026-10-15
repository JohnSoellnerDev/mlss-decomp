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
