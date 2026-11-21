#include "global.h"

struct ActorPose
{
    u8 filler0[0x04];
    u16 unk4;
    u16 unk6;
    u8 filler8[0x0C - 0x08];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

/* ActorPose.flags bit: the current animation has finished */
#define POSE_FLAG_ANIM_DONE 0x08

struct ActorPart
{
    u8 filler0[0xB4];
    u8 flagB4;
    u8 fillerB5[0xBA - 0xB5];
    s16 valueBA;
    u8 fillerBC[0xD8 - 0xBC];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 hp;
    s16 maxHp;
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
    s32 (*handler58)(struct ActorObj *, u32);
    s32 (*handler5C)(struct ActorObj *, u32, u32);
    s32 (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*callback)(struct ActorObj *);
    s32 link;
    u8 filler70[0x77 - 0x70];
    u8 flags77;
    u8 flags78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 index;          /* time of the move left, 8.8 frames (0: arrived) */
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 timer;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    s16 wait;
    s16 phase;
    s16 linkB0;
    u16 linkB2;
};

struct ActorFlags79
{
    u8 bit0 : 1;
    u8 unk1 : 4;
    u8 bit5 : 1;
    u8 unk6 : 2;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

// The child takes over bits 5 and 6 of the parent's flag byte at +0x77.
static inline void copy_flags77_bits(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

struct ActorCand
{
    u8 filler0[0x08];
    struct ActorObj obj;
};

struct ActorCands
{
    u8 filler0[0x58];
    u32 actors[6];
    struct ActorCand *cand70;
    struct ActorCand *cand74;
    u8 filler78[0x24C - 0x78];
    u32 item;
};

extern struct ActorCands *gWorld;
extern u32 gUnk_03000E3C; /* the attack's hitbox: the follower effect_from_table_obj spawns */

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_release(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void start_motion(struct ActorObj *obj);
void effect_spawn_at(u32 effect, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_link_slot(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void add_obj_target_offset(u32 item, s32 a, s32 b);
void set_obj_target(u32 item, s32 a, s32 b);
void on_arrive(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void place_parts_by_phase_then_wait(struct ActorObj *obj);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
s32 menu_cursor_pos(s32 x, s32 y, u32 n);
s32 signed_byte_e4(void *part);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void timer_place_then_fire(struct ActorObj *obj);
void timer_place_then_fire_b(struct ActorObj *obj);
void drift_back_then_index(struct ActorObj *obj);
void stop_when_pose_active_c(struct ActorObj *obj);
void stop_when_pose_active_b(struct ActorObj *obj);
void child_gone_place_notify(struct ActorObj *obj);
void place_steps_spawn_random_child(struct ActorObj *obj);
void pose_action5_prep_then_effect(struct ActorObj *obj);
s32 unlink_then_move_end(struct ActorObj *obj, u32 partner);
s32 lookup_part_then_wait(struct ActorObj *obj, u32 a, u32 b);
void drift_then_wait_index(struct ActorObj *obj);
void anim_bit1_chance(void *part, u32 chance);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void step_timer_anim_then_remove(struct ActorObj *obj);
void step_countdown_place_then_wait(struct ActorObj *obj);
void step_countdown_notify_or_place(struct ActorObj *obj);
void set_anim_04_place_part(struct ActorObj *obj);
s32 mt19937_next(void);
s32 kind_place_or_finish(struct ActorObj *obj, struct ActorObj *owner);
void slide_step_to_target(struct ActorObj *obj);
void phase_wait_idle(struct ActorObj *obj);
s32 lookup_part_b_then_prep(struct ActorObj *obj);
void keep_index_then_wait(struct ActorObj *obj);

s32 step_to_anim_0a(struct ActorObj *obj);
s32 step_to_anim_0e(struct ActorObj *obj);
s32 sub_0807680C(struct ActorObj *obj);
s32 sub_08076820(struct ActorObj *obj);
s32 sub_08076834(struct ActorObj *obj);
s32 unlink_then_step_64c8(struct ActorObj *obj, u32 partner);
void set_anim_0a_notify_af(struct ActorObj *obj);
void set_anim_0e_next(struct ActorObj *obj);
void set_anim_02_place_part(struct ActorObj *obj);
void set_anim_by_phase_a(struct ActorObj *obj);
void set_anim_0f_start_wait(struct ActorObj *obj);
void set_anim_by_phase_b(struct ActorObj *obj);
void place_then_start_move(struct ActorObj *obj);
void pose_action7_place_then_move(struct ActorObj *obj);
void periodic_spawn_step(struct ActorObj *obj);
void loop_targets_then_move(struct ActorObj *obj);
void action_move_speed300_then_wait(struct ActorObj *obj);
void move_speed280_then_wait(struct ActorObj *obj);
void slide_fire_then_finish(struct ActorObj *obj);
void place_then_wait_slide_a(struct ActorObj *obj);
void place_then_wait_slide_b(struct ActorObj *obj);
void shrink_then_finish(struct ActorObj *obj);
void slide_finish_then_fire(struct ActorObj *obj);
void wait_phase_world_update(struct ActorObj *obj);
void menu_cursor_place_step(struct ActorObj *obj);
void start_move_then_wait_slot(struct ActorObj *obj);
void effect_timer_step(struct ActorObj *obj);
void pose_start_move_then_wait(struct ActorObj *obj);

/* Actor state: in mode 0 (timer == 0) the sprite spins; every 10 frames a trail effect is spawned at the
   base position. Once index reaches 0, mode 0 goes on to slide off screen; any other mode starts a move
   to 0x20 px right of the owner's target, level with the second actor (gWorld->cand74), and then waits
   in place_then_wait_slide_a. */
void approach_then_place_a(struct ActorObj *obj)
{
    struct ActorCand *other;
    s32 index;
    s32 flags;

    if (obj->timer == 0)
    {
        obj->pose->unkC += 0x600;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        if (obj->timer == 0)
        {
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 12, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 10;
    }
    index = obj->index;
    if (index == 0)
    {
        if (obj->timer == 0)
        {
            obj->state = slide_fire_then_finish;
        }
        else
        {
            other = gWorld->cand74;
            obj->targetX = obj->owner->part->targetX + 0x2000;
            obj->targetY = other->obj.part->targetY;
            obj->targetH = obj->posH;
            flags = obj->flags79;
            flags |= 0x20;
            obj->flags79 = flags;
            obj->motionSpeed = obj->paramA0;
            obj->motionStart = index;
            start_motion(obj);
            obj->state = place_then_wait_slide_a;
        }
    }
}

/* Actor state after approach_then_place_a (mode 0): slides left by paramA0 a frame. In mode 0
   (timer == 0) the sprite spins; every 10 frames a trail effect is spawned at the base position
   (0x23C3 12 px lower in mode 0, else 0x23CC). Once it is 32 px past the left edge of the screen,
   song 0x8C is stopped and the actor is released. */
void slide_fire_then_finish(struct ActorObj *obj)
{
    if (obj->timer == 0)
    {
        obj->pose->unkC += 0x400;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        if (obj->timer == 0)
        {
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 12, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 10;
    }
    obj->posX -= obj->paramA0;
    if (obj->posX / 0x100 <= -32)
    {
        sound_stop_song(0x8C);
        obj_release(obj);
    }
}

/* Actor state: in mode 0 (timer == 0) the sprite spins; every 10 frames a trail effect is spawned at the
   base position. Once index reaches 0, mode 0 goes on to slide off screen; any other mode starts a move
   to 0x20 px right of the owner's target, level with the first actor (gWorld->cand70), and then waits
   in place_then_wait_slide_b. */
void approach_then_place_b(struct ActorObj *obj)
{
    struct ActorCand *other;
    s32 index;
    s32 flags;

    if (obj->timer == 0)
    {
        obj->pose->unkC += 0x600;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        if (obj->timer == 0)
        {
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 12, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 10;
    }
    index = obj->index;
    if (index == 0)
    {
        if (obj->timer == 0)
        {
            obj->state = slide_finish_then_fire;
        }
        else
        {
            other = gWorld->cand70;
            obj->targetX = obj->owner->part->targetX + 0x2000;
            obj->targetY = other->obj.part->targetY;
            obj->targetH = obj->posH;
            flags = obj->flags79;
            flags |= 0x20;
            obj->flags79 = flags;
            obj->motionSpeed = obj->paramA0;
            obj->motionStart = index;
            start_motion(obj);
            obj->state = place_then_wait_slide_b;
        }
    }
}

/* Actor state after approach_then_place_b (mode 0), the twin of slide_fire_then_finish: slides left
   by paramA0 a frame. In mode 0 (timer == 0) the sprite spins; every 10 frames a trail effect is
   spawned at the base position (0x23C3 12 px lower in mode 0, else 0x23CC). Once it is 32 px past
   the left edge of the screen, the actor is released and song 0x8C is stopped. */
void slide_finish_then_fire(struct ActorObj *obj)
{
    if (obj->timer == 0)
    {
        obj->pose->unkC += 0x400;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        if (obj->timer == 0)
        {
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 12, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 10;
    }
    obj->posX -= obj->paramA0;
    if (obj->posX / 0x100 <= -32)
    {
        obj_release(obj);
        sound_stop_song(0x8C);
    }
}

NONMATCH("asm/code_0807544C/place_steps_spawn_random_child.s", void place_steps_spawn_random_child(struct ActorObj *obj))
{
    struct ActorObj *child;
    u8 roll;
    s32 kind;
    s32 idx;
    s32 dx;
    s32 dy;
    s32 x0;
    s32 y0;
    s32 sx;
    s32 sy;
    s32 y;
    s32 xStart;

    roll = (u32)mt19937_next() % 100;
    kind = 0;
    if (roll <= 9)
    {
        kind = 1;
    }
    else if (roll <= 0x45)
    {
        kind = 2;
    }

    x0 = obj->part->targetX - 0x3000;
    y0 = obj->part->targetY + 0x100;

    idx = mt19937_next() & 3;
    xStart = -0x4000;
    if (idx < 0)
    {
        dx = 0x3000;
        dy = 0x6C00;
    }
    else if (idx <= 1)
    {
        dx = 0x3600;
        dy = 0x5C00;
    }
    else if (idx <= 3)
    {
        dx = 0x2800;
        dy = 0x7C00;
    }
    else
    {
        dx = 0x3000;
        dy = 0x6C00;
    }

    sx = x0 - dx;
    sy = y0 - dy;
    y = y0 + ((x0 - xStart) * sy) / sx;

    if (kind == 0)
    {
        child = obj_spawn(obj->part, 0x4188, xStart / 0x100, y / 0x100, 0, 0, 1);
    }
    else if (kind == 1)
    {
        child = obj_spawn(obj->part, 0x40B1, xStart / 0x100, y / 0x100, 0, 0, kind);
    }
    else
    {
        child = obj_spawn(obj->part, 0x4055, xStart / 0x100, y / 0x100, 0, 0, 1);
    }

    child->flags78 = (child->flags78 & 0x1F) | 0x20;
    child->child = obj->child;
    obj->child = child;
    copy_flags77_bits(obj, child);
    child->callback = on_arrive;
    child->link = obj->link;
    child->owner = obj->owner;
    obj_list_push_default(child);
    child->handler58 = (s32 (*)(struct ActorObj *, u32))kind_place_or_finish;
    child->state = slide_step_to_target;
    child->targetX = sx;
    child->targetY = sy;
    child->phase = x0 / 0x100;
    child->linkB0 = y0 / 0x100;
    child->targetH = 0x200;
    child->timer = kind;
    child->paramA0 = 0;
    child->pose->unkC = 0;
    child->paramA4 = 0x100;
    child->pose->unk6 = 0x100;
    child->pose->unk4 = 0x100;
}
END_NONMATCH

/*
 * State of a child spawned by place_steps_spawn_random_child: it hops along a
 * straight line (slope targetY / targetX) from its spawn X to the end point
 * (phase, linkB0), bouncing with vertical speed targetH under gravity. Kind 2
 * spins while it hops and bounces off a raised floor (height 8). Past the end
 * point it heads for its part's target and moves on to shrink_then_finish.
 */
void slide_step_to_target(struct ActorObj *obj)
{

    if (obj->timer == 2)
    {
        obj->paramA0 += -0x600;
        obj->pose->unkC = obj->paramA0;
    }
    obj->posX += 0x200;
    obj->posY = ((s32)obj->linkB0 << 8) + ((((s32)obj->phase << 8) - obj->posX) * obj->targetY) / obj->targetX;
    obj->posH += obj->targetH;
    obj->targetH -= 0x80;
    if (obj->timer == 2)
    {
        if (obj->posH <= 0x7FF)
        {
            obj->targetH = 0x200;
            obj->posH = 0x800;
        }
    }
    else if (obj->posH < 0)
    {
        obj->targetH = 0x200;
        obj->posH = 0;
    }
    if (obj->posX > ((s32)obj->phase << 8))
    {
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY + 0x100;
        obj->targetH = obj->part->targetH + 0x2000;
        ((struct ActorFlags79 *)&obj->flags79)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = shrink_then_finish;
    }
}

/* The thrower's loop: while neither bro (gWorld->cand70/cand74) is busy (index 0), throws a random
 * item every 60 frames (targetH counts down to place_steps_spawn_random_child). Every 16 frames
 * (wait) it spawns effect 0x23D5 at its base. Each time its animation ends, targetX counts down the
 * loops left; when none are left, song 0x157 stops and it waits in child_gone_place_notify for
 * the last item to be gone. */
void periodic_spawn_step(struct ActorObj *obj)
{
    struct ActorCand *bro1;
    struct ActorCand *bro2;

    bro1 = gWorld->cand70;
    bro2 = gWorld->cand74;
    if (bro1->obj.index == 0 && bro2->obj.index == 0)
    {
        obj->targetH--;
        if (obj->targetH <= 0)
        {
            place_steps_spawn_random_child(obj);
            obj->targetH = 60;
        }
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        effect_spawn_at(0x23D5, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->wait = 16;
    }
    if (obj->pose->flags & 8)
    {
        obj->targetX--;
        if (obj->targetX <= 0)
        {
            sound_stop_song(0x157);
            obj->state = child_gone_place_notify;
        }
    }
}

void menu_cursor_place_step(struct ActorObj *obj)
{
    s32 cursor;
    s32 sign;

    if (obj->pose->flags & 8)
    {
        cursor = menu_cursor_pos(obj->part->hp, obj->part->maxHp, 0xA);
        sign = signed_byte_e4(obj->part);
        popup_digit_spawn(cursor - obj->part->hp, obj->baseX / 0x100, obj->baseY / 0x100, sign, 1);
        obj->part->hp = cursor;
        sprite_set_action(obj, 0, 0, 0);
        obj->state = phase_wait_idle;
        obj->phase = 0x20;
    }
}

/* Arrival handler of the items place_steps_spawn_random_child throws at its owner, by kind (timer):
 * kind 0 heals the owner by 5 HP (with a number popup and effect 0xF93), kind 1 tries the status of
 * anim_bit1_chance on it (chance 200, effect 0x2396); both are then released. Kind 2 hits the owner
 * instead (partner_link_update) and leaves the object list. */
s32 kind_place_or_finish(struct ActorObj *obj, struct ActorObj *owner)
{
    s32 newHp;
    s32 popupZ;

    switch (obj->timer)
    {
    case 0:
        newHp = menu_cursor_pos(owner->part->hp, owner->part->maxHp, 5);
        popupZ = signed_byte_e4(owner->part);
        popup_digit_spawn(newHp - owner->part->hp, owner->baseX / 0x100, owner->baseY / 0x100, popupZ, 1);
        owner->part->hp = newHp;
        effect_spawn_at(0xF93, owner->baseX / 0x100, owner->baseY / 0x100, owner->baseH / 0x100, owner);
        obj_release(obj);
        return 1;
    case 1:
        anim_bit1_chance(owner->part, 0xC8);
        effect_spawn_at(0x2396, owner->baseX / 0x100, owner->baseY / 0x100, owner->baseH / 0x100, owner);
        obj_release(obj);
        return 1;
    default:
        partner_link_update(obj, (u32)owner, 0xFF);
        obj_list_remove(obj);
        return 0;
    }
}

void start_move_then_wait_slot(struct ActorObj *obj)
{

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x1C00;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->owner->part->targetH;
        ((struct ActorFlags79 *)&obj->flags79)->bit5 = 1;
        obj->motionSpeed = 0x180;
        obj->motionStart = 0;
        start_motion(obj);
        obj->wait = 8;
        set_slot_value(0x58, -1);
        obj->state = timer_place_then_fire;
    }
}

/* State during the move start_move_then_wait_slot starts: puffs effect 0x244B every 8 frames on the
 * way; on arrival it stops the travel sound 0x58, plays 0x12B, switches to action 4, spawns effect
 * 0x2454 and goes on to pose_action5_prep_then_effect. */
void timer_place_then_fire(struct ActorObj *obj)
{
    obj->wait--;
    if (obj->wait <= 0)
    {
        effect_spawn_at(0x244B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->wait = 8;
    }
    if (obj->index == 0)
    {
        sound_stop_song(0x58);
        set_slot_value(0x12B, -1);
        sprite_set_action(obj, 4, 0, 0);
        effect_spawn_at(0x2454, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = pose_action5_prep_then_effect;
    }
}

/*
 * Attacker state: each time the current animation ends, counts `wait` down; at 0 it strikes:
 * action 6, sound 0xAF, effect 0x2470 and a hitbox from offset row 0x183E. A hit
 * (unlink_then_step_64c8) or a counter (lookup_part_b_then_prep) decides what follows; otherwise it
 * goes on to pose_action7_place_then_move.
 */
void effect_timer_step(struct ActorObj *obj)
{
    if (obj->pose->flags & POSE_FLAG_ANIM_DONE)
    {
        obj->wait--;
        if (obj->wait <= 0)
        {
            sprite_set_action(obj, 6, 0, 0);
            set_slot_value(0xAF, -1);
            effect_spawn_at(0x2470, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            gUnk_03000E3C = effect_from_table_obj(obj, 0x183E);
            obj->handler58 = unlink_then_step_64c8;
            obj->handler60 = lookup_part_b_then_prep;
            obj->state = pose_action7_place_then_move;
        }
    }
}

void pose_start_move_then_wait(struct ActorObj *obj)
{
    s32 flags;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->wait = 8;
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->targetH = obj->part->targetH;
        flags = obj->flags79;
        flags |= 0x20;
        obj->flags79 = flags;
        obj->motionSpeed = 0x300;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = timer_place_then_fire_b;
    }
}

/* State during the move pose_start_move_then_wait starts: leaves trail effect 0x2487 every 8 frames;
 * on arrival (index 0) it plays sound 0x12B (set_slot_value plays a song), switches to action 9,
 * spawns landing effect 0x2462 and waits for that animation to end (stop_when_pose_active_c). */
void timer_place_then_fire_b(struct ActorObj *obj)
{
    obj->wait--;
    if (obj->wait <= 0)
    {
        effect_spawn_at(0x2487, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->wait = 8;
    }
    if (obj->index == 0)
    {
        set_slot_value(0x12B, -1);
        sprite_set_action(obj, 9, 0, 0);
        effect_spawn_at(0x2462, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = stop_when_pose_active_c;
    }
}

NONMATCH("asm/code_0807544C/loop_targets_then_move.s", void loop_targets_then_move(struct ActorObj *obj))
{
    s32 motionSpeedVal = 0x180;
    struct ActorObj *node;
    s32 *tx;
    s32 *ty;
    s32 *th;
    u8 *flags;
    s16 *mstart;
    s16 *speed;
    s32 *count;
    u16 *wp;
    s32 idx;
    s32 zero;
    s32 cur;

    obj->wait--;
    if (obj->wait <= 0)
    {
        obj->timer--;
        if (obj->timer <= 0)
        {
            obj->state = pose_action7_place_then_move;
        }
        else
        {
            wp = &obj->wait;
            tx = &obj->targetX;
            ty = &obj->targetY;
            th = &obj->targetH;
            flags = &obj->flags79;
            speed = &obj->motionSpeed;
            mstart = &obj->motionStart;
            count = &obj->paramA8;
            do
            {
                idx = (*count + 1) & 1;
                *count = idx;
                node = &((struct ActorCand *)gWorld->actors[idx + 6])->obj;
                cur = node->part->hp;
                zero = 1;
                if (cur != 0)
                {
                    zero = 0;
                }
            } while (zero != 0);
            sprite_set_action(obj, 3, 0, 0);
            *tx = node->part->targetX + 0x1C00;
            *ty = node->part->targetY;
            *th = node->part->targetH;
            *flags |= 0x20;
            *speed = motionSpeedVal;
            *mstart = zero;
            start_motion(obj);
            *wp = 8;
            obj->state = timer_place_then_fire;
        }
    }
}
END_NONMATCH
