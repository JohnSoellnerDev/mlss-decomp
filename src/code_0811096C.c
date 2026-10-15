#include "global.h"
#include "ram_routines.h"

struct LayoutNode
{
    struct LayoutNode *next;
    s32 x;
    s32 y;
    s32 z;
};

struct LayoutOffsets
{
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct ActionPart
{
    u8 filler0[0x14];
    struct LayoutOffsets offs;
    u8 filler24[0x38 - 0x24];
    struct LayoutNode *unk38;
    u8 filler3C[0x7D - 0x3C];
    u8 unk7D;
    u8 filler7E[0xD8 - 0x7E];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler0E4[0xEC - 0xE4];
    s16 kind;
    u8 filler0EE[0x113 - 0xEE];
    u8 animFlag113;
    u8 filler114[0x11E - 0x114];
    u16 flags;
};

struct ActionRecord
{
    u8 filler0[0x12];
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F;
    u8 display;
    u8 unk21;
    u8 filler22;
    u8 unk23;
};

struct ActionFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 high : 4;
};

struct ActionObj
{
    u8 filler0[0x08];
    struct ActionRecord *record;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    s16 unk1C;
    s16 unk1E;
    u8 filler20[0x28 - 0x20];
    struct ActionPart *part;
    struct ActionObj *owner;
    struct ActionObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActionObj *);
    u8 filler50[0x58 - 0x50];
    void *callback58;
    void *callback5C;
    void (*handler)(struct ActionObj *);
    u8 filler64[0x75 - 0x64];
    u8 timer;
    u8 ownerFlags;
    u8 markerFlags;
    u8 filler78;
    u8 childFlags;
    u8 filler7A[0x7C - 0x7A];
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 link;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler90[0x92 - 0x90];
    u16 effectCount;
    s32 accum;
    s32 effectTimer;
    s32 variant;
    s32 scale;
    s32 display;
    s32 level;
    s16 counter;
    s16 dirX;
    s16 dirY;
    s16 steps;
};

struct AngleStep
{
    u16 step;
    u8 filler2[6];
};

extern const struct AngleStep gAngleStepTable[];
extern const u16 gWaveSlotTable[];

u8 step_motion(struct ActionObj *obj);
void sprite_set_action(struct ActionObj *obj, s32 action, u32 arg, u32 extra);
void sound_stop_song(u32 song);
void obj_release(struct ActionObj *obj);
void wait_for_index(struct ActionObj *obj);
void set_slot_value(u32 id, s32 value);
void place_targets(struct ActionObj *obj, s32 x, s32 y, s32 z, s32 extra);
void set_motion_start(struct ActionObj *obj, s32 speed);
void set_motion_from_value(struct ActionObj *obj, s32 value);
void obj_detach_active(struct ActionRecord *record);
void action_part_states_spawn_part_child(struct ActionObj *obj);
void start_action_2_then_wait(struct ActionObj *obj);
void start_action_2_then_wait_b(struct ActionObj *obj);
void start_place_on_part_44(struct ActionObj *obj);
void start_place_on_part_44_b(struct ActionObj *obj);
void start_move_to_fixed_target(struct ActionObj *obj);
void count_down_then_action_16(struct ActionObj *obj);
void hero_marker_begin(struct ActionObj *obj);
void hero_marker_reset(struct ActionObj *obj);
void hero_marker_arrive(struct ActionObj *obj);
void obj_anim_4_or_7_if_active(struct ActionObj *obj);
void obj_anim_10_or_11_if_active(struct ActionObj *obj);
void obj_anim_15_or_16(struct ActionObj *obj);
void obj_anim_11_or_12(struct ActionObj *obj);
void obj_wait_motion_spawn_part(struct ActionObj *obj);
void obj_stop_anim_if_active(struct ActionObj *obj);
void obj_spawn_part_chain(struct ActionObj *obj);
void obj_start_anim11_if_active(struct ActionObj *obj);
void obj_slide_in_from_left(struct ActionObj *obj);
void obj_lookup_link_or_move(struct ActionObj *obj);
void obj_fly_to_scene_target(struct ActionObj *obj);
void obj_finish_when_idle(struct ActionObj *obj);
void obj_motion_then_place_part(struct ActionObj *obj);
void obj_place_at_scene_offset(struct ActionObj *obj);
void node_chain_obj_release_when_idle(struct ActionObj *obj);
void obj_list_remove(struct ActionObj *obj);
u32 callback_return_1(void);
u32 callback_return_0(void);
void obj_link_or_cancel(struct ActionObj *obj);
void obj_wait_motion_then_flags(struct ActionObj *obj);
void obj_wave_step_a(struct ActionObj *obj);
void arc_step(struct ActionObj *obj);
void linear_step_then_arrive(struct ActionObj *obj);
void ease_step(struct ActionObj *obj);
void timed_effect_then_step(struct ActionObj *obj);
void link_and_start_action_from_flags(void);
struct ActionObj *obj_spawn_linked(struct ActionObj *parent, u32 kind, u32 value);
void wait_progress_then_return_or_destroy(struct ActionObj *obj);
void start_move_to_part(struct ActionObj *obj);
u32 mark_chain_then_state(struct ActionObj *obj);
u32 partner_link_update(struct ActionObj *obj, struct ActionObj *partner, u32 mode);
void end_action_clear_b(struct ActionObj *obj);
void sprite_entry_update(struct ActionRecord *record);
void obj_list_push_default(struct ActionObj *obj);
void countdown_anim_18(struct ActionObj *obj);
void set_step_counter_12(struct ActionObj *obj);
void end_action_clear(struct ActionObj *obj);
void wait_progress_then_action_11(struct ActionObj *obj);
void wait_progress_then_action_7(struct ActionObj *obj);
void wait_progress_then_action_7_b(struct ActionObj *obj);
void start_progress_30_then_wait(struct ActionObj *obj);
void start_progress_30_then_wait_b(struct ActionObj *obj);
void wait_child_then_place(struct ActionObj *obj);
void start_part_offset_effect(struct ActionObj *obj);
void start_part_place_b(struct ActionObj *obj);
void start_part_place_c(struct ActionObj *obj);
void start_part_place_list(struct ActionObj *obj);
void start_part_place(struct ActionObj *obj);
void wait_move_then_action_4(struct ActionObj *obj);
void wait_move_then_action_4_b(struct ActionObj *obj);
u8 candidates_advance_progress(struct ActionObj *obj);
u32 lookup_part(struct ActionObj *obj);
u32 lookup_link_slot(struct ActionObj *obj, struct ActionObj *other);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActionObj *owner);

void wait_move_then_action_8(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        obj->state = end_action_clear;
    }
}

void wait_move_then_start_progress(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        set_motion_from_value(obj, 0x28);
        sound_stop_song(0x112);
        obj->state = wait_progress_then_action_11;
    }
}

void wait_move_then_place_base(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        sound_stop_song(0x112);
        effect_spawn_at(0x215C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseZ / 0x100, obj);
        obj->state = start_progress_30_then_wait;
    }
}

void wait_move_then_clear(struct ActionObj *obj)
{
    u8 busy = step_motion(obj);

    if (busy == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        sound_stop_song(0x11B);
        obj->state = (void (*)(struct ActionObj *))(u32)busy;
    }
}

void wait_child_then_progress(struct ActionObj *obj)
{
    if (obj->child->record->flags & 8)
    {
        sprite_set_action(obj->child, 0, 0, 0);
        set_motion_from_value(obj, 0xC);
        set_slot_value(0x10F, -1);
        obj->state = wait_child_then_place;
    }
}

void wait_move_then_action_15(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 0xF, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        sound_stop_song(0x112);
        obj->state = set_step_counter_12;
    }
}

void wait_move_then_place_base_b(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        sound_stop_song(0x112);
        effect_spawn_at(0x215C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseZ / 0x100, obj);
        obj->state = start_progress_30_then_wait_b;
    }
}

u32 sprite_state_pair_a(struct ActionObj *obj)
{
    obj->state = start_action_2_then_wait;
    return 1;
}

u32 sprite_state_pair_d(struct ActionObj *obj)
{
    obj->state = start_action_2_then_wait_b;
    return 1;
}

void end_action_clear(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
    }
}

void wait_progress_then_action_11(struct ActionObj *obj)
{
    s32 flags;

    if (obj->record->unk23 == 1)
    {
        flags = obj->record->flags;
        obj->record->flags = flags | 0x10;
    }
    if (candidates_advance_progress(obj) == 0)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        obj->state = start_part_offset_effect;
    }
}

void start_progress_30_then_wait(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        set_motion_from_value(obj, 0x1E);
        obj->state = wait_progress_then_action_7;
    }
}

void start_action_2_then_wait(struct ActionObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
    obj->state = start_place_on_part_44;
}

void set_step_counter_12(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        obj->counter = 0xC;
        obj->state = count_down_then_action_16;
    }
}

void start_progress_30_then_wait_b(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        set_motion_from_value(obj, 0x1E);
        obj->state = wait_progress_then_action_7_b;
    }
}

void start_action_2_then_wait_b(struct ActionObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
    obj->state = start_place_on_part_44_b;
}

void wait_progress_then_action_7(struct ActionObj *obj)
{
    if (candidates_advance_progress(obj) == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        obj->state = start_part_place_c;
    }
}

void start_place_on_part_44(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        struct ActionPart *part = obj->owner->part;

        place_targets(obj, part->targetX / 0x100 + 0x44, part->targetY / 0x100, part->targetZ / 0x100, 0x100);
        set_motion_start(obj, 0x280);
        sprite_set_action(obj, 3, 0, 0);
        set_slot_value(0x11B, -1);
        obj->state = wait_move_then_action_4;
    }
}

void count_down_then_action_16(struct ActionObj *obj)
{
    u16 counter = obj->counter;

    if (obj->counter != 0)
    {
        obj->counter = counter - 1;
    }
    else
    {
        sprite_set_action(obj, 0x10, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        obj->state = action_part_states_spawn_part_child;
    }
}

void wait_progress_then_action_7_b(struct ActionObj *obj)
{
    if (candidates_advance_progress(obj) == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        obj->state = start_part_place_b;
    }
}

void start_place_on_part_44_b(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        struct ActionPart *part = obj->owner->part;

        place_targets(obj, part->targetX / 0x100 + 0x44, part->targetY / 0x100, part->targetZ / 0x100, 0x100);
        set_motion_start(obj, 0x280);
        sprite_set_action(obj, 3, 0, 0);
        set_slot_value(0x11B, -1);
        obj->state = wait_move_then_action_4_b;
    }
}

void wait_move_then_action_4(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        sound_stop_song(0x11B);
        obj->state = start_part_place_list;
    }
}

void wait_move_then_action_4_b(struct ActionObj *obj)
{
    if (step_motion(obj) == 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        sound_stop_song(0x11B);
        obj->state = start_part_place;
    }
}

void start_move_to_fixed_target(struct ActionObj *obj)
{
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 steps;
    s32 q;
    s32 *level;

    if (obj->record->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->targetX = 0x7800;
        obj->targetY = 0x6C00;
        obj->targetZ = 0x4000;
        dx = obj->targetX - obj->baseX;
        dy = obj->targetY - obj->baseY;
        dz = obj->targetZ - obj->baseZ;
        dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy + dz * dz);
        steps = dist / 0x200;
        q = dz / (steps * steps);
        level = &obj->level;
        if (q <= 1)
        {
            if (q < 0)
            {
                q = -q;
            }
            else
            {
                q = 2;
            }
        }
        *level = q;
        obj->dirX = dx / 0x100;
        obj->dirY = dy / 0x100;
        obj->steps = steps;
        obj->counter = 0;
        set_slot_value(0x8A, -1);
        obj->state = arc_step;
    }
}

void arc_step(struct ActionObj *obj)
{
    s32 kind;
    s32 anim;
    s32 counter;

    obj->posX = ((obj->dirX * obj->counter) << 8) / obj->steps + obj->part->targetX;
    obj->posY = ((obj->dirY * obj->counter) << 8) / obj->steps + obj->part->targetY;
    obj->posZ = (obj->counter * obj->counter) * obj->level + obj->part->targetZ;
    counter = (u16)obj->counter;
    counter = counter + 1;
    obj->counter = counter;
    if ((s16)counter > obj->steps)
    {
        kind = -obj->owner->part->kind;
        anim = 4;
        if (kind != 1)
        {
            anim = 0x15;
        }
        sprite_set_action(obj, anim, 0, 0);
        obj->posX = obj->targetX;
        obj->posY = obj->targetY;
        obj->posZ = obj->targetZ;
        obj->counter = 0x40;
        sound_stop_song(0x8A);
        set_slot_value(0x88, -1);
        obj->state = ease_step;
    }
}

void ease_step(struct ActionObj *obj)
{
    struct ActionPart *part;
    s32 counter;
    s32 z;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 steps;
    s32 q;
    s32 *level;

    z = obj->baseZ + gSineTable[(obj->counter * 4) & 0xFF] / 0x100;
    obj->baseZ = z;
    obj->posZ = z;
    counter = (u16)obj->counter;
    counter = counter - 1;
    obj->counter = counter;
    if ((s16)counter == 0)
    {
        part = obj->owner->part;
        obj->targetX = part->targetX;
        obj->targetY = part->targetY;
        obj->targetZ = part->targetZ;
        obj->unk1C = obj->baseX;
        obj->unk1E = obj->baseY;
        dx = obj->targetX - obj->baseX;
        dy = obj->targetY - obj->baseY;
        dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
        steps = dist / 0x100;
        q = obj->baseZ / (steps * steps);
        level = &obj->level;
        if (q <= 1)
        {
            if (q < 0)
            {
                q = -q;
            }
            else
            {
                q = 2;
            }
        }
        *level = q;
        obj->dirX = dx / 0x100;
        obj->dirY = dy / 0x100;
        obj->steps = steps;
        obj->accum = 0;
        obj->counter = 0x100;
        sprite_set_action(obj, 5, 0, 0);
        obj_list_push_default(obj);
        obj->callback58 = (void *)link_and_start_action_from_flags;
        obj->callback5C = (void *)mark_chain_then_state;
        obj->effectCount = 3;
        obj->effectTimer = 4;
        obj->state = timed_effect_then_step;
        sound_stop_song(0x88);
        set_slot_value(0x10F, -1);
    }
}
