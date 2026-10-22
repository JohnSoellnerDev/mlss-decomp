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
    u16 unkA6;
    u8 unkA8;
    u8 unkA9;
    u8 filler[0xC8 - 0xAA];
    void *unkC8;
};

struct MenuChild
{
    u8 filler0[0x14];
    void *parent;
    void *vtable;
};

extern u16 gUnk_02000008;
extern const u8 gUnk_08A59364[];
extern const u8 gUnpackTag[];
extern const u8 gSaveMenuClass[];
extern const u8 gSaveMenuChildClass[];
extern const u8 gMenuChildTaskTag[];
extern const u8 gMenuChildObjTag[];
extern u8 gUnk_03000388[];
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

void *menu_obj_open(struct ScreenObj *obj, u8 kind, void *arg)
{
    u32 hw;
    u8 *sysFlags;
    struct MenuChild *child;
    void *menu_mem;
    void *menu;

    task_insert(obj, kind, arg);
    obj->vtable = (void *)gSaveMenuClass;
    obj->keys = gUnk_03000388;
    hw = *(u16 *)(gUnk_03000388 + 0x3C);
    obj->unkA6 = (hw << 22) >> 22;
    *(u16 *)(gUnk_03000388 + 0x3C) = hw | 0x3FF;
    obj->unkA8 = obj->keys[8];
    obj->unkA9 = obj->keys[9];
    obj->keys[8] = 0xA;
    obj->keys[9] = 8;
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000050 = 0;
    frame_update();
    sysFlags = gUnk_03000388 + 0x84C;
    *sysFlags |= 0x20;
    *sysFlags |= 0x40;
    child = heap_alloc2(0x1C, 0, gMenuChildTaskTag, 1);
    task_insert(child, 8, gMenuChildTaskTag);
    child->vtable = (void *)gSaveMenuChildClass;
    obj->child = child;
    menu_mem = heap_alloc2(0x5C, 0, gMenuChildObjTag, 1);
    menu = menu_obj_init(menu_mem, 8, gMenuChildObjTag, 8);
    obj->menu = menu;
    ((struct MenuChild *)obj->child)->parent = obj;
    return obj;
}

NONMATCH("asm/code_08128B60/screen_obj_free.s", void screen_obj_free(struct ScreenObj *obj, u32 kind))
{
    s32 flags;
    void *owner;
    void *mem;
    void *menu;
    void *child;

    obj->vtable = (void *)gSaveMenuClass;
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000048 = 0;
    *(u16 *)0x0200004A = 0;
    *(u16 *)0x02000050 = 0;
    flags = gSys[0x888];
    flags &= -0x21;
    flags &= -9;
    flags &= -0x41;
    gSys[0x888] = flags;
    owner = obj->owner;
    if (owner != 0)
        owner_destroy(owner, 3);
    run_pending_obj_callbacks();
    mem = obj->unkC8;
    if (mem != 0)
        heap_free_a(mem);
    menu = obj->menu;
    if (menu != 0)
        menu_obj_free(menu, 3);
    child = obj->child;
    if (child != 0)
        task_remove(child, 3);
    task_remove(obj, kind);
}
END_NONMATCH

ASM_FUNC("asm/code_08128B60/save_menu_step.s", void save_menu_step(void))

ASM_FUNC("asm/code_08128B60/save_menu_update.s", void save_menu_update(void))

ASM_FUNC("asm/code_08128B60/screen_setup_sprites.s", void screen_setup_sprites(void))
