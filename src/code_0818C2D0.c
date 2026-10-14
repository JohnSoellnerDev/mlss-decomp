#include "global.h"

// Spawns a point-model object. Parameters 5 to 13 are passed on the stack as one block:
// callback, attribute, mode, a zero word, point count, two flag words, start data, count.
void fx_obj_spawn(u32 x, u32 y, u32 z, u32 type, void (*callback)(void), u32 attr, u32 mode,
                  u32 unk0C, u32 points, u32 flagA, u32 flagB, const void *table, u32 count);

extern void obj_bounce_point(void);
extern void obj_bounce_point_3fb0(void);
extern void obj_bounce_point_40dc(void);
extern void obj_bounce_point_b(void);
extern void obj_bounce_point_plain(void);
extern void obj_bounce_reverse_point(void);
extern void obj_bounce_spin16_point_81d0(void);
extern void obj_bounce_spin16_point_8350(void);
extern void obj_bounce_spin_point_8050(void);
extern void obj_bounce_turn_point(void);
extern void obj_bounce_x_point_8c50(void);
extern void obj_bounce_x_point_8da4(void);
extern void obj_bounce_zoom_model10(void);
extern void obj_bounce_zoom_point_a(void);
extern void obj_bounce_zoom_point_b(void);
extern void obj_damped_zoom_turn_points_3d24(void);
extern void obj_drift_grow_9664(void);
extern void obj_drift_point_9190(void);
extern void obj_drift_shrink_9050(void);
extern void obj_drift_spin_late_shrink_97a4(void);
extern void obj_drift_spin_shrink_84d0(void);
extern void obj_drift_y_1830(void);
extern void obj_drift_y_3060(void);
extern void obj_drift_y_3060_7e24(void);
extern void obj_drift_y_60c0(void);
extern void obj_drift_y_60c0_7d08(void);
extern void obj_drift_y_keep_7f40(void);
extern void obj_fall_accel_00cc(void);
extern void obj_fall_accel_fd84(void);
extern void obj_fall_accel_ffc0(void);
extern void obj_fall_floor_point(void);
extern void obj_fall_point(void);
extern void obj_fall_point_b(void);
extern void obj_fall_spin_point_a(void);
extern void obj_fall_spin_point_b(void);
extern void obj_fall_x_spin_point_8ef8(void);
extern void obj_fall_y_1830(void);
extern void obj_fall_y_3060(void);
extern void obj_fall_y_60c0(void);
extern void obj_fixed_point(void);
extern void obj_fixed_point_b(void);
extern void obj_grow_spin_point_a1d4(void);
extern void obj_grow_spin_points_2180(void);
extern void obj_grow_spin_points_2428(void);
extern void obj_grow_spin_points_26d0(void);
extern void obj_grow_spin_points_2980(void);
extern void obj_grow_spin_shrink_9924(void);
extern void obj_grow_turn_points_3ac0(void);
extern void obj_grow_zoom_point(void);
extern void obj_jump_zoom_turn_points_1cb8(void);
extern void obj_place_point(void);
extern void obj_place_point_b(void);
extern void obj_place_point_c(void);
extern void obj_pulse_model6_b(void);
extern void obj_pulse_zoom_point(void);
extern void obj_rec_x_drift_01d8(void);
extern void obj_rec_x_drift_fe90(void);
extern void obj_rise_turn_points_1f4c(void);
extern void obj_scroll_bounce_point_3978(void);
extern void obj_scroll_point_3858(void);
extern void obj_shrink_y_offset5_88f0(void);
extern void obj_shrink_y_offset5_8a10(void);
extern void obj_shrink_y_offset5_8b30(void);
extern void obj_slide_point(void);
extern void obj_slide_speed_point(void);
extern void obj_spin_hop_rot45_model5(void);
extern void obj_spin_hop_rot90_model5(void);
extern void obj_spin_model5(void);
extern void obj_spin_rise_model12(void);
extern void obj_spin_rise_model3_a(void);
extern void obj_spin_rise_model3_b(void);
extern void obj_spin_rise_model3_c(void);
extern void obj_spin_rise_model3_fast(void);
extern void obj_spin_swap_model5(void);
extern void obj_spin_tilt67_model5(void);
extern void obj_spin_zoom5_863c(void);
extern void obj_spin_zoom5_9f20(void);
extern void obj_spring_turn_points_2c50(void);
extern void obj_spring_turn_points_2eb8(void);
extern void obj_spring_turn_points_3120(void);
extern void obj_spring_turn_points_3388(void);
extern void obj_spring_turn_points_35f0(void);
extern void obj_swing_point(void);
extern void obj_tilt_zoom8_6128(void);
extern void obj_tilt_zoom8_6560(void);
extern void obj_turn_zoom4_5830(void);
extern void obj_turn_zoom4_6310(void);
extern void obj_turn_zoom4_6748(void);
extern void obj_zoom_fall6_0a80(void);
extern void obj_zoom_quarter10_1440(void);
extern void obj_zoom_quarter_spin8_0d14(void);
extern void obj_zoom_quarter_spin8_0f78(void);
extern void obj_zoom_quarter_spin8_1804(void);
extern void obj_zoom_spin6_9a84(void);
extern void obj_zoom_spin6_9cc8(void);
extern void obj_zoom_spin8_0308(void);
extern void obj_zoom_spin8_0578(void);
extern void obj_zoom_spin8_07fc(void);
extern void obj_zoom_tilt10_1620(void);
extern void obj_zoom_tilt_model2_92b4(void);
extern void obj_zoom_tilt_model2_948c(void);
extern void obj_zoom_tilt_rise8_11dc(void);
extern void obj_zoom_tilt_rise8_1a5c(void);

extern const u8 gPointModelStartFallAccel00cc[];
extern const u8 gPointModelStartFallAccelFd84[];
extern const u8 gPointModelStartFallAccelFfc0[];
extern const u8 gPointModelStartRecXDrift01d8[];
extern const u8 gPointModelStartRecXDriftFe90[];
extern const u8 gPointModelStartZoomFall60a80[];
extern const u8 gPointModelStartZoomQuarterSpin80d14[];
extern const u8 gPointModelStartZoomQuarterSpin80f78[];
extern const u8 gPointModelStartZoomSpin80308[];
extern const u8 gPointModelStartZoomSpin80578[];
extern const u8 gPointModelStartZoomSpin807fc[];
extern const u8 gSpawnStartBouncePoint[];
extern const u8 gSpawnStartBouncePoint3fb0[];
extern const u8 gSpawnStartBouncePoint40dc[];
extern const u8 gSpawnStartBouncePointB[];
extern const u8 gSpawnStartBouncePointPlain[];
extern const u8 gSpawnStartBounceZoomModel10[];
extern const u8 gSpawnStartBounceZoomPointA[];
extern const u8 gSpawnStartBounceZoomPointB[];
extern const u8 gSpawnStartDampedZoomTurnPoints3d24[];
extern const u8 gSpawnStartFallFloorPoint[];
extern const u8 gSpawnStartJumpZoomTurnPoints1cb8[];
extern const u8 gSpawnStartPulseModel6B[];
extern const u8 gSpawnStartScrollBouncePoint3978[];
extern const u8 gSpawnStartScrollPoint3858[];
extern const u8 gSpawnStartSlidePoint[];
extern const u8 gSpawnStartSpinRiseModel12[];
extern const u8 gSpawnStartSpinRiseModel3A[];
extern const u8 gSpawnStartSpinRiseModel3B[];
extern const u8 gSpawnStartSpinRiseModel3C[];
extern const u8 gSpawnStartSpinRiseModel3Fast[];
extern const u8 gSpawnStartSpringTurnPoints2c50[];
extern const u8 gSpawnStartSpringTurnPoints2eb8[];
extern const u8 gSpawnStartSpringTurnPoints3120[];
extern const u8 gSpawnStartSpringTurnPoints3388[];
extern const u8 gSpawnStartSpringTurnPoints35f0[];
extern const u8 gSpawnStartZoomQuarter101440[];
extern const u8 gSpawnStartZoomQuarterSpin81804[];
extern const u8 gSpawnStartZoomTilt101620[];
extern const u8 gSpawnStartZoomTiltRise811dc[];
extern const u8 gSpawnStartZoomTiltRise81a5c[];
extern const u8 gSpawnStateBounceReverse685c[];
extern const u8 gSpawnStateBounceTurn68cc[];
extern const u8 gSpawnStateDriftY680c[];
extern const u8 gSpawnStateDriftY6814[];
extern const u8 gSpawnStateDriftY681c[];
extern const u8 gSpawnStateDriftY6914[];
extern const u8 gSpawnStateDriftY691c[];
extern const u8 gSpawnStateFallSpinA68e4[];
extern const u8 gSpawnStateFallSpinB68f8[];
extern const u8 gSpawnStateFallY6824[];
extern const u8 gSpawnStateFallY682c[];
extern const u8 gSpawnStateFallY6834[];
extern const u8 gSpawnStateGrowZoom6800[];
extern const u8 gSpawnStatePulseZoom67f8[];
extern const u8 gSpawnStateSlideSpeed690c[];
extern const u8 gSpawnStateSpinHop456890[];
extern const u8 gSpawnStateSpinHop90687c[];
extern const u8 gSpawnStateSpinSwap68a4[];
extern const u8 gSpawnStateSpinTilt6768b8[];
extern const u8 gSpawnStateSwing686c[];
extern const u8 gSpawnStateTiltZoom8683c[];
extern const u8 gSpawnStateTiltZoom8684c[];
extern const u8 gSpawnStateTurnZoom46804[];
extern const u8 gSpawnStateTurnZoom46844[];
extern const u8 gSpawnStateTurnZoom46854[];
extern const u8 gStartObjBounceSpin16Point81d0[];
extern const u8 gStartObjBounceSpin16Point8350[];
extern const u8 gStartObjBounceSpinPoint8050[];
extern const u8 gStartObjBounceXPoint8c50[];
extern const u8 gStartObjBounceXPoint8da4[];
extern const u8 gStartObjDriftGrow9664[];
extern const u8 gStartObjDriftPoint9190[];
extern const u8 gStartObjDriftShrink9050[];
extern const u8 gStartObjDriftSpinLateShrink97a4[];
extern const u8 gStartObjDriftSpinShrink84d0[];
extern const u8 gStartObjDriftYKeep7f40[];
extern const u8 gStartObjFallPoint[];
extern const u8 gStartObjFallPointB[];
extern const u8 gStartObjFallXSpinPoint8ef8[];
extern const u8 gStartObjGrowSpinShrink9924[];
extern const u8 gStartObjShrinkYOffset588f0[];
extern const u8 gStartObjShrinkYOffset58a10[];
extern const u8 gStartObjShrinkYOffset58b30[];
extern const u8 gStartObjSpinModel5[];
extern const u8 gStartObjSpinZoom5863c[];
extern const u8 gStartObjSpinZoom59f20[];
extern const u8 gStartObjZoomSpin69a84[];
extern const u8 gStartObjZoomSpin69cc8[];
extern const u8 gStartObjZoomTiltModel292b4[];
extern const u8 gStartObjZoomTiltModel2948c[];
extern const u8 gSpawnStartRiseTurnPoints1f4c[];
extern const u8 gSpawnStartGrowSpinPoints2180[];
extern const u8 gSpawnStartGrowSpinPoints2428[];
extern const u8 gSpawnStartGrowSpinPoints26d0[];
extern const u8 gSpawnStartGrowSpinPoints2980[];
extern const u8 gSpawnStartGrowTurnPoints3ac0[];
extern const u8 gSpawnStartGrowSpinPointA1d4[];

void spawn_tbl_6b1c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_pulse_model6_b, 0x4159, 1, 0, 6, flag, 0,
                 gSpawnStartPulseModel6B, 2);
}

void spawn_tbl_6b04(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x97, obj_bounce_point_b, 0x403F, 0, 0, 1, flag, flag,
                 gSpawnStartBouncePointB, 6);
}

void spawn_tbl_6afc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_fall_floor_point, 0x403F, 0, 0, 1, flag, flag,
                 gSpawnStartFallFloorPoint, 2);
}

void spawn_cb_ae89(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_place_point_c, 0x417A, 0, 0, 1, flag, flag,
                 0, 0);
}

void spawn_cb_ad95(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_place_point_b, 0x4179, 0, 0, 1, flag, flag,
                 0, 0);
}

void spawn_cb_aca1(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_place_point, 0x4178, 0, 0, 1, flag, flag,
                 0, 0);
}

void spawn_cb_ab9d(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_fixed_point_b, 0x4179, 0, 0, 1, flag, flag,
                 0, 0);
}

void spawn_cb_aa99(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_fixed_point, 0x4178, 0, 0, 1, flag, flag,
                 0, 0);
}

void spawn_tbl_6af8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 9, obj_slide_point, 0x416B, 1, 0, 1, flag, flag,
                 gSpawnStartSlidePoint, 1);
}

void spawn_tbl_6ae0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x65, obj_bounce_point, 0x4043, 0, 0, 1, flag, flag,
                 gSpawnStartBouncePoint, 6);
}

void spawn_tbl_6ad0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x79, obj_fall_point_b, 0x400B, 1, 0, 1, flag, flag,
                 gStartObjFallPointB, 4);
}

void spawn_tbl_6ac0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x79, obj_fall_point, 0x4002, 1, 0, 1, flag, flag,
                 gStartObjFallPoint, 4);
}

void spawn_tbl_6aac(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x21, obj_spin_model5, 0x4177, 0, 0, 5, flag, flag,
                 gStartObjSpinModel5, 5);
}

void spawn_tbl_6aa4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_grow_spin_point_a1d4, 0x4040, 0, 0, 1, flag, flag,
                 gSpawnStartGrowSpinPointA1d4, 2);
}

void spawn_tbl_6a90(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x21, obj_spin_zoom5_9f20, 0x4176, 0, 0, 5, flag, flag,
                 gStartObjSpinZoom59f20, 5);
}

void spawn_tbl_6a80(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_zoom_spin6_9cc8, 0x4167, 0, 0, 6, flag, flag,
                 gStartObjZoomSpin69cc8, 4);
}

void spawn_tbl_6a70(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x19, obj_zoom_spin6_9a84, 0x4167, 0, 0, 6, flag, flag,
                 gStartObjZoomSpin69a84, 4);
}

void spawn_tbl_6a60(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x45, obj_grow_spin_shrink_9924, 0x4168, 0, 0, 1, flag, flag,
                 gStartObjGrowSpinShrink9924, 4);
}

void spawn_tbl_6a44(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_drift_spin_late_shrink_97a4, 0x4165, 0, 0, 1, flag, 0,
                 gStartObjDriftSpinLateShrink97a4, 7);
}

void spawn_tbl_6a30(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x14, obj_drift_grow_9664, 0x415F, 1, 0, 1, flag, 0,
                 gStartObjDriftGrow9664, 5);
}

void spawn_tbl_6a28(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_tilt_model2_948c, 0x415B, 0, 0, 2, flag, 0,
                 gStartObjZoomTiltModel2948c, 2);
}

void spawn_tbl_6a20(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_tilt_model2_92b4, 0x4159, 0, 0, 2, flag, 0,
                 gStartObjZoomTiltModel292b4, 2);
}

void spawn_tbl_6a10(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_drift_point_9190, 0x415A, 0, 0, 1, flag, 0,
                 gStartObjDriftPoint9190, 4);
}

void spawn_tbl_69fc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x11, obj_drift_shrink_9050, 0x415F, 1, 0, 1, flag, 0,
                 gStartObjDriftShrink9050, 5);
}

void spawn_tbl_69e8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x29, obj_fall_x_spin_point_8ef8, 0x40B0, 1, 0, 1, flag, flag,
                 gStartObjFallXSpinPoint8ef8, 5);
}

void spawn_tbl_69d0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x3D, obj_bounce_x_point_8da4, 0x40C8, 8, 0, 1, flag, flag,
                 gStartObjBounceXPoint8da4, 6);
}

void spawn_tbl_69b8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x3D, obj_bounce_x_point_8c50, 0x40C6, 8, 0, 1, flag, flag,
                 gStartObjBounceXPoint8c50, 6);
}

void spawn_tbl_69b0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 9, obj_shrink_y_offset5_8b30, 0x40E1, 1, 0, 5, flag, flag,
                 gStartObjShrinkYOffset58b30, 2);
}

void spawn_tbl_69a8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 9, obj_shrink_y_offset5_8a10, 0x40E0, 1, 0, 5, flag, flag,
                 gStartObjShrinkYOffset58a10, 2);
}

void spawn_tbl_69a0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 9, obj_shrink_y_offset5_88f0, 0x40E6, 1, 0, 5, flag, flag,
                 gStartObjShrinkYOffset588f0, 2);
}

void spawn_tbl_698c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_spin_zoom5_863c, 0x4175, 0, 0, 5, flag, flag,
                 gStartObjSpinZoom5863c, 5);
}

void spawn_tbl_6974(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_drift_spin_shrink_84d0, 0x40AA, 0x2E, 0, 1, flag, flag,
                 gStartObjDriftSpinShrink84d0, 6);
}

void spawn_tbl_695c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x65, obj_bounce_spin16_point_8350, 0x40AD, 0, 0, 1, flag, flag,
                 gStartObjBounceSpin16Point8350, 6);
}

void spawn_tbl_6944(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x65, obj_bounce_spin16_point_81d0, 0x40AC, 0, 0, 1, flag, flag,
                 gStartObjBounceSpin16Point81d0, 6);
}

void spawn_tbl_692c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x65, obj_bounce_spin_point_8050, 0x40AF, 0, 0, 1, flag, flag,
                 gStartObjBounceSpinPoint8050, 6);
}

void spawn_tbl_6924(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1F, obj_drift_y_keep_7f40, 0x40F4, 0, 0, 1, flag, 0,
                 gStartObjDriftYKeep7f40, 2);
}

void spawn_tbl_691c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_drift_y_3060_7e24, 0x4183, 0, 0, 1, flag, 0,
                 gSpawnStateDriftY691c, 2);
}

void spawn_tbl_6914(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_drift_y_60c0_7d08, 0x4183, 0, 0, 1, flag, 0,
                 gSpawnStateDriftY6914, 2);
}

void spawn_tbl_690c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_slide_speed_point, 0x4183, 0, 0, 1, flag, 0,
                 gSpawnStateSlideSpeed690c, 2);
}

void spawn_tbl_68f8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x33, obj_fall_spin_point_b, 0x4079, 0, 0, 1, flag, flag,
                 gSpawnStateFallSpinB68f8, 5);
}

void spawn_tbl_68e4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x33, obj_fall_spin_point_a, 0x4079, 0, 0, 1, flag, flag,
                 gSpawnStateFallSpinA68e4, 5);
}

void spawn_tbl_68cc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x65, obj_bounce_turn_point, 0x4110, 0, 0, 1, flag, flag,
                 gSpawnStateBounceTurn68cc, 6);
}

void spawn_tbl_68b8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_spin_tilt67_model5, 0x4165, 0, 0, 5, flag, flag,
                 gSpawnStateSpinTilt6768b8, 5);
}

void spawn_tbl_68a4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_spin_swap_model5, 0x4165, 0, 0, 5, flag, flag,
                 gSpawnStateSpinSwap68a4, 5);
}

void spawn_tbl_6890(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_spin_hop_rot45_model5, 0x4165, 0, 0, 5, flag, flag,
                 gSpawnStateSpinHop456890, 5);
}

void spawn_tbl_687c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_spin_hop_rot90_model5, 0x4165, 0, 0, 5, flag, flag,
                 gSpawnStateSpinHop90687c, 5);
}

void spawn_tbl_686c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x51, obj_swing_point, 0x416C, 0, 0, 1, flag, flag,
                 gSpawnStateSwing686c, 4);
}

void spawn_tbl_685c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x51, obj_bounce_reverse_point, 0x416C, 1, 0, 1, flag, flag,
                 gSpawnStateBounceReverse685c, 4);
}

void spawn_tbl_6854(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_turn_zoom4_6748, 0x415B, 1, 0, 4, flag, 0,
                 gSpawnStateTurnZoom46854, 2);
}

void spawn_tbl_684c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt_zoom8_6560, 0x415B, 1, 0, 8, flag, 0,
                 gSpawnStateTiltZoom8684c, 2);
}

void spawn_tbl_6844(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_turn_zoom4_6310, 0x4159, 1, 0, 4, flag, 0,
                 gSpawnStateTurnZoom46844, 2);
}

void spawn_tbl_683c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt_zoom8_6128, 0x4159, 1, 0, 8, flag, 0,
                 gSpawnStateTiltZoom8683c, 2);
}

void spawn_tbl_6834(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x29, obj_fall_y_1830, 0x4183, 0, 0, 1, flag, flag,
                 gSpawnStateFallY6834, 2);
}

void spawn_tbl_682c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x24, obj_fall_y_3060, 0x4183, 0, 0, 1, flag, flag,
                 gSpawnStateFallY682c, 2);
}

void spawn_tbl_6824(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x20, obj_fall_y_60c0, 0x4183, 0, 0, 1, flag, flag,
                 gSpawnStateFallY6824, 2);
}

void spawn_tbl_681c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_drift_y_1830, 0x4183, 0, 0, 1, flag, 0,
                 gSpawnStateDriftY681c, 2);
}

void spawn_tbl_6814(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_drift_y_3060, 0x4183, 0, 0, 1, flag, 0,
                 gSpawnStateDriftY6814, 2);
}

void spawn_tbl_680c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1A, obj_drift_y_60c0, 0x4183, 0, 0, 1, flag, 0,
                 gSpawnStateDriftY680c, 2);
}

void spawn_tbl_6804(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_turn_zoom4_5830, 0x4165, 0, 0, 4, flag, flag,
                 gSpawnStateTurnZoom46804, 2);
}

void spawn_tbl_6800(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 5, obj_grow_zoom_point, 0x40D3, 0, 0, 1, flag, flag,
                 gSpawnStateGrowZoom6800, 1);
}

void spawn_tbl_67f8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x8D, obj_pulse_zoom_point, 0x40D3, 1, 0, 1, flag, flag,
                 gSpawnStatePulseZoom67f8, 2);
}

void spawn_tbl_67e8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_spin_rise_model12, 0x40C4, 0, 0, 0xC, flag, flag,
                 gSpawnStartSpinRiseModel12, 4);
}

void spawn_tbl_67d8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_spin_rise_model3_c, 0x40C4, 0, 0, 3, flag, flag,
                 gSpawnStartSpinRiseModel3C, 4);
}

void spawn_tbl_67c8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_spin_rise_model3_b, 0x40C4, 0, 0, 3, flag, flag,
                 gSpawnStartSpinRiseModel3B, 4);
}

void spawn_tbl_67b8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_spin_rise_model3_a, 0x40C4, 0, 0, 3, flag, flag,
                 gSpawnStartSpinRiseModel3A, 4);
}

void spawn_tbl_67a8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_spin_rise_model3_fast, 0x40C4, 0, 0, 3, flag, flag,
                 gSpawnStartSpinRiseModel3Fast, 4);
}

void spawn_tbl_679c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x29, obj_bounce_point_plain, 0x416D, 0, 0, 1, flag, flag,
                 gSpawnStartBouncePointPlain, 3);
}

void spawn_tbl_6788(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x97, obj_bounce_zoom_model10, 0x416D, 0, 0, 0xA, flag, flag,
                 gSpawnStartBounceZoomModel10, 5);
}

void spawn_tbl_6774(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x83, obj_bounce_zoom_point_b, 0x416D, 0, 0, 1, flag, flag,
                 gSpawnStartBounceZoomPointB, 5);
}

void spawn_tbl_6760(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x83, obj_bounce_zoom_point_a, 0x416E, 0, 0, 1, flag, flag,
                 gSpawnStartBounceZoomPointA, 5);
}

void spawn_tbl_6754(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x97, obj_bounce_point_40dc, 0x416D, 0, 0, 1, flag, flag,
                 gSpawnStartBouncePoint40dc, 3);
}

void spawn_tbl_6748(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x97, obj_bounce_point_3fb0, 0x416D, 0, 0, 1, flag, flag,
                 gSpawnStartBouncePoint3fb0, 3);
}

void spawn_tbl_6738(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_damped_zoom_turn_points_3d24, 0x416C, 0, 0, 3, flag, flag,
                 gSpawnStartDampedZoomTurnPoints3d24, 4);
}

void spawn_tbl_672c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xB5, obj_grow_turn_points_3ac0, 0x416C, 0, 0, 3, flag, flag,
                 gSpawnStartGrowTurnPoints3ac0, 3);
}

void spawn_tbl_671c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x43, obj_scroll_bounce_point_3978, 0x415F, 0, 0, 1, flag, flag,
                 gSpawnStartScrollBouncePoint3978, 4);
}

void spawn_tbl_6714(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x25, obj_scroll_point_3858, 0x415F, 0, 0, 1, flag, flag,
                 gSpawnStartScrollPoint3858, 2);
}

void spawn_tbl_6708(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xA1, obj_spring_turn_points_35f0, 0x415F, 0, 0, 2, flag, flag,
                 gSpawnStartSpringTurnPoints35f0, 3);
}

void spawn_tbl_66fc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xA1, obj_spring_turn_points_3388, 0x415F, 0, 0, 2, flag, flag,
                 gSpawnStartSpringTurnPoints3388, 3);
}

void spawn_tbl_66f0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xA1, obj_spring_turn_points_3120, 0x415F, 0, 0, 2, flag, flag,
                 gSpawnStartSpringTurnPoints3120, 3);
}

void spawn_tbl_66e4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xA1, obj_spring_turn_points_2eb8, 0x415F, 0, 0, 2, flag, flag,
                 gSpawnStartSpringTurnPoints2eb8, 3);
}

void spawn_tbl_66d8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xA1, obj_spring_turn_points_2c50, 0x415F, 0, 0, 2, flag, flag,
                 gSpawnStartSpringTurnPoints2c50, 3);
}

void spawn_tbl_66d4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF1, obj_grow_spin_points_2980, 0x4165, 0, 0, 8, flag, flag,
                 gSpawnStartGrowSpinPoints2980, 1);
}

void spawn_tbl_66d0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF1, obj_grow_spin_points_26d0, 0x415F, 0, 0, 8, flag, flag,
                 gSpawnStartGrowSpinPoints26d0, 1);
}

void spawn_tbl_66cc(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF1, obj_grow_spin_points_2428, 0x415F, 0, 0, 8, flag, flag,
                 gSpawnStartGrowSpinPoints2428, 1);
}

void spawn_tbl_66c8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xF1, obj_grow_spin_points_2180, 0x415E, 0, 0, 0xA, flag, flag,
                 gSpawnStartGrowSpinPoints2180, 1);
}

void spawn_tbl_66c0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x3D, obj_rise_turn_points_1f4c, 0x4165, 0, 0, 2, flag, flag,
                 gSpawnStartRiseTurnPoints1f4c, 2);
}

void spawn_tbl_66ac(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x29, obj_jump_zoom_turn_points_1cb8, 0x4165, 0, 0, 8, flag, flag,
                 gSpawnStartJumpZoomTurnPoints1cb8, 5);
}

void spawn_tbl_66a0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x79, obj_zoom_tilt_rise8_1a5c, 0x4165, 0, 0, 8, flag, flag,
                 gSpawnStartZoomTiltRise81a5c, 3);
}

void spawn_tbl_6694(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x79, obj_zoom_quarter_spin8_1804, 0x4165, 0, 0, 8, flag, flag,
                 gSpawnStartZoomQuarterSpin81804, 3);
}

void spawn_tbl_668c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1F, obj_zoom_tilt10_1620, 0x4165, 0, 0, 0xA, flag, flag,
                 gSpawnStartZoomTilt101620, 2);
}

void spawn_tbl_6684(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1F, obj_zoom_quarter10_1440, 0x4165, 0, 0, 0xA, flag, flag,
                 gSpawnStartZoomQuarter101440, 2);
}

void spawn_tbl_6678(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_zoom_tilt_rise8_11dc, 0x4165, 0, 0, 8, flag, flag,
                 gSpawnStartZoomTiltRise811dc, 3);
}

void spawn_tbl_666c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x47, obj_zoom_quarter_spin8_0f78, 0x4165, 0, 0, 8, flag, flag,
                 gPointModelStartZoomQuarterSpin80f78, 3);
}

void spawn_tbl_6660(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1F, obj_zoom_quarter_spin8_0d14, 0x4165, 0, 0, 8, flag, flag,
                 gPointModelStartZoomQuarterSpin80d14, 3);
}

void spawn_tbl_664c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x15, obj_zoom_fall6_0a80, 0x4165, 0, 0, 6, flag, flag,
                 gPointModelStartZoomFall60a80, 5);
}

void spawn_tbl_6640(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin8_07fc, 0x4159, 0, 0, 8, flag, flag,
                 gPointModelStartZoomSpin807fc, 3);
}

void spawn_tbl_6634(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin8_0578, 0x4159, 0, 0, 8, flag, flag,
                 gPointModelStartZoomSpin80578, 3);
}

void spawn_tbl_6628(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin8_0308, 0x4159, 0, 0, 8, flag, flag,
                 gPointModelStartZoomSpin80308, 3);
}

void spawn_tbl_6620(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x11, obj_rec_x_drift_01d8, 0x4184, 1, 0, 1, flag, flag,
                 gPointModelStartRecXDrift01d8, 2);
}

void spawn_tbl_6618(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xC, obj_fall_accel_00cc, 0x4184, 0, 0, 1, flag, flag,
                 gPointModelStartFallAccel00cc, 2);
}

void spawn_tbl_6610(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xC, obj_fall_accel_ffc0, 0x4184, 1, 0, 1, flag, flag,
                 gPointModelStartFallAccelFfc0, 2);
}

void spawn_tbl_6608(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x11, obj_rec_x_drift_fe90, 0x4184, 0, 0, 1, flag, flag,
                 gPointModelStartRecXDriftFe90, 2);
}

void spawn_tbl_6600(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0xC, obj_fall_accel_fd84, 0x4184, 1, 0, 1, flag, flag,
                 gPointModelStartFallAccelFd84, 2);
}
