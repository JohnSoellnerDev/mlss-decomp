#include "global.h"
#include "ram_routines.h"

struct Char {
    u8 filler0[4];
    u32 unk04;
    s32 unk08;
    u8 filler0C[8];
    u16 unk14;
};

struct CharSpeedS {
    u8 filler0[0x14];
    s16 unk14;
};

struct GameState {
    u8 filler0[0x1C98];
    struct Char *charA;
    struct Char *charB;
    u8 filler1CA0[0x1CB0 - 0x1CA0];
    void *unk1CB0;
    u8 filler1CB4[0x1F1C - 0x1CB4];
    u16 unk1F1C;
    u8 filler1F1E[2];
    u8 unk1F20;
};

extern struct GameState *gGamePtr;

struct Sprite {
    u16 unk00;
    u16 unk02;
};

struct DigitObj {
    struct Sprite *unk00;
    struct Sprite *unk04;
    struct Sprite *unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
};

struct TrackObj {
    u8 filler0[4];
    u32 unk04;
    u8 filler8[0xC];
    u16 unk14;
    u8 filler16[0x22];
    s16 unk38;
};

struct SubObj {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u8 filler08[0x4];
    u16 unkC;
    u16 unkE;
    u8 filler10;
    u8 unk11;
    u8 unk12;
    u8 filler13[0xC];
    u8 unk1F;
};

struct E080Obj {
    struct SubObj *unk00;
    u8 filler04[0x2C];
    const void *unk30;
    struct SubObj *unk34;
    u16 unk38;
};

extern const u8 gTransitionClassE080[];
struct SubObj *obj_get_sprite(void *obj);

struct FlagBit5 {
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct SubFlagBits {
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct E380Obj {
    u8 filler0[0x24];
    u8 unk24;
    u8 filler25[7];
    u8 unk2C;
    u8 unk2D;
    u8 filler2E[2];
    const void *unk30;
    u32 unk34;
    struct SubObj *unk38;
    u16 unk3C;
    u16 unk3E;
    u8 unk40;
    u8 unk41;
};

extern const u8 gClassE380[];
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
void obj_init_from_record(void *obj, u32 arg);
void obj_create_sprite(void *obj, u32 zero);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);

void obj_update_digits(struct DigitObj *obj);
void set_slot_value(u32 slot, s32 value);
void sprite_update_cd0(void *sprite);
void release_obj_131c(struct Sprite *sprite);
void obj_set_sprite_kind(void *obj, u32 kind);
void obj_release_sprite(void *obj);
u32 hitboxes_overlap(void *a, void *b);
void obj_add_value(void *a, s32 b);

ASM_FUNC("asm/code_08154490/obj_behavior_e1e0.s", void obj_behavior_e1e0(void))

ASM_FUNC("asm/code_08154490/obj_behavior_e1a0.s", void obj_behavior_e1a0(void))

struct Obj;

struct E0E0Table {
    u8 filler0[0x10];
    s16 unk10;
    u8 filler12[2];
    void (*unk14)(void *, u32, u32, u32, u32);
    u8 filler18[0x18];
    s16 unk30;
    u8 filler32[2];
    void (*unk34)(void *);
    s16 unk38;
    u8 filler3A[2];
    void (*unk3C)(void *);
};

struct E0E0Char {
    u8 filler0[4];
    s32 unk04;
    s32 unk08;
    u8 filler0C[0x18];
    u8 unk24;
    u8 filler25[0xB];
    struct E0E0Table *unk30;
    struct SubObj *unk34;
    u16 unk38;
};

struct CharPos {
    u8 filler0[4];
    s32 unk04;
    u8 filler08[0xC];
    u16 unk14;
};

struct E160Obj {
    struct SubObj *unk00;
    s32 unk04;
    s32 unk08;
    u8 filler0C[8];
    s16 unk14;
    u8 filler16[2];
    u16 unk18;
    u8 filler1A[2];
    u16 unk1C;
    u8 filler1E[2];
    u16 unk20;
    u8 filler22[2];
    u8 unk24;
    u8 filler25;
    u8 unk26;
    u8 filler27[9];
    struct E0E0Table *unk30;
    u8 filler34[4];
    s32 unk38;
    s32 unk3C;
};

ASM_FUNC("asm/code_08154490/obj_behavior_e160.s", void obj_behavior_e160(void))


struct E380Obj *obj_init_e380(struct E380Obj *obj, u32 initArg, u16 kind)
{
    obj_init_from_record(obj, initArg);
    obj->unk30 = gClassE380;
    obj->unk3C = kind;
    obj->unk34 = 0;
    obj->unk24 = 0;
    obj->unk40 = 0;
    obj->unk41 = 0;
    obj->unk38 = 0;
    obj->unk3E = rand_below(0xFF, mt19937_next());
    obj_create_sprite(obj, 0);
    obj->unk38 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk38, 0x27, -1, 0, 0);
    obj->unk38->unkE = obj->unk2C;
    ((struct SubFlagBits *)&obj->unk38->unk1F)->f0 = obj->unk2D;
    ((struct SubFlagBits *)&obj->unk38->unk1F)->f2 = obj->unk2D;
    ((struct SubFlagBits *)&obj->unk38->unk1F)->f4 = obj->unk2D;
    ((struct SubFlagBits *)&obj->unk38->unk1F)->f6 = obj->unk2D;
    return obj;
}

ASM_FUNC("asm/code_08154490/obj_behavior_e360.s", void obj_behavior_e360(void))

struct ModeBits {
    s8 mode : 5;
};

extern const u8 gClassE100[];
extern u8 gGameMode;

struct E100Obj {
    struct SubObj *unk00;
    u8 filler04[0x2C];
    const void *unk30;
    struct SubObj *unk34;
    u16 unk38;
};

struct E100Obj *obj_init_e100(struct E100Obj *obj, u32 initArg, u16 kind)
{
    struct SubObj *sp;

    obj_init_from_record(obj, initArg);
    obj->unk30 = gClassE100;
    obj->unk38 = kind;
    obj_create_sprite(obj, 0);
    obj->unk34 = 0;
    if (((struct ModeBits *)&gGameMode)->mode > 5) {
        obj_set_sprite_kind(obj, 0x3B);
        return obj;
    }
    obj->unk34 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    if ((u8)(gGamePtr->unk1F20 - 1) <= 4) {
        if (gGamePtr->unk1F20 == 5)
            obj_load_record(obj->unk34, 0xA, -1, 0, 0);
        else
            obj_load_record(obj->unk34, gGamePtr->unk1F20 + 0x2F, -1, 0, 0);
    } else {
        obj_load_record(obj->unk34, 0x30, -1, 0, 0);
    }
    sprite_update_cd0(obj->unk34);
    ((struct FlagBit5 *)&obj->unk34->unk12)->bit5 = 1;

    sp = obj_get_sprite(obj);
    obj->unk34->unk00 = sp->unk00;
    sp = obj_get_sprite(obj);
    obj->unk34->unk02 = sp->unk02;
    obj->unk34->unkE = 0x13;

    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f0 = (u32)(sp->unk1F << 30) >> 30;
    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f2 = (sp->unk1F & 0xC) >> 2;
    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f4 = (sp->unk1F & 0x30) >> 4;
    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f6 = sp->unk1F >> 6;
    return obj;
}

ASM_FUNC("asm/code_08154490/obj_behavior_e0e0.s", void obj_behavior_e0e0(void))

struct SpriteMode11 {
    u8 low2 : 2;
    u8 mode : 2;
    u8 high : 4;
};

struct E080Obj *obj_init_e080(struct E080Obj *obj, u32 initArg, u16 kind)
{
    struct SubObj *sp;

    obj_init_from_record(obj, initArg);
    obj->unk30 = gTransitionClassE080;
    obj->unk38 = kind;
    obj_create_sprite(obj, 0);
    ((struct SpriteMode11 *)&obj->unk00->unk11)->mode = 1;
    obj->unk34 = create_obj_de4(4, 0x500A, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk34, 2, -1, 0, 0);
    sprite_update_cd0(obj->unk34);
    ((struct FlagBit5 *)&obj->unk34->unk12)->bit5 = 1;
    obj->unk34->unk00 = 0x78;
    obj->unk34->unk02 = 0x50;
    obj->unk34->unkE = 0x13;

    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f0 = (u32)(sp->unk1F << 30) >> 30;
    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f2 = (sp->unk1F & 0xC) >> 2;
    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f4 = (sp->unk1F & 0x30) >> 4;
    sp = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->unk34->unk1F)->f6 = sp->unk1F >> 6;
    return obj;
}

struct E060Obj {
    struct SubObj *unk00;
    s32 unk04;
    s32 unk08;
    u8 filler0C[8];
    s16 unk14;
    u8 filler16[2];
    u16 unk18;
    u8 filler1A[2];
    u16 unk1C;
    u8 filler1E[2];
    u16 unk20;
    u8 filler22[2];
    u8 unk24;
    u8 filler25[0xB];
    struct E0E0Table *unk30;
    struct SubObj *unk34;
    u16 unk38;
};

extern u8 *gUnk_03001014;

ASM_FUNC("asm/code_08154490/obj_behavior_e060.s", void obj_behavior_e060(void))

struct DfeObj {
    u8 filler0[4];
    s32 unk04;
    s32 unk08;
    u8 filler0C[8];
    u16 unk14;
    u8 filler16[2];
    u16 unk18;
    u8 filler1A[2];
    u16 unk1C;
    u8 filler1E[2];
    u16 unk20;
    u8 filler22[2];
    u8 unk24;
    u8 filler25[0xB];
    struct E0E0Table *unk30;
    u8 filler34[4];
    s32 unk38;
    s32 unk3C;
};

NONMATCH("asm/code_08154490/obj_behavior_dfe0.s", u32 obj_behavior_dfe0(struct DfeObj *obj, u32 unused1, u16 speed, u32 unused3, u8 mode))
{
    struct E0E0Table *table;
    s32 ay;

    switch (mode) {
    case 3:
        table = obj->unk30;
        table->unk34((u8 *)obj + table->unk30);
        obj->unk18 = 0xFE0C;
        obj->unk24 = 1;
        break;
    case 4:
        table = obj->unk30;
        table->unk3C((u8 *)obj + table->unk38);
        obj->unk24 = 2;
        break;
    case 5:
        break;
    default:
        if (obj->unk24 == 1) {
            obj->unk14 += obj->unk1C;
            ay = (s16)speed + obj->unk20;
            obj->unk18 = obj->unk18 + ay;
            obj->unk04 += (s16)obj->unk14;
            obj->unk08 += (s16)obj->unk18;
            if (obj->unk08 > obj->unk3C) {
                obj->unk04 = obj->unk38;
                obj->unk08 = obj->unk3C;
                obj->unk18 = 0xFE0C;
            }
        }
        break;
    }
    return 0;
}
END_NONMATCH

struct StateObj {
    void *unk00;
    s32 unk04;
    u8 filler08[0xC];
    s16 unk14;
    u8 filler16[2];
    u16 unk18;
    u8 filler1A[0xA];
    u8 unk24;
    u8 filler25[0x11];
    u16 unk36;
};

void obj_state_load_step(struct StateObj *obj)
{
    s32 speed;
    s32 t;

    switch (obj->unk24) {
    case 0:
        if (obj->unk14 == 0)
            break;
        obj_load_record(obj->unk00, 0, 0, 0, 0);
        if (obj->unk04 > 229119)
            obj->unk24 = 10;
        else
            obj->unk24 = 1;
        break;
    case 1:
        if (obj->unk04 > 229119) {
            obj->unk18 = -gGamePtr->unk1F1C;
            obj_load_record(obj->unk00, 2, 0, 0, 0);
            obj->unk36 = 0xA;
            obj->unk24 = 2;
            break;
        }
        speed = obj->unk14;
        goto check_speed;
    case 10:
        speed = obj->unk14;
check_speed:
        if (speed != 0)
            break;
        obj_load_record(obj->unk00, 1, 0, 0, 0);
        obj->unk24 = speed;
        break;
    case 2:
        t = obj->unk36;
        t -= 1;
        obj->unk36 = t;
        if ((s16)t > 0)
            break;
        obj_load_record(obj->unk00, 3, 0, 0, 0);
        obj->unk36 = 0x3C;
        obj->unk24 += 1;
        break;
    case 3:
        t = obj->unk36;
        t -= 1;
        obj->unk36 = t;
        if ((s16)t > 0)
            break;
        obj_load_record(obj->unk00, 4, 0, 0, 0);
        obj->unk36 = 0xA;
        obj->unk24 += 1;
        break;
    case 4:
        t = obj->unk36;
        t -= 1;
        obj->unk36 = t;
        if ((s16)t > 0)
            break;
        obj_load_record(obj->unk00, 0, 0, 0, 0);
        obj->unk24 = 10;
        break;
    default:
        break;
    }
}

void obj_track_char_a(struct TrackObj *obj)
{
    struct Char *charA;
    u16 speed;
    s16 s;

    charA = gGamePtr->charA;
    switch (obj->unk38)
    {
    case 0:
        if ((s16)charA->unk14 > 0)
            obj->unk38 = 1;
        break;
    case 1:
        speed = charA->unk14;
        s = speed;
        if (s == 0)
        {
            if ((s32)(charA->unk04 - obj->unk04) <= 0x3BFF)
                obj->unk38 = 2;
        }
        else if ((s16)obj->unk14 >= s)
            obj->unk14 = speed;
        else
            obj->unk14 += 2;
        break;
    case 2:
        obj->unk14 = 0;
        obj->unk38 = 0;
        break;
    }
}

NONMATCH("asm/code_08154490/obj_create_digits.s", struct DigitObj *obj_create_digits(struct DigitObj *obj))
{
    u32 rowGap;

    obj->unk1C = 0;
    obj->unk08 = 0;
    obj->unk04 = 0;
    obj->unk00 = 0;
    obj->unk20 = 0;
    obj->unk24 = 0;
    obj->unk28 = 0;
    obj->unk2C = 0;
    obj->unk14 = 0xE800;
    obj->unk18 = 0x1400;
    obj->unk30 = 0;
    obj->unk34 = 0;

    obj->unk00 = create_obj_de4(4, 0x500C, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk00, 0xA, -1, 0, 0);
    sprite_update_cd0(obj->unk00);
    obj->unk00->unk00 = 0xD8;
    rowGap = 0x10;
    obj->unk00->unk02 = rowGap;

    obj->unk04 = create_obj_de4(4, 0x500C, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk04, 0xA, -1, 0, 0);
    sprite_update_cd0(obj->unk04);
    obj->unk04->unk00 = 0xE0;
    obj->unk04->unk02 = rowGap;

    obj->unk08 = create_obj_de4(4, 0x500C, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk08, 0xA, -1, 0, 0);
    sprite_update_cd0(obj->unk08);
    obj->unk08->unk00 = 0xE8;
    obj->unk08->unk02 = rowGap;

    obj_update_digits(obj);
    return obj;
}
END_NONMATCH

void obj_shake_or_countdown(struct DigitObj *obj)
{
    if (obj->unk20 != 1) {
        if ((s32)obj->unk20 <= 1)
            return;
        if (obj->unk20 == 2)
            goto state2;
        return;
    }
    obj->unk24 -= 1;
    if ((s32)obj->unk24 <= 0) {
        obj->unk20 = 0;
    } else {
        obj->unk28 = 4 - obj->unk28;
        {
            s32 t = obj->unk00->unk00 - 2;
            obj->unk00->unk00 = obj->unk28 + t;
        }
        {
            s32 t = obj->unk04->unk00 - 2;
            obj->unk04->unk00 = obj->unk28 + t;
        }
        {
            s32 t = obj->unk08->unk00 - 2;
            obj->unk08->unk00 = obj->unk28 + t;
        }
    }
    return;
state2:
    obj->unk24 -= 1;
    if ((s32)obj->unk24 <= 0) {
        obj->unk34 = 0;
        obj->unk30 = 0;
        obj->unk20 = 0;
        obj_update_digits(obj);
    } else {
        obj->unk34 -= 1;
        if ((s32)obj->unk34 < 0) {
            obj->unk30 = 0xA - obj->unk30;
            obj->unk34 = 5;
            obj_update_digits(obj);
        }
        obj->unk2C += 1;
    }
}

struct FallObj {
    u16 unk00;
    u8 filler02[2];
    struct Sprite *unk04;
    u16 unk08;
    u8 filler0A[2];
    u32 unk0C;
    u8 filler10[4];
    u32 unk14;
    u8 filler18[4];
    u32 unk1C;
};


u32 obj_anim_fall_step(struct FallObj *obj)
{
    s32 state = (s16)obj->unk08;

    if (state != 1) {
        if (state <= 1)
            return 0;
        if (state == 2)
            goto state2;
        if (state == 3)
            goto state3;
        return 0;
    }
    obj->unk14 -= obj->unk1C;
    obj->unk0C += obj->unk14;
    obj->unk04->unk00 = (s32)obj->unk0C >> 8;
    if ((s32)obj->unk14 >= 0)
        return 0;
    obj->unk00 = 0;
    obj->unk08 += 1;
state2:
    obj->unk00 += 1;
    if ((s16)obj->unk00 > 0x3B) {
        set_slot_value(0x4B, -1);
        obj->unk00 = 0;
        obj->unk08 += 1;
        return 3;
    }
    return 0;
state3:
    obj->unk14 += obj->unk1C;
    obj->unk0C += obj->unk14;
    obj->unk04->unk00 = (s32)obj->unk0C >> 8;
    if ((s32)obj->unk0C <= 0xEFFF)
        return 0;
    release_obj_131c(obj->unk04);
    obj->unk04 = 0;
    return 2;
}

struct RecObj {
    u8 filler0[4];
    struct Sprite *unk04;
    u16 unk08;
    u8 filler0A[2];
    u32 unk0C;
    u32 unk10;
};

extern u8 gSys[];

void obj_create_sprite_record(struct RecObj *obj)
{
    obj->unk04 = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
    switch (gSys[0x8C0]) {
    case 0:
        obj_load_record(obj->unk04, 0x1C, -1, 0, 0);
        break;
    case 1:
        obj_load_record(obj->unk04, 0x1D, -1, 0, 0);
        break;
    case 2:
        obj_load_record(obj->unk04, 0x1E, -1, 0, 0);
        break;
    case 3:
        obj_load_record(obj->unk04, 0x1F, -1, 0, 0);
        break;
    case 4:
        obj_load_record(obj->unk04, 0x20, -1, 0, 0);
        break;
    default:
        obj_load_record(obj->unk04, 0x1B, -1, 0, 0);
        break;
    }
    sprite_update_cd0(obj->unk04);
    obj->unk04->unk00 = (s32)obj->unk0C >> 8;
    obj->unk04->unk02 = (s32)obj->unk10 >> 8;
    obj->unk08 = 1;
}

ASM_FUNC("asm/code_08154490/transition_setup_scene.s", void transition_setup_scene(void))
