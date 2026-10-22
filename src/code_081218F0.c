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

ASM_FUNC("asm/code_081218F0/screen_wave_frame.s", void screen_wave_frame(void))

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

ASM_FUNC("asm/code_081218F0/screen_view_update.s", void screen_view_update(void))

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
