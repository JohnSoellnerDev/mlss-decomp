#include "global.h"

struct Obj;

struct SubObj
{
    u8 filler0[0x368];
    u8 *table;
};

struct Obj
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 depth;
    s32 depthDelta;
    u8 *unk1C;
    u8 filler20[0x28 - 0x20];
    struct Obj *entries[2];
    u8 filler30[0x40 - 0x30];
    s32 deltaDir;
    u8 filler44[0xCC - 0x44];
    u8 *resList;
    u8 *record;
    u8 filler0D4[0xFC - 0xD4];
    u8 unkFC;
    u8 filler0FD[0x10A - 0xFD];
    u8 unk10A;
    u8 entryTotal;
    u8 filler10C;
    u8 resCount;
    u8 cntA;
    u8 filler10F;
    u8 cntB;
    u8 filler111;
    u8 cntC;
    u8 filler113;
    u8 cntD;
    u8 filler115;
    u8 cntE;
    u8 filler117[0x11A - 0x117];
    u8 cntF;
    u8 filler11B[0x12A - 0x11B];
    u8 testLevel;
    u8 filler12B[0x134 - 0x12B];
    u8 entryBytes[0x1F4 - 0x134];
    u8 entryCount;
    u8 filler1F5;
    u16 seqTimer;
    s8 seqIndex;
    u8 filler1F9[0x200 - 0x1F9];
    struct SubObj *sub200;
    struct SubObj *sub204;
    u8 filler208[0x246 - 0x208];
    u8 unk246;
    u8 filler247[0x270 - 0x247];
    s16 boxYOfs;
    u16 boxXSize;
    u16 boxDepthSize;
    u16 boxYSize;
    u8 filler278[0x31C - 0x278];
    u32 unk31C;
};

struct SysRam
{
    u8 filler0[0x28];
    u16 keys28;
    u16 pressed;
    u8 filler2C[0x8C0 - 0x2C];
    u8 lang8C0;
};

struct Desc
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 tag;
    s32 unk18;
    u8 filler1C[0x54 - 0x1C];
    u8 unk54;
};

struct HitEntry
{
    u8 flags;
    u8 gx;
    u8 gy;
    u8 tag;
    u8 filler4[8];
};

extern struct SysRam gSys;
extern const u16 gInputSequenceKeys[];
u8 *bank_data_5bit(u16 bank, u8 *out);
void find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, u32 *out, s32 slot);

typedef void (*EntCb)(void *view, void *sel, u32 a, u32 b);

struct EntDesc
{
    s16 viewOff;
    u16 filler2;
    EntCb cb;
};

struct EntClass
{
    u8 filler0[0x60];
    struct EntDesc desc;
};

struct EntRecord
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x21 - 0x13];
    u8 kind21;
};

struct EntEntity
{
    u8 filler0[0x214];
    u8 flags214;
    u8 filler215[0x294 - 0x215];
    struct EntRecord *record;
    u8 filler298[0x338 - 0x298];
    struct EntClass *info;
};

struct EntObj
{
    u8 filler0[0x28];
    struct EntEntity *entries[0x2B];
    u8 filler0D4[0x10B - 0xD4];
    u8 entryCount;
    u8 filler10C[0x10E - 0x10C];
    u8 listPStart;
    u8 listPCount;
    u8 listAStart;
    u8 listACount;
    u8 listCStart;
    u8 listCCount;
    u8 listB1Start;
    u8 listB1Count;
    u8 listB2Start;
    u8 listB2Count;
    u8 filler118[0x208 - 0x118];
    u8 flags208;
    u8 flags209;
    u8 filler20A[0x246 - 0x20A];
    u8 state246;
};

void obj_set_sprite_link(void *ent, u32 zero);
void obj_load_record(void *rec, u32 a, s32 b, u32 c, u32 d);
void obj_update_flags_from_child(void *ent);

struct Bits8
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
} __attribute__((packed));

struct Word22C
{
    u32 lo : 10;
    u32 id : 12;
    u32 hi : 10;
};

struct SceneItem
{
    u8 filler0[0x54];
    u8 index54;
    u8 filler55[0x20C - 0x55];
    struct Bits8 bits20C;
    u8 filler20D[0x20F - 0x20D];
    struct Bits8 bits20F;
    struct Bits8 bits210;
    u8 filler211[0x22C - 0x211];
    struct Word22C bits22C;
    u8 filler230[0x239 - 0x230];
    u8 mode239;
    u8 filler23A[0x344 - 0x23A];
    struct SceneItem *child;
    u8 filler348[0x352 - 0x348];
    struct Bits8 bits352;
    u8 filler353[0x368 - 0x353];
    u32 ptr368;
};

struct SceneSlot
{
    u8 data[0x18];
};

struct SceneObj
{
    u8 filler0[0x28];
    struct SceneItem *entries[0x20];
    u32 slotsA8[8];
    u8 filler0C8[0xFB - 0xC8];
    u8 flagsFB;
    u8 filler0FC[0x10B - 0xFC];
    u8 entryCount;
    u8 filler10C[0x10E - 0x10C];
    u8 first1;
    u8 count1;
    u8 first2;
    u8 count2;
    u8 first3;
    u8 count3;
    u8 first4;
    u8 count4;
    u8 first5;
    u8 count5;
    u8 first6;
    u8 count6;
    u8 first7;
    u8 count7;
    u8 hint11C;
    u8 sum11D;
    u8 filler11E[0x208 - 0x11E];
    u8 flags208;
    u8 filler209[0x20C - 0x209];
    struct SceneSlot slots20C[0x20];
};

extern u8 *gUnk_03000FC0;
extern const u8 gHeapTagItemKind0[];
extern const u8 gHeapTagItemKind1[];
extern const u8 gHeapTagItemKind2[];
extern const u8 gSceneObjKind3Tag[];
extern const u8 gSceneObjKind4Tag[];
extern const u8 gSceneObjKind5Tag[];
extern const u8 gSceneObjKind6Tag[];
extern const u8 gSceneObjKind7Tag[];
extern const u8 gSceneObjItemTag[];

void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
u32 object_def_mode(u16 bank, u8 index);
u8 bank_byte1_high(u16 bank, u8 *out);
void *obj_init_defaults(void *rec, u32 def, u32 i);
void *obj_init(void *rec, u32 def, u32 i);
void *obj_init_with_child(void *rec, u32 def, u32 i);
void *obj_init_flags(void *rec, u32 def, u32 i);
void *obj_init_state(void *rec, u32 def, u32 i);
void obj_set_motion_params(void *item, u32 a, u32 b, u32 c, u32 d);
u8 test_flag(u8 *table, u8 *tableB, u16 id);
u32 set_low3_bits(void *h, u32 idx);

NONMATCH("asm/code_08025650/obj_update_entities.s", void obj_update_entities(struct EntObj *obj))
{
    struct EntEntity **slot;
    u32 remaining;
    u8 *stateAddr;
    u32 stateMask;
    u32 bit;
    u32 f208;
    struct EntEntity **aSlot;
    struct EntEntity **cSlot;
    struct EntEntity **bSlot;
    s32 first;

    if (obj->listPStart != 0)
    {
        slot = &obj->entries[obj->listPStart];
        remaining = obj->listPCount;
        if (remaining != 0)
        {
            stateAddr = &obj->state246;
            stateMask = 0x1C;
            bit = 1;
            do
            {
                if ((obj->flags209 & bit) == 0)
                {
                    if ((*stateAddr & stateMask) != 0xC)
                    {
                        struct EntEntity *e = *slot;
                        struct EntDesc *d = &e->info->desc;
                        f208 = obj->flags208;
                        d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + (f208 & 4), 1, 0);
                    }
                    f208 = obj->flags208;
                    if ((f208 & 0x10) == 0 || (*stateAddr & stateMask) == 0xC)
                    {
                        struct EntEntity *e = *slot;
                        struct EntDesc *d = &e->info->desc;
                        d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + ((((f208 >> 2) & 1) ^ bit) << 2), 1, 0);
                    }
                }
                {
                    struct EntEntity *e = *slot++;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj + 0x30, obj->entryCount - 2, 0);
                }
                remaining--;
            } while (remaining != 0);
        }
    }

    if (obj->listAStart != 0)
    {
        aSlot = &obj->entries[obj->listAStart];
        remaining = obj->listACount;
        if (remaining != 0)
        {
            s32 more = 1;
            if ((remaining & 1) != 0)
            {
                u32 sel;
                u32 rest;
                if ((obj->state246 & 0x1C) != 0xC)
                {
                    sel = 0;
                    rest = obj->entryCount;
                }
                else
                {
                    struct EntEntity *e = *aSlot;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)(obj->entries), 1, 0);
                    sel = 2;
                    rest = obj->entryCount - 2;
                }
                {
                    struct EntEntity *e = *aSlot++;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + sel * 4, rest, 0);
                }
                remaining--;
                more = remaining != 0;
            }
            while (more)
            {
                u32 sel;
                u32 rest;
                u32 sel2;
                u32 rest2;
                struct EntEntity **second;
                if ((obj->state246 & 0x1C) != 0xC)
                {
                    sel = 0;
                    rest = obj->entryCount;
                }
                else
                {
                    struct EntEntity *e = *aSlot;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)(obj->entries), 1, 0);
                    sel = 2;
                    rest = obj->entryCount - 2;
                }
                second = aSlot + 1;
                {
                    struct EntEntity *e = *aSlot;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + sel * 4, rest, 0);
                }
                if ((obj->state246 & 0x1C) != 0xC)
                {
                    sel2 = 0;
                    rest2 = obj->entryCount;
                }
                else
                {
                    struct EntEntity *e = *second;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)(obj->entries), 1, 0);
                    sel2 = 2;
                    rest2 = obj->entryCount - 2;
                }
                {
                    struct EntEntity *e = *second;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + sel2 * 4, rest2, 0);
                }
                remaining -= 2;
                aSlot += 2;
                more = remaining != 0;
            }
        }
    }

    if (obj->listCStart != 0)
    {
        cSlot = &obj->entries[obj->listCStart];
        remaining = obj->listCCount;
        if (remaining != 0)
        {
            s32 more = 1;
            if ((remaining & 1) != 0)
            {
                u32 sel;
                u32 rest;
                if ((obj->state246 & 0x1C) != 0x10)
                {
                    sel = 2;
                    rest = obj->entryCount - 2;
                }
                else
                {
                    sel = 1;
                    rest = obj->entryCount - 1;
                }
                {
                    struct EntEntity *e = *cSlot;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + sel * 4, rest, 0);
                }
                obj_update_flags_from_child(*cSlot++);
                remaining--;
                more = remaining != 0;
            }
            while (more)
            {
                u32 sel;
                u32 rest;
                u32 sel2;
                u32 rest2;
                struct EntEntity **second;
                if ((obj->state246 & 0x1C) != 0x10)
                {
                    sel = 2;
                    rest = obj->entryCount - 2;
                }
                else
                {
                    sel = 1;
                    rest = obj->entryCount - 1;
                }
                {
                    struct EntEntity *e = *cSlot;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + sel * 4, rest, 0);
                }
                obj_update_flags_from_child(*cSlot);
                second = cSlot + 1;
                if ((obj->state246 & 0x1C) != 0x10)
                {
                    sel2 = 2;
                    rest2 = obj->entryCount - 2;
                }
                else
                {
                    sel2 = 1;
                    rest2 = obj->entryCount - 1;
                }
                {
                    struct EntEntity *e = *second;
                    struct EntDesc *d = &e->info->desc;
                    d->cb((u8 *)e + d->viewOff, (u8 *)obj->entries + sel2 * 4, rest2, 0);
                }
                obj_update_flags_from_child(*second);
                remaining -= 2;
                cSlot += 2;
                more = remaining != 0;
            }
        }
    }

    first = obj->listB1Start;
    if (first == 0)
        first = obj->listB2Start;
    if (first > 0)
    {
        bSlot = &obj->entries[first];
        remaining = obj->listB1Count + obj->listB2Count;
        if (remaining != 0)
        {
            do
            {
                struct EntEntity *e = *bSlot;
                if (e->record->kind21 == 1 && (e->record->flags12 & 8) != 0)
                {
                    obj_set_sprite_link(e, 0);
                    (*bSlot)->flags214 &= -8;
                    obj_load_record((*bSlot)->record, 0, 0, 0, 0);
                    (*bSlot)->record->flags12 &= -7;
                    (*bSlot)->record->flags12 &= -9;
                }
                bSlot++;
                remaining--;
            } while (remaining != 0);
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_08025650/process_obj_entries.s", void process_obj_entries(void))

NONMATCH("asm/code_08025650/obj_collision_mask.s", u32 obj_collision_mask(struct Obj *owner, u32 idx))
{
    struct Obj *subject;
    struct Obj *other;
    u8 mask;
    s32 k;
    s32 leftS;
    s32 yS;
    s32 depthS;
    s32 leftO;
    s32 yO;
    s32 depthO;
    s32 lo;
    s32 hi;
    u32 fullO;

    subject = owner->entries[idx];
    mask = 0;
    leftS = subject->posX - (subject->boxXSize >> 1);
    yS = subject->posY + subject->boxYOfs;
    depthS = subject->depth;
    if (subject->depthDelta > 0 || (subject->depthDelta < 0 && subject->deltaDir < 0))
        depthS += subject->depthDelta;
    for (k = 0; k < 2; k++)
    {
        other = owner->entries[k];
        fullO = other->boxXSize;
        leftO = other->posX - (fullO >> 1);
        if (leftO < leftS)
        {
            hi = leftO + fullO;
            lo = leftS;
        }
        else
        {
            hi = subject->boxXSize + leftS;
            lo = leftO;
        }
        if (hi - lo <= 0)
            continue;
        yO = other->posY + other->boxYOfs;
        if (yO > yS)
            hi = yS - (yO - other->boxYSize);
        else
            hi = yO - (yS - subject->boxYSize);
        if (hi <= 0)
            continue;
        depthO = other->depth;
        if (other->depthDelta > 0 || (other->depthDelta < 0 && other->deltaDir < 0))
            depthO += other->depthDelta;
        if (depthO <= depthS)
            hi = depthO + other->boxDepthSize - depthS;
        else
            hi = depthS + subject->boxDepthSize - depthO;
        if (hi < 0)
            continue;
        mask |= 1 << k;
    }
    return mask;
}
END_NONMATCH

struct TestRec
{
    u16 h0;
    u16 h2;
};

struct TestTab
{
    u8 filler0[4];
    u16 h4;
    u16 h6;
};

extern u8 gUnk_02000100[];

u32 obj_test_mode(struct Obj *obj, u32 mode)
{
    if (obj->testLevel <= 1)
        return 0;
    if (mode > 7)
        return 0;
    switch (mode)
    {
    case 0:
    {
        struct TestRec *rec = (struct TestRec *)obj->record;
        if ((rec->h0 & 0x300) == 0)
        {
            struct TestTab *tab = (struct TestTab *)obj->sub200->table;
            if (rec->h2 & tab->h4)
                return 1;
        }
        break;
    }
    case 1:
    {
        struct TestRec *rec = (struct TestRec *)obj->record;
        if ((rec->h0 & 0x300) == 0)
        {
            struct TestTab *tab = (struct TestTab *)obj->sub204->table;
            if (rec->h2 & tab->h4)
                return 1;
        }
        break;
    }
    case 2:
    {
        struct TestRec *rec = (struct TestRec *)obj->record;
        struct TestTab *tab = (struct TestTab *)obj->sub204->table;
        if (rec->h0 & tab->h6)
            break;
        tab = (struct TestTab *)obj->sub200->table;
        if (rec->h2 & tab->h6)
            return 1;
        break;
    }
    case 3:
    {
        struct TestRec *rec = (struct TestRec *)obj->record;
        struct TestTab *tab = (struct TestTab *)obj->sub200->table;
        if (rec->h0 & tab->h6)
            break;
        tab = (struct TestTab *)obj->sub204->table;
        if (rec->h2 & tab->h6)
            return 1;
        break;
    }
    case 4:
        if (((struct TestRec *)obj->record)->h2 & 8)
            return 1;
        break;
    case 5:
        if ((*(u32 *)obj->record & 0xFFFF0300) == 0x40000)
            return 1;
        break;
    case 6:
        if ((*(u32 *)obj->record & 0x01000200) == 0x01000200)
            return 1;
        if ((*(u32 *)obj->record & (u32)gUnk_02000100) == (u32)gUnk_02000100)
            return 1;
        break;
    case 7:
        if ((gSys.keys28 & 0x300) == 0x300)
            return 1;
        break;
    }
    return 0;
}

u32 obj_sequence_step(struct Obj *obj)
{
    if (gSys.pressed != 0)
    {
        if (gSys.pressed == gInputSequenceKeys[obj->seqIndex])
        {
            obj->seqIndex++;
            if ((u8)obj->seqIndex == 10)
            {
                obj->seqIndex = 0;
                obj->seqTimer = 0xFFFF;
                return 1;
            }
            obj->seqTimer = 0x1E;
        }
        else
        {
            obj->seqIndex = 0;
            obj->seqTimer = 0xFFFF;
        }
    }
    obj->seqTimer--;
    if ((s16)obj->seqTimer <= 0)
    {
        obj->seqIndex = 0;
        obj->seqTimer = 0xFFFF;
    }
    return 0;
}

ASM_FUNC("asm/code_08025650/obj_fill_sprite_entry.s", void obj_fill_sprite_entry(void))

NONMATCH("asm/code_08025650/obj_find_hit_entry.s", u32 obj_find_hit_entry(struct Obj *obj, struct Desc *desc))
{
    struct HitEntry *e;
    s32 i;
    s32 t;
    u16 px;
    u16 py;
    s16 tag;
    s32 x1;
    s32 y1;
    s32 xa;
    s32 xb;
    s32 ya;
    s32 x0;
    s32 y0;

    if (obj->entryCount != 0)
    {
        e = (struct HitEntry *)obj->entryBytes;
        t = desc->posX;
        if (t < 0)
            t += 0xFF;
        px = t >> 8;
        t = desc->posY;
        if (t < 0)
            t += 0xFF;
        py = t >> 8;
        t = desc->tag;
        if (t < 0)
            t += 0x7FF;
        tag = t >> 11;
        for (i = 0; i < obj->entryCount; e++, i++)
        {
            if (e->flags & 2)
                continue;
            if (desc->unk18 != 0)
                continue;
            if (tag != e->tag)
                continue;
            if (desc->unk54 != 1 && !(e->flags & 1))
                continue;
            x0 = e->gx << 4;
            x1 = x0 + 0xF;
            xa = x0 + 4;
            xb = x1 - 4;
            y0 = e->gy << 4;
            y1 = y0 + 0xF;
            ya = y0 + 7;
            if ((s16)px >= xa && (s16)px <= xb && (s16)py >= ya && (s16)py <= y1)
                return i;
            xa -= 8;
            xb += 8;
            ya -= 8;
            y1 += 8;
            if ((s16)px >= xa && (s16)px <= xb && (s16)py >= ya && (s16)py <= y1)
                return i | 0x80;
        }
    }
    return -1;
}
END_NONMATCH

ASM_FUNC("asm/code_08025650/obj_draw_desc_entries.s", void obj_draw_desc_entries(void))

extern u8 gItemTable[];
extern u8 gStringTable12[];
extern u8 *gPrefixPtr;
void refresh_bg_layer(u32 arg);

struct FcBits
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

void obj_build_message(struct Obj *obj, u32 unused, u32 kind, u32 index, u32 flag)
{
    u8 *text = 0;
    u8 *entry;
    u32 offset;
    u32 table;
    u8 *dst;
    u8 *src;
    u8 ch;

    if ((u32)(kind - 4) <= 7)
    {
        offset = index << 4;
        table = (u32)gItemTable;
        table += 0xC;
    }
    else
    {
        if ((s32)kind <= 11)
            goto check;
        offset = index << 1;
        offset += index;
        offset <<= 2;
        table = (u32)gStringTable12;
    }
    entry = (u8 *)(offset + table);
    {
        u32 lang = gSys.lang8C0;
        u32 *langs = *(u32 **)entry;
        text = *(u8 **)langs[lang];
    }
check:
    if (text != 0)
    {
        refresh_bg_layer(obj->unk31C);
        dst = obj->unk1C + 0x27C;
        src = gPrefixPtr;
        ch = *src;
        if (ch != 0)
        {
            do
            {
                *dst = ch;
                src++;
                dst++;
                ch = *src;
            } while (ch != 0);
        }
        while (*text != 0)
        {
            text++;
        }
        text++;
        while (*text != 0)
        {
            *dst++ = *text++;
        }
        *dst++ = 0xFF;
        *dst++ = 0xC;
        *dst++ = 0x1E;
        *dst++ = 0xFF;
        *dst = 0xA;
        dst[1] = 0;
        ((struct FcBits *)&obj->unkFC)->b1 = flag;
        obj->unk10A = 0x14;
    }
}

void obj_load_resources(struct Obj *obj, u16 id)
{
    u32 slot;
    u8 *list;
    s32 i;
    u32 state;

    list = obj->resList = bank_data_5bit(id, &obj->resCount);
    for (i = 0; i < obj->resCount; i++, list += 2)
    {
        find_or_alloc_obj_slots(0, *(u16 *)list, 0, 0, 0, &slot, -1);
    }
    state = obj->unk246 & 0x1C;
    if (state == 0x10)
    {
        find_or_alloc_obj_slots(0, 0x1003, 0, 0, 0, &slot, 0xC);
    }
    else if (state == 4)
    {
        find_or_alloc_obj_slots(0, 0x1004, 0, 0, 0, &slot, 0xC);
    }
    find_or_alloc_obj_slots(0, 0x8003, 0, 0, 0, &slot, 0xD);
    find_or_alloc_obj_slots(0, 0x8000, 0, 0, 0, &slot, 0xE);
    find_or_alloc_obj_slots(0, 0x8001, 0, 0, 0, &slot, 0xF);
}

ASM_FUNC("asm/code_08025650/sprites_obj_setup_sprites.s", void sprites_obj_setup_sprites(void))

void build_scene_children(struct SceneObj *obj, u32 def)
{
    s32 i;
    s32 j;
    u8 kind;
    u32 hint;
    struct SceneItem *item;

    bank_byte1_high(def, &obj->entryCount);
    obj->first1 = 0;
    obj->count1 = 0;
    obj->first3 = 0;
    obj->count3 = 0;
    obj->first7 = 0;
    obj->count7 = 0;
    obj->first5 = 0;
    obj->count5 = 0;
    obj->first4 = 0;
    obj->count4 = 0;
    obj->first6 = 0;
    obj->count6 = 0;
    obj->first2 = 0;
    obj->count2 = 0;

    for (i = 0; i < obj->entryCount; i++)
    {
        kind = object_def_mode(def, i);
        switch (kind)
        {
        case 0:
            item = obj_init_defaults(heap_alloc2(0x38C, 1, gHeapTagItemKind0, 1), def, i);
            if (obj->flagsFB & 4)
                obj_set_motion_params(item, 0x100, 0x100, 0xD2, 0xD);
            else
                obj_set_motion_params(item, 0x200, 0x200, 0x52C, 0x75);
            item->ptr368 = (u32)&obj->slots20C[i];
            obj->entries[i] = item;
            break;
        case 1:
            obj->entries[i] = obj_init(heap_alloc2(0x33C, 1, gHeapTagItemKind1, 1), def, i);
            if (obj->first1 == 0)
                obj->first1 = i;
            obj->count1++;
            break;
        case 2:
            obj->entries[i] = obj_init(heap_alloc2(0x33C, 1, gHeapTagItemKind2, 1), def, i);
            if (obj->first2 == 0)
                obj->first2 = i;
            obj->count2++;
            break;
        case 3:
            obj->entries[i] = obj_init_with_child(heap_alloc2(0x348, 1, gSceneObjKind3Tag, 1), def, i);
            if (obj->entries[i]->child != 0)
                obj->entries[i]->child->index54 = i;
            if (obj->first3 == 0)
                obj->first3 = i;
            obj->count3++;
            break;
        case 4:
            obj->entries[i] = obj_init(heap_alloc2(0x33C, 1, gSceneObjKind4Tag, 1), def, i);
            if (obj->first4 == 0)
                obj->first4 = i;
            obj->count4++;
            break;
        case 5:
            item = obj_init(heap_alloc2(0x33C, 1, gSceneObjKind5Tag, 1), def, i);
            if (test_flag(gUnk_03000FC0 + 0x38C, 0, item->bits22C.id + 0x1160) == 1)
            {
                item->bits20C.b0 = 0;
                item->bits20F.b1 = 0;
            }
            item->bits210.b5 = 1;
            obj->entries[i] = item;
            if (obj->first5 == 0)
                obj->first5 = i;
            obj->count5++;
            break;
        case 6:
            obj->entries[i] = obj_init_flags(heap_alloc2(0x33C, 1, gSceneObjKind6Tag, 1), def, i);
            if (obj->first6 == 0)
                obj->first6 = i;
            obj->count6++;
            break;
        case 7:
            obj->entries[i] = obj_init_state(heap_alloc2(0x350, 1, gSceneObjKind7Tag, 1), def, i);
            if (obj->first7 == 0)
                obj->first7 = i;
            obj->count7++;
            break;
        default:
            obj->entries[i] = 0;
            break;
        }
        obj->entries[i]->bits210.b4 = obj->entries[i]->bits20C.b5;
        obj->entries[i]->mode239 = 0x10 >> ((struct Bits8 *)&obj->flagsFB)->b2;
        obj->entries[i]->index54 = i;
    }

    obj->entries[((struct Bits8 *)&obj->flags208)->b2]->bits352.b7 = 1;
    hint = obj->first1;
    if (hint == 0)
    {
        hint = obj->first2;
        if (hint == 0)
            hint = obj->first3;
    }
    obj->hint11C = hint;
    obj->sum11D = obj->count1 + obj->count2 + obj->count3;

    for (j = 0; j < 8; j++)
        obj->slotsA8[j] = set_low3_bits(heap_alloc2(0x4C, 1, gSceneObjItemTag, 1), j);
}


ASM_FUNC("asm/code_08025650/build_object_table.s", void build_object_table(void))
