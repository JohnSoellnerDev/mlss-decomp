#include "global.h"
#include "ram_routines.h"

struct Frame
{
    u8 filler0[0x2];
    u16 count;
    u8 filler4[0x6 - 0x4];
    s8 k6;
};

struct Pose
{
    u8 filler0[0x6];
    s16 unk6;
    u8 filler8[0x2];
    s16 unkA;
    u8 filler0C[0x11 - 0x0C];
    u8 flags11;
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 amount20;
    u8 filler21[0x40 - 0x21];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler4C[0x54 - 0x4C];
    s32 unk54;
    u8 filler58[0xE4 - 0x58];
    struct Frame *frame;
    u8 fillerE8[0x111 - 0xE8];
    u8 status111;
};

struct HalfPair
{
    s16 lo;
    s16 hi;
};

struct PoseFlagsBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct PoseFlags12Bit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct PoseFlags11Bits
{
    u8 unk0 : 6;
    u8 mirror : 1;
    u8 unk7 : 1;
};

struct FlagsBits
{
    u8 unk0 : 5;
    u8 rebuild : 1;
    u8 unk6 : 2;
};

struct Rec
{
    u8 filler0[0x18];
    s32 x;
    s32 y;
    s32 h;
    u8 filler24[0x7E - 0x24];
    u8 flags7E;
    u8 filler7F[0x81 - 0x7F];
    u8 flags81;
    u8 filler82[0xD8 - 0x82];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    struct Frame *frame;
    u8 fillerE8[0xEC - 0xE8];
    s16 slot;
};

struct RecPos
{
    u8 filler0[0x10];
    s32 x;
    s32 y;
    s32 h;
};

struct Actor
{
    u8 filler0[0x4];
    u32 unk4;
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct Rec *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct Actor *);
    void (*savedState)(struct Actor *);
    u8 filler54[0x68 - 0x54];
    void (*callback68)(struct Actor *);
    u32 link;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 flags77;
    u8 flags78;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s16 progress;
    u32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x92 - 0x90];
    u16 heightStep;
    s32 offsetA;
    s32 heightLimit;
    s32 unk9C;
    s32 angleOfs;
    s32 paramA4;
    s32 waitCount;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

struct CandMgr
{
    u8 filler0[0x58];
    struct Rec *slots[6];
    struct Rec *candA;
    struct Rec *candB;
    u8 filler78[0x80 - 0x78];
    struct Rec *cur;
    struct Rec *target;
};

extern struct CandMgr *gWorld;

s32 find_frame_entry(struct Actor *obj, s32 index);
s32 contact_face(struct Actor *a, struct Actor *b);
void obj_stop(void *obj);
void set_motion_offsets(struct Actor *obj, u32 value);
void finish_if_flagged(struct Actor *obj);
void finish_rebuild_checked(struct Actor *obj);
s32 distance_step(s32 x, s32 y, s32 z);
void arbitrate_candidates(struct Actor *obj);
u32 step_motion(struct Actor *obj);
void sprite_set_action(struct Actor *obj, u32 action, u32 arg, u32 extra);
void start_rebuild(struct Actor *obj);
void start_action_6_if_posed(struct Actor *obj);
void start_motion(struct Actor *obj);
void obj_list_remove(struct Actor *obj);
void obj_list_push_default(struct Actor *obj);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void wait_for_index(struct Actor *obj);
void countdown_then_move_2800(struct Actor *obj);
void finish_rebuild(struct Actor *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct Actor *owner);
void obj_finish_placement(struct Actor *obj);
s32 parabola_height(s32 t, s32 v, s32 s, s32 h);
u32 clamp_reflect_offsets(s32 *outA, s32 *outB, s32 lo, s32 hi, s32 v);
void timed_move_3000(struct Actor *obj);
void timed_move_3800(struct Actor *obj);
void state_stack_fall_step(struct Actor *obj);
s32 step_vector(u16 *outX, u16 *outY, s32 dx, s32 dy, s32 speed);
s32 move_step(u16 *outX, u16 *outY, u16 *outH, s32 dx, s32 dy, s32 dh, s32 speed);
void set_motion_speed_aux(struct Actor *obj, u32 value, s32 aux);

ASM_FUNC("asm/code_08087E1C/search_frame_pairs.s", u32 search_frame_pairs(struct Actor *a, struct Actor *b))

struct Rec *candidate_for_slot(struct Rec *obj)
{
    struct CandMgr *mgr = gWorld;

    if (mgr->target != NULL)
        return mgr->slots[obj->slot + 8];
    return NULL;
}

struct Rec *active_candidate(struct Rec *obj)
{
    struct CandMgr *mgr = gWorld;
    struct Rec *c = mgr->cur;

    if (c == obj)
        c = mgr->target;
    if (c != NULL && (c->flags7E & 6) == 2)
        return c;
    return NULL;
}

s32 height_from_frame_scale(struct Actor *obj)
{
    s32 v = obj->data->frame->k6;

    if (obj->pose != NULL)
    {
        s32 t = v * obj->pose->unk6 * obj->pose->unkA;
        if (t < 0)
            t += 0xFF;
        v = t >> 8;
    }
    else
    {
        v = v << 8;
    }
    v += obj->h;
    return v;
}

void copy_pos_to_active_candidate(struct Actor *obj)
{
    struct RecPos *p = (struct RecPos *)((u8 *)active_candidate(obj->data) + 8);

    p->x = obj->x;
    p->y = obj->y;
    p->h = obj->h;
}

void set_progress(struct Actor *obj, s32 value)
{
    obj->progress = value;
}

u32 tick_progress(struct Actor *obj)
{
    s16 *progress = &obj->progress;
    s32 value;
    u32 result;

    if (*progress > 0) {
        (*progress)--;
    }
    value = *progress;
    result = 1;
    if (value != 0) {
        result = 0;
    }
    return result;
}

u32 progress_is_zero(struct Actor *obj)
{
    if (obj->progress != 0)
        return 0;
    return 1;
}

void set_speed_from_delta(struct Actor *obj, s32 value)
{
    s32 len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(obj->dx * obj->dx + obj->dy * obj->dy + obj->dh * obj->dh);

    obj->motionSpeed = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(value << 12, len);
}

void sub_08087FCC(struct Actor *obj, s32 divisor)
{
    obj->motionSpeed = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, divisor);
}

u32 advance_height(struct Actor *obj)
{
    obj->heightStep += obj->offsetA;
    obj->h = obj->baseH + (s16)obj->heightStep;
    if (obj->heightLimit < obj->h)
        return 0;
    obj->h = obj->heightLimit;
    obj->offsetA = 0;
    return 1;
}

s32 parabola_height(s32 t, s32 v, s32 s, s32 h)
{
    if (s < 0)
    {
        s32 q = v * (0x4000 - t);
        s32 a;
        s32 b;
        s32 c;
        s32 d;

        if (q < 0)
            q += 0x3FFF;
        a = (q >> 14) - s;
        b = v - s;
        {
            s32 sq = s * s;

            if (sq < 0)
                sq += 0xFF;
            c = h - (sq >> 8);
        }
        {
            s32 e = b * b;

            if (e < 0)
                e += 0xFF;
            d = c - (e >> 8);
        }
        {
            s32 e = a * a;

            if (e < 0)
                e += 0xFF;
            return d + (e >> 8);
        }
    }
    else
    {
        s32 q = v * t;
        s32 a;
        s32 e;

        if (q < 0)
            q += 0x3FFF;
        a = (q >> 14) - s;
        e = a * a;
        if (e < 0)
            e += 0xFF;
        return h - (e >> 8);
    }
}

u32 clamp_reflect_offsets(s32 *outA, s32 *outB, s32 lo, s32 hi, s32 v)
{
    s32 flip = 0;
    s32 s1;
    s32 s2;

    if (v < lo)
    {
        if (v < hi)
        {
            v = lo + hi - v;
            flip = 1;
        }
        else
        {
            v = lo;
        }
    }
    else if (v < hi)
    {
        v = hi;
    }
    s1 = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)((v - hi) << 8);
    s2 = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)((v - lo) << 8);
    *outA = s2 + s1;
    *outB = s2;
    if (flip)
    {
        *outA = -*outA;
        *outB = -*outB;
    }
    {
        s32 r = v;

        if (r < 0)
            r += 0xFF;
        return r >> 8;
    }
}

s32 distance_step(s32 x, s32 y, s32 z)
{
    s32 a = (x << 1) + y;
    u32 r = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(a * a + ((z * y) << 3));
    s32 q = -(s32)r - a;

    return MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(q, y << 1);
}

s32 distance_step_for_height(struct Actor *obj)
{
    return distance_step(obj->dh, obj->offsetA, obj->heightLimit - obj->baseH);
}

s32 move_step(u16 *outX, u16 *outY, u16 *outH, s32 dx, s32 dy, s32 dh, s32 speed)
{
    s32 x4 = dx;
    s32 y4 = dy;
    s32 h4 = dh;
    s32 len;

    x4 = x4 / 4;
    y4 = y4 / 4;
    h4 = h4 / 4;
    if (x4 == 0 && y4 == 0 && h4 == 0)
    {
        *outX = h4;
        *outY = h4;
        *outH = h4;
        return 0;
    }
    len = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(x4 * x4 + y4 * y4 + h4 * h4);
    *outX = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(x4 * speed, len);
    *outY = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(y4 * speed, len);
    *outH = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(h4 * speed, len);
    return len;
}

s32 step_vector(u16 *outX, u16 *outY, s32 dx, s32 dy, s32 speed)
{
    s32 x4 = dx;
    s32 y4 = dy;
    s32 len;

    x4 = x4 / 4;
    y4 = y4 / 4;
    if (x4 == 0 && y4 == 0)
    {
        *outX = y4;
        *outY = y4;
        return 0;
    }
    len = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(x4 * x4 + y4 * y4);
    *outX = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(x4 * speed, len);
    *outY = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(y4 * speed, len);
    return len;
}

void mark_candidate_pair(struct Actor *obj)
{
    struct Rec *a = gWorld->candA;
    struct Rec *b = gWorld->candB;

    if ((a->flags7E & 6) == 2)
    {
        u8 *p = &a->flags81;
        *p |= 4;
    }
    if ((b->flags7E & 6) == 2)
    {
        u8 *p = &b->flags81;
        *p |= 4;
    }
    obj->unk4 = (u32)arbitrate_candidates;
}

void finish_step_and_place(struct Actor *obj)
{
    if ((u8)step_motion(obj) == 0)
    {
        set_slot_value(0x2E, -1);
        ((struct PoseFlags12Bit4 *)&obj->pose->flags12)->bit4 = 0;
        effect_spawn_at(0x33, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 1;
        obj->state = obj_finish_placement;
    }
}

void noop_state_080883dc(struct Actor *obj)
{
}

void timed_motion_done(struct Actor *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 1)
    {
        obj->pose->amount20 = 0x10;
        obj->state = obj->savedState;
        obj->savedState = 0;
    }
}

void place_part_sprite(struct Actor *obj)
{
    struct Pose *pose = obj->pose;

    if (pose->frame->count != 0) {
        u16 count = pose->frame->count;

        effect_spawn_at(count, pose->baseX / 0x100, pose->baseY / 0x100, pose->baseH / 0x100, (struct Actor *)&pose->filler8);
    }
    pose->unk54 = 0;
    obj->unk4 = 0;
}

void stop_unless_flagged(struct Actor *obj)
{
    struct Pose *pose = obj->pose;
    u8 f = pose->status111 & 2;

    if (f == 0)
    {
        obj_stop(pose->filler8);
        obj->unk4 = f;
    }
}

void link_index_state(struct Actor *obj)
{
    u32 index = obj->index;

    if (index == 0)
    {
        if (obj->callback68 != NULL)
            obj->callback68(obj);
        obj->link = index;
    }
}

void start_rebuild(struct Actor *obj)
{
    step_motion(obj);
    if (obj->flags & 0x20)
    {
        u32 m = obj->mode & 6;

        if (m == 2 || m == 4)
        {
            if (obj->data->slot == -1)
                sprite_set_action(obj, 8, 0x2000, 0);
            else
                sprite_set_action(obj, 8, 0x204D, 0);
            ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 1;
        }
        obj->state = finish_rebuild;
    }
}

ASM_FUNC("asm/code_08087E1C/step_motion.s", u32 step_motion(struct Actor *obj))

u32 step_motion_checked(struct Actor *obj)
{
    s32 p;
    s32 h;

    p = obj->progress + (obj->motionStart * obj->motionSpeed) / 0x100;
    obj->timer = obj->progress;
    obj->progress = p;
    if (p <= 0x3FFF)
    {
        obj->x = ((obj->targetX >> 16) * p + (s16)obj->targetX * (0x4000 - p)) / 0x40;
        obj->y = ((obj->targetY >> 16) * p + (s16)obj->targetY * (0x4000 - p)) / 0x40;
        obj->h = parabola_height(p, obj->offsetA, obj->heightLimit, (s16)obj->heightStep << 8);
        return 1;
    }
    obj->x = ((struct HalfPair *)&obj->targetX)->hi << 8;
    obj->y = ((struct HalfPair *)&obj->targetY)->hi << 8;
    h = parabola_height(p, obj->offsetA, obj->heightLimit, (s16)obj->heightStep << 8);
    obj->h = h;
    if (h < 0)
    {
        obj->x = ((struct HalfPair *)&obj->targetX)->hi << 8;
        obj->y = ((struct HalfPair *)&obj->targetY)->hi << 8;
        obj->h = 0;
        ((struct FlagsBits *)&obj->flags)->rebuild = 1;
        return 0;
    }
    ((struct FlagsBits *)&obj->flags)->rebuild = 0;
    return 1;
}

u32 step_motion_scaled(struct Actor *obj)
{
    s16 *progress;
    s32 p;
    s32 t;
    s32 dx;
    s32 dy;
    s32 dh;
    s32 hl;
    s32 oa;

    progress = &obj->progress;
    p = *progress + (obj->motionStart * obj->motionSpeed) / 0x100;
    obj->timer = *progress;
    *progress = p;
    if (p <= 0x3FFF && obj->motionStart != 0)
    {
        hl = obj->heightLimit;
        oa = obj->offsetA;
        t = ((hl * p) / 0x100 * p) / 0x40 + oa * p;
        dx = (obj->targetX >> 16) - (s16)obj->targetX;
        dy = (obj->targetY >> 16) - (s16)obj->targetY;
        dh = (obj->targetH >> 16) - (s16)obj->targetH;
        obj->x = ((s16)obj->targetX << 8) + (dx * t) / 0x4000;
        obj->y = ((s16)obj->targetY << 8) + (dy * t) / 0x4000;
        obj->h = ((s16)obj->targetH << 8) + (dh * t) / 0x4000;
        return 1;
    }
    obj->x = ((struct HalfPair *)&obj->targetX)->hi << 8;
    obj->y = ((struct HalfPair *)&obj->targetY)->hi << 8;
    obj->h = ((struct HalfPair *)&obj->targetH)->hi << 8;
    *((u8 *)obj + 0x79) |= 0x20;
    return 0;
}

u32 step_motion_d(struct Actor *obj)
{
    s32 t;
    s32 v;

    t = obj->progress;
    t += (obj->motionStart * obj->motionSpeed) / 0x800;
    obj->timer = obj->progress;
    obj->progress = t;

    v = (obj->targetX >> 16) * t + (s16)obj->targetX * (0x800 - t);
    if (v < 0)
        v += 7;
    obj->x = v >> 3;

    v = (obj->targetY >> 16) * t + (s16)obj->targetY * (0x800 - t);
    if (v < 0)
        v += 7;
    obj->y = v >> 3;

    v = (obj->targetH >> 16) * t + (s16)obj->targetH * (0x800 - t);
    if (v < 0)
        v += 7;
    obj->h = v >> 3;

    if (t <= 0x7FF)
        return 1;
    return 0;
}

void set_motion_start(struct Actor *obj, u32 value)
{
    s32 dx = (obj->targetX >> 16) - (s16)obj->targetX;
    s32 dy = (obj->targetY >> 16) - (s16)obj->targetY;
    s32 dh = (obj->targetH >> 16) - (s16)obj->targetH;
    s32 len = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy + dh * dh);
    s32 r;

    if (len != 0)
        r = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)((value << 6) + len / 2, len);
    else
        r = 0x4000;
    if (r > 0x4000)
        r = 0x4000;
    obj->motionStart = r;
    ((struct FlagsBits *)&obj->flags)->rebuild = 0;
}

ASM_FUNC("asm/code_08087E1C/set_motion_offsets.s", void set_motion_offsets(struct Actor *obj, u32 value))
