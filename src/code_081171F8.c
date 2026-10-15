#include "global.h"

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
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void run_pending_obj_callbacks(void);
void menu_obj_free(struct MenuTable *obj, u32 flags);

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
            if ((((u32)entryFlags << 26) >> 29) & 6)
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

ASM_FUNC("asm/code_081171F8/screen_obj_setup.s", void screen_obj_setup(void))

ASM_FUNC("asm/code_081171F8/screen_obj_update.s", void screen_obj_update(void))

ASM_FUNC("asm/code_081171F8/run_frame_hooks.s", void run_frame_hooks(void))

void obj_toggle_1c(struct MenuObj *obj)
{
    obj->unk1C ^= 1;
}

void unknown_thumb_08117742(struct MenuObj *obj, u32 kind)
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

struct MenuObj *sub_0811777C(struct MenuObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = gScreenChildClass;
    obj->unk1C = 0;
    return obj;
}

ASM_FUNC("asm/code_081171F8/drop_roll.s", void drop_roll(void))

ASM_FUNC("asm/code_081171F8/build_menu_list.s", void build_menu_list(void))

ASM_FUNC("asm/code_081171F8/text_block_state.s", void text_block_state(void))

ASM_FUNC("asm/code_081171F8/text_block_busy.s", void text_block_busy(void))

ASM_FUNC("asm/code_081171F8/text_slot_status.s", void text_slot_status(void))

ASM_FUNC("asm/code_081171F8/text_slot_set.s", void text_slot_set(void))

ASM_FUNC("asm/code_081171F8/char_menu_cell.s", void char_menu_cell(void))

ASM_FUNC("asm/code_081171F8/char_menu_list.s", void char_menu_list(void))

ASM_FUNC("asm/code_081171F8/unknown_thumb_081188a4.s", void unknown_thumb_081188a4(void))

ASM_FUNC("asm/code_081171F8/sub_0811891C.s", void sub_0811891C(void))

ASM_FUNC("asm/code_081171F8/menu_cursor_pos.s", void menu_cursor_pos(void))
