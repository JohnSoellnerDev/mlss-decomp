#include "global.h"

void fx_obj_spawn(u32 x, u32 y, u32 z, u32 type, void (*callback)(void), u32 attr, u32 mode,
                  u32 unk0C, u32 points, u32 flagA, u32 flagB, const void *table, u32 count);

extern void obj_zoom_grow2_3678(void);
extern void obj_zoom_grow2_3494(void);
extern void sprite_list_draw_copy(void);
extern void sprite_list_draw(void);
extern const u8 gStartObjZoomGrow23678[];
extern const u8 gStartObjZoomGrow23494[];
extern const u8 gStartSpriteListDrawCopy[];
extern const u8 gStartSpriteListDraw[];

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

extern s16 gSineTable[];
extern s16 gCosineTable[];

void spawn_obj_zoom_grow2_3678(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_grow2_3678, 0x2080, 3, 0, 2, flag, flag,
                 gStartObjZoomGrow23678, 2);
}

void spawn_obj_zoom_grow2_3494(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_zoom_grow2_3494, 0x2080, 2, 0, 2, flag, flag,
                 gStartObjZoomGrow23494, 2);
}

void spawn_sprite_list_draw_copy(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, sprite_list_draw_copy, 0x2080, 1, 0, 2, flag, flag,
                 gStartSpriteListDrawCopy, 2);
}

void spawn_sprite_list_draw(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, sprite_list_draw, 0x2080, 0, 0, 2, flag, flag,
                 gStartSpriteListDraw, 2);
}

// Single-point object whose size word grows by 0x40 each frame; the size goes into both
// record fields and the data block.
void obj_point_grow_814c(struct Point1Obj *obj)
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
    u32 size;

    words = obj->data;
    size = *words++;
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
    size += 0x40;
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words = size;
}

NONMATCH("asm/code_08178064/obj_zoom_fixed5_size100_8254.s", void obj_zoom_fixed5_size100_8254(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 base;
    s32 counter;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    base = *words++;
    counter = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 5; i++) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = z1;
        t = t * (gCosineTable[38] / 0x40);
        z2 = (t - (gSineTable[38] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[38] / 0x40) + (gSineTable[38] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[32] / 0x40);
        y2 = (t - (gSineTable[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[32] / 0x40)) + (y1 * (gSineTable[32] / 0x40))) / 0x100;

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
    vel = (vel * 4) / 5;
    base += counter;
    counter++;
    rec->unkC = ((obj->flags & 0x80) ? -base : base) / 8 << 8;
    rec->unk4 = 0x100 - base;
    rec->unk6 = 0x100 - base;
    words = obj->data;
    *words++ = zoom;
    *words++ = vel;
    *words++ = base;
    *words = counter;
}
END_NONMATCH

ASM_FUNC("asm/code_08178064/obj_zoom_spin5_size200_84cc.s", void obj_zoom_spin5_size200_84cc(struct Point1Obj *obj))

ASM_FUNC("asm/code_08178064/obj_zoom_spin6_shrink_8754.s", void obj_zoom_spin6_shrink_8754(void))

ASM_FUNC("asm/code_08178064/obj_zoom_spin6_shrink_89c0.s", void obj_zoom_spin6_shrink_89c0(void))

ASM_FUNC("asm/code_08178064/obj_zoom_spin6_shrink_8c30.s", void obj_zoom_spin6_shrink_8c30(void))

ASM_FUNC("asm/code_08178064/obj_zoom_spin6_shrink_8ea0.s", void obj_zoom_spin6_shrink_8ea0(void))

// Six-point object at a fixed tilt angle (table entry 12) whose zoom grows by 2 and whose
// offsets accelerate each frame; the record size shrinks as the counter grows.
void obj_zoom_fixed6_accel_910c(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 base;
    s32 counter;
    s32 xOff;
    s32 yOff;
    s32 vel;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x0;
    s32 y0;
    s32 z0;
    s32 y1;
    s32 z1;
    s32 xs;
    s32 pos;
    s32 t;
    s32 size;

    words = obj->data;
    zoom = *words++;
    base = *words++;
    counter = *words++;
    xOff = *words++;
    yOff = *words++;
    vel = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 5;
    for (;;) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x0 = vx * zoom / 0x100;
        l.x = x0;
        y0 = vy * zoom / 0x100;
        l.y = y0;
        z0 = vz * zoom / 0x100;
        l.z = z0;
        t = y0;
        t = t * (gCosineTable[12] / 0x40);
        y1 = (t - (gSineTable[12] / 0x40) * z0) / 0x100;
        l.y = y1;
        z1 = (z0 * (gCosineTable[12] / 0x40) + y0 * (gSineTable[12] / 0x40)) / 0x100;
        l.z = z1;
        xs = x0 + xOff;
        l.x = xs;
        l.y = y1 + yOff;
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = xs;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = xs;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t + pos;
        }
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += 0x200;
    yOff += vel;
    vel += 0x60;
    base += counter;
    counter++;
    size = 0xA0 - base;
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words++ = zoom;
    *words++ = base;
    *words++ = counter;
    *words++ = xOff;
    *words++ = yOff;
    *words = vel;
}

void obj_zoom_fixed10_accel_9304(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 zoom;
    s32 base;
    s32 counter;
    s32 yOff;
    s32 vel;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 x0;
    s32 y0;
    s32 z0;
    s32 y1;
    s32 z1;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    base = *words++;
    counter = *words++;
    yOff = *words++;
    vel = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 10; i++) {
        vx = *words++;
        l.x = vx;
        vy = *words++;
        l.y = vy;
        vz = *words++;
        l.z = vz;
        x0 = (vx * zoom) / 0x100;
        l.x = x0;
        y0 = (vy * zoom) / 0x100;
        l.y = y0;
        z0 = (vz * zoom) / 0x100;
        l.z = z0;
        t = y0;
        t = t * (gCosineTable[12] / 0x40);
        y1 = (t - (gSineTable[12] / 0x40) * z0) / 0x100;
        l.y = y1;
        z1 = (z0 * (gCosineTable[12] / 0x40) + y0 * (gSineTable[12] / 0x40)) / 0x100;
        l.z = z1;
        l.y = y1 + yOff;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x0;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x0;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t + pos;
        }
        t = obj->posY;
        pos = l.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.y = t + pos;
        t = obj->posZ;
        pos = l.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        l.z = t + pos;
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
    zoom += 0x400;
    yOff += vel;
    vel += 0x60;
    base += counter;
    counter++;
    rec->unk4 = 0x140 - base;
    rec->unk6 = 0x140 - base;
    words = obj->data;
    *words++ = zoom;
    *words++ = base;
    *words++ = counter;
    *words++ = yOff;
    *words = vel;
}

ASM_FUNC("asm/code_08178064/zoom_bounce_94fc.s", void zoom_bounce_94fc(void))

ASM_FUNC("asm/code_08178064/zoom_bounce_97bc.s", void zoom_bounce_97bc(void))
