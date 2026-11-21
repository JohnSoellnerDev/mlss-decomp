#include "global.h"
#include "ram_routines.h"

struct Entity
{
    u8 filler0[0xC];
    s32 unkC;  /* x, 24.8 fixed point */
    s32 unk10; /* y, 24.8 fixed point */
    s32 unk14; /* z = unk14 + unk18, 24.8 fixed point */
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

/* View of the halfword parameter groups at Entity+0x100; fields are named by their Entity offset. */
struct EntityParams
{
    u16 unk100[4]; /* 8.8 fixed point */
    u16 unk108[4]; /* 12.4 fixed point */
    u16 unk110[4];
    u16 unk118[4];
    u16 unk120[4];
    u16 unk128[4];
    u8 filler130[0x136 - 0x130];
    u16 unk136[4];
    u16 unk13E[4];
};

/* Script arguments for a fixed-point group: either a whole number, or PARAM_FRACTION | n for n/16. */
#define PARAM_FRACTION 0x40
#define PARAM_FRACTION_MASK 0xF

struct MotionBody
{
    u32 flags;
    s32 x;
    s32 y;
    u32 z;
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

/* Record.unkA0 flags */
#define REC_MOVE_RUNNING   0x02 /* a move started by a script command is running (set_flag_a0_2) */
#define REC_MOTION_RUNNING 0x40 /* a motion is running (obj_stop_motion ends it) */
#define REC_MODE_SET       0x400 /* unk140 holds a mode from gEntityModeDataTable (set_entity_mode) */
#define REC_ANIM_WAIT      0x20  /* the script waits for the animation to end (flags_vars_set_obj_state) */
#define REC_ANIM_THEN_IDLE 0x200 /* when that animation ends, the entity goes back to state 0 (update_obj_sub_state) */

/* Arguments of set_obj_via_ptr's script command (ListObjArgs without the slot). */
struct ObjStateArgs
{
    u8 methodArg;  /* passed to the entity's virtual method at vtable + 0x90 */
    u8 filler1[3];
    u32 cell;      /* frame index for obj_set_cell_coord */
    u8 flip;       /* XORed into the sprite's flip bit */
    u8 filler9[3];
    u32 wait;      /* nonzero: play the animation once and wait for it to end */
};

#define ENTITY_MODE_COUNT 0x34 /* entries of gEntityModeDataTable */

/* ObjList.unk208 bit 2: which of the two heroes (list entries 0 and 1) leads. */
#define LIST_LEAD_HERO(list) (((u32)(list)->unk208 << 29) >> 31)

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

struct SubBits11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Lo2Bits
{
    u8 lo2 : 2;
    u8 rest : 6;
};

struct Mid5Half
{
    u16 lo5 : 5;
    u16 mid5 : 5;
    u16 rest : 6;
};

struct Mid4Bits
{
    u8 lo1 : 1;
    u8 mid4 : 4;
    u8 rest : 3;
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

struct Bits24
{
    u16 lo3 : 3;
    u16 mid4 : 4;
    u16 hi2 : 2;
    u16 rest : 7;
};

struct SignedId7
{
    s8 id : 7;
    u8 top : 1;
};

struct Mid3Bits
{
    u8 lo1 : 1;
    u8 mid3 : 3;
    u8 rest : 4;
};

struct SubBits11b
{
    u8 filler0[0x11];
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 rest : 4;
};

struct Flags20C
{
    u8 bit0 : 1;
    u8 mode : 3;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

struct Flags20D
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
} __attribute__((packed));

struct Sub11Bits
{
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 rest : 4;
} __attribute__((packed));

struct EntityFlagBits
{
    u8 filler0[0x20C];
    u32 lo4_20C : 4;
    u32 bit4_20C : 1;
    u32 bit5_20C : 1;
    u32 bit6_20C : 1;
    u32 bit7_20C : 1;
    u32 bit0_20D : 1;
    u32 bit1_20D : 1;
    u32 rest_20D : 6;
    u32 lo2_20E : 2;
    u32 bit2_20E : 1;
    u32 rest_20E : 13;
    u8 filler210[0x33D - 0x210];
    u8 lo4_33D : 4;
    u8 hi4_33D : 4;
};

struct CmdTestArgs
{
    u32 want : 1;
    s32 kind : 31;
    u32 value;
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
void set_half_23e(void *obj, s32 v);
/* Starts a move of obj to (x, y, z) in pixels (relative to its position if `relative`). mode is
   MOTION_BY_SPEED or MOTION_BY_TIME. skip_* = 0x100 leaves an axis out of the distance used for the
   speed; speed 0 uses the entity's speed at 0x23E. target_slot is -1 or an entity of the owner's list.
   Returns 0 if obj is already there, 1 if a move was started. */
u8 cell_motion_obj_move_toward_target(void *obj, u32 mode, u32 relative, s32 x, s32 y, s32 z, u32 skip_x, u32 skip_y, u32 skip_z, u16 scale_x, u16 scale_y, u16 scale_z, u16 unk12, u16 speed, s32 sign, u32 target_slot);
#define MOTION_BY_SPEED 3
#define MOTION_BY_TIME 4
/* Sets bit 1 of the script record's flags at 0xA0: a move is running (apply_stage_command finishes it). */
void set_flag_a0_2(void *a, void *b, u8 *p, u32 c);
extern u8 *gEffectState;
extern const u32 gRecordPtrTable[];
void obj_set_sprite_link(struct Entity *ent, u32 link);

/* Script command on one entity of the owner's list: args are (slot, op, value).
   op 0: set the entity's sprite link to value.
   op 1: set flag bit 2 at 0x20E.   op 2: set flag bit 4 at 0x20C.
   op 3: value != 0 loads the five flags 0x20C bits 5-7 / 0x20D bits 0-1 from the entity's 0x14-byte
         record (bytes 0x10 and 0x11), value == 0 clears them. Entities 0 and 1 instead get their bit in
         gEffectState[0x55A] bits 6-7 set and 0x20C bits 5-7 cleared.
   op 4: set (value != 0) or clear bit 4 of byte 0x33D.
   Always returns 1. */
u32 command_on_list_entry(struct OwnerObj *owner, void *unused, u32 *args)
{
    u32 slot;
    struct Entity *ent;

    slot = *args++;
    ent = *(struct Entity **)((u8 *)owner->list + slot * 4 + 0x28);
    switch (*args++)
    {
    case 0:
        obj_set_sprite_link(ent, *args);
        break;
    case 1:
        ((struct EntityFlagBits *)ent)->bit2_20E = *args;
        break;
    case 2:
        ((struct EntityFlagBits *)ent)->bit4_20C = *args;
        break;
    case 3:
    {
        u16 record_set = *(u16 *)((u8 *)owner->list + 0x126);
        u8 *records = (u8 *)*(u32 *)((u8 *)gRecordPtrTable + (record_set << 2));
        u8 *rec;

        /* the block's bytes 6-7 hold how far back its record table starts */
        records -= (records[7] << 8) | records[6];
        rec = records + ent->unk54 * 0x14;
        if (ent->unk54 <= 1)
        {
            u8 *marks = gEffectState + 0x55A;
            u32 top_bits = *marks >> 6;

            top_bits |= 1 << ent->unk54;
            top_bits <<= 6;
            *marks = (*marks & 0x3F) | top_bits;
            ((struct EntityFlagBits *)ent)->bit5_20C = 0;
            ((struct EntityFlagBits *)ent)->bit6_20C = 0;
            ((struct EntityFlagBits *)ent)->bit7_20C = 0;
        }
        else
        {
            ((struct EntityFlagBits *)ent)->bit5_20C = *args ? (rec[0x10] >> 5) & 1 : 0;
            ((struct EntityFlagBits *)ent)->bit6_20C = *args ? (rec[0x10] >> 6) & 1 : 0;
            ((struct EntityFlagBits *)ent)->bit7_20C = *args ? (rec[0x10] >> 7) & 1 : 0;
        }
        ((struct EntityFlagBits *)ent)->bit0_20D = *args ? rec[0x11] & 1 : 0;
        ((struct EntityFlagBits *)ent)->bit1_20D = *args ? (rec[0x11] >> 1) & 1 : 0;
        break;
    }
    case 4:
        if (*args)
        {
            u8 *flags = (u8 *)ent + 0x33D;
            u32 high_nibble = *flags >> 4;

            high_nibble |= 1;
            high_nibble <<= 4;
            *flags = (*flags & 0xF) | high_nibble;
        }
        else
        {
            u8 *flags = (u8 *)ent + 0x33D;
            u32 high_nibble = *flags >> 4;

            high_nibble &= ~1;
            high_nibble <<= 4;
            *flags = (*flags & 0xF) | high_nibble;
        }
        break;
    }
    return 1;
}

/* Script command: args are (op, slot, value). The slot picks an entity of the owner's list.
   op 0: set the entity's 8.8 speed (set_half_23e) to a whole number or, with PARAM_FRACTION,
         to n/16; a slot with bit 0x40 set means the list's current leader (0x208 bit 2).
   op 1: set the 2-bit field at sub294+0x11 bits 2-3.   op 2: set the 3-bit mode at 0x20C.
   op 3: set the halfword at 0x270.
   op 4: value bit 15 is the new state of the flags picked by bits 4..0 (0x20C bits 5-7,
         0x20D bits 0-1); entities 0 and 1 also mark their bit in gEffectState[0x55A] bits 6-7.
   Always returns 1. */
u32 set_obj_flags_cmd(struct OwnerObj *owner, void *unused, u32 *args)
{
    struct Entity *ent;
    u32 slot;

    switch (*args++)
    {
    case 0:
        if (args[0] & 0x40)
            ent = *(struct Entity **)((u8 *)owner->list + ((struct Flags20D *)&owner->list->unk208)->bit2 * 4 + 0x28);
        else
            ent = *(struct Entity **)((u8 *)owner->list + args[0] * 4 + 0x28);
        {
            s32 speed = args[1];
            if ((args[1] & PARAM_FRACTION) != 0)
            {
                speed &= PARAM_FRACTION_MASK;
                speed <<= 4;
            }
            else
            {
                /* signed whole number in the low byte, as 8.8 */
                speed <<= 24;
                speed >>= 16;
            }
            set_half_23e(ent, speed);
        }
        break;
    case 1:
        slot = *args++;
        ent = *(struct Entity **)((u8 *)owner->list + slot * 4 + 0x28);
        {
            struct SubObj *sub = ent->sub294;
            s32 value = args[0];
            ((struct Sub11Bits *)&sub->unk11)->mid2 = value & 3;
        }
        break;
    case 2:
        slot = *args++;
        ent = *(struct Entity **)((u8 *)owner->list + slot * 4 + 0x28);
        {
            s32 value = args[0];
            ((struct Flags20C *)((u8 *)ent + 0x20C))->mode = value & 7;
        }
        break;
    case 3:
        slot = *args++;
        ent = *(struct Entity **)((u8 *)owner->list + slot * 4 + 0x28);
        ent->unk270 = args[0];
        break;
    case 4:
        slot = *args++;
        ent = *(struct Entity **)((u8 *)owner->list + slot * 4 + 0x28);
        {
            u32 on = ((s32)args[0] >> 15) & 1;
            if (ent->unk54 <= 1)
            {
                u8 *marks = gEffectState + 0x55A;
                u32 topBits = *marks >> 6;
                topBits |= 1 << ent->unk54;
                topBits <<= 6;
                *marks = (*marks & 0x3F) | topBits;
            }
            if (((s32)args[0] >> 4) & 1)
                ((struct Flags20C *)&ent->unk20C)->bit5 = on;
            if (((s32)args[0] >> 3) & 1)
                ((struct Flags20C *)&ent->unk20C)->bit6 = on;
            if (((s32)args[0] >> 2) & 1)
                ((struct Flags20C *)&ent->unk20C)->bit7 = on;
            if (((s32)args[0] >> 1) & 1)
                ((struct Flags20D *)&ent->unk20D)->bit0 = on;
            if (args[0] & 1)
                ((struct Flags20D *)&ent->unk20D)->bit1 = on;
        }
        break;
    }
    return 1;
}

extern u8 *gEffectState;
void set_flag_f8_40(void *host);
void camera_track_entity(void *list, u32 field, u32 zero);

/* Scene (owner->list) camera fields, read by offset. */
#define SCENE_CAMERA_FLAGS 0xF8  /* u8: bit 6 (SCENE_CAMERA_FOLLOW) makes the camera follow an entity */
#define SCENE_CAMERA_FOLLOW 0x40
#define SCENE_CAMERA_TARGET 0xFA /* u16: bits 5-9 are the index of the entity the camera follows */
#define SCENE_LEAD_FLAGS 0x208   /* u8: bit 2 is the index of the leading brother */

/* Camera-target argument meaning "follow the leading brother". */
#define CAMERA_TARGET_LEAD 0xFF

static inline void set_bit6_f8(void *host, u32 v)
{
    ((struct Flags8 *)((u8 *)host + 0xF8))->bit6 = v;
}

/* Script command: chooses the entity the camera follows. args[0] is the entity index (the camera
 * then follows it), or CAMERA_TARGET_LEAD for the leading brother, with args[1] saying whether
 * the camera follows. If it follows, the camera moves to the target now (camera_track_entity).
 * The index is also kept in gEffectState + 0x530. */
u32 set_host_flags(struct OwnerObj *owner, void *unused1, u32 *args)
{
    if (args[0] != CAMERA_TARGET_LEAD)
    {
        ((struct Mid5Half *)((u8 *)owner->list + SCENE_CAMERA_TARGET))->mid5 = args[0];
        set_flag_f8_40(owner->list);
    }
    else
    {
        u32 leadIndex = ((u32)*(u8 *)((u8 *)owner->list + SCENE_LEAD_FLAGS) << 29) >> 31;
        ((struct Mid5Half *)((u8 *)owner->list + SCENE_CAMERA_TARGET))->mid5 = leadIndex;
        set_bit6_f8(owner->list, args[1]);
    }
    if ((*(u8 *)((u8 *)owner->list + SCENE_CAMERA_FLAGS) & SCENE_CAMERA_FOLLOW) != 0)
        camera_track_entity(owner->list, ((u32)*(u16 *)((u8 *)owner->list + SCENE_CAMERA_TARGET) << 22) >> 27, 0);
    *(u32 *)(gEffectState + 0x530) = ((u32)*(u16 *)((u8 *)owner->list + SCENE_CAMERA_TARGET) << 22) >> 27;
    return 1;
}

u32 reset_host_entry(struct OwnerObj *owner, void *unused, u32 *args)
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
        u32 bit = ((struct SubBits11 *)entry->sub294)->bit6;
        bit = bit ^ args[2];
        ((struct SubBits11 *)entry->sub294)->bit6 = bit;
    }
    return 1;
}

/* Script command, the running entity's own version of set_list_obj_state: puts obj into state 1,
   syncs its sprite slot, calls its virtual method 0x90 with methodArg and shows frame `cell`.
   With `wait` set, the entity's id218 is saved in unk21C first (flags215 bit 0) and the animation
   plays once while the script waits; the entity then goes back to state 0 (REC_ANIM_THEN_IDLE).
   Returns 1 when the command is done at once. */
u32 set_obj_via_ptr(struct OwnerObj *owner, struct Entity *obj, struct Record *rec, struct ObjStateArgs *args)
{
    struct MethodEntry *method;
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, args->wait);
    if (args->wait != 0)
    {
        obj->unk21C = obj->unk218;
        {
            u32 flags = obj->unk215;
            flags = flags | 1;
            obj->unk215 = flags;
        }
    }
    method = (struct MethodEntry *)(obj->unk338 + 0x90);
    method->fn((u8 *)obj + method->offset, args->methodArg);
    obj_set_cell_coord(obj, args->cell, 0);
    rec->unkA0 = rec->unkA0 | REC_ANIM_THEN_IDLE;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, rec, obj->sub294, *(u8 *)&args->wait, args->flip);
}

/* Arguments of set_list_obj_state's script command. */
struct ListObjArgs
{
    u32 slot;      /* entity of the owner's list */
    u8 methodArg;  /* passed to the entity's virtual method at vtable + 0x90 */
    u8 filler5[3];
    u32 cell;      /* frame index for obj_set_cell_coord */
    u8 flip;       /* XORed into the sprite's flip bit */
    u8 filler[3];
    u32 wait;      /* nonzero: play the animation once and wait for it to end */
};

/* Script command: puts entity `slot` of the owner's list into state 1, saves its current
   animation in slot 0, calls its virtual method 0x90 with methodArg and shows frame `cell`.
   With `wait` set, the entity's id218 is saved in unk21C first (flags215 bit 0) and the
   animation plays once while the script waits; returns 1 when the command is done at once. */
u32 set_list_obj_state(struct OwnerObj *owner, void *unused1, struct ListObjArgs *args)
{
    struct MethodEntry *method;
    u32 slot = args->slot;
    struct Entity *obj = *(struct Entity **)((u8 *)owner->list + slot * 4 + 0x28);
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, 0);
    if (args->wait != 0)
    {
        obj->unk21C = obj->unk218;
        {
            u32 flags = obj->unk215;
            flags = flags | 1;
            obj->unk215 = flags;
        }
    }
    method = (struct MethodEntry *)(obj->unk338 + 0x90);
    method->fn((u8 *)obj + method->offset, args->methodArg);
    obj_set_cell_coord(obj, args->cell, 0);
    obj->unkF8 = obj->unkF8 | 0x200; /* flags of the script record at +0x58 */
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, (u8 *)obj + 0x58, obj->sub294, *(u8 *)&args->wait, args->flip);
}

/* One of the two 8-byte sprite records gUnk_03000FF8 points at (the party block's unk78). */
struct PartySpriteRec
{
    u16 spriteId;    /* bits 12-15: bank + 1, bits 0-11: index in that bank (decode_coord_mode) */
    u8 record;       /* the sprite record to show, decoded from a frame number */
    u8 unk3;
    u8 unk4;         /* nonzero: the HUD draws this one with tile 0x270 and palette 7 */
    u8 flipped : 1;  /* the sprite faces the other way */
    u8 bit1 : 1;
    u8 rest : 6;
};

extern u32 gUnk_03000FF8; /* -> struct PartySpriteRec[2] */
extern const u32 gRecordPtrTable[];
void decode_coord_mode(u32 bank, u32 index, s32 frame, u8 *outRecord, u8 *outFlip);

/* Script command: args = { slot, entry, frame, unk3, unk4, flip }. Sets party sprite record `slot`
   (gUnk_03000FF8) to sprite `entry` of the owner list's record set (gRecordPtrTable), decodes `frame`
   into that sprite's record and flip (decode_coord_mode) and flips it again when `flip` bit 0 is set.
   Always returns 1 (go on with the script). */
u32 set_record_from_args(struct OwnerObj *owner, void *unused, u32 *args)
{
    u8 record;
    u8 flipped;
    u32 *recordsPtr = &gUnk_03000FF8;
    u32 slot = *args++;
    struct PartySpriteRec *rec = (struct PartySpriteRec *)(*recordsPtr + (slot << 3));
    u16 recordSet = *(u16 *)((u8 *)owner->list + 0x126);
    u8 *block = (u8 *)*(u32 *)((u8 *)gRecordPtrTable + (recordSet << 2));
    /* the block's bytes 2-3 hold how far back its sprite id table starts */
    u8 *spriteIds = block - ((block[3] << 8) | block[2]);
    u32 entry = *args++;
    rec->spriteId = ((u16 *)spriteIds)[entry];
    record = *args++;
    decode_coord_mode(((u32)rec->spriteId >> 12) - 1, rec->spriteId & 0xFFF, record, &record, &flipped);
    rec->record = record;
    rec->unk3 = *args++;
    rec->unk4 = *args++;
    rec->flipped = (*args ^ flipped) & 1;
    rec->bit1 = 0;
    return 1;
}

ASM_FUNC("asm/code_080EDB40/set_effect_key.s", void set_effect_key(void))

/* Script command: args = { entity index, test, value }. test >> 1 picks a condition on the owner list's
   entity, test bit 0 the result wanted (1: true, 0: false); when the condition gives it, value is
   written to *out:
   0: flag bit 1 of Entity+0x20F,   1: bit 0 of the record's flags (Entity+0xF8),
   2: any of bits 3-8 of the halfword at Entity+0x24,   3: any of bits 3-6 of Entity+0x25,
   4: list entry 118 or 119 has Entity+0x54 in the low 7 bits of its +0x33C,
   5: flag bit 0 of Entity+0x20C. Always returns 1 (go on with the script). */
u32 cmd_test_and_store(struct OwnerObj *owner, u32 *out, u32 *args)
{
    struct Entity *entity;
    struct Record *record;
    u32 *test;
    u8 passed;
    u32 index = *args++;

    test = args;
    entity = *(struct Entity **)((u8 *)owner->list + index * 4 + 0x28);
    record = (struct Record *)((u8 *)entity + 0x58);
    switch ((s32)test[0] >> 1)
    {
    case 0:
        passed = (test[0] & 1) == ((u32)entity->unk20F << 30) >> 31;
        break;
    case 1:
        passed = (record->unkA0 & 1) ? test[0] & 1 : ((u8)test[0] ^ 1) & 1;
        break;
    case 2:
        passed = ((((u32)entity->unk24 << 25) >> 28) | (((u32)*(u16 *)&entity->unk24 << 23) >> 30)) ? test[0] & 1 : ((u8)test[0] ^ 1) & 1;
        break;
    case 3:
        passed = (entity->unk25 & 0x78) ? test[0] & 1 : ((u8)test[0] ^ 1) & 1;
        break;
    case 4:
        passed = ((s32)(owner->list->entries[118]->unk33C << 25) >> 25 == entity->unk54
            || (s32)(owner->list->entries[119]->unk33C << 25) >> 25 == entity->unk54) ? test[0] & 1 : ((u8)test[0] ^ 1) & 1;
        break;
    case 5:
        passed = (test[0] & 1) == ((u32)entity->unk20C << 31) >> 31;
        break;
    }
    if (passed)
        *out = test[1];
    return 1;
}

/* Arguments of cmd_move_toward. */
struct MoveTowardArgs
{
    /* bit 0: arc sign +1 (else -1); bit 1: first/second are relative to the entity;
       bits 2-3: motion mode - MOTION_BY_SPEED; bit 4: first/second are in 16-pixel tiles */
    u32 flags;
    s32 fixed_axis; /* 0 x, 1 y, 2 z: the axis that keeps its value */
    s32 first;      /* target on the first other axis (y, or x) */
    s32 second;     /* target on the second other axis (z, or y) */
    u16 unk10;
    u16 filler12;
    u32 speed; /* MOTION_BY_SPEED: whole pixels or PARAM_FRACTION | n for n/16, 0 keeps the entity's */
};

/* Script command: start moving the entity to a point, keeping its position on args->fixed_axis
   (or not moving on it, when the targets are relative). Speed mode sets the entity's speed first.
   Returns 0 (and marks the record's move as running) when a move started, 1 when it is already there. */
u32 cmd_move_toward(struct OwnerObj *owner, struct Entity *ent, u32 record, struct MoveTowardArgs *args)
{
    s32 x;
    s32 y;
    s32 z;
    u32 skip_x;
    u32 skip_y;
    u32 skip_z;
    s32 fixed_axis;
    u8 started;

    if ((((s32)args->flags >> 4) & 1) != 0)
    {
        /* tiles to pixels */
        args->first = args->first << 4;
        args->second = args->second << 4;
    }
    fixed_axis = args->fixed_axis;
    switch (fixed_axis)
    {
    case 0:
        if (((((u8)(args->flags >> 1)) ^ 1) & 1) != 0)
            x = ent->unkC / 0x100;
        else
            x = 0;
        y = args->first;
        z = args->second;
        skip_x = 1;
        skip_y = 0;
        skip_z = 0;
        break;
    case 1:
        x = args->first;
        if (((((u8)(args->flags >> 1)) ^ 1) & 1) != 0)
            y = ent->unk10 / 0x100;
        else
            y = 0;
        z = args->second;
        skip_x = 0;
        skip_y = 1;
        skip_z = 0;
        break;
    case 2:
        x = args->first;
        y = args->second;
        if (((((u8)(args->flags >> 1)) ^ 1) & 1) != 0)
            z = (ent->unk14 + ent->unk18) / 0x100;
        else
            z = 0;
        skip_x = 0;
        skip_y = 0;
        skip_z = 1;
        break;
    }
    if ((((s32)args->flags >> 2) & 3) == 0)
    {
        /* MOTION_BY_SPEED: the speed becomes 8.8 fixed point */
        if ((args->speed & PARAM_FRACTION) != 0)
            args->speed = (args->speed & PARAM_FRACTION_MASK) << 4;
        else
            args->speed = args->speed << 8;
        if (args->speed != 0)
            set_half_23e(ent, (s16)args->speed);
    }
    started = cell_motion_obj_move_toward_target(ent, (((s32)args->flags >> 2) & 3) + MOTION_BY_SPEED,
        ((s32)args->flags >> 1) & 1, x, y, z, skip_x << 8, skip_y << 8, skip_z << 8, 0x100, 0x100, 0x100,
        args->unk10, *(u16 *)&args->speed, (args->flags & 1) ? 1 : -1, -1);
    if (started != 0)
    {
        set_flag_a0_2(owner, ent, (u8 *)record, 0);
        return 0;
    }
    return 1;
}

/* Arguments of cmd_move_slot. */
struct MoveToSlotArgs
{
    /* bit 0: arc sign +1 (else -1); bits 1-2: motion mode - MOTION_BY_SPEED */
    u32 flags;
    s32 fixed_axis; /* 0 x, 1 y, 2 z: the axis on which the entity keeps its own position */
    u32 slot;       /* the entity of the owner's list to move to */
    u16 unkC;
    u16 fillerE;
    u32 speed; /* MOTION_BY_SPEED: whole pixels or PARAM_FRACTION | n for n/16, 0 keeps the entity's */
};

/* Script command: start moving the entity to another entity of the owner's list (args->slot),
   keeping its own position on args->fixed_axis. Speed mode first converts args->speed to 8.8 and,
   if it isn't 0, sets the entity's speed - from the low half of args->slot, as the game does.
   Returns 0 (and marks the record's move as running) when a move started, 1 when it is already there. */
u32 cmd_move_slot(struct OwnerObj *owner, struct Entity *ent, u32 record, struct MoveToSlotArgs *args)
{
    struct Entity *target;
    s32 x;
    s32 y;
    s32 z;
    u32 skip_x;
    u32 skip_y;
    u32 skip_z;
    s32 fixed_axis;
    u8 started;

    target = *(struct Entity **)((u8 *)owner->list + args->slot * 4 + 0x28);
    fixed_axis = args->fixed_axis;
    switch (fixed_axis)
    {
    case 0:
        x = ent->unkC / 0x100;
        y = target->unk10 / 0x100;
        z = (target->unk14 + target->unk18) / 0x100;
        skip_x = 1;
        skip_y = 0;
        skip_z = 0;
        break;
    case 1:
        x = target->unkC / 0x100;
        y = ent->unk10 / 0x100;
        z = (target->unk14 + target->unk18) / 0x100;
        skip_x = 0;
        skip_y = 1;
        skip_z = 0;
        break;
    case 2:
        x = target->unkC / 0x100;
        y = target->unk10 / 0x100;
        z = (ent->unk14 + ent->unk18) / 0x100;
        skip_x = 0;
        skip_y = 0;
        skip_z = 1;
        break;
    }
    if ((((s32)args->flags >> 1) & 3) == 0)
    {
        /* MOTION_BY_SPEED: the speed becomes 8.8 fixed point */
        if ((args->speed & PARAM_FRACTION) != 0)
            args->speed = (args->speed & PARAM_FRACTION_MASK) << 4;
        else
            args->speed = args->speed << 8;
        if (args->speed != 0)
            set_half_23e(ent, *(s16 *)&args->slot);
    }
    started = cell_motion_obj_move_toward_target(ent, (((s32)args->flags >> 1) & 3) + MOTION_BY_SPEED, 0, x, y, z,
        skip_x << 8, skip_y << 8, skip_z << 8, 0x100, 0x100, 0x100, args->unkC,
        *(u16 *)&args->speed, (args->flags & 1) ? 1 : -1, args->slot);
    if (started != 0)
    {
        set_flag_a0_2(owner, ent, (u8 *)record, 0);
        return 0;
    }
    return 1;
}

/* Script command: turns obj to face one of 8 directions (obj_set_stage), after finishing any move
 * or motion the script left running. args are (op, value):
 *   op 0: face direction value.        op 1: turn by value (wraps at 8).
 *   op 2: face the owner's list entry value (-1: the lead hero).
 *   op 3: face a random direction.
 * Always returns 1. */
u32 apply_stage_command(struct OwnerObj *owner, struct Entity *obj, struct Record *rec, u32 *args)
{
    u32 dir;
    struct Entity *target;
    if ((rec->unkA0 & REC_MOVE_RUNNING) != 0)
    {
        obj_finish_move_step(obj);
        rec->unkA0 = rec->unkA0 & (u16)~REC_MOVE_RUNNING;
    }
    if ((rec->unkA0 & REC_MOTION_RUNNING) != 0)
    {
        obj_stop_motion(obj);
        rec->unkA0 = rec->unkA0 & (u16)~REC_MOTION_RUNNING;
    }
    switch ((s32)*args++)
    {
    case 0:
        dir = *(u8 *)args;
        break;
    case 1:
        dir = ((((u32)obj->unk24 << 29) >> 29) + *args) & 7; /* unk24 bits 0-2: current direction */
        break;
    case 2:
        if (*args == -1)
            *args = LIST_LEAD_HERO(owner->list);
        target = *(struct Entity **)((u8 *)owner->list + *args * 4 + 0x28); /* owner->list->entries[*args] */
        dir = (u8)direction_octant((u8 *)owner + 0x1C, obj->unkC, obj->unk10, target->unkC, target->unk10);
        break;
    case 3:
        dir = (u8)rand_below(8, mt19937_next());
        break;
    }
    obj_set_stage(obj, dir);
    return 1;
}

/* Script command: sets the entity's box, the halfwords at Entity+0x138 (top, bottom, left, right).
 * cmds[0] bit 0: cmds[1..4] (left, top, right, bottom) are 16-px tiles, measured to the tile's
 * centre; bit 1: they are relative to the entity's position (24.8 fixed point, rounded toward 0).
 * Returns 1. */
u32 set_entity_box(void *unused0, struct Entity *entity, void *unused2, u32 *cmds)
{
    u8 *base = (u8 *)entity + 0x100;
    s16 scale;
    s16 centre;
    s16 originY;
    s32 originX;
    s32 d;
    s32 left, top, right, bottom;

    scale = (*cmds & 1) ? 16 : 1;
    centre = (*cmds & 1) ? 8 : 0;
    originY = ((*cmds & 2) ? entity->unk10 / 256 : 0) + centre;
    originX = centre + ((*cmds++ & 2) ? entity->unkC / 256 : 0);
    d = scale * *cmds++;
    left = (s16)originX + d;
    *(s16 *)(base + 0x3C) = left;
    d = scale * *cmds++;
    top = originY + d;
    *(s16 *)(base + 0x38) = top;
    d = scale * *cmds++;
    right = (s16)originX + d;
    *(s16 *)(base + 0x3E) = right;
    d = scale * *cmds;
    bottom = originY + d;
    *(s16 *)(base + 0x3A) = bottom;
    return 1;
}

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

NONMATCH("asm/code_080EDB40/set_entity_field.s", u32 set_entity_field(void *unused0, struct Entity *entity, void *unused2, u32 *cmds))
{
    struct Entity *target;
    if (*cmds++ != 0)
        target = entity->link344;
    else
        target = entity;
    switch (*cmds++)
    {
    case 0:
        target->unk131 = *cmds;
        break;
    case 1:
        target->unk2F4 = *cmds;
        break;
    case 2:
        target->unk2F8 = *cmds;
        break;
    case 3:
        target->unk136 = *cmds;
        break;
    case 4:
        ((struct Lo3Bits *)&target->unk137)->lo3 = *cmds;
        break;
    case 5:
        ((struct Lo2Bits *)&target->unk130)->lo2 = *cmds;
        break;
    case 6:
        ((struct Flags8 *)&target->unk130)->bit2 = *cmds;
        break;
    case 7:
        target->unk134 = *cmds;
        break;
    }
    return 1;
}
END_NONMATCH

/* Script command: cmds[0] picks the entity (0) or its linked entity, cmds[1] picks one of its
 * four-halfword parameter groups, and cmds[2..5] are stored into it. */
u32 set_box_fields(void *unused0, struct Entity *entity, void *unused2, u32 *cmds)
{
    struct Entity *target;
    struct EntityParams *params;
    struct EntityParams *paramsAlt;
    u32 *arg = cmds;
    u32 v;
    u32 w;

    if (*arg++ != 0)
        target = entity->link344;
    else
        target = entity;
    params = (struct EntityParams *)((u8 *)target + 0x100);
    paramsAlt = params;
    switch (*arg++)
    {
    case 0:
        v = *arg;
        if (v & PARAM_FRACTION)
            params->unk108[0] = v & PARAM_FRACTION_MASK;
        else
            *(s16 *)&params->unk108[0] = v << 4;
        arg++;
        v = *arg;
        if (v & PARAM_FRACTION)
            params->unk108[1] = v & PARAM_FRACTION_MASK;
        else
            *(s16 *)&params->unk108[1] = v << 4;
        arg++;
        v = *arg;
        if (v & PARAM_FRACTION)
            params->unk108[2] = v & PARAM_FRACTION_MASK;
        else
            *(s16 *)&params->unk108[2] = v << 4;
        v = arg[1];
        if (v & PARAM_FRACTION)
            params->unk108[3] = v & PARAM_FRACTION_MASK;
        else
            *(s16 *)&params->unk108[3] = v << 4;
        break;
    case 1:
        v = *arg;
        if (v & PARAM_FRACTION)
            params->unk100[0] = (v & PARAM_FRACTION_MASK) << 4;
        else
            params->unk100[0] = v << 8;
        arg++;
        v = *arg;
        if (v & PARAM_FRACTION)
            params->unk100[1] = (v & PARAM_FRACTION_MASK) << 4;
        else
            params->unk100[1] = v << 8;
        arg++;
        v = *arg;
        if (v & PARAM_FRACTION)
            params->unk100[2] = (v & PARAM_FRACTION_MASK) << 4;
        else
            params->unk100[2] = v << 8;
        v = arg[1];
        if (v & PARAM_FRACTION)
            params->unk100[3] = (v & PARAM_FRACTION_MASK) << 4;
        else
            params->unk100[3] = v << 8;
        break;
    case 2:
        params->unk128[0] = *arg++;
        params->unk128[1] = *arg++;
        params->unk128[2] = *arg++;
        params->unk128[3] = *arg;
        break;
    case 3:
        arg[0] <<= 4;
        arg[1] <<= 4;
        arg[2] <<= 4;
        arg[3] <<= 4;
        /* fall through */
    case 4:
        params->unk120[0] = *arg++;
        params->unk120[1] = *arg++;
        params->unk120[2] = *arg++;
        params->unk120[3] = *arg;
        break;
    case 5:
        params->unk110[0] = *arg++;
        params->unk110[1] = *arg++;
        params->unk110[2] = *arg++;
        params->unk110[3] = *arg;
        break;
    case 6:
        params->unk118[0] = *arg++;
        params->unk118[1] = *arg++;
        params->unk118[2] = *arg++;
        params->unk118[3] = *arg;
        break;
    case 7:
        paramsAlt->unk13E[0] = *arg++;
        v = *arg++;
        paramsAlt->unk13E[1] = v;
        w = *arg++;
        paramsAlt->unk13E[2] = w;
        v = *arg;
        paramsAlt->unk13E[3] = v;
        break;
    case 8:
        paramsAlt->unk136[0] = *arg++;
        paramsAlt->unk136[1] = *arg++;
        paramsAlt->unk136[2] = *arg++;
        paramsAlt->unk136[3] = *arg;
        break;
    case 9:
        params->unk108[0] = *arg++;
        params->unk108[1] = *arg++;
        params->unk108[2] = *arg++;
        params->unk108[3] = *arg;
        break;
    case 10:
        params->unk100[0] = *arg++;
        params->unk100[1] = *arg++;
        params->unk100[2] = *arg++;
        params->unk100[3] = *arg;
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

/* Script command, set_entity_mode plus two words: args[0] is the mode (0 and 1 also set and clear
 * bit 6 of unk2B6; a mode past the table clears REC_MODE_SET), args[1] and args[2] go to the
 * entity's unk1B0/unk1B4 (Record unk158/unk15C) and to those of the entity linked at link344. */
u32 set_entity_mode_and_link(void *unused0, struct Entity *entity, void *unused2, u32 *args)
{
    struct Record *rec = (struct Record *)((u8 *)entity + 0x58);
    u32 flags = entity->unk2B6 | 0x80;
    entity->unk2B6 = flags;
    switch (args[0])
    {
    case 0:
        flags = flags | 0x40;
        entity->unk2B6 = flags;
        break;
    case 1:
        flags = flags & ~0x40;
        entity->unk2B6 = flags;
        break;
    }
    if ((s32)args[0] < ENTITY_MODE_COUNT)
    {
        rec->unk140 = gEntityModeDataTable[args[0]];
        rec->unkA0 = rec->unkA0 | REC_MODE_SET;
        ((struct Bits1 *)&rec->unk17E)->bit1 = 0;
        ((struct Bits1 *)&rec->unk17E)->bit0 = 0;
    }
    else
    {
        rec->unkA0 = rec->unkA0 & (u16)~REC_MODE_SET;
    }
    args++;
    rec->unk158 = *args++;
    rec->unk15C = *args;
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

    ((struct Mid4Bits *)(gEffectState + 0x55B))->mid4 = bits & 0xF;

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
