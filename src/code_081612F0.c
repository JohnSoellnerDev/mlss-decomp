#include "global.h"
struct ListHeadX
{
    u8 filler0[0x3C];
    u32 head3C;
};


struct TileObj
{
    u8 tilemap[0x800];
    u8 unk800[4];
    void *tiles804;
    u16 *tiles808;
    void *tiles80C;
    u32 unk810;
    u8 filler814[0x820 - 0x814];
    u16 tileWidth;
};

struct EffectObj
{
    u32 unk00;
    s32 unk04;
    s32 unk08;
    void *unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
};

struct EffectChildTable
{
    u8 filler0[0x10];
    s16 ofs10;
    u16 filler12;
    void *fn14;
    s16 ofs18;
    u16 filler1A;
    void *fn1C;
    u8 filler20[0x8];
    s16 ofs28;
    u16 filler2A;
    void *fn2C;
    s16 ofs30;
    u16 filler32;
    void *fn34;
    s16 ofs38;
    u16 filler3A;
    void *fn3C;
    s16 ofs40;
    u16 filler42;
    void *fn44;
};

struct EffectChild
{
    u8 filler0[0x2C];
    struct EffectChildTable *table;
};

struct EffectWorld
{
    u8 filler0[0x24];
    s32 posX;
    s32 posY;
    struct TileObj *tilesA;
    struct TileObj *tilesB;
    u8 filler34[0x3C - 0x34];
    struct EffectChild *child;
};

struct ObjVtable
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*update)(void *self, u32 mode);
};

struct ChildObj
{
    u8 filler0[0xC];
    struct ObjVtable *vtable;
};

extern u32 gClassEa88[];
extern u32 gClassEa60[];
extern u32 gClassEa78[];
extern u32 gClassE9e8[];
extern const s16 gSineTable[];

struct EffectC
{
    struct SpriteXY6 *sprite;
    s32 angle;
    s32 state;
    s32 timer;
};

struct SpriteXY6
{
    u8 filler0[4];
    s16 x;
    s16 y;
};
extern u32 gEffectStateClass[];
extern u32 gUnknownEffectStateTable[];
extern u32 gFrameUpdateClass[];
extern u32 gHeapTagListB[];
extern u32 gHeapTagOtkdB[];
extern u32 gHeapTagMt7p[];
extern u16 gUnk_02000050[];
extern u32 gClassE980[];
extern u8 gSys[];
extern struct EffectWorld *gUnk_03001024;
void init_bg_gfx();
struct EffectChild *obj_init_ex();
void effect_dma_callback(void);


struct ChildVt4
{
    u8 filler0[4];
    struct ObjVtable *vtable;
};

struct PosPair
{
    s32 x;
    s32 y;
};

struct SpritePos
{
    s16 x;
    s16 y;
};

struct EffectHead
{
    u32 a;
    u32 b;
    u32 c;
};

struct EffectB
{
    u16 timer;
    s16 state;
    u16 ofsX;
    u16 ofsY;
    struct SpritePos *outX;
    struct SpritePos *outY;
    s32 posX;
    s32 posY;
};

void heap_free_a(void *p);
void heap_free_b(void *p);
void task_insert(void *node, u8 kind, void *data);
void release_obj_131c(void *p);
void *heap_alloc2(u32 size, u32 a, void *table, u32 c);
void *obj_list_init(void *p);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void obj_release_slots(void *sprite);
void task_remove(void *node, u32 kind);
s16 view_coord(struct TileObj *obj, s32 pos);
u32 arc_tan2(s32 x, s32 y);
u32 obj_classify_row(struct TileObj *obj, u32 index);
u32 classify_tile_code(void *obj, u32 tile, s32 row);
struct EffectObj *obj_init_class_ea30(struct EffectObj *obj);
void *lookup_or_alloc_13b4(s32 a, s32 b, s32 c, s32 d);
void *find_or_alloc_obj_slots(s32 a, s32 b, s32 c, s32 d, s32 e, void *f, s32 g);

ASM_FUNC("asm/code_081612F0/classify_tile_code.s", u32 classify_tile_code(void *obj, u32 tile, s32 row))

ASM_FUNC("asm/code_081612F0/obj_classify_row.s", u32 obj_classify_row(struct TileObj *obj, u32 index))

s16 view_coord(struct TileObj *obj, s32 pos)
{
    s32 row = pos >> 8;
    u32 entry;

    entry = ((u32 *)obj->tiles80C)[(s16)(((u32)row >> 3) % obj->tileWidth)];
    switch (((s32)((u8 *)&entry)[1] << 28) >> 28)
    {
    case 0:
        return ((s8 *)&entry)[0] * 8;
    case 1:
        {
            s32 t = ((s8 *)&entry)[0] * 8;

            return (s16)(t + row % 8);
        }
    case 2:
        {
            s32 t = ((s8 *)&entry)[0] * 8 + 8;

            return (s16)(t - row % 8);
        }
    case 3:
        {
            s32 t = ((s8 *)&entry)[0] * 8;
            s32 h = row % 8 / 2 - 8;

            return (s16)(t - h);
        }
    case 4:
        {
            s32 t = ((s8 *)&entry)[0] * 8 + 4;

            return (s16)(t - row % 8 / 2);
        }
    case 5:
        {
            s32 t = ((s8 *)&entry)[0] * 8;

            return (s16)(t + row % 8 / 2);
        }
    case 6:
        {
            s32 t = ((s8 *)&entry)[0] * 8;
            s32 h = row % 8 / 2 + 4;

            return (s16)(t + h);
        }
    }
    return ((s8 *)&entry)[0] * 8;
}

ASM_FUNC("asm/code_081612F0/scan_special_run.s", void scan_special_run(void))

s32 tile_step_by_mode(struct TileObj *obj, s32 pos)
{
    u32 entry;
    s32 mode;

    entry = ((u32 *)obj->tiles80C)[(s16)(((u32)(pos >> 8) >> 3) % obj->tileWidth)];
    mode = ((s32)((u8 *)&entry)[1] << 28) >> 28;
    switch (mode)
    {
    case 0:
        return 0;
    case 1:
        return 0x20;
    case 2:
        return -0x20;
    case 3:
    case 4:
        return -0x10;
    case 5:
    case 6:
        return 0x10;
    }
    return 0;
}

u8 view_angle(struct TileObj *obj, s32 x, s32 y)
{
    u16 rowX = view_coord(obj, x);
    s16 rowY = view_coord(obj, y);
    s32 dx = ((y - x) << 8) >> 16;
    s32 dy = rowY - (s16)rowX;

    return ((u32)arc_tan2(dx, (s16)dy) << 16) >> 24;
}

void obj_build_tile_table(struct TileObj *obj)
{
    u32 *table;
    u32 *entry;
    s32 i;

    for (i = 0; i < obj->tileWidth; i++)
    {
        table = (u32 *)obj->tiles80C;
        entry = (u32 *)(i * 4 + (u32)table);
        *entry = obj_classify_row(obj, i);
    }
}

ASM_FUNC("asm/code_081612F0/scan_classify_rows_down.s", void scan_classify_rows_down(void))

ASM_FUNC("asm/code_081612F0/scan_classify_rows_up.s", void scan_classify_rows_up(void))

u32 tile_col_entry(struct TileObj *obj, u32 col)
{
    return ((u32 *)obj->tiles80C)[(s16)((col >> 3) % obj->tileWidth)];
}

struct DmaChannel3
{
    vu32 sad;
    vu32 dad;
    vu32 cnt;
};

u32 dma3_copy_tilemap(struct TileObj *obj)
{
    struct DmaChannel3 *dma = (struct DmaChannel3 *)REG_ADDR_DMA3SAD;

    dma->sad = (u32)obj;
    dma->dad = obj->unk810;
    dma->cnt = 0x84000200;
    return dma->cnt;
}

void obj_free_tile_bufs(struct TileObj *obj)
{
    if (obj->tiles804 != NULL)
    {
        heap_free_a(obj->tiles804);
        obj->tiles804 = NULL;
    }
    if (obj->tiles80C != NULL)
    {
        heap_free_a(obj->tiles80C);
        obj->tiles80C = NULL;
    }
}

void obj_free_tiles(struct TileObj *obj, u32 flags)
{
    obj_free_tile_bufs(obj);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct TileObj *obj_clear_tile_ptrs(struct TileObj *obj)
{
    obj->tiles804 = NULL;
    obj->tiles80C = NULL;
    return obj;
}

extern u32 gClassE980[];

struct ObjVtableB
{
    u8 filler0[0x10];
    s16 thisOffset;
    u16 filler12;
    u32 (*fn)(void *self, void *arg);
};

static inline void obj_task_teardown(struct EffectObj *obj)
{
    struct ChildVt4 *child;

    obj->unk18 = (s32)gClassE980;
    if (obj->unk1C != 0)
    {
        task_remove((void *)obj->unk1C, 3);
    }
    child = (struct ChildVt4 *)obj->unk20;
    if (child != NULL)
    {
        child->vtable->update((u8 *)child + child->vtable->thisOffset, 3);
    }
    task_remove(obj, 3);
}

void obj_task_update(struct EffectObj *obj)
{
    u16 *flags = (u16 *)gSys;
    struct ChildVt4 *child;
    struct ObjVtableB *vb;

    if ((flags[0x15] & 4) != 0)
    {
        if (obj == NULL)
        {
            return;
        }
        obj_task_teardown(obj);
        return;
    }
    child = (struct ChildVt4 *)obj->unk20;
    vb = (struct ObjVtableB *)child->vtable;
    if (vb->fn((u8 *)child + vb->thisOffset, obj) == 0)
    {
        return;
    }
    if (obj == NULL)
    {
        return;
    }
    obj_task_teardown(obj);
}

extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
void update_obj_list(void);
void obj_list_pass_0a8c(void);
void update_objects_frame(void)
{
    u8 *base;
    u32 *oam_slot;
    struct ListHeadX *list;
    u32 old;

    base = (u8 *)&gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    update_obj_list();
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHeadX *)gUnk_0203FFB8)->head3C,
        ((struct ListHeadX *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHeadX *)gUnk_0203FFB8;
    old = ((u32 (*)(u32, u32))gUnk_03000D4C)(list->head3C, old);
    *oam_slot = old;
}

void effect_release_cls980(struct EffectObj *obj, u32 kind)
{
    struct ChildVt4 *child;

    obj->unk18 = (s32)gClassE980;
    if (obj->unk1C != 0)
    {
        task_remove((void *)obj->unk1C, 3);
    }
    child = (struct ChildVt4 *)obj->unk20;
    if (child != NULL)
    {
        child->vtable->update((u8 *)child + child->vtable->thisOffset, 3);
    }
    task_remove(obj, kind);
}

NONMATCH("asm/code_081612F0/effect_ctor_cls980.s", struct EffectObj *effect_ctor_cls980(struct EffectObj *obj, u8 kind, void *data))
{
    struct EffectObj *child;
    vu16 *p;

    task_insert(obj, kind, data);
    obj->unk18 = (s32)gClassE980;
    child = heap_alloc2(0x1c, 1, gHeapTagOtkdB, 0);
    task_insert(child, 8, gHeapTagOtkdB);
    child->unk18 = (s32)gFrameUpdateClass;
    obj->unk1C = (s32)child;
    child->unk14 = (s32)obj;
    obj->unk20 = (s32)obj_init_class_ea30(heap_alloc2(8, 1, gHeapTagMt7p, 0));
    obj->unk24 = 0;
    obj->unk28 = 0;
    obj->unk40 = 0;
    obj->unk44 = 0;
    p = (vu16 *)gUnk_02000050;
    *p = 0xdf;
    p += 2;
    *p = 0x1f;
    return obj;
}
END_NONMATCH

ASM_FUNC("asm/code_081612F0/init_bg_gfx.s", void init_bg_gfx())

u32 pos_in_view_box(struct EffectObj *obj, struct EffectObj *other)
{
    if (other->unk14 < obj->unk24 + -0x2000)
    {
        return 0;
    }
    if (other->unk14 > obj->unk24 + 0x11000)
    {
        return 0;
    }
    if (other->unk18 < obj->unk28)
    {
        return 0;
    }
    if (other->unk18 > obj->unk28 + 0xA000)
    {
        return 0;
    }
    return 1;
}

void *obj_add_task(struct EffectObj *obj, u8 kind, void *data)
{
    task_insert(obj, kind, data);
    obj->unk18 = (s32)gFrameUpdateClass;
    return obj;
}

void obj_step_clamped(struct EffectObj *obj)
{
    s32 limit;

    obj->unk14 += obj->unk24;
    obj->unk18 += obj->unk28 + obj->unk10;
    limit = view_coord((struct TileObj *)obj->unk30, obj->unk14) << 8;
    if (obj->unk18 > limit)
    {
        obj->unk18 = limit;
    }
}

void stub_nop_08161ff8(void)
{
}

ASM_FUNC("asm/code_081612F0/obj_init_ex.s", struct EffectChild *obj_init_ex())

void obj_effect_child_method(struct EffectObj *obj)
{
    obj->unk24 = obj->unk14 - obj->unk1C;
    obj->unk28 = obj->unk18 - obj->unk20;
    obj->unk1C = obj->unk14;
    obj->unk20 = obj->unk18;
}

void obj_advance_pos(struct EffectObj *obj)
{
    obj->unk14 += obj->unk24;
    obj->unk18 += obj->unk28 + obj->unk10;
}

void obj_set_pos_pair(struct EffectObj *obj, u32 *src)
{
    obj->unk14 = obj->unk1C = src[0];
    obj->unk18 = obj->unk20 = src[1];
}

void obj_sprite_pos_rel_cam(struct EffectObj *obj, struct PosPair *cam)
{
    ((struct SpritePos *)obj->unk0C)->x = (obj->unk14 - cam->x) >> 8;
    ((struct SpritePos *)obj->unk0C)->y = (obj->unk18 - cam->y) >> 8;
}

void sprites_obj_refresh_sprite(struct EffectObj *obj, u32 flag)
{
    u8 *spr = obj->unk0C;

    if (!(spr[0x29] & 0x40))
    {
        obj_load_record(spr, *(s8 *)&obj->unk08, -1, 0, 0);
    }
    if (flag != 0)
    {
        sprite_update_cd0(obj->unk0C);
    }
    else
    {
        obj_release_slots(obj->unk0C);
    }
}

void obj_release_child_131c(struct EffectObj *obj)
{
    release_obj_131c(obj->unk0C);
    obj->unk0C = NULL;
}

void obj_create_child(struct EffectObj *obj, u32 arg)
{
    struct SpritePos *child;

    child = create_obj_de4(0, *(s16 *)((u8 *)obj + 6), arg, -1, -1, -1, -1);
    obj->unk0C = child;
    child->x = obj->unk14 >> 8;
    ((struct SpritePos *)obj->unk0C)->y = obj->unk18 >> 8;
}

s32 stub_return_zero_08162174(void)
{
    return 0;
}

void obj_release_both(struct EffectObj *obj, u32 flags)
{
    obj->unk2C = (s32)gClassE9e8;
    if (obj->unk0C != NULL)
    {
        obj_release_child_131c(obj);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct EffectObj *effect_sub_init_cls9e8(struct EffectObj *obj, struct EffectHead *src, s32 arg)
{
    s32 tmp;
    s32 x;
    s32 hi;
    u16 lo;

    obj->unk2C = (s32)gClassE9e8;
    obj->unk10 = arg;
    *(struct EffectHead *)obj = *src;
    obj->unk0C = 0;
    obj->unk14 = obj->unk1C = ((s16 *)obj)[0] << 8;
    obj->unk18 = obj->unk20 = ((s16 *)obj)[1] << 8;
    obj->unk24 = 0;
    obj->unk28 = 0;
    lookup_or_alloc_13b4(0, ((s16 *)obj)[3], -1, 1);
    x = ((s16 *)obj)[3];
    hi = (x >> 12) & 0xf;
    lo = x & 0xfff;
    find_or_alloc_obj_slots(0, hi << 12 | lo, 0, 0, 0, &tmp, -1);
    return obj;
}

ASM_FUNC("asm/code_081612F0/effect_step.s", void effect_step(void))

void obj_set_class_ea60(struct EffectObj *obj, u32 flags)
{
    obj->unk04 = (u32)gClassEa60;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct EffectObj *obj_init_class_ea30(struct EffectObj *obj)
{
    obj->unk00 = 0;
    obj->unk04 = (u32)gEffectStateClass;
    return obj;
}

void stub_nop_081623c4(void)
{
}

void obj_set_class_ea60_b(struct EffectObj *obj, u32 flags)
{
    obj->unk04 = (u32)gClassEa60;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_init_class_ea48(struct EffectObj *obj)
{
    obj->unk00 = 0;
    obj->unk04 = (u32)gUnknownEffectStateTable;
}

s32 stub_return_zero_081623f8(void)
{
    return 0;
}

void obj_set_class_ea60_c(struct EffectObj *obj, u32 flags)
{
    obj->unk04 = (u32)gClassEa60;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_init_class_ea60(struct EffectObj *obj)
{
    obj->unk04 = (u32)gClassEa60;
    obj->unk00 = 0;
}

void effect_dma_callback(void)
{
    dma3_copy_tilemap(gUnk_03001024->tilesA);
    dma3_copy_tilemap(gUnk_03001024->tilesB);
}

void obj_set_class_ea78(struct EffectObj *obj, u32 flags)
{
    struct ChildObj *child;

    obj->unk04 = (u32)gClassEa78;
    child = (struct ChildObj *)obj->unk00;
    if (child != NULL)
    {
        child->vtable->update((u8 *)child + child->vtable->thisOffset, 3);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void *init_cls_ea78(struct EffectObj *obj)
{
    obj->unk04 = (u32)gClassEa78;
    obj->unk00 = (u32)obj_list_init(heap_alloc2(0x10, 1, gHeapTagListB, 0));
    return obj;
}

void stub_nop_081624b0(void)
{
}

void obj_set_class_ea88(struct EffectObj *obj, u32 flags)
{
    obj->unk00 = (u32)gClassEa88;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_set_class_ea88_keep(struct EffectObj *obj)
{
    obj->unk00 = (u32)gClassEa88;
}

ASM_FUNC("asm/code_081612F0/effect_init_sprites.s", void effect_init_sprites(void))

NONMATCH("asm/code_081612F0/effect_step_a.s", s32 effect_step_a(struct EffectObj *obj))
{
    s32 delta;
    s32 pos;

    switch (obj->unk30)
    {
    case 0:
        delta = obj->unk10 + 0x14;
        obj->unk10 = delta;
        pos = obj->unk08 + delta;
        obj->unk08 = pos;
        if (pos > 0x4FFF)
        {
            obj->unk10 = (-delta * 2) / 3;
            if ((obj->unk10 >> 8) == 0)
                goto finish;
        }
        ((struct SpritePos *)obj->unk00)->x = (obj->unk04 + obj->unk14) >> 8;
        ((struct SpritePos *)obj->unk00)->y = (obj->unk08 + obj->unk18) >> 8;
        break;
    case 1:
        if (--obj->unk2C <= 0)
            return 1;
        break;
    }
    return 0;

finish:
    obj->unk2C = 0x50;
    obj->unk30 = 1;
    return 0;
}
END_NONMATCH

void obj_release_child_free(struct EffectObj *obj, u32 flags)
{
    release_obj_131c((void *)obj->unk00);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

s32 effect_step_b(struct EffectB *obj)
{
    switch (obj->state)
    {
    case 0:
        break;
    case 1:
        obj->timer--;
        if ((s16)obj->timer < 0)
        {
            obj->timer = 0x50;
            obj->state++;
        }
        obj->posX += 1000;
        obj->posY += -1000;
        obj->outX->x = (obj->posX >> 8) + obj->ofsX;
        obj->outY->x = (obj->posY >> 8) + obj->ofsY;
        break;
    case 2:
        obj->timer--;
        if ((s16)obj->timer > 0)
            return 0;
        return 1;
    }
    return 0;
}

ASM_FUNC("asm/code_081612F0/effect_init_sprite_pair.s", void effect_init_sprite_pair(void))

void obj_release_two_children(struct EffectObj *obj, u32 flags)
{
    if (obj->unk08 != 0)
    {
        release_obj_131c((void *)obj->unk08);
    }
    if (obj->unk0C != NULL)
    {
        release_obj_131c(obj->unk0C);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

static inline void clear_axis(s32 *p)
{
    p[0] = 0;
    p[2] = 0;
}

ASM_FUNC("asm/code_081612F0/obj_clear_bounds.s", void obj_clear_bounds(void))

struct EffectObj *effect_create_sprites(struct EffectObj *obj)
{
    void *sprite;
    s32 y;

    sprite = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
    obj->unk00 = (u32)sprite;
    switch (gSys[0x8C0])
    {
    case 0:
        obj_load_record((void *)obj->unk00, 0x22, -1, 0, 0);
        break;
    case 1:
        obj_load_record((void *)obj->unk00, 0x23, -1, 0, 0);
        break;
    case 2:
        obj_load_record((void *)obj->unk00, 0x24, -1, 0, 0);
        break;
    case 3:
        obj_load_record((void *)obj->unk00, 0x25, -1, 0, 0);
        break;
    case 4:
        obj_load_record((void *)obj->unk00, 0x26, -1, 0, 0);
        break;
    default:
        obj_load_record(sprite, 0x21, -1, 0, 0);
        break;
    }
    sprite_update_cd0((void *)obj->unk00);
    ((struct SpritePos *)obj->unk00)->x = 0x78;
    ((struct SpritePos *)obj->unk00)->y = y = 0x50;
    obj->unk0C = NULL;
    obj->unk04 = y + 0xB0;
    obj->unk08 = 0;
    return obj;
}

static inline void effect_c_wave(struct EffectC *obj, s32 phase)
{
    obj->sprite->x = (gSineTable[phase & 0xFF] / 64) * 2;
    obj->sprite->y = (gSineTable[phase & 0xFF] / 64) * 2;
}

u32 effect_step_c(struct EffectC *obj)
{
    s32 phase = obj->angle >> 8;

    switch (obj->state)
    {
    case 0:
        effect_c_wave(obj, phase);
        obj->angle += 0x200;
        if (phase > 0x40)
        {
            obj->state++;
        }
        break;
    case 1:
        effect_c_wave(obj, phase);
        obj->angle += 0x400;
        if (phase > 0x60)
        {
            obj->timer = 0x50;
            obj->state++;
        }
        break;
    case 2:
        obj->timer -= 1;
        if (obj->timer > 0)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

void obj_release_child_free_b(struct EffectObj *obj, u32 flags)
{
    release_obj_131c((void *)obj->unk00);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_081612F0/effect_init_sprites_b.s", void effect_init_sprites_b(void))
