#include "global.h"

struct ScreenObj
{
    u8 filler0[0x18];
    void *vtable;
    void *child;
    void *menu;
    void *owner;
    u8 filler28[0x8C - 0x28];
    u8 *keys;
    u8 filler90[0xA6 - 0x90];
    u16 unkA6; /* the enabled keys from before menu_obj_open */
    u8 unkA8;  /* the key repeat delay from before menu_obj_open */
    u8 unkA9;  /* the key repeat rate from before menu_obj_open */
    u8 filler[0xC8 - 0xAA];
    void *unkC8;
};

struct MenuChild
{
    u8 filler0[0x14];
    void *parent;
    void *vtable;
};

struct SysBits888
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct SysFlags888
{
    u8 filler0[0x888];
    struct SysBits888 bits;
};

extern u16 gUnk_02000008;
extern const u8 gUnk_08A59364[];
extern const u8 gUnpackTag[];
extern const u8 gSaveMenuClass[];
extern const u8 gSaveMenuChildClass[];
extern const u8 gMenuChildTaskTag[];
extern const u8 gMenuChildObjTag[];
extern u8 gUnk_03000388[]; /* key input settings: [8] repeat delay, [9] repeat rate, +0x3C enabled keys */
#define KEY_REPEAT_DELAY 8          /* byte of gUnk_03000388: frames before a held key repeats */
#define KEY_REPEAT_RATE 9           /* byte of gUnk_03000388: frames between repeats */
#define KEY_INPUT_ENABLED_KEYS 0x3C /* u16 offset in gUnk_03000388 */
#define KEYS_ALL 0x3FF              /* all 10 GBA buttons */
extern u8 gSys[];
extern void (*gUnk_03000C84)(const u8 *src, void *dst);

void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void *heap_alloc2(u32 size, u32 pool, const u8 *tag, u8 clear);
void heap_free(void *p);
void heap_free_a(void *p);
void cpu_set(const void *src, void *dst, u32 ctl);
s32 read_varlen_plus1(const u8 *src);
void task_insert(void *node, u8 kind, const void *data);
void task_remove(void *node, u32 kind);
void frame_update(void);
void *menu_obj_init(void *obj, u8 kind, const u8 *tag, u8 flag);
void menu_obj_free(void *obj, u32 flags);
void owner_destroy(void *owner, u32 flags);
void run_pending_obj_callbacks(void);

/* Fills a rectangle of 16-bit tile entries with zeros (flag 0) or with running numbers from first. */
NONMATCH("asm/code_08128B60/fill_tile_rect.s", void fill_tile_rect(u16 *map, s32 x, s32 y, s32 width, s32 height, u16 first, u8 flag))
{
    u16 number = first;
    u8 fill = flag;
    u16 row;
    u16 col;

    x >>= 3;
    y >>= 3;
    width += x;
    height += y;
    row = y;
    if (row < height)
    {
        do
        {
            s32 nextRow;
            col = x;
            nextRow = row + 1;
            if (col < width)
            {
                u16 *rowBase = (u16 *)((u32)(row << 6) + (u32)map);
                do
                {
                    if (fill == 0)
                        rowBase[col] = fill;
                    else
                        rowBase[col] = number++;
                    col++;
                } while (col < width);
            }
            row = nextRow;
        } while (row < height);
    }
}
END_NONMATCH

u32 vram_tile_index(u32 addr)
{
    u32 base = (gUnk_02000008 & 0xC) << 12;
    return ((addr - base) << 17) >> 22;
}

s32 unpack_to_block(const u8 *src, void *dst, u32 direct)
{
    s32 size = read_varlen_plus1(src);
    if (direct != 0)
    {
        dst = heap_alloc_raw(1, size, gUnpackTag);
        (*gUnk_03000C84)(src, dst);
        return size;
    }
    else
    {
        void *temp = heap_alloc_raw(1, size, gUnpackTag);
        (*gUnk_03000C84)(src, temp);
        {
            u32 count = size;
            if (size < 0)
                count = size + 3;
            cpu_set(temp, dst, ((count << 9) >> 11) | 0x04000000);
        }
        heap_free(temp);
    }
    return size;
}

u32 rom_table_entry(u32 idx, u32 *len_out)
{
    u32 off = idx << 2;
    u32 base = (u32)gUnk_08A59364;
    u32 entry = *(u32 *)(base + off) + base;
    if (len_out != 0)
    {
        u32 base4 = base + 4;
        *len_out = *(u32 *)(base4 + off) + base - entry;
    }
    return entry;
}

/* Constructor of the save menu: inserts obj as a task, saves the enabled keys and key repeat delay/rate
 * and switches to all keys with repeat 10/8, blanks the display, runs a frame, sets bits 5 and 6 of
 * byte 0x84C of gUnk_03000388, then creates its child task and menu object. screen_obj_free undoes it. */
void *menu_obj_open(struct ScreenObj *obj, u8 kind, void *arg)
{
    u32 enabledKeys;
    u8 *sysFlags;
    struct MenuChild *child;
    void *menuMem;
    void *menu;

    task_insert(obj, kind, arg);
    obj->vtable = (void *)gSaveMenuClass;
    obj->keys = gUnk_03000388;
    enabledKeys = *(u16 *)(gUnk_03000388 + KEY_INPUT_ENABLED_KEYS);
    obj->unkA6 = (enabledKeys << 22) >> 22;
    *(u16 *)(gUnk_03000388 + KEY_INPUT_ENABLED_KEYS) = enabledKeys | KEYS_ALL;
    obj->unkA8 = obj->keys[KEY_REPEAT_DELAY];
    obj->unkA9 = obj->keys[KEY_REPEAT_RATE];
    obj->keys[KEY_REPEAT_DELAY] = 10;
    obj->keys[KEY_REPEAT_RATE] = 8;
    REG_BUF_DISPCNT = 0;
    REG_BUF_BLDCNT = 0;
    frame_update();
    sysFlags = gUnk_03000388 + 0x84C;
    *sysFlags |= 0x20;
    *sysFlags |= 0x40;
    child = heap_alloc2(0x1C, 0, gMenuChildTaskTag, 1);
    task_insert(child, 8, gMenuChildTaskTag);
    child->vtable = (void *)gSaveMenuChildClass;
    obj->child = child;
    menuMem = heap_alloc2(0x5C, 0, gMenuChildObjTag, 1);
    menu = menu_obj_init(menuMem, 8, gMenuChildObjTag, 8);
    obj->menu = menu;
    ((struct MenuChild *)obj->child)->parent = obj;
    return obj;
}

void screen_obj_free(struct ScreenObj *obj, u32 kind)
{
    obj->vtable = (void *)gSaveMenuClass;
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000048 = 0;
    *(u16 *)0x0200004A = 0;
    *(u16 *)0x02000050 = 0;
    ((struct SysFlags888 *)gSys)->bits.bit5 = 0;
    ((struct SysFlags888 *)gSys)->bits.bit3 = 0;
    ((struct SysFlags888 *)gSys)->bits.bit6 = 0;
    if (obj->owner != 0)
        owner_destroy(obj->owner, 3);
    run_pending_obj_callbacks();
    if (obj->unkC8 != 0)
        heap_free_a(obj->unkC8);
    if (obj->menu != 0)
        menu_obj_free(obj->menu, 3);
    if (obj->child != 0)
        task_remove(obj->child, 3);
    task_remove(obj, kind);
}

ASM_FUNC("asm/code_08128B60/save_menu_step.s", void save_menu_step(void))

ASM_FUNC("asm/code_08128B60/save_menu_update.s", void save_menu_update(void))

ASM_FUNC("asm/code_08128B60/screen_setup_sprites.s", void screen_setup_sprites(void))
