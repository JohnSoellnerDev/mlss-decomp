#include "global.h"

// The affine sprite an effect object draws.
struct PtRecord
{
    u8 filler0[4];
    u16 unk4;  // scale X (0x100 = 1.0)
    u16 unk6;  // scale Y (0x100 = 1.0)
    u8 filler8[0x34 - 0x8];
    u32 model; // the sprite's OAM row list
};

// An effect object that draws copies of its sprite at points of a model.
struct PtObj
{
    u8 filler0[8];
    struct PtRecord *rec;
    u32 *data;  // state words, then the model's points (x, y, z each)
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;   // 0x80: mirrored horizontally
};

struct PtLocal
{
    s32 x;
    s32 y;
    s32 z;
    u32 model;
    u32 saved;
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


/*
 * Draws 2 sprites that bob on a spring. obj->data holds a scale, the spring's displacement and
 * its velocity, then 2 x/y/z points. Each point is scaled by scale/256, turned about the
 * vertical axis by the displacement (as an angle, 256 to a turn), tilted by a fixed angle
 * (12/256 of a turn), raised or lowered by the displacement, mirrored when the object is
 * flipped, offset by the object's position and placed on screen through the camera. Afterwards
 * the spring moves on: the displacement grows by the velocity, which is pulled back by 1/128 of it.
 */
void obj_spring_turn_points_2eb8(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 scale;
    s32 spring;
    s32 springVel;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    scale = *data++;
    spring = *data++;
    springVel = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 2; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        ry = sz;
        cosEntry = &gCosineTable[spring & 0xFF];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[spring & 0xFF];
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
        pt.y = ry + spring;
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
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    spring += springVel;
    springVel -= spring / 0x80;
    data = (s32 *)obj->data;
    *data++ = scale;
    *data++ = spring;
    *data = springVel;
}



/*
 * Draws 2 sprites that bob on a spring (same as obj_spring_turn_points_2eb8). obj->data holds a
 * scale, the spring's displacement and its velocity, then 2 x/y/z points. Each point is scaled by
 * scale/256, turned about the vertical axis by the displacement (as an angle, 256 to a turn),
 * tilted by a fixed angle (12/256 of a turn), raised or lowered by the displacement, mirrored when
 * the object is flipped, offset by the object's position and placed on screen through the camera.
 * Afterwards the spring moves on: the displacement grows by the velocity, which is pulled back by
 * 1/128 of it.
 */
void obj_spring_turn_points_3120(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 scale;
    s32 spring;
    s32 springVel;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    scale = *data++;
    spring = *data++;
    springVel = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 2; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        ry = sz;
        cosEntry = &gCosineTable[spring & 0xFF];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[spring & 0xFF];
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
        pt.y = ry + spring;
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
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    spring += springVel;
    springVel -= spring / 0x80;
    data = (s32 *)obj->data;
    *data++ = scale;
    *data++ = spring;
    *data = springVel;
}


/*
 * Effect update: two sprites whose offsets (8.8 fixed point) are scaled by a
 * fixed zoom, turned by a yaw angle that swings back and forth like a spring
 * (the angle also lifts them in Y), tilted by a fixed pitch (angle 12), then
 * drawn around the object (mirrored in X when flag 0x80 is set).
 * Same as obj_spring_turn_points_2c50.
 */
void obj_spring_turn_points_3388(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *state;
    s32 zoom;
    s32 angle;
    s32 angleVel;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s16 *cosA;
    s16 *sinA;
    s32 offX;
    s32 offY;
    s32 offZ;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 yawX;
    s32 tmpY;
    s32 yawZ;
    s32 tmp;
    s32 ofs;

    state = (s32 *)obj->data;
    zoom = *state++;
    angle = *state++;
    angleVel = *state++;
    pt.model = obj->rec->model;
    rowCount = block_flag_byte(obj->rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 2; i++) {
        offX = *state++;
        pt.x = offX;
        offY = *state++;
        pt.y = offY;
        offZ = *state++;
        pt.z = offZ;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (offY * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (offZ * zoom) / 0x100;
        pt.z = scaledZ;

        /* yaw by the swinging angle: rotate in the X/Z plane */
        tmpY = scaledZ;
        cosA = &gCosineTable[angle & 0xFF];
        tmpY = tmpY * (*cosA / 0x40);
        sinA = &gSineTable[angle & 0xFF];
        yawZ = (tmpY - (*sinA / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosA / 0x40) + (*sinA / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* fixed pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[12] / 0x40);
        tmpY = (tmp - (gSineTable[12] / 0x40) * yawZ) / 0x100;
        pt.y = tmpY;
        pt.z = ((yawZ * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * scaledY)) / 0x100;
        pt.y = tmpY + angle;

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
    /* spring: the angle accelerates back towards 0 */
    angle += angleVel;
    angleVel -= angle / 0x80;
    state = (s32 *)obj->data;
    *state++ = zoom;
    *state++ = angle;
    *state = angleVel;
}


/*
 * Same as obj_spring_turn_points_2eb8: draws 2 sprites bobbing on a spring. obj->data holds a
 * scale, the spring's displacement and its velocity, then 2 x/y/z points; each is scaled, turned
 * by the displacement, tilted by 12/256 of a turn, raised by the displacement, mirrored when the
 * object is flipped, offset by its position and placed through the camera. Then the spring moves on.
 */
void obj_spring_turn_points_35f0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 scale;
    s32 spring;
    s32 springVel;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    scale = *data++;
    spring = *data++;
    springVel = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 2; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        ry = sz;
        cosEntry = &gCosineTable[spring & 0xFF];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[spring & 0xFF];
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
        pt.y = ry + spring;
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
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    spring += springVel;
    springVel -= spring / 0x80;
    data = (s32 *)obj->data;
    *data++ = scale;
    *data++ = spring;
    *data = springVel;
}


/*
 * Draws 1 sprite that drifts sideways while it grows. obj->data holds the drift and the sprite's
 * scale, then 1 x/y/z point. The point is moved by the drift, mirrored when the object is flipped,
 * offset by the object's position and placed on screen through the camera. Afterwards the drift
 * grows by 3 pixels and the scale by 8/256.
 */
void obj_scroll_point_3858(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 drift;
    s32 scale;
    struct PtRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 i;
    s32 srcX;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    drift = *data++;
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
        pt.x = srcX + drift;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = pt.x;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = pt.x;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    drift += 0x300;
    scale += 8;
    rec->unk4 = scale;
    rec->unk6 = scale;
    data = (s32 *)obj->data;
    *data++ = drift;
    *data = scale;
}


/*
 * Draws 1 sprite that drifts sideways while it bounces and grows. obj->data holds the drift, the
 * bounce height and its velocity and the sprite's scale, then 1 x/y/z point. The point is moved by
 * the drift and the height, mirrored when the object is flipped, offset by the object's position
 * and placed on screen through the camera. Afterwards the drift grows by 2 pixels, gravity (0x80)
 * pulls the bounce down, landing (height > 0) kicks it back up at -0x600, and the scale grows by
 * 4/256.
 */
void obj_scroll_bounce_point_3978(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 drift;
    s32 bounce;
    s32 bounceVel;
    s32 scale;
    struct PtRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    drift = *data++;
    bounce = *data++;
    bounceVel = *data++;
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
        srcY = *data++;
        pt.y = srcY;
        pt.z = *data++;
        pt.x = srcX + drift;
        pt.y = srcY + bounce;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = pt.x;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = pt.x;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    drift += 0x200;
    bounce += bounceVel;
    bounceVel += 0x80;
    if (bounce > 0) {
        bounceVel = -0x600;
    }
    scale += 4;
    rec->unk4 = scale;
    rec->unk6 = scale;
    data = (s32 *)obj->data;
    *data++ = drift;
    *data++ = bounce;
    *data++ = bounceVel;
    *data = scale;
}


/*
 * Draws 3 sprites that grow, spin and sink. obj->data holds a scale, a turn angle (256 to a turn)
 * and a height, then 3 x/y/z points. Each point is scaled by scale/256, turned about the vertical
 * axis, tilted by 12/256 of a turn, raised by the height, mirrored when the object is flipped,
 * offset by the object's position and placed on screen through the camera. Each frame the scale
 * grows by 1.0, the angle by 4 and the height drops by 1.0.
 */
void obj_grow_turn_points_3ac0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 *data;
    s32 scale;
    s32 angle;
    s32 height;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    scale = *data++;
    angle = *data++;
    height = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 3; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        ry = sz;
        cosEntry = &gCosineTable[angle & 0xFF];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[angle & 0xFF];
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
        pt.y = ry + height;
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
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    scale += 0x100;
    angle += 4;
    height -= 0x100;
    data = (s32 *)obj->data;
    *data++ = scale;
    *data++ = angle;
    *data = height;
}



/* Effect update: draws 3 pieces that spread out (scale grows by a step that shrinks to 7/8 each frame),
   spin around the vertical axis, are tilted by a fixed angle (12/256) and rise steadily;
   state is 4 words in obj->data. */
void obj_damped_zoom_turn_points_3d24(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s16 *cosp;
    s16 *sinp;
    s32 *state;
    s32 scale;
    s32 scaleStep;
    s32 yaw;
    s32 yOffset;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 x;
    s32 y;
    s32 z;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 t;
    s32 pos;

    state = (s32 *)obj->data;
    scale = *state++;
    scaleStep = *state++;
    yaw = *state++;
    yOffset = *state++;
    pt.model = obj->rec->model;
    attr = block_flag_byte(obj->rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    /* each piece: its offset (x, y, z) follows the state words */
    for (piece = 0; piece < 3; piece++) {
        x = *state++;
        pt.x = x;
        y = *state++;
        pt.y = y;
        z = *state++;
        pt.z = z;
        sx = (x * scale) / 0x100;
        pt.x = sx;
        sy = (y * scale) / 0x100;
        pt.y = sy;
        sz = (z * scale) / 0x100;
        pt.z = sz;
        /* spin around the vertical axis */
        ry = sz;
        cosp = &gCosineTable[yaw & 0xFF];
        ry = ry * (*cosp / 0x40);
        sinp = &gSineTable[yaw & 0xFF];
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
    yaw += 8;
    yOffset -= 0x200;
    state = (s32 *)obj->data;
    *state++ = scale;
    *state++ = scaleStep;
    *state++ = yaw;
    *state = yOffset;
}



/*
 * Draws 1 sprite that bounces to rest. obj->data holds the bounce height (negative is up), its
 * velocity and a launch counter (counts up to 0), then 1 x/y/z point. The point is raised by the
 * height, mirrored when the object is flipped, offset by the object's position and placed on
 * screen through the camera. Afterwards gravity (0x40) pulls the velocity down; on landing the
 * sprite is launched again at counter * 256, each bounce lower, until the counter reaches 0.
 */
void obj_bounce_point_3fb0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 height;
    s32 velocity;
    s32 launch;
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

    data = (s32 *)obj->data;
    height = *data++;
    velocity = *data++;
    launch = *data++;
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
        pt.y = srcY + height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    height += velocity;
    velocity += 0x40;
    if (height > 0) {
        height = 0;
        launch++;
        if (launch > 0) {
            launch = 0;
        }
        velocity = launch << 8;
    }
    data = (s32 *)obj->data;
    *data++ = height;
    *data++ = velocity;
    *data = launch;
}


/*
 * Draws 1 sprite that bounces to a stop (same as obj_bounce_point_3fb0). obj->data holds a height,
 * its velocity and the next rebound (negative, in 1/256 of the velocity's unit), then 1 x/y/z point.
 * The point is raised by the height, mirrored when the object is flipped, offset by the object's
 * position and placed on screen through the camera. Afterwards the height moves by the velocity,
 * which gravity pulls down by 0x40; on landing (height > 0) the rebound weakens by 1 (stopping at 0)
 * and kicks the velocity back up to rebound * 256.
 */
void obj_bounce_point_40dc(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 height;
    s32 heightVel;
    s32 rebound;
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

    data = (s32 *)obj->data;
    height = *data++;
    heightVel = *data++;
    rebound = *data++;
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
        pt.y = srcY + height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    height += heightVel;
    heightVel += 0x40;
    if (height > 0) {
        height = 0;
        rebound++;
        if (rebound > 0) {
            rebound = 0;
        }
        heightVel = rebound << 8;
    }
    data = (s32 *)obj->data;
    *data++ = height;
    *data++ = heightVel;
    *data = rebound;
}


/*
 * Draws 1 sprite that bounces while it zooms. obj->data holds a scale, its per-frame step, a
 * height above the ground (negative is up), the falling velocity and a bounce counter (negative,
 * counting up to 0), then 1 x/y/z point. The point is scaled by scale/256, tilted by a fixed angle
 * (12/256 of a turn), raised by the height, mirrored when the object is flipped, offset by the
 * object's position and placed on screen through the camera. Afterwards the scale grows by its
 * step and the height falls under gravity (0x40 per frame); on landing the zoom step halves and
 * the sprite bounces back up with counter * 256, until the counter reaches 0.
 */
void obj_bounce_zoom_point_a(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 scale;
    s32 scaleStep;
    s32 height;
    s32 fallVel;
    s32 bounce;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    scale = *data++;
    scaleStep = *data++;
    height = *data++;
    fallVel = *data++;
    bounce = *data++;
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
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (gCosineTable[12] / 0x40);
        ry = (tmp - (gSineTable[12] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry + height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = sx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = sx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        /* landed: bounce back up, a little less each time */
        scaleStep = scaleStep / 2;
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        fallVel = bounce << 8;
    }
    data = (s32 *)obj->data;
    *data++ = scale;
    *data++ = scaleStep;
    *data++ = height;
    *data++ = fallVel;
    *data = bounce;
}


// Bouncing effect: draws the sprite at one model point, zoomed and tilted by angle 12,
// at a height that falls under gravity. Each time it lands it bounces back up a little
// less high and zooms half as fast, until it comes to rest.
void obj_bounce_zoom_point_b(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *words;
    s32 zoom;
    s32 zoom_speed;
    s32 height;
    s32 fall_speed;
    s32 bounce_speed;
    u32 row_count;
    u32 cursor_taken;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 x;
    s32 y;
    s32 z;
    s32 zoomed_x;
    s32 zoomed_y;
    s32 zoomed_z;
    s32 tilted_y;
    s32 t;
    s32 d;

    words = (s32 *)obj->data;
    zoom = *words++;
    zoom_speed = *words++;
    height = *words++;
    fall_speed = *words++;
    bounce_speed = *words++;
    pt.model = obj->rec->model;
    row_count = block_flag_byte(obj->rec);
    cursor_taken = 0;
    oam_list_rows_b(pt.model, row_count);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        x = *words++;
        pt.x = x;
        y = *words++;
        pt.y = y;
        z = *words++;
        pt.z = z;

        // Zoom the point (8.8 fixed point), tilt it around the X axis, lift it.
        zoomed_x = (x * zoom) / 0x100;
        pt.x = zoomed_x;
        zoomed_y = (y * zoom) / 0x100;
        pt.y = zoomed_y;
        zoomed_z = (z * zoom) / 0x100;
        pt.z = zoomed_z;
        t = zoomed_y;
        t = t * (gCosineTable[12] / 0x40);
        tilted_y = (t - (gSineTable[12] / 0x40) * zoomed_z) / 0x100;
        pt.y = tilted_y;
        pt.z = ((zoomed_z * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * zoomed_y)) / 0x100;
        pt.y = tilted_y + height;

        // Place it around the object, mirrored if the object faces left.
        if (obj->flags & 0x80) {
            t = obj->posX;
            d = zoomed_x;
            if (d < 0) {
                d += 0xFF;
            }
            d >>= 8;
            pt.x = t - d;
        } else {
            t = obj->posX;
            d = zoomed_x;
            if (d < 0) {
                d += 0xFF;
            }
            d >>= 8;
            pt.x = t + d;
        }
        t = obj->posY;
        d = pt.y;
        if (d < 0) {
            d += 0xFF;
        }
        d >>= 8;
        pt.y = t + d;
        t = obj->posZ;
        d = pt.z;
        if (d < 0) {
            d += 0xFF;
        }
        d >>= 8;
        pt.z = t + d;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, row_count, pt.x, pt.y, pt.z << 14, 0, 0);

        // The first copy uses the sprite's own OAM rows, any others take free ones.
        if (!cursor_taken) {
            pt.model = gModelCtx->model;
            cursor_taken = 1;
        }
    }
    if (cursor_taken) {
        gModelCtx->model = pt.model;
    }

    zoom += zoom_speed;
    height += fall_speed;
    fall_speed += 0x40;
    // Landed (heights are negative upwards): bounce with the next, weaker speed.
    if (height > 0) {
        zoom_speed = zoom_speed / 2;
        height = 0;
        bounce_speed++;
        if (bounce_speed > 0) {
            bounce_speed = 0;
        }
        fall_speed = bounce_speed << 8;
    }

    words = (s32 *)obj->data;
    *words++ = zoom;
    *words++ = zoom_speed;
    *words++ = height;
    *words++ = fall_speed;
    *words = bounce_speed;
}



/*
 * Draws 10 sprites that bounce while they zoom. obj->data holds a scale, its per-frame step, a
 * height above the ground (negative is up), the falling velocity and a bounce counter (negative,
 * counting up to 0), then 10 x/y/z points. Each point is scaled by scale/256, tilted by a fixed
 * angle (12/256 of a turn), raised by the height, mirrored when the object is flipped, offset by
 * the object's position and placed on screen through the camera. Afterwards the scale grows by its
 * step and the height falls under gravity (0x40 per frame); on landing the zoom step halves and
 * the sprites bounce back up with counter * 256, until the counter reaches 0.
 */
void obj_bounce_zoom_model10(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *data;
    s32 scale;
    s32 scaleStep;
    s32 height;
    s32 fallVel;
    s32 bounce;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 tmp;
    s32 offs;

    data = (s32 *)obj->data;
    scale = *data++;
    scaleStep = *data++;
    height = *data++;
    fallVel = *data++;
    bounce = *data++;
    pt.model = obj->rec->model;
    pieceCount = block_flag_byte(obj->rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 10; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (gCosineTable[12] / 0x40);
        ry = (tmp - (gSineTable[12] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry + height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = sx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = sx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
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
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        /* landed: bounce back up, a little less each time */
        scaleStep = scaleStep / 2;
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        fallVel = bounce << 8;
    }
    data = (s32 *)obj->data;
    *data++ = scale;
    *data++ = scaleStep;
    *data++ = height;
    *data++ = fallVel;
    *data = bounce;
}


