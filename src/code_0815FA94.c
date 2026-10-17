#include "global.h"
struct ObjCore
{
    u8 filler0[0xC];
    u16 unk0C;
    u8 filler0E[4];
    u8 unk12;
};
struct SubObj
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0xE];
    u8 unk12;
};
struct SceneObj
{
    struct ObjCore *core;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s16 unk14;
    u8 filler16[2];
    s16 unk18;
    u8 filler1A[2];
    u16 unk1C;
    u8 filler1E[6];
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 filler27;
    u8 unk28;
    u8 unk29;
    u8 filler2A[6];
    const void *unk30;
    u16 unk34;
    u16 unk36;
    u32 unk38;
    struct SceneObj *unk3C;
    struct SubObj *unk40;
    s32 unk44;
};
struct GameVars
{
    u8 filler0[0x1898];
    void *unk1898[0x100];
    struct SceneObj *unk1C98;
    u8 filler1C9C[0x1CAC - 0x1C9C];
    void *unk1CAC;
    u8 filler1CB0[4];
    void *unk1CB4;
    s32 unk1CB8;
    s32 unk1CBC;
    u8 filler1CC0[4];
    u16 unk1CC4;
    u8 filler1CC6[0x1F20 - 0x1CC6];
    u8 unk1F20;
    u8 filler1F21[7];
    s16 unk1F28;
    u16 unk1F2A;
};


struct SpriteObj
{
    u16 x;
    u16 y;
    u16 unk04;
    u16 unk06;
    u8 filler08[4];
    u16 unk0C;
    u8 filler0E[0x12 - 0x0E];
    u8 unk12;
    u8 filler13[0x21 - 0x13];
    u8 unk21;
};

struct Bits12Field
{
    u8 b0 : 1;
    u8 mode : 2;
};

struct ClassVt
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*fn0C)(void *self, u32 mode);
};

struct InitRecord
{
    s16 x;
    s16 y;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
};

struct ChildPtrs
{
    void *c00;
    void *c04;
    void *c08;
    void *c0C;
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

struct ChildRec
{
    void *unk00;
    const void *klass04;
    u8 filler08[0x18 - 0x08];
    const void *vt18;
};

struct CamPos
{
    s32 x;
    s32 y;
};

struct TransObj
{
    struct SpriteObj *sprite;
    s32 posX;
    s32 posY;
    s32 prevX;
    s32 prevY;
    s16 speedX;
    u8 filler16[2];
    s16 speedY;
    u8 unk1A;
    u8 unk1B;
          u16 unk1C;
      u8 filler1E[2];
              u16 unk20;
    u8 filler22[2];
    u8 state;
    u8 filler25;
    u8 unk26;
    u8 filler27[0x2E - 0x27];
    u8 flags2E;
    u8 filler2F;
    const void *behavior;
    union
    {
        struct TransObj *child;
        struct SpriteObj *sprite;
        struct
        {
            s16 kind;
            u16 unk36;
        } pair;
    } slot34;
    union
    {
        u16 half;
        s32 word;
        struct TransObj *child;
    } slot38;
    union
    {
        u16 half;
        s32 word;
        struct TransObj *child;
        struct SpriteObj *sprite;
    } slot3C;
    union
    {
        u16 half;
        struct TransObj *child;
        struct SpriteObj *sprite;
    } slot40;
    s32 unk44;
};

extern const u8 gClassE940[];
extern const u8 gClassE4e0[];
extern const u8 gClassE520[];
extern const u8 gClassE560[];
extern const u8 gClassE5a0[];
extern u8 *gUnk_03001020;
extern const u8 gClassE5e0[];
extern const u8 gClassE660[];
extern const u8 gTransitionChildVtableE820[];
extern const u8 gTransitionChildVtableE860[];
extern const u8 gTransitionChildVtableE6a0[];
extern const u8 gTransitionChildVtableE6e0[];
extern const u8 gTransitionChildVtableE720[];
extern const u8 gTransitionChildVtableE760[];
extern const u8 gTransitionChildVtableE8e0[];
extern const u8 gClassE620[];
extern const u8 gClassE7a0[];
extern const u8 gClassE7e0[];
extern const u8 gClassE440[];
extern const u8 gClassE480[];
extern const u8 gClassE4c0[];
extern const u8 gClassE430[];
extern const u8 gReleaseVtableE920[];
extern const u8 gClassE8a0[];
extern struct GameVars *gGamePtr;
extern struct GameVars *gGamePtr;
void obj_set_sprite_kind(struct TransObj *obj, s16 kind);

u32 mt19937_next(void);
u32 view_coord(u32 a, s32 b);
void heap_free_b(void *p);
void task_insert(void *node, u8 kind);
void obj_reset_e920(struct ChildRec *obj);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *transition_children_obj_init_scan_pool(void *obj, u8 *game);
void obj_free_ptr_fields_d(void *p);
extern const u8 gTransitionTagPhaz[];
void release_obj_131c(void *sprite);
void obj_release_slots(void *sprite);
void sprite_update_cd0(void *sprite);
void sprite_set_pos_rel_cam(struct TransObj *obj, struct CamPos *cam);
void obj_move_by_speed(struct TransObj *obj, s32 dy);
void obj_reset(struct TransObj *obj, u32 flags);
struct SpriteObj *obj_get_sprite(struct TransObj *obj);
void obj_update_speed(struct TransObj *obj);
void obj_sync_sprites_a(struct TransObj *obj, struct CamPos *cam);
void obj_release_sprite(struct TransObj *obj);
struct TransObj *obj_init_from_record(struct TransObj *obj, u32 arg);
void obj_create_sprite(struct TransObj *obj, u32 zero);

void obj_step_pos18(struct TransObj *obj, s16 value)
{
    if (obj->speedY + value < 0)
    {
        obj->speedY = 0;
    }
    else
    {
        obj->speedY = obj->speedY + value;
    }
    if (value < 0)
    {
        obj->unk1B = 100;
        obj->unk1A = 1;
    }
    else if (value > 0)
    {
        obj->unk1B = 100;
        obj->unk1A = 2;
    }
}
s16 obj_get_pos18(struct TransObj *obj)
{
    return obj->speedY;
}

void obj_store_nonneg_u16_18(struct TransObj *obj, s16 value)
{
    if (value < 0)
    {
        obj->speedY = 0;
    }
    else
    {
        obj->speedY = value;
    }
}
void obj_release_children_4(struct ChildPtrs *obj, u32 flags)
{
    if (obj->c00 != 0)
    {
        release_obj_131c(obj->c00);
    }
    if (obj->c04 != 0)
    {
        release_obj_131c(obj->c04);
    }
    if (obj->c08 != 0)
    {
        release_obj_131c(obj->c08);
    }
    release_obj_131c(obj->c0C);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

s32 obj_move_dispatch(struct TransObj *obj, u32 ctx, u16 speed, u32 unused, u8 mode)
{
    const struct ObjClass *klass;
    s32 step = speed;
    s32 limit;

    if (mode == 3)
    {
        goto call_fn34;
    }
    if (mode == 4)
    {
        goto call_fn3c;
    }
    obj->speedX = obj->speedX + obj->unk1C;
    step = (s16)speed;
    step += obj->unk20;
    obj->speedY = obj->speedY + step;
    obj->posX += obj->speedX;
    obj->posY += obj->speedY;
    limit = (s32)(view_coord(ctx, obj->posX) << 16) >> 8;
    if (obj->posY > limit)
    {
        obj->posY = (s32)(view_coord(ctx, obj->posX) << 16) >> 8;
    }
    goto done;

call_fn34:
    klass = obj->behavior;
    klass->fn34((u8 *)obj + klass->thisOffset30);
    goto done;

call_fn3c:
    klass = obj->behavior;
    klass->fn3C((u8 *)obj + klass->thisOffset38);

done:
    return 0;
}

void set_class_e4e0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE4e0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e4e0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gClassE4e0;
    obj->slot34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

void stub_nop_0815fbf4(void)
{
}

s32 transition_children_obj_dispatch_by_mode(struct TransObj *obj, u32 unused1, u32 unused2, u32 unused3, u8 mode)
{
    const struct ObjClass *klass;
    void *self;

    switch (mode)
    {
    case 3:
        klass = obj->behavior;
        self = (u8 *)obj + klass->thisOffset30;
        klass->fn34(self);
        break;
    case 4:
        klass = obj->behavior;
        self = (u8 *)obj + klass->thisOffset38;
        klass->fn3C(self);
        obj->state = 1;
        break;
    }
    return 0;
}

void set_class_e560(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE560;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e560_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gClassE560;
    obj->slot34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

void set_class_e520(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE520;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e520_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    s32 x;
    s32 y;

    obj_init_from_record(obj, arg);
    obj->behavior = gClassE520;
    obj->slot34.pair.kind = kind;
    x = obj->posX;
    y = obj->posY;
    obj->slot38.word = x;
    obj->slot3C.word = y;
    obj_create_sprite(obj, 0);
    return obj;
}

void set_class_e5a0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE5a0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e5a0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gClassE5a0;
    obj->slot34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

s32 obj_set_state_by_mode(struct TransObj *obj, u32 ctx, u16 speed, u32 unused, u8 mode)
{
    const struct ObjClass *klass;
    struct SpriteObj *sprite;

    switch (mode)
    {
    case 3:
        klass = obj->behavior;
        klass->fn34((u8 *)obj + klass->thisOffset30);
        obj->state = 1;
        break;
    case 4:
        obj_release_sprite(obj);
        break;
    case 0xc:
        obj->state = 2;
        break;
    default:
        switch (obj->state)
        {
        case 0:
            break;
        case 1:
            obj->posY = (s32)(view_coord(ctx, obj->posX) << 16) >> 8;
            break;
        case 2:
            obj_set_sprite_kind(obj, 0x3c);
            sprite = obj_get_sprite(obj);
            ((struct Bits12Field *)&sprite->unk12)->mode = 1;
            break;
        }
    }
    return 0;
}

void set_class_e5e0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE5e0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e5e0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gClassE5e0;
    obj->slot34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

void transition_children_obj_sync_sprite_pos(struct TransObj *obj, struct CamPos *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
    if (obj->slot34.sprite != 0)
    {
        obj->slot34.sprite->x = (-cam->x + obj->posX) >> 8;
        obj->slot34.sprite->y = (-cam->y + obj->posY) >> 8;
    }
}

void set_class_e620(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE620;
    if (obj->slot34.child != 0)
    {
        release_obj_131c(obj->slot34.child);
        obj->slot34.child = 0;
    }
    obj_reset(obj, arg);
}

extern u8 *gUnk_03001020;
s32 obj_check_chars(struct TransObj *obj)
{
    struct TransObj *other;
    s32 count;

    s32 state = obj->state;

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
    other = *(struct TransObj **)(gUnk_03001020 + 0x1C98);
    if (obj->posX - other->posX < 0)
    {
        count = 1;
        other->state = 6;
    }
    other = *(struct TransObj **)(gUnk_03001020 + 0x1C9C);
    if (obj->posX - other->posX < 0)
    {
        count += 1;
        other->state = 6;
    }
    if (count > 1)
    {
        obj->state = 2;
    }
    goto ret_zero;

ret_one:
    return 1;

ret_zero:
    return 0;
}

void set_class_e660(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE660;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e660_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gClassE660;
    obj->slot34.pair.kind = kind;
    obj->state = 1;
    return obj;
}

u32 obj_drift_x(struct TransObj *obj, u32 unused1, u32 unused2, s32 *limitRef)
{
    obj->posX -= obj->slot34.pair.kind;
    if (obj->posX - *limitRef < -0x2000)
    {
        obj->posX += ((mt19937_next() & 0x1FF) + 0x200) << 8;
    }
    return 0;
}

void obj_finish_sprites(struct TransObj *obj)
{
    const struct ObjClass *cls;

    obj_create_sprite(obj, 0);
    if (obj->unk26 == 1)
    {
        obj_get_sprite(obj)->unk04 = 0x140;
        obj_get_sprite(obj)->unk06 = 0x140;
    }
    obj->posY = 0xA000;
    cls = obj->behavior;
    cls->fn34((u8 *)obj + cls->thisOffset30);
}

void set_class_e820(struct TransObj *obj, u32 arg)
{
    obj->behavior = gTransitionChildVtableE820;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e820_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE820;
    obj->slot3C.half = kind;
    obj->slot34.pair.kind = 0;
    obj->slot38.half = 0;
    obj->posX += ((mt19937_next() & 0x1FF) + 0x200) << 8;
    return obj;
}

s32 stub_return_zero_0815ffac(void)
{
    return 0;
}

void set_class_e860(struct TransObj *obj, u32 arg)
{
    obj->behavior = gTransitionChildVtableE860;
    obj_reset(obj, arg);
}

struct TransObj *obj_set_class_e860_val(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE860;
    obj->slot34.pair.kind = kind;
    return obj;
}

void set_class_e8a0(struct TransObj *obj, u32 arg)
{
    struct TransObj *child;

    obj->behavior = gClassE8a0;
    child = obj->slot34.child;
    if (child != 0)
    {
        const struct ClassVt *vt = child->behavior;

        vt->fn0C((u8 *)child + vt->thisOffset, 3);
    }
    obj_reset(obj, arg);
}

void obj_prepare(struct TransObj *obj, struct CamPos *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
}

void set_class_e6a0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gTransitionChildVtableE6a0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e6a0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE6a0;
    obj->slot34.pair.kind = kind;
    obj->state = 0;
    return obj;
}

void obj_prepare_2(struct TransObj *obj, struct CamPos *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
}

void set_class_e6e0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gTransitionChildVtableE6e0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e6e0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE6e0;
    obj->slot34.pair.kind = kind;
    obj->state = 0;
    obj_create_sprite(obj, 0);
    if (obj->unk26 == 0)
    {
        obj->sprite->unk04 = 0x120;
        obj->sprite->unk06 = 0x120;
    }
    return obj;
}

void set_class_e720(struct TransObj *obj, u32 arg)
{
    obj->behavior = gTransitionChildVtableE720;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e720_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE720;
    obj->slot34.pair.kind = kind;
    obj->slot34.pair.unk36 = 0;
    obj->slot38.half = 0;
    obj->state = 2;
    obj_create_sprite(obj, 0);
    return obj;
}

void obj_init_state3(struct TransObj *obj, s32 arg)
{
    obj->speedY = 0x64;
    obj->unk20 = -arg;
    obj->slot38.half = 0;
    obj_get_sprite(obj)->unk0C = 0;
    obj->state = 3;
}

ASM_FUNC("asm/code_0815FA94/obj_set_dir_to_char_a.s", void obj_set_dir_to_char_a(void))

void set_class_e760(struct TransObj *obj, u32 arg)
{
    obj->behavior = gTransitionChildVtableE760;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e760_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE760;
    obj->slot34.pair.kind = kind;
    obj->slot34.pair.unk36 = 0;
    obj->slot38.half = 0;
    obj_create_sprite(obj, 0);
    return obj;
}

void obj_sync_sprites_a(struct TransObj *obj, struct CamPos *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
    if (obj->slot3C.sprite != 0)
    {
        obj->slot3C.sprite->x = (-cam->x + obj->posX) >> 8;
        obj->slot3C.sprite->y = ((-cam->y + obj->slot38.word) >> 8) + 8;
    }
    if (obj->slot40.sprite != 0)
    {
        obj->slot40.sprite->x = obj_get_sprite(obj)->x;
        obj->slot40.sprite->y = obj_get_sprite(obj)->y;
    }
}

void obj_finish_sprites_c0(struct TransObj *obj)
{
    obj_create_sprite(obj, 0);
    obj_get_sprite(obj)->unk04 = 0xC0;
    obj_get_sprite(obj)->unk06 = 0xC0;
}

void set_class_e7a0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE7a0;
    if (obj->slot40.child != 0)
    {
        release_obj_131c(obj->slot40.child);
        obj->slot40.child = 0;
    }
    if (obj->slot3C.child != 0)
    {
        release_obj_131c(obj->slot3C.child);
        obj->slot3C.child = 0;
    }
    obj_reset(obj, arg);
}

void obj_sync_sprites_b(struct TransObj *obj, struct CamPos *cam)
{
    sprite_set_pos_rel_cam(obj, cam);
    if (obj->slot40.sprite != 0)
    {
        obj->slot40.sprite->x = obj_get_sprite(obj)->x;
        obj->slot40.sprite->y = obj_get_sprite(obj)->y;
    }
    if (obj->slot3C.sprite != 0)
    {
        obj->slot3C.sprite->x = (-cam->x + obj->posX) >> 8;
        obj->slot3C.sprite->y = ((-cam->y + obj->slot38.word) >> 8) + 8;
    }
}

void set_class_e7e0(struct TransObj *obj, u32 arg)
{
    obj->behavior = gClassE7e0;
    if (obj->slot40.child != 0)
    {
        release_obj_131c(obj->slot40.child);
        obj->slot40.child = 0;
    }
    if (obj->slot3C.child != 0)
    {
        release_obj_131c(obj->slot3C.child);
        obj->slot3C.child = 0;
    }
    obj_reset(obj, arg);
}

void clear_child_u16_0c(struct TransObj *obj)
{
    obj->sprite->unk0C = 0;
}

ASM_FUNC("asm/code_0815FA94/sub_081602FC.s", void sub_081602FC(void))

void set_pos_from_view_coord(struct TransObj *obj, u32 view)
{
    s32 pos = (s16)view_coord(view, obj->posX) << 8;

    obj->unk44 = pos;
    obj->posY = pos;
    obj->state = 9;
}

void obj_update_y_limit(struct TransObj *obj, u32 view)
{
    s32 y = (s16)view_coord(view, obj->posX) << 8;

    obj->unk44 = y;
    if (obj->posY > y)
    {
        s32 x;

        obj->posY = y;
        x = obj->posX;
        obj->speedX = x - obj->prevX;
        obj->speedY = y - obj->prevY;
        obj->prevX = x;
        obj->prevY = y;
    }
    else
    {
        obj_update_speed(obj);
    }
}

void obj_apply_092c(struct TransObj *obj, u32 unused, s32 dy)
{
    obj_move_by_speed(obj, dy);
}

void clamp_max_reset_timer(struct TransObj *obj, s32 max)
{
    if (obj->posY > max)
    {
        obj->posY = max;
        obj->speedY = 0;
    }
}

void set_class_e8e0(struct TransObj *obj, u32 arg)
{
    struct TransObj *child;

    obj->behavior = gTransitionChildVtableE8e0;
    if (obj->slot3C.child != 0)
    {
        obj_release_sprite(obj->slot3C.child);
        child = obj->slot3C.child;
        if (child != 0)
        {
            const struct ClassVt *vt = child->behavior;

            vt->fn0C((u8 *)child + vt->thisOffset, 3);
        }
    }
    obj_reset(obj, arg);
}

struct TransObj *set_class_e8e0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->behavior = gTransitionChildVtableE8e0;
    obj->state = 0;
    obj->slot34.pair.kind = 0;
    obj->slot34.pair.unk36 = kind;
    obj->slot40.half = 0;
    obj->slot3C.word = 0;
    obj->unk44 = 0;
    obj_create_sprite(obj, 0);
    return obj;
}

void transition_child_init(u8 *game)
{
    void *table = heap_alloc2(0x1C, 1, gTransitionTagPhaz, 0);

    *(void **)(game + 0x1E94) = transition_children_obj_init_scan_pool(table, game);
    obj_free_ptr_fields_d(*(void **)(game + 0x1E8C));
}

void store_at_1e94(u8 *game, void *child)
{
    *(void **)(game + 0x1E94) = child;
}

struct SceneObj *pool_find_kind1_2(struct GameVars *game, s32 kind)
{
    s32 i = 0;
    void **p = game->unk1898;
    struct SceneObj *obj;

    do
    {
        obj = p[0];
        if (obj != 0 && obj->unk25 == kind) return obj;
        obj = p[1];
        if (obj != 0 && obj->unk25 == kind) return obj;
        obj = p[2];
        if (obj != 0 && obj->unk25 == kind) return obj;
        obj = p[3];
        if (obj != 0 && obj->unk25 == kind) return obj;
        p += 4;
        i += 4;
    } while (i <= 0xFF);
    return 0;
}

void *obj_spawn_by_kind(void *game);
NONMATCH("asm/code_0815FA94/sub_081604EC.s", void *sub_081604EC(struct GameVars *game))
{
    void **p;
    s32 i;
    s32 slot;

    for (i = 0, p = game->unk1898; i <= 0xFF; i += 8)
    {
        if (p[0] == 0)
            goto store;
        slot = i + 1;
        if (p[1] == 0)
            goto found;
        slot = i + 2;
        if (p[2] == 0)
            goto found;
        slot = i + 3;
        if (p[3] == 0)
            goto found;
        slot = i + 4;
        if (p[4] == 0)
            goto found;
        slot = i + 5;
        if (p[5] == 0)
            goto found;
        slot = i + 6;
        if (p[6] == 0)
            goto found;
        slot = i + 7;
        if (p[7] == 0)
        {
found:
            i = slot;
            goto store;
        }
        p += 8;
    }
    slot = -1;
    i = slot;
store:
    return game->unk1898[i] = obj_spawn_by_kind(game);
}
END_NONMATCH

NONMATCH("asm/code_0815FA94/pool_first_free_slot_3.s", s32 pool_first_free_slot_3(struct GameVars *game))
{
    struct SceneObj **p;
    s32 slot;
    struct SceneObj *obj;
    s32 i;

    i = 0;
    p = (struct SceneObj **)game->unk1898;
    do
    {
        obj = p[0];
        if (obj == 0)
            return i;
        slot = i + 1;
        obj = p[1];
        if (obj == 0)
            goto found;
        slot = i + 2;
        obj = p[2];
        if (obj == 0)
            goto found;
        slot = i + 3;
        obj = p[3];
        if (obj == 0)
            goto found;
        slot = i + 4;
        obj = p[4];
        if (obj == 0)
            goto found;
        slot = i + 5;
        obj = p[5];
        if (obj == 0)
            goto found;
        slot = i + 6;
        obj = p[6];
        if (obj == 0)
            goto found;
        slot = i + 7;
        obj = p[7];
        if (obj != 0)
            goto next;
    found:
        return slot;
    next:
        p += 8;
        i += 8;
    } while (i <= 0xFF);
    return -1;
}
END_NONMATCH

extern void *gUnk_0300101C;
void transition_exit(void *obj, u32 task);
void game_dma3_scroll(struct GameVars *gv)
{
    vu32 *dma;
    u8 *base;
    u8 *rec;
    u32 ret;

    dma = (vu32 *)0x040000D4;
    dma[0] = gv->unk1CBC;
    dma[1] = (u32)gUnk_0300101C + 4;
    dma[2] = 0x84000140;
    (void)dma[2];
    base = *(u8 **)((u8 *)gv + 0x1E94);
    rec = *(u8 **)(base + 4);
    ret = ((u32 (*)(void *, void *))*(void **)(rec + 0x14))(base + *(s16 *)(rec + 0x10), gv);
    if (ret == 0 && gv != NULL)
    {
        transition_exit(gv, 3);
    }
}

struct ChildRec *init_vt_08cde430(struct ChildRec *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->vt18 = gClassE430;
    return obj;
}

void stub_nop_08160640(void)
{
}

void obj_class_e920_free(struct ChildRec *obj, u32 flags)
{
    obj->klass04 = gReleaseVtableE920;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct ChildRec *init_child_e440(struct ChildRec *obj)
{
    obj_reset_e920(obj);
    obj->klass04 = gClassE440;
    return obj;
}

void stub_nop_0816067c(void)
{
}

void obj_class_e920_free_2(struct ChildRec *obj, u32 flags)
{
    obj->klass04 = gReleaseVtableE920;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void stub_nop_081606a0(void)
{
}

void obj_class_e920_free_3(struct ChildRec *obj, u32 flags)
{
    obj->klass04 = gReleaseVtableE920;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct ChildRec *init_child_e480(struct ChildRec *obj)
{
    obj_reset_e920(obj);
    obj->klass04 = gClassE480;
    return obj;
}

void stub_nop_081606dc(void)
{
}

void obj_class_e920_free_4(struct ChildRec *obj, u32 flags)
{
    obj->klass04 = gReleaseVtableE920;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void stub_nop_08160700(void)
{
}

void obj_class_e920_free_5(struct ChildRec *obj, u32 flags)
{
    obj->klass04 = gReleaseVtableE920;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct ChildRec *init_child_e4c0(struct ChildRec *obj)
{
    obj_reset_e920(obj);
    obj->klass04 = gClassE4c0;
    return obj;
}

struct TransObj *obj_init_from_record(struct TransObj *obj, u32 arg)
{
    struct InitRecord *rec = (struct InitRecord *)arg;

    obj->behavior = gClassE940;
    obj->posX = obj->prevX = rec->x << 8;
    obj->posY = obj->prevY = rec->y << 8;
    obj->speedX = 0;
    obj->speedY = 0;
    obj->unk1C = 0;
    obj->unk20 = 0;
    obj->state = 0;
    obj->filler25 = rec->unkC;
    obj->unk26 = rec->unk6;
    obj->filler27[0] = rec->unk7;
    obj->filler27[1] = rec->unk4;
    obj->filler27[2] = rec->unk5;
    obj->filler27[3] = rec->unk8;
    obj->filler27[4] = rec->unk9;
    obj->filler27[5] = rec->unkA;
    obj->filler27[6] = rec->unkB;
    obj->sprite = NULL;
    obj->flags2E = 0;
    return obj;
}

ASM_FUNC("asm/code_0815FA94/obj_create_sprite.s", void obj_create_sprite(struct TransObj *obj, u32 zero))

struct SpriteObj *obj_get_sprite(struct TransObj *obj)
{
    return obj->sprite;
}

void obj_free_sprite_slots(struct TransObj *obj)
{
    if (obj->sprite != 0)
    {
        obj_release_slots(obj->sprite);
    }
}

void obj_update_sprite(struct TransObj *obj)
{
    if (obj->sprite != 0)
    {
        sprite_update_cd0(obj->sprite);
    }
}

void stub_nop_081608a8(void)
{
}

void obj_set_class_e940(struct TransObj *obj)
{
    obj->behavior = gClassE940;
}

void obj_set_sprite_kind(struct TransObj *obj, s16 kind)
{
    struct SpriteObj *sprite = obj->sprite;

    if (sprite != 0)
    {
        if (sprite->unk21 != kind)
        {
            obj_load_record(sprite, kind, 0, 0, 0);
        }
    }
}

void sprite_set_pos_rel_cam(struct TransObj *obj, struct CamPos *cam)
{
    if (obj->sprite != 0)
    {
        obj->sprite->x = (-cam->x + obj->posX) >> 8;
        obj->sprite->y = (-cam->y + obj->posY) >> 8;
    }
}

void obj_update_speed(struct TransObj *obj)
{
    s32 x = obj->posX;
    s32 y;

    obj->speedX = x - obj->prevX;
    y = obj->posY;
    obj->speedY = y - obj->prevY;
    obj->prevX = x;
    obj->prevY = y;
}

void obj_move_by_speed(struct TransObj *obj, s32 dy)
{
    obj->posX += obj->speedX;
    obj->posY = obj->posY + obj->speedY + dy;
}

void obj_release_sprite(struct TransObj *obj)
{
    if (obj->sprite != 0)
    {
        release_obj_131c(obj->sprite);
        obj->sprite = 0;
    }
}

void obj_reset(struct TransObj *obj, u32 flags)
{
    obj->behavior = gClassE940;
    obj_release_sprite(obj);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_bit2e_ops(struct TransObj *obj)
{
    obj->flags2E &= 0xFD;
}

void sub_08160990(struct TransObj *obj)
{
    obj->flags2E |= 2;
}

u8 sub_0816099C(struct TransObj *obj)
{
    return obj->flags2E & 2;
}

void sub_081609AC(struct TransObj *obj)
{
    obj->flags2E &= 0xFE;
}

void sub_081609B8(struct TransObj *obj)
{
    obj->flags2E |= 1;
}

u32 sub_081609C4(struct TransObj *obj)
{
    return obj->flags2E & 1;
}

ASM_FUNC("asm/code_0815FA94/obj_setup_tiles.s", void obj_setup_tiles(void))

ASM_FUNC("asm/code_0815FA94/obj_fill_tilemap32.s", void obj_fill_tilemap32(void))

ASM_FUNC("asm/code_0815FA94/obj_scroll_tile_ring.s", void obj_scroll_tile_ring(void))
