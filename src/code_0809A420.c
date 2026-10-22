#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u16 unk0;
    s16 unk2;
    u8 filler04[0x0C - 0x04];
    u16 unkC;
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F[0x21 - 0x1F];
    u8 unk21;
};

struct PoseTimer
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
};

struct SpriteBit4
{
    u8 low : 4;
    u8 bit4 : 1;
    u8 high : 3;
};

struct ModeByte
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct BodyFlagBits
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct HeroBody;

typedef void (*BodyFunc)(struct HeroBody *);

struct BodyData
{
    u8 filler0[0xD8];
    s32 unkD8;
    s32 height;
    s32 unkE0;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseOfs;
    u8 filler0EE[0xF6 - 0xEE];
    s16 effect;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 z;
    s16 unk1C;
    s16 unk1E;
    u16 unk20;
    u8 filler22[0x28 - 0x22];
    struct BodyData *data;
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 *unk34;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    BodyFunc state;
    void (*subState)();
    u8 filler54[0x58 - 0x54];
    BodyFunc unk58;
    BodyFunc unk5C;
    BodyFunc unk60;
    u8 filler64[0x68 - 0x64];
    BodyFunc spriteCallback;
    u8 filler6C[0x74 - 0x6C];
    u8 unk74;
    u8 filler75;
    u8 unk76;
    u8 style;
    u8 unk78;
    u8 flags;
    u16 unk7A;
    u16 unk7C;
    u16 unk7E;
    u8 filler80[0x84 - 0x80];
    s32 startX;
    s32 startY;
    s32 unk8C;
    u8 filler90[0x94 - 0x90];
    u32 unk94;
    u32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u16 counter;
    s16 unkAE;
    s16 unkB0;
    u16 unkB2;
};

void step_motion_scaled(struct HeroBody *body);
u32 partner_link_update(struct HeroBody *obj, struct HeroBody *partner, u32 mode);
u32 lookup_slot(struct HeroBody *body, u32 arg);
void obj_list_remove(struct HeroBody *body);
void change_item_count(u32 item, u32 count);
void set_slot_value(u32 id, s32 value);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
extern u16 gUnk_0200004A;
extern u16 gUnk_02000052;
extern const u16 gActionCodes[];
extern u32 gPoseTable[];

struct ModelCtx
{
    u8 filler0[0x28];
    u16 unk28;
    u16 unk2A;
};

extern struct ModelCtx *gModelCtx;
extern u8 *gWorkBuf;
extern u8 gUnk_02000460[];

struct Hero;

struct World
{
    u8 filler0[0x0C];
    u8 unkC;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    struct HeroBody *focus;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

extern struct World *gWorld;

static inline void *focus_obj(void)
{
    return gWorld->focus;
}

void on_arrive(struct HeroBody *body);
void obj_list_push_default(struct HeroBody *body);
void task_create_list_48(void (*fn)(), struct HeroBody *body);
void update_layer_tiles(void);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void follow_hero_anim(struct HeroBody *body);
void spawn_child_anim(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void start_anim1(struct HeroBody *body);
void sprite_set_action(struct HeroBody *body, u32 action, u32 arg, u32 extra);
void obj_release(void *obj);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
s32 obj_last_y(void *obj);
s32 obj_last_x(void *obj);
void set_motion_start(struct HeroBody *body, u32 value);
void obj_detach_active(void *obj);
void obj_release_node(void *node);
u32 lookup_part(struct HeroBody *body, u32 arg1, u32 arg2);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void approach_partner(struct HeroBody *body);
void focus_follow_step_toward_partner(struct HeroBody *body);

void spawn_trail_part(struct HeroBody *body);
void trail_follow_partner(struct HeroBody *body);
void begin_tile_update(struct HeroBody *body);
u32 lookup_part_or_wait(struct HeroBody *body, u32 arg1, u32 arg2);
u32 unknown_part_steps_0809a7dc(struct HeroBody *body);
u32 sub_0809A7E8(struct HeroBody *body);
u32 sub_0809A7F4(struct HeroBody *body);
void update_pose_if_shown(struct HeroBody *body);
void start_anim6_then_effect(struct HeroBody *body);
void start_approach_anim2(struct HeroBody *body);
void run_sub_state(struct HeroBody *body);
void reset_anim2_then_9c85(struct HeroBody *body);
void effect_0d26_then_wait(struct HeroBody *body);
void anim8_when_shown(struct HeroBody *body);
void anim9_when_shown(struct HeroBody *body);
void anim10_when_shown(struct HeroBody *body);
void spawn_child_pose(struct HeroBody *body);
void child_jump_to_pose(struct HeroBody *body);
void apply_velocity_step(struct HeroBody *body);
u32 try_lookup_slot(struct HeroBody *body, u32 arg);
u32 finish_and_remove(struct HeroBody *body);
void draw_if_visible(struct HeroBody *body);
void follow_focus_then_pose(struct HeroBody *body);
void update_pose_if_shown_b(struct HeroBody *body);
u32 set_pose_mode_6_or_7(struct HeroBody *body);
u32 set_pose_mode_0_or_1(struct HeroBody *body);
void move_and_remove_offscreen(struct HeroBody *body);
void anim2_then_play_flag1(struct HeroBody *body);
void play_anim_by_flag1(struct HeroBody *body);
void play_anim_by_flag2(struct HeroBody *body);
void spawn_two_children(struct HeroBody *body);
void update_pose_timers(struct HeroBody *body);
void init_random_timer(struct HeroBody *body);
void seek_hero(struct HeroBody *body);
void fall_and_release(struct HeroBody *body);
void place_at_focus_y(struct HeroBody *body);
void update_pair_link(struct HeroBody *body);
void draw_if_visible(struct HeroBody *body);
void follow_focus_then_pose(struct HeroBody *body);
void hide_and_reset_slot(struct HeroBody *body);
void random_timers_init(struct HeroBody *body);
u32 part_step_start_anim_mode2(struct HeroBody *body);
void clear_state(struct HeroBody *body);
void start_anim_mode2(struct HeroBody *body);
void start_anim_mode3(struct HeroBody *body);
void start_anim4_fx(struct HeroBody *body);
void start_anim_by_hero(struct HeroBody *body);
void start_anim_by_partner(struct HeroBody *body);
void fx_then_stop(struct HeroBody *body);
void choose_hero_target(struct HeroBody *body);
void rise_stop(struct HeroBody *body);
void wait_child_gone(struct HeroBody *body);

struct HeroBody *obj_spawn(void *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void *sprite_node_alloc(s32 x, s32 y, s32 z);
void spawn_trail_part(struct HeroBody *body)
{
    struct HeroBody *child;
    void **node;
    void *head;
    s32 i;

    if (body->sprite->flags & 0x8) {
        sprite_set_action(body, 0xB, 0, 0);
        effect_spawn_at(0xD5F, body->targetX / 0x100, body->targetY / 0x100, body->targetZ / 0x100, body);
        child = obj_spawn(body->data, 0, body->targetX / 0x100 - 0x14, body->targetY / 0x100 - 8,
                          body->targetZ / 0x100, 1, 1);
        body->child = child;
        child->unk9C = 0;
        *(u32 *)((u8 *)child + 0x6C) = *(u32 *)((u8 *)body + 0x6C);
        child->partner = body->partner;
        child->startX = child->x;
        child->startY = child->y;
        child->unkAE = 0;
        for (i = 0; i < 19; i++)
        {
            node = sprite_node_alloc(child->targetX, child->targetY, child->targetZ);
            head = *(void **)((u8 *)child + 0xC);
            *node = head;
            *(void **)((u8 *)child + 0xC) = node;
        }
        child->state = trail_follow_partner;
        body->state = begin_tile_update;
        set_slot_value(0x8C, -1);
    }
}

ASM_FUNC("asm/code_0809A420/trail_follow_partner.s", void trail_follow_partner(struct HeroBody *body))

void begin_tile_update(struct HeroBody *body)
{
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gUnk_02000460, gWorkBuf + 0x3CD0, 0x20);
    gModelCtx->unk28 = gModelCtx->unk2A;
    gModelCtx->unk2A |= 0xFFFF;
    task_create_list_48(update_layer_tiles, body);
    body->state = update_pose_if_shown;
}


u32 lookup_part_or_wait(struct HeroBody *body, u32 arg1, u32 arg2)
{
    u32 found;

    body->subState = body->state;
    found = lookup_part(body, arg1, arg2);
    if (found == 0)
        body->state = run_sub_state;
    return found;
}

u32 unknown_part_steps_0809a7dc(struct HeroBody *body)
{
    body->state = start_anim6_then_effect;
    return 1;
}

u32 sub_0809A7E8(struct HeroBody *body)
{
    body->state = start_approach_anim2;
    return 1;
}

u32 sub_0809A7F4(struct HeroBody *body)
{
    body->state = reset_anim2_then_9c85;
    return 1;
}

void update_pose_if_shown(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
        on_arrive(body);
}

void start_anim6_then_effect(struct HeroBody *body)
{
    sprite_set_action(body, 6, 0, 0);
    body->state = effect_0d26_then_wait;
}

void start_approach_anim2(struct HeroBody *body)
{
    sprite_set_action(body, 2, 0, 0);
    body->counter = 0;
    body->startX = body->targetX;
    body->startY = body->targetY;
    body->state = approach_partner;
}

void run_sub_state(struct HeroBody *body)
{
    BodyFunc before = body->state;

    if (body->subState != 0)
        body->subState(body);
    if (before != body->state)
    {
        body->subState = body->state;
        body->state = run_sub_state;
    }
    if (body->sprite->unk21 == 0)
    {
        sprite_set_action(body, 5, 0, 0);
        body->state = body->subState;
    }
}

void reset_anim2_then_9c85(struct HeroBody *body)
{
    sprite_set_action(body, 2, 0, 0);
    body->counter = 0;
    body->startX = body->targetX;
    body->startY = body->targetY;
    body->state = focus_follow_step_toward_partner;
}

void effect_0d26_then_wait(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 7, 0, 0);
        effect_spawn_at(0xD26, body->targetX / 0x100, body->targetY / 0x100,
                        body->targetZ / 0x100, body);
        body->state = anim8_when_shown;
    }
}

void anim8_when_shown(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 8, 0, 0);
        body->state = anim9_when_shown;
    }
}

void anim9_when_shown(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 9, 0, 0);
        body->state = anim10_when_shown;
    }
}

void anim10_when_shown(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0xA, 0, 0);
        body->state = spawn_trail_part;
    }
}

NONMATCH("asm/code_0809A420/spawn_child_pose.s", void spawn_child_pose(struct HeroBody *body))
{
    struct HeroBody *child;
    s32 childX;
    s32 childY;
    s32 childZ;
    s32 kind;
    u32 value;

    if (body->sprite->flags & 8)
    {
        u32 *flags98 = &body->unk98;

        if ((body->unk98 & 2) == 0)
            sprite_set_action(body, 6, 0, 0);
        else
            sprite_set_action(body, 8, 0, 0);
        body->state = update_pose_if_shown_b;
        if (body->unk9C < 0)
            effect_spawn_at(0xF69, body->targetX / 0x100, body->targetY / 0x100,
                            body->targetZ / 0x100, body);
        else
            effect_spawn_at(0xF75, body->targetX / 0x100, body->targetY / 0x100,
                            body->targetZ / 0x100, body);
        childX = body->targetX / 0x100 - 0x12;
        childY = body->targetY / 0x100 + 1;
        childZ = body->targetZ / 0x100 + 0x1D;
        value = body->unk9C;
        if ((s32)value < 0)
            kind = 0x40D7;
        else
            kind = (value & 0x7F) + 0x4188;
        child = obj_spawn(body->data, kind, childX, childY, childZ, 1, 1);
        body->child = child;
        child->partner = body->partner;
        child->spriteCallback = on_arrive;
        *(u32 *)((u8 *)child + 0x6C) = *(u32 *)((u8 *)body + 0x6C);
        obj_list_push_default(child);
        child->unk74 = 0xFF;
        {
            s32 bit6 = body->style & 0x40;
            s32 merged = (child->style & ~0x40) | bit6;
            s32 bit5 = body->style & 0x20;

            child->style = (merged & ~0x20) | bit5;
        }
        child->unk9C = body->unk9C;
        child->unkA0 = body->unkA0;
        child->unkA4 = body->unkA4;
        child->unkA8 = body->unkA8;
        child->unk98 = *flags98;
        if ((s32)body->unk9C < 0)
        {
            child->unk60 = (BodyFunc)try_lookup_slot;
            child->unk78 = (child->unk78 & 0x1F) | 0x20;
            set_slot_value(0x85, -1);
        }
        else
        {
            child->unk58 = (BodyFunc)finish_and_remove;
            child->unk5C = (BodyFunc)finish_and_remove;
            child->unk60 = (BodyFunc)try_lookup_slot;
            child->unk78 = (child->unk78 & 0x1F) | 0x20;
            set_slot_value(0x85, -1);
        }
        child_jump_to_pose(child);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0809A420/child_jump_to_pose.s", void child_jump_to_pose(struct HeroBody *body))

void apply_velocity_step(struct HeroBody *body)
{
    body->unkB2 += body->unk94;
    body->x += body->unkAE;
    body->y += body->unkB0;
    body->z += (s16)body->unkB2;
    if (body->unk9C < 0)
        body->sprite->unkC += 0xA00;
    if (body->unk8C >= body->z)
    {
        u32 value = body->sprite->unk16 | ((((u32)body->sprite->unk1E << 28) >> 16) + 0x1000);

        if (value == 0x40D7)
            set_slot_value(0x6A, -1);
        else
            set_slot_value(0x6A, -1);
        if (!(body->unk98 & 4))
        {
            body->unk98 |= 4;
            body->x = body->startX;
            body->y = body->startY;
            body->z = body->unk8C;
            body->state = child_jump_to_pose;
        }
        else
            obj_release(body);
    }
}


u32 try_lookup_slot(struct HeroBody *body, u32 arg)
{
    u32 found = lookup_slot(body, arg);

    if (found == 0)
    {
        body->state = move_and_remove_offscreen;
        body->unkAE = body->unkAE / 2;
        body->unkB0 = found;
        body->unkB2 = found;
        obj_list_remove(body);
    }
    return 0;
}


u32 finish_and_remove(struct HeroBody *body)
{
    change_item_count(body->unk9C & 0x7F, 1);
    effect_spawn_at(0xF93, body->targetX / 0x100, body->targetY / 0x100,
                    body->targetZ / 0x100, body);
    set_slot_value(0x152, -1);
    obj_release(body);
    return 0;
}


void update_pose_if_shown_b(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
        on_arrive(body);
}

u32 set_pose_mode_6_or_7(struct HeroBody *body)
{
    body->state = anim2_then_play_flag1;
    body->spriteCallback = on_arrive;
    if (body->partner->data->poseOfs == -1)
        body->unk98 = 6;
    else
        body->unk98 = 7;
    return 1;
}


u32 set_pose_mode_0_or_1(struct HeroBody *body)
{
    u32 value;

    body->state = anim2_then_play_flag1;
    body->spriteCallback = on_arrive;
    value = 1;
    if (body->partner->data->poseOfs == -1)
        body->unk98 = 0;
    else
        body->unk98 = value;
    return value;
}


void move_and_remove_offscreen(struct HeroBody *body)
{
    s32 pos[3];

    body->x = body->targetX + body->unkAE;
    body->z = body->targetZ + (s16)body->unkB2;
    body->unkB2 -= 0x5E;
    pos[0] = body->x / 0x100;
    pos[1] = body->y / 0x100;
    pos[2] = body->z / 0x100;
    offset_three_words_camera(&pos[0], &pos[1], &pos[2], 0);
    if (pos[1] > 0x110)
        obj_release(body);
    else if (pos[0] < -0x10)
        obj_release(body);
}


void anim2_then_play_flag1(struct HeroBody *body)
{
    sprite_set_action(body, 2, 0, 0);
    body->state = play_anim_by_flag1;
}

void play_anim_by_flag1(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        if (!(body->unk98 & 1))
            sprite_set_action(body, 3, 0, 0);
        else
            sprite_set_action(body, 4, 0, 0);
        set_slot_value(0x88, -1);
        body->state = play_anim_by_flag2;
    }
}


void play_anim_by_flag2(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        if (!(body->unk98 & 2))
            sprite_set_action(body, 5, 0, 0);
        else
            sprite_set_action(body, 7, 0, 0);
        body->state = spawn_child_pose;
    }
}


NONMATCH("asm/code_0809A420/spawn_two_children.s", void spawn_two_children(struct HeroBody *body))
{
    BodyFunc init;
    struct HeroBody *child;
    u32 art;

    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 5, 0x40BE, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 1;
        art = 0x40C0;
        child = obj_spawn_linked(body, art, 0);
        child->flags |= 1;
        child->flags |= 4;
        init = init_random_timer;
        child->state = init;
        init(child);
        child->unk34 = (u8 *)gPoseTable[0];
        child->unk20 = 0;
        child = obj_spawn_linked(body, art, 0);
        child->flags |= 1;
        child->flags |= 4;
        child->state = init;
        init(child);
        child->unk34 = (u8 *)gPoseTable[0x12];
        child->unk20 = 1;
        set_slot_value(0x15D, -1);
        effect_spawn_at(0x2C7D, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
        body->state = random_timers_init;
    }
}
END_NONMATCH

void update_pose_timers(struct HeroBody *body)
{
    struct HeroBody *child;
    struct HeroBody *grand;
    struct PoseTimer *link;
    s32 tb;
    s32 h;

    if (body->child == 0)
    {
        body->state = 0;
        return;
    }
    tb = (u16)body->unkAE - 1;
    body->unkAE = tb;
    if ((tb << 16) < 0)
    {
        set_slot_value(0x15D, -1);
        body->unkAE = rand_below(0x28, mt19937_next()) + 0x1E;
    }
    if (--body->unkA8 < 0)
    {
        effect_spawn_at(0x2C7D, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
        body->unkA8 = rand_below(0xA, mt19937_next()) + 0x32;
    }
    h = 0;
    child = body->child;
    link = (struct PoseTimer *)child->unkA8;
    if (link != 0)
    {
        u16 partnerH = child->partner->sprite->unkE;

        child->sprite->unkE = partnerH + 1;
        link->unkE = partnerH - 1;
        link->unk0 = child->sprite->unk0;
        link->unk2 = child->sprite->unk2;
        h = 1;
    }
    grand = body->child->child;
    if (grand != 0 && grand->unkA8 != 0)
    {
        struct PoseTimer *link2 = (struct PoseTimer *)grand->unkA8;
        u16 gpH = grand->partner->sprite->unkE;

        grand->sprite->unkE = gpH + 1;
        link2->unkE = gpH - 1;
        link2->unk0 = grand->sprite->unk0;
        link2->unk2 = grand->sprite->unk2;
        h |= 2;
    }
    switch (h)
    {
    case 0:
        body->child->sprite->unkE = 0x400;
        if (body->child->child != 0)
            body->child->child->sprite->unkE = 0x400;
        break;
    case 1:
        if (body->child->child != 0)
        {
            if (body->child->y < body->child->child->y)
                body->child->child->sprite->unkE = body->child->sprite->unkE - 3;
            else
                body->child->child->sprite->unkE = body->child->sprite->unkE + 1;
        }
        break;
    case 2:
        if (body->child->child->y < body->child->y)
            body->child->sprite->unkE = body->child->child->sprite->unkE - 3;
        else
            body->child->sprite->unkE = body->child->child->sprite->unkE + 1;
        break;
    }
}


void init_random_timer(struct HeroBody *body)
{
    body->partner = 0;
    body->unkA8 = 0;
    sprite_set_action(body, 0, 0x40C0, 0);
    ((struct ModeByte *)&body->sprite->flags)->mode = 1;
    body->unk1C = rand_below(0, mt19937_next()) + 0x14;
    body->unk1E = 0;
    body->unk74 = 0x7F;
    body->state = hide_and_reset_slot;
}


ASM_FUNC("asm/code_0809A420/seek_hero.s", void seek_hero(struct HeroBody *body))

void fall_and_release(struct HeroBody *body)
{
    struct HeroBody *heroA = &gWorld->heroA->body;
    struct HeroBody *heroB = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;

    body->z += (s16)body->unkB2;
    body->unkB2 -= 0x33;
    if (body->sprite->unk2 >= partner->sprite->unk2 + 0x20)
    {
        obj_detach_active(body->sprite);
        if (gWorld->unkC & 0x80)
        {
            struct HeroBody *other = heroB;

            if (body != heroA)
                other = heroA;
            obj_detach_active(other->sprite);
        }
        obj_release_node((void *)partner->unkA8);
        partner->unkA8 = 0;
        body->partner = (struct HeroBody *)((u8 *)partner->data + 8);
        partner->partner = 0;
        body->state = place_at_focus_y;
    }
}


void place_at_focus_y(struct HeroBody *body)
{
    struct World **pworld = &gWorld;
    struct World *world = *pworld;
    u32 flags;

    if (body->partner->child == 0)
    {
        gUnk_02000052 = 0x808;
        sprite_set_action(body, 0, gActionCodes[(world->unkC >> 7) * 0x12] + ~body->data->poseOfs, 0);
        flags = body->sprite->flags;
        body->sprite->flags = flags | 0x10;
        body->targetZ = body->z = ((s32)(obj_last_y((*pworld)->focus) << 16) >> 8) + body->data->height;
        if (body->data->effect != 0)
        {
            place_targets(body, body->targetX / 0x100, body->targetY / 0x100, 0, -1);
            set_motion_speed_aux(body, 0, 0x66);
        }
        body->state = update_pair_link;
    }
}


void update_pair_link(struct HeroBody *body)
{
    struct HeroBody *partner = body->partner;

    step_motion_scaled(body);
    if (body->flags & 0x20)
    {
        partner_link_update(partner, body, 0xFF);
        body->state = clear_state;
    }
}


void draw_if_visible(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
        obj_release(body);
}

void follow_focus_then_pose(struct HeroBody *body)
{
    struct World *world = gWorld;
    u8 *pick = body->unk34;

    if (body->data->effect != 0)
    {
        u8 offX = pick[1];
        u8 offY = pick[2];
        s32 x = (s16)obj_last_x(world->focus) + offX;
        s32 y = (s16)obj_last_y(focus_obj()) + offY;

        place_targets(body, x, y, 0, -1);
        set_motion_start(body, 0);
    }
    set_motion_start(body, pick[0] << 2);
    body->state = seek_hero;
}


void hide_and_reset_slot(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 1, 0x40C0, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 0;
        set_slot_value(0x8C, -1);
        gUnk_0200004A = 0xC0FF;
        body->state = follow_focus_then_pose;
    }
}


void random_timers_init(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 0, 0x40BE, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 0;
        body->unkAE = rand_below(0x28, mt19937_next()) + 0x1E;
        body->unkA8 = rand_below(0xA, mt19937_next()) + 0x32;
        body->state = update_pose_timers;
    }
}


u32 part_step_start_anim_mode2(struct HeroBody *body)
{
    body->subState = 0;
    body->state = start_anim_mode2;
    body->unk7C = 0x100;
    return 1;
}

void clear_state(struct HeroBody *body)
{
    body->state = 0;
}

void start_anim_mode2(struct HeroBody *body)
{
    sprite_set_action(body, 2, 0x40BE, 0);
    ((struct ModeByte *)&body->sprite->flags)->mode = 1;
    body->state = start_anim_mode3;
}


void start_anim_mode3(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 3, 0x40BE, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 1;
        body->state = start_anim4_fx;
    }
}


void start_anim4_fx(struct HeroBody *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 4, 0x40BE, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 1;
        effect_spawn_at(0x11B8, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
        body->state = spawn_two_children;
    }
}


NONMATCH("asm/code_0809A420/start_anim_by_hero.s", void start_anim_by_hero(struct HeroBody *body))
{
    struct HeroBody *heroA = &gWorld->heroA->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;
    s32 z;

    if (body->unk9C != 0)
    {
        if (partner == heroA)
            sprite_set_action(body, 2, 0x40BF, 0);
        else
            sprite_set_action(body, 5, 0x40BF, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 1;
    }
    x = body->x;
    if (x < 0)
        x += 0xFF;
    x >>= 8;
    y = body->y;
    if (y < 0)
        y += 0xFF;
    y >>= 8;
    z = body->z;
    if (z < 0)
        z += 0xFF;
    z >>= 8;
    effect_spawn_at(0x11E6, x, y, z, body);
    body->state = follow_hero_anim;
}
END_NONMATCH


NONMATCH("asm/code_0809A420/start_anim_by_partner.s", void start_anim_by_partner(struct HeroBody *body))
{
    struct HeroBody *heroA = &gWorld->heroA->body;
    struct HeroBody *partner = body->partner;

    if (body->sprite->flags & 8)
    {
        if (partner == heroA)
            sprite_set_action(body, 4, 0x40BF, 0);
        else
            sprite_set_action(body, 7, 0x40BF, 0);
        ((struct ModeByte *)&body->sprite->flags)->mode = 1;
        if (body->data->effect != 0)
        {
            body->unk7A = 0x4000;
            body->unk7E = 0;
            ((struct BodyFlagBits *)&body->flags)->bit5 = 0;
        }
        body->state = fx_then_stop;
    }
}
END_NONMATCH


void fx_then_stop(struct HeroBody *body)
{
    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        effect_spawn_at(0x11F0, body->x / 0x100, body->y / 0x100, body->z / 0x100, body);
        if (body->data->effect != 0)
        {
            body->unk7A = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x28);
            body->unk7E = 0;
            ((struct BodyFlagBits *)&body->flags)->bit5 = 0;
        }
        body->state = spawn_child_anim;
    }
}


void choose_hero_target(struct HeroBody *body)
{
    u8 unused[4];
    struct HeroBody *heroA = &gWorld->heroA->body;
    struct HeroBody *heroB = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body);
    if (body->sprite->unk21 != 0 && (body->sprite->flags & 8))
        sprite_set_action(body, 0, 0, 0);
    if (body->flags & 0x20)
    {
        if (partner == heroA && ((struct ModeByte *)&heroB->unk76)->mode == 1)
            partner = heroB;
        else if (partner == heroB && ((struct ModeByte *)&heroA->unk76)->mode == 1)
            partner = heroA;
        else
            partner = 0;
        if (partner != 0)
        {
            struct HeroBody *child;
            BodyFunc fn;

            body->partner = partner;
            child = obj_spawn_linked(body, 0x40C2, (u32)partner);
            fn = rise_stop;
            child->state = fn;
            fn(child);
        }
        body->state = wait_child_gone;
    }
}


void rise_stop(struct HeroBody *body)
{
    body->y += 0x100;
    body->z = 0x2600;
    if (body->data->effect != 0)
    {
        body->unk7A = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x18);
        body->unk7E = 0;
        ((struct BodyFlagBits *)&body->flags)->bit5 = 0;
    }
    body->state = start_anim1;
}


void wait_child_gone(struct HeroBody *body)
{
    if (body->sprite->unk21 != 0)
    {
        if ((body->sprite->flags & 8) == 0)
            return;
        sprite_set_action(body, 0, 0, 0);
    }
    if (body->child == 0)
        body->state = 0;
}
