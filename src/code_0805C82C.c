#include "global.h"
// A map tile animation (map_anim2_start_0805E018 starts one, map_anim_slots_update plays it).
struct MapAnimSlot
{
    u8 state;      // 0: free; 0x10: waiting for frameTimer, 0x20: next frame due; low nibble: mode (0 loop, 1 ping-pong, 2 once)
    u8 filler01;
    u16 vramTile;  // first tile it overwrites, from 0x06004000
    u8 tileCount;  // tiles per frame
    u8 frameTimer; // frames left on the current frame
    u8 frameCount;
    u8 curFrame;
    u8 animFlags;  // bit 0: ping-pong running backwards
    u8 filler09;
    u16 frameTiles[16]; // gObjectTilesets index of each frame
    u16 frameTimes[16]; // duration of each frame
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
struct SysHeadPal
{
    u8 filler0[2];
    u16 palDirty;
};
struct MetatileMapObj
{
    u8 filler0[0x1E];
    u16 selector;
    u8 filler20[0x94 - 0x20];
    u16 *bgBuf;
    u16 *tilemap;
    u8 filler9C[0xA8 - 0x9C];
    u16 (*metatiles)[4];
};


void heap_free_b(void *p);
void menu_entry_refresh(void *ctx, u32 id, u32 value);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void heap_free_a(void *p);
void set_0d80_slot(void (*fn)(void), u32 slot);
void task_remove(void *node, u32 kind);
void cpu_fast_set(const void *src, void *dst, u32 control);
typedef u32 MetatileId;
void blit_metatile(u8 *obj, u16 layer, s16 col, u16 row, MetatileId tileArg);
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
extern u8 gSys[];
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
extern const u8 gHeapTagBgViewSmall[];
extern const u8 gHeapTagBgViewLarge[];
extern u16 gUnk_02000010[];
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
extern const u8 gHeapTagDisplayBufA[];
extern const u8 gHeapTagDisplayBufB[];

NONMATCH("asm/code_0805C82C/map_obj_init_display_b.s", void map_obj_init_display_b(u8 *obj))
{
    u8 *flagp = obj + 0x584;
    u8 flag = *flagp;
    u8 *indexp;
    u8 *table;
    s32 zero;
    s32 value;
    u32 i;

    if (flag == 0)
    {
        indexp = obj + 0x585;
        *indexp = 2;
        zero = 0;
        *(u16 *)(obj + 0x58A) = 3;
        *(u16 *)(obj + 0x58C) = 2;
        *(u16 *)(obj + 0x590) = 6;
        *(u16 *)(obj + 0x592) = 4;
        obj[0x588] = 2;
        obj[0x596] = 8;
        *(u16 *)(obj + 0x58E) = flag;
        *(u16 *)(obj + 0x594) = flag;
        obj[0x597] = 2;
        *flagp = 1;
        table = obj + 0x60;
        value = *(s16 *)(table + (*indexp << 1));
        if (value <= 0x97)
        {
            s32 base = -104;

            obj[0x586] = zero;
            obj[0x587] = (u8)(base - table[*indexp << 1]);
        }
        else
        {
            obj[0x586] = 0xA0;
            obj[0x587] = zero;
        }
        gUnk_03000E08 = heap_alloc(0x500, 1, gHeapTagDisplayBufA, 0);
        gUnk_03000E0C = heap_alloc(0x500, 0, gHeapTagDisplayBufB, 0);
        i = 0;
        do
        {
            u32 off = i << 2;
            u16 *a = (u16 *)((u8 *)gUnk_03000E08 + off);
            u16 *b = (u16 *)((u8 *)gUnk_03000E0C + off);
            u16 v = *(u16 *)(obj + 0x5A + (*indexp << 1));

            b[0] = v;
            a[0] = v;
            v = *(u16 *)(table + (*indexp << 1));
            b[1] = v;
            a[1] = v;
            i = (u16)(i + 1);
        } while (i <= 0x9F);
        update_scanline_offsets(obj);
        set_0d80_slot(bg3_line_scroll_frame, 0);
    }
}
END_NONMATCH
extern s16 gSineTable[];

static inline s32 scan_wave(u32 t, u16 freq, u16 amp)
{
    s32 s = gSineTable[(t * freq) & 0xFF];

    if (s < 0)
        s += 0x3F;
    s >>= 6;
    s = s * amp;
    if (s < 0)
        s += 0xFF;
    return s >> 8;
}

static inline u32 scan_idx2(u8 *obj)
{
    return obj[0x585] << 1;
}

static inline s32 scan_base_y(u8 *obj)
{
    return *(s16 *)(obj + 0x60 + scan_idx2(obj));
}

static inline u16 scan_base_x(u8 *obj)
{
    return *(u16 *)(obj + 0x5A + scan_idx2(obj));
}

NONMATCH("asm/code_0805C82C/update_scanline_offsets.s", void update_scanline_offsets(u8 *obj))
{
    u32 phaseB;
    u32 i;

    {
        u32 base = (s32)*(s16 *)(obj + 0x5A + scan_idx2(obj)) + (scan_base_y(obj) << 16);
        u32 *buf = gUnk_03000E08;
        u8 k = 0;

        do
        {
            buf[k] = base;
            k = (u8)(k + 1);
        } while (k <= 0x9F);
    }

    {
        u32 phaseA = *(u16 *)(obj + 0x58E) >> 2;

        phaseB = *(u16 *)(obj + 0x594) >> 2;
        if (obj[0x597] == 0)
        {
            if (obj[0x586] <= obj[0x587])
            {
                i = obj[0x586];
                do
                {
                    u16 *ent = (u16 *)((u8 *)gUnk_03000E08 + (i << 2));

                    ent[0] = (u16)(scan_wave(i + phaseA + scan_base_y(obj), *(u16 *)(obj + 0x58A), *(u16 *)(obj + 0x58C)) + scan_base_x(obj));
                    i = (u16)(i + 1);
                } while (i <= obj[0x587]);
            }
        }
        else if (obj[0x597] == 1)
        {
            if (obj[0x586] <= obj[0x587])
            {
                i = obj[0x586];
                do
                {
                    u16 *ent = (u16 *)((u8 *)gUnk_03000E08 + (i << 2));

                    ent[1] = (u16)(scan_wave(i + phaseA + scan_base_y(obj), *(u16 *)(obj + 0x58A), *(u16 *)(obj + 0x58C)) + *(u16 *)(obj + 0x60 + scan_idx2(obj)));
                    i = (u16)(i + 1);
                } while (i <= obj[0x587]);
            }
        }
        else if (obj[0x597] == 2)
        {
            if (scan_base_y(obj) > 0x97)
            {
                obj[0x586] = 0xA0;
                obj[0x587] = 0;
            }
            else
            {
                obj[0x586] = 0;
                obj[0x587] = (u8)(-104 - obj[0x60 + scan_idx2(obj)]);
            }
            if (obj[0x586] <= obj[0x587])
            {
                i = obj[0x586];
                do
                {
                    u16 *ent = (u16 *)((u8 *)gUnk_03000E08 + (i << 2));

                    ent[0] = (u16)(scan_wave(i + phaseA + scan_base_y(obj), *(u16 *)(obj + 0x58A), *(u16 *)(obj + 0x58C)) + scan_base_x(obj));
                    ent[1] = (u16)(scan_wave(i + phaseB + scan_base_y(obj), *(u16 *)(obj + 0x590), *(u16 *)(obj + 0x592)) + *(u16 *)(obj + 0x60 + scan_idx2(obj)));
                    i = (u16)(i + 1);
                } while (i <= obj[0x587]);
            }
        }
        else
        {
            if (scan_base_y(obj) > 0x97)
            {
                obj[0x586] = 0xA0;
                obj[0x587] = 0;
            }
            else
            {
                obj[0x586] = 0;
                obj[0x587] = (u8)(-104 - obj[0x60 + scan_idx2(obj)]);
            }
            obj[0x598] = (u8)(obj[0x598] + 1);
            if (obj[0x598] > 0x1D)
            {
                obj[0x598] = 0;
                if (*(u16 *)(obj + 0x592) <= 3)
                    *(u16 *)(obj + 0x592) = *(u16 *)(obj + 0x592) + 1;
                else if (*(u16 *)(obj + 0x58C) <= 1)
                    *(u16 *)(obj + 0x58C) = *(u16 *)(obj + 0x58C) + 1;
            }
            if (obj[0x586] <= obj[0x587])
            {
                i = obj[0x586];
                do
                {
                    u16 *ent = (u16 *)((u8 *)gUnk_03000E08 + (i << 2));

                    ent[0] = (u16)(scan_wave(i + phaseA + scan_base_y(obj), *(u16 *)(obj + 0x58A), *(u16 *)(obj + 0x58C)) + scan_base_x(obj));
                    ent[1] = (u16)(scan_wave(i + phaseB + scan_base_y(obj), *(u16 *)(obj + 0x590), *(u16 *)(obj + 0x592)) + *(u16 *)(obj + 0x60 + scan_idx2(obj)));
                    i = (u16)(i + 1);
                } while (i <= obj[0x587]);
            }
        }
    }

    *(u16 *)(obj + 0x58E) = *(u16 *)(obj + 0x58E) + obj[0x588];
    *(u16 *)(obj + 0x594) = *(u16 *)(obj + 0x594) + obj[0x596];
}
END_NONMATCH

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

NONMATCH("asm/code_0805C82C/init_bg_view.s", u32 init_bg_view(u8 *obj, u8 mode))
{
    u32 fill;
    u16 *tiles;
    const u8 *pattern;
    u32 x;
    u32 y;

    if (obj[0xA] != 0)
        return 0;
    *(void **)(obj + 4) = heap_alloc(0x200, 1, gHeapTagBgViewSmall, 0);
    fill = gBgViewTable[mode].unk0;
    cpu_fast_set(&fill, *(void **)(obj + 4), 0x01000080);
    dma_copy_or_queue(*(void **)(obj + 4), (void *)0x06003E00, 0x200, 0x20, 2);
    *(void **)obj = heap_alloc(0x500, 1, gHeapTagBgViewLarge, 0);
    fill = 0;
    cpu_fast_set(&fill, *(void **)obj, 0x01000140);
    pattern = gBgViewTable[mode].unk4;
    y = 0;
    tiles = *(u16 **)obj;
    do
    {
        u32 nextY;
        u32 rowBase;
        u32 fold;
        u32 mirror;

        x = 0;
        nextY = y + 1;
        rowBase = y << 5;
        fold = (y << 4) - y;
        mirror = (0x13 - y) << 5;
        do
        {
            u32 x2 = x - 0x1D;

            tiles[rowBase + x] = (u16)(0xF1F0 + pattern[fold + x]);
            tiles[rowBase - x2] = (u16)(0xF5F0 + pattern[fold + x]);
            tiles[mirror + x] = (u16)(0xF9F0 + pattern[fold + x]);
            tiles[mirror - x2] = (u16)(0xFDF0 + pattern[fold + x]);
            x = (u16)(x + 1);
        } while (x <= 0xE);
        y = (u16)nextY;
    } while (y <= 9);
    dma_copy_or_queue(tiles, (void *)0x06003000, 0x500, 0x20, 2);
    gUnk_02000008 = 0x600;
    gUnk_02000010[0] = 0;
    gUnk_02000010[1] = 0;
    obj[0xB] = mode;
    *(u16 *)(obj + 8) = 0;
    obj[0xA] = 1;
}
END_NONMATCH

NONMATCH("asm/code_0805C82C/map_view_transition_step.s", u32 map_view_transition_step(u8 *obj))
{
    const u8 *bits;
    u8 *buf;
    u32 k;

    if (obj[0xA] == 0)
        return 0;
    bits = gBgViewTable[obj[0xB]].unk8;
    k = 0;
    buf = *(u8 **)(obj + 4);
    do
    {
        u32 step = *(u16 *)(obj + 8);
        u32 next = k + 1;

        if (k <= step)
        {
            u32 n = (u16)(step - k);
            u32 row;
            s32 three;
            u8 j;

            if (n > 0xF)
                n = 0xF;
            j = 0;
            three = 3;
            row = k << 5;
            do
            {
                    s32 v = (bits[((j >> 2) << 4) + n] >> ((three & ~j) << 1)) & three;
                    if (v == 0)
                        *(*(u8 **)(obj + 4) + (row + j)) = 0;
                    else if (v == 1)
                        *(*(u8 **)(obj + 4) + (row + j)) = 0x90;
                    else if (v == 2)
                        *(*(u8 **)(obj + 4) + (row + j)) = 9;
                    else if (v == 3)
                        *(*(u8 **)(obj + 4) + (row + j)) = 0x99;
                j = (u8)(j + 1);
                buf = *(u8 **)(obj + 4);
            } while (j <= 0x1F);
        }
        k = (u16)next;
    } while (k <= 0xF);
    dma_copy_or_queue(buf, (void *)0x06003E00, 0x200, 0x20, 0);
    {
        u32 step = *(u16 *)(obj + 8) + 1;

        *(u16 *)(obj + 8) = step;
        if ((u16)step > 0x20)
        {
            obj[0xA] = 0;
            if (*(void **)(obj + 4) != NULL)
                heap_free_a(*(void **)(obj + 4));
            if (*(void **)obj != NULL)
                heap_free_a(*(void **)obj);
            if (gBgViewTable[obj[0xB]].unkC != 0)
                gUnk_02000008 = 0x1508;
            return 0;
        }
    }
    return 1;
}
END_NONMATCH

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

static inline const u8 *map_palette_set(void)
{
    if ((gSys[0x888] & 6) == 2)
        return gMapPalettesB;
    return gMapPalettesA;
}

/* Loads the current map's BG palettes (from gMapPalettesB in link mode, else gMapPalettesA; the
   set starts with a table of offsets, picked by gRomBlockTable[gUnk_03000DD0[5]]) into the BG
   palette shadow gUnk_02000080. Each of palettes 0-14 is copied only if one of its colours 1-15
   isn't black, so palettes the map leaves empty keep what's there. Then all 15 are marked dirty. */
void upload_map_palettes(u8 *obj)
{
  register const u8 *set asm("r2");
  const u8 *palettes;
  u16 pal;
  u16 color;
  u8 used;

  if ((gSys[0x888] & 6) != 2) /* not in link mode */
  {
    goto single_player;
  }
  set = gMapPalettesB;
  goto have_set;
single_player:
  set = gMapPalettesA;

have_set:
  palettes = set + ((const u32 *)set)[gRomBlockTable[gUnk_03000DD0[5]]];

  for (pal = 0; pal < 15; pal++)
  {
    /* 16 colours of 2 bytes per palette; colour 0 is transparent */
    used = 0;
    for (color = 1; color < 16; color++)
    {
      if ((palettes[(pal * 32) + (color * 2)] != 0) || (palettes[((pal * 32) + (color * 2)) + 1] != 0))
      {
        used++;
      }
    }

    if (used != 0)
    {
      const u8 *src = palettes + (pal * 32);
      register u8 *shadow asm("r2") = gUnk_02000080;
      cpu_fast_set(src, (u8 *)((pal * 32) + (u32)shadow), 32 / 4);
    }
  }

  ((struct SysHeadPal *)gSys)->palDirty = 0x7FFF;
}

ASM_FUNC("asm/code_0805C82C/load_layer_columns.s", void load_layer_columns(void))


NONMATCH("asm/code_0805C82C/draw_layer_metatiles.s", void draw_layer_metatiles(u8 *obj))
{
    u16 layer = 0;
    u32 next;
    u32 base;
    s16 x;
    u16 y;

    do
    {
        if (*(u16 *)(gColumnTable + ((2 - layer) << 1) + (*(u16 *)(obj + 0x1E) << 3)) != 0xFFFF)
        {
            next = layer + 1;
            base = layer << 11;
            for (y = 0; (s16)y <= 15; y++)
            {
                u32 rowBase = ((((s16)y) * 3) << 4) + base;

                for (x = 0; x <= 31; x++)
                {
                    u16 tile = (*(u16 **)(obj + 0x98))[rowBase + x];

                    blit_metatile(obj, layer, x, (s16)y, tile);
                }
            }
        }
        else
        {
            obj[0xA5A] = layer;
            next = layer + 1;
        }
        layer = (u16)next;
    } while (layer <= 2);
}
END_NONMATCH

/*
 * Draws metatile `tileArg` (4 tilemap entries: top-left, top-right, bottom-left, bottom-right) at
 * metatile column col, row row into BG layer `layer` of the map's tilemap buffer (obj is a
 * struct MetatileMapObj); the same as draw_metatile_2x2. A layer is 64 x 32 tiles (0x800 entries):
 * two 32 x 32 screen blocks side by side, so bit 4 of col picks the right one (+0x400) and the low
 * 4 bits of col and row the place in it.
 */
void blit_metatile(u8 *obj, u16 layer, s16 col, u16 row, MetatileId tileArg)
{
    register u32 y asm("r5") = row;
    u16 tile = tileArg;
    u16 (*metatiles)[4] = ((struct MetatileMapObj *)obj)->metatiles;
    u16 top;
    u16 bottom;

    if ((col & 0x10) == 0)
    {
        int mask = 0xF;
        int base = layer << 11;
        int ym = y & mask;
        top = base + (ym << 6) + ((col & mask) << 1);
        bottom = base + (((ym << 1) + 1) << 5) + ((col & mask) << 1);
    }
    else
    {
        int mask = 0xF;
        int base = layer << 11;
        int ym = y & mask;
        top = base + (ym << 6) + ((col & mask) << 1) + 0x400;
        bottom = base + (((ym << 1) + 1) << 5) + ((col & mask) << 1) + 0x400;
    }
    ((struct MetatileMapObj *)obj)->bgBuf[top] = metatiles[tile][0];
    ((struct MetatileMapObj *)obj)->bgBuf[top + 1] = metatiles[tile][1];
    ((struct MetatileMapObj *)obj)->bgBuf[bottom] = metatiles[tile][2];
    ((struct MetatileMapObj *)obj)->bgBuf[bottom + 1] = metatiles[tile][3];
}
ASM_FUNC("asm/code_0805C82C/map_anim_slots_update.s", void map_anim_slots_update(u8 *obj))


void anim_slots_clear(struct MapLoaderObj *obj)
{
    u8 i;
    u8 j;

    for (i = 0; i < 0x20; i++)
    {
        obj->anim[i].state = 0;
        obj->anim[i].vramTile = 0;
        obj->anim[i].tileCount = 0;
        obj->anim[i].frameTimer = 0;
        obj->anim[i].frameCount = 0;
        obj->anim[i].curFrame = 0;
        obj->anim[i].animFlags = 0;
        for (j = 0; j < 0x10; j++)
        {
            obj->anim[i].frameTiles[j] = 0;
            obj->anim[i].frameTimes[j] = 0;
        }
    }
}

ASM_FUNC("asm/code_0805C82C/anim_slots_load.s", void anim_slots_load(struct MapLoaderObj *obj))


extern const void *gAnimListTable[];
// Starts map tile animation entryId of the current map's list (gAnimListTable[gUnk_03000DD0[7]])
// in the first free one of the 16 slots: its VRAM tile, tile count, play mode and the frame list
// (tileset index and duration per frame, the last one marked by bit 15). State 0x20 makes
// map_anim_slots_update upload the first frame. The enable bit (0x40) is read from the entry at the
// slot's index, not entryId's.
void map_anim2_start_0805E018(struct MapLoaderObj *obj, u16 entryId)
{
    u8 slot = 0;
    u8 count;
    u8 n;

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
        u8 mapAnims = gUnk_03000DD0[7];
        const struct MapAnimEntry *list = (const struct MapAnimEntry *)gAnimListTable[mapAnims];

        if (list != NULL)
        {
            u8 enable = list[slot].flag;

            if (enable & 0x40)
            {
                const struct MapAnimChunk *frames;

                obj->anim[slot].state = (((u32)list[entryId].type << 30) >> 30) + 0x20;
                obj->anim[slot].vramTile = (*(const u32 *)&list[entryId] >> 10) & 0x3FF;
                obj->anim[slot].tileCount = ((u32)list[entryId].unk2 << 23) >> 27;

                frames = (const struct MapAnimChunk *)list[entryId].chunks;
                count = 0;
                do
                {
                    n = count;
                    obj->anim[slot].frameTiles[n] = frames[n].x;
                    obj->anim[slot].frameTimes[n] = frames[n].y & 0x7FFF;
                    count = (u8)(count + 1);
                } while (!(frames[n].y & 0x8000));

                obj->anim[slot].frameCount = count;
                obj->anim[slot].curFrame = 0;
                obj->anim[slot].animFlags = 0;
            }
        }
    }
}
