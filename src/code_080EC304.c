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

struct EntryKindFC
{
    u32 kind : 8;
    u32 rest : 24;
} __attribute__((packed));

/* One of the 4 script threads at the start of gEffectState (0xA8 bytes each) */
struct ScriptThread
{
    u8 *start;                  /* the thread's script */
    u8 *pc;                     /* where it reads next; NULL while unused */
    u8 filler8[0xA0 - 0x8];
    u16 flags;                  /* bit 0: running */
    u8 fillerA2[0xA4 - 0xA2];
    u8 kind;
    u8 fillerA5[0xA8 - 0xA5];
};

struct CmdFlags55A
{
    u8 lo6 : 6;
    u8 hi2 : 2;
} __attribute__((packed));

/* the object's flag bytes +0x20C..+0x217 as seen by apply_object_command */
struct ObjCmdBits
{
    u8 filler0[0x20C];
    u32 b20C_0 : 4;
    u32 b20C_4 : 1;
    u32 b20C_5 : 1;
    u32 b20C_6 : 1;
    u32 b20C_7 : 1;
    u32 b20D_0 : 1;
    u32 b20D_1 : 1;
    u32 b20D_2 : 1;
    u32 b20D_3 : 1;
    u32 b20D_4 : 1;
    u32 b20D_5 : 1;
    u32 b20D_6 : 1;
    u32 b20D_7 : 1;
    u32 b20E_0 : 2;
    u32 b20E_2 : 1;
    u32 b20E_3 : 5;
    u32 b20F_0 : 1;
    u32 b20F_1 : 1;
    u32 b20F_2 : 1;
    u32 b20F_3 : 5;
    u32 b210_0 : 1;
    u32 b210_1 : 2;
    u32 b210_3 : 1;
    u32 b210_4 : 1;
    u32 b210_5 : 3;
    u32 b211_0 : 1;
    u32 b211_1 : 7;
    u32 b212 : 16;
    u32 b214_0 : 13;
    u32 b214_13 : 4;
    u32 b214_17 : 15;
};

struct SpriteBits10
{
    u8 filler0[0x10];
    u32 lo10 : 10;
    u32 prio : 2;
    u32 hi20 : 20;
};

extern u8 *gRecordPtrTable[];
void obj_set_active(u8 *obj, s32 value);
void obj_set_sprite_link(u8 *obj, s32 value);

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

/*
 * Script command: restarts script thread args[0] (0, 1, 2, 3 are threads 0, 3, 1, 2) from the start
 * of its script and sets it running, rewinds every other active thread of the same kind, and unbinds
 * the owner's entries bound to that kind (kind 0xFF, flag bit 3 of +0xF8 cleared). Always returns 1.
 */
u32 reset_state_slot_kind(u8 *owner, struct ScriptThread *thread, s32 *args)
{
    struct ScriptThread *other;
    u8 *list;
    u8 **entries;
    u8 kind;
    s16 i;

    switch (*args)
    {
    case 0:
        kind = 0;
        thread = (struct ScriptThread *)gEffectState;
        break;
    case 1:
        kind = 3;
        thread = (struct ScriptThread *)gEffectState + 3;
        break;
    case 2:
        kind = 1;
        thread = (struct ScriptThread *)gEffectState + 1;
        break;
    case 3:
        kind = 2;
        thread = (struct ScriptThread *)gEffectState + 2;
        break;
    }
    thread->pc = thread->start;
    thread->flags = 1;
    other = (struct ScriptThread *)gEffectState;
    for (i = 4; i > 0; i--)
    {
        if (other->kind == kind && other->pc != 0)
            other->pc = other->start;
        other++;
    }
    /* the owner's entry list: count at +0x10B, entry pointers from +0x28 */
    list = *(u8 **)(owner + 0x14);
    i = list[0x10B];
    entries = (u8 **)(list + 0x28);
    for (; i > 0; i--, entries++)
    {
        u32 mask = 0xFFF7;
        u8 *entry = *entries;
        if (entry[0xFC] == kind)
        {
            ((struct EntryKindFC *)(entry + 0xFC))->kind = 0xFF;
            *(u16 *)(entry + 0xF8) &= mask;
        }
    }
    return 1;
}

/*
 * Script command: args[0] picks one of the object's settings and args[1] is the value to give it
 * (mostly single flag bits at obj+0x20C..+0x217). Objects 0 and 1 (obj[0x54], the bros) also mark
 * themselves in the two-bit mask at gEffectState[0x55A] when their visibility flags change.
 * Always returns 1 (the script goes on).
 */
u32 apply_object_command(u8 *owner, u8 *obj, u32 unused, s32 *args)
{
    switch (*args++)
    {
    case 1: /* set active, then the sprite link */
        obj_set_active(obj, *args);
    case 0: /* set the sprite link */
        obj_set_sprite_link(obj, *args);
        break;
    case 2:
        ((struct ObjCmdBits *)obj)->b20D_3 = *args;
        break;
    case 3: /* 0: index 0xE, 1/2: step the index with that value - 1 */
        switch (*args)
        {
        case 0:
            ((struct ObjCmdBits *)obj)->b214_13 = 0xE;
            break;
        case 1:
            obj_step_index(obj, 0, 0);
            break;
        case 2:
            obj_step_index(obj, 0, 1);
            break;
        }
        break;
    case 4: /* turn the flags on as the object's record has them, or all off */
    {
        u8 *table = *(gRecordPtrTable + *(u16 *)(*(u8 **)(owner + 0x14) + 0x126));
        u8 *record = table - ((table[7] << 8) | table[6]) + obj[0x54] * 0x14;

        if (obj[0x54] <= 1)
        {
            {
                u8 *broMask = gEffectState + 0x55A;
                u32 bros = *broMask >> 6;
                bros |= 1 << obj[0x54];
                bros <<= 6;
                *broMask = (*broMask & 0x3F) | bros;
            }
            ((struct ObjCmdBits *)obj)->b20C_5 = 0;
            ((struct ObjCmdBits *)obj)->b20C_6 = 0;
            ((struct ObjCmdBits *)obj)->b20C_7 = 0;
        }
        else
        {
            ((struct ObjCmdBits *)obj)->b20C_5 = *args ? (record[0x10] >> 5) & 1 : 0;
            ((struct ObjCmdBits *)obj)->b20C_6 = *args ? (record[0x10] >> 6) & 1 : 0;
            ((struct ObjCmdBits *)obj)->b20C_7 = *args ? (record[0x10] >> 7) & 1 : 0;
        }
        ((struct ObjCmdBits *)obj)->b20D_0 = *args ? record[0x11] & 1 : 0;
        ((struct ObjCmdBits *)obj)->b20D_1 = *args ? (record[0x11] >> 1) & 1 : 0;
        break;
    }
    case 5: /* set the same flags to the value */
        if (obj[0x54] <= 1)
        {
            u8 *broMask = gEffectState + 0x55A;
            u32 bros = *broMask >> 6;
            bros |= 1 << obj[0x54];
            bros <<= 6;
            *broMask = (*broMask & 0x3F) | bros;
        }
        ((struct ObjCmdBits *)obj)->b20C_6 = *args;
        ((struct ObjCmdBits *)obj)->b20C_7 = *args;
        ((struct ObjCmdBits *)obj)->b20D_0 = *args;
        ((struct ObjCmdBits *)obj)->b20D_1 = *args;
        break;
    case 6:
        ((struct ObjCmdBits *)obj)->b20F_0 = *args;
        break;
    case 7:
        ((struct ObjCmdBits *)obj)->b20D_6 = *args;
        break;
    case 8:
        ((struct ObjCmdBits *)obj)->b20D_5 = *args;
        break;
    case 9:
        ((struct ObjCmdBits *)obj)->b20F_2 = *args;
        break;
    case 10:
        ((struct ObjCmdBits *)obj)->b20E_2 = *args;
        break;
    case 11:
        obj_set_active(obj, *args);
        break;
    case 12: /* the sprite's priority */
        ((struct SpriteBits10 *)*(u8 **)(obj + 0x294))->prio = *args;
        break;
    case 13:
        ((struct ObjCmdBits *)obj)->b210_0 = *args;
        break;
    case 14:
        ((struct ObjCmdBits *)obj)->b210_4 = *args;
        break;
    case 15:
        ((struct ObjCmdBits *)obj)->b20C_4 = *args;
        break;
    case 16:
        ((struct ObjCmdBits *)obj)->b210_3 = *args;
        break;
    case 17:
        ((struct ObjCmdBits *)obj)->b211_0 = *args;
        break;
    case 18: /* bit 4 of obj+0x33D */
        if (*args != 0)
        {
            u8 *flags = obj + 0x33D;
            u32 hi = *flags >> 4;
            hi |= 1;
            hi <<= 4;
            *flags = (*flags & 0xF) | hi;
        }
        else
        {
            u8 *flags = obj + 0x33D;
            u32 hi = *flags >> 4;
            hi &= ~1;
            hi <<= 4;
            *flags = (*flags & 0xF) | hi;
        }
        break;
    }
    return 1;
}

/*
 * Script command on an object: args[0] picks what to do, args[2] is the main operand
 * and args[1] a second one. 0: step the object's index toward the owner's object number
 * args[2] (value args[1]); 1: set its mode (args[1] != 0) or the half at +0x23E from
 * args[2] (bit 6 set: the low 4 bits as they are, else the signed low bits scaled up);
 * 2: set the flags args[2] selects (bits 4..0: +0x20C bits 5-7, +0x20D bits 0-1) to
 * args[1]; 3: the mode bits of +0x20C; 4: the half at +0x270.
 * Always returns 1 (the script goes on).
 */
u32 dispatch_object_command(u8 *owner, u8 *obj, u32 unused, u32 *args)
{
    s32 operand = args[2];

    switch (*args++)
    {
    case 0:
        obj_step_index(obj, ((u8 **)(*(u8 **)(owner + 0x14) + 0x28))[operand], args[0]);
        break;
    case 1:
        if (*args++ != 0)
        {
            s32 mode = args[0];
            if ((args[0] & 0x40) != 0)
                mode &= 0xF;
            else
            {
                mode <<= 28;
                mode >>= 24;
            }
            flags_obj_set_mode(obj, mode);
        }
        else
        {
            s32 half = args[0];
            if ((args[0] & 0x40) != 0)
            {
                half &= 0xF;
                half <<= 4;
            }
            else
            {
                half <<= 24;
                half >>= 16;
            }
            set_half_23e(obj, half);
        }
        break;
    case 2:
        /* objects 0 and 1 (the bros) mark themselves in gEffectState[0x55A] bits 6-7 */
        if (obj[0x54] <= 1)
        {
            {
                u8 *broMask = gEffectState + 0x55A;
                u32 bros = *broMask >> 6;
                bros |= 1 << obj[0x54];
                bros <<= 6;
                *broMask = (*broMask & 0x3F) | bros;
            }
        }
        if ((operand >> 4) & 1)
        {
            {
                u32 on = args[0];
                ((struct CmdFlags20C *)(obj + 0x20C))->bit5 = on & 1;
            }
        }
        if ((operand >> 3) & 1)
        {
            {
                u32 on = args[0];
                ((struct CmdFlags20C *)(obj + 0x20C))->bit6 = on & 1;
            }
        }
        if ((operand >> 2) & 1)
        {
            {
                u32 on = args[0];
                u8 *flags = obj + 0x20C;
                on <<= 7;
                *flags = (*flags & 0x7F) | on;
            }
        }
        if ((operand >> 1) & 1)
        {
            {
                u32 on = args[0];
                ((struct CmdFlags20D *)(obj + 0x20D))->bit0 = on & 1;
            }
        }
        if (operand & 1)
        {
            {
                u32 on = args[0];
                ((struct CmdFlags20D *)(obj + 0x20D))->bit1 = on & 1;
            }
        }
        break;
    case 3:
        ((struct CmdFlags20C *)(obj + 0x20C))->mode = operand & 7;
        break;
    case 4:
        *(u16 *)(obj + 0x270) = operand;
        break;
    }
    return 1;
}
