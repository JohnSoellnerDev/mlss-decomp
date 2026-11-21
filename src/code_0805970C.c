#include "global.h"

struct AreaEntry
{
    u8 area;
    u8 unk1;
    u8 filler2[22];
};

struct Block24
{
    u8 bytes[24];
};

struct SysWords
{
    u16 unk0;
    u16 unk2;
};

struct MapStateDD0
{
    u8 bytes[12];
    u16 half[4];
};

extern const u32 gSceneDataPtrTable[];
extern const struct AreaEntry gAreaEntryTable[];
extern const u8 gClassDc68[];
extern u8 gUnk_03000DD0[];
extern const u8 gMapObjLoadClass[];
extern const u8 gTilePtrNone[];
extern const u8 gColumnTable[];
extern const u8 gCollisionGrids[];
extern const u8 gHeapTagObjBuf6c[];
extern const u8 gHeapTagObjBuf74[];
extern const u8 gHeapTagObjBuf68[];
extern const u8 gHeapTagObjBuf70[];
extern const u8 gHeapTagObjBuf78[];
extern u8 gUnk_03000DE8[];

void task_insert(void *node, u8 kind, u32 data);
void task_remove(void *node, u32 kind);
void free_block_if_bit0(void *ptr, u32 flags);
void clear_byte_0a(void *p);
void load_map_layout(u8 *obj);
void load_map_tilemap_layers(u8 *obj, u32 mode);
void calc_obj_scaled_pos(u8 *obj);
void redraw_map_columns(u8 *obj);
void write_scroll_shadows(u8 *obj);
void copy_map_layers_12(u8 *obj, u32 unused);
void obj_setup_track_layouts(u8 *obj);
void redraw_track_tiles(u8 *obj);
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void heap_free_a(void *p);
void cpu_fast_set(const void *src, void *dst, u32 control);
u32 dma_cancel_request(u32 dst);
void draw_metatile_2x2(u8 *obj, u32 layer, s32 col, s32 row, u16 tile);
struct SelRecord
{
    u8 animIndex;
    u8 metatileIndex;
    u8 chunkIndex0;
    u8 chunkSet;
    u8 filler4;
    u8 spriteIndex;
    u8 filler6[6];
};
extern const struct SelRecord gSelTable[];
extern const u8 gObjectPalettesA[];
extern const u8 gObjectPalettesB[];
extern const u8 gPalIndexTable[];
extern u16 gUnk_02000010[];
void map_load_chunks(u8 *obj);
void map_load_metatiles(u8 *obj);
void map_blit_chunks(u8 *obj);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void map_obj_draw(u8 *obj);

ASM_FUNC("asm/code_0805970C/redraw_metatile_row.s", void redraw_metatile_row(void))

ASM_FUNC("asm/code_0805970C/redraw_metatile_column.s", void redraw_metatile_column(void))

ASM_FUNC("asm/code_0805970C/redraw_map_row.s", void redraw_map_row(void))

void draw_metatile_2x2(u8 *obj, u32 layer, s32 col, s32 row, u16 tile);
ASM_FUNC("asm/code_0805970C/redraw_map_column.s", void redraw_map_column(void))

// Overwrites one field of the current area entry (gUnk_03000DD0, the IWRAM copy of a
// gAreaEntryTable entry). Fields are numbered 0: byte 0x00, 1-10: bytes 0x02-0x0B,
// 11-14: the u16 halves at 0x0C-0x12, 15: byte 0x14, 16: byte 0x01.
void map_state_set_field_by_index(u32 unused, u8 field, u16 value)
{
    switch (field)
    {
    case 0:
        gUnk_03000DD0[0] = value;
        break;
    case 1:
        gUnk_03000DD0[2] = value;
        break;
    case 2:
        gUnk_03000DD0[3] = value;
        break;
    case 3:
        gUnk_03000DD0[4] = value;
        break;
    case 4:
        gUnk_03000DD0[5] = value;
        break;
    case 5:
        gUnk_03000DD0[6] = value;
        break;
    case 6:
        gUnk_03000DD0[7] = value;
        break;
    case 7:
        gUnk_03000DD0[8] = value;
        break;
    case 8:
        gUnk_03000DD0[9] = value;
        break;
    case 9:
        gUnk_03000DD0[10] = value;
        break;
    case 10:
        gUnk_03000DD0[11] = value;
        break;
    case 11:
        ((struct MapStateDD0 *)gUnk_03000DD0)->half[0] = value;
        break;
    case 12:
        ((struct MapStateDD0 *)gUnk_03000DD0)->half[1] = value;
        break;
    case 13:
        ((struct MapStateDD0 *)gUnk_03000DD0)->half[2] = value;
        break;
    case 14:
        ((struct MapStateDD0 *)gUnk_03000DD0)->half[3] = value;
        break;
    case 15:
        gUnk_03000DD0[20] = value;
        break;
    case 16:
        gUnk_03000DD0[1] = value;
    }
}

extern u16 gUnk_02000014[];
extern u16 gUnk_02000016[];
extern u16 gUnk_0200001C[];
ASM_FUNC("asm/code_0805970C/obj_set_scroll_target.s", void obj_set_scroll_target(void))

/*
 * Constructor of the map loader task (class gMapObjLoadClass; obj_free_buffers_and_remove is its
 * destructor). Inserts the task, starts the loading phase at -5 (see obj_load_step), keeps the
 * area index and the extra argument, allocates and clears its three buffers (0x3000 bytes at
 * obj + 0x68, 0x6000 for the tileset graphics at +0x70, 0x200 for the palette at +0x78), copies
 * the area's gAreaEntryTable entry to gUnk_03000DE8 (bit 15 of the index is a flag) and zeroes
 * the layers' scroll positions at +0x4E..+0x64.
 */
u8 *obj_init_task_and_buffers(u8 *obj, u8 prio, u32 arg, u16 areaIndex, u16 extra)
{
    u32 zero;
    u32 clear;
    u16 firstPhase;

    task_insert(obj, prio, arg);
    *(const u8 **)(obj + 0x18) = gMapObjLoadClass;
    clear = 0;
    firstPhase = -5;
    *(u16 *)(obj + 0x1C) = firstPhase;
    *(u16 *)(obj + 0x1E) = areaIndex;
    *(u16 *)(obj + 0x22) = clear;
    *(u16 *)(obj + 0x24) = clear;
    *(u16 *)(obj + 0x7C) = extra;
    *(void **)(obj + 0x68) = heap_alloc(0x3000, 1, gHeapTagObjBuf68, 0);
    *(void **)(obj + 0x70) = heap_alloc(0x6000, 1, gHeapTagObjBuf70, 0);
    *(void **)(obj + 0x78) = heap_alloc(0x200, 1, gHeapTagObjBuf78, 0);
    /* fill each buffer with zero words (cpu_fast_set: bit 24 = fill, low bits = word count) */
    zero = clear;
    cpu_fast_set(&zero, *(void **)(obj + 0x68), 0x01000000 | (0x3000 / 4));
    zero = clear;
    cpu_fast_set(&zero, *(void **)(obj + 0x70), 0x01000000 | (0x6000 / 4));
    zero = clear;
    cpu_fast_set(&zero, *(void **)(obj + 0x78), 0x01000000 | (0x200 / 4));
    *(struct Block24 *)gUnk_03000DE8 = *(struct Block24 *)&gAreaEntryTable[*(u16 *)(obj + 0x1E) & 0x7FFF];
    *(u16 *)(obj + 0x52) = clear;
    *(u16 *)(obj + 0x50) = clear;
    *(u16 *)(obj + 0x4E) = clear;
    *(u16 *)(obj + 0x58) = clear;
    *(u16 *)(obj + 0x56) = clear;
    *(u16 *)(obj + 0x54) = clear;
    *(u16 *)(obj + 0x5E) = clear;
    *(u16 *)(obj + 0x5C) = clear;
    *(u16 *)(obj + 0x5A) = clear;
    *(u16 *)(obj + 0x64) = clear;
    *(u16 *)(obj + 0x62) = clear;
    *(u16 *)(obj + 0x60) = clear;
    return obj;
}

void obj_copy_rom_block(u8 *obj);
void obj_rebuild_map_tiles(u8 *obj);
/* gTbase: a block of graphics, headed by a table of u32 offsets into it.
 * gLoadList: per tileset (gUnk_03000DE8[3], the area entry's byte 3), 4 indices into gTbase's
 * table, 0xFF for none. */
extern const u8 gTbase[];
extern const u8 gLoadList[];
/* a copy request: [0] state (0 = done, 2 = queued), [1] source, [2] destination, [3] 0 */
extern u32 gUnk_03000DC0[];
/* queues a copy request for the interrupt handler, or runs it at once when none is installed */
void table_register(void *entry);

/*
 * One step of the map loader's loading phases (the u16 at obj + 0x1C counts up from -5 to 0):
 * -5 copies the area's palette block and requests part 0 of the tileset's graphics into the
 * buffer at obj + 0x70; -4 and -3 wait for the previous copy, then request parts 1 and 2 to
 * +0x2000 and +0x4000; -2 waits for the last one; -1 rebuilds the map's tiles.
 */
void obj_load_step(u8 *obj)
{
    const u8 *gfx;
    const u32 *offsets;
    u32 tileset;
    u32 entry;
    u32 zero;
    u32 state;

    gfx = gTbase;
    offsets = (const u32 *)gfx;
    tileset = gUnk_03000DE8[3];
    switch ((s16)(*(u16 *)(obj + 0x1C) + 5))
    {
    case 0:
        obj_copy_rom_block(obj);
        zero = 0;
        gUnk_03000DC0[0] = zero;
        {
            register const u8 *list asm("r0") = gLoadList;
            entry = list[tileset * 4];
        }
        if (entry != 0xFF)
        {
            gUnk_03000DC0[1] = (u32)(gfx + offsets[entry]);
            gUnk_03000DC0[2] = *(u32 *)(obj + 0x70);
            gUnk_03000DC0[3] = zero;
            table_register(gUnk_03000DC0);
        }
        (*(u16 *)(obj + 0x1C))++;
        break;
    case 1:
    {
        register u32 *req asm("r3") = gUnk_03000DC0;

        state = req[0];
        if (state != 0)
            break;
        {
            register const u8 *list asm("r0") = gLoadList;
            u32 i = tileset * 4 + 1;
            entry = *(const u8 *)(i + (u32)list);
        }
        if (entry != 0xFF)
        {
            req[1] = (u32)(gfx + offsets[entry]);
            req[2] = *(u32 *)(obj + 0x70) + 0x2000;
            req[3] = state;
            table_register(req);
        }
        (*(u16 *)(obj + 0x1C))++;
        break;
    }
    case 2:
    {
        register u32 *req asm("r3") = gUnk_03000DC0;

        state = req[0];
        if (state != 0)
            break;
        {
            register const u8 *list asm("r0") = gLoadList;
            u32 i = tileset * 4 + 2;
            entry = *(const u8 *)(i + (u32)list);
        }
        if (entry != 0xFF)
        {
            req[1] = (u32)(gfx + offsets[entry]);
            req[2] = *(u32 *)(obj + 0x70) + 0x4000;
            req[3] = state;
            table_register(req);
        }
        (*(u16 *)(obj + 0x1C))++;
        break;
    }
    case 3:
        if (gUnk_03000DC0[0] != 0)
            break;
        (*(u16 *)(obj + 0x1C))++;
        break;
    case 4:
        obj_rebuild_map_tiles(obj);
        (*(u16 *)(obj + 0x1C))++;
        break;
    }
}

void draw_metatile_pal(u8 *obj, u16 row, s16 col, s16 sub, u16 tile);
ASM_FUNC("asm/code_0805970C/redraw_track_tiles.s", void redraw_track_tiles(u8 *obj))

ASM_FUNC("asm/code_0805970C/draw_metatile_pal.s", void draw_metatile_pal(u8 *obj, u16 row, s16 col, s16 sub, u16 tile))

ASM_FUNC("asm/code_0805970C/obj_setup_track_layouts.s", void obj_setup_track_layouts(u8 *obj))

ASM_FUNC("asm/code_0805970C/copy_map_layers_12.s", void copy_map_layers_12(u8 *obj, u32 unused))

extern u8 gSys[];
extern const u8 gMapPalettesA[];
extern const u8 gMapPalettesB[];

static inline const u8 *map_palette_set(void)
{
    if ((gSys[0x888] & 6) == 2)
        return gMapPalettesB;
    return gMapPalettesA;
}
extern const u8 gRomBlockTable[];
extern u8 gUnk_03000DE8[];
void obj_copy_rom_block(u8 *obj)
{
    register const u8 *src asm("r2");
    if ((gSys[0x888] & 6) != 2)
        goto use_default;
    src = gMapPalettesB;
    goto copy;
use_default:
    src = gMapPalettesA;
copy:
    cpu_fast_set(src + ((const u32 *)src)[gRomBlockTable[gUnk_03000DE8[5]]], *(void **)(obj + 0x78), 0x78);
}

void obj_rebuild_map_tiles(u8 *obj)
{
    u32 zero;

    *(void **)(obj + 0x6C) = heap_alloc(0x3000, 1, gHeapTagObjBuf6c, 0);
    *(void **)(obj + 0x74) = heap_alloc(0x2800, 1, gHeapTagObjBuf74, 0);
    zero = 0;
    cpu_fast_set(&zero, *(void **)(obj + 0x6C), 0x01000C00);
    zero = 0;
    cpu_fast_set(&zero, *(void **)(obj + 0x74), 0x01000A00);
    obj_setup_track_layouts(obj);
    copy_map_layers_12(obj, 0);
    redraw_track_tiles(obj);
    if (*(void **)(obj + 0x74) != 0)
    {
        heap_free_a(*(void **)(obj + 0x74));
    }
    if (*(void **)(obj + 0x6C) != 0)
    {
        heap_free_a(*(void **)(obj + 0x6C));
    }
}

void obj_free_buffers_and_remove(u8 *obj, u32 arg)
{
    void *buf;

    *(const u8 **)(obj + 0x18) = gMapObjLoadClass;
    buf = *(void **)(obj + 0x78);
    if (buf != 0)
    {
        heap_free_a(buf);
    }
    buf = *(void **)(obj + 0x70);
    if (buf != 0)
    {
        heap_free_a(buf);
    }
    buf = *(void **)(obj + 0x68);
    if (buf != 0)
    {
        heap_free_a(buf);
    }
    task_remove(obj, arg);
}

u8 area_entry_byte1(u32 unused, u16 idx)
{
    return gAreaEntryTable[idx].unk1;
}

u8 area_table_lookup_0805A6EC(u32 unused, u16 idx)
{
    return gAreaEntryTable[idx].area;
}

void map_obj_draw_load_map_state(u32 unused, u16 cfg)
{
    *(struct Block24 *)gUnk_03000DD0 = *(struct Block24 *)&gAreaEntryTable[cfg];
}

u32 table_083b0dac(u16 i)
{
    return gSceneDataPtrTable[i];
}

u16 count_area_entries(void)
{
    u32 count = 0;
    const struct AreaEntry *p = gAreaEntryTable;

    while (p->area != 0xFF && p->unk1 != 0xFF)
    {
        p++;
        count++;
    }
    return count;
}

void write_scroll_shadows(u8 *obj)
{
    vu16 *p;
    register vu16 *q asm("r1");

    p = gUnk_02000014;
    *p = 0x1FF & *(u16 *)(obj + 0x5A);
    p += 2;
    *p = 0x1FF & *(u16 *)(obj + 0x5C);
    *(vu16 *)gUnk_0200001C = 0x1FF & *(u16 *)(obj + 0x5E);
    q = gUnk_02000016;
    *q = obj[0x60];
    q += 2;
    *q = obj[0x62];
    q += 2;
    *q = obj[0x64];
}

extern u8 gUnk_02000080[];
static inline struct SysWords *sys_words(void)
{
    return (struct SysWords *)gSys;
}

void copy_rom_block_fixed_dst(void)
{
    register const u8 *src asm("r2");
    if ((gSys[0x888] & 6) != 2)
        goto use_default;
    src = gMapPalettesB;
    goto copy;
use_default:
    src = gMapPalettesA;
copy:
    cpu_fast_set(src + ((const u32 *)src)[gRomBlockTable[gUnk_03000DD0[5]]], gUnk_02000080, 0x78);
    ((struct SysWords *)gSys)->unk2 = 0x7FFF;
}

void obj_reload_map(u8 *obj)
{
    write_scroll_shadows(obj);
    load_map_layout(obj);
    load_map_tilemap_layers(obj, 0);
    calc_obj_scaled_pos(obj);
    redraw_map_columns(obj);
}

void obj_blit_tile_rows(u8 *obj)
{
    const u8 *grids = gCollisionGrids;
    const u8 *columns = gColumnTable;
    const u8 *group;
    const u8 *src;
    s32 row;
    s32 col;

    group = columns + *(u16 *)(obj + 0x1E) * 8;
    src = grids + *(u32 *)(grids + *(u16 *)(group + 6) * 4);
    for (row = 0; row < *(u16 *)(obj + 0x28); row++)
    {
        for (col = 0; col < *(u16 *)(obj + 0x26); col++)
        {
            *(*(u8 **)(obj + 0x9C) + (row * *(u16 *)(obj + 0x26) + col)) = src[row * *(u16 *)(obj + 0x26) + col];
        }
    }
}

extern const u32 *const gRomPtrTableA0[];
void obj_load_rom_block_a0(u8 *obj)
{
    u8 index = gUnk_03000DD0[6];
    const u32 *const *table = gRomPtrTableA0;
    const u32 *const *entry = &table[index];
    s32 i = 0;
    u32 *volatile *dst = (u32 *volatile *)(obj + 0xA0);
    const u32 *src = *entry;

    do
    {
        (*dst)[i] = *src++;
        i++;
    } while (i < 0x100);
}

void copy_track_limits(u8 *obj, u16 track, u16 *outA, u16 *outB)
{
    u32 off = track * 2;
    u16 *p = (u16 *)(obj + 0x36);
    u16 *q;
    *outA = *(u16 *)((u8 *)p + off);
    q = (u16 *)(obj + 0x3C);
    *outB = *(u16 *)((u8 *)q + off);
}

u32 obj_list_entry(u8 *obj, u8 idx)
{
    return *(u32 *)(obj + 0xA0) + idx * 4;
}

u32 tile_ptr_at(u8 *obj, s16 x, s16 y)
{
    u16 idx;
    u8 tile;

    if (x < 0 || x >= *(u16 *)(obj + 0x46) || y < 0 || y >= *(u16 *)(obj + 0x4C))
    {
        return (u32)gTilePtrNone;
    }
    idx = x + y * *(u16 *)(obj + 0x26);
    tile = *(*(u8 **)(obj + 0x9C) + idx);
    return obj_list_entry(obj, tile);
}

u8 get_obj_grid_byte(u8 *obj, s16 x, s16 y)
{
    u16 index = x + y * *(u16 *)(obj + 0x26);
    u8 *buf = *(u8 **)(obj + 0x9C);
    return buf[index];
}

void sub_0805A9C4(u8 *obj, u16 track, u16 *outA, u16 *outB)
{
    u32 off = track * 2;
    u16 *p = (u16 *)(obj + 0x4E);
    u16 *q;
    *outA = *(u16 *)((u8 *)p + off);
    q = (u16 *)(obj + 0x54);
    *outB = *(u16 *)((u8 *)q + off);
}

void obj_clear_vram(u8 *obj)
{
    u32 zero;
    void *buf;

    dma_cancel_request(0x06000000);
    buf = *(void **)(obj + 0xA4);
    if (buf != 0)
    {
        heap_free_a(buf);
    }
    zero = 0;
    cpu_fast_set(&zero, (void *)0x06000000, 0x01002000);
}

void stub_nop_destroy_08cddc68(void)
{
}

void sub_0805AA20(u8 *obj, u32 kind)
{
    *(const u8 **)(obj + 0x18) = gClassDc68;
    free_block_if_bit0(obj + 0x20, 2);
    task_remove(obj, kind);
}

u8 *init_vt_08cddc68(u8 *obj, u8 kind, u32 data)
{
    task_insert(obj, kind, data);
    *(const u8 **)(obj + 0x18) = gClassDc68;
    clear_byte_0a(obj + 0x20);
    return obj;
}

ASM_FUNC("asm/code_0805970C/obj_setup_map_object.s", void obj_setup_map_object(void))

ASM_FUNC("asm/code_0805970C/map_obj_update.s", void map_obj_update(void))

/* Shows a map object's screen: writes its BG0/BG1 scroll (x at obj + 0x30/0x32, masked to 0-511;
 * y at obj + 0x34/0x36, masked to 0-255) to the IO register shadows at 0x02000010, loads and blits
 * its chunks and metatiles, copies 0x2000 bytes of tile graphics (obj + 0x38) to VRAM 0x0600D000,
 * then loads the 8 palettes of its gSelTable entry into the palette buffer (from gObjectPalettesA
 * when the gSys + 0x888 mode bits are 1, else gObjectPalettesB) and marks them all to upload. */
void map_obj_draw(u8 *obj)
{
    register const u8 *palettes asm("r5");
    vu16 *bg0Scroll;

    bg0Scroll = gUnk_02000010;
    *bg0Scroll = 0x1FF & *(u16 *)(obj + 0x30);
    *(vu16 *)gUnk_02000014 = 0x1FF & *(u16 *)(obj + 0x32);
    bg0Scroll += 1;
    *bg0Scroll = 0xFF & *(u16 *)(obj + 0x34);
    *(vu16 *)gUnk_02000016 = 0xFF & *(u16 *)(obj + 0x36);
    map_load_chunks(obj);
    map_load_metatiles(obj);
    map_blit_chunks(obj);
    dma_copy_or_queue(*(void **)(obj + 0x38), (void *)0x0600D000, 0x2000, 0x20, 0);
    if ((gSys[0x888] & 6) != 2)
        goto use_b;
    palettes = gObjectPalettesA;
    goto copy;
use_b:
    palettes = gObjectPalettesB;
copy:
    /* each palette set starts with a table of offsets from its start */
    cpu_fast_set(palettes + ((const u32 *)palettes)[gPalIndexTable[gSelTable[*(u16 *)(obj + 0x1E)].chunkIndex0]], gUnk_02000080, 0x40);
    sys_words()->unk2 = 0x7FFF;
}
