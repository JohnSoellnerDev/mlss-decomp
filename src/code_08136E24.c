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

ASM_FUNC("asm/code_08136E24/mat4_add.s", void mat4_add(struct Mat4 *dst, struct Mat4 *a, struct Mat4 *b))

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

ASM_FUNC("asm/code_08136E24/mat4_init_words.s", void mat4_init_words(struct Mat4 *m, s32 v0, s32 v1, s32 v2, s32 v3, s32 v4, s32 v5, s32 v6, s32 v7, s32 v8, s32 v9, s32 v10, s32 v11, s32 v12, s32 v13, s32 v14, s32 v15))
