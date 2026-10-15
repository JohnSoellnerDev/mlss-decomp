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
void mat4_set_identity(struct Mat4 *m);
void vec3_zero(struct Vec3 *v);
void vec3_copy(struct Vec3 *dst, struct Vec3 *src);
void tree_walk_children(struct T3dNode *node);
void t3d_node_free(struct T3dNode *node, u32 flags);
struct T3dF *t3d_f_init(struct T3dF *f);
void t3d_node_update(struct T3dNode *node, struct Mat4 *m);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);

ASM_FUNC("asm/code_08136260/mat4_apply_translate.s", void mat4_apply_translate(struct Mat4 *m, s32 x, s32 y, s32 z))

struct T3dF *t3d_f_init(struct T3dF *f)
{
    struct Vec3 *v = (struct Vec3 *)&f->root;
    struct Vec3 *v2;
    struct Vec3 *v3;
    struct Mat4 *m;

    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    v2 = &f->unk20;
    v2->cb10 = gAppearCallback10Class;
    vec3_zero(v2);
    v3 = &f->unk34;
    v3->cb10 = gAppearCallback10Class;
    vec3_zero(v3);
    m = &f->unk48;
    m->cb40 = gAppearCallback40Class;
    mat4_set_identity(m);
    m = &f->unk8C;
    m->cb40 = gAppearCallback40Class;
    mat4_set_identity(m);
    f->unk0 = 0;
    f->unk4 = 0;
    f->count8 = 0;
    f->root = 0;
    v->y = 0;
    v->z = 0;
    v2->x = 0;
    v2->y = 0;
    v2->z = 0;
    v3->x = 0;
    v3->y = 0;
    v3->z = 0;
    f->unkD4 = 0;
    f->unkDC = 0;
    f->unkD0 = 0;
    f->unkD8 = 0;
    return f;
}

ASM_FUNC("asm/code_08136260/t3d_node_free.s", void t3d_node_free(struct T3dNode *node, u32 flags))
ASM_FUNC("asm/code_08136260/t3d_load_nodes.s", void t3d_load_nodes(void))

ASM_FUNC("asm/code_08136260/t3d_node_update.s", void t3d_node_update(struct T3dNode *node, struct Mat4 *m))
