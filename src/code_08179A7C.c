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

NONMATCH("asm/code_08179A7C/zoom_turn_a2f8.s", void zoom_turn_a2f8(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 vel;
    s32 off;
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
        pos = z2;
        pos = pos * (gCosineTable[82] / 0x40);
        z3 = (pos - (gSineTable[82] / 0x40) * l.x) / 0x100;
        l.z = z3;
        x2 = (l.x * (gCosineTable[82] / 0x40) + (gSineTable[82] / 0x40) * z2) / 0x100;
        x2 += off;
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
    vel = (vel * 2) / 3;
    off -= 0x80;
    rec->unk4 = 0x160;
    rec->unk6 = 0x160;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = off;
}
END_NONMATCH

NONMATCH("asm/code_08179A7C/zoom_turn_a534.s", void zoom_turn_a534(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 vel;
    s32 off;
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
        pos = z2;
        pos = pos * (gCosineTable[82] / 0x40);
        z3 = (pos - (gSineTable[82] / 0x40) * l.x) / 0x100;
        l.z = z3;
        x2 = (l.x * (gCosineTable[82] / 0x40) + (gSineTable[82] / 0x40) * z2) / 0x100;
        x2 += off;
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
    vel = (vel * 2) / 3;
    off -= 0x80;
    rec->unk4 = 0x140;
    rec->unk6 = 0x140;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = off;
}
END_NONMATCH

NONMATCH("asm/code_08179A7C/obj_model8_zoom_xslide_a770.s", void obj_model8_zoom_xslide_a770(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 vel;
    s32 off;
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
        pos = z2;
        pos = pos * (gCosineTable[82] / 0x40);
        z3 = (pos - (gSineTable[82] / 0x40) * l.x) / 0x100;
        l.z = z3;
        x2 = (l.x * (gCosineTable[82] / 0x40) + (gSineTable[82] / 0x40) * z2) / 0x100;
        x2 += off;
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
    vel = (vel * 2) / 3;
    off -= 0x80;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = off;
}
END_NONMATCH

/* One-point object that doesn't move: draws its single piece at the 24.8 fixed-point offset in
 * obj->data (3 words) from the object's position, mirrored in x when flags bit 7 is set, and sets
 * the record's size (unk4, unk6) to 0x160. */
void obj_point_static_oam_160_a99c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 tmp;
    s32 ofs;

    words = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *words++;
        pt.x = offX;
        pt.y = *words++;
        pt.z = *words++;
        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp + ofs;
        }
        tmp = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = tmp + ofs;
        tmp = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = tmp + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
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
    s32 base;

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
        base = z2;
        base = base * (gCosineTable[82] / 0x40);
        z3 = (base - (gSineTable[82] / 0x40) * l.x) / 0x100;
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
    s32 base;

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
        base = z2;
        base = base * (gCosineTable[82] / 0x40);
        z3 = (base - (gSineTable[82] / 0x40) * l.x) / 0x100;
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

/*
 * Effect update: draws 5 pieces whose offsets are scaled by the zoom (8.8), tilted by a fixed
 * 12/256 turn around the x axis (y/z plane) and shifted by a common offset. The zoom grows by 4.0
 * every frame while the offset falls with gravity 0x60; state is 4 words in obj->data (zoom,
 * offset x, offset y, offset y speed), then 3 words per piece.
 */
void obj_model5_zoom_drop_af10(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 zoom;
    s32 offsetX;
    s32 offsetY;
    s32 offsetYVel;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 piecesLeft;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 offZ;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 tiltY;
    s32 tiltZ;
    s32 shiftedX;
    s32 tmp;
    s32 ofs;

    state = obj->data;
    zoom = *state++;
    offsetX = *state++;
    offsetY = *state++;
    offsetYVel = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    piecesLeft = 4;
    for (;;) {
        offX = *state++;
        pt.x = offX;
        offY = *state++;
        pt.y = offY;
        offZ = *state++;
        pt.z = offZ;

        /* scale the offset by zoom / 256 */
        scaledX = offX * zoom / 0x100;
        pt.x = scaledX;
        scaledY = offY * zoom / 0x100;
        pt.y = scaledY;
        scaledZ = offZ * zoom / 0x100;
        pt.z = scaledZ;

        /* tilt: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[12] / 0x40);
        tiltY = (tmp - (gSineTable[12] / 0x40) * scaledZ) / 0x100;
        pt.y = tiltY;
        tiltZ = (scaledZ * (gCosineTable[12] / 0x40) + (gSineTable[12] / 0x40) * scaledY) / 0x100;
        pt.z = tiltZ;

        shiftedX = offsetX + scaledX;
        pt.x = shiftedX;
        pt.y = tiltY + offsetY;

        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = shiftedX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = shiftedX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp + ofs;
        }
        tmp = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = tmp + ofs;
        tmp = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = tmp + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
        piecesLeft--;
        if (piecesLeft < 0) {
            break;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += 0x400;
    offsetY += offsetYVel;
    offsetYVel += 0x60;
    state = obj->data;
    *state++ = zoom;
    *state++ = offsetX;
    *state++ = offsetY;
    *state = offsetYVel;
}

// Five-point object that grows and falls: each point is zoomed (8.8 fixed point), tilted by the
// fixed table angle 12, shifted by the x offset and the fall height and drawn at the object
// (mirrored when flags bit 7 is set). Every frame the zoom grows by 4.0, gravity speeds up the
// fall, and the record's size (0xA0 - shrink) shrinks ever faster.
void obj_model5_zoom_fall_oam_b0e0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    s32 zoom;
    s32 shrink;
    s32 shrink_speed;
    s32 offset_x;
    s32 fall_y;
    s32 fall_speed;
    struct ObjRecord *rec;
    u32 oam_flags;
    u32 model_saved;
    s32 points_left;
    s32 *p_y;
    s32 *p_z;
    u32 *p_model;
    s32 px;
    s32 py;
    s32 pz;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tilt_y;
    s32 tilt_z;
    s32 shifted_x;
    s32 t;
    s32 pos;

    // State: six words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrink_speed = *words++;
    offset_x = *words++;
    fall_y = *words++;
    fall_speed = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oam_flags = block_flag_byte(rec);
    model_saved = 0;
    oam_list_rows_b(pt.model, oam_flags);
    p_y = &pt.y;
    p_z = &pt.z;
    p_model = &pt.model;
    points_left = 4;
    for (;;) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        py = *words++;
        pt.y = py;
        pz = *words++;
        pt.z = pz;
        // Zoom it.
        zx = px * zoom / 0x100;
        pt.x = zx;
        zy = py * zoom / 0x100;
        pt.y = zy;
        zz = pz * zoom / 0x100;
        pt.z = zz;
        // Tilt it.
        t = zy;
        t = t * (gCosineTable[12] / 0x40);
        tilt_y = (t - (gSineTable[12] / 0x40) * zz) / 0x100;
        pt.y = tilt_y;
        tilt_z = (zz * (gCosineTable[12] / 0x40) + (gSineTable[12] / 0x40) * zy) / 0x100;
        pt.z = tilt_z;
        // Shift it and let it fall.
        shifted_x = offset_x + zx;
        pt.x = shifted_x;
        pt.y = tilt_y + fall_y;
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = shifted_x;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = shifted_x;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        t = obj->posY;
        pos = pt.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        pt.y = t + pos;
        t = obj->posZ;
        pos = pt.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        pt.z = t + pos;
        offset_words_camera(&pt.x, p_y, p_z, 0);
        oam_place_points(p_model, oam_flags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!model_saved) {
            pt.model = gModelCtx->model;
            model_saved = 1;
        }
        points_left--;
        if (points_left < 0) {
            break;
        }
    }
    if (model_saved) {
        gModelCtx->model = pt.model;
    }
    zoom += 0x400;
    fall_y += fall_speed;
    fall_speed += 0x60; // gravity
    shrink += shrink_speed;
    shrink_speed += 1;
    rec->unk4 = 0xA0 - shrink;
    rec->unk6 = 0xA0 - shrink;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrink_speed;
    *words++ = offset_x;
    *words++ = fall_y;
    *words = fall_speed;
}
