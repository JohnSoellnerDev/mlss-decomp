#include "global.h"

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

ASM_FUNC("asm/code_080554E4/overlay_update.s", void overlay_update(void))

ASM_FUNC("asm/code_080554E4/textbox_draw_oam.s", void textbox_draw_oam(void))

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

ASM_FUNC("asm/code_080554E4/scene_start_task.s", void scene_start_task(void))

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

ASM_FUNC("asm/code_080554E4/map_obj_draw_obj_setup_sprites.s", void map_obj_draw_obj_setup_sprites(void))

ASM_FUNC("asm/code_080554E4/hblank_scroll_table_dma.s", void hblank_scroll_table_dma(void))

ASM_FUNC("asm/code_080554E4/map_scene_update.s", void map_scene_update(void))
