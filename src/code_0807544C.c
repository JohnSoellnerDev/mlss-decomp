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

void approach_then_place_a(struct ActorObj *obj)
{
    struct ActorCand *cand;
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
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 0xC, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 0xA;
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
            cand = gWorld->cand74;
            obj->targetX = obj->owner->part->targetX + 0x2000;
            obj->targetY = cand->obj.part->targetY;
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
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 0xC, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 0xA;
    }
    obj->posX -= obj->paramA0;
    if (obj->posX / 0x100 <= -0x20)
    {
        sound_stop_song(0x8C);
        obj_release(obj);
    }
}

void approach_then_place_b(struct ActorObj *obj)
{
    struct ActorCand *cand;
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
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 0xC, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 0xA;
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
            cand = gWorld->cand70;
            obj->targetX = obj->owner->part->targetX + 0x2000;
            obj->targetY = cand->obj.part->targetY;
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
            effect_spawn_at(0x23C3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100 - 0xC, obj);
        }
        else
        {
            effect_spawn_at(0x23CC, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->wait = 0xA;
    }
    obj->posX -= obj->paramA0;
    if (obj->posX / 0x100 <= -0x20)
    {
        obj_release(obj);
        sound_stop_song(0x8C);
    }
}

ASM_FUNC("asm/code_0807544C/place_steps_spawn_random_child.s", void place_steps_spawn_random_child(struct ActorObj *obj))

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

void periodic_spawn_step(struct ActorObj *obj)
{
    struct ActorCand *candA;
    struct ActorCand *candB;

    candA = gWorld->cand70;
    candB = gWorld->cand74;
    if (candA->obj.index == 0 && candB->obj.index == 0)
    {
        obj->targetH--;
        if (obj->targetH <= 0)
        {
            place_steps_spawn_random_child(obj);
            obj->targetH = 0x3C;
        }
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        effect_spawn_at(0x23D5, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->wait = 0x10;
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
        cursor = menu_cursor_pos(obj->part->cursorX, obj->part->cursorY, 0xA);
        sign = signed_byte_e4(obj->part);
        popup_digit_spawn(cursor - obj->part->cursorX, obj->baseX / 0x100, obj->baseY / 0x100, sign, 1);
        obj->part->cursorX = cursor;
        sprite_set_action(obj, 0, 0, 0);
        obj->state = phase_wait_idle;
        obj->phase = 0x20;
    }
}

ASM_FUNC("asm/code_0807544C/kind_place_or_finish.s", s32 kind_place_or_finish(struct ActorObj *obj, struct ActorObj *owner))

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

void effect_timer_step(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
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

ASM_FUNC("asm/code_0807544C/loop_targets_then_move.s", void loop_targets_then_move(struct ActorObj *obj))
