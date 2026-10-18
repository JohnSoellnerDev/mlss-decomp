/* agbcc: -funroll-loops */
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
    u8 filler18[0xC];
    u8 rateSrc24;
    u8 filler25[0xF];
    s32 curX;
    s32 curY;
    u8 filler3C[0x1D0];
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
    u8 flags358;
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
    u8 fillerFC[0x2E];
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

extern s16 gSineTable[];
extern s16 gCosineTable[];
u8 *tile_ptr_at(u8 *map, s16 x, s16 y);
void pair_set_mode(struct SceneObj *obj, u32 mode, u32 flag);
void advance_anim_frame(struct SceneObj *obj, struct SubPart *sub, u32 a, u32 b);
void obj_shift_state_slots(void *sub);
void sprites_obj_set_move_target(struct SceneObj *obj, u32 type, u32 one, s32 x, s32 y, s32 z);
void camera_track_entity(struct SceneObj *obj, u32 field, u32 zero);
void set_object_mode(struct SceneObj *obj, u32 mode, u32 a, u32 packed, u32 x, u32 y);

ASM_FUNC("asm/code_0802D558/entity_update_obj_update_stage.s", void entity_update_obj_update_stage(void))

ASM_FUNC("asm/code_0802D558/setup_obj_rates.s", void setup_obj_rates(void))

ASM_FUNC("asm/code_0802D558/dispatch_sub_state.s", void dispatch_sub_state(void))

ASM_FUNC("asm/code_0802D558/overlaps_entry_lists.s", void overlaps_entry_lists(void))

ASM_FUNC("asm/code_0802D558/advance_anim_frame.s", void advance_anim_frame(struct SceneObj *obj, struct SubPart *sub, u32 a, u32 b))

ASM_FUNC("asm/code_0802D558/pair_turn_and_sample.s", void pair_turn_and_sample(void))

ASM_FUNC("asm/code_0802D558/queue_sub_motion.s", void queue_sub_motion(void))

ASM_FUNC("asm/code_0802D558/follow_anchor.s", void follow_anchor(void))

NONMATCH("asm/code_0802D558/anchor_tile_matches.s", u32 anchor_tile_matches(struct SceneObj *obj, u32 mode))
{
    struct SubPart *anchor;
    s32 kind;
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    u8 *map;
    u8 *tile;
    u32 k;

    anchor = obj->subA;
    kind = anchor->kind >> 5;
    if (((u8)(kind ^ 1) & 1) == 0) {
        return 0;
    }
    x = anchor->curX;
    y = anchor->curY;
    switch (kind) {
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
    px = (x < 0) ? x + 0xFFF : x;
    py = (y < 0) ? y + 0xFFF : y;
    tile = tile_ptr_at(map, (s16)((px << 4) >> 16), (s16)((py << 4) >> 16));
    if (mode == 0) {
        if ((*tile & 0x78) == 0x18) {
            return 1;
        }
    } else {
        k = *tile & 0x78;
        if (k == 0x28) {
            return 1;
        }
        if (k == 0x38) {
            return 1;
        }
    }
    return 0;
}
END_NONMATCH

ASM_FUNC("asm/code_0802D558/set_sub_pair_mode.s", void set_sub_pair_mode(void))

ASM_FUNC("asm/code_0802D558/swap_sub_pair.s", void swap_sub_pair(void))

ASM_FUNC("asm/code_0802D558/select_sub_pair.s", void select_sub_pair(void))

ASM_FUNC("asm/code_0802D558/start_sub_pair_step.s", void start_sub_pair_step(void))
