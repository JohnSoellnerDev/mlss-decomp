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

/* Per-column surface entry (obj_build_tile_table): the first tilemap row from the top whose
 * tile is ground, and what shape its top has. */
struct RowClass
{
    u32 row : 8;   /* 0-31, or 32 when the column has no ground */
    u32 kind : 4;  /* enum SurfaceKind */
    u32 rest : 20;
};

enum SurfaceKind
{
    SURFACE_SOLID = 0,            /* flat-topped ground, or no ground at all */
    SURFACE_SLOPE = 1,            /* one-tile slope: tiles 0xCF, 0xF0, 0x1EF, 0x210 */
    SURFACE_SLOPE_FLIPPED = 2,
    SURFACE_SLOPE2_B_FLIPPED = 3, /* second tile of a two-tile slope: 0xF2, 0x1A7 */
    SURFACE_SLOPE2_A_FLIPPED = 4, /* first tile of a two-tile slope: 0xF1, 0x1A6 */
    SURFACE_SLOPE2_A = 5,
    SURFACE_SLOPE2_B = 6
};

/* Fields of a struct RowClass held in a u32 `e`, read byte-wise (row sign-extended) */
#define SURFACE_ROW(e) (((s8 *)&(e))[0])
#define SURFACE_KIND(e) (((s32)((u8 *)&(e))[1] << 28) >> 28)

#define TILEMAP_ROWS 32
#define TILE_NUM_MASK 0x3FF /* BG tilemap entry: tile number */
#define TILE_HFLIP 0x400    /* BG tilemap entry: horizontal flip */

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

static inline void *obj_src_lookup(struct EffectObj *obj)
{
    return lookup_or_alloc_13b4(0, ((s16 *)obj)[3], -1, 1);
}
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

/* Scans tilemap column `col` from the top for the first ground tile and returns its row and
 * surface kind (a slope's kind depends on its horizontal flip), or row 32 when there is none. */
u32 obj_classify_row(struct TileObj *obj, u32 col)
{
    struct RowClass result;
    s32 row;

    for (row = 0; row < TILEMAP_ROWS; row++)
    {
        u16 tile = obj->tiles808[obj->tileWidth * row + col];

        switch (tile & TILE_NUM_MASK)
        {
        case 0xA3 ... 0xA6:
        case 0xB2:
        case 0xC0 ... 0xC4:
        case 0x180 ... 0x181:
        case 0x1C0:
        case 0x1C7:
        case 0x1E0 ... 0x1E2:
        case 0x1EA ... 0x1ED:
        case 0x26E:
        case 0x28C ... 0x28D:
            result.kind = SURFACE_SOLID;
            result.row = row;
            return *(u32 *)&result;
        case 0xCF:
        case 0xF0:
        case 0x1EF:
        case 0x210:
            if (tile & TILE_HFLIP)
                result.kind = SURFACE_SLOPE_FLIPPED;
            else
                result.kind = SURFACE_SLOPE;
            result.row = row;
            return *(u32 *)&result;
        case 0xF1:
        case 0x1A6:
            if (tile & TILE_HFLIP)
                result.kind = SURFACE_SLOPE2_A_FLIPPED;
            else
                result.kind = SURFACE_SLOPE2_A;
            result.row = row;
            return *(u32 *)&result;
        case 0xF2:
        case 0x1A7:
            if (tile & TILE_HFLIP)
                result.kind = SURFACE_SLOPE2_B_FLIPPED;
            else
                result.kind = SURFACE_SLOPE2_B;
            result.row = row;
            return *(u32 *)&result;
        }
    }
    result.kind = SURFACE_SOLID;
    result.row = TILEMAP_ROWS;
    return *(u32 *)&result;
}

/* Returns the ground's y in pixels under x = pos (24.8 fixed point): the top row of the column's
 * surface entry (obj_build_tile_table, columns wrap at tileWidth) times 8, plus how far the
 * slope's top has dropped at x within the tile. */
s16 view_coord(struct TileObj *obj, s32 pos)
{
    s32 x = pos >> 8;
    u32 surface;

    surface = ((u32 *)obj->tiles80C)[(s16)(((u32)x >> 3) % obj->tileWidth)];
    switch (SURFACE_KIND(surface))
    {
    case SURFACE_SOLID:
        return SURFACE_ROW(surface) * 8;
    case SURFACE_SLOPE: /* 0 to 7 */
        {
            s32 top = SURFACE_ROW(surface) * 8;

            return (s16)(top + x % 8);
        }
    case SURFACE_SLOPE_FLIPPED: /* 8 to 1 */
        {
            s32 top = SURFACE_ROW(surface) * 8 + 8;

            return (s16)(top - x % 8);
        }
    case SURFACE_SLOPE2_B_FLIPPED: /* 8 to 5 */
        {
            s32 top = SURFACE_ROW(surface) * 8;
            s32 rise = x % 8 / 2 - 8;

            return (s16)(top - rise);
        }
    case SURFACE_SLOPE2_A_FLIPPED: /* 4 to 1 */
        {
            s32 top = SURFACE_ROW(surface) * 8 + 4;

            return (s16)(top - x % 8 / 2);
        }
    case SURFACE_SLOPE2_A: /* 0 to 3 */
        {
            s32 top = SURFACE_ROW(surface) * 8;

            return (s16)(top + x % 8 / 2);
        }
    case SURFACE_SLOPE2_B: /* 4 to 7 */
        {
            s32 top = SURFACE_ROW(surface) * 8;
            s32 drop = x % 8 / 2 + 4;

            return (s16)(top + drop);
        }
    }
    return SURFACE_ROW(surface) * 8;
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

/* Per-frame task of a gClassE980 effect: Select (newly pressed) tears it down at once; otherwise it runs
 * its child's (unk20) update method with obj, and tears it down when that says it's done (nonzero). */
void obj_task_update(struct EffectObj *obj)
{
    u16 *sys = (u16 *)gSys;
    struct ChildVt4 *child;
    struct ObjVtableB *vtable;

    // gSys + 0x2A: the keys newly pressed this frame
    if ((sys[0x15] & SELECT_BUTTON) != 0)
    {
        if (obj == NULL)
        {
            return;
        }
        obj_task_teardown(obj);
        return;
    }
    child = (struct ChildVt4 *)obj->unk20;
    vtable = (struct ObjVtableB *)child->vtable;
    if (vtable->fn((u8 *)child + vtable->thisOffset, obj) == 0)
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

    task_insert(obj, kind, data);
    obj->unk18 = (s32)gClassE980;
    child = heap_alloc2(0x1c, 1, gHeapTagOtkdB, 0);
    task_insert(child, 8, gHeapTagOtkdB);
    child->unk18 = (s32)gFrameUpdateClass;
    obj->unk1C = (s32)child;
    child->unk14 = (s32)obj;
    obj->unk20 = (s32)obj_init_class_ea30(heap_alloc2(8, 1, gHeapTagMt7p, 0));
    obj->unk24 = 0;
    obj->unk28 = 0;
    obj->unk40 = 0;
    obj->unk44 = 0;
    {
        register vu16 *p asm("r1") = (vu16 *)gUnk_02000050;

        *p = 0xdf;
        p += 2;
        *p = 0x1f;
    }
    return obj;
}

struct SysBgDirty
{
    u8 filler0[2];
    u16 bgPalDirty;
};

/* Sets up the effect scene's backgrounds: decompresses the BG tiles (gUnk_08002D2C + 0x81F4) to
   VRAM char block 0, loads the BG palettes into the shadow (gUnk_02000080) with a new backdrop
   colour, points BG1 at screen block 23 and BG3 at block 29, turns both on in the DISPCNT shadow
   and marks BG palettes 0-14 dirty. gUnk_03000C84 is the RAM copy of the decompressor. */
void init_bg_gfx()
{
    const u8 *packed = gUnk_08002D2C + 0x81F4;
    u8 *tiles = heap_alloc(read_varlen_plus1(packed), 1, gBgDecompBufferTag, 0);

    ((void (*)(const void *, void *))gUnk_03000C84)(packed, tiles);
    cpu_fast_set(tiles, (void *)VRAM, 0x8000 / 4);
    if (tiles != NULL)
    {
        heap_free_a(tiles);
    }
    cpu_fast_set(gScenePaletteA, gUnk_02000080, 0x200 / 4);
    *(u16 *)gUnk_02000080 = 0x190; /* backdrop colour */
    {
        /* gUnk_0200000A: the BG1CNT shadow, followed by BG2CNT and BG3CNT */
        register vu16 *bgCnt asm("r1") = (vu16 *)gUnk_0200000A;

        *bgCnt = BGCNT_SCREENBASE(23) | BGCNT_PRIORITY(1);
        bgCnt += 2;
        *bgCnt = BGCNT_SCREENBASE(29) | BGCNT_PRIORITY(2);
    }
    *(vu16 *)0x02000000 |= DISPCNT_BG1_ON | DISPCNT_BG3_ON; /* DISPCNT shadow */
    ((struct SysBgDirty *)gSys)->bgPalDirty |= 0x7FFF;
}

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

struct EffectChild *obj_init_ex(struct EffectObj *obj, void *src, void *parent, s32 extra)
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
    obj_src_lookup(obj);
    x = ((s16 *)obj)[3];
    hi = (x >> 12) & 0xf;
    lo = x & 0xfff;
    find_or_alloc_obj_slots(0, hi << 12 | lo, 0, 0, 0, &tmp, -1);
    obj->unk2C = (s32)gClassE9a0;
    obj->unk30 = (s32)parent;
    return (struct EffectChild *)obj;
}

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

/* Steps the scene's effect task. State 0 sets the scene up: BG1's tilemap layer (tilesA, from
   gUnk_08002D2C + 4 into screen block 23) and BG3's (tilesB, + 0x2F34 into block 29), their gfx,
   a child object (gravity 0x14) that lands on BG1's ground, the fade cleared (BLDCNT and BLDY
   shadows) and the DMA callback. State 1, every frame: runs the child's methods against the
   camera (scene->posX/posY, 24.8 fixed point) and scrolls BG1 and BG3 to it. */
s32 effect_step(struct EffectObj *effect, struct EffectWorld *scene)
{
    struct EffectChildSub { s16 ofs; u16 filler; void (*fn)(void *self); };
    u32 state = effect->unk00;
    struct EffectChildTable *table;
    struct EffectChildSub *method44;
    vu16 *reg;
    void *cam;
    s32 scrollX;
    s32 scrollY;

    switch (state)
    {
    case 0:
        {
            register const u8 *tilemap asm("r1");

            gUnk_03001024 = scene;
            scene->tilesA = obj_clear_tile_ptrs(heap_alloc2(0x824, 1, gEffectTilesATag, 0));
            tilemap = gUnk_08002D2C + 4;
            obj_setup_tiles(scene->tilesA, tilemap, 0xE0, 0x20, state, state, (void *)0x0600B800, state);
            scene->tilesB = obj_clear_tile_ptrs(heap_alloc2(0x824, 1, gEffectTilesBTag, 0));
            tilemap = gUnk_08002D2C + 0x2F34;
            obj_setup_tiles(scene->tilesB, tilemap, 0xE0, 0x20, state, state, (void *)0x0600E800, state);
        }
        init_bg_gfx(scene);
        scene->child = obj_init_ex(heap_alloc2(0x34, 1, gEffectChildTag, 0), gEffectChildInit, scene->tilesA, 0x14);
        table = scene->child->table;
        table->fn1C((u8 *)scene->child + table->ofs18, 1);
        table = scene->child->table;
        table->fn2C((u8 *)scene->child + table->ofs28, 1);
        // BLDCNT and BLDY shadows: no fade
        reg = gUnk_02000050;
        *reg = state;
        reg += 2;
        *reg = state;
        set_irq_callback(2, effect_dma_callback);
        effect->unk00++;
        break;
    case 1:
        table = scene->child->table;
        table->fn14((u8 *)scene->child + table->ofs10, cam = &scene->posX);
        table = scene->child->table;
        table->fn3C((u8 *)scene->child + table->ofs38);
        method44 = (struct EffectChildSub *)((u8 *)scene->child->table + 0x40);
        method44->fn((u8 *)scene->child + method44->ofs);
        table = scene->child->table;
        table->fn34((u8 *)scene->child + table->ofs30, cam);
        scrollX = scene->posX >> 8;
        scrollY = scene->posY >> 8;
        obj_scroll_tile_ring(scene->tilesA, scrollX, scrollY);
        obj_scroll_tile_ring(scene->tilesB, scrollX, scrollY);
        // BG1HOFS/BG1VOFS, then BG3HOFS/BG3VOFS shadows
        reg = gUnk_02000014;
        *reg = scrollX;
        reg++;
        *reg = scrollY;
        reg += 3;
        *reg = scrollX;
        reg++;
        *reg = scrollY;
        break;
    }
    return 0;
}

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

/* A language-specific sprite that drops in from above the screen, bounces and rests
 * (the same layout as struct EffectObj, which effect_step_a steps). */
struct DropInEffect
{
    struct SpriteEntry entries[1]; /* 0x00 */
    s32 restX;                     /* 0x1C */
    s32 restY;                     /* 0x20 */
    s32 unk24;
    s32 unk28;
    s32 timer;                     /* 0x2C */
    s32 state;                     /* 0x30 */
};

/* gSys[SYS_LANG]: the game's language, 0-4 (higher: the default records) */
#define SYS_LANG 0x8C0

/* A sprite dropped from above the screen that bounces on the floor (0x34 bytes, made by
 * effect_init_sprites, moved by effect_step_a). Same layout as SpriteEntries, whose
 * entries[1] really holds the fields from startX on. */
struct DropEffect
{
    struct SpriteEntry sprites[1]; /* unk10: fall speed; vx/vy: offset added on screen */
    s32 startX;                    /* 1/256 px */
    s32 floorY;                    /* 1/256 px */
    s32 unk24;
    s32 unk28;
    s32 timer;                     /* frames to wait after the last bounce */
    s32 phase;                     /* 0: falling, 1: waiting */
};

/* Sets up the bouncing drop effect in `p` (a 0x34-byte buffer): one sprite with the current
 * language's record (0x39 + language, or 0x3E), starting 40 px above the screen at x = 120 px,
 * falling onto y = 80 px. Returns the effect. */
s32 effect_init_sprites(void *p)
{
    struct DropEffect *effect = p;
    void *sprite;
    s32 i;

    effect->startX = 0x7800;
    effect->floorY = 0x5000;
    effect->unk24 = 0;
    effect->unk28 = 0;
    effect->timer = 300;
    effect->phase = 0;
    i = 0;
    do
    {
        sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
        effect->sprites[i].sprite = sprite;
        switch (gSys[SYS_LANG])
        {
        case 0:
            obj_load_record(effect->sprites[i].sprite, i * 6 + 0x39, -1, 0, 0);
            break;
        case 1:
            obj_load_record(effect->sprites[i].sprite, i * 6 + 0x3A, -1, 0, 0);
            break;
        case 2:
            obj_load_record(effect->sprites[i].sprite, i * 6 + 0x3B, -1, 0, 0);
            break;
        case 3:
            obj_load_record(effect->sprites[i].sprite, i * 6 + 0x3C, -1, 0, 0);
            break;
        case 4:
            obj_load_record(effect->sprites[i].sprite, i * 6 + 0x3D, -1, 0, 0);
            break;
        default:
            obj_load_record(sprite, i * 6 + 0x3E, -1, 0, 0);
            break;
        }
        sprite_update_cd0(effect->sprites[i].sprite);
        effect->sprites[i].x = effect->startX;
        effect->sprites[i].y = -0x2800;
        effect->sprites[i].vx = 0;
        effect->sprites[i].vy = 0;
        effect->sprites[i].unkC = 0;
        effect->sprites[i].unk10 = 0;
        ((struct SpritePos *)effect->sprites[i].sprite)->x = (effect->sprites[i].x + effect->sprites[i].vx) >> 8;
        ((struct SpritePos *)effect->sprites[i].sprite)->y = (effect->sprites[i].y + effect->sprites[i].vy) >> 8;
        i++;
    } while (i <= 0);
    return (s32)effect;
}

s32 effect_step_a(struct EffectObj *obj)
{
    s32 delta;
    register s32 pos asm("r1");
    switch (obj->unk30)
    {
    case 0:
        delta = obj->unk10 + 0x14;
        obj->unk10 = delta;
        pos = obj->unk08 + delta;
        obj->unk08 = pos;
        if (pos > 0x4FFF)
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

/* Two sprites that start at the screen's left and right edges and slide together (0x18 bytes,
 * cleared by obj_clear_bounds, set up by effect_init_sprite_pair, moved by effect_step_b, which
 * sees the same object as a struct EffectB). */
struct SpritePair
{
    u16 timer;       /* frames left to slide */
    s16 state;       /* 1: sliding, 2: holding */
    u16 xOfs[2];     /* per-language x nudge added to each sprite on screen, px */
    void *sprites[2];
    s32 x[2];        /* 1/256 px */
};

/* Sets up the sliding sprite pair in `p` (a struct SpritePair): two sprites with the current
 * language's records (0x2E + language and 0x34 + language, or 0x2D and 0x33 for others) at y 80,
 * the first at the screen's left edge and the second at x 240, each nudged by a few pixels in
 * languages 1-4, sliding together for 30 frames. */
void effect_init_sprite_pair(void *p)
{
    struct SpritePair *pair = p;
    void *sprite;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
        pair->sprites[i] = sprite;
        switch (gSys[SYS_LANG])
        {
        case 0:
            obj_load_record(pair->sprites[i], i * 6 + 0x2E, -1, 0, 0);
            break;
        case 1:
            obj_load_record(pair->sprites[i], i * 6 + 0x2F, -1, 0, 0);
            pair->xOfs[0] = -4;
            pair->xOfs[1] = 4;
            break;
        case 2:
            obj_load_record(pair->sprites[i], i * 6 + 0x30, -1, 0, 0);
            pair->xOfs[0] = -4;
            pair->xOfs[1] = 4;
            break;
        case 3:
            obj_load_record(pair->sprites[i], i * 6 + 0x31, -1, 0, 0);
            pair->xOfs[0] = -2;
            pair->xOfs[1] = 2;
            break;
        case 4:
            obj_load_record(pair->sprites[i], i * 6 + 0x32, -1, 0, 0);
            pair->xOfs[0] = -2;
            pair->xOfs[1] = 2;
            break;
        default:
            obj_load_record(sprite, i * 6 + 0x2D, -1, 0, 0);
            break;
        }
        sprite_update_cd0(pair->sprites[i]);
        pair->x[i] = (i * 15) << 12; /* 0 or 240 px */
        ((struct SpritePos *)pair->sprites[i])->x = pair->x[i] >> 8;
        ((struct SpritePos *)pair->sprites[i])->y = 80;
    }
    pair->timer = 30;
    pair->state = 1;
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

struct EffectB *obj_clear_bounds(struct EffectB *obj)
{
    struct SpritePair *p = (struct SpritePair *)obj;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        p->sprites[i] = 0;
        p->x[i] = 0;
    }
    p->timer = 0;
    p->state = 0;
    p->xOfs[0] = 0;
    p->xOfs[1] = 0;
    return obj;
}

/* Sets up the zoom-in banner of a transition (stepped by effect_step_c): creates its sprite from
 * record 0x22-0x26 (one per language in gSys[0x8C0], 0x21 otherwise) centred on screen at
 * (120, 80), and starts it at angle 0x100, state 0, timer 0. */
struct EffectC *effect_create_sprites(struct EffectC *banner)
{
    void *sprite;
    s32 y;

    sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
    banner->sprite = sprite;
    switch (gSys[0x8C0]) // language
    {
    case 0:
        obj_load_record(banner->sprite, 0x22, -1, 0, 0);
        break;
    case 1:
        obj_load_record(banner->sprite, 0x23, -1, 0, 0);
        break;
    case 2:
        obj_load_record(banner->sprite, 0x24, -1, 0, 0);
        break;
    case 3:
        obj_load_record(banner->sprite, 0x25, -1, 0, 0);
        break;
    case 4:
        obj_load_record(banner->sprite, 0x26, -1, 0, 0);
        break;
    default:
        obj_load_record(sprite, 0x21, -1, 0, 0);
        break;
    }
    sprite_update_cd0(banner->sprite);
    ((struct SpritePos *)banner->sprite)->x = 0x78;
    ((struct SpritePos *)banner->sprite)->y = y = 0x50;
    banner->timer = 0;
    banner->angle = y + 0xB0;
    banner->state = 0;
    return banner;
}

static inline void effect_c_wave(struct EffectC *obj, s32 phase)
{
    obj->sprite->x = (gSineTable[phase & 0xFF] / 64) * 2;
    obj->sprite->y = (gSineTable[phase & 0xFF] / 64) * 2;
}

/* Steps the zoom-in banner set up by effect_create_sprites, once a frame. Its size (sprite +4/+6)
 * follows a sine wave of `angle` (8.8, 0x100 steps a cycle): it grows while the angle runs up to
 * 0x40 at 2 steps a frame, eases back a little up to 0x60 at 4 steps a frame, then holds for 80
 * frames. Returns 1 once it is done, 0 while it runs. */
u32 effect_step_c(struct EffectC *banner)
{
    s32 phase = banner->angle >> 8;

    switch (banner->state)
    {
    case 0: /* grow */
        effect_c_wave(banner, phase);
        banner->angle += 0x200;
        if (phase > 0x40)
        {
            banner->state++;
        }
        break;
    case 1: /* ease back */
        effect_c_wave(banner, phase);
        banner->angle += 0x400;
        if (phase > 0x60)
        {
            banner->timer = 0x50;
            banner->state++;
        }
        break;
    case 2: /* hold */
        banner->timer -= 1;
        if (banner->timer > 0)
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

/* Sets up a drop-in effect: rests at screen (120, 80) for 300 frames, and creates its sprite
 * (records 0x4A-0x4F, one per language in gSys[0x8C0]) at x 120, 40 pixels above the screen. */
s32 effect_init_sprites_b(void *p)
{
    struct DropInEffect *obj = p;
    void *sprite;
    s32 i;

    obj->restX = 0x7800;
    obj->restY = 0x5000;
    obj->unk24 = 0;
    obj->unk28 = 0;
    obj->timer = 300;
    obj->state = 0;
    i = 0;
    do
    {
        sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
        obj->entries[i].sprite = sprite;
        switch (gSys[0x8C0]) // language
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
        obj->entries[i].x = obj->restX;
        obj->entries[i].y = -0x2800;
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
