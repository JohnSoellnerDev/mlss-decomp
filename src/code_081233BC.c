#include "global.h"
#include "ram_routines.h"

struct SysRam
{
    u8 filler0[0x24];
    u32 word24;
    u16 keys28;
    u16 keys2A;
    u8 filler2C[0x898 - 0x2C];
    u16 songSlot[2];
    u8 filler89C[0x8C0 - 0x89C];
    u8 lang8C0;
};

struct AreaEntry
{
    u8 area;
    u8 unk1;
    u8 filler2[22];
};

extern struct SysRam gSys;
extern u8 *gParty;
extern u32 gUnk_03000D40;
extern u32 gUnk_03000FF8;
extern u32 gUnk_03000FFC;
extern const u8 gPartyStateTag[];
extern const u8 gPartyMenuObjTag[];
extern const u8 gMenuObjTagFlagged[];
extern const u32 gAreaPtrTable[];
extern const struct AreaEntry gAreaEntryTable[];

void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void heap_free_a(void *p);
void cpu_set(const void *src, void *dst, u32 ctl);
void *rom_table_entry(u32 idx, u32 flag);
void unpack_to_block(void *src, void *dst, u32 mode);
void window_obj_init(void *obj, u32 a, const void *tag, u32 ctx, s32 b, u32 flag);
void menu_obj_open(void *obj, u32 a, const void *tag, s32 b);
void save_slot_store(void);
void sound_dma_reset(void);
void slot_reset(u32 id);
u32 slot_bit_test(u32 a);
void slot_set_mode(u32 i, u32 v, u8 flag);
u16 eeprom_write_block_wait(u16 idx, void *data);
u32 eeprom_verify(u16 idx, void *data);

ASM_FUNC("asm/code_081233BC/draw_party_stats_2.s", void draw_party_stats_2(void))

ASM_FUNC("asm/code_081233BC/draw_markers.s", void draw_markers(void))

void refresh_hud_tiles(u32 ctx)
{
    void *block = rom_table_entry(gSys.lang8C0 + 7, 0);
    unpack_to_block(block, (u8 *)ctx + 0x840, 0);
}

ASM_FUNC("asm/code_081233BC/draw_sprite.s", void draw_sprite(void))

u16 *oam_write_attrs(u16 *oam, u16 y, u8 flags, u8 mode, u8 mosaic, u8 colors, u8 shape, u16 x, u8 affine, u8 size, u16 tile, u8 prio, u8 palette)
{
    oam[0] = y | (shape << 14 | colors << 13 | mosaic << 12 | mode << 10 | flags << 8);
    oam[1] = x | (size << 14 | affine << 9);
    oam[2] = tile | (palette << 12 | prio << 10);
    return oam + 4;
}

void create_menu_object(u32 ctx, u32 flag)
{
    if (flag == 0)
    {
        void *obj = heap_alloc2(0x260, 0, gPartyMenuObjTag, 0);
        window_obj_init(obj, 8, gPartyMenuObjTag, ctx, -2, flag);
    }
    else
    {
        void *obj = heap_alloc2(0x128, 0, gMenuObjTagFlagged, 0);
        menu_obj_open(obj, 8, gMenuObjTagFlagged, -2);
    }
}

ASM_FUNC("asm/code_081233BC/init_party_state.s", void init_party_state(void))

ASM_FUNC("asm/code_081233BC/give_reward.s", void give_reward(void))

ASM_FUNC("asm/code_081233BC/recalc_char_stats.s", void recalc_char_stats(void))

void clear_byte_if_sys_flag(u8 *flag)
{
    if ((gSys.keys2A & 2) != 0)
    {
        *flag = 0;
    }
}

u32 area_ptr_lookup(u16 idx)
{
    const u32 *table = gAreaPtrTable;
    const u32 *row = &table[idx];
    u32 lang = gSys.lang8C0;
    u32 *entry = (u32 *)*row;
    return *(u32 *)*(lang + entry);
}

u32 area_table_lookup_081240AC(u16 entry)
{
    const u32 *table = gAreaPtrTable;
    const struct AreaEntry *ent = gAreaEntryTable;
    const u32 *row = &table[ent[entry].area];
    u32 lang = gSys.lang8C0;
    u32 *ptr = (u32 *)*row;
    return *(u32 *)*(lang + ptr);
}

void state_free(void)
{
    if (gParty != 0)
    {
        heap_free_a(gParty);
    }
    gParty = 0;
}

NONMATCH("asm/code_081233BC/state_load.s", void state_load(void))
{
    u8 *buf = (u8 *)gUnk_03000FFC;
    u8 **party = &gParty;
    u8 *block = heap_alloc2(0x1BC, 1, gPartyStateTag, 0);
    *party = block;
    cpu_set(buf + ((gUnk_03000D40 << 0x1C) >> 0x1E) * 0x6F8 + 0x30, block, 0xDE);
    gUnk_03000FF8 = (u32)(*party + 0x78);
}
END_NONMATCH

ASM_FUNC("asm/code_081233BC/save_data_verify.s", void save_data_verify(void))

NONMATCH("asm/code_081233BC/save_game_flush_irq_off.s", u32 save_game_flush_irq_off(u32 mode, u32 unused, u8 *out))
{
    u8 *buf;
    u8 flags;
    u32 flagWord;
    u16 ie[2];
    u32 idx;
    u32 blk;
    u8 *slot;
    u8 *p;
    u8 *ptr;
    u32 sum;
    u32 pos;
    s32 i;
    s32 attempt;
    s32 wc;
    s32 vc;
    u16 res;

    buf = (u8 *)gUnk_03000FFC;
    ie[0] = *(vu16 *)0x04000200;
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 = 0;
    flags = 0;
    if ((u8)slot_bit_test(0) == 1)
        flags = 1;
    out[0] = gSys.songSlot[0];
    slot_reset(0);
    if ((u8)slot_bit_test(1) == 1)
        flags |= 2;
    out[1] = gSys.songSlot[1];
    slot_reset(1);
    sound_dma_reset();
    ((u8 *)&flagWord)[0] = flags;
    buf[0] = 0x4D;
    buf[1] = 0x41;
    buf[2] = 0x4C;
    buf[3] = 0x52;
    buf[4] = 0x50;
    buf[5] = 0x47;
    buf[7] = 0;
    if (mode != 1)
    {
        if (mode == 0)
        {
            save_slot_store();
            idx = (gUnk_03000D40 << 0x1C) >> 0x1E;
            *(u32 *)(buf + idx * 0x6F8 + 0x700) = gSys.word24;
            MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gParty, buf + idx * 0x6F8 + 0x30, 0x1BC);
        }
        idx = (gUnk_03000D40 << 0x1C) >> 0x1E;
        buf[6] |= 1 << idx;
        slot = buf + idx * 0x6F8;
        slot[0x10] = idx;
        p = slot + 0x14;
        sum = 0;
        for (i = 0; i < 0x6F0; i += 0xC)
        {
            sum = sum + p[i] + p[i + 1] + p[i + 2] + p[i + 3] + p[i + 4] + p[i + 5]
                + p[i + 6] + p[i + 7] + p[i + 8] + p[i + 9] + p[i + 10] + p[i + 11];
        }
        slot[0x11] = sum;
        slot[0x12] = 0x41;
        slot[0x13] = 0x44;
        ptr = slot + 0x704;
        ptr[0] = 0;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;
    }
    blk = 0;
    wc = 1;
    vc = 1;
    for (attempt = 0; attempt < 5; attempt++)
    {
        res = 0;
        ptr = buf;
        pos = 0;
        for (i = 0; i < wc; i++)
        {
            res = eeprom_write_block_wait(pos >> 16, ptr);
            if (res != 0)
                break;
            ptr += 8;
            pos += 0x10000;
        }
        if (res == 0)
        {
            ptr = buf;
            pos = 0;
            for (i = 0; i < vc; i++)
            {
                res = eeprom_verify(pos >> 16, ptr);
                if (res != 0)
                    break;
                ptr += 8;
                pos += 0x10000;
            }
        }
        if (res == 0)
            break;
    }
    if (attempt != 5)
    {
        idx = (gUnk_03000D40 << 0x1C) >> 0x1E;
        blk = idx * 223 + 2;
        vc = 0xDF;
        for (attempt = 0; attempt < 5; attempt++)
        {
            res = 0;
            ptr = buf + idx * 0x6F8 + 0x10;
            pos = blk << 16;
            for (i = 0; i < 0xDF; i++)
            {
                res = eeprom_write_block_wait(pos >> 16, ptr);
                if (res != 0)
                    break;
                ptr += 8;
                pos += 0x10000;
            }
            if (res == 0)
            {
                ptr = buf + idx * 0x6F8 + 0x10;
                pos = blk << 16;
                for (i = 0; i < vc; i++)
                {
                    res = eeprom_verify(pos >> 16, ptr);
                    if (res != 0)
                        break;
                    ptr += 8;
                    pos += 0x10000;
                }
            }
            if (res == 0)
                break;
        }
        *(vu16 *)0x04000200 = ie[0];
        if (attempt == 5)
            return 0;
        return 1;
    }
    if (flagWord != 0)
    {
        if (flagWord & 1)
            slot_set_mode(0, 0xFF, 0x1E);
        if (flagWord & 2)
            slot_set_mode(1, 0xFF, 0x1E);
    }
    *(vu16 *)0x04000200 = ie[0];
    return 0;
}
END_NONMATCH

ASM_FUNC("asm/code_081233BC/save_slot.s", void save_slot(void))

ASM_FUNC("asm/code_081233BC/eeprom_store_blocks.s", void eeprom_store_blocks(void))
