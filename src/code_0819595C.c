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

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

extern s16 gCosineTable[];
extern s16 gSineTable[];

NONMATCH("asm/code_0819595C/obj_zoom_model8_tilt22_595c.s", void obj_zoom_model8_tilt22_595c(struct Point1Obj *obj))
{
    struct Point1Local l;
    struct ObjRecord *rec;
    s32 zoom;
    s32 step;
    u32 *words;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 vx;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 t;
    s32 pos;
    s32 y1;

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
        t = z1;
        t = t * (gCosineTable[32] / 0x40);
        z2 = (t - (gSineTable[32] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[32] / 0x40) + (gSineTable[32] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[16] / 0x40);
        l.y = (t - (gSineTable[16] / 0x40) * z2) / 0x100;
        t = z2;
        t = t * (gCosineTable[16] / 0x40);
        l.z = (t + ((gSineTable[16] / 0x40) * y1)) / 0x100;

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
    step = (step * 2) / 3;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}
END_NONMATCH

/*
 * Draws the object's one point (an offset in 1/256 pixels from its position, mirrored when
 * flags bit 7 flips it), then grows it: data[1] (the scale) gains data[0] (the growth rate),
 * which shrinks to 6/7 each frame. The record's x scale gets the new scale, its y scale a third.
 */
void obj_point1_decay_5ba4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    s32 growth;
    s32 scale;
    struct ObjRecord *rec;
    u32 blockFlags;
    u32 modelSaved;
    s32 i;
    s32 *pz;
    u32 *pmodel;
    s32 *py;
    s32 offsetX;
    s32 pos;
    s32 offset;

    data = obj->data;
    growth = *data++;
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
    scale += growth;
    growth = growth * 6 / 7;
    rec->unk4 = scale;
    rec->unk6 = scale / 3;
    data = obj->data;
    *data++ = growth;
    *data = scale;
}

NONMATCH("asm/code_0819595C/obj_point1_zoom23_acc_5cd0.s", void obj_point1_zoom23_acc_5cd0(struct Point1Obj *obj))
{
    struct Point1Local l;
    u32 *words;
    s32 zoom;
    s32 phase;
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

    words = obj->data;
    zoom = *words++;
    phase = *words++;
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
    phase = zoom + phase;
    zoom = 2 * zoom / 3;
    rec->unk4 = phase;
    rec->unk6 = phase / 3;
    words = obj->data;
    *words++ = zoom;
    *words = phase;
}
END_NONMATCH

/*
 * Effect update: draws 1 piece at its offset from obj->data (mirrored in x if obj->flags bit 7 is
 * set) and grows the sprite: the scale rises by its speed every frame and the speed drops to 4/5.
 * obj->data: scale speed, scale (8.8), then the piece's x, y, z (8.8).
 */
void obj_point1_decay45_5e00(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 scaleSpeed;
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
    scaleSpeed = *state++;
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

        /* flags bit 7: mirrored in x */
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
    scale += scaleSpeed;
    scaleSpeed = scaleSpeed * 4 / 5;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state++ = scaleSpeed;
    *state = scale;
}

/* Effect update: draws one piece at its offset from the object (mirrored when flags bit 7 is set)
   while its sprite grows by a speed that slows to 6/7 each frame; state is 2 words in obj->data
   (grow speed, sprite scale), followed by the piece's offset (x, y, z). */
void obj_point1_decay67_5f1c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 growSpeed;
    u32 spriteScale;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 pos;
    s32 t;

    state = obj->data;
    growSpeed = *state++;
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
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = srcX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = srcX;
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
    spriteScale += growSpeed;
    growSpeed = growSpeed * 6 / 7;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = growSpeed;
    *state = spriteScale;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and grows (or shrinks) the sprite's scale by a constant step each frame.
   obj->data holds the step, the current scale, then the piece's offset (x, y, z). */
void obj_point1_sum_6040(struct Point1Obj *obj)
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

/*
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * (mirrored in X when flag 0x80 is set) and grows its scale by a constant speed
 * every frame. State in obj->data: the scale speed, the scale, then the offset (x, y, z).
 */
void obj_point1_sum_6150(struct Point1Obj *obj)
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
 * Draws the object's one point (an offset in 1/256 pixels from its position, mirrored when
 * flags bit 7 flips it), then grows it: data[1] (the scale) gains data[0] (the constant growth
 * rate). The record's x scale gets the new scale, its y scale a third.
 */
void obj_point1_div3_6260(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    u32 growth;
    s32 scale;
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
    growth = *data++;
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
    scale += growth;
    rec->unk4 = scale;
    rec->unk6 = scale / 3;
    data = obj->data;
    *data++ = growth;
    *data = scale;
}

/*
 * Draws the object's one point (an offset in 1/256 pixels from its position, mirrored when
 * flags bit 7 flips it), then grows it: data[1] (the scale) gains data[0] (the growth rate)
 * each frame. The record's x scale gets the new scale, its y scale half of it.
 */
void obj_point1_half_637c(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *data;
    u32 growth;
    s32 scale;
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
    growth = *data++;
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
    scale += growth;
    rec->unk4 = scale;
    rec->unk6 = scale / 2;
    data = obj->data;
    *data++ = growth;
    *data = scale;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and grows (or shrinks) the sprite's scale by a constant step each frame.
   obj->data holds the step, the current scale, then the piece's offset (x, y, z). */
void obj_point1_sum_6494(struct Point1Obj *obj)
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
void obj_point1_sum_65a4(struct Point1Obj *obj)
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

/*
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * (mirrored in X when flag 0x80 is set) and changes its scale by a fixed step
 * every frame. State in obj->data: the scale step, the scale, then the offset
 * (x, y, z).
 */
void obj_point1_sum_66b4(struct Point1Obj *obj)
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

/*
 * Effect update: draws 1 piece at its fixed offset while the record's sprite scales up and spins:
 * the scale (x and y alike) grows by the zoom speed and the angle turns by 2/256 every frame
 * (the other way when flags bit 7 mirrors it). State is 3 words in obj->data (zoom speed, scale,
 * angle), then 3 words (x, y, z) for the piece.
 */
void obj_point1_turn_p2_67c4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 zoomVel;
    u32 scale;
    s32 angle;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 pieceX;
    s32 base;
    s32 ofs;

    state = obj->data;
    zoomVel = *state++;
    scale = *state++;
    angle = *state++;
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
        pt.y = *state++;
        pt.z = *state++;
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
    scale += zoomVel;
    angle += 2;
    rec->unk4 = scale;
    rec->unk6 = scale;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    state = obj->data;
    *state++ = zoomVel;
    *state++ = scale;
    *state = angle;
}

// Effect update for one point that grows and spins: each frame the sprite's x/y scale (8.8) grows by
// the scale speed and its angle turns by -4/256 turn (mirrored when flags bit 7 is set).
// State in obj->data: scale speed, scale, angle, then the point's offset (x, y, z).
void obj_point1_turn_m4_6900(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 scaleSpeed;
    u32 scale;
    s32 angle;
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
    scaleSpeed = *state++;
    scale = *state++;
    angle = *state++;
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
        /* flags bit 7: mirrored in x */
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
    scale += scaleSpeed;
    angle += -4;
    rec->unk4 = scale;
    rec->unk6 = scale;
    if (obj->flags & 0x80) {
        rec->unkC = (-angle) << 8;
    } else {
        rec->unkC = angle << 8;
    }
    state = obj->data;
    *state++ = scaleSpeed;
    *state++ = scale;
    *state = angle;
}

/*
 * Draws a growing burst of 8 sprites. obj->data holds the burst's scale, scale velocity and
 * angle, then 8 x/y/z points. Each point is scaled by scale/256, rotated about the vertical
 * axis by the angle, tilted by a fixed angle (12/256 of a turn), mirrored when the object is
 * flipped, offset by the object's position and placed on screen through the camera.
 * Afterwards the burst grows by its velocity (which decays by 7/8); the angle stays the same.
 */
void obj_zoom_model8_turn_6a40(struct Point1Obj *obj)
{
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
    s16 *cosEntry;
    s16 *sinEntry;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rz;
    s32 rx;
    s32 ry;
    s32 tiltZ;
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
        pt.y = *data++;
        pt.z = *data++;

        /* scale */
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (pt.y * scale) / 0x100;
        pt.y = sy;
        sz = (pt.z * scale) / 0x100;
        pt.z = sz;

        /* rotate about the vertical axis */
        ry = sz;
        cosEntry = &gCosineTable[angle & 0xFF];
        ry = ry * (*cosEntry / 0x40);
        sinEntry = &gSineTable[angle & 0xFF];
        rz = (ry - (*sinEntry / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosEntry / 0x40) + (*sinEntry / 0x40) * sz) / 0x100;
        pt.x = rx;

        /* fixed tilt towards the camera */
        tmp = sy;
        tmp = tmp * (gCosineTable[12] / 0x40);
        ry = (tmp - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        tiltZ = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.z = tiltZ;
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
    rec->unk4 = 0x104;
    rec->unk6 = 0x104;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = angle;
}

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   turned by a fixed yaw, tilted by a fixed angle (12/256) and drawn 8 px higher; the record's size
   is set to 0x104. State is 3 words in obj->data (scale, scale speed, yaw). */
void obj_zoom_model8_turn_6cbc(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 yaw;
    struct ObjRecord *rec;
    u32 *state;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s16 *cosp;
    s16 *sinp;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rz;
    s32 rx;
    s32 ry;
    s32 tz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    yaw = *state++;
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
        /* turn around the vertical axis */
        ry = sz;
        cosp = &gCosineTable[yaw & 0xFF];
        ry = ry * (*cosp / 0x40);
        sinp = &gSineTable[yaw & 0xFF];
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        tz = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
        pt.z = tz;
        pt.y = ry - 0x800;

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
    rec->unk4 = 0x104;
    rec->unk6 = 0x104;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state = yaw;
}

NONMATCH("asm/code_0819595C/obj_pulse_tilt_model4.s", void obj_pulse_tilt_model4(struct Point1Obj *obj))
{
    struct Point1Local l;
    s32 zoom;
    s32 step;
    struct ObjRecord *rec;
    u32 h;
    u32 saved;
    s32 i;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    u32 *words;
    s32 vx;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 z2;
    s32 x2;
    s32 y2;
    s32 z3;
    s32 t;
    s32 pos;

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
        t = z1;
        t = t * (gCosineTable[64] / 0x40);
        z2 = (t - (gSineTable[64] / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (gCosineTable[64] / 0x40) + (gSineTable[64] / 0x40) * z1) / 0x100;
        l.x = x2;
        t = y1;
        t = t * (gCosineTable[12] / 0x40);
        y2 = (t - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        t = z2;
        t = t * (gCosineTable[12] / 0x40);
        z3 = (t + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.z = z3;
        l.y = y2 - 0x800;

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
    step = (step * 7) / 8;
    rec->unk4 = 0xF0;
    rec->unk6 = 0xF0;
    words = obj->data;
    *words++ = zoom;
    *words = step;
}
END_NONMATCH
