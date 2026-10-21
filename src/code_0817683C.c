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

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

void fx_obj_spawn(u32 x, u32 y, u32 z, u32 type, void (*callback)(), u32 attr, u32 mode,
                  u32 unk0C, u32 points, u32 flagA, u32 flagB, const void *table, u32 count);

extern void obj_zoom_spin4_size180_6b24(void);
extern void obj_zoom_spin5_6e14(void);
extern void obj_zoom_spin5_size140_683c(void);
void obj_zoom_spin5_7104(struct Point1Obj *obj);
void obj_zoom_spin5_736c(struct Point1Obj *obj);
void obj_zoom_spin6_75d4(struct Point1Obj *obj);
extern void obj_point_fixed_3ed8(void);
extern void obj_point_fixed_3fd8(void);
extern void obj_point_fixed_40d8(void);
extern void obj_point_fixed_41d8(void);
extern void obj_point_fixed_53c4(void);
extern void obj_point_shrink140_5c34(void);
extern void obj_point_shrink180_3858(void);
extern void obj_point_shrink180_3978(void);
extern void obj_point_shrink180_4f74(void);
extern void obj_point_shrink180_5088(void);
extern void obj_point_shrink180_5d48(void);
extern void obj_point_shrink1c0_5e5c(void);
extern void obj_point_shrink200_519c(void);
extern void obj_point_shrink200_52b0(void);
extern void obj_point_shrink_4d4c(void);
extern void obj_point_shrink_4e60(void);
extern void obj_point_shrink_5b20(void);
extern void obj_point_shrink_wide_54b8(void);
extern void obj_zoom_grow2_3d00(void);
extern void obj_zoom_spin4_3a98(void);
extern void obj_zoom_spin4_55d0(void);
extern void obj_zoom_spin5_44b8(void);
extern void obj_zoom_spin5_4794(void);
extern void obj_zoom_spin5_size100_5f70(void);
extern void obj_zoom_spin5_size1c0_6260(void);
extern void obj_zoom_spin5_size200_6550(void);
extern void obj_zoom_spin6_4a70(void);
extern void obj_zoom_tilt5_42d8(void);
extern void obj_zoom_tilt6_58c0(void);
extern const u8 gStartDataObjZoomSpin4Size1806b24[];
extern const u8 gStartDataObjZoomSpin56e14[];
extern const u8 gStartDataObjZoomSpin57104[];
extern const u8 gStartDataObjZoomSpin5736c[];
extern const u8 gStartDataObjZoomSpin675d4[];
extern const u8 gStartObjZoomGrow23d00[];
extern const u8 gStartObjZoomSpin43a98[];
extern const u8 gStartObjZoomSpin455d0[];
extern const u8 gStartObjZoomSpin544b8[];
extern const u8 gStartObjZoomSpin54794[];
extern const u8 gStartObjZoomSpin5Size1005f70[];
extern const u8 gStartObjZoomSpin5Size140683c[];
extern const u8 gStartObjZoomSpin5Size1c06260[];
extern const u8 gStartObjZoomSpin5Size2006550[];
extern const u8 gStartObjZoomSpin64a70[];
extern const u8 gStartObjZoomTilt542d8[];
extern const u8 gStartObjZoomTilt658c0[];
extern const u8 gUnk_08506168[];
extern const u8 gUnk_08506170[];
extern const u8 gUnk_0850618C[];
extern const u8 gUnk_08506190[];
extern const u8 gUnk_08506194[];
extern const u8 gUnk_08506198[];
extern const u8 gUnk_085061E0[];
extern const u8 gUnk_085061E4[];
extern const u8 gUnk_085061E8[];
extern const u8 gUnk_085061EC[];
extern const u8 gUnk_085061F0[];
extern const u8 gUnk_085061F4[];
extern const u8 gUnk_085061F8[];
extern const u8 gUnk_0850621C[];
extern const u8 gUnk_08506220[];
extern const u8 gUnk_08506224[];
extern const u8 gUnk_08506228[];

ASM_FUNC("asm/code_0817683C/obj_zoom_spin5_size140_683c.s", void obj_zoom_spin5_size140_683c(void))

ASM_FUNC("asm/code_0817683C/obj_zoom_spin4_size180_6b24.s", void obj_zoom_spin4_size180_6b24(void))

ASM_FUNC("asm/code_0817683C/obj_zoom_spin5_6e14.s", void obj_zoom_spin5_6e14(void))

void obj_zoom_spin5_7104(struct Point1Obj *obj)
{
    s16 *cs;
    s16 *sn;
    struct Point1Local l;
    s32 vel;
    s32 angle;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 zoom;
    u32 *words;
    s32 vx;
    s32 z1;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 z3;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 5; i++) {
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
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[32] / 0x40);
        y2 = (t - (gSineTable[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = (z2 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * y1) / 0x100;
        l.z = z3;
        l.y = y2 - 0x800;

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
    zoom += vel;
    vel = (vel * 3) / 4;
    angle += 2;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}


void obj_zoom_spin5_736c(struct Point1Obj *obj)
{
    s16 *cs;
    s16 *sn;
    struct Point1Local l;
    s32 vel;
    s32 angle;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 zoom;
    u32 *words;
    s32 vx;
    s32 z1;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 z3;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 5; i++) {
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
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[32] / 0x40);
        y2 = (t - (gSineTable[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = (z2 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * y1) / 0x100;
        l.z = z3;
        l.y = y2 - 0x800;

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
    zoom += vel;
    vel = (vel * 3) / 4;
    angle += 2;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}

void obj_zoom_spin6_75d4(struct Point1Obj *obj)
{
    s16 *cs;
    s16 *sn;
    struct Point1Local l;
    s32 vel;
    s32 angle;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 zoom;
    u32 *words;
    s32 vx;
    s32 z1;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 z3;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 6; i++) {
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
        y2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        y2 = y2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (y2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[32] / 0x40);
        y2 = (t - (gSineTable[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = (z2 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * y1) / 0x100;
        l.z = z3;
        l.y = y2 - 0x800;

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
    zoom += vel;
    vel = (vel * 3) / 4;
    angle += 2;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words = angle;
}

void spawn_obj_zoom_spin6_75d4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin6_75d4, 0x2080, 3, 0, 6, flag, flag,
                 gStartDataObjZoomSpin675d4, 3);
}

void spawn_obj_zoom_spin5_736c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_736c, 0x2080, 2, 0, 5, flag, flag,
                 gStartDataObjZoomSpin5736c, 3);
}

void spawn_obj_zoom_spin5_7104(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_7104, 0x2080, 1, 0, 5, flag, flag,
                 gStartDataObjZoomSpin57104, 3);
}

void spawn_obj_zoom_spin5_6e14(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_6e14, 0x2080, 1, 0, 5, flag, flag,
                 gStartDataObjZoomSpin56e14, 5);
}

void spawn_obj_zoom_spin4_size180_6b24(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin4_size180_6b24, 0x2080, 1, 0, 4, flag, flag,
                 gStartDataObjZoomSpin4Size1806b24, 5);
}

void spawn_obj_zoom_spin5_size140_683c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_size140_683c, 0x2080, 1, 0, 5, flag, flag,
                 gStartObjZoomSpin5Size140683c, 5);
}

void spawn_obj_zoom_spin5_size200_6550(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_size200_6550, 0x207d, 7, 0, 5, flag, flag,
                 gStartObjZoomSpin5Size2006550, 5);
}

void spawn_obj_zoom_spin5_size1c0_6260(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_size1c0_6260, 0x207d, 7, 0, 5, flag, flag,
                 gStartObjZoomSpin5Size1c06260, 5);
}

void spawn_obj_zoom_spin5_size100_5f70(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin5_size100_5f70, 0x207d, 2, 0, 5, flag, flag,
                 gStartObjZoomSpin5Size1005f70, 5);
}

void spawn_obj_point_shrink1c0_5e5c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink1c0_5e5c, 0x2080, 0, 0, 1, flag, flag,
                 gUnk_08506228, 1);
}

void spawn_obj_point_shrink180_5d48(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink180_5d48, 0x2080, 0, 0, 1, flag, flag,
                 gUnk_08506224, 1);
}

void spawn_obj_point_shrink140_5c34(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink140_5c34, 0x2080, 0, 0, 1, flag, flag,
                 gUnk_08506220, 1);
}

void spawn_obj_point_shrink_5b20(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink_5b20, 0x2080, 0, 0, 1, flag, flag,
                 gUnk_0850621C, 1);
}

void spawn_obj_zoom_tilt6_58c0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_tilt6_58c0, 0x2080, 5, 0, 6, flag, flag,
                 gStartObjZoomTilt658c0, 3);
}

void spawn_obj_zoom_spin4_55d0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin4_55d0, 0x2080, 2, 0, 4, flag, flag,
                 gStartObjZoomSpin455d0, 5);
}

void spawn_obj_point_shrink_wide_54b8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink_wide_54b8, 0x207d, 9, 0, 1, flag, flag,
                 gUnk_085061F8, 1);
}

void spawn_obj_point_fixed_53c4(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_fixed_53c4, 0x207c, 9, 0, 1, flag, flag,
                 0, 0);
}

void spawn_obj_point_shrink200_52b0(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink200_52b0, 0x207f, 1, 0, 1, flag, flag,
                 gUnk_085061F4, 1);
}

void spawn_obj_point_shrink200_519c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink200_519c, 0x207e, 1, 0, 1, flag, 0,
                 gUnk_085061F0, 1);
}

void spawn_obj_point_shrink180_5088(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink180_5088, 0x207f, 1, 0, 1, flag, flag,
                 gUnk_085061EC, 1);
}

void spawn_obj_point_shrink180_4f74(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink180_4f74, 0x207f, 1, 0, 1, flag, 0,
                 gUnk_085061E8, 1);
}

void spawn_obj_point_shrink_4e60(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink_4e60, 0x207f, 1, 0, 1, flag, flag,
                 gUnk_085061E4, 1);
}

void spawn_obj_point_shrink_4d4c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink_4d4c, 0x207f, 1, 0, 1, flag, 0,
                 gUnk_085061E0, 1);
}

void spawn_obj_zoom_spin6_4a70(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1d, obj_zoom_spin6_4a70, 0x207d, 7, 0, 6, flag, flag,
                 gStartObjZoomSpin64a70, 5);
}

void spawn_obj_zoom_spin5_4794(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x1f, obj_zoom_spin5_4794, 0x207d, 7, 0, 5, flag, flag,
                 gStartObjZoomSpin54794, 5);
}

void spawn_obj_zoom_spin5_44b8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x19, obj_zoom_spin5_44b8, 0x207d, 7, 0, 5, flag, flag,
                 gStartObjZoomSpin544b8, 5);
}

void spawn_obj_zoom_tilt5_42d8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_tilt5_42d8, 0x2080, 2, 0, 5, flag, flag,
                 gStartObjZoomTilt542d8, 2);
}

void spawn_obj_point_fixed_41d8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_fixed_41d8, 0x208e, 1, 0, 1, flag, flag,
                 gUnk_08506198, 1);
}

void spawn_obj_point_fixed_40d8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_fixed_40d8, 0x207f, 2, 0, 1, flag, flag,
                 gUnk_08506194, 1);
}

void spawn_obj_point_fixed_3fd8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_fixed_3fd8, 0x207f, 2, 0, 1, flag, 0,
                 gUnk_08506190, 1);
}

void spawn_obj_point_fixed_3ed8(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_fixed_3ed8, 0x2092, 0, 0, 1, flag, flag,
                 gUnk_0850618C, 1);
}

void spawn_obj_zoom_grow2_3d00(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_grow2_3d00, 0x207c, 12, 0, 2, flag, flag,
                 gStartObjZoomGrow23d00, 2);
}

void spawn_obj_zoom_spin4_3a98(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_spin4_3a98, 0x207d, 10, 0, 4, flag, flag,
                 gStartObjZoomSpin43a98, 3);
}

void spawn_obj_point_shrink180_3978(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink180_3978, 0x207d, 0, 0, 1, flag, flag,
                 gUnk_08506170, 2);
}

void spawn_obj_point_shrink180_3858(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_point_shrink180_3858, 0x207d, 0, 0, 1, flag, 0,
                 gUnk_08506168, 2);
}
