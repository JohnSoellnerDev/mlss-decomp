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

// Single point that bounces: the height falls by its speed, the speed grows by the gravity,
// and each landing sets the bounce stage (the speed is the stage shifted by 8 after a landing).
void obj_bounce_point_plain(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 saved;
    s32 *pt;
    s32 height;
    s32 speed;
    s32 bounce;
    u32 h;
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

ASM_FUNC("asm/code_08184814/obj_spin_rise_model3_fast.s", void obj_spin_rise_model3_fast(void))

ASM_FUNC("asm/code_08184814/obj_spin_rise_model3_a.s", void obj_spin_rise_model3_a(void))

ASM_FUNC("asm/code_08184814/obj_spin_rise_model3_b.s", void obj_spin_rise_model3_b(void))

ASM_FUNC("asm/code_08184814/obj_spin_rise_model3_c.s", void obj_spin_rise_model3_c(void))

ASM_FUNC("asm/code_08184814/obj_spin_rise_model12.s", void obj_spin_rise_model12(void))

// Single point with a zoom that moves by its step each frame; the step drops by 1 above zoom 384
// and the zoom jumps to 256 when the step is negative and the zoom is at most 255.
void obj_pulse_zoom_point(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 count;
    s32 *p2;
    u32 *pslot;
    s32 *p1;
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
    for (count = 0; count < 1; count++) {
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
    zoom += step;
    if (zoom > 384) {
        step--;
    }
    if (step < 0) {
        if (zoom <= 255) {
            zoom = 0x100;
        }
    }
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

// Single point with a zoom that grows by 0x40 each frame; the zoom is width and height.
void obj_grow_zoom_point(struct Point1Obj *obj)
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

    words = obj->data;
    zoom = *words++;
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
    zoom += 0x40;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words = zoom;
}

ASM_FUNC("asm/code_08184814/obj_turn_zoom4_5830.s", void obj_turn_zoom4_5830(void))
// Single point drifting up at a constant speed. Record size 0x60 x 0xC0.
void obj_drift_y_60c0(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *pt;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    velocity = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
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
    height += velocity;
    rec->unk4 = 96;
    rec->unk6 = 192;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = velocity;
}

// Same as obj_drift_y_60c0 with record size 0x30 x 0x60.
void obj_drift_y_3060(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *pt;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    velocity = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
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
    height += velocity;
    rec->unk4 = 48;
    rec->unk6 = 96;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = velocity;
}

// Same as obj_drift_y_60c0 with record size 0x18 x 0x30.
void obj_drift_y_1830(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *pt;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    velocity = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
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
    height += velocity;
    rec->unk4 = 24;
    rec->unk6 = 48;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = velocity;
}

// Single point whose upward speed grows by the gravity each frame. Record size 0x60 x 0xC0.
void obj_fall_y_60c0(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *pt;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    velocity = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
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
    height += velocity;
    velocity += 0x40;
    rec->unk4 = 96;
    rec->unk6 = 192;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = velocity;
}

// Same as obj_fall_y_60c0 with record size 0x30 x 0x60.
void obj_fall_y_3060(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *pt;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    velocity = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
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
    height += velocity;
    velocity += 0x40;
    rec->unk4 = 48;
    rec->unk6 = 96;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = velocity;
}

// Same as obj_fall_y_60c0 with record size 0x18 x 0x30.
void obj_fall_y_1830(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *pt;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 count;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    velocity = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (count = 0; count < 1; count++) {
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
    height += velocity;
    velocity += 0x40;
    rec->unk4 = 24;
    rec->unk6 = 48;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = velocity;
}
