#include "global.h"
#include "ram_routines.h"

struct ActorChild
{
    u8 filler0[0x11];
    u8 unk11;
};

struct Flags1D6Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct ModeBits
{
    u8 mode : 3;
    u8 rest : 5;
};

struct SubBox
{
    u8 filler0[0x38];
    u16 unk38;
    u16 unk3A;
    u16 unk3C;
    u16 unk3E;
};

struct Owner
{
    u8 filler0[0x28];
    void *entries[0];
};

struct Actor
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 baseA;
    s32 baseB;
    u8 filler1C[0x136 - 0x1C];
    u8 flags136;
    u8 filler137[0x198 - 0x137];
    void *update;
    void *update2;
    void *handler;
    void *collide;
    u8 filler1A8[0x1B0 - 0x1A8];
    s8 *rect;
    s16 *sensor;
    s16 vecX;
    s16 vecY;
    u8 filler1BC[0x1D6 - 0x1BC];
    u8 flags1D6;
    u8 filler1D7[0x214 - 0x1D7];
    u8 mode;
    u8 filler215[0x242 - 0x215];
    s16 vx;
    s16 vy;
    u8 filler246[0x272 - 0x246];
    u16 width;
    u16 unk274;
    u16 height;
    u8 filler278[0x294 - 0x278];
    struct ActorChild *unk294;
    u8 filler298[0x2B6 - 0x298];
    u8 flags2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 startX;
    s32 startY;
    s32 axisStart;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 targetX;
    s32 targetY;
    s32 axisTarget;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 step;
    u16 frames;
    u8 filler2E0[0x304 - 0x2E0];
    void *map;
    u8 filler308[0x33E - 0x308];
    u8 flags33E;
};

void load_record_fields(struct Actor *ent, u32 mode);
void load_hit_box_words(struct Actor *ent, u32 mode);
void load_hit_box_bytes(struct Actor *ent, u32 mode);
void setup_random_offset_state(void);
void run_handler_1a0(s32 arg, u8 *obj);
void run_handler_1a0_b(s32 arg, u8 *obj);
void entity_mode_49_noop(void);
void box_hit_state_d4a5(void);
void active_hit_state(void);
void box_hit_state_d4ed(void);
void setup_mode4_with_cell(s32 arg, struct Actor *obj, s32 *slot);
void pick_active_target(void);
u8 boxes_overlap_oriented(s16 *box, struct Actor *ent, void *entry, s32 zAbove, s32 zBelow);
void setup_mode4_with_index(s32 arg, struct Actor *obj, s32 *slot);
void run_state_handler(void);
void start_state_c189(s32 arg, struct Actor *obj);
void start_state_d385(s32 arg, struct Actor *obj);
void start_state_d405(s32 arg, struct Actor *obj);
void obj_random_offset(struct Actor *obj, u16 dir);
void random_step_pick(void);
u32 obj_pick_dir_toward_owner(s32 arg, struct Actor *obj);
void obj_start_move_to_target(struct Actor *obj, s32 a, s32 vx, s32 vy);
void obj_try_start_jump(struct Actor *obj);
void obj_step_toward_target_08138FDC(struct Actor *obj);
void obj_jump_arc_step(struct Actor *obj);
void obj_pick_move_vector(struct Actor *obj, u32 dir);
u32 owner_entry(s32 arg, u32 idx);
u8 find_active_entry(struct Owner *owner, u8 *idx);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);

ASM_FUNC("asm/code_0813C4A8/step_z.s", s32 step_z(struct Actor *obj))

ASM_FUNC("asm/code_0813C4A8/start_axis_move.s", void start_axis_move(void))

ASM_FUNC("asm/code_0813C4A8/advance_move.s", s32 advance_move(struct Actor *obj))

ASM_FUNC("asm/code_0813C4A8/start_move_2d.s", void start_move_2d(void))

s32 box_overlap_check(struct Actor *a, struct Actor *b)
{
    s32 aSum = a->baseA + a->baseB;
    s32 bSum = b->baseA + b->baseB;
    s32 aW = a->unk274;
    s32 bW = b->unk274;
    s32 result = 0;

    if (aSum + aW >= bSum)
    {
        if (aSum <= bSum + bW)
        {
            result = 1;
        }
    }
    return result;
}

s32 heights_overlap_y(struct Actor *a, struct Actor *b)
{
    s32 result = 0;

    if (a->posY >= b->posY - b->height)
    {
        if (a->posY - a->height <= b->posY)
        {
            result = 1;
        }
    }
    return result;
}

s32 widths_overlap_x(struct Actor *a, struct Actor *b)
{
    s32 result;
    u16 halfA = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->width, 2);
    u16 halfB = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b->width, 2);

    result = 0;
    if (a->posX + halfA >= b->posX - halfB)
    {
        if (a->posX - halfA <= b->posX + halfB)
        {
            result = 1;
        }
    }
    return result;
}

ASM_FUNC("asm/code_0813C4A8/tile_blocks_move.s", void tile_blocks_move(void))

ASM_FUNC("asm/code_0813C4A8/step_move_set_state_8.s", void step_move_set_state_8(void))

ASM_FUNC("asm/code_0813C4A8/probe_then_move.s", void probe_then_move(void))

ASM_FUNC("asm/code_0813C4A8/route_or_move_ad10.s", void route_or_move_ad10(void))

ASM_FUNC("asm/code_0813C4A8/route_or_move_ac90.s", void route_or_move_ac90(void))

ASM_FUNC("asm/code_0813C4A8/step_move_set_state_4.s", void step_move_set_state_4(void))

void vec3_normalize_fx8(s32 *v, s32 *out)
{
    s32 len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    out[0] = (v[0] << 8) / len;
    out[1] = (v[1] << 8) / len;
    out[2] = (v[2] << 8) / len;
}

void vec2_normalize_fx8(s32 *v, s32 *out)
{
    s32 len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v[0] * v[0] + v[1] * v[1]);

    out[0] = (v[0] << 8) / len;
    out[1] = (v[1] << 8) / len;
}

void vec3_to_fx8(s32 *src, s32 *dst)
{
    dst[0] = src[0] << 8;
    dst[1] = src[1] << 8;
    dst[2] = src[2] << 8;
}

void pair_shl8(s32 *src, s32 *dst)
{
    dst[0] = src[0] << 8;
    dst[1] = src[1] << 8;
}

void vec3_div_256(s32 *v, s32 *out)
{
    out[0] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[0], 0x100);
    out[1] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[1], 0x100);
    out[2] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[2], 0x100);
}

void pair_div256(s32 *v, s32 *out)
{
    out[0] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[0], 0x100);
    out[1] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[1], 0x100);
}

void vec3_normalize_0813CB40(s32 *v, s32 *out)
{
    s32 len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    out[0] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[0], len);
    out[1] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[1], len);
    out[2] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[2], len);
}

void vec2_normalize(s32 *v, s32 *out)
{
    s32 len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v[0] * v[0] + v[1] * v[1]);

    out[0] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[0], len);
    out[1] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v[1], len);
}

s32 vec3_length_0813cc34(s32 *v)
{
    return MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

s32 vec2_length(s32 *v)
{
    return MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v[0] * v[0] + v[1] * v[1]);
}

void vec3_cross_0813cc90(s32 *a, s32 *b, s32 *out)
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

s32 sub_0813CCC4(s32 *a, s32 *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

s32 vec2_dot(s32 *a, s32 *b)
{
    return a[0] * b[0] + a[1] * b[1];
}

void sub_0813CCF8(s32 *src, s32 k, s32 *dst)
{
    dst[0] = src[0] * k;
    dst[1] = src[1] * k;
    dst[2] = src[2] * k;
}

void vec2_scale(s32 *src, s32 k, s32 *dst)
{
    dst[0] = src[0] * k;
    dst[1] = src[1] * k;
}

void vec3_sub(s32 *a, s32 *b, s32 *out)
{
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

void vec2_sub(s32 *a, s32 *b, s32 *out)
{
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
}

void vec3_add(s32 *a, s32 *b, s32 *out)
{
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void vec2_add(s32 *a, s32 *b, s32 *out)
{
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
}

void vec3_store(s32 a, s32 b, s32 c, s32 *out)
{
    out[0] = a;
    out[1] = b;
    out[2] = c;
}

void vec2_store(s32 a, s32 b, s32 *out)
{
    out[0] = a;
    out[1] = b;
}

ASM_FUNC("asm/code_0813C4A8/active_hit_state.s", void active_hit_state(void))

ASM_FUNC("asm/code_0813C4A8/pick_active_target.s", void pick_active_target(void))

ASM_FUNC("asm/code_0813C4A8/step_toward.s", void step_toward(void))

void entity_mode_50(s32 unused, struct Actor *obj)
{
    struct ActorChild *child = obj->unk294;
    u8 *p = &child->unk11;

    *p = *p | 0x40;
    ((struct ModeBits *)&obj->mode)->mode = 4;
}

void stub_nop_0813d198(void)
{
}

void entity_mode_49(s32 unused, struct Actor *obj)
{
    u8 *p = &obj->flags33E;

    *p = *p | 0x10;
    obj->update = (void *)entity_mode_49_noop;
    ((struct ModeBits *)&obj->mode)->mode = 4;
}

void set_update_dispatch(s32 unused, struct Actor *obj)
{
    load_record_fields(obj, 0);
    load_hit_box_words(obj, 0);
    obj->update = (void *)run_handler_1a0;
    obj->handler = (void *)setup_random_offset_state;
    obj->collide = (void *)active_hit_state;
    ((struct ModeBits *)&obj->mode)->mode = 3;
}

void set_update_dispatch_noop(s32 unused, struct Actor *obj)
{
    u8 *p = &obj->flags33E;

    *p = *p | 0x10;
    load_record_fields(obj, 0);
    load_hit_box_words(obj, 0);
    obj->update = (void *)run_handler_1a0_b;
    obj->handler = (void *)setup_random_offset_state;
    obj->collide = (void *)setup_random_offset_state;
    ((struct ModeBits *)&obj->mode)->mode = 3;
}

void entity_mode_49_noop(void)
{
}

void run_handler_1a0(s32 arg, u8 *obj)
{
    u8 *slot = obj + 0x1A0;
    void (*handler)(s32, u8 *, u8 *) = *(void (**)(s32, u8 *, u8 *))slot;

    handler(arg, obj, slot);
}

void run_handler_1a0_b(s32 arg, u8 *obj)
{
    u8 *slot = obj + 0x1A0;
    void (*handler)(s32, u8 *, u8 *) = *(void (**)(s32, u8 *, u8 *))slot;

    handler(arg, obj, slot);
}

ASM_FUNC("asm/code_0813C4A8/random_step_pick.s", void random_step_pick(void))

ASM_FUNC("asm/code_0813C4A8/box_hit_state_d4a5.s", void box_hit_state_d4a5(void))

ASM_FUNC("asm/code_0813C4A8/box_hit_state_d4ed.s", void box_hit_state_d4ed(void))

void start_state_d405(s32 arg, struct Actor *obj)
{
    void (*fn)(s32, struct Actor *);
    load_record_fields(obj, 0x15);
    load_hit_box_bytes(obj, 0x28);
    obj->update = (void *)box_hit_state_d4ed;
    fn = (void (*)(s32, struct Actor *))box_hit_state_d4ed;
    obj->handler = (void *)setup_mode4_with_index;
    obj->collide = (void *)setup_mode4_with_index;
    fn(arg, obj);
}

void start_state_d385(s32 arg, struct Actor *obj)
{
    void (*fn)(s32, struct Actor *);

    load_record_fields(obj, 0);
    load_hit_box_words(obj, 0);
    obj->update = (void *)box_hit_state_d4a5;
    fn = (void (*)(s32, struct Actor *))box_hit_state_d4a5;
    obj->handler = (void *)setup_mode4_with_cell;
    obj->collide = (void *)setup_mode4_with_cell;
    fn(arg, obj);
}

void flags_then_start_d385(s32 arg, struct Actor *obj)
{
    u8 b = obj->flags33E;
    u8 c;

    obj->flags33E = b | 0x10;
    c = obj->flags2B6;
    obj->flags2B6 = c | 0x40;
    start_state_d385(arg, obj);
}

void setup_mode4_with_cell(s32 arg, struct Actor *obj, s32 *slot)
{
    ((struct ModeBits *)&obj->mode)->mode = 4;
    obj_random_offset(obj, (u16)obj_pick_dir_toward_owner(arg, obj));
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_try_start_jump(obj);
    obj_step_toward_target_08138FDC(obj);
    obj_jump_arc_step(obj);
    ((struct Flags1D6Bits *)&obj->flags1D6)->bit0 = 0;
    ((struct Flags1D6Bits *)&obj->flags1D6)->bit1 = 1;
    *slot = (s32)random_step_pick;
}

void flags_then_start_c189(s32 arg, struct Actor *obj)
{
    u8 b = obj->flags33E;
    u8 c;

    obj->flags33E = b | 0x10;
    obj->flags136 = 1;
    c = obj->flags2B6;
    obj->flags2B6 = c | 0x40;
    start_state_c189(arg, obj);
}

void setup_mode4_with_index(s32 arg, struct Actor *obj, s32 *slot)
{
    ((struct ModeBits *)&obj->mode)->mode = 4;
    obj_pick_move_vector(obj, owner_entry(arg, 0xFF));
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_try_start_jump(obj);
    obj_step_toward_target_08138FDC(obj);
    obj_jump_arc_step(obj);
    ((struct Flags1D6Bits *)&obj->flags1D6)->bit0 = 0;
    ((struct Flags1D6Bits *)&obj->flags1D6)->bit1 = 1;
    *slot = (s32)random_step_pick;
}

void start_state_c189(s32 arg, struct Actor *obj)
{
    void (*fn)(s32, struct Actor *);

    load_record_fields(obj, 0);
    load_hit_box_words(obj, 0);
    obj->update = (void *)run_state_handler;
    fn = (void (*)(s32, struct Actor *))run_state_handler;
    obj->handler = (void *)setup_mode4_with_cell;
    obj->collide = (void *)setup_mode4_with_cell;
    fn(arg, obj);
}

ASM_FUNC("asm/code_0813C4A8/timer_overlap_state.s", void timer_overlap_state(void))

ASM_FUNC("asm/code_0813C4A8/aim_and_check_overlap.s", void aim_and_check_overlap(void))

ASM_FUNC("asm/code_0813C4A8/timer_overlap_state_b.s", void timer_overlap_state_b(void))

ASM_FUNC("asm/code_0813C4A8/aim_and_box_state.s", void aim_and_box_state(void))

ASM_FUNC("asm/code_0813C4A8/timer_overlap_state_c.s", void timer_overlap_state_c(void))

ASM_FUNC("asm/code_0813C4A8/place_child_by_stage.s", void place_child_by_stage(void))
