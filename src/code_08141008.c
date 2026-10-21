#include "global.h"
#include "ram_routines.h"

struct EnemyRec
{
    u8 filler0[0x1A];
    u16 waitTimer;
    u8 filler1C[0x1E - 0x1C];
    u8 frames[4];
    u8 amp22;
    u8 filler23;
    u16 dist24;
    u16 rate26;
};

struct Bits3
{
    u8 field0 : 3;
    u8 rest : 5;
};

struct Bits4
{
    u8 low4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bits1
{
    u8 bit0 : 1;
    u8 field1 : 2;
    u8 rest : 5;
};

struct Behavior
{
    u8 filler0[0x17A];
    u16 timerB;
};

struct Sprite
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 frame20;
};

struct Owner
{
    u8 filler0[0x28];
    void *entries[4];
    u8 filler38[0x204 - 0x38];
    struct Actor *tgtB;
    u8 flags208;
};

struct Actor
{
    u8 filler0[0x2];
    u8 angle;
    u8 filler3[0xC - 0x3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u8 dir;
    u8 filler25[0x54 - 0x25];
    u8 group;
    u8 filler55[0x100 - 0x55];
    u16 table[0x20];
    u8 filler140[0x198 - 0x140];
    void *state;
    u8 filler19C[0x1A0 - 0x19C];
    void *handler;
    u8 filler1A4[0x1A8 - 0x1A4];
    void *prep;
    u8 filler1AC[0x1B0 - 0x1AC];
    struct EnemyRec *rec;
    u8 *sensor;
    u8 filler1B8[0x1D0 - 0x1B8];
    u16 index;
    u16 timerB;
    u16 timer;
    u8 flags1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDz;
    u8 filler1EC[0x20D - 0x1EC];
    u8 flags20D;
    u8 flags20E;
    u8 filler20F[0x214 - 0x20F];
    u8 mode;
    u8 filler215[0x23E - 0x215];
    u16 speed2;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x294 - 0x246];
    struct Sprite *sprite;
    u8 filler298[0x2D8 - 0x298];
    u16 moveSpeed;
    u8 filler2DA[0x2DE - 0x2DA];
    u16 unk2DE;
    u8 filler2E0[0x2EC - 0x2E0];
    s32 landZ;
    u8 filler2F0[0x2F4 - 0x2F0];
    u32 phase;
    s16 hopStep;
    u8 filler2FA[0x33E - 0x2FA];
    u8 flags33E;
};

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
u8 enemy_partner_in_sensor(u8 *owner, struct Actor *ent);
u8 step_pos_xy_c(struct Actor *ent);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void tick_store(void);
void enemy_wander_random_edge(void);
void enemy_aim_at_partner(struct Owner *owner, struct Actor *ent, void **out);
void enemy_switch_to_free_partner(void);
void obj_set_cell_coord(struct Actor *ent, u32 cell, u32 arg);
void enemy_touch_partner_or_step(struct Owner *owner, struct Actor *ent, void **out);
void enemy_move_to_partner_3d(struct Owner *owner, struct Actor *ent, void **out);
void enemy_move_home_3d(void);
void enemy_state_run_move(struct Owner *owner, struct Actor *ent);
void enemy_wait_sprite_timer(void);
void enemy_turn_to_partner(struct Owner *owner, struct Actor *ent, void **out);
u8 z_approach(struct Actor *ent);
void enemy_rand_setup(struct Actor *ent);
void enemy_partner_walk_step(struct Owner *owner, struct Actor *ent, void **out);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_step_e6a0(void *spr);
void enemy_timer_then_cell(u8 *owner, struct Actor *ent, void **out);
void enemy_prepare_move_to_partner(struct Owner *owner, struct Actor *ent, void **out);
void obj_set_sprite_link(struct Actor *ent, u32 zero);
void obj_set_active(struct Actor *ent, u32 zero);
void start_move_to(struct Actor *ent, s32 x, s32 y, s32 speed, ByteArg zero);
u8 step_pos_xy_plain(struct Actor *ent);
u16 angle_of_vector16(s32 x, s32 y);
void enemy_z_step_done_wait(void);
void collide_react(struct Owner *owner, struct Actor *ent, void *bro, u32 kick, u32 speed, s32 offX, s32 offY, u32 a, u32 b);
u8 find_active_entry(struct Owner *owner, u8 *idx);
u8 find_touching_partner(struct Owner *owner, struct Actor *ent, struct Actor **out);
void obj_box_to_stack(s16 *box, struct Actor *ent, u32 a, u32 b);
u8 point_in_box(s16 *box, void *entry, s32 a, s32 b);
u8 step_pos_xy(struct Actor *ent);
void start_move_to_3d(struct Actor *ent, s32 x, s32 y, s32 z, u32 speed, ByteArg zero);
u8 enemy_sensor_hit(struct Actor *ent, struct Actor *target);
void enemy_kick_after_probe(void);
u8 step_pos_xyz(struct Actor *ent);

ASM_FUNC("asm/code_08141008/enemy_approach_partner.s", void enemy_approach_partner(void))

ASM_FUNC("asm/code_08141008/enemy_init_group_random.s", void enemy_init_group_random(void))

ASM_FUNC("asm/code_08141008/enemy_state_run_move.s", void enemy_state_run_move(struct Owner *owner, struct Actor *ent))

void enemy_timer_then_cell(u8 *owner, struct Actor *ent, void **out)
{
    struct EnemyRec *rec = ent->rec;
    u16 *timer = &ent->timerB;
    s32 left = *timer - 1;

    *timer = left;
    if ((left << 16) == 0)
    {
        ((struct Bits3 *)&ent->mode)->field0 = 4;
        ((struct Bits1 *)&ent->sprite->flags12)->field1 = 1;
        ent->sprite->frame20 = ((u8 *)rec)[ent->index + 0x1E];
        {
            s32 cell = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->dir << 29) >> 29, 2);
            obj_set_cell_coord(ent, cell + 4, 0);
        }
        out[0] = enemy_switch_to_free_partner;
    }
}

ASM_FUNC("asm/code_08141008/enemy_switch_to_free_partner.s", void enemy_switch_to_free_partner(void))

void enemy_step_or_bump_update(struct Owner *owner, struct Actor *ent)
{
    struct Actor *e = ((struct Actor **)((u8 *)owner + ent->group * 4 + 0x28))[ent->timer];
    u8 *sensor = ent->sensor;
    s16 box[8];
    s32 nx = e->posX + e->vx;
    s32 ny = e->posY + e->vy;
    u8 i;

    if (e->unk2DE != 0)
    {
        for (i = 0; i < sensor[1]; i++)
        {
            obj_box_to_stack(box, ent, 0x22, i * 8 + 2);
            if ((e->posX < box[0] * 0x100 || e->posX > box[1] * 0x100 || e->posY < box[3] * 0x100 || e->posY > box[2] * 0x100)
                && (nx >= box[0] * 0x100 && nx <= box[1] * 0x100 && ny >= box[3] * 0x100 && ny <= box[2] * 0x100))
            {
                e->posX -= e->vx;
                e->posY -= e->vy;
                e->unk2DE = 0;
            }
        }
    }
    step_pos_xy_plain(e);
    if (z_approach(e) == 0)
    {
        *(u8 *)&e->flags20D |= 0x40;
        *(s8 *)&e->flags20E &= -5;
        obj_set_cell_coord(e, 0, 0);
        enemy_rand_setup(ent);
    }
}

ASM_FUNC("asm/code_08141008/enemy_prepare_move_to_partner.s", void enemy_prepare_move_to_partner(struct Owner *owner, struct Actor *ent, void **out))

ASM_FUNC("asm/code_08141008/enemy_state_wander_run.s", void enemy_state_wander_run(void))

void enemy_wait_then_wander(u8 *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    u16 *table = ent->table;
    struct EnemyRec *rec = ent->rec;
    u16 *timer = &ent->timer;
    s32 t = *timer;
    s32 next;

    if (t == 0)
    {
        if (enemy_partner_in_sensor(owner, ent))
        {
            out[0] = enemy_aim_at_partner;
            return;
        }
        next = rec->waitTimer;
    }
    else
    {
        next = t - 1;
    }
    *timer = next;
    if (step_pos_xy_c(ent))
    {
        return;
    }
    {
        u32 rnd = mt19937_next();
        s32 pick = rand_below(4, rnd);
        ai->timerB = table[pick + 0x14];
    }
    out[0] = tick_store;
    out[3] = enemy_wander_random_edge;
}

void enemy_aim_at_partner(struct Owner *owner, struct Actor *ent, void **out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    u8 idx;
    u32 index;
    struct Actor *partner;
    s32 pick;
    s32 dx;
    s32 dy;
    s32 dir;
    u32 (*math)(u32, u32);

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
    pick = rand_below(4, mt19937_next());
    *(u16 *)(ai + 0x178) = pick;
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    ent->sprite->frame20 = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x28];
    math = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
    dx = partner->posX - ent->posX;
    dy = partner->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        u16 angle = angle_of_vector16(dx, dy);
        dir = (((s32)(angle + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir = -1;
    }
    {
        u8 facing = math(dir, 2);
        obj_set_cell_coord(ent, facing + 4, 0);
    }
    ((struct Bits4 *)(ai + 0x17E))->bit4 = 0;
    ((struct Behavior *)ai)->timerB = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x2C];
    out[0] = enemy_move_to_partner_3d;
}

void enemy_move_to_partner_3d(struct Owner *owner, struct Actor *ent, void **out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    u8 idx;
    u32 index;
    struct Actor *partner;
    u32 (*math)(u32, u32);
    s32 dx;
    s32 dy;
    s32 dir;
    u8 facing;

    if (ent->timerB != 0)
    {
        if (find_active_entry(owner, &idx) != 0)
        {
            index = idx;
        }
        else
        {
            index = ((u32)owner->flags208 << 30) >> 31;
        }
        partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
        math = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
        dx = partner->posX - ent->posX;
        dy = partner->posY - ent->posY;
        if ((dx | dy) != 0)
        {
            u16 angle = angle_of_vector16(dx, dy);
            dir = (((s32)(angle + 0x1000)) >> 13) & 7;
        }
        else
        {
            dir = -1;
        }
        facing = math(dir, 2);
        if (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->dir << 29) >> 29, 2) != facing)
        {
            obj_set_cell_coord(ent, facing + 4, 0);
        }
        {
            s32 left = ((struct Behavior *)ai)->timerB - 1;
            ((struct Behavior *)ai)->timerB = left;
            if ((u16)left == 0)
            {
                start_move_to_3d(ent, partner->posX + (partner->vx << 4), partner->posY + (partner->vy << 4),
                                 partner->posZ + partner->dz, ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[0x18], (u8)0);
                *(s32 *)(ai + 0x184) = ent->posX;
                *(s32 *)(ai + 0x188) = ent->posY;
                *(s32 *)(ai + 0x18C) = ent->posZ;
                *(s32 *)(ai + 0x190) = ent->dz;
                *(u8 *)(ai + 0x17E) |= 0x10;
            }
        }
    }
    else
    {
        if (ent->flags1D6 & 0x10)
        {
            if (step_pos_xyz(ent) == 0)
            {
                *out = enemy_move_home_3d;
            }
        }
    }
}

NONMATCH("asm/code_08141008/enemy_collide_random_kick.s", void enemy_collide_random_kick(struct Owner *owner, struct Actor *ent, void **out))
{
    u16 *table = ent->table;
    u8 idx;
    s32 pick;
    s32 pick2;
    u16 speed;
    u16 kick;
    void *partner;
    u8 index;

    pick = rand_below(4, mt19937_next());
    ((struct Bits3 *)&ent->mode)->field0 = 4;
    {
        struct Sprite *sprite = ent->sprite;
        u16 *entry = &table[(s8)pick];
        sprite->frame20 = entry[4];
        speed = entry[0];
    }
    pick2 = rand_below(2, mt19937_next());
    kick = table[pick2 + 0x18];
    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = ((void **)((u8 *)owner + 0x28))[index];
    collide_react(owner, ent, partner, kick, speed, (s8)*((u8 *)table + 0x40), (s8)*((u8 *)table + 0x41), 0x22, 0);
    *out = enemy_touch_partner_or_step;
}
END_NONMATCH

void enemy_touch_partner_or_step(struct Owner *owner, struct Actor *ent, void **out)
{
    u8 *ai = (u8 *)ent + 0x58;
    u16 *table = ent->table;
    u16 *timer = &ent->timer;
    s32 t = *timer;
    struct Actor *partner;
    s16 box[8];

    if (t == 0)
    {
        if (find_touching_partner(owner, ent, &partner))
        {
            obj_box_to_stack(box, ent, 0x22, 0);
            if (point_in_box(box, ent, 0x7FFF, 0))
            {
                if (partner->posZ == ent->posZ)
                {
                    out[0] = enemy_turn_to_partner;
                    return;
                }
                if (partner != owner->tgtB && owner->tgtB->posZ == ent->posZ)
                {
                    out[0] = enemy_turn_to_partner;
                    return;
                }
            }
        }
        {
            struct EnemyRec *rec = *(struct EnemyRec **)(ai + 0x158);
            u16 reload = rec->waitTimer;
            *(u16 *)(ai + 0x17C) = reload;
        }
    }
    else
    {
        *timer = t - 1;
    }
    if (step_pos_xy(ent))
    {
        return;
    }
    {
        s32 pick = rand_below(4, mt19937_next());
        ((struct Behavior *)ai)->timerB = table[pick + 0x14];
    }
    out[0] = tick_store;
    out[3] = enemy_collide_random_kick;
}

ASM_FUNC("asm/code_08141008/enemy_turn_to_partner.s", void enemy_turn_to_partner(struct Owner *owner, struct Actor *ent, void **out))

ASM_FUNC("asm/code_08141008/enemy_start_partner_walk.s", void enemy_start_partner_walk(void))

ASM_FUNC("asm/code_08141008/enemy_partner_walk_step.s", void enemy_partner_walk_step(struct Owner *owner, struct Actor *ent, void **out))
