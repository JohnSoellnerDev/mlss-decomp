#include "global.h"

struct ObjRecord
{
    u8 filler0[4];
    u16 width;
    u16 height;
    u8 filler8[4];
    u16 angle;
    u8 filler10[0x34 - 0x10];
    u32 model;
};

struct PointObj
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

/* PointObj.flags: the object faces left, so x offsets and angles are mirrored. */
#define POINT_OBJ_MIRRORED 0x80

struct PointLocal
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

// One point that falls and spins backwards: its x/y offset (8.8) grows by its speed every frame,
// gravity adds 0x20 to the y speed, and the sprite turns -6/256 of a turn per frame (the other way
// when mirrored by flags bit 7). State in obj->data: offX, velX, offY, velY, angle, then the
// point's x, y, z.
void obj_fall_spin_point_a(struct PointObj *obj)
{
    struct PointLocal pt;
    s32 velX;
    s32 velY;
    struct ObjRecord *rec;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 angle;
    u32 rows;
    u32 *state;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += offY;
        /* 8.8 offsets to whole pixels around the object; bit 7: mirrored in x */
        if (obj->flags & 0x80) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
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
    velY += 0x20;
    angle += -6;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state++ = velY;
    *state = angle;
}

// One point that falls and spins forwards: like obj_fall_spin_point_a, its x/y offset (8.8) grows
// by its speed every frame and gravity adds 0x20 to the y speed, but the sprite turns +6/256 of a
// turn per frame (the other way when mirrored). State in obj->data: offX, velX, offY, velY, angle,
// then the point's x, y, z.
void obj_fall_spin_point_b(struct PointObj *obj)
{
    struct PointLocal pt;
    s32 velX;
    s32 velY;
    struct ObjRecord *rec;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 angle;
    u32 rows;
    u32 *state;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += offY;
        /* 8.8 offsets to whole pixels around the object, mirrored in x when it faces left */
        if (obj->flags & POINT_OBJ_MIRRORED) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
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
    velY += 0x20;
    angle += 6;
    rec->angle = ((obj->flags & POINT_OBJ_MIRRORED) ? -angle : angle) << 8;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state++ = velY;
    *state = angle;
}

// One point that slides along y at a constant speed: its y offset (8.8) grows by the speed every frame,
// the x offset is mirrored when the object faces left, and the sprite is sized 0.75 x 1.5 (0xC0 x 0x180).
// State in obj->data: offY, speedY, then the point's x, y, z.
void obj_slide_speed_point(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 speedY;
    u32 offY;
    u32 *state;
    struct ObjRecord *rec;
    u32 rowCount;
    u32 modelSaved;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 srcX;
    s32 pos;
    s32 t;

    state = obj->data;
    offY = *state++;
    speedY = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += offY;
        if (obj->flags & POINT_OBJ_MIRRORED) {
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
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    offY += speedY;
    rec->width = 0xC0;
    rec->height = 0x180;
    state = obj->data;
    *state++ = offY;
    *state = speedY;
}

// Single point whose height (8.8 fixed, negative is up) changes by a constant speed each frame,
// drawn at a fixed size of 0x60 x 0xC0.
void obj_drift_y_60c0_7d08(struct PointObj *obj)
{
    struct PointLocal l;
    u32 speed;
    u32 height;
    u32 *words;
    struct ObjRecord *rec;
    u32 pieceCount;
    u32 cursorSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *cursor;
    s32 srcX;
    s32 tmp;
    s32 offs;

    words = obj->data;
    height = *words++;
    speed = *words++;
    rec = obj->rec;
    l.model = rec->model;
    pieceCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(l.model, pieceCount);
    py = &l.y;
    pz = &l.z;
    cursor = &l.model;
    for (i = 0; i < 1; i++) {
        srcX = *words++;
        l.x = srcX;
        l.y = *words++;
        l.z = *words++;
        l.y += height;
        if (obj->flags & 0x80) {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp - offs;
        } else {
            tmp = obj->posX;
            offs = srcX;
            if (offs < 0) {
                offs += 0xFF;
            }
            offs >>= 8;
            l.x = tmp + offs;
        }
        tmp = obj->posY;
        offs = l.y;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.y = tmp + offs;
        tmp = obj->posZ;
        offs = l.z;
        if (offs < 0) {
            offs += 0xFF;
        }
        offs >>= 8;
        l.z = tmp + offs;
        offset_words_camera(&l.x, py, pz, 0);
        oam_place_points(cursor, pieceCount, l.x, l.y, l.z << 14, 0, 0);
        if (!cursorSaved) {
            l.model = gModelCtx->model;
            cursorSaved = 1;
        }
    }
    if (cursorSaved) {
        gModelCtx->model = l.model;
    }
    height += speed;
    rec->width = 0x60;
    rec->height = 0xC0;
    words = obj->data;
    *words++ = height;
    *words = speed;
}

// Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when flag
// 0x80 is set) raised by a drift that grows by a constant speed each frame; the sprite is sized 0x30 x 0x60.
// State in obj->data: the drift, its speed, then the piece's x, y, z.
void obj_drift_y_3060_7e24(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 driftSpeed;
    u32 drift;
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
    drift = *state++;
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
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += drift;
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
    drift += driftSpeed;
    rec->width = 0x30;
    rec->height = 0x60;
    state = obj->data;
    *state++ = drift;
    *state = driftSpeed;
}

/* Effect update: draws one sprite piece at the object's position plus its 8.8 offset (x mirrored when
   flag 0x80 is set), with y moved by a drift that grows by a constant speed each frame; the sprite's
   scale is left alone. obj->data holds the drift, its speed, then the piece's offset (x, y, z). */
void obj_drift_y_keep_7f40(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 speed;
    u32 drift;
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
    drift = *state++;
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
        pt.y += drift;
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
    drift += speed;
    state = obj->data;
    *state++ = drift;
    *state = speed;
}

// One point hops and spins: it falls (gravity 0x40), and each time it lands its sideways speed
// halves and it bounces up with the next bounce speed (bounce * 0x100, rising to 0); it turns by 4 per frame.
void obj_bounce_spin_point_8050(struct PointObj *obj)
{
    struct PointLocal l;
    s32 xOff;
    s32 velX;
    s32 angle;
    struct ObjRecord *rec;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 i;
    u32 h;
    s32 yOff;
    s32 velY;
    s32 bounce;
    u32 *words;
    s32 x;
    s32 pos;
    s32 t;

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
    velY += 0x40;
    if (yOff > 0) {
        /* landed: bounce */
        velX /= 2;
        yOff = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        velY = bounce << 8;
    }
    angle += 4;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = yOff;
    *words++ = velY;
    *words++ = bounce;
    *words = angle;
}

// One point hops and spins: it bounces off the floor, drifts sideways and turns by 0x10 per frame.
// obj->data holds the state (x offset, side speed, height, fall speed, bounce, angle), then the point.
// Each bounce halves the side speed and kicks it up by bounce * 0x100; bounce counts up to 0.
void obj_bounce_spin16_point_81d0(struct PointObj *obj)
{
    struct PointLocal l;
    s32 xOff;
    s32 sideVel;
    s32 angle;
    struct ObjRecord *rec;
    u32 saved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 count;
    u32 rows;
    s32 height;
    s32 fallVel;
    s32 bounce;
    u32 *words;
    s32 pointX;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    sideVel = *words++;
    height = *words++;
    fallVel = *words++;
    bounce = *words++;
    angle = *words++;
    rec = obj->rec;
    l.model = rec->model;
    rows = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, rows);
    py = &l.y;
    pz = &l.z;
    pmodel = &l.model;
    for (count = 0; count < 1; count++) {
        pointX = *words++;
        l.x = pointX;
        l.y = *words++;
        l.z = *words++;
        pointX += xOff;
        l.x = pointX;
        l.y += height;
        /* to the object's position: point offsets are 24.8 */
        if (obj->flags & POINT_OBJ_MIRRORED) {
            pos = obj->posX;
            t = pointX;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = pointX;
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
        oam_place_points(pmodel, rows, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gModelCtx->model;
            saved = 1;
        }
    }
    if (saved) {
        gModelCtx->model = l.model;
    }
    xOff += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        /* hit the floor */
        sideVel /= 2;
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        fallVel = bounce << 8;
    }
    angle += 0x10;
    rec->angle = ((obj->flags & POINT_OBJ_MIRRORED) ? -angle : angle) << 8;
    words = obj->data;
    *words++ = xOff;
    *words++ = sideVel;
    *words++ = height;
    *words++ = fallVel;
    *words++ = bounce;
    *words = angle;
}

/* Effect update: one point hops along the floor. It drifts sideways (mirrored when flags bit 7 is set),
   falls under gravity 0x40, and on landing loses half its sideways speed and bounces up again with
   rebound << 8 (rebound counts up to 0, so each bounce is lower). It spins by 0x10 per frame.
   State is 6 words in obj->data, then the point's x, y, z offset. */
void obj_bounce_spin16_point_8350(struct PointObj *obj)
{
    struct PointLocal pt;
    s32 xOff;
    s32 sideSpeed;
    s32 angle;
    struct ObjRecord *rec;
    u32 modelSaved;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 piece;
    u32 rowCount;
    s32 height;
    s32 fallSpeed;
    s32 rebound;
    u32 *state;
    s32 srcX;
    s32 pos;
    s32 t;

    state = obj->data;
    xOff = *state++;
    sideSpeed = *state++;
    height = *state++;
    fallSpeed = *state++;
    rebound = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (piece = 0; piece < 1; piece++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += xOff;
        pt.x = srcX;
        pt.y += height;
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
        oam_place_points(pmodel, rowCount, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    xOff += sideSpeed;
    height += fallSpeed;
    fallSpeed += 0x40;
    /* landed: lose half the sideways speed and bounce back up, a little weaker each time */
    if (height > 0) {
        sideSpeed /= 2;
        height = 0;
        rebound++;
        if (rebound > 0) {
            rebound = 0;
        }
        fallSpeed = rebound << 8;
    }
    angle += 0x10;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    state = obj->data;
    *state++ = xOff;
    *state++ = sideSpeed;
    *state++ = height;
    *state++ = fallSpeed;
    *state++ = rebound;
    *state = angle;
}

// One point drifts sideways and up while it spins by 4 per frame and shrinks by 0xC per frame.
NONMATCH("asm/code_08187938/obj_drift_spin_shrink_84d0.s", void obj_drift_spin_shrink_84d0(struct PointObj *obj))
{
    struct PointLocal l;
    s32 velX;
    s32 velY;
    u32 size;
    u32 saved;
    s32 count;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 xOff;
    s32 height;
    s32 angle;
    u32 *words;
    struct ObjRecord *rec;
    u32 h;
    s32 vx;
    s32 pos;
    s32 t;

    words = obj->data;
    xOff = *words++;
    velX = *words++;
    height = *words++;
    velY = *words++;
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
    for (count = 0; count < 1; count++) {
        vx = *words++;
        l.x = vx;
        l.y = *words++;
        l.z = *words++;
        vx += xOff;
        l.x = vx;
        l.y += height;
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
    height += velY;
    angle += 4;
    rec->angle = ((obj->flags & 0x80) ? -angle : angle) << 8;
    size -= 0xc;
    rec->width = size;
    rec->height = *(u16 *)&size;
    words = obj->data;
    *words++ = xOff;
    *words++ = velX;
    *words++ = height;
    *words++ = velY;
    *words++ = angle;
    *words = size;
}
END_NONMATCH

ASM_FUNC("asm/code_08187938/obj_spin_zoom5_863c.s", void obj_spin_zoom5_863c(void))

// Effect update: draws five sprite pieces at the object's position plus their 8.8 offsets, all lifted by a
// shared y offset that rises 4 pixels (0x400) per frame, while the sprite shrinks by 0x20 per frame.
// State in obj->data: scale, y offset, then each piece's x, y, z.
void obj_shrink_y_offset5_88f0(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 cursorSaved;
    u32 scale;
    u32 yOffset;
    u32 *state;
    struct ObjRecord *rec;
    u32 partCount;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    scale = *state++;
    yOffset = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
    for (piece = 0; piece < 5; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += yOffset;
        if (obj->flags & POINT_OBJ_MIRRORED) {
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
    yOffset += -0x400;
    scale -= 0x20;
    rec->width = scale;
    rec->height = scale;
    state = obj->data;
    *state++ = scale;
    *state = yOffset;
}

/*
 * Effect update: draws five sprites at their 8.8 fixed-point offsets from the
 * object (mirrored in X when flag 0x80 is set), all lifted by a rise that grows
 * by 0x400 (4 pixels) per frame, while the record's size shrinks by 0x20 per
 * frame. State in obj->data: size, y offset, then each sprite's offset (x, y, z).
 */
void obj_shrink_y_offset5_8a10(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 modelSaved;
    u32 size;
    u32 yOff;
    u32 *state;
    struct ObjRecord *rec;
    u32 rowCount;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 base;
    s32 ofs;

    state = obj->data;
    size = *state++;
    yOff = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rowCount = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rowCount);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 5; i++) {
        offX = *state++;
        pt.x = offX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += yOff;
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
    yOff += -0x400;
    size -= 0x20;
    rec->width = size;
    rec->height = size;
    state = obj->data;
    *state++ = size;
    *state = yOff;
}

// Effect update: draws five sprite pieces at the object's position plus their 8.8 offsets, all lifted by a
// shared y offset that rises 4 pixels (0x400) per frame, while the sprite shrinks by 0x20 per frame.
// State in obj->data: scale, y offset, then each piece's x, y, z.
void obj_shrink_y_offset5_8b30(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 cursorSaved;
    u32 scale;
    u32 yOffset;
    u32 *state;
    struct ObjRecord *rec;
    u32 partCount;
    s32 piece;
    s32 *py;
    s32 *pz;
    u32 *pcursor;
    s32 offsetX;
    s32 base;
    s32 offset;

    state = obj->data;
    scale = *state++;
    yOffset = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    partCount = block_flag_byte(rec);
    cursorSaved = 0;
    oam_list_rows_b(pt.model, partCount);
    py = &pt.y;
    pz = &pt.z;
    pcursor = &pt.model;
    for (piece = 0; piece < 5; piece++) {
        offsetX = *state++;
        pt.x = offsetX;
        pt.y = *state++;
        pt.z = *state++;
        pt.y += yOffset;
        if (obj->flags & POINT_OBJ_MIRRORED) {
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
    yOffset += -0x400;
    scale -= 0x20;
    rec->width = scale;
    rec->height = scale;
    state = obj->data;
    *state++ = scale;
    *state = yOffset;
}

// One point bounces and drifts sideways. State in obj->data: x offset, side speed, height, fall speed, bounce,
// one word kept as is, then the point's x, y, z. Gravity adds 0x40 to the fall speed; each landing halves the
// side speed and kicks the point up by bounce * 0x100, bounce counting up to 0.
void obj_bounce_x_point_8c50(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 kept;
    u32 rows;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 sideVel;
    s32 height;
    s32 fallVel;
    s32 bounce;
    u32 *state;
    struct ObjRecord *rec;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    sideVel = *state++;
    height = *state++;
    fallVel = *state++;
    bounce = *state++;
    kept = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += height;
        /* 8.8 offsets to whole pixels around the object, mirrored in x when it faces left */
        if (obj->flags & POINT_OBJ_MIRRORED) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
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
        oam_place_points(pmodel, rows, pt.x, pt.y, pt.z << 14, 0, 0);
        if (!modelSaved) {
            pt.model = gModelCtx->model;
            modelSaved = 1;
        }
    }
    if (modelSaved) {
        gModelCtx->model = pt.model;
    }
    offX += sideVel;
    height += fallVel;
    fallVel += 0x40;
    if (height > 0) {
        /* landed */
        sideVel /= 2;
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        fallVel = bounce << 8;
    }
    state = obj->data;
    *state++ = offX;
    *state++ = sideVel;
    *state++ = height;
    *state++ = fallVel;
    *state++ = bounce;
    *state = kept;
}

// One point that bounces while drifting sideways: its x/y offsets (8.8) grow by their speeds,
// gravity adds 0x40 to the fall speed, and on each landing (offY > 0) the side speed halves and
// the point rebounds at bounce << 8, bounce rising towards 0 (no more rebounds). State in
// obj->data: offX, velX, offY, velY, bounce, a kept word, then the point's x, y, z.
void obj_bounce_x_point_8da4(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 kept;
    u32 rows;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 velX;
    s32 offY;
    s32 velY;
    s32 bounce;
    u32 *state;
    struct ObjRecord *rec;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    bounce = *state++;
    kept = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += offY;
        /* 8.8 offsets to whole pixels around the object, mirrored in x when facing left */
        if (obj->flags & POINT_OBJ_MIRRORED) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
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
    velY += 0x40;
    if (offY > 0) {
        /* landed: slow down sideways and rebound a little weaker */
        velX /= 2;
        offY = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        velY = bounce << 8;
    }
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state++ = velY;
    *state++ = bounce;
    *state = kept;
}

// One point that is thrown sideways and falls fast while spinning slowly: its x/y offset (8.8)
// grows by its speed every frame, gravity adds 0x40 to the y speed, and the sprite turns +2/256 of
// a turn per frame (the other way when mirrored). State in obj->data: offX, velX, offY, velY,
// angle, then the point's x, y, z.
void obj_fall_x_spin_point_8ef8(struct PointObj *obj)
{
    struct PointLocal pt;
    s32 velX;
    s32 velY;
    struct ObjRecord *rec;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    s32 offX;
    s32 offY;
    s32 angle;
    u32 rows;
    u32 *state;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    angle = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += offY;
        /* 8.8 offsets to whole pixels around the object, mirrored in x when it faces left */
        if (obj->flags & POINT_OBJ_MIRRORED) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
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
    velY += 0x40;
    angle += 2;
    rec->angle = ((obj->flags & POINT_OBJ_MIRRORED) ? -angle : angle) << 8;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state++ = velY;
    *state = angle;
}

// One point that drifts with a constant speed while its sprite shrinks: its x/y offset (8.8) grows
// by its speed every frame and the record's width and height lose 0x10 per frame. State in
// obj->data: offX, velX, offY, velY, size, then the point's x, y, z.
void obj_drift_shrink_9050(struct PointObj *obj)
{
    struct PointLocal pt;
    u32 velX;
    u32 velY;
    u32 modelSaved;
    s32 i;
    s32 *py;
    s32 *pz;
    u32 *pmodel;
    u32 offX;
    u32 offY;
    u32 size;
    u32 *state;
    struct ObjRecord *rec;
    u32 rows;
    s32 srcX;
    s32 base;
    s32 whole;

    state = obj->data;
    offX = *state++;
    velX = *state++;
    offY = *state++;
    velY = *state++;
    size = *state++;
    rec = obj->rec;
    pt.model = rec->model;
    rows = block_flag_byte(rec);
    modelSaved = 0;
    oam_list_rows_b(pt.model, rows);
    py = &pt.y;
    pz = &pt.z;
    pmodel = &pt.model;
    for (i = 0; i < 1; i++) {
        srcX = *state++;
        pt.x = srcX;
        pt.y = *state++;
        pt.z = *state++;
        srcX += offX;
        pt.x = srcX;
        pt.y += offY;
        /* 8.8 offsets to whole pixels around the object, mirrored in x when it faces left */
        if (obj->flags & POINT_OBJ_MIRRORED) {
            base = obj->posX;
            whole = srcX;
            if (whole < 0) {
                whole += 0xFF;
            }
            whole >>= 8;
            pt.x = base - whole;
        } else {
            base = obj->posX;
            whole = srcX;
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
    size += -0x10;
    rec->width = size;
    rec->height = size;
    state = obj->data;
    *state++ = offX;
    *state++ = velX;
    *state++ = offY;
    *state++ = velY;
    *state = size;
}
