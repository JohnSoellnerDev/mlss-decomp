#include "global.h"

struct PairFrames
{
    u8 count;
    u8 filler1;
    u8 frame;
    u8 filler3[0x10 - 0x3];
    u8 cnt10;
    u8 cnt11;
    u8 filler12[0x16 - 0x12];
    u8 lo16 : 7;
    u8 bit16_7 : 1;
};

struct PairSprite
{
    s16 posX;
    s16 posY;
    u8 filler4[2];
    u16 half6;
    u8 filler8[0x12 - 0x8];
    u8 flags12;
    u8 filler13[0x21 - 0x13];
    u8 byte21;
    u8 filler22;
    u8 byte23;
};

struct PairCtl
{
    u16 half0;
    u16 half2;
    u8 filler4[0x12 - 0x4];
    u8 byte12;
};

struct PairSub
{
    u8 filler0[4];
    u16 state;
    u8 filler6[0x0C - 0x06];
    s32 posX0C;
    s32 posY10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x54 - 0x1C];
    u8 group54;
    u8 filler55[0x210 - 0x55];
    u8 flags210;
    u8 filler211[0x214 - 0x211];
    u8 stateWord214;
    u8 filler215[0x218 - 0x215];
    u16 half218;
    u8 filler21A[0x238 - 0x21A];
    s8 field238;
    u8 filler239[0x23B - 0x239];
    u8 field23B;
    u8 filler23C[0x258 - 0x23C];
    s32 unk258;
    u8 filler25C[0x25C - 0x25C];
    s32 unk25C;
    u8 filler260[0x283 - 0x260];
    u8 byte283;
    u8 filler284[0x294 - 0x284];
    struct PairSprite *sprite;
    u8 filler298[0x344 - 0x298];
    union
    {
        s8 kind;
        struct PairSub *ptr;
    } f344;
    u8 filler348[0x351 - 0x348];
    u8 flags351;
    u8 mode352;
    u8 filler353[0x355 - 0x353];
    s8 field355;
    u8 filler356[0x358 - 0x356];
    u8 ringFlags358;
    u8 field359;
    u8 filler35A[0x35B - 0x35A];
    u8 flags35B;
    u8 filler35C[0x368 - 0x35C];
    struct PairFrames *frames;
    struct PairCtl *ctl36C;
};

struct Bits35B
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bits352
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct SpriteBit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bit0Only
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct StateLo3
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct ModeBits12
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct PairObj
{
    u8 filler0[0x28];
    void *entries[32];
    u8 filler0A8[0xFA - 0xA8];
    u8 maskFA;
    u8 fieldFB;
    u8 filler0FC[0x10B - 0xFC];
    u8 field10B;
    u8 filler10C[0x12A - 0x10C];
    u8 byte12A;
    u8 filler12B[0x200 - 0x12B];
    struct PairSub *subA;
    struct PairSub *subB;
    u8 flags208;
    u8 filler209;
    u16 half20A;
    u8 filler20C[0x246 - 0x20C];
    u8 byte246;
    u8 filler247[0x28D - 0x247];
    u8 byte28D;
    u8 filler28E;
    u8 byte28F;
    u8 byte290;
    u8 flags291;
    u16 half292;
    u8 filler294[0x2A8 - 0x294];
    u8 flags2a8;
    u8 flags2a9;
    u8 filler2AA[0x304 - 0x2AA];
    void *map304;
    u8 filler308[0x31C - 0x308];
    void **slotRef31C;
};

struct Bits208
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct MaskBits
{
    u8 lo4 : 4;
    u8 hi4 : 4;
};

struct TileSrc
{
    u8 filler0[8];
    u8 *base;
};

struct TileCtxX
{
    u8 filler0[0x10];
    struct TileSrc *src;
};

struct TileCtx
{
    u8 filler0[0x1C];
    struct TileCtxX *x;
};

struct Flags2A9
{
    u8 lo2 : 2;
    u8 st : 3;
    u8 rest : 3;
};

struct CtlBits12
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct Flags291
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 mid : 3;
    u8 rest : 2;
};

void pair_kind8_step(void *obj, u32 arg, u32 flag);
void pair_set_mode(void *list, u32 mode, u32 flag);
void sound_stop_song(u32 song);
void obj_start_state(void *sub);
void obj_enter_stage3_unlink(void *sub);
void advance_obj_motion(void *sub, void *entries);
s32 obj_link_index(u32 idx);
void layer3_entries_by_attr(void *map, u16 attr);
void set_slot_value(u32 id, s32 value);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void obj_shift_state_slots(void *sub);
void update_pair_facing(void *obj, u32 a, u32 b);
void obj_set_id(void *sub, u32 id);
void motion_obj_set_move_target(void *obj, u32 a, s32 b, s32 c);
void attach_sprite_mode(void *obj, u32 a, u32 b);
void obj_stop_motion(void *obj);
void sprites_obj_set_move_target(void *obj, u32 a, u32 b, s32 c, s32 d, u32 e);
void set_flag_2_2(void *obj);
void set_slot_fields(u32 idx, u32 level, u32 period);
void sprites_obj_step_move(void *obj);
u32 obj_has_pending_bits(void *obj);
void obj_reset_move_state(void *sub);
void obj_finish_move_step(void *sub);
void release_unit(void *obj, void *sub);
void init_sub_unit_table(void *obj, void *sub);
void setup_unit_move(void *obj, void *sub);
void setup_pair_move_58(void *obj);
void set_unit_table_flags(void *obj, void *sub);
void update_obj_gfx_state(void *obj);
void unlink_d54(void *ctl);
void setup_sub_unit(struct PairObj *obj, struct PairSub *unit);
void obj_load_record(void *rec, u32 a, u32 b, u32 c, u32 d);
void child_forward_obj_step_motion(void *sub, void *entries);
void step_pair_member(struct PairObj *obj, struct PairSub *sub);
void update_sub_pair_d6ec(struct PairObj *obj);
extern const u8 gUnk_083A1538[];
extern const u16 gUnk_083A1544[];
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void obj_scan_contacts(void *sub, void *contacts, s32 range);
s32 obj_probe_ledge(void *sub);
extern const u8 gRomTileOfsA[];
extern const u8 gRomTileOfsB[];
extern const u8 gEntryValueRemap[];
void select_sub_pair(void *obj);
void pair_turn_and_sample(void *obj, u32 mode);
u32 obj_flags_2b5_07_set(void *obj);
u8 test_flag(u8 *table, u8 *tableB, u16 id);
extern u32 gUnk_03000FC0;

void set_sub_pair_state_b2(struct PairObj *obj)
{
    struct PairSub *b = obj->subB;
    struct PairSub *a = obj->subA;
    a->state = 0xB2;
    b->state = 0xB2;
}

u32 sub_0803C454(struct PairObj *obj)
{
    u32 hit = 0;
    s8 kind = obj->subA->f344.kind;
    if (kind == 0x1C || kind == 0x15)
    {
        hit = 1;
    }
    return hit;
}

NONMATCH("asm/code_0803C438/sub_0803C47C.s", u32 sub_0803C47C(struct PairObj *obj))
{
    s32 mask;
    if ((u16)(obj->subA->state - 0x2A) <= 1)
    {
        return 1;
    }
    mask = ~0x180;
    obj->half20A = mask & obj->half20A;
    return 0;
}
END_NONMATCH

NONMATCH("asm/code_0803C438/sub_0803C4B4.s", u32 sub_0803C4B4(struct PairObj *obj))
{
    s32 mask;
    if ((u16)(obj->subA->state - 0x27) <= 1)
    {
        return 1;
    }
    mask = ~0x180;
    obj->half20A = mask & obj->half20A;
    return 0;
}
END_NONMATCH

u32 sub_0803C4EC(struct PairObj *obj)
{
    u16 state = obj->subA->state;
    if ((u16)(state - 0x10) <= 8)
    {
        return 1;
    }
    if ((u16)(state - 0x1D) > 4)
    {
        return 0;
    }
    return 1;
}

u32 sub_0803C51C(struct PairObj *obj)
{
    if ((s16)obj->subA->state > 12)
    {
        return 1;
    }
    if ((s16)obj->subB->state > 12)
    {
        return 1;
    }
    return 0;
}

void call_faf8_pair(struct PairObj *obj, u32 arg)
{
    pair_kind8_step(obj, arg, 1);
}

u32 test_flag_stage(void)
{
    u32 stage;
    if (test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x205) == 0)
    {
        if (test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x204) == 0)
        {
            stage = test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x203) != 0;
        }
        else
        {
            stage = 2;
        }
    }
    else
    {
        stage = 3;
    }
    return stage;
}

void refresh_sub_pair_lists(struct PairObj *obj)
{
    advance_obj_motion(obj->subA, obj->entries);
    advance_obj_motion(obj->subB, obj->entries);
    if ((u8)obj_flags_2b5_07_set(obj->subA) == 0)
    {
        if ((u8)obj_flags_2b5_07_set(obj->subB) == 0)
        {
            select_sub_pair(obj);
            pair_turn_and_sample(obj, 0);
        }
    }
}

void set_flags_208_bit3(struct PairObj *obj, u32 value)
{
    ((struct Bits208 *)&obj->flags208)->bit3 = value;
}

void reset_pair_slots(void)
{
    sound_stop_song(0x157);
    sound_stop_song(0x87);
    sound_stop_song(0xDB);
    sound_stop_song(0xDA);
    sound_stop_song(0xDA);
}

void advance_sub_a_state(struct PairObj *obj)
{
    struct PairSub *a = obj->subA;
    u16 state = a->state;
    if ((u16)(state - 5) <= 4)
    {
        obj_start_state(a);
    }
    else if ((u16)(state - 0xA) <= 2)
    {
        obj_enter_stage3_unlink(a);
    }
}

NONMATCH("asm/code_0803C438/entry_table_value.s", u32 entry_table_value(struct PairObj *obj, u32 idx))
{
    struct PairSub *e = obj->entries[idx];
    struct PairFrames *frames = e->frames;
    u32 mode = e->mode352;
    u8 *counters = (u8 *)frames + 0x10;
    u8 v = counters[(mode >> 7) ^ 1];
    if ((mode & 0x80) != 0 || (obj->flags208 & 0x10) == 0 || frames->count == 0xA)
    {
        v = gEntryValueRemap[v];
    }
    return v;
}
END_NONMATCH

void shift_and_advance_pair_subs(struct PairObj *obj, u32 which)
{
    if ((which & 1) != 0)
    {
        obj_shift_state_slots(obj->subA);
        advance_anim_frame(obj, obj->subA, 0, 0);
    }
    if ((which & 2) != 0)
    {
        obj_shift_state_slots(obj->subB);
        advance_anim_frame(obj, obj->subB, (((u32)obj->flags208 << 27) >> 31), 0);
    }
}

void advance_pair_frames(struct PairObj *obj, u32 which)
{
    if ((which & 1) != 0)
    {
        obj->subA->frames->cnt10 = 0;
        advance_anim_frame(obj, obj->subA, 0, 0);
    }
    if ((which & 2) != 0)
    {
        obj->subB->frames->cnt11 = 0;
        advance_anim_frame(obj, obj->subB, (((u32)obj->flags208 << 27) >> 31), 0);
    }
}

void dma_tile_frame(struct TileCtx *ctx, struct PairSub *sub)
{
    struct TileSrc *y = ctx->x->src;
    const u8 *tableA = gRomTileOfsA;
    u32 frame = sub->frames->frame;
    u8 *src = y->base + (tableA[frame] << 5);
    u32 dst = 0x06010DC0 + (sub->group54 << 10);
    dma_copy_or_queue(src, (void *)dst, gRomTileOfsB[frame] << 5, 0x20, 0);
}

NONMATCH("asm/code_0803C438/mark_link_entry.s", void mark_link_entry(struct PairObj *obj, u32 idx))
{
    s32 link = obj_link_index(idx);
    if (link >= 0)
    {
        u8 *mask = &obj->maskFA;
        if (((((u32)*mask << 28) >> 28) & (1 << link)) == 0)
        {
            u32 bit = 1 << link;
            layer3_entries_by_attr(obj->map304, (u16)link);
            {
                u32 v = *mask;
                s32 keep = -16;
                *mask = (v & keep) | (((((u32)v << 28) >> 28) | bit) & 0xF);
            }
            set_slot_value(0x53, -1);
        }
    }
}
END_NONMATCH

void scan_ledge_contact(struct PairObj *obj, struct PairSub *other)
{
    if ((other->flags35B & 0x10) != 0)
    {
        obj_scan_contacts(other, (u8 *)obj + 0x30, obj->field10B - 2);
        if ((u8)obj_probe_ledge(other) != 0)
        {
            struct PairFrames *frames = other->frames;
            if (frames->count != 7)
            {
                frames->lo16 |= 1;
            }
        }
    }
}

void call_9a20_pair(struct PairObj *obj, u32 arg)
{
    pair_set_mode(obj, arg, 1);
}

NONMATCH("asm/code_0803C438/set_sub_pair_action.s", void set_sub_pair_action(struct PairObj *obj, u32 unused, u32 flag))
{
    struct Flags291 *bits;
    struct PairSub **sp;
    struct PairSub *a;
    struct PairSub *b;
    struct PairSub *unit;
    struct PairSub *src;
    struct PairSub *e;
    struct PairSprite *sprite;
    struct PairCtl *ctl;
    u8 f;
    u32 fl;
    u8 fl8;
    u32 kind;
    u32 phase;
    s32 sx;
    s32 sy;
    s32 sel;
    s32 refY;
    s32 xfix;
    s32 yfix;
    s32 xoff;
    s32 ypx;
    u8 *off;

    f = flag;
    bits = (struct Flags291 *)&obj->flags291;
    bits->b0 = 1;
    obj->half292 = 0;
    obj->byte290 = 0xFE;
    bits->mid = 2;
    bits->b2 = f;
    sp = (struct PairSub **)&obj->subA;
    b = sp[1];
    a = sp[0];
    b->flags351 |= 0x40;
    a->flags351 |= 0x40;
    set_slot_fields(0, 0, 0x10);
    if (f != 0)
    {
        set_flag_2_2(*obj->slotRef31C);
        fl = obj->flags2a9;
        if ((fl & 0x1C) != 0)
        {
            fl8 = obj->flags2a8;
            unit = obj->entries[((u32)fl << 26) >> 31];
            src = obj->entries[((u32)fl8 << 27) >> 27];
            if ((fl8 & 0x20) != 0)
            {
                src = src->f344.ptr;
            }
            sx = src->sprite->posX;
            sy = src->sprite->posY;
            switch (((u32)fl << 27) >> 29)
            {
            case 1:
            case 5:
                motion_obj_set_move_target(unit, 0x10, -1, -1);
                attach_sprite_mode(unit, 0, 0);
                ctl = unit->ctl36C;
                ((struct CtlBits12 *)&ctl->byte12)->mode = 2;
                if (unit->half218 == 0x1001 || unit->half218 == 0x1028 || unit->half218 == 0x1002)
                {
                    sprite = unit->sprite;
                    kind = sprite->byte21;
                    phase = kind % 5;
                    obj_load_record(sprite, (u8)phase + 5, 0, 0, 0);
                }
                goto offs;
            case 2:
                attach_sprite_mode(unit, 0, 1);
offs:
                unit->ctl36C->half0 = sx + 1;
                unit->ctl36C->half2 = sy - src->byte283 + 2;
                break;
            case 4:
                attach_sprite_mode(unit, 1, 0);
                break;
            case 3:
                obj_stop_motion(unit);
                motion_obj_set_move_target(unit, 0x10, -1, -1);
                attach_sprite_mode(unit, 1, 0);
                break;
            }
        }
        if ((obj->flags2a9 & 0x40) != 0)
        {
            sel = (s8)obj->subB->field238;
            if (sel != -1)
            {
                e = obj->entries[sel];
                refY = e->unk14 + e->unk18;
            }
            else
            {
                refY = obj->subB->unk14;
            }
            xfix = obj->subB->posX0C;
            if (xfix < 0)
            {
                xfix += 0xFF;
            }
            xoff = (xfix >> 8) - 0x78;
            yfix = obj->subB->posY10 - refY;
            if (yfix < 0)
            {
                yfix += 0xFF;
            }
            ypx = yfix >> 8;
            if (obj->subB->unk25C != -1)
            {
                off = &obj->byte28D;
            }
            else
            {
                off = &obj->byte28F;
            }
            sprites_obj_set_move_target(obj, 1, 0, xoff, ypx - *off, 0xC00);
        }
    }
    else
    {
        ((struct Flags2A9 *)&obj->flags2a9)->st = 0;
    }
    obj->byte12A = 0xA;
}
END_NONMATCH

ASM_FUNC("asm/code_0803C438/update_obj_pair_anim.s", void update_obj_pair_anim(void))

static inline u32 is_member_step_state(s32 s)
{
    switch (s)
    {
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 0xF:
    case 0x3A:
    case 0x3E:
    case 0x3F:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x6B:
        return 1;
    default:
        return 0;
    }
}

void step_sub_pair(struct PairObj *obj)
{
    if ((obj->subA->ringFlags358 & 8) != 0)
    {
        update_sub_pair_d6ec(obj);
    }
    else
    {
        if (is_member_step_state((s16)obj->subA->state))
        {
            step_pair_member(obj, obj->subA);
        }
        else
        {
            ((struct SpriteBit4 *)&obj->subA->sprite->flags12)->bit4 = 1;
            child_forward_obj_step_motion(obj->subA, obj->entries);
        }
        if (is_member_step_state((s16)obj->subB->state))
        {
            step_pair_member(obj, obj->subB);
        }
        else
        {
            ((struct SpriteBit4 *)&obj->subB->sprite->flags12)->bit4 = 1;
            child_forward_obj_step_motion(obj->subB, obj->entries);
        }
    }
}

ASM_FUNC("asm/code_0803C438/step_pair_member.s", void step_pair_member(struct PairObj *obj, struct PairSub *sub))

ASM_FUNC("asm/code_0803C438/update_sub_pair_d6ec.s", void update_sub_pair_d6ec(struct PairObj *obj))

void pair_sub_load_record(struct PairObj *owner, struct PairSub *unit)
{
    u8 t1 = gUnk_083A1538[unit->field355 / 4];
    u32 flag = t1 & 0x80;
    u8 b21 = unit->sprite->byte21;
    u8 b23 = unit->sprite->byte23;
    u32 lo = t1 & 0x7F;
    u16 hw1 = gUnk_083A1544[lo + ((unit->field359 >> 5) << 1)];
    obj_set_id(unit, hw1);
    obj_load_record(unit->sprite, b21, b23, 0, 0);
    unit->sprite->half6 = gUnk_083A1544[(lo + ((unit->field359 >> 5) << 1)) + 1];
    if (flag != 0)
    {
        owner->byte290 = 0;
        ((struct Bit0Only *)&owner->byte246)->b0 = 1;
    }
    else
    {
        unit->field355++;
    }
}

void setup_sub_unit(struct PairObj *obj, struct PairSub *unit)
{
    update_pair_facing(obj, 1, 1);
    ((struct Bits35B *)&unit->flags35B)->bit4 = 0;
    unit->flags210 |= 2;
    unit->flags210 |= 4;
    unit->state = 0x3E;
    ((struct Bits352 *)&unit->flags351)->bit6 = 1;
    unit->field23B = 0;
    ((struct Bits352 *)&unit->mode352)->bit6 = 0;
    ((struct StateLo3 *)&unit->stateWord214)->lo3 = 4;
    obj_set_id(unit, 0x1044);
    ((struct ModeBits12 *)&unit->sprite->flags12)->mode = 1;
    set_slot_value(0x38, -1);
}
