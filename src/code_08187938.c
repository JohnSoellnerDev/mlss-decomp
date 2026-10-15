#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 width;
    u16 height;
    u8 filler8[4];
    u16 angle;
    u8 filler10[0x34 - 0x10];
    u32 model;
};

struct PointObj
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

struct PointLocal
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

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

// One point falls and spins the other way: the offsets grow by their speeds, the angle falls by 6 per frame.
void obj_fall_spin_point_a(struct PointObj *obj)
{
    struct PointLocal l;
    s32 velX;
    s32 velY;
    struct ObjRecord *rec;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 yOff;
    s32 angle;
    u32 h;
    u32 *words;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    yOff = *words++;
    velY = *words++;
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
    yOff += velY;
    velY += 0x20;
    angle += -6;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = yOff;
    *words++ = velY;
    *words = angle;
}

// One point falls and spins: the offsets grow by their speeds, the angle grows by 6 per frame.
void obj_fall_spin_point_b(struct PointObj *obj)
{
    struct PointLocal l;
    s32 velX;
    s32 velY;
    struct ObjRecord *rec;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 yOff;
    s32 angle;
    u32 h;
    u32 *words;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    yOff = *words++;
    velY = *words++;
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
    yOff += velY;
    velY += 0x20;
    angle += 6;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = yOff;
    *words++ = velY;
    *words = angle;
}

// Point that slides: the position grows by a constant speed; the record gets size 0xC0 x 0x180.
void obj_slide_speed_point(struct PointObj *obj)
{
    struct PointLocal l;
    u32 vel;
    u32 height;
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

    words = obj->data;
    height = *words++;
    vel = *words++;
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
        l.y += height;
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
    height += vel;
    rec->width = 0xC0;
    rec->height = 0x180;
    words = obj->data;
    *words++ = height;
    *words = vel;
}

// Point that moves up in y at a constant speed; the record gets size 0x60 x 0xC0.
void obj_drift_y_60c0_7d08(struct PointObj *obj)
{
    struct PointLocal l;
    u32 vel;
    u32 height;
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

    words = obj->data;
    height = *words++;
    vel = *words++;
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
        l.y += height;
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
    height += vel;
    rec->width = 0x60;
    rec->height = 0xC0;
    words = obj->data;
    *words++ = height;
    *words = vel;
}

// Point that moves up in y at a constant speed; the record gets size 0x30 x 0x60.
void obj_drift_y_3060_7e24(struct PointObj *obj)
{
    struct PointLocal l;
    u32 vel;
    u32 height;
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

    words = obj->data;
    height = *words++;
    vel = *words++;
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
        l.y += height;
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
    height += vel;
    rec->width = 0x30;
    rec->height = 0x60;
    words = obj->data;
    *words++ = height;
    *words = vel;
}

// Point that moves up in y at a constant speed; the record is left unchanged.
void obj_drift_y_keep_7f40(struct PointObj *obj)
{
    struct PointLocal l;
    u32 vel;
    u32 height;
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

    words = obj->data;
    height = *words++;
    vel = *words++;
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
        l.y += height;
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
    height += vel;
    words = obj->data;
    *words++ = height;
    *words = vel;
}

// One point hops and spins: it bounces off the floor, drifts sideways and turns by 4 per frame.
void obj_bounce_spin_point_8050(struct PointObj *obj)
{
    struct PointLocal l;
    s32 xOff;
    s32 sideVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    u32 h;
    s32 height;
    s32 fallVel;
    s32 stage;
    u32 *words;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    sideVel = *words++;
    height = *words++;
    fallVel = *words++;
    stage = *words++;
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
        l.y += height;
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
    xOff += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        sideVel /= 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        fallVel = stage << 8;
    }
    angle += 4;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = sideVel;
    *words++ = height;
    *words++ = fallVel;
    *words++ = stage;
    *words = angle;
}

// One point hops and spins: it bounces off the floor, drifts sideways and turns by 0x10 per frame.
void obj_bounce_spin16_point_81d0(struct PointObj *obj)
{
    struct PointLocal l;
    s32 xOff;
    s32 sideVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    u32 h;
    s32 height;
    s32 fallVel;
    s32 stage;
    u32 *words;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    sideVel = *words++;
    height = *words++;
    fallVel = *words++;
    stage = *words++;
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
        l.y += height;
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
    xOff += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        sideVel /= 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        fallVel = stage << 8;
    }
    angle += 0x10;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = sideVel;
    *words++ = height;
    *words++ = fallVel;
    *words++ = stage;
    *words = angle;
}

// One point hops and spins: it bounces off the floor, drifts sideways and turns by 0x10 per frame.
void obj_bounce_spin16_point_8350(struct PointObj *obj)
{
    struct PointLocal l;
    s32 xOff;
    s32 sideVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    u32 h;
    s32 height;
    s32 fallVel;
    s32 stage;
    u32 *words;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    sideVel = *words++;
    height = *words++;
    fallVel = *words++;
    stage = *words++;
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
        l.y += height;
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
    xOff += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        sideVel /= 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        fallVel = stage << 8;
    }
    angle += 0x10;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = sideVel;
    *words++ = height;
    *words++ = fallVel;
    *words++ = stage;
    *words = angle;
}

// One point drifts sideways and up while it spins by 4 per frame and shrinks by 0xC per frame.
NONMATCH("asm/code_08187938/obj_drift_spin_shrink_84d0.s", void obj_drift_spin_shrink_84d0(struct PointObj *obj))
{
    struct PointLocal l;
    s32 velX;
    s32 velY;
    u32 size;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 height;
    s32 angle;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    height = *words++;
    velY = *words++;
    angle = *words++;
    size = *words++;
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
        l.y += height;
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
    height += velY;
    angle += 4;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    size -= 0xc;
    rec->width = size;
    rec->height = *(u16 *)&size;
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = height;
    *words++ = velY;
    *words++ = angle;
    *words = size;
}
END_NONMATCH

ASM_FUNC("asm/code_08187938/obj_spin_zoom5_863c.s", void obj_spin_zoom5_863c(void))

// Five points whose y offset falls by 0x400 per frame while the record shrinks by 0x20 per frame.
void obj_shrink_y_offset5_88f0(struct PointObj *obj)
{
    struct PointLocal l;
    u32 saved;
    u32 size;
    u32 yOff;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    size = *words++;
    yOff = *words++;
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
    yOff += -0x400;
    size -= 0x20;
    rec->width = size;
    rec->height = size;
    words = obj->data;
    *words++ = size;
    *words = yOff;
}

// Five points whose y offset falls by 0x400 per frame while the record shrinks by 0x20 per frame.
void obj_shrink_y_offset5_8a10(struct PointObj *obj)
{
    struct PointLocal l;
    u32 saved;
    u32 size;
    u32 yOff;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    size = *words++;
    yOff = *words++;
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
    yOff += -0x400;
    size -= 0x20;
    rec->width = size;
    rec->height = size;
    words = obj->data;
    *words++ = size;
    *words = yOff;
}

// Five points whose y offset falls by 0x400 per frame while the record shrinks by 0x20 per frame.
void obj_shrink_y_offset5_8b30(struct PointObj *obj)
{
    struct PointLocal l;
    u32 saved;
    u32 size;
    u32 yOff;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    size = *words++;
    yOff = *words++;
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
    yOff += -0x400;
    size -= 0x20;
    rec->width = size;
    rec->height = size;
    words = obj->data;
    *words++ = size;
    *words = yOff;
}

// One point bounces and drifts sideways: a fall speed and a side speed, halved on each landing.
void obj_bounce_x_point_8c50(struct PointObj *obj)
{
    struct PointLocal l;
    u32 unused;
    u32 h;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 sideVel;
    s32 height;
    s32 fallVel;
    s32 stage;
    u32 *words;
    struct ObjRecord *rec;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    sideVel = *words++;
    height = *words++;
    fallVel = *words++;
    stage = *words++;
    unused = *words++;
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
        l.y += height;
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
    xOff += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        sideVel /= 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        fallVel = stage << 8;
    }
    words = obj->data;
    *words++ = xOff;
    *words++ = sideVel;
    *words++ = height;
    *words++ = fallVel;
    *words++ = stage;
    *words = unused;
}

// One point bounces and drifts sideways: a fall speed and a side speed, halved on each landing.
void obj_bounce_x_point_8da4(struct PointObj *obj)
{
    struct PointLocal l;
    u32 unused;
    u32 h;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 sideVel;
    s32 height;
    s32 fallVel;
    s32 stage;
    u32 *words;
    struct ObjRecord *rec;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    sideVel = *words++;
    height = *words++;
    fallVel = *words++;
    stage = *words++;
    unused = *words++;
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
        l.y += height;
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
    xOff += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        sideVel /= 2;
        height = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        fallVel = stage << 8;
    }
    words = obj->data;
    *words++ = xOff;
    *words++ = sideVel;
    *words++ = height;
    *words++ = fallVel;
    *words++ = stage;
    *words = unused;
}

// One point falls and sways sideways while spinning by 2 per frame; the record angle is negated on bit 7.
void obj_fall_x_spin_point_8ef8(struct PointObj *obj)
{
    struct PointLocal l;
    s32 velX;
    s32 velY;
    struct ObjRecord *rec;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 yOff;
    s32 angle;
    u32 h;
    u32 *words;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    yOff = *words++;
    velY = *words++;
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
    yOff += velY;
    velY += 0x40;
    angle += 2;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = yOff;
    *words++ = velY;
    *words = angle;
}

// One point drifts with a constant speed in x and y while the record shrinks by 0x10 per frame.
void obj_drift_shrink_9050(struct PointObj *obj)
{
    struct PointLocal l;
    u32 velX;
    u32 velY;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    u32 offX;
    u32 offY;
    u32 size;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
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
    for (count = 0; count < 1; count++) {
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
    size += -0x10;
    rec->width = size;
    rec->height = size;
    words = obj->data;
    *words++ = offX;
    *words++ = velX;
    *words++ = offY;
    *words++ = velY;
    *words = size;
}
