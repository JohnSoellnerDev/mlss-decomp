#include "global.h"

struct SlotObj
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xA];
    u16 unkE;
};

struct SlotEntry
{
    struct SlotObj *obj;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u8 unkC;
    u8 counter;
    u8 unkE;
    u8 inUse;
};

struct SlotState
{
    u8 filler0[4];
    u8 *unk4;
};

struct TextPair
{
    u8 filler0[0xC];
    void *unkC;
};

extern u8 gUnk_0300034C[];
extern void *gUnk_03000FB8;
extern void *gUnk_03000FBC;
extern void *gUnk_03000FC0;
extern void *gUnk_03000FC4;
extern void (*gUnk_03000F8C)(u32, u32, u32, u32, u32, u32, u32);
extern void (*gMemRoutines)(u32 a, void *b, u32 c);
extern u8 gTagTextState[];
extern u8 gFlagBlockTag[];

void heap_free(void *p);
void heap_free_b(void *p);
void *heap_alloc_raw(u32 pool, u32 size, void *tag);
void obj_release_slots(void *obj, u32 off, u32 a, u32 b);
void free_engine_block(void);
u32 arc_tan2(s32 a, s32 b);
void text_state_free(void);
u8 test_flag(u8 *table, u8 *tableB, u16 id);
void set_flag(u8 *flagsA, u8 *flagsB, u16 id, u8 value);
u32 read_var(u16 id);
void write_var(u16 id, u16 value);

ASM_FUNC("asm/code_080E9020/init_slot_table.s", void init_slot_table(void))

ASM_FUNC("asm/code_080E9020/claim_table_slot.s", void claim_table_slot(void))

ASM_FUNC("asm/code_080E9020/layout_text_run.s", void layout_text_run(void))

ASM_FUNC("asm/code_080E9020/text_stream_step.s", void text_stream_step(void))

ASM_FUNC("asm/code_080E9020/format_decimal.s", void format_decimal(void))

ASM_FUNC("asm/code_080E9020/text_copy_entry.s", void text_copy_entry(void))

void text_state_free(void)
{
    if (gUnk_03000FBC != 0)
    {
        heap_free(gUnk_03000FBC);
        gUnk_03000FBC = 0;
    }
}

void text_state_alloc(void)
{
    void **dst = &gUnk_03000FBC;
    void *p = heap_alloc_raw(1, 0x34, gTagTextState);

    *dst = p;
    gMemRoutines(0, p, 0x34);
}

void text_window_close(void)
{
    free_engine_block();
    if (gUnk_03000FB8 != 0)
    {
        heap_free(gUnk_03000FB8);
        gUnk_03000FB8 = 0;
    }
    text_state_free();
}

ASM_FUNC("asm/code_080E9020/text_window_open.s", void text_window_open(void))

ASM_FUNC("asm/code_080E9020/update_slot_bounds.s", void update_slot_bounds(void))

u32 slot_countdown(u8 **table, u8 idx, u32 unused2, u32 a3)
{
    u32 off = (u8)idx << 4;
    struct SlotEntry *entry = (struct SlotEntry *)(*table + off);
    struct SlotEntry *entry2;
    u8 counter;
    u8 left;

    if (entry->inUse == 0)
    {
        return 0;
    }
    counter = entry->counter;
    if (counter == 0)
    {
        return 1;
    }
    entry->counter = counter - 1;
    left = counter - 1;
    if (left != 0)
    {
        return 1;
    }
    entry2 = (struct SlotEntry *)(*table + off);
    obj_release_slots(entry2->obj, off, (u32)entry, a3);
    entry2->inUse = left;
    return 0;
}

void slot_obj_clear(u8 **table, u8 idx, u32 a, u32 b)
{
    u32 off = (u8)idx << 4;
    struct SlotEntry *entry = (struct SlotEntry *)(*table + off);

    obj_release_slots(entry->obj, off, a, b);
    entry->inUse = 0;
}

void free_ptr_pair(void **st, u32 flag)
{
    heap_free(*st);
    if (flag & 1)
    {
        heap_free_b(st);
    }
}

u32 slot_column(struct SlotState *st, u32 a)
{
    u16 *q = *(u16 **)(st->unk4 + 0x188);

    return ((a - ((0xC & *q) << 12)) << 16) >> 21;
}

ASM_FUNC("asm/code_080E9020/text_mode_update.s", void text_mode_update(void))

ASM_FUNC("asm/code_080E9020/fill_tile_map_rect.s", void fill_tile_map_rect(void))

ASM_FUNC("asm/code_080E9020/unknown_thumb_080e98a8.s", void unknown_thumb_080e98a8(void))

ASM_FUNC("asm/code_080E9020/fill_rect_88.s", void fill_rect_88(void))

ASM_FUNC("asm/code_080E9020/unknown_thumb_080e997c.s", void unknown_thumb_080e997c(void))

ASM_FUNC("asm/code_080E9020/fill_rect_55.s", void fill_rect_55(void))

void unknown_thumb_080e9a64(u32 a0, u32 a1, s16 a2, s16 a3, u8 a4, u8 a5)
{
    gUnk_03000F8C(a1, (u8)(a4 + 1), a2, a3, a2 + (a4 << 3), a3 + (a5 << 3), 0);
}

void sub_080E9AA8(u32 a0, u32 a1, u32 a2, u32 a3, u8 a4, u8 a5, u32 a6)
{
    gUnk_03000F8C(a1, (u8)(a4 + 1), 0, 0, a4 << 3, a5 << 3, a6);
}

u32 load_u32(u32 *p)
{
    return *p;
}

ASM_FUNC("asm/code_080E9020/text_run_layout.s", void text_run_layout(void))

u32 lang_table_pair(u32 unused0, u8 **table)
{
    u8 *p = table[gUnk_0300034C[0x8C0]];

    return (p[0] << 16) | p[1];
}

ASM_FUNC("asm/code_080E9020/bump_record.s", void bump_record(void))

ASM_FUNC("asm/code_080E9020/free_list_move.s", void free_list_move(void))

ASM_FUNC("asm/code_080E9020/free_list_alloc.s", void free_list_alloc(void))

void free_text_pair(struct TextPair *st, u32 flag)
{
    if (st->unkC != 0)
    {
        heap_free(st->unkC);
        st->unkC = 0;
    }
    if (flag & 1)
    {
        heap_free_b(st);
    }
}

ASM_FUNC("asm/code_080E9020/build_record_chain.s", void build_record_chain(void))

u32 angle_of_vector16(s32 x, s32 y)
{
    s32 a;
    s32 b;
    s32 negX;
    s32 negY;

    a = x;
    if (a < 0)
    {
        negX = 1;
        a = -a;
    }
    else
    {
        negX = 0;
    }
    b = y;
    if (b < 0)
    {
        negY = 1;
        b = -b;
    }
    else
    {
        negY = 0;
    }
    while (((a | b) & 0xFFFF8000) != 0)
    {
        a >>= 1;
        b >>= 1;
    }
    if (negX)
    {
        a = -a;
    }
    if (negY)
    {
        b = -b;
    }
    return (u16)(arc_tan2((s16)a, (s16)b) + 0x5000);
}

ASM_FUNC("asm/code_080E9020/unknown_thumb_080e9e2e.s", void unknown_thumb_080e9e2e(void))

ASM_FUNC("asm/code_080E9020/sub_080E9EC4.s", void sub_080E9EC4(void))

ASM_FUNC("asm/code_080E9020/decode_stream_run.s", void decode_stream_run(void))

ASM_FUNC("asm/code_080E9020/test_flag.s", u8 test_flag(u8 *table, u8 *tableB, u16 id))

ASM_FUNC("asm/code_080E9020/set_flag.s", void set_flag(u8 *flagsA, u8 *flagsB, u16 id, u8 value))

ASM_FUNC("asm/code_080E9020/read_var.s", u32 read_var(u16 id))

ASM_FUNC("asm/code_080E9020/write_var.s", void write_var(u16 id, u16 value))

void free_var_block(void)
{
    heap_free(gUnk_03000FC4);
}

ASM_FUNC("asm/code_080E9020/alloc_var_block.s", void alloc_var_block(void))

u32 unknown_thumb_080ea384(u16 id)
{
    u8 *block;
    u16 *counter;
    u16 v;

    if (id <= 0x1D17)
    {
        u16 idx = id - 0x1CD8;
        u32 off;
        u8 *base;

        block = gUnk_03000FC0;
        off = idx << 1;
        base = block + 0x394;
        counter = (u16 *)(base + off);
        v = *counter;
        if (v != 0)
        {
            v--;
        }
        *counter = v;
        return v;
    }
    return 0;
}

u32 inc_counter(u16 id)
{
    u16 idx;
    u8 *block;
    u32 off;
    u8 *base;
    u16 *counter;
    u16 v;

    if (id <= 0x1D17)
    {
        idx = id - 0x1CD8;
        block = gUnk_03000FC0;
        off = idx << 1;
        base = block + 0x394;
        counter = (u16 *)(base + off);
        v = *counter;
        if (v <= 0xFFFE)
        {
            v = v + 1;
        }
        *counter = v;
        return v;
    }
    return 0;
}

void set_word_slot(u8 *mid, u8 *low, s16 index, u32 value)
{
    if (index <= 15)
    {
        *(u32 *)(low + index * 4) = value;
    }
    else if (index <= 52)
    {
        *(u32 *)(mid + index * 4 - 0x40) = value;
    }
    else
    {
        u8 *block = gUnk_03000FC0;

        *(u32 *)(block + ((index - 0x35) << 2)) = value;
    }
}

u32 get_word_slot(u8 *mid, u8 *low, s16 index)
{
    u32 value;

    if (index <= 15)
    {
        value = *(u32 *)(low + index * 4);
    }
    else if (index > 52)
    {
        u8 *block = gUnk_03000FC0;

        value = *(u32 *)(block + ((index - 0x35) << 2));
    }
    else
    {
        value = *(u32 *)(mid + index * 4 - 0x40);
    }
    return value;
}

void set_var(u8 *mid, u8 *low, u8 *flagsA, u8 *flagsB, s16 index, u32 value)
{
    s16 rel;

    if (index <= 60)
    {
        set_word_slot(mid, low, index, value);
        return;
    }
    rel = index - 61;
    if (rel <= 0x1B8F)
    {
        set_flag(flagsA, flagsB, rel, value);
        return;
    }
    if (rel <= 0x1D17)
    {
        write_var(rel, value);
    }
}

u32 get_var(u8 *mid, u8 *low, u8 *flagsA, u8 *flagsB, s16 index)
{
    s16 rel;
    u32 value;

    if (index <= 60)
    {
        return get_word_slot(mid, low, index);
    }
    rel = index - 61;
    if (rel <= 0x1B8F)
    {
        return (u8)test_flag(flagsA, flagsB, rel);
    }
    if (rel > 0x1D17)
    {
        value = 0;
    }
    else
    {
        value = (u16)read_var(rel);
    }
    return value;
}

void free_flag_block(void)
{
    heap_free(gUnk_03000FC0);
    gUnk_03000FC0 = 0;
}

void alloc_flag_block(void)
{
    void **dst = &gUnk_03000FC0;
    u32 size = 0x570;
    void *block = heap_alloc_raw(1, size, gFlagBlockTag);

    *dst = block;
    gMemRoutines(0, block, size);
}

ASM_FUNC("asm/code_080E9020/draw_text_item.s", void draw_text_item(void))

ASM_FUNC("asm/code_080E9020/apply_obj_mode.s", void apply_obj_mode(void))

ASM_FUNC("asm/code_080E9020/update_obj_counters.s", void update_obj_counters(void))
