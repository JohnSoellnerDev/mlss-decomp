#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    s16 x;
    s16 y;
    u8 filler4[0xE - 0x4];
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
    u8 action;
};

struct DisplayEntry
{
    u8 filler0[4];
    u32 unk4;
};

struct Hero
{
    u8 filler0[0x10];
    u16 c10;
    u8 filler12[0x16 - 0x12];
    u16 c16;
    u8 filler18[0x1A - 0x18];
    u16 c1A;
    u8 filler1C[0x1E - 0x1C];
    u16 c1E;
    u8 filler20[0x22 - 0x20];
    u16 c22;
    u8 filler24[0x26 - 0x24];
    u16 c26;
};

struct LevelRec
{
    u8 inc[6];
    u16 lo6;
    u8 hi8;
    u8 filler9[3];
};

struct Task
{
    u8 filler0[0x10];
    u16 timer;
    u16 unk12;
    u16 unk14;
    u16 unk16;
};

struct Widget
{
    u16 unk0;
    u8 filler2[0x12 - 0x2];
    u16 unk12;
};

struct Figure
{
    u8 filler0[0x10];
    struct Sprite *link;
    u8 filler14[0x18 - 0x14];
    s32 x;
    s32 y;
    s32 z;
    u8 filler24[0x7E - 0x24];
    u8 ctrl;
    u8 filler7F[0x8C - 0x7F];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler98[0xA4 - 0x98];
    s32 accel;
    u8 fillerA8[0xB6 - 0xA8];
    s16 stepX;
    s16 stepY;
    u16 stepZ;
    u8 fillerBC[0xEC - 0xBC];
    s16 anim;
    u8 fillerEE[0xF5 - 0xEE];
    s8 points;
};

struct Owner
{
    u32 unk0;
    void (*state)(struct Owner *);
    struct Sprite *sprite;
    struct Figure *fig;
    s16 timer;
    s16 posX;
    s16 posY;
    s16 index;
    s16 scrollStep;
    u8 filler1A[0x38 - 0x1A];
    s32 baseX;
    s32 baseY;
    u8 filler40[0x6C - 0x40];
    void *block;
};

struct Scroll
{
    u8 filler0[0x8];
    struct Sprite *sprite8;
    u8 filler0C[0x10 - 0xC];
    s32 pos10;
};

struct World
{
    u8 filler0[0x48];
    void *taskList48;
    u8 filler4C[0x24C - 0x4C];
    void *focus24C;
    u8 filler250[0x278 - 0x250];
    struct Owner *obj278;
    u8 filler27C[0x342 - 0x27C];
    u8 pose342;
    u8 filler343[0x368 - 0x343];
    struct Widget *menuObj;
    struct Widget *obj36C;
    struct Widget *digitObj;
    u8 filler374[0x37C - 0x374];
    struct Scroll *scrollObj37C;
    struct DisplayEntry *entry380[6];
};

extern struct World *gUnk_03000FD8;
extern u16 gUnk_0300034C[];
extern u8 *gUnk_03000FF4;
extern u8 *gUnk_03000FDC;

void state_create_result_entries(struct Owner *owner);
void state_fx_step_counter(struct Owner *owner);
void state_digit_input_wait(struct Owner *owner);
void state_wait_obj_active(struct Owner *owner);
void state_show_hero_panel(struct Owner *owner);
void state_sprite_park(struct Owner *owner);
void state_draw_pose_digits(struct Owner *owner);
void state_slide_back(struct Owner *owner);
void state_entry_countdown(struct Owner *owner);
void task_countdown_digits(struct Owner *owner);
struct Task *list_add_task(void *list, void (*state)(struct Owner *));
struct Owner *task_create_list_4c(void (*state)(struct Owner *), u32 arg);
extern const u8 gLevelExpTable[];
void offset_three_words(void *a, s32 *x, s32 *y, s32 *z, u32 w);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void set_marker_position(s32 x, s32 y, s32 z);
void place_world_object(s32 x, s32 y, s32 z, s32 w, u32 e);
void hero_panel_digits(u8 *digits, u32 heroCtx, u8 *tblA, u8 *tblB, u32 pose, s32 hero);
u32 to_bcd(u8 value);
struct DisplayEntry *create_display_entry(struct Owner *owner, s32 value, u32 stat, u32 c, s32 s0, s32 s1, s32 s2, s32 s3, s32 s4, s32 s5, s32 s6, s32 s7, s32 s8);
void draw_number_rotated(s32 value, u32 digit, s32 rightEdge, s32 rowY, u32 scaled);
void state_obj_anim_5_when_active(struct Owner *owner);
void sprite_set_action(struct Owner *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 slot, s32 value);
void sound_stop_song(u32 song);
u32 draw_digit_cycle(struct Owner *owner, s32 n);
void state_pose_draw_free(struct Owner *owner);
s32 number_right_edge(s32 a, s32 b);
void draw_digits_c0_p11(s32 a, s32 b, s32 c, s32 d);
struct Sprite *spawn_obj_by_id(s32 id, u32 a, u32 b, u32 c);
void sprite_entry_update(void *obj);

ASM_FUNC("asm/code_081041E4/state_figure_land.s", void state_figure_land(void))


ASM_FUNC("asm/code_081041E4/state_tally_hero_stats.s", void state_tally_hero_stats(void))

void state_slide_back(struct Owner *owner)
{
    u16 timer = owner->timer;
    s16 signedTimer = owner->timer;

    if (signedTimer > 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct World *world;

        owner->timer = 0x40;
        owner->fig->x += -0x300;
        world = gUnk_03000FD8;
        world->menuObj->unk0 -= 3;
        world->digitObj->unk12 -= 3;
        world->obj36C->unk0 -= 3;
        owner->timer -= 3;
        owner->state = state_create_result_entries;
    }
}

ASM_FUNC("asm/code_081041E4/state_create_result_entries.s", void state_create_result_entries(struct Owner *owner))

ASM_FUNC("asm/code_081041E4/task_draw_sprite_row.s", void task_draw_sprite_row(void))

ASM_FUNC("asm/code_081041E4/state_hero_entry_timeline.s", void state_hero_entry_timeline(void))

void state_scroll_entry_to_focus(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;
    struct Scroll *scroll = world->scrollObj37C;
    struct Owner *marker = world->obj278;
    s32 y;
    s32 ax;
    s32 ay;
    s32 az;
    s32 markerX;

    y = scroll->pos10 - 0x100;
    scroll->pos10 = y;
    ((struct Scroll *)marker)->pos10 = y;
    y = scroll->pos10;
    if (y < 0)
    {
        y += 0xFF;
    }
    ax = y >> 8;
    ay = 0;
    az = 0;
    offset_three_words(world->focus24C, &ax, &ay, &az, 0);
    if (ax <= 0xD0)
    {
        markerX = (s32)(obj_last_x(gUnk_03000FD8->focus24C) << 16) >> 8;
        markerX += 0xD000;
        scroll->pos10 = markerX;
        ((struct Scroll *)marker)->pos10 = markerX;
        sprite_set_action(marker, 4, 0, 0);
        gUnk_03000FD8->pose342 = 0;
        owner->state = state_obj_anim_5_when_active;
    }
}

ASM_FUNC("asm/code_081041E4/state_select_pose.s", void state_select_pose(void))
ASM_FUNC("asm/code_081041E4/hero_panel_digits.s", void hero_panel_digits(u8 *digits, u32 heroCtx, u8 *tblA, u8 *tblB, u32 pose, s32 hero))

void state_show_hero_panel(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;
    struct Owner *marker = world->obj278;

    if (marker->sprite->flags & 8)
    {
        s32 heroIdx = owner->posX;
        u8 *hero = gUnk_03000FF4 + heroIdx * 0x3C;
        u8 *effect = gUnk_03000FDC;
        u8 *digits = effect + 0x8E34;
        u8 heroCtx = hero[0x30];

        hero_panel_digits(digits, heroCtx, effect + 0x8E1C, effect + 0x8E28, world->pose342, heroIdx);
        sprite_set_action(marker, 7, 0, 0);
        owner->timer = 0;
        owner->state = state_digit_input_wait;
        set_slot_value(0x123, -1);
    }
}

ASM_FUNC("asm/code_081041E4/draw_digit_cycle.s", u32 draw_digit_cycle(struct Owner *owner, s32 n))

void state_digit_input_wait(struct Owner *owner)
{
    struct Owner *marker;

    owner->timer = draw_digit_cycle(owner, 3);
    marker = gUnk_03000FD8->obj278;
    if (gUnk_0300034C[0x1F] & 1)
    {
        owner->posY = 1;
        set_slot_value(0x60, -1);
        owner->state = state_fx_step_counter;
    }
    else if (gUnk_0300034C[0x1F] & 2)
    {
        sprite_set_action(marker, 6, 0, 0xB);
        marker->sprite->frame = 0xF0;
        set_slot_value(0x61, -1);
        owner->state = state_obj_anim_5_when_active;
        sound_stop_song(0x123);
    }
}

ASM_FUNC("asm/code_081041E4/state_draw_value_step.s", void state_draw_value_step(void))

ASM_FUNC("asm/code_081041E4/state_add_points_to_hero.s", void state_add_points_to_hero(void))

ASM_FUNC("asm/code_081041E4/state_entry_countdown.s", void state_entry_countdown(struct Owner *owner))
