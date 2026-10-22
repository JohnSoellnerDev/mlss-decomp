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

struct Bits2B6
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Bits1D6
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct Bits20D
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
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

void obj_random_pose_short(struct Owner *owner, struct Enemy *obj, u32 *out)
{
    s16 *posA;
    u16 *table;
    s16 *posB;
    struct Sprite *sprite;
    u32 pick;
    u16 palette;
    u16 v;

    table = obj->unk100;
    v = obj->unk1C4;
    posA = &obj->unk1B8;
    *posA = v;
    v = obj->unk1C6;
    posB = &obj->unk1BA;
    *posB = v;
    pick = rand_below(4, mt19937_next());
    sprite = obj->sprite;
    palette = obj->unk108[(u16)pick];
    obj->unk239 = palette;
    sprite->unk20 = palette;
    table += (u16)pick;
    obj->unk23E = *table;
    obj_start_move_quarter(obj, 0, *posA << 8, *posB << 8);
    obj_move_interp(obj);
    ((struct Bits1D6 *)&obj->unk1D6)->bit0 = 0;
    ((struct Bits1D6 *)&obj->unk1D6)->bit1 = 1;
    out[0] = (u32)move_step_handler;
}


static inline u32 obj_try_link_anchor(struct Enemy *anchor, struct Enemy *obj)
{
    u32 lock;

    if ((anchor->unk20C & 1) != 0)
    {
        return 0;
    }
    lock = anchor->unk33C & 6;
    if (lock != 0)
    {
        return 0;
    }
    obj_set_sprite_link(anchor, 1);
    obj_set_active(anchor, 1);
    obj_set_or_add_pos(anchor, obj->posX, obj->posY + 0x100, obj->posZ + obj->unk18 + 0x1000, 1, lock);
    return 1;
}

void obj_link_sprites(struct Owner *owner, struct Enemy *obj)
{
    u8 *bahn;

    bahn = (u8 *)obj + 0x58;
    if ((obj->sprite->flags12 & 8) != 0)
    {
        obj_apply_state_a(obj);
        obj->unk198 = start_linked_state;
        if (!obj_try_link_anchor(*(struct Enemy **)((u8 *)owner + (obj->unk54 * 4) + 0x2C), obj))
        {
            obj_try_link_anchor(*(struct Enemy **)((u8 *)owner + (obj->unk54 * 4) + 0x30), obj);
        }
        *(u16 *)(bahn + 0x17A) = 0x78;
    }
}


ASM_FUNC("asm/code_08137B08/obj_random_offset.s", void obj_random_offset(void))


ASM_FUNC("asm/code_08137B08/obj_random_offset_step.s", void obj_random_offset_step(void))


void obj_random_facing_target(struct Enemy *obj)
{
    u8 stages[2];
    u16 *table;
    u16 sel;
    u32 rnd;
    u32 facing;
    struct Sprite *sprite;
    u16 pal;

    table = obj->unk100;
    obj->unk1CC = (s32)obj->unk130[rand_below(2, mt19937_next())] << 8;
    rnd = rand_below(4, mt19937_next());
    sprite = obj->sprite;
    sel = rnd;
    pal = obj->unk108[sel];
    sprite->unk20 = pal;
    obj->unk239 = pal;
    obj->unk23E = *(u16 *)((u8 *)table + (sel << 1));
    obj->unk1D0 = sel;

    facing = ((u32)obj->unk24 << 29) >> 29;
    if ((facing & 2) != 0)
    {
        stages[0] = 0;
        stages[1] = 4;
    }
    else
    {
        stages[0] = 2;
        stages[1] = 6;
    }
    obj_set_stage(obj, stages[rand_below(2, mt19937_next())]);
    obj->unk2CC = obj->posX;
    obj->unk2D0 = obj->posY;

    switch (((u32)obj->unk24 << 29) >> 29)
    {
    case 0:
        obj->unk2D0 = (s32)*(s16 *)((u8 *)table + 0x38) << 8;
        break;
    case 2:
        obj->unk2CC = (s32)*(s16 *)((u8 *)table + 0x3E) << 8;
        break;
    case 4:
        obj->unk2D0 = (s32)*(s16 *)((u8 *)table + 0x3A) << 8;
        break;
    case 6:
        obj->unk2CC = (s32)*(s16 *)((u8 *)table + 0x3C) << 8;
        break;
    }
}


ASM_FUNC("asm/code_08137B08/obj_pick_move_vector.s", void obj_pick_move_vector(void))

ASM_FUNC("asm/code_08137B08/obj_random_orbit_offset.s", void obj_random_orbit_offset(void))


void obj_start_move_to_target(struct Enemy *obj, u8 mode, s32 tx, s32 ty)
{
    u8 *bahn;
    u8 **routines;
    u16 *flagp;
    u16 flagValue;
    s32 vx;
    s32 vy;
    s32 dx;
    s32 dy;

    bahn = (u8 *)obj + 0x58;
    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;

    if (mode == 1)
    {
        tx += obj->posX;
        ty += obj->posY;
    }
    obj->unk2BC = obj->posX;
    obj->unk2C0 = obj->posY;
    obj->unk2CC = tx;
    obj->unk2D0 = ty;

    if (tx > obj->posX)
    {
        vx = obj->unk23E;
    }
    else if (tx < obj->posX)
    {
        vx = -obj->unk23E;
    }
    else
    {
        vx = 0;
    }
    obj->unk242 = vx;
    obj->unk2D8 = vx;

    if (ty > obj->posY)
    {
        vy = obj->unk23E;
    }
    else if (ty < obj->posY)
    {
        vy = -obj->unk23E;
    }
    else
    {
        vy = 0;
    }
    obj->unk244 = vy;
    obj->unk2DA = vy;
    obj->unk2DE = 0;

    {
        u16 hx;
        u16 hy;

        hx = *(u16 *)&obj->unk2D8;
        hy = *(u16 *)&obj->unk2DA;
        if ((hx | hy) != 0)
    {
        obj->heading = (u32)((u16)angle_of_vector16(obj->unk2D8, obj->unk2DA) + 0x10) >> 8;
    }
    else
    {
        obj->heading = 0xFF;
    }
    }

    routines = &gMathRoutines;
    *(s32 *)(bahn + 0x174) = ((s32 (*)(s32))*routines)((obj->unk2CC - obj->unk2BC) * (obj->unk2CC - obj->unk2BC) + (obj->unk2D0 - obj->unk2C0) * (obj->unk2D0 - obj->unk2C0));
    flagp = (u16 *)(bahn + 0xA0);
    flagValue = *flagp | 0x800;
    *flagp = flagValue;
}


NONMATCH("asm/code_08137B08/obj_start_move_quarter.s", void obj_start_move_quarter(struct Enemy *obj, u8 mode, s32 tx, s32 ty))
{
    u8 *bahn;
    u8 **routines;
    u16 *flagp;
    s16 *vx;
    s16 *vy;
    s32 len;
    s32 dx;
    s32 dy;

    bahn = (u8 *)obj + 0x58;
    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;
    if (mode == 1)
    {
        tx += obj->posX;
        ty += obj->posY;
    }
    obj->unk2BC = obj->posX;
    obj->unk2C0 = obj->posY;
    obj->unk2CC = tx;
    obj->unk2D0 = ty;

    dx = (tx - obj->posX) / 4;
    vx = (s16 *)&obj->unk242;
    *vx = dx;
    obj->unk2D8 = dx;
    dy = (ty - obj->posY) / 4;
    vy = (s16 *)&obj->unk244;
    *vy = dy;
    obj->unk2DA = dy;

    routines = &gMathRoutines;
    len = ((s32 (*)(s32))*routines)(*vx * *vx + *vy * *vy);
    obj->unk2E0 = ((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(len, (s16)obj->unk23E) << 2;
    obj->unk1CC = len << 2;
    obj->unk2DE = 0;
    if ((*(u16 *)vx | *(u16 *)vy) != 0)
    {
        obj->heading = (u32)((u16)angle_of_vector16(*vx, *vy) + 0x10) >> 8;
    }
    else
    {
        obj->heading = 0xFF;
    }
    flagp = (u16 *)(bahn + 0xA0);
    *flagp = 0x800 | *flagp;
}
END_NONMATCH

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

void obj_start_move_zero_anchor(struct Enemy *obj, u8 mode, s32 tx, s32 ty)
{
    u8 *bahn;
    u8 **routines;
    u16 *flagp;
    u16 flagValue;
    s32 vx;
    s32 vy;
    s32 dx;
    s32 dy;

    bahn = (u8 *)obj + 0x58;
    obj->unk1C4 = obj->posX / 0x100;
    obj->unk1C6 = obj->posY / 0x100;
    obj->unk1C8 = (obj->posZ + obj->unk18) / 0x100;

    if (mode == 1)
    {
        tx += obj->posX;
        ty += obj->posY;
    }
    obj->unk2BC = 0;
    obj->unk2C0 = 0;
    obj->unk2CC = tx;
    obj->unk2D0 = ty;

    if (tx > obj->posX)
    {
        vx = obj->unk23E;
    }
    else if (tx < obj->posX)
    {
        vx = -obj->unk23E;
    }
    else
    {
        obj->unk2BC = 0;
        vx = 0;
    }
    obj->unk242 = vx;
    obj->unk2D8 = vx;

    if (ty > obj->posY)
    {
        vy = obj->unk23E;
    }
    else if (ty < obj->posY)
    {
        vy = -obj->unk23E;
    }
    else
    {
        obj->unk2C0 = 0;
        vy = 0;
    }
    obj->unk244 = vy;
    obj->unk2DA = vy;
    obj->unk2DE = 0;

    {
        u16 hx;
        u16 hy;

        hx = *(u16 *)&obj->unk2D8;
        hy = *(u16 *)&obj->unk2DA;
        if ((hx | hy) != 0)
        {
            obj->heading = (u32)((u16)angle_of_vector16(obj->unk2D8, obj->unk2DA) + 0x10) >> 8;
        }
        else
        {
            obj->heading = 0xFF;
        }
    }

    routines = &gMathRoutines;
    *(s32 *)(bahn + 0x174) = ((s32 (*)(s32))*routines)((obj->unk2CC - obj->posX) * (obj->unk2CC - obj->posX) + (obj->unk2D0 - obj->posY) * (obj->unk2D0 - obj->posY));
    flagp = (u16 *)(bahn + 0xA0);
    flagValue = *flagp | 0x800;
    *flagp = flagValue;
}

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

NONMATCH("asm/code_08137B08/obj_try_start_hop.s", void obj_try_start_hop(struct Enemy *obj))
{
    u16 hop;
    u32 mask;
    u32 value;
    u8 *flags;

    hop = obj->unk120[obj->unk1D0];
    if (obj->unk25C == -1)
    {
        if (hop != 0)
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 0;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 0;
            obj->unk1BE = hop;
            obj->unk1C0 = obj->unk1CC;
            obj->unk24C = obj->posZ + obj->unk18;
            obj->unk250 = obj->unk18;
            obj->unk254 = (s16)obj->unk23E;
            obj->unk25C = 0;
            flags = &obj->unk2B5;
            value = *flags;
            mask = 8;
        }
        else
        {
            flags = &obj->unk2B6;
            value = *flags | 0x80;
            mask = 0x40;
        }
        *flags = value | mask;
    }
}
END_NONMATCH

NONMATCH("asm/code_08137B08/obj_try_start_jump.s", void obj_try_start_jump(struct Enemy *obj))
{
    u16 a_raw;
    u16 b_raw;
    s32 a;
    s32 b;
    u8 *flags;
    u8 value;
    u8 mask;

    a_raw = obj->unk110[obj->unk1D0];
    b_raw = obj->unk118[obj->unk1D0];
    if (obj->unk25C == -1)
    {
        a = (s16)a_raw;
        b = (s16)b_raw;
        if ((a | b) != 0)
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 0;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 0;
            obj->unk24C = obj->posZ + obj->unk18;
            obj->unk250 = obj->unk18;
            obj->unk254 = obj->unk258 = a;
            obj->unk248 = b;
            obj->unk25C = 0;
            flags = &obj->unk2B5;
            value = *flags;
            mask = 8;
        }
        else
        {
            flags = &obj->unk2B6;
            value = *flags | 0x80;
            mask = 0x40;
        }
        *flags = value | mask;
    }
}
END_NONMATCH

NONMATCH("asm/code_08137B08/obj_flags_2b6_update.s", void obj_flags_2b6_update(struct Enemy *obj))
{
    s32 t;
    s32 half;
    u8 *flags;
    u32 value;
    u32 mask;

    if (obj->unk25C == -1)
    {
        if (obj->unk1CC != 0)
        {
            ((struct Bits2B6 *)&obj->unk2B6)->bit7 = 0;
            ((struct Bits2B6 *)&obj->unk2B6)->bit6 = 0;
            t = obj->posZ + obj->unk18;
            if (t < 0)
            {
                t += 0xFF;
            }
            t >>= 8;
            t -= obj->unk1BC;
            obj->unk1BE = t;
            obj->unk1C0 = obj->unk1CC * 2;
            obj->unk24C = (s16)obj->unk1BC << 8;
            obj->unk250 = 0;
            obj->unk254 = half = (s16)obj->unk23E / 2;
            obj->unk25C = obj->unk1C0 / (half * 8);
            flags = &obj->unk2B5;
            value = *flags;
            mask = 8;
        }
        else
        {
            flags = &obj->unk2B6;
            value = *flags | 0x80;
            mask = 0x40;
        }
        *flags = value | mask;
    }
}
END_NONMATCH

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

NONMATCH("asm/code_08137B08/obj_approach_step.s", void obj_approach_step(struct Enemy *obj))
{
    s32 dx;
    s32 dy;
    s32 dx4;
    s32 dy4;
    s32 len;

    if ((obj->unk216 & 0x20) == 0)
    {
        obj->unk2DE++;
        dx = obj->unk2BC - obj->unk2CC;
        if (dx < 0)
        {
            dx += 3;
        }
        dx4 = dx >> 2;
        dy = obj->unk2C0 - obj->unk2D0;
        if (dy < 0)
        {
            dy += 3;
        }
        dy4 = dy >> 2;
        if ((dx4 | dy4) != 0)
        {
            len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx4 * dx4 + dy4 * dy4);
            obj->posX += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2D8 * dx4, len);
            obj->posY += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2D8 * dy4, len);
        }
        obj->unk2D8 -= obj->unk248;
        if ((s16)obj->unk2D8 > 0)
        {
            return;
        }
        ((struct Bits20D *)&obj->unk20D)->bit3 = 0;
        if (-(s16)obj->unk2D8 < (s16)obj->unk23E)
        {
            return;
        }
        obj->posX = obj->unk2CC;
        obj->posY = obj->unk2D0;
    }
}
END_NONMATCH

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

