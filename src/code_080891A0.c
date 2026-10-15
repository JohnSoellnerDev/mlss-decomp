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
    u8 filler54[0x5C - 0x54];
    void (*handler5C)(struct Actor *);
    void (*handler60)(struct Actor *);
    u8 filler64[0x68 - 0x64];
    void (*callback68)(struct Actor *);
    u32 link;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 filler75[0x76 - 0x75];
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

extern struct CandMgr *gUnk_03000FD8;

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

void setup_motion_from_owner(struct Actor *obj)
{
    struct Actor *owner = obj->owner;

    obj->targetX = owner->data->targetX;
    obj->targetY = owner->data->targetY;
    obj->targetH = obj->baseH;
    ((struct FlagsBits *)&obj->flags)->rebuild = 1;
    obj->motionSpeed = 0x200;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = obj->unk9C;
    obj_list_push_default(obj);
    sprite_set_action(obj, 7, 0, 0);
    ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 0;
    sound_stop_song(0x81);
    set_slot_value(0x83, -1);
    obj->state = timed_move_3000;
}

void timed_move_3000(struct Actor *obj)
{
    u32 index = obj->index;

    if (index == 0)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            obj_list_remove(obj);
            obj->targetX = obj->baseX + 0x3000;
            obj->targetY = obj->baseY;
            obj->targetH = obj->baseH;
            ((struct FlagsBits *)&obj->flags)->rebuild = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 2, 0, 0);
            ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 0;
            obj->state = wait_for_index;
        }
    }
}

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Actor *obj_spawn(void *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void slide_x_timed(struct Actor *obj);
void approach_contact(struct Actor *obj);
void approach_slot(struct Actor *obj);
void on_arrive(struct Actor *obj);
void on_arrive_if_posed(struct Actor *obj);

void spawn_pose_child(struct Actor *obj)
{
    struct Actor *child;

    child = obj_spawn(obj->data, 0x4004, obj->baseX / 0x100 - 0x20, obj->baseY / 0x100,
                      obj->baseH / 0x100 + 0xE, ((u32)obj->pose->flags11 << 25) >> 31, 1);
    obj->child = child;
    ((struct Flags78Bits *)&child->flags78)->kind = 1;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
    child->state = slide_x_timed;
    child->callback68 = on_arrive;
    child->link = obj->link;
    child->owner = obj->owner;
    obj_list_push_default(child);
    child->handler5C = approach_contact;
    child->handler60 = approach_slot;
    child->dx = 0xFF00;
    child->timer = 0;
    child->unk9C = obj->unk9C;
    child->angleOfs = obj->angleOfs;
    child->paramA4 = obj->paramA4;
    child->waitCount = obj->waitCount;
    child->tag = 0xFF;
    effect_spawn_at(0xBDA, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    sprite_set_action(obj, 3, 0, 0);
    obj->link = 0;
    obj->state = on_arrive_if_posed;
}

void begin_countdown(struct Actor *obj)
{
    struct Actor *owner = obj->owner;

    obj->state = countdown_then_move_2800;
    obj->targetX = owner->data->targetX;
    obj->targetY = owner->data->targetY;
    obj->targetH = obj->baseH;
    ((struct FlagsBits *)&obj->flags)->rebuild = 1;
    obj->motionSpeed = obj->angleOfs << 8;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 0x10;
    sprite_set_action(obj, 4, 0, 0);
    ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 0;
    obj->state = countdown_then_move_2800;
}

void countdown_then_move_2800(struct Actor *obj)
{
    u32 index = obj->index;

    if (index == 0)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sound_stop_song(0x81);
            obj_list_remove(obj);
            obj->targetX = obj->baseX + 0x2800;
            obj->targetY = obj->baseY;
            obj->targetH = obj->baseH;
            ((struct FlagsBits *)&obj->flags)->rebuild = 1;
            obj->motionSpeed = 0x400;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 2, 0, 0);
            ((struct PoseFlagsBits *)&obj->pose->flags12)->mode = 0;
            obj->state = wait_for_index;
        }
    }
}

void start_jump(struct Actor *obj)
{
    u32 index = obj->index;

    if (index == 0)
    {
        struct Actor *owner = obj->owner;

        obj->targetX = owner->data->targetX + 0x3000;
        obj->targetY = owner->data->targetY;
        obj->targetH = obj->baseH;
        ((struct FlagsBits *)&obj->flags)->rebuild = 1;
        obj->motionSpeed = index;
        obj->motionStart = obj->paramA4;
        start_motion(obj);
        obj->dh = 0x300;
        sprite_set_action(obj, 5, 0, 0);
        sound_stop_song(0x81);
        set_slot_value(0x84, -1);
        obj->state = state_stack_fall_step;
    }
}

void state_stack_fall_step(struct Actor *obj)
{
    struct Rec *data = obj->data;
    u16 *dh = (u16 *)&obj->dh;
    u32 index;

    *dh = *dh - 0x5E;
    obj->h += obj->dh;
    if (obj->h < data->targetH)
        obj->h = data->targetH;
    index = obj->index;
    if (index == 0)
    {
        struct Actor *owner = obj->owner;

        *dh = index;
        obj->h = obj->data->targetH;
        obj->targetX = owner->data->targetX;
        obj->targetY = owner->data->targetY;
        obj->targetH = obj->baseH;
        ((struct FlagsBits *)&obj->flags)->rebuild = 1;
        obj->motionSpeed = obj->angleOfs << 8;
        obj->motionStart = index;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        set_slot_value(0x81, -1);
        obj->state = timed_move_3800;
    }
}
