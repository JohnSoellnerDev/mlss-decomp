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
    u8 filler0[0x70];
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

void set_motion_speed_aux(struct Actor *obj, u32 value, s32 aux)
{
    s32 dx = (obj->targetX >> 16) - (s16)obj->targetX;
    s32 dy = (obj->targetY >> 16) - (s16)obj->targetY;
    s32 dz = (obj->targetH >> 16) - (s16)obj->targetH;
    s32 len = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy + dz * dz);
    s32 len2 = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)((((len << 1) * aux) << 8) + value * value);
    s32 r5;
    s32 speed;

    obj->heightLimit = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)((r5 = len2 - value) << 8, len2 + value);
    obj->offsetA = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(value << 9, len2 + value);
    if (aux != 0)
    {
        if (r5 != 0)
            speed = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)((aux << 14) + r5 / 2, r5);
        else
            speed = 0x4000;
    }
    else
    {
        if (len != 0)
            speed = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)((value << 6) + len / 2, len);
        else
            speed = 0x4000;
    }
    if (speed > 0x600)
        speed = 0x600;
    obj->motionStart = speed;
    ((struct FlagsBits *)&obj->flags)->rebuild = 0;
}

void set_motion_from_value(struct Actor *obj, s32 value)
{
    if (value != 0)
    {
        obj->progress = 0;
        obj->motionSpeed = 0x100;
        obj->motionStart = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(value / 2 + 0x4000, value);
    }
}

u32 candidates_advance_progress(struct Actor *obj)
{
    u16 *prog = (u16 *)&obj->progress;
    s32 d = obj->motionStart * obj->motionSpeed;
    s32 v;

    if (d < 0)
        d += 0xFF;
    d = d >> 8;
    v = d + *prog;
    *prog = v;
    if ((v << 16) <= 0x3FFF0000)
        return 1;
    return 0;
}

void set_step_and_offsets(struct Actor *obj, u32 value, u16 step)
{
    obj->heightStep = step;
    set_motion_offsets(obj, value);
}

void place_targets(struct Actor *obj, u32 x, u32 y, u32 h, s32 speed)
{
    obj->targetX = (x << 16) | ((obj->baseX / 0x100) & 0xFFFF);
    obj->targetY = (y << 16) | ((obj->baseY / 0x100) & 0xFFFF);
    obj->targetH = (h << 16) | ((obj->baseH / 0x100) & 0xFFFF);
    obj->heightLimit = 0;
    obj->progress = 0;
    if (speed > 0)
        obj->motionSpeed = speed;
}

u32 state_stack_advance_progress(struct Actor *obj)
{
    s16 *prog = &obj->progress;
    s32 cur = *prog;
    s32 d = obj->motionStart * obj->motionSpeed;
    s32 np;

    if (d < 0)
        d += 0xFF;
    np = cur + (d >> 8);
    obj->timer = *(u16 *)prog;
    *prog = np;
    if (np <= 0x3FFF)
        return 1;
    ((struct FlagsBits *)&obj->flags)->rebuild = 1;
    return 0;
}

void finish_rebuild(struct Actor *obj)
{
    if (obj->flags & 0x20)
    {
        u32 m = obj->mode & 6;

        if (m == 2 || m == 4)
        {
            sprite_set_action(obj, 0, 0xFFFFFFFF, 0);
            ((struct PoseFlags11Bits *)&obj->pose->flags11)->mirror = 0;
            ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 0;
        }
        obj->state = obj->savedState;
        obj->savedState = 0;
    }
}

void start_action_5(struct Actor *obj)
{
    u32 m = obj->mode & 6;

    if (m == 2 || m == 4)
    {
        s32 t;

        if (obj->data->slot == -1)
            sprite_set_action(obj, 5, 0x2000, 0);
        else
            sprite_set_action(obj, 5, 0x204D, 0);
        ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 1;
    }
    obj->state = start_rebuild;
}

void start_action_4(struct Actor *obj)
{
    u32 m = obj->mode & 6;

    if (m == 2 || m == 4)
    {
        s32 t;

        if (obj->data->slot == -1)
            sprite_set_action(obj, 4, 0x2000, 0);
        else
            sprite_set_action(obj, 4, 0x204D, 0);
        ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 1;
    }
    obj->savedState = obj->state;
    obj->state = start_action_6_if_posed;
}

void enter_finish_flag(struct Actor *obj)
{
    obj->savedState = obj->state;
    obj->state = finish_if_flagged;
}

void enter_finish_rebuild(struct Actor *obj)
{
    obj->savedState = obj->state;
    obj->state = finish_rebuild_checked;
}

void start_action_6_if_posed(struct Actor *obj)
{
    if (obj->pose->flags12 & 8)
    {
        u32 m = obj->mode & 6;

        if (m == 2 || m == 4)
        {
            if (obj->data->slot == -1)
                sprite_set_action(obj, 6, 0x2000, 0);
            else
                sprite_set_action(obj, 6, 0x204D, 0);
            ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 1;
        }
        obj->state = start_rebuild;
    }
}

void finish_if_flagged(struct Actor *obj)
{
    step_motion(obj);
    if (obj->flags & 0x20)
    {
        obj->state = obj->savedState;
        obj->savedState = 0;
    }
}

void finish_rebuild_checked(struct Actor *obj)
{
    step_motion(obj);
    if (obj->flags & 0x20)
    {
        u32 m = obj->mode & 6;

        if (m == 2 || m == 4)
        {
            sprite_set_action(obj, 0, 0xFFFFFFFF, 0);
            ((struct PoseFlags11Bits *)&obj->pose->flags11)->mirror = 0;
            ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 0;
        }
        obj->state = obj->savedState;
        obj->savedState = 0;
    }
}
