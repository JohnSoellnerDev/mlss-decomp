#include "global.h"

// Spawns a point-model object. Parameters 5 to 13 are passed on the stack as one block:
// callback, attribute, mode, a zero word, point count, two flag words, start data, count.
void fx_obj_spawn(u32 x, u32 y, u32 z, u32 type, void (*callback)(void), u32 attr, u32 mode,
                  u32 unk0C, u32 points, u32 flagA, u32 flagB, const void *table, u32 count);

extern void obj_accel_tilt5_f4e4(void);
extern void obj_accel_tilt9_e2c4(void);
extern void obj_accel_tilt9_e4f0(void);
extern void obj_angle_zoom4_e71c(void);
extern void obj_angle_zoom4_e998(void);
extern void obj_angle_zoom8_ec14(void);
extern void obj_drift_point_ceac(void);
extern void obj_drift_point_cfe0(void);
extern void obj_drift_point_d114(void);
extern void obj_drift_point_d248(void);
extern void obj_drift_point_f2a4(void);
extern void obj_drift_point_f3c4(void);
extern void obj_drift_x_point_d37c(void);
extern void obj_fall_accel_fc78(void);
extern void obj_fixed_point_de6c(void);
extern void obj_fixed_point_df7c(void);
extern void obj_glide_x_f958(void);
extern void obj_model5_zoom_drop_af10(void);
extern void obj_model5_zoom_fall_oam_b0e0(void);
extern void obj_model8_zoom_c07c(void);
extern void obj_model8_zoom_c274(void);
extern void obj_model8_zoom_c46c(void);
extern void obj_model8_zoom_oam_180_c85c(void);
extern void obj_model8_zoom_oam_190_c664(void);
extern void obj_model8_zoom_vel7_oam_130_aaa0(void);
extern void obj_model8_zoom_vel7_oam_130_acd8(void);
extern void obj_model8_zoom_vel9_b2d8(void);
extern void obj_model8_zoom_vel9_b4f0(void);
extern void obj_model8_zoom_vel9_b708(void);
extern void obj_model8_zoom_xslide_a770(void);
extern void obj_perm_spin8_e08c(void);
extern void obj_point_fixed_ca54(void);
extern void obj_point_grow_814c(void);
extern void obj_point_shrink_cb70(void);
extern void obj_point_slide_x_oam_160_cd9c(void);
extern void obj_point_slide_x_oam_a0_cc8c(void);
extern void obj_point_static_oam_160_a99c(void);
extern void obj_rise_oam_fb70(void);
extern void obj_rotx_zoom12_f0c0(void);
extern void obj_sink_oam_fa68(void);
extern void obj_sized_point_d494(void);
extern void obj_spin_model4_d990(void);
extern void obj_tilt_model5_d5a0(void);
extern void obj_tilt_model5_d798(void);
extern void obj_tilt_zoom2_b920(void);
extern void obj_tilt_zoom2_bcd4(void);
extern void obj_tilt_zoom2_bea8(void);
extern void obj_tilt_zoom2_oam_c0_baf4(void);
extern void obj_turn_zoom12_ee84(void);
extern void obj_zoom_fixed10_accel_9304(void);
extern void obj_zoom_fixed5_size100_8254(void);
extern void obj_zoom_fixed6_accel_910c(void);
extern void obj_zoom_model8_dbf0(void);
extern void obj_zoom_spin5_size200_84cc(void);
extern void obj_zoom_spin6_shrink_8754(void);
extern void obj_zoom_spin6_shrink_89c0(void);
extern void obj_zoom_spin6_shrink_8c30(void);
extern void obj_zoom_spin6_shrink_8ea0(void);
extern void obj_zoom_tilt4_f6dc(void);
extern void spin_offset_9ea4(void);
extern void spin_offset_a0cc(void);
extern void spin_scroll_9a7c(void);
extern void spin_scroll_9c94(void);
extern void zoom_bounce_94fc(void);
extern void zoom_bounce_97bc(void);
extern void zoom_turn_a2f8(void);
extern void zoom_turn_a534(void);

extern const u8 gDriftPointDataCeac[];
extern const u8 gDriftPointDataCfe0[];
extern const u8 gDriftPointDataD114[];
extern const u8 gDriftPointDataD248[];
extern const u8 gDriftXPointDataD37c[];
extern const u8 gPermSpin8DataE08c[];
extern const u8 gPointModelStartAccelTilt5F4e4[];
extern const u8 gPointModelStartAccelTilt9E2c4[];
extern const u8 gPointModelStartAccelTilt9E4f0[];
extern const u8 gPointModelStartAngleZoom4E71c[];
extern const u8 gPointModelStartAngleZoom4E998[];
extern const u8 gPointModelStartAngleZoom8Ec14[];
extern const u8 gPointModelStartDriftPointF2a4[];
extern const u8 gPointModelStartDriftPointF3c4[];
extern const u8 gPointModelStartFallAccelFc78[];
extern const u8 gPointModelStartGlideXF958[];
extern const u8 gPointModelStartRotxZoom12F0c0[];
extern const u8 gPointModelStartSinkOamFa68[];
extern const u8 gPointModelStartTurnZoom12Ee84[];
extern const u8 gPointModelStartZoomTilt4F6dc[];
extern const u8 gPointShrinkDataCb70[];
extern const u8 gSizedPointDataD494[];
extern const u8 gSpinModel4DataD990[];
extern const u8 gStartDataObjModel5ZoomDropAf10[];
extern const u8 gStartDataObjModel8ZoomVel7Oam130Aaa0[];
extern const u8 gStartDataObjModel8ZoomVel7Oam130Acd8[];
extern const u8 gStartDataObjModel8ZoomXslideA770[];
extern const u8 gStartDataObjZoomFixed10Accel9304[];
extern const u8 gStartDataObjZoomFixed5Size1008254[];
extern const u8 gStartDataObjZoomFixed6Accel910c[];
extern const u8 gStartDataObjZoomSpin5Size20084cc[];
extern const u8 gStartDataObjZoomSpin6Shrink8754[];
extern const u8 gStartDataObjZoomSpin6Shrink89c0[];
extern const u8 gStartDataObjZoomSpin6Shrink8c30[];
extern const u8 gStartDataObjZoomSpin6Shrink8ea0[];
extern const u8 gStartDataSpinOffset9ea4[];
extern const u8 gStartDataSpinOffsetA0cc[];
extern const u8 gStartDataSpinScroll9a7c[];
extern const u8 gStartDataSpinScroll9c94[];
extern const u8 gStartDataZoomBounce94fc[];
extern const u8 gStartDataZoomBounce97bc[];
extern const u8 gStartDataZoomTurnA2f8[];
extern const u8 gStartDataZoomTurnA534[];
extern const u8 gTiltModel5DataD5a0[];
extern const u8 gTiltModel5DataD798[];
extern const u8 gUnk_085062C8[];
extern const u8 gUnk_08506494[];
extern const u8 gUnk_08506498[];
extern const u8 gUnk_085065F4[];
extern const u8 gZoomFallModel5DataB0e0[];
extern const u8 gZoomModel8DataC07c[];
extern const u8 gZoomModel8DataC274[];
extern const u8 gZoomModel8DataC46c[];
extern const u8 gZoomModel8DataDbf0[];
extern const u8 gZoomModel8Oam180DataC85c[];
extern const u8 gZoomModel8Oam190DataC664[];
extern const u8 gZoomModel8Vel9DataB2d8[];
extern const u8 gZoomModel8Vel9DataB4f0[];
extern const u8 gZoomModel8Vel9DataB708[];
extern const u8 gZoomTilt2DataB920[];
extern const u8 gZoomTilt2DataBcd4[];
extern const u8 gZoomTilt2DataBea8[];
extern const u8 gZoomTilt2OamC0DataBaf4[];

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

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

/* Effect update: draws 5 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model5_turn_rem6_ebf4(struct Point1Obj *obj)
{
    s16 *cosp;
    s16 *sinp;
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
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 spinIdx;
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
    for (piece = 0; piece < 5; piece++) {
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
        spinIdx = spin & 0xFF;
        cosp = &gCosineTable[spinIdx];
        rz = rz * (*cosp / 0x40);
        sinp = &gSineTable[spinIdx];
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (gSineTable[64] / 0x40) * rz) / 0x100;
        pt.z = (rz * (gCosineTable[64] / 0x40) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

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

/* Effect update: draws 2 pieces that spread out (scale grows by a speed that slows to 2/3 each
   frame), turned by angle 0 around the vertical axis and tipped over a quarter turn (64/256)
   around the x axis so they lie flat; state is 2 words in obj->data (scale, scale speed),
   followed by each piece's offset (x, y, z). */
void obj_zoom_model2_flat_ee48(struct Point1Obj *obj)
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

/* Effect update: draws 6 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite's scale drops by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model6_turn_rem6_f07c(struct Point1Obj *obj)
{
    s16 *cosp;
    s16 *sinp;
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
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 spinIdx;
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
        spinIdx = spin & 0xFF;
        cosp = &gCosineTable[spinIdx];
        rz = rz * (*cosp / 0x40);
        sinp = &gSineTable[spinIdx];
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (gSineTable[64] / 0x40) * rz) / 0x100;
        pt.z = (rz * (gCosineTable[64] / 0x40) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

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
 * zoom that grows by a velocity decaying to 2/3 each frame, rotated by a fixed
 * yaw (angle 32, 45 degrees) and pitch (angle 64), then drawn around the
 * object (mirrored in X when flag 0x80 is set).
 */
void obj_zoom_model4_turn45_f2d0(struct Point1Obj *obj)
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
