#include "global.h"

extern u8 *gEffectState;

void reset_obj_params_b(u8 *dst, u8 *obj, u32 a, u32 b, u32 c, u32 d);
void apply_key_filter(u8 *list, u32 mask);
void obj_step_index(u8 *obj, u8 *entry, u32 value);
void flags_obj_set_mode(u8 *obj, s32 mode);
void set_half_23e(u8 *obj, s32 v);

struct CmdFlags20C
{
    u8 unk0 : 1;
    u8 mode : 3;
    u8 unk4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

struct CmdFlags20D
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
} __attribute__((packed));

struct CmdFlags55A
{
    u8 lo6 : 6;
    u8 hi2 : 2;
} __attribute__((packed));

ASM_FUNC("asm/code_080EC304/obj_command_dispatch.s", void obj_command_dispatch(void))

ASM_FUNC("asm/code_080EC304/effect_entry_step.s", void effect_entry_step(void))

struct FlagBit0
{
    u8 bit0 : 1;
    u8 rest : 7;
};

NONMATCH("asm/code_080EC304/register_effect_slot.s", u32 register_effect_slot(u8 *owner, u32 unused, u32 *args))
{
    s32 kind = *args++;

    switch (kind)
    {
    case 0:
    {
        u8 *slot = gEffectState;
        reset_obj_params_b(owner + 0x1C, slot, *args, 0, 1, 0xFF);
        break;
    }
    case 1:
    {
        u8 *slot = gEffectState + 0x1F8;
        reset_obj_params_b(owner + 0x1C, slot, *args, 0, kind, 0xFF);
        break;
    }
    case 2:
    {
        u8 *state = gEffectState;
        u8 *slot = state + 0xA8;
        if ((state[0x148] ^ 1) & 1)
        {
            u8 *list;
            reset_obj_params_b(owner + 0x1C, slot, *args, 0, 1, 0xFF);
            list = *(u8 **)(owner + 0x14);
            if (((u8 *)*(u8 **)(list + 0x28))[0x20D] & 1)
            {
                {
                    u8 *st = gEffectState + 0x55A;
                    u32 hi = *st >> 6;
                    hi &= 0xFFFE;
                    hi <<= 6;
                    *st = (*st & 0x3F) | hi;
                }
                ((struct FlagBit0 *)((u8 *)*(u8 **)(list + 0x28) + 0x20D))->bit0 = 0;
            }
            if (((u8 *)*(u8 **)(list + 0x2C))[0x20D] & 1)
            {
                {
                    u8 *st = gEffectState + 0x55A;
                    u32 hi = *st >> 6;
                    hi &= 0xFFFD;
                    hi <<= 6;
                    *st = (*st & 0x3F) | hi;
                }
                ((struct FlagBit0 *)((u8 *)*(u8 **)(list + 0x2C) + 0x20D))->bit0 = 0;
            }
            kind &= gEffectState[0x55A];
            if (kind == 0)
            {
                apply_key_filter(*(u8 **)(owner + 0x14), 0x3FF);
            }
            break;
        }
        /* fall through */
    }
    case 3:
    {
        u8 *state = gEffectState;
        u8 *slot = state + 0x150;
        if (((state[0x1F0] ^ 1) & 1) && (state[0x55A] & 1))
        {
            reset_obj_params_b(owner + 0x1C, slot, *args, 0, 1, 0xFF);
        }
        break;
    }
    }
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080EC304/reset_state_slot_kind.s", void reset_state_slot_kind(void))

ASM_FUNC("asm/code_080EC304/apply_object_command.s", void apply_object_command(void))

u32 dispatch_object_command(u8 *owner, u8 *obj, u32 unused, u32 *args)
{
    s32 val = args[2];

    switch (*args++)
    {
    case 0:
        obj_step_index(obj, ((u8 **)(*(u8 **)(owner + 0x14) + 0x28))[val], args[0]);
        break;
    case 1:
        if (*args++ != 0)
        {
            s32 arg = args[0];
            if ((args[0] & 0x40) != 0)
                arg &= 0xF;
            else
            {
                arg <<= 28;
                arg >>= 24;
            }
            flags_obj_set_mode(obj, arg);
        }
        else
        {
            s32 arg = args[0];
            if ((args[0] & 0x40) != 0)
            {
                arg &= 0xF;
                arg <<= 4;
            }
            else
            {
                arg <<= 24;
                arg >>= 16;
            }
            set_half_23e(obj, arg);
        }
        break;
    case 2:
        if (obj[0x54] <= 1)
        {
            {
                u8 *st = gEffectState + 0x55A;
                u32 hi = *st >> 6;
                hi |= 1 << obj[0x54];
                hi <<= 6;
                *st = (*st & 0x3F) | hi;
            }
        }
        if ((val >> 4) & 1)
        {
            {
                u32 v = args[0];
                ((struct CmdFlags20C *)(obj + 0x20C))->bit5 = v & 1;
            }
        }
        if ((val >> 3) & 1)
        {
            {
                u32 v = args[0];
                ((struct CmdFlags20C *)(obj + 0x20C))->bit6 = v & 1;
            }
        }
        if ((val >> 2) & 1)
        {
            {
                u32 v = args[0];
                u8 *p = obj + 0x20C;
                v <<= 7;
                *p = (*p & 0x7F) | v;
            }
        }
        if ((val >> 1) & 1)
        {
            {
                u32 v = args[0];
                ((struct CmdFlags20D *)(obj + 0x20D))->bit0 = v & 1;
            }
        }
        if (val & 1)
        {
            {
                u32 v = args[0];
                ((struct CmdFlags20D *)(obj + 0x20D))->bit1 = v & 1;
            }
        }
        break;
    case 3:
        ((struct CmdFlags20C *)(obj + 0x20C))->mode = val & 7;
        break;
    case 4:
        *(u16 *)(obj + 0x270) = val;
        break;
    }
    return 1;
}
