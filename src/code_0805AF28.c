#include "global.h"

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
    u8 filler48[0x579 - 0x48];
    u8 layer3Flag579;
    u8 filler57A[0x57C - 0x57A];
    void *stateSpriteCtx;
    u8 stateSpriteSlots[4];
    u8 filler584[0x599 - 0x584];
    u8 layer3Flag599;
    u8 filler59A[0x768 - 0x59A];
    void *spriteCtx;
    u8 spriteSlots[4];
    u8 animLock;
};

extern u16 gUnk_02000010;
extern u16 gUnk_02000014;
extern u16 gUnk_02000016;
extern const u32 gObjectTilesets[];
extern const u32 gMapObjDescriptor[];
extern const u32 gMetatileSets[];
extern const u32 gSpriteTable[];
extern u8 gUnk_03000DD0[];
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

ASM_FUNC("asm/code_0805AF28/map_load_chunks.s", void map_load_chunks(void))

ASM_FUNC("asm/code_0805AF28/map_blit_chunks.s", void map_blit_chunks(void))

ASM_FUNC("asm/code_0805AF28/map_blit_tile.s", void map_blit_tile(struct MapLoaderObj *obj, u16 layer, s16 col, u16 row, u32 tile))

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

ASM_FUNC("asm/code_0805AF28/map_obj_write_scroll.s", void map_obj_write_scroll(void))

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

ASM_FUNC("asm/code_0805AF28/map_anim_slots_step.s", void map_anim_slots_step(void))

ASM_FUNC("asm/code_0805AF28/map_blit_tile_rect.s", void map_blit_tile_rect(void))

ASM_FUNC("asm/code_0805AF28/map_anim_update.s", void map_anim_update(void))

ASM_FUNC("asm/code_0805AF28/map_anim_init.s", void map_anim_init(void))

ASM_FUNC("asm/code_0805AF28/map_loader_map_obj_setup_sprites.s", void map_loader_map_obj_setup_sprites(void))

void map_upload_tiles(u32 unused, u16 slot, u8 count, u16 index)
{
    dma_copy_or_queue((u8 *)gObjectTilesets + gObjectTilesets[index], (void *)(0x06000000 + slot * 0x20), count * 0x20, 0x20, 0);
}

ASM_FUNC("asm/code_0805AF28/map_anim2_update.s", void map_anim2_update(void))

ASM_FUNC("asm/code_0805AF28/map_anim2_init.s", void map_anim2_init(void))

ASM_FUNC("asm/code_0805AF28/map_anim2_start_0805BCCC.s", void map_anim2_start_0805BCCC(void))

ASM_FUNC("asm/code_0805AF28/map_check_layer3.s", void map_check_layer3(void))

ASM_FUNC("asm/code_0805AF28/map_layer3_entry.s", void map_layer3_entry(struct MapLoaderObj *obj, u32 index))

ASM_FUNC("asm/code_0805AF28/map_put_rect.s", void map_put_rect(struct MapLoaderObj *obj, u32 layer, void *rect))

ASM_FUNC("asm/code_0805AF28/map_refresh_entry.s", void map_refresh_entry(void))

ASM_FUNC("asm/code_0805AF28/map_entry_step.s", void map_entry_step(void))

ASM_FUNC("asm/code_0805AF28/map_obj_setup_state_sprites.s", void map_obj_setup_state_sprites(void))

ASM_FUNC("asm/code_0805AF28/init_bg3_scroll_obj.s", void init_bg3_scroll_obj(void))

ASM_FUNC("asm/code_0805AF28/line_scroll_init.s", void line_scroll_init(void))

ASM_FUNC("asm/code_0805AF28/map_obj_init_display.s", void map_obj_init_display(void))
