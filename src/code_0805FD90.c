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
    u8 filler64[0x68 - 0x64];
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

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

// The child takes over bits 5 and 6 of the parent's flag byte at +0x77.
static inline void copy_flags77_bits(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

// The spawned child takes over the kind and two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child, u32 kind)
{
    ((struct Flags78Bits *)&child->dir)->kind = kind >> 5;
    copy_flags77_bits(obj, child);
}

s32 is_state_busy(void);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_release(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void map_obj_wait_action2(struct ActorObj *obj);
void map_obj_place_at_base(struct ActorObj *obj);
void map_obj_place_owner_sprite(struct ActorObj *obj);
void obj_pose_active_action0_clear(struct ActorObj *obj);
void obj_pose_active_choose_motion(struct ActorObj *obj);
void pose_steps_obj_wait_ready_action2_b(struct ActorObj *obj);
void sound_stop_song(u32 song);
void map_obj_action5_spawn(struct ActorObj *obj);
void pose_steps_obj_pose_active_spawn_child(struct ActorObj *obj);
void obj_pose_active_action0c_timer8(struct ActorObj *obj);
void obj_pose_active_action8(struct ActorObj *obj);
void obj_pose_active_timer4_action9(struct ActorObj *obj);
void obj_pose_active_action6_timer1(struct ActorObj *obj);
void obj_pose_active_aim_part_action3(struct ActorObj *obj);
void obj_pos_x_dec_then_cull(struct ActorObj *obj);
void obj_pose_active_callback_clear_link(struct ActorObj *obj);
void obj_pose_active_action2(struct ActorObj *obj);
void obj_pose_active_timer_action7_effect(struct ActorObj *obj);
void obj_wait_ready_action2_external(struct ActorObj *obj);
void pose_steps_obj_pose_active_timer_action_d(struct ActorObj *obj);
void pose_steps_obj_index_zero_action4(struct ActorObj *obj);
s32 obj_query_link_slot_wait(struct ActorObj *obj);
void obj_timer_down_then_idle(struct ActorObj *obj);
void obj_pose_active_action5(struct ActorObj *obj);
void obj_pose_active_timer_action_a(struct ActorObj *obj);
void obj_pose_active_spawn_child_b(struct ActorObj *obj);
void obj_move_and_cull(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_link_slot(struct ActorObj *obj);
s32 query_contact(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
extern u32 gUnk_03000E3C;
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void map_obj_action4_wait(struct ActorObj *obj);
void map_obj_settle_count(struct ActorObj *obj);
void obj_index_zero_reset_x(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_advance_x_until_param(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void obj_pose_active_action1_timer8(struct ActorObj *obj);
void obj_index_zero_setup_action4(struct ActorObj *obj);
void obj_index_zero_slot_action4(struct ActorObj *obj);
void obj_index_zero_slot_action4_b(struct ActorObj *obj);
s32 obj_query_part_start_motion(struct ActorObj *obj);
void obj_part_ba_zero_finish(struct ActorObj *obj);
void obj_slide_step_then_wrap(struct ActorObj *obj);
s32 obj_query_part_place_and_finish(struct ActorObj *obj);
s32 obj_place_sprite_1118_and_finish(struct ActorObj *obj, u32 a);
void map_obj_spawn_child(struct ActorObj *obj);
void obj_timer_down_action7_then_idle(struct ActorObj *obj);
void obj_arc_step_timer(struct ActorObj *obj);
void obj_drift_fast(struct ActorObj *obj);
void obj_drift_slow(struct ActorObj *obj);
s32 obj_query_part_settle(struct ActorObj *obj);
void obj_pose_active_action5_then_spawn(struct ActorObj *obj);

void map_obj_settle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        if (obj->param9C == 0)
        {
            obj->state = wait_for_index;
        }
        else
        {
            if (obj->param9C == 1)
            {
                obj->paramA0 = 0;
            }
            else
            {
                obj->paramA0 = 0x96;
            }
            obj->state = map_obj_settle_count;
        }
    }
}

s32 unknown_part_steps_0805fde4(struct ActorObj *obj)
{
    obj->param9C = 2;
    obj->state = map_obj_wait_action2;
    return 1;
}

s32 map_obj_wait_mode1(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = map_obj_wait_action2;
    return 1;
}

s32 map_obj_wait_mode0(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = map_obj_wait_action2;
    return 1;
}

void map_obj_settle_count(struct ActorObj *obj)
{
    obj->paramA0--;
    if (obj->paramA0 < 0)
    {
        obj->state = wait_for_index;
    }
}

void map_obj_wait_action2(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        set_slot_value(0x118, -1);
        obj->state = map_obj_place_at_base;
    }
}

void map_obj_place_at_base(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        effect_spawn_at(0x12D5, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = map_obj_action4_wait;
    }
}

void map_obj_action4_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = map_obj_action5_spawn;
    }
}

/* Once obj's animation has played out (pose flag 8): plays action 3 and sound 0x3A, and marks the
 * target position of the owner's part with effect 0xDCE when obj's dy is 0 (the fast shot of
 * map_obj_spawn_child), else 0xE1F (the slow one). Goes on to map_obj_spawn_child. */
void map_obj_place_owner_sprite(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        set_slot_value(0x3A, -1);
        if (obj->dy == 0)
        {
            effect_spawn_at(0xDCE, obj->owner->part->targetX / 0x100, obj->owner->part->targetY / 0x100,
                            obj->owner->part->targetH / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0xE1F, obj->owner->part->targetX / 0x100, obj->owner->part->targetY / 0x100,
                            obj->owner->part->targetH / 0x100, obj);
        }
        obj->state = map_obj_spawn_child;
    }
}

/* Once obj's animation has played out (pose flag 8): plays action 4 with effect 0xE64 at obj's base
 * point and launches a child (0x4132, kind 2, facing like obj) left of it. The child flies along
 * the line from 0x20 px left of obj's base point (paramA4/A8) towards the target position of the
 * owner's part, fast (obj_drift_fast) when obj's dy is 0, else slow (obj_drift_slow), and settles
 * on contact (obj_query_part_settle). Sound 0x85 plays and obj goes on in
 * obj_pose_active_action5_then_spawn. */
void map_obj_spawn_child(struct ActorObj *obj)
{
    struct ActorObj *shot;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        effect_spawn_at(0xE64, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        // PoseBits6.bit6: the pose is mirrored
        shot = obj_spawn(obj->part, 0x4132, obj->baseX / 0x100 - 0x20, obj->baseY / 0x100 + 2,
                         obj->baseH / 0x100 + 0x10, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
        obj->child = shot;
        obj_child_flags(obj, shot, 0x40);
        shot->handler68 = on_arrive;
        shot->link = obj->link;
        shot->owner = obj->owner;
        obj_list_push_default(shot);
        shot->handler5C = (s32 (*)(struct ActorObj *))obj_query_part_settle;
        shot->handler60 = (s32 (*)(struct ActorObj *))obj_query_part_settle;
        shot->byte74 = obj->byte74;
        shot->timer = 0;
        shot->param9C = obj->param9C;
        shot->paramA0 = obj->paramA0;
        // the flight line: from (paramA4, paramA8) to (targetX, targetY)
        shot->paramA4 = obj->baseX - 0x2000;
        shot->paramA8 = obj->baseY;
        shot->targetX = obj->owner->part->targetX;
        shot->targetY = obj->owner->part->targetY;
        if (obj->dy == 0)
        {
            shot->state = obj_drift_fast;
            shot->dx = 0x40;
        }
        else
        {
            shot->state = obj_drift_slow;
            shot->dx = 2;
        }
        set_slot_value(0x85, -1);
        obj->state = obj_pose_active_action5_then_spawn;
    }
}

/* Once obj's animation has played out (pose flag 8): obj starts a charge. It plays action 6, joins the
 * active list with obj_query_part_start_motion as its contact handlers (on a hit it flies back to
 * (param9C, paramA0)) and spawns sprite 0x4131 at its base point, facing like obj. That sprite gets
 * obj's flags bits 5-6, link, owner and byte74, and is released once the part's valueBA is 0
 * (obj_part_ba_zero_finish). Sound 0x86 plays and obj slides left in obj_slide_step_then_wrap. */
void pose_steps_obj_pose_active_spawn_child(struct ActorObj *obj)
{
    struct ActorObj *effect;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        obj->dh = 1;
        obj_list_push_default(obj);
        obj->handler5C = (s32 (*)(struct ActorObj *))obj_query_part_start_motion;
        obj->handler60 = (s32 (*)(struct ActorObj *))obj_query_part_start_motion;
        ((struct ObjFlags77 *)obj)->bit5 = 1;
        // PoseBits6.bit6: the pose is mirrored
        effect = obj_spawn(obj->part, 0x4131, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100,
                           ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
        ((struct Flags79Bit0 *)&effect->flags)->bit0 = 1;
        copy_flags77_bits(obj, effect);
        effect->handler68 = on_arrive;
        effect->link = obj->link;
        effect->owner = obj->owner;
        effect->byte74 = obj->byte74;
        effect->state = obj_part_ba_zero_finish;
        obj->timer = 0; /* counts frames between the trail effects of the slide */
        set_slot_value(0x86, -1); /* plays sound 0x86 */
        obj->state = obj_slide_step_then_wrap;
    }
}

/* The charge started by pose_steps_obj_pose_active_spawn_child: obj slides left, speeding up by
 * 0x10 (dy set) or 0x13 a frame, along the line from (param9C, paramA0) toward (targetX, targetY),
 * leaving trail effect 0xE72 every 8 frames. Once it is off the left edge it leaves the active list,
 * reappears at the right (x = 272 px, y = paramA0) and slides back in obj_advance_x_until_param. */
void obj_slide_step_then_wrap(struct ActorObj *obj)
{
    s32 lineDX = obj->param9C - obj->targetX;
    s32 lineDY = obj->paramA0 - obj->targetY;

    if (obj->dy != 0)
    {
        *(u16 *)&obj->dx += 0x10;
    }
    else
    {
        *(u16 *)&obj->dx += 0x13;
    }
    obj->posX -= obj->dx;
    obj->posY = (obj->posX - obj->param9C) * lineDY / lineDX + obj->paramA0;
    {
        s32 frames = obj->timer;

        frames++;
        obj->timer = frames;
        if ((s16)frames > 7)
        {
            obj->timer = 0;
            effect_spawn_at(0xE72, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
    if (obj->posX < -0x20)
    {
        obj_list_remove(obj);
        obj->posX = 0x11000;
        obj->posY = obj->paramA0;
        obj->state = obj_advance_x_until_param;
    }
}

/* The fast shot launched by map_obj_spawn_child: it flies left, speeding up by 0xC a frame, along
 * the line from (paramA4, paramA8) toward (targetX, targetY), leaving trail effect 0xE7A every 12
 * frames. Once it is more than 0x20 px off the left edge of the screen it is released. */
void obj_drift_fast(struct ActorObj *obj)
{
    s32 screen[3];
    s32 lineDX = obj->paramA4 - obj->targetX;
    s32 lineDY = obj->paramA8 - obj->targetY;

    *(u16 *)&obj->dx += 0xC;
    obj->posX -= obj->dx;
    obj->posY = (obj->posX - obj->paramA4) * lineDY / lineDX + obj->paramA8;
    /* position in pixels, made relative to the camera */
    screen[0] = obj->posX / 0x100;
    screen[1] = obj->posY / 0x100;
    screen[2] = obj->posH / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    {
        s32 frames = obj->timer;

        frames++;
        obj->timer = frames;
        if ((s16)frames > 0xB)
        {
            obj->timer = 0;
            effect_spawn_at(0xE7A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
    if (screen[0] < -0x20)
    {
        obj_release(obj);
    }
}

/* The slow shot launched by map_obj_spawn_child: like obj_drift_fast, but it speeds up by only 4 a
 * frame. It flies left along the line from (paramA4, paramA8) toward (targetX, targetY), leaving
 * trail effect 0xE7A every 12 frames, and is released once it is more than 0x20 px off the left edge
 * of the screen. */
void obj_drift_slow(struct ActorObj *obj)
{
    s32 screen[3];
    s32 lineDX = obj->paramA4 - obj->targetX;
    s32 lineDY = obj->paramA8 - obj->targetY;

    *(u16 *)&obj->dx += 4;
    obj->posX -= obj->dx;
    obj->posY = (obj->posX - obj->paramA4) * lineDY / lineDX + obj->paramA8;
    /* position in pixels, made relative to the camera */
    screen[0] = obj->posX / 0x100;
    screen[1] = obj->posY / 0x100;
    screen[2] = obj->posH / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    {
        s32 frames = obj->timer;

        frames++;
        obj->timer = frames;
        if ((s16)frames > 0xB)
        {
            obj->timer = 0;
            effect_spawn_at(0xE7A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
    if (screen[0] < -0x20)
    {
        obj_release(obj);
    }
}

s32 obj_query_part_settle(struct ActorObj *obj)
{
    s32 result = query_contact(obj);
    s32 ret;

    if (result == 0)
    {
        obj->dx = obj->dx / 2;
        obj->dy = 0;
        obj->dh = 0;
        sprite_set_action(obj, 1, 0, 0);
        obj->state = obj_move_and_cull;
        ret = 0;
    }
    else
    {
        ret = result;
    }
    return ret;
}
