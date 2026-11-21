#include "global.h"
#include "ram_routines.h"

struct Sensor
{
    u8 filler0[0x8];
    s8 flag8;
    s8 kind;
    u8 filler0A[0x18 - 0xA];
    u16 unk18;
    u16 unk1A;
};

struct Sprite
{
    u8 filler0[0x11];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 anim;
    u8 record;
    u8 filler22;
    u8 state;
    u8 filler24[0x2A - 0x24];
    s16 timer;
};

struct FlagBits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct StateBits
{
    u8 low3 : 3;
    u8 high5 : 5;
};

struct FacingBits
{
    u8 dir : 3;
    u8 high5 : 5;
} __attribute__((packed));

struct Behavior
{
    u8 filler0[0x158];
    struct ZoneAnim *anim158;
    u8 filler15C[0x178 - 0x15C];
    u16 index178;
    u16 timer17C;
    u8 filler17E[0x184 - 0x17E];
    s32 homeX;
    s32 homeY;
};

struct MirrorBits
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct SpriteFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct CompanionObj
{
    u8 filler0[0x2];
    u8 angle;
    u8 filler3[0xC - 0x3];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u8 facing;
    u8 filler25[0x28 - 0x25];
    struct CompanionObj *entries[2];
    u8 filler30[0x100 - 0x30];
    s16 tbl[0x18];
    u8 filler130[0x198 - 0x130];
    u32 altHandler;
    u8 filler19C[0x1A0 - 0x19C];
    u32 handler;
    u32 handler1A4;
    u32 handler1A8;
    u32 nextHandler;
    u8 *animTable;
    struct Sensor *sensor;
    u8 filler1B8[0x1BE - 0x1B8];
    s16 variant;
    u8 filler1C0[0x1D0 - 0x1C0];
    u16 index1D0;
    u16 timer;
          u8 filler1D4[0x1D6 - 0x1D4];
    u8 flags1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 homeX;
    s32 homeY;
    u8 filler1E4[0x208 - 0x1E4];
    u8 flags208;
    u8 filler209[0x20C - 0x209];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 filler210[0x214 - 0x210];
    u8 state;
    u8 filler215;
    u8 flags216;
    u8 filler217[0x242 - 0x217];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    u32 stepLen;
    u8 filler24C[0x272 - 0x24C];
    u16 size272;
    u16 size274;
    u16 size276;
    u16 size278;
    u16 size27A;
    u16 size27C;
    u8 filler27E[0x294 - 0x27E];
    struct Sprite *sprite;
    u8 filler298[0x2C8 - 0x298];
    u32 unk2C8;
    u8 filler2CC[0x338 - 0x2CC];
    u8 *methods;
    u8 filler33C[0x33D - 0x33C];
    u8 flags33D;
    u8 flags33E;
    u8 filler33F[0x344 - 0x33F];
    struct CompanionObj *child;
    u8 filler348[0x352 - 0x348];
    u8 flags352;
};

struct ZoneAnim
{
    u8 filler0[0x1A];
    s16 unk1A;
    u16 unk1C;
    s16 unk1E;
};

typedef void (*ChaseHandler)(void *owner, struct CompanionObj *obj, u32 *slot);
typedef u8 (*ChaseMethod)(void *self, void *fn, u32 zero);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

struct AiBlock
{
    u8 filler0[0x148];
    u32 callback;
};

struct MethodSlot
{
    s16 offset;
    u16 unk2;
    ChaseMethod fn;
};

struct OwnerObj
{
    u8 filler0[0x28];
    struct CompanionObj *entries[2];
    u8 filler30[0x124 - 0x30];
    u16 kind;
    u8 filler126[0x200 - 0x126];
    u32 tgtA;
    u32 tgtB;
};

struct BoxVol
{
    u8 filler0[0xC];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
};


u8 probe_body_hit(void *owner, struct CompanionObj *body, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_deactivate(struct CompanionObj *obj);

u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
u32 angle_of_vector16(s32 dx, s32 dy);
void obj_set_cell_coord(struct CompanionObj *obj, u32 cell, u32 zero);
void ai_set_facing_from_velocity(struct CompanionObj *owner, struct CompanionObj *obj);
void ai_set_box_for_state(void *parent, struct CompanionObj *obj, u32 *slot);
u32 obj_inside_area(struct CompanionObj *obj);
void step_xy_or_handler(void *parent, struct CompanionObj *obj, u32 *slot);
void start_move_to(struct CompanionObj *obj, s32 x, s32 y, s32 speed, ByteArg zero);
u32 point_in_box(void *box, struct CompanionObj *obj, u32 limit, u32 zero);
void obj_box_to_stack(void *box, struct CompanionObj *obj, u32 extent, u32 zero);
u8 step_pos_xy_plain(struct CompanionObj *obj);
u8 step_pos_xy_c(struct CompanionObj *obj);
void obj_load_record(struct Sprite *spr, u32 record, u32 loadFlags, u32 arg3, u32 arg4);
void wander_to_edge(struct CompanionObj *obj, u32 a, u32 b);
void wander_diag(struct CompanionObj *obj, u32 a, u32 b);
void reset_record_fields(struct CompanionObj *obj);
void snap_to_linked(struct CompanionObj *obj);
void run_state_handler(void);
void face_entry(void);
void random_cell_walk(void);
void pick_next_handler(struct CompanionObj *parent, struct CompanionObj *obj, u32 *slot);
void ai_choose_move_target(void);
void load_anim_tables(struct CompanionObj *obj);
void init_size_and_pose(struct CompanionObj *obj);
void companion_chase_tick(void *parent, struct CompanionObj *obj);
void tick_store(void);
void obj_roll_spawn(void);
void ai_hold_at_home(void *parent, struct CompanionObj *obj, u32 *slot);
void run_e93c_then_handler(void *parent, struct CompanionObj *obj, u32 *slot);
u32 cpu_set(void *src, void *dst, u32 control);
void sprite_step_e6a0(struct Sprite *spr);
void tick_store(void);
void obj_load_anim_table(struct CompanionObj *obj);
void ai_seek_target(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot);
void ai_check_flags_c0(void *parent, struct CompanionObj *obj, u32 *slot);
u8 find_active_entry(struct CompanionObj *owner, u8 *idx);
u8 obj_flags_2b5_30_set(void *obj);
u8 obj_flags_2b5_c0_set(void *obj);
void obj_set_motion_a_08046AA4(void *obj, u32 a, u32 b, u32 c, u32 d);
void obj_set_motion_b(void *obj, u32 a, u32 b, u32 c, u32 d);
void obj_advance_axes(void *obj);
void obj_clear_flags_2b5_30(void *obj);
void obj_clear_flags_2b5_c0(void *obj);
void obj_set_sprite_link(void *obj, u32 v);
void obj_set_active(void *obj, u32 v);
void obj_set_or_add_pos(void *obj, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_roll_spawn(void);
void obj_load_kind_record(void);
void random_pose(void *owner, struct CompanionObj *obj, u32 *slot);
void entry_switch_handler(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot);
u32 entry_within_reach(struct CompanionObj *obj, struct CompanionObj *other);

/* Starts the companion's seek AI: loads sprite record 9, eases both motion axes
 * (A and B) to 0x10 at speed 1, unlinks the sprite, clears its state flags and
 * installs ai_seek_target as the per-frame state handler. */
void obj_enter_ai(void *parent, struct CompanionObj *obj)
{
    ((struct FlagBits *)&obj->flags33E)->bit4 = 0;
    obj_load_anim_table(obj);
    obj_load_record(obj->sprite, 9, -1, 0, 0);
    obj_set_motion_a_08046AA4(obj, 2, 0, 0x10, 1);
    obj_set_motion_b(obj, 2, 0, 0x10, 1);
    obj_advance_axes(obj);
    obj_clear_flags_2b5_30(obj);
    obj_clear_flags_2b5_c0(obj);
    obj_set_sprite_link(obj, 0);
    ((struct FlagBits *)&obj->flags20C)->bit7 = 0;
    ((struct FlagBits *)&obj->flags20C)->bit6 = 0;
    ((struct FlagBits *)&obj->flags20C)->bit5 = 0;
    ((struct FlagBits *)&obj->flags20D)->bit2 = 0;
    obj->altHandler = (u32)run_state_handler;
    obj->handler = (u32)ai_seek_target;
}

/* Companion AI state: waits until the owner's active partner is inside the companion's box
 * (extent 0x22), then links its sprite and eases both motion axes (A and B) to 0x100 at a speed
 * picked at random from the parameter record's bytes +0x20..+0x23; next state ai_check_flags_c0. */
void ai_seek_target(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot)
{
    u8 box[0x10];
    u8 activeIdx;
    struct CompanionObj *partner;
    u8 *speedPtr;
    u16 speed;

    if (find_active_entry(owner, &activeIdx) != 0)
    {
        /* owner's entries start at word 10 (+0x28) */
        partner = (struct CompanionObj *)((u32 *)owner)[activeIdx + 10];
        obj_box_to_stack(box, obj, 0x22, 0x10);
        if ((u8)point_in_box(box, partner, 0x7FFF, 0) != 0)
        {
            u32 roll;
            u32 goal;

            obj_set_sprite_link(obj, 1);
            roll = rand_below(4, mt19937_next());
            speedPtr = (u8 *)(roll + (u32)obj->animTable) + 0x20;
            goal = 0x100;
            speed = *(s8 *)speedPtr;
            obj_set_motion_a_08046AA4(obj, 2, 0, goal, speed);
            obj_set_motion_b(obj, 2, 0, goal, speed);
            obj_advance_axes(obj);
            *slot = (u32)ai_check_flags_c0;
        }
    }
}

/* AI state after ai_seek_target's jump: waits for both motions started with obj_set_motion_a/_b to
   end (flags 0x2B5 bits 4-5 and 6-7), noting each in flags1D6 bits 2 and 3. Once both are done the
   object turns its flags 0x20C bits 5-7 and 0x20D bit 2 back on, gets a 0xE00 x 0x100 x 0x800 box
   and goes on to ai_choose_move_target. */
void ai_check_flags_c0(void *parent, struct CompanionObj *obj, u32 *slot)
{
    struct FlagBits *done;
    u8 moving;
    u8 doneFlags;
    u32 doneA;
    u32 doneB;

    moving = obj_flags_2b5_30_set(obj);
    if (moving == 0)
    {
        obj_clear_flags_2b5_30(obj);
        ((struct FlagBits *)&obj->flags1D6)->bit2 = 1;
    }
    moving = obj_flags_2b5_c0_set(obj);
    done = (struct FlagBits *)&obj->flags1D6;
    if (moving == 0)
    {
        obj_clear_flags_2b5_c0(obj);
        done->bit3 = 1;
    }
    doneFlags = obj->flags1D6;
    doneA = ((u32)doneFlags << 29) >> 31;
    doneB = ((u32)doneFlags << 28) >> 31;
    if (doneB & doneA)
    {
        ((struct FlagBits *)&obj->flags20C)->bit5 = 1;
        ((struct FlagBits *)&obj->flags20C)->bit6 = 1;
        ((struct FlagBits *)&obj->flags20C)->bit7 = 1;
        ((struct FlagBits *)&obj->flags20D)->bit2 = 1;
        done->bit3 = 0;
        done->bit2 = 0;
        obj->size272 = 0xE00;
        obj->size274 = 0x100;
        obj->size276 = 0x800;
        *slot = (u32)ai_choose_move_target;
    }
}

ASM_FUNC("asm/code_0814DF44/ai_choose_move_target.s", void ai_choose_move_target(void))

void ai_set_anim_by_mode(struct CompanionObj *obj, u8 mode);
void ai_aim_and_strike(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot);
u8 widths_overlap_x(struct CompanionObj *obj, u32 other);
u8 heights_overlap_y(struct CompanionObj *obj, u32 other);
/*
 * AI state: with a small strike box (8 x 4 x 6 px, 8.8 fixed point) swapped in for the
 * check, if the object overlaps either of the owner's targets it goes on to
 * ai_aim_and_strike. Otherwise it keeps stepping its current move; once that ends it
 * walks back home at a random one of its speeds tbl[4..7] (step_xy_or_handler) when
 * it's outside its area, or picks a new move target (ai_choose_move_target).
 */
void ai_move_or_return(struct OwnerObj *owner, struct CompanionObj *obj, u32 *slot)
{
    s32 unused[4];
    struct Behavior *ai;
    s16 *tbl;
    u16 savedSizeX;
    u16 savedSizeY;
    u16 savedSizeZ;
    s32 speed;
    u32 roll;

    ai = (struct Behavior *)((u8 *)obj + 0x58);
    tbl = obj->tbl;
    savedSizeX = obj->size272;
    savedSizeY = obj->size274;
    savedSizeZ = obj->size276;
    obj->size272 = 0x800;
    obj->size274 = 0x400;
    obj->size276 = 0x600;
    if (((u8)widths_overlap_x(obj, owner->tgtA) != 0 && (u8)heights_overlap_y(obj, owner->tgtA) != 0)
        || ((u8)widths_overlap_x(obj, owner->tgtB) != 0 && (u8)heights_overlap_y(obj, owner->tgtB) != 0))
    {
        obj->size272 = savedSizeX;
        obj->size274 = savedSizeY;
        obj->size276 = savedSizeZ;
        *slot = (u32)ai_aim_and_strike;
    }
    else
    {
        obj->size272 = savedSizeX;
        obj->size274 = savedSizeY;
        obj->size276 = savedSizeZ;
        if ((u8)step_pos_xy_plain(obj) == 0)
        {
            if ((u8)obj_inside_area(obj) == 0)
            {
                roll = rand_below(4, mt19937_next());
                speed = tbl[roll + 4];
                start_move_to(obj, ai->homeX, ai->homeY, speed, (u8)0);
                ai_set_anim_by_mode(obj, 2);
                *slot = (u32)step_xy_or_handler;
            }
            else
            {
                ai_set_anim_by_mode(obj, 0);
                *slot = (u32)ai_choose_move_target;
            }
        }
    }
}

NONMATCH("asm/code_0814DF44/ai_aim_and_strike.s", void ai_aim_and_strike(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot))
{
    u16 *tbl;
    u8 idx;
    u8 *ip;
    u32 pick;
    struct CompanionObj *target;
    s32 dx;
    s32 dy;
    u32 dir;
    u8 f;
    s32 mask;
    u8 *fp;
    u32 cell;
    u32 ang;
    u32 roll;
    u32 angle;

    ai_set_facing_from_velocity(owner, obj);
    tbl = (u16 *)obj->tbl;
    ip = &idx;
    if (find_active_entry(owner, ip) == 0)
        pick = ((u32)owner->flags208 << 30) >> 31;
    else
        pick = idx;
    *ip = pick;
    target = (struct CompanionObj *)((u32 *)owner)[idx + 10];
    dx = target->x - obj->x;
    dy = target->y - obj->y;
    if ((dx | dy) != 0)
    {
        ang = angle_of_vector16(dx, dy);
        dir = (((s32)((u16)ang + 0x1000)) >> 13) & 7;
    }
    else
        dir = -1;
    fp = &obj->facing;
    dir &= 7;
    mask = -8;
    f = (*fp & mask) | dir;
    *fp = f;
    cell = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((u32)f << 29) >> 29, 2);
    obj_set_cell_coord(obj, cell, 0);
    obj->state = (obj->state & mask) | 1;
    ((struct SpriteFlagBits *)&obj->sprite->flags)->mode = 1;
    roll = rand_below(4, mt19937_next());
    obj->sprite->anim = tbl[roll + 20];
    dx = target->x - obj->x;
    dy = target->y - obj->y;
    if ((dx | dy) != 0)
    {
        ang = angle_of_vector16(dx, dy);
        angle = ((u32)(u16)ang + 0x10) >> 8;
    }
    else
        angle = 0xFF;
    obj->angle = angle;
    *slot = (u32)ai_set_box_for_state;
}
END_NONMATCH

void ai_set_box_for_state(void *parent, struct CompanionObj *obj, u32 *slot)
{
    u8 st;

    st = obj->sprite->state;
    if (st > 1)
    {
        obj->size272 = 0xC00;
        obj->size274 = 0xC00;
        obj->size276 = 0x800;
    }
    else if (st != 0)
    {
        obj->size272 = 0xC00;
        obj->size274 = 0x800;
        obj->size276 = 0x800;
    }
    if ((obj->sprite->flags & 8) != 0)
    {
        ((struct StateBits *)&obj->state)->low3 = 0;
        *slot = (u32)run_e93c_then_handler;
    }
}

/*
 * AI state after a strike (set by run_e93c_then_handler, which plays the strike
 * animation backwards): keeps the strike box (12 x 6 x 8 px, then 12 x 8 x 8 px,
 * 8.8 fixed point) while the sprite is in state 0 or 1, and waits until the reversed
 * animation has wound back (timer <= |anim step|). Then it reloads the idle record,
 * restores the normal box (14 x 1 x 8 px) and walks home at a random one of its
 * speeds tbl[4..7] (step_xy_or_handler) when outside its area, or picks a new move
 * target (ai_choose_move_target).
 */
void ai_hold_at_home(void *parent, struct CompanionObj *obj, u32 *slot)
{
    s16 *tbl;
    struct Behavior *ai;
    struct Sprite **sprp;
    s32 timer;
    s32 step;
    s32 speed;
    u32 roll;

    ai = (struct Behavior *)((u8 *)obj + 0x58);
    tbl = obj->tbl;
    if (obj->sprite->state == 0)
    {
        obj->size272 = 0xC00;
        obj->size274 = 0x600;
        obj->size276 = 0x800;
    }
    else
    {
        if (obj->sprite->state > 1)
            return;
        obj->size272 = 0xC00;
        obj->size274 = 0x800;
        obj->size276 = 0x800;
    }
    sprp = &obj->sprite;
    if ((*sprp)->state > 1)
        return;
    timer = (*sprp)->timer;
    step = (s8)(*sprp)->anim;
    if (step < 0)
        step = -step;
    if (timer > step)
        return;
    ((struct StateBits *)&obj->state)->low3 = 0;
    obj_load_record(*sprp, 9, -1, 0, 0);
    obj->size272 = 0xE00;
    obj->size274 = 0x100;
    obj->size276 = 0x800;
    if ((u8)obj_inside_area(obj) == 0)
    {
        roll = rand_below(4, mt19937_next());
        speed = tbl[roll + 4];
        start_move_to(obj, ai->homeX, ai->homeY, speed, (u8)0);
        *slot = (u32)step_xy_or_handler;
    }
    else
        *slot = (u32)ai_choose_move_target;
}

/*
 * Loads the object's AI parameters tbl[0..23] from its 0x20-byte parameter record (animTable):
 * two runs of 4 halfwords (tbl[0..3], and the walking speeds tbl[4..7]) copied as they are,
 * then 16 signed bytes widened to tbl[8..23] (the animation values ai_set_anim_by_mode and
 * ai_set_facing_from_velocity pick from).
 */
void obj_load_anim_table(struct CompanionObj *obj)
{
    s16 *params;
    s8 *src;

    params = obj->tbl;
    src = (s8 *)obj->animTable;
    cpu_set(src, params, 4); /* 4 halfwords */
    src += 8;
    cpu_set(src, &obj->tbl[4], 4);
    src += 8;
    params[8] = src[0];
    params[9] = src[1];
    src += 2;
    params[10] = src[0];
    src++;
    params[11] = src[0];
    src++;
    params[12] = src[0];
    src++;
    params[13] = src[0];
    src++;
    params[14] = src[0];
    src++;
    params[15] = src[0];
    src++;
    params[16] = src[0];
    src++;
    params[17] = src[0];
    src++;
    params[18] = src[0];
    src++;
    params[19] = src[0];
    src++;
    params[20] = src[0];
    src++;
    params[21] = src[0];
    src++;
    params[22] = src[0];
    src++;
    params[23] = src[0];
}

/* Puts the object in state 1 facing its velocity (one of 8 directions, -1 when it stands still),
   picks the sprite cell for that direction, plays a random one of the animations tbl[20..23] and
   aims obj->angle (256 steps, 0xFF when on top of it) at the owner's active target, or at entry
   (owner->flags208 bit 1) when none is active. */
void ai_set_facing_from_velocity(struct CompanionObj *owner, struct CompanionObj *obj)
{
    s16 *params;
    s16 *vyp;
    s16 *vxp;
    u8 targetIdx;
    u8 *targetIdxp;
    u32 pick;
    struct CompanionObj *target;
    s32 dx;
    s32 dy;
    s32 dir;
    u32 cell;
    u32 vecAngle;
    u32 roll;
    u32 angle;

    params = obj->tbl;
    targetIdxp = &targetIdx;
    if (find_active_entry(owner, targetIdxp) == 0)
        pick = ((u32)owner->flags208 << 30) >> 31;
    else
        pick = targetIdx;
    *targetIdxp = pick;
    /* owner->entries[targetIdx] */
    target = (struct CompanionObj *)((u32 *)owner)[targetIdx + 10];
    ((struct StateBits *)&obj->state)->low3 = 1;
    vxp = &obj->vx;
    vyp = &obj->vy;
    {
        u32 vx = (u16)vxp[0];
    if ((vx | (u16)vyp[0]) != 0)
    {
        vecAngle = angle_of_vector16(vxp[0], vyp[0]);
        /* nearest of 8 directions (0x2000 of the 0x10000 circle each) */
        dir = (((s32)((u16)vecAngle + 0x1000)) >> 13) & 7;
    }
    else
        dir = -1;
    }
    ((struct FacingBits *)&obj->facing)->dir = dir;
    cell = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(((struct FacingBits *)&obj->facing)->dir, 2);
    obj_set_cell_coord(obj, cell, 0);
    roll = rand_below(4, mt19937_next());
    obj->sprite->anim = params[roll + 20];
    dx = target->x - obj->x;
    dy = target->y - obj->y;
    if ((dx | dy) != 0)
    {
        vecAngle = angle_of_vector16(dx, dy);
        angle = ((u32)(u16)vecAngle + 0x10) >> 8;
    }
    else
        angle = 0xFF;
    obj->angle = angle;
}



void ai_reload_sprite_mirrored(void *parent, struct CompanionObj *obj)
{
    struct Sprite **sprp;
    u32 zero;

    sprp = &obj->sprite;
    zero = 0;
    obj_load_record(*sprp, (*sprp)->record, 5, 0, zero);
    ((struct StateBits *)&obj->state)->low3 = 1;
    ((struct SpriteFlagBits *)&(*sprp)->flags)->mode = 1;
    (*sprp)->anim = -(*sprp)->anim;
}

/* Reloads the sprite with record 9, puts the object in AI state 4 and sets the sprite's anim value
 * to a random one of its parameters (obj_load_anim_table): tbl[8..11] for mode 0 (before a new move
 * target), tbl[12..13] for mode 1, tbl[18..19] for mode 2 (walking back home; the record is reloaded
 * once more first). */
void ai_set_anim_by_mode(struct CompanionObj *obj, u8 mode)
{
    u16 *params;
    u32 roll;

    params = (u16 *)obj->tbl;
    obj_load_record(obj->sprite, 9, -1, 0, 0);
    sprite_step_e6a0(obj->sprite);
    ((struct StateBits *)&obj->state)->low3 = 4;
    ((struct SpriteFlagBits *)&obj->sprite->flags)->mode = 0;
    switch (mode)
    {
    case 0:
        roll = rand_below(4, mt19937_next());
        obj->sprite->anim = params[roll + 8];
        break;
    case 1:
        roll = rand_below(2, mt19937_next());
        obj->sprite->anim = params[roll + 12];
        break;
    case 2:
        obj_load_record(obj->sprite, 9, -1, 0, 0);
        sprite_step_e6a0(obj->sprite);
        roll = rand_below(2, mt19937_next());
        obj->sprite->anim = params[roll + 18];
        break;
    }
}
void ai_wander_random(void *parent, struct CompanionObj *obj, u32 *slot)
{
    if ((rand_below(100, mt19937_next()) & 1) != 0)
        wander_to_edge(obj, 0x10, 0x100);
    else
        wander_diag(obj, 0x10, 0x100);
    ((struct StateBits *)&obj->state)->low3 = 1;
    *slot = (u32)pick_next_handler;
}

u32 obj_inside_area(struct CompanionObj *obj)
{
    u8 box[0x10];
    u32 inside;

    obj_box_to_stack(box, obj, 0x22, 0);
    inside = point_in_box(box, obj, 0x7FFF, 0);
    if ((u8)inside != 0)
        inside = 1;
    return inside;
}

void run_e93c_then_handler(void *parent, struct CompanionObj *obj, u32 *slot)
{
    ai_reload_sprite_mirrored(parent, obj);
    *slot = (u32)ai_hold_at_home;
}

void step_xy_or_handler(void *parent, struct CompanionObj *obj, u32 *slot)
{
    if (step_pos_xy_plain(obj) == 0)
        *slot = (u32)ai_choose_move_target;
}

void entity_mode_42_noop(void)
{
}

void setup_linked_d3cd(void *parent, struct CompanionObj *obj)
{
    ((struct FlagBits *)&obj->flags33E)->bit4 = 1;
    reset_record_fields(obj);
    ((struct FlagBits *)&obj->flags33D)->bit3 = 1;
    snap_to_linked(obj);
    obj->altHandler = (u32)run_state_handler;
    obj->handler = (u32)face_entry;
    obj->handler1A4 = (u32)face_entry;
    obj->handler1A8 = (u32)face_entry;
}

void setup_linked_ced5(void *parent, struct CompanionObj *obj)
{
    ((struct FlagBits *)&obj->flags33E)->bit4 = 1;
    reset_record_fields(obj);
    ((struct FlagBits *)&obj->flags33D)->bit3 = 1;
    snap_to_linked(obj);
    obj->altHandler = (u32)run_state_handler;
    obj->handler = (u32)random_cell_walk;
    obj->handler1A4 = (u32)random_cell_walk;
    obj->handler1A8 = (u32)random_cell_walk;
}

void pick_next_handler(struct CompanionObj *parent, struct CompanionObj *obj, u32 *slot)
{
    if ((parent->entries[0]->flags20C & 1) != 0 && (parent->entries[1]->flags20C & 1) != 0)
        *slot = (u32)ai_choose_move_target;
    else if (step_pos_xy_c(obj) == 0)
        *slot = (u32)ai_wander_random;
}

static inline void clear_child_handler(struct CompanionObj *c)
{
    c->handler = 0;
}

/* Puts a companion into follow mode: record 1 gets size 0xA00 x 0x1000 x 0xA00, its animation
 * tables and pose are loaded, it faces the way its sensor says, remembers its current position as
 * home and picks a random index 0-3. It then chases (companion_chase_tick) under
 * entry_switch_handler; under owner kind 0x197 it first waits 15 frames (tick_store). */
void enter_owner_follow(void *parent, struct CompanionObj *obj)
{
    struct Sprite **sprp;
    struct CompanionObj *child;
    s32 facing;
    struct Sprite *spr;

    child = obj->child;
    ((struct FlagBits *)&obj->flags33E)->bit4 = 1;
    sprp = &obj->sprite;
    if ((*sprp)->record == 1)
    {
        obj->size272 = 0xA00;
        obj->size274 = 0x1000;
        obj->size276 = 0xA00;
    }
    load_anim_tables(obj);
    ((struct StateBits *)&obj->state)->low3 = 1;
    ((struct FlagBits *)&obj->flags33D)->bit3 = 1;
    init_size_and_pose(child);
    spr = obj->sprite;
    facing = obj->sensor->flag8;
    ((struct MirrorBits *)&spr->mirror)->bit6 = facing & 1;
    obj->variant = (*sprp)->record;
    obj->homeX = obj->x;
    obj->homeY = obj->y;
    obj->index1D0 = rand_below(4, mt19937_next());
    clear_child_handler(child);
    obj->altHandler = (u32)companion_chase_tick;
    obj->handler = (u32)entry_switch_handler;
    if (((struct OwnerObj *)parent)->kind == 0x197)
    {
        obj->timer = 0xF;
        obj->handler = (u32)tick_store;
        obj->nextHandler = (u32)entry_switch_handler;
    }
}


void enter_idle_wait(struct CompanionObj *obj, u32 *slot);
NONMATCH("asm/code_0814DF44/companion_chase_tick.s", void companion_chase_tick(void *parent, struct CompanionObj *obj))
{
    struct CompanionObj *body;
    struct AiBlock *bodyAi;
    struct AiBlock *entAi;
    struct MethodSlot *m;
    u8 result;
    u8 locked;

    body = obj->child;
    entAi = (struct AiBlock *)((u8 *)obj + 0x58);
    bodyAi = (struct AiBlock *)((u8 *)body + 0x58);
    ((ChaseHandler)obj->handler)(parent, obj, &obj->handler);
    if (body->handler == 0)
        return;
    ((ChaseHandler)body->handler)(parent, body, &body->handler);
    m = (struct MethodSlot *)(body->methods + 0x68);
    result = m->fn((u8 *)body + m->offset, (void *)m->fn, 0);
    if (result == 0)
    {
        locked = body->flags216 & 0x20;
        if (locked == 0 && (body->flags20C & 1) != 0)
        {
            if (probe_body_hit(parent, body, body->x, body->y, body->z + body->dz, 1, locked) == 0)
                return;
        }
    }
    bodyAi->callback = 0;
    {
        s32 f = body->flags20D;
        f |= 8;
        body->flags20D = f;
    }
    obj_deactivate(body);
    enter_idle_wait(obj, &entAi->callback);
}
END_NONMATCH

/* Companion handler while it follows: once the lead brother (find_active_entry), or else the other one
 * when it is active and has flags352 bit 3 set, is within reach, goes on to obj_load_kind_record;
 * otherwise, when the companion's child has no handler left, picks a random pose (random_pose).
 * The brothers are owner->entries[0] and [1]. */
void entry_switch_handler(struct CompanionObj *owner, struct CompanionObj *obj, u32 *slot)
{
    u8 unused[0x10];
    u8 broIdx;
    u8 *idxp;
    struct CompanionObj *bro;

    idxp = &broIdx;
    if (find_active_entry(owner, idxp) != 0)
    {
        bro = (struct CompanionObj *)((u32 *)owner)[*idxp + 10]; /* owner->entries[broIdx] */
        if ((u8)entry_within_reach(obj, bro) != 0)
        {
            *slot = (u32)obj_load_kind_record;
            return;
        }
        /* the other brother */
        *idxp = !*idxp;
        bro = (struct CompanionObj *)((u32 *)owner)[*idxp + 10];
        if ((((u32)bro->flags20C << 31) >> 31) & (((u32)bro->flags352 << 28) >> 31))
        {
            if ((u8)entry_within_reach(obj, bro) != 0)
            {
                *slot = (u32)obj_load_kind_record;
                return;
            }
        }
        if (obj->child->handler == 0)
            *slot = (u32)random_pose;
    }
}

/* Gives obj a random pose: its sprite (mode 1) takes one of the four animations in tbl[4..7], loads
   record 4 (variant 0) or 5 (variant 1) and steps once; a random index 0-3 goes to index1D0 and the
   handler goes on to obj_roll_spawn. */
void random_pose(void *owner, struct CompanionObj *obj, u32 *slot)
{
    struct Sprite **spritePtr;
    struct Behavior *ai;
    s16 *tbl;

    ai = (struct Behavior *)((u8 *)obj + 0x58);
    tbl = obj->tbl;
    spritePtr = &obj->sprite;
    ((struct SpriteFlagBits *)&(*spritePtr)->flags)->mode = 1;
    (*spritePtr)->anim = ((u16 *)tbl)[rand_below(4, mt19937_next()) + 4];
    switch (obj->variant)
    {
    case 0:
        obj_load_record(*spritePtr, 4, -1, 0, 0);
        break;
    case 1:
        obj_load_record(*spritePtr, 5, -1, 0, 0);
        break;
    }
    sprite_step_e6a0(obj->sprite);
    ai->index178 = rand_below(4, mt19937_next()); /* obj->index1D0 */
    *slot = (u32)obj_roll_spawn;
}

ASM_FUNC("asm/code_0814DF44/follow_entry_axis.s", void follow_entry_axis(void))

void load_anim_tables(struct CompanionObj *obj)
{
    register s16 *tbl asm("r4");
    register void *arg1 asm("r1");
    struct CompanionObj *child;
    s16 *ctbl;
    register s8 *p asm("r6");

    if (obj == arg1)
        ctbl = NULL;
    tbl = obj->tbl;
    child = obj->child;
    if (child == NULL && obj == NULL)
        ctbl = NULL;
    ctbl = child->tbl;
    p = (s8 *)obj->animTable;
    cpu_set(p, tbl, 4);
    p += 8;
    tbl[4] = p[0];
    tbl[5] = p[1];
    p += 2;
    tbl[6] = p[0];
    p++;
    tbl[7] = p[0];
    p++;
    cpu_set(p, ctbl, 4);
    p += 8;
    cpu_set(p, &child->tbl[12], 4);
    p += 8;
    cpu_set(p, &child->tbl[16], 4);
    p += 8;
    tbl[20] = p[0];
    tbl[21] = p[1];
    p += 2;
    tbl[22] = p[0];
    tbl[23] = p[1];
}

void enter_idle_wait(struct CompanionObj *obj, u32 *slot)
{
    struct Sprite **sprp;
    s16 *tbl;

    tbl = obj->tbl;
    ((struct FlagBits *)&obj->flags20D)->bit2 = 1;
    sprp = &obj->sprite;
    ((struct SpriteFlagBits *)&(*sprp)->flags)->mode = 0;
    ((struct StateBits *)&obj->state)->low3 = 1;
    obj_load_record(*sprp, obj->variant, -1, 0, 0);
    sprite_step_e6a0(*sprp);
    obj->timer = ((u16 *)tbl)[rand_below(4, mt19937_next()) + 20];
    *slot = (u32)tick_store;
    slot[3] = (u32)entry_switch_handler;
}

/* Puts a companion's child in AI state 4 with sprite anim 0x10, reloads its sprite with record 8,
 * gives both of its size triples (size272-276 and size278-27C) 0x800 x 0x600 x 0x600, and sets
 * flags20D bit 3 and flags20F bit 2 while clearing flags20D bits 2 and 5 and flags20E bit 2. */
void init_size_and_pose(struct CompanionObj *obj)
{
    struct Sprite **sprp;

    ((struct StateBits *)&obj->state)->low3 = 4;
    sprp = &obj->sprite;
    (*sprp)->anim = 0x10;
    obj_load_record(*sprp, 8, -1, 0, 0);
    sprite_step_e6a0(*sprp);
    obj->size278 = 0x800;
    obj->size272 = 0x800;
    obj->size27A = 0x600;
    obj->size274 = 0x600;
    obj->size27C = 0x600;
    obj->size276 = 0x600;
    ((struct FlagBits *)&obj->flags20D)->bit3 = 1;
    ((struct FlagBits *)&obj->flags20D)->bit5 = 0;
    ((struct FlagBits *)&obj->flags20D)->bit2 = 0;
    ((struct FlagBits *)&obj->flags20E)->bit2 = 0;
    ((struct FlagBits *)&obj->flags20F)->bit2 = 1;
}

/* Launch obj from its parent (obj->child): 0x14 pixels in front of it and 0xB above when the
 * parent faces sideways (variant 0), 8 pixels below it when it faces down (variant 1), with a
 * speed picked at random from obj->tbl. Small shots (kind 0) also pick a random travel length;
 * big ones (kind 1) take their speed from another row of the table. */
void spawn_random_offset(struct CompanionObj *obj, u8 kind)
{
    u16 *tbl;
    u8 *parentBody;
    struct CompanionObj *parent;
    s32 variant;
    u32 speed;
    u32 length;
    u32 pick;
    u32 *lengthCopy;
    u32 *lengthLeft;
    s32 zOff;

    parentBody = (u8 *)obj->child + 0x58;
    tbl = (u16 *)obj->tbl;
    if (kind == 0)
    {
        pick = rand_below(4, mt19937_next());
        lengthLeft = &obj->unk2C8;
        lengthCopy = &obj->stepLen;
        length = tbl[pick + 12];
        *lengthCopy = length;
        *lengthLeft = length;
        speed = tbl[rand_below(4, mt19937_next()) + 16];
    }
    else if (kind == 1)
    {
        speed = tbl[rand_below(4, mt19937_next())];
    }
    /* the parent's variant, at +0x1BE */
    variant = *(s16 *)(parentBody + 0x166);
    switch (variant)
    {
    case 0:
        parent = obj->child;
        if ((parent->sprite->mirror & 0x40) != 0)
        {
            obj_set_or_add_pos(obj, parent->x - 0x1400, parent->y, parent->z + parent->dz + 0xB00, 1, variant);
            obj->vx = -speed;
            obj->vy = variant;
        }
        else
        {
            zOff = 0xB00;
            obj_set_or_add_pos(obj, parent->x + 0x1400, parent->y, parent->z + zOff + parent->dz, 1, variant);
            obj->vx = speed;
            obj->vy = variant;
        }
        break;
    case 1:
        zOff = 0xB00;
        obj_set_or_add_pos(obj, obj->child->x, obj->child->y + 0x800, obj->child->z + zOff + obj->child->dz, variant, 0);
        obj->vx = 0;
        obj->vy = speed;
        break;
    }
    obj->sprite->anim = 0x10;
    ((struct StateBits *)&obj->state)->low3 = 1;
    obj_set_active(obj, 1);
    obj_set_sprite_link(obj, 1);
}

NONMATCH("asm/code_0814DF44/box_fx8_div_and_dispatch.s", u32 box_fx8_div_and_dispatch(void *self, struct CompanionObj *obj, struct BoxVol *box))
{
    s32 xb;
    s32 yb;
    s32 xa;
    s32 ya;

    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->x, 0x100);
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->y, 0x100);
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->z + obj->dz, 0x100);
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(box->x, 0x100);
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(box->y, 0x100);
    MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(box->z + box->dz, 0x100);
    switch (obj->variant)
    {
    case 0:
        if ((obj->sprite->mirror & 0x40) != 0)
        {
            xb = 0x6400;
            xa = 0;
            yb = 0x400;
            ya = yb;
        }
        else
        {
            xb = 0;
            xa = 0x6400;
            yb = 0x400;
            ya = yb;
        }
        break;
    case 1:
        xb = 0x400;
        xa = xb;
        yb = 0;
        ya = 0x6400;
        break;
    }
    if (obj->x + xa >= box->x && obj->x - xb <= box->x && obj->y + ya >= box->y
        && obj->y - yb <= box->y && obj->z + obj->dz + 0x400 >= box->z + box->dz
        && obj->z + obj->dz <= box->z + box->dz)
        return 1;
    return 0;
}
END_NONMATCH

u32 entry_within_reach(struct CompanionObj *obj, struct CompanionObj *other)
{
    struct Sensor *sensor = obj->sensor;

    if (obj->variant == 0)
    {
        if (sensor->kind == 1)
        {
            u16 half = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->size278, 2);

            if (obj->x - half <= other->x && obj->x + half >= other->x)
                return 1;
        }
    }
    else if (obj->variant == 1 && sensor->kind == 1)
    {
        if (obj->y >= other->y && obj->y - obj->size27C <= other->y)
            return 1;
    }
    return 0;
}

ASM_FUNC("asm/code_0814DF44/zone_move_handler.s", void zone_move_handler(void))
