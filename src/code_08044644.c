#include "global.h"

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct LedgeObj
{
    u8 filler0[0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler18[0x20D - 0x18];
    u8 flags20D;
    u8 filler20E[0x27A - 0x20E];
    u16 sizeB1;
    u8 filler27C[0x290 - 0x27C];
    u8 *world;
};

extern s16 gTableLedge[];
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);

ASM_FUNC("asm/code_08044644/obj_settle_on_tiles.s", void obj_settle_on_tiles(void))

ASM_FUNC("asm/code_08044644/obj_probe_tile_heights.s", void obj_probe_tile_heights(void))

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

ASM_FUNC("asm/code_08044644/obj_step_sequence.s", void obj_step_sequence(void))

ASM_FUNC("asm/code_08044644/contacts_obj_start_move.s", void contacts_obj_start_move(void))
