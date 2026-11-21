#include "global.h"

/* the sprite an effect object draws */
struct ObjRecord
{
    u8 filler0[4];
    u16 unk4;           /* sprite scale x, 8.8 fixed point (0x100 = normal size) */
    u16 unk6;           /* sprite scale y */
    u8 filler8[4];
    u16 unkC;
    u8 filler10[0x34 - 0x10];
    u32 model;          /* first OAM node of the sprite's parts */
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

/*
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * (mirrored in X when flag 0x80 is set) and grows its scale by a constant speed
 * every frame. State in obj->data: the scale speed, the scale, then the offset (x, y, z).
 */
void obj_point1_sum_46f8(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    u32 scaleSpeed;
    u32 scale;
    struct ObjRecord *rec;
    u32 blockFlags;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offsetX;
    s32 pos;
    s32 offset;

    data = obj->data;
    scaleSpeed = *data++;
    scale = *data++;
    rec = obj->rec;
    pt.model = rec->model;
    blockFlags = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, blockFlags);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offsetX = *data++;
        pt.x = offsetX;
        pt.y = *data++;
        pt.z = *data++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = pos - offset;
        } else {
            pos = obj->posX;
            offset = offsetX;
            if (offset < 0) {
                offset += 0xFF;
            }
            offset >>= 8;
            pt.x = pos + offset;
        }
        pos = obj->posY;
        offset = pt.y;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.y = pos + offset;
        pos = obj->posZ;
        offset = pt.z;
        if (offset < 0) {
            offset += 0xFF;
        }
        offset >>= 8;
        pt.z = pos + offset;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(pmodel, blockFlags, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    rec->unk4 = scale;
    rec->unk6 = scale;
    data = obj->data;
    *data++ = scaleSpeed;
    *data = scale;
}

/*
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * (mirrored in X when flag 0x80 is set) and grows it: the scale goes up by a
 * fixed step every frame. State in obj->data: the scale step, the scale, then
 * the offset (x, y, z).
 */
void obj_point1_sum_4808(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 scaleStep;
    u32 scale;
    struct ObjRecord *rec;
    u32 partCount;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offsetX;
    s32 pos;
    s32 ofs;

    state = obj->data;
    scaleStep = *state++;
    scale = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            ofs = offsetX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = pos - ofs;
        } else {
            pos = obj->posX;
            ofs = offsetX;
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
        oam_place_points(pmodel, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleStep;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = scaleStep;
    *state = scale;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and grows (or shrinks) the sprite's scale by a constant step each frame.
   obj->data holds the step, the current scale, then the piece's offset (x, y, z). */
void obj_point1_sum_4918(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 scaleStep;
    u32 scale;
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
    scaleStep = *state++;
    scale = *state++;
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
    scale += scaleStep;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = scaleStep;
    *state = scale;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and grows (or shrinks) the sprite's scale by a constant step each frame.
   obj->data holds the step, the current scale, then the piece's offset (x, y, z). */
void obj_point1_sum_4a28(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 scaleStep;
    u32 scale;
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
    scaleStep = *state++;
    scale = *state++;
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
    scale += scaleStep;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = scaleStep;
    *state = scale;
}
