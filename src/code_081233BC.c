#include "global.h"
#include "ram_routines.h"

struct SysRam
{
    u8 filler0[0x24];
    u32 playTime;
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

struct PartyMember
{
    u8 filler0[0x32];
    u8 unk32;
    u8 filler33[0x36 - 0x33];
    u8 unk36;
    u8 unk37;
    u8 filler38[0x3C - 0x38];
};

struct PartyState
{
    struct PartyMember members[2];
    u16 unk78[8];
    u16 unk88;
    u8 unk8A[0x20];
    u8 unkAA[8];
    u8 unkB2[8];
    u8 unkBA[0x38];
    u8 unkF2[0x38];
    u8 unk12A[0x10];
    u8 unk13A[0x38];
    u8 unk172[0x38];
    u8 unk1AA[0x10];
};

struct RowEntry
{
    u8 filler0[6];
    u8 unk6;
    u8 filler7[0xA - 0x7];
    u8 unkA;
    u8 fillerB[0x14 - 0xB];
};

#define PARTY ((struct PartyState *)gParty)

/* One save slot in the EEPROM image (gUnk_03000FFC + 0x10 + n * 0x6F8). */
struct SaveSlot
{
    u8 index;               /* slot number 0-2 */
    u8 sum;                 /* low byte of the sum of the 0x6F0 bytes from data */
    u8 tag[2];              /* "AD" once written */
    u8 data[0x20 - 0x4];
    u8 party[0x1BC];        /* copy of the gParty block */
    u8 data1DC[0x6F0 - 0x1DC];
    u32 playTime;           /* gSys.playTime when saved */
    u8 tail[4];
};

/* gUnk_03000D40: save/session state bits. */
struct SaveCtl
{
    u8 lo2 : 2;             /* bit 0: serial buffers allocated, bit 1: game loaded from a save */
    u8 slot : 2;            /* selected save slot */
    u8 rest : 4;
};

/* The selected save slot, from gUnk_03000D40. */
#define CUR_SAVE_SLOT (((struct SaveCtl *)&gUnk_03000D40)->slot)

static inline u8 save_checksum(u8 *data, s32 size)
{
    u32 sum = 0;
    s32 i;

    for (i = 0; i < size; i++)
        sum += data[i];
    return sum;
}

/* The EEPROM save image gUnk_03000FFC points at. */
struct SaveFile
{
    u8 magic[6];            /* "MALRPG" */
    u8 valid;               /* bit n: slot n holds a save */
    u8 unk7;
    u8 filler8[8];
    struct SaveSlot slots[3];
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
extern const struct PartyMember gPartyDefaultRecords[];
extern const struct RowEntry gRowTable[];

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
void recalc_char_stats(u8 idx);
void set_sys24_request(u32 value);

ASM_FUNC("asm/code_081233BC/draw_party_stats_2.s", void draw_party_stats_2(void))

ASM_FUNC("asm/code_081233BC/draw_markers.s", void draw_markers(void))

void refresh_hud_tiles(u32 ctx)
{
    void *block = rom_table_entry(gSys.lang8C0 + 7, 0);
    unpack_to_block(block, (u8 *)ctx + 0x840, 0);
}

ASM_FUNC("asm/code_081233BC/draw_sprite.s", void draw_sprite(void))

/* Packs one GBA OAM entry's three attributes into oam[0..2] and returns the next entry (oam + 4,
 * skipping the affine parameter halfword). Each field is ORed in as given, unmasked. */
u16 *oam_write_attrs(u16 *oam, u16 y, u8 affineMode, u8 objMode, u8 mosaic, u8 is8bpp, u8 shape, u16 x, u8 matrixOrFlip, u8 size, u16 tile, u8 priority, u8 palette)
{
    /* attr0: Y 0-7, affine/double-size 8-9, mode 10-11, mosaic 12, 256 colours 13, shape 14-15 */
    oam[0] = y | (shape << 14 | is8bpp << 13 | mosaic << 12 | objMode << 10 | affineMode << 8);
    /* attr1: X 0-8, affine matrix (or H/V flip in bits 12-13) 9-13, size 14-15 */
    oam[1] = x | (size << 14 | matrixOrFlip << 9);
    /* attr2: tile 0-9, priority 10-11, palette 12-15 */
    oam[2] = tile | (palette << 12 | priority << 10);
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

/*
 * Resets the party block for a new game: copies both members' default
 * records (0x1E halfwords each), and for each member whose id (unk32) is set
 * (bit 7 clear) takes two bytes from that id's gRowTable row and recalculates
 * the member's stats. Then clears the 8 halfwords at unk78 (and points
 * gUnk_03000FF8 at them), marks every slot of the party's tables empty
 * (0xFF, unkB2 zero), flags both member ids in unkF2 and lists them first in
 * unk172.
 */
void init_party_state(void)
{
    u8 i;
    vu16 zero;

    for (i = 0; i < 2; i++)
        cpu_set(&gPartyDefaultRecords[i], &PARTY->members[i], 0x1E);
    if (!(PARTY->members[0].unk32 & 0x80))
    {
        const struct RowEntry *rowTable = gRowTable;

        PARTY->members[0].unk36 = rowTable[PARTY->members[0].unk32].unk6;
        PARTY->members[0].unk37 = rowTable[PARTY->members[0].unk32].unkA;
        recalc_char_stats(0);
    }
    if (!(PARTY->members[1].unk32 & 0x80))
    {
        const struct RowEntry *rowTable = gRowTable;

        PARTY->members[1].unk36 = rowTable[PARTY->members[1].unk32].unk6;
        PARTY->members[1].unk37 = rowTable[PARTY->members[1].unk32].unkA;
        recalc_char_stats(1);
    }

    /* fill mode (bit 24), 8 halfwords */
    zero = 0;
    cpu_set((void *)&zero, PARTY->unk78, 0x1000008);
    gUnk_03000FF8 = (u32)PARTY->unk78;
    PARTY->unk88 = 0;

    /* every table entry starts empty */
    for (i = 0; i < 0x20; i++)
        PARTY->unk8A[i] |= 0xFF;
    for (i = 0; i < 8; i++)
        PARTY->unkB2[i] = 0;
    for (i = 0; i < 8; i++)
        PARTY->unkAA[i] |= 0xFF;
    for (i = 0; i < 0x38; i++)
        PARTY->unkBA[i] |= 0xFF;
    for (i = 0; i < 0x38; i++)
        PARTY->unkF2[i] |= 0xFF;
    for (i = 0; i < 0x10; i++)
        PARTY->unk12A[i] |= 0xFF;
    for (i = 0; i < 0x38; i++)
        PARTY->unk13A[i] |= 0xFF;
    for (i = 0; i < 0x38; i++)
        PARTY->unk172[i] |= 0xFF;
    for (i = 0; i < 0x10; i++)
        PARTY->unk1AA[i] |= 0xFF;

    /* the members' own ids are flagged and listed first */
    PARTY->unkF2[PARTY->members[0].unk32] = 1;
    PARTY->unkF2[PARTY->members[1].unk32] = 1;
    PARTY->unk172[0] = PARTY->members[0].unk32;
    PARTY->unk172[1] = PARTY->members[1].unk32;
    set_sys24_request(0);
}

ASM_FUNC("asm/code_081233BC/give_reward.s", void give_reward(void))

ASM_FUNC("asm/code_081233BC/recalc_char_stats.s", void recalc_char_stats(u8 idx))

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

void state_load(void)
{
    u32 base = gUnk_03000FFC;
    u32 offset;
    gParty = heap_alloc2(0x1BC, 1, gPartyStateTag, 0);
    offset = ((gUnk_03000D40 << 28) >> 30) * 0x6F8;
    cpu_set((void *)(offset + base + 0x30), gParty, 0xDE);
    gUnk_03000FF8 = (u32)(gParty + 0x78);
}

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
            *(u32 *)(buf + idx * 0x6F8 + 0x700) = gSys.playTime;
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

/*
 * Fills the EEPROM save image's header and the selected save slot before it is
 * written out. mode 0 stores the current game (play time and party) into the
 * slot, 1 erases the slot; any other mode only refreshes its checksum and tags.
 */
void save_slot(u32 mode)
{
    struct SaveFile *save;
    u32 zero;
    s32 i;

    save = (struct SaveFile *)gUnk_03000FFC;
    save->magic[0] = 'M';
    save->magic[1] = 'A';
    save->magic[2] = 'L';
    save->magic[3] = 'R';
    save->magic[4] = 'P';
    save->magic[5] = 'G';
    save->unk7 = 0;
    if (mode != 1)
    {
        if (mode == 0)
        {
            save_slot_store();
            save->slots[CUR_SAVE_SLOT].playTime = gSys.playTime;
            /* 0xDE halfwords: the whole 0x1BC-byte party block */
            cpu_set(gParty, save->slots[CUR_SAVE_SLOT].party, 0xDE);
        }
        save->valid |= 1 << CUR_SAVE_SLOT;
        {
            register u32 bits asm("r2") = gUnk_03000D40 << 0x1C;
            register u32 slot asm("r1") = bits >> 0x1E;
            save->slots[slot].index = bits = slot;
        }
        save->slots[CUR_SAVE_SLOT].sum = save_checksum(save->slots[CUR_SAVE_SLOT].data, 0x6F0);
        save->slots[CUR_SAVE_SLOT].tag[0] = 'A';
        save->slots[CUR_SAVE_SLOT].tag[1] = 'D';
        for (i = 0; i < 4; i++)
            save->slots[CUR_SAVE_SLOT].tail[i] = 0;
    }
    else
    {
        /* 32-bit fill of 0x1BE words: the whole 0x6F8-byte slot */
        zero = 0;
        cpu_set(&zero, &save->slots[CUR_SAVE_SLOT], 0x050001BE);
        save->valid &= ~(1 << CUR_SAVE_SLOT);
    }
}

ASM_FUNC("asm/code_081233BC/eeprom_store_blocks.s", void eeprom_store_blocks(void))
