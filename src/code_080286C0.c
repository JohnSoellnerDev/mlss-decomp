/* agbcc: -funroll-loops */
#include "global.h"
#include "ram_routines.h"

struct EntrySlot
{
    struct SpriteEntry *sprite;
    u16 x;
    u16 y;
};

struct SubObjA
{
    u8 filler0[0x23E];
    u16 half23E;
    u8 filler240[0x351 - 0x240];
    u8 flags351;
};

struct DmaSource
{
    u8 filler0[8];
    void *data;
};

struct SceneTable
{
    struct DmaSource *unk0;
    u8 filler4[0x0C - 0x04];
    struct DmaSource *unk0C;
    u8 filler10[0x2C - 0x10];
    void *ids2C[12];
    void *ids5C[64];
    void *ids15C[64];
    struct EntrySlot entries25C[4];
};

struct SceneObj
{
    u8 state;
    u8 filler1[0x1C - 0x01];
    struct SceneTable *table;
    u8 filler20[0x24 - 0x20];
    u8 *blocks;
    void *entities[32];
    void *heapBufs[8];
    u16 *idTable;
    u8 filler0CC[0xF0 - 0xCC];
    u32 dirtyMask;
    u16 unkF4;
    u16 unkF6;
    u8 keyFlags;
    u8 pending;
    u8 filler0FA[0xFC - 0xFA];
    u16 fc;
    u8 filler0FE[0x100 - 0xFE];
    u16 half100;
    u8 filler102[0x10B - 0x102];
    u8 childCount;
    u8 idCount;
    u8 filler10D[0x112 - 0x10D];
    u8 itemFlag;
    u8 itemCount;
    u8 filler114[0x12B - 0x114];
    s8 slot;
    u8 state12C;
    u8 flags12D;
    u8 filler12E[0x200 - 0x12E];
    struct SubObjA *subA;
    void *subB;
    u8 flags208;
    u8 flags209;
    u8 filler20A;
    u8 flags20B;
    u8 filler20C[0x246 - 0x20C];
    u8 mode246;
    u8 filler247[0x274 - 0x247];
    s16 unk274;
    s16 unk276;
    s16 unk278;
    u16 unk27A;
    u16 unk27C;
    s16 camX;
    s16 camY;
    u8 filler282[0x286 - 0x282];
    u16 unk286;
    u16 unk288;
    u8 unk28A;
    u8 unk28B;
    u8 unk28C;
    u8 unk28D;
    u8 unk28E;
    u8 unk28F;
    u8 filler290[0x2AC - 0x290];
    u16 saved2AC;
    u8 filler2AE[0x2B4 - 0x2AE];
    u16 half2B4;
    u8 filler2B6[0x2EC - 0x2B6];
    u16 unk2EC;
    u16 unk2EE;
    u8 filler2F0[0x2F2 - 0x2F0];
    u16 unk2F2;
    u16 unk2F4;
    u8 filler2F6[0x2F8 - 0x2F6];
    u16 unk2F8;
    u16 unk2FA;
    u16 unk2FC;
    u8 unk2FE;
    u8 filler2FF[0x304 - 0x2FF];
    void *resource;
    u8 filler308[0x314 - 0x308];
    void *child;
};

struct PendingBits
{
    u8 low2 : 2;
    u8 unk2 : 6;
};

struct KeyFlagBits
{
    u8 unk0 : 6;
    u8 bit6 : 1;
    u8 unk7 : 1;
};

struct Bit1Bits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
};

struct SceneStateBits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 mode : 2;
    u8 unk4 : 4;
};

struct StateByteBits
{
    u8 low2 : 2;
    u8 field : 6;
};

struct FcByteBits
{
    u8 unk0 : 3;
    u8 mode : 4;
    u8 unk7 : 1;
};

struct FcHalfBits
{
    u16 unk0 : 7;
    u16 field : 4;
    u16 unk11 : 5;
};

struct KeyRegs
{
    u8 filler0[0x28];
    u16 raw28;
    u16 raw2A;
    u16 raw2C;
    u16 raw2E;
    u8 filler30[0x3C - 0x30];
    u16 held3C;
    u16 held3E;
    u16 held40;
    u16 held42;
    u8 filler44[0x78 - 0x44];
    u16 enable78 : 10;
};

extern void (*gUnk_03000D80[])(void);
void set_slot_value(u32 id, s32 value);
void map_obj_init_display(void *p);
void map_obj_init_display_b(void *p);
void obj_stop_line_scroll(void *p);
void sub_08117380(void *child);

struct EntryProc
{
    u8 filler0[8];
    s16 offset;
    u8 fillerA[0xC - 0xA];
    void (*fn)(void *, u32);
};

struct Entity
{
    u8 filler0[0xC];
    s32 x;
    s32 y;
    s32 yAdj1;
    s32 yAdj2;
    u8 filler1C[0x338 - 0x1C];
    struct EntryProc *proc;
};

struct SpriteEntry
{
    u16 posX;
    u16 posY;
    u8 filler4[0x12 - 0x4];
    u8 flags12;
};


struct SlotInit
{
    u8 unk0;
    u8 filler1[8 - 1];
    void *ptrA;
    void *ptrB;
    u8 unk10;
    u8 unk11;
    u8 unk12[4];
    u8 unk16;
    u8 filler17;
};

extern u8 gFlowState;
extern u32 gUnk_03000C78;
void sub_0800063C(void);
extern u8 gUnk_02000080[];
extern const u8 gTagBtl[];
extern const u8 gTagEndd[];
extern const u16 gLookup13b4Ids[];
extern u8 gSys[];
extern u16 gUnk_02000008;
extern u32 gUnk_03000E00;
extern u32 gUnk_03000E04;
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void poll_keys(void);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_draw_desc_entries(void *a0, u8 *p1, u32 p2, u32 p3, u32 x5, u32 p6, ByteArg p7, ByteArg p8);
extern void *gSlotInitPtrPairs[];
extern const u16 gLookup14b8Ids[];
void serial_link_disable(void);
void set_flag_2_2(struct SceneObj *obj);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void cpu_fast_set(const void *src, void *dst, u32 control);
void free_matching_slots_1930(s32 slot);
void find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, u32 *out, s32 slot);
void obj_teardown_subs(struct SceneObj *obj, u32 mode);
void status_obj_init(void *p, u32 size, const void *tag);
void effect_start(void *p, u32 size, const void *tag, void *arg);
void slot_reset(u32 id);
void object_teardown(struct SceneObj *obj);
void text_window_close(void);
void obj_release_parts(void *parts, u32 mode);
void set_irq_callback(u32 kind, void *cb);
void scene_setup(s32 slot);
u8 *bank_data_6bit(u16 bank, u8 *out);
u32 lookup_or_alloc_13b4(u32 a, u16 id, s32 b, u32 c);
void unlink_d54(void *e);
void heap_free_b(void *p);
void sub_0811733C(void *child, u32 value);
u32 arc_tan2(s32 x, s32 y);
void create_menu_object(s32 a, u32 b);

ASM_FUNC("asm/code_080286C0/camera_update.s", void camera_update(void))

struct AnimActor
{
    u8 filler0[0x31C];
    u32 anim;
};

struct AnimSrc
{
    u8 filler0[0x22C];
    u32 word;
};

struct AnimObj
{
    u8 filler0[0x33C];
    u8 animC;
    u8 filler33D[0x342 - 0x33D];
    s8 animA;
    u8 filler343;
    u8 animB;
    u8 filler345[0x350 - 0x345];
    u8 mode;
};

void register_effect_entry(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);

NONMATCH("asm/code_080286C0/apply_anim_state.s", void apply_anim_state(struct AnimActor *actor, struct AnimObj *obj, struct AnimSrc *src))
{
    u32 modeLo = 0;
    u32 modeHi = 0;

    switch (((u32)obj->mode << 28) >> 28)
    {
    case 1:
        modeLo = 4;
        break;
    case 2:
        modeLo = 6;
        break;
    case 4:
        break;
    case 8:
        modeLo = 2;
        break;
    }

    switch (((u32)obj->mode << 26) >> 30)
    {
    case 1:
        modeHi = 2;
        break;
    case 2:
        modeHi = 1;
        break;
    }

    if (obj->animA == 3)
    {
        obj->animB = 0x19;
    }
    else
    {
        switch ((s8)(obj->animB - 7))
        {
        case 0:
        case 2:
        case 3:
            if (obj->animA == -1)
                obj->animB = 0;
            break;
        case 5:
        case 7:
            if (obj->animA == -1)
                obj->animB = 0xB;
            break;
        }
    }

    register_effect_entry(actor->anim, ((u32)(((s32)((u32)obj->animC << 25)) >> 1)) >> 24, src->word, modeLo, modeHi, ((s8)obj->animB ^ 0x10) != 0, (u16)(s8)obj->animB);
}
END_NONMATCH

void sprites_obj_set_move_target(struct SceneObj *obj, u32 mode, u32 rel, s32 tx, s32 ty, u32 speed)
{
    u16 spd = speed;

    if (rel == 1)
    {
        tx += (s16)obj->unk27A;
        ty += (s16)obj->unk27C;
    }
    if (tx < 0)
    {
        tx = 0;
    }
    else if (tx > obj->unk286 - 0xF0)
    {
        tx = obj->unk286 - 0xF0;
    }
    if (ty < 0)
    {
        ty = 0;
    }
    else if (ty > obj->unk288 - 0xA0)
    {
        ty = obj->unk288 - 0xA0;
    }
    obj->unk2EC = obj->unk27A;
    obj->unk2EE = obj->unk27C;
    obj->unk2F2 = tx;
    obj->unk2F4 = ty;
    if (mode == 1)
    {
        s32 dx = (s16)obj->unk2F2 - (s16)obj->unk2EC;
        s32 dy = (s16)obj->unk2F4 - (s16)obj->unk2EE;
        s32 dist = ((s32 (*)(s32))gMathRoutines)(dx * dx + dy * dy);
        s32 q;
        u16 *dur;

        q = (dist << 8) / (s16)spd;
        dur = &obj->unk2FC;
        *dur = (u16)(q + 1);
    }
    else
    {
        obj->unk2FC = spd;
    }
    obj->unk2FA = 0;
    obj->unk2FE = arc_tan2((s16)(obj->unk2EE - obj->unk2F4), (s16)(obj->unk2F2 - obj->unk2EC)) >> 8;
    ((struct PendingBits *)&obj->pending)->low2 = mode;
}

void clear_obj_state_2f8(struct SceneObj *obj);
ASM_FUNC("asm/code_080286C0/sprites_obj_step_move.s", void sprites_obj_step_move(void))

NONMATCH("asm/code_080286C0/apply_key_filter.s", void apply_key_filter(struct SceneObj *obj, u16 keep))
{
    u32 block;
    u32 allowed;
    u32 old;

    block = ~keep & 0x3FF;
    old = ((struct KeyRegs *)gSys)->enable78;
    allowed = old & block;
    ((struct KeyRegs *)gSys)->enable78 = block;
    if ((obj->keyFlags & 0x20) == 0)
    {
        ((struct KeyRegs *)gSys)->held3C = keep | ((struct KeyRegs *)gSys)->held3C;
        poll_keys();
        ((struct KeyRegs *)gSys)->held3E &= allowed;
        ((struct KeyRegs *)gSys)->held40 &= allowed;
        ((struct KeyRegs *)gSys)->held42 = allowed & ((struct KeyRegs *)gSys)->held42;
    }
    else
    {
        ((struct KeyRegs *)gSys)->held3C = ((struct KeyRegs *)gSys)->raw28 & block;
        ((struct KeyRegs *)gSys)->held3E = ((struct KeyRegs *)gSys)->raw2A & block;
        ((struct KeyRegs *)gSys)->held40 = ((struct KeyRegs *)gSys)->raw2C & block;
        ((struct KeyRegs *)gSys)->held42 = ((struct KeyRegs *)gSys)->raw2E & block;
    }
    obj->keyFlags |= 0x20;
}
END_NONMATCH

void free_slot_and_call_1588(s32 unused, u32 a, u32 b, s32 slot)
{
    u32 out;

    if (slot != -1)
        free_matching_slots_1930(slot);
    find_or_alloc_obj_slots(a, b, 0, 0, 0, &out, slot);
}

void clear_obj_state_2f8(struct SceneObj *obj)
{
    s32 v;

    obj->unk2F8 = 0;
    obj->unk2FC = 0;
    ((struct PendingBits *)&obj->pending)->low2 = 0;
}

s32 obj_has_pending_bits(struct SceneObj *obj)
{
    u32 v = obj->pending & 0x3;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

void entity_camera_offset(struct SceneObj *obj, u32 idx, s32 *outX, s32 *outY)
{
    struct Entity *e = (struct Entity *)obj->entities[idx];
    s32 x = e->x;
    s32 y;

    if (x < 0)
        x += 0xFF;
    *outX = (x >> 8) - obj->camX;
    y = e->y - e->yAdj1 - e->yAdj2;
    if (y < 0)
        y += 0xFF;
    *outY = (y >> 8) - obj->camY;
}

void clear_flags_f8_41(struct SceneObj *obj)
{
    ((struct KeyFlagBits *)&obj->keyFlags)->bit6 = 0;
}

void set_flag_f8_40(struct SceneObj *obj)
{
    s32 v = obj->keyFlags;
    v |= 0x40;
    obj->keyFlags = v;
}

void scene_end_begin(struct SceneObj *obj)
{
    void *p;

    serial_link_disable();
    if (obj != 0)
        obj_teardown_subs(obj, 3);
    p = heap_alloc2(0x4AC, 0, gTagEndd, 0);
    status_obj_init(p, 8, gTagEndd);
}

void set_scene_flag_20(struct SceneObj *obj)
{
    s32 v = obj->flags12D;
    v |= 0x20;
    obj->flags12D = v;
}

void set_scene_loaded(struct SceneObj *obj)
{
    s32 v = obj->flags12D;
    v |= 0x10;
    obj->flags12D = v;
}

void start_battle_block(struct SceneObj *obj)
{
    obj->saved2AC = obj->half2B4;
    obj->half2B4 = 0xFFFF;
    set_flag_2_2(obj);
    obj->state = 2;
    gFlowState = 1;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
    {
        void *p = heap_alloc2(0x1C, 0, gTagBtl, 0);
        effect_start(p, 8, gTagBtl, &obj->saved2AC);
    }
}

void unload_scene(struct SceneObj *obj)
{
    serial_link_disable();
    slot_reset(0);
    object_teardown(obj);
    text_window_close();
    if (obj->resource != 0)
    {
        obj_release_parts(obj->resource, 3);
        obj->resource = 0;
    }
    set_irq_callback(2, 0);
    obj->state = 1;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
    scene_setup(obj->slot);
    obj->slot = -1;
}

void set_scene_slot(struct SceneObj *obj, s32 slot)
{
    obj->slot = slot;
}

void set_scene_state(struct SceneObj *obj, u32 flag, u32 mode)
{
    struct SceneStateBits *b = (struct SceneStateBits *)&obj->flags12D;

    b->bit0 = 1;
    b->bit1 = flag;
    b->mode = mode;
}

void set_obj_flags_12c_bit1(struct SceneObj *obj)
{
    s32 v = obj->state12C;
    v |= 2;
    obj->state12C = v;
}

void set_obj_flags_12c_bit0(struct SceneObj *obj)
{
    s32 v = obj->state12C;
    v |= 1;
    obj->state12C = v;
}

void enter_scene_mode2(struct SceneObj *obj)
{
    set_flag_2_2(obj);
    obj->state = 2;
    gFlowState = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
    create_menu_object((s8)obj->state12C >> 2, 0);
    obj->state12C = (obj->state12C & 3) | 0xF8;
}

void set_obj_12c_field(struct SceneObj *obj, u32 value)
{
    ((struct StateByteBits *)&obj->state12C)->field = value;
}

void flush_dirty_blocks(struct SceneObj *obj)
{
    if (obj->dirtyMask != 0)
    {
        u8 *src = obj->blocks;
        u8 *dst = gUnk_02000080;

        do
        {
            if (obj->dirtyMask & 1)
                cpu_fast_set(src, dst, 8);
            src += 0x20;
            dst += 0x20;
            obj->dirtyMask >>= 1;
        } while (obj->dirtyMask != 0);
    }
}

void load_dirty_blocks(struct SceneObj *obj, u32 hi, u16 lo)
{
    u32 mask = lo;
    u8 *dst;
    u8 *src;
    u32 bit;

    mask |= hi << 16;
    dst = gUnk_02000080;
    src = obj->blocks;
    bit = 1;

    while (mask != 0)
    {
        if (mask & 1)
        {
            cpu_fast_set(dst, src, 8);
            obj->dirtyMask |= bit;
        }
        dst += 0x20;
        src += 0x20;
        bit <<= 1;
        mask >>= 1;
    }
}

void set_obj_fc_state(struct SceneObj *obj, s32 value)
{
    struct FcByteBits *bits = (struct FcByteBits *)&obj->fc;

    switch ((u32)(*(u8 *)bits << 25) >> 28)
    {
    case 1:
        sub_0811733C(obj->child, value);
        bits->mode = value + 1;
        break;
    case 2:
        sub_0811733C(obj->child, value);
        bits->mode = 2 - value;
        break;
    }
}

void call_0d80_handlers(void);
void call_7380_for_state(struct SceneObj *obj)
{
    s32 mode = ((u32)(*(u8 *)&obj->fc) << 25) >> 28;

    switch (mode)
    {
    case 1:
    case 2:
        sub_08117380(obj->child);
        break;
    }
}

NONMATCH("asm/code_080286C0/sync_and_draw_obj.s", void sync_and_draw_obj(struct SceneObj *obj, u32 load, u32 p2, u32 p3, u32 p5, u32 p6))
{
    u32 lo;
    u32 hi;
    u32 e00;
    u32 e04;
    u8 *pal = gSys;

    lo = (u16)p5;
    hi = (p6 << 16) >> 16;
    pal += 0x884;
    *(u32 *)pal |= (p6 << 16) | lo;
    if (load == 0)
    {
        if (obj->dirtyMask != 0)
            flush_dirty_blocks(obj);
    }
    else
    {
        load_dirty_blocks(obj, hi, lo);
    }
    obj->unkF4 = lo;
    obj->unkF6 = hi;
    e00 = (lo & 1) ? gUnk_03000E00 : 0;
    e04 = (lo & 1) ? gUnk_03000E04 : 0;
    sub_08115F54(obj->child, (u8)load, (u16)p3, (u16)p2, lo, hi, e00, e04);
}
END_NONMATCH

struct BdObj
{
    u8 filler0[6];
    s16 unk6;
};

void clear_bd4_bit6(struct BdObj *obj)
{
    u8 *p = gSys;

    p += 0x888;
    ((struct KeyFlagBits *)p)->bit6 = 0;
    if (obj->unk6 == 6)
    {
        MEM_ROUTINE(void (*)(u32, void *, u32), iwram_fill)(0, (void *)0x0600C000, 0x20);
        gUnk_02000008 = 0x1508;
    }
}

void destroy_obj_children(struct SceneObj *obj)
{
    s32 i;

    for (i = 0; i < obj->childCount; i++)
    {
        struct Entity *e = (struct Entity *)obj->entities[i];

        if (e != 0)
        {
            struct EntryProc *p = e->proc;

            p->fn((u8 *)e + p->offset, 3);
        }
    }
    heap_free_b(obj->heapBufs[0]);
    heap_free_b(obj->heapBufs[1]);
    heap_free_b(obj->heapBufs[2]);
    heap_free_b(obj->heapBufs[3]);
    heap_free_b(obj->heapBufs[4]);
    heap_free_b(obj->heapBufs[5]);
    heap_free_b(obj->heapBufs[6]);
    heap_free_b(obj->heapBufs[7]);
}

void obj_update_child(void *obj);

void process_obj_item_list(struct SceneObj *obj)
{
    void **item;
    s32 i;

    if (obj->itemFlag != 0)
    {
        item = &obj->entities[2];
        for (i = 0; i < obj->itemCount; i++)
            obj_update_child(*item++);
    }
}

u32 lookup_or_alloc_14b8(u32 a, u16 id, s32 b, u32 c);

void resolve_ids_14b8(struct SceneObj *obj)
{
    u16 *p;
    s32 i;

    p = obj->idTable;
    for (i = 0; i < obj->idCount; i++, p++)
        obj->table->ids15C[i] = (void *)lookup_or_alloc_14b8(0, *p, -1, 1);
}

void load_id_table_13b4(struct SceneObj *obj, u16 id)
{
    u16 *p;
    s32 i;

    obj->idTable = (u16 *)bank_data_6bit(id, &obj->idCount);
    gUnk_03000C78 = 0;
    p = obj->idTable;
    for (i = 0; i < obj->idCount; i++, p++)
        obj->table->ids5C[i] = (void *)lookup_or_alloc_13b4(0, *p, -1, 1);
    gUnk_03000C78 = (u32)sub_0800063C;
}

void dma_copy_or_queue(const void *src, void *dst, u32 size, u32 a, u32 b);

void queue_tile_dmas(struct SceneObj *obj)
{
    dma_copy_or_queue(obj->table->unk0->data, (void *)0x06010400, 0x4C0, 0x20, 0);
    dma_copy_or_queue(obj->table->unk0C->data, (void *)0x060108C0, 0x500, 0x20, 0);
}

void load_table_14b8(struct SceneObj *obj)
{
    const u16 *p = gLookup14b8Ids;
    s32 i;

    for (i = 0; i < 11; p++, i++)
        obj->table->ids2C[i] = (void *)lookup_or_alloc_14b8(0, *p, -1, 1);
}

NONMATCH("asm/code_080286C0/load_table_13b4.s", void load_table_13b4(struct SceneObj *obj))
{
    const u16 *p;
    s32 i;

    gUnk_03000C78 = 0;
    p = gLookup13b4Ids;
    for (i = 0; i < 11; p++, i++)
        ((void **)obj->table)[i] = (void *)lookup_or_alloc_13b4(0, *p, -1, 1);
    gUnk_03000C78 = (u32)sub_0800063C;
}
END_NONMATCH

static inline void place_entry_sprite(struct EntrySlot *e, u16 *camX, u16 *camY)
{
    if ((e->sprite->flags12 & 0x20) == 0)
    {
        e->sprite->posX = e->x - *camX;
        e->sprite->posY = e->y - *camY;
    }
}

NONMATCH("asm/code_080286C0/update_entry_sprites.s", void update_entry_sprites(struct SceneObj *obj))
{
    struct SceneTable *t = obj->table;
    struct EntrySlot *e = &t->entries25C[0];
    u16 *camX;
    u16 *camY;

    camX = (u16 *)&obj->camX;
    camY = (u16 *)&obj->camY;
    place_entry_sprite(e, camX, camY);
    place_entry_sprite(e + 1, camX, camY);
    place_entry_sprite(e + 2, camX, camY);
    place_entry_sprite(e + 3, camX, camY);
}
END_NONMATCH

void unlink_entry_sprites(struct SceneObj *obj)
{
    struct SceneTable *t = obj->table;
    struct SpriteEntry *e;

    e = t->entries25C[0].sprite;
    if ((e->flags12 & 0x20) == 0)
        unlink_d54(e);
    e = t->entries25C[1].sprite;
    if ((e->flags12 & 0x20) == 0)
        unlink_d54(e);
    e = t->entries25C[2].sprite;
    if ((e->flags12 & 0x20) == 0)
        unlink_d54(e);
    e = t->entries25C[3].sprite;
    if ((e->flags12 & 0x20) == 0)
        unlink_d54(e);
}

struct DrawDesc
{
    u8 filler0[0xC];
    u8 *p0C;
    u32 p10;
    u32 p14;
    u32 p18;
};

void draw_desc_with_offset(void *a0, u32 *p1, u32 p2, ByteArg p3, s32 p5, ByteArg p6)
{
    obj_draw_desc_entries(a0, (u8 *)p1[3], p1[4], p1[5], (u32)((u8 *)p1[6] + ((p5 << 8) + 0xF00)), p2, p3, p6);
}

void finish_obj_fc(struct SceneObj *obj)
{
    if (((obj->fc << 21) >> 28) == 0)
        obj_stop_line_scroll(obj->resource);
    ((struct FcHalfBits *)&obj->fc)->field = 0xF;
}

void release_obj_resource_if_idle(struct SceneObj *obj)
{
    if (((obj->fc << 21) >> 28) == 0)
        map_obj_init_display_b(obj->resource);
}

void reset_obj_fc(struct SceneObj *obj, s32 flag)
{
    if (flag == 0)
    {
        map_obj_init_display(obj->resource);
        ((struct FcHalfBits *)&obj->fc)->field = 0;
    }
}

void clear_208_bit1_set_351_bit6(struct SceneObj *obj)
{
    ((struct Bit1Bits *)&obj->flags208)->bit1 = 0;
    ((struct KeyFlagBits *)&obj->subA->flags351)->bit6 = 1;
}

void set_208_bit1_copy_100(struct SceneObj *obj)
{
    ((struct Bit1Bits *)&obj->flags208)->bit1 = 1;
    obj->subA->half23E = obj->half100;
    ((struct KeyFlagBits *)&obj->subA->flags351)->bit6 = 0;
}

void reset_slot_50(void)
{
    set_slot_value(0x50, -1);
}

void set_0d80_slot(void (*fn)(void), u32 slot)
{
    if (gUnk_03000D80[slot] != fn)
    {
        REG_IME = 0;
        gUnk_03000D80[slot] = fn;
        REG_IME = 1;
    }
}

void reset_0d80_table(void)
{
    void (*handler)(void) = call_0d80_handlers;
    void (**table)(void) = gUnk_03000D80;
    s32 i;

    for (i = 0; i < 2; i++)
        table[i] = 0;
    set_irq_callback(2, handler);
}

void call_0d80_handlers(void)
{
    if (gUnk_03000D80[0] != 0)
        gUnk_03000D80[0]();
    if (gUnk_03000D80[1] != 0)
        gUnk_03000D80[1]();
}

ASM_FUNC("asm/code_080286C0/init_obj_slots.s", void init_obj_slots(void))

ASM_FUNC("asm/code_080286C0/pair_set_mode.s", void pair_set_mode(void))
