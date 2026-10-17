#include "global.h"
#include "ram_routines.h"

struct Enemy;

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

struct Vec3
{
    s32 x;
    s32 y;
    s32 z;
    u8 filler0C[4];
    const void *cb10;
};

struct Mat4
{
    s32 m[16];
    const void *cb40;
};

struct TaskObj
{
    u8 filler0[0x14];
    void *owner14;
    const void *cb18;
};

struct T3dF;

struct T3dNode
{
    struct T3dNode *prev0;
    struct T3dNode *child4;
    struct T3dNode *child8;
};

struct T3dL
{
    u8 filler0[4];
    struct Vec3 unk4;
    struct Vec3 unk18;
    struct Vec3 unk2C;
    struct Mat4 unk40;
};

struct T3dC
{
    u8 filler0[8];
    struct Vec3 unk8;
    struct Vec3 unk1C;
    struct Vec3 unk30;
    struct Vec3 unk44;
    struct Mat4 unk58;
};

struct Fade
{
    u8 filler0[0x18];
    const void *cb18;
    u8 unk1C;
    u8 filler1D[7];
    struct TaskObj *unk24;
    u8 unk28;
    u8 filler29;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
};

struct T3dF
{
    s32 unk0;
    s32 unk4;
    s32 count8;
    struct T3dNode *root;
    u8 filler10[0x10];
    struct Vec3 unk20;
    struct Vec3 unk34;
    struct Mat4 unk48;
    struct Mat4 unk8C;
    s32 unkD0;
    void *unkD4;
    s32 unkD8;
    void *unkDC;
};

struct T3dModel
{
    struct T3dC *unk0;
    struct T3dL *unk4;
    s32 unk8;
    struct T3dNode *unkC;
    struct Mat4 unk10;
};

extern u16 gUnk_02000050[];
extern u16 gUnk_02000054;
extern u16 gUnk_0300034C[];
void fade_end(struct Fade *obj);
extern const u8 gAppearCallback10Class[];
extern const u8 gAppearCallback40Class[];
extern const u8 gFadeClass[];
extern const u8 gTaskRemoveClass[];
extern const u8 gHeapTagHdrw[];
extern const u8 gHeapTagHasd[];
extern const u8 gHeapTagT3dc[];
extern const u8 gHeapTagT3dl[];
extern const u8 gHeapTagT3df[];

void heap_free_a(void *p);
void heap_free_b(void *p);
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void mat4_mul(struct Mat4 *out, struct Mat4 *a, struct Mat4 *b);
void mat4_apply_translate(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_apply_rotate_yxz(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_apply_rotate_zxy(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_apply_scale(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_set_identity(struct Mat4 *m);
void vec3_zero(struct Vec3 *v);
void vec3_copy(struct Vec3 *dst, struct Vec3 *src);
void tree_walk_children(struct T3dNode *node);
void t3d_node_free(struct T3dNode *node, u32 flags);
struct T3dF *t3d_f_init(struct T3dF *f);
void t3d_node_update(struct T3dNode *node, struct Mat4 *m);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);

ASM_FUNC("asm/code_08137510/fade_tick.s", void fade_tick(void))

void task_remove_noop(void)
{
}

void obj_callback_18_task_remove(struct TaskObj *obj, u32 task)
{
    obj->cb18 = gTaskRemoveClass;
    task_remove(obj, task);
}

struct TaskObj *obj_callback_18_task_insert(struct TaskObj *obj, u8 kind, const void *tag)
{
    task_insert(obj, kind);
    obj->cb18 = gTaskRemoveClass;
    return obj;
}

NONMATCH("asm/code_08137510/fade_end.s", void fade_end(struct Fade *obj))
{
    vu16 *p;

    p = gUnk_02000050;
    *p = obj->unk2C;
    p += 2;
    *p = obj->unk2A;
    if (obj->unk24 != 0)
    {
        obj_callback_18_task_remove(obj->unk24, 3);
    }
    obj->unk24 = 0;
    if (obj != 0)
    {
        obj->cb18 = gFadeClass;
        task_remove(obj, 3);
    }
}
END_NONMATCH

void obj_callback_18_remove_de60(struct TaskObj *obj, u32 task)
{
    obj->cb18 = gFadeClass;
    task_remove(obj, task);
}

NONMATCH("asm/code_08137510/fade_start.s", struct Fade *fade_start(struct Fade *obj, u8 kind))
{
    struct TaskObj *t;
    vu16 *p;

    task_insert(obj, kind);
    obj->cb18 = gFadeClass;
    obj->unk1C = 0;
    t = obj_callback_18_task_insert(heap_alloc2(0x1C, 0, gHeapTagHdrw, 0), 8, gHeapTagHasd);
    obj->unk24 = t;
    t->owner14 = obj;
    obj->unk28 = 0;
    obj->unk2E = 0x1000;
    p = gUnk_02000050;
    obj->unk2C = *p;
    *p = 0xFF;
    p += 2;
    obj->unk2A = *p;
    *p = 0x10;
    p -= 0x2A;
    *p = 0x1140;
    return obj;
}
END_NONMATCH

s8 get_s8_28(u8 *obj)
{
    return *(s8 *)(obj + 0x28);
}

ASM_FUNC("asm/code_08137510/obj_random_pose.s", void obj_random_pose(void))

ASM_FUNC("asm/code_08137510/obj_pick_next_state.s", void obj_pick_next_state(void))

ASM_FUNC("asm/code_08137510/obj_find_active_state.s", void obj_find_active_state(void))

ASM_FUNC("asm/code_08137510/obj_begin_glide.s", void obj_begin_glide(void))
