#include "global.h"

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
    u8 filler20D[0x20F - 0x20D];
    u8 unk20F;
    u8 filler210[0x214 - 0x210];
    u8 unk214;
    u8 unk215;
    u8 filler216[0x218 - 0x216];
    u16 unk218;
    u8 filler21A[0x21C - 0x21A];
    u16 unk21C;
    u8 filler21E[0x294 - 0x21E];
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

void obj_set_state(void *obj, u32 state);
void obj_finish_move_step(void *obj);
void obj_stop_motion(void *obj);
void obj_set_stage(void *obj, u32 stage);
u32 direction_octant(void *root, s32 x0, s32 y0, s32 x1, s32 y1);
u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 seed);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_sync_sub_slot(void *obj, u32 value);
void obj_set_cell_coord(void *obj, u32 cell, u32 arg);
u32 flags_vars_set_obj_state(void *root, void *dst, void *obj, u32 state, u32 zero);

ASM_FUNC("asm/code_080EDB40/command_on_list_entry.s", void command_on_list_entry(void))

ASM_FUNC("asm/code_080EDB40/set_obj_flags_cmd.s", void set_obj_flags_cmd(void))

extern u8 *gEffectState;
void set_flag_f8_40(void *host);
void camera_track_entity(void *list, u32 field, u32 zero);

ASM_FUNC("asm/code_080EDB40/set_host_flags.s", void set_host_flags(void))

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
        s32 flags = entry->sub294->unk11;
        s32 bit = ((u32)flags << 25) >> 31;

        bit ^= args[2];
        bit &= 1;
        bit <<= 6;
        flags = (-0x41 & flags) | bit;
        entry->sub294->unk11 = flags;
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

ASM_FUNC("asm/code_080EDB40/set_record_from_args.s", void set_record_from_args(void))

ASM_FUNC("asm/code_080EDB40/set_effect_key.s", void set_effect_key(void))

ASM_FUNC("asm/code_080EDB40/cmd_test_and_store.s", void cmd_test_and_store(void))

ASM_FUNC("asm/code_080EDB40/cmd_move_toward.s", void cmd_move_toward(void))

ASM_FUNC("asm/code_080EDB40/cmd_move_slot.s", void cmd_move_slot(void))

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
        stage = (u8)sub_081DE850(8, mt19937_next());
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

ASM_FUNC("asm/code_080EDB40/set_box_fields.s", void set_box_fields(void))

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

ASM_FUNC("asm/code_080EDB40/set_motion_slot.s", void set_motion_slot(void))

ASM_FUNC("asm/code_080EDB40/set_effect_flags.s", void set_effect_flags(void))

ASM_FUNC("asm/code_080EDB40/place_list_text.s", void place_list_text(void))
