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

extern s16 gCosineTable[];
extern s16 gSineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

void obj_zoom_model6_tilt45_4b38(struct Point1Obj *obj)
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 step;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 t;
    s32 pos;
    s32 y1;

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
    for (i = 0; i < 6; i++) {
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
        t = z1;
        t = t * (gCosineTable[32] / 0x40);
        z2 = (t - (gSineTable[32] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        l.y = (t - (gSineTable[64] / 0x40) * z2) / 0x100;
        l.z = ((z2 * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2;
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
    zoom += step;
    step = (step * 2) / 3;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

void obj_zoom_model8_tilt45_4d74(struct Point1Obj *obj)
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 step;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 t;
    s32 pos;
    s32 y1;

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
    for (i = 0; i < 8; i++) {
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
        t = z1;
        t = t * (gCosineTable[32] / 0x40);
        z2 = (t - (gSineTable[32] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        l.y = (t - (gSineTable[64] / 0x40) * z2) / 0x100;
        l.z = ((z2 * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2;
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
    zoom += step;
    step = (step * 2) / 3;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

void obj_zoom_model8_tilt45_4fb0(struct Point1Obj *obj)
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 step;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 t;
    s32 pos;
    s32 y1;

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
    for (i = 0; i < 8; i++) {
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
        t = z1;
        t = t * (gCosineTable[32] / 0x40);
        z2 = (t - (gSineTable[32] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        l.y = (t - (gSineTable[64] / 0x40) * z2) / 0x100;
        l.z = ((z2 * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2;
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
    zoom += step;
    step = (step * 2) / 3;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}
