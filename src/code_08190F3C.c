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

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);


/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model4_turn_rem6_0f3c(struct Point1Obj *obj)
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
 * Effect update: four sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a velocity decaying to 6/7 each frame, rotated by a yaw
 * that turns 4 steps per frame and a fixed pitch (angle 64), then drawn around
 * the object (mirrored in X when flag 0x80 is set). The record's size fields
 * (unk4/unk6) shrink by 6 every frame.
 */
void obj_zoom_model4_turn_rem6_1190(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 zoom;
    s32 zoomVel;
    s32 yaw;
    s32 size;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    s16 *sineTable;
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
    for (i = 0; i < 4; i++) {
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
        sineTable = gSineTable;
        sinYaw = sineTable + (yaw & 0xFF);
        yawZ = (yawZ - (*sinYaw / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosYaw / 0x40) + (*sinYaw / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[64] / 0x40);
        pt.y = (tmp - (sineTable[64] / 0x40) * yawZ) / 0x100;
        pt.z = ((yawZ * (gCosineTable[64] / 0x40)) + ((sineTable[64] / 0x40) * scaledY)) / 0x100;

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
    zoomVel = (zoomVel * 6) / 7;
    yaw -= 4;
    size -= 6;
    rec->unk4 = size;
    rec->unk6 = size;
    state = obj->data;
    *state++ = zoom;
    *state++ = zoomVel;
    *state++ = yaw;
    *state = size;
}

/*
 * Effect update: six sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying step (2/3 per frame), rotated by a yaw of angle 0
 * and a pitch of angle 64 (laid flat), then drawn around the object (mirrored
 * in X when flag 0x80 is set). Same as obj_zoom_model6_flat_f760.
 */
void obj_zoom_model6_flat_13e4(struct Point1Obj *obj)
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

/* Effect update: draws one sprite at its offset (8.8 fixed point) from the object (mirrored
   in X when flag 0x80 is set) while it shrinks: the sprite scale drops by a speed that slows
   to 31/32 each frame; state is 2 words in obj->data (sprite scale, shrink speed) followed by
   the sprite's offset (x, y, z). */
void obj_point1_rem_step31_1618(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 pos;
    s32 t;
    s32 spriteScale;
    s32 shrinkSpeed;

    state = obj->data;
    spriteScale = *state++;
    shrinkSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos + t;
        }
        pos = obj->posY;
        t = pt.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.y = pos + t;
        pos = obj->posZ;
        t = pt.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.z = pos + t;
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
    spriteScale -= shrinkSpeed;
    shrinkSpeed = (shrinkSpeed * 31) / 32;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = spriteScale;
    *state = shrinkSpeed;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set); the sprite's scale drops by a step that is damped to 15/16 each frame, so it eases to a stop.
// State in obj->data: scale, step, then the piece's x, y, z.
void obj_point1_rem_step15_1738(struct Point1Obj *obj)
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

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite's scale drops by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model4_turn_rem6_1858(struct Point1Obj *obj)
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

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model4_turn_rem6_1aac(struct Point1Obj *obj)
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
 * Effect update: eight sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a velocity decaying to 2/3 each frame, run through a yaw
 * of angle 0 (no turn) and a fixed pitch (angle 64), then drawn around the
 * object (mirrored in X when flag 0x80 is set).
 */
void obj_zoom_model8_flat_1d00(struct Point1Obj *obj)
{
    s16 *cosYaw;
    s16 *sinYaw;
    s16 *cosPitch;
    s16 *sinPitch;
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
    for (i = 0; i < 8; i++) {
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

        /* yaw (angle 0): rotate in the X/Z plane */
        tmp = scaledZ;
        cosYaw = &gCosineTable[0];
        tmp = tmp * (*cosYaw / 0x40);
        sinYaw = &gSineTable[0];
        yawZ = (tmp - (*sinYaw / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosYaw / 0x40) + (*sinYaw / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch (angle 64): rotate in the Y/Z plane */
        tmp = scaledY;
        cosPitch = &gCosineTable[64];
        tmp = tmp * (*cosPitch / 0x40);
        sinPitch = &gSineTable[64];
        pt.y = (tmp - (*sinPitch / 0x40) * yawZ) / 0x100;
        pt.z = (yawZ * (*cosPitch / 0x40) + ((*sinPitch / 0x40) * scaledY)) / 0x100;

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

/* Effect update: draws one piece at its offset from the object (mirrored when flags bit 7 is set)
   while its sprite shrinks by a speed that slows to 27/28 each frame; state is 2 words in
   obj->data (sprite scale, shrink speed), followed by the piece's offset (x, y, z). */
void obj_point1_rem_step27_1f34(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *pz;
    u32 *pmodel;
    s32 *py;
    s32 srcX;
    s32 pos;
    s32 t;
    s32 spriteScale;
    s32 shrinkSpeed;

    state = obj->data;
    spriteScale = *state++;
    shrinkSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos + t;
        }
        pos = obj->posY;
        t = pt.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.y = pos + t;
        pos = obj->posZ;
        t = pt.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.z = pos + t;
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
    spriteScale -= shrinkSpeed;
    shrinkSpeed = (shrinkSpeed * 27) / 28;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = spriteScale;
    *state = shrinkSpeed;
}

/* Effect update: draws one piece at its offset from the object (mirrored when flags bit 7 is set)
   while its sprite shrinks by a speed that slows to 21/22 each frame; state is 2 words in
   obj->data (sprite scale, shrink speed), followed by the piece's offset (x, y, z). */
void obj_point1_rem_step21_2058(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *pz;
    u32 *pmodel;
    s32 *py;
    s32 srcX;
    s32 pos;
    s32 t;
    s32 spriteScale;
    s32 shrinkSpeed;

    state = obj->data;
    spriteScale = *state++;
    shrinkSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos + t;
        }
        pos = obj->posY;
        t = pt.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.y = pos + t;
        pos = obj->posZ;
        t = pt.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.z = pos + t;
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
    spriteScale -= shrinkSpeed;
    shrinkSpeed = (shrinkSpeed * 21) / 22;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = spriteScale;
    *state = shrinkSpeed;
}

/*
 * Effect update: four sprites whose offsets (8.8 fixed point) are scaled by a
 * fixed zoom, rotated by a yaw that turns 3 steps per frame and a fixed pitch
 * (angle 64), then drawn around the object (mirrored in X when flag 0x80 is
 * set). The stored zoom velocity is kept but not applied.
 */
void obj_spin_model4_minus3(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 zoom;
    s32 zoomVel;
    s32 yaw;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    struct ObjRecord *rec;
    s16 *cosineTable;
    s16 *sineTable;
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
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 4; i++) {
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
        cosineTable = gCosineTable;
        cosYaw = cosineTable + (yaw & 0xFF);
        yawZ = yawZ * (*cosYaw / 0x40);
        sineTable = gSineTable;
        sinYaw = sineTable + (yaw & 0xFF);
        yawZ = (yawZ - (*sinYaw / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosYaw / 0x40) + (*sinYaw / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (cosineTable[64] / 0x40);
        pt.y = (tmp - (sineTable[64] / 0x40) * yawZ) / 0x100;
        pt.z = ((yawZ * (cosineTable[64] / 0x40)) + ((sineTable[64] / 0x40) * scaledY)) / 0x100;

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
    yaw -= 3;
    state = obj->data;
    *state++ = zoom;
    *state++ = zoomVel;
    *state = yaw;
}

/* Effect update: draws 4 pieces at a fixed scale that spin around the vertical axis (3/256 a frame)
   and are tipped over a quarter turn (64/256) around the x axis; state is 3 words in obj->data
   (scale, scale speed (kept, unused here), spin). */
void obj_spin_model4_plus3(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    struct ObjRecord *rec;
    s16 *cosTable;
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
        /* spin around the vertical axis */
        rz = sz;
        cosTable = gCosineTable;
        cosp = cosTable + (spin & 0xFF);
        rz = rz * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + (spin & 0xFF);
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (cosTable[64] / 0x40);
        pt.y = (t - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (cosTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

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
    spin += 3;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state = spin;
}

/* Effect update (same as obj_point1_rem_step31_1618): draws one sprite at its offset (8.8
   fixed point) from the object (mirrored in X when flag 0x80 is set) while it shrinks: the
   sprite scale drops by a speed that slows to 31/32 each frame; state is 2 words in obj->data
   (sprite scale, shrink speed) followed by the sprite's offset (x, y, z). */
void obj_point1_zoom_step31(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 pos;
    s32 t;
    s32 spriteScale;
    s32 shrinkSpeed;

    state = obj->data;
    spriteScale = *state++;
    shrinkSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos + t;
        }
        pos = obj->posY;
        t = pt.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.y = pos + t;
        pos = obj->posZ;
        t = pt.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.z = pos + t;
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
    spriteScale -= shrinkSpeed;
    shrinkSpeed = (shrinkSpeed * 31) / 32;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = spriteScale;
    *state = shrinkSpeed;
}

/* Effect update (like obj_point1_zoom_step31, but slowing to 15/16): draws one sprite at its
   offset (8.8 fixed point) from the object (mirrored in X when flag 0x80 is set) while it
   shrinks: the sprite scale drops by a speed that slows to 15/16 each frame; state is 2 words
   in obj->data (sprite scale, shrink speed) followed by the sprite's offset (x, y, z). */
void obj_point1_zoom_step15(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 pos;
    s32 t;
    s32 spriteScale;
    s32 shrinkSpeed;

    state = obj->data;
    spriteScale = *state++;
    shrinkSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos + t;
        }
        pos = obj->posY;
        t = pt.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.y = pos + t;
        pos = obj->posZ;
        t = pt.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.z = pos + t;
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
    spriteScale -= shrinkSpeed;
    shrinkSpeed = (shrinkSpeed * 15) / 16;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = spriteScale;
    *state = shrinkSpeed;
}
