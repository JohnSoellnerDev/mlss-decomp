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

NONMATCH("asm/code_08181620/obj_zoom_tilt10_1620.s", void obj_zoom_tilt10_1620(struct PtObj *obj))
{
    struct PtLocal l;
    struct PtRecord *rec;
    s32 zoom;
    s32 speed;
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
    s32 u;

    words = obj->data;
    zoom = *words++;
    speed = *words++;
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
        u = y1;
        u = u * (gCosineTable[12] / 0x40);
        y2 = (u - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

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
    zoom += speed;
    speed = (speed * 7) / 8;
    words = obj->data;
    *words++ = zoom;
    *words = speed;
}
END_NONMATCH

void obj_zoom_quarter_spin8_1804(struct PtObj *obj)
{
    struct PtLocal l;
    struct PtRecord *rec;
    s32 zoom;
    s32 speed;
    s32 angle;
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
    s32 x2;
    s32 y3;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    speed = *words++;
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
        x2 = x1;
        x2 = x2 * (gCosineTable[angle & 0xFF] / 0x40);
        x2 = (x2 - (gSineTable[angle & 0xFF] / 0x40) * y2) / 0x100;
        l.x = x2;
        y3 = ((y2 * (gCosineTable[angle & 0xFF] / 0x40)) + ((gSineTable[angle & 0xFF] / 0x40) * x1)) / 0x100;
        l.y = y3 - 0x1000;

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
    zoom -= speed;
    speed += 0x20;
    angle += 3;
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words = angle;
}

void obj_zoom_tilt_rise8_1a5c(struct PtObj *obj)
{
    struct PtLocal l;
    s16 *cs;
    s16 *sn;
    s16 *cs2;
    s16 *sn2;
    struct PtRecord *rec;
    s32 zoom;
    s32 speed;
    s32 angle;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ra;
    s32 rb;
    s32 ty;
    s32 tz;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    speed = *words++;
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
        x = *words++;
        l.x = x;
        l.y = *words++;
        l.z = *words++;
        sx = (x * zoom) / 0x100;
        l.x = sx;
        sy = (l.y * zoom) / 0x100;
        l.y = sy;
        sz = (l.z * zoom) / 0x100;
        l.z = sz;
        ty = sz;
        cs = &gCosineTable[angle & 0xFF];
        ty = ty * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        ra = (ty - (*sn / 0x40) * sx) / 0x100;
        l.z = ra;
        rb = ((sx * (*cs / 0x40)) + ((*sn / 0x40) * sz)) / 0x100;
        l.x = rb;
        t = sy;
        cs2 = &gCosineTable[12];
        t = t * (*cs2 / 0x40);
        sn2 = &gSineTable[12];
        ty = (t - (*sn2 / 0x40) * ra) / 0x100;
        l.y = ty;
        tz = ((ra * (*cs2 / 0x40)) + ((*sn2 / 0x40) * sy)) / 0x100;
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
    zoom -= speed;
    speed += 0x20;
    angle += 3;
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words = angle;
}

void obj_jump_zoom_turn_points_1cb8(struct PtObj *obj)
{
    struct PtLocal l;
    s16 *cs;
    s16 *sn;
    s16 *cs2;
    s16 *sn2;
    struct PtRecord *rec;
    s32 zoom;
    s32 speed;
    s32 angle;
    s32 drift;
    s32 accel;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ra;
    s32 rb;
    s32 ty;
    s32 tz;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    speed = *words++;
    angle = *words++;
    drift = *words++;
    accel = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 8; i++) {
        x = *words++;
        l.x = x;
        l.y = *words++;
        l.z = *words++;
        sx = (x * zoom) / 0x100;
        l.x = sx;
        sy = (l.y * zoom) / 0x100;
        l.y = sy;
        sz = (l.z * zoom) / 0x100;
        l.z = sz;
        ty = sz;
        cs = &gCosineTable[angle & 0xFF];
        ty = ty * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        ra = (ty - (*sn / 0x40) * sx) / 0x100;
        l.z = ra;
        rb = ((sx * (*cs / 0x40)) + ((*sn / 0x40) * sz)) / 0x100;
        l.x = rb;
        t = sy;
        cs2 = &gCosineTable[12];
        t = t * (*cs2 / 0x40);
        sn2 = &gSineTable[12];
        ty = (t - (*sn2 / 0x40) * ra) / 0x100;
        l.y = ty;
        tz = ((ra * (*cs2 / 0x40)) + ((*sn2 / 0x40) * sy)) / 0x100;
        l.z = tz;
        l.y = ty + drift;

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
    zoom += speed;
    speed -= 0x14;
    angle += 1;
    drift += accel;
    accel += 0x60;
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words++ = angle;
    *words++ = drift;
    *words = accel;
}

ASM_FUNC("asm/code_08181620/obj_rise_turn_points_1f4c.s", void obj_rise_turn_points_1f4c(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2180.s", void obj_grow_spin_points_2180(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2428.s", void obj_grow_spin_points_2428(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_26d0.s", void obj_grow_spin_points_26d0(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2980.s", void obj_grow_spin_points_2980(void))

ASM_FUNC("asm/code_08181620/obj_spring_turn_points_2c50.s", void obj_spring_turn_points_2c50(void))
