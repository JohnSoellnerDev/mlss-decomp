#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xF6 - 0xE4];
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
};

struct ActorCands
{
    u8 filler0[0x70];
    u8 *cand70;
    u8 *cand74;
};

struct PoseModeBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct ActorFlags79Bits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 high : 2;
};

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
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
    void (*callback)(struct ActorObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 flags77;
    u8 flags78;
    u8 flags79;
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

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_release(struct ActorObj *obj);
void release_object(void *part);
void task_list_remove(void *part);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
u32 lookup_part(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
u32 is_state_busy(struct ActorObj *obj);
u32 mt19937_next(void);
void obj_spawn_child_412b(struct ActorObj *obj);
void obj_pick_random_target(struct ActorObj *obj);
void obj_index_zero_idle(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
void obj_find_slot(struct ActorObj *obj);
void jump_obj_fall_step(struct ActorObj *obj);
void obj_jump_arc_land(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
extern u32 gUnk_03000E3C;
extern struct ActorCands *gWorld;
void obj_link_owner_then_wait(struct ActorObj *obj);
void obj_query_part_then_idle(struct ActorObj *obj);

void obj_wait_child_then_place(struct ActorObj *obj);
void obj_timer_wave_state(struct ActorObj *obj);
void actor_start_pair(struct ActorObj *obj);
void sprite_drop_left(struct ActorObj *obj);
void sprite_slide_left(struct ActorObj *obj);
void actor_slide_left_to_target(struct ActorObj *obj);
void actor_wait_child_then_move(struct ActorObj *obj);
void actor_timed_restart(struct ActorObj *obj);
void actor_announce_then_act(struct ActorObj *obj);
u32 unknown_part_steps_080691a8(struct ActorObj *obj);
u32 actor_init_mode0(struct ActorObj *obj);
void actor_setup_part_wait(struct ActorObj *obj);
void actor_begin_action_c_or_f(struct ActorObj *obj);
void part_drift_right(struct ActorObj *obj);
void actor_wait_then_action_d(struct ActorObj *obj);
void actor_drop_to_ground(struct ActorObj *obj);
void actor_scroll_out_left(struct ActorObj *obj);
void actor_move_to_part_x(struct ActorObj *obj);
void actor_idle_reset(struct ActorObj *obj);
void actor_start_countdown(struct ActorObj *obj);
u32 actor_init_random_wait_mode1(struct ActorObj *obj);
u32 actor_init_random_wait_mode0(struct ActorObj *obj);
void actor_countdown_then_act(struct ActorObj *obj);
void actor_wait_ready_then_jump(struct ActorObj *obj);
void actor_start_entry(struct ActorObj *obj);
u32 actor_link_owner_scroll(struct ActorObj *obj);
u32 actor_lookup_part_then_idle(struct ActorObj *obj);
void actor_slide_left_to_part_x(struct ActorObj *obj);
void actor_fire_event_idle(struct ActorObj *obj);
void actor_action_b_idle(struct ActorObj *obj);
void actor_land_and_move(struct ActorObj *obj);
void actor_start_motion_wait(struct ActorObj *obj);
void actor_wait_index_action6(struct ActorObj *obj);
void actor_link_partners_jump(struct ActorObj *obj);
void actor_fall_place_sprite(struct ActorObj *obj);
u32 actor_lookup_slot_wait(struct ActorObj *obj);
void actor_timeout_release(struct ActorObj *obj);
void actor_wait_index_action4(struct ActorObj *obj);
u32 unknown_part_steps_08069c48(struct ActorObj *obj);
u32 actor_init_pose_wait(struct ActorObj *obj);
void actor_action9_idle(struct ActorObj *obj);
void actor_wait_ready_then_fly(struct ActorObj *obj);
void actor_fly_then_land_ext(struct ActorObj *obj);
void actor_move_countdown_next(struct ActorObj *obj);
void actor_wait_ready_show_sprite(struct ActorObj *obj);
void actor_spawn_child_4072(struct ActorObj *obj);
void actor_aim_and_count(struct ActorObj *obj);
void actor_jump_to_target_a(struct ActorObj *obj);
void actor_jump_to_target_b(struct ActorObj *obj);
void obj_jump_arc(struct ActorObj *obj);

void actor_land_and_move(struct ActorObj *obj)
{
    obj->posH += obj->paramA4;
    obj->paramA4 -= obj->paramA8;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        sprite_set_action(obj, 3, 0, 0);
        if (obj->dx == 0)
        {
            obj->targetX = obj->owner->part->targetX + (obj->param9C << 8);
            obj->targetY = obj->owner->part->targetY;
            obj->motionSpeed = 0x100;
        }
        else
        {
            obj->targetX = obj->param9C;
            obj->targetY = obj->paramA0;
            obj->motionSpeed = 0x100;
        }
        obj->targetH = obj->baseH;
        ((struct ActorFlags79Bits *)&obj->flags79)->bit5 = 1;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 0x14;
        obj->state = actor_wait_index_action4;
        if (obj->dx == 0)
            set_slot_value(0x81, -1);
        else
            set_slot_value(0x81, -1);
    }
}

void actor_start_motion_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 left = --obj->timer << 16;

        if (left <= 0)
        {
            sprite_set_action(obj, 5, 0, 0);
            if (obj->dx == 0)
            {
                obj->targetX = obj->owner->part->targetX;
                obj->targetY = obj->owner->part->targetY;
                obj->motionSpeed = obj->paramA0;
            }
            else
            {
                obj->targetX = 0x3000;
                obj->targetY = obj->baseY;
                obj->motionSpeed = 0x200;
            }
            obj->motionStart = 0;
            obj->targetH = obj->baseH;
            ((struct ActorFlags79Bits *)&obj->flags79)->bit5 = 1;
            start_motion(obj);
            obj_list_push_default(obj);
            obj->handler58 = 0;
            obj->timer = 0xF;
            if (obj->dx == 0)
                obj->handler60 = (void (*)(struct ActorObj *))actor_lookup_slot_wait;
            obj->state = actor_wait_index_action6;
            if (obj->dx == 0)
                set_slot_value(0x81, -1);
            else
                set_slot_value(0x81, -1);
        }
    }
}

void actor_wait_index_action6(struct ActorObj *obj)
{
    s16 dx = obj->dx;

    if (dx == 0 && (obj->owner->part->flags111 & 4))
    {
        obj->state = actor_fall_place_sprite;
        obj->paramA4 = 0x280;
        obj->paramA8 = 0x66;
        obj->index = dx;
        sprite_set_action(obj, 7, 0, 0);
        sound_stop_song(0x81);
        return;
    }
    {
        s32 left = --obj->timer << 16;

        if (left <= 0)
        {
            effect_spawn_at(0x1C9F, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            obj->timer = 0xF;
        }
    }
    if (obj->index == 0)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct PoseModeBits *)&obj->pose->flags)->mode = 2;
        obj->state = actor_link_partners_jump;
        if (obj->dx == 0)
        {
            sound_stop_song(0x81);
            set_slot_value(0x40, -1);
        }
        else
        {
            sound_stop_song(0x81);
            set_slot_value(0xCC, -1);
        }
    }
}

void actor_link_partners_jump(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->dx == 0)
        {
            partner_link_update(obj, obj->owner, 0xFF);
        }
        else
        {
            struct ActorCands *cands = gWorld;
            struct ActorObj *a = (struct ActorObj *)(cands->cand70 + 8);
            struct ActorObj *b = (struct ActorObj *)(cands->cand74 + 8);

            if (a->part->unkF6 != 0)
                partner_link_update(obj, a, 0xFF);
            if (b->part->unkF6 != 0)
                partner_link_update(obj, b, 0xFF);
        }
        if (obj->dx == 0)
            effect_spawn_at(0x1C8A, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
        else
            effect_spawn_at(0x1CB1, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
        obj->timer = 0x14;
        obj->state = actor_timeout_release;
    }
}

void actor_fall_place_sprite(struct ActorObj *obj)
{
    obj->posX -= 0x80;
    obj->posH += obj->paramA4;
    obj->paramA4 -= obj->paramA8;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        effect_spawn_at(0x1CA8, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
        sprite_set_action(obj, 8, 0, 0);
        obj->state = actor_action9_idle;
    }
}

u32 actor_lookup_slot_wait(struct ActorObj *obj)
{
    u32 found = lookup_slot(obj);

    if (found == 0)
    {
        if (obj->dx == 0)
            sound_stop_song(0x81);
        else
            sound_stop_song(0x81);
        obj->index = 0;
        obj->state = actor_wait_index_action6;
        return 0;
    }
    return found;
}

void actor_timeout_release(struct ActorObj *obj)
{
    s32 left = --obj->timer << 16;

    if (left > 0)
        return;
    release_object(obj->part);
    task_list_remove(obj->part);
}

void actor_wait_index_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        if (obj->dx == 0)
            sound_stop_song(0x81);
        else
            sound_stop_song(0x81);
        sprite_set_action(obj, 4, 0, 0);
        if (obj->dx == 0)
            obj->timer = 4;
        else
            obj->timer = 4;
        obj->state = actor_start_motion_wait;
    }
}

u32 unknown_part_steps_08069c48(struct ActorObj *obj)
{
    obj->dx = 1;
    obj->state = actor_wait_ready_then_fly;
    return 1;
}

u32 actor_init_pose_wait(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 0x48;
        obj->paramA0 = 0x200;
    }
    obj->dx = 0;
    obj->state = actor_wait_ready_then_fly;
    return 1;
}

void actor_action9_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = wait_for_index;
    }
}

void actor_wait_ready_then_fly(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->paramA4 = 0x300;
        obj->paramA8 = 0x66;
        obj->state = actor_land_and_move;
    }
}

void actor_fly_then_land_ext(struct ActorObj *obj)
{
    obj->posH += obj->targetH;
    obj->targetH -= obj->paramA8;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        sprite_set_action(obj, 3, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x3000;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->baseH;
        ((struct ActorFlags79Bits *)&obj->flags79)->bit5 = 1;
        obj->motionSpeed = 0x180;
        obj->motionStart = 0;
        start_motion(obj);
        obj_list_push_default(obj);
        obj->handler58 = obj_link_owner_then_wait;
        obj->handler5C = obj_query_part_then_idle;
        obj->state = obj_pick_random_target;
        set_slot_value(0x81, -1);
    }
}

void actor_move_countdown_next(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 n;

        sprite_set_action(obj, 4, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x3000;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->baseH;
        ((struct ActorFlags79Bits *)&obj->flags79)->bit5 = 1;
        obj->motionSpeed = 0x180;
        obj->motionStart = 0;
        start_motion(obj);
        n = obj->param9C - 1;
        obj->param9C = n;
        obj->state = n > 0 ? obj_pick_random_target : obj_index_zero_idle;
    }
}

void actor_wait_ready_show_sprite(struct ActorObj *obj);
void actor_spawn_child_4072(struct ActorObj *obj);
void actor_aim_and_count(struct ActorObj *obj);
void actor_jump_to_target_a(struct ActorObj *obj);
void actor_jump_to_target_b(struct ActorObj *obj);
void obj_jump_arc(struct ActorObj *obj);

void actor_wait_ready_show_sprite(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        set_slot_value(0x55, -1);
        if (obj->param9C == 1)
        {
            sprite_set_action(obj, 7, 0, 0);
            effect_spawn_at(0x1D5A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        else
        {
            sprite_set_action(obj, 9, 0, 0);
            effect_spawn_at(0x1D6E, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        obj->state = actor_spawn_child_4072;
    }
}

void actor_spawn_child_4072(struct ActorObj *obj)
{
    struct ActorObj *child;

    if (obj->pose->flags & 8)
    {
        if (obj->param9C == 1)
        {
            sprite_set_action(obj, 4, 0x2000, 0);
        }
        else
        {
            sprite_set_action(obj, 4, 0x204D, 0);
        }
        child = obj_spawn(obj->part, 0x4072, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100,
                          ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
        obj->child = child;
        ((struct ActorFlags79Bits *)&child->flags79)->bit0 = 1;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = 0;
        child->callback = on_arrive;
        child->link = obj->link;
        child->owner = obj->owner;
        child->byte74 = obj->byte74;
        ((struct ActorFlags79Bits *)&child->flags79)->bit3 = 0;
        obj_list_push_default(child);
        child->handler58 = 0;
        child->handler60 = (void (*)(struct ActorObj *))obj_find_slot;
        child->state = 0;
        child->param9C = 0;
        obj->targetX = 3;
        obj->targetY = obj->param9C;
        obj->state = actor_aim_and_count;
    }
}

void actor_aim_and_count(struct ActorObj *obj)
{
    struct ActorObj *child = obj->child;

    if (child->param9C != 0)
    {
        obj->param9C = 0x300;
        obj->paramA0 = 0x66;
        obj->state = jump_obj_fall_step;
        obj_release(child);
        return;
    }
    if (obj->pose->flags & 8)
    {
        s32 r7;
        s32 px;
        s32 py;
        s32 x;
        s32 y;
        s32 ratio;

        if (obj->targetY == 1)
            sprite_set_action(obj, 5, 0x2000, 0);
        else
            sprite_set_action(obj, 5, 0x204D, 0);
        if (obj->targetX > 1)
            obj->state = actor_jump_to_target_a;
        else if (obj->targetX == 1)
            obj->state = actor_jump_to_target_b;
        else
        {
            set_slot_value(0x30, -1);
            r7 = (mt19937_next() & 0xF) + 0x30;
            obj->targetH = r7;
            px = obj->owner->part->targetX;
            py = obj->owner->part->targetY;
            obj->dx = obj->baseX / 0x100;
            obj->dy = obj->baseY / 0x100;
            x = px - (obj->dx << 8);
            y = py - (obj->dy << 8);
            ((s32 (*)(s32))gMathRoutines)(x * x + y * y);
            ratio = (r7 << 8) / 400;
            if (ratio == 1)
                ratio = 2;
            obj->param9C = -ratio;
            obj->paramA0 = 0x28;
            obj->paramA4 = x;
            obj->paramA8 = y;
            obj->timer = 0;
            obj->state = obj_jump_arc_land;
        }
    }
}

void actor_jump_to_target_a(struct ActorObj *obj)
{
    struct ActorObj *child = obj->child;
    s32 cp = child->param9C;

    if (cp != 0)
    {
        obj->param9C = 0x300;
        obj->paramA0 = 0x66;
        obj->state = jump_obj_fall_step;
        obj_release(child);
    }
    else
    {
        s32 q;
        s32 r;
        s32 x;
        s32 y;
        s32 ratio;
        s32 px;

        set_slot_value(0x30, -1);
        px = obj->owner->part->targetX + 0x1000;
        q = (obj->baseX - px) / (obj->targetX + 1);
        mt19937_next();
        r = (mt19937_next() & 0xF) + 0x30;
        obj->targetH = r;
        x = obj->baseX - q;
        {
            s32 yb = ((mt19937_next() & 0x3F) + 0x50) << 8;

            obj->dx = obj->baseX / 0x100;
            obj->dy = obj->baseY / 0x100;
            obj->dh = obj->baseH / 0x100;
            x = x - (obj->dx << 8);
            y = yb - (obj->dy << 8);
        }
        ((s32 (*)(s32))gMathRoutines)(x * x + y * y);
        ratio = (r << 8) / 400;
        if (ratio == 1)
            ratio = 2;
        obj->param9C = -ratio;
        obj->paramA0 = 0x28;
        obj->paramA4 = x;
        obj->paramA8 = y;
        obj->timer = cp;
        obj->state = obj_jump_arc;
    }
}

void actor_jump_to_target_b(struct ActorObj *obj)
{
    struct ActorObj *child = obj->child;
    s32 cp = child->param9C;

    if (cp != 0)
    {
        obj->param9C = 0x300;
        obj->paramA0 = 0x66;
        obj->state = jump_obj_fall_step;
        obj_release(child);
    }
    else
    {
        s32 r;
        s32 px;
        s32 py;
        s32 x;
        s32 y;
        s32 ratio;

        set_slot_value(0x30, -1);
        r = (mt19937_next() & 0xF) + 0x30;
        obj->targetH = r;
        px = obj->owner->part->targetX + 0x1C00;
        py = obj->owner->part->targetY;
        obj->dx = obj->baseX / 0x100;
        obj->dy = obj->baseY / 0x100;
        obj->dh = obj->baseH / 0x100;
        x = px - (obj->dx << 8);
        y = py - (obj->dy << 8);
        ((s32 (*)(s32))gMathRoutines)(x * x + y * y);
        ratio = (r << 8) / 400;
        if (ratio == 1)
            ratio = 2;
        obj->param9C = -ratio;
        obj->paramA0 = 0x28;
        obj->paramA4 = x;
        obj->paramA8 = y;
        obj->timer = cp;
        obj->state = obj_jump_arc;
    }
}

NONMATCH("asm/code_08069778/obj_jump_arc.s", void obj_jump_arc(struct ActorObj *obj))
{
    struct ActorObj *child = obj->child;

    if (child->param9C != 0)
    {
        obj->param9C = 0x300;
        obj->paramA0 = 0x66;
        obj->state = jump_obj_fall_step;
        obj_release(child);
    }
    else
    {
        s32 x;
        s32 y;

        x = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
        obj->posX = x;
        y = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
        obj->posY = y;
        {
            s32 offset = (s16)obj->timer - obj->paramA0 / 2;

            obj->posH = (offset * offset) * obj->param9C + (obj->targetH << 8) + ((s16)obj->dh << 8);
        }
        child->posX = x;
        child->posY = obj->posY;
        child->posH = obj->posH;
        obj->timer++;
        if ((s16)obj->timer == obj->paramA0 / 2)
        {
            u32 variant;

            if (obj->targetY == 1)
                variant = 0x2000;
            else
                variant = 0x204D;
            sprite_set_action(obj, 6, variant, 0);
            ((struct PoseModeBits *)&obj->pose->flags)->mode = 1;
        }
        if ((s16)obj->timer > obj->paramA0)
        {
            if (obj->targetY == 1)
                sprite_set_action(obj, 4, 0x2000, 0);
            else
                sprite_set_action(obj, 4, 0x204D, 0);
            obj->targetX--;
            obj->posH = 0;
            obj->state = actor_aim_and_count;
        }
    }
}
END_NONMATCH
