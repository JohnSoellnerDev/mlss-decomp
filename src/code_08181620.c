#include "global.h"

struct PtRecord
{
    u8 filler0[0x34];
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

struct PtLocal
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

u32 block_flag_byte(struct PtRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

extern s16 gSineTable[];
extern s16 gCosineTable[];

NONMATCH("asm/code_08181620/obj_zoom_tilt10_1620.s", void obj_zoom_tilt10_1620(struct PtObj *obj))
{
    struct PtLocal l;
    struct PtRecord *rec;
    s32 zoom;
    s32 speed;
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
    speed = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 10; i++) {
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
        t = z1;
        t = t * (gCosineTable[12] / 0x40);
        l.z = (t + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

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
    zoom += speed;
    speed = (speed * 7) / 8;
    words = obj->data;
    *words++ = zoom;
    *words = speed;
}
END_NONMATCH

/*
 * Draws 8 sprites in a ring that spins and shrinks away. obj->data holds a scale, its shrink speed
 * and a spin angle (256 to a turn), then 8 x/y/z points. Each point is scaled by scale/256, tipped
 * a quarter turn so the ring lies flat, spun in the screen plane by the angle, lifted 16 pixels,
 * mirrored when the object is flipped, offset by the object's position and placed on screen
 * through the camera. Each frame the scale drops by the speed, which grows by 0x20, and the angle
 * advances by 3.
 */
void obj_zoom_quarter_spin8_1804(struct PtObj *obj)
{
    struct PtLocal pt;
    struct PtRecord *rec;
    s32 scale;
    s32 shrinkSpeed;
    s32 angle;
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
    s32 ty;
    s32 rx;
    s32 ry;
    s32 tmp;
    s32 offs;

    data = obj->data;
    scale = *data++;
    shrinkSpeed = *data++;
    angle = *data++;
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
        tmp = tmp * (gCosineTable[64] / 0x40);
        ty = (tmp - (gSineTable[64] / 0x40) * sz) / 0x100;
        pt.y = ty;
        pt.z = ((sz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

        /* spin in the screen plane */
        rx = sx;
        rx = rx * (gCosineTable[angle & 0xFF] / 0x40);
        rx = (rx - (gSineTable[angle & 0xFF] / 0x40) * ty) / 0x100;
        pt.x = rx;
        ry = ((ty * (gCosineTable[angle & 0xFF] / 0x40)) + ((gSineTable[angle & 0xFF] / 0x40) * sx)) / 0x100;
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
    scale -= shrinkSpeed;
    shrinkSpeed += 0x20;
    angle += 3;
    data = obj->data;
    *data++ = scale;
    *data++ = shrinkSpeed;
    *data = angle;
}

// Eight-point object that shrinks ever faster while it turns: each point is zoomed (8.8 fixed point),
// turned around the vertical axis by `yaw` (a table angle), tilted by the fixed angle 12, shifted
// along y by the yaw itself and drawn at the object (mirrored when flags bit 7 is set). Every frame
// the zoom drops by a speed that grows by 0x20, and the yaw turns by 3.
void obj_zoom_tilt_rise8_1a5c(struct PtObj *obj)
{
    struct PtLocal pt;
    s16 *cosYaw;
    s16 *sinYaw;
    s16 *cosTilt;
    s16 *sinTilt;
    struct PtRecord *rec;
    s32 zoom;
    s32 shrinkVel;
    s32 yaw;
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
    s32 yawZ;
    s32 yawX;
    s32 tiltY;
    s32 tiltZ;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    shrinkVel = *words++;
    yaw = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 8; i++) {
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
        cosYaw = &gCosineTable[yaw & 0xFF];
        tiltY = tiltY * (*cosYaw / 0x40);
        sinYaw = &gSineTable[yaw & 0xFF];
        yawZ = (tiltY - (*sinYaw / 0x40) * zx) / 0x100;
        pt.z = yawZ;
        yawX = ((zx * (*cosYaw / 0x40)) + ((*sinYaw / 0x40) * zz)) / 0x100;
        pt.x = yawX;
        // ...and tilt it.
        t = zy;
        cosTilt = &gCosineTable[12];
        t = t * (*cosTilt / 0x40);
        sinTilt = &gSineTable[12];
        tiltY = (t - (*sinTilt / 0x40) * yawZ) / 0x100;
        pt.y = tiltY;
        tiltZ = ((yawZ * (*cosTilt / 0x40)) + ((*sinTilt / 0x40) * zy)) / 0x100;
        pt.z = tiltZ;
        pt.y = tiltY + yaw;

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
    zoom -= shrinkVel;
    shrinkVel += 0x20;
    yaw += 3;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrinkVel;
    *words = yaw;
}

// Eight-point object that spreads out while it turns and falls: each point is zoomed (8.8 fixed
// point), turned around the vertical axis by `yaw` (a table angle), tilted by the fixed angle 12,
// lowered by the fall and drawn at the object (mirrored when flags bit 7 is set). Every frame the
// zoom grows by a speed that slows down by 0x14, the yaw turns by 1 and gravity (0x60) speeds up the fall.
void obj_jump_zoom_turn_points_1cb8(struct PtObj *obj)
{
    struct PtLocal pt;
    s16 *cosYaw;
    s16 *sinYaw;
    s16 *cosTilt;
    s16 *sinTilt;
    struct PtRecord *rec;
    s32 zoom;
    s32 zoomVel;
    s32 yaw;
    s32 fallY;
    s32 fallVel;
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
    s32 yawZ;
    s32 yawX;
    s32 tiltY;
    s32 tiltZ;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    zoomVel = *words++;
    yaw = *words++;
    fallY = *words++;
    fallVel = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 8; i++) {
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
        cosYaw = &gCosineTable[yaw & 0xFF];
        tiltY = tiltY * (*cosYaw / 0x40);
        sinYaw = &gSineTable[yaw & 0xFF];
        yawZ = (tiltY - (*sinYaw / 0x40) * zx) / 0x100;
        pt.z = yawZ;
        yawX = ((zx * (*cosYaw / 0x40)) + ((*sinYaw / 0x40) * zz)) / 0x100;
        pt.x = yawX;
        // ...and tilt it.
        t = zy;
        cosTilt = &gCosineTable[12];
        t = t * (*cosTilt / 0x40);
        sinTilt = &gSineTable[12];
        tiltY = (t - (*sinTilt / 0x40) * yawZ) / 0x100;
        pt.y = tiltY;
        tiltZ = ((yawZ * (*cosTilt / 0x40)) + ((*sinTilt / 0x40) * zy)) / 0x100;
        pt.z = tiltZ;
        pt.y = tiltY + fallY;

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
    zoomVel -= 0x14;
    yaw += 1;
    fallY += fallVel;
    fallVel += 0x60; // gravity
    words = obj->data;
    *words++ = zoom;
    *words++ = zoomVel;
    *words++ = yaw;
    *words++ = fallY;
    *words = fallVel;
}

NONMATCH("asm/code_08181620/obj_rise_turn_points_1f4c.s", void obj_rise_turn_points_1f4c(struct PtObj *obj))
{
    struct PtLocal l;
    s16 *cs;
    s16 *sn;
    s16 *sb;
    struct PtRecord *rec;
    s32 *pt;
    s32 height;
    s32 angle;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 z;
    s32 x24;
    s32 y24;
    s32 z24;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 t;
    s32 pos;

    pt = (s32 *)obj->data;
    height = *pt++;
    angle = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 2; i++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        x24 = x * 24;
        l.x = x24;
        y24 = y * 24;
        l.y = y24;
        z24 = z * 24;
        l.z = z24;
        y2 = z24;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sb = gSineTable;
        sn = sb + (angle & 0xFF);
        z1 = (y2 - (*sn / 0x40) * x24) / 0x100;
        l.z = z1;
        x2 = (x24 * (*cs / 0x40) + (*sn / 0x40) * z24) / 0x100;
        l.x = x2;
        t = y24;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (sb[12] / 0x40) * z1) / 0x100;
        l.y = y2;
        t = z1;
        t = t * (gCosineTable[12] / 0x40);
        l.z = (t + ((sb[12] / 0x40) * y24)) / 0x100;
        l.y = y2 + height;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2;
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
    height -= 0x100;
    angle += 0xC;
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt = angle;
}
END_NONMATCH

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2180.s", void obj_grow_spin_points_2180(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2428.s", void obj_grow_spin_points_2428(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_26d0.s", void obj_grow_spin_points_26d0(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2980.s", void obj_grow_spin_points_2980(void))

/*
 * Effect update: two sprites whose offsets (8.8 fixed point) are scaled by a
 * fixed zoom, turned by a yaw angle that swings back and forth like a spring
 * (the angle also lifts them in Y), tilted by a fixed pitch (angle 12), then
 * drawn around the object (mirrored in X when flag 0x80 is set).
 */
void obj_spring_turn_points_2c50(struct PtObj *obj)
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
