#include "global.h"
#include "ram_routines.h"

void heap_free_b(void *p);
void release_obj_131c(void *p);
extern const u8 gVtDe08[];
extern const u8 gAppearSubClass[];
extern const u8 gAppearCallback8Class[];
extern const u8 gAppearCallback40Class[];
extern const u8 gUnknownObjInitTableA[];
extern const u8 gUnknownObjInitTableB[];
extern const u16 gBldcntModeTable[];
extern const s32 gZoneTable[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
extern u8 *gWorld;

struct ListOwner
{
    u8 filler0[0x3C];
    u32 owner;
};
extern u8 gSys[];

struct ModelCtx
{
    u8 filler0[0x28];
    u16 unk28;
    u16 unk2A;
};

extern struct ModelCtx *gModelCtx;
extern u8 *gWorkBuf;
extern u8 gUnk_02000460[];

struct Mat4
{
    s32 m[16];
    const void *cb40;
};

void mat4_mul(struct Mat4 *out, struct Mat4 *a, struct Mat4 *b);
void update_obj_list(void);
void obj_list_pass_0a8c(void);

struct ListNode
{
    struct ListNode *prev;
    struct ListNode *next;
};

struct ObjList
{
    struct ListNode *head;
    struct ListNode *tail;
    u32 count;
    const void *vtable;
};

struct VtSlot
{
    s16 offset;
    u16 filler2;
    void (*fn)(void *);
};

struct ObjVt
{
    u8 filler0[0x10];
    struct VtSlot slot10;
};

struct ListObj
{
    struct ListNode link;
    u8 filler08[0x40 - 0x08];
    const struct ObjVt *vt40;
};

struct ObjCallback8
{
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 filler6[0x8 - 0x6];
    const void *vtable;
};

struct ObjRecord
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0xC - 0x04];
    u16 unk0C;
    u16 unk0E;
    u8 filler10[0x12 - 0x10];
    u8 flags12;
};

struct ItemObj
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0x21 - 0x04];
    u8 unk21;
};

struct AppearObj
{
    u8 filler0[0x8];
    struct ObjRecord *unk08;
    u8 filler0C[0x10 - 0x0C];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x28 - 0x1C];
    void *unk28;
    void *unk2C;
    u8 filler30[0x38 - 0x30];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x4C - 0x44];
    void (*unk4C)(struct AppearObj *obj);
    u8 filler50[0x79 - 0x50];
    u8 unk79;
    u16 unk7A;
          u16 unk7C;
      s16 unk7E;
      u32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    u8 filler90[0x92 - 0x90];
    u16 unk92;
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u16 unkAC;
    u16 unkAE;
};

struct AnimSub
{
    u32 unk00;
    u32 unk04;
    struct ObjRecord *unk08;
    struct ItemObj *unk0C;
    u32 unk10;
    u32 unk14;
    void *unk18;
    u8 unk1C;
    u8 unk1D;
    u16 unk1E;
    u16 unk20;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 filler25[0x28 - 0x25];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    const void *vtable;
};

struct Pair7A
{
    u16 a;
    u16 filler2;
    u16 b;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

void init_appear_obj_base(struct AnimSub *sub);
void list_drain(struct ObjList *list);
s8 blend_fade_step(struct ObjCallback8 *obj);
s8 blend_timer_done(struct ObjCallback8 *obj);
s32 lerp_step_081359D8(s32 from, s32 to, s32 steps, s32 step);
s32 is_state_busy(void);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void release_item_8(struct AnimSub *sub);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_release_slots(void *sprite);
void obj_list_push_default(struct AppearObj *obj);
void sprite_set_action(struct AppearObj *obj, u32 action, u32 arg, u32 extra);
void start_motion(struct AppearObj *obj);
void wait_for_index(struct AppearObj *obj);
void obj_appear_sprite(struct AppearObj *obj);
void part_fly_drop(struct AppearObj *obj);
void part_fade_3(struct AppearObj *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct AppearObj *owner);
void obj_track_target(u8 *target);
void set_slot_value(u32 id, s32 value);
u32 mt19937_next(void);
void anim_update_frame(struct AnimSub *sub);
void obj_zone_update(struct AnimSub *sub);
void obj_list_remove(struct AppearObj *obj);
void sound_stop_song(u32 song);
void anim_place_record(struct AnimSub *sub, s32 off_x, s32 off_y);
void sub_08135940(struct ObjCallback8 *obj, u16 a, u16 b);

void enter_wait_index_state(struct AppearObj *obj)
{
    if (obj->unk08->flags12 & 8)
    {
        sprite_set_action(obj, 0x10, 0, 0);
        obj->unk4C = wait_for_index;
    }
}

void task_create_list_48(void (*fn)(), struct AppearObj *obj);
void part_refresh_tiles(struct AppearObj *obj);
void part_spawn_timed(struct AppearObj *obj);
void part_setup_render(struct AppearObj *obj)
{
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gUnk_02000460, gWorkBuf + 0x3CD0, 0x20);
    gModelCtx->unk28 = gModelCtx->unk2A;
    gModelCtx->unk2A |= 0xFFFF;
    task_create_list_48(part_refresh_tiles, obj);
    obj->unk4C = part_spawn_timed;
}

void part_fade_2(struct AppearObj *obj)
{
    if (obj->unk08->flags12 & 8)
    {
        if ((s16)obj->unkAC < obj->unk9C)
        {
            obj->unk18 = obj->unk18 + 0x1000;
            sprite_set_action(obj, 0xC, 0, 0);
            set_slot_value(0xD5, -1);
            obj->unk4C = part_fade_3;
        }
        else
        {
            obj->unkAC = 0;
            sprite_set_action(obj, 0xE, 0, 0);
            obj->unk4C = part_fly_drop;
        }
    }
}

void part_fade_3(struct AppearObj *obj)
{
    obj->unk08->unk0C = obj->unk08->unk0C + 0x1400;
    obj->unkA8 = obj->unkA8 + 1;
    if (obj->unkA8 >= obj->unkA4)
    {
        obj->unkAC = obj->unkAC + 1;
        obj->unk08->unk0C = 0;
        obj->unkA8 = 0;
        obj->unk18 = obj->unk18 + 0xFFFFF000;
        obj->unk4C = part_fade_2;
        sprite_set_action(obj, 0xD, 0, 0);
    }
}

void part_drop_wait(struct AppearObj *obj)
{
    obj->unk18 = obj->unk18 + obj->unk8C;
    obj->unk8C = obj->unk8C - obj->unk94;
    if (obj->unk8C <= 0)
    {
        obj->unk18 = obj->unk18 + 0x1000;
        obj->unkA4 = 0xC;
        obj->unkA8 = 0;
        sprite_set_action(obj, 0xC, 0, 0);
        set_slot_value(0xD5, -1);
        obj->unk4C = part_fade_3;
    }
}

void obj_appear_sprite(struct AppearObj *obj)
{
    if (obj->unk08->flags12 & 8)
    {
        effect_spawn_at(0x2734, obj->unk38 / 0x100, obj->unk3C / 0x100 + 2, obj->unk40 / 0x100, obj);
        obj->unk8C = 0x800;
        obj->unk94 = 0x80;
        sprite_set_action(obj, 0xB, 0, 0);
        set_slot_value(0x69, -1);
        obj->unk4C = part_drop_wait;
    }
}

void obj_init_unindexed(struct AppearObj *obj)
{
    if (obj->unk80 == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->unk4C = obj_appear_sprite;
    }
}

void obj_start_motion_when_idle(struct AppearObj *obj)
{
    s32 busy = is_state_busy();
    s32 flags;
    if (busy == 0)
    {
        obj->unk84 = obj->unkA4 << 8;
        obj->unk88 = 0x6A00;
        obj->unk8C = obj->unk40;
        flags = obj->unk79;
        flags = flags | 0x20;
        obj->unk79 = flags;
        obj->unk7C = 0x280;
        obj->unk7A = busy;
        start_motion(obj);
        sprite_set_action(obj, 9, 0, 0);
        obj->unk4C = obj_init_unindexed;
    }
}

u32 obj_randomize_timer(struct AppearObj *obj)
{
    if (obj->unk9C == 0)
    {
        obj->unk9C = (mt19937_next() & 3) + 1;
        obj->unkA0 = 0x28;
        obj->unkA4 = 0x8C;
    }
    obj->unkAC = 0;
    obj->unk4C = obj_start_motion_when_idle;
    return 1;
}

void appear_motion_obj_fall_step(struct AppearObj *obj);
void appear_motion_obj_orbit_step(struct AppearObj *obj);
void obj_orbit_step_fixed(struct AppearObj *obj);
NONMATCH("asm/code_081348C4/obj_pick_follow_mode.s", u32 obj_pick_follow_mode(struct AppearObj *obj))
{
    s32 mode;
    s32 slot;
    s32 *p84;

    if (obj->unk9C == 0)
    {
        obj->unk9C = 0x200;
        obj->unkA0 = mt19937_next() & 3;
        obj->unkA4 = mt19937_next() & 1;
    }
    obj->unkAC = 0;
    if (obj->unkA4 != 0)
    {
        obj->unk9C = -obj->unk9C;
    }
    mode = obj->unkA0;
    switch (mode)
    {
    case 0:
    case 1:
        slot = mode + 6;
        obj->unk2C = (void *)(((u32 *)(gWorld + 0x58))[slot] + 8);
        ((struct ObjFlags77 *)obj)->bit6 = 0;
        ((struct ObjFlags77 *)obj)->bit5 = 0;
        obj_list_push_default(obj);
        obj->unkAC = 8;
        obj->unkA4 = obj->unk38;
        obj->unkA8 = obj->unk3C;
        {
            u8 *q;

            q = *(u8 **)((u8 *)obj->unk2C + 0x28);
            obj->unk84 = *(s32 *)(q + 0xD8);
            obj->unk88 = *(s32 *)(q + 0xDC);
        }
        obj->unkAE = obj->unk9C;
        obj->unk4C = appear_motion_obj_fall_step;
        set_slot_value(0xDB, -1);
        break;
    case 2:
        ((struct ObjFlags77 *)obj)->bit6 = 0;
        ((struct ObjFlags77 *)obj)->bit5 = 0;
        obj_list_push_default(obj);
        p84 = &obj->unk84;
        {
            u8 *a;
            u8 *b;

            a = *(u8 **)(gWorld + 0x70);
            b = *(u8 **)(gWorld + 0x74);
            *p84 = (*(s32 *)(a + 0x40) + *(s32 *)(b + 0x40)) / 2;
            obj->unk88 = (*(s32 *)(a + 0x44) + *(s32 *)(b + 0x44)) / 2;
        }
        obj->unk8C = (obj->unk38 - obj->unk84) / 0x100;
        obj->unkAC = 8;
        obj->unk98 = 0;
        obj->unk4C = appear_motion_obj_orbit_step;
        set_slot_value(0xDB, -1);
        break;
    case 3:
        ((struct ObjFlags77 *)obj)->bit6 = 0;
        ((struct ObjFlags77 *)obj)->bit5 = 0;
        obj_list_push_default(obj);
        obj->unk84 = obj->unk38 / 0x100 - 0x54;
        obj->unkAC = 8;
        obj->unk98 = 0;
        obj->unk4C = obj_orbit_step_fixed;
        set_slot_value(0xDB, -1);
        break;
    }
    return 1;
}
END_NONMATCH

void obj_list_push_default(struct AppearObj *obj);
void appear_motion_obj_fall_step(struct AppearObj *obj)
{
    s32 div;
    s32 mul;
    s32 wait;

    div = obj->unkA4 - obj->unk84;
    mul = obj->unkA8 - obj->unk88;
    wait = obj->unkAC - 1;
    obj->unkAC = wait;
    if ((wait << 16) <= 0)
    {
        if (obj->unk98 > 0x8000)
        {
            effect_spawn_at(0x27A9, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0x27B2, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        }
        obj->unkAC = 8;
    }
    obj->unk10 -= obj->unk9C;
    obj->unk14 = mul * (obj->unk10 - obj->unkA4) / div + obj->unkA8;
    if (obj->unk10 / 0x100 < -0x20)
    {
        obj_list_remove(obj);
        obj->unk9C = -obj->unk9C;
        obj_list_push_default(obj);
    }
    else if (obj->unk10 > obj->unkA4)
    {
        obj_list_remove(obj);
        obj->unk4C = 0;
        sound_stop_song(0xDB);
    }
}

void appear_motion_obj_orbit_step(struct AppearObj *obj)
{
    s32 idx;
    s32 cos;
    s32 sin;
    s32 wait;

    idx = (obj->unk98 / 0x100) & 0xFF;
    wait = obj->unkAC - 1;
    obj->unkAC = wait;
    if ((wait << 16) <= 0)
    {
        if (obj->unk98 > 0x8000)
        {
            effect_spawn_at(0x27A9, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0x27B2, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        }
        obj->unkAC = 8;
    }
    cos = gCosineTable[idx];
    obj->unk10 = (cos / 0x40) * obj->unk8C + obj->unk84;
    sin = gSineTable[idx];
    obj->unk14 = (sin / 0x40) * 0x10 + obj->unk88;
    obj->unk98 += obj->unk9C;
    if ((u32)(obj->unk98 + 0x10000) > 0x20000)
    {
        obj_list_remove(obj);
        obj->unk4C = 0;
        sound_stop_song(0xDB);
    }
}

void obj_list_remove(struct AppearObj *obj);
void sound_stop_song(u32 song);
void obj_orbit_step_fixed(struct AppearObj *obj)
{
    s32 idx;
    s32 cos;
    s32 sin;
    s32 wait;

    idx = (obj->unk98 / 0x100) & 0xFF;
    wait = obj->unkAC - 1;
    obj->unkAC = wait;
    if ((wait << 16) <= 0)
    {
        if (obj->unk98 > 0x8000)
        {
            effect_spawn_at(0x27A9, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0x27B2, obj->unk38 / 0x100, obj->unk3C / 0x100, obj->unk40 / 0x100, obj);
        }
        obj->unkAC = 8;
    }
    cos = gCosineTable[idx];
    obj->unk10 = (cos / 0x40) * obj->unk84 + 0x5400;
    sin = gSineTable[idx];
    obj->unk14 = (sin / 0x40) * 0x20 + 0x6C00;
    obj->unk98 += obj->unk9C;
    if ((u32)(obj->unk98 + 0x10000) > 0x20000)
    {
        obj_list_remove(obj);
        obj->unk4C = 0;
        sound_stop_song(0xDB);
    }
}

s32 obj_step_toward_target_081350A0(struct AppearObj *obj)
{
    s32 index = obj->unk80;
    s32 dx = obj->unk84 - obj->unk10;
    s32 dy = obj->unk88 - obj->unk14;
    s32 dh = obj->unk8C - obj->unk18;
    s32 step = obj->unk7E;

    if (index < step)
    {
        step = index;
    }
    if (dx != 0)
    {
        obj->unk10 += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx * step, index);
    }
    if (dy != 0)
    {
        obj->unk14 += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dy * step, index);
    }
    if (dh != 0)
    {
        obj->unk18 += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dh * step, index);
    }
    obj->unk80 -= step;
    if (index != 0)
    {
        return 0;
    }
    return 1;
}

NONMATCH("asm/code_081348C4/obj_integrate_and_speed.s", void obj_integrate_and_speed(struct AppearObj *obj))
{
    s32 r;
    s32 x;
    s32 y;
    s32 z;
    s32 t;
    s32 flags;

    r = (s16)obj->unk7A << 8;
    x = obj->unk84;
    y = obj->unk88;
    z = obj->unk8C;
    if ((obj->unk79 & 0x20) == 0)
    {
        x += obj->unk10;
        y += obj->unk14;
        z += obj->unk18;
    }
    obj->unk84 = x;
    obj->unk88 = y;
    obj->unk8C = z;
    if (r == 0)
    {
        x = (x - obj->unk10) / 4;
        y = (y - obj->unk14) / 4;
        z = (z - obj->unk18) / 4;
        t = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(x * x + y * y + z * z);
        r = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(t << 6, (s16)obj->unk7C / 16);
    }
    obj->unk80 = r;
    flags = obj->unk79;
    flags &= -0x21;
    obj->unk79 = flags;
    obj->unk7C = 0;
    ((struct Pair7A *)&obj->unk7A)->a = 0;
    ((struct Pair7A *)&obj->unk7A)->b = 0x100;
}
END_NONMATCH

u32 obj_step_accum_clamp(struct AppearObj *obj)
{
    s16 accum;
    s32 height;

    obj->unk92 = obj->unk92 + obj->unk94;
    accum = obj->unk92;
    height = obj->unk40 + accum;
    obj->unk18 = height;
    if (obj->unk98 < height)
    {
        return 0;
    }
    obj->unk18 = obj->unk98;
    obj->unk94 = 0;
    return 1;
}

void sub_08135290(struct AppearObj *obj)
{
    u8 *p;

    p = obj->unk28;
    if (*(s16 *)(p + 0xEC) == -1)
    {
        if (*(s16 *)(p + 0x226) != 0)
        {
            obj_track_target(p + 0x138);
        }
    }
    else
    {
        if (*(s16 *)(p - 0x3A) != 0)
        {
            obj_track_target(p - 0x128);
        }
    }
}

struct FxRecE
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 0x4];
    u16 unkE;
};

struct FxBits1FLow
{
    u8 filler0[0x1F];
    u8 a : 2;
    u8 b : 2;
    u8 c : 2;
    u8 d : 2;
};

NONMATCH("asm/code_081348C4/item_attach_sprite_8.s", void item_attach_sprite_8(struct AnimSub *sub, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5))
{
    s32 t;
    s32 hi;
    u8 *rec;

    release_item_8(sub);
    sub->unk1C = p4;
    sub->unk08 = create_obj_de4(p1, p2, p3, -1, -1, -1, -1);
    obj_load_record(sub->unk08, p4, -1, 0, 0);
    sprite_update_cd0(sub->unk08);
    hi = p5 << 6;
    rec = (u8 *)sub->unk08;
    t = rec[0x1F];
    t = (t & 0x3F) | hi;
    p5 &= 3;
    t = (t & -0x31) | (p5 << 4);
    t = (t & -0xD) | (p5 << 2);
    t = (t & -4) | p5;
    rec[0x1F] = t;
    ((struct FxRecE *)sub->unk08)->unkE = 0;
}
END_NONMATCH

struct RecFlags11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

NONMATCH("asm/code_081348C4/anim_update_frame.s", void anim_update_frame(struct AnimSub *sub))
{
    u8 *table;
    u8 *bytes;
    s32 idx;
    u32 flag;
    u32 sub_index;
    u8 b;

    table = (u8 *)sub->unk14;
    if (table != NULL)
    {
        bytes = (u8 *)sub->unk10;
        if (bytes != NULL)
        {
            if (*((s8 *)sub + 0x1C) != *((s8 *)sub + 0x1D) || (sub->unk1E >> 8) != (sub->unk20 >> 8))
            {
                idx = *((s8 *)sub + 0x1C);
                if (idx <= 3)
                {
                    if (idx >= 0)
                    {
                        b = *(bytes + (sub->unk1E >> 8));
                        flag = b >> 7;
                        sub_index = (b & 0x7F) + table[idx];
                        obj_load_record(sub->unk08, sub_index, 0, sub->unk38, sub->unk3C);
                        ((struct RecFlags11 *)sub->unk08)->bit6 = flag;
                        if (*((s8 *)sub + 0x1C) == 0)
                        {
                            sub->unk23 = 1;
                        }
                    }
                }
                *((u8 *)sub + 0x1D) = *((u8 *)sub + 0x1C);
                sub->unk20 = sub->unk1E;
            }
        }
    }
}
END_NONMATCH

void anim_place_record(struct AnimSub *sub, s32 off_x, s32 off_y)
{
    s32 y;

    sub->unk08->unk00 = sub->unk28 / 256 - off_x;
    y = sub->unk2C + sub->unk30;
    sub->unk08->unk02 = y / 256 - off_y;
    if (sub->unk0C != NULL)
    {
        sub->unk0C->unk00 = sub->unk08->unk00;
        sub->unk0C->unk02 = sub->unk08->unk02;
    }
    sub->unk08->unk02 += sub->unk34 / 256;
    sub->unk08->unk0E = sub->unk24 - (sub->unk30 / 256 - 0x400);
}

void scene_obj_list_pass(struct ObjList *head)
{
    struct ListObj *obj;
    struct ListOwner *list;
    void **hook;
    u8 *base;
    u32 owner;

    for (obj = (struct ListObj *)head->head; obj != NULL; obj = (struct ListObj *)obj->link.next)
    {
        obj->vt40->slot10.fn((u8 *)obj + obj->vt40->slot10.offset);
    }
    update_obj_list();
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)((list = (struct ListOwner *)gUnk_0203FFB8)->owner,
        list->owner + 0xA14);
    hook = (void **)&gUnk_03000D4C;
    owner = list->owner;
    base = (u8 *)gSys;
    *(u32 *)(base + 0x880) = ((u32 (*)(u32, u32))*hook)(owner, *(u32 *)(base + 0x880));
}

u32 list_get_head_tail_count(struct ObjList *list)
{
    return list->count;
}

struct ListNode *sub_081354D0(struct ObjList *list)
{
    return list->tail;
}

struct ListNode *sub_081354D4(struct ObjList *list)
{
    return list->head;
}

u32 list_contains_node(struct ObjList *list, struct ListNode *node)
{
    struct ListNode *cur = list->head;
    while (cur != NULL)
    {
        if (cur == node)
        {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

void list_drain(struct ObjList *list)
{
    struct ListNode *node;

    while (list->head != NULL)
    {
        node = list->head;
        if (list_contains_node(list, node))
        {
            if (node->prev != NULL)
            {
                if (node->next != NULL)
                {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
                else
                {
                    node->prev->next = NULL;
                    list->tail = node->prev;
                }
            }
            else
            {
                if (node->next != NULL)
                {
                    node->next->prev = NULL;
                    list->head = node->next;
                }
                else
                {
                    list->tail = NULL;
                    list->head = NULL;
                }
            }
            list->count--;
        }
    }
}

void list_remove_node(struct ObjList *list, struct ListNode *node)
{
    if (list_contains_node(list, node))
    {
        if (node->prev != NULL)
        {
            if (node->next != NULL)
            {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            else
            {
                node->prev->next = NULL;
                list->tail = node->prev;
            }
        }
        else
        {
            if (node->next != NULL)
            {
                node->next->prev = NULL;
                list->head = node->next;
            }
            else
            {
                list->tail = NULL;
                list->head = NULL;
            }
        }
        list->count--;
    }
}

void sub_08135590(struct ObjList *list, struct ListNode *node)
{
    if (!list_contains_node(list, node))
    {
        if (list->head == NULL)
        {
            node->next = NULL;
            node->prev = NULL;
            list->tail = node;
            list->head = node;
        }
        else
        {
            list->tail->next = node;
            node->prev = list->tail;
            node->next = NULL;
            list->tail = node;
        }
        list->count++;
    }
}

void obj_list_drain_free(struct ObjList *obj, u32 flags)
{
    obj->vtable = gVtDe08;
    list_drain(obj);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void init_obj_list_vt_de08(struct ObjList *list)
{
    list->vtable = gVtDe08;
    list->tail = NULL;
    list->head = NULL;
    list->count = 0;
}

u32 obj_is_idle_08135604(struct AnimSub *sub)
{
    if (sub->unk18 == NULL)
    {
        return 1;
    }
    if (sub->unk22 == 0)
    {
        return 1;
    }
    return 0;
}

static inline void item_load_record(struct ItemObj *item, s32 record, s32 b, u32 c, u32 d)
{
    obj_load_record(item, record, b, c, d);
}

NONMATCH("asm/code_081348C4/obj_zone_update.s", void obj_zone_update(struct AnimSub *sub))
{
    s32 zone;
    s32 t;
    s32 z;

    if (sub->unk0C != NULL)
    {
        zone = 0;
        z = sub->unk34;
        if (z < gZoneTable[0])
        {
            t = gZoneTable[1];
        }
        else
        {
            zone = 1;
            if (z < gZoneTable[2])
            {
                t = gZoneTable[3];
            }
            else
            {
                zone = 2;
                if (z >= gZoneTable[4])
                {
                    zone = 3;
                }
                else
                {
                    t = gZoneTable[5];
                }
            }
        }
        if (zone != 3)
        {
            if (sub->unk0C->unk21 != t)
            {
                item_load_record(sub->unk0C, t, -1, 0, 0);
            }
            sprite_update_cd0(sub->unk0C);
        }
        if (zone > 2)
        {
            obj_release_slots(sub->unk0C);
        }
    }
}
END_NONMATCH

u32 anim_refresh(struct AnimSub *sub)
{
    anim_update_frame(sub);
    obj_zone_update(sub);
    anim_place_record(sub, 0, 0);
    return 0;
}

u32 anim_refresh_entry(struct AnimSub *sub)
{
    return anim_refresh(sub);
}

u8 obj_field_accessors_10_30(struct AnimSub *sub)
{
    return sub->unk1C;
}

void sub_081356D4(struct AnimSub *sub, u32 x, u32 y)
{
    sub->unk38 = x;
    sub->unk3C = y;
}

void sub_081356DC(struct AnimSub *sub, u8 v)
{
    sub->unk1C = v;
}

u32 sub_081356E0(struct AnimSub *sub)
{
    return sub->unk2C + sub->unk30;
}

u32 sub_081356EC(struct AnimSub *sub)
{
    return sub->unk30;
}

u32 sub_081356F0(struct AnimSub *sub)
{
    return sub->unk2C;
}

u32 sub_081356F4(struct AnimSub *sub)
{
    return sub->unk28;
}

void sub_081356F8(struct AnimSub *sub, u32 *out)
{
    out[0] = sub->unk28;
    out[1] = sub->unk2C;
    out[2] = sub->unk30;
}

void sub_08135708(struct AnimSub *sub, u32 a, u32 b, u32 c)
{
    sub->unk28 = a;
    sub->unk2C = b;
    sub->unk30 = c;
}

u16 sub_08135710(struct AnimSub *sub)
{
    return sub->unk1E;
}

u32 sub_08135714(struct AnimSub *sub)
{
    return sub->unk1E >> 8;
}

void sub_0813571C(struct AnimSub *sub, u16 v)
{
    sub->unk1E = v;
}

void sub_08135720(struct AnimSub *sub, u32 v)
{
    sub->unk1E = v << 8;
}

void sub_08135728(struct AnimSub *sub, u32 v)
{
    sub->unk10 = v;
}

void sub_0813572C(struct AnimSub *sub, u32 v)
{
    sub->unk14 = v;
}

void release_item_c(struct AnimSub *sub)
{
    if (sub->unk0C != NULL)
    {
        release_obj_131c(sub->unk0C);
        sub->unk0C = NULL;
    }
}

void release_item_8(struct AnimSub *sub)
{
    if (sub->unk08 != NULL)
    {
        release_obj_131c(sub->unk08);
        sub->unk08 = NULL;
    }
}

void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);

void item_attach_sprite_c(struct AnimSub *sub, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5)
{
    release_item_c(sub);
    sub->unk0C = create_obj_de4(p1, p2, p3, -1, -1, -1, -1);
    obj_load_record(sub->unk0C, p4, -1, 0, 0);
    sprite_update_cd0(sub->unk0C);
    ((struct FxBits1FLow *)sub->unk0C)->a = p5 & 3;
    ((struct FxRecE *)sub->unk0C)->unkE = 0xFFFF;
}

void init_appear_obj_base(struct AnimSub *sub)
{
    sub->unk00 = 0;
    sub->unk04 = 0;
    sub->unk08 = NULL;
    sub->unk0C = NULL;
    sub->unk10 = (u32)gUnknownObjInitTableA;
    sub->unk14 = (u32)gUnknownObjInitTableB;
    sub->unk18 = NULL;
    sub->unk20 = 0;
    sub->unk1E = 0;
    sub->unk1D = 0xFF;
    sub->unk1C = -1;
    sub->unk24 = 0;
    sub->unk28 = 0;
    sub->unk2C = 0;
    sub->unk30 = 0;
    sub->unk34 = 0;
    sub->unk22 = 0;
    sub->unk23 = 0;
}

void release_items_free(struct AnimSub *sub, u32 flags)
{
    sub->vtable = gAppearSubClass;
    release_item_8(sub);
    release_item_c(sub);
    if (flags & 1)
    {
        heap_free_b(sub);
    }
}

void item_attach_sprite_8(struct AnimSub *sub, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
struct AnimSub *init_appear_sub_item(struct AnimSub *sub, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5)
{
    sub->vtable = gAppearSubClass;
    init_appear_obj_base(sub);
    item_attach_sprite_8(sub, p1, p2, p3, p4, p5);
    return sub;
}

struct AnimSub *init_appear_sub_base(struct AnimSub *sub)
{
    sub->vtable = gAppearSubClass;
    init_appear_obj_base(sub);
    return sub;
}

s8 blend_fade_step(struct ObjCallback8 *obj)
{
    u16 level;

    level = 0x10;
    if ((s8)blend_timer_done(obj) != 0)
    {
        return 1;
    }
    obj->unk2 = obj->unk2 + 1;
    switch (obj->unk0)
    {
    case 0:
    case 2:
        level = lerp_step_081359D8(0x10, 0, obj->unk4, obj->unk2);
        break;
    case 1:
    case 3:
        level = lerp_step_081359D8(0, 0x10, obj->unk4, obj->unk2);
        break;
    }
    *(u16 *)0x04000050 = gBldcntModeTable[obj->unk0];
    *(u16 *)0x04000054 = level;
    return (s8)blend_timer_done(obj);
}

s8 blend_timer_done(struct ObjCallback8 *obj)
{
    if (obj->unk2 >= obj->unk4)
    {
        return 1;
    }
    return 0;
}

void sub_08135940(struct ObjCallback8 *obj, u16 a, u16 b)
{
    if (a > 3)
    {
        a = 3;
    }
    obj->unk0 = a;
    obj->unk2 = 0;
    obj->unk4 = b;
    if (b == 0)
    {
        obj->unk4 = 1;
        blend_fade_step(obj);
    }
}

void sub_0813596C(struct ObjCallback8 *obj)
{
    obj->unk0 = 0;
    obj->unk2 = 0;
    obj->unk4 = 0x10;
}

void obj_callback_8_free(struct ObjCallback8 *obj, u32 flags)
{
    obj->vtable = gAppearCallback8Class;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct ObjCallback8 *init_obj_callback_8_timer(struct ObjCallback8 *obj, u16 a, u16 b)
{
    obj->vtable = gAppearCallback8Class;
    sub_08135940(obj, a, b);
    return obj;
}

struct ObjCallback8 *init_obj_callback_8(struct ObjCallback8 *obj)
{
    obj->vtable = gAppearCallback8Class;
    sub_0813596C(obj);
    return obj;
}

s32 lerp_step_081359D8(s32 from, s32 to, s32 steps, s32 step)
{
    if (steps <= 0)
    {
        steps = 1;
    }
    if (step < 0)
    {
        step = 0;
    }
    if (step >= steps)
    {
        step = steps;
    }
    return from + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((to - from) * step, steps);
}

NONMATCH("asm/code_081348C4/interp_cubic_steps.s", s32 interp_cubic_steps(s32 a, s32 b, s32 steps, s32 step))
{
    s32 (*div)(s32, s32);
    s32 acc;
    s32 part;

    if (steps <= 0)
    {
        steps = 1;
    }
    if (step < 0)
    {
        step = 0;
    }
    if (step >= steps)
    {
        step = steps;
    }
    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    acc = div((b * 2 + b - (a * 2 + a)) * step, steps);
    acc = div(acc * step, steps);
    acc = acc + a;
    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    part = div((a - b) * (step * 2), steps);
    part = div(part * step, steps);
    part = div(part * step, steps);
    return acc + part;
}
END_NONMATCH

ASM_FUNC("asm/code_081348C4/mat4_mul.s", void mat4_mul(struct Mat4 *out, struct Mat4 *a, struct Mat4 *b))


NONMATCH("asm/code_081348C4/mat4_apply_scale.s", void mat4_apply_scale(struct Mat4 *m, s32 x, s32 y, s32 z))
{
    struct Mat4 sc;
    struct Mat4 out;
    s32 i;

    sc.cb40 = gAppearCallback40Class;
    sc.m[0] = x;
    sc.m[1] = 0;
    sc.m[2] = 0;
    sc.m[3] = 0;
    sc.m[4] = 0;
    sc.m[5] = y;
    sc.m[6] = 0;
    sc.m[7] = 0;
    sc.m[8] = 0;
    sc.m[9] = 0;
    sc.m[10] = z;
    sc.m[11] = 0;
    sc.m[12] = 0;
    sc.m[13] = 0;
    sc.m[14] = 0;
    sc.m[15] = 0x100;
    mat4_mul(&out, m, &sc);
    m->m[0] = out.m[0];
    m->m[1] = out.m[1];
    m->m[2] = out.m[2];
    m->m[3] = out.m[3];
    m->m[4] = out.m[4];
    m->m[5] = out.m[5];
    m->m[6] = out.m[6];
    m->m[7] = out.m[7];
    m->m[8] = out.m[8];
    m->m[9] = out.m[9];
    m->m[10] = out.m[10];
    m->m[11] = out.m[11];
    m->m[12] = out.m[12];
    m->m[13] = out.m[13];
    m->m[14] = out.m[14];
    m->m[15] = out.m[15];
    out.cb40 = gAppearCallback40Class;
    sc.cb40 = gAppearCallback40Class;
}
END_NONMATCH

ASM_FUNC("asm/code_081348C4/mat4_apply_rotate_zxy.s", void mat4_apply_rotate_zxy(void))

ASM_FUNC("asm/code_081348C4/mat4_apply_rotate_yxz.s", void mat4_apply_rotate_yxz(void))
