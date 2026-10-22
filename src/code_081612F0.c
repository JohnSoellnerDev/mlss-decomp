#include "global.h"
struct ListHeadX
{
    u8 filler0[0x3C];
    u32 head3C;
};


struct TileObj
{
    u8 tilemap[0x800];
    u8 unk800[4];
    void *tiles804;
    u16 *tiles808;
    void *tiles80C;
    u32 unk810;
    u8 filler814[0x820 - 0x814];
    u16 tileWidth;
};

struct EffectObj
{
    u32 unk00;
    s32 unk04;
    s32 unk08;
    void *unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
};

struct EffectChildTable
{
    u8 filler0[0x10];
    s16 ofs10;
    u16 filler12;
    void (*fn14)(void *self, void *pos);
    s16 ofs18;
    u16 filler1A;
    void (*fn1C)(void *self, u32 mode);
    u8 filler20[0x8];
    s16 ofs28;
    u16 filler2A;
    void (*fn2C)(void *self, u32 mode);
    s16 ofs30;
    u16 filler32;
    void (*fn34)(void *self, void *pos);
    s16 ofs38;
    u16 filler3A;
    void (*fn3C)(void *self);
    s16 ofs40;
    u16 filler42;
    void (*fn44)(void *self);
};

struct EffectChild
{
    u8 filler0[0x2C];
    struct EffectChildTable *table;
};

struct EffectWorld
{
    u8 filler0[0x24];
    s32 posX;
    s32 posY;
    struct TileObj *tilesA;
    struct TileObj *tilesB;
    u8 filler34[0x3C - 0x34];
    struct EffectChild *child;
};

struct ObjVtable
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*update)(void *self, u32 mode);
};

struct ChildObj
{
    u8 filler0[0xC];
    struct ObjVtable *vtable;
};

extern u32 gClassEa88[];
extern u32 gClassEa60[];
extern u32 gClassEa78[];
extern u32 gClassE9e8[];
extern const s16 gSineTable[];

struct EffectC
{
    struct SpriteXY6 *sprite;
    s32 angle;
    s32 state;
    s32 timer;
};

struct SpriteXY6
{
    u8 filler0[4];
    s16 x;
    s16 y;
};
extern u32 gEffectStateClass[];
extern u32 gUnknownEffectStateTable[];
extern u32 gFrameUpdateClass[];
extern u32 gHeapTagListB[];
extern u32 gHeapTagOtkdB[];
extern u32 gHeapTagMt7p[];
extern u16 gUnk_02000050[];
extern u32 gClassE980[];
extern u8 gSys[];
extern struct EffectWorld *gUnk_03001024;
void init_bg_gfx();
struct EffectChild *obj_init_ex(struct EffectObj *obj, void *src, void *parent, s32 extra);
void effect_dma_callback(void);
u32 read_varlen_plus1(const void *p);
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void cpu_fast_set(const void *src, void *dst, u32 ctl);
extern const u8 gUnk_08002D2C[];
extern const u16 gScenePaletteA[];
extern const u8 gBgDecompBufferTag[];
extern u32 gUnk_03000C84;
extern u8 gUnk_02000080[];
extern u8 gUnk_0200000A[];
extern u32 gClassE9a0[];
extern struct EffectWorld *gUnk_03001024;


struct ChildVt4
{
    u8 filler0[4];
    struct ObjVtable *vtable;
};

struct PosPair
{
    s32 x;
    s32 y;
};

struct SpritePos
{
    s16 x;
    s16 y;
};

struct EffectHead
{
    u32 a;
    u32 b;
    u32 c;
};

struct EffectB
{
    u16 timer;
    s16 state;
    u16 ofsX;
    u16 ofsY;
    struct SpritePos *outX;
    struct SpritePos *outY;
    s32 posX;
    s32 posY;
};

void heap_free_a(void *p);
void heap_free_b(void *p);
void task_insert(void *node, u8 kind, void *data);
void release_obj_131c(void *p);
void *heap_alloc2(u32 size, u32 a, void *table, u32 c);
void *obj_list_init(void *p);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void obj_release_slots(void *sprite);
void task_remove(void *node, u32 kind);
s16 view_coord(struct TileObj *obj, s32 pos);
u32 arc_tan2(s32 x, s32 y);
u32 obj_classify_row(struct TileObj *obj, u32 index);
u32 classify_tile_code(void *obj, u32 tile, s32 row);
struct EffectObj *obj_init_class_ea30(struct EffectObj *obj);
void *lookup_or_alloc_13b4(s32 a, s32 b, s32 c, s32 d);
void *find_or_alloc_obj_slots(s32 a, s32 b, s32 c, s32 d, s32 e, void *f, s32 g);
void set_irq_callback(u32 kind, void *cb);
void obj_setup_tiles(void *obj, const void *src, u32 w, u32 h, u32 x, u32 y, void *vram, u32 extra);
void obj_scroll_tile_ring(struct TileObj *obj, s32 x, s32 y);
extern u32 gEffectTilesATag[];
extern u32 gEffectTilesBTag[];
extern u32 gEffectChildTag[];
extern u32 gEffectChildInit[];
extern u16 gUnk_02000014[];

ASM_FUNC("asm/code_081612F0/classify_tile_code.s", u32 classify_tile_code(void *obj, u32 tile, s32 row))

ASM_FUNC("asm/code_081612F0/obj_classify_row.s", u32 obj_classify_row(struct TileObj *obj, u32 index))

s16 view_coord(struct TileObj *obj, s32 pos)
{
    s32 row = pos >> 8;
    u32 entry;

    entry = ((u32 *)obj->tiles80C)[(s16)(((u32)row >> 3) % obj->tileWidth)];
    switch (((s32)((u8 *)&entry)[1] << 28) >> 28)
    {
    case 0:
        return ((s8 *)&entry)[0] * 8;
    case 1:
        {
            s32 t = ((s8 *)&entry)[0] * 8;

            return (s16)(t + row % 8);
        }
    case 2:
        {
            s32 t = ((s8 *)&entry)[0] * 8 + 8;

            return (s16)(t - row % 8);
        }
    case 3:
        {
            s32 t = ((s8 *)&entry)[0] * 8;
            s32 h = row % 8 / 2 - 8;

            return (s16)(t - h);
        }
    case 4:
        {
            s32 t = ((s8 *)&entry)[0] * 8 + 4;

            return (s16)(t - row % 8 / 2);
        }
    case 5:
        {
            s32 t = ((s8 *)&entry)[0] * 8;

            return (s16)(t + row % 8 / 2);
        }
    case 6:
        {
            s32 t = ((s8 *)&entry)[0] * 8;
            s32 h = row % 8 / 2 + 4;

            return (s16)(t + h);
        }
    }
    return ((s8 *)&entry)[0] * 8;
}

ASM_FUNC("asm/code_081612F0/scan_special_run.s", void scan_special_run(void))

s32 tile_step_by_mode(struct TileObj *obj, s32 pos)
{
    u32 entry;
    s32 mode;

    entry = ((u32 *)obj->tiles80C)[(s16)(((u32)(pos >> 8) >> 3) % obj->tileWidth)];
    mode = ((s32)((u8 *)&entry)[1] << 28) >> 28;
    switch (mode)
    {
    case 0:
        return 0;
    case 1:
        return 0x20;
    case 2:
        return -0x20;
    case 3:
    case 4:
        return -0x10;
    case 5:
    case 6:
        return 0x10;
    }
    return 0;
}

u8 view_angle(struct TileObj *obj, s32 x, s32 y)
{
    u16 rowX = view_coord(obj, x);
    s16 rowY = view_coord(obj, y);
    s32 dx = ((y - x) << 8) >> 16;
    s32 dy = rowY - (s16)rowX;

    return ((u32)arc_tan2(dx, (s16)dy) << 16) >> 24;
}

void obj_build_tile_table(struct TileObj *obj)
{
    u32 *table;
    u32 *entry;
    s32 i;

    for (i = 0; i < obj->tileWidth; i++)
    {
        table = (u32 *)obj->tiles80C;
        entry = (u32 *)(i * 4 + (u32)table);
        *entry = obj_classify_row(obj, i);
    }
}

ASM_FUNC("asm/code_081612F0/scan_classify_rows_down.s", void scan_classify_rows_down(void))

ASM_FUNC("asm/code_081612F0/scan_classify_rows_up.s", void scan_classify_rows_up(void))

u32 tile_col_entry(struct TileObj *obj, u32 col)
{
    return ((u32 *)obj->tiles80C)[(s16)((col >> 3) % obj->tileWidth)];
}

struct DmaChannel3
{
    vu32 sad;
    vu32 dad;
    vu32 cnt;
};

u32 dma3_copy_tilemap(struct TileObj *obj)
{
    struct DmaChannel3 *dma = (struct DmaChannel3 *)REG_ADDR_DMA3SAD;

    dma->sad = (u32)obj;
    dma->dad = obj->unk810;
    dma->cnt = 0x84000200;
    return dma->cnt;
}

void obj_free_tile_bufs(struct TileObj *obj)
{
    if (obj->tiles804 != NULL)
    {
        heap_free_a(obj->tiles804);
        obj->tiles804 = NULL;
    }
    if (obj->tiles80C != NULL)
    {
        heap_free_a(obj->tiles80C);
        obj->tiles80C = NULL;
    }
}

void obj_free_tiles(struct TileObj *obj, u32 flags)
{
    obj_free_tile_bufs(obj);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct TileObj *obj_clear_tile_ptrs(struct TileObj *obj)
{
    obj->tiles804 = NULL;
    obj->tiles80C = NULL;
    return obj;
}

extern u32 gClassE980[];

struct ObjVtableB
{
    u8 filler0[0x10];
    s16 thisOffset;
    u16 filler12;
    u32 (*fn)(void *self, void *arg);
};

static inline void obj_task_teardown(struct EffectObj *obj)
{
    struct ChildVt4 *child;

    obj->unk18 = (s32)gClassE980;
    if (obj->unk1C != 0)
    {
        task_remove((void *)obj->unk1C, 3);
    }
    child = (struct ChildVt4 *)obj->unk20;
    if (child != NULL)
    {
        child->vtable->update((u8 *)child + child->vtable->thisOffset, 3);
    }
    task_remove(obj, 3);
}

void obj_task_update(struct EffectObj *obj)
{
    u16 *flags = (u16 *)gSys;
    struct ChildVt4 *child;
    struct ObjVtableB *vb;

    if ((flags[0x15] & 4) != 0)
    {
        if (obj == NULL)
        {
            return;
        }
        obj_task_teardown(obj);
        return;
    }
    child = (struct ChildVt4 *)obj->unk20;
    vb = (struct ObjVtableB *)child->vtable;
    if (vb->fn((u8 *)child + vb->thisOffset, obj) == 0)
    {
        return;
    }
    if (obj == NULL)
    {
        return;
    }
    obj_task_teardown(obj);
}

extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
void update_obj_list(void);
void obj_list_pass_0a8c(void);
void update_objects_frame(void)
{
    u8 *base;
    u32 *oam_slot;
    struct ListHeadX *list;
    u32 old;

    base = (u8 *)&gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    update_obj_list();
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHeadX *)gUnk_0203FFB8)->head3C,
        ((struct ListHeadX *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHeadX *)gUnk_0203FFB8;
    old = ((u32 (*)(u32, u32))gUnk_03000D4C)(list->head3C, old);
    *oam_slot = old;
}

void effect_release_cls980(struct EffectObj *obj, u32 kind)
{
    struct ChildVt4 *child;

    obj->unk18 = (s32)gClassE980;
    if (obj->unk1C != 0)
    {
        task_remove((void *)obj->unk1C, 3);
    }
    child = (struct ChildVt4 *)obj->unk20;
    if (child != NULL)
    {
        child->vtable->update((u8 *)child + child->vtable->thisOffset, 3);
    }
    task_remove(obj, kind);
}

struct EffectObj *effect_ctor_cls980(struct EffectObj *obj, u8 kind, void *data)
{
    struct EffectObj *child;
    vu16 *p;

    task_insert(obj, kind, data);
    obj->unk18 = (s32)gClassE980;
    do
    {
        child = heap_alloc2(0x1C, 1, gHeapTagOtkdB, 0);
        task_insert(child, 8, gHeapTagOtkdB);
        child->unk18 = (s32)gFrameUpdateClass;
        obj->unk1C = (s32)child;
        child->unk14 = (s32)obj;
        obj->unk20 = (s32)obj_init_class_ea30(heap_alloc2(8, 1, gHeapTagMt7p, 0));
        obj->unk44 = obj->unk40 = obj->unk28 = obj->unk24 = 0;
        p = (vu16 *)gUnk_02000050;
        *p = 0xDF;
        p += 2;
        *p = 0x1F;
    } while (0);
    return obj;
}

struct SysBgDirty
{
    u8 filler0[2];
    u16 bgPalDirty;
};

NONMATCH("asm/code_081612F0/init_bg_gfx.s", void init_bg_gfx())
{
    const u8 *packed = gUnk_08002D2C + 0x81F4;
    u8 *dst = heap_alloc(read_varlen_plus1(packed), 1, gBgDecompBufferTag, 0);
    vu16 *bg;

    ((void (*)(const void *, void *))gUnk_03000C84)(packed, dst);
    cpu_fast_set(dst, (void *)0x06000000, 0x2000);
    if (dst != NULL)
    {
        heap_free_a(dst);
    }
    cpu_fast_set(gScenePaletteA, gUnk_02000080, 0x80);
    *(u16 *)gUnk_02000080 = 0x190;
    bg = (vu16 *)gUnk_0200000A;
    *bg = 0x1701;
    bg += 2;
    *bg = 0x1D02;
    *(vu16 *)0x02000000 |= 0xA00;
    ((struct SysBgDirty *)gSys)->bgPalDirty |= 0x7FFF;
}
END_NONMATCH

u32 pos_in_view_box(struct EffectObj *obj, struct EffectObj *other)
{
    if (other->unk14 < obj->unk24 + -0x2000)
    {
        return 0;
    }
    if (other->unk14 > obj->unk24 + 0x11000)
    {
        return 0;
    }
    if (other->unk18 < obj->unk28)
    {
        return 0;
    }
    if (other->unk18 > obj->unk28 + 0xA000)
    {
        return 0;
    }
    return 1;
}

void *obj_add_task(struct EffectObj *obj, u8 kind, void *data)
{
    task_insert(obj, kind, data);
    obj->unk18 = (s32)gFrameUpdateClass;
    return obj;
}

void obj_step_clamped(struct EffectObj *obj)
{
    s32 limit;

    obj->unk14 += obj->unk24;
    obj->unk18 += obj->unk28 + obj->unk10;
    limit = view_coord((struct TileObj *)obj->unk30, obj->unk14) << 8;
    if (obj->unk18 > limit)
    {
        obj->unk18 = limit;
    }
}

void stub_nop_08161ff8(void)
{
}

NONMATCH("asm/code_081612F0/obj_init_ex.s", struct EffectChild *obj_init_ex(struct EffectObj *obj, void *src, void *parent, s32 extra))
{
    s32 tmp;
    s32 x;
    s32 hi;
    u16 lo;

    obj->unk2C = (s32)gClassE9e8;
    obj->unk10 = extra;
    *(struct EffectHead *)obj = *(struct EffectHead *)src;
    obj->unk0C = 0;
    obj->unk14 = obj->unk1C = ((s16 *)obj)[0] << 8;
    obj->unk18 = obj->unk20 = ((s16 *)obj)[1] << 8;
    obj->unk24 = 0;
    obj->unk28 = 0;
    lookup_or_alloc_13b4(0, ((s16 *)obj)[3], -1, 1);
    x = ((s16 *)obj)[3];
    hi = (x >> 12) & 0xf;
    lo = x & 0xfff;
    find_or_alloc_obj_slots(0, hi << 12 | lo, 0, 0, 0, &tmp, -1);
    obj->unk2C = (s32)gClassE9a0;
    obj->unk30 = (s32)parent;
    return (struct EffectChild *)obj;
}
END_NONMATCH

void obj_effect_child_method(struct EffectObj *obj)
{
    obj->unk24 = obj->unk14 - obj->unk1C;
    obj->unk28 = obj->unk18 - obj->unk20;
    obj->unk1C = obj->unk14;
    obj->unk20 = obj->unk18;
}

void obj_advance_pos(struct EffectObj *obj)
{
    obj->unk14 += obj->unk24;
    obj->unk18 += obj->unk28 + obj->unk10;
}

void obj_set_pos_pair(struct EffectObj *obj, u32 *src)
{
    obj->unk14 = obj->unk1C = src[0];
    obj->unk18 = obj->unk20 = src[1];
}

void obj_sprite_pos_rel_cam(struct EffectObj *obj, struct PosPair *cam)
{
    ((struct SpritePos *)obj->unk0C)->x = (obj->unk14 - cam->x) >> 8;
    ((struct SpritePos *)obj->unk0C)->y = (obj->unk18 - cam->y) >> 8;
}

void sprites_obj_refresh_sprite(struct EffectObj *obj, u32 flag)
{
    u8 *spr = obj->unk0C;

    if (!(spr[0x29] & 0x40))
    {
        obj_load_record(spr, *(s8 *)&obj->unk08, -1, 0, 0);
    }
    if (flag != 0)
    {
        sprite_update_cd0(obj->unk0C);
    }
    else
    {
        obj_release_slots(obj->unk0C);
    }
}

void obj_release_child_131c(struct EffectObj *obj)
{
    release_obj_131c(obj->unk0C);
    obj->unk0C = NULL;
}

void obj_create_child(struct EffectObj *obj, u32 arg)
{
    struct SpritePos *child;

    child = create_obj_de4(0, *(s16 *)((u8 *)obj + 6), arg, -1, -1, -1, -1);
    obj->unk0C = child;
    child->x = obj->unk14 >> 8;
    ((struct SpritePos *)obj->unk0C)->y = obj->unk18 >> 8;
}

s32 stub_return_zero_08162174(void)
{
    return 0;
}

void obj_release_both(struct EffectObj *obj, u32 flags)
{
    obj->unk2C = (s32)gClassE9e8;
    if (obj->unk0C != NULL)
    {
        obj_release_child_131c(obj);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct EffectObj *effect_sub_init_cls9e8(struct EffectObj *obj, struct EffectHead *src, s32 arg)
{
    s32 tmp;
    s32 x;
    s32 hi;
    u16 lo;

    obj->unk2C = (s32)gClassE9e8;
    obj->unk10 = arg;
    *(struct EffectHead *)obj = *src;
    obj->unk0C = 0;
    obj->unk14 = obj->unk1C = ((s16 *)obj)[0] << 8;
    obj->unk18 = obj->unk20 = ((s16 *)obj)[1] << 8;
    obj->unk24 = 0;
    obj->unk28 = 0;
    lookup_or_alloc_13b4(0, ((s16 *)obj)[3], -1, 1);
    x = ((s16 *)obj)[3];
    hi = (x >> 12) & 0xf;
    lo = x & 0xfff;
    find_or_alloc_obj_slots(0, hi << 12 | lo, 0, 0, 0, &tmp, -1);
    return obj;
}

NONMATCH("asm/code_081612F0/effect_step.s", s32 effect_step(struct EffectObj *effect, struct EffectWorld *owner))
{
    u32 size = sizeof(struct TileObj);
    u32 state = effect->unk00;
    struct EffectChildTable *table;
    vu16 *p;
    void *pos;
    s32 x;
    s32 y;

    switch (state)
    {
    case 0:
    {
        gUnk_03001024 = owner;
        owner->tilesA = obj_clear_tile_ptrs(heap_alloc2(size, 1, gEffectTilesATag, 0));
        obj_setup_tiles(owner->tilesA, gUnk_08002D2C + 4, 0xE0, 0x20, state, state, (void *)0x0600B800, state);
        owner->tilesB = obj_clear_tile_ptrs(heap_alloc2(size, 1, gEffectTilesBTag, 0));
        obj_setup_tiles(owner->tilesB, gUnk_08002D2C + 0x2F34, 0xE0, 0x20, state, state, (void *)0x0600E800, state);
        init_bg_gfx(owner);
        owner->child = obj_init_ex(heap_alloc2(0x34, 1, gEffectChildTag, 0), gEffectChildInit, owner->tilesA, 0x14);
        table = owner->child->table;
        table->fn1C((u8 *)owner->child + table->ofs18, 1);
        table = owner->child->table;
        table->fn2C((u8 *)owner->child + table->ofs28, 1);
        p = gUnk_02000050;
        *p = state;
        p += 2;
        *p = state;
        set_irq_callback(2, effect_dma_callback);
        effect->unk00++;
    }
        break;
    case 1:
    {
        table = owner->child->table;
        table->fn14((u8 *)owner->child + table->ofs10, pos = &owner->posX);
        table = owner->child->table;
        table->fn3C((u8 *)owner->child + table->ofs38);
        table = owner->child->table;
        table->fn44((u8 *)owner->child + table->ofs40);
        table = owner->child->table;
        table->fn34((u8 *)owner->child + table->ofs30, pos);
        x = owner->posX >> 8;
        y = owner->posY >> 8;
        obj_scroll_tile_ring(owner->tilesA, x, y);
        obj_scroll_tile_ring(owner->tilesB, x, y);
        p = gUnk_02000014;
        *p = x;
        p++;
        *p = y;
        p += 3;
        *p = x;
        p++;
        *p = y;
    }
        break;
    }
    return 0;
}
END_NONMATCH

void obj_set_class_ea60(struct EffectObj *obj, u32 flags)
{
    obj->unk04 = (u32)gClassEa60;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct EffectObj *obj_init_class_ea30(struct EffectObj *obj)
{
    obj->unk00 = 0;
    obj->unk04 = (u32)gEffectStateClass;
    return obj;
}

void stub_nop_081623c4(void)
{
}

void obj_set_class_ea60_b(struct EffectObj *obj, u32 flags)
{
    obj->unk04 = (u32)gClassEa60;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_init_class_ea48(struct EffectObj *obj)
{
    obj->unk00 = 0;
    obj->unk04 = (u32)gUnknownEffectStateTable;
}

s32 stub_return_zero_081623f8(void)
{
    return 0;
}

void obj_set_class_ea60_c(struct EffectObj *obj, u32 flags)
{
    obj->unk04 = (u32)gClassEa60;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_init_class_ea60(struct EffectObj *obj)
{
    obj->unk04 = (u32)gClassEa60;
    obj->unk00 = 0;
}

void effect_dma_callback(void)
{
    dma3_copy_tilemap(gUnk_03001024->tilesA);
    dma3_copy_tilemap(gUnk_03001024->tilesB);
}

void obj_set_class_ea78(struct EffectObj *obj, u32 flags)
{
    struct ChildObj *child;

    obj->unk04 = (u32)gClassEa78;
    child = (struct ChildObj *)obj->unk00;
    if (child != NULL)
    {
        child->vtable->update((u8 *)child + child->vtable->thisOffset, 3);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void *init_cls_ea78(struct EffectObj *obj)
{
    obj->unk04 = (u32)gClassEa78;
    obj->unk00 = (u32)obj_list_init(heap_alloc2(0x10, 1, gHeapTagListB, 0));
    return obj;
}

void stub_nop_081624b0(void)
{
}

void obj_set_class_ea88(struct EffectObj *obj, u32 flags)
{
    obj->unk00 = (u32)gClassEa88;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_set_class_ea88_keep(struct EffectObj *obj)
{
    obj->unk00 = (u32)gClassEa88;
}

struct SpriteEntry
{
    void *sprite;
    s32 x;
    s32 y;
    s32 unkC;
    s32 unk10;
    s32 vx;
    s32 vy;
};

struct SpriteEntries
{
    struct SpriteEntry entries[2];
};

s32 effect_init_sprites(void *p)
{
    struct SpriteEntries *obj = p;
    void *sprite;
    s32 i;

    obj->entries[1].sprite = (void *)0x7800;
    obj->entries[1].x = 0x5000;
    obj->entries[1].y = 0;
    obj->entries[1].unkC = 0;
    obj->entries[1].unk10 = 0x12C;
    obj->entries[1].vx = 0;
    i = 0;
    do
    {
        sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
        obj->entries[i].sprite = sprite;
        switch (gSys[0x8C0])
        {
        case 0:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x39, -1, 0, 0);
            break;
        case 1:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x3A, -1, 0, 0);
            break;
        case 2:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x3B, -1, 0, 0);
            break;
        case 3:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x3C, -1, 0, 0);
            break;
        case 4:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x3D, -1, 0, 0);
            break;
        default:
            obj_load_record(sprite, i * 6 + 0x3E, -1, 0, 0);
            break;
        }
        sprite_update_cd0(obj->entries[i].sprite);
        obj->entries[i].x = (s32)obj->entries[1].sprite;
        obj->entries[i].y = 0xFFFFD800;
        obj->entries[i].vx = 0;
        obj->entries[i].vy = 0;
        obj->entries[i].unkC = 0;
        obj->entries[i].unk10 = 0;
        ((struct SpritePos *)obj->entries[i].sprite)->x = (obj->entries[i].x + obj->entries[i].vx) >> 8;
        ((struct SpritePos *)obj->entries[i].sprite)->y = (obj->entries[i].y + obj->entries[i].vy) >> 8;
        i++;
    } while (i <= 0);
    return (s32)obj;
}

NONMATCH("asm/code_081612F0/effect_step_a.s", s32 effect_step_a(struct EffectObj *obj))
{
    s32 delta;

    switch (obj->unk30)
    {
    case 0:
        delta = obj->unk10 + 0x14;
        obj->unk10 = delta;
        obj->unk08 += delta;
        if (obj->unk08 > 0x4FFF)
        {
            obj->unk10 = (-delta * 2) / 3;
            if ((obj->unk10 >> 8) == 0)
                goto finish;
        }
        ((struct SpritePos *)obj->unk00)->x = (obj->unk04 + obj->unk14) >> 8;
        ((struct SpritePos *)obj->unk00)->y = (obj->unk08 + obj->unk18) >> 8;
        break;
    case 1:
        if (--obj->unk2C <= 0)
            return 1;
        break;
    }
    return 0;

finish:
    obj->unk2C = 0x50;
    obj->unk30 = 1;
    return 0;
}
END_NONMATCH

void obj_release_child_free(struct EffectObj *obj, u32 flags)
{
    release_obj_131c((void *)obj->unk00);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

s32 effect_step_b(struct EffectB *obj)
{
    switch (obj->state)
    {
    case 0:
        break;
    case 1:
        obj->timer--;
        if ((s16)obj->timer < 0)
        {
            obj->timer = 0x50;
            obj->state++;
        }
        obj->posX += 1000;
        obj->posY += -1000;
        obj->outX->x = (obj->posX >> 8) + obj->ofsX;
        obj->outY->x = (obj->posY >> 8) + obj->ofsY;
        break;
    case 2:
        obj->timer--;
        if ((s16)obj->timer > 0)
            return 0;
        return 1;
    }
    return 0;
}

struct SpritePair
{
    u16 width;
    u16 height;
    u16 unk4;
    u16 unk6;
    void *sprites[2];
    s32 posY[2];
};

void effect_init_sprite_pair(void *p)
{
    struct SpritePair *obj = p;
    void *sprite;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
        obj->sprites[i] = sprite;
        switch (gSys[0x8C0])
        {
        case 0:
            obj_load_record(obj->sprites[i], i * 6 + 0x2E, -1, 0, 0);
            break;
        case 1:
            obj_load_record(obj->sprites[i], i * 6 + 0x2F, -1, 0, 0);
            obj->unk4 = 0xFFFC;
            obj->unk6 = 4;
            break;
        case 2:
            obj_load_record(obj->sprites[i], i * 6 + 0x30, -1, 0, 0);
            obj->unk4 = 0xFFFC;
            obj->unk6 = 4;
            break;
        case 3:
            obj_load_record(obj->sprites[i], i * 6 + 0x31, -1, 0, 0);
            obj->unk4 = 0xFFFE;
            obj->unk6 = 2;
            break;
        case 4:
            obj_load_record(obj->sprites[i], i * 6 + 0x32, -1, 0, 0);
            obj->unk4 = 0xFFFE;
            obj->unk6 = 2;
            break;
        default:
            obj_load_record(sprite, i * 6 + 0x2D, -1, 0, 0);
            break;
        }
        sprite_update_cd0(obj->sprites[i]);
        obj->posY[i] = (i * 15) << 12;
        ((struct SpritePos *)obj->sprites[i])->x = obj->posY[i] >> 8;
        ((struct SpritePos *)obj->sprites[i])->y = 0x50;
    }
    obj->width = 0x1E;
    obj->height = 1;
}

void obj_release_two_children(struct EffectObj *obj, u32 flags)
{
    if (obj->unk08 != 0)
    {
        release_obj_131c((void *)obj->unk08);
    }
    if (obj->unk0C != NULL)
    {
        release_obj_131c(obj->unk0C);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

NONMATCH("asm/code_081612F0/obj_clear_bounds.s", struct EffectB *obj_clear_bounds(struct EffectB *obj))
{
    s32 *upper;

    obj->outX = 0;
    upper = (s32 *)&obj->outX;
    upper[2] = 0;
    obj->outY = 0;
    upper++;
    upper[2] = 0;
    obj->timer = 0;
    obj->state = 0;
    obj->ofsX = 0;
    obj->ofsY = 0;
    return obj;
}
END_NONMATCH

struct EffectObj *effect_create_sprites(struct EffectObj *obj)
{
    void *sprite;
    s32 y;

    sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
    obj->unk00 = (u32)sprite;
    switch (gSys[0x8C0])
    {
    case 0:
        obj_load_record((void *)obj->unk00, 0x22, -1, 0, 0);
        break;
    case 1:
        obj_load_record((void *)obj->unk00, 0x23, -1, 0, 0);
        break;
    case 2:
        obj_load_record((void *)obj->unk00, 0x24, -1, 0, 0);
        break;
    case 3:
        obj_load_record((void *)obj->unk00, 0x25, -1, 0, 0);
        break;
    case 4:
        obj_load_record((void *)obj->unk00, 0x26, -1, 0, 0);
        break;
    default:
        obj_load_record(sprite, 0x21, -1, 0, 0);
        break;
    }
    sprite_update_cd0((void *)obj->unk00);
    ((struct SpritePos *)obj->unk00)->x = 0x78;
    ((struct SpritePos *)obj->unk00)->y = y = 0x50;
    obj->unk0C = NULL;
    obj->unk04 = y + 0xB0;
    obj->unk08 = 0;
    return obj;
}

static inline void effect_c_wave(struct EffectC *obj, s32 phase)
{
    obj->sprite->x = (gSineTable[phase & 0xFF] / 64) * 2;
    obj->sprite->y = (gSineTable[phase & 0xFF] / 64) * 2;
}

u32 effect_step_c(struct EffectC *obj)
{
    s32 phase = obj->angle >> 8;

    switch (obj->state)
    {
    case 0:
        effect_c_wave(obj, phase);
        obj->angle += 0x200;
        if (phase > 0x40)
        {
            obj->state++;
        }
        break;
    case 1:
        effect_c_wave(obj, phase);
        obj->angle += 0x400;
        if (phase > 0x60)
        {
            obj->timer = 0x50;
            obj->state++;
        }
        break;
    case 2:
        obj->timer -= 1;
        if (obj->timer > 0)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

void obj_release_child_free_b(struct EffectObj *obj, u32 flags)
{
    release_obj_131c((void *)obj->unk00);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

s32 effect_init_sprites_b(void *p)
{
    struct SpriteEntries *obj = p;
    void *sprite;
    s32 i;

    obj->entries[1].sprite = (void *)0x7800;
    obj->entries[1].x = 0x5000;
    obj->entries[1].y = 0;
    obj->entries[1].unkC = 0;
    obj->entries[1].unk10 = 0x12C;
    obj->entries[1].vx = 0;
    i = 0;
    do
    {
        sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
        obj->entries[i].sprite = sprite;
        switch (gSys[0x8C0])
        {
        case 0:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x4B, -1, 0, 0);
            break;
        case 1:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x4C, -1, 0, 0);
            break;
        case 2:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x4D, -1, 0, 0);
            break;
        case 3:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x4E, -1, 0, 0);
            break;
        case 4:
            obj_load_record(obj->entries[i].sprite, i * 6 + 0x4F, -1, 0, 0);
            break;
        default:
            obj_load_record(sprite, i * 6 + 0x4A, -1, 0, 0);
            break;
        }
        sprite_update_cd0(obj->entries[i].sprite);
        obj->entries[i].x = (s32)obj->entries[1].sprite;
        obj->entries[i].y = 0xFFFFD800;
        obj->entries[i].vx = 0;
        obj->entries[i].vy = 0;
        obj->entries[i].unkC = 0;
        obj->entries[i].unk10 = 0;
        ((struct SpritePos *)obj->entries[i].sprite)->x = (obj->entries[i].x + obj->entries[i].vx) >> 8;
        ((struct SpritePos *)obj->entries[i].sprite)->y = (obj->entries[i].y + obj->entries[i].vy) >> 8;
        i++;
    } while (i <= 0);
    return (s32)obj;
}
