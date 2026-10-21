#include "global.h"

extern u8 *gEffectState;

void reset_obj_params_b(u8 *dst, u8 *obj, u32 a, u32 b, u32 c, u32 d);
void apply_key_filter(u8 *list, u32 mask);

ASM_FUNC("asm/code_080EC304/obj_command_dispatch.s", void obj_command_dispatch(void))

ASM_FUNC("asm/code_080EC304/effect_entry_step.s", void effect_entry_step(void))

struct Flags55A
{
    u8 lo6 : 6;
    u8 hi2 : 2;
};

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
        if ((gEffectState[0x148] ^ 1) & 1)
        {
            u8 *list;
            reset_obj_params_b(owner + 0x1C, gEffectState + 0xA8, *args, 0, 1, 0xFF);
            list = *(u8 **)(owner + 0x14);
            if (((u8 *)*(u8 **)(list + 0x28))[0x20D] & 1)
            {
                ((struct Flags55A *)(gEffectState + 0x55A))->hi2 &= 0xFE;
                ((struct FlagBit0 *)((u8 *)*(u8 **)(list + 0x28) + 0x20D))->bit0 = 0;
            }
            if (((u8 *)*(u8 **)(list + 0x2C))[0x20D] & 1)
            {
                ((struct Flags55A *)(gEffectState + 0x55A))->hi2 &= 0xFD;
                ((struct FlagBit0 *)((u8 *)*(u8 **)(list + 0x2C) + 0x20D))->bit0 = 0;
            }
            if ((kind & gEffectState[0x55A]) == 0)
            {
                apply_key_filter(*(u8 **)(owner + 0x14), 0x3FF);
            }
            break;
        }
        /* fall through */
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

ASM_FUNC("asm/code_080EC304/dispatch_object_command.s", void dispatch_object_command(void))

