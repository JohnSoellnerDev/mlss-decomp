#include "global.h"
#include "ram_routines.h"

struct RawKeys
{
    u8 filler0[0x28];
    u16 keys;
    u16 keys2A;
};

extern struct RawKeys gSys;

extern const u8 gStackClass[];
extern const u8 gHeapTagStackPart1[];
extern const u8 gHeapTagStackPart2[];
extern const u8 gHeapTagStackPart3[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *obj_init_big(void *obj, u32 first);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void list_insert(void *list, void *node);
void write_var(u16 id, u16 value);
void tween_groups_set_flag_bit(u16 index, u8 value);
void timer_set(void *timer, u32 limit);
void anim_task_start(void *task, u32 a, const void *table, u32 b, u32 c);
extern const u8 gHeapTagAnimTaskData[];

struct ChildPart
{
    u8 filler0[0xC];
    u32 classC;
    u8 *sprite;
    u8 filler14[0x18 - 0x14];
    s32 x;
    s32 y;
    u8 filler20[0x6C - 0x20];
    s32 unk6C;
    s32 unk70; /* obj_step_bounce: vertical speed, added to unk78 each frame */
    s32 unk74;
    s32 unk78; /* obj_step_bounce: height, bounces (speed halved) between 0x5800 and 0x7800 */
    u8 filler7C[0x84 - 0x7C];
    s32 unk84;
    s32 unk88;
    s32 unk8C;
};

struct StageClass
{
    u8 filler0[0x10];
    s16 thisOffset;
    u16 filler12;
    void (*behavior)(void *, u32);
};

struct SpawnAnim
{
    void *unk0;
    void *unk4;
    u8 filler8[0x18 - 0x8];
    s32 unk18;
    s32 unk1C;
};

struct SpawnNode2C
{
    u8 filler0[0x88];
    s32 unk88;
};

struct PuffNode
{
    u8 filler0[0xC];
    u32 classC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 filler24[0x6C - 0x24];
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
};

struct PartA
{
    u8 filler0[0x6C];
    struct ChildPart *bounce1;
    struct ChildPart *bounce2;
    u8 filler74[0x88 - 0x74];
    s32 unk88;
};

struct Rec10
{
    s16 x;
    s16 y;
};

struct StageTarget
{
    u8 filler0[0x10];
    struct Rec10 *rec;
};

struct StageObj
{
    u8 filler0[0x18];
    struct StageClass *cls;
    void *children;
    u8 filler20[0x28 - 0x20];
    void *sprite;
    struct PartA *partA;
    struct ChildPart *part30;
    struct ChildPart *part34;
    struct PlacedObj *placed38;
    struct SpawnAnim *anim3C;
    void *shadowSrc;
    struct StageTarget *target;
    struct ChildPart *part48;
    s32 timer4C;
    s32 lastCount;
    s32 stage;
    s32 nextStage;
    s32 substate;
    s32 unk60;
    s32 tick;
    u16 phaseHw;
    u16 unk6A;
    u16 unk6C;
    u16 flags6E;
};

/* StageObj.stage: the stack minigame's top-level states */
enum StackGameStage
{
    STACK_STAGE_INIT,     /* reload palettes, start the music */
    STACK_STAGE_FADE_IN,  /* wait for timer4C */
    STACK_STAGE_INTRO,    /* obj_step_stage2: stacks close up, instructions text */
    STACK_STAGE_PLAY,     /* obj_spawn_phase: spawn falling pieces until time runs out */
    STACK_STAGE_RESULTS,  /* obj_step_stage4: count the last pieces, show the score */
    STACK_STAGE_PAUSE,    /* obj_tween_state: pause panel, Start returns to nextStage */
    STACK_STAGE_FADE_OUT, /* wait for timer4C */
    STACK_STAGE_EXIT,     /* clear the screen, stop the music, destroy the object */
};

/* gSys + 0x884: mask of palette banks to reload this frame, -1 = all */
#define SYS_PALETTE_RELOAD_OFS 0x884
/* the minigame's music track, played on song slot 0 */
#define STACK_GAME_SONG 5

extern u16 gUnk_02000050[]; /* REG_BLDCNT shadow; [2] is the REG_BLDY shadow */

struct PlacedObj
{
    u8 filler0[0x18];
    s32 unk18;
    u8 filler1C[0x34 - 0x1C];
    s32 unk34;
    u8 filler38[0x50 - 0x38];
    s32 unk50;
};

struct ChildObj
{
    s32 kind;
    u8 filler4[0x8 - 0x4];
    struct ChildObj *next;
    u32 classC;
    u8 *sprite;
    u8 filler14[0x18 - 0x14];
    s32 unk18;                  /* stack: x (8.8 fixed point) */
    s32 unk1C;                  /* stack: y, bounces between 0x4000 and 0x8000 */
    u8 filler20[0x2C - 0x20];
    s32 unk2C;
    struct ChildPart *part30;
    struct ChildPart *part34;
    struct PlacedObj *placed38;
    u8 filler3C[0x48 - 0x3C];
    struct ChildPart *part48;
    u8 filler4C[0x60 - 0x4C];
    s32 unk60;
    s32 unk64;
    u8 filler68[0x6C - 0x68];
    struct ChildPart *part6C;   /* stack: upper part (frame 3) */
    struct ChildPart *part70;   /* stack: lower part (frame 4) */
    struct ChildPart *part74;   /* stack: side part (frame 9, or 0xC when mirrored) */
    s32 unk78;                  /* stack: 1 idle, 2 spreading apart, 3 closing */
    s32 unk7C;
    s32 unk80;                  /* stack: y step per frame */
    s32 unk84;                  /* stack: how far the upper part sits above y */
    s32 unk88;                  /* stack: how far the lower part sits below y */
    s32 unk8C;                  /* stack: side part wobble phase, 0 = still */
};
/* sprite byte 0x11 (its mirror bits) bit 6: drawn flipped horizontally */
#define SPRITE_MIRROR_OFS 0x11
#define SPRITE_MIRROR_FLIP 0x40

void obj_sync_children_pos(struct ChildObj *obj);
void obj_step_bounce(struct ChildPart *part);
void obj_set_flag88_slot97(struct ChildObj *obj);
void obj_clear_flag88_slot97(struct ChildObj *obj);
void obj_aim_children(struct ChildObj *obj);
void obj_step_stage2(struct StageObj *obj);
void obj_spawn_phase(struct StageObj *obj);
void obj_step_stage4(struct StageObj *obj);
u32 rand_scaled(u32 a, u32 b);
extern const u8 gChaseReturnTable[];
void set_slot_value(u32 id, s32 value);
void obj_place_children(struct PlacedObj *p, u32 mode);
void obj_step_child_speeds(struct ChildObj *obj);
void obj_move_child_step(struct ChildObj *obj);
s32 obj_chase_step(struct ChildObj *node, s32 a1, s32 a2, void *a3, void *a4);
u32 timer_tick(void *timer);
void palette_fill(u32 v);
void slot_reset(u32 idx);
void set_slot_target(u32 a, u32 b, u32 c);
void obj_copy_pos_to_shadow(void *src);
void obj_tween_state(struct StageObj *obj);
void obj_step_children(struct ChildObj *obj);
void obj_script_step(void *obj);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_setup_slot(void *part, s32 pos, s32 a, s32 b, s32 c, ByteArg d);
u32 obj_is_done(void *obj);
u32 get_flag_bit(u32 id);
void win_draw_text(void *spr, u32 image, s32 x, s32 y, u32 flag);
void obj_load_record(void *rec, u32 id, u32 a, u32 b, u32 c);
void obj_init_class_38fa(void *part, u32 v);
void obj_list_update_pos(void *children, u32 a, u32 b);
void obj_update_anim_pair(void *anim, u32 a, u32 b);
void update_obj_list(void);
void sound_stop_song(u32 id);
u32 obj_flush_anim(void *sprite);
void obj_release_and_flush(void *sprite, u32 mode);
extern const u8 gText[];

static inline u32 sys_lang(void)
{
    return *((u8 *)&gSys + 0x8C0);
}

ASM_FUNC("asm/code_0816CA84/obj_create_pair_children.s", void obj_create_pair_children(void))

/* Stack minigame, each frame: holding B lifts the upper part and holding A the lower one
 * (speed +0x20 per frame up to 0x100), a released part falls back (-0x20 down to -0x200);
 * both then take a bounce step. Sets slot 97's flag while either button is held, then re-aims
 * the children. */
void obj_step_child_speeds(struct ChildObj *obj)
{
    struct ChildPart *part;
    s32 speed;

    if (gSys.keys & B_BUTTON)
    {
        part = obj->part6C;
        speed = part->unk70 + 0x20;
        part->unk70 = speed;
        if (speed > 0x100)
            part->unk70 = 0x100;
        obj_step_bounce(part);
    }
    else
    {
        part = obj->part6C;
        speed = part->unk70 - 0x20;
        part->unk70 = speed;
        if (speed < -0x200)
            part->unk70 = -0x200;
        obj_step_bounce(part);
    }

    if (gSys.keys & A_BUTTON)
    {
        part = obj->part70;
        speed = part->unk70 + 0x20;
        part->unk70 = speed;
        if (speed > 0x100)
            part->unk70 = 0x100;
        obj_step_bounce(part);
    }
    else
    {
        part = obj->part70;
        speed = part->unk70 - 0x20;
        part->unk70 = speed;
        if (speed < -0x200)
            part->unk70 = -0x200;
        obj_step_bounce(part);
    }

    if (gSys.keys & (A_BUTTON | B_BUTTON))
        obj_set_flag88_slot97(obj);
    else
        obj_clear_flag88_slot97(obj);
    obj_aim_children(obj);
}

ASM_FUNC("asm/code_0816CA84/obj_aim_children.s", void obj_aim_children(struct ChildObj *obj))

/*
 * Builds a stack object at (x, y) (8.8 fixed point) from three parts of sprite 0x5007, each put
 * in `list`: the upper part (frame 3), the lower part (frame 4) and the side part (frame 9).
 * Right of the screen centre (x > 120 px) the upper and lower parts are mirrored and the side
 * part uses frame 0xC. It starts idle, closed, still, bouncing by `yStep` per frame.
 */
struct ChildObj *obj_create_stack(struct ChildObj *obj, void *list, s32 x, s32 y, s32 yStep)
{
    u8 *upperSprite;
    u8 *lowerSprite;

    obj_init_big(obj, 2);
    obj->classC = (u32)gStackClass;
    obj->part6C = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagStackPart1, 0), 3);
    list_insert(list, obj->part6C);
    obj_create_sprite_10(obj->part6C, 4, 0x5007, 0, 3, 2);
    obj->part70 = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagStackPart2, 0), 4);
    list_insert(list, obj->part70);
    obj_create_sprite_10(obj->part70, 4, 0x5007, 0, 4, 2);
    obj->part74 = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagStackPart3, 0), 5);
    list_insert(list, obj->part74);
    if (x > 0x7800)
    {
        upperSprite = obj->part6C->sprite;
        lowerSprite = obj->part70->sprite;
        lowerSprite[SPRITE_MIRROR_OFS] |= SPRITE_MIRROR_FLIP;
        upperSprite[SPRITE_MIRROR_OFS] |= SPRITE_MIRROR_FLIP;
        obj_create_sprite_10(obj->part74, 4, 0x5007, 0, 0xC, 2);
    }
    else
    {
        obj_create_sprite_10(obj->part74, 4, 0x5007, 0, 9, 2);
    }
    obj->unk18 = x;
    obj->unk1C = y;
    obj->unk7C = 0;
    obj->unk80 = yStep;
    obj->unk84 = 0;
    obj->unk88 = 0;
    obj->unk78 = 1;
    obj->unk8C = 0;
    obj_sync_children_pos(obj);
    return obj;
}

void obj_move_child_step(struct ChildObj *obj)
{
    s32 *step = &obj->unk80;
    s32 v;
    s32 next;

    obj->unk1C += *step;
    if (obj->unk1C < 0x4000)
    {
        obj->unk1C = 0x4000;
        *step = -*step;
    }
    else if (obj->unk1C > 0x8000)
    {
        obj->unk1C = 0x8000;
        *step = -*step;
    }

    if (obj->unk78 == 2)
        goto grow;
    if (obj->unk78 <= 2)
        goto sync;
    if (obj->unk78 == 3)
        goto shrink;
    goto sync;

grow:
    v = obj->unk84 + 0x100;
    obj->unk84 = v;
    obj->unk88 = v;
    if (obj->unk84 <= 0x1800)
        goto sync;
    obj->unk88 = 0x1800;
    obj->unk84 = 0x1800;
    next = 0;
    goto store;

shrink:
    v = obj->unk84 - 0x100;
    obj->unk84 = v;
    obj->unk88 = v;
    if (obj->unk84 >= 0)
        goto sync;
    obj->unk88 = 0;
    obj->unk84 = 0;
    next = 1;

store:
    obj->unk78 = next;

sync:
    obj_sync_children_pos(obj);
}

/*
 * Stack minigame: places the parts around the stack at (unk18, unk1C). The upper part sits
 * unk84 above it and the lower part unk88 below it. The side part sits 0x2800 (40 px) to the
 * left (to the right when the stack is mirrored), nudged in by half a sine wave while the
 * wobble phase unk8C runs from 3 up to 0x80 (it stops at 0 again).
 */
void obj_sync_children_pos(struct ChildObj *stack)
{
    s32 phase;
    s32 offset;
    s32 sideX;
    u8 *upperSprite;
    struct ChildPart *side;

    stack->part6C->x = stack->unk18;
    stack->part6C->y = stack->unk1C - stack->unk84;
    stack->part70->x = stack->unk18;
    stack->part70->y = stack->unk1C + stack->unk88;

    phase = stack->unk8C;
    if (phase != 0)
    {
        phase += 3;
        stack->unk8C = phase;
        if (phase > 0x80)
            stack->unk8C = 0;
    }

    upperSprite = stack->part6C->sprite;
    if (upperSprite[SPRITE_MIRROR_OFS] & SPRITE_MIRROR_FLIP)
    {
        offset = gSineTable[*(u8 *)&stack->unk8C];
        side = stack->part74;
        if (offset < 0)
            offset += 0x3F;
        offset >>= 6; /* offset / 64 */
        offset <<= 5;
        offset += -0x2800;
        sideX = stack->unk18 - offset;
    }
    else
    {
        offset = gSineTable[*(u8 *)&stack->unk8C];
        side = stack->part74;
        if (offset < 0)
            offset += 0x3F;
        offset >>= 6; /* offset / 64 */
        offset <<= 5;
        offset += -0x2800;
        sideX = stack->unk18 + offset;
    }
    side->x = sideX;
    side->y = stack->unk1C;
}

ASM_FUNC("asm/code_0816CA84/obj_chase_step.s", s32 obj_chase_step(struct ChildObj *node, s32 a1, s32 a2, void *a3, void *a4))

ASM_FUNC("asm/code_0816CA84/obj_scene_init.s", void obj_scene_init(void))

/*
 * Per-frame update of the stack minigame: runs the current stage (see enum StackGameStage), then
 * copies the background scroll to the BG offset shadows. The exit stage instead clears the
 * palettes, stops the music, turns blending off and destroys the object (vtable call, mode 3).
 * set_slot_target(slot, song, -1) plays a song, slot_reset(slot) stops it.
 */
void obj_step_stages(struct StageObj *game)
{
    u16 *blend;

    rand_scaled(0, 2); /* only advances the RNG */
    switch (game->stage)
    {
    case STACK_STAGE_INIT:
        {
            s32 off = SYS_PALETTE_RELOAD_OFS;
            *(s32 *)((u8 *)&gSys + off) = -1;
        }
        set_slot_target(0, STACK_GAME_SONG, -1);
        game->stage = STACK_STAGE_FADE_IN;
        break;
    case STACK_STAGE_FADE_IN:
        if ((u8)timer_tick(&game->timer4C))
            game->stage = STACK_STAGE_INTRO;
        break;
    case STACK_STAGE_INTRO:
        obj_step_stage2(game);
        break;
    case STACK_STAGE_PLAY:
        obj_spawn_phase(game);
        break;
    case STACK_STAGE_RESULTS:
        obj_step_stage4(game);
        break;
    case STACK_STAGE_PAUSE:
        obj_tween_state(game);
        break;
    case STACK_STAGE_FADE_OUT:
        if ((u8)timer_tick(&game->timer4C))
            game->stage = STACK_STAGE_EXIT;
        break;
    case STACK_STAGE_EXIT:
        palette_fill(0);
        slot_reset(0);
        blend = (u16 *)gUnk_02000050;
        *blend = 0;     /* BLDCNT */
        blend += 2;
        *blend = 0;     /* BLDY */
        if (game != 0)
            game->cls->behavior((u8 *)game + game->cls->thisOffset, 3);
        return;
    default:
        break;
    }
    obj_copy_pos_to_shadow(game->shadowSrc);
}

/*
 * Steps every child in obj's list by its kind (0: speeds, 2: bouncing move, 6: chaser). A chaser
 * step returns the points it scored: their absolute value goes on the score counter (placed38:
 * +0x18 score, clamped to 0..999; +0x34 best score; +0x50 set on a new best), with sound 0x2E for a
 * positive result. A negative result also knocks back part30 (or part34 when +0x60 is set): it starts
 * shrinking (state 3) and shakes, sound 0x40. The table's special result adds 60 to the timer at
 * +0x64 and shows part48 (clears bit 4 of its sprite's +0x12). Then the counter is drawn.
 */
void obj_step_children(struct ChildObj *obj)
{
    struct ChildObj *node;
    struct ChildObj *next;
    s32 points;
    struct ChildPart *part;
    struct PlacedObj *counter;
    u8 *sprite;
    s32 bits;

    node = *(struct ChildObj **)obj->unk1C;
    if (node == 0)
        goto place;
    do
    {
        next = node->next;
        switch (node->kind)
        {
        case 0:
            obj_step_child_speeds(node);
            break;
        case 2:
            obj_move_child_step(node);
            break;
        case 6:
            points = obj_chase_step(node, obj->unk1C, obj->unk2C, obj->part30, obj->part34);
            if (points == (s8)gChaseReturnTable[2])
            {
                obj->unk64 += 60;
                sprite = obj->part48->sprite;
                bits = sprite[0x12];
                sprite[0x12] = bits & ~0x10;
            }
            if (points >= 0)
            {
                if (points > 0)
                {
                    set_slot_value(0x2E, -1);
                    counter = obj->placed38;
                    counter->unk18 += points;
                    if (counter->unk18 < 0)
                        counter->unk18 = 0;
                    if (counter->unk18 > 999)
                        counter->unk18 = 999;
                    if (counter->unk34 < counter->unk18)
                    {
                        counter->unk34 = counter->unk18;
                        counter->unk50 = 1;
                    }
                }
            }
            else
            {
                counter = obj->placed38;
                counter->unk18 -= points;
                if (counter->unk18 < 0)
                    counter->unk18 = 0;
                if (counter->unk18 > 999)
                    counter->unk18 = 999;
                if (counter->unk34 < counter->unk18)
                {
                    counter->unk34 = counter->unk18;
                    counter->unk50 = 1;
                }
                if (obj->unk60 == 0)
                {
                    part = obj->part30;
                    if (part->unk78 == 0)
                    {
                        part->unk78 = 3;
                        part->unk8C += 0x10;
                        set_slot_value(0x40, -1);
                    }
                }
                else
                {
                    part = obj->part34;
                    if (part->unk78 == 0)
                    {
                        part->unk78 = 3;
                        part->unk8C += 0x10;
                        set_slot_value(0x40, -1);
                    }
                }
            }
            break;
        }
        node = next;
    } while (node != 0);

place:
    obj_place_children(obj->placed38, 0);
}

NONMATCH("asm/code_0816CA84/obj_step_stage2.s", void obj_step_stage2(struct StageObj *obj))
{
    s32 sub;
    struct ChildPart *bounce;
    struct PartA *a;
    s32 limit;
    s32 off;
    s32 x;
    s32 y;
    s32 sv;
    s32 v;
    u32 done;
    u32 image;
    u32 flushed;
    u8 flag;
    void *sprite;
    struct Rec10 *rec;
    u8 zero;
    u8 one;

    sub = obj->substate;
    if (sub <= 10)
    {
        if (sub >= 7)
        {
            if ((gSys.keys2A & 8) != 0)
            {
                a = obj->partA;
                if (a->unk88 != 0)
                {
                    a->unk88 = 0;
                    sound_stop_song(0x97);
                }
                set_slot_value(0x37, -1);
                obj->nextStage = 2;
                obj->stage = 5;
                obj->phaseHw = 0;
                return;
            }
        }
    }
    obj_script_step(obj->target);
    bounce = obj->partA->bounce1;
    bounce->unk70 += 0x20;
    limit = 0x100;
    if (bounce->unk70 > limit)
        bounce->unk70 = limit;
    obj_step_bounce(bounce);
    bounce = obj->partA->bounce2;
    bounce->unk70 += 0x20;
    if (bounce->unk70 > limit)
        bounce->unk70 = limit;
    obj_step_bounce(bounce);
    obj_aim_children((struct ChildObj *)obj->partA);
    obj_script_step(obj->part30);
    obj_script_step(obj->part34);
    obj_place_children(obj->placed38, 0);
    obj_update_anim_pair(obj->anim3C, 0, 0);

    switch (obj->substate)
    {
    case 0:
        zero = 0;
        obj->tick = 0;
        obj->placed38->unk18 = 0;
        obj->placed38->unk50 = 0;
        obj->anim3C->unk18 = 0x3C;
        obj->anim3C->unk1C = 0;
        if ((obj->flags6E & 1) != 0)
        {
            obj_setup_slot(obj->part30, obj->part30->x, 0x6000, 0, 0x3C, zero);
            obj_setup_slot(obj->part34, obj->part34->x, 0x6000, 0, 0x3C, zero);
            obj->substate = 7;
            break;
        }
        one = 1;
        obj->substate = 1;
        obj->flags6E |= 1;
        goto count_up;
    case 1:
    count_up:
        off = obj->part34->unk84 + 0x100;
        obj->part34->unk84 = off;
        if (off > 9215)
        {
            obj_load_record(obj->target->rec, 0xA, 0, 0, 0);
            one = 1;
            obj_setup_slot(obj->target, 0xFC00, 0x7000, 0, 0x5A, one);
            obj->substate = 2;
        }
        break;
    case 2:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
        {
            flag = get_flag_bit(0x1F1);
            if (flag == 0)
            {
                sprite = obj->sprite;
                image = *(u32 *)((u8 *)gText + 0x11C824 + (sys_lang() << 2));
                rec = obj->target->rec;
                x = rec->x - 8;
                y = rec->y - 0x10;
                win_draw_text(sprite, image, x, y, flag);
                obj_load_record(obj->target->rec, 0xB, 0, 0, flag);
                obj->substate = 3;
            }
            else
            {
                sprite = obj->sprite;
                image = *(u32 *)((u8 *)gText + 0x11C838 + (sys_lang() << 2));
                rec = obj->target->rec;
                x = rec->x - 8;
                y = rec->y - 0x10;
                win_draw_text(sprite, image, x, y, 0);
                obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
                obj->substate = 4;
            }
        }
        break;
    case 3:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 0)
        {
            obj_release_and_flush(obj->sprite, 0);
            image = *(u32 *)((u8 *)gText + 0x11C838 + (sys_lang() << 2));
            rec = obj->target->rec;
            x = rec->x - 8;
            y = rec->y - 0x10;
            win_draw_text(obj->sprite, image, x, y, flushed);
            obj_load_record(obj->target->rec, 0xB, 0, 0, flushed);
            obj->substate = 4;
        }
        break;
    case 4:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 0)
        {
            obj_release_and_flush(obj->sprite, 0);
            obj_load_record(obj->target->rec, 0xA, 0, 0, flushed);
            one = 1;
            obj_setup_slot(obj->target, 0x12000, 0x7000, 0, 0x1E, one);
            obj->substate = 5;
        }
        break;
    case 5:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
            obj->substate = 6;
        break;
    case 6:
        off = obj->part34->unk84 - 0x100;
        obj->part34->unk84 = off;
        if (off <= 0)
        {
            zero = 0;
            obj->part34->unk84 = 0;
            obj_setup_slot(obj->part30, obj->part30->x, 0x6000, 0, 0x3C, zero);
            obj_setup_slot(obj->part34, obj->part34->x, 0x6000, 0, 0x3C, zero);
            obj->substate = 7;
        }
        break;
    case 7:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj_init_class_38fa(obj->part30, 0x1E);
            obj->substate = 8;
        }
        break;
    case 8:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj->substate = 9;
            obj_init_class_38fa(obj->part30, 0x40);
        }
        break;
    case 9:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj->substate = 10;
            obj_init_class_38fa(obj->part30, 0x1E);
            obj->part34->unk88 = 0;
            obj->part34->unk84 = 0;
            obj->part30->unk88 = 0;
            obj->part30->unk84 = 0;
        }
        else
        {
            sv = gSineTable[((*(s16 *)((u8 *)obj->part30 + 0x4C)) * 4) & 0xFF];
            if (sv < 0)
                sv += 0x3F;
            v = (sv >> 6) << 4;
            if (v < 0)
                v = -v;
            obj->part34->unk88 = v;
            obj->part34->unk84 = v;
            obj->part30->unk88 = v;
            obj->part30->unk84 = v;
        }
        break;
    case 10:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj->stage = 3;
            obj->substate = 0;
        }
        break;
    default:
        break;
    }
    obj_sync_children_pos((struct ChildObj *)obj->part30);
    obj_sync_children_pos((struct ChildObj *)obj->part34);
    obj_list_update_pos(obj->children, 0, 0);
    update_obj_list();
}
END_NONMATCH

ASM_FUNC("asm/code_0816CA84/obj_spawn_phase.s", void obj_spawn_phase(struct StageObj *obj))

void write_var(u16 id, u16 value);
void tween_groups_set_flag_bit(u16 index, u8 value);
void timer_set(void *timer, u32 limit);
void anim_task_start(void *task, u32 a, const void *table, u32 b, u32 c);
extern const u8 gHeapTagAnimTaskData[];
NONMATCH("asm/code_0816CA84/obj_step_stage4.s", void obj_step_stage4(struct StageObj *obj))
{
    struct ChildObj *node;
    struct ChildObj *next;
    struct ChildPart *bounce;
    struct PlacedObj *placed;
    s32 chase_count;
    s32 delta;
    s32 limit;
    s32 off;
    s32 x;
    s32 y;
    s32 pflag;
    u32 done;
    u32 flushed;
    u32 image;
    u8 flag;
    u8 zero;
    u8 one;
    void *sprite;

    obj_script_step(obj->target);
    obj_script_step(obj->part30);
    obj_script_step(obj->part34);
    bounce = obj->partA->bounce1;
    bounce->unk70 -= 0x20;
    if (bounce->unk70 < -512)
        bounce->unk70 = -512;
    obj_step_bounce(bounce);
    bounce = obj->partA->bounce2;
    bounce->unk70 += 0x20;
    limit = 0x100;
    if (bounce->unk70 > limit)
        bounce->unk70 = limit;
    obj_step_bounce(bounce);
    obj_aim_children((struct ChildObj *)obj->partA);

    chase_count = 0;
    node = *(struct ChildObj **)obj->children;
    while (node != 0)
    {
        next = node->next;
        if (node->kind == 6)
        {
            chase_count++;
            delta = obj_chase_step(node, (s32)obj->children, (s32)obj->partA, obj->part30, obj->part34);
            if (delta < 0)
                delta = -delta;
            placed = obj->placed38;
            placed->unk18 += delta;
            if (placed->unk18 < 0)
                placed->unk18 = 0;
            if (placed->unk18 > 0x3E7)
                placed->unk18 = 0x3E7;
            if (placed->unk34 < placed->unk18)
            {
                placed->unk34 = placed->unk18;
                placed->unk50 = 1;
            }
        }
        node = next;
    }
    obj_place_children(obj->placed38, 0);

    switch (obj->substate)
    {
    case 0:
        zero = 0;
        obj->tick = 0;
        obj_load_record(obj->part48->sprite, 0x25, 0, 0, zero);
        ((u8 *)obj->part48->sprite)[0x12] |= 0x10;
        if (obj->partA->unk88 == 0)
        {
            obj->partA->unk88 = 1;
            set_slot_value(0x97, -1);
        }
        obj_setup_slot(obj->part30, obj->part30->x, 0x8000, 0, 0x3C, zero);
        obj_setup_slot(obj->part34, obj->part34->x, 0x8000, 0, 0x3C, zero);
        obj->substate = 1;
        break;
    case 1:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            if (obj->partA->unk88 != 0)
            {
                obj->partA->unk88 = 0;
                sound_stop_song(0x97);
            }
            obj_init_class_38fa(obj->part30, 0x3C);
            obj->substate = 2;
        }
        break;
    case 2:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            if (chase_count == 0)
                obj->substate = 3;
        }
        break;
    case 3:
        off = obj->part34->unk84 + 0x100;
        obj->part34->unk84 = off;
        if (off > 9215)
        {
            obj_load_record(obj->target->rec, 0xA, 0, 0, 0);
            one = 1;
            obj_setup_slot(obj->target, 0xFC00, 0x7000, 0, 0x5A, one);
            obj->substate = 4;
        }
        break;
    case 4:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
        {
            placed = obj->placed38;
            if (obj->lastCount < placed->unk18)
            {
                obj->lastCount = placed->unk18;
                write_var(0x1CDD, placed->unk18);
                tween_groups_set_flag_bit(0x1ED, 1);
            }
            placed = obj->placed38;
            pflag = placed->unk50;
            if (pflag == 0)
            {
                sprite = obj->sprite;
                image = *(u32 *)((u8 *)gText + 0x11C9A0 + (sys_lang() << 2));
                x = obj->target->rec->x - 8;
                y = obj->target->rec->y - 0x10;
                win_draw_text(sprite, image, x, y, 0);
                obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
                obj->substate = 7;
            }
            else
            {
                write_var(0x1CE3, placed->unk34);
                flag = get_flag_bit(0x1F1);
                if (flag == 0)
                {
                    tween_groups_set_flag_bit(0x1F1, 1);
                    sprite = obj->sprite;
                    image = *(u32 *)((u8 *)gText + 0x11C860 + (sys_lang() << 2));
                    x = obj->target->rec->x - 8;
                    y = obj->target->rec->y - 0x10;
                    win_draw_text(sprite, image, x, y, flag);
                    obj_load_record(obj->target->rec, 0xB, 0, 0, flag);
                    obj->substate = 8;
                }
                else
                {
                    sprite = obj->sprite;
                    image = *(u32 *)((u8 *)gText + 0x11C9B4 + (sys_lang() << 2));
                    x = obj->target->rec->x - 8;
                    y = obj->target->rec->y - 0x10;
                    win_draw_text(sprite, image, x, y, 0);
                    obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
                    obj->substate = 7;
                }
            }
        }
        break;
    case 5:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
            obj->substate = 6;
        break;
    case 6:
        off = obj->part34->unk84 - 0x100;
        obj->part34->unk84 = off;
        if (off <= 0)
        {
            obj->part34->unk84 = 0;
            obj->substate = 0;
            obj->stage = 2;
        }
        break;
    case 7:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 2)
            goto reload;
        if ((s32)flushed > 2)
        {
            if (flushed == 0x3E7)
                goto reload;
            break;
        }
        if (flushed == 1)
        {
            obj_release_and_flush(obj->sprite, 0);
            obj_load_record(obj->target->rec, 0xA, 0, 0, 0);
            obj_setup_slot(obj->target, 0x12000, 0x7000, 0, 0x1E, (u8)flushed);
            obj->substate = 5;
        }
        break;
    reload:
        obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
        obj_release_and_flush(obj->sprite, 0);
        image = *(u32 *)((u8 *)gText + 0x11C874 + (sys_lang() << 2));
        x = obj->target->rec->x - 8;
        y = obj->target->rec->y - 0x10;
        win_draw_text(obj->sprite, image, x, y, 0);
        obj->substate = 8;
        break;
    case 8:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 0)
        {
            obj_release_and_flush(obj->sprite, 0);
            timer_set(&obj->timer4C, 0x20);
            anim_task_start(heap_alloc2(0x24, 0, gHeapTagAnimTaskData, 0), 8, gHeapTagAnimTaskData, 1, 0x20);
            obj->stage = 6;
        }
        break;
    default:
        break;
    }
    obj_sync_children_pos((struct ChildObj *)obj->part30);
    obj_sync_children_pos((struct ChildObj *)obj->part34);
    obj_list_update_pos(obj->children, 0, 0);
    update_obj_list();
}
END_NONMATCH
