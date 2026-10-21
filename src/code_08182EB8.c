#include "global.h"

struct PtRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[0x34 - 0x8];
    u32 model;
};

struct PtObj
{
    u8 filler0[8];
    struct PtRecord *rec;
    u32 *data;
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;
};

struct PtLocal
{
    s32 x;
    s32 y;
    s32 z;
    u32 model;
    u32 saved;
};

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;
};

extern struct ModelCtx *gModelCtx;

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct PtRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);


void obj_spring_turn_points_2eb8(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 angle;
    s32 angVel;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s16 *cs;
    s16 *sn;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    angle = *pt++;
    angVel = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 2; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + angle;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    angle += angVel;
    angVel -= angle / 0x80;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = angle;
    *pt = angVel;
}



void obj_spring_turn_points_3120(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 angle;
    s32 angVel;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s16 *cs;
    s16 *sn;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    angle = *pt++;
    angVel = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 2; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + angle;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    angle += angVel;
    angVel -= angle / 0x80;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = angle;
    *pt = angVel;
}


void obj_spring_turn_points_3388(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 angle;
    s32 angVel;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s16 *cs;
    s16 *sn;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    angle = *pt++;
    angVel = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 2; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + angle;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    angle += angVel;
    angVel -= angle / 0x80;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = angle;
    *pt = angVel;
}


void obj_spring_turn_points_35f0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 angle;
    s32 angVel;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s16 *cs;
    s16 *sn;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    angle = *pt++;
    angVel = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 2; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + angle;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    angle += angVel;
    angVel -= angle / 0x80;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = angle;
    *pt = angVel;
}


void obj_scroll_point_3858(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 offset;
    s32 counter;
    struct PtRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    offset = *pt++;
    counter = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        l.y = *pt++;
        l.z = *pt++;
        l.x = x + offset;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = l.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = l.x;
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
    offset += 0x300;
    counter += 8;
    rec->unk4 = counter;
    rec->unk6 = counter;
    pt = (s32 *)obj->data;
    *pt++ = offset;
    *pt = counter;
}


void obj_scroll_bounce_point_3978(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 offset;
    s32 height;
    s32 speed;
    s32 counter;
    struct PtRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    offset = *pt++;
    height = *pt++;
    speed = *pt++;
    counter = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
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
        l.x = x + offset;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = l.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = l.x;
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
    offset += 0x200;
    height += speed;
    speed += 0x80;
    if (height > 0) {
        speed = -0x600;
    }
    counter += 4;
    rec->unk4 = counter;
    rec->unk6 = counter;
    pt = (s32 *)obj->data;
    *pt++ = offset;
    *pt++ = height;
    *pt++ = speed;
    *pt = counter;
}


void obj_grow_turn_points_3ac0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s16 *cs;
    s16 *sn;
    s32 *pt;
    s32 zoom;
    s32 angle;
    s32 angVel;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    angle = *pt++;
    angVel = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 3; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + angVel;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    zoom += 0x100;
    angle += 4;
    angVel -= 0x100;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = angle;
    *pt = angVel;
}



void obj_damped_zoom_turn_points_3d24(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s16 *cs;
    s16 *sn;
    s32 *pt;
    s32 zoom;
    s32 step;
    s32 angle;
    s32 height;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    step = *pt++;
    angle = *pt++;
    height = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 3; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + height;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    angle += 8;
    height -= 0x200;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = step;
    *pt++ = angle;
    *pt = height;
}



void obj_bounce_point_3fb0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 height;
    s32 speed;
    s32 bounce;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    bounce = *pt++;
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
    speed += 0x40;
    if (height > 0) {
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        speed = bounce << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt++ = speed;
    *pt = bounce;
}


void obj_bounce_point_40dc(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 height;
    s32 speed;
    s32 bounce;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    bounce = *pt++;
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
    speed += 0x40;
    if (height > 0) {
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        speed = bounce << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt++ = speed;
    *pt = bounce;
}


void obj_bounce_zoom_point_a(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 step;
    s32 height;
    s32 vel;
    s32 stage;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    step = *pt++;
    height = *pt++;
    vel = *pt++;
    stage = *pt++;
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
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + height;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    height += vel;
    vel += 0x40;
    if (height > 0) {
        step = step / 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        vel = stage << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = step;
    *pt++ = height;
    *pt++ = vel;
    *pt = stage;
}


void obj_bounce_zoom_point_b(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 step;
    s32 height;
    s32 vel;
    s32 stage;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    step = *pt++;
    height = *pt++;
    vel = *pt++;
    stage = *pt++;
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
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + height;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    height += vel;
    vel += 0x40;
    if (height > 0) {
        step = step / 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        vel = stage << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = step;
    *pt++ = height;
    *pt++ = vel;
    *pt = stage;
}



void obj_bounce_zoom_model10(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 zoom;
    s32 step;
    s32 height;
    s32 vel;
    s32 stage;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    step = *pt++;
    height = *pt++;
    vel = *pt++;
    stage = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 10; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x1 = (x * zoom) / 0x100;
        l.x = x1;
        y1 = (y * zoom) / 0x100;
        l.y = y1;
        z1 = (z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 + height;
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
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    height += vel;
    vel += 0x40;
    if (height > 0) {
        step = step / 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        vel = stage << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = step;
    *pt++ = height;
    *pt++ = vel;
    *pt = stage;
}


