#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    u8 filler0[0x12];
    u8 flags;
};

struct SpriteFlags
{
    u8 filler0[0x12];
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 high : 3;
};

struct Figure
{
    u8 filler0[0x8];
    u8 sprite[0x8];
    struct Sprite *link;
    u8 filler14[0x7E - 0x14];
    u8 ctrl;
    u8 filler7F[0xEC - 0x7F];
    s16 anim;
};

struct DisplaySub
{
    u8 filler0[0x4];
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
};

struct DisplayEntry
{
    u8 filler0[0x4];
    void (*state)(struct DisplayEntry *);
    void *data;
    struct DisplaySub *sub;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 mode18;
};

struct Owner
{
    u8 filler0[0x4];
    void (*state)(struct Owner *);
    u32 unk8;
    struct Figure *fig;
    u16 timer;
    u16 index;
    s16 posY;
    s16 unk16;
    u16 phase18;
};


struct ListNode
{
    u8 filler0[0x4];
    u32 unk4;
};

struct World
{
    u8 filler0[0x07];
    u8 digitCtr;
    u8 spriteMode;
    u8 filler9[0x48 - 0x09];
    u8 list48[0x10];
    u8 filler58[0x80 - 0x58];
    struct Figure *fig[2];
    u8 filler88[0x250 - 0x88];
    void *slot4;
    u8 filler254[0x2D4 - 0x254];
    u16 timer2D4;
    u8 filler2D6[0x368 - 0x2D6];
    void *menuObj;
    void *obj36C;
    struct ListNode *digitObj;
    struct ListNode *listOut;
    void *spriteChain;
    void *scrollObj;
    struct ListNode *entry380[6];
};

extern struct World *gWorld;
extern u8 gSys[];
void sprite_set_action(void *obj, u32 action, u32 a2, u32 a3);
struct DisplayEntry *list_add_task(void *list, void (*state)(struct DisplayEntry *));
struct DisplaySub *sprite_node_alloc_default(void);
u32 to_bcd(u32 value);
void state_display_entry_draw(struct DisplayEntry *entry);
void state_spawn_list_entry(struct Owner *owner);
void state_show_hero_label(struct Owner *owner);
void state_apply_hero_stats(struct Owner *owner);
void draw_obj_at_xy(struct Owner *obj);
void sprite_node_release(struct DisplaySub *sub);
void draw_entry_flags(struct DisplayEntry *entry);
void draw_digits_c0_p12(u32 value, u32 x, s32 y, s32 right);
void draw_digits_c0_p11(u32 value, u32 x, s32 y, s32 right);
void draw_digits_49_p12(u32 value, u32 x, s32 y, s32 right);
void clear_map_rect(void *slot, s32 a, s32 b, s32 c, s32 d, s32 e);
void obj_release(void *obj);
void obj_release_node(void *node);
extern u8 *gParty;
extern const u8 gLevelExpTable[];
void ensure_code_entry(u32 id);
void alloc_code_entry(u32 id);
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void draw_digits_49_p11(s32 a, s32 b, s32 c, s32 d);
extern u8 gSys[];
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];

ASM_FUNC("asm/code_08102934/state_draw_rows.s", void state_draw_rows(void))

typedef struct { u16 value; } HalfArg;

NONMATCH("asm/code_08102934/create_display_entry.s", struct DisplayEntry *create_display_entry(void *data, u32 value, u32 stat, u32 mode, HalfArg s0, HalfArg s1, HalfArg s2, HalfArg s3, u32 s4, u32 s5, u32 s6, u32 s7, s32 width))
{
    struct DisplayEntry *entry;
    struct DisplaySub *sub;
    u32 bcdA;
    u32 bcdB;
    s32 k1FF = 0x1FF;
    u32 arg34;
    u32 kHi;

    entry = list_add_task(gWorld->list48, state_display_entry_draw);
    sub = sprite_node_alloc_default();
    bcdA = to_bcd(value);
    bcdB = to_bcd(stat);
    entry->data = data;
    entry->sub = sub;
    entry->mode18 = mode;
    sub->unk16 = value;
    sub->unk4 = bcdA;
    sub->unk8 = stat;
    sub->unkC = bcdB;
    sub->unk10 = s0.value;
    entry->unk10 = s1.value;
    sub->unk12 = s2.value;
    sub->unk14 = s3.value;
    kHi = 0xFFFFF000;
    arg34 = s4 & k1FF;
    sub->unk1A = (sub->unk1A & kHi) | arg34;
    sub->unk18 = s5 & 0xFF;
    entry->unk12 = s6 & k1FF;
    entry->unk14 = s7 & 0xFF;
    if (width == 0xFFFFFB2E)
    {
        width = s4 - 8;
        do
        {
            bcdA = (s32)bcdA >> 4;
            width -= 8;
        } while (bcdA != 0);
    }
    entry->unk16 = width & 0x1FF;
    gWorld->timer2D4 = 0;
    return entry;
}
END_NONMATCH

NONMATCH("asm/code_08102934/state_display_entry_draw.s", void state_display_entry_draw(struct DisplayEntry *entry))
{
    struct DisplaySub *sub;
    void *data;
    s32 src;
    s16 mode;
    s32 limit;
    s32 subX;
    s32 subH;
    s32 dataY;
    s32 drawW;
    s32 pos;
    s32 w;
    s32 bit;
    u32 s0;
    u32 s4;
    u32 s8;
    u32 sC;
    u32 s10;
    u32 s14;
    u32 s18;
    u32 s1C;
    u32 s20;
    u8 *queue;
    u8 *sysb;

    data = entry->data;
    sub = entry->sub;
    src = (s16)sub->unk16;
    s0 = sub->unk4;
    s4 = sub->unk8;
    drawW = sub->unkC;
    s8 = sub->unk10 & 0xFFF;
    sC = entry->unk10 & 0xFFF;
    mode = (s16)entry->mode18;
    if (mode > 0)
    {
        limit = mode;
    }
    else if (mode == -2)
    {
        limit = 0x9999;
    }
    else
    {
        limit = 0x999999;
    }
    subX = (s16)sub->unk12;
    subH = (s16)sub->unk14;
    s10 = ((u32)sub->unk1A << 20) >> 20;
    s14 = (s16)sub->unk18;
    s18 = (s16)entry->unk12;
    s1C = (s16)entry->unk14;
    s20 = (s16)entry->unk16;
    dataY = *(s16 *)((u8 *)data + 0x10);
    if (dataY < subX + subH)
    {
        if (subX < dataY)
        {
            pos = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((dataY - subX) * src, subH);
            s0 = to_bcd(src - pos);
            w = to_bcd(s4 + pos);
            drawW = w;
            if (drawW > limit)
            {
                drawW = limit;
            }
            if (*(u16 *)((u8 *)gWorld + 0x2D4) != pos)
            {
                *(u16 *)((u8 *)gWorld + 0x2D4) = pos;
            }
        }
        if (dataY < (s32)s8)
        {
            bit = ((s32)(sub->unk10 << 16) >> 28) & 1;
            draw_digits_c0_p12(s0, s10, s14, bit);
        }
        else
        {
            bit = ((s32)(sub->unk10 << 16) >> 28) & 1;
            draw_digits_c0_p11(s0, s10, s14, bit);
        }
        if (dataY < (s32)sC)
        {
            bit = ((s32)(entry->unk10 << 16) >> 28) & 1;
            draw_digits_49_p12(drawW, s18, s1C, bit);
        }
        else
        {
            bit = ((s32)(entry->unk10 << 16) >> 28) & 1;
            draw_digits_49_p11(drawW, s18, s1C, bit);
        }
        if (dataY < (s32)s8)
        {
            sysb = gSys;
            queue = *(u8 **)(sysb + 0x880);
            *(u16 *)queue = (s14 + 6) & 0xFF;
            *(u16 *)(queue + 2) = (u16)s20;
            *(u16 *)(queue + 4) = 0xC0D4;
            *(u8 **)(sysb + 0x880) = queue + 8;
        }
        else
        {
            sysb = gSys;
            queue = *(u8 **)(sysb + 0x880);
            *(u16 *)queue = (s14 + 6) & 0xFF;
            *(u16 *)(queue + 2) = (u16)s20;
            *(u16 *)(queue + 4) = 0xB0D4;
            *(u8 **)(sysb + 0x880) = queue + 8;
        }
    }
    else
    {
        sprite_node_release(sub);
        w = to_bcd(s4 + src);
        drawW = w;
        if (drawW > limit)
        {
            drawW = limit;
        }
        entry->sub = (struct DisplaySub *)drawW;
        entry->state = draw_entry_flags;
        entry->state(entry);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_08102934/state_create_display_entries.s", void state_create_display_entries(void))

NONMATCH("asm/code_08102934/obj_apply_world_mode.s", void obj_apply_world_mode(struct Figure *fig))
{
    u32 mode;
    u32 bits;
    s32 kA;
    s32 kB;
    kA = 0x2003;
    kB = 0x2050;

    if (fig == 0)
    {
        return;
    }
    if ((fig->ctrl & 6) != 2)
    {
        return;
    }
    bits = (u32)gWorld->spriteMode << 28;
    mode = (bits >> 28) & 3;
    if (mode == 0)
    {
        mode = (bits >> 30) & 3;
        if (mode == 0)
        {
            mode = 1;
        }
    }
    if (fig != 0 && (fig->ctrl & 6) == 2)
    {
        if (fig->anim == -1)
        {
            sprite_set_action(fig->sprite, mode, kA, 0);
        }
        else
        {
            sprite_set_action(fig->sprite, mode, kB, 0);
        }
        ((struct SpriteFlags *)fig->link)->bit4 = 0;
        ((struct SpriteFlags *)fig->link)->mode = 0;
    }
    ((struct SpriteFlags *)fig->link)->mode = 1;
}
END_NONMATCH

ASM_FUNC("asm/code_08102934/state_result_wait_confirm.s", void state_result_wait_confirm(void))

ASM_FUNC("asm/code_08102934/state_assign_figure.s", void state_assign_figure(void))

void state_show_hero_label(struct Owner *owner)
{
    u32 msg;
    u16 *glyphs;
    u32 *maps;
    u32 idx;

    ensure_code_entry(0x7003);
    alloc_code_entry(0x7003);
    switch (gSys[0x8C0])
    {
    case 0:
        msg = 0x7004;
        break;
    case 1:
        msg = 0x7005;
        break;
    case 2:
        msg = 0x7006;
        break;
    case 3:
        msg = 0x7007;
        break;
    case 4:
        msg = 0x7008;
        break;
    }
    ensure_code_entry(msg);
    alloc_code_entry(msg);
    glyphs = gGlyphIndexTable;
    idx = glyphs[1] << 2;
    maps = gMapTable;
    queue_display_entry(2, (u8 *)maps + *(u32 *)((u8 *)maps + idx), (void *)0x06008000, 0x920, 0, 0);
    idx = gSys[0x8C0];
    idx += 2;
    queue_display_entry(2, (u8 *)maps + maps[glyphs[idx]], (void *)0x06008920, 0x520, 0, 0);
    owner->timer = 0;
    owner->phase18 = 3;
    owner->state = state_spawn_list_entry;
}

ASM_FUNC("asm/code_08102934/state_figure_jump_start.s", void state_figure_jump_start(void))

ASM_FUNC("asm/code_08102934/task_countdown_digits.s", void task_countdown_digits(void))

ASM_FUNC("asm/code_08102934/state_digits_slide.s", void state_digits_slide(void))
