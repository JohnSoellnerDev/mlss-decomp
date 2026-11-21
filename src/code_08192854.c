#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 unk4; /* sprite scale X (0x100 = 1:1) */
    u16 unk6; /* sprite scale Y */
    u8 filler8[4];
    u16 unkC; /* sprite rotation (angle in 1/256 turns, << 8) */
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

extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 6/7 each frame),
   spin around the vertical axis (backwards, 4/256 a frame) and are tipped over a quarter turn
   (64/256) around the x axis, while each sprite shrinks by 6/256 a frame; state is 4 words in
   obj->data (scale, scale speed, spin, sprite scale). */
void obj_zoom_model8_turn_rem6(struct Point1Obj *obj)
{
    struct Point1Local pt;
    s32 scale;
    s32 scaleSpeed;
    s32 spin;
    s32 spriteScale;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 *state;
    s16 *sinTable;
    s16 *cosp;
    s16 *sinp;
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 t;
    s32 pos;

    state = obj->data;
    scale = *state++;
    scaleSpeed = *state++;
    spin = *state++;
    spriteScale = *state++;
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
        /* spin around the vertical axis */
        rz = sz;
        cosp = &gCosineTable[spin & 0xFF];
        rz = rz * (*cosp / 0x40);
        sinTable = gSineTable;
        sinp = sinTable + (spin & 0xFF);
        rz = (rz - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        t = t * (gCosineTable[64] / 0x40);
        pt.y = (t - (sinTable[64] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[64] / 0x40)) + ((sinTable[64] / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 6) / 7;
    spin -= 4;
    spriteScale -= 6;
    rec->unk4 = spriteScale;
    rec->unk6 = spriteScale;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state++ = spin;
    *state = spriteScale;
}

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 2/3 each
   frame), turned by angle 0 around the vertical axis and tipped over a quarter turn (64/256)
   around the x axis so they lie flat; state is 2 words in obj->data (scale, scale speed),
   followed by each piece's offset (x, y, z). */
void obj_zoom_model8_2aa8(struct Point1Obj *obj)
{
    s16 *cosTurn;
    s16 *sinTurn;
    s16 *cosTip;
    s16 *sinTip;
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
    s32 sz;
    s32 rx;
    s32 rz;
    s32 t;
    s32 pos;
    s32 sy;

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
        /* turn around the vertical axis (angle 0) */
        t = sz;
        cosTurn = &gCosineTable[0];
        t = t * (*cosTurn / 0x40);
        sinTurn = &gSineTable[0];
        rz = (t - (*sinTurn / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosTurn / 0x40) + (*sinTurn / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip over a quarter turn around the x axis */
        t = sy;
        cosTip = &gCosineTable[64];
        t = t * (*cosTip / 0x40);
        sinTip = &gSineTable[64];
        pt.y = (t - (*sinTip / 0x40) * rz) / 0x100;
        pt.z = (rz * (*cosTip / 0x40) + ((*sinTip / 0x40) * sy)) / 0x100;

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
    scaleSpeed = (scaleSpeed * 2) / 3;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   turned by a fixed yaw, tilted by a fixed angle (12/256) and drawn 8 px higher; the record's size
   is set to 0x118. State is 3 words in obj->data (scale, scale speed, yaw). */
void obj_pulse_tilt_turn4(struct Point1Obj *obj)
{
    s16 *cosp;
    s16 *sinp;
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
    s32 srcX;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rx;
    s32 ry;
    s32 rz;
    s32 yawIdx;
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
        /* turn around the vertical axis */
        ry = sz;
        yawIdx = yaw & 0xFF;
        cosp = &gCosineTable[yawIdx];
        ry = ry * (*cosp / 0x40);
        sinp = &gSineTable[yawIdx];
        rz = (ry - (*sinp / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (*cosp / 0x40) + (*sinp / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tilt around the x axis */
        t = sy;
        t = t * (gCosineTable[12] / 0x40);
        ry = (t - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;
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
    rec->unk4 = 0x118;
    rec->unk6 = 0x118;
    state = obj->data;
    *state++ = scale;
    *state++ = scaleSpeed;
    *state = yaw;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
// POINT1_OBJ_MIRRORED is set), then damps the zoom kept in the state to 31/32 of its value.
// State in obj->data: the zoom, then the piece's x, y, z.
void obj_point1_decay31_2f58(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 zoom;
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
    zoom = *state++;
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
    zoom = zoom * 31 / 32;
    state = obj->data;
    *state = zoom;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored
// when the object faces left), then damps the value kept in obj->data[0] to 31/32 of itself.
// State in obj->data: that value, then the piece's x, y, z.
void obj_point1_decay31_3064(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 decay;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    decay = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
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
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    /* decay * 31 / 32, rounded toward zero */
    offset = (decay << 5) - decay;
    if (offset < 0) {
        offset += 0x1F;
    }
    decay = offset >> 5;
    state = obj->data;
    *state = decay;
}

/* Effect update: draws one sprite at the object plus its offset (8.8 fixed point, mirrored in X
   when flag 0x80 is set) and makes it shrink and spin: each frame the sprite scale drops by the
   shrink step plus 6, the step halves and the angle goes back 6/256 of a turn. State is 3 words
   in obj->data (scale, shrink step, angle), followed by the offset (x, y, z). */
void obj_point1_zoom_step_half_3170(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 scale;
    s32 shrink;
    s32 angle;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 pos;
    s32 ofs;

    state = obj->data;
    scale = *state++;
    shrink = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    attr = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            ofs = offX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = pos - ofs;
        } else {
            pos = obj->posX;
            ofs = offX;
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
        oam_place_points(pmodel, attr, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale = scale - shrink - 6;
    shrink = ((s32)((u32)shrink >> 31) + shrink) >> 1; /* shrink / 2 */
    angle -= 6;
    rec->unk4 = scale;
    rec->unk6 = scale;
    if (obj->flags & 0x80) {
        rec->unkC = -angle << 8;
    } else {
        rec->unkC = angle << 8;
    }
    state = obj->data;
    *state++ = scale;
    *state++ = shrink;
    *state = angle;
}

NONMATCH("asm/code_08192854/obj_point1_zoom_step35_32b8.s", void obj_point1_zoom_step35_32b8(struct Point1Obj *obj))
{
    s32 flagBit = 0x80;
    struct Point1Local l;
    s32 zoom;
    u32 *words;
    s32 step;
    s32 phase;
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
    zoom = *words++;
    step = *words++;
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
        if (obj->flags & flagBit) {
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
    zoom = zoom - step + 6;
    step = (step * 3) / 5;
    phase += 6;
    rec->unk4 = zoom;
    rec->unk6 = zoom;
    if (obj->flags & 0x80) {
        rec->unkC = -phase << 8;
    } else {
        rec->unkC = phase << 8;
    }
    words = obj->data;
    *words++ = zoom;
    *words++ = step;
    *words = phase;
}
END_NONMATCH

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set) and shrinks the sprite from 1.25x (0x140) by 1/256 per frame. State in obj->data: a word that
// is kept as it is, the age, then the piece's x, y, z.
void obj_point1_rem140_3404(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 keptWord;
    u32 age;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    keptWord = *state++;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
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
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    age += 1;
    rec->unk4 = 0x140 - age;
    rec->unk6 = 0x140 - age;
    state = obj->data;
    *state++ = keptWord;
    *state = age;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set) and shrinks the sprite from 1.25x (0x140) by 1/256 per frame. State in obj->data: a word that
// is kept as it is, the age, then the piece's x, y, z.
void obj_point1_rem140(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 keptWord;
    u32 age;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    keptWord = *state++;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
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
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    age += 1;
    rec->unk4 = 0x140 - age;
    rec->unk6 = 0x140 - age;
    state = obj->data;
    *state++ = keptWord;
    *state = age;
}

/* Effect update: draws one sprite at its offset (8.8 fixed point) from the object (mirrored
   in X when flag 0x80 is set) while it shrinks by 1/256 a frame from a scale of 0x168; state
   is 2 words in obj->data (unused here, frame count) followed by the sprite's offset (x, y, z). */
void obj_point1_rem168_3644(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 unused;
    u32 frames;
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
    unused = *state++;
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
    frames += 1;
    rec->unk4 = 0x168 - frames;
    rec->unk6 = 0x168 - frames;
    state = obj->data;
    *state++ = unused;
    *state = frames;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set) and shrinks the sprite from 1.41x (0x168) by 1/256 per frame. State in obj->data: a word that
// is kept as it is, the age, then the piece's x, y, z.
void obj_point1_rem168_3764(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 keptWord;
    u32 age;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    keptWord = *state++;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
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
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    age += 1;
    rec->unk4 = 0x168 - age;
    rec->unk6 = 0x168 - age;
    state = obj->data;
    *state++ = keptWord;
    *state = age;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set) and shrinks the sprite from 1.56x (0x190) by 1/256 per frame. State in obj->data: a word that
// is kept as it is, the age, then the piece's x, y, z.
void obj_point1_rem190_3884(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 keptWord;
    u32 age;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    keptWord = *state++;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
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
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    age += 1;
    rec->unk4 = 0x190 - age;
    rec->unk6 = 0x190 - age;
    state = obj->data;
    *state++ = keptWord;
    *state = age;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set) and shrinks the sprite from 1.56x (0x190) by 1/256 per frame. State in obj->data: a word that
// is kept as it is, the age, then the piece's x, y, z.
void obj_point1_rem190_39a4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 keptWord;
    u32 age;
    struct ObjRecord *rec;
    u32 partCount;
    u32 cursorSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    keptWord = *state++;
    age = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
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
        oam_place_points(pcursor, partCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!cursorSaved) {
            pt.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = pt.model;
    }
    age += 1;
    rec->unk4 = 0x190 - age;
    rec->unk6 = 0x190 - age;
    state = obj->data;
    *state++ = keptWord;
    *state = age;
}

/*
 * Effect update: six sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying velocity (2/3 per frame), rotated by yaw
 * angle 0 and pitch angle 64, then drawn around the object (mirrored in X
 * when flag 0x80 is set).
 */
void obj_zoom_model6(struct Point1Obj *obj)
{
    s16 *yawCos;
    s16 *yawSin;
    s16 *pitchCos;
    s16 *pitchSin;
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomVel;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 scaledX;
    s32 scaledZ;
    s32 yawX;
    s32 pitchY;
    s32 yawZ;
    s32 tmp;
    s32 ofs;
    s32 scaledY;

    state = obj->data;
    zoom = *state++;
    zoomVel = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 6; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (pt.y * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (pt.z * zoom) / 0x100;
        pt.z = scaledZ;

        /* yaw: rotate in the X/Z plane */
        tmp = scaledZ;
        yawCos = &gCosineTable[0];
        tmp = tmp * (*yawCos / 0x40);
        yawSin = &gSineTable[0];
        yawZ = (tmp - (*yawSin / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*yawCos / 0x40) + (*yawSin / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        pitchCos = &gCosineTable[64];
        tmp = tmp * (*pitchCos / 0x40);
        pitchSin = &gSineTable[64];
        pitchY = (tmp - (*pitchSin / 0x40) * yawZ) / 0x100;
        pt.y = pitchY;
        pt.z = (yawZ * (*pitchCos / 0x40) + ((*pitchSin / 0x40) * scaledY)) / 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = yawX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = yawX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp + ofs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
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
    zoom += zoomVel;
    zoomVel = (zoomVel * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomVel;
}

/*
 * Effect update: eight sprites whose offsets (8.8 fixed point) are scaled by a
 * zoom that grows by a decaying step (2/3 per frame), rotated by a yaw of angle 0
 * and a pitch of angle 64 (laid flat), then drawn around the object (mirrored
 * in X when flag 0x80 is set).
 */
void obj_zoom_model8(struct Point1Obj *obj)
{
    s16 *yawCos;
    s16 *yawSin;
    s16 *pitchCos;
    s16 *pitchSin;
    struct Point1Local pt;
    struct ObjRecord *rec;
    s32 zoom;
    s32 zoomStep;
    u32 *state;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 scaledX;
    s32 scaledZ;
    s32 yawX;
    s32 yawZ;
    s32 tmp;
    s32 ofs;
    s32 scaledY;

    state = obj->data;
    zoom = *state++;
    zoomStep = *state++;
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
        pt.y = *state++;
        pt.z = *state++;

        /* scale the offset by zoom / 256 */
        scaledX = (offX * zoom) / 0x100;
        pt.x = scaledX;
        scaledY = (pt.y * zoom) / 0x100;
        pt.y = scaledY;
        scaledZ = (pt.z * zoom) / 0x100;
        pt.z = scaledZ;

        /* yaw: rotate in the X/Z plane */
        tmp = scaledZ;
        yawCos = &gCosineTable[0];
        tmp = tmp * (*yawCos / 0x40);
        yawSin = &gSineTable[0];
        yawZ = (tmp - (*yawSin / 0x40) * scaledX) / 0x100;
        pt.z = yawZ;
        yawX = (scaledX * (*yawCos / 0x40) + (*yawSin / 0x40) * scaledZ) / 0x100;
        pt.x = yawX;

        /* pitch: rotate in the Y/Z plane */
        tmp = scaledY;
        pitchCos = &gCosineTable[64];
        tmp = tmp * (*pitchCos / 0x40);
        pitchSin = &gSineTable[64];
        pt.y = (tmp - (*pitchSin / 0x40) * yawZ) / 0x100;
        pt.z = (yawZ * (*pitchCos / 0x40) + ((*pitchSin / 0x40) * scaledY)) / 0x100;

        if (obj->flags & 0x80) {
            tmp = obj->posX;
            ofs = yawX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp - ofs;
        } else {
            tmp = obj->posX;
            ofs = yawX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = tmp + ofs;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
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
    zoom += zoomStep;
    zoomStep = (zoomStep * 2) / 3;
    state = obj->data;
    *state++ = zoom;
    *state = zoomStep;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set) and grows (or shrinks) the sprite's scale by a constant step each frame.
   obj->data holds the step, the current scale, then the piece's offset (x, y, z). */
void obj_point1_sum_3f2c(struct Point1Obj *obj)
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
   obj->data holds the step, the current scale, then the piece's offset (x, y, z). Same code as
   obj_point1_sum_3f2c, used by another effect. */
void obj_point1_sum_403c(struct Point1Obj *obj)
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

