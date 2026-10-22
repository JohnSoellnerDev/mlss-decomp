#include "global.h"

struct MapAnimSlot
{
    u8 state;
    u8 filler01;
    u16 value;
    u8 sub;
    u8 timer;
    u8 limit;
    u8 index;
    u8 flags;
    u8 filler09;
    u16 tiles[16];
    u16 offsets[16];
    u8 filler4A[0x4C - 0x4A];
};

struct MapLoaderObj
{
    u8 filler0[0x18];
    void *desc;
    s16 phase;
    u16 selector;
    s16 impulseX;
    s16 impulseY;
    s16 baseX;
    s16 baseY;
    s16 scaleX;
    s16 scaleY;
    u8 filler2C[0x2D - 0x2C];
    u8 wavePhase;
    u8 waveStep;
    u8 filler2F[0x30 - 0x2F];
    u16 posX;
    u16 accX;
    u16 posY;
    u16 accY;
    void *tilemap;
    void *chunks;
    void *gfx;
    void *metatiles;
    u8 filler48[0x4E - 0x48];
    u16 trackA[3];
    u16 trackB[3];
    u8 filler5A[0x98 - 0x5A];
    void *layerTiles;
    u8 filler9C[0xAC - 0x9C];
    struct MapAnimSlot anim[16];
    u8 entryFlags;
    u8 entryCounter;
    u8 entryIndex;
    u8 filler56F;
    u16 entryBldcnt;
    u16 entryTimer;
    void *entryTable;
    u8 filler578;
    u8 layer3Flag579;
    u8 filler57A[0x57C - 0x57A];
    void *stateSpriteCtx;
    u8 stateSpriteSlots[4];
    u8 displayFlag584;
    u8 displayIndex585;
    u8 filler586[0x588 - 0x586];
    u8 display588;
    u8 filler589;
    u16 display58A;
    u16 display58C;
    u16 display58E;
    u16 display590;
    u16 display592;
    u16 display594;
    u8 display596;
    u8 display597;
    u8 display598;
    u8 layer3Flag599;
    u8 filler59A[0x768 - 0x59A];
    void *spriteCtx;
    u8 spriteSlots[4];
    u8 animLock;
};

extern u16 gUnk_02000010;
extern u16 gUnk_02000014;
extern u16 gUnk_02000016;
extern u16 gUnk_02000050;
extern u16 gUnk_02000052;
extern const u32 gObjectTilesets[];
extern const u32 gMapObjDescriptor[];
extern const u32 gMetatileSets[];
extern const u32 gSpriteTable[];
extern u8 gUnk_03000DD0[];
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void update_scanline_offsets();
void set_0d80_slot(void (*fn)(void), u32 slot);
extern void *gUnk_03000E08;
extern void *gUnk_03000E0C;
extern const u8 gHeapTagDisplayBufA[];
extern const u8 gHeapTagDisplayBufB[];

struct MapAnimEntry
{
    u8 flag;
    u8 type;
    u16 unk2;
    void *chunks;
};

struct MapAnimChunk
{
    u16 x;
    u16 y;
};

extern const void *gAnimListTable[];
u32 sprite_alloc(void *ctx, u32 a, u32 b, u32 c, u32 d, u32 e);
extern const u16 gMetatileIndexTable[][2];
extern const u32 gLayer3Table[][2];
extern const u16 gChunkSetTable[][2];
void map_blit_tile(struct MapLoaderObj *obj, u16 layer, s16 col, u16 row, u32 tile);
extern u8 *gUnk_03000FC0;
u8 test_flag(u8 *table, u8 *tableB, u16 id);
void map_put_rect(struct MapLoaderObj *obj, u32 layer, void *rect);
void put_layer3_rect(struct MapLoaderObj *obj, void *rect);
void set_flag(u8 *flags, u32 a, u32 b, u32 c);

struct Layer3Entry
{
    void *rect0;
    void *rect1;
    void *rect2;
    void *rect3;
    u16 flags;
    u8 filler12[0x13 - 0x12];
    u8 last;
    u8 filler14[20 - 0x14];
};

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

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void heap_free_a(void *p);
u32 dma_cancel_request(u32 dst);
void task_remove(void *node, u32 kind);

struct EntryRec
{
    u32 low5 : 5;
    u32 mid5 : 5;
    u32 t8 : 8;
    u32 rest : 14;
};

extern const u32 gChunkGrids[];

NONMATCH("asm/code_0805AF28/map_load_chunks.s", void map_load_chunks(struct MapLoaderObj *obj))
{
    u8 *grids;
    u8 set;
    u32 next;
    u16 layer;
    u16 row;
    u16 col;

    grids = (u8 *)gChunkGrids;
    set = gSelTable[obj->selector].chunkSet;
    layer = 0;
    do
    {
        u16 chunk = gChunkSetTable[set][layer];

        next = layer + 1;
        if (chunk != 0xFFFF)
        {
            u8 *data = grids + gChunkGrids[chunk];
            u16 *dst = obj->chunks;

            row = 0;
            do
            {
                col = 0;
                do
                {
                    dst[(row << 5) + ((layer << 9) + col)] = data[(row << 5) + col];
                    col++;
                } while (col <= 0x1F);
                row++;
            } while (row <= 0xF);
        }
        else
        {
            u16 *dst = obj->chunks;

            row = 0;
            do
            {
                col = 0;
                do
                {
                    dst[(row << 5) + ((layer << 9) + col)] = 0;
                    col++;
                } while (col <= 0x1F);
                row++;
            } while (row <= 0xF);
        }
        layer = next & 0xFFFF;
    } while (layer <= 1);
}
END_NONMATCH

NONMATCH("asm/code_0805AF28/map_blit_chunks.s", void map_blit_chunks(struct MapLoaderObj *obj))
{
    u32 set;
    u32 next;
    u16 layer;
    u16 row;
    u16 col;

    set = gSelTable[obj->selector].chunkSet;
    layer = 0;
    do
    {
        u16 chunk = gChunkSetTable[set][layer];

        next = layer + 1;
        if (chunk != 0xFFFF)
        {
            row = 0;
            do
            {
                col = 0;
                do
                {
                    u16 tile = ((u16 *)obj->chunks)[(row << 5) + (layer << 9) + col];

                    map_blit_tile(obj, layer, (s16)col, (s16)row, tile);
                    col++;
                } while (col <= 0x1F);
                row++;
            } while (row <= 0xF);
        }
        layer = next & 0xFFFF;
    } while (layer <= 1);
}
END_NONMATCH

NONMATCH("asm/code_0805AF28/map_blit_tile.s", void map_blit_tile(struct MapLoaderObj *obj, u16 layer, s16 col, u16 row, u32 tile))
{
    u16 tl;
    u16 bl;
    u16 *tilemap;
    u16 *rec;
    u16 *metatiles;
    u16 *dst;
    u32 index;

    index = (tile << 22) >> 22;
    metatiles = (u16 *)obj->metatiles;
    if ((col & 0x10) == 0)
    {
        tl = (layer << 11) + ((row & 0xF) << 6) + ((col & 0xF) << 1);
        bl = (layer << 11) + ((((row & 0xF) << 1) + 1) << 5) + ((col & 0xF) << 1);
    }
    else
    {
        tl = (layer << 11) + ((row & 0xF) << 6) + ((col & 0xF) << 1) + 0x400;
        bl = (layer << 11) + ((((row & 0xF) << 1) + 1) << 5) + ((col & 0xF) << 1) + 0x400;
    }
    tilemap = (u16 *)obj->tilemap;
    dst = tilemap + tl;
    rec = metatiles + index * 4;
    dst[0] = rec[0];
    dst[1] = rec[1];
    dst = tilemap + bl;
    dst[0] = rec[2];
    dst[1] = rec[3];
}
END_NONMATCH

void map_load_metatiles(struct MapLoaderObj *obj)
{
    u8 *sets = (u8 *)gMetatileSets;
    u8 index = gSelTable[obj->selector].metatileIndex;
    u16 entry = gMetatileIndexTable[index][0];
    u16 *src = (u16 *)(sets + gMetatileSets[entry]);
    u16 *dst;
    u16 i;

    i = 0;
    dst = obj->metatiles;
    for (; i <= 0x3FF; i++)
        dst[i] = src[i];
}

void map_obj_set_impulse(struct MapLoaderObj *obj, s16 x, s16 y)
{
    obj->impulseX = x;
    obj->impulseY = y;
}

void map_obj_write_scroll(struct MapLoaderObj *obj)
{
    vu16 *p = &gUnk_02000010;

    *p = obj->posX & 0x1FF;
    gUnk_02000014 = obj->accX & 0x1FF;
    p++;
    *p = obj->posY & 0xFF;
    gUnk_02000016 = obj->accY & 0xFF;
}

void map_loader_map_obj_destroy(struct MapLoaderObj *obj, u32 param)
{
    obj->desc = (void *)gMapObjDescriptor;
    dma_cancel_request(0x06000000);
    if (obj->gfx != NULL)
        heap_free_a(obj->gfx);
    if (obj->metatiles != NULL)
        heap_free_a(obj->metatiles);
    if (obj->chunks != NULL)
        heap_free_a(obj->chunks);
    if (obj->tilemap != NULL)
        heap_free_a(obj->tilemap);
    task_remove(obj, param);
}

ASM_FUNC("asm/code_0805AF28/map_anim_slots_load.s", void map_anim_slots_load(void))

void map_blit_tile_rect(struct MapLoaderObj *obj, u32 layer, void *rect);
ASM_FUNC("asm/code_0805AF28/map_anim_slots_step.s", void map_anim_slots_step(void))

ASM_FUNC("asm/code_0805AF28/map_blit_tile_rect.s", void map_blit_tile_rect(struct MapLoaderObj *obj, u32 layer, void *rect))

ASM_FUNC("asm/code_0805AF28/map_anim_update.s", void map_anim_update(void))

ASM_FUNC("asm/code_0805AF28/map_anim_init.s", void map_anim_init(void))

struct SpriteEntry
{
    u8 flags;
    u8 arg;
    u16 unk2;
};

NONMATCH("asm/code_0805AF28/map_loader_map_obj_setup_sprites.s", void map_loader_map_obj_setup_sprites(struct MapLoaderObj *obj))
{
    struct SpriteEntry *table;
    u8 index;
    u8 i;
    u8 cur;

    i = 0;
    do
    {
        obj->spriteSlots[i] |= 0xFF;
        i++;
    } while (i <= 3);

    if (obj->spriteCtx != NULL)
    {
        index = gSelTable[obj->selector].spriteIndex;
        table = (struct SpriteEntry *)gSpriteTable[index];
        if (table != NULL)
        {
            i = 0;
            do
            {
                cur = i;
                if ((table[cur].flags & 0x40) == 0)
                {
                    obj->spriteSlots[cur] = sprite_alloc(obj->spriteCtx, 0xFF, table[cur].arg | 0x5000, 0xFF, 0xFFFF, 0xFFFF);
                }
                i = cur + 1;
            } while ((table[cur].flags & 0x80) == 0);
        }
    }
}
END_NONMATCH

void map_upload_tiles(u32 unused, u16 slot, u8 count, u16 index)
{
    dma_copy_or_queue((u8 *)gObjectTilesets + gObjectTilesets[index], (void *)(0x06000000 + slot * 0x20), count * 0x20, 0x20, 0);
}

ASM_FUNC("asm/code_0805AF28/map_anim2_update.s", void map_anim2_update(void))

ASM_FUNC("asm/code_0805AF28/map_anim2_init.s", void map_anim2_init(void))

void map_anim2_start_0805BCCC(struct MapLoaderObj *obj, u16 animId)
{
    u8 slot = 0;
    u8 i;
    u8 j;

    if (obj->anim[0].state != 0)
    {
        do
        {
            slot = (u8)(slot + 1);
            if (obj->anim[slot].state == 0)
                break;
        } while (slot <= 0xF);
    }

    if (slot <= 0xF)
    {
        u8 variant = gUnk_03000DD0[7];
        const struct MapAnimEntry *list = (const struct MapAnimEntry *)gAnimListTable[variant];

        if (list != NULL)
        {
            u8 flag = list[slot].flag;

            if (flag & 0x40)
            {
                const struct MapAnimChunk *chunk;

                obj->anim[slot].state = (((u32)list[animId].type << 30) >> 30) + 0x20;
                obj->anim[slot].value = (*(const u32 *)&list[animId] >> 10) & 0x3FF;
                obj->anim[slot].sub = ((u32)list[animId].unk2 << 23) >> 27;

                chunk = (const struct MapAnimChunk *)list[animId].chunks;
                i = 0;
                do
                {
                    j = i;
                    obj->anim[slot].tiles[j] = chunk[j].x;
                    obj->anim[slot].offsets[j] = chunk[j].y & 0x7FFF;
                    i = (u8)(i + 1);
                } while (!(chunk[j].y & 0x8000));

                obj->anim[slot].limit = i;
                obj->anim[slot].index = 0;
                obj->anim[slot].flags = 0;
            }
        }
    }
}

void map_layer3_entry(struct MapLoaderObj *obj, u32 index);
void map_check_layer3(struct MapLoaderObj *obj)
{
    struct Layer3Entry *list;
    u8 variant;
    u8 k;
    u8 cur;
    u32 mask;

    variant = gUnk_03000DD0[0xA];
    if (variant != 0)
    {
        list = (struct Layer3Entry *)gLayer3Table[variant][0];
        k = 0;
        mask = 0x7FFF;
        do
        {
            u16 flags = list[k].flags;

            if ((mask & flags) != mask)
            {
                u32 event = (u32)flags << 17;

                if (test_flag(gUnk_03000FC0 + 0x38C, 0, ((event >> 1) + 0x07200000) >> 16) == 1)
                    map_layer3_entry(obj, k);
            }
            cur = k;
            k = (u8)(cur + 1);
        } while ((list[cur].last & 0x80) == 0);
    }
}

NONMATCH("asm/code_0805AF28/map_layer3_entry.s", void map_layer3_entry(struct MapLoaderObj *obj, u32 index))
{
    u8 variant;
    u16 idx;
    u16 flags;
    struct Layer3Entry *entry;
    struct Layer3Entry *list;

    idx = index;
    variant = gUnk_03000DD0[0xA];
    if (variant != 0)
    {
        list = (struct Layer3Entry *)gLayer3Table[variant][0];
        entry = (struct Layer3Entry *)((u32)idx * 20 + (u32)list);
        if (entry->rect0 != NULL)
            map_put_rect(obj, 0, entry->rect0);
        if (entry->rect1 != NULL)
            map_put_rect(obj, 1, entry->rect1);
        if (entry->rect2 != NULL)
            map_put_rect(obj, 2, entry->rect2);
        if (entry->rect3 != NULL)
            put_layer3_rect(obj, entry->rect3);
        obj->layer3Flag579 = 1;
        flags = entry->flags;
        if ((0x7FFF & flags) != 0x7FFF)
        {
            u32 event = (u32)flags << 17;

            set_flag(gUnk_03000FC0 + 0x38C, 0, ((event >> 1) + 0x07200000) >> 16, 1);
        }
        obj->layer3Flag599 = 1;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0805AF28/map_put_rect.s", void map_put_rect(struct MapLoaderObj *obj, u32 layer, void *rect))

ASM_FUNC("asm/code_0805AF28/map_refresh_entry.s", void map_refresh_entry(void))

void map_entry_step(struct MapLoaderObj *obj)
{
    if (obj->entryFlags & 0x80)
    {
        u32 timer = obj->entryTimer - 1;

        obj->entryTimer = timer;
        if ((timer << 16) == 0)
        {
            u32 mode = obj->entryFlags & 3;

            if (mode == 1)
            {
                if ((mode &= obj->entryCounter) != 0)
                {
                    if (obj->entryIndex == 0)
                    {
                        obj->entryIndex++;
                        obj->entryCounter++;
                    }
                    else
                        obj->entryIndex--;
                }
                else
                {
                    u8 index = obj->entryIndex;
                    u8 *e = (u8 *)obj->entryTable;

                    if (e[(index << 2) + 2] & 4)
                    {
                        obj->entryIndex--;
                        obj->entryCounter++;
                    }
                    else
                        obj->entryIndex++;
                }
            }
            else if (mode == 0)
            {
                u8 index = obj->entryIndex;
                u8 *e = (u8 *)obj->entryTable;

                if (e[(index << 2) + 2] & 4)
                    obj->entryIndex = 0;
                else
                    obj->entryIndex = index + 1;
            }

            obj->entryTimer = ((struct EntryRec *)obj->entryTable)[obj->entryIndex].t8;
            gUnk_02000052 = ((((struct EntryRec *)obj->entryTable)[obj->entryIndex].mid5 << 8) + ((struct EntryRec *)obj->entryTable)[obj->entryIndex].low5);
        }
        gUnk_02000050 = obj->entryBldcnt;
    }
}

NONMATCH("asm/code_0805AF28/map_obj_setup_state_sprites.s", void map_obj_setup_state_sprites(struct MapLoaderObj *obj))
{
    struct SpriteEntry *table;
    u8 i;
    u8 cur;
    u8 index;

    i = 0;
    do
    {
        obj->stateSpriteSlots[i] |= 0xFF;
        i++;
    } while (i <= 3);

    if (obj->stateSpriteCtx != NULL)
    {
        index = gUnk_03000DD0[8];
        table = (struct SpriteEntry *)gSpriteTable[index];
        if (table != NULL)
        {
            i = 0;
            do
            {
                cur = i;
                if ((table[cur].flags & 0x40) == 0)
                {
                    obj->stateSpriteSlots[cur] = sprite_alloc(obj->stateSpriteCtx, 0xFF, table[cur].arg | 0x5000, 0xFF, 0xFFFF, 0xFFFF);
                }
                i = cur + 1;
            } while ((table[cur].flags & 0x80) == 0);
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0805AF28/init_bg3_scroll_obj.s", void init_bg3_scroll_obj(void))

ASM_FUNC("asm/code_0805AF28/line_scroll_init.s", void line_scroll_init(void))

extern void *gUnk_03000E08;
extern void *gUnk_03000E0C;
extern const u8 gHeapTagDisplayBufA[];
extern const u8 gHeapTagDisplayBufB[];
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void update_scanline_offsets();
void set_0d80_slot(void (*fn)(void), u32 slot);
void bg3_line_scroll_frame(void);

NONMATCH("asm/code_0805AF28/map_obj_init_display.s", void map_obj_init_display(struct MapLoaderObj *obj))
{
    u8 *fp = &obj->displayFlag584;
    u8 flag = *fp;

    if (flag == 0)
    {
        u8 two = 2;
        u16 i;
        void **bufA = &gUnk_03000E08;
        void **bufB = &gUnk_03000E0C;

        u8 *index = &obj->displayIndex585;
        *index = two;
        obj->display58A = 3;
        obj->display58C = flag;
        obj->display590 = 6;
        obj->display592 = flag;
        obj->display588 = two;
        obj->display596 = 8;
        obj->display58E = flag;
        obj->display594 = flag;
        obj->display597 = 3;
        *fp = 1;
        obj->display598 = 0;
        *bufA = heap_alloc(0x500, 1, gHeapTagDisplayBufA, 0);
        *bufB = heap_alloc(0x500, 0, gHeapTagDisplayBufB, 0);
        {
            i = 0;
            do
            {
                u16 *a = (u16 *)((u8 *)*bufA + (i << 2));
                u16 *b = (u16 *)((u8 *)*bufB + (i << 2));
                u16 v = ((u16 *)((u8 *)obj + 0x5A))[*index];

                b[0] = v;
                a[0] = v;
                v = ((u16 *)((u8 *)obj + 0x60))[*index];
                b[1] = v;
                a[1] = v;
                i++;
            } while (i <= 0x9F);
        }
        update_scanline_offsets(obj);
        set_0d80_slot(bg3_line_scroll_frame, 0);
    }
}
END_NONMATCH
