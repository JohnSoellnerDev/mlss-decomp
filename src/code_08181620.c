#include "global.h"

struct PtRecord
{
    u8 filler0[0x34];
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
};

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;
};

extern struct ModelCtx *gModelCtx;

u32 block_flag_byte(struct PtRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

extern s16 gSineTable[];
extern s16 gCosineTable[];

ASM_FUNC("asm/code_08181620/obj_zoom_tilt10_1620.s", void obj_zoom_tilt10_1620(void))

NONMATCH("asm/code_08181620/obj_zoom_quarter_spin8_1804.s", void obj_zoom_quarter_spin8_1804(struct PtObj *obj))
{
    struct PtLocal l;
    s32 *pt;
    s32 zoom;
    s32 speed;
    s32 angle;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 ys;
    s32 zs;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    speed = *pt++;
    angle = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 8; i++) {
        vx = *pt++;
        l.x = vx;
        l.y = *pt++;
        l.z = *pt++;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        ys = (l.y * zoom) / 0x100;
        l.y = ys;
        zs = (l.z * zoom) / 0x100;
        l.z = zs;
        t = ys;
        t = t * (gCosineTable[64] / 0x40);
        y1 = (t - (gSineTable[64] / 0x40) * zs) / 0x100;
        l.y = y1;
        z1 = ((zs * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * ys)) / 0x100;
        l.z = z1;
        t = x1;
        t = t * (gCosineTable[angle & 0xFF] / 0x40);
        x2 = (t - (gSineTable[angle & 0xFF] / 0x40) * y1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[angle & 0xFF] / 0x40);
        y2 = (t + (gSineTable[angle & 0xFF] / 0x40) * x1) / 0x100;
        l.y = y2 - 0x1000;

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
    zoom -= speed;
    speed += 0x20;
    angle += 3;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = speed;
    *pt = angle;
}
END_NONMATCH

NONMATCH("asm/code_08181620/obj_zoom_tilt_rise8_1a5c.s", void obj_zoom_tilt_rise8_1a5c(struct PtObj *obj))
{
    struct PtLocal l;
    s32 *pt;
    s32 zoom;
    s32 speed;
    s32 angle;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ra;
    s32 rb;
    s32 ty;
    s32 tz;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    speed = *pt++;
    angle = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 8; i++) {
        x = *pt++;
        l.x = x;
        l.y = *pt++;
        l.z = *pt++;
        sx = (x * zoom) / 0x100;
        l.x = sx;
        sy = (l.y * zoom) / 0x100;
        l.y = sy;
        sz = (l.z * zoom) / 0x100;
        l.z = sz;
        t = sz;
        t = t * (gCosineTable[angle & 0xFF] / 0x40);
        ra = (t - (gSineTable[angle & 0xFF] / 0x40) * sx) / 0x100;
        l.z = ra;
        t = sx;
        t = t * (gCosineTable[angle & 0xFF] / 0x40);
        rb = (t + (gSineTable[angle & 0xFF] / 0x40) * sz) / 0x100;
        l.x = rb;
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ty = (t - (gSineTable[12] / 0x40) * ra) / 0x100;
        l.y = ty;
        tz = ((ra * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        l.z = tz;
        l.y = ty + angle;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = rb;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = rb;
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
    zoom -= speed;
    speed += 0x20;
    angle += 3;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt++ = speed;
    *pt = angle;
}
END_NONMATCH

ASM_FUNC("asm/code_08181620/obj_jump_zoom_turn_points_1cb8.s", void obj_jump_zoom_turn_points_1cb8(void))

ASM_FUNC("asm/code_08181620/obj_rise_turn_points_1f4c.s", void obj_rise_turn_points_1f4c(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2180.s", void obj_grow_spin_points_2180(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2428.s", void obj_grow_spin_points_2428(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_26d0.s", void obj_grow_spin_points_26d0(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2980.s", void obj_grow_spin_points_2980(void))

ASM_FUNC("asm/code_08181620/obj_spring_turn_points_2c50.s", void obj_spring_turn_points_2c50(void))
