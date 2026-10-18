#include "global.h"

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

struct SysSlots
{
    u8 filler0[0x898];
    u16 songSlot[2];
};

struct ScreenObj
{
    u8 filler0[0x14];
    struct ScreenObj *unk14;
    const void *cls18;
    u8 filler1C[0x50 - 0x1C];
    u8 *settings50;
    void *heapBlock54;
    u8 filler58[0xE4 - 0x58];
    u16 flagsE4;
    u8 filler_E6[0xF7 - 0xE6];
    u8 unkF7;
    u8 unkF8;
    u8 filler_F9[0x158 - 0xF9];
    struct ScreenObj *childTask158;
    struct ScreenObj *childObj15C;
    void *auxObj160;
};

extern u8 gSys[];
extern u32 gUnk_03000FFC;
extern u32 gUnk_03000D40;
extern const u8 gSaveScreenClass[];
extern const u8 gOpenScreenTaskTag[];
extern const u8 gOpenScreenObjTag[];
extern u8 gUnk_03000388[];

void owner_destroy(void *obj, u32 kind);
void run_pending_obj_callbacks(void);
void heap_free_a(void *p);
void menu_obj_free(void *obj, u32 kind);
void task_remove(void *node, u32 kind);
void task_insert(void *node, u8 kind, u32 data);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *create_child_task(void *task, u8 kind, const void *data);
void *menu_obj_init(void *obj, u32 kind, const void *tag, u32 flag);
void frame_update(void);
void cpu_fast_set(const void *src, void *dst, u32 control);
u8 *ptr_03000ffc_plus_1ff8(void);

void slot_set_mode(u32 i, u32 v, u8 flag);
void slot_reset(u32 id);
u32 slot_bit_test(u32 a);
void set_slot_fields(u32 a, u32 b, u32 c);
void sound_dma_reset(void);

ASM_FUNC("asm/code_08124F38/init_save_buffer.s", void init_save_buffer(void))

ASM_FUNC("asm/code_08124F38/save_blocks_write.s", void save_blocks_write(void))

ASM_FUNC("asm/code_08124F38/save_to_eeprom.s", void save_to_eeprom(void))

ASM_FUNC("asm/code_08124F38/eeprom_save_slots.s", void eeprom_save_slots(void))

ASM_FUNC("asm/code_08124F38/save_game_flush_dma_off.s", void save_game_flush_dma_off(void))

ASM_FUNC("asm/code_08124F38/save_game_flush_sound_off.s", void save_game_flush_sound_off(void))

ASM_FUNC("asm/code_08124F38/eeprom_write_block_muted.s", void eeprom_write_block_muted(void))

ASM_FUNC("asm/code_08124F38/eeprom_read_slots_setup.s", void eeprom_read_slots_setup(void))

ASM_FUNC("asm/code_08124F38/eeprom_write_slots_setup.s", void eeprom_write_slots_setup(void))

u8 *ptr_03000ffc_plus_1ff8(void)
{
    return (u8 *)gUnk_03000FFC + 0x1FF8;
}

u32 save_slot_in_use(void)
{
    s32 flags = ((u8 *)gUnk_03000FFC)[6];
    if (((flags >> ((gUnk_03000D40 << 0x1C) >> 0x1E)) & 1) != 0)
        return 1;
    return 0;
}

void set_sound_slot_modes(u8 mask, u8 unused, u8 value)
{
    if (mask != 0)
    {
        if (mask & 1)
            slot_set_mode(0, value, 0x1E);
        if (mask & 2)
            slot_set_mode(1, value, 0x1E);
    }
}

u32 sound_slots_save_reset(u8 *dst)
{
    u32 flags = 0;
    if ((u8)slot_bit_test(0) == 1)
        flags = 1;
    dst[0] = ((struct SysSlots *)gSys)->songSlot[0];
    slot_reset(0);
    if ((u8)slot_bit_test(1) == 1)
        flags |= 2;
    dst[1] = ((struct SysSlots *)gSys)->songSlot[1];
    slot_reset(1);
    sound_dma_reset();
    return flags;
}

u32 stash_sound_slots(u8 *dst, u32 value)
{
    u32 active = 0;
    if ((u8)slot_bit_test(0) == 1)
        active = 1;
    dst[0] = ((struct SysSlots *)gSys)->songSlot[0];
    value = (u8)value;
    set_slot_fields(0, 0, value);
    if ((u8)slot_bit_test(1) == 1)
        active |= 2;
    dst[1] = ((struct SysSlots *)gSys)->songSlot[1];
    set_slot_fields(1, 0, value);
    return active;
}

ASM_FUNC("asm/code_08124F38/checksum_bytes_sum.s", void checksum_bytes_sum(void))

struct ScreenObj *open_screen(struct ScreenObj *obj, u8 kind, u32 arg, u16 flags)
{
    struct ScreenObj *child;
    struct ScreenObj **slot158;
    struct ScreenObj **slot15C;

    task_insert(obj, kind, arg);
    obj->cls18 = gSaveScreenClass;
    obj->flagsE4 = flags;
    obj->settings50 = gUnk_03000388;
    obj->unkF7 = gUnk_03000388[8];
    obj->unkF8 = obj->settings50[9];
    obj->settings50[8] = 0xA;
    obj->settings50[9] = 8;
    slot158 = &obj->childTask158;
    *slot158 = create_child_task(heap_alloc2(0x1C, 0, gOpenScreenTaskTag, 0), 8, gOpenScreenTaskTag);
    slot15C = &obj->childObj15C;
    child = menu_obj_init(heap_alloc2(0x5C, 0, gOpenScreenObjTag, 1), 8, gOpenScreenObjTag, 8);
    *slot15C = child;
    (*slot158)->unk14 = obj;
    child->unk14 = obj;
    frame_update();
    gUnk_03000388[0x84C] |= 0x20;
    gUnk_03000388[0x84C] |= 0x40;
    *(u16 *)0x02000000 = 0;
    return obj;
}

void close_screen(struct ScreenObj *obj, u32 kind)
{
    obj->cls18 = gSaveScreenClass;
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000048 = 0;
    *(u16 *)0x0200004A = 0;
    *(u16 *)0x02000050 = 0;
    ((struct SysFlags888 *)gSys)->bits.bit5 = 0;
    ((struct SysFlags888 *)gSys)->bits.bit3 = 0;
    ((struct SysFlags888 *)gSys)->bits.bit6 = 0;
    if (obj->auxObj160 != 0)
        owner_destroy(obj->auxObj160, 3);
    run_pending_obj_callbacks();
    if (obj->heapBlock54 != 0)
        heap_free_a(obj->heapBlock54);
    if (obj->childObj15C != 0)
        menu_obj_free(obj->childObj15C, 3);
    if (obj->childTask158 != 0)
        task_remove(obj->childTask158, 3);
    task_remove(obj, kind);
}

ASM_FUNC("asm/code_08124F38/save_screen_task.s", void save_screen_task(void))
