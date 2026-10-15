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

struct SpriteObj
{
    u8 filler0[0x28];
    struct SpriteHead *head;
};

struct SpriteHead
{
    u8 filler0[0x111];
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

struct SpriteState
{
    u8 filler0[0xB];
    u8 unkB;
};

extern struct SpriteState *gUnk_03000FD8;
extern const u8 gMapObjDescriptor[];

void map_loader_map_obj_destroy(struct MapObj *obj, u32 param);
void sprite_load_active_tables(void);
void task_state_machine(struct SpriteTask *task);
void task_run_parts(struct SpriteTask *task);
void state_set_flags_and_run(struct SpriteTask *task);
s32 is_state_busy(void);
void set_free_list_head(void);

ASM_FUNC("asm/code_08081DA4/move_slots_toward.s", void move_slots_toward(void))

ASM_FUNC("asm/code_08081DA4/state_show_and_scale.s", void state_show_and_scale(void))

ASM_FUNC("asm/code_08081DA4/state_random_step.s", void state_random_step(void))

ASM_FUNC("asm/code_08081DA4/state_timed_motion.s", void state_timed_motion(void))

ASM_FUNC("asm/code_08081DA4/state_wait_arrival.s", void state_wait_arrival(void))

ASM_FUNC("asm/code_08081DA4/state_draw_pose.s", void state_draw_pose(void))

void state_wait_flag(struct SpriteTask *task)
{
    struct SpriteHead *head = task->obj->head;

    if (!(head->flags111 & 1))
    {
        task->next = state_set_flags_and_run;
    }
}

ASM_FUNC("asm/code_08081DA4/state_start_motion.s", void state_start_motion(void))

ASM_FUNC("asm/code_08081DA4/state_start_wait.s", void state_start_wait(void))

ASM_FUNC("asm/code_08081DA4/state_set_anim_and_draw.s", void state_set_anim_and_draw(void))

ASM_FUNC("asm/code_08081DA4/run_sequence.s", void run_sequence(void))

void noop_callback_0808261C(void)
{
}

ASM_FUNC("asm/code_08081DA4/run_sequence_of_obj.s", void run_sequence_of_obj(void))

ASM_FUNC("asm/code_08081DA4/run_sequence_of_obj_b.s", void run_sequence_of_obj_b(void))

void noop_stub_080826AC(void)
{
}

ASM_FUNC("asm/code_08081DA4/draw_fixed_labels.s", void draw_fixed_labels(void))

ASM_FUNC("asm/code_08081DA4/sprite_load_tables.s", void sprite_load_tables(void))

ASM_FUNC("asm/code_08081DA4/run_slot_script.s", void run_slot_script(void))

ASM_FUNC("asm/code_08081DA4/state_set_flags_and_run.s", void state_set_flags_and_run(struct SpriteTask *task))

ASM_FUNC("asm/code_08081DA4/state_wait_idle.s", void state_wait_idle(struct SpriteTask *task))

ASM_FUNC("asm/code_08081DA4/state_clear_mode_bit.s", void state_clear_mode_bit(void))

ASM_FUNC("asm/code_08081DA4/state_spawn_object.s", void state_spawn_object(void))

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

ASM_FUNC("asm/code_08081DA4/state_start_idle.s", void state_start_idle(void))

ASM_FUNC("asm/code_08081DA4/state_countdown_reset.s", void state_countdown_reset(void))

ASM_FUNC("asm/code_08081DA4/state_countdown_start.s", void state_countdown_start(void))

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

ASM_FUNC("asm/code_08081DA4/state_move_then_check.s", void state_move_then_check(void))

ASM_FUNC("asm/code_08081DA4/state_move_then_image.s", void state_move_then_image(void))

ASM_FUNC("asm/code_08081DA4/state_move_then_check_bit3.s", void state_move_then_check_bit3(void))

ASM_FUNC("asm/code_08081DA4/state_wait_mode_bit1.s", void state_wait_mode_bit1(void))

ASM_FUNC("asm/code_08081DA4/init_sprite_pools.s", void init_sprite_pools(void))

ASM_FUNC("asm/code_08081DA4/refresh_sprite_display.s", void refresh_sprite_display(void))

ASM_FUNC("asm/code_08081DA4/unknown_code_08082f54.s", void unknown_code_08082f54(void))

ASM_FUNC("asm/code_08081DA4/queue_display_entry.s", void queue_display_entry(void))

ASM_FUNC("asm/code_08081DA4/load_table_entries.s", void load_table_entries(void))

ASM_FUNC("asm/code_08081DA4/load_table_entries_typed.s", void load_table_entries_typed(void))

ASM_FUNC("asm/code_08081DA4/clear_list_entries.s", void clear_list_entries(void))

s32 unknown_code_0808320c(void)
{
    return 1;
}

ASM_FUNC("asm/code_08081DA4/sub_08083210.s", void sub_08083210(void))

ASM_FUNC("asm/code_08081DA4/set_free_list_head.s", void set_free_list_head(void))

ASM_FUNC("asm/code_08081DA4/alloc_code_entry.s", void alloc_code_entry(void))

ASM_FUNC("asm/code_08081DA4/ensure_code_entry.s", void ensure_code_entry(void))

s32 is_state_busy(void)
{
    struct SpriteState *state = gUnk_03000FD8;

    if (state->unkB & 0x10)
    {
        return 1;
    }
    return 0;
}

ASM_FUNC("asm/code_08081DA4/sync_obj_impulse.s", void sync_obj_impulse(void))

ASM_FUNC("asm/code_08081DA4/write_screen_entry.s", void write_screen_entry(void))

ASM_FUNC("asm/code_08081DA4/unknown_code_08083430.s", void unknown_code_08083430(void))

ASM_FUNC("asm/code_08081DA4/offset_words_by_camera.s", void offset_words_by_camera(void))

ASM_FUNC("asm/code_08081DA4/offset_three_words.s", void offset_three_words(void))

ASM_FUNC("asm/code_08081DA4/unknown_code_080834d8.s", void unknown_code_080834d8(void))

ASM_FUNC("asm/code_08081DA4/write_entry_offsets.s", void write_entry_offsets(void))

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
