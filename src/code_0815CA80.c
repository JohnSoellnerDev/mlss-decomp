#include "global.h"

struct VtSlot
{
    s16 offset;
    u16 filler2;
    void (*fn)();
};

struct MotionClass
{
    u8 filler0[0x10];
    struct VtSlot slot10;
    u8 filler18[0x30 - 0x18];
    struct VtSlot slot30;
    struct VtSlot slot38;
};

struct MotionObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[8];
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u8 unk24;
    u8 filler25[0x30 - 0x25];
    struct MotionClass *cls30;
    struct ChildSprite *child34;
    s32 unk38;
    s32 unk3C;
};

void obj_init_from_record(void *obj, u32 rec);
void obj_create_sprite(void *obj, u32 a);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
extern u8 gClassE8a0[];
extern u8 gClassE620[];
void sprite_update_cd0(void *sprite);
void obj_set_sprite_kind(void *obj, u32 kind);
void obj_release_sprite(void *obj);
u32 view_coord(u32 a, s32 b);
void set_slot_value(u32 id, s32 value);
s16 obj_get_pos18(void *obj);

struct SprChild
{
    u16 unk0;
    u16 unk2;
};

struct SpriteGroup
{
    struct SprChild *child0;
    struct SprChild *child4;
    struct SprChild *child8;
    struct SprChild *childC;
    s32 unk10;
    s32 unk14;
    u16 unk18;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
};

struct InitObj
{
    u8 filler0[0x24];
    u8 unk24;
    u8 filler25[0x2C - 0x25];
    u8 unk2C;
    u8 unk2D;
    u8 filler2E[0x30 - 0x2E];
    struct MotionClass *cls30;
    s32 unk34;
    struct ChildSprite *child38;
    u16 unk3C;
    u16 unk3E;
    u8 unk40;
    u8 unk41;
};

struct ChildSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 4];
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 g0 : 2;
    u8 g2 : 2;
    u8 g4 : 2;
    u8 g6 : 2;
    u8 filler20[0x21 - 0x20];
    u8 unk21;
};

struct E620Obj
{
    u8 filler0[0x30];
    struct MotionClass *cls30;
    struct ChildSprite *child34;
    u16 unk38;
};

struct SpriteRef
{
    u16 x;
    u16 y;
    u8 filler4[0x1F - 4];
    u8 flags1F;
};

struct SprBits12
{
    u8 filler0[0x12];
    u8 lo1 : 1;
    u8 mode : 2;
    u8 mid2 : 2;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

extern u8 *gUnk_03001020;
extern s16 gCosineTable[];
extern const u8 gSceneLayerSetupTable[];
void *obj_get_sprite(void *obj);

struct SprBits11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

ASM_FUNC("asm/code_0815CA80/obj_cmd_follow_b.s", void obj_cmd_follow_b(void))


ASM_FUNC("asm/code_0815CA80/obj_cmd_approach_char.s", void obj_cmd_approach_char(void))

ASM_FUNC("asm/code_0815CA80/obj_cmd_seek_char.s", void obj_cmd_seek_char(void))

ASM_FUNC("asm/code_0815CA80/obj_cmd_motion.s", void obj_cmd_motion(void))

ASM_FUNC("asm/code_0815CA80/obj_cmd_move_spawn.s", void obj_cmd_move_spawn(void))

u32 obj_init_e8a0(struct InitObj *obj, u32 rec, u16 param)
{
    struct ChildSprite *child;

    obj_init_from_record(obj, rec);
    obj->cls30 = (struct MotionClass *)gClassE8a0;
    obj->unk3C = param;
    obj->unk34 = 0;
    obj->unk24 = 0;
    obj->unk40 = 0;
    obj->unk41 = 0;
    obj->child38 = 0;
    obj->unk3E = 0;
    obj_create_sprite(obj, 0);
    child = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj->child38 = (struct ChildSprite *)child;
    obj_load_record(child, 0x27, -1, 0, 0);
    obj->child38->unkE = obj->unk2C;
    obj->child38->g0 = obj->unk2D;
    obj->child38->g2 = obj->unk2D;
    obj->child38->g4 = obj->unk2D;
    obj->child38->g6 = obj->unk2D;
    return (u32)obj;
}

ASM_FUNC("asm/code_0815CA80/obj_cmd_home.s", void obj_cmd_home(void))

NONMATCH("asm/code_0815CA80/obj_init_e620.s", u32 obj_init_e620(struct E620Obj *obj, u32 rec, u16 param))
{
    struct SpriteRef *sprite;
    struct ChildSprite *child;
    u32 zero;
    s32 none;
    u32 index;
    u32 bits;
    s32 cur;

    obj_init_from_record(obj, rec);
    obj->cls30 = (struct MotionClass *)gClassE620;
    zero = 0;
    obj->unk38 = param;
    obj_create_sprite(obj, 0);
    obj->child34 = (struct ChildSprite *)create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    none = -1;
    index = *((u8 *)gUnk_03001020 + 0x1EA2);
    if (index <= 3)
    {
        obj_load_record(obj->child34, index + 0x30, none, 0, zero);
    }
    else
    {
        obj_load_record(obj->child34, 0x30, none, 0, zero);
    }
    sprite_update_cd0(obj->child34);
    obj->child34->unk12 = obj->child34->unk12 | 0x20;
    sprite = obj_get_sprite(obj);
    obj->child34->unk0 = sprite->x;
    sprite = obj_get_sprite(obj);
    obj->child34->unk2 = sprite->y;
    obj->child34->unkE = 0x13;
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = ((u32)sprite->flags1F << 30) >> 30;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & -4) | bits;
    ((u8 *)child)[0x1F] = cur;
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = sprite->flags1F & 0xC;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & -13) | bits;
    ((u8 *)child)[0x1F] = cur;
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = sprite->flags1F & 0x30;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & -0x31) | bits;
    ((u8 *)child)[0x1F] = cur;
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = (sprite->flags1F >> 6) << 6;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & 0x3F) | bits;
    ((u8 *)child)[0x1F] = cur;
    return (u32)obj;
}
END_NONMATCH

ASM_FUNC("asm/code_0815CA80/obj_cmd_follow.s", void obj_cmd_follow(void))

ASM_FUNC("asm/code_0815CA80/obj_cmd_fly.s", void obj_cmd_fly(void))

NONMATCH("asm/code_0815CA80/obj_cmd_drop.s", u32 obj_cmd_drop(struct MotionObj *obj, u32 a1, u16 gravity, u32 a3, u8 mode))
{
    struct VtSlot *entry;
    u8 *target;
    void (*fn)();
    s32 y;

    switch (mode)
    {
    case 3:
        entry = &obj->cls30->slot30;
        target = (u8 *)obj + entry->offset;
        fn = entry->fn;
        fn(target, fn, 0x30, mode);
        obj->unk18 = 0xFE0C;
        obj->unk24 = 1;
        return 0;
    case 4:
        entry = &obj->cls30->slot38;
        target = (u8 *)obj + entry->offset;
        fn = entry->fn;
        fn(target, fn, 0x38, mode);
        obj->unk24 = 2;
        return 0;
    case 5:
        return 0;
    }
    if (obj->unk24 == 1)
    {
        obj->unk14 += obj->unk1C;
        obj->unk18 += (s16)gravity + obj->unk20;
        obj->x += (s16)obj->unk14;
        y = obj->y + (s16)obj->unk18;
        obj->y = y;
        if (y > obj->unk3C)
        {
            obj->x = obj->unk38;
            obj->y = obj->unk3C;
            obj->unk18 = 0xFE0C;
        }
    }
    return 0;
}
END_NONMATCH

NONMATCH("asm/code_0815CA80/sprite_group4_init.s", struct SpriteGroup *sprite_group4_init(struct SpriteGroup *g))
{
    u16 k;

    g->unk18 = 0;
    g->child8 = 0;
    g->child4 = 0;
    g->child0 = 0;
    g->unk1A = 0;
    g->unk1B = 0;
    g->unk1C = 0;
    g->unk1D = 0;
    g->unk10 = 0xE800;
    g->unk14 = 0x1400;
    g->child0 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(g->child0, 0xA, -1, 0, 0);
    sprite_update_cd0(g->child0);
    g->child0->unk0 = 0xD8;
    k = 0x10;
    g->child0->unk2 = k;
    g->child4 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(g->child4, 0xA, -1, 0, 0);
    sprite_update_cd0(g->child4);
    g->child4->unk0 = 0xE0;
    g->child4->unk2 = k;
    g->child8 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(g->child8, 0xA, -1, 0, 0);
    sprite_update_cd0(g->child8);
    g->child8->unk0 = 0xE8;
    g->child8->unk2 = k;
    g->childC = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(g->childC, 0x26, -1, 0, 0);
    sprite_update_cd0(g->childC);
    g->childC->unk0 = 0xFFF0;
    g->childC->unk2 = 0xFFF0;
    return g;
}
END_NONMATCH

NONMATCH("asm/code_0815CA80/obj_update_sprites_e130.s", void obj_update_sprites_e130(struct SpriteGroup *obj))
{
    s32 clearMask = 0xFFFE;
    struct ChildSprite *sa;
    struct ChildSprite *sb;
    struct ChildSprite *sc;
    s32 h;
    s32 q;
    s32 state;
    s32 cnt;
    s32 base;
    s32 cos;

    h = (s16)obj->unk18;
    q = (s16)(h / 100);
    sa = (struct ChildSprite *)obj->child0;
    if (q != (s32)sa->unk21 - 0xA)
    {
        obj_load_record(sa, q + 0xA, 0, 0, 0);
    }
    h = (s16)obj->unk18;
    q = (s16)(h / 10);
    sb = (struct ChildSprite *)obj->child4;
    if (q != (s32)sb->unk21 - 0xA)
    {
        obj_load_record(sb, (s16)(q % 10) + 0xA, 0, 0, 0);
    }
    h = (s16)obj->unk18;
    q = (s16)(h % 10);
    sc = (struct ChildSprite *)obj->child8;
    if (q != (s32)sc->unk21 - 0xA)
    {
        obj_load_record(sc, q + 0xA, 0, 0, 0);
    }
    state = obj->unk1A;
    if (state == 1)
    {
        cnt = obj->unk1B - 1;
        obj->unk1B = cnt;
        cnt = (u8)cnt;
        if (cnt == 0)
        {
            obj->unk1A = cnt;
        }
        else
        {
            obj->childC->unk0 = 0xFFF0;
            obj->childC->unk2 = 0xFFF0;
            obj->unk1C = 4 - obj->unk1C;
            obj->child0->unk0 = obj->unk1C + clearMask + obj->child0->unk0;
            obj->child4->unk0 = obj->unk1C + clearMask + obj->child4->unk0;
            obj->child8->unk0 = obj->unk1C + clearMask + obj->child8->unk0;
        }
    }
    else if (state >= 2)
    {
        if (state == 2)
        {
            cnt = obj->unk1B - 1;
            obj->unk1B = cnt;
            if ((u8)cnt == 0)
            {
                obj->childC->unk0 = 0xFFF0;
                obj->childC->unk2 = 0xFFF0;
                obj->unk1A = 0;
            }
            else
            {
                obj->unk1D = obj->unk1D + 1;
                base = obj->unk10;
                cos = gCosineTable[obj->unk1D];
                obj->childC->unk0 = (base + (cos / 0x40) * 8) >> 8;
                obj->childC->unk2 = obj->unk14 >> 8;
            }
        }
    }
}
END_NONMATCH
