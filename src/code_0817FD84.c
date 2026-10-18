#include "global.h"

struct FallRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[4];
    u16 unkC;
    u8 filler10[0x34 - 0x10];
    u32 model;
};

struct FallObj
{
    u8 filler0[8];
    struct FallRecord *rec;
    u32 *data;
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;
};

struct FallLocal
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

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct FallRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

void obj_fall_accel_fd84(struct FallObj *obj)
{
    struct FallLocal l;
    s32 *pt;
    s32 height;
    s32 speed;
    u32 h;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;
    u32 saved;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
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
    height += speed;
    speed += 0x80;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = speed;
}

void obj_rec_x_drift_fe90(struct FallObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    struct FallRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 counter;
    s32 yoff;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    yoff = *pt++;
    counter = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 1; i++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + yoff;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
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
    counter += 8;
    rec->unkC = (obj->flags & 0x80) ? -counter << 8 : counter << 8;
    pt = (s32 *)obj->data;
    *pt++ = yoff;
    *pt = counter;
}

void obj_fall_accel_ffc0(struct FallObj *obj)
{
    struct FallLocal l;
    s32 *pt;
    s32 height;
    s32 speed;
    u32 h;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;
    u32 saved;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
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
    height += speed;
    speed += 0x80;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = speed;
}

void obj_fall_accel_00cc(struct FallObj *obj)
{
    struct FallLocal l;
    s32 *pt;
    s32 height;
    s32 speed;
    u32 h;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;
    u32 saved;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
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
    height += speed;
    speed += 0x80;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = speed;
}

void obj_rec_x_drift_01d8(struct FallObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    struct FallRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 counter;
    s32 yoff;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    yoff = *pt++;
    counter = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 1; i++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + yoff;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
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
    counter += 8;
    rec->unkC = (obj->flags & 0x80) ? -counter << 8 : counter << 8;
    pt = (s32 *)obj->data;
    *pt++ = yoff;
    *pt = counter;
}

ASM_FUNC("asm/code_0817FD84/obj_zoom_spin8_0308.s", void obj_zoom_spin8_0308(void))

ASM_FUNC("asm/code_0817FD84/obj_zoom_spin8_0578.s", void obj_zoom_spin8_0578(void))

ASM_FUNC("asm/code_0817FD84/obj_zoom_spin8_07fc.s", void obj_zoom_spin8_07fc(void))

ASM_FUNC("asm/code_0817FD84/obj_zoom_fall6_0a80.s", void obj_zoom_fall6_0a80(void))

ASM_FUNC("asm/code_0817FD84/obj_zoom_quarter_spin8_0d14.s", void obj_zoom_quarter_spin8_0d14(struct FallObj *obj))

ASM_FUNC("asm/code_0817FD84/obj_zoom_quarter_spin8_0f78.s", void obj_zoom_quarter_spin8_0f78(void))

ASM_FUNC("asm/code_0817FD84/obj_zoom_tilt_rise8_11dc.s", void obj_zoom_tilt_rise8_11dc(struct FallObj *obj))

NONMATCH("asm/code_0817FD84/obj_zoom_quarter10_1440.s", void obj_zoom_quarter10_1440(struct FallObj *obj))
{
    struct FallLocal l;
    struct FallRecord *rec;
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
    for (i = 0; i < 10; i++) {
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
        t = t * (gCosineTable[64] / 0x40);
        y2 = (t - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x1000;

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
    vel = (vel * 7) / 8;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH
