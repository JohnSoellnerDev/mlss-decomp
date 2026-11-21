#include "global.h"

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
    u8 filler0[0xAC];
    s32 unkAC;
    s32 unkB0;
    u8 fillerB4[0xD8 - 0xB4];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 hp;
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
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    u32 (*handler58)(struct ActorObj *, u32);
    u8 filler5C[0x60 - 0x5C];
    u32 (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*callback68)(struct ActorObj *);
    s32 link6C;
    u8 filler70[0x77 - 0x70];
    u8 unk77;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    s16 wait;
    s16 phase;
    s16 linkB;
    s16 linkH;
};

struct FlagBits
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct FlagBits5
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct Bits77
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Hero
{
    u8 filler0[0x08];
    struct ActorObj body;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gMathRoutines;
extern const s32 gAimTargetX[];
extern const s32 gAimTargetY[];
extern const s32 gSpawnTargetX[];
extern const s32 gSpawnTargetY[];

void sprite_set_action(struct ActorObj *obj, u32 action, u32 arg2, u32 arg3);
s32 resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_release(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);
u32 mt19937_next(void);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_slot(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
u32 anim_sets_spawn_random_child(struct ActorObj *obj, u32 excludeArg);
void spawn_actor_children(struct ActorObj *obj);
void spawn_child_8031(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
u32 arc_tan2(s32 x, s32 y);

// Spawns effect `id` at the object's base position (each axis divided by 0x100).
static inline void place_effect_at_base(struct ActorObj *obj, u32 id)
{
    effect_spawn_at(id, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
}

void spawn_child_4046(struct ActorObj *obj);
void link_actors_when_ready(struct ActorObj *obj);
void spawn_child_4048_aim(struct ActorObj *obj);
void update_pose_index(struct ActorObj *obj);
u32 move_to_target_if_found(struct ActorObj *obj);
void step_move_and_finish(struct ActorObj *obj);
void step_link_or_countdown_a(struct ActorObj *obj);
void step_link_or_wait_b(struct ActorObj *obj);
void spawn_child_4049_move(struct ActorObj *obj);
void step_arc_move(struct ActorObj *obj);
void reset_anim_if_active_a(struct ActorObj *obj);
void step_countdown_then_select(struct ActorObj *obj);
void reset_anim_if_active_b(struct ActorObj *obj);
u32 link_place_and_remove(struct ActorObj *obj, u32 other);
u32 step_countdown_notify_or_place(struct ActorObj *obj, struct ActorObj *other);
void place_and_remove_when_index_zero(struct ActorObj *obj);
void reset_anim_if_active_c(struct ActorObj *obj);
void step_timer_anim_then_remove(struct ActorObj *obj);
void step_remove_child_then_place(struct ActorObj *obj);
void step_start_child_anim(struct ActorObj *obj);
void step_countdown_place_then_wait(struct ActorObj *obj);
void set_anim_04_place_part(struct ActorObj *obj);
s32 unknown_part_steps_08077c80(struct ActorObj *obj);
s32 sub_08077C8C(struct ActorObj *obj);
s32 sub_08077C98(struct ActorObj *obj);
s32 sub_08077CA4(struct ActorObj *obj);
s32 step_init_timers(struct ActorObj *obj);
void step_random_anim_3_or_5(struct ActorObj *obj);
void set_anim_0d_place_part(struct ActorObj *obj);
void set_anim_0a_next(struct ActorObj *obj);
void step_remove_children_then_idle(struct ActorObj *obj);
void step_rise_until_height(struct ActorObj *obj);
void step_place_children_then_idle(struct ActorObj *obj);
void set_anim_06_place_part(struct ActorObj *obj);
void set_anim_02_place_part_24ba(struct ActorObj *obj);
void set_anim_0e_save_timer(struct ActorObj *obj);
void countdown_set_anim_0b_place_part(struct ActorObj *obj);
void link_actor_pair_start(struct ActorObj *obj);
void countdown_phase_then_wait(struct ActorObj *obj);
void countdown_spawn_pair_link(struct ActorObj *obj);

/* Actor state after set_anim_06_place_part: once the pose has finished (flag 8) it takes action 7,
   spawns effect 0x2503 5 px below its base and an object 0x4046 72 px left of (and 2 px below) its
   part's target, facing the way the actor faces (pose +0x11 bit 6). The new object goes first in the
   actor's child list with flags bit 0 set; sound 0x9D plays and step_start_child_anim takes over. */
void spawn_child_4046(struct ActorObj *obj)
{
    struct ActorObj *child;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        effect_spawn_at(0x2503, obj->baseX / 0x100, obj->baseY / 0x100 + 5, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->part, 0x4046, obj->part->targetX / 0x100 - 72,
                          obj->part->targetY / 0x100 + 2, obj->part->targetH / 0x100,
                          ((u32)obj->pose->unk11 << 25) >> 31, 1);
        child->child = obj->child;
        obj->child = child;
        ((struct FlagBits *)&child->flags)->bit0 = 1;
        set_slot_value(0x9D, -1);
        obj->state = step_start_child_anim;
    }
}

/* Actor state after step_start_child_anim: for `wait` frames, hits each of the two heroes once
   (partner_link_update deals the damage) while it is alive and standing on the ground (height <= 0);
   phase and linkB remember which hero was already hit. When the time is up, the child takes action 2
   and step_remove_child_then_place takes over. */
void link_actors_when_ready(struct ActorObj *obj)
{
    struct ActorObj *hero;

    hero = &gWorld->heroA->body;
    if (hero->part->hp != 0 && hero->posH <= 0)
    {
        if (obj->phase == 0)
        {
            partner_link_update(obj, (u32)hero, 0xFF);
            obj->phase = 1;
        }
    }
    hero = &gWorld->heroB->body;
    if (hero->part->hp != 0 && hero->posH <= 0)
    {
        if (obj->linkB == 0)
        {
            partner_link_update(obj, (u32)hero, 0xFF);
            obj->linkB = 1;
        }
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        sprite_set_action(obj->child, 2, 0, 0);
        obj->state = step_remove_child_then_place;
    }
}

// Once the object's animation ends (pose flag 8): plays action 0xB with effect 0x2539, spawns child
// object 0x4048 just left of and above the object's part and sends it (speed 0x400, height 0x800) to
// aim spot param9C: the owner's target for spots 3 and 4, else gAimTargetX/Y. The child's sprite is
// turned towards that spot (paramA0, applied by update_pose_index) and it inherits param9C.
void spawn_child_4048_aim(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 aimDx;
    s32 aimDy;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        place_effect_at_base(obj, 0x2539);
        child = obj_spawn(obj->part, 0x4048, obj->part->targetX / 0x100 - 0x10,
                          obj->part->targetY / 0x100 + 8, obj->part->targetH / 0x100 + 0x10,
                          ((u32)obj->pose->unk11 << 25) >> 31, 1);
        sprite_set_action(child, 2, 0, 0);
        child->child = obj->child;
        obj->child = child;
        child->dir = (child->dir & 0x1F) | 0x20;
        ((struct Bits77 *)&child->unk77)->bit6 = 0;
        ((struct Bits77 *)&child->unk77)->bit5 = 0;
        child->callback68 = on_arrive;
        child->link6C = obj->link6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->state = update_pose_index;

        /* spots 3 and 4: wherever the owner is headed */
        if ((u32)(obj->param9C - 3) <= 1)
        {
            child->targetX = obj->owner->part->targetX;
            child->targetY = obj->owner->part->targetY;
        }
        else
        {
            child->targetX = gAimTargetX[obj->param9C] << 8;
            child->targetY = gAimTargetY[obj->param9C] << 8;
        }
        child->targetH = 0x800;
        child->motionSpeed = 0x400;
        ((struct FlagBits5 *)&child->flags)->bit5 = 1;
        child->motionStart = 0;
        start_motion(child);
        child->param9C = obj->param9C;
        child->paramA0 = 0;

        /* sprite rotation: the angle from the spawn point to the aim spot */
        aimDx = gAimTargetX[obj->param9C] + 0x10;
        aimDx -= obj->part->targetX / 0x100;
        aimDy = -8 - obj->part->targetH / 0x100;
        child->paramA0 = (((u32)arc_tan2((s16)aimDx, (s16)aimDy) << 16) >> 24) + 0xFFFFC000;
        set_slot_value(0x86, -1);
        obj->state = reset_anim_if_active_c;
    }
}

// State of the 0x4048 object thrown by spawn_child_4048_aim: keeps its sprite turned by paramA0 while
// the move runs. Once it has arrived (index, the frames left, is 0) the sprite turns back and sound
// 0x8B plays: at aim spots 3 and up it bursts (effect 0x254E) and is freed, else it lands on the
// ground (effect 0x2541, action 0) and stays there without a state.
void update_pose_index(struct ActorObj *obj)
{
    s32 framesLeft;

    obj->pose->unkC = obj->paramA0;
    framesLeft = obj->index;
    if (framesLeft == 0)
    {
        obj->pose->unkC = framesLeft;
        if (obj->param9C > 2)
        {
            place_effect_at_base(obj, 0x254E);
            obj_release(obj);
            set_slot_value(0x8B, -1);
        }
        else
        {
            place_effect_at_base(obj, 0x2541);
            obj->state = NULL;
            obj->posH = 0;
            sprite_set_action(obj, 0, 0, 0);
            set_slot_value(0x8B, -1);
        }
    }
}

// Throws a random projectile from above the actor's home position: kind 0 (35%, type 0x404A),
// kind 1 (35%, type 0x404B) or kinds 2-7 (30%, type 0x404C), never kind 0 or 1 twice in a row.
// It flies to the kind's spot in gSpawnTargetX/Y and remembers the home position to be sent
// back to. Returns the kind thrown, for the next call's lastKindArg.
u32 anim_sets_spawn_random_child(struct ActorObj *obj, u32 lastKindArg)
{
    u8 kind;
    u8 roll;
    u16 lastKind;
    struct ActorObj *child;

    lastKind = lastKindArg;
    roll = mt19937_next() % 100;
    if (roll <= 34)
    {
        kind = 0;
    }
    else if (roll <= 69)
    {
        kind = 1;
    }
    else
    {
        kind = mt19937_next() % 6 + 2;
    }
    if (kind == (s16)lastKind && kind <= 1)
    {
        kind += 2;
    }

    /* Spawned 0x40 to the side of and 0xA0 above the home position, facing as the actor does. */
    switch (kind)
    {
    case 0:
        child = obj_spawn(obj->part, 0x404A, obj->part->targetX / 0x100 + 0x40,
                          obj->part->targetY / 0x100, obj->part->targetH / 0x100 + 0xA0,
                          ((u32)obj->pose->unk11 << 25) >> 31, 1);
        break;
    case 1:
        child = obj_spawn(obj->part, 0x404B, obj->part->targetX / 0x100 + 0x40,
                          obj->part->targetY / 0x100, obj->part->targetH / 0x100 + 0xA0,
                          ((u32)obj->pose->unk11 << 25) >> 31, kind);
        break;
    default:
        child = obj_spawn(obj->part, 0x404C, obj->part->targetX / 0x100 + 0x40,
                          obj->part->targetY / 0x100, obj->part->targetH / 0x100 + 0xA0,
                          ((u32)obj->pose->unk11 << 25) >> 31, 1);
        break;
    }
    child->child = obj->child;
    obj->child = child;

    child->targetX = gSpawnTargetX[kind] << 8;
    child->targetY = gSpawnTargetY[kind] << 8;
    child->targetH = 0x1000;
    child->motionSpeed = 0xB3 << 2;
    ((struct FlagBits5 *)&child->flags)->bit5 = 1;
    child->motionStart = 0;
    start_motion(child);
    ((struct Bits77 *)&child->unk77)->bit6 = 0;
    ((struct Bits77 *)&child->unk77)->bit5 = 0;
    child->callback68 = on_arrive;
    child->link6C = obj->link6C;
    child->owner = obj->owner;
    obj_list_push_default(child);

    /* Hitting a hero links and removes it; hit back, it flies to the home position. */
    child->handler58 = link_place_and_remove;
    child->handler60 = move_to_target_if_found;
    child->phase = obj->part->targetX / 0x100;
    child->linkB = obj->part->targetY / 0x100;
    child->state = place_and_remove_when_index_zero;
    return kind;
}

// handler60 of the thrown projectiles: once lookup_slot reports the hit (0), plays effect 0x25D5 and
// sends the projectile back to the home position saved in phase/linkB along a parabola. It flies at
// 4 px per frame (paramA0 frames, paramA4/paramA8 the whole move) and peaks 0x38 px above its start
// halfway there; param9C is the parabola's (negative) curvature. step_move_and_finish flies it.
u32 move_to_target_if_found(struct ActorObj *obj)
{
    u32 result;
    s32 frames;
    s32 dx;
    s32 dy;
    s32 curve;

    result = lookup_slot(obj);
    if (result != 0)
    {
        return result;
    }
    place_effect_at_base(obj, 0x25D5);
    sprite_set_action(obj, 1, 0, 0);

    /* phase/linkB/linkH change from the home position to the start of the flight */
    obj->targetX = obj->phase << 8;
    obj->targetY = obj->linkB << 8;
    obj->phase = obj->posX / 0x100;
    obj->linkB = obj->posY / 0x100;
    obj->linkH = obj->posH / 0x100;
    dx = obj->targetX - (obj->phase << 8);
    dy = obj->targetY - (obj->linkB << 8);

    /* gMathRoutines starts with the integer square root */
    frames = ((s32 (*)(s32))gMathRoutines)(dx * dx + dy * dy) / 0x400;
    curve = 0x3800 / ((frames / 2) * (frames / 2));
    if (curve == 1)
    {
        curve = 2;
    }
    obj->param9C = -curve;
    obj->paramA0 = frames;
    obj->paramA4 = dx;
    obj->paramA8 = dy;
    obj->wait = result;
    obj->state = step_move_and_finish;
    obj->targetH = result;
    return 0;
}

// One frame of a projectile's flight home (set up by move_to_target_if_found): frame `wait` of
// paramA0 moves it on a straight line in x/y from (phase, linkB) px by paramA4/paramA8, and on a
// parabola in height that peaks 0x38 px above linkH halfway; it spins (pose unkC, 0x800 a frame).
// At the end: effect 0x25C7, the projectile is released, part->unkB0 = 1 and sound 0xCC plays.
void step_move_and_finish(struct ActorObj *obj)
{
    s32 fromMiddle;
    s32 drop;
    s32 peak;

    obj->targetH -= 0x800;
    obj->pose->unkC = obj->targetH;
    obj->posX = (obj->paramA4 * obj->wait) / obj->paramA0 + (obj->phase << 8);
    obj->posY = (obj->paramA8 * obj->wait) / obj->paramA0 + (obj->linkB << 8);
    fromMiddle = obj->wait - obj->paramA0 / 2;
    drop = (fromMiddle * fromMiddle) * obj->param9C; // param9C < 0
    peak = (obj->linkH << 8) + 0x3800;
    obj->posH = drop + peak;
    obj->wait++;
    if (obj->wait >= obj->paramA0)
    {
        place_effect_at_base(obj, 0x25C7);
        obj_release(obj);
        obj->part->unkB0 = 1;
        set_slot_value(0xCC, -1);
    }
}

// Throwing state (set up by set_anim_0e_save_timer): every 0x32 frames throws a random projectile
// (param9C counts down, linkH is the last kind thrown) until `wait` throws are done, then moves on
// to step_link_or_wait_b; every 0x14 frames (paramA0) plays effect 0x25A3. Once hit (paramA8 set),
// reacts to the contact and pauses 0x1E frames in step_countdown_then_select, which comes back here
// (paramA8 = 0x10).
void step_link_or_countdown_a(struct ActorObj *obj)
{
    if (obj->paramA8 != 0)
    {
        resolve_contact_action(obj->owner, obj, obj->posX / 0x100, obj->posH / 0x100,
                               obj->owner->part->slot != -1 ? 0x13 : 0x12);
        sprite_set_action(obj, 1, 0, 0);
        obj->phase = 0x1E;
        obj->state = step_countdown_then_select;
        obj->paramA8 = 0x10;
    }
    else
    {
        obj->param9C--;
        if (obj->param9C <= 0)
        {
            obj->linkH = anim_sets_spawn_random_child(obj, obj->linkH);
            obj->param9C = 0x32;
            obj->wait--;
            if (obj->wait <= 0)
            {
                obj->state = step_link_or_wait_b;
            }
            set_slot_value(0x8C, -1);
        }
        obj->paramA0--;
        if (obj->paramA0 <= 0)
        {
            place_effect_at_base(obj, 0x25A3);
            obj->paramA0 = 0x14;
        }
    }
}

void step_link_or_wait_b(struct ActorObj *obj)
{
    if (obj->paramA8 != 0)
    {
        resolve_contact_action(obj->owner, obj, obj->posX / 0x100, obj->posH / 0x100,
                               obj->owner->part->slot != -1 ? 0x13 : 0x12);
        sprite_set_action(obj, 1, 0, 0);
        obj->phase = 0x1E;
        obj->state = step_countdown_then_select;
        obj->paramA8 = 0x11;
    }
    else if (obj->child == NULL)
    {
        sprite_set_action(obj, 0xF, 0, 0);
        obj->state = reset_anim_if_active_b;
    }
}

// Once the object's animation ends (pose flag 8): spawns child object 0x4049 at the object's
// part and throws it at its owner's target. The child bounces along repeated parabolas
// (step_arc_move) until it leaves the screen on the left. Without param9C (action 4) each hop
// covers the whole distance and peaks a third of it high; with it (action 6) a hop covers 2/3
// of the distance and peaks half of that high, at least 0x30. A hop lasts distance / 0x1B3 frames.
void spawn_child_4049_move(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 startX;
    s32 startY;
    s32 aimX;
    s32 aimY;
    s32 *hopX;
    s32 *hopY;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 duration;
    s32 half;
    s32 curve;

    if (obj->pose->flags & 8)
    {
        child = obj_spawn(obj->part, 0x4049, obj->part->targetX / 0x100, obj->part->targetY / 0x100,
                          obj->part->targetH / 0x100, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        child->child = obj->child;
        obj->child = child;
        child->dir = (child->dir & 0x1F) | 0x20;
        ((struct Bits77 *)&child->unk77)->bit6 = 0;
        ((struct Bits77 *)&child->unk77)->bit5 = 0;
        child->callback68 = on_arrive;
        child->link6C = obj->link6C;
        child->owner = obj->owner;
        obj_list_push_default(child);

        // One hop's displacement (8.8 fixed point) and its peak height (whole units).
        startX = child->posX;
        startY = child->posY;
        aimX = obj->owner->part->targetX;
        aimY = obj->owner->part->targetY;
        hopX = &child->targetX;
        dx = aimX - startX;
        *hopX = dx;
        hopY = &child->targetY;
        dy = aimY - startY;
        *hopY = dy;
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 4, 0, 0);
            dist = ((s32 (*)(s32))gMathRoutines)(dx * dx + dy * dy);
            child->targetH = dist / 0x300;
        }
        else
        {
            sprite_set_action(obj, 6, 0, 0);
            child->targetX = dx * 2 / 3;
            child->targetY = dy * 2 / 3;
            dx = child->targetX;
            dy = child->targetY;
            dist = ((s32 (*)(s32))gMathRoutines)(dx * dx + dy * dy);
            child->targetH = dist / 0x200;
            if (child->targetH <= 0x2F)
            {
                child->targetH = 0x30;
            }
        }

        // Start of the first hop (whole units); height = peak - curve * (wait - duration / 2)^2.
        child->phase = startX / 0x100;
        child->linkB = startY / 0x100;
        child->linkH = 0;
        duration = dist / 0x1B3;
        half = duration / 2;
        curve = (child->targetH << 8) / (half * half);
        if (curve == 1)
        {
            curve = 2;
        }
        child->param9C = -curve;
        child->paramA0 = duration;
        child->paramA4 = dx;
        child->paramA8 = dy;
        child->wait = 0;
        child->state = step_arc_move;
        set_slot_value(0x9C, -1);
        obj->state = reset_anim_if_active_a;
    }
}

// State of the bouncing child that spawn_child_4049_move throws: spins its sprite, and flies the
// current hop from (phase, linkB) by (paramA4, paramA8) over paramA0 frames on the parabola
// height = peak (targetH) + curvature (param9C, negative) * (wait - paramA0 / 2)^2. Past the top,
// once it is down to 7 units it starts the next hop from where it is. It is released once it is
// 0x20 px off the left edge of the screen.
void step_arc_move(struct ActorObj *obj)
{
    s32 fromTop;

    if (obj->posX / 0x100 < -0x20)
    {
        obj_release(obj);
    }
    else
    {
        obj->pose->unkC += 0x600;
        obj->posX = (obj->paramA4 * obj->wait) / obj->paramA0 + (obj->phase << 8);
        obj->posY = (obj->paramA8 * obj->wait) / obj->paramA0 + (obj->linkB << 8);
        fromTop = obj->wait - obj->paramA0 / 2;
        obj->posH = (fromTop * fromTop) * obj->param9C + (obj->targetH << 8) + (obj->linkH << 8);
        obj->wait++;
        if (obj->wait > obj->paramA0 / 2)
        {
            if (obj->posH / 0x100 <= 7)
            {
                /* landed: bounce from here */
                obj->posH = 0x800;
                obj->phase = obj->posX / 0x100;
                obj->linkB = obj->posY / 0x100;
                obj->wait = 0;
            }
        }
    }
}

void reset_anim_if_active_a(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = NULL;
    }
}

void step_countdown_then_select(struct ActorObj *obj)
{
    obj->phase--;
    if (obj->phase <= 0)
    {
        if (obj->part->hp == 0)
        {
            obj_stop(obj);
        }
        else
        {
            sprite_set_action(obj, 0xE, 0, 0);
            if (obj->paramA8 == 0x10)
            {
                obj->state = step_link_or_countdown_a;
            }
            else
            {
                obj->state = step_link_or_wait_b;
            }
            obj->paramA8 = 0;
        }
    }
}

void reset_anim_if_active_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = NULL;
    }
}

u32 link_place_and_remove(struct ActorObj *obj, u32 other)
{
    partner_link_update(obj, other, 0xFF);
    place_effect_at_base(obj, 0x25B9);
    obj_release(obj);
    return 0;
}

void place_and_remove_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        place_effect_at_base(obj, 0x25AB);
        set_slot_value(0x76, -1);
        obj_release(obj);
    }
}

void reset_anim_if_active_c(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = NULL;
    }
}

u32 step_countdown_notify_or_place(struct ActorObj *obj, struct ActorObj *other)
{
    s16 *wait = &obj->wait;

    (*wait)--;
    if (*wait != 0)
    {
        set_slot_value(0x47, -1);
        sprite_set_action(obj, *wait, 0, 0);
        obj->targetH = 0x300;
        obj->state = step_rise_until_height;
        other->linkH = 0;
    }
    else
    {
        set_slot_value(0x55, -1);
        place_effect_at_base(obj, 0x24CB);
        obj_release(obj);
    }
    return 0;
}

void step_timer_anim_then_remove(struct ActorObj *obj)
{
    s16 wait;

    obj->param9C--;
    if (obj->param9C <= 0)
    {
        obj->param9C = 0x3C;
        obj->wait--;
        sprite_set_action(obj, obj->wait, 0, 0);
        wait = obj->wait;
        if (wait == 0)
        {
            obj->part->unkAC = wait;
            obj_release(obj);
            set_slot_value(0x10A, -1);
        }
        else
        {
            set_slot_value(0xA8, -1);
        }
    }
}

void step_remove_child_then_place(struct ActorObj *obj)
{
    if (obj->child->pose->flags & 8)
    {
        obj_release(obj->child);
        sprite_set_action(obj, 8, 0, 0);
        place_effect_at_base(obj, 0x2515);
        obj->state = step_place_children_then_idle;
    }
}

void step_start_child_anim(struct ActorObj *obj)
{
    if (obj->child->pose->flags & 8)
    {
        sprite_set_action(obj->child, 1, 0, 0);
        obj->wait = 0x14;
        obj->linkB = 0;
        obj->phase = 0;
        obj->state = link_actors_when_ready;
    }
}

void step_countdown_place_then_wait(struct ActorObj *obj)
{
    obj->paramA8--;
    if (obj->paramA8 <= 0)
    {
        place_effect_at_base(obj, 0x24E9);
        obj->paramA8 = 0xA;
    }
    if (obj->paramA4 == 0)
    {
        obj->state = set_anim_06_place_part;
    }
}

void set_anim_04_place_part(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        place_effect_at_base(obj, 0x25F5);
        obj->state = spawn_child_8031;
    }
}

s32 unknown_part_steps_08077c80(struct ActorObj *obj)
{
    obj->state = step_random_anim_3_or_5;
    return 1;
}

s32 sub_08077C8C(struct ActorObj *obj)
{
    obj->state = set_anim_0d_place_part;
    return 1;
}

s32 sub_08077C98(struct ActorObj *obj)
{
    obj->state = step_remove_children_then_idle;
    return 1;
}

s32 sub_08077CA4(struct ActorObj *obj)
{
    obj->state = set_anim_0a_next;
    return 1;
}

s32 step_init_timers(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 5;
    }
    if (obj->paramA0 == 0)
    {
        obj->paramA0 = 4;
    }
    obj->state = set_anim_02_place_part_24ba;
    return 1;
}

void step_random_anim_3_or_5(struct ActorObj *obj)
{
    obj->param9C = mt19937_next() & 1;
    if (obj->param9C == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 5, 0, 0);
    }
    obj->state = spawn_child_4049_move;
}

void set_anim_0d_place_part(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xD, 0, 0);
    place_effect_at_base(obj, 0x2563);
    set_slot_value(0x4B, -1);
    obj->state = set_anim_0e_save_timer;
}

void set_anim_0a_next(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xA, 0, 0);
    obj->state = spawn_child_4048_aim;
}

void step_remove_children_then_idle(struct ActorObj *obj)
{
    while (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    obj->state = NULL;
}

void step_rise_until_height(struct ActorObj *obj)
{
    s32 h;

    obj->posH += obj->targetH;
    obj->targetH -= 0x80;
    h = obj->posH;
    if (h < 0)
    {
        h += 0xFF;
    }
    h >>= 8;
    if (h <= 0x28)
    {
        obj->posH = 0x2800;
        obj->state = NULL;
    }
}

void step_place_children_then_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        while (obj->child != NULL)
        {
            effect_spawn_at(0x24E0, obj->child->baseX / 0x100, obj->child->baseY / 0x100,
                            obj->child->baseH / 0x100, obj);
            obj_release(obj->child);
        }
        obj->state = NULL;
    }
}

void set_anim_06_place_part(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        place_effect_at_base(obj, 0x24FB);
        obj->state = spawn_child_4046;
    }
}

void set_anim_02_place_part_24ba(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    place_effect_at_base(obj, 0x24BA);
    obj->state = spawn_actor_children;
}

void set_anim_0e_save_timer(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        obj->wait = obj->param9C;
        obj->param9C = 0x32;
        obj->paramA0 = 0x14;
        obj->paramA8 = 0;
        obj->linkH = 3;
        obj->state = step_link_or_countdown_a;
    }
}

void countdown_set_anim_0b_place_part(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->wait--;
        if (obj->wait <= 0)
        {
            sprite_set_action(obj, 0xB, 0, 0);
            set_slot_value(0x89, -1);
            place_effect_at_base(obj, 0x2983);
            obj->wait = 0x3C;
            obj->state = link_actor_pair_start;
        }
    }
}
