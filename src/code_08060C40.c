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

void obj_pose_active_action0c_timer8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        obj->timer = 8;
        obj->state = pose_steps_obj_pose_active_timer_action_d;
    }
}

void obj_pose_active_action8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->state = obj_pose_active_timer4_action9;
    }
}

void pose_steps_obj_index_zero_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_pose_active_action0_clear;
    }
}

void obj_pose_active_action5(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = obj_pose_active_action6_timer1;
    }
}

void pose_steps_obj_wait_ready_action2_b(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_pose_active_choose_motion;
    }
}

void obj_timer_down_then_idle(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((s16)t <= 0)
    {
        obj->state = wait_for_index;
    }
}

void pose_steps_obj_pose_active_timer_action_d(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((s16)t < 0)
        {
            sprite_set_action(obj, 0xD, 0, 0);
            set_slot_value(0x83, -1);
            obj->state = obj_pose_active_action2;
        }
    }
}

void obj_pose_active_timer4_action9(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer = 4;
        sprite_set_action(obj, 9, 0, 0);
        effect_spawn_at(0x10F5, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_pose_active_timer_action_a;
    }
}

void obj_pose_active_action0_clear(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void obj_pose_active_action2(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_pose_active_aim_part_action3;
    }
}

void obj_pose_active_action6_timer1(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        obj->timer = 1;
        obj->state = obj_pose_active_timer_action7_effect;
    }
}

void obj_pose_active_timer_action_a(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((s16)t < 0)
        {
            sprite_set_action(obj, 0xA, 0, 0);
            effect_spawn_at(0x1107, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            obj->state = obj_pose_active_spawn_child_b;
        }
    }
}

void obj_pose_active_timer_action7_effect(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((s16)t < 0)
        {
            sprite_set_action(obj, 7, 0, 0);
            gUnk_03000E3C = effect_from_table_obj(obj, 0x1428);
            obj->handler60 = obj_query_link_slot_wait;
            set_slot_value(0x83, -1);
            obj->state = obj_pose_active_action2;
        }
    }
}

void obj_wait_ready_setup_motion(struct ActorObj *obj)
{
    s32 busy = is_state_busy();

    if (busy == 0)
    {
        s32 dX;
        s32 dY;
        s32 d;
        s32 t;
        s32 q;
        struct ActorObj *owner;

        obj_list_push_default(obj);
        set_slot_value(0xD8, -1);
        owner = obj->owner;
        obj->targetX = owner->part->targetX;
        obj->targetY = owner->part->targetY;
        obj->targetH = obj->part->targetH;
        obj->dx = obj->baseX / 0x100;
        obj->dy = obj->baseY / 0x100;
        obj->dh = obj->baseH / 0x100;
        dX = obj->targetX - obj->baseX;
        dY = obj->targetY - obj->baseY;
        d = ((s32 (*)(s32))gMathRoutines)(dX * dX + dY * dY);
        if (d < 0)
        {
            d += 0x1FF;
        }
        t = d >> 9;
        q = 0x1000 / ((t / 2) * (t / 2));
        if (q == 1)
        {
            q = 2;
        }
        obj->param9C = -q;
        obj->paramA0 = t;
        obj->paramA4 = dX;
        obj->paramA8 = dY;
        obj->timer = busy;
        sprite_set_action(obj, 5, 0, 0);
        effect_spawn_at(0x120A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_arc_step_timer;
    }
}
