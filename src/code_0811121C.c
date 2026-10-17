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
u32 obj_lookup_link_or_move(struct ActionObj *obj);
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
u32 lookup_link_slot(struct ActionObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActionObj *owner);

ASM_FUNC("asm/code_0811121C/timed_effect_then_step.s", void timed_effect_then_step(struct ActionObj *obj))

void start_move_to_part(struct ActionObj *obj)
{
    struct ActionPart *part;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 steps;
    s32 q;
    s32 *level;

    if (obj->child == 0)
    {
        part = obj->part;
        dx = part->targetX - obj->posX;
        dy = part->targetY - obj->posY;
        dz = part->targetZ - obj->posZ;
        dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
        steps = dist / 0x300;
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
        obj_list_remove(obj);
        obj->state = linear_step_then_arrive;
    }
}

void linear_step_then_arrive(struct ActionObj *obj)
{
    struct ActionPart *part;
    s32 counter;
    s32 x;
    s32 y;
    s32 z;
    u8 flag;

    obj->posX = ((obj->dirX * obj->counter) << 8) / obj->steps + obj->targetX;
    obj->posY = ((obj->dirY * obj->counter) << 8) / obj->steps + obj->targetY;
    obj->posZ = ((obj->counter - obj->steps) * (obj->counter - obj->steps)) * obj->level + obj->part->targetZ;
    counter = (u16)obj->counter;
    counter = counter + 1;
    obj->counter = counter;
    if ((s16)counter > obj->steps)
    {
        part = obj->part;
        x = part->targetX;
        obj->baseX = x;
        obj->posX = x;
        y = part->targetY;
        obj->baseY = y;
        obj->posY = y;
        z = part->targetZ;
        obj->baseZ = z;
        obj->posZ = z;
        flag = part->animFlag113;
        if (flag != 0)
        {
            sprite_set_action(obj, obj->record->unk21 + 4, 0, 0);
            ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
            obj->state = end_action_clear_b;
            sound_stop_song(0xA3);
        }
        else
        {
            sprite_set_action(obj, -1, -1, 0);
            obj->state = (void (*)(struct ActionObj *))(u32)flag;
        }
    }
}

void wait_progress_then_return_or_destroy(struct ActionObj *obj)
{
    struct ActionObj *src;
    s32 flags;
    s32 x;
    s32 y;
    s32 z;
    u32 key;
    s32 action;

    if (candidates_advance_progress(obj) == 0)
    {
        src = (struct ActionObj *)((u8 *)obj->part + 8);
        if (obj->level == 0 && src->posX < src->part->targetX)
        {
            x = src->posX;
            obj->baseX = x;
            obj->posX = x;
            y = src->posY;
            obj->baseY = y;
            obj->posY = y;
            z = src->posZ;
            obj->baseZ = z;
            obj->posZ = z;
            obj->timer = obj->part->unk7D + 1;
            key = src->record->unk16 | ((((u32)src->record->unk1E << 28) >> 16) + 0x1000);
            if (key == 0x4136)
            {
                action = src->record->unk21 + 2;
            }
            else
            {
                action = 0;
            }
            sprite_set_action(obj, action, 0, 0);
            obj->record->unk23 = src->record->unk23;
            flags = obj->record->flags;
            obj->record->flags = flags | 0x10;
            set_motion_from_value(obj, 0xC);
        }
        else
        {
            obj_release(obj);
        }
    }
}

ASM_FUNC("asm/code_0811121C/link_and_start_action_from_flags.s", void link_and_start_action_from_flags(void))

u32 mark_chain_then_state(struct ActionObj *obj)
{
    struct ActionObj *node;
    u32 result;

    result = lookup_part(obj);
    if (result == 0)
    {
        for (node = obj->child; node != 0; node = node->child)
        {
            node->level = 1;
        }
        obj->state = countdown_anim_18;
        sound_stop_song(0x10F);
        result = 0;
    }
    return result;
}

void end_action_clear_b(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = 0;
    }
}

u32 sprite_state_pair_b(struct ActionObj *obj)
{
    obj->state = hero_marker_begin;
    return 1;
}

u32 sprite_state_pair_e(struct ActionObj *obj)
{
    obj->state = hero_marker_reset;
    return 1;
}

void hero_marker_begin(struct ActionObj *obj)
{
    sprite_set_action(obj, -obj->owner->part->kind + 0xD, 0, 0);
    ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
    set_slot_value(0x119, -1);
    obj->state = hero_marker_arrive;
}

void countdown_anim_18(struct ActionObj *obj)
{
    s32 remaining = obj->posZ - 0x200;

    obj->posZ = remaining;
    if (remaining <= 0)
    {
        obj->posZ = 0;
        obj->state = wait_for_index;
    }
}

void hero_marker_reset(struct ActionObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
    obj->part->animFlag113 = 0;
    obj->state = start_move_to_fixed_target;
}

void hero_marker_arrive(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        set_motion_from_value(obj, 0x20);
        obj->posX = obj->owner->part->targetX + 0x2000;
        obj->baseX = obj->posX;
        obj->posY = obj->owner->part->targetY;
        obj->baseY = obj->posY;
        obj->posZ = obj->owner->part->targetZ + 0x800;
        obj->baseZ = obj->posZ;
        obj_detach_active(obj->record);
        sound_stop_song(0x119);
        obj->state = obj_wait_motion_spawn_part;
    }
}

void obj_wait_motion_spawn_part(struct ActionObj *obj)
{
    u8 busy = candidates_advance_progress(obj);

    if (busy == 0)
    {
        sprite_set_action(obj, 0x10, 0, 0);
        sprite_entry_update(obj->record);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        effect_spawn_at(0x219E, 0, 0, 0, (struct ActionObj *)(u32)busy);
        obj_list_push_default(obj);
        obj->handler = (void (*)(struct ActionObj *))obj_lookup_link_or_move;
        set_slot_value(0xD5, -1);
        obj->state = obj_start_anim11_if_active;
    }
}

NONMATCH("asm/code_0811121C/obj_lookup_link_or_move.s", u32 obj_lookup_link_or_move(struct ActionObj *obj))
{
    struct ActionObj *o = obj;
    u32 slot;
    u32 result;

    slot = lookup_link_slot(o);
    if (slot == 0)
    {
        set_motion_start(o, 0x300);
        sprite_set_action(o, 1, 0, 0);
        {
            u32 *addr = (u32 *)((u8 *)o + 0x80);
            *addr = slot;
        }
        result = 0;
    }
    else
    {
        result = slot;
    }
    return result;
}
END_NONMATCH

void obj_start_anim11_if_active(struct ActionObj *obj)
{
    if (obj->record->flags & 8)
    {
        sprite_set_action(obj, 0x11, 0, 0);
        set_motion_from_value(obj, 0x10);
        obj->timer = obj->owner->timer + 1;
        sound_stop_song(0xD5);
        obj->state = obj_slide_in_from_left;
    }
}

void obj_slide_in_from_left(struct ActionObj *obj)
{
    if (candidates_advance_progress(obj) == 0)
    {
        place_targets(obj, -0x20, obj->baseY / 0x100, obj->baseZ / 0x100, 0x100);
        set_motion_start(obj, 0x200);
        sprite_set_action(obj, 0x12, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        set_slot_value(0x83, -1);
        obj->state = obj_fly_to_scene_target;
    }
}

void obj_fly_to_scene_target(struct ActionObj *obj)
{
    struct ActionPart *part;
    s32 y;
    s32 z;

    if (step_motion(obj) == 0)
    {
        obj_list_remove(obj);
        part = obj->part;
        obj->posX = 0x10000;
        obj->baseX = 0x10000;
        y = part->targetY;
        obj->posY = y;
        obj->baseY = y;
        z = part->targetZ;
        obj->posZ = z;
        obj->baseZ = z;
        sprite_set_action(obj, 3, 0, 0);
        place_targets(obj, part->targetX / 0x100, part->targetY / 0x100, part->targetZ / 0x100, 0x100);
        set_motion_start(obj, 0x300);
        sound_stop_song(0x83);
        obj->state = obj_finish_when_idle;
    }
}

void obj_finish_when_idle(struct ActionObj *obj)
{
    u8 busy = step_motion(obj);

    if (busy == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->state = (void (*)(struct ActionObj *))(u32)busy;
    }
}

ASM_FUNC("asm/code_0811121C/obj_spawn_part_chain.s", void obj_spawn_part_chain(struct ActionObj *obj))

ASM_FUNC("asm/code_0811121C/obj_layout_node_chain.s", void obj_layout_node_chain(void))
