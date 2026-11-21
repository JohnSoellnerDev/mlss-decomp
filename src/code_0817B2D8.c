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

NONMATCH("asm/code_0817B2D8/obj_model8_zoom_vel9_b2d8.s", void obj_model8_zoom_vel9_b2d8(struct Point1Obj *obj))
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
    s32 y1;
    s32 z1;
    s32 y2;
    s32 x2;
    s32 z2;
    s32 t;
    s32 base;
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
    for (i = 0; i < 8; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        l.x = (vx * zoom) / 0x100;
        y1 = (l.y * zoom) / 0x100;
        l.y = y1;
        z1 = (l.z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        y2 = (t - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        z2 = (z1 * (gCosineTable[64] / 0x40) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        l.z = z2;
        t = z2;
        l.z = (t * (gCosineTable[96] / 0x40) - (gSineTable[96] / 0x40) * l.x) / 0x100;
        x2 = ((l.x * (gCosineTable[96] / 0x40)) + (gSineTable[96] / 0x40) * z2) / 0x100;
        l.x = x2;

        if (obj->flags & 0x80) {
            base = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = base - pos;
        } else {
            base = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = base + pos;
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
    vel = (vel * 9) / 10;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH

NONMATCH("asm/code_0817B2D8/obj_model8_zoom_vel9_b4f0.s", void obj_model8_zoom_vel9_b4f0(struct Point1Obj *obj))
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
    s32 y1;
    s32 z1;
    s32 y2;
    s32 x2;
    s32 z2;
    s32 t;
    s32 base;
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
    for (i = 0; i < 8; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        l.x = (vx * zoom) / 0x100;
        y1 = (l.y * zoom) / 0x100;
        l.y = y1;
        z1 = (l.z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        y2 = (t - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        z2 = (z1 * (gCosineTable[64] / 0x40) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        l.z = z2;
        t = z2;
        l.z = (t * (gCosineTable[96] / 0x40) - (gSineTable[96] / 0x40) * l.x) / 0x100;
        x2 = ((l.x * (gCosineTable[96] / 0x40)) + (gSineTable[96] / 0x40) * z2) / 0x100;
        l.x = x2;

        if (obj->flags & 0x80) {
            base = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = base - pos;
        } else {
            base = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = base + pos;
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
    vel = (vel * 9) / 10;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH

NONMATCH("asm/code_0817B2D8/obj_model8_zoom_vel9_b708.s", void obj_model8_zoom_vel9_b708(struct Point1Obj *obj))
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
    s32 y1;
    s32 z1;
    s32 y2;
    s32 x2;
    s32 z2;
    s32 t;
    s32 base;
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
    for (i = 0; i < 8; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        l.x = (vx * zoom) / 0x100;
        y1 = (l.y * zoom) / 0x100;
        l.y = y1;
        z1 = (l.z * zoom) / 0x100;
        l.z = z1;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        y2 = (t - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        z2 = (z1 * (gCosineTable[64] / 0x40) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        l.z = z2;
        t = z2;
        l.z = (t * (gCosineTable[96] / 0x40) - (gSineTable[96] / 0x40) * l.x) / 0x100;
        x2 = ((l.x * (gCosineTable[96] / 0x40)) + (gSineTable[96] / 0x40) * z2) / 0x100;
        l.x = x2;

        if (obj->flags & 0x80) {
            base = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = base - pos;
        } else {
            base = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = base + pos;
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
    vel = (vel * 9) / 10;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH

/*
 * Effect update: draws 2 pieces whose offsets are scaled by the zoom (8.8) and tilted by a fixed
 * 12/256 turn around the x axis (y/z plane). The zoom grows by its speed every frame and the speed
 * drops to 4/5; state is 2 words in obj->data (zoom, zoom speed), then 3 words per piece.
 */
void obj_tilt_zoom2_b920(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 tiltY;
    s32 tmp;
    s32 ofs;

    state = obj->data;
    zoom = *state++;
    zoomVel = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 2; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (pt.y * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (pt.z * zoom) / 0x100;
        pt.z = scaledZ;

        /* tilt: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[12] / 0x40);
        tiltY = (tmp - (gSineTable[12] / 0x40) * scaledZ) / 0x100;
        pt.y = tiltY;
        pt.z = ((scaledZ * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * scaledY)) / 0x100;

        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = scaledX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = scaledX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp + ofs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
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
    zoom += zoomVel;
    zoomVel = (zoomVel * 4) / 5;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

NONMATCH("asm/code_0817B2D8/obj_tilt_zoom2_oam_c0_baf4.s", void obj_tilt_zoom2_oam_c0_baf4(struct Point1Obj *obj))
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
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;

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
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH

/* Effect update: draws 2 pieces that spread out (scale grows by a speed that slows to 4/5 each frame),
   tipped 12/256 of a turn around the x axis; state is 2 words in obj->data (scale, scale speed),
   followed by each piece's offset (x, y, z). */
void obj_tilt_zoom2_bcd4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 scale;
    s32 scaleSpeed;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 2; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;
        /* tip 12/256 of a turn around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (gSineTable[12] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    scaleSpeed = (scaleSpeed * 4) / 5;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/*
 * Effect update (the same as obj_tilt_zoom2_b920): draws 2 pieces whose offsets are scaled by the
 * zoom (8.8) and tilted by a fixed 12/256 turn around the x axis (y/z plane). The zoom grows by its
 * speed every frame and the speed drops to 4/5; state is 2 words in obj->data (zoom, zoom speed),
 * then 3 words per piece.
 */
void obj_tilt_zoom2_bea8(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 tiltY;
    s32 tmp;
    s32 ofs;

    state = obj->data;
    zoom = *state++;
    zoomVel = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 2; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (pt.y * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (pt.z * zoom) / 0x100;
        pt.z = scaledZ;

        /* tilt: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[12] / 0x40);
        tiltY = (tmp - (gSineTable[12] / 0x40) * scaledZ) / 0x100;
        pt.y = tiltY;
        pt.z = ((scaledZ * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * scaledY)) / 0x100;

        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = scaledX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = scaledX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp + ofs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
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
    zoom += zoomVel;
    zoomVel = (zoomVel * 4) / 5;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

/*
 * Draws an 8-part sprite model that zooms in: obj->data holds the zoom (8.8 fixed point),
 * the zoom velocity and the 8 parts' (x, y, z) offsets. Each offset is scaled by the zoom,
 * tilted about the X axis by angle 64 of the sine table and moved up 12 and back 1 pixel,
 * mirrored in X if obj->flags & 0x80, then placed as OAM at the object's screen position.
 * Afterwards the zoom grows by its velocity, which slows to 7/8 each frame.
 */
void obj_model8_zoom_c07c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoom_vel;
    u32 *data;
    u32 oam_count;
    u32 cursor_reset;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 part_x;
    s32 scaled_x;
    s32 scaled_y;
    s32 scaled_z;
    s32 tilted_y;
    s32 tilted_z;
    s32 t;
    s32 ofs_x;

    data = obj->data;
    zoom = *data++;
    zoom_vel = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    oam_count = block_flag_byte(rec);
    cursor_reset = 0;
    oam_list_rows_b(pt.model, oam_count);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 8; i++) {
        part_x = *data++;
        pt.x = part_x;
        pt.y = *data++;
        pt.z = *data++;
        scaled_x = (part_x * zoom) / 0x100;
        pt.x = scaled_x;
        scaled_y = (pt.y * zoom) / 0x100;
        pt.y = scaled_y;
        scaled_z = (pt.z * zoom) / 0x100;
        pt.z = scaled_z;
        t = scaled_y;
        t = t * (gCosineTable[64] / 0x40);
        tilted_y = (t - (gSineTable[64] / 0x40) * scaled_z) / 0x100;
        pt.y = tilted_y;
        tilted_z = ((scaled_z * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * scaled_y)) / 0x100;
        pt.y = tilted_y - 0xC00;
        pt.z = tilted_z - 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            ofs_x = scaled_x;
            if (ofs_x < 0) {
                ofs_x += 0xFF;
            }
            ofs_x >>= 8;
            pt.x = t - ofs_x;
        } else {
            t = obj->posX;
            ofs_x = scaled_x;
            if (ofs_x < 0) {
                ofs_x += 0xFF;
            }
            ofs_x >>= 8;
            pt.x = t + ofs_x;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, oam_count, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursor_reset) {
            pt.model = gModelCtx->model;
            cursor_reset = 1;
        }
    }
    if (cursor_reset) {
        gModelCtx->model = pt.model;
    }
    zoom += zoom_vel;
    zoom_vel = (zoom_vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    data = obj->data;
    *data++ = zoom;
    *data = zoom_vel;
}

/* Draws the object's sprite model at 8 points: obj->data holds the zoom (0x100 = 1.0), its
 * velocity, then 8 (x, y, z) offsets. Each offset is scaled by the zoom, turned by sine-table
 * angle 64 about the X axis, shifted by (0, -0xC00, -0x100), mirrored in x when flags bit 7 is set,
 * and placed relative to the object. The first copy reuses the model's own OAM rows, the others go
 * to the shared list at gModelCtx->model. Then the zoom grows by its velocity, which decays to 7/8. */
void obj_model8_zoom_c274(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoom_vel;
    u32 *params;
    u32 row_count;
    u32 on_shared_list;
    s32 i;
    s32 *pt_y;
    s32 *pt_z;
    u32 *cursor;
    s32 ofs_x;
    s32 scaled_x;
    s32 scaled_y;
    s32 scaled_z;
    s32 turned_y;
    s32 turned_z;
    s32 tmp;
    s32 whole_x;

    params = obj->data;
    zoom = *params++;
    zoom_vel = *params++;
    rec = obj->rec;
    pt.model = rec->model;
    row_count = block_flag_byte(rec);
    on_shared_list = 0;
    oam_list_rows_b(pt.model, row_count);
    pt_y = &pt.y;
    pt_z = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 8; i++) {
        ofs_x = *params++;
        pt.x = ofs_x;
        pt.y = *params++;
        pt.z = *params++;
        scaled_x = (ofs_x * zoom) / 0x100;
        pt.x = scaled_x;
        scaled_y = (pt.y * zoom) / 0x100;
        pt.y = scaled_y;
        scaled_z = (pt.z * zoom) / 0x100;
        pt.z = scaled_z;
        tmp = scaled_y;
        tmp = tmp * (gCosineTable[64] / 0x40);
        turned_y = (tmp - (gSineTable[64] / 0x40) * scaled_z) / 0x100;
        pt.y = turned_y;
        turned_z = ((scaled_z * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * scaled_y)) / 0x100;
        pt.y = turned_y - 0xC00;
        pt.z = turned_z - 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            whole_x = scaled_x;
            if (whole_x < 0) {
                whole_x += 0xFF;
            }
            whole_x >>= 8;
            pt.x = tmp - whole_x;
        } else {
            tmp = obj->posX;
            whole_x = scaled_x;
            if (whole_x < 0) {
                whole_x += 0xFF;
            }
            whole_x >>= 8;
            pt.x = tmp + whole_x;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, pt_y, pt_z, 0);
        oam_place_points(cursor, row_count, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!on_shared_list) {
            pt.model = gModelCtx->model;
            on_shared_list = 1;
        }
    }
    if (on_shared_list) {
        gModelCtx->model = pt.model;
    }
    zoom += zoom_vel;
    zoom_vel = (zoom_vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    params = obj->data;
    *params++ = zoom;
    *params = zoom_vel;
}

// Eight-point object that zooms out: each point is zoomed (8.8 fixed point), tilted by the fixed
// table angle 64, raised by 12 and pushed back by 1, then drawn at the object (mirrored when
// flags bit 7 is set). Every frame the zoom grows by a speed that decays to 7/8; the record's
// size stays at 0x200.
void obj_model8_zoom_c46c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    u32 *words;
    u32 oamFlags;
    u32 modelSaved;
    s32 i;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tiltY;
    s32 tiltZ;
    s32 t;
    s32 pos;

    // State: two words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    zoomVel = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 8; i++) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        pt.y = *words++;
        pt.z = *words++;
        // Zoom it.
        zx = (px * zoom) / 0x100;
        pt.x = zx;
        zy = (pt.y * zoom) / 0x100;
        pt.y = zy;
        zz = (pt.z * zoom) / 0x100;
        pt.z = zz;
        // Tilt it.
        t = zy;
        t = t * (gCosineTable[64] / 0x40);
        tiltY = (t - (gSineTable[64] / 0x40) * zz) / 0x100;
        pt.y = tiltY;
        tiltZ = ((zz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * zy)) / 0x100;
        pt.y = tiltY - 0xC00;
        pt.z = tiltZ - 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += zoomVel;
    zoomVel = (zoomVel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words = zoomVel;
}

// Eight-point object whose zoom grows ever more slowly: each point is zoomed (8.8 fixed point), tilted
// by the fixed table angle 64, raised by 0xC and pulled forward by 1 (whole units) and drawn at the
// object (mirrored when flags bit 7 is set). Every frame the zoom speed decays to 7/8; the record's
// size stays 0x190.
void obj_model8_zoom_oam_190_c664(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    u32 *words;
    u32 oamFlags;
    u32 modelSaved;
    s32 i;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tiltY;
    s32 tiltZ;
    s32 t;
    s32 pos;

    // State: two words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    zoomVel = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 8; i++) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        pt.y = *words++;
        pt.z = *words++;
        // Zoom it.
        zx = (px * zoom) / 0x100;
        pt.x = zx;
        zy = (pt.y * zoom) / 0x100;
        pt.y = zy;
        zz = (pt.z * zoom) / 0x100;
        pt.z = zz;
        // Tilt it.
        t = zy;
        t = t * (gCosineTable[64] / 0x40);
        tiltY = (t - (gSineTable[64] / 0x40) * zz) / 0x100;
        pt.y = tiltY;
        tiltZ = ((zz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * zy)) / 0x100;
        pt.y = tiltY - 0xC00;
        pt.z = tiltZ - 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += zoomVel;
    zoomVel = (zoomVel * 7) / 8; // friction
    rec->unk4 = 0x190;
    rec->unk6 = 0x190;
    words = obj->data;
    *words++ = zoom;
    *words = zoomVel;
}

// Same as obj_model8_zoom_c46c with the record's size held at 0x180: eight points zoomed
// (8.8 fixed point), tilted by the fixed table angle 64, raised by 12 and pushed back by 1, then
// drawn at the object (mirrored when flags bit 7 is set). The zoom speed decays to 7/8 per frame.
void obj_model8_zoom_oam_180_c85c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    u32 *words;
    u32 oamFlags;
    u32 modelSaved;
    s32 i;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tiltY;
    s32 tiltZ;
    s32 t;
    s32 pos;

    // State: two words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    zoomVel = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 8; i++) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        pt.y = *words++;
        pt.z = *words++;
        // Zoom it.
        zx = (px * zoom) / 0x100;
        pt.x = zx;
        zy = (pt.y * zoom) / 0x100;
        pt.y = zy;
        zz = (pt.z * zoom) / 0x100;
        pt.z = zz;
        // Tilt it.
        t = zy;
        t = t * (gCosineTable[64] / 0x40);
        tiltY = (t - (gSineTable[64] / 0x40) * zz) / 0x100;
        pt.y = tiltY;
        tiltZ = ((zz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * zy)) / 0x100;
        pt.y = tiltY - 0xC00;
        pt.z = tiltZ - 0x100;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += zoomVel;
    zoomVel = (zoomVel * 7) / 8;
    rec->unk4 = 0x180;
    rec->unk6 = 0x180;
    words = obj->data;
    *words++ = zoom;
    *words = zoomVel;
}

/*
 * Effect update: draws 1 piece at its fixed offset from obj->data (3 words: x, y, z in 8.8),
 * moved up 12 and back 1 pixel and mirrored in x if obj->flags bit 7 is set, then sets the
 * record's unk4/unk6 (both 0x180).
 */
void obj_point_fixed_ca54(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offZ;
    s32 base;
    s32 ofs;

    state = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        offZ = *state++;
        pt.y -= 0xC00;
        pt.z = offZ - 0x100;

        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
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
    rec->unk4 = 0x180;
    rec->unk6 = 0x180;
}
