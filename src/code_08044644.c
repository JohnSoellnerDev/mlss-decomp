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

/* one map cell as tile_query and tile_ptr_at return it */
struct Tile
{
    u32 kindA : 3;
    u32 shape : 4;
    u32 unk7 : 9;
    u32 kindC : 4;
    u32 unk20 : 9;
    u32 mode : 2;
    u32 unk31 : 1;
};

struct GroundBits
{
    u8 bit0 : 1;
    u8 onTile : 1;
    u8 side : 2;
    u8 art : 2;
    u8 hi : 2;
};

struct ByteBits
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
} __attribute__((packed));

struct Word24
{
    u32 lo : 15;
    u32 mode : 2;
    u32 hi : 15;
};

struct SettleObj
{
    u8 filler0;
    s8 link01;
    u8 filler2[0x0C - 0x02];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 posZB;
    u8 filler1C[0x20 - 0x1C];
    struct Tile *tile20;
    u16 w24;
    u8 ground26;
    u8 filler27[0x34 - 0x27];
    s32 tgtX;
    s32 tgtY;
    s32 tgtZ;
    s32 tgtZB;
    u8 filler44[0x20C - 0x44];
    u8 flags20C;
    u8 flags20D;
    u8 filler20E[0x210 - 0x20E];
    u8 flags210;
    u8 filler211[0x216 - 0x211];
    u8 flags216;
    u8 filler217[0x230 - 0x217];
    s32 savedX;
    s32 savedY;
    u8 filler238[0x23E - 0x238];
    s16 unk23E;
    u8 filler240[0x250 - 0x240];
    s32 accum250;
    u8 filler254[0x25C - 0x254];
    s32 counter25C;
    u8 filler260[0x27A - 0x260];
    u16 sizeZ;
    u8 filler27C[0x27E - 0x27C];
    u16 sizeX;
    u16 sizeY;
    u8 filler282[0x290 - 0x282];
    u8 *world;
};

extern s16 gTableLedge[];
void *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);
void *tile_ptr_at(u8 *obj, s16 x, s16 y);
void obj_set_sprite_link(struct LedgeObj *obj, u32 value);
void obj_init_motion(struct SettleObj *obj, u32 a, s32 b, s32 c);

NONMATCH("asm/code_08044644/obj_settle_on_tiles.s", void obj_settle_on_tiles(struct SettleObj *obj, u8 cellStep))
{
    s32 surf;
    s32 lo;
    s32 hi;
    s32 outA;
    s32 outB;
    s32 outC;
    struct Tile *bestTile;
    s32 candA;
    s32 candB;
    s32 bestAX;
    s32 bestAY;
    s32 bestCX;
    s32 bestCY;
    s32 kindA;
    s32 kindC;
    s32 topTgt;
    s32 topTgtExt;
    s32 topCur;
    struct Tile *cell;
    s32 modeA;
    s32 modeB;
    s32 x;
    s32 y;
    s32 tx;
    s32 z;
    s32 tz;
    s32 tzb;
    s32 on;
    struct Tile *tile;
    struct Tile *bestATile;
    s32 bestA;
    s32 bestC;
    s32 probeX;
    s32 probeY;
    s32 adj;
    s32 half;
    s32 topLim;
    s32 topExt;
    s32 edgeOk;
    s32 zb;
    s32 found;

    if (cellStep == 0)
    {
        if (!(obj->flags20C & 1))
            return;
        if (obj->flags216 & 0x20)
            return;
        if ((obj->flags20D & 0x20) && !(obj->flags210 & 1))
        {
            tx = obj->tgtX;
            x = obj->posX;
            y = obj->posY;
            if (x == tx && y == obj->tgtY && obj->posZ == obj->tgtZ && obj->posZB == obj->tgtZB)
                return;
            obj->tile20 = tile_query(obj->world, x, y, &surf, &lo, &hi, (u8)0, (u8)((obj->flags20D >> 3) & 1));
            on = 0;
            if (hi != -1 && obj->posZ + obj->posZB >= hi)
                on = 1;
            ((struct GroundBits *)&obj->ground26)->onTile = on;
            obj->savedX = obj->posX;
            obj->savedY = obj->posY;
            if (((struct GroundBits *)&obj->ground26)->onTile)
            {
                obj->posZB = obj->posZ + obj->posZB - hi;
                obj->posZ = hi;
            }
            else
            {
                obj->posZB = obj->posZ + obj->posZB - surf;
                obj->posZ = surf;
            }
            ((struct GroundBits *)&obj->ground26)->art = 0;
            return;
        }
    }

    x = obj->posX;
    tx = obj->tgtX;
    z = obj->posZ;
    tz = obj->tgtZ;
    tzb = obj->tgtZB;
    if (x != tx || obj->posY != obj->tgtY || z != tz || obj->posZB != tzb || cellStep != 0)
    {
        found = 0;
        bestTile = 0;
        bestC = 0;
        kindA = 0;
        kindC = 0;
        topTgt = tz + tzb;
        topTgtExt = topTgt + obj->sizeZ;
        topCur = z + obj->posZB;

        /* probe 0: the object's position */
        tile = tile_query(obj->world, obj->posX, obj->posY, &outA, &outB, &outC,
                          (u8)(!(obj->flags20D & 0x20) || (obj->flags210 & 1)), (u8)((obj->flags20D >> 3) & 1));
        candA = outA;
        if (tile->kindA > 3)
            candA += 0x1000;
        if (obj->flags210 & 0x40)
        {
            candB = outC;
            if (outC >= 0 && tile->kindC > 3 && tile->kindC <= 6)
                candB += 0x1000;
        }
        else
        {
            outC = -1;
            candB = -1;
        }
        bestA = outA;
        bestAX = obj->posX;
        bestAY = obj->posY;
        bestATile = tile;
        if (bestC < outC)
        {
            bestC = outC;
            bestCX = bestAX;
            bestCY = bestAY;
            bestTile = bestATile;
        }

        /* probe 1: left edge */
        probeX = bestAX - (obj->sizeX >> 1);
        probeY = bestAY;
        tile = tile_query(obj->world, probeX, probeY, &outA, &outB, &outC,
                          (u8)(!(obj->flags20D & 0x20) || (obj->flags210 & 1)), (u8)((obj->flags20D >> 3) & 1));
        if (tile->kindA == 5)
            outA -= obj->sizeX >> 1;
        if (candA < outA && candB <= outA && bestA < outA)
        {
            bestA = outA;
            bestAX = probeX;
            bestAY = probeY;
            bestATile = tile;
            kindA = 1;
            if (tile->kindA > 3)
                candA = bestA + 0x1000;
        }
        if (obj->flags210 & 0x40)
        {
            if (tile->kindC == 5)
                outC -= obj->sizeX >> 1;
            if (candA < outC && candB < outC && bestC < outC)
            {
                bestC = outC;
                bestCX = probeX;
                bestCY = probeY;
                bestTile = tile;
                kindC = 1;
                if (tile->kindC >= 4 && tile->kindC <= 6)
                    candB = bestC + 0x1000;
            }
        }

        /* probe 2: right edge */
        probeX = obj->posX - 0x100 + (obj->sizeX >> 1);
        probeY = obj->posY;
        tile = tile_query(obj->world, probeX, probeY, &outA, &outB, &outC,
                          (u8)(!(obj->flags20D & 0x20) || (obj->flags210 & 1)), (u8)((obj->flags20D >> 3) & 1));
        if (tile->kindA == 4)
            outA = outA + 0x100 - (obj->sizeX >> 1);
        if (candA < outA && candB <= outA && bestA < outA)
        {
            bestA = outA;
            bestAX = probeX;
            bestAY = probeY;
            bestATile = tile;
            kindA = 2;
            if (tile->kindA > 3)
                candA = bestA + 0x1000;
        }
        if (obj->flags210 & 0x40)
        {
            if (tile->kindC == 4)
                outC = outC + 0x100 - (obj->sizeX >> 1);
            if (candA < outC && candB < outC && bestC < outC)
            {
                bestC = outC;
                bestCX = probeX;
                bestCY = probeY;
                bestTile = tile;
                kindC = 2;
                if (tile->kindC >= 4 && tile->kindC <= 6)
                    candB = bestC + 0x1000;
            }
        }

        /* the cell at the back edge */
        cell = tile_ptr_at(obj->world, (obj->posX & 0xFFFFF000) / 0x1000, ((obj->posY + 0x100 - obj->sizeY) & 0xFFFFF000) / 0x1000);

        /* probe 3: back left corner */
        modeA = 0;
        probeX = obj->posX - (obj->sizeX >> 1);
        probeY = obj->posY + 0x100 - obj->sizeY;
        tile = tile_query(obj->world, probeX, probeY, &outA, &outB, &outC,
                          (u8)(!(obj->flags20D & 0x20) || (obj->flags210 & 1)), (u8)((obj->flags20D >> 3) & 1));
        if (tile->mode != 0 && (*(u32 *)tile & 0xF007F) != 0x30043)
            modeA = tile->mode;
        if (tile->kindA == 5)
            outA -= obj->sizeX >> 1;
        else if (tile->kindA == 4)
        {
            outA += obj->sizeX >> 1;
            if (cell->kindA != 4 && cell->kindC != 4)
                outA &= 0xFFFFF000;
        }
        else if (tile->kindA == 6)
            outA = outA + 0x100 - obj->sizeY;
        if (candA < outA && candB <= outA && bestA < outA)
        {
            bestA = outA;
            bestAX = probeX;
            bestAY = probeY;
            bestATile = tile;
            kindA = 3;
        }
        if (obj->flags210 & 0x40)
        {
            if (tile->kindC == 5)
                outC -= obj->sizeX >> 1;
            else if (tile->kindC == 4)
            {
                outC += obj->sizeX >> 1;
                if (cell->kindA != 4 && cell->kindC != 4)
                    outC &= 0xFFFFF000;
            }
            else if (tile->kindC == 6)
                outC = outC + 0x100 - obj->sizeY;
            if (candA < outC && candB < outC && bestC < outC)
            {
                bestC = outC;
                bestCX = probeX;
                bestCY = probeY;
                bestTile = tile;
                kindC = 3;
            }
        }

        /* probe 4: back right corner */
        modeB = 0;
        probeX = obj->posX - 0x100 + (obj->sizeX >> 1);
        probeY = obj->posY + 0x100 - obj->sizeY;
        tile = tile_query(obj->world, probeX, probeY, &outA, &outB, &outC,
                          (u8)(!(obj->flags20D & 0x20) || (obj->flags210 & 1)), (u8)((obj->flags20D >> 3) & 1));
        if (tile->mode != 0 && (*(u32 *)tile & 0xF007F) != 0x30043)
            modeB = tile->mode;
        adj = 0;
        if (tile->kindA == 4)
            outA = outA + 0x100 - (obj->sizeX >> 1);
        else if (tile->kindA == 5)
        {
            outA = outA - 0x100 + (obj->sizeX >> 1);
            if (modeA & modeB)
                adj = 0x1000;
        }
        else if (tile->kindA == 6)
            outA = outA + 0x100 - obj->sizeY;
        else if (modeA & modeB)
            adj = -0x1000;
        if (candA < outA && candB <= outA && bestA < outA + adj)
        {
            bestA = outA;
            bestAX = probeX;
            bestAY = probeY;
            bestATile = tile;
            kindA = 4;
        }
        if (obj->flags210 & 0x40)
        {
            adj = 0;
            if (tile->kindC == 4)
                outC = outC + 0x100 - (obj->sizeX >> 1);
            else if (tile->kindC == 5)
            {
                half = (obj->sizeX >> 1) - 0x100;
                if ((outC & 0xFFF) != 0 && 0x1000 - (outC & 0xFFF) > half)
                {
                    outC += half;
                    if (modeA & modeB)
                        adj = 0x1000;
                }
            }
            else if (tile->kindC == 6)
                outC = outC + 0x100 - obj->sizeY;
            else if (modeA & modeB)
                adj = -0x1000;
            if (candA < outC && candB < outC && bestC < outC + adj)
            {
                bestC = outC;
                bestCX = probeX;
                bestCY = probeY;
                bestTile = tile;
                kindC = 4;
            }
        }

        /* upper limit for the new height */
        topLim = topCur;
        if (topLim < topTgt)
        {
            if (bestC > 0 && topTgtExt > bestC && topLim < bestC)
                topLim = bestC;
            else if (topCur < bestA)
                topLim = bestA;
        }
        topExt = 0;
        if (bestTile->mode & ((struct GroundBits *)&obj->ground26)->art)
            topExt = obj->unk23E;

        edgeOk = 0;
        if (bestC > 0)
        {
            if (bestA < bestC)
            {
                if (bestC <= topLim + topExt)
                    edgeOk = 1;
            }
            else if (bestA > topLim)
                edgeOk = 1;
        }

        if (edgeOk)
        {
            /* stand on the edge height */
            if (bestTile->kindC >= 4 && bestTile->kindC <= 6)
            {
                ((struct GroundBits *)&obj->ground26)->side = bestTile->kindC - 3;
                switch (kindC)
                {
                case 1:
                    if (bestTile->kindC == 4 || bestTile->kindC == 5)
                    {
                        bestC += 0xFFF;
                        bestC &= 0xFFFFF000;
                    }
                    else if (bestTile->kindC == 6)
                    {
                        tile_query(obj->world, obj->posX - (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        bestC = outC;
                    }
                    break;
                case 2:
                    if (bestTile->kindC == 4 || bestTile->kindC == 5)
                    {
                        bestC += 0xFFF;
                        bestC &= 0xFFFFF000;
                    }
                    else if (bestTile->kindC == 6)
                    {
                        tile_query(obj->world, obj->posX - 0x100 + (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        bestC = outC;
                    }
                    break;
                case 3:
                    if (bestTile->kindC == 4 || bestTile->kindC == 5)
                    {
                        tile = tile_query(obj->world, obj->posX, obj->posY + 0x100 - obj->sizeY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        if (((struct Tile *)(*(u32 *)tile & *(u32 *)bestTile))->mode)
                        {
                            if (outA >= bestC - 0x1000 && outA < bestC + 0x1000)
                                bestC = outA;
                            else
                                bestC = outC;
                        }
                        else
                        {
                            if (bestTile->kindC == 5)
                                bestC += 0xFFF;
                            bestC &= 0xFFFFF000;
                        }
                    }
                    else if (bestTile->kindC == 6)
                    {
                        tile = tile_query(obj->world, obj->posX - (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        if (((struct Tile *)(*(u32 *)tile & *(u32 *)bestTile))->mode)
                        {
                            if (outA >= bestC - 0x1000 && outA < bestC + 0x1000)
                                bestC = outA;
                            else
                                bestC = outC;
                        }
                        else
                            bestC &= 0xFFFFF000;
                    }
                    break;
                case 4:
                    if (bestTile->kindC == 4 || bestTile->kindC == 5)
                    {
                        tile = tile_query(obj->world, obj->posX, obj->posY + 0x100 - obj->sizeY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        if (((struct Tile *)(*(u32 *)tile & *(u32 *)bestTile))->mode)
                        {
                            if (outA >= bestC - 0x1000 && outA < bestC + 0x1000)
                                bestC = outA;
                            else
                                bestC = outC;
                        }
                        else
                        {
                            if (bestTile->kindC == 4)
                                bestC += 0xFFF;
                            bestC &= 0xFFFFF000;
                        }
                    }
                    else if (bestTile->kindC == 6)
                    {
                        tile = tile_query(obj->world, obj->posX - 0x100 + (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        if (((struct Tile *)(*(u32 *)tile & *(u32 *)bestTile))->mode)
                        {
                            if (outA >= bestC - 0x1000 && outA < bestC + 0x1000)
                                bestC = outA;
                            else
                                bestC = outC;
                        }
                        else
                            bestC &= 0xFFFFF000;
                    }
                    break;
                }
            }
            else
                ((struct GroundBits *)&obj->ground26)->side = 0;

            zb = topLim - bestC;
            if (cellStep == 0 && obj->posZB == 0 && zb <= obj->unk23E + 0x100
                && ((obj->flags20D & 8) || (obj->tgtZB == 0 && (bestTile->mode & ((struct GroundBits *)&obj->ground26)->art))))
                zb = 0;
            obj->posZB = zb;
            obj->posZ = bestC;
            ((struct GroundBits *)&obj->ground26)->onTile = 1;
            obj->savedX = bestCX;
            obj->savedY = bestCY;
            obj->tile20 = bestTile;
            if ((!((struct ByteBits *)&obj->flags20D)->b5 | ((struct ByteBits *)&obj->flags210)->b0)
                && bestTile->mode != 0 && bestTile->kindA != 3 && bestTile->kindC != 3 && bestTile->shape != 8)
                ((struct GroundBits *)&obj->ground26)->art = bestTile->mode;
            else
                ((struct GroundBits *)&obj->ground26)->art = 0;
        }
        else
        {
            /* stand on the surface height */
            if (bestATile->kindA >= 4 && bestATile->kindA <= 6)
            {
                ((struct GroundBits *)&obj->ground26)->side = bestATile->kindA - 3;
                switch (kindA)
                {
                case 1:
                    if (bestATile->kindA == 4 || bestATile->kindA == 5)
                    {
                        bestA += 0xFFF;
                        bestA &= 0xFFFFF000;
                    }
                    else if (bestATile->kindA == 6)
                    {
                        tile_query(obj->world, obj->posX - (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        bestA = outA;
                    }
                    break;
                case 2:
                    if (bestATile->kindA == 4 || bestATile->kindA == 5)
                    {
                        bestA += 0xFFF;
                        bestA &= 0xFFFFF000;
                    }
                    else if (bestATile->kindA == 6)
                    {
                        tile_query(obj->world, obj->posX - 0x100 + (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        bestA = outA;
                    }
                    break;
                case 3:
                    if (bestATile->kindA == 4 || bestATile->kindA == 5)
                    {
                        tile = tile_query(obj->world, obj->posX, obj->posY + 0x100 - obj->sizeY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        if (((struct Tile *)(*(u32 *)tile & *(u32 *)bestATile))->mode)
                        {
                            if (outA >= bestA - 0x1000 && outA < bestA + 0x1000)
                                bestA = outA;
                            else
                                bestA = outC;
                        }
                        else
                        {
                            if (bestATile->kindA == 5)
                                bestA += 0xFFF;
                            bestA &= 0xFFFFF000;
                        }
                    }
                    else if (bestATile->kindA == 6)
                    {
                        tile_query(obj->world, obj->posX - (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        bestA = outA;
                    }
                    break;
                case 4:
                    if (bestATile->kindA == 4 || bestATile->kindA == 5)
                    {
                        tile = tile_query(obj->world, obj->posX, obj->posY + 0x100 - obj->sizeY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        if (((struct Tile *)(*(u32 *)tile & *(u32 *)bestATile))->mode)
                        {
                            if (outA >= bestA - 0x1000 && outA < bestA + 0x1000)
                                bestA = outA;
                            else
                                bestA = outC;
                        }
                        else
                        {
                            if (bestATile->kindA == 4)
                                bestA += 0xFFF;
                            bestA &= 0xFFFFF000;
                        }
                    }
                    else if (bestATile->kindA == 6)
                    {
                        tile_query(obj->world, obj->posX - 0x100 + (obj->sizeX >> 1), obj->posY, &outA, &outB, &outC, (u8)1, (u8)((obj->flags20D >> 3) & 1));
                        bestA = outA;
                    }
                    break;
                }
            }
            else
                ((struct GroundBits *)&obj->ground26)->side = 0;

            zb = topLim - bestA;
            if (cellStep == 0 && obj->posZB == 0 && zb <= obj->unk23E + 0x100
                && ((obj->flags20D & 8) || (obj->tgtZB == 0 && (bestATile->mode & ((struct GroundBits *)&obj->ground26)->art))))
                zb = 0;
            obj->posZB = zb;
            obj->posZ = bestA;
            if (bestA + zb < 0)
            {
                obj->posZB = 0;
                obj->posZ = 0;
            }
            ((struct GroundBits *)&obj->ground26)->onTile = 0;
            obj->savedX = bestAX;
            obj->savedY = bestAY;
            obj->tile20 = bestATile;
            if ((!((struct ByteBits *)&obj->flags20D)->b5 | ((struct ByteBits *)&obj->flags210)->b0)
                && bestATile->mode != 0 && bestATile->kindA != 3 && bestATile->kindC != 3 && bestATile->shape != 8)
                ((struct GroundBits *)&obj->ground26)->art = bestATile->mode;
            else
                ((struct GroundBits *)&obj->ground26)->art = 0;
        }

        if (obj->counter25C >= 0)
            obj->accum250 += obj->tgtZ - obj->posZ;
        if (found)
            ((struct Word24 *)&obj->w24)->mode = 1;
        else if (obj->posZB == 0)
            ((struct Word24 *)&obj->w24)->mode = 2;
    }

    if (obj->posZB > 0 && obj->counter25C == -1 && (obj->flags20D & 0x40) && obj->link01 == -1 && cellStep == 0)
    {
        if (obj->posZB <= obj->unk23E && ((struct GroundBits *)&obj->ground26)->art)
            obj->posZB = 0;
        else
            obj_init_motion(obj, 0, -1, obj->unk23E);
    }
}
END_NONMATCH

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

struct Lo3
{
    u8 lo3 : 3;
    u8 rest : 5;
};

u32 arc_tan2(s32 x, s32 y);

u32 contacts_obj_start_move(struct MoveObj *obj, s32 kind, s32 relative, s32 x, s32 y, s32 z, u16 speed)
{
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dxp;
    s32 dyp;
    s32 dzp;
    s32 r;
    u16 s;
    s32 p;
    s32 q;
    s32 tmp;
    s32 scale;

    /* target in 8-bit fixed point, optionally relative to the current position */
    x <<= 8;
    y <<= 8;
    z <<= 8;
    if (relative == 1)
    {
        x += obj->posX;
        y += obj->posY;
        z += obj->posZ + obj->posZB;
    }
    obj->savedX = obj->posX;
    obj->savedY = obj->posY;
    obj->savedZ = obj->posZ + obj->posZB;
    obj->snapX = x;
    obj->snapY = y;
    obj->snapZ = z;
    /* already there in whole pixels: nothing to do */
    if (obj->savedX >> 8 == obj->snapX >> 8 && obj->savedY >> 8 == obj->snapY >> 8)
    {
        if ((obj->flags20D & 0x40) != 0 || (obj->savedZ >> 8) == (z >> 8))
            return 0;
    }

    dx = obj->snapX - obj->savedX;
    dy = obj->savedY - obj->snapY;
    if ((obj->flags20D & 0x40) == 0)
        dz = obj->snapZ - obj->savedZ;
    else
        dz = 0;

    dxp = dx / 0x100;
    dyp = dy / 0x100;
    dzp = dz / 0x100;

    /* large distances: work in whole pixels */
    if ((u32)(dx + 0x1000) > 0x2000 || dy < -0x1000 || dy > 0x1000 || dz < -0x1000 || dz > 0x1000)
    {
        dx = dxp;
        dy = dyp;
        dz = dzp;
    }

    r = (u16)arc_tan2((s16)dy, (s16)dx);
    obj->angle = r / 0x100 + ((r / 0x80) & 1);

    if ((obj->flags20D & 0x40) == 0)
    {
        tmp = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
        r = (u16)arc_tan2((s16)tmp, (s16)dz);
        obj->angleZ = r / 0x100 + ((r / 0x80) & 1);
    }

    if (kind == 1)
    {
        /* fixed step per frame: speed argument or the object's speed */
        if (speed == 0)
            s = obj->speed23E;
        else
            s = speed;

        if ((obj->flags20D & 0x40) != 0)
        {
            s32 sv = (s16)s;
            obj->dirX = (gSineTable[obj->angle] * sv) / 0x4000 + (((gSineTable[obj->angle] * sv) / 0x2000) & 1);
            obj->dirY = -((gCosineTable[obj->angle] * sv) / 0x4000 + (((gCosineTable[obj->angle] * sv) / 0x2000) & 1));
            obj->dirZ = 0;
            ((struct Lo3 *)&obj->stepFlags)->lo3 = 4;
        }
        else
        {
            s32 sv = (s16)s;
            tmp = gSineTable[obj->angle] * sv;
            p = tmp / 0x4000 + ((tmp / 0x2000) & 1);
            obj->dirX = (gCosineTable[obj->angleZ] * p) / 0x4000 + (((gCosineTable[obj->angleZ] * p) / 0x2000) & 1);
            tmp = gCosineTable[obj->angle] * sv;
            q = tmp / 0x4000 + ((tmp / 0x2000) & 1);
            obj->dirY = -((gCosineTable[obj->angleZ] * q) / 0x4000 + (((gCosineTable[obj->angleZ] * q) / 0x2000) & 1));
            obj->dirZ = (gSineTable[obj->angleZ] * sv) / 0x4000 + (((gSineTable[obj->angleZ] * sv) / 0x2000) & 1);
            ((struct Lo3 *)&obj->stepFlags)->lo3 = 0;
        }
        obj->moveX = obj->dirX;
        obj->moveY = obj->dirY;
    }
    else
    {
        /* fixed number of frames: step = length / frames */
        obj->modeLimit = speed;
        tmp = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dxp * dxp + dyp * dyp + dzp * dzp);
        tmp = (tmp << 8) / obj->modeLimit;
        scale = (s16)tmp;
        obj->moveX = (scale * gSineTable[obj->angle]) / 0x4000;
        obj->moveY = -((scale * gCosineTable[obj->angle]) / 0x4000);
    }

    obj->modeCount = 0;
    ((struct Lo3 *)&obj->moveFlags)->lo3 = kind;
    return 1;
}
