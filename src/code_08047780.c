#include "global.h"

struct FieldSub
{
    u8 filler0[0xC];
    u16 unk0C;
    u8 fillerE[0x4];
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
    u8 filler3[0x9];
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
    u8 filler2B[0x9];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x8];
    u8 unk4C;
    u8 filler4D[0x1BF];
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
    u8 filler23A[0x2];
    u8 unk23C;
    u8 filler23D[0x1];
    s16 unk23E;
    u16 unk240;
    u8 filler242[0xA];
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
    u8 filler33F[0x1];
    u8 unk340;
    u8 filler341[0x5];
    u8 unk346;
    u8 unk347;
    u8 filler348[0x9];
    u8 unk351;
    u8 unk352;
    u8 unk353;
    u8 filler354[0x1];
    u8 unk355;
    u8 filler356[0x2];
    u8 unk358;
    u8 unk359;
    u8 filler35A[0x12];
    struct FieldAttrEntry * attr;
    u8 filler370[0x8];
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
void obj_queue_update(void *sub, u32 key, u8 *flags, u32 c);
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
    u8 filler0[0x4];
    u16 unk04;
    u16 unk06;
    u8 filler08[0x11 - 0x08];
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

ASM_FUNC("asm/code_08047780/obj_set_stage.s", void obj_set_stage(void))

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

ASM_FUNC("asm/code_08047780/obj_flags_2b6_30_set.s", void obj_flags_2b6_30_set(void))

ASM_FUNC("asm/code_08047780/obj_flags_2b6_0c_set.s", void obj_flags_2b6_0c_set(void))

ASM_FUNC("asm/code_08047780/obj_flags_2b6_03_set.s", void obj_flags_2b6_03_set(void))

ASM_FUNC("asm/code_08047780/obj_flags_2b5_c0_set.s", void obj_flags_2b5_c0_set(void))

ASM_FUNC("asm/code_08047780/obj_flags_2b5_30_set.s", void obj_flags_2b5_30_set(void))

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

ASM_FUNC("asm/code_08047780/obj_flags_2b5_07_set.s", void obj_flags_2b5_07_set(void))

ASM_FUNC("asm/code_08047780/obj_set_motion_mode.s", void obj_set_motion_mode(void))

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

ASM_FUNC("asm/code_08047780/obj_reset_link_fields.s", void obj_reset_link_fields(void))

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

ASM_FUNC("asm/code_08047780/obj_alloc_entry.s", void obj_alloc_entry(struct FieldObj *obj, u8 mode))

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

ASM_FUNC("asm/code_08047780/obj_init_defaults.s", void obj_init_defaults(void))

ASM_FUNC("asm/code_08047780/obj_select_state_code.s", void obj_select_state_code(void))

ASM_FUNC("asm/code_08047780/obj_store_attrs.s", void obj_store_attrs(void))

ASM_FUNC("asm/code_08047780/obj_place_sprite.s", void obj_place_sprite(void))

ASM_FUNC("asm/code_08047780/init_pos_obj_refresh_sprite.s", void init_pos_obj_refresh_sprite(void))

ASM_FUNC("asm/code_08047780/obj_copy_sprite_attrs.s", void obj_copy_sprite_attrs(void))

ASM_FUNC("asm/code_08047780/obj_sub_frame_update.s", void obj_sub_frame_update(void))
