#include "global.h"

struct ObjNode
{
    u8 filler0[0x12];
    u8 flags;
    u8 flags13;
    u16 slotMask;
    u8 filler16[0x18 - 0x16];
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E;
    u8 filler1F[0x29 - 0x1F];
    u8 flags29;
    u8 filler2A[0x2E - 0x2A];
    u16 rangeStart;
    u16 rangeLen;
    u8 filler32[0x44 - 0x32];
    u32 block44;
    u32 block48;
    u8 filler4C[0x54 - 0x4C];
    s32 unk54;
    u8 filler58[0x64 - 0x58];
    struct ObjNode *prev;
    struct ObjNode *next;
};

struct ObjListHead
{
    u8 filler0;
    u8 count;
    u8 filler2[6];
    u16 rangeEnd;
    u8 filler0A[0x2C - 0x0A];
    struct BlockRec *firstBlock;
    struct ObjNode *first;
    struct ObjNode *last;
};

struct BlockRec
{
    struct BlockRec *prev;
    struct BlockRec *next;
    u32 size;
    u8 kind;
    u8 sub;
    u16 id;
};

extern struct ObjListHead gUnk_0203FFB8;
void heap_free(void *p);
extern u16 gUnk_0203FFC4[];

u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g);
void sprite_step_e6a0(struct ObjNode *obj);
u32 find_obj_1a2c(struct ObjNode *p, u32 mode);
void release_obj_131c(struct ObjNode *obj);
void obj_release_blocks(struct ObjNode *obj);
void release_block(void *node);

ASM_FUNC("asm/code_08020798/iwram_routine_03006c53.s", void iwram_routine_03006c53(void))

ASM_FUNC("asm/code_08020798/sub_080207BC.s", void sub_080207BC(void))

ASM_FUNC("asm/code_08020798/sub_080207FC.s", void sub_080207FC(void))

ASM_FUNC("asm/code_08020798/init_node_list.s", void init_node_list(void))

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

ASM_FUNC("asm/code_08020798/release_timers_0908.s", void release_timers_0908(void))

ASM_FUNC("asm/code_08020798/init_obj_09a8.s", void init_obj_09a8(void))

ASM_FUNC("asm/code_08020798/obj_list_pass_0a8c.s", void obj_list_pass_0a8c(void))

ASM_FUNC("asm/code_08020798/sprite_update_cd0.s", void sprite_update_cd0(struct ObjNode *obj))

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

ASM_FUNC("asm/code_08020798/lookup_or_alloc_13b4.s", void lookup_or_alloc_13b4(void))

ASM_FUNC("asm/code_08020798/lookup_or_alloc_14b8.s", void lookup_or_alloc_14b8(void))

ASM_FUNC("asm/code_08020798/find_or_alloc_obj_slots.s", u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g))

ASM_FUNC("asm/code_08020798/free_matching_slots_1930.s", void free_matching_slots_1930(void))

ASM_FUNC("asm/code_08020798/find_obj_1a2c.s", u32 find_obj_1a2c(struct ObjNode *p, u32 mode))

ASM_FUNC("asm/code_08020798/obj_setup_blocks.s", void obj_setup_blocks(void))

void obj_release_blocks(struct ObjNode *obj)
{
    struct ObjNode *other;
    struct ObjListHead *list;

    if (obj->block48 != 0 && (obj->flags13 & 4))
    {
        other = (struct ObjNode *)find_obj_1a2c(obj, 0);
        if (other == NULL)
            release_block((void *)obj->block48);
        else
            other->flags13 |= 4;
    }
    if (obj->block44 != 0)
    {
        if (obj->block44 <= 0x07FFFFFF && (obj->flags13 & 8))
        {
            other = (struct ObjNode *)find_obj_1a2c(obj, 1);
            if (other == NULL)
                release_block((void *)obj->block44);
            else
                other->flags13 |= 8;
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

ASM_FUNC("asm/code_08020798/alloc_list_entry.s", void alloc_list_entry(void))

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

ASM_FUNC("asm/code_08020798/obj_lists_fill_free.s", void obj_lists_fill_free(void))

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

ASM_FUNC("asm/code_08020798/obj_queue_update.s", void obj_queue_update(void))

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
