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
    s16 cursorX;
    s16 cursorY;
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
    u8 filler78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 index;
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

struct ActorFlags77
{
    u8 unk0 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 unk7 : 1;
};

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
extern u32 gUnk_03000E3C;

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

void place_then_wait_slide_b(struct ActorObj *obj)
{
    obj->wait--;
    if (obj->wait <= 0)
    {
        effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->wait = 0xA;
    }
    if (obj->index == 0)
    {
        obj->state = slide_finish_then_fire;
    }
}

void place_then_wait_slide_a(struct ActorObj *obj)
{
    obj->wait--;
    if (obj->wait <= 0)
    {
        effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->wait = 0xA;
    }
    if (obj->index == 0)
    {
        obj->state = slide_fire_then_finish;
    }
}

u32 lookup_slot_place_finish(struct ActorObj *obj, u32 slot)
{
    u32 found = lookup_slot(obj, slot);

    if (found != 0)
    {
        return found;
    }
    effect_spawn_at(0x2423, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
    sound_stop_song(0x8C);
    return 0;
}

void stop_when_pose_active_d(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

s32 step_to_anim_0a(struct ActorObj *obj)
{
    obj->state = set_anim_0a_notify_af;
    return 1;
}

s32 set_counter_by_owner_slot(struct ActorObj *obj)
{
    if (obj->timer == 0)
    {
        obj->timer = 3;
    }
    if (obj->owner->part->slot == -1)
    {
        obj->paramA8 = 0;
    }
    else
    {
        obj->paramA8 = 1;
    }
    obj->state = set_anim_02_place_part;
    return 1;
}

s32 step_to_anim_0e(struct ActorObj *obj)
{
    obj->state = set_anim_0e_next;
    return 1;
}

s32 sub_0807680C(struct ActorObj *obj)
{
    obj->phase = 2;
    obj->state = set_anim_by_phase_a;
    return 1;
}

s32 sub_08076820(struct ActorObj *obj)
{
    obj->phase = 1;
    obj->state = set_anim_by_phase_a;
    return 1;
}

s32 sub_08076834(struct ActorObj *obj)
{
    obj->phase = 0;
    obj->state = set_anim_by_phase_a;
    return 1;
}

void set_anim_0a_notify_af(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xA, 0, 0);
    obj->state = place_then_start_move;
    set_slot_value(0xAF, -1);
}

void set_anim_02_place_part(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    set_slot_value(0x12B, -1);
    effect_spawn_at(0x243D, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj->state = start_move_then_wait_slot;
}

void set_anim_0e_next(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xE, 0, 0);
    obj->state = set_anim_0f_start_wait;
}

void set_anim_by_phase_a(struct ActorObj *obj)
{
    if (obj->timer == 0)
    {
        obj->timer = 0x100;
    }
    switch (obj->phase)
    {
    case 0:
        sprite_set_action(obj, 2, 0, 0);
        break;
    case 1:
        sprite_set_action(obj, 6, 0, 0);
        break;
    default:
        sprite_set_action(obj, 0xA, 0, 0);
        break;
    }
    obj->state = set_anim_by_phase_b;
}

void set_anim_0f_start_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        set_slot_value(0x157, -1);
        sprite_set_action(obj, 0xF, 0, 0);
        obj->wait = 0x10;
        obj->targetX = 0x32;
        obj->targetH = 0;
        obj->state = wait_phase_world_update;
        obj->phase = 0x20;
    }
}

void set_anim_by_phase_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        set_slot_value(0x6C, -1);
        switch (obj->phase)
        {
        case 0:
            sprite_set_action(obj, 3, 0, 0);
            break;
        case 1:
            sprite_set_action(obj, 7, 0, 0);
            break;
        default:
            sprite_set_action(obj, 0xB, 0, 0);
            break;
        }
        obj->targetX = obj->paramA0;
        obj->wait = 8;
        obj->state = place_parts_by_phase_then_wait;
    }
}

void wait_phase_world_update(struct ActorObj *obj)
{
    add_obj_target_offset(gWorld->item, -1, 0);
    obj->phase--;
    if (obj->phase <= 0)
    {
        obj->state = periodic_spawn_step;
    }
}

void spawn_actor_children(struct ActorObj *obj)
{
    struct ActorObj *children[2];
    struct ActorCand *cand;
    s32 i;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        for (i = 0; i < 2; i++)
        {
            cand = (struct ActorCand *)gWorld->actors[i + 6];
            children[i] = obj_spawn(obj->part, 0x4047, cand->obj.part->targetX / 0x100,
                                    cand->obj.part->targetY / 0x100, cand->obj.part->targetH / 0x100 + 0x28, 0, 1);
            sprite_set_action(children[i], obj->timer, 0, 0);
            children[i]->child = obj->child;
            obj->child = children[i];
            ((struct ActorFlags79 *)&children[i]->flags79)->bit0 = 1;
            ((struct ActorFlags77 *)&children[i]->flags77)->bit6 = 0;
            ((struct ActorFlags77 *)&children[i]->flags77)->bit5 = 0;
            children[i]->callback = on_arrive;
            children[i]->link = obj->link;
            children[i]->owner = obj->owner;
            obj_list_push_default(children[i]);
            children[i]->handler58 = (s32 (*)(struct ActorObj *, u32))step_countdown_notify_or_place;
            children[i]->wait = obj->timer;
            children[i]->state = 0;
            effect_spawn_at(0x24C2, children[i]->posX / 0x100, children[i]->posY / 0x100,
                            children[i]->posH / 0x100, children[i]);
        }
        set_slot_value(0x119, -1);
        obj->state = set_anim_04_place_part;
    }
}
