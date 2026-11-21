#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4; /* sprite x scale, 8.8 (0x100 = 1:1) */
    u16 unk6; /* sprite y scale, 8.8 (0xFF00 = flipped) */
    u8 filler8[4];
    u16 unkC; /* sprite rotation angle, 8.8 in 1/256 turns */
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

/*
 * Effect update: draws 1 piece that drifts at a constant speed. Its x and y offsets grow by their
 * speeds every frame; flags bit 7 mirrors it in x. The record is left unchanged. State is 4 words
 * in obj->data (x offset, x speed, y offset, y speed), then 3 words (x, y, z) for the piece.
 */
void obj_drift_point_9190(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 velX;
    s32 velY;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 pieceX;
    s32 base;
    s32 ofs;
    s32 offX;
    s32 offY;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        pieceX = *state++;
        pt.x = pieceX;
        pt.y = *state++;
        pt.z = *state++;
        pieceX += offX;
        pt.x = pieceX;
        pt.y += offY;
        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = pieceX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = pieceX;
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
    offX += velX;
    offY += velY;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state = velY;
}

/*
 * Effect update: draws 2 pieces whose offsets are scaled by the zoom (8.8) and tilted by a fixed
 * 12/256 turn around the x axis (y/z plane). The zoom grows by its speed every frame and the speed
 * drops to 7/8; state is 2 words in obj->data (zoom, zoom speed), then 3 words per piece.
 */
void obj_zoom_tilt_model2_92b4(struct Point1Obj *obj)
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
    zoomVel = (zoomVel * 7) / 8;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

/* Effect update: draws 2 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tilted by the fixed angle 12/256 around the x axis; state is 2 words in obj->data (scale, scale speed). */
void obj_zoom_tilt_model2_948c(struct Point1Obj *obj)
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
    /* each piece: its offset (x, y, z) follows the state words */
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
        /* tilt around the x axis */
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
    scaleSpeed = (scaleSpeed * 7) / 8;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

// Effect update for one point that drifts and grows. Each frame its x/y offset (8.8) moves by its
// speed, and the sprite's x and y scale grow by 0x10 (1/16). State in obj->data: offset x, speed x,
// offset y, speed y, scale, then the point's base offset (x, y, z).
void obj_drift_grow_9664(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 offX;
    s32 offY;
    s32 scale;
    struct ObjRecord *rec;
    s32 speedX;
    s32 speedY;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 x;
    s32 base;
    s32 ofs;

    state = obj->data;
    offX = *state++;
    speedX = *state++;
    offY = *state++;
    speedY = *state++;
    scale = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        x = *state++;
        pt.x = x;
        pt.y = *state++;
        pt.z = *state++;
        x += offX;
        pt.x = x;
        pt.y += offY;
        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = x;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = x;
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
    offX += speedX;
    offY += speedY;
    scale += 0x10;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = offX;
    *state++ = speedX;
    *state++ = offY;
    *state++ = speedY;
    *state = scale;
}

// One point that drifts and spins: its x/y offset grows by its speed every frame and the sprite
// turns 4/256 of a turn per frame (the other way when mirrored by flags bit 7). After 10 frames the
// sprite's scale shrinks by 0x10 per frame. State in obj->data: offX, velX, offY, velY, angle,
// scale, frames, then the point's x, y, z.
void obj_drift_spin_late_shrink_97a4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 offX;
    s32 velX;
    s32 offY;
    s32 velY;
    s32 angle;
    s32 scale;
    s32 frames;
    struct ObjRecord *rec;
    u32 rows;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    angle = *state++;
    scale = *state++;
    frames = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += offY;
        /* 8.8 offsets to whole pixels around the object; bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base + whole;
        }
        base = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = base + whole;
        base = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = base + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rows, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
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
        scale -= 0x10;
    }
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state++ = velY;
    *state++ = angle;
    *state++ = scale;
    *state = frames;
}

// One point whose sprite grows and spins: the scale (8.8) grows by its step every frame up to 0x100
// (1:1) and the sprite turns 3/256 of a turn per frame (the other way when mirrored by flags bit 7).
// After 52 frames the step becomes -0x10 and the sprite shrinks. State in obj->data: scale,
// scaleStep, frames, angle, then the point's x, y, z.
void obj_grow_spin_shrink_9924(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 scale;
    s32 scaleStep;
    s32 frames;
    struct ObjRecord *rec;
    u32 rows;
    s32 angle;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    scale = *state++;
    scaleStep = *state++;
    frames = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* 8.8 offsets to whole pixels around the object; bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base + whole;
        }
        base = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = base + whole;
        base = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = base + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rows, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleStep;
    if (scale > 0x100) {
        scale = 0x100;
    }
    angle += 3;
    rec->unk4 = scale;
    rec->unk6 = scale;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    frames++;
    if (frames > 0x34) {
        scaleStep = -0x10;
    }
    state = obj->data;
    *state++ = scale;
    *state++ = scaleStep;
    *state++ = frames;
    *state = angle;
}

/*
 * Effect update: six sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a fixed velocity, rotated by a yaw that turns 5 steps per
 * frame and a fixed pitch (angle 64), then drawn around the object (mirrored
 * in X when flag 0x80 is set). The record's size fields (unk4/unk6) grow by
 * 10 every frame.
 */
void obj_zoom_spin6_9a84(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 zoom;
    s32 zoomVel;
    s32 yaw;
    s32 size;
    struct ObjRecord *rec;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s16 *cosYaw;
    s16 *sinYaw;
    s32 offX;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 yawX;
    s32 yawZ;
    s32 tmp;
    s32 ofs;

    state = obj->data;
    zoom = *state++;
    zoomVel = *state++;
    yaw = *state++;
    size = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 6; i++) {
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

        /* yaw: rotate in the X/Z plane */
        yawZ = scaledZ;
        cosYaw = &gCosineTable[yaw & 0xFF];
        yawZ = yawZ * (*cosYaw / 0x40);
        sinYaw = &gSineTable[yaw & 0xFF];
        yawZ = (yawZ - (*sinYaw / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosYaw / 0x40) + (*sinYaw / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[64] / 0x40);
        pt.y = (tmp - (gSineTable[64] / 0x40) * yawZ) / 0x100;
        pt.z = ((yawZ * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * scaledY)) / 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = yawX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = yawX;
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
    yaw -= 5;
    size += 10;
    rec->unk4 = size;
    rec->unk6 = size;
    state = obj->data;
    *state++ = zoom;
    *state++ = zoomVel;
    *state++ = yaw;
    *state = size;
}

/* Effect update: draws 6 pieces that spread out at a steady speed, spin around the vertical axis
   (backwards, 3/256 a frame) and are tipped over a quarter turn (64/256) around the x axis, while
   each sprite grows by 8/256 a frame up to double size; state is 4 words in obj->data
   (scale, scale speed, spin, sprite scale). */
void obj_zoom_spin6_9cc8(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    s32 spriteScale;
    struct ObjRecord *rec;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s16 *cosp;
    s16 *sinp;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    spin = *state++;
    spriteScale = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    /* each piece: its offset (x, y, z) follows the state words */
    for (piece = 0; piece < 6; piece++) {
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
        /* spin around the vertical axis */
        rz = sz;
        cosp = &gCosineTable[spin & 0xFF];
        rz = rz * (*cosp / 0x40);
        sinp = &gSineTable[spin & 0xFF];
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (gSineTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = rx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = rx;
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
    spin -= 3;
    spriteScale += 8;
    if (spriteScale > 0x200) {
        spriteScale = 0x200;
    }
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = spin;
    *state = spriteScale;
}

ASM_FUNC("asm/code_08189190/obj_spin_zoom5_9f20.s", void obj_spin_zoom5_9f20(void))

/* One point that grows and spins: the record's x/y scale (8.8) grows by 0x10 per frame up to
 * 0x100, and its rotation grows by 0x10 per frame (the other way round when mirrored, flags bit 7).
 * State is 2 words in obj->data (scale, angle), then the point's 3 offset words. */
void obj_grow_spin_point_a1d4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 scale;
    struct ObjRecord *rec;
    u32 rows;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 pos;
    s32 ofs;
    s32 angle;

    state = obj->data;
    scale = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = pos - ofs;
        } else {
            pos = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = pos + ofs;
        }
        pos = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = pos + ofs;
        pos = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = pos + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rows, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += 0x10;
    if (scale > 0x100) {
        scale = 0x100;
    }
    angle += 0x10;
    rec->unk4 = scale;
    rec->unk6 = scale;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    state = obj->data;
    *state++ = scale;
    *state = angle;
}

ASM_FUNC("asm/code_08189190/obj_spin_model5.s", void obj_spin_model5(struct Point1Obj *obj))

/*
 * Effect update: draws 1 piece that falls. Its y offset grows by the y speed, which grows by
 * 0x40 every frame (gravity); its x offset grows by the x speed. Flags bit 7 mirrors it in x, and
 * the record's y scale is set to 0xFF00 (flipped). State is 4 words in obj->data (y offset,
 * y speed, x offset, x speed), then 3 words (x, y, z) for the piece.
 */
void obj_fall_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 velX;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 pieceX;
    s32 base;
    s32 ofs;
    s32 offY;
    s32 velY;
    s32 offX;

    state = obj->data;
    offY = *state++;
    velY = *state++;
    offX = *state++;
    velX = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        pieceX = *state++;
        pt.x = pieceX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += offY;
        pieceX += offX;
        pt.x = pieceX;
        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = pieceX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = pieceX;
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
    offY += velY;
    velY += 0x40;
    offX += velX;
    rec->unk6 = 0xFF00;
    state = obj->data;
    *state++ = offY;
    *state++ = velY;
    *state++ = offX;
    *state = velX;
}


/*
 * Effect update, a copy of obj_fall_point for another effect: draws 1 piece that falls. Its y
 * offset grows by the y speed, which grows by 0x40 every frame (gravity); its x offset grows by
 * the x speed. Flags bit 7 mirrors it in x, and the record's y scale is set to 0xFF00 (flipped).
 * State is 4 words in obj->data (y offset, y speed, x offset, x speed), then 3 words (x, y, z)
 * for the piece.
 */
void obj_fall_point_b(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 velX;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 pieceX;
    s32 base;
    s32 ofs;
    s32 offY;
    s32 velY;
    s32 offX;

    state = obj->data;
    offY = *state++;
    velY = *state++;
    offX = *state++;
    velX = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        pieceX = *state++;
        pt.x = pieceX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += offY;
        pieceX += offX;
        pt.x = pieceX;
        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = pieceX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = pieceX;
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
    offY += velY;
    velY += 0x40;
    offX += velX;
    rec->unk6 = 0xFF00;
    state = obj->data;
    *state++ = offY;
    *state++ = velY;
    *state++ = offX;
    *state = velX;
}


// One point that flies off, falls (gravity 0x40 per frame) and bounces while it spins 16/256 of a
// turn per frame (the other way when mirrored by flags bit 7). On each landing (yOff > 0) its x speed
// halves and it bounces back up at bounceSpeed pixels per frame, which loses 1 per bounce until 0.
// State in obj->data: xOff, xVel, yOff, yVel, bounceSpeed (negative = up), angle, then the offset (x, y, z).
void obj_bounce_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 xOff;
    s32 xVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 rows;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 base;
    s32 whole;
    s32 yOff;
    s32 yVel;
    s32 bounceSpeed;

    state = obj->data;
    xOff = *state++;
    xVel = *state++;
    yOff = *state++;
    yVel = *state++;
    bounceSpeed = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += xOff;
        pt.x = srcX;
        pt.y += yOff;
        /* 8.8 offsets to whole pixels around the object; bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base + whole;
        }
        base = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = base + whole;
        base = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = base + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rows, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    xOff += xVel;
    yOff += yVel;
    yVel += 0x40;
    if (yOff > 0) {
        /* landed: bounce */
        xVel = xVel / 2;
        yOff = 0;
        bounceSpeed++;
        if (bounceSpeed > 0) {
            bounceSpeed = 0;
        }
        yVel = bounceSpeed << 8;
    }
    angle += 0x10;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    state = obj->data;
    *state++ = xOff;
    *state++ = xVel;
    *state++ = yOff;
    *state++ = yVel;
    *state++ = bounceSpeed;
    *state = angle;
}

/*
 * Draws one point of a sprite that grows flattened: obj->data holds the sprite's scale (8.8) and one
 * x/y/z point (24.8 fixed point), placed on screen like its neighbours (mirrored when flipped, offset
 * by the object's position, through the camera). The scale then grows by 0x20 per frame and goes to
 * the record as x scale, with the y scale 0xC0 below it.
 */
void obj_slide_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 pos;
    s32 offs;
    s32 scale;

    data = obj->data;
    scale = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = pos - offs;
        } else {
            pos = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = pos + offs;
        }
        pos = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = pos + offs;
        pos = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = pos + offs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    scale += 0x20;
    rec->unk4 = scale;
    rec->unk6 = scale - 0xC0;
    data = obj->data;
    *data = scale;
}
