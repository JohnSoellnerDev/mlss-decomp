#include "global.h"

struct Effect
{
    u8 filler0[8];
    u16 unk8;
    u16 unkA;
};

struct OwnerState
{
    u8 filler0[0x1788];
    u32 flags1788;
    u8 filler178C[0x1798 - 0x178C];
    u32 flags1798;
    u8 filler179C[0x179D - 0x179C];
    u8 state179D;
};

struct Owner
{
    u8 filler0[0x2C];
    struct OwnerState *state;
};

struct SlotObj
{
    u8 filler0[8];
    u8 *obj8;
    u8 filler0C[0x10 - 0xC];
    s32 drawX;
    s32 drawY;
    s32 drawZ;
    u8 filler1C[0x28 - 0x1C];
    struct RingBlock *ring;
    u8 filler2C[0x38 - 0x2C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x76 - 0x44];
    u8 state76;
    u8 filler77[0x79 - 0x77];
    u8 flag79;
    s16 timer7A;
    u16 unk7C;
    u8 filler7E[0x84 - 0x7E];
    s32 vecX;
    s32 vecY;
    s32 vecZ;
    u8 filler90[0xC5 - 0x90];
    u8 biasC5;
};
struct RingEntry
{
    u32 unk0;
    u8 filler4[0xC - 0x4];
    u8 keyC;
    u8 filler[0xF - 0xD];
    u8 flagF;
};

struct RingBlock
{
    struct RingEntry *entries;
};

struct ObjFlags
{
    u8 filler0[0xA0];
    u16 flagsA0;
    u8 filler[0xFE - 0xA2];
    u8 bitsFE;
};

struct HitRec
{
    u8 filler0[0x24];
    u16 h24;
    u16 h26;
};

struct Slot
{
    u8 filler0[0x10];
    u32 unk10;
    u8 filler14[0x7E - 0x14];
    u8 flags7E;
    u8 filler7F[0xD8 - 0x7F];
    u32 unkD8;
    u32 unkDC;
    u32 unkE0;
    u8 fillerE4[0xEC - 0xE4];
    s16 keyEC;
    struct HitRec *recF0;
    u8 fillerF4[0xF6 - 0xF4];
    s16 unkF6;
    s16 unkF8;
    u8 fillerFA[0x110 - 0xFA];
    u8 flag110;
};

struct World
{
    u8 filler0[0x16];
    u16 pos16;
    u16 pos18;
    u8 filler1A[0x1C - 0x1A];
    u8 *ptr1C;
    u8 filler20[0x24 - 0x20];
    void *callback24;
    u8 filler28[0x58 - 0x28];
    struct Slot *slots[8];
    u8 filler78[0x7C - 0x78];
    struct Cam *cam;
    u8 filler80[0x244 - 0x80];
    struct Owner *cur;
    void *ptr248;
    u8 filler24C[0x2BF - 0x24C];
    u8 flags2BF;
    u8 filler2C0[0x2D8 - 0x2C0];
    struct Effect *effect;
    u8 filler2DC[0x2F0 - 0x2DC];
    u32 word2F0;
    u32 pending2F4;
    u8 flag2F8;
    u8 filler2F9[0x2FC - 0x2F9];
    u32 table2FC[5];
    void *ptr310;
    u8 filler314[0x318 - 0x314];
    u32 bits318;
    u8 filler31C[0x342 - 0x31C];
    u8 unk342;
    u8 unk343;
    u8 unk344;
    u8 unk345;
    u8 filler346[0x350 - 0x346];
    u8 unk350[0x3A6 - 0x350];
    u8 count3A6;
};

struct Cam
{
    u8 filler0[0x34];
    struct CamTarget *target;
};

struct CamTarget
{
    u8 filler0[0x28];
    u8 *ptr28;
};

struct CopyNode
{
    u32 unk0;
    u8 filler4[0x14 - 0x4];
    u32 unk14;
};

struct TaskObj
{
    u8 filler0[0xE8];
    u32 unkE8;
    u8 filler[0xF4 - 0xEC];
    u8 unkF4;
};

struct SubStartCmd
{
    u32 mode;
    u32 arg;
    u16 unk8;
    u16 unkA;
    u16 unkC;
};

extern struct World *gWorld;
extern u8 *gWorkBuf;

u8 is_state_busy(void);
void pool_slot_init(u32 a, u32 b);
void apply_offset_row(u32 value, u32 delta);
void draw_sprite_small(u32 a, u32 b, u32 c);
void draw_sprite_block(u32 a, u32 b, u32 c, u32 d, u32 e);
void display_finish(u32 a, s32 b);
void display_pause(void);
void update_slot_markers(void);
void release_slot_objects(void);
void task_slot_write(struct Owner *cur, u32 kind, u32 entry);
void obj_detach_active(void *obj);
void obj_release(void *obj);
void record_slot_hit(struct Slot *slot, struct World *world);
void state_hero_menu_plain(void);
u8 sub_obj_running(void *menu);
u8 world_entry_ready(void);
void start_motion(void *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, void *owner);
void slot_set_timer(s16 idx, u32 value);
void task_spawn_wait_idle(void *obj);

ASM_FUNC("asm/code_080F7100/refresh_slot_view.s", void refresh_slot_view(void))

ASM_FUNC("asm/code_080F7100/slot_apply_anim.s", void slot_apply_anim(void))

ASM_FUNC("asm/code_080F7100/slot_reload_record.s", void slot_reload_record(void))

ASM_FUNC("asm/code_080F7100/slot_set_value.s", void slot_set_value(void))

ASM_FUNC("asm/code_080F7100/slot_get_field.s", void slot_get_field(void))

u32 world_frame_index(void)
{
    s32 idx;
    u32 word;
    if (gWorld->ptr310 == 0)
        return -1;
    idx = *(s16 *)((u8 *)gWorld->ptr310 + 0x10);
    word = gWorld->table2FC[idx];
    if (word == 0)
        return -1;
    if (gWorld->cam->target != 0)
    {
        if (gWorld->callback24 != (void *)state_hero_menu_plain)
            return -1;
        return gWorld->cam->target->ptr28[0xF4] + 0x11;
    }
    if (*(u32 *)((u8 *)word + 0x9C) == 5)
        return 0x10;
    if ((gWorld->unk343 & 0x80) == 0)
        return gWorld->unk343 + 4;
    if ((gWorld->unk344 & 0x80) == 0)
        return gWorld->unk344 + 7;
    if ((gWorld->unk345 & 0x80) == 0)
        return gWorld->unk345 + 0x19;
    if ((gWorld->unk342 & 0x80) == 0)
        return gWorld->unk342;
    return -1;
}

u32 world_frame_index_b(void)
{
    s32 idx;
    u32 word;
    if (gWorld->ptr310 == 0)
        return -1;
    idx = *(s16 *)((u8 *)gWorld->ptr310 + 0x10);
    word = gWorld->table2FC[idx];
    if (word == 0)
        return -1;
    if (gWorld->cam->target != 0)
    {
        if (gWorld->callback24 != (void *)state_hero_menu_plain)
            return -1;
        return gWorld->cam->target->ptr28[0xF4] + 0x11;
    }
    if (*(u32 *)((u8 *)word + 0x9C) == 5)
        return 0x10;
    if ((gWorld->unk344 & 0x80) == 0)
        return gWorld->flags2BF >> 6;
    return -1;
}

ASM_FUNC("asm/code_080F7100/apply_slot_reward.s", void apply_slot_reward(void))

ASM_FUNC("asm/code_080F7100/change_item_count.s", void change_item_count(void))

ASM_FUNC("asm/code_080F7100/record_slot_hit.s", void record_slot_hit(struct Slot *slot, struct World *world))

void set_cur_flags_pending(u32 bits)
{
    struct OwnerState *st = gWorld->cur->state;
    st->flags1788 |= bits;
    st->flags1798 |= bits;
    if ((gWorld->pending2F4 & bits) != 0)
    {
        gWorld->pending2F4 ^= bits;
        gWorld->flag2F8 = 1;
    }
}

void clear_cur_flags(u32 bits)
{
    gWorld->cur->state->flags1788 &= ~bits;
}

void set_cur_flags(u32 bits)
{
    gWorld->cur->state->flags1788 |= bits;
}

void set_cur_flags_both(u32 bits)
{
    struct OwnerState *st = gWorld->cur->state;
    st->flags1788 |= bits;
    st->flags1798 |= bits;
}

struct Bit0Bits
{
    u8 b0 : 1;
    u8 hi7 : 7;
};

void set_world_flag0(u8 flag)
{
    if (flag != 0)
        update_slot_markers();
    else
        release_slot_objects();
    ((struct Bit0Bits *)&gWorld->flags2BF)->b0 = flag & 1;
}

NONMATCH("asm/code_080F7100/slot_start_motion.s", void slot_start_motion(s16 idx, u8 flag))
{
    struct SlotObj *obj;
    if (idx <= 7)
        obj = (struct SlotObj *)((u8 *)gWorld->slots[idx] + 8);
    else
        obj = (struct SlotObj *)(gWorkBuf + idx * 0xD0 + 0x300);
    start_motion(obj);
    task_spawn_wait_idle(obj);
    if (obj->ring != 0 && flag != 0)
    {
        u8 *sub = (u8 *)obj->ring;
        *(s32 *)(sub + 0xD8) = obj->vecX;
        *(s32 *)(sub + 0xDC) = obj->vecY;
        *(s32 *)(sub + 0xE0) = obj->vecZ;
    }
}
END_NONMATCH

NONMATCH("asm/code_080F7100/slot_set_timer.s", void slot_set_timer(s16 idx, u32 value))
{
    struct SlotObj *obj;
    if (idx <= 7)
        obj = (struct SlotObj *)((u8 *)gWorld->slots[idx] + 8);
    else
        obj = (struct SlotObj *)(gWorkBuf + 0x300 + idx * 0xD0);
    obj->timer7A = value;
    obj->unk7C = 0;
}
END_NONMATCH

ASM_FUNC("asm/code_080F7100/slot_set_vec.s", void slot_set_vec(void))

void obj_task_attach(struct TaskObj *obj)
{
    s32 bits;
    task_slot_write(gWorld->cur, obj->unkF4, obj->unkE8);
    bits = gWorld->flags2BF;
    bits |= 4;
    gWorld->flags2BF = bits;
}

ASM_FUNC("asm/code_080F7100/obj_task_attach_idx.s", void obj_task_attach_idx(void))

u32 slot_widest_span(void *unused0, s32 threshold)
{
    s32 best = threshold;
    u32 bestIdx = 0xFFFFFFFF;
    s16 i;
    for (i = 0; i <= 5; i++)
    {
        struct Slot *s = gWorld->slots[i];
        if (s != 0 && (s->flags7E & 6) == 4)
        {
            s32 span = s->unkF8 - s->unkF6;
            if (span > best)
            {
                best = span;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

u32 slot_find_min_key(void *unused0, s32 threshold)
{
    s32 best = threshold;
    u32 bestIdx = 0xFFFFFFFF;
    s16 i;
    for (i = 0; i <= 5; i++)
    {
        struct Slot *s = gWorld->slots[i];
        if (s != 0 && (s->flags7E & 6) == 4)
        {
            s32 key = s->unkF6;
            if (key < best)
            {
                best = key;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

u32 slot_find_max_key(void *unused0, s32 threshold)
{
    s32 best = threshold;
    u32 bestIdx = 0xFFFFFFFF;
    s16 i;
    for (i = 0; i <= 5; i++)
    {
        struct Slot *s = gWorld->slots[i];
        if (s != 0 && (s->flags7E & 6) == 4)
        {
            s32 key = s->unkF6;
            if (key > best)
            {
                best = key;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

NONMATCH("asm/code_080F7100/slot_read_pos.s", void slot_read_pos(void *unused0, s16 idx, s16 *outX, s16 *outY, s16 *outZ))
{
    struct SlotObj *obj;
    if (idx <= 7)
        obj = (struct SlotObj *)((u8 *)gWorld->slots[idx] + 8);
    else
        obj = (struct SlotObj *)(gWorkBuf + idx * 0xD0 + 0x300);
    *outX = obj->drawX / 0x100;
    *outY = obj->drawY / 0x100;
    *outZ = obj->drawZ / 0x100;
}
END_NONMATCH

NONMATCH("asm/code_080F7100/slot_set_pos_rel.s", void slot_set_pos_rel(void *unused0, s16 idx, s32 x, s32 y, s32 z))
{
    struct SlotObj *obj;
    u32 addr;
    u32 addr2;
    s32 v;
    if (idx <= 7)
        obj = (struct SlotObj *)((u8 *)gWorld->slots[idx] + 8);
    else
        obj = (struct SlotObj *)(gWorkBuf + idx * 0xD0 + 0x300);
    addr = (u32)obj->ring + 0xD8;
    v = obj->posX + (x << 8);
    obj->drawX = v;
    *(s32 *)addr = v;
    addr2 = addr + 4;
    v = obj->posY + (y << 8);
    obj->drawY = v;
    *(s32 *)addr2 = v;
    addr = addr2 + 4;
    v = obj->posZ + (z << 8);
    obj->drawZ = v;
    *(s32 *)addr = v;
}
END_NONMATCH

NONMATCH("asm/code_080F7100/slot_set_pos_abs.s", void slot_set_pos_abs(void *unused0, s16 idx, s32 x, s32 y, s32 z))
{
    struct SlotObj *obj;
    u32 addr;
    u32 addr2;
    if (idx <= 7)
        obj = (struct SlotObj *)((u8 *)gWorld->slots[idx] + 8);
    else
        obj = (struct SlotObj *)(gWorkBuf + 0x300 + idx * 0xD0);
    addr = (u32)obj->ring + 0xD8;
    obj->drawX = x << 8;
    *(u32 *)addr = x << 8;
    addr2 = addr + 4;
    obj->drawY = y << 8;
    *(u32 *)addr2 = y << 8;
    addr = addr2 + 4;
    obj->drawZ = z << 8;
    *(u32 *)addr = z << 8;
}
END_NONMATCH

ASM_FUNC("asm/code_080F7100/slot_set_flag6.s", void slot_set_flag6(void))

NONMATCH("asm/code_080F7100/slot_set_bias.s", void slot_set_bias(void *unused0, s16 idx, u8 bias))
{
    struct SlotObj *obj;
    if (idx <= 7)
        obj = (struct SlotObj *)((u8 *)gWorld->slots[idx] + 8);
    else
        obj = (struct SlotObj *)(gWorkBuf + 0x300 + idx * 0xD0);
    obj->biasC5 = bias;
}
END_NONMATCH

ASM_FUNC("asm/code_080F7100/slot_pos_offset.s", void slot_pos_offset(void))

u32 ring_find_entry(struct SlotObj *obj, struct CopyNode *node, u32 *key)
{
    s16 count = 0;
    struct RingEntry *entry = obj->ring->entries;
    do
    {
        if (entry->flagF != 0)
        {
            if (entry->keyC == *key || *key == 0x3F)
            {
                node->unk0 = node->unk14;
                return 0;
            }
        }
        count++;
        entry++;
    } while (count <= 3);
    return 1;
}

u32 ring_release_key(struct SlotObj *obj, void *unused1, u32 *key)
{
    s16 count = 0;
    struct RingEntry *entry = obj->ring->entries;
    do
    {
        if (entry->flagF != 0 && entry->keyC == *key)
        {
            u32 off = (u8)count;
            struct RingEntry *hit = obj->ring->entries + off;
            obj_detach_active((void *)hit->unk0);
            hit->flagF = 0;
            return 1;
        }
        count++;
        entry++;
    } while (count <= 3);
    return 1;
}

u32 cmd_mode_switch(void *unused0, void *unused1, u32 *cmd)
{
    switch ((s32)cmd[0])
    {
        case 0:
            display_finish(0, -1);
            break;
        case 1:
            display_finish(1, -1);
            break;
        case 2:
            display_pause();
            break;
    }
    return 1;
}

u32 world_state_copy(void *unused0, struct CopyNode *node)
{
    if (sub_obj_running(gWorld->ptr248) != 0 || world_entry_ready() != 0
        || (gWorld->flags2BF & 0x20) != 0 || gWorld->word2F0 != 0)
    {
        node->unk0 = node->unk14;
        return 0;
    }
    return 1;
}

u32 cmd_sub_start_ext(void *unused0, void *unused1, struct SubStartCmd *cmd)
{
    s32 bit = cmd->mode & 1;
    u32 mask = ((-bit | bit) >> 31) & 4;
    u32 limit = 0;
    if ((s32)cmd->mode > 1)
        limit = 0x7FFF;
    draw_sprite_block(mask, limit, cmd->arg, cmd->unk8, cmd->unkC);
    return 1;
}

u32 cmd_sub_start(void *unused0, void *unused1, struct SubStartCmd *cmd)
{
    s32 bit = cmd->mode & 1;
    u32 mask = ((-bit | bit) >> 31) & 4;
    u32 limit = 0;
    if ((s32)cmd->mode > 1)
        limit = 0x7FFF;
    draw_sprite_small(mask, limit, cmd->arg);
    return 1;
}

u32 owner_state_copy(struct Owner *owner, struct CopyNode *node)
{
    if ((owner->state->state179D & 0x7F) != 0)
    {
        node->unk0 = node->unk14;
        return 0;
    }
    return 1;
}

struct StateBits
{
    u8 lo2 : 2;
    u8 hi6 : 6;
};

u32 owner_state_set(struct Owner *obj)
{
    ((struct StateBits *)&obj->state->state179D)->lo2 = 1;
    return 1;
}

ASM_FUNC("asm/code_080F7100/cmd_slot_place.s", void cmd_slot_place(void))

ASM_FUNC("asm/code_080F7100/cmd_slot_start_table.s", void cmd_slot_start_table(void))

u32 cmd_noop(void)
{
    return 1;
}

u32 busy_state_copy(void *unused0, struct CopyNode *node)
{
    if (is_state_busy())
    {
        node->unk0 = node->unk14;
        return 0;
    }
    return 1;
}

u32 cmd_world_set_byte350(void *unused0, void *unused1, u32 *block)
{
    u16 idx = *block++ - 6;
    if (*block++ == 0)
        gWorld->unk350[idx] = *block;
    return 1;
}

u32 cmd_effect_set(void *unused0, void *unused1, u32 *block)
{
    u32 first = *block++;
    if (first == 0)
    {
        gWorld->effect->unk8 = *block;
        gWorld->effect->unkA = first;
    }
    return 1;
}

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};

NONMATCH("asm/code_080F7100/cmd_obj_flags.s", u32 cmd_obj_flags(void *unused0, struct ObjFlags *obj, u32 *block))
{
    u32 kind = *block++;
    switch (kind)
    {
        case 0:
        {
            u32 idx = *block;
            struct World *w = gWorld;
            record_slot_hit(*(struct Slot **)((idx << 2) + (u8 *)w + 0x58), w);
            break;
        }
        case 1:
            ((struct Lo3Bits *)&obj->bitsFE)->lo3 = *(u8 *)block;
            obj->flagsA0 |= 0x1000;
            return 0;
    }
    return 1;
}
END_NONMATCH

u32 cmd_world_bits318(void *unused0, void *unused1, u32 *block)
{
    if (*block++ == 0)
    {
        u32 shift = (*block++ - 7) << 1;
        u32 v;
        u8 *flags;
        gWorld->bits318 = (gWorld->bits318 & ~(3 << shift)) | ((*block - 1) << shift);
        v = *block - 1;
        flags = &gWorld->flags2BF;
        v <<= 6;
        *flags = (*flags & 0x3F) | v;
    }
    return 1;
}

u32 cmd_slot_offset_row(void *unused0, void *unused1, u32 *p)
{
    apply_offset_row(gWorld->slots[p[0]]->unk10, p[1]);
    return 1;
}

u8 cmd_slot_copy_pos(void *unused0, void *unused1, u32 *block)
{
    struct Slot *dst = gWorld->slots[*block++];
    struct Slot *src = gWorld->slots[*block++];
    dst->unkD8 = src->unkD8;
    dst->unkDC = src->unkDC;
    dst->unkE0 = src->unkE0;
    return *(u8 *)block;
}

u32 cmd_slot_ring_pos(void *unused0, void *unused1, u32 *block)
{
    struct Slot *slot = gWorld->slots[block[1]];
    u32 mode = *block++;
    switch (mode)
    {
        case 0:
            slot->unkD8 = block[1] << 8;
            slot->unkDC = block[2] << 8;
            slot->unkE0 = block[3] << 8;
            break;
        case 1:
            slot->unkD8 += block[1] << 8;
            slot->unkDC += block[3] << 8;
            slot->unkE0 += block[2] << 8;
            break;
    }
    return 1;
}

u32 buf_obj_call_if_state(void *unused0, void *unused1, s32 *p)
{
    s32 idx = *p;
    if (idx > 7)
    {
        u8 *entry = gWorkBuf + (idx * 0xD0 + 0x980);
        struct SlotObj *obj = (struct SlotObj *)(entry - 0x680);
        if ((obj->state76 & 6) == 6)
            obj_release(obj);
    }
    return 1;
}

u32 cmd_call_ca04(void *unused0, void *unused1, u32 *p)
{
    pool_slot_init(p[0], p[1]);
    return 1;
}
