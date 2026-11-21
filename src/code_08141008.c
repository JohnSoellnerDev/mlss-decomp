#include "global.h"
#include "ram_routines.h"

struct HalfBit5
{
    u16 lo : 5;
    u16 bit5 : 1;
    u16 hi : 10;
};

struct HalfBit2
{
    u16 lo : 2;
    u16 bit2 : 1;
    u16 hi : 13;
};

struct HalfBit4
{
    u16 lo : 4;
    u16 bit4 : 1;
    u16 hi : 11;
};

struct EnemyRec
{
    u8 filler0[0x1A];
    u16 waitTimer;
    u8 filler1C[0x1E - 0x1C];
    u8 frames[4];
    u8 bobAmp;        /* amplitude of the hover bob (times sin / 64) */
    u8 filler23;
    u16 hoverHeight;  /* hover height above the ground, in 16 px units */
    u16 hoverSpeed;   /* how fast the hover height is approached, per frame */
    u8 aimFrame[4];   /* sprite frame while aiming, per variant */
    u8 aimTime[4];    /* frames spent aiming before the leap, per variant */
    u16 leapSpeed[4]; /* speed of the leap at the hero, per variant */
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

/* View of an Actor from +0x58 (the enemy AI's base pointer): index is Actor.index, the
 * variant (0-3) that picks entries of the EnemyRec's per-variant tables. */
struct Behavior
{
    u8 filler0[0x158];
    struct EnemyRec *rec;   /* Actor.rec */
    u8 filler15C[0x178 - 0x15C];
    u16 index;
    u16 timerB;
    u16 timer;              /* Actor.timer */
    u8 flags;
    u8 filler17F[0x184 - 0x17F];
    s32 homeX;  /* where the leap started (Actor.homeX..homeDz) */
    s32 homeY;
    s32 homeZ;
    s32 homeDz;
};

/* Behavior.flags / Actor.flags1D6: a move started by start_move_to is in progress */
#define ENEMY_AI_MOVING 0x10

struct Sprite
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 frame20;
    u8 filler21[0x2C - 0x21];
    s16 unk2C;
};

struct ByteLo2
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct ModeBits
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct Byte56
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
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
    u8 filler1EC[0x20C - 0x1EC];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 filler20F[0x212 - 0x20F];
    u16 mode212;
    u8 mode;
    u8 filler215[0x23E - 0x215];
    u16 speed2;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x290 - 0x246];
    u8 *layer290;
    struct Sprite *sprite;
    u8 filler298[0x2D8 - 0x298];
    s16 moveSpeed;
    u8 filler2DA[0x2DE - 0x2DA];
    u16 unk2DE;
    u8 filler2E0[0x2EC - 0x2E0];
    s32 landZ;
    u8 filler2F0[0x2F4 - 0x2F0];
    u32 phase;
    s16 hopStep;
    u8 filler2FA[0x33C - 0x2FA];
    u8 flags33C;
    u8 filler33D;
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
void enemy_switch_to_free_partner(struct Owner *owner, struct Actor *ent, void **out);
void enemy_activate(void *owner, struct Actor *ent, void **out);
u8 probe_body_hit(struct Owner *owner, struct Actor *body, s32 x, s32 y, s32 z, u32 a, u32 b);
u8 boxes_overlap(s16 *bounds, struct Actor *a, struct Actor *b, s32 zAbove, s32 zBelow);
void obj_set_or_add_pos(struct Actor *ent, s32 x, s32 y, s32 z, u32 a, u32 b);
void start_z_move(struct Actor *ent, s32 z, u16 speed, u8 rel, s32 zLimit);
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);
u8 *obj_probe_tile_heights(struct Actor *obj, s32 x, s32 y, s32 *h0, s32 *h1, s32 *h2);
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
void enemy_start_return_home(void *owner, struct Actor *ent, void **out);
u8 step_pos_xyz(struct Actor *ent);

/* Enemy AI step: turn toward the active hero and walk to it at the variant's speed (or, if its
 * sensor already sees the hero, wait a random time and kick); once the walk ends, wait the
 * variant's time before kicking. */
void enemy_approach_partner(struct Owner *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    u16 *table = ent->table;
    struct EnemyRec *rec = ent->rec;
    u8 *sensor = ent->sensor;
    u8 activeIdx;
    u32 index;
    struct Actor *partner;
    s32 dx;
    s32 dy;
    u16 hit;
    s32 pick;
    u16 wait;
    u16 speed;
    s32 flags;

    if ((ent->flags1D6 & ENEMY_AI_MOVING) == 0)
    {
        if (z_approach(ent) != 0)
        {
            return;
        }
        if (find_active_entry(owner, &activeIdx) != 0)
        {
            index = activeIdx;
        }
        else
        {
            index = ((u32)owner->flags208 << 30) >> 31;
        }
        partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
        dx = partner->posX - ent->posX;
        dy = partner->posY - ent->posY;
        if ((dx | dy) != 0)
        {
            u16 angle = angle_of_vector16(dx, dy);
            ent->angle = (u32)(angle + 0x10) >> 8;
        }
        else
        {
            ent->angle = 0xFF;
        }
        ((struct Bits3 *)&ent->mode)->field0 = 3;
        hit = *(u16 *)(sensor + 8);
        if (hit != 0)
        {
            flags = ent->flags20D;
            flags |= 0x40;
            ent->flags20D = flags;
            hit = enemy_sensor_hit(ent, partner);
            if (hit != 0)
            {
                pick = rand_below(4, mt19937_next());
                wait = table[pick + 0x14];
                ai->timerB = wait;
                out[0] = tick_store;
                out[3] = enemy_kick_after_probe;
                return;
            }
            ent->sprite->frame20 = ((u8 *)rec)[ai->index + 0x32];
            speed = ((u16 *)((u8 *)rec + ai->index * 2))[0x1B];
            start_move_to(ent, partner->posX, partner->posY, speed, (u8)hit);
        }
        else
        {
            ent->sprite->frame20 = ((u8 *)rec)[ai->index + 0x32];
            speed = ((u16 *)((u8 *)rec + ai->index * 2))[0x1B];
            start_move_to(ent, partner->posX, partner->posY, speed, (u8)hit);
        }
        /* ai->flags, written as a plain byte for the match */
        *((u8 *)ai + 0x17E) |= ENEMY_AI_MOVING;
    }
    else
    {
        if (step_pos_xy_plain(ent) == 0)
        {
            wait = ((u8 *)rec)[ent->index + 0x3E];
            ent->timerB = wait;
            out[0] = tick_store;
            out[3] = enemy_kick_after_probe;
        }
    }
}

NONMATCH("asm/code_08141008/enemy_init_group_random.s", void enemy_init_group_random(struct Owner *owner, struct Actor *ent))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    u8 *sensor = ent->sensor;
    u8 n;
    s32 pick;
    u16 timerVal;

    ((struct HalfBit4 *)&ent->flags33E)->bit4 = 1;
    for (n = 1; n < sensor[0] + 1; n++)
    {
        struct Actor *sibling = ((struct Actor **)((u8 *)owner + ent->group * 4 + 0x28))[n];
        ((struct Bits4 *)&sibling->flags33E)->bit4 = 0;
        obj_set_sprite_link(sibling, 0);
        obj_set_active(sibling, 0);
    }
    pick = rand_below(4, mt19937_next());
    *(u16 *)(ai + 0x178) = pick;
    timerVal = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x16];
    *(u16 *)(ai + 0x17A) = timerVal;
    ent->speed2 = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[3];
    ent->moveSpeed = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[3];
    ent->landZ = ent->posZ + ent->dz;
    ent->phase = 0;
    ent->hopStep = ((u8 *)rec)[1];
    ((struct HalfBit2 *)&ent->flags20E)->bit2 = 1;
    ((struct HalfBit5 *)&ent->flags20D)->bit5 = 1;
    *(void **)(ai + 0x140) = enemy_state_run_move;
    *(void **)(ai + 0x148) = enemy_timer_then_cell;
    *(void **)(ai + 0x150) = enemy_prepare_move_to_partner;
}
END_NONMATCH

ASM_FUNC("asm/code_08141008/enemy_state_run_move.s", void enemy_state_run_move(struct Owner *owner, struct Actor *ent))

/* Enemy AI step: counts timerB down. When it runs out the enemy goes to mode 4, its sprite plays
 * once at the variant's rate (EnemyRec.frames, byte 0x1E + variant), it shows frame group 1
 * facing its direction (the 8-way facing halved to four directions) and
 * enemy_switch_to_free_partner takes over. */
void enemy_timer_then_cell(u8 *owner, struct Actor *ent, void **out)
{
    struct EnemyRec *rec = ent->rec;
    u16 *timer = &ent->timerB;
    s32 left = *timer - 1;

    *timer = left;
    if ((left << 16) == 0)
    {
        ((struct Bits3 *)&ent->mode)->field0 = 4;
        ((struct Bits1 *)&ent->sprite->flags12)->field1 = 1; /* play the animation once */
        ent->sprite->frame20 = ((u8 *)rec)[ent->index + 0x1E];
        {
            s32 dir4 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->dir << 29) >> 29, 2);
            obj_set_cell_coord(ent, dir4 + 4, 0);
        }
        out[0] = enemy_switch_to_free_partner;
    }
}

NONMATCH("asm/code_08141008/enemy_switch_to_free_partner.s", void enemy_switch_to_free_partner(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    u8 *sensor = ent->sensor;
    u8 idx;
    u32 index;
    struct Actor *target;
    struct Actor *cand;
    s16 box[8];
    u8 k;
    u8 limit;
    s32 dx = 0;
    s32 dy = 0;
    u8 dirAfter;
    s8 tdx;
    s32 tdy;
    s32 x;
    s32 y;
    s32 hA;
    s32 hB;
    s32 hC;
    u8 *tile;
    s32 tileX;
    s32 tileY;
    u32 found = 0;

    if (ent->sprite->unk2C <= 0x5F)
    {
        return;
    }
    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    target = ((struct Actor **)((u8 *)owner + 0x28))[index];
    obj_box_to_stack(box, ent, 0x11, (u16)(*(u16 *)(ai + 0x178) + 0x22));
    if (boxes_overlap(box, ent, target, 0x7FFF, 0x7FFF) == 0)
    {
        return;
    }
    for (k = 0; k < sensor[1]; k++)
    {
        obj_box_to_stack(box, ent, 0x22, k * 8 + 2);
        if (point_in_box(box, target, 0x7FFF, 0))
        {
            return;
        }
        if (point_in_box(box, ent, 0x7FFF, 0))
        {
            return;
        }
    }
    *(u16 *)(ai + 0x17C) = 0;
    limit = sensor[0] + 1;
    for (k = 1; k < limit; k++)
    {
        cand = ((struct Actor **)((u8 *)owner + ent->group * 4 + 0x28))[k];
        if (cand->flags20C & 0x02000001)
        {
            continue;
        }
        if ((cand->mode212 & 0x180) == 0x80)
        {
            continue;
        }
        if ((cand->mode212 & 0x180) == 0x100)
        {
            continue;
        }
        found = 1;
        break;
    }
    if (found == 0)
    {
        return;
    }

    dirAfter = 0xFF;
    switch (target->dir & 7)
    {
    case 0:
        dx = 0;
        dy = -0x2000;
        dirAfter = 0;
        break;
    case 1:
        dx = 0x2000;
        dy = -0x2000;
        dirAfter = 1;
        break;
    case 2:
        dx = 0x2000;
        dy = 0;
        dirAfter = 2;
        break;
    case 3:
        dx = 0x2000;
        dy = 0x2000;
        dirAfter = 3;
        break;
    case 4:
        dx = 0;
        dy = 0x2000;
        dirAfter = 4;
        break;
    case 5:
        dx = -0x2000;
        dy = 0x2000;
        dirAfter = 5;
        break;
    case 6:
        dx = -0x2000;
        dy = 0;
        dirAfter = 6;
        break;
    case 7:
        dx = -0x2000;
        dy = -0x2000;
        dirAfter = 7;
        break;
    }
    switch (dirAfter)
    {
    case 0:
        tdx = 0;
        tdy = -1;
        break;
    case 1:
        tdx = 1;
        tdy = -1;
        break;
    case 2:
        tdx = 1;
        tdy = 0;
        break;
    case 3:
        tdx = 1;
        tdy = 1;
        break;
    case 4:
        tdx = 0;
        tdy = 1;
        break;
    case 5:
        tdx = -1;
        tdy = 1;
        break;
    case 6:
        tdx = -1;
        tdy = 0;
        break;
    case 7:
        tdx = -1;
        tdy = -1;
        break;
    default:
        tdx = 0;
        tdy = 0;
        break;
    }
    if (target->vx != 0)
    {
        dx *= 4;
    }
    if (target->vy != 0)
    {
        dy *= 4;
    }

    tile = tile_query(ent->layer290, target->posX + dx, target->posY + dy, &hA, &hB, &hC, (u8)0, (u8)1);
    if (probe_body_hit(owner, cand, target->posX + dx, target->posY + dy, 0, 1, 0) != 0)
    {
        return;
    }
    if (*tile & 0x80)
    {
        return;
    }
    tileX = target->posX;
    if (tileX < 0)
    {
        tileX += 0xFFF;
    }
    tileX = (tileX >> 12) + tdx;
    tileY = ent->posY;
    if (tileY < 0)
    {
        tileY += 0xFFF;
    }
    tileY = (tileY >> 12) + tdy;
    tile = obj_probe_tile_heights(ent, tileX, tileY, &hA, &hB, &hC);
    if (*tile & 0x80)
    {
        return;
    }

    x = target->posX + dx;
    y = target->posY + dy;
    for (k = 0; k < sensor[1]; k++)
    {
        obj_box_to_stack(box, ent, 0x22, k * 8 + 2);
        if (x >= box[0] * 0x100 && x <= box[1] * 0x100 && y >= box[3] * 0x100 && y <= box[2] * 0x100)
        {
            return;
        }
    }
    x = target->posX + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx, 2);
    y = target->posY + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dy, 2);
    for (k = 0; k < sensor[1]; k++)
    {
        obj_box_to_stack(box, ent, 0x22, k * 8 + 2);
        if (x >= box[0] * 0x100 && x <= box[1] * 0x100 && y >= box[3] * 0x100 && y <= box[2] * 0x100)
        {
            return;
        }
    }

    cand->state = enemy_activate;
    obj_set_active(cand, 1);
    obj_set_sprite_link(cand, 1);
    cand->state = enemy_activate;
    ((struct Byte56 *)&cand->flags20D)->bit6 = 0;
    ((struct ByteLo2 *)&cand->flags20E)->bit2 = 1;
    ((struct Byte56 *)&cand->flags20D)->bit5 = 0;
    obj_set_or_add_pos(cand, ent->posX, ent->posY + 0x200, ent->posZ + ent->dz + 0x1000, 1, 0);
    start_z_move(cand, cand->posZ + cand->dz + 0x180, 0xA0, 0, 0);
    *(u16 *)(ai + 0x17C) = k;
    start_move_to(cand, target->posX + dx, target->posY + dy, 0x180, (u8)0);
    cand->flags20C |= 0xE0;
    ((struct Bits3 *)&cand->flags33C)->field0 = 0;
    ((struct Bits4 *)&cand->sprite->flags12)->bit4 = 0;
    ((struct ModeBits *)&cand->mode)->bit3 = 0;
    ((struct ModeBits *)&cand->mode)->lo2 = 3;
    obj_load_record(cand->sprite, 3, 0, 0, 0);
    sprite_step_e6a0(cand->sprite);

    ((struct Bits3 *)&ent->sprite->flags12)->field0 = 0;
    obj_set_cell_coord(ent, MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->dir << 29) >> 29, 2), 0);
    *(u16 *)(ai + 0x17A) = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x16];
    *out = enemy_timer_then_cell;
}
END_NONMATCH

/* Moves the bro the enemy bumped (owner's entry group + timer) by its velocity. While its push count (unk2DE)
 * runs, a step that would carry it from outside into one of the enemy's sensor boxes is undone and the push
 * ends. When the bro's height move is done (z_approach) its flags20D bit 6 is set, flags20E bit 2 cleared, its
 * cell reset and the enemy picks a new move. */
void enemy_step_or_bump_update(struct Owner *owner, struct Actor *ent)
{
    struct Actor *bro = ((struct Actor **)((u8 *)owner + ent->group * 4 + 0x28))[ent->timer];
    u8 *sensor = ent->sensor;
    s16 box[8];
    s32 nextX = bro->posX + bro->vx;
    s32 nextY = bro->posY + bro->vy;
    u8 i;

    if (bro->unk2DE != 0)
    {
        for (i = 0; i < sensor[1]; i++)
        {
            obj_box_to_stack(box, ent, 0x22, i * 8 + 2);
            if ((bro->posX < box[0] * 0x100 || bro->posX > box[1] * 0x100 || bro->posY < box[3] * 0x100 || bro->posY > box[2] * 0x100)
                && (nextX >= box[0] * 0x100 && nextX <= box[1] * 0x100 && nextY >= box[3] * 0x100 && nextY <= box[2] * 0x100))
            {
                bro->posX -= bro->vx;
                bro->posY -= bro->vy;
                bro->unk2DE = 0;
            }
        }
    }
    step_pos_xy_plain(bro);
    if (z_approach(bro) == 0)
    {
        *(u8 *)&bro->flags20D |= 0x40;
        *(s8 *)&bro->flags20E &= -5;
        obj_set_cell_coord(bro, 0, 0);
        enemy_rand_setup(ent);
    }
}

NONMATCH("asm/code_08141008/enemy_prepare_move_to_partner.s", void enemy_prepare_move_to_partner(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    u8 *sensor = ent->sensor;
    u8 idx;
    u32 index;
    struct Actor *partner;
    s16 box[8];
    u16 speed;
    s32 dx;
    s32 dy;

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
    for (idx = 0; idx < sensor[1]; idx++)
    {
        obj_box_to_stack(box, ent, 0x22, idx * 8 + 2);
        if (point_in_box(box, partner, 0x7FFF, 0))
        {
            return;
        }
    }
    if (*out == enemy_switch_to_free_partner)
    {
        speed = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[7];
    }
    else
    {
        speed = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[3];
    }
    ent->moveSpeed = speed;
    dx = partner->posX - ent->posX;
    dy = partner->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        u16 angle = angle_of_vector16(dx, dy);
        ent->angle = (u32)(angle + 0x10) >> 8;
    }
    else
    {
        ent->angle = 0xFF;
    }
    start_move_to(ent, partner->posX, partner->posY, ent->moveSpeed, (u8)0);
    step_pos_xy_plain(ent);
}
END_NONMATCH

/* Enemy state for a hovering enemy: runs its current AI handler, then (unless it is aiming at,
 * moving to or returning from a hero) bobs it up and down on a sine wave above landZ and moves
 * landZ towards the record's hover height above the ground (posZ) at the record's hover speed. */
void enemy_state_wander_run(struct Owner *owner, struct Actor *ent)
{
    struct EnemyRec *rec = ent->rec;
    void **handler = (void **)&ent->handler;
    void *next;
    u8 bobAmp;
    s32 *baseZ;
    s32 height;
    s32 bob;
    s32 phaseStep;
    s32 bobSpeed;
    s8 frame;
    u16 hoverHeight;
    u16 hoverSpeed;
    s32 targetHeight;

    ((void (*)(struct Owner *, struct Actor *, void **))*handler)(owner, ent, handler);
    next = *handler;
    if (next == enemy_aim_at_partner || next == enemy_move_to_partner_3d || next == enemy_move_home_3d
        || next == enemy_z_step_done_wait)
    {
        return;
    }
    bobAmp = rec->bobAmp;
    baseZ = &ent->landZ;
    height = *baseZ - ent->posZ;
    bob = gSineTable[ent->phase & 0xFF] / 0x40;
    ent->dz = height + bobAmp * bob;
    /* the bob's phase advances with the sprite's frame */
    bobSpeed = ent->hopStep;
    frame = ent->sprite->frame20;
    phaseStep = frame * bobSpeed + 8;
    ent->phase = ((phaseStep / 0x10) + ent->phase) & 0xFF;
    hoverHeight = rec->hoverHeight;
    hoverSpeed = rec->hoverSpeed;
    targetHeight = hoverHeight << 12;
    if (*baseZ - ent->posZ > targetHeight)
    {
        *baseZ -= hoverSpeed;
        if (*baseZ - ent->posZ <= targetHeight)
            *baseZ = ent->posZ + targetHeight;
    }
    else if (*baseZ - ent->posZ < targetHeight)
    {
        *baseZ += hoverSpeed;
        if (*baseZ - ent->posZ >= targetHeight)
            *baseZ = ent->posZ + targetHeight;
    }
}

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

/* Enemy AI step: the hero has come into view. Picks a random variant (0-3), shows its aiming
 * frame (EnemyRec.aimFrame), turns to face the hero in front (or the leading one) and aims
 * for the variant's aim time (EnemyRec.aimTime) before enemy_move_to_partner_3d leaps. */
void enemy_aim_at_partner(struct Owner *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    struct EnemyRec *rec = ent->rec;
    u8 activeIdx;
    u32 index;
    struct Actor *partner;
    s32 variant;
    s32 dx;
    s32 dy;
    s32 dir;
    u32 (*divide)(u32, u32);

    /* the hero in front, or the leading one (flags208 bit 1) */
    if (find_active_entry(owner, &activeIdx) != 0)
    {
        index = activeIdx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
    variant = rand_below(4, mt19937_next());
    ai->index = variant;
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    /* rec->aimFrame[ai->index] */
    ent->sprite->frame20 = ((u8 *)rec)[ai->index + 0x28];
    divide = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
    dx = partner->posX - ent->posX;
    dy = partner->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        /* one of 8 directions */
        u16 angle = angle_of_vector16(dx, dy);
        dir = (((s32)(angle + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir = -1;
    }
    {
        u8 facing = divide(dir, 2);
        obj_set_cell_coord(ent, facing + 4, 0);
    }
    ((struct Bits4 *)&ai->flags)->bit4 = 0; /* not ENEMY_AI_MOVING */
    /* rec->aimTime[ai->index] */
    ai->timerB = ((u8 *)rec)[ai->index + 0x2C];
    out[0] = enemy_move_to_partner_3d;
}

/* Enemy AI step: while aiming (timerB counts down), face the active hero; when it runs out, leap
 * in 3D at where the hero will be in 16 frames, at the variant's leap speed (EnemyRec.leapSpeed),
 * remembering the start spot. Once the leap is over, head back there (enemy_move_home_3d). */
void enemy_move_to_partner_3d(struct Owner *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    struct EnemyRec *rec = ent->rec;
    u8 activeIdx;
    u32 index;
    struct Actor *partner;
    u32 (*divide)(u32, u32);
    s32 dx;
    s32 dy;
    s32 dir;
    u8 facing;

    if (ent->timerB != 0)
    {
        /* the hero in front, or the leading one (flags208 bit 1) */
        if (find_active_entry(owner, &activeIdx) != 0)
        {
            index = activeIdx;
        }
        else
        {
            index = ((u32)owner->flags208 << 30) >> 31;
        }
        partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
        divide = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
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
        facing = divide(dir, 2);
        if (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->dir << 29) >> 29, 2) != facing)
        {
            obj_set_cell_coord(ent, facing + 4, 0);
        }
        {
            s32 left = ai->timerB - 1;
            ai->timerB = left;
            if ((u16)left == 0)
            {
                start_move_to_3d(ent, partner->posX + (partner->vx << 4), partner->posY + (partner->vy << 4),
                                 partner->posZ + partner->dz, ((u16 *)((u8 *)rec + ai->index * 2))[0x18], (u8)0);
                ai->homeX = ent->posX;
                ai->homeY = ent->posY;
                ai->homeZ = ent->posZ;
                ai->homeDz = ent->dz;
                *(u8 *)&ai->flags |= ENEMY_AI_MOVING;
            }
        }
    }
    else
    {
        if (ent->flags1D6 & ENEMY_AI_MOVING)
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

/* Enemy AI step: when its timer has run out and a hero touches it inside its 0x22 box on its own
 * height (or the other hero is on its height), turn to that hero (enemy_turn_to_partner); else the
 * timer restarts at the record's wait time. Meanwhile it keeps walking; once the walk ends it waits
 * a random variant's time (table[0x14..0x17]) and then kicks (enemy_collide_random_kick). */
void enemy_touch_partner_or_step(struct Owner *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    u16 *table = ent->table;
    u16 *timer = &ent->timer;
    s32 left = *timer;
    struct Actor *partner;
    s16 box[8];

    if (left == 0)
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
            struct EnemyRec *rec = ai->rec;
            u16 wait = rec->waitTimer;
            ai->timer = wait;
        }
    }
    else
    {
        *timer = left - 1;
    }
    if (step_pos_xy(ent))
    {
        return;
    }
    {
        s32 pick = rand_below(4, mt19937_next());
        ai->timerB = table[pick + 0x14];
    }
    out[0] = tick_store;
    out[3] = enemy_collide_random_kick;
}

/* Enemy AI step: turn to face the active hero (the 8-way direction to it, halved to the
 * sprite's 4-way facing), pick a random variant, show its frame and set its wait time, then
 * play the sprite and hand over to enemy_wait_sprite_timer. */
void enemy_turn_to_partner(struct Owner *owner, struct Actor *ent, void **out)
{
    struct Behavior *ai = (struct Behavior *)((u8 *)ent + 0x58);
    struct EnemyRec *rec = ent->rec;
    u8 activeIdx;
    u32 index;
    struct Actor *partner;
    u32 (*divide)(u32, u32);
    s32 dx;
    s32 dy;
    s32 dir8;
    u8 facing;
    s32 pick;
    u32 zero;
    u16 wait;
    u16 *variant;

    if (find_active_entry(owner, &activeIdx) != 0)
    {
        index = activeIdx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
    divide = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div);
    dx = partner->posX - ent->posX;
    dy = partner->posY - ent->posY;
    if ((dx | dy) != 0)
    {
        u16 angle = angle_of_vector16(dx, dy);
        dir8 = (((s32)(angle + 0x1000)) >> 13) & 7;
    }
    else
    {
        dir8 = -1;
    }
    facing = divide(dir8, 2);
    if (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->dir << 29) >> 29, 2) != facing)
    {
        obj_set_cell_coord(ent, facing, 0);
    }
    pick = rand_below(4, mt19937_next());
    variant = &ai->index;
    zero = 0;
    *variant = pick;
    wait = ((u8 *)rec)[*variant + 0x26];
    ai->timerB = wait;
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    ent->sprite->frame20 = ((u8 *)rec)[*variant + 0x22];
    obj_load_record(ent->sprite, 3, -1, 0, zero);
    sprite_step_e6a0(ent->sprite);
    *out = enemy_wait_sprite_timer;
}

NONMATCH("asm/code_08141008/enemy_start_partner_walk.s", void enemy_start_partner_walk(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    u16 *ip = (u16 *)((u8 *)ent + 0x1D0);
    u8 idx;
    u32 index;
    struct Actor *partner;
    u16 timerVal;
    u16 speed;

    timerVal = ((u8 *)rec)[*ip + 0x2E];
    ent->timerB = timerVal;
    obj_load_record(ent->sprite, 3, -1, 0, 0);
    sprite_step_e6a0(ent->sprite);
    ent->sprite->frame20 = ((u8 *)rec)[*ip + 0x2A];
    ((struct Bits1 *)&ent->sprite->flags12)->field1 = 0;
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = ((u32)owner->flags208 << 30) >> 31;
    }
    partner = ((struct Actor **)((u8 *)owner + 0x28))[index];
    ((struct Bits3 *)&ent->mode)->field0 = 1;
    obj_load_record(ent->sprite, 3, -1, 0, 0);
    sprite_step_e6a0(ent->sprite);
    speed = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[0x1D];
    start_move_to(ent, partner->posX, partner->posY, speed, (u8)0);
    *(s32 *)(ai + 0x184) = ent->posX;
    *(s32 *)(ai + 0x188) = ent->posY;
    *(s32 *)(ai + 0x18C) = ent->posZ;
    *(s32 *)(ai + 0x190) = ent->dz;
    ((struct Bits1 *)(ai + 0x17E))->bit0 = 0;
    ((struct Bits4 *)(ai + 0x17E))->bit4 = 0;
    *out = enemy_partner_walk_step;
}
END_NONMATCH

NONMATCH("asm/code_08141008/enemy_partner_walk_step.s", void enemy_partner_walk_step(struct Owner *owner, struct Actor *ent, void **out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct EnemyRec *rec = ent->rec;
    struct Actor *partner;
    u8 idx;
    s16 box[8];
    u32 (*math)(u32, u32);
    s32 dx;
    s32 dy;
    s32 dir;
    u32 facing;
    u32 speed;
    s32 timerVal;
    s32 flags;
    s32 bit;
    u8 zero;
    s32 x;
    s32 y;
    s32 inside;

    if (ent->flags1D6 & 1)
    {
        if (ent->sprite->flags12 & 8)
        {
            timerVal = ent->timerB - 1;
            ent->timerB = timerVal;
            if ((timerVal << 16) == 0)
            {
                ent->timerB = (s8)((u8 *)rec)[ent->index + 0x2E];
                ((struct Bits3 *)&ent->mode)->field0 = 3;
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
                obj_set_cell_coord(ent, facing, 0);
                ((struct Bits1 *)(ai + 0x17E))->bit0 = 0;
            }
        }
        return;
    }
    timerVal = ent->timerB - 1;
    ent->timerB = timerVal;
    if ((timerVal << 16) == 0)
    {
        obj_box_to_stack(box, ent, 0x22, 0);
        inside = point_in_box(box, ent, 0x7FFF, 0);
        if (!inside)
        {
            *out = enemy_start_return_home;
            return;
        }
        if (find_active_entry(owner, &idx) == 0)
        {
            *out = enemy_start_return_home;
            return;
        }
        partner = ((struct Actor **)((u8 *)owner + 0x28))[idx];
        if (partner->posZ != ent->posZ)
        {
            if (partner != owner->tgtB && owner->tgtB->posZ != ent->posZ)
            {
                *out = enemy_start_return_home;
                return;
            }
            partner = owner->tgtB;
        }
        if (*(u8 *)(ai + 0x17E) & 0x10)
        {
            ((struct Bits3 *)&ent->mode)->field0 = 3;
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
            obj_set_cell_coord(ent, math(dir, 2), 0);
            speed = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[0x19];
        }
        else
        {
            ((struct Bits3 *)&ent->mode)->field0 = 1;
            obj_load_record(ent->sprite, 3, -1, 0, 0);
            sprite_step_e6a0(ent->sprite);
            speed = ((u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2))[0x1D];
        }
        dx = partner->posX - ent->posX;
        dy = partner->posY - ent->posY;
        if ((dx | dy) != 0)
        {
            u16 angle = angle_of_vector16(dx, dy);
            facing = (u32)(angle + 0x10) >> 8;
        }
        else
        {
            facing = 0xFF;
        }
        zero = 0;
        ent->angle = facing;
        x = partner->posX + (partner->vx << 4);
        y = partner->posY + (partner->vy << 4);
        start_move_to(ent, x, y, speed, zero);
        flags = *(u8 *)(ai + 0x17E);
        bit = (((((u32)flags >> 4) ^ 1) & 1)) << 4;
        *(u8 *)(ai + 0x17E) = (flags & -0x11) | bit;
        timerVal = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x2E];
        *(u16 *)(ai + 0x17A) = timerVal;
    }
    step_pos_xy_plain(ent);
}
END_NONMATCH
