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
extern const u8 gSelTable[];
void map_obj_draw(u8 *obj);

ASM_FUNC("asm/code_0805970C/redraw_metatile_row.s", void redraw_metatile_row(void))

ASM_FUNC("asm/code_0805970C/redraw_metatile_column.s", void redraw_metatile_column(void))

ASM_FUNC("asm/code_0805970C/redraw_map_row.s", void redraw_map_row(void))

void draw_metatile_2x2(u8 *obj, u32 layer, s32 col, s32 row, u16 tile);
ASM_FUNC("asm/code_0805970C/redraw_map_column.s", void redraw_map_column(void))

void map_state_set_field_by_index(u32 unused, u8 index, u16 value)
{
    switch (index)
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

u8 *obj_init_task_and_buffers(u8 *obj, u8 prio, u32 arg, u16 cfg, u16 extra)
{
    u32 zero;
    u32 clear;
    u16 phase;

    task_insert(obj, prio, arg);
    *(const u8 **)(obj + 0x18) = gMapObjLoadClass;
    clear = 0;
    phase = 0xFFFB;
    *(u16 *)(obj + 0x1C) = phase;
    *(u16 *)(obj + 0x1E) = cfg;
    *(u16 *)(obj + 0x22) = clear;
    *(u16 *)(obj + 0x24) = clear;
    *(u16 *)(obj + 0x7C) = extra;
    *(void **)(obj + 0x68) = heap_alloc(0x3000, 1, gHeapTagObjBuf68, 0);
    *(void **)(obj + 0x70) = heap_alloc(0x6000, 1, gHeapTagObjBuf70, 0);
    *(void **)(obj + 0x78) = heap_alloc(0x200, 1, gHeapTagObjBuf78, 0);
    zero = clear;
    cpu_fast_set(&zero, *(void **)(obj + 0x68), 0x01000C00);
    zero = clear;
    cpu_fast_set(&zero, *(void **)(obj + 0x70), 0x01001800);
    zero = clear;
    cpu_fast_set(&zero, *(void **)(obj + 0x78), 0x01000080);
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
ASM_FUNC("asm/code_0805970C/obj_load_step.s", void obj_load_step(void))

void draw_metatile_pal(u8 *obj, u16 row, s16 col, s16 sub, u16 tile);
ASM_FUNC("asm/code_0805970C/redraw_track_tiles.s", void redraw_track_tiles(u8 *obj))

ASM_FUNC("asm/code_0805970C/draw_metatile_pal.s", void draw_metatile_pal(u8 *obj, u16 row, s16 col, s16 sub, u16 tile))

ASM_FUNC("asm/code_0805970C/obj_setup_track_layouts.s", void obj_setup_track_layouts(u8 *obj))

ASM_FUNC("asm/code_0805970C/copy_map_layers_12.s", void copy_map_layers_12(u8 *obj, u32 unused))

extern u8 gSys[];
extern const u8 gMapPalettesA[];
extern const u8 gMapPalettesB[];
extern const u8 gRomBlockTable[];
extern u8 gUnk_03000DE8[];
static inline const u8 *map_palette_set(void)
{
    if ((gSys[0x888] & 6) == 2)
        return gMapPalettesB;
    return gMapPalettesA;
}

NONMATCH("asm/code_0805970C/obj_copy_rom_block.s", void obj_copy_rom_block(u8 *obj))
{
    const u8 *src = map_palette_set();

    cpu_fast_set(src + ((const u32 *)src)[gRomBlockTable[gUnk_03000DE8[5]]], *(void **)(obj + 0x78), 0x78);
}
END_NONMATCH

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

NONMATCH("asm/code_0805970C/write_scroll_shadows.s", void write_scroll_shadows(u8 *obj))
{
    vu16 *p;

    p = gUnk_02000014;
    *p = 0x1FF & *(u16 *)(obj + 0x5A);
    p += 2;
    *p = 0x1FF & *(u16 *)(obj + 0x5C);
    *(vu16 *)gUnk_0200001C = 0x1FF & *(u16 *)(obj + 0x5E);
    p = gUnk_02000016;
    *p = obj[0x60];
    p += 2;
    *p = obj[0x62];
    p += 2;
    *p = obj[0x64];
}
END_NONMATCH

extern u8 gUnk_02000080[];
ASM_FUNC("asm/code_0805970C/copy_rom_block_fixed_dst.s", void copy_rom_block_fixed_dst(void))

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

ASM_FUNC("asm/code_0805970C/obj_load_rom_block_a0.s", void obj_load_rom_block_a0(void))

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

extern const u8 gSelTable[];
ASM_FUNC("asm/code_0805970C/map_obj_draw.s", void map_obj_draw(u8 *obj))
