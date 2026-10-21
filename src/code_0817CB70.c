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

// Single point whose size shrinks: the data block holds the size, the point follows it.
void obj_point_shrink_cb70(struct Point1Obj *obj)
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
    s32 zraw;
    s32 pos;
    s32 t;
    u32 size;

    words = obj->data;
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
        zraw = *words++;
        l.y -= 0xC00;
        l.z = zraw - 0x100;
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
    size -= 0x20;
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words = size;
}

// Single point that slides in X: the data block is an X offset, reduced by 0x80 per frame.
// The object record gets a 0xA0 x 0xA0 OAM size.
void obj_point_slide_x_oam_a0_cc8c(struct Point1Obj *obj)
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
    s32 offset;
    s32 xo;

    words = obj->data;
    offset = *words++;
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
        xo = vx + offset;
        l.x = xo;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = xo;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = xo;
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
    offset -= 0x80;
    rec->unk4 = 0xA0;
    rec->unk6 = 0xA0;
    words = obj->data;
    *words = offset;
}

// Single point that slides in X: the data block is an X offset, reduced by 0x80 per frame.
// The object record gets a 0x160 x 0x160 OAM size.
void obj_point_slide_x_oam_160_cd9c(struct Point1Obj *obj)
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
    s32 offset;
    s32 xo;

    words = obj->data;
    offset = *words++;
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
        xo = vx + offset;
        l.x = xo;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = xo;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = xo;
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
    offset -= 0x80;
    rec->unk4 = 0x160;
    rec->unk6 = 0x160;
    words = obj->data;
    *words = offset;
}


// Single point that drifts: the data block holds a base position and a velocity for X and Y.
// Each frame the base moves by its velocity, the point is offset by the base and placed.
void obj_drift_point_ceac(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vel_x;
    s32 vel_y;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 base_x;
    s32 base_y;
    s32 pos;
    s32 t;

    words = obj->data;
    base_x = *words++;
    vel_x = *words++;
    base_y = *words++;
    vel_y = *words++;
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
        vx += base_x;
        l.x = vx;
        vy += base_y;
        l.y = vy;
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
    base_x += vel_x;
    base_y += vel_y;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    words = obj->data;
    *words++ = base_x;
    *words++ = vel_x;
    *words++ = base_y;
    *words = vel_y;
}


// Single point that drifts: the data block holds a base position and a velocity for X and Y.
// Each frame the base moves by its velocity, the point is offset by the base and placed.
void obj_drift_point_cfe0(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vel_x;
    s32 vel_y;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 base_x;
    s32 base_y;
    s32 pos;
    s32 t;

    words = obj->data;
    base_x = *words++;
    vel_x = *words++;
    base_y = *words++;
    vel_y = *words++;
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
        vx += base_x;
        l.x = vx;
        vy += base_y;
        l.y = vy;
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
    base_x += vel_x;
    base_y += vel_y;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    words = obj->data;
    *words++ = base_x;
    *words++ = vel_x;
    *words++ = base_y;
    *words = vel_y;
}


// Single point that drifts: the data block holds a base position and a velocity for X and Y.
// Each frame the base moves by its velocity, the point is offset by the base and placed.
void obj_drift_point_d114(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vel_x;
    s32 vel_y;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 base_x;
    s32 base_y;
    s32 pos;
    s32 t;

    words = obj->data;
    base_x = *words++;
    vel_x = *words++;
    base_y = *words++;
    vel_y = *words++;
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
        vx += base_x;
        l.x = vx;
        vy += base_y;
        l.y = vy;
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
    base_x += vel_x;
    base_y += vel_y;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    words = obj->data;
    *words++ = base_x;
    *words++ = vel_x;
    *words++ = base_y;
    *words = vel_y;
}


// Single point that drifts: the data block holds a base position and a velocity for X and Y.
// Each frame the base moves by its velocity, the point is offset by the base and placed.
void obj_drift_point_d248(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vel_x;
    s32 vel_y;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 base_x;
    s32 base_y;
    s32 pos;
    s32 t;

    words = obj->data;
    base_x = *words++;
    vel_x = *words++;
    base_y = *words++;
    vel_y = *words++;
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
        vx += base_x;
        l.x = vx;
        vy += base_y;
        l.y = vy;
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
    base_x += vel_x;
    base_y += vel_y;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    words = obj->data;
    *words++ = base_x;
    *words++ = vel_x;
    *words++ = base_y;
    *words = vel_y;
}


// Single point that drifts in X: the data block holds the X base and its velocity. The base
// moves by the velocity each frame, the point is offset by the base. The OAM size is 0xC0.
void obj_drift_x_point_d37c(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vel_x;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 xo;
    s32 base_x;
    s32 pos;
    s32 t;

    words = obj->data;
    base_x = *words++;
    vel_x = *words++;
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
        xo = vx + base_x;
        l.x = xo;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = xo;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = xo;
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
    base_x += vel_x;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    words = obj->data;
    *words++ = base_x;
    *words = vel_x;
}


// Single point with a fixed OAM size: the data word is the size written to the object record.
void obj_sized_point_d494(struct Point1Obj *obj)
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
    u32 size;

    words = obj->data;
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
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words = size;
}


// Model of 5 points, tilted about the X axis: the data block holds the zoom, the shrink
// (the OAM size is 0xA0 minus it) with its speed, the X offset, the lift and its speed.
void obj_tilt_model5_d5a0(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 hub;
    s32 shrink;
    s32 shrink_speed;
    s32 xoff;
    s32 hub_speed;
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
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 xv;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrink_speed = *words++;
    xoff = *words++;
    hub = *words++;
    hub_speed = *words++;
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
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        xv = xoff + x1;
        l.x = xv;
        l.y = y2 + hub;
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = xv;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = xv;
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
    hub += hub_speed;
    hub_speed += 0x60;
    shrink += shrink_speed;
    shrink_speed += 1;
    rec->unk4 = 0xA0 - shrink;
    rec->unk6 = 0xA0 - shrink;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrink_speed;
    *words++ = xoff;
    *words++ = hub;
    *words = hub_speed;
}


// Same code as the function above (second copy).
void obj_tilt_model5_d798(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 hub;
    s32 shrink;
    s32 shrink_speed;
    s32 xoff;
    s32 hub_speed;
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
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 xv;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrink_speed = *words++;
    xoff = *words++;
    hub = *words++;
    hub_speed = *words++;
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
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        xv = xoff + x1;
        l.x = xv;
        l.y = y2 + hub;
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = xv;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = xv;
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
    hub += hub_speed;
    hub_speed += 0x60;
    shrink += shrink_speed;
    shrink_speed += 1;
    rec->unk4 = 0xA0 - shrink;
    rec->unk6 = 0xA0 - shrink;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrink_speed;
    *words++ = xoff;
    *words++ = hub;
    *words = hub_speed;
}

ASM_FUNC("asm/code_0817CB70/obj_spin_model4_d990.s", void obj_spin_model4_d990(void))

ASM_FUNC("asm/code_0817CB70/obj_zoom_model8_dbf0.s", void obj_zoom_model8_dbf0(void))

// Single fixed point (no data words): it is raised by 8 units and placed at the object
// position. The object record gets a 0x1C0 x 0x1C0 OAM size.
void obj_fixed_point_de6c(struct Point1Obj *obj)
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

    words = obj->data;
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
        l.y -= 0x800;
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
    rec->unk4 = 0x1C0;
    rec->unk6 = 0x1C0;
}


// Single fixed point (no data words): it is raised by 8 units and placed at the object
// position. The object record gets a 0x200 x 0x200 OAM size.
void obj_fixed_point_df7c(struct Point1Obj *obj)
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

    words = obj->data;
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
        l.y -= 0x800;
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
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
}


NONMATCH("asm/code_0817CB70/obj_perm_spin8_e08c.s", void obj_perm_spin8_e08c(struct Point1Obj *obj))
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 vel;
    s32 angle;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 y3;
    s32 t;
    s32 pos;

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
    for (i = 0; i < 8; i++) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        pos = y1;
        pos = pos * (gCosineTable[64] / 0x40);
        y2 = (pos - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        pos = z1;
        pos = pos * (gCosineTable[64] / 0x40);
        z2 = (pos + (gSineTable[64] / 0x40) * y1) / 0x100;
        l.z = z2;
        t = x1;
        t = t * (gCosineTable[angle & 0xFF] / 0x40);
        x2 = (t - (gSineTable[angle & 0xFF] / 0x40) * y2) / 0x100;
        l.x = x2;
        y3 = (y2 * (gCosineTable[angle & 0xFF] / 0x40) + (gSineTable[angle & 0xFF] / 0x40) * x1) / 0x100;
        l.y = y3;
        l.x = obj->posX + x2 / 0x100;
        l.y = obj->posY + y3 / 0x100;
        l.z = obj->posZ + z2 / 0x100;
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
    t = 0;
    zoom += vel;
    vel = (vel * 7) / 8;
    angle += 3;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}
END_NONMATCH

// Model of 9 points tilted about the X axis, with a flag-dependent X offset.
// The data block holds the zoom, the height and its speed, the Y offset and its speed, and
// the X speed (written to the record as a signed value).
void obj_accel_tilt9_e2c4(struct Point1Obj *obj)
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
    s32 vy;
    s32 vz;
    s32 x1;
    s32 y1;
    s32 z1;
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
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        l.y = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = l.y + offset_y;
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
    rec->unkC = (obj->flags & 0x80) ? -x_speed << 8 : x_speed << 8;
    words = obj->data;
    *words++ = zoom;
    *words++ = height;
    *words++ = height_speed;
    *words++ = offset_y;
    *words++ = offset_y_speed;
    *words = x_speed;
}

