#include "global.h"

struct AnimPose
{
    u8 filler0[0x12];
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F[0x21 - 0x1F];
    u8 kind21;
};

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

s32 anim_dispatch(struct AnimObj *obj, u32 flags, s32 value, u32 extra)
{
    if (flags & 1)
    {
        return anim_bit1_chance(obj, value);
    }
    if (flags & 2)
    {
        return anim_bit2_chance(obj, value);
    }
    if (flags & 0xC)
    {
        return anim_set_motion_a(obj, value, extra);
    }
    if (flags & 0xC0)
    {
        return anim_set_motion_c(obj, value, extra);
    }
    if (flags & 0x30)
    {
        return anim_set_motion_b(obj, value, extra);
    }
    if (flags & 0x100)
    {
        return anim_bit100_step(obj, value);
    }
    if (flags & 0x200)
    {
        return anim_bit200_step(obj, value);
    }
    if (flags & 0x400)
    {
        return anim_bit400_step(obj, value);
    }
    if (flags & 0x800)
    {
        return anim_chance_start(obj, value);
    }
    if (flags & 0x1000)
    {
        return anim_roll_bit1000(obj, value);
    }
    if (flags & 0x2000)
    {
        return anim_roll_bit2000(obj, value);
    }
    if (flags & 0x4000)
    {
        return anim_roll_bit4000(obj, value);
    }
    if (flags & 0x8000)
    {
        return anim_roll_bit8000(obj, value);
    }
    return 0;
}

ASM_FUNC("asm/code_08108AC8/anim_copy_state.s", void anim_copy_state(void))

s32 anim_bit1_chance(struct AnimObj *obj, s32 value)
{
    u8 *party;
    u8 *entry;
    s32 actor;
    u8 kind;
    s32 left;
    s32 roll;
    s32 ok;

    if (obj->bits & 1)
    {
        return 0;
    }
    if (obj->index < 0)
    {
        party = gParty;
        actor = ~obj->index;
        entry = party + (((actor << 4) - actor) << 2);
        kind = entry[0x37];
        if (kind != 0xF)
        {
            if (kind == 0x11)
            {
                value -= 0x3C;
            }
        }
        else
        {
            value -= 0x1E;
        }
        roll = rand_below(100, mt19937_next());
        if (value < roll)
        {
            return 0;
        }
    }
    else
    {
        left = 100 - gPoseChancePct[(obj->part[0x1C] >> 2) & 3];
        if (left == 100)
        {
            return 0;
        }
        if (left != 0)
        {
            s32 margin = value - left / 3;

            roll = rand_below(100, mt19937_next());
            ok = 1;
            if (margin < roll)
            {
                ok = 0;
            }
            if (ok == 0)
            {
                return 0;
            }
        }
        if ((obj->bits & 0x300) == 0)
        {
            obj->unk128 = 0xFF;
            obj->unk126 = 1;
        }
    }
    obj->bits = (obj->bits & 0xFFFC) | 1;
    ((struct AnimFlags77 *)&obj->unk120)->low3 = 4;
    return 1;
}

s32 anim_bit2_chance(struct AnimObj *obj, s32 value)
{
    u8 *party;
    u8 *entry;
    s32 actor;
    u8 kind;
    s32 left;
    s32 roll;
    s32 ok;

    if (obj->bits & 2)
    {
        return 0;
    }
    if (obj->index < 0)
    {
        party = gParty;
        actor = ~obj->index;
        entry = party + (((actor << 4) - actor) << 2);
        kind = entry[0x37];
        if (kind != 0xF)
        {
            if (kind == 0x11)
            {
                value -= 0x3C;
            }
        }
        else
        {
            value -= 0x1E;
        }
        roll = rand_below(100, mt19937_next());
        if (value < roll)
        {
            return 0;
        }
    }
    else
    {
        left = 100 - gPoseChancePct[(obj->part[0x1C] >> 2) & 3];
        if (left == 100)
        {
            return 0;
        }
        if (left != 0)
        {
            s32 margin = value - left / 3;

            roll = rand_below(100, mt19937_next());
            ok = 1;
            if (margin < roll)
            {
                ok = 0;
            }
            if (ok == 0)
            {
                return 0;
            }
        }
        if ((obj->bits & 0x300) == 0)
        {
            obj->unk128 = 0xFF;
            obj->unk126 = 1;
        }
    }
    obj->bits = (obj->bits & 0xFFFC) | 2;
    ((struct AnimFlags77 *)&obj->unk120)->low3 = 0;
    return 1;
}


ASM_FUNC("asm/code_08108AC8/anim_bit100_step.s", s32 anim_bit100_step(struct AnimObj *obj, s32 value))
