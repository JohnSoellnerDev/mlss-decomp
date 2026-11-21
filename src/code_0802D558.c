#include "global.h"

struct FrameTable
{
    u8 filler0[2];
    u8 cur;
    u8 filler3[0x10 - 3];
    u8 unk10;
    u8 filler11[0x14 - 0x11];
    u8 unk14;
};

struct Entry
{
    u8 filler0[0x14];
    s32 e14;
    s32 e18;
};

struct SubPart
{
    u8 filler0[0x2];
    u8 kind;
    u8 filler3[0x1];
    u16 unk4;
    u8 filler6[0x6];
    s32 targetX;
    s32 targetY;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 rate24 : 3;
    u8 rateHi24 : 5;
    u8 filler25[0xF];
    s32 curX;
    s32 curY;
    u8 filler3C[0x54 - 0x3C];
    u8 unk54;
    u8 filler55[0x20C - 0x55];
    u8 flags20C;
    u8 flags20D;
    u8 filler20E[0x2A];
    s8 unk238;
    u8 filler239[0x23];
    s32 unk25C;
    u8 filler260[0x18];
    u16 halfW278;
    u8 filler27A[0x2];
    u16 halfH27C;
    u8 filler27E[0x16];
    u8 *unk294;
    u8 filler298[0xB9];
    u8 flags351;
    u8 filler352[0x6];
    u8 flags358lo : 3;
    u8 bit358_3 : 1;
    u8 flags358hi : 4;
    u8 filler359[0xF];
    struct FrameTable *frames;
};

struct SceneObj
{
    u8 filler0[0x28];
    struct Entry *entries[0x34];
    u8 fillerF8[0x1];
    u8 flagsF9;
    u16 unkFA;
    u8 fillerFC[0x110 - 0xFC];
    u8 listAStart;  // entries[] index of list A (0: none)
    u8 listACount;
    u8 filler112[0x114 - 0x112];
    u8 listB1Start; // lists B1 and B2 lie back to back in entries[]
    u8 listB1Count;
    u8 listB2Start;
    u8 listB2Count;
    u8 filler118[0x12A - 0x118];
    u8 action12A;
    u8 filler12B[0xD5];
    struct SubPart *subA;
    struct SubPart *subB;
    u8 flags208;
    u8 flags209;
    u8 filler20A[0x1];
    u8 flags20B;
    u8 filler20C[0x34];
    s16 speed;
    u8 filler242[0x4];
    u8 flags246;
    u8 filler247[0x1];
    u16 offX;
    u16 offY;
    u8 filler24C[0x41];
    u8 sel28D;
    u8 filler28E[0x1];
    u8 sel28F;
    u8 filler290[0x48];
    u8 state2D8;
    u8 filler2D9[0x2B];
    u8 *map;
};

struct Bit3Bits
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct Bit5Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

struct FABits
{
    u16 lo5 : 5;
    u16 mid5 : 5;
    u16 bit10 : 1;
    u16 hi5 : 5;
};

struct Bit6Bits
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

/* SubPart.unk4: the part's mode. */
#define SUB_MODE_FOLLOW 0xD /* moves along with the pair's entries (dispatch_sub_state) */

/* Bits 3-6 of a map tile's first byte (tile_ptr_at): the tile's kind. */
#define TILE_KIND_MASK 0x78

struct RateVec
{
    s32 x;
    s32 y;
};

extern s16 gSineTable[];
extern s16 gCosineTable[];
u8 *tile_ptr_at(u8 *map, s32 x, s32 y);
void pair_set_mode(struct SceneObj *obj, u32 mode, u32 flag);
void advance_anim_frame(struct SceneObj *obj, struct SubPart *sub, u32 a, u32 b);
void obj_shift_state_slots(void *sub);
void sprites_obj_set_move_target(struct SceneObj *obj, u32 type, u32 one, s32 x, s32 y, s32 z);
void camera_track_entity(struct SceneObj *obj, u32 field, u32 zero);
void set_object_mode(struct SceneObj *obj, u32 mode, u32 a, u32 packed, u32 x, u32 y);

/* Steps the state slots and the animation of A (bit 0) and/or B (bit 1). */
static inline void pair_shift_advance(struct SceneObj *obj, u32 which)
{
    if ((which & 1) != 0)
    {
        obj_shift_state_slots(obj->subA);
        advance_anim_frame(obj, obj->subA, 0, 0);
    }
    if ((which & 2) != 0)
    {
        obj_shift_state_slots(obj->subB);
        advance_anim_frame(obj, obj->subB, (((u32)obj->flags208 << 27) >> 31), 0);
    }
}

ASM_FUNC("asm/code_0802D558/entity_update_obj_update_stage.s", void entity_update_obj_update_stage(void))

/* Puts the pair into follow mode: unless flags209 bit 6 is set, with B on frame entry 1 the pair's
 * offset becomes B's target in pixels (the speed along A's direction is worked out but unused).
 * Then sets flags20B bit 7, flags209 bit 5, bit 3 of both parts' flags358 and both parts' mode
 * to SUB_MODE_FOLLOW. */
void setup_obj_rates(struct SceneObj *obj)
{
    u8 angle;
    s32 speed;
    s32 vx;
    s32 vy;

    if (!(obj->flags209 & 0x40) && obj->subB->frames->cur == 1)
    {
        /* A's eight-way direction, turned around, as a 256-step angle */
        angle = (obj->subA->rate24 ^ 4) << 5;
        obj->offX = obj->subB->targetX / 256;
        obj->offY = obj->subB->targetY / 256;
        speed = obj->speed;
        vx = gSineTable[angle] * speed;
        if (vx < 0)
            vx = -vx;
        vy = gCosineTable[angle] * speed;
        if (vy < 0)
            vy = -vy;
    }
    ((struct Bit6Bits *)&obj->flags20B)->hi1 = 1;
    ((struct Bit5Bits *)&obj->flags209)->bit5 = 1;
    obj->subB->bit358_3 = obj->subA->bit358_3 = 1;
    obj->subB->unk4 = obj->subA->unk4 = SUB_MODE_FOLLOW;
}

extern u8 *gUnk_03000FC0;
u8 test_flag(u8 *table, u8 *tableB, u16 id);
void advance_obj_motion(struct SubPart *sub, struct Entry **entries);
u8 obj_flags_2b5_07_set(struct SubPart *sub);
void obj_advance_motion(struct SubPart *sub);
void update_linked_pos(struct SubPart *sub, struct Entry **entries, u8 a, u8 b);
void pair_start_state_3b(struct SceneObj *obj);
void sub_08035988(struct SceneObj *obj);
void sub_0803908C(struct SceneObj *obj);
void pair_start_state_37(struct SceneObj *obj);
void set_sub_pair_state_49(struct SceneObj *obj);
void obj_enter_stage4_slot(struct SubPart *sub, u8 flag);
void sub_08036E28(struct SceneObj *obj);
void sub_08039A14(struct SceneObj *obj);
void sub_08038154(struct SceneObj *obj);
void sub_08039B70(struct SceneObj *obj);
void pair_start_state_3a(struct SceneObj *obj);
void init_sub_pair(struct SceneObj *obj);
void obj_enter_stage4_aux(struct SubPart *sub);
/* Picks the pair's next state. While B is in mode 0xD it moves along with the pair's entries and
   keeps its unk14 + unk18 level equal to A's. Once flags209 bit 5 is clear, B's current frame
   entry (1 or 2) and the pair's mode (flags246 bits 2-4) choose the next state; in modes 0 and 8
   it also depends on which hero leads (B's unk54) and on that hero's story flag (0x206/0x207 for
   entry 1, 0x20A/0x20B for entry 2). Any state picked sets A's flags351 bit 6. */
void dispatch_sub_state(struct SceneObj *obj)
{
    struct SubPart *follower;
    u32 pair_mode;

    if ((s16)obj->subB->unk4 == SUB_MODE_FOLLOW)
    {
        advance_obj_motion(obj->subB, obj->entries);
        if (!obj_flags_2b5_07_set(obj->subB))
            ((struct Bit5Bits *)&obj->flags209)->bit5 = 0;
        obj_advance_motion(obj->subB);
        update_linked_pos(obj->subB, obj->entries, 0, 0);
        follower = obj->subB;
        if (follower->unk14 + follower->unk18 != obj->subA->unk14 + obj->subA->unk18)
            follower->unk18 = obj->subA->unk14 + obj->subA->unk18 - follower->unk14;
    }
    if (obj->flags209 & 0x20)
        return;
    switch (obj->subB->frames->cur)
    {
    case 1:
        pair_mode = obj->flags246 & 0x1C;
        if (pair_mode == 0 || pair_mode == 8)
        {
            if (obj->subB->unk54 == 0)
            {
                if (test_flag(gUnk_03000FC0 + 0x38C, 0, 0x206))
                {
                    pair_start_state_3b(obj);
                    break;
                }
            }
            else
            {
                if (test_flag(gUnk_03000FC0 + 0x38C, 0, 0x207))
                {
                    sub_08035988(obj);
                    break;
                }
            }
            sub_0803908C(obj);
        }
        else if (pair_mode == 4)
            pair_start_state_37(obj);
        else if (pair_mode == 0x10)
            set_sub_pair_state_49(obj);
        else
            obj_enter_stage4_slot(obj->subB, (((u8 *)&obj->unkFA)[1] >> 2) & 1);
        break;
    case 2:
        pair_mode = obj->flags246 & 0x1C;
        if (pair_mode == 0)
        {
            if (obj->subB->unk54 == 0)
            {
                if (test_flag(gUnk_03000FC0 + 0x38C, 0, 0x20A))
                    sub_08036E28(obj);
                else
                    sub_08039A14(obj);
            }
            else
            {
                if (test_flag(gUnk_03000FC0 + 0x38C, 0, 0x20B))
                    sub_08038154(obj);
                else
                    sub_08039B70(obj);
            }
        }
        else if (pair_mode == 8)
            init_sub_pair(obj);
        else if (pair_mode == 4)
            pair_start_state_3a(obj);
        else if (pair_mode == 0x10)
            init_sub_pair(obj);
        else
            obj_enter_stage4_aux(obj->subB);
        break;
    default:
        return;
    }
    ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
}

u8 obj_tile_corners_check(struct SubPart *sub);
u8 objects_overlap(struct SubPart *sub, struct Entry *entry);
// Returns 1 if sub is blocked: by the map tiles at its corners, or by overlapping any entity in
// list A or in lists B1/B2 (which lie back to back in entries[]); 0 if it is free.
u8 overlaps_entry_lists(struct SceneObj *obj, struct SubPart *sub)
{
    struct Entry **entry;
    s32 countA;
    s32 countB;
    s32 startB;

    if (obj_tile_corners_check(sub))
        return 1;
    if (obj->listAStart != 0)
    {
        entry = &obj->entries[obj->listAStart];
        for (countA = obj->listACount; countA != 0; countA--, entry++)
        {
            if (objects_overlap(sub, *entry))
                return 1;
        }
    }
    if (obj->listB1Start != 0)
        startB = obj->listB1Start;
    else if (obj->listB2Start != 0)
        startB = obj->listB2Start;
    else
        startB = 0;
    if (startB > 0)
    {
        entry = &obj->entries[startB];
        for (countB = obj->listB1Count + obj->listB2Count; countB != 0; countB--, entry++)
        {
            if (objects_overlap(sub, *entry))
                return 1;
        }
    }
    return 0;
}

ASM_FUNC("asm/code_0802D558/advance_anim_frame.s", void advance_anim_frame(struct SceneObj *obj, struct SubPart *sub, u32 a, u32 b))

ASM_FUNC("asm/code_0802D558/pair_turn_and_sample.s", void pair_turn_and_sample(void))

ASM_FUNC("asm/code_0802D558/queue_sub_motion.s", void queue_sub_motion(void))

ASM_FUNC("asm/code_0802D558/follow_anchor.s", void follow_anchor(void))

/* Looks at the map tile at the edge of part A in the direction it faces (the top 3 bits of
 * SubPart.kind, 8 ways; diagonals never match): with mode 0 says whether that tile's kind is 0x18,
 * otherwise whether it is 0x28 or 0x38. Positions are 24.8 fixed point, tiles 16 px (0x1000). */
u32 anchor_tile_matches(struct SceneObj *obj, u32 mode)
{
    struct SubPart *anchor;
    s32 facing;
    s32 x;
    s32 y;
    s32 roundX;
    s32 roundY;
    u8 *map;
    u8 *tile;
    u32 tileKind;
    s32 tileX;
    s32 tileY;

    anchor = obj->subA;
    facing = anchor->kind >> 5;
    if (((u8)(facing ^ 1) & 1) == 0) {
        return 0;
    }
    x = anchor->curX;
    y = anchor->curY;
    switch (facing) {
    case 0:
        y += 0x100;
        break;
    case 2:
        x -= anchor->halfW278 >> 1;
        break;
    case 4:
        y -= anchor->halfH27C;
        break;
    case 6:
        x += -0x100;
        x += anchor->halfW278 >> 1;
        break;
    }
    map = obj->map;
    roundX = x;
    if (roundX < 0) {
        roundX += 0xFFF;
    }
    tileX = (roundX << 4) >> 16;
    roundY = y;
    if (roundY < 0) {
        roundY += 0xFFF;
    }
    tileY = (roundY << 4) >> 16;
    tile = tile_ptr_at(map, tileX, tileY);
    if (mode == 0) {
        if ((*tile & TILE_KIND_MASK) == 0x18) {
            return 1;
        }
    } else {
        tileKind = *tile & TILE_KIND_MASK;
        if (tileKind == 0x28) {
            return 1;
        }
        if (tileKind == 0x38) {
            return 1;
        }
    }
    return 0;
}

ASM_FUNC("asm/code_0802D558/set_sub_pair_mode.s", void set_sub_pair_mode(void))

ASM_FUNC("asm/code_0802D558/swap_sub_pair.s", void swap_sub_pair(void))

ASM_FUNC("asm/code_0802D558/select_sub_pair.s", void select_sub_pair(void))

// Steps A's state slots and animation and marks both members (flags351 bit 6). Once all of
// state2D8's low five bits are set it brings the camera onto the pair and goes to action 4:
// with mode 0 it pans the camera to centre on B, above the floor B stands on (entry
// B->unk238, or B's own unk14) less the sel28D/sel28F margin; otherwise the camera follows the
// other member (unkFA bits 5-9, ^ 1). Until then it sets object mode 2 (step 0x10 with mode 0,
// else 1), clears A's flags20D bits 3 and 6, sets flagsF9 bit 5 and goes to action 0x11.
void start_sub_pair_step(struct SceneObj *obj, s32 mode)
{
    s32 camX;
    s32 camY;

    pair_shift_advance(obj, 1);
    ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
    ((struct Bit6Bits *)&obj->subB->flags351)->bit6 = 1;
    if ((obj->state2D8 & 0x1F) == 0x1F)
    {
        if (mode == 0)
        {
            s32 floor;
            u8 *margin;

            if (obj->subB->unk238 != -1)
            {
                struct Entry *standOn = obj->entries[obj->subB->unk238];

                floor = standOn->e14 + standOn->e18;
            }
            else
            {
                floor = obj->subB->unk14;
            }
            camX = obj->subB->targetX / 256 - SCREEN_WIDTH / 2;
            camY = (obj->subB->targetY - floor) / 256;
            if (obj->subB->unk25C != -1)
                margin = &obj->sel28D;
            else
                margin = &obj->sel28F;
            sprites_obj_set_move_target(obj, 1, 0, camX, camY - *margin, 0xC00);
        }
        else
        {
            camera_track_entity(obj, ((u32)(obj->unkFA << 22) >> 27) ^ 1, 0);
        }
        obj->action12A = 4;
    }
    else
    {
        u32 step = 1;

        if (mode == 0)
            step = 0x10;
        set_object_mode(obj, 2, step, 0, 0xFFFF, 0xFFFF);
        ((struct Bit3Bits *)&obj->subA->flags20D)->bit3 = 0;
        ((struct Bit6Bits *)&obj->subA->flags20D)->bit6 = 0;
        ((struct Bit5Bits *)&obj->flagsF9)->bit5 = 1;
        obj->action12A = 0x11;
    }
}
