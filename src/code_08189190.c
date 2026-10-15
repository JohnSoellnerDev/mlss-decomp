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

extern struct ModelCtx *gUnk_03000D74;

extern s16 gCosineTable[];
extern s16 gSineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

// One point that drifts at a constant speed; the object record stays unchanged. The offset in x
// and y grows by its speed every frame.
void obj_drift_point_9190(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 velX;
    s32 velY;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;
    s32 offX;
    s32 offY;

    words = obj->data;
    offX = *words++;
    velX = *words++;
    offY = *words++;
    velY = *words++;
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
        vx += offX;
        l.x = vx;
        l.y += offY;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    offX += velX;
    offY += velY;
    words = obj->data;
    *words++ = offX;
    *words++ = velX;
    *words++ = offY;
    *words = velY;
}

// Two points are scaled by the zoom (8.8), then turned by the fixed tilt angle in the y-z plane
// (entry 12 of the sine and cosine tables). The zoom grows by its step per frame, and the step
// drops to 7/8 per frame.
NONMATCH("asm/code_08189190/obj_zoom_tilt_model2_92b4.s", void obj_zoom_tilt_model2_92b4(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 zoomStep;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 sy;
    s32 sx;
    s32 sz;
    s32 pos;
    s32 t;

    words = obj->data;
    zoom = *words++;
    zoomStep = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 2; i++) {
        l.x = *words++;
        l.y = *words++;
        l.z = *words++;
        sx = l.x * zoom / 0x100;
        l.x = sx;
        sy = l.y * zoom / 0x100;
        l.y = sy;
        sz = l.z * zoom / 0x100;
        l.z = sz;
        l.y = (sy * (gCosineTable[12] / 0x40) - sz * (gSineTable[12] / 0x40)) / 0x100;
        l.z = (sz * (gCosineTable[12] / 0x40) + sy * (gSineTable[12] / 0x40)) / 0x100;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = sx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = sx;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    zoom += zoomStep;
    zoomStep = zoomStep * 7 / 8;
    words = obj->data;
    *words++ = zoom;
    *words = zoomStep;
}
END_NONMATCH

// Two points are scaled by the zoom (8.8), then turned by the fixed tilt angle in the y-z plane
// (entry 12 of the sine and cosine tables). The zoom grows by its step per frame, and the step
// drops to 7/8 per frame.
NONMATCH("asm/code_08189190/obj_zoom_tilt_model2_948c.s", void obj_zoom_tilt_model2_948c(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 zoomStep;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 sy;
    s32 sx;
    s32 sz;
    s32 pos;
    s32 t;

    words = obj->data;
    zoom = *words++;
    zoomStep = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 2; i++) {
        l.x = *words++;
        l.y = *words++;
        l.z = *words++;
        sx = l.x * zoom / 0x100;
        l.x = sx;
        sy = l.y * zoom / 0x100;
        l.y = sy;
        sz = l.z * zoom / 0x100;
        l.z = sz;
        l.y = (sy * (gCosineTable[12] / 0x40) - sz * (gSineTable[12] / 0x40)) / 0x100;
        l.z = (sz * (gCosineTable[12] / 0x40) + sy * (gSineTable[12] / 0x40)) / 0x100;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = sx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = sx;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    zoom += zoomStep;
    zoomStep = zoomStep * 7 / 8;
    words = obj->data;
    *words++ = zoom;
    *words = zoomStep;
}
END_NONMATCH

// One point that drifts and grows: the offset in x and y grows by its speed per frame, and the
// record size grows by 0x10 per frame and sets both record width and height.
void obj_drift_grow_9664(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 offX;
    s32 offY;
    s32 size;
    struct ObjRecord *rec;
    s32 velX;
    s32 velY;
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
    offX = *words++;
    velX = *words++;
    offY = *words++;
    velY = *words++;
    size = *words++;
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
        vx += offX;
        l.x = vx;
        l.y += offY;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    offX += velX;
    offY += velY;
    size += 0x10;
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words++ = offX;
    *words++ = velX;
    *words++ = offY;
    *words++ = velY;
    *words = size;
}

// One point that drifts and spins: the offset in x and y grows by its speed per frame and the
// angle turns 4 steps per frame. After the frame counter passes 10 the record shrinks by 0x10 per
// frame. Bit 7 of the flag byte negates the record angle.
void obj_drift_spin_late_shrink_97a4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 offX;
    s32 velX;
    s32 offY;
    s32 velY;
    s32 angle;
    s32 size;
    s32 frames;
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
    offX = *words++;
    velX = *words++;
    offY = *words++;
    velY = *words++;
    angle = *words++;
    size = *words++;
    frames = *words++;
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
        vx += offX;
        l.x = vx;
        l.y += offY;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    offX += velX;
    offY += velY;
    angle += 4;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    frames++;
    if (frames > 10) {
        size -= 0x10;
    }
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words++ = offX;
    *words++ = velX;
    *words++ = offY;
    *words++ = velY;
    *words++ = angle;
    *words++ = size;
    *words = frames;
}

// One point that grows and spins: the size (8.8) grows by its step per frame up to 0x100, and the
// angle turns 3 steps per frame. After the frame counter passes 52 the growth becomes -0x10 and
// the point shrinks.
void obj_grow_spin_shrink_9924(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 size;
    s32 grow;
    s32 frames;
    struct ObjRecord *rec;
    u32 h;
    s32 angle;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    size = *words++;
    grow = *words++;
    frames = *words++;
    angle = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    size += grow;
    if (size > 0x100) {
        size = 0x100;
    }
    angle += 3;
    rec->unk4 = size;
    rec->unk6 = size;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    frames++;
    if (frames > 0x34) {
        grow = -0x10;
    }
    words = obj->data;
    *words++ = size;
    *words++ = grow;
    *words++ = frames;
    *words = angle;
}

ASM_FUNC("asm/code_08189190/obj_zoom_spin6_9a84.s", void obj_zoom_spin6_9a84(void))

ASM_FUNC("asm/code_08189190/obj_zoom_spin6_9cc8.s", void obj_zoom_spin6_9cc8(void))

ASM_FUNC("asm/code_08189190/obj_spin_zoom5_9f20.s", void obj_spin_zoom5_9f20(void))

// One point that grows: the record's width and height (8.8) grow by 0x10 per frame up to 0x100,
// and the record angle grows by 0x10 per frame (negated when bit 7 of the flag byte is set).
// The point position stays unchanged.
void obj_grow_spin_point_a1d4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 size;
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
    s32 angle;

    words = obj->data;
    size = *words++;
    angle = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    size += 0x10;
    if (size > 0x100) {
        size = 0x100;
    }
    angle += 0x10;
    rec->unk4 = size;
    rec->unk6 = size;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    words = obj->data;
    *words++ = size;
    *words = angle;
}

ASM_FUNC("asm/code_08189190/obj_spin_model5.s", void obj_spin_model5(void))

// One point that falls: y grows by vy, vy grows by 0x40 per frame (gravity), and x grows by the
// x offset, which grows by vx per frame. Bit 7 of the flag byte mirrors the x shift.
void obj_fall_point(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 velX;
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
    s32 yOff;
    s32 vy;
    s32 xOff;

    words = obj->data;
    yOff = *words++;
    vy = *words++;
    xOff = *words++;
    velX = *words++;
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
        l.y += yOff;
        vx += xOff;
        l.x = vx;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    yOff += vy;
    vy += 0x40;
    xOff += velX;
    rec->unk6 = 0xFF00;
    words = obj->data;
    *words++ = yOff;
    *words++ = vy;
    *words++ = xOff;
    *words = velX;
}


// Same as obj_fall_point (other effect callback). One point that falls: y grows by vy, vy grows by 0x40 per frame (gravity), and x grows by the
// x offset, which grows by vx per frame. Bit 7 of the flag byte mirrors the x shift.
void obj_fall_point_b(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 velX;
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
    s32 yOff;
    s32 vy;
    s32 xOff;

    words = obj->data;
    yOff = *words++;
    vy = *words++;
    xOff = *words++;
    velX = *words++;
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
        l.y += yOff;
        vx += xOff;
        l.x = vx;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    yOff += vy;
    vy += 0x40;
    xOff += velX;
    rec->unk6 = 0xFF00;
    words = obj->data;
    *words++ = yOff;
    *words++ = vy;
    *words++ = xOff;
    *words = velX;
}


// One point that bounces off the floor (y = 0): gravity grows by 0x40 per frame. On impact
// (y > 0) y is reset to 0, vx is halved, and the next jump is vy = level << 8. The level counts
// down at each impact and stays at 0 or below. The angle grows per frame; bit 7 of the flag byte
// mirrors the angle and the x shift.
void obj_bounce_point(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 xOff;
    s32 velX;
    s32 angle;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;
    s32 yOff;
    s32 vy;
    s32 level;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    yOff = *words++;
    vy = *words++;
    level = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        vx += xOff;
        l.x = vx;
        l.y += yOff;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    xOff += velX;
    yOff += vy;
    vy += 0x40;
    if (yOff > 0) {
        velX = velX / 2;
        yOff = 0;
        level++;
        if (level > 0) {
            level = 0;
        }
        vy = level << 8;
    }
    angle += 0x10;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = yOff;
    *words++ = vy;
    *words++ = level;
    *words = angle;
}

// One point that slides: the position in the first data word grows by 0x20 per frame. It is
// also stored as halfwords in the object record (+4, and +6 is 0xC0 lower).
void obj_slide_point(struct Point1Obj *obj)
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
    s32 base;

    words = obj->data;
    base = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    base += 0x20;
    rec->unk4 = base;
    rec->unk6 = base - 0xC0;
    words = obj->data;
    *words = base;
}
