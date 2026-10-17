#include "global.h"
#include "ram_routines.h"
struct RomBox
{
    s16 left;
    s16 bottom;
    s16 right;
    s16 top;
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

struct FlagBits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
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
    u8 filler1BC[0x1D4 - 0x1BC];
    u16 timer1D4;
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
void box_hit_state_d4a5(struct Owner *owner, struct Actor *obj);
u8 advance_move_step(struct Owner *owner, struct Actor *ent);
void obj_step_toward_target_08138FDC(struct Actor *obj);
void tick_store(void);
void obj_state_run_advance(void);
void child_box_state(void);
void place_child_by_stage(s32 arg, struct Obj *ent, u32 *out);
u32 mt19937_next(void);
s32 sub_081DE850(s32 range, u32 seed);
void active_hit_state(struct Owner *owner, struct Actor *ent, void **out);
void box_hit_state_d4ed(struct Owner *owner, struct Actor *obj);
void setup_mode4_with_cell(s32 arg, struct Actor *obj, s32 *slot);
void pick_active_target(struct Owner *owner, struct Actor *obj, void **out);
u8 boxes_overlap_oriented(s16 *box, struct Actor *ent, void *entry, s32 zAbove, s32 zBelow);
void setup_mode4_with_index(s32 arg, struct Actor *obj, s32 *slot);
void run_state_handler(void);
void start_state_c189(s32 arg, struct Actor *obj);
void start_state_d385(s32 arg, struct Actor *obj);
void start_state_d405(s32 arg, struct Actor *obj);
void obj_random_offset(struct Actor *obj, u16 dir);
void random_step_pick(struct Owner *owner, struct Actor *ent, u32 *out);
u32 obj_pick_dir_toward_owner(s32 arg, struct Actor *obj);
void obj_start_move_to_target(struct Actor *obj, s32 a, s32 vx, s32 vy);
void obj_try_start_jump(struct Actor *obj);
void obj_step_toward_target_08138FDC(struct Actor *obj);
void obj_jump_arc_step(struct Actor *obj);
void obj_pick_move_vector(struct Actor *obj, u32 dir);
u32 owner_entry(s32 arg, u32 idx);
u8 find_active_entry(struct Owner *owner, u8 *idx);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
s8 zone_overlap_code(struct Owner *owner, struct Actor *ent, u32 zero);
void box_contacts_aim_at_target(void);
void aim_and_box_state(struct Owner *owner, struct Actor *ent);
void aim_and_check_overlap(struct Owner *owner, struct Actor *ent);

u32 step_z(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->dz += obj->zSpeed;
    obj->steps--;
    return obj->steps != 0;
}

void start_axis_move(struct Actor *ent, s32 a, u16 frames, u8 rel)
{
    if (rel != 0)
        a += ent->baseA + ent->baseB;

    ent->frames = frames;
    ent->axisStart = ent->baseA + ent->baseB;
    ent->axisTarget = a;
    a -= ent->axisStart;
    ent->step = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a, frames);
}

u32 advance_move(struct Obj *obj)
{
    if (obj->steps == 0)
        return 0;
    obj->x += obj->vx;
    obj->y += obj->vy;
    obj->steps--;
    return obj->steps != 0;
}

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

static inline u8 actor_advance_move(struct Actor *ent)
{
    if (ent->frames == 0)
        return 0;
    ent->posX += ent->vx;
    ent->posY += ent->vy;
    ent->frames--;
    return ent->frames != 0;
}

void step_move_set_state_8(struct Owner *owner, struct Actor *ent, u32 *out)
{
    if (actor_advance_move(ent) == 0)
        out[0] = out[2];
}

void probe_then_move(struct Owner *owner, struct Actor *ent, u32 *out)
{
    if (zone_overlap_code(owner, ent, 0) != -1)
    {
        out[0] = (u32)box_contacts_aim_at_target;
        return;
    }

    if (actor_advance_move(ent) == 0)
        out[0] = out[1];
}

void box_contacts_aim_at_target(void);
void route_or_move_ad10(struct Owner *owner, struct Actor *ent, u32 *out)
{
    if (zone_overlap_code(owner, ent, 0) != -1)
    {
        out[0] = out[2];
        return;
    }

    if (actor_advance_move(ent) == 0)
        out[0] = out[1];
}

s8 zone_point_code(struct Owner *owner, struct Actor *ent, u32 zero);
void route_or_move_ac90(struct Owner *owner, struct Actor *ent, u32 *out)
{
    if (zone_point_code(owner, ent, 0) != -1)
    {
        out[0] = out[2];
        return;
    }

    if (actor_advance_move(ent) == 0)
        out[0] = out[1];
}

void step_move_set_state_4(struct Owner *owner, struct Actor *ent, u32 *out)
{
    if (actor_advance_move(ent) == 0)
        out[0] = out[1];
}

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

ASM_FUNC("asm/code_0813C4A8/active_hit_state.s", void active_hit_state(struct Owner *owner, struct Actor *ent, void **out))

void step_toward(struct Actor *obj, void *entry, s32 a, s32 *out);
void pair_div256(s32 *dst, s32 *src);
void pair_shl8(s32 *dst, s32 *src);
ASM_FUNC("asm/code_0813C4A8/pick_active_target.s", void pick_active_target(struct Owner *owner, struct Actor *obj, void **out))

ASM_FUNC("asm/code_0813C4A8/step_toward.s", void step_toward(struct Actor *obj, void *entry, s32 a, s32 *out))

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

void random_step_pick(struct Owner *owner, struct Actor *ent, u32 *out)
{
    s32 flag = 0;

    if (advance_move_step(owner, ent) == 1)
    {
        obj_step_toward_target_08138FDC(ent);
    }
    else
    {
        s32 t = 0;

        if (*(s32 *)((u8 *)ent + 0x25C) + 1 == 0)
        {
            t = 1;
        }
        flag = t;
    }
    if ((*((u8 *)ent + 0x2B5) & 8) != 0)
    {
        obj_jump_arc_step(ent);
    }
    else
    {
        s32 t = 0;

        if (*(s32 *)((u8 *)ent + 0x25C) + 1 == 0)
        {
            t = 1;
        }
        flag = t;
    }
    if (flag != 0)
    {
        u32 idx = sub_081DE850(4, mt19937_next());
        u32 off = idx << 1;
        u8 *slot = (u8 *)ent + 0x128;
        u16 value = *(u16 *)(slot + off);

        *(u16 *)((u8 *)ent + 0x1D2) = value;
        ((struct Flags1D6Bits *)&ent->flags1D6)->bit0 = 1;
        ((struct Flags1D6Bits *)&ent->flags1D6)->bit1 = 0;
        if (*(u16 *)((u8 *)ent + 0x1D2) != 0)
        {
            ((struct ModeBits *)&ent->mode)->mode = 0;
            out[0] = (u32)tick_store;
            *(u32 *)((u8 *)ent + 0x1AC) = out[1];
        }
        else
        {
            out[0] = out[1];
        }
    }
}

void box_hit_state_d4a5(struct Owner *owner, struct Actor *obj)
{
    void *entry;
    s16 box[8];
    u8 idx;
    struct SubBox *sub;
    void **handler;

    handler = (void **)&obj->handler;
    ((void (*)(struct Owner *, struct Actor *, void **))*handler)(owner, obj, handler);
    if ((obj->flags1D6 & 1) != 0)
    {
        sub = (struct SubBox *)((u8 *)obj + 0x100);
        if (find_active_entry(owner, &idx) != 0)
        {
            entry = *(void **)((u8 *)owner + 0x28 + (idx << 2));
            box[0] = sub->unk3C;
            box[1] = sub->unk3E;
            box[2] = sub->unk3A;
            box[3] = sub->unk38;
            if (point_in_box(box, entry, 0x7FFF, 0) != 0)
                obj->update = (void *)start_state_d405;
        }
    }
}

ASM_FUNC("asm/code_0813C4A8/box_hit_state_d4ed.s", void box_hit_state_d4ed(struct Owner *owner, struct Actor *obj))

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

ASM_FUNC("asm/code_0813C4A8/aim_and_check_overlap.s", void aim_and_check_overlap(struct Owner *owner, struct Actor *ent))

ASM_FUNC("asm/code_0813C4A8/timer_overlap_state_b.s", void timer_overlap_state_b(void))

ASM_FUNC("asm/code_0813C4A8/aim_and_box_state.s", void aim_and_box_state(struct Owner *owner, struct Actor *ent))

ASM_FUNC("asm/code_0813C4A8/timer_overlap_state_c.s", void timer_overlap_state_c(void))
ASM_FUNC("asm/code_0813C4A8/place_child_by_stage.s", void place_child_by_stage(s32 arg, struct Obj *ent, u32 *out))
