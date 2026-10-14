#include "global.h"

// Spawns an effect object: the first three values are the position, the fourth is the kind;
// the rest goes on the stack.
void fx_obj_spawn(u32 p0, u32 p1, u32 p2, u32 kind, void (*callback)(), u32 id, u32 variant,
                  u32 zero, u32 count, u32 flagA, u32 flagB, const void *table, u32 words);

extern u8 gSpawnStartPoint1Diff7a90[];
extern u32 gDriverParams[];

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

extern struct ModelCtx *gUnk_03000D74;

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

void obj_point1_diff(struct Point1Obj *obj);
void obj_point1_pm20(struct Point1Obj *obj);
void obj_point1_decay31_2f58(void);
void obj_point1_decay31_3064(void);
void obj_point1_decay45_5e00(void);
void obj_point1_decay67_5f1c(void);
void obj_point1_decay_5ba4(void);
void obj_point1_div3_6260(void);
void obj_point1_half(struct Point1Obj *obj);
void obj_point1_half_637c(void);
void obj_point1_mp20(struct Point1Obj *obj);
void obj_point1_rem140(void);
void obj_point1_rem140_3404(void);
void obj_point1_rem168_3644(void);
void obj_point1_rem168_3764(void);
void obj_point1_rem190_3884(void);
void obj_point1_rem190_39a4(void);
void obj_point1_rem_step15_1738(void);
void obj_point1_rem_step21_2058(void);
void obj_point1_rem_step27_1f34(void);
void obj_point1_rem_step31_1618(void);
void obj_point1_sum_3f2c(void);
void obj_point1_sum_403c(void);
void obj_point1_sum_46f8(void);
void obj_point1_sum_4808(void);
void obj_point1_sum_4918(void);
void obj_point1_sum_4a28(void);
void obj_point1_sum_51ec(void);
void obj_point1_sum_52fc(void);
void obj_point1_sum_540c(void);
void obj_point1_sum_551c(void);
void obj_point1_sum_562c(void);
void obj_point1_sum_573c(void);
void obj_point1_sum_584c(void);
void obj_point1_sum_6040(void);
void obj_point1_sum_6150(void);
void obj_point1_sum_6494(void);
void obj_point1_sum_65a4(void);
void obj_point1_sum_66b4(void);
void obj_point1_sum_a(struct Point1Obj *obj);
void obj_point1_sum_b(struct Point1Obj *obj);
void obj_point1_sum_c(struct Point1Obj *obj);
void obj_point1_turn_a(struct Point1Obj *obj);
void obj_point1_turn_b(struct Point1Obj *obj);
void obj_point1_turn_m4_6900(void);
void obj_point1_turn_p2_67c4(void);
void obj_point1_zoom23_acc_5cd0(void);
void obj_point1_zoom_step15(void);
void obj_point1_zoom_step15_fab4(void);
void obj_point1_zoom_step31(void);
void obj_point1_zoom_step31_005c(void);
void obj_point1_zoom_step31_ead4(void);
void obj_point1_zoom_step31_f994(void);
void obj_point1_zoom_step35_32b8(void);
void obj_point1_zoom_step_half_3170(void);
void obj_pulse_tilt_model4(void);
void obj_pulse_tilt_turn4(void);
void obj_spin_model4_minus3(void);
void obj_spin_model4_plus3(void);
void obj_zoom_model2_flat_0624(void);
void obj_zoom_model2_flat_ee48(void);
void obj_zoom_model2_turn_rem6_017c(void);
void obj_zoom_model2_turn_rem6_03d0(void);
void obj_zoom_model3_turn_rem6_0858(void);
void obj_zoom_model3_turn_rem6_0aac(void);
void obj_zoom_model4_turn45_0d00(void);
void obj_zoom_model4_turn45_f2d0(void);
void obj_zoom_model4_turn_rem6_0f3c(void);
void obj_zoom_model4_turn_rem6_1190(void);
void obj_zoom_model4_turn_rem6_1858(void);
void obj_zoom_model4_turn_rem6_1aac(void);
void obj_zoom_model5_turn_rem6_ebf4(void);
void obj_zoom_model6(void);
void obj_zoom_model6_flat_13e4(void);
void obj_zoom_model6_flat_f760(void);
void obj_zoom_model6_rem140(void);
void obj_zoom_model6_rem168(void);
void obj_zoom_model6_rem190(void);
void obj_zoom_model6_tilt45_4b38(void);
void obj_zoom_model6_turn_rem6_f07c(void);
void obj_zoom_model8(void);
void obj_zoom_model8_2aa8(void);
void obj_zoom_model8_flat_1d00(void);
void obj_zoom_model8_flat_fe28(void);
void obj_zoom_model8_tilt22_595c(void);
void obj_zoom_model8_tilt45_4d74(void);
void obj_zoom_model8_tilt45_4fb0(void);
void obj_zoom_model8_turn_6a40(void);
void obj_zoom_model8_turn_6cbc(void);
void obj_zoom_model8_turn_rem6(void);
void obj_zoom_model8_turn_rem6_f50c(void);
void obj_zoom_model8_turn_rem6_fbd4(void);

extern u8 gPoint1PhaseDecayParams08506e00[];
extern u8 gPoint1PhaseParams08506d54[];
extern u8 gPoint1PhaseParams08506d5c[];
extern u8 gPoint1PhaseParams08506d88[];
extern u8 gPoint1PhaseParams08506d90[];
extern u8 gPoint1PhaseParams08506d98[];
extern u8 gPoint1PhaseParams08506da0[];
extern u8 gPoint1PhaseParams08506dc0[];
extern u8 gPoint1PhaseParams08506dc8[];
extern u8 gPoint1PhaseParams08506dd0[];
extern u8 gPoint1PhaseParams08506dd8[];
extern u8 gPoint1PhaseParams08506de0[];
extern u8 gPoint1PhaseParams08506de8[];
extern u8 gPoint1PhaseParams08506df0[];
extern u8 gPoint1ValueStepParams08506e08[];
extern u8 gPoint1ZoomStep35Params08506d08[];
extern u8 gSpawnStartPoint1Decay312f58[];
extern u8 gSpawnStartPoint1Decay313064[];
extern u8 gSpawnStartPoint1Decay455e00[];
extern u8 gSpawnStartPoint1Decay675f1c[];
extern u8 gSpawnStartPoint1Div36260[];
extern u8 gSpawnStartPoint1Half637c[];
extern u8 gSpawnStartPoint1Half73c8[];
extern u8 gSpawnStartPoint1Mp2072b0[];
extern u8 gSpawnStartPoint1Pm207198[];
extern u8 gSpawnStartPoint1RemStep151738[];
extern u8 gSpawnStartPoint1RemStep212058[];
extern u8 gSpawnStartPoint1RemStep271f34[];
extern u8 gSpawnStartPoint1RemStep311618[];
extern u8 gSpawnStartPoint1Sum6040[];
extern u8 gSpawnStartPoint1Sum6150[];
extern u8 gSpawnStartPoint1Sum6494[];
extern u8 gSpawnStartPoint1Sum65a4[];
extern u8 gSpawnStartPoint1Sum66b4[];
extern u8 gSpawnStartPoint1SumA7760[];
extern u8 gSpawnStartPoint1SumB7870[];
extern u8 gSpawnStartPoint1SumC7980[];
extern u8 gSpawnStartPoint1TurnA74e0[];
extern u8 gSpawnStartPoint1TurnB7620[];
extern u8 gSpawnStartPoint1TurnM46900[];
extern u8 gSpawnStartPoint1TurnP267c4[];
extern u8 gSpawnStartPoint1ZoomStep152734[];
extern u8 gSpawnStartPoint1ZoomStep15Fab4[];
extern u8 gSpawnStartPoint1ZoomStep31005c[];
extern u8 gSpawnStartPoint1ZoomStep312614[];
extern u8 gSpawnStartPoint1ZoomStep31Ead4[];
extern u8 gSpawnStartPoint1ZoomStep31F994[];
extern u8 gSpawnStartPoint1ZoomStepHalf3170[];
extern u8 gSpawnStartPulseTiltModel46f38[];
extern u8 gSpawnStartPulseTiltTurn42cdc[];
extern u8 gSpawnStartSpinModel4Minus3217c[];
extern u8 gSpawnStartSpinModel4Plus323c8[];
extern u8 gSpawnStartZoomModel2Flat0624[];
extern u8 gSpawnStartZoomModel2FlatEe48[];
extern u8 gSpawnStartZoomModel2TurnRem6017c[];
extern u8 gSpawnStartZoomModel2TurnRem603d0[];
extern u8 gSpawnStartZoomModel3TurnRem60858[];
extern u8 gSpawnStartZoomModel3TurnRem60aac[];
extern u8 gSpawnStartZoomModel4Turn450d00[];
extern u8 gSpawnStartZoomModel4Turn45F2d0[];
extern u8 gSpawnStartZoomModel4TurnRem60f3c[];
extern u8 gSpawnStartZoomModel4TurnRem61190[];
extern u8 gSpawnStartZoomModel4TurnRem61858[];
extern u8 gSpawnStartZoomModel4TurnRem61aac[];
extern u8 gSpawnStartZoomModel5TurnRem6Ebf4[];
extern u8 gSpawnStartZoomModel6Flat13e4[];
extern u8 gSpawnStartZoomModel6FlatF760[];
extern u8 gSpawnStartZoomModel6TurnRem6F07c[];
extern u8 gSpawnStartZoomModel82aa8[];
extern u8 gSpawnStartZoomModel8Flat1d00[];
extern u8 gSpawnStartZoomModel8FlatFe28[];
extern u8 gSpawnStartZoomModel8Turn6a40[];
extern u8 gSpawnStartZoomModel8Turn6cbc[];
extern u8 gSpawnStartZoomModel8TurnRem62854[];
extern u8 gSpawnStartZoomModel8TurnRem6F50c[];
extern u8 gSpawnStartZoomModel8TurnRem6Fbd4[];
extern u8 gUnk_08506D14[];
extern u8 gUnk_08506D1C[];
extern u8 gUnk_08506D24[];
extern u8 gUnk_08506D2C[];
extern u8 gUnk_08506D34[];
extern u8 gUnk_08506D3C[];
extern u8 gZoomModel6Params08506d44[];
extern u8 gZoomModel6Rem140Params08506d64[];
extern u8 gZoomModel6Rem168Params08506d70[];
extern u8 gZoomModel6Rem190Params08506d7c[];
extern u8 gZoomModel6Tilt45Params08506da8[];
extern u8 gZoomModel8Params08506d4c[];
extern u8 gZoomModel8Tilt22Params08506df8[];
extern u8 gZoomModel8Tilt45Params08506db0[];
extern u8 gZoomModel8Tilt45Params08506db8[];

void obj_point1_pm20(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    u32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase + 0x20;
    rec->unk6 = phase + 0x20 - 0x40;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void obj_point1_mp20(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    u32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase - 0x20;
    rec->unk6 = phase - 0x20 + 0x40;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void obj_point1_half(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    s32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase;
    rec->unk6 = phase / 2;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void obj_point1_turn_a(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    s32 phase;
    u32 spin;
    s32 v;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
    spin = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase;
    rec->unk6 = phase / 3;
    if (obj->flags & 0x80) {
        v = (-spin) << 8;
    } else {
        v = spin << 8;
    }
    rec->unkC = v;
    words = obj->data;
    *words++ = zoom;
    *words++ = phase;
    *words = spin;
}

void obj_point1_turn_b(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    s32 phase;
    u32 spin;
    s32 v;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
    spin = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase;
    rec->unk6 = phase / 3;
    if (obj->flags & 0x80) {
        v = (-spin) << 8;
    } else {
        v = spin << 8;
    }
    rec->unkC = v;
    words = obj->data;
    *words++ = zoom;
    *words++ = phase;
    *words = spin;
}

void obj_point1_sum_a(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    u32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase;
    rec->unk6 = phase;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void obj_point1_sum_b(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    u32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase;
    rec->unk6 = phase;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void obj_point1_sum_c(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    u32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase += zoom;
    rec->unk4 = phase;
    rec->unk6 = phase;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void obj_point1_diff(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    u32 zoom;
    u32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    phase -= zoom;
    rec->unk4 = phase;
    rec->unk6 = phase;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}

void spawn_cb_08197a90(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x11, obj_point1_diff, 0x20B9, 0, 0, 1, flags, flags,
                 gSpawnStartPoint1Diff7a90, 2);
}

void spawn_cb_08197980(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_c, 0x20B8, 1, 0,
                 1, flags, flags, gSpawnStartPoint1SumC7980, 2);
}

void spawn_cb_08197870(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_b, 0x20B8, 0, 0,
                 1, flags, flags, gSpawnStartPoint1SumB7870, 2);
}

void spawn_cb_08197760(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x7, obj_point1_sum_a, 0x20B8, 0, 0,
                 1, flags, flags, gSpawnStartPoint1SumA7760, 2);
}

void spawn_cb_08197620(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_turn_b, 0x20B7, 0, 0,
                 1, flags, 0, gSpawnStartPoint1TurnB7620, 3);
}

void spawn_cb_081974e0(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_turn_a, 0x20B7, 0, 0,
                 1, flags, 0, gSpawnStartPoint1TurnA74e0, 3);
}

void spawn_cb_081973c8(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_half, 0x20B5, 1, 0,
                 1, flags, flags, gSpawnStartPoint1Half73c8, 2);
}

void spawn_cb_081972b0(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_mp20, 0x20B6, 1, 0,
                 1, flags, flags, gSpawnStartPoint1Mp2072b0, 2);
}

void spawn_cb_08197198(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_pm20, 0x20B6, 2, 0,
                 1, flags, flags, gSpawnStartPoint1Pm207198, 2);
}

void spawn_cb_08196f38(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_pulse_tilt_model4, 0x20B6, 0, 0,
                 4, flags, 0, gSpawnStartPulseTiltModel46f38, 2);
}

void spawn_cb_08196cbc(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model8_turn_6cbc, 0x20B6, 0, 0,
                 8, flags, 0, gSpawnStartZoomModel8Turn6cbc, 3);
}

void spawn_cb_08196a40(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model8_turn_6a40, 0x20B6, 0, 0,
                 8, flags, 0, gSpawnStartZoomModel8Turn6a40, 3);
}

void spawn_cb_08196900(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xD, obj_point1_turn_m4_6900, 0x20B5, 0, 0,
                 1, flags, flags, gSpawnStartPoint1TurnM46900, 3);
}

void spawn_cb_081967c4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_turn_p2_67c4, 0x20B4, 1, 0,
                 1, flags, flags, gSpawnStartPoint1TurnP267c4, 3);
}

void spawn_cb_081966b4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_point1_sum_66b4, 0x20B4, 2, 0,
                 1, flags, flags, gSpawnStartPoint1Sum66b4, 2);
}

void spawn_cb_081965a4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x5, obj_point1_sum_65a4, 0x20B4, 0, 0,
                 1, flags, flags, gSpawnStartPoint1Sum65a4, 2);
}

void spawn_cb_08196494(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x5, obj_point1_sum_6494, 0x20B3, 1, 0,
                 1, flags, flags, gSpawnStartPoint1Sum6494, 2);
}

void spawn_cb_0819637c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_half_637c, 0x20B3, 0, 0,
                 1, flags, flags, gSpawnStartPoint1Half637c, 2);
}

void spawn_cb_08196260(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_div3_6260, 0x20B0, 4, 0,
                 1, flags, flags, gSpawnStartPoint1Div36260, 2);
}

void spawn_cb_08196150(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_6150, 0x20B0, 3, 0,
                 1, flags, flags, gSpawnStartPoint1Sum6150, 2);
}

void spawn_cb_08196040(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xD, obj_point1_sum_6040, 0x20B0, 2, 0,
                 1, flags, flags, gSpawnStartPoint1Sum6040, 2);
}

void spawn_cb_08195f1c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x19, obj_point1_decay67_5f1c, 0x20B0, 3, 0,
                 1, flags, flags, gSpawnStartPoint1Decay675f1c, 2);
}

void spawn_cb_08195e00(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x21, obj_point1_decay45_5e00, 0x20B0, 2, 0,
                 1, flags, flags, gSpawnStartPoint1Decay455e00, 2);
}

void spawn_cb_08195cd0(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x19, obj_point1_zoom23_acc_5cd0, 0x20B0, 4, 0,
                 1, flags, flags, gPoint1ValueStepParams08506e08, 2);
}

void spawn_cb_08195ba4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x19, obj_point1_decay_5ba4, 0x20B0, 1, 0,
                 1, flags, flags, gPoint1PhaseDecayParams08506e00, 2);
}

void spawn_cb_0819595c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_zoom_model8_tilt22_595c, 0x20B0, 0, 0,
                 8, flags, flags, gZoomModel8Tilt22Params08506df8, 2);
}

void spawn_cb_0819584c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x11, obj_point1_sum_584c, 0x20AF, 2, 0,
                 1, flags, flags, gPoint1PhaseParams08506df0, 2);
}

void spawn_cb_0819573c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_573c, 0x20AF, 3, 0,
                 1, flags, flags, gPoint1PhaseParams08506de8, 2);
}

void spawn_cb_0819562c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_562c, 0x20AF, 3, 0,
                 1, flags, flags, gPoint1PhaseParams08506de0, 2);
}

void spawn_cb_0819551c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_551c, 0x20AF, 3, 0,
                 1, flags, flags, gPoint1PhaseParams08506dd8, 2);
}

void spawn_cb_0819540c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_540c, 0x20AF, 1, 0,
                 1, flags, flags, gPoint1PhaseParams08506dd0, 2);
}

void spawn_cb_081952fc(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_52fc, 0x20AF, 1, 0,
                 1, flags, flags, gPoint1PhaseParams08506dc8, 2);
}

void spawn_cb_081951ec(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xD, obj_point1_sum_51ec, 0x20AF, 2, 0,
                 1, flags, flags, gPoint1PhaseParams08506dc0, 2);
}

void spawn_cb_08194fb0(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_zoom_model8_tilt45_4fb0, 0x20AF, 4, 0,
                 8, flags, flags, gZoomModel8Tilt45Params08506db8, 2);
}

void spawn_cb_08194d74(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_zoom_model8_tilt45_4d74, 0x20AF, 0, 0,
                 8, flags, flags, gZoomModel8Tilt45Params08506db0, 2);
}

void spawn_cb_08194b38(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_zoom_model6_tilt45_4b38, 0x20AF, 0, 0,
                 6, flags, flags, gZoomModel6Tilt45Params08506da8, 2);
}

void spawn_cb_08194a28(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x5, obj_point1_sum_4a28, 0x20AE, 2, 0,
                 1, flags, flags, gPoint1PhaseParams08506da0, 2);
}

void spawn_cb_08194918(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x5, obj_point1_sum_4918, 0x20AE, 2, 0,
                 1, flags, flags, gPoint1PhaseParams08506d98, 2);
}

void spawn_cb_08194808(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_4808, 0x20AE, 1, 0,
                 1, flags, flags, gPoint1PhaseParams08506d90, 2);
}

void spawn_cb_081946f8(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_46f8, 0x20AE, 1, 0,
                 1, flags, flags, gPoint1PhaseParams08506d88, 2);
}

void spawn_cb_08194514(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model6_rem190, 0x207D, 10, 0,
                 6, flags, flags, gZoomModel6Rem190Params08506d7c, 3);
}

void spawn_cb_08194330(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model6_rem168, 0x207D, 10, 0,
                 6, flags, flags, gZoomModel6Rem168Params08506d70, 3);
}

void spawn_cb_0819414c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model6_rem140, 0x207D, 10, 0,
                 6, flags, flags, gZoomModel6Rem140Params08506d64, 3);
}

void spawn_cb_0819403c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_403c, 0x20AE, 1, 0,
                 1, flags, flags, gPoint1PhaseParams08506d5c, 2);
}

void spawn_cb_08193f2c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x9, obj_point1_sum_3f2c, 0x20AE, 1, 0,
                 1, flags, flags, gPoint1PhaseParams08506d54, 2);
}

void spawn_cb_08193cf8(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model8, 0x20AE, 0, 0,
                 8, flags, flags, gZoomModel8Params08506d4c, 2);
}

void spawn_cb_08193ac4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model6, 0x20AE, 0, 0,
                 6, flags, flags, gZoomModel6Params08506d44, 2);
}

void spawn_cb_081939a4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_point1_rem190_39a4, 0x207D, 13, 0,
                 1, flags, flags, gUnk_08506D3C, 2);
}

void spawn_cb_08193884(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_point1_rem190_3884, 0x207D, 13, 0,
                 1, flags, 0, gUnk_08506D34, 2);
}

void spawn_cb_08193764(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_point1_rem168_3764, 0x207D, 13, 0,
                 1, flags, flags, gUnk_08506D2C, 2);
}

void spawn_cb_08193644(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_point1_rem168_3644, 0x207D, 13, 0,
                 1, flags, 0, gUnk_08506D24, 2);
}

void spawn_cb_08193524(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_point1_rem140, 0x207D, 13, 0,
                 1, flags, flags, gUnk_08506D1C, 2);
}

void spawn_cb_08193404(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_point1_rem140_3404, 0x207D, 13, 0,
                 1, flags, 0, gUnk_08506D14, 2);
}

void spawn_cb_081932b8(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x25, obj_point1_zoom_step35_32b8, 0x20AC, 0, 0,
                 1, flags, flags, gPoint1ZoomStep35Params08506d08, 3);
}

void spawn_cb_08193170(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x25, obj_point1_zoom_step_half_3170, 0x20AC, 1, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStepHalf3170, 3);
}

void spawn_cb_08193064(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x15, obj_point1_decay31_3064, 0x20AC, 3, 0,
                 1, flags, flags, gSpawnStartPoint1Decay313064, 1);
}

void spawn_cb_08192f58(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x15, obj_point1_decay31_2f58, 0x20AC, 2, 0,
                 1, flags, flags, gSpawnStartPoint1Decay312f58, 1);
}

void spawn_cb_08192cdc(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_pulse_tilt_turn4, 0x20AB, 0, 0,
                 4, flags, 0, gSpawnStartPulseTiltTurn42cdc, 3);
}

void spawn_cb_08192aa8(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model8_2aa8, 0x20AA, 0, 0,
                 8, flags, flags, gSpawnStartZoomModel82aa8, 2);
}

void spawn_cb_08192854(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model8_turn_rem6, 0x20AA, 1, 0,
                 8, flags, flags, gSpawnStartZoomModel8TurnRem62854, 4);
}

void spawn_cb_08192734(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x13, obj_point1_zoom_step15, 0x20AA, 2, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStep152734, 2);
}

void spawn_cb_08192614(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_point1_zoom_step31, 0x20AA, 3, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStep312614, 2);
}

void spawn_cb_081923c8(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_spin_model4_plus3, 0x20A8, 0, 0,
                 4, flags, flags, gSpawnStartSpinModel4Plus323c8, 3);
}

void spawn_cb_0819217c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_spin_model4_minus3, 0x20A9, 0, 0,
                 4, flags, flags, gSpawnStartSpinModel4Minus3217c, 3);
}

void spawn_cb_08192058(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x13, obj_point1_rem_step21_2058, 0x20A9, 1, 0,
                 1, flags, flags, gSpawnStartPoint1RemStep212058, 2);
}

void spawn_cb_08191f34(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x11, obj_point1_rem_step27_1f34, 0x20A9, 2, 0,
                 1, flags, flags, gSpawnStartPoint1RemStep271f34, 2);
}

void spawn_cb_08191d00(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model8_flat_1d00, 0x20A8, 0, 0,
                 8, flags, flags, gSpawnStartZoomModel8Flat1d00, 2);
}

void spawn_cb_08191aac(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model4_turn_rem6_1aac, 0x20A8, 4, 0,
                 4, flags, flags, gSpawnStartZoomModel4TurnRem61aac, 4);
}

void spawn_cb_08191858(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model4_turn_rem6_1858, 0x20A8, 1, 0,
                 4, flags, flags, gSpawnStartZoomModel4TurnRem61858, 4);
}

void spawn_cb_08191738(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x13, obj_point1_rem_step15_1738, 0x20A8, 2, 0,
                 1, flags, flags, gSpawnStartPoint1RemStep151738, 2);
}

void spawn_cb_08191618(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_point1_rem_step31_1618, 0x20A8, 3, 0,
                 1, flags, flags, gSpawnStartPoint1RemStep311618, 2);
}

void spawn_cb_081913e4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model6_flat_13e4, 0x20A8, 0, 0,
                 6, flags, flags, gSpawnStartZoomModel6Flat13e4, 2);
}

void spawn_cb_08191190(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model4_turn_rem6_1190, 0x20A8, 4, 0,
                 4, flags, flags, gSpawnStartZoomModel4TurnRem61190, 4);
}

void spawn_cb_08190f3c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model4_turn_rem6_0f3c, 0x20A8, 1, 0,
                 4, flags, flags, gSpawnStartZoomModel4TurnRem60f3c, 4);
}

void spawn_cb_08190d00(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model4_turn45_0d00, 0x20A8, 0, 0,
                 4, flags, flags, gSpawnStartZoomModel4Turn450d00, 2);
}

void spawn_cb_08190aac(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model3_turn_rem6_0aac, 0x20A8, 4, 0,
                 3, flags, flags, gSpawnStartZoomModel3TurnRem60aac, 4);
}

void spawn_cb_08190858(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model3_turn_rem6_0858, 0x20A8, 1, 0,
                 3, flags, flags, gSpawnStartZoomModel3TurnRem60858, 4);
}

void spawn_cb_08190624(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model2_flat_0624, 0x20A8, 0, 0,
                 2, flags, flags, gSpawnStartZoomModel2Flat0624, 2);
}

void spawn_cb_081903d0(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model2_turn_rem6_03d0, 0x20A8, 4, 0,
                 2, flags, flags, gSpawnStartZoomModel2TurnRem603d0, 4);
}

void spawn_cb_0819017c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model2_turn_rem6_017c, 0x20A8, 1, 0,
                 2, flags, flags, gSpawnStartZoomModel2TurnRem6017c, 4);
}

void spawn_cb_0819005c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x13, obj_point1_zoom_step31_005c, 0x20A8, 2, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStep31005c, 2);
}

void spawn_cb_0818fe28(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model8_flat_fe28, 0x20A7, 0, 0,
                 8, flags, flags, gSpawnStartZoomModel8FlatFe28, 2);
}

void spawn_cb_0818fbd4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model8_turn_rem6_fbd4, 0x20A7, 1, 0,
                 8, flags, flags, gSpawnStartZoomModel8TurnRem6Fbd4, 4);
}

void spawn_cb_0818fab4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x13, obj_point1_zoom_step15_fab4, 0x20A7, 2, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStep15Fab4, 2);
}

void spawn_cb_0818f994(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xB, obj_point1_zoom_step31_f994, 0x20A7, 3, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStep31F994, 2);
}

void spawn_cb_0818f760(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model6_flat_f760, 0x20A7, 0, 0,
                 6, flags, flags, gSpawnStartZoomModel6FlatF760, 2);
}

void spawn_cb_0818f50c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model8_turn_rem6_f50c, 0x20A7, 1, 0,
                 8, flags, flags, gSpawnStartZoomModel8TurnRem6F50c, 4);
}

void spawn_cb_0818f2d0(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model4_turn45_f2d0, 0x20A7, 0, 0,
                 4, flags, flags, gSpawnStartZoomModel4Turn45F2d0, 2);
}

void spawn_cb_0818f07c(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model6_turn_rem6_f07c, 0x20A7, 1, 0,
                 6, flags, flags, gSpawnStartZoomModel6TurnRem6F07c, 4);
}

void spawn_cb_0818ee48(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x0, obj_zoom_model2_flat_ee48, 0x20A7, 0, 0,
                 2, flags, flags, gSpawnStartZoomModel2FlatEe48, 2);
}

void spawn_cb_0818ebf4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0xF, obj_zoom_model5_turn_rem6_ebf4, 0x20A7, 1, 0,
                 5, flags, flags, gSpawnStartZoomModel5TurnRem6Ebf4, 4);
}

void spawn_cb_0818ead4(u32 p0, u32 p1, u32 p2, u32 flags)
{
    fx_obj_spawn(p0, p1, p2, 0x13, obj_point1_zoom_step31_ead4, 0x20A7, 2, 0,
                 1, flags, flags, gSpawnStartPoint1ZoomStep31Ead4, 2);
}

u32 table_085213b8(u32 index)
{
    return gDriverParams[index];
}
