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

struct TaskData
{
    u8 b[4];
};

/* A task's header (the start of its ListNode) as task_insert fills it in */
struct TaskHeader
{
    u8 unk0;
    u8 prio;               /* the list is kept sorted by it, lowest first */
    u16 active : 1;        /* run_tasks only runs active tasks (cleared to pause one) */
    u16 ranThisFrame : 1;  /* set once the task has run this frame */
    u16 unk2_2 : 4;
    u16 unk2_6 : 4;
    u16 depth : 6;         /* the task's position in the list when it was inserted */
    u32 unk4;
    struct TaskData data;
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

/* A decompression job, as handed to table_register (embedded in e.g. struct DispEntry). */
struct UnpackJob
{
    u32 state;      /* UNPACK_* */
    const void *src; /* packed data */
    void *dst;      /* where it unpacks to; also the job's key in the queue */
};

/* UnpackJob.state */
#define UNPACK_DONE    0 /* unpacked */
#define UNPACK_PENDING 1 /* to submit */
#define UNPACK_QUEUED  2 /* waiting in gUnk_03000C88 */

/* Size of the decompression queue gUnk_03000C88 (a ring) */
#define UNPACK_QUEUE_SIZE 32

struct SysArea
{
    u8 filler0[0x0C];
    struct ListNode *listHead;
    struct ListNode *iter;
    struct ListNode *current;
    u8 taskCount;
    u8 filler19[0x88C - 0x19];
    u8 unk88C;
    u8 rampFlags;               /* RAMP_*: the fades in progress */
    u8 filler88E[0x890 - 0x88E];
    /* Levels are 8.8 fixed point: the high byte goes to the sound driver. */
    u16 ramp0Value;             /* player 0 (song) volume */
    u8 ramp0Period;
    u8 unk893;
    u16 ramp0Target;
    u16 ramp0Speed;
    u16 songSlot[2];            /* song playing on players 1 and 2 */
    u16 slot89C[2];             /* volume of players 1 and 2 */
    u16 slot8A0[2];             /* volume to restore when the slot restarts */
    u16 slot8A4[2];             /* word of players 1 and 2 (sound command 8), default 0x4B00 */
    u8 period8A8[2];            /* volume fade: frames it takes */
    u8 unk8AA[2];               /* volume fade: frames done */
    u16 target8AC[2];           /* volume fade: target */
    u16 speed8B0[2];            /* volume fade: step added each frame (signed) */
    u8 period8B4[2];            /* word fade: the same four */
    u8 unk8B6[2];
    u16 target8B8[2];
    u16 speed8BC[2];
};

/* SysArea.rampFlags bits, stepped once a frame by update_meter_ramps */
#define RAMP_SONG_VOL  0x04
#define RAMP_SLOT0_VOL 0x08
#define RAMP_SLOT1_VOL 0x10
#define RAMP_SLOT0_WORD 0x20
#define RAMP_SLOT1_WORD 0x40

#define SYS_AREA ((struct SysArea *)gSys)

/* The multiplayer serial link (gUnk_03000D0C), set up by serial_link_init. */
struct LinkState
{
    u8 unk0;               /* SIO_MULTI_SD (8) on the parent GBA, 0 on a child */
    u8 unk1;               /* phase (serial_frame): 0 handshake, 1 settling (unk8 counts up to 8), 2 linked */
    u8 unk2;
    u8 unk3;
    u8 unk4;               /* buf1C holds a finished packet (serial_packet_checksum_write) */
    u8 filler5;
    u8 unk6;               /* parent: a transfer was requested (serial_flag6_if_active) */
    u8 unk7;               /* SIOCNT's error bit when the last transfer started */
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;               /* packet counter, written as each packet's first byte */
    u8 filler0C[0x14 - 0x0C];
    u32 unk14;             /* position in buf20, reset when the packets swap */
    u32 unk18;
    u8 *buf1C;             /* packet being built */
    u8 *buf20;             /* packet being sent */
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
extern const u8 decompress_routine_end[];  /* = sub_0800063C, the end of the routine block */
extern const u8 gHeapTags[];
extern const u8 gDefaultTaskDesc[];
void sub_0800063C(void);
extern u32 gUnk_03000C84;   /* decompress function: the RAM copy of decompress_routine, (src, dst) */
extern u32 gUnk_03000C78;   /* unpack mode: sub_0800063C queues jobs, 0 decompresses them at once */
extern u8 gUnk_03000D08;    /* unpack queue head: the slot the search for a free one starts at */
extern u8 gUnk_03000C80;    /* number of jobs waiting in the unpack queue */
extern u8 gUnk_03000D18;
extern u8 gUnk_03000D30[];
void serial_irq_handler(void);
void serial_timeout(void);
void serial_clear_timer(void);
void set_irq_callback(u32 kind, void *cb);
extern struct UnpackJob *gUnk_03000C88[UNPACK_QUEUE_SIZE]; /* unpack queue: jobs waiting to decompress */
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

/* Plays `song` on music player slot+1 (song 0 stops it), unless the player already plays it. A
 * started song gets volume `volume` (0xFFFFFFFF: the slot's last volume, slot8A0, or full volume
 * 0xFF when there is none yet) and slot8A4 0x4B00. The slot's song is remembered in songSlot. */
void set_slot_target(u32 slot, u32 song, u32 volume)
{
    if (song == 0)
    {
        slot_reset(slot);
    }
    else
    {
        if ((u8)slot_bit_test(slot) == 0 || SYS_AREA->songSlot[slot] != song)
        {
            queue_player_start(slot + 1, song);
            if (volume == 0xFFFFFFFF)
            {
                u16 lastVolume = SYS_AREA->slot8A0[slot];
                if (lastVolume == 0)
                    set_slot_fields(slot, 0xFF, 0);
                else
                    SYS_AREA->slot89C[slot] = lastVolume;
            }
            else
            {
                set_slot_fields(slot, (u8)volume, 0);
            }
            SYS_AREA->slot8A4[slot] = 0x4B00;
        }
    }
    SYS_AREA->songSlot[slot] = song;
}

/* Sets the volume of sound player slot `slot` (player slot + 1) to `level`: at once (also as the
   volume to restore) when `frames` is 0, otherwise as a fade over that many frames that
   update_meter_ramps steps (target, period, count reset, per-frame step and its RAMP_SLOTn_VOL flag). */
void set_slot_fields(u32 slot, u8 level, u8 frames)
{
    register u8 period asm("r6") = frames;
    if (period == 0)
    {
        SYS_AREA->slot89C[slot] = SYS_AREA->slot8A0[slot] = level << 8;
        queue_player_volume(slot + 1, level);
    }
    else
    {
        u16 target = level << 8;
        register u32 offset asm("r4");
        register u8 zero asm("r3");
        register u32 sys asm("r5") = (u32)gSys;
        u32 index = slot * 2;
        u32 base = sys + 0x8AC;
        u16 *fadeTarget = (u16 *)(index + base); /* &SYS_AREA->target8AC[slot] */
        zero = 0;
        *fadeTarget = target;
        offset = 0x8A8;
        base = sys + offset;
        *(u8 *)(slot + base) = period;  /* SYS_AREA->period8A8[slot] */
        offset += 2;
        base = sys + offset;
        *(u8 *)(slot + base) = zero;    /* SYS_AREA->unk8AA[slot], the frames done */
        SYS_AREA->speed8B0[slot] = (target - SYS_AREA->slot89C[slot]) / period;
        if (slot == 0)
        {
            u8 flags = SYS_AREA->rampFlags;
            SYS_AREA->rampFlags = flags | RAMP_SLOT0_VOL;
        }
        else
        {
            u8 flags = SYS_AREA->rampFlags;
            SYS_AREA->rampFlags = flags | RAMP_SLOT1_VOL;
        }
    }
}

/* Sets the word of sound player slot `slot` (player slot + 1, sound command 8) to `level`: at once
   when `period` is 0, otherwise as a fade over that many frames that update_meter_ramps steps
   (target, period, count reset, per-frame step and its RAMP_SLOTn_WORD flag), like set_slot_fields
   does for the volume. */
void set_slot_fields_45(u32 slot, u8 level, u8 period)
{
    if (period == 0)
    {
        SYS_AREA->slot8A4[slot] = level << 8;
        queue_player_word(slot + 1, level);
    }
    else
    {
        u16 target = level << 8;
        register u8 zero asm("r3");
        register u32 sys asm("r5") = (u32)gSys;
        u32 index = slot * 2;
        u32 base = sys + 0x8B8;
        u16 *fadeTarget = (u16 *)(index + base); /* &SYS_AREA->target8B8[slot] */
        zero = 0;
        *fadeTarget = target;
        base = sys + 0x8B4;
        *(u8 *)(slot + base) = period;  /* SYS_AREA->period8B4[slot] */
        base = sys + 0x8B6;
        *(u8 *)(slot + base) = zero;    /* SYS_AREA->unk8B6[slot], the frames done */
        /* the current word is shifted up once more here, as for player 0's volume */
        SYS_AREA->speed8BC[slot] = (target - (SYS_AREA->slot8A4[slot] << 8)) / period;
        if (slot == 0)
        {
            u8 flags = SYS_AREA->rampFlags;
            SYS_AREA->rampFlags = flags | RAMP_SLOT0_WORD;
        }
        else
        {
            u8 flags = SYS_AREA->rampFlags;
            SYS_AREA->rampFlags = flags | RAMP_SLOT1_WORD;
        }
    }
}

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

/* Submits a decompression job. When jobs are queued (gUnk_03000C78), a job not queued yet goes into
   the first free slot of the unpack queue from its head on and 1 is returned; a job for the same dst
   already waiting, or a full queue, leaves it out. Otherwise the job is decompressed at once. */
u32 table_register(struct UnpackJob *job)
{
    if (gUnk_03000C78 == (u32)sub_0800063C)
    {
        if (job->state <= UNPACK_PENDING)
        {
            register s32 slot asm("r2") = gUnk_03000D08;
            s32 tries = 0;
            register struct UnpackJob **queue asm("r1") = gUnk_03000C88;
            do
            {
                struct UnpackJob *queued = queue[slot];
                if (queued == NULL)
                    goto found;
                if (queued->dst == job->dst)
                    break; /* already waiting */
                if (queue[slot] == NULL)
                {
                found:
                    {
                        register u32 addr asm("r0") = slot * sizeof(*queue);
                        addr += (u32)queue;
                        *(struct UnpackJob **)addr = job;
                    }
                    job->state = UNPACK_QUEUED;
                    gUnk_03000C80++;
                    return 1;
                }
                {
                    /* slot = (slot + 1) % UNPACK_QUEUE_SIZE */
                    register s32 next asm("r0") = ++slot;
                    if (slot < 0)
                        next += UNPACK_QUEUE_SIZE - 1;
                    slot -= (next >> 5) * UNPACK_QUEUE_SIZE;
                }
                tries++;
            } while (tries < UNPACK_QUEUE_SIZE);
        }
    }
    else
    {
        ((void (*)(const void *, void *))gUnk_03000C84)(job->src, job->dst);
        job->state = UNPACK_DONE;
    }
    return 0;
}

// The original loop was unrolled by the compiler; the remainder entry differs slightly.
u32 read_varlen_plus1(u8 *p)
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
      n = *(++p);
      v += n << shift;
      shift += 8;
    }
    while ((--count) != 0);
  }
  v += 1;
  return v;
}

// The original loads the table end a second time from its own literal pool entry; GCC shares one.
void load_rom_table(void)
{
    s32 len = (u32)decompress_routine_end;
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

/* Links `task` into the system's task list, which is kept sorted by priority (lowest first; a task
   goes after the ones of its own priority), and sets up its header: priority, active and
   ran-this-frame set, its position in the list, the task running now as its parent (unk14), and
   the 4 bytes of `data`. Returns the task. */
struct ListNode *task_insert(struct ListNode *task, u8 prio, u8 *data)
{
    struct ListNode *node;
    u32 position;
    s32 j;

    task->desc = (void *)gDefaultTaskDesc;
    position = 0;
    node = SYS_AREA->listHead;
    if (node == 0)
    {
        /* the list is empty */
        SYS_AREA->listHead = task;
        task->prev = 0;
        task->next = 0;
    }
    else
    {
        for (;;)
        {
            /* unk1 is the node's priority: insert before the first node with a higher one */
            if (node->unk1 > prio)
            {
                if (node->prev == 0)
                {
                    SYS_AREA->listHead = task;
                    task->prev = 0;
                }
                else
                {
                    node->prev->next = task;
                    task->prev = node->prev;
                }
                task->next = node;
                node->prev = task;
                break;
            }
            if (node->next == 0)
            {
                /* append at the end */
                node->next = task;
                task->prev = node;
                task->next = 0;
                position++;
                goto done;
            }
            node = node->next;
            position++;
        }
    }
done:
    task->unk14 = SYS_AREA->current;
    ((struct TaskHeader *)task)->depth = position;
    ((struct TaskHeader *)task)->unk0 = 0;
    ((struct TaskHeader *)task)->prio = prio;
    ((struct TaskHeader *)task)->active = 1;
    ((struct TaskHeader *)task)->ranThisFrame = 1;
    ((struct TaskHeader *)task)->unk2_2 = 1;
    ((struct TaskHeader *)task)->unk2_6 = 0;
    ((struct TaskHeader *)task)->unk4 = 0;
    for (j = 0; j < 4; j++)
        task->unk8[j] = data[j];
    SYS_AREA->taskCount++;
    return task;
}

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

/* Pauses a task: clears its active bit and sets its ran-this-frame bit, so run_tasks skips it
   (set_flags_2_3 sets both bits and resumes it from the next frame). */
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
/* Tears down the serial link serial_link_init set up: masks and acknowledges the timer 3 and serial
   IRQs, leaves SIO idle in multiplayer mode at 115200 bps, stops timer 3 (reload 0xABFB), frees the
   link state (and its buffer at +0x180) and the transfer buffer, and clears bit 7 of the system's
   link mode byte (gSys + 0x888). */
void serial_link_shutdown(void)
{
    REG_IME = 0;
    REG_IE = REG_IE & (u16)~IRQ_TIMER3;
    REG_IF = REG_IF | IRQ_TIMER3;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE = REG_IE & (u16)~IRQ_SERIAL;
    REG_IF = REG_IF | IRQ_SERIAL;
    REG_IME = 1;
    REG_IME = 0;
    REG_SIOCNT = SIO_MULTI_MODE | SIO_115200_BPS;
    REG_TM3CNT = 0xABFB; /* timer off (control 0), reload 0xABFB */
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

void serial_packet_checksum_write(void)
{
    u8 *buf;
    u8 first;
    u16 *hw;
    u32 sum;
    s32 i;
    u16 zero;

    buf = gUnk_03000D0C->buf1C;
    first = gUnk_03000D0C->unkB;
    zero = 0;
    buf[0] = first;
    gUnk_03000D0C->buf1C[1] = gUnk_03000D0C->unk2 ^ gUnk_03000D0C->unk3;
    {
        struct LinkState *state = gUnk_03000D0C;
        ((u16 *)state->buf1C)[1] = zero;
        cpu_set(gUnk_03000D10 + 2, *(u8 *volatile *)&state->buf1C + 4, 0x04000004);
    }
    hw = (u16 *)gUnk_03000D0C->buf1C;
    sum = 0;
    for (i = 0; i < 10; i++)
        sum += hw[i];
    hw[1] = ~sum - 12;
    gUnk_03000D0C->unk4 = 1;
}

ASM_FUNC("asm/code_0801931C/move_complete_records.s", u8 move_complete_records(u8 *base, u32 unused, u32 arg2, u32 arg3))

/* Once a frame, while a serial link is open. The parent, once past the handshake and asked to send,
 * starts the next transfer: swaps the receive buffers, swaps in the newly built packet if there is
 * one, notes SIOCNT's error bit, sends the 0xFEFE sync word and starts timer 3 to clock the rest.
 * A child instead sets the serial bit of the BIOS IntrWait flags if unk9 is clear, then clears unk9. */
void serial_link_update(void)
{
    struct LinkState *link = gUnk_03000D0C;
    if (link != 0)
    {
        u32 isParent = link->unk0;
        if (isParent != 0)
        {
            if (link->unk1 != 0 && link->unk6 != 0)
            {
                u32 recv;
                u8 *packet;
                vu16 *sio;
                u32 sync;
                link->unk18 = 0xFFFFFFFE;
                recv = link->unk28;
                link->unk28 = link->unk24;
                link->unk24 = recv;
                if (link->unk4 != 0)
                {
                    packet = link->buf20;
                    link->buf20 = link->buf1C;
                    link->buf1C = packet;
                    link->unk4 = 0;
                    gUnk_03000D0C->unk14 = 0;
                }
                gUnk_03000D0C->unk7 = ((*(vu32 *)REG_ADDR_SIOCNT) << 25) >> 31; /* SIOCNT bit 6: error */
                sio = (vu16 *)REG_ADDR_SIOCNT;
                sync = 0xFEFE;
                sio[1] = sync;                 /* REG_SIOMLT_SEND */
                sio[0] = sio[0] | SIO_START;
                REG_TM3CNT_H = TIMER_ENABLE | TIMER_IRQ_ENABLE;
            }
        }
        else
        {
            if (link->unk9 == 0)
            {
                REG_IME = isParent;
                *(vu16 *)0x03007FF8 = *(vu16 *)0x03007FF8 | IRQ_SERIAL; /* INTR_CHECK, BIOS IntrWait flags */
                REG_IME = 1;
            }
            gUnk_03000D0C->unk9 = isParent;
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

struct SysBits88A
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 rest : 4;
};

NONMATCH("asm/code_0801931C/obj_init_gfx.s", struct ListNode *obj_init_gfx(struct ListNode *obj, u8 kind, u8 *taskData, void *data))
{
    u8 *base;
    struct GfxObj *g;
    u16 *shadow;
    s32 k10 = 0x10;

    task_insert(obj, kind, taskData);
    g = (struct GfxObj *)obj;
    g->handler = (void *)gObjHandler;
    base = gSys;
    ((struct SysBits88A *)&base[0x88A])->bit3 = 0;
    g->unk1C = k10;
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
    gUnk_02000054 = k10;
    shadow = (u16 *)((u8 *)shadow - 0x48);
    *shadow = 0x88;
    shadow = (u16 *)((u8 *)shadow - 8);
    *shadow = 0x140;
    return obj;
}
END_NONMATCH

void task_remove(struct ListNode *task, u32 flags);
/* The task that shows the full-screen picture obj_init_gfx loads (it sets BLDCNT to brighten and
   BLDY to 16, so the screen starts white). */
struct AnimObj
{
    u8 state;           /* a PICTURE_* state */
    u8 filler1[3];
    u32 counter;        /* task header word, cleared by task_insert */
    u8 filler8[0x18 - 0x8];
    void *handler;
    s8 timer;           /* frames left in the current state */
    u8 filler1D;
    u16 keys;           /* d-pad keys held this frame */
    u16 prevKeys;
    u8 filler22[2];
    u32 dpadPressTime;      /* counter when all four directions were last pressed together */
    u32 prevDpadPressTime;
    void (*onDone)(void);   /* runs once the picture has faded out */
};

enum
{
    PICTURE_FADE_IN,  /* BLDY 16 -> 0 */
    PICTURE_HOLD,     /* 50 frames */
    PICTURE_FADE_OUT, /* BLDY 0 -> 16 */
};

/*
 * Runs the picture task once a frame: fades the picture in from white, holds it 50 frames and fades
 * it back to white through the BLDY shadow (gUnk_02000054), then clears the DISPCNT and BLDCNT
 * shadows, removes the task and calls onDone. Meanwhile, pressing all four d-pad directions at once
 * twice within 3 ticks (which a real d-pad can't do) switches the system's link mode to 1.
 */
void anim_state_step(struct AnimObj *task)
{
    u16 old;
    register u32 keys asm("r0");
    u32 lastPress;
    u32 now;

    switch (task->state)
    {
    case PICTURE_FADE_IN:
    {
        s32 t;

        task->timer--;
        gUnk_02000054 = task->timer;
        t = task->timer;
        if (t == 0)
        {
            task->timer = 50;
            task->state++;
        }
        break;
    }
    case PICTURE_HOLD:
        task->timer--;
        if (task->timer == 0)
        {
            task->timer = 16;
            task->state++;
        }
        break;
    case PICTURE_FADE_OUT:
    {
        s32 t;

        task->timer--;
        gUnk_02000054 = 16 - task->timer;
        t = task->timer;
        if (t == 0)
        {
            if (task != NULL)
            {
                u8 *sys;
                task->handler = (void *)gObjHandler;
                sys = gSys;
                sys[0x88A] |= 8; /* SysBits88A.bit3 */
                *(vu16 *)&REG_BUF_DISPCNT = 0;
                *(vu16 *)&REG_BUF_BLDCNT = 0;
                task_remove((struct ListNode *)task, 3);
            }
            if (task->onDone != NULL)
                task->onDone();
            return;
        }
        break;
    }
    }

    old = task->keys;
    task->prevKeys = old;
    keys = REG_KEYINPUT;
    keys = ~keys & DPAD_ANY;
    task->keys = keys;
    if ((keys & ~old & DPAD_ANY) == DPAD_ANY)
    {
        lastPress = task->dpadPressTime;
        task->prevDpadPressTime = lastPress;
        now = task->counter;
        task->dpadPressTime = now;
        if (lastPress != 0 && now - lastPress <= 3)
        {
            u8 *sys = gSys;
            ((struct LinkModeBits *)&sys[0x888])->mode = 1;
            ((struct LinkStateBits *)&gUnk_03000D18)->b1_3 = 0;
        }
    }
}

/* Starts the serial link when the system's link mode (gSys + 0x888, bits 1-2) is 1: resets the
   link state (gUnk_03000D18), puts SIO in 32-bit normal mode with its IRQ on and an external
   clock, installs serial_irq_handler and serial_timeout as the serial and timer 3 handlers,
   clears the 3-word buffer at gUnk_03000D30, starts a transfer and runs timer 3 as the timeout
   (reload 0x8000, 64-cycle ticks, IRQ on). */
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
        // both IRQs off and acknowledged while the link is set up
        REG_IME = 0;
        REG_IE = REG_IE & (u16)~IRQ_TIMER3;
        REG_IF = REG_IF | IRQ_TIMER3;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE = REG_IE & (u16)~IRQ_SERIAL;
        REG_IF = REG_IF | IRQ_SERIAL;
        REG_IME = 1;
        REG_RCNT = 0;
        REG_SIOCNT = SIO_32BIT_MODE | SIO_SO_HIGH;
        REG_SIOCNT = REG_SIOCNT | SIO_IRQ_ENABLE;
        set_irq_callback(1, serial_irq_handler);
        set_irq_callback(0, serial_timeout);
        REG_IME = 0;
        REG_IE = REG_IE | IRQ_SERIAL;
        REG_IF = REG_IF | IRQ_SERIAL;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE = REG_IE | IRQ_TIMER3;
        REG_IF = REG_IF | IRQ_TIMER3;
        REG_IME = 1;
        // external shift clock
        ((struct BitB0 *)REG_ADDR_SIOCNT)->b0 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b1_3 = 1;
        zero = 0;
        cpu_set(&zero, gUnk_03000D30, 0x05000003); // fill 3 words with 0
        REG_IME = 0;
        REG_SIOCNT = REG_SIOCNT | SIO_START;
        REG_IME = 1;
        REG_TM3CNT_L = 0x8000;
        REG_TM3CNT_H = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_64CLK;
    }
}

/* Stops the serial link started by serial_link_enable (when the link mode, gSys + 0x888 bits 1-2,
   is 1): clears the link state bits, turns the timer 3 and serial IRQs off and acknowledges them,
   puts SIO back in multiplayer mode and stops timer 3 (reload 0x8000, control 0). */
void serial_link_disable(void)
{
    u8 *base = gSys;

    if (((struct LinkModeBits *)(base + 0x888))->mode == 1)
    {
        ((struct LinkStateBits *)&gUnk_03000D18)->b0 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b1_3 = 0;
        ((struct LinkStateBits *)&gUnk_03000D18)->b4_5 = 0;
        REG_IME = 0;
        REG_IE = REG_IE & (u16)~IRQ_TIMER3;
        REG_IF = REG_IF | IRQ_TIMER3;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE = REG_IE & (u16)~IRQ_SERIAL;
        REG_IF = REG_IF | IRQ_SERIAL;
        REG_IME = 1;
        REG_IME = 0;
        REG_SIOCNT = SIO_MULTI_MODE;
        REG_TM3CNT = 0x8000; /* TM3CNT_L (reload) = 0x8000, TM3CNT_H = 0: timer off */
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
    u32 acc;
    u8 i;
    u32 k = kind - 2;

    switch (k)
    {
    case 0:
        word = 0x10000010;
        acc = 1;
        for (i = 6; i != 0; i--)
            acc ^= (word >> (i << 2)) & 0xF;
        packet = (acc & 0xF) | word;
        break;
    case 1:
        word = ((((struct LinkRam *)&gUnk_03000D18)->rx2 & 0xFFFFFF) << 4) | 0x20000000;
        acc = 2;
        for (i = 6; i != 0; i--)
            acc ^= (word >> (i << 2)) & 0xF;
        packet = (acc & 0xF) | word;
        break;
    case 2:
        if ((((struct LinkRam *)&gUnk_03000D18)->phase & 0x30) == 0)
        {
            word = ((serial_slot_bits(0, ((struct LinkRam *)&gUnk_03000D18)->phase, (u32)&gUnk_03000D18) & 0xFFFFFF) << 4) | 0x40000000;
            acc = 4;
            for (i = 6; i != 0; i--)
                acc ^= (word >> (i << 2)) & 0xF;
            packet = (acc & 0xF) | word;
        }
        if ((((struct LinkRam *)&gUnk_03000D18)->phase & 0x30) == 0x10)
        {
            word = ((serial_slot_bits(1, ((struct LinkRam *)&gUnk_03000D18)->phase, (u32)&gUnk_03000D18) & 0xFFFFFF) << 4) | 0x40000000;
            acc = 4;
            for (i = 6; i != 0; i--)
                acc ^= (word >> (i << 2)) & 0xF;
            packet = (acc & 0xF) | word;
        }
        if ((((struct LinkRam *)&gUnk_03000D18)->phase & 0x30) == 0x20)
        {
            word = ((serial_slot_bits(2, ((struct LinkRam *)&gUnk_03000D18)->phase, (u32)&gUnk_03000D18) & 0xFFFFFF) << 4) | 0x40000000;
            acc = 4;
            for (i = 6; i != 0; i--)
                acc ^= (word >> (i << 2)) & 0xF;
            packet = (acc & 0xF) | word;
        }
        break;
    case 3:
    case 4:
        word = 0x10000010;
        acc = 1;
        for (i = 6; i != 0; i--)
            acc ^= (word >> (i << 2)) & 0xF;
        packet = (acc & 0xF) | word;
        break;
    }
    return packet;
}
END_NONMATCH
