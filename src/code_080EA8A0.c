#include "global.h"

struct ScriptObj
{
    u32 word0;
    u32 word4;
    u32 word8;
    u32 wordC;
    u32 word10;
    u8 filler14[0x58 - 0x14];
    u32 entries[4];
    u8 filler68[0x98 - 0x68];
    u32 word98;
    u32 word9C;
    u16 halfA0;
    u16 halfA2;
    u8 byteA4;
    u8 byteA5;
    u8 countA6;
};

struct IdleRequest
{
    u32 flag;
    u16 a;
    u8 filler6[2];
    u16 b;
    u8 fillerA[2];
    u32 c;
};

struct RampRequest
{
    u32 flag;
    u8 byte4;
    u8 filler5[3];
    u8 byte8;
};

struct CmpRecord
{
    u8 op;
    u8 filler1[3];
    u32 a;
    u32 value;
};

struct CmpMatchRecord
{
    u8 op;
    u8 filler1[3];
    u32 a;
    u32 b;
    u32 expect;
    u32 value;
};

struct BranchState
{
    u32 word0;
    u8 filler4[0x10];
    u32 word14;
};

struct ListEntry
{
    u8 filler0[0xC];
    u8 id;
    u8 filler1[2];
    u8 active;
};

struct KeyRam
{
    u8 filler0[0x28];
    u16 keys28;
    u16 keys2A;
    u8 filler2C[2];
    u16 keys2E;
    u8 filler30[0x78 - 0x30];
    u16 enable78;
    u16 setByGame7A;
    u16 oneFrame7C;
    u8 filler7E[0x8C0 - 0x7E];
    u8 lang8C0;
};

struct KeyGroupRecord
{
    s16 index;
    u8 filler2[2];
    u16 group;
};

struct VarCmd
{
    s16 index;
    u8 filler2[2];
    u32 word4;
};

struct FieldObj
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10[2];
    u16 unk12;
    u16 unk14;
    u8 filler16[2];
    u16 unk18;
    u8 filler1A[0x3C - 0x1A];
};

struct FlagRec
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
};

struct TextItemRec
{
    u8 byte0;
    u8 filler1[3];
    s16 half4;
    u8 filler6[2];
    s16 half8;
    s32 wordC;
    u16 half10;
    u8 filler12[2];
    u16 half14;
    u8 filler16[2];
    u8 byte18;
    u8 filler19[3];
    u32 word1C;
};

struct KeyMatchRecord
{
    u32 flag;
    u32 kind;
    u32 mask;
    u32 value;
};

/* Operators of the script's binary-op command. */
enum BinaryOp
{
    BINOP_NONE,
    BINOP_ADD,
    BINOP_SUB,
    BINOP_MUL,
    BINOP_DIV,
    BINOP_MOD,
    BINOP_SHL,
    BINOP_SHR,
    BINOP_AND,
    BINOP_OR,
    BINOP_XOR,
    BINOP_LOGICAL_NOT,
    BINOP_BITWISE_NOT
};

/* Operators of the script's conditions (compare_op): true when `a OP b` holds. 6 and 7 are
   never true. */
enum CompareOp
{
    CMP_EQ,           /* a == b */
    CMP_NE,           /* a != b */
    CMP_LT,           /* a < b, signed */
    CMP_GT,           /* a > b, signed */
    CMP_LE,           /* a <= b, signed */
    CMP_GE,           /* a >= b, signed */
    CMP_OR = 8,       /* a or b is nonzero */
    CMP_AND,          /* a and b share a bit */
    CMP_XOR,          /* a and b differ */
    CMP_LOGICAL_NOT,  /* a is 0 (b unused) */
    CMP_BITWISE_NOT   /* ~a is nonzero, a isn't 0xFFFFFFFF (b unused) */
};

/* Arguments of the binary-op command: var = value OP operand. */
struct BinaryOpArgs
{
    s16 var;
    u8 filler2[2];
    u32 op;
    u32 value;
    u32 operand;
};

extern u8 *gUnk_03000FB8;
void set_link_slot_mask(u32 mask);
void serial_schedule(u32 a, u32 b, u32 c);
void set_ramp0_target(u32 a, u32 b);
extern struct KeyRam gSys;
extern u8 *gCurObj;
extern u8 *gParty;
void draw_text_prim(void *a, u32 b, s32 c, s32 d, u32 e, u32 f, u32 g, u32 h, u32 i, u32 j);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 value);
u32 compare_op(void *ctx, u32 op, u32 a, u32 b);
u32 atan2_angle(s32 x, s32 y);
void set_var(u32 a, void *b, void *c, u32 d, s32 e, u32 f);
void poll_keys(void);
void slot_obj_clear(struct ListEntry **list, u8 index);
void dma_stop_and_reset(void);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void sound_stop_all(void);
u32 sound_is_playing(u32 song);
void bump_record(void *st, u32 a1);
void set_slot_target(u32 a, u32 b, u32 c);
void slot_set_mode(u32 a, u32 b, u32 c);
void set_slot_fields(u32 a, u32 b, u32 c);
void set_slot_fields_45(u32 a, u32 b, u32 c);
void clear_slot_rect(void *obj, u32 idx);
void clear_active_slot_rects(void *obj);
void copy_map_rects(void *obj, u32 arg);
void slot_reset(u32 a);
u32 slot_bit_test(u32 a);
void serial_start_phase_2(void);
void serial_clear_phase(void);
void serial_enter_phase_1(void);
void give_reward(u32 item, u32 amount, u32 flag);

/* Script command: set or clear one of a hero's 9 per-hero flags. args: 0 (anything else does
   nothing), 1 to clear or 0 to set, index 0-8 for Mario or 9-17 for Luigi. Index i is bit i % 3
   of 6-bit set i / 3. The three sets lie just before the open0-2/done0-2 sets of PartyHero
   (code_0807ED70.c), which use the same index layout. */
u32 set_obj_flag(void *ctx, void *unused, u32 *args)
{
  struct
  {
    u8 filler0[7];
    u8 low2 : 2;
    u8 set0 : 6;
    u16 set1 : 6;
    u16 set2 : 6;
  } *hero;
  u16 clear;
  s32 index;
  s16 flag;
  u16 bitInSet;
  u32 bits;
  if ((*(args++)) == 0)
  {
    hero = (void *) gParty;
    clear = *(args++);
    index = (u16) *args;
    if ((s16) index > 8)
    {
      hero = (void *) (((u8 *) hero) + 0x3C); /* Luigi's record */
      index = (s16) index - 9;
      index = (u16) index;
    }
    flag = index;
    if (flag <= 2)
    {
      if (clear)
      {
        hero->set0 &= ~(1 << flag);
      }
      else
      {
        bits = hero->set0;
        bits |= 1 << flag;
        hero->set0 = bits;
      }
    }
    else if (flag <= 5)
    {
      bitInSet = flag - 3;
      if (clear)
        hero->set1 &= ~(1 << ((s16) bitInSet));
      else
        hero->set1 |= 1 << ((s16) bitInSet);
    }
    else
    {
      bitInSet = flag - 6;
      if (clear)
        hero->set2 &= ~(1 << ((s16) bitInSet));
      else
        hero->set2 |= 1 << ((s16) bitInSet);
    }
  }
  return 1;
}

NONMATCH("asm/code_080EA8A0/cmd_update_and_store.s", u32 cmd_update_and_store(void *ctx, struct ScriptObj *obj, u32 *cmd, u32 mid))
{
    u32 *cur;
    u32 index;
    s32 kind;
    u32 sel;
    u32 amount;
    u32 result;
    u8 *addr;

    cur = cmd;
    index = *cur++;
    kind = *cur++;
    switch (kind)
    {
    case 0:
        sel = *cur++;
        switch (((s32)sel >> 7) - 1)
        {
        case 0:
            addr = gParty + 0xBA + (sel & 0x7F);
            break;
        case 1:
            addr = gParty + 0xF2 + (sel & 0x7F);
            break;
        case 3:
            addr = gParty + 0x8A + (sel & 0x7F);
            break;
        case 2:
            addr = gParty + 0xAA + (sel & 0x7F);
            break;
        case 5:
            addr = gParty + 0x12A + (sel & 0x7F);
            break;
        case 4:
            cur--;
            goto bit_path;
        }
        {
            u32 mode;
            u8 cell = *addr;

            amount = (cell == 0xFF) ? 0 : cell;
            mode = *cur++;
            switch (mode)
            {
            case 0:
                {
                    u32 delta = *cur;

                    amount = (u16)((s16)amount + delta);
                    give_reward((u8)((s32)sel >> 7), sel & 0x7F, (u8)delta);
                    result = *addr == (s16)amount;
                }
                break;
            case 1:
                {
                    u32 diff = ((s16)amount - *cur) << 16;

                    amount = diff >> 16;
                    if ((s32)diff < 0)
                    {
                        *addr = 0;
                        result = 0;
                    }
                    else
                    {
                        *addr = amount;
                        result = 1;
                    }
                }
                break;
            }
        }
        break;
    case 1:
        {
            u32 sub;

            cur++;
            sub = *cur++;
            switch (sub)
            {
            case 0:
                {
                    s32 sum = *(u16 *)(gParty + 0x88) + *cur;

                    amount = (u16)sum;
                    if ((s16)sum > 9999)
                    {
                        amount = 9999;
                        result = 0;
                    }
                    else
                    {
                        result = 1;
                    }
                }
                break;
            case 1:
                {
                    u32 diff = (*(u16 *)(gParty + 0x88) - *cur) << 16;

                    amount = diff >> 16;
                    if ((s32)diff < 0)
                    {
                        amount = 0;
                        result = 0;
                    }
                    else
                    {
                        result = 1;
                    }
                }
                break;
            }
            *(u16 *)(gParty + 0x88) = amount;
        }
        break;
    case 2:
    bit_path:
        {
            u32 src = cur[0];
            u32 mode = cur[1];
            s32 bitsrc = src;

            addr = gParty + 0xB2 + (((s32)(src & 0x7F)) >> 3);
            if ((s32)src < 0)
            {
                bitsrc = src + 7;
            }
            amount = (0x10000u << (src - ((bitsrc >> 3) << 3))) >> 16;
            switch (mode)
            {
            case 0:
                {
                    u32 cell = *addr;

                    if (((s16)amount & cell) != 0)
                    {
                        result = 0;
                    }
                    else
                    {
                        *addr = cell | amount;
                        result = 1;
                    }
                }
                break;
            case 1:
                {
                    u32 cell = *addr;

                    if (((s16)amount & cell) == 0)
                    {
                        result = 0;
                    }
                    else
                    {
                        *addr = cell & ~amount;
                        result = 1;
                    }
                }
                break;
            }
        }
        break;
    }

    if ((s32)index >= 0)
    {
        set_var(mid, (u8 *)obj + 0x18, 0, 0, (s16)index, result);
    }
    return 1;
}
END_NONMATCH

/* Script command: reads a party inventory value into script variable args[0]. args[1] is 0xFF
   for the coins (the u16 at gParty + 0x88), else kind << 7 | slot: kinds 1-4 and 6 are count
   tables at gParty + 0xBA, 0xF2, 0xAA, 0x8A and 0x12A, kind 5 the bit flags at gParty + 0xB2.
   A count of 0xFF (empty) reads as 0. */
u32 cmd_read_store(void *ctx, struct ScriptObj *obj, u32 *args, u32 mid)
{
    u32 value;
    u32 varIndex = *args++;

    if (*args == 0xFF)
    {
        value = *(u16 *)(gParty + 0x88);
    }
    else
    {
        switch (((s32)*args >> 7) - 1)
        {
        case 0:
            value = gParty[0xBA + (*args & 0x7F)];
            break;
        case 1:
            value = gParty[0xF2 + (*args & 0x7F)];
            break;
        case 3:
            value = gParty[0x8A + (*args & 0x7F)];
            break;
        case 2:
            value = gParty[0xAA + (*args & 0x7F)];
            break;
        case 5:
            value = gParty[0x12A + (*args & 0x7F)];
            break;
        case 4:
        {
            s32 item = *args;
            s32 rounded;
            u32 flagByte;

            /* bit (item % 8) of byte (item & 0x7F) / 8 */
            flagByte = gParty[0xB2 + ((item & 0x7F) >> 3)];
            rounded = item;
            if (item < 0)
            {
                rounded = item + 7;
            }
            value = ((s32)flagByte >> (item - ((rounded >> 3) << 3))) & 1;
            break;
        }
        }
        if (value == 0xFF)
        {
            value = 0;
        }
    }
    set_var(mid, (u8 *)obj + 0x18, 0, 0, (s16)varIndex, value);
    return 1;
}

struct CounterRec
{
    u32 word0;
    u8 op4;
    u8 filler5[3];
    u32 cmp8;
    u32 value;
};

NONMATCH("asm/code_080EA8A0/counter_step_and_branch.s", u32 counter_step_and_branch(void *ctx, struct ScriptObj *obj, struct CounterRec *rec))
{
    u8 *count = &obj->countA6;
    u32 *entry;
    u32 mode;
    u32 result;

    entry = (u32 *)((u8 *)obj + (*count << 2));
    entry = (u32 *)((u8 *)entry + 0x54);
    mode = rec->word0 & ~0x18;
    mode = (u8)mode;

    if (mode == 3)
    {
        *entry = *entry + 1;
    }
    else if (mode == 4)
    {
        *entry = *entry - 1;
    }
    result = (u8)compare_op(ctx, rec->op4, rec->cmp8, *entry);
    if (result != 0)
    {
        obj->word0 = rec->value;
        if ((rec->word0 & 0x18) == 0x10)
        {
            (*count)--;
            *entry = 0;
            return 1;
        }
    }
    else if ((rec->word0 & 0x18) == 0x18)
    {
        (*count)--;
        *entry = result;
        return 1;
    }
    if (mode == 1)
    {
        *entry = *entry + 1;
    }
    else if (mode == 2)
    {
        *entry = *entry - 1;
    }
    return 1;
}
END_NONMATCH

void set_obj_params_a(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    obj->word8 = a;
    obj->wordC = b;
    obj->halfA2 = s5;
    obj->byteA5 = s6;
}

void set_obj_params_b(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    obj->word0 = a;
    obj->word4 = b;
    obj->word10 = 0;
    obj->halfA0 = s5;
    obj->byteA4 = s6;
}

void clear_obj_fields(void *ctx, struct ScriptObj *obj, struct ScriptObj *flags)
{
    if ((obj->halfA0 & 8) != 0)
    {
        flags->halfA0 &= 0xFFFB;
    }
    obj->wordC = 0;
    obj->word8 = 0;
    obj->byteA5 = 0;
    obj->halfA2 = 0;
}

void reset_obj_params_a(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    clear_obj_fields(ctx, obj, (struct ScriptObj *)a);
    set_obj_params_a(ctx, obj, a, b, s5, s6);
}

void reset_obj_params_b(void *ctx, struct ScriptObj *obj, u32 a, u32 b, u16 s5, u8 s6)
{
    clear_obj_fields(ctx, obj, (struct ScriptObj *)a);
    set_obj_params_b(ctx, obj, a, b, s5, s6);
}

struct SpriteModeBits
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

struct FlagRecBits
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 b6 : 1;
    u8 b7 : 1;
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

u8 flags_vars_set_obj_state(void *ctx, struct ScriptObj *obj, struct FlagRec *sub, u8 value, u8 bit)
{
    struct FlagRecBits *rec = (struct FlagRecBits *)sub;
    u8 flip = rec->b6;

    rec->b6 = flip ^ bit;
    if (value != 0)
    {
        obj->word9C = value;
        obj->halfA0 |= 0x20;
        rec->mode = 1;
    }
    else
    {
        rec->mode = 0;
    }
    return value == 0;
}

u32 direction_octant(void *ctx, s32 x0, s32 y0, s32 x1, s32 y1)
{
    s32 negX;
    s32 negY;
    s32 t;

    x0 = x1 - x0;
    y0 = y1 - y0;
    negX = 0;
    negY = 0;
    if (x0 < 0)
    {
        negX = 1;
        x0 = -x0;
    }
    if (y0 < 0)
    {
        negY = 1;
        y0 = -y0;
    }
    while (((x0 | y0) & 0xFFFF8000) != 0)
    {
        x0 >>= 1;
        y0 >>= 1;
    }
    if (negX)
        x0 = -x0;
    if (negY)
        y0 = -y0;
    t = atan2_angle(x0, y0) + 0x50;
    if (t < 0)
        t += 0x1F;
    return (t >> 5) & 7;
}

/* Turns a script command's direction and distance (args[0], args[1]) into an X/Y/Z offset:
   directions 0-7 go clockwise from -Y (0 = -Y, 2 = +X, 4 = +Y, 6 = -X, odd ones diagonal),
   8 is +Z and 9 is -Z. A nonzero args[2] scales the offset by 16. ctx isn't used. */
void unpack_stream_values(void *ctx, u32 *args, u32 *outX, u32 *outY, u32 *outZ)
{
    u32 direction;

    *outZ = 0;
    *outY = 0;
    *outX = 0;
    direction = *args++;
    switch (direction)
    {
    case 0:
        *outY = -*args;
        break;
    case 1:
        *outX = *args;
        *outY = -*args;
        break;
    case 2:
        *outX = *args;
        break;
    case 3:
        *outX = *args;
        *outY = *args;
        break;
    case 4:
        *outY = *args;
        break;
    case 5:
        *outX = -*args;
        *outY = *args;
        break;
    case 6:
        *outX = -*args;
        break;
    case 7:
        *outX = -*args;
        *outY = -*args;
        break;
    case 8:
        *outZ = *args;
        break;
    case 9:
        *outZ = -*args;
        break;
    }
    if (args[1] != 0)
    {
        *outX = *outX << 4;
        *outY = *outY << 4;
        *outZ = *outZ << 4;
    }
}

void tick_obj_timer(void *ctx, struct ScriptObj *obj)
{
    s32 t = obj->word9C - 1;

    obj->word9C = t;
    if (t == 0)
    {
        obj->halfA0 &= 0xFFEF;
    }
}

/* Evaluates a script condition: whether `a OP b` holds for the CompareOp op (1 or 0). */
u32 compare_op(void *ctx, u32 op, u32 a, u32 b)
{
    switch ((u8)op)
    {
    case CMP_EQ:
        return a == b;
    case CMP_NE:
        return a != b;
    case CMP_LT:
        return (s32)a < (s32)b;
    case CMP_GT:
        return (s32)a > (s32)b;
    case CMP_LE:
        return (s32)a <= (s32)b;
    case CMP_GE:
        return (s32)a >= (s32)b;
    case CMP_OR:
        return (a | b) != 0;
    case CMP_AND:
        return (a & b) != 0;
    case CMP_XOR:
        return (a ^ b) != 0;
    case CMP_LOGICAL_NOT:
        return a == 0;
    case CMP_BITWISE_NOT:
        return ~a != 0;
    }
    return 0;
}

u32 cmd_set_var_lang(void *ctx, struct ScriptObj *obj, struct VarCmd *cmd, u32 mid)
{
    u32 lang;

    if (cmd->word4 == 0)
    {
        lang = gSys.lang8C0;
    }
    set_var(mid, (u8 *)obj + 0x18, 0, 0, cmd->index, lang);
    return 1;
}

u32 request_if_idle(void *ctx, void *unused, u32 *rec)
{
    u32 flag = *rec++;

    if (flag == 0)
    {
        serial_schedule(((u16 *)rec)[0], ((u16 *)rec)[2], rec[2]);
    }
    return 1;
}

u32 dispatch_obj_mode_call(void *ctx, void *unused, u32 *mode_ptr)
{
    s32 mode = *mode_ptr;

    switch (mode)
    {
    case 0:
        dma_stop_and_reset();
        break;
    case 1:
        serial_start_phase_2();
        break;
    case 2:
        serial_clear_phase();
        break;
    case 3:
        serial_enter_phase_1();
        break;
    }
    return 1;
}

u32 set_obj_if_cmp(void *ctx, struct ScriptObj *obj, struct CmpRecord *rec)
{
    if ((u8)compare_op(ctx, rec->op, rec->a, obj->word98))
    {
        obj->word0 = rec->value;
    }
    return 1;
}

u32 pop_obj_entry(void *ctx, struct ScriptObj *obj)
{
    u8 *count = &obj->countA6;
    u32 idx;

    *count = *count - 1;
    idx = obj->countA6;
    *(u32 *)((idx << 2) + (u32)obj + 0x58) = 0;
    return 1;
}

u32 set_obj_word_98(void *ctx, struct ScriptObj *obj, u32 *src)
{
    obj->word98 = *src;
    return 1;
}

u32 push_obj_entry(void *ctx, struct ScriptObj *obj, u32 *src)
{
    *(u32 *)((u8 *)obj + 0x58 + (obj->countA6 << 2)) = *src;
    obj->countA6++;
    return 1;
}

u32 pick_random_entry(void *ctx, u32 *out, u32 *table)
{
    s32 key = table[0];
    u32 rnd = mt19937_next();
    s32 index = rand_below(key, rnd);
    u32 entry = table[index + 1];

    if (entry != 0)
    {
        *out = entry;
    }
    return 1;
}

u32 random_obj_placement(void *ctx, struct ScriptObj *obj, struct VarCmd *src, u32 mid)
{
    s32 key = src->word4;
    u32 rnd = mt19937_next();
    s32 pick = rand_below(key, rnd);

    set_var(mid, (u8 *)obj + 0x18, 0, 0, src->index, pick);
    return 1;
}

u32 call_b0c0(void *ctx, void *unused, u16 *rec)
{
    set_link_slot_mask(*rec);
    return 1;
}

u32 copy_on_key_match_a(void *ctx, u32 *out, struct KeyMatchRecord *rec)
{
    u32 keys = gSys.keys2E;
    u32 mask = rec->mask;
    u32 want = mask;
    u32 kind = rec->kind + 0xB;

    switch (kind)
    {
    case 0xB:
        mask = 0;
        break;
    case 0xC:
        mask = ~mask;
        want = mask;
        break;
    }
    if ((rec->flag & 1) != 0)
    {
        if ((want & keys) != mask)
        {
            *out = rec->value;
        }
    }
    else if ((want & keys) == mask)
    {
        *out = rec->value;
    }
    return 1;
}

u32 copy_on_key_match_b(void *ctx, u32 *out, struct KeyMatchRecord *rec)
{
    u32 keys = gSys.keys28;
    u32 mask;
    u32 want;
    u32 kind;

    if (((s32)rec->flag >> 1) == 0)
        keys = gSys.keys2A;
    mask = rec->mask;
    want = mask;
    kind = rec->kind + 0xB;
    switch (kind)
    {
    case 0xB:
        mask = 0;
        break;
    case 0xC:
        mask = ~mask;
        want = mask;
        break;
    }
    if ((rec->flag & 1) != 0)
    {
        if ((want & keys) != mask)
        {
            *out = rec->value;
        }
    }
    else if ((want & keys) == mask)
    {
        *out = rec->value;
    }
    return 1;
}

u32 set_var_from_keys(void *ctx, struct ScriptObj *obj, s16 *src, u32 mid)
{
    set_var(mid, (u8 *)obj + 0x18, 0, 0, *src, gSys.keys2E);
    return 1;
}

u32 set_var_from_key_group(void *ctx, struct ScriptObj *obj, struct KeyGroupRecord *src, u32 mid)
{
    u16 value = src->group;

    if (value == 0)
    {
        value = gSys.keys2A;
    }
    else if (value == 1)
    {
        value = gSys.keys28;
    }
    else
    {
        value = 0;
    }
    set_var(mid, (u8 *)obj + 0x18, 0, 0, src->index, value);
    return 1;
}

u32 set_key_ignore_mask(void *ctx, void *unused, u32 *src)
{
    struct KeyWord
    {
        u16 low : 10;
        u16 high : 6;
    };
    struct SysKeysRam
    {
        u8 filler0[0x78];
        struct KeyWord keys;
    };
    struct SysKeysRam *sys = (struct SysKeysRam *)&gSys;
    u16 low = ~*src & 0x3FF;

    sys->keys.low = low;
    poll_keys();
    return 1;
}

u32 set_key_mode_bits(void *ctx, void *unused, u8 *src)
{
    struct KeyModeBits16
    {
        u16 lo10 : 10;
        u16 f2 : 2;
        u16 hi4 : 4;
    };
    u8 *sys = (u8 *)&gSys;

    ((struct KeyModeBits16 *)(sys + 0x78))->f2 = *src;
    return 1;
}

u32 set_shadow_bits(void *ctx, void *unused, u32 *src)
{
    s32 op = *src++;

    switch (op)
    {
    case 0:
        gSys.oneFrame7C |= *src;
        break;
    case 1:
        gSys.setByGame7A |= *src;
        break;
    case 2:
        gSys.setByGame7A &= ~*src;
        break;
    }
    return 1;
}

u32 copy_if_cmp_match(void *ctx, u32 *out, struct CmpMatchRecord *rec)
{
    u8 res = compare_op(ctx, rec->op, rec->a, rec->b);

    if (res == rec->expect)
    {
        *out = rec->value;
    }
    return 1;
}

/* Script command: applies args->op to args->value and args->operand, keeps the result in args->value
 * and stores it in script variable args->var. */
u32 binary_op_and_set_var(void *ctx, u8 *obj, struct BinaryOpArgs *args, u32 vars, void *flagsA, u32 flagsB)
{
    switch (args->op)
    {
    case BINOP_NONE:
        break;
    case BINOP_ADD:
        args->value = args->value + args->operand;
        break;
    case BINOP_SUB:
        args->value = args->value - args->operand;
        break;
    case BINOP_MUL:
        args->value = args->value * args->operand;
        break;
    case BINOP_DIV:
        args->value = (s32)args->value / (s32)args->operand;
        break;
    case BINOP_MOD:
        args->value = (s32)args->value % (s32)args->operand;
        break;
    case BINOP_SHL:
        args->value = args->value << args->operand;
        break;
    case BINOP_SHR:
        args->value = (s32)args->value >> args->operand;
        break;
    case BINOP_AND:
        args->value = args->value & args->operand;
        break;
    case BINOP_OR:
        args->value = args->value | args->operand;
        break;
    case BINOP_XOR:
        args->value = args->value ^ args->operand;
        break;
    case BINOP_LOGICAL_NOT:
        args->value = args->value == 0;
        break;
    case BINOP_BITWISE_NOT:
        args->value = ~args->value;
        break;
    }
    set_var(vars, obj + 0x18, flagsA, flagsB, args->var, args->value);
    return 1;
}

u32 cmd_store_obj_field(void *ctx, struct ScriptObj *obj, u32 *args, u32 mid)
{
    u32 index = *args++;
    struct FieldObj *item = (struct FieldObj *)gParty + *args++;
    u32 value;

    switch ((s32)*args)
    {
    case 0:
        value = item->unkE;
        break;
    case 1:
        value = item->unk12;
        break;
    case 2:
        value = item->unk14;
        break;
    case 3:
        value = item->unk18;
        break;
    }
    set_var(mid, (u8 *)obj + 0x18, 0, 0, (s16)index, value);
    return 1;
}

/* Script command: stores one field of the timer gCurObj (see timer_set_time) in script variable
   args[0]; args[1] picks it: 0 hours (0x22), 1 minutes (0x23), 2 hundredths (0x25), 3 the third
   value timer_set_time got (0x24). Returns 1 (go on). */
u32 store_obj_byte_to_var(void *ctx, struct ScriptObj *obj, u32 *args, u32 vars)
{
    s32 field = args[1];
    switch (field)
    {
    case 0:
        set_var(vars, (u8 *)obj + 0x18, 0, 0, *(s16 *)args, *(s8 *)(gCurObj + 0x22));
        break;
    case 1:
        set_var(vars, (u8 *)obj + 0x18, 0, 0, *(s16 *)args, *(s8 *)(gCurObj + 0x23));
        break;
    case 2:
        set_var(vars, (u8 *)obj + 0x18, 0, 0, *(s16 *)args, *(s8 *)(gCurObj + 0x25));
        break;
    case 3:
        set_var(vars, (u8 *)obj + 0x18, 0, 0, *(s16 *)args, *(s8 *)(gCurObj + 0x24));
        break;
    }
    return 1;
}

u32 set_cur_obj_26_27(void *ctx, void *unused, u32 *args)
{
    u8 *cur = gCurObj;
    u32 first = *args++;
    u32 second;

    cur[0x26] = first;
    cur = gCurObj;
    second = *args;
    cur[0x27] = second;
    return 1;
}

u32 set_cur_obj_29(void *ctx, void *unused, u32 *args)
{
    u8 *cur;
    u8 value;

    if (*args == 0)
    {
        cur = gCurObj + 0x29;
        value = 1;
    }
    else
    {
        cur = gCurObj + 0x29;
        value = 0xFF;
    }
    *cur = value;
    return 1;
}

void timer_set_time(void *obj, s8 h, s8 m, s8 s);

u32 cur_obj_bbf8(void *ctx, void *unused, u32 *args)
{
    s8 *bytes = (s8 *)args;

    timer_set_time(gCurObj, bytes[0], bytes[4], bytes[8]);
    return 1;
}

u32 stub_return_1_080EB624(void)
{
    return 1;
}

u32 branch_if_list_entry(void *ctx, struct ListEntry **list, struct BranchState *state, u32 *id)
{
    struct ListEntry *entry;
    s16 i;

    i = 0;
    entry = *list;
    for (; i <= 3; i++, entry++)
    {
        if (entry->active != 0)
        {
            if (entry->id == *id || *id == 0x3F)
            {
                state->word0 = state->word14;
                return 0;
            }
        }
    }
    return 1;
}

u32 stub_return_1_b(void)
{
    return 1;
}

u32 clear_list_entry(void *ctx, struct ListEntry **list, u32 *unused, u32 *id)
{
    struct ListEntry *entry;
    s16 i;

    i = 0;
    entry = *list;
    for (; i <= 3; i++, entry++)
    {
        if (entry->active != 0 && entry->id == *id)
        {
            slot_obj_clear(list, i);
            break;
        }
    }
    return 1;
}

u32 set_var_from_text_byte(void *ctx, struct ScriptObj *obj, s16 *src, u32 mid)
{
    set_var(mid, (u8 *)obj + 0x18, 0, 0, *src, gUnk_03000FB8[0x30]);
    return 1;
}

u32 store_if_text_byte(void *ctx, u32 *dst, u32 *args)
{
    if (gUnk_03000FB8[0x30] == args[0])
    {
        *dst = args[1];
    }
    return 1;
}

u32 copy_if_slot_bit(void *ctx, struct ScriptObj *obj, u32 *dst, u32 *args)
{
    u8 *state = (u8 *)obj->word4;
    s32 bits = state[0x1B2];

    if (((bits >> *args++) & 1) != 0)
    {
        *dst = *args;
    }
    return 1;
}

u32 set_ramp0_if_zero(void *ctx, void *unused, u32 *rec)
{
    u32 flag = *rec++;

    if (flag == 0)
    {
        set_ramp0_target(((u8 *)rec)[4], ((u8 *)rec)[0]);
    }
    return 1;
}

u32 slot_value_cmd(void *ctx, struct BranchState *state, u32 *args)
{
    s32 op = *args++;

    switch (op)
    {
    case 0:
        set_slot_value(*args, -1);
        break;
    case 1:
        if (*args != 0)
        {
            sound_stop_song(*args);
        }
        else
        {
            sound_stop_all();
        }
        break;
    case 2:
        if ((u8)sound_is_playing(*args) != 0)
        {
            state->word0 = state->word14;
            return 0;
        }
        break;
    }
    return 1;
}

u32 set_slot_param(void *ctx, void *unused, u32 *args)
{
    s32 op = *args++;

    switch (op)
    {
    case 0:
        if (args[2] == 2)
        {
            args[2] = -1;
        }
        set_slot_target(args[0], args[1], args[2]);
        break;
    case 1:
        if (args[2] == 2)
        {
            args[2] = -1;
        }
        slot_set_mode(args[0], args[2], (u8)args[1]);
        break;
    case 2:
        set_slot_fields(args[0], (u8)args[2], (u8)args[1]);
        break;
    case 3:
        set_slot_fields_45(args[0], (u8)args[2], (u8)args[1]);
        break;
    }
    return 1;
}

u32 slot_target_cmd(void *ctx, struct BranchState *state, u32 *args)
{
    u32 op = *args++;

    switch (op)
    {
    case 0:
        set_slot_target(args[0], args[1], -1);
        break;
    case 1:
        set_slot_target(args[0], args[1], 0xFF);
        break;
    case 2:
        set_slot_fields(args[0], 0xFF, (u8)args[1]);
        break;
    case 3:
        set_slot_fields(args[0], 0, (u8)args[1]);
        break;
    case 4:
    {
        s32 k = *args++;

        switch (k)
        {
        case 0:
        case 1:
            slot_reset(*args);
            break;
        case 2:
            if ((u8)slot_bit_test(*args) != 0)
            {
                state->word0 = state->word14;
                return 0;
            }
            break;
        }
        break;
    }
    }
    return 1;
}

u32 set_text_window_32(void *ctx, void *unused, u32 *src)
{
    u8 *p = gUnk_03000FB8;
    u32 v = *src;

    p[0x32] = v;
    return 1;
}

u32 bump_or_branch_record(void *ctx, u32 *st, u32 *dst, u32 *p)
{
    u32 sel = *p++;
    u8 *rec;

    switch (sel)
    {
    case 0:
        bump_record(st, (u16)*p);
        break;
    case 1:
        rec = (u8 *)st[1];
        rec += *p * 0x4C;
        if ((rec[0x42] << 29) >= 0)
        {
            dst[0] = dst[5];
            return 0;
        }
        break;
    }
    return 1;
}

u32 map_rects_cmd(void *ctx, struct ScriptObj *obj, void *unused, u32 *cmd)
{
    if ((s32)*cmd <= 4)
    {
        clear_slot_rect(obj, (u16)*cmd);
        copy_map_rects(obj, (u8)((0x1000000 << *cmd) >> 24));
    }
    else
    {
        clear_active_slot_rects(obj);
        copy_map_rects(obj, *((u8 *)obj->word4 + 0x1B3));
    }
    gUnk_03000FB8[0x31] = 0;
    return 1;
}

u32 branch_if_state_bit(void *ctx, struct ScriptObj *obj, struct BranchState *state, u32 *args)
{
    u8 *block = (u8 *)obj->word4;
    s32 bits = block[0x1B4];

    if (((bits >> *args) & 1) != 0)
    {
        state->word0 = state->word14;
        return 0;
    }
    return 1;
}

extern u8 gTextWidthTable[];

NONMATCH("asm/code_080EA8A0/draw_text_item_raw.s", u32 draw_text_item_raw(void *ctx, void *obj, void *unused, struct TextItemRec *rec))
{
    u32 attr = rec->wordC;
    u32 extra = *(u32 *)((u8 *)rec + 0x14);
    u32 hi = (u8)((s32)attr >> 8);
    u32 width = gTextWidthTable[(attr & 0xFF) << 1];
    u32 bits;

    bits = (extra << 2) & 0x800;
    bits |= ((extra - 0x80) << 1) & 0x300;
    bits |= (extra << 1) & 0xC0;
    bits |= extra & 0x1F;
    draw_text_prim(obj, rec->byte0, rec->half4, rec->half8, hi, width, rec->half10, bits, rec->byte18, rec->word1C);
    return 1;
}
END_NONMATCH

u32 unknown_thumb_080eb9e0(void *ctx, void *obj, void *unused, struct TextItemRec *rec)
{
    draw_text_prim(obj, rec->byte0, rec->half4, rec->half8, (u8)(rec->wordC >> 8), (u8)rec->wordC, rec->half10, rec->half14, rec->byte18, rec->word1C);
    return 1;
}

u32 set_obj_word9c_flag(void *ctx, struct ScriptObj *obj, u32 *src)
{
    u32 result;

    obj->word9C = *src;
    if (obj->word9C != 0)
    {
        obj->halfA0 |= 0x10;
        result = 0;
    }
    else
    {
        result = 1;
    }
    return result;
}

u32 set_obj_word0_saved(void *ctx, struct ScriptObj *obj, u32 *rec)
{
    if (rec[0] == 1)
    {
        obj->word10 = obj->word0;
    }
    obj->word0 = rec[1];
    return 1;
}

u32 restore_obj_word0(void *ctx, struct ScriptObj *obj)
{
    register u32 saved asm("r2") = obj->word10;

    if (saved == 0)
    {
        obj->word4 = obj->word0;
    }
    else
    {
        obj->word0 = saved;
        obj->word10 = 0;
    }
    return 1;
}

u32 reset_cursor(void *ctx, struct ScriptObj *obj)
{
    obj->word4 = obj->word0;
    obj->word10 = 0;
    return 1;
}

u8 step_effect_slot(u8 *owner, u32 index, u8 *state);
ASM_FUNC("asm/code_080EA8A0/update_owner_objects.s", void update_owner_objects(void))

struct ObjSubState
{
    u8 filler0[0x214];
    u8 unk214;
    u8 filler215[0x294 - 0x215];
    struct FlagRec *sub;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};

void obj_apply_state_b(void *obj);

void update_obj_sub_state(void *ctx, struct ObjSubState *obj, struct ScriptObj *other)
{
    if ((obj->sub->unk12 & 6) != 2)
        ((struct SpriteModeBits *)&obj->sub->unk12)->mode = 1;
    if (obj->sub->unk12 & 8)
    {
        if (--other->word9C == 0)
        {
            if (other->halfA0 & 0x200)
            {
                ((struct Lo3Bits *)&obj->unk214)->lo3 = 0;
                other->halfA0 &= 0xFDDF;
                return;
            }
            obj_apply_state_b(obj);
            other->halfA0 ^= 0x20;
        }
        ((struct SpriteModeBits *)&obj->sub->unk12)->mode = 0;
    }
}

ASM_FUNC("asm/code_080EA8A0/set_obj_mode.s", void set_obj_mode(void))

ASM_FUNC("asm/code_080EA8A0/step_effect_slot.s", u8 step_effect_slot(u8 *owner, u32 index, u8 *state))
