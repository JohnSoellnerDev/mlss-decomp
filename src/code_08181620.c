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
    s32 *pt;
    s32 zoom;
    s32 speed;
    struct PtRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xs;
    s32 ys;
    s32 zs;
    s32 x;
    s32 y;
    s32 z;
    s32 y1;
    s32 z1;
    s32 x_px;

    pt = (s32 *)obj->data;
    zoom = *pt++;
    speed = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 10; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        z = *pt++;
        l.z = z;
        xs = (x * zoom) / 0x100;
        l.x = xs;
        ys = (y * zoom) / 0x100;
        l.y = ys;
        zs = (z * zoom) / 0x100;
        l.z = zs;
        y1 = (ys * (gCosineTable[12] / 0x40) - zs * (gSineTable[12] / 0x40)) / 0x100;
        l.y = y1;
        z1 = (zs * (gCosineTable[12] / 0x40) + ys * (gSineTable[12] / 0x40)) / 0x100;
        l.z = z1;
        l.y = y1 + -0x800;
        if (obj->flags & 0x80) {
            x_px = obj->posX - (xs / 0x100);
        } else {
            x_px = obj->posX + (xs / 0x100);
        }
        l.x = x_px;
        l.y = obj->posY + (l.y / 0x100);
        l.z = obj->posZ + (l.z / 0x100);
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
    pt = (s32 *)obj->data;
    *pt++ = zoom;
    *pt = speed;
}
END_NONMATCH

ASM_FUNC("asm/code_08181620/obj_zoom_quarter_spin8_1804.s", void obj_zoom_quarter_spin8_1804(void))

ASM_FUNC("asm/code_08181620/obj_zoom_tilt_rise8_1a5c.s", void obj_zoom_tilt_rise8_1a5c(void))

ASM_FUNC("asm/code_08181620/obj_jump_zoom_turn_points_1cb8.s", void obj_jump_zoom_turn_points_1cb8(void))

ASM_FUNC("asm/code_08181620/obj_rise_turn_points_1f4c.s", void obj_rise_turn_points_1f4c(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2180.s", void obj_grow_spin_points_2180(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2428.s", void obj_grow_spin_points_2428(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_26d0.s", void obj_grow_spin_points_26d0(void))

ASM_FUNC("asm/code_08181620/obj_grow_spin_points_2980.s", void obj_grow_spin_points_2980(void))

ASM_FUNC("asm/code_08181620/obj_spring_turn_points_2c50.s", void obj_spring_turn_points_2c50(void))
