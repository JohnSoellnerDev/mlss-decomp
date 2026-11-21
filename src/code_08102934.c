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

struct OamAffineParams
{
    u8 filler0[0x6];
    vu16 pa : 16;
    u8 filler8[0x6];
    vu16 pb : 16;
    u8 filler10[0x6];
    vu16 pc : 16;
    u8 filler18[0x6];
    vu16 pd : 16;
};

struct WorldDigitBits
{
    u8 filler0[0x7];
    u8 digitCtr : 5;
    u8 digitHi : 3;
};

extern struct World *gWorld;
extern u8 gSys[];

/* gSys offsets: two 1 KiB OAM buffers, the bit picking the one being filled, and the write
 * cursor of the OAM queue (a u8 * to the next free entry). */
#define SYS_OAM_BUFFER 0x80
#define SYS_OAM_BANK 0x888
#define SYS_OAM_CURSOR 0x880
/* gSys[SYS_LANGUAGE]: the game's language, 0-4 */
#define SYS_LANGUAGE 0x8C0
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
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void copy_map_rect_add(void *src, u32 a, u32 b, u32 c, u32 d, u32 e, void *f, u32 g);
void state_assign_figure(struct Owner *owner);
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
    u8 **qp;
    u8 *sysb;
    s32 val;
    u16 k;

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
            val = (s14 + 6) & 0xFF;
            sysb = gSys;
            queue = *(u8 **)(sysb + 0x880);
            *(u16 *)queue = val;
            *(u16 *)(queue + 2) = (u16)s20;
            k = 0xC0D4;
            *(u8 **)(sysb + 0x880) = queue + 8;
        }
        else
        {
            val = (s14 + 6) & 0xFF;
            sysb = gSys;
            queue = *(u8 **)(sysb + 0x880);
            *(u16 *)queue = val;
            *(u16 *)(queue + 2) = (u16)s20;
            k = 0xB0D4;
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

static inline void fig_world_action(struct Figure *fig, u32 mode)
{
    if (fig != 0 && (fig->ctrl & 6) == 2)
    {
        if (fig->anim == -1)
            sprite_set_action(fig->sprite, mode, 0x2003, 0);
        else
            sprite_set_action(fig->sprite, mode, 0x2050, 0);
        ((struct SpriteFlags *)fig->link)->bit4 = 0;
        ((struct SpriteFlags *)fig->link)->mode = 0;
    }
    ((struct SpriteFlags *)fig->link)->mode = 1;
}

/* For a figure in play (ctrl bits 1-2 == 2), plays art 0x2003 (party member 0, anim -1) or 0x2050
   (member 1) as the action that World.spriteMode picks: its bits 0-1, else bits 2-3, else 1.
   The figure's sprite is then shown in mode 1 (bit 4 cleared). */
void obj_apply_world_mode(struct Figure *fig)
{
    u32 modeBits;
    u32 action;
    u32 lowNibble;
    u32 fieldMask;
    s32 artMember0;
    s32 artMember1;

    artMember0 = 0x2003;
    artMember1 = 0x2050;
    if (fig == 0)
        return;
    if ((fig->ctrl & 6) != 2)
        return;
    modeBits = ((u32)gWorld->spriteMode) << 28;
    lowNibble = modeBits >> 28;
    action = 3;
    action &= lowNibble;
    fieldMask = 3;
    if (action == 0)
    {
        action = (modeBits >> 30) & fieldMask;
        if (action == 0)
            action = 1;
    }
    if (fig != 0 && (fig->ctrl & 6) == 2)
    {
        if (fig->anim == -1)
            sprite_set_action(fig->sprite, action, artMember0, 0);
        else
            sprite_set_action(fig->sprite, action, artMember1, 0);
        ((struct SpriteFlags *)fig->link)->bit4 = 0;
        ((struct SpriteFlags *)fig->link)->mode = 0;
    }
    ((struct SpriteFlags *)fig->link)->mode = 1;
}

static inline u32 lang_text(u32 base)
{
    u8 lang;
    u32 row;

    lang = gSys[0x8C0];
    if (lang == 2)
    {
        row = 0x42;
    }
    else if (lang == 3)
    {
        row = 0x44;
    }
    else
    {
        row = 0x40;
        if (lang == 4)
        {
            row = 0x46;
        }
    }
    return gMapTable[*(u16 *)((u8 *)gGlyphIndexTable + row)] + (u32)gMapTable + base;
}

struct PartyMember
{
    u8 filler0[0x4];
    u32 exp : 24;
    u32 hi : 8;
    u8 filler8[0x3C - 0x8];
};

struct SysKeys
{
    u8 filler0[0x3E];
    u16 pressed;
};

static inline void party_add_score(struct Figure *fig)
{
    struct PartyMember *m;

    m = &((struct PartyMember *)gParty)[~fig->anim];
    m->exp += *(u16 *)((u8 *)gWorld + 0x16);
    if (m->exp > 999999U)
    {
        m->exp = 999999;
    }
}

void state_result_wait_confirm(struct Owner *owner)
{
    void *text;
    u16 *coins;
    struct Figure *fig;
    s32 i;

    if (gWorld->fig[1] != 0)
    {
        if ((gWorld->fig[0]->ctrl & 6) == 2 && (s16)owner->timer == 0x5A)
        {
            set_slot_value(0x79, -1);
            obj_apply_world_mode(gWorld->fig[0]);
            copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6,
                (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x8000);
        }
        if ((gWorld->fig[1]->ctrl & 6) == 2 && (s16)owner->timer == 0x5A)
        {
            obj_apply_world_mode(gWorld->fig[1]);
            copy_map_rect_add(gWorld->slot4, 1, 7, 0xB, 7, 6,
                (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable) + 0x180, 0x8000);
        }
        if ((s16)owner->timer == 0x5A)
        {
            copy_map_rect_add(gWorld->slot4, 1, 0x15, 0xD, 7, 4,
                ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x21C, 0x8000);
        }
    }
    else
    {
        if ((s16)owner->timer == 0x28)
        {
            set_slot_value(0x79, -1);
            obj_apply_world_mode(gWorld->fig[0]);
            if (gWorld->fig[0]->anim == -1)
                text = (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable);
            else
                text = (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable) + 0x180;
            copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6, text, 0x8000);
        }
        if ((s16)owner->timer == 0x3C)
        {
            copy_map_rect_add(gWorld->slot4, 1, 0x15, 0xD, 7, 4,
                ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x21C, 0x8000);
        }
    }

    if ((s16)owner->timer <= 0x6D)
    {
        if (((struct SysKeys *)gSys)->pressed & 1)
        {
            if (gWorld->fig[1] != 0)
            {
                if ((gWorld->fig[0]->ctrl & 6) == 2 && (s16)owner->timer <= 0x59)
                {
                    obj_apply_world_mode(gWorld->fig[0]);
                    copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6,
                        (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x8000);
                }
                if ((gWorld->fig[1]->ctrl & 6) == 2 && (s16)owner->timer <= 0x59)
                {
                    obj_apply_world_mode(gWorld->fig[1]);
                    copy_map_rect_add(gWorld->slot4, 1, 7, 0xB, 7, 6,
                        (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable) + 0x180, 0x8000);
                }
                if ((s16)owner->timer <= 0x59)
                {
                    copy_map_rect_add(gWorld->slot4, 1, 0x15, 0xD, 7, 4,
                        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x21C, 0x8000);
                }
            }
            else
            {
                if ((s16)owner->timer <= 0x27)
                {
                    obj_apply_world_mode(gWorld->fig[0]);
                    if (gWorld->fig[0]->anim == -1)
                        text = (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable);
                    else
                        text = (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable) + 0x180;
                    copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6, text, 0x8000);
                }
                if ((s16)owner->timer <= 0x3B)
                {
                    copy_map_rect_add(gWorld->slot4, 1, 0x15, 0xD, 7, 4,
                        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x21C, 0x8000);
                }
            }
            owner->timer = 0x6E;
        }
        else
        {
            owner->timer++;
        }
    }
    else
    {
        sound_stop_song(0x79);
        if (((((struct SysKeys *)gSys)->pressed ^ 1) & 1) == 0)
        {
            coins = (u16 *)(gParty + 0x88);
            *coins += *(u16 *)((u8 *)gWorld + 0x18);
            if (*coins > 9999)
                *coins = 9999;
            for (i = 0; i < 2; i++)
            {
                fig = gWorld->fig[i];
                if (fig != 0 && (fig->ctrl & 6) == 2)
                    party_add_score(fig);
            }
            owner->timer = 0;
            owner->state = state_assign_figure;
        }
    }
}

ASM_FUNC("asm/code_08102934/state_assign_figure.s", void state_assign_figure(struct Owner *owner))

/* Loads this screen's graphics: sprite set 0x7003 plus the one for the current language
 * (0x7004-0x7008), and queues two compressed BG tile uploads to VRAM: archive entry 1 (0x920
 * bytes at 0x06008000) and the language's entry 2-6 right after it. Then sets phase 3 and moves
 * on to state_spawn_list_entry. */
void state_show_hero_label(struct Owner *owner)
{
    u32 langSpriteSet;
    u16 *archiveIndex;
    u32 *archive;
    u32 entry;

    ensure_code_entry(0x7003);
    alloc_code_entry(0x7003);
    switch (gSys[SYS_LANGUAGE])
    {
    case 0:
        langSpriteSet = 0x7004;
        break;
    case 1:
        langSpriteSet = 0x7005;
        break;
    case 2:
        langSpriteSet = 0x7006;
        break;
    case 3:
        langSpriteSet = 0x7007;
        break;
    case 4:
        langSpriteSet = 0x7008;
        break;
    }
    ensure_code_entry(langSpriteSet);
    alloc_code_entry(langSpriteSet);

    /* gMapTable holds offsets (from its own start) to compressed data blocks */
    archiveIndex = gGlyphIndexTable;
    entry = archiveIndex[1] << 2;
    archive = gMapTable;
    queue_display_entry(2, (u8 *)archive + *(u32 *)((u8 *)archive + entry), (void *)0x06008000, 0x920, 0, 0);
    entry = gSys[SYS_LANGUAGE];
    entry += 2;
    queue_display_entry(2, (u8 *)archive + archive[archiveIndex[entry]], (void *)0x06008920, 0x520, 0, 0);

    owner->timer = 0;
    owner->phase18 = 3;
    owner->state = state_spawn_list_entry;
}

ASM_FUNC("asm/code_08102934/state_figure_jump_start.s", void state_figure_jump_start(void))

void state_digits_slide(struct Owner *obj);
/* While obj->timer runs down, shows the digit sprite at 4x size at the centre of the screen
 * ((0x68 or 0x78, 0x44), two affine 32x32 objects) beside the plain digits; when it runs out it
 * restarts the timer at 0xC0 and hands over to state_digits_slide. With the timer already at 0
 * it only draws the digits. */
void task_countdown_digits(struct Owner *obj)
{
    s32 affineIndex;
    u32 bank;
    u32 off;
    struct OamAffineParams *affine;
    u8 *queue;
    u8 *sys;

    if ((s16)obj->timer == 0)
    {
        draw_digits_49_p11((s16)obj->unk16, (s16)obj->index + 8, (s16)obj->posY - 8, 0);
    }
    else
    {
        obj->timer--;
        if ((s16)obj->timer != 0)
        {
            affineIndex = ((struct WorldDigitBits *)gWorld)->digitCtr--;
            bank = ((u32)gSys[SYS_OAM_BANK] << 28) >> 31;
            bank <<= 10;
            off = (affineIndex << 5) + (u32)(gSys + SYS_OAM_BUFFER);
            affine = (struct OamAffineParams *)(bank + off);
            /* 4x zoom */
            affine->pa = 0x40;
            affine->pb = 0;
            affine->pc = 0;
            affine->pd = 0x40;

            /* two affine, double-size 32x32 objects sharing the affine slot */
            sys = gSys;
            queue = *(u8 **)(sys + SYS_OAM_CURSOR);
            *(u16 *)(queue + 8) = (0x44 - 0x20) | 0x300;
            *(u16 *)queue = (0x44 - 0x20) | 0x300;
            if ((s16)obj->unk16 <= 8)
            {
                *(u16 *)(queue + 0xA) = (affineIndex << 9) | (0x8000 | (0x68 - 0x20));
                *(u16 *)(queue + 2) = (affineIndex << 9) | (0x8000 | (0x68 - 0x20));
            }
            else
            {
                *(u16 *)(queue + 0xA) = (affineIndex << 9) | (0x8000 | (0x78 - 0x20));
                *(u16 *)(queue + 2) = (affineIndex << 9) | (0x8000 | (0x78 - 0x20));
            }
            *(u16 *)(queue + 4) = 0xB390;
            *(u16 *)(queue + 0xC) = 0xB3A0;
            *(u8 **)(sys + SYS_OAM_CURSOR) = queue + 0x10;
            draw_digits_49_p11((s16)obj->unk16, (s16)obj->index + 8, (s16)obj->posY - 8, 0);
        }
        else
        {
            obj->timer = 0xC0;
            obj->state = state_digits_slide;
            obj->state(obj);
        }
    }
}

/* Shrinks the digit sprite from 4x to normal size while sliding it toward its final spot;
 * when the timer runs out it shows the new value (obj->unk8) and switches to plain drawing. */
void state_digits_slide(struct Owner *obj)
{
    s32 affineIndex;
    u32 bank;
    u32 off;
    struct OamAffineParams *affine;
    s32 x;
    s32 y;
    u8 *queue;
    u8 *sys;

    obj->timer -= 0x13;
    if ((s16)obj->timer <= 0)
    {
        obj->unk16 = obj->unk8;
        if ((s16)obj->unk16 == 0x10)
        {
            obj->index += 4;
        }
        obj->timer = 0;
        obj->state = draw_obj_at_xy;
        obj->state(obj);
    }
    else
    {
        affineIndex = ((struct WorldDigitBits *)gWorld)->digitCtr--;
        bank = ((u32)gSys[SYS_OAM_BANK] << 28) >> 31;
        bank <<= 10;
        off = (affineIndex << 5) + (u32)(gSys + SYS_OAM_BUFFER);
        affine = (struct OamAffineParams *)(bank + off);
        affine->pa = 0x100 - obj->timer;
        affine->pb = 0;
        affine->pc = 0;
        affine->pd = 0x100 - obj->timer;

        x = (s16)obj->index;
        if ((s16)obj->unk16 <= 8)
        {
            x += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((0x68 - x) * (s16)obj->timer) << 8, ((0x100 - (s16)obj->timer) * 3) << 8);
        }
        else
        {
            x += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((0x78 - x) * (s16)obj->timer) << 8, ((0x100 - (s16)obj->timer) * 3) << 8);
            if ((s16)obj->unk16 == 9)
            {
                x += 4;
            }
        }
        y = (s16)obj->posY + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((0x44 - (s16)obj->posY) * (s16)obj->timer) << 8, ((0x100 - (s16)obj->timer) * 3) << 8);

        /* two affine, double-size 32x32 objects sharing the affine slot */
        sys = gSys;
        queue = *(u8 **)(sys + SYS_OAM_CURSOR);
        *(u16 *)queue = *(u16 *)(queue + 8) = ((y - 0x20) & 0xFF) | 0x300;
        *(u16 *)(queue + 2) = *(u16 *)(queue + 0xA) = ((x - 0x20) & 0x1FF) | (affineIndex << 9) | 0x8000;
        *(u16 *)(queue + 4) = 0xB390;
        *(u16 *)(queue + 0xC) = 0xB3A0;
        *(u8 **)(sys + SYS_OAM_CURSOR) = queue + 0x10;
        draw_digits_49_p11((s16)obj->unk16, (s16)obj->index + 8, (s16)obj->posY - 8, 0);
    }
}
