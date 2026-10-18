#include "global.h"

struct FieldSub
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0x8];
    u16 unk0C;
    u16 unk0E;
    u8 filler10[0x2];
    u8 unk12;
    u8 filler13[0x1];
    u16 mask14;
    u8 filler16[0x9];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
};

struct FieldData
{
    u8 filler0[0x38];
    s16 unk38;
    u8 filler3A[0x2];
    void (*callback)(u8 *, u32);
    u8 filler40[0x78 - 0x40];
    s16 unk78;
    u8 filler7A[0x2];
    void (*callbackA)(u8 *, u32);
};

struct FieldObj
{
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 filler3[0x1];
    s16 unk04;
    u16 unk06;
    u16 unk08;
    u16 unk0A;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u32 * ptr20;
    u8 stage;
    u8 unk25;
    u8 flags26;
    u8 filler27[0x2];
    u8 unk29;
    u8 unk2A;
    u8 filler2B[0x1];
    s16 unk2C;
    u8 filler2E[0x6];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x8];
    u8 unk4C;
    u8 filler4D[0x7];
    u8 unk54;
    u8 filler55[0x1B7];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 unk210;
    u8 unk211;
    u8 flags212;
    u8 flags213;
    u8 state;
    u8 req;
    u8 flags216;
    u8 flags217;
    u16 id;
    u16 idPrevState7;
    u16 idPrevReq0;
    u8 filler21E[0x4];
    u8 unk222;
    u8 filler223[0x15];
    s8 linkIndex;
    u8 mode;
    s8 unk23A;
    s8 unk23B;
    u8 unk23C;
    u8 filler23D[0x1];
    s16 unk23E;
    u16 unk240;
    u16 unk242;
    u16 unk244;
    u8 filler246[0x6];
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    u8 filler258[0x4];
    s32 counter25C;
    u16 unk260;
    u16 unk262;
    u16 unk264;
    u16 unk266;
    u16 unk268;
    u8 filler26A[0x2];
    u16 entryKey;
    u8 entry;
    u8 filler26F[0x5];
    u16 unk274;
    u8 filler276[0x1A];
    void * owner;
    struct FieldSub * sub;
    u8 filler298[0x10];
    u16 * idTable;
    u8 filler2AC[0x4];
    u8 * moveTable;
    u8 unk2B4;
    u8 flags2B5;
    u8 flags2B6;
    u8 filler2B7[0x6B];
    u16 unk322;
    u16 unk324;
    u8 filler326[0x2];
    u32 unk328;
    u32 unk32C;
    u16 unk330;
    u16 unk332;
    void * entryTable;
    struct FieldData * data;
    u8 unk33C;
    u8 unk33D;
    u8 unk33E;
    u8 unk33F;
    u8 unk340;
    u8 unk341;
    u8 unk342;
    s8 unk343;
    u8 unk344;
    s8 unk345;
    u8 unk346;
    u8 unk347;
    u8 filler348[0x8];
    u8 unk350;
    u8 unk351;
    u8 unk352;
    u8 unk353;
    u8 unk354;
    u8 unk355;
    u8 filler356[0x2];
    u8 unk358;
    u8 unk359;
    u8 filler35A[0x8];
    u16 unk362;
    u16 unk364;
    u8 filler366[0x2];
    struct FieldSpr16 * unk368;
    struct FieldAttrEntry * attr;
          u16 unk370;
    u8 unk372;
    u8 unk373;
    u8 unk374;
    u8 filler375[0x3];
    u8 unk378;
    u8 unk379;
    u8 filler37A[0x1];
    u8 unk37B;
    u8 unk37C;
    u8 filler37D[0x1];
    u16 unk37E;
    u16 unk380;
    u16 unk382;
    u16 unk384;
};

struct FieldBits213
{
    u8 low : 6;
    u8 hi : 2;
};

struct FieldBits212
{
    u16 low : 7;
    u16 mode : 2;
    u16 high : 7;
};

void obj_update_special_flag(struct FieldObj *obj);
void obj_refresh_anim_state(struct FieldObj *obj, u32 a, s32 b, s32 c);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_init_motion(struct FieldObj *obj, u32 value, s32 b, s32 c);
void motion_obj_set_move_target(struct FieldObj *obj, u32 value, s32 b, s32 c);
void menu_entry_mark_1(void *table, u32 idx);
void menu_entry_mark_2(void *table, u32 idx);
void menu_entry_refresh(void *table, u32 idx, u32 mode);
void obj_alloc_entry(struct FieldObj *obj, u8 mode);
void heap_free_b(void *p);
void obj_queue_update();
void obj_create_sub(struct FieldObj *obj);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
extern const u8 gObjBaseClass[];
u32 arc_tan2(s32 x, s32 y);

struct FieldBit0
{
    u8 active : 1;
    u8 rest : 7;
};

struct FieldBit7
{
    u8 low : 7;
    u8 b7 : 1;
};

struct FlagPairs
{
    u8 b01 : 2;
    u8 b23 : 2;
    u8 b45 : 2;
    u8 b67 : 2;
};

struct FieldTail
{
    s16 off;
    u16 pad;
    void (*callback)(u8 *, u32);
};

struct FieldAttrEntry
{
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u8 filler08[0x6];
    u16 unk0E;
    u8 filler10;
    u8 unk11;
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 unk1F;
    u8 filler20;
    u8 unk21;
    u8 filler22;
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    u16 unk2A;
    u16 unk2C;
};

struct FieldBits6
{
    u8 low6 : 6;
    u8 hi2 : 2;
};

struct FieldBits12
{
    u8 b0 : 1;
    u8 mid2 : 2;
    u8 rest : 5;
};

struct FieldBit3
{
    u8 low3 : 3;
    u8 bit3 : 1;
    u8 high4 : 4;
};

struct FieldBit5
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high2 : 2;
};

struct FieldBit6
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 high1 : 1;
};

struct FieldBits378
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b23 : 2;
    u8 b45 : 2;
    u8 b67 : 2;
};

struct FieldBits379
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

void obj_set_active(struct FieldObj *obj, u32 on)
{
    ((struct FieldBit0 *)&obj->flags20C)->active = on;
}

static inline void set_u8_bits(u8 *p, u32 v)
{
    *p |= v;
}

void obj_mark_dirty(struct FieldObj *obj)
{
    set_u8_bits(&obj->flags20D, 0x10);
    set_u8_bits(&obj->flags217, 1);
}

struct FieldBit4
{
    u8 low : 4;
    u8 bit4 : 1;
    u8 high : 3;
};

struct FieldBits24
{
    u32 low : 3;
    u32 b3 : 4;
    u32 b7 : 2;
    u32 b9 : 1;
    u32 b10 : 1;
    u32 b11 : 4;
    u32 b15 : 2;
    u32 rest : 15;
};

struct FieldLow3
{
    u8 low3 : 3;
    u8 high5 : 5;
} __attribute__((packed));

struct FieldBits1F
{
    u8 lo : 2;
    u8 hi : 2;
    u8 rest : 4;
};

void obj_set_sub_1f_bits(struct FieldObj *obj, u32 a, u32 b)
{
    ((struct FieldBit4 *)&obj->flags20D)->bit4 = 0;
    ((struct FieldBits1F *)&obj->sub->unk1F)->lo = a;
    ((struct FieldBits1F *)&obj->sub->unk1F)->hi = b;
    obj->unk23C = b;
}

void obj_set_table_id(struct FieldObj *obj, u8 idx)
{
    u16 newId = obj->idTable[idx];
    u16 oldId = obj->id;

    obj->idPrevReq0 = oldId;
    obj->id = newId;
    if (obj->idPrevReq0 != newId)
    {
        obj_update_special_flag(obj);
        obj_refresh_anim_state(obj, 1, -1, -1);
    }
    ((struct FieldBit0 *)&obj->req)->active = 1;
}

void obj_set_table_id_flag7(struct FieldObj *obj, u8 idx)
{
    u16 newId = obj->idTable[idx];
    u16 oldId = obj->id;

    obj->idPrevState7 = oldId;
    obj->id = newId;
    if (obj->idPrevState7 != newId)
    {
        obj_update_special_flag(obj);
        obj_refresh_anim_state(obj, 1, -1, -1);
    }
    ((struct FieldBit7 *)&obj->state)->b7 = 1;
}

void obj_set_stage(struct FieldObj *obj, u8 stage)
{
    ((struct FieldBits24 *)&obj->stage)->low = stage;
    obj->unk02 = stage << 5;
    ((struct FieldBits24 *)&obj->unk4C)->low = ((struct FieldLow3 *)&obj->stage)->low3;
    obj->unk2A = obj->unk02;
    obj->data->callback((u8 *)obj + obj->data->unk38, 0);
}

void flags_obj_set_mode(struct FieldObj *obj, u8 mode)
{
    obj->mode = mode;
    obj->sub->unk20 = mode;
}

void obj_refresh_sub_attr(struct FieldObj *obj)
{
    obj_load_record(obj->sub, obj->sub->unk21, 0, 0, 0);
}

void set_half_23e(struct FieldObj *obj, u32 v)
{
    obj->unk23E = v;
}

void obj_clear_flags_2b6_30(struct FieldObj *obj)
{
    ((struct FlagPairs *)&obj->flags2B6)->b45 = 0;
}

void obj_clear_flags_2b6_0c(struct FieldObj *obj)
{
    ((struct FlagPairs *)&obj->flags2B6)->b23 = 0;
}

void obj_clear_flags_2b6_03(struct FieldObj *obj)
{
    ((struct FlagPairs *)&obj->flags2B6)->b01 = 0;
}

void obj_clear_flags_2b5_c0(struct FieldObj *obj)
{
    obj->flags2B5 &= 0x3F;
}

void obj_clear_flags_2b5_30(struct FieldObj *obj)
{
    ((struct FlagPairs *)&obj->flags2B5)->b45 = 0;
}

s32 obj_flags_2b6_30_set(struct FieldObj *obj)
{
    u32 v = obj->flags2B6 & 0x30;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

s32 obj_flags_2b6_0c_set(struct FieldObj *obj)
{
    u32 v = obj->flags2B6 & 0xC;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

s32 obj_flags_2b6_03_set(struct FieldObj *obj)
{
    u32 v = obj->flags2B6 & 0x3;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

s32 obj_flags_2b5_c0_set(struct FieldObj *obj)
{
    u32 v = obj->flags2B5 & 0xC0;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

s32 obj_flags_2b5_30_set(struct FieldObj *obj)
{
    u32 v = obj->flags2B5 & 0x30;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

u32 obj_set_move_params(struct FieldObj *obj, u32 mode, u16 value, s32 dir, u16 extra)
{
    if (mode == 2 && value == 0)
        return 0;
    obj->unk322 = obj->sub->unk0C;
    if (dir >= 0)
        dir = 1;
    else
        dir = -1;
    obj->unk328 = value * (dir << 8);
    if (mode == 1)
        obj->unk324 = extra * dir;
    else
        obj->unk332 = extra;
    obj->unk32C = 0;
    obj->unk330 = 0;
    ((struct FlagPairs *)&obj->flags2B6)->b45 = mode & 3;
    return 1;
}

u32 flag_2b5_bit3(struct FieldObj *obj)
{
    return (obj->flags2B5 >> 3) & 1;
}

void obj_set_move(struct FieldObj *obj, u32 mode, u32 value)
{
    s32 saved = obj->unk23E;

    if (mode == 0)
        obj_init_motion(obj, value, -1, -1);
    else
        motion_obj_set_move_target(obj, value, -1, -1);
    obj->unk23E = saved;
    ((struct FieldBit3 *)&obj->flags2B5)->bit3 = 1;
}

void obj_set_limit(struct FieldObj *obj, u32 value)
{
    obj->unk268 = obj->unk266;
    obj->unk266 = value;
}

s32 obj_flags_2b5_07_set(struct FieldObj *obj)
{
    u32 v = obj->flags2B5 & 0x7;
    s32 r = (u8)v;
    while (v != 0)
    {
        r = 1;
        break;
    }
    return r;
}

extern const u8 gUnk_083A1E72[];
extern const u8 gFrameAttrTable[];
void obj_set_sprite_link(struct FieldObj *obj, u32 zero);
void obj_set_motion_mode(struct FieldObj *obj, u32 mode)
{
    u32 bits = ((u32)obj->sub->unk12 << 29) >> 30;
    ((struct FieldBits213 *)&obj->flags213)->hi = bits;
    ((struct FieldBits212 *)&obj->flags212)->mode = mode;
    obj->moveTable = mode == 1 ? (u8 *)(gUnk_083A1E72 + 1) : (u8 *)(gFrameAttrTable + 0x31);
    obj->unk2B4 = *obj->moveTable;
    obj_set_sprite_link(obj, 0);
}

s32 obj_compare_depth(struct FieldObj *a, struct FieldObj *b)
{
    u32 wa;

    if (a->unk1C == b->unk1C)
        goto zero;
    if ((a->flags26 & 0xC) == 0 && (b->flags26 & 0xC) == 0)
        goto diff;
    wa = *a->ptr20;
    if ((((*b->ptr20) & wa) << 1) >> 30)
        goto zero;
diff:
    return a->unk1C - b->unk1C;
zero:
    return 0;
}

void obj_update_link_pos(struct FieldObj *obj, struct FieldObj **list, u8 idx)
{
    struct FieldTail *tail;
    struct FieldObj *other;
    s32 r3;

    tail = (struct FieldTail *)((u8 *)obj->data + 0x78);
    tail->callback((u8 *)obj + tail->off, idx);
    other = 0;
    if (obj->linkIndex != -1)
    {
        other = list[obj->linkIndex];
        if (obj->unk14 >= other->unk14 + other->unk18 + other->unk274)
        {
            obj->unk29 = 0xFF;
            obj->unk01 = 0xFF;
            obj->linkIndex = -1;
            other = 0;
        }
    }
    r3 = obj->unk14;
    if (other != 0)
        r3 += other->unk18 + other->unk274;
    obj->unk1C = r3;
}

void obj_reset_link_fields(struct FieldObj *obj)
{
    obj->unk00 = 0xFF;
    obj->unk01 = 0xFF;
    obj->linkIndex = -1;
    ((struct FieldBits24 *)&obj->stage)->b3 = 0;
    ((struct FieldBits24 *)&obj->stage)->b7 = 0;
    ((struct FieldBits24 *)&obj->stage)->b9 = 0;
    ((struct FieldBits24 *)&obj->stage)->b10 = 0;
    ((struct FieldBits24 *)&obj->stage)->b11 = 0;
    ((struct FieldBits24 *)&obj->stage)->b15 = 0;
}

void obj_entry_activate(struct FieldObj *obj)
{
    menu_entry_mark_1(obj->entryTable, obj->entry);
}

void obj_entry_release(struct FieldObj *obj)
{
    menu_entry_mark_2(obj->entryTable, obj->entry);
}

void obj_release_entry(struct FieldObj *obj, u8 mode)
{
    menu_entry_refresh(obj->entryTable, obj->entry, mode);
    ((struct FieldBit6 *)&obj->flags20E)->bit6 = 0;
    obj->entry = 0xFF;
    obj->entryKey = 0xFFFF;
}

u32 sprite_alloc(void *ctx, u32 a, u32 b, u32 c, u32 d, u32 e);

NONMATCH("asm/code_08047780/obj_alloc_entry.s", void obj_alloc_entry(struct FieldObj *obj, u8 mode))
{
    if (obj->entryKey != 0xFFFF)
    {
        s32 shift = 0;
        s32 mask = obj->sub->mask14;

        while (((mask >> shift) & 1) == 0)
            shift++;
        obj->entry = sprite_alloc(obj->entryTable, mode, obj->entryKey, (u8)(shift + 0x10), 0xFFFF, 0xFFFF);
        set_u8_bits(&obj->flags216, 0x40);
    }
}
END_NONMATCH

void obj_request_entry(struct FieldObj *obj, u32 key)
{
    obj->entryKey = key;
    ((struct FieldBit6 *)&obj->flags20E)->bit6 = 1;
    obj_alloc_entry(obj, obj->entry);
}

u32 obj_angle_to_saved_pos(struct FieldObj *obj)
{
    if (obj->unk10 == obj->unk38 && obj->unk0C == obj->unk34)
        return -1;
    return (u16)arc_tan2((s16)(obj->unk38 - obj->unk10), (s16)(obj->unk0C - obj->unk34));
}

void obj_reset_move_state(struct FieldObj *obj)
{
    obj->counter25C = -1;
    ((struct FieldBit5 *)&obj->flags213)->bit5 = 0;
    {
        s32 *p24C = &obj->unk24C;
        s32 *p250 = &obj->unk250;

        *p250 = -1;
        *p24C = -1;
    }
    obj->unk23E = obj->unk240;
}

void obj_set_motion_params(struct FieldObj *obj, u16 a, u32 b, u32 d, u32 e)
{
    obj->unk23E = a;
    obj->unk260 = a;
    obj->unk262 = b;
    obj->unk264 = d;
    obj->unk266 = e;
    obj->unk254 = 0;
    obj->counter25C = -1;
    obj->unk24C = -1;
    obj->unk250 = -1;
}

void obj_set_or_add_pos(struct FieldObj *obj, u32 dx, u32 dy, u32 dz, u32 copy, u32 add)
{
    if (add == 0)
    {
        obj->unk0C = dx;
        obj->unk10 = dy;
        obj->unk18 = dz;
    }
    else
    {
        obj->unk0C += dx;
        obj->unk10 += dy;
        obj->unk18 += obj->unk14 + dz;
    }
    obj->unk14 = 0;
    if (copy != 0)
    {
        struct FieldTail *tail;

        obj->unk34 = obj->unk0C;
        obj->unk38 = obj->unk10;
        obj->unk3C = 0;
        obj->unk40 = obj->unk18;
        if (obj->owner != 0)
        {
            tail = (struct FieldTail *)((u8 *)obj->data + 0x78);
            tail->callback((u8 *)obj + tail->off, 1);
        }
    }
    set_u8_bits(&obj->flags217, 1);
}

void obj_sync_sprite_link(struct FieldObj *obj, u32 unused1, u32 unused2, u32 c)
{
    u32 mode;

    if (obj->unk222 != 0xFF)
    {
        set_u8_bits(&obj->flags20F, 2);
        mode = (((u32)obj->sub->unk12) << 29) >> 30;
        obj_queue_update(obj->sub, 0x294, &obj->flags20F, c);
        ((struct FieldBits12 *)&obj->sub->unk12)->mid2 = mode;
        if (((struct FieldBit6 *)&obj->flags20E)->bit6 != 0)
            obj_alloc_entry(obj, 0xFF);
    }
}

void obj_set_base_table(struct FieldObj *obj, u32 flags)
{
    obj->data = (struct FieldData *)gObjBaseClass;
    if (flags & 1)
        heap_free_b(obj);
}

struct FieldHalf358
{
    u16 lo4 : 4;
    u16 mid8 : 8;
    u16 hi4 : 4;
};

struct FieldBits352
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 hi : 2;
};

extern const u8 gObjDefaultClass[];
void obj_init(struct FieldObj *obj);

struct FieldObj *obj_init_defaults(struct FieldObj *obj)
{
    obj_init(obj);
    obj->data = (struct FieldData *)gObjDefaultClass;
    obj->unk355 = 0;
    set_u8_bits(&obj->unk33C, 0x7F);
    set_u8_bits(&obj->unk33D, 0x7F);
    obj->unk33E = 0xFF;
    obj->unk346 = 0xE;
    obj->unk347 = 7;
    obj->unk340 = 0;
    ((struct FieldBit6 *)&obj->unk351)->bit6 = 0;
    ((struct FieldBit3 *)&obj->unk358)->bit3 = 0;
    ((struct FieldHalf358 *)&obj->unk358)->mid8 = 0;
    ((struct FieldBit4 *)&obj->unk359)->bit4 = 0;
    ((struct FieldBits352 *)&obj->unk352)->b0 = 0;
    ((struct FieldBits352 *)&obj->unk352)->b1 = 0;
    ((struct FieldBits352 *)&obj->unk352)->b2 = 0;
    ((struct FieldBits352 *)&obj->unk352)->b3 = 0;
    ((struct FieldBits352 *)&obj->unk352)->b4 = 0;
    ((struct FieldBits352 *)&obj->unk352)->b5 = 0;
    set_u8_bits(&obj->unk211, 1);
    ((struct FieldBit7 *)&obj->unk210)->b7 = 1;
    set_u8_bits(&obj->unk353, 8);
    obj->attr = 0;
    return obj;
}

ASM_FUNC("asm/code_08047780/obj_select_state_code.s", void obj_select_state_code(void))

struct FieldSpr16
{
    u8 filler0[0x16];
    u8 unk16;
    u8 unk17;
};

struct FieldHalf350
{
    u16 lo7 : 7;
    u16 mid4 : 4;
    u16 hi5 : 5;
};

struct FieldBits34
{
    u8 low3 : 3;
    u8 mid2 : 2;
    u8 high3 : 3;
};

void obj_refresh_dirty(struct FieldObj *obj);

static inline void store_low7(u8 *p, u32 v)
{
    ((struct FieldBit7 *)p)->low = v;
}

static inline void merge_hi(u8 *p, u32 v)
{
    *p = (*p & 0x7F) | v;
}

static inline void store_bit5(u8 *p, u32 v)
{
    ((struct FieldBit5 *)p)->bit5 = v;
}

static inline void store_mid2(u8 *p, u32 v)
{
    ((struct FieldBits34 *)p)->mid2 = v;
}

static inline void store_mid4(u8 *p, u32 v)
{
    ((struct FieldHalf350 *)p)->mid4 = v;
}

static inline void store_b7(u8 *p, u32 v)
{
    ((struct FieldBit7 *)p)->b7 = v;
}

NONMATCH("asm/code_08047780/obj_store_attrs.s", void obj_store_attrs(struct FieldObj *obj))
{
    obj_refresh_dirty(obj);
    store_low7(&obj->unk33D, ((u32)obj->unk33C << 25) >> 25);
    store_bit5(&obj->unk351, ((u32)obj->unk350 << 25) >> 31);
    merge_hi(&obj->unk33D, ((u32)obj->unk33C >> 7) << 7);
    obj->unk33F = obj->unk33E;
    obj->unk341 = obj->unk340;
    obj->unk343 = obj->unk342;
    store_mid4(&obj->unk350, ((u32)obj->unk350 << 28) >> 28);
    store_mid2(&obj->unk351, ((u32)obj->unk350 << 26) >> 30);
    obj->unk345 = obj->unk344;
    obj->unk354 = obj->linkIndex;
    obj->unk362 = obj->unk242;
    obj->unk364 = obj->unk244;
    store_b7(&obj->unk353, ((u32)obj->unk353 << 25) >> 31);
    store_low7(&obj->unk368->unk17, ((u32)obj->unk368->unk16 << 25) >> 25);
    merge_hi(&obj->unk368->unk17, ((u32)obj->unk368->unk16 >> 7) << 7);
    store_low7(&obj->unk368->unk16, 0);
}
END_NONMATCH

ASM_FUNC("asm/code_08047780/obj_place_sprite.s", void obj_place_sprite(void))

void init_pos_obj_refresh_sprite(struct FieldObj *obj)
{
    obj_create_sub(obj);
    if ((obj->unk378 & 1) != 0)
    {
        obj->attr = create_obj_de4(0, obj->unk370, 1, -1, -1, -1, -1);
        obj_load_record(obj->attr, obj->unk37B, -1, 0, 0);
        obj_load_record(obj->attr, obj->unk37B, obj->unk37C, 0, 0);
        obj_queue_update(obj->attr);
        ((struct FieldBit6 *)&obj->attr->unk11)->bit6 = (((u32)obj->unk378 << 30) >> 31);
        ((struct FieldBits1F *)&obj->attr->unk1F)->lo = (((u32)obj->unk378 << 28) >> 30);
        ((struct FieldBits1F *)&obj->attr->unk1F)->hi = (((u32)obj->unk378 << 26) >> 30);
        ((struct FieldBits12 *)&obj->attr->unk12)->mid2 = obj->unk378 >> 6;
        ((struct FieldBit3 *)&obj->attr->unk12)->bit3 = (((u32)obj->unk379 << 31) >> 31);
        ((struct FieldBit4 *)&obj->attr->unk12)->bit4 = (((u32)obj->unk379 << 30) >> 31);
        obj->attr->unk2A = obj->unk37E;
        obj->attr->unk2C = obj->unk380;
        obj->attr->unk04 = obj->unk382;
        obj->attr->unk06 = obj->unk384;
        ((struct FieldBit0 *)&obj->unk378)->active = 0;
    }
    else
    {
        obj->attr = create_obj_de4(0, -1, 1, -1, -1, -1, -1);
    }
}

void obj_copy_sub_fields(struct FieldObj *obj);

static inline void set_flags378_b1(u8 *p, u32 v)
{
    ((struct FieldBits378 *)p)->b1 = v;
}

static inline void set_flags379_b0(u8 *p, u32 v)
{
    ((struct FieldBits379 *)p)->b0 = v;
}

void obj_copy_sprite_attrs(struct FieldObj *obj)
{
    obj_copy_sub_fields(obj);
    if ((obj->attr->unk12 & 0x20) == 0)
    {
        set_flags378_b1(&obj->unk378, ((u32)obj->attr->unk11 << 25) >> 31);
        ((struct FieldBits378 *)&obj->unk378)->b23 = ((u32)obj->attr->unk1F << 30) >> 30;
        ((struct FieldBits378 *)&obj->unk378)->b45 = ((u32)obj->attr->unk1F << 28) >> 30;
        ((struct FieldBits378 *)&obj->unk378)->b67 = ((u32)obj->attr->unk12 << 29) >> 30;
        set_flags379_b0(&obj->unk379, ((u32)obj->attr->unk12 << 28) >> 31);
        ((struct FieldBits379 *)&obj->unk379)->b1 = ((u32)obj->attr->unk12 << 27) >> 31;
        obj->unk37B = obj->attr->unk21;
        obj->unk37C = obj->attr->unk23;
        obj->unk37E = obj->attr->unk2A;
        obj->unk380 = obj->attr->unk2C;
        obj->unk382 = obj->attr->unk04;
        obj->unk384 = obj->attr->unk06;
        set_u8_bits(&obj->unk378, 1);
    }
}

ASM_FUNC("asm/code_08047780/obj_sub_frame_update.s", void obj_sub_frame_update(void))
