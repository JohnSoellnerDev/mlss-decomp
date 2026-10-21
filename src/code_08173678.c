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

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

void obj_zoom_grow2_3678(struct Point1Obj *obj)
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
        l.z = ((z1 * (gCosineTable[0] / 0x40)) + ((gSineTable[0] / 0x40) * y1)) / 0x100;
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
    vel = (vel * 4) / 5;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}

void obj_point_shrink180_3858(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 head;
    u32 counter;
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
    head = *words++;
    counter = *words++;
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
    counter++;
    rec->unk4 = 0x180 - counter;
    rec->unk6 = 0x180 - counter;
    words = obj->data;
    *words++ = head;
    *words = counter;
}

void obj_point_shrink180_3978(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 head;
    u32 counter;
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
    head = *words++;
    counter = *words++;
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
    counter++;
    rec->unk4 = 0x180 - counter;
    rec->unk6 = 0x180 - counter;
    words = obj->data;
    *words++ = head;
    *words = counter;
}

void obj_zoom_spin4_3a98(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    u32 counter;
    struct ObjRecord *rec;
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
    s32 zp;
    s32 xp;
    s32 yp;
    s32 zz;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    counter = *words++;
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
        t = z1;
        t = t * (gCosineTable[32] / 0x40);
        zp = (t - (gSineTable[32] / 0x40) * x1) / 0x100;
        l.z = zp;
        xp = ((x1 * (gCosineTable[32] / 0x40)) + ((gSineTable[32] / 0x40) * z1)) / 0x100;
        l.x = xp;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        yp = (t - (gSineTable[64] / 0x40) * zp) / 0x100;
        l.y = yp;
        zz = ((zp * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        l.z = zz;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = xp;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = xp;
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
    counter++;
    rec->unk4 = 0x180 - counter;
    rec->unk6 = 0x180 - counter;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = counter;
}


void obj_zoom_grow2_3d00(struct Point1Obj *obj)
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
        l.z = ((z1 * (gCosineTable[0] / 0x40)) + ((gSineTable[0] / 0x40) * y1)) / 0x100;

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

void obj_point_fixed_3ed8(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 head;
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
    head = *words++;
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
    words = obj->data;
    *words = head;
}

void obj_point_fixed_3fd8(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 head;
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
    head = *words++;
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
    words = obj->data;
    *words = head;
}

void obj_point_fixed_40d8(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 head;
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
    head = *words++;
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
    words = obj->data;
    *words = head;
}

void obj_point_fixed_41d8(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 head;
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
    head = *words++;
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
    words = obj->data;
    *words = head;
}

void obj_zoom_tilt5_42d8(struct Point1Obj *obj)
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
    for (i = 0; i < 5; i++) {
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
        t = t * (gCosineTable[42] / 0x40);
        y2 = (t - (gSineTable[42] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[42] / 0x40)) + ((gSineTable[42] / 0x40) * y1)) / 0x100;
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

ASM_FUNC("asm/code_08173678/obj_zoom_spin5_44b8.s", void obj_zoom_spin5_44b8(void))

ASM_FUNC("asm/code_08173678/obj_zoom_spin5_4794.s", void obj_zoom_spin5_4794(void))

ASM_FUNC("asm/code_08173678/obj_zoom_spin6_4a70.s", void obj_zoom_spin6_4a70(void))

void obj_point_shrink_4d4c(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 counter;
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
    counter = *words++;
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
    counter++;
    rec->unk4 = 0x100 - counter;
    rec->unk6 = 0x100 - counter;
    words = obj->data;
    *words = counter;
}

void obj_point_shrink_4e60(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 counter;
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
    counter = *words++;
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
    counter++;
    rec->unk4 = 0x100 - counter;
    rec->unk6 = 0x100 - counter;
    words = obj->data;
    *words = counter;
}
