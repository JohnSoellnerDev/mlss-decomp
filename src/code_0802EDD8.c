#include "global.h"

u8 test_flag(u8 *table, u8 *tableB, u16 id);
extern u32 gUnk_03000FC0;
extern u8 *gModelCtx;
extern u8 gSys[];
extern u16 gUnk_02000282;
extern u16 gUnk_020002A2;
extern const u16 gRomLevelTable[];

struct PairRoutineSlot
{
    s16 thisOffset;
    u16 filler2;
    void (*fn)(void *self, void *entry, s32 count, s32 zero);
};

struct PairSub
{
    u8 filler0[0x0C];
    s32 posX;
    s32 posY;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x20D - 0x1C];
    u8 flags20D;
    u8 filler20E[0x2B5 - 0x20E];
    u8 flags2B5;
    u8 filler2B6[0x338 - 0x2B6];
    u8 *routines;
    u8 filler33C[0x351 - 0x33C];
    u8 flags351;
    u8 mode352;
    u8 flags353;
    u8 filler354[0x368 - 0x354];
    u8 *frames;
    u8 filler36C[0x388 - 0x36C];
    u32 ptr388;
};

struct Bit6Bits
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct Bit1Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct PairObj
{
    u8 filler0[0x28];
    u32 entries[32];
    u8 filler0A8[0xE0 - 0xA8];
    u32 e0;
    u32 e4;
    u8 filler0E8[0xF9 - 0xE8];
    u8 flagF9;
    u8 filler0FA;
    u8 flagFB;
    u8 filler0FC[0x10B - 0xFC];
    u8 entryCount;
    u8 filler10C[2];
    u8 idx10E;
    u8 filler10F;
    u8 idx110;
    u8 idx111;
    u8 filler112[2];
    u8 idx114;
    u8 idx115;
    u8 idx116;
    u8 idx117;
    u8 filler118[2];
    u8 idx11A;
    u8 filler11B[0x12A - 0x11B];
    u8 byte12A;
    u8 filler12B[0x200 - 0x12B];
    struct PairSub *subA;
    struct PairSub *subB;
    u8 flags208;
    u8 flags209;
    u8 filler20A[0x26E - 0x20A];
    u16 half26E;
    u8 filler270[0x286 - 0x270];
    u16 bound286;
    u16 bound288;
    u8 filler28A[0x2B8 - 0x28A];
    u8 flags2B8;
};

ASM_FUNC("asm/code_0802EDD8/try_engage_pair.s", void try_engage_pair(void))

ASM_FUNC("asm/code_0802EDD8/pair_ring_pair_step.s", void pair_ring_pair_step(void))

ASM_FUNC("asm/code_0802EDD8/update_sub_pair.s", void update_sub_pair(void))

NONMATCH("asm/code_0802EDD8/update_sub_bounds.s", void update_sub_bounds(struct PairObj *obj))
{
    struct PairSub *sub;
    s32 x;
    s32 y;
    s32 tx;
    s32 ty;

    sub = obj->subA;
    x = sub->posX;
    if (x < 0)
        x += 0xFF;
    tx = x >> 8;
    y = sub->posY - sub->unk14 - sub->unk18;
    if (y < 0)
        y += 0xFF;
    ty = y >> 8;
    if (tx < 0 || ty < 0 || tx >= obj->bound286 || ty >= obj->bound288)
    {
        obj->subA->flags353 |= 2;
    }
    else
    {
        ((struct Bit6Bits *)&sub->flags20D)->bit6 = (((u32)obj->flagFB << 29) >> 31) ^ 1;
        ((struct Bit1Bits *)&obj->subA->flags353)->bit1 = 0;
    }
    if ((obj->flags208 & 0x10) != 0)
    {
        sub = obj->subB;
        x = sub->posX;
        if (x < 0)
            x += 0xFF;
        tx = x >> 8;
        y = sub->posY - sub->unk14 - obj->subA->unk18;
        if (y < 0)
            y += 0xFF;
        ty = y >> 8;
        if (tx < 0 || ty < 0 || tx >= obj->bound286 || ty >= obj->bound288)
        {
            obj->subB->flags353 |= 2;
        }
        else
        {
            ((struct Bit6Bits *)&sub->flags20D)->bit6 = (((u32)obj->flagFB << 29) >> 31) ^ 1;
            ((struct Bit1Bits *)&obj->subB->flags353)->bit1 = 0;
        }
    }
}
END_NONMATCH

static inline u32 test_flag_level(void)
{
    u32 level;
    if (test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x205) == 0)
    {
        if (test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x204) == 0)
        {
            level = test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x203) != 0;
        }
        else
        {
            level = 2;
        }
    }
    else
    {
        level = 3;
    }
    return level;
}

NONMATCH("asm/code_0802EDD8/apply_test_level.s", void apply_test_level(void))
{
    u32 level;
    s32 count;
    u16 half;
    u32 idx;
    u8 *state;
    u16 *dst;

    level = test_flag_level();
    count = level;
    if (count > 0)
    {
        count = count - 1;
        state = gModelCtx;
        half = *(u16 *)(state + 0xC);
        if ((half & 0x1FF) == 1 && ((half >> 9) & 0xF) == 0)
        {
            idx = ((u32)gSys[0x888] << 29) >> 30;
            dst = &gUnk_02000282;
            *dst = gRomLevelTable[idx * 3 + count];
            *(u16 *)gSys = 1 | *(u16 *)gSys;
        }
        half = *(u16 *)(state + 0xE);
        if ((half & 0x1FF) == 2 && ((half >> 9) & 0xF) == 0)
        {
            idx = ((u32)gSys[0x888] << 29) >> 30;
            dst = &gUnk_020002A2;
            *dst = gRomLevelTable[idx * 3 + count];
            *(u16 *)gSys = 2 | *(u16 *)gSys;
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0802EDD8/obj_pair_tick.s", void obj_pair_tick(void))

ASM_FUNC("asm/code_0802EDD8/pair_kind8_step.s", void pair_kind8_step(void))

ASM_FUNC("asm/code_0802EDD8/update_sub_pair_layers.s", void update_sub_pair_layers(void))

void pair_call_entry_routine(struct PairObj *obj, struct PairSub *sub)
{
    s32 idx;
    u8 *flag;
    u8 *p;
    u8 v;

    flag = &obj->idx10E;
    if (*flag != 0 && (sub->mode352 & 0x80) != 0)
        idx = *flag;
    else
    {
        p = &obj->idx110;
        v = *p;
        idx = 0;
        if (v != 0)
            idx = v;
    }

    if (idx > 0)
    {
        struct PairRoutineSlot *slot = (struct PairRoutineSlot *)(sub->routines + 0x60);
        slot->fn((u8 *)sub + slot->thisOffset, &obj->entries[idx], obj->entryCount - idx, 0);
    }
    p = &obj->idx114;
    if (*p != 0)
        idx = *p;
    else
    {
        p = &obj->idx116;
        if (*p != 0)
            idx = *p;
        else
        {
            p = &obj->idx11A;
            if (*p != 0)
                idx = *p;
            else
                idx = 0;
        }
    }

    if (idx > 0)
    {
        struct PairRoutineSlot *slot = (struct PairRoutineSlot *)(sub->routines + 0x60);
        slot->fn((u8 *)sub + slot->thisOffset, &obj->entries[idx], obj->entryCount - idx, 0);
    }
}

ASM_FUNC("asm/code_0802EDD8/pair_start_state_10.s", void pair_start_state_10(void))
