#include "global.h"

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

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;
};

extern struct ModelCtx *gModelCtx;

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

extern s16 gCosineTable[];
extern s16 gSineTable[];

ASM_FUNC("asm/code_0818F50C/obj_zoom_model8_turn_rem6_f50c.s", void obj_zoom_model8_turn_rem6_f50c(struct Point1Obj *obj))
ASM_FUNC("asm/code_0818F50C/obj_zoom_model6_flat_f760.s", void obj_zoom_model6_flat_f760(struct Point1Obj *obj))
// Single-point object with a zoom that sinks by its step; the step is damped to 31/32.
void obj_point1_zoom_step31_f994(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;
    s32 zoom;
    s32 step;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 1; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
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
    zoom -= step;
    step = (step * 31) / 32;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

// Same as obj_point1_zoom_step31_f994, but the step is damped to 15/16.
void obj_point1_zoom_step15_fab4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;
    s32 zoom;
    s32 step;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 1; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
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
    zoom -= step;
    step = (step * 15) / 16;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

ASM_FUNC("asm/code_0818F50C/obj_zoom_model8_turn_rem6_fbd4.s", void obj_zoom_model8_turn_rem6_fbd4(void))

ASM_FUNC("asm/code_0818F50C/obj_zoom_model8_flat_fe28.s", void obj_zoom_model8_flat_fe28(void))

// Same body as obj_point1_zoom_step31_f994 (the step is damped to 31/32).
void obj_point1_zoom_step31_005c(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;
    s32 zoom;
    s32 step;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 1; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
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
    zoom -= step;
    step = (step * 31) / 32;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

ASM_FUNC("asm/code_0818F50C/obj_zoom_model2_turn_rem6_017c.s", void obj_zoom_model2_turn_rem6_017c(void))

ASM_FUNC("asm/code_0818F50C/obj_zoom_model2_turn_rem6_03d0.s", void obj_zoom_model2_turn_rem6_03d0(void))

ASM_FUNC("asm/code_0818F50C/obj_zoom_model2_flat_0624.s", void obj_zoom_model2_flat_0624(void))

ASM_FUNC("asm/code_0818F50C/obj_zoom_model3_turn_rem6_0858.s", void obj_zoom_model3_turn_rem6_0858(void))

ASM_FUNC("asm/code_0818F50C/obj_zoom_model3_turn_rem6_0aac.s", void obj_zoom_model3_turn_rem6_0aac(void))

ASM_FUNC("asm/code_0818F50C/obj_zoom_model4_turn45_0d00.s", void obj_zoom_model4_turn45_0d00(void))
