#ifndef GUARD_TILE_WINDOW_H
#define GUARD_TILE_WINDOW_H

// The window layer's tilemap manager (init_sprite_object in code_080835B0, the slot and span
// functions in code_08084DB4). Windows draw into a RAM copy of two 32x20 BG tilemaps; every run
// of cells they change is queued as a span, and flush_copy_queue copies the spans to VRAM.

#define TILE_WINDOW_SLOT_COUNT 12
#define TILE_WINDOW_SPAN_COUNT 32
#define TILE_WINDOW_NODE_COUNT 8
// Bytes of the RAM tilemap: two 32x20 maps of u16 cells.
#define TILE_WINDOW_MAP_SIZE 0xA00
// Cells in a row of a tilemap, and in a whole tilemap.
#define TILE_WINDOW_MAP_WIDTH 32
#define TILE_WINDOW_MAP_CELLS (TILE_WINDOW_MAP_WIDTH * 20)

// One window's rectangle of tiles. Byte 5 (the top bits of size): bit 6 picks the tilemap,
// bit 7 is set once its tiles are allocated.
struct TileSlot
{
    u8 value;    // left column of the rectangle in the tilemap
    u8 unk1;     // top row
    u8 maxTiles; // width in tiles (0: the slot is unused)
    u8 unk3;     // height in tiles
    u16 size;    // bits 0-13: width * height
    u16 end;     // cells from pos to the rectangle's last one, plus 1
    u16 pos;     // cell of the top left corner (tilemap * 0x280 + row * 32 + column)
    u8 filler0A[2];
    u8 *block;   // the tile graphics in VRAM (0: none allocated)
    u8 *spare;
};

// `len` tilemap cells from cell `start` that still have to be copied to VRAM.
struct SpanNode
{
    struct SpanNode *next;
    s16 start;
    s16 len;
};

struct TileWindowNode
{
    struct TileWindowNode *next;
    u32 unk4[3];
};

struct TileWindow
{
    struct TileSlot slots[TILE_WINDOW_SLOT_COUNT];
    struct SpanNode spanPool[TILE_WINDOW_SPAN_COUNT];
    struct SpanNode *freeSpans;  // unused nodes of spanPool
    struct SpanNode *dirtySpans; // waiting for flush_copy_queue, sorted by start
    struct TileWindowNode nodePool[TILE_WINDOW_NODE_COUNT];
    struct TileWindowNode *freeNodes;
    struct TileWindowNode *unk27C;
    void *tileHeaps[2]; // free lists of each tilemap's tile block (create_text_entry)
    u16 *tilemap;       // TILE_WINDOW_MAP_SIZE bytes
};

#endif // GUARD_TILE_WINDOW_H
