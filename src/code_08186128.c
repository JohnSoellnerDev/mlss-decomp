#include "global.h"

struct PtRecord
{
    u8 filler0[4];
    u16 unk4; /* sprite x scale, 8.8 (0x100 = 1:1) */
    u16 unk6; /* sprite y scale, 8.8 */
    u8 filler8[4];
    u16 unkC; /* sprite rotation angle, 8.8 in 1/256 turns */
    u8 filler10[0x34 - 0x10];
    u32 model;
};

struct PtObj
{
    u8 filler0[8];
    struct PtRecord *rec;
    u32 *data;
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;
};

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;
};

extern struct ModelCtx *gModelCtx;
extern s16 gSineTable[];
extern s16 gCosineTable[];

u32 block_flag_byte(struct PtRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);


/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped over a quarter turn (64/256) around the x axis, with the sprites drawn at double size; state
   is 2 words in obj->data (scale, scale speed), followed by each piece's offset (x, y, z). */
void obj_tilt_zoom8_6128(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 scale;
    s32 scaleSpeed;
    s32 *state;
    struct PtRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 t;
    s32 pos;

    state = (s32 *)obj->data;
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
        srcY = *state++;
        pt.y = srcY;
        srcZ = *state++;
        pt.z = srcZ;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
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
    scaleSpeed = (scaleSpeed * 8 - scaleSpeed) / 8;
    rec->unk4 = 0x200;
    rec->unk6 = 0x200;
    state = (s32 *)obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped over a quarter turn (64/256) around the x axis and then turned an eighth (32/256) in the
   screen plane, with the sprites drawn at double size; state is 2 words in obj->data (scale, scale
   speed), followed by each piece's offset (x, y, z). */
void obj_turn_zoom4_6310(struct PtObj *obj)
{
    s16 *cosTable;
    s16 *sinTable;
    s16 *cosTip;
    s16 *sinTip;
    s16 *cosTurn;
    s16 *sinTurn;
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 scale;
    s32 scaleSpeed;
    u32 *state;
    struct PtRecord *rec;
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
        cosTip = cosTable + 64;
        t = t * (*cosTip / 0x40);
        sinTable = gSineTable;
        sinTip = sinTable + 64;
        ry = (t - (*sinTip / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (*cosTip / 0x40)) + ((*sinTip / 0x40) * sy)) / 0x100;
        /* turn an eighth in the screen plane */
        t = sx;
        cosTurn = cosTable + 32;
        t = t * (*cosTurn / 0x40);
        sinTurn = sinTable + 32;
        rx = (t - (*sinTurn / 0x40) * ry) / 0x100;
        pt.x = rx;
        pt.y = ((ry * (*cosTurn / 0x40)) + ((*sinTurn / 0x40) * sx)) / 0x100;

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

/* Effect update: draws 8 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped over a quarter turn (64/256) around the x axis, with the sprites drawn at double size; state
   is 2 words in obj->data (scale, scale speed), followed by each piece's offset (x, y, z). */
void obj_tilt_zoom8_6560(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 scale;
    s32 scaleSpeed;
    s32 *state;
    struct PtRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 ry;
    s32 t;
    s32 pos;

    state = (s32 *)obj->data;
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
        srcY = *state++;
        pt.y = srcY;
        srcZ = *state++;
        pt.z = srcZ;
        sx = (srcX * scale) / 0x100;
        pt.x = sx;
        sy = (srcY * scale) / 0x100;
        pt.y = sy;
        sz = (srcZ * scale) / 0x100;
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
    state = (s32 *)obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* Effect update: draws 4 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped a quarter turn (64/256) around the x axis and then an eighth turn (32/256) around the
   z axis, with sprites at double size; state is 2 words in obj->data (scale, scale speed). */
void obj_turn_zoom4_6748(struct PtObj *obj)
{
    s16 *cosTable;
    s16 *sinTable;
    s16 *cosp;
    s16 *sinp;
    s16 *cosp2;
    s16 *sinp2;
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 scale;
    s32 scaleSpeed;
    u32 *state;
    struct PtRecord *rec;
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

/* Effect update: draws one piece at its offset (x, y, z) plus a hop offset (x mirrored when flags
   bit 7 is set), then moves the hop: x by its speed, y by a speed that drops by 0x18 a frame and,
   once below -0x100, starts again at 0x100 with the x speed turned round, so the piece hops to and
   fro; y also drifts by 0x80 a frame. State is 4 words in obj->data (x offset, x speed, y offset,
   y speed; 1/256 px), followed by the piece's offset. */
void obj_bounce_reverse_point(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *state;
    s32 hopX;
    s32 speedX;
    s32 hopY;
    s32 speedY;
    u32 attr;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 piece;
    s32 srcX;
    s32 srcY;
    s32 pos;
    s32 t;

    state = (s32 *)obj->data;
    hopX = *state++;
    speedX = *state++;
    hopY = *state++;
    speedY = *state++;
    pt.model = obj->rec->model;
    attr = block_flag_byte(obj->rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        srcY = *state++;
        pt.y = srcY;
        pt.z = *state++;
        pt.x = srcX + hopX;
        pt.y = srcY + hopY;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = pt.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = pt.x;
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
    hopX += speedX;
    speedY -= 0x18;
    if (speedY < -0x100) {
        speedY = 0x100;
        speedX = -speedX;
    }
    hopY += speedY;
    hopY += 0x80;
    state = (s32 *)obj->data;
    *state++ = hopX;
    *state++ = speedX;
    *state++ = hopY;
    *state = speedY;
}


/* Effect update: draws one piece at its offset (x, y, z) plus a swing offset (x mirrored when flags
   bit 7 is set), then moves the swing: its x speed is pulled back by a 64th of the x offset each
   frame, like a spring, and its y speed by a 256th of the x offset, so the piece sways to and fro.
   State is 4 words in obj->data (x offset, y offset, x speed, y speed; 1/256 px), followed by the
   piece's offset. */
void obj_swing_point(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *state;
    s32 swingX;
    s32 swingY;
    s32 speedX;
    s32 speedY;
    u32 attr;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 piece;
    s32 srcX;
    s32 srcY;
    s32 pos;
    s32 t;

    state = (s32 *)obj->data;
    swingX = *state++;
    swingY = *state++;
    speedX = *state++;
    speedY = *state++;
    pt.model = obj->rec->model;
    attr = block_flag_byte(obj->rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, attr);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        srcY = *state++;
        pt.y = srcY;
        pt.z = *state++;
        pt.x = srcX + swingX;
        pt.y = srcY + swingY;
        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = pt.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = pt.x;
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
    /* speedX -= swingX / 64 */
    t = swingX;
    if (t < 0) {
        t += 0x3F;
    }
    t >>= 6;
    speedX -= t;
    swingX += speedX;
    /* speedY -= swingX / 256 */
    t = swingX;
    if (t < 0) {
        t += 0xFF;
    }
    t >>= 8;
    speedY -= t;
    swingY += speedY;
    state = (s32 *)obj->data;
    *state++ = swingX;
    *state++ = swingY;
    *state++ = speedX;
    *state = speedY;
}

ASM_FUNC("asm/code_08186128/obj_spin_hop_rot90_model5.s", void obj_spin_hop_rot90_model5(void))

ASM_FUNC("asm/code_08186128/obj_spin_hop_rot45_model5.s", void obj_spin_hop_rot45_model5(void))

ASM_FUNC("asm/code_08186128/obj_spin_swap_model5.s", void obj_spin_swap_model5(void))

ASM_FUNC("asm/code_08186128/obj_spin_tilt67_model5.s", void obj_spin_tilt67_model5(void))

/* Effect update: one piece that flies off, falls (gravity 0x40 per frame) and bounces while it spins
   16/256 of a turn per frame (the other way when mirrored by flags bit 7). On each landing its x speed
   halves and it bounces back up at bounceSpeed pixels per frame, which loses 1 per bounce until 0.
   State in obj->data: xOff, xVel, yOff, yVel, bounceSpeed (negative = up), angle, then the offset (x, y, z). */
void obj_bounce_turn_point(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } pt;
    s32 *state;
    s32 xOff;
    s32 xVel;
    s32 angle;
    struct PtRecord *rec;
    u32 modelSaved;
    s32 yOff;
    s32 yVel;
    s32 bounceSpeed;
    u32 attr;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 piece;
    s32 srcX;
    s32 srcY;
    s32 movedX;
    s32 base;
    s32 whole;

    state = (s32 *)obj->data;
    xOff = *state++;
    xVel = *state++;
    yOff = *state++;
    yVel = *state++;
    bounceSpeed = *state++;
    angle = *state++;
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
        srcY = *state++;
        pt.y = srcY;
        pt.z = *state++;
        movedX = srcX + xOff;
        pt.x = movedX;
        pt.y = srcY + yOff;
        /* 8.8 offsets to whole pixels around the object; bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            whole = movedX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = movedX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base + whole;
        }
        base = obj->posY;
        whole = pt.y;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.y = base + whole;
        base = obj->posZ;
        whole = pt.z;
        if (whole < 0) {
            whole += 0xFF;
        }
        whole >>= 8;
        pt.z = base + whole;
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
    xOff += xVel;
    yOff += yVel;
    yVel += 0x40;
    if (yOff > 0) {
        /* landed: bounce */
        xVel = xVel / 2;
        yOff = 0;
        bounceSpeed++;
        if (bounceSpeed > 0) {
            bounceSpeed = 0;
        }
        yVel = bounceSpeed << 8;
    }
    angle += 0x10;
    if (obj->flags & 0x80) {
        rec->unkC = -angle << 8;
    } else {
        rec->unkC = angle << 8;
    }
    state = (s32 *)obj->data;
    *state++ = xOff;
    *state++ = xVel;
    *state++ = yOff;
    *state++ = yVel;
    *state++ = bounceSpeed;
    *state = angle;
}
