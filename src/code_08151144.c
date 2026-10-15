#include "global.h"

struct Obj
{
    void *rows;
    s32 posX;
    s32 posY;
    s32 x;
    s32 y;
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    union
    {
        u16 vy;
        s32 dz;
    } field18;
    u16 count;
    u8 filler1E[0x20 - 0x1E];
    u16 unk20;
    u8 filler22[0x24 - 0x22];
    u16 unk24;
    u8 filler26[0x28 - 0x26];
    union
    {
        void *sub28;
        struct
        {
            u8 halfW;
            u8 halfH;
        } half;
    } field28;
    u16 flags;
    u8 filler2E[0x30 - 0x2E];
    union
    {
        void *behavior;
        s32 radius;
    } field30;
    s32 base;
    s32 value;
    s32 steps;
    s32 unk40;
    u8 filler44[0x198 - 0x44];
    void *handlerA;
    u8 filler19C[0x1A0 - 0x19C];
    void *handlerB;
    void *handlerC;
    u8 filler1A8[0x1BE - 0x1A8];
    s16 kind;
    u8 filler1C0[0x1D0 - 0x1C0];
    u16 entryIndex;
    u16 timerB;
    u8 filler1D4[0x200 - 0x1D4];
    s32 tgtA;
    u8 filler204[0x20D - 0x204];
    u8 flags20D;
    u8 filler20E[0x212 - 0x20E];
    u16 seq;
    u8 flags214;
    u8 filler215[0x242 - 0x215];
    s16 speedX;
    s16 speedY;
    u8 filler246[0x248 - 0x246];
    s32 jumpCount;
    u8 filler24C[0x294 - 0x24C];
    void *cell294;
    u8 filler298[0x2B4 - 0x298];
    u8 unk2B4;
    u8 filler2B5[0x2C8 - 0x2B5];
    s32 jumpInit;
    u8 filler2CC[0x344 - 0x2CC];
    void *linked;
};


u32 read_var(u16 id);
void write_var(u16 id, u16 value);
void run_pending_obj_callbacks(void);
extern u8 *gUnk_03000FC0;
void obj_set_active(struct Obj *obj, u32 on);
void obj_set_sprite_link(struct Obj *obj, u32 link);
void obj_build_iris_rows(struct Obj *obj, s32 *pos);
void obj_set_flag_08(struct Obj *obj, s32 on);
void heap_free_a(void *p);
void heap_free_b(void *p);
void release_obj_131c(void *p);
void obj_random_cell(struct Obj *obj, s32 zero);
void obj_halt(struct Obj *obj, u8 flag);
s32 zone_point_code(void *parent, struct Obj *obj, s32 zero);
void zone_move_handler(void);
void obj_jump_step(s32 unused, struct Obj *obj, void **slot);
void obj_air_step(s32 unused, struct Obj *obj, void **slot);
void zone_timer_step(void *parent, struct Obj *obj, void **slot);
void zone_start_move(void *parent, struct Obj *obj, void **slot);
u8 obj_target_in_reach(struct Obj *obj, s32 target);
void obj_apply_speed_xy(s32 unused, struct Obj *obj);

ASM_FUNC("asm/code_08151144/obj_halt.s", void obj_halt(struct Obj *obj, u8 flag))

ASM_FUNC("asm/code_08151144/edge_facing_ok.s", void edge_facing_ok(void))

ASM_FUNC("asm/code_08151144/obj_random_cell.s", void obj_random_cell(struct Obj *obj, s32 zero))

void zone_target_check(struct Obj *parent, struct Obj *obj, void **slot)
{
    if ((s8)zone_point_code(parent, obj, 0) == -1)
    {
        obj_halt(obj, 1);
    }
    else if (obj_target_in_reach(obj, parent->tgtA) == 0)
    {
        obj->timerB = obj->entryIndex;
        slot[0] = zone_timer_step;
        slot[3] = zone_start_move;
    }
}

ASM_FUNC("asm/code_08151144/obj_reset_home.s", void obj_reset_home(void))

void obj_deactivate(struct Obj *obj)
{
    obj_set_active(obj, 0);
    obj_set_sprite_link(obj, 0);
}

ASM_FUNC("asm/code_08151144/obj_load_kind_record.s", void obj_load_kind_record(s32 unused, struct Obj *obj, void **slot))

ASM_FUNC("asm/code_08151144/obj_roll_spawn.s", void obj_roll_spawn(void))

void zone_start_move(void *parent, struct Obj *obj, void **slot)
{
    void (*handler)();

    obj_random_cell(obj, 0);
    handler = zone_move_handler;
    slot[3] = handler;
    handler(parent, obj, slot);
}

void zone_timer_step(void *parent, struct Obj *obj, void **slot)
{
    if ((s8)zone_point_code(parent, obj, 0) == -1)
    {
        obj_halt(obj, 1);
    }
    else
    {
        obj->timerB--;
        if (obj->timerB == 0)
        {
            slot[0] = slot[3];
        }
    }
}

void spawn_small_handler(s32 unused0, s32 unused1, void **slot)
{
    *slot = obj_jump_step;
}

void spawn_big_handler(s32 unused0, s32 unused1, void **slot)
{
    *slot = obj_apply_speed_xy;
}

ASM_FUNC("asm/code_08151144/zone_pick_handler.s", void zone_pick_handler(void))

ASM_FUNC("asm/code_08151144/obj_jump_step.s", void obj_jump_step(s32 unused, struct Obj *obj, void **slot))

void obj_apply_speed_xy(s32 unused, struct Obj *obj)
{
    obj->x += obj->speedX;
    obj->y += obj->speedY;
}

void obj_air_step(s32 unused, struct Obj *obj, void **slot)
{
    if (obj->field18.dz > 255)
    {
        obj->jumpCount = obj->jumpInit;
        slot[0] = obj_jump_step;
    }
    else
    {
        obj->x += obj->speedX;
        obj->y += obj->speedY;
    }
}

ASM_FUNC("asm/code_08151144/scene_setup.s", void scene_setup(void))

void store_byte_base_53c(s32 index, u8 value)
{
    u8 *base = gUnk_03000FC0 + 0x53C;

    base[index] = value;
}

u8 game_var_read(s32 index)
{
    return read_var(index + 0x1B90);
}

void game_var_write(s32 index, u8 value)
{
    write_var(index + 0x1B90, value);
}

void obj_run_callbacks(void)
{
    run_pending_obj_callbacks();
}

ASM_FUNC("asm/code_08151144/obj_init_table.s", void obj_init_table(void))

ASM_FUNC("asm/code_08151144/cam_update.s", void cam_update(void))

ASM_FUNC("asm/code_08151144/obj_place_on_screen.s", void obj_place_on_screen(void))

ASM_FUNC("asm/code_08151144/obj_build_iris_rows.s", void obj_build_iris_rows(struct Obj *obj, s32 *pos))

ASM_FUNC("asm/code_08151144/obj_set_flag_08.s", void obj_set_flag_08(struct Obj *obj, s32 on))

void obj_set_behavior_table(struct Obj *obj, void *table)
{
    obj->field30.behavior = table;
}

s32 obj_get_base(struct Obj *obj)
{
    return obj->base;
}

void obj_set_value_steps(struct Obj *obj, s32 value)
{
    obj->flags |= 4;
    obj->value = value;
    obj->steps = (value - obj->base) / 10;
}

void call_zeroed_arg_block(struct Obj *obj)
{
    s32 args[2];

    args[1] = 0;
    args[0] = 0;
    obj_build_iris_rows(obj, args);
}

ASM_FUNC("asm/code_08151144/obj_clear_flag_01.s", void obj_clear_flag_01(void))

void obj_set_flag_01(struct Obj *obj)
{
    obj->flags |= 1;
}

void obj_set_vy_if_free(struct Obj *obj, s32 vy)
{
    if ((obj->flags & 0x20) == 0)
    {
        obj->count = 1;
        obj->field18.vy = vy;
    }
}

ASM_FUNC("asm/code_08151144/obj_point_in_iris.s", void obj_point_in_iris(void))

ASM_FUNC("asm/code_08151144/obj_set_iris.s", void obj_set_iris(struct Obj *obj, s32 *target, s32 *anchor, s32 radius))

void *obj_ofs_addr(struct Obj *obj)
{
    return &obj->x;
}

ASM_FUNC("asm/code_08151144/obj_apply_flag_offsets.s", void obj_apply_flag_offsets(void))

void obj_destroy_iris(struct Obj *obj, u32 freeObj)
{
    if (obj->field28.sub28 != 0)
    {
        release_obj_131c(obj->field28.sub28);
    }
    if (obj->rows != 0)
    {
        heap_free_a(obj->rows);
    }
    if ((freeObj & 1) != 0)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_08151144/sync_script_vars.s", void sync_script_vars(void))

ASM_FUNC("asm/code_08151144/hblank_iris_window.s", void hblank_iris_window(void))

ASM_FUNC("asm/code_08151144/hitboxes_overlap.s", void hitboxes_overlap(void))

ASM_FUNC("asm/code_08151144/scene_init_display.s", void scene_init_display(void))

ASM_FUNC("asm/code_08151144/scene_teardown.s", void scene_teardown(void))

ASM_FUNC("asm/code_08151144/scene_build_state.s", void scene_build_state(void))
