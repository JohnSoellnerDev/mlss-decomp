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
    u8 filler13[0x1F - 0x13];
    u8 bits1F;
    u8 frame;
    u8 action;
};

struct Owner;

struct DisplayEntry
{
    u8 filler0[4];
    u32 unk4;
    u8 filler8[4];
    struct Owner *owner;
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
    u8 filler28[0x30 - 0x28];
    u8 level;
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

struct TaskSprite
{
    u8 filler0[8];
    struct Sprite *sprite8;
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
    s32 pos10; /* x position, 8.8 fixed point */
};

struct World
{
    u8 filler0[0x48];
    void *taskList48;
    u8 filler4C[0x7C - 0x4C];
    u32 unk7C;
    u8 filler80[0x24C - 0x80];
    void *focus24C;                  /* the camera: offset_three_words makes positions screen-relative to it */
    void *mapSrc250;
    u8 filler254[0x278 - 0x254];
    struct Owner *obj278;            /* the stat row cursor */
    u8 filler27C[0x342 - 0x27C];
    u8 pose342;                      /* stat row picked for the level-up bonus (0-5) */
    u8 filler343[0x368 - 0x343];
    struct Widget *menuObj;
    struct Widget *obj36C;
    struct Widget *digitObj;
    u8 filler374[0x37C - 0x374];
    struct Scroll *scrollObj37C;     /* the up/down scroll arrows beside the stat rows */
    struct DisplayEntry *entry380[6];
};

extern struct World *gWorld;
extern u16 gSys[];
extern u8 *gParty;
extern u8 *gWorkBuf;

#define PARTY_HERO_SIZE 0x3C        /* one hero's record in gParty: Mario's at 0, Luigi's at 0x3C */
#define SPRITE_FLAG_ANIM_DONE 0x08  /* Sprite.flags: the current animation has finished */

/* gWorkBuf + LEVELUP_STATS / LEVELUP_GAINS: u16 per stat row (6), the stat after the level-up and its gain */
#define LEVELUP_STATS 0x8E1C
#define LEVELUP_GAINS 0x8E28

/* gWorkBuf + HERO_PANEL_DIGITS: the 10 bonus digits the hero panel's roulette cycles through */
#define HERO_PANEL_DIGITS 0x8E34
#define HERO_PANEL_DIGIT_COUNT 10

/* Owner->timer of the digit roulette: frame within the current digit (low 11 bits), digit index above */
#define ROULETTE_FRAME_MASK 0x7FF
#define ROULETTE_DIGIT_SHIFT 11

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
u32 to_bcd(s32 value);
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
extern const u8 gStatTable[];
void mem_copy_fast(void *dst, const void *src, u32 n);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);

void state_tally_hero_stats(struct Owner *owner);
struct SpriteFlags12
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 lo4 : 1;
    u8 bit4 : 1;
    u8 hi : 3;
};

static inline void fig_sprite_action_4(struct Figure *fig, u32 action)
{
    sprite_set_action((struct Owner *)((u8 *)fig + 8), 4, action, 0);
}

NONMATCH("asm/code_081041E4/state_figure_land.s", void state_figure_land(struct Owner *owner))
{
    struct Figure *fig;
    struct Sprite *disp;
    struct Task *task;
    s32 langOfs = 0x8C0;
    u32 labelId;
    u8 lang;
    u8 pointsRaw;
    s32 points;

    fig = owner->fig;
    fig->stepZ += fig->accel;
    fig->x += fig->stepX;
    fig->y += fig->stepY;
    fig->z += (s16)fig->stepZ;
    if ((s16)fig->stepZ <= 0 && fig->targetZ >= fig->z)
    {
        fig->x = fig->targetX;
        fig->y = fig->targetY;
        fig->z = fig->targetZ;
        if (fig != 0 && (fig->ctrl & 6) == 2)
        {
            if (fig->anim == -1)
                fig_sprite_action_4(fig, 0x2003);
            else
                fig_sprite_action_4(fig, 0x2050);
            ((struct SpriteFlags12 *)&fig->link->flags)->bit4 = 0;
            ((struct SpriteFlags12 *)&fig->link->flags)->mode = 0;
        }
        ((struct SpriteFlags12 *)&fig->link->flags)->mode = 1;
        disp = spawn_obj_by_id(0x7003, fig->anim != -1, 0, 1);
        gWorld->menuObj = (struct Widget *)disp;
        sprite_entry_update(disp);
        disp->x = 0x78;
        disp->y = 0x1C;
        lang = ((u8 *)gSys)[langOfs];
        if (lang <= 4)
        {
            switch (lang)
            {
            case 0:
                labelId = 0x7004;
                break;
            case 1:
                labelId = 0x7005;
                break;
            case 2:
                labelId = 0x7006;
                break;
            case 3:
                labelId = 0x7007;
                break;
            case 4:
                labelId = 0x7008;
                break;
            }
        }
        disp = spawn_obj_by_id((u16)labelId, 0, 0, 1);
        gWorld->obj36C = (struct Widget *)disp;
        sprite_entry_update(disp);
        disp->x = 0x78;
        disp->y = 0x40;
        ((struct SpriteFlags12 *)&disp->flags)->bit4 = 1;
        pointsRaw = *(u8 *)&fig->points;
        points = ((s32)pointsRaw << 24) >> 24;
        task = list_add_task((void *)&gWorld->taskList48, task_countdown_digits);
        task->unk12 = 0x78;
        if (points <= 9)
            task->unk12 = 0x74;
        task->unk14 = 0x3A;
        task->unk16 = to_bcd(points);
        task->timer = 0;
        gWorld->digitObj = (struct Widget *)task;
        owner->state = state_tally_hero_stats;
    }
}
END_NONMATCH



ASM_FUNC("asm/code_081041E4/state_tally_hero_stats.s", void state_tally_hero_stats(struct Owner *owner))

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
        world = gWorld;
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

/*
 * State: slides the scroll arrows and the stat row cursor in from the right, a pixel a frame, until
 * they are at screen x 0xD0. Then puts both exactly there, starts the cursor's action 4 on row 0
 * and hands over to state_obj_anim_5_when_active.
 */
void state_scroll_entry_to_focus(struct Owner *owner)
{
    struct World *world = gWorld;
    struct Scroll *arrows = world->scrollObj37C;
    struct Owner *cursor = world->obj278;
    s32 x;
    s32 screenX;
    s32 screenY;
    s32 screenZ;
    s32 stopX;

    x = arrows->pos10 - 0x100;
    arrows->pos10 = x;
    ((struct Scroll *)cursor)->pos10 = x;
    x = arrows->pos10;
    if (x < 0)
    {
        x += 0xFF;
    }
    screenX = x >> 8;
    screenY = 0;
    screenZ = 0;
    offset_three_words(world->focus24C, &screenX, &screenY, &screenZ, 0);
    if (screenX <= 0xD0)
    {
        /* the camera's x plus 0xD0, in 8.8 fixed point */
        stopX = (s32)(obj_last_x(gWorld->focus24C) << 16) >> 8;
        stopX += 0xD000;
        arrows->pos10 = stopX;
        ((struct Scroll *)cursor)->pos10 = stopX;
        sprite_set_action(cursor, 4, 0, 0);
        gWorld->pose342 = 0;
        owner->state = state_obj_anim_5_when_active;
    }
}

/*
 * State: picking the stat row for the level-up bonus (gWorld->pose342, rows 0-5). Up/down (with
 * key repeat) move the cursor one row, wrapping round; the scroll arrows animate while up/down
 * are held. A confirms the row: the cursor plays action 6 and state_show_hero_panel starts the
 * bonus roulette.
 */
void state_select_pose(struct Owner *owner)
{
    struct Owner *cursor = gWorld->obj278;
    struct Scroll *arrows = gWorld->scrollObj37C;
    s32 row = gWorld->pose342 & 0x7F;
    u8 *sys;
    u16 *repeatKeys;

    /* gSys + 0x3E: keys pressed this frame, + 0x40: released, + 0x42: pressed or repeating */
    sys = (u8 *)gSys;
    repeatKeys = (u16 *)(sys + 0x42);
    if (*repeatKeys & DPAD_UP)
    {
        set_slot_value(0x5F, -1);
        row--;
    }
    if (*repeatKeys & DPAD_DOWN)
    {
        set_slot_value(0x5F, -1);
        row++;
    }
    if (row < 0)
    {
        s32 x;
        s32 y;

        row = 5;
        x = cursor->baseX / 0x100;
        y = cursor->baseY / 0x100;
        set_marker_position(x, y, 0x20 - (s16)obj_last_y(gWorld->focus24C));
    }
    if (row > 5)
    {
        s32 x;
        s32 y;

        row = 0;
        x = cursor->baseX / 0x100;
        y = cursor->baseY / 0x100;
        set_marker_position(x, y, 0x70 - (s16)obj_last_y(gWorld->focus24C));
    }
    if (row != gWorld->pose342)
    {
        s32 z;
        s32 x;
        s32 y;
        s32 cameraY;
        s32 rowY;

        /* the cursor slides to the new row, 16 pixels per row from y 0x30 */
        gWorld->pose342 = row;
        cameraY = (s16)obj_last_y(gWorld->focus24C);
        rowY = row << 4;
        rowY += 0x30;
        z = 0xA0 - (cameraY + rowY);
        x = cursor->baseX / 0x100;
        y = cursor->baseY / 0x100;
        place_world_object(x, y, z, 5, 0x7001);
    }
    if (gSys[0x1F] & DPAD_UP)
        sprite_set_action((struct Owner *)arrows, 1, 0, 0);
    else if (gSys[0x1F] & DPAD_DOWN)
        sprite_set_action((struct Owner *)arrows, 2, 0, 0);
    else if (gSys[0x20] & (DPAD_UP | DPAD_DOWN))
        sprite_set_action((struct Owner *)arrows, 0, 0, 0);
    if (gSys[0x1F] & A_BUTTON)
    {
        if (arrows->sprite8->action != 0)
            sprite_set_action((struct Owner *)arrows, 0, 0, 0);
        sprite_set_action(cursor, 6, 0, 0);
        set_slot_value(0x60, -1);
        owner->state = state_show_hero_panel;
    }
}

struct HeroPanelStack
{
    s32 table[0x50];
    s32 stats[3];
    s32 ctx;
    s32 valueA;
    s32 scale;
    double fRow;
    double fScale;
    s32 rowOfs;
    s32 countSave;
};

static inline u32 panel_digit_store(u8 *cell, s32 word, s32 scale, s32 ctx)
{
    s32 value = scale * word;
    s32 limit;
    u32 changed = 0;

    if (value < 0)
        value += 0x3FF;
    *cell = (value >> 10) + 1;
    limit = ctx / 10 + 3;
    if ((s32)*cell > limit || *cell > 5)
    {
        changed = 1;
        if ((s32)*cell > limit)
            *cell = limit;
        if (*cell > 5)
            *cell = 5;
    }
    return changed;
}

NONMATCH("asm/code_081041E4/hero_panel_digits.s", void hero_panel_digits(u8 *digits, u32 heroCtx, u8 *tblA, u8 *tblB, u32 pose, s32 hero))
{
    struct HeroPanelStack ps;
    s32 classScale;
    s32 row;
    s32 adjusted;
    s32 tier;
    s32 pick1;
    s32 pick2;
    s32 i;
    s32 acc;
    s32 valueScaled;
    s32 ctxScaled;
    s32 quotient;
    s32 product;
    s32 *cur;
    s32 *w;
    s32 half;
    s32 limit;
    s32 count;
    u8 *cell;

    ps.ctx = heroCtx;
    ps.valueA = ((u16 *)tblA)[pose];
    row = ((u16 *)tblB)[pose];
    adjusted = 0;
    mem_copy_fast(ps.table, gStatTable, 0x140);

    if (hero == 0)
    {
        if (pose <= 5)
        {
            switch (pose)
            {
            case 0:
                classScale = 0x14C;
                ps.scale = 0x133;
                break;
            case 1:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 2:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 3:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 4:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 5:
                classScale = 0x14C;
                ps.scale = 0x14C;
                break;
            }
        }
    }
    else
    {
        if (pose <= 5)
        {
            switch (pose)
            {
            case 0:
                classScale = 0x119;
                ps.scale = 0x133;
                break;
            case 1:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 2:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 3:
                classScale = 0x119;
                ps.scale = 0x119;
                break;
            case 4:
                classScale = 0x133;
                ps.scale = 0x133;
                break;
            case 5:
                classScale = 0x14C;
                ps.scale = 0x14C;
                break;
            }
        }
    }

    ps.fRow = (double)row;
    acc = (s32)(ps.fRow * 0.75 * 256.0);
    ps.fScale = (double)(classScale * (ps.ctx + row));
    ps.stats[0] = acc + (s32)(ps.fScale * 0.25);
    ps.stats[1] = (s32)(ps.fRow * 0.5 * 256.0) + (s32)(ps.fScale * 0.5);
    ps.stats[2] = (s32)(ps.fRow * 0.25 * 256.0) + (s32)(ps.fScale * 0.75);

    tier = 0;
    valueScaled = ps.valueA << 8;
    ctxScaled = ps.ctx << 8;
    if (valueScaled >= ps.stats[0])
    {
        cur = &ps.stats[0];
        for (;;)
        {
            cur++;
            tier++;
            if (tier > 2)
                break;
            if (valueScaled < *cur)
                break;
        }
    }

    quotient = ctxScaled / 5;
    if (quotient <= 0xFF)
        quotient = 0x100;
    product = ps.scale * quotient;
    if (product < 0)
        product += 0xFF;
    ps.scale = product >> 8;
    if (tier == 1)
        ps.scale = (((ps.scale - 0x100) * 2) / 3) + 0x100;
    if (tier == 2)
        ps.scale = ((ps.scale - 0x100) / 3) + 0x100;
    if (tier == 3)
        ps.scale = 0x100;

    pick1 = rand_below(4, mt19937_next()) * 5;
    w = ps.table + tier * 20 + pick1;
    for (i = 0; i < 5; i++)
        adjusted += panel_digit_store(digits + i, w[i], ps.scale, ps.ctx);

    pick2 = rand_below(4, mt19937_next()) * 5;
    if (pick2 == pick1)
        pick2 = rand_below(4, mt19937_next()) * 5;
    w = ps.table + tier * 20 + pick2;
    for (i = 0; i < 5; i++)
        adjusted += panel_digit_store(digits + 5 + i, w[i], ps.scale, ps.ctx);

    if (adjusted > 3)
    {
        half = adjusted / 2;
        if (half > 0)
        {
            limit = ps.ctx / 10 + 3;
            do
            {
                count = 0;
                do
                {
                    count++;
                    cell = digits + rand_below(10, mt19937_next());
                    if (*cell == 5 || *cell == limit)
                    {
                        *cell >>= 1;
                        count = 100;
                    }
                } while (count <= 31);
                half--;
            } while (half > 0);
        }
    }
}
END_NONMATCH

/*
 * Level-up state: once the stat cursor's confirm animation has finished, rolls the 10 bonus digits
 * for the chosen stat row (from the hero's level and that row's new stat and gain), plays the
 * cursor's action 7, starts the roulette sound 0x123 and lets state_digit_input_wait spin the
 * roulette.
 */
void state_show_hero_panel(struct Owner *owner)
{
    struct World *world = gWorld;
    struct Owner *cursor = world->obj278;

    if (cursor->sprite->flags & SPRITE_FLAG_ANIM_DONE)
    {
        s32 heroIdx = owner->posX;
        struct Hero *hero = (struct Hero *)(gParty + heroIdx * PARTY_HERO_SIZE);
        u8 *workBuf = gWorkBuf;
        u8 *bonusDigits = workBuf + HERO_PANEL_DIGITS;
        u8 level = hero->level;

        hero_panel_digits(bonusDigits, level, workBuf + LEVELUP_STATS, workBuf + LEVELUP_GAINS,
                          world->pose342, heroIdx);
        sprite_set_action(cursor, 7, 0, 0);
        owner->timer = 0;
        owner->state = state_digit_input_wait;
        set_slot_value(0x123, -1);
    }
}

/*
 * One frame of the hero panel's bonus roulette: each of the 10 digits stays for
 * framesPerDigit frames while it rolls in over the previous one. owner->timer packs
 * the frame and the digit index; owner->index holds the current digit as BCD.
 * Returns the new packed timer.
 */
u32 draw_digit_cycle(struct Owner *owner, s32 framesPerDigit)
{
    u32 frame;
    s32 digit;
    u32 prevBcd;
    u32 angle;
    u32 rowY;
    u8 *digits;

    frame = owner->timer & ROULETTE_FRAME_MASK;
    digit = owner->timer >> ROULETTE_DIGIT_SHIFT;
    if (frame == 0)
    {
        digit++;
        if (digit == HERO_PANEL_DIGIT_COUNT)
        {
            digit = 0;
        }
        digits = gWorkBuf + HERO_PANEL_DIGITS;
        owner->index = to_bcd(digits[digit]);
    }
    frame++;
    if (frame == framesPerDigit)
    {
        frame = 0;
    }
    if (digit != 0)
    {
        prevBcd = to_bcd(*(gWorkBuf + digit + (HERO_PANEL_DIGITS - 1)));
    }
    else
    {
        prevBcd = to_bcd(*(gWorkBuf + (HERO_PANEL_DIGITS + HERO_PANEL_DIGIT_COUNT - 1)));
    }
    rowY = gWorld->pose342 * 0x10 + 0x28;
    /* roll progress: 0..63 is a quarter turn of the sine table */
    angle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(frame << 6, framesPerDigit);
    draw_number_rotated(owner->index, prevBcd, number_right_edge(owner->index, 0xB8), rowY, angle);
    return frame | digit << ROULETTE_DIGIT_SHIFT;
}

void state_digit_input_wait(struct Owner *owner)
{
    struct Owner *marker;

    owner->timer = draw_digit_cycle(owner, 3);
    marker = gWorld->obj278;
    if (gSys[0x1F] & 1)
    {
        owner->posY = 1;
        set_slot_value(0x60, -1);
        owner->state = state_fx_step_counter;
    }
    else if (gSys[0x1F] & 2)
    {
        sprite_set_action(marker, 6, 0, 0xB);
        marker->sprite->frame = 0xF0;
        set_slot_value(0x61, -1);
        owner->state = state_obj_anim_5_when_active;
        sound_stop_song(0x123);
    }
}

void state_draw_value_step(struct Owner *owner)
{
    s32 rowY = ((gWorld->pose342 << 1) + 5) << 3;
    s32 value = owner->index;
    s32 rightEdge = number_right_edge(value, 0xB8);
    struct Sprite *obj;

    draw_digits_c0_p11(value, rightEdge, rowY, 0);
    owner->timer++;
    if (owner->timer > 9)
    {
        obj = spawn_obj_by_id(0x7003, 8, 0, 1);
        sprite_entry_update(obj);
        obj->x = 0xB8;
        obj->y = rowY + 0xA;
        obj->unkE = 0;
        owner->sprite = obj;
        owner->timer = 0;
        owner->state = state_pose_draw_free;
        sound_stop_song(0x123);
    }
}

static inline void hero_add_points(u16 *counter, u16 points)
{
    s32 sum = points + *counter;

    *counter = sum;
    if ((u16)sum > 0x3E7)
        *counter = 0x3E7;
}

NONMATCH("asm/code_081041E4/state_add_points_to_hero.s", void state_add_points_to_hero(struct Owner *owner))
{
    u8 pose = gWorld->pose342;
    s32 poseX2 = pose << 1;
    s32 rowSlot = (poseX2 + 5) << 3;
    s32 value = owner->index;
    s32 rightEdge = number_right_edge(value, 0xB8);
    s32 timer;

    draw_digits_c0_p11(value, rightEdge, rowSlot, 0);
    timer = (u16)owner->timer + 1;
    owner->timer = timer;
    if ((s16)timer > 9)
    {
        u16 packed = owner->index;
        struct DisplayEntry *entry;
        struct Hero *hero;
        u16 stat;
        u8 *effect;
        s32 valueNow;

        owner->index = (packed & 0xF) + ((s16)packed >> 4) * 10;
        gWorld->entry380[pose]->unk4 = 0;
        valueNow = owner->index;
        effect = (u8 *)gWorkBuf + 0x8E1C;
        stat = *(u16 *)(effect + poseX2);
        entry = create_display_entry(owner, valueNow, stat, 0x999, 0, 0, 0x20, 0xC,
                                     (owner->index <= 9) ? 0xBC : 0xC0, rowSlot, 0xA8,
                                     (pose << 4) + 0x2A, 0xA8);
        gWorld->entry380[pose] = entry;
        hero = (struct Hero *)(gParty + owner->posX * 0x3C);
        switch (gWorld->pose342)
        {
        case 0:
            hero_add_points(&hero->c10, owner->index);
            break;
        case 1:
            hero_add_points(&hero->c16, owner->index);
            break;
        case 2:
            hero_add_points(&hero->c1A, owner->index);
            break;
        case 3:
            hero_add_points(&hero->c22, owner->index);
            break;
        case 4:
            hero_add_points(&hero->c1E, owner->index);
            break;
        case 5:
            hero_add_points(&hero->c26, owner->index);
            break;
        }
        owner->timer = 1;
        owner->state = state_entry_countdown;
    }
}
END_NONMATCH

static inline void fig_sprite_action(struct Figure *fig, u32 action)
{
    sprite_set_action((struct Owner *)((u8 *)fig + 8), 6, action, 0);
}

/* Owner state after the points are tallied: counts owner->timer up to 0x2C, starting song 0x79
 * (set_slot_value queues a song at a volume) at 0x20. Then stops the song, puts the figure (if
 * it is idle) back into its standing action, makes its sprite visible again and waits for the
 * object to go active (state_wait_obj_active). */
void state_entry_countdown(struct Owner *owner)
{
    struct Figure *fig;

    if (owner->timer <= 0x2B)
    {
        if (owner->timer == 0x20)
            set_slot_value(0x79, -1);
        owner->timer++;
    }
    else
    {
        sound_stop_song(0x79);
        fig = owner->fig;
        if (fig != NULL && (fig->ctrl & 6) == 2)
        {
            if (fig->anim == -1)
                fig_sprite_action(fig, 0x2003);
            else
                fig_sprite_action(fig, 0x2050);
            ((struct SpriteFlags12 *)&fig->link->flags)->bit4 = 0;
            ((struct SpriteFlags12 *)&fig->link->flags)->mode = 0;
        }
        ((struct SpriteFlags12 *)&fig->link->flags)->mode = 1;
        owner->state = state_wait_obj_active;
    }
}
