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

NONMATCH("asm/code_0803F510/obj_refresh_dirty.s", void obj_refresh_dirty(struct FieldObj *obj))
{
    s32 dirty;
    s32 flags;

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
        dirty = (dirty & 0xF) << 1;
        flags = obj->flags213;
        flags &= ~0x1E;
        obj->flags213 = flags | dirty;
        obj->unk50 = obj->sub->posX;
        obj->unk52 = obj->sub->posY;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0803F510/motion_obj_set_move_target.s", void motion_obj_set_move_target(void))

NONMATCH("asm/code_0803F510/obj_init_motion.s", void obj_init_motion(struct FieldObj *obj, s32 goal, s32 step, s32 speed))
{
    s32 *p248;
    s32 *p254;
    u16 *p23E;
    s32 dir;
    u32 bits;
    s32 flags;
    u8 *pf;

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
    pf = &obj->flags213;
    dir <<= 5;
    flags = *pf;
    flags = ~0x20 & flags;
    *pf = flags | dir;
}
END_NONMATCH

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

ASM_FUNC("asm/code_0803F510/obj_update_state.s", void obj_update_state(void))

ASM_FUNC("asm/code_0803F510/obj_create_sub.s", void obj_create_sub(void))

ASM_FUNC("asm/code_0803F510/obj_update_special_flag.s", void obj_update_special_flag(void))

ASM_FUNC("asm/code_0803F510/obj_refresh_anim_state.s", void obj_refresh_anim_state(void))

ASM_FUNC("asm/code_0803F510/obj_copy_sub_fields.s", void obj_copy_sub_fields(void))

ASM_FUNC("asm/code_0803F510/obj_refresh_motion_state.s", void obj_refresh_motion_state(void))

ASM_FUNC("asm/code_0803F510/obj_sync_sub_state.s", void obj_sync_sub_state(void))

ASM_FUNC("asm/code_0803F510/obj_update_screen_pos.s", void obj_update_screen_pos(void))
