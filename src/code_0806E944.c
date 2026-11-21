#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0x11E - 0xEE];
    u16 unk11E;
};

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x28 - 0x1C];
    struct ActorPart *part;
    struct ActorObj *owner;
    struct ActorObj *kind;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    u8 filler5C[0x60 - 0x5C];
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 phase;
    u8 filler90[0x9C - 0x90];
    s32 param9C;  /* jump arc: (negative) curvature of the height parabola */
    s32 paramA0;  /* jump arc: flight time in frames */
    s32 paramA4;  /* jump arc: x distance to cover (24.8) */
    s32 side;     /* jump arc: y distance to cover (24.8) */
    u16 timer;    /* jump arc: frames flown */
    s16 dx;       /* jump arc: start x in pixels */
    s16 dy;       /* jump arc: start y in pixels */
    s16 dh;       /* jump arc: start height in pixels */
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ActorFlagBits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 high : 4;
};

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
u32 obj_lookup_part_to_drift(struct ActorObj *obj, u32 arg);
void obj_wave_drift(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_timer_sprite_move_1(struct ActorObj *obj);
void side_walk_obj_index_zero_action4(struct ActorObj *obj);
void obj_timer_sprite_move_2(struct ActorObj *obj);
void obj_timer_sprite_move_3(struct ActorObj *obj);
void obj_timer_sprite_countdown(struct ActorObj *obj);
void obj_timer_clear_slot_action6(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_timer_action5_setup(struct ActorObj *obj);
void obj_timer_exit_left(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 mt19937_next(void);
void obj_action2_then_fall_d70c(struct ActorObj *obj);
void obj_action2_then_fall_d408(struct ActorObj *obj);
void obj_fall_and_launch(struct ActorObj *obj);
u32 obj_lookup_part_pick_state(struct ActorObj *obj, u32 arg);
void obj_wait_30_when_index_zero(struct ActorObj *obj);
void obj_anim5_when_index_zero(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
void actor_side_walk_steps(struct ActorObj *obj);
void obj_setup_partner_pose(struct ActorObj *obj);
void obj_timer_clear_slot_action6_alt(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_reset_when_index_zero(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_detach_active(struct ActorPose *pose);
void sprite_entry_update(struct ActorPose *pose);
void obj_owner_release_motion(struct ActorObj *obj);
void obj_move_to_base_right(struct ActorObj *obj);
void obj_owner_callback_step(struct ActorObj *obj);
void obj_lookup_slot_step(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void obj_drift_up_until_height(struct ActorObj *obj);
void obj_aim_at_owner(struct ActorObj *obj);
void obj_prepare_when_index_clear(struct ActorObj *obj);
void obj_arc_land(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_finish_if_active(struct ActorObj *obj);
void obj_idle_when_active(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_sync_child_reset(struct ActorObj *obj);

struct ActorCands
{
    u8 filler0[0x70];
    u8 *cand70;
    u8 *cand74;
};

extern struct ActorCands *gWorld;

/* One frame of the jump set up by obj_arc_start: x and y move linearly from the start over
 * paramA0 frames, the height follows a parabola peaking 0x28 px above the start at half time.
 * obj->kind (the shadow/second part) follows. Past the peak, landing (height <= 0) ends the jump. */
void obj_arc_step(struct ActorObj *obj)
{
    s32 x;
    s32 h;
    s32 peakH;

    x = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + (obj->dx << 8);
    obj->posX = x;
    obj->posY = (obj->side * (s16)obj->timer) / obj->paramA0 + (obj->dy << 8);
    h = (((s16)obj->timer - obj->paramA0 / 2) * ((s16)obj->timer - obj->paramA0 / 2)) * obj->param9C;
    peakH = (obj->dh << 8) + 0x2800;
    h += peakH;
    obj->posH = h;
    obj->kind->posX = x;
    obj->kind->posY = obj->posY;
    obj->kind->posH = obj->posH;
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            obj->state = wait_for_index;
        }
    }
}

/*
 * Launches obj on a parabolic jump from where it stands to its part's target: spawns the take-off
 * effect, picks the jump animation (param9C chooses the variant), copies the owner's layer bytes,
 * then sets up obj_arc_step. dx/dy/dh hold the start in pixels, paramA4/side the distance to cover,
 * paramA0 the flight time (one frame per 2.5 pixels) and param9C the (negative) curvature that makes
 * the arc peak 0x28 pixels above the start at half time.
 */
void obj_arc_start(struct ActorObj *obj)
{
    u8 *kindFlags;
    struct ActorPart *part;
    s32 *curvature;
    s32 *targetH;
    s32 *targetX;
    s32 *targetY;
    s32 duration;
    s32 halfTime;
    s32 steepness;
    s32 spanX;
    s32 spanY;

    effect_spawn_at(0x1FCF, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 0x30, obj);
    sprite_entry_update(obj->kind->pose);
    kindFlags = &obj->kind->flags;
    *kindFlags |= 8;
    if (obj->param9C == 0)
    {
        sprite_set_action(obj, 0, 0x4112, 0);
        sprite_set_action(obj, 1, 0, 0);
        sprite_set_action(obj->kind, 1, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 0, 0x4114, 0);
        sprite_set_action(obj, 1, 0, 0);
        sprite_set_action(obj->kind, 1, 0, 0);
    }
    obj->pose->unkC = 0;
    obj->byte74 = obj->owner->byte74;
    obj->kind->byte74 = obj->owner->byte74;
    obj->byte75 = obj->owner->byte75;
    obj->kind->byte75 = obj->owner->byte75 + 1;

    /* start 0x30 pixels lower, but not below the ground */
    obj->posH -= 0x3000;
    if (obj->posH < 0)
    {
        obj->posH = 0;
    }

    targetX = &obj->targetX;
    part = obj->part;
    *targetX = part->targetX;
    targetY = &obj->targetY;
    *targetY = part->targetY;
    targetH = &obj->phase;
    *targetH = part->targetH;

    obj->dx = obj->posX / 0x100;
    obj->dy = obj->posY / 0x100;
    obj->dh = obj->posH / 0x100;
    spanX = *targetX - (obj->dx << 8);
    spanY = *targetY - (obj->dy << 8);
    /* gMathRoutines starts with the integer square root */
    duration = ((s32 (*)(s32))gMathRoutines)(spanX * spanX + spanY * spanY) / 0x280;
    halfTime = duration / 2;
    steepness = 0x2800 / (halfTime * halfTime);
    if (steepness == 1)
    {
        steepness = 2;
    }
    curvature = &obj->param9C;
    *curvature = -steepness;
    obj->paramA0 = duration;
    obj->paramA4 = spanX;
    obj->side = spanY;
    obj->timer = 0;
    obj->state = obj_arc_step;
}

void obj_start_move_right(struct ActorObj *obj)
{
    u8 *flags;

    obj->byte74 = obj->owner->byte74;
    obj->byte75 = obj->owner->byte75 - 5;
    sprite_set_action(obj, 3, 0, 0);
    obj->targetX = obj->owner->part->targetX + 0x1800;
    obj->targetY = obj->owner->part->targetY;
    obj->phase = obj->baseH;
    obj->motionSpeed = 0x199;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x99, -1);
    obj->state = obj_prepare_when_index_clear;
}

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

/*
 * Once obj's pose is done and its timer has run out: takes obj off the active list, hands the
 * owner back to actor_side_walk_idle (after its handler68), plays action 6, spawns effect 0x1EC0 and
 * a companion object 0x4111 (kept in obj->kind) at the owner's part, which copies obj's layer bytes
 * and flag bits and removes itself when its pose ends. obj then moves to 0x40 pixels right of the
 * part and waits in obj_idle_when_active.
 */
void obj_wait_then_spawn_child(struct ActorObj *obj)
{
    struct ActorObj *companion;

    if (obj->pose->flags & 8)
    {
        if ((s16)--obj->timer > 0)
        {
            return;
        }
        obj_list_remove(obj);
        if (obj->owner->handler68 != 0)
        {
            obj->owner->handler68(obj->owner);
        }
        obj->owner->state = actor_side_walk_idle;
        sprite_set_action(obj, 6, 0, 0);
        effect_spawn_at(0x1EC0, obj->owner->part->targetX / 0x100, obj->owner->part->targetY / 0x100,
                        obj->owner->part->targetH / 0x100, obj);
        /* pose->unk11 bit 6 is passed on as the spawn's facing */
        companion = obj_spawn(obj->part, 0x4111, obj->owner->part->targetX / 0x100,
                              obj->owner->part->targetY / 0x100, obj->owner->part->targetH / 0x100,
                              ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->kind = companion;
        companion->byte74 = obj->byte74;
        companion->byte75 = obj->byte75;
        ((struct ActorFlagBits *)&companion->flags)->bit0 = 1;
        companion->state = obj_finish_if_active;
        ((struct ObjFlags77 *)companion)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)companion)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        companion->handler68 = on_arrive;
        companion->unk6C = obj->unk6C;
        companion->owner = obj->owner;
        gUnk_03000E3C = effect_from_table_obj(companion, 0x173E);
        companion->handler60 = 0;

        /* walk to 0x40 pixels right of the owner's part */
        obj->targetX = obj->owner->part->targetX + 0x4000;
        obj->targetY = obj->owner->part->targetY;
        obj->phase = obj->baseH;
        obj->motionSpeed = 0x199;
        ((struct ActorModeBits *)&obj->flags)->bit5 = 1;
        obj->motionStart = 0;
        start_motion(obj);
        set_slot_value(0x98, -1);
        obj->state = obj_idle_when_active;
    }
}
