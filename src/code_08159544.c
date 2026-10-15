#include "global.h"
#include "ram_routines.h"

struct SpriteObj
{
    u16 x;
    u16 y;
    u16 unk04;
    u16 unk06;
    u8 filler08[0x0C - 0x08];
    u16 unk0C;
    u8 filler0E[0x12 - 0x0E];
    u8 unk12;
};

struct Pos16
{
    u16 x;
    u16 y;
};

struct CamPos
{
    s32 x;
    s32 y;
};

struct ObjPair
{
    s32 first;
    s32 second;
};

struct ScrollSub
{
    s32 unk00;
    s32 unk04;
};

struct ObjClass
{
    u8 filler0[0x30];
    s16 thisOffset30;
    u16 filler32;
    void (*fn34)(void *self);
    s16 thisOffset38;
    u16 filler3A;
    void (*fn3C)(void *self);
};

struct ClassVt
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*fn0C)(void *self, u32 mode);
};

struct TransObj
{
    union
    {
        u16 half;
        s32 word;
    } unk00;
    s32 unk04;
    union
    {
        u16 half;
        s32 word;
    } unk08;
    s32 unk0C;
    s32 unk10;
    union
    {
        u16 half;
        s16 shalf;
        s32 word;
    } unk14;
    union
    {
        u16 half;
        s16 shalf;
        s32 word;
    } unk18;
    union
    {
        s16 half;
        u16 uhalf;
        s32 word;
    } unk1C;
    union
    {
        s16 half;
        u16 uhalf;
        s32 word;
    } unk20;
    union
    {
        u8 byte;
        s32 word;
        u8 bytes[4];
    } unk24;
    u8 filler28[0x30 - 0x28];
    const void *klass30;
    union
    {
        struct TransObj *child;
        struct SpriteObj *sprite;
        s32 word;
        struct
        {
            s16 kind;
            u16 unk36;
        } pair;
    } unk34;
    union
    {
        u16 half;
        s32 word;
    } unk38;
    union
    {
        u16 half;
        s32 word;
    } unk3C;
    struct Pos16 *unk40;
    u8 filler44[0x4C - 0x44];
    struct ScrollSub *unk4C;
};

extern const u8 gTransitionClassDf40[];
extern const u8 gTransitionClassDf80[];
extern const u8 gTransitionClassDfc0[];
extern const u8 gTransitionClassE000[];
extern const u8 gTransitionClassE040[];
extern const u8 gTransitionClassE080[];
extern const u8 gTransitionClassE0c0[];
extern const u8 gClassE100[];
extern const u8 gClassE140[];
extern const u8 gClassE180[];
extern const u8 gClassE1c0[];
extern const u8 gClassE200[];
extern const u8 gClassE240[];
extern const u8 gClassE300[];
extern const u8 gClassE340[];
extern const u8 gClassE400[];
extern const u8 gClassE380[];
extern const u8 gTransitionClassDea0[];
extern const u8 gTransitionClassDf00[];
extern const u8 gHeapTagObjInitBuf[];
extern u8 gGameMode;
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
s32 effect_init_sprites_b(void *p);
s32 effect_init_sprites(void *p);

void sprite_set_pos_rel_cam(struct TransObj *obj, void *cam);
void obj_reset(struct TransObj *obj, u32 flags);
void obj_init_from_record(struct TransObj *obj, u32 arg);
void obj_create_sprite(struct TransObj *obj, u32 zero);
s32 obj_flush_anim(const void *klass);
void release_obj_131c(void *p);
u32 mt19937_next(void);
void heap_free_b(void *p);
struct SpriteObj *obj_get_sprite(struct TransObj *obj);
void obj_set_sprite_kind(struct TransObj *obj, u32 kind);
void obj_update_digits(struct TransObj *obj);
u32 view_coord(u32 a, s32 b);
void obj_release_sprite(struct TransObj *obj);
void effect_init_sprite_pair(void *p);
extern const u8 gTransitionClassDee0[];
void obj_state_load_step(struct TransObj *obj);
void obj_track_char_a(struct TransObj *obj);
u32 sub_081DE850(u32 n, u32 r);
void obj_release_child_free(void *p, u32 mode);
void obj_release_child_free_c(void *p, u32 mode);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_init_from_record(struct TransObj *obj, u32 arg);

extern u8 *gGamePtr;
extern s16 *gUnk_03001014;
void win_draw_text(void *target, u32 arg, s32 dx, u32 y, u32 flag);

void set_class_e0c0(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gTransitionClassE0c0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e0c0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gTransitionClassE0c0;
    obj->unk34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

void transition_states_obj_sync_sprite_pos(struct TransObj *obj, struct CamPos *cam)
{
    struct SpriteObj *sprite;

    sprite_set_pos_rel_cam(obj, cam);
    sprite = obj->unk34.sprite;
    if (sprite != 0)
    {
        s32 camX = cam->x;

        sprite->x = (obj->unk04 - camX) >> 8;
        sprite = obj->unk34.sprite;
        {
            s32 camY = cam->y;

            sprite->y = (obj->unk08.word - camY) >> 8;
        }
    }
}

void set_class_e100(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE100;
    if (obj->unk34.child != 0)
    {
        release_obj_131c(obj->unk34.child);
        obj->unk34.child = 0;
    }
    obj_reset(obj, arg);
}

s32 obj_phase_update(struct TransObj *obj)
{
    struct TransObj *other;
    s32 count;

    s32 state = obj->unk24.byte;

    if (state == 1)
    {
        goto phase_one;
    }
    if (state <= 1)
    {
        goto ret_zero;
    }
    if (state == 2)
    {
        goto ret_one;
    }
    goto ret_zero;

phase_one:
    count = 0;
    other = *(struct TransObj **)(gGamePtr + 0x1C98);
    if (obj->unk04 - other->unk04 < 0)
    {
        count = 1;
        other->unk24.byte = 6;
    }
    other = *(struct TransObj **)(gGamePtr + 0x1C9C);
    if (obj->unk04 - other->unk04 < 0)
    {
        count += 1;
        other->unk24.byte = 6;
    }
    if (count > 1)
    {
        obj->unk24.byte = 2;
    }
    goto ret_zero;

ret_one:
    return 1;

ret_zero:
    return 0;
}

void set_class_e140(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE140;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e140_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE140;
    obj->unk34.pair.kind = kind;
    obj->unk24.byte = 1;
    return obj;
}

s32 obj_scroll_recycle(struct TransObj *obj, u32 unused1, u32 unused2, s32 *scroll)
{
    s32 step = obj->unk34.pair.kind;

    obj->unk04 -= step;
    if (obj->unk04 - *scroll < -0x2000)
    {
        obj->unk04 += ((mt19937_next() & 0x1FF) + 0x200) << 8;
    }
    return 0;
}

void transition_states_obj_setup_sprites(struct TransObj *obj)
{
    const struct ObjClass *klass;
    void *self;

    obj_create_sprite(obj, 0);
    if (obj->unk24.bytes[2] == 1)
    {
        obj_get_sprite(obj)->unk04 = 0x140;
        obj_get_sprite(obj)->unk06 = 0x140;
    }
    obj->unk08.word = 0xA000;
    klass = obj->klass30;
    self = (u8 *)obj + klass->thisOffset30;
    klass->fn34(self);
}

void set_class_e300(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE300;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e300_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE300;
    obj->unk3C.half = kind;
    obj->unk34.pair.kind = 0;
    obj->unk38.half = 0;
    obj->unk04 += ((mt19937_next() & 0x1FF) + 0x200) << 8;
    return obj;
}

s32 stub_return_zero_08159764(void)
{
    return 0;
}

void set_class_e340(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE340;
    obj_reset(obj, arg);
}

struct TransObj *obj_set_class_e340_val(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE340;
    obj->unk34.pair.kind = kind;
    return obj;
}

void set_class_e380(struct TransObj *obj, u32 arg)
{
    struct TransObj *child;
    const struct ClassVt *vt;

    obj->klass30 = gClassE380;
    child = obj->unk34.child;
    if (child != 0)
    {
        vt = child->klass30;
        vt->fn0C((u8 *)child + vt->thisOffset, 3);
    }
    obj_reset(obj, arg);
}

void obj_prepare_a(struct TransObj *obj, void *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
}

void set_class_e180(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE180;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e180_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE180;
    obj->unk34.pair.kind = kind;
    obj->unk24.byte = 0;
    return obj;
}

void transition_states_obj_prepare_b(struct TransObj *obj, void *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
}

void set_class_e1c0(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE1c0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e1c0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE1c0;
    obj->unk34.pair.kind = kind;
    obj->unk24.byte = 0;
    obj_create_sprite(obj, 0);
    return obj;
}

void obj_enter_state4(struct TransObj *obj)
{
    if (obj_get_sprite(obj) != 0)
    {
        obj_set_sprite_kind(obj, 1);
        obj_get_sprite(obj)->unk0C = 0;
    }
    obj->unk34.pair.unk36 = 0x1E;
    obj->unk24.byte = 4;
}

void obj_setup_e200(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE200;
    obj_reset(obj, arg);
}

struct TransObj *obj_create_e200(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE200;
    obj->unk34.pair.kind = kind;
    obj->unk34.pair.unk36 = 0;
    obj->unk38.half = 0;
    obj->unk24.byte = 2;
    obj_create_sprite(obj, 0);
    return obj;
}

void obj_start_fall(struct TransObj *obj, s32 speed)
{
    obj->unk18.half = 0xC8;
    obj->unk20.half = -speed;
    obj->unk38.half = 0;
    obj_get_sprite(obj)->unk0C = 0;
    obj->unk24.byte = 4;
}

void obj_enter_state2(struct TransObj *obj)
{
    if (obj_get_sprite(obj) != 0)
    {
        obj_set_sprite_kind(obj, 1);
        obj_get_sprite(obj)->unk0C = 0;
    }
    obj->unk34.pair.unk36 = 0x1E;
    obj->unk24.byte = 2;
}

void obj_setup_e240(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gClassE240;
    obj_reset(obj, arg);
}

struct TransObj *obj_create_e240_random(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gClassE240;
    obj->unk34.pair.kind = kind;
    obj->unk34.pair.unk36 = 0;
    obj->unk38.half = 0;
    obj->unk24.bytes[2] = sub_081DE850(0xA, mt19937_next()) > 5;
    obj_create_sprite(obj, 0);
    return obj;
}

void obj_init_from_pair_a(struct TransObj *obj, struct ObjPair *src)
{
    s32 second = src->second;
    s32 first = src->first;

    obj->unk04 = first;
    obj->unk08.word = second;
    obj->unk0C = first;
    obj->unk10 = second;
    obj->unk14.half = 0;
    obj->unk18.half = 0;
    obj->unk24.byte = 0;
}

void obj_update_sub_pos_e280(struct TransObj *obj, struct CamPos *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
    if (obj->unk40 != 0)
    {
        obj->unk40->x = obj_get_sprite(obj)->x;
        obj->unk40->y = obj_get_sprite(obj)->y;
    }
}
