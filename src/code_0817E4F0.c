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

extern s16 gSineTable[];
extern s16 gCosineTable[];

void obj_accel_tilt9_e4f0(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 height;
    s32 height_speed;
    s32 offset_y;
    s32 offset_y_speed;
    s32 x_speed;
    struct ObjRecord *rec;
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
    height = *words++;
    height_speed = *words++;
    offset_y = *words++;
    offset_y_speed = *words++;
    x_speed = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 9; i++) {
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
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + offset_y;
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
    zoom += 0x400;
    offset_y += offset_y_speed;
    offset_y_speed += 0x60;
    height += height_speed;
    height_speed += 1;
    rec->unk4 = 0x140 - height;
    rec->unk6 = 0x140 - height;
    x_speed += 6;
    rec->unkC = (obj->flags & 0x80) ? (-x_speed << 8) : (x_speed << 8);
    words = obj->data;
    *words++ = zoom;
    *words++ = height;
    *words++ = height_speed;
    *words++ = offset_y;
    *words++ = offset_y_speed;
    *words = x_speed;
}

NONMATCH("asm/code_0817E4F0/obj_angle_zoom4_e71c.s", void obj_angle_zoom4_e71c(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 angle;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 t;
    s32 pos;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 idx;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 4; i++) {
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
        idx = angle & 0xFF;
        z2 = (z1 * (gCosineTable[idx] / 0x40) - (gSineTable[idx] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[idx] / 0x40) + (gSineTable[idx] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

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
    zoom += vel;
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}
END_NONMATCH


NONMATCH("asm/code_0817E4F0/obj_angle_zoom4_e998.s", void obj_angle_zoom4_e998(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 angle;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 t;
    s32 pos;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 idx;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 4; i++) {
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
        idx = angle & 0xFF;
        z2 = (z1 * (gCosineTable[idx] / 0x40) - (gSineTable[idx] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[idx] / 0x40) + (gSineTable[idx] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

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
    zoom += vel;
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}
END_NONMATCH

ASM_FUNC("asm/code_0817E4F0/obj_angle_zoom8_ec14.s", void obj_angle_zoom8_ec14(void))

NONMATCH("asm/code_0817E4F0/obj_turn_zoom12_ee84.s", void obj_turn_zoom12_ee84(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 step;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x0;
    s32 y0;
    s32 z0;
    s32 z1;
    s32 x1;
    s32 pos;
    s32 t;

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
    left = 11;
    for (;;) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x0 = vx * zoom / 0x100;
        l.x = x0;
        y0 = vy * zoom / 0x100;
        l.y = y0;
        z0 = vz * zoom / 0x100;
        l.z = z0;
        z1 = (z0 * (gCosineTable[16] / 0x40) - x0 * (gSineTable[16] / 0x40)) / 0x100;
        l.z = z1;
        x1 = (x0 * (gCosineTable[16] / 0x40) + z0 * (gSineTable[16] / 0x40)) / 0x100;
        l.x = x1;
        l.y = (y0 * (gCosineTable[12] / 0x40) - z1 * (gSineTable[12] / 0x40)) / 0x100;
        l.z = (z1 * (gCosineTable[12] / 0x40) + y0 * (gSineTable[12] / 0x40)) / 0x100;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x1;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x1;
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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += step;
    step = (step * 9) / 10;
    rec->unk4 = 0x140;
    rec->unk6 = 0x140;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}
END_NONMATCH

void obj_rotx_zoom12_f0c0(struct Point1Obj *obj)
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
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

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
    for (i = 0; i < 12; i++) {
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
        t = t * (gCosineTable[16] / 0x40);
        y2 = (t - (gSineTable[16] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[16] / 0x40)) + ((gSineTable[16] / 0x40) * y1)) / 0x100;

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
    zoom += step;
    step = (step * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

void obj_drift_point_f2a4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 offset_x;
    s32 offset_y;
    s32 speed_x;
    s32 speed_y;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x_in;
    s32 y_in;
    s32 pos;
    s32 pos_z;
    s32 t;

    words = obj->data;
    offset_x = *words++;
    offset_y = *words++;
    speed_x = *words++;
    speed_y = *words++;
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
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x_in = vx + offset_x;
        l.x = x_in;
        y_in = vy + offset_y;
        l.y = y_in;
        pos = obj->posX;
        t = x_in;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.x = pos + t;
        pos = obj->posY;
        t = y_in;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos_z = obj->posZ;
        t = vz;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos_z + t;
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
    offset_x += speed_x;
    offset_y += speed_y;
    rec->unk4 = 0xEC;
    rec->unk6 = 0xEC;
    words = obj->data;
    *words++ = offset_x;
    *words++ = offset_y;
    *words++ = speed_x;
    *words = speed_y;
}

void obj_drift_point_f3c4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 offset_x;
    s32 offset_y;
    s32 speed_x;
    s32 speed_y;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x_in;
    s32 y_in;
    s32 pos;
    s32 pos_z;
    s32 t;

    words = obj->data;
    offset_x = *words++;
    offset_y = *words++;
    speed_x = *words++;
    speed_y = *words++;
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
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x_in = vx + offset_x;
        l.x = x_in;
        y_in = vy + offset_y;
        l.y = y_in;
        pos = obj->posX;
        t = x_in;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.x = pos + t;
        pos = obj->posY;
        t = y_in;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos_z = obj->posZ;
        t = vz;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos_z + t;
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
    offset_x += speed_x;
    offset_y += speed_y;
    rec->unk4 = 0xEC;
    rec->unk6 = 0xEC;
    words = obj->data;
    *words++ = offset_x;
    *words++ = offset_y;
    *words++ = speed_x;
    *words = speed_y;
}

void obj_accel_tilt5_f4e4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 height;
    s32 height_speed;
    s32 offset_x;
    s32 offset_y;
    s32 offset_y_speed;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x0;
    s32 y0;
    s32 z0;
    s32 y1;
    s32 z1;
    s32 x1;
    s32 height_new;
    s32 pos;
    s32 t;

    words = obj->data;
    zoom = *words++;
    height = *words++;
    height_speed = *words++;
    offset_x = *words++;
    offset_y = *words++;
    offset_y_speed = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 4;
    for (;;) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x0 = vx * zoom / 0x100;
        l.x = x0;
        y0 = vy * zoom / 0x100;
        l.y = y0;
        z0 = vz * zoom / 0x100;
        l.z = z0;
        pos = y0;
        pos = pos * (gCosineTable[12] / 0x40);
        y1 = (pos - (gSineTable[12] / 0x40) * z0) / 0x100;
        l.y = y1;
        z1 = (z0 * (gCosineTable[12] / 0x40) + (gSineTable[12] / 0x40) * y0) / 0x100;
        l.z = z1;
        x1 = offset_x + x0;
        l.x = x1;
        l.y = y1 + offset_y;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x1;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x1;
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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += 0x400;
    offset_y += offset_y_speed;
    offset_y_speed += 0x60;
    height += height_speed;
    height_speed += 1;
    rec->unk4 = 0xA0 - height;
    rec->unk6 = 0xA0 - height;
    words = obj->data;
    *words++ = zoom;
    *words++ = height;
    *words++ = height_speed;
    *words++ = offset_x;
    *words++ = offset_y;
    *words = offset_y_speed;
}

NONMATCH("asm/code_0817E4F0/obj_zoom_tilt4_f6dc.s", void obj_zoom_tilt4_f6dc(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 angle;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 t;
    s32 pos;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 idx;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 4; i++) {
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
        idx = angle & 0xFF;
        z2 = (z1 * (gCosineTable[idx] / 0x40) - (gSineTable[idx] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[idx] / 0x40) + (gSineTable[idx] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

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
    zoom += vel;
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}
END_NONMATCH

void obj_glide_x_f958(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 base;
    u32 speed;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x_off;
    s32 pos;
    s32 t;

    words = obj->data;
    base = *words++;
    speed = *words++;
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
        x_off = vx + base;
        l.x = x_off;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x_off;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x_off;
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
    base += speed;
    words = obj->data;
    *words++ = base;
    *words = speed;
}

void obj_sink_oam_fa68(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 height;
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

    words = obj->data;
    height = *words++;
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
    height -= 0x20;
    rec->unk4 = height;
    rec->unk6 = height;
    words = obj->data;
    *words = height;
}

void obj_rise_oam_fb70(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 height;
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

    words = obj->data;
    height = *words++;
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
    height += 4;
    rec->unk4 = height;
    rec->unk6 = height;
    words = obj->data;
    *words = height;
}

void obj_fall_accel_fc78(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 height;
    u32 speed;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 y_off;
    s32 pos;
    s32 t;

    words = obj->data;
    height = *words++;
    speed = *words++;
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
        vy = *words++;
        l.y = vy;
        l.z = *words++;
        y_off = vy + height;
        l.y = y_off;
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
    height += speed;
    speed += 0x80;
    words = obj->data;
    *words++ = height;
    *words = speed;
}
