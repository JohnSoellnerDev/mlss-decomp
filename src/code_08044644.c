#include "global.h"
#include "ram_routines.h"

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct SubObj
{
    u8 filler0[0x12];
    u8 bits12;
};

struct SubBits12
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

/* The u16 at LedgeObj.seqLo212: dir is the link sequence's state (0 off, 1 backward, else forward). */
struct SeqBits
{
    u16 lo : 7;
    u16 dir : 2;
    u16 rest : 7;
};

#define SEQ_DIR_MASK 0x180     /* SeqBits.dir as a mask of the u16 */
#define SEQ_DIR_BACKWARD 0x080 /* SeqBits.dir == 1 */
#define OBJ_FLAG20F_LINKED 0x02 /* obj_set_sprite_link's current state */
#define SEQ_END 0xFF           /* seqTimer value that ends the sequence */

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

/* Steps the obj's timed link sequence while SeqBits.dir is set: seqPtr points at
 * {unlinked frames, linked frames} pairs. When the timer runs out it toggles obj_set_sprite_link,
 * taking the next pair's first byte (stepping backward or forward) after unlinking, or the pair's
 * second byte after linking. A timer of 0xFF ends the sequence: the link is left on for a
 * backward sequence, dir is cleared and the sub's mode is restored from change213's top bits. */
void obj_step_sequence(struct LedgeObj *obj)
{
    u16 *seqFlags;
    u8 *timer;
    s32 link;
    u8 *cur;
    u8 *next;

    seqFlags = (u16 *)&obj->seqLo212;
    if (*seqFlags & SEQ_DIR_MASK)
    {
        timer = &obj->seqTimer;
        *timer = *timer - 1;
        if (*timer == 0)
        {
            if (obj->flags20F & OBJ_FLAG20F_LINKED)
            {
                obj_set_sprite_link(obj, 0);
                cur = obj->seqPtr;
                if ((*seqFlags & SEQ_DIR_MASK) == SEQ_DIR_BACKWARD)
                    next = cur - 2;
                else
                    next = cur + 2;
                obj->seqPtr = next;
                *timer = *next;
            }
            else
            {
                obj_set_sprite_link(obj, 1);
                *timer = obj->seqPtr[1];
            }
        }
        if (*(s8 *)&obj->seqTimer == (s8)SEQ_END)
        {
            link = 0;
            if (((struct SeqBits *)&obj->seqLo212)->dir == 1)
                link = 1;
            obj_set_sprite_link(obj, link);
            ((struct SeqBits *)&obj->seqLo212)->dir = 0;
            ((struct SubBits12 *)&obj->sub->bits12)->mode = obj->change213 >> 6;
        }
    }
}

/* An object moved in a straight line by contacts_obj_start_move. Positions are 24.8 fixed point. */
struct MoveObj
{
    u8 filler0[0x02];
    u8 heading;         /* direction in the ground plane, 0-255 */
    u8 pitch;           /* climb angle, 0-255 */
    u8 filler4[0x0C - 0x04];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 posZB;          /* height on top of posZ */
    u8 filler1C[0x20D - 0x1C];
    u8 flags20D;        /* MOVEOBJ_FLAT, ... */
    u8 filler20E[0x23E - 0x20E];
    u16 moveSpeed;      /* default step length of a MOVE_BY_SPEED move */
    u8 filler240[0x242 - 0x240];
    s16 stepX;          /* per-frame step in the ground plane */
    s16 stepY;
    u8 filler246[0x2B5 - 0x246];
    u8 moveFlags;       /* low 3 bits: enum MoveKind of the current move */
    u8 filler2B6;
    u8 stepFlags;       /* low 3 bits: 4 = flat move, 0 = 3D move */
    u8 filler2B8[0x2BC - 0x2B8];
    s32 startX;         /* where the move started */
    s32 startY;
    s32 startZ;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 targetX;        /* where the move ends */
    s32 targetY;
    s32 targetZ;
    s16 velX;           /* per-frame step in 3D (MOVE_BY_SPEED) */
    s16 velY;
    s16 velZ;
    u16 moveTimer;      /* frames since the move started */
    u16 moveFrames;     /* length of a MOVE_BY_FRAMES move */
};

/* flags20D: the object moves on the ground plane only, its height is left alone */
#define MOVEOBJ_FLAT 0x40

enum MoveKind
{
    MOVE_BY_SPEED = 1,  /* a fixed step every frame */
    MOVE_BY_FRAMES = 2, /* arrive after a fixed number of frames */
};

/* v / d rounded to nearest (d a power of two) */
#define DIV_ROUND(v, d) ((v) / (d) + (((v) / ((d) / 2)) & 1))

struct Lo3
{
    u8 lo3 : 3;
    u8 rest : 5;
};

u32 arc_tan2(s32 y, s32 x);

/*
 * Starts moving obj in a straight line to pixel (x, y, z), or by (x, y, z) when relative == 1.
 * MOVE_BY_SPEED steps speedOrFrames (obj->moveSpeed when 0) every frame; MOVE_BY_FRAMES arrives
 * after speedOrFrames frames. Returns 0 when obj is already there, 1 when the move has started.
 */
u32 contacts_obj_start_move(struct MoveObj *obj, s32 kind, s32 relative, s32 x, s32 y, s32 z, u16 speedOrFrames)
{
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dxPixels;
    s32 dyPixels;
    s32 dzPixels;
    s32 angle;
    u16 speed;
    s32 flatX;
    s32 flatY;
    s32 tmp;
    s32 stepLen;

    x <<= 8;
    y <<= 8;
    z <<= 8;
    if (relative == 1)
    {
        x += obj->posX;
        y += obj->posY;
        z += obj->posZ + obj->posZB;
    }
    obj->startX = obj->posX;
    obj->startY = obj->posY;
    obj->startZ = obj->posZ + obj->posZB;
    obj->targetX = x;
    obj->targetY = y;
    obj->targetZ = z;
    /* already there in whole pixels: nothing to do */
    if (obj->startX >> 8 == obj->targetX >> 8 && obj->startY >> 8 == obj->targetY >> 8)
    {
        if ((obj->flags20D & MOVEOBJ_FLAT) != 0 || (obj->startZ >> 8) == (z >> 8))
            return 0;
    }

    dx = obj->targetX - obj->startX;
    dy = obj->startY - obj->targetY;
    if ((obj->flags20D & MOVEOBJ_FLAT) == 0)
        dz = obj->targetZ - obj->startZ;
    else
        dz = 0;

    dxPixels = dx / 0x100;
    dyPixels = dy / 0x100;
    dzPixels = dz / 0x100;

    /* beyond 16 pixels the squares would overflow: work in whole pixels */
    if ((u32)(dx + 0x1000) > 0x2000 || dy < -0x1000 || dy > 0x1000 || dz < -0x1000 || dz > 0x1000)
    {
        dx = dxPixels;
        dy = dyPixels;
        dz = dzPixels;
    }

    angle = (u16)arc_tan2((s16)dy, (s16)dx);
    obj->heading = DIV_ROUND(angle, 0x100);

    if ((obj->flags20D & MOVEOBJ_FLAT) == 0)
    {
        tmp = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
        angle = (u16)arc_tan2((s16)tmp, (s16)dz);
        obj->pitch = DIV_ROUND(angle, 0x100);
    }

    if (kind == MOVE_BY_SPEED)
    {
        if (speedOrFrames == 0)
            speed = obj->moveSpeed;
        else
            speed = speedOrFrames;

        if ((obj->flags20D & MOVEOBJ_FLAT) != 0)
        {
            s32 sv = (s16)speed;
            obj->velX = DIV_ROUND(gSineTable[obj->heading] * sv, 0x4000);
            obj->velY = -DIV_ROUND(gCosineTable[obj->heading] * sv, 0x4000);
            obj->velZ = 0;
            ((struct Lo3 *)&obj->stepFlags)->lo3 = 4;
        }
        else
        {
            /* split the speed into ground and height parts by the pitch */
            s32 sv = (s16)speed;
            tmp = gSineTable[obj->heading] * sv;
            flatX = DIV_ROUND(tmp, 0x4000);
            obj->velX = DIV_ROUND(gCosineTable[obj->pitch] * flatX, 0x4000);
            tmp = gCosineTable[obj->heading] * sv;
            flatY = DIV_ROUND(tmp, 0x4000);
            obj->velY = -DIV_ROUND(gCosineTable[obj->pitch] * flatY, 0x4000);
            obj->velZ = DIV_ROUND(gSineTable[obj->pitch] * sv, 0x4000);
            ((struct Lo3 *)&obj->stepFlags)->lo3 = 0;
        }
        obj->stepX = obj->velX;
        obj->stepY = obj->velY;
    }
    else
    {
        /* step = distance / frames */
        obj->moveFrames = speedOrFrames;
        tmp = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dxPixels * dxPixels + dyPixels * dyPixels + dzPixels * dzPixels);
        tmp = (tmp << 8) / obj->moveFrames;
        stepLen = (s16)tmp;
        obj->stepX = (stepLen * gSineTable[obj->heading]) / 0x4000;
        obj->stepY = -((stepLen * gCosineTable[obj->heading]) / 0x4000);
    }

    obj->moveTimer = 0;
    ((struct Lo3 *)&obj->moveFlags)->lo3 = kind;
    return 1;
}
