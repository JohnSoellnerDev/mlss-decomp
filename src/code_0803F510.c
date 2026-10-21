#include "global.h"

struct TaskObj
{
    u8 filler0[0x18];
    const void *cb18;
};

extern const u8 gEntryUpdateTaskClass[];
extern const u8 gObjUpdateOamTaskClass[];

struct FieldSub
{
    s16 posX;
    s16 posY;
    u16 unk4;
    u16 unk6;
    u8 filler8[0x11 - 0x8];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    u16 unk2A;
    u16 unk2C;
};

struct SubBits11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags213Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct SubBits1F
{
    u8 filler0[0x1F];
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 hi4 : 4;
};

struct SubBits12
{
    u8 filler0[0x12];
    u8 bit0 : 1;
    u8 pair12 : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct Flags213Dirty
{
    u8 bit0 : 1;
    u8 dirty : 4;
    u8 hi3 : 3;
};

struct Flags298Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct Flags299Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct FieldVtable
{
    u8 filler0[0x38];
    s16 off38;
    u8 filler3A[0x2];
    void (*fn3C)(u8 *, u32);
};

struct FieldObj
{
    u8 filler0[0x1];
    u8 unk01;
    u8 unk02;
    u8 filler3;
    s16 state4;
    u8 filler6[0x6];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x28 - 0x25];
    u8 unk28;
    u8 unk29;
    u8 filler2A[0x2C - 0x2A];
    s16 unk2C;
    u8 filler2E[0x34 - 0x2E];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x4C - 0x44];
    u8 unk4C;
    u8 filler4D[0x50 - 0x4D];
    u16 unk50;
    u16 unk52;
    u8 filler54[0x20C - 0x54];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 flags210;
    u8 filler211[0x213 - 0x211];
    u8 flags213;
    u8 state214;
    u8 filler215;
    u8 flags216;
    u8 filler217;
    u16 id218;
    u8 filler21A[0x21E - 0x21A];
    u16 extra21E;
    u8 filler220[0x230 - 0x220];
    s32 unk230;
    s32 unk234;
    u8 unk238;
    u8 unk239;
    u8 filler23A[0x23D - 0x23A];
    u8 unk23D;
    u16 unk23E;
    u16 unk240;
    s16 unk242;
    s16 unk244;
    u8 filler246[0x248 - 0x246];
    s32 unk248;
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    s32 unk258;
    s32 counter25C;
    u8 filler260[0x262 - 0x260];
    u16 unk262;
    s16 unk264;
    s16 unk266;
    u16 unk268;
    u8 filler26A[0x26C - 0x26A];
    u16 unk26C;
    u8 unk26E;
    u8 filler26F[0x294 - 0x26F];
    struct FieldSub *sub;
    u8 flags298;
    u8 flags299;
    u8 filler29A;
    u8 unk29B;
    u8 unk29C;
    u8 filler29D;
    u16 unk29E;
    u16 unk2A0;
    u16 unk2A2;
    u16 unk2A4;
    u8 filler2A6[0x2B5 - 0x2A6];
    u8 flags2B5;
    u8 filler2B6;
    u8 flags2B7;
    u8 flags2B8;
    u8 filler2B9[0x338 - 0x2B9];
    struct FieldVtable *vt338;
};

extern const u32 gTableEntryRows[];
extern const u8 gFrameAttrTable[];

void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void cpu_set(const void *src, void *dst, u32 ctl);
void obj_reset_move_state(struct FieldObj *obj);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_set_motion_params(struct FieldObj *obj, u16 a, u32 b, u32 d, u32 e);
void load_object_def(u16 def, u8 index, struct FieldObj *obj);
extern const u8 gObjBaseClass[];
extern const u8 gSpriteFacingTable[];
extern u8 gUnk_02000001[];
void setup_obj_10bc(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);

ASM_FUNC("asm/code_0803F510/obj_update_oam.s", void obj_update_oam(void))

ASM_FUNC("asm/code_0803F510/build_sprite_oam.s", void build_sprite_oam(void))

ASM_FUNC("asm/code_0803F510/tile_query.s", void tile_query(void))

void decode_coord_mode(u32 row, u32 col, s32 x, u8 *out_a, u8 *out_b)
{
    u32 word;
    u32 mode;
    u32 variant;
    s32 mul;
    u8 v;

    word = *(u32 *)(gTableEntryRows[row] + (col << 2));
    mode = (word << 1) >> 30;
    variant = (word << 3) >> 31;
    if (mode != 0)
    {
        if (variant == 0)
        {
            if (mode == 1)
            {
                mul = (x / 4) * 3;
                x -= (x / 4) * 4;
                x *= 2;
            }
            else
            {
                mul = (x / 8) * 5;
                x -= (x / 8) * 8;
            }
        }
        else
        {
            if (mode == 1)
            {
                mul = (x / 4) * 4;
                x -= mul;
                x *= 2;
            }
            else
            {
                mul = (x / 8) * 8;
                x -= mul;
            }
        }
        v = gFrameAttrTable[(mode << 4) + (variant << 3) + x];
        *out_a = mul + (v & 0x7F);
        *out_b = v >> 7;
    }
    else
    {
        *out_a = x;
        *out_b = mode;
    }
}

void entry_update_task_remove(struct TaskObj *obj, u32 kind)
{
    obj->cb18 = gEntryUpdateTaskClass;
    task_remove(obj, kind);
}

struct TaskObj *entry_update_task_insert(struct TaskObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->cb18 = gEntryUpdateTaskClass;
    return obj;
}

struct TaskObj *obj_update_oam_task_insert(struct TaskObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->cb18 = gObjUpdateOamTaskClass;
    return obj;
}

ASM_FUNC("asm/code_0803F510/obj_init.s", void obj_init(void))

void obj_refresh_dirty(struct FieldObj *obj)
{
    s32 dirty;

    if (!(obj->flags216 & 0x20))
    {
        cpu_set(obj, &obj->unk28, 0x0400000A);
        dirty = obj->unk34 != obj->unkC;
        if (obj->unk38 != obj->unk10)
        {
            dirty |= 2;
        }
        if (obj->unk3C != obj->unk14)
        {
            dirty |= 4;
        }
        if (obj->unk40 != obj->unk18)
        {
            dirty |= 8;
        }
        ((struct Flags213Dirty *)&obj->flags213)->dirty = dirty & 0xF;
        obj->unk50 = obj->sub->posX;
        obj->unk52 = obj->sub->posY;
    }
}

ASM_FUNC("asm/code_0803F510/motion_obj_set_move_target.s", void motion_obj_set_move_target(void))

void obj_init_motion(struct FieldObj *obj, s32 goal, s32 step, s32 speed)
{
    s32 *p248;
    s32 *p254;
    u16 *p23E;
    s32 dir;

    p248 = &obj->unk248;
    if (step == -1)
    {
        *p248 = obj->unk266;
    }
    else
    {
        *p248 = step;
    }
    p254 = &obj->unk254;
    if (goal == -1)
    {
        *p254 = obj->unk264;
    }
    else
    {
        *p254 = goal;
    }
    obj->unk258 = obj->unk254;
    obj->counter25C = 0;
    obj->unk24C = obj->unk14 + obj->unk18;
    obj->unk250 = obj->unk18;
    p23E = &obj->unk23E;
    obj->unk240 = *p23E;
    if (speed == -1)
    {
        *p23E = obj->unk262;
    }
    else
    {
        *p23E = speed;
    }
    dir = 0;
    if (obj->unk258 > 0)
    {
        dir = 1;
    }
    ((struct Flags213Bits *)&obj->flags213)->bit5 = dir;
}

void motion_obj_step_motion(struct FieldObj *obj)
{
    s32 *counter;

    counter = &obj->counter25C;
    if (*counter >= 0)
    {
        obj->unk18 += obj->unk258;
        obj->unk258 -= obj->unk248;
        if (!(obj->flags20D & 0x20))
        {
            if (obj->unk18 <= 0)
            {
                obj_reset_move_state(obj);
                obj->unk18 = 0;
                return;
            }
        }
        else
        {
            if (obj->unk14 + obj->unk18 <= obj->unk24C)
            {
                obj_reset_move_state(obj);
                if (obj->unk14 + obj->unk18 < 0)
                {
                    obj->unk18 = -obj->unk14;
                }
                return;
            }
        }
        *counter += 1;
    }
}

NONMATCH("asm/code_0803F510/obj_update_state.s", void obj_update_state(struct FieldObj *obj))
{
    s32 nextState = 2;
    s32 a;
    s32 b;
    s32 y;

    if (obj->flags20C & 1)
    {
        if (!(obj->flags216 & 0x20))
        {
            if (obj->counter25C >= 0)
            {
                a = obj->unk14 + obj->unk18;
                b = obj->unk3C + obj->unk40;
                obj->state4 = (a < b) ? 3 : nextState;
            }
            else
            {
                if (obj->unk242 != 0 || (y = obj->unk244) != 0)
                {
                    obj->state4 = 1;
                }
                else
                {
                    obj->state4 = y;
                }
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0803F510/obj_create_sub.s", void obj_create_sub(void))

void obj_update_special_flag(struct FieldObj *obj)
{
    u32 special;
    u32 flags;
    u8 *pf;

    special = 0;
    if ((u16)(obj->id218 - 0x1001) <= 1 || obj->id218 == 0x1028)
    {
        special = 1;
    }
    pf = &obj->flags216;
    special <<= 7;
    flags = (*pf & 0x7F) | special;
    *pf = flags;
    setup_obj_10bc(obj->sub, 0, obj->id218, (flags >> 1) & 1, -1, -1, -1, obj->extra21E);
}

ASM_FUNC("asm/code_0803F510/obj_refresh_anim_state.s", void obj_refresh_anim_state(void))

void obj_copy_sub_fields(struct FieldObj *obj)
{
    u32 v;

    v = ((struct SubBits11 *)obj->sub)->bit6;
    ((struct Flags298Bits *)&obj->flags298)->bit1 = v;
    v = ((struct SubBits1F *)obj->sub)->lo2;
    ((struct Flags298Bits *)&obj->flags298)->f2 = v;
    v = ((struct SubBits1F *)obj->sub)->mid2;
    ((struct Flags298Bits *)&obj->flags298)->f4 = v;
    v = ((struct SubBits12 *)obj->sub)->pair12;
    ((struct Flags298Bits *)&obj->flags298)->f6 = v;
    v = ((struct SubBits12 *)obj->sub)->bit3;
    ((struct Flags299Bits *)&obj->flags299)->bit0 = v;
    v = ((struct SubBits12 *)obj->sub)->bit4;
    ((struct Flags299Bits *)&obj->flags299)->bit1 = v;
    obj->unk29B = obj->sub->unk21;
    obj->unk29C = obj->sub->unk23;
    obj->unk29E = obj->sub->unk2A;
    obj->unk2A0 = obj->sub->unk2C;
    obj->unk2A2 = obj->sub->unk4;
    obj->unk2A4 = obj->sub->unk6;
    ((struct Flags298Bits *)&obj->flags298)->bit0 = 1;
}

NONMATCH("asm/code_0803F510/obj_refresh_motion_state.s", void obj_refresh_motion_state(struct FieldObj *obj, u32 force))
{
    s32 flag;
    s32 moving;
    u32 pair;
    u32 idx;
    u32 t;
    u32 b216;
    struct FieldVtable *vt;
    u8 *tbl;
    u32 k7;

    k7 = 7;
    if ((*(u32 *)((u8 *)obj + 0x20C) & (u32)gUnk_02000001) == (u32)gUnk_02000001)
    {
        b216 = obj->flags216;
        if (!(b216 & 0x20))
        {
            if ((((u32)obj->state214 << 29) >> 29) > 2)
            {
                tbl = (u8 *)gSpriteFacingTable;
                idx = obj->unk02 >> 4;
                if ((b216 & 0xC) != 4)
                {
                    idx += 0x10;
                }
                t = tbl[idx] & k7;
                ((struct Lo3Bits *)&obj->unk24)->lo3 = t;
            }
            flag = force;
            moving = 0;
            switch (((u32)obj->state214 << 29) >> 29)
            {
            case 0:
            case 1:
            case 4:
                if (((u32)obj->unk24 << 29) != ((u32)obj->unk4C << 29))
                {
                    flag = 1;
                }
                if ((obj->state214 & 7) == 0)
                {
                    moving = 1;
                }
                break;
            case 2:
            case 3:
            case 5:
                if (obj->state4 != obj->unk2C)
                {
                    flag = 1;
                }
                else if (((u32)obj->unk24 << 29) != ((u32)obj->unk4C << 29))
                {
                    flag = 1;
                }
                if (obj->unk242 == 0 && obj->unk244 == 0)
                {
                    moving = 1;
                }
                else if ((obj->state214 & 7) == 5)
                {
                    moving = 1;
                }
                break;
            }
            if (flag != 0)
            {
                pair = ((struct SubBits12 *)obj->sub)->pair12;
                vt = obj->vt338;
                vt->fn3C((u8 *)obj + vt->off38, 1);
                ((struct SubBits12 *)obj->sub)->pair12 = pair;
            }
            ((struct SubBits12 *)obj->sub)->bit4 = moving;
        }
    }
}
END_NONMATCH

void obj_sync_sub_state(struct FieldObj *obj, u8 force)
{
    u32 b;
    u8 want;
    u32 dir;
    u8 t;

    b = obj->flags216;
    if (!(b & 0xC))
    {
        want = obj->sub->unk21;
        dir = 0;
    }
    else
    {
        t = gFrameAttrTable[((((b << 28) >> 30) << 4)) + ((((b << 27) >> 31) << 3)) + (((u32)obj->unk24 << 29) >> 29)];
        want = t & 0x7F;
        dir = t >> 7;
    }
    if (obj->sub->unk21 != want || ((struct SubBits11 *)obj->sub)->bit6 != dir || force != 0)
    {
        obj_load_record(obj->sub, want, 0, 0, 0);
        ((struct SubBits11 *)obj->sub)->bit6 = dir;
    }
}

ASM_FUNC("asm/code_0803F510/obj_update_screen_pos.s", void obj_update_screen_pos(void))
