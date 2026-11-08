#include "global.h"

extern const u8 gOamSizeTable[];
extern s16 gSineTable[];
extern s16 gCosineTable[];

struct RecObj {
    s16 x;                          // 0x00
    s16 y;                          // 0x02
    s16 scaleX;                     // 0x04
    s16 scaleY;                     // 0x06
    s16 pa;                         // 0x08
    s16 pd;                         // 0x0A
    s16 angleS;                     // 0x0C (also read as u16)
    u16 group;                      // 0x0E
    u8 ctrlLo;                      // 0x10 (ctrl is the halfword 0x10-0x11)
    u8 flip;                        // 0x11
    u8 filler12[2];
    u16 palMask;                    // 0x14
    u8 filler16[9];
    u8 priority;                    // 0x1F
    u8 filler20[9];
    u8 flags29;                     // 0x29
    u8 filler2A[4];
    u16 tileBase;                   // 0x2E
};

struct OamNode {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 attr3;
    u32 key;
    u32 unkC;
    struct OamNode *next;
};

struct OamPart {
    s8 dx;
    s8 dy;
    u8 tile;
    u8 flags;
};

static inline s32 half_of(s32 v)
{
    return (v + ((u32)v >> 31)) >> 1;
}

static inline s32 round_asr8(s32 v)
{
    if (v < 0) {
        v += 0xFF;
    }
    return v >> 8;
}

static inline s32 round_asr14(s32 v)
{
    if (v < 0) {
        v += 0x3FFF;
    }
    return v >> 14;
}

struct OamPart6 {
    s8 y;
    u8 flags1;
    u8 x;
    u8 byte3;
    u8 tile;
    u8 byte5;
};

NONMATCH("asm/code_0801EED0/draw_parts_rec6.s", u32 draw_parts_rec6(struct OamNode *nodeIn, struct RecObj *obj,
                    u32 **streamIn, s32 countIn, u8 *tilePtr, s8 *slotTable, u32 slotNext, u8 *ctr))
{
    u16 pal[16];
    u8 prioTab[4];
    struct OamNode *node;
    struct OamPart6 *part;
    s32 mode;
    s32 prio;
    s32 index;
    s32 sub;
    s32 affine;
    s32 slotKey;
    s32 flags1;
    s32 w, h, x, y, flip;

    node = nodeIn;

    {
        s32 i = 0;
        u16 *slot = pal;
        s32 mask = obj->palMask;
        if (mask != 0) {
            do {
                if (mask & 1) {
                    *slot = i << 12;
                    slot++;
                }
                i++;
                mask >>= 1;
            } while (mask != 0);
        }
    }

    {
        s32 angle = obj->angleS;
        if ((angle != 0 && (obj->flags29 & 0x80) == 0)
            || obj->scaleX > 0x100 || obj->scaleY > 0x100 || obj->pa > 0x100 || obj->pd > 0x100) {
            mode = 3;
        } else {
            mode = 1;
            if (angle == 0 && *(u32 *)&obj->scaleX == 0x01000100) {
                mode = (*(u32 *)&obj->pa ^ *(u32 *)&obj->scaleX) != 0;
            }
        }
    }

    prioTab[0] = obj->priority & 3;
    prioTab[1] = (obj->priority >> 2) & 3;
    prioTab[2] = (obj->priority >> 4) & 3;
    prioTab[3] = (obj->priority >> 6) & 3;

    part = (struct OamPart6 *)*streamIn;
    index = 0;

    if (index < countIn) {
        do {
            s32 byte3;
            flags1 = part->flags1;
            sub = flags1 & 3;
            affine = 3;
            if (mode != 3) {
                affine = mode;
                if (sub != 0) {
                    affine = sub;
                }
            }

            byte3 = part->byte3;
            {
                const u8 *sz = gOamSizeTable + (((flags1 >> 6) << 3) + ((byte3 >> 6) << 1));
                w = sz[0];
                h = sz[1];
            }
            x = part->x;
            if ((byte3 & 1) != 0) {
                x |= -0x100;
            }
            y = part->y;
            if (sub == 3) {
                x = x + half_of(w);
                y = y + half_of(h);
            }

            {
                s32 sx = obj->scaleX;
                if (sx != 0x100) {
                    s32 w8 = w << 8;
                    s32 xs = x * sx;
                    s32 wsx = w8 * sx;
                    wsx = round_asr8(wsx);
                    x = xs - half_of(w8 - wsx);
                    x = round_asr8(x);
                    w = round_asr8(w8);
                }
            }

            flip = obj->flip;
            if ((flip & 0x40) != 0) {
                x = -x - w;
            }

            {
                s32 sy = obj->scaleY;
                if (sy != 0x100) {
                    s32 h8 = h << 8;
                    s32 ys = y * sy;
                    s32 hsy = h8 * sy;
                    hsy = round_asr8(hsy);
                    y = ys - half_of(h8 - hsy);
                    y = round_asr8(y);
                    h = round_asr8(h8);
                }
            }
            if ((flip & 0x80) != 0) {
                y = -y - h;
            }

            if ((*(u16 *)&obj->angleS) != 0) {
                s32 hw = half_of(w);
                s32 hh = half_of(h);
                s32 cx = x + hw;
                s32 cy = y + hh;
                s32 ang = ((*(u16 *)&obj->angleS) >> 8) & 0xFF;
                s32 cs = gCosineTable[ang];
                s32 sn = gSineTable[ang];
                s32 rx = cs * cx + sn * cy;
                s32 ry = -cx * sn + cy * cs;
                x = round_asr14(rx) - hw;
                y = round_asr14(ry) - hh;
            }

            if (*(u32 *)&obj->pa != 0x01000100) {
                s32 pa = obj->pa;
                s32 pd = obj->pd;
                s32 xp = pa * x;
                s32 wpa = pa * w;
                s32 yp = pd * y;
                s32 hpd = pd * h;
                xp = round_asr8(xp);
                wpa = round_asr8(wpa);
                x = xp - half_of(w - wpa);
                yp = round_asr8(yp);
                hpd = round_asr8(hpd);
                y = yp - half_of(h - hpd);
            }

            if (affine == 3) {
                x = x - half_of(w);
                y = y - half_of(h);
                w = w << 1;
                h = h << 1;
            }

            x = x + obj->x;
            y = y + obj->y;
            if (x + w >= 0 && x <= 0xEF && y + h >= 0 && y <= 0x9F) {
                s32 key;
                s32 prioBits;

                prio = prioTab[(part->byte5 >> 2) & 3];
                prioBits = prio << 10;
                key = (prio << 30) | (obj->group << 14);
                key = key | (*ctr << 7);
                key = key | index;
                node->key = key;
                (*ctr)++;

                if (affine != 0 && sub == 0) {
                    slotKey = (part->byte3 >> 4) & 3;
                    slotKey = slotKey ^ ((((u32)obj->flip >> 6) & 1) | ((obj->flip >> 7) << 1));
                    if (slotTable[slotKey] == -1) {
                        slotTable[slotKey] = (u8)slotNext;
                        slotNext++;
                        {
                            u32 ctrl = (*(u16 *)&obj->ctrlLo);
                            (*(u16 *)&obj->ctrlLo) = (ctrl & 0xFFFFFC3F)
                                        | (((((ctrl >> 6) & 0xF) + 1) & 0xF) << 6);
                        }
                    }
                }

                node->attr3 = 0;
                {
                    s32 attr0Hi;
                    s32 byte1;
                    s32 shape;
                    s32 flipXBit;
                    if ((obj->flip & 0xC) != 0) {
                        attr0Hi = (((u32)obj->flip << 28) >> 30) << 10;
                        byte1 = part->flags1;
                    } else {
                        byte1 = part->flags1;
                        attr0Hi = (0xC & byte1) << 8;
                    }
                    shape = ((byte1 & 0xF0) | affine) << 8;
                    flipXBit = ((((u32)obj->flip << 27) >> 31)) << 12;
                    node->attr0 = attr0Hi | shape | flipXBit | (y & 0xFF);
                }

                if (affine != 0) {
                    s32 b3hi = (part->byte3 & 0xC0) << 8;
                    s32 xBits = x & 0x1FF;
                    s32 slotVal;
                    if (sub == 0) {
                        slotVal = ((s8)slotTable[slotKey]) << 9;
                    } else {
                        slotVal = ((((u32)obj->ctrlLo << 26) >> 27) + ((part->byte3 & 0x3E) >> 1)) << 9;
                    }
                    node->attr1 = slotVal | b3hi | xBits;
                } else {
                    s32 v = (part->byte3 & 0xC0)
                            | ((part->byte3 & 0x10) ^ ((((u32)obj->flip << 25) >> 31) << 4));
                    node->attr1 = ((v | ((part->byte3 & 0x20) ^ ((obj->flip >> 7) << 5))) << 8)
                                  | (x & 0x1FF);
                }

                {
                    s32 b5 = part->byte5;
                    s32 b5hi = b5 >> 4;
                    s32 tile;
                    if (tilePtr == 0) {
                        tile = part->tile | ((b5 & 3) << 8);
                    } else {
                        tile = *tilePtr;
                    }
                    node->attr2 = prioBits | pal[b5hi] | ((obj->tileBase >> 5) + tile);
                    node = node->next;
                }
            }

            if (tilePtr != 0) {
                tilePtr++;
            }
            index++;
            part++;
        } while (index < countIn);
    }

    *streamIn = (u32 *)part;
    return (u32)node;
}
END_NONMATCH


struct OamPart5 {
    s8 dx;
    s8 dy;
    u8 form;
    u8 tile;
    u8 flags;
};

NONMATCH("asm/code_0801EED0/draw_parts_rec5.s", u32 draw_parts_rec5(struct OamNode *nodeIn, struct RecObj *obj,
                    u32 **streamIn, s32 countIn, u8 *tilePtr, s8 *slotTable, u32 slotNext, u8 *ctr))
{
    u16 pal[16];
    struct OamNode *node;
    struct OamPart5 *part;
    s32 mode;
    s32 prio;
    s32 index;
    s32 flags;
    s32 sub;
    s32 affine;
    s32 slotIdx;
    s32 w, h, dx, dy, flip;

    node = nodeIn;

    {
        s32 i = 0;
        u16 *slot = pal;
        s32 mask = obj->palMask;
        if (mask != 0) {
            do {
                if (mask & 1) {
                    *slot = i << 12;
                    slot++;
                }
                i++;
                mask >>= 1;
            } while (mask != 0);
        }
    }

    {
        s32 angle = obj->angleS;
        if ((angle != 0 && (obj->flags29 & 0x80) == 0)
            || obj->scaleX > 0x100 || obj->scaleY > 0x100 || obj->pa > 0x100 || obj->pd > 0x100) {
            mode = 3;
        } else {
            mode = 1;
            if (angle == 0 && *(u32 *)&obj->scaleX == 0x01000100) {
                mode = (*(u32 *)&obj->pa ^ *(u32 *)&obj->scaleX) != 0;
            }
        }
    }

    part = (struct OamPart5 *)*streamIn;
    index = 0;

    if (index < countIn) {
        do {
            flags = part->flags;
            sub = (flags >> 2) & 3;
            affine = 3;
            if (mode != 3) {
                affine = mode;
                if (sub != 0) {
                    affine = sub;
                }
            }

            {
                const u8 *sz = gOamSizeTable + (((u32)flags >> 4) << 1);
                w = sz[0];
                h = sz[1];
            }
            dx = part->dx;
            dy = part->dy;
            if (sub == 3) {
                dx = dx + (w >> 1);
                dy = dy + (h >> 1);
            }

            {
                s32 sx = obj->scaleX;
                if (sx != 0x100) {
                    s32 w8 = w << 8;
                    s32 dxs = dx * sx;
                    s32 wsx = w8 * sx;
                    wsx = round_asr8(wsx);
                    dx = dxs - half_of(w8 - wsx);
                    dx = round_asr8(dx);
                    w = round_asr8(w8);
                }
            }

            flip = obj->flip;
            if ((flip & 0x40) != 0) {
                dx = -dx - w;
            }

            {
                s32 sy = obj->scaleY;
                if (sy != 0x100) {
                    s32 h8 = h << 8;
                    s32 dys = dy * sy;
                    s32 hsy = h8 * sy;
                    hsy = round_asr8(hsy);
                    dy = dys - half_of(h8 - hsy);
                    dy = round_asr8(dy);
                    h = round_asr8(h8);
                }
            }
            if ((flip & 0x80) != 0) {
                dy = -dy - h;
            }

            if ((*(u16 *)&obj->angleS) != 0) {
                s32 hw = half_of(w);
                s32 hh = half_of(h);
                s32 cx = dx + hw;
                s32 cy = dy + hh;
                s32 ang = ((*(u16 *)&obj->angleS) >> 8) & 0xFF;
                s32 cs = gCosineTable[ang];
                s32 sn = gSineTable[ang];
                s32 rx = cs * cx + sn * cy;
                s32 ry = -cx * sn + cy * cs;
                dx = round_asr14(rx) - hw;
                dy = round_asr14(ry) - hh;
            }

            if (*(u32 *)&obj->pa != 0x01000100) {
                s32 pa = obj->pa;
                s32 pd = obj->pd;
                s32 dxp = pa * dx;
                s32 wpa = pa * w;
                s32 dyp = pd * dy;
                s32 hpd = pd * h;
                dxp = round_asr8(dxp);
                wpa = round_asr8(wpa);
                dx = dxp - half_of(w - wpa);
                dyp = round_asr8(dyp);
                hpd = round_asr8(hpd);
                dy = dyp - half_of(h - hpd);
            }

            if (affine == 3) {
                dx = dx - half_of(w);
                dy = dy - half_of(h);
                w = w << 1;
                h = h << 1;
            }

            dx = dx + obj->x;
            dy = dy + obj->y;
            if (dx + w >= 0 && dx <= 0xEF && dy + h >= 0 && dy <= 0x9F) {
                s32 key;

                if ((((u32)part->flags >> 1) & 1) == 0) {
                    prio = ((u32)obj->priority << 30) >> 20;
                } else {
                    prio = (((u32)obj->priority << 28) >> 30) << 10;
                }
                key = (prio << 20) | (obj->group << 14);
                key = key | (*ctr << 7);
                key = key | index;
                node->key = key;
                (*ctr)++;

                if (affine != 0 && sub == 0) {
                    slotIdx = ((u32)part->form >> 2) & 3;
                    slotIdx = slotIdx ^ ((((u32)obj->flip >> 6) & 1) | ((obj->flip >> 7) << 1));
                    if (slotTable[slotIdx] == -1) {
                        slotTable[slotIdx] = (u8)slotNext;
                        slotNext++;
                        {
                            u32 ctrl = (*(u16 *)&obj->ctrlLo);
                            (*(u16 *)&obj->ctrlLo) = (ctrl & 0xFFFFFC3F)
                                        | (((((ctrl >> 6) & 0xF) + 1) & 0xF) << 6);
                        }
                    }
                }

                node->attr3 = 0;
                node->attr0 = ((*(u16 *)&obj->ctrlLo) & 0x3C00) | ((flags & 0xC0) << 8)
                            | (affine << 8) | (dy & 0xFF);

                if (affine != 0) {
                    s32 size = ((u32)flags & 0x30) << 10;
                    s32 x9 = dx & 0x1FF;
                    s32 rot;
                    if (sub != 0) {
                        s32 base = ((u32)obj->ctrlLo << 26) >> 27;
                        rot = base + (part->form & 0xF);
                    } else {
                        rot = slotTable[slotIdx];
                    }
                    node->attr1 = (rot << 9) | size | x9;
                } else {
                    s32 flagsHi = flags & 0x30;
                    s32 form = part->form;
                    s32 fx = (form & 4) ^ ((((u32)obj->flip >> 6) & 1) << 2);
                    s32 fy = (form & 8) ^ ((obj->flip >> 7) << 3);
                    node->attr1 = ((flagsHi | (fx | fy)) << 10) | (dx & 0x1FF);
                }

                {
                    s32 palIdx = (u32)part->form >> 4;
                    s32 tile;
                    if (tilePtr == 0) {
                        tile = part->tile | ((flags & 1) << 8);
                    } else {
                        tile = *tilePtr;
                    }
                    node->attr2 = prio | pal[palIdx] | ((obj->tileBase >> 5) + tile);
                    node = node->next;
                }
            }

            if (tilePtr != 0) {
                tilePtr++;
            }
            index++;
            part++;
        } while (index < countIn);
    }

    *streamIn = (u32 *)part;
    return (u32)node;
}
END_NONMATCH

NONMATCH("asm/code_0801EED0/draw_parts_rec4.s", u32 draw_parts_rec4(struct OamNode *nodeIn, struct RecObj *obj,
                    u32 **streamIn, s32 countIn, u8 *tilePtr, s8 *slotTable, u32 slotNext, u8 *ctr))
{
    u16 pal[16];
    struct OamNode *node;
    u32 **stream;
    s32 count;
    s32 mode;
    s32 prio;
    struct OamPart *part;
    s32 index;
    s32 flags;
    s32 slotIdx;
    s32 w, h, dx, dy, flip;

    node = nodeIn;
    stream = streamIn;
    count = countIn;

    {
        s32 i = 0;
        u16 *slot = pal;
        s32 mask = obj->palMask;
        if (mask != 0) {
            do {
                if (mask & 1) {
                    *slot = i << 12;
                    slot++;
                }
                i++;
                mask >>= 1;
            } while (mask != 0);
        }
    }

    {
        s32 angle = obj->angleS;
        if ((angle != 0 && (obj->flags29 & 0x80) == 0)
            || obj->scaleX > 0x100 || obj->scaleY > 0x100 || obj->pa > 0x100 || obj->pd > 0x100) {
            mode = 3;
        } else {
            mode = 1;
            if (angle == 0 && *(u32 *)&obj->scaleX == 0x01000100) {
                mode = (*(u32 *)&obj->pa ^ *(u32 *)&obj->scaleX) != 0;
            }
        }
    }

    prio = ((u32)obj->priority << 30) >> 20;
    part = (struct OamPart *)*stream;
    index = 0;

    if (index < count) {
        do {
            flags = part->flags;
            {
                const u8 *sz = gOamSizeTable + (((u32)flags >> 4) << 1);
                w = sz[0];
                h = sz[1];
            }
            dx = part->dx;
            dy = part->dy;

            {
                s32 sx = obj->scaleX;
                if (sx != 0x100) {
                    s32 w8 = w << 8;
                    s32 dxs = dx * sx;
                    s32 wsx = w8 * sx;
                    wsx = wsx / 0x100;
                    dx = dxs - half_of(w8 - wsx);
                    dx = dx / 0x100;
                    w = w8 / 0x100;
                }
            }

            flip = obj->flip;
            if ((flip & 0x40) != 0) {
                dx = -dx - w;
            }

            {
                s32 sy = obj->scaleY;
                if (sy != 0x100) {
                    s32 h8 = h << 8;
                    s32 dys = dy * sy;
                    s32 hsy = h8 * sy;
                    hsy = hsy / 0x100;
                    dy = dys - half_of(h8 - hsy);
                    dy = dy / 0x100;
                    h = h8 / 0x100;
                }
            }
            if ((flip & 0x80) != 0) {
                dy = -dy - h;
            }

            if ((*(u16 *)&obj->angleS) != 0) {
                s32 hw = half_of(w);
                s32 hh = half_of(h);
                s32 cx = dx + hw;
                s32 cy = dy + hh;
                s32 ang = ((*(u16 *)&obj->angleS) >> 8) & 0xFF;
                s32 cs = gCosineTable[ang];
                s32 sn = gSineTable[ang];
                s32 rx = cx * cs + sn * cy;
                s32 ry = -cx * sn + cy * cs;
                dx = rx / 0x4000 - hw;
                dy = ry / 0x4000 - hh;
            }

            if (*(u32 *)&obj->pa != 0x01000100) {
                s32 pa = obj->pa;
                s32 pd = obj->pd;
                s32 dxp = pa * dx;
                s32 wpa = pa * w;
                s32 dyp = pd * dy;
                s32 hpd = pd * h;
                dxp = dxp / 0x100;
                wpa = wpa / 0x100;
                dx = dxp - half_of(w - wpa);
                dyp = dyp / 0x100;
                hpd = hpd / 0x100;
                dy = dyp - half_of(h - hpd);
            }

            if (mode == 3) {
                dx = dx - half_of(w);
                dy = dy - half_of(h);
                w = w << 1;
                h = h << 1;
            }

            dx = dx + obj->x;
            dy = dy + obj->y;
            if (dx + w >= 0 && dx <= 0xEF && dy + h >= 0 && dy <= 0x9F) {
                s32 key = (prio << 20) | (obj->group << 14);
                key = key | (*ctr << 7);
                key = key | index;
                node->key = key;
                (*ctr)++;

                if (mode != 0) {
                    slotIdx = (u32)flags >> 2;
                    slotIdx = (slotIdx & 3) ^ ((((u32)obj->flip >> 6) & 1) | ((obj->flip >> 7) << 1));
                    if (slotTable[slotIdx] == -1) {
                        slotTable[slotIdx] = (u8)slotNext;
                        slotNext++;
                        {
                            u32 ctrl = (*(u16 *)&obj->ctrlLo);
                            (*(u16 *)&obj->ctrlLo) = (ctrl & 0xFFFFFC3F)
                                        | (((((ctrl >> 6) & 0xF) + 1) & 0xF) << 6);
                        }
                    }
                }

                node->attr3 = 0;
                node->attr0 = ((*(u16 *)&obj->ctrlLo) & 0x3C00) | ((flags & 0xC0) << 8) | (mode << 8) | (dy & 0xFF);

                if (mode != 0) {
                    s32 size = ((u32)flags & 0x30) << 10;
                    s32 v = size | ((s32)slotTable[slotIdx] << 9);
                    node->attr1 = v | (dx & 0x1FF);
                } else {
                    s32 f2 = flags & 0x3C;
                    s32 fx = (((u32)obj->flip >> 6) & 1) << 2;
                    s32 fy = (obj->flip >> 7) << 3;
                    s32 v = (f2 ^ (fx | fy)) << 10;
                    node->attr1 = v | (dx & 0x1FF);
                }

                {
                    s32 palIdx = flags & 3;
                    s32 tile;
                    if (tilePtr == 0) {
                        tile = part->tile;
                    } else {
                        tile = *tilePtr;
                    }
                    node->attr2 = prio | pal[palIdx] | ((obj->tileBase >> 5) + tile);
                    node = node->next;
                }
            }

            if (tilePtr != 0) {
                tilePtr++;
            }
            index++;
            part++;
        } while (index < count);
    }

    *stream = (u32 *)part;
    return (u32)node;
}
END_NONMATCH

NONMATCH("asm/code_0801EED0/draw_parts_one_pal.s", u32 draw_parts_one_pal(struct OamNode *nodeIn, struct RecObj *obj,
                    u32 **streamIn, s32 countIn, u8 *tilePtr, s8 *slotTable, u32 slotNext, u8 *ctr))
{
    u16 pal[16];
    s32 palBits;
    struct OamNode *node;
    u32 **stream;
    s32 count;
    s32 mode;
    s32 prio;
    struct OamPart *part;
    s32 index;
    s32 flags;
    s32 slotIdx;
    s32 w, h, dx, dy, flip;

    node = nodeIn;
    stream = streamIn;
    count = countIn;

    {
        s32 mask = obj->palMask;
        palBits = 0;
        while ((mask & 1) == 0) {
            palBits++;
            mask >>= 1;
        }
    }
    palBits = palBits << 12;

    {
        s32 angle = obj->angleS;
        if ((angle != 0 && (obj->flags29 & 0x80) == 0)
            || obj->scaleX > 0x100 || obj->scaleY > 0x100 || obj->pa > 0x100 || obj->pd > 0x100) {
            mode = 3;
        } else {
            mode = 1;
            if (angle == 0 && *(u32 *)&obj->scaleX == 0x01000100) {
                mode = (*(u32 *)&obj->pa ^ *(u32 *)&obj->scaleX) != 0;
            }
        }
    }

    part = (struct OamPart *)*stream;
    index = 0;

    if (index < count) {
        do {
            flags = part->flags;
            {
                const u8 *sz = gOamSizeTable + (((u32)flags >> 4) << 1);
                w = sz[0];
                h = sz[1];
            }
            dx = part->dx;
            dy = part->dy;

            {
                s32 sx = obj->scaleX;
                if (sx != 0x100) {
                    s32 w8 = w << 8;
                    s32 dxs = dx * sx;
                    s32 wsx = w8 * sx;
                    wsx = wsx / 0x100;
                    dx = dxs - half_of(w8 - wsx);
                    dx = dx / 0x100;
                    w = w8 / 0x100;
                }
            }

            flip = obj->flip;
            if ((flip & 0x40) != 0) {
                dx = -dx - w;
            }

            {
                s32 sy = obj->scaleY;
                if (sy != 0x100) {
                    s32 h8 = h << 8;
                    s32 dys = dy * sy;
                    s32 hsy = h8 * sy;
                    hsy = hsy / 0x100;
                    dy = dys - half_of(h8 - hsy);
                    dy = dy / 0x100;
                    h = h8 / 0x100;
                }
            }
            if ((flip & 0x80) != 0) {
                dy = -dy - h;
            }

            if ((*(u16 *)&obj->angleS) != 0) {
                s32 hw = half_of(w);
                s32 hh = half_of(h);
                s32 cx = dx + hw;
                s32 cy = dy + hh;
                s32 ang = ((*(u16 *)&obj->angleS) >> 8) & 0xFF;
                s32 cs = gCosineTable[ang];
                s32 sn = gSineTable[ang];
                s32 rx = cx * cs + sn * cy;
                s32 ry = -cx * sn + cy * cs;
                dx = rx / 0x4000 - hw;
                dy = ry / 0x4000 - hh;
            }

            if (*(u32 *)&obj->pa != 0x01000100) {
                s32 pa = obj->pa;
                s32 pd = obj->pd;
                s32 dxp = pa * dx;
                s32 wpa = pa * w;
                s32 dyp = pd * dy;
                s32 hpd = pd * h;
                dxp = dxp / 0x100;
                wpa = wpa / 0x100;
                dx = dxp - half_of(w - wpa);
                dyp = dyp / 0x100;
                hpd = hpd / 0x100;
                dy = dyp - half_of(h - hpd);
            }

            if (mode == 3) {
                dx = dx - half_of(w);
                dy = dy - half_of(h);
                w = w << 1;
                h = h << 1;
            }

            dx = dx + obj->x;
            dy = dy + obj->y;
            if (dx + w >= 0 && dx <= 0xEF && dy + h >= 0 && dy <= 0x9F) {
                s32 key;
                if (((flags >> 1) & 1) == 0) {
                    prio = ((u32)obj->priority << 28) >> 30 << 10;
                } else {
                    prio = ((u32)obj->priority << 30) >> 20;
                }
                key = (prio << 20) | (obj->group << 14);
                key = key | (*ctr << 7);
                key = key | index;
                node->key = key;
                (*ctr)++;

                if (mode != 0) {
                    slotIdx = (u32)flags >> 2;
                    slotIdx = (slotIdx & 3) ^ ((((u32)obj->flip >> 6) & 1) | ((obj->flip >> 7) << 1));
                    if (slotTable[slotIdx] == -1) {
                        slotTable[slotIdx] = (u8)slotNext;
                        slotNext++;
                        {
                            u32 ctrl = (*(u16 *)&obj->ctrlLo);
                            (*(u16 *)&obj->ctrlLo) = (ctrl & 0xFFFFFC3F)
                                        | (((((ctrl >> 6) & 0xF) + 1) & 0xF) << 6);
                        }
                    }
                }

                node->attr3 = 0;
                node->attr0 = ((*(u16 *)&obj->ctrlLo) & 0x3C00) | ((flags & 0xC0) << 8) | (mode << 8) | (dy & 0xFF);

                if (mode != 0) {
                    s32 size = ((u32)flags & 0x30) << 10;
                    s32 v = size | ((s32)slotTable[slotIdx] << 9);
                    node->attr1 = v | (dx & 0x1FF);
                } else {
                    s32 f2 = flags & 0x3C;
                    s32 fx = (((u32)obj->flip >> 6) & 1) << 2;
                    s32 fy = (obj->flip >> 7) << 3;
                    s32 v = (f2 ^ (fx | fy)) << 10;
                    node->attr1 = v | (dx & 0x1FF);
                }

                {
                    s32 tile;
                    if (tilePtr == 0) {
                        tile = part->tile | ((flags & 1) << 8);
                    } else {
                        tile = *tilePtr;
                    }
                    node->attr2 = prio | palBits | ((obj->tileBase >> 5) + tile);
                    node = node->next;
                }
            }

            if (tilePtr != 0) {
                tilePtr++;
            }
            index++;
            part++;
        } while (index < count);
    }

    *stream = (u32 *)part;
    return (u32)node;
}
END_NONMATCH

struct OamPart3 {
    u8 x;
    u8 y;
    u8 tile;
};

NONMATCH("asm/code_0801EED0/draw_parts_one_pal_rec3.s", u32 draw_parts_one_pal_rec3(struct OamNode *nodeIn, struct RecObj *obj, u32 **streamIn, s32 countIn, u8 *tilePtr, s8 *slotTable, u32 slotNext, u8 *ctr))
{
    struct OamNode *node;
    u32 **stream;
    s32 count;
    s32 pal;
    s32 mode;
    s32 prio;
    struct OamPart3 *part;
    s32 index;
    s32 b0, b1, b2;
    s32 w, h, x, y, flip;
    s32 angle;

    node = nodeIn;
    stream = streamIn;
    count = countIn;
    pal = 0;

    {
        s32 mask = obj->palMask;
        while ((mask & 1) == 0) {
            pal++;
            mask >>= 1;
        }
    }
    pal = pal << 12;

    angle = obj->angleS;
    if ((angle != 0 && (obj->flags29 & 0x80) == 0)
        || obj->scaleX > 0x100 || obj->scaleY > 0x100 || obj->pa > 0x100 || obj->pd > 0x100) {
        mode = 3;
    } else {
        mode = 1;
        if (angle == 0 && *(u32 *)&obj->scaleX == 0x01000100) {
            mode = (*(u32 *)&obj->pa ^ *(u32 *)&obj->scaleX) != 0;
        }
    }

    prio = ((u32)obj->priority << 30) >> 20;
    part = (struct OamPart3 *)*stream;
    index = 0;

    if (index < count) {
        do {
            b0 = part->x;
            b1 = part->y;
            b2 = part->tile;
            {
                const u8 *sz = gOamSizeTable + ((((u32)b0 >> 6) << 3) + (((u32)b1 >> 6) << 1));
                w = sz[0];
                h = sz[1];
            }
            x = b0 & 0x3F;
            if ((x & 0x20) != 0) {
                x |= 0xFFFFFFC0;
            }
            y = b1 & 0x3F;
            if ((y & 0x20) != 0) {
                y |= 0xFFFFFFC0;
            }

            {
                s32 sx = obj->scaleX;
                if (sx != 0x100) {
                    s32 w8 = w << 8;
                    s32 xs = x * sx;
                    s32 wsx = w8 * sx;
                    wsx = wsx / 0x100;
                    x = xs - half_of(w8 - wsx);
                    x = x / 0x100;
                    w = w8 / 0x100;
                }
            }

            flip = obj->flip;
            if ((flip & 0x40) != 0) {
                x = -x - w;
            }

            {
                s32 sy = obj->scaleY;
                if (sy != 0x100) {
                    s32 h8 = h << 8;
                    s32 ys = y * sy;
                    s32 hsy = h8 * sy;
                    hsy = hsy / 0x100;
                    y = ys - half_of(h8 - hsy);
                    y = y / 0x100;
                    h = h8 / 0x100;
                }
            }
            if ((flip & 0x80) != 0) {
                y = -y - h;
            }

            if ((*(u16 *)&obj->angleS) != 0) {
                s32 hw = half_of(w);
                s32 hh = half_of(h);
                s32 cx = x + hw;
                s32 cy = y + hh;
                s32 ang = ((*(u16 *)&obj->angleS) >> 8) & 0xFF;
                s32 cs = gCosineTable[ang];
                s32 sn = gSineTable[ang];
                s32 rx = cx * cs + sn * cy;
                s32 ry = -cx * sn + cy * cs;
                x = rx / 0x4000 - hw;
                y = ry / 0x4000 - hh;
            }

            if (*(u32 *)&obj->pa != 0x01000100) {
                s32 pa = obj->pa;
                s32 pd = obj->pd;
                s32 xp = pa * x;
                s32 wpa = pa * w;
                s32 yp = pd * y;
                s32 hpd = pd * h;
                xp = xp / 0x100;
                wpa = wpa / 0x100;
                x = xp - half_of(w - wpa);
                yp = yp / 0x100;
                hpd = hpd / 0x100;
                y = yp - half_of(h - hpd);
            }

            if (mode == 3) {
                x = x - half_of(w);
                y = y - half_of(h);
                w = w << 1;
                h = h << 1;
            }

            x = x + obj->x;
            y = y + obj->y;
            if (x + w >= 0 && x <= 0xEF && y + h >= 0 && y <= 0x9F) {
                s32 key = (prio << 20) | (obj->group << 14);
                s32 slotKey;
                key = key | (*ctr << 7);
                key = key | index;
                node->key = key;
                (*ctr)++;

                flip = obj->flip;
                slotKey = (b2 >> 7) ^ (((flip << 25) >> 31) | ((flip >> 7) << 1));
                if (mode != 0) {
                    if (slotTable[slotKey] == -1) {
                        slotTable[slotKey] = (u8)slotNext;
                        slotNext++;
                        {
                            u32 ctrl = (*(u16 *)&obj->ctrlLo);
                            (*(u16 *)&obj->ctrlLo) = (ctrl & 0xFFFFFC3F)
                                                     | (((((ctrl >> 6) & 0xF) + 1) & 0xF) << 6);
                        }
                    }
                }

                node->attr3 = 0;
                node->attr0 = ((*(u16 *)&obj->ctrlLo) & 0x3C00) | ((b0 & 0xC0) << 8) | (mode << 8) | (y & 0xFF);

                if (mode != 0) {
                    s32 v = ((b1 & 0xC0) << 8) | ((s32)slotTable[slotKey] << 9);
                    node->attr1 = v | (x & 0x1FF);
                } else {
                    node->attr1 = (((b1 & 0xC0) << 8) | (slotKey << 12)) | (x & 0x1FF);
                }

                {
                    s32 tile;
                    if (tilePtr != 0) {
                        tile = *tilePtr;
                    } else {
                        tile = b2 & 0x7F;
                    }
                    node->attr2 = pal | prio | ((*(u16 *)&obj->tileBase >> 5) + tile);
                    node = node->next;
                }
            }

            if (tilePtr != 0) {
                tilePtr++;
            }
            index++;
            part = (struct OamPart3 *)((u8 *)part + 3);
        } while (index < count);
    }

    *stream = (u32 *)part;
    return (u32)node;
}
END_NONMATCH


NONMATCH("asm/code_0801EED0/draw_parts_one_pal_rec3_plain.s", u32 draw_parts_one_pal_rec3_plain(struct OamNode *nodeIn, struct RecObj *obj, u32 **streamIn, s32 countIn, u8 *tilePtr, s8 *slotTable, u32 slotNext, u8 *ctr))
{
    struct OamNode *node;
    u32 **stream;
    s32 count;
    s32 pal;
    s32 mode;
    s32 prio;
    struct OamPart3 *part;
    s32 index;
    s32 b0, b1, b2;
    s32 w, h, x, y, flip;
    s32 angle;

    node = nodeIn;
    stream = streamIn;
    count = countIn;
    pal = 0;

    {
        s32 mask = obj->palMask;
        while ((mask & 1) == 0) {
            pal++;
            mask >>= 1;
        }
    }
    pal = pal << 12;

    angle = obj->angleS;
    if ((angle != 0 && (obj->flags29 & 0x80) == 0)
        || obj->scaleX > 0x100 || obj->scaleY > 0x100 || obj->pa > 0x100 || obj->pd > 0x100) {
        mode = 3;
    } else {
        mode = 1;
        if (angle == 0 && *(u32 *)&obj->scaleX == 0x01000100) {
            mode = (*(u32 *)&obj->pa ^ *(u32 *)&obj->scaleX) != 0;
        }
    }

    prio = ((u32)obj->priority << 30) >> 20;
    part = (struct OamPart3 *)*stream;
    index = 0;

    if (index < count) {
        do {
            b0 = part->x;
            b1 = part->y;
            b2 = part->tile;
            {
                const u8 *sz = gOamSizeTable + ((((u32)b0 >> 6) << 3) + (((u32)b1 >> 6) << 1));
                w = sz[0];
                h = sz[1];
            }
            x = b0 & 0x3F;
            if ((x & 0x20) != 0) {
                x |= 0xFFFFFFC0;
            }
            y = b1 & 0x3F;
            if ((y & 0x20) != 0) {
                y |= 0xFFFFFFC0;
            }

            {
                s32 sx = obj->scaleX;
                if (sx != 0x100) {
                    s32 w8 = w << 8;
                    s32 xs = x * sx;
                    s32 wsx = w8 * sx;
                    wsx = wsx / 0x100;
                    x = xs - half_of(w8 - wsx);
                    x = x / 0x100;
                    w = w8 / 0x100;
                }
            }

            flip = obj->flip;
            if ((flip & 0x40) != 0) {
                x = -x - w;
            }

            {
                s32 sy = obj->scaleY;
                if (sy != 0x100) {
                    s32 h8 = h << 8;
                    s32 ys = y * sy;
                    s32 hsy = h8 * sy;
                    hsy = hsy / 0x100;
                    y = ys - half_of(h8 - hsy);
                    y = y / 0x100;
                    h = h8 / 0x100;
                }
            }
            if ((flip & 0x80) != 0) {
                y = -y - h;
            }

            if ((*(u16 *)&obj->angleS) != 0) {
                s32 hw = half_of(w);
                s32 hh = half_of(h);
                s32 cx = x + hw;
                s32 cy = y + hh;
                s32 ang = ((*(u16 *)&obj->angleS) >> 8) & 0xFF;
                s32 cs = gCosineTable[ang];
                s32 sn = gSineTable[ang];
                s32 rx = cx * cs + sn * cy;
                s32 ry = -cx * sn + cy * cs;
                x = rx / 0x4000 - hw;
                y = ry / 0x4000 - hh;
            }

            if (*(u32 *)&obj->pa != 0x01000100) {
                s32 pa = obj->pa;
                s32 pd = obj->pd;
                s32 xp = pa * x;
                s32 wpa = pa * w;
                s32 yp = pd * y;
                s32 hpd = pd * h;
                xp = xp / 0x100;
                wpa = wpa / 0x100;
                x = xp - half_of(w - wpa);
                yp = yp / 0x100;
                hpd = hpd / 0x100;
                y = yp - half_of(h - hpd);
            }

            if (mode == 3) {
                x = x - half_of(w);
                y = y - half_of(h);
                w = w << 1;
                h = h << 1;
            }

            x = x + obj->x;
            y = y + obj->y;
            if (x + w >= 0 && x <= 0xEF && y + h >= 0 && y <= 0x9F) {
                s32 key = (prio << 20) | (obj->group << 14);
                s32 slotKey;
                key = key | (*ctr << 7);
                key = key | index;
                node->key = key;
                (*ctr)++;

                flip = obj->flip;
                slotKey = ((flip << 25) >> 31) | ((flip >> 7) << 1);
                if (mode != 0) {
                    if (slotTable[slotKey] == -1) {
                        slotTable[slotKey] = (u8)slotNext;
                        slotNext++;
                        {
                            u32 ctrl = (*(u16 *)&obj->ctrlLo);
                            (*(u16 *)&obj->ctrlLo) = (ctrl & 0xFFFFFC3F)
                                                     | (((((ctrl >> 6) & 0xF) + 1) & 0xF) << 6);
                        }
                    }
                }

                node->attr3 = 0;
                node->attr0 = ((*(u16 *)&obj->ctrlLo) & 0x3C00) | ((b0 & 0xC0) << 8) | (mode << 8) | (y & 0xFF);

                if (mode != 0) {
                    s32 v = ((b1 & 0xC0) << 8) | ((s32)slotTable[slotKey] << 9);
                    node->attr1 = v | (x & 0x1FF);
                } else {
                    node->attr1 = (((b1 & 0xC0) << 8) | (slotKey << 12)) | (x & 0x1FF);
                }

                {
                    s32 tile = b2;
                    if (tilePtr != 0) {
                        tile = *tilePtr;
                    }
                    node->attr2 = pal | prio | ((*(u16 *)&obj->tileBase >> 5) + tile);
                    node = node->next;
                }
            }

            if (tilePtr != 0) {
                tilePtr++;
            }
            index++;
            part = (struct OamPart3 *)((u8 *)part + 3);
        } while (index < count);
    }

    *stream = (u32 *)part;
    return (u32)node;
}
END_NONMATCH
