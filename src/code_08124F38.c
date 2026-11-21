#include "global.h"

struct ByteBit1
{
    u8 lo : 1;
    u8 bit1 : 1;
    u8 hi : 6;
};

struct SaveHdrBits
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 hi : 2;
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

struct DmaChannel
{
    vu32 src;
    vu32 dst;
    vu16 cntL;
    vu16 cntH;
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
    u8 *settings50;     /* gUnk_03000388: KEY_REPEAT_DELAY, KEY_REPEAT_RATE */
    void *heapBlock54;
    u8 filler58[0xE4 - 0x58];
    u16 flagsE4;
    u8 filler_E6[0xF7 - 0xE6];
    u8 unkF7;           /* the key repeat delay from before open_screen */
    u8 unkF8;           /* the key repeat rate from before open_screen */
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
/* Bytes of gUnk_03000388: frames before a held key repeats, and frames between repeats. */
#define KEY_REPEAT_DELAY 8
#define KEY_REPEAT_RATE 9

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
void sound_driver_init(void);
u16 eeprom_write_block_wait(u16 idx, void *data);
u32 eeprom_verify(u16 idx, void *data);
u16 eeprom_read_block(u16 idx, void *data);
#define EEPROM_BLOCK_COUNT 0x400 // 8-byte blocks in the 8 KiB EEPROM

extern u8 gUnk_03001004; // song slots playing (bit 0, bit 1) when an EEPROM erase started
extern u8 gUnk_03001006[]; // both slots' song ids, saved when an EEPROM erase started
extern u8 *gUnk_03001BD0;

/* Starts a blank save: clears the 8 KiB EEPROM image at gUnk_03000FFC and writes its header
 * ("MALRPG", no save slots in use, the default option bits and their checksum at byte 15) and the
 * record in its last 8 bytes that check_warm_reset_counter uses (20000 and 3, checksummed in byte
 * 7). Then stops both song slots and the sound FIFO DMAs 1 and 2. */
void init_save_buffer(void)
{
    s32 trailerSum;
    s32 headerSum;
    u8 *save;
    u8 *trailer;
    u32 zero;
    s32 i;
    struct DmaChannel *dma1;
    struct DmaChannel *dma2;

    save = (u8 *)gUnk_03000FFC;
    zero = 0;
    cpu_fast_set(&zero, save, 0x01000800); /* fill 0x800 words */
    save[0] = 'M';
    save[1] = 'A';
    save[2] = 'L';
    save[3] = 'R';
    save[4] = 'P';
    save[5] = 'G';
    save[6] = 0; /* save slots in use (save_slot_in_use) */
    save[7] = 0;
    ((struct SaveHdrBits *)&save[8])->lo3 = 7;
    ((struct SaveHdrBits *)&save[8])->bit3 = 0;
    ((struct SaveHdrBits *)&save[8])->bit4 = 0;
    ((struct SaveHdrBits *)&save[8])->bit5 = 0;
    headerSum = 0;
    for (i = 8; i <= 0xE; i++)
        headerSum += save[i];
    save[0xF] = headerSum;
    trailer = ptr_03000ffc_plus_1ff8();
    *(u32 *)trailer = 20000;
    *(u32 *)(trailer + 4) = 3;
    trailerSum = 0;
    for (i = 0; i <= 6; i++)
        trailerSum += trailer[i];
    trailer[7] = trailerSum;
    slot_reset(0);
    slot_reset(1);
    /* clear repeat and start timing, then the enable bit */
    dma1 = (struct DmaChannel *)0x040000BC;
    *(u16 *)&dma1->cntH = dma1->cntH & 0xC5FF;
    *(u16 *)&dma1->cntH = dma1->cntH & 0x7FFF;
    dma1->cntH;
    dma2 = (struct DmaChannel *)0x040000C8;
    *(u16 *)&dma2->cntH = dma2->cntH & 0xC5FF;
    *(u16 *)&dma2->cntH = dma2->cntH & 0x7FFF;
    dma2->cntH;
}

ASM_FUNC("asm/code_08124F38/save_blocks_write.s", void save_blocks_write(void))

ASM_FUNC("asm/code_08124F38/save_to_eeprom.s", void save_to_eeprom(void))

ASM_FUNC("asm/code_08124F38/eeprom_save_slots.s", void eeprom_save_slots(void))

ASM_FUNC("asm/code_08124F38/save_game_flush_dma_off.s", void save_game_flush_dma_off(void))

NONMATCH("asm/code_08124F38/save_game_flush_sound_off.s", void save_game_flush_sound_off(u8 *buf, u16 block))
{
    u16 ie[2];
    u8 unused[8];
    u8 slots[2];
    s32 i;
    s32 n;
    u8 *p;
    u32 flags;
    u32 mask;
    u8 *sq;

    ie[0] = *(u16 *)0x04000200;
    *(u16 *)0x04000208 = 0;
    *(u16 *)0x04000200 = 0;
    sq = slots;
    flags = 0;
    if ((u8)slot_bit_test(0) == 1)
        flags = 1;
    sq[0] = ((struct SysSlots *)gSys)->songSlot[0];
    slot_reset(0);
    if ((u8)slot_bit_test(1) == 1)
        ((struct ByteBit1 *)&flags)->bit1 = 1;
    sq[1] = ((struct SysSlots *)gSys)->songSlot[1];
    slot_reset(1);
    sound_dma_reset();
    mask = flags;
    n = 0x400;
    if ((block & 7) != 0)
        n = 0x3FF - (block >> 3);
    else
        n -= (block >> 3);
    p = buf;
    for (i = n; i < *(u16 *)(gUnk_03001BD0 + 4); i++)
    {
        eeprom_write_block_wait(i, p);
        p += 8;
    }
    p = buf;
    for (i = n; i < *(u16 *)(gUnk_03001BD0 + 4); i++)
    {
        eeprom_verify(i, p);
        p += 8;
    }
    sound_driver_init();
    if (mask != 0)
    {
        if (mask & 1)
            slot_set_mode(0, 0xFF, 0x1E);
        if (mask & 2)
            slot_set_mode(1, 0xFF, 0x1E);
    }
    *(u16 *)0x04000200 = ie[0];
}
END_NONMATCH

// One step of wiping the save EEPROM: writes 8 zero bytes to `block` and verifies them,
// with interrupts off. Block 0 stops both song slots (remembering which were playing);
// after the last block they are started again.
u16 eeprom_write_block_muted(u32 unused0, u32 unused1, u16 block)
{
    vu16 savedIe;
    u16 zeros[4];
    s32 ofs;
    u32 playingMask;
    u32 savedMask;
    u8 mask;
    u8 *sys;
    u8 *songs;
    u16 *fill;
    u16 *start;
    s32 nextBlock;

    savedIe = REG_IE;
    REG_IME = 0;
    REG_IE = 0;
    if (block == 0)
    {
        sys = gSys;
        sys[0x888] |= 0x40;
        songs = gUnk_03001006;
        playingMask = 0;
        if ((u8)slot_bit_test(0) == 1)
            playingMask = 1;
        songs[0] = ((struct SysSlots *)sys)->songSlot[0];
        slot_reset(0);
        if ((u8)slot_bit_test(1) == 1)
            playingMask |= 2;
        songs[1] = ((struct SysSlots *)sys)->songSlot[1];
        slot_reset(1);
        sound_dma_reset();
        gUnk_03001004 = playingMask;
    }
    fill = zeros;
    start = fill;
    nextBlock = block + 1;
    for (ofs = 0; ofs < 8; ofs += 2)
        *fill++ = 0;
    fill = zeros;
    fill = start;
    eeprom_write_block_wait(block, fill);
    eeprom_verify(block, zeros);
    if (nextBlock == EEPROM_BLOCK_COUNT)
    {
        ((struct SysFlags888 *)gSys)->bits.bit6 = 0;
        savedMask = gUnk_03001004;
        mask = savedMask;
        if (mask != 0)
        {
            if (mask & 1)
                slot_set_mode(0, 0xFF, 0x1E);
            if (mask & 2)
                slot_set_mode(1, 0xFF, 0x1E);
        }
    }
    REG_IE = savedIe;
    return nextBlock;
}

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

u32 checksum_bytes_sum(u8 *data, u16 len)
{
    s32 i;
    s32 sum = 0;
    for (i = 0; i < len; i++)
        sum += data[i];
    return sum;
}

/* Opens the save screen: inserts obj as a task of the given kind, saves the key repeat delay and rate
 * and sets them to 10 and 8 frames, creates its child task and menu object (both pointing back at obj),
 * then runs a frame and sets bits 5 and 6 of byte 0x84C of gUnk_03000388. */
struct ScreenObj *open_screen(struct ScreenObj *obj, u8 kind, u32 arg, u16 flags)
{
    struct ScreenObj *menu;
    struct ScreenObj **taskSlot;
    struct ScreenObj **menuSlot;

    task_insert(obj, kind, arg);
    obj->cls18 = gSaveScreenClass;
    obj->flagsE4 = flags;
    obj->settings50 = gUnk_03000388;
    obj->unkF7 = gUnk_03000388[KEY_REPEAT_DELAY];
    obj->unkF8 = obj->settings50[KEY_REPEAT_RATE];
    obj->settings50[KEY_REPEAT_DELAY] = 10;
    obj->settings50[KEY_REPEAT_RATE] = 8;
    taskSlot = &obj->childTask158;
    *taskSlot = create_child_task(heap_alloc2(0x1C, 0, gOpenScreenTaskTag, 0), 8, gOpenScreenTaskTag);
    menuSlot = &obj->childObj15C;
    menu = menu_obj_init(heap_alloc2(0x5C, 0, gOpenScreenObjTag, 1), 8, gOpenScreenObjTag, 8);
    *menuSlot = menu;
    (*taskSlot)->unk14 = obj;
    menu->unk14 = obj;
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
