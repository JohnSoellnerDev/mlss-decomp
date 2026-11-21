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
    u16 entryIndex; /* frames to wait before each move (record +0x18), reloads timerB */
    u16 timerB;     /* counts the wait down (zone_timer_step) */
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

struct GameModeFlags
{
    u32 mode : 5;
    u32 bit5 : 1;
    u32 bit6 : 1;
    u32 sub7 : 3;
    u32 sub10 : 5;
    u32 bit15 : 1;
};

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
void init_obj_09a8(u32 a, u32 b, u32 c, u32 d);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void scene_init_kind2(void *obj, u32 kind, const void *table);
void obj_scene_init(void *obj, u32 kind, const void *table);
void scene_build_hud(void *obj, u32 kind, const void *table);
void obj_init_screen(void *obj, u32 kind, const void *table);
extern const u8 gHeapTagSceneKind0[];
extern const u8 gHeapTagSceneKind1[];
extern const u8 gHeapTagSceneKind2[];
extern const u8 gHeapTagSceneKind3[];
extern const u8 gHeapTagSceneKind4[];

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

/* Starts the zone AI: plain animation tables, motion axis A eased to 0x10 at speed 1, sprite
 * unlinked, the current position kept as home, the wait between moves loaded from the record,
 * and zone_entry_handler installed as the state handler. */
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

/* Once obj's cell value v2C passes 0xDF, clears obj's and its linked object's flags and starts the
 * linked one at a random offset: 3 times in 4 (roll above 249 of 1000) with flag 1 and
 * spawn_big_handler, else with flag 0 and spawn_small_handler. obj goes on to zone_pick_handler. */
void obj_roll_spawn(s32 unused, struct Obj *obj, void **slot)
{
    struct Obj *linked = obj->linked;
    u32 rnd;

    if (((struct Cell *)obj->cell294)->v2C > 0xDF)
    {
        ((struct Lo3Bits *)&obj->flags214)->low3 = 0;
        ((struct Bit2Bits *)&obj->flags20D)->bit2 = 0;
        ((struct Bit5Bits *)&linked->unk216)->bit5 = 0;
        rnd = mt19937_next();
        if (rand_below(1000, rnd) > 249)
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

/*
 * Starts scene `scene` (0-4) once the previous one is unloaded (unload_scene): resets the sprite
 * heap (init_obj_09a8), allocates the scene's state and runs its init. Scene 0 also sets
 * gGameMode: its mode is game var 0x1E, and when that's nonzero, var 0x1CD9 (1 if that is 0).
 */
void scene_setup(s32 scene)
{
    void *state;

    switch (scene)
    {
    case 0:
        init_obj_09a8(0, 0, 0, 0xBF);
        ((struct GameModeFlags *)&gGameMode)->mode = game_var_read(0x1E);
        if ((gGameMode & 0x1F) != 0)
        {
            if ((u16)read_var(0x1CD9) == 0)
                ((struct GameModeFlags *)&gGameMode)->mode = 1;
            else
                ((struct GameModeFlags *)&gGameMode)->mode = (u16)read_var(0x1CD9);
        }
        ((struct GameModeFlags *)&gGameMode)->bit5 = 1;
        ((struct GameModeFlags *)&gGameMode)->bit6 = 1;
        ((struct GameModeFlags *)&gGameMode)->sub7 = 0;
        ((struct GameModeFlags *)&gGameMode)->sub10 = 8;
        ((struct GameModeFlags *)&gGameMode)->bit15 = 0;
        state = heap_alloc2(0x1F34, 1, gHeapTagSceneKind0, 0);
        scene_init_display(state, 8, gHeapTagSceneKind0);
        break;
    case 1:
        init_obj_09a8(0, 0, 0, 0);
        state = heap_alloc2(0x70, 0, gHeapTagSceneKind1, 0);
        obj_scene_init(state, 8, gHeapTagSceneKind1);
        break;
    case 2:
        init_obj_09a8(0, 0, 0, 0);
        state = heap_alloc2(0x70, 0, gHeapTagSceneKind2, 0);
        scene_init_kind2(state, 8, gHeapTagSceneKind2);
        break;
    case 3:
        init_obj_09a8(0, 0, 0, 0);
        state = heap_alloc2(0x54, 0, gHeapTagSceneKind3, 0);
        scene_build_hud(state, 8, gHeapTagSceneKind3);
        break;
    case 4:
        init_obj_09a8(0, 0, 0, 0);
        state = heap_alloc2(0x494, 0, gHeapTagSceneKind4, 0);
        obj_init_screen(state, 8, gHeapTagSceneKind4);
        break;
    }
}

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

void *heap_alloc(u32 size, u32 pool, const void *tag, u32 flags);
extern const u8 gHeapTagObjTableBuf[];

struct Obj *obj_init_table(struct Obj *obj)
{
    s32 i;
    u16 *buf;
    u16 fill;

    obj->posX = 0;
    obj->posY = 0;
    obj->x = 0;
    obj->y = 0xFFFF9C00;
    *(u16 *)&obj->z = 0;
    obj->field18.vy = 0;
    obj->count = 0;
    obj->unk20 = 0;
    *(u16 *)&obj->unk24 = 0;
    obj->field28.sub28 = 0;
    obj->flags = 0;
    obj->field30.radius = 0x50;
    obj->base = 0x10000;
    obj->unk40 = 0;
    obj->value = 0x10000;
    buf = heap_alloc(0x1C8, 1, gHeapTagObjTableBuf, 0);
    obj->rows = buf;
    fill = 0xEF;
    for (i = 0x9F; i >= 0; i--)
        buf[i] = fill;
    return obj;
}




struct SysKeys28
{
    u8 filler0[0x28];
    u16 keys;
};

extern u16 gUnk_02000052; /* BLDALPHA shadow: EVB << 8 | EVA */

/* Obj.flags of an iris: base (the radius scale) is growing towards value (obj_set_value_steps) */
#define IRIS_FLAG_GROWING 4

/* Moves the iris's centre 4 pixels a frame with the held D-pad, and while flags 1 and 8 are set
 * grows its radius scale (base) towards value, or shrinks it by 0x20 down to 0, and sets the
 * blend's EVB from it (EVA 8). */
void cam_update(struct Obj *cam)
{
    u8 *sys = gSys;
    s32 evb;

    if (((struct SysKeys28 *)sys)->keys & DPAD_RIGHT)
        cam->x += 0x400;
    if (((struct SysKeys28 *)sys)->keys & DPAD_LEFT)
        cam->x -= 0x400;
    if (((struct SysKeys28 *)sys)->keys & DPAD_DOWN)
        cam->y += 0x400;
    if (((struct SysKeys28 *)sys)->keys & DPAD_UP)
        cam->y -= 0x400;
    if ((*(s16 *)&cam->flags & 1) && (*(s16 *)&cam->flags & 8))
    {
        if ((s16)(*(s16 *)&cam->flags & IRIS_FLAG_GROWING))
        {
            cam->base += cam->steps;
            if (cam->base >= cam->value)
            {
                cam->base = cam->value;
                *(s16 *)&cam->flags &= ~IRIS_FLAG_GROWING;
            }
        }
        else
        {
            cam->base -= 0x20;
            if (cam->base < 0)
                cam->base = 0;
        }
        evb = cam->base / 0x1000 + 1;
        gUnk_02000052 = evb << 8 | 8;
    }
}





void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);

struct CamXY
{
    s32 x;
    s32 y;
};

/* Keeps the iris circle's centre (camera + offset, 8.8 fixed point) inside the 240x160 screen,
 * runs the wobble obj_set_vy_if_free started (count steps 12 per frame through a cosine until it
 * passes 0xFF), then rebuilds the window's scanline rows and queues their copy into the HBlank
 * buffer at gUnk_03001014. */
void obj_place_on_screen(struct Obj *obj, struct CamXY *cam)
{
    s32 screenX;
    s32 screenY;
    s32 camY;
    s32 v;
    s32 wobble;
    s32 frozen;

    screenX = obj->x + cam->x;
    screenY = obj->y + cam->y;
    if (screenX > 240 << 8)
    {
        obj->x = (240 << 8) - cam->x;
    }
    else if (screenX < 0)
    {
        obj->x = -cam->x;
    }
    if (screenY > 160 << 8)
    {
        v = cam->y;
        obj->y = (160 << 8) - v;
        camY = v;
    }
    else
    {
        camY = cam->y;
        if (screenY < 0)
        {
            obj->y = -camY;
        }
    }
    frozen = (s16)(obj->flags & 0x20);
    if (frozen == 0)
    {
        if (*(s16 *)&obj->count > 0)
        {
            obj->count += 12;
            if (*(s16 *)&obj->count > 0xFF)
            {
                obj->count = 0;
            }
            /* wobble = cos / 64 * 3, rounding towards zero */
            v = gCosineTable[(u8)obj->count];
            if (v < 0)
            {
                v += 0x3F;
            }
            v >>= 6;
            wobble = (v << 1) + v;
            obj->unk20 = wobble;
            obj->field18.vy = wobble;
            obj->x += *(s16 *)&obj->z;
            obj->y += *(s16 *)&obj->field18.vy;
        }
    }
    obj->posX = -(cam->x + obj->x);
    obj->posY = -(camY + obj->y);
    obj_build_iris_rows(obj, (s32 *)cam);
    dma_copy_or_queue(obj->rows, gUnk_03001014 + 4, 228 * sizeof(u16), 0x10, 1);
}

NONMATCH("asm/code_08151144/obj_build_iris_rows.s", void obj_build_iris_rows(struct Obj *obj, s32 *pos))
{
    s32 cx;
    s32 cy;
    s32 radius;
    s32 y;
    s32 dy;
    s32 half;
    s32 left;
    s32 right;
    s32 i;
    s32 k;

    cx = (pos[0] + obj->x) >> 8;
    cy = (pos[1] + obj->y) >> 8;
    if (obj->flags & 1)
    {
        radius = obj->field30.radius;
        for (y = 0; y <= 0xE3; y++)
        {
            if (y > cy - radius && y < cy + radius)
            {
                dy = y - cy;
                half = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(radius * radius - dy * dy);
                left = cx - half;
                if (left <= 0)
                {
                    left = 1;
                }
                else if (left > 0xEF)
                {
                    left = 0xEF;
                }
                right = cx + half;
                if (right <= 0)
                {
                    right = 1;
                }
                else if (right > 0xEF)
                {
                    right = 0xEF;
                }
                ((u16 *)obj->rows)[y] = ((left << 8) | right);
            }
            else
            {
                ((u16 *)obj->rows)[y] = 0;
            }
        }
    }
    else
    {
        for (i = 0; i < 0xE4; i++)
        {
            ((u16 *)obj->rows)[i] = 0;
        }
    }
}
END_NONMATCH

void obj_set_flag_08(struct Obj *obj, s32 on)
{
    if (on == 0)
    {
        s32 f = obj->flags;
        s32 m = -9;
        m &= f;
        obj->flags = m;
    }
    else
    {
        s32 f = obj->flags;
        s32 m = 8;
        m |= f;
        obj->flags = m;
    }
}

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

u32 obj_point_in_iris(struct Obj *obj, s32 *target, s32 *anchor)
{
    s32 ax;
    s32 ay;
    s32 dx;
    s32 dy;
    s32 limit;
    s32 reach;
    s32 dist;

    ax = anchor[0] + obj->x;
    ay = anchor[1] + obj->y;
    dx = target[0] - ax;
    dy = target[1] - ay;
    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
    limit = obj->base / 0x100 * obj->field30.radius;
    reach = (limit << 1) + limit;
    if (reach < 0)
    {
        reach += 3;
    }
    limit = reach >> 2;
    if (dist < limit)
    {
        return 1;
    }
    return 0;
}

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
/* Saves the minigame's results in script vars: the level reached in 0x1CD9 (6 in modes above 5)
 * and the score (the digits object at +0x1CB0) in 0x1CDA. A level or score above the records
 * read at setup (+0x1F24 / +0x1F26) becomes the new record in 0x1CDF / 0x1CE0. Game var 0x1C
 * gets the current level (+0x1F20). */
void sync_script_vars(void)
{
    u16 level;

    if (((s32)((u32)gGameMode << 27) >> 27) > 5)
    {
        write_var(0x1CD9, 6);
    }
    else
    {
        write_var(0x1CD9, gGamePtr[0x1F20]);
    }
    level = read_var(0x1CD9);
    if (level > *(s16 *)(gGamePtr + 0x1F24))
    {
        write_var(0x1CDF, level);
    }
    write_var(0x1CDA, obj_get_value(*(void **)(gGamePtr + 0x1CB0)));
    if ((s16)obj_get_value(*(void **)(gGamePtr + 0x1CB0)) > *(s16 *)(gGamePtr + 0x1F26))
    {
        write_var(0x1CE0, obj_get_value(*(void **)(gGamePtr + 0x1CB0)));
    }
    game_var_write(0x1C, gGamePtr[0x1F20]);
}

/* The per-line buffer at gUnk_03001014 that hblank_iris_window reads. */
struct IrisBuf
{
    s16 surfaceY;      /* the surface's world y in pixels (code_08154490) */
    s16 barLines;      /* lines blacked out at the top and at the bottom */
    u16 win0h[0xE4];   /* REG_WIN0H for each line */
    u16 color[0xE4];   /* backdrop colour for each line */
};

// HBlank handler of the shutter effect: every line shows BG2's row 0x80. The top and bottom
// barLines lines are darkened to black with window 0 off. The lines between them show window 0's
// span for that line, with BG2 alpha-blended over everything. Unless game flag 4 is set, it also
// sets the backdrop colour from the per-line gradient.
void hblank_iris_window(void)
{
    s32 line = (u8)REG_VCOUNT;
    struct IrisBuf *buf;

    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0x80 - line;
    buf = (struct IrisBuf *)gUnk_03001014;
    if (line <= buf->barLines)
    {
        REG_BLDCNT = BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_BG2
                   | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG0;
        REG_BLDY = 0x1F;
        REG_WIN0H = 0;
        REG_DISPCNT &= (u16)~DISPCNT_WIN0_ON;
    }
    else if (line > SCREEN_HEIGHT - buf->barLines)
    {
        REG_BLDCNT = BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_BG2
                   | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG0;
        REG_BLDY = 0x1F;
        REG_WIN0H = 0;
        REG_DISPCNT &= (u16)~DISPCNT_WIN0_ON;
    }
    else
    {
        REG_DISPCNT |= DISPCNT_WIN0_ON;
        REG_BLDCNT = BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2;
        REG_WIN0H = buf->win0h[line];
    }
    if ((*(u16 *)(gGamePtr + 0x1CC4) & 4) == 0)
    {
        *(u16 *)BG_PLTT = ((struct IrisBuf *)gUnk_03001014)->color[line];
    }
}


u32 hitboxes_overlap(struct Obj *a, struct Obj *b)
{
    s32 hwA;
    s32 hhA;
    s32 hwB;
    s32 hhB;
    s16 ta;
    s16 tb;
    s32 dx;
    s32 dy;

    hwA = a->field28.half.halfW >> 1;
    hhA = a->field28.half.halfH >> 1;
    hwB = b->field28.half.halfW >> 1;
    hhB = b->field28.half.halfH >> 1;
    dx = ((u32)((a->posX > b->posX) ? a->posX - b->posX
              : (a->posX < b->posX) ? b->posX - a->posX : 0) << 8) >> 16;
    ta = (a->posY >> 8) - hhA;
    tb = (b->posY >> 8) - hhB;
    dy = (u16)((ta > tb) ? ta - tb : (ta < tb) ? tb - ta : 0);
    if ((s16)dx > hwA + hwB)
    {
        return 0;
    }
    if ((s16)dy > hhA + hhB)
    {
        return 0;
    }
    return 1;
}



extern u8 gSceneStateTable_083CCAC0[];
static inline u8 *state_rec_for(u8 *game)
{
    return gSceneStateTable_083CCAC0 + (game[0x1F20] << 5);
}

extern u16 gUnk_02000050;
extern u16 gUnk_02000054;
extern u16 gUnk_0200000A;
extern u16 gUnk_02000080;
extern u32 gUnk_03000C84;
extern const u8 gSceneDisplayClass[];
extern const u8 gTransitionClassDf60[];
extern const u8 gSceneTilemap[];
extern const u8 gScenePaletteA[];
extern const u8 gScenePaletteB[];
extern const u8 gHeapTagSceneTask[];
extern const u8 gHeapTagSceneUnpack[];
extern const u8 gHeapTagScrollBuf[];
extern const u8 gHeapTagSceneTable[];
extern const u8 gHeapTagSceneObjB[];
void task_insert(void *node, u8 kind, u32 data);
void obj_clear_tile_ptrs(void *p);
void obj_reset_e400(void *p);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
u32 read_varlen_plus1(const void *src);
void cpu_fast_set(const void *src, void *dst, u32 ctl);

NONMATCH("asm/code_08151144/scene_init_display.s", void *scene_init_display(void *obj, u32 kind, const void *table))
{
    u8 *s;
    s32 mode;
    s32 set;
    s32 i;
    s32 k;
    s32 r;
    s32 g;
    s32 b;
    u16 px;
    u16 *p;
    u8 *buf;
    u32 packed;
    void *objA;
    u8 *ent;

    s = obj;
    task_insert(s, (u8)kind, (u32)table);
    *(void **)(s + 0x18) = (void *)gSceneTable08cdde80;
    obj_clear_tile_ptrs(s + 0x1C);
    obj_clear_tile_ptrs(s + 0x840);
    obj_clear_tile_ptrs(s + 0x1064);
    *(u16 *)0x02000000 = 0;
    *(u16 *)(s + 0x1CC4) = 0;

    mode = ((s32)((u32)gGameMode << 27)) >> 27;
    if (mode > 5)
    {
        s[0x1F20] = 1;
        *(u16 *)(s + 0x1CC4) = *(u16 *)(s + 0x1CC4) | 0x20;
    }
    else
    {
        s[0x1F20] = mode;
    }

    ent = state_rec_for(s);
    *(u16 *)(s + 0x1F16) = *(u16 *)(ent + 0x1C);
    ent = state_rec_for(s);
    *(u16 *)(s + 0x1F18) = *(u16 *)(ent + 0x1C) - 0x64;
    *(u16 *)(s + 0x1F1A) = 0xE;
    *(u16 *)(s + 0x1F1C) = 0x1F4;
    s[0x1F21] = 0;
    s[0x1F22] = 0;
    *(s32 *)(s + 0x1CB4) = 0;
    *(u16 *)(s + 0x1CC6) = 0;
    *(s32 *)(s + 0x1C98) = 0;
    *(s32 *)(s + 0x1C9C) = 0;
    *(s32 *)(s + 0x1CA0) = 0;
    *(s32 *)(s + 0x1CA4) = 0;
    *(u16 *)(s + 0x1F24) = read_var(0x1CDF);
    *(u16 *)(s + 0x1F26) = read_var(0x1CE0);
    *(u16 *)(s + 0x1F28) = 0;
    *(u16 *)(s + 0x1F2A) = 0;
    *(u16 *)(s + 0x1F2C) = 0;

    objA = heap_alloc2(0x1C, 1, gHeapTagSceneTask, 0);
    task_insert(objA, 8, (u32)gHeapTagSceneTask);
    *(u32 *)((u8 *)objA + 0x18) = (u32)gSceneDisplayClass;
    *(void **)(s + 0x1F30) = objA;
    *(void **)((u8 *)objA + 0x14) = s;
    gGamePtr = s;
    gUnk_02000050 = 0xDF;
    *(u16 *)0x04000050 = gUnk_02000050;
    gUnk_02000054 = 0x1F;
    *(u16 *)0x04000054 = gUnk_02000054;
    *(u16 *)0x04000000 = *(u16 *)0x02000000 & 0xFBFF;
    (&gUnk_0200000A)[0] = 0x1701;
    (&gUnk_0200000A)[1] = 0x5801;
    (&gUnk_0200000A)[2] = 0x1D02;

    packed = 0x0800AF20;
    k = read_varlen_plus1((const void *)packed);
    buf = heap_alloc(k, 1, gHeapTagSceneUnpack, 0);
    ((void (*)(const void *, void *))gUnk_03000C84)((const void *)packed, buf);
    cpu_fast_set(buf, (void *)0x06000000, 0x2000);
    if (buf != 0)
    {
        heap_free_a(buf);
    }

    if ((gSys[0x888] & 6) == 2)
    {
        cpu_fast_set(gScenePaletteB, &gUnk_02000080, 0x80);
    }
    else
    {
        cpu_fast_set(gScenePaletteA, &gUnk_02000080, 0x80);
    }
    gUnk_02000080 = 0x190;

    for (set = 0; set <= 15; set++)
    {
        if (set == 0 || set == 2)
        {
            for (i = 0; i <= 15; i++)
            {
                p = (u16 *)((u8 *)&gUnk_02000080 + (set << 5) + (i << 1));
                px = *p;
                r = (((s32)(px & 0x1F)) * 3) >> 1;
                if (r > 0x1F)
                {
                    r = 0x1F;
                }
                g = (((s32)((px >> 5) & 0x1F)) * 3) >> 1;
                if (g > 0x1F)
                {
                    g = 0x1F;
                }
                b = (((s32)((px >> 10) & 0x1F)) * 3) >> 1;
                if (b > 0x1F)
                {
                    b = 0x1F;
                }
                *p = r | (g << 5) | (b << 10);
            }
        }
        else
        {
            for (i = 0; i <= 15; i++)
            {
                p = (u16 *)((u8 *)&gUnk_02000080 + (set << 5) + (i << 1));
                px = *p;
                r = (((s32)(px & 0x1F)) * 5) >> 1;
                if (r > 0x1F)
                {
                    r = 0x1F;
                }
                g = (((s32)((px >> 5) & 0x1F)) * 5) >> 1;
                if (g > 0x1F)
                {
                    g = 0x1F;
                }
                b = (((s32)((px >> 10) & 0x1F)) * 5) >> 1;
                if (b > 0x1F)
                {
                    b = 0x1F;
                }
                *p = r | (g << 5) | (b << 10);
            }
        }
    }

    cpu_fast_set(gSceneTilemap, (void *)0x0600C000, 0x400);
    *(u16 *)(gSys + 2) = *(u16 *)(gSys + 2) | 0x7FFF;

    for (k = 0; k < 0x40; k++)
    {
        px = ((u16 *)&gUnk_02000080)[k];
        b = (((s32)((px >> 10) & 0x1F)) * 2);
        if (b > 0x1F)
        {
            b = 0x1F;
        }
        g = (px >> 5) & 0x1F;
        r = (px & 0x1F) >> 1;
        if (r > 0x1F)
        {
            r = 0x1F;
        }
        ((u16 *)(s + 0x1CCA))[k] = r | (g << 5) | (b << 10);
    }

    gUnk_03001014 = heap_alloc_raw(0, 0x560, gHeapTagScrollBuf);
    *(void **)(s + 0x1CB4) = obj_init_table(heap_alloc2(0x44, 0, gHeapTagSceneTable, 0));
    gSys[0x888] |= 0x20;
    *(s32 *)(s + 0x1CB0) = 0;
    objA = heap_alloc2(8, 1, gHeapTagSceneObjB, 0);
    obj_reset_e400(objA);
    ((void **)objA)[1] = (void *)gTransitionClassDf60;
    *(void **)(s + 0x1CAC) = objA;
    return s;
}
END_NONMATCH

struct SysHalf2
{
    u8 filler0[2];
    u16 v2;
};

/* Ends the scene: turns off the HBlank interrupt (shadow of REG_DISPSTAT at 0x02000004, REG_IE,
   pending REG_IF) and its callbacks, releases the scene's objects, game task and iris, stops the
   HBlank DMA from the scanline buffer gUnk_03001014 and frees it, resets the display (shadow of
   REG_DISPCNT at 0x02000000) to OBJ 1D mapping only, frees the tile sets and removes the task. */
void scene_teardown(struct SceneState *state, u32 task)
{
    s32 flagsByte;
    s32 mask;

    state->sceneTable = (void *)gSceneTable08cdde80;
    slot_reset(0);
    set_irq_callback(2, NULL);
    REG_IME = 0;
    gUnk_02000004 = gUnk_02000004 & (u16)~DISPSTAT_HBLANK_IRQ;
    REG_IE = REG_IE & (u16)~IRQ_HBLANK;
    REG_IF = REG_IF | IRQ_HBLANK;
    REG_IME = 1;
    set_irq_callback(4, NULL);
    scene_release_objects(state);
    if (state->gameTask != NULL)
    {
        task_remove(state->gameTask, 3);
    }
    dma_cancel_request(gUnk_03001014 + 4);
    if (state->iris != NULL)
    {
        obj_destroy_iris((struct Obj *)state->iris, 3);
    }
    heap_free(gUnk_03001014);
    ((struct SysHalf2 *)gSys)->v2 = 0xFFFF;
    flagsByte = *(u8 *)(gSys + 0x888);
    mask = ~0x20;
    mask &= flagsByte;
    *(u8 *)(gSys + 0x888) = mask;
    set_flags_2_3(state->x14);
    obj_run_callbacks();
    *(u16 *)0x02000000 = DISPCNT_OBJ_1D_MAP;
    obj_free_tiles((u8 *)state + 0x1064, 2);
    obj_free_tiles((u8 *)state + 0x840, 2);
    obj_free_tiles((u8 *)state + 0x1C, 2);
    task_remove(state, task);
}

extern u16 gUnk_02000008;
extern u8 gSceneStateTable_083CCAC0[];
extern const u8 gHeapTagTimer[];
extern const u8 gHeapTagBounds[];
extern const u8 gHeapTagDigits[];
extern const u8 gHeapTagWindowObj[];
void obj_setup_tiles(void *obj, const void *src, u32 w, u32 h, u32 x, u32 y, void *vram, u32 extra);
void *obj_init_timer(void *obj);
void *obj_clear_bounds(void *obj);
void *obj_create_digits(void *obj);
void obj_set_value(void *obj, s16 value);
void *window_obj_setup(void *obj, u32 a, u32 b, u32 c, u32 d);
void *obj_create(void *unused, void *record);

NONMATCH("asm/code_08151144/scene_build_state.s", void scene_build_state(u8 *game))
{
    s32 i;
    s32 red;
    s32 green;
    s32 redStep;
    s32 greenStep;
    u16 flags;
    u16 *flagsP;
    s32 slot;
    void *def;
    void *obj;
    u8 *rec;

    flagsP = (u16 *)(game + 0x1CC4);
    if (game[0x1F20] == 0)
    {
        flags = *flagsP & 4;
    }
    else
    {
        flags = *flagsP | 4;
    }
    *flagsP = flags;

    redStep = 0x22;
    greenStep = 0x1A;
    red = 0x1F00;
    green = 0x1800;
    for (i = 0; i < 228; i++)
    {
        *(u16 *)(gUnk_03001014 + 0x1CC + (i << 1)) = ((red >> 8) | ((green >> 8) << 5));
        red -= redStep;
        green -= greenStep;
    }

    for (i = 0; i < 228; i++)
    {
        *(u16 *)(gUnk_03001014 + 4 + (i << 1)) = 0;
    }

    *(s32 *)(game + 0x1888) = 0;
    *(s32 *)(game + 0x188C) = 0;
    *(s32 *)(game + 0x1890) = 0;
    *(s32 *)(game + 0x1894) = 0;
    *(u16 *)(game + 0x1F12) = *(u16 *)(game + 0x1F1A);
    *(s32 *)(game + 0x1CB8) = 0;
    *(s32 *)(game + 0x1CBC) = 0;
    *(u16 *)(game + 0x1F14) = 0;

    rec = state_rec_for(game);
    *(u16 *)(game + 0x1CC8) = rec[0x1B];
    rec = state_rec_for(game);
    obj_setup_tiles(game + 0x1C, *(void **)rec, ((s32)((u32)*(u16 *)(rec + 0x18) << 21)) >> 16, 0x20, 0, 0, (void *)0x0600B800, *(u32 *)(rec + 4));

    rec = state_rec_for(game);
    obj_setup_tiles(game + 0x840, *(void **)(rec + 8), ((s32)((u32)*(u16 *)(rec + 0x18) << 21)) >> 16, 0x20, 0, 0, (void *)0x0600E800, *(u32 *)(rec + 0xC));

    *(void **)(game + 0x1CA0) = obj_init_timer(heap_alloc2(0x20, 1, gHeapTagTimer, 0));
    *(void **)(game + 0x1CA4) = obj_clear_bounds(heap_alloc2(0x18, 1, gHeapTagBounds, 0));
    *(void **)(game + 0x1CB0) = obj_create_digits(heap_alloc2(0x38, 1, gHeapTagDigits, 0));
    if (((s32)((u32)gGameMode << 27) >> 27) > 5)
    {
        obj_set_value(*(void **)(game + 0x1CB0), *(s16 *)(game + 0x1F2C));
    }

    *(void **)(game + 0x1CA8) = 0;
    if ((*(u16 *)(game + 0x1CC4) & 4) == 0)
    {
        obj = heap_alloc2(0xA4, 0, gHeapTagWindowObj, 0);
        obj = window_obj_setup(obj, 0, 2, 0x1C, 0xD);
        *(void **)(game + 0x1CA8) = obj;
        *(u16 *)((u8 *)obj + 0xA2) = 0x6F;
    }
    else
    {
        gUnk_02000008 = 0x1C03;
        rec = state_rec_for(game);
        obj_setup_tiles(game + 0x1064, *(void **)(rec + 0x14), 0x60, 0x20, 0, 0, (void *)0x0600E000, 0);
    }

    if ((gGameMode & 0x20) != 0)
    {
        *(u16 *)(game + 0x1CC4) = *(u16 *)(game + 0x1CC4) | 2;
    }

    def = *(void **)(state_rec_for(game) + 0x10);
    slot = 0;
    for (;;)
    {
        if (*(u32 *)def == 0)
        {
            break;
        }
        obj = obj_create(game, def);
        *(void **)(game + 0x1898 + (slot << 2)) = obj;
        def = (u8 *)def + 0x10;
        slot++;
        if (slot > 255)
        {
            break;
        }
    }
    if (slot <= 255)
    {
        for (i = slot; i <= 255; i++)
        {
            *(u32 *)(game + 0x1898 + (i << 2)) = 0;
        }
    }

    *(void **)(game + 0x1C98) = *(void **)(game + 0x1898);
    *(void **)(game + 0x1C9C) = *(void **)(game + 0x189C);
}
END_NONMATCH
