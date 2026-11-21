#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 mode;
};

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
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    s32 (*handler58)(struct ActorObj *, u32);
    s32 (*handler5C)(struct ActorObj *);
    s32 (*handler60)(struct ActorObj *);
    void *handler64;
    void (*handler68)(struct ActorObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
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
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

// The spawned child takes over two flag bits of its parent (kind is set by the caller).
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

struct ActorObj *obj_spawn(struct ActorPart *part, u32 kind, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void obj_begin_move_to_params(struct ActorObj *obj);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
s32 is_state_busy(void);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void obj_action0_to_idle(struct ActorObj *obj);
void obj_action3_wait_pose(struct ActorObj *obj);
void obj_spawn_child_when_none(struct ActorObj *obj);
void obj_wait_ready_action_flag(struct ActorObj *obj);
void obj_timer_expired_set_motion(struct ActorObj *obj);
void obj_begin_approach(struct ActorObj *obj);
void obj_begin_idle_at_base(struct ActorObj *obj);
void obj_pose_active_aim_at_owner_part(struct ActorObj *obj);
void obj_tick_counter_place(struct ActorObj *obj);
void obj_spawn_child_glide(struct ActorObj *obj);
void obj_slot_notify_action(struct ActorObj *obj);
void obj_begin_table_jump(struct ActorObj *obj);
void base_moves_obj_wait_ready_action2(struct ActorObj *obj);
void obj_index_zero_action5_timer2(struct ActorObj *obj);
void obj_timer_down_idle7(struct ActorObj *obj);
void obj_timer_down_action5(struct ActorObj *obj);
void obj_tick_wait_table_timer(struct ActorObj *obj);
s32 obj_lookup_slot_free_heap_then_wait(struct ActorObj *obj);
void obj_timer_down_action4_then_idle(struct ActorObj *obj);
void obj_count_timer_then_move(struct ActorObj *obj);
void obj_begin_drop_to_base(struct ActorObj *obj);
u32 obj_unlink_reset(struct ActorObj *obj, u32 a, u32 b);
void sound_stop_song(u32 song);
void actor_pose_steps(struct ActorObj *obj);
void obj_begin_move_to_owner(struct ActorObj *obj);
void obj_spawn_child_sprite(struct ActorObj *obj);
void obj_begin_table_move(struct ActorObj *obj);
void obj_index_zero_set_motion(struct ActorObj *obj);
void obj_timer_action4_or_8(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void start_motion(struct ActorObj *obj);
void obj_index_zero_set_motion_b(struct ActorObj *obj);
void obj_index_zero_action9_then_idle(struct ActorObj *obj);
void obj_wait_action7(struct ActorObj *obj);
void obj_action3_begin(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void obj_pose_active_action3_place(struct ActorObj *obj);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u8 poll_spline_path(u32 obj, s32 *outX, s32 *outY);
void heap_free_b(void *p);
/* gParamTable is indexed by ActorObj.param9C (the variant, 0 or 1). */
struct ParamEntry
{
    s32 dx;       /* move target: pixels from the owner part's target (obj_begin_table_move) */
    s32 dy;
    s32 dh;
    s32 speed;    /* motionSpeed of that move */
    s32 unk10;    /* timer after the move (obj_tick_action4_timer) */
    s32 unk14;    /* jump height in pixels above the owner part's target (obj_begin_table_jump) */
    s32 unk18;    /* motionSpeed of the jump */
    s32 unk1C;    /* timer after the jump (obj_tick_wait_table_timer) */
    s32 duration; /* frames between the effects obj_tick_counter_place spawns (paramA4) */
};
extern struct ParamEntry gParamTable[];
extern u32 gUnk_03000E18;
extern s32 gUnk_03000E1C;
extern s32 gUnk_03000E20;
void obj_wait_ready_setup_motion(struct ActorObj *obj);
void obj_timer_end_build_anim_record(struct ActorObj *obj);
void obj_remove_child_idle(struct ActorObj *obj);
void obj_wait_ready_spawn_glide(struct ActorObj *obj);
void obj_wait_ready_setup_offsets(struct ActorObj *obj);
void base_moves_obj_wait_ready_place_sprite(struct ActorObj *obj);
void obj_begin_hover_at_base(struct ActorObj *obj);
void obj_update_jump_arc(struct ActorObj *obj);
void obj_action0_then_spawn(struct ActorObj *obj);

// Once the current pose is done (pose flag 8): plays sound 0xD9 and sends obj (speed 0x380, flags
// bit 5) to the point two thirds of the way from its owner's part target to the stored params point,
// 0x30 pixels above that point's height, with action 4, spawns effect 0x1071 at its base and moves
// on to obj_begin_hover_at_base.
void obj_pose_active_aim_at_owner_part(struct ActorObj *obj)
{
    struct ActorObj *owner;
    struct ActorPart *part;
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        set_slot_value(0xD9, -1);
        owner = obj->owner;
        obj->targetX = (part = owner->part)->targetX + ((obj->param9C << 8) - part->targetX) * 2 / 3;
        obj->targetY = part->targetY + ((obj->paramA0 << 8) - part->targetY) * 2 / 3;
        obj->targetH = (obj->paramA4 + 0x30) << 8;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x380;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->mode = 0x10;
        effect_spawn_at(0x1071, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_begin_hover_at_base;
    }
}

/* Once the current animation has ended: restarts sound 0xD9 and flies (speed 0x380, action 4) to a
 * point a third of the way from the owner's part to obj's home point (param9C, paramA0, saved in
 * pixels by obj_store_base_as_params), 0x30 above the home height. Spawns effect 0x1071 at the
 * base point and goes on to obj_begin_idle_at_base. */
void obj_begin_approach(struct ActorObj *obj)
{
    struct ActorObj *owner;
    struct ActorPart *part;
    u8 *flags;

    if (obj->pose->flags & 8) /* animation over */
    {
        sound_stop_song(0xD9);
        set_slot_value(0xD9, -1);
        owner = obj->owner;
        obj->targetX = (part = owner->part)->targetX + ((obj->param9C << 8) - part->targetX) / 3;
        obj->targetY = part->targetY + ((obj->paramA0 << 8) - part->targetY) / 3;
        obj->targetH = (obj->paramA4 + 0x30) << 8;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x380;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->mode = 0x10;
        effect_spawn_at(0x1071, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_begin_idle_at_base;
    }
}

void obj_begin_idle_at_base(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        obj->targetX = obj->baseX;
        obj->targetY = obj->baseY;
        obj->targetH = obj->baseH - 0x3000;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x280;
        obj->motionStart = idx;
        start_motion(obj);
        sprite_set_action(obj, 5, 0, 0);
        obj_list_push_default(obj);
        obj->handler58 = (s32 (*)(struct ActorObj *, u32))obj_unlink_reset;
        f = obj->mode;
        f |= 0x20;
        obj->mode = f;
        obj->timer = 0;
        obj->paramA8 = 0;
        obj->state = obj_count_timer_then_move;
    }
}

/* Once the current animation has ended: restarts sound 0xD9 and flies (speed 0x380, action 4) to the
 * owner's part, 0x30 pixels above obj's home height, or 0x48 if obj_count_timer_then_move saw the
 * owner off the ground (paramA8). Spawns effect 0x1071 at the base point and goes on to
 * obj_begin_drop_to_base. */
void obj_begin_move_to_owner(struct ActorObj *obj)
{
    struct ActorObj *owner;
    struct ActorPart *part;
    u8 *flags;

    if (obj->pose->flags & 8) /* animation over */
    {
        sound_stop_song(0xD9);
        set_slot_value(0xD9, -1);
        owner = obj->owner;
        obj->targetX = (part = owner->part)->targetX;
        obj->targetY = part->targetY;
        if (obj->paramA8 == 0)
        {
            obj->targetH = (obj->paramA4 + 0x30) << 8;
        }
        else
        {
            obj->targetH = (obj->paramA4 + 0x48) << 8;
        }
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x380;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        obj->pose->mode = 0x10;
        effect_spawn_at(0x1071, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_begin_drop_to_base;
    }
}

void obj_begin_drop_to_base(struct ActorObj *obj)
{
    u8 *flags;

    if (obj->index == 0)
    {
        sound_stop_song(0xD9);
        set_slot_value(0xD9, -1);
        obj->targetX = obj->baseX;
        obj->targetY = obj->baseY;
        if ((s16)obj->timer == 0)
            obj->targetH = obj->baseH - 0x5000;
        else
            obj->targetH = obj->baseH - 0x6000;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x280;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 5, 0, 0);
        obj->state = obj_begin_move_to_params;
    }
}

struct PoseBit11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

void obj_begin_move_to_params(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        obj->targetX = obj->param9C << 8;
        obj->targetY = obj->paramA0 << 8;
        obj->targetH = obj->paramA4 << 8;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x300;
        obj->motionStart = idx;
        start_motion(obj);
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseBit11 *)&obj->pose->unk11)->bit6 = 0;
        obj->state = obj_wait_action7;
    }
}

u32 obj_unlink_reset(struct ActorObj *obj, u32 a, u32 b)
{
    partner_link_update(obj, a, b);
    obj->index = 0;
    obj->state = obj_begin_move_to_params;
    return 0;
}

void obj_wait_action7(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = obj_action0_to_idle;
    }
}

void obj_count_timer_then_move(struct ActorObj *obj)
{
    if (obj->timer <= 0x3FFF)
    {
        if (obj->owner->baseH > 0)
        {
            obj->paramA8 = 1;
        }
    }
    obj->timer++;
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0x10;
        obj->state = obj_begin_move_to_owner;
    }
}

void obj_begin_hover_at_base(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        obj->targetX = obj->baseX;
        obj->targetY = obj->baseY;
        obj->targetH = obj->baseH - 0x3000;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x280;
        obj->motionStart = idx;
        start_motion(obj);
        sprite_set_action(obj, 5, 0, 0);
        obj->state = obj_action3_begin;
    }
}

s32 obj_store_base_as_params(struct ActorObj *obj)
{
    obj->param9C = obj->baseX / 0x100;
    obj->paramA0 = obj->baseY / 0x100;
    obj->paramA4 = obj->baseH / 0x100;
    obj->state = base_moves_obj_wait_ready_action2;
    return 1;
}

void obj_action0_to_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_action3_begin(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0x10;
        obj->state = obj_begin_approach;
    }
}

void base_moves_obj_wait_ready_action2(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_action3_wait_pose;
    }
}

void obj_action3_wait_pose(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0x10;
        obj->state = obj_pose_active_aim_at_owner_part;
    }
}

void obj_tick_counter_place(struct ActorObj *obj)
{
    obj->paramA0++;
    if (obj->paramA0 >= obj->paramA4)
    {
        obj->paramA0 = 0;
        if (obj->param9C == 0)
        {
            effect_spawn_at(0x125C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0x1274, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
}

/* State: once no other state is busy, plays action 2, spawns the variant's effect (0x1254 for
 * variant 0, else 0x126C) at obj's base point in pixels, plays sound 0x88 (set_slot_value queues
 * the song) and goes on to obj_begin_table_move. */
void base_moves_obj_wait_ready_place_sprite(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        if (obj->param9C == 0)
        {
            effect_spawn_at(0x1254, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            set_slot_value(0x88, -1);
        }
        else
        {
            effect_spawn_at(0x126C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            set_slot_value(0x88, -1);
        }
        obj->state = obj_begin_table_move;
    }
}

void obj_tick_action4_timer(struct ActorObj *obj);
/* State: once the pose's animation ends, moves to the owner part's target plus the variant's
 * gParamTable offset at its speed, plays action 3, restarts the effect counter (one effect every
 * `duration` frames, see obj_tick_counter_place), starts sound 0xD8 (variant 0) or 0xD9 and waits
 * for the move in obj_tick_action4_timer. */
void obj_begin_table_move(struct ActorObj *obj)
{
    s32 variant;
    struct ActorPart *part;
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        variant = obj->param9C;
        obj->targetX = (part = obj->owner->part)->targetX + (gParamTable[variant].dx << 8);
        obj->targetY = part->targetY + (gParamTable[variant].dy << 8);
        obj->targetH = part->targetH + (gParamTable[variant].dh << 8);
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = gParamTable[variant].speed;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 3, 0, 0);
        obj->paramA0 = 0;
        obj->paramA4 = gParamTable[variant].duration;
        if (obj->param9C == 0)
        {
            set_slot_value(0xD8, -1);
        }
        else
        {
            set_slot_value(0xD9, -1);
        }
        obj->state = obj_tick_action4_timer;
    }
}

/* State (after obj_timer_down_action5): keeps spawning the variant's trail effects
 * (obj_tick_counter_place); once the pose's animation ends, jumps up to the variant's gParamTable
 * height (unk14 pixels) above the owner part's target at its speed (unk18), plays action 6 and waits
 * for the jump in obj_tick_wait_table_timer. */
void obj_begin_table_jump(struct ActorObj *obj)
{
    s32 variant;
    struct ActorPart *part;
    u8 *flags;

    obj_tick_counter_place(obj);
    if (obj->pose->flags & 8) /* animation over */
    {
        variant = obj->param9C;
        obj->targetX = (part = obj->owner->part)->targetX;
        obj->targetY = part->targetY;
        obj->targetH = part->targetH + (gParamTable[variant].unk14 << 8);
        flags = &obj->flags;
        *flags |= 0x20; /* start_motion: targetX/Y/H are a position, not an offset */
        obj->motionSpeed = gParamTable[variant].unk18;
        obj->motionStart = 0;
        start_motion(obj);
        obj_list_push_default(obj);
        sprite_set_action(obj, 6, 0, 0);
        obj->state = obj_tick_wait_table_timer;
    }
}

void obj_tick_wait_table_timer(struct ActorObj *obj)
{
    obj_tick_counter_place(obj);
    if (obj->index == 0)
    {
        s32 idx = obj->param9C;

        obj->timer = gParamTable[idx].unk1C;
        obj->state = obj_timer_down_idle7;
    }
}

void obj_tick_action4_timer(struct ActorObj *obj)
{
    obj_tick_counter_place(obj);
    if (obj->index == 0)
    {
        s32 idx;

        sprite_set_action(obj, 4, 0, 0);
        idx = obj->param9C;
        obj->timer = gParamTable[idx].unk10;
        if (obj->param9C == 0)
        {
            sound_stop_song(0xD8);
            set_slot_value(0x88, -1);
        }
        else
        {
            sound_stop_song(0xD9);
            set_slot_value(0x88, -1);
        }
        obj->state = obj_timer_down_action5;
    }
}

s32 unknown_part_steps_08062044(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = base_moves_obj_wait_ready_place_sprite;
    return 1;
}

s32 sub_08062058(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = base_moves_obj_wait_ready_place_sprite;
    return 1;
}

void obj_timer_down_idle7(struct ActorObj *obj)
{
    s32 t;

    obj_tick_counter_place(obj);
    t = obj->timer;
    t--;
    obj->timer = t;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_timer_down_action5(struct ActorObj *obj)
{
    s32 t;

    obj_tick_counter_place(obj);
    if (obj->pose->flags & 8)
    {
        t = obj->timer;
        t--;
        obj->timer = t;
        if ((s16)t <= 0)
        {
            sprite_set_action(obj, 5, 0, 0);
            obj->state = obj_begin_table_jump;
        }
    }
}

void obj_slot_notify_action(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        set_slot_value(0, -1);
        if (obj->paramA0 == 0)
        {
            set_slot_value(0x88, -1);
        }
        else
        {
            set_slot_value(0x88, -1);
        }
        switch (obj->param9C & 1)
        {
        case 0:
            sprite_set_action(obj, 3, 0, 0);
            obj->timer = 4;
            break;
        case 1:
            sprite_set_action(obj, 7, 0, 0);
            obj->timer = 4;
            break;
        }
        obj->state = obj_timer_action4_or_8;
    }
}

void obj_update_scroll_left(struct ActorObj *obj);
void obj_action0_to_idle_b(struct ActorObj *obj);
/* State: once the throw animation is done (pose flag 8), plays the throw effect, starts the
   follow-up action (5, or 9 when param9C is set) and spawns a projectile (kind 0x40CB, or 0x40CE
   when paramA0 is set) beside the actor. The projectile flies left in a straight line from where
   it starts towards a target picked by param9C: 0/1 the owner's part's target, 2/3/other the
   screen points (0x30, 0x30)/(0x30, 0x68)/(0x30, 0x98) (obj_update_scroll_left). The actor then
   waits for its animation to end (obj_action0_to_idle_b); a sound marks the throw. */
void obj_spawn_child_sprite(struct ActorObj *obj)
{
    struct ActorObj *shot;

    if (obj->pose->flags & 8)
    {
        set_slot_value(0, -1);
        effect_spawn_at(0xC7F, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 9, 0, 0);
        }
        /* spawned 0x11 px to the left and 0x13 px up, facing the way the actor faces */
        if (obj->paramA0 == 0)
        {
            shot = obj_spawn(obj->part, 0x40CB, obj->baseX / 0x100 - 0x11, obj->baseY / 0x100 + 2,
                             obj->baseH / 0x100 + 0x13, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        }
        else
        {
            shot = obj_spawn(obj->part, 0x40CE, obj->baseX / 0x100 - 0x11, obj->baseY / 0x100,
                             obj->baseH / 0x100 + 0x13, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        }
        obj->child = shot;
        ((struct Flags78Bits *)&shot->dir)->kind = 1;
        obj_child_flags(obj, shot);
        shot->handler68 = on_arrive;
        shot->link = obj->link;
        shot->owner = obj->owner;
        obj_list_push_default(shot);
        shot->handler5C = 0;
        shot->handler60 = 0;
        shot->handler64 = 0;
        shot->byte74 = 3;
        /* start point in pixels; targetX/Y become the flight vector (8.8 fixed point) */
        shot->dx = shot->baseX / 0x100;
        shot->dy = shot->baseY / 0x100;
        switch (obj->param9C)
        {
        case 0:
        case 1:
            shot->targetX = obj->owner->part->targetX + shot->dx * -0x100;
            shot->targetY = obj->owner->part->targetY + shot->dy * -0x100;
            break;
        case 2:
            shot->targetX = shot->dx * -0x100 + 0x3000;
            shot->targetY = shot->dy * -0x100 + 0x3000;
            break;
        case 3:
            shot->targetX = shot->dx * -0x100 + 0x3000;
            shot->targetY = 0x6800 + shot->dy * -0x100;
            break;
        default:
            shot->targetX = shot->dx * -0x100 + 0x3000;
            shot->targetY = 0x9800 + shot->dy * -0x100;
            break;
        }
        shot->state = obj_update_scroll_left;
        obj->state = obj_action0_to_idle_b;
        if (obj->paramA0 == 0)
        {
            set_slot_value(0x9C, -1);
        }
        else
        {
            set_slot_value(0x85, -1);
        }
    }
}

// Once the object's animation ends (pose flag 8): plays action 0xB and effect 0xC8E and
// throws a child object 0x40CC from 0x18 units above itself to the point (param9C, paramA0).
// The child flies along a parabola (obj_update_jump_arc): the flight lasts one frame per 0x80
// units of distance (0x100 when it is 0x3000 or more), and the arc peaks 0x1E (0x50) units high.
void obj_spawn_child_glide(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 duration;
    s32 curve;
    s32 rounded;
    s32 half;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        effect_spawn_at(0xC8E, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->part, 0x40CC, obj->baseX / 0x100, obj->baseY / 0x100 + 2,
                          obj->baseH / 0x100 + 0x18, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        ((struct Flags78Bits *)&child->dir)->kind = 1;
        obj_child_flags(obj, child);
        child->handler68 = on_arrive;
        child->link = obj->link;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->handler5C = 0;
        child->handler60 = 0;
        child->handler64 = 0;
        child->byte74 = 3;

        // Start (dx, dy, dh, whole units) and target (8.8 fixed point) of the flight.
        child->targetX = obj->param9C << 8;
        child->targetY = obj->paramA0 << 8;
        child->dx = child->baseX / 0x100;
        child->dy = child->baseY / 0x100;
        child->dh = child->baseH / 0x100;
        dx = child->targetX - child->baseX;
        dy = child->targetY - child->baseY;
        dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);

        // Height = peak - curve * (timer + 6 - duration / 2)^2, so curve = peak / half^2.
        if (dist <= 0x2FFF)
        {
            duration = dist / 0x80;
            half = duration / 2 - 5;
            curve = 0x1E00 / (half * half);
            child->targetH = 0x1E;
        }
        else
        {
            rounded = dist;
            if (rounded < 0)
            {
                rounded += 0xFF;
            }
            duration = rounded >> 8;
            half = (((s32)(duration + ((u32)rounded >> 31))) >> 1) - 5;
            curve = 0x5000 / (half * half);
            child->targetH = 0x50;
        }
        if (curve == 1)
        {
            curve = 2;
        }
        child->param9C = -curve;
        child->paramA0 = duration;
        child->paramA4 = dx;
        child->paramA8 = dy;
        child->timer = 0;
        child->state = obj_update_jump_arc;
        obj->state = obj_action0_then_spawn;
        set_slot_value(0x57, -1);
    }
}

// Waits until the object thrown by obj_spawn_child_glide is gone (obj->child is NULL again), then
// plays sound 0xAE and effect 0xC95 and spawns object 0x4011 with action 0xC where it landed
// (param9C, paramA0), facing the way obj's sprite does, as obj's new child. Goes on to wait_for_index.
void obj_spawn_child_when_none(struct ActorObj *obj)
{
    struct ActorObj *spawned;
    struct ActorObj *thrown;

    thrown = obj->child;
    if (thrown == 0)
    {
        set_slot_value(0xAE, -1);
        effect_spawn_at(0xC95, obj->param9C, obj->paramA0 + 2, obj->baseH / 256, obj);
        spawned = obj_spawn(obj->part, 0x4011, obj->param9C, obj->paramA0, obj->baseH,
                            ((u32)obj->pose->unk11 << 25) >> 31 /* unk11 bit 6: mirrored */, 1);
        sprite_set_action(spawned, 0xC, 0, 0);
        obj->child = spawned;
        ((struct Flags78Bits *)&spawned->dir)->kind = 1;
        obj_child_flags(obj, spawned);
        spawned->handler68 = on_arrive;
        spawned->owner = obj->owner;
        spawned->link = (s32)thrown; /* 0 */
        obj->timer = 1;
        obj->state = wait_for_index;
    }
}

/* One frame of the flight set up by obj_spawn_child_glide: x and y go linearly from the start
 * (dx, dy) by the distance (paramA4, paramA8) over paramA0 frames, the height follows the parabola
 * peak (targetH) + curve (param9C, negative) * (frame + 6 - paramA0 / 2)^2 above dh, and the pose's
 * unkC turns by 0x800. Once the height drops below 4 units, the timer is set to -1 and the object
 * is released. */
void obj_update_jump_arc(struct ActorObj *obj)
{
    s32 fromPeak;
    s32 drop;

    obj->pose->unkC += 0x800;
    obj->posX = (obj->paramA4 * *(s16 *)&obj->timer) / obj->paramA0 + (obj->dx << 8);
    obj->posY = (obj->paramA8 * *(s16 *)&obj->timer) / obj->paramA0 + (obj->dy << 8);
    fromPeak = *(s16 *)&obj->timer;
    fromPeak += 6;
    fromPeak -= obj->paramA0 / 2;
    drop = fromPeak * fromPeak;
    drop = obj->param9C * drop;
    obj->posH = drop + (obj->targetH << 8) + (obj->dh << 8);
    obj->timer++;
    if (obj->posH <= 0x3FF)
    {
        obj->timer |= 0xFFFF;
        obj_release(obj);
    }
}

void obj_action0_then_spawn(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = obj_spawn_child_when_none;
    }
}

void obj_update_scroll_left(struct ActorObj *obj)
{
    s32 x;

    obj->pose->unkC += 0x1600;
    x = obj->posX;
    x += -0x280;
    obj->posX = x;
    obj->posY = ((x - (obj->dx << 8)) * obj->targetY) / obj->targetX + (obj->dy << 8);
    if (x < -0x20)
    {
        obj_release(obj);
    }
}

void obj_action0_to_idle_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_timer_action4_or_8(struct ActorObj *obj)
{
    s32 t;

    if (obj->pose->flags & 8)
    {
        t = obj->timer;
        t--;
        obj->timer = t;
        if ((s16)t <= 0)
        {
            if ((obj->param9C & 1) == 0)
            {
                sprite_set_action(obj, 4, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 8, 0, 0);
            }
            obj->state = obj_spawn_child_sprite;
        }
    }
}

s32 obj_bump_param_9c_a(struct ActorObj *obj)
{
    s32 *p = &obj->param9C;
    u32 v = *p;

    if (v > 2)
    {
        v = 2;
    }
    else
    {
        v += 2;
    }
    *p = v;
    obj->paramA0 = 1;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

s32 obj_param_from_slot_a(struct ActorObj *obj)
{
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->paramA0 = 1;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

s32 unknown_part_steps_080628f0(struct ActorObj *obj)
{
    obj->state = obj_remove_child_idle;
    return 1;
}

s32 obj_bump_param_9c_b(struct ActorObj *obj)
{
    s32 *p = &obj->param9C;
    u32 v = *p;

    if (v > 2)
    {
        v = 2;
    }
    else
    {
        v += 2;
    }
    *p = v;
    obj->paramA0 = 0;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

s32 unknown_part_steps_08062928(struct ActorObj *obj)
{
    obj->state = obj_wait_ready_spawn_glide;
    return 1;
}

s32 obj_param_from_slot_b(struct ActorObj *obj)
{
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->paramA0 = 0;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

void obj_remove_child_idle(struct ActorObj *obj)
{
    if (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    obj->state = wait_for_index;
}

void obj_wait_ready_spawn_glide(struct ActorObj *obj)
{
    if (obj->child != NULL)
    {
        obj->state = wait_for_index;
    }
    else if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->state = obj_spawn_child_glide;
    }
}

void obj_wait_ready_action_flag(struct ActorObj *obj)
{
    s32 bit;

    if (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    if (is_state_busy() == 0)
    {
        bit = obj->param9C & 1;
        switch (bit)
        {
        case 0:
            sprite_set_action(obj, 2, 0, 0);
            break;
        case 1:
            sprite_set_action(obj, 6, 0, 0);
            break;
        }
        obj->state = obj_slot_notify_action;
    }
}

u32 obj_try_link_slot(struct ActorObj *obj, struct ActorObj *partner);
u32 obj_release_link_idle(struct ActorObj *obj, struct ActorObj *partner);
void obj_wait_timer_sprite_notify(struct ActorObj *obj);

/* Once the current animation has finished (pose flag 8): joins the contact list, with
 * obj_release_link_idle (handler58) and obj_try_link_slot (handler60) as its callbacks, starts
 * moving to the owner's target point (its own height kept) at speed 0x199, plays action 9,
 * spawns effect 0xED2 at its base and starts sound 0x81. obj_wait_timer_sprite_notify then
 * repeats the effect until the move ends. */
void obj_begin_linked_wait(struct ActorObj *obj)
{
    struct ActorPart *ownerPart;
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        obj_list_push_default(obj);
        obj->handler58 = (s32 (*)(struct ActorObj *, u32))obj_release_link_idle;
        obj->handler60 = (s32 (*)(struct ActorObj *))obj_try_link_slot;
        obj->targetX = (ownerPart = obj->owner->part)->targetX;
        obj->targetY = ownerPart->targetY;
        obj->targetH = obj->baseH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x199;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 9, 0, 0);
        effect_spawn_at(0xED2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0;
        obj->state = obj_wait_timer_sprite_notify;
        set_slot_value(0x81, -1);
    }
}
