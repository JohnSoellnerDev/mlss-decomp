#include "global.h"
#include "ram_routines.h"

struct SceneSprite
{
    u8 filler0[0x04];
    u16 unk4;
    u16 unk6;
    u8 filler8[0x11 - 0x08];
    u8 unk11;
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
};

struct SceneFlag79Bits
{
    u8 unk0 : 5;
    u8 bit5 : 1;
    u8 unk6 : 2;
};

struct SpriteUnk11Bits
{
    u8 unk0 : 6;
    u8 bit6 : 1;
    u8 unk7 : 1;
};

struct SceneData
{
    u8 filler0[0x40];
    u8 filler40[0xB0 - 0x40];
    u32 unkB0;
    s16 unkB4;
    u8 filler0B6[0xD8 - 0xB6];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x113 - 0xF8];
    u8 modeByte;
    u8 filler114[0x124 - 0x114];
    u16 unk124;
};

struct SceneObj
{
    u8 filler0[0x08];
    struct SceneSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 dispX;
    s32 dispY;
    s32 dispZ;
    u8 filler1C[0x28 - 0x1C];
    struct SceneData *data;
    struct SceneObj *owner;
    struct SceneObj *hold;
    u8 filler34[0x38 - 0x34];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct SceneObj *obj);
    u8 filler50[0x54 - 0x50];
    void (*link54)(struct SceneObj *obj);
    u8 filler58[0x5C - 0x58];
    void *callback5C;
    void *callback60;
    u8 filler64[0x75 - 0x64];
    u8 unk75;
    u8 unk76;
    u8 filler77[0x78 - 0x77];
    u8 unk78;
    u8 flags79;
    s16 stepX;
    s16 stepY;
    u8 unk7E;
    u8 unk7F;
    u8 filler80[0x84 - 0x80];
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    u8 filler90[0x92 - 0x90];
    s16 unk92;
    s32 unk94;
    s32 orbitAngle;
    s32 unk9C;
    s32 unkA0;
    u8 filler0A4[0xA8 - 0xA4];
    s32 phase;
    s16 timer;
    s16 unkAE;
    s16 unkB0;
    u8 filler0B2[0xB8 - 0xB2];
    u16 unkB8;
};

struct SceneWorld
{
    u8 filler0[0x40];
    u32 unk40;
    u8 filler44[0x58 - 0x44];
    u8 *unk58;
    u8 filler5C[0x80 - 0x5C];
    struct SceneObj *unk80;
    struct SceneObj *unk84;
};

struct SceneSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

extern struct SceneWorld *gUnk_03000FD8;

void sprite_set_action(struct SceneObj *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void place_targets(struct SceneObj *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct SceneObj *obj, u32 speed);
u8 step_motion(struct SceneObj *obj);
void sound_stop_song(u32 song);
void effect_spawn_at(u32 part, s32 x, s32 y, s32 z, struct SceneObj *obj);
void obj_release(struct SceneObj *obj);
u32 lookup_link_slot(struct SceneObj *obj);
void partner_link_update(void *a, struct SceneObj *b, u32 flags);
void obj_list_push_default(struct SceneObj *obj);
void obj_list_remove(struct SceneObj *obj);
void effect_from_table_obj(struct SceneObj *obj, s32 index);
void obj_link_flag_set(struct SceneObj *obj);
void obj_phase_wait_idle(struct SceneObj *obj);
void obj_anim_mode5_timer3(struct SceneObj *obj);
u32 lookup_slot(struct SceneObj *obj, struct SceneObj *other);
void set_step_and_offsets(struct SceneObj *obj, u32 step, u32 count);
extern u32 gNodeChainStepTable[];
extern u32 gNodeChainSceneScale[];
extern u16 gNodeChainSpriteScale[];
struct SceneObj *obj_spawn_linked(struct SceneObj *parent, u32 kind, u32 value);
u32 mt19937_next(void);
u32 sub_081DE850(u32 range, u32 seed);
extern u16 gPhaseLevelTable[];
extern const u16 gPhaseChanceTable[];
extern u16 gWaveSlotTable[];
s32 parabola_height(s32 t, s32 v, s32 s, s32 h);
void obj_place_at_slot_offset(struct SceneObj *obj);
void resolve_contact_action(struct SceneObj *a, struct SceneObj *b, s32 c, s32 d, u32 e);

void obj_wave_step_b(struct SceneObj *obj);
void obj_update_orbit_pos(struct SceneObj *obj);
void obj_countdown_then_place(struct SceneObj *obj);
void obj_anim6_spawn_child(struct SceneObj *obj);
void obj_random_chance_or_wait(struct SceneObj *obj);
void obj_spawn_and_place_part(struct SceneObj *obj);
void obj_place_by_owner_kind(struct SceneObj *obj);
u32 obj_lookup_slot_or_place(struct SceneObj *obj, struct SceneObj *other);
void obj_anim_c_link_pair(struct SceneObj *obj);
void obj_wait_blend_then_detach(struct SceneObj *obj);
void obj_wait_scene_b4_then_anim4(struct SceneObj *obj);
void obj_spawn_two_linked(struct SceneObj *obj, void *other, s32 x, s32 y, s32 a, s32 b, void *cb);
void obj_set_motion_from_keys(struct SceneObj *obj, u32 speed, u32 b);
u32 clamp_reflect_offsets(s32 *outA, s32 *outB, s32 lo, s32 hi, s32 v);
u32 obj_blend_position(struct SceneObj *obj);
void obj_start_anim4_if_active(struct SceneObj *obj);
void obj_wait_scene_mode(struct SceneObj *obj);
void obj_place_owner_target_a(struct SceneObj *obj);
void obj_place_owner_target_b(struct SceneObj *obj);
void obj_anim_d_if_timer(struct SceneObj *obj);
void obj_phase_countdown(struct SceneObj *obj);
void obj_place_scene_target(struct SceneObj *obj);
void obj_link_owner_part(struct SceneObj *obj);
void obj_anim_d_if_active(struct SceneObj *obj);
void obj_anim1_for_slot(struct SceneObj *obj);
void world_set_link_callback(void (*cb)(struct SceneObj *obj));
u32 obj_link_or_cancel(struct SceneObj *obj);
u32 callback_return_0(struct SceneObj *obj);
void obj_start_scene_move(struct SceneObj *obj);
u32 sprite_state_pair_c(struct SceneObj *obj);
u32 sub_081131A0(struct SceneObj *obj);
u32 sub_081131B4(struct SceneObj *obj);
void obj_timer_anim5(struct SceneObj *obj);
void obj_anim_stop_timer_up(struct SceneObj *obj);
void obj_place_fixed_a(struct SceneObj *obj);
void obj_release_when_idle_a(struct SceneObj *obj);
void obj_anim_e_if_active(struct SceneObj *obj);
void obj_place_stored_xy(struct SceneObj *obj);
void callback_noop(struct SceneObj *obj);
void obj_place_scene_anim_c(struct SceneObj *obj);
void obj_idle_anim_b_part(struct SceneObj *obj);
void obj_anim9_slot_value(struct SceneObj *obj);
void obj_anim6_owner_flag(struct SceneObj *obj);
void obj_place_scene_hold(struct SceneObj *obj);
void obj_idle_anim_b(struct SceneObj *obj);
void obj_idle_anim_3(struct SceneObj *obj);
void obj_detach_from_scene(struct SceneObj *obj);
void obj_release_when_idle_c(struct SceneObj *obj);
void obj_wait_world_then_flags(struct SceneObj *obj);
void obj_move_toward_owner(struct SceneObj *obj);
void obj_release_when_idle_b(struct SceneObj *obj);
void obj_place_part_timer(struct SceneObj *obj);
void obj_set_scene_state(struct SceneObj *obj);
void obj_track_scene_part(struct SceneObj *obj);
void obj_timer_anim_6(struct SceneObj *obj);
void obj_place_part_mode_anim7(struct SceneObj *obj);
void obj_scene_move_or_stop(struct SceneObj *obj);

u32 obj_blend_position(struct SceneObj *obj)
{
    s32 ctrl;
    s32 next;

    ctrl = *(s16 *)&obj->unk7E;
    next = ctrl + (obj->stepX * obj->stepY) / 0x100;
    obj->timer = *(u16 *)&obj->unk7E;
    *(s16 *)&obj->unk7E = next;
    if (next <= 0x3FFF)
    {
        obj->dispX = ((obj->unk84 >> 16) * next + (s16)obj->unk84 * (0x4000 - next)) / 0x40;
        obj->dispZ = ((obj->unk8C >> 16) * next + (s16)obj->unk8C * (0x4000 - next)) / 0x40;
        if (obj->orbitAngle == 0)
        {
            obj->dispY = ((obj->unk88 >> 16) * next + (s16)obj->unk88 * (0x4000 - next)) / 0x40;
        }
        else
        {
            obj->dispY = parabola_height(next, obj->unk94, obj->orbitAngle, (s16)obj->unk92 << 8);
        }
        return 1;
    }
    obj->dispX = (s16)(obj->unk84 >> 16) << 8;
    obj->dispY = (s16)(obj->unk88 >> 16) << 8;
    obj->dispZ = (s16)(obj->unk8C >> 16) << 8;
    ((struct SceneFlag79Bits *)&obj->flags79)->bit5 = 1;
    return 0;
}


void obj_start_anim4_if_active(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 0x20;
        obj->state = obj_timer_anim5;
    }
}


void obj_wait_scene_mode(struct SceneObj *obj)
{
    if (obj->data->modeByte == 0)
    {
        obj->timer = 0x10;
        obj->state = obj_wait_scene_b4_then_anim4;
    }
}


void obj_place_owner_target_a(struct SceneObj *obj)
{
    place_targets(obj, obj->owner->data->targetX / 0x100, obj->owner->data->targetY / 0x100, obj->posZ / 0x100, 0x100);
    obj_set_motion_from_keys(obj, 0x380, 0xA4);
    obj->state = obj_wait_blend_then_detach;
}


void obj_place_owner_target_b(struct SceneObj *obj)
{
    place_targets(obj, obj->owner->data->targetX / 0x100, obj->owner->data->targetY / 0x100, obj->posZ / 0x100, 0x100);
    obj_set_motion_from_keys(obj, 0x280, 0x3C);
    obj->state = obj_wait_blend_then_detach;
}


void obj_anim_d_if_timer(struct SceneObj *obj)
{
    if (obj->timer != 0)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_anim_stop_timer_up;
    }
}


void obj_phase_countdown(struct SceneObj *obj)
{
    if (obj->phase == 0)
    {
        effect_spawn_at(0x264C, obj->posX / 0x100, obj->posY / 0x100 - 1, obj->posZ / 0x100 - 1, obj);
        obj->phase = 4;
    }
    obj->phase--;
    step_motion(obj);
    if (obj->flags79 & 0x20)
    {
        obj_release(obj);
        obj->state = 0;
    }
}


void obj_place_scene_target(struct SceneObj *obj)
{
    if (gUnk_03000FD8->unk40 == 0)
    {
        place_targets(obj, obj->data->targetX / 0x100, obj->data->targetY / 0x100, obj->data->targetZ / 0x100, 0x100);
        set_motion_start(obj, 0x400);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_release_when_idle_a;
        set_slot_value(0x11B, -1);
    }
}


void obj_link_owner_part(struct SceneObj *obj)
{
    if (obj->hold == 0)
    {
        resolve_contact_action(obj->owner, obj, obj->dispX / 0x100, obj->dispZ / 0x100, obj->owner->data->slot != -1 ? 0x13 : 0x12);
        obj->unkB8 = 0x1B45;
        obj->state = obj_place_scene_target;
        set_slot_value(0xAD, -1);
    }
}


void obj_anim_d_if_active(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_anim_e_if_active;
    }
}


void obj_anim1_for_slot(struct SceneObj *obj)
{
    if (-obj->data->slot != 1)
    {
        sprite_set_action(obj, 1, 0x2057, 0);
    }
    else
    {
        sprite_set_action(obj, 1, 0x2028, 0);
    }
    obj->state = callback_noop;
}


void world_set_link_callback(void (*cb)(struct SceneObj *obj))
{
    struct SceneObj *linked = gUnk_03000FD8->unk80;
    if ((linked->unk7E & 6) == 2)
    {
        linked->link54 = cb;
    }
    linked = gUnk_03000FD8->unk84;
    if (linked != 0 && (linked->unk7E & 6) == 2)
    {
        linked->link54 = cb;
    }
}


u32 obj_link_or_cancel(struct SceneObj *obj)
{
    u32 slot = lookup_link_slot(obj);
    if (slot != 0)
    {
        return slot;
    }
    obj->state = obj_place_scene_anim_c;
    return 0;
}


u32 callback_return_0(struct SceneObj *obj)
{
    return 0;
}

void obj_start_scene_move(struct SceneObj *obj)
{
    place_targets(obj, obj->data->targetX / 0x100, obj->data->targetY / 0x100, obj->data->targetZ / 0x100, 0x100);
    set_motion_start(obj, 0x300);
    obj->phase = 0;
    obj->state = obj_idle_anim_b_part;
}


u32 sprite_state_pair_c(struct SceneObj *obj)
{
    obj->state = obj_place_fixed_a;
    obj->data->modeByte = 0;
    return 1;
}


u32 sub_081131A0(struct SceneObj *obj)
{
    obj->state = obj_place_stored_xy;
    obj->stepY = 0x100;
    return 1;
}


u32 sub_081131B4(struct SceneObj *obj)
{
    obj->state = obj_anim9_slot_value;
    return 1;
}


void obj_timer_anim5(struct SceneObj *obj)
{
    obj->timer--;
    if (obj->timer == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_anim6_owner_flag;
        set_slot_value(0xA3, -1);
    }
}


void obj_anim_stop_timer_up(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        obj->timer++;
        sprite_set_action(obj, -1, -1, 0);
        obj->state = obj_place_scene_hold;
    }
}


void obj_place_fixed_a(struct SceneObj *obj)
{
    place_targets(obj, 0x8A, 0x6C, 0, 0x100);
    set_motion_start(obj, 0x380);
    sprite_set_action(obj, 0xA, 0, 0);
    obj->state = obj_idle_anim_b;
    set_slot_value(0x11B, -1);
}


void obj_release_when_idle_a(struct SceneObj *obj)
{
    u8 done = step_motion(obj);
    if (done == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
        sound_stop_song(0x11B);
    }
}


void obj_anim_e_if_active(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        obj->state = obj_random_chance_or_wait;
    }
}


void obj_place_stored_xy(struct SceneObj *obj)
{
    place_targets(obj, obj->unk9C, obj->unkA0, 0, 0x100);
    set_motion_start(obj, 0x300);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_idle_anim_3;
    set_slot_value(0x11B, -1);
}


void callback_noop(struct SceneObj *obj)
{
}

void obj_place_scene_anim_c(struct SceneObj *obj)
{
    place_targets(obj, obj->data->targetX / 0x100, obj->data->targetY / 0x100, obj->data->targetZ / 0x100, 0x100);
    set_motion_start(obj, 0x300);
    sprite_set_action(obj, 0xC, 0, 0);
    obj->state = obj_release_when_idle_c;
}


void obj_idle_anim_b_part(struct SceneObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        if (gUnk_03000FD8->unk40 == 0)
        {
            if (obj->phase == 0)
            {
                world_set_link_callback(obj_detach_from_scene);
                obj->phase = 1;
            }
        }
        effect_spawn_at(0x2689, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->state = obj_wait_world_then_flags;
    }
}


void obj_anim9_slot_value(struct SceneObj *obj)
{
    sprite_set_action(obj, 9, 0, 0);
    obj->state = obj_place_at_slot_offset;
    set_slot_value(0x86, -1);
}


void obj_anim6_owner_flag(struct SceneObj *obj)
{
    struct SceneObj *owner;
    u8 *flags76;
    s32 bit;

    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        owner = obj->owner;
        bit = 0;
        if (owner->data->unkF6 == 0)
        {
            bit = 1;
        }
        flags76 = &owner->unk76;
        bit = bit << 7;
        *flags76 = (*flags76 & 0x7F) | bit;
        obj->state = obj_move_toward_owner;
    }
}


void obj_place_scene_hold(struct SceneObj *obj)
{
    if (obj->hold == 0)
    {
        place_targets(obj, obj->data->targetX / 0x100, obj->data->targetY / 0x100, obj->data->targetZ / 0x100, 0x100);
        set_motion_start(obj, 0x400);
        sprite_set_action(obj, 0xA, 0, 0);
        obj->state = obj_release_when_idle_b;
        set_slot_value(0x11B, -1);
    }
}


void obj_idle_anim_b(struct SceneObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_anim_c_link_pair;
        sound_stop_song(0x11B);
    }
}


void obj_idle_anim_3(struct SceneObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_place_part_timer;
        sound_stop_song(0x11B);
    }
}


void obj_detach_from_scene(struct SceneObj *obj)
{
    sprite_set_action(obj, -1, -1, 0);
    obj->state = 0;
    partner_link_update(gUnk_03000FD8->unk58 + 8, obj, 0xFF);
}


void obj_release_when_idle_c(struct SceneObj *obj)
{
    u8 done = step_motion(obj);
    if (done == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
    }
}


void obj_wait_world_then_flags(struct SceneObj *obj)
{
    if (gUnk_03000FD8->unk40 == 0)
    {
        if (((u8)(obj->phase ^ 1) & 1) != 0)
        {
            world_set_link_callback(obj_detach_from_scene);
            obj->phase |= 1;
        }
    }
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->phase |= 2;
    }
    if (obj->phase > 2)
    {
        obj->state = 0;
    }
}


void obj_move_toward_owner(struct SceneObj *obj)
{
    struct SceneObj *owner = obj->owner;

    if ((owner->unk76 & 0x80) == 0)
    {
        if (owner->posZ <= obj->posZ)
        {
            partner_link_update(obj, owner, 0xFF);
        }
    }
    if (obj->sprite->flags12 & 8)
    {
        effect_spawn_at(0x2764, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj_release(obj);
        set_slot_value(0x88, -1);
        set_slot_value(0xA3, -1);
    }
}


void obj_release_when_idle_b(struct SceneObj *obj)
{
    u8 done = step_motion(obj);
    if (done == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
        sound_stop_song(0x11B);
    }
}


void obj_place_part_timer(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        effect_spawn_at(0x2700, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->timer = 0;
        obj->phase = 4;
        obj->state = obj_countdown_then_place;
        set_slot_value(0x116, -1);
    }
}


void obj_set_scene_state(struct SceneObj *obj)
{
    struct SceneData *target;

    target = obj->owner->data;
    if (obj->data->modeByte != 0)
    {
        obj->unkAE = target->targetX / 0x100 + 0x2E;
        place_targets(obj, target->targetX / 0x100 + 0x2E, target->targetY / 0x100 - 2, target->targetZ / 0x100, 0x100);
    }
    else
    {
        obj->unkAE = target->targetX / 0x100 + 0x40;
        place_targets(obj, target->targetX / 0x100 + 0x40, target->targetY / 0x100, target->targetZ / 0x100, 0x100);
    }
    set_motion_start(obj, 0x100);
    sprite_set_action(obj, 0x19, 0, 0);
    obj->phase = 0;
    obj->state = obj_track_scene_part;
}


void obj_track_scene_part(struct SceneObj *obj)
{
    s32 part;

    if ((obj->unk84 & 0xFFFFFF00) != (obj->posX & 0xFFFFFF00))
    {
        if (obj->phase == 0)
        {
            if (obj->posX / 0x100 < obj->unkAE)
            {
                part = 0x297A;
            }
            else
            {
                part = 0x2971;
            }
            effect_spawn_at(part, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
            obj->phase = 0xA;
        }
        obj->phase--;
    }
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, obj->data->modeByte + 4, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_anim_mode5_timer3;
    }
}


void obj_timer_anim_6(struct SceneObj *obj)
{
    s32 mode;
    s32 index;

    if (obj->sprite->flags12 & 8)
    {
        obj->timer--;
        if (obj->timer == 0)
        {
            sprite_set_action(obj, obj->data->modeByte + 6, 0, 0);
            ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
            obj->sprite->unk20 = 6;
            mode = obj->data->modeByte;
            index = 0x18A6;
            if (mode != 0)
            {
                index += 0x14;
            }
            effect_from_table_obj(obj, index);
            obj->callback60 = obj_link_flag_set;
            obj->state = obj_place_part_mode_anim7;
            sound_stop_song(0x8A);
            set_slot_value(0x119, -1);
        }
    }
}


void obj_place_part_mode_anim7(struct SceneObj *obj)
{
    s32 mode;
    s32 part;

    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, obj->data->modeByte + 7, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->sprite->unk20 = 0x10;
        mode = obj->data->modeByte;
        part = 0x27F5;
        if (mode != 0)
        {
            part += 0xE;
        }
        effect_spawn_at(part, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->state = obj_scene_move_or_stop;
        set_slot_value(0x55, -1);
    }
}


void obj_scene_move_or_stop(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        if (obj->unk9C != 0)
        {
            place_targets(obj, obj->data->targetX / 0x100, obj->data->targetY / 0x100, obj->data->targetZ / 0x100, 0x100);
            set_motion_start(obj, 0x200);
            sprite_set_action(obj, 0x19, 0, 0);
            obj->phase = 0;
            obj->state = obj_phase_wait_idle;
        }
        else
        {
            sprite_set_action(obj, -1, -1, 0);
            obj->state = 0;
        }
    }
}
