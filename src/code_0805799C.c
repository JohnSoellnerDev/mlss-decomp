#include "global.h"

struct SceneObj
{
    u8 filler0[0x18];
    u8 *unk18;
    u8 filler1C[0x3C - 0x1C];
    void *unk3C;
};

struct MapObj
{
    u8 filler0[0x18];
    void *unk18;
    u8 filler1C[0x94 - 0x1C];
    void *unk94;
    void *unk98;
    void *unk9C;
    void *unkA0;
    void *unkA4;
    void *unkA8;
    u8 fillerAC[0xB8 - 0xAC];
    void *unkB8;
};

/* Task holding a number as decimal digits (set_obj_digits), most significant digit first. */
struct DigitsObj
{
    u8 filler0[0x18];
    u8 *vtable;        /* gObjDigitsClass */
    u8 digits[6];      /* a 6-digit number */
    u8 smallDigits[2]; /* a 2-digit number */
};

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

struct TrackObj
{
    u8 filler0[0x1E];
    u16 selector;
    u8 filler20[0x36 - 0x20];
    u16 widthPx[2];
    u8 filler3A[0x3C - 0x3A];
    u16 heightPx[2];
    u8 filler40[0x42 - 0x40];
    u16 mapCols[2];
    u8 filler46[0x48 - 0x46];
    u16 mapRows[2];
    u8 filler4C[0x4E - 0x4C];
    u16 posA[2];
    s16 camX;
    u16 posB[2];
    s16 camY;
    u16 dstA[2];
    s16 velX;
    u16 dstB[2];
    s16 velY;
    s16 stepA[2];
    u16 deltaX;
    s16 stepB[2];
    u16 deltaY;
    u16 baseA[2];
    u8 filler76[0x78 - 0x76];
    u16 baseB[2];
    u8 filler7C[0x7E - 0x7C];
    s16 scaleA[2];
    u8 filler82[0x84 - 0x82];
    s16 scaleB[2];
    u8 filler88[0x8A - 0x88];
    u8 flags[2];
    u8 filler8C;
    u8 phase[2];
    u8 filler8F;
    u8 rate[2];
};

struct MapStateScroll
{
    u8 filler0[0xC];
    u16 scroll;
};

extern u8 gSceneChildClass[];
extern u8 gMapSceneClass[];
extern u8 gObjDigitsClass[];
extern u8 gMapObjClass[];
extern void *gUnk_03000DA0;
extern u8 gUnk_02000080[];
void obj_stop_line_scroll(u8 *obj);
void dma_cancel_request(u32 addr);
void heap_free_a(void *p);
void heap_free_b(void *p);
void frame_update(void);
extern u8 gSceneStepClass[];
extern u8 gSys[];
extern struct ObjLists gUnk_0203FFB8;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void run_pending_obj_callbacks(void);
void obj_list_pass_0a8c(void);
extern u8 gUnk_03000DD0[];
extern u8 gUnk_0203FFF8[];
void set_irq_callback(u32 kind, void *cb);
u8 *ptr_03000ffc_plus_1ff8(void);
u32 eeprom_save_slots(void *rec, u32 mode);
extern const u8 gColumnTable[];
extern s16 gWaveX[];
extern s16 gWaveY[];
void scroll_map_slot(struct TrackObj *obj, u32 slot, s16 a, s16 b);
void redraw_metatile_column(struct TrackObj *obj, u16 slot, s16 col, s16 row);
void redraw_metatile_row(struct TrackObj *obj, u16 slot, s16 col, s16 row);
void redraw_map_row(struct TrackObj *obj, u16 slot, s16 col, s16 row, s16 dstCol, s16 dstRow);
void redraw_map_column(struct TrackObj *obj, u16 slot, s16 col, s16 row, s16 dstCol, s16 dstRow);
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void cpu_fast_set(const void *src, void *dst, u32 control);
extern const u8 gHeapTagObjBgLayers[];
extern u8 gUnk_0200000A[];

ASM_FUNC("asm/code_0805799C/oam_draw_unit_markers.s", void oam_draw_unit_markers(void))

/* Starts a digits task (struct DigitsObj, priority kind) showing `value` as 6 decimal digits and
   `small` as 2, most significant digit first; higher digits are dropped. The map scene fills one
   from the save. The digits are written by byte offset: field access changes the code. */
u8 *set_obj_digits(u8 *obj, u8 kind, u32 unusedTag, u32 value, u8 small)
{
    s32 i;
    u8 *tens;

    task_insert(obj, kind);
    ((struct DigitsObj *)obj)->vtable = gObjDigitsClass;
    for (i = 0; i < 6; i++)
    {
        obj[0x21 - i] = value % 10; /* digits[5 - i], from the last digit up */
        value /= 10;
    }
    tens = obj + 0x22;              /* smallDigits[0] */
    obj[0x23] = small % 10;         /* smallDigits[1] */
    *tens = (u8)(small / 10) % 10;
    return obj;
}

void map_scene_free(struct MapObj *obj, u32 kind)
{
    vu16 *dma;
    u8 *sys;
    u32 one;
    u16 zero;
    u16 v;

    obj->unk18 = gMapSceneClass;
    dma = (vu16 *)REG_ADDR_DMA0SAD;
    dma[5] = dma[5] & 0xC5FF;
    dma[5] = dma[5] & 0x7FFF;
    (void)dma[5];
    set_irq_callback(2, NULL);
    if (obj->unkB8 != NULL)
    {
        task_remove(obj->unkB8, 3);
    }
    heap_free_b(gUnk_03000DA0);
    run_pending_obj_callbacks();
    *(u16 *)gUnk_02000080 = 0x7FFF;
    sys = gSys;
    v = *(u16 *)(sys + 2);
    one = 1;
    zero = 0;
    one |= v;
    *(u16 *)(sys + 2) = one;
    *(u16 *)0x02000000 = zero;
    sys[0x31] = 2;
    frame_update();
    task_remove(obj, kind);
}

void flip_oam_buffer(void)
{
    u8 *base = gSys;
    u32 *oamPtr = (u32 *)(base + 0x880);
    u32 oam = *oamPtr;

    obj_list_pass_0a8c();
    gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
    oam = gUnk_03000D4C(gUnk_0203FFB8.root, oam);
    *oamPtr = oam;
}

void scene_step_free(struct SceneObj *obj, u32 kind)
{
    obj->unk18 = gSceneStepClass;
    run_pending_obj_callbacks();
    if (obj->unk3C != NULL)
    {
        task_remove(obj->unk3C, 3);
    }
    task_remove(obj, kind);
}

struct SceneObj *scene_child_init(struct SceneObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = gSceneChildClass;
    return obj;
}

void sio_send_callback(void)
{
    REG_SIOMLT_SEND = 0xFEFE;
}

void sio_reset_and_ack(void)
{
    REG_IME = 0;
    REG_IE = REG_IE & 0xFF7F;
    REG_IF = REG_IF | 0x80;
    REG_IME = 1;
    REG_IME = 0;
    REG_SIOCNT = 0x2003;
    REG_IME = 1;
}

void sio_multi_setup(void)
{
    set_irq_callback(1, sio_send_callback);
    REG_IME = 0;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT = REG_SIOCNT | 0x4003;
    REG_SIOMLT_SEND = 0xFEFE;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE = REG_IE | 0x80;
    REG_IF = REG_IF | 0x80;
    REG_IME = 1;
}

u32 check_warm_reset_counter(void)
{
    u8 *rec;
    u8 *fresh;
    u32 saved;
    u8 savedB;
    u32 changed;
    u8 result;

    rec = ptr_03000ffc_plus_1ff8();
    saved = *(u32 *)rec;
    savedB = rec[4];
    changed = 0;
    if (*(u32 *)gUnk_0203FFF8 > saved)
    {
        changed = 1;
        *(u32 *)rec = *(u32 *)gUnk_0203FFF8;
    }
    if (gUnk_0203FFF8[4] > savedB)
    {
        changed = 1;
        rec[4] = gUnk_0203FFF8[4];
    }
    if (changed != 0)
    {
        result = (u8)eeprom_save_slots(rec, 0);
        if (result == 0)
        {
            fresh = ptr_03000ffc_plus_1ff8();
            *(u32 *)fresh = 0x4E20;
            fresh[4] = 3;
            *(u32 *)gUnk_0203FFF8 = 0x4E20;
            gUnk_0203FFF8[4] = fresh[4];
        }
        return result;
    }
    return 1;
}

ASM_FUNC("asm/code_0805799C/map_obj_start.s", void map_obj_start(void))


void obj_release_parts(struct MapObj *obj, u32 arg)
{
    obj->unk18 = gMapObjClass;
    if (((u8 *)obj)[0x584] != 0)
    {
        obj_stop_line_scroll((u8 *)obj);
    }
    dma_cancel_request(0x06000000);
    if (obj->unkA4 != NULL)
    {
        heap_free_a(obj->unkA4);
    }
    if (obj->unkA0 != NULL)
    {
        heap_free_a(obj->unkA0);
    }
    if (obj->unk9C != NULL)
    {
        heap_free_a(obj->unk9C);
    }
    if (obj->unkA8 != NULL)
    {
        heap_free_a(obj->unkA8);
    }
    if (obj->unk98 != NULL)
    {
        heap_free_a(obj->unk98);
    }
    if (obj->unk94 != NULL)
    {
        heap_free_a(obj->unk94);
    }
    task_remove(obj, arg);
}

/*
 * Sets the map scene's BG layers up: allocates and clears its 8 KiB layer buffer (obj+0xA4) and
 * writes the BG1-BG3 control shadows (gUnk_0200000A, mirrors of REG_BG1CNT..REG_BG3CNT): 512x256
 * text BGs with tiles in char block 1 and maps at screen blocks 0, 2 and 4. Their priorities come
 * from bits 5-6 of the current area entry's byte 2 (gUnk_03000DD0[2]); an unknown order leaves all 0.
 */
void init_obj_bg_layers(u8 *obj)
{
    u32 zero = 0;
    u16 phase = 0xFFFB;
    void **bufSlot;
    void *buf;
    u8 layerOrder;
    u16 bg1Priority;
    u16 bg2Priority;
    u16 bg3Priority;

    *(u16 *)(obj + 0x1C) = phase;
    obj[0x578] = 1;
    bufSlot = (void **)(obj + 0xA4);
    buf = heap_alloc(0x2000, 1, gHeapTagObjBgLayers, 0);
    *bufSlot = buf;
    {
        u32 fill = zero;

        // Fill 0x800 words (8 KiB) with 0.
        cpu_fast_set(&fill, buf, 0x1000800);
    }
    layerOrder = gUnk_03000DD0[2] & 0x60;
    bg1Priority = 0;
    bg2Priority = 0;
    bg3Priority = 0;
    switch (layerOrder)
    {
    case 0:
        bg1Priority = 1;
        bg2Priority = 2;
        bg3Priority = 3;
        break;
    case 0x20:
        bg1Priority = 1;
        bg2Priority = 3;
        bg3Priority = 2;
        break;
    case 0x40:
        bg1Priority = 2;
        bg2Priority = 3;
        bg3Priority = 1;
        break;
    }
    {
        register vu16 *bgCnt asm("r1") = (vu16 *)gUnk_0200000A;

        *bgCnt++ = bg1Priority | (BGCNT_TXT512x256 | BGCNT_SCREENBASE(0) | BGCNT_CHARBASE(1));
        *bgCnt++ = bg2Priority | (BGCNT_TXT512x256 | BGCNT_SCREENBASE(2) | BGCNT_CHARBASE(1));
        *bgCnt = bg3Priority | (BGCNT_TXT512x256 | BGCNT_SCREENBASE(4) | BGCNT_CHARBASE(1));
    }
}

u8 clamp_camera_to_map(struct TrackObj *obj, s16 x, s16 y)
{
    s16 cx;
    s16 cy;
    u8 flags;
    u32 slot;

    cx = x;
    cy = y;
    flags = 0;
    slot = 2;
    if (cx <= 0)
    {
        cx = 0;
        flags = 4;
    }
    else if (cx >= obj->widthPx[slot] - 240)
    {
        cx = obj->widthPx[slot] - 240;
        flags = 8;
    }
    if (cy <= 0)
    {
        cy = 0;
        flags |= 1;
    }
    else if (cy >= obj->heightPx[slot] - 160)
    {
        cy = obj->heightPx[slot] - 160;
        flags |= 2;
    }
    obj->stepA[slot] = cx - (s16)obj->posA[slot];
    obj->stepB[slot] = cy - (s16)obj->posB[slot];
    return flags;
}

ASM_FUNC("asm/code_0805799C/map_obj_slots_obj_state_step.s", void map_obj_slots_obj_state_step(void))

void offset_obj_pos_by_mode(struct TrackObj *obj, u8 kind, s16 dx, s16 dy)
{
    s16 *p;

    switch (kind)
    {
    case 0:
        if ((((struct MapStateScroll *)gUnk_03000DD0)->scroll & 0xC000) != 0x8000)
            return;
        p = &obj->baseA[0];
        *p = dx + *p;
        p += 3;
        *p = dy + *p;
        break;
    case 1:
        if ((((struct MapStateScroll *)gUnk_03000DD0)->scroll & 0x3000) != 0x2000)
            return;
        p = &obj->baseA[1];
        *p = dx + *p;
        p += 3;
        *p = dy + *p;
        break;
    }
}

ASM_FUNC("asm/code_0805799C/obj_slots_update.s", void obj_slots_update(struct TrackObj *obj))

NONMATCH("asm/code_0805799C/obj_slots_motion.s", void obj_slots_motion(struct TrackObj *obj))
{
    u16 i;

    for (i = 0; i < 2; i++)
    {
        u8 *cols;
        u16 entry;
        u16 *dst;
        u16 val;

        cols = (u8 *)gColumnTable;
        entry = *(u16 *)(cols + (((2 - i) << 1) + (obj->selector << 3)));

        if (entry != 0xFFFF)
        {
            u8 *sa;
            u8 *sb;
            s32 jump;
            u32 off;

            jump = obj->flags[i] & 0x80;
            off = i << 1;
            sa = (u8 *)obj->stepA;
            sb = (u8 *)obj->stepB;
            if (jump != 0)
            {
                s32 prodB;
                s32 sum;

                /* the rate bytes follow the phase bytes in the object */
                sum = obj->phase[i] + (obj->phase + 3)[i];
                obj->phase[i] = sum;
                *(s16 *)(sa + off) = gWaveX[obj->flags[i] & 0xF] * ((u8)sum >> 2);
                *(s16 *)(sb + off) = prodB = gWaveY[obj->flags[i] & 0xF] * (obj->phase[i] >> 2);
                if (*(s16 *)(sa + off) != 0 || (prodB << 16) != 0)
                {
                    scroll_map_slot(obj, i, *(s16 *)(sa + off), *(s16 *)(sb + off));
                    obj->phase[i] = 0;
                }
            }
            val = 0;
            *(s16 *)(sa + off) = val;
            dst = (u16 *)(sb + off);
        }
        else
        {
            obj->posA[i] = *(u16 *)&obj->camX;
            obj->posB[i] = *(u16 *)&obj->camY;
            obj->dstA[i] = obj->posA[i];
            dst = &obj->dstB[i];
            val = obj->posB[i];
        }
        *dst = val;
    }
}
END_NONMATCH

void calc_obj_scaled_pos(struct TrackObj *obj)
{
    u16 i;

    for (i = 0; i < 2; i++)
    {
        obj->posA[i] = obj->baseA[i] + ((obj->camX * obj->scaleA[i]) >> 2);
        obj->posB[i] = obj->baseB[i] + ((obj->camY * obj->scaleB[i]) >> 2);
    }
}

ASM_FUNC("asm/code_0805799C/load_map_layout.s", void load_map_layout(void))

ASM_FUNC("asm/code_0805799C/redraw_map_columns.s", void redraw_map_columns(void))

ASM_FUNC("asm/code_0805799C/load_map_tilemap_layers.s", void load_map_tilemap_layers(void))

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

/*
 * Draws metatile `tile` (4 tilemap entries: top-left, top-right, bottom-left, bottom-right) at
 * metatile column col, row row into BG layer `layer` of the map's tilemap buffer. A layer is
 * 64 x 32 tiles (0x800 entries): two 32 x 32 screen blocks side by side, so bit 4 of col picks the
 * right one (+0x400) and the low 4 bits of col and row the place in it.
 */
void draw_metatile_2x2(struct MetatileMapObj *map, u16 layer, s16 col, u16 row, u32 tileArg)
{
    register u32 y asm("r5") = row;
    u16 tile = tileArg;
    u16 (*metatiles)[4] = map->metatiles;
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
    map->bgBuf[top] = metatiles[tile][0];
    map->bgBuf[top + 1] = metatiles[tile][1];
    map->bgBuf[bottom] = metatiles[tile][2];
    map->bgBuf[bottom + 1] = metatiles[tile][3];
}

ASM_FUNC("asm/code_0805799C/scroll_map_slot.s", void scroll_map_slot(struct TrackObj *obj, u32 slot, s16 a, s16 b))
