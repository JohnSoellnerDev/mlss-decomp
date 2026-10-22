#include "global.h"

struct FlagBits2
{
    u8 unk0 : 1;
    u8 unk1 : 1;
};

struct LinkModeBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct BitB0
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct LinkStateBits
{
    u8 b0 : 1;
    u8 b1_3 : 3;
    u8 b4_5 : 2;
    u8 rest : 2;
};

struct ListNode
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u32 unk4;
    u8 unk8[4];
    struct ListNode *prev;
    struct ListNode *next;
    void *unk14;
    void *desc;
};

struct RegEntry
{
    u32 state;
    u32 unk4;
    u32 key;
};

struct SysArea
{
    u8 filler0[0x0C];
    struct ListNode *listHead;
    struct ListNode *iter;
    struct ListNode *current;
    u8 taskCount;
    u8 filler19[0x88C - 0x19];
    u8 unk88C;
    u8 rampFlags;
    u8 filler88E[0x890 - 0x88E];
    u16 ramp0Value;
    u8 ramp0Period;
    u8 unk893;
    u16 ramp0Target;
    u16 ramp0Speed;
    u16 songSlot[2];
    u16 slot89C[2];
    u16 slot8A0[2];
    u16 slot8A4[2];
    u8 period8A8[2];
    u8 unk8AA[2];
    u16 target8AC[2];
    u16 speed8B0[2];
    u8 period8B4[2];
    u8 unk8B6[2];
    u16 target8B8[2];
    u16 speed8BC[2];
};

#define SYS_AREA ((struct SysArea *)gSys)

struct LinkState
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 filler5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 filler0C[0x14 - 0x0C];
    u32 unk14;
    u32 unk18;
    u8 *buf1C;
    u8 *buf20;
    u32 unk24;
    u32 unk28;
    u8 *recs2C;
};

extern u8 gSys[];
extern u8 gUnk_03000C28;
extern u32 gUnk_03000C6C;
extern u32 gUnk_03000C78;
extern const u32 gRomBootWords[];
extern struct LinkState *gUnk_03000D0C;
extern u8 *gUnk_03000D10;
extern const u8 decompress_routine[];
extern const u8 gHeapTags[];
extern const u8 gDefaultTaskDesc[];
void sub_0800063C(void);
extern u32 gUnk_03000C84;
extern u32 gUnk_03000C78;
extern u8 gUnk_03000D08;
extern u8 gUnk_03000C80;
extern u8 gUnk_03000D18;
extern u8 gUnk_03000D30[];
void serial_irq_handler(void);
void serial_timeout(void);
void serial_clear_timer(void);
void set_irq_callback(u32 kind, void *cb);
extern struct RegEntry *gUnk_03000C88[];
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void cpu_set(const void *src, void *dst, u32 ctl);
void heap_free_b(void *p);

void queue_stop_all(void);
void queue_song_stop(void);
void queue_song_play(u32 a, u32 b);
void queue_player_start(u32 a, u32 b);
void queue_player_key_off(u32 a);
void queue_player_enable(u32 a);
void queue_player_volume(u32 a, u32 b);
void queue_player_word(u32 a, u32 b);
u32 sub_0819CB90(void);
s32 active_player_mask(u32 i);
u32 sound_tick(u32 t, u32 b);
void sound_buffer_kick(u32 r);
void sound_dma_flip(u32 t, u32 b);
void sound_dma_reset(void);
u32 slot_bit_test(u32 i);
void slot_reset(u32 idx);
void set_slot_fields(u32 idx, u8 level, u8 period);
void set_ramp0_target(u8 level, u8 period);
u8 move_complete_records(u8 *base, u32 unused, u32 arg2, u32 arg3);
struct ListNode *task_insert(struct ListNode *task, u8 prio, u8 *data);

void set_slot_target(u32 idx, u32 value, u32 mode)
{
    if (value == 0)
    {
        slot_reset(idx);
    }
    else
    {
        if ((u8)slot_bit_test(idx) == 0 || SYS_AREA->songSlot[idx] != value)
        {
            queue_player_start(idx + 1, value);
            if (mode == 0xFFFFFFFF)
            {
                u16 stored = SYS_AREA->slot8A0[idx];
                if (stored == 0)
                    set_slot_fields(idx, 0xFF, 0);
                else
                    SYS_AREA->slot89C[idx] = stored;
            }
            else
            {
                set_slot_fields(idx, (u8)mode, 0);
            }
            SYS_AREA->slot8A4[idx] = 0x4B00;
        }
    }
    SYS_AREA->songSlot[idx] = value;
}

NONMATCH("asm/code_0801931C/set_slot_fields.s", void set_slot_fields(u32 idx, u8 level, u8 period))
{
    if (period == 0)
    {
        SYS_AREA->slot89C[idx] = SYS_AREA->slot8A0[idx] = level << 8;
        queue_player_volume(idx + 1, level);
    }
    else
    {
        u16 target = level << 8;
        SYS_AREA->target8AC[idx] = target;
        SYS_AREA->period8A8[idx] = period;
        SYS_AREA->unk8AA[idx] = 0;
        SYS_AREA->speed8B0[idx] = (target - SYS_AREA->slot89C[idx]) / period;
        {
            u8 fb;
            u8 *fp;
            u32 bit;
            if (idx == 0)
            {
                fp = &SYS_AREA->rampFlags;
                fb = *fp;
                bit = 0x08;
            }
            else
            {
                fp = &SYS_AREA->rampFlags;
                fb = *fp;
                bit = 0x10;
            }
            *fp = fb | bit;
        }
    }
}
END_NONMATCH

NONMATCH("asm/code_0801931C/set_slot_fields_45.s", void set_slot_fields_45(u32 idx, u8 level, u8 period))
{
    if (period == 0)
    {
        SYS_AREA->slot8A4[idx] = level << 8;
        queue_player_word(idx + 1, level);
    }
    else
    {
        u16 target = level << 8;
        SYS_AREA->target8B8[idx] = target;
        SYS_AREA->period8B4[idx] = period;
        SYS_AREA->unk8B6[idx] = 0;
        SYS_AREA->speed8BC[idx] = (target - (SYS_AREA->slot8A4[idx] << 8)) / period;
        {
            u8 fb;
            u8 *fp;
            u32 bit;
            if (idx == 0)
            {
                fp = &SYS_AREA->rampFlags;
                fb = *fp;
                bit = 0x20;
            }
            else
            {
                fp = &SYS_AREA->rampFlags;
                fb = *fp;
                bit = 0x40;
            }
            *fp = fb | bit;
        }
    }
}
END_NONMATCH

void set_ramp0_target(u8 level, u8 period)
{
    u8 p = period;
    if (p == 0)
    {
        SYS_AREA->ramp0Value = level << 8;
        queue_player_volume(0, level);
    }
    else
    {
        u16 target = level << 8;
        SYS_AREA->ramp0Target = target;
        SYS_AREA->ramp0Period = p;
        SYS_AREA->unk893 = 0;
        {
            int b = SYS_AREA->rampFlags;
            b |= 4;
            SYS_AREA->rampFlags = b;
        }
        SYS_AREA->ramp0Speed = (target - (SYS_AREA->ramp0Value << 8)) / p;
    }
}

u32 sound_is_playing(void)
{
    s32 found = sub_0819CB90();
    return ((u32)(-found) | (u32)found) >> 31;
}

void sound_stop_all(void)
{
    queue_stop_all();
}

void sound_stop_song(void)
{
    queue_song_stop();
}

void set_slot_value(u32 a, u32 v)
{
    queue_song_play(a, v);
    if (v == 0xFFFFFFFF && SYS_AREA->ramp0Value != 0)
        return;
    if (SYS_AREA->ramp0Value == 0)
        v = 0xFF;
    set_ramp0_target((u8)v, 0);
}

u32 slot_bit_test(u32 i)
{
    s32 mask = active_player_mask(i);
    return (mask >> (i + 1)) & 1;
}

u16 sys_table_898(u32 i)
{
    return SYS_AREA->songSlot[i];
}

void slot_reset(u32 idx)
{
    queue_player_key_off(idx + 1);
    SYS_AREA->slot89C[idx] = 0;
}

void slot_set_mode(u32 i, u32 v, u8 flag)
{
    queue_player_enable(i + 1);
    if (v == 0xFFFFFFFF)
    {
        v = SYS_AREA->slot8A0[i] >> 8;
        if (v == 0)
            v = 0xFF;
    }
    set_slot_fields(i, (u8)v, flag);
}

void flag_88c_handler_a(void)
{
    u32 b = SYS_AREA->unk88C;
    u32 t = 1 & b;
    if (t != 0)
    {
        u32 r = sound_tick(t, b);
        sound_buffer_kick(r);
    }
}

void flag_88c_handler_b(void)
{
    u32 b = SYS_AREA->unk88C;
    u32 t = 1 & b;
    if (t != 0)
    {
        sound_dma_flip(t, b);
    }
}

// The original has four no-op words after each DMA control write (not reproducible from C).
NONMATCH("asm/code_0801931C/sound_dma_reset.s", void sound_dma_reset(void))
{
    REG_TM0CNT_H &= 0xFF7F;
    REG_SOUNDCNT_H = 0x88;
    REG_DMA1CNT = (REG_DMA1CNT & 0x41800000) | 0x84400004;
    REG_DMA1CNT &= 0x7FFFFFFF;
    REG_DMA2CNT = (REG_DMA2CNT & 0x41800000) | 0x84400004;
    REG_DMA2CNT &= 0x7FFFFFFF;
}
END_NONMATCH

void sound_stop_all_reset(void)
{
    u8 *base;

    queue_player_key_off(0);
    queue_player_key_off(1);
    queue_stop_all();
    sound_dma_reset();
    base = gSys;
    ((struct FlagBits2 *)(base + 0x88C))->unk0 = 0;
}

s16 recip_fixed16(s16 x)
{
    s32 n = 0x10000;
    return n / x;
}

s16 fixed_div_8_8(s16 a, s16 b)
{
    return ((s32)a << 8) / b;
}

s16 fixed_mul_8_8(s16 a, s16 b)
{
    s32 x = a * b;
    x /= 256;
    return x;
}

u32 decimal_digits(s32 x)
{
    u32 n = 0;
    s32 p = 1;
    do
    {
        n++;
        p = p * 10;
    } while (p <= x);
    return n;
}

void reset_state_0c28(void)
{
    u32 *dst;
    const u8 *romWord;
    gUnk_03000C28 = 0;
    dst = &gUnk_03000C6C;
    romWord = (const u8 *)gRomBootWords + 4;
    *dst = *(const u32 *)romWord;
    gUnk_03000C78 = 0;
}

ASM_FUNC("asm/code_0801931C/table_register.s", u32 table_register(struct RegEntry *p))

// The original loop was unrolled by the compiler; the remainder entry differs slightly.
NONMATCH("asm/code_0801931C/read_varlen_plus1.s", u32 read_varlen_plus1(u8 *p))
{
    u32 v = p[0];
    u32 n = v >> 6;
    s32 shift;
    u32 count;

    v &= 0x3F;

    if (n != 0)
    {
        shift = 6;
        count = n;
        do
        {
            p++;
            v += *p << shift;
            shift += 8;
        } while (--count != 0);
    }
    v += 1;
    return v;
}
END_NONMATCH

// The original loads the table end a second time from its own literal pool entry; GCC shares one.
NONMATCH("asm/code_0801931C/load_rom_table.s", void load_rom_table(void))
{
    s32 len = (u32)sub_0800063C;
    u8 *buf;
    len -= (u32)decompress_routine;
    buf = heap_alloc(len, 0, gHeapTags + 0x48, 0);
    gUnk_03000C84 = (u32)buf;
    if (len < 0)
        len += 3;
    cpu_set(decompress_routine, buf, ((((u32)len) << 9) >> 11) | 0x04000000);
    gUnk_03000C78 = (u32)sub_0800063C;
    gUnk_03000D08 = 0;
    gUnk_03000C80 = 0;
}
END_NONMATCH

ASM_FUNC("asm/code_0801931C/task_insert.s", struct ListNode *task_insert(struct ListNode *task, u8 prio, u8 *data))

ASM_FUNC("asm/code_0801931C/run_tasks.s", void run_tasks(void))

u32 task_is_linked(struct ListNode *task)
{
    struct ListNode *t = SYS_AREA->listHead;
    if (t != 0)
    {
        do
        {
            if (t == task)
                return 1;
            t = t->next;
        } while (t != 0);
    }
    return 0;
}

void obj_list_set_flags_3(void)
{
    struct ListNode *node = SYS_AREA->listHead;
    if (node != 0)
    {
        do
        {
            int b = node->unk2;
            b |= 1;
            b |= 2;
            node->unk2 = b;
            node = node->next;
        } while (node != 0);
    }
}

void obj_list_update_flags(void)
{
    struct ListNode *node = SYS_AREA->listHead;

    for (; node != 0; node = node->next)
    {
        if (node->unk1 != 0 && node->unk1 != 0xFF)
        {
            ((struct FlagBits2 *)&node->unk2)->unk0 = 0;
            ((struct FlagBits2 *)&node->unk2)->unk1 = 1;
        }
    }
}

void set_flags_2_3(u8 *p)
{
    int b = p[2];
    b |= 1;
    b |= 2;
    p[2] = b;
}

void set_flag_2_2(u8 *p)
{
    ((struct FlagBits2 *)(p + 2))->unk0 = 0;
    ((struct FlagBits2 *)(p + 2))->unk1 = 1;
}

void task_remove(struct ListNode *task, u32 flags)
{
    task->desc = (void *)gDefaultTaskDesc;
    if (task->next != 0)
        task->next->prev = task->prev;
    if (task->prev == 0)
        SYS_AREA->listHead = task->next;
    else
        task->prev->next = task->next;
    SYS_AREA->taskCount--;
    if (SYS_AREA->current == task)
        SYS_AREA->current = 0;
    if (SYS_AREA->iter == task)
        SYS_AREA->iter = task->next;
    if ((flags & 1) != 0)
        heap_free_b(task);
}

ASM_FUNC("asm/code_0801931C/serial_link_init.s", void serial_link_init(void))

void heap_free(void *p);
void serial_link_shutdown(void)
{
    REG_IME = 0;
    REG_IE = REG_IE & 0xFFBF;
    REG_IF = REG_IF | 0x40;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE = REG_IE & 0xFF7F;
    REG_IF = REG_IF | 0x80;
    REG_IME = 1;
    REG_IME = 0;
    REG_SIOCNT = 0x2003;
    REG_TM3CNT = 0xABFB;
    REG_IME = 1;
    heap_free(gUnk_03000D10);
    heap_free(*(void **)((u8 *)gUnk_03000D0C + 0x180));
    heap_free(gUnk_03000D0C);
    REG_IME = 0;
    gUnk_03000D0C = 0;
    gUnk_03000D10 = 0;
    gSys[0x888] = gSys[0x888] & 0x7F;
    REG_IME = 1;
}

ASM_FUNC("asm/code_0801931C/serial_frame.s", void serial_frame(void))

NONMATCH("asm/code_0801931C/serial_packet_checksum_write.s", void serial_packet_checksum_write(void))
{
    u16 *hw;
    u32 sum;
    s32 i;
    u16 zero;

    gUnk_03000D0C->buf1C[0] = gUnk_03000D0C->unkB;
    zero = 0;
    gUnk_03000D0C->buf1C[1] = gUnk_03000D0C->unk2 ^ gUnk_03000D0C->unk3;
    ((u16 *)gUnk_03000D0C->buf1C)[1] = zero;
    cpu_set(gUnk_03000D10 + 2, gUnk_03000D0C->buf1C + 4, 0x04000004);
    hw = (u16 *)gUnk_03000D0C->buf1C;
    sum = 0;
    for (i = 0; i < 10; i++)
        sum += hw[i];
    hw[1] = ~sum - 12;
    gUnk_03000D0C->unk4 = 1;
}
END_NONMATCH

ASM_FUNC("asm/code_0801931C/move_complete_records.s", u8 move_complete_records(u8 *base, u32 unused, u32 arg2, u32 arg3))

void serial_link_update(void)
{
    struct LinkState *s = gUnk_03000D0C;
    if (s != 0)
    {
        u32 b0 = s->unk0;
        if (b0 != 0)
        {
            if (s->unk1 != 0 && s->unk6 != 0)
            {
                u32 t;
                u8 *p;
                vu16 *sio;
                u32 fefe;
                s->unk18 = 0xFFFFFFFE;
                t = s->unk28;
                s->unk28 = s->unk24;
                s->unk24 = t;
                if (s->unk4 != 0)
                {
                    p = s->buf20;
                    s->buf20 = s->buf1C;
                    s->buf1C = p;
                    s->unk4 = 0;
                    gUnk_03000D0C->unk14 = 0;
                }
                gUnk_03000D0C->unk7 = ((*(vu32 *)REG_ADDR_SIOCNT) << 25) >> 31;
                sio = (vu16 *)REG_ADDR_SIOCNT;
                fefe = 0xFEFE;
                sio[1] = fefe;
                sio[0] = sio[0] | 0x80;
                REG_TM3CNT_H = 0xC0;
            }
        }
        else
        {
            if (s->unk9 == 0)
            {
                REG_IME = b0;
                *(vu16 *)0x03007FF8 = *(vu16 *)0x03007FF8 | 0x80;
                REG_IME = 1;
            }
            gUnk_03000D0C->unk9 = b0;
        }
    }
}

void serial_flag6_if_active(void)
{
    if (gUnk_03000D0C->unk0)
    {
        gUnk_03000D0C->unk6 = 1;
    }
}

extern const u8 gObjHandler[];
extern const u8 gObjInitBgPalette[];
extern const u8 gObjInitBgTiles[];
extern const u8 gScreenMapObjInit[];
extern u8 gUnk_02000080[];
extern u16 gUnk_02000050[];
extern u16 gUnk_02000054;
void cpu_fast_set(const void *src, void *dst, u32 control);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void frame_update(void);

struct GfxObj
{
    u8 filler0[0x18];
    void *handler;
    u8 unk1C;
    u8 filler1D;
    u16 unk1E;
    u16 unk20;
    u32 unk24;
    u32 unk28;
    void *data2C;
};

NONMATCH("asm/code_0801931C/obj_init_gfx.s", struct ListNode *obj_init_gfx(struct ListNode *obj, u8 kind, u8 *taskData, void *data))
{
    u8 *base;
    struct GfxObj *g;
    u16 *shadow;
    u32 flags;

    task_insert(obj, kind, taskData);
    g = (struct GfxObj *)obj;
    g->handler = (void *)gObjHandler;
    base = gSys;
    flags = base[0x88A];
    flags &= -9;
    base[0x88A] = flags;
    g->unk1C = 0x10;
    g->data2C = data;
    g->unk1E = 0;
    g->unk20 = 0;
    g->unk24 = 0;
    g->unk28 = 0;
    cpu_fast_set(gObjInitBgPalette, gUnk_02000080, 0x80);
    dma_copy_or_queue((void *)gObjInitBgTiles, (void *)0x06008000, 0x4000, 0x20, 0);
    dma_copy_or_queue((void *)gScreenMapObjInit, (void *)0x06000000, 0x500, 0x20, 0);
    *(u32 *)(base + 0x884) |= 0xFFFF;
    {
        int hw = *(u16 *)(base + 2);
        hw |= 0xFFFF;
        *(u16 *)(base + 2) = hw;
    }
    frame_update();
    shadow = gUnk_02000050;
    shadow[0] = 0xBF;
    gUnk_02000054 = 0x10;
    shadow = (u16 *)((u8 *)shadow - 0x48);
    *shadow = 0x88;
    shadow = (u16 *)((u8 *)shadow - 8);
    *shadow = 0x140;
    return obj;
}
END_NONMATCH

void task_remove(struct ListNode *task, u32 flags);
struct AnimObj
{
    u8 state;
    u8 filler1[3];
    u32 unk4;
    u8 filler8[0x18 - 0x8];
    void *handler;
    u8 timer;
    u8 filler1D;
    u16 keys;
    u16 prevKeys;
    u8 filler22[2];
    u32 unk24;
    u32 unk28;
    void (*fn)(void);
};

NONMATCH("asm/code_0801931C/anim_state_step.s", void anim_state_step(struct AnimObj *p))
{
    u32 mask;
    u16 old;
    u16 keys;
    u32 start;
    u32 cnt;

    switch (p->state)
    {
    case 0:
        p->timer--;
        *(vu16 *)&gUnk_02000054 = (s8)p->timer;
        if ((s8)p->timer == 0)
        {
            p->timer = 0x32;
            p->state++;
        }
        break;
    case 1:
        p->timer--;
        if (p->timer == 0)
        {
            p->timer = 0x10;
            p->state++;
        }
        break;
    case 2:
        p->timer--;
        *(vu16 *)&gUnk_02000054 = 0x10 - (s8)p->timer;
        if ((s8)p->timer == 0)
        {
            if (p != 0)
            {
                u8 *sys;
                p->handler = (void *)gObjHandler;
                sys = gSys;
                sys[0x88A] |= 8;
                *(vu16 *)0x02000000 = 0;
                *(vu16 *)0x02000050 = 0;
                task_remove((struct ListNode *)p, 3);
            }
            if (p->fn != 0)
                p->fn();
            return;
        }
        break;
    }

    old = p->keys;
    p->prevKeys = old;
    keys = ~REG_KEYINPUT & 0xF0;
    p->keys = keys;
    mask = 0xF0;
    if ((keys & ~old & mask) == mask)
    {
        start = p->unk24;
        p->unk28 = start;
        cnt = p->unk4;
        p->unk24 = cnt;
        if (start != 0 && cnt - start <= 3)
        {
            u8 *sys = gSys;
            ((struct LinkModeBits *)&sys[0x888])->mode = 1;
            ((struct LinkStateBits *)&gUnk_03000D18)->b1_3 = 0;
        }
    }
}
END_NONMATCH

void serial_link_enable(void)
{
    u8 *base = gSys;
    u32 zero;

    if (((struct LinkModeBits *)(base + 0x888))->mode == 1)
    {
        serial_clear_timer();
        ((struct LinkStateBits *)&gUnk_03000D18)->b4_5 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->rest = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b0 = 1;
        REG_IME = 0;
        REG_IE = REG_IE & 0xFFBF;
        REG_IF = REG_IF | 0x40;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE = REG_IE & 0xFF7F;
        REG_IF = REG_IF | 0x80;
        REG_IME = 1;
        REG_RCNT = 0;
        REG_SIOCNT = 0x1008;
        REG_SIOCNT = REG_SIOCNT | 0x4000;
        set_irq_callback(1, serial_irq_handler);
        set_irq_callback(0, serial_timeout);
        REG_IME = 0;
        REG_IE = REG_IE | 0x80;
        REG_IF = REG_IF | 0x80;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE = REG_IE | 0x40;
        REG_IF = REG_IF | 0x40;
        REG_IME = 1;
        ((struct BitB0 *)REG_ADDR_SIOCNT)->b0 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b1_3 = 1;
        zero = 0;
        cpu_set(&zero, gUnk_03000D30, 0x05000003);
        REG_IME = 0;
        REG_SIOCNT = REG_SIOCNT | 0x80;
        REG_IME = 1;
        REG_TM3CNT_L = 0x8000;
        REG_TM3CNT_H = 0xC1;
    }
}

void serial_link_disable(void)
{
    u8 *base = gSys;

    if (((struct LinkModeBits *)(base + 0x888))->mode == 1)
    {
        ((struct LinkStateBits *)&gUnk_03000D18)->b0 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b1_3 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b4_5 = 0;
        REG_IME = 0;
        REG_IE = REG_IE & 0xFFBF;
        REG_IF = REG_IF | 0x40;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE = REG_IE & 0xFF7F;
        REG_IF = REG_IF | 0x80;
        REG_IME = 1;
        REG_IME = 0;
        REG_SIOCNT = 0x2000;
        REG_TM3CNT = 0x8000;
        REG_IME = 1;
    }
}

ASM_FUNC("asm/code_0801931C/serial_irq_handler.s", void serial_irq_handler(void))

u8 serial_rx_error(void);
u32 serial_check_packet(u8 kind)
{
    u32 *regs = (u32 *)&gUnk_03000D18;
    u32 type = regs[2] >> 28;

    if (serial_rx_error() != 0)
        return 1;

    switch (kind)
    {
    case 2:
        regs[3] = ((regs[2] & 0xFFFFFFF) >> 4) & 1;
        if (type == 1)
            goto ok;
        return 1;
    case 3:
        if (type == 2 && regs[3] == ((regs[2] & 0xFFFFFFF) >> 4))
            goto ok;
        return 1;
    case 4:
        if (type == 3)
            goto ok;
        return 1;
    }
    return 1;

ok:
    return 0;
}

struct LinkRam
{
    u8 phase;
    u8 filler1[0xB];
    u32 rx2;
};

u32 serial_slot_bits(u32 index, u32 unused1, u32 bits);

static inline u32 fold_nibbles(u32 acc, u32 word)
{
    u8 i;
    for (i = 6; i != 0; i--)
        acc ^= (word >> (i << 2)) & 0xF;
    return acc;
}

NONMATCH("asm/code_0801931C/serial_make_packet.s", u32 serial_make_packet(u8 kind, u32 unused1, u32 unused2, u32 packet))
{
    u32 word;
    u8 phase;
    u32 k = kind - 2;

    switch (k)
    {
    case 0:
        word = 0x10000010;
        packet = (fold_nibbles(1, word) & 0xF) | word;
        break;
    case 1:
        word = ((((struct LinkRam *)&gUnk_03000D18)->rx2 & 0xFFFFFF) << 4) | 0x20000000;
        packet = (fold_nibbles(word >> 28, word) & 0xF) | word;
        break;
    case 2:
        phase = ((struct LinkRam *)&gUnk_03000D18)->phase;
        if ((phase & 0x30) == 0)
        {
            word = ((serial_slot_bits(0, phase, (u32)&gUnk_03000D18) & 0xFFFFFF) << 4) | 0x40000000;
            packet = (fold_nibbles(word >> 28, word) & 0xF) | word;
        }
        phase = ((struct LinkRam *)&gUnk_03000D18)->phase;
        if ((phase & 0x30) == 0x10)
        {
            word = ((serial_slot_bits(1, phase, (u32)&gUnk_03000D18) & 0xFFFFFF) << 4) | 0x40000000;
            packet = (fold_nibbles(word >> 28, word) & 0xF) | word;
        }
        phase = ((struct LinkRam *)&gUnk_03000D18)->phase;
        if ((phase & 0x30) == 0x20)
        {
            word = ((serial_slot_bits(2, phase, (u32)&gUnk_03000D18) & 0xFFFFFF) << 4) | 0x40000000;
            packet = (fold_nibbles(word >> 28, word) & 0xF) | word;
        }
        break;
    case 3:
    case 4:
        word = 0x10000010;
        packet = (fold_nibbles(1, word) & 0xF) | word;
        break;
    }
    return packet;
}
END_NONMATCH
