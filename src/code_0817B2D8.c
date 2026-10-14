#include "global.h"
#include "ram_routines.h"

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

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_vel9_b2d8.s", void obj_model8_zoom_vel9_b2d8(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_vel9_b4f0.s", void obj_model8_zoom_vel9_b4f0(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_vel9_b708.s", void obj_model8_zoom_vel9_b708(void))

ASM_FUNC("asm/code_0817B2D8/obj_tilt_zoom2_b920.s", void obj_tilt_zoom2_b920(void))

ASM_FUNC("asm/code_0817B2D8/obj_tilt_zoom2_oam_c0_baf4.s", void obj_tilt_zoom2_oam_c0_baf4(void))

ASM_FUNC("asm/code_0817B2D8/obj_tilt_zoom2_bcd4.s", void obj_tilt_zoom2_bcd4(void))

ASM_FUNC("asm/code_0817B2D8/obj_tilt_zoom2_bea8.s", void obj_tilt_zoom2_bea8(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_c07c.s", void obj_model8_zoom_c07c(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_c274.s", void obj_model8_zoom_c274(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_c46c.s", void obj_model8_zoom_c46c(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_oam_190_c664.s", void obj_model8_zoom_oam_190_c664(void))

ASM_FUNC("asm/code_0817B2D8/obj_model8_zoom_oam_180_c85c.s", void obj_model8_zoom_oam_180_c85c(void))

void obj_point_fixed_ca54(struct Point1Obj *obj)
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
    s32 zraw;
    s32 pos;
    s32 t;

    words = obj->data;
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
        zraw = *words++;
        l.y -= 0xC00;
        l.z = zraw - 0x100;
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
    rec->unk4 = 0x180;
    rec->unk6 = 0x180;
}
