#include "global.h"
#include "ram_routines.h"

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct SubObj
{
    u8 filler0[0x12];
    u8 bits12;
};

struct LedgeObj
{
    u8 filler0[0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler18[0x20D - 0x18];
    u8 flags20D;
    u8 filler20E[0x20F - 0x20E];
    u8 flags20F;
    u8 flags210;
    u8 filler211;
    u8 seqLo212;
    u8 change213;
    u8 filler214[0x27A - 0x214];
    u16 sizeB1;
    u8 filler27C[0x290 - 0x27C];
    u8 *world;
    struct SubObj *sub;
    u8 filler298[0x2B0 - 0x298];
    u8 *seqPtr;
    u8 seqTimer;
};

extern s16 gTableLedge[];
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);
u32 tile_ptr_at(u8 *obj, s16 x, s16 y);
void obj_set_sprite_link(struct LedgeObj *obj, u32 value);

ASM_FUNC("asm/code_08044644/obj_settle_on_tiles.s", void obj_settle_on_tiles(void))

NONMATCH("asm/code_08044644/obj_probe_tile_heights.s", u8 *obj_probe_tile_heights(struct LedgeObj *obj, s32 x, s32 y, s32 *out0, s32 *out1, s32 *out2))
{
    u8 *tile;
    u32 t1;
    u32 t2;
    u32 t3;
    u32 f;
    s32 h0;
    s32 h1;
    u8 flags;

    tile = (u8 *)tile_ptr_at(obj->world, x, y);
    if ((tile[0] & 0x78) != 0x78)
    {
            t1 = *(u32 *)tile;
            h0 = (((t1 << 20) >> 28) << 11) << (t1 >> 31);
            *out0 = h0;
            flags = obj->flags210;
            t2 = *(u32 *)tile;
            if ((flags & 0x40) != 0)
            {
                f = (u32)flags << 28;
                if ((tile[2] & 0xF0) != 0 || (((t2 << 24) >> 31) & (f >> 31)) != 0)
                {
                    h1 = h0 + ((((t2 << 16) >> 28) << 11) << (t2 >> 31));
                    *out1 = h1;
                    t3 = *(u32 *)tile;
                    if ((((t3 << 24) >> 31) & (f >> 31)) != 0)
                        *out2 = 0x7FFFFFFF;
                    else
                        *out2 = h1 + ((((t3 << 8) >> 28) << 11) << (t3 >> 31));
                    return tile;
                }
            }
        *out2 = -1;
    }
    else
    {
        *out0 = 0x7FFFFFFF;
        *out2 = -1;
    }
    *out1 = -1;
    return tile;
}
END_NONMATCH

NONMATCH("asm/code_08044644/obj_probe_ledge.s", s32 obj_probe_ledge(struct LedgeObj *obj))
{
    s32 outA;
    s32 outB;
    s32 outC;
    u32 step = 0;
    u8 *tile;

    do
    {
        tile = tile_query(obj->world, obj->posX + gTableLedge[step * 2], obj->posY + gTableLedge[step * 2 + 1], &outA, &outB, &outC, (u8)0, (u8)((obj->flags20D >> 3) & 1));
        if ((tile[0] & 0x78) == 0x28 || (tile[0] & 0x78) == 0x38)
        {
            if (outC > obj->posZ && outB <= obj->posZ + obj->sizeB1)
                return 1;
        }
        tile = tile_query(obj->world, obj->posX + gTableLedge[step * 2 + 2], obj->posY + gTableLedge[step * 2 + 3], &outA, &outB, &outC, (u8)0, (u8)((obj->flags20D >> 3) & 1));
        if ((tile[0] & 0x78) == 0x28 || (tile[0] & 0x78) == 0x38)
        {
            if (outC > obj->posZ && outB <= obj->posZ + obj->sizeB1)
                return 1;
        }
        step += 2;
    } while (step <= 9);
    return 0;
}
END_NONMATCH

NONMATCH("asm/code_08044644/obj_step_sequence.s", void obj_step_sequence(struct LedgeObj *obj))
{
    s32 active;
    u16 *seqFlags;
    u16 fl;
    u8 *timer;
    s32 dir;
    struct SubObj *sub;
    u8 *ptr;
    u8 next;
    s32 v;

    seqFlags = (u16 *)&obj->seqLo212;
    fl = *seqFlags;
    active = 0x180;
    if ((active & fl) != 0)
    {
        timer = &obj->seqTimer;
        *timer = *timer - 1;
        if (*timer == 0)
        {
            if ((obj->flags20F & 2) != 0)
            {
                obj_set_sprite_link(obj, 0);
                ptr = obj->seqPtr + 2;
                if ((*seqFlags & active) == 0x80)
                    ptr = obj->seqPtr - 2;
                obj->seqPtr = ptr;
                next = *ptr;
            }
            else
            {
                obj_set_sprite_link(obj, 1);
                next = obj->seqPtr[1];
            }
            *timer = next;
        }
        if (*(s8 *)&obj->seqTimer == -1)
        {
            dir = 0;
            if ((*(u16 *)&obj->seqLo212 & 0x180) == 0x80)
                dir = 1;
            obj_set_sprite_link(obj, dir);
            { s32 f = ~0x180 & *(u16 *)&obj->seqLo212; *(u16 *)&obj->seqLo212 = f; }
            sub = obj->sub;
            v = (obj->change213 >> 6) << 1;
            sub->bits12 = (-7 & sub->bits12) | v;
        }
    }
}
END_NONMATCH

struct MoveObj
{
    u8 filler0[0x02];
    u8 angle;
    u8 angleZ;
    u8 filler4[0x0C - 0x04];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 posZB;
    u8 filler1C[0x20D - 0x1C];
    u8 flags20D;
    u8 filler20E[0x23E - 0x20E];
    u16 speed23E;
    u8 filler240[0x242 - 0x240];
    s16 moveX;
    s16 moveY;
    u8 filler246[0x2B5 - 0x246];
    u8 moveFlags;
    u8 filler2B6;
    u8 stepFlags;
    u8 filler2B8[0x2BC - 0x2B8];
    s32 savedX;
    s32 savedY;
    s32 savedZ;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 snapX;
    s32 snapY;
    s32 snapZ;
    s16 dirX;
    s16 dirY;
    s16 dirZ;
    u16 modeCount;
    u16 modeLimit;
};

u32 arc_tan2(s32 x, s32 y);

static inline s32 round14(s32 p)
{
    return p / 0x4000 + ((p / 0x2000) & 1);
}

NONMATCH("asm/code_08044644/contacts_obj_start_move.s", u32 contacts_obj_start_move(struct MoveObj *obj, s32 kind, s32 relative, s32 x, s32 y, s32 z, u16 speed))
{
    s32 tx;
    s32 ty;
    s32 tz;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dxp;
    s32 dyp;
    s32 dzp;
    s32 t;
    s32 r;
    s32 s;
    s32 p;
    s32 q;
    s32 len;
    s32 scale;

    tx = x << 8;
    ty = y << 8;
    tz = z << 8;
    if (relative == 1)
    {
        tx += obj->posX;
        ty += obj->posY;
        tz += obj->posZ + obj->posZB;
    }
    obj->savedX = obj->posX;
    obj->savedY = obj->posY;
    obj->savedZ = obj->posZ + obj->posZB;
    obj->snapX = tx;
    obj->snapY = ty;
    obj->snapZ = tz;
    if (obj->savedX >> 8 == obj->snapX >> 8 && obj->savedY >> 8 == obj->snapY >> 8)
    {
        if ((obj->flags20D & 0x40) != 0 || (obj->savedZ >> 8) == (tz >> 8))
            return 0;
    }

    dx = obj->snapX - obj->savedX;
    dy = obj->savedY - obj->snapY;
    if ((obj->flags20D & 0x40) != 0)
        dz = 0;
    else
        dz = obj->snapZ - obj->savedZ;

    t = dx;
    if (t < 0)
        t += 0xFF;
    dxp = t >> 8;
    t = dy;
    if (t < 0)
        t += 0xFF;
    dyp = t >> 8;
    t = dz;
    if (t < 0)
        t += 0xFF;
    dzp = t >> 8;

    if ((u32)(dx + 0x1000) > 0x2000 || dy < -0x1000 || dy > 0x1000 || dz < -0x1000 || dz > 0x1000)
    {
        dx = dxp;
        dy = dyp;
        dz = dzp;
    }

    r = (u16)arc_tan2((s16)dy, (s16)dx);
    obj->angle = (r >> 8) + ((r >> 7) & 1);

    if ((obj->flags20D & 0x40) == 0)
    {
        len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
        r = (u16)arc_tan2((s16)len, (s16)dz);
        obj->angleZ = (r >> 8) + ((r >> 7) & 1);
    }

    if (kind == 1)
    {
        s = speed;
        if (speed == 0)
            s = obj->speed23E;
        s = (s16)s;

        if ((obj->flags20D & 0x40) != 0)
        {
            obj->dirX = round14(s * gSineTable[obj->angle]);
            obj->dirY = -round14(s * gCosineTable[obj->angle]);
            obj->dirZ = 0;
            obj->stepFlags = (obj->stepFlags & -8) | 4;
        }
        else
        {
            p = round14(s * gSineTable[obj->angle]);
            obj->dirX = round14(gCosineTable[obj->angleZ] * p);
            q = round14(s * gCosineTable[obj->angle]);
            obj->dirY = -round14(gCosineTable[obj->angleZ] * q);
            obj->dirZ = round14(s * gSineTable[obj->angleZ]);
            obj->stepFlags = obj->stepFlags & -8;
        }
        obj->moveX = obj->dirX;
        obj->moveY = obj->dirY;
    }
    else
    {
        obj->modeLimit = speed;
        len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dxp * dxp + dyp * dyp + dzp * dzp);
        len = (len << 8) / obj->modeLimit;
        scale = (s16)len;
        obj->moveX = (scale * gSineTable[obj->angle]) / 0x4000;
        obj->moveY = -((scale * gCosineTable[obj->angle]) / 0x4000);
    }

    obj->modeCount = 0;
    obj->moveFlags = (obj->moveFlags & -8) | (kind & 7);
    return 1;
}
END_NONMATCH
