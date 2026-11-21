#include "global.h"
#include "ram_routines.h"

struct ListHeadX
{
    u8 filler0[0x3C];
    u32 head3C;
};

extern u8 gSys[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
extern u8 *gParty;
extern const u8 gRowTable[];
extern const u8 gRowTableC[];
extern const u8 gRowTable8[];
struct ItemRec
{
    u8 unk0;
    u8 unk1;
    u8 filler2[14];
};

struct DropRoll
{
    u32 chance : 2;
    u32 item1 : 4;
    u32 amount1 : 7;
    u32 item2 : 4;
    u32 amount2 : 7;
    u32 rest : 8;
};

extern const u8 gDropRollTable[];
extern u8 gItemTable[];
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void give_reward(u32 item, u32 amount, u32 flag);
void update_obj_list(void);
void obj_list_pass_0a8c(void);

struct MenuObj
{
    u8 filler0[0x18];
    void *unk18;
    u8 unk1C;
    u8 filler1D[0x2C - 0x1D];
    u8 *unk2C;
    u8 filler30[2];
    u8 unk32;
    u16 unk34;
    u8 filler36[0x44 - 0x36];
    u16 unk44;
    u8 filler46[0x48 - 0x46];
    u8 unk48;
    u8 filler49[0x4C - 0x49];
    struct MenuObj *unk4C;
    struct MenuTable *unk50;
};

struct MenuEntry
{
    u8 flags;
    u8 filler1[0xB];
    void *unkC;
    void *unk10;
    void *unk14;
    u8 filler18[0x1C - 0x18];
};

struct MenuTable
{
    u8 filler0[0x18];
    void *unk18;
    void *unk1C;
    void *unk20;
    u8 unk24;
    u8 filler25[0x2C - 0x25];
    struct MenuEntry *unk2C;
};

struct EntryBits
{
    u8 mode : 2;
    u8 unk2 : 6;
};

extern u8 gScreenObjClass[];
extern u8 gMenuTableVtable[];
void heap_free_a(void *p);
void heap_free_b(void *p);
extern u8 gScreenChildClass[];
void task_insert(void *node, u8 kind, u32 data);
void task_remove(void *node, u32 kind);
void run_pending_obj_callbacks(void);
void menu_obj_free(struct MenuTable *obj, u32 flags);

void cpu_set(const void *src, void *dst, u32 ctl);
u32 sprite_alloc(void *ctx, u32 a, u32 b, u32 c, u32 d, u32 e);
void menu_entry_refresh(void *ctx, u32 id, u32 value);
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void init_obj_09a8(u32 a, u32 b, u32 c, u32 d);
void *menu_obj_init(void *block, u32 a, const void *tag, u32 c);
void cpu_fast_set(const void *src, void *dst, u32 ctl);
extern const u8 gScreenChildTag[];
extern const u8 gScreenChildTaskTag[];
extern const u8 gScreenMenuTag[];
extern const u8 gScreenBgTilesBlock0[];
extern const u8 gScreenBgTilesBlock1[];
extern const u8 gScreenBgPalette[];
extern u8 gUnk_02000080[];
extern u8 gUnk_0200000A[];

/* gSys + 0x2A: the keys newly pressed this frame (A_BUTTON, DPAD_UP, ...) */
struct SysKeys2A
{
    u8 filler0[0x2A];
    u16 keys;
};

struct SysBgDirty
{
    u8 filler0[2];
    u16 unk2;
};

/* A palette test screen (a task; screen_obj_setup, screen_obj_update). */
struct ScreenObj
{
    u8 filler0[0x18];
    void *unk18;                /* class: gScreenObjClass */
    void *unk1C;                /* its sprite object */
    u16 unk20;                  /* 1 while palette unk47 is loaded into slot 0 */
    u16 unk22;
    u16 unk24;
    u8 filler26[0x46 - 0x26];
    u8 unk46;                   /* cursor: palette 0-7 to load */
    u8 unk47;                   /* the palette that was loaded */
    u8 filler48[0x4C - 0x48];
    struct ScreenChild *unk4C;  /* child task, toggled by B */
    void *unk50;                /* its struct MenuTable (sprite / palette slots) */
};

struct ScreenChild
{
    u8 filler0[0x14];
    struct ScreenObj *unk14;
    void *unk18;
    u8 unk1C;
};

/* One hero's record in gParty (Mario, then Luigi), as in code_080E9020. */
struct PartyHero
{
    u8 filler0[0xE];
    u16 hp;
    u8 filler10[2];
    u16 hpMax;
    u16 bp;
    u8 filler16[2];
    u16 bpMax;
    u8 filler1A[0x3C - 0x1A];
};

#define PARTY_MARIO 0
#define PARTY_LUIGI 1

void cpu_set(const void *src, void *dst, u32 ctl);

ASM_FUNC("asm/code_081171F8/obj_draw_tile_rows.s", void obj_draw_tile_rows(void))

void sub_0811733C(struct MenuObj *obj, u32 fade)
{
    obj->unk34 = obj->unk44;
    obj->unk44 = 0;
    if (fade != 0)
    {
        if (obj->unk32 & 4)
        {
            obj->unk48 = 0xFF;
            obj->unk32 = 0;
        }
        else
        {
            obj->unk48 = 1;
            obj->unk32 = 4;
        }
    }
}

void sub_08117380(struct MenuObj *obj)
{
    u16 running = obj->unk34;

    obj->unk44 = running;
    obj->unk34 = 0;
}

u32 sub_obj_running(struct MenuObj *obj)
{
    if (obj->unk44 != 0)
    {
        return 4;
    }
    if (obj->unk34 == 0)
    {
        return 0;
    }
    if (obj->unk32 & 4)
    {
        return 2;
    }
    return 1;
}


void menu_entry_mark_1(struct MenuObj *obj, u8 index)
{
    u8 *table = obj->unk2C;
    u32 offset = index * 0x1C;
    struct EntryBits *bits = (struct EntryBits *)(offset + (u32)table);

    if (bits->mode == 2)
    {
        bits->mode = 1;
    }
}

void menu_entry_mark_2(struct MenuObj *obj, u8 index)
{
    u8 *table = obj->unk2C;
    u32 offset = index * 0x1C;
    struct EntryBits *bits = (struct EntryBits *)(offset + (u32)table);

    if (bits->mode == 1)
    {
        bits->mode = 2;
    }
}

struct FlagsB { u8 lo3 : 3; u8 f : 3; u8 hi : 2; } __attribute__((packed));

void menu_obj_free(struct MenuTable *obj, u32 flags)
{
    struct MenuEntry *entry;
    s32 index;
    u8 entryFlags;

    obj->unk18 = gMenuTableVtable;
    entry = obj->unk2C;
    for (index = 0; index < obj->unk24; index++, entry++)
    {
        entryFlags = entry->flags;
        if ((entryFlags & 3) != 0)
        {
            if (((struct FlagsB *)&entry->flags)->f & 6)
            {
                if (entry->unk14 != 0)
                {
                    heap_free_b(entry->unk14);
                }
                heap_free_b(entry->unk10);
            }
            heap_free_b(entry->unkC);
        }
    }
    if (obj->unk2C != 0)
    {
        heap_free_a(obj->unk2C);
    }
    if (obj->unk20 != 0)
    {
        heap_free_a(obj->unk20);
    }
    if (obj->unk1C != 0)
    {
        heap_free_a(obj->unk1C);
    }
    task_remove(obj, flags);
}

struct SprHead { u16 w0; u16 w2; };
NONMATCH("asm/code_081171F8/screen_obj_setup.s", struct ScreenObj *screen_obj_setup(struct ScreenObj *obj, u8 prio, u32 arg))
{
    struct ScreenChild *child;
    void *menu;
    void *sprite;
    s32 attr;

    task_insert(obj, prio, arg);
    obj->unk18 = gScreenObjClass;
    obj->unk20 = 0;
    obj->unk46 = 0;
    obj->unk24 = 0;
    obj->unk22 = 0;
    child = heap_alloc2(0x20, 0, gScreenChildTag, 0);
    task_insert(child, 8, (u32)gScreenChildTaskTag);
    child->unk18 = gScreenChildClass;
    child->unk1C = 0;
    obj->unk4C = child;
    menu = heap_alloc2(0x5C, 0, gScreenMenuTag, 0);
    obj->unk50 = menu_obj_init(menu, 8, gScreenMenuTag, 0x10);
    obj->unk4C->unk14 = obj;
    init_obj_09a8(0, 0, 0, 0);
    sprite = create_obj_de4(0, 0x1001, 0, -1, -1, -1, -1);
    obj->unk1C = sprite;
    obj_load_record(sprite, 4, -1, 0, 0);
    sprite_update_cd0(obj->unk1C);
    attr = ((u8 *)obj->unk1C)[0x12];
    attr = (attr & -7) | 2;
    ((u8 *)obj->unk1C)[0x12] = attr;
    ((struct SprHead *)obj->unk1C)->w0 = 0x50;
    ((struct SprHead *)obj->unk1C)->w2 = 0x78;
    cpu_fast_set(gScreenBgTilesBlock0, (void *)0x06000000, 0x1E0);
    cpu_fast_set(gScreenBgTilesBlock1, (void *)0x06001000, 0x160);
    cpu_set(gScreenBgPalette, gUnk_02000080, 0x04000008);
    ((struct SysBgDirty *)gSys)->unk2 = 0xFF;
    *(u16 *)gUnk_0200000A = 0x201;
    *(u16 *)(gUnk_0200000A - 10) = 0x1340;
    return obj;
}
END_NONMATCH

void obj_toggle_1c(struct MenuObj *obj);
/* Per-frame input of the palette test screen: Up/Down move the cursor over palettes 0-7, A loads the
   selected one into slot 0 of the screen's MenuTable (sprite_alloc) or frees it again
   (menu_entry_refresh), R fades the palettes to black over 8 frames, B toggles the child task and
   Start closes the screen (as screen_obj_release does). */
void screen_obj_update(struct ScreenObj *screen)
{
    u8 *cursor;
    s32 index;

    if (((struct SysKeys2A *)gSys)->keys & DPAD_UP)
    {
        cursor = &screen->unk46;
        index = *cursor;
        if (index == 7)
        {
            goto cursor_done;
        }
        index = index + 1;
        goto cursor_store;
    }
    if (((struct SysKeys2A *)gSys)->keys & DPAD_DOWN)
    {
        cursor = &screen->unk46;
        index = *cursor;
        if (index == 0)
        {
            goto cursor_done;
        }
        index = index - 1;
    }
    else
    {
        goto cursor_done;
    }
cursor_store:
    *cursor = index;
cursor_done:
    if (((struct SysKeys2A *)gSys)->keys & A_BUTTON)
    {
        if (screen->unk20 == 0)
        {
            sprite_alloc(screen->unk50, 0, screen->unk46, 0xFF, 0xFFFF, 0xFFFF);
            screen->unk47 = screen->unk46;
        }
        else
        {
            menu_entry_refresh(screen->unk50, 0, 1);
        }
        screen->unk20 ^= 1;
    }
    if (((struct SysKeys2A *)gSys)->keys & R_BUTTON)
    {
        sub_08115F54(screen->unk50, 4, 0, 8, 0x7FFF, 0xFFFF, 0, 0);
    }
    if (((struct SysKeys2A *)gSys)->keys & B_BUTTON)
    {
        obj_toggle_1c((struct MenuObj *)screen->unk4C);
    }
    if (((struct SysKeys2A *)gSys)->keys & START_BUTTON)
    {
        if (screen != 0)
        {
            screen->unk18 = gScreenObjClass;
            run_pending_obj_callbacks();
            if (screen->unk50 != 0)
            {
                menu_obj_free((struct MenuTable *)screen->unk50, 3);
            }
            if (screen->unk4C != 0)
            {
                task_remove(screen->unk4C, 3);
            }
            task_remove(screen, 3);
        }
    }
}

void run_frame_hooks(void)
{
    u8 *base;
    u32 *oam_slot;
    struct ListHeadX *list;
    u32 old;

    base = (u8 *)&gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    update_obj_list();
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHeadX *)gUnk_0203FFB8)->head3C,
        ((struct ListHeadX *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHeadX *)gUnk_0203FFB8;
    old = ((u32 (*)(u32, u32))gUnk_03000D4C)(list->head3C, old);
    *oam_slot = old;
}

void obj_toggle_1c(struct MenuObj *obj)
{
    obj->unk1C ^= 1;
}

void screen_obj_release(struct MenuObj *obj, u32 kind)
{
    obj->unk18 = gScreenObjClass;
    run_pending_obj_callbacks();
    if (obj->unk50 != 0)
    {
        menu_obj_free(obj->unk50, 3);
    }
    if (obj->unk4C != 0)
    {
        task_remove(obj->unk4C, 3);
    }
    task_remove(obj, kind);
}

struct MenuObj *screen_child_init(struct MenuObj *obj, u8 kind, u32 data)
{
    task_insert(obj, kind, data);
    obj->unk18 = gScreenChildClass;
    obj->unk1C = 0;
    return obj;
}

/* Rolls what a hit block drops and gives it. The block's gDropRollTable entry (struct DropRoll
 * layout) holds two drops (reward kind, amount). Blocks of kind 0x10 and 0x20 give the first
 * drop when bro is 0 and the second otherwise; other blocks give the first one when a roll of
 * 0-3 is at most the entry's chance, the second one else. Kinds above 7 are given without an
 * amount. Returns what the block shows in bits 0-3 (mapped from the reward kind), bit 4 set and
 * the amount in bits 5-11. */
u32 drop_roll(u16 index, u8 blockKind, u8 bro)
{
    const u8 *entry;
    struct ItemRec *items;
    u32 drop;
    u32 amount;
    u32 count;
    u32 packed;
    u8 roll;

    if (blockKind == 0x10 || blockKind == 0x20)
    {
        if (bro == 0)
        {
            u32 tab = (u32)gDropRollTable;
            const u8 *look = (const u8 *)(index * 4 + tab);
            drop = ((u32)look[0] << 26) >> 28;
            amount = ((u32)*(u16 *)look << 19) >> 25;
        }
        else
        {
            u32 tab = (u32)gDropRollTable;
            const u8 *look = (const u8 *)(index * 4 + tab);
            drop = (*(u32 *)look << 15) >> 28;
            amount = look[2] >> 1;
        }
    }
    else
    {
        u32 tab;
        roll = rand_below(4, mt19937_next());
        tab = (u32)gDropRollTable;
        entry = (const u8 *)(index * 4 + tab);
        /* DropRoll.chance */
        if (roll <= ((u32)entry[0] << 30) >> 30)
        {
            /* DropRoll.item1, amount1 */
            drop = ((u32)entry[0] << 26) >> 28;
            amount = ((u32)*(u16 *)entry << 19) >> 25;
        }
        else
        {
            /* DropRoll.item2, amount2 */
            drop = (*(u32 *)entry << 15) >> 28;
            amount = entry[2] >> 1;
        }
    }
    count = amount;
    if (drop > 7)
        amount = 0;
    if (drop != 0)
        give_reward(drop, amount, 1);
    /* from here on, drop is what the block shows */
    switch (drop)
    {
    case 0:
        drop = 0;
        break;
    case 2:
        drop = 4;
        break;
    case 4:
        items = (struct ItemRec *)gItemTable;
        drop = items[amount].unk1;
        break;
    case 5:
        drop = 5;
        break;
    case 3:
        drop = amount + 0xC;
        break;
    case 8:
        drop = 1;
        break;
    case 9:
        drop = 2;
        break;
    case 1:
    case 12:
        drop = 3;
        break;
    default:
        break;
    }
    packed = (count << 5) | 0x10;
    drop &= 0xF;
    return drop | packed;
}

ASM_FUNC("asm/code_081171F8/build_menu_list.s", void build_menu_list(void))

/* Picks the hero who needs HP most: of the heroes who are hurt but not knocked out (0 < hp < hpMax),
   returns the one with the lower share of HP left (PARTY_LUIGI only when his is strictly lower),
   or 0xFF when neither is. */
u32 text_block_state(void)
{
    struct PartyHero *heroes;
    s32 marioPercent;
    s32 luigiScaled;
    s32 luigiPercent;

    heroes = (struct PartyHero *)gParty;
    if (heroes[PARTY_MARIO].hp == 0 || heroes[PARTY_MARIO].hp == heroes[PARTY_MARIO].hpMax)
    {
        if (heroes[PARTY_LUIGI].hp == 0 || heroes[PARTY_LUIGI].hp == heroes[PARTY_LUIGI].hpMax)
        {
            return 0xFF;
        }
        return PARTY_LUIGI;
    }
    if (heroes[PARTY_LUIGI].hp == 0 || heroes[PARTY_LUIGI].hp == heroes[PARTY_LUIGI].hpMax)
    {
        return PARTY_MARIO;
    }
    marioPercent = heroes[PARTY_MARIO].hp * 100;
    marioPercent = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(marioPercent, heroes[PARTY_MARIO].hpMax);
    heroes = (struct PartyHero *)gParty;
    luigiScaled = heroes[PARTY_LUIGI].hp * 100;
    luigiPercent = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(luigiScaled, heroes[PARTY_LUIGI].hpMax);
    return marioPercent > luigiPercent;
}


ASM_FUNC("asm/code_081171F8/text_block_busy.s", u32 text_block_busy(void))

u32 text_block_state(void);
u32 text_block_busy(void);
ASM_FUNC("asm/code_081171F8/text_slot_status.s", void text_slot_status(void))

// Raise cur toward range: by step, or to a quarter, half or all of range for the special steps.
static inline s32 text_step(s32 cur, s32 range, s32 step)
{
    switch (step)
    {
    case 0xFFFD:
        cur = range >> 2;
        if (range & 3)
            cur++;
        break;
    case 0xFFFE:
        cur = range >> 1;
        if (range & 1)
            cur++;
        break;
    default:
        cur += step;
        if (cur > range)
            cur = range;
        break;
    case 0xFFFF:
        cur = range;
        break;
    }
    return cur;
}

// The same, reading the current value through a pointer.
static inline s32 text_raise(u16 *p, s32 range, s32 step)
{
    s32 cur = *p;

    switch (step)
    {
    case 0xFFFD:
        cur = range >> 2;
        if (range & 3)
            cur++;
        break;
    case 0xFFFE:
        cur = range >> 1;
        if (range & 1)
            cur++;
        break;
    default:
        cur += step;
        if (range >= cur)
            break;
    case 0xFFFF:
        cur = range;
        break;
    }
    return cur;
}

// A bro's stats in gParty (0x3C bytes each; 0: Mario, 1: Luigi). The first of each
// max/stat pair is the bro's own value, the second the one in effect (assumed); the
// four stats after BP are assumed to follow the game's order POW, DEF, SPEED, STACHE.
struct BroStats
{
    u8 filler0[0xE];
    u16 hp;
    u16 maxHpBase;
    u16 maxHp;
    u16 bp;
    u16 maxBpBase;
    u16 maxBp;
    u16 powBase;
    u16 pow;
    u16 defBase;
    u16 def;
    u16 speedBase;
    u16 speed;
    u16 stacheBase;
    u16 stache;
    u8 filler2A[0x3C - 0x2A];
};

struct BroStatsBlock
{
    struct BroStats bro[3];
};

#define BRO(i) (((struct BroStatsBlock *)gParty)->bro[i])

#define STAT_MAX 999

// The item counts in gParty, indexed by item id. 0xFF: the item can't be used.
struct ItemCounts
{
    u8 filler0[0x8A];
    u8 count[1];
};

// An entry of gItemTable (16 bytes per item id).
struct ItemDef
{
    u8 kind; // enum ItemKind
    u8 filler1[7];
    u16 amount; // or ITEM_AMOUNT_QUARTER/HALF/FULL for restoring items
    u8 fillerA[6];
};

#define ITEM_AMOUNT_QUARTER 0xFFFD
#define ITEM_AMOUNT_HALF 0xFFFE
#define ITEM_AMOUNT_FULL 0xFFFF

#define ITEM_TARGET_BOTH 0xFD // text_slot_set target: both bros
#define ITEM_STAT_NONE 0xFF   // text_slot_set stat: none chosen

// What an item does when used (text_slot_set).
enum ItemKind
{
    ITEM_KIND_HEAL_HP = 1,
    ITEM_KIND_REVIVE = 3,          // only on a bro with 0 HP
    ITEM_KIND_RAISE_MAX_HP = 4,
    ITEM_KIND_RAISE_DEF = 5,
    ITEM_KIND_RAISE_STACHE = 6,
    ITEM_KIND_RAISE_MAX_BP = 7,
    ITEM_KIND_RAISE_POW = 8,
    ITEM_KIND_RAISE_SPEED = 9,
    ITEM_KIND_RAISE_CHOSEN = 0xA,  // raises the stat the player picks (kind 4 + stat)
    ITEM_KIND_HEAL_BP = 11,
    ITEM_KIND_HEAL_HP_BP = 12,
};

// Uses one of item `id` on bro `target` (or both, ITEM_TARGET_BOTH): restores HP/BP,
// revives, or raises a stat (by amount, up to STAT_MAX), `stat` picking the stat for
// ITEM_KIND_RAISE_CHOSEN. Returns 1 and takes the item if it was used, 0 if not.
// Note: raising max BP clamps maxBpBase twice and never maxBp (original behaviour).
u32 text_slot_set(u8 id, u8 target, u8 stat)
{
    struct ItemDef *item = (struct ItemDef *)(gItemTable + (id << 4));
    u8 kind;

    if (((struct ItemCounts *)gParty)->count[id] == 0xFF)
        return 0;
    if (((struct ItemCounts *)gParty)->count[id] == 0)
        return 0;

    if (item->kind == ITEM_KIND_RAISE_CHOSEN)
    {
        if (stat != ITEM_STAT_NONE)
            kind = stat + ITEM_KIND_RAISE_MAX_HP;
        else
            return 0;
    }
    else
    {
        kind = item->kind;
    }

    switch (kind)
    {
    case ITEM_KIND_HEAL_HP:
        if (target == ITEM_TARGET_BOTH)
        {
            if (BRO(0).hp != 0 && BRO(0).maxHp > BRO(0).hp)
                BRO(0).hp = text_raise(&BRO(0).hp, BRO(0).maxHp, item->amount);
            if (BRO(1).hp == 0)
                break;
            if (BRO(1).hp == 1)
                kind = 0;
            if (!(BRO(1).maxHp > BRO(1).hp))
                break;
            BRO(1).hp = text_raise(&BRO(1).hp, BRO(1).maxHp, item->amount);
            break;
        }
        if (BRO(target).hp == 0)
            return 0;
        if (!(BRO(target).maxHp > BRO(target).hp))
            return 0;
        BRO(target).hp = text_raise(&BRO(target).hp, BRO(target).maxHp, item->amount);
        break;
    case ITEM_KIND_HEAL_BP:
        if (!(BRO(target).maxBp > BRO(target).bp))
            return 0;
        BRO(target).bp = text_raise(&BRO(target).bp, BRO(target).maxBp, item->amount);
        break;
    case ITEM_KIND_HEAL_HP_BP:
        if (BRO(target).hp == 0
            || (BRO(target).maxHp <= BRO(target).hp && BRO(target).maxBp <= BRO(target).bp))
            return 0;
        if (BRO(target).maxHp > BRO(target).hp)
            BRO(target).hp = text_raise(&BRO(target).hp, BRO(target).maxHp, item->amount);
        if (BRO(target).maxBp > BRO(target).bp)
            BRO(target).bp = text_raise(&BRO(target).bp, BRO(target).maxBp, item->amount);
        break;
    case ITEM_KIND_REVIVE:
        if (BRO(target).hp != 0)
            return 0;
        BRO(target).hp = text_raise(&BRO(target).hp, BRO(target).maxHp, item->amount);
        break;
    case ITEM_KIND_RAISE_MAX_HP:
        if (BRO(target).maxHpBase >= STAT_MAX)
            return 0;
        BRO(target).maxHpBase += item->amount;
        if (BRO(target).maxHpBase >= STAT_MAX)
            BRO(target).maxHpBase = STAT_MAX;
        BRO(target).maxHp += item->amount;
        if (BRO(target).maxHp >= STAT_MAX)
            BRO(target).maxHp = STAT_MAX;
        break;
    case ITEM_KIND_RAISE_DEF:
        if (BRO(target).defBase >= STAT_MAX)
            return 0;
        BRO(target).defBase += item->amount;
        if (BRO(target).defBase >= STAT_MAX)
            BRO(target).defBase = STAT_MAX;
        BRO(target).def += item->amount;
        if (BRO(target).def >= STAT_MAX)
            BRO(target).def = STAT_MAX;
        break;
    case ITEM_KIND_RAISE_STACHE:
        if (BRO(target).stacheBase >= STAT_MAX)
            return 0;
        BRO(target).stacheBase += item->amount;
        if (BRO(target).stacheBase >= STAT_MAX)
            BRO(target).stacheBase = STAT_MAX;
        BRO(target).stache += item->amount;
        if (BRO(target).stache >= STAT_MAX)
            BRO(target).stache = STAT_MAX;
        break;
    case ITEM_KIND_RAISE_MAX_BP:
        if (BRO(target).maxBpBase >= STAT_MAX)
            return 0;
        BRO(target).maxBpBase += item->amount;
        if (BRO(target).maxBpBase >= STAT_MAX)
            BRO(target).maxBpBase = STAT_MAX;
        BRO(target).maxBp += item->amount;
        if (BRO(target).maxBpBase >= STAT_MAX)
            BRO(target).maxBpBase = STAT_MAX;
        break;
    case ITEM_KIND_RAISE_POW:
        if (BRO(target).powBase >= STAT_MAX)
            return 0;
        BRO(target).powBase += item->amount;
        if (BRO(target).powBase >= STAT_MAX)
            BRO(target).powBase = STAT_MAX;
        BRO(target).pow += item->amount;
        if (BRO(target).pow >= STAT_MAX)
            BRO(target).pow = STAT_MAX;
        break;
    case ITEM_KIND_RAISE_SPEED:
        if (BRO(target).speedBase >= STAT_MAX)
            return 0;
        BRO(target).speedBase += item->amount;
        if (BRO(target).speedBase >= STAT_MAX)
            BRO(target).speedBase = STAT_MAX;
        BRO(target).speed += item->amount;
        if (BRO(target).speed >= STAT_MAX)
            BRO(target).speed = STAT_MAX;
        break;
    }
    ((struct ItemCounts *)gParty)->count[id]--;
    return 1;
}

ASM_FUNC("asm/code_081171F8/char_menu_cell.s", void char_menu_cell(void))

ASM_FUNC("asm/code_081171F8/char_menu_list.s", void char_menu_list(void))

#define BRO_RECORD_SIZE 0x3C // one bro's record in gParty

// A bro's three gear slots in his gParty record: each holds a row id of its gear table, or
// GEAR_NONE. recalc_char_stats adds the gear's stat bonuses to the bro's stats.
#define BRO_GEAR_SLOT_0 0x32 // a row of gRowTable (GEAR_ROW_SIZE bytes)
#define BRO_GEAR_SLOT_1 0x31 // a row of gRowTableC (GEAR_ROW_SIZE bytes)
#define BRO_GEAR_SLOT_2 0x33 // a row of gRowTable8 (GEAR8_ROW_SIZE bytes)
#define GEAR_NONE 0xFF

// A gRowTable / gRowTableC row has two stat bonuses: a stat kind byte (0x64-0x67 picks the stat,
// see recalc_char_stats) followed by the u16 amount 2 bytes on.
#define GEAR_ROW_SIZE 20
#define GEAR_ROW_BONUS1_STAT 6
#define GEAR_ROW_BONUS2_STAT 10
#define GEAR8_ROW_SIZE 8
#define GEAR8_ROW_BONUS_STAT 4 // a gRowTable8 row's only stat bonus

u8 unknown_thumb_081188a4(u8 kind, u8 idx)
{
    u8 result;
    u8 *base;
    u8 *field;
    const u8 *table;

    switch (kind)
    {
    case 0:
        base = gParty;
        base += idx * 0x3C;
        field = base;
        field += 0x32;
        if (*field == 0xFF)
        {
            result = 0xFF;
            break;
        }
        table = gRowTable;
        result = table[*field * 20 + 6];
        break;
    case 1:
        base = gParty;
        base += idx * 0x3C;
        field = base;
        field += 0x31;
        if (*field == 0xFF)
        {
            result = 0xFF;
            break;
        }
        table = gRowTableC;
        result = table[*field * 20 + 6];
        break;
    case 2:
        result = 0xFF;
        break;
    }
    return result;
}

// Returns the stat kind of the second stat bonus of the gear bro `bro` wears in gear slot `slot`
// (0: BRO_GEAR_SLOT_0, 1: BRO_GEAR_SLOT_1, 2: BRO_GEAR_SLOT_2, whose rows have only one bonus), or
// GEAR_NONE when the slot is empty. unknown_thumb_081188a4 returns the first bonus's.
u8 sub_0811891C(u8 slot, u8 bro)
{
    u8 statKind;
    u8 *record;
    u8 *gear;
    const u8 *table;

    switch (slot)
    {
    case 0:
        record = gParty;
        record += bro * BRO_RECORD_SIZE;
        gear = record;
        gear += BRO_GEAR_SLOT_0;
        if (*gear == GEAR_NONE)
        {
            statKind = GEAR_NONE;
            break;
        }
        table = gRowTable;
        statKind = table[*gear * GEAR_ROW_SIZE + GEAR_ROW_BONUS2_STAT];
        break;
    case 1:
        record = gParty;
        record += bro * BRO_RECORD_SIZE;
        gear = record;
        gear += BRO_GEAR_SLOT_1;
        if (*gear == GEAR_NONE)
        {
            statKind = GEAR_NONE;
            break;
        }
        table = gRowTableC;
        statKind = table[*gear * GEAR_ROW_SIZE + GEAR_ROW_BONUS2_STAT];
        break;
    case 2:
    {
        u8 *party = gParty;
        u8 *gear2;

        party += bro * BRO_RECORD_SIZE;
        gear2 = party + BRO_GEAR_SLOT_2;
        if (*gear2 == GEAR_NONE)
        {
            statKind = GEAR_NONE;
            break;
        }
        {
            const u8 *rows = gRowTable8;

            statKind = rows[*gear2 * GEAR8_ROW_SIZE + GEAR8_ROW_BONUS_STAT];
        }
        break;
    }
    }
    return statKind;
}

s32 menu_cursor_pos(s32 pos, s32 max, s32 step)
{
    switch (step)
    {
    case 0xFFFD:
        pos = max >> 2;
        if (max & 3)
            pos++;
        break;
    case 0xFFFE:
        pos = max >> 1;
        if (max & 1)
            pos++;
        break;
    case 0xFFFF:
        pos = max;
        break;
    default:
        pos += step;
        if (max < pos)
            pos = max;
        break;
    }
    return pos;
}
