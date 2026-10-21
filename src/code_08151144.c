#include "global.h"
#include "ram_routines.h"

struct Bit4Bits
{
    u8 low4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bit2Bits
{
    u8 low2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Bit5Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

struct SeqBits
{
    u16 low7 : 7;
    u16 mid2 : 2;
    u16 rest : 7;
};

struct Cell
{
    u8 filler0[0x2C];
    s16 v2C;
};

struct SceneState
{
    u8 filler0[0x14];
    s32 x14;
    void *sceneTable;
    u8 filler1C[0x1CB4 - 0x1C];
    void *iris;
    u8 filler1CB8[0x1F30 - 0x1CB8];
    void *gameTask;
};

struct Bit3Bits
{
    u8 low3 : 3;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct Lo3Bits
{
    u8 low3 : 3;
    u8 rest : 5;
};

struct Obj
{
    void *rows;
    s32 posX;
    s32 posY;
    s32 x;
    s32 y;
    s32 z;
    union
    {
        u16 vy;
        s32 dz;
    } field18;
    u16 count;
    u8 filler1E[0x20 - 0x1E];
    u16 unk20;
    u8 filler22[0x24 - 0x22];
    u8 unk24;
    u8 filler25[0x28 - 0x25];
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
    u8 filler44[0x100 - 0x44];
    u16 tbl[0x18];
    u8 filler130[0x198 - 0x130];
    void *handlerA;
    u8 filler19C[0x1A0 - 0x19C];
    void *handlerB;
    void *handlerC;
    u8 filler1A8[0x1B0 - 0x1A8];
    void *record;
    u8 filler1B4[0x1BE - 0x1B4];
    s16 kind;
    u8 filler1C0[0x1D0 - 0x1C0];
    u16 entryIndex;
    u16 timerB;
    u8 filler1D4[0x1DC - 0x1D4];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x200 - 0x1EC];
    s32 tgtA;
    u8 filler204[0x20D - 0x204];
    u8 flags20D;
    u8 filler20E[0x212 - 0x20E];
    u16 seq;
    u8 flags214;
    u8 filler215;
    u8 unk216;
    u8 filler217[0x242 - 0x217];
    s16 speedX;
    s16 speedY;
    u8 filler246[0x248 - 0x246];
    s32 jumpCount;
    u8 filler24C[0x283 - 0x24C];
    u8 width283;
    u8 filler284[0x294 - 0x284];
    void *cell294;
    u8 filler298[0x2B4 - 0x298];
    u8 unk2B4;
    u8 filler2B5[0x2C8 - 0x2B5];
    s32 jumpInit;
    u8 filler2CC[0x33E - 0x2CC];
    u8 flags33E;
    u8 filler33F[0x344 - 0x33F];
    void *linked;
};


u32 read_var(u16 id);
void write_var(u16 id, u16 value);
u8 game_var_read(s32 index);
extern u8 gGameMode;
void *scene_init_display(void *obj, u32 kind, const void *table);
void run_pending_obj_callbacks(void);
extern u8 *gUnk_03000FC0;
void obj_set_active(struct Obj *obj, u32 on);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void obj_set_cell_coord(struct Obj *obj, s32 cell, u32 zero);
extern const u8 gFrameAttrTable[];
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
void enter_zone_move(void);
void enter_zone_entry(void);
void obj_jump_step(s32 unused, struct Obj *obj, void **slot);
void obj_air_step(s32 unused, struct Obj *obj, void **slot);
void zone_timer_step(void *parent, struct Obj *obj, void **slot);
void zone_start_move(void *parent, struct Obj *obj, void **slot);
u8 obj_target_in_reach(struct Obj *obj, s32 target);
void obj_apply_speed_xy(s32 unused, struct Obj *obj);
void load_anim_tables_plain(struct Obj *obj);
void obj_set_motion_a_08046AA4(void *obj, u32 a, u32 b, u32 c, u32 d);
void obj_advance_axes(void *obj);
void obj_clear_flags_2b5_30(void *obj);
void run_state_handler(void);
void zone_entry_handler(void);
void obj_load_record(void *spr, u32 record, u32 loadFlags, u32 arg3, u32 arg4);
void follow_entry_axis(void);
void spawn_random_offset(void *obj, u32 flag);
void zone_pick_handler(void *parent, struct Obj *obj, void **out);
void entry_switch_handler(void);
void obj_load_kind_record(s32 unused, struct Obj *obj, void **slot);
u8 find_active_entry(void *owner, u8 *idx);
u8 entry_within_reach(struct Obj *obj, void *other);
void spawn_small_handler(s32 unused0, s32 unused1, void **slot);
void spawn_big_handler(s32 unused0, s32 unused1, void **slot);
void obj_free_tiles(void *p, u32 n);
void set_irq_callback(u32 id, void *cb);
void slot_reset(u32 value);
void dma_cancel_request(void *src);
void heap_free(void *p);
void task_remove(void *task, u32 id);
void set_flags_2_3(u32 value);
void scene_release_objects(struct SceneState *state);
extern const u8 gSceneTable08cdde80[];
extern u16 gUnk_02000004;
extern u8 gSys[];
extern u8 *gUnk_03001014;
extern u16 gUnk_02000000;

void obj_halt(struct Obj *obj, u8 flag)
{
    ((struct SeqBits *)&obj->seq)->mid2 = 0;
    obj->unk2B4 = 0;
    if (flag == 0)
    {
        obj->handlerB = (void *)enter_zone_move;
    }
    else
    {
        obj->handlerB = (void *)enter_zone_entry;
    }
}

NONMATCH("asm/code_08151144/edge_facing_ok.s", u32 edge_facing_ok(struct Obj *obj, s32 x, s32 y, u8 dir, u8 s0, u8 s1))
{
    s32 edge;

    edge = obj->width283 << 8;
    if (x <= edge)
    {
        if (y <= edge)
        {
            if ((((u32)obj->unk24 << 29) >> 29) == dir)
            {
                return 1;
            }
            if ((((u32)obj->unk24 << 29) >> 29) == s0)
            {
                return 1;
            }
            if ((((u32)obj->unk24 << 29) >> 29) != s1)
            {
                return 0;
            }
            return 1;
        }
        if ((((u32)obj->unk24 << 29) >> 29) == s0)
        {
            return 1;
        }
        return 0;
    }
    if (y <= edge)
    {
        if ((((u32)obj->unk24 << 29) >> 29) == s1)
        {
            return 1;
        }
        return 0;
    }
    if ((((u32)obj->unk24 << 29) >> 29) == dir)
    {
        return 1;
    }
    return 0;
}
END_NONMATCH

void obj_random_cell(struct Obj *obj, s32 zero)
{
    u8 kind;
    u16 *tbl;
    u8 *cell;
    u16 start;
    s32 idx;

    kind = (u8)zero;
    tbl = obj->tbl;
    idx = rand_below(4, mt19937_next());
    cell = (u8 *)obj->cell294;
    start = tbl[idx + 4];
    cell[0x20] = start;
    ((struct Lo3Bits *)&obj->flags214)->low3 = 1;
    obj_set_cell_coord(obj, kind + gFrameAttrTable[((((u32)obj->unk216 << 28) >> 30) << 4) + (((u32)obj->unk24 << 29) >> 29) + 8], 0);
}

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

void obj_reset_home(void *parent, struct Obj *obj)
{
    ((struct Bit4Bits *)&obj->flags33E)->bit4 = 0;
    load_anim_tables_plain(obj);
    obj_set_motion_a_08046AA4(obj, 2, 0, 0x10, 1);
    obj_advance_axes(obj);
    obj_clear_flags_2b5_30(obj);
    obj_set_sprite_link(obj, 0);
    obj->homeX = obj->x;
    obj->homeY = obj->y;
    obj->homeZ = obj->z;
    obj->homeDZ = obj->field18.dz;
    obj->entryIndex = ((u16 *)obj->record)[0x18 / 2];
    obj->handlerA = (void *)run_state_handler;
    obj->handlerB = (void *)zone_entry_handler;
}

void obj_deactivate(struct Obj *obj)
{
    obj_set_active(obj, 0);
    obj_set_sprite_link(obj, 0);
}

void obj_load_kind_record(s32 unused, struct Obj *obj, void **slot)
{
    s32 kind;

    kind = obj->kind;
    switch (kind)
    {
    case 0:
        obj_load_record(obj->cell294, 6, -1, 0, kind);
        break;
    case 1:
        obj_load_record(obj->cell294, 7, -1, 0, 0);
        break;
    }
    ((struct Lo3Bits *)&obj->flags214)->low3 = 1;
    slot[0] = follow_entry_axis;
}

void obj_roll_spawn(s32 unused, struct Obj *obj, void **slot)
{
    struct Obj *linked = obj->linked;
    u32 seed;

    if (((struct Cell *)obj->cell294)->v2C > 0xDF)
    {
        ((struct Lo3Bits *)&obj->flags214)->low3 = 0;
        ((struct Bit2Bits *)&obj->flags20D)->bit2 = 0;
        ((struct Bit5Bits *)&linked->unk216)->bit5 = 0;
        seed = mt19937_next();
        if (rand_below(1000, seed) > 0xF9)
        {
            spawn_random_offset(linked, 1);
            linked->handlerB = (void *)spawn_big_handler;
        }
        else
        {
            spawn_random_offset(linked, 0);
            linked->handlerB = (void *)spawn_small_handler;
        }
        *slot = zone_pick_handler;
    }
}

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

void zone_pick_handler(void *parent, struct Obj *obj, void **out)
{
    u8 idx;
    void *handler;

    if (obj->linked == 0)
    {
        handler = entry_switch_handler;
    }
    else
    {
        find_active_entry(parent, &idx);
        if (!entry_within_reach(obj, ((void **)parent)[idx + 10]))
        {
            idx = idx == 0;
            if (!entry_within_reach(obj, ((void **)parent)[idx + 10]))
                return;
        }
        handler = obj_load_kind_record;
    }
    *out = handler;
}

void obj_jump_step(s32 unused, struct Obj *obj, void **slot)
{
    obj->field18.dz -= obj->jumpCount;
    if (obj->field18.dz <= 0)
    {
        obj->field18.dz = 0;
        ((struct Bit3Bits *)&obj->flags20D)->bit3 = 0;
        slot[0] = (void *)obj_air_step;
    }
    else
        obj->jumpCount += 0x20;
}

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

ASM_FUNC("asm/code_08151144/obj_init_table.s", u32 obj_init_table(void))

ASM_FUNC("asm/code_08151144/cam_update.s", void cam_update(void))

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);

struct CamXY
{
    s32 x;
    s32 y;
};

void obj_place_on_screen(struct Obj *obj, struct CamXY *cam)
{
    s32 sx;
    s32 sy;
    s32 camY;
    s32 v;
    s32 dz;
    s32 flagBit;

    sx = obj->x + cam->x;
    sy = obj->y + cam->y;
    if (sx > 0xF000)
    {
        obj->x = 0xF000 - cam->x;
    }
    else if (sx < 0)
    {
        obj->x = -cam->x;
    }
    if (sy > 0xA000)
    {
        v = cam->y;
        obj->y = 0xA000 - v;
        camY = v;
    }
    else
    {
        camY = cam->y;
        if (sy < 0)
        {
            obj->y = -camY;
        }
    }
    flagBit = (s16)(obj->flags & 0x20);
    if (flagBit == 0)
    {
        if (*(s16 *)&obj->count > 0)
        {
            obj->count += 0xC;
            if (*(s16 *)&obj->count > 0xFF)
            {
                obj->count = 0;
            }
            v = gCosineTable[(u8)obj->count];
            if (v < 0)
            {
                v += 0x3F;
            }
            v >>= 6;
            dz = (v << 1) + v;
            obj->unk20 = dz;
            obj->field18.vy = dz;
            obj->x += *(s16 *)&obj->z;
            obj->y += *(s16 *)&obj->field18.vy;
        }
    }
    obj->posX = -(cam->x + obj->x);
    obj->posY = -(camY + obj->y);
    obj_build_iris_rows(obj, (s32 *)cam);
    dma_copy_or_queue(obj->rows, gUnk_03001014 + 4, 0x1C8, 0x10, 1);
}

ASM_FUNC("asm/code_08151144/obj_build_iris_rows.s", void obj_build_iris_rows(struct Obj *obj, s32 *pos))

NONMATCH("asm/code_08151144/obj_set_flag_08.s", void obj_set_flag_08(struct Obj *obj, s32 on))
{
    s32 m;

    if (on == 0)
    {
        s32 f = obj->flags;
        m = -9 & f;
    }
    else
    {
        s32 f = obj->flags;
        m = 8 | f;
    }
    obj->flags = m;
}
END_NONMATCH

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

void obj_clear_flag_01(struct Obj *obj)
{
    s32 f;
    s32 m;

    f = obj->flags;
    m = -2;
    m &= f;
    obj->flags = m;
}

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

NONMATCH("asm/code_08151144/obj_point_in_iris.s", u32 obj_point_in_iris(struct Obj *obj, s32 *target, s32 *anchor))
{
    s32 ax;
    s32 ay;
    s32 dx;
    s32 dy;
    s32 scale;
    s32 limit;
    s32 dist;

    ax = anchor[0] + obj->x;
    ay = anchor[1] + obj->y;
    dx = target[0] - ax;
    dy = target[1] - ay;
    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
    scale = obj->base / 0x100;
    limit = scale * obj->field30.radius;
    limit = (limit << 1) + limit;
    if (limit < 0)
    {
        limit += 3;
    }
    if (dist < (limit >> 2))
    {
        return 1;
    }
    return 0;
}
END_NONMATCH

void obj_set_iris(struct Obj *obj, s32 *target, s32 *anchor, s32 radius)
{
    struct XY { s32 x, y; } *off;

    obj->field30.radius = radius;
    off = (struct XY *)&obj->x;
    *off = *(struct XY *)anchor;
    obj->x = target[0] - obj->x;
    obj->y = target[1] - obj->y;
    obj_build_iris_rows(obj, anchor);
    obj_set_flag_01(obj);
    obj_set_flag_08(obj, 1);
}

void *obj_ofs_addr(struct Obj *obj)
{
    return &obj->x;
}

void obj_apply_flag_offsets(struct Obj *obj)
{
    u8 *base = gSys;

    if (*(u16 *)(base + 0x28) & 0x10)
        obj->x += 0x400;
    if (*(u16 *)(base + 0x28) & 0x20)
        obj->x -= 0x400;
    if (*(u16 *)(base + 0x28) & 0x80)
        obj->y += 0x400;
    if (*(u16 *)(base + 0x28) & 0x40)
        obj->y -= 0x400;
}

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

extern u8 gGameMode;
extern u8 *gGamePtr;
s32 obj_get_value(void *obj);
void game_var_write(s32 index, u8 value);
void sync_script_vars(void)
{
    u16 var;

    if (((s32)((u32)gGameMode << 27) >> 27) > 5)
    {
        write_var(0x1CD9, 6);
    }
    else
    {
        write_var(0x1CD9, gGamePtr[0x1F20]);
    }
    var = read_var(0x1CD9);
    if (var > *(s16 *)(gGamePtr + 0x1F24))
    {
        write_var(0x1CDF, var);
    }
    write_var(0x1CDA, obj_get_value(*(void **)(gGamePtr + 0x1CB0)));
    if ((s16)obj_get_value(*(void **)(gGamePtr + 0x1CB0)) > *(s16 *)(gGamePtr + 0x1F26))
    {
        write_var(0x1CE0, obj_get_value(*(void **)(gGamePtr + 0x1CB0)));
    }
    game_var_write(0x1C, gGamePtr[0x1F20]);
}

ASM_FUNC("asm/code_08151144/hblank_iris_window.s", void hblank_iris_window(void))

ASM_FUNC("asm/code_08151144/hitboxes_overlap.s", void hitboxes_overlap(void))

u32 obj_init_table(void);
ASM_FUNC("asm/code_08151144/scene_init_display.s", void *scene_init_display(void *obj, u32 kind, const void *table))

struct SysHalf2
{
    u8 filler0[2];
    u16 v2;
};

void scene_teardown(struct SceneState *state, u32 task)
{
    s32 flagsByte;
    s32 mask;

    state->sceneTable = (void *)gSceneTable08cdde80;
    slot_reset(0);
    set_irq_callback(2, 0);
    REG_IME = 0;
    gUnk_02000004 = gUnk_02000004 & 0xFFEF;
    REG_IE = REG_IE & 0xFFFD;
    REG_IF = REG_IF | 2;
    REG_IME = 1;
    set_irq_callback(4, 0);
    scene_release_objects(state);
    if (state->gameTask != 0)
    {
        task_remove(state->gameTask, 3);
    }
    dma_cancel_request(gUnk_03001014 + 4);
    if (state->iris != 0)
    {
        obj_destroy_iris((struct Obj *)state->iris, 3);
    }
    heap_free(gUnk_03001014);
    ((struct SysHalf2 *)gSys)->v2 = 0xFFFF;
    flagsByte = *(u8 *)(gSys + 0x888);
    mask = -0x21;
    mask &= flagsByte;
    *(u8 *)(gSys + 0x888) = mask;
    set_flags_2_3(state->x14);
    obj_run_callbacks();
    *(u16 *)0x02000000 = 0x40;
    obj_free_tiles((u8 *)state + 0x1064, 2);
    obj_free_tiles((u8 *)state + 0x840, 2);
    obj_free_tiles((u8 *)state + 0x1C, 2);
    task_remove(state, task);
}

ASM_FUNC("asm/code_08151144/scene_build_state.s", void scene_build_state(void))
