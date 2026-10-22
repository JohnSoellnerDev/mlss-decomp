#include "global.h"

struct SlotBuf
{
    u8 value;
    u8 prev;
    u8 filler2[0x0E];
    u8 pair[2];
    u8 unk12;
    s8 unk13;
    u8 filler14[2];
    u8 unk16;
};

struct Flag7Byte
{
    u8 lo7 : 7;
    u8 bit7 : 1;
};

struct StageBits
{
    u8 stage : 3;
    u8 rest : 5;
};

struct Bit2Byte
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Bit4Byte
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bits350
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Lo6Byte
{
    u8 lo6 : 6;
    u8 rest : 2;
};

struct Lo3Byte
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Bit6Byte
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 rest : 1;
};

struct Mode12Byte
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct Bits6Low
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 rest : 2;
};

struct Lo4Hi2Byte
{
    u8 lo4 : 4;
    u8 hi2 : 2;
    u8 rest : 2;
};

struct SpriteObj
{
    u16 posX;
    u16 posY;
    u8 filler4[0xA];
    u16 unkE;
    u8 filler10;
    u8 flags11;
    u8 flags12;
    u8 filler13[0x1C - 0x13];
    u16 unk1C;
    u8 unk1E;
    u8 flags1F;
};

struct Bit0Byte
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct Bits1F
{
    u8 b01 : 2;
    u8 b23 : 2;
    u8 rest : 4;
};

struct UnitObj
{
    u8 filler0[4];
    s16 state4;
    u8 filler6[0xC - 6];
    s32 x;
    s32 y;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 flags24;
    u8 filler25[0x2C - 0x25];
    s16 unk2C;
    u8 filler2E[0x3C - 0x2E];
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x54 - 0x44];
    u8 variant54;
    u8 filler55[0x20C - 0x55];
    u8 flags20C;
    u8 filler20D[0x212 - 0x20D];
    u8 flags212;
    u8 filler213;
    u8 bits214;
    u8 filler215;
    u8 flags216;
    u8 filler217;
    u16 id218;
    u8 filler21A[0x21E - 0x21A];
    u16 pos21E;
    u8 filler220[0x242 - 0x220];
    s16 unk242;
    s16 unk244;
    u8 filler246[0x25C - 0x246];
    s32 counter25C;
    u8 filler260[0x264 - 0x260];
    s16 unk264;
    s16 unk266;
    u8 filler268[0x26C - 0x268];
    u16 entryKey;
    u8 entry26E;
    u8 filler26F[0x278 - 0x26F];
    u16 unk278;
    u8 filler27A[2];
    u16 unk27C;
    u8 filler27E[0x294 - 0x27E];
    struct SpriteObj *sub;
    u8 filler298[0x2B5 - 0x298];
    u8 flags2B5;
    u8 filler2B6[0x334 - 0x2B6];
    void *entryTable;
    u8 filler338[0x33C - 0x338];
    u8 unk33C;
    u8 filler33D;
    u8 unk33E;
    u8 filler33F;
    u8 unk340;
    u8 filler341;
    u8 unk342;
    u8 filler343[0x350 - 0x343];
    u8 flags350;
    u8 flags351;
    u8 flags352;
    u8 flags353;
    u8 filler354;
    u8 flags355;
    u16 unk356;
    u8 flags358;
    u8 flags359;
    u8 flags35A;
    u8 flags35B;
    u8 filler35C[0x368 - 0x35C];
    struct SlotBuf *slots;
    struct SpriteObj *aux36C;
    u16 auxId370;
    s8 auxOfs372;
    s8 auxOfs373;
    s8 auxOfs374;
};

void menu_entry_refresh(void *table, u32 idx, u32 mode);
void setup_obj_10bc(void *obj, u32 bank, s32 val, u32 one, s32 a, s32 b, s32 c, s32 d);
void obj_refresh_anim_state(struct UnitObj *obj, u32 a, s32 b, s32 c);
void unlink_d54(void *ctl);
void obj_init_motion(struct UnitObj *obj, s32 x, s32 y, s32 z);
void set_slot_value(u32 slot, u32 value);
void obj_copy_sub_fields(struct UnitObj *obj);
void obj_reset_link_fields(struct UnitObj *obj);
extern u8 *gAttachedSpriteCodePtrTable[];
extern u8 gAuxOffsetTable[];
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_queue_update(void *sprite);
void obj_setup_aux_sprite(struct UnitObj *obj, u32 aux);
void sound_stop_song(u32 song);

void obj_set_id(struct UnitObj *obj, u16 id)
{
    u32 special;

    if (obj->id218 != id)
    {
        obj->id218 = id;
        if (obj->entry26E != 0xFF)
        {
            menu_entry_refresh(obj->entryTable, obj->entry26E, 0);
            obj->entryKey = 0xFFFF;
            obj->entry26E = 0xFF;
        }
        special = (u16)(obj->id218 - 0x1001) <= 1 || obj->id218 == 0x1028;
        ((struct Flag7Byte *)&obj->flags216)->bit7 = special;
        setup_obj_10bc(obj->sub, 0, obj->id218, 1, -1, -1, -1, -1);
        obj->pos21E = ((obj->sub->unk1E >> 4) + 1) << 12 | obj->sub->unk1C;
        obj_refresh_anim_state(obj, 1, -1, -1);
    }
}

void flags_obj_update_stage(struct UnitObj *obj, u8 flag)
{
    u32 id;

    if (!(obj->flags20C & 1))
        return;
    if (obj->flags216 & 0x20)
        return;
    if (obj->counter25C >= 0)
    {
        if (flag != 0 && (obj->flags2B5 & 0xF) == 0)
        {
            obj->state4 = 4;
            if (obj->variant54 == 0)
                id = (obj->flags35B & 8) ? 0x103C : 0x103B;
            else
                id = 0x1052;
            obj_set_id(obj, id);
            if (((struct StageBits *)&obj->bits214)->stage != 4)
                ((struct StageBits *)&obj->bits214)->stage = 5;
        }
        else
        {
            if (obj->unk14 + obj->unk18 >= obj->unk3C + obj->unk40)
                obj->state4 = 2;
            else
                obj->state4 = 3;
        }
    }
    else
    {
        if (obj->unk242 != 0)
            obj->state4 = 1;
        else if (obj->unk244 != 0)
            obj->state4 = 1;
        else
            obj->state4 = 0;
    }
    if (flag == 0)
        return;
    if (obj->flags2B5 & 0xF)
        return;
    if (obj->unk2C != 4)
        return;
    if (obj->state4 == 4)
        return;
    if (obj->variant54 == 0)
        id = (obj->flags35B & 8) ? 0x1028 : 0x1001;
    else
        id = 0x1002;
    obj_set_id(obj, id);
    ((struct StageBits *)&obj->bits214)->stage = 3;
}


void obj_enter_stage4(struct UnitObj *obj)
{
    u32 id;

    ((struct StageBits *)&obj->bits214)->stage = 4;
    if (obj->state4 != 4)
    {
        if (obj->variant54 == 0)
            id = (obj->flags35B & 8) ? 0x103C : 0x103B;
        else
            id = 0x1052;
        obj_set_id(obj, id);
    }
    else
    {
        if (obj->sub->flags12 & 0x10)
            obj_refresh_anim_state(obj, 1, -1, -1);
    }
    obj_init_motion(obj, obj->unk264, obj->unk266, -1);
    obj->flags355 = 1;
    obj->state4 = 4;
    set_slot_value(0xD7, -1);
}

void obj_update_stage3_id(struct UnitObj *obj)
{
    u32 id;

    if (obj->variant54 == 0)
    {
        if (obj->id218 == 0x103E)
            goto matched;
    }
    if (obj->variant54 != 1)
        goto tail;
    if (obj->id218 != 0x1053)
        goto tail;
matched:
    ((struct StageBits *)&obj->bits214)->stage = 3;
    if (obj->variant54 == 0)
        id = (obj->flags35B & 8) ? 0x1028 : 0x1001;
    else
        id = 0x1002;
    obj_set_id(obj, id);
tail:
    ((struct Bit2Byte *)&obj->flags35A)->bit2 = 0;
}

void obj_enter_stage4_slot(struct UnitObj *obj, u8 flag)
{
    u32 id;
    struct SpriteObj *sub;

    ((struct StageBits *)&obj->bits214)->stage = 4;
    if (obj->variant54 == 0)
        id = (obj->flags35B & 8) ? 0x1029 : 0x1025;
    else
        id = 0x103F;
    obj_set_id(obj, id);
    sub = obj->sub;
    ((struct Mode12Byte *)&sub->flags12)->mode = 1;
    if (flag == 0)
        set_slot_value(0x3C, -1);
    else
        set_slot_value(0x3C, -1);
    obj->state4 = 5;
    ((struct Bit6Byte *)&obj->flags212)->bit6 = 1;
    ((struct Bit4Byte *)&obj->flags359)->bit4 = 0;
    ((struct Bit2Byte *)&obj->flags353)->bit2 = 0;
}

void obj_enter_stage4_aux(struct UnitObj *obj)
{
    u32 id;
    u32 aux;
    u32 slot;
    u8 *p54;
    u32 v;

    ((struct StageBits *)&obj->bits214)->stage = 4;
    if (obj->variant54 == 0)
        id = (obj->flags35B & 8) ? 0x102A : 0x1026;
    else
        id = 0x1040;
    obj_set_id(obj, id);
    aux = 0;
    if (*(u8 *)obj->slots == 0xB)
        aux = 1;
    obj_setup_aux_sprite(obj, aux);
    p54 = &obj->variant54;
    v = *p54;
    slot = 0x87;
    if (v == 0)
        slot += 0xD0;
    set_slot_value(slot, -1);
    obj->state4 = 0xA;
    obj->flags355 = 0;
    ((struct Bit4Byte *)&obj->flags359)->bit4 = 0;
}

void obj_enter_stage3_unlink(struct UnitObj *obj)
{
    u32 id;
    u32 first;
    u32 second;
    u8 *p54;
    u32 v;

    ((struct StageBits *)&obj->bits214)->stage = 3;
    unlink_d54(obj->aux36C);
    if (obj->variant54 == 0)
        id = (obj->flags35B & 8) ? 0x1028 : 0x1001;
    else
        id = 0x1002;
    obj_set_id(obj, id);
    p54 = &obj->variant54;
    v = *p54;
    first = 0x87;
    if (v == 0)
        first += 0xD0;
    sound_stop_song(first);
    if (*p54 == 0)
        second = 0x158;
    else
        second = 0x156;
    sound_stop_song(second);
    obj->state4 = 0;
}

NONMATCH("asm/code_08049014/obj_setup_aux_sprite.s", void obj_setup_aux_sprite(struct UnitObj *obj, u32 aux))
{
    u32 id;
    u8 *table;

    id = 0x105A;
    if (obj->variant54 == 0)
        id = 0x1059;
    obj->auxId370 = id;
    setup_obj_10bc(obj->aux36C, 0, obj->auxId370, 1, -1, -1, -1, -1);
    obj_load_record(obj->aux36C, 0, -1, 0, 0);
    obj_queue_update(obj->aux36C);
    table = gAuxOffsetTable + aux * 24;
    ((struct Bit0Byte *)&obj->flags353)->bit0 = 1;
    obj->auxOfs372 = table[((u32)obj->flags24 << 29 >> 29) * 3];
    obj->auxOfs373 = table[((u32)obj->flags24 << 29 >> 29) * 3 + 1];
    obj->auxOfs374 = table[((u32)obj->flags24 << 29 >> 29) * 3 + 2];
    ((struct Bit6Byte *)&obj->aux36C->flags11)->bit6 = 0;
    ((struct Bits1F *)&obj->aux36C->flags1F)->b01 = ((u32)obj->sub->flags1F << 30) >> 30;
    ((struct Bits1F *)&obj->aux36C->flags1F)->b23 = (obj->sub->flags1F & 0xC) >> 2;
    obj->aux36C->posX = obj->auxOfs372 + obj->sub->posX;
    obj->aux36C->posY = obj->auxOfs373 + obj->sub->posY;
    obj->aux36C->unkE = obj->auxOfs374 + obj->sub->unkE;
    ((struct Mode12Byte *)&obj->aux36C->flags12)->mode = 0;
}
END_NONMATCH


void obj_setup_aux_1045(struct UnitObj *obj)
{
    struct SpriteObj *aux;
    u32 low;
    u32 high;

    obj->auxId370 = 0x1045;
    setup_obj_10bc(obj->aux36C, 0, 0x1045, 1, -1, -1, -1, -1);
    obj_load_record(obj->aux36C, 1, -1, 0, 0);
    obj_queue_update(obj->aux36C);
    ((struct Bit0Byte *)&obj->flags353)->bit0 = 1;
    obj->auxOfs372 = 0;
    obj->auxOfs373 = 0;
    obj->auxOfs374 = 0xFC;
    ((struct Bit6Byte *)&obj->aux36C->flags11)->bit6 = 0;
    aux = obj->aux36C;
    low = ((u32)obj->sub->flags1F << 30) >> 30;
    aux->flags1F = (aux->flags1F & 0xFC) | low;
    aux = obj->aux36C;
    high = obj->sub->flags1F & 0xC;
    ((struct Bits1F *)&aux->flags1F)->b23 = high >> 2;
    obj->aux36C->posX = obj->sub->posX;
    obj->aux36C->posY = obj->sub->posY;
    obj->aux36C->unkE = obj->sub->unkE;
    ((struct Mode12Byte *)&obj->aux36C->flags12)->mode = 2;
}

void obj_setup_aux_1054(struct UnitObj *obj)
{
    struct SpriteObj *aux;
    u32 low;
    u32 high;
    u32 v;

    obj->auxId370 = 0x1054;
    setup_obj_10bc(obj->aux36C, 0, 0x1054, 1, -1, -1, -1, -1);
    obj_load_record(obj->aux36C, 0, -1, 0, 0);
    obj_queue_update(obj->aux36C);
    ((struct Bit0Byte *)&obj->flags353)->bit0 = 1;
    obj->auxOfs372 = 0;
    obj->auxOfs373 = 0xF0;
    obj->auxOfs374 = 0xFC;
    ((struct Bit6Byte *)&obj->aux36C->flags11)->bit6 = 0;
    aux = obj->aux36C;
    low = ((u32)obj->sub->flags1F << 30) >> 30;
    aux->flags1F = (aux->flags1F & 0xFC) | low;
    aux = obj->aux36C;
    high = obj->sub->flags1F & 0xC;
    v = aux->flags1F;
    v = (v & -0xD) | high;
    aux->flags1F = v;
    obj->aux36C->posX = obj->auxOfs372 + obj->sub->posX;
    obj->aux36C->posY = obj->auxOfs373 + obj->sub->posY;
    obj->aux36C->unkE = obj->auxOfs374 + obj->sub->unkE;
    ((struct Mode12Byte *)&obj->aux36C->flags12)->mode = 0;
}

void obj_setup_aux_8004(struct UnitObj *obj)
{
    struct SpriteObj *aux;
    u32 low;
    u32 high;
    u32 v;

    obj->auxId370 = 0x8004;
    setup_obj_10bc(obj->aux36C, 0, 0x8004, 1, -1, -1, -1, -1);
    obj_load_record(obj->aux36C, 0, -1, 0, 0);
    obj_queue_update(obj->aux36C);
    ((struct Bit0Byte *)&obj->flags353)->bit0 = 1;
    obj->auxOfs372 = 0;
    obj->auxOfs373 = 0xE0;
    obj->auxOfs374 = 0xFC;
    ((struct Bit6Byte *)&obj->aux36C->flags11)->bit6 = 0;
    aux = obj->aux36C;
    low = ((u32)obj->sub->flags1F << 30) >> 30;
    aux->flags1F = (aux->flags1F & 0xFC) | low;
    aux = obj->aux36C;
    high = obj->sub->flags1F & 0xC;
    v = aux->flags1F;
    v = (v & -0xD) | high;
    aux->flags1F = v;
    obj->aux36C->posX = obj->auxOfs372 + obj->sub->posX;
    obj->aux36C->posY = obj->auxOfs373 + obj->sub->posY;
    obj->aux36C->unkE = obj->auxOfs374 + obj->sub->unkE;
    ((struct Mode12Byte *)&obj->aux36C->flags12)->mode = 0;
}

void attach_sprite_setup(struct UnitObj *obj)
{
    struct SpriteObj *aux;
    u32 low;
    u32 high;

    obj->auxId370 = 0x800B;
    setup_obj_10bc(obj->aux36C, 2, 0x800B, 1, -1, -1, -1, -1);
    obj_load_record(obj->aux36C, 2, -1, 0, 0);
    obj_queue_update(obj->aux36C);
    ((struct Bit0Byte *)&obj->flags353)->bit0 = 0;
    obj->auxOfs372 = 0;
    obj->auxOfs373 = 0;
    obj->auxOfs374 = 0;
    ((struct Bit6Byte *)&obj->aux36C->flags11)->bit6 = 0;
    aux = obj->aux36C;
    low = ((u32)obj->sub->flags1F << 30) >> 30;
    ((struct Bits1F *)&aux->flags1F)->b01 = low;
    aux = obj->aux36C;
    high = obj->sub->flags1F & 0xC;
    ((struct Bits1F *)&aux->flags1F)->b23 = high >> 2;
    obj->aux36C->posX = obj->sub->posX;
    obj->aux36C->posY = obj->sub->posY;
    obj->aux36C->unkE = obj->sub->unkE;
    ((struct Mode12Byte *)&obj->aux36C->flags12)->mode = 2;
}

void attach_sprite_mode(struct UnitObj *obj, u32 mode, u32 anim)
{
    struct SpriteObj *aux;
    u32 high;
    u32 low;
    u32 id;

    switch (mode)
    {
    case 0:
        obj->auxId370 = 0x800B;
        setup_obj_10bc(obj->aux36C, 2, 0x800B, 1, -1, -1, -1, -1);
        obj_load_record(obj->aux36C, anim, -1, 0, mode);
        obj_queue_update(obj->aux36C);
        ((struct Bit0Byte *)&obj->flags353)->bit0 = 0;
        ((struct Bit6Byte *)&obj->aux36C->flags11)->bit6 = 0;
        aux = obj->aux36C;
        ((struct Bits1F *)&aux->flags1F)->b01 = ((u32)obj->sub->flags1F << 30) >> 30;
        aux = obj->aux36C;
        high = obj->sub->flags1F & 0xC;
        ((struct Bits1F *)&aux->flags1F)->b23 = high >> 2;
        obj->aux36C->unkE = obj->sub->unkE;
        break;
    case 1:
    {
        obj_copy_sub_fields(obj);
        obj->unk356 = obj->id218;
        low = ((u32)obj->flags24 << 29) >> 29;
        ((struct Lo3Byte *)&obj->flags358)->lo3 = low;
        if (obj->variant54 == 0)
            id = (obj->flags35B & 8) ? 0x103D : 0x1032;
        else
            id = 0x104B;
        obj_set_id(obj, id);
    }
        break;
    }
}

void obj_set_slot_value(struct UnitObj *obj, u32 value)
{
    struct SlotBuf **slots = &obj->slots;
    u8 cur = (*slots)->value;
    if (cur != value)
    {
        if ((*slots)->unk13 == -1)
        {
            (*slots)->prev = cur;
            (*slots)->unk13 = (*slots)->pair[(obj->flags352 >> 7) ^ 1];
        }
        (*slots)->value = value;
        (*slots)->pair[(obj->flags352 >> 7) ^ 1] = 0;
    }
}

ASM_FUNC("asm/code_08049014/sync_levels_with_list.s", void sync_levels_with_list(void))

NONMATCH("asm/code_08049014/find_table_entry.s", u8 *find_table_entry(struct UnitObj *obj, u8 *table))
{
        s32 px;
        s32 py;
        s32 pz;
        s32 extY;
        s32 count;
        s32 i;
        s32 margin;
        s32 dz;
        s32 x0;
        s32 x1;
        s32 xe;
        s32 y0;
        s32 y1;
        s32 yh;
        s32 z0;
        s32 z1;
        s32 zmax;
        s32 zh;
        u8 *e;

    if ((*(u32 *)&obj->flags20C & 0x01000001) == 0x01000001)
    {
        px = obj->x / 0x100;
        py = obj->y / 0x100;
        dz = obj->unk18;
        pz = (obj->unk14 + dz) / 0x100;
        margin = (obj->unk278 >> 8) >> 1;
        extY = obj->unk27C >> 8;
        count = table[4];
        e = table - ((table[6] << 8) | table[5]);
        for (i = 0; i < count; i++, e += 11)
        {
            x0 = e[0] << 4;
            x1 = x0 + (e[2] << 4);
            xe = x1 - 1;
            y0 = e[1] << 4;
            y1 = y0 + (e[3] << 4);
            yh = y1 - 1;
            z0 = (e[4] & 0x7F) << 3;
            zh = e[5] & 0x7F;
            zmax = 0xFF << 3;
            if (zh != 0x7F)
                zmax = zh << 3;
            z1 = z0 + zmax;
            if ((e[6] & 3) == 0)
            {
                x0 -= margin;
                xe += margin;
                y0 -= 1;
                yh = y1 - 2;
                yh += extY;
            }
            else
            {
                x0 += 4;
                xe = x1 - 5;
                y0 += 7;
            }
            if (px < x0 || px > xe || py < y0 || py > yh || pz < z0 || pz > z1)
                continue;
            if (zmax == 0 && dz > 0)
                continue;
            return e;
        }
    }
    return 0;
}
END_NONMATCH


ASM_FUNC("asm/code_08049014/find_zone_offset.s", void find_zone_offset(void))

void obj_reset_anim_code(struct UnitObj *obj)
{
    u8 code;
    s32 one;

    obj_reset_link_fields(obj);
    ((struct Flag7Byte *)&obj->unk33C)->lo7 = 0x7F;
    ((struct Bit6Byte *)&obj->flags350)->bit6 = 0;
    ((struct Flag7Byte *)&obj->unk33C)->bit7 = 0;
    ((struct Lo4Hi2Byte *)&obj->flags350)->lo4 = 0;
    ((struct Lo4Hi2Byte *)&obj->flags350)->hi2 = 0;
    obj->unk33E = 0xFF;
    obj->unk340 = 0;
    obj->unk342 = 0xFF;
    ((struct Bit6Byte *)&obj->flags353)->bit6 = 0;
    if (obj->flags351 & 0x80)
        code = 2;
    else
        code = gAttachedSpriteCodePtrTable[(obj->flags352 >> 7) ^ 1][obj->state4];
    one = 1;
    ((struct Bits6Low *)&obj->flags352)->b0 = code & one;
    code >>= 1;
    ((struct Bits6Low *)&obj->flags352)->b1 = code & one;
    code >>= 1;
    ((struct Bits6Low *)&obj->flags352)->b2 = code & one;
    code >>= 1;
    ((struct Bits6Low *)&obj->flags352)->b3 = code & one;
    code >>= 1;
    ((struct Bits6Low *)&obj->flags352)->b4 = code & one;
    code >>= 1;
    ((struct Bits6Low *)&obj->flags352)->b5 = code & one;
}


ASM_FUNC("asm/code_08049014/obj_resolve_entry_contacts.s", void obj_resolve_entry_contacts(void))
