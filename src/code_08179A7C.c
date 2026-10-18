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

extern struct ModelCtx *gModelCtx;

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

ASM_FUNC("asm/code_08179A7C/spin_scroll_9a7c.s", void spin_scroll_9a7c(void))
ASM_FUNC("asm/code_08179A7C/spin_scroll_9c94.s", void spin_scroll_9c94(void))
ASM_FUNC("asm/code_08179A7C/spin_offset_9ea4.s", void spin_offset_9ea4(void))
ASM_FUNC("asm/code_08179A7C/spin_offset_a0cc.s", void spin_offset_a0cc(void))

ASM_FUNC("asm/code_08179A7C/zoom_turn_a2f8.s", void zoom_turn_a2f8(void))

ASM_FUNC("asm/code_08179A7C/zoom_turn_a534.s", void zoom_turn_a534(void))

ASM_FUNC("asm/code_08179A7C/obj_model8_zoom_xslide_a770.s", void obj_model8_zoom_xslide_a770(void))

void obj_point_static_oam_160_a99c(struct Point1Obj *obj)
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
    rec->unk4 = 0x160;
    rec->unk6 = 0x160;
}

ASM_FUNC("asm/code_08179A7C/obj_model8_zoom_vel7_oam_130_aaa0.s", void obj_model8_zoom_vel7_oam_130_aaa0(void))

ASM_FUNC("asm/code_08179A7C/obj_model8_zoom_vel7_oam_130_acd8.s", void obj_model8_zoom_vel7_oam_130_acd8(void))

ASM_FUNC("asm/code_08179A7C/obj_model5_zoom_drop_af10.s", void obj_model5_zoom_drop_af10(void))
ASM_FUNC("asm/code_08179A7C/obj_model5_zoom_fall_oam_b0e0.s", void obj_model5_zoom_fall_oam_b0e0(void))
