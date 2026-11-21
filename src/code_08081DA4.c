#include "global.h"
#include "ram_routines.h"

struct SpriteTask;
struct SpriteObj;
struct DispEntry;
struct SpriteHead;
struct SpriteRunner;
typedef void (*TaskFunc)(struct SpriteTask *task);

struct SpriteTask
{
    u8 filler0[4];
    TaskFunc next;
    struct SpriteObj *obj;
    struct SpriteRunner *runner;
    u16 timer;
};

struct SpriteRunner
{
    u8 filler0[0x4C];
    void (*handler)(struct SpriteRunner *runner);
};

struct SpriteRecord
{
    u8 filler0[0x12];
    u8 unk12;
};

struct SpriteRecordBits
{
    u8 unk0 : 1;
    u8 mode : 2;
};

struct SpriteModeBits
{
    u8 unk0 : 5;
    u8 bit5 : 1;
};

struct SpriteObj
{
    u8 filler0[0x8];
    struct SpriteRecord *record;
    u8 filler0C[0x18 - 0xC];
    s32 unk18;
    u8 filler1C[0x28 - 0x1C];
    struct SpriteHead *head;
    u8 filler2C[0x38 - 0x2C];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x6C - 0x44];
    u16 **seq6C;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 filler76[0x79 - 0x76];
    u8 flags79;
    u8 filler7A[0xC5 - 0x7A];
    s8 unkC5;
};

struct SpriteHead
{
    u8 filler0[0xD8];
    s32 unkD8;
    s32 unkDC;
    u8 filler0E0[0xEC - 0xE0];
    u8 unkEC;
    u8 filler0ED[0xF6 - 0xED];
    s16 timerF6;         /* HP: 0 knocks the object out */
    u8 filler0F8[0x108 - 0xF8];
    s16 size108;
    u8 filler10A[0x111 - 0x10A];
    u8 flags111;         /* bit 0: being knocked out (task_release_obj), cleared when it's done */
};

struct MapObj
{
    u8 filler0[0x18];
    u32 descriptor;
    u8 filler1C[0x774 - 0x1C];
    s16 lastX;
    s16 lastY;
    u16 targetX;
    u16 targetY;
};

struct ListEntry
{
    u16 unk0;
    u16 unk2;
    void *block;
};

struct SpritePart
{
    u8 filler0[0x7E];
    u8 flags7E;
    u8 filler7F[0xE4 - 0x7F];
    u16 *script;
};

struct SpritePartBits
{
    u8 unk0 : 1;
    u8 mode : 2;
};

struct SpriteStateBits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
};

struct SpriteLo2Bits
{
    u8 lo2 : 2;
};

/* gWorld->unkB as bits */
struct WorldUnpackBits
{
    u8 unk0 : 4;
    u8 unpackBusy : 1; /* gUnk_03000C88 still holds decompression jobs */
};

struct SpriteBit2Bits
{
    u8 lo2 : 2;
    u8 bit2 : 1;
};

/* The low byte of DispEntry.tileInfo */
struct DispKindBits
{
    u8 kind : 2;
    u8 rest : 6;
};

/* Copy modes in DispEntry.tileInfo bits 0-1 */
#define UPLOAD_NONE 0 /* unpacked straight into VRAM */
#define UPLOAD_CPU  1
#define UPLOAD_DMA  2

/*
 * A tile upload: packed graphics are decompressed into unpackBuf, then
 * copied to dstAddr. A stack in gWorkBuf + 0x3668, gWorld->uploadTop
 * points at the top entry.
 */
struct DispEntry
{
    u16 tileInfo;        /* bits 0-1: copy mode, bits 2-15: size in 0x20-byte tiles */
    u16 srcOffset;       /* where in unpackBuf the copy starts */
    u32 dstAddr;
    /* From here on a decompression job, as submitted with table_register */
    u32 unpackState;     /* 0 unpacked, 1 to submit, 2 submitted */
    void *packedSrc;
    u32 unpackBuf;
    u8 filler14[4];
};

struct SpriteState
{
    u8 *unk0;
    u8 filler4[0xB - 4];
    u8 unkB;
    u8 filler0C;
    u8 unkD;
    u8 filler0E[2];
    u8 unk10;
    u8 filler11[0x12 - 0x11];
    u16 objPalDirty;     /* OBJ palettes the fader changed, one bit each */
    u8 filler14[0x58 - 0x14];
    struct SpritePart *part58[6];
    u8 filler70[0x1F0 - 0x70];
    u32 defaultUnpackBuf;
    struct ListEntry *list1F4;
    struct DispEntry *uploadTop;
    u8 filler1FC[0x200 - 0x1FC];
    void *unk200[8];
    u8 filler220[0x244 - 0x220];
    void *unk244;
    void *palFade;       /* screen fader; its working palette is at +0x1C */
    u8 filler24C[0x2BE - 0x24C];
    u8 unk2BE;
    u8 unk2BF;
    u8 unk2C0;
    u8 filler2C1[0x2DC - 0x2C1];
    u8 *unk2DC;
    u8 filler2E0[0x349 - 0x2E0];
    u8 unk349;
};

extern struct SpriteState *gWorld;
extern const u8 gMapObjDescriptor[];

void map_loader_map_obj_destroy(struct MapObj *obj, u32 param);
void sprite_load_active_tables(void);
void task_state_machine(struct SpriteTask *task);
void task_run_parts(struct SpriteTask *task);
void state_set_flags_and_run(struct SpriteTask *task);
void set_flags_2_3(void *flags);
void world_begin_scene(struct SpriteTask *task);
void release_block(void *block);
void ensure_code_entry(u32 key);
s32 is_state_busy(void);
s32 set_free_list_head(void);
void run_obj_entry();
void move_slots_toward(struct SpriteObj *obj);
u8 step_motion(struct SpriteObj *obj);
void map_obj_set_impulse(struct MapObj *obj, s32 dx, s32 dy);
u32 lookup_or_alloc_13b4(u32 a, u32 key, s32 b, s32 c);
extern const u8 gPartTemplates[];
void state_move_then_check(struct SpriteTask *task);
void state_move_then_image(struct SpriteTask *task);
void state_move_then_check_bit3(struct SpriteTask *task);
void state_wait_mode_bit1(struct SpriteTask *task);
void owner_wait_entry(void);
void task_create_list_4c(void (*fn)(void), u32 arg);
void sprite_set_action(struct SpriteObj *obj, u32 action, u32 arg, u32 extra);
void lookup_or_alloc_14b8(u32 a, u32 key, s32 b, s32 c);
void set_flag_2_2(void *ptr);
void load_table_entries(u32 code);
void *spawn_obj_by_id(u32 id, u32 a, u32 b, u32 c);
void state_timed_motion(struct SpriteTask *task);
void state_wait_arrival(struct SpriteTask *task);
void state_countdown_reset(struct SpriteTask *task);
void state_countdown_next(struct SpriteTask *task);
void state_start_idle(struct SpriteTask *task);
void state_countdown_start(struct SpriteTask *task);
void task_release_obj(void);
void state_wait_flag(struct SpriteTask *task);
void sprite_sum_parts(struct SpriteObj *obj);
void place_targets(struct SpriteObj *obj, s32 x, s32 y, s32 z, s32 w);
void set_step_and_offsets(struct SpriteObj *obj, u32 a, s32 b);
void anim_bit200_step(struct SpriteHead *head, u32 arg);
void anim_bit100_step(struct SpriteHead *head, u32 arg);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct SpriteObj *owner);
u8 *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(u8 *obj, u32 a, s32 b, u32 c, u32 flag);
void task_free_blocks(struct SpriteTask *task);
u16 *load_table_value(s32 *count, u16 code);

/* Resource ids (lookup_or_alloc_14b8): bits 12-15 pick the bank ((id >> 12) - 1), bits 0-11 the entry. */
#define RES_BANK_MASK 0xF000
/* Banks whose resources aren't preloaded with lookup_or_alloc_14b8. */
#define RES_BANK_PRELOADS(bank) ((bank) != 0x2000 && (bank) != 0xB000 && (bank) != 0x4000 && (bank) != 0x7000)
/* In a preload list: ends the resource groups; single resource ids follow. */
#define PRELOAD_GROUPS_END 0xFFFF
extern u16 gEffectCodeTable[];
extern u16 gSpriteLoadList[];
extern u8 *gWorkBuf;
extern u8 gSys[];
u32 read_varlen_plus1(const u8 *src);
u32 mt19937_next(void);
void list_remove_active(u8 *obj);
void table_register(void *p);
u32 sub_obj_running(void *menu);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
extern u8 gUnk_02000280[];
extern u32 gUnk_03000C88[];
s32 rand_below(s32 n, u32 seed);
void number_popup(s32 value, s32 x, s32 y, s32 z, u32 a, u32 b, u32 c);
extern const u16 gActionCodes[];

ASM_FUNC("asm/code_08081DA4/move_slots_toward.s", void move_slots_toward(struct SpriteObj *obj))

void state_start_wait(struct SpriteTask *task);
static inline void show_part_scaled(struct SpriteObj *obj, s32 index)
{
    struct SpritePart *part = gWorld->part58[index];
    struct SpriteObj *slotObj = (struct SpriteObj *)((u8 *)part + 8);
    s32 objSize;
    s32 slotSize;
    s32 scale;
    s32 ratio;
    s16 kind;

    if ((part->flags7E & 6) == 4)
    {
        objSize = obj->head->size108;
        slotSize = slotObj->head->size108;
        kind = *(s16 *)&obj->head->unkEC;
        scale = (kind == -1) ? 0x78 : 0x8C;
        ratio = (scale * objSize) / slotSize;
        anim_bit100_step(slotObj->head, ratio);
        sprite_set_action(slotObj, 0, -1, 0);
    }
}

/*
 * Task state: plays action 3 with animation 0x2054 (0x2025 when bit 0 of
 * gWorld->unk2BE is clear) and spawns effect 0x1E0 on the object. Each of the six
 * parts in gWorld->part58 whose mode is 2 is scaled to the object's size (120%, or
 * 140% when the object's head->unkEC isn't -1; see show_part_scaled) and redrawn.
 * Then waits 30 frames in state_start_wait.
 */
void state_show_and_scale(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    s32 variant;
    s32 anim;
    s32 part;

    variant = gWorld->unk2BE & 1;
    anim = 0x2054;
    if (variant == 0)
    {
        anim -= 0x2F;
    }
    sprite_set_action(obj, 3, anim, 0);
    ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 0;
    effect_spawn_at(0x1E0, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
    /* parts 0-5, two at a time */
    for (part = 0; part <= 5; part += 2)
    {
        show_part_scaled(obj, part);
        show_part_scaled(obj, part + 1);
    }
    task->timer = 0x1E;
    task->next = state_start_wait;
}

void state_start_motion(struct SpriteTask *task);
/*
 * The object takes a random hit: 1-3 damage (doubled while gWorld->unkD bit 0 is set) comes off
 * its HP (head->timerF6, not below 0) and pops up as a number above it. It shows the hurt pose
 * and starts hopping back home (head->unkD8/unkDC) in 4 hops, this one a quarter of the way;
 * state_timed_motion makes the rest.
 */
void state_random_step(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    s32 damage;
    s32 hpLeft;
    s32 hopX;
    s32 hopY;
    s32 anim;
    s16 *hp;
    s32 three;
    s32 *homeX;
    s32 (*div)(s32, s32);

    sprite_sum_parts(obj);
    damage = rand_below(3, mt19937_next()) + 1;
    if (gWorld->unkD & 1)
    {
        damage <<= 1;
    }
    hp = &obj->head->timerF6;
    if (*hp <= damage)
    {
        hpLeft = 0;
    }
    else
    {
        hpLeft = (u16)*hp - damage;
    }
    *hp = hpLeft;
    number_popup(damage, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100 + obj->unkC5, 1, 0xB, 0);
    anim = gActionCodes[5];
    if ((1 & gWorld->unk2BE) != 0)
    {
        anim++;
    }
    sprite_set_action(obj, 0, anim, 0);
    ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 0;
    task->timer = 4;
    /* first hop: (home + 3 * position) / 4 */
    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    homeX = &obj->head->unkD8;
    three = 3;
    hopX = div((*homeX + obj->unk38 * three) / 0x100, 4);
    hopY = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->head->unkDC + obj->unk3C * three) / 0x100, 4);
    place_targets(obj, hopX, hopY, 0, 0x100);
    set_step_and_offsets(obj, 0x700, obj->unk40 / 0x100 + 0x18);
    task->timer--;
    task->next = state_start_motion;
}


/* Task state after state_random_step: once the current hop has landed, hop again 1/n of the way
   from the object's position (unk38/unk3C) back home (head->unkD8/unkDC), n being the hops left
   in task->timer, each hop peaking 0x14 above the current height. With no hops left it waits 20
   frames in state_wait_arrival. */
void state_timed_motion(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    s32 hopsLeft;
    s32 hopX;
    s32 hopY;
    if ((u8)step_motion(obj) != 0)
    {
        return;
    }
    if ((s16)task->timer != 0)
    {
        hopsLeft = (s16)task->timer;
        /* (home + position * (n - 1)) / n, in whole pixels */
        hopX = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->head->unkD8 + obj->unk38 * (hopsLeft - 1)) / 0x100, hopsLeft);
        hopY = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->head->unkDC + obj->unk3C * (hopsLeft - 1)) / 0x100, hopsLeft);
        place_targets(obj, hopX, hopY, 0, 0x100);
        set_step_and_offsets(obj, 0x700, obj->unk40 / 0x100 + 0x14);
        task->timer--;
    }
    else
    {
        task->timer = 0x14;
        task->next = state_wait_arrival;
    }
}

/*
 * Task state after the last hop home (state_timed_motion). An object whose HP (head->timerF6) is 0
 * is knocked out: flags111 bit 0 is set, task_release_obj sends it away, gWorld->unk2C0 gets 1 in
 * its low two bits and, in bit 2, the inverse of bit 0 of head->unkEC, and the task waits in
 * state_wait_flag for the knock-out to finish. Otherwise it waits task->timer frames, puts the
 * object back in its idle pose (action 0) and goes on with state_set_flags_and_run.
 */
void state_wait_arrival(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    struct SpriteHead *head = obj->head;
    s32 hp = head->timerF6;
    struct SpriteState *world;
    u8 notOdd;

    if (hp == 0)
    {
        head->flags111 |= 1;
        task_create_list_4c(task_release_obj, (u32)obj);
        task->next = state_wait_flag;
        task->timer = hp;
        ((struct SpriteLo2Bits *)&gWorld->unk2C0)->lo2 = 1;
        world = gWorld;
        notOdd = ~obj->head->unkEC & 1;
        ((struct SpriteBit2Bits *)&world->unk2C0)->bit2 = notOdd;
    }
    else
    {
        u16 framesLeft = task->timer;

        if ((s16)task->timer > 0)
        {
            task->timer = framesLeft - 1;
        }
        else
        {
            sprite_set_action(obj, 0, -1, 0);
            ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 0;
            task->next = state_set_flags_and_run;
        }
    }
}

void state_draw_pose(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    s32 code;
    s32 flag;

    anim_bit200_step(obj->head, 0xC8);
    flag = gWorld->unk2BE & 1;
    code = 0x166;
    if (flag == 0)
    {
        code -= 0xC;
    }
    effect_spawn_at(code, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
    task->timer = 0x3C;
    task->next = state_start_idle;
}

void state_wait_flag(struct SpriteTask *task)
{
    struct SpriteHead *head = task->obj->head;

    if (!(head->flags111 & 1))
    {
        task->next = state_set_flags_and_run;
    }
}

void state_start_motion(struct SpriteTask *task)
{
    run_obj_entry((u32)task);
    task_create_list_4c(owner_wait_entry, 0);
    gWorld->unk10 = 4;
    {
        s32 flags = gSys[0x888];

        flags |= 0x10;
        gSys[0x888] = flags;
    }
    task->next = state_timed_motion;
}

void state_start_wait(struct SpriteTask *task)
{
    run_obj_entry((u32)task);
    task_create_list_4c(owner_wait_entry, 0);
    gWorld->unk10 = 4;
    {
        s32 flags = gSys[0x888];

        flags |= 0x10;
        gSys[0x888] = flags;
    }
    task->next = state_countdown_reset;
}

void state_set_anim_and_draw(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    struct SpriteHead *head;
    s32 x;
    s32 y;

    sprite_sum_parts(obj);
    head = obj->head;
    x = head->unkD8 / 0x100;
    y = head->unkDC / 0x100;
    place_targets(obj, x, y, 0, 0x100);
    set_step_and_offsets(obj, 0x600, obj->unk40 / 0x100 + 0x20);
    sprite_set_action(obj, 5, 0, 0);
    ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 1;
    task->timer = 0x14;
    task->next = state_countdown_start;
}

void run_sequence(s32 index)
{
    u8 *tpl = (u8 *)gPartTemplates + index * 0x2C;
    u16 *cursor = *(u16 **)(tpl + 0x14);
    u16 item;

    if (cursor == 0)
    {
        return;
    }
    do
    {
        item = *cursor;
        cursor++;
        if (item == 0)
        {
            return;
        }
    } while (item != 0xFFFF);
    item = *cursor;
    cursor++;
    if (item == 0)
    {
        return;
    }
    if (item != 0xFFFF)
    {
        do
        {
            load_table_entries(item);
            item = *cursor;
            cursor++;
            if (item == 0)
            {
                return;
            }
        } while (item != 0xFFFF);
    }
    for (;;)
    {
        item = *cursor;
        cursor++;
        if (item == 0)
        {
            return;
        }
        ensure_code_entry(item);
    }
}

void noop_callback_0808261C(void)
{
}

void run_sequence_of_obj(struct SpriteObj *obj)
{
    u16 *base = *obj->seq6C;
    u16 *cursor = (u16 *)base + 1;
    u16 item;

    if (cursor == 0)
    {
        return;
    }
    item = base[1];
    cursor++;
    if (item == 0)
    {
        return;
    }
    if (item != 0xFFFF)
    {
        do
        {
            load_table_entries(item);
            item = *cursor;
            cursor++;
            if (item == 0)
            {
                return;
            }
        } while (item != 0xFFFF);
    }
    for (;;)
    {
        item = *cursor;
        cursor++;
        if (item == 0)
        {
            return;
        }
        ensure_code_entry(item);
    }
}

void run_sequence_of_obj_b(struct SpriteObj *obj)
{
    u16 *cursor = *obj->seq6C;
    u16 item;

    if (cursor == 0)
    {
        return;
    }
    item = *cursor;
    cursor++;
    if (item == 0)
    {
        return;
    }
    if (item != 0xFFFF)
    {
        do
        {
            load_table_entries(item);
            item = *cursor;
            cursor++;
            if (item == 0)
            {
                return;
            }
        } while (item != 0xFFFF);
    }
    for (;;)
    {
        item = *cursor;
        cursor++;
        if (item == 0)
        {
            return;
        }
        ensure_code_entry(item);
    }
}

void noop_stub_080826AC(void)
{
}

void draw_fixed_labels(void)
{
    ensure_code_entry(0x2027);
    ensure_code_entry(0x2046);
    ensure_code_entry(0x2056);
    ensure_code_entry(0x2075);
    ensure_code_entry(0x2089);
}

void sprite_load_tables(void)
{
    u32 idx;
    u16 *list;
    u16 entry;

    idx = gWorld->unk2DC[2];
    ensure_code_entry(idx + 0x4188);
    idx = gWorld->unk349;
    load_table_entries(gEffectCodeTable[idx]);
    load_table_entries(gEffectCodeTable[idx]);
    list = gSpriteLoadList;
    if (list == 0)
    {
        return;
    }
    entry = *list;
    list++;
    if (entry == 0)
    {
        return;
    }
    if (entry != 0xFFFF)
    {
        do
        {
            load_table_entries(entry);
            entry = *list;
            list++;
            if (entry == 0)
            {
                return;
            }
        } while (entry != 0xFFFF);
    }
    for (;;)
    {
        entry = *list;
        list++;
        if (entry == 0)
        {
            return;
        }
        ensure_code_entry(entry);
    }
}

void run_slot_script(s32 index)
{
    struct SpritePart *part;

    if (gWorld->list1F4 == 0)
    {
        return;
    }
    if (index < 0)
    {
        return;
    }
    if (index > 5)
    {
        return;
    }
    part = gWorld->part58[index];
    if (((struct SpritePartBits *)&part->flags7E)->mode == 2
        || ((struct SpritePartBits *)&part->flags7E)->mode == 1)
    {
        if (part->script != 0)
        {
            load_table_entries(part->script[1]);
        }
    }
}

void state_set_flags_and_run(struct SpriteTask *task)
{
    set_flags_2_3(gWorld->unk244);
    task->next = world_begin_scene;
    task->next(task);
}

void state_wait_idle(struct SpriteTask *task)
{
    if (!(u8)is_state_busy())
    {
        set_free_list_head();
        sprite_load_active_tables();
        task->next = task_state_machine;
    }
}

void state_clear_mode_bit(struct SpriteTask *task)
{
    if (gWorld->unk2BF & 0x20)
    {
        ((struct SpriteModeBits *)&gWorld->unk2BF)->bit5 = 0;
        set_flag_2_2(gWorld->unk244);
        task->next = state_wait_idle;
        task->next(task);
    }
}

void state_spawn_object(struct SpriteTask *task)
{
    u32 busy = (u8)is_state_busy();

    if (busy == 0)
    {
        u8 *obj = gWorld->unk0;

        release_block(*(void **)(*(u8 **)(obj + 0x48) + 8));
        obj = create_obj_de4(0, 0x2091, 0, -1, -1, -1, -1);
        obj_load_record(obj, 0, -1, 0, busy);
        gWorld->unk0 = obj;
        task->next = task_free_blocks;
    }
}

void state_spawn_object(struct SpriteTask *task);
/* Task state: once nothing is busy, spawns eight 0x2084 objects into gWorld->unk200, creates the
   scene object from resource 0x7000 (loading its record) as gWorld->unk0, and goes on to
   state_spawn_object, which swaps it for 0x2091. */
void state_fill_table_and_spawn(struct SpriteTask *task)
{
    s32 i;
    u8 *sceneObj;

    if (!(u8)is_state_busy())
    {
        for (i = 0; i < 8; i++)
        {
            gWorld->unk200[i] = spawn_obj_by_id(0x2084, 0, 2, 1);
        }
        sceneObj = create_obj_de4(0, 0x7000, 0, -1, -1, -1, -1);
        obj_load_record(sceneObj, 0, -1, 0, 0);
        gWorld->unk0 = sceneObj;
        task->next = state_spawn_object;
    }
}

void state_run_handler(struct SpriteTask *task)
{
    struct SpriteRunner *runner = task->runner;

    if (runner->handler != 0)
    {
        runner->handler(runner);
        if (runner->handler != 0)
        {
            return;
        }
    }
    task->next = task_run_parts;
}

void state_start_idle(struct SpriteTask *task)
{
    run_obj_entry((u32)task);
    task_create_list_4c(owner_wait_entry, 0);
    gWorld->unk10 = 4;
    {
        s32 flags = gSys[0x888];

        flags |= 0x10;
        gSys[0x888] = flags;
    }
    task->next = state_countdown_next;
}

void state_countdown_reset(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    u16 t = task->timer;

    if ((s16)task->timer > 0)
    {
        task->timer = t - 1;
    }
    else
    {
        sprite_set_action(obj, 0, -1, 0);
        ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 0;
        task->next = state_set_flags_and_run;
    }
}

void state_countdown_start(struct SpriteTask *task)
{
    if ((s16)task->timer > 1)
        task->timer--;
    else
        task->timer = 1;
    run_obj_entry();
    task_create_list_4c(owner_wait_entry, 0);
    gWorld->unk10 = 4;
    {
        s32 flags = gSys[0x888];

        flags |= 0x10;
        gSys[0x888] = flags;
    }
    task->next = state_move_then_check;
}

void state_countdown_next(struct SpriteTask *task)
{
    u16 t = task->timer;

    if ((s16)task->timer > 0)
    {
        task->timer = t - 1;
    }
    else
    {
        task->next = state_set_flags_and_run;
    }
}

void state_move_then_check(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;

    step_motion(obj);
    {
        u16 t = task->timer;

        if ((s16)task->timer > 0)
        {
            s32 left = t - 1;

            task->timer = left;
            if ((u16)left == 0)
            {
                move_slots_toward(obj);
            }
        }
    }
    if (obj->unk40 >= obj->unk18)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 1;
        task->next = state_move_then_image;
    }
}

void state_move_then_image(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    u16 t = task->timer;

    if ((s16)task->timer > 0)
    {
        s32 left = t - 1;

        task->timer = left;
        if ((u16)left == 0)
        {
            move_slots_toward(obj);
        }
    }
    if (!(u8)step_motion(obj))
    {
        sprite_set_action(obj, 8, 0, 0);
        task->next = state_move_then_check_bit3;
    }
}

void state_move_then_check_bit3(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    u16 t = task->timer;

    if ((s16)task->timer > 0)
    {
        s32 left = t - 1;

        task->timer = left;
        if ((u16)left == 0)
        {
            move_slots_toward(obj);
        }
    }
    if (obj->record->unk12 & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        ((struct SpriteRecordBits *)&obj->record->unk12)->mode = 0;
        task->next = state_wait_mode_bit1;
    }
}

void state_wait_mode_bit1(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    u16 t = task->timer;

    if ((s16)task->timer > 0)
    {
        s32 left = t - 1;

        task->timer = left;
        if ((u16)left == 0)
        {
            move_slots_toward(obj);
        }
    }
    else if (gWorld->unkB & 2)
    {
        ((struct SpriteStateBits *)&gWorld->unkB)->bit1 = 0;
    }
    else
    {
        task->next = state_set_flags_and_run;
    }
}

ASM_FUNC("asm/code_08081DA4/init_sprite_pools.s", void init_sprite_pools(void))

/*
 * Once a frame: while the screen fader runs, copies the OBJ palettes it changed
 * into gUnk_02000280 (the OBJ palette shadow), then handles the top tile upload
 * (starts its decompression, or copies the unpacked tiles to VRAM and pops it),
 * and sets unpackBusy while gUnk_03000C88 still holds decompression jobs.
 */
void refresh_sprite_display(void)
{
    u32 dirty;
    u8 *palDst;
    u8 *palSrc;
    u16 *palMask;
    u8 *sys;
    struct DispEntry *upload;
    s32 state;
    s32 mode;
    s32 i;

    if ((u8)sub_obj_running(gWorld->palFade) != 0)
    {
        sys = gSys;
        palMask = (u16 *)(sys + 0x884);
        dirty = palMask[1];          /* OBJ half of the palette mask */
        dirty &= *(u16 *)sys;        /* OBJ palettes enabled */
        dirty &= gWorld->objPalDirty;
        palDst = gUnk_02000280;
        /* the fader's working palette: BG at +0, OBJ at +0x200 */
        palSrc = *(u8 **)((u8 *)gWorld->palFade + 0x1C) + 0x200;
        if (dirty != 0)
        {
            do
            {
                if ((dirty & 1) != 0)
                {
                    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(palDst, palSrc, 0x20);
                }
                dirty >>= 1;
                palDst += 0x20;
                palSrc += 0x20;
            } while (dirty != 0);
        }
    }
    gWorld->objPalDirty = 0;
    upload = gWorld->uploadTop;
    if (upload != 0)
    {
        state = upload->unpackState;
        if (state != 0)
        {
            if (state == 1)
            {
                table_register((u8 *)upload + 8);
            }
        }
        else
        {
            mode = (((u32)*(u8 *)upload) << 30) >> 30;
            switch (mode)
            {
            case UPLOAD_CPU:
                MEM_ROUTINE(void (*)(u8 *, u32, s32), iwram_copy)((u8 *)upload->unpackBuf + upload->srcOffset, upload->dstAddr, (((s16)upload->tileInfo) >> 2) << 5);
                break;
            case UPLOAD_DMA: /* at the next VBlank */
                dma_copy_or_queue((u8 *)upload->unpackBuf + upload->srcOffset, (void *)upload->dstAddr, (((s16)upload->tileInfo) >> 2) << 5, 0x20, state);
                break;
            case UPLOAD_NONE:
                break;
            }
            if (upload == (struct DispEntry *)(gWorkBuf + 0x3668))
            {
                gWorld->uploadTop = 0;
            }
            else
            {
                gWorld->uploadTop = gWorld->uploadTop - 1;
            }
        }
    }
    ((struct WorldUnpackBits *)&gWorld->unkB)->unpackBusy = 0;
    for (i = 0; i < 0x20; i++)
    {
        if (gUnk_03000C88[i] != 0)
        {
            ((struct WorldUnpackBits *)&gWorld->unkB)->unpackBusy = 1;
            break;
        }
    }
}

/* Preloads the resources of a 0-terminated u16 list: first group codes (each a counted list of
 * resource ids, load_table_value) up to PRELOAD_GROUPS_END, then single resource ids. Every id that
 * isn't 0 and whose bank preloads goes through lookup_or_alloc_14b8. */
void unknown_code_08082f54(u16 *list)
{
    s32 count;
    s32 stop;
    s32 none;
    u32 mask;
    u32 bank;
    u16 *group;
    u16 code;
    u16 id;

    if (list == 0)
    {
        return;
    }
    code = *list;
    list++;
    if (code == 0)
    {
        return;
    }
    if (code != PRELOAD_GROUPS_END)
    {
        none = -1;
        stop = PRELOAD_GROUPS_END;
        do
        {
            group = load_table_value(&count, code);
            count--;
            while (count != none)
            {
                id = group[count];
                if (id != 0)
                {
                    mask = RES_BANK_MASK;
                    bank = mask & id;

                    if (RES_BANK_PRELOADS(bank))
                    {
                        lookup_or_alloc_14b8(0, id, -1, 1);
                    }
                }
                count--;
            }
            code = *list;
            list++;
            if (code == 0)
            {
                return;
            }
        } while (code != stop);
    }
    for (;;)
    {
        id = *list;
        list++;
        if (id == 0)
        {
            return;
        }
        {
            mask = RES_BANK_MASK;
            bank = mask & id;

            if (RES_BANK_PRELOADS(bank))
            {
                lookup_or_alloc_14b8(0, id, -1, 1);
            }
        }
    }
}

/* Pushes a tile upload onto the stack at gWorkBuf + 0x3668 (gWorld->uploadTop): `packed` is
   decompressed into `unpackBuf` (0: gWorld->defaultUnpackBuf; with UPLOAD_NONE straight into
   `dst`), then copied from srcOffset on to `dst` with copy mode `mode`. `size` is the size in
   bytes, or negative to take it from the packed data's header. */
void queue_display_entry(u32 mode, void *packed, void *dst, u32 size, u32 unpackBuf, u32 srcOffset)
{
    struct DispEntry **top;
    struct DispEntry *next;
    struct DispEntry *ent;
    u32 tiles;
    u32 sizeBits;
    u16 tileInfo;

    top = &gWorld->uploadTop;
    next = *top;
    if (next != 0)
    {
        next = next + 1;
    }
    else
    {
        next = (struct DispEntry *)(gWorkBuf + 0x3668);
    }
    *top = next;
    ent = gWorld->uploadTop;
    ((struct DispKindBits *)ent)->kind = mode;
    ent->dstAddr = (u32)dst;
    ent->srcOffset = srcOffset;
    if ((s32)size < 0)
    {
        tiles = read_varlen_plus1(packed) >> 5;
    }
    else
    {
        tiles = size >> 5;
    }
    /* size in 0x20-byte tiles goes in bits 2-15, next to the copy mode */
    sizeBits = tiles << 2;
    tileInfo = (ent->tileInfo & 3) | sizeBits;
    ent->tileInfo = tileInfo;
    ent->packedSrc = packed;
    if (mode != UPLOAD_NONE)
    {
        if (unpackBuf == 0)
        {
            ent->unpackBuf = gWorld->defaultUnpackBuf;
        }
        else
        {
            ent->unpackBuf = unpackBuf;
        }
    }
    else
    {
        ent->unpackBuf = (u32)dst;
    }
    ent->unpackState = 1; /* to submit */
}

void load_table_entries(u32 code)
{
    s32 count;
    u16 *table;

    table = load_table_value(&count, (u16)code);
    count--;
    while (count != -1)
    {
        u32 item = table[count];

        if (item != 0)
        {
            struct ListEntry *entry = (struct ListEntry *)(gWorkBuf + 0x3660);

            while (entry->unk0 != 0)
            {
                if (entry->unk0 == item)
                {
                    goto next;
                }
                entry--;
            }
            {
                u32 value = lookup_or_alloc_13b4(0, item, -1, 0);

                if (value != 0)
                {
                    if (value <= 0x07FFFFFF)
                    {
                        entry->unk0 = item;
                        entry->block = (void *)value;
                    }
                }
            }
        }
    next:
        count--;
    }
}

void load_table_entries_typed(u32 code)
{
    s32 count;
    u16 *table;

    table = load_table_value(&count, (u16)code);
    count--;
    while (count != -1)
    {
        u32 item = table[count];

        if (item != 0)
        {
            u32 kind = item & 0xF000;

            if (kind != 0x2000 && kind != 0xB000 && kind != 0x4000 && kind != 0x7000)
            {
                lookup_or_alloc_14b8(0, item, -1, 1);
            }
        }
        count--;
    }
}

void clear_list_entries(void)
{
    struct ListEntry *entry = gWorld->list1F4;

    if (entry != 0 && entry->unk0 != 0)
    {
        do
        {
            if (entry->block != 0)
            {
                release_block(entry->block);
            }
            entry->unk0 = 0;
            entry->block = 0;
            entry--;
        } while (entry->unk0 != 0);
    }
}

s32 unknown_code_0808320c(void)
{
    return 1;
}

s32 list_remove_key(u32 key)
{
    struct ListEntry *entry;
    struct ListEntry *head = gWorld->list1F4;

    if (head != 0)
    {
        entry = head;
        while (entry->unk0 != 0)
        {
            if (entry->unk0 == key)
            {
                struct ListEntry *prev;

                release_block(entry->block);
                while (entry->unk0 != 0)
                {
                    prev = entry - 1;
                    entry->unk0 = prev->unk0;
                    entry->block = prev->block;
                    entry = prev;
                }
                return 1;
            }
            entry--;
        }
    }
    return 0;
}

s32 set_free_list_head(void)
{
    struct ListEntry *entry;

    if (gWorld->unkB & 0x10)
    {
        return 1;
    }
    entry = (struct ListEntry *)(gWorkBuf + 0x3660);
    while (entry->unk0 != 0)
    {
        entry--;
    }
    gWorld->list1F4 = entry;
    return 0;
}

void alloc_code_entry(u32 code)
{
    if (code != 0)
    {
        u32 kind = code & 0xF000;

        if (kind != 0x2000 && kind != 0xB000 && kind != 0x4000 && kind != 0x7000)
        {
            lookup_or_alloc_14b8(0, code, -1, 1);
        }
    }
}

void ensure_code_entry(u32 key)
{
    struct ListEntry *entry;

    if (key != 0)
    {
        entry = (struct ListEntry *)(gWorkBuf + 0x3660);
        while (entry->unk0 != 0)
        {
            if (entry->unk0 == key)
            {
                return;
            }
            entry--;
        }
        {
            u32 value = lookup_or_alloc_13b4(0, key, -1, 0);

            if (value != 0)
            {
                if (value <= 0x07FFFFFF)
                {
                    entry->unk0 = key;
                    entry->block = (void *)value;
                }
            }
        }
    }
}

s32 is_state_busy(void)
{
    struct SpriteState *state = gWorld;

    if (state->unkB & 0x10)
    {
        return 1;
    }
    return 0;
}

void sync_obj_impulse(struct MapObj *obj)
{
    u32 target = *(u32 *)&obj->targetX;

    if (target != *(u32 *)&obj->lastX)
    {
        s32 dx = (s16)(obj->targetX - *(u16 *)&obj->lastX);
        s32 dy = (s16)(obj->targetY - *(u16 *)&obj->lastY);

        map_obj_set_impulse(obj, dx, dy);
        obj->lastX = obj->targetX;
        obj->lastY = obj->targetY;
    }
}

struct ScreenEntry
{
    u16 x;
    u16 y;
    u8 filler4[0xE - 4];
    u16 unkE;
};

struct ScreenCamera
{
    u8 filler0[0x774];
    u16 unk774;
    u16 unk776;
};

void write_screen_entry(struct ScreenCamera *camera, struct SpriteObj *obj)
{
    struct ScreenEntry *entry;
    s32 x;
    s32 y;
    s32 z;
    s32 v;
    s32 base;
    s32 yz;

    entry = (struct ScreenEntry *)obj->record;
    if (entry == 0)
    {
        return;
    }
    x = obj->unk38 / 0x100;
    y = obj->unk3C / 0x100;
    z = obj->unk40 / 0x100;
    yz = y - z;
    entry->x = x - camera->unk774;
    entry->y = yz - camera->unk776;
    if ((obj->flags79 & 4) == 0)
    {
        base = 0x1F0;
        v = (s8)obj->unk74 + base - y + (s8)obj->unk75;
        if (v < 0)
        {
            entry->unkE = 0;
        }
        else
        {
            entry->unkE = v;
        }
    }
}

void offset_pair_by_camera(u8 *camera, s32 *a, s32 *b)
{
    *a += *(s16 *)(camera + 0x774);
    *b += *(s16 *)(camera + 0x776);
}

void offset_words_by_camera(u8 *camera, s32 *a, s32 *b, s32 *d)
{
    *a -= *(s16 *)(camera + 0x774);
    *b -= *(s16 *)(camera + 0x776);
    *d += 0x1F0;
}

void offset_three_words(u8 *camera, s32 *a, s32 *b, s32 *d)
{
    s32 av = *a;
    s32 bOld = *b;
    s32 bv = bOld - *d;

    av -= *(s16 *)(camera + 0x774);
    *a = av;
    bv -= *(s16 *)(camera + 0x776);
    *b = bv;
    *d = 0x1F0 - bOld;
}

void write_entry_offsets_fixed(u8 *camera, u16 *out, s32 x, s32 y, s32 z)
{
    out[0] = x - *(u16 *)(camera + 0x774);
    out[1] = y - *(u16 *)(camera + 0x776);
    out[7] = z + 0x1F0;
}

void write_entry_offsets(u8 *camera, u16 *entry, s32 x, s32 y, s32 z)
{
    s32 yv = y - z;

    entry[0] = x - *(u16 *)(camera + 0x774);
    entry[1] = yv - *(u16 *)(camera + 0x776);
    entry[7] = 0x1F0 - y;
}

void add_obj_target_offset(struct MapObj *obj, s32 dx, s32 dy)
{
    obj->targetX += dx;
    obj->targetY += dy;
}

void set_obj_target(struct MapObj *obj, s32 x, s32 y)
{
    obj->targetX = x;
    obj->targetY = y;
}

s32 obj_last_y(struct MapObj *obj)
{
    return obj->lastY;
}

s32 obj_last_x(struct MapObj *obj)
{
    return obj->lastX;
}

void destroy_map_obj(struct MapObj *obj, u32 param)
{
    obj->descriptor = (u32)gMapObjDescriptor;
    map_loader_map_obj_destroy(obj, param);
}
