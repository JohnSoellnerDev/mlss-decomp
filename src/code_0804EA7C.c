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
    u8 filler2A[0x40 - 0x2A];
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

ASM_FUNC("asm/code_0804EA7C/obj_snap_to_tile_walls.s", void obj_snap_to_tile_walls(void))

ASM_FUNC("asm/code_0804EA7C/sweep_obj_motion.s", void sweep_obj_motion(void))

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

NONMATCH("asm/code_0804EA7C/write_oam_entry.s", void write_oam_entry(struct Obj *obj, u16 a, u16 b))
{
    obj->oam44->attr0 = (obj->unk8 / 0x100) - (s16)a;
    obj->oam44->attr1 = ((obj->unkC - obj->unk10 - obj->unk14) / 0x100) - (s16)b;
    obj->oam44->attrE = ((((u32)*(u16 *)obj << 21) >> 28) << 12) | (0x800 - (obj->unkC / 0x100));
}
END_NONMATCH

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

NONMATCH("asm/code_0804EA7C/update_facing_bits.s", void update_facing_bits(struct Obj *obj))
{
    if (obj->unk25C == -1 && (obj->unk2B5 & 7) == 0) {
        const u8 *table = gFacingTable;
        u32 idx = ((obj->flags2 >> 5) << 2) + (((u32)((u8 *)obj)[0x26] << 28) >> 30);
        u8 value = table[idx];
        u8 *dst = &obj->unk353;
        ((struct Bits45 *)dst)->f = value & 3;
    }
}
END_NONMATCH

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

struct Obj *obj_init_with_child(struct Obj *obj, u32 a, u32 b)
{
    u8 found;

    obj_init(obj);
    obj->vtable338 = (struct ObjVtable *)gObjForwardChildClass;
    ((struct Bit0 *)&obj->unk210)->b0 = 0;
    if ((u8)find_def_index((u16)a, &found, obj) != 0) {
        struct Obj **slot = &obj->child344;
        void *block = heap_alloc2(0x348, 1, gObjInitChildTag, 1);

        *slot = obj_attach_child(block, -1, b, found, obj);
    }
    ((struct Bits33C *)&obj->unk33C)->b0 = 1;
    ((struct Bits33C *)&obj->unk33C)->mid12 = 0;
    if (test_flag(gUnk_03000FC0 + 0x38C, 0, ((obj->unk22C << 10) >> 20) + 0x960) == 1) {
        ((struct Bit0 *)((u8 *)obj + 0x20C))->b0 = 0;
        ((struct Bit1Field *)((u8 *)obj + 0x20F))->b1 = 0;
        if (obj->child344 != 0) {
            ((struct Bit0 *)((u8 *)obj->child344 + 0x20C))->b0 = 0;
            ((struct Bit1Field *)((u8 *)obj->child344 + 0x20F))->b1 = 0;
        }
    }
    return obj;
}

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

NONMATCH("asm/code_0804EA7C/obj_update_mode.s", void obj_update_mode(struct Obj *obj))
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
                obj->unk33E = (obj->unk33E & -4) | 2;
            } else {
                obj->flags2 += 0x10;
            }
            break;
        case 2: {
            u32 cnt = obj->unk340 - 1;

            obj->unk340 = cnt;
            if ((cnt << 24) == 0) {
                obj->unk340 = 0x3C;
                obj->unk33E |= 3;
            }
            break;
        }
        case 3: {
            u32 cnt = obj->unk340 - 1;
            s32 sc;
            u32 flags;

            obj->unk340 = cnt;
            sc = (s8)cnt;
            if (sc == 0) {
                u32 m2 = ((u32)obj->unk33E << 29) >> 31;

                ((struct Bit5 *)((u8 *)obj + 0x20D))->b5 = m2;
                obj->flags2 = obj->unk33F;
                obj->unk23A = sc;
                flags = obj->unk33C | 1;
                obj->unk33C = flags;
                if ((flags & 0x80) != 0) {
                    ((struct Lo3Bits *)&obj->unk33D)->lo3 = (flags << 25) >> 29;
                } else {
                    ((struct Lo3Bits *)&obj->unk214)->lo3 = (flags << 25) >> 29;
                }
                ((struct Bits33C *)&obj->unk33C)->b3 = 0;
                ((struct Bits33E *)&obj->unk33E)->mode = 0;
            } else {
                s32 bits = cnt & mode;

                switch (bits) {
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
END_NONMATCH

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
