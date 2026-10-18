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
    u8 filler0;
    u8 count;
    u8 filler2;
    u8 unk03;
    u8 unk04;
    u8 unk05;
    u8 unk06;
    u8 filler07;
    u16 rangeEnd;
    u16 unk0A;
    u8 filler0C[0x2C - 0x0C];
    struct BlockRec *firstBlock;
    struct ObjNode *first;
    struct ObjNode *last;
    u8 filler38[0x3C - 0x38];
    void *buf3C;
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

struct NodeRec
{
    u32 unk0;
    u16 unk4;
    u16 unk6;
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
        if (node->unk6 == 0)
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
    s32 rem;
    s32 lim;
    u16 flag;
    struct NodeRec *q;

    count--;
    node->key = 0;
    node++;
    if (count == 0)
        return;
    flag = 1;
    rem = -count & 3;
    if (rem == 0)
        goto loop;
    lim = 3;
    if (rem >= lim)
        goto one;
    lim = 2;
    if (rem >= lim)
        goto two;
    node->unk6 = flag;
    count--;
    node++;
two:
    node->unk6 = flag;
    count--;
    node++;
one:
    node->unk6 = flag;
    count--;
    node++;
    if (count == 0)
        return;
loop:
    do
    {
        node[0].unk6 = flag;
        node[1].unk6 = flag;
        node[2].unk6 = flag;
        q = node + 3;
        q->unk6 = flag;
        count -= 4;
        node += 4;
    } while (count != 0);
}

ASM_FUNC("asm/code_08020798/init_node_list.s", void init_node_list(void))

extern u8 gSys[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u32 gUnk_03000D50;
extern u8 gUnk_03000D70;
extern u32 gUnk_03000D58[];

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

ASM_FUNC("asm/code_08020798/lookup_or_alloc_13b4.s", void lookup_or_alloc_13b4(void))

void *lookup_or_alloc_14b8(u32 bank, u32 index, s32 size, u8 flag)
{
    u8 *hdr;
    void *iVar1;
    u32 uVar2;
    u32 span;

    if ((index & 0xF000) != 0)
    {
        bank = ((s32)index >> 12) - 1;
        index &= 0xFFF;
    }
    uVar2 = (*(u32 *)(gTableEntryRows[bank] + (index << 2)) & 0x7FFFFFF) >> 0x12;
    hdr = (u8 *)gBankRecordTablePtrs[bank][uVar2];
    span = hdr[2] | (hdr[3] << 8);
    if (span == 0)
    {
        u8 *base = hdr - (hdr[0] | (hdr[1] << 8));
        u32 cnt = (hdr[6] & 0x7F) + 1;
        u32 unit = base[0];

        if (size == -1)
            size = cnt * unit + 1;
        size = (size + 3) & 0xFFFFFFFC;
        iVar1 = find_list_entry(3, bank & 0xFF, uVar2);
        if (iVar1 == NULL)
        {
            iVar1 = alloc_list_entry(3, size, bank & 0xFF, uVar2);
            decode_e400(iVar1, (u16 *)hdr);
        }
        else if (flag == 0)
            return NULL;
    }
    else
        iVar1 = hdr - span;
    return iVar1;
}

ASM_FUNC("asm/code_08020798/find_or_alloc_obj_slots.s", u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g))

ASM_FUNC("asm/code_08020798/free_matching_slots_1930.s", void free_matching_slots_1930(void))

u32 find_obj_1a2c(struct ObjNode *p, u32 mode)
{
    struct ObjNode *cur;

    cur = gUnk_0203FFB8.first;
    if (cur == NULL)
        return 0;
    if (mode == 2)
    {
        do
        {
            if (cur != p && !(cur->flags & 0x20) && !(cur->flags29 & 0x10)
                && (*(u32 *)&cur->unk1C & 0x00F0FFFF) == (*(u32 *)&p->unk1C & 0x00F0FFFF))
                return (u32)cur;
            cur = cur->next;
        } while (cur != NULL);
        return 0;
    }
    do
    {
        if (cur != p && (cur->unk1E & 0xF) == (p->unk1E & 0xF))
        {
            if ((mode == 0 && cur->unk18 == p->unk18)
                || (mode == 1 && cur->unk1A == p->unk1A)
                || (mode == 3 && cur->rangeStart == p->rangeStart)
                || (mode == 4 && cur->unk18 == p->unk18 && cur->block44 == 0))
                return (u32)cur;
        }
        cur = cur->next;
    } while (cur != NULL);
    return 0;
}

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

ASM_FUNC("asm/code_08020798/alloc_list_entry.s", void *alloc_list_entry(s32 kind, u32 size, u8 sub, u16 id))

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
