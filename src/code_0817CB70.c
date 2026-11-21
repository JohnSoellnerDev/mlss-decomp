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
extern s16 gCosineTable[];
extern s16 gSineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set), raised by 12 px and moved 1 px in z, and shrinks the sprite by 0x20/256 per frame. State in
// obj->data: the sprite scale, then the piece's x, y, z.
void obj_point_shrink_cb70(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offsetX;
    s32 offsetZ;
    s32 base;
    s32 offset;
    u32 spriteScale;

    state = obj->data;
    spriteScale = *state++;
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
        offsetZ = *state++;
        pt.y -= 0xC00;
        pt.z = offsetZ - 0x100;
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
    spriteScale -= 0x20;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state = spriteScale;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   POINT1_OBJ_MIRRORED is set), at a fixed 0xA0/0x100 scale. obj->data holds an extra x offset, which
   slides the piece left by half a pixel (0x80) each frame, then the piece's offset (x, y, z). */
void obj_point_slide_x_oam_a0_cc8c(struct Point1Obj *obj)
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
    s32 slide;
    s32 slidX;

    state = obj->data;
    slide = *state++;
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
        slidX = offX + slide;
        pt.x = slidX;
        if (obj->flags & POINT1_OBJ_MIRRORED) {
            base = obj->posX;
            ofs = slidX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = slidX;
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
    slide -= 0x80;
    rec->unk4 = 0xA0;
    rec->unk6 = 0xA0;
    state = obj->data;
    *state = slide;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   POINT1_OBJ_MIRRORED is set), at a fixed 0x160/0x100 scale. obj->data holds an extra x offset, which
   slides the piece left by half a pixel (0x80) each frame, then the piece's offset (x, y, z). */
void obj_point_slide_x_oam_160_cd9c(struct Point1Obj *obj)
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
    s32 slide;
    s32 slidX;

    state = obj->data;
    slide = *state++;
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
        slidX = offX + slide;
        pt.x = slidX;
        if (obj->flags & POINT1_OBJ_MIRRORED) {
            base = obj->posX;
            ofs = slidX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = slidX;
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
    slide -= 0x80;
    rec->unk4 = 0x160;
    rec->unk6 = 0x160;
    state = obj->data;
    *state = slide;
}


/* One point that drifts at a constant speed, drawn at 3/4 size (record x/y scale 0xC0, 8.8).
 * State is 4 words in obj->data (offset x, speed x, offset y, speed y), then the point's 3 offset
 * words; the offsets grow by their speeds every frame. */
void obj_drift_point_ceac(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rows;
    s32 velX;
    s32 velY;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 x;
    s32 y;
    s32 offX;
    s32 offY;
    s32 pos;
    s32 ofs;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
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
        y = *state++;
        pt.y = y;
        pt.z = *state++;
        x += offX;
        pt.x = x;
        y += offY;
        pt.y = y;
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
    offX += velX;
    offY += velY;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state = velY;
}


/*
 * Effect update: draws 1 piece that drifts at a constant speed, its sprite at 3/4 size (scale
 * 0xC0). The offset in x and y grows by its speed every frame; flags bit 7 mirrors it in x.
 * State is 4 words in obj->data (x offset, x speed, y offset, y speed), then 3 words (x, y, z)
 * for the piece.
 */
void obj_drift_point_cfe0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rowCount;
    s32 velX;
    s32 velY;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 pieceX;
    s32 pieceY;
    s32 offX;
    s32 offY;
    s32 base;
    s32 ofs;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        pieceX = *state++;
        pt.x = pieceX;
        pieceY = *state++;
        pt.y = pieceY;
        pt.z = *state++;
        pieceX += offX;
        pt.x = pieceX;
        pieceY += offY;
        pt.y = pieceY;
        /* flags bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = pieceX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = pieceX;
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
    offX += velX;
    offY += velY;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state = velY;
}


/* One point that drifts at a constant speed, drawn at 3/4 size (record x/y scale 0xC0, 8.8).
 * State is 4 words in obj->data (offset x, speed x, offset y, speed y), then the point's 3 offset
 * words; the offsets grow by their speeds every frame. */
void obj_drift_point_d114(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 rows;
    s32 velX;
    s32 velY;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 x;
    s32 y;
    s32 offX;
    s32 offY;
    s32 pos;
    s32 ofs;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
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
        y = *state++;
        pt.y = y;
        pt.z = *state++;
        x += offX;
        pt.x = x;
        y += offY;
        pt.y = y;
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
    offX += velX;
    offY += velY;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state = velY;
}


/*
 * Draws one sprite that drifts. obj->data holds a base x and its velocity, a base y and its
 * velocity, then one x/y/z point. The point is moved by the base, mirrored when the object faces
 * left, offset by the object's position and placed on screen through the camera at 3/4 scale. Each
 * frame the base moves by its velocity.
 */
void obj_drift_point_d248(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    struct ObjRecord *rec;
    u32 pieceCount;
    s32 velX;
    s32 velY;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 baseX;
    s32 baseY;
    s32 pos;
    s32 offs;

    data = obj->data;
    baseX = *data++;
    velX = *data++;
    baseY = *data++;
    velY = *data++;
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
        srcY = *data++;
        pt.y = srcY;
        pt.z = *data++;
        srcX += baseX;
        pt.x = srcX;
        srcY += baseY;
        pt.y = srcY;
        if (obj->flags & POINT1_OBJ_MIRRORED) {
            pos = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = pos - offs;
        } else {
            pos = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            pt.x = pos + offs;
        }
        pos = obj->posY;
        offs = pt.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.y = pos + offs;
        pos = obj->posZ;
        offs = pt.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        pt.z = pos + offs;
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
    baseX += velX;
    baseY += velY;
    rec->unk4 = 0xC0; /* 3/4 scale */
    rec->unk6 = 0xC0;
    data = obj->data;
    *data++ = baseX;
    *data++ = velX;
    *data++ = baseY;
    *data = velY;
}


// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when the
// object faces left), with a drift added to the x offset that grows by a fixed speed every frame. The sprite
// is drawn at 0xC0/256 scale. State in obj->data: the x drift, its speed, then the piece's x, y, z.
void obj_drift_x_point_d37c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    struct ObjRecord *rec;
    u32 attr;
    s32 driftSpeed;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 pieceX;
    s32 offsetX;
    s32 driftX;
    s32 base;
    s32 offset;

    state = obj->data;
    driftX = *state++;
    driftSpeed = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        pieceX = *state++;
        pt.x = pieceX;
        pt.y = *state++;
        pt.z = *state++;
        offsetX = pieceX + driftX;
        pt.x = offsetX;
        if (obj->flags & POINT1_OBJ_MIRRORED) {
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
    driftX += driftSpeed;
    rec->unk4 = 0xC0;
    rec->unk6 = 0xC0;
    state = obj->data;
    *state++ = driftX;
    *state = driftSpeed;
}


// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
// POINT1_OBJ_MIRRORED is set), at a constant sprite scale. State in obj->data: the sprite scale (8.8,
// used for x and y), then the piece's x, y, z.
void obj_sized_point_d494(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
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
    u32 spriteScale;

    state = obj->data;
    spriteScale = *state++;
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
        if (obj->flags & POINT1_OBJ_MIRRORED) {
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
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state = spriteScale;
}


/*
 * Draws a 5-part sprite model that falls while it grows and shrinks: obj->data holds the
 * zoom (8.8 fixed point), the shrink and its speed (the parts' scale is 0xA0 minus the
 * shrink), an X offset, a drop and its speed, then the 5 parts' (x, y, z) offsets. Each
 * offset is zoomed, tilted about the X axis by angle 12, shifted by the X offset and the
 * drop, mirrored in X if obj->flags & 0x80, and placed as OAM at the object's position.
 * Each frame the zoom grows by 0x400, the drop speeds up by 0x60 and the shrink speed by 1.
 */
void obj_tilt_model5_d5a0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    s32 zoom;
    s32 drop;
    s32 shrink;
    s32 shrink_speed;
    s32 x_offset;
    s32 drop_speed;
    struct ObjRecord *rec;
    u32 oam_count;
    u32 cursor_reset;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 part_x;
    s32 part_y;
    s32 part_z;
    s32 scaled_x;
    s32 scaled_y;
    s32 scaled_z;
    s32 tilted_y;
    s32 shifted_x;
    s32 t;
    s32 ofs_x;

    data = obj->data;
    zoom = *data++;
    shrink = *data++;
    shrink_speed = *data++;
    x_offset = *data++;
    drop = *data++;
    drop_speed = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    oam_count = block_flag_byte(rec);
    cursor_reset = 0;
    oam_list_rows_b(pt.model, oam_count);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 5; i++) {
        part_x = *data++;
        pt.x = part_x;
        part_y = *data++;
        pt.y = part_y;
        part_z = *data++;
        pt.z = part_z;
        scaled_x = (part_x * zoom) / 0x100;
        pt.x = scaled_x;
        scaled_y = (part_y * zoom) / 0x100;
        pt.y = scaled_y;
        scaled_z = (part_z * zoom) / 0x100;
        pt.z = scaled_z;
        t = scaled_y;
        t = t * (gCosineTable[12] / 0x40);
        tilted_y = (t - (gSineTable[12] / 0x40) * scaled_z) / 0x100;
        pt.y = tilted_y;
        pt.z = ((scaled_z * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * scaled_y)) / 0x100;
        shifted_x = x_offset + scaled_x;
        pt.x = shifted_x;
        pt.y = tilted_y + drop;
        if (obj->flags & 0x80) {
            t = obj->posX;
            ofs_x = shifted_x;
            if (ofs_x < 0) {
                ofs_x += 0xFF;
            }
            ofs_x >>= 8;
            pt.x = t - ofs_x;
        } else {
            t = obj->posX;
            ofs_x = shifted_x;
            if (ofs_x < 0) {
                ofs_x += 0xFF;
            }
            ofs_x >>= 8;
            pt.x = t + ofs_x;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, oam_count, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursor_reset) {
            pt.model = gModelCtx->model;
            cursor_reset = 1;
        }
    }
    if (cursor_reset) {
        gModelCtx->model = pt.model;
    }
    zoom += 0x400;
    drop += drop_speed;
    drop_speed += 0x60;
    shrink += shrink_speed;
    shrink_speed += 1;
    rec->unk4 = 0xA0 - shrink;
    rec->unk6 = 0xA0 - shrink;
    data = obj->data;
    *data++ = zoom;
    *data++ = shrink;
    *data++ = shrink_speed;
    *data++ = x_offset;
    *data++ = drop;
    *data = drop_speed;
}


// Five-point object that grows and falls: each point is zoomed (8.8 fixed point), tilted by the
// fixed table angle 12, shifted by the x offset and the fall height and drawn at the object
// (mirrored when flags bit 7 is set). Every frame the zoom grows by 4.0, gravity speeds up the
// fall, and the record's size (0xA0 - shrink) shrinks ever faster. Same code as the function above.
void obj_tilt_model5_d798(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    s32 zoom;
    s32 fall_y;
    s32 shrink;
    s32 shrink_speed;
    s32 offset_x;
    s32 fall_speed;
    struct ObjRecord *rec;
    u32 oam_flags;
    u32 model_saved;
    s32 i;
    s32 *p_y;
    s32 *p_z;
    u32 *p_model;
    s32 px;
    s32 py;
    s32 pz;
    s32 zx;
    s32 zy;
    s32 zz;
    s32 tilt_y;
    s32 shifted_x;
    s32 t;
    s32 pos;

    // State: six words in obj->data, followed by the points.
    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrink_speed = *words++;
    offset_x = *words++;
    fall_y = *words++;
    fall_speed = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    oam_flags = block_flag_byte(rec);
    model_saved = 0;
    oam_list_rows_b(pt.model, oam_flags);
    p_y = &pt.y;
    p_z = &pt.z;
    p_model = &pt.model;
    for (i = 0; i < 5; i++) {
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
        tilt_y = (t - (gSineTable[12] / 0x40) * zz) / 0x100;
        pt.y = tilt_y;
        pt.z = ((zz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * zy)) / 0x100;
        // Shift it and let it fall.
        shifted_x = offset_x + zx;
        pt.x = shifted_x;
        pt.y = tilt_y + fall_y;
        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = shifted_x;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t - pos;
        } else {
            t = obj->posX;
            pos = shifted_x;
            if (pos < 0) {
                pos += 0xFF;
            }
            pos >>= 8;
            pt.x = t + pos;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, p_y, p_z, 0);
        oam_place_points(p_model, oam_flags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!model_saved) {
            pt.model = gModelCtx->model;
            model_saved = 1;
        }
    }
    if (model_saved) {
        gModelCtx->model = pt.model;
    }
    zoom += 0x400;
    fall_y += fall_speed;
    fall_speed += 0x60; // gravity
    shrink += shrink_speed;
    shrink_speed += 1;
    rec->unk4 = 0xA0 - shrink;
    rec->unk6 = 0xA0 - shrink;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrink_speed;
    *words++ = offset_x;
    *words++ = fall_y;
    *words = fall_speed;
}

/*
 * Effect update: four sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that shrinks ever faster, spun by a yaw angle that turns 8 steps a
 * frame, tilted by a fixed pitch (angle 12) and lifted by a Y offset that
 * rises 2 px a frame, then drawn around the object.
 */
void obj_spin_model4_d990(struct Point1Obj *obj)
{
    s16 *cosTable;
    s16 *sinTable;
    s16 *cosYaw;
    s16 *sinYaw;
    s16 *cosPitch;
    s16 *sinPitch;
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 shrinkSpeed;
    s32 angle;
    s32 liftY;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 offZ;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 yawX;
    s32 yawZ;
    s32 pitchY;
    s32 pitchZ;
    s32 tmp;
    s32 tmpZ;

    state = obj->data;
    zoom = *state++;
    shrinkSpeed = *state++;
    angle = *state++;
    liftY = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 4; i++) {
        offX = *state++;
        pt.x = offX;
        offY = *state++;
        pt.y = offY;
        offZ = *state++;
        pt.z = offZ;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (offY * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (offZ * zoom) / 0x100;
        pt.z = scaledZ;

        /* yaw by the spin angle: rotate in the X/Z plane */
        tmpZ = scaledZ;
        cosTable = gCosineTable;
        cosYaw = cosTable + (angle & 0xFF);
        tmpZ = tmpZ * (*cosYaw / 0x40);
        sinTable = gSineTable;
        sinYaw = sinTable + (angle & 0xFF);
        yawZ = (tmpZ - (*sinYaw / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*cosYaw / 0x40) + (*sinYaw / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* fixed pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        cosPitch = cosTable + 12;
        tmp = tmp * (*cosPitch / 0x40);
        sinPitch = sinTable + 12;
        pitchY = (tmp - (*sinPitch / 0x40) * yawZ) / 0x100;
        pt.y = pitchY;
        pitchZ = (yawZ * (*cosPitch / 0x40) + (*sinPitch / 0x40) * scaledY) / 0x100;
        pt.z = pitchZ;
        pitchY += liftY;
        pt.y = pitchY;

        pt.x = obj->posX + yawX / 0x100;
        pt.y = obj->posY + pitchY / 0x100;
        pt.z = obj->posZ + pitchZ / 0x100;
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
    tmp = tmpZ;
    zoom -= shrinkSpeed;
    shrinkSpeed += 0x28;
    angle += 8;
    liftY -= 0x200;
    state = obj->data;
    *state++ = zoom;
    *state++ = shrinkSpeed;
    *state++ = angle;
    *state = liftY;
}

/*
 * Draws a growing burst of 8 sprites. obj->data holds the burst's scale, scale velocity and
 * angle, then 8 x/y/z points. Each point is scaled by scale/256, rotated about the vertical
 * axis by the angle, tilted by a fixed angle (12/256 of a turn), mirrored when the object is
 * flipped, offset by the object's position and placed on screen through the camera.
 * Afterwards the burst grows by its velocity (which decays by 7/8); the angle stays the same.
 */
void obj_zoom_model8_dbf0(struct Point1Obj *obj)
{
    s16 *cosEntry;
    s16 *sinEntry;
    struct Point1Local pt;
    s32 scale;
    s32 scaleVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 *data;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 angleIdx;
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
    for (i = 0; i < 8; i++) {
        srcX = *data++;
        pt.x = srcX;
        srcY = *data++;
        pt.y = srcY;
        srcZ = *data++;
        pt.z = srcZ;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
        pt.z = sz;

        /* rotate about the vertical axis */
        ry = sz;
        angleIdx = angle & 0xFF;
        cosEntry = &gCosineTable[angleIdx];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[angleIdx];
        rz = (ry - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (gCosineTable[12] / 0x40);
        ry = (tmp - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.y = ry - 0x800;

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
    scaleVel = (scaleVel * 7) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = angle;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   POINT1_OBJ_MIRRORED is set), raised by 8 px, at a fixed 0x1C0/0x100 scale. obj->data holds only the
   piece's offset (x, y, z); nothing changes from frame to frame. */
void obj_fixed_point_de6c(struct Point1Obj *obj)
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
        pt.y -= 0x800;
        if (obj->flags & POINT1_OBJ_MIRRORED) {
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
    rec->unk4 = 0x1C0;
    rec->unk6 = 0x1C0;
}


// Effect update: draws one sprite piece at the object's position plus its 8.8 offset, raised by
// 8 px (x mirrored when flag 0x80 is set), at twice the normal size. obj->data holds only the
// piece's offset (x, y, z).
void obj_fixed_point_df7c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
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
        pt.y -= 0x800; /* 8 px up */
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
    rec->unk4 = 0x200; /* 2x scale */
    rec->unk6 = 0x200;
}


/*
 * Effect update: eight sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying velocity (7/8 per frame), pitched by a fixed
 * angle 64 in the Y/Z plane, then spun in the X/Y plane by an angle that
 * advances by 3 every frame, and drawn around the object.
 */
void obj_perm_spin8_e08c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    s32 spin;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 offZ;
    s32 scaledX;
    s32 scaledY;
    s32 scaledZ;
    s32 spinX;
    s32 pitchY;
    s32 pitchZ;
    s32 spinY;
    s32 tmp;
    s32 prod;

    state = obj->data;
    zoom = *state++;
    zoomVel = *state++;
    spin = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 8; i++) {
        offX = *state++;
        pt.x = offX;
        offY = *state++;
        pt.y = offY;
        offZ = *state++;
        pt.z = offZ;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (offY * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (offZ * zoom) / 0x100;
        pt.z = scaledZ;

        /* pitch: rotate in the Y/Z plane by a fixed angle */
        tmp = scaledY;
        tmp = tmp * (gCosineTable[64] / 0x40);
        pitchY = (tmp - (gSineTable[64] / 0x40) * scaledZ) / 0x100;
        pt.y = pitchY;
        pitchZ = (scaledZ * (gCosineTable[64] / 0x40) + (gSineTable[64] / 0x40) * scaledY) / 0x100;
        pt.z = pitchZ;

        /* spin: rotate in the X/Y plane by the current angle */
        prod = scaledX;
        prod = prod * (gCosineTable[spin & 0xFF] / 0x40);
        spinX = (prod - (gSineTable[spin & 0xFF] / 0x40) * pitchY) / 0x100;
        pt.x = spinX;
        spinY = (pitchY * (gCosineTable[spin & 0xFF] / 0x40) + (gSineTable[spin & 0xFF] / 0x40) * scaledX) / 0x100;
        pt.y = spinY;

        pt.x = obj->posX + spinX / 0x100;
        pt.y = obj->posY + spinY / 0x100;
        pt.z = obj->posZ + pitchZ / 0x100;
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
    tmp = prod;
    zoom += zoomVel;
    zoomVel = (zoomVel * 7) / 8;
    spin += 3;
    state = obj->data;
    *state++ = zoom;
    *state++ = zoomVel;
    *state = spin;
}

// Model of 9 points tilted about the X axis that flies off sideways. Each frame the zoom
// grows, the height (the OAM size is 0x140 minus it) and the Y offset rise with speeds that
// accelerate, and the X speed grows by 6; it is stored in the record facing the object's
// direction (flag 0x80). The data block holds the zoom, the height and its speed, the Y
// offset and its speed, the X speed, then the 9 model points (x, y, z).
void obj_accel_tilt9_e2c4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    s32 zoom;
    s32 height;
    s32 height_speed;
    s32 offset_y;
    s32 offset_y_speed;
    s32 x_speed;
    struct ObjRecord *rec;
    u32 flag_byte;
    u32 model_saved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 zoomed_x;
    s32 zoomed_y;
    s32 zoomed_z;
    s32 tmp;
    s32 dx;

    words = obj->data;
    zoom = *words++;
    height = *words++;
    height_speed = *words++;
    offset_y = *words++;
    offset_y_speed = *words++;
    x_speed = *words++;
    rec = obj->rec;
    pt.model = rec->model;
    flag_byte = block_flag_byte(rec);
    model_saved = 0;
    oam_list_rows_b(pt.model, flag_byte);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 9; i++) {
        vx = *words++;
        pt.x = vx;
        vy = *words++;
        pt.y = vy;
        vz = *words++;
        pt.z = vz;

        // Scale by the zoom (8.8 fixed point).
        zoomed_x = (vx * zoom) / 0x100;
        pt.x = zoomed_x;
        zoomed_y = (vy * zoom) / 0x100;
        pt.y = zoomed_y;
        zoomed_z = (vz * zoom) / 0x100;
        pt.z = zoomed_z;

        // Rotate about the X axis by angle 12 of the sine table.
        tmp = zoomed_y;
        tmp = tmp * (gCosineTable[12] / 0x40);
        pt.y = (tmp - (gSineTable[12] / 0x40) * zoomed_z) / 0x100;
        pt.z = ((zoomed_z * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * zoomed_y)) / 0x100;
        pt.y = pt.y + offset_y;

        // Mirror X when the object faces the other way.
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            dx = zoomed_x;
            if (dx < 0) {
                dx += 0xFF;
            }
            dx >>= 8;
            pt.x = tmp - dx;
        } else {
            tmp = obj->posX;
            dx = zoomed_x;
            if (dx < 0) {
                dx += 0xFF;
            }
            dx >>= 8;
            pt.x = tmp + dx;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, flag_byte, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!model_saved) {
            pt.model = gModelCtx->model;
            model_saved = 1;
        }
    }
    if (model_saved) {
        gModelCtx->model = pt.model;
    }

    zoom += 0x400;
    offset_y += offset_y_speed;
    offset_y_speed += 0x60;
    height += height_speed;
    height_speed += 1;
    rec->unk4 = 0x140 - height;
    rec->unk6 = 0x140 - height;
    x_speed += 6;
    rec->unkC = (obj->flags & 0x80) ? -x_speed << 8 : x_speed << 8;

    words = obj->data;
    *words++ = zoom;
    *words++ = height;
    *words++ = height_speed;
    *words++ = offset_y;
    *words++ = offset_y_speed;
    *words = x_speed;
}

