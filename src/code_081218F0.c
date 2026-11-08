#include "global.h"

struct TweenEntry
{
    void *sprite;
    u8 filler4[0x3C - 4];
    u8 flags3C;
    u8 filler3D[3];
};

struct StatsCtx
{
    u8 filler0[0x20];
    void *sub20;
    u8 filler24[0x30 - 0x24];
    void *sprite30;
    u8 filler34[0x156 - 0x34];
    u8 flag156;
    u8 filler157;
    struct TweenEntry *tweens;
    u8 filler15C[0x17E - 0x15C];
    u16 displayShadow17E;
};

struct WindowObj
{
    u8 filler0[0x18];
    const void *klass;
};

struct SlotSrc
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 4];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 filler21[0x24 - 0x21];
    u8 unk24;
    u8 unk25;
};

struct SlotRec
{
    struct SlotSrc *src;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u16 bits10;
    u16 unk12;
    u8 filler14[0x3A - 0x14];
    u16 unk3A;
    u8 flag3C;
    u8 flag3D;
};

struct SlotRecBitsAt
{
    u8 filler0[0x10];
    u16 b0 : 1;
    u16 b1 : 1;
    u16 b2 : 1;
    u16 f3 : 2;
    u16 f5 : 2;
    u16 f7 : 2;
    u16 f9 : 2;
};

struct SlotHolder
{
    u8 filler0[0x158];
    struct SlotRec *recs;
};

struct SpriteObj
{
    u16 oam0;
    u16 oam1;
    u8 filler4[0x12 - 4];
    u8 flags12;
    u8 filler13[0x16 - 0x13];
    u16 tile16;
};

struct SlideObj
{
    u8 filler0[0xC];
    u8 state;
};

struct ScreenView
{
    u8 state;
    u8 filler1[0x30 - 1];
    struct SpriteObj *cursor;
    struct SpriteObj *slider;
    u8 filler38[0x3C - 0x38];
    void *item;
    u8 filler40[0x44 - 0x40];
    struct SpriteObj *sideL;
    struct SpriteObj *sideR;
    void *spr4C[14];
    u8 filler84[0x88 - 0x84];
    struct SlideObj *slide;
    u8 filler8C[0x14A - 0x8C];
    u8 slideMax;
    u8 slideStep;
    s8 slideMin;
    u8 filler14D[0x14F - 0x14D];
    u8 mode;
    u8 filler150;
    u8 entryMark;
    u8 transPending;
    u8 filler153;
    u8 transActive;
    u8 filler155[0x158 - 0x155];
    struct TweenEntry *tweens;
    u8 filler15C[0x188 - 0x15C];
    u8 waveFlag;
    u8 cursorIdx;
    u8 count;
    u8 filler18B[0x18E - 0x18B];
    u8 values[3];
    u8 flagsTbl[3];
    u8 index;
    u8 filler195[0x19E - 0x195];
    u8 category;
    u8 page;
    u8 filler1A0[0x1BC - 0x1A0];
    u8 member;
    u8 filler1BD;
    u8 pick;
};

struct ViewOwner
{
    u8 filler0[0x14];
    struct ScreenView *screen;
};

extern u16 gUnk_02000008;
extern u8 gUnk_03000C80[];
extern u32 gUnk_03000FE4;
extern const u8 gWindowChildClass[];

void obj_release_slots(void *sprite);
void sprite_update_cd0(void *sprite);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void task_insert(void *node, u8 kind);
u32 sub_obj_running(void *obj);
void sub_08115F54(void *sub, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
void setup_obj_10bc(void *obj, u32 bank, s32 val, u32 one, s32 a, s32 b, s32 c, s32 d);
u32 draw_decimal_sprites(u32 digits, u32 value, u32 oam, u32 x, u32 y, u32 mode, s32 a, s32 b);

extern u8 gSys[];
extern u8 *gParty;
extern u16 gUnk_02000044[];
extern const u16 gScreenLangTileTable[];
extern u8 *gUnk_03000FC0;
struct ObjListRootX
{
    u8 filler0[0x3C];
    u32 root;
};
extern struct ObjListRootX gUnk_0203FFB8;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();

void menu_transition_step(struct ScreenView *screen);
void screen_wave_frame(struct ScreenView *screen);
void screen_pending_redraw(struct ScreenView *screen);
u32 test_flag(u8 *table, u8 *tableB, u16 id);
void mem_copy_fast(void *dst, const void *src, u32 n);
void update_obj_list(void);
void obj_list_pass_0a8c(void);
u32 obj_draw_list(struct ScreenView *screen, u32 oam);
u32 screen_helpers_obj_draw_grid(struct ScreenView *screen, u32 oam);
u32 draw_party_overview(struct ScreenView *screen, u32 oam);
u32 draw_cursor_row(struct ScreenView *screen, u32 oam);
u32 obj_draw_scroll_list(struct ScreenView *screen, u32 oam);
u32 draw_member_status_page(struct ScreenView *screen, u32 oam);

ASM_FUNC("asm/code_081218F0/screen_wave_frame.s", void screen_wave_frame(struct ScreenView *screen))

ASM_FUNC("asm/code_081218F0/tween_move_to.s", void tween_move_to(void))

ASM_FUNC("asm/code_081218F0/tween_step.s", void tween_step(void))

NONMATCH("asm/code_081218F0/unknown_thumb_08121e42.s", void unknown_thumb_08121e42(struct SlotHolder *base))
{
    s32 three;
    s32 one;
    s32 i;
    struct SlotRec **recs;

    recs = &base->recs;
    one = 1;
    three = 3;
    for (i = 0; i < 12; i++)
    {
        if ((*recs)[i].flag3D != 0)
        {
            (*recs)[i].unk4 = (*recs)[i].src->unk0;
            (*recs)[i].unk6 = (*recs)[i].src->unk2;
            (*recs)[i].unk8 = ((((u32)(*recs)[i].src->unk1E << 28) >> 16) + 0x1000) | (*recs)[i].src->unk16;
            (*recs)[i].unkA = (*recs)[i].unk3A;
            (*recs)[i].unkC = (*recs)[i].src->unk24;
            (*recs)[i].unkD = (*recs)[i].src->unk25;
            (*recs)[i].unkE = (*recs)[i].flag3D;
            (*recs)[i].unkF = (*recs)[i].src->unk20;
            ((struct SlotRecBitsAt *)&(*recs)[i])->b0 = (((u32)(*recs)[i].flag3C << 31) >> 31);
            ((struct SlotRecBitsAt *)&(*recs)[i])->b1 = (((u32)(*recs)[i].src->unk12 << 27) >> 31);
            ((struct SlotRecBitsAt *)&(*recs)[i])->b2 = (((u32)(*recs)[i].src->unk11 << 25) >> 31);
            ((struct SlotRecBitsAt *)&(*recs)[i])->f3 = (((u32)(*recs)[i].src->unk1F << 30) >> 30);
            ((struct SlotRecBitsAt *)&(*recs)[i])->f5 = (((u32)(*recs)[i].src->unk1F << 28) >> 30);
            ((struct SlotRecBitsAt *)&(*recs)[i])->f7 = (((u32)(*recs)[i].src->unk1F << 26) >> 30);
            ((struct SlotRecBitsAt *)&(*recs)[i])->f9 = ((*recs)[i].src->unk1F >> 6);
            (*recs)[i].unk12 = (*recs)[i].src->unkE;
        }
    }
}
END_NONMATCH

static inline void update_if_set(void *p)
{
    if (p != 0)
    {
        sprite_update_cd0(p);
    }
}

static inline void tween_refresh(struct ScreenView *screen, s32 idx)
{
    struct TweenEntry *entry = &screen->tweens[idx];

    if (entry->sprite != 0)
    {
        if (entry->flags3C & 1)
        {
            sprite_update_cd0(entry->sprite);
        }
        else
        {
            obj_release_slots(entry->sprite);
        }
    }
}

NONMATCH("asm/code_081218F0/screen_view_update.s", void screen_view_update(struct ViewOwner *owner))
{
    struct ScreenView *screen;
    struct SlideObj **slideSlot;
    void **p;
    u16 langTiles[5];
    s32 group;
    u32 oam;
    u32 x;
    u32 flagA;
    u32 flagB;
    u32 flagC;
    u32 colC;
    struct SpriteObj *cur;
    u8 *sys;

    mem_copy_fast(langTiles, gScreenLangTileTable, 0xA);
    sys = gSys;
    screen = owner->screen;
    oam = *(u32 *)(sys + 0x880);
    if (screen->state <= 4)
    {
        return;
    }
    if (screen->transActive != 0)
    {
        menu_transition_step(screen);
    }
    if (screen->transPending != 0)
    {
        screen_pending_redraw(screen);
    }
    if (!(sys[0x888] & 0x10))
    {
        slideSlot = &screen->slide;
        if ((*slideSlot)->state == 1)
        {
            if (!(screen->cursor->flags12 & 0x20))
            {
                obj_release_slots(screen->cursor);
            }
            obj_release_slots(screen->sideL);
            obj_release_slots(screen->sideR);
            screen->slider->oam1 -= screen->slideStep;
            if ((s32)screen->slideMin > (s16)screen->slider->oam1)
            {
                screen->slider->oam1 = screen->slideMin;
            }
        }
        else if ((*slideSlot)->state == 3)
        {
            obj_release_slots(screen->cursor);
            obj_release_slots(screen->sideL);
            obj_release_slots(screen->sideR);
            screen->slider->oam1 += screen->slideStep;
            if ((s32)screen->slideMax < (s16)screen->slider->oam1)
            {
                screen->slider->oam1 = screen->slideMax;
            }
        }

        if (screen->transActive != 0)
        {
            switch (screen->mode)
            {
            case 1:
                screen->cursor->oam0 = 0x4E;
                screen->cursor->oam1 = screen->values[screen->index] * 0xB + 0xC;
                if (screen->flagsTbl[screen->index] != 0)
                {
                    sprite_update_cd0(screen->cursor);
                }
                screen->sideL->oam0 = 0x84;
                screen->sideR->oam0 = 0x14;
                screen->sideL->oam1 = 0x7C;
                screen->sideR->oam1 = 0x7C;
                sprite_update_cd0(screen->sideL);
                sprite_update_cd0(screen->sideR);
                break;
            case 2:
                obj_load_record(screen->item, screen->page + 1, 0, 0, 0);
                sprite_update_cd0(screen->item);
                screen->cursor->oam0 = 0x68;
                flagA = test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x222);
                flagB = test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x220);
                flagC = test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x222);
                x = 0x2C - ((flagA << 24) >> 21);
                x = x - ((flagB << 24) >> 20);
                colC = 0x20 - ((flagC << 24) >> 21);
                cur = screen->cursor;
                cur->oam1 = x + colC * screen->category;
                sprite_update_cd0(screen->cursor);
                break;
            case 3:
                obj_load_record(screen->item, screen->member + 1, 0, 0, 0);
                sprite_update_cd0(screen->item);
                break;
            case 4:
                p = &screen->spr4C[7];
                update_if_set(screen->spr4C[0]);
                update_if_set(screen->spr4C[1]);
                update_if_set(screen->spr4C[2]);
                update_if_set(screen->spr4C[3]);
                update_if_set(screen->spr4C[4]);
                update_if_set(screen->spr4C[5]);
                update_if_set(screen->spr4C[6]);
                update_if_set(screen->spr4C[7]);
                update_if_set(screen->spr4C[8]);
                update_if_set(screen->spr4C[9]);
                update_if_set(p[3]);
                update_if_set(p[4]);
                update_if_set(p[5]);
                update_if_set(p[6]);
                if (screen->pick != 0xFF)
                {
                    setup_obj_10bc(screen->cursor, 7, langTiles[sys[0x8C0]], 1, -1, -1, -1, -1);
                    obj_load_record(screen->cursor, screen->pick, -1, 0, 0);
                    sprite_update_cd0(screen->cursor);
                    screen->cursor->oam0 = 0xDA;
                    screen->cursor->oam1 = 0xA;
                }
                break;
            case 0:
                if (screen->cursor->tile16 == (langTiles[sys[0x8C0]] & 0xFFF))
                {
                    if (screen->pick != 0xFF)
                    {
                        setup_obj_10bc(screen->cursor, 5, 0x6000, 1, -1, -1, -1, -1);
                    }
                }
                obj_load_record(screen->cursor, 0, -1, 0, 0);
                screen->cursor->oam0 = 0x18;
                screen->cursor->oam1 = ((((screen->cursorIdx << 1) + screen->cursorIdx) << 3) + 0x46) - (screen->count << 3);
                sprite_update_cd0(screen->cursor);
                screen->cursor->flags12 &= 0xEF;
                screen->cursor->flags12 &= 0xF9;
                screen->sideL->oam0 = 0xCC;
                screen->sideR->oam0 = 0x6C;
                screen->sideL->oam1 = 0x50;
                screen->sideR->oam1 = 0x50;
                sprite_update_cd0(screen->sideL);
                sprite_update_cd0(screen->sideR);
                screen->slider->flags12 &= 0xEF;
                break;
            default:
                break;
            }
        }

        if (screen->mode == 0 && !((*slideSlot)->state & 1) && screen->waveFlag == 0)
        {
            oam = draw_decimal_sprites(8, gParty[0x30], oam, 0xA8, 0x38, 2, -1, -1);
            oam = draw_decimal_sprites(8, gParty[0x6C], oam, 0x48, 0x38, 2, -1, -1);
        }
        if (screen->mode == 1 && !((*slideSlot)->state & 1))
        {
            oam = obj_draw_list(screen, oam);
        }
        if (screen->mode == 4 && !((*slideSlot)->state & 1))
        {
            oam = screen_helpers_obj_draw_grid(screen, oam);
        }
        if (screen->entryMark != 0xFF)
        {
            group = 0;
            do
            {
                tween_refresh(screen, group);
                tween_refresh(screen, group + 1);
                tween_refresh(screen, group + 2);
                group += 3;
            } while (group <= 11);
        }

        update_obj_list();
        obj_list_pass_0a8c();
        gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
        oam = gUnk_03000D4C(gUnk_0203FFB8.root, oam);
        if (!((*slideSlot)->state & 1))
        {
            switch (screen->mode)
            {
            case 0:
                oam = draw_party_overview(screen, oam);
                break;
            case 1:
                oam = draw_cursor_row(screen, oam);
                break;
            case 2:
                oam = obj_draw_scroll_list(screen, oam);
                break;
            case 3:
                oam = draw_member_status_page(screen, oam);
                break;
            }
        }
        *(u32 *)(sys + 0x880) = oam;
    }
    if (screen->waveFlag != 0)
    {
        screen_wave_frame(screen);
    }
}
END_NONMATCH

NONMATCH("asm/code_081218F0/unknown_thumb_081225d0.s", void unknown_thumb_081225d0(void *unused, u16 *map, s32 x, s32 y, s32 w, s32 h, u16 start, u8 mode))
{
    u16 i;
    u16 j;
    u16 *row;

    x = x >> 3;
    y = y >> 3;
    w = w + x;
    h = h + y;

    for (i = y; i < h; i++)
    {
        for (j = x; j < w; j++)
        {
            row = map + i * 32;
            if (mode == 0)
            {
                row[j] = mode;
            }
            else
            {
                row[j] = start++;
            }
        }
    }
}
END_NONMATCH

u32 sub_08122654(u32 unused, u32 x)
{
    return ((x - ((gUnk_02000008 & 0xC) << 12)) << 17) >> 22;
}

void release_obj_slots(struct StatsCtx *ctx)
{
    obj_release_slots(ctx->sprite30);
}

void update_sprite_obj(struct StatsCtx *ctx)
{
    sprite_update_cd0(ctx->sprite30);
}

u32 stub_return_0(void)
{
    return 0;
}

void set_tween_anim(struct StatsCtx *ctx, u8 idx, u32 anim, s32 arg)
{
    obj_load_record(ctx->tweens[idx].sprite, anim, arg, 0, 0);
}

void ensure_tween_sprite(struct StatsCtx *ctx, u8 idx, s32 val)
{
    struct TweenEntry *entry = &ctx->tweens[idx];
    void *old = entry->sprite;

    if (old == 0)
    {
        void *created = create_obj_de4((val >> 12) - 1, val, 1, -1, -1, -1, -1);
        entry->sprite = created;
        obj_load_record(created, 0, -1, 0, (u32)old);
    }
    else
    {
        setup_obj_10bc(old, (val >> 12) - 1, val, 1, -1, -1, -1, -1);
    }
}

u32 unknown_thumb_08122714(void)
{
    if (gUnk_03000C80[0] == 0)
    {
        return 1;
    }
    return 0;
}

void sub_08122730(struct StatsCtx *ctx, u8 idx, s32 val)
{
    ctx->tweens[idx].sprite = create_obj_de4((val >> 12) - 1, val, 1, -1, -1, -1, -1);
}

u32 sub_08122768(struct StatsCtx *ctx)
{
    u32 result;

    if ((u8)sub_obj_running(ctx->sub20) != 0)
    {
        result = 1;
    }
    else
    {
        ctx->flag156 = 0;
        result = 0;
    }
    return result;
}

void start_sub_obj(struct StatsCtx *ctx)
{
    sub_08115F54(ctx->sub20, 4, 0, 8, 0xFFFF, 0xFFFF, 0, 0);
    ctx->flag156 = 1;
}

void start_sub_obj_display(struct StatsCtx *ctx)
{
    u32 off = 0x884;

    *(s32 *)(gSys + off) = -1;
    sub_08115F54(ctx->sub20, 0, 0, 8, 0xFFFF, 0xFFFF, 0, 0);
    *(vu16 *)0x02000000 = ctx->displayShadow17E;
    ctx->flag156 = 1;
}

NONMATCH("asm/code_081218F0/unknown_thumb_08122818.s", void unknown_thumb_08122818(void *unused, s16 v, u8 idx))
{
    s32 t = v;
    u16 mag;

    if (v < 0)
    {
        t = -v;
    }
    mag = t;
    if (v <= 0)
    {
        *(u16 *)((u8 *)gUnk_02000044 + (idx << 1)) = mag;
    }
    else
    {
        *(u16 *)((u8 *)gUnk_02000044 + (idx << 1)) = ((0xA0 - mag) << 8) | 0xA0;
    }
}
END_NONMATCH

NONMATCH("asm/code_081218F0/draw_bro_levels.s", u32 draw_bro_levels(void *unused, u32 oam))
{
    oam = draw_decimal_sprites(8, gParty[0x30], oam, 0xA8, 0x38, 2, -1, -1);
    return draw_decimal_sprites(8, gParty[0x6C], oam, 0x48, 0x38, 2, -1, -1);
}
END_NONMATCH

void window_irq_callback(void)
{
    vu32 *dma = (vu32 *)REG_ADDR_DMA0SAD;
    vu16 *cntH = (vu16 *)REG_ADDR_DMA0CNT_H;

    *cntH = 0;
    dma[0] = gUnk_03000FE4;
    dma[1] = 0x04000040;
    dma[2] = 0xA2400001;
}

struct WindowObj *sub_081228E4(struct WindowObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->klass = gWindowChildClass;
    return obj;
}

ASM_FUNC("asm/code_081218F0/draw_decimal_sprites.s", u32 draw_decimal_sprites(u32 digits, u32 value, u32 oam, u32 x, u32 y, u32 mode, s32 a, s32 b))

ASM_FUNC("asm/code_081218F0/draw_number_tiles.s", void draw_number_tiles(void))

ASM_FUNC("asm/code_081218F0/text_sprites_b.s", void text_sprites_b(void))

ASM_FUNC("asm/code_081218F0/draw_member_status.s", void draw_member_status(void))

ASM_FUNC("asm/code_081218F0/draw_two_values.s", void draw_two_values(void))

ASM_FUNC("asm/code_081218F0/draw_stat.s", void draw_stat(void))
ASM_FUNC("asm/code_081218F0/draw_party_stats.s", void draw_party_stats(void))
