#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    s8 unk20; // animation speed, 0x10 = normal
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

struct SpriteBits11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 rest : 1;
};

struct MotionBits
{
    u8 lo4 : 4;
    u8 xMode : 2;
    u8 yMode : 2;
};

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

struct SpriteBits12
{
    u8 bit0 : 1;
    u8 field1 : 2;
    u8 rest : 5;
};

struct OwnerFlags
{
    u8 unk0 : 2;
    u8 bit2 : 1;
    u8 unk3 : 5;
};

struct Partner
{
    u8 filler0[0x4];
    s16 unk4;
    u8 filler6[0xC - 0x6];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
          u8 filler38[0x200 - 0x38];
      void *unk200;
      void *unk204;
    struct OwnerFlags flags208;
};

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
    u8 filler3F[0x46 - 0x3F];
    u8 unk46;
    u8 filler47[0x4A - 0x47];
    u16 unk4A[1];
};

struct Enemy
{
    u8 filler0[0x2];
    u8 unk2; // heading: 256-step direction it faces (0xFF: none)
    u8 filler3[0xC - 0x3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18; // drawn height above posZ, 1/256 pixels
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x34 - 0x25];
    s32 prevX;
    s32 prevY;
    u8 filler3C[0x100 - 0x3C];
    u16 wanderDist[4];
    u8 filler108[0x130 - 0x108];
    u16 wanderSteps[2];
    u8 filler134[0x198 - 0x134];
    void *unk198;
    void *unk19C;
    void *unk1A0[2];
    u8 filler1A8[0x1B0 - 0x1A8];
    struct Record *rec;
    u8 filler1B4[0x1D0 - 0x1B4];
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
          u8 filler1EC[0x20C - 0x1EC];
      u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x214 - 0x211];
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x23E - 0x217];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
          u8 filler246[0x248 - 0x246];
    s32 stepLen;
          u8 filler24C[0x276 - 0x24C];
      u16 unk276;
      u8 filler278[0x27A - 0x278];
      u16 unk27A;
          u8 filler27C[0x290 - 0x27C];
      void *layer290;
      struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2D8 - 0x2B7];
    u16 copyVX;
    u16 copyVY;
    u16 speed;
    u16 runLeft;
          u8 filler2E0[0x2E4 - 0x2E0];
      s32 unk2E4; // swing centre x (enemy_after_step_906c)
      s32 unk2E8; // swing centre y
            s32 unk2EC; // swing centre z; hover z for enemy_update_landing (eases to posZ + 0x2000)
        u32 unk2F0; // swing radius y, bounces between 0 and 22 * speed at the rate unk30C
    u32 unk2F4;   // swing radius x, likewise at the rate unk30A; hover bob phase (0-255)
    s16 unk2F8;   // hover bob speed, scaled by the sprite's animation speed
          u8 filler2FA[0x30A - 0x2FA];
      u16 unk30A; // swing radius x rate (s16)
      u16 unk30C; // swing radius y rate (s16)
      u16 unk30E; // swing angle x rate (s16, 0x100 = one step of 256)
      u16 unk310; // swing angle y rate
      u8 filler312[0x328 - 0x312];
      u32 unk328; // swing angle y (sine), 8.8
      u32 unk32C; // swing angle x (cosine), 8.8
      u8 filler330[0x338 - 0x330];
    void *unk338;
    u8 filler33C[0x33D - 0x33C];
    u8 unk33D;
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct Enemy *body;
};

u8 step_pos_xy_plain(struct Enemy *ent);
void reset_record_fields(struct Enemy *ent);
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 value);
void run_state_handler(void);
void tick_store(void);
void enemy_move_home(struct Owner *owner, struct Enemy *ent, u32 *out);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
// A byte the caller builds straight in its stack slot (slot address first, then the value):
// the zero-length array makes this one-byte union BLKmode, so it never goes through a register.
struct SlotFlag
{
    u8 value;
    u8 end[0];
} __attribute__((packed));
typedef union __attribute__((packed)) { struct SlotFlag flag; u8 value; } SlotFlagArg __attribute__((transparent_union));
void start_move_to(struct Enemy *ent, s32 x, s32 y, u32 speed, SlotFlagArg zero);
void start_move_to_3d(struct Enemy *obj, s32 x, s32 y, s32 z, u16 speed, ByteArg flag);
void enemy_z_fall_step(void);
void enemy_home_step(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_wander_to_edge(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_seek_partner(void);
void enemy_guard_aim(void);
void enemy_step_wait(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_hop_start(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_aim_and_move(struct Owner *owner, struct Enemy *ent, u32 *out);
u32 angle_of_vector16(s32 x, s32 y);
void enemy_move_to_target(struct Owner *owner, struct Enemy *ent, u32 *out);
void obj_box_mirror_to_stack(s16 *box, struct Enemy *ent, u32 flags, u32 index);
u8 boxes_overlap(s16 *box, struct Enemy *ent, struct Partner *other, s32 zAbove, s32 zBelow);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void load_record_fields(struct Enemy *ent, u32 mode);
void load_hit_box_words(struct Enemy *ent, u32 mode);
void clear_obj_flags(void *obj);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void enemy_zone_pick(void);
extern const u8 gFrameAttrTable[];
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void enemy_face_owner(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_place_on_owner(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_place_body_quadrant(struct Enemy *body);
void enemy_after_step_906c(struct Owner *owner, struct Enemy *ent, u32 *out);
u8 widths_overlap_x(struct Enemy *a, struct Partner *b);
u8 heights_overlap_y(struct Enemy *a, struct Partner *b);
void bump_attack_enemy_move_3d_step(void);
void enemy_wait_scan(void);
void enemy_fall_end_state(void);
void enemy_fall_decay(void);
void enemy_fall_pos(void);
void enemy_bump_wait(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_collide_settle(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_start_bump(struct Owner *owner, struct Enemy *ent, u32 *out);
void enemy_idle_walk(struct Owner *owner, struct Enemy *ent, u32 *out);
s8 zone_point_code(struct Owner *owner, struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void enemy_start_run(void);
void enemy_chase_partner(void);
void seek_run_enemy_return_home(struct Owner *owner, struct Enemy *ent, u32 *out);
u8 enemy_companion_hit(struct Owner *owner, struct Enemy *ent);
void enemy_pose_a(struct Owner *owner, struct Enemy *ent, u32 *out);
u8 enemy_touches_companion_oriented(struct Owner *owner, struct Enemy *ent);
u8 step_pos_xy_c(struct Enemy *ent);
void obj_refresh_sub_attr(struct Enemy *ent);
void collide_react(struct Owner *owner, struct Enemy *ent, void *target, s32 part, s32 speed, s32 a, s32 b, s32 c, s32 zero);
u8 tile_blocks_move(struct Owner *owner, s32 x, s32 y, s32 z, s32 w, s32 one, s32 zero);
void enemy_run_return(struct Owner *owner, struct Enemy *ent, u32 *out);
void wander_to_edge(struct Enemy *ent, s32 steps, s32 dist);
void obj_set_or_add_pos(struct Enemy *ent, s32 x, s32 y, s32 z, u32 zero0, u32 zero1);
u32 angle_of_vector16(s32 x, s32 y);

/* Enemy state while its body flies off: counts the timer unk1D2 down once (then shows the cell of
 * its quarter direction and sets unk1D6 bit 4) and moves the body by its velocity. While the body
 * is shown and its class method at 0x68 returns 0, it keeps flying. Otherwise the body is put away
 * and the enemy runs: after tick_store, enemy_start_run, then enemy_chase_partner when the owner's
 * zone has a point for it, else seek_run_enemy_return_home. */
void enemy_body_step(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Enemy *body = ent->body;
    s32 timer;

    if ((ent->unk1D6 & 0x10) == 0)
    {
        timer = ent->unk1D2 - 1;
        ent->unk1D2 = timer;
        if ((timer << 16) == 0)
        {
            /* direction (unk24 bits 0-2) / 2 */
            obj_set_cell_coord(ent, MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((u32)ent->unk24 << 29 >> 29, 2), 0);
            ((struct Flags8 *)&ent->unk1D6)->bit4 = 1;
        }
    }
    body->posX = body->posX + body->vx;
    body->posY = body->posY + body->vy;
    if ((body->unk20F & 2) != 0 && (body->unk216 & 0x20) == 0 && (*((u8 *)body + 0x20C) & 1) != 0)
    {
        /* method entry: s16 offset to add to the object, then the function */
        u8 *method = (u8 *)body->unk338 + 0x68;
        s32 thisOffset = *(s16 *)method;

        if (((u8 (*)(struct Enemy *))(*(u32 *)(method + 4)))((struct Enemy *)((u8 *)body + thisOffset)) == 0)
        {
            return;
        }
    }
    obj_set_cell_coord(ent, MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((u32)ent->unk24 << 29 >> 29, 2), 0);
    obj_set_active(body, 0);
    obj_set_sprite_link(body, 0);
    *(u16 *)(ai + 0x17A) = 1;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
    out[0] = (u32)tick_store;
    out[3] = (u32)enemy_start_run;
    if (zone_point_code(owner, ent, 0) != -1)
    {
        out[2] = (u32)enemy_chase_partner;
    }
    else
    {
        out[2] = (u32)seek_run_enemy_return_home;
    }
}

/* Enemy state after seek_run_enemy_return_home put it back home: for runLeft frames it shivers,
   stepping `speed` sideways every other frame and turning round (unk1D6 bit 2) every second step.
   Then it faces left, gets its flags and palette (rec byte 6 + index) back and goes on with
   enemy_wait_scan. */
void enemy_run_return(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    s32 step;

    ent->unk1D4 = ent->unk1D4 + 1;
    if (ent->unk1D4 > 1)
    {
        ent->unk1D2 = ent->unk1D2 + 1;
        if ((ent->unk1D6 & 4) != 0)
        {
            obj_set_sprite_link(ent, 0);
            ent->vx = ent->speed;
            step = ent->vx;
        }
        else
        {
            obj_set_sprite_link(ent, 1);
            ent->vx = -ent->speed;
            step = ent->vx;
        }
        ent->posX = ent->posX + step;
        /* ai + 0x17A..0x17E: ent->unk1D2, unk1D4, unk1D6 */
        if (*(u16 *)(ai + 0x17A) > 1)
        {
            if (((struct Flags8 *)(ai + 0x17E))->bit2)
            {
                ((struct Flags8 *)(ai + 0x17E))->bit2 = 0;
            }
            else
            {
                ((struct Flags8 *)(ai + 0x17E))->bit2 = 1;
            }
            *(u16 *)(ai + 0x17A) = 0;
        }
        *(u16 *)(ai + 0x17C) = 0;
    }
    {
        s32 framesLeft = ent->runLeft - 1;

        ent->runLeft = framesLeft;
        if ((framesLeft << 16) == 0)
        {
            obj_set_sprite_link(ent, 1);
            ((struct Flags8 *)((u8 *)ent + 0x20C))->bit5 = 1;
            ((struct Flags8 *)((u8 *)ent + 0x20C))->bit7 = 1;
            ((struct Flags8 *)((u8 *)ent + 0x20C))->bit6 = 1;
            ((struct Flags8 *)&ent->unk210)->bit4 = 1;
            ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
            ((struct Mode3 *)&ent->unk214)->mode = 1;
            {
                struct Sprite *sprite = ent->sprite;
                u32 index = *(u16 *)(ai + 0x178); /* ent->index */
                u32 rec = *(u32 *)(ai + 0x158);   /* ent->rec */

                sprite->unk20 = *((u8 *)(index + rec) + 6);
            }
            out[0] = (u32)enemy_wait_scan;
        }
    }
}

void enemy_clear_body_flags(struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk20E)->bit2 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit3 = 1;
    ((struct Flags8 *)&ent->unk20D)->bit5 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit6 = 0;
    *(u32 *)((u8 *)ent + 0x20C) = *(u32 *)((u8 *)ent + 0x20C) & 0xFFFC7FFF;
    ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit7 = 1;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit6 = 1;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit5 = 1;
    ((struct Flags8 *)&ent->unk20E)->bit2 = 0;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
}

void seek_run_enemy_return_home(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    obj_set_or_add_pos(ent, ent->homeX, ent->homeY, ent->homeZ + ent->homeDZ, 0, 0);
    ent->speed = 0x200;
    ent->runLeft = 0x3C;
    out[0] = (u32)enemy_run_return;
}

NONMATCH("asm/code_08147904/enemy_move_to_partner.s", void enemy_move_to_partner(struct Owner *owner, struct Enemy *ent, u32 *out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Record *rec = ent->rec;
    struct Partner *partner;
    u8 idx;
    u32 index;
    s32 pick;
    u16 speed;

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    }
    partner = ((struct Partner **)((u8 *)owner + 0x28))[index];
    pick = rand_below(4, mt19937_next());
    speed = *(u16 *)((u8 *)rec + pick * 2 + 0x2E);
    start_move_to(ent, partner->posX, partner->posY, speed, (u8)0);
    *(s32 *)(ai + 0x184) = ent->posX;
    *(s32 *)(ai + 0x188) = ent->posY;
    *out = (u32)enemy_step_wait;
}
END_NONMATCH

void enemy_start_guard(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;
    s32 pick = rand_below(4, mt19937_next());

    u8 *slot = (u8 *)(pick + (u32)rec);

    ent->unk1D4 = slot[0x1E];
    {
        u8 *p = &ent->unk1D6;
        *p = *p | 0x10;
    }
    *(u32 *)((u8 *)ent + 0x20C) = (*(u32 *)((u8 *)ent + 0x20C) & 0xFFFC7FFF) | 0x8000;
    {
        u8 *p = &ent->unk20F;
        *p = *p | 4;
    }
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    out[0] = (u32)enemy_guard_aim;
}

void enemy_wander_to_edge(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u16 *table = ent->wanderDist;
    s32 pick1 = rand_below(4, mt19937_next());
    u16 dist = table[pick1];
    s32 pick2 = rand_below(2, mt19937_next());
    u16 steps = *(u16 *)((u8 *)table + pick2 * 2 + 0x30);

    wander_to_edge(ent, steps, dist);
    ((struct Mode3 *)&ent->unk214)->mode = 3;
    out[0] = (u32)enemy_seek_partner;
}

void enemy_go_idle(struct Owner *owner, struct Enemy *ent)
{
    {
        u8 *p = &ent->unk33E;
        *p = *p | 0x10;
    }
    reset_record_fields(ent);
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_wander_to_edge;
}

void enemy_step_wait(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    if (step_pos_xy_plain(ent) == 0)
    {
        out[0] = (u32)enemy_move_home;
    }
}

void enemy_move_home(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;
    s32 pick = rand_below(4, mt19937_next());
    u16 speed = *(u16 *)((u8 *)rec + pick * 2 + 0x36);
    start_move_to(ent, ent->homeX, ent->homeY, speed, (u8)0);
    *out = (u32)enemy_home_step;
}

void enemy_home_step(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;

    if (step_pos_xy_plain(ent) == 0)
    {
        ent->unk1D2 = rec->unk3E;
        out[0] = (u32)tick_store;
        out[3] = (u32)enemy_wander_to_edge;
    }
}

/* Undoes enemy_go_idle: clears unk33E bit 4, reloads the enemy's record fields and hit box for
   mode 0, clears its body's flags (clear_obj_flags) and its own (movement mode 0, unk20C bits 5-7,
   unk20F bit 2; unk20E bit 2 set), detaches its sprite link and runs enemy_zone_pick under
   run_state_handler. */
void enemy_release_body(struct Owner *owner, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk33E)->bit4 = 0;
    load_record_fields(ent, 0);
    load_hit_box_words(ent, 0);
    clear_obj_flags(ent->body);
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    ((struct Flags8 *)&ent->unk20C)->bit7 = 0;
    ((struct Flags8 *)&ent->unk20C)->bit6 = 0;
    ((struct Flags8 *)&ent->unk20C)->bit5 = 0;
    obj_set_sprite_link(ent, 0);
    ((struct Flags8 *)&ent->unk20F)->bit2 = 0;
    ((struct Flags8 *)&ent->unk20E)->bit2 = 1;
    ent->unk198 = run_state_handler;
    ent->unk1A0[0] = enemy_zone_pick;
}

ASM_FUNC("asm/code_08147904/enemy_jump_partner.s", void enemy_jump_partner(void))

NONMATCH("asm/code_08147904/enemy_move_checked.s", void enemy_move_checked(struct Owner *owner, struct Enemy *ent, u32 *out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Record *rec;
    struct Partner *partner;
    u8 idx;
    u8 *slot;
    u32 index;
    s32 dy;
    s32 dzAdd;
    u8 blocked;

    slot = &idx;
    if (find_active_entry(owner, slot) == 0)
    {
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    }
    else
    {
        index = idx;
    }
    *slot = index;
    partner = owner->party[idx];
    ent->posX = ent->body->posX;
    ent->posY = ent->body->posY;
    ent->posZ = ent->body->posZ;
    dy = ent->body->homeY - partner->posY;
    if (dy >= 0)
    {
        dzAdd = dy + 0xA000;
    }
    else
    {
        dzAdd = 0xA000;
    }
    ent->unk18 = partner->unk18 + dzAdd;
    rec = *(struct Record **)(ai + 0x158);
    (void)*(u32 *)(ai + 0x15C);
    ent->speed = rec->unk30;
    *(u16 *)(ai + 0x17A) = rec->unk2E;
    blocked = tile_blocks_move(owner, ent->posX, ent->posY, ent->posZ, ent->unk27A, 1, 0);
    if (blocked != 0)
    {
        out[0] = (u32)enemy_fall_end_state;
        return;
    }
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit7 = 1;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit6 = 1;
    ((struct Flags8 *)((u8 *)ent + 0x20C))->bit5 = 1;
    obj_set_sprite_link(ent, 1);
    ((struct Flags8 *)&ent->unk20F)->bit2 = 1;
    *(u16 *)(ai + 0x17C) = rec->unk32;
    ent->unk23E = rec->unk34;
    out[0] = (u32)enemy_fall_decay;
}
END_NONMATCH

/* Runs the enemy's state handler, then (unless it moved on to enemy_hop_start or enemy_fall_pos)
   makes it hover: its drawn height is its hover z above posZ plus a sine bob, the bob phase advances
   by its bob speed times the sprite's animation speed, and the hover z eases by 0x60 a frame towards
   32 pixels (0x2000) above posZ. */
void enemy_update_landing(struct Owner *owner, struct Enemy *ent)
{
    void **handler = &ent->unk1A0[0];
    void *next;
    s32 *hoverZ;
    s32 hoverHeight;
    s32 bob;
    s32 phaseStep;
    s32 bobSpeed;
    s8 animSpeed;
    register s32 target asm("r2");
    s32 z;

    ((void (*)(struct Owner *, struct Enemy *, void **))*handler)(owner, ent, handler);
    next = *handler;
    if (next == enemy_hop_start || next == enemy_fall_pos)
        return;
    hoverZ = &ent->unk2EC;
    hoverHeight = *hoverZ - ent->posZ;
    bob = gSineTable[ent->unk2F4 & 0xFF] / 0x40;
    ent->unk18 = hoverHeight + bob * 2;
    bobSpeed = ent->unk2F8;
    animSpeed = ent->sprite->unk20;
    phaseStep = animSpeed * bobSpeed + 8;
    ent->unk2F4 = ((phaseStep / 0x10) + ent->unk2F4) & 0xFF;
    target = 0x2000;
    if (*hoverZ - ent->posZ > target)
    {
        *hoverZ -= 0x60;
        z = ent->posZ;
        if (*hoverZ - z > target)
            return;
        {
            register s32 t asm("r0") = z + target;
            *hoverZ = t;
        }
    }
    else
    {
        if (*hoverZ - ent->posZ >= target)
            return;
        *hoverZ += 0x60;
        z = ent->posZ;
        if (*hoverZ - z < target)
            return;
        *hoverZ = z + 0x2000;
    }
}

NONMATCH("asm/code_08147904/enemy_start_bump.s", void enemy_start_bump(struct Owner *owner, struct Enemy *ent, u32 *out))
{
    u16 *table = ent->wanderDist;
    u8 pick = rand_below(4, mt19937_next());
    u16 *entry;
    u8 idx;
    u32 index;
    struct Partner *partner;
    u16 part;

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    }
    partner = ((struct Partner **)((u8 *)owner + 0x28))[index];
    part = table[rand_below(2, mt19937_next()) + 0x18];
    entry = table + pick;
    collide_react(owner, ent, partner, part, entry[0], *(s8 *)((u8 *)table + 0x40), *(s8 *)((u8 *)table + 0x41), 0x22, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 3;
    ent->sprite->unk20 = entry[4];
    *out = (u32)enemy_bump_wait;
}
END_NONMATCH

/* Enemy state after enemy_start_bump: every rec->unk1A frames (counted down in unk1D4) it checks
   whether its box (0x14 x 0x22) overlaps the active brother's, and if enemy_companion_hit says
   so, goes on to enemy_aim_and_move. Otherwise it keeps moving until its steps run out, then
   waits wanderDist[0x14 + rand(4)] frames (tick_store counts unk1D2 down) and bumps again. */
void enemy_bump_wait(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58; /* ai + 0x17A is ent->unk1D2, ai + 0x17C ent->unk1D4 */
    struct Record *rec = ent->rec;
    u16 *table = ent->wanderDist;
    struct Partner *brother;
    s16 box[8];
    u8 activeIdx;

    if (ent->unk1D4 == 0)
    {
        if (find_active_entry(owner, &activeIdx) != 0)
        {
            brother = *(struct Partner **)((u8 *)owner + activeIdx * 4 + 0x28);
            obj_box_mirror_to_stack(box, ent, 0x14, 0x22);
            if (boxes_overlap(box, ent, brother, 0x7ff, 0x7fff) != 0)
            {
                if (enemy_companion_hit(owner, ent) != 0)
                {
                    out[0] = (u32)enemy_aim_and_move;
                    return;
                }
            }
        }
        {
            u16 checkInterval = rec->unk1A;

            *(u16 *)(ai + 0x17C) = checkInterval;
        }
    }
    else
    {
        ent->unk1D4 = ent->unk1D4 - 1;
    }
    if (step_pos_xy_c(ent) == 0)
    {
        s32 pick = rand_below(4, mt19937_next());
        u16 wait = table[pick + 0x14];

        *(u16 *)(ai + 0x17A) = wait;
        out[0] = (u32)tick_store;
        out[3] = (u32)enemy_start_bump;
    }
}

/* Chase state, entered from enemy_bump_wait: turns to face the active brother (or the default one),
 * sets the sprite's animation speed (rec byte 0x2A + index), heads for the brother at the speed
 * rec->unk2E[index] and gives the chase rec byte 0x36 + index frames (unk1D2) in enemy_move_to_target.
 * index (ai + 0x178) is the variant 0-3 picked for the current move. */
void enemy_aim_and_move(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai;
    struct Record *rec;
    struct Partner *brother;
    u8 activeIdx;
    u8 zero;
    u32 index;
    s32 dx;
    s32 dy;
    u32 heading;
    u16 chaseTime;
    u16 speed;

    ai = (u8 *)ent + 0x58;
    rec = ent->rec;
    if (find_active_entry(owner, &activeIdx) != 0) {
        index = activeIdx;
    } else {
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    }
    brother = ((struct Partner **)((u8 *)owner + 0x28))[index];
    dx = brother->posX - ent->posX;
    dy = brother->posY - ent->posY;
    if ((dx | dy) != 0) {
        heading = ((u32)((u16)angle_of_vector16(dx, dy) + 0x10)) >> 8;
    } else {
        heading = 0xff;
    }
    ent->unk2 = heading;
    ent->sprite->unk20 = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x2A];
    speed = (&rec->unk2E)[*(u16 *)(ai + 0x178)];
    start_move_to(ent, brother->posX, brother->posY, speed, zero = 0);
    chaseTime = ((u8 *)rec)[*(u16 *)(ai + 0x178) + 0x36];
    *(u16 *)(ai + 0x17A) = chaseTime;
    *out = (u32)enemy_move_to_target;
}

void enemy_move_to_target(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    register struct Enemy *enemy asm("r5") = ent;
    register u32 *slot asm("r6") = out;
    u8 *ai = (u8 *)enemy + 0x58;
    struct Partner *partner;
    struct { s16 box[8]; u8 idx; } local;
    u32 index;
    u32 next;
    register u8 *arg asm("r1");

    arg = (u8 *)&local;
    arg += 16;
    if (find_active_entry(owner, arg) != 0)
    {
        u8 *ip = &local.idx;

        index = *ip;
    }
    else
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    partner = ((struct Partner **)((u8 *)owner + 0x28))[index];
    obj_box_mirror_to_stack(local.box, enemy, 0x14, 0x26);
    if (boxes_overlap(local.box, enemy, partner, 0x7FFF, 0x7FFF) == 0)
        next = (u32)enemy_hop_start;
    else
    {
        u16 *counter = (u16 *)(ai + 0x17A);
        int value = *counter - 1;
        *counter = value;
        if ((u16)value != 0 && step_pos_xy_plain(enemy) != 0)
            return;
        next = (u32)enemy_aim_and_move;
    }
    *slot = next;
}

void start_z_move(struct Enemy *ent, s32 z, u32 a, u32 b, u32 c);
NONMATCH("asm/code_08147904/enemy_hop_start.s", void enemy_hop_start(struct Owner *owner, struct Enemy *ent, u32 *out))
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Record *rec = ent->rec;
    struct Partner *partner;
    u8 idx;
    u32 index;
    u16 speed;
    u8 kind;
    u8 zb;
    u8 zero = 0;
    s32 (*divFn)(s32, s32);
    s32 value;

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    }
    partner = ((struct Partner **)((u8 *)owner + 0x28))[index];
    speed = *(u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2 + 0x3E);
    start_move_to(ent, partner->posX, partner->posY, speed, zero);
    zb = *(u8 *)((u8 *)rec + *(u16 *)(ai + 0x178) + 0x3A);
    start_z_move(ent, ent->posZ + ent->unk18 + (zb << 8), speed, 0, 0);
    divFn = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    if ((ent->vx | ent->vy) != 0)
    {
        value = (((u16)angle_of_vector16(ent->vx, ent->vy) + 0x1000) >> 13) & 7;
    }
    else
    {
        value = -1;
    }
    kind = divFn(value, 2);
    if (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2) != kind)
    {
        obj_set_cell_coord(ent, kind, 0);
    }
    ((struct Mode3 *)&ent->unk214)->mode = 0;
    *(s32 *)(ai + 0x184) = ent->posX;
    *(s32 *)(ai + 0x188) = ent->posY;
    *(s32 *)(ai + 0x18C) = ent->posZ;
    *(s32 *)(ai + 0x190) = ent->unk18;
    *out = (u32)enemy_fall_pos;
}
END_NONMATCH

/* Hop back home: turns to face its home spot (homeX/homeY, saved by enemy_hop_start where the hop
 * began), moves back to it in 3D (homeZ + homeDZ, at the speed rec->unk4A[index]), takes sprite
 * mode 4 with the animation speed rec->unk46 and goes on to enemy_z_fall_step. As the original
 * does, index and the home spot are read through ent + 0x58 (ai + 0x178 = index, ai + 0x184 = homeX). */
void enemy_hop_3d(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58;
    struct Record *rec = ent->rec;
    s32 dx = ent->homeX - ent->posX;
    s32 dy = ent->homeY - ent->posY;
    u32 heading;
    u16 speed;
    u8 zero;

    if ((dx | dy) != 0)
    {
        heading = ((u32)((u16)angle_of_vector16(dx, dy) + 0x10)) >> 8;
    }
    else
    {
        heading = 0xff;
    }
    ent->unk2 = heading;
    speed = *(u16 *)((u8 *)rec + *(u16 *)(ai + 0x178) * 2 + 0x4A);
    start_move_to_3d(ent, *(s32 *)(ai + 0x184), *(s32 *)(ai + 0x188),
                     *(s32 *)(ai + 0x18C) + *(s32 *)(ai + 0x190), speed, zero = 0);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ent->sprite->unk20 = rec->unk46;
    *out = (u32)enemy_z_fall_step;
}

NONMATCH("asm/code_08147904/enemy_collide_settle.s", void enemy_collide_settle(struct Owner *owner, struct Enemy *ent, u32 *out))
{
    u16 *table = ent->wanderDist;
    u16 *entry;
    u8 pick = rand_below(4, mt19937_next());
    u8 idx;
    u32 index;
    struct Partner *partner;

    if (find_active_entry(owner, &idx) != 0)
    {
        index = idx;
    }
    else
    {
        index = (u32)(*(u8 *)&owner->flags208 << 30) >> 31;
    }
    partner = ((struct Partner **)((u8 *)owner + 0x28))[index];
    entry = table + pick;
    collide_react(owner, ent, partner, entry[0x18], entry[0], *(s8 *)((u8 *)ent->wanderDist + 0x40), *(s8 *)((u8 *)ent->wanderDist + 0x41), 0x22, 0);
    obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8], 0);
    ((struct Mode3 *)&ent->unk214)->mode = 3;
    ent->sprite->unk20 = entry[4];
    ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 0;
    *out = (u32)enemy_idle_walk;
}
END_NONMATCH

/*
 * Enemy state while it wanders (set up by enemy_collide_settle): every rec->unk1A frames
 * (counted down in unk1D4) it checks whether it touches the active brother, and if so goes
 * on to enemy_pose_a. Otherwise it keeps walking until its steps run out, then waits:
 * 40% of the time standing still (cell +0x10) for the record's byte at 0x4A + rand(4)
 * frames, else for wanderDist[0x14 + rand(4)] frames; then enemy_collide_settle again.
 */
void enemy_idle_walk(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u8 *ai = (u8 *)ent + 0x58; /* ai + 0x158 is ent->rec, ai + 0x17A ent->unk1D2 */
    struct Record *rec = ent->rec;
    u16 *table = ent->wanderDist;
    u16 *checkTimer = &ent->unk1D4;
    u32 roll;

    if (*checkTimer == 0)
    {
        if (enemy_touches_companion_oriented(owner, ent) != 0)
        {
            out[0] = (u32)enemy_pose_a;
            return;
        }
        *checkTimer = rec->unk1A;
    }
    else
    {
        (*checkTimer)--;
    }
    if (step_pos_xy_c(ent) != 0)
    {
        return;
    }
    roll = rand_below(100, mt19937_next());
    if (roll > 59)
    {
        s32 pick = rand_below(4, mt19937_next());
        u16 wait = *(u8 *)(pick + *(u32 *)(ai + 0x158) + 0x4A);

        *(u16 *)(ai + 0x17A) = wait;
        /* the standing cell for (unk216 bits 2-3, direction) */
        obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 0x10, 0);
        ((struct Mode3 *)&ent->unk214)->mode = 1;
        ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 0;
    }
    else
    {
        s32 pick = rand_below(4, mt19937_next());
        u16 wait = table[pick + 0x14];

        *(u16 *)(ai + 0x17A) = wait;
        obj_refresh_sub_attr(ent);
        ((struct Mode3 *)&ent->unk214)->mode = 0;
    }
    /* tick_store counts unk1D2 down, then switches to out[3] */
    out[0] = (u32)tick_store;
    out[3] = (u32)enemy_collide_settle;
}

/* Enemy state entered when it touches the active brother (from enemy_idle_walk): picks one of
 * four pose variants (index), plays the pose cell (+4) once at the record's speed for that
 * variant, flags the pose as running (unk1D6 bit 0) and hands over to enemy_face_owner. */
void enemy_pose_a(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    struct Record *rec = ent->rec;
    s32 variant = rand_below(4, mt19937_next());
    u16 *variantIndex = &ent->index;

    *variantIndex = variant;
    /* the pose cell for (unk216 bits 2-3, direction) */
    obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 4, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 4;
    ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 1;
    /* animation speed: the record's byte at 0x22 + variant */
    ent->sprite->unk20 = *((u8 *)(*variantIndex + (u32)rec) + 0x22);
    ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
    ((struct Flags8 *)&ent->unk20D)->bit2 = 0;
    out[0] = (u32)enemy_face_owner;
}

/*
 * Enemy state after enemy_pose_a, in two phases. While unk1D6 bit 0 is set it waits for the pose
 * animation to end (sprite flag 8), then loads a wait time (the record's s8 at 0x26 + index) and
 * plays the next animation. Once the wait has run out it turns towards the targeted brother (the
 * active one, else the one bit 1 of the owner's flags208 picks): sideways when he is farther away in
 * x than in y (direction 2 right / 6 left, mirrored for left), else up (0) or down (4); then plays
 * that direction's animation and moves on to enemy_place_on_owner.
 */
void enemy_face_owner(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u32 recBase = (u32)ent->rec;
    struct Partner *target;
    u8 activeIndex;
    s32 targetIndex;
    s32 dx;
    s32 dy;
    s32 absDx;
    s32 absDy;
    s32 down;
    struct Mode3 *direction;

    if (((struct Flags8 *)&ent->unk1D6)->bit0 != 0)
    {
        if ((ent->sprite->unk12 & 8) != 0)
        {
            ent->unk1D2 = *(s8 *)((u8 *)(ent->index + recBase) + 0x26);
            /* the animation cell for (unk216 bits 2-3, direction) */
            obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 8, 0);
            ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 0;
            ((struct Flags8 *)&ent->unk1D6)->bit0 = 0;
        }
    }
    else if (--ent->unk1D2 == 0)
    {
        if (find_active_entry(owner, &activeIndex) != 0)
            targetIndex = activeIndex;
        else
            targetIndex = ((u32)*(u8 *)&owner->flags208 << 30) >> 31;
        target = *(struct Partner **)((targetIndex << 2) + (u32)owner + 0x28); /* owner->party[targetIndex] */
        dx = target->posX - ent->posX;
        dy = target->posY - ent->posY;
        absDx = dx;
        if (dx < 0)
            absDx = -dx;
        absDy = dy;
        if (dy < 0)
            absDy = -dy;
        if (absDx > absDy)
        {
            ent->sprite->unk21 = 1;
            ((struct SpriteBits11 *)&ent->sprite->unk11)->bit6 = (u32)dx >> 31;
            ((struct Mode3 *)&ent->unk24)->mode = dx < 0 ? 6 : 2;
        }
        else
        {
            /* dy >= 0: down (direction 4, sprite row 2), else up (0) */
            ent->sprite->unk21 = ((~dy) >> 31) & 2;
            down = 4;
            direction = (struct Mode3 *)&ent->unk24;
            direction->mode = ((~dy) >> 31) & down;
        }
        obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 0xC, 0);
        ((struct Mode3 *)&ent->unk214)->mode = 1;
        ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 1;
        out[0] = (u32)enemy_place_on_owner;
    }
}

/*
 * Once the enemy's animation has reached its end (sprite flag 8), releases its body in front
 * of it (above, right, at, or left of it, by its facing direction), gives the body a random
 * one of four looks and starts its swing (centre at its position, zero radii and angles),
 * loads the swing timer from the record and hands over to enemy_after_step_906c.
 */
void enemy_place_on_owner(struct Owner *owner, struct Enemy *ent, u32 *out)
{
    u16 *ai = (u16 *)((u8 *)ent + 0x58); /* ai[0xBC] is ent->index, ai[0xBD] ent->unk1D2 */
    struct Record *rec = ent->rec;
    u16 *table = ent->wanderDist;
    struct Enemy *body = ent->body;
    s32 facing;
    s32 pick;
    u16 timer;

    if ((ent->sprite->unk12 & 8) == 0)
    {
        return;
    }
    enemy_place_body_quadrant(body);
    /* direction 0-7 (unk24 & 7) to a quarter: 0 up, 1 right, 2 down, 3 left */
    facing = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((u32)ent->unk24 << 29) >> 29, 2);
    switch (facing)
    {
    case 0:
        obj_set_or_add_pos(body, ent->posX, ent->posY - ent->unk276, ent->posZ + ent->unk18 + 0x800, 1, 0);
        break;
    case 1:
        obj_set_or_add_pos(body, ent->posX + 0xA00, ent->posY, ent->posZ + ent->unk18 + 0x800, 1, 0);
        break;
    case 2:
        obj_set_or_add_pos(body, ent->posX, ent->posY, ent->posZ + ent->unk18 + 0x800, 1, 0);
        break;
    case 3:
        obj_set_or_add_pos(body, ent->posX - 0xA00, ent->posY, ent->posZ + ent->unk18 + 0x800, 1, 0);
        break;
    }
    obj_set_active(body, 1);
    obj_set_sprite_link(body, 1);
    pick = rand_below(4, mt19937_next());
    body->sprite->unk20 = table[pick + 4];

    /* swing: centre where the body is, radii and angles from zero */
    body->unk2E4 = body->posX;
    body->unk2E8 = body->posY;
    body->unk2EC = body->posZ;
    body->unk2F4 = 0;
    body->unk2F0 = 0;
    body->unk30A = 0x100;
    body->unk30C = 0x180;
    body->unk32C = 0;
    body->unk328 = 0;
    body->unk30E = 0xC0;
    body->unk310 = 0x140;

    /* swing timer: the record's u16 at 0x2A for this enemy's index */
    timer = *(u16 *)((u8 *)rec + ai[0xBC] * 2 + 0x2A);
    ai[0xBD] = timer;
    obj_set_cell_coord(ent, gFrameAttrTable[((((u32)ent->unk216 << 28) >> 30) << 4) + (((u32)ent->unk24 << 29) >> 29) + 8] + 0x10, 0);
    ((struct Mode3 *)&ent->unk214)->mode = 1;
    ((struct SpriteBits12 *)&ent->sprite->unk12)->field1 = 0;
    *out = (u32)enemy_after_step_906c;
}

/* The enemy's released body homes in on the active brother on a swinging path: its swing centre
   moves towards him (higher up unless a brother is in state 4) while the body circles the
   centre on an ellipse whose radii grow and shrink. Touching a brother in state 5, or the swing
   timer running out, hands over to bump_attack_enemy_move_3d_step; with no active brother the
   enemy waits and settles. */
void enemy_after_step_906c(struct Owner *owner, struct Enemy *ent, u32 *next)
{
    u8 zero = 0;
    u8 *ai = (u8 *)ent + 0x58;
    struct Record *rec = ent->rec;
    u16 *table = ent->wanderDist;
    struct Enemy *body = ent->body;
    struct Partner *target;
    u8 targetIdx;
    u16 speed;
    s32 targetZ;
    s32 rateX;
    s32 rateY;
    s32 distX;
    s32 distY;

    /* no active brother: wait, then settle */
    if (find_active_entry(owner, &targetIdx) == 0)
    {
        ent->unk1D2 = table[ent->index + 0x14];
        next[0] = (u32)tick_store;
        next[3] = (u32)enemy_collide_settle;
        return;
    }
    target = *(struct Partner **)((u8 *)owner + targetIdx * 4 + 0x28);
    /* the body touches a brother in state 5: fast turn rates, go on with the body step */
    if (widths_overlap_x(body, target) != 0 && heights_overlap_y(body, target) != 0 && target->unk4 == 5)
    {
        body->unk30E = 0x500;
        body->unk310 = 0x500;
        ((struct Flags8 *)&body->unk20D)->bit3 = 0;
        next[0] = (u32)bump_attack_enemy_move_3d_step;
    }
    else
    {
        /* target height and speed depend on whether a brother is in state 4 */
        if (((struct Partner *)owner->unk200)->unk4 != 4 && ((struct Partner *)owner->unk204)->unk4 != 4)
        {
            targetZ = ((struct Partner *)owner->unk200)->posZ + ((struct Partner *)owner->unk200)->unk18 + 0x2900;
            speed = *(u16 *)((u8 *)((u16 *)rec + *(u16 *)(ai + 0x178)) + 0x3A);
        }
        else
        {
            targetZ = ((struct Partner *)owner->unk200)->posZ + ((struct Partner *)owner->unk200)->unk18;
            speed = *(u16 *)((u8 *)((u16 *)rec + *(u16 *)(ai + 0x178)) + 0x32);
        }
        /* the radius rates follow the speed and keep their sign */
        rateX = speed;
        if ((s16)body->unk30A < 0)
        {
            rateX = -rateX;
        }
        body->unk30A = rateX;
        rateY = speed + 0x80;
        if ((s16)body->unk30C < 0)
        {
            rateY = -rateY;
        }
        body->unk30C = rateY;

        /* the angle along the longer axis to the brother turns faster */
        distX = target->posX - body->posX;
        if (distX < 0)
        {
            distX = -distX;
        }
        distY = target->posY - body->posY;
        if (distY < 0)
        {
            distY = -distY;
        }
        if (distX >= distY)
        {
            body->unk30E = 0x140;
            body->unk310 = 0xC0;
        }
        else
        {
            body->unk30E = 0xC0;
            body->unk310 = 0x140;
        }
        start_move_to_3d(body, target->posX, target->posY, targetZ, speed, zero);

        /* move the centre along, then place the body on the ellipse around it */
        body->copyVX = body->vx;
        body->copyVY = body->vy;
        body->stepLen = (s16)body->speed << 1;
        body->unk2E4 += (s16)body->copyVX;
        body->unk2E8 += (s16)body->copyVY;
        body->unk2EC += body->stepLen;
        {
        s32 cosX;
        s32 sinY;

        cosX = gCosineTable[((s32)body->unk32C / 0x100) & 0xFF] / 0x40;
        sinY = gSineTable[((s32)body->unk328 / 0x100) & 0xFF] / 0x40;
        body->posX = body->unk2E4 + cosX * MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(body->unk2F4, 0x100);
        body->posY = body->unk2E8 + sinY * MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(body->unk2F0, 0x100);
        }
        body->unk18 += body->stepLen;
        body->unk32C = ((((s32)body->unk32C + (s16)body->unk30E) / 0x100) & 0xFF) << 8;
        body->unk328 = ((((s32)body->unk328 + (s16)body->unk310) / 0x100) & 0xFF) << 8;
        /* the radii bounce between 0 and 22 * speed */
        body->unk2F4 += (s16)body->unk30A;
        if ((s32)body->unk2F4 < 0 || (s32)body->unk2F4 > speed * 0x16)
        {
            ((struct MotionBits *)&body->unk2B5)->xMode = 1;
        }
        body->unk2F0 += (s16)body->unk30C;
        if ((s32)body->unk2F0 < 0 || (s32)body->unk2F0 > speed * 0x16)
        {
            ((struct MotionBits *)&body->unk2B5)->yMode = 1;
        }
        if (((struct MotionBits *)&body->unk2B5)->xMode != 0)
        {
            body->unk30A = -body->unk30A;
            ((struct MotionBits *)&body->unk2B5)->xMode = 0;
        }
        if (((struct MotionBits *)&body->unk2B5)->yMode != 0)
        {
            body->unk30C = -body->unk30C;
            ((struct MotionBits *)&body->unk2B5)->yMode = 0;
        }
        if ((body->unk20C & 1) == 0)
        {
            obj_set_sprite_link(body, 0);
            ((struct Flags8 *)&ent->unk20D)->bit2 = 1;
            next[0] = (u32)enemy_collide_settle;
        }

        body->vx = body->posX - body->prevX;
        body->vy = body->posY - body->prevY;
        /* when the swing timer runs out: recentre the swing, reset radii, angles and rates */
        if (--*(u16 *)(ai + 0x17A) != 0)
        {
            return;
        }
        body->unk2E4 = body->posX;
        body->unk2E8 = body->posY;
        body->unk2F4 = 0;
        body->unk2F0 = 0;
        body->unk30A = 0x100;
        body->unk30C = 0x100;
        body->unk32C = 0;
        body->unk328 = 0;
        body->unk30E = 0x500;
        body->unk310 = 0x500;
        ((struct Flags8 *)&body->unk20D)->bit3 = 0;
        next[0] = (u32)bump_attack_enemy_move_3d_step;
    }
}
