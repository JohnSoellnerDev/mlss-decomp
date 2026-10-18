/* agbcc: -funroll-loops */
#include "global.h"

struct SpriteLink
{
    u8 filler0[0xA];
    u8 flags;
    u8 filler0C[0x30 - 0xB];
    struct SpriteLink *next;
};

struct SpriteEntry
{
    u16 posX;
    u16 posY;
    u8 filler4[4];
    struct SpriteLink link8;
    u8 filler3C[0x7E - 0x3C];
    u8 state;
    u8 filler7F[0x12D - 0x7F];
    u8 type;
};

struct SpriteNode
{
    struct SpriteNode *next;
    void (*callback)(void *);
    struct SpriteEntry *entry;
    void *buffer;
    u16 timer;
};

struct SpriteInfo
{
    u8 unk0;
    u8 unk1;
};

struct PoolRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 flags11;
    u8 flags12;
    u8 flags13;
    u8 filler14[0x1F - 0x14];
    u8 flags1F;
    u8 unk20;
    u8 filler21[0x29 - 0x21];
    u8 flags29;
    u8 filler2A[0x5C - 0x2A];
    u32 unk5C;
    u32 unk60;
};
struct ChildBits
{
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct PoolFlags
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct PoolBits76
{
    u8 filler0[0x76];
    u8 bit0 : 1;
    u8 busy : 2;
    u8 rest : 5;
};

struct PoolBits77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct PoolObj
{
    struct PoolObj *next;
    void *unk4;
    struct PoolRecord *record;
    void *buffer;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x28 - 0x1C];
    struct PoolObj *ring;
    u32 unk2C;
    struct PoolObj *sibling;
    u8 filler34[0x38 - 0x34];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    u32 unk4C;
    u8 filler50[0x54 - 0x50];
    u32 unk54;
    u8 filler58[0x68 - 0x58];
    u32 link68;
    u32 unk6C;
    u8 filler70[0x76 - 0x70];
    u8 flags76;
    u8 filler77[2];
    u8 flags79;
    u8 filler7A[0x7C - 0x7A];
    u16 unk7C;
    u8 filler7E[0x90 - 0x7E];
    u16 unk90;
    u8 filler92[0x9C - 0x92];
    u32 unk9C;
    u32 unkA0;
    u32 unkA4;
    u32 unkA8;
    u8 fillerAC[0xC5 - 0xAC];
    u8 unkC5;
    u8 fillerC6[0xD0 - 0xC6];
};

extern u8 *gWorkBuf;
extern u8 *gMemRoutines;

struct SpriteEnt
{
    u8 filler0[0x12];
    u8 flags12;
    u8 flags13;
    u16 mask14;
    u16 unk16;
    u16 half1C;
    u8 byte1E;
    u8 filler1F[0x54 - 0x1F];
    u32 ptr54;
    u8 filler58[0x68 - 0x58];
    struct SpriteEnt *next68;
};

struct ModelCtx
{
    u8 filler0[0x30];
    struct SpriteEnt *list30;
};

extern struct ModelCtx *gModelCtx;

struct SpriteWorld
{
    u8 unk0[4];
    u8 counter4;
    u8 counter5;
    u8 unk6[0xD - 6];
    u8 unkD;
    u8 unkE[0x12 - 0xE];
    u16 unk12;
    u8 unk14[0x16 - 0x14];
    u16 scrollX;
    u16 scrollY;
    u8 unk1A[0x1C - 0x1A];
    struct SpriteInfo *info;
    u8 unk20[0x3C - 0x20];
    struct SpriteNode *freeNodes;
    u8 unk40[0x44 - 0x40];
    struct SpriteNode *spriteList;
    u8 unk48[0x4C - 0x48];
    struct SpriteNode *list4C;
    u8 unk50[0x58 - 0x50];
    struct SlotHead *slots[8];
    struct PoolObj *list78;
    u8 unk7C[0x220 - 0x7C];
    struct SpriteEnt *slot220;
    struct SpriteEnt *slot224;
    u8 unk228[0x244 - 0x228];
    void *resource244;
    struct SlotTable *table248;
};

struct SlotMove
{
    u8 filler0[0x24];
    u16 dx;
    u16 dy;
};

struct SlotHead
{
    u8 filler0[0xF0];
    struct SlotMove *move;
};

struct StateBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 kind : 3;
    u8 top : 2;
};

struct SlotTable
{
    u8 filler0[0x2C];
    u8 *records;
};

struct MaskEntry
{
    u8 filler0[0x14];
    u16 mask;
};

struct Bit0Byte
{
    u8 bit0 : 1;
    u8 rest : 7;
};

extern struct SpriteWorld *gWorld;
extern const u8 gRomMotionTable[];

void heap_free(void *p);
void obj_release_node(void *node);
void obj_list_remove(void *node);
void set_state_word_1780(void *resource, u32 value);
void sprite_entry_update(void *entPtr);
void alloc_slot_for_node(void);
void obj_bind_slots(void *rec);
void list_remove_active(void *node);
u32 sprite_node_release(u32 p);
void sprite_update_cd0(void *sprite);
void list_push_active(void *obj);
u32 find_entry_code(struct SlotTable *tbl, u32 v);
u32 place_part_in_slot(s32 slot, s32 mode, s32 a2, s32 a3, s32 s0, s32 s4, s32 s8);
void menu_entry_refresh(void *entry, u32 idx, u32 arg);
struct PoolObj *obj_spawn(struct PoolObj *tmpl, u32 flags, s32 x, s32 y, s32 z, u32 vis, u32 extra);

NONMATCH("asm/code_0807B7A0/update_sprite_list.s", void update_sprite_list(void))
{
    struct SpriteNode *node = gWorld->spriteList;
    struct SpriteEntry *obj;
    u32 flags;

    if (node != NULL)
    {
        do
        {
            s16 timer;
            obj = node->entry;
            timer = --node->timer;

            if (timer == 0)
            {
                goto release;
            }
            flags = obj->link8.flags;
            if (timer < 0 && (8 & flags))
            {
                goto release;
            }
            obj->posX = 0x78;
            obj->posY = 0x50;
            if (0x20 & flags)
            {
                sprite_entry_update(obj);
                node->callback(node);
            }
            goto next;

        release:
            obj_release_node(obj);
            if (node->buffer != NULL)
            {
                heap_free(node->buffer);
            }
            node->callback = NULL;

        next:
            node = node->next;
        } while (node != NULL);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0807B7A0/place_part_in_slot.s", u32 place_part_in_slot(s32 slot, s32 mode, s32 a2, s32 a3, s32 s0, s32 s4, s32 s8))

void release_object(struct SpriteEntry *obj)
{
    if (obj != NULL)
    {
        u32 st = obj->state & 6;
        struct StateBits *bits = (struct StateBits *)&obj->state;
        struct SpriteLink *child;

        if (st == 2)
        {
            gWorld->counter4--;
        }
        else if (st == 4)
        {
            gWorld->counter5--;
        }
        child = &obj->link8;
        if (child != NULL)
        {
            do
            {
                obj_list_remove(child);
                child = child->next;
            } while (child != NULL);
        }
        set_state_word_1780(gWorld->resource244, gWorld->counter5);
        bits->mode = 3;
        bits->kind = 0;
        if (obj->type == 0x1D)
        {
            ((struct Bit0Byte *)&gWorld->unkD)->bit0 = 0;
        }
    }
}

u32 place_from_table(s32 slot, s32 mode, s32 flag)
{
    struct SpriteWorld *world = gWorld;
    struct SlotHead *head = world->slots[slot];
    u32 result;

    if (head == NULL)
    {
        result = 0;
    }
    else
    {
        if (mode >= 0)
        {
            if (head->move != NULL)
            {
                if (flag != 0)
                {
                    world->scrollX += head->move->dx;
                    world->scrollY += head->move->dy;
                }
            }
        }
        {
            u32 index = gWorld->info->unk1;
            u8 *tbl = (u8 *)gRomMotionTable + (((index << 1) + index) << 3) + (slot << 2);
            result = place_part_in_slot(slot, mode,
                ((s32)(*(u16 *)tbl << 22)) >> 22,
                ((s32)(*(u32 *)tbl << 12)) >> 22,
                ((s32)(*(u16 *)(tbl + 2) << 18)) >> 22,
                (*(u32 *)tbl << 1) >> 31,
                0);
        }
    }
    return result;
}

ASM_FUNC("asm/code_0807B7A0/alloc_slot_for_node.s", void alloc_slot_for_node(void))

ASM_FUNC("asm/code_0807B7A0/alloc_slot_refresh_for_node.s", void alloc_slot_refresh_for_node(void))

ASM_FUNC("asm/code_0807B7A0/refresh_slots_for_mask.s", void refresh_slots_for_mask(void))


ASM_FUNC("asm/code_0807B7A0/sprite_entry_update.s", void sprite_entry_update(void *entPtr))

ASM_FUNC("asm/code_0807B7A0/obj_bind_slots.s", void obj_bind_slots(void *rec))

ASM_FUNC("asm/code_0807B7A0/obj_spawn.s", struct PoolObj *obj_spawn(struct PoolObj *tmpl, u32 flags, s32 x, s32 y, s32 z, u32 vis, u32 extra))

ASM_FUNC("asm/code_0807B7A0/obj_spawn_linked.s", void obj_spawn_linked(void))

struct PoolObj *pool_slot_init(s32 slot, u32 value)
{
    struct PoolObj *obj;
    struct PoolRecord *child;

    if (slot <= 7)
    {
        goto none;
    }
    if (slot > 0xF)
    {
        goto none;
    }
    {
        u8 *base = gWorkBuf + (slot * 0xD0 + 0x980);
        obj = (struct PoolObj *)(base + (s32)-0x680);
    }
    if (((struct PoolFlags *)&obj->flags76)->mode == 3)
    {
none:
        return NULL;
    }
    child = obj->record;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, obj, 0xD0);
    obj->record = child;
    obj->next = gWorld->list78;
    gWorld->list78 = obj;
    ((struct PoolFlags *)&obj->flags76)->mode = 3;
    obj->unk90 = 0x100;
    child->unk20 = 0x10;
    {
        struct ChildBits *bits = (struct ChildBits *)&child->flags1F;
        bits->f0 = 2;
        bits->f2 = 2;
        bits->f4 = 2;
        bits->f6 = 2;
    }
    if (value != 0xFFFFFF00)
    {
        obj->unkC5 = value;
    }
    return obj;
}

ASM_FUNC("asm/code_0807B7A0/obj_release.s", void obj_release(void))

ASM_FUNC("asm/code_0807B7A0/draw_sprite_nodes.s", void draw_sprite_nodes(void))

ASM_FUNC("asm/code_0807B7A0/oam_place_points.s", void oam_place_points(void))

ASM_FUNC("asm/code_0807B7A0/emit_active_parts_oam.s", void emit_active_parts_oam(void))
