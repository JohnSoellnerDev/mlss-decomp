#include "global.h"
#include "ram_routines.h"

struct World
{
    u8 filler0[4];
    u8 unk4;
    u8 filler5[0x24 - 5];
    void (*callback24)(void);
    u8 filler28[0x58 - 0x28];
    u8 *slots[8];
    u8 *unk78;
    u8 *curObj;
    u8 *unk80;
    u8 *unk84;
    u8 filler88[0x248 - 0x88];
    u8 *unk248;
    s32 angle;
    u8 filler250[0x2B4 - 0x250];
    u16 unk2B4;
    u8 filler2B6[0x2BF - 0x2B6];
    u8 unk2BF;
    u8 filler2C0[0x2E0 - 0x2C0];
    u32 unk2E0[3];
    u8 filler2EC[0x2EC - 0x2EC];
    u16 unk2EC;
    u16 unk2EE;
    u8 filler2F0[0x310 - 0x2F0];
    u8 *unk310;
    u8 filler314[0x346 - 0x314];
    u8 unk346;
    u8 filler347[0x350 - 0x347];
    u8 unk350;
    u8 unk351;
    u8 filler352[0x354 - 0x352];
    u16 unk354;
    u16 unk356;
    u16 unk358;
    u16 unk35A;
    u16 unk35C;
    u16 unk35E;
};

extern struct World *gWorld;

struct SlotByteFD
{
    u8 filler0[0xFD];
    u8 lo : 4;
    u8 hi : 4;
};

struct WorldBits2BF
{
    u8 filler0[0x2BF];
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct OwnerState
{
    u8 filler0[0x179C];
    u8 slotCode;
};

struct Owner
{
    u8 filler0[0x2C];
    struct OwnerState *state;
};

void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, u8 *owner);
void list_splice_pair(u8 *entry, u8 *head);
s32 anim_set_motion_a(u8 *obj, u16 speed, s32 extra);
s32 anim_set_motion_b(u8 *obj, u16 speed, s32 extra);
s32 anim_set_motion_c(u8 *obj, u16 speed, s32 extra);
void obj_stop(u8 *obj);
void obj_draw_part(u8 *obj, s32 amount, s32 kind, s32 a3, s32 a4, s32 a5);
s32 obj_last_x(s32 angle);
s32 obj_last_y(s32 angle);
extern u8 *gWorkBuf;
extern u8 gUnk_02000080[];
void menu_open_setup(void);
void sub_08115F54(u8 *a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
extern u8 gClaimKindTable[];
void slot_pos_offset(u8 *obj, s32 idx, s32 *x, s32 *y, s32 z);
s32 cmd_spawn_obj(u8 *table, u32 art, u32 sub, u32 width, s32 a, s32 b, s32 c);
void state_status_release(u8 *value);
void release_marker_object(void);
void spawn_linked_children(void);
void spawn_timer_child(void);
void sprite_set_action(u8 *obj, u32 a, u32 b, u32 c);
void obj_track_target(u8 *target);
void state_update_hero_markers(void);
void anim_dispatch(u8 *obj, s32 value, u32 a, u32 b);
struct PartTemplate
{
    u8 filler0[8];
    u8 *sprite;
    u8 filler0C[0x2C - 0x0C];
};

extern const struct PartTemplate gPartTemplates[];
u32 lookup_or_alloc_13b4(u32 a, u32 key, s32 b, s32 c);
void release_block(void *block);
void place_part_in_slot(u32 effect, s32 slot, s32 x, s32 y, s32 z, s32 a, s32 b);
void noop_callback_0808261C(void);
void run_slot_script(u32 index);
void sprite_entry_update(u8 *obj);
void release_object(u8 *obj);
void release_object_data(u8 *obj);
void task_list_remove(u8 *obj);
void process_record_ring(u8 *obj);

ASM_FUNC("asm/code_080F5884/cmd_set_var_from_slot_field.s", void cmd_set_var_from_slot_field(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_set_field.s", void cmd_slot_set_field(void))

NONMATCH("asm/code_080F5884/cmd_slot_link_list.s", u32 cmd_slot_link_list(u32 unused0, u32 unused1, u32 *cmd))
{
    u32 *cursor;
    u8 *obj;
    u8 *entry;
    s32 f;
    s32 mode;
    s32 s;
    s32 bit;
    u8 *flags;

    cursor = cmd;
    entry = gWorld->slots[*cursor++];
    obj = entry + 8;
    mode = *cursor++;
    switch (mode)
    {
    case 0:
        flags = entry + 0x80;
        f = *flags;
        bit = (((u32)f << 29) >> 31) << 3;
        f = (f & -9) | bit;
        f = f | 4;
        *flags = f;
        effect_spawn_at(*cursor, *(s32 *)(obj + 0x38) / 0x100, *(s32 *)(obj + 0x3C) / 0x100,
                        *(s32 *)(obj + 0x40) / 0x100, obj);
        break;
    case 1:
        list_splice_pair(entry, gWorld->slots[*cursor]);
        break;
    case 2:
        s = *cursor;
        *(u16 *)(entry + 0x124) = s;
        break;
    }
    return 1;
}
END_NONMATCH

u32 slot_cmds_cmd_slot_set_motion(u32 unused0, u32 unused1, u32 *cmd)
{
    struct World *world;
    u32 *cursor;
    u8 *entry;
    s32 mode;

    cursor = cmd;
    world = gWorld;
    entry = world->slots[*cursor++];
    mode = *cursor++;
    switch (mode)
    {
    case 0:
        *(u16 *)(entry + 0x11E) = *(u16 *)(entry + 0x11E) & 0xFFF3;
        anim_set_motion_a(entry, *(u16 *)cursor, 0x7FFFFFFF);
        break;
    case 1:
        *(u16 *)(entry + 0x11E) = *(u16 *)(entry + 0x11E) & 0xFFCF;
        anim_set_motion_b(entry, *(u16 *)cursor, 0x7FFFFFFF);
        break;
    case 2:
        *(u16 *)(entry + 0x11E) = *(u16 *)(entry + 0x11E) & 0xFF3F;
        anim_set_motion_c(entry, *(u16 *)cursor, 0x7FFFFFFF);
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F5884/cmd_slot_spawn_a.s", void cmd_slot_spawn_a(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_spawn_b.s", void cmd_slot_spawn_b(void))

u32 cmd_slot_release(u32 unused0, u8 *dst, u32 *cmd)
{
    u32 *cursor;
    u32 action;
    u8 *obj;
    u8 *p;
    u8 flag;

    cursor = cmd;
    action = *cursor++;
    switch (action)
    {
    case 0:
        p = gWorld->slots[cursor[0]];
        place_part_in_slot(cursor[0], *(s16 *)(p + 0xEC), *(s32 *)(p + 0xD8) / 0x100, *(s32 *)(p + 0xDC) / 0x100,
                           *(s32 *)(p + 0xE0) / 0x100, ((u32)p[0x110] << 25) >> 31, 0);
        run_slot_script(cursor[0]);
        sprite_entry_update(*(u8 **)(p + 0x10));
        break;
    case 1:
        obj = gWorld->slots[cursor[0]];
        flag = obj[0x111] & 2;
        if (flag != 0)
        {
            *(u32 *)dst = *(u32 *)(dst + 0x14);
            return 0;
        }
        obj_stop(obj + 8);
        *(u16 *)(obj + 0x11E) = flag;
        break;
    case 3:
        release_object_data(gWorld->slots[cursor[0]]);
        break;
    case 2:
        obj = gWorld->slots[cursor[0]];
        release_object(obj);
        task_list_remove(obj);
        break;
    case 4:
        release_object(gWorld->slots[cursor[0]]);
        break;
    case 5:
        obj = gWorld->slots[cursor[0]];
        flag = obj[0x111] & 2;
        if (flag != 0)
        {
            *(u32 *)dst = *(u32 *)(dst + 0x14);
            return 0;
        }
        process_record_ring(obj + 8);
        *(u16 *)(obj + 0x11E) = flag;
        break;
    }
    return 1;
}

u32 cmd_slot_move_step(u32 unused0, u8 *dst, u32 *cmd)
{
    u8 *entry;
    s32 mode;
    s32 amount;
    s32 kind;
    s32 lower;
    s32 upper;

    entry = gWorld->slots[*cmd++];
    mode = *cmd++;
    switch (mode)
    {
    case 0:
        amount = *cmd;
        kind = 0;
        if (*(s16 *)(entry + 0xF6) <= amount)
        {
            if (entry[0x111] & 2)
            {
                *(u32 *)dst = *(u32 *)(dst + 0x14);
                return 0;
            }
            obj_stop(entry + 8);
            *(u16 *)(entry + 0xF6) = kind;
            *(u16 *)(entry + 0x11E) = kind;
        }
        else
        {
            *(u16 *)(entry + 0xF6) = *(u16 *)(entry + 0xF6) - amount;
        }
        break;
    case 1:
        amount = *cmd;
        kind = 3;
        lower = *(s16 *)(entry + 0xF6);
        if (lower + amount > (upper = *(s16 *)(entry + 0xF8)))
        {
            amount = upper - lower;
        }
        *(u16 *)(entry + 0xF6) = *(u16 *)(entry + 0xF6) + amount;
        break;
    }
    obj_draw_part(entry + 8, amount, kind, 0, 0, 0);
    return 1;
}


NONMATCH("asm/code_080F5884/cmd_slot_set_bits.s", u32 cmd_slot_set_bits(u32 unused0, u32 unused1, u32 *cmd))
{
    u8 *cur;
    u8 *q;
    s32 mode;

    cur = gWorld->curObj;
    q = *(u8 **)(cur + 0x38);
    mode = *cmd;
    switch (mode)
    {
    case 0:
    case 2:
    {
        u8 *ptr;

        cur[0x80] |= 0x10;
        ptr = *(u8 **)(cur + 0x10);
        if (ptr != 0)
        {
            ptr[0x12] |= 0x10;
        }
        if (q != 0)
        {
            q[0x78] |= 0x10;
            ptr = *(u8 **)(q + 8);
            if (ptr != 0)
            {
                ptr[0x12] |= 0x10;
            }
        }
    }
        break;
    case 1:
    case 3:
    {
        u8 *ptr;
        s32 mask;

        mask = -0x11;
        cur[0x80] = cur[0x80] & mask;
        ptr = *(u8 **)(cur + 0x10);
        if (ptr != 0)
        {
            ptr[0x12] = ptr[0x12] & mask;
        }
        if (q != 0)
        {
            q[0x78] = q[0x78] & mask;
            ptr = *(u8 **)(q + 8);
            if (ptr != 0)
            {
                ptr[0x12] = ptr[0x12] & mask;
            }
        }
    }
        break;
    }
    return 1;
}
END_NONMATCH


NONMATCH("asm/code_080F5884/cmd_slot_set_bit_pair.s", u32 cmd_slot_set_bit_pair(u32 unused0, u8 *dst, u32 *cmd))
{
    u8 *p;
    s32 w1;
    s32 mode;

    if (cmd[0] == 0x3F)
    {
        cmd[0] = ((struct SlotByteFD *)dst)->lo;
    }
    p = gWorld->slots[*cmd++];
    w1 = *cmd;
    mode = w1 >> 1;
    switch (mode)
    {
    case 0:
    {
        u8 *q;
        u8 *r;
        s32 bit;
        s32 hi;
        s32 lo;
        s32 flags;

        bit = w1 & 1;
        q = p + 0x7F;
        hi = bit << 7;
        *q = (*q & 0x7F) | hi;
        r = p + 0x80;
        flags = *r;
        flags = (flags & -2) | bit;
        lo = bit << 1;
        flags = (flags & -3) | lo;
        *r = flags;
    }
        break;
    case 1:
    {
        s32 flags;
        s32 both;
        s32 hi;
        s32 lo;

        both = w1 & mode;
        p += 0x80;
        hi = both << 2;
        flags = *p;
        flags = (flags & -5) | hi;
        lo = both << 3;
        flags = (flags & -9) | lo;
        *p = flags;
    }
        break;
    }
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_080F5884/cmd_world_slot.s", u32 cmd_world_slot(u32 unused0, u32 unused1, u32 *cmd))
{
    s32 idx;
    u32 *cursor;
    s32 kind;
    u32 old;
    u32 block;
    u8 *entry;

    cursor = cmd;
    kind = *cursor++;
    switch (kind)
    {
    case 0:
    {
        u32 field;

        if (cursor[0] == 0)
        {
            field = (u32)gWorld->unk2EC << 22;
        }
        else if (cursor[0] == 1)
        {
            field = *(u32 *)&gWorld->unk2EC << 12;
        }
        else
        {
            field = (u32)gWorld->unk2EE << 18;
        }
        idx = field >> 22;
        old = gWorld->unk2E0[cursor[0]];
        if (old != 0)
        {
            release_block((void *)old);
        }
        block = lookup_or_alloc_13b4(*(u16 *)gPartTemplates[cursor[1]].sprite, idx, -1, 1);
        gWorld->unk2E0[cursor[0]] = block;
    }
        break;
    case 1:
        if ((s32)cursor[0] >= 0)
        {
            noop_callback_0808261C();
        }
        break;
    case 2:
        entry = gWorld->slots[cursor[1]];
        place_part_in_slot(cursor[1], cursor[0], *(s32 *)(entry + 0xD8) / 0x100, *(s32 *)(entry + 0xDC) / 0x100,
                           *(s32 *)(entry + 0xE0) / 0x100, 1, cursor[2] == 0);
        run_slot_script(cursor[1]);
        break;
    }
    return 1;
}
END_NONMATCH


ASM_FUNC("asm/code_080F5884/cmd_slot_table.s", void cmd_slot_table(void))

NONMATCH("asm/code_080F5884/slot_follow_source.s", u32 slot_follow_source(struct Owner *owner, u8 *obj, u32 *cmd))
{
    u8 srcIdx;
    u8 *dst;
    u8 *src;
    u8 *dstPos;
    u8 *srcPos;
    s32 d;
    u16 v;
    u16 *field;

    if (owner->state->slotCode <= 6)
    {
        srcIdx = ((struct SlotByteFD *)obj)->lo;
    }
    else
    {
        srcIdx = owner->state->slotCode - 7;
    }
    dst = gWorld->slots[*cmd];
    dstPos = dst + 8;
    src = gWorld->slots[srcIdx];
    srcPos = src + 8;
    *(u32 *)(dst + 0xBC) = *(u32 *)(src + 0xBC);
    v = *(u16 *)(src + 0xC0);
    *(u16 *)(dst + 0xC0) = v;
    *(u16 *)(dst + 0xC2) = *(u16 *)(src + 0xC2);
    field = (u16 *)(src + 0xC4);
    d = *(s32 *)(dstPos + 0x38) - *(s32 *)(srcPos + 0x38);
    if (d < 0)
    {
        d += 0xFF;
    }
    v = (d >> 8) + *field;
    *(u16 *)(dst + 0xC4) = v;
    field = (u16 *)(src + 0xC6);
    d = *(s32 *)(dstPos + 0x3C) - *(s32 *)(srcPos + 0x3C);
    if (d < 0)
    {
        d += 0xFF;
    }
    v = (d >> 8) + *field;
    *(u16 *)(dst + 0xC6) = v;
    field = (u16 *)(src + 0xC8);
    d = *(s32 *)(dstPos + 0x40) - *(s32 *)(srcPos + 0x40);
    if (d < 0)
    {
        d += 0xFF;
    }
    v = (d >> 8) + *field;
    *(u16 *)(dst + 0xC8) = v;
    *(u32 *)(dst + 0xBC) = 0;
    return 1;
}
END_NONMATCH


ASM_FUNC("asm/code_080F5884/cmd_set_ctx_fields.s", void cmd_set_ctx_fields(void))


ASM_FUNC("asm/code_080F5884/cmd_world_scroll.s", void cmd_world_scroll(void))

NONMATCH("asm/code_080F5884/cmd_set_scroll.s", u32 cmd_set_scroll(u32 unused0, u8 *obj, u32 *cmd))
{
    struct World *world;
    u32 *cursor;
    u32 flags;
    s32 bits;
    s32 add;
    u16 *a0;
    u16 old;

    gWorld->unk354 = obj_last_x(gWorld->angle);
    gWorld->unk356 = obj_last_y(gWorld->angle);
    world = gWorld;
    cursor = cmd;
    world->unk358 = *cursor++;
    world->unk35A = *cursor++;
    flags = *cursor;
    if (flags & 0x40)
    {
        bits = flags & 0xF;
    }
    else
    {
        bits = flags << 4;
    }
    world->unk35C = bits;
    gWorld->unk35E = 0;
    old = *(u16 *)(obj + 0xA0);
    *(u16 *)(obj + 0xA0) = old | (cursor[1] ? 0x500 : 0x100);
    return 0;
}
END_NONMATCH

u32 cmd_flags_show(u32 unused0, u32 unused1, u32 *cmd)
{
    u32 flags;
    s32 limit;
    u8 *sub;

    flags = *cmd;
    if ((flags & 1) != 0)
    {
        if ((flags & 4) != 0 || gWorld->unk4 != 0)
        {
            MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gUnk_02000080, gWorkBuf + 0x3AF0, 0x400);
            ((struct WorldBits2BF *)gWorld)->bit3 = 1;
            sub = gWorld->unk248;
            if ((*cmd & 2) != 0)
            {
                limit = 0x7FFF;
            }
            else
            {
                limit = 0;
            }
            sub_08115F54(sub, 4, limit, 0x10, 0xFFFF, 0xFFFF, 0, 0);
        }
        else
        {
            menu_open_setup();
        }
    }
    else
    {
        ((struct WorldBits2BF *)gWorld)->bit5 = 1;
    }
    return 1;
}

u32 list_slot_ops_cmd_world_mode(u32 unused0, u32 unused1, s32 *cmd)
{
    u8 *a;

    switch (*cmd)
    {
    case 0:
        gWorld->unk350 = gWorld->unk351 = 0x80;
        break;
    case 1:
        state_status_release(gWorld->unk310);
        release_marker_object();
        spawn_linked_children();
        spawn_timer_child();
        sprite_set_action(gWorld->curObj + 8, 0, 0, 0);
        gWorld->unk346 = 0xFF;
        a = gWorld->unk80;
        if (a != 0 && *(s16 *)(a + 0xF6) != 0)
        {
            obj_track_target(a + 8);
        }
        a = gWorld->unk84;
        if (a != 0 && *(s16 *)(a + 0xF6) != 0)
        {
            obj_track_target(a + 8);
        }
        gWorld->callback24 = state_update_hero_markers;
        break;
    case 2:
        gWorld->unk2B4 = 0;
        *((s8 *)gWorld + 0x2C1) &= -9;
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F5884/draw_list_item.s", void draw_list_item(void))

ASM_FUNC("asm/code_080F5884/cmd_spawn_item.s", void cmd_spawn_item(void))

