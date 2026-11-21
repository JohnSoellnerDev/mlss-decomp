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
    s32 unk94;          /* orbit: added to orbitAngle each frame */
    s32 orbitAngle;     /* orbit: bits 8-15 index the sine/cosine tables */
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

struct SceneSignedCount
{
    u8 filler0[0x75];
    s8 count;
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

extern struct SceneWorld *gWorld;

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
u32 rand_below(u32 range, u32 seed);
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
u32 obj_start_stored_xy(struct SceneObj *obj);
u32 obj_start_anim9(struct SceneObj *obj);
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

/* Orbit step: moves obj along its orbit (obj_update_orbit_pos). Each full lap (orbitAngle past
   +-0x10000) wraps the angle, counts the lap in timer (after two it moves on to
   obj_start_scene_move) and stops the song gWaveSlotTable gives for unkB0. On the first frame it
   sets the world's link callback to obj_anim1_for_slot. Every 6 frames it spawns effect 0x2677 on
   the first half of the circle (angle 0-0x7F) or 0x2680 on the second. */
void obj_wave_step_b(struct SceneObj *obj)
{
    u32 effect;
    s32 angle;

    obj_update_orbit_pos(obj);
    if ((u32)(obj->orbitAngle + 0xFFFF) > 0x1FFFE)
    {
        obj->orbitAngle -= obj->orbitAngle / 0x10000 * 0x10000;
        obj->timer++;
        if (obj->timer > 1)
        {
            obj->state = obj_start_scene_move;
        }
        if (gWaveSlotTable[obj->unkB0] != 0)
        {
            sound_stop_song(gWaveSlotTable[obj->unkB0]);
        }
    }
    if (obj->phase == 0)
    {
        world_set_link_callback(obj_anim1_for_slot);
        obj->phase = 1;
    }
    if (obj->stepY == 0)
    {
        angle = (obj->orbitAngle / 0x100) & 0xFF;
        effect = 0x2680;
        if (angle <= 0x7F)
        {
            effect -= 9;
        }
        effect_spawn_at(effect, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->stepY = 6; /* effect countdown */
    }
    obj->stepY--;
}


/* Places obj on its orbit for this frame and advances orbitAngle by unk94: an ellipse 0x30 px wide
   and 0x24 px high around (centerX, 0x6C), its center X swinging by unkAE px between 0x19 and 0x30
   as the angle goes round. */
void obj_update_orbit_pos(struct SceneObj *obj)
{
    s32 angle;
    s32 centerX;

    angle = (obj->orbitAngle / 0x100) & 0xFF;
    centerX = (angle - 0x40) & 0xFF;
    if (centerX <= 0x7F)
    {
        centerX = (obj->unkAE * (centerX & 0xFF)) / 0x80 + 0x19;
    }
    else
    {
        centerX = 0x30 - (obj->unkAE * ((centerX - 0x80) & 0xFF)) / 0x80;
    }
    obj->dispX = (gCosineTable[angle] / 0x40) * 0x30 + (centerX << 8);
    obj->dispY = (gSineTable[angle] / 0x40) * 0x24 + 0x6C00;
    obj->orbitAngle += obj->unk94;
}


void obj_countdown_then_place(struct SceneObj *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        obj->phase--;
        if (obj->phase != 0)
        {
            if (obj->timer == 0)
            {
                effect_spawn_at(0x2700, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
                obj->timer = 0x3E7;
            }
            obj->timer--;
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
            ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
            obj->state = obj_anim6_spawn_child;
        }
    }
}


struct StageBits
{
    u8 lo5 : 5;
    u8 hi3 : 3;
};

struct SpriteActiveBits
{
    u8 low3 : 3;
    u8 active : 1;
    u8 high4 : 4;
};

struct Flag79Low
{
    u8 bit0 : 1;
    u8 rest : 7;
};

/*
 * Once the sprite's current animation has ended: plays action 6 (mode 1), spawns effect 0x270E on
 * the object and a linked child 0x4040 4 px left, 1 px down and 20 px up from where it appears. The
 * child's level comes from gPhaseLevelTable[phase] (bits 5-7 of unk78 = level - 1; level 0 sets bit
 * 0 of flags79 instead); it then goes to its spot by the owner (obj_place_by_owner_kind). Plays
 * sound 0x85; the object waits in obj_anim_d_if_active.
 */
void obj_anim6_spawn_child(struct SceneObj *obj)
{
    struct SceneObj *child;
    s32 x;
    s32 y;
    s32 z;

    /* bit 3 of flags12: the animation has ended */
    if (((struct SpriteActiveBits *)&obj->sprite->flags12)->active != 0)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        effect_spawn_at(0x270E, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        child = obj_spawn_linked(obj, 0x4040, 0);
        /* positions are in 1/256 pixels */
        x = child->posX - 0x400;
        child->posX = x;
        child->dispX = x;
        y = child->posY + 0x100;
        child->posY = y;
        child->dispY = y;
        z = child->posZ + 0x1400;
        child->posZ = z;
        child->dispZ = z;
        if (gPhaseLevelTable[obj->phase] != 0)
        {
            ((struct StageBits *)&child->unk78)->hi3 = gPhaseLevelTable[obj->phase] - 1;
        }
        else
        {
            ((struct Flag79Low *)&child->flags79)->bit0 = 1;
        }
        obj_list_push_default(child);
        child->callback60 = (void *)obj_lookup_slot_or_place;
        child->state = obj_place_by_owner_kind;
        obj->state = obj_anim_d_if_active;
        set_slot_value(0x85, -1);
    }
}


/*
 * Waits for the part obj holds to land: once the part has reached its X target and its motion
 * progress (unk7E as an s16, 0 to 0x4000) is within four steps of the end, rolls 0-99 against
 * gPhaseChanceTable[phase]. Below it, obj throws again (action 0xF, mode 1, next phase,
 * obj_spawn_and_place_part); otherwise it misses (obj_link_owner_part). Holding nothing, obj goes
 * back to obj_place_scene_target.
 */
void obj_random_chance_or_wait(struct SceneObj *obj)
{
    struct SceneObj *part;

    part = obj->hold;
    if (part == NULL)
    {
        obj->state = obj_place_scene_target;
        return;
    }
    if (part->posX > part->dispX)
    {
        return;
    }
    /* still more than four steps of motion to go */
    if ((0x4000 - *(s16 *)&part->unk7E) > ((part->stepX * part->stepY / 0x100) << 2))
    {
        return;
    }
    if (rand_below(100, mt19937_next()) < gPhaseChanceTable[obj->phase])
    {
        sprite_set_action(obj, 0xF, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_spawn_and_place_part;
        obj->phase++;
    }
    else
    {
        obj->state = obj_link_owner_part;
    }
}


/*
 * Throws a part (chosen by obj_random_chance_or_wait): plays action 0x10 (mode 1) and takes the part
 * the object holds, or spawns a linked 0x4040 part 4 px right, 1 px down and 40 px up from where it
 * appears, its level from gPhaseLevelTable[phase] (bits 5-7 of unk78 = level - 1; level 0 sets bit 0
 * of flags79 instead). The part is listed and goes to its spot by the owner (obj_place_by_owner_kind);
 * spawns effect 0x2716 on the object, plays sound 0x6A, and the object waits in obj_anim_d_if_active.
 */
void obj_spawn_and_place_part(struct SceneObj *obj)
{
    struct SceneObj *part;
    s32 x;
    s32 y;
    s32 z;

    sprite_set_action(obj, 0x10, 0, 0);
    ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
    part = obj->hold;
    if (part == NULL)
    {
        part = obj_spawn_linked(obj, 0x4040, 0);
        /* positions are in 1/256 pixels */
        x = part->posX + 0x400;
        part->posX = x;
        part->dispX = x;
        y = part->posY + 0x100;
        part->posY = y;
        part->dispY = y;
        z = part->posZ + 0x2800;
        part->posZ = z;
        part->dispZ = z;
        if (gPhaseLevelTable[obj->phase] != 0)
        {
            ((struct StageBits *)&part->unk78)->hi3 = gPhaseLevelTable[obj->phase] - 1;
        }
        else
        {
            ((struct Flag79Low *)&part->flags79)->bit0 = 1;
        }
    }
    obj_list_push_default(part);
    part->callback60 = (void *)obj_lookup_slot_or_place;
    effect_spawn_at(0x2716, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    part->state = obj_place_by_owner_kind;
    obj->state = obj_anim_d_if_active;
    set_slot_value(0x6A, -1);
}



/* A thrown part's first step: it flies to its owner's data target, 8 px left of it and 16 px
   up, at the chain speed for its kind (data->unkB0) with an arc of 0x50. The kind's tables also
   give data->unk124 and both sprite scales (unk4, unk6). Sets sprite bit 6 of unk11, then the
   part counts down in obj_phase_countdown. */
void obj_place_by_owner_kind(struct SceneObj *part)
{
    struct SceneData *ownerData;
    struct SceneSprite *sprite;
    u32 kind;

    ownerData = part->owner->data;
    kind = part->data->unkB0;
    /* positions are in 1/256 pixels */
    place_targets(part, ownerData->targetX / 0x100 - 8, ownerData->targetY / 0x100, ownerData->targetZ / 0x100 + 0x10, -1);
    set_step_and_offsets(part, gNodeChainStepTable[kind], 0x50);
    part->data->unk124 = gNodeChainSceneScale[kind];
    sprite = part->sprite;
    sprite->unk6 = gNodeChainSpriteScale[kind];
    sprite->unk4 = gNodeChainSpriteScale[kind];
    ((struct SpriteUnk11Bits *)&part->sprite->unk11)->bit6 = 1;
    part->phase = 0;
    part->state = obj_phase_countdown;
}


/* Hit callback (callback60) of a thrown part: when `hitter` strikes it from the front (lookup_slot
   returns 0), the part flies back to its thrower's spot (4 px right, 1 px down and 40 px up from the
   object embedded at data + 8) at the chain speed for its kind, effect 0x2661 appears on the hitter,
   sprite bit 6 of unk11 is cleared and the part counts down in obj_phase_countdown. Returns
   lookup_slot's result. */
u32 obj_lookup_slot_or_place(struct SceneObj *part, struct SceneObj *hitter)
{
    u32 result;

    result = lookup_slot(part, hitter);
    if (result == 0)
    {
        struct SceneData *data = part->data;
        struct SceneObj *thrower = (struct SceneObj *)((u8 *)data + 8);

        /* positions are in 1/256 pixels */
        place_targets(part, thrower->posX / 0x100 + 4, thrower->posY / 0x100 + 1, thrower->posZ / 0x100 + 0x28, -1);
        set_step_and_offsets(part, gNodeChainStepTable[data->unkB0], 0x50);
        effect_spawn_at(0x2661, hitter->posX / 0x100, hitter->posY / 0x100, hitter->posZ / 0x100, hitter);
        ((struct SpriteUnk11Bits *)&part->sprite->unk11)->bit6 = 0;
        part->phase = result;
        part->state = obj_phase_countdown;
    }
    return result;
}


/* Once obj's animation has finished (sprite flags bit 3), starts its action 0xC and throws a
   linked pair (obj_spawn_two_linked) at each of the two objects at gWorld+0x80/+0x84 that is in
   mode 2: with both active, one pair goes at the first (offset -8,-2, motion b) and one at the
   second (8,2, motion a); with only one active, one pair goes at it. Then resets the timer, moves
   on to obj_anim_d_if_timer and plays sound 0x10E (set_slot_value). */
void obj_anim_c_link_pair(struct SceneObj *obj)
{
    struct SceneObj *first;
    struct SceneObj *second;
    u32 firstMode;

    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        first = gWorld->unk80;
        second = gWorld->unk84;
        firstMode = first->unk7E & 6;
        if (firstMode == 2)
        {
            if ((second->unk7E & 6) == 2)
            {
                obj_spawn_two_linked(obj, (u8 *)first + 8, -8, -2, 10, firstMode, obj_place_owner_target_b);
                obj_spawn_two_linked(obj, (u8 *)second + 8, 8, 2, 10, firstMode, obj_place_owner_target_a);
            }
            else
            {
                obj_spawn_two_linked(obj, (u8 *)first + 8, 8, 2, 10, firstMode, obj_place_owner_target_a);
            }
        }
        else
        {
            obj_spawn_two_linked(obj, (u8 *)second + 8, -8, -2, 10, 2, obj_place_owner_target_b);
        }
        obj->timer = 0;
        obj->state = obj_anim_d_if_timer;
        set_slot_value(0x10E, -1);
    }
}


void obj_wait_blend_then_detach(struct SceneObj *obj)
{
    s32 count;
    s32 level;

    if ((obj->sprite->flags12 & 0xE) == 0xA)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj_list_push_default(obj);
        obj->callback5C = (void *)partner_link_update;
    }
    count = ((struct SceneSignedCount *)obj->owner)->count;
    if (obj->phase != 0)
        level = count - 2;
    else
        level = count + 2;
    obj->unk75 = level;
    if ((u8)obj_blend_position(obj) == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj_list_remove(obj);
        obj->data->modeByte--;
        obj->state = obj_wait_scene_mode;
    }
}


void obj_wait_scene_b4_then_anim4(struct SceneObj *obj)
{
    if (obj->data->unkB4 == 1)
    {
        return;
    }
    if (obj->data->unkB4 == 0)
    {
        obj->timer--;
        if (obj->timer == 0)
        {
            obj->data->unkB4 = 1;
        }
    }
    else
    {
        sprite_set_action(obj, 3, 0, 0);
        ((struct SceneSpriteBits *)&obj->sprite->flags12)->mode = 1;
        obj->state = obj_start_anim4_if_active;
        set_slot_value(0x88, -1);
    }
}


/* Spawns the two halves (0x4041 and 0x4042) of a linked pair at the first one's spawn point moved
   by (dx, dy, dz) px, both owned by `owner` and running `state`. Half B draws one layer above
   obj and half A one below (phase 0 and 1); obj's data counts both as alive. A nonzero level
   is stored (minus 1) in half B's level bits. */
void obj_spawn_two_linked(struct SceneObj *obj, void *owner, s32 dx, s32 dy, s32 dz, s32 level, void *state)
{
    struct SceneObj *childA;
    struct SceneObj *childB;
    struct SceneSprite *spriteA;
    struct SceneSprite *spriteB;
    u16 ofsX;
    u16 ofsY;
    u16 ofsZ;
    u16 stage;
    s32 x;
    s32 y;
    s32 z;

    ofsX = dx;
    ofsY = dy;
    ofsZ = dz;
    stage = level;
    childA = obj_spawn_linked(obj, 0x4041, 0);
    childB = obj_spawn_linked(obj, 0x4042, 0);
    /* (s16)ofs << 8: whole pixels to 8.8 */
    x = childA->posX + ((s32)(ofsX << 16) >> 8);
    childA->posX = x;
    childB->posX = x;
    childA->dispX = x;
    childB->dispX = x;
    y = childA->posY + ((s32)(ofsY << 16) >> 8);
    childA->posY = y;
    childB->posY = y;
    childA->dispY = y;
    childB->dispY = y;
    z = childA->posZ + ((s32)(ofsZ << 16) >> 8);
    childA->posZ = z;
    childB->posZ = z;
    childA->dispZ = z;
    childB->dispZ = z;
    childA->owner = owner;
    childB->owner = owner;
    spriteB = childB->sprite;
    spriteA = childA->sprite;
    ((struct SceneSpriteBits *)&spriteA->flags12)->mode = 1;
    ((struct SceneSpriteBits *)&spriteB->flags12)->mode = 1;
    childA->state = state;
    childB->state = state;
    /* draw layer */
    childB->unk75 = obj->unk75 + 1;
    childA->unk75 = obj->unk75 - 1;
    childB->phase = 0;
    childA->phase = 1;
    obj->data->modeByte += 2;
    if (stage != 0)
    {
        ((struct StageBits *)&childB->unk78)->hi3 = stage - 1;
    }
    else
    {
        ((struct Flag79Low *)&childB->flags79)->bit0 = 1;
    }
    ((struct Flag79Low *)&childB->flags79)->bit0 = 1;
}


NONMATCH("asm/code_0811229C/obj_set_motion_from_keys.s", void obj_set_motion_from_keys(struct SceneObj *obj, u32 speed, u32 b))
{
    s32 out1;
    s32 out2;
    u32 clamped;
    s32 spanX;
    s32 spanZ;
    s32 first;
    s32 lenSq;
    s32 len;
    u32 result;

    clamped = clamp_reflect_offsets(&out1, &out2, ((s16 *)&obj->unk88)[0] << 8, ((s16 *)&obj->unk88)[1] << 8, b << 8);
    obj->unk92 = clamped;
    b = (s16)clamped;
    spanX = (obj->unk84 >> 16) - (s16)obj->unk84;
    spanZ = (obj->unk8C >> 16) - (s16)obj->unk8C;
    first = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(out1 * ((out1 * ((s32)b - ((s16 *)&obj->unk88)[0])) / 0x40), out2 * (out2 / 0x40));
    lenSq = spanX * spanX + first * first + spanZ * spanZ;
    len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(lenSq);
    if (len != 0)
    {
        result = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((speed << 6) + len / 2, len);
    }
    else
    {
        result = 0x4000;
    }
    obj->stepX = result;
    ((struct SceneFlag79Bits *)&obj->flags79)->bit5 = 0;
    obj->unk94 = out1;
    obj->orbitAngle = out2;
}
END_NONMATCH
