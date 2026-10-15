#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 phase;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 cursorX;
    s16 cursorY;
    u8 filler3[0x104 - 0xFA];
    s16 unk104;
    u8 filler4[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
};

struct PoseBits
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 top : 1;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gUnk_03000FD8;
extern u32 gUnk_03000E3C;
extern u8 gUnk_03000E7C;
extern u8 gUnk_03000E7D;
extern const u8 gStepSequenceTable[];

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
    u8 filler50[0x54 - 0x50];
    void (*handler54)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
    u8 dir;
    u8 flags;
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

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_arc_start(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_anim5_then_spawn(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_place_at_base_when_index_zero(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_timer_follow_owner(struct ActorObj *obj);
void obj_start_move_right(struct ActorObj *obj);
void obj_copy_bytes_sprite_88(struct ActorObj *obj);
void obj_init_pair_motion(struct ActorObj *obj);
void obj_anim6_wait4(struct ActorObj *obj);
void obj_move_to_part_x_offset(struct ActorObj *obj);
void obj_spawn_child_4115(struct ActorObj *obj);
void obj_spawn_child_4113(struct ActorObj *obj);
void obj_place_sprite_anim9(struct ActorObj *obj);
void obj_link_owner_go(struct ActorObj *obj);
void obj_lookup_slot_start(struct ActorObj *obj);
void obj_setup_wait_target(struct ActorObj *obj);
void obj_lookup_part_setup(struct ActorObj *obj);
void obj_fall_to_ground(struct ActorObj *obj);
void obj_wave_left(struct ActorObj *obj);
void obj_timer_to_move(struct ActorObj *obj);
void obj_wave_left_to_target(struct ActorObj *obj);
void obj_wave_rise_stop(struct ActorObj *obj);
void obj_wave_fall_stop(struct ActorObj *obj);
void obj_move_by_path(void);
void obj_count_down_exit(struct ActorObj *obj);
void obj_timer_table_next(struct ActorObj *obj);
void obj_link_then_finish(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_index_zero_count(struct ActorObj *obj);
void obj_notify_and_show(struct ActorObj *obj);
void obj_lookup_part_move(struct ActorObj *obj);
void obj_setup_part_motion(struct ActorObj *obj);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 mt19937_next(void);
void obj_list_remove(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_anim_a_wait60(struct ActorObj *obj);
void obj_countdown_then_anim_b(struct ActorObj *obj);
void obj_wait_then_spawn_child(struct ActorObj *obj);
void side_walk_obj_timer_spawn_effect(struct ActorObj *obj);
void obj_pose_release_partner(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);

void obj_link_then_finish(struct ActorObj *obj)
{
    struct ActorObj *partner = (struct ActorObj *)(gUnk_03000FD8->partners[obj->targetX + 5] + 8);

    if (*(s32 *)((u8 *)partner + 0x18) == 0)
    {
        if (obj->dx == 0)
        {
            partner_link_update(obj, partner, 0xFF);
            obj->dx = 1;
        }
    }
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        effect_spawn_at(0x2071, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
    }
}


void obj_start_wave_left(struct ActorObj *obj)
{
    obj->dx = obj->posX / 0x100;
    obj->dy = obj->posY / 0x100;
    obj->paramA4 = obj->owner->part->targetX - (obj->dx << 8);
    obj->paramA8 = obj->owner->part->targetY - (obj->dy << 8);
    sprite_set_action(obj, 4, 0, 0);
    obj->posH = 0x1000;
    obj_list_push_default(obj);
    obj->handler58 = obj_link_owner_go;
    obj->handler60 = obj_lookup_slot_start;
    set_slot_value(0x6C, -1);
    obj->state = obj_wave_left;
}


void obj_wave_left(struct ActorObj *obj)
{
    obj->posH = 0xE00 - (gSineTable[((obj->targetH / 0x100) - 0x40) & 0xFF] / 0x40) * 2;
    if ((u32)((obj->targetH / 0x100) & 0x7F) <= 0x3F)
    {
        obj->posX += -0x199;
        obj->targetH += 0x400;
    }
    else
    {
        obj->posX += -0x133;
        obj->targetH += 0x300;
    }
    obj->posY = obj->paramA8 * (obj->posX - (obj->dx << 8)) / obj->paramA4 + (obj->dy << 8);
    obj->pose->phase = obj->targetH;
    if (obj->posX / 0x100 <= -0x20)
    {
        obj_list_remove(obj);
        obj->posX = 0x10000;
        obj->posY = obj->part->targetY;
        obj->state = obj_wave_left_to_target;
        obj->targetH = 0xF000;
    }
}


void obj_wave_left_to_target(struct ActorObj *obj)
{
    obj->posH = 0xE00 - (gSineTable[((obj->targetH / 0x100) - 0x40) & 0xFF] / 0x40) * 2;
    if ((u32)((obj->targetH / 0x100) & 0x7F) <= 0x3F)
    {
        obj->posX += -0x199;
        obj->targetH += 0x400;
    }
    else
    {
        obj->posX += -0x133;
        obj->targetH += 0x300;
    }
    obj->pose->phase = obj->targetH;
    if (obj->posX <= obj->part->targetX)
    {
        obj->posX = obj->part->targetX;
        obj->state = obj_wave_rise_stop;
    }
}


void obj_wave_rise_stop(struct ActorObj *obj)
{
    s32 old = obj->targetH & 0xFF00;
    s32 sine;

    obj->targetH += 0x400;
    obj->pose->phase = obj->targetH;
    sine = gSineTable[((obj->targetH / 0x100) - 0x40) & 0xFF];
    obj->posH = 0xE00 - (sine / 0x40) * 2;
    if ((obj->targetH & 0xFF00) <= old)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->posH = 0;
        obj->pose->phase = 0;
        obj->state = 0;
        sound_stop_song(0x6C);
    }
}


void obj_wave_right(struct ActorObj *obj)
{
    obj->posH = 0xE00 - (gSineTable[((obj->targetH / 0x100) - 0x40) & 0xFF] / 0x40) * 2;
    if ((u32)((obj->targetH / 0x100) & 0x7F) <= 0x3F)
    {
        obj->posX += 0x600;
        obj->targetH += -0xC00;
    }
    else
    {
        obj->posX += 0x400;
        obj->targetH += -0x900;
    }
    obj->posY = obj->paramA8 * (obj->posX - (obj->dx << 8)) / obj->paramA4 + (obj->dy << 8);
    obj->pose->phase = obj->targetH;
    if (obj->posX >= obj->part->targetX)
    {
        obj->posX = obj->part->targetX;
        obj->posY = obj->part->targetY;
        obj->state = obj_wave_fall_stop;
    }
}


void obj_wave_fall_stop(struct ActorObj *obj)
{
    s32 old = obj->targetH & 0xFF00;
    s32 sine;

    obj->targetH += -0xC00;
    obj->pose->phase = obj->targetH;
    sine = gSineTable[((obj->targetH / 0x100) - 0x40) & 0xFF];
    obj->posH = 0xE00 - (sine / 0x40) * 2;
    if ((obj->targetH & 0xFF00) >= old)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->posH = 0;
        obj->pose->phase = 0;
        obj->state = 0;
    }
}


void obj_land_random_target(struct ActorObj *obj)
{
    struct ActorObj *candA;
    struct ActorObj *candB;

    obj->posH += obj->param9C;
    obj->param9C -= obj->paramA0;
    if (obj->posH <= 0)
    {
        obj->posH = 0;
        sprite_set_action(obj, 3, 0x40E7, 0);
        candA = (struct ActorObj *)(gUnk_03000FD8->partners[6] + 8);
        candB = (struct ActorObj *)(gUnk_03000FD8->partners[7] + 8);
        if (mt19937_next() & 1)
        {
            obj->targetY = candA->part->targetY;
        }
        else
        {
            obj->targetY = candB->part->targetY;
        }
        obj->targetX = obj->owner->part->targetX + 0x4000;
        obj->targetH = obj->baseH;
        obj->motionSpeed = 0x280;
        *(u8 *)&obj->flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        set_slot_value(0xA7, -1);
        obj->state = obj_setup_part_motion;
    }
}


void obj_timer_release(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            if (obj->owner->part->slot == -1)
            {
                sprite_set_action(obj, 3, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 0x21, 0, 0);
            }
            ((struct PoseBits *)&obj->pose->unk11)->bit6 = 0;
            obj->timer = 0x3C;
            obj->state = obj_timer_to_move;
        }
    }
}


void obj_timer_to_move(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        *(u8 *)&obj->pose->unk11 |= 0x40;
        set_slot_value(0x11C, -1);
        sprite_set_action(obj, 0x17, 0, 0);
        obj->param9C = obj->baseX;
        obj->paramA0 = obj->baseY;
        obj->paramA4 = 0;
        obj->targetX = obj->baseX - obj->owner->posX;
        obj->targetY = obj->owner->posY - obj->baseY;
        obj_list_push_default(obj);
        obj->handler58 = obj_setup_wait_target;
        obj->handler60 = obj_lookup_part_setup;
        obj->state = obj_fall_to_ground;
    }
}


void obj_launch_to_target(struct ActorObj *obj)
{
    s32 distX;
    s32 distY;
    s32 flight;
    s32 half;
    s32 vy;

    sprite_set_action(obj, 0x19, 0, 0);
    obj->targetH = obj->param9C;
    obj->targetX = 0x7000;
    obj->targetY = 0x6800;
    obj->dx = obj->posX / 0x100;
    obj->dy = obj->posY / 0x100;
    obj->dh = obj->posH / 0x100;
    distX = obj->targetX - (obj->dx << 8);
    distY = obj->targetY - (obj->dy << 8);
    flight = (s32)MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(distX * distX + distY * distY) / 0x280;
    half = flight / 2;
    vy = 0x3000 / (half * half);
    if (vy == 1)
    {
        vy = 2;
    }
    obj->param9C = -vy;
    obj->paramA0 = flight;
    obj->paramA4 = distX;
    obj->paramA8 = distY;
    obj->timer = 0;
    obj->state = (void (*)(struct ActorObj *))obj_move_by_path;
}
