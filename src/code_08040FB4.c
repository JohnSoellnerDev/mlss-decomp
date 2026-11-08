#include "global.h"

struct ContactObj
{
    u8 id0;
    u8 link1;
    u8 filler2[0x0C - 0x02];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 posZB;
    u8 filler1C[0x24 - 0x1C];
    u8 dir24;
    u8 moveBits25;
    u8 filler26[0x29 - 0x26];
    s8 listIndex29;
    u8 filler2A[0x34 - 0x2A];
    s32 tgtX;
    s32 tgtY;
    s32 tgtZ;
    s32 tgtZB;
    u8 filler44[0x4C - 0x44];
    u16 h4C;
    u8 filler4E[0x54 - 0x4E];
    u8 group54;
    u8 filler55[0x20C - 0x55];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 filler20F[0x210 - 0x20F];
    u8 flags210;
    u8 filler211[0x213 - 0x211];
    u8 change213;
    u8 filler214[0x216 - 0x214];
    u8 flags216;
    u8 filler217[0x238 - 0x217];
    u8 touchGroup238;
    u8 filler239[0x23E - 0x239];
    u16 h23E;
    u16 h240;
    u8 filler242[0x24C - 0x242];
    s32 v24C;
    s32 v250;
    u8 filler254[0x258 - 0x254];
    s32 speed258;
    s32 counter25C;
    u8 filler260[0x270 - 0x260];
    s16 partOff270;
    u16 size272;
    u16 zExt274;
    u16 size276;
    u8 filler278[0x2B5 - 0x278];
    u8 moveFlags2B5;
    u8 axisFlags2B6;
};

static inline int zb_counts(s32 zb, s32 tgtZB)
{
    if (zb > 0)
        return 1;
    if (zb >= 0)
        return 0;
    if (tgtZB >= 0)
        return 0;
    return 1;
}

void obj_stop_motion(void *obj);
void obj_finish_move_step(void *obj);

ASM_FUNC("asm/code_08040FB4/obj_update_cell_levels.s", void obj_update_cell_levels(void))

ASM_FUNC("asm/code_08040FB4/obj_emit_entry.s", void obj_emit_entry(void))

ASM_FUNC("asm/code_08040FB4/obj_load_part_shape.s", void obj_load_part_shape(void))

ASM_FUNC("asm/code_08040FB4/obj_set_part_sizes.s", void obj_set_part_sizes(void))

ASM_FUNC("asm/code_08040FB4/obj_try_ride_platform.s", void obj_try_ride_platform(void))

NONMATCH("asm/code_08040FB4/obj_resolve_contacts.s", void obj_resolve_contacts(struct ContactObj *obj, struct ContactObj **list, u32 count, u8 flag))
{
    struct ContactObj *other;
    s32 objLeft;
    s32 objPosY;
    s32 objTopZ;
    s32 remain;
    s32 i;
    s32 otherTopZ;
    s32 ovX;
    s32 ovY;
    s32 ovZ;
    s32 tgtX;
    s32 tgtY;
    s32 otherTgtX;
    s32 otherTgtY;
    s32 objPart;
    s32 otherPart;
    s32 objH;
    s32 otherH;
    s32 v;
    u32 side;
    u32 sideOther;
    u32 kind;
    u32 kindOther;
    u32 touch;
    u32 ctact;

    if (!(obj->flags20C & 1))
        return;
    if (obj->flags216 & 0x20)
        return;
    if (!((((u32)obj->flags20C << 25) >> 31) | (obj->flags20D & 1)))
        return;

    objLeft = obj->posX - (obj->size272 >> 1);
    objPosY = obj->posY;
    objTopZ = obj->posZ;
    if (zb_counts(obj->posZB, obj->tgtZB))
        objTopZ += obj->posZB;

    remain = 2;
    if (obj->flags20D & 1)
        remain = count;

    if (remain != 0) {
        i = 0;
        do {
            other = list[i];
            if (obj->group54 == other->group54)
                continue;
            if (flag != 0 && !(other->flags210 & 0x20))
                continue;
            if (!(other->flags20C & 1))
                continue;
            if (other->flags216 & 0x20) {
                if (other->group54 != obj->listIndex29)
                    continue;
            }
            if ((s8)other->listIndex29 == obj->group54)
                continue;

            if (other->flags20E & 0x38) {
                ctact = ((other->flags20D >> 1) & 1) & (obj->flags20D & 1);
                touch = ((obj->flags20D >> 1) & 1) & (other->flags20D & 1);
            } else {
                ctact = ((other->flags20D >> 1) & 1) & ((obj->flags20C >> 6) & 1);
                touch = ((obj->flags20C >> 5) & 1) & (other->flags20D & 1);
            }
            if (!ctact)
                continue;

            {
                s32 otherLeft = other->posX - (other->size272 >> 1);
                if (otherLeft >= objLeft)
                    ovX = objLeft + obj->size272 - otherLeft;
                else
                    ovX = otherLeft + other->size272 - objLeft;
                if (ovX <= 0)
                    break;
            }

            objPart = obj->partOff270;
            otherPart = other->partOff270;
            {
                s32 objY = objPosY + objPart;
                s32 otherY = other->posY + otherPart;
                objH = obj->size276;
                otherH = other->size276;
                if (otherY <= objY)
                    ovY = otherY - (objY - objH);
                else
                    ovY = objY - (otherY - otherH);
                if (ovY <= 0)
                    break;
            }

            otherTopZ = other->posZ;
            if (zb_counts(other->posZB, other->tgtZB))
                otherTopZ += other->posZB;
            if (otherTopZ > objTopZ)
                ovZ = objTopZ + obj->zExt274 - otherTopZ;
            else
                ovZ = otherTopZ + other->zExt274 - objTopZ;
            if (ovZ < 0)
                break;

            side = 0;
            sideOther = 0;
            kind = 0;
            kindOther = 0;
            if ((s8)obj->listIndex29 == other->group54) {
                kind = 2;
                kindOther = 1;
            } else {
                s32 objTz = obj->tgtZ + obj->tgtZB;
                if (objTz + obj->zExt274 > otherTopZ) {
                    if (objTz >= otherTopZ + other->zExt274) {
                        kind = 2;
                        kindOther = 1;
                    }
                } else {
                    kind = 1;
                    kindOther = 2;
                }
            }

            do {
                if (kind == 2) {
                    if ((s8)obj->listIndex29 != other->group54)
                        obj->posZB = (obj->posZB + ovZ) & 0xFFFFFF00;
                    obj->touchGroup238 = other->group54;
                    obj->link1 = other->group54;
                    if (obj->moveFlags2B5 & 8) {
                        obj_stop_motion(obj);
                        break;
                    }
                    if (obj->counter25C < 0)
                        break;
                    obj->counter25C = -1;
                    obj->change213 &= ~0x20;
                    obj->v24C = -1;
                    obj->v250 = -1;
                    obj->h23E = obj->h240;
                    break;
                } else if (kind == 1) {
                    if ((obj->h4C & 0x180) == 0x80)
                        break;
                    if ((s8)other->listIndex29 == obj->group54)
                        break;
                    obj->posZB -= ovZ;
                    if (obj->counter25C < 0)
                        break;
                    if (obj->speed258 > 0)
                        obj->speed258 = -obj->speed258;
                } else {
                    otherTgtX = other->tgtX;
                    otherTgtY = other->tgtY;
                    tgtX = obj->tgtX;
                    tgtY = obj->tgtY;
                    objPart = obj->partOff270;
                    otherPart = other->partOff270;
                    if (tgtX == otherTgtX && tgtY == otherTgtY) {
                        u32 bits = obj->dir24 & 7;
                        if (bits <= 1 || bits == 7) {
                            side = 1;
                            sideOther = 4;
                        } else if (bits == 2) {
                            side = 8;
                            sideOther = 2;
                        } else if (bits <= 5) {
                            side = 4;
                            sideOther = 1;
                        } else {
                            side = 2;
                            sideOther = 8;
                        }
                    } else {
                        s32 a = tgtY + objPart - obj->size276;
                        s32 b = otherTgtY + otherPart;
                        if (a >= b || tgtY + objPart <= b - other->size276) {
                            if (tgtY <= otherTgtY) {
                                side = 4;
                                sideOther = 1;
                            } else {
                                side = 1;
                                sideOther = 4;
                            }
                        } else if (tgtX >= otherTgtX) {
                            side = 8;
                            sideOther = 2;
                        } else {
                            side = 2;
                            sideOther = 8;
                        }
                    }

                    if (side == 8) {
                        if (obj->posX < tgtX) {
                            v = other->posX + ((other->size272 + obj->size272) >> 1);
                            if (v > tgtX)
                                v = tgtX;
                            if (v > obj->posX)
                                obj->posX = v;
                        }
                    } else if (side == 2) {
                        if (obj->posX > tgtX) {
                            v = other->posX - ((other->size272 + obj->size272) >> 1);
                            if (v < tgtX)
                                v = tgtX;
                            if (v < obj->posX)
                                obj->posX = v;
                        }
                    } else if (side == 1) {
                        if (obj->posY < tgtY) {
                            v = other->posY + obj->size276 + otherPart - objPart;
                            if (v > tgtY)
                                v = tgtY;
                            if (v > obj->posY)
                                obj->posY = v;
                        }
                    } else {
                        if (obj->posY > tgtY) {
                            v = other->posY + otherPart - other->size276 - objPart;
                            if (v < tgtY)
                                v = tgtY;
                            if (v < obj->posY)
                                obj->posY = v;
                        }
                    }
                }
            } while (0);

            if (ctact) {
                u16 h;
                obj->dir24 = (obj->dir24 & 0x87) | ((((obj->dir24 >> 3) & 0xF) | side) << 3);
                h = *(u16 *)&obj->dir24;
                h = (h & 0xFE7F) | ((((h >> 7) & 3) | kind) << 7);
                *(u16 *)&obj->dir24 = h;
                if ((s8)obj->id0 == -1)
                    obj->id0 = other->group54;
                if (other->flags20E & 0x38)
                    obj->moveBits25 |= 4;
                else
                    obj->moveBits25 |= 2;
            }

            if (touch != 0) {
                u16 h;
                other->dir24 = (other->dir24 & 0x87) | ((((other->dir24 >> 3) & 0xF) | sideOther) << 3);
                h = *(u16 *)&other->dir24;
                h = (h & 0xFE7F) | ((((h >> 7) & 3) | kindOther) << 7);
                *(u16 *)&other->dir24 = h;
                if ((s8)obj->id0 == -1)
                    obj->id0 = other->group54;
                other->moveBits25 |= 4;
            }
        } while (++i != remain);
    }

    if (obj->dir24 & 0x78) {
        if (obj->moveFlags2B5 & 7) {
            if (obj->axisFlags2B6 & 0x40) {
                obj_finish_move_step(obj);
                obj_stop_motion(obj);
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_08040FB4/obj_step_toward_goal.s", void obj_step_toward_goal(void))
