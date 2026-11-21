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

extern s16 gCosineTable[];
extern s16 gSineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

/*
 * Effect update: six sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying velocity (2/3 per frame), rotated by a fixed
 * yaw (angle 32) and pitch (angle 64), then drawn around the object
 * (mirrored in X when flag 0x80 is set).
 */
void obj_zoom_model6_tilt45_4b38(struct Point1Obj *obj)
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
        tmp = tmp * (gCosineTable[32] / 0x40);
        yawZ = (tmp - (gSineTable[32] / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * scaledZ) / 0x100;
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
    zoomVel = (zoomVel * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

/*
 * Effect update: eight sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a velocity decaying to 2/3 each frame, rotated by a fixed
 * yaw (angle 32, 45 degrees) and pitch (angle 64), then drawn around the
 * object (mirrored in X when flag 0x80 is set).
 */
void obj_zoom_model8_tilt45_4d74(struct Point1Obj *obj)
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

        /* yaw: rotate in the X/Z plane */
        tmp = scaledZ;
        tmp = tmp * (gCosineTable[32] / 0x40);
        yawZ = (tmp - (gSineTable[32] / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * scaledZ) / 0x100;
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
    zoomVel = (zoomVel * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

/*
 * Effect update: eight sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a velocity decaying to 2/3 each frame, rotated by a fixed
 * yaw (angle 32, 45 degrees) and pitch (angle 64), then drawn around the
 * object (mirrored in X when flag 0x80 is set). Same code as
 * obj_zoom_model8_tilt45_4d74.
 */
void obj_zoom_model8_tilt45_4fb0(struct Point1Obj *obj)
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

        /* yaw: rotate in the X/Z plane */
        tmp = scaledZ;
        tmp = tmp * (gCosineTable[32] / 0x40);
        yawZ = (tmp - (gSineTable[32] / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * scaledZ) / 0x100;
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
    zoomVel = (zoomVel * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}
