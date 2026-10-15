#include "global.h"
#include "ram_routines.h"

struct RomBox
{
    s16 left;
    s16 bottom;
    s16 right;
    s16 top;
};

struct Box6
{
    s16 left;
    s16 right;
    s16 top;
    s16 bottom;
    s16 zTop;
    s16 zBottom;
};

struct Obj
{
    u8 filler0[0x2];
    u8 facing;
    u8 filler3[0xC - 0x3];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u8 sector;
    u8 filler25[0x28 - 0x25];
    struct Obj *entA;
    struct Obj *entB;
    u8 filler30[0x1B4 - 0x30];
    struct RomBox *romBox;
    u8 filler1B8[0x1C0 - 0x1B8];
    s32 zSpan;
    u8 filler1C4[0x1CC - 0x1C4];
    s32 unk1CC;
    u8 filler1D0[0x200 - 0x1D0];
    struct Obj *tgtA;
    struct Obj *tgtB;
    u8 filler208[0x214 - 0x208];
    u8 mode;
    u8 filler215;
    u8 unk216;
    u8 filler217[0x23E - 0x217];
    s16 speed;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 zRest;
    u8 filler24C[0x258 - 0x24C];
    s32 zVel;
    u8 filler25C[0x294 - 0x25C];
    u8 *rec;
    u8 filler298[0x2C4 - 0x298];
    s32 zStart;
    s32 zLimit;
    u8 filler2CC[0x2D4 - 0x2CC];
    s32 zTarget;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 zSpeed;
    u16 steps;
};

struct State
{
    u8 filler0[0x1A0];
    u32 handler;
    u8 filler1A4[0x1AC - 0x1A4];
    u32 callback;
    u8 filler1B0[0x1D2 - 0x1B0];
    u16 timer;
    u8 filler1D4[0x1D6 - 0x1D4];
    u8 flags;
};

void probe_then_move(void);
void route_or_move_ac90(void);
void route_or_move_ad10(void);
void step_move_set_state_4(void);
void wander_to_edge(void);
void wander_diag(void);
void random_cell_setup(struct Obj *obj, u32 callback);

ASM_FUNC("asm/code_0813AC90/zone_point_code.s", s8 zone_point_code(struct Obj *obj, struct Obj *zone, s32 margin))

ASM_FUNC("asm/code_0813AC90/zone_overlap_code.s", void zone_overlap_code(void))

ASM_FUNC("asm/code_0813AC90/box_hit_kind.s", void box_hit_kind(void))

ASM_FUNC("asm/code_0813AC90/probe_body_hit.s", void probe_body_hit(void))

ASM_FUNC("asm/code_0813AC90/box_overlap_xy.s", void box_overlap_xy(void))

ASM_FUNC("asm/code_0813AC90/start_move_to.s", void start_move_to(struct Obj *obj, s32 x, s32 y, s32 speed, u8 zero))

ASM_FUNC("asm/code_0813AC90/start_move_to_b.s", void start_move_to_b(void))

ASM_FUNC("asm/code_0813AC90/start_move_to_3d.s", void start_move_to_3d(void))

ASM_FUNC("asm/code_0813AC90/wander_to_edge.s", void wander_to_edge(void))

ASM_FUNC("asm/code_0813AC90/wander_diag.s", void wander_diag(void))

ASM_FUNC("asm/code_0813AC90/collide_react.s", void collide_react(void))

ASM_FUNC("asm/code_0813AC90/collide_wander_diag.s", void collide_wander_diag(void))

ASM_FUNC("asm/code_0813AC90/z_approach.s", void z_approach(void))

ASM_FUNC("asm/code_0813AC90/collide_boxes.s", void collide_boxes(void))

ASM_FUNC("asm/code_0813AC90/random_cell_setup.s", void random_cell_setup(struct Obj *obj, u32 callback))

u32 box_contains_fixed(struct Box6 *box, struct Obj *a, struct Obj *b)
{
    s32 ax = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->x, 0x100);
    s32 ay = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->y, 0x100);
    s32 az = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->z, 0x100);
    s32 bx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->x, 0x100);
    s32 by = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->y, 0x100);
    s32 bz = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->z, 0x100);

    if (ax - box->left > bx)
        return 0;
    if (box->right + ax < bx)
        return 0;
    if (ay - box->bottom > by)
        return 0;
    if (ay + box->top < by)
        return 0;
    if (az - box->zBottom > bz)
        return 0;
    if (box->zTop + az < bz)
        return 0;
    return 1;
}

u32 distance_xyz(struct Obj *a, struct Obj *b)
{
    s32 dx = a->x - b->x;
    s32 dy = a->y - b->y;
    s32 dz = (a->z + a->dz) - (b->z + b->dz);
    return MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy + dz * dz);
}

u32 distance_xy(struct Obj *a, struct Obj *b)
{
    s32 dx = a->x - b->x;
    s32 dy = a->y - b->y;
    return MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy);
}

ASM_FUNC("asm/code_0813AC90/tick_fire.s", void tick_fire(void))

ASM_FUNC("asm/code_0813AC90/tick_store.s", void tick_store(void))

ASM_FUNC("asm/code_0813AC90/box_contacts_aim_at_target.s", void box_contacts_aim_at_target(void))

void start_wander(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)probe_then_move;
}

void start_wander_edge_c888(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)route_or_move_ad10;
}

void start_wander_diag_c8f8(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_diag);
    *out = (u32)route_or_move_ac90;
}

void start_wander_edge_c8f8(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)route_or_move_ac90;
}

void start_wander_diag_b(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_diag);
    *out = (u32)step_move_set_state_4;
}

void start_wander_b(u32 unused, struct Obj *obj, u32 *out)
{
    random_cell_setup(obj, (u32)wander_to_edge);
    *out = (u32)step_move_set_state_4;
}

void run_state_handler(struct Obj *obj, struct State *state)
{
    u32 *slot = &state->handler;
    ((void (*)(struct Obj *, struct State *, u32 *))*slot)(obj, state, slot);
}

void start_z_move(struct Obj *obj, s32 z, u16 speed, u8 rel, s32 zLimit)
{
    s32 d;
    s32 raw;

    if (rel != 0)
        z += obj->z + obj->dz;
    obj->zTarget = z;
    obj->zStart = obj->z + obj->dz;
    obj->zLimit = zLimit;
    obj->zSpan = obj->zTarget - obj->zStart;
    obj->zSpeed = speed;
    d = speed + 2 * obj->zSpan;
    raw = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(-speed * speed + ((d + (s32)((u32)d >> 31)) >> 1), d);
    obj->zRest = -raw;
    if (obj->zRest == 0)
        obj->zRest = 1;
}

u32 accum_step_check(struct Obj *obj)
{
    u32 moving;
    obj->dz += obj->zVel;
    if (obj->dz > 0)
    {
        obj->zVel -= obj->zRest;
        moving = 1;
    }
    else
        moving = 0;
    return moving;
}

void set_z_motion(struct Obj *obj, s32 zVel, s32 zRest)
{
    obj->zVel = zVel;
    obj->zRest = zRest;
}

u32 sub_0813C27C(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xy(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xy_b(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xy_c(struct Obj *obj)
{
    u32 moving;
    if (obj->steps == 0)
        moving = 0;
    else
    {
        u32 n;
        obj->x += obj->vx;
        obj->y += obj->vy;
        n = obj->steps - 1;
        obj->steps = n;
        moving = (0u - (u16)n) >> 31;
    }
    return moving;
}

u32 step_pos_xyz(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->x += obj->vx;
    obj->y += obj->vy;
    obj->dz += obj->zSpeed;
    obj->steps--;
    return obj->steps != 0;
}

u32 pos_step_countdown_running(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->dz += obj->zSpeed;
    obj->steps--;
    return obj->steps != 0;
}

u32 step_pos_xy_plain(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->x += obj->vx;
    obj->y += obj->vy;
    obj->steps--;
    return obj->steps != 0;
}
