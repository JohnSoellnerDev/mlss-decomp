#include "global.h"

struct SysRam
{
    u8 filler0[0x28];
    u16 keys28;
    u16 keys2A;
    u8 filler2C[0x8C0 - 0x2C];
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

ASM_FUNC("asm/code_081233BC/draw_party_stats_2.s", void draw_party_stats_2(void))

ASM_FUNC("asm/code_081233BC/draw_markers.s", void draw_markers(void))

void refresh_hud_tiles(u32 ctx)
{
    void *block = rom_table_entry(gSys.lang8C0 + 7, 0);
    unpack_to_block(block, (u8 *)ctx + 0x840, 0);
}

ASM_FUNC("asm/code_081233BC/draw_sprite.s", void draw_sprite(void))

ASM_FUNC("asm/code_081233BC/oam_write_attrs.s", void oam_write_attrs(void))

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

ASM_FUNC("asm/code_081233BC/save_game_flush_irq_off.s", void save_game_flush_irq_off(void))

ASM_FUNC("asm/code_081233BC/save_slot.s", void save_slot(void))

ASM_FUNC("asm/code_081233BC/eeprom_store_blocks.s", void eeprom_store_blocks(void))
