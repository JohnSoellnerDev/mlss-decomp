#include "global.h"

struct HudObj
{
    u8 filler0[0x0C];
    const void *classPtr;
    u8 filler10[0x14 - 0x10];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u8 filler28[0x30 - 0x28];
    u32 unk30;
    u8 filler34[0x38 - 0x34];
    u32 unk38;
    u8 filler3C[0x44 - 0x3C];
    u32 state44;
    u8 filler48[0x52 - 0x48];
    u16 unk52;
    u8 filler54[0x56 - 0x54];
    u16 unk56;
    u8 filler58[0x6C - 0x58];
    s32 unk6C;
    s32 unk70;
    u8 unk74;
    u8 unk75;
    u8 filler76[0xD8 - 0x76];
    u32 unkD8;
    u8 fillerDC[0xE0 - 0xDC];
    u32 unkE0;
    u8 fillerE4[0xE8 - 0xE4];
    u16 unkE8;
    u16 unkEA;
};

struct OwnerSlots
{
    u8 filler0[0xA8];
    u32 unkA8;
    u32 unkAC;
    u32 unkB0;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;
    u32 unkC0;
    u32 unkC4;
    u32 unkC8;
    u32 unkCC;
    u32 unkD0;
    u32 unkD4;
};

struct StageSwitch
{
    u32 state;
    u8 filler4[0x10 - 0x4];
    void *unk10;
};

struct LinkDC
{
    u8 filler0[0xDC];
    u16 unkDC;
};

struct FnSlotA
{
    u8 filler0[0x10];
    s16 off10;
    u16 filler12;
    void (*fn14)(void *, u32);
};

struct ChildA
{
    u8 filler0[0x18];
    struct FnSlotA *tbl18;
};

struct FnSlotB
{
    u8 filler0[0x8];
    s16 off8;
    u16 filler10;
    void (*fn)(void *, u32);
};

struct ChildB
{
    u8 filler0[0xC];
    struct FnSlotB *tbl;
};

struct KeyRam
{
    u8 filler0[0x2A];
    u16 held;
};

extern struct KeyRam gSys;
extern const u8 gClassEc78[];
extern const u8 gClassSpriteB[];
extern const u8 gClassSpriteA[];
extern const u8 gClassCounter[];
extern const u8 gClassTask[];
extern const u8 gClassScene[];

void obj_destroy_sprites(void *obj, u32 flags);
void obj_release_sprites(void *obj);
void obj_update_sprites(void *obj);
void heap_free_b(void *p);
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void *obj_init_big(void *obj, u32 first);
void obj_restore_counter_tiles(void *obj);
void obj_draw_counter(void *obj);
void obj_list_pass_0a8c(void);
extern void *gUnk_03000D48;
extern void *gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
void write_var(u16 id, u16 value);
void window_destroy(void *win, u32 flags);
void obj_run_callbacks(void);
void set_flags_2_3(u32 v);
void free_if_bit0(void *ptr, u32 flags);
void free_if_bit0_b(void *ptr, u32 flags);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_copy_mode_fields(void *obj);
void obj_update_frame_flags(void *obj);
void obj_set_table_44(void *obj, const void *table);
void obj_set_frame_table(void *obj, const void *table);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_setup_slot(void *obj, u32 a, u32 b, u32 c, s32 offset, ByteArg flag);
extern const u8 gSlidePartCTimerScript[];
extern const u8 gSlidePartBTimerScript[];
extern const u8 gSpriteFrameBaseByMode[];
extern s16 gSineTable[];
extern s16 gCosineTable[];
extern struct ModelCtx *gModelCtx;

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[4];
    u16 unkC;
    u8 filler10[0x34 - 0x10];
    u32 model;
};

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;
};

struct Point1Obj
{
    u8 filler0[8];
    struct ObjRecord *rec;
    u32 *data;
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;
};

struct Point1Local
{
    s32 x;
    s32 y;
    s32 z;
    u32 model;
};

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

ASM_FUNC("asm/code_08171CAC/obj_phase_slide_a.s", void obj_phase_slide_a(void))

ASM_FUNC("asm/code_08171CAC/obj_phase_count_b.s", void obj_phase_count_b(void))

struct ListHead3C
{
    u8 filler0[0x3C];
    u32 head3C;
};

void slot_hud_obj_list_pass_store(void)
{
    struct ListHead3C *list;
    void **hook;
    u8 *base;
    u32 head;

    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHead3C *)gUnk_0203FFB8)->head3C,
        ((struct ListHead3C *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHead3C *)gUnk_0203FFB8;
    hook = &gUnk_03000D4C;
    head = list->head3C;
    base = (u8 *)&gSys;
    *(u32 *)(base + 0x880) = ((u32 (*)(u32, u32))*hook)(head, *(u32 *)(base + 0x880));
}

void slot_hud_obj_state4_if_flag8(struct HudObj *obj)
{
    if (gSys.held & 8)
    {
        obj->state44 = 4;
    }
}

void obj_destroy_task(struct HudObj *obj, u32 flag)
{
    obj->unk18 = (u32)gClassScene;
    write_var(0x1CE2, ((struct LinkDC *)obj->unk30)->unkDC);
    if (obj->unk30 != 0)
    {
        heap_free_b((void *)obj->unk30);
    }
    if (obj->unk38 != 0)
    {
        free_if_bit0((void *)obj->unk38, 3);
    }
    if (obj->unk24 != 0)
    {
        struct FnSlotA *tbl = ((struct ChildA *)obj->unk24)->tbl18;

        tbl->fn14((u8 *)obj->unk24 + tbl->off10, 3);
    }
    if (obj->unk20 != 0)
    {
        window_destroy((void *)obj->unk20, 3);
    }
    if (obj->unk1C != 0)
    {
        struct FnSlotB *tbl = ((struct ChildB *)obj->unk1C)->tbl;

        tbl->fn((u8 *)obj->unk1C + tbl->off8, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(obj->unk14);
    free_if_bit0_b((u8 *)obj + 0x40, 2);
    task_remove(obj, flag);
}

void obj_release_child_sprites(struct HudObj *obj)
{
    obj_release_sprites((void *)obj->unk6C);
    obj_release_sprites((void *)obj->unk70);
}

void obj_update_sprites_6c_70(struct HudObj *obj)
{
    obj_update_sprites((void *)obj->unk6C);
    obj_update_sprites((void *)obj->unk70);
}

void obj_destroy_ec78(struct HudObj *obj, u32 flags)
{
    obj->classPtr = gClassEc78;
    obj_destroy_sprites(obj, flags);
}

NONMATCH("asm/code_08171CAC/slide_part_c_setup.s", void slide_part_c_setup(struct HudObj *obj))
{
    u8 zero;

    obj_copy_mode_fields(obj);
    obj_setup_slot(obj, 0x3000, 0, 0x3C00, (obj->unk6C + 1) << 3, zero = 0);
    obj->unk52 = 0;
    obj->unk56 = 1;
    obj_update_frame_flags(obj);
    obj_set_table_44(obj, gSlidePartCTimerScript);
    obj->unk6C = 0;
}
END_NONMATCH

void obj_destroy_ec88(struct HudObj *obj, u32 flags)
{
    obj->classPtr = gClassSpriteB;
    obj_destroy_sprites(obj, flags);
}

struct HudObj *init_vt_08cdec88(struct HudObj *obj)
{
    obj_init_big(obj, 1);
    obj->classPtr = gClassSpriteB;
    obj_create_sprite_10(obj, 0, 0x1002, 1, 0, 3);
    obj_set_frame_table(obj, gSpriteFrameBaseByMode);
    obj->unk52 = 2;
    obj_update_frame_flags(obj);
    obj->unk18 = 0x3000;
    obj->unk20 = 0x3C00;
    obj->unk6C = 0;
    obj->unk70 = 0;
    *(u16 *)&obj->unk74 = 0;
    return obj;
}

NONMATCH("asm/code_08171CAC/slide_part_b_setup.s", void slide_part_b_setup(struct HudObj *obj))
{
    u8 zero;

    obj_copy_mode_fields(obj);
    obj_setup_slot(obj, 0x4800, 0, 0x2400, (obj->unk6C + 1) << 3, zero = 0);
    obj->unk52 = 6;
    obj->unk56 = 1;
    obj_update_frame_flags(obj);
    obj_set_table_44(obj, gSlidePartBTimerScript);
    obj->unk6C = 0;
}
END_NONMATCH

void obj_destroy_ec98(struct HudObj *obj, u32 flags)
{
    obj->classPtr = gClassSpriteA;
    obj_destroy_sprites(obj, flags);
}

struct HudObj *init_vt_08cdec98(struct HudObj *obj)
{
    obj_init_big(obj, 0);
    obj->classPtr = gClassSpriteA;
    obj_create_sprite_10(obj, 0, 0x1001, 1, 0, 3);
    obj_set_frame_table(obj, gSpriteFrameBaseByMode);
    obj->unk52 = 2;
    obj_update_frame_flags(obj);
    obj->unk18 = 0x4800;
    obj->unk20 = 0x2400;
    obj->unk6C = 0;
    obj->unk70 = 0;
    *(u16 *)&obj->unk74 = 0;
    return obj;
}

void obj_release_slot_97(struct HudObj *obj)
{
    if (obj->unkEA != 0)
    {
        obj->unkEA = 0;
        sound_stop_song(0x97);
    }
}

void obj_claim_slot_97(struct HudObj *obj)
{
    if (obj->unkEA == 0)
    {
        obj->unkEA++;
        set_slot_value(0x97, -1);
    }
}

void obj_clear_bit8_e8(struct HudObj *obj)
{
    obj->unkD8 = 0;
    obj->unkE0 = 0;
    obj->unkE8 &= 0xFEFF;
}

s32 owner_has_children(struct OwnerSlots *owner)
{
    if (owner->unkA8 != 0)
    {
        return 1;
    }
    if (owner->unkAC != 0)
    {
        return 1;
    }
    if (owner->unkB0 != 0)
    {
        return 1;
    }
    if (owner->unkB4 != 0)
    {
        return 1;
    }
    if (owner->unkB8 != 0)
    {
        return 1;
    }
    if (owner->unkBC != 0)
    {
        return 1;
    }
    if (owner->unkC0 != 0)
    {
        return 1;
    }
    if (owner->unkC4 != 0)
    {
        return 1;
    }
    if (owner->unkC8 != 0)
    {
        return 1;
    }
    if (owner->unkCC != 0)
    {
        return 1;
    }
    if (owner->unkD0 != 0)
    {
        return 1;
    }
    if (owner->unkD4 == 0)
    {
        return 0;
    }
    return 1;
}

s32 stage_class_e0(struct HudObj *obj)
{
    s32 v = obj->unkE0;

    if (v <= 7)
    {
        return 0;
    }
    if (v <= 0xF)
    {
        return 1;
    }
    return 2;
}

void stage_switch_2fa0(u32 unused, struct StageSwitch *p, u32 mode)
{
    u32 value;

    if (mode != 0)
    {
        mode--;
        switch (mode)
        {
        case 0:
            value = 4;
            break;
        case 1:
            value = 5;
            break;
        case 2:
            value = 6;
            break;
        case 3:
            value = 7;
            break;
        case 4:
            value = 8;
            break;
        default:
            goto load;
        }
        p->state = value;
    load:
        obj_load_record(p->unk10, mode, 0, 0, 0);
    }
}

void free_if_bit0_c(void *ptr, u32 flags)
{
    if (flags & 1)
    {
        heap_free_b(ptr);
    }
}

void obj_init_counters(struct HudObj *obj)
{
    obj->unk6C = 0xB4;
    obj->unk70 = 0;
}

void obj_frame_timer_step(struct HudObj *obj)
{
    obj->unk70++;
    if (obj->unk70 > 0x3B)
    {
        obj->unk70 = 0;
        obj->unk6C--;
        if (obj->unk6C < 0)
        {
            obj->unk6C = 0;
        }
    }
    if (obj->unk74 != 0)
    {
        obj_draw_counter(obj);
    }
}

void obj_destroy_eca8(struct HudObj *obj, u32 flags)
{
    obj->classPtr = gClassCounter;
    obj_destroy_sprites(obj, flags);
}

struct HudObj *init_vt_08cdeca8(struct HudObj *obj)
{
    obj_init_big(obj, 0xD);
    obj->classPtr = gClassCounter;
    obj_init_counters(obj);
    obj_restore_counter_tiles(obj);
    return obj;
}

void obj_set_ec60_remove(struct HudObj *obj, u32 kind)
{
    obj->unk18 = (u32)gClassTask;
    task_remove(obj, kind);
}

struct HudObj *init_vt_08cdec60(struct HudObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = (u32)gClassTask;
    obj->unk20 = 0;
    obj->unk1C = 0;
    return obj;
}

NONMATCH("asm/code_08171CAC/sprite_list_draw.s", void sprite_list_draw(struct Point1Obj *obj))
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 accX;
    s32 accY;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    words = obj->data;
    accX = *words++;
    accY = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 2; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        x1 = (vx * accX) / 0x100;
        l.x = x1;
        y1 = (l.y * accX) / 0x100;
        l.y = y1;
        z1 = (l.z * accX) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[0] / 0x40);
        y2 = (t - (gSineTable[0] / 0x40) * z1) / 0x100;
        l.y = y2;
        t = z1;
        t = t * (gCosineTable[0] / 0x40);
        l.z = (t + ((gSineTable[0] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x400;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t + pos;
        }
        l.y = obj->posY + l.y / 0x100;
        l.z = obj->posZ + l.z / 0x100;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    accX += accY;
    accY = (accY * 3) / 4;
    words = obj->data;
    *words++ = accX;
    *words = accY;
}
END_NONMATCH

NONMATCH("asm/code_08171CAC/sprite_list_draw_copy.s", void sprite_list_draw_copy(struct Point1Obj *obj))
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 accX;
    s32 accY;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    words = obj->data;
    accX = *words++;
    accY = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 2; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        x1 = (vx * accX) / 0x100;
        l.x = x1;
        y1 = (l.y * accX) / 0x100;
        l.y = y1;
        z1 = (l.z * accX) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[0] / 0x40);
        y2 = (t - (gSineTable[0] / 0x40) * z1) / 0x100;
        l.y = y2;
        t = z1;
        t = t * (gCosineTable[0] / 0x40);
        l.z = (t + ((gSineTable[0] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x400;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t + pos;
        }
        l.y = obj->posY + l.y / 0x100;
        l.z = obj->posZ + l.z / 0x100;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    accX += accY;
    accY = (accY * 3) / 4;
    words = obj->data;
    *words++ = accX;
    *words = accY;
}
END_NONMATCH

NONMATCH("asm/code_08171CAC/obj_zoom_grow2_3494.s", void obj_zoom_grow2_3494(struct Point1Obj *obj))
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 vel;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 2; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (l.y * zoom) / 0x100;
        l.y = y1;
        z1 = (l.z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[0] / 0x40);
        y2 = (t - (gSineTable[0] / 0x40) * z1) / 0x100;
        l.y = y2;
        t = z1;
        t = t * (gCosineTable[0] / 0x40);
        l.z = (t + ((gSineTable[0] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x400;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t + pos;
        }
        l.y = obj->posY + l.y / 0x100;
        l.z = obj->posZ + l.z / 0x100;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += vel;
    vel = (vel * 3) / 4;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH
