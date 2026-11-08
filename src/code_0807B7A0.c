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
    u16 mask14;
    u8 filler16[0x1C - 0x16];
    u16 half1C;
    u8 filler1E;
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

struct RecFlags11
{
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct RecFlags12
{
    u8 lo4 : 4;
    u8 b4 : 1;
    u8 hi3 : 3;
};

struct RecFlags13
{
    u8 lo2 : 2;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 rest : 4;
};

struct RecFlags29
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
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
    u32 oamStart;
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
    u8 filler77;
    u8 frame78;
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
    u8 fillerC6;
    s8 biasX;
    u8 fillerC8[0xD0 - 0xC8];
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
    u8 filler18[0x1C - 0x18];
    u16 half1C;
    u8 byte1E;
    u8 filler1F[0x54 - 0x1F];
    u32 ptr54;
    u8 filler58[0x68 - 0x58];
    struct SpriteEnt *next68;
};

struct ModelCtx
{
    u8 filler0[0xC];
    u16 slotTab[(0x30 - 0xC) / 2];
    struct SpriteEnt *list30;
    u32 unk34;
    u32 unk38;
};

extern struct ModelCtx *gModelCtx;

struct DrawChild
{
    struct DrawChild *next;
    s32 x;
    s32 y;
    s32 z;
    u8 filler10[0x1B - 0x10];
    u8 flags;
};

struct DrawNode
{
    struct DrawNode *next;
    struct PoolObj *obj;
    struct DrawChild *child;
    u8 bits;
};

struct SpriteWorld
{
    u8 unk0[4];
    u8 counter4;
    u8 counter5;
    u8 unk6;
    u8 oamRow;
    u8 unk8[0xA - 8];
    u8 unkA;
    u8 unkB[0xD - 0xB];
    u8 unkD;
    u8 unkE[0x12 - 0xE];
    u16 unk12;
    u16 unk14;
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
    u8 unk7C[0x1C0 - 0x7C];
    u32 frames[8];
    u8 unk1E0[0x220 - 0x1E0];
    struct SpriteEnt *slot220;
    struct SpriteEnt *slot224;
    u8 unk228[0x238 - 0x228];
    struct DrawNode *nodeFree;
    struct DrawNode *nodeUsed;
    u8 unk240[0x244 - 0x240];
    void *resource244;
    struct SlotTable *table248;
    s32 *camera;
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

struct SpriteFlags12
{
    u8 bit0 : 1;
    u8 mid : 2;
    u8 rest3 : 2;
    u8 bit5 : 1;
    u8 top : 2;
};

struct SpriteFlags13
{
    u8 lo2 : 2;
    u8 rest : 6;
};

extern struct SpriteWorld *gWorld;
extern const u8 gRomMotionTable[];

void heap_free(void *p);
void obj_release_node(void *node);
void obj_list_remove(void *node);
void set_state_word_1780(void *resource, u32 value);
void sprite_entry_update(struct SpriteEnt *ent);
void alloc_slot_for_node(void);
void setup_obj_10bc(void *rec, u32 a, u32 group, u32 b, s32 c, s32 d, s32 e, s32 f);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
u32 block_flag_byte(void *obj);
void oam_list_rows_a(u32 list, u32 count, u32 layer);
void oam_list_rows_b(u32 list, u32 count);
void offset_three_words(void *camera, s32 *a, s32 *b, s32 *c, u32 hide);
void oam_place_points(u32 *cursor, u32 count, s32 x, s32 y, u32 extra, u32 attr0Or, u32 mirror);
u32 sprite_alloc(void *ctx, u32 a, u32 b, u32 c, u32 d, u32 e);
void obj_bind_slots(struct PoolRecord *rec);
void obj_release_slots(void *rec);
void list_remove_active(void *node);
u32 sprite_node_release(u32 p);
void sprite_update_cd0(void *sprite);
void list_push_active(void *obj);
u32 find_entry_code(struct SlotTable *tbl, u32 v);
u32 place_part_in_slot(s32 slot, s32 mode, s32 a2, s32 a3, s32 s0, s32 s4, s32 s8);
void menu_entry_refresh(void *entry, u32 idx, u32 arg);
struct PoolObj *obj_spawn(struct PoolObj *tmpl, u32 flags, s32 x, s32 y, s32 z, u32 vis, u32 extra);

void update_sprite_list(void)
{
    s32 visible;
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
            visible = 0x20;
            visible &= flags;
            if (visible)
            {
                sprite_entry_update((struct SpriteEnt *)obj);
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

extern u8 *gParty;
extern u8 gPartTemplates[];
extern u16 gPartSetupA[];
extern u16 gPartSetupB[];
extern u8 gLevelExpTable[];

void anim_copy_state(void *a, void *b);
void release_object(struct SpriteEntry *obj);
void task_list_remove(void *node);
void obj_setup_load(void *obj, u32 h);
u32 mt19937_next(void);
u32 rand_below(u32 n, u32 rnd);
void anim_chance_start(void *a, u32 b);
void anim_bit400_step(void *a, u32 b);

NONMATCH("asm/code_0807B7A0/place_part_in_slot.s", u32 place_part_in_slot(s32 slot, s32 mode, s32 a2, s32 a3, s32 s0, s32 s4, s32 s8))
{
    u8 *st;
    u8 *head;
    u8 *entry;
    u8 *entrySp;
    u16 *setup;
    u8 *obj;
    u8 *stateAddr;
    u32 flag6;
    s32 sum;
    s32 i;
    s32 row;
    u32 kind;
    u32 pick;
    u32 bit6;
    u16 setupH;
    void *hook;
    void *arg0;

    flag6 = s4;
    st = (u8 *)gWorld;
    head = (u8 *)gWorld->slots[slot];
    *(u32 *)(st + 0x8C) = *(u32 *)(head + 4);
    *(u16 *)(st + 0x17E) = *(u16 *)(head + 0xF6);
    *(u16 *)(st + 0x184) = *(u16 *)(head + 0xFC);
    *(u16 *)(st + 0x1AA) = (*(u16 *)(st + 0x1AA) & 0xFE7F) | (*(u16 *)(head + 0x122) & 0x180);
    anim_copy_state(head, st + 0x88);
    release_object((struct SpriteEntry *)head);
    task_list_remove(head);
    ((void (*)(u32, u8 *, u32, u8 *))gMemRoutines)(0, head, 0x130, gMemRoutines);
    *(u8 **)(head + 0x30) = head;
    *(u32 *)(head + 4) = *(u32 *)(st + 0x8C);
    head[0xF4] = (u8)slot;
    head[0x110] = (head[0x110] & 0xBF) | ((flag6 & 1) << 6);
    *(u16 *)(head + 0xEC) = (u16)mode;
    *(u32 *)(head + 0x18) = a2 << 8;
    *(u32 *)(head + 0x40) = a2 << 8;
    *(u32 *)(head + 0xD8) = a2 << 8;
    *(u32 *)(head + 0x1C) = a3 << 8;
    *(u32 *)(head + 0x44) = a3 << 8;
    *(u32 *)(head + 0xDC) = a3 << 8;
    *(u32 *)(head + 0x20) = s0 << 8;
    *(u32 *)(head + 0x48) = s0 << 8;
    *(u32 *)(head + 0xE0) = s0 << 8;
    *(u16 *)(head + 0x98) = 0x100;

    *(u32 *)(head + 0x08) = *(u32 *)(st + 0x78);
    stateAddr = head + 0x7E;

    if (mode < 0)
    {
        if (mode == -2)
        {
            entry = gParty + 0x3C;
            setup = gPartSetupA;
        }
        else if (mode == -1)
        {
            entry = gParty;
            setup = gPartSetupB;
        }
        obj = *(u8 **)(st + 0x8C);
        if (*(u16 *)(entry + 0xE) != 0)
        {
            *(u32 *)(head + 0x08) = *(u32 *)(st + 0x78);
            *(u32 *)(st + 0x78) = (u32)(head + 8);
            head[0x7E] = (head[0x7E] & 0xF9) | 2;
            obj_setup_load(obj, *setup);
            obj_load_record(obj, 0, -1, 0, 0);
            kind = obj[0x25];
            pick = rand_below(kind, mt19937_next());
            obj_load_record(obj, 0, pick, 0, 0);
            obj[0x1F] = (((((obj[0x1F] & 0xFC) | 2) & 0xF3) | 8) & 0xCF) | 0x20;
        }
        else
        {
            head[0x7E] = head[0x7E] | 6;
            obj[0x1F] = obj[0x1F] & 0xFC & 0xF3 & 0xCF;
            *(u16 *)(obj + 0xE) = 0;
        }
        /* shared part of the offset-entry branches */
        bit6 = head[0x110] & 0x40;
        obj[0x11] = (obj[0x11] & 0xBF) | bit6;
        *(u16 *)(obj + 6) = 0x100;
        *(u16 *)(obj + 4) = 0x100;
        *(u32 *)(head + 0xE8) = 0;
        head[0xF5] = entry[0x30];
        *(u16 *)(head + 0xF6) = *(u16 *)(entry + 0xE);
        *(u16 *)(head + 0xF8) = *(u16 *)(entry + 0x12);
        *(u16 *)(head + 0x106) = *(u16 *)(entry + 0x20);
        *(u16 *)(head + 0x102) = *(u16 *)(entry + 0x1C);
        *(u16 *)(head + 0x104) = *(u16 *)(entry + 0x24);
        *(u16 *)(head + 0x108) = *(u16 *)(entry + 0x28);
        *(u16 *)(head + 0xFC) = *(u16 *)(entry + 0x14);
        *(u16 *)(head + 0xFE) = *(u16 *)(entry + 0x18);
        head[0x80] = (head[0x80] & 0x1F) | 0x20;
        *(u16 **)(head + 0xE4) = setup;
        *(u8 **)(head + 0x10) = obj;
        head[0x12B] = entry[0x35] + 1;
        head[0x12C] = entry[0x37] + 1;
        head[0x12D] = entry[0x38] + 1;
        if (head[0x12C] == 0x19)
        {
            u8 *party = gParty;
            u32 acc;
            u32 val;
            s32 base;
            u32 neg;

            if (party[0x8A] == 0xFF)
            {
                acc = 0;
            }
            else
            {
                acc = party[0x8A] * 51;
            }
            if (party[0x8B] != 0xFF)
            {
                acc += party[0x8B] * 51;
            }
            if (party[0x8C] != 0xFF)
            {
                acc += party[0x8C] * 0x4C;
            }
            neg = ~(u32)(s32)*(s16 *)(head + 0xEC);
            base = *(u16 *)(party + neg * 60 + 0x28);
            if (party[0x8D] != 0xFF)
            {
                val = acc + party[0x8D] * 0x66;
            }
            else
            {
                val = acc;
            }
            if ((s32)val < 0)
            {
                val += 0xFF;
            }
            *(u16 *)(head + 0x108) = base + ((s32)val >> 8);
            if ((s16)*(u16 *)(head + 0x108) > 999)
            {
                *(u16 *)(head + 0x108) = 999;
            }
        }
        /* BB6C */
        if (head[0x12D] == 0x1D)
        {
            st[0xD] = st[0xD] | 1;
        }
    }
    else
    {
        /* template branch */
        u8 *node = head + 8;
        entry = gPartTemplates + mode * 0x2C;
        *(u32 *)(st + 0x78) = (u32)node;
        entrySp = *(u8 **)(entry + 8);
        head[0x7E] = (head[0x7E] & 0xF9) | 4;
        head[0x7D] = entrySp[0xA];
        head[0xF5] = entry[4];
        *(u16 *)(head + 0xF6) = *(u16 *)(entry + 6);
        *(u16 *)(head + 0xF8) = *(u16 *)(entry + 6);
        *(u16 *)(head + 0x106) = *(u16 *)(entry + 0x1A);
        *(u16 *)(head + 0xF6) = *(u16 *)(entry + 6);
        *(u16 *)(head + 0xF8) = *(u16 *)(entry + 6);
        *(u16 *)(head + 0x106) = *(u16 *)(entry + 0x1A);
        *(u16 *)(head + 0x104) = *(u16 *)(entry + 0x18);

        sum = 0;
        row = ((s32)head[0xF5] << 24) >> 24;
        for (i = row - 1; i >= 0; i--)
        {
            sum = (u16)(sum + gLevelExpTable[12 * i + 4] + gLevelExpTable[0x4A8 + 12 * i]);
        }
        {
            s32 t = (s16)sum;
            t = (t + (((u16)sum) >> 15)) >> 1;
            *(u16 *)(head + 0x108) = (u16)t;
        }
        if (entrySp[7] != 0)
        {
            head[0x80] = (head[0x80] & 0x1F) | ((entrySp[7] - 1) << 5);
        }
        else
        {
            head[0x81] = head[0x81] | 1;
        }
        *(u8 **)(head + 0xE4) = entrySp;
        *(u32 *)(head + 0xE8) = *(u32 *)(entry + 0x20);
        {
            u8 *dst = gWorkBuf + slot * 0x2C + 0x33E0;
            u32 *d = (u32 *)dst;
            u32 *s = (u32 *)(gPartTemplates + mode * 0x2C);
            *(u8 **)(head + 0xF0) = dst;
            d[0] = s[0];
            d[1] = s[1];
            d[2] = s[2];
            d[3] = s[3];
            d[4] = s[4];
            d[5] = s[5];
            d[6] = s[6];
            d[7] = s[7];
            d[8] = s[8];
            d[9] = s[9];
            d[10] = s[10];
        }
        setupH = *(u16 *)(*(u8 **)(head + 0xE4));
        if (setupH != 0)
        {
            obj = *(u8 **)(st + 0x8C);
            obj_setup_load(obj, setupH);
            obj_load_record(obj, 0, -1, 0, 0);
            kind = obj[0x25];
            pick = rand_below(kind, mt19937_next());
            obj_load_record(obj, 0, pick, 0, 0);
            obj[0x1F] = (((((obj[0x1F] & 0xFC) | 2) & 0xF3) | 8) & 0xCF) | 0x20;
            bit6 = head[0x110] & 0x40;
            obj[0x11] = (obj[0x11] & 0xBF) | bit6;
            *(u16 *)(obj + 6) = 0x100;
            *(u16 *)(obj + 4) = 0x100;
            *(u8 **)(head + 0x10) = obj;
        }
        if ((st[0xA] & 3) != 0)
        {
            hook = *(void **)(entry + 0x10);
            if (hook != NULL)
            {
                arg0 = head;
                do
                {
                    ((void (*)(void *, void *, u32))hook)(arg0, 0, 0);
                    hook = *(void **)(head + 0x54);
                    arg0 = node;
                } while (*(u32 *)(head + 0x54) != 0);
            }
        }
    }

    /* BE6E: common tail */
    *(u16 *)(head + 0x10A) = 0x100;
    *(u16 *)(head + 0x10C) = 0x100;
    *(u16 *)(head + 0x11C) = 0x100;
    entrySp = *(u8 **)(head + 0xE4);
    head[0xCB] = entrySp[4];
    head[0xCC] = entrySp[5];
    head[0xCD] = entrySp[6];
    if (s8 == 1)
    {
        *(u16 *)(head + 0xF6) = *(u16 *)(st + 0x17E);
        head[0x110] = (head[0x110] & 0x7F) | (st[0x198] & 0x80);
        *(u16 *)(head + 0xFC) = *(u16 *)(st + 0x184);
        *(u16 *)(head + 0x122) = (*(u16 *)(head + 0x122) & 0xFE7F) | (*(u16 *)(st + 0x1AA) & 0x180);
        anim_copy_state(st + 0x88, head);
    }
    /* BF0A */
    if ((stateAddr[0] & 6) == 2)
    {
        st[4] = st[4] + 1;
    }
    else if ((stateAddr[0] & 6) == 4)
    {
        st[5] = st[5] + 1;
    }
    set_state_word_1780(*(void **)(st + 0x244), st[5]);
    if (mode < 0)
    {
        u8 *a120;
        kind = head[0x12C];
        a120 = NULL;
        if (kind == 0x14)
        {
            anim_bit400_step(head, 0xC8);
            a120 = head + 0x120;
        }
        else if (kind == 0x13)
        {
            anim_chance_start(head, 0xC8);
            a120 = head + 0x120;
        }
        else if (kind == 0x15)
        {
            pick = rand_below(3, mt19937_next());
            if (pick == 0)
            {
                anim_chance_start(head, 0xC8);
                a120 = head + 0x120;
            }
            else if (pick == 1)
            {
                anim_bit400_step(head, 0xC8);
                a120 = head + 0x120;
            }
        }
        if (a120 != NULL)
        {
            *(u32 *)a120 = *(u32 *)a120 & 0xFFFC7FFF;
        }
    }
    return (u32)head;
}
END_NONMATCH

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

NONMATCH("asm/code_0807B7A0/refresh_slots_for_mask.s", void refresh_slots_for_mask(struct SpriteEnt *ent, u32 arg))
{
    u32 mask = ent->mask14;
    u32 bit = 0;
    
    if (mask != 0)
    {
        do
        {
            u32 low = mask & 1;
            u32 rest = mask >> 1;
            u32 nextBit = bit + 1;

            if (low)
            {
                
                
                s32 slot;
                u32 off;
                struct SlotTable *tbl;
                u32 idx;
                u32 next;

                for (slot = 0, off = 0; slot <= 7; slot += 2, off += 0x38)
                {
                    u8 *part;

                    tbl = gWorld->table248;
                    part = tbl->records + off;
                    if (((((u32)part[1]) << 28) >> 28) == bit && (part[0] & 3) != 0)
                    {
                        idx = slot;
                    }
                    else
                    {
                        next = slot + 1;
                        tbl = gWorld->table248;
                        part = tbl->records + off + 0x1C;
                        if (((((u32)part[1]) << 28) >> 28) != bit || (part[0] & 3) == 0)
                        {
                            continue;
                        }
                        idx = next;
                    }
                    menu_entry_refresh(tbl, (u8)idx, (u8)arg);
                    break;
                }
            }
            mask = rest;
            bit = nextBit;
        } while (mask != 0);
    }
}
END_NONMATCH

NONMATCH("asm/code_0807B7A0/sprite_entry_update.s", void sprite_entry_update(struct SpriteEnt *ent))
{
    if (ent == NULL)
    {
        return;
    }
    if (!(ent->flags12 & 0x20))
    {
        return;
    }
    list_push_active(ent);
    if ((*(u32 *)&ent->half1C & 0x00F0FFFF) != 0x600000)
    {
        u16 oldMask = ent->mask14;
        u32 link = ((u32)ent->flags13 << 30) >> 30;
        u32 shifted;
        u16 size;
        u16 key;
        u32 found;
        u16 newMask;
        u16 w12;
        struct SpriteEnt *node;
        struct SpriteNode *n;
        struct SpriteWorld *world;
        struct SlotTable *tbl;
        void (*cb)(void *);

        sprite_update_cd0(ent);
        if (link != 0)
        {
            ((struct SpriteFlags13 *)&ent->flags13)->lo2 = link;
        }
        newMask = ent->mask14;
        if (oldMask != newMask)
        {
            w12 = gWorld->unk12;
            gWorld->unk12 = w12 | newMask;
        }
        size = ent->half1C;
        shifted = (u32)ent->byte1E << 28;
        if (((size | ((shifted >> 16) + 0x1000)) - 0x2000) <= 1)
        {
            if (gWorld->slot220 == NULL)
            {
                gWorld->slot220 = ent;
            }
            else if (gWorld->slot220 != ent)
            {
                gWorld->slot224 = ent;
            }
        }
        else
        {
            found = find_entry_code(gWorld->table248, (u16)(((shifted >> 16) + 0x1000) | ent->unk16));
            if ((found << 16) == 0xFFFF0000)
            {
                return;
            }
            node = gModelCtx->list30;
            if (node != NULL)
            {
                do
                {
                    if (!(node->flags12 & 0x20) && node != ent && node->mask14 == ent->mask14
                        && ((struct SpriteFlags13 *)&node->flags13)->lo2 != 0)
                    {
                        return;
                    }
                    node = node->next68;
                } while (node != NULL);
            }
            cb = (void (*)(void *))alloc_slot_for_node;
            world = gWorld;
            n = world->freeNodes;
            world->freeNodes = n->next;
            n->next = world->list4C;
            n->callback = cb;
            n->timer = 0;
            world->list4C = n;
            n->entry = (struct SpriteEntry *)ent;
        }
    }
    else
    {
        if (ent->ptr54 == 0)
        {
            return;
        }
        ((struct SpriteFlags12 *)&ent->flags12)->bit5 = 0;
        ((struct SpriteFlags12 *)&ent->flags12)->mid = 0;
        ent->mask14 = 0x7800;
        ((struct SpriteFlags13 *)&ent->flags13)->lo2 = 0;
    }
}
END_NONMATCH

NONMATCH("asm/code_0807B7A0/obj_bind_slots.s", void obj_bind_slots(struct PoolRecord *rec))
{
    u16 slots;
    s32 first;
    struct SpriteEnt *obj;
    u32 idx;
    u32 partIdx;
    u32 partOff;

    if (rec == NULL)
    {
        return;
    }
    if (rec->flags12 & 0x20)
    {
        return;
    }
    slots = rec->mask14;
    if (slots == 0)
    {
        return;
    }
    first = 0;
    while (((slots ^ 1) & 1) != 0)
    {
        slots >>= 1;
        first++;
    }

    if (rec->flags29 & 0x10)
    {
        obj = gModelCtx->list30;
        if (obj != NULL)
        {
            do
            {
                if (!(obj->flags12 & 0x20) && obj != (struct SpriteEnt *)rec && ((obj->mask14 >> first) & 1))
                {
                    for (;;)
                    {
                    }
                }
                obj = obj->next68;
            } while (obj != NULL);
        }
        idx = first;
        if (slots != 0)
        {
            do
            {
                if (slots & 1)
                {
                    gModelCtx->slotTab[idx] |= 0xFFFF;
                }
                slots >>= 1;
                idx++;
            } while (slots != 0);
        }
        rec->mask14 = 0;
    }

    if ((*(u32 *)&rec->half1C & 0x00F0FFFF) == 0x600000)
    {
        rec->flags12 |= 0x20;
        return;
    }

    partOff = 0;
    for (partIdx = 0; partIdx <= 7; partIdx++)
    {
        u8 *part = gWorld->table248->records + partOff;

        if (((((u32)part[1]) << 28) >> 28) == first && (part[0] & 3) != 0)
        {
            for (obj = gModelCtx->list30; obj != NULL; obj = obj->next68)
            {
                if (!(obj->flags12 & 0x20) && obj != (struct SpriteEnt *)rec && ((obj->mask14 >> first) & 1))
                {
                    goto release;
                }
            }
            menu_entry_refresh(gWorld->table248, partIdx & 0xFF, 0);
            break;
        }
        partOff += 0x1C;
    }

release:
    obj_release_slots(rec);
}
END_NONMATCH

NONMATCH("asm/code_0807B7A0/obj_spawn.s", struct PoolObj *obj_spawn(struct PoolObj *tmpl, u32 flags, s32 x, s32 y, s32 z, u32 vis, u32 extra))
{
    struct PoolRecord *rec;
    struct PoolObj *obj;
    u16 group;
    u8 *flagsPtr;
    u32 v;
    s32 bit8;

    group = flags;
    obj = (struct PoolObj *)(gWorkBuf + 0x1000);
    if (obj->flags76 & 6)
    {
        do
        {
            obj = (struct PoolObj *)((u8 *)obj + 0xD0);
        } while (obj->flags76 & 6);
    }
    rec = obj->record;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, obj, 0xD0);
    obj->record = rec;
    obj->ring = tmpl;
    obj->next = gWorld->list78;
    gWorld->list78 = obj;
    if (group != 0)
    {
        obj_bind_slots(rec);
        ((struct RecFlags13 *)&rec->flags13)->b2 = 0;
        ((struct RecFlags13 *)&rec->flags13)->b3 = 0;
        setup_obj_10bc(rec, 0, group, 1, -1, -1, -1, -1);
        rec->flags13 |= 4;
        bit8 = 8;
        rec->flags13 |= bit8;
        obj_load_record(rec, 0, -1, 0, 0);
        list_remove_active(rec);
        obj_load_record(rec, 0, -1, 0, 0);
        sprite_entry_update((struct SpriteEnt *)rec);
        v = rec->flags1F;
        v = ((((((v & 0xFFFFFFFC) | 2) & 0xFFFFFFF3) | bit8) & 0xFFFFFFCF) | 0x20) & 0x3F;
        rec->flags1F = v | 0x80;
        ((struct RecFlags11 *)&rec->flags11)->b6 = vis & 1;
    }
    ((struct PoolFlags *)&obj->flags76)->mode = 2;
    obj->unk10 = x << 8;
    obj->posX = x << 8;
    obj->unk14 = y << 8;
    obj->posY = y << 8;
    obj->unk18 = z << 8;
    obj->posZ = z << 8;
    obj->unk90 = 0x100;
    flagsPtr = (u8 *)obj + 0xCC;
    if (tmpl != NULL)
    {
        *(u16 *)((u8 *)obj + 0xC8) = *(u16 *)((u8 *)tmpl + 0xD0);
        *flagsPtr = (*flagsPtr & 0x80) | (*((u8 *)tmpl + 0xD4) & 0x7F);
        *(u16 *)((u8 *)obj + 0xCA) = *(u16 *)((u8 *)tmpl + 0xD2);
    }
    *flagsPtr &= 0x7F;
    return obj;
}
END_NONMATCH

NONMATCH("asm/code_0807B7A0/obj_spawn_linked.s", struct PoolObj *obj_spawn_linked(struct PoolObj *src, u32 flags, struct PoolObj *base))
{
    struct PoolObj *pos = base;
    struct PoolObj *child;
    u16 group = flags;
    u32 busy;

    if (pos == NULL)
    {
        pos = src;
    }
    child = obj_spawn(src->ring, group, pos->posX / 0x100, pos->posY / 0x100, pos->posZ / 0x100,
        ((u32)src->record->flags11 << 25) >> 31, 1);
    child->sibling = src->sibling;
    src->sibling = child;
    busy = src->flags76 & 6;
    child->flags76 = (child->flags76 & -7) | busy;
    ((struct PoolBits77 *)child)->bit6 = ((struct PoolBits77 *)src)->bit6;
    ((struct PoolBits77 *)child)->bit5 = ((struct PoolBits77 *)src)->bit5;
    child->unk6C = src->unk6C;
    child->unk2C = src->unk2C;
    child->unk4C = 0;
    child->link68 = 0;
    child->unk7C = src->unk7C;
    child->unk9C = src->unk9C;
    child->unkA0 = src->unkA0;
    child->unkA4 = src->unkA4;
    child->unkA8 = src->unkA8;
    return child;
}
END_NONMATCH

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

NONMATCH("asm/code_0807B7A0/obj_release.s", void obj_release(struct PoolObj *obj))
{
    if (obj != NULL)
    {
        u8 *flags;
        u32 link;

        ((struct StateBits *)&obj->flags76)->kind = 0;
        obj_list_remove(obj);
        link = obj->link68;
        flags = &obj->flags76;
        if (link != 0 && link != (u32)obj_release)
        {
            ((void (*)(void *))link)(obj);
        }
        if (obj->ring != NULL)
        {
            struct PoolObj *ringHead = (struct PoolObj *)((u8 *)obj->ring + 8);

            if (obj == ringHead)
            {
                struct PoolObj *child = obj->sibling;

                if (child != NULL)
                {
                    do
                    {
                        struct PoolObj *next = child->sibling;

                        if (child->flags79 & 8)
                        {
                            obj_release(child);
                        }
                        child = next;
                    } while (child != NULL);
                }
            }
            else
            {
                struct PoolObj *prev = ringHead;

                while (prev->sibling != NULL)
                {
                    if (obj == prev->sibling)
                    {
                        prev->sibling = obj->sibling;
                        obj->sibling = NULL;
                        break;
                    }
                    prev = prev->sibling;
                }
            }
        }
        {
            u32 node = (u32)obj->buffer;

            if (node != 0)
            {
                do
                {
                    node = sprite_node_release(node);
                } while (node != 0);
            }
        }
        if (obj->unk4 != NULL)
        {
            ((struct PoolObj *)obj->unk4)->unk4 = NULL;
            obj->unk4 = NULL;
        }
        {
            struct PoolRecord *rec = obj->record;

            rec->unk20 = 0x10;
            rec->unk4 = 0x100;
            rec->unk6 = 0x100;
            rec->unk8 = 0x100;
            rec->unkA = 0x100;
            rec->unkC = 0;
            ((struct RecFlags11 *)&rec->flags11)->mid2 = 0;
            ((struct RecFlags11 *)&rec->flags11)->b4 = 0;
            ((struct RecFlags11 *)&rec->flags11)->b5 = 0;
            ((struct RecFlags11 *)&rec->flags11)->b6 = 0;
            ((struct RecFlags11 *)&rec->flags11)->b7 = 0;
            ((struct RecFlags12 *)&rec->flags12)->b4 = 0;
            ((struct RecFlags29 *)&rec->flags29)->b1 = 0;
            rec->unk5C = 0;
            rec->unk60 = 0;
            obj_bind_slots(rec);
            list_remove_active(rec);
        }
        {
            struct PoolObj *prev = NULL;
            struct PoolObj *cur = gWorld->list78;

            while (cur != NULL)
            {
                if (cur == obj)
                {
                    if (prev == NULL)
                    {
                        gWorld->list78 = obj->next;
                    }
                    else
                    {
                        prev->next = obj->next;
                    }
                    break;
                }
                prev = cur;
                cur = cur->next;
            }
        }
        {
            u32 pos = (u32)obj - 0x1000;

            if (pos - (u32)gWorkBuf <= 0xCFF)
            {
                ((struct PoolFlags *)flags)->mode = 0;
            }
        }
        obj->unk54 = 0;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0807B7A0/draw_sprite_nodes.s", void draw_sprite_nodes(void))

ASM_FUNC("asm/code_0807B7A0/oam_place_points.s", void oam_place_points(u32 *cursor, u32 count, s32 x, s32 y, u32 extra, u32 attr0Or, u32 mirror))

struct PartChild
{
    struct PartChild *next;
    s32 x;
    s32 y;
    s32 z;
};

struct PartKids
{
    u8 filler0[8];
    struct PartChild *parts;
    u8 flags;
};

extern u8 gSys[];
extern u8 *gMathRoutines;
extern u8 iwram_div[];
extern u8 iwram_sqrt[];

NONMATCH("asm/code_0807B7A0/emit_active_parts_oam.s", u32 emit_active_parts_oam(u32 oam))
{
    struct SpriteWorld **wp = &gWorld;
    u32 out = oam;
    u32 level;
    struct PoolObj *obj;
    struct PoolRecord *rec;
    struct PartKids *kids;
    struct PartChild *child;
    u32 frame;
    u16 *ent;
    s32 cpos[3];
    s32 pos[3];
    s32 scale;
    s32 routine;
    s32 left;
    s32 n;
    s32 cx;
    s32 cy;
    u32 busy;
    u32 arg3;
    s32 depth;
    s32 c4;
    s32 c6;
    s32 d4;
    s32 d6;

    if (((*wp)->unkA & 3) == 0)
    {
        return out;
    }
    level = ((u32)(*wp)->oamRow << 27) >> 27;
    for (obj = (*wp)->list78; obj != NULL; obj = obj->next)
    {
        rec = obj->record;
        if (rec == NULL)
        {
            continue;
        }
        if (rec->flags12 & 0x20)
        {
            continue;
        }
        busy = obj->flags79;
        if (busy & 1)
        {
            continue;
        }
        kids = (struct PartKids *)obj->unk4;
        if (kids != NULL)
        {
            if (kids->flags & 0x20)
            {
                continue;
            }
            child = kids->parts;
            if (child == NULL)
            {
                continue;
            }
            for (; child != NULL; child = child->next)
            {
                if (child->z < 0)
                {
                    continue;
                }
                cpos[0] = child->x;
                if (cpos[0] < 0)
                {
                    cpos[0] += 0xFF;
                }
                cpos[0] >>= 8;
                cpos[1] = child->y;
                if (cpos[1] < 0)
                {
                    cpos[1] += 0xFF;
                }
                cpos[1] >>= 8;
                cpos[2] = 0;
                offset_three_words((*wp)->camera, &cpos[0], &cpos[1], &cpos[2], 0);
                cx = cpos[0];
                cy = cpos[1];
                if (cx < -64 || cy < -64 || cx > 304 || cy > 224)
                {
                    continue;
                }
                frame = (*wp)->frames[obj->frame78 >> 5];
                ent = (u16 *)(frame + 2);
                n = *(u16 *)frame - 1;
                if (n == -1)
                {
                    continue;
                }
                if ((~n & 1) != 0)
                {
                    *(u16 *)(out + 0) = ((ent[0] + cy) & 0xFF) | ent[1];
                    *(u16 *)(out + 2) = ((ent[2] + cx) & 0x1FF) | ent[3];
                    *(u16 *)(out + 4) = ent[4];
                    out += 8;
                    ent += 5;
                    n -= 1;
                    if (n == -1)
                    {
                        continue;
                    }
                }
                do
                {
                    *(u16 *)(out + 0) = ((ent[0] + cy) & 0xFF) | ent[1];
                    *(u16 *)(out + 2) = ((ent[2] + cx) & 0x1FF) | ent[3];
                    *(u16 *)(out + 4) = ent[4];
                    *(u16 *)(out + 8) = ((ent[5] + cy) & 0xFF) | ent[6];
                    *(u16 *)(out + 0xA) = ((ent[7] + cx) & 0x1FF) | ent[8];
                    *(u16 *)(out + 0xC) = ent[9];
                    out += 0x10;
                    ent += 10;
                    n -= 2;
                } while (n != -1);
            }
            continue;
        }

        if (obj->posZ < 0)
        {
            continue;
        }
        pos[0] = obj->posX;
        if (pos[0] < 0)
        {
            pos[0] += 0xFF;
        }
        pos[0] = (pos[0] >> 8) + obj->biasX;
        pos[1] = obj->posY;
        if (pos[1] < 0)
        {
            pos[1] += 0xFF;
        }
        pos[1] >>= 8;
        pos[2] = 0;
        offset_three_words((*wp)->camera, &pos[0], &pos[1], &pos[2], 0);
        cx = pos[0];
        cy = pos[1];
        if (cx < -64 || cy < -64 || cx > 304 || cy > 224)
        {
            continue;
        }
        frame = (*wp)->frames[obj->frame78 >> 5];
        n = *(u16 *)frame;
        ent = (u16 *)(frame + 2);
        arg3 = 0;
        if (obj->posZ != 0)
        {
            arg3 = busy & 2;
            if (arg3 == 0)
            {
                depth = obj->posZ;
                if (depth < 0)
                {
                    depth += 0x7F;
                }
                scale = (depth >> 7) + 0x100;
                if (scale > 0x200)
                {
                    scale = 0x200;
                }
                {
                    u8 *dst = gSys + 0x80 + (level << 5) + ((gSys[0x888] & 1) << 10);

                    *(u16 *)(dst + 6) = scale;
                    *(u16 *)(dst + 0xE) = arg3;
                    *(u16 *)(dst + 0x16) = arg3;
                    *(u16 *)(dst + 0x1E) = scale;
                }
                if (n == 1)
                {
                    *(u16 *)(out + 0) = ((ent[0] + cy) & 0xFF) | ent[1] | 0x100;
                    *(u16 *)(out + 2) = ((ent[2] + cx) & 0x1FF) | ent[3] | (level << 9);
                    *(u16 *)(out + 4) = ent[4];
                    out += 8;
                    level -= 1;
                    continue;
                }
                n -= 1;
                left = level - 1;
                if (n == -1)
                {
                    level = left;
                    continue;
                }
                routine = (u32)iwram_div - (u32)iwram_sqrt;
                if ((~n & 1) != 0)
                {
                    c4 = (s16)ent[0];
                    c6 = (s16)ent[2];
                    if (c4 != 0)
                    {
                        c4 = ((s32 (*)(s32, s32))(gMathRoutines + routine))(c4 << 8, scale);
                    }
                    if (c6 != 0)
                    {
                        c6 = ((s32 (*)(s32, s32))(gMathRoutines + routine))(c6 << 8, scale);
                    }
                    *(u16 *)(out + 0) = ((cy + c4) & 0xFF) | ent[1] | 0x100;
                    *(u16 *)(out + 2) = ((cx + c6) & 0x1FF) | ent[3] | (level << 9);
                    *(u16 *)(out + 4) = ent[4];
                    out += 8;
                    ent += 5;
                    n -= 1;
                    if (n == -1)
                    {
                        level = left;
                        continue;
                    }
                }
                do
                {
                    c4 = (s16)ent[0];
                    c6 = (s16)ent[2];
                    if (c4 != 0)
                    {
                        c4 = ((s32 (*)(s32, s32))(gMathRoutines + routine))(c4 << 8, scale);
                    }
                    if (c6 != 0)
                    {
                        c6 = ((s32 (*)(s32, s32))(gMathRoutines + routine))(c6 << 8, scale);
                    }
                    *(u16 *)(out + 0) = ((cy + c4) & 0xFF) | ent[1] | 0x100;
                    *(u16 *)(out + 2) = ((cx + c6) & 0x1FF) | ent[3] | (level << 9);
                    *(u16 *)(out + 4) = ent[4];
                    d4 = (s16)ent[5];
                    d6 = (s16)ent[7];
                    if (d4 != 0)
                    {
                        d4 = ((s32 (*)(s32, s32))(gMathRoutines + routine))(d4 << 8, scale);
                    }
                    if (d6 != 0)
                    {
                        d6 = ((s32 (*)(s32, s32))(gMathRoutines + routine))(d6 << 8, scale);
                    }
                    *(u16 *)(out + 8) = ((cy + d4) & 0xFF) | ent[6] | 0x100;
                    *(u16 *)(out + 0xA) = ((cx + d6) & 0x1FF) | ent[8] | (level << 9);
                    *(u16 *)(out + 0xC) = ent[9];
                    out += 0x10;
                    ent += 10;
                    n -= 2;
                } while (n != -1);
                level = left;
                continue;
            }
        }
        n -= 1;
        if (n == -1)
        {
            continue;
        }
        if ((~n & 1) != 0)
        {
            *(u16 *)(out + 0) = ((ent[0] + cy) & 0xFF) | ent[1];
            *(u16 *)(out + 2) = ((ent[2] + cx) & 0x1FF) | ent[3];
            *(u16 *)(out + 4) = ent[4];
            out += 8;
            ent += 5;
            n -= 1;
            if (n == -1)
            {
                continue;
            }
        }
        do
        {
            *(u16 *)(out + 0) = ((ent[0] + cy) & 0xFF) | ent[1];
            *(u16 *)(out + 2) = ((ent[2] + cx) & 0x1FF) | ent[3];
            *(u16 *)(out + 4) = ent[4];
            *(u16 *)(out + 8) = ((ent[5] + cy) & 0xFF) | ent[6];
            *(u16 *)(out + 0xA) = ((ent[7] + cx) & 0x1FF) | ent[8];
            *(u16 *)(out + 0xC) = ent[9];
            out += 0x10;
            ent += 10;
            n -= 2;
        } while (n != -1);
    }
    (*wp)->oamRow = ((*wp)->oamRow & 0xE0) | level;
    return out;
}
END_NONMATCH
