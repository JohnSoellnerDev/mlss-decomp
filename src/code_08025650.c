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

extern struct SysRam gUnk_0300034C;
extern const u16 gInputSequenceKeys[];
u8 *bank_data_5bit(u16 bank, u8 *out);
void find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, u32 *out, s32 slot);

ASM_FUNC("asm/code_08025650/obj_update_entities.s", void obj_update_entities(void))

ASM_FUNC("asm/code_08025650/process_obj_entries.s", void process_obj_entries(void))

ASM_FUNC("asm/code_08025650/obj_collision_mask.s", void obj_collision_mask(void))

ASM_FUNC("asm/code_08025650/obj_test_mode.s", void obj_test_mode(void))

u32 obj_sequence_step(struct Obj *obj)
{
    if (gUnk_0300034C.pressed != 0)
    {
        if (gUnk_0300034C.pressed == gInputSequenceKeys[obj->seqIndex])
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

ASM_FUNC("asm/code_08025650/obj_find_hit_entry.s", void obj_find_hit_entry(void))

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

NONMATCH("asm/code_08025650/obj_build_message.s", void obj_build_message(struct Obj *obj, u32 unused, u32 kind, u32 index, u32 flag))
{
    u8 *text = 0;
    u8 *entry;
    u8 *table;
    u32 offset;
    u8 *dst;
    u8 *src;
    u8 ch;

    if ((u32)(kind - 4) <= 7)
    {
        table = gItemTable;
        table += 0xC;
        offset = index << 4;
    }
    else
    {
        if ((s32)kind <= 11)
            goto check;
        table = gStringTable12;
        offset = ((index << 1) + index) << 2;
    }
    entry = offset + table;
    {
        u32 lang = gUnk_0300034C.lang8C0;
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
END_NONMATCH

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

ASM_FUNC("asm/code_08025650/build_scene_children.s", void build_scene_children(void))

ASM_FUNC("asm/code_08025650/build_object_table.s", void build_object_table(void))
