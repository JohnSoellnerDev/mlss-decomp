#include "global.h"
#include "ram_routines.h"

struct OverlayObj
{
    u8 filler0[0x18];
    void *vtable;
    u8 filler1C[4];
    void *heap20;
    u8 filler24[4];
    void *heap28;
    void *heap2C;
    void *task30;
};

void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void heap_free_a(void *p);
void heap_free_b(void *p);
void run_pending_obj_callbacks(void);
extern const u8 gOverlayVtable[];
extern const u8 gChildVtable[];
extern u16 gUnk_02000008;
extern u16 gUnk_02000010[];
extern u16 gUnk_02000018;
extern void *gUnk_03000DA0;
extern const u8 gScrollGroupEnd[];

struct SpriteObj
{
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u8 filler8[6];
    u16 unkE;
    u8 filler10[2];
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 flags1F;
};

struct SceneObj
{
    u8 flags0;
    u8 filler1[0x18 - 1];
    void *vtable;
    u8 filler1C;
    u8 flags1D;
    u8 filler1E[2];
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    void *child3C;
    struct SpriteObj *sprite40;
    struct SpriteObj *sprite44;
    struct SpriteObj *sprite48;
    u8 filler4C[0x80 - 0x4C];
    u8 flags80;
    u8 filler81[0xD4 - 0x81];
    struct SpriteObj *unkD4;
    void *unkD8;
    u8 fillerDC[4];
    struct SpriteObj *unkE0;
};

struct Lo4Bits
{
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 rest : 4;
};

struct Bit5Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

void obj_release_slots(void *slots);
void obj_load_record(struct SpriteObj *rec, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(struct SpriteObj *sprite);

struct TextBoxObj
{
    u8 filler0[0x14];
    u8 *owner14;
};

struct ObjHead
{
    u8 filler0[0x3C];
    u8 *unk3C;
};

extern u8 gSys[];
extern u8 *gParty;
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
void *draw_two_values(void *oam, u8 a, u8 b, u32 flag);
void *text_sprites_b(void *oam, u32 a, u32 b, u32 c);
void *draw_member_status(void *oam, u32 a, u32 b);
void obj_list_pass_0a8c(void);
void *heap_alloc(u32 size, u32 mode, const void *tag, u32 flags);
void *heap_alloc2(u32 size, u32 pool, const void *tag, u32 clear);
void frame_update(void);
u8 *ptr_03000ffc_plus_1ff8(void);
u32 read_varlen_plus1(const void *p);
void cpu_fast_set(const void *src, void *dst, u32 control);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void *set_obj_digits(void *obj, u32 kind, const void *tag, u32 value, u32 small);
void init_obj_09a8(u32 a, u32 b, u32 c, u32 d);
void set_irq_callback(u32 kind, void (*cb)(void));
void set_slot_target(u32 idx, u32 value, u32 mode);
void hblank_scroll_table_dma(void);
void map_obj_draw_obj_setup_sprites(struct SceneObj *obj, u32 count);
extern u8 gMapSceneClass[];
extern u8 gMapScrollTag[];
extern u8 gDigitsObjTag[];
extern u8 gGfxWorkBlockTag[];
extern u8 gTitleSkyCopyright[];
extern u8 gSharedBg083a61ec[];
extern u8 gMapStartBg0Map[];
extern u8 gMapStartBg1Map[];
extern u8 gMapStartBg2Map[];
extern u8 gMapStartFirstObjectByLang[];
extern u8 gMapStartObjectTable[];
extern u8 gMapPaletteA[];
extern u8 gMapPaletteB[];
extern u32 gUnk_03000C78;
extern u32 gUnk_03000FFC;
extern void (*gUnk_03000C84)(void *src, void *dst);
extern u16 gUnk_02000080;
extern u16 gUnk_02000050[];
extern u8 gUnk_0203FFF8[];

ASM_FUNC("asm/code_080554E4/overlay_update.s", void overlay_update(void))

ASM_FUNC("asm/code_080554E4/textbox_draw_oam.s", void textbox_draw_oam(struct TextBoxObj *obj))

void overlay_free(struct OverlayObj *obj, u32 flags)
{
    obj->vtable = (void *)gOverlayVtable;
    if (obj->task30 != 0)
    {
        task_remove(obj->task30, 3);
    }
    if (obj->heap20 != 0)
    {
        heap_free_a(obj->heap20);
    }
    if (obj->heap28 != 0)
    {
        heap_free_a(obj->heap28);
    }
    heap_free_b(obj->heap2C);
    run_pending_obj_callbacks();
    task_remove(obj, flags);
}

struct OverlayObj *overlay_child_init(struct OverlayObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->vtable = (void *)gChildVtable;
    return obj;
}

ASM_FUNC("asm/code_080554E4/scene_create.s", void scene_create(void))

ASM_FUNC("asm/code_080554E4/scene_step.s", void scene_step(void))

NONMATCH("asm/code_080554E4/scene_start_task.s", void scene_start_task(struct SceneObj *obj, u8 prio, u32 tag, u32 mode))
{
    u8 *sys;
    u8 *cfg;
    u8 *save;
    u8 b;
    u32 sub;
    u8 *palette;
    void **digitsSlot;
    void **slot0;
    void **slot;
    void *block;
    void *vram;
    void *digits;
    void *digitsObj;
    void *firstObj;
    void *objA;
    void *objB;
    u16 *rec;
    u16 *bg;
    u32 len;
    u32 code;
    u32 flagsHw;
    u32 ctl;
    u32 bits;
    u32 bv;
    u32 fl;
    s32 m1;
    u32 pair;

    ((void (*)(void *, u32, u32))task_insert)(obj, prio, tag);
    obj->vtable = gMapSceneClass;
    gUnk_02000080 = 0x7FFF;
    sys = gSys;
    *(u16 *)(sys + 2) = 1;
    *(u16 *)0x02000000 = 0;
    frame_update();
    sys[0x31] = 8;
    save = ptr_03000ffc_plus_1ff8();
    *(u32 *)gUnk_0203FFF8 = *(u32 *)save;
    gUnk_0203FFF8[4] = save[4];
    cfg = (u8 *)gUnk_03000FFC;
    bv = gUnk_0203FFF8[7] & -4;
    gUnk_0203FFF8[7] = bv;
    bv = (bv & -5) | ((((u32)cfg[8] << 28) >> 31) << 2);
    gUnk_0203FFF8[7] = bv;
    bv = (bv & -9) | ((((u32)cfg[8] << 26) >> 31) << 3);
    bv = (bv & -0x71) | (((u32)cfg[8] << 29) >> 25);
    bv = (bv & 0x7F) | ((((u32)sys[0x888] << 29) >> 30) << 7);
    gUnk_0203FFF8[7] = bv;
    gUnk_03000DA0 = heap_alloc2(0x154, 0, gMapScrollTag, 1);
    digitsSlot = (void **)((u8 *)obj + 0xB8);
    digits = heap_alloc2(0x24, 0, gDigitsObjTag, 0);
    digitsObj = set_obj_digits(digits, 8, gDigitsObjTag, *(u32 *)gUnk_0203FFF8, gUnk_0203FFF8[4]);
    *digitsSlot = digitsObj;
    ((struct TextBoxObj *)digitsObj)->owner14 = (u8 *)obj;
    if (mode != 0)
        sub = mode - 1;
    else
        sub = 0;
    *((u8 *)obj + 0xBC) = sub;
    obj->filler1C = 0x10;
    fl = obj->flags1D & -4 & -0x11;
    obj->flags1D = fl | 0x20;
    obj->flags80 = 0;
    slot0 = (void **)((u8 *)obj + 0xC0);
    obj->filler81[0] = 0xFF;
    obj->filler81[1] = 0xFF;
    obj->filler81[2] = 0xFF;
    obj->filler81[3] = 0xFF;
    obj->filler81[4] = 0xFF;
    gUnk_03000C78 = 0;
    block = heap_alloc(0x8000, 1, gGfxWorkBlockTag, 0);
    (*gUnk_03000C84)(gTitleSkyCopyright, block);
    vram = (void *)0x06000000;
    len = read_varlen_plus1(gTitleSkyCopyright);
    cpu_fast_set(block, vram, (len >> 2) & 0x1FFFFF);
    (*gUnk_03000C84)(gSharedBg083a61ec, block);
    len = read_varlen_plus1(gSharedBg083a61ec);
    cpu_fast_set(block, (void *)0x06004000, (len >> 2) & 0x1FFFFF);
    cpu_fast_set(gMapStartBg0Map, (void *)0x0600C000, 0x140);
    cpu_fast_set(gMapStartBg1Map, (void *)0x0600D000, 0x400);
    cpu_fast_set(gMapStartBg2Map, (void *)0x0600C800, 0x138);
    heap_free_b(block);
    init_obj_09a8(0, 0, 0, 0);
    code = ((u16 *)gMapStartFirstObjectByLang)[sys[0x8C0]];
    flagsHw = ((u16 *)gMapStartObjectTable)[1];
    m1 = -1;
    firstObj = create_obj_de4(0, code, flagsHw, m1, m1, m1, m1);
    *slot0 = firstObj;
    obj_load_record(firstObj, 0, m1, 0, 0);
    pair = 1;
    rec = (u16 *)gMapStartObjectTable + 2;
    slot = (void **)((u8 *)obj + 0xC4);
    do
    {
        objA = create_obj_de4(0, rec[0], rec[1], m1, m1, m1, m1);
        slot[0] = objA;
        obj_load_record(objA, 0, m1, 0, 0);
        objB = create_obj_de4(0, rec[2], rec[3], m1, m1, m1, m1);
        slot[1] = objB;
        obj_load_record(objB, 0, m1, 0, 0);
        slot += 2;
        rec += 4;
        pair += 2;
    } while (pair <= 8);
    if (mode == 0)
    {
        obj->unk20 = 0x7800;
        obj->unk24 = mode;
        *(u16 *)((u8 *)obj + 0x2A) = 0x200;
        obj_load_record(obj->unkD4, 0, 0, 0, 0);
        obj->unkD4->unk0 = 0x78;
        obj->unkD4->unk2 = (u16)mode;
        ((struct Lo4Bits *)&obj->unkD4->flags1F)->lo2 = 0;
        ((struct Lo4Bits *)&obj->unkD4->flags1F)->b23 = 1;
        obj->unkD4->unkE = 2;
        sprite_update_cd0(obj->unkD4);
    }
    else
    {
        map_obj_draw_obj_setup_sprites(obj, mode);
    }
    gUnk_03000C78 = 0x0800063C;
    bg = gUnk_02000050;
    *bg = 0xBF;
    bg += 2;
    *bg = 0x10;
    bg -= 38;
    *bg = 0x1802;
    bg++;
    *bg = 0x5A03;
    bg++;
    *bg = 0x5984;
    bg = gUnk_02000010;
    *bg = 0;
    bg++;
    *bg = 0;
    bg++;
    *bg = 0;
    bg++;
    *bg = 0;
    palette = gMapPaletteA;
    if ((sys[0x888] & 6) == 0)
        palette = gMapPaletteB;
    cpu_fast_set(palette, &gUnk_02000080, 0x70);
    gUnk_02000080 = 0x7FFF;
    *(u16 *)(sys + 2) = 0xFFFF;
    *(u16 *)sys = 0xFFFF;
    set_irq_callback(2, hblank_scroll_table_dma);
    set_slot_target(0, 0x29, -1);
    if (mode == 0)
    {
        obj->flags1D = obj->flags1D & -13;
        ctl = *(u16 *)0x02000000;
        bits = 0x1241;
    }
    else
    {
        obj->flags1D = (obj->flags1D & -13) | 8;
        ctl = *(u16 *)0x02000000;
        bits = 0x1741;
    }
    *(u16 *)0x02000000 = ctl | bits;
}
END_NONMATCH

NONMATCH("asm/code_080554E4/scene_reset_sprites.s", void scene_reset_sprites(struct SceneObj *obj))
{
    u16 *bg;

    obj_release_slots(obj->unkD8);
    obj_load_record(obj->unkE0, 0, 0, 0, 0);
    obj->unkE0->unk0 = 0x79;
    obj->unkE0->unk2 = 0x50;
    obj->unkE0->unk4 = 0x100;
    obj->unkE0->unk6 = 0x100;
    ((struct Lo4Bits *)&obj->unkE0->flags1F)->lo2 = 0;
    ((struct Lo4Bits *)&obj->unkE0->flags1F)->b23 = 0;
    obj->unkE0->unkE = 2;
    sprite_update_cd0(obj->unkE0);
    obj_load_record(obj->unkD4, 2, 0, 0, 0);
    obj->unkD4->unk0 = 0x78;
    obj->unkD4->unk2 = 0x8C;
    ((struct Lo4Bits *)&obj->unkD4->flags1F)->lo2 = 0;
    ((struct Lo4Bits *)&obj->unkD4->flags1F)->b23 = 1;
    obj->unkD4->unkE = 2;
    {
        u8 m = obj->unkD4->unk12;
        obj->unkD4->unk12 = m | 0x10;
    }
    sprite_update_cd0(obj->unkD4);
    obj->unk20 = 0x7800;
    obj->unk24 = 0x8C00;
    ((struct Bit5Bits *)&obj->flags1D)->bit5 = 0;
    bg = (u16 *)&gUnk_02000018;
    *bg = 0;
    bg++;
    *bg = 0;
    bg += 3;
    *bg = 0x100;
    bg++;
    *bg = 0;
    bg++;
    *bg = 0;
    bg++;
    *bg = 0x100;
    bg++;
    *(u32 *)bg = 0;
    bg = (u16 *)((u8 *)bg + 4);
    *(u32 *)bg = 0;
    gUnk_02000008 = 0x1802;
    bg = gUnk_02000010;
    *bg = 0;
    bg++;
    *bg = 0;
    *(u16 *)0x02000000 |= 0x100;
    obj->unk30 = 0;
    obj->unk34 = 0x100;
    obj->unk38 = 0x100;
    obj->flags80 = 0xFF;
}
END_NONMATCH

ASM_FUNC("asm/code_080554E4/map_obj_draw_obj_setup_sprites.s", void map_obj_draw_obj_setup_sprites(struct SceneObj *obj, u32 count))

NONMATCH("asm/code_080554E4/hblank_scroll_table_dma.s", void hblank_scroll_table_dma(void))
{
    s32 *st;
    u16 *dst;
    s32 group;
    u8 filled;
    s32 v;
    u32 value;
    u8 end;
    vu16 *bg1;
    vu32 *dma;

    st = (s32 *)gUnk_03000DA0;
    st[0] -= 0x40;
    st[1] -= 0x80;
    st[2] -= 0x40;
    st[3] -= 0x20;
    filled = 0;
    dst = (u16 *)((u8 *)st + 0x14);
    group = 0;
    do
    {
        v = *(s32 *)((u8 *)gUnk_03000DA0 + (group << 2));
        if (v < 0)
            v += 0xFF;
        value = ((u32)v << 8) >> 16;
        end = gScrollGroupEnd[group];
        group++;
        if (filled <= end)
        {
            do
            {
                *dst = value;
                filled++;
                dst++;
            } while (filled <= end);
        }
    } while (group <= 4);
    bg1 = (vu16 *)0x04000014;
    *bg1 = *(u16 *)(((u8 *)gUnk_03000DA0) + 0x14);
    dma = (vu32 *)0x040000B0;
    *(vu16 *)0x040000BA = 0;
    dma[0] = (u32)(((u8 *)gUnk_03000DA0) + 0x16);
    dma[1] = (u32)bg1;
    dma[2] = 0xA2400001;
}
END_NONMATCH

ASM_FUNC("asm/code_080554E4/map_scene_update.s", void map_scene_update(void))
