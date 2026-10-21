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

struct ScreenObj
{
    u8 filler0[0x18];
    void *unk18;
    void *unk1C;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u8 filler26[0x46 - 0x26];
    u8 unk46;
    u8 unk47;
    u8 filler48[0x4C - 0x48];
    struct ScreenChild *unk4C;
    void *unk50;
};

struct ScreenChild
{
    u8 filler0[0x14];
    struct ScreenObj *unk14;
    void *unk18;
    u8 unk1C;
};

struct TextBlock
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u16 unk12;
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    u16 unk18;
    u8 filler1A[0x4A - 0x1A];
    u16 unk4A;
    u8 filler4C[0x4E - 0x4C];
    u16 unk4E;
    u16 unk50;
    u8 filler52[0x54 - 0x52];
    u16 unk54;
};

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

NONMATCH("asm/code_081171F8/menu_obj_free.s", void menu_obj_free(struct MenuTable *obj, u32 flags))
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
            if (((u32)entryFlags << 26) >> 29 & 6)
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
END_NONMATCH

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
    *(u16 *)obj->unk1C = 0x50;
    *(u16 *)((u8 *)obj->unk1C + 2) = 0x78;
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
void screen_obj_update(struct ScreenObj *obj)
{
    u8 *cell;
    s32 value;

    if (((struct SysKeys2A *)gSys)->keys & 0x40)
    {
        cell = &obj->unk46;
        value = *cell;
        if (value == 7)
        {
            goto cell_done;
        }
        value = value + 1;
        goto cell_store;
    }
    if (((struct SysKeys2A *)gSys)->keys & 0x80)
    {
        cell = &obj->unk46;
        value = *cell;
        if (value == 0)
        {
            goto cell_done;
        }
        value = value - 1;
    }
    else
    {
        goto cell_done;
    }
cell_store:
    *cell = value;
cell_done:
    if (((struct SysKeys2A *)gSys)->keys & 1)
    {
        if (obj->unk20 == 0)
        {
            sprite_alloc(obj->unk50, 0, obj->unk46, 0xFF, 0xFFFF, 0xFFFF);
            obj->unk47 = obj->unk46;
        }
        else
        {
            menu_entry_refresh(obj->unk50, 0, 1);
        }
        obj->unk20 ^= 1;
    }
    if (((struct SysKeys2A *)gSys)->keys & 0x100)
    {
        sub_08115F54(obj->unk50, 4, 0, 8, 0x7FFF, 0xFFFF, 0, 0);
    }
    if (((struct SysKeys2A *)gSys)->keys & 2)
    {
        obj_toggle_1c((struct MenuObj *)obj->unk4C);
    }
    if (((struct SysKeys2A *)gSys)->keys & 8)
    {
        if (obj != 0)
        {
            obj->unk18 = gScreenObjClass;
            run_pending_obj_callbacks();
            if (obj->unk50 != 0)
            {
                menu_obj_free((struct MenuTable *)obj->unk50, 3);
            }
            if (obj->unk4C != 0)
            {
                task_remove(obj->unk4C, 3);
            }
            task_remove(obj, 3);
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

ASM_FUNC("asm/code_081171F8/drop_roll.s", void drop_roll(void))

ASM_FUNC("asm/code_081171F8/build_menu_list.s", void build_menu_list(void))

u32 text_block_state(void)
{
    struct TextBlock *block;
    s32 span;
    s32 scaled;
    s32 second;

    block = (struct TextBlock *)gParty;
    if (block->unkE == 0 || block->unkE == block->unk12)
    {
        if (block->unk4A == 0 || block->unk4A == block->unk4E)
        {
            return 0xFF;
        }
        return 1;
    }
    if (block->unk4A == 0 || block->unk4A == block->unk4E)
    {
        return 0;
    }
    span = block->unkE * 100;
    span = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(span, block->unk12);
    block = (struct TextBlock *)gParty;
    scaled = block->unk4A * 100;
    second = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(scaled, block->unk4E);
    return span > second;
}


ASM_FUNC("asm/code_081171F8/text_block_busy.s", u32 text_block_busy(void))

u32 text_block_state(void);
u32 text_block_busy(void);
ASM_FUNC("asm/code_081171F8/text_slot_status.s", void text_slot_status(void))

ASM_FUNC("asm/code_081171F8/text_slot_set.s", void text_slot_set(void))

ASM_FUNC("asm/code_081171F8/char_menu_cell.s", void char_menu_cell(void))

ASM_FUNC("asm/code_081171F8/char_menu_list.s", void char_menu_list(void))

NONMATCH("asm/code_081171F8/unknown_thumb_081188a4.s", u8 unknown_thumb_081188a4(u8 kind, u8 idx))
{
    u8 result;
    u8 *field;
    const u8 *table;

    switch (kind)
    {
    case 0:
        field = gParty + idx * 0x3C + 0x32;
        if (*field == 0xFF)
        {
            result = 0xFF;
            break;
        }
        table = gRowTable;
        result = table[*field * 20 + 6];
        break;
    case 1:
        field = gParty + idx * 0x3C + 0x31;
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
END_NONMATCH

NONMATCH("asm/code_081171F8/sub_0811891C.s", u8 sub_0811891C(u8 kind, u8 idx))
{
    u8 result;
    u8 *field;
    const u8 *table;

    switch (kind)
    {
    case 0:
        field = gParty + idx * 0x3C + 0x32;
        if (*field == 0xFF)
        {
            result = 0xFF;
            break;
        }
        table = gRowTable;
        result = table[*field * 20 + 10];
        break;
    case 1:
        field = gParty + idx * 0x3C + 0x31;
        if (*field == 0xFF)
        {
            result = 0xFF;
            break;
        }
        table = gRowTableC;
        result = table[*field * 20 + 10];
        break;
    case 2:
        field = gParty + idx * 0x3C;
        if (field[0x33] == 0xFF)
        {
            result = 0xFF;
            break;
        }
        table = gRowTable8;
        result = table[field[0x33] * 8 + 4];
        break;
    }
    return result;
}
END_NONMATCH

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
