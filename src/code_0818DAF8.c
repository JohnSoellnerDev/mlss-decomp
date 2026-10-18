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

void spawn_tbl_65f8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xC, obj_fall_accel_fc78, 0x4184, 0, 0, 1, flag,
                 flag, gPointModelStartFallAccelFc78, 2);
}

void spawn_tbl_65f4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x41, obj_rise_oam_fb70, 0x40DC, 0, 0, 1, flag,
                 flag, gUnk_085065F4, 1);
}

void spawn_tbl_65f0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 9, obj_sink_oam_fa68, 0x4182, 0, 0, 1, flag,
                 flag, gPointModelStartSinkOamFa68, 1);
}

void spawn_tbl_65e8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x79, obj_glide_x_f958, 0x400C, 5, 0, 1, flag,
                 flag, gPointModelStartGlideXF958, 2);
}

void spawn_tbl_65dc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_tilt4_f6dc, 0x415C, 5, 0, 4, flag,
                 0, gPointModelStartZoomTilt4F6dc, 3);
}

void spawn_tbl_65c4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, obj_accel_tilt5_f4e4, 0x4165, 0, 0, 5, flag,
                 flag, gPointModelStartAccelTilt5F4e4, 6);
}

void spawn_tbl_65b4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_f3c4, 0x4159, 5, 0, 1, 0,
                 0, gPointModelStartDriftPointF3c4, 4);
}

void spawn_tbl_65a4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_f2a4, 0x4159, 5, 0, 1, 0,
                 0, gPointModelStartDriftPointF2a4, 4);
}

void spawn_tbl_659c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_rotx_zoom12_f0c0, 0x415B, 0, 0, 0xC, flag,
                 0, gPointModelStartRotxZoom12F0c0, 2);
}

void spawn_tbl_6594(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_turn_zoom12_ee84, 0x415B, 1, 0, 0xC, flag,
                 0, gPointModelStartTurnZoom12Ee84, 2);
}

void spawn_tbl_6588(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_angle_zoom8_ec14, 0x415B, 0, 0, 8, flag,
                 0, gPointModelStartAngleZoom8Ec14, 3);
}

void spawn_tbl_657c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_angle_zoom4_e998, 0x4159, 0, 0, 4, flag,
                 0, gPointModelStartAngleZoom4E998, 3);
}

void spawn_tbl_6570(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_angle_zoom4_e71c, 0x415B, 0, 0, 4, flag,
                 0, gPointModelStartAngleZoom4E71c, 3);
}

void spawn_tbl_6558(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_accel_tilt9_e4f0, 0x417D, 0, 0, 9, flag,
                 flag, gPointModelStartAccelTilt9E4f0, 6);
}

void spawn_tbl_6540(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_accel_tilt9_e2c4, 0x417C, 0, 0, 9, flag,
                 flag, gPointModelStartAccelTilt9E2c4, 6);
}

void spawn_tbl_6534(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_perm_spin8_e08c, 0x4175, 0, 0, 8, 0,
                 flag, gPermSpin8DataE08c, 3);
}

void spawn_cb_df7d(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_fixed_point_df7c, 0x415B, 5, 0, 1, flag,
                 0, 0, 0);
}

void spawn_cb_de6d(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_fixed_point_de6c, 0x4159, 5, 0, 1, flag,
                 0, 0, 0);
}

void spawn_tbl_6528(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_model8_dbf0, 0x415B, 1, 0, 8, flag,
                 0, gZoomModel8DataDbf0, 3);
}

void spawn_tbl_6518(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x17, obj_spin_model4_d990, 0x4175, 0, 0, 4, 0,
                 flag, gSpinModel4DataD990, 4);
}

void spawn_tbl_6500(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, obj_tilt_model5_d798, 0x4172, 3, 0, 5, flag,
                 flag, gTiltModel5DataD798, 6);
}

void spawn_tbl_64e8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, obj_tilt_model5_d5a0, 0x4172, 3, 0, 5, flag,
                 flag, gTiltModel5DataD5a0, 6);
}

void spawn_tbl_64e4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_sized_point_d494, 0x4159, 4, 0, 1, flag,
                 0, gSizedPointDataD494, 1);
}

void spawn_tbl_64dc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_x_point_d37c, 0x415B, 0, 0, 1, flag,
                 0, gDriftXPointDataD37c, 2);
}

void spawn_tbl_64cc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_d248, 0x4159, 0, 0, 1, flag,
                 0, gDriftPointDataD248, 4);
}

void spawn_tbl_64bc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_d114, 0x4159, 0, 0, 1, flag,
                 0, gDriftPointDataD114, 4);
}

void spawn_tbl_64ac(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_cfe0, 0x415B, 0, 0, 1, flag,
                 0, gDriftPointDataCfe0, 4);
}

void spawn_tbl_649c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_ceac, 0x415B, 0, 0, 1, flag,
                 0, gDriftPointDataCeac, 4);
}

void spawn_tbl_6498(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_slide_x_oam_160_cd9c, 0x4159, 1, 0, 1, flag,
                 0, gUnk_08506498, 1);
}

void spawn_tbl_6494(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_slide_x_oam_a0_cc8c, 0x4159, 1, 0, 1, flag,
                 0, gUnk_08506494, 1);
}

void spawn_tbl_6490(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xC, obj_point_shrink_cb70, 0x416B, 0, 0, 1, flag,
                 flag, gPointShrinkDataCb70, 1);
}

void spawn_cb_ca55(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 3, obj_point_fixed_ca54, 0x416A, 0, 0, 1, flag,
                 flag, 0, 0);
}

void spawn_tbl_6488(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_oam_180_c85c, 0x4159, 1, 0, 8, flag,
                 0, gZoomModel8Oam180DataC85c, 2);
}

void spawn_tbl_6480(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_oam_190_c664, 0x4159, 1, 0, 8, flag,
                 0, gZoomModel8Oam190DataC664, 2);
}

void spawn_tbl_6478(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_c46c, 0x4159, 1, 0, 8, flag,
                 0, gZoomModel8DataC46c, 2);
}

void spawn_tbl_6470(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_c274, 0x415A, 1, 0, 8, flag,
                 0, gZoomModel8DataC274, 2);
}

void spawn_tbl_6468(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_c07c, 0x4159, 1, 0, 8, flag,
                 0, gZoomModel8DataC07c, 2);
}

void spawn_tbl_6460(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt_zoom2_bea8, 0x4159, 0, 0, 2, flag,
                 flag, gZoomTilt2DataBea8, 2);
}

void spawn_tbl_6458(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt_zoom2_bcd4, 0x4159, 0, 0, 2, flag,
                 flag, gZoomTilt2DataBcd4, 2);
}

void spawn_tbl_6450(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt_zoom2_oam_c0_baf4, 0x4159, 0, 0, 2, flag,
                 0, gZoomTilt2OamC0DataBaf4, 2);
}

void spawn_tbl_6448(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt_zoom2_b920, 0x4159, 0, 0, 2, flag,
                 0, gZoomTilt2DataB920, 2);
}

void spawn_tbl_6440(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_model8_zoom_vel9_b708, 0x4175, 0, 0, 8, flag,
                 flag, gZoomModel8Vel9DataB708, 2);
}

void spawn_tbl_6438(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, obj_model8_zoom_vel9_b4f0, 0x4175, 0, 0, 8, flag,
                 flag, gZoomModel8Vel9DataB4f0, 2);
}

void spawn_tbl_6430(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x11, obj_model8_zoom_vel9_b2d8, 0x4175, 0, 0, 8, flag,
                 flag, gZoomModel8Vel9DataB2d8, 2);
}

void spawn_tbl_6418(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, obj_model5_zoom_fall_oam_b0e0, 0x4172, 3, 0, 5, flag,
                 flag, gZoomFallModel5DataB0e0, 6);
}

void spawn_tbl_6408(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, obj_model5_zoom_drop_af10, 0x4175, 0, 0, 5, flag,
                 flag, gStartDataObjModel5ZoomDropAf10, 4);
}

void spawn_tbl_63fc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_vel7_oam_130_acd8, 0x415B, 0, 0, 8, flag,
                 0, gStartDataObjModel8ZoomVel7Oam130Acd8, 3);
}

void spawn_tbl_63f0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_vel7_oam_130_aaa0, 0x415B, 4, 0, 8, flag,
                 0, gStartDataObjModel8ZoomVel7Oam130Aaa0, 3);
}

void spawn_cb_a99d(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_static_oam_160_a99c, 0x415B, 5, 0, 1, flag,
                 0, 0, 0);
}

void spawn_tbl_63e4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_model8_zoom_xslide_a770, 0x4159, 0, 0, 8, flag,
                 0, gStartDataObjModel8ZoomXslideA770, 3);
}

void spawn_tbl_63d8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, zoom_turn_a534, 0x4159, 0, 0, 8, flag,
                 0, gStartDataZoomTurnA534, 3);
}

void spawn_tbl_63cc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, zoom_turn_a2f8, 0x4159, 0, 0, 8, flag,
                 0, gStartDataZoomTurnA2f8, 3);
}

void spawn_tbl_63c0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF, spin_offset_a0cc, 0x4174, 0, 0, 4, 0,
                 flag, gStartDataSpinOffsetA0cc, 3);
}

void spawn_tbl_63b4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF, spin_offset_9ea4, 0x4174, 0, 0, 4, 0,
                 flag, gStartDataSpinOffset9ea4, 3);
}

void spawn_tbl_63ac(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x13, spin_scroll_9c94, 0x4174, 0, 0, 4, 0,
                 flag, gStartDataSpinScroll9c94, 2);
}

void spawn_spin_scroll_9a7c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF, spin_scroll_9a7c, 0x4174, 0, 0, 4, 0,
                 flag, gStartDataSpinScroll9a7c, 2);
}

void spawn_zoom_bounce_97bc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, zoom_bounce_97bc, 0x4173, 4, 0, 4, flag,
                 flag, gStartDataZoomBounce97bc, 7);
}

void spawn_zoom_bounce_94fc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, zoom_bounce_94fc, 0x4173, 4, 0, 4, flag,
                 flag, gStartDataZoomBounce94fc, 7);
}

void spawn_obj_zoom_fixed10_accel_9304(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_zoom_fixed10_accel_9304, 0x4172, 0, 0, 0xA, flag,
                 flag, gStartDataObjZoomFixed10Accel9304, 5);
}

void spawn_obj_zoom_fixed6_accel_910c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x10, obj_zoom_fixed6_accel_910c, 0x4172, 0, 0, 6, flag,
                 flag, gStartDataObjZoomFixed6Accel910c, 6);
}

void spawn_obj_zoom_spin6_shrink_8ea0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x25, obj_zoom_spin6_shrink_8ea0, 0x4165, 0, 0, 6, flag,
                 flag, gStartDataObjZoomSpin6Shrink8ea0, 5);
}

void spawn_obj_zoom_spin6_shrink_8c30(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1B, obj_zoom_spin6_shrink_8c30, 0x4165, 0, 0, 6, flag,
                 flag, gStartDataObjZoomSpin6Shrink8c30, 5);
}

void spawn_obj_zoom_spin6_shrink_89c0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1B, obj_zoom_spin6_shrink_89c0, 0x4165, 0, 0, 6, flag,
                 flag, gStartDataObjZoomSpin6Shrink89c0, 5);
}

void spawn_obj_zoom_spin6_shrink_8754(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1B, obj_zoom_spin6_shrink_8754, 0x4165, 0, 0, 6, flag,
                 flag, gStartDataObjZoomSpin6Shrink8754, 5);
}

void spawn_obj_zoom_spin5_size200_84cc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_zoom_spin5_size200_84cc, 0x4165, 0, 0, 5, flag,
                 flag, gStartDataObjZoomSpin5Size20084cc, 5);
}

void spawn_obj_zoom_fixed5_size100_8254(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_zoom_fixed5_size100_8254, 0x4164, 0, 0, 5, flag,
                 flag, gStartDataObjZoomFixed5Size1008254, 4);
}

void spawn_obj_point_grow_814c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 5, obj_point_grow_814c, 0x4169, 0, 0, 1, flag,
                 flag, gUnk_085062C8, 1);
}

void obj_point1_zoom_step31_ead4(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 pos;
    s32 t;
    s32 zoom;
    s32 step;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 1; i++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = vx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
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
    zoom -= step;
    step = (step * 31) / 32;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}
