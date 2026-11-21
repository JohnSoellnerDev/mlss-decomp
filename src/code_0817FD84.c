#include "global.h"

struct FallRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[4];
    u16 unkC;
    u8 filler10[0x34 - 0x10];
    u32 model;
};

struct FallObj
{
    u8 filler0[8];
    struct FallRecord *rec;
    u32 *data;
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;
};

/* FallObj.flags: the object faces left, so x offsets are mirrored. */
#define FALL_OBJ_MIRRORED 0x80

struct FallLocal
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

u32 block_flag_byte(struct FallRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

/*
 * Draws a sprite falling ever faster. obj->data holds a y offset, the fall speed and one x/y/z
 * point (24.8 fixed point): the point, shifted by the y offset, mirrored when the object is flipped
 * and offset by the object's position, is placed on screen through the camera. Afterwards the
 * y offset grows by the speed and the speed by 0x80 (half a pixel per frame).
 */
void obj_fall_accel_fd84(struct FallObj *obj)
{
    struct FallLocal pt;
    s32 *data;
    s32 yOffset;
    s32 speed;
    u32 pieceCount;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 i;
    s32 srcX;
    s32 srcY;
    s32 pos;
    s32 offs;
    u32 cursorSaved;

    data = (s32 *)obj->data;
    yOffset = *data++;
    speed = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        pt.z = *data++;
        pt.y = srcY + yOffset;
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
    yOffset += speed;
    speed += 0x80;
    data = (s32 *)obj->data;
    *data++ = yOffset;
    *data = speed;
}

/*
 * Draws a sprite drifting sideways ever faster. obj->data holds a y offset, the drift speed and
 * one x/y/z point (24.8 fixed point): the point, shifted by the y offset, mirrored when the
 * object is flipped and offset by the object's position, is placed on screen through the camera.
 * Afterwards the drift speed grows by 8 and goes to rec->unkC as x speed (negated when flipped).
 */
void obj_rec_x_drift_fe90(struct FallObj *obj)
{
    struct FallLocal pt;
    s32 *data;
    struct FallRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 drift;
    s32 yOffset;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 pos;
    s32 offs;

    data = (s32 *)obj->data;
    yOffset = *data++;
    drift = *data++;
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
        srcY = *data++;
        pt.y = srcY;
        pt.z = *data++;
        pt.y = srcY + yOffset;
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
    drift += 8;
    rec->unkC = (obj->flags & 0x80) ? -drift << 8 : drift << 8;
    data = (s32 *)obj->data;
    *data++ = yOffset;
    *data = drift;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
// FALL_OBJ_MIRRORED is set), pushed down by a fall distance that grows with a speed accelerating by
// half a pixel (0x80) per frame. State in obj->data: the fall distance, its speed, then the piece's x, y, z.
void obj_fall_accel_ffc0(struct FallObj *obj)
{
    struct FallLocal pt;
    s32 *state;
    s32 fall;
    s32 fallSpeed;
    u32 attr;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 piece;
    s32 offsetX;
    s32 offsetY;
    s32 base;
    s32 offset;
    u32 modelSaved;

    state = (s32 *)obj->data;
    fall = *state++;
    fallSpeed = *state++;
    pt.model = obj->rec->model;
    attr = block_flag_byte(obj->rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        offsetY = *state++;
        pt.y = offsetY;
        pt.z = *state++;
        pt.y = offsetY + fall;
        if (obj->flags & FALL_OBJ_MIRRORED) {
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
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    fall += fallSpeed;
    fallSpeed += 0x80;
    state = (s32 *)obj->data;
    *state++ = fall;
    *state = fallSpeed;
}

/*
 * Same as obj_fall_accel_fd84: draws a sprite falling ever faster. obj->data holds a y offset,
 * the fall speed and one x/y/z point (24.8 fixed point): the point, shifted by the y offset,
 * mirrored when the object is flipped and offset by the object's position, is placed on screen
 * through the camera. Afterwards the y offset grows by the speed and the speed by 0x80.
 */
void obj_fall_accel_00cc(struct FallObj *obj)
{
    struct FallLocal pt;
    s32 *data;
    s32 yOffset;
    s32 speed;
    u32 pieceCount;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 i;
    s32 srcX;
    s32 srcY;
    s32 pos;
    s32 offs;
    u32 cursorSaved;

    data = (s32 *)obj->data;
    yOffset = *data++;
    speed = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        pt.z = *data++;
        pt.y = srcY + yOffset;
        if (obj->flags & FALL_OBJ_MIRRORED) {
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
    yOffset += speed;
    speed += 0x80;
    data = (s32 *)obj->data;
    *data++ = yOffset;
    *data = speed;
}

/*
 * Same as obj_rec_x_drift_fe90: draws a sprite drifting sideways ever faster. obj->data holds a
 * y offset, the drift speed and one x/y/z point (24.8 fixed point): the point, shifted by the
 * y offset, mirrored when the object is flipped and offset by the object's position, is placed
 * on screen through the camera. Afterwards the drift speed grows by 8 and goes to rec->unkC as
 * x speed (negated when flipped).
 */
void obj_rec_x_drift_01d8(struct FallObj *obj)
{
    struct FallLocal pt;
    s32 *data;
    struct FallRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 drift;
    s32 yOffset;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 pos;
    s32 offs;

    data = (s32 *)obj->data;
    yOffset = *data++;
    drift = *data++;
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
        srcY = *data++;
        pt.y = srcY;
        pt.z = *data++;
        pt.y = srcY + yOffset;
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
    drift += 8;
    rec->unkC = (obj->flags & 0x80) ? -drift << 8 : drift << 8;
    data = (s32 *)obj->data;
    *data++ = yOffset;
    *data = drift;
}

/*
 * Draws a spinning burst of 8 sprites. obj->data holds the burst's scale, scale velocity and
 * spin angle, then 8 x/y/z points. Each point is scaled by scale/256, rotated about the
 * vertical axis by the spin angle, tilted by a fixed angle (12/256 of a turn), raised by 8 px,
 * mirrored when the object is flipped, offset by the object's position and placed on screen
 * through the camera. Afterwards the burst grows by its velocity (which decays by 7/8) and spins on.
 */
void obj_zoom_spin8_0308(struct FallObj *obj)
{
    s16 *cosEntry;
    s16 *sinEntry;
    struct FallLocal pt;
    struct FallRecord *rec;
    s32 scale;
    s32 scaleVel;
    s32 spin;
    u32 *data;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 angleIdx;
    s32 tmp;
    s32 offs;


    data = obj->data;
    scale = *data++;
    scaleVel = *data++;
    spin = *data++;
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

        /* spin about the vertical axis */
        ry = sz;
        angleIdx = spin & 0xFF;
        cosEntry = &gCosineTable[angleIdx];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[angleIdx];
        rz = (ry - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (gCosineTable[12] / 0x40);
        ry = (tmp - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry - 0x800;

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
    scale += scaleVel;
    scaleVel = (scaleVel * 7) / 8;
    spin += 0x12;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = spin;
}

/*
 * Draws a spinning burst of 8 sprites. obj->data holds the burst's scale, scale velocity and
 * spin angle, then 8 x/y/z points. Each point is scaled by scale/256, rotated about the
 * vertical axis by the spin angle, tilted by a fixed angle (12/256 of a turn), mirrored when
 * the object is flipped, offset by the object's position and placed on screen through the
 * camera. Afterwards the burst grows by its velocity (which decays by 7/8) and spins on.
 */
void obj_zoom_spin8_0578(struct FallObj *obj)
{
    s16 *cosEntry;
    s16 *sinEntry;
    struct FallLocal pt;
    s32 scale;
    s32 scaleVel;
    s32 spin;
    struct FallRecord *rec;
    u32 *data;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 angleIdx;
    s32 tmp;
    s32 offs;


    data = obj->data;
    scale = *data++;
    scaleVel = *data++;
    spin = *data++;
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

        /* spin about the vertical axis */
        ry = sz;
        angleIdx = spin & 0xFF;
        cosEntry = &gCosineTable[angleIdx];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[angleIdx];
        rz = (ry - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (gCosineTable[12] / 0x40);
        ry = (tmp - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry - 0x800;

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
    scale += scaleVel;
    scaleVel = (scaleVel * 7) / 8;
    spin += 0x12;
    rec->unk4 = 0x180;
    rec->unk6 = 0x180;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = spin;
}

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   spin around the vertical axis, are tilted by a fixed angle (12/256) and drawn 8 px higher;
   state is 3 words in obj->data (scale, scale speed, yaw). */
void obj_zoom_spin8_07fc(struct FallObj *obj)
{
    s16 *cosp;
    s16 *sinp;
    struct FallLocal pt;
    s32 scale;
    s32 scaleSpeed;
    s32 yaw;
    struct FallRecord *rec;
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
    s32 rx;
    s32 ry;
    s32 rz;
    s32 yawIdx;
    s32 t;
    s32 pos;


    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    yaw = *state++;
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
        ry = sz;
        yawIdx = yaw & 0xFF;
        cosp = &gCosineTable[yawIdx];
        ry = ry * (*cosp / 0x40);
        sinp = &gSineTable[yawIdx];
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry - 0x800;

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
    scaleSpeed = (scaleSpeed * 7) / 8;
    yaw += 0x12;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state = yaw;
}

/* Effect update: draws 6 pieces that spread out (scale grows by a slowing speed), spin around the
   vertical axis, are tilted by a fixed angle (12/256) and fall with gravity; state is 5 words in obj->data. */
void obj_zoom_fall6_0a80(struct FallObj *obj)
{
    s16 *cosp;
    s16 *sinp;
    struct FallLocal pt;
    struct FallRecord *rec;
    s32 scale;
    s32 scaleSpeed;
    s32 yaw;
    s32 yOffset;
    s32 ySpeed;
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
    s32 rx;
    s32 ry;
    s32 rz;
    s32 yawIdx;
    s32 t;
    s32 pos;


    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    yaw = *state++;
    yOffset = *state++;
    ySpeed = *state++;
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
        ry = sz;
        yawIdx = yaw & 0xFF;
        cosp = &gCosineTable[yawIdx];
        ry = ry * (*cosp / 0x40);
        sinp = &gSineTable[yawIdx];
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry + yOffset;

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
    scaleSpeed -= 0x14;
    yaw += 1;
    yOffset += ySpeed;
    ySpeed += 0x60;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = yaw;
    *state++ = yOffset;
    *state = ySpeed;
}

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   are tipped over by a quarter turn (64/256) around the x axis, spin in the screen plane (around the
   z axis) and are drawn 16 px higher; state is 3 words in obj->data (scale, scale speed, spin). */
void obj_zoom_quarter_spin8_0d14(struct FallObj *obj)
{
    s16 *cosTable;
    s16 *cosQuarter;
    s16 *sinTable;
    s16 *sinQuarter;
    s16 *cosp;
    s16 *sinp;
    struct FallLocal pt;
    struct FallRecord *rec;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
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
    s32 rx;
    s32 ry;
    s32 spinIdx;
    s32 ty;
    s32 tz;
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
        /* tip over a quarter turn around the x axis */
        t = sy;
        cosTable = gCosineTable;
        cosQuarter = cosTable + 64;
        t = t * (*cosQuarter / 0x40);
        sinTable = gSineTable;
        sinQuarter = sinTable + 64;
        ty = (t - (*sinQuarter / 0x40) * sz) / 0x100;
        pt.y = ty;
        tz = ((sz * (*cosQuarter / 0x40)) + ((*sinQuarter / 0x40) * sy)) / 0x100;
        pt.z = tz;
        /* spin around the z axis */
        rx = sx;
        spinIdx = spin & 0xFF;
        cosp = &gCosineTable[spinIdx];
        rx = rx * (*cosp / 0x40);
        sinp = sinTable + spinIdx;
        rx = (rx - (*sinp / 0x40) * ty) / 0x100;
        pt.x = rx;
        ry = (ty * (*cosp / 0x40) + (*sinp / 0x40) * sx) / 0x100;
        pt.y = ry - 0x1000;

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
    scaleSpeed = (scaleSpeed * 7) / 8;
    spin += 3;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state = spin;
}

/*
 * Draws a ring of 8 sprites facing the camera. obj->data holds the ring's scale, scale velocity
 * and spin angle, then 8 x/y/z points. Each point is scaled by scale/256, tipped a quarter turn
 * (64/256) about the x axis so the ring stands upright, spun about the view axis by the spin
 * angle, raised by 16 px, mirrored when the object is flipped, offset by the object's position
 * and placed on screen through the camera. Afterwards the scale springs: it grows by its
 * velocity, which is pulled back by 1/128 of the scale, and the ring spins on by 4.
 */
void obj_zoom_quarter_spin8_0f78(struct FallObj *obj)
{
    s16 *cosTable;
    s16 *cosQuarter;
    s16 *sinTable;
    s16 *sinQuarter;
    s16 *cosEntry;
    s16 *sinEntry;
    struct FallLocal pt;
    struct FallRecord *rec;
    s32 scale;
    s32 scaleVel;
    s32 spin;
    u32 *data;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 angleIdx;
    s32 tippedY;
    s32 tippedZ;
    s32 tmp;
    s32 offs;

    data = obj->data;
    scale = *data++;
    scaleVel = *data++;
    spin = *data++;
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

        /* tip a quarter turn about the x axis */
        tmp = sy;
        cosTable = gCosineTable;
        cosQuarter = cosTable + 64;
        tmp = tmp * (*cosQuarter / 0x40);
        sinTable = gSineTable;
        sinQuarter = sinTable + 64;
        tippedY = (tmp - (*sinQuarter / 0x40) * sz) / 0x100;
        pt.y = tippedY;
        tippedZ = ((sz * (*cosQuarter / 0x40)) + ((*sinQuarter / 0x40) * sy)) / 0x100;
        pt.z = tippedZ;

        /* spin about the view axis */
        rx = sx;
        angleIdx = spin & 0xFF;
        cosEntry = &gCosineTable[angleIdx];
        rx = rx * (*cosEntry / 0x40);
        sinEntry = sinTable + angleIdx;
        rx = (rx - (*sinEntry / 0x40) * tippedY) / 0x100;
        pt.x = rx;
        ry = (tippedY * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sx) / 0x100;
        pt.y = ry - 0x1000;

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
    scale += scaleVel;
    scaleVel -= scale / 0x80;
    spin += 4;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = spin;
}


ASM_FUNC("asm/code_0817FD84/obj_zoom_tilt_rise8_11dc.s", void obj_zoom_tilt_rise8_11dc(struct FallObj *obj))

/* Effect update: draws 10 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped a quarter turn (64/256) around the x axis and lifted 16 pixels; state is 2 words in obj->data
   (scale, scale speed). */
void obj_zoom_quarter10_1440(struct FallObj *obj)
{
    struct FallLocal pt;
    struct FallRecord *rec;
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
    for (piece = 0; piece < 10; piece++) {
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
        /* tip over a quarter turn around the x axis, then lift 16 pixels */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        ry = (t - (gSineTable[64] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;
        pt.y = ry - 0x1000;

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

