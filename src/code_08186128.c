#include "global.h"

struct PtRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[4];
    u16 unkC;
    u8 filler10[0x34 - 0x10];
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


NONMATCH("asm/code_08186128/obj_tilt_zoom8_6128.s", void obj_tilt_zoom8_6128(struct PtObj *obj))
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 zoom;
    s32 step;
    s32 *pt;
    struct PtRecord *rec;
    u32 flags;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 cy;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    step = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    flags = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, flags);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 7;
    do {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        sx = (x * zoom) / 0x100;
        l.x = sx;
        sy = (y * zoom) / 0x100;
        l.y = sy;
        sz = (z * zoom) / 0x100;
        l.z = sz;
        cy = sy * (gCosineTable[0x40] / 0x40);
        l.y = (cy - (gSineTable[0x40] / 0x40) * sz) / 0x100;
        l.z = (sz * (gCosineTable[0x40] / 0x40) + sy * (gSineTable[0x40] / 0x40)) / 0x100;
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
        oam_place_points(pslot, flags, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
        left--;
    } while (left >= 0);
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += step;
    step = (step * 8 - step) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt = step;
}
END_NONMATCH

ASM_FUNC("asm/code_08186128/obj_turn_zoom4_6310.s", void obj_turn_zoom4_6310(void))

NONMATCH("asm/code_08186128/obj_tilt_zoom8_6560.s", void obj_tilt_zoom8_6560(struct PtObj *obj))
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 zoom;
    s32 step;
    s32 *pt;
    struct PtRecord *rec;
    u32 flags;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 cy;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    step = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    flags = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, flags);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 7;
    do {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        sx = (x * zoom) / 0x100;
        l.x = sx;
        sy = (y * zoom) / 0x100;
        l.y = sy;
        sz = (z * zoom) / 0x100;
        l.z = sz;
        cy = sy * (gCosineTable[0x40] / 0x40);
        l.y = (cy - (gSineTable[0x40] / 0x40) * sz) / 0x100;
        l.z = (sz * (gCosineTable[0x40] / 0x40) + sy * (gSineTable[0x40] / 0x40)) / 0x100;
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
        oam_place_points(pslot, flags, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
        left--;
    } while (left >= 0);
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += step;
    step = (step * 8 - step) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt = step;
}
END_NONMATCH

ASM_FUNC("asm/code_08186128/obj_turn_zoom4_6748.s", void obj_turn_zoom4_6748(void))

ASM_FUNC("asm/code_08186128/obj_bounce_reverse_point.s", void obj_bounce_reverse_point(void))


void obj_swing_point(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 xOff;
    s32 yOff;
    s32 vx;
    s32 vy;
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
    xOff = *pt++;
    yOff = *pt++;
    vx = *pt++;
    vy = *pt++;
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
        l.x = x + xOff;
        l.y = y + yOff;
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
    t = xOff;
    if (t < 0) {
        t += 0x3F;
    }
    t >>= 6;
    vx -= t;
    xOff += vx;
    t = xOff;
    if (t < 0) {
        t += 0xFF;
    }
    t >>= 8;
    vy -= t;
    yOff += vy;
    pt = (s32 *)obj->data;
    *pt++ = xOff;
    *pt++ = yOff;
    *pt++ = vx;
    *pt = vy;
}

ASM_FUNC("asm/code_08186128/obj_spin_hop_rot90_model5.s", void obj_spin_hop_rot90_model5(void))

ASM_FUNC("asm/code_08186128/obj_spin_hop_rot45_model5.s", void obj_spin_hop_rot45_model5(void))

ASM_FUNC("asm/code_08186128/obj_spin_swap_model5.s", void obj_spin_swap_model5(void))

ASM_FUNC("asm/code_08186128/obj_spin_tilt67_model5.s", void obj_spin_tilt67_model5(void))

void obj_bounce_turn_point(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 xOff;
    s32 xVel;
    s32 angle;
    struct PtRecord *rec;
    u32 saved;
    s32 yOff;
    s32 yVel;
    s32 stage;
    u32 h;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 xs;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    xOff = *pt++;
    xVel = *pt++;
    yOff = *pt++;
    yVel = *pt++;
    stage = *pt++;
    angle = *pt++;
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
        xs = x + xOff;
        l.x = xs;
        l.y = y + yOff;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = xs;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = xs;
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
    xOff += xVel;
    yOff += yVel;
    yVel += 0x40;
    if (yOff > 0) {
        xVel = xVel / 2;
        yOff = 0;
        stage++;
        if (stage > 0) {
            stage = 0;
        }
        yVel = stage << 8;
    }
    angle += 0x10;
    if (obj->flags & 0x80) {
        rec->unkC = -angle << 8;
    } else {
        rec->unkC = angle << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = xOff;
    *pt++ = xVel;
    *pt++ = yOff;
    *pt++ = yVel;
    *pt++ = stage;
    *pt = angle;
}
