#include "global.h"
#include "ram_routines.h"

struct AnimPose
{
    u8 filler0[0x12];
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;   /* animation index within its bank */
    u8 filler18[0x1E - 0x18];
    u8 unk1E;    /* low nibble: animation bank (see POSE_ANIM_ID) */
    u8 filler1F[0x21 - 0x1F];
    u8 kind21;
};

/* the sprite id a pose's animation was loaded from, like the 0x2xxx ids sprite_set_action takes */
#define POSE_ANIM_ID(pose) ((pose)->unk16 | ((((u32)(pose)->unk1E << 28) >> 16) + 0x1000))

/* AnimObj.index of the two heroes: the bitwise NOT of their gParty index */
#define HERO_INDEX_MARIO -1
#define HERO_INDEX_LUIGI -2

/* AnimObj.stepB2 (a figure jump's vertical speed, added to its height unk18) drops by this each frame */
#define FIGURE_JUMP_GRAVITY 0xB3

/* each hero's first animation id; a jump only starts from it */
#define MARIO_ANIM_BASE 0x2000
#define LUIGI_ANIM_BASE 0x204D

struct AnimPic
{
    struct AnimPose *sprite;
    u8 filler4[4];
    struct AnimPose *pose;
};

struct World
{
    u8 filler0[0x0B];
    u8 flags0B;
    u8 filler0C[0x7C - 0x0C];
    u8 *active;
    struct AnimObj *fig80[2];
    u8 filler88[0x2C1 - 0x88];
    u8 flags2C1;
};

struct WorldFlags
{
    u8 low2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct AnimObj
{
    u8 filler0[8];
    struct AnimPic pic;
    u8 filler14[0x18 - 0x14];
    s32 unk18;
    u8 filler1C[0x28 - 0x1C];
    u8 *anim;
    u8 filler2C[0x40 - 0x2C];
    s32 unk40;
    u8 filler44[0x4C - 0x44];
    u32 unk4C;
    u8 filler50[0x58 - 0x50];
    u32 unk58;
    u8 filler5C[0x77 - 0x5C];
    u8 flags77;
    u8 filler78[0x7E - 0x78];
    u8 ctrl;
    u8 filler7F[0xB2 - 0x7F];
    u16 stepB2;
    u8 fillerB4[0xEC - 0xB4];
    s16 index;
    u8 filler0EE[0xF0 - 0xEE];
    u8 *part;
    u8 filler0F4[0x10A - 0xF4];
    u16 factor10A;
    u16 factor10C;
    u8 filler10E[0x118 - 0x10E];
    void *lowestPose;
    u16 factor11C;
    u16 bits;
    u8 unk120;
    u8 unk121;
    u8 flags;
    u8 filler123[0x126 - 0x123];
    u16 unk126;
    u8 unk128;
    u8 unk129;
    u8 unk12A;
    u8 filler12B;
    u8 unk12C;
};
struct AnimPoseFlags
{
    u8 low0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct AnimFlags77
{
    u8 low3 : 3;
    u8 rest : 5;
};

struct AnimMode120
{
    u8 low3 : 3;
    u8 mode : 3;
    u8 rest : 2;
};

struct AnimMode120Wide
{
    u16 low6 : 6;
    u16 mode : 3;
    u16 rest : 7;
};

struct AnimMode121Low
{
    u8 low1 : 1;
    u8 mode : 3;
    u8 rest : 4;
};

struct AnimMode121
{
    u8 low4 : 4;
    u8 mid : 3;
    u8 top : 1;
};

struct AnimFlags
{
    u8 low2 : 2;
    u8 mode : 3;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

/* AnimObj.bits: the battle statuses a figure has (see code_08108AC8.c). */
enum AnimStatusBits
{
    ANIM_STATUS_BIT1 = 0x0001,     /* bits 0-1: one of two exclusive statuses (anim_bit1_chance) */
    ANIM_STATUS_BIT2 = 0x0002,     /* (anim_bit2_chance) */
    ANIM_STATUS_MOTION_A = 0x000C, /* 4: raised, 8: lowered, by factor10A (anim_set_motion_a) */
    ANIM_STATUS_MOTION_B = 0x0030, /* 0x10: raised, 0x20: lowered, by factor10C (anim_set_motion_b) */
    ANIM_STATUS_MOTION_C = 0x00C0, /* 0x40: raised, 0x80: lowered, by factor11C (anim_set_motion_c) */
    ANIM_STATUS_BIT100 = 0x0100,   /* 0x100 and 0x200 exclude each other */
    ANIM_STATUS_BIT200 = 0x0200,
    ANIM_STATUS_BIT400 = 0x0400,   /* 0x400 and 0x800 cancel each other out (anim_bit400_step) */
    ANIM_STATUS_BIT800 = 0x0800,   /* (anim_chance_start) */
    ANIM_STATUS_BIT1000 = 0x1000,  /* bits 12-15: rolled by anim_roll_bit* */
    ANIM_STATUS_BIT2000 = 0x2000,
    ANIM_STATUS_BIT4000 = 0x4000,
    ANIM_STATUS_BIT8000 = 0x8000
};

/* Bits 15-17 of the word at AnimObj+0x120: the turns status 0x400 / 0x800 has left. */
#define ANIM_STATUS_400_800_TURNS 0x38000

/* AnimObj.unk12C of a hero with status 0x800 / 0x400 of its own, which it keeps
   (anim_end_mode gives it back when the timed one ends). */
#define FIGURE_KIND_OWN_BIT800 0x13
#define FIGURE_KIND_OWN_BIT400 0x14

struct AnimTask
{
    u8 filler0[4];
    void (*state)(struct AnimTask *);
    struct AnimObj *obj;
    u8 filler0C[0x10 - 0x0C];
    u16 timer;
};

void set_slot_value(u32 id, s32 value);
void *find_lowest_order_pose(u8 *pic);
void sprite_set_action(void *obj, u32 action, u32 arg, u32 extra);
extern struct World *gWorld;
extern u8 *gParty;
void sound_stop_song(u32 song);
extern s8 gPoseKindPct[];
extern s8 gPoseChancePct[];
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void ensure_code_entry(u32 key);
u8 is_state_busy(void);
struct AnimTask *task_create_list_40(void (*entry)(struct AnimTask *));
void state_hero_marker_end_quiet(struct AnimTask *owner);
void state_hero_marker_begin(struct AnimTask *owner);
void state_figure_end(struct AnimTask *owner);
void state_figure_move(struct AnimTask *owner);
void state_figure_limit(struct AnimTask *owner);
void state_wait_ready(struct AnimTask *owner);
void task_wait_control(struct AnimTask *owner);
s32 anim_bit1_chance(struct AnimObj *obj, s32 value);
s32 anim_bit2_chance(struct AnimObj *obj, s32 value);
s32 anim_bit100_step(struct AnimObj *obj, s32 value);
s32 anim_bit200_step(struct AnimObj *obj, s32 value);
s32 anim_bit400_step(struct AnimObj *obj, s32 value);
s32 anim_chance_start(struct AnimObj *obj, s32 value);
s32 anim_set_motion_a(struct AnimObj *obj, u16 value, s32 extra);
s32 anim_set_motion_b(struct AnimObj *obj, u16 value, s32 extra);
s32 anim_set_motion_c(struct AnimObj *obj, u16 value, s32 extra);
s32 anim_roll_bit1000(struct AnimObj *obj, s32 chance);
s32 anim_roll_bit2000(struct AnimObj *obj, s32 chance);
s32 anim_roll_bit4000(struct AnimObj *obj, s32 chance);
s32 anim_roll_bit8000(struct AnimObj *obj, s32 chance);

/* Called when the turns of status 0x400 / 0x800 run out: clears both statuses, gives a hero whose
   unk12C says it has one of them of its own (0x13: 0x800, 0x14: 0x400) that status back and
   refreshes its pose, then clears the turn counter. */
void anim_end_mode(struct AnimObj *obj)
{
    if (*(u32 *)((u8 *)obj + 0x120) & ANIM_STATUS_400_800_TURNS)
    {
        obj->bits &= ~(ANIM_STATUS_BIT400 | ANIM_STATUS_BIT800) & 0xFFFF;
        if (obj->index < 0)
        {
            u32 kind = obj->unk12C;
            u32 bits = obj->bits;
            u32 next;

            if (kind != FIGURE_KIND_OWN_BIT800)
            {
                next = bits;
                if (kind == FIGURE_KIND_OWN_BIT400)
                {
                    next |= ANIM_STATUS_BIT400;
                }
            }
            else
            {
                next = ANIM_STATUS_BIT800 | bits;
            }
            obj->bits = next;
            obj->unk128 = 0xFF;
            obj->unk126 = 1;
            obj->unk12A = 1;
            ((struct AnimFlags *)&obj->flags)->bit6 = 1;
        }
        *(u32 *)((u8 *)obj + 0x120) &= ~ANIM_STATUS_400_800_TURNS;
    }
}

void state_hero_marker_end(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;
    struct AnimPic *pic = &obj->pic;

    if (pic->pose->flags & 8)
    {
        u32 slot;
        s32 index;

        sprite_set_action(pic, -1, -1, 0);
        ((struct AnimFlags *)&obj->flags)->bit5 = 0;
        owner->state = 0;
        index = obj->index;
        slot = 0xF5;
        if (index == -1)
        {
            slot += 0x52;
        }
        sound_stop_song(slot);
    }
}

void state_figure_begin(struct AnimTask *owner);
void state_hero_marker_begin(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;

    if ((obj->ctrl & 6) == 2)
    {
        struct AnimPic *pic = &obj->pic;
        s32 index;
        s32 none;
        u32 action;
        u32 slot;

        index = obj->index;
        none = -1;
        action = 0x2057;
        if (index == none)
        {
            action -= 0x2F;
        }
        sprite_set_action(pic, 2, action, 0);
        owner->state = state_hero_marker_end;
        if (obj->index == none)
        {
            slot = 0x147;
        }
        else
        {
            slot = 0xF5;
        }
        set_slot_value(slot, none);
    }
    else
    {
        ((struct AnimFlags *)&obj->flags)->bit5 = 0;
        owner->state = 0;
    }
}

void state_hero_marker_end_quiet(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;
    struct AnimPic *pic = &obj->pic;

    if (pic->pose->flags & 8)
    {
        sprite_set_action(pic, -1, -1, 0);
        ((struct AnimFlags *)&obj->flags)->bit5 = 0;
        owner->state = 0;
    }
}

void state_countdown_slot(struct AnimTask *owner)
{
    u16 timer = owner->timer;

    if ((s16)owner->timer != 0)
    {
        s32 left = timer - 1;

        owner->timer = left;
        if ((left << 16) != 0)
        {
            return;
        }
    }
    set_slot_value(0x151, -1);
    owner->state = state_hero_marker_end_quiet;
}

void state_wait_ready(struct AnimTask *owner)
{
    if (!is_state_busy()) {
        struct AnimObj *obj = owner->obj;
        struct AnimPic *pic = &obj->pic;
        u32 action;
        u32 slot;

        if (obj->index == -1)
        {
            action = 0x202B;
        }
        else
        {
            action = 0x205A;
        }
        sprite_set_action(pic, 0, action, 0);
        if (obj->index == -1)
        {
            slot = 0xF0;
        }
        else
        {
            slot = 0xFE;
        }
        set_slot_value(slot, -1);
        owner->timer = 0x10;
        owner->state = state_countdown_slot;
    }
}

void task_wait_control(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;

    if ((obj->ctrl & 6) == 2)
    {
        s32 index;
        s32 none;
        u32 action;

        index = obj->index;
        none = -1;
        action = 0x205A;
        if (index == none)
        {
            action -= 0x2F;
        }
        ensure_code_entry(action);
        owner->state = state_wait_ready;
    }
    else
    {
        ((struct AnimFlags *)&obj->flags)->bit5 = 0;
        owner->state = 0;
    }
}

void anim_bit80_scale11c(struct AnimObj *obj)
{
    if (obj->bits & 0x80)
    {
        obj->bits &= 0xFF7F;
        obj->factor11C = 0x100;
    }
}

void anim_bit20_scale10c(struct AnimObj *obj)
{
    if (obj->bits & 0x20)
    {
        obj->bits &= 0xFFDF;
        obj->factor10C = 0x100;
    }
}

void anim_bit8_scale10a(struct AnimObj *obj)
{
    if (obj->bits & 8)
    {
        obj->bits &= 0xFFF7;
        obj->factor10A = 0x100;
    }
}

void anim_clear_high_bits(struct AnimObj *obj)
{
    obj->bits &= 0xFFF;
}

void anim_stop_mode(struct AnimObj *obj)
{
    if (obj->bits & 3)
    {
        if (obj->index < 0)
        {
            if ((obj->flags & 0x20) == 0)
            {
                struct AnimTask *task = task_create_list_40(task_wait_control);

                task->obj = obj;
                ((struct AnimFlags *)&obj->flags)->bit5 = 1;
            }
        }
        obj->bits &= 0xFFFC;
    }
}

s32 anim_roll_bit8000(struct AnimObj *obj, s32 chance)
{
    if (obj->bits & 0x8000)
    {
        return 0;
    }
    if (chance < rand_below(100, mt19937_next()))
    {
        return 0;
    }
    obj->bits = (s16)(obj->bits & 0xFFF) | (s16)0x8000;
    ((struct AnimFlags *)&obj->flags)->mode = 3;
    return 1;
}

s32 anim_roll_bit4000(struct AnimObj *obj, s32 chance)
{
    if (obj->bits & 0x4000)
    {
        return 0;
    }
    if (chance < rand_below(100, mt19937_next()))
    {
        return 0;
    }
    obj->bits = (obj->bits & 0xFFF) | 0x4000;
    ((struct AnimFlags *)&obj->flags)->mode = 3;
    return 1;
}

s32 anim_roll_bit2000(struct AnimObj *obj, s32 chance)
{
    if (obj->bits & 0x2000)
    {
        return 0;
    }
    if (chance < rand_below(100, mt19937_next()))
    {
        return 0;
    }
    obj->bits = (obj->bits & 0xFFF) | 0x2000;
    ((struct AnimFlags *)&obj->flags)->mode = 3;
    return 1;
}

s32 anim_roll_bit1000(struct AnimObj *obj, s32 chance)
{
    if (obj->bits & 0x1000)
    {
        return 0;
    }
    if (chance < rand_below(100, mt19937_next()))
    {
        return 0;
    }
    obj->bits = (obj->bits & 0xFFF) | 0x1000;
    ((struct AnimFlags *)&obj->flags)->mode = 3;
    return 1;
}

void anim_ensure_pose(struct AnimObj *obj)
{
    if ((obj->flags & 0x40) == 0)
    {
        obj->lowestPose = find_lowest_order_pose((u8 *)obj + 8);
        ((struct AnimFlags *)&obj->flags)->bit6 = 1;
    }
}

void anim_clear_flag_bits(struct AnimObj *obj, u32 mask)
{
    if (mask & 0xC)
    {
        obj->factor10A = 0x100;
    }
    if (mask & 0x30)
    {
        obj->factor10C = 0x100;
    }
    if (mask & 0xC0)
    {
        obj->factor11C = 0x100;
    }
    obj->bits &= ~mask;
}

void state_figure_wait(struct AnimTask *owner);
struct AnimTask *task_create_list_4c(void (*fn)(struct AnimTask *), void *obj);
/* A or B pressed: makes hero figure gWorld->fig80[slot] jump. Plays the buzzer (0x15F, unless
 * flags2C1 bit 3 mutes it) when that figure is missing or not under player control. The jump
 * only starts while the hero is in pose kind 0, 1 or 0xE showing his base animation (Mario 0x2000,
 * Luigi 0x204D): it sets flags0B bit 2 and starts the jump task (state_figure_wait) on his body. */
void figure_select_action(s32 slot)
{
    struct AnimObj *hero = gWorld->fig80[slot];
    struct AnimPose *pose;
    u8 kind;
    s16 heroIndex;

    if (hero == NULL)
    {
        if ((gWorld->flags2C1 & 8) == 0)
            set_slot_value(0x15F, -1);
        return;
    }
    /* control mode (bits 1-2) must be 1 */
    if ((hero->ctrl & 6) != 2)
    {
        if ((gWorld->flags2C1 & 8) == 0)
            set_slot_value(0x15F, -1);
        return;
    }
    pose = hero->pic.pose;
    kind = pose->kind21;
    if ((kind == 0 || kind == 0xE || kind == 1)
        && ((heroIndex = hero->index) != HERO_INDEX_MARIO || POSE_ANIM_ID(pose) == MARIO_ANIM_BASE)
        && (heroIndex != HERO_INDEX_LUIGI || POSE_ANIM_ID(pose) == LUIGI_ANIM_BASE))
    {
        ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 1;
        task_create_list_4c(state_figure_wait, (u8 *)hero + 8);
    }
}

void state_figure_wait(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;
    struct AnimPose *sprite;
    u8 kind;
    s16 index;

    ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 1;
    sprite = obj->pic.sprite;
    kind = sprite->kind21;
    if (!((kind == 0 || kind == 0xE || kind == 1)
          && ((index = *(s16 *)(obj->anim + 0xEC)) != -1
              || (sprite->unk16 | ((((u32)sprite->unk1E << 28) >> 16) + 0x1000)) == 0x2000)
          && (index != -2 || (sprite->unk16 | ((((u32)sprite->unk1E << 28) >> 16) + 0x1000)) == 0x204D)))
    {
        owner->state = 0;
        ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 0;
    }
    else
    {
        owner->state = state_figure_begin;
        owner->state(owner);
    }
}

/* Starts a figure's jump (task state after state_figure_wait): ends the task if the object is
 * busy with a state of its own; otherwise plays sound 0x2B, sets the jump's starting speed and
 * plays the jump action (5; the active hero uses 0xA unless bit 3 of its flags77 is set), then
 * moves on to state_figure_move. World.flags0B bit 2 stays set while the jump runs. */
void state_figure_begin(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;
    u32 (*isqrt)(u32);

    ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 1;
    if (obj->unk4C != 0)
    {
        ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 0;
        owner->state = 0;
        return;
    }
    set_slot_value(0x2B, -1);
    isqrt = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt);
    obj->stepB2 = ((s32)isqrt(0xB37D29) - FIGURE_JUMP_GRAVITY) / 2;
    if (obj == (struct AnimObj *)(gWorld->active + 8))
    {
        sprite_set_action(obj, (*(gWorld->active + 0x7F) & 8) ? 5 : 0xA, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 5, 0, 0);
    }
    ((struct AnimPoseFlags *)&obj->pic.sprite->flags)->mode = 1;
    owner->state = state_figure_move;
}

void state_figure_move(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;

    ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 1;
    if (obj->unk4C != 0)
    {
        ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 0;
        owner->state = 0;
        return;
    }
    obj->stepB2 -= 0xB3;
    obj->unk18 = obj->unk40 + (s16)obj->stepB2;
    if ((s16)obj->stepB2 > 0)
    {
        return;
    }
    if ((u8 *)obj != gWorld->active + 8)
    {
        sprite_set_action(obj, 6, 0, 0);
    }
    ((struct AnimPoseFlags *)&obj->pic.sprite->flags)->mode = 1;
    ((struct AnimFlags77 *)&obj->flags77)->low3 = 1;
    owner->state = state_figure_limit;
}

void state_figure_limit(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;

    ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 1;
    if (obj->unk4C != 0)
    {
        ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 0;
        owner->state = 0;
        return;
    }
    obj->stepB2 -= 0xB3;
    obj->unk18 = obj->unk40 + (s16)obj->stepB2;
    if (obj->unk18 > *(s32 *)(obj->anim + 0xE0))
    {
        return;
    }
    obj->unk58 = 0;
    ((struct AnimFlags77 *)&obj->flags77)->low3 = 0;
    obj->unk18 = *(s32 *)(obj->anim + 0xE0);
    if ((u8 *)obj != gWorld->active + 8)
    {
        sprite_set_action(obj, 8, 0, 0);
    }
    owner->state = state_figure_end;
}

void state_figure_end(struct AnimTask *owner)
{
    struct AnimObj *obj = owner->obj;

    ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 1;
    if (obj->unk4C != 0)
    {
        ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 0;
        owner->state = 0;
        return;
    }
    if (obj == (struct AnimObj *)(gWorld->active + 8))
    {
        u32 flagBits = (u32)*(gWorld->active + 0x7F) >> 3;
        u32 action = 1;

        action &= ~flagBits;
        sprite_set_action(obj, action, 0, 0);
    }
    else
    {
        if ((obj->pic.sprite->flags & 8) == 0)
        {
            return;
        }
        sprite_set_action(obj, 0, 0, 0);
    }
    ((struct WorldFlags *)&gWorld->flags0B)->bit2 = 0;
    owner->state = 0;
}

ASM_FUNC("asm/code_08109714/status_slots_setup.s", void status_slots_setup(void))
