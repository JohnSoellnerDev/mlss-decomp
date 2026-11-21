#include "global.h"
#include "ram_routines.h"

struct Record
{
    u8 filler0[0x1A];
    u16 unk1A;
    u8 filler1C[0x2E - 0x1C];
    u16 unk2E;
    u16 unk30;
    u16 unk32;
    u16 unk34;
    u16 speeds[4];
    u8 unk3E;
};

struct Partner
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x20C - 0x1C];
    u8 unk20C;
    u8 filler20D[0x278 - 0x20D];
    u16 width;  /* 8.8 fixed point */
    u8 filler27A[0x27C - 0x27A];
    u16 height; /* 8.8 fixed point */
};

struct OwnerFlags
{
    u8 unk0 : 2;
    u8 bit2 : 1;
    u8 unk3 : 5;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x10B - 0x38];
    u8 unk10B;
    u8 filler10C[0x208 - 0x10C];
    u8 flags208;
    u8 filler209[0x246 - 0x209];
    u8 unk246;
};

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    s8 unk20;
    u8 unk21;
    u8 filler22[0x23 - 0x22];
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    s16 unk2A;
};

struct Flags8
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

typedef void (*EnemyFn)();

struct PartOffsets
{
    u8 filler0[0x36];
    u16 ids[4];
    s16 offsZ[4];
    s16 offsXY[8];
};

struct HitBox
{
    u8 filler0[0x34];
    u16 linkIndex;
    u8 filler36[0x38 - 0x36];
    s16 hitBottom;
    s16 hitTop;
    s16 hitLeft;
    s16 hitRight;
};

/* Box edge from a record: in tiles (16 px, centred) when bit 0 of the record's flags is set. */
static inline s32 hit_box_edge(s32 v, s16 *entry)
{
    s32 tiles = *(u8 *)entry & 1;
    s32 edge;

    if (tiles)
        v <<= 4;
    edge = v;
    if (tiles)
        edge += 8;
    return edge;
}

struct Enemy
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18; /* height above posZ (8.8 fixed point): hops, bobbing */
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
          u8 unk25;
      u8 filler26[0x34 - 0x26];
    s32 prevX;
    s32 prevY;
          u8 filler3C[0xF8 - 0x3C];
      u16 unkF8;
      u8 fillerFA[0x100 - 0xFA];
    u16 wanderDist[4];
    u16 unk108[4];
    u16 unk110[4];
    u16 unk118[4];
    u8 filler120[0x128 - 0x120];
    u16 unk128[4]; /* wait times, one picked at random */
    u16 wanderSteps[2];
    u16 linkIndex;
    u8 filler136[0x138 - 0x136];
    s16 hitBottom;
    s16 hitTop;
    s16 hitLeft;
    s16 hitRight;
    u8 unk140;
    u8 unk141;
    u8 filler142[0x198 - 0x142];
    EnemyFn unk198;
    EnemyFn unk19C;
    EnemyFn unk1A0[2];
    u8 filler1A8[0x1AC - 0x1A8];
    EnemyFn unk1AC;
    struct Record *rec;
    s16 *box;
    s16 vecX;
    s16 vecY;
    u8 filler1BC[0x1BE - 0x1BC];
    s16 hopValue;
    u32 hopDist;
    u8 filler1C4[0x1D0 - 0x1C4];
    u16 index;
    u16 unk1D2;
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x1D8 - 0x1D7];
    u16 unk1D8;
    u8 filler1DA[0x1DC - 0x1DA];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x20D - 0x1EC];
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
          u8 filler211[0x213 - 0x211];
      u8 unk213;
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x239 - 0x217];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u32 hopZ0;
    u8 filler250[0x254 - 0x250];
    u32 hopSpeed0;
    s32 jumpVz;
    s32 jumpFrames;
    u8 filler260[0x278 - 0x260];
    u16 width;  /* 8.8 fixed point */
    u8 filler27A[0x27C - 0x27A];
    u16 height; /* 8.8 fixed point */
    u8 filler27E[0x294 - 0x27E];
    struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 orbitCenX;
    s32 orbitCenY;
    u8 filler2C4[0x2C8 - 0x2C4];
    s32 orbitCenZ;
    s32 orbitRadX;
    s32 orbitRadY;
          u8 filler2D4[0x2D8 - 0x2D4];
      u16 unk2D8;
      u16 unk2DA;
      u8 filler2DC[0x2DE - 0x2DC];
      u16 unk2DE;
      u8 filler2E0[0x2F4 - 0x2E0];
    u32 unk2F4; /* phase angle (0-0xFF) of the orbit / bobbing (gSineTable index) */
    s16 unk2F8; /* phase step: how far unk2F4 turns per frame */
    u8 filler2FA[0x33D - 0x2FA];
    u8 unk33D;
    u8 filler33E[0x344 - 0x33E];
    struct Enemy *body;
};

u32 mt19937_next(void);
s32 rand_below(s32 n, u32 value);
void obj_set_state(void *obj, u32 state);
void obj_sync_sub_slot(void *obj, u32 value);
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void flags_obj_set_mode(struct Enemy *ent, u32 mode);
void obj_apply_state_a(void *obj);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void run_state_handler(void);
void tick_store(void);
void obj_pick_next_state(void);
void obj_random_pose(void);
void obj_random_offset(struct Enemy *obj, u32 dir);
void obj_random_offset_step(struct Enemy *obj);
void obj_start_move_to_target(struct Enemy *obj, u32 mode, s32 x, s32 y);
void obj_start_move_quarter(struct Enemy *obj, u32 mode, s32 x, s32 y);
void obj_step_toward_target_08138FDC(struct Enemy *obj);
void obj_pick_move_vector(struct Enemy *obj, struct Partner *entry);
void obj_move_interp(struct Enemy *obj);
void obj_random_facing_target(struct Enemy *obj);
void obj_velocity_from_facing(struct Enemy *obj);
void obj_try_start_hop(struct Enemy *obj);

void obj_hop_landing_step(struct Enemy *obj);
void obj_orbit_circle_step(struct Enemy *obj);
u32 obj_pick_dir_toward_owner(struct Owner *owner, struct Enemy *obj);
void load_record_fields(struct Enemy *obj, s16 index);
void load_record_fields_scaled(struct Enemy *obj, s16 index);
void load_hit_box(struct Enemy *obj, s16 index);
void load_path_points(struct Enemy *obj, s16 index);
void load_part_offsets(struct Enemy *obj, s16 index);
u32 advance_move_step(void *ctx, struct Enemy *obj);
u32 position_blocked(struct Owner *owner, struct Enemy *obj, u32 x, u32 y, u32 z);
u32 div_scaled_fixed(u32 a, u32 x, u32 b, u32 k);
struct Partner *owner_entry(struct Owner *owner, u16 index);
void set_timer_from_table(struct Enemy *obj, s16 index);
void load_hit_box_bytes(struct Enemy *obj, s16 index);
void load_hit_box_words(struct Enemy *obj, s16 index);
void load_record_words_128(struct Enemy *obj, s16 index);
void obj_pick_random_entry(struct Enemy *obj);
void tick_fire_indirect(void);
void sub_08139F08(void);
void body_timer_tick(void *ctx, struct Enemy *ent, EnemyFn *out);
void tick_body_timer(void *ctx, struct Enemy *ent, EnemyFn *out);
void update_hop_phase(struct Enemy *obj);
void obj_jump_arc_step(struct Enemy *obj);
void stub_nop_0813A064(void);
void start_linked_state(struct Owner *owner, struct Enemy *obj);
void set_flags_1d6_copy(void *ctx, struct Enemy *obj, EnemyFn *out);
void advance_handler_slot(void *ctx, struct Enemy *obj, EnemyFn *out);
void link_to_owner_entry(struct Owner *owner, struct Enemy *obj, EnemyFn *out);
void move_step_handler(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_advance(void *ctx, struct Enemy *obj);
void enter_run_state(void *ctx, struct Enemy *obj);
void setup_random_offset_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void enter_idle_hop_state(void *ctx, struct Enemy *obj);
void restart_run_state(void *ctx, struct Enemy *obj);
void set_flag_enter_idle_hop(void *ctx, struct Enemy *obj);
void setup_pick_next_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void setup_owner_offset_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void update_facing_from_motion(void *ctx, struct Enemy *obj);
void move_handler_random_wait(void *ctx, struct Enemy *obj, EnemyFn *out);
void start_random_move_wait(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_hop_landing(void *ctx, struct Enemy *obj);
void obj_callback_apply_and_run(void *ctx, struct Enemy *obj, EnemyFn *slot);
void setup_random_offset_settle(void *ctx, struct Enemy *obj, EnemyFn *out);
void move_handler_timed_callback(void *ctx, struct Enemy *obj, EnemyFn *out);
void setup_offset_target_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void move_handler_timed_settle(void *ctx, struct Enemy *obj, EnemyFn *out);
void setup_random_offset_short(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_hop_phase(void *ctx, struct Enemy *obj);
void obj_link_sprites(void);
u16 angle_of_vector16(s32 x, s32 y);
void obj_set_stage(struct Enemy *ent, u32 stage);
void call_start_linked_state(void *ctx, struct Enemy *obj);
void enter_run_state_a311(void *ctx, struct Enemy *obj);
void enter_state_a6cd(void *ctx, struct Enemy *obj);
void enter_state_a3b5(void *ctx, struct Enemy *obj);
void enter_state_hop_landing(void *ctx, struct Enemy *obj);
void enter_run_state_a511(void *ctx, struct Enemy *obj);
void enter_run_state_76b5(void *ctx, struct Enemy *obj);
void enter_run_state_a5d5(void *ctx, struct Enemy *obj);
void enter_idle_hop_a689(void *ctx, struct Enemy *obj);
void reset_record_fields(struct Enemy *obj);
void obj_box_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index);
void obj_box_mirror_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index);
void load_box_oriented(s16 *out, struct Enemy *obj, u32 flags, u32 offset);
void load_hop_entry(struct Enemy *ent, s16 index);
void cpu_set(const void *src, void *dst, u32 ctl);

struct Nibble33D
{
    u8 filler0[0x33D];
    u8 lo4 : 4;
    u8 hi4 : 4;
};

static inline void decrement_body_timer(struct Enemy *body)
{
    body->unk1D2--;
}

static inline s32 clear_timer_flag(s32 flags)
{
    return flags & ~2;
}

struct PathFlags130
{
    u8 lo2 : 2;  /* the path record's byte 0 */
    u8 bit2 : 1; /* the path record's byte 1 */
    u8 bit3 : 1; /* cleared when a path is loaded */
};

/* The path an enemy walks, over its bytes from 0x130 (load_path_points). */
struct EnemyPath
{
    u8 filler0[0x130];
    u8 flags; /* struct PathFlags130 */
    u8 count; /* number of points */
    u16 x[16];
    u16 y[16];
    u8 filler172[0x192 - 0x172];
    u8 unk192; /* cleared when a path is loaded */
    u8 unk193; /* cleared when a path is loaded */
};

void obj_hop_landing_step(struct Enemy *obj)
{
    if (obj->jumpFrames != -1)
    {
        obj->jumpFrames++;
        obj->unk18 = div_scaled_fixed(obj->jumpFrames, obj->hopDist, obj->hopValue, obj->hopSpeed0) + obj->hopZ0 - obj->posZ;
        if (obj->unk18 <= 0)
        {
            obj->unk18 = 0;
            obj->jumpFrames = -1;
            ((struct Flags8 *)&obj->unk2B5)->bit3 = 0;
        }
    }
}

/* One frame of circling round (orbitCenX, orbitCenY) (whole pixels) on an ellipse with radii
   orbitRadX/Y (quarter pixels: radius * sine / 64 is 8.8 fixed point): sets the position, the height
   that puts it at orbitCenZ, turns the phase angle unk2F4 by unk2F8 and keeps the frame's motion in
   vx/vy. */
void obj_orbit_circle_step(struct Enemy *obj)
{
    s32 offset;
    s32 *radiusX = &obj->orbitRadX;

    offset = *radiusX * gCosineTable[obj->unk2F4 & 0xFF];
    if (offset < 0)
    {
        offset += 0x3F;
    }
    obj->posX = (offset >> 6) + (obj->orbitCenX << 8);

    offset = obj->orbitRadY * gSineTable[obj->unk2F4 & 0xFF];
    if (offset < 0)
    {
        offset += 0x3F;
    }
    obj->posY = (offset >> 6) + (obj->orbitCenY << 8);

    obj->unk18 = (obj->orbitCenZ << 8) - obj->posZ;
    obj->unk2F4 = (obj->unk2F4 + obj->unk2F8) & 0xFF;

    obj->vx = obj->posX - obj->prevX;
    obj->vy = obj->posY - obj->prevY;
}

NONMATCH("asm/code_081393B0/obj_pick_dir_toward_owner.s", u32 obj_pick_dir_toward_owner(struct Owner *owner, struct Enemy *obj))
{
    struct HitBox *hit = (struct HitBox *)((u8 *)obj + 0x100);
    struct Partner *ent;
    s32 px;
    s32 py;
    u8 flag;
    u16 margin;
    s32 roll;
    s32 t;

    if ((obj->unk140 | obj->unk141) == 0)
    {
        return 0xFF;
    }
    ent = owner_entry(owner, 0xFF);
    t = obj->posX;
    if (t < 0)
    {
        t += 0xFF;
    }
    px = t >> 8;
    t = obj->posY;
    if (t < 0)
    {
        t += 0xFF;
    }
    py = t >> 8;
    flag = *(u8 *)((u8 *)obj + 0x136) & 1;
    margin = *(s16 *)((u8 *)hit + 0x30) * flag;
    if (ent == NULL)
    {
        return 0xFF;
    }
    roll = (s16)rand_below(0x64, mt19937_next());
    if ((s8)obj->unk140 > roll)
    {
        if (obj->posX >= ent->posX)
        {
            if (px - (s16)margin <= hit->hitLeft)
            {
                return 0xFF;
            }
            return 2;
        }
        if (px + (s16)margin >= hit->hitRight)
        {
            return 0xFF;
        }
        return 3;
    }
    roll = roll - (s8)obj->unk140;
    if ((s8)obj->unk141 <= (s16)roll)
    {
        return 0xFF;
    }
    if (obj->posY >= ent->posY)
    {
        if (py - (s16)margin <= hit->hitBottom)
        {
            return 0xFF;
        }
        return 0;
    }
    if ((s16)margin + py >= hit->hitTop)
    {
        return 0xFF;
    }
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_081393B0/load_record_fields.s", void load_record_fields(struct Enemy *obj, s16 index))

ASM_FUNC("asm/code_081393B0/load_record_fields_scaled.s", void load_record_fields_scaled(struct Enemy *obj, s16 index))

/* Loads obj's box from the s16 record at obj->box[index]: flags, then left, bottom, right, top.
 * Flag bit 0: the edges are in tiles (16 px, centred); bit 1: they are relative to obj's position. */
void load_hit_box(struct Enemy *obj, s16 index)
{
    struct HitBox *hit = (struct HitBox *)((u8 *)obj + 0x100);
    s16 *box = obj->box + index;
    s16 *entry = box;
    register s16 *edge asm("r3") = box + 2;
    s32 v;

    box = 0;
    hit->hitLeft = hit_box_edge(entry[1], entry);
    {
        register s32 bottom asm("r0") = *edge++;
        hit->hitBottom = hit_box_edge(bottom, entry);
    }
    {
        register s32 right asm("r0") = *edge++;
        hit->hitRight = hit_box_edge(right, entry);
    }
    hit->hitTop = hit_box_edge(*edge, entry);

    v = hit->hitLeft;
    if (*(u8 *)entry & 2)
        v += obj->posX / 0x100;
    hit->hitLeft = v;
    v = hit->hitBottom;
    if (*(u8 *)entry & 2)
        v += obj->posY / 0x100;
    hit->hitBottom = v;
    v = hit->hitRight;
    if (*(u8 *)entry & 2)
        v += obj->posX / 0x100;
    hit->hitRight = v;
    v = hit->hitTop;
    if (*(u8 *)entry & 2)
        v += obj->posY / 0x100;
    hit->hitTop = v;
    box = 0;
}

/* Loads obj's path from the record at obj->box[index]: byte 0 the mode, byte 1 a flag, then a u16
 * point count and that many (x, y) u16 pairs. */
void load_path_points(struct Enemy *obj, s16 index)
{
    register u8 *rec asm("r2") = (u8 *)(obj->box + index);
    register u16 *src asm("r5") = (u16 *)(rec + 2);
    register s32 mode asm("r0") = rec[0];
    struct PathFlags130 *flags = (struct PathFlags130 *)&((struct EnemyPath *)obj)->flags;
    u16 *xs;
    register u16 *ys asm("r3");
    register u16 count asm("r2");
    register u8 *countp asm("r1");
    s32 zero;

    flags->lo2 = mode;
    flags->bit2 = rec[1];
    xs = ((struct EnemyPath *)obj)->x;
    ys = ((struct EnemyPath *)obj)->y;
    count = *src;
    countp = &((struct EnemyPath *)obj)->count;
    zero = 0;
    *countp = count;
    src++;
    ((struct EnemyPath *)obj)->unk193 = zero;
    ((struct EnemyPath *)obj)->unk192 = zero;
    flags->bit3 = 0;
    do {
        u32 shifted;
        register u32 step asm("r1");
        *xs = *src; src++; xs++;
        *ys = *src; src++; ys++;
        shifted = (u32)count << 16;
        step = 0xffff0000u;
        count = (shifted + step) >> 16;
    } while ((s16)count > 0);
}

void load_part_offsets(struct Enemy *obj, s16 index)
{
    struct PartOffsets *parts = (struct PartOffsets *)((u8 *)obj + 0x100);
    u16 *src = (u16 *)((u8 *)obj->rec + index * 2);
    s8 *bytes = (s8 *)(src + 4);
    s16 i;

    for (i = 0; i < 4; i++, bytes++)
    {
        parts->ids[i] = *src++;
        parts->offsZ[i] = bytes[0];
        parts->offsXY[i] = bytes[4];
        parts->offsXY[i + 4] = bytes[8];
    }
}

void obj_finish_move_step(struct Enemy *obj);
/* One step of a move started by obj_start_move_* (unkF8 bit 0x800 set): returns 0 when no move is
 * running or when obj has reached the target (orbitRadX/Y hold it during a move; the move is then
 * finished), else 1. A move blocked on X (unk24 bits 3-6) or Y (unk25 bits 3-6) stops obj; if
 * unk2B6 bit 6 (X) or 7 (Y) says so, the move gives up (returns 0) there. While blocked the wait
 * count unk2DE runs down first; otherwise obj takes the move's velocity (unk2D8, unk2DA) again. */
u32 advance_move_step(void *ctx, struct Enemy *obj)
{
    u8 *state = (u8 *)obj + 0x58;
    s32 blocked = 0;

    if (!(obj->unkF8 & 0x800))
        return 0;
    if (obj->posX == obj->orbitRadX && obj->posY == obj->orbitRadY)
    {
        obj_finish_move_step(obj);
        obj->unkF8 ^= 0x800;
        return 0;
    }
    // clear bits 2-5 of unk1D6
    ((struct Flags8 *)(state + 0x17E))->bit2 = 0;
    ((struct Flags8 *)(state + 0x17E))->bit3 = 0;
    ((struct Flags8 *)(state + 0x17E))->bit4 = 0;
    ((struct Flags8 *)(state + 0x17E))->bit5 = 0;
    if (obj->unk24 & 0x78)
    {
        // ctx holds 0x38C-byte entries, indexed by obj's first byte
        if ((obj->unk20E & 0x38) != 0x18 || (*(s8 *)obj & 0xFE) != 0
            || !(*((u8 *)ctx + *(s8 *)obj * 0x38C + 0x37A) & 8))
        {
            if (!(obj->unk213 & 0x1E))
            {
                obj->vy = 0;
                obj->vx = 0;
                if (obj->unk2B6 & 0x40)
                {
                    if (obj->unk2DE != 0)
                        obj->unk2DE--;
                    obj->jumpFrames = -1;
                    ((struct Flags8 *)&obj->unk2B5)->bit3 = 0;
                    return 0;
                }
            }
            blocked |= 1;
        }
    }
    if (obj->unk25 & 0x78)
    {
        // bits 3-4 of unk213
        if (!((((u32)obj->unk213 << 27) >> 28) & 0xC))
        {
            obj->vy = 0;
            obj->vx = 0;
            if (obj->unk2B6 & 0x80)
            {
                if (obj->unk2DE != 0)
                    obj->unk2DE--;
                obj->jumpFrames = -1;
                ((struct Flags8 *)&obj->unk2B5)->bit3 = 0;
                return 0;
            }
        }
        blocked |= 1;
    }
    if (blocked && obj->unk2DE != 0)
    {
        obj->unk2DE--;
    }
    else
    {
        obj->vx = obj->unk2D8;
        obj->vy = obj->unk2DA;
    }
    return 1;
}

/* Returns 1 when obj, standing at (x, y, z) (8.8 fixed point), overlaps the box of the owner's
 * second party member: only while the owner's mode (bits 2-4 of unk246) is 3, the partner's
 * unk20C bit 0 is set and both are on the same height. The boxes get 2 px of slack; obj's
 * height is compared unscaled. */
u32 position_blocked(struct Owner *owner, struct Enemy *obj, u32 x, u32 y, u32 z)
{
    struct Partner *partner;
    s32 t;
    s32 partnerX, partnerY, partnerZ;
    s32 px = x, py = y, pz = z;
    s32 partnerW, partnerH, halfW, myHalfW, left;

    partner = owner->party[1];
    t = partner->posX;
    if (t < 0)
    {
        t += 0xFF;
    }
    partnerX = t >> 8;
    t = partner->posY;
    if (t < 0)
    {
        t += 0xFF;
    }
    partnerY = t >> 8;
    t = partner->posZ;
    if (t < 0)
    {
        t += 0xFF;
    }
    partnerZ = t >> 8;
    partnerW = partner->width >> 8;
    partnerH = partner->height >> 8;
    t = px;
    if (t < 0)
    {
        t += 0xFF;
    }
    px = t >> 8;
    t = py;
    if (t < 0)
    {
        t += 0xFF;
    }
    py = t >> 8;
    t = pz;
    if (t < 0)
    {
        t += 0xFF;
    }
    pz = t >> 8;

    if ((owner->unk246 & 0x1C) != 0xC)
    {
        return 0;
    }
    if ((partner->unk20C & 1) == 0)
    {
        return 0;
    }
    if (partnerZ != pz)
    {
        return 0;
    }
    halfW = (u16)partnerW >> 1;
    left = partnerX - halfW - 2;
    myHalfW = obj->width >> 9;
    if (left > px + myHalfW)
    {
        return 0;
    }
    if (partnerX + halfW + 2 < px - myHalfW)
    {
        return 0;
    }
    if (partnerY - partnerH - 2 > py)
    {
        return 0;
    }
    if (partnerY + 2 < py - obj->height)
    {
        return 0;
    }
    return 1;
}

u32 div_scaled_fixed(u32 a, u32 x, u32 b, u32 k)
{
    x = (s32)x / 8;
    a *= k;
    a -= x;
    return (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(-b * a * a, x * x) + b) << 8;
}

struct Partner *owner_entry(struct Owner *owner, u16 index)
{
    u8 idx;

    if (index >= owner->unk10B)
    {
        if (find_active_entry(owner, &idx) == 0)
        {
            goto fail;
        }
        index = idx;
    }
    return *(struct Partner **)((u8 *)owner + index * 4 + 0x28);
fail:
    return NULL;
}

void load_hop_entry(struct Enemy *ent, s16 index)
{
    s16 *entry = (s16 *)ent->rec + index;

    ent->unk2F4 = entry[0];
    ent->unk2F8 = ((u16 *)entry)[1];
}

void set_timer_from_table(struct Enemy *ent, s16 index)
{
    ent->unk1D4 = ((u16 *)ent->rec)[index];
}

void load_hit_box_bytes(struct Enemy *ent, s16 index)
{
    struct HitBox *hit = (struct HitBox *)((u8 *)ent + 0x100);
    s8 *entry = (s8 *)ent->rec + index * 2;

    hit->hitLeft = (entry[0] << 4) + 8;
    entry++;
    hit->hitBottom = (entry[0] << 4) + 8;
    entry++;
    hit->hitRight = (entry[0] << 4) + 8;
    hit->hitTop = (entry[1] << 4) + 8;
}

void load_hit_box_words(struct Enemy *ent, s16 index)
{
    struct HitBox *hit = (struct HitBox *)((u8 *)ent + 0x100);
    s16 *entry = ent->box + index;
    s32 v = entry[0];

    hit->hitLeft = (v << 4) + 8;
    entry++;
    v = entry[0];
    hit->hitBottom = (v << 4) + 8;
    entry++;
    v = entry[0];
    hit->hitRight = (v << 4) + 8;
    v = entry[1];
    hit->hitTop = (v << 4) + 8;
}

void load_record_words_128(struct Enemy *ent, s16 index)
{
    u16 *src = (u16 *)ent->rec + index;
    s16 i;

    for (i = 0; i < 4; i++, src++)
    {
        ent->unk128[i] = *src;
    }
}

void obj_pick_random_entry(struct Enemy *ent)
{
    u16 *dist = ent->wanderDist;
    struct Sprite *sprite;
    s32 pick;
    u16 value;

    pick = rand_below(4, mt19937_next());
    sprite = ent->sprite;
    value = ent->unk108[(u16)pick];
    ent->unk239 = value;
    sprite->unk20 = value;
    dist += (u16)pick;
    ent->unk23E = *dist;
}

void tick_fire_indirect(void)
{
}

void sub_08139F08(void)
{
}

void body_timer_tick(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    struct Enemy *body = ent->body;

    decrement_body_timer(body);
    if (ent->unk1D2 == 0)
    {
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
        *out = body->unk1AC;
        (*out)(ctx, ent, out);
    }
}

void tick_body_timer(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    struct Enemy *body = ent->body;
    s32 t = body->unk1D2 - 1;

    body->unk1D2 = t;
    if ((u16)t == 0)
    {
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
        *out = body->unk1AC;
    }
}

void update_hop_phase(struct Enemy *obj)
{
    s32 t;

    obj->unk18 += gSineTable[obj->unk2F4 & 0xFF] / 0x80;
    t = obj->unk2F8 * (s8)obj->unk239;
    obj->unk2F4 = ((t + 8) / 0x10 + obj->unk2F4) & 0xFF;
}

void obj_jump_arc_step(struct Enemy *obj)
{
    s32 flags;

    if (obj->jumpFrames != -1)
    {
        obj->unk18 += obj->jumpVz;
        obj->jumpVz -= obj->accel;
        obj->jumpFrames++;
        if (obj->unk18 <= 0)
        {
            obj->unk18 = 0;
            obj->jumpFrames = -1;
            ((struct Flags8 *)&obj->unk2B5)->bit3 = 0;
        }
    }
}

void stub_nop_0813A064(void)
{
}

void start_linked_state(struct Owner *owner, struct Enemy *obj)
{
    owner_entry(owner, ((struct HitBox *)((u8 *)obj + 0x100))->linkIndex);
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, 0);
    obj_set_cell_coord(obj, (((u32)obj->unk24 << 29) >> 30) + 4, 0);
    flags_obj_set_mode(obj, 0x10);
    obj->unk198 = obj_link_sprites;
}

void set_flags_1d6_copy(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
    ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
    out[0] = out[1];
}

void advance_handler_slot(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    if (ent->unk214 & 0x40)
    {
        obj_apply_state_a(ent);
    }
    out[0] = out[1];
}

void link_to_owner_entry(struct Owner *owner, struct Enemy *obj, EnemyFn *out)
{
    u8 idx;

    if (find_active_entry(owner, &idx) == 0)
    {
        idx = (u32)(owner->flags208 << 29) >> 31;
    }
    {
        struct Partner *bro = *(struct Partner **)((u8 *)owner + idx * 4 + 0x28);
        obj_start_move_quarter(obj, 0, bro->posX, bro->posY);
    }
    obj_move_interp(obj);
    ((struct Flags8 *)&obj->unk1D6)->bit1 = 1;
    ((struct Flags8 *)&obj->unk1D6)->bit0 = 0;
    ((struct Flags8 *)&obj->unk1D6)->bit3 = 0;
    ((struct Flags8 *)&obj->unk1D6)->bit2 = 0;
    *out = move_step_handler;
}

void move_step_handler(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    if ((u8)advance_move_step(ctx, ent) == 1)
    {
        obj_move_interp(ent);
    }
    else
    {
        ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        out[0] = out[1];
    }
}

void obj_state_run_advance(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot = &ent->unk1A0[0];
    EnemyFn fn = slot[0];
    u8 flags;

    fn(ctx, ent, slot);
    flags = ent->unk1D6;
    if ((flags & 2) == 0 && (flags << 31) != 0)
    {
        ent->unk198 = ent->unk19C;
    }
}

void enter_run_state(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    load_record_fields(ent, 0);
    load_path_points(ent, 0);
    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_offset_target_state;
    ent->unk1A0[1] = setup_offset_target_state;
    fn(ctx, ent);
}

void setup_random_offset_state(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    u32 dir = obj_pick_dir_toward_owner(ctx, ent);

    obj_random_offset(ent, (u16)dir);
    obj_start_move_to_target(ent, 1, ent->vecX << 8, ent->vecY << 8);
    obj_step_toward_target_08138FDC(ent);
    *out = move_handler_timed_settle;
}

void enter_idle_hop_state(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot;
    EnemyFn fn;

    load_record_fields(ent, 0);
    load_hit_box(ent, 0);
    ent->unk198 = obj_state_run_hop_phase;
    slot = &ent->unk1A0[0];
    ent->unk1A0[0] = setup_random_offset_state;
    ent->unk1A0[1] = setup_random_offset_state;
    fn = slot[0];
    fn(ctx, ent, slot);
    if (ent->unk2F8 != 0)
    {
        update_hop_phase(ent);
    }
}

void restart_run_state(void *ctx, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk2B6)->bit6 = 0;
    enter_run_state(ctx, ent);
}

void set_flag_enter_idle_hop(void *ctx, struct Enemy *ent)
{
    int flags = ent->unk2B6;

    flags |= 0x40;
    ent->unk2B6 = flags;
    enter_idle_hop_state(ctx, ent);
}

void setup_pick_next_state(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Partner *entry = owner_entry(ctx, ((struct HitBox *)((u8 *)obj + 0x100))->linkIndex);

    obj_pick_move_vector(obj, entry);
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_try_start_hop(obj);
    obj_step_toward_target_08138FDC(obj);
    *out = obj_pick_next_state;
}

void setup_owner_offset_state(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    struct Partner *entry = owner_entry(ctx, ((struct HitBox *)((u8 *)ent + 0x100))->linkIndex);

    obj_pick_move_vector(ent, entry);
    obj_start_move_to_target(ent, 1, ent->vecX << 8, ent->vecY << 8);
    obj_step_toward_target_08138FDC(ent);
    *out = move_handler_timed_settle;
}

void update_facing_from_motion(void *ctx, struct Enemy *obj)
{
    s32 dx;
    s32 dy;
    u32 stage;

    obj_orbit_circle_step(obj);
    dx = obj->posX - obj->prevX;
    dy = obj->posY - obj->prevY;
    if ((dx | dy) != 0)
    {
        stage = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
    }
    else
    {
        stage = 0xFFFF;
    }
    if (stage != (((u32)obj->unk24 << 29) >> 29))
    {
        obj_set_stage(obj, (u8)stage);
    }
    obj->vx = 1;
}

void move_handler_random_wait(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    if ((u8)advance_move_step(ctx, obj) == 1)
    {
        obj_step_toward_target_08138FDC(obj);
        if ((obj->unk2B5 & 8) == 0)
        {
            obj_try_start_hop(obj);
        }
    }
    else
    {
        obj->unk1D2 = obj->unk128[rand_below(4, mt19937_next())];
        out[0] = obj->unk1D2 == 0 ? out[1] : tick_store;
    }
}

void start_random_move_wait(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    obj_random_facing_target(ent);
    obj_velocity_from_facing(ent);
    obj_try_start_hop(ent);
    obj_step_toward_target_08138FDC(ent);
    *out = move_handler_random_wait;
}

void obj_state_run_hop_landing(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot = &ent->unk1A0[0];
    EnemyFn fn = slot[0];

    fn(ctx, ent, slot);
    if (ent->unk2B5 & 8)
    {
        obj_hop_landing_step(ent);
    }
}

void obj_callback_apply_and_run(void *ctx, struct Enemy *ent, EnemyFn *slot)
{
    if (ent->unk214 & 0x40)
    {
        obj_apply_state_a(ent);
    }
    slot[1](ctx, ent, slot);
}

void setup_random_offset_settle(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    obj_random_offset(obj, 0xFF);
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_try_start_hop(obj);
    obj_step_toward_target_08138FDC(obj);
    obj_hop_landing_step(obj);
    *out = obj_pick_next_state;
}

void move_handler_timed_callback(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    if ((u8)advance_move_step(ctx, obj) == 1)
    {
        obj_move_interp(obj);
    }
    else
    {
        obj->unk1D2 = obj->unk128[rand_below(4, mt19937_next())];
        if (obj->unk1D2 != 0)
        {
            out[0] = tick_store;
            obj->unk1AC = out[1];
        }
        else
        {
            out[0] = out[1];
            ((struct Flags8 *)&obj->unk1D6)->bit1 = 0;
            ((struct Flags8 *)&obj->unk1D6)->bit0 = 1;
        }
    }
}

void setup_offset_target_state(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    obj_random_offset_step(ent);
    obj_start_move_quarter(ent, 0, ent->vecX << 8, ent->vecY << 8);
    obj_move_interp(ent);
    *out = move_handler_timed_callback;
}

void move_handler_timed_settle(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    if ((u8)advance_move_step(ctx, obj) == 1)
    {
        obj_step_toward_target_08138FDC(obj);
    }
    else
    {
        obj->unk1D2 = obj->unk128[rand_below(4, mt19937_next())];
        if (obj->unk1D2 != 0)
        {
            out[0] = tick_store;
            obj->unk1AC = out[1];
        }
        else
        {
            out[0] = out[1];
            ((struct Flags8 *)&obj->unk1D6)->bit1 = 0;
            ((struct Flags8 *)&obj->unk1D6)->bit0 = 1;
        }
    }
}

void setup_random_offset_short(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    obj_random_offset(obj, 0xFF);
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_step_toward_target_08138FDC(obj);
    *out = move_handler_timed_settle;
}

void obj_state_run_hop_phase(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot = &ent->unk1A0[0];
    EnemyFn fn = slot[0];

    fn(ctx, ent, slot);
    if (ent->unk2F8 != 0)
    {
        update_hop_phase(ent);
    }
}

void call_start_linked_state(void *ctx, struct Enemy *ent)
{
    start_linked_state(ctx, ent);
}

void enter_run_state_a311(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_pick_next_state;
    ent->unk1A0[1] = setup_pick_next_state;
    ent->unk1AC = obj_callback_apply_and_run;
    fn(ctx, ent);
}

void enter_state_a6cd(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = obj_state_run_hop_phase;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_owner_offset_state;
    ent->unk1A0[1] = setup_owner_offset_state;
    fn(ctx, ent);
}

void enter_state_a3b5(void *ctx, struct Enemy *ent)
{
    ent->unk198 = update_facing_from_motion;
    ent->unk198(ctx, ent);
}

void enter_state_hop_landing(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = obj_state_run_hop_landing;
    fn = ent->unk198;
    ent->unk1A0[0] = start_random_move_wait;
    ent->unk1A0[1] = start_random_move_wait;
    ent->unk1AC = start_random_move_wait;
    fn(ctx, ent);
}

void enter_run_state_a511(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_random_offset_settle;
    ent->unk1A0[1] = setup_random_offset_settle;
    ent->unk1AC = obj_callback_apply_and_run;
    fn(ctx, ent);
}

void enter_run_state_76b5(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = obj_random_pose;
    ent->unk1A0[1] = obj_random_pose;
    fn(ctx, ent);
}

void enter_run_state_a5d5(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_offset_target_state;
    ent->unk1A0[1] = setup_offset_target_state;
    fn(ctx, ent);
}

void enter_idle_hop_a689(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = obj_state_run_hop_phase;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_random_offset_short;
    ent->unk1A0[1] = setup_random_offset_short;
    fn(ctx, ent);
}

/* Loads the enemy's movement parameters from its record: a u16 mask, then one block per set bit.
 * Bit 0: 4 u16 wait times (unk128), bit 1: 4 u16 wander distances, bit 2: 4 s8 values (unk108),
 * bit 3: 4 u16 (unk110), bit 4: 4 u16 (unk118), bit 5: 2 s8 wander steps, bit 6: the two chances
 * (percent) of stepping toward the owner along x and y; bits 12-15 go to unk33D's high nibble.
 * u16 blocks are 8 bytes. Bit 6 reads from where the last s8 block ended, as the game does. */
void reset_record_fields(struct Enemy *obj)
{
    s32 highBits;
    u8 *fields = (u8 *)obj + 0x100;
    u8 *src;
    u8 *cursor;
    u32 maskWord;
    u32 mask;

    src = (u8 *)obj->rec;
    maskWord = *(u16 *)src;
    src += 2;
    maskWord <<= 16;
    mask = (s32)maskWord >> 16;
    if ((mask & 1) != 0)
    {
        cpu_set(src, (u8 *)obj + 0x128, 4); /* unk128 */
        src += 8;
    }
    if ((mask & 2) != 0)
    {
        cpu_set(src, fields, 4); /* wanderDist */
        src += 8;
    }
    if ((mask & 4) != 0)
    {
        cursor = src;
        ((s16 *)fields)[4] = *(s8 *)cursor; /* unk108[0..3] */
        cursor++;
        ((s16 *)fields)[5] = *(s8 *)cursor;
        cursor++;
        ((s16 *)fields)[6] = *(s8 *)cursor;
        cursor++;
        ((s16 *)fields)[7] = *(s8 *)cursor;
        cursor++;
        src = cursor;
    }
    if ((mask & 8) != 0)
    {
        cpu_set(src, (u8 *)obj + 0x110, 4); /* unk110 */
        src += 8;
    }
    if ((mask & 0x10) != 0)
    {
        cpu_set(src, (u8 *)obj + 0x118, 4); /* unk118 */
        src += 8;
    }
    if ((mask & 0x20) != 0)
    {
        cursor = src;
        ((s16 *)fields)[0x18] = *(s8 *)cursor; /* wanderSteps[0..1] */
        cursor++;
        ((s16 *)fields)[0x19] = *(s8 *)cursor;
        cursor++;
    }
    if ((mask & 0x40) != 0)
    {
        *((u8 *)obj + 0x140) = cursor[0]; /* unk140, unk141 */
        *((u8 *)obj + 0x141) = cursor[1];
    }
    highBits = 0xF000;
    highBits &= mask;
    if (highBits != 0)
    {
        ((struct Nibble33D *)obj)->hi4 = maskWord >> 28;
    }
}

ASM_FUNC("asm/code_081393B0/obj_box_to_stack.s", void obj_box_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index))

void obj_box_mirror_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index)
{
  register u32 sel asm("r2");
  u32 high = flags >> 4;
  u32 one = 1;
  s32 val;
  one &= ~high;
  sel = one;
  if (flags & 4)
  {
    register u32 off asm("r1") = one * (sizeof(void *));
    register u8 *base asm("r0") = ((u8 *) obj) + 0x1B0;
    s8 *p = (*((s8 **) (base + off))) + index;
    out[0] = (-(*(p++))) << 4;
    out[3] = (-(*(p++))) << 4;
    out[1] = p[0] << 4;
    out[2] = p[1] << 4;
  }
  else
    if (flags & 8)
  {
    register u32 off asm("r1") = sel * (sizeof(void *));
    register u8 *base asm("r0") = ((u8 *) obj) + 0x1B0;
    s16 *p = (s16 *) ((*((u8 **) (base + off))) + index);
    val = *p;
    out[0] = (-val) << 4;
    p++;
    val = *p;
    out[3] = (-val) << 4;
    p++;
    val = p[0];
    out[1] = val << 4;
    val = p[1];
    out[2] = val << 4;
    off = off;
  }
}

ASM_FUNC("asm/code_081393B0/load_box_oriented.s", void load_box_oriented(s16 *out, struct Enemy *obj, u32 flags, u32 offset))
