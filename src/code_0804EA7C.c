#include "global.h"

struct HalfBit4
{
    u16 lo : 4;
    u16 bit4 : 1;
    u16 hi : 11;
};

struct HalfBit0
{
    u16 bit0 : 1;
    u16 hi : 15;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Bit6Bits
{
    u8 low6 : 6;
    u8 b6 : 1;
    u8 rest : 1;
};

struct Bit0
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct Bit5
{
    u8 low5 : 5;
    u8 b5 : 1;
    u8 rest : 2;
};

struct Bits33C
{
    u8 b0 : 1;
    u8 mid12 : 2;
    u8 b3 : 1;
    u8 bits4 : 3;
    u8 b7 : 1;
};

struct Bit1Field
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

struct Bits33E
{
    u8 mode : 2;
    u8 b2 : 1;
    u8 rest : 5;
};

struct Bits33CByte
{
    u8 b0 : 1;
    u8 mid12 : 2;
    u8 b3 : 1;
    u8 bits4 : 3;
    u8 b7 : 1;
} __attribute__((packed));

struct ObjBits214
{
    u8 filler0[0x214];
    u8 lo3 : 3;
    u8 rest : 5;
};

struct ObjBits33D
{
    u8 filler0[0x33D];
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Bits45
{
    u8 lo4 : 4;
    u8 f : 2;
    u8 rest : 2;
};

struct SlotState
{
    u8 unk0;
    u8 unk1;
    u8 filler2[0xE];
    u8 unk10[2];
    u8 unk12;
    s8 unk13;
};

struct OamEntry
{
    u16 attr0;
    u16 attr1;
    u8 filler4[0xA];
    u16 attrE;
};

struct VtEntry
{
    s16 off;
    u16 pad2;
    void (*fn)(void *, u32);
};

struct ObjVtable
{
    u8 filler0[0x80];
    struct VtEntry e80;
};

struct SpriteRec
{
    u8 filler0[0x12];
    u8 unk12;
};

struct Obj
{
    u8 flags0;
    u8 unk1;
    u8 flags2;
    u8 unk3;
    u16 unk4;
    u8 filler6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u32 unk20;
    s32 unk24;
    u8 filler28;
    u8 unk29;
    u8 filler2A[0x32 - 0x2A];
    u16 unk32;
    u8 filler34[0x36 - 0x34];
    u16 unk36;
    u8 filler38[0x40 - 0x38];
    u8 *map40;
    struct OamEntry *oam44;
    u8 filler48[0x54 - 0x48];
    u8 unk54;
    u8 filler55[0x20C - 0x55];
    u16 unk20C;
    u8 filler20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211;
    u8 unk212;
    u8 filler213;
    u8 unk214;
    u8 filler215[0x22C - 0x215];
    u32 unk22C;
    u8 filler230[0x238 - 0x230];
    s8 unk238;
    u8 filler239[0x23A - 0x239];
    u8 unk23A;
    u8 filler23B[0x242 - 0x23B];
    s16 unk242;
    s16 unk244;
    u8 filler246[0x25C - 0x246];
    s32 unk25C;
    u8 filler260[0x274 - 0x260];
    u16 unk274;
    u8 filler276[0x294 - 0x276];
    struct SpriteRec *sprite294;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 filler2B6[0x338 - 0x2B6];
    struct ObjVtable *vtable338;
    u8 unk33C;
    u8 unk33D;
    u8 unk33E;
    u8 unk33F;
    u8 unk340;
    u8 filler341[0x344 - 0x341];
    struct Obj *child344;
    s32 unk348;
    s32 unk34C;
    u8 filler350[2];
    u8 unk352;
    u8 unk353;
    u8 filler354[0x35A - 0x354];
    u8 unk35A;
    u8 unk35B;
    u8 filler35C[0x368 - 0x35C];
    struct SlotState *slots;
};

struct ObjSel353
{
    u8 filler0[0x353];
    u8 lo4 : 4;
    u8 sel : 2;
    u8 hi2 : 2;
} __attribute__((packed));

void unlink_d54(void *e);
void obj_set_id(struct Obj *obj, u32 id);
u32 obj_step_toward_goal(struct Obj *obj);
void obj_step_tile_collide(struct Obj *obj, u8 a, u8 b);
void obj_enter_stage3_unlink(struct Obj *obj);
void obj_set_table_id(struct Obj *obj, u32 id);
void obj_set_table_id_flag7(struct Obj *obj, u32 id);
void motion_obj_step_motion(struct Obj *obj);
u8 *tile_ptr_at(u8 *map, s32 x, s32 y);
void motion_obj_set_move_target(struct Obj *obj, u32 a, u32 b, s32 c);
void set_flag(u8 *table, u32 zero, u32 index, u32 one);
extern u8 *gUnk_03000FC0;
void obj_init(struct Obj *obj);
u32 find_def_index(u32 id, u8 *found, struct Obj *obj);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
struct Obj *obj_attach_child(void *block, s32 neg, u32 b, u8 flag, struct Obj *obj);
u8 test_flag(u8 *table, u32 zero, u32 index);
extern u8 gObjForwardChildClass[];
extern const u8 gObjInitChildTag[];
extern u8 gUnk_02000001[];
extern const u8 gFacingTable[];

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);

ASM_FUNC("asm/code_0804EA7C/obj_snap_to_tile_walls.s", void obj_snap_to_tile_walls(void))

ASM_FUNC("asm/code_0804EA7C/sweep_obj_motion.s", void sweep_obj_motion(struct Obj *obj))

u8 *cell_offsets(struct Obj *obj, s16 x, s16 y, u32 *outA, u32 *outB, u32 *outC)
{
    u8 *cell = tile_ptr_at(obj->map40, x, y);
    u32 a = (((*(u32 *)cell << 20) >> 28) << 11) << (*(u32 *)cell >> 31);

    *outA = a;
    if ((cell[2] & 0xF0) != 0) {
        u32 b = a + ((((*(u32 *)cell << 16) >> 28) << 11) << (*(u32 *)cell >> 31));

        *outB = b;
        *outC = b + ((((*(u32 *)cell << 8) >> 28) << 11) << (*(u32 *)cell >> 31));
    } else {
        *outC = -1;
        *outB = -1;
    }
    return cell;
}

void write_oam_entry(struct Obj *obj, u16 a, u16 b)
{
    obj->oam44->attr0 = (obj->unk8 / 0x100) - ((s32)((u32)a << 16) >> 16);
    obj->oam44->attr1 = ((obj->unkC - obj->unk10 - obj->unk14) / 0x100) - ((s32)((u32)b << 16) >> 16);
    obj->oam44->attrE = ((((u32)*(u16 *)obj << 21) >> 28) << 12) | (0x800 - (obj->unkC / 0x100));
}

void obj_release_oam(struct Obj *obj)
{
    unlink_d54(obj->oam44);
    ((struct Bit6Bits *)&obj->flags0)->b6 = 0;
}

void save_prev_position(struct Obj *obj)
{
    obj->unk18 = obj->unk8;
    obj->unk1C = obj->unkC;
    obj->unk20 = obj->unk10;
    obj->unk24 = obj->unk14;
}

u8 *set_low3_bits(u8 *ptr, u32 bits)
{
    ((struct Lo3Bits *)ptr)->lo3 = bits;
    return ptr;
}

s32 obj_link_index(struct Obj *obj)
{
    struct Obj *target = (struct Obj *)obj->unk20;

    if ((target->flags0 & 7) == 3 || (target->flags2 & 0xF) == 3) {
        return (*(u32 *)target << 1) >> 30;
    }
    return -1;
}

void update_linked_pos(struct Obj *obj, struct Obj **table, u8 a, u8 b)
{
    struct Obj *link;
    s32 pos;

    obj_step_tile_collide(obj, a, b);
    link = 0;
    if (obj->unk238 != -1) {
        link = table[obj->unk238];
        if (obj->unk14 >= link->unk14 + link->unk18 + link->unk274) {
            obj->unk29 = 0xFF;
            obj->unk1 = -1;
            obj->unk238 = -1;
            link = 0;
        }
    }
    pos = obj->unk14;
    if (link != 0) {
        pos += link->unk18 + link->unk274;
    }
    obj->unk1C = pos;
}

u8 update_if_active(struct Obj *obj)
{
    u8 result = 0;

    if ((obj->unk20C & 0x801) == 0x801 && (obj->unk35A & 2) == 0) {
        obj->unk348 = obj->unkC;
        obj->unk34C = obj->unk10;
        result = obj_step_toward_goal(obj);
    }
    return result;
}

void update_facing_bits(struct Obj *obj)
{
    if (obj->unk25C == -1 && (obj->unk2B5 & 7) == 0)
    {
        u32 value = gFacingTable[((obj->flags2 >> 5) << 2) + (((u32)((u8 *)obj)[0x26] << 28) >> 30)];
        ((struct ObjSel353 *)obj)->sel = value;
    }
}

void obj_shift_state_slots(struct Obj *obj)
{
    if (obj->slots->unk13 != -1) {
        obj->slots->unk0 = obj->slots->unk1;
        obj->slots->unk10[(obj->unk352 >> 7) ^ 1] = obj->slots->unk13;
        obj->slots->unk13 = -1;
    }
}

void obj_start_state(struct Obj *obj)
{
    u32 id;

    ((struct Lo3Bits *)&obj->unk214)->lo3 = 3;
    if (obj->unk54 == 0) {
        if (obj->unk35B & 8) {
            id = 0x1028;
        } else {
            id = 0x1001;
        }
    } else {
        id = 0x1002;
    }
    obj_set_id(obj, id);
    obj->unk4 = 0;
    ((struct Bit6Bits *)&obj->unk212)->b6 = 0;
}

void obj_start_state_4(struct Obj *obj)
{
    u32 id;

    ((struct Lo3Bits *)&obj->unk214)->lo3 = 4;
    if (obj->unk54 != 0) {
        id = 0x1053;
    } else {
        id = 0x1053;
        id -= 0x15;
    }
    obj_set_id(obj, id);
    obj->unk35A |= 4;
}

void obj_set_state_flags(struct Obj *obj, u8 value)
{
    u32 state = ((u32)obj->unk214 << 29) >> 29;
    u32 dir = ((u32)((u8 *)obj)[0x24] << 29) >> 29;
    u16 kind = obj->unk4;

    if ((u16)(kind - 5) <= 4) {
        obj_start_state(obj);
    } else if ((u16)(kind - 10) <= 2) {
        obj_enter_stage3_unlink(obj);
    }
    ((struct Lo3Bits *)&obj->unk214)->lo3 = state;
    ((struct Lo3Bits *)&obj->unk24)->lo3 = dir;
    obj_set_table_id(obj, value);
}

void obj_set_state_flags_alt(struct Obj *obj, u8 value)
{
    u32 state = ((u32)obj->unk214 << 29) >> 29;
    u32 dir = ((u32)((u8 *)obj)[0x24] << 29) >> 29;
    u16 kind = obj->unk4;

    if ((u16)(kind - 5) <= 4) {
        obj_start_state(obj);
    } else if ((u16)(kind - 10) <= 2) {
        obj_enter_stage3_unlink(obj);
    }
    ((struct Lo3Bits *)&obj->unk214)->lo3 = state;
    ((struct Lo3Bits *)&obj->unk24)->lo3 = dir;
    obj_set_table_id_flag7(obj, value);
}

void child_forward_obj_step_motion(struct Obj *obj)
{
    if (((struct Bit0 *)&obj->unk20C)->b0 != 0) {
        if ((obj->unk2B5 & 7) == 0) {
            obj->unkC += obj->unk242;
            obj->unk10 += obj->unk244;
        }
        if ((obj->unk2B5 & 8) == 0) {
            motion_obj_step_motion(obj);
        }
    }
}

/* Sets up an object that forwards to a child: inits it, sets its class, and when definition defId
 * is found allocates and attaches the child object (child344). If the object's one-time story flag
 * (0x960 + the 12-bit index in bits 10-21 of unk22C, set by obj_refresh_child_sprite) is already
 * set, the object and its child start inactive (0x20C bit 0) and hidden (0x20F bit 1). */
struct Obj *obj_init_with_child(struct Obj *obj, u32 defId, u32 childArg)
{
    u8 defIndex;

    obj_init(obj);
    obj->vtable338 = (struct ObjVtable *)gObjForwardChildClass;
    ((struct Bit0 *)&obj->unk210)->b0 = 0;
    if ((u8)find_def_index((u16)defId, &defIndex, obj) != 0) {
        struct Obj **childSlot = &obj->child344;
        void *block = heap_alloc2(0x348, 1, gObjInitChildTag, 1);

        *childSlot = obj_attach_child(block, -1, childArg, defIndex, obj);
    }
    ((struct Bits33C *)&obj->unk33C)->b0 = 1;
    ((struct Bits33C *)&obj->unk33C)->mid12 = 0;
    if (test_flag(gUnk_03000FC0 + 0x38C, 0, ((obj->unk22C << 10) >> 20) + 0x960) == 1) {
        ((struct Bit0 *)&obj->unk20C)->b0 = 0;
        ((struct Bit1Field *)&obj->unk20F)->b1 = 0;
        if (obj->child344 != NULL) {
            ((struct Bit0 *)&obj->child344->unk20C)->b0 = 0;
            ((struct Bit1Field *)&obj->child344->unk20F)->b1 = 0;
        }
    }
    return obj;
}

/* Starts the spin-and-recover sequence that obj_update_mode steps (mode 1 in unk33E): saves what
   the sequence changes and puts back at its end (0x20D bit 5 in unk33E bit 2, the facing flags2 in
   unk33F, and once the 3-bit state of unk214 in unk33C bits 4-6, bit 3 marking it saved), clears
   0x20D bit 5 and unk33C bit 0, sets state 5 and makes the object hop (height 0x40, gravity 0x20). */
void obj_reset_movement(struct Obj *obj)
{
    ((struct Bits33E *)&obj->unk33E)->mode = 1;
    ((struct Bits33E *)&obj->unk33E)->b2 = (((u32)((u8 *)obj)[0x20D] << 26) >> 31) & 1;
    ((struct Bit5 *)((u8 *)obj + 0x20D))->b5 = 0;
    ((struct Bits33C *)&obj->unk33C)->b0 = 0;
    obj->unk33F = obj->flags2;
    if (((struct Bits33C *)&obj->unk33C)->b3 == 0) {
        ((struct Bits33C *)&obj->unk33C)->b3 = 1;
        ((struct Bits33C *)&obj->unk33C)->bits4 = ((u32)obj->unk214 << 29) >> 29;
    }
    ((struct Lo3Bits *)&obj->unk214)->lo3 = 5;
    motion_obj_set_move_target(obj, 0x40, 0x20, -1);
}

/*
 * Steps the spin-and-recover sequence obj_reset_movement starts (mode in the low 2 bits of
 * unk33E), while both bits of the mask gUnk_02000001 (0x02000001, an address used as a constant)
 * are set in the word at unk20C:
 *   1: spin (flags2's top bits are the facing, +0x10 a frame) until unk18 reaches 0, then face
 *      0x80 and wait 3 frames;
 *   2: wait, then wobble for 60 frames;
 *   3: unk23A wobbles 0, -1, 0, +1; at the end everything obj_reset_movement saved is put back
 *      (0x20D bit 5, flags2, the 3-bit state in unk33D or unk214) and the sequence ends.
 */
void obj_update_mode(struct Obj *obj)
{
    u32 mode;

    if ((*(u32 *)&obj->unk20C & (u32)gUnk_02000001) == (u32)gUnk_02000001) {
        mode = ((u32)obj->unk33E << 30) >> 30;
        switch (mode) {
        case 1:
            if ((s32)obj->unk18 <= 0) {
                obj->flags2 = 0x80;
                obj->unk18 = 0;
                obj->unk340 = 3;
                ((struct Bits33E *)&obj->unk33E)->mode = 2;
            } else {
                obj->flags2 += 0x10;
            }
            break;
        case 2: {
            u32 timer = obj->unk340 - 1;

            obj->unk340 = timer;
            if ((timer << 24) == 0) {
                obj->unk340 = 60;
                ((struct Bits33E *)&obj->unk33E)->mode = 3;
            }
            break;
        }
        case 3: {
            u32 timer = obj->unk340 - 1;
            s32 left;

            obj->unk340 = timer;
            left = (s8)timer;
            if (left == 0) {
                u32 savedBit5 = ((u32)obj->unk33E << 29) >> 31;

                ((struct Bit5 *)((u8 *)obj + 0x20D))->b5 = savedBit5;
                obj->flags2 = obj->unk33F;
                obj->unk23A = left;
                ((struct Bits33CByte *)&obj->unk33C)->b0 = 1;
                if (((struct Bits33CByte *)&obj->unk33C)->b7) {
                    ((struct ObjBits33D *)obj)->lo3 = ((struct Bits33CByte *)&obj->unk33C)->bits4;
                } else {
                    ((struct ObjBits214 *)obj)->lo3 = ((struct Bits33CByte *)&obj->unk33C)->bits4;
                }
                ((struct Bits33C *)&obj->unk33C)->b3 = 0;
                ((struct Bits33E *)&obj->unk33E)->mode = 0;
            } else {
                s32 wobble = timer & mode; /* mode is 3 here */

                switch (wobble) {
                case 0:
                case 2:
                    obj->unk23A = 0;
                    break;
                case 1:
                    obj->unk23A = 0xFF;
                    break;
                case 3:
                    obj->unk23A = 1;
                    break;
                }
            }
            break;
        }
        }
    }
}

NONMATCH("asm/code_0804EA7C/obj_refresh_child_sprite.s", void obj_refresh_child_sprite(struct Obj *obj))
{
    struct VtEntry *e;

    ((struct HalfBit0 *)&*((u8 *)obj + 0x20C))->bit0 = 0;
    ((struct Bits33C *)((u8 *)obj + 0x33C))->mid12 = 1;
    ((struct HalfBit4 *)&obj->sprite294->unk12)->bit4 = 1;
    e = &obj->vtable338->e80;
    e->fn((u8 *)obj + e->off, 2);
    if (obj->child344 != 0 && (obj->child344->unk20F & 2) != 0) {
        ((struct HalfBit0 *)&*((u8 *)obj->child344 + 0x20C))->bit0 = 0;
        ((struct HalfBit4 *)&obj->child344->sprite294->unk12)->bit4 = 1;
        e = &obj->child344->vtable338->e80;
        e->fn((u8 *)obj->child344 + e->off, 2);
    }
    set_flag(gUnk_03000FC0 + 0x38C, 0, ((obj->unk22C << 10) >> 20) + 0x960, 1);
}
END_NONMATCH
