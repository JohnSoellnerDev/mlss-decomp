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

extern s16 gCosineTable[];
extern s16 gSineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

// Spawns a point-model object. Parameters 5 to 13 are passed on the stack as one block.
void fx_obj_spawn(u32 x, u32 y, u32 z, u32 type, void (*callback)(), u32 attr, u32 mode,
                  u32 unk0C, u32 points, u32 flagA, u32 flagB, const void *table, u32 count);

extern const u8 gSpawnStartGrowPoint1[];
extern const u8 gSpawnStartPulseModel6[];
extern const u8 gSpawnStartTilt45Model4[];
extern const u8 gSpawnStartPulseModel8[];
extern const u8 gSpawnStartSpinModel10[];
extern const u8 gSpawnStartRisePoint[];
extern const u8 gSpawnStartSpinModel6[];

extern void obj_spin_model6(struct Point1Obj *obj);

// One fixed point at the object position; the object record gets a 0x80 x 0x80 size.
void obj_fixed_point(struct Point1Obj *obj)
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
    rec->unk4 = 0x80;
    rec->unk6 = 0x80;
}

// Same code as obj_fixed_point.
void obj_fixed_point_b(struct Point1Obj *obj)
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
    rec->unk4 = 0x80;
    rec->unk6 = 0x80;
}

// Places one point at the object position without changing the object's state.
void obj_place_point(struct Point1Obj *obj)
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
}

// Same code as obj_place_point.
void obj_place_point_b(struct Point1Obj *obj)
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
}

// Same code as obj_place_point.
void obj_place_point_c(struct Point1Obj *obj)
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
}

// One point that falls with gravity 0x40 per frame and stays on the floor (y <= 0).
void obj_fall_floor_point(struct Point1Obj *obj)
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
    s32 yOff;
    s32 vy;

    words = obj->data;
    yOff = *words++;
    vy = *words++;
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
        l.y += yOff;
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
    yOff += vy;
    if (yOff > 0) {
        yOff = 0;
    }
    vy += 0x40;
    words = obj->data;
    *words++ = yOff;
    *words = vy;
}

// One point that bounces: x and y move by their velocities, the floor at y = -0x800 starts a new
// bounce, and the angle turns by 6 per frame. The state is written back to the object's data.
void obj_bounce_point_b(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 xOff;
    s32 velX;
    s32 angle;
    struct ObjRecord *rec;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    u32 *words;
    u32 h;
    s32 i;
    s32 vx;
    s32 pos;
    s32 t;
    s32 yOff;
    s32 velY;
    s32 level;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    yOff = *words++;
    velY = *words++;
    level = *words++;
    angle = *words++;
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
        vx += xOff;
        l.x = vx;
        l.y += yOff;
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
    xOff += velX;
    yOff += velY;
    velY += 0x20;
    if (yOff > -0x800) {
        yOff = -0x800;
        level++;
        if (level > 0) {
            level = 0;
        }
        velY = level << 8;
    }
    angle += 6;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = yOff;
    *words++ = velY;
    *words++ = level;
    *words = angle;
}

void obj_pulse_model6_b(struct Point1Obj *obj)
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 vel;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
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
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x1;
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
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}

void obj_spin_model6(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 zoom;
    s32 step;
    s32 angle;
    s32 size;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s16 *cs;
    s16 *sn;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    angle = *words++;
    size = *words++;
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
        z2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        z2 = z2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (z2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[64] / 0x40);
        l.y = (t - (gSineTable[64] / 0x40) * z2) / 0x100;
        l.z = ((z2 * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;

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
    zoom += step;
    angle -= 3;
    size += 8;
    if (size > 0x200) {
        size = 0x200;
    }
    rec->unk4 = size;
    rec->unk6 = size;
    words = obj->data;
    *words++ = zoom;
    *words++ = step;
    *words++ = angle;
    *words = size;
}

// One point that rises: y grows by vy, vy slows above 384 and y snaps to 256 when falling
// near the bottom. The position is also stored as halfwords in the object record.
void obj_rise_point(struct Point1Obj *obj)
{
    struct Point1Local l;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p2;
    u32 *pslot;
    s32 *p1;
    s32 vx;
    s32 pos;
    s32 t;
    s32 yPos;
    s32 vy;

    words = obj->data;
    yPos = *words++;
    vy = *words++;
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
    yPos += vy;
    if (yPos > 0x180) {
        vy--;
    }
    if (vy < 0) {
        if (yPos <= 0xFF) {
            yPos = 0x100;
        }
    }
    rec->unk4 = yPos;
    rec->unk6 = yPos;
    words = obj->data;
    *words++ = yPos;
    *words = vy;
}

NONMATCH("asm/code_0818AA98/obj_spin_model10.s", void obj_spin_model10(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 step;
    s32 angle;
    struct ObjRecord *rec;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s16 *cs;
    s16 *sn;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    step = *words++;
    angle = *words++;
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
        l.y = *words++;
        l.z = *words++;
        x1 = (vx * zoom) / 0x100;
        l.x = x1;
        y1 = (l.y * zoom) / 0x100;
        l.y = y1;
        z1 = (l.z * zoom) / 0x100;
        l.z = z1;
        z2 = z1;
        cs = &gCosineTable[angle & 0xFF];
        z2 = z2 * (*cs / 0x40);
        sn = &gSineTable[angle & 0xFF];
        z2 = (z2 - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        l.y = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;

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
    zoom += step;
    angle += 2;
    words = obj->data;
    *words++ = zoom;
    *words++ = step;
    *words = angle;
}
END_NONMATCH

void obj_pulse_model8(struct Point1Obj *obj)
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 vel;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 8; i++) {
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
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x1;
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
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}

NONMATCH("asm/code_0818AA98/obj_tilt45_model4.s", void obj_tilt45_model4(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 x1;
    s32 zoom;
    s32 vel;
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
    s32 y1;
    s32 z1;
    s32 y2;
    s32 x2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (i = 0; i < 4; i++) {
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
        cs = &gCosineTable[64];
        t = t * (*cs / 0x40);
        y2 = (t - (gSineTable[64] / 0x40) * z1) / 0x100;
        l.y = y2;
        l.z = ((z1 * (*cs / 0x40)) + ((gSineTable[64] / 0x40) * y1)) / 0x100;
        t = x1;
        t = t * (gCosineTable[32] / 0x40);
        x2 = (t - (gSineTable[32] / 0x40) * y2) / 0x100;
        l.x = x2;
        l.y = ((y2 * (gCosineTable[32] / 0x40)) + ((gSineTable[32] / 0x40) * x1)) / 0x100;

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
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}
END_NONMATCH

void obj_pulse_model6(struct Point1Obj *obj)
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 vel;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 y2;
    s32 t;
    s32 pos;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
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
            t = obj->posX;
            pos = x1;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x1;
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
    vel = (vel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    words = obj->data;
    *words++ = zoom;
    *words = vel;
}

// One point that grows: zoom +0x10 and step -8 per frame. Bit 7 of the flags reverses the
// direction of the x offset and of the halfword at +0x0C in the object record.
void obj_grow_point1(struct Point1Obj *obj)
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
    s32 zoom;
    s32 step;

    words = obj->data;
    zoom = *words++;
    step = *words++;
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
    zoom += 0x10;
    if (obj->flags & 0x80) {
        rec->unkC = (-zoom) << 8;
    } else {
        rec->unkC = zoom << 8;
    }
    step -= 8;
    rec->unk4 = step;
    rec->unk6 = step;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}

void spawn_tbl_6b60(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x20, obj_grow_point1, 0x4027, 0, 0, 1, flag, flag,
                 gSpawnStartGrowPoint1, 2);
}

void spawn_tbl_6b58(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_pulse_model6, 0x415D, 1, 0, 6, flag, 0,
                 gSpawnStartPulseModel6, 2);
}

void spawn_tbl_6b50(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_tilt45_model4, 0x415D, 1, 0, 4, flag, 0,
                 gSpawnStartTilt45Model4, 2);
}

void spawn_tbl_6b48(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_pulse_model8, 0x415D, 1, 0, 8, flag, 0,
                 gSpawnStartPulseModel8, 2);
}

void obj_spin_model10(struct Point1Obj *obj);

void spawn_tbl_6b3c(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0, obj_spin_model10, 0x415B, 5, 0, 0xA, flag, 0,
                 gSpawnStartSpinModel10, 3);
}

void spawn_tbl_6b34(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x8D, obj_rise_point, 0x402B, 1, 0, 1, flag, flag,
                 gSpawnStartRisePoint, 2);
}

void spawn_tbl_6b24(u32 x, u32 y, u32 z, u32 flag)
{
    fx_obj_spawn(x, y, z, 0x22, obj_spin_model6, 0x404D, 0, 0, 6, flag, flag,
                 gSpawnStartSpinModel6, 4);
}
