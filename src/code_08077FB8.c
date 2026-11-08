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

struct SpriteNode
{
    struct SpriteNode *next;
    void *unk4;
    void *unk8;
    u8 lo5 : 5;
    u8 hi3 : 3;
};

struct Low5Byte
{
    u8 lo5 : 5;
    u8 hi3 : 3;
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
    u8 filler0[4];
    struct SpriteNode *sprite4;
    struct ActorPose *pose;
    struct SpriteNode *nodes;
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
    u8 filler70[4];
    u8 unk74;
    u8 filler75;
    u8 unk76;
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
extern s32 gUnk_03000EB0[];
extern s32 gUnk_03000EB8[];
extern s32 gUnk_03000EC0[];
extern u16 gUnk_03000EC8[];
extern u16 gUnk_03000ECC[];

void remove_if_no_child(struct ActorObj *obj);
void spawn_child_4025_adopt(struct ActorObj *obj);
void timer2_anim_d(struct ActorObj *obj);
struct SpriteNode *sprite_node_take(void);
struct SpriteNode *sprite_node_alloc(s32 x, s32 y, s32 h);

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

static inline u32 pose_bit6(struct ActorObj *obj)
{
    return ((u32)obj->pose->unk11 << 25) >> 31;
}

static inline void link_sprite(struct ActorObj *child, struct SpriteNode *sprite)
{
    child->sprite4 = sprite;
    sprite->unk4 = child->pose;
    sprite->unk8 = &child->nodes;
}

static inline struct SpriteNode *alloc_node_at(struct ActorObj *obj)
{
    return sprite_node_alloc(obj->posX, obj->posY, obj->posH);
}

static inline struct ActorObj *spawn_pair_child(struct ActorObj *obj, u32 type, s32 dx, s32 dy, s32 dh)
{
    s32 x = obj->posX / 0x100 + dx;
    s32 y = obj->posY / 0x100 + dy;
    s32 h = obj->posH / 0x100 + dh;
    u32 bit = pose_bit6(obj);
    return obj_spawn(obj->part, type, x, y, h, bit, 1);
}

static inline void set_own_tables(struct ActorPart *a, struct ActorPart *b)
{
    gUnk_03000EB0[0] = a->targetX;
    gUnk_03000EB8[0] = a->targetY;
    gUnk_03000EC0[0] = a->targetH;
    gUnk_03000EB0[1] = b->targetX;
    gUnk_03000EB8[1] = b->targetY;
    gUnk_03000EC0[1] = b->targetH;
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

NONMATCH("asm/code_08077FB8/countdown_spawn_pair_link.s", void countdown_spawn_pair_link(struct ActorObj *obj))
{
    struct ActorObj *childA;
    struct ActorObj *childB;
    struct ActorObj *childC;
    struct ActorObj *childD;
    struct ActorObj *actorA;
    struct ActorObj *actorB;
    struct ActorPart *partA;
    struct ActorPart *partB;
    struct SpriteNode *sprite;
    struct SpriteNode *node;
    struct SpriteNode *prev;
    struct Low5Byte *counter;
    s32 x0;
    s32 y0;
    s32 edgeDx;
    s32 edgeA;
    s32 edgeB;
    s32 mode;
    s32 count;
    u32 old;

    obj->phase--;
    if (obj->phase <= 0)
    {
        set_slot_value(0x54, -1);
        obj->phase = 0xA;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        set_slot_value(0x10B, -1);
        place_effect_at_base(obj, 0x29CA);

        childA = spawn_pair_child(obj, 0x4026, -0x1A, 4, 0x57);
        childB = spawn_pair_child(obj, 0x4026, -0xE, 5, 0x5A);
        obj->child = childA;
        childA->child = childB;
        childA->flags |= 1;
        childA->unk74 = 0;
        childB->flags |= 1;
        childB->unk74 = 0;
        childA->state = remove_if_no_child;
        childB->state = remove_if_no_child;

        actorA = &gWorld->heroA->body;
        actorB = &gWorld->heroB->body;
        x0 = obj->posX - 0x1A00;
        y0 = obj->posY + 0x500;
        partA = actorA->part;
        edgeDx = -0x1000 - x0;
        edgeA = (edgeDx * (partA->targetY - y0)) / (partA->targetX - x0) + y0;
        partB = actorB->part;
        edgeB = (edgeDx * (partB->targetY - y0)) / (partB->targetX - x0) + y0;

        mode = obj->param9C;
        switch (mode)
        {
        case 0:
            gUnk_03000EB0[0] = -0x1000;
            gUnk_03000EB8[0] = edgeA;
            gUnk_03000EC0[0] = 0x800;
            gUnk_03000EB0[1] = partB->targetX;
            gUnk_03000EB8[1] = partB->targetY;
            gUnk_03000EC0[1] = partB->targetH;
            break;
        case 1:
            gUnk_03000EB0[0] = partA->targetX;
            gUnk_03000EB8[0] = partA->targetY;
            gUnk_03000EC0[0] = partA->targetH;
            gUnk_03000EB0[1] = -0x1000;
            gUnk_03000EB8[1] = edgeB;
            gUnk_03000EC0[1] = 0x1000;
            break;
        case 2:
            gUnk_03000EB0[0] = -0x1000;
            gUnk_03000EB8[0] = edgeA;
            gUnk_03000EC0[0] = 0x800;
            gUnk_03000EB0[1] = -0x1000;
            gUnk_03000EB8[1] = edgeB;
            gUnk_03000EC0[1] = 0x1000;
            break;
        default:
            set_own_tables(partA, partB);
            break;
        }

        childC = spawn_pair_child(obj, 0x4025, -0x1A, 5, 0x57);
        childB->child = childC;
        childC->phase = 0;
        childC->flags |= 1;
        childC->targetX = childC->baseX;
        childC->targetY = childC->baseY;
        childC->targetH = childC->baseH;
        childC->unk74 = 0;
        childC->unk76 |= 6;
        childC->owner = obj->owner;
        sprite = sprite_node_take();
        link_sprite(childC, sprite);
        counter = (struct Low5Byte *)((u8 *)gWorld + 7);
        old = counter->lo5;
        counter->lo5 = old - 1;
        sprite->lo5 = old;

        childC->nodes = alloc_node_at(childC);
        prev = childC->nodes;
        node = alloc_node_at(childC);
        prev->next = node;
        prev = node;
        node = alloc_node_at(childC);
        prev->next = node;
        prev = node;
        count = 7;
        do
        {
            node = alloc_node_at(childC);
            prev->next = node;
            prev = node;
            node = alloc_node_at(childC);
            prev->next = node;
            prev = node;
            node = alloc_node_at(childC);
            prev->next = node;
            prev = node;
            node = alloc_node_at(childC);
            prev->next = node;
            prev = node;
            count -= 4;
        } while (count >= 0);
        prev->next = NULL;
        childC->param9C = 0;
        childC->paramA0 = obj->param9C;
        childC->state = spawn_child_4025_adopt;

        childD = spawn_pair_child(obj, 0x4025, -0xE, 6, 0x5A);
        childC->child = childD;
        childD->phase = 0;
        childD->flags |= 1;
        childD->targetX = childD->baseX;
        childD->targetY = childD->baseY;
        childD->targetH = childD->baseH;
        childD->unk74 = 0;
        childD->unk76 |= 6;
        childD->owner = obj->owner;
        sprite = sprite_node_take();
        link_sprite(childD, sprite);
        counter = (struct Low5Byte *)((u8 *)gWorld + 7);
        old = counter->lo5;
        counter->lo5 = old - 1;
        sprite->lo5 = old;

        childD->nodes = alloc_node_at(childD);
        prev = childD->nodes;
        node = alloc_node_at(childD);
        prev->next = node;
        prev = node;
        node = alloc_node_at(childD);
        prev->next = node;
        prev = node;
        count = 7;
        do
        {
            node = alloc_node_at(childD);
            prev->next = node;
            prev = node;
            node = alloc_node_at(childD);
            prev->next = node;
            prev = node;
            node = alloc_node_at(childD);
            prev->next = node;
            prev = node;
            node = alloc_node_at(childD);
            prev->next = node;
            prev = node;
            count -= 4;
        } while (count >= 0);
        prev->next = NULL;
        childD->param9C = 1;
        childD->paramA0 = obj->param9C;
        childD->state = spawn_child_4025_adopt;

        gUnk_03000EC8[1] = 0;
        gUnk_03000EC8[0] = 0;
        gUnk_03000ECC[1] = 0;
        gUnk_03000ECC[0] = 0;
        obj->state = timer2_anim_d;
    }
}
END_NONMATCH
