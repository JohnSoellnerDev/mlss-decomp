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

/* Constructs a T3dModel in place: identity base matrix, plus a freshly allocated
 * T3dC and T3dL (vectors zeroed, matrices identity) and a T3dF node tree. */
struct T3dModel *t3d_model_new(struct T3dModel *model)
{
    struct Mat4 *modelMat = &model->unk10;
    struct T3dC *t3dc;
    struct T3dL *t3dl;
    struct Vec3 *vec;
    struct Mat4 *t3dcMat;
    struct Mat4 *t3dlMat;

    modelMat->cb40 = gAppearCallback40Class;
    mat4_set_identity(modelMat);

    t3dc = heap_alloc2(sizeof(struct T3dC), 1, gHeapTagT3dc, 0);
    vec = &t3dc->unk8;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    vec = &t3dc->unk1C;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    vec = &t3dc->unk30;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    vec = &t3dc->unk44;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    t3dcMat = &t3dc->unk58;
    t3dcMat->cb40 = gAppearCallback40Class;
    mat4_set_identity(t3dcMat);
    model->unk0 = t3dc;

    t3dl = heap_alloc2(sizeof(struct T3dL), 1, gHeapTagT3dl, 0);
    vec = &t3dl->unk4;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    vec = &t3dl->unk18;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    vec = &t3dl->unk2C;
    vec->cb10 = gAppearCallback10Class;
    vec3_zero(vec);
    t3dlMat = &t3dl->unk40;
    t3dlMat->cb40 = gAppearCallback40Class;
    mat4_set_identity(t3dlMat);
    model->unk4 = t3dl;

    model->unk8 = 0;
    model->unkC = (struct T3dNode *)t3d_f_init(heap_alloc2(sizeof(struct T3dF), 1, gHeapTagT3df, 0));
    return model;
}
void t3d_model_free(struct T3dModel *m, u32 flags)
{
    struct T3dC *c = m->unk0;
    struct T3dL *l;

    if (c != 0)
    {
        struct Mat4 *cm = &c->unk58;

        cm->cb40 = gAppearCallback40Class;
        c->unk44.cb10 = gAppearCallback10Class;
        c->unk30.cb10 = gAppearCallback10Class;
        c->unk1C.cb10 = gAppearCallback10Class;
        c->unk8.cb10 = gAppearCallback10Class;
        heap_free_b(c);
        m->unk0 = 0;
    }
    l = m->unk4;
    if (l != 0)
    {
        struct Mat4 *lm = &l->unk40;

        lm->cb40 = gAppearCallback40Class;
        l->unk2C.cb10 = gAppearCallback10Class;
        l->unk18.cb10 = gAppearCallback10Class;
        l->unk4.cb10 = gAppearCallback10Class;
        heap_free_b(l);
        m->unk4 = 0;
    }
    if (m->unkC != 0)
    {
        t3d_node_free(m->unkC, 3);
        m->unk8 = 0;
        m->unkC = 0;
    }
    m->unk10.cb40 = gAppearCallback40Class;
    if (flags & 1)
        heap_free_b(m);
}

/* Updates the model's node tree (unkC) with a base matrix: the T3dC's matrix when the model has
 * one, the identity otherwise. */
void t3d_model_update(struct T3dModel *model)
{
    struct Mat4 base;
    struct Mat4 *src;

    base.cb40 = gAppearCallback40Class;
    mat4_set_identity(&base);
    base.m[0] = 0x100;
    base.m[1] = 0;
    base.m[2] = 0;
    base.m[3] = 0;
    base.m[4] = 0;
    base.m[5] = 0x100;
    base.m[6] = 0;
    base.m[7] = 0;
    base.m[8] = 0;
    base.m[9] = 0;
    base.m[10] = 0x100;
    base.m[11] = 0;
    base.m[12] = 0;
    base.m[13] = 0;
    base.m[14] = 0;
    base.m[15] = 0x100;
    if (model->unk0 != NULL)
    {
        src = &model->unk0->unk58;
        base.m[0] = src->m[0];
        base.m[1] = src->m[1];
        base.m[2] = src->m[2];
        base.m[3] = src->m[3];
        base.m[4] = src->m[4];
        base.m[5] = src->m[5];
        base.m[6] = src->m[6];
        base.m[7] = src->m[7];
        base.m[8] = src->m[8];
        base.m[9] = src->m[9];
        base.m[10] = src->m[10];
        base.m[11] = src->m[11];
        base.m[12] = src->m[12];
        base.m[13] = src->m[13];
        base.m[14] = src->m[14];
        base.m[15] = src->m[15];
    }
    if (model->unkC != NULL)
        t3d_node_update(model->unkC, &base);
    base.cb40 = gAppearCallback40Class;
}

void t3d_f_walk(struct T3dF *f)
{
    struct T3dNode *node = f->root;

    if (node != 0)
    {
        if (node->child4 != 0)
            tree_walk_children(node->child4);
        if (node->child8 != 0)
            tree_walk_children(node->child8);
    }
}

void t3d_f_append_node(struct T3dF *f, struct T3dNode *node)
{
    struct T3dNode *head = f->root;

    if (head == 0)
    {
        f->root = node;
        node->prev0 = head;
    }
    else
    {
        struct T3dNode *cur = head->child8;

        if (cur == 0)
        {
            head->child8 = node;
            f->root->child8->prev0 = f->root;
        }
        else
        {
            if (cur->child4 != 0)
            {
                do
                    cur = cur->child4;
                while (cur->child4 != 0);
            }
            cur->child4 = node;
            node->prev0 = cur->prev0;
        }
    }
    f->count8++;
}

void sub_08136C38(void)
{
}

void t3d_l_release(struct T3dL *obj, u32 flags)
{
    struct Mat4 *m = &obj->unk40;

    m->cb40 = gAppearCallback40Class;
    obj->unk2C.cb10 = gAppearCallback10Class;
    obj->unk18.cb10 = gAppearCallback10Class;
    obj->unk4.cb10 = gAppearCallback10Class;
    if (flags & 1)
        heap_free_b(obj);
}

struct T3dL *t3d_l_init(struct T3dL *obj)
{
    struct Vec3 *v = &obj->unk4;

    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    v = &obj->unk18;
    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    v = &obj->unk2C;
    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    {
        struct Mat4 *m = &obj->unk40;

        m->cb40 = gAppearCallback40Class;
        mat4_set_identity(m);
    }
    return obj;
}

void t3d_c_free(void)
{
}

void t3d_c_release(struct T3dC *obj, u32 flags)
{
    struct Mat4 *m = &obj->unk58;

    m->cb40 = gAppearCallback40Class;
    obj->unk44.cb10 = gAppearCallback10Class;
    obj->unk30.cb10 = gAppearCallback10Class;
    obj->unk1C.cb10 = gAppearCallback10Class;
    obj->unk8.cb10 = gAppearCallback10Class;
    if (flags & 1)
        heap_free_b(obj);
}

struct T3dC *t3d_c_init(struct T3dC *obj)
{
    struct Vec3 *v = &obj->unk8;

    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    v = &obj->unk1C;
    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    v = &obj->unk30;
    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    v = &obj->unk44;
    v->cb10 = gAppearCallback10Class;
    vec3_zero(v);
    {
        struct Mat4 *m = &obj->unk58;

        m->cb40 = gAppearCallback40Class;
        mat4_set_identity(m);
    }
    return obj;
}

void tree_walk_children(struct T3dNode *node)
{
    if (node->child4 != 0)
        tree_walk_children(node->child4);
    if (node->child8 != 0)
        tree_walk_children(node->child8);
}

void mat4_set_identity(struct Mat4 *m)
{
    m->m[0] = 0x100;
    m->m[1] = 0;
    m->m[2] = 0;
    m->m[3] = 0;
    m->m[4] = 0;
    m->m[5] = 0x100;
    m->m[6] = 0;
    m->m[7] = 0;
    m->m[8] = 0;
    m->m[9] = 0;
    m->m[10] = 0x100;
    m->m[11] = 0;
    m->m[12] = 0;
    m->m[13] = 0;
    m->m[14] = 0;
    m->m[15] = 0x100;
}

void mat4_translate_v(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_translate(m, v->x, v->y, v->z);
}

void mat4_rotate_yxz_v(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_rotate_yxz(m, v->x, v->y, v->z);
}

void mat4_rotate_zxy_v(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_rotate_zxy(m, v->x, v->y, v->z);
}

void mat4_scale_v(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_scale(m, v->x, v->y, v->z);
}

struct Mat4 *mat4_mul_inplace(struct Mat4 *dst, struct Mat4 *src)
{
    struct Mat4 tmp;

    mat4_mul(&tmp, dst, src);
    dst->m[0] = tmp.m[0];
    dst->m[1] = tmp.m[1];
    dst->m[2] = tmp.m[2];
    dst->m[3] = tmp.m[3];
    dst->m[4] = tmp.m[4];
    dst->m[5] = tmp.m[5];
    dst->m[6] = tmp.m[6];
    dst->m[7] = tmp.m[7];
    dst->m[8] = tmp.m[8];
    dst->m[9] = tmp.m[9];
    dst->m[10] = tmp.m[10];
    dst->m[11] = tmp.m[11];
    dst->m[12] = tmp.m[12];
    dst->m[13] = tmp.m[13];
    dst->m[14] = tmp.m[14];
    dst->m[15] = tmp.m[15];
    tmp.cb40 = gAppearCallback40Class;
    return dst;
}
