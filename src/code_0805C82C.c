#include "global.h"
struct MapAnimSlot
{
    u8 state;
    u8 filler01;
    u16 animId;
    u8 unk04;
    u8 filler05;
    u8 frame;
    u8 timer;
    u8 unk08;
    u8 filler09;
    u16 x[16];
    u16 y[16];
    u16 filler4A;
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
    u8 filler48[0xAC - 0x48];
    struct MapAnimSlot anim[16];
    u8 filler56C[0x579 - 0x56C];
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


void heap_free_b(void *p);
void menu_entry_refresh(void *ctx, u32 id, u32 value);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void heap_free_a(void *p);
void set_0d80_slot(void (*fn)(void), u32 slot);
void task_remove(void *node, u32 kind);
void cpu_fast_set(const void *src, void *dst, u32 control);
void blit_metatile(u8 *obj, u16 a, s16 flags, u16 c, u16 index);
extern u16 *gUnk_03000E04;
extern void *gUnk_03000E00;
extern void *gUnk_03000E08;
extern void *gUnk_03000E0C;
extern u16 gUnk_02000008;
extern const u32 gObjectTilesets[];
struct BgViewEntry
{
    u32 unk0;
    const u8 *unk4;
    const u8 *unk8;
    u8 unkC;
    u8 filler1[3];
};
extern const struct BgViewEntry gBgViewTable[];
extern const u8 gMapBg3ScrollClass[];
extern const u8 gBgSlotTable[];
extern const u8 gColumnTable[];
extern u8 gUnk_0300034C[];
extern u8 gUnk_02000080[];
extern const u8 gMapPalettesA[];
extern const u8 gMapPalettesB[];
extern const u8 gRomBlockTable[];
extern const u32 gSpriteTable[];
u32 sprite_alloc(void *ctx, u32 a, u32 b, u32 c, u32 d, u32 e);
extern u8 gUnk_03000DD0[];
extern const u32 gLayer3Table[];
void map_layer3_entry(u8 *obj, u32 index);
void update_scanline_offsets();
void bg3_line_scroll_frame(void);
extern const void *gAnimListTable[];

ASM_FUNC("asm/code_0805C82C/map_obj_init_display_b.s", void map_obj_init_display_b(void))

ASM_FUNC("asm/code_0805C82C/update_scanline_offsets.s", void update_scanline_offsets(void))

void bg3_line_scroll_frame(void)
{
    vu16 *regs = (vu16 *)0x04000014;
    u32 hofs = (u32)&regs[4];
    void **slot = &gUnk_03000E0C;
    u16 *table = *slot;

    regs[4] = table[0];
    regs[5] = table[1];
    cpu_fast_set(gUnk_03000E08, table, 0xA0);
    {
        vu32 *dma = (vu32 *)0x040000B0;
        vu16 *cnt = (vu16 *)0x040000BA;
        *cnt = 0;
        dma[0] = (u32)*slot + 2;
        dma[1] = hofs;
        dma[2] = 0xA6400001;
    }
}

void line_scroll_frame_slot(void)
{
    u32 index = gUnk_03000DD0[0xB] & 0x1F;
    u32 offset = ((u32)gBgSlotTable[index << 2] << 30) >> 28;
    vu16 *hofs = (vu16 *)(0x04000014 + offset);
    void **slot = &gUnk_03000E0C;
    u16 *table = *slot;

    *hofs = table[0];
    *(vu16 *)(offset + 0x04000016) = table[1];
    cpu_fast_set(gUnk_03000E08, table, 0xA0);
    {
        vu32 *dma = (vu32 *)0x040000B0;
        vu16 *cnt = (vu16 *)0x040000BA;
        *cnt = 0;
        dma[0] = (u32)*slot + 2;
        dma[1] = (u32)hofs;
        dma[2] = 0xA6400001;
    }
}

void obj_stop_line_scroll(u8 *obj)
{
    *(vu16 *)0x040000BA = 0;
    set_0d80_slot(0, 0);
    if (gUnk_03000E0C != NULL)
        heap_free_a(gUnk_03000E0C);
    if (gUnk_03000E08 != NULL)
        heap_free_a(gUnk_03000E08);
    obj[0x584] = 0;
}

void copy_palette_head_dma(void)
{
    vu16 *pal = (vu16 *)0x05000000;
    u16 *src = gUnk_03000E04;

    *pal = *src;
    {
        vu32 *dma = (vu32 *)0x040000B0;
        vu16 *dmaCntH = (vu16 *)0x040000BA;
        *dmaCntH = 0;
        dma[0] = (u32)(src + 1);
        dma[1] = (u32)pal;
        dma[2] = 0xA2400001;
    }
}

void stub_nop_2_0805CF74(void)
{
}

void map_view_map_obj_destroy(u8 *obj, u32 arg)
{
    *(void **)(obj + 0x18) = (void *)gMapBg3ScrollClass;
    *(vu16 *)0x040000BA = 0;
    set_0d80_slot(0, 0);
    obj[0x1C] = 0;
    if (gUnk_03000E04 != NULL)
        heap_free_a(gUnk_03000E04);
    if (gUnk_03000E00 != NULL)
        heap_free_a(gUnk_03000E00);
    task_remove(obj, arg);
}

void set_state_sprite_value(struct MapLoaderObj *obj, u8 slot, u8 value)
{
    menu_entry_refresh(obj->stateSpriteCtx, obj->stateSpriteSlots[slot], value);
}

void create_state_sprite(struct MapLoaderObj *obj, u8 slot)
{
    void *ctx = obj->stateSpriteCtx;

    if (ctx != NULL)
    {
        u8 sel = gUnk_03000DD0[8];
        const u8 *entries = (const u8 *)gSpriteTable[sel];

        if (entries != NULL)
        {
            const u8 *entry = (const u8 *)(slot * 4 + (u32)entries);

            if (entry[0] & 0x40)
            {
                u16 tile = entry[1] | 0x5000;

                obj->stateSpriteSlots[slot] = sprite_alloc(ctx, 0xFF, tile, 0xFF, 0xFFFF, 0xFFFF);
            }
        }
    }
}

void put_layer3_rect(u8 *obj, u8 *src)
{
    u8 x = src[0];
    u8 y = src[1];
    u8 w = src[2];
    u8 h = src[3];
    u8 i;
    u8 j;
    u16 k = 4;

    for (i = 0; i < h; i++)
    {
        u16 off = (u16)(x + (y + i) * *(u16 *)(obj + 0x26));

        for (j = 0; j < w; j++)
            *(*(u8 **)(obj + 0x9C) + (off + j)) = src[k++];
    }
}

void layer3_entries_by_group(u8 *obj, u16 group)
{
    u8 variant = gUnk_03000DD0[0xA];

    if (variant != 0)
    {
        const u8 *entries = (const u8 *)gLayer3Table[variant * 2];
        u16 k = 0;
        u16 cur;

        do
        {
            cur = k;
            if ((((u32)*(u16 *)(entries + cur * 20 + 0x12)) << 17) >> 24 == group)
                map_layer3_entry(obj, cur);
            k = (u16)(k + 1);
        } while ((entries[cur * 20 + 0x13] & 0x80) == 0);
    }
}

void layer3_entries_by_attr(u8 *obj, u16 attr)
{
    u8 variant = gUnk_03000DD0[0xA];

    if (variant != 0)
    {
        const u8 *entries = (const u8 *)gLayer3Table[variant * 2];
        u16 k = 0;
        u16 cur;

        do
        {
            cur = k;
            if ((((u32)*(u32 *)(entries + cur * 20 + 0x10)) << 9) >> 24 == attr)
                map_layer3_entry(obj, cur);
            k = (u16)(k + 1);
        } while ((entries[cur * 20 + 0x13] & 0x80) == 0);
    }
}

void copy_halfwords_to_obj_buf(u8 *obj, u16 a, u16 count, u16 *src)
{
    u16 i;

    a *= 4;
    for (i = 0; i < count * 4; i++)
        ((struct { u8 filler0[0xA8]; u16 *buf; } *)obj)->buf[a + i] = src[i];
}

void map_anim2_upload(void *unused, u16 slot, u8 count, u16 index)
{
    const u8 *src = (const u8 *)gObjectTilesets + gObjectTilesets[index];

    dma_copy_or_queue((void *)src, (void *)(0x06004000 + slot * 0x20), count * 0x20, 0x20, 0);
}

ASM_FUNC("asm/code_0805C82C/init_bg_view.s", void init_bg_view(void))

ASM_FUNC("asm/code_0805C82C/map_view_transition_step.s", void map_view_transition_step(void))

void map_obj_free_bufs(u8 *obj)
{
    obj[0xA] = 0;
    if (*(void **)(obj + 4) != NULL)
        heap_free_a(*(void **)(obj + 4));
    if (*(void **)obj != NULL)
        heap_free_a(*(void **)obj);
    if (gBgViewTable[obj[0xB]].unkC != 0)
        gUnk_02000008 = 0x1508;
}

void free_block_if_bit0(void *block, u32 flags)
{
    if (flags & 1)
        heap_free_b(block);
}

void clear_byte_0a(u8 *obj)
{
    obj[0xA] = 0;
}

ASM_FUNC("asm/code_0805C82C/init_map_obj.s", void init_map_obj(void))

void upload_map_palettes(u8 *obj);
void anim_slots_load(struct MapLoaderObj *obj);
void map_anim_slots_update(u8 *obj);
ASM_FUNC("asm/code_0805C82C/map_obj_loader_step.s", void map_obj_loader_step(void))

ASM_FUNC("asm/code_0805C82C/upload_map_palettes.s", void upload_map_palettes(u8 *obj))

ASM_FUNC("asm/code_0805C82C/load_layer_columns.s", void load_layer_columns(void))

ASM_FUNC("asm/code_0805C82C/draw_layer_metatiles.s", void draw_layer_metatiles(void))

ASM_FUNC("asm/code_0805C82C/blit_metatile.s", void blit_metatile(u8 *obj, u16 a, s16 flags, u16 c, u16 index))

ASM_FUNC("asm/code_0805C82C/map_anim_slots_update.s", void map_anim_slots_update(u8 *obj))

void anim_slots_clear(struct MapLoaderObj *obj)
{
    u8 i;
    u8 j;

    for (i = 0; i < 0x20; i++)
    {
        obj->anim[i].state = 0;
        obj->anim[i].animId = 0;
        obj->anim[i].unk04 = 0;
        obj->anim[i].filler05 = 0;
        obj->anim[i].frame = 0;
        obj->anim[i].timer = 0;
        obj->anim[i].unk08 = 0;
        for (j = 0; j < 0x10; j++)
        {
            obj->anim[i].x[j] = 0;
            obj->anim[i].y[j] = 0;
        }
    }
}

ASM_FUNC("asm/code_0805C82C/anim_slots_load.s", void anim_slots_load(struct MapLoaderObj *obj))

extern const void *gAnimListTable[];
void map_anim2_start_0805E018(struct MapLoaderObj *obj, u16 animId)
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
                obj->anim[slot].animId = (*(const u32 *)&list[animId] >> 10) & 0x3FF;
                obj->anim[slot].unk04 = ((u32)list[animId].unk2 << 23) >> 27;

                chunk = (const struct MapAnimChunk *)list[animId].chunks;
                i = 0;
                do
                {
                    j = i;
                    obj->anim[slot].x[j] = chunk[j].x;
                    obj->anim[slot].y[j] = chunk[j].y & 0x7FFF;
                    i = (u8)(i + 1);
                } while (!(chunk[j].y & 0x8000));

                obj->anim[slot].frame = i;
                obj->anim[slot].timer = 0;
                obj->anim[slot].unk08 = 0;
            }
        }
    }
}
