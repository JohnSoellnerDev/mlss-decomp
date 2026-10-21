#include "global.h"

u8 test_flag(u8 *table, u8 *tableB, u16 id);
extern u32 gUnk_03000FC0;
extern u8 *gModelCtx;
extern u8 gSys[];
extern u16 gUnk_02000282;
extern u16 gUnk_020002A2;
extern const u16 gRomLevelTable[];

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
u32 find_zone_offset(void *sub, u32 e0, void *p1, void *p2, ByteArg flagB, ByteArg stackFlag);
void set_object_mode(void *obj, u32 mode, u32 a, u32 packed, u32 x, u32 y);
s32 obj_compare_depth(void *a, void *b);
u32 obj_flags_2b5_07_set(void *obj);
void advance_obj_motion(void *sub, void *entries);
void obj_advance_motion(void *sub);
void pair_turn_and_sample(void *obj, u32 mode);
void motion_obj_set_move_target(void *obj, u32 value, s32 b, s32 c);
void contacts_obj_start_move(void *obj, s32 a, s32 b, s32 x, s32 y, s32 zero, s32 mode);
void set_slot_value(u32 id, s32 value);
void update_pair_facing(void *obj, u32 a, u32 b);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void obj_try_ride_platform(u32 entry, u32 *list);
void obj_update_entities(void *obj);
void process_obj_entries(void *obj);
void dispatch_sub_state(void *obj);
void obj_update_contacts(void *obj, void *sub, u32 zero);
void pair_b_state_10_1c(void *obj);
void pair_frame4_step(void *obj);
void pair_state_2e_3a(void *obj);
void pair_state_step(void *obj);
void pair_state_a_49_4c(void *obj);
void pair_b_state_step(void *obj);
void pair_ab_step_59_6a(void *obj);
void pair_state_6c_76_step(void *obj);
void pair_state_7a_7f(void *obj);
void pair_b_state_80_8f(void *obj);
void pair_b_state_90_9f_step(void *obj);
void pair_state_a1_b2_step(void *obj);
void obj_shift_state_slots(void *sub);

struct PairRoutineSlot
{
    s16 thisOffset;
    u16 filler2;
    void (*fn)(void *self, void *entry, s32 count, s32 zero);
};

struct PairSub
{
    u8 filler0[4];
    u16 state04;
    u8 filler06[0x0C - 0x06];
    s32 posX;
    s32 posY;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x26 - 0x1C];
    u8 type26;
    u8 filler27[0x34 - 0x27];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x20C - 0x44];
    u8 flags20C;
    u8 flags20D;
    u8 filler20E[0x20F - 0x20E];
    u8 flags20F;
    u8 filler210[0x2B5 - 0x210];
    u8 flags2B5;
    u8 filler2B6[0x338 - 0x2B6];
    u8 *routines;
    u8 filler33C[0x351 - 0x33C];
    u8 flags351;
    u8 mode352;
    u8 flags353;
    u8 filler354[0x358 - 0x354];
    u8 flags358;
    u8 filler359[0x368 - 0x359];
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

struct Bit3Bits
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct Bit4Bits
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bit5Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
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
    u8 filler0FC[0x100 - 0xFC];
    s16 move100;
    u8 filler102[0x10B - 0x102];
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
    u8 flags20A;
    u8 flags20B;
    u8 filler20C[0x246 - 0x20C];
    u8 state246;
    u8 filler247[0x248 - 0x247];
    u16 pixX;
    u16 pixY;
    u8 filler24C[0x26E - 0x24C];
    u16 half26E;
    u8 filler270[0x286 - 0x270];
    u16 bound286;
    u16 bound288;
    u8 filler28A[0x2B8 - 0x28A];
    u8 flags2B8;
};

void update_sub_bounds(struct PairObj *obj);
u8 overlaps_entry_lists(void *obj, void *sub);
void sub_08035988(void *obj);
void sub_08033BAC(void *obj);
void sub_08034FFC(void *obj);
void sub_08034884(void *obj, void *entry);
void pair_sub_state_3a(void *obj, void *sub);
void obj_start_state(void *sub);
void obj_enter_stage3_unlink(void *sub);
void sound_stop_song(u32 song);

u32 try_engage_pair(struct PairObj *obj, struct PairSub *other, u8 engage, u8 flagB, ByteArg stackArg)
{
    u32 ok;
    u8 flags;
    u8 stackFlag;
    u8 *pb;
    stackFlag = stackArg.value;

    if ((u8)(obj->byte12A - 1) <= 1 && (obj->flagF9 & 0x30) == 0 && (other->flags2B5 & 7) == 0)
    {
        ok = find_zone_offset(other, obj->e0, (u8 *)obj + 0x2D0, (u8 *)obj + 0x2D4, flagB, stackFlag);
        obj->e4 = ok;
        if (ok != 0)
        {
            flags = other->flags353;
            flags |= 0x40;
            other->flags353 = flags;
            if ((flags & 0x80) == 0 && engage != 0 && obj->byte12A == 2)
            {
                obj_shift_state_slots(obj->subA);
                obj_shift_state_slots(obj->subB);
                if ((obj->flags208 & 0x10) != 0)
                {
                    if (obj_compare_depth(other, (void *)other->ptr388) != 0)
                    {
                        obj->flags2B8 |= 0x20;
                        obj->half26E = 0x3C;
                    }
                }

                set_object_mode(obj, 2, 0x10, 0, 0xFFFF, 0xFFFF);
                obj->subA->flags351 |= 0x40;
                pb = &obj->subB->flags351;
                *pb |= 0x40;
                obj->byte12A = 7;
                return 1;
            }
        }
    }
    return 0;
}

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

NONMATCH("asm/code_0802EDD8/obj_pair_tick.s", void obj_pair_tick(struct PairObj *obj))
{
    s32 i;

    if (obj->byte12A != 0)
    {
        for (i = 2; i < obj->entryCount; i++)
            obj_try_ride_platform(obj->entries[i], obj->entries);
        obj_update_entities(obj);
        process_obj_entries(obj);
        obj_try_ride_platform(obj->entries[0], obj->entries);
        obj_try_ride_platform(obj->entries[1], obj->entries);
        if ((obj->flags209 & 0x20) != 0)
            dispatch_sub_state(obj);
        else
        {
            switch (*obj->subA->frames - 3)
            {
            case 0: pair_b_state_10_1c(obj); break;
            case 1: pair_frame4_step(obj); break;
            case 2: pair_state_2e_3a(obj); break;
            case 3:
            case 4: pair_state_step(obj); break;
            case 6: pair_state_a_49_4c(obj); break;
            case 7: pair_b_state_step(obj); break;
            case 8: pair_ab_step_59_6a(obj); break;
            case 9: pair_state_6c_76_step(obj); break;
            case 10: pair_state_7a_7f(obj); break;
            case 11:
            case 12: pair_b_state_80_8f(obj); break;
            case 13:
            case 14: pair_b_state_90_9f_step(obj); break;
            case 15:
            case 16: pair_state_a1_b2_step(obj); break;
            }
        }
        if (obj->byte12A <= 2 && (obj->flagF9 & 0x30) == 0)
        {
            obj_update_contacts(obj, obj->subA, 0);
            obj_update_contacts(obj, obj->subB, 0);
        }
        else
        {
            obj_shift_state_slots(obj->subA);
            obj_shift_state_slots(obj->subB);
        }
    }
}
END_NONMATCH

NONMATCH("asm/code_0802EDD8/pair_kind8_step.s", void pair_kind8_step(struct PairObj *obj, u32 mode, u32 arg))
{
    u8 *pa;
    u8 *pb;

    if (mode == 1)
    {
        if ((obj->state246 & 0x1C) != 8)
            return;
        pa = &obj->flags20A;
        *pa |= 2;
        obj->subA->flags351 |= 0x40;
        pb = &obj->subB->flags351;
        *pb |= 0x40;
        if ((s16)obj->subB->state04 > 100)
            return;
        if (arg != 0 && overlaps_entry_lists(obj, obj->subB))
        {
            obj->subB->posX = obj->subA->posX;
            obj->subB->posY = obj->subA->posY;
            obj->subB->unk14 = obj->subA->unk14;
            obj->subB->unk18 = obj->subA->unk18;
            obj->subB->unk34 = obj->subA->unk34;
            obj->subB->unk38 = obj->subA->unk38;
            obj->subB->unk3C = obj->subA->unk3C;
            obj->subB->unk40 = obj->subA->unk40;
        }
        sub_08035988(obj);
        return;
    }
    if ((obj->state246 & 0x1C) == 8)
        return;
    if (*obj->subA->frames <= 2)
        return;
    pa = &obj->flags20A;
    *pa |= 2;
    obj->subA->flags351 |= 0x40;
    pb = &obj->subB->flags351;
    *pb |= 0x40;
    switch ((((u32)obj->state246) << 27) >> 29)
    {
    case 0:
    case 2:
        break;
    case 1:
        if ((obj->subA->flags358 & 8) == 0)
            pair_sub_state_3a(obj, obj->subA);
        break;
    case 3:
        if (arg != 0 && overlaps_entry_lists(obj, obj->subB))
        {
            obj->subB->posX = obj->subA->posX;
            obj->subB->posY = obj->subA->posY;
            obj->subB->unk14 = obj->subA->unk14;
            obj->subB->unk18 = obj->subA->unk18;
            obj->subB->unk34 = obj->subA->unk34;
            obj->subB->unk38 = obj->subA->unk38;
            obj->subB->unk3C = obj->subA->unk3C;
            obj->subB->unk40 = obj->subA->unk40;
        }
        {
            u16 st = ((struct PairSub *)obj->entries[1])->state04;
            if (!((u16)(st - 0x40) <= 5 || (u16)(st - 0x3B) <= 2))
                sub_08033BAC(obj);
        }
        if ((((struct PairSub *)obj->entries[0])->flags20C & 1) != 0)
        {
            u16 st0 = ((struct PairSub *)obj->entries[0])->state04;
            if ((u16)(st0 - 5) <= 4)
                obj_start_state((void *)obj->entries[0]);
            else if ((u16)(st0 - 0xA) <= 2)
                obj_enter_stage3_unlink((void *)obj->entries[0]);
        }
        break;
    case 4:
        if ((u16)(obj->subA->state04 - 0x49) > 2)
        {
            if (*obj->subA->frames == 0xA)
                sub_08034FFC(obj);
            else
                sub_08034884(obj, (void *)obj->entries[1]);
        }
        break;
    case 5:
    case 6:
        sound_stop_song(0x157);
        sound_stop_song(0x87);
        sound_stop_song(0xDB);
        sound_stop_song(0xDA);
        sound_stop_song(0xDA);
        break;
    }
}
END_NONMATCH

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
