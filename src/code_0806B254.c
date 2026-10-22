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
    u8 filler0E4[0xEC - 0xE4];
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

extern u32 gUnk_03000E3C;

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 arg, u32 extra);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void start_motion(struct ActorObj *obj);
u32 mt19937_next(void);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_link_slot(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj, u32 a, u32 b);
u32 is_state_busy(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void wait_for_index(struct ActorObj *obj);
void actor_stop_when_pose_active(struct ActorObj *obj);
void actor_wait_ready_show_sprite(struct ActorObj *obj);
void actor_fly_then_land_ext(struct ActorObj *obj);
void actor_move_countdown_next(struct ActorObj *obj);
void actor_drift_left_release(struct ActorObj *obj);
void obj_set_action2_then_state(struct ActorObj *obj);
void jump_obj_wait_ready_action2(struct ActorObj *obj);
void obj_pose_active_move_to_owner(struct ActorObj *obj);
void obj_pose_active_move_to_base(struct ActorObj *obj);
void obj_timer_effect_then_state(struct ActorObj *obj);
void obj_timer_arc_setup(struct ActorObj *obj);
void obj_arc_descend(struct ActorObj *obj);
void obj_fall_to_idle(struct ActorObj *obj);
void obj_index_zero_timer(struct ActorObj *obj);
void obj_index_zero_timer30(struct ActorObj *obj);
void obj_timer_action6_idle(struct ActorObj *obj);
void obj_wait_busy_then_state(struct ActorObj *obj);
void obj_counter_motion_to_owner(struct ActorObj *obj);
void obj_timer_move_then_sprite(struct ActorObj *obj);
void obj_timer_move_part_target(struct ActorObj *obj);
void obj_timer_move_sprite(struct ActorObj *obj);
void obj_timer_place_sprite_then_wait(struct ActorObj *obj);
void obj_index_zero_random_wait(struct ActorObj *obj);
void obj_timer_place_sprite_index(struct ActorObj *obj);
void obj_timer_place_sprite_index_zero(struct ActorObj *obj);
void obj_spawn_part_child(struct ActorObj *obj);
void obj_index_zero_slot_timer(struct ActorObj *obj);
u32 obj_link_other_then_idle(struct ActorObj *obj, struct ActorObj *partner);
u32 obj_notify_lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 obj_link_owner_move_part(struct ActorObj *obj);
void obj_lookup_part_b_then_state(void);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_list_push_default(struct ActorObj *obj);
s32 resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void actor_release_when_free(struct ActorObj *obj);
void actor_timed_effect_then_child(struct ActorObj *obj);
void obj_land_pose_link(struct ActorObj *obj);
void obj_jump_arc_fly(struct ActorObj *obj);
void jump_obj_pose_active_idle(struct ActorObj *obj);
void obj_index_zero_clear_state(struct ActorObj *obj);
u32 obj_link_owner_then_descend(struct ActorObj *obj);
u32 obj_lookup_part_then_wait(struct ActorObj *obj, u32 a, u32 b);
void obj_list_remove(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void jump_obj_pose_active_clear_state(struct ActorObj *obj);
u32 obj_pair_link_state(struct ActorObj *obj);
u32 actor_lookup_or_remove(struct ActorObj *obj, u32 slot);
void obj_slide_x_finish(struct ActorObj *obj);
void actor_action8_if_no_child(struct ActorObj *obj);
u32 obj_link_owner(struct ActorObj *obj);
u32 obj_query_part_finish(struct ActorObj *obj, u32 a, u32 b);
void actor_begin_offset_motion(struct ActorObj *obj);
void actor_idle_reset_alt(struct ActorObj *obj);

void obj_spawn_part_child(struct ActorObj *obj)
{
    struct ActorObj *child;
    s16 counted;

    counted = ++obj->timer;
    if (counted == 0x1E)
        set_slot_value(0x125, -1);
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        effect_spawn_at(0xD7A, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, obj);
        child = obj_spawn(obj->part, 0x408A, obj->baseX / 256 - 0x11, obj->baseY / 256 + 2,
                          obj->baseH / 256, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        ((struct Flags78Bits *)&child->flags78)->kind = 4;
        child->param9C = obj->param9C;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = 0;
        child->handler68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->handler58 = (void (*)(struct ActorObj *))obj_pair_link_state;
        child->handler60 = (void (*)(struct ActorObj *))actor_lookup_or_remove;
        child->tag = obj->tag;
        child->state = actor_drift_left_release;
        set_slot_value(0x58, -1);
        obj->state = jump_obj_pose_active_clear_state;
    }
}

void jump_obj_pose_active_clear_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void unknown_code_0806b3ba(struct ActorObj *obj)
{
    struct ActorObj *child;

    if ((s16)--obj->timer <= 0)
    {
        effect_spawn_at(0xDB2, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, obj);
        obj->timer = 10;
    }
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        if (obj->paramA0 != 0)
        {
            child = obj_spawn(obj->part, 0x408A, 1, obj->baseY / 256 + 2, obj->baseH / 256,
                              ((u32)obj->pose->unk11 << 25) >> 31, 1);
            obj->child = child;
            ((struct Flags78Bits *)&child->flags78)->kind = 4;
            obj_child_flags(obj, child);
            child->handler68 = on_arrive;
            child->unk6C = obj->unk6C;
            child->owner = obj->owner;
            child->targetX = obj->baseX;
            child->tag = obj->tag;
            child->state = obj_slide_x_finish;
            obj->state = actor_action8_if_no_child;
        }
        else
        {
            obj->state = 0;
        }
    }
}

NONMATCH("asm/code_0806B254/obj_spawn_child_by_slot.s", void obj_spawn_child_by_slot(struct ActorObj *obj))
{
    struct ActorObj *child;
    s32 t;
    s32 dxl;
    s32 dyl;
    u32 bits;
    u32 parent6;

    if (obj->pose->flags & 8)
    {
        t = obj->timer;
        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            if (obj->paramA8 == 0)
            {
                sprite_set_action(obj, obj->paramA4 + 3, 0, 0);
                effect_spawn_at(0xD92, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            }
            else
            {
                sprite_set_action(obj, obj->paramA4 + 6, 0, 0);
                effect_spawn_at(0xD9A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            }
            obj->state = actor_idle_reset_alt;
            if (obj->paramA8 == 0)
            {
                child = obj_spawn(obj->part, 0x408B, obj->baseX / 0x100 - 0x40, obj->baseY / 0x100 + 2,
                                  obj->baseH / 0x100 + 0x30, ((u32)obj->pose->unk11 << 25) >> 31, 1);
            }
            else
            {
                child = obj_spawn(obj->part, 0x408C, obj->baseX / 0x100 - 0x3C, obj->baseY / 0x100 + 2,
                                  obj->baseH / 0x100 + 0x1D, ((u32)obj->pose->unk11 << 25) >> 31, 1);
            }
            obj->child = child;
            ((struct Flags78Bits *)&child->flags78)->kind = 2;
            parent6 = obj->flags77;
            parent6 &= 0x40;
            bits = child->flags77;
            bits = (bits & ~0x40) | parent6;
            bits &= ~0x20;
            child->flags77 = bits;
            child->handler68 = on_arrive;
            child->unk6C = obj->unk6C;
            child->owner = obj->owner;
            child->tag = obj->tag;
            obj_list_push_default(child);
            child->handler58 = (void (*)(struct ActorObj *))obj_link_owner;
            child->handler5C = (void (*)(struct ActorObj *))obj_query_part_finish;
            child->targetX = obj->owner->part->targetX;
            child->targetY = obj->owner->part->targetY;
            child->targetH = obj->baseH;
            *(u8 *)&child->flags79 |= 0x20;
            child->motionSpeed = 0x300;
            child->motionStart = 0;
            start_motion(child);
            child->state = actor_begin_offset_motion;
            dyl = obj->owner->part->targetY - child->baseY;
            dxl = obj->owner->part->targetX - child->baseX;
            child->dh = child->baseH / 0x100;
            child->dy = (obj->owner->part->targetY + dyl) / 0x100;
            child->dx = (obj->owner->part->targetX + dxl) / 0x100;
            set_slot_value(0x85, -1);
        }
    }
}
END_NONMATCH

void obj_pose_active_move_high(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x4000;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->baseH;
        *(u8 *)&obj->flags79 |= 0x20;
        obj->motionSpeed = 0x100;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 0xA;
        set_slot_value(0x6C, -1);
        obj->state = obj_timer_place_sprite_then_wait;
    }
}

void obj_timer_place_sprite_then_wait(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        effect_spawn_at(0xDB2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0xA;
    }
    if (obj->index == 0)
    {
        sound_stop_song(0x6C);
        obj->timer = 0x14;
        obj->state = obj_timer_move_sprite;
    }
}

void obj_timer_move_sprite(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0x1000;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->baseH;
        *(u8 *)&obj->flags79 |= 0x20;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 0xA;
        obj->state = obj_timer_place_sprite_index;
        obj_list_push_default(obj);
        obj->handler58 = (void (*)(struct ActorObj *))obj_link_owner_move_part;
        obj->handler60 = (void (*)(struct ActorObj *))obj_lookup_part_b_then_state;
        set_slot_value(0x83, -1);
    }
}

void obj_timer_place_sprite_index(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        effect_spawn_at(0xDB2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0xA;
    }
    if (obj->index == 0)
    {
        obj->timer = 0x14;
        obj->state = obj_timer_move_part_target;
    }
}

void obj_timer_move_part_target(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->targetH = obj->baseH;
        *(u8 *)&obj->flags79 |= 0x20;
        obj->motionSpeed = 0x400;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 0xA;
        obj->state = obj_timer_place_sprite_index_zero;
    }
}

ASM_FUNC("asm/code_0806B254/obj_lookup_part_b_then_state.s", void obj_lookup_part_b_then_state(void))

u32 obj_link_owner_move_part(struct ActorObj *obj)
{
    partner_link_update(obj, obj->owner, 0xFF);
    sprite_set_action(obj, 5, 0, 0);
    obj->targetX = obj->part->targetX;
    obj->targetY = obj->part->targetY;
    obj->targetH = obj->baseH;
    *(u8 *)&obj->flags79 |= 0x20;
    obj->motionSpeed = 0x400;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 0xA;
    obj->state = obj_timer_place_sprite_index_zero;
    return 0;
}

void obj_timer_place_sprite_index_zero(struct ActorObj *obj)
{
    s32 t = obj->timer;
    s32 index;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        effect_spawn_at(0xDBB, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0xA;
    }
    index = obj->index;
    if (index == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = (void (*)(struct ActorObj *))index;
    }
}

u32 obj_query_part_finish(struct ActorObj *obj, u32 a, u32 b)
{
    u32 result = query_contact(obj, a, b);

    if (result != 0)
    {
        return result;
    }
    obj_release(obj);
    return 0;
}

u32 obj_link_owner(struct ActorObj *obj)
{
    partner_link_update(obj, obj->owner, 0xFF);
    return 0;
}

void actor_begin_offset_motion(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->targetX = obj->dx << 8;
        obj->targetY = obj->dy << 8;
        obj->targetH = obj->dh << 8;
        *(u8 *)&obj->flags79 |= 0x20;
        obj->motionSpeed = 0x300;
        obj->motionStart = index;
        start_motion(obj);
        obj->state = actor_release_when_free;
        set_slot_value(0xD5, -1);
    }
}

void actor_idle_reset_alt(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

u32 actor_lookup_or_remove(struct ActorObj *obj, u32 slot)
{
    u32 result = lookup_slot(obj, slot);

    if (result != 0)
    {
        return result;
    }
    effect_spawn_at(0xD82, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
    sound_stop_song(0x58);
    return 0;
}

u32 obj_pair_link_state(struct ActorObj *obj)
{
    partner_link_update(obj, obj->owner, 0xFF);
    obj->state = actor_drift_left_release;
    return 0;
}

void obj_slide_x_finish(struct ActorObj *obj)
{
    obj->posX -= obj->param9C;
    if (obj->posX <= obj->targetX)
    {
        obj_release(obj);
    }
}

void actor_drift_left_release(struct ActorObj *obj)
{
    obj->posX -= obj->param9C;
    if (obj->posX / 0x100 <= -0x40)
    {
        sound_stop_song(0x58);
        obj_release(obj);
    }
}

void actor_action8_if_no_child(struct ActorObj *obj)
{
    if (obj->child == 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->state = actor_stop_when_pose_active;
    }
}

void actor_timed_effect_then_child(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        effect_spawn_at(0xDB2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0xA;
    }
    if (obj->index == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = obj_spawn_part_child;
    }
}
