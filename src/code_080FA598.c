#include "global.h"

struct OwnerState
{
    u8 filler0[0x888];
    u32 handles[0x46];
    u8 cur9A0;
    u8 mode9A1;
    u8 busy9A2;
    u8 flags9A3;
};

struct Slot
{
    u8 *obj;
    u8 filler4[0x14 - 4];
    s32 xFix;
    s32 yFix;
    u8 filler1C[0x3A - 0x1C];
    u16 scale;
    u8 flags;
    u8 unk3D;
    u8 filler3E[0x40 - 0x3E];
};

struct ObjList
{
    u8 filler0[0x158];
    struct Slot *slots;
};

struct Owner
{
    u8 filler0[0x14];
    struct ObjList *list;
    u32 type;
    u8 filler1C[0x20 - 0x1C];
    u32 aux;
    struct OwnerState *state;
    u8 *entry;
};

struct CmdRec
{
    u8 filler0[0xA0];
    u16 flagsA0;
};

extern u8 *gUnk_03000FC0;
extern u8 gOwnerTypeCreateDestroy[];
extern u8 gStateTag[];
extern u32 gTextExtraFlagPtrs[];
extern u32 gTextCountPtrs[];
extern u32 gTextKindPtrs[];
extern u32 gTextModePtrs[];
extern u32 gTextHalfPtrs[];
extern u8 *gEffectState;
extern u8 *gMemRoutines;
extern u8 gUnk_03000C80[];
extern u8 gClaimKindTable[];

void clear_command_records(struct Owner *owner);
void owner_reset(struct Owner *owner);
void owner_release_handles(struct Owner *owner);
void clear_slot_rect(u8 *rect, u32 n);
void copy_map_rects(u8 *rects, u32 n);
void task_insert(struct Owner *owner, u32 prio, u32 arg);
void task_remove(struct Owner *owner, u32 arg);
u8 *heap_alloc_raw(u32 a, u32 size, u8 *tag);
void heap_free(u8 *p);
void free_ptr_pair(u8 *p, u32 n);
void release_obj_parts(u8 *p, u32 n);
void unpack_stream_values(u8 *a, u32 *stream, u32 *p1, u32 *p2, u32 *p3);
void tween_move_to(struct ObjList *list, u32 index, u32 flags, u32 one, u32 v1, u32 v2, s32 v3);
void release_obj_131c(u32 handle);
u32 claim_table_slot(u32 table, u32 a, u32 b, u32 c, s32 d, s32 e, u32 f);
void update_slot_bounds(u32 table, u32 index);

struct SprBits11
{
    u8 lo : 6;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct FlagBit0
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct SprBits12
{
    u8 lo : 4;
    u8 b4 : 1;
    u8 hi : 3;
};

// Sets the mirror bit (bit 6 of the sprite byte at +0x11).
static inline void spr_set_b6(u8 *spr, u32 flag)
{
    ((struct SprBits11 *)(spr + 0x11))->b6 = flag;
}

void set_tween_anim(struct ObjList *list, u32 a, u32 b, u32 c);
void slot_load_object(struct Owner *owner, u32 index);
void slot_store_object(struct Owner *owner, u32 index);
void ensure_tween_sprite(struct ObjList *list, u32 a, u32 b);
u8 flags_vars_set_obj_state(u8 *owner1C, u32 a2, u8 *obj, u32 b, u32 c);
u32 sub_08122768(struct ObjList *list);
u32 tween_step(struct ObjList *list, u32 id);
void start_sub_obj(struct ObjList *list);
void start_sub_obj_display(struct ObjList *list);
void release_obj_slots(struct ObjList *list);
void update_sprite_obj(struct ObjList *list);
u32 restore_obj_word0(u8 *obj, u32 arg);
void reset_obj_params_b(u8 *obj, u8 *state, u32 arg, u32 zero, u32 one, u32 ff);
u32 record_advance(struct Owner *owner, u8 *rec);
u32 record_dispatch(struct Owner *owner, u8 *rec, u32 *buf);
void decode_stream_run(u8 *rec, u32 *buf, u8 *block, u8 *p3, u8 *p4, u32 advanced);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);

void reset_obj_params_a(u8 *obj, u8 *state, u32 arg, u32 zero, u32 one, u32 ff);
void draw_text_prim(void *a, u32 b, s32 c, s32 d, u32 e, u32 f, u32 g, u32 h, u32 i, u32 j);
u32 lang_table_pair(u8 *entry, u32 lo);
extern u8 gTextWidthTable[];

u32 cmd_set_value(struct Owner *owner, struct CmdRec *rec, u32 *data, u8 cur)
{
    u8 *target = (u8 *)owner->state + data[1] * 0xA8 + 0xA8;
    u32 mode = 1;

    if (data[0] & 1)
    {
        rec->flagsA0 = rec->flagsA0 | 4;
        mode = 9;
    }
    switch (data[0])
    {
    case 0:
    case 1:
        reset_obj_params_b((u8 *)owner + 0x1C, target, *(u32 *)rec, data[2], mode, cur);
        *(u32 *)rec = data[2];
        break;
    case 2:
    case 3:
        reset_obj_params_b((u8 *)owner + 0x1C, target, data[2], 0, mode, cur);
        break;
    case 4:
    case 5:
        if ((((target[0xA0] ^ 1) & 1)))
            reset_obj_params_b((u8 *)owner + 0x1C, target, *(u32 *)rec, data[2], mode, cur);
        else
            reset_obj_params_a((u8 *)owner + 0x1C, target, *(u32 *)rec, data[2], mode, cur);
        *(u32 *)rec = data[2];
        break;
    case 6:
    case 7:
        if ((((target[0xA0] ^ 1) & 1)))
            reset_obj_params_b((u8 *)owner + 0x1C, target, data[2], 0, mode, cur);
        else
            reset_obj_params_a((u8 *)owner + 0x1C, target, data[2], 0, mode, cur);
        break;
    default:
        break;
    }
    return !(rec->flagsA0 & 4);
}




NONMATCH("asm/code_080FA598/cmd_slot_value_by_kind.s", u32 cmd_slot_value_by_kind(struct Owner *owner, u32 unused, u32 *data))
{
    u8 *obj;

    switch (data[0])
    {
    case 0:
    case 1:
    {
        struct ObjList *list = owner->list;
        u32 cur = owner->state->cur9A0;
        struct Slot *slot = &list->slots[cur - 1];

        ((struct FlagBit0 *)&slot->flags)->b0 = data[0] & 1;
        return 1;
    }
    case 2:
        obj = owner->list->slots[owner->state->cur9A0 - 1].obj;
        *(u16 *)(obj + 0xE) = 0;
        return 1;
    case 3:
        obj = owner->list->slots[owner->state->cur9A0 - 1].obj;
        *(u16 *)(obj + 0xE) = 0xF000;
        return 1;
    case 4:
        obj = owner->list->slots[owner->state->cur9A0 - 1].obj;
        *(u16 *)(obj + 0xE) = 0x8000;
        return 1;
    default:
        return 1;
    }
}
END_NONMATCH

NONMATCH("asm/code_080FA598/cmd_slot_op.s", u32 cmd_slot_op(struct Owner *owner, u32 unused, u32 *data))
{
    u8 index = owner->state->cur9A0 - 1;
    u32 kind = *data++;

    switch (kind)
    {
    case 0:
    {
        u32 value = *data;

        if (value & 0x40)
            owner->list->slots[index].scale = (value & 0xF) << 4;
        else
            owner->list->slots[index].scale = value << 8;
        break;
    }
    case 1:
    {
        u32 value = *data;

        if (value & 0x40)
        {
            u8 *obj = owner->list->slots[index].obj;
            u32 low = value & 0xF;

            obj[0x20] = low;
        }
        else
        {
            u8 *obj = owner->list->slots[index].obj;
            u32 high = value << 4;

            obj[0x20] = high;
        }
        break;
    }
    case 2:
    {
        u8 *obj = owner->list->slots[index].obj;
        u8 *src = owner->list->slots[*data].obj;
        u16 e = *(u16 *)(src + 0xE);

        if (e != 0)
            *(u16 *)(obj + 0xE) = e - 1;
        break;
    }
    case 3:
    {
        u8 *obj = owner->list->slots[index].obj;
        u8 *src = owner->list->slots[*data].obj;
        u16 e = *(u16 *)(src + 0xE);

        if (e > 0xF000)
            *(u16 *)(obj + 0xE) = e + 1;
        break;
    }
    case 4:
        owner->list->slots[index].unk3D = *data;
        break;
    case 5:
    {
        u8 *obj = owner->list->slots[index].obj;
        u32 value = *data & 3;
        u32 shifted = value << 2;
        s32 bits = obj[0x1F];

        bits = (bits & ~0xC) | shifted;
        bits = (bits & ~3) | value;
        obj[0x1F] = bits;
        break;
    }
    }
    return 1;
}
END_NONMATCH

u32 cmd_slot_field(struct Owner *owner, u32 unused, u32 *data)
{
    u8 index = *data++;
    u32 kind = *data++;

    switch (kind)
    {
    case 0:
    {
        u32 value = *data;

        if (value & 0x40)
            owner->list->slots[index].scale = (value & 0xF) << 4;
        else
            owner->list->slots[index].scale = value << 8;
        break;
    }
    case 1:
    {
        u32 value = *data;

        if (value & 0x40)
        {
            u8 *obj = owner->list->slots[index].obj;
            u32 low = value & 0xF;

            obj[0x20] = low;
        }
        else
        {
            u8 *obj = owner->list->slots[index].obj;
            u32 high = value << 4;

            obj[0x20] = high;
        }
        break;
    }
    }
    return 1;
}

u8 cmd_place_object(struct Owner *owner, u32 a2, u32 *args)
{
    u8 *spr;

    slot_load_object(owner, (u8)args[0]);
    ensure_tween_sprite(owner->list, (u8)args[0], args[1]);
    set_tween_anim(owner->list, (u8)args[0], args[2], 0);
    slot_store_object(owner, (u8)args[0]);
    spr = owner->list->slots[args[0]].obj;
    ((struct SprBits12 *)(spr + 0x12))->b4 = 0;
    spr_set_b6(spr, args[3]);
    return flags_vars_set_obj_state((u8 *)owner + 0x1C, a2, spr, (u8)args[4], 0);
}

u32 cmd_start_move_list(struct Owner *owner, struct CmdRec *rec, s32 *data)
{
    s32 mode = (data[1] >> 1) & 3;
    s32 one = data[1] & 1;
    u8 index = owner->state->cur9A0 - 1;

    if (data[0] != 0)
    {
        data[2] = data[2] << 4;
        data[3] = data[3] << 4;
    }
    if (mode == 1)
    {
        if (data[4] & 0x40)
            data[4] = (data[4] & 0xF) << 4;
        else
            data[4] = data[4] << 8;
        if (data[4] != 0)
            owner->list->slots[index].scale = data[4];
    }
    tween_move_to(owner->list, index, mode, one, data[2], data[3], (s16)data[4]);
    rec->flagsA0 = rec->flagsA0 | 2;
    return 0;
}

NONMATCH("asm/code_080FA598/cmd_set_position.s", u32 cmd_set_position(struct Owner *owner, u32 unused, u32 *data))
{
    u8 index = owner->state->cur9A0 - 1;
    struct ObjList *list;
    u32 offset;
    s32 oldX;
    s32 oldY;
    s32 rx;
    s32 ry;
    u8 *obj;

    if (data[0] & 2)
    {
        s32 x = data[1] << 4;

        data[1] = x;
        {
            s32 y = data[2] << 4;

            data[2] = y;
            if ((data[0] & 1) == 0)
            {
                data[1] = x + 8;
                data[2] = y + 8;
            }
        }
    }
    if (data[0] & 1)
    {
        list = owner->list;
        oldX = list->slots[index].xFix;
        oldY = list->slots[index].yFix;
        offset = index << 6;
    }
    else
    {
        oldX = 0;
        oldY = 0;
        list = owner->list;
        offset = index << 6;
    }
    list->slots[index].xFix = (data[1] << 8) + oldX;
    list->slots[index].yFix = (data[2] << 8) + oldY;
    obj = list->slots[index].obj;
    rx = oldX;
    if (rx < 0)
        rx += 0xFF;
    *(u16 *)obj = data[1] + (rx >> 8);

    obj = owner->list->slots[index].obj;
    ry = oldY;
    if (ry < 0)
        ry += 0xFF;
    *(u16 *)(obj + 2) = data[2] + (ry >> 8);
    return 0;
}
END_NONMATCH

NONMATCH("asm/code_080FA598/cmd_place_sprite.s", u32 cmd_place_sprite(struct Owner *owner, u32 unused1, u32 unused2, u32 *data))
{
    struct Slot *slot = &owner->list->slots[data[1]];
    s32 sx;
    s32 sy;
    s32 t;
    s32 ix;
    s32 lim;
    s32 tail;
    s32 d;
    s32 dir;
    u32 ofs;
    u32 sx16;
    u32 attr;
    u32 z;
    u32 fix16;
    u32 adj;
    u32 flag;
    u8 tbl;
    u8 low;

    t = slot->xFix;
    if (t < 0)
        t += 0xFF;
    sx = t >> 8;
    t = slot->yFix;
    if (t < 0)
        t += 0xFF;
    sy = t >> 8;
    ofs = slot->unk3D;
    sx16 = (u16)sx;
    tbl = gTextWidthTable[(u8)data[2] << 1];
    attr = data[3] & 0x1F;
    attr |= (data[3] << 1) & 0x3C0;
    attr |= (data[3] << 2) & 0x800;
    z = data[2];
    if ((attr & 3) == 2)
    {
        if (tbl > 3)
            tbl = (tbl + 1) & 0xFE;
        else
            tbl = 4;
    }
    if ((attr & 0x300) == 0)
    {
        dir = 2;
        d = sy - ofs - (((tbl + 3) << 16) >> 13);
        if (d < 0)
            dir = 1;
        attr |= (dir << 8) & 0x300;
    }
    else if (attr & 0x40)
    {
        adj = 0x18;
        if (attr & 0x80)
            adj = 0xFFE8;
        sx16 = (u16)((s16)sx16 + (s16)adj);
    }
    if ((attr & 0x300) == 0x200)
        sy = sy - 0xD - ((tbl + 2) << 3) - ofs;
    else
        sy -= 2;
    flag = 0;
    if (sy < 0)
    {
        sy = 0;
        if (attr & 0x100)
        {
            sy = 0x10;
            flag = 1;
        }
    }
    else
    {
        t = tbl << 3;
        if (sy + t + 0x14 > 0x9F)
        {
            sy = 0x90 - t;
            if (attr & 0x200)
            {
                sy -= 0x10;
                flag = 1;
            }
        }
    }
    if (flag)
        attr = (attr | 0x40) & 0xFFFF;
    ix = ((s32)z >> 8) & 0xFF;
    if (ix == 0)
        ix = (lang_table_pair(owner->entry, data[5]) >> 16) & 0xFF;
    if ((attr & 3) == 2)
    {
        dir = (ix + 2) / 3;
        ix = (dir * 3) & 0xFFFF;
        if (ix & 1)
            sx += 4;
    }
    low = ix;
    if (low == 0)
    {
        ix = 2;
        low = 2;
    }
    if (sx < 0)
    {
        sx16 = 0x20;
        attr &= 0xFF7F;
        attr |= 0x40;
    }
    else if (sx > 0xEF)
    {
        sx16 = 0xD0;
        attr |= 0xC0;
    }
    sx = sx - 0x10 - (ix << 2);
    fix16 = sx16 << 16;
    lim = (s32)fix16 >> 16;
    if (sx + 0x18 <= lim)
    {
        t = ix << 3;
        if (sx + t + 8 < lim)
            sx = lim - 8 - t;
    }
    else
        sx = lim - 0x18;
    lim = (s32)fix16 >> 16;
    if (lim > 23)
    {
        if (lim > 215)
        {
            if ((attr & 0xC0) == 0x40)
                sx16 = (lim - 0x30) & 0xFFFF;
            else
                sx16 = 0xD8;
            sx = 0xD0 - (ix << 3);
            attr |= 0xC0;
            fix16 = sx16 << 16;
            tail = (s16)sx16 - 0x18;
            if (sx > tail)
                sx = tail;
        }
    }
    else
    {
        if ((attr & 0xC0) == 0xC0)
            sx16 = (lim + 0x30) & 0xFFFF;
        else
            sx16 = 0x18;
        sx = 0;
        attr = (attr & 0xFF7F) | 0x40;
        fix16 = sx16 << 16;
        tail = (s16)sx16 - (ix << 1) - 0x18;
        if (sx < tail)
            sx = tail;
    }
    if (sx < 0)
    {
        if ((attr & 3) == 2)
        {
            t = lim;
            if (lim < 0)
                t = lim + 7;
            sx = (s16)(lim - ((t >> 3) << 3));
        }
        else
            sx = 0;
    }
    else
    {
        t = ix << 3;
        if (sx + t + 0x20 > 0xF0)
        {
            t = 0xD0 - t;
            if ((attr & 3) == 2)
            {
                s32 t8 = t - 8;

                lim = (s32)fix16 >> 16;
                t = lim;
                if (lim < 0)
                    t = lim + 7;
                sx = t8 + (s16)(lim - ((t >> 3) << 3));
            }
            else
                sx = t;
        }
    }
    t = (s32)fix16 >> 16;
    t = t - sx;
    attr = (attr - 0x100) & 0xFFFF;
    draw_text_prim(owner->entry, data[0] & 0xFF, (s16)sx, (s16)sy, low, tbl, t & 0xFFFF, attr,
                   (u8)data[4], data[5]);
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_080FA598/cmd_aux_entry.s", u32 cmd_aux_entry(struct Owner *owner, u32 unused, u32 *data))
{
    u8 found;
    u8 index = owner->state->cur9A0 - 1;
    u32 py = 0;
    u32 px = 0;

    if (data[2] != 0)
    {
        struct Slot *slot = &owner->list->slots[index];
        s32 t = slot->xFix;

        if (t < 0)
            t += 0xFF;
        px = t >> 8;
        t = slot->yFix;
        if (t < 0)
            t += 0xFF;
        py = (t >> 8) - slot->unk3D;
    }
    if (data[5] == 0xFF)
        data[5] = gClaimKindTable[data[1]];
    found = claim_table_slot(owner->aux, *(u8 *)data, (u8)data[1], *(u8 *)&data[2],
                             *(s16 *)&data[3], *(s16 *)&data[4], *(u8 *)&data[5]);
    if (found != 0x3F)
    {
        u8 *entry = *(u8 **)owner->aux + (found << 4);

        *(u16 *)(entry + 8) = px;
        *(u16 *)(entry + 0xA) = py;
        update_slot_bounds(owner->aux, found);
    }
    return 1;
}
END_NONMATCH

u32 cmd_latch_when_ready(struct Owner *owner, u32 *cmd)
{
    if ((u8)sub_08122768(owner->list) != 0)
    {
        cmd[0] = cmd[5];
        return 0;
    }
    return 1;
}

u32 cmd_list_toggle(struct Owner *owner, u32 unused, u32 *data)
{
    if (*data & 1)
        start_sub_obj(owner->list);
    else
        start_sub_obj_display(owner->list);
    return 1;
}

u32 stub_return_0(struct ObjList *list);

u32 cmd_set_mode2_or_latch(struct Owner *owner, u32 *cmd)
{
    if ((u8)stub_return_0(owner->list) != 1)
    {
        cmd[0] = cmd[5];
        return 0;
    }
    owner->state->mode9A1 = 2;
    return 1;
}

u32 cmd_start_move(struct Owner *owner, struct CmdRec *rec, u32 *data)
{
    u32 v1;
    u32 v2;
    u32 v3;
    u8 index = owner->state->cur9A0 - 1;

    unpack_stream_values(owner->filler1C, data, &v1, &v2, &v3);
    data += 3;
    tween_move_to(owner->list, index, data[0] & 3, 1, v1, v2, *(s16 *)&data[1]);
    rec->flagsA0 = rec->flagsA0 | 2;
    return 0;
}

u32 cmd_list_mode(struct Owner *owner, u32 unused, u32 *data)
{
    switch (*data)
    {
    case 0:
        release_obj_slots(owner->list);
        break;
    case 1:
        update_sprite_obj(owner->list);
        break;
    }
    return 1;
}

u8 cmd_set_mode_1(struct Owner *owner, u32 arg)
{
    owner->state->mode9A1 = 1;
    return restore_obj_word0((u8 *)owner + 0x1C, arg);
}

u32 cmd_latch_when_free(struct Owner *owner, u32 *cmd)
{
    u32 own = ((u32)owner->state->busy9A2 << 31) >> 31;

    if ((gUnk_03000C80[0] | own) != 0)
    {
        cmd[0] = cmd[5];
        return 0;
    }
    return 1;
}

NONMATCH("asm/code_080FA598/cmd_push_handle.s", u32 cmd_push_handle(struct Owner *owner, u32 unused, u32 *data))
{
    struct OwnerState *state;
    struct FlagBit0 *bits;
    u8 count;
    u32 shifted;
    u32 handle;

    handle = (u32)create_obj_de4(0, data[0], 1, -1, -1, -1, -1);
    obj_load_record((void *)handle, 0, -1, 0, 0);
    state = owner->state;
    bits = (struct FlagBit0 *)&state->busy9A2;
    count = state->busy9A2;
    shifted = (u32)count << 24;
    bits->rest = (shifted >> 25) + 1;
    state->handles[shifted >> 25] = handle;
    ((struct FlagBit0 *)&owner->state->busy9A2)->b0 = 1;
    return 0;
}
END_NONMATCH
