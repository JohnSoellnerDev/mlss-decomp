#include "global.h"

struct PartSprite
{
    u8 filler0[0xC];
    u16 anim;
    u8 filler0E[0x11 - 0x0E];
    u8 flags11;
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 frame20;
    u8 filler21[0x2C - 0x21];
    s16 unk2C;
};

struct Part
{
    u32 unk4;
    u8 filler8[0x8 - 0x4];
    struct PartSprite *sprite;
    struct SpriteNode *spriteNodes;
    s32 posA;
    s32 posB;
    s32 height;
    u8 filler1C[0x28 - 0x1C];
    struct Part *owner;
    struct Part *link;
    struct Part *next;
    u8 filler34[0x38 - 0x34];
    s32 baseA;
    s32 baseB;
    s32 baseH;
    s32 unk44;
    s32 unk48;
    void (*state)(struct Part *);
    u8 filler50[0x54 - 0x50];
    void (*ownerState)(struct Part *);
    u8 filler58[0x5C - 0x58];
    void *handler5C;
    void *handler60;
    void *handler64;
    void (*callback68)(struct Part *);
    u32 link6C;
    u8 filler70[0x74 - 0x70];
    u8 kind74;
    u8 filler75[0x76 - 0x75];
    u8 flags76;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 step7A;
    u16 size7C;
    struct Part *cand80;
    s32 targetA;
    s32 targetB;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 stepH;
    u8 filler98[0x9C - 0x98];
    s32 flag9C;
    s32 limA0;
    s32 limA4;
    s32 countA8;
    u16 timer;
    s16 velA;
    s16 velB;
    u16 velH;
    u8 filler0B4[0xD8 - 0xB4];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    u8 filler0E4[0xEC - 0xE4];
    s16 unkEC;
    u8 filler0EE[0x113 - 0xEE];
    u8 flag113;
};

struct SpriteNode
{
    struct SpriteNode *next;
};

struct OwnerSprite
{
    u8 filler0[0x11];
    u8 flags11;
};

struct StatusChild
{
    u8 filler0[0x18];
    u8 *classData;
};

struct StatusObj
{
    u8 filler0[0x18];
    u8 *classData;
    u8 filler1C[0x284 - 0x1C];
    void *ptr284;
    void *ptr288;
    u8 filler28C[0x290 - 0x28C];
    void *ptr290;
    u8 filler294[0x2B8 - 0x294];
    void *ptr2B8;
    void *ptr2BC;
    void *ptr2C0;
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

struct Flags76Bits
{
    u8 low7 : 7;
    u8 bit7 : 1;
};

// The spawned child takes over two flag bits of its parent.
static inline void part_child_flags(struct Part *obj, struct Part *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

extern u8 gClassStatusChild[];
extern u8 gClassStatusObj[];
extern void *gUnk_03001008;

void run_pending_obj_callbacks(void);
void heap_free_a(void *p);
void heap_free_b(void *p);
void menu_obj_free(void *obj, u32 kind);
void destroy_map_obj_loader(void *obj, u32 kind);
void task_remove(void *node, u32 kind);

u8 step_motion(struct Part *obj);
void set_motion_from_value(struct Part *obj, u32 value);
void sprite_set_action(void *obj, u32 action, u32 arg, u32 extra);
s32 is_state_busy(void);
void task_insert(void *node, u8 kind);
void wait_for_index(struct Part *obj);
void start_motion(struct Part *obj);
void sound_stop_song(u32 id);
u32 query_contact(struct Part *obj);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void set_slot_value(u32 id, s32 value);
void set_motion_start(struct Part *obj, u32 speed);
void place_targets(struct Part *obj, s32 x, s32 y, s32 z, s32 w);
u8 candidates_advance_progress(struct Part *obj);
void obj_list_push_default(struct Part *obj);
void obj_list_remove(struct Part *obj);
void obj_release(struct Part *obj);
struct Part *obj_spawn(struct Part *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct Part *owner);
struct SpriteNode *sprite_node_alloc(s32 x, s32 y, s32 h);
void part_setup_render(struct Part *obj);
void on_arrive(struct Part *obj);

u32 set_state_motion_begin(struct Part *obj);
void motion_begin(struct Part *obj);
void motion_step_1(struct Part *obj);
void motion_step_2(struct Part *obj);
void motion_step_3(struct Part *obj);
void motion_end(struct Part *obj);
void part_launch_a(struct Part *obj);
void part_launch_b(struct Part *obj);
void part_flight_a(struct Part *obj);
void part_flight_b(struct Part *obj);
void part_fall(struct Part *obj);
void part_sink(struct Part *obj);
void part_settle(struct Part *obj);
void part_rest_idle(struct Part *obj);
void part_stop(struct Part *obj);
void part_rest_idle_b(struct Part *obj);
u32 set_state_part_wait_ready_8(struct Part *obj);
u32 set_state_part_wait_ready_4(struct Part *obj);
void part_wait_ready_8(struct Part *obj);
void part_wait_ready_4(struct Part *obj);
void part_hold_start(struct Part *obj);
void part_hold_from_9c(struct Part *obj);
void part_countdown_a(struct Part *obj);
void part_countdown_b(struct Part *obj);
void part_anim_wait(struct Part *obj);
void part_launch_c(struct Part *obj);
void part_track(struct Part *obj);
u32 part_check_contact(struct Part *obj);
void part_land_check(struct Part *obj);
void part_owner_watch(struct Part *obj);
void part_owner_idle(struct Part *obj);
void part_owner_idle_b(struct Part *obj);
u32 part_check_link(struct Part *obj);
void part_offscreen(struct Part *obj);
void part_unlinked_idle(struct Part *obj);
void part_wait_ready_2(struct Part *obj);
void part_setup_sprite(struct Part *obj);
void part_motion_wait(struct Part *obj);
void part_fly_drop(struct Part *obj);
void part_refresh_tiles(struct Part *obj);
void part_spawn_timed(struct Part *obj);
void part_update_nodes(struct Part *obj);

void destroy_status_obj(struct StatusObj *obj, u32 kind)
{
    obj->classData = gClassStatusObj;
    run_pending_obj_callbacks();
    heap_free_b(obj->ptr284);
    heap_free_b(obj->ptr290);
    if (obj->ptr288 != 0)
    {
        heap_free_a(obj->ptr288);
    }
    heap_free_b(gUnk_03001008);
    if (obj->ptr2B8 != 0)
    {
        menu_obj_free(obj->ptr2B8, 3);
    }
    if (obj->ptr2BC != 0)
    {
        destroy_map_obj_loader(obj->ptr2BC, 3);
    }
    if (obj->ptr2C0 != 0)
    {
        task_remove(obj->ptr2C0, 3);
    }
    task_remove(obj, kind);
}

struct StatusChild *init_status_child(struct StatusChild *obj, u8 slot)
{
    task_insert(obj, slot);
    obj->classData = gClassStatusChild;
    return obj;
}

u32 set_state_motion_begin(struct Part *obj)
{
    obj->state = motion_begin;
    return 1;
}

void motion_begin(struct Part *obj)
{
    place_targets(obj, obj->link->baseA / 0x100 + 0x28, obj->link->baseB / 0x100, 0, 0x100);
    set_motion_start(obj, 0x280);
    obj->state = motion_step_1;
}

void motion_step_1(struct Part *obj)
{
    if (step_motion(obj) == 0)
    {
        set_motion_from_value(obj, 0xA);
        obj->state = motion_step_2;
    }
}

void motion_step_2(struct Part *obj)
{
    if (candidates_advance_progress(obj) == 0)
    {
        place_targets(obj, obj->link->baseA / 0x100, obj->link->baseB / 0x100, 0, 0x100);
        set_motion_start(obj, 0x300);
        obj_list_push_default(obj);
        obj->state = motion_step_3;
    }
}

void motion_step_3(struct Part *obj)
{
    if (step_motion(obj) == 0)
    {
        place_targets(obj, obj->owner->unkD8 / 0x100, obj->owner->unkDC / 0x100, obj->owner->unkE0 / 0x100, 0x100);
        set_motion_start(obj, 0x380);
        obj_list_remove(obj);
        obj->state = motion_end;
    }
}

void motion_end(struct Part *obj)
{
    u8 moving = step_motion(obj);

    if (moving == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
    }
}

// When the current animation ends: takes action 7, plays sound 0x85 with effect 0x2177 and launches
// a shot (object 0x4103) that flies in a straight line from in front of obj to its target's spot,
// 6 px above the ground (part_flight_a); obj then rests (part_rest_idle_b).
void part_launch_a(struct Part *obj)
{
    struct Part *shot;
    struct Part *target;

    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        set_slot_value(0x85, -1);
        effect_spawn_at(0x2177, obj->baseA / 256, obj->baseB / 256 + 2, obj->baseH / 256, obj);
        shot = obj_spawn(obj->owner, 0x4103, obj->baseA / 256 - 0x11, obj->baseB / 256 + 5,
                         obj->baseH / 256 + 0x17, ((u32)obj->sprite->flags11 << 25) >> 31, 1);
        obj->next = shot;
        ((struct Flags78Bits *)&shot->flags78)->kind = 1;
        part_child_flags(obj, shot);
        shot->callback68 = on_arrive;
        shot->link6C = obj->link6C;
        target = obj->link;
        shot->link = target;
        ((struct Flags76Bits *)&target->flags76)->bit7 = 0;
        obj_list_push_default(shot);
        shot->handler5C = 0;
        shot->handler60 = 0;
        shot->handler64 = 0;
        shot->kind74 = 3;

        // During the flight velA/velB/velH hold the start point in pixels and
        // targetA/targetB/targetH the distance (1/256 px) from there to the target.
        shot->velA = shot->baseA / 256;
        shot->velB = shot->baseB / 256;
        shot->velH = shot->baseH / 256;
        shot->targetA = obj->link->owner->unkD8 - ((shot->velA - 4) << 8);
        shot->targetB = obj->link->owner->unkDC - (shot->velB << 8);
        shot->targetH = 0x600 - ((s16)shot->velH << 8);
        shot->state = part_flight_a;
        obj->state = part_rest_idle_b;
    }
}

// When the current animation ends: takes action 0xB, plays sound 0x62 with effect 0x217F and launches
// a shot (object 0x4104) that flies in a straight line from in front of obj towards its target's spot,
// 6 px above the ground (part_flight_b); obj then stops (part_stop).
void part_launch_b(struct Part *obj)
{
    struct Part *shot;

    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        set_slot_value(0x62, -1);
        effect_spawn_at(0x217F, obj->baseA / 256, obj->baseB / 256 + 2, obj->baseH / 256, obj);
        shot = obj_spawn(obj->owner, 0x4104, obj->baseA / 256 - 0x18, obj->baseB / 256 + 5,
                         obj->baseH / 256 + 0x1D, ((u32)obj->sprite->flags11 << 25) >> 31, 1);
        obj->next = shot;
        ((struct Flags78Bits *)&shot->flags78)->kind = 1;
        part_child_flags(obj, shot);
        shot->callback68 = on_arrive;
        shot->link6C = obj->link6C;
        shot->link = obj->link;
        obj_list_push_default(shot);
        shot->handler5C = 0;
        shot->handler60 = 0;
        shot->handler64 = 0;
        shot->kind74 = 3;

        // Start point in pixels, and the distance (1/256 px) from there to the target.
        shot->velA = shot->baseA / 256;
        shot->velB = shot->baseB / 256;
        shot->velH = shot->baseH / 256;
        shot->targetA = obj->link->owner->unkD8 - (shot->velA << 8);
        shot->targetB = obj->link->owner->unkDC - (shot->velB << 8);
        shot->targetH = 0x600 - ((s16)shot->velH << 8);
        shot->state = part_flight_b;
        obj->state = part_stop;
    }
}

// Flight of the shot part_launch_a fires: spins its sprite, moves 4 px a frame along A (towards
// lower A) and sets B and the height by linear interpolation from the start point (velA/velB/velH,
// in pixels) over the distance to the target (targetA/targetB/targetH). Once it has passed the
// target it plays sound 0xAD, bounces back up with a jump arc (gravity 0x400) and part_settle
// takes over.
void part_flight_a(struct Part *shot)
{
    shot->sprite->anim += 0x1200;
    shot->posA -= 0x400;
    shot->posB = ((shot->posA - (shot->velA << 8)) * shot->targetB) / shot->targetA + (shot->velB << 8);
    shot->height = ((shot->posA - (shot->velA << 8)) * shot->targetH) / shot->targetA + ((s16)shot->velH << 8);
    if (shot->posA < (shot->velA << 8) + shot->targetA)
    {
        set_slot_value(0xAD, -1);
        shot->stepH = jump_velocity(&shot->velA, &shot->velB, &shot->velH, -0xC800,
                                    -(shot->targetB * 0xC800) / shot->targetA, 0, 0x3000, 0x400, 0x100, 0);
        shot->state = part_settle;
    }
}

// Flight of the shot part_launch_b fires: spins its sprite, moves 2.5 px a frame along A (towards
// lower A) and sets B and the height by linear interpolation from the start point (velA/velB/velH,
// in pixels) over the distance to the target (targetA/targetB/targetH). Once it is down to 6 px
// above the ground it bounces off with a jump arc (gravity 0x280) and part_fall takes over.
void part_flight_b(struct Part *shot)
{
    shot->sprite->anim += 0x1000;
    shot->posA -= 0x280;
    shot->posB = ((shot->posA - (shot->velA << 8)) * shot->targetB) / shot->targetA + (shot->velB << 8);
    shot->height = ((shot->posA - (shot->velA << 8)) * shot->targetH) / shot->targetA + ((s16)shot->velH << 8);
    if (shot->height <= 0x5FF)
    {
        shot->stepH = jump_velocity(&shot->velA, &shot->velB, &shot->velH, -0xA000,
                                    -(shot->targetB * 0xA000) / shot->targetA, 0, 0x3800, 0x280, 0x100, 0);
        shot->state = part_fall;
    }
}

// Flight of part_launch_b's shot after it bounces off (part_flight_b): spins its sprite and follows
// its jump arc. Once it is off screen (A below -16 px) it plays sound 0x62 and comes back in at
// A = 256 px, at its owner's B and height (the owner is a bro whose body is at +8, so its position
// is at +0x40/+0x44/+0x48), on a jump arc (gravity 0x280) that ends 23 px in front of the owner.
// It then leaves the object list and part_sink takes over.
void part_fall(struct Part *shot)
{
    shot->sprite->anim += 0x1000;
    shot->velH += shot->stepH;
    shot->posA += shot->velA;
    shot->posB += shot->velB;
    shot->height += (s16)shot->velH;
    if (shot->posA < -0x1000)
    {
        set_slot_value(0x62, -1);
        shot->posA = 0x10000;
        shot->posB = shot->owner->unk44;
        shot->height = shot->owner->unk48;
        shot->stepH = jump_velocity(&shot->velA, &shot->velB, &shot->velH, shot->owner->baseH - 0xE900,
                                    0x500, 0x2300, 0x4000, 0x280, 0x100, 0);
        obj_list_remove(shot);
        shot->state = part_sink;
    }
}
