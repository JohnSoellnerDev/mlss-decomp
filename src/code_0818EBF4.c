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

ASM_FUNC("asm/code_0818EBF4/obj_zoom_model5_turn_rem6_ebf4.s", void obj_zoom_model5_turn_rem6_ebf4(void))

ASM_FUNC("asm/code_0818EBF4/obj_zoom_model2_flat_ee48.s", void obj_zoom_model2_flat_ee48(void))

ASM_FUNC("asm/code_0818EBF4/obj_zoom_model6_turn_rem6_f07c.s", void obj_zoom_model6_turn_rem6_f07c(void))

ASM_FUNC("asm/code_0818EBF4/obj_zoom_model4_turn45_f2d0.s", void obj_zoom_model4_turn45_f2d0(void))
