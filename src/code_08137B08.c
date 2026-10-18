#include "global.h"
#include "ram_routines.h"

struct Enemy;

struct Owner
{
    u8 filler0[0x2C];
    struct Enemy *anchors[4];
    u8 filler3C[0x208 - 0x3C];
    u8 unk208;
};

struct Sprite
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
};

struct Enemy
{
    u8 filler0[0x2];
    u8 heading;
    u8 filler3[0xC - 0x3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x34 - 0x25];
    s32 unk34;
    s32 unk38;
    u8 filler3C[0x54 - 0x3C];
    u8 unk54;
    u8 filler55[0xF8 - 0x55];
    u16 unkF8;
    u8 fillerFA[0x100 - 0xFA];
    u16 unk100[4];
    u16 unk108[4];
    u16 unk110[4];
    u16 unk118[4];
    u16 unk120[4];
    u16 unk128[4];
    s16 unk130[2];
    u8 filler134[0x138 - 0x134];
    s16 unk138;
    s16 unk13A;
    s16 unk13C;
    s16 unk13E;
    u8 filler140[0x198 - 0x140];
    void *unk198;
    u8 filler19C[0x1AC - 0x19C];
    u32 unk1AC;
    u8 filler1B0[0x1B8 - 0x1B0];
    s16 unk1B8;
    s16 unk1BA;
    u16 unk1BC;
    u16 unk1BE;
    s32 unk1C0;
    u16 unk1C4;
    u16 unk1C6;
    u16 unk1C8;
    u8 filler1CA[0x1CC - 0x1CA];
    s32 unk1CC;
    u16 unk1D0;
    u16 unk1D2;
    u8 filler1D4[0x1D6 - 0x1D4];
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
    u8 unk20D;
    u8 filler20E[0x216 - 0x20E];
    u8 unk216;
    u8 filler217[0x239 - 0x217];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    u16 unk242;
    u16 unk244;
    u8 filler246[0x248 - 0x246];
    s32 unk248;
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    s32 unk258;
    s32 unk25C;
    u8 filler260[0x294 - 0x260];
    struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 unk2BC;
    s32 unk2C0;
    u8 filler2C4[0x2CC - 0x2C4];
    s32 unk2CC;
    s32 unk2D0;
    u8 filler2D4[0x2D8 - 0x2D4];
    s16 unk2D8;
    s16 unk2DA;
    u8 filler2DC[0x2DE - 0x2DC];
    u16 unk2DE;
    u16 unk2E0;
    u8 filler2E2[0x33C - 0x2E2];
    u8 unk33C;
};

void obj_pick_random_entry(struct Enemy *ent);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void tick_store(void);
void move_step_handler(void);
void start_linked_state(void);
void obj_apply_state_a(void *obj);
void obj_set_sprite_link(struct Enemy *ent, u32 zero);
void obj_set_active(struct Enemy *ent, u32 zero);
void obj_set_stage(struct Enemy *ent, u32 stage);
u16 angle_of_vector16(s32 x, s32 y);
void obj_set_or_add_pos(struct Enemy *ent, s32 x, s32 y, s32 z, u32 a, u32 b);
void obj_start_move_quarter(struct Enemy *obj, u8 mode, s32 tx, s32 ty);
void obj_move_interp(struct Enemy *ent);
void obj_step_toward_target_08138FDC(struct Enemy *obj);

void obj_random_target_b(struct Owner *owner, struct Enemy *obj, u32 *out)
{
    s32 flags;

    obj_pick_random_entry(obj);
    obj->unk1D2 = obj->unk128[rand_below(4, mt19937_next())];
    flags = (obj->unk1D6 | 2) & -2;
    obj->unk1D6 = flags;
    if (obj->unk1D2 != 0)
    {
        out[0] = (u32)tick_store;
        obj->unk1AC = out[1];
    }
    else
    {
        s32 bits = -3;
        bits &= flags;
        obj->unk1D6 = bits | 1;
        obj->unkF8 &= 0xFBFF;
    }
}

ASM_FUNC("asm/code_08137B08/obj_move_to_target.s", void obj_move_to_target(void))

ASM_FUNC("asm/code_08137B08/obj_random_pose_short.s", void obj_random_pose_short(void))

ASM_FUNC("asm/code_08137B08/obj_link_sprites.s", void obj_link_sprites(void))

ASM_FUNC("asm/code_08137B08/obj_random_offset.s", void obj_random_offset(void))

ASM_FUNC("asm/code_08137B08/obj_random_offset_step.s", void obj_random_offset_step(void))

ASM_FUNC("asm/code_08137B08/obj_random_facing_target.s", void obj_random_facing_target(void))

ASM_FUNC("asm/code_08137B08/obj_pick_move_vector.s", void obj_pick_move_vector(void))

ASM_FUNC("asm/code_08137B08/obj_random_orbit_offset.s", void obj_random_orbit_offset(void))

ASM_FUNC("asm/code_08137B08/obj_start_move_to_target.s", void obj_start_move_to_target(void))

ASM_FUNC("asm/code_08137B08/obj_start_move_quarter.s", void obj_start_move_quarter(struct Enemy *obj, u8 mode, s32 tx, s32 ty))

void obj_start_hop(struct Enemy *obj, u8 mode, s32 tx, s32 ty)
{
    u8 **routines;
    s32 dy;
    s32 len;
    s32 speed;
    s32 t;

    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;

    if (mode == 1)
    {
        tx += obj->posX;
        ty += obj->posY;
    }
    obj->unk2CC = obj->posX;
    obj->unk2D0 = obj->posY;
    obj->unk2BC = tx;
    obj->unk2C0 = ty;

    routines = &gMathRoutines;
    tx -= obj->posX;
    tx = tx * (tx / 4) / 4;
    dy = ty - obj->posY;
    len = ((s32 (*)(s32))*routines)(tx + dy * (dy / 4) / 4);

    obj->unk2D8 = obj->unk23E;
    obj->unk242 = obj->unk23E;
    speed = (s16)obj->unk23E;
    t = speed + len * 8;
    obj->unk248 = -((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(-speed * speed + t / 2, t);
    if (obj->unk248 == 0)
    {
        obj->unk248 = 1;
    }
    obj->unk2DE = 0;
    obj->unkF8 |= 0x800;
}

ASM_FUNC("asm/code_08137B08/obj_start_move_zero_anchor.s", void obj_start_move_zero_anchor(void))

void obj_velocity_from_facing(struct Enemy *obj)
{
    switch (((u32)obj->unk24 << 29) >> 29)
    {
    case 0:
        obj->unk2D8 = 0;
        obj->unk2DA = -obj->unk23E;
        break;
    case 1:
        obj->unk2D8 = obj->unk23E;
        obj->unk2DA = -obj->unk23E;
        break;
    case 2:
        obj->unk2D8 = obj->unk23E;
        obj->unk2DA = 0;
        break;
    case 3:
        obj->unk2D8 = obj->unk23E;
        obj->unk2DA = obj->unk23E;
        break;
    case 4:
        obj->unk2D8 = 0;
        obj->unk2DA = obj->unk23E;
        break;
    case 5:
        obj->unk2D8 = -obj->unk23E;
        obj->unk2DA = obj->unk23E;
        break;
    case 6:
        obj->unk2D8 = -obj->unk23E;
        obj->unk2DA = 0;
        break;
    case 7:
        obj->unk2D8 = -obj->unk23E;
        obj->unk2DA = -obj->unk23E;
        break;
    }
    obj->unk242 = obj->unk2D8;
    obj->unk244 = obj->unk2DA;
    obj->unk2DE = 0;
    obj->unkF8 |= 0x800;
}

ASM_FUNC("asm/code_08137B08/obj_try_start_hop.s", void obj_try_start_hop(void))

ASM_FUNC("asm/code_08137B08/obj_try_start_jump.s", void obj_try_start_jump(void))

ASM_FUNC("asm/code_08137B08/obj_flags_2b6_update.s", void obj_flags_2b6_update(void))

void obj_step_toward_target_08138FDC(struct Enemy *obj)
{
    s32 vx;
    s32 vy;

    if ((obj->unk216 & 0x20) == 0)
    {
        obj->posX += obj->unk2D8;
        obj->posY += obj->unk2DA;
        obj->unk2DE++;
        vx = obj->unk2D8;
        if ((vx > 0 && obj->posX + 0x100 > obj->unk2CC) || (vx < 0 && obj->posX - 0x100 < obj->unk2CC) || vx == 0)
        {
            obj->posX = obj->unk2CC;
        }
        vy = obj->unk2DA;
        if ((vy > 0 && obj->posY + 0x100 > obj->unk2D0) || (vy < 0 && obj->posY - 0x100 < obj->unk2D0) || vy == 0)
        {
            obj->posY = obj->unk2D0;
        }
    }
}

void obj_move_interp(struct Enemy *obj)
{
    u32 frames;

    if ((obj->unk216 & 0x20) == 0)
    {
        frames = obj->unk2DE + 1;
        obj->unk2DE = frames;
        if ((u16)frames >= obj->unk2E0)
        {
            obj->posX = obj->unk2CC;
            obj->posY = obj->unk2D0;
        }
        else
        {
            obj->posX = (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2DE * obj->unk2D8, obj->unk2E0) << 2) + obj->unk2BC;
            obj->posY = (MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2DE * obj->unk2DA, obj->unk2E0) << 2) + obj->unk2C0;
        }
    }
}

ASM_FUNC("asm/code_08137B08/obj_approach_step.s", void obj_approach_step(void))

void obj_drift_with_cap_step(struct Enemy *obj)
{
    s32 vx;
    s32 vy;

    if ((obj->unk216 & 0x20) == 0)
    {
        obj->posX += obj->unk2BC;
        obj->posY += obj->unk2C0;
        obj->unk242 = obj->unk2BC;
        obj->unk244 = obj->unk2C0;
        obj->unk2BC += obj->unk2D8;
        obj->unk2C0 += obj->unk2DA;
        if (obj->unk2BC > 0x600)
        {
            obj->unk2BC = 0x600;
        }
        if (obj->unk2C0 > 0x600)
        {
            obj->unk2C0 = 0x600;
        }
        obj->unk2DE++;
        vx = obj->unk2BC;
        if ((vx > 0 && obj->posX + 0x100 > obj->unk2CC) || (vx < 0 && obj->posX - 0x100 < obj->unk2CC) || vx == 0)
        {
            obj->posX = obj->unk2CC;
        }
        vy = obj->unk2C0;
        if ((vy > 0 && obj->posY + 0x100 > obj->unk2D0) || (vy < 0 && obj->posY - 0x100 < obj->unk2D0) || vy == 0)
        {
            obj->posY = obj->unk2D0;
        }
    }
}

