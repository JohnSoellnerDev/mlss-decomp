#include "global.h"

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

// One point being placed: its screen position and the OAM cursor it's drawn at.
struct Point1Local
{
    s32 x;
    s32 y;
    s32 z;
    u32 model;  // OAM row cursor
};

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;  // next free OAM row
};

extern struct ModelCtx *gModelCtx;

u32 block_flag_byte(struct ObjRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);

extern s16 gSineTable[];
extern s16 gCosineTable[];

// Burst effect: draws the sprite at 9 model points, scaled by a growing zoom, tilted
// back by a fixed angle and falling; the sprite shrinks and spins faster each frame.
void obj_accel_tilt9_e4f0(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    s32 zoom;
    s32 shrink;
    s32 shrink_speed;
    s32 fall;
    s32 fall_speed;
    s32 spin;
    struct ObjRecord *sprite;
    u32 row_count;
    u32 cursor_taken;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 vx;
    s32 x;
    s32 y;
    s32 z;
    s32 tilted_y;
    s32 t;
    s32 dx;

    words = obj->data;
    zoom = *words++;
    shrink = *words++;
    shrink_speed = *words++;
    fall = *words++;
    fall_speed = *words++;
    spin = *words++;
    sprite = obj->rec;
    pt.model = sprite->model;
    row_count = block_flag_byte(sprite);
    cursor_taken = 0;
    oam_list_rows_b(pt.model, row_count);
    py = &pt.y;
    pz = &pt.z;
    cursor = &pt.model;
    for (i = 0; i < 9; i++) {
        vx = *words++;
        pt.x = vx;
        pt.y = *words++;
        pt.z = *words++;

        // Scale the point by the zoom (8.8 fixed point).
        x = (vx * zoom) / 0x100;
        pt.x = x;
        y = (pt.y * zoom) / 0x100;
        pt.y = y;
        z = (pt.z * zoom) / 0x100;
        pt.z = z;

        // Tilt it around the X axis by angle 12, then let it fall.
        t = y;
        t = t * (gCosineTable[12] / 0x40);
        tilted_y = (t - (gSineTable[12] / 0x40) * z) / 0x100;
        pt.y = tilted_y;
        pt.z = ((z * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y)) / 0x100;
        pt.y = tilted_y + fall;

        // Place it around the object, mirrored if the object faces left.
        if (obj->flags & 0x80) {
            t = obj->posX;
            dx = x;
            if (dx < 0) {
                dx += 0xFF;
            }
            dx >>= 8;
            pt.x = t - dx;
        } else {
            t = obj->posX;
            dx = x;
            if (dx < 0) {
                dx += 0xFF;
            }
            dx >>= 8;
            pt.x = t + dx;
        }
        pt.y = obj->posY + pt.y / 0x100;
        pt.z = obj->posZ + pt.z / 0x100;
        offset_words_camera(&pt.x, py, pz, 0);
        oam_place_points(cursor, row_count, pt.x, pt.y, pt.z << 14, 0, 0);

        // The first copy uses the sprite's own OAM rows, the rest take free ones.
        if (!cursor_taken) {
            pt.model = gModelCtx->model;
            cursor_taken = 1;
        }
    }
    if (cursor_taken) {
        gModelCtx->model = pt.model;
    }

    zoom += 0x400;
    fall += fall_speed;
    fall_speed += 0x60;
    shrink += shrink_speed;
    shrink_speed += 1;
    sprite->unk4 = 0x140 - shrink;
    sprite->unk6 = 0x140 - shrink;
    spin += 6;
    sprite->unkC = (obj->flags & 0x80) ? (-spin << 8) : (spin << 8);

    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrink_speed;
    *words++ = fall;
    *words++ = fall_speed;
    *words = spin;
}

NONMATCH("asm/code_0817E4F0/obj_angle_zoom4_e71c.s", void obj_angle_zoom4_e71c(struct Point1Obj *obj))
{
    s16 *cs;
    s16 *sn;
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 angle;
    struct ObjRecord *rec;
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
    s32 y2;
    s32 idx;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
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
        idx = angle & 0xFF;
        cs = &gCosineTable[idx];
        t = t * (*cs / 0x40);
        sn = &gSineTable[idx];
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        pos = y1;
        pos = pos * (gCosineTable[12] / 0x40);
        y2 = (pos - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2 / 0x100;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2 / 0x100;
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
    *words++ = vel;
    *words = angle;
}
END_NONMATCH


NONMATCH("asm/code_0817E4F0/obj_angle_zoom4_e998.s", void obj_angle_zoom4_e998(struct Point1Obj *obj))
{
    s16 *cs;
    s16 *sn;
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 angle;
    struct ObjRecord *rec;
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
    s32 y2;
    s32 idx;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
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
        idx = angle & 0xFF;
        cs = &gCosineTable[idx];
        t = t * (*cs / 0x40);
        sn = &gSineTable[idx];
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        pos = y1;
        pos = pos * (gCosineTable[12] / 0x40);
        y2 = (pos - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2 / 0x100;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2 / 0x100;
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
    *words++ = vel;
    *words = angle;
}
END_NONMATCH

/*
 * Draws a growing burst of 8 sprites. obj->data holds the burst's scale, scale velocity and
 * angle, then 8 x/y/z points. Each point is scaled by scale/256, rotated about the vertical
 * axis by the angle, tilted by a fixed angle (12/256 of a turn), mirrored when the object is
 * flipped, offset by the object's position and placed on screen through the camera.
 * Afterwards the burst grows by its velocity (which decays by 7/8); the angle stays the same.
 */
void obj_angle_zoom8_ec14(struct Point1Obj *obj)
{
    s16 *cosTable;
    s16 *sinTable;
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
    s32 sx;
    s32 sz;
    s32 rx;
    s32 rz;
    s32 offs;
    s32 tmp;
    s32 sy;
    s32 ry;

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
        ry = (tmp - (sinTable[12] / 0x40) * rz) / 0x100;
        pt.y = ry;
        pt.z = ((rz * (cosTable[12] / 0x40)) + ((sinTable[12] / 0x40) * sy)) / 0x100;

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
    rec->unk4 = 0x100;
    rec->unk6 = 0x100;
    data = obj->data;
    *data++ = scale;
    *data++ = scaleVel;
    *data = angle;
}

/* Effect update: draws 12 pieces that spread out (scale grows by a speed that slows to 9/10 each frame),
   turned 16/256 around the vertical axis and tipped 12/256 around the x axis, with sprites at
   1.25x size; state is 2 words in obj->data (scale, scale speed). */
void obj_turn_zoom12_ee84(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 scale;
    s32 scaleSpeed;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piecesLeft;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 srcY;
    s32 srcZ;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 rz;
    s32 rx;
    s32 pos;
    s32 t;

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
    piecesLeft = 11;
    /* each piece: its offset (x, y, z) follows the state words */
    for (;;) {
        srcX = *state++;
        pt.x = srcX;
        srcY = *state++;
        pt.y = srcY;
        srcZ = *state++;
        pt.z = srcZ;
        sx = srcX * scale / 0x100;
        pt.x = sx;
        sy = srcY * scale / 0x100;
        pt.y = sy;
        sz = srcZ * scale / 0x100;
        pt.z = sz;
        /* turn around the vertical axis */
        pos = sz;
        pos = pos * (gCosineTable[16] / 0x40);
        rz = (pos - (gSineTable[16] / 0x40) * sx) / 0x100;
        pt.z = rz;
        rx = (sx * (gCosineTable[16] / 0x40) + (gSineTable[16] / 0x40) * sz) / 0x100;
        pt.x = rx;
        /* tip around the x axis */
        pos = sy;
        pos = pos * (gCosineTable[12] / 0x40);
        pt.y = (pos - (gSineTable[12] / 0x40) * rz) / 0x100;
        pt.z = ((rz * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * sy)) / 0x100;

        /* bit 7: mirrored */
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = rx;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            pt.x = pos - t;
        } else {
            pos = obj->posX;
            t = rx;
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
        piecesLeft--;
        if (piecesLeft < 0) {
            break;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    scale += scaleSpeed;
    scaleSpeed = (scaleSpeed * 9) / 10;
    rec->unk4 = 0x140;
    rec->unk6 = 0x140;
    state = obj->data;
    *state++ = scale;
    *state = scaleSpeed;
}

/* Effect update: draws 12 pieces that spread out (scale grows by a speed that slows to 7/8 each frame),
   tipped a sixteenth of a turn (16/256) around the x axis, with the sprites drawn at double size; state
   is 2 words in obj->data (scale, scale speed), followed by each piece's offset (x, y, z). */
void obj_rotx_zoom12_f0c0(struct Point1Obj *obj)
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
    for (piece = 0; piece < 12; piece++) {
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
        /* tip a sixteenth of a turn around the x axis */
        t = sy;
        t = t * (gCosineTable[16] / 0x40);
        ry = (t - (gSineTable[16] / 0x40) * sz) / 0x100;
        pt.y = ry;
        pt.z = ((sz * (gCosineTable[16] / 0x40)) + ((gSineTable[16] / 0x40) * sy)) / 0x100;

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
 * Effect update: draws one sprite at its 8.8 fixed-point offset from the object
 * plus a drift (x, y) that moves by a constant speed every frame, at a fixed
 * scale of 0xEC. State in obj->data: drift x, drift y, speed x, speed y, then
 * the sprite's offset (x, y, z).
 */
void obj_drift_point_f2a4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 driftX;
    s32 driftY;
    s32 speedX;
    s32 speedY;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 offZ;
    s32 driftedX;
    s32 driftedY;
    s32 base;
    s32 baseZ;
    s32 ofs;

    state = obj->data;
    driftX = *state++;
    driftY = *state++;
    speedX = *state++;
    speedY = *state++;
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
        offY = *state++;
        pt.y = offY;
        offZ = *state++;
        pt.z = offZ;
        driftedX = offX + driftX;
        pt.x = driftedX;
        driftedY = offY + driftY;
        pt.y = driftedY;
        base = obj->posX;
        ofs = driftedX;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.x = base + ofs;
        base = obj->posY;
        ofs = driftedY;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        baseZ = obj->posZ;
        ofs = offZ;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = baseZ + ofs;
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
    driftX += speedX;
    driftY += speedY;
    rec->unk4 = 0xEC;
    rec->unk6 = 0xEC;
    state = obj->data;
    *state++ = driftX;
    *state++ = driftY;
    *state++ = speedX;
    *state = speedY;
}

/*
 * Effect update (same as obj_drift_point_f2a4): draws one sprite at its 8.8
 * fixed-point offset from the object plus a drift (x, y) that moves by a
 * constant speed every frame, at a fixed scale of 0xEC. State in obj->data:
 * drift x, drift y, speed x, speed y, then the sprite's offset (x, y, z).
 */
void obj_drift_point_f3c4(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    s32 driftX;
    s32 driftY;
    s32 speedX;
    s32 speedY;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 offZ;
    s32 driftedX;
    s32 driftedY;
    s32 base;
    s32 baseZ;
    s32 ofs;

    state = obj->data;
    driftX = *state++;
    driftY = *state++;
    speedX = *state++;
    speedY = *state++;
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
        offY = *state++;
        pt.y = offY;
        offZ = *state++;
        pt.z = offZ;
        driftedX = offX + driftX;
        pt.x = driftedX;
        driftedY = offY + driftY;
        pt.y = driftedY;
        base = obj->posX;
        ofs = driftedX;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.x = base + ofs;
        base = obj->posY;
        ofs = driftedY;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.y = base + ofs;
        baseZ = obj->posZ;
        ofs = offZ;
        if (ofs < 0) {
            ofs += 0xFF;
        }
        ofs >>= 8;
        pt.z = baseZ + ofs;
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
    driftX += speedX;
    driftY += speedY;
    rec->unk4 = 0xEC;
    rec->unk6 = 0xEC;
    state = obj->data;
    *state++ = driftX;
    *state++ = driftY;
    *state++ = speedX;
    *state = speedY;
}

// Five-point object that grows and falls: each point is zoomed (8.8 fixed point), tilted by the
// fixed table angle 12, shifted by the x offset and the fall height and drawn at the object
// (mirrored when flags bit 7 is set). Every frame the zoom grows by 4.0, gravity speeds up the
// fall, and the record's size (0xA0 - shrink) shrinks ever faster.
void obj_accel_tilt5_f4e4(struct Point1Obj *obj)
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
    s32 t;
    s32 pos;

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
    pointsLeft = 4;
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
        tiltZ = (zz * (gCosineTable[12] / 0x40) + (gSineTable[12] / 0x40) * zy) / 0x100;
        pt.z = tiltZ;
        // Shift it and let it fall.
        shiftedX = offsetX + zx;
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
    zoom += 0x400;
    fallY += fallVel;
    fallVel += 0x60; // gravity
    shrink += shrinkSpeed;
    shrinkSpeed++;
    rec->unk4 = 0xA0 - shrink;
    rec->unk6 = 0xA0 - shrink;
    words = obj->data;
    *words++ = zoom;
    *words++ = shrink;
    *words++ = shrinkSpeed;
    *words++ = offsetX;
    *words++ = fallY;
    *words = fallVel;
}

NONMATCH("asm/code_0817E4F0/obj_zoom_tilt4_f6dc.s", void obj_zoom_tilt4_f6dc(struct Point1Obj *obj))
{
    s16 *cs;
    s16 *sn;
    struct Point1Local l;
    s32 zoom;
    s32 vel;
    s32 angle;
    struct ObjRecord *rec;
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
    s32 y2;
    s32 idx;

    words = obj->data;
    zoom = *words++;
    vel = *words++;
    angle = *words++;
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
        idx = angle & 0xFF;
        cs = &gCosineTable[idx];
        t = t * (*cs / 0x40);
        sn = &gSineTable[idx];
        z2 = (t - (*sn / 0x40) * x1) / 0x100;
        l.z = z2;
        x2 = (x1 * (*cs / 0x40) + (*sn / 0x40) * z1) / 0x100;
        l.x = x2;
        pos = y1;
        pos = pos * (gCosineTable[12] / 0x40);
        y2 = (pos - (gSineTable[12] / 0x40) * z2) / 0x100;
        l.y = y2;
        l.z = ((z2 * (gCosineTable[12] / 0x40)) + ((gSineTable[12] / 0x40) * y1)) / 0x100;
        l.y = y2 - 0x800;

        if (obj->flags & 0x80) {
            t = obj->posX;
            pos = x2 / 0x100;
            l.x = t - pos;
        } else {
            t = obj->posX;
            pos = x2 / 0x100;
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
    *words++ = vel;
    *words = angle;
}
END_NONMATCH

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset, shifted
// along x by a glide offset (the whole x offset is mirrored when flag 0x80 is set). The glide
// offset grows by a constant speed each frame. State in obj->data: the glide offset, its speed,
// then the piece's x, y, z.
void obj_glide_x_f958(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 glide;
    u32 speed;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 glideX;
    s32 base;
    s32 ofs;

    state = obj->data;
    glide = *state++;
    speed = *state++;
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
        glideX = offX + glide;
        pt.x = glideX;
        if (obj->flags & 0x80) {
            base = obj->posX;
            ofs = glideX;
            if (ofs < 0) {
                ofs += 0xFF;
            }
            ofs >>= 8;
            pt.x = base - ofs;
        } else {
            base = obj->posX;
            ofs = glideX;
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
    glide += speed;
    state = obj->data;
    *state++ = glide;
    *state = speed;
}

// One-point effect that shrinks: draws its sprite at the model's one point (8.8 fixed point
// offset from the object, mirrored when it faces left), and every frame the scale kept in
// obj->data drops by 0x20 (1/8) and becomes the sprite's X and Y scale.
void obj_sink_oam_fa68(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *words;
    u32 scale;
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

    // State: the scale word in obj->data, followed by the point.
    words = obj->data;
    scale = *words++;
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
        if (obj->flags & POINT1_OBJ_MIRRORED) {
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
    scale -= 0x20;
    rec->unk4 = scale;
    rec->unk6 = scale;
    words = obj->data;
    *words = scale;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored
// when the object is mirrored), then grows the sprite's scale by 4/256 each frame. State in
// obj->data: the scale, then the piece's x, y, z.
void obj_rise_oam_fb70(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
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
    scale += 4;
    rec->unk4 = scale;
    rec->unk6 = scale;
    state = obj->data;
    *state = scale;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
// POINT1_OBJ_MIRRORED is set), pushed down by a fall distance that grows with a speed accelerating by
// half a pixel (0x80) per frame. State in obj->data: the fall distance, its speed, then the piece's x, y, z.
void obj_fall_accel_fc78(struct Point1Obj *obj)
{
    struct Point1Local pt;
    u32 *state;
    u32 fall;
    u32 fallSpeed;
    struct ObjRecord *rec;
    u32 attr;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offsetX;
    s32 pieceY;
    s32 offsetY;
    s32 base;
    s32 offset;

    state = obj->data;
    fall = *state++;
    fallSpeed = *state++;
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
        pieceY = *state++;
        pt.y = pieceY;
        pt.z = *state++;
        offsetY = pieceY + fall;
        pt.y = offsetY;
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
    fall += fallSpeed;
    fallSpeed += 0x80;
    state = obj->data;
    *state++ = fall;
    *state = fallSpeed;
}
