#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4; /* sprite x scale, 8.8 (0x100 = 1:1) */
    u16 unk6; /* sprite y scale, 8.8 */
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

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

extern s16 gSineTable[];
extern s16 gCosineTable[];

/* Effect update: draws one sprite piece at the object's position plus an 8.8 fixed point offset (x mirrored
   when flag 0x80 is set) and shrinks the sprite from 1.5x by 1/256 per frame. obj->data holds the age,
   then the piece's offset (x, y, z). */
void obj_point_shrink180_4f74(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 age;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    age++;
    rec->unk4 = 0x180 - age;
    rec->unk6 = 0x180 - age;
    state = obj->data;
    *state = age;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
// flag 0x80 is set) and shrinks the sprite from 1.5x by one more 1/256 step each frame (scale
// 0x180 - frames). State in obj->data: the frame count, then the piece's x, y, z.
void obj_point_shrink180_5088(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 frames;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    frames = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = base - offset;
        } else {
            base = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = base + offset;
        }
        base = obj->posY;
        offset = pt.y;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.y = base + offset;
        base = obj->posZ;
        offset = pt.z;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.z = base + offset;
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
    frames++;
    rec->unk4 = 0x180 - frames;
    rec->unk6 = 0x180 - frames;
    state = obj->data;
    *state = frames;
}

/*
 * Draws 1 sprite. obj->data holds a frame count, then one x/y/z point (8.8 fixed) that is mirrored
 * when the object is flipped, offset by the object's position and placed on screen through the
 * camera. Each frame the count goes up and the record's x and y scale (unk4/unk6) are set to
 * 0x200 - count, so the sprite shrinks from twice its size.
 */
void obj_point_shrink200_519c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    u32 frame;
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
    frame = *data++;
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
    frame++;
    rec->unk4 = 0x200 - frame;
    rec->unk6 = 0x200 - frame;
    data = obj->data;
    *data = frame;
}

/* Effect update: draws one sprite piece at the object's position plus an 8.8 fixed point offset (x mirrored
   when flag 0x80 is set) and shrinks the sprite from 2x by 1/256 per frame. obj->data holds the age,
   then the piece's offset (x, y, z). */
void obj_point_shrink200_52b0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 age;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    age++;
    rec->unk4 = 0x200 - age;
    rec->unk6 = 0x200 - age;
    state = obj->data;
    *state = age;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored
   when flag 0x80 is set), at a fixed size. obj->data holds the piece's offset (x, y, z). */
void obj_point_fixed_53c4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
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
 * Draws 1 sprite. obj->data holds a frame count, then one x/y/z point (8.8 fixed) that is mirrored
 * when the object is flipped, offset by the object's position and placed on screen through the
 * camera. Each frame the count goes up and the record's unk4/unk6 (x/y size) are set to
 * 0x180 - count and 0x100 - count, so the sprite shrinks from a wide start.
 */
void obj_point_shrink_wide_54b8(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    u32 frame;
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
    frame = *data++;
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
    frame++;
    rec->unk4 = 0x180 - frame;
    rec->unk6 = 0x100 - frame;
    data = obj->data;
    *data = frame;
}

ASM_FUNC("asm/code_08174F74/obj_zoom_spin4_55d0.s", void obj_zoom_spin4_55d0(struct Point1Obj *obj))

/*
 * Draws 6 sprites that zoom out and spin. obj->data holds a scale, its growth per frame and a turn
 * angle (256 to a turn), then 6 x/y/z points. Each point is scaled by scale/256, turned about the
 * vertical axis, tilted by 12/256 of a turn, mirrored when the object is flipped, offset by the
 * object's position and placed on screen through the camera. Each frame the scale grows by its
 * growth, which shrinks to 3/4, and the angle advances by 3.
 */
void obj_zoom_tilt6_58c0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    u32 *data;
    s16 *cosTable;
    s16 *sinTable;
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rz;
    s32 rx;
    s32 ty;
    s32 tz;
    s32 tmp;
    s32 offs;

    data = obj->data;
    scale = *data++;
    scaleVel = *data++;
    angle = *data++;
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
        cosTable = gCosineTable;
        cosEntry = cosTable + (angle & 0xFF);
        rz = rz * (*cosEntry / 0x40);
        sinTable = gSineTable;
        sinEntry = sinTable + (angle & 0xFF);
        rz = (rz - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (cosTable[12] / 0x40);
        ty = (tmp - (sinTable[12] / 0x40) * rz) / 0x100;
        pt.y = ty;
        tz = (rz * (cosTable[12] / 0x40) + ((sinTable[12] / 0x40) * sy)) / 0x100;
        pt.z = tz;

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
    scale += scaleVel;
    scaleVel = (scaleVel * 3) / 4;
    angle += 3;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = angle;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
// flag 0x80 is set) and shrinks the sprite from 1:1 by one more 1/256 step each frame (scale
// 0x100 - frames). State in obj->data: the frame count, then the piece's x, y, z.
void obj_point_shrink_5b20(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 frames;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    frames = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = base - offset;
        } else {
            base = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = base + offset;
        }
        base = obj->posY;
        offset = pt.y;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.y = base + offset;
        base = obj->posZ;
        offset = pt.z;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.z = base + offset;
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
    frames++;
    rec->unk4 = 0x100 - frames;
    rec->unk6 = 0x100 - frames;
    state = obj->data;
    *state = frames;
}

/* Effect update: draws one sprite piece at the object's position plus an 8.8 fixed point offset (x mirrored
   when flag 0x80 is set) and shrinks the sprite from 1.25x by 1/256 per frame. obj->data holds the age,
   then the piece's offset (x, y, z). */
void obj_point_shrink140_5c34(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 age;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    age++;
    rec->unk4 = 0x140 - age;
    rec->unk6 = 0x140 - age;
    state = obj->data;
    *state = age;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and shrinks the sprite from 1.5x by 1/256 per frame (scale 0x180 - age).
   obj->data holds the age, then the piece's offset (x, y, z). */
void obj_point_shrink180_5d48(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 age;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    age++;
    rec->unk4 = 0x180 - age;
    rec->unk6 = 0x180 - age;
    state = obj->data;
    *state = age;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and shrinks the sprite from 1.75x by 1/256 per frame (scale 0x1C0 - age).
   obj->data holds the age, then the piece's offset (x, y, z). */
void obj_point_shrink1c0_5e5c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 age;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base + ofs;
        }
        base = obj->posY;
        ofs = pt.y;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        base = obj->posZ;
        ofs = pt.z;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = base + ofs;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    age++;
    rec->unk4 = 0x1C0 - age;
    rec->unk6 = 0x1C0 - age;
    state = obj->data;
    *state = age;
}

ASM_FUNC("asm/code_08174F74/obj_zoom_spin5_size100_5f70.s", void obj_zoom_spin5_size100_5f70(void))

ASM_FUNC("asm/code_08174F74/obj_zoom_spin5_size1c0_6260.s", void obj_zoom_spin5_size1c0_6260(void))

ASM_FUNC("asm/code_08174F74/obj_zoom_spin5_size200_6550.s", void obj_zoom_spin5_size200_6550(void))
