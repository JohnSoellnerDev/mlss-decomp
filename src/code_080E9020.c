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

struct FlagPairs
{
    u8 b01 : 2;
    u8 b23 : 2;
    u8 b45 : 2;
    u8 b67 : 2;
};

struct Voice
{
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
};

extern u8 gSys[];
extern u8 gItemTable[];
extern u8 *gParty;
extern void *gUnk_03000FB8;
extern void *gUnk_03000FBC;
extern void *gUnk_03000FC0;
extern void *gUnk_03000FC4;
extern void (*gUnk_03000F8C)(u32, u32, u32, u32, u32, u32, u32);
extern void (*gMemRoutines)(u32 a, void *b, u32 c);
extern u8 gTagTextState[];
extern u8 gFlagBlockTag[];
extern u8 tile_shift_copy[];
extern u8 var_block_routine[];
extern u8 gVarBlockTag[];
void cpu_set(const void *src, void *dst, u32 ctl);

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

void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void *format_decimal(void *dst, u32 a, u32 b);
char *text_copy_entry(char *dst, u16 idx);
void voice_setup_params(void *voice, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i);
void voice_set_data_ptr(void *voice, u32 a, u32 b, void *ptr);
void mem_copy_fast(void *dst, void *src, u32 len);
u32 layout_text_run();
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, u32 *out, s32 slot);
extern u8 gTagSlots[];

NONMATCH("asm/code_080E9020/init_slot_table.s", void *init_slot_table(void **slot))
{
    u8 *cursor;
    u32 local;
    u16 count;
    u32 v;
    void *obj;

    cursor = *slot = heap_alloc_raw(1, 0x40, gTagSlots);
    gMemRoutines(0, cursor, 0x40);
    find_or_alloc_obj_slots(7, 0x8001, 1, 0, 0, &local, 0xF);
    count = 4;
    do
    {
        cursor[0xC] = 0xFF;
        obj = create_obj_de4(0, 0x8004, 1, -1, -1, -1, -1);
        *(void **)cursor = obj;
        *((u8 *)obj + 0x1F) = 0;
        obj_load_record(obj, 0, -1, 0, 0);
        v = ((u32)count << 16) + 0xFFFF0000;
        cursor += 0x10;
        count = v >> 16;
    } while ((s32)v > 0);
    return slot;
}
END_NONMATCH

void sprite_update_cd0(void *sprite);
u8 claim_table_slot(void *state, u8 a, u8 b, u8 c, u16 d, u16 e, u8 f)
{
    struct SlotEntry *p;
    struct SlotObj *obj;
    s16 i;

    i = 0;
    p = *(struct SlotEntry **)state;
    for (; i <= 3; i++, p++)
    {
        if (p->inUse == 0)
        {
            obj = p->obj;
            obj_load_record(obj, b, -1, 0, 0);
            p->unkC = a;
            p->unkE = c;
            p->unk4 = d;
            p->unk6 = e;
            p->unk8 = 0;
            p->unkA = 0;
            p->counter = f;
            p->inUse = 1;
            ((struct FlagPairs *)((u8 *)obj + 0x1F))->b67 = 1;
            ((struct FlagPairs *)((u8 *)obj + 0x1F))->b45 = 1;
            ((struct FlagPairs *)((u8 *)obj + 0x1F))->b23 = 1;
            ((struct FlagPairs *)((u8 *)obj + 0x1F))->b01 = 1;
            obj->unkE = 0;
            sprite_update_cd0(obj);
            return i;
        }
    }
    return 0xFF;
}

ASM_FUNC("asm/code_080E9020/layout_text_run.s", u32 layout_text_run())

struct TextStream
{
    u8 filler0[8];
    u8 *cursor;
};

struct TextStreamState
{
    u32 *unk0;
    u16 *unk4;
    u8 filler8[4];
    u32 unkC;
};

void text_stream_step(struct TextStream *st, u32 advance)
{
    u8 *p;
    u8 prev;
    u8 cur;
    struct TextStreamState *g;

    p = st->cursor;
    prev = p[-1];
    cur = *p;
    if (advance == 0)
    {
        return;
    }
    if (prev > 0xA)
    {
        st->cursor = p + 1;
    }
    switch (prev)
    {
    case 0x0E:
        g = (struct TextStreamState *)gUnk_03000FBC;
        g->unkC = (u32)st->cursor;
        st->cursor = text_copy_entry((char *)g + 0x10, *g->unk4);
        break;
    case 0x0F:
        g = (struct TextStreamState *)gUnk_03000FBC;
        g->unkC = (u32)st->cursor;
        st->cursor = format_decimal((u8 *)g + 0x10, 0, *g->unk0);
        break;
    case 0x10:
        g = (struct TextStreamState *)gUnk_03000FBC;
        g->unkC = (u32)st->cursor;
        st->cursor = format_decimal((u8 *)g + 0x10, cur, *g->unk0);
        break;
    case 0x0A:
        g = (struct TextStreamState *)gUnk_03000FBC;
        if (g->unkC != 0)
        {
            st->cursor = (u8 *)g->unkC;
            g->unkC = 0;
        }
        break;
    }
}


ASM_FUNC("asm/code_080E9020/format_decimal.s", void *format_decimal(void *dst, u32 a, u32 b))

char *text_copy_entry(char *dst, u16 idx)
{
    u8 *table;
    u8 *entry;
    u32 lang;
    u8 **langs;
    u8 *src;
    u8 *out;
    u32 offset;

    table = gItemTable;
    offset = (idx & 0xFF) << 4;
    table += 0xC;
    entry = table + offset;
    lang = gSys[0x8C0];
    langs = *(u8 ***)entry;
    src = *(u8 **)langs[lang];
    out = (u8 *)dst;
    while (*src != 0)
    {
        *out = *src;
        src++;
        out++;
    }
    *out = 0xFF;
    out[1] = 0x0A;
    out[2] = 0;
    return dst;
}

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

extern u8 gWindowData[];
extern u8 gTagWindow[];
extern u8 gTagWindowBuf[];
extern void (*gUnk_03000C84)(void *a, void *b);
void load_engine_block(void);
void set_03001074(void *fn);
u32 read_varlen_plus1(void *src);

struct WindowData
{
    u8 filler0[0x1C];
    u8 *unk1C;
};

void text_window_open(void)
{
    void **dst;
    void *win;
    struct WindowData *wd;
    u8 *rec;
    u32 size;
    u8 *buf;
    vu32 *dma;

    load_engine_block();
    set_03001074(text_stream_step);
    dst = &gUnk_03000FB8;
    win = heap_alloc_raw(1, 0x34, gTagWindow);
    *dst = win;
    gMemRoutines(0, win, 0x34);
    text_state_alloc();
    wd = (struct WindowData *)gWindowData;
    rec = wd->unk1C;
    rec = rec + *(u32 *)(rec + 0xC);
    size = read_varlen_plus1(rec);
    buf = heap_alloc_raw(1, size, gTagWindowBuf);
    gUnk_03000C84(rec, buf);
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)buf;
    dma[1] = 0x06010000;
    dma[2] = 0x840000A0;
    (void)dma[2];
    heap_free(buf);
}

void update_slot_bounds(u8 **table, u8 idx)
{
    u32 off = (u8)idx << 4;
    u8 *base = *table;
    struct SlotObj *obj;
    struct SlotEntry *entry;
    u32 sum1;

    base += off;
    entry = (struct SlotEntry *)base;
    obj = entry->obj;
    obj->unk0 = entry->unk4 + entry->unk8;
    sum1 = entry->unkA;
    base = (u8 *)(u32)entry->unk6;
    sum1 += (u32)base;
    obj->unk2 = sum1;
    obj->unkE = 0x7FFF - sum1;
}

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

extern u16 gTextModeTable[];
void sound_stop_song(u16 id);
void set_slot_value(u32 id, s32 value);

void text_mode_update(u32 unused, u32 a1, u32 a2)
{
    u16 x;
    u16 mode;

    x = a1;
    mode = a2;
    switch (mode)
    {
    case 0:
        if (x != 0)
        {
            sound_stop_song(gTextModeTable[x]);
        }
        break;
    case 1:
        if (x != 0)
        {
            set_slot_value(gTextModeTable[x], -1);
        }
        break;
    case 2:
        if (x != 0)
        {
            sound_stop_song(gTextModeTable[x]);
            set_slot_value(gTextModeTable[x + 7], -1);
        }
        break;
    }
}

ASM_FUNC("asm/code_080E9020/fill_tile_map_rect.s", void fill_tile_map_rect(void))

ASM_FUNC("asm/code_080E9020/unknown_thumb_080e98a8.s", void unknown_thumb_080e98a8(void))

void fill_rect_88(u32 a0, u32 a1, s16 a2, s16 a3, u8 a4, u8 a5)
{
    gUnk_03000F8C(a1, (u8)(a4 + 1), a2 + 8, a3 + 8,
                  a2 + (u16)(a4 - 1) * 8, a3 + (u16)(a5 - 1) * 8, 0x88888888);
}

void unknown_thumb_080e997c(u32 a0, u32 a1, s16 a2, s16 a3, u8 a4, u8 a5)
{
    gUnk_03000F8C(a1, (u8)(a4 + 1), a2 + 8, a3 + 8,
                  a2 + (u16)(a4 - 1) * 8, a3 + (u16)(a5 - 1) * 8, 0x55555555);
}

void fill_rect_55(u32 a0, u32 a1, s16 a2, s16 a3, u8 a4, u8 a5)
{
    gUnk_03000F8C(a1, (u8)(a4 + 1), a2 + 8, a3 + 8,
                  a2 + (u16)(a4 - 1) * 8, a3 + (u16)(a5 - 1) * 8, 0x55555555);
}

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

u32 text_run_layout(u8 *obj, void *dataPtr, u32 mode)
{
    struct Voice voice[2];

    voice_setup_params(&voice[0], 0, 0x20, 0, 0, 0, 0, 6, 1,
                       ((u8 *)*(u32 *)(obj + 4))[0x1B7] >> 5);
    voice_set_data_ptr(&voice[0], 0, 0, dataPtr);
    mem_copy_fast(&voice[1], &voice[0], 0x14);
    return layout_text_run(voice[1], mode);
}

u32 lang_table_pair(u32 unused0, u8 **table)
{
    u8 *p = table[gSys[0x8C0]];

    return (p[0] << 16) | p[1];
}

struct RecBits
{
    u8 lo2 : 2;
    u8 b2 : 1;
    u8 field : 4;
    u8 hi : 1;
};

NONMATCH("asm/code_080E9020/bump_record.s", void bump_record(struct SlotState *st, u16 k))
{
    u8 *p;
    s32 v;
    u8 *q;
    u8 rec;

    p = st->unk4;
    v = (s32)p[0x1B2] >> k;
    if ((((u8)v ^ 1) & 1) != 0)
    {
        return;
    }
    q = p + k * 0x4C + 0x42;
    rec = *q;
    if ((rec & 4) != 0)
    {
        s32 t = -5;
        t &= rec;
        *q = t;
    }
    else
    {
        ((struct RecBits *)q)->field = (((u32)rec << 25) >> 28) + 1;
    }
}
END_NONMATCH

struct FreeDesc
{
    struct FreeDesc *next;
    u32 start;
    u32 size;
};

struct FreeListState
{
    struct FreeDesc *unk0;
    struct FreeDesc *unk4;
    struct FreeDesc *unk8;
};

void free_list_move(struct FreeListState *st, u32 a1)
{
    struct FreeDesc *prev;
    struct FreeDesc *head;
    struct FreeDesc *cur;
    u32 size;
    u32 start;

    prev = 0;
    head = st->unk0;
    while (head != 0)
    {
        start = head->start;
        if (start == a1)
        {
            break;
        }
        prev = head;
        head = head->next;
    }
    if (head == 0)
    {
        while (1)
        {
        }
    }
    if (prev != 0)
    {
        prev->next = head->next;
    }
    else
    {
        st->unk0 = head->next;
    }
    prev = 0;
    cur = st->unk4;
    while (cur != 0 && cur->start <= a1)
    {
        prev = cur;
        cur = cur->next;
    }
    if (prev != 0)
    {
        prev->next = head;
    }
    else
    {
        st->unk4 = head;
    }
    head->next = cur;
    if (cur != 0)
    {
        size = head->size;
        if (start + size == cur->start)
        {
            head->size = size + cur->size;
            head->next = cur->next;
            cur->next = st->unk8;
            st->unk8 = cur;
        }
    }
    if (prev != 0)
    {
        if (prev->start + prev->size == start)
        {
            prev->size = prev->size + head->size;
            prev->next = head->next;
            head->next = st->unk8;
            st->unk8 = head;
        }
    }
}

NONMATCH("asm/code_080E9020/free_list_alloc.s", u32 free_list_alloc(struct FreeListState *st, u32 a1))
{
    struct FreeDesc *best;
    struct FreeDesc *cur;
    struct FreeDesc *back;
    struct FreeDesc *newDesc;
    u32 size;

    if (st->unk8 == 0)
    {
        return 0;
    }
    best = 0;
    back = 0;
    cur = st->unk4;
    while (cur != 0)
    {
        size = cur->size;
        if (size > a1)
        {
            if (best == 0)
            {
                best = cur;
            }
        }
        else if (size == a1)
        {
            if (back == 0)
            {
                st->unk4 = cur->next;
            }
            else
            {
                back->next = cur->next;
            }
            cur->next = st->unk0;
            st->unk0 = cur;
            return cur->start;
        }
        back = cur;
        cur = cur->next;
    }
    if (best != 0)
    {
        newDesc = st->unk8;
        newDesc->size = a1;
        best->size -= a1;
        newDesc->start = best->start + best->size;
        st->unk8 = newDesc->next;
        newDesc->next = st->unk0;
        st->unk0 = newDesc;
        return newDesc->start;
    }
    return 0;
}
END_NONMATCH

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

NONMATCH("asm/code_080E9020/sub_080E9EC4.s", void sub_080E9EC4(s16 idx, s16 mode, u16 bit))
{
    u8 *entry;
    u32 b;
    u32 f;
    s32 t;

    entry = gParty + idx * 60;
    switch (mode)
    {
    case 0:
        b = entry[7];
        f = ((b >> 2) & ~(1 << (s16)bit)) << 2;
        t = 3;
        t &= b;
        t |= f;
        entry[7] = t;
        break;
    case 1:
        b = entry[8];
        f = ((b << 26) >> 26) & ~(1 << (s16)bit);
        t = -0x40;
        t &= b;
        t |= f;
        entry[8] = t;
        break;
    case 2:
        b = *(u16 *)(entry + 8);
        f = (((b << 20) >> 26) & ~(1 << (s16)bit)) << 6;
        t = 0xFFFFF03F;
        t &= b;
        t |= f;
        *(u16 *)(entry + 8) = t;
        break;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E9020/decode_stream_run.s", void decode_stream_run(void))

ASM_FUNC("asm/code_080E9020/test_flag.s", u8 test_flag(u8 *table, u8 *tableB, u16 id))

void set_flag(u8 *flagsA, u8 *flagsB, u16 id, u8 value)
{
    u32 *word;
    u32 bit;

    if (id <= 0x1B5F)
    {
        word = (u32 *)((u8 *)gUnk_03000FC0 + 0x20 + ((id >> 5) << 2));
    }
    else if (id <= 0x1B7F)
    {
        id = id - 0x1B60;
        word = (u32 *)(flagsA + ((id >> 5) << 2));
    }
    else
    {
        if (id > 0x1B8F)
        {
            return;
        }
        if (flagsB != 0)
        {
            id = id - 0x1B80;
            word = (u32 *)(flagsB + ((id >> 5) << 2));
        }
        else
        {
            id = id - 0x1B60;
            word = (u32 *)(flagsA + ((id >> 5) << 2));
        }
    }
    bit = id & 0x1F;
    *word = (*word & ~(1 << bit)) | ((value & 1) << bit);
}

NONMATCH("asm/code_080E9020/read_var.s", u32 read_var(u16 arg))
{
    u32 id;
    u8 *base;
    u32 value;

    id = arg;
    if (id <= 0x1CB7)
    {
        id = (u16)(id - 0x1B90);
        base = (u8 *)gUnk_03000FC0 + 0x414;
        value = base[id];
    }
    else if (id <= 0x1CD7)
    {
        id = (u16)(id - 0x1CB8);
        base = (u8 *)gUnk_03000FC0 + 0x53C;
        value = base[id];
    }
    else if (id <= 0x1D17)
    {
        u32 off;

        off = ((u32)(u16)(id - 0x1CD8) << 16) >> 15;
        base = (u8 *)gUnk_03000FC0 + 0x394;
        value = *(u16 *)(base + off);
    }
    else
    {
        value = 0;
    }
    return value;
}
END_NONMATCH

ASM_FUNC("asm/code_080E9020/write_var.s", void write_var(u16 id, u16 value))

void free_var_block(void)
{
    heap_free(gUnk_03000FC4);
}

extern u8 tile_shift_copy[];
extern u8 var_block_routine[];
extern u8 gVarBlockTag[];
void cpu_set(const void *src, void *dst, u32 ctl);
void alloc_var_block(void)
{
    void **dst = &gUnk_03000FC4;
    u8 *src;
    void *p;
    u32 size;

    size = (u32)tile_shift_copy;
    src = var_block_routine;
    size -= (u32)src;
    p = heap_alloc_raw(0, size, gVarBlockTag);
    *dst = p;
    cpu_set(src, p, (size & 0x3FFFFF) >> 1);
}

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

u32 lang_table_pair(u32 unused0, u8 **table);
ASM_FUNC("asm/code_080E9020/draw_text_item.s", void draw_text_item(void))

struct ObjFlags28
{
    u8 filler0[0x28];
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 field : 4;
    u8 rest : 2;
};

extern u8 *gCurObj;

void anim_set_gfx(u8 *obj, u32 half, u32 field);
void set_flags_2_3(u8 *p);
void set_flag_2_2(u8 *p);

u32 apply_obj_mode(u32 a, u32 b, s32 *modePtr)
{
    switch (*modePtr)
    {
    case 0:
        ((struct ObjFlags28 *)gCurObj)->bit0 = 0;
        {
            u32 lo = ((u32)((u8 *)gCurObj)[0x28] << 31) >> 31;
            u32 hi = ((u32)((u8 *)gCurObj)[0x28] << 30) >> 31;
            if ((hi | lo) == 0)
                set_flag_2_2(gCurObj);
        }
        break;
    case 1:
        ((struct ObjFlags28 *)gCurObj)->bit0 = 1;
        anim_set_gfx(gCurObj, *(u16 *)(gCurObj + 0x20), ((struct ObjFlags28 *)gCurObj)->field);
        set_flags_2_3(gCurObj);
        break;
    case 2:
        ((struct ObjFlags28 *)gCurObj)->bit1 = 0;
        set_flags_2_3(gCurObj);
        break;
    case 3:
        ((struct ObjFlags28 *)gCurObj)->bit1 = 1;
        {
            u32 lo = ((u32)((u8 *)gCurObj)[0x28] << 31) >> 31;
            u32 hi = ((u32)((u8 *)gCurObj)[0x28] << 30) >> 31;
            if ((hi | lo) == 0)
                set_flag_2_2(gCurObj);
        }
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080E9020/update_obj_counters.s", void update_obj_counters(void))
