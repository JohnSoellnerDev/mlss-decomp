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

// The affine sprite an effect object draws.
struct ObjRecord
{
    u8 filler0[4];
    u16 unk4;  // scale X (0x100 = 1.0)
    u16 unk6;  // scale Y (0x100 = 1.0)
    u8 filler8[4];
    u16 unkC;  // rotation angle
    u8 filler10[0x34 - 0x10];
    u32 model; // the sprite's OAM row list
};

// An effect object that draws copies of its sprite at points of a model.
struct Point1Obj
{
    u8 filler0[8];
    struct ObjRecord *rec;
    u32 *data;  // state words, then the model's points (x, y, z each)
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;   // 0x80: mirrored horizontally
};

/* Point1Obj.flags: the object faces left, so x offsets and angles are mirrored. */
#define POINT1_OBJ_MIRRORED 0x80

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

// One-point effect that grows: draws its sprite at the model's one point (8.8 fixed point
// offset from the object, mirrored when it faces left), and every frame the scale kept in
// obj->data grows by 0x40 (1/4) and becomes the sprite's X and Y scale.
void obj_point_grow_814c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    struct ObjRecord *rec;
    u32 oamFlags;
    u32 modelSaved;
    s32 i;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 pos;
    s32 t;
    u32 size;

    // State: the size word in obj->data, followed by the point.
    words = obj->data;
    size = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 1; i++) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        pt.y = *words++;
        pt.z = *words++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = px;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = px;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos + t;
        }
        pos = obj->posY;
        t = pt.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.y = pos + t;
        pos = obj->posZ;
        t = pt.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        pt.z = pos + t;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
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
    s16 *cs;
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
        cs = &gCosineTable[38];
        t = t * (*cs / 0x40);
        z2 = (t - (gSineTable[38] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (gSineTable[38] / 0x40) * z1) / 0x100;
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

NONMATCH("asm/code_08178064/obj_zoom_spin5_size200_84cc.s", void obj_zoom_spin5_size200_84cc(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 speed;
    s32 base;
    s32 counter;
    s32 angle;
    struct ObjRecord *rec;
    u32 *words;
    s16 *sb;
    s16 *cs;
    s16 *sn;
    s16 *cb;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 zoom;
    s32 vx;
    s32 vy;
    s32 vz;
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
    speed = *words++;
    base = *words++;
    counter = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    left = 4;
    for (;;) {
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
        cs = &gCosineTable[angle & 0xFF];
        t = t * (*cs / 0x40);
        sb = gSineTable;
        sn = sb + (angle & 0xFF);
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        cb = gCosineTable;
        cs = cb + 32;
        t = t * (*cs / 0x40);
        y2 = (t - (sb[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = ((z2 * (*cs / 0x40)) + ((sb[32] / 0x40) * y1)) / 0x100;
        l.z = z3;

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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += speed;
    speed = (speed * 3) / 4;
    base += counter;
    counter++;
    angle += 6;
    rec->unkC = ((obj->flags & 0x80) ? -base : base) / 8 << 8;
    rec->unk4 = 0x200 - base;
    rec->unk6 = 0x200 - base;
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words++ = base;
    *words++ = counter;
    *words = angle;
}
END_NONMATCH

NONMATCH("asm/code_08178064/obj_zoom_spin6_shrink_8754.s", void obj_zoom_spin6_shrink_8754(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 speed;
    s32 size;
    s32 angle;
    s32 counter;
    struct ObjRecord *rec;
    u32 *words;
    s16 *sb;
    s16 *cs;
    s16 *sn;
    s16 *cb;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
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
    speed = *words++;
    size = *words++;
    angle = *words++;
    counter = *words++;
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
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = z1;
        cs = &gCosineTable[angle & 0xFF];
        t = t * (*cs / 0x40);
        sb = gSineTable;
        sn = sb + (angle & 0xFF);
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        cb = gCosineTable;
        cs = cb + 32;
        t = t * (*cs / 0x40);
        y2 = (t - (sb[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = ((z2 * (*cs / 0x40)) + ((sb[32] / 0x40) * y1)) / 0x100;
        l.z = z3;

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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += speed;
    speed = (speed * 4) / 5;
    angle += 6;
    rec->unk4 = size;
    rec->unk6 = size;
    counter++;
    if (counter > 10) {
        size -= 0x20;
    }
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words++ = size;
    *words++ = angle;
    *words = counter;
}
END_NONMATCH

NONMATCH("asm/code_08178064/obj_zoom_spin6_shrink_89c0.s", void obj_zoom_spin6_shrink_89c0(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 speed;
    s32 size;
    s32 angle;
    s32 counter;
    struct ObjRecord *rec;
    u32 *words;
    s16 *sb;
    s16 *cs;
    s16 *sn;
    s16 *cb;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
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
    speed = *words++;
    size = *words++;
    angle = *words++;
    counter = *words++;
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
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = z1;
        cs = &gCosineTable[angle & 0xFF];
        t = t * (*cs / 0x40);
        sb = gSineTable;
        sn = sb + (angle & 0xFF);
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        cb = gCosineTable;
        cs = cb + 32;
        t = t * (*cs / 0x40);
        y2 = (t - (sb[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = ((z2 * (*cs / 0x40)) + ((sb[32] / 0x40) * y1)) / 0x100;
        l.z = z3;

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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += speed;
    speed = (speed * 7) / 8;
    angle += 6;
    rec->unk4 = size;
    rec->unk6 = size;
    counter++;
    if (counter > 10) {
        size -= 0x20;
    }
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words++ = size;
    *words++ = angle;
    *words = counter;
}
END_NONMATCH

NONMATCH("asm/code_08178064/obj_zoom_spin6_shrink_8c30.s", void obj_zoom_spin6_shrink_8c30(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 speed;
    s32 size;
    s32 angle;
    s32 counter;
    struct ObjRecord *rec;
    u32 *words;
    s16 *sb;
    s16 *cs;
    s16 *sn;
    s16 *cb;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
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
    speed = *words++;
    size = *words++;
    angle = *words++;
    counter = *words++;
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
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = z1;
        cs = &gCosineTable[angle & 0xFF];
        t = t * (*cs / 0x40);
        sb = gSineTable;
        sn = sb + (angle & 0xFF);
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        cb = gCosineTable;
        cs = cb + 32;
        t = t * (*cs / 0x40);
        y2 = (t - (sb[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = ((z2 * (*cs / 0x40)) + ((sb[32] / 0x40) * y1)) / 0x100;
        l.z = z3;

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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += speed;
    speed = (speed * 10) / 11;
    angle += 6;
    rec->unk4 = size;
    rec->unk6 = size;
    counter++;
    if (counter > 10) {
        size -= 0x20;
    }
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words++ = size;
    *words++ = angle;
    *words = counter;
}
END_NONMATCH

NONMATCH("asm/code_08178064/obj_zoom_spin6_shrink_8ea0.s", void obj_zoom_spin6_shrink_8ea0(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 speed;
    s32 size;
    s32 angle;
    s32 counter;
    struct ObjRecord *rec;
    u32 *words;
    s16 *sb;
    s16 *cs;
    s16 *sn;
    s16 *cb;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 vy;
    s32 vz;
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
    speed = *words++;
    size = *words++;
    angle = *words++;
    counter = *words++;
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
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (vy * zoom) / 0x100;
        l.y = y1;
        z1 = (vz * zoom) / 0x100;
        l.z = z1;
        t = z1;
        cs = &gCosineTable[angle & 0xFF];
        t = t * (*cs / 0x40);
        sb = gSineTable;
        sn = sb + (angle & 0xFF);
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        cb = gCosineTable;
        cs = cb + 32;
        t = t * (*cs / 0x40);
        y2 = (t - (sb[32] / 0x40) * z2) / 0x100;
        l.y = y2;
        z3 = ((z2 * (*cs / 0x40)) + ((sb[32] / 0x40) * y1)) / 0x100;
        l.z = z3;

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
        left--;
        if (left < 0) {
            break;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    zoom += speed;
    speed = (speed * 4) / 5;
    angle += 6;
    rec->unk4 = size;
    rec->unk6 = size;
    counter++;
    if (counter > 10) {
        size -= 0x20;
    }
    words = obj->data;
    *words++ = zoom;
    *words++ = speed;
    *words++ = size;
    *words++ = angle;
    *words = counter;
}
END_NONMATCH

// Six-point object that grows and falls: each point is zoomed (8.8 fixed point), tilted by the
// fixed table angle 12, shifted by the x offset and the fall height and drawn at the object
// (mirrored when flags bit 7 is set). Every frame the zoom grows by 2.0, gravity speeds up the
// fall, and the record's size (0xA0 - shrink) shrinks ever faster.
void obj_zoom_fixed6_accel_910c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    s32 zoom;
    s32 shrink;
    s32 shrinkSpeed;
    s32 offsetX;
    s32 fallY;
    s32 fallVel;
    struct ObjRecord *rec;
    u32 oamFlags;
    u32 modelSaved;
    s32 pointsLeft;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 py;
    s32 pz;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tiltY;
    s32 tiltZ;
    s32 shiftedX;
    s32 pos;
    s32 t;
    s32 size;

    // State: six words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrinkSpeed = *words++;
    offsetX = *words++;
    fallY = *words++;
    fallVel = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    pointsLeft = 5;
    for (;;) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        py = *words++;
        pt.y = py;
        pz = *words++;
        pt.z = pz;
        // Zoom it.
        zx = px * zoom / 0x100;
        pt.x = zx;
        zy = py * zoom / 0x100;
        pt.y = zy;
        zz = pz * zoom / 0x100;
        pt.z = zz;
        // Tilt it.
        t = zy;
        t = t * (gCosineTable[12] / 0x40);
        tiltY = (t - (gSineTable[12] / 0x40) * zz) / 0x100;
        pt.y = tiltY;
        tiltZ = (zz * (gCosineTable[12] / 0x40) + zy * (gSineTable[12] / 0x40)) / 0x100;
        pt.z = tiltZ;
        // Shift it and let it fall.
        shiftedX = zx + offsetX;
        pt.x = shiftedX;
        pt.y = tiltY + fallY;
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = shiftedX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = shiftedX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        t = obj->posY;
        pos = pt.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        pt.y = t + pos;
        t = obj->posZ;
        pos = pt.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        pt.z = t + pos;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
        pointsLeft--;
        if (pointsLeft < 0) {
            break;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += 0x200;
    fallY += fallVel;
    fallVel += 0x60; // gravity
    shrink += shrinkSpeed;
    shrinkSpeed++;
    size = 0xA0 - shrink;
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrinkSpeed;
    *words++ = offsetX;
    *words++ = fallY;
    *words = fallVel;
}

// Ten-point object that grows and falls: each point is zoomed (8.8 fixed point), tilted by the
// fixed table angle 12, lowered by the fall height and drawn at the object (mirrored when flags
// bit 7 is set). Every frame the zoom grows by 4.0, gravity speeds up the fall, and the record's
// size (0x140 - shrink) shrinks ever faster.
void obj_zoom_fixed10_accel_9304(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 zoom;
    s32 shrink;
    s32 shrinkSpeed;
    s32 fallY;
    s32 fallVel;
    struct ObjRecord *rec;
    u32 *words;
    u32 oamFlags;
    u32 modelSaved;
    s32 i;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 py;
    s32 pz;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tiltY;
    s32 tiltZ;
    s32 t;
    s32 pos;

    // State: five words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrinkSpeed = *words++;
    fallY = *words++;
    fallVel = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 10; i++) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        py = *words++;
        pt.y = py;
        pz = *words++;
        pt.z = pz;
        // Zoom it.
        zx = (px * zoom) / 0x100;
        pt.x = zx;
        zy = (py * zoom) / 0x100;
        pt.y = zy;
        zz = (pz * zoom) / 0x100;
        pt.z = zz;
        // Tilt it.
        t = zy;
        t = t * (gCosineTable[12] / 0x40);
        tiltY = (t - (gSineTable[12] / 0x40) * zz) / 0x100;
        pt.y = tiltY;
        tiltZ = (zz * (gCosineTable[12] / 0x40) + zy * (gSineTable[12] / 0x40)) / 0x100;
        pt.z = tiltZ;
        // Let it fall.
        pt.y = tiltY + fallY;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = zx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        t = obj->posY;
        pos = pt.y;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        pt.y = t + pos;
        t = obj->posZ;
        pos = pt.z;
        if (pos < 0) {
            pos += 0xFF;
        }
        pos >>= 8;
        pt.z = t + pos;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += 0x400;
    fallY += fallVel;
    fallVel += 0x60; // gravity
    shrink += shrinkSpeed;
    shrinkSpeed++;
    rec->unk4 = 0x140 - shrink;
    rec->unk6 = 0x140 - shrink;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrinkSpeed;
    *words++ = fallY;
    *words = fallVel;
}

// Four-point object that bounces like a ball: each point is zoomed (8.8 fixed point), turned by the
// fixed table angles 40 (around the vertical axis) and 16 (tilt), lifted by the bounce height and
// drawn at the object (mirrored when flags bit 7 is set). Every frame gravity pulls the bounce
// down; each landing halves the zoom speed, weakens the rebound and slows the spin, whose angle
// goes into rec->unkC.
void zoom_bounce_94fc(struct Point1Obj *obj)
{
    s16 *cosTab;
    s16 *sinTab;
    s16 *cosYaw;
    s16 *sinYaw;
    struct Point1Local pt;
    s32 zoom;
    s32 zoomVel;
    s32 bounceY;
    s32 bounceVel;
    s32 rebound;
    s32 spin;
    s32 spinSpeed;
    struct ObjRecord *rec;
    u32 *words;
    u32 oamFlags;
    u32 modelSaved;
    s32 pointsLeft;
    s32 *pY;
    s32 *pZ;
    u32 *pModel;
    s32 px;
    s32 py;
    s32 pz;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 yawX;
    s32 tiltY;
    s32 yawZ;
    s32 tiltZ;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    zoomVel = *words++;
    bounceY = *words++;
    bounceVel = *words++;
    rebound = *words++;
    spin = *words++;
    spinSpeed = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    pY = &pt.y;
    pZ = &pt.z;
    pModel = &pt.model;
    pointsLeft = 3;
    for (;;) {
        // The point's offset from the object.
        px = *words++;
        pt.x = px;
        py = *words++;
        pt.y = py;
        pz = *words++;
        pt.z = pz;
        // Zoom it.
        zx = (px * zoom) / 0x100;
        pt.x = zx;
        zy = (py * zoom) / 0x100;
        pt.y = zy;
        zz = (pz * zoom) / 0x100;
        pt.z = zz;
        // Turn it around the vertical axis...
        t = zz;
        cosTab = gCosineTable;
        cosYaw = cosTab + 40;
        t = t * (*cosYaw / 0x40);
        sinTab = gSineTable;
        sinYaw = &gSineTable[40];
        yawZ = (t - (*sinYaw / 0x40) * zx) / 0x100;
        pt.z = yawZ;
        yawX = (zx * (*cosYaw / 0x40) + (*sinYaw / 0x40) * zz) / 0x100;
        pt.x = yawX;
        // ...and tilt it.
        t = zy;
        t = t * (cosTab[16] / 0x40);
        tiltY = (t - (sinTab[16] / 0x40) * yawZ) / 0x100;
        pt.y = tiltY;
        tiltZ = ((yawZ * (cosTab[16] / 0x40)) + (sinTab[16] / 0x40) * zy) / 0x100;
        pt.z = tiltZ;
        pt.y = tiltY + bounceY;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = yawX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = yawX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, pY, pZ, 0);
        oam_place_points(pModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
        pointsLeft--;
        if (pointsLeft < 0) {
            break;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    zoom += zoomVel;
    bounceY += bounceVel;
    bounceVel += 0x60; // gravity
    if (bounceY > 0) {
        // Landed: bounce back up, weaker each time.
        zoomVel = zoomVel / 2;
        bounceY = 0;
        rebound += 2;
        if (rebound > 0) {
            rebound = 0;
        }
        bounceVel = rebound << 8;
        spinSpeed -= 2;
        if (spinSpeed < 0) {
            spinSpeed = 0;
        }
    }
    spin += spinSpeed;
    rec->unkC = ((obj->flags & 0x80) ? -spin : spin) << 8;
    rec->unk4 = 0x172;
    rec->unk6 = 0x172;
    words = obj->data;
    *words++ = zoom;
    *words++ = zoomVel;
    *words++ = bounceY;
    *words++ = bounceVel;
    *words++ = rebound;
    *words++ = spin;
    *words = spinSpeed;
}

/* A four-point model that grows, falls and bounces while spinning. Its state is seven words
   in obj->data (scale, scale speed, bounce height and velocity, rebound, spin, spin speed),
   followed by the points. Each point is scaled, turned by two fixed angles (table index 40
   about the vertical axis, 16 about the horizontal one), dropped by the bounce, mirrored
   when flags bit 7 is set, and placed. Each bounce halves the growth, takes 2 off the rebound
   and the spin speed. */
void zoom_bounce_97bc(struct Point1Obj *obj)
{
    s16 *cosTab;
    s16 *sinTab;
    s16 *cosYaw;
    s16 *sinYaw;
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 bounceY;
    s32 bounceVel;
    s32 rebound;
    s32 spin;
    s32 spinSpeed;
    struct ObjRecord *rec;
    u32 *words;
    u32 oamFlags;
    u32 modelSaved;
    s32 pointsLeft;
    s32 *ptY;
    s32 *ptZ;
    u32 *ptModel;
    s32 modelX;
    s32 modelY;
    s32 modelZ;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 yawX;
    s32 pitchY;
    s32 yawZ;
    s32 pitchZ;
    s32 t;
    s32 pos;

    words = obj->data;
    scale = *words++;
    scaleSpeed = *words++;
    bounceY = *words++;
    bounceVel = *words++;
    rebound = *words++;
    spin = *words++;
    spinSpeed = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oamFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, oamFlags);
    ptY = &pt.y;
    ptZ = &pt.z;
    ptModel = &pt.model;
    pointsLeft = 3;
    for (;;) {
        modelX = *words++;
        pt.x = modelX;
        modelY = *words++;
        pt.y = modelY;
        modelZ = *words++;
        pt.z = modelZ;
        scaledX = (modelX * scale) / 0x100;
        pt.x = scaledX;
        scaledY = (modelY * scale) / 0x100;
        pt.y = scaledY;
        scaledZ = (modelZ * scale) / 0x100;
        pt.z = scaledZ;
        t = scaledZ;
        cosTab = gCosineTable;
        cosYaw = cosTab + 40;
        t = t * (*cosYaw / 0x40);
        sinTab = gSineTable;
        sinYaw = &gSineTable[40];
        yawZ = (t - (*sinYaw / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosYaw / 0x40) + (*sinYaw / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;
        t = scaledY;
        t = t * (cosTab[16] / 0x40);
        pitchY = (t - (sinTab[16] / 0x40) * yawZ) / 0x100;
        pt.y = pitchY;
        pitchZ = ((yawZ * (cosTab[16] / 0x40)) + (sinTab[16] / 0x40) * scaledY) / 0x100;
        pt.z = pitchZ;
        pt.y = pitchY + bounceY;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = yawX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = yawX;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, ptY, ptZ, 0);
        oam_place_points(ptModel, oamFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
        pointsLeft--;
        if (pointsLeft < 0) {
            break;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    bounceY += bounceVel;
    bounceVel += 0x60; /* gravity */
    if (bounceY > 0) {
        /* hit the ground */
        scaleSpeed = scaleSpeed / 2;
        bounceY = 0;
        rebound += 2;
        if (rebound > 0) {
            rebound = 0;
        }
        bounceVel = rebound << 8;
        spinSpeed -= 2;
        if (spinSpeed < 0) {
            spinSpeed = 0;
        }
    }
    spin += spinSpeed;
    rec->unkC = ((obj->flags & 0x80) ? -spin : spin) << 8; /* rotation */
    rec->unk4 = 0x172;
    rec->unk6 = 0x172;
    words = obj->data;
    *words++ = scale;
    *words++ = scaleSpeed;
    *words++ = bounceY;
    *words++ = bounceVel;
    *words++ = rebound;
    *words++ = spin;
    *words = spinSpeed;
}
