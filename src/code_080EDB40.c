#include "global.h"
#include "ram_routines.h"

struct Entity
{
    u8 filler0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 unk25;
    u8 filler26[0x54 - 0x26];
    u8 unk54;
    u8 filler55[0xF8 - 0x55];
    u16 unkF8;
    u8 filler0FA[0x130 - 0xFA];
    u8 unk130;
    u8 unk131;
    u8 filler132[0x134 - 0x132];
    u16 unk134;
    u8 unk136;
    u8 unk137;
    u8 filler138[0x146 - 0x138];
    u16 unk146[8];
    u8 filler156[0x198 - 0x156];
    void *unk198;
    u8 filler19C[0x1B0 - 0x19C];
    u32 unk1B0;
    u32 unk1B4;
    u8 filler1B8[0x1D6 - 0x1B8];
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
          u8 unk20D;
      u8 filler20E[0x20F - 0x20E];
    u8 unk20F;
    u8 filler210[0x214 - 0x210];
    u8 unk214;
    u8 unk215;
    u8 filler216[0x218 - 0x216];
    u16 unk218;
    u8 filler21A[0x21C - 0x21A];
    u16 unk21C;
          u8 filler21E[0x270 - 0x21E];
      u16 unk270;
      u8 filler272[0x294 - 0x272];
    struct SubObj *sub294;
    u8 filler298[0x2B6 - 0x298];
    u8 unk2B6;
    u8 filler2B7[0x2F4 - 0x2B7];
    u32 unk2F4;
    u16 unk2F8;
    u8 filler2FA[0x338 - 0x2FA];
    u8 *unk338;
    u8 unk33C;
    u8 filler33D[0x344 - 0x33D];
    struct Entity *link344;
};

struct MotionBody
{
    u32 flags;
    s32 x;
    s32 y;
    u32 z;
};

struct ObjArgs
{
    u8 unk0;
    u8 filler1[3];
    u32 unk4;
    u8 unk8;
    u8 filler9[3];
    u32 unkC;
};

typedef void (*ObjMethod)(void *self, u32 arg);

struct MethodEntry
{
    s16 offset;
    u16 unk2;
    ObjMethod fn;
};

struct Record
{
    u8 filler0[0xA0];
    u16 unkA0;
    u8 filler1A2[0x140 - 0xA2];
    void *unk140;
    u8 filler144[0x158 - 0x144];
    u32 unk158;
    u32 unk15C;
    u8 filler160[0x17E - 0x160];
    u8 unk17E;
};

struct Track
{
    s32 flags;
    u32 mode;
    s32 x;
    s32 y;
    u32 z;
};

extern void *const gEntityModeDataTable[];

struct SubObj
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x21 - 0x13];
    u8 unk21;
};

struct ObjList
{
    u8 filler0[0x28];
    struct Entity *entries[(0x208 - 0x28) / 4];
    u8 unk208;
};

struct OwnerObj
{
    u8 filler0[0x14];
    struct ObjList *list;
};

struct Bits1
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct SubFlags
{
    u8 bit0 : 1;
    u8 mid : 2;
    u8 rest : 5;
};

struct Flags8
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Mid5Half
{
    u16 lo5 : 5;
    u16 mid5 : 5;
    u16 rest : 6;
};

struct MotionSlot
{
    u32 unk0[4];
    u16 unk10;
    u16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
};

void obj_set_state(void *obj, u32 state);
void obj_finish_move_step(void *obj);
void obj_stop_motion(void *obj);
void obj_set_stage(void *obj, u32 stage);
u32 direction_octant(void *root, s32 x0, s32 y0, s32 x1, s32 y1);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_sync_sub_slot(void *obj, u32 value);
void obj_set_cell_coord(void *obj, u32 cell, u32 arg);
u32 flags_vars_set_obj_state(void *root, void *dst, void *obj, u32 state, u32 zero);
void set_half_23e(void *obj, s16 v);
u8 cell_motion_obj_move_toward_target(void *obj, u32 mode, u32 rel, s32 pos_a0, s32 pos_b0, s32 pos_c0, u32 d6, u32 d7, u32 d8, u16 ext_a, u16 ext_b, u16 ext_c, u16 scale, u16 val6_in, s32 sign_in, u32 bits15);
void set_flag_a0_2(void *a, void *b, u8 *p, u32 c);
extern u8 *gEffectState;

ASM_FUNC("asm/code_080EDB40/command_on_list_entry.s", void command_on_list_entry(void))

ASM_FUNC("asm/code_080EDB40/set_obj_flags_cmd.s", void set_obj_flags_cmd(void))

extern u8 *gEffectState;
void set_flag_f8_40(void *host);
void camera_track_entity(void *list, u32 field, u32 zero);

static inline void set_u8_bit6(u8 *p, u32 v)
{
    ((struct Flags8 *)p)->bit6 = v;
}

static inline void set_bit6_f8(void *host, u32 v)
{
    ((struct Flags8 *)((u8 *)host + 0xF8))->bit6 = v;
}

u32 set_host_flags(struct OwnerObj *owner, void *unused1, u32 *args)
{
    if (args[0] != 0xFF)
    {
        ((struct Mid5Half *)((u8 *)owner->list + 0xFA))->mid5 = args[0];
        set_flag_f8_40(owner->list);
    }
    else
    {
        u32 bit2 = ((u32)*(u8 *)((u8 *)owner->list + 0x208) << 29) >> 31;
        ((struct Mid5Half *)((u8 *)owner->list + 0xFA))->mid5 = bit2;
        set_bit6_f8(owner->list, args[1]);
    }
    if ((*(u8 *)((u8 *)owner->list + 0xF8) & 0x40) != 0)
        camera_track_entity(owner->list, ((u32)*(u16 *)((u8 *)owner->list + 0xFA) << 22) >> 27, 0);
    *(u32 *)(gEffectState + 0x530) = ((u32)*(u16 *)((u8 *)owner->list + 0xFA) << 22) >> 27;
    return 1;
}

NONMATCH("asm/code_080EDB40/reset_host_entry.s", u32 reset_host_entry(struct OwnerObj *owner, void *unused, u32 *args))
{
    u32 idx = args[0];
    struct ObjList *list = owner->list;
    struct Entity *entry = *(struct Entity **)((u8 *)list + 0x28 + (idx << 2));

    obj_set_state(entry, 0);
    obj_load_record(entry->sub294, entry->sub294->unk21, args[1], 0, 0);
    ((struct Flags8 *)&entry->unk214)->bit7 = 0;
    ((struct Flags8 *)&entry->unk215)->bit1 = 0;
    ((struct SubFlags *)&entry->sub294->unk12)->mid = 0;
    {
        u32 old = entry->sub294->unk11;
        u32 bit = (old << 25) >> 31;
        bit = args[2] ^ bit;
        set_u8_bit6(&entry->sub294->unk11, bit);
    }
    return 1;
}
END_NONMATCH

u32 set_obj_via_ptr(struct OwnerObj *owner, struct Entity *obj, struct Record *dst, struct ObjArgs *args)
{
    struct MethodEntry *m;
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, args->unkC);
    if (args->unkC != 0)
    {
        obj->unk21C = obj->unk218;
        {
            u32 v = obj->unk215;
            v = v | 1;
            obj->unk215 = v;
        }
    }
    m = (struct MethodEntry *)(obj->unk338 + 0x90);
    m->fn((u8 *)obj + m->offset, args->unk0);
    obj_set_cell_coord(obj, args->unk4, 0);
    dst->unkA0 = dst->unkA0 | 0x200;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, dst, obj->sub294, *(u8 *)&args->unkC, args->unk8);
}

struct ListObjArgs
{
    u32 index;
    u8 unk4;
    u8 filler5[3];
    u32 unk8;
    u8 unkC;
    u8 filler[3];
    u32 unk10;
};

u32 set_list_obj_state(struct OwnerObj *owner, void *unused1, struct ListObjArgs *args)
{
    struct MethodEntry *m;
    u32 index = args->index;
    struct Entity *obj = *(struct Entity **)((u8 *)owner->list + index * 4 + 0x28);
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, 0);
    if (args->unk10 != 0)
    {
        obj->unk21C = obj->unk218;
        {
            u32 v = obj->unk215;
            v = v | 1;
            obj->unk215 = v;
        }
    }
    m = (struct MethodEntry *)(obj->unk338 + 0x90);
    m->fn((u8 *)obj + m->offset, args->unk4);
    obj_set_cell_coord(obj, args->unk8, 0);
    obj->unkF8 = obj->unkF8 | 0x200;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, (u8 *)obj + 0x58, obj->sub294, *(u8 *)&args->unk10, args->unkC);
}

struct Rec8
{
    u16 head;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
};

extern u32 gUnk_03000FF8;
extern const u32 gRecordPtrTable[];
void decode_coord_mode(u32 row, u32 col, s32 x, u8 *out_a, u8 *out_b);

u32 set_record_from_args(struct OwnerObj *owner, void *unused, u32 *args)
{
    u8 mode_a;
    u8 mode_b;
    u32 *slot = &gUnk_03000FF8;
    u32 index = *args++;
    struct Rec8 *rec = (struct Rec8 *)(*slot + (index << 3));
    u16 sel = *(u16 *)((u8 *)owner->list + 0x126);
    u8 *block = (u8 *)*(u32 *)((u8 *)gRecordPtrTable + (sel << 2));
    u8 *table = block - ((block[3] << 8) | block[2]);
    u32 entry = *args++;
    rec->head = ((u16 *)table)[entry];
    mode_a = *args++;
    decode_coord_mode(((u32)rec->head >> 12) - 1, rec->head & 0xFFF, mode_a, &mode_a, &mode_b);
    rec->b2 = mode_a;
    rec->b3 = *args++;
    rec->b4 = *args++;
    ((struct Bits1 *)&rec->b5)->bit0 = (*args ^ mode_b) & 1;
    ((struct Bits1 *)&rec->b5)->bit1 = 0;
    return 1;
}

ASM_FUNC("asm/code_080EDB40/set_effect_key.s", void set_effect_key(void))

ASM_FUNC("asm/code_080EDB40/cmd_test_and_store.s", void cmd_test_and_store(void))

u32 cmd_move_toward(struct OwnerObj *owner, struct Entity *ent, u32 arg, u32 *rec)
{
    s32 step;
    s32 p7;
    s32 p6;
    u32 f10;
    u32 f9;
    u32 f38;
    s32 mode;
    u8 moved;

    if ((((s32)rec[0] >> 4) & 1) != 0)
    {
        rec[2] = rec[2] << 4;
        rec[3] = rec[3] << 4;
    }
    mode = rec[1];
    switch (mode)
    {
    case 0:
        if (((((u8)(rec[0] >> 1)) ^ 1) & 1) != 0)
            step = ent->unkC / 0x100;
        else
            step = 0;
        p7 = rec[2];
        p6 = rec[3];
        f10 = 1;
        f9 = 0;
        f38 = 0;
        break;
    case 1:
        step = rec[2];
        if (((((u8)(rec[0] >> 1)) ^ 1) & 1) != 0)
            p7 = ent->unk10 / 0x100;
        else
            p7 = 0;
        p6 = rec[3];
        f10 = 0;
        f9 = 1;
        f38 = 0;
        break;
    case 2:
        step = rec[2];
        p7 = rec[3];
        if (((((u8)(rec[0] >> 1)) ^ 1) & 1) != 0)
            p6 = (ent->unk14 + ent->unk18) / 0x100;
        else
            p6 = 0;
        f10 = 0;
        f9 = 0;
        f38 = 1;
        break;
    }
    if ((((s32)rec[0] >> 2) & 3) == 0)
    {
        if ((rec[5] & 0x40) != 0)
            rec[5] = (rec[5] & 0xF) << 4;
        else
            rec[5] = rec[5] << 8;
        if (rec[5] != 0)
            set_half_23e(ent, (s16)rec[5]);
    }
    moved = cell_motion_obj_move_toward_target(ent, (((s32)rec[0] >> 2) & 3) + 3, ((s32)rec[0] >> 1) & 1, step, p7, p6,
        f10 << 8, f9 << 8, f38 << 8, 0x100, 0x100, 0x100, *(u16 *)((u8 *)rec + 0x10),
        *(u16 *)((u8 *)rec + 0x14), (rec[0] & 1) ? 1 : -1, -1);
    if (moved != 0)
    {
        set_flag_a0_2(owner, ent, (u8 *)arg, 0);
        return 0;
    }
    return 1;
}

void set_half_23e(void *obj, s16 v);
u32 cmd_move_slot(struct OwnerObj *owner, struct Entity *ent, u32 arg, u32 *rec)
{
    struct Entity *target;
    s32 ax;
    s32 bx;
    s32 cx;
    u32 f9;
    u32 f8;
    u32 f38;
    s32 mode;
    u8 moved;

    target = *(struct Entity **)((u8 *)owner->list + rec[2] * 4 + 0x28);
    mode = rec[1];
    switch (mode)
    {
    case 0:
        ax = ent->unkC / 0x100;
        bx = target->unk10 / 0x100;
        cx = (target->unk14 + target->unk18) / 0x100;
        f9 = 1;
        f8 = 0;
        f38 = 0;
        break;
    case 1:
        ax = target->unkC / 0x100;
        bx = ent->unk10 / 0x100;
        cx = (target->unk14 + target->unk18) / 0x100;
        f9 = 0;
        f8 = 1;
        f38 = 0;
        break;
    case 2:
        ax = target->unkC / 0x100;
        bx = target->unk10 / 0x100;
        cx = (ent->unk14 + ent->unk18) / 0x100;
        f9 = 0;
        f8 = 0;
        f38 = 1;
        break;
    }
    if ((((s32)rec[0] >> 1) & 3) == 0)
    {
        if ((rec[4] & 0x40) != 0)
            rec[4] = (rec[4] & 0xF) << 4;
        else
            rec[4] = rec[4] << 8;
        if (rec[4] != 0)
            set_half_23e(ent, *(s16 *)((u8 *)rec + 8));
    }
    moved = cell_motion_obj_move_toward_target(ent, (((s32)rec[0] >> 1) & 3) + 3, 0, ax, bx, cx,
        f9 << 8, f8 << 8, f38 << 8, 0x100, 0x100, 0x100, *(u16 *)((u8 *)rec + 0xC),
        *(u16 *)((u8 *)rec + 0x10), (rec[0] & 1) ? 1 : -1, rec[2]);
    if (moved != 0)
    {
        set_flag_a0_2(owner, ent, (u8 *)arg, 0);
        return 0;
    }
    return 1;
}

u32 apply_stage_command(struct OwnerObj *owner, struct Entity *obj, struct Record *rec, u32 *cmds)
{
    u32 stage;
    struct Entity *target;
    if ((rec->unkA0 & 2) != 0)
    {
        obj_finish_move_step(obj);
        rec->unkA0 = rec->unkA0 & 0xFFFD;
    }
    if ((rec->unkA0 & 0x40) != 0)
    {
        obj_stop_motion(obj);
        rec->unkA0 = rec->unkA0 & 0xFFBF;
    }
    switch ((s32)*cmds++)
    {
    case 0:
        stage = *(u8 *)cmds;
        break;
    case 1:
        stage = ((((u32)obj->unk24 << 29) >> 29) + *cmds) & 7;
        break;
    case 2:
        if (*cmds == -1)
            *cmds = ((u32)owner->list->unk208 << 29) >> 31;
        target = *(struct Entity **)((u8 *)owner->list + *cmds * 4 + 0x28);
        stage = (u8)direction_octant((u8 *)owner + 0x1C, obj->unkC, obj->unk10, target->unkC, target->unk10);
        break;
    case 3:
        stage = (u8)rand_below(8, mt19937_next());
        break;
    }
    obj_set_stage(obj, stage);
    return 1;
}

ASM_FUNC("asm/code_080EDB40/set_entity_box.s", void set_entity_box(void))

ASM_FUNC("asm/code_080EDB40/step_entity_track.s", void step_entity_track(void))

u32 set_quad_coords(void *unused0, struct Entity *entity, void *unused2, u32 *cmds)
{
    struct Entity *target;
    if ((*cmds++ & 2) != 0)
        target = entity->link344;
    else
        target = entity;
    target->unk146[0] = *cmds++;
    target->unk146[1] = *cmds++;
    target->unk146[2] = *cmds++;
    target->unk146[3] = *cmds++;
    target->unk146[4] = *cmds++;
    target->unk146[5] = *cmds++;
    target->unk146[6] = *cmds++;
    target->unk146[7] = *cmds;
    return 1;
}

ASM_FUNC("asm/code_080EDB40/set_entity_field.s", void set_entity_field(void))

u32 set_box_fields(void *unused0, struct Entity *entity, void *unused2, u32 *cmds)
{
    struct Entity *target;
    u8 *base;
    u8 *hi;
    u32 *p = cmds;
    u32 v;
    u32 w;

    if (*p++ != 0)
        target = entity->link344;
    else
        target = entity;
    base = (u8 *)target + 0x100;
    hi = base;
    switch (*p++)
    {
    case 0:
        v = *p;
        if (v & 0x40)
            *(u16 *)(base + 0x8) = v & 0xF;
        else
            *(s16 *)(base + 0x8) = v << 4;
        p++;
        v = *p;
        if (v & 0x40)
            *(u16 *)(base + 0xA) = v & 0xF;
        else
            *(s16 *)(base + 0xA) = v << 4;
        p++;
        v = *p;
        if (v & 0x40)
            *(u16 *)(base + 0xC) = v & 0xF;
        else
            *(s16 *)(base + 0xC) = v << 4;
        v = p[1];
        if (v & 0x40)
            *(u16 *)(base + 0xE) = v & 0xF;
        else
            *(s16 *)(base + 0xE) = v << 4;
        break;
    case 1:
        v = *p;
        if (v & 0x40)
            *(u16 *)(base + 0x0) = (v & 0xF) << 4;
        else
            *(u16 *)(base + 0x0) = v << 8;
        p++;
        v = *p;
        if (v & 0x40)
            *(u16 *)(base + 0x2) = (v & 0xF) << 4;
        else
            *(u16 *)(base + 0x2) = v << 8;
        p++;
        v = *p;
        if (v & 0x40)
            *(u16 *)(base + 0x4) = (v & 0xF) << 4;
        else
            *(u16 *)(base + 0x4) = v << 8;
        v = p[1];
        if (v & 0x40)
            *(u16 *)(base + 0x6) = (v & 0xF) << 4;
        else
            *(u16 *)(base + 0x6) = v << 8;
        break;
    case 2:
        *(u16 *)(base + 0x28) = *p++;
        *(u16 *)(base + 0x2A) = *p++;
        *(u16 *)(base + 0x2C) = *p++;
        *(u16 *)(base + 0x2E) = *p;
        break;
    case 3:
        p[0] <<= 4;
        p[1] <<= 4;
        p[2] <<= 4;
        p[3] <<= 4;
        /* fall through */
    case 4:
        *(u16 *)(base + 0x20) = *p++;
        *(u16 *)(base + 0x22) = *p++;
        *(u16 *)(base + 0x24) = *p++;
        *(u16 *)(base + 0x26) = *p;
        break;
    case 5:
        *(u16 *)(base + 0x10) = *p++;
        *(u16 *)(base + 0x12) = *p++;
        *(u16 *)(base + 0x14) = *p++;
        *(u16 *)(base + 0x16) = *p;
        break;
    case 6:
        *(u16 *)(base + 0x18) = *p++;
        *(u16 *)(base + 0x1A) = *p++;
        *(u16 *)(base + 0x1C) = *p++;
        *(u16 *)(base + 0x1E) = *p;
        break;
    case 7:
        *(u16 *)(hi + 0x3E) = *p++;
        v = *p++;
        *(u16 *)(hi + 0x40) = v;
        w = *p++;
        *(u16 *)(hi + 0x42) = w;
        v = *p;
        *(u16 *)(hi + 0x44) = v;
        break;
    case 8:
        *(u16 *)(hi + 0x36) = *p++;
        *(u16 *)(hi + 0x38) = *p++;
        *(u16 *)(hi + 0x3A) = *p++;
        *(u16 *)(hi + 0x3C) = *p;
        break;
    case 9:
        *(u16 *)(base + 0x8) = *p++;
        *(u16 *)(base + 0xA) = *p++;
        *(u16 *)(base + 0xC) = *p++;
        *(u16 *)(base + 0xE) = *p;
        break;
    case 10:
        *(u16 *)(base + 0x0) = *p++;
        *(u16 *)(base + 0x2) = *p++;
        *(u16 *)(base + 0x4) = *p++;
        *(u16 *)(base + 0x6) = *p;
        break;
    }
    return 1;
}

u32 set_entity_mode(void *unused0, struct Entity *entity, void *unused2, u32 *cmds)
{
    struct Record *rec = (struct Record *)((u8 *)entity + 0x58);
    u32 flags = entity->unk2B6 | 0x80;
    entity->unk2B6 = flags;
    switch (cmds[0])
    {
    case 0:
        flags = flags | 0x40;
        entity->unk2B6 = flags;
        break;
    case 1:
        flags = flags & -0x41;
        entity->unk2B6 = flags;
        break;
    }
    if ((s32)cmds[0] <= 0x33)
    {
        rec->unk140 = gEntityModeDataTable[cmds[0]];
        rec->unkA0 = rec->unkA0 | 0x400;
        ((struct Bits1 *)&rec->unk17E)->bit1 = 0;
        ((struct Bits1 *)&rec->unk17E)->bit0 = 0;
    }
    else
    {
        rec->unkA0 = rec->unkA0 & 0xFBFF;
    }
    return 1;
}

u32 set_entity_mode_and_link(void *unused0, struct Entity *entity, void *unused2, u32 *cmds)
{
    struct Record *rec = (struct Record *)((u8 *)entity + 0x58);
    u32 flags = entity->unk2B6 | 0x80;
    entity->unk2B6 = flags;
    switch (cmds[0])
    {
    case 0:
        flags = flags | 0x40;
        entity->unk2B6 = flags;
        break;
    case 1:
        flags = flags & -0x41;
        entity->unk2B6 = flags;
        break;
    }
    if ((s32)cmds[0] <= 0x33)
    {
        rec->unk140 = gEntityModeDataTable[cmds[0]];
        rec->unkA0 = rec->unkA0 | 0x400;
        ((struct Bits1 *)&rec->unk17E)->bit1 = 0;
        ((struct Bits1 *)&rec->unk17E)->bit0 = 0;
    }
    else
    {
        rec->unkA0 = rec->unkA0 & 0xFBFF;
    }
    cmds++;
    rec->unk158 = *cmds++;
    rec->unk15C = *cmds;
    if (entity->link344 != 0)
    {
        entity->link344->unk1B0 = rec->unk158;
        entity->link344->unk1B4 = rec->unk15C;
    }
    return 1;
}

void entity_camera_offset(void *list, u32 mode, s32 *x, s32 *y);
void sprites_obj_set_move_target(void *list, u32 type, u32 one, s32 x, s32 y, s32 z);

u32 start_track_move(struct OwnerObj *owner, struct Record *rec, struct Track *track)
{
    s32 cx;
    s32 cy;
    if ((track->flags & 1) != 0)
    {
        track->x = track->x << 4;
        track->y = track->y << 4;
    }
    if (((track->flags >> 1) & 3) == 1)
    {
        u32 z = track->z;
        u32 v;
        if ((z & 0x40) != 0)
            v = (z & 0xF) << 4;
        else
            v = z << 8;
        track->z = v;
    }
    entity_camera_offset(owner->list, track->mode, &cx, &cy);
    sprites_obj_set_move_target(owner->list, (track->flags >> 1) & 3, 1,
        cx + track->x - 0x78, cy + track->y - 0x70, *(s16 *)&track->z);
    rec->unkA0 = rec->unkA0 | 0x100;
    return 0;
}

NONMATCH("asm/code_080EDB40/set_motion_slot.s", u32 set_motion_slot(void *unused0, struct Entity *obj, void *unused2, u32 *cmds))
{
    u32 *p = cmds;
    u32 k = *p++;
    struct MotionSlot *slot = (struct MotionSlot *)(gEffectState + ((k << 5) + 0x2A0));
    s32 old = gEffectState[0x55B];
    u32 bits = (((u32)old << 27) >> 28) | (1 << k);

    {
        s32 mask = -0x1F;
        gEffectState[0x55B] = (mask & old) | ((bits & 0xF) << 1);
    }

    p[1] = -p[1];
    p[2] = -p[2];
    if ((p[0] & 1) != 0)
    {
        p[1] = p[1] << 4;
        p[2] = p[2] << 4;
    }
    if (((s32)p[0] >> 1 & 3) == 1)
    {
        s32 magnitude = ((s32 (*)(s32))gMathRoutines)((p[1] * p[1] + p[2] * p[2]) << 4);
        if ((p[3] & 0x40) != 0)
            p[3] = (p[3] & 0xF) << 2;
        else
            p[3] = p[3] << 6;
        slot->unk14 = (s32)((p[3] * p[1]) << 4) / magnitude;
        slot->unk18 = (s32)((p[3] * p[2]) << 4) / magnitude;
    }
    else
    {
        slot->unk14 = (s32)(p[1] << 8) / (s32)p[3];
        slot->unk18 = (s32)(p[2] << 8) / (s32)p[3];
    }
    slot->unk0[3] = 0;
    slot->unk0[2] = 0;
    slot->unk0[1] = 0;
    slot->unk0[0] = 0;
    if (obj != 0)
        slot->unk1C = obj->unk54 + 4;
    else
        slot->unk1C = gEffectState[0x548];
    slot->unk10 = p[1];
    slot->unk12 = p[2];
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080EDB40/set_effect_flags.s", void set_effect_flags(void))

ASM_FUNC("asm/code_080EDB40/place_list_text.s", void place_list_text(void))
