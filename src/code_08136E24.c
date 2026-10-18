#include "global.h"
#include "ram_routines.h"

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
extern u16 gSys[];
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

NONMATCH("asm/code_08136E24/mat4_add.s", struct Mat4 *mat4_add(struct Mat4 *dst, struct Mat4 *a, struct Mat4 *b))
{
    s32 t7;
    s32 t8;
    s32 t9;
    s32 t10;
    s32 t11;
    s32 t12;
    s32 t13;
    s32 t14;
    s32 t15;
    dst->m[0] = a->m[0] + b->m[0];
    dst->m[1] = a->m[1] + b->m[1];
    dst->m[2] = a->m[2] + b->m[2];
    dst->m[3] = a->m[3] + b->m[3];
    dst->m[4] = a->m[4] + b->m[4];
    dst->m[5] = a->m[5] + b->m[5];
    dst->m[6] = a->m[6] + b->m[6];
    t7 = a->m[7] + b->m[7];
    t8 = a->m[8] + b->m[8];
    t9 = a->m[9] + b->m[9];
    t10 = a->m[10] + b->m[10];
    t11 = a->m[11] + b->m[11];
    t12 = a->m[12] + b->m[12];
    t13 = a->m[13] + b->m[13];
    t14 = a->m[14] + b->m[14];
    t15 = a->m[15] + b->m[15];
    dst->cb40 = gAppearCallback40Class;
    dst->m[7] = t7;
    dst->m[8] = t8;
    dst->m[9] = t9;
    dst->m[10] = t10;
    dst->m[11] = t11;
    dst->m[12] = t12;
    dst->m[13] = t13;
    dst->m[14] = t14;
    dst->m[15] = t15;
    return dst;
}
END_NONMATCH

void mat4_copy(struct Mat4 *dst, struct Mat4 *src)
{
    dst->m[0] = src->m[0];
    dst->m[1] = src->m[1];
    dst->m[2] = src->m[2];
    dst->m[3] = src->m[3];
    dst->m[4] = src->m[4];
    dst->m[5] = src->m[5];
    dst->m[6] = src->m[6];
    dst->m[7] = src->m[7];
    dst->m[8] = src->m[8];
    dst->m[9] = src->m[9];
    dst->m[10] = src->m[10];
    dst->m[11] = src->m[11];
    dst->m[12] = src->m[12];
    dst->m[13] = src->m[13];
    dst->m[14] = src->m[14];
    dst->m[15] = src->m[15];
}

void obj_callback_40_free(struct Mat4 *obj, u32 flags)
{
    obj->cb40 = gAppearCallback40Class;
    if (flags & 1)
        heap_free_b(obj);
}

struct Mat4 *mat4_init_words(struct Mat4 *m, s32 v0, s32 v1, s32 v2, s32 v3, s32 v4, s32 v5, s32 v6, s32 v7, s32 v8, s32 v9, s32 v10, s32 v11, s32 v12, s32 v13, s32 v14, s32 v15)
{
    m->cb40 = gAppearCallback40Class;
    m->m[0] = v0;
    m->m[1] = v1;
    m->m[2] = v2;
    m->m[3] = v3;
    m->m[4] = v4;
    m->m[5] = v5;
    m->m[6] = v6;
    m->m[7] = v7;
    m->m[8] = v8;
    m->m[9] = v9;
    m->m[10] = v10;
    m->m[11] = v11;
    m->m[12] = v12;
    m->m[13] = v13;
    m->m[14] = v14;
    m->m[15] = v15;
    return m;
}
