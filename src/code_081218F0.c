#include "global.h"

struct SpriteObj;

// A sprite a menu screen moves around (ScreenView.tweens[12], 0x40 bytes each).
struct TweenEntry
{
    struct SpriteObj *sprite;
    // 0x04-0x13: copy of the sprite's drawing state (unknown_thumb_08121e42)
    u16 savedX;
    u16 savedY;
    u16 savedAttr2; // tile | (palette + 1) << 12
    u16 savedUnkA; // copy of unk3A
    u8 savedUnk24;
    u8 savedUnk25;
    u8 savedUnk3D;
    u8 savedUnk20;
    u16 savedBits; // struct TweenSavedBits16 / TweenSavedBits8
    u16 savedUnkE;
    s32 x; // position, 24.8 fixed point
    s32 y;
    s32 startX;
    s32 startY;
    s32 endX;
    s32 endY;
    s16 velX; // per-frame step in velocity mode
    s16 velY;
    u16 frame;
    u16 duration; // frames from start to end in interpolate mode
    u8 filler34[0x36 - 0x34];
    u16 unk36;
    u16 unk38;
    u16 unk3A;
    u8 flags3C; // struct TweenFlags
    u8 unk3D;
    u8 filler3E[2];
};

// TweenEntry.flags3C
struct TweenFlags
{
    u8 active : 1; // drawn each frame; its sprite's slots are released while clear
    u8 mode : 2; // 0: idle, 2: interpolate start->end over duration, else: step by vel until end
    u8 rest : 5;
} __attribute__((packed));

// A TweenEntry seen through the bitfields of its savedBits, read and written
// as a halfword
struct TweenSavedBits16
{
    u8 filler0[0x10];
    u16 active : 1; // TweenFlags.active
    u16 flag12_4 : 1; // SpriteObj.flags12 bit 4
    u16 flag11_6 : 1; // SpriteObj.flags11 bit 6
    u16 bits1F_0 : 2; // the four 2-bit fields of SpriteObj.bits1F
    u16 bits1F_2 : 2;
    u16 bits1F_4 : 2;
    u16 bits1F_6 : 2;
};

// The same, written a byte at a time
struct TweenSavedBits8
{
    u8 filler0[0x10];
    u8 active : 1;
    u8 flag12_4 : 1;
    u8 flag11_6 : 1;
    u8 bits1F_0 : 2;
    u8 bits1F_2 : 2;
    u16 bits1F_4 : 2;
    u8 bits1F_6 : 2;
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

struct SpriteObj
{
    u16 oam0; // x
    u16 oam1; // y
    u8 filler4[0xE - 4];
    u16 unkE;
    u8 filler10;
    u8 flags11; // struct SprFlags11Word
    u8 flags12; // struct SprFlags12 / SprFlags12Word
    u8 filler13[0x16 - 0x13];
    u16 tile16;
    u8 filler18[0x1E - 0x18];
    u8 palette1E; // low 4 bits: palette
    u8 bits1F; // struct SprBits1F
    u8 unk20;
    u8 filler21[0x24 - 0x21];
    u8 unk24;
    u8 unk25;
};

struct SprFlags11Word
{
    u32 b0_5 : 6;
    u32 b6 : 1;
    u32 b7 : 1;
} __attribute__((packed));

struct SprFlags12Word
{
    u32 b0_3 : 4;
    u32 b4 : 1;
    u32 b5_7 : 3;
} __attribute__((packed));

struct SprBits1F
{
    u32 f0 : 2;
    u32 f2 : 2;
    u32 f4 : 2;
    u32 f6 : 2;
} __attribute__((packed));

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
    u8 *waveSrc;
    void *item;
    u8 filler40[0x44 - 0x40];
    struct SpriteObj *sideL;
    struct SpriteObj *sideR;
    void *texts[2];
    void *objE;
    void *four[4];
    void *seven[7];
    u8 filler84[0x88 - 0x84];
    struct SlideObj *slide;
    u8 filler8C[0x14A - 0x8C];
    u8 slideMax;
    u8 slideStep;
    s8 slideMin;
    u8 blendLo;
    u8 blendHi;
    u8 mode;
    u8 filler150;
    u8 entryMark;
    u8 transPending;
    u8 filler153;
    u8 transActive;
    u8 filler155;
    u8 waveActive;
    u8 filler157;
    struct TweenEntry *tweens;
    u8 filler15C[0x168 - 0x15C];
    u8 waveTimer;
    u8 filler169[0x188 - 0x169];
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

// ScreenView.mode: which page of the screen is shown, named after the function
// that draws it.
enum ScreenMode
{
    SCREEN_MODE_PARTY_OVERVIEW = 0,
    SCREEN_MODE_CURSOR_ROW = 1,
    SCREEN_MODE_SCROLL_LIST = 2,
    SCREEN_MODE_MEMBER_STATUS = 3,
    SCREEN_MODE_GRID = 4,
};

// SlideObj.state: odd while the slider moves, even while it rests.
enum SlideState
{
    SLIDE_MOVE_LEFT = 1,
    SLIDE_MOVE_RIGHT = 3,
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

extern const u8 gWindowWaveTable[];
extern u16 gUnk_02000050[];
extern u16 gUnk_02000052;
extern u16 gUnk_02000012;
/* One frame of the screen's window wave (drawn while waveFlag is set). The I/O register shadows at
   0x02000000 get: WIN0V (gUnk_02000044) top = gWindowWaveTable[frame of waveSrc] down to line 160;
   every 5th frame BLDALPHA (gUnk_02000052) one more step of EVB (blendHi, up to 8); and BG0VOFS
   (gUnk_02000012) 0xE0, which after 16 frames speeds up on a parabola. After 26 frames the wave
   ends: BG0CNT (gUnk_02000008) = 0x1A08 and BLDCNT/BLDALPHA (gUnk_02000050) cleared. Returns the
   frame mod 5. */
u32 screen_wave_frame(struct ScreenView *screen)
{
    u8 wave[20];
    s32 scroll_y;
    u8 *frame;
    u8 *counter;
    u16 *blend;
    u16 win0v;
    u32 step;
    register u32 phase asm("r0");

    mem_copy_fast(wave, gWindowWaveTable, 0x14);
    scroll_y = 0xE0;
    frame = screen->waveSrc + 0x23;
    win0v = (wave[*frame] << 8) | 0xA0;
    *gUnk_02000044 = win0v;
    step = *frame;
    step %= 5;
    phase = step;
    if ((u8)step == 0)
    {
        counter = &screen->blendHi;
        if (++*counter > 8)
            *counter = 8;
        gUnk_02000052 = (*counter << 8) | screen->blendLo;
    }
    counter = &screen->waveTimer;
    if (++*counter > 0x10)
    {
        scroll_y = (*counter - 0x10) * (*counter - 0x10) + 0xE0;
        if (scroll_y > 0x60 && *counter > 0x1A)
        {
            screen->waveActive = 0;
            gUnk_02000008 = 0x1A08;
            scroll_y = 0;
            blend = gUnk_02000050;
            *blend++ = 0;
            *blend = 0;
        }
    }
    gUnk_02000012 = scroll_y;
    return phase;
}

ASM_FUNC("asm/code_081218F0/tween_move_to.s", void tween_move_to(void))

ASM_FUNC("asm/code_081218F0/tween_step.s", void tween_step(void))

// Copies the drawing state of each of the screen's 12 tweens' sprites (position,
// tile and palette, flag bits) into the tween's saved fields.
void unknown_thumb_08121e42(struct ScreenView *screen)
{
    s32 i;
    u8 bits;

    for (i = 0; i < 12; i++)
    {
        if (screen->tweens[i].unk3D != 0)
        {
            screen->tweens[i].savedX = screen->tweens[i].sprite->oam0;
            screen->tweens[i].savedY = screen->tweens[i].sprite->oam1;
            screen->tweens[i].savedAttr2 = ((((u32)screen->tweens[i].sprite->palette1E << 28) >> 16) + 0x1000) | screen->tweens[i].sprite->tile16;
            screen->tweens[i].savedUnkA = screen->tweens[i].unk3A;
            screen->tweens[i].savedUnk24 = screen->tweens[i].sprite->unk24;
            screen->tweens[i].savedUnk25 = screen->tweens[i].sprite->unk25;
            screen->tweens[i].savedUnk3D = screen->tweens[i].unk3D;
            screen->tweens[i].savedUnk20 = screen->tweens[i].sprite->unk20;
            ((struct TweenSavedBits8 *)&screen->tweens[i])->active = ((struct TweenFlags *)&screen->tweens[i].flags3C)->active;
            ((struct TweenSavedBits16 *)&screen->tweens[i])->flag12_4 = ((struct SprFlags12Word *)&screen->tweens[i].sprite->flags12)->b4;
            ((struct TweenSavedBits16 *)&screen->tweens[i])->flag11_6 = ((struct SprFlags11Word *)&screen->tweens[i].sprite->flags11)->b6;
            bits = ((struct SprBits1F *)&screen->tweens[i].sprite->bits1F)->f0 & 3;
            ((struct TweenSavedBits8 *)&screen->tweens[i])->bits1F_0 = bits;
            bits = ((struct SprBits1F *)&screen->tweens[i].sprite->bits1F)->f2 & 3;
            ((struct TweenSavedBits8 *)&screen->tweens[i])->bits1F_2 = bits;
            ((struct TweenSavedBits16 *)&screen->tweens[i])->bits1F_4 = ((struct SprBits1F *)&screen->tweens[i].sprite->bits1F)->f4 & 3;
            bits = ((struct SprBits1F *)&screen->tweens[i].sprite->bits1F)->f6 & 3;
            ((struct TweenSavedBits8 *)&screen->tweens[i])->bits1F_6 = bits;
            screen->tweens[i].savedUnkE = screen->tweens[i].sprite->unkE;
        }
    }
}

struct SysView
{
    u8 filler0[0x880];
    u32 oam880;
    u8 filler884[0x888 - 0x884];
    u8 flags888;
    u8 filler889[0x8C0 - 0x889];
    u8 lang8C0;
};

#define SYS_VIEW ((struct SysView *)gSys)

#define SYS_FLAG_NO_DRAW 0x10 // SysView.flags888: skip drawing the screen this frame

struct SprFlags12
{
    u8 b0 : 1;
    u8 b12 : 2;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b67 : 2;
};

/* Draws the level of both brothers (party entries 0x3C bytes apart). */
static inline u32 bro_levels_draw(u32 oam)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        oam = draw_decimal_sprites(8, gParty[0x30 + i * 0x3C], oam, 0xA8 - i * 0x60, 0x38, 2, -1, -1);
    }
    return oam;
}

// Per-frame update of the menu screen: steps transitions, slides the slider,
// repositions the current page's sprites, then draws the page into OAM.
void screen_view_update(struct ViewOwner *owner)
{
    struct ScreenView *screen;
    u16 langTiles[5];
    u32 oam;
    s32 i;
    s32 j;
    s32 k;
    u32 flag222a;
    u32 flag220;
    u32 flag222b;

    mem_copy_fast(langTiles, gScreenLangTileTable, 0xA);
    screen = owner->screen;
    oam = SYS_VIEW->oam880;
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
    if (!(SYS_VIEW->flags888 & SYS_FLAG_NO_DRAW))
    {
        if (screen->slide->state == SLIDE_MOVE_LEFT)
        {
            if (!(screen->cursor->flags12 & 0x20))
            {
                obj_release_slots(screen->cursor);
            }
            obj_release_slots(screen->sideL);
            obj_release_slots(screen->sideR);
            screen->slider->oam1 -= screen->slideStep;
            if (screen->slideMin > (s16)screen->slider->oam1)
            {
                screen->slider->oam1 = screen->slideMin;
            }
        }
        else if (screen->slide->state == SLIDE_MOVE_RIGHT)
        {
            obj_release_slots(screen->cursor);
            obj_release_slots(screen->sideL);
            obj_release_slots(screen->sideR);
            screen->slider->oam1 += screen->slideStep;
            if (screen->slideMax < (s16)screen->slider->oam1)
            {
                screen->slider->oam1 = screen->slideMax;
            }
        }

        if (screen->transActive != 0)
        {
            switch (screen->mode)
            {
            case SCREEN_MODE_CURSOR_ROW:
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
            case SCREEN_MODE_SCROLL_LIST:
                obj_load_record(screen->item, screen->page + 1, 0, 0, 0);
                sprite_update_cd0(screen->item);
                screen->cursor->oam0 = 0x68;
                flag222a = test_flag(gUnk_03000FC0 + 0x38C, 0, 0x222);
                flag220 = test_flag(gUnk_03000FC0 + 0x38C, 0, 0x220);
                flag222b = test_flag(gUnk_03000FC0 + 0x38C, 0, 0x222);
                {
                    s32 left = 0x2C;

                    screen->cursor->oam1 = left - (u8)flag222a * 8 - (u8)flag220 * 16 + screen->category * (0x20 - (u8)flag222b * 8);
                }
                sprite_update_cd0(screen->cursor);
                break;
            case SCREEN_MODE_MEMBER_STATUS:
                obj_load_record(screen->item, screen->member + 1, 0, 0, 0);
                sprite_update_cd0(screen->item);
                break;
            case SCREEN_MODE_GRID:
                for (i = 0; i < 2; i++)
                {
                    if (screen->texts[i] != 0)
                    {
                        sprite_update_cd0(screen->texts[i]);
                    }
                }
                if (screen->objE != 0)
                {
                    sprite_update_cd0(screen->objE);
                }
                for (j = 0; j < 4; j++)
                {
                    if (screen->four[j] != 0)
                    {
                        sprite_update_cd0(screen->four[j]);
                    }
                }
                for (k = 0; k < 7; k++)
                {
                    if (screen->seven[k] != 0)
                    {
                        sprite_update_cd0(screen->seven[k]);
                    }
                }
                if (screen->pick != 0xFF)
                {
                    setup_obj_10bc(screen->cursor, 7, langTiles[SYS_VIEW->lang8C0], 1, -1, -1, -1, -1);
                    obj_load_record(screen->cursor, screen->pick, -1, 0, 0);
                    sprite_update_cd0(screen->cursor);
                    screen->cursor->oam0 = 0xDA;
                    screen->cursor->oam1 = 0xA;
                }
                break;
            case SCREEN_MODE_PARTY_OVERVIEW:
                if (screen->cursor->tile16 == (langTiles[SYS_VIEW->lang8C0] & 0xFFF)
                    && screen->pick != 0xFF)
                {
                    setup_obj_10bc(screen->cursor, 5, 0x6000, 1, -1, -1, -1, -1);
                }
                obj_load_record(screen->cursor, 0, -1, 0, 0);
                screen->cursor->oam0 = 0x18;
                screen->cursor->oam1 = screen->cursorIdx * 0x18 + 0x46 - screen->count * 8;
                sprite_update_cd0(screen->cursor);
                ((struct SprFlags12 *)&screen->cursor->flags12)->b4 = 0;
                ((struct SprFlags12 *)&screen->cursor->flags12)->b12 = 0;
                screen->sideL->oam0 = 0xCC;
                screen->sideR->oam0 = 0x6C;
                screen->sideL->oam1 = 0x50;
                screen->sideR->oam1 = 0x50;
                sprite_update_cd0(screen->sideL);
                sprite_update_cd0(screen->sideR);
                ((struct SprFlags12 *)&screen->slider->flags12)->b4 = 0;
                break;
            }
        }

        if (screen->mode == SCREEN_MODE_PARTY_OVERVIEW && !(screen->slide->state % 2) && screen->waveFlag == 0)
        {
            oam = bro_levels_draw(oam);
        }
        if (screen->mode == SCREEN_MODE_CURSOR_ROW && !(screen->slide->state % 2))
        {
            oam = obj_draw_list(screen, oam);
        }
        if (screen->mode == SCREEN_MODE_GRID && !(screen->slide->state % 2))
        {
            oam = screen_helpers_obj_draw_grid(screen, oam);
        }
        if (screen->entryMark != 0xFF)
        {
            for (j = 0; j < 12; j++)
            {
                if (screen->tweens[j].sprite != 0)
                {
                    if (screen->tweens[j].flags3C & 1)
                    {
                        sprite_update_cd0(screen->tweens[j].sprite);
                    }
                    else
                    {
                        obj_release_slots(screen->tweens[j].sprite);
                    }
                }
            }
        }

        update_obj_list();
        obj_list_pass_0a8c();
        gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
        oam = gUnk_03000D4C(gUnk_0203FFB8.root, oam);
        if (!(screen->slide->state % 2))
        {
            switch (screen->mode)
            {
            case SCREEN_MODE_PARTY_OVERVIEW:
                oam = draw_party_overview(screen, oam);
                break;
            case SCREEN_MODE_CURSOR_ROW:
                oam = draw_cursor_row(screen, oam);
                break;
            case SCREEN_MODE_SCROLL_LIST:
                oam = obj_draw_scroll_list(screen, oam);
                break;
            case SCREEN_MODE_MEMBER_STATUS:
                oam = draw_member_status_page(screen, oam);
                break;
            }
        }
        SYS_VIEW->oam880 = oam;
    }
    if (screen->waveFlag != 0)
    {
        screen_wave_frame(screen);
    }
}

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

void unknown_thumb_08122818(void *unused, s16 v, u8 idx)
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
        ((u16 *)gUnk_02000044)[idx] = mag;
    }
    else
    {
        u32 off = idx << 1;
        register u32 base asm("r1") = (u32)gUnk_02000044;
        u16 *p = (u16 *)(off + base);
        register u32 w asm("r1");

        w = ((0xA0 - mag) << 8) | 0xA0;
        *p = w;
    }
}

u32 draw_bro_levels(void *unused, u32 oam)
{
    return bro_levels_draw(oam);
}

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
