#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 phase;
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
    u8 filler2[0xF6 - 0xEE];
    s16 hp;
    s16 maxHp;
    u8 filler3[0x104 - 0xFA];
    s16 defense;     /* subtracted in the damage formula */
    u8 filler4[0x10A - 0x106];
    u16 powScale;    /* attack multiplier, 0x100 = 1.0 (status bits 2-3) */
    u16 defScale;    /* defense multiplier, 0x100 = 1.0 (status bits 4-5) */
};

struct PoseBits
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 top : 1;
};

struct ObjBits78
{
    u8 low5 : 5;
    u8 top3 : 3;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gWorld;
extern u32 gUnk_03000E3C;
extern u8 gUnk_03000E7C;
extern u8 gUnk_03000E7D;
extern const u8 gStepSequenceTable[];

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
    u8 filler50[0x54 - 0x50];
    void (*handler54)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    struct ActorPose *link;
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
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    /* per-state parameters; for obj_arc_step_clamped: the arc's curvature, its length in
     * frames, and the whole X and Y distance (8.8 fixed point) */
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx; /* obj_arc_step_clamped: the start position in pixels */
    s16 dy;
    s16 dh;
};

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_arc_start(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_anim5_then_spawn(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_place_at_base_when_index_zero(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_timer_follow_owner(struct ActorObj *obj);
void obj_start_move_right(struct ActorObj *obj);
void obj_copy_bytes_sprite_88(struct ActorObj *obj);
void obj_init_pair_motion(struct ActorObj *obj);
void obj_anim6_wait4(struct ActorObj *obj);
void obj_move_to_part_x_offset(struct ActorObj *obj);
void obj_spawn_child_4115(struct ActorObj *obj);
void obj_spawn_child_4113(struct ActorObj *obj);
void obj_place_sprite_anim9(struct ActorObj *obj);
void obj_link_owner_go(struct ActorObj *obj);
void obj_lookup_slot_start(struct ActorObj *obj);
void obj_setup_wait_target(struct ActorObj *obj);
void obj_lookup_part_setup(struct ActorObj *obj);
void obj_fall_to_ground(struct ActorObj *obj);
void obj_wave_left(struct ActorObj *obj);
void obj_timer_to_move(struct ActorObj *obj);
void obj_wave_left_to_target(struct ActorObj *obj);
void obj_wave_rise_stop(struct ActorObj *obj);
void obj_wave_fall_stop(struct ActorObj *obj);
void obj_move_by_path(void);
void obj_count_down_exit(struct ActorObj *obj);
void obj_timer_table_next(struct ActorObj *obj);
void obj_link_then_finish(struct ActorObj *obj);
void obj_timer_finish(struct ActorObj *obj);
void obj_arc_step_clamped(struct ActorObj *obj);
void obj_arc_to_base_sprite(struct ActorObj *obj);
void obj_pose_idle(struct ActorObj *obj);

static inline s32 arc_px(s32 v)
{
    if (v < 0)
    {
        v += 0xFF;
    }
    return v >> 8;
}
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_index_zero_count(struct ActorObj *obj);
void obj_notify_and_show(struct ActorObj *obj);
void obj_lookup_part_move(struct ActorObj *obj);
void obj_setup_part_motion(struct ActorObj *obj);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 mt19937_next(void);
void obj_list_remove(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_anim_a_wait60(struct ActorObj *obj);
void obj_countdown_then_anim_b(struct ActorObj *obj);
void obj_wait_then_spawn_child(struct ActorObj *obj);
void side_walk_obj_timer_spawn_effect(struct ActorObj *obj);
void obj_pose_release_partner(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
s32 menu_cursor_pos(s32 x, s32 y, u32 n);
s32 signed_byte_e4(void *part);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void obj_pose_action_5(struct ActorObj *obj);
void obj_timer_to_f870(struct ActorObj *obj);
void obj_timer_then_table_step(struct ActorObj *obj);

/*
 * On the pose's event flag (8): play action 6 and effect 0x2041, spawn object 0x40E3 16 pixels left
 * of the base (with this object's bytes 0x74-0x75 and facing), and throw it in a 44-frame arc that
 * peaks 64 pixels up to the point (param9C, paramA0) this object holds; then idle.
 */
void obj_spawn_child_arc(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 distX;
    s32 distY;
    s32 facing;
    s32 frames;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        effect_spawn_at(0x2041, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->part, 0x40E3, obj->baseX / 0x100 - 16, obj->baseY / 0x100,
                          obj->baseH / 0x100, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        child->byte74 = obj->byte74;
        child->byte75 = obj->byte75;
        facing = obj->dir >> 5;
        ((struct ObjBits78 *)&child->dir)->top3 = facing;
        child->state = obj_arc_step_clamped;
        child->targetX = obj->param9C;
        child->targetY = obj->paramA0;
        child->targetH = 0;
        child->dx = arc_px(child->posX);
        child->dy = arc_px(child->posY);
        child->dh = arc_px(child->posH);
        distX = child->targetX - ((s16)child->dx << 8);
        distY = child->targetY - ((s16)child->dy << 8);
        MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(distX * distX + distY * distY); /* result unused */
        frames = 44;
        child->param9C = -33;
        child->paramA0 = frames;
        child->paramA4 = distX;
        child->paramA8 = distY;
        child->timer = 0;
        set_slot_value(0x13B, -1);
        obj->state = obj_pose_idle;
    }
}

/*
 * One frame of a thrown object's flight (set up by obj_spawn_child_arc): X and Y move linearly from
 * the start (dx, dy) by the distance (paramA4, paramA8) over paramA0 frames, and the height follows a
 * parabola, curvature param9C, that peaks 64 pixels above dh halfway through. X and Y stop at the
 * target. Once past the peak and back on the ground it plays effect 0x2049 and sound 0x65
 * (set_slot_value queues a song at a volume) and waits 32 frames in obj_timer_finish.
 */
void obj_arc_step_clamped(struct ActorObj *obj)
{
    s32 newX;
    s32 fromPeak;
    s32 height;
    s32 peakHeight;

    newX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->posX = newX;
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    fromPeak = (s16)obj->timer - obj->paramA0 / 2;
    height = fromPeak * fromPeak;
    height = height * obj->param9C;
    peakHeight = ((s16)obj->dh << 8) + 0x4000;
    obj->posH = height + peakHeight;
    if (newX <= obj->targetX)
    {
        obj->posX = obj->targetX;
    }
    if (obj->targetY > ((s16)obj->dy << 8))
    {
        if (obj->posY >= obj->targetY)
        {
            obj->posY = obj->targetY;
        }
    }
    else if (obj->posY <= obj->targetY)
    {
        obj->posY = obj->targetY;
    }
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            effect_spawn_at(0x2049, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            set_slot_value(0x65, -1);
            obj->posH = 0;
            obj->timer = 32;
            obj->state = obj_timer_finish;
        }
    }
}

/*
 * HP drain: pushes the owner right each frame (unless its pose flag 0x20 is set) and spawns effect
 * 0x202E every 10 frames. After the animation has ended `timer` times it plays action 4 and heals
 * its part by 60% of the damage its attack (link power * 0.4 * powScale) does against the owner's
 * defense (* 0.2 * defScale), at least 1, shows the HP gained and goes to obj_pose_action_5.
 * menu_cursor_pos(points, max, amount) adds amount to points, at most max.
 */
void obj_countdown_place_and_setup(struct ActorObj *obj)
{
    struct ActorObj *owner;
    struct ActorPart *part;
    struct ActorPart *ownerPart;
    s32 t;
    s32 power;
    s32 powScale;
    s32 defense;
    s32 defScale;
    s32 damage;
    s32 defense51;
    s32 defense3;
    s32 heal;
    s32 newHp;
    s32 popupZ;
    s32 gained;

    if (!(obj->owner->pose->flags & 0x20))
    {
        obj->owner->posX += 0x100;
    }
    obj->param9C--;
    if (obj->param9C <= 0)
    {
        effect_spawn_at(0x202E, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->param9C = 10;
    }
    if (obj->pose->flags & 8)
    {
        t = obj->timer - 1;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            set_slot_value(0xD5, -1);
            sprite_set_action(obj, 4, 0, 0);
            effect_spawn_at(0x2059, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            owner = obj->owner;
            power = obj->link->phase;
            part = obj->part;
            powScale = part->powScale;
            ownerPart = owner->part;
            defense = ownerPart->defense;
            defScale = ownerPart->defScale;
            damage = power * 102;
            damage = arc_px(damage * powScale);
            defense3 = (defense << 1) + defense;
            defense51 = defense3 + (defense3 << 4);
            damage = damage - arc_px(defScale * defense51);
            damage = arc_px(damage);
            if (damage <= 0)
            {
                damage = 1;
            }
            heal = (((damage << 4) - damage) << 2) / 100; /* 60% */
            newHp = menu_cursor_pos(part->hp, part->maxHp, heal);
            popupZ = signed_byte_e4(obj->part);
            gained = newHp - obj->part->hp;
            popup_digit_spawn(gained, arc_px(obj->baseX), arc_px(obj->baseY), popupZ, 1);
            obj->part->hp = newHp;
            obj->state = obj_pose_action_5;
        }
    }
}

/*
 * One step of the attack sequence: gStepSequenceTable row gUnk_03000E7C (6 bytes a row), step
 * gUnk_03000E7D, names the target slot. Spawns object 0x40E8 at that target's position (4 pixels
 * lower) with no state of its own, plays action 0xA on a target above y 0x68 (else action 8) and
 * waits 18 frames in obj_timer_table_next, which moves on to the next step.
 */
void obj_spawn_child_from_table(struct ActorObj *obj)
{
    struct ActorObj *target;
    struct ActorObj *child;
    struct ActorPart *part;
    s32 slot;

    slot = gStepSequenceTable[gUnk_03000E7D + gUnk_03000E7C * 6];
    target = (struct ActorObj *)(gWorld->partners[slot + 6] + 8);
    part = obj->part;
    child = obj_spawn(part, 0x40E8, target->part->targetX / 0x100,
                      target->part->targetY / 0x100 + 4, target->part->targetH / 0x100,
                      ((u32)obj->pose->unk11 << 25) >> 31, 1);
    obj->child = child;
    child->byte74 = obj->byte74;
    child->byte75 = obj->byte75;
    *(u8 *)&child->flags |= 1;
    child->state = NULL;
    if (target->part->targetY < 0x6800)
    {
        sprite_set_action(obj, 0xA, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 8, 0, 0);
    }
    obj->timer = 18;
    obj->state = obj_timer_table_next;
}

static inline void seq_tick_a8(struct ActorObj *obj)
{
    obj->paramA8++;
    if (obj->paramA8 > 1)
    {
        obj->state = obj_timer_then_table_step;
    }
}

NONMATCH("asm/code_0806F354/obj_spawn_child_seq.s", void obj_spawn_child_seq(struct ActorObj *obj))
{
    struct ActorObj *child;
    struct ActorObj *candB;
    struct ActorObj *candA;
    struct ActorPart *src;
    struct ActorPart *part;
    s32 pick;
    s32 targetY;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 steps;
    s32 half;
    s32 factor;

    sprite_set_action(obj, obj->paramA4 + 8, 0, 0);
    obj->timer = 2;
    obj->state = obj_timer_to_f870;
    if (obj->paramA8 != 0)
    {
        pick = 0;
        if (obj->paramA4 == 0)
        {
            if (obj->param9C == 0 || obj->param9C == 2)
            {
                pick = 2;
            }
        }
        else if (obj->paramA4 == 2 && (u32)obj->param9C <= 1)
        {
            pick = 1;
        }
        if (pick != 0)
        {
            candA = (struct ActorObj *)(gWorld->partners[6] + 8);
            candB = (struct ActorObj *)(gWorld->partners[7] + 8);
            part = obj->part;
            child = obj_spawn(part, 0x40E5, arc_px(obj->baseX) - 8, arc_px(obj->baseY),
                              arc_px(obj->baseH) + 0x18, ((u32)obj->pose->unk11 << 25) >> 31, 1);
            obj->child = child;
            child->byte74 = obj->byte74;
            child->byte75 = obj->byte75;
            ((struct ObjBits78 *)&child->dir)->top3 = obj->dir >> 5;
            child->state = obj_arc_to_base_sprite;
            if (pick == 1)
            {
                src = candA->part;
                child->targetX = src->targetX + 0x800;
                targetY = src->targetY + 0x300;
            }
            else
            {
                src = candB->part;
                child->targetX = src->targetX + 0x1000;
                targetY = src->targetY;
            }
            child->targetY = targetY;
            child->targetH = src->targetH;
            child->dx = arc_px(child->posX);
            child->dy = arc_px(child->posY);
            child->dh = arc_px(child->posH);
            dx = child->targetX - ((s16)child->dx << 8);
            dy = child->targetY - ((s16)child->dy << 8);
            dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
            steps = dist / 0x180;
            half = steps / 2;
            factor = 0x3000 / (half * half);
            if (factor == 1)
            {
                factor = 2;
            }
            child->param9C = -factor;
            child->paramA0 = steps;
            child->paramA4 = dx;
            child->paramA8 = dy;
            child->timer = 0;
            child->targetH = 0;
            child->targetX = pick;
            child->link = obj->link;
            effect_spawn_at(0x2061, arc_px(obj->baseX), arc_px(obj->baseY), arc_px(obj->baseH), obj);
            set_slot_value(0x85, -1);
        }
    }
    if (obj->paramA0 == 0)
    {
        obj->paramA4++;
        if (obj->paramA4 > 9)
        {
            obj->paramA4 = obj->paramA0;
            seq_tick_a8(obj);
        }
    }
    else
    {
        obj->paramA4--;
        if (obj->paramA4 < 0)
        {
            obj->paramA4 = 9;
        }
        if (obj->paramA4 == 0)
        {
            seq_tick_a8(obj);
        }
    }
}
END_NONMATCH

/*
 * Thrown-object state set up by obj_spawn_child_seq: spins the sprite (targetH is the spin angle, +0x400 a
 * frame) and moves it along a parabola from its start (dx, dy, dh in pixels) by the distance
 * (paramA4, paramA8) over paramA0 frames, param9C being the (negative) curvature and the peak 0x30 pixels up.
 * Once past the peak and on the ground: sound 0x9C, effect 0x2069, action 0 with sprite 0x40E6,
 * and obj_link_then_finish after 14 frames.
 */
void obj_arc_to_base_sprite(struct ActorObj *obj)
{
    s32 newX;
    s32 fromPeak;
    s32 height;
    s32 startHeight;

    obj->pose->phase = obj->targetH;
    obj->targetH += 0x400;
    newX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->posX = newX;
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    fromPeak = (s16)obj->timer - obj->paramA0 / 2;
    height = fromPeak * fromPeak;
    height = height * obj->param9C;
    startHeight = ((s16)obj->dh << 8) + 0x3000;
    obj->posH = height + startHeight;
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            obj->posH = 0;
            set_slot_value(0x9C, -1);
            effect_spawn_at(0x2069, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            obj->dx = 0;
            obj->pose->phase = 0;
            sprite_set_action(obj, 0, 0x40E6, 0);
            obj->timer = 0xE;
            obj->state = obj_link_then_finish;
        }
    }
}
