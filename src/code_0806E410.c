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
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
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

/* Once the current pose is done (pose flag 8): lets go of the partner sprite (obj->kind), picks
   one of four sprite actions 0x4126-0x4129 by param9C and paramA0, lifts obj 0x30 pixels onto its
   owner's line with an upward speed of 3 (in phase), saves the top 3 bits of dir in dx and faces
   it 3, puts it on the list and hands over to obj_drift_up_until_height. */
void obj_pose_release_partner(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        register u32 action asm("r2");

        obj_detach_active(obj->kind->pose);
        ((struct ActorFlagBits *)&obj->kind->flags)->bit3 = 0;
        if (obj->paramA0 == 0)
        {
            if (obj->param9C == 0)
                action = 0x4126;
            else
                action = 0x4128;
        }
        else
        {
            if (obj->param9C == 0)
                action = 0x4127;
            else
                action = 0x4129;
        }
        sprite_set_action(obj, 0, action, 0);
        obj->posY = obj->owner->part->targetY;
        obj->posH += 0x3000;
        obj->dh = 0;
        obj->phase = 0x300; /* upward speed, slowed by 0x33 a frame */
        obj->dx = obj->dir >> 5; /* obj_aim_at_owner puts it back */
        ((struct ActorFlagBits *)&obj->flags)->bit0 = 0;
        obj->dir = (obj->dir & 0x1F) | 0x60;
        obj_list_push_default(obj);
        obj->handler58 = obj_owner_callback_step;
        obj->handler60 = obj_lookup_slot_step;
        obj->state = obj_drift_up_until_height;
    }
}

void obj_drift_up_until_height(struct ActorObj *obj)
{
    obj->posX += -0x300;
    obj->posH += obj->phase;
    obj->phase -= 0x33;
    obj->pose->unkC += 0x100;
    if (obj->posH <= 0x3000)
    {
        set_slot_value(0x8B, -1);
        set_slot_value(0x6C, -1);
        effect_spawn_at(0x1FC7, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->posH = 0x3000;
        obj->timer = 10;
        obj->state = obj_timer_drift_left_exit;
    }
}

/* Drifts the object off the left edge at 3 px a frame, spinning its sprite and spawning effect 0x1FDC
   below it every 10 frames; an idle owner is pushed along with it. Once 64 px past the edge it stops
   song 0x6C, starts song 0x40 (set_slot_value queues a song; -1 keeps the volume), spawns effect 0x1FCF
   and hands over to obj_aim_at_owner. */
void obj_timer_drift_left_exit(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        effect_spawn_at(0x1FDC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 0x30, obj);
        obj->timer = 10;
    }
    obj->byte75 = obj->owner->byte75 + 5;
    obj->posX += -0x300;
    obj->pose->unkC += 0x100;
    if (obj->owner->state == actor_side_walk_idle)
    {
        obj->owner->posX += -0x300;
    }
    if (obj->posX / 0x100 <= -0x40)
    {
        sound_stop_song(0x6C);
        set_slot_value(0x40, -1);
        effect_spawn_at(0x1FCF, 0, obj->kind->kind->baseY / 0x100, obj->kind->kind->baseH / 0x100 - 0x30, obj);
        obj->state = obj_aim_at_owner;
    }
}

/* Sets up the throw of the owner onto its part's target: moves the low 3 bits of dx into the top
   3 bits of dir, takes the object off the list, saves param9C in phase (obj_arc_land puts it back),
   records the owner's start (dx, dy, dh in pixels) and the 8.8 distance to the target, and sizes the
   arc: one frame per 0x380 of distance (sqrt from the IWRAM math routines) and a curve param9C that
   peaks 0x3000 above the start halfway through. Then hands over to obj_arc_land. */
void obj_aim_at_owner(struct ActorObj *obj)
{
    s16 *dxField;
    u8 *dirField;
    s32 *savedParam;
    s32 *curve;
    s32 deltaX;
    s32 deltaY;
    s32 frames;
    s32 halfFrames;
    s32 steep;
    s32 facing;

    dxField = &obj->dx;
    facing = (u8)*dxField;
    dirField = &obj->dir;
    facing = facing << 5;
    *dirField = (*dirField & 0x1F) | facing;
    ((struct ActorFlagBits *)&obj->flags)->bit0 = 0;
    obj_list_remove(obj);
    savedParam = &obj->phase;
    curve = &obj->param9C;
    *savedParam = *curve;
    obj->targetX = obj->owner->part->targetX;
    obj->targetY = obj->owner->part->targetY;
    obj->dx = obj->owner->posX / 0x100;
    obj->dy = obj->owner->posY / 0x100;
    obj->dh = obj->owner->posH / 0x100;
    deltaX = obj->targetX - (obj->dx << 8);
    deltaY = obj->targetY - (obj->dy << 8);
    /* gMathRoutines + 0 is the square root */
    frames = ((s32 (*)(s32))gMathRoutines)(deltaX * deltaX + deltaY * deltaY) / 0x380;
    halfFrames = frames / 2;
    steep = 0x3000 / (halfFrames * halfFrames);
    if (steep == 1)
    {
        steep = 2;
    }
    *curve = -steep;
    obj->paramA0 = frames;
    obj->paramA4 = deltaX;
    obj->paramA8 = deltaY;
    obj->timer = 0;
    obj->state = obj_arc_land;
}

/* State set up by obj_aim_at_owner: carries the owner along a parabola from its start (dx, dy, dh in pixels)
   by (paramA4, paramA8) over paramA0 frames, with param9C as the (negative) curve; once past the apex and
   back on the ground, the owner lands, both sprites get their landing actions and this object walks to its
   part's target, then waits in obj_sync_child_reset. */
void obj_arc_land(struct ActorObj *obj)
{
    struct ActorObj *owner;
    s32 fromApex;
    s32 drop;
    s32 apexH;

    owner = obj->owner;
    owner->posX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    owner->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    fromApex = (s16)obj->timer - obj->paramA0 / 2;
    drop = (fromApex * fromApex) * obj->param9C;
    apexH = ((s16)obj->dh << 8) + 0x3000;
    owner->posH = drop + apexH;
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->owner->posH <= 0)
        {
            /* landed: restore the value obj_aim_at_owner saved in phase */
            obj->owner->posH = obj->owner->part->targetH;
            obj->param9C = obj->phase;
            obj->owner->state = 0;
            partner_link_update(obj, (u32)obj->owner, 0xFF);
            sprite_entry_update(obj->kind->pose);
            if (obj->param9C == 0)
            {
                sprite_set_action(obj, 0, 0x4112, 0);
                sprite_set_action(obj, 0xE, 0, 0);
                sprite_set_action(obj->kind, 0xE, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 0, 0x4114, 0);
                sprite_set_action(obj, 0xE, 0, 0);
                sprite_set_action(obj->kind, 0xE, 0, 0);
            }
            obj->pose->unkC = 0;
            /* place this object and its kind at x = 300 on the part's line */
            obj->posX = 0x12C00;
            obj->posY = obj->part->targetY;
            obj->posH = obj->part->targetH;
            obj->kind->posX = 0x12C00;
            obj->kind->posY = obj->posY;
            obj->kind->posH = obj->posH;
            ((struct ActorFlagBits *)&obj->kind->flags)->bit3 = 1;
            obj->byte74 = obj->owner->byte74;
            obj->kind->byte74 = obj->owner->byte74;
            obj->byte75 = obj->owner->byte75;
            obj->kind->byte75 = obj->owner->byte75 + 1;
            /* then move to the part's target */
            obj->targetX = obj->part->targetX;
            obj->targetY = obj->part->targetY;
            obj->phase = obj->part->targetH;
            obj->motionSpeed = 0x180;
            ((struct ActorModeBits *)&obj->flags)->bit5 = 1;
            obj->motionStart = 0;
            start_motion(obj);
            obj->state = obj_sync_child_reset;
        }
    }
}
