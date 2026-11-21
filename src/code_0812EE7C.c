#include "global.h"

struct ViewFrame
{
    u8 mode;
    u8 filler1[0xB8 - 1];
    u8 flagB8;
};

struct ViewOwner
{
    u8 filler0[0x14];
    struct ViewFrame *view14;
};

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

struct MenuSub
{
    u8 filler0[0x18];
    const void *cb18;
    u8 filler1C[0x20 - 0x1C];
    void *sub20;
};

struct MenuList
{
    u8 filler0[0x40];
    u8 *items40;   /* ids of the entries: items, then gRowTable rows, then gRowTableC rows */
    u8 filler44[0xA8 - 0x44];
    u16 countA8;   /* number of entries */
    u16 firstAA;   /* number of items (the first entries) */
    u16 countAC;   /* number of gRowTableC rows (the last entries) */
    u16 countAE;   /* number of gRowTable rows (after the items) */
    u16 valueB0;   /* menu_item_value of the entry under the cursor */
    u8 fillerB2[0xB4 - 0xB2];
    u16 indexB4;   /* index byte of the entry under the cursor */
    u16 kindB6;    /* kind of the entry under the cursor (3 for items and gRowTableC rows) */
    u8 fillerB8[0xC4 - 0xB8];
    u8 cursorC4;   /* bits 0-6: entry under the cursor */
};

extern u8 gItemTable[];
extern const u8 gRowTable[];
extern const u8 gRowTableC[];
extern const u8 gSubCallbacks[];
extern const u8 gScreenTagOpen[];
extern u8 gSys[];
extern struct ObjLists gUnk_0203FFB8;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
void update_obj_list(void);
void obj_list_pass_0a8c(void);
u32 draw_party_panel(struct ViewFrame *view, u32 oam);
void menu_redraw_step(struct ViewFrame *menu);
u32 sub_obj_running(void *menu);
void sub_08115F54(void *sub, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void task_insert(void *node, u8 kind);
void *heap_alloc2(u32 size, u32 flags, const void *tag, u32 zero);
void menu_screen_init(void *obj, u32 prio, const void *tag, u32 flags_bd, u32 flags_be, s32 neg);

ASM_FUNC("asm/code_0812EE7C/menu_redraw_step.s", void menu_redraw_step(struct ViewFrame *menu))

ASM_FUNC("asm/code_0812EE7C/menu_item_value.s", u32 menu_item_value(void *menu, u32 amount, u32 index, u32 kind, u32 flag))

u32 menu_item_value(void *menu, u32 amount, u32 index, u32 kind, u32 flag);
// Looks up the entry under the cursor (an item, a gRowTable row or a gRowTableC row, in that order
// in the list), stores its kind and index byte, and caches its menu_item_value for display.
void menu_cursor_update(struct MenuList *menu)
{
    if ((menu->cursorC4 & 0x7F) < menu->firstAA)
    {
        const u8 *items;

        menu->kindB6 = 3;
        items = gItemTable;
        menu->indexB4 = items[menu->items40[menu->cursorC4 & 0x7F] * 16 + 3];
    }
    else if (menu->firstAA <= (menu->cursorC4 & 0x7F) && (menu->cursorC4 & 0x7F) < menu->firstAA + menu->countAE
             && menu->countAE != 0)
    {
        const u8 *rows;

        rows = gRowTable;
        menu->kindB6 = rows[menu->items40[menu->cursorC4 & 0x7F] * 20 + 0x11];
        menu->indexB4 = rows[menu->items40[menu->cursorC4 & 0x7F] * 20 + 0xE];
    }
    else if (menu->firstAA + menu->countAE <= (menu->cursorC4 & 0x7F) && (menu->cursorC4 & 0x7F) < menu->countA8
             && menu->countAC != 0)
    {
        const u8 *rows;

        menu->kindB6 = 3;
        rows = gRowTableC;
        menu->indexB4 = rows[menu->items40[menu->cursorC4 & 0x7F] * 20 + 0xE];
    }
    menu->valueB0 = menu_item_value(menu, 0, (u8)menu->indexB4, (u8)menu->kindB6, 1);
}

ASM_FUNC("asm/code_0812EE7C/menu_list_build.s", void menu_list_build(void))

void obj_view_frame(struct ViewOwner *ctx)
{
    struct ViewFrame *view = ctx->view14;
    u32 off = 0x880;
    u32 *oamSlot = (u32 *)(gSys + off);
    u32 oam = *oamSlot;

    if (view->mode > 3)
    {
        if (view->flagB8 != 0)
        {
            menu_redraw_step(view);
        }
        update_obj_list();
        obj_list_pass_0a8c();
        gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
        oam = gUnk_03000D4C(gUnk_0203FFB8.root, oam);
        oam = draw_party_panel(view, oam);
        *oamSlot = oam;
    }
}

u32 sub_081302BC(struct MenuSub *obj)
{
    s32 status;

    status = sub_obj_running(obj->sub20) << 24;
    if (status == 0)
    {
        return 0;
    }
    return 1;
}

void sub_081302D4(struct MenuSub *obj)
{
    sub_08115F54(obj->sub20, 4, 0, 8, 0xFFFF, 0xFFFF, 0, 0);
}

void sub_081302FC(struct MenuSub *obj)
{
    u32 off = 0x884;

    *(s32 *)(gSys + off) = -1;
    sub_08115F54(obj->sub20, 0, 0, 8, 0xFFFF, 0xFFFF, 0, 0);
}

void *init_menu_sub_view(void *obj, u8 kind)
{
    task_insert(obj, kind);
    ((struct MenuSub *)obj)->cb18 = gSubCallbacks;
    return obj;
}

void menu_screen_open(u32 flags_bd, u32 flags_be)
{
    void *block;

    block = heap_alloc2(0xC8, 0, gScreenTagOpen, 0);
    menu_screen_init(block, 8, gScreenTagOpen, flags_bd, flags_be, -2);
}

ASM_FUNC("asm/code_0812EE7C/status_obj_init.s", void status_obj_init(void))
