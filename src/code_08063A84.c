#include "global.h"
#include "ram_routines.h"

struct ActorDesc
{
    u8 filler0[0x0C];
    u8 order;
};

struct ActorPose
{
    u8 filler0[0x0C];
    u16 counter0C;
    u8 filler0E[0x11 - 0x0E];
    u8 flip;
    u8 flags;
};

struct ActorObj
{
    u8 filler0[0x04];
    struct ActorDesc *desc;
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
    u8 unk76;
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

/* The part holds a nested object at +0x08 with the same layout as struct ActorObj. */
struct ActorPart
{
    u8 filler0[0x08];
    struct ActorObj sub8;
    u8 filler0BC[0xD8 - 0xBC];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
};

static inline void set_rebuild_flag(struct ActorObj *obj)
{
    u8 *flags = &obj->flags79;

    *flags = *flags | 0x20;
}

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

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

struct ActionBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct PoseBits12
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

// The child takes over bits 5 and 6 of the flag byte at +0x7F of its part.
static inline void copy_part_flags(struct ActorObj *child, struct ActorPart *part)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)&part->sub8)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)&part->sub8)->bit5;
}

// The spawned child takes over the kind and two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child, u32 kind)
{
    ((struct Flags78Bits *)&child->flags78)->kind = kind >> 5;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

void obj_wait_ready_action3(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj);
u32 is_state_busy(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_wait_ready_action2_next(struct ActorObj *obj);
void obj_action3_place_sprite(struct ActorObj *obj);
void obj_begin_chain(struct ActorObj *obj);
void obj_motion_fast_start(struct ActorObj *obj);
void obj_free_place_remove(struct ActorObj *obj);
void obj_free_clear_remove(struct ActorObj *obj);
void chain_obj_spawn_child_launch(struct ActorObj *obj);
void obj_begin_move_fixed_x(struct ActorObj *obj);
void obj_notify_action5_or_7(struct ActorObj *obj);
void obj_timer_action6_or_8(struct ActorObj *obj);
void chain_obj_index_wait_action4(struct ActorObj *obj);
void obj_pose_action_b(struct ActorObj *obj);
void chain_obj_wait_ready_action2(struct ActorObj *obj);
void obj_begin_linked_wait(struct ActorObj *obj);
void obj_timer_action_a_idle(struct ActorObj *obj);
void obj_timer_action5(struct ActorObj *obj);
void obj_timer_action6(struct ActorObj *obj);
void obj_timer_action6_spawn(struct ActorObj *obj);
void obj_spawn_child_11c(struct ActorObj *obj);
void obj_wait_ready_place(struct ActorObj *obj);
void obj_timer_place_a(struct ActorObj *obj);
void obj_action_e_idle(struct ActorObj *obj);
void obj_place_and_notify(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void obj_index_wait_action5(struct ActorObj *obj);
void obj_motion_slow_start(struct ActorObj *obj);
void obj_timer_effect_b(struct ActorObj *obj);
void obj_action7_wait(struct ActorObj *obj);
void obj_arc_motion_step(struct ActorObj *obj);
void obj_counter_release(struct ActorObj *obj);
void obj_index_action5(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_alloc_anim(struct ActorObj *obj);
u32 obj_query_part3(struct ActorObj *obj);
u32 obj_unlink_end(struct ActorObj *obj);
void obj_idle_action8(struct ActorObj *obj);
s32 chain_metric(struct ActorObj *obj);
void obj_list_push_arg(struct ActorObj *obj, u32 arg);
void obj_chain_cull(struct ActorObj *obj);
void chain_link_end(struct ActorObj *obj);
u32 obj_lookup_action1(struct ActorObj *obj, struct ActorObj *partner);
void obj_action_a_timer(struct ActorObj *obj);
void obj_sync_target_place(struct ActorObj *obj);
extern u8 gUnk_0300034C[];
u8 poll_spline_path(u32 buf, s32 *x, s32 *h);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
extern s32 gUnk_03000E40;
extern s32 gUnk_03000E44;
void obj_stop(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
void heap_free_b(void *p);
extern u32 gUnk_03000E18;
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void set_slot_value(u32 id, s32 value);
void start_motion(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, struct ActorObj *partner);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, u32 a, u32 b, u32 code);
u32 mt19937_next(void);

void obj_poll_anim(struct ActorObj *obj)
{
    s32 v[3];
    s32 x;
    s32 y;

    if (poll_spline_path(gUnk_03000E18, &gUnk_03000E40, &gUnk_03000E44) == 1)
    {
        x = gUnk_03000E40;
        obj->posX = x;
        y = gUnk_03000E44;
        obj->posY = y;
        v[0] = x / 0x100;
        v[1] = y / 0x100;
        v[2] = obj->posH / 0x100;
        offset_three_words_camera(&v[0], &v[1], &v[2], 0);
    }
    else
    {
        heap_free_b((void *)gUnk_03000E18);
        obj_release(obj);
    }
}

u32 obj_query_part3(struct ActorObj *obj)
{
    u32 result = query_contact(obj);

    if (result != 0)
    {
        return result;
    }
    obj->state = obj_free_place_remove;
    sound_stop_song(0x11C);
    return 0;
}

u32 obj_unlink_end(struct ActorObj *obj)
{
    partner_link_update(obj, obj->owner, 0xFF);
    obj->state = obj_free_clear_remove;
    sound_stop_song(0x11C);
    return 0;
}

void obj_idle_action8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        set_slot_value(0xD8, -1);
        sprite_set_action(obj, 8, 0, 0);
        obj->state = wait_for_index;
    }
}

u32 obj_random_action(struct ActorObj *obj)
{
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->paramA8 = mt19937_next() % 3;
    obj->state = obj_wait_ready_place;
    return 1;
}

void obj_free_place_remove(struct ActorObj *obj)
{
    effect_spawn_at(0x12CD, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    heap_free_b((void *)gUnk_03000E18);
    obj_release(obj);
}

void obj_free_clear_remove(struct ActorObj *obj)
{
    effect_spawn_at(0x12CD, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj->state = 0;
    heap_free_b((void *)gUnk_03000E18);
    obj_release(obj);
}

void obj_wait_ready_place(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        effect_spawn_at(0x12A3, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_motion_slow_start;
    }
}

void obj_motion_slow_start(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->targetX = 0xC0 << 7;
        obj->targetY = 0xD0 << 7;
        obj->targetH = obj->baseH;
        set_rebuild_flag(obj);
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_index_wait_action5;
    }
}

void obj_index_wait_action5(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 4, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        obj->timer = 1;
        obj->state = obj_timer_action6_spawn;
    }
}

void obj_timer_action6_spawn(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 6, 0, 0);
            set_slot_value(0xAE, -1);
            obj->state = obj_spawn_child_11c;
        }
    }
}

void obj_motion_from_owner(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        struct ActorObj *owner = obj->owner;

        obj->targetX = owner->part->targetX + 0x2000;
        obj->targetY = owner->part->targetY + 0x100;
        obj->targetH = obj->baseH;
        set_rebuild_flag(obj);
        obj->motionSpeed = 0x100;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        if (obj->param9C == 0)
        {
            set_slot_value(0x58, -1);
        }
        else
        {
            set_slot_value(0x58, -1);
        }
        obj->state = obj_index_action5;
    }
}

void chain_obj_spawn_child_launch(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 deltaX;
    s32 deltaY;
    s32 length;
    s32 duration;
    s32 speed;

    if (obj->pose->flags & 8)
    {
        if (obj->param9C == 0)
        {
            effect_spawn_at(0x1296, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
            child = obj_spawn(obj->part, 0x40F8, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100,
                              ((struct PoseBits6 *)&obj->pose->flip)->bit6, 1);
            obj->child = child;
            obj_child_flags(obj, child, 0x20);
            child->handler68 = on_arrive;
            child->unk6C = obj->unk6C;
            child->owner = obj->owner;
            obj_list_push_default(child);
            child->handler60 = (void (*)(struct ActorObj *))obj_lookup_action1;
            child->tag = obj->tag;
            child->posH = obj->posH + 0x800;
            child->baseH = child->posH;
            child->targetX = child->baseX - 0x4000;
            child->targetY = child->baseY;
            child->targetH = child->baseH;
            child->dx = child->baseX / 0x100;
            child->dy = child->baseY / 0x100;
            child->dh = child->baseH / 0x100;
            deltaX = child->targetX - ((s16)child->dx << 8);
            deltaY = child->targetY - ((s16)child->dy << 8);
            length = ((s32 (*)(s32))gMathRoutines)(deltaX * deltaX + deltaY * deltaY);
            duration = length / 0x133;
            speed = 0x3000 / ((duration / 2) * (duration / 2));
            if (speed == 1)
                speed = 2;
            child->param9C = -speed;
            child->paramA0 = duration;
            child->paramA4 = deltaX;
            child->paramA8 = deltaY;
            child->timer = 0;
            child->state = obj_arc_motion_step;
            child->targetH = 0x4000 / duration;
            obj->targetX = obj->posX - 0x4000;
            obj->targetY = obj->posY;
            obj->posX = 0x10000;
            obj->baseX = 0x10000;
            obj->posY = 0x10000;
            obj->baseY = 0x10000;
            obj->state = obj_sync_target_place;
            set_slot_value(0x57, -1);
        }
        else
        {
            effect_spawn_at(0x129B, obj->baseX / 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100, obj);
            sprite_set_action(obj, 8, 0, 0);
            obj->state = obj_action_a_timer;
            set_slot_value(0x38, -1);
        }
    }
}

void obj_arc_motion_step(struct ActorObj *obj)
{
    s32 d;
    s32 a;
    s32 b;

    obj->pose->counter0C += obj->targetH;
    obj->posX = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    d = (s16)obj->timer - obj->paramA0 / 2;
    a = (d * d) * obj->param9C;
    b = ((s16)obj->dh << 8) + 0x3000;
    obj->posH = a + b;
    obj->timer++;
    if ((s16)obj->timer > obj->paramA0)
    {
        set_slot_value(0x45, -1);
        obj_list_remove(obj);
        obj_release(obj);
    }
}

u32 obj_lookup_action1(struct ActorObj *obj, struct ActorObj *partner)
{
    u32 result = lookup_slot(obj, partner);

    if (result != 0)
    {
        return result;
    }
    resolve_contact_action(partner, obj, obj->posX / 0x100, obj->posH / 0x100,
                           partner->part->slot != -1 ? 0x13 : 0x12);
    sprite_set_action(obj, 1, 0, 0);
    obj->targetX = 0x1E;
    obj->state = obj_counter_release;
    return 0;
}

void obj_action_a_timer(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->timer = 3;
        obj->state = obj_timer_effect_b;
    }
}

void obj_sync_target_place(struct ActorObj *obj)
{
    s32 x;
    s32 y;

    if (obj->child == 0)
    {
        x = obj->targetX;
        obj->posX = x;
        obj->baseX = x;
        y = obj->targetY;
        obj->posY = y;
        obj->baseY = y;
        effect_spawn_at(0x1296, x / 0x100, y / 0x100 + 2, obj->baseH / 0x100, obj);
        sprite_set_action(obj, 6, 0, 0);
        obj->state = obj_action7_wait;
    }
}

void obj_index_action5(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = chain_obj_spawn_child_launch;
    }
}

u32 unknown_part_steps_08064288(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = obj_wait_ready_action3;
    return 1;
}

u32 sub_0806429C(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = obj_wait_ready_action3;
    return 1;
}

void obj_counter_release(struct ActorObj *obj)
{
    s32 counter = obj->targetX - 1;

    obj->targetX = counter;
    if (counter <= 0)
    {
        struct ActorObj *part8 = &obj->part->sub8;

        part8->handler68 = on_arrive;
        part8->unk6C = obj->unk6C;
        obj->unk6C = 0;
        if (part8->part->unkF6 != 0)
        {
            sprite_set_action(obj, 0, 0, 0);
            obj->state = obj_arc_motion_step;
        }
        else
        {
            obj_stop(part8);
            obj_release(obj);
        }
    }
}
