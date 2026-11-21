#include "global.h"

/* The object's main sprite */
struct FieldSub
{
    u16 x;     /* screen px */
    u16 y;
    u8 filler04[0x8];
    u16 unk0C;
    u16 depth; /* sort key: (layer << 12) | (0x800 - y px), or 0xFFFF */
    u8 filler10[0x1];
    u8 unk11;
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
    u16 screenX;  /* px, from x and the camera */
    u16 screenY;  /* px, from y and unk14 and the camera */
    u16 unk0A;
    s32 unk0C;    /* x, 1/256 px */
    s32 unk10;    /* y, 1/256 px */
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
    s8 spriteOfsX; /* px added to the main sprite's position (obj_update_mode wobbles it) */
    s8 spriteOfsY;
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
    s32 unk258;
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
    /* rotation tween (axis E of code_08045F18.c's tweens, mode in flags2B6 bits 4-5) */
    u16 unk322;   /* base: sub->unk0C (rotation) when it started */
    u16 unk324;   /* step per frame (TWEEN_STEP) */
    u8 filler326[0x2];
    u32 unk328;   /* target offset from the base, 8.8 */
    u32 unk32C;   /* current offset */
    u16 unk330;   /* frames run */
    u16 unk332;   /* frames to take (TWEEN_TIMED) */
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
    u16 attrSpriteId;
    u8 attrOfsX;     /* s8: attr sprite's x, y and depth relative to the main sprite */
    u8 attrOfsY;
    u8 attrOfsDepth;
    u8 filler375[0x3];
    /* attr sprite state kept while the sprite is gone (obj_copy_sprite_attrs saves it) */
    u8 savedAttrFlags;  /* bit 0 valid, 1 attr 0x11 bit 6, 2-3/4-5 attr 0x1F bits 0-1/2-3, 6-7 attr 0x12 bits 1-2 */
    u8 savedAttrFlags2; /* bits 0-1: attr 0x12 bits 3-4 */
    u8 filler37A[0x1];
    u8 savedAnim;
    u8 savedFrame;
    u8 filler37D[0x1];
    u16 savedAttr2A;
    u16 savedAttr2C;
    u16 savedAttr04;
    u16 savedAttr06;
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

/* The object's second sprite, which can follow the main one */
struct FieldAttrEntry
{
    u16 x;
    u16 y;
    u16 unk04;
    u16 unk06;
    u8 filler08[0x6];
    u16 depth;
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

/* Starts obj's rotation tween: turns `angle` (whole units, the sign of `dir` gives the way) from the
 * sprite's current rotation, by `stepOrFrames` per frame (mode 1, TWEEN_STEP) or over `stepOrFrames`
 * frames (mode 2, TWEEN_TIMED). A timed tween of angle 0 isn't started: returns 0, else 1. */
u32 obj_set_move_params(struct FieldObj *obj, u32 mode, u16 angle, s32 dir, u16 stepOrFrames)
{
    if (mode == 2 && angle == 0)
        return 0;
    obj->unk322 = obj->sub->unk0C;
    if (dir >= 0)
        dir = 1;
    else
        dir = -1;
    obj->unk328 = angle * (dir << 8);
    if (mode == 1)
        obj->unk324 = stepOrFrames * dir;
    else
        obj->unk332 = stepOrFrames;
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

void obj_alloc_entry(struct FieldObj *obj, u8 mode)
{
    if (obj->entryKey != 0xFFFF)
    {
        s32 shift = 0;
        s32 mask = obj->sub->mask14;

        while (((((u8)(mask >> shift)) ^ 1) & 1) != 0)
            shift++;
        obj->entry = sprite_alloc(obj->entryTable, mode, obj->entryKey, (u8)(shift + 0x10), 0xFFFF, 0xFFFF);
        set_u8_bits(&obj->flags216, 0x40);
    }
}

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
struct FieldObj *obj_init(struct FieldObj *obj, s32 def, u32 index);

/*
 * Constructor of the default field object class: builds the base object (obj_init, which loads
 * object definition `def` when it isn't -1), switches the class table to gObjDefaultClass and
 * resets the class's own fields: the 7-bit values at 0x33C/0x33D to 0x7F, 0x33E to 0xFF, the
 * sprite flags in 0x352 and the attr sprite (none yet).
 */
struct FieldObj *obj_init_defaults(struct FieldObj *obj, s32 def, u32 index)
{
    obj_init(obj, def, index);
    obj->data = (struct FieldData *)gObjDefaultClass;
    obj->unk355 = 0;
    ((struct FieldBit7 *)&obj->unk33C)->low = 0x7F;
    ((struct FieldBit7 *)&obj->unk33D)->low = 0x7F;
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
    obj->attr = NULL;
    return obj;
}

extern u8 *gSelectStateCodePtrTable[];

/* Picks the object's state code (unk344) for `mode`. The base code comes from its character's
   table (gSelectStateCodePtrTable[unk54], 0 or 1) at its action unk04. For actions 0-4 and 0xD,
   modes 0-4 override it for one character or the other (flags213 bit 5 picks between pairs);
   in mode 2 character 0 also turns 7 into 0xC and 9 into 0xE. Last, codes 7 and 0xC become
   their variants 8 and 0xD when last frame's code (unk345) was that variant, unk343 is set, or
   unk2C is 6 or 7. */
void obj_select_state_code(struct FieldObj *obj, u32 mode)
{
    s32 code = *(s8 *)(gSelectStateCodePtrTable[obj->unk54] + obj->unk04);

    if ((u16)obj->unk04 <= 4 || obj->unk04 == 0xD)
    {
        switch (mode)
        {
        case 0:
            if (code == 1 && (obj->flags213 & 0x20) == 0)
                code = 0;
            break;
        case 1:
            if (obj->unk54 == 0)
                code = 0x17;
            break;
        case 2:
            if (obj->unk54 == 0)
            {
                if (code == 1 && (obj->flags213 & 0x20) != 0)
                    code = 0xF;
                else
                    code = 0xB;
            }
            break;
        case 3:
            if (obj->unk54 == 1)
                code = 0x10;
            break;
        case 4:
            if (obj->unk54 == 1)
                code = 0x12;
            break;
        case 5:
        case 6:
        default:
            break;
        }
    }
    if (obj->unk54 == 0 && mode == 2)
    {
        if (code == 7)
            code = 0xC;
        else if (code == 9)
            code = 0xE;
    }
    if (code == 7)
    {
        if (obj->unk345 == 8 || obj->unk343 != -1 || obj->unk2C == 6 || obj->unk2C == 7)
            code = 8;
    }
    else if (code == 0xC)
    {
        if (obj->unk345 == 0xD || obj->unk343 != -1 || obj->unk2C == 6 || obj->unk2C == 7)
            code = 0xD;
    }
    obj->unk344 = code;
}

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

void obj_update_screen_pos(struct FieldObj *obj, s32 a, s32 b, void *table);

/* Places the object's sprites for a camera at (camX, camY). Objects flagged for simple placement
 * (unk352 bit 6) get their screen position straight from x, y and unk14, and their main sprite is
 * put there in front (depth 0xFFFF), unless they have no sprite yet or are culled; the rest go
 * through obj_update_screen_pos. Then the attr sprite, if shown and set to follow (unk353 bit 0),
 * is moved to the main sprite plus its offsets and takes the main sprite's priority bits. */
void obj_place_sprite(struct FieldObj *obj, u16 camX, u16 camY, void *objs)
{
    u8 culled;
    s32 sx;
    s32 sy;

    if ((obj->unk352 & 0x40) != 0)
    {
        if ((obj->flags20F & 2) != 0)
        {
            culled = obj->flags216 & 0x20;
            if (culled == 0)
            {
                obj->screenX = sx = obj->unk0C / 0x100 - (s16)camX;
                sy = (obj->unk10 - obj->unk14) / 0x100 - (s16)camY - 3;
                obj->screenY = sy;
                obj->sub->x = obj->screenX + (s8)obj->spriteOfsX;
                obj->sub->y = obj->screenY - obj->unk18 / 0x100 + ((s8)obj->spriteOfsY + 4);
                obj->sub->depth = 0xFFFF;
                obj->unk0A = 0;
            }
        }
    }
    else
    {
        obj_update_screen_pos(obj, (s16)camX, (s16)camY, objs);
    }

    if ((obj->attr->unk12 & 0x20) == 0)
    {
        if ((obj->unk353 & 1) != 0)
        {
            obj->attr->x = obj->sub->x + (s8)obj->attrOfsX;
            obj->attr->y = obj->sub->y + (s8)obj->attrOfsY;
            obj->attr->depth = obj->sub->depth + (s8)obj->attrOfsDepth;
            ((struct FieldBits1F *)&obj->attr->unk1F)->lo = (((u32)obj->sub->unk1F << 30) >> 30);
        }
    }
}

/* Creates the object's sub sprite and its attr sprite. If obj_copy_sprite_attrs saved the attr sprite's
 * state earlier, the sprite is rebuilt from it (animation, frame, flags, timers) and the save is used up;
 * otherwise an empty attr sprite is made. */
void init_pos_obj_refresh_sprite(struct FieldObj *obj)
{
    obj_create_sub(obj);
    if ((obj->savedAttrFlags & 1) != 0)
    {
        obj->attr = create_obj_de4(0, obj->attrSpriteId, 1, -1, -1, -1, -1);
        obj_load_record(obj->attr, obj->savedAnim, -1, 0, 0);
        obj_load_record(obj->attr, obj->savedAnim, obj->savedFrame, 0, 0);
        obj_queue_update(obj->attr);
        ((struct FieldBit6 *)&obj->attr->unk11)->bit6 = (((u32)obj->savedAttrFlags << 30) >> 31);
        ((struct FieldBits1F *)&obj->attr->unk1F)->lo = (((u32)obj->savedAttrFlags << 28) >> 30);
        ((struct FieldBits1F *)&obj->attr->unk1F)->hi = (((u32)obj->savedAttrFlags << 26) >> 30);
        ((struct FieldBits12 *)&obj->attr->unk12)->mid2 = obj->savedAttrFlags >> 6;
        ((struct FieldBit3 *)&obj->attr->unk12)->bit3 = (((u32)obj->savedAttrFlags2 << 31) >> 31);
        ((struct FieldBit4 *)&obj->attr->unk12)->bit4 = (((u32)obj->savedAttrFlags2 << 30) >> 31);
        obj->attr->unk2A = obj->savedAttr2A;
        obj->attr->unk2C = obj->savedAttr2C;
        obj->attr->unk04 = obj->savedAttr04;
        obj->attr->unk06 = obj->savedAttr06;
        ((struct FieldBit0 *)&obj->savedAttrFlags)->active = 0;
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

/* Saves the object's sprites so init_pos_obj_refresh_sprite can rebuild them later: the main
 * sprite through obj_copy_sub_fields, then (unless the attr sprite's unk12 bit 5 is set) the attr
 * sprite's flag bits, animation, frame and timers, and marks that save valid. */
void obj_copy_sprite_attrs(struct FieldObj *obj)
{
    obj_copy_sub_fields(obj);
    if ((obj->attr->unk12 & 0x20) == 0)
    {
        set_flags378_b1(&obj->savedAttrFlags, ((u32)obj->attr->unk11 << 25) >> 31);                     // unk11 bit 6
        ((struct FieldBits378 *)&obj->savedAttrFlags)->b23 = ((u32)obj->attr->unk1F << 30) >> 30;      // unk1F bits 0-1
        ((struct FieldBits378 *)&obj->savedAttrFlags)->b45 = ((u32)obj->attr->unk1F << 28) >> 30;      // unk1F bits 2-3
        ((struct FieldBits378 *)&obj->savedAttrFlags)->b67 = ((u32)obj->attr->unk12 << 29) >> 30;      // unk12 bits 1-2
        set_flags379_b0(&obj->savedAttrFlags2, ((u32)obj->attr->unk12 << 28) >> 31);                   // unk12 bit 3
        ((struct FieldBits379 *)&obj->savedAttrFlags2)->b1 = ((u32)obj->attr->unk12 << 27) >> 31;      // unk12 bit 4
        obj->savedAnim = obj->attr->unk21;
        obj->savedFrame = obj->attr->unk23;
        obj->savedAttr2A = obj->attr->unk2A;
        obj->savedAttr2C = obj->attr->unk2C;
        obj->savedAttr04 = obj->attr->unk04;
        obj->savedAttr06 = obj->attr->unk06;
        set_u8_bits(&obj->savedAttrFlags, 1); // the save is valid
    }
}

void obj_sub_frame_update(struct FieldObj *obj, u8 force)
{
    u8 want;
    u32 dir;
    s32 extra;

    if (obj->unk04 >= 0 && obj->unk04 <= 3)
    {
        u32 b;
        u32 hi;

        extra = 0;
        b = obj->flags216;
        if (!(b & 0xC))
        {
            want = obj->sub->unk21;
            dir = 0;
        }
        else
        {
            u8 t = gFrameAttrTable[(((b << 28) >> 30) << 4) + (((b << 27) >> 31) << 3) + (((u32)obj->stage << 29) >> 29)];
            want = t & 0x7F;
            dir = t >> 7;
            hi = 0x80;
            hi &= b;
            if (hi
                && (((struct FieldBit3 *)&obj->state)->low3 == 2 || ((struct FieldBit3 *)&obj->state)->low3 == 3)
                && (obj->unk04 == 2 || obj->unk04 == 3))
                want += (obj->unk04 - 1) * 5;
        }
        if (obj->unk04 == 1 && obj->unk2C == 0
            && (((struct FieldBit3 *)&obj->state)->low3 == 2 || ((struct FieldBit3 *)&obj->state)->low3 == 3))
            extra = 1;
        if (obj->sub->unk21 != want || (((u32)obj->sub->unk11 << 25) >> 31) != dir || force != 0)
        {
            obj_load_record(obj->sub, want, extra, 0, 0);
            ((struct FieldBit6 *)&obj->sub->unk11)->bit6 = dir;
        }
    }
    else
    {
        u8 t = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4) + ((((u32)obj->flags216 << 27) >> 31) << 3) + (((u32)obj->stage << 29) >> 29)];
        want = t & 0x7F;
        dir = t >> 7;
        if (obj->unk352 & 0x80)
        {
            switch (obj->unk04)
            {
            case 6:
            case 8:
                want += 10;
                break;
            case 7:
            case 9:
                want += 5;
                break;
            case 11:
            case 12:
                want += 5;
                break;
            case 20:
                want += 20;
                break;
            case 25:
            case 26:
                want += 15;
                break;
            case 33:
            case 34:
            case 35:
            case 36:
            case 39:
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
                want += 15;
                break;
            case 63:
            case 133:
            case 141:
            case 149:
            case 157:
                want += 5;
                break;
            case 116:
            case 124:
                want += 10;
                break;
            case 72:
            case 88:
            case 101:
                if ((obj->flags216 & 0x80) && obj->counter25C >= 0)
                {
                    if (obj->unk258 > 0)
                        want += 5;
                    else if (obj->unk258 < 0)
                        want += 10;
                }
                break;
            case 115:
                want += 5;
                break;
            case 161:
                want += 2;
                break;
            case 172:
            case 173:
                want += 5;
                break;
            case 165:
            case 166:
                want += 10;
                break;
            case 167:
                want += 15;
                break;
            case 169:
                want += 30;
                break;
            case 171:
                want += 30;
                break;
            case 174:
                want += 20;
                break;
            case 176:
                want += 25;
                break;
            }
        }
        else
        {
            switch (obj->unk04)
            {
            case 11:
            case 12:
            case 78:
            case 98:
            case 109:
            case 110:
            case 120:
            case 121:
            case 145:
            case 146:
            case 153:
            case 154:
                want += 5;
                break;
            case 16:
            case 18:
            case 23:
            case 27:
            case 29:
            case 31:
            case 37:
            case 46:
            case 48:
            case 53:
            case 63:
            case 64:
            case 66:
            case 68:
            case 81:
            case 83:
            case 86:
            case 102:
            case 105:
            case 172:
                want += 5;
                break;
            case 17:
            case 19:
            case 24:
            case 28:
            case 30:
            case 32:
            case 38:
            case 47:
            case 49:
            case 54:
            case 65:
            case 67:
            case 69:
            case 82:
            case 84:
            case 87:
            case 103:
            case 106:
            case 116:
            case 124:
            case 173:
                want += 10;
                break;
            case 101:
            case 160:
                if ((obj->flags216 & 0x80) && obj->counter25C >= 0)
                {
                    if (obj->unk258 > 0)
                        want += 5;
                    else if (obj->unk258 < 0)
                        want += 10;
                }
                break;
            case 20:
            case 25:
            case 26:
                want += 20;
                break;
            case 161:
                want += 1;
                break;
            case 162:
                want += 2;
                break;
            case 163:
                want += 3;
                break;
            case 165:
            case 166:
                want += 10;
                break;
            case 167:
                want += 15;
                break;
            case 175:
                want += 20;
                break;
            case 33:
            case 34:
            case 35:
            case 36:
            case 39:
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 176:
                want += 25;
                break;
            }
        }
        if (obj->sub->unk21 != want || (((u32)obj->sub->unk11 << 25) >> 31) != dir || force != 0)
        {
            obj_load_record(obj->sub, want, 0, 0, 0);
            ((struct FieldBit6 *)&obj->sub->unk11)->bit6 = dir;
        }
    }
}
