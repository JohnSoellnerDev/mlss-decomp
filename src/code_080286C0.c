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
    u8 filler0F4[0xF8 - 0xF4];
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
    u8 filler247[0x27A - 0x247];
    u16 unk27A;
    u16 unk27C;
    s16 camX;
    s16 camY;
    u8 filler282[0x286 - 0x282];
    u16 unk286;
    u16 unk288;
    u8 filler28A[0x2AC - 0x28A];
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

extern u8 gUnk_03000C24;
extern u32 gUnk_03000C78;
extern u8 gUnk_02000080[];
extern const u8 gTagBtl[];
extern const u8 gTagEndd[];
extern const u16 gLookup13b4Ids[];
extern u8 gUnk_0300034C[];
extern u16 gUnk_02000008;
extern u32 gUnk_03000E00;
extern u32 gUnk_03000E04;
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void poll_keys(void);
void obj_draw_desc_entries(void *a0, u8 *p1, u32 p2, u32 p3, u32 x5, u32 p6, u8 p7, u8 p8);
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

ASM_FUNC("asm/code_080286C0/apply_anim_state.s", void apply_anim_state(void))

NONMATCH("asm/code_080286C0/sprites_obj_set_move_target.s", void sprites_obj_set_move_target(struct SceneObj *obj, u32 mode, u32 rel, s32 tx, s32 ty, u32 speed))
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

        obj->unk2FC = (u16)(((dist << 8) / (s16)spd) + 1);
    }
    else
    {
        obj->unk2FC = spd;
    }
    obj->unk2FA = 0;
    obj->unk2FE = arc_tan2((s16)(obj->unk2EE - obj->unk2F4), (s16)(obj->unk2F2 - obj->unk2EC)) >> 8;
    ((struct PendingBits *)&obj->pending)->low2 = mode;
}
END_NONMATCH

ASM_FUNC("asm/code_080286C0/sprites_obj_step_move.s", void sprites_obj_step_move(void))

ASM_FUNC("asm/code_080286C0/apply_key_filter.s", void apply_key_filter(void))

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

ASM_FUNC("asm/code_080286C0/obj_has_pending_bits.s", void obj_has_pending_bits(void))

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
    gUnk_03000C24 = 1;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = 0x0800063C;
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
        gUnk_03000C78 = 0x0800063C;
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
    gUnk_03000C24 = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = 0x0800063C;
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

ASM_FUNC("asm/code_080286C0/set_obj_fc_state.s", void set_obj_fc_state(void))

ASM_FUNC("asm/code_080286C0/call_7380_for_state.s", void call_7380_for_state(void))

ASM_FUNC("asm/code_080286C0/sync_and_draw_obj.s", void sync_and_draw_obj(void))

struct BdObj
{
    u8 filler0[6];
    s16 unk6;
};

void clear_bd4_bit6(struct BdObj *obj)
{
    u8 *p = gUnk_0300034C;

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
    gUnk_03000C78 = 0x0800063C;
}

void dma_copy_or_queue(const void *src, void *dst, u32 size, u32 a, u32 b);

void queue_tile_dmas(struct SceneObj *obj)
{
    dma_copy_or_queue(obj->table->unk0->data, (void *)0x06010400, 0x4C0, 0x20, 0);
    dma_copy_or_queue(obj->table->unk0C->data, (void *)0x060108C0, 0x500, 0x20, 0);
}

ASM_FUNC("asm/code_080286C0/load_table_14b8.s", void load_table_14b8(void))

ASM_FUNC("asm/code_080286C0/load_table_13b4.s", void load_table_13b4(void))

ASM_FUNC("asm/code_080286C0/update_entry_sprites.s", void update_entry_sprites(void))

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

ASM_FUNC("asm/code_080286C0/draw_desc_with_offset.s", void draw_desc_with_offset(void))

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

ASM_FUNC("asm/code_080286C0/reset_0d80_table.s", void reset_0d80_table(void))

void call_0d80_handlers(void)
{
    if (gUnk_03000D80[0] != 0)
        gUnk_03000D80[0]();
    if (gUnk_03000D80[1] != 0)
        gUnk_03000D80[1]();
}

ASM_FUNC("asm/code_080286C0/init_obj_slots.s", void init_obj_slots(void))

ASM_FUNC("asm/code_080286C0/pair_set_mode.s", void pair_set_mode(void))
