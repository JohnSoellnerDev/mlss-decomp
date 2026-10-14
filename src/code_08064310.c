#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct Flags77Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct PoseBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorPart
{
    u8 filler0[0xB4];
    u16 unkB4;
    u8 filler1[0xD8 - 0xB6];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler2[0xEC - 0xE4];
    s16 slot;
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
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
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

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_release(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 is_state_busy(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
u32 lookup_part(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_query_part_action3(struct ActorObj *obj);
void obj_pose_active_effect_c(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
extern u32 gUnk_03000E18;
extern s32 gUnk_03000E60;
extern s32 gUnk_03000E64;
u32 poll_spline_path(u32 buf, s32 *x, s32 *h);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void heap_free_b(u32 ptr);
void set_targets_from_record(struct ActorObj *obj);
void obj_motion_from_owner(struct ActorObj *obj);
void obj_place_sprite_finish_b(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj);

void obj_timer_effect_b(struct ActorObj *obj);
void obj_action7_wait(struct ActorObj *obj);
void obj_wait_ready_action3(struct ActorObj *obj);
u32 obj_query_effect(struct ActorObj *obj);
void obj_effect_action_c(struct ActorObj *obj);
void obj_drift_left(struct ActorObj *obj);
void obj_countdown_then_action1(struct ActorObj *obj);
void obj_pose_active_action0_ext(struct ActorObj *obj);
void parts_obj_pose_active_timer_action_d(struct ActorObj *obj);
void obj_spawn_part_sprite(struct ActorObj *obj);
void obj_slide_step_then_cull(struct ActorObj *obj);
void parts_obj_spawn_child_launch(struct ActorObj *obj);
void obj_update_position_timer(struct ActorObj *obj);
u32 obj_slot_check_action1(struct ActorObj *obj);
void obj_pose_active_stop(struct ActorObj *obj);
u32 obj_query_part_then_state(struct ActorObj *obj);
u32 obj_lookup_slot_then_state(struct ActorObj *obj);
void obj_pose_active_action0_next(struct ActorObj *obj);
u32 unknown_part_steps_08064b14(struct ActorObj *obj);
u32 sub_08064B20(struct ActorObj *obj);
u32 sub_08064B34(struct ActorObj *obj);
u32 sub_08064B40(struct ActorObj *obj);
u32 sub_08064B54(struct ActorObj *obj);
void obj_move_xh_then_cull(struct ActorObj *obj);
void obj_restart_action6(struct ActorObj *obj);
void parts_obj_wait_ready_place_sprite(struct ActorObj *obj);
void parts_obj_fall_step(struct ActorObj *obj);
void obj_place_sprite_and_finish(struct ActorObj *obj);
void actor_parts_steps(struct ActorObj *obj);
void obj_action3_restart_spawn(struct ActorObj *obj);
void obj_wait_ready_choose_state(struct ActorObj *obj);
void obj_timer_up_then_idle(struct ActorObj *obj);
void obj_pose_active_action3(struct ActorObj *obj);
void parts_obj_pose_active_clear_state(struct ActorObj *obj);
void obj_active_place_then_wait(struct ActorObj *obj);
void parts_obj_spawn_child_b(struct ActorObj *obj);
void unknown_code_08065044(struct ActorObj *obj);
void obj_poll_buffer_or_free(struct ActorObj *obj);
void obj_timer_start_motion(struct ActorObj *obj);
void obj_wait_then_slide_and_finish(struct ActorObj *obj);
void obj_move_step_c(struct ActorObj *obj);
void obj_move_step_d(struct ActorObj *obj);
void obj_glide_step_finish(struct ActorObj *obj);
void obj_wait_flag_then_cull(struct ActorObj *obj);
void obj_wait_then_state_5619(struct ActorObj *obj);
void obj_pose_active_action5_or_9(struct ActorObj *obj);
u32 obj_check_flag_a0_zero(struct ActorObj *obj);
u32 obj_check_flag_a0_one(struct ActorObj *obj);
void parts_obj_pose_active_idle(struct ActorObj *obj);
void parts_obj_wait_ready_action2(struct ActorObj *obj);
void obj_pose_active_place_sprite_a(struct ActorObj *obj);
void obj_timer_spawn_child_a(struct ActorObj *obj);
void obj_timer_spawn_child_b(struct ActorObj *obj);
void obj_timer_spawn_effect_place(struct ActorObj *obj);
u32 obj_lookup_place_cull(struct ActorObj *obj);
void obj_pose_active_finish(struct ActorObj *obj);
void obj_pose_active_effect_b(struct ActorObj *obj);
u32 obj_lookup_slot_then_state_bc0(struct ActorObj *obj);
void obj_move_place_cull(struct ActorObj *obj);
void obj_no_sprite_idle(struct ActorObj *obj);

void obj_timer_effect_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 0xB, 0, 0);
            effect_from_table_obj(obj, 0x1471);
            obj->handler5C = (void (*)(struct ActorObj *))obj_query_effect;
            set_slot_value(0x83, -1);
            obj->state = obj_effect_action_c;
        }
    }
}

void obj_action7_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = obj_drift_left;
    }
}

void obj_wait_ready_action3(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_motion_from_owner;
    }
}

u32 obj_query_effect(struct ActorObj *obj)
{
    u32 result = lookup_part(obj);

    if (result == 0)
    {
        obj->state = obj_countdown_then_action1;
        obj->targetX = 0x10;
    }
    return result;
}

void obj_effect_action_c(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        effect_from_table_obj(obj, 0x1485);
        obj->handler5C = (void (*)(struct ActorObj *))obj_query_effect;
        obj->timer = 3;
        obj->state = parts_obj_pose_active_timer_action_d;
    }
}

ASM_FUNC("asm/code_08064310/obj_drift_left.s", void obj_drift_left(struct ActorObj *obj))

void obj_countdown_then_action1(struct ActorObj *obj)
{
    s32 t = obj->targetX;

    t--;
    obj->targetX = t;
    if (t <= 0)
    {
        sprite_set_action(obj, 1, 0, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_action0_ext;
    }
}

void obj_pose_active_action0_ext(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = set_targets_from_record;
    }
}

void parts_obj_pose_active_timer_action_d(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 0xD, 0, 0);
            obj->state = obj_pose_active_action0_ext;
        }
    }
}

ASM_FUNC("asm/code_08064310/obj_spawn_part_sprite.s", void obj_spawn_part_sprite(struct ActorObj *obj))

ASM_FUNC("asm/code_08064310/obj_slide_step_then_cull.s", void obj_slide_step_then_cull(struct ActorObj *obj))

ASM_FUNC("asm/code_08064310/parts_obj_spawn_child_launch.s", void parts_obj_spawn_child_launch(struct ActorObj *obj))

void obj_update_position_timer(struct ActorObj *obj)
{
    s32 t;
    s32 sq;
    s32 hb;

    obj->pose->unkC += 0x400;
    obj->posX = (obj->paramA4 * *(s16 *)&obj->timer) / obj->paramA0 + (obj->dx << 8);
    obj->posY = (obj->paramA8 * *(s16 *)&obj->timer) / obj->paramA0 + (obj->dy << 8);
    t = *(s16 *)&obj->timer - obj->paramA0 / 2;
    sq = t * t;
    sq = obj->param9C * sq;
    hb = obj->dh << 8;
    hb += 0x4000;
    obj->posH = sq + hb;
    obj->timer++;
    if (obj->posH <= 0)
    {
        obj_release(obj);
    }
}

u32 obj_slot_check_action1(struct ActorObj *obj)
{
    u32 result = lookup_slot(obj);

    if (result != 0)
    {
        return result;
    }
    sprite_set_action(obj, 1, 0, 0);
    obj->state = obj_move_xh_then_cull;
    return 0;
}

void obj_pose_active_stop(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

u32 obj_query_part_then_state(struct ActorObj *obj)
{
    u32 result = query_contact(obj);

    if (result != 0)
    {
        return result;
    }
    obj->state = parts_obj_fall_step;
    return 0;
}

u32 obj_lookup_slot_then_state(struct ActorObj *obj)
{
    u32 result = lookup_slot(obj);

    if (result != 0)
    {
        return result;
    }
    obj->state = obj_place_sprite_and_finish;
    return 0;
}

void obj_pose_active_action0_next(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = actor_parts_steps;
    }
}

u32 unknown_part_steps_08064b14(struct ActorObj *obj)
{
    obj->state = parts_obj_wait_ready_place_sprite;
    return 1;
}

u32 sub_08064B20(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = obj_wait_ready_choose_state;
    return 1;
}

u32 sub_08064B34(struct ActorObj *obj)
{
    obj->state = obj_restart_action6;
    return 1;
}

u32 sub_08064B40(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = obj_wait_ready_choose_state;
    return 1;
}

u32 sub_08064B54(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = obj_action3_restart_spawn;
    return 1;
}

void obj_move_xh_then_cull(struct ActorObj *obj)
{
    obj->posX += 0x200;
    obj->posH -= 0x380;
    if (obj->posH <= 0)
    {
        obj_release(obj);
    }
}

void obj_restart_action6(struct ActorObj *obj)
{
    sprite_set_action(obj, 6, 0, 0);
    obj->state = parts_obj_spawn_child_launch;
}

void parts_obj_wait_ready_place_sprite(struct ActorObj *obj)
{
    u32 busy = is_state_busy(obj);

    if (busy == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        effect_spawn_at(0x1089, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = busy;
        obj->state = obj_timer_up_then_idle;
    }
}

void parts_obj_fall_step(struct ActorObj *obj)
{
    s32 xOld = obj->posX;
    s32 xNew = xOld - 0x219;
    s32 hOld;
    s32 hNew;
    s32 xChk;
    s32 hChk;

    obj->posX = xNew;
    hOld = obj->posH;
    hNew = hOld - 0x100;
    obj->posH = hNew;
    xChk = xNew;
    if (xChk < 0)
    {
        xChk = xOld - 0x11A;
    }
    if ((xChk >> 8) > -0x20)
    {
        hChk = hNew;
        if (hChk < 0)
        {
            hChk = hOld - 1;
        }
        if ((hChk >> 8) > 0)
        {
            return;
        }
    }
    obj_release(obj);
}

void obj_place_sprite_and_finish(struct ActorObj *obj)
{
    effect_spawn_at(0x1081, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
}

void actor_parts_steps(struct ActorObj *obj)
{
    obj->state = 0;
}

void obj_action3_restart_spawn(struct ActorObj *obj)
{
    sprite_set_action(obj, 3, 0, 0);
    obj->state = obj_spawn_part_sprite;
}

void obj_wait_ready_choose_state(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        ((struct PoseBits *)&obj->pose->flags)->mode = 1;
        if (obj->param9C == 0)
        {
            obj->state = parts_obj_pose_active_clear_state;
        }
        else
        {
            set_slot_value(0xAF, -1);
            obj->state = obj_pose_active_action3;
        }
    }
}

void obj_timer_up_then_idle(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t++;
    obj->timer = t;
    if (((t << 16) >> 16) == 0xF)
    {
        set_slot_value(0xB5, -1);
    }
    if (obj->pose->flags & 8)
    {
        obj->state = 0;
    }
}

void obj_pose_active_action3(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_spawn_part_sprite;
    }
}

void parts_obj_pose_active_clear_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->state = 0;
    }
}

void obj_active_place_then_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 3, 0, 0);
            effect_spawn_at(0x1284, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        else
        {
            sprite_set_action(obj, 4, 0, 0);
            effect_spawn_at(0x128D, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        if (obj->paramA0 == 0)
        {
            set_slot_value(0x28, -1);
        }
        else
        {
            set_slot_value(0x28, -1);
        }
        obj->state = obj_pose_active_action5_or_9;
    }
}

ASM_FUNC("asm/code_08064310/parts_obj_spawn_child_b.s", void parts_obj_spawn_child_b(struct ActorObj *obj))

ASM_FUNC("asm/code_08064310/unknown_code_08065044.s", void unknown_code_08065044(struct ActorObj *obj))

void obj_poll_buffer_or_free(struct ActorObj *obj)
{
    s32 pos[3];

    obj->pose->unkC += 0x1000;
    if ((u8)poll_spline_path(gUnk_03000E18, &gUnk_03000E60, &gUnk_03000E64) == 1)
    {
        obj->posX = gUnk_03000E60;
        obj->posH = gUnk_03000E64;
        pos[0] = obj->posX / 0x100;
        pos[1] = obj->posY / 0x100;
        pos[2] = obj->posH / 0x100 + 0x20;
        offset_three_words_camera(&pos[0], &pos[1], &pos[2], 0);
    }
    else
    {
        heap_free_b(gUnk_03000E18);
        obj->timer = 0;
        obj->state = obj_timer_start_motion;
    }
}

void obj_timer_start_motion(struct ActorObj *obj)
{
    s32 t;

    obj->pose->unkC += 0x1000;
    t = obj->timer;
    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->targetH = obj->part->targetH;
        ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
        obj->motionSpeed = 0x900;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_wait_flag_then_cull;
    }
}

void obj_wait_then_slide_and_finish(struct ActorObj *obj)
{
    obj->pose->unkC += 0x1000;
    if (obj->posH <= 0x7FF && *(s16 *)&obj->timer != 0)
    {
        obj->timer--;
    }
    else
    {
        obj->posX -= obj->param9C;
        obj->posY = (obj->dy * ((obj->posX - obj->targetX) << 8)) / (obj->dx << 8) + obj->targetY;
        if (obj->posH > 0x800)
        {
            obj->posH += obj->paramA0;
            obj->paramA0 -= 0x2C;
        }
        if (obj->posX - obj->owner->part->targetX <= 0x5DFF)
        {
            obj->param9C -= 0x19;
        }
        if (obj->param9C < -0x280)
        {
            obj->param9C = -0x280;
        }
        if (obj->posX > obj->targetX)
        {
            obj->part->unkB4 = 1;
            obj_release(obj);
        }
    }
}

void obj_move_step_c(struct ActorObj *obj)
{
    s32 x;

    obj->pose->unkC += 0x1000;
    x = obj->posX - obj->param9C;
    obj->posX = x;
    obj->posY = (obj->dy * ((x - obj->targetX) << 8)) / (obj->dx << 8) + obj->targetY;
    x -= obj->owner->part->targetX;
    if (x <= 0x5DFF)
    {
        obj->param9C -= 0x11;
    }
    if (obj->posH > 0x800)
    {
        obj->posH -= 0xE6;
    }
    if (obj->posX <= 0x3FFF)
    {
        if (obj->posY > 0x6800)
        {
            obj->paramA0 = -0x60;
        }
        else
        {
            obj->paramA0 = 0x60;
        }
        obj->state = obj_move_step_d;
    }
}

void obj_move_step_d(struct ActorObj *obj)
{
    obj->pose->unkC += 0x1000;
    obj->posX -= obj->param9C;
    obj->param9C -= 0x11;
    obj->posY += obj->paramA0;
    if (obj->posX > 0x4000)
    {
        obj->dx = (obj->targetX - obj->baseX) / 0x100;
        obj->dy = (obj->targetY - obj->baseY) / 0x100;
        obj->state = obj_glide_step_finish;
    }
}

void obj_glide_step_finish(struct ActorObj *obj)
{
    obj->pose->unkC += 0x1000;
    obj->posX -= obj->param9C;
    obj->posY = (obj->dy * ((obj->posX - obj->targetX) << 8)) / (obj->dx << 8) + obj->targetY;
    obj->param9C -= 0x11;
    if (obj->posH <= 0xFFF)
    {
        obj->posH += 0xE6;
    }
    if (obj->posX > obj->targetX)
    {
        obj->part->unkB4 = 1;
        obj_release(obj);
    }
}

void obj_wait_flag_then_cull(struct ActorObj *obj)
{
    obj->pose->unkC += 0x1000;
    if (obj->index == 0)
    {
        obj->part->unkB4 = 1;
        obj_release(obj);
    }
}

void obj_wait_then_state_5619(struct ActorObj *obj)
{
    s16 t = *(s16 *)&obj->timer;

    if (t != 0)
    {
        if (t == 1)
        {
            if (obj->paramA0 == 0)
            {
                sound_stop_song(0x117);
                sprite_set_action(obj, 7, 0, 0);
            }
            else
            {
                sound_stop_song(0x117);
                sprite_set_action(obj, 0xB, 0, 0);
            }
        }
        else
        {
            if (obj->paramA0 == 0)
            {
                sprite_set_action(obj, 8, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 0xC, 0, 0);
            }
        }
        obj->state = parts_obj_pose_active_idle;
    }
}

void obj_pose_active_action5_or_9(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->paramA0 == 0)
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 9, 0, 0);
        }
        obj->state = parts_obj_spawn_child_b;
    }
}

u32 obj_check_flag_a0_zero(struct ActorObj *obj)
{
    obj->paramA0 = 0;
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = parts_obj_wait_ready_action2;
    return 1;
}

u32 obj_check_flag_a0_one(struct ActorObj *obj)
{
    obj->paramA0 = 1;
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = parts_obj_wait_ready_action2;
    return 1;
}

void parts_obj_pose_active_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

void parts_obj_wait_ready_action2(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_active_place_then_wait;
    }
}

void obj_pose_active_place_sprite_a(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        effect_spawn_at(0xEDB, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0x4A;
        effect_spawn_at(0xEEA, obj->owner->part->targetX / 0x100 + 0x30, obj->owner->part->targetY / 0x100,
                        obj->baseH / 0x100 + 0x48, obj);
        obj->timer = 0x4A;
        set_slot_value(0x10C, -1);
        obj->state = obj_timer_spawn_child_a;
    }
}

ASM_FUNC("asm/code_08064310/obj_timer_spawn_child_a.s", void obj_timer_spawn_child_a(struct ActorObj *obj))

ASM_FUNC("asm/code_08064310/obj_timer_spawn_child_b.s", void obj_timer_spawn_child_b(struct ActorObj *obj))

void obj_timer_spawn_effect_place(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 1, 0, 0);
            gUnk_03000E3C = effect_from_table_obj(obj, 0x14F2);
            obj->handler5C = (void (*)(struct ActorObj *))obj_lookup_place_cull;
            effect_spawn_at(0xF16, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            obj->state = obj_pose_active_effect_b;
        }
    }
}

u32 obj_lookup_place_cull(struct ActorObj *obj)
{
    u32 result = query_contact(obj);

    if (result == 0)
    {
        effect_spawn_at(0xF24, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
        obj->state = obj_pose_active_finish;
    }
    return result;
}

void obj_pose_active_finish(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj_release(obj);
    }
}

void obj_pose_active_effect_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 2, 0, 0);
        gUnk_03000E3C = effect_from_table_obj(obj, 0x1509);
        obj->handler5C = obj_query_part_action3;
        obj->state = obj_pose_active_effect_c;
    }
}

u32 obj_lookup_slot_then_state_bc0(struct ActorObj *obj)
{
    u32 result = lookup_slot(obj);

    if (result != 0)
    {
        return result;
    }
    obj->state = obj_place_sprite_finish_b;
    return 0;
}

ASM_FUNC("asm/code_08064310/obj_move_place_cull.s", void obj_move_place_cull(struct ActorObj *obj))

void obj_no_sprite_idle(struct ActorObj *obj)
{
    if (obj->child == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}
