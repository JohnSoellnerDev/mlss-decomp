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
    /* wander_step's shot timers. timerA and timerB: shots left at hero A / hero B (high byte) and
     * frames to the next one (low byte); timerC: frames to the next untargeted shot. */
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

extern struct World *gWorld;

/* ActorBody.flags: set by state_stack_advance_progress once the motion has finished */
#define ACTOR_FLAG_MOTION_DONE 0x20

/* the art (sprite set) of the strike attach_to_heroes drops on each hero */
#define ART_HERO_STRIKE 0x40C2

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
s32 rand_below(s32 range, u32 seed);

u32 part_step_start_anim_by_hero(struct ActorBody *body)
{
    body->unk50 = 0;
    body->state = start_anim_by_hero;
    body->unk7C = 0x100;
    return 1;
}

void start_anim1(struct ActorBody *body)
{
    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        sprite_set_action(body, 1, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        set_slot_value(0x15D, -1);
        body->state = fx_at_partner_a;
    }
}

void rise_anim1(struct ActorBody *body)
{
    body->y += 0x100;
    body->z = 0x2600;
    sprite_set_action(body, 1, 0x40C2, 0);
    ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
    set_slot_value(0x15D, -1);
    body->state = fx_at_partner_b;
}

void fx_at_partner_a(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;

    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 2, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        effect_spawn_at(0x1202, partner->x / 0x100, partner->y / 0x100, 0, partner);
        body->state = link_anim3_a;
    }
}

void fx_at_partner_b(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;

    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 2, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        effect_spawn_at(0x1202, partner->x / 0x100, partner->y / 0x100, 0, partner);
        body->state = link_anim3_b;
    }
}

void link_anim3_a(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;

    if (partner->targetZ == 0 && (partner->mode & 0x3E) == 2)
    {
        partner_link_update(body, partner, 0xFF);
    }
    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 3, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        body->state = draw_if_visible_b;
    }
}

void link_anim3_b(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;

    if (partner->targetZ == 0 && (partner->mode & 0x3E) == 2)
    {
        partner_link_update(body, partner, 0xFF);
    }
    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 3, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        body->state = draw_if_visible_c;
    }
}

void draw_if_visible_b(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        obj_release(body);
    }
}

void draw_if_visible_c(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        obj_release(body);
    }
}

void start_anim10(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 0xA, 0x40BF, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        if (body->data->effect != 0)
        {
            body->handle = 0x4000;
            body->handleB = 0;
            ((struct ActorFlagBits *)&body->flags)->active = 0;
        }
        body->state = attach_to_heroes;
    }
}

/* Once the caster's motion has finished: plays effect 0x11F0 at its position and drops a strike
 * (ART_HERO_STRIKE, starting in rise_stop_b) on each hero whose mode is 2, making that hero the
 * caster's partner; then waits for the strikes to end (wait_child_idle). */
void attach_to_heroes(struct ActorBody *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct ActorBody *bodyA = &heroA->body;
    struct Hero *heroB = gWorld->heroB;
    struct ActorBody *bodyB = &heroB->body;
    struct ActorBody *strike;
    void (*firstState)(struct ActorBody *body);

    state_stack_advance_progress(body);
    if (body->flags & ACTOR_FLAG_MOTION_DONE)
    {
        effect_spawn_at(0x11F0, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
        if ((heroA->body.mode & 6) == 2)
        {
            body->partner = bodyA;
            strike = obj_spawn_linked(body, ART_HERO_STRIKE, bodyA);
            firstState = rise_stop_b;
            strike->state = firstState;
            firstState(strike);
        }
        if ((heroB->body.mode & 6) == 2)
        {
            body->partner = bodyB;
            strike = obj_spawn_linked(body, ART_HERO_STRIKE, bodyB);
            firstState = rise_stop_b;
            strike->state = firstState;
            firstState(strike);
        }
        body->state = wait_child_idle;
    }
}

void fx_at_partner_link(struct ActorBody *body)
{
    struct ActorBody *partner = body->partner;

    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 2, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        effect_spawn_at(0x1202, partner->x / 0x100, partner->y / 0x100, 0, partner);
        if (partner->targetZ == 0 && (partner->mode & 0x3E) == 2)
        {
            partner_link_update(body, partner, 0xFF);
        }
        body->state = start_anim3_c;
    }
}

void start_anim3_c(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        sprite_set_action(body, 3, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        body->state = draw_if_visible_d;
    }
}

void rise_stop_b(struct ActorBody *body)
{
    body->y += 0x100;
    body->z = 0x2600;
    if (body->data->effect != 0)
    {
        body->handle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x28);
        body->handleB = 0;
        ((struct ActorFlagBits *)&body->flags)->active = 0;
    }
    body->state = start_anim1_b;
}

void wait_child_idle(struct ActorBody *body)
{
    if (body->sprite->kind != 0)
    {
        if ((body->sprite->flags & 0x8) == 0)
        {
            return;
        }
        sprite_set_action(body, 0, 0x40BF, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 0;
    }
    if (body->child == 0)
    {
        body->state = 0;
    }
}

u32 part_step_fx_anim8(struct ActorBody *body)
{
    body->unk50 = 0;
    body->state = fx_anim8;
    body->unk7C = 0x100;
    return 1;
}

void draw_if_visible_d(struct ActorBody *body)
{
    if (body->sprite->flags & 0x8)
    {
        obj_release(body);
    }
}

void start_anim1_b(struct ActorBody *body)
{
    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        sprite_set_action(body, 1, 0x40C2, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        set_slot_value(0x15D, -1);
        body->state = fx_at_partner_link;
    }
}

void fx_anim8(struct ActorBody *body)
{
    if (body->timeLink != 0)
    {
        sprite_set_action(body, 8, 0x40BF, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
    }
    effect_spawn_at(0x11E6, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
    body->state = start_anim9;
}

void start_anim9(struct ActorBody *body)
{
    if (body->timeLink == 0 || (body->sprite->flags & 0x8))
    {
        set_slot_value(0x65, -1);
        sprite_set_action(body, 9, 0x40BF, 0);
        ((struct SpriteFlagBits *)&body->sprite->flags)->mode = 1;
        body->state = start_anim10;
    }
}

/* Once the sprite's animation has ended, detaches it and sets up wander_step's barrage: a shot
 * period of 70-89 frames, hero A's first shot a random part of it away and hero B's half a period
 * after that, each with 3 + rand_below(1) shots, and the first untargeted shot in 35-49 frames.
 * If the data has an effect, the handle is reset to 0x4000 / 0x20 and the active bit cleared. */
void start_wander_timers(struct ActorBody *body)
{
    s32 period;
    s32 phase;
    s32 half;
    s32 phaseB;

    if (body->sprite->flags & 0x8)
    {
        obj_detach_active(body->sprite);
        if (body->data->effect != 0)
        {
            body->handle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x20);
            body->handleB = 0;
            ((struct ActorFlagBits *)&body->flags)->active = 0;
        }
        period = rand_below(20, mt19937_next()) + 70;
        phase = rand_below(period, mt19937_next());
        body->timerA = ((rand_below(1, mt19937_next()) + 3) << 8) | (period - phase);
        half = period / 2;
        phaseB = phase - half;
        if (phase < half)
        {
            phaseB = phase + half;
        }
        phase = phaseB;
        body->timerB = ((rand_below(1, mt19937_next()) + 3) << 8) | (period - phase);
        body->timerC = rand_below(15, mt19937_next()) + 35;
        body->state = wander_step;
    }
}

/* Each frame of the barrage: counts down the shot timers and, when one runs out, launches a projectile
   (art 0x40C4) that flies in from the right at hero A, at hero B or at no one (set_focus_pos_a/b/c),
   pushing the untargeted shot back by 35-49 frames. Once all shots are spent it waits for them to end
   (free_data_done). */
void wander_step(struct ActorBody *body)
{
    struct ActorBody *heroA;
    struct ActorBody *heroB;
    struct ActorBody *shot;
    void (*launch)(struct ActorBody *body);
    s32 frames;
    s32 delay;
    u16 rawFrames;

    heroA = &gWorld->heroA->body;
    heroB = &gWorld->heroB->body;
    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        if ((u8)body->timerA != 0)
        {
            body->timerA = body->timerA - 1;
        }
        else if (body->timerA & 0xFF00)
        {
            body->timerA = body->timerA - 0x100;
            delay = rand_below(20, mt19937_next()) + 70;
            body->timerA = delay + body->timerA;
            delay = rand_below(15, mt19937_next()) + 35;
            body->timerC = delay + body->timerC;
            body->partner = heroA;
            shot = obj_spawn_linked(body, 0x40C4, 0);
            launch = set_focus_pos_a;
            shot->state = launch;
            launch(shot);
        }
        if ((u8)body->timerB != 0)
        {
            body->timerB = body->timerB - 1;
        }
        else if (body->timerB & 0xFF00)
        {
            body->timerB = body->timerB - 0x100;
            delay = rand_below(20, mt19937_next()) + 70;
            body->timerB = delay + body->timerB;
            delay = rand_below(15, mt19937_next()) + 35;
            body->timerC = delay + body->timerC;
            body->partner = heroB;
            shot = obj_spawn_linked(body, 0x40C4, 0);
            launch = set_focus_pos_b;
            shot->state = launch;
            launch(shot);
        }
        rawFrames = body->timerC;
        frames = body->timerC;
        if (frames != 0)
        {
            body->timerC = rawFrames - 1;
        }
        else if (body->timerA != 0 && body->timerB != 0)
        {
            delay = rand_below(15, mt19937_next()) + 35;
            body->timerC = delay + body->timerC;
            body->partner = NULL;
            shot = obj_spawn_linked(body, 0x40C4, 0);
            launch = set_focus_pos_c;
            shot->state = launch;
            launch(shot);
        }
        if ((body->timerA | body->timerB | body->timerC) == 0)
        {
            body->state = free_data_done;
        }
    }
}

/* First state of a barrage shot aimed at hero A (spawned by wander_step): puts the shot 256 px right
 * of the camera (gWorld->focus) at height 120, flies it towards hero A's lane (camera + 224, 92,
 * height 60) when the data has an effect, plays sounds 0x88 and 0x8D (set_slot_value) and goes on to
 * the next leg (fx_only_c). */
void set_focus_pos_a(struct ActorBody *body)
{
    s32 destX;
    s32 destY;
    s32 startX;
    s32 keepVolume;

    startX = (s16)obj_last_x(gWorld->focus) * 0x100 + 0x10000;
    body->x = startX;
    body->targetX = startX;
    body->z = 0x7800;
    body->targetZ = 0x7800;
    body->unk78 = (body->unk78 & 0x1F) | 0x20;
    if (body->data->effect != 0)
    {
        destX = (s16)obj_last_x(gWorld->focus) + 0xE0;
        destY = (s16)obj_last_y(gWorld->focus) + 0x5C;
        place_targets(body, destX, destY, 0x3C, -1);
        set_motion_start(body, 0x140);
    }
    keepVolume = -1;
    set_slot_value(0x88, keepVolume);
    set_slot_value(0x8D, keepVolume);
    body->state = fx_only_c;
}
