#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x04];
    u16 unk4;
    u8 filler6[0x0C - 0x06];
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
    s16 unkEC;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gWorld;

struct Flags77Bits
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags79Bits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ChainNode
{
    struct ChainNode *next;
    s32 x;
    s32 y;
    s32 h;
};

extern u16 gUnk_03000E80[];
extern u16 gUnk_03000E98[];

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
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    u32 (*handler58)();
    s32 (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 field6C;
    u8 filler70[0x74 - 0x70];
    u8 group;
    u8 depth;
    u8 filler76[0x77 - 0x76];
    u8 flags77;
    u8 flags78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
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

/* flags79 bit 5: start_motion takes targetX/Y/H as an absolute point (else as an offset from
   posX/Y/H) and clears the bit again */
#define MOTION_TARGET_ABSOLUTE 0x20
/* pose->flags bit 3: the current animation has finished */
#define POSE_ANIM_DONE 8

u32 sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_link_slot(struct ActorObj *obj);
u32 mt19937_next(void);
void anim_bit200_step(struct ActorPart *part, u32 value);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_list_remove(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void lookup_then_slide(struct ActorObj *obj);
void spawn_child_and_arm(struct ActorObj *obj);
void spawn_slot_sprite(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void start_motion(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void finish_when_index_zero(struct ActorObj *obj);
void wait_timer_then_place(struct ActorObj *obj);
void link_owner_then_wait(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_detach_active(struct ActorPose *pose);
void sprite_entry_update(struct ActorPose *pose);
void on_arrive(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void wait_counter_then_finish(struct ActorObj *obj);
void notify_when_index_zero(struct ActorObj *obj);

void reset_height_motion(struct ActorObj *obj);
void spawn_child_and_place(struct ActorObj *obj);
void place_and_move_fixed(struct ActorObj *obj);
void timer_spawn_grandchild(struct ActorObj *obj);
void obj_fall_and_split(struct ActorObj *obj);
void anim_track_slot(struct ActorObj *obj);
void move_by_speed_xy(struct ActorObj *obj);
void move_by_speed_then_land(struct ActorObj *obj);
void scroll_obj_out(struct ActorObj *obj);
u32 roll_action_chance(struct ActorObj *obj, struct ActorObj *target);
u32 bind_part_then_step(struct ActorObj *obj, u32 part);
void scroll_obj_out_and_free(struct ActorObj *obj);
void timer_then_step_2b35(struct ActorObj *obj);
void timer_30_when_idle(struct ActorObj *obj);
void count_dx_and_notify(struct ActorObj *obj);
u32 lookup_part_b_then_step(struct ActorObj *obj);
void remove_child_and_move(struct ActorObj *obj);
void timer_then_anim5(struct ActorObj *obj);
s32 arm_slot_anim_2f_then_follow(struct ActorObj *obj);
s32 arm_slot_anim_2f_then_step_2e0d(struct ActorObj *obj);
void slot_anim_2f_then_follow(struct ActorObj *obj);
void slot_anim_2f_then_step_2e0d(struct ActorObj *obj);
void slot_follow_move(struct ActorObj *obj);
void slide_to_anchor_start(struct ActorObj *obj);
void action16_when_index_zero(struct ActorObj *obj);
void begin_fx_wait(struct ActorObj *obj);
void advance_offset_then_notify(struct ActorObj *obj);
void pose_action15_then_exit(struct ActorObj *obj);
void spawn_child_at_base(struct ActorObj *obj);
void timer_then_target_from_owner(struct ActorObj *obj);
void prepare_slide_timing(struct ActorObj *obj);
void slide_step(struct ActorObj *obj);
void wait_timer_then_settle(struct ActorObj *obj);
void start_slide_to_owner(struct ActorObj *obj);
void slide_step_with_pose(struct ActorObj *obj);
void hover_bob_step(struct ActorObj *obj);
void settle_to_data_target(struct ActorObj *obj);
void step_child_chain(struct ActorObj *obj);
void step_child_chain_b(struct ActorObj *obj);

void reset_height_motion(struct ActorObj *obj)
{
    struct ActorPart *part;
    s32 flags;

    obj->posH = 0;
    sprite_set_action(obj, 5, 0x40AE, 0);
    obj->depth = obj->owner->depth;
    obj->targetX = (part = obj->part)->targetX;
    obj->targetY = part->targetY;
    obj->targetH = obj->posH;
    obj->motionSpeed = 0x200;
    flags = obj->flags79;
    flags |= 0x20;
    obj->flags79 = flags;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 8;
    obj->param9C = 2;
    obj->state = wait_timer_then_place;
}

/* Once the current animation has finished (pose flag 8): hides the first partner (it reappears in
 * anim_track_slot), spawns the 0x40B8 child at the object's base point, facing the same way, one
 * layer in front and without a state of its own, plays action 2 on both (the object with sprite
 * 0x40B7), spawns effect 0x21F2 and goes on to count_dx_and_notify with dx cleared. */
void spawn_child_and_place(struct ActorObj *obj)
{
    struct ActorObj *child;
    struct ActorObj *partner;
    s32 zero;

    if (obj->pose->flags & 8)
    {
        partner = (struct ActorObj *)(gWorld->partners[0] + 8);
        obj_detach_active(partner->pose);
        /* bit 6 of pose->unk11: facing (mirrored) */
        child = obj_spawn(obj->part, 0x40B8, obj->baseX / 0x100, obj->baseY / 0x100,
                          obj->baseH / 0x100, (((u32)obj->pose->unk11 << 25) >> 31), 1);
        obj->child = child;
        child->group = obj->group;
        zero = 0;
        child->depth = obj->depth - 1;
        ((struct Flags79Bits *)&child->flags79)->bit3 = 1;
        child->state = (void (*)(struct ActorObj *))zero;
        sprite_set_action(obj, 2, 0x40B7, 0);
        sprite_set_action(obj->child, 2, 0, 0);
        effect_spawn_at(0x21F2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = count_dx_and_notify;
        obj->dx = zero;
    }
}

/* Once the current animation has finished: plays action 4 on the object (sprite 0x40B7) and its
 * child, spawns effect 0x21FB at the base point, starts a move at speed 0x500 to the fixed point
 * (0xA000, 0x6800, 0xA000), plays sound 0x69 and goes on to timer_30_when_idle. */
void place_and_move_fixed(struct ActorObj *obj)
{
    s32 flags;

    if (obj->pose->flags & POSE_ANIM_DONE)
    {
        sprite_set_action(obj, 4, 0x40B7, 0);
        sprite_set_action(obj->child, 4, 0, 0);
        effect_spawn_at(0x21FB, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->targetX = 0xA000;
        obj->targetY = 0x6800;
        obj->targetH = 0xA000;
        obj->motionSpeed = 0x500;
        flags = obj->flags79;
        flags |= MOTION_TARGET_ABSOLUTE;
        obj->flags79 = flags;
        obj->motionStart = 0;
        start_motion(obj);
        set_slot_value(0x69, -1);
        obj->state = timer_30_when_idle;
    }
}

/* Counts the timer down (set by timer_30_when_idle). When it runs out: action 5 on the object (sprite
 * 0x40B7) and its child, fall speed 0x66 growing by 0x33 per frame (param9C/paramA0), and a 0x40BB
 * rider is spawned at the base point as the child's child, facing the same way, ten layers in front
 * and without a state of its own. Then the object falls in obj_fall_and_split. */
void timer_spawn_grandchild(struct ActorObj *obj)
{
    struct ActorObj *rider;

    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        sprite_set_action(obj, 5, 0x40B7, 0);
        sprite_set_action(obj->child, 5, 0, 0);
        obj->param9C = 0x66;
        obj->paramA0 = 0x33;
        /* bit 6 of pose->unk11: facing (mirrored) */
        rider = obj_spawn(obj->part, 0x40BB, obj->baseX / 0x100, obj->baseY / 0x100,
                          obj->baseH / 0x100, (((u32)obj->pose->unk11 << 25) >> 31), 1);
        obj->child->child = rider;
        rider->group = obj->group;
        rider->depth = obj->depth + 10;
        ((struct Flags79Bits *)&rider->flags79)->bit3 = 0;
        rider->state = NULL;
        obj->state = obj_fall_and_split;
    }
}

/* Drops the object by its fall speed (param9C, growing by paramA0 each frame) with its
   grandchild riding 0x10 above it. On landing: sound 0x40, two impact effects, and the
   object splits into four 0x40BA pieces that slide off sideways (param9C: 0 left, 1 right;
   paramA0: slide speed). The first pair waits 60 frames, the second goes at once; the two
   left pieces can hit things. */
void obj_fall_and_split(struct ActorObj *obj)
{
    struct ActorObj *leftA;
    struct ActorObj *rightA;
    struct ActorObj *leftB;
    struct ActorObj *rightB;

    obj->child->child->posH = obj->posH + 0x1000;
    obj->posH -= obj->param9C;
    obj->param9C += obj->paramA0;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        set_slot_value(0x40, -1);
        obj_release(obj->child->child);
        sprite_set_action(obj, 6, 0x40B7, 0);
        sprite_set_action(obj->child, 6, 0, 0);
        effect_spawn_at(0x2204, obj->posX / 0x100, obj->posY / 0x100, obj->posH / 0x100, obj);
        effect_spawn_at(0x220C, obj->posX / 0x100, obj->posY / 0x100, obj->posH / 0x100, obj);
        obj->timer = 0x14;

        leftA = obj_spawn(obj->part, 0x40BA, obj->posX / 0x100, obj->posY / 0x100,
                          obj->posH / 0x100, 1, 1);
        obj->child->child = leftA;
        leftA->group = obj->group;
        leftA->depth = 0x7F;
        leftA->param9C = 0;
        leftA->paramA0 = 0;
        ((struct Flags79Bits *)&leftA->flags79)->bit0 = 1;
        ((struct Flags77Bits *)&leftA->flags77)->bit6 = 0;
        ((struct Flags77Bits *)&leftA->flags77)->bit5 = 0;
        leftA->handler68 = on_arrive;
        leftA->field6C = obj->field6C;
        leftA->owner = obj->owner;
        obj_list_push_default(leftA);
        leftA->handler58 = bind_part_then_step;
        leftA->handler5C = 0;
        leftA->timer = 0x3C;
        leftA->state = timer_then_step_2b35;

        rightA = obj_spawn(obj->part, 0x40BA, obj->posX / 0x100, obj->posY / 0x100,
                           obj->posH / 0x100, 0, 1);
        leftA->child = rightA;
        rightA->group = obj->group;
        rightA->depth = 0x7F;
        rightA->param9C = 1;
        rightA->paramA0 = 0;
        rightA->timer = 0x3C;
        rightA->state = timer_then_step_2b35;

        leftB = obj_spawn(obj->part, 0x40BA, obj->posX / 0x100, obj->posY / 0x100,
                          obj->posH / 0x100, 1, 1);
        rightA->child = leftB;
        leftB->group = obj->group;
        leftB->depth = 0x7F;
        leftB->param9C = 0;
        leftB->paramA0 = 0x33;
        leftB->pose->unk4 = 0x80;
        ((struct Flags79Bits *)&leftB->flags79)->bit0 = 1;
        leftB->state = scroll_obj_out;
        ((struct Flags77Bits *)&leftB->flags77)->bit6 = 0;
        ((struct Flags77Bits *)&leftB->flags77)->bit5 = 0;
        leftB->handler68 = on_arrive;
        leftB->field6C = obj->field6C;
        leftB->owner = obj->owner;
        obj_list_push_default(leftB);
        leftB->handler58 = roll_action_chance;
        leftB->handler5C = 0;

        rightB = obj_spawn(obj->part, 0x40BA, obj->posX / 0x100, obj->posY / 0x100,
                           obj->posH / 0x100, 0, 1);
        leftB->child = rightB;
        rightB->group = obj->group;
        rightB->depth = 0x7F;
        rightB->param9C = 1;
        rightB->paramA0 = 0x33;
        rightB->state = scroll_obj_out;
        rightB->pose->unk4 = 0x80;
        obj->state = anim_track_slot;
    }
}

/*
 * Once the current animation has finished and the timer has run out, the first partner
 * reappears at this object's position (sprite 0x40AA) and both jump back to their own part's
 * target on a parabola 0x30 pixels high: the partner at 0x66 and this object at 0x180 per
 * frame (action 7, landing in move_by_speed_then_land). For each jump param9C holds the
 * parabola's (negative) coefficient, paramA0 its length in frames, paramA4/paramA8 the
 * distance to cover and dx/dy/dh the start position in pixels.
 */
void anim_track_slot(struct ActorObj *obj)
{
    struct ActorObj *partner;
    s32 distX;
    s32 distY;
    s32 partnerDistX;
    s32 partnerDistY;
    s32 dist;
    s32 frames;
    s32 half;
    s32 coef;

    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            partner = (struct ActorObj *)(gWorld->partners[0] + 8);
            sprite_entry_update(partner->pose);
            partner->posX = obj->posX;
            partner->posY = obj->posY;
            partner->posH = obj->posH;
            sprite_set_action(partner, 0, 0x40AA, 0);
            partner->state = move_by_speed_xy;
            partner->targetX = partner->part->targetX;
            partner->targetY = partner->part->targetY;
            partner->dx = partner->posX / 0x100;
            partner->dy = partner->posY / 0x100;
            partner->dh = partner->posH / 0x100;
            partnerDistX = partner->targetX - (partner->dx << 8);
            partnerDistY = partner->targetY - (partner->dy << 8);
            dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(partnerDistX * partnerDistX + partnerDistY * partnerDistY);
            frames = dist / 0x66;
            half = frames / 2;
            /* starts and ends at the base height, 0x3000 above it halfway */
            coef = 0x3000 / (half * half);
            if (coef == 1)
            {
                coef = 2;
            }
            partner->param9C = -coef;
            partner->paramA0 = frames;
            partner->paramA4 = partnerDistX;
            partner->paramA8 = partnerDistY;
            partner->timer = 0;

            sprite_set_action(obj, 7, 0x40B7, 0);
            sprite_set_action(obj->child, 7, 0, 0);
            obj->targetX = obj->part->targetX;
            obj->targetY = obj->part->targetY;
            obj->dx = obj->posX / 0x100;
            obj->dy = obj->posY / 0x100;
            obj->dh = obj->posH / 0x100;
            distX = obj->targetX - (obj->dx << 8);
            distY = obj->targetY - (obj->dy << 8);
            dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(distX * distX + distY * distY);
            frames = dist / 0x180;
            half = frames / 2;
            coef = 0x3000 / (half * half);
            if (coef == 1)
            {
                coef = 2;
            }
            obj->param9C = -coef;
            obj->paramA0 = frames;
            obj->paramA4 = distX;
            obj->paramA8 = distY;
            obj->timer = 0;
            obj->state = move_by_speed_then_land;
        }
    }
}

/* One frame of the partner's jump that anim_track_slot set up: moves linearly from the start
 * (dx, dy) by (paramA4, paramA8) over paramA0 frames, on a parabola (coefficient param9C < 0)
 * 0x30 px above the start height dh at its middle. Once past the middle and down to the ground,
 * it lands on its part's target height and stops (state = 0). Positions are 24.8 fixed point. */
void move_by_speed_xy(struct ActorObj *obj)
{
    s32 fromMiddle;
    s32 arc;
    s32 peakH;

    obj->posX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + (obj->dx << 8);
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + (obj->dy << 8);
    fromMiddle = (s16)obj->timer - obj->paramA0 / 2;
    arc = fromMiddle * fromMiddle * obj->param9C;
    peakH = (obj->dh << 8) + 0x3000;
    obj->posH = arc + peakH;
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            obj->posH = obj->part->targetH;
            obj->state = 0;
        }
    }
}

/* One frame of the jump anim_track_slot set up: moves linearly from the start (dx, dy) by
 * (paramA4, paramA8) over paramA0 frames, on a parabola (coefficient param9C) 0x30 px above the
 * start height at its middle. Once past the middle and down to the ground, it lands on its
 * part's target height, spawns effect 0x2214 there, shows sprite 0x40B3, releases its child and
 * waits (wait_for_index). */
void move_by_speed_then_land(struct ActorObj *obj)
{
    s32 fromMiddle;
    s32 h;
    s32 peakH;
    s32 groundH;

    obj->posX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + (obj->dx << 8);
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + (obj->dy << 8);
    fromMiddle = (s16)obj->timer - obj->paramA0 / 2;
    h = fromMiddle * fromMiddle * obj->param9C;
    peakH = (obj->dh << 8) + 0x3000;
    obj->posH = h + peakH;
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            groundH = obj->part->targetH;
            obj->posH = groundH;
            effect_spawn_at(0x2214, obj->posX / 0x100, obj->posY / 0x100, groundH / 0x100, obj);
            sprite_set_action(obj, 0, 0x40B3, 0);
            obj_release(obj->child);
            obj->state = wait_for_index;
        }
    }
}

void scroll_obj_out(struct ActorObj *obj)
{
    s32 accel = obj->paramA0 + 0x14;
    s32 x;
    s32 px;

    obj->paramA0 = accel;
    if (obj->param9C == 0)
    {
        x = obj->posX - accel;
        obj->posX = x;
        if (x < 0)
        {
            x += 0xFF;
        }
        px = x >> 8;
        if (px > -0x20)
        {
            goto done;
        }
        obj_list_remove(obj);
    }
    else
    {
        x = obj->posX + accel;
        obj->posX = x;
        if (x < 0)
        {
            x += 0xFF;
        }
        if ((x >> 8) <= 0xFF)
        {
            goto done;
        }
    }
    obj_release(obj);
done:;
}

u32 roll_action_chance(struct ActorObj *obj, struct ActorObj *target)
{
    u8 roll;

    roll = mt19937_next() % 100;
    if (roll <= 0x32 && target->part->unkF6 != 0)
    {
        anim_bit200_step(target->part, 0xC8);
        sprite_set_action(target, 0, 0, 0);
    }
    obj->state = scroll_obj_out;
}
