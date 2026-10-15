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
void sub_08136D48(struct Mat4 *m);
void sub_08137278(struct Vec3 *v);
void vec3_copy(struct Vec3 *dst, struct Vec3 *src);
void tree_walk_children(struct T3dNode *node);
void t3d_node_free(struct T3dNode *node, u32 flags);
struct T3dF *t3d_f_init(struct T3dF *f);
void t3d_node_update(struct T3dNode *node, struct Mat4 *m);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);

void mat4_clone(struct Mat4 *dst, struct Mat4 *src)
{
    dst->cb40 = gAppearCallback40Class;
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

struct Mat4 *mat4_new_identity(struct Mat4 *m)
{
    m->cb40 = gAppearCallback40Class;
    sub_08136D48(m);
    return m;
}

struct Vec3 *vec3_xform_to(struct Vec3 *dst, struct Vec3 *v, struct Mat4 *m)
{
    struct Vec3 tmp;

    tmp.cb10 = gAppearCallback10Class;
    sub_08137278(&tmp);
    tmp.x = v->x * m->m[0] + v->y * m->m[4] + v->z * m->m[8] + m->m[12];
    tmp.y = v->x * m->m[1] + v->y * m->m[5] + v->z * m->m[9] + m->m[13];
    tmp.z = v->x * m->m[2] + v->y * m->m[6] + v->z * m->m[10] + m->m[14];
    dst->cb10 = gAppearCallback10Class;
    vec3_copy(dst, &tmp);
    tmp.cb10 = gAppearCallback10Class;
    return dst;
}

struct Vec3 *vec3_xform_inplace(struct Vec3 *v, struct Mat4 *m)
{
    struct Vec3 tmp;

    vec3_xform_to(&tmp, v, m);
    v->x = tmp.x;
    v->y = tmp.y;
    v->z = tmp.z;
    tmp.cb10 = gAppearCallback10Class;
    return v;
}

void vec3_normalize_q24(struct Vec3 *v)
{
    s32 inv = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x10000, MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v->x * v->x + v->y * v->y + v->z * v->z));

    v->x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v->x * inv, 0x100);
    v->y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v->y * inv, 0x100);
    v->z = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v->z * inv, 0x100);
}

s32 vec3_length(struct Vec3 *v)
{
    return MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v->x * v->x + v->y * v->y + v->z * v->z);
}

void mem_copy_fast(void *dst, void *src, u32 len);

static inline struct Vec3 *vec3_cross_store(struct Vec3 *dst, struct Vec3 *a, struct Vec3 *b)
{
    struct Vec3 tmp;
    s32 x = a->y * b->z - a->z * b->y;
    s32 y = a->z * b->x - a->x * b->z;
    s32 z = a->x * b->y - a->y * b->x;

    tmp.cb10 = gAppearCallback10Class;
    tmp.x = x;
    tmp.y = y;
    tmp.z = z;
    mem_copy_fast(&tmp, &tmp, sizeof(tmp));
    dst->x = tmp.x;
    dst->y = tmp.y;
    dst->z = tmp.z;
    tmp.cb10 = gAppearCallback10Class;
    return dst;
}

struct Vec3 *vec3_cross_self(struct Vec3 *a, struct Vec3 *b)
{
    return vec3_cross_store(a, a, b);
}

struct Vec3 *vec3_cross_2(struct Vec3 *dst, struct Vec3 *a, struct Vec3 *b)
{
    return vec3_cross_store(dst, a, b);
}

s32 vec3_dot(struct Vec3 *a, struct Vec3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void sub_08137278(struct Vec3 *v)
{
    v->x = 0;
    v->y = 0;
    v->z = 0;
}

struct Vec3 *sub_08137284(struct Vec3 *v, s32 s)
{
    v->x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v->x * s, 0x100);
    v->y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v->y * s, 0x100);
    v->z = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(v->z * s, 0x100);
    return v;
}

struct Vec3 *vec3_scale(struct Vec3 *dst, struct Vec3 *src, s32 s)
{
    s32 x = src->x * s;
    s32 y = src->y * s;
    s32 z = src->z * s;

    dst->cb10 = gAppearCallback10Class;
    dst->x = x;
    dst->y = y;
    dst->z = z;
    return dst;
}

struct Vec3 *vec3_cross_self_2(struct Vec3 *a, struct Vec3 *b)
{
    return vec3_cross_store(a, a, b);
}

struct Vec3 *vec3_cross(struct Vec3 *dst, struct Vec3 *a, struct Vec3 *b)
{
    s32 x = a->y * b->z - a->z * b->y;
    s32 y = a->z * b->x - a->x * b->z;
    s32 z = a->x * b->y - a->y * b->x;

    dst->cb10 = gAppearCallback10Class;
    dst->x = x;
    dst->y = y;
    dst->z = z;
    return dst;
}

struct Vec3 *vec3_neg_to(struct Vec3 *dst, struct Vec3 *src)
{
    s32 x = -src->x;
    s32 y = -src->y;
    s32 z = -src->z;

    dst->cb10 = gAppearCallback10Class;
    dst->x = x;
    dst->y = y;
    dst->z = z;
    return dst;
}

void vec3_sub_self(struct Vec3 *a, struct Vec3 *b)
{
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

struct Vec3 *sub_08137424(struct Vec3 *dst, struct Vec3 *a, struct Vec3 *b)
{
    s32 x = a->x - b->x;
    s32 y = a->y - b->y;
    s32 z = a->z - b->z;

    dst->cb10 = gAppearCallback10Class;
    dst->x = x;
    dst->y = y;
    dst->z = z;
    return dst;
}

void vec3_add_self(struct Vec3 *a, struct Vec3 *b)
{
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

struct Vec3 *sub_08137468(struct Vec3 *dst, struct Vec3 *a, struct Vec3 *b)
{
    s32 x = a->x + b->x;
    s32 y = a->y + b->y;
    s32 z = a->z + b->z;

    dst->cb10 = gAppearCallback10Class;
    dst->x = x;
    dst->y = y;
    dst->z = z;
    return dst;
}

void vec3_copy(struct Vec3 *dst, struct Vec3 *src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void obj_callback_10_free(struct Vec3 *obj, u32 flags)
{
    obj->cb10 = gAppearCallback10Class;
    if (flags & 1)
        heap_free_b(obj);
}

struct Vec3 *vec3_clone(struct Vec3 *dst, struct Vec3 *src)
{
    dst->cb10 = gAppearCallback10Class;
    vec3_copy(dst, src);
    return dst;
}

struct Vec3 *vec3_init(struct Vec3 *v, s32 x, s32 y, s32 z)
{
    v->cb10 = gAppearCallback10Class;
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

struct Vec3 *vec3_init_zero(struct Vec3 *v)
{
    v->cb10 = gAppearCallback10Class;
    sub_08137278(v);
    return v;
}
