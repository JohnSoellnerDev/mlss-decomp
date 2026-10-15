#include "global.h"

struct SpriteTask;
struct SpriteObj;
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
    s8 unk74;
    s8 unk75;
    u8 filler76[0x79 - 0x76];
    u8 flags79;
};

struct SpriteHead
{
    u8 filler0[0xD8];
    s32 unkD8;
    s32 unkDC;
    u8 filler0E0[0xEC - 0xE0];
    u8 unkEC;
    u8 filler0ED[0xF6 - 0xED];
    s16 timerF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
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

struct SpriteState
{
    u8 *unk0;
    u8 filler4[0xB - 4];
    u8 unkB;
    u8 filler0C[4];
    u8 unk10;
    u8 filler11[0x58 - 0x11];
    struct SpritePart *part58[6];
    u8 filler70[0x1F4 - 0x70];
    struct ListEntry *list1F4;
    u8 filler1F8[0x244 - 0x1F8];
    void *unk244;
    u8 filler248[0x2BE - 0x248];
    u8 unk2BE;
    u8 unk2BF;
    u8 unk2C0;
    u8 filler2C1[0x2DC - 0x2C1];
    u8 *unk2DC;
    u8 filler2E0[0x349 - 0x2E0];
    u8 unk349;
};

extern struct SpriteState *gUnk_03000FD8;
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
void run_obj_entry(u32 arg);
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
void state_timed_motion(struct SpriteTask *task);
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
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct SpriteObj *owner);
u8 *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(u8 *obj, u32 a, s32 b, u32 c, u32 flag);
void task_free_blocks(struct SpriteTask *task);
u16 *load_table_value(s32 *count, u16 code);
extern u16 gEffectCodeTable[];
extern u16 gSpriteLoadList[];
extern u8 *gWorkBuf;
extern u8 gUnk_0300034C[];

ASM_FUNC("asm/code_08081DA4/move_slots_toward.s", void move_slots_toward(struct SpriteObj *obj))

ASM_FUNC("asm/code_08081DA4/state_show_and_scale.s", void state_show_and_scale(void))

ASM_FUNC("asm/code_08081DA4/state_random_step.s", void state_random_step(void))

ASM_FUNC("asm/code_08081DA4/state_timed_motion.s", void state_timed_motion(struct SpriteTask *task))

ASM_FUNC("asm/code_08081DA4/state_wait_arrival.s", void state_wait_arrival(void))

void state_draw_pose(struct SpriteTask *task)
{
    struct SpriteObj *obj = task->obj;
    s32 code;
    s32 flag;

    anim_bit200_step(obj->head, 0xC8);
    flag = gUnk_03000FD8->unk2BE & 1;
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
    gUnk_03000FD8->unk10 = 4;
    {
        s32 flags = gUnk_0300034C[0x888];

        flags |= 0x10;
        gUnk_0300034C[0x888] = flags;
    }
    task->next = state_timed_motion;
}

void state_start_wait(struct SpriteTask *task)
{
    run_obj_entry((u32)task);
    task_create_list_4c(owner_wait_entry, 0);
    gUnk_03000FD8->unk10 = 4;
    {
        s32 flags = gUnk_0300034C[0x888];

        flags |= 0x10;
        gUnk_0300034C[0x888] = flags;
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

ASM_FUNC("asm/code_08081DA4/sprite_load_tables.s", void sprite_load_tables(void))

void run_slot_script(s32 index)
{
    struct SpritePart *part;

    if (gUnk_03000FD8->list1F4 == 0)
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
    part = gUnk_03000FD8->part58[index];
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
    set_flags_2_3(gUnk_03000FD8->unk244);
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
    if (gUnk_03000FD8->unk2BF & 0x20)
    {
        ((struct SpriteModeBits *)&gUnk_03000FD8->unk2BF)->bit5 = 0;
        set_flag_2_2(gUnk_03000FD8->unk244);
        task->next = state_wait_idle;
        task->next(task);
    }
}

void state_spawn_object(struct SpriteTask *task)
{
    u32 busy = (u8)is_state_busy();

    if (busy == 0)
    {
        u8 *obj = gUnk_03000FD8->unk0;

        release_block(*(void **)(*(u8 **)(obj + 0x48) + 8));
        obj = create_obj_de4(0, 0x2091, 0, -1, -1, -1, -1);
        obj_load_record(obj, 0, -1, 0, busy);
        gUnk_03000FD8->unk0 = obj;
        task->next = task_free_blocks;
    }
}

ASM_FUNC("asm/code_08081DA4/state_fill_table_and_spawn.s", void state_fill_table_and_spawn(void))

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
    gUnk_03000FD8->unk10 = 4;
    {
        s32 flags = gUnk_0300034C[0x888];

        flags |= 0x10;
        gUnk_0300034C[0x888] = flags;
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

ASM_FUNC("asm/code_08081DA4/state_countdown_start.s", void state_countdown_start(struct SpriteTask *task))

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
    else if (gUnk_03000FD8->unkB & 2)
    {
        ((struct SpriteStateBits *)&gUnk_03000FD8->unkB)->bit1 = 0;
    }
    else
    {
        task->next = state_set_flags_and_run;
    }
}

ASM_FUNC("asm/code_08081DA4/init_sprite_pools.s", void init_sprite_pools(void))

ASM_FUNC("asm/code_08081DA4/refresh_sprite_display.s", void refresh_sprite_display(void))

ASM_FUNC("asm/code_08081DA4/unknown_code_08082f54.s", void unknown_code_08082f54(void))

ASM_FUNC("asm/code_08081DA4/queue_display_entry.s", void queue_display_entry(void))

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
    struct ListEntry *entry = gUnk_03000FD8->list1F4;

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

s32 sub_08083210(u32 key)
{
    struct ListEntry *entry;
    struct ListEntry *head = gUnk_03000FD8->list1F4;

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

    if (gUnk_03000FD8->unkB & 0x10)
    {
        return 1;
    }
    entry = (struct ListEntry *)(gWorkBuf + 0x3660);
    while (entry->unk0 != 0)
    {
        entry--;
    }
    gUnk_03000FD8->list1F4 = entry;
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
    struct SpriteState *state = gUnk_03000FD8;

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

ASM_FUNC("asm/code_08081DA4/write_screen_entry.s", void write_screen_entry(void))

void unknown_code_08083430(u8 *camera, s32 *a, s32 *b)
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

void unknown_code_080834d8(u8 *camera, u16 *out, s32 x, s32 y, s32 z)
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
