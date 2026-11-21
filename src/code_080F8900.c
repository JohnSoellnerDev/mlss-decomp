#include "global.h"

u32 apply_obj_mode(void *, void *, void *);
u32 binary_op_and_set_var(void *, void *, void *, void *, void *, void *);
u32 branch_if_list_entry(void *, void *, void *, void *);
u32 branch_if_state_bit(void *, void *, void *, void *);
u32 bump_or_branch_record(void *, void *, void *, void *);
u32 call_b0c0(void *, void *, void *);
u32 clear_list_entry(void *, void *, void *, void *);
u32 cmd_aux_entry(void *, void *, void *);
u32 cmd_find_record(void *, void *, void *, u32);
u32 cmd_latch_when_free(void *, void *);
u32 cmd_latch_when_ready(void *, void *);
u32 cmd_list_mode(void *, void *, void *);
u32 cmd_list_toggle(void *, void *, void *);
u32 cmd_obj_op_ae49(void *, void *, void *);
u32 cmd_obj_setup_global(void *, void *, void *);
u32 cmd_object_op_cur(void *, void *, void *);
u32 cmd_place_obj_flags3(void *, void *, void *);
u32 cmd_place_obj_sync(void *, void *, void *);
u32 cmd_place_object(void *, void *, void *);
u32 cmd_place_slot_flags(void *, void *, void *);
u32 cmd_place_sprite(void *, void *, void *, void *);
u32 cmd_push_handle(void *, void *, void *);
u32 cmd_read_store(void *, void *, void *, void *);
u32 cmd_reset_next_record(void *, void *, void *);
u32 cmd_set_mode_1(void *, void *, void *);
u32 cmd_set_next_flag4(void *, void *, void *);
u32 cmd_set_obj_flags_list2(void *, void *, void *);
u32 cmd_set_object_flags_list(void *, void *, void *);
u32 cmd_set_pos_for_current(void *, void *, void *);
u32 cmd_set_position(void *, void *, void *);
u32 cmd_set_slot_flag(void *, void *, void *);
u32 cmd_set_value(void *, void *, void *, u32);
u32 cmd_set_var_lang(void *, void *, void *, void *);
u32 cmd_slot_field(void *, void *, void *);
u32 cmd_slot_op(void *, void *, void *);
u32 cmd_slot_value_by_kind(void *, void *, void *);
u32 cmd_start_move(void *, void *, void *);
u32 cmd_start_move_list(void *, void *, void *);
u32 cmd_store_obj_field(void *, void *, void *, void *);
u32 copy_if_cmp_match(void *, void *, void *);
u32 copy_if_slot_bit(void *, void *, void *, void *);
u32 copy_on_key_match_a(void *, void *, void *);
u32 copy_on_key_match_b(void *, void *, void *);
u32 counter_step_and_branch(void *, void *, void *);
u32 cur_obj_bbf8(void *, void *, void *);
u32 dispatch_obj_mode_call(void *, void *, void *);
u32 draw_text_item(void *, void *, void *, void *);
u32 draw_text_item_raw(void *, void *, void *, void *);
u32 map_rects_cmd(void *, void *, void *, void *);
u32 pick_random_entry(void *, void *, void *);
u32 pop_obj_entry(void *, void *);
u32 push_obj_entry(void *, void *, void *);
u32 random_obj_placement(void *, void *, void *, void *);
u32 request_if_idle(void *, void *, void *);
u32 reset_cursor(void *, void *);
u32 restore_obj_word0(void *, void *);
u32 set_cur_obj_26_27(void *, void *, void *);
u32 set_cur_obj_29(void *, void *, void *);
u32 set_key_ignore_mask(void *, void *, void *);
u32 set_key_mode_bits(void *, void *, void *);
u32 set_obj_flag(void *, void *, void *);
u32 set_obj_if_cmp(void *, void *, void *);
u32 set_obj_word0_saved(void *, void *, void *);
u32 set_obj_word9c_flag(void *, void *, void *);
u32 set_obj_word_98(void *, void *, void *);
u32 set_ramp0_if_zero(void *, void *, void *);
u32 set_shadow_bits(void *, void *, void *);
u32 set_slot_param(void *, void *, void *);
u32 set_text_window_32(void *, void *, void *);
u32 set_var_from_key_group(void *, void *, void *, void *);
u32 set_var_from_keys(void *, void *, void *, void *);
u32 set_var_from_text_byte(void *, void *, void *, void *);
u32 slot_target_cmd(void *, void *, void *);
u32 slot_value_cmd(void *, void *, void *);
u32 store_if_text_byte(void *, void *, void *);
u32 store_obj_byte_to_var(void *, void *, void *, void *);
u32 update_obj_counters(void *, void *, void *);

static inline u8 *rd_ctx(u8 *owner)
{
    return owner + 0x1C;
}

static inline u8 *rd_state(u8 *owner)
{
    return *(u8 **)(owner + 0x24);
}

static inline u8 *rd_entry(u8 *owner)
{
    return *(u8 **)(owner + 0x28);
}

static inline u8 *rd_aux(u8 *owner)
{
    return *(u8 **)(owner + 0x20);
}

static inline u8 *rd_vars(u8 *owner)
{
    return rd_state(owner) + 0x908;
}

static inline u8 *rd_params(u8 *owner)
{
    return rd_state(owner) + 0x99C;
}

static inline u8 rd_cur(u8 *owner)
{
    return rd_state(owner)[0x9A0];
}

// Script command opcodes, the byte at +0xA7 of a command record; record_dispatch
// runs them. Names follow their handlers.
enum RecordCmd
{
    RCMD_RESET_CURSOR = 0x00,
    RCMD_RESTORE_OBJ_WORD0 = 0x01,
    RCMD_SET_OBJ_WORD0_SAVED = 0x02,
    RCMD_SET_OBJ_WORD9C_FLAG = 0x03,
    RCMD_SET_VALUE = 0x04,
    RCMD_SET_NEXT_FLAG4 = 0x05,
    RCMD_FIND_RECORD = 0x06,
    RCMD_RESET_NEXT_RECORD = 0x07,
    RCMD_OBJ_SETUP_GLOBAL = 0x09,
    RCMD_START_MOVE = 0x0D,
    RCMD_OBJECT_OP_CUR = 0x10,
    RCMD_SET_POS_FOR_CURRENT = 0x11,
    RCMD_OBJ_OP_AE49 = 0x12,
    RCMD_SET_OBJ_FLAGS_LIST2 = 0x13,
    RCMD_SET_OBJECT_FLAGS_LIST = 0x14,
    RCMD_DRAW_TEXT_ITEM_RAW = 0x15,
    RCMD_DRAW_TEXT_ITEM = 0x16,
    RCMD_PLACE_SPRITE = 0x17,
    RCMD_BRANCH_IF_STATE_BIT = 0x18,
    RCMD_MAP_RECTS = 0x19,
    RCMD_BUMP_OR_BRANCH = 0x1A,
    RCMD_COPY_IF_SLOT_BIT = 0x1C,
    RCMD_STORE_IF_TEXT_BYTE = 0x1D,
    RCMD_SET_VAR_FROM_TEXT_BYTE = 0x1E,
    RCMD_SET_TEXT_WINDOW_32 = 0x1F,
    RCMD_CLEAR_LIST_ENTRY = 0x20,
    RCMD_AUX_ENTRY = 0x21,
    RCMD_BRANCH_IF_LIST_ENTRY = 0x22,
    RCMD_SLOT_TARGET = 0x23,
    RCMD_SET_SLOT_PARAM = 0x24,
    RCMD_SLOT_VALUE = 0x25,
    RCMD_SET_RAMP0_IF_ZERO = 0x26,
    RCMD_CUR_OBJ_BBF8 = 0x27,
    RCMD_SET_CUR_OBJ_29 = 0x28,
    RCMD_SET_CUR_OBJ_26_27 = 0x29,
    RCMD_APPLY_OBJ_MODE = 0x2A,
    RCMD_STORE_OBJ_BYTE_TO_VAR = 0x2B,
    RCMD_LIST_TOGGLE = 0x2C,
    RCMD_LATCH_WHEN_READY = 0x2F,
    RCMD_UPDATE_OBJ_COUNTERS = 0x31,
    RCMD_SET_OBJ_FLAG = 0x32,
    RCMD_STORE_OBJ_FIELD = 0x33,
    RCMD_UPDATE_AND_STORE = 0x34,
    RCMD_READ_STORE = 0x35,
    RCMD_BINARY_OP = 0x36,
    RCMD_SET_SHADOW_BITS = 0x37,
    RCMD_SET_KEY_MODE_BITS = 0x38,
    RCMD_SET_VAR_FROM_KEY_GROUP = 0x39,
    RCMD_SET_VAR_FROM_KEYS = 0x3A,
    RCMD_COPY_ON_KEY_MATCH_B = 0x3B,
    RCMD_COPY_ON_KEY_MATCH_A = 0x3C,
    RCMD_CALL_B0C0 = 0x3D,
    RCMD_COPY_IF_CMP_MATCH = 0x3F,
    RCMD_RANDOM_OBJ_PLACEMENT = 0x41,
    RCMD_PICK_RANDOM_ENTRY = 0x42,
    RCMD_PUSH_OBJ_ENTRY = 0x43,
    RCMD_POP_OBJ_ENTRY = 0x44,
    RCMD_COUNTER_STEP_AND_BRANCH = 0x45,
    RCMD_SET_OBJ_WORD_98 = 0x46,
    RCMD_SET_OBJ_IF_CMP = 0x47,
    RCMD_DISPATCH_OBJ_MODE_CALL = 0x48,
    RCMD_REQUEST_IF_IDLE = 0x49,
    RCMD_SET_VAR_LANG = 0x4A,
    RCMD_SLOT_VALUE_BY_KIND = 0x4D,
    RCMD_SLOT_OP = 0x4E,
    RCMD_SET_SLOT_FLAG = 0x4F,
    RCMD_SLOT_FIELD = 0x50,
    RCMD_START_MOVE_LIST = 0x51,
    RCMD_SET_POSITION = 0x52,
    RCMD_PLACE_OBJ_FLAGS3 = 0x53,
    RCMD_PLACE_OBJ_SYNC = 0x54,
    RCMD_PLACE_OBJECT = 0x55,
    RCMD_PLACE_SLOT_FLAGS = 0x56,
    RCMD_SET_KEY_IGNORE_MASK = 0x57,
    RCMD_PUSH_HANDLE = 0x58,
    RCMD_LATCH_WHEN_FREE = 0x59,
    RCMD_SET_MODE_1 = 0x5A,
    RCMD_LIST_MODE = 0x5B,
};


struct OwnerObj
{
    u8 filler0[0x20];
    u8 *textEntries[2]; /* text holders made by create_text_entry */
    void *slotTable;    /* init_obj_slot_table, see setup_bg_entry */
    u8 *state;
    u8 *bgEntry;        /* the text entry setup_bg_entry gave a BG layer */
};

struct SlotTable
{
    u8 *entries;
};

struct SlotEntry
{
    void *sprite;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u8 objIndex; /* the slot object (0-11) the sprite follows while `follow` is set */
    u8 count0D;
    u8 follow;
    u8 flag0F;
};

struct SlotPos
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xA];
    u16 unkE;
};

struct PartCtl
{
    u8 filler0[0x1A0];
    u16 unk1A0;
};

struct KeyWord
{
    u16 low : 10;
    u16 high : 6;
};

struct SysKeys78
{
    u8 filler0[0x78];
    struct KeyWord keys;
};

struct OwnerFlag9A2
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct HeapOwner
{
    void *block;
};

struct WorldSlotTable
{
    u8 filler0[0x58];
    u8 *slots[8];
};

extern u8 *gWorld;
extern void (*gMemRoutines)(u32 a, void *b, u32 c);
extern const u8 gTextEntryTag[];
extern const u8 gLayerEntryTag[];
extern u8 *gCurObj;
void *init_obj_slot_table(void *block);
void flags_vars_setup_bg_layer(void *holder, u32 zero, u32 flags, void *block, u32 a4, u32 a5, void *a6, void *a7, void *a8);
extern u8 gSys[];
extern u8 *gWorkBuf;
void state_apply_hero_stats(struct OwnerObj *owner);
void slot_read_pos(void *obj, s32 idx, s16 *a, s16 *b, s16 *c);
void slot_set_pos_abs(void *obj, u32 idx, s32 x, s32 y, s32 z);
void refresh_slot_view(void *obj, s32 idx);
void slot_reload_record(void *obj, s32 idx, s32 value);
void slot_set_flag6(void *obj, s32 idx, s32 flag);
void set_var(u8 *mid, u8 *low, u8 *flagsA, u8 *flagsB, s16 index, u32 value);
void set_text_ptrs(u32 a);
void start_timer_display(void);
void voice_message_step(void);
u32 slot_find_max_key(void *unused0, s32 threshold);
u32 slot_find_min_key(void *unused0, s32 threshold);
u32 slot_widest_span(void *unused0, s32 threshold);
s32 slot_get_field(void *obj, s32 idx, s32 field);

void heap_free(void *p);
void heap_free_b(void *p);
void obj_detach_active(void *sprite);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *init_text_obj(void *mem, u32 a, u32 b, u32 c);
u32 load_u32(void *p);
void reset_obj_params_b(void *ctx, void *obj, u32 a, u32 b, u16 s5, u8 s6);
u32 restore_obj_word0(void *ctx, void *obj);
u32 cmd_reset_obj_part(struct OwnerObj *obj, u8 *state, u32 *params, u8 idx);
void set_targets_from_record(void *obj);
void change_item_count(u8 item, s16 count);
void unpack_stream_values(u8 *a, u32 *stream, u32 *p1, u32 *p2, u32 *p3);
void slot_set_value(struct OwnerObj *obj, u32 idx, s32 value, u32 flag);
void slot_set_vec(u32 idx, s32 x, s32 y, s32 z, u32 flag);
void slot_start_motion(u32 idx, u32 flag);
void set_mark_callbacks(void);
void spawn_slot_markers(void);
void set_world_flag0(u8 flag);
void update_slot_markers(void);
void release_slot_objects(void);
void release_timer_tiles(void);
extern u8 *gUnk_03000FC0;
u32 node_step(struct OwnerObj *obj, u8 *node);
u32 run_node_command(struct OwnerObj *obj, u8 *node, u32 *buf);
void decode_stream_run(u8 *rec, u32 *buf, u8 *block, u8 *p3, u8 *p4, u8 *advanced);

struct Flags110
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct NodeFlags1F
{
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct SlotObjectEntry
{
    u8 *node;
    u8 unk04[0xB];
    u8 spriteByte20; /* copy of the sprite's byte 0x20 (default 0x10) */
    u16 : 3;
    u16 spriteBits0 : 2; /* copies of the four 2-bit fields of the sprite's byte 0x1F (default 1) */
    u16 spriteBits2 : 2;
    u16 spriteBits4 : 2;
    u16 spriteBits6 : 2;
    u16 spriteAttr0E; /* copy of the sprite's halfword 0x0E (default 0x8000) */
    s32 xFix; /* position, 8.8 fixed point */
    s32 yFix;
    u8 filler1C[0x3A - 0x1C];
    u16 scale;
    u8 flags3C;
    u8 unk3D;
    u8 filler3E[0x40 - 0x3E];
};

struct SlotObjectFlags
{
    u16 : 3;
    u16 f3 : 2;
    u16 f5 : 2;
    u16 f7 : 2;
    u16 f9 : 2;
};

u32 cmd_slot_flag_20(void *obj, u32 unused, u32 *p)
{
    u8 *entry = ((struct WorldSlotTable *)gWorld)->slots[p[1]];
    u32 flag = 0;

    if (p[0] == 0)
    {
        flag = 1;
    }
    ((struct Flags110 *)(entry + 0x110))->bit5 = flag;
    return 1;
}

struct CurFlags28
{
    u8 lo2 : 2;
    u8 mid4 : 4;
    u8 hi2 : 2;
};

struct Lo7Bits
{
    u8 lo7 : 7;
    u8 bit7 : 1;
};

u32 cmd_slot_set_fields(struct OwnerObj *obj, u32 unused, u32 *p)
{
  u8 *entry = ((struct WorldSlotTable *) gWorld)->slots[*(p++)];
  long v = *(p++);
  u8 low;
  u32 hi;
  u16 *new_var;
  u32 new_var2;
  u8 *flags;
  *((u16 *) (entry + 0xD0)) = v;
  new_var2 = (0, *p);
  hi = new_var2;
  ;
  flags = entry + 0xD4;
  ((struct Lo7Bits *) flags)->lo7 = (*(p++)) & 0x7F;
  hi = (0, *p);
  *(new_var = (u16 *) (entry + 0xD2)) = hi;
  ((struct Lo7Bits *) flags)->bit7 = 1;
  return 1;
}

u32 obj_slot_set_fields(struct OwnerObj *obj, u8 *node, u32 *p)
{
    u8 *entry = ((struct WorldSlotTable *)gWorld)->slots[((u32)node[0xFD] << 28) >> 28];
    u32 v = *p++;
    u32 low;
    u32 hi;
    u8 *flags;

    *(u16 *)(entry + 0xD0) = v;
    low = *p;
    hi = low;
    flags = entry + 0xD4;
    ((struct Lo7Bits *)flags)->lo7 = *p++ & 0x7F;
    hi = *p;
    *(u16 *)(entry + 0xD2) = hi;
    ((struct Lo7Bits *)flags)->bit7 = 1;
    return 1;
}

u32 cmd_call_cf0(void *obj, u32 unused, s32 *p)
{
    u8 *world = gWorld;
    u8 *cur = *(u8 **)(world + 0x7C);

    if (cur != 0)
    {
        s32 idx = p[0];
        u8 *rec;

        if (idx < 0)
        {
            rec = cur;
        }
        else
        {
            rec = ((struct WorldSlotTable *)world)->slots[idx];
        }
        if (*(u32 *)(rec + 0x54) != 0)
        {
            set_targets_from_record(rec + 8);
        }
    }
    return 1;
}

u32 cmd_obj_chain(struct OwnerObj *obj, u8 *dst, u32 *p)
{
    switch (p[0])
    {
    case 0:
    {
        u8 *node = ((struct WorldSlotTable *)gWorld)->slots[((u32)dst[0xFD] << 28) >> 28] + 8;
        register u16 *flags asm("r2");
        register u16 oldFlags asm("r1");
        register u32 bitv asm("r3");
        register u32 result asm("r0");

        while (node != 0)
        {
            if (*(u32 *)(node + 0x4C) != 0)
                break;
            node = *(u8 **)node;
        }
        if (node == 0)
            return 1;
        flags = (u16 *)(dst + 0xA0);
        oldFlags = *flags;
        bitv = 0x200;
        result = bitv | oldFlags;
        *flags = result;
        break;
    }
    case 1:
    {
        u8 *slot = ((struct WorldSlotTable *)gWorld)->slots[((u32)dst[0xFD] << 28) >> 28];

        if (*(u32 *)(slot + 0x54) != 0)
        {
            *(u32 *)dst = *(u32 *)(dst + 0x14);
            break;
        }
        return 1;
    }
    }
    return 0;
}

u32 cmd_world_flag_2b4(void *obj, u32 unused, u32 *p)
{
    u8 *world = gWorld;
    u32 v = p[0];

    *(u16 *)(world + 0x2B4) = v;
    world[0x2C1] |= 8;
    return 1;
}

u32 cmd_key_word(void *obj, u32 unused, u32 *p)
{
    struct SysKeys78 *sys = (struct SysKeys78 *)gSys;
    u16 low = ~*p & 0x3FF;

    sys->keys.low = low;
    return 1;
}

u32 cmd_swap_slots(void *obj, u32 unused, u32 *p)
{
    u8 *world = gWorld;
    u32 ia = p[0];
    u32 off = ia << 2;
    u8 **table = (u8 **)(world + 0x58);
    u8 **slot_a = (u8 **)((u8 *)table + off);
    u8 *old_a = *slot_a;
    u8 **slot_b = table + p[1];
    u8 *old_b = *slot_b;

    *slot_a = old_b;
    *slot_b = old_a;
    (*slot_a)[0xF4] = ia;
    ((struct WorldSlotTable *)gWorld)->slots[p[1]][0xF4] = p[1];
    return 1;
}

struct WorkRec
{
    u8 filler0[0xD0];
};

struct SlotIdxLo4
{
    u8 filler0[0xFD];
    u8 lo4 : 4;
};

struct WorldFlagsB
{
    u8 filler0[0xB];
    u8 bit0 : 1;
    u8 bit1 : 1;
};

struct WorldBit3
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

struct WorldBit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

NONMATCH("asm/code_080F8900/world_cmds_cmd_world_mode.s", u32 world_cmds_cmd_world_mode(void *obj, u32 unused, u32 *p))
{
    switch (*p++)
    {
    case 0:
        if (p[0] != 0)
        {
            spawn_slot_markers();
        }
        else
        {
            set_mark_callbacks();
        }
        break;
    case 1:
        set_world_flag0((u8)p[0]);
        break;
    case 2:
        ((struct WorldBit3 *)(gWorld + 0xB))->bit3 = (p[0] == 0);
        if (p[0] != 0)
        {
            update_slot_markers();
        }
        else
        {
            release_slot_objects();
        }
        break;
    case 3:
        if (p[0] != 0)
        {
            start_timer_display();
        }
        else
        {
            release_timer_tiles();
        }
        break;
    case 4:
    {
        u8 *world = gWorld;
        u32 v = p[0];

        ((struct WorldBit4 *)(world + 0x2BF))->bit4 = v & 1;
        break;
    }
    }
    return 1;
}
END_NONMATCH

struct EffectMode
{
    u8 filler0[5];
    u8 mode : 2;
};

u32 cmd_effect_mode(void *obj, u32 unused, u32 *p)
{
    u8 *effect = *(u8 **)(gWorld + 0x2D8);
    u32 op = *p;
    u32 mode;

    if ((((u8)op ^ 1) & 1) != 0)
    {
        u32 bit = op & 2;

        mode = 2;
        if (bit != 0)
        {
            mode = 1;
        }
    }
    else
    {
        mode = 0;
    }
    ((struct EffectMode *)effect)->mode = mode;
    *(void **)(gWorld + 0x24) = state_apply_hero_stats;
    return 1;
}

u32 cmd_update_and_store(u8 *dst, u32 arg, u32 *p, u32 arg3);

u8 cmd_reward_or_update(struct OwnerObj *obj, u32 arg, u32 *p, u32 arg3)
{
    if (p[1] == 0)
    {
        if ((s32)p[2] >> 7 == 4)
        {
            p[2] &= 0x7F;
            if (p[3] != 0)
            {
                p[4] = -p[4];
            }
            change_item_count(((u8 *)p)[8], *(s16 *)((u8 *)p + 0x10));
            return 1;
        }
    }
    return cmd_update_and_store((u8 *)obj + 0x1C, arg, p, arg3);
}

u32 cmd_find_slot_set_var(struct OwnerObj *obj, u8 *var, s16 *p, u32 ctx)
{
    u32 value;

    switch (*(s32 *)(p + 2))
    {
    case 0:
        value = slot_find_max_key(obj, *(s32 *)(p + 4));
        break;
    case 1:
        value = slot_find_min_key(obj, *(s32 *)(p + 4));
        break;
    case 2:
        value = slot_widest_span(obj, *(s32 *)(p + 4));
        break;
    }
    set_var((u8 *)ctx, var + 0x18, obj->state + 0x1794, var + 0xA8, p[0], value);
    return 1;
}

u32 cmd_slot_get_set_var(struct OwnerObj *obj, u8 *var, s16 *p, u32 ctx)
{
    u32 value = slot_get_field(obj, p[2], *(s32 *)(p + 4));

    set_var((u8 *)ctx, var + 0x18, obj->state + 0x1794, var + 0xA8, p[0], value);
    return 1;
}

u32 cmd_flag_op(struct OwnerObj *obj, u32 *dst, u32 *p)
{
    switch ((s32)*p++)
    {
    case 0:
    {
        u8 *world = gWorld;

        if (world[0x2F8] == 0)
        {
            *(u32 *)(world + 0x2F4) |= p[0];
        }
        break;
    }
    case 1:
        if ((*(u32 *)(obj->state + 0x1788) & p[0]) == 0)
        {
            dst[0] = dst[5];
            return 0;
        }
        break;
    case 2:
        set_text_ptrs(p[0]);
        if (*(u32 *)(gWorld + 0x290) != 0)
        {
            voice_message_step();
        }
        else
        {
            start_timer_display();
        }
        break;
    }
    return 1;
}

u32 cmd_world_flag_2f8(void *obj, u32 unused, u32 *p)
{
    u8 *dst;
    u8 val;

    switch (p[0])
    {
    case 0:
        dst = gWorld + 0x2F8;
        val = 1;
        break;
    case 1:
        dst = gWorld + 0x2F8;
        val = 0;
        break;
    default:
        return 1;
    }
    *dst = val;
    return 1;
}

u32 cmd_slot_refresh(struct OwnerObj *obj, u32 unused, s16 *p)
{
    u8 *entry;
    register s32 idx asm("r4");

    refresh_slot_view(obj, p[0]);
    slot_reload_record(obj, p[0], p[2]);
    slot_set_flag6(obj, p[0], ((s8 *)p)[8]);
    idx = *(s32 *)p;
    if (idx < 8)
    {
        struct WorldSlotTable *world = (struct WorldSlotTable *)gWorld;
        register u8 *offset asm("r1") = (u8 *)(idx * sizeof(u8 *));
        register u32 slots asm("r0");
        slots = (u32)world->slots;
        slots += (u32)offset;
        entry = *(u8 **)slots + 8;
    }
    else
    {
        register u32 offset asm("r0");
        u8 **work = (u8 **)&gWorkBuf;
        offset = sizeof(struct WorkRec) * idx;
        offset += (u32)*work;
        entry = (u8 *)offset + 0x300;
    }
    ((u8 *)*(u8 **)(entry + 8))[0x12] |= 0x10;
    return 1;
}

u32 cmd_slot_shift_pos(struct OwnerObj *obj, u32 unused, s32 *p)
{
    u8 base = obj->state[0x179C];
    u8 idx;
    s16 x, y, z;

    if (base < 7)
    {
        idx = base - 1;
    }
    else
    {
        idx = base - 7;
    }
    slot_read_pos(obj, (s16)p[0], &x, &y, &z);
    slot_set_pos_abs(obj, idx, p[1] + x, p[2] + y, p[3] + z);
    return 0;
}

u32 cmd_slot_resync_pos(struct OwnerObj *obj, u32 unused, s16 *p)
{
    u8 base = obj->state[0x179C];
    u8 idx;
    s16 x, y, z;

    if (base < 7)
    {
        idx = base - 1;
    }
    else
    {
        idx = base - 7;
    }
    slot_read_pos(obj, p[0], &x, &y, &z);
    slot_set_pos_abs(obj, idx, x, y, z);
    return 0;
}

u32 cmd_slot_flag_copy(struct OwnerObj *obj, u32 *dst, u32 *p)
{
    u32 zero = 0;
    register s32 idx asm("r3") = p[0];
    u8 *entry;

    if (idx < 8)
    {
        struct WorldSlotTable *world = (struct WorldSlotTable *)gWorld;
        register u32 offset asm("r1") = idx * sizeof(u8 *);
        register u32 slots asm("r0") = (u32)world->slots;
        slots += offset;
        entry = *(u8 **)slots + 8;
    }
    else
    {
        register u32 offset asm("r0");
        u8 **work = (u8 **)&gWorkBuf;
        offset = sizeof(struct WorkRec) * idx;
        offset += (u32)*work;
        entry = (u8 *)offset + 0x300;
    }
    if (*(u32 *)(entry + 0x94) == 0)
        zero = 1;
    if (zero != p[1])
        *dst = p[2];
    return 1;
}

u32 cmd_slot_set_stream(struct OwnerObj *obj, u8 *node, u32 *p)
{
    s32 bit = 2;
    u8 base = obj->state[0x179C];
    u8 idx;
    u32 x;
    u32 y;
    u32 z;
    u32 mode;
    s32 value;

    if (base < 7)
    {
        idx = base - 1;
    }
    else
    {
        idx = base - 7;
    }
    unpack_stream_values((u8 *)obj + 0x1C, p, &x, &y, &z);
    value = p[4];
    mode = 0;
    if ((p[3] & 3) == 1)
    {
        mode = 1;
    }
    slot_set_value(obj, idx, value, mode);
    slot_set_vec(idx, x, y, z, 0);
    slot_start_motion(idx, 1);
    *(u16 *)(node + 0xA0) |= bit;
    return 0;
}

u32 cmd_wait_flag_copy(struct OwnerObj *obj, u32 *dst, u32 *p, u32 flag)
{
    switch ((s32)*p)
    {
    case 0:
        flag = *(u32 *)(gWorld + 0x40);
        break;
    case 1:
    {
        struct WorldFlagsB *bits = (struct WorldFlagsB *)gWorld;

        flag = bits->bit1;
        bits->bit1 = 0;
        break;
    }
    case 2:
        flag = ((struct WorldFlagsB *)gWorld)->bit0;
        break;
    }
    if (flag != 0)
    {
        dst[0] = dst[5];
        return 0;
    }
    return 1;
}

u32 cmd_owner_flag_copy(struct OwnerObj *obj, u32 *dst, u32 *p)
{
    u32 sel = (u8)(*p++ + 1);
    u32 flag = *p++;
    u32 bit;

    if (flag == 0)
    {
        u8 *part = obj->state;
        u32 x;

        part += sel << 8;
        x = *(u16 *)(part + 0xA0) & 0xFF7;
        bit = ((-x) | x) >> 31;
    }
    if (bit != *p)
    {
        return 1;
    }
    *dst = p[1];
    return 1;
}

u8 cmd_restore_word0(void *obj, void *arg, u32 *p)
{
    if (p[0] != 0)
    {
        gWorld[0x2BF] |= 2;
    }
    return restore_obj_word0((u8 *)obj + 0x1C, arg);
}

u32 cmd_owner_set_var(struct OwnerObj *obj, u8 *var, u32 *p, u32 ctx)
{
    s32 varBase = 0x100;
    u8 *part;
    u8 *owner;

    switch (p[1])
    {
    case 0:
        part = obj->state;
        owner = part;
        break;
    case 1:
    {
        u32 off = (p[2] << 8) + varBase;
        u8 *st = obj->state;

        part = st + off;
        owner = st;
        break;
    }
    default:
        owner = obj->state;
        break;
    }
    set_var((u8 *)ctx, var + 0x18, owner + 0x1794, var + 0xA8, *(s16 *)p, *(u16 *)(part + 0xA0) & 1);
    return 1;
}

u32 cmd_owner_part_copy(struct OwnerObj *obj, u32 *dst, u32 *p)
{
    u32 sel = *p++;
    u8 *part;

    switch (sel)
    {
    case 0:
        part = obj->state;
        break;
    case 1:
        part = obj->state + ((p[1] << 8) + 0x100);
        break;
    }
    if (*(u16 *)(part + 0xA0) & 1)
    {
        *dst = *p;
    }
    return 1;
}

void unpack_stream_values(u8 *a, u32 *stream, u32 *p1, u32 *p2, u32 *p3);
void slot_set_value(struct OwnerObj *obj, u32 idx, s32 value, u32 flag);
void slot_set_vec(u32 idx, s32 x, s32 y, s32 z, u32 flag);
void slot_start_motion(u32 idx, u32 flag);
u32 cmd_owner_part_reset(struct OwnerObj *obj, u32 unused, u32 *p)
{
    u8 *part;
    s16 count;

    switch (*p++)
    {
    case 0:
        part = obj->state;
        break;
    case 1:
        part = obj->state + (((*p + 1) << 24) >> 16);
        break;
    default:
        part = (u8 *)unused;
        break;
    }
    *(u32 *)(part + 4) = *(u32 *)part;
    *(u16 *)(part + 0xA0) = 1;
    for (count = 0x17; count > 0; count--)
    {
    }
    return 1;
}

u32 cmd_reset_obj_part(struct OwnerObj *obj, u8 *state, u32 *params, u8 idx)
{
    u8 *part;

    switch (*params++)
    {
    case 0:
        part = obj->state;
        reset_obj_params_b((u8 *)obj + 0x1C, part, *params, 0, 1, 0xFF);
        break;
    case 1:
        if (idx <= 5)
        {
            part = obj->state + ((idx << 8) + 0x100);
            if (((part[0xA0] ^ 1) & 1) != 0)
            {
                reset_obj_params_b((u8 *)obj + 0x1C, part, *params, 0, 1, 0xFF);
                ((struct SlotIdxLo4 *)part)->lo4 = idx;
            }
        }
        break;
    }
    return 1;
}

u32 cmd_part_shift(struct OwnerObj *obj, u32 unused, u32 *p)
{
    u8 *part = obj->state + (p[0] << 8);
    u8 *base = part + 0x700;

    *(u16 *)(part + 0x7A0) = 0;
    *(u32 *)base = *(u32 *)(base + 4);
    return 1;
}

u32 cmd_find_part_copy(struct OwnerObj *obj, u32 *dst, u32 *p, u8 key)
{
    u8 *part = obj->state + 0x700;
    u32 start = 16;
    s16 remaining;

    if (p[0] == 0)
    {
        part += p[1] * 0x100;
        start = 1;
    }
    remaining = start;
    if (remaining != 0)
    {
        do
        {
            if ((*(u16 *)(part + 0xA0) & 1) != 0 && part[0xA4] == key)
            {
                dst[0] = dst[5];
                return 0;
            }
            remaining--;
            part += 0x100;
        } while (remaining > 0);
    }
    return 1;
}

u32 slot_find_max_key(void *unused0, s32 threshold);
u32 slot_find_min_key(void *unused0, s32 threshold);
u32 slot_widest_span(void *unused0, s32 threshold);
s32 slot_get_field(void *obj, s32 idx, s32 field);
u32 cmd_part_flag4(struct OwnerObj *obj, u32 unused, u32 *p)
{
    u16 flagBit = 4;
    u8 *part = obj->state + (p[1] << 8);

    u16 *flags;

    switch (p[0])
    {
    case 0:
        flags = (u16 *)(part + 0x7A0);
        *flags |= flagBit;
        break;
    case 1:
        flags = (u16 *)(part + 0x7A0);
        *flags &= 0xFFFB;
        break;
    default:
        return 1;
    }
    return 1;
}

u32 cmd_node_stream(struct OwnerObj *obj, u8 *node)
{
    u32 buf[13];

    if (((node[0xA0] ^ 1) & 1) != 0)
    {
        return 0;
    }
    while ((u8)node_step(obj, node) == 0)
    {
        decode_stream_run(node, buf, gUnk_03000FC0, obj->state + 0x1700, obj->state + 0x1794, node + 0xA8);
        if ((u8)run_node_command(obj, node, buf) == 0)
        {
            return 1;
        }
    }
    return 1;
}

void cmd_slot_clear_flag2(struct OwnerObj *obj, u8 *node, u8 idx)
{
    u8 *entry;
    register u32 slot asm("r2") = idx;

    if (slot < 8)
    {
        struct WorldSlotTable *world = (struct WorldSlotTable *)gWorld;
        register u32 offset asm("r1") = slot * sizeof(u8 *);
        register u32 slots asm("r0") = (u32)world->slots;
        slots += offset;
        entry = *(u8 **)slots + 8;
    }
    else
    {
        register u32 offset asm("r0");
        u8 **work = (u8 **)&gWorkBuf;
        offset = sizeof(struct WorkRec) * slot;
        offset += (u32)*work;
        entry = (u8 *)offset + 0x300;
    }
    if (*(u32 *)(entry + 0x80) == 0)
        *(u16 *)(node + 0xA0) &= 0xFFFD;
}

static inline s32 part_scan_stop(u8 *part, u8 *node, s32 b, s32 cnt)
{
    u16 flags;
    s32 nb;
    s32 pb;

    if (part == node)
    {
        return 0;
    }
    flags = *(u16 *)(part + 0xA0);
    if ((flags & 0x1000) != 0)
    {
        nb = node[0xFE] & 7;
        pb = part[0xFE] & 7;
        if (b > cnt)
        {
            return pb < nb + 1;
        }
        return pb < nb;
    }
    return (flags & 0xFF7) != 0;
}

ASM_FUNC("asm/code_080F8900/cmd_part_scan.s", void cmd_part_scan(struct OwnerObj *obj, u8 *node, u8 idx))

void clear_node_bit9(struct OwnerObj *obj, u8 *node)
{
    if (*(u32 *)(obj->state + 0x1788) & 0x4000)
    {
        *(u16 *)(node + 0xA0) &= 0xFDFF;
    }
}

void slot_clear_flag800_if_idle(struct OwnerObj *obj, u8 *node)
{
    u32 idx = node[0xFD] >> 4;
    u8 *state = obj->state;

    if ((*(u16 *)((idx << 8) + (u32)state + 0x7A0) & 0x20) == 0)
    {
        *(u16 *)(node + 0xA0) &= 0xF7FF;
    }
}

/* Gives the owner's text entry idx (see create_text_entry) a BG layer (layer, pal, gfx, bgArg,
 * mode 2, state at state+0x1790) and makes it the owner's bgEntry, after setting gCurObj's
 * halfword 0x20 (curWord20) and the 4-bit field at 0x28 (curMid4). Then gives the owner a fresh
 * slot table. */
void setup_bg_entry(struct OwnerObj *obj, u8 idx, u8 layer, u8 pal, u32 gfx, u16 bgArg, u16 curWord20, u8 curMid4)
{
    u8 *state = obj->state;
    u8 *cur = gCurObj;
    u8 **entries;
    u8 *entry;
    u8 *mem;
    u32 zero;
    s32 off;

    zero = 0;
    *(u16 *)(cur + 0x20) = curWord20;
    ((struct CurFlags28 *)(cur + 0x28))->mid4 = curMid4;
    off = idx << 2;
    entries = obj->textEntries;
    entry = *(u8 **)((u8 *)entries + off);
    obj->bgEntry = entry;
    flags_vars_setup_bg_layer(entry, layer, pal, (void *)gfx, 2, bgArg, state + 0x1790, (void *)zero, (void *)zero);
    mem = heap_alloc2(4, 0, gLayerEntryTag, 0);
    obj->slotTable = init_obj_slot_table(mem);
}

void set_mark_callbacks(void);
void spawn_slot_markers(void);
void set_world_flag0(u8 flag);
void update_slot_markers(void);
void release_slot_objects(void);
void release_timer_tiles(void);
u32 create_text_entry(u8 *owner, u8 idx, u32 a, u32 b, u32 c)
{
    u32 *slot;
    void *text;
    u32 off = (u32)idx << 2;

    owner += 0x20;
    slot = (u32 *)(owner + off);

    text = heap_alloc2(8, 0, gTextEntryTag, 0);
    text = init_text_obj(text, a, b, c);
    *slot = (u32)text;
    return load_u32(text);
}

u16 read_entry_half_1a0(struct OwnerObj *obj, u8 idx)
{
    return *(u16 *)(obj->state + ((u32)idx << 8) + 0x1A0);
}

void unknown_thumb_080f95f2(struct OwnerObj *obj, u8 value)
{
    *(u32 *)(obj->state + 0x1784) = value;
}

void sub_080F9608(struct OwnerObj *obj, u8 value)
{
    *(u32 *)(obj->state + 0x177C) = value;
}

void set_state_word_1780(struct OwnerObj *obj, u8 value)
{
    *(u32 *)(obj->state + 0x1780) = value;
}

void unknown_thumb_080f962c(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) |= mask;
    *(u32 *)(obj->state + 0x1798) |= mask;
}

void owner_clear_flags_1788(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) &= ~mask;
}

void owner_set_flags_1788(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) |= mask;
}

NONMATCH("asm/code_080F8900/sub_080F9674.s", void sub_080F9674(struct OwnerObj *obj, u8 idx))
{
    u32 off = (u32)idx << 8;
    u32 size = 0x100;
    u8 *st;

    st = obj->state;
    gMemRoutines(0, st + (off + size), size);
    off += 0x700;
    gMemRoutines(0, obj->state + off, size);
}
END_NONMATCH

void reset_entry_part(struct OwnerObj *obj, u8 idx, u32 a)
{
    u32 params[2];
    u8 *state;
    struct PartCtl *part;

    params[0] = 1;
    params[1] = a;
    state = obj->state;
    part = (struct PartCtl *)(((u32)idx << 8) + (u32)state);
    part->unk1A0 = 0;
    cmd_reset_obj_part(obj, obj->state, params, idx);
}

void reset_root_params(struct OwnerObj *obj, u32 a)
{
    reset_obj_params_b((u8 *)obj + 0x1C, obj->state, a, 0, 1, 0xFF);
}

void unknown_thumb_080f970e(struct SlotTable *table, u8 idx)
{
    u32 off = (u8)idx << 4;
    u8 *base = table->entries;
    struct SlotPos *obj;
    struct SlotEntry *entry;
    u32 sum1;

    base += off;
    entry = (struct SlotEntry *)base;
    obj = entry->sprite;
    obj->unk0 = entry->unk4 + entry->unk8;
    sum1 = entry->unkA;
    base = (u8 *)(u32)entry->unk6;
    sum1 += (u32)base;
    obj->unk2 = sum1;
    obj->unkE = 0x7FFF - sum1;
}

u32 slot_countdown_step(struct SlotTable *table, u8 idx)
{
    u32 off = (u32)idx << 4;
    u32 n;
    u8 count;

    if (((struct SlotEntry *)(table->entries + off))->flag0F == 0)
    {
        return 0;
    }
    n = ((struct SlotEntry *)(table->entries + off))->count0D;
    if (n == 0)
    {
        return 1;
    }
    n = n - 1;
    ((struct SlotEntry *)(table->entries + off))->count0D = n;
    count = n;
    if (count != 0)
    {
        return 1;
    }
    {
        struct SlotEntry *entry = (struct SlotEntry *)(table->entries + off);

        obj_detach_active(entry->sprite);
        entry->flag0F = count;
    }
    return 0;
}

void slot_detach_entry(struct SlotTable *table, u8 idx)
{
    struct SlotEntry *entry = (struct SlotEntry *)(table->entries + ((u32)idx << 4));

    obj_detach_active(entry->sprite);
    entry->flag0F = 0;
}

void heap_free_owner(struct HeapOwner *obj, u32 flags)
{
    heap_free(obj->block);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct Owner;
struct ScriptThread;
void owner_tick_records(struct Owner *owner);
void release_obj_131c(void *p);
void tick_obj_timer(void *ctx, struct ScriptThread *thread);
void toggle_on_motion_done(struct Owner *owner, struct ScriptThread *thread, u8 slot);
void tick_slot_timer(struct Owner *owner, struct ScriptThread *thread, u8 slot);
// Flags of a script thread (+0xA0). run_record runs a thread only while THREAD_ACTIVE is set;
// owner_tick_records counts the waits down.
enum ScriptThreadFlags
{
    THREAD_ACTIVE = 0x01,
    THREAD_WAIT_TWEEN = 0x02,      /* until its slot object's tween ends (toggle_on_motion_done) */
    THREAD_PAUSED = 0x04,
    THREAD_RESUME_CALLER = 0x08,   /* unpause thread `caller` when the script ends (record_advance) */
    THREAD_WAIT_TIMER = 0x10,      /* until waitTimer runs out (tick_obj_timer) */
    THREAD_WAIT_SLOT_TIMER = 0x20, /* until its slot object's animation timer ends (tick_slot_timer) */
    THREAD_UNK80 = 0x80,           /* like THREAD_PAUSED, stops a slot thread's waits counting down */
};

struct ScriptThread
{
    u32 pc;      /* next command */
    u32 end;     /* end of the script */
    u32 nextPc;  /* script queued to run once this one ends (0: none) */
    u32 nextEnd;
    u8 filler10[0x9C - 0x10];
    u32 waitTimer; /* frames left of a THREAD_WAIT_TIMER or THREAD_WAIT_SLOT_TIMER wait */
    u16 flags;     /* enum ScriptThreadFlags */
    u16 nextFlags; /* flags of the queued script */
    u8 caller;     /* thread to unpause, with THREAD_RESUME_CALLER */
    u8 nextCaller;
    u8 fillerA6[0xA8 - 0xA6];
};

// A script owner's state: thread 0 runs the main script, threads 1-12 one slot object each.
struct OwnerState
{
    struct ScriptThread threads[13];
    u8 filler888[0x908 - 0x888];
    u8 vars[0x994 - 0x908]; /* script variables (rd_vars) */
    u32 curSlot; /* slot of the thread being run (a script variable) */
    u8 bgArg998[4]; /* given to flags_vars_setup_bg_layer by owner_init_entry */
    u8 params[4];   /* rd_params */
    u8 cur9A0;   /* thread being run */
    u8 mode9A1;
    u8 busy9A2;
    u8 flags9A3; /* bit 0: the owner keeps its own 12 slot objects */
};

struct TextLayer
{
    u8 filler0[0x1B2];
    u8 busy; /* nonzero while text_layer_step has work to do */
};

struct TextEntry
{
    u32 unk0;
    struct TextLayer *layer;
};

struct ObjList
{
    u8 filler0[0x158];
    struct SlotObjectEntry *slots; /* 12 slot objects */
};

// A script owner (struct Owner in code_080FB4E0.c): the object list, the slot table of the
// text box's sprites (aux), the script state and the text entry.
struct Owner
{
    u8 filler0[0x14];
    struct ObjList *list;
    u32 type;
    u8 unk1C[0x20 - 0x1C]; /* handed to tick_obj_timer, which ignores it */
    struct SlotTable *aux;
    struct OwnerState *state;
    struct TextEntry *entry;
};

u32 run_record(struct Owner *owner, struct ScriptThread *thread);
u8 text_layer_step(u8 *owner);
u32 slot_countdown(struct SlotTable *table, u8 idx);
void update_slot_bounds(struct SlotTable *table, u8 idx);
// Runs one frame of a script owner: counts its threads' waits down, runs the main thread unless it
// is paused or waiting on a timer, then every slot thread that isn't paused or waiting (curSlot tells
// the script whose it is), and steps the text layer. When the owner keeps its own slot objects, each
// of the 4 text box sprites that follows one is moved onto it (raised by its unk3D).
void owner_step(struct Owner *owner)
{
    struct OwnerState *state = owner->state;
    u8 *cur = &state->cur9A0;
    struct ScriptThread *thread;
    struct ScriptThread *end;
    struct SlotEntry *entry;
    s16 i;

    ((struct OwnerFlag9A2 *)&state->busy9A2)->b0 = 0;
    owner_tick_records(owner);
    *cur = 0;
    thread = state->threads;
    if ((thread->flags & (THREAD_PAUSED | THREAD_WAIT_TIMER)) == 0)
    {
        run_record(owner, thread);
    }
    *cur = *cur + 1;
    thread++;
    end = thread + 12;
    while (thread != end)
    {
        if ((thread->flags & (THREAD_WAIT_TWEEN | THREAD_PAUSED | THREAD_WAIT_TIMER | THREAD_WAIT_SLOT_TIMER)) == 0)
        {
            owner->state->curSlot = *cur - 1;
            run_record(owner, thread);
        }
        thread++;
        *cur = *cur + 1;
    }
    if (owner->entry->layer->busy != 0)
    {
        owner->entry->layer->busy = text_layer_step((u8 *)owner->entry);
    }
    if ((owner->state->flags9A3 & 1) != 0)
    {
        i = 0;
        entry = (struct SlotEntry *)owner->aux->entries;
        do
        {
            if ((u8)slot_countdown(owner->aux, (u8)i) != 0)
            {
                if (entry->follow != 0)
                {
                    struct SlotObjectEntry *obj = &owner->list->slots[entry->objIndex];
                    s32 v = obj->xFix;
                    if (v < 0)
                    {
                        v += 0xFF;
                    }
                    entry->unk8 = v >> 8;
                    v = obj->yFix;
                    if (v < 0)
                    {
                        v += 0xFF;
                    }
                    entry->unkA = (v >> 8) - obj->unk3D;
                }
                update_slot_bounds(owner->aux, (u8)i);
            }
            i++;
            entry++;
        } while (i < 4);
    }
}

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f98d8.s", void unknown_thumb_080f98d8(void))

extern u8 *gCurObj;
extern const u8 gOwnerEntryTag[];
extern const u8 gOwnerAuxTag[];
void *init_slot_table(void *state);
void flags_vars_setup_bg_layer(void *holder, u32 zero, u32 flags, void *block, u32 a4, u32 a5, void *a6, void *a7, void *a8);

struct Bit1Byte
{
    u8 lo1 : 1;
    u8 bit1 : 1;
    u8 hi6 : 6;
};

struct Bit0Byte
{
    u8 bit0 : 1;
    u8 rest : 7;
};

/* Sets up the owner's text entry: a text holder (VRAM destination vramAddr, textSize bytes, mode 0xA)
 * at owner+0x28 with its BG layer (layer, pal, gfx), and a fresh slot table at owner+0x20. Unless
 * keep_slots is set, the state is marked fresh and the 12 0x40-byte slots are reset (cleared, scale
 * 0x100, enabled). Returns the holder's record chain. */
u32 owner_init_entry(u8 *owner, u8 layer, u8 pal, u32 vramAddr, u32 textSize, u32 gfx, u16 bgArg, u32 curWord20, u8 curMid4, u8 keep_slots)
{
    u8 *state = *(u8 **)(owner + 0x24);
    u8 *cur = gCurObj;
    u32 zero;
    s32 fresh;
    u8 *holder;

    zero = 0;
    *(u16 *)(cur + 0x20) = curWord20;
    ((struct CurFlags28 *)(cur + 0x28))->mid4 = curMid4;
    holder = init_text_obj(heap_alloc2(8, 0, gOwnerEntryTag, 0), vramAddr, textSize, 0xA);
    *(u8 **)(owner + 0x28) = holder;
    flags_vars_setup_bg_layer(holder, layer, pal, (void *)gfx, 2, bgArg, state + 0x998, (void *)zero, (void *)zero);
    ((struct Bit1Byte *)(*(u8 **)(*(u8 **)(owner + 0x28) + 4) + 0x1B8))->bit1 = 0;
    *(void **)(owner + 0x20) = init_slot_table(heap_alloc2(4, 0, gOwnerAuxTag, 0));
    {
        u8 *st = *(u8 **)(owner + 0x24);

        fresh = 0;
        if (keep_slots == 0)
        {
            fresh = 1;
        }
        ((struct Bit0Byte *)(st + 0x9A3))->bit0 = fresh;
    }
    if (keep_slots == 0)
    {
        u8 *slot;
        s16 count;
        u32 zeroWord;
        s32 scale;

        count = 12;
        slot = *(u8 **)(*(u8 **)(owner + 0x14) + 0x158);
        zeroWord = 0;
        scale = 0x100;
        for (; count > 0; count--, slot += 0x40)
        {
            *(u32 *)slot = zeroWord;
            *(u16 *)(slot + 0x3A) = scale;
            ((struct Bit0Byte *)(slot + 0x3C))->bit0 = 1;
        }
    }
    return load_u32(*(u8 **)(owner + 0x28));
}

/* Counts a script owner's thread waits down by one frame: the main thread's timer, then for each of
 * the 12 slot threads that isn't paused, its tween, slot animation timer and timer waits. */
void owner_tick_records(struct Owner *owner)
{
    s16 i;
    struct ScriptThread *thread;
    u16 *flags;

    i = 1;
    thread = owner->state->threads;
    do
    {
        if ((thread->flags & THREAD_WAIT_TIMER) != 0)
        {
            tick_obj_timer(owner->unk1C, thread);
        }
        i--;
        thread++;
    } while (i > 0);
    i = 0;
    flags = &thread->flags;
    do
    {
        if ((*flags & (THREAD_UNK80 | THREAD_PAUSED)) == 0)
        {
            if ((*flags & THREAD_WAIT_TWEEN) != 0)
            {
                toggle_on_motion_done(owner, thread, i);
            }
            if ((*flags & THREAD_WAIT_SLOT_TIMER) != 0)
            {
                tick_slot_timer(owner, thread, i);
            }
            if ((*flags & THREAD_WAIT_TIMER) != 0)
            {
                tick_obj_timer(owner->unk1C, thread);
            }
        }
        i++;
        flags += sizeof(struct ScriptThread) / sizeof(u16);
        thread++;
    } while (i <= 11);
}

/* Resets a script owner: when it keeps its own 12 slot objects, destroys each one's sprite node
 * (release_obj_131c) and resets the slot (scale 0x100, enabled); then clears the params and the
 * script variables (through curSlot and bgArg998) and the mode. */
void owner_reset(struct Owner *owner)
{
    u8 *slot;
    s16 count;
    u16 scale;

    if (owner->state->flags9A3 & 1)
    {
        count = 12;
        slot = (u8 *)owner->list->slots;
        do
        {
            if (*(u32 *)slot != 0)
            {
                release_obj_131c(*(void **)slot);
                *(u32 *)slot = 0;
                scale = 0x100;
                *(u16 *)(slot + 0x3A) = scale;
                slot[0x3C] |= 1;
            }
            slot += 0x40;
            count--;
        } while (count > 0);
    }
    gMemRoutines(0, owner->state->params, sizeof(owner->state->params));
    gMemRoutines(0, owner->state->vars, 0x99C - 0x908);
    owner->state->mode9A1 = 0;
}

/* Saves the settings of a slot's sprite (halfword 0x0E, byte 0x20 and the four 2-bit fields of byte
 * 0x1F) in its slot entry, or the defaults when the slot has no sprite yet, so that they survive the
 * sprite being set up again; slot_store_object writes them back. */
void slot_load_object(struct Owner *owner, u32 index)
{
    u32 shifted = index << 24;
    struct SlotObjectEntry **slots = &owner->list->slots;
    struct SlotObjectEntry *entry = (struct SlotObjectEntry *)((u8 *)*slots + (shifted >> 18));
    u8 *sprite = entry->node;

    if (sprite != NULL)
    {
        entry->spriteAttr0E = *(u16 *)(sprite + 0xE);
        entry->spriteByte20 = sprite[0x20];
        entry->spriteBits0 = ((u32)sprite[0x1F] << 30) >> 30;
        entry->spriteBits2 = ((u32)sprite[0x1F] << 28) >> 30;
        entry->spriteBits4 = ((u32)sprite[0x1F] << 26) >> 30;
        entry->spriteBits6 = (u32)sprite[0x1F] >> 6;
    }
    else
    {
        entry->spriteAttr0E = 0x8000;
        entry->spriteByte20 = 0x10;
        entry->spriteBits6 = 1;
        entry->spriteBits4 = 1;
        entry->spriteBits2 = 1;
        entry->spriteBits0 = 1;
    }
}

void slot_store_object(void *owner, u32 idx)
{
    u32 t = idx << 24;
    u8 **table = (u8 **)(*(u8 **)((u8 *)owner + 0x14) + 0x158);
    u8 *entry = *table + (t >> 18);
    u8 *node = *(u8 **)entry;

    *(u16 *)(node + 0xE) = *(u16 *)(entry + 0x12);
    {
        u32 a = entry[0xF];
        node[0x20] = a;
    }
    ((struct NodeFlags1F *)(node + 0x1F))->f0 = ((u32)entry[0x10] << 27) >> 30;
    ((struct NodeFlags1F *)(node + 0x1F))->f2 = ((u32)entry[0x10] << 25) >> 30;
    ((struct NodeFlags1F *)(node + 0x1F))->f4 = ((u32)*(u16 *)(entry + 0x10) << 23) >> 30;
    ((struct NodeFlags1F *)(node + 0x1F))->f6 = ((u32)entry[0x11] << 29) >> 30;
}

/* Called for a thread that may have reached the end of its script. Returns 0 while it hasn't, or when
 * a queued script was started in its place (the thread waits on nothing else). Otherwise the thread
 * stops (flags 0, unless it still waits on something) and 1 is returned. At the end it unpauses its
 * caller if THREAD_RESUME_CALLER asks for it; the main thread ending also clears mode9A1 unless it's 1. */
u32 record_advance(struct Owner *owner, struct ScriptThread *thread)
{
    struct OwnerState *state;
    u16 *flags;
    register u32 waits asm("r4");

    if (thread->pc != thread->end)
    {
        return 0;
    }
    flags = &thread->flags;
    if ((*flags & THREAD_RESUME_CALLER) != 0)
    {
        u8 *caller = (u8 *)owner->state + thread->caller * 0xA8;

        ((struct ScriptThread *)caller)->flags &= 0xFFFB;
        *flags &= 0xFFF7;
    }
    waits = *flags & ~THREAD_ACTIVE;
    if (waits == 0)
    {
        if (thread->nextPc != 0)
        {
            /* start the queued script and empty the queue (waits is 0 here); the raw u16 access
             * to nextFlags keeps the original's register use */
            register u8 *nextCaller asm("r12");
            u8 zero;
            u16 nextFlags;

            thread->pc = thread->nextPc;
            thread->end = thread->nextEnd;
            nextFlags = *(u16 *)((u8 *)thread + 0xA2);
            zero = 0;
            *flags = nextFlags;
            nextCaller = &thread->nextCaller;
            thread->caller = *nextCaller;
            thread->nextEnd = waits;
            thread->nextPc = waits;
            *(u16 *)((u8 *)thread + 0xA2) = waits;
            *nextCaller = zero;
            return 0;
        }
        *flags = 0;
    }
    state = owner->state;
    if (state->cur9A0 == 0 && state->mode9A1 != 1)
    {
        state->mode9A1 = 0;
    }
    return 1;
}

// Runs the decoded command of a script record (opcode at +0xA7, arguments in args) by
// calling its handler. Returns the handler's result: 0 stops the record for this frame,
// nonzero keeps running it. Unknown opcodes are skipped (1).
u32 record_dispatch(u8 *owner, u8 *rec, u32 *args)
{
    u32 result;

    switch (rec[0xA7])
    {
    case RCMD_RESET_CURSOR:
        result = reset_cursor(rd_ctx(owner), rec);
        break;
    case RCMD_RESTORE_OBJ_WORD0:
        result = restore_obj_word0(rd_ctx(owner), rec);
        break;
    case RCMD_SET_OBJ_WORD0_SAVED:
        result = set_obj_word0_saved(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_OBJ_WORD9C_FLAG:
        result = set_obj_word9c_flag(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_VALUE:
        result = cmd_set_value(owner, rec, args, rd_cur(owner));
        break;
    case RCMD_SET_NEXT_FLAG4:
        result = cmd_set_next_flag4(owner, rec, args);
        break;
    case RCMD_FIND_RECORD:
        result = cmd_find_record(owner, rec, args, rd_cur(owner));
        break;
    case RCMD_RESET_NEXT_RECORD:
        result = cmd_reset_next_record(owner, rec, args);
        break;
    case RCMD_OBJ_SETUP_GLOBAL:
        result = cmd_obj_setup_global(owner, rec, args);
        break;
    case RCMD_START_MOVE:
        result = cmd_start_move(owner, rec, args);
        break;
    case RCMD_OBJECT_OP_CUR:
        result = cmd_object_op_cur(owner, rec, args);
        break;
    case RCMD_SET_POS_FOR_CURRENT:
        result = cmd_set_pos_for_current(owner, rec, args);
        break;
    case RCMD_OBJ_OP_AE49:
        result = cmd_obj_op_ae49(owner, rec, args);
        break;
    case RCMD_SET_OBJ_FLAGS_LIST2:
        result = cmd_set_obj_flags_list2(owner, rec, args);
        break;
    case RCMD_SET_OBJECT_FLAGS_LIST:
        result = cmd_set_object_flags_list(owner, rec, args);
        break;
    case RCMD_DRAW_TEXT_ITEM_RAW:
        result = draw_text_item_raw(rd_ctx(owner), rd_entry(owner), rec, args);
        break;
    case RCMD_PLACE_SPRITE:
        result = cmd_place_sprite(owner, rd_entry(owner), rec, args);
        break;
    case RCMD_DRAW_TEXT_ITEM:
        result = draw_text_item(rd_ctx(owner), rd_entry(owner), rec, args);
        break;
    case RCMD_BRANCH_IF_STATE_BIT:
        result = branch_if_state_bit(rd_ctx(owner), rd_entry(owner), rec, args);
        break;
    case RCMD_MAP_RECTS:
        result = map_rects_cmd(rd_ctx(owner), rd_entry(owner), rec, args);
        break;
    case RCMD_BUMP_OR_BRANCH:
        result = bump_or_branch_record(rd_ctx(owner), rd_entry(owner), rec, args);
        break;
    case RCMD_SET_TEXT_WINDOW_32:
        result = set_text_window_32(rd_ctx(owner), rd_entry(owner), args);
        break;
    case RCMD_SLOT_TARGET:
        result = slot_target_cmd(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_SLOT_PARAM:
        result = set_slot_param(rd_ctx(owner), rec, args);
        break;
    case RCMD_SLOT_VALUE:
        result = slot_value_cmd(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_RAMP0_IF_ZERO:
        result = set_ramp0_if_zero(rd_ctx(owner), rec, args);
        break;
    case RCMD_COPY_IF_SLOT_BIT:
        result = copy_if_slot_bit(rd_ctx(owner), rd_entry(owner), rec, args);
        break;
    case RCMD_STORE_IF_TEXT_BYTE:
        result = store_if_text_byte(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_VAR_FROM_TEXT_BYTE:
        result = set_var_from_text_byte(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_CLEAR_LIST_ENTRY:
        result = clear_list_entry(rd_ctx(owner), rd_aux(owner), rec, args);
        break;
    case RCMD_AUX_ENTRY:
        result = cmd_aux_entry(owner, rec, args);
        break;
    case RCMD_BRANCH_IF_LIST_ENTRY:
        result = branch_if_list_entry(rd_ctx(owner), rd_aux(owner), rec, args);
        break;
    case RCMD_CUR_OBJ_BBF8:
        result = cur_obj_bbf8(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_CUR_OBJ_29:
        result = set_cur_obj_29(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_CUR_OBJ_26_27:
        result = set_cur_obj_26_27(rd_ctx(owner), rec, args);
        break;
    case RCMD_APPLY_OBJ_MODE:
        result = apply_obj_mode(rd_ctx(owner), rec, args);
        break;
    case RCMD_STORE_OBJ_BYTE_TO_VAR:
        result = store_obj_byte_to_var(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_LIST_TOGGLE:
        result = cmd_list_toggle(owner, rec, args);
        break;
    case RCMD_LATCH_WHEN_READY:
        result = cmd_latch_when_ready(owner, rec);
        break;
    case RCMD_UPDATE_OBJ_COUNTERS:
        result = update_obj_counters(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_OBJ_FLAG:
        result = set_obj_flag(rd_ctx(owner), rec, args);
        break;
    case RCMD_STORE_OBJ_FIELD:
        result = cmd_store_obj_field(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_UPDATE_AND_STORE:
        result = cmd_update_and_store(rd_ctx(owner), (u32)rec, args, (u32)rd_vars(owner));
        break;
    case RCMD_READ_STORE:
        result = cmd_read_store(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_BINARY_OP:
        result = binary_op_and_set_var(rd_ctx(owner), rec, args, rd_vars(owner), rd_params(owner), 0);
        break;
    case RCMD_SET_SHADOW_BITS:
        result = set_shadow_bits(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_VAR_FROM_KEY_GROUP:
        result = set_var_from_key_group(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_SET_VAR_FROM_KEYS:
        result = set_var_from_keys(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_COPY_ON_KEY_MATCH_B:
        result = copy_on_key_match_b(rd_ctx(owner), rec, args);
        break;
    case RCMD_COPY_ON_KEY_MATCH_A:
        result = copy_on_key_match_a(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_KEY_MODE_BITS:
        result = set_key_mode_bits(rd_ctx(owner), rec, args);
        break;
    case RCMD_CALL_B0C0:
        result = call_b0c0(rd_ctx(owner), rec, args);
        break;
    case RCMD_COPY_IF_CMP_MATCH:
        result = copy_if_cmp_match(rd_ctx(owner), rec, args);
        break;
    case RCMD_RANDOM_OBJ_PLACEMENT:
        result = random_obj_placement(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_PICK_RANDOM_ENTRY:
        result = pick_random_entry(rd_ctx(owner), rec, args);
        break;
    case RCMD_PUSH_OBJ_ENTRY:
        result = push_obj_entry(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_OBJ_WORD_98:
        result = set_obj_word_98(rd_ctx(owner), rec, args);
        break;
    case RCMD_POP_OBJ_ENTRY:
        result = pop_obj_entry(rd_ctx(owner), rec);
        break;
    case RCMD_COUNTER_STEP_AND_BRANCH:
        result = counter_step_and_branch(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_OBJ_IF_CMP:
        result = set_obj_if_cmp(rd_ctx(owner), rec, args);
        break;
    case RCMD_DISPATCH_OBJ_MODE_CALL:
        result = dispatch_obj_mode_call(rd_ctx(owner), rec, args);
        break;
    case RCMD_REQUEST_IF_IDLE:
        result = request_if_idle(rd_ctx(owner), rec, args);
        break;
    case RCMD_SET_VAR_LANG:
        result = cmd_set_var_lang(rd_ctx(owner), rec, args, rd_vars(owner));
        break;
    case RCMD_SLOT_VALUE_BY_KIND:
        result = cmd_slot_value_by_kind(owner, rec, args);
        break;
    case RCMD_SLOT_OP:
        result = cmd_slot_op(owner, rec, args);
        break;
    case RCMD_SET_SLOT_FLAG:
        result = cmd_set_slot_flag(owner, rec, args);
        break;
    case RCMD_SLOT_FIELD:
        result = cmd_slot_field(owner, rec, args);
        break;
    case RCMD_START_MOVE_LIST:
        result = cmd_start_move_list(owner, rec, args);
        break;
    case RCMD_SET_POSITION:
        result = cmd_set_position(owner, rec, args);
        break;
    case RCMD_PLACE_OBJ_FLAGS3:
        result = cmd_place_obj_flags3(owner, rec, args);
        break;
    case RCMD_PLACE_OBJ_SYNC:
        result = cmd_place_obj_sync(owner, rec, args);
        break;
    case RCMD_PLACE_OBJECT:
        result = cmd_place_object(owner, rec, args);
        break;
    case RCMD_PLACE_SLOT_FLAGS:
        result = cmd_place_slot_flags(owner, rec, args);
        break;
    case RCMD_SET_KEY_IGNORE_MASK:
        result = set_key_ignore_mask(rd_ctx(owner), rec, args);
        break;
    case RCMD_PUSH_HANDLE:
        result = cmd_push_handle(owner, rec, args);
        break;
    case RCMD_LATCH_WHEN_FREE:
        result = cmd_latch_when_free(owner, rec);
        break;
    case RCMD_SET_MODE_1:
        result = cmd_set_mode_1(owner, rec, args);
        break;
    case RCMD_LIST_MODE:
        result = cmd_list_mode(owner, rec, args);
        break;
    default:
        goto invalid;
    }
    return (s8)result;
invalid:
    return 1;
}
