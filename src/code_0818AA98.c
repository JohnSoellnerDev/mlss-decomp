#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4; /* sprite x scale, 8.8 (0x100 = 1:1) */
    u16 unk6; /* sprite y scale, 8.8 (0xFF00 = flipped) */
    u8 filler8[4];
    u16 unkC; /* sprite rotation angle, 8.8 in 1/256 turns */
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

// Places the one point in the object's data (an 8.8 offset, mirrored when the object is flipped) at
// the object's position, through the camera, and draws the sprite at half size (scale 0x80).
void obj_fixed_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 base;
    s32 offs;

    data = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = base - offs;
        } else {
            base = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = base + offs;
        }
        base = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = base + offs;
        base = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = base + offs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    rec->unk4 = 0x80;
    rec->unk6 = 0x80;
}

// Same code as obj_fixed_point: draws one point at the object's position plus the (x, y, z) offset
// in obj->data (8.8 fixed point, x mirrored when flags bit 7 is set), then sets the record's sprite
// scale (unk4 / unk6) to half size (0x80).
void obj_fixed_point_b(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 drawFlags;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pModel;
    s32 offsetX;
    s32 pos;
    s32 whole;

    data = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    drawFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, drawFlags);
    py = &pt.y;
    pz = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 1; i++) {
        offsetX = *data++;
        pt.x = offsetX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos - whole;
        } else {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos + whole;
        }
        pos = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = pos + whole;
        pos = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = pos + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pModel, drawFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        /* keep the model context as the first point left it */
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    rec->unk4 = 0x80;
    rec->unk6 = 0x80;
}

// Draws the one point in the object's data (an 8.8 (x, y, z) offset, x mirrored when flags bit 7
// is set) at the object's position, through the camera. Unlike obj_fixed_point it leaves the
// record's sprite scale alone.
void obj_place_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pModel;
    s32 offsetX;
    s32 pos;
    s32 whole;

    data = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 1; i++) {
        offsetX = *data++;
        pt.x = offsetX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos - whole;
        } else {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos + whole;
        }
        pos = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = pos + whole;
        pos = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = pos + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pModel, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        /* keep the model context as the first point left it */
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
}

// Same code as obj_place_point: draws the one point in the object's data (an 8.8 (x, y, z) offset,
// x mirrored when flags bit 7 is set) at the object's position, through the camera.
void obj_place_point_b(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pModel;
    s32 offsetX;
    s32 pos;
    s32 whole;

    data = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 1; i++) {
        offsetX = *data++;
        pt.x = offsetX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos - whole;
        } else {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos + whole;
        }
        pos = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = pos + whole;
        pos = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = pos + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pModel, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        /* keep the model context as the first point left it */
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
}

// Same code as obj_place_point: draws the one point in the object's data (an 8.8 (x, y, z) offset,
// x mirrored when flags bit 7 is set) at the object's position, through the camera.
void obj_place_point_c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pModel;
    s32 offsetX;
    s32 pos;
    s32 whole;

    data = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    pModel = &pt.model;
    for (i = 0; i < 1; i++) {
        offsetX = *data++;
        pt.x = offsetX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos - whole;
        } else {
            pos = obj->posX;
            whole = offsetX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = pos + whole;
        }
        pos = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = pos + whole;
        pos = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = pos + whole;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pModel, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        /* keep the model context as the first point left it */
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
}

/*
 * Draws 1 sprite that falls to the floor. obj->data holds a y offset and a y speed, then one x/y/z
 * point (8.8 fixed). The point is lifted by the offset, mirrored when the object is flipped, offset
 * by the object's position and placed on screen through the camera. Each frame the offset moves by
 * the speed and stops at 0 (the floor), and the speed grows by the gravity 0x40.
 */
void obj_fall_floor_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 base;
    s32 offs;
    s32 fallY;
    s32 fallSpeed;

    data = obj->data;
    fallY = *data++;
    fallSpeed = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;
        pt.y += fallY;
        if (obj->flags & 0x80) {
            base = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = base - offs;
        } else {
            base = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = base + offs;
        }
        base = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = base + offs;
        base = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = base + offs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    fallY += fallSpeed;
    if (fallY > 0) {
        fallY = 0;
    }
    fallSpeed += 0x40;
    data = obj->data;
    *data++ = fallY;
    *data = fallSpeed;
}

// One point that bounces: x and y move by their velocities (gravity 0x20), the floor at y = -0x800
// starts a new bounce with speed bounce * 0x100 (bounce rises to 0), and the angle (rec->unkC)
// turns by 6 per frame. The state is written back to the object's data.
void obj_bounce_point_b(struct Point1Obj *obj)
{
    struct Point1Local l;
    s32 xOff;
    s32 velX;
    s32 angle;
    struct ObjRecord *rec;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *words;
    u32 h;
    s32 i;
    s32 x;
    s32 pos;
    s32 t;
    s32 yOff;
    s32 velY;
    s32 bounce;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    yOff = *words++;
    velY = *words++;
    bounce = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(l.model, h);
    py = &l.y;
    pz = &l.z;
    pmodel = &l.model;
    for (i = 0; i < 1; i++) {
        /* the point's own offset (8.8 fixed point), mirrored when the object faces left */
        x = *words++;
        l.x = x;
        l.y = *words++;
        l.z = *words++;
        x += xOff;
        l.x = x;
        l.y += yOff;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
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
        offset_words_camera(&l.x, py, pz, 0);
        oam_place_points(pmodel, h, l.x, l.y, l.z << 14, 0, 0);
        if (!modelSaved) {
            l.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = l.model;
    }
    xOff += velX;
    yOff += velY;
    velY += 0x20;
    if (yOff > -0x800) {
        /* hit the floor: bounce */
        yOff = -0x800;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        velY = bounce << 8;
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
    *words++ = bounce;
    *words = angle;
}

/* Effect update: draws 6 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped over a quarter turn (64/256) around the x axis, with the sprites drawn at double size; state
   is 2 words in obj->data (scale, scale speed), followed by each piece's offset (x, y, z). */
void obj_pulse_model6_b(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 scale;
    s32 scaleSpeed;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 6; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        ry = (t - (gSineTable[64] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    scaleSpeed = (scaleSpeed * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/*
 * Draws 6 sprites that spread out, spin and grow. obj->data holds a scale, its growth per frame,
 * a turn angle (256 to a turn) and the sprites' size, then 6 x/y/z points. Each point is scaled by
 * scale/256, turned about the vertical axis, tipped a quarter turn about the x axis, mirrored when
 * the object is flipped, offset by the object's position and placed on screen through the camera.
 * Each frame the scale grows, the angle turns back by 3 and the size (rec->unk4/unk6, the sprite's
 * scale) grows by 8 up to 0x200.
 */
void obj_spin_model6(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleStep;
    s32 angle;
    s32 size;
    struct ObjRecord *rec;
    u32 *data;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 tmp;
    s32 offs;

    data = obj->data;
    scale = *data++;
    scaleStep = *data++;
    angle = *data++;
    size = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 6; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;

        /* turn about the vertical axis */
        rz = sz;
        cosEntry = &gCosineTable[angle & 0xFF];
        rz = rz * (*cosEntry / 0x40);
        sinEntry = &gSineTable[angle & 0xFF];
        rz = (rz - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* tip a quarter turn about the x axis */
        tmp = sy;
        tmp = tmp * (gCosineTable[64] / 0x40);
        pt.y = (tmp - (gSineTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = rx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = rx;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleStep;
    angle -= 3;
    size += 8;
    if (size > 0x200) {
        size = 0x200;
    }
    rec->unk4 = size;
    rec->unk6 = size;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleStep;
    *data++ = angle;
    *data = size;
}

// Draws 1 sprite at its point that pops in: obj->data holds the sprite's scale (8.8) and its
// growth per frame, then 1 x/y/z point. The scale grows by the rate; past 1.5 (0x180) the rate
// drops by 1 a frame, so it overshoots and shrinks back, and once shrinking it stops at 1:1
// (0x100). The scale goes to the record's x and y scale.
void obj_rise_point(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *pz;
    u32 *cursor;
    s32 *py;
    s32 srcX;
    s32 tmp;
    s32 offs;
    s32 scale;
    s32 growth;

    data = obj->data;
    scale = *data++;
    growth = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, pieceCount);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *data++;
        pt.x = srcX;
        pt.y = *data++;
        pt.z = *data++;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = tmp + offs;
        tmp = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = tmp + offs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    scale += growth;
    if (scale > 0x180) {
        growth--;
    }
    if (growth < 0) {
        if (scale <= 0xFF) {
            scale = 0x100;
        }
    }
    rec->unk4 = scale;
    rec->unk6 = scale;
    data = obj->data;
    *data++ = scale;
    *data = growth;
}

/* Effect update: draws 10 pieces that spread out at a steady speed (scale grows by the same step
   every frame), spin around the vertical axis (2/256 a frame) and are tilted by the fixed angle
   12/256 around the x axis; state is 3 words in obj->data (scale, scale speed, spin). */
void obj_spin_model10(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    struct ObjRecord *rec;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s16 *cosines;
    s16 *cosp;
    s16 *sinp;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 spinIdx;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    spin = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    /* each piece: its offset (x, y, z) follows the state words */
    for (piece = 0; piece < 10; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;
        /* spin around the vertical axis */
        rz = sz;
        cosines = gCosineTable;
        spinIdx = spin & 0xFF;
        cosp = cosines + spinIdx;
        rz = rz * (*cosp / 0x40);
        sinp = &gSineTable[spinIdx];
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        pt.y = (t - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = rx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = rx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    spin += 2;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state = spin;
}

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped a quarter turn (64/256) around the x axis, with sprites at double size; state is 2 words in
   obj->data (scale, scale speed). */
void obj_pulse_model8(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 scale;
    s32 scaleSpeed;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    /* each piece: its offset (x, y, z) follows the state words */
    for (piece = 0; piece < 8; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        ry = (t - (gSineTable[64] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    scaleSpeed = (scaleSpeed * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped a quarter turn (64/256) around the x axis and then an eighth turn (32/256) around the
   z axis, with sprites at double size; state is 2 words in obj->data (scale, scale speed). */
void obj_tilt45_model4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s16 *cosTable;
    s16 *sinTable;
    s16 *cosp;
    s16 *sinp;
    s16 *cosp2;
    s16 *sinp2;
    s32 sx;
    s32 scale;
    s32 scaleSpeed;
    struct ObjRecord *rec;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 rx;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    /* each piece: its offset (x, y, z) follows the state words */
    for (piece = 0; piece < 4; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;
        /* tip over a quarter turn around the x axis */
        t = sy;
        cosTable = gCosineTable;
        cosp = cosTable + 64;
        t = t * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + 64;
        ry = (t - (*sinp / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (*cosp / 0x40)) + ((*sinp / 0x40) * sy)) / 0x100;
        /* then an eighth turn around the z axis */
        t = sx;
        cosp2 = cosTable + 32;
        t = t * (*cosp2 / 0x40);
        sinp2 = sinTable + 32;
        rx = (t - (*sinp2 / 0x40) * ry) / 0x100;
        pt.x = rx;
        pt.y = ((ry * (*cosp2 / 0x40)) + ((*sinp2 / 0x40) * sx)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = rx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = rx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    scaleSpeed = (scaleSpeed * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* Effect update: draws 6 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped over a quarter turn (64/256) around the x axis, with the sprites drawn at double size; state
   is 2 words in obj->data (scale, scale speed), followed by each piece's offset (x, y, z). */
void obj_pulse_model6(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 scale;
    s32 scaleSpeed;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 6; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        ry = (t - (gSineTable[64] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[64] / 0x40)) + ((gSineTable[64] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = sx;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    scaleSpeed = (scaleSpeed * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* One point that spins while it shrinks: obj->data holds the rotation angle (1/256 turns) and the
 * x/y scale (8.8), then the point's 3 offset words. Every frame the angle grows by 0x10 and the
 * scale drops by 8; flag bit 7 (mirrored) turns the rotation and the x offset the other way. */
void obj_grow_point1(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rows;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 x;
    s32 pos;
    s32 ofs;
    s32 angle;
    s32 scale;

    state = obj->data;
    angle = *state++;
    scale = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        x = *state++;
        pt.x = x;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            ofs = x;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = pos - ofs;
        } else {
            pos = obj->posX;
            ofs = x;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = pos + ofs;
        }
        pos = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = pos + ofs;
        pos = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = pos + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rows, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    angle += 0x10;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    scale -= 8;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = angle;
    *state = scale;
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
