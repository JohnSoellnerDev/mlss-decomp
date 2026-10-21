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

NONMATCH("asm/code_0819414C/obj_zoom_model6_rem140.s", void obj_zoom_model6_rem140(struct Point1Obj *obj))
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 step;
    s32 rem;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 base;
    s32 y1;
    s32 t;
    s32 pos;
    s32 x1;
    s32 vx;
    s32 y2;
    s32 z1;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    rem = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
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
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        y2 = (t - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        if (obj->flags & 0x80) {
            base = obj->posX;
            pos = x1;
            if (pos < 0)
                pos += 0xFF;
            pos >>= 8;
            l.x = base - pos;
        } else {
            base = obj->posX;
            pos = x1;
            if (pos < 0)
                pos += 0xFF;
            pos >>= 8;
            l.x = base + pos;
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
    if (saved)
        gModelCtx->model = l.model;
    zoom += step;
    step = (step * 3) / 4;
    rem++;
    rec->unk4 = 0x140 - rem;
    rec->unk6 = 0x140 - rem;
    words = obj->data;
    *words++ = zoom;
    *words++ = step;
    *words = rem;
}
END_NONMATCH

ASM_FUNC("asm/code_0819414C/obj_zoom_model6_rem168.s", void obj_zoom_model6_rem168(void))

ASM_FUNC("asm/code_0819414C/obj_zoom_model6_rem190.s", void obj_zoom_model6_rem190(void))
