#include "global.h"

struct ObjNode
{
    u32 unk00;
    u32 unk04;
    u32 unk08;
    s16 unk0C;
    u16 unk0E;
    u8 unk10;
    u8 unk11;
    u8 flags;
    u8 flags13;
    u16 slotMask;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E;
    u8 filler1F[0x22 - 0x1F];
    u8 unk22;
    u8 filler23[0x29 - 0x23];
    u8 flags29;
    u8 filler2A[0x2E - 0x2A];
    u16 rangeStart;
    u16 rangeLen;
    u8 filler32[0x38 - 0x32];
    void *unk38;
    u8 filler3C[0x44 - 0x3C];
    u32 block44;
    u32 block48;
    u32 block4C;
    u32 block50;
    s32 unk54;
    s32 unk58;
    u8 filler5C[0x64 - 0x5C];
    struct ObjNode *prev;
    struct ObjNode *next;
};

struct ObjListHead
{
    u8 mode;
    u8 count;
    u8 unk02;
    u8 unk03;
    u8 unk04;
    u8 unk05;
    u8 unk06;
    u8 filler07;
    u16 rangeEnd;
    u16 unk0A;
    u16 slots0C[(0x2C - 0x0C) / 2];
    struct BlockRec *firstBlock;
    struct ObjNode *first;
    struct ObjNode *last;
    u32 unk38;
    void *buf3C;
};

/* ObjListHead.slots0C: 16 slot words, OBJ_SLOT_FREE when unused, otherwise the owner's kind
   (bits 10-13) and id (bits 0-9). */
#define OBJ_SLOT_FREE 0xFFFF
#define OBJ_SLOT_KIND(slot) (((slot) >> 10) & 0xF)
#define OBJ_SLOT_ID(slot) ((slot) & 0x3FF)

struct BlockRec
{
    struct BlockRec *prev;
    struct BlockRec *next;
    u32 size;
    u8 kind;
    u8 sub;
    u16 id;
};

/* One entry of the sprite node buffer (ObjListHead.buf3C, 130 of them): a doubly linked list kept
   sorted by key; iwram_routine_03006c53 copies out the nodes in use (unused == 0), 8 bytes apart. */
struct NodeRec
{
    u32 unk0;
    u16 unk4;
    u16 unused;
    u32 key;
    struct NodeRec *prev;
    struct NodeRec *next;
};

struct NodeListHead
{
    u8 filler0[0x10];
    struct NodeRec *first;
};

struct NodeCopy
{
    u32 unk0;
    u16 unk4;
};

extern struct ObjListHead gUnk_0203FFB8;
void heap_free(void *p);
extern u16 gUnk_0203FFC4[];
extern struct BlockRec gUnk_0203FFA8;
extern void *gModelCtx;
extern u8 *gMemRoutines;
extern const u8 gHeapTagSpriteNodes[];
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void setup_sprite_buffers(u32 a3);
void init_node_list(struct NodeRec *node, u32 count);

struct ObjFlags12
{
    u8 b0 : 1;
    u8 lo12 : 2;
    u8 b34 : 2;
    u8 bit5 : 1;
    u8 b67 : 2;
};

struct Lo2Bits
{
    u8 lo2 : 2;
    u8 rest : 6;
};

struct Flags13Bits
{
    u8 lo2 : 2;
    u8 bit2 : 1; /* OBJ_OWNS_BLOCK48 */
    u8 bit3 : 1; /* OBJ_OWNS_BLOCK44 */
    u8 rest : 4;
};

/* ObjNode.flags13: the object allocated the block itself (obj_setup_blocks), so it frees it, or
 * hands it on to another object that shares it. */
#define OBJ_OWNS_BLOCK48 4 /* block48, a kind-2 block from alloc_list_entry */
#define OBJ_OWNS_BLOCK44 8 /* block44, a kind-3 block */

extern u16 gUnk_0203FFC0;

u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g);
void sprite_step_e6a0(struct ObjNode *obj);
u32 find_obj_1a2c(struct ObjNode *p, u32 mode);
void release_obj_131c(struct ObjNode *obj);
void obj_release_blocks(struct ObjNode *obj);
void release_block(void *node);
extern const u32 gTableEntryRows[];
extern u16 **gBankRecordTablePtrs[];
void *find_list_entry(u32 kind, u8 sub, u16 id);
void *alloc_list_entry(s32 kind, u32 size, u8 sub, u16 id);
void decode_e400(void *dst, u16 *src);

struct NodeCopy *iwram_routine_03006c53(struct NodeRec *node, struct NodeCopy *dst)
{
    struct NodeCopy *out = dst;

    while ((node = node->next) != NULL)
    {
        if (node->unused == 0)
        {
            u32 a = node->unk0;
            u16 b = node->unk4;

            out->unk0 = a;
            out->unk4 = b;
            out++;
        }
    }
    return out;
}

void sort_node_list(struct NodeListHead *list, struct NodeRec *end)
{
    struct NodeRec *cur = list->first;
    struct NodeRec *prev;
    struct NodeRec *moved;
    struct NodeRec *before;
    struct NodeRec *after;

    do
    {
        prev = cur->prev;
        moved = NULL;
        while (prev->key > cur->key)
        {
            before = prev->prev;
            after = cur->next;
            before->next = cur;
            after->prev = prev;
            cur->prev = before;
            prev->prev = cur;
            prev->next = after;
            cur->next = prev;
            moved = prev;
            prev = cur->prev;
        }
        if (moved != NULL)
            cur = moved->next;
        else
            cur = cur->next;
    } while (cur != end);
}

void init_node_flags(struct NodeRec *node, s32 count)
{
    count--;
    node->key = 0;
    node++;
    while (count != 0)
    {
        node->unused = 1;
        count--;
        node++;
    }
}

/* Links `count` nodes of a buffer into a doubly linked list, each marked unused with key -1. */
void init_node_list(struct NodeRec *node, u32 count)
{
    struct NodeRec *prev = NULL;
    struct NodeRec *next;

    while (count != 0)
    {
        node->unused = 1;
        node->key = -1;
        node->prev = prev;
        next = node + 1;
        node->next = next;
        prev = node;
        count--;
        /* never true: keeps `node++` a plain increment so the unrolled loop matches */
        if (prev != node)
            next = NULL;
        node++;
    }
    (node - 1)->next = NULL;
}

extern u8 gSys[];
/* The object-list routines copied into a heap block: gUnk_03000D48 is the block itself and its
 * first routine (sorts the object list, called with (head, head + 0xA14)), gUnk_03000D4C writes
 * the objects to OAM and returns the next OAM entry, gUnk_03000D50 a third routine. */
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u32 gUnk_03000D50;
/* Bit i set: sprite-command handler i (gUnk_03000D58[i]) has been loaded into its own heap block. */
extern u8 gUnk_03000D70;
/* SPRITE_CMD_HANDLER_COUNT sprite-command handlers, Thumb addresses (block + 1), called by
 * the sprite script interpreter in code_0801D358. */
extern u32 gUnk_03000D58[];

#define SPRITE_CMD_HANDLER_COUNT 6
#define THUMB_BIT 1 /* set in the address of a Thumb routine */

struct RecordSource
{
    u8 filler0[0x48];
    u32 *rec;
    u32 unk4C;
    u32 unk50;
};

void fill_record_08f4(struct RecordSource *p)
{
    u32 *rec = p->rec;

    rec[0] = 1;
    rec[1] = p->unk50;
    rec[2] = p->unk4C;
    rec[3] = 0;
}

/* Frees the routines copied into RAM for drawing objects: the object-list routine block
 * (gUnk_03000D48, with its entry points gUnk_03000D4C / gUnk_03000D50) and every loaded
 * sprite-command handler (gUnk_03000D58[i] where bit i of gUnk_03000D70 is set, its Thumb bit
 * removed to get the block back), then marks them all unloaded. */
void release_timers_0908(void)
{
    s32 i;
    u32 *handler;

    heap_free((void *)gUnk_03000D48);
    gUnk_03000D48 = 0;
    gUnk_03000D4C = 0;
    gUnk_03000D50 = 0;
    for (i = 0; i < SPRITE_CMD_HANDLER_COUNT; i++)
    {
        if ((gUnk_03000D70 >> i) & 1)
        {
            handler = &gUnk_03000D58[i];
            *handler = *handler - THUMB_BIT;
            heap_free((void *)*handler);
            *handler = 0;
        }
    }
    gUnk_03000D70 = 0;
}

NONMATCH("asm/code_08020798/init_obj_09a8.s", void init_obj_09a8(u16 a0, u8 a1, u8 a2, u8 a3))
{
    struct ObjListHead *obj = &gUnk_0203FFB8;
    struct BlockRec *aux = &gUnk_0203FFA8;
    s32 i;

    gModelCtx = obj;
    aux->next = NULL;
    aux->prev = NULL;
    aux->size = 0x0003FFB8;
    aux->kind = 0;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, obj, 0x40);
    obj->unk0A = a0;
    obj->rangeEnd = a0;
    obj->unk04 = a1;
    obj->unk03 = a1;
    obj->unk06 = a2;
    obj->unk05 = a2;
    obj->firstBlock = aux;
    for (i = 0; i < 16; i++)
        obj->slots0C[i] |= -1;
    obj->buf3C = heap_alloc(0xA28, ((u32)a3 >> 7) ^ 1, gHeapTagSpriteNodes, 0);
    setup_sprite_buffers(a3);
    init_node_list(obj->buf3C, 0x82);
}
END_NONMATCH

ASM_FUNC("asm/code_08020798/obj_list_pass_0a8c.s", void obj_list_pass_0a8c(void))

void sprite_update_cd0(struct ObjNode *obj)
{
    s32 res;
    u8 *base;

    if (obj->flags & 0x20)
    {
        ((struct ObjFlags12 *)&obj->flags)->bit5 = 0;
        ((struct ObjFlags12 *)&obj->flags)->lo12 = 0;
        if (obj->unk54 != 0)
        {
            res = find_or_alloc_obj_slots(obj->unk1E >> 4, obj->unk1C, ((u32)obj->flags29 << 27) >> 31, 1, 1, &obj->slotMask, -1);
            if (res == 2)
                ((struct Lo2Bits *)&obj->flags13)->lo2 = 0;
            else
            {
                base = gSys;
                ((struct Lo2Bits *)&obj->flags13)->lo2 = ((((u32)base[0x888] << 26) >> 31) + 1);
            }
        }
    }
}

void unlink_d54(struct ObjNode *obj)
{
    u16 *slot;
    s32 mask;
    struct ObjListHead *list;
    struct ObjNode *prev;
    struct ObjNode *next;

    if (!(obj->flags & 0x20))
    {
        obj->flags |= 0x20;
        if (find_obj_1a2c(obj, 2) == 0)
        {
            slot = gUnk_0203FFC4;
            mask = obj->slotMask;
            if (mask != 0)
            {
                do
                {
                    if ((*slot & 0x4000) && (mask & 1))
                        *slot = 0xFFFF;
                    slot++;
                    mask >>= 1;
                } while (mask != 0);
            }
        }
        list = &gUnk_0203FFB8;
        prev = obj->prev;
        if (prev != NULL)
            prev->next = obj->next;
        else
            list->first = obj->next;
        next = obj->next;
        if (next != NULL)
            next->prev = obj->prev;
        else
            list->last = obj->prev;
        obj->next = NULL;
        obj->prev = NULL;
        list->count--;
    }
}

ASM_FUNC("asm/code_08020798/create_obj_de4.s", void create_obj_de4(void))

ASM_FUNC("asm/code_08020798/setup_obj_10bc.s", void setup_obj_10bc(void))

void release_obj_131c(struct ObjNode *obj)
{
    u16 *slot;
    s32 mask;
    struct ObjListHead *list;
    struct ObjNode *prev;
    struct ObjNode *next;

    if (!(obj->flags & 0x20))
    {
        obj->flags |= 0x20;
        if (find_obj_1a2c(obj, 2) == 0)
        {
            slot = gUnk_0203FFC4;
            mask = obj->slotMask;
            if (mask != 0)
            {
                do
                {
                    if ((*slot & 0x4000) && (mask & 1))
                        *slot = 0xFFFF;
                    slot++;
                    mask >>= 1;
                } while (mask != 0);
            }
        }
    }
    obj_release_blocks(obj);
    list = &gUnk_0203FFB8;
    prev = obj->prev;
    if (prev != NULL)
        prev->next = obj->next;
    else
        list->first = obj->next;
    next = obj->next;
    if (next != NULL)
        next->prev = obj->prev;
    else
        list->last = obj->prev;
    release_block(obj);
    list->count--;
}

extern u8 *gMemRoutines;
extern u8 *gWindowData[];
u32 read_varlen_plus1(u8 *p);
void table_register(void *entry);

/*
 * Finds or makes the tile block (list kind 2) of table entry `index` of `bank` (an index with
 * bits 12-15 set picks bank (bits - 1) itself). The entry word gives the record (bits 18-26)
 * and the source data in gWindowData[bank] (bits 9-17); a negative word means no entry.
 * tiles == -1 sizes the block to the larger of the record's tile count and the data's.
 * A new block gets a 0x10-byte header (refcount 1, source, data pointer) and is registered
 * for loading. An existing one is returned only when reuse is nonzero; otherwise NULL.
 */
void *lookup_or_alloc_13b4(u32 bank, u32 index, s32 tiles, u8 reuse)
{
    u32 entryWord;
    u32 recIndex;
    u32 dataIndex;
    u32 minTiles;
    u8 bank8;
    u8 *rec;
    u8 *dataTbl;
    u8 *src;
    void *entry;

    if ((index & 0xF000) != 0)
    {
        bank = ((s32)index >> 12) - 1;
        index &= 0xFFF;
    }
    entryWord = *(u32 *)(gTableEntryRows[bank] + (index << 2));
    if ((s32)entryWord >= 0)
    {
        recIndex = (entryWord << 5) >> 23;
        dataIndex = (entryWord << 14) >> 23;
        rec = (u8 *)gBankRecordTablePtrs[bank][recIndex];
        dataTbl = gWindowData[bank];
        src = dataTbl + ((u32 *)dataTbl)[dataIndex];
        if (tiles == -1)
        {
            minTiles = (rec[4] | ((rec[5] & 1) << 8)) + 1;
            tiles = read_varlen_plus1(src) >> 5;
            if ((s32)tiles < (s32)minTiles)
                tiles = minTiles;
        }
        // 32 bytes per 4bpp tile.
        tiles <<= 5;
        bank8 = bank & 0xFF;
        entry = find_list_entry(2, bank8, dataIndex);
        if (entry == NULL)
        {
            entry = alloc_list_entry(2, tiles + 0x10, bank8, dataIndex);
            ((void (*)(u32, void *, u32))gMemRoutines)(0, entry, tiles + 0x10);
            *(u32 *)entry = 1;
            *(u8 **)((u8 *)entry + 4) = src;
            *(u8 **)((u8 *)entry + 8) = (u8 *)entry + 0x10;
            *(u32 *)((u8 *)entry + 0xC) = 0;
            table_register(entry);
        }
        else if (reuse == 0)
            return NULL;
        return entry;
    }
    return NULL;
}

/*
 * Finds or makes the decoded block (list kind 3) of the record behind table entry `index` of
 * `bank` (an index with bits 12-15 set picks bank (bits - 1) itself; entry word bits 18-26 give
 * the record). A record whose u16 at +2 is nonzero keeps its block prebuilt that many bytes
 * before itself. Otherwise the block is decode_e400's output: size == -1 sizes it to
 * groups * (count + 1) + 1 bytes, rounded up to a word. A new block is decoded from the record;
 * an existing one is returned only when reuse is nonzero, otherwise NULL.
 */
void *lookup_or_alloc_14b8(u32 bank, u32 index, s32 size, u8 reuse)
{
    u8 *rec;
    void *block;
    u32 recIndex;
    u32 prebuiltOfs;

    if ((index & 0xF000) != 0)
    {
        bank = ((s32)index >> 12) - 1;
        index &= 0xFFF;
    }
    recIndex = (*(u32 *)(gTableEntryRows[bank] + (index << 2)) & 0x7FFFFFF) >> 0x12;
    rec = (u8 *)gBankRecordTablePtrs[bank][recIndex];
    prebuiltOfs = rec[2] | (rec[3] << 8);
    if (prebuiltOfs == 0)
    {
        u8 *base = rec - (rec[0] | (rec[1] << 8));
        u32 perGroup = (rec[6] & 0x7F) + 1;
        u32 groups = base[0];

        if (size == -1)
            size = perGroup * groups + 1;
        size = (size + 3) & 0xFFFFFFFC;
        block = find_list_entry(3, bank & 0xFF, recIndex);
        if (block == NULL)
        {
            block = alloc_list_entry(3, size, bank & 0xFF, recIndex);
            decode_e400(block, (u16 *)rec);
        }
        else if (reuse == 0)
            return NULL;
    }
    else
        block = rec - prebuiltOfs;
    return block;
}

ASM_FUNC("asm/code_08020798/find_or_alloc_obj_slots.s", u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g))

/* Frees every slot from list->unk04 on that belongs to (group, id); returns a mask of the freed
   slots (bit i = slot i). Same as obj_lists_fill_free. */
static inline u16 free_slots_matching(struct ObjListHead *list, u32 group, u32 id)
{
    u16 *slots;
    u16 *p;
    u16 mask;
    s32 i;
    u32 kindField;
    u32 idField;

    slots = list->slots0C;
    mask = 0;
    for (i = list->unk04; i < 16; i++)
    {
        p = &slots[i];
        if (*p != OBJ_SLOT_FREE)
        {
            kindField = OBJ_SLOT_KIND(*p);
            idField = OBJ_SLOT_ID(*p);
            if (group == kindField && id == idField)
            {
                *p = OBJ_SLOT_FREE;
                mask |= 1 << i;
            }
        }
    }
    return mask;
}

/* Frees slot `slotIndex` of the object list together with every other slot (from list->unk04 on)
   owned by the same kind and id. Returns the mask of freed slots, or 0 if the slot was free. */
u16 free_matching_slots_1930(u32 slotIndex)
{
    struct ObjListHead *list;
    u16 *slots;
    u32 kind;
    u32 id;
    u16 freedMask;

    list = &gUnk_0203FFB8;
    slots = list->slots0C;
    freedMask = 0;
    if (slots[slotIndex] != OBJ_SLOT_FREE)
    {
        kind = OBJ_SLOT_KIND(slots[slotIndex]);
        id = OBJ_SLOT_ID(slots[slotIndex]);
        freedMask = free_slots_matching(list, kind, id);
    }
    return freedMask;
}

/* Looks for another object in the list (gUnk_0203FFB8) that shares something with `obj`, so a
 * resource is freed only when its last user goes. mode 2: a live object (flags bit 5 clear,
 * flags29 bit 4 clear) with the same key (the u32 at unk1C, masked to 0x00F0FFFF), which shares
 * obj's slots. Other modes need the same low nibble of unk1E too, and then: 0 the same unk18
 * (block48), 1 the same unk1A (block44), 3 the same rangeStart, 4 the same unk18 with no block44.
 * Returns the object found, or 0. */
u32 find_obj_1a2c(struct ObjNode *obj, u32 mode)
{
    struct ObjNode *other;

    other = gUnk_0203FFB8.first;
    if (other == NULL)
        return 0;
    if (mode == 2)
    {
        do
        {
            if (other != obj && !(other->flags & 0x20) && !(other->flags29 & 0x10)
                && (*(u32 *)&other->unk1C & 0x00F0FFFF) == (*(u32 *)&obj->unk1C & 0x00F0FFFF))
                return (u32)other;
            other = other->next;
        } while (other != NULL);
        return 0;
    }
    do
    {
        if (other != obj && (other->unk1E & 0xF) == (obj->unk1E & 0xF))
        {
            if ((mode == 0 && other->unk18 == obj->unk18)
                || (mode == 1 && other->unk1A == obj->unk1A)
                || (mode == 3 && other->rangeStart == obj->rangeStart)
                || (mode == 4 && other->unk18 == obj->unk18 && other->block44 == 0))
                return (u32)other;
        }
        other = other->next;
    } while (other != NULL);
    return 0;
}

static inline u8 *find_block_kind(u32 kind, u8 sub, u16 id)
{
    struct BlockRec *rec = gUnk_0203FFB8.firstBlock;

    do
    {
        if (rec->kind == kind && rec->sub == sub && rec->id == id)
            return (u8 *)rec - (rec->size - 0x10);
        rec = rec->next;
    } while (rec != NULL);
    return NULL;
}

NONMATCH("asm/code_08020798/obj_setup_blocks.s", void obj_setup_blocks(struct ObjNode *obj, u32 payload, u8 *table, s32 arg4, s32 size, s32 sizeB, s32 direct, s32 arg8))
{
    struct BlockRec *rec;
    u8 *found;
    u8 *link;
    u32 w;
    u32 sub;
    u32 id;
    struct ObjNode *other;
    u8 raw;
    u8 *blk;

    if (direct == 0)
    {
        raw = obj->unk1E;
        sub = ((u32)raw << 28) >> 28;
        id = obj->unk18;
        found = find_block_kind(2, sub, id);
        if (found == NULL)
        {
            blk = alloc_list_entry(2, size + 0x10, ((u32)raw << 28) >> 28, id);
            obj->block48 = (u32)blk;
            obj->block50 = payload;
            obj->block4C = (u32)(blk + 0x10);
            fill_record_08f4((struct RecordSource *)obj);
            ((struct Flags13Bits *)&obj->flags13)->bit2 = 1;
        }
        else
        {
            obj->block48 = (u32)found;
            obj->block50 = *(u32 *)(found + 4);
            obj->block4C = (u32)(found + 0x10);
            ((struct Flags13Bits *)&obj->flags13)->bit2 = 0;
        }
    }
    else
    {
        obj->block4C = payload;
        obj->block48 = 0;
        obj->block50 = 0;
    }
    if (sizeB != 0)
    {
        w = table[2] | (table[3] << 8);
        if (w == 0)
        {
            raw = obj->unk1E;
            sub = ((u32)raw << 28) >> 28;
            id = obj->unk1A;
            link = find_block_kind(3, sub, id);
            if (link == NULL)
            {
                link = alloc_list_entry(3, sizeB, ((u32)raw << 28) >> 28, id);
                ((struct Flags13Bits *)&obj->flags13)->bit3 = 1;
            }
            else
                ((struct Flags13Bits *)&obj->flags13)->bit3 = 0;
        }
        else
            link = table - w;
        obj->block44 = (u32)link;
    }
    else if (arg8 == 0)
    {
        other = (struct ObjNode *)find_obj_1a2c(obj, 4);
        if (other == NULL)
        {
            obj->rangeStart = gUnk_0203FFC0;
            gUnk_0203FFC0 += size;
        }
        else
            obj->rangeStart = other->rangeStart;
        obj->block44 = 0;
    }
    obj->unk38 = table;
    obj->unk54 = arg4;
    obj->unk58 = 0;
}
END_NONMATCH

/* Frees obj's blocks as it goes: a block it owns (block48, block44) is freed unless another object
 * shares it (find_obj_1a2c), which then becomes its owner. block44 above 0x07FFFFFF is in ROM and
 * never freed. With no block44, obj took its range from the top of the list's bump allocator
 * (rangeEnd), which is given back if obj's range is the topmost one and nobody else uses it. */
void obj_release_blocks(struct ObjNode *obj)
{
    struct ObjNode *sharer;
    struct ObjListHead *list;

    if (obj->block48 != 0 && (obj->flags13 & OBJ_OWNS_BLOCK48))
    {
        sharer = (struct ObjNode *)find_obj_1a2c(obj, 0);
        if (sharer == NULL)
            release_block((void *)obj->block48);
        else
            sharer->flags13 |= OBJ_OWNS_BLOCK48;
    }
    if (obj->block44 != 0)
    {
        if (obj->block44 <= 0x07FFFFFF && (obj->flags13 & OBJ_OWNS_BLOCK44))
        {
            sharer = (struct ObjNode *)find_obj_1a2c(obj, 1);
            if (sharer == NULL)
                release_block((void *)obj->block44);
            else
                sharer->flags13 |= OBJ_OWNS_BLOCK44;
        }
    }
    else
    {
        list = &gUnk_0203FFB8;
        if (obj->rangeStart + obj->rangeLen == list->rangeEnd)
        {
            if (find_obj_1a2c(obj, 3) == 0)
                list->rangeEnd -= obj->rangeLen;
        }
    }
}

void *alloc_list_entry(s32 kind, u32 size, u8 sub, u16 id)
{
    struct BlockRec *rec = gUnk_0203FFB8.firstBlock;
    struct BlockRec *blk;
    struct BlockRec *after;
    u8 *data;
    u32 avail;

    size += 0xF;
    size &= ~0xF;
    size += 0x10;
    do
    {
        avail = rec->size;
        if (rec->kind == 0 && (s32)avail >= (s32)size)
        {
            if (avail >= size + 0x10)
            {
                blk = (struct BlockRec *)((u8 *)rec - size);
                blk->prev = rec;
                after = rec->next;
                blk->next = after;
                if (after != NULL)
                    after->prev = blk;
                blk->size = rec->size - size;
                blk->kind = 0;
                rec->next = blk;
                rec->size = size;
            }
            else
            {
                rec->size = avail;
            }
            rec->kind = kind;
            rec->sub = sub;
            rec->id = id;
            data = (u8 *)rec - (rec->size - 0x10);
            switch (kind)
            {
            case 1:
                break;
            case 2:
                *(u32 *)data = 0;
                break;
            case 3:
                *data = 0;
                break;
            }
            return data;
        }
        rec = rec->next;
    } while (rec != NULL);
    return NULL;
}

void release_block(void *node)
{
    struct BlockRec *rec;
    struct BlockRec *prev;
    struct BlockRec *next;
    struct BlockRec *after;

    if (node != NULL)
    {
        rec = *(struct BlockRec **)((u8 *)node - 0x10);
        prev = rec->prev;
        next = rec->next;
        rec->kind = 0;
        if (next != NULL && next->kind == 0)
        {
            rec->size += next->size;
            after = next->next;
            rec->next = after;
            if (after != NULL)
                after->prev = rec;
        }
        if (prev != NULL && prev->kind == 0)
        {
            prev->size += rec->size;
            after = rec->next;
            prev->next = after;
            if (after != NULL)
                after->prev = prev;
        }
    }
}

void *find_list_entry(u32 kind, u8 sub, u16 id)
{
    struct BlockRec *rec = gUnk_0203FFB8.firstBlock;

    do
    {
        if (rec->kind == kind && rec->sub == sub && rec->id == id)
            return (u8 *)rec - (rec->size - 0x10);
        rec = rec->next;
    } while (rec != NULL);
    return NULL;
}

/* Frees every slot of the object list (from list->unk04 on) owned by this kind and id, and returns
   the mask of the freed slots (bit i = slot i). */
u16 obj_lists_fill_free(u32 kind, u32 id)
{
    return free_slots_matching(&gUnk_0203FFB8, kind, id);
}

extern const u32 gTableEntryRows[];

u32 rom_table_ptr(u32 bank, u32 index)
{
    if (index & 0xF000)
    {
        bank = ((s32)index >> 12) - 1;
        index &= 0xFFF;
    }
    return gTableEntryRows[bank] + (index << 2);
}

void sprite_update_cd0(struct ObjNode *obj);

extern u8 gSys[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u32 gUnk_03000D50;
extern u8 gUnk_03000D70;
extern u32 gUnk_03000D58[];
void obj_queue_update(struct ObjNode *obj)
{
    struct ObjListHead *list;

    if (obj->flags & 0x20)
    {
        if (obj->prev == obj->next)
        {
            list = &gUnk_0203FFB8;
            if (list->first == NULL)
                list->first = obj;
            else
            {
                obj->next = (*(struct ObjNode *volatile *)&list->last)->next;
                list->last->next = obj;
            }
            obj->prev = list->last;
            list->last = obj;
            list->count++;
        }
        sprite_update_cd0(obj);
    }
}

void obj_release_slots(struct ObjNode *obj)
{
    u16 *slot;
    s32 mask;

    if (!(obj->flags & 0x20))
    {
        obj->flags |= 0x20;
        if (find_obj_1a2c(obj, 2) == 0)
        {
            slot = gUnk_0203FFC4;
            mask = obj->slotMask;
            if (mask != 0)
            {
                do
                {
                    if ((*slot & 0x4000) && (mask & 1))
                        *slot = 0xFFFF;
                    slot++;
                    mask >>= 1;
                } while (mask != 0);
            }
        }
    }
}

void update_obj_list(void)
{
    struct ObjNode *obj = gUnk_0203FFB8.first;
    struct ObjNode *next;
    u32 kind;

    if (obj != NULL)
    {
        do
        {
            next = obj->next;
            if (obj->flags29 & 0x40)
            {
                sprite_step_e6a0(obj);
                if (obj->flags & 8)
                {
                    kind = obj->flags & 6;
                    if (kind == 4)
                        obj_release_slots(obj);
                    else if (kind == 6)
                        release_obj_131c(obj);
                }
            }
            obj = next;
        } while (obj != NULL);
    }
}
