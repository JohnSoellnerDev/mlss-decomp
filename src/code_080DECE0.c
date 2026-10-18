#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
};

struct PoseFlagBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct ActorData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 partner;
    u8 filler0EE[0x113 - 0xEE];
    u8 codeByte;
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
    struct ActorData *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    s32 (*handler58)(struct ActorObj *);
    s32 (*handler5C)(struct ActorObj *, s32, s32);
    u8 filler60[0x68 - 0x60];
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x78 - 0x75];
    u8 kind78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 link;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 codeId;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

struct FxSpawnList
{
    s32 pad0;
    s32 active;
    struct ActorObj *owner;
    u16 *cursor;
    s16 delay;
    s16 baseX;
    s16 baseY;
    s16 baseH;
};

struct Particle
{
    u8 filler0[0x4];
    s32 posX;
    s32 posH;
    s32 posY;
    s16 x;
    s16 dh;
    s16 y;
    s16 timer;
    s16 dx;
};

void obj_release(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void change_item_count(u32 id, s32 amount);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void on_arrive(struct ActorObj *obj);
extern u8 *gWorld;
extern u8 gItemTable[];
void set_slot_value(u32 id, s32 value);
void sprite_load_tables(u32 index);
void ensure_code_entry(u32 id);
u32 is_state_busy(void);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void obj_spawn_linked_child_jump(struct ActorObj *obj);
void obj_begin_move_pair(struct ActorObj *obj);
void obj_state_spawn_child(struct ActorObj *obj);
void obj_fall_settle_step(struct ActorObj *obj);
void obj_wait_idle_then_action12(struct ActorObj *obj);
void obj_pose_active_arrive_idle(struct ActorObj *obj);
void obj_wait_not_busy_action10(struct ActorObj *obj);
void obj_action10_pose_mode(struct ActorObj *obj);
void obj_pair_midpoint_move_then_f3b4(struct ActorObj *obj);
void obj_timer_pair_move_then_e640(struct ActorObj *obj);
void obj_timer_move_to_owner_then_f358(struct ActorObj *obj);
void obj_wait_not_busy_then_begin_pair(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorData *data, u32 kind, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_list_push_default(struct ActorObj *obj);
void obj_begin_jump_to_data(struct ActorObj *obj);
s32 obj_unlink_release_code(struct ActorObj *obj);
void obj_pose_active_arrive(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_state_idle_if_unlinked(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_wait_link_then_timer(struct ActorObj *obj);
u32 part_lookup_clear_11a(struct ActorObj *obj, u32 a, u32 b);
u32 part_unlink_clear_11a(struct ActorObj *obj, u32 a, u32 b);
void start_motion(struct ActorObj *obj);
void obj_load_code_entry_then_wait(struct ActorObj *obj);
void obj_select_code_entry_world(struct ActorObj *obj);
void obj_wait_not_busy_then_e321(struct ActorObj *obj);
void obj_finish_now(struct ActorObj *obj);
void obj_timer_begin_move_to_data(struct ActorObj *obj);
void obj_pose_active_action11(struct ActorObj *obj);
void obj_pose_active_action11_notify(struct ActorObj *obj);
static inline void copy_child_flags(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

void obj_state_spawn_child(struct ActorObj *obj)
{
    struct ActorPose *pose = obj->pose;

    if (pose->flags & 8)
    {
        struct ActorData *data = obj->data;

        if (data->codeByte & 0x80)
        {
            struct ActorObj *child;

            child = obj_spawn(data, *(u16 *)&obj->codeId, obj->baseX / 0x100 - 0x14,
                              obj->baseY / 0x100 - 1, obj->baseH / 0x100 + 0x14,
                              ((u32)pose->byte11 << 25) >> 31, 1);
            obj->child = child;
            child->kind78 = child->kind78 & 0x1F;
            copy_child_flags(obj, child);
            child->state = obj_begin_jump_to_data;
            child->handler68 = on_arrive;
            child->childValue = obj->childValue;
            child->owner = obj->owner;
            obj_list_push_default(child);
            child->handler58 = obj_unlink_release_code;
            child->param9C = obj->param9C;
            child->paramA0 = obj->paramA0;
            child->paramA4 = obj->paramA4;
            child->codeId = obj->codeId;
            child->byte74 = 0xFF;
            sprite_set_action(child, 0, 0, 0);
        }
        obj->childValue = 0;
        sprite_set_action(obj, 0xC, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_arrive;
    }
}

ASM_FUNC("asm/code_080DECE0/obj_fall_settle_step.s", void obj_fall_settle_step(struct ActorObj *obj))

ASM_FUNC("asm/code_080DECE0/obj_begin_move_pair.s", void obj_begin_move_pair(struct ActorObj *obj))

ASM_FUNC("asm/code_080DECE0/obj_timer_begin_move_to_data.s", void obj_timer_begin_move_to_data(struct ActorObj *obj))

ASM_FUNC("asm/code_080DECE0/obj_timer_release_move_pair.s", void obj_timer_release_move_pair(void))

void obj_state_idle_if_unlinked(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        obj->state = wait_for_index;
    }
}

u32 part_lookup_clear_11a(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x11A);
    return lookup_part(obj, a, b);
}

u32 part_unlink_clear_11a(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x11A);
    return partner_link_update(obj, a, b);
}

void obj_wait_link_then_timer(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        sound_stop_song(0x11C);
        sprite_set_action(obj, obj->owner->data->partner != -1 ? 0x11 : 0xF, 0, 0);
        obj->timer = obj->param9C;
        obj->state = obj_timer_begin_move_to_data;
    }
}

s32 obj_unlink_release_code(struct ActorObj *obj)
{
    obj->state = obj_finish_now;
    if (obj->data->codeByte & 0x80)
    {
        change_item_count(obj->data->codeByte & 0x7F, 1);
        obj->data->codeByte = 0;
    }
    return 1;
}

void obj_begin_jump_to_data(struct ActorObj *obj)
{
    s32 *ground = &obj->ground;
    struct ActorData *data = obj->owner->data;
    s32 targetH = data->targetH;

    *ground = targetH;
    obj->dhStep = jump_velocity(&obj->dx, (s16 *)((u8 *)ground + 0x18), &obj->dh,
                                data->targetX - obj->baseX, data->targetY - obj->baseY,
                                targetH - obj->baseH, 0x2800, 0x7CC, 0xFD, 0);
    obj->state = obj_fall_settle_step;
}

void obj_pose_active_arrive(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}

void obj_lower_height_512(struct ActorObj *obj)
{
    if (obj->posH > 0x200)
    {
        obj->posH = obj->posH - 0x200;
    }
    else
    {
        obj->posH = 0;
    }
}

void obj_arrive_toggle_pose(struct ActorObj *obj)
{
    struct ActorPose *pose;
    s32 flags;
    s32 bit;
    s32 cleared;

    if (obj->link == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        pose = obj->pose;
        flags = pose->byte11;
        bit = ((u32)flags << 25) >> 31;
        bit = bit ^ 1;
        bit = bit << 6;
        cleared = -0x41 & flags;
        cleared = cleared | bit;
        pose->byte11 = cleared;
        on_arrive(obj);
    }
}

void obj_action8_slot126_timer(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        set_slot_value(0x126, -1);
        obj->timer = 0x30;
        obj->state = obj_timer_pair_move_then_e640;
    }
}

u32 link_slot_lookup_clear_11c(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x11C);
    return lookup_link_slot(obj, a, b);
}

void obj_partner_action_timer(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        sprite_set_action(obj, obj->owner->data->partner != -1 ? 6 : 4, 0, 0);
        sound_stop_song(0x11C);
        obj->timer = obj->param9C;
        obj->state = obj_timer_move_to_owner_then_f358;
    }
}

s32 sprite_state_chain_e(struct ActorObj *obj)
{
    obj->state = obj_wait_not_busy_then_begin_pair;
    return 1;
}

s32 obj_enter_load_code_entry(struct ActorObj *obj)
{
    obj->state = obj_load_code_entry_then_wait;
    return 1;
}

s32 obj_enter_select_code_entry_world(struct ActorObj *obj)
{
    obj->state = obj_select_code_entry_world;
    return 1;
}

s32 obj_enter_wait_not_busy_then_e321(struct ActorObj *obj)
{
    obj->state = obj_wait_not_busy_then_e321;
    return 1;
}

s32 obj_random_next_state(struct ActorObj *obj)
{
    u32 pick = rand_below(4, mt19937_next());

    switch (pick)
    {
    case 0:
        obj->state = obj_wait_not_busy_then_e321;
        break;
    case 1:
        obj->state = obj_select_code_entry_world;
        break;
    case 2:
        obj->state = obj_load_code_entry_then_wait;
        break;
    case 3:
        obj->state = obj_wait_not_busy_then_begin_pair;
        break;
    }
    return 1;
}

void obj_wait_not_busy_then_begin_pair(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_begin_move_pair;
    }
}

void obj_finish_now(struct ActorObj *obj)
{
    obj_release(obj);
}

ASM_FUNC("asm/code_080DECE0/obj_load_code_entry_then_wait.s", void obj_load_code_entry_then_wait(struct ActorObj *obj))

void obj_select_code_entry_world(struct ActorObj *obj)
{
    u32 index = obj->data->codeByte;
    void (*next)(struct ActorObj *);

    if ((index & 0x80) == 0)
    {
        next = 0;
    }
    else if (index == 0xFF)
    {
        next = 0;
    }
    else
    {
        index = index & 0x7F;
        gWorld[0x349] = index;
        *(u8 **)(gWorld + 0x2DC) = gItemTable + (index << 4);
        sprite_load_tables(index);
        obj->motionSpeed = 0x100;
        next = obj_action10_pose_mode;
    }
    obj->state = next;
}

void obj_wait_not_busy_then_e321(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_pair_midpoint_move_then_f3b4;
    }
}

void obj_wait_not_busy_action10(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_action11;
    }
}

void obj_action10_pose_mode(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xA, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = obj_pose_active_action11_notify;
}

void obj_pose_active_action11(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_state_spawn_child;
    }
}

void obj_pose_active_action11_notify(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        set_slot_value(0x128, -1);
        obj->state = obj_wait_idle_then_action12;
    }
}

void obj_wait_idle_then_action12(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        u32 busy = is_state_busy();

        if (busy == 0)
        {
            obj_spawn_linked_child_jump(obj);
            obj->childValue = busy;
            sprite_set_action(obj, 0xC, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            obj->state = obj_pose_active_arrive_idle;
        }
    }
}

void obj_pose_active_arrive_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}

ASM_FUNC("asm/code_080DECE0/fx_obj_spawn.s", void fx_obj_spawn(void))

ASM_FUNC("asm/code_080DECE0/effect_spawn_at.s", void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner))

ASM_FUNC("asm/code_080DECE0/fx_script_step.s", void fx_script_step(void))

void fx_spawn_list_step(struct FxSpawnList *fx)
{
    while (fx->delay == 0)
    {
        u16 *rec = fx->cursor;
        u32 type = rec[0];

        if (type == 0xFFFD)
        {
            effect_spawn_at(type, (s16)rec[1], (s16)rec[2], (s16)rec[3], 0);
        }
        else
        {
            s32 x = fx->baseX;
            struct ActorObj *owner = fx->owner;

            if (owner != 0 && (owner->pose->byte11 & 0x40))
            {
                x -= (s16)rec[1];
            }
            else
            {
                x += (s16)rec[1];
            }
            effect_spawn_at(type, x, fx->baseY + (s16)rec[2], fx->baseH + (s16)rec[3], owner);
        }
        fx->delay = rec[4];
        fx->cursor = rec + 5;
    }
    if (fx->delay < 0)
    {
        fx->active = 0;
    }
    else
    {
        fx->delay--;
    }
}

ASM_FUNC("asm/code_080DECE0/fx_tick_move_records.s", void fx_tick_move_records(void))

ASM_FUNC("asm/code_080DECE0/spawn_random_particle.s", void spawn_random_particle(void))

ASM_FUNC("asm/code_080DECE0/init_drift_particle.s", void init_drift_particle(void))

ASM_FUNC("asm/code_080DECE0/step_particle.s", void step_particle(void))

ASM_FUNC("asm/code_080DECE0/orbit_point.s", void orbit_point(void))

ASM_FUNC("asm/code_080DECE0/start_obj_frame.s", void start_obj_frame(void))

ASM_FUNC("asm/code_080DECE0/effect_obj_frame.s", void effect_obj_frame(void))
