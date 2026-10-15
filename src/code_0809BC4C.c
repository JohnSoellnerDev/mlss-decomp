#include "global.h"
#include "ram_routines.h"

struct ActorSprite
{
    s16 unk0;
    u8 filler2[0x12 - 0x2];
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 kind;
};

struct SpriteFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ActorBody;

struct SpriteNode
{
    struct SpriteNode *next;
    struct ActorSprite *sprite;
    struct SpriteNode **link;
};

struct ActorFlagBits
{
    u8 low : 5;
    u8 active : 1;
    u8 high : 2;
};

struct ActorFlagByte
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct ActorData
{
    u8 filler0[0x38];
    struct ActorBody *members;
    u8 filler3C[0xF6 - 0x3C];
    s16 effect;
};

struct ActorBody
{
    u8 filler0[0x04];
    struct SpriteNode *node;
    struct ActorSprite *sprite;
    struct SpriteNode *parts;
    s32 x;
    s32 y;
    s32 z;
    s16 timerA;
    s16 timerB;
    s16 timerC;
    u8 filler22[0x28 - 0x22];
    struct ActorData *data;
    struct ActorBody *partner;
    struct ActorBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x45 - 0x44];
    s8 unk45;
    u8 filler46[0x4C - 0x46];
    void (*state)(struct ActorBody *body);
    u32 unk50;
    void (*unk54)(struct ActorBody *body);
    u32 unk58;
    void (*unk5C)(struct ActorBody *body);
    void (*unk60)(struct ActorBody *body);
    u8 filler64[0x6C - 0x64];
    s32 unk6C;
    u8 filler70[0x75 - 0x70];
    u8 index;
    u8 mode;
    u8 filler77;
    u8 unk78;
    u8 flags;
    u16 handle;
    u16 unk7C;
    u16 handleB;
    u8 filler80[0x84 - 0x80];
    s32 baseX;
    s32 baseY;
    u8 filler8C[0x9C - 0x8C];
    u32 timeLink;
    u32 unkA0;
    u32 unkA4;
    s32 link;
    u8 fillerAC[0xAE - 0xAC];
    u16 vspeed;
};

struct Hero
{
    u8 filler0[0x08];
    struct ActorBody body;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *focus;
};

extern struct World *gUnk_03000FD8;

void obj_release(void *obj);
void release_object(void *obj);
void task_list_remove(void *obj);
void sprite_set_action(struct ActorBody *body, u32 action, u32 arg, u32 extra);
void step_motion_d(struct ActorBody *body);
void apply_pose_to_parts(struct ActorBody *body);
void set_slot_value(u32 id, s32 value);
void state_stack_advance_progress(struct ActorBody *body);
void partner_link_update(struct ActorBody *body, struct ActorBody *partner, u32 mask);
void place_targets(struct ActorBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct ActorBody *body, u32 speed);
void start_anim_by_hero(struct ActorBody *body);
void fx_anim8(struct ActorBody *body);
void set_anim_0b_mode1(struct ActorBody *body);
void anim_0c_fx_at_pos(struct ActorBody *body);
void start_child_chain(struct ActorBody *body);
void spawn_five_children(struct ActorBody *body);
void anim_stop_if_sprite_active(struct ActorBody *body);
void draw_if_visible_b(struct ActorBody *body);
void draw_if_visible_c(struct ActorBody *body);
void draw_if_visible_d(struct ActorBody *body);
void fx_at_partner_a(struct ActorBody *body);
void fx_at_partner_b(struct ActorBody *body);
void fx_at_partner_link(struct ActorBody *body);
void float_above_focus(struct ActorBody *body);
void start_anim10(struct ActorBody *body);
void start_pose_4(struct ActorBody *body);
void start_pose_2(struct ActorBody *body);
void stop_after_index_bump(struct ActorBody *body);
void place_beside_partner(struct ActorBody *body);
void rise_anim1(struct ActorBody *body);
void link_anim3_a(struct ActorBody *body);
void link_anim3_b(struct ActorBody *body);
void start_anim1_b(struct ActorBody *body);
void start_anim9(struct ActorBody *body);
void start_wander_timers(struct ActorBody *body);
void attach_to_heroes(struct ActorBody *body);
void choose_hero_target(struct ActorBody *body);
void start_anim_by_partner(struct ActorBody *body);
struct ActorBody *obj_spawn_linked(struct ActorBody *body, u32 art, struct ActorBody *partner);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct ActorBody *owner);
void step_motion(struct ActorBody *body);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void follow_focus_while_effect(struct ActorBody *body);
void place_on_focus_a(struct ActorBody *body);
void place_on_focus_b(struct ActorBody *body);
void place_on_focus_c(struct ActorBody *body);
void release_fx_a(struct ActorBody *body);
void release_fx_b(struct ActorBody *body);
void remove_when_expired(struct ActorBody *body);
void start_anim3_c(struct ActorBody *body);
void fx_only_a(struct ActorBody *body);
void fx_only_b(struct ActorBody *body);
void fx_only_c(struct ActorBody *body);
void check_offscreen_b(struct ActorBody *body);
void obj_list_push_arg(struct ActorBody *obj, u32 arg);
void obj_detach_active(struct ActorSprite *sprite);
void wander_step(struct ActorBody *body);
void set_focus_pos_a(struct ActorBody *body);
struct ActorBody *obj_spawn(struct ActorData *data, u32 art, s32 x, s32 y, s32 z, u32 a, u32 b);
void *sprite_node_take(void);
void *sprite_node_alloc(s32 x, s32 y, s32 h);
void orbit_parts_falling(struct ActorBody *body);
void pose_update_b(struct ActorBody *body);
void set_focus_pos_b(struct ActorBody *body);
void set_focus_pos_c(struct ActorBody *body);
void free_data_done(struct ActorBody *body);
void rise_stop_b(struct ActorBody *body);
void wait_child_idle(struct ActorBody *body);
void step_motion_scaled(struct ActorBody *body);
void enter_start_pose_4(struct ActorBody *body);
void clear_state_if_sprite_active(struct ActorBody *body);
extern const s8 gChildChainOffsetTable[];
u32 mt19937_next(void);
void approach_contact(struct ActorBody *body);
void approach_slot(struct ActorBody *body);
void obj_list_push_default(struct ActorBody *obj);
void set_motion_speed_aux(struct ActorBody *body, u32 speed, u32 aux);
void draw_while_group_busy(struct ActorBody *body);
u32 mt19937_next(void);
s32 sub_081DE850(s32 range, u32 seed);

void spawn_child_anim(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;
    struct ActorBody *child;
    void (*fn)(struct ActorBody *body);

    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        child = obj_spawn_linked(body, 0x40C2, partner);
        fn = rise_anim1;
        child->state = fn;
        fn(child);
        if (body->data->effect != 0)
        {
            body->handle = 0x4000;
            body->handleB = 0;
            ((struct ActorFlagBits *)&body->flags)->active = 0;
        }
        body->state = choose_hero_target;
    }
}

ASM_FUNC("asm/code_0809BC4C/follow_hero_anim.s", void follow_hero_anim(void))
