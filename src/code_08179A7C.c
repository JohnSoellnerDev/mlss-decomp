#include "global.h"

extern s16 gSineTable[];
extern s16 gCosineTable[];

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

ASM_FUNC("asm/code_08179A7C/spin_scroll_9a7c.s", void spin_scroll_9a7c(void))
ASM_FUNC("asm/code_08179A7C/spin_scroll_9c94.s", void spin_scroll_9c94(struct Point1Obj *obj))
ASM_FUNC("asm/code_08179A7C/spin_offset_9ea4.s", void spin_offset_9ea4(void))
ASM_FUNC("asm/code_08179A7C/spin_offset_a0cc.s", void spin_offset_a0cc(void))

ASM_FUNC("asm/code_08179A7C/zoom_turn_a2f8.s", void zoom_turn_a2f8(void))

ASM_FUNC("asm/code_08179A7C/zoom_turn_a534.s", void zoom_turn_a534(void))

ASM_FUNC("asm/code_08179A7C/obj_model8_zoom_xslide_a770.s", void obj_model8_zoom_xslide_a770(void))

void obj_point_static_oam_160_a99c(struct Point1Obj *obj)
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
    rec->unk4 = 0x160;
    rec->unk6 = 0x160;
}

NONMATCH("asm/code_08179A7C/obj_model8_zoom_vel7_oam_130_aaa0.s", void obj_model8_zoom_vel7_oam_130_aaa0(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 off;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    u32 *words;
    s32 zoom;
    s32 vel;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 z2;
    s32 z3;
    s32 x2;
    s32 pos;
    s32 t;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    off = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 7;
    for (;;) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        l.x = (vx * zoom) / 0x100;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        pos = y1;
        pos = pos * (gCosineTable[64] / 0x40);
        y2 = (pos - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        z2 = (z1 * (gCosineTable[64] / 0x40) + (gSineTable[64] / 0x40) * y1) / 0x100;
        l.z = z2;
        z3 = (z2 * (gCosineTable[82] / 0x40) - (gSineTable[82] / 0x40) * l.x) / 0x100;
        l.z = z3;
        x2 = (l.x * (gCosineTable[82] / 0x40) + (gSineTable[82] / 0x40) * z2) / 0x100;
        l.x = x2;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x2;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x2;
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
    zoom += vel;
    vel = (vel * 7) / 8;
    off -= 0x80;
    rec->unk4 = 0x130;
    rec->unk6 = 0x130;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = off;
}
END_NONMATCH

NONMATCH("asm/code_08179A7C/obj_model8_zoom_vel7_oam_130_acd8.s", void obj_model8_zoom_vel7_oam_130_acd8(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 off;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    u32 *words;
    s32 zoom;
    s32 vel;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 z2;
    s32 z3;
    s32 x2;
    s32 pos;
    s32 t;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    off = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 7;
    for (;;) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        l.x = (vx * zoom) / 0x100;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        pos = y1;
        pos = pos * (gCosineTable[64] / 0x40);
        y2 = (pos - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        z2 = (z1 * (gCosineTable[64] / 0x40) + (gSineTable[64] / 0x40) * y1) / 0x100;
        l.z = z2;
        z3 = (z2 * (gCosineTable[82] / 0x40) - (gSineTable[82] / 0x40) * l.x) / 0x100;
        l.z = z3;
        x2 = (l.x * (gCosineTable[82] / 0x40) + (gSineTable[82] / 0x40) * z2) / 0x100;
        l.x = x2;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x2;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x2;
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
    zoom += vel;
    vel = (vel * 7) / 8;
    off -= 0x80;
    rec->unk4 = 0x130;
    rec->unk6 = 0x130;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = off;
}
END_NONMATCH

void obj_model5_zoom_drop_af10(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
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
    s32 pos;
    s32 t;

    words = obj->data;
    zoom = *words++;
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
    words = obj->data;
    *words++ = zoom;
    *words++ = offset_x;
    *words++ = offset_y;
    *words = offset_y_speed;
}

void obj_model5_zoom_fall_oam_b0e0(struct Point1Obj *obj)
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
