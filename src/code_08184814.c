#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4;  // scale X (0x100 = 1.0)
    u16 unk6;  // scale Y (0x100 = 1.0)
    u8 filler8[4];
    u16 unkC;
    u8 filler10[0x34 - 0x10];
    u32 model; // the sprite's OAM row list
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

// Single point that bounces: its height (8.8 fixed, negative is up) falls by the speed and the
// speed grows by gravity (0x40). On landing the height clamps to 0 and the bounce count rises
// towards 0; the next rebound speed is bounce << 8, so each bounce is weaker until it stops.
void obj_bounce_point_plain(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 cursorSaved;
    s32 *words;
    s32 height;
    s32 speed;
    s32 bounce;
    u32 pieceCount;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 i;
    s32 srcX;
    s32 srcY;
    s32 tmp;
    s32 offs;

    words = (s32 *)obj->data;
    height = *words++;
    speed = *words++;
    bounce = *words++;
    l.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(l.model, pieceCount);
    py = &l.y;
    pz = &l.z;
    cursor = &l.model;
    for (i = 0; i < 1; i++) {
        srcX = *words++;
        l.x = srcX;
        srcY = *words++;
        l.y = srcY;
        l.z = *words++;
        l.y = srcY + height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = l.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.y = tmp + offs;
        tmp = obj->posZ;
        offs = l.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.z = tmp + offs;
        offset_words_camera(&l.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, l.x, l.y, l.z << 14, 0, 0);
        if (!cursorSaved) {
            l.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = l.model;
    }
    height += speed;
    speed += 0x40;
    if (height > 0) {
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        speed = bounce << 8;
    }
    words = (s32 *)obj->data;
    *words++ = height;
    *words++ = speed;
    *words = bounce;
}

/* Effect update: draws 3 pieces that spread out (scale grows by a step that shrinks to 7/8 each frame),
   spin around the vertical axis (6/256 per frame), are tilted by a fixed angle (12/256) and rise by
   0x300 per frame; state is 4 words in obj->data. */
void obj_spin_rise_model3_fast(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 yawIdx;
    s32 scaleStep;
    s32 yaw;
    s32 yOffset;
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
    s32 ry;
    s32 rz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleStep = *state++;
    yaw = *state++;
    yOffset = *state++;
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
        ry = sz;
        yawIdx = yaw & 0xFF;
        cosp = &gCosineTable[yawIdx];
        ry = ry * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + yawIdx;
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (sinTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((sinTable[12] / 0x40) * sy)) / 0x100;
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
    scale += scaleStep;
    scaleStep = (scaleStep * 7) / 8;
    yaw += 6;
    yOffset -= 0x300;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleStep;
    *state++ = yaw;
    *state = yOffset;
}

/* Effect update: draws 3 pieces that spread out (scale grows by a step that shrinks to 7/8 each frame),
   spin around the vertical axis (3/256 per frame), are tilted by a fixed angle (12/256) and rise by
   0x200 per frame; state is 4 words in obj->data. */
void obj_spin_rise_model3_a(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 yawIdx;
    s32 scaleStep;
    s32 yaw;
    s32 yOffset;
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
    s32 ry;
    s32 rz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleStep = *state++;
    yaw = *state++;
    yOffset = *state++;
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
        ry = sz;
        yawIdx = yaw & 0xFF;
        cosp = &gCosineTable[yawIdx];
        ry = ry * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + yawIdx;
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (sinTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((sinTable[12] / 0x40) * sy)) / 0x100;
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
    scale += scaleStep;
    scaleStep = (scaleStep * 7) / 8;
    yaw += 3;
    yOffset -= 0x200;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleStep;
    *state++ = yaw;
    *state = yOffset;
}

// Three-point object that spins while it rises: each point is zoomed (8.8 fixed point), turned
// around the vertical axis by `yaw` (a table angle), tilted by the fixed angle 12, lifted and drawn
// at the object (mirrored when flags bit 7 is set). Every frame the zoom grows by a speed that
// shrinks to 7/8, the yaw turns by 3 and the object rises by 0x200.
void obj_spin_rise_model3_b(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 zoom;
    s32 yawIdx;
    s32 zoomVel;
    s32 yaw;
    s32 lift;
    struct ObjRecord *rec;
    u32 oamFlags;
    u32 modelSaved;
    s32 i;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    u32 *words;
    s16 *sinTab;
    s16 *cosYaw;
    s16 *sinYaw;
    s32 px;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 yawX;
    s32 tiltY;
    s32 yawZ;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    zoomVel = *words++;
    yaw = *words++;
    lift = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 3; i++) {
        // The point's offset from the object, zoomed.
        px = *words++;
        pt.x = px;
        pt.y = *words++;
        pt.z = *words++;
        zx = (px * zoom) / 0x100;
        pt.x = zx;
        zy = (pt.y * zoom) / 0x100;
        pt.y = zy;
        zz = (pt.z * zoom) / 0x100;
        pt.z = zz;
        // Turn it around the vertical axis...
        tiltY = zz;
        yawIdx = yaw & 0xFF;
        cosYaw = &gCosineTable[yawIdx];
        tiltY = tiltY * (*cosYaw / 0x40);
        sinTab = gSineTable;
        sinYaw = sinTab + yawIdx;
        yawZ = (tiltY - (*sinYaw / 0x40) * zx) / 0x100;
        pt.z = yawZ;
        yawX = (zx * (*cosYaw / 0x40) + (*sinYaw / 0x40) * zz) / 0x100;
        pt.x = yawX;
        // ...and tilt it.
        t = zy;
        t = t * (gCosineTable[12] / 0x40);
        tiltY = (t - (sinTab[12] / 0x40) * yawZ) / 0x100;
        pt.y = tiltY;
        pt.z = ((yawZ * (gCosineTable[12] / 0x40)) + ((sinTab[12] / 0x40) * zy)) / 0x100;
        pt.y = tiltY + lift;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = yawX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = yawX;
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
    yaw += 3;
    lift -= 0x200;
    words = obj->data;
    *words++ = zoom;
    *words++ = zoomVel;
    *words++ = yaw;
    *words = lift;
}

/* Effect update (same as obj_spin_rise_model3_a): draws 3 pieces that spread out (scale grows by a step
   that shrinks to 7/8 each frame), spin around the vertical axis (3/256 per frame), are tilted by a fixed
   angle (12/256) and rise by 0x200 per frame; state is 4 words in obj->data. */
void obj_spin_rise_model3_c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 yawIdx;
    s32 scaleStep;
    s32 yaw;
    s32 yOffset;
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
    s32 ry;
    s32 rz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleStep = *state++;
    yaw = *state++;
    yOffset = *state++;
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
        ry = sz;
        yawIdx = yaw & 0xFF;
        cosp = &gCosineTable[yawIdx];
        ry = ry * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + yawIdx;
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (sinTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((sinTable[12] / 0x40) * sy)) / 0x100;
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
    scale += scaleStep;
    scaleStep = (scaleStep * 7) / 8;
    yaw += 3;
    yOffset -= 0x200;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleStep;
    *state++ = yaw;
    *state = yOffset;
}

/* Effect update: draws 12 pieces that spread out (scale grows by a step that shrinks to 7/8 each frame),
   spin around the vertical axis (3/256 per frame), are tilted by a fixed angle (12/256) and rise by
   0x200 per frame; state is 4 words in obj->data. */
void obj_spin_rise_model12(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 yawIdx;
    s32 scaleStep;
    s32 yaw;
    s32 yOffset;
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
    s32 ry;
    s32 rz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleStep = *state++;
    yaw = *state++;
    yOffset = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    /* each piece: its offset (x, y, z) follows the state words */
    for (piece = 0; piece < 12; piece++) {
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
        sinTable = gSineTable;
        sinp = sinTable + yawIdx;
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (sinTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((sinTable[12] / 0x40) * sy)) / 0x100;
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
    scale += scaleStep;
    scaleStep = (scaleStep * 7) / 8;
    yaw += 3;
    yOffset -= 0x200;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleStep;
    *state++ = yaw;
    *state = yOffset;
}

// Single point drawn with a pulsing zoom (the sprite's width and height scale): obj->data holds the
// zoom and its step, then 1 x/y/z point. Each frame the zoom moves by the step; above 384 the step
// shrinks by 1, and once the step is negative the zoom never drops below 256.
void obj_pulse_zoom_point(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *pz;
    u32 *cursor;
    s32 *py;
    s32 srcX;
    s32 tmp;
    s32 offs;
    s32 zoom;
    s32 zoomStep;

    words = obj->data;
    zoom = *words++;
    zoomStep = *words++;
    rec = obj->rec;
    l.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(l.model, pieceCount);
    py = &l.y;
    pz = &l.z;
    cursor = &l.model;
    for (i = 0; i < 1; i++) {
        srcX = *words++;
        l.x = srcX;
        l.y = *words++;
        l.z = *words++;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = l.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.y = tmp + offs;
        tmp = obj->posZ;
        offs = l.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.z = tmp + offs;
        offset_words_camera(&l.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, l.x, l.y, l.z << 14, 0, 0);
        if (!cursorSaved) {
            l.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = l.model;
    }
    zoom += zoomStep;
    if (zoom > 384) {
        zoomStep--;
    }
    if (zoomStep < 0) {
        if (zoom <= 255) {
            zoom = 0x100;
        }
    }
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words++ = zoom;
    *words = zoomStep;
}

// Single point drawn with a growing zoom (the sprite's width and height scale): obj->data holds the
// zoom, then 1 x/y/z point (8.8 fixed, x mirrored when flags bit 7 is set). Each frame the zoom
// grows by 0x40.
void obj_grow_zoom_point(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 tmp;
    s32 offs;
    s32 zoom;

    words = obj->data;
    zoom = *words++;
    rec = obj->rec;
    l.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(l.model, pieceCount);
    py = &l.y;
    pz = &l.z;
    cursor = &l.model;
    for (i = 0; i < 1; i++) {
        srcX = *words++;
        l.x = srcX;
        l.y = *words++;
        l.z = *words++;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = l.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.y = tmp + offs;
        tmp = obj->posZ;
        offs = l.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.z = tmp + offs;
        offset_words_camera(&l.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, l.x, l.y, l.z << 14, 0, 0);
        if (!cursorSaved) {
            l.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = l.model;
    }
    zoom += 0x40;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words = zoom;
}

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 7/8 each
   frame), tipped over a quarter turn (64/256) around the x axis and then turned an eighth of a turn
   (32/256) around the z axis; state is 2 words in obj->data (scale, scale speed). */
void obj_turn_zoom4_5830(struct Point1Obj *obj)
{
    s16 *cosines;
    s16 *sines;
    s16 *cosTip;
    s16 *sinTip;
    s16 *cosTurn;
    s16 *sinTurn;
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
    s32 rx;
    s32 ry;
    s32 rz;
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
        /* tip over a quarter turn around the x axis */
        t = sy;
        cosines = gCosineTable;
        cosTip = cosines + 64;
        t = t * (*cosTip / 0x40);
        sines = gSineTable;
        sinTip = sines + 64;
        ry = (t - (*sinTip / 0x40) * sz) / 0x100;
        pt.y = ry;
        rz = (sz * (*cosTip / 0x40) + (*sinTip / 0x40) * sy) / 0x100;
        pt.z = rz;
        /* turn an eighth of a turn around the z axis */
        t = sx;
        cosTurn = cosines + 32;
        t = t * (*cosTurn / 0x40);
        sinTurn = sines + 32;
        rx = (t - (*sinTurn / 0x40) * ry) / 0x100;
        pt.x = rx;
        pt.y = (ry * (*cosTurn / 0x40) + (*sinTurn / 0x40) * sx) / 0x100;

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
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}
// Single point whose height (8.8 fixed, negative is up) changes by a constant speed each frame,
// drawn at a fixed scale of 0x60 x 0xC0.
void obj_drift_y_60c0(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 *words;
    s32 height;
    s32 speed;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 i;
    s32 srcX;
    s32 srcY;
    s32 tmp;
    s32 offs;

    words = (s32 *)obj->data;
    height = *words++;
    speed = *words++;
    rec = obj->rec;
    l.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(l.model, pieceCount);
    py = &l.y;
    pz = &l.z;
    cursor = &l.model;
    for (i = 0; i < 1; i++) {
        srcX = *words++;
        l.x = srcX;
        srcY = *words++;
        l.y = srcY;
        l.z = *words++;
        l.y = srcY + height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = l.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.y = tmp + offs;
        tmp = obj->posZ;
        offs = l.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.z = tmp + offs;
        offset_words_camera(&l.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, l.x, l.y, l.z << 14, 0, 0);
        if (!cursorSaved) {
            l.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = l.model;
    }
    height += speed;
    rec->unk4 = 96;
    rec->unk6 = 192;
    words = (s32 *)obj->data;
    *words++ = height;
    *words = speed;
}

/*
 * Effect update: draws 1 piece at its offset from obj->data, raised by a height that grows by a
 * constant velocity every frame (mirrored in x if obj->flags bit 7 is set), at scale 0x30 x 0x60.
 * obj->data: height, velocity, then the piece's x, y, z (8.8). Like obj_drift_y_60c0.
 */
void obj_drift_y_3060(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 *state;
    s32 height;
    s32 velocity;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 i;
    s32 offX;
    s32 offY;
    s32 base;
    s32 ofs;

    state = (s32 *)obj->data;
    height = *state++;
    velocity = *state++;
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
        offY = *state++;
        pt.y = offY;
        pt.z = *state++;
        pt.y = offY + height;

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
    height += velocity;
    rec->unk4 = 48; /* scale x */
    rec->unk6 = 96; /* scale y */
    state = (s32 *)obj->data;
    *state++ = height;
    *state = velocity;
}

// Effect update (same as obj_drift_y_60c0 with the sprite scale 0x18 x 0x30): draws one sprite piece at the
// object's position plus its 8.8 offset (x mirrored when flag 0x80 is set) raised by a drift that grows by a
// constant speed each frame. State in obj->data: the drift, its speed, then the piece's x, y, z.
void obj_drift_y_1830(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 *state;
    s32 drift;
    s32 driftSpeed;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 piece;
    s32 offsetX;
    s32 offsetY;
    s32 base;
    s32 offset;

    state = (s32 *)obj->data;
    drift = *state++;
    driftSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
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
        pt.y = offsetY + drift;
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
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    drift += driftSpeed;
    rec->unk4 = 24;
    rec->unk6 = 48;
    state = (s32 *)obj->data;
    *state++ = drift;
    *state = driftSpeed;
}

/*
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * (mirrored in X when flag 0x80 is set), pushed down by a fall distance whose
 * speed grows by a gravity of 0x40 each frame; scaled to 0x60 x 0xC0. State in
 * obj->data: fall distance, fall speed, then the sprite's offset (x, y, z).
 */
void obj_fall_y_60c0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 *state;
    s32 fall;
    s32 fallSpeed;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 i;
    s32 offX;
    s32 offY;
    s32 base;
    s32 ofs;

    state = (s32 *)obj->data;
    fall = *state++;
    fallSpeed = *state++;
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
        offY = *state++;
        pt.y = offY;
        pt.z = *state++;
        pt.y = offY + fall;
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
    fall += fallSpeed;
    fallSpeed += 0x40;
    rec->unk4 = 96;
    rec->unk6 = 192;
    state = (s32 *)obj->data;
    *state++ = fall;
    *state = fallSpeed;
}

/*
 * Effect update, as obj_fall_y_60c0 but scaled to 0x30 x 0x60: draws one sprite at its 8.8
 * fixed-point offset from the object (mirrored in X when flag 0x80 is set), pushed down by a fall
 * distance whose speed grows by a gravity of 0x40 each frame. State in obj->data: fall distance,
 * fall speed, then the sprite's offset (x, y, z).
 */
void obj_fall_y_3060(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 *state;
    s32 fall;
    s32 fallSpeed;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 i;
    s32 offX;
    s32 offY;
    s32 base;
    s32 ofs;

    state = (s32 *)obj->data;
    fall = *state++;
    fallSpeed = *state++;
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
        offY = *state++;
        pt.y = offY;
        pt.z = *state++;
        pt.y = offY + fall;
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
    fall += fallSpeed;
    fallSpeed += 0x40;
    rec->unk4 = 48;
    rec->unk6 = 96;
    state = (s32 *)obj->data;
    *state++ = fall;
    *state = fallSpeed;
}

/*
 * Effect update (same as obj_fall_y_60c0, scaled to 0x18 x 0x30): draws one
 * sprite at its 8.8 fixed-point offset from the object, pushed down by a fall
 * distance whose speed grows by a gravity of 0x40 each frame. State in
 * obj->data: fall distance, fall speed, then the sprite's offset (x, y, z).
 */
void obj_fall_y_1830(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 *state;
    s32 fall;
    s32 fallSpeed;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 i;
    s32 offX;
    s32 offY;
    s32 base;
    s32 ofs;

    state = (s32 *)obj->data;
    fall = *state++;
    fallSpeed = *state++;
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
        offY = *state++;
        pt.y = offY;
        pt.z = *state++;
        pt.y = offY + fall;
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
    fall += fallSpeed;
    fallSpeed += 0x40;
    rec->unk4 = 24;
    rec->unk6 = 48;
    state = (s32 *)obj->data;
    *state++ = fall;
    *state = fallSpeed;
}
