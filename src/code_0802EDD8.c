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
    s32 unk14;      // height (posZ, 8.8 fixed point)
    s32 unk18;      // height above that (posZB: jump/offset)
    u8 filler1C[0x26 - 0x1C];
    u8 type26;      // bits 4-5: collision layers
    u8 filler27[0x34 - 0x27];
    s32 unk34;      // saved X
    s32 unk38;      // saved Y
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

struct Sub20DBits
{
    u8 filler0[0x20D];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct Bit6Bits
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct Bit2Bits
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Half20A
{
    u16 lo7 : 7;
    u16 b78 : 2;
    u16 rest : 7;
};

struct Bit7Bits
{
    u8 lo7 : 7;
    u8 bit7 : 1;
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
    u8 state246;        // bits 2-4: the pair's current action (0-6, see pair_kind8_step)
    u8 filler247[0x248 - 0x247];
    u16 pixX;
    u16 pixY;
    u8 filler24C[0x26E - 0x24C];
    u16 half26E;
    u8 filler270[0x286 - 0x270];
    u16 mapWidth;       // pixels
    u16 mapHeight;
    u8 filler28A[0x2B8 - 0x28A];
    u8 flags2B8;
};

void update_sub_bounds(struct PairObj *obj);
u8 overlaps_entry_lists(void *obj, void *sub);
void sub_08035988(void *obj);
void sub_08033BAC(void *obj, void *entry);
void sub_08034FFC(void *obj);
void sub_08034884(void *obj, void *entry);
void pair_sub_state_3a(void *obj, void *sub);
void obj_start_state(void *sub);
void obj_enter_stage3_unlink(void *sub);
void sound_stop_song(u32 song);

/*
 * While the pair is in mode 1 or 2 (byte12A), not busy (flagF9 bits 4-5) and `other` isn't moving
 * (flags2B5 bits 0-2), looks up other's zone (kept in e4) and marks other as touched (flags353 bit 6).
 * If other isn't locked (flags353 bit 7), `engage` is set and the pair is in mode 2, the pair
 * engages: both subs shift their state slots, B is held back 60 frames (flags2B8 bit 5) when it is
 * along and other lies in front, the object enters mode 2 (step 0x10), both subs' flags351 bit 6
 * are set and the pair goes to mode 7. Returns 1 when it engaged, else 0.
 */
u32 try_engage_pair(struct PairObj *obj, struct PairSub *other, u8 engage, u8 zoneFlag, ByteArg zoneFlag2Arg)
{
    u32 zone;
    u8 otherFlags;
    u8 zoneFlag2;
    u8 *flagsB;
    zoneFlag2 = zoneFlag2Arg.value;

    if ((u8)(obj->byte12A - 1) <= 1 && (obj->flagF9 & 0x30) == 0 && (other->flags2B5 & 7) == 0)
    {
        zone = find_zone_offset(other, obj->e0, (u8 *)obj + 0x2D0, (u8 *)obj + 0x2D4, zoneFlag, zoneFlag2);
        obj->e4 = zone;
        if (zone != 0)
        {
            otherFlags = other->flags353;
            otherFlags |= 0x40;
            other->flags353 = otherFlags;
            if ((otherFlags & 0x80) == 0 && engage != 0 && obj->byte12A == 2)
            {
                obj_shift_state_slots(obj->subA);
                obj_shift_state_slots(obj->subB);
                if ((obj->flags208 & 0x10) != 0)
                {
                    if (obj_compare_depth(other, (void *)other->ptr388) != 0)
                    {
                        obj->flags2B8 |= 0x20;
                        obj->half26E = 60;
                    }
                }

                set_object_mode(obj, 2, 0x10, 0, 0xFFFF, 0xFFFF);
                obj->subA->flags351 |= 0x40;
                flagsB = &obj->subB->flags351;
                *flagsB |= 0x40;
                obj->byte12A = 7;
                return 1;
            }
        }
    }
    return 0;
}

ASM_FUNC("asm/code_0802EDD8/pair_ring_pair_step.s", void pair_ring_pair_step(void))

/* Per-frame motion of a moving pair (flags2B8 bit 4): advances sub A along the pair's entries,
   and sub B too while it has one (flags208 bit 4) and B isn't held back (flags2B8 bit 5, which
   half26E counts down). Bit 4 stays set while the leading sub is still busy. When the motion
   ends, each sub's pending offsets (unk18, unk40) are folded into unk14/unk3C unless flagFB
   bit 2 keeps them, its flags are reset, and the pair turns to face (pair_turn_and_sample);
   otherwise the subs' screen bounds are checked. */
void update_sub_pair(struct PairObj *obj)
{
    u8 keepOffsets;

    if ((obj->flags2B8 & 0x10) == 0)
        return;
    ((struct Bit3Bits *)&obj->subA->flags20D)->bit3 = 0;
    advance_obj_motion(obj->subA, obj->entries);
    obj_advance_motion(obj->subA);

    if (!(obj->flags2B8 & 0x20) && (obj->flags208 & 0x10))
    {
        ((struct Bit3Bits *)&obj->subB->flags20D)->bit3 = 0;
        advance_obj_motion(obj->subB, obj->entries);
        obj_advance_motion(obj->subB);
    }

    /* still moving while the leading sub is busy */
    if (obj->flags208 & 0x10)
    {
        ((struct Bit4Bits *)&obj->flags2B8)->bit4 = obj_flags_2b5_07_set(obj->subB) % 2;
        if (obj->flags2B8 & 0x20)
        {
            obj->half26E--;
            if (obj->half26E == 0)
                ((struct Bit5Bits *)&obj->flags2B8)->bit5 = 0;
        }
    }
    else
    {
        ((struct Bit4Bits *)&obj->flags2B8)->bit4 = obj_flags_2b5_07_set(obj->subA) % 2;
    }

    if ((obj->flags2B8 & 0x10) == 0)
    {
        /* motion done: settle sub A */
        keepOffsets = obj->flagFB & 4;
        if (keepOffsets == 0)
        {
            obj->subA->unk14 += obj->subA->unk18;
            obj->subA->unk18 = keepOffsets;
            obj->subA->unk3C += obj->subA->unk40;
            obj->subA->unk40 = keepOffsets;
        }
        ((struct Bit1Bits *)&obj->subA->flags353)->bit1 = 0;
        ((struct Bit3Bits *)&obj->subA->flags20D)->bit3 = 1;
        ((struct Bit6Bits *)&obj->subA->flags20D)->bit6 = 1;
        if (obj->flags208 & 0x10)
        {
            /* and sub B */
            keepOffsets = obj->flagFB & 4;
            if (keepOffsets == 0)
            {
                obj->subB->unk14 += obj->subB->unk18;
                obj->subB->unk18 = keepOffsets;
                obj->subB->unk3C += obj->subB->unk40;
                obj->subB->unk40 = keepOffsets;
            }
            ((struct Bit1Bits *)&obj->subB->flags353)->bit1 = 0;
            ((struct Bit3Bits *)&obj->subB->flags20D)->bit3 = 1;
            ((struct Bit6Bits *)&obj->subB->flags20D)->bit6 = 1;
            pair_turn_and_sample(obj, 0);
        }
        {
            struct PairSub *subB = obj->subB;
            struct PairSub *subA = obj->subA;
            ((struct Bit2Bits *)&subA->flags20F)->bit2 = 1;
            ((struct Bit2Bits *)&subB->flags20F)->bit2 = 1;
        }
        ((struct Half20A *)&obj->flags20A)->b78 = 0;
    }
    else
    {
        update_sub_bounds(obj);
    }
}

/* Checks whether each bro's ground point (A always, B while flags208 bit 4 is set) lies inside the
   map, in pixels (8.8 position minus height). Off the map sets the sub's flags353 bit 1; on the map
   clears it and sets flags20D bit 6 unless the pair's flagFB bit 2 is set. B's height uses A's
   pending offset (unk18). */
void update_sub_bounds(struct PairObj *obj)
{
    struct PairSub *sub;
    s32 x;
    s32 y;
    s32 pixelX;
    s32 pixelY;

    sub = obj->subA;
    x = sub->posX;
    if (x < 0)
        x += 0xFF;
    pixelX = x >> 8;
    y = sub->posY - sub->unk14 - sub->unk18;
    if (y < 0)
        y += 0xFF;
    pixelY = y >> 8;
    if (pixelX < 0 || pixelY < 0 || pixelX >= obj->mapWidth || pixelY >= obj->mapHeight)
    {
        ((struct Bit1Bits *)&obj->subA->flags353)->bit1 = 1;
    }
    else
    {
        ((struct Sub20DBits *)sub)->bit6 = (((u32)obj->flagFB << 29) >> 31) ^ 1;
        ((struct Bit1Bits *)&obj->subA->flags353)->bit1 = 0;
    }
    if ((obj->flags208 & 0x10) != 0)
    {
        sub = obj->subB;
        x = sub->posX;
        if (x < 0)
            x += 0xFF;
        pixelX = x >> 8;
        y = sub->posY - sub->unk14 - obj->subA->unk18;
        if (y < 0)
            y += 0xFF;
        pixelY = y >> 8;
        if (pixelX < 0 || pixelY < 0 || pixelX >= obj->mapWidth || pixelY >= obj->mapHeight)
        {
            ((struct Bit1Bits *)&obj->subB->flags353)->bit1 = 1;
        }
        else
        {
            ((struct Sub20DBits *)sub)->bit6 = (((u32)obj->flagFB << 29) >> 31) ^ 1;
            ((struct Bit1Bits *)&obj->subB->flags353)->bit1 = 0;
        }
    }
}

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

/* Reads the level set by story flags 0x203-0x205 (1-3, 0 if none). For a level, when the entry at
 * gModelCtx + 0xC is kind 1 (and + 0xE kind 2) with sub-value 0, sets colour 1 of OBJ palette 0
 * (palette 1) in the palette shadow from gRomLevelTable[link mode * 3 + level - 1] and marks that
 * OBJ palette for upload (bit 0 / bit 1 of the first halfword of gSys). The link mode is bits 1-2
 * of gSys + 0x888. */
void apply_test_level(void)
{
    u32 level;
    s32 index;
    u16 entry;
    u8 *ctx;
    s32 pal0Bit;
    s32 pal1Bit;

    pal0Bit = 1;
    pal1Bit = 2;
    level = test_flag_level();
    index = level;
    if (index > 0)
    {
        index = index - 1;
        ctx = gModelCtx;
        entry = *(u16 *)(ctx + 0xC);
        if ((entry & 0x1FF) == 1 && ((entry >> 9) & 0xF) == 0)
        {
            u16 *color = &gUnk_02000282; /* OBJ palette 0, colour 1 */
            *color = gRomLevelTable[((((u32)gSys[0x888] << 29) >> 30) * 3) + index];
            *(u16 *)gSys = pal0Bit | *(u16 *)gSys;
        }
        entry = *(u16 *)(ctx + 0xE);
        if ((entry & 0x1FF) == 2 && ((entry >> 9) & 0xF) == 0)
        {
            u16 *color = &gUnk_020002A2; /* OBJ palette 1, colour 1 */
            *color = gRomLevelTable[((((u32)gSys[0x888] << 29) >> 30) * 3) + index];
            *(u16 *)gSys = pal1Bit | *(u16 *)gSys;
        }
    }
}

/* Per-frame update of the hero pair: the other entries (2 and up) and then the two heroes
 * (entries 0 and 1) get a chance to ride a platform, then the step for hero A's current
 * action (subA->frames[0]) runs, and the contacts of both heroes are updated. */
void obj_pair_tick(struct PairObj *obj)
{
    s32 i;

    if (obj->byte12A != 0)
    {
        for (i = 2; i < obj->entryCount; i++)
            obj_try_ride_platform(obj->entries[i], obj->entries);
        obj_update_entities(obj);
        process_obj_entries(obj);
        for (i = 0; i < 2; i++)
            obj_try_ride_platform(obj->entries[i], obj->entries);
        if ((obj->flags209 & 0x20) != 0)
            dispatch_sub_state(obj);
        else
        {
            switch (*obj->subA->frames)
            {
            case 3: pair_b_state_10_1c(obj); break;
            case 4: pair_frame4_step(obj); break;
            case 5: pair_state_2e_3a(obj); break;
            case 6:
            case 7: pair_state_step(obj); break;
            case 9: pair_state_a_49_4c(obj); break;
            case 10: pair_b_state_step(obj); break;
            case 11: pair_ab_step_59_6a(obj); break;
            case 12: pair_state_6c_76_step(obj); break;
            case 13: pair_state_7a_7f(obj); break;
            case 14:
            case 15: pair_b_state_80_8f(obj); break;
            case 16:
            case 17: pair_b_state_90_9f_step(obj); break;
            case 18:
            case 19: pair_state_a1_b2_step(obj); break;
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

/* Step of the bros pair for one action (state246 bits 2-4). It marks the pair (flags20A bit 1)
   and both bros (flags351 bit 6) as busy, then:
   - phase 1, while action 2 runs: unless subB's state is above 100, snaps subB onto subA when
     `snapStuck` is set and subB is stuck in a wall or another object, then calls sub_08035988;
   - any other phase, once subA's frame number is above 2 and the action isn't 2: runs the action
     (1: subA's state 3A, 3: snap subB as above and restart the bros' states, 4: subA's step,
     5/6: stop the action's sounds). */
void pair_kind8_step(struct PairObj *pair, u32 phase, u32 snapStuck)
{
    u8 *pairFlags;

    if (phase == 1)
    {
        if ((pair->state246 & 0x1C) != 8)
            return;
        pairFlags = &pair->flags20A;
        *pairFlags |= 2;
        pair->subA->flags351 |= 0x40;
        { u8 *subFlags = &pair->subB->flags351; *subFlags |= 0x40; }
        if ((s16)pair->subB->state04 > 100)
            return;
        if (snapStuck != 0 && overlaps_entry_lists(pair, pair->subB))
        {
            pair->subB->posX = pair->subA->posX;
            pair->subB->posY = pair->subA->posY;
            pair->subB->unk14 = pair->subA->unk14;
            pair->subB->unk18 = pair->subA->unk18;
            pair->subB->unk34 = pair->subA->unk34;
            pair->subB->unk38 = pair->subA->unk38;
            pair->subB->unk3C = pair->subA->unk3C;
            pair->subB->unk40 = pair->subA->unk40;
        }
        sub_08035988(pair);
        return;
    }
    if ((pair->state246 & 0x1C) == 8)
        return;
    if (*pair->subA->frames <= 2)
        return;
    pairFlags = &pair->flags20A;
    *pairFlags |= 2;
    pair->subA->flags351 |= 0x40;
    { u8 *subFlags = &pair->subB->flags351; *subFlags |= 0x40; }
    switch ((((u32)pair->state246) << 27) >> 29)
    {
    case 0:
    case 2:
        break;
    case 1:
        if ((pair->subA->flags358 & 8) == 0)
            pair_sub_state_3a(pair, pair->subA);
        break;
    case 3:
        if (snapStuck != 0 && overlaps_entry_lists(pair, pair->subB))
        {
            pair->subB->posX = pair->subA->posX;
            pair->subB->posY = pair->subA->posY;
            pair->subB->unk14 = pair->subA->unk14;
            pair->subB->unk18 = pair->subA->unk18;
            pair->subB->unk34 = pair->subA->unk34;
            pair->subB->unk38 = pair->subA->unk38;
            pair->subB->unk3C = pair->subA->unk3C;
            pair->subB->unk40 = pair->subA->unk40;
        }
        {
            struct PairSub *second = (struct PairSub *)pair->entries[1];
            u16 state = second->state04;
            if (!((u16)(state - 0x40) <= 5 || (u16)(state - 0x3B) <= 2))
                sub_08033BAC(pair, second);
        }
        if ((((struct PairSub *)pair->entries[0])->flags20C & 1) != 0)
        {
            u16 firstState = ((struct PairSub *)pair->entries[0])->state04;
            if ((u16)(firstState - 5) <= 4)
                obj_start_state((void *)pair->entries[0]);
            else if ((u16)(firstState - 0xA) <= 2)
                obj_enter_stage3_unlink((void *)pair->entries[0]);
        }
        break;
    case 4:
        if ((u16)(pair->subA->state04 - 0x49) > 2)
        {
            if (*pair->subA->frames == 0xA)
                sub_08034FFC(pair);
            else
                sub_08034884(pair, (void *)pair->entries[1]);
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

ASM_FUNC("asm/code_0802EDD8/update_sub_pair_layers.s", void update_sub_pair_layers(void))

/* Calls sub's method at routine slot 0x60 on the tail of the pair's entry list, entries[start..]
   (entryCount - start of them), for up to two start indices. The first is idx10E when it's set and
   sub's mode352 bit 7 is on, else idx110; the second is the first set one of idx114, idx116 and
   idx11A. A start of 0 means no call. */
void pair_call_entry_routine(struct PairObj *obj, struct PairSub *sub)
{
    s32 start;
    u8 *first;
    u8 *index;
    u8 value;

    first = &obj->idx10E;
    if (*first != 0 && (sub->mode352 & 0x80) != 0)
        start = *first;
    else
    {
        index = &obj->idx110;
        value = *index;
        start = 0;
        if (value != 0)
            start = value;
    }

    if (start > 0)
    {
        struct PairRoutineSlot *method = (struct PairRoutineSlot *)(sub->routines + 0x60);
        method->fn((u8 *)sub + method->thisOffset, &obj->entries[start], obj->entryCount - start, 0);
    }
    index = &obj->idx114;
    if (*index != 0)
        start = *index;
    else
    {
        index = &obj->idx116;
        if (*index != 0)
            start = *index;
        else
        {
            index = &obj->idx11A;
            if (*index != 0)
                start = *index;
            else
                start = 0;
        }
    }

    if (start > 0)
    {
        struct PairRoutineSlot *method = (struct PairRoutineSlot *)(sub->routines + 0x60);
        method->fn((u8 *)sub + method->thisOffset, &obj->entries[start], obj->entryCount - start, 0);
    }
}

/* Common start of the pair state starts: facing, slots, animation. */
static inline void pair_start_begin(struct PairObj *obj)
{
    update_pair_facing(obj, 1, 1);
    obj_shift_state_slots(obj->subA);
    advance_anim_frame(obj, obj->subA, 0, 0);
    ((struct Bit1Bits *)&obj->state246)->bit1 = 0;
}

// Pair state 0x10: B jumps onto A. B's position is snapped to whole pixels (or onto A
// when within 4 pixels), both enter state 0x10, and B starts a jump to A's position high
// enough to clear the difference in height.
void pair_start_state_10(struct PairObj *obj)
{
    s32 maxHeight;
    s32 jumpHeight;

    pair_start_begin(obj);
    obj->pixX = obj->subB->posX / 0x100;
    obj->pixY = obj->subB->posY / 0x100;
    obj->subB->posX &= ~0xFF;
    obj->subB->posY &= ~0xFF;
    obj->subB->unk34 = obj->subB->posX;
    obj->subB->unk38 = obj->subB->posY;
    /* B snaps onto A when it is within 4 pixels. */
    if (obj->subB->posX >= obj->subA->posX - 0x400 && obj->subB->posX <= obj->subA->posX + 0x400
        && obj->subB->posY >= obj->subA->posY - 0x400 && obj->subB->posY <= obj->subA->posY + 0x400)
    {
        obj->subB->posX = obj->subA->posX;
        obj->subB->posY = obj->subA->posY;
    }
    {
        u8 *framesB = obj->subB->frames;
        *obj->subA->frames = 3;
        *framesB = 3;
    }
    {
        struct PairSub *subB = obj->subB;
        obj->subA->state04 = 0x10;
        subB->state04 = 0x10;
    }
    {
        struct PairSub *subB = obj->subB;
        ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
        ((struct Bit6Bits *)&subB->flags351)->bit6 = 1;
    }
    {
        struct PairSub *subB = obj->subB;
        ((struct Bit3Bits *)&obj->subA->flags358)->bit3 = 1;
        ((struct Bit3Bits *)&subB->flags358)->bit3 = 1;
    }

    /* The jump is 0x20 high, more when A stands higher on a shared layer. */
    maxHeight = 0x20;
    if (obj->subA->unk14 > obj->subB->unk14)
    {
        u8 layersA = obj->subA->type26;
        u8 layersB = obj->subB->type26;
        if ((((u32)(layersA & layersB)) << 26) >> 30)
            maxHeight = obj->subA->unk14 - obj->subB->unk14 + 0x20;
    }
    jumpHeight = ((obj->subA->unk14 + obj->subA->unk18) - (obj->subB->unk14 + obj->subB->unk18)) / 0x100 + 0x20;
    if (jumpHeight > maxHeight)
        jumpHeight = maxHeight;

    /* flagFB bit 2 halves the gravity and stretches the jump from 8 to 14 frames. */
    motion_obj_set_move_target(obj->subB, jumpHeight, 0xEE >> (((u32)obj->flagFB << 29) >> 31), obj->move100);
    contacts_obj_start_move(obj->subB, 2, 0, obj->subA->posX / 0x100, obj->subA->posY / 0x100, 0,
        (obj->flagFB & 4) ? 0xE : 8);
    ((struct Bit7Bits *)&obj->flags20B)->bit7 = 1;
    set_slot_value(0x2B, -1);
}
