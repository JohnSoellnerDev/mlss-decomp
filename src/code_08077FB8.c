#include "global.h"

struct ByteBit4
{
    u8 lo : 4;
    u8 bit4 : 1;
    u8 hi : 3;
};

struct ByteBit7
{
    u8 lo : 7;
    u8 bit7 : 1;
};

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
    u8 filler0[0xAC];
    s32 unkAC;
    s32 unkB0;
    u8 fillerB4[0xD8 - 0xB4];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
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
    u32 (*handler58)(struct ActorObj *, u32);
    u8 filler5C[0x60 - 0x5C];
    u32 (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*callback68)(struct ActorObj *);
    s32 link6C;
    u8 filler70[0x77 - 0x70];
    u8 unk77;
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
    s16 wait;
    s16 phase;
    s16 linkB;
    s16 linkH;
};

struct FlagBits
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct FlagBits5
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct Bits77
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Hero
{
    u8 filler0[0x08];
    struct ActorObj body;
};

struct LinkBlock
{
    u8 filler0[0x7E];
    u8 flags;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gMathRoutines;
extern const s32 gAimTargetX[];
extern const s32 gAimTargetY[];
extern const s32 gSpawnTargetX[];
extern const s32 gSpawnTargetY[];
extern void (*gUnk_03000ED4)(struct ActorObj *);
extern void (*gUnk_03000ED8)(struct ActorObj *);
extern u32 gUnk_03000EDC[];
extern u32 gUnk_03000F1C[];
extern u8 gUnk_03000F5C;
extern u8 gUnk_03000F5D;

void set_slot_fields(u32 a, u32 b, u32 c);
void actor_link_anim_steps(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 arg2, u32 arg3);
s32 resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_release(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);
u32 mt19937_next(void);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_slot(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
u32 anim_sets_spawn_random_child(struct ActorObj *obj, u32 excludeArg);
void spawn_actor_children(struct ActorObj *obj);
void spawn_child_8031(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
u32 arc_tan2(s32 x, s32 y);

// Spawns effect `id` at the object's base position (each axis divided by 0x100).
static inline void place_effect_at_base(struct ActorObj *obj, u32 id)
{
    effect_spawn_at(id, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
}

void spawn_child_4046(struct ActorObj *obj);
void link_actors_when_ready(struct ActorObj *obj);
void spawn_child_4048_aim(struct ActorObj *obj);
void update_pose_index(struct ActorObj *obj);
u32 move_to_target_if_found(struct ActorObj *obj);
void step_move_and_finish(struct ActorObj *obj);
void step_link_or_countdown_a(struct ActorObj *obj);
void step_link_or_wait_b(struct ActorObj *obj);
void spawn_child_4049_move(struct ActorObj *obj);
void step_arc_move(struct ActorObj *obj);
void reset_anim_if_active_a(struct ActorObj *obj);
void step_countdown_then_select(struct ActorObj *obj);
void reset_anim_if_active_b(struct ActorObj *obj);
u32 link_place_and_remove(struct ActorObj *obj, u32 other);
u32 step_countdown_notify_or_place(struct ActorObj *obj, struct ActorObj *other);
void place_and_remove_when_index_zero(struct ActorObj *obj);
void reset_anim_if_active_c(struct ActorObj *obj);
void step_timer_anim_then_remove(struct ActorObj *obj);
void step_remove_child_then_place(struct ActorObj *obj);
void step_start_child_anim(struct ActorObj *obj);
void step_countdown_place_then_wait(struct ActorObj *obj);
void set_anim_04_place_part(struct ActorObj *obj);
s32 unknown_part_steps_08077c80(struct ActorObj *obj);
s32 sub_08077C8C(struct ActorObj *obj);
s32 sub_08077C98(struct ActorObj *obj);
s32 sub_08077CA4(struct ActorObj *obj);
s32 step_init_timers(struct ActorObj *obj);
void step_random_anim_3_or_5(struct ActorObj *obj);
void set_anim_0d_place_part(struct ActorObj *obj);
void set_anim_0a_next(struct ActorObj *obj);
void step_remove_children_then_idle(struct ActorObj *obj);
void step_rise_until_height(struct ActorObj *obj);
void step_place_children_then_idle(struct ActorObj *obj);
void set_anim_06_place_part(struct ActorObj *obj);
void set_anim_02_place_part_24ba(struct ActorObj *obj);
void set_anim_0e_save_timer(struct ActorObj *obj);
void countdown_set_anim_0b_place_part(struct ActorObj *obj);
void link_actor_pair_start(struct ActorObj *obj);
void countdown_phase_then_wait(struct ActorObj *obj);
void countdown_spawn_pair_link(struct ActorObj *obj);

void link_actor_pair_start(struct ActorObj *obj)
{
    struct Hero *linkA;
    struct Hero *linkB;
    struct ActorObj *actorA;
    struct ActorObj *actorB;
    struct ActorObj *child;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xC, 0, 0);
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        obj->wait = 0x20;
        linkA = gWorld->heroA;
        actorA = &linkA->body;
        linkB = gWorld->heroB;
        actorB = &linkB->body;
        ((struct ByteBit7 *)&((struct LinkBlock *)linkA)->flags)->bit7 = 0;
        ((struct ByteBit7 *)&((struct LinkBlock *)linkB)->flags)->bit7 = 0;
        gUnk_03000ED4 = actorA->state;
        gUnk_03000ED8 = actorB->state;
        ((struct ByteBit4 *)&actorA->pose->flags)->bit4 = 1;
        ((struct ByteBit4 *)&actorB->pose->flags)->bit4 = 1;
        actorA->state = actor_link_anim_steps;
        actorB->state = actor_link_anim_steps;
        gUnk_03000F5C = 0;
        gUnk_03000F5D = 0;

        child = actorA->child;
        if (child != NULL)
        {
            do
            {
                ((struct ByteBit4 *)&child->pose->flags)->bit4 = 1;
                gUnk_03000EDC[gUnk_03000F5C++] = (u32)child->state;
                child->state = actor_link_anim_steps;
                child = child->child;
            } while (child != NULL);
        }

        child = actorB->child;
        if (child != NULL)
        {
            do
            {
                ((struct ByteBit4 *)&child->pose->flags)->bit4 = 1;
                gUnk_03000F1C[gUnk_03000F5D++] = (u32)child->state;
                child->state = actor_link_anim_steps;
                child = child->child;
            } while (child != NULL);
        }

        set_slot_fields(1, 0, 0);
        set_slot_value(0x54, -1);
        *((u8 *)gWorld->trackedObj + 0x770) = 1;
        obj->phase = 0xA;
        obj->state = countdown_phase_then_wait;
    }
}

void countdown_phase_then_wait(struct ActorObj *obj)
{
    obj->phase--;
    if (obj->phase <= 0)
    {
        set_slot_value(0x54, -1);
        obj->phase = 0xA;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        sprite_set_action(obj, 0x11, 0, 0);
        obj->wait = 0x64;
        obj->state = countdown_spawn_pair_link;
    }
}

ASM_FUNC("asm/code_08077FB8/countdown_spawn_pair_link.s", void countdown_spawn_pair_link(struct ActorObj *obj))
