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
    u8 filler0;
    u8 unk1;
    u8 unk2;
    u8 filler3[0x0C - 3];
    struct ListNode *prev;
    struct ListNode *next;
    u8 filler14[0x18 - 0x14];
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

#define SYS_AREA ((struct SysArea *)gUnk_0300034C)

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
};

extern u8 gUnk_0300034C[];
extern u8 gUnk_03000C28;
extern u32 gUnk_03000C6C;
extern u32 gUnk_03000C78;
extern const u32 gRomBootWords[];
extern struct LinkState *gUnk_03000D0C;
extern u8 *gUnk_03000D10;
extern const u8 decompress_routine[];
extern const u8 gHeapTags[];
extern const u8 gDefaultTaskDesc[];
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

ASM_FUNC("asm/code_0801931C/set_slot_fields.s", void set_slot_fields(u32 idx, u8 level, u8 period))

ASM_FUNC("asm/code_0801931C/set_slot_fields_45.s", void set_slot_fields_45(void))

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

ASM_FUNC("asm/code_0801931C/sound_dma_reset.s", void sound_dma_reset(void))

void sound_stop_all_reset(void)
{
    u8 *base;

    queue_player_key_off(0);
    queue_player_key_off(1);
    queue_stop_all();
    sound_dma_reset();
    base = gUnk_0300034C;
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

ASM_FUNC("asm/code_0801931C/fixed_mul_8_8.s", void fixed_mul_8_8(void))

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

ASM_FUNC("asm/code_0801931C/table_register.s", void table_register(void))

ASM_FUNC("asm/code_0801931C/read_varlen_plus1.s", void read_varlen_plus1(void))

ASM_FUNC("asm/code_0801931C/load_rom_table.s", void load_rom_table(void))

ASM_FUNC("asm/code_0801931C/task_insert.s", void task_insert(void))

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

ASM_FUNC("asm/code_0801931C/obj_list_update_flags.s", void obj_list_update_flags(void))

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

ASM_FUNC("asm/code_0801931C/serial_link_shutdown.s", void serial_link_shutdown(void))

ASM_FUNC("asm/code_0801931C/serial_frame.s", void serial_frame(void))

ASM_FUNC("asm/code_0801931C/serial_packet_checksum_write.s", void serial_packet_checksum_write(void))

ASM_FUNC("asm/code_0801931C/move_complete_records.s", void move_complete_records(void))

ASM_FUNC("asm/code_0801931C/serial_link_update.s", void serial_link_update(void))

void serial_flag6_if_active(void)
{
    if (gUnk_03000D0C->unk0)
    {
        gUnk_03000D0C->unk6 = 1;
    }
}

ASM_FUNC("asm/code_0801931C/obj_init_gfx.s", void obj_init_gfx(void))

ASM_FUNC("asm/code_0801931C/anim_state_step.s", void anim_state_step(void))

void serial_link_enable(void)
{
    u8 *base = gUnk_0300034C;
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
    u8 *base = gUnk_0300034C;

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

ASM_FUNC("asm/code_0801931C/serial_check_packet.s", void serial_check_packet(void))

ASM_FUNC("asm/code_0801931C/serial_make_packet.s", void serial_make_packet(void))
