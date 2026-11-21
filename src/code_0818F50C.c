#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4; /* sprite x scale, 8.8 (0x100 = 1:1) */
    u16 unk6; /* sprite y scale, 8.8 */
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

extern s16 gCosineTable[];
extern s16 gSineTable[];

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model8_turn_rem6_f50c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    s32 spriteScale;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    s16 *sinTable;
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
    for (piece = 0; piece < 8; piece++) {
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
        sinTable = gSineTable;
        sinp = sinTable + (spin & 0xFF);
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 6) / 7;
    spin -= 4;
    spriteScale -= 6;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = spin;
    *state = spriteScale;
}
/*
 * Effect update: six sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying step (2/3 per frame), rotated by a yaw of angle 0
 * and a pitch of angle 64 (laid flat), then drawn around the object (mirrored
 * in X when flag 0x80 is set).
 */
void obj_zoom_model6_flat_f760(struct Point1Obj *obj)
{
    s16 *yawCos;
    s16 *yawSin;
    s16 *pitchCos;
    s16 *pitchSin;
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomStep;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 scaledX;
    s32 scaledZ;
    s32 yawX;
    s32 pitchY;
    s32 yawZ;
    s32 tmp;
    s32 ofs;
    s32 scaledY;

    state = obj->data;
    zoom = *state++;
    zoomStep = *state++;
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
        tmp = scaledZ;
        yawCos = &gCosineTable[0];
        tmp = tmp * (*yawCos / 0x40);
        yawSin = &gSineTable[0];
        yawZ = (tmp - (*yawSin / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*yawCos / 0x40) + (*yawSin / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        pitchCos = &gCosineTable[64];
        tmp = tmp * (*pitchCos / 0x40);
        pitchSin = &gSineTable[64];
        pitchY = (tmp - (*pitchSin / 0x40) * yawZ) / 0x100;
        pt.y = pitchY;
        pt.z = (yawZ * (*pitchCos / 0x40) + ((*pitchSin / 0x40) * scaledY)) / 0x100;

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
    zoom += zoomStep;
    zoomStep = (zoomStep * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomStep;
}
/*
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * (mirrored in X when flag 0x80 is set) and shrinks its scale by a step that
 * slows to 31/32 each frame. State in obj->data: scale, scale step, then the
 * sprite's offset (x, y, z).
 */
void obj_point1_zoom_step31_f994(struct Point1Obj *obj)
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
    s32 base;
    s32 ofs;
    s32 zoom;
    s32 zoomStep;

    state = obj->data;
    zoom = *state++;
    zoomStep = *state++;
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
        pt.z = *state++;
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
    zoom -= zoomStep;
    zoomStep = (zoomStep * 31) / 32;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    state = obj->data;
    *state++ = zoom;
    *state = zoomStep;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set); the sprite's scale drops by a step that is damped to 15/16 each frame, so it eases to a stop.
// State in obj->data: scale, step, then the piece's x, y, z.
void obj_point1_zoom_step15_fab4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;
    s32 scale;
    s32 step;

    state = obj->data;
    scale = *state++;
    step = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = base - offset;
        } else {
            base = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = base + offset;
        }
        base = obj->posY;
        offset = pt.y;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.y = base + offset;
        base = obj->posZ;
        offset = pt.z;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.z = base + offset;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    scale -= step;
    step = (step * 15) / 16;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = scale;
    *state = step;
}

/*
 * Draws 8 sprites that spread out, spin and shrink. obj->data holds a scale, its growth per frame,
 * a turn angle (256 to a turn) and the sprites' size, then 8 x/y/z points. Each point is scaled by
 * scale/256, turned about the vertical axis, tipped a quarter turn about the x axis, mirrored when
 * the object is flipped, offset by the object's position and placed on screen through the camera.
 * Each frame the scale grows by its growth, which slows to 6/7, the angle turns back by 4 and the
 * size (rec->unk4/unk6, the sprite's scale) drops by 6.
 */
void obj_zoom_model8_turn_rem6_fbd4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleStep;
    s32 angle;
    s32 size;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    u32 *data;
    s16 *sinTable;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = obj->data;
    scale = *data++;
    scaleStep = *data++;
    angle = *data++;
    size = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 8; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        rz = sz;
        cosEntry = &gCosineTable[angle & 0xFF];
        rz = rz * (*cosEntry / 0x40);
        sinTable = gSineTable;
        sinEntry = sinTable + (angle & 0xFF);
        rz = (rz - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* tip a quarter turn about the x axis */
        tmp = sy;
        tmp = tmp * (gCosineTable[64] / 0x40);
        pt.y = (tmp - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = rx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = rx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
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
    scale += scaleStep;
    scaleStep = (scaleStep * 6) / 7;
    angle -= 4;
    size -= 6;
    rec->unk4 = size;
    rec->unk6 = size;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleStep;
    *data++ = angle;
    *data = size;
}

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 2/3 each
   frame), turned by angle 0 around the vertical axis and tipped over a quarter turn (64/256)
   around the x axis so they lie flat; state is 2 words in obj->data (scale, scale speed),
   followed by each piece's offset (x, y, z). */
void obj_zoom_model8_flat_fe28(struct Point1Obj *obj)
{
    s16 *cosTurn;
    s16 *sinTurn;
    s16 *cosTip;
    s16 *sinTip;
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
    s32 sz;
    s32 rx;
    s32 rz;
    s32 t;
    s32 pos;
    s32 sy;

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
    for (piece = 0; piece < 8; piece++) {
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
        /* turn around the vertical axis (angle 0) */
        t = sz;
        cosTurn = &gCosineTable[0];
        t = t * (*cosTurn / 0x40);
        sinTurn = &gSineTable[0];
        rz = (t - (*sinTurn / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosTurn / 0x40) + (*sinTurn / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        cosTip = &gCosineTable[64];
        t = t * (*cosTip / 0x40);
        sinTip = &gSineTable[64];
        pt.y = (t - (*sinTip / 0x40) * rz) / 0x100;
        pt.z = (rz * (*cosTip / 0x40) + ((*sinTip / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 2) / 3;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/*
 * Effect update (same as obj_point1_zoom_step31_f994): draws one sprite at its
 * 8.8 fixed-point offset from the object and shrinks its scale by a step that
 * slows to 31/32 each frame. State in obj->data: scale, scale step, then the
 * sprite's offset (x, y, z).
 */
void obj_point1_zoom_step31_005c(struct Point1Obj *obj)
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
    s32 base;
    s32 ofs;
    s32 zoom;
    s32 zoomStep;

    state = obj->data;
    zoom = *state++;
    zoomStep = *state++;
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
        pt.z = *state++;
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
    zoom -= zoomStep;
    zoomStep = (zoomStep * 31) / 32;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    state = obj->data;
    *state++ = zoom;
    *state = zoomStep;
}

/*
 * Same as obj_zoom_model8_turn_rem6_fbd4 with 2 sprites: obj->data holds a scale, its growth, a
 * turn angle and the sprites' size, then 2 x/y/z points, each scaled, turned about the vertical
 * axis, tipped a quarter turn, mirrored when flipped, offset by the object's position and placed
 * through the camera. Each frame the growth slows to 6/7, the angle turns back by 4 and the size
 * (rec->unk4/unk6, the sprite's scale) drops by 6.
 */
void obj_zoom_model2_turn_rem6_017c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleStep;
    s32 angle;
    s32 size;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    u32 *data;
    s16 *sinTable;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = obj->data;
    scale = *data++;
    scaleStep = *data++;
    angle = *data++;
    size = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 2; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        rz = sz;
        cosEntry = &gCosineTable[angle & 0xFF];
        rz = rz * (*cosEntry / 0x40);
        sinTable = gSineTable;
        sinEntry = sinTable + (angle & 0xFF);
        rz = (rz - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* tip a quarter turn about the x axis */
        tmp = sy;
        tmp = tmp * (gCosineTable[64] / 0x40);
        pt.y = (tmp - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = rx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = rx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
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
    scale += scaleStep;
    scaleStep = (scaleStep * 6) / 7;
    angle -= 4;
    size -= 6;
    rec->unk4 = size;
    rec->unk6 = size;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleStep;
    *data++ = angle;
    *data = size;
}

/* Effect update: draws 2 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model2_turn_rem6_03d0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    s32 spriteScale;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    s16 *sinTable;
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
        /* spin around the vertical axis */
        rz = sz;
        cosp = &gCosineTable[spin & 0xFF];
        rz = rz * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + (spin & 0xFF);
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 6) / 7;
    spin -= 4;
    spriteScale -= 6;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = spin;
    *state = spriteScale;
}

/*
 * Effect update: two sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying velocity (2/3 per frame), rotated by yaw
 * angle 0 and pitch angle 64, then drawn around the object (mirrored in X
 * when flag 0x80 is set).
 */
void obj_zoom_model2_flat_0624(struct Point1Obj *obj)
{
    s16 *yawCos;
    s16 *yawSin;
    s16 *pitchCos;
    s16 *pitchSin;
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
    s32 scaledZ;
    s32 yawX;
    s32 yawZ;
    s32 tmp;
    s32 ofs;
    s32 scaledY;

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

        /* yaw: rotate in the X/Z plane */
        tmp = scaledZ;
        yawCos = &gCosineTable[0];
        tmp = tmp * (*yawCos / 0x40);
        yawSin = &gSineTable[0];
        yawZ = (tmp - (*yawSin / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*yawCos / 0x40) + (*yawSin / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        pitchCos = &gCosineTable[64];
        tmp = tmp * (*pitchCos / 0x40);
        pitchSin = &gSineTable[64];
        pt.y = (tmp - (*pitchSin / 0x40) * yawZ) / 0x100;
        pt.z = (yawZ * (*pitchCos / 0x40) + ((*pitchSin / 0x40) * scaledY)) / 0x100;

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
    zoomVel = (zoomVel * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

/* Effect update: draws 3 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model3_turn_rem6_0858(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    s32 spriteScale;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    s16 *sinTable;
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
    for (piece = 0; piece < 3; piece++) {
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
        sinTable = gSineTable;
        sinp = sinTable + (spin & 0xFF);
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 6) / 7;
    spin -= 4;
    spriteScale -= 6;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = spin;
    *state = spriteScale;
}

/* Effect update: draws 3 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite's scale drops by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model3_turn_rem6_0aac(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    s32 spriteScale;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    s16 *sines;
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
    for (piece = 0; piece < 3; piece++) {
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
        sines = gSineTable;
        sinp = sines + (spin & 0xFF);
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (sines[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sines[64] / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 6) / 7;
    spin -= 4;
    spriteScale -= 6;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = spin;
    *state = spriteScale;
}

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 2/3 each frame),
   turned an eighth turn (32/256) around the vertical axis and tipped over a quarter turn (64/256)
   around the x axis; state is 2 words in obj->data (scale, scale speed). */
void obj_zoom_model4_turn45_0d00(struct Point1Obj *obj)
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
    s32 sz;
    s32 rx;
    s32 rz;
    s32 t;
    s32 pos;
    s32 sy;

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
    for (piece = 0; piece < 4; piece++) {
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
        /* an eighth turn around the vertical axis */
        t = sz;
        t = t * (gCosineTable[32] / 0x40);
        rz = (t - (gSineTable[32] / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * sz) / 0x100;
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
    scaleSpeed = (scaleSpeed * 2) / 3;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

