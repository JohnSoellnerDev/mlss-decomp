#include "global.h"
#include "ram_routines.h"

extern u8 *gParty;
extern u8 *gUnk_03000FF8;
extern u8 *gUnk_03000FC0;
struct SysTimer {
    u8 filler0[0x24];
    u32 frames;
};
extern struct SysTimer gSys;

u32 draw_bro_levels(u8 *screen, u32 oam);
u32 draw_decimal_sprites(u32 flags, u32 value, u32 oam, u32 x, u32 y, u32 width, u32 palette, u32 spacing);
u32 draw_sprite(u32 oam, u32 x, u32 y, u32 mode, u32 form, u32 size, u32 tile, u32 prio, u32 palette, u32 flipH, u32 flipV);
u8 test_flag(u8 *table, u8 *tableB, u16 id);
void mem_copy_fast(void *dst, const void *src, u32 n);

ASM_FUNC("asm/code_0811C1E8/obj_sprite_state_step.s", void obj_sprite_state_step(void))

ASM_FUNC("asm/code_0811C1E8/obj_sprite_refresh.s", void obj_sprite_refresh(void))

ASM_FUNC("asm/code_0811C1E8/window_obj_create_sprites.s", void window_obj_create_sprites(void))

NONMATCH("asm/code_0811C1E8/draw_party_overview.s", u32 draw_party_overview(u8 *screen, u32 oam))
{
    u16 tileBase[2];
    u16 tileStage[2];
    u8 palSrc[2];
    u8 *pal;
    u8 *hero;
    u32 frames;
    s32 coarse;
    s32 rem;
    s32 tens;
    u32 remOff;
    u32 xOff;
    u32 fieldNo;
    u32 digitX;
    u32 row;
    s32 item;
    u32 count;
    u32 cursor;
    u32 member;
    u32 colOff;
    u32 recOff;
    u16 *tileSlot;
    s32 xA;
    s32 xB;
    s32 xC;
    s32 xD;
    s32 xE;
    s32 markY;
    s32 y;
    u32 hp;
    u32 v;

    hero = gUnk_03000FF8;
    tileStage[0] = (hero[4] != 0) ? 0x270 : 0x110;
    tileStage[1] = (hero[0xC] != 0) ? 0x270 : 0x110;
    mem_copy_fast(tileBase, tileStage, 4);
    hero = gUnk_03000FF8;
    palSrc[0] = (hero[4] != 0) ? 7 : 6;
    palSrc[1] = (hero[0xC] != 0) ? 7 : 6;
    pal = (u8 *)tileStage;
    mem_copy_fast(pal, palSrc, 2);
    if (screen[0x188] != 0) {
        oam = draw_bro_levels(screen, oam);
    }
    hp = *(u16 *)(gParty + 0x88);
    oam = draw_decimal_sprites(0x10, hp, oam, 0x6C, 8, 4, 0xFFFFFFFF, 7);
    frames = gSys.frames;
    if (frames > 0x014988F0) {
        oam = draw_decimal_sprites(2, 0x63, oam, 0x9E, 8, 2, 0xFFFFFFFF, 6);
        oam = draw_decimal_sprites(2, 0x3B, oam, 0xAC, 8, 2, 0xFFFFFFFF, 6);
    } else {
        coarse = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(frames, 0x3C);
        fieldNo = 0;
        remOff = (u32)iwram_math_08199424 - (u32)iwram_sqrt;
        xOff = 0;
        rem = -1;
        do {
            digitX = 0xAC;
            coarse = coarse / 0x3C;
            if (fieldNo == 0) {
                rem = ((s32 (*)(s32, s32))(gMathRoutines + remOff))(coarse, 0x3C);
            } else {
                rem = ((s32 (*)(s32, s32))(gMathRoutines + remOff))(coarse, 0x63);
            }
            tens = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(rem, 0xA);
            if (tens == 0) {
                oam = draw_decimal_sprites(2, 0, oam, (0xAC - xOff) & 0xFFFF, 8, 1, 0xFFFFFFFF, 0xFFFFFFFF);
                digitX = 0xB2;
            }
            oam = draw_decimal_sprites(2, rem, oam, (digitX - xOff) & 0xFFFF, 8, 2, 0xFFFFFFFF, 6);
            xOff += 0xE;
            fieldNo++;
        } while ((s32)fieldNo <= 1);
    }
    oam = draw_sprite(oam, 0xA9, 8, 0, 2, 0, 0x4A, 2, 1, 0, 0);
    oam = draw_sprite(oam, 0x60, 0, 0, 1, 3, 0x100, 2, 1, 0, 0);
    oam = draw_sprite(oam, 0xA0, 0, 0, 0, 2, 0x120, 2, 1, 0, 0);
    oam = draw_sprite(oam, 0x20, 0, 0, 1, 3, 0x160, 2, 4, 0, 0);
    oam = draw_sprite(oam, 0xC0, 0, 0, 0, 2, 0x184, 2, 4, 0, 0);
    oam = draw_sprite(oam, 0xB0, 0x10, 0, 0, 1, 0x180, 2, 4, 0, 0);
    oam = draw_sprite(oam, 0x72, 0x67, 0, 0, 2, 0x194, 2, 4, 0, 0);
    oam = draw_sprite(oam, 0x72, 0x87, 0, 1, 1, 0x1A4, 2, 4, 0, 0);
    row = 0;
    item = 0;
    if (item < screen[0x18A]) {
        do {
            if (item == 0) {
                if (test_flag(gUnk_03000FC0 + 0x38C, 0, 0x225) == 0) {
                    row++;
                }
            }
            cursor = screen[0x189];
            count = screen[0x18A];
            if (cursor != count - (item + 1)) {
                markY = (s16)((count << 4) - (item * 24 - 0x20));
                oam = draw_sprite(oam, 8, markY, 0, 0, 2, (0xB0 - (row << 4)) & 0xFFFF, 2, 3, 0, 0);
            }
            y = (s16)((screen[0x18A] << 4) - (item * 24 - 0x30));
            oam = draw_sprite(oam, 0x10, y, 0, 0, 1, 0x68, 2, 5, 0, 0);
            item = item + 1;
            row++;
        } while (item < screen[0x18A]);
    }
    member = 0;
    colOff = 0;
    recOff = 0;
    tileSlot = tileBase;
    for (;;) {
        u8 *rec = gParty + recOff;
        hp = *(u16 *)(rec + 0xE);
        xA = (s16)(0xA0 - colOff);
        oam = draw_decimal_sprites(8, hp, oam, xA & 0xFFFF, 0x50, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        v = *(u16 *)(gParty + recOff + 0x12);
        xB = (0xC0 - colOff) & 0xFFFF;
        oam = draw_decimal_sprites(8, v, oam, xB, 0x54, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        xC = (s16)(0xB9 - colOff);
        oam = draw_sprite(oam, xC, 0x58, 0, 0, 0, 0x49, 1, 0, 0, 0);
        v = *(u16 *)(gParty + recOff + 0x14);
        oam = draw_decimal_sprites(8, v, oam, xA & 0xFFFF, 0x68, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        v = *(u16 *)(gParty + recOff + 0x18);
        oam = draw_decimal_sprites(8, v, oam, xB, 0x6C, 3, 0xFFFFFFFF, 0xFFFFFFFF);
        oam = draw_sprite(oam, xC, 0x70, 0, 0, 0, 0x49, 1, 0, 0, 0);
        xD = (s16)(0xBC - colOff);
        oam = draw_sprite(oam, xD, 0x2C, 0, 0, 2, *tileSlot + 0x20, 3, *(u8 *)((u8 *)pal + member), 0, 0);
        oam = draw_sprite(oam, xD, 0x4C, 0, 1, 1, *tileSlot + 0x30, 3, *(u8 *)((u8 *)pal + member), 0, 0);
        xE = (s16)(0x80 - colOff);
        oam = draw_sprite(oam, xE, 0x50, 0, 1, 2, 0xC0, 2, 2, 0, 0);
        oam = draw_sprite(oam, xE, 0x68, 0, 1, 2, 0xC8, 2, 2, 0, 0);
        tileSlot++;
        colOff += 0x60;
        recOff += 0x3C;
        member++;
        if ((s32)member > 1) {
            break;
        }
    }
    return oam;
}
END_NONMATCH
