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

ASM_FUNC("asm/code_081120B4/obj_wave_step_a.s", void obj_wave_step_a(struct ActionObj *obj))
