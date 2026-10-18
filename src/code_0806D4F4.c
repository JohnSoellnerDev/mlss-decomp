#include "global.h"
#include "ram_routines.h"

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
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0x11E - 0xEE];
    u16 unk11E;
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
    struct ActorObj *kind;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    u8 filler5C[0x60 - 0x5C];
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 unk6C;
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
    s32 phase;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 side;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ActorFlagBits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 high : 4;
};

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
u32 obj_lookup_part_to_drift(struct ActorObj *obj, u32 arg);
void obj_wave_drift(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_timer_sprite_move_1(struct ActorObj *obj);
void side_walk_obj_index_zero_action4(struct ActorObj *obj);
void obj_timer_sprite_move_2(struct ActorObj *obj);
void obj_timer_sprite_move_3(struct ActorObj *obj);
void obj_timer_sprite_countdown(struct ActorObj *obj);
void obj_timer_clear_slot_action6(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_timer_action5_setup(struct ActorObj *obj);
void obj_timer_exit_left(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 mt19937_next(void);
void obj_action2_then_fall_d70c(struct ActorObj *obj);
void obj_action2_then_fall_d408(struct ActorObj *obj);
void obj_fall_and_launch(struct ActorObj *obj);
u32 obj_lookup_part_pick_state(struct ActorObj *obj, u32 arg);
void obj_wait_30_when_index_zero(struct ActorObj *obj);
void obj_anim5_when_index_zero(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
void actor_side_walk_steps(struct ActorObj *obj);
void obj_setup_partner_pose(struct ActorObj *obj);
void obj_timer_clear_slot_action6_alt(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_reset_when_index_zero(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_detach_active(struct ActorPose *pose);
void sprite_entry_update(struct ActorPose *pose);
void obj_owner_release_motion(struct ActorObj *obj);
void obj_move_to_base_right(struct ActorObj *obj);
void obj_owner_callback_step(struct ActorObj *obj);
void obj_lookup_slot_step(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void obj_drift_up_until_height(struct ActorObj *obj);
void obj_aim_at_owner(struct ActorObj *obj);
void obj_prepare_when_index_clear(struct ActorObj *obj);
void obj_arc_land(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_finish_if_active(struct ActorObj *obj);
void obj_idle_when_active(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_sync_child_reset(struct ActorObj *obj);

struct ActorCands
{
    u8 filler0[0x70];
    u8 *cand70;
    u8 *cand74;
};

extern struct ActorCands *gWorld;

void obj_timer_exit_left(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        u16 *timer;
        s32 left;

        left = --obj->timer << 16;
        timer = &obj->timer;
        if (left <= 0)
        {
            sprite_set_action(obj, 5, 0, 0);
            set_slot_value(0xD2, -1);
            obj->handler68 = on_arrive;
            ((struct ActorModeBits *)&obj->mode)->bit6 = 0;
            ((struct ActorModeBits *)&obj->mode)->bit5 = 0;
            obj_list_push_default(obj);
            obj->handler58 = (void (*)(struct ActorObj *))obj_lookup_part_to_drift;
            if (obj->dx != 0)
            {
                obj->targetX = (obj->baseX - 0x5000) / 0x100;
            }
            else
            {
                obj->targetX = (obj->baseX - 0x3000) / 0x100;
            }
            obj->targetY = 0x12;
            obj->phase = 0;
            *timer = 10;
            obj->state = obj_wave_drift;
        }
    }
}

ASM_FUNC("asm/code_0806D4F4/obj_wave_drift.s", void obj_wave_drift(struct ActorObj *obj))
