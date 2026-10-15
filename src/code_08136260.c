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
    u8 filler0[0x18];
    const void *cb18;
};

struct T3dNode
{
    u8 filler0[4];
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

extern const u8 gAppearCallback10Class[];
extern const u8 gAppearCallback40Class[];
extern const u8 gFadeClass[];
extern const u8 gTaskRemoveClass[];

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

ASM_FUNC("asm/code_08136260/mat4_apply_translate.s", void mat4_apply_translate(struct Mat4 *m, s32 x, s32 y, s32 z))

ASM_FUNC("asm/code_08136260/t3d_f_init.s", void t3d_f_init(void))

ASM_FUNC("asm/code_08136260/t3d_node_free.s", void t3d_node_free(void))

ASM_FUNC("asm/code_08136260/t3d_load_nodes.s", void t3d_load_nodes(void))

ASM_FUNC("asm/code_08136260/t3d_node_update.s", void t3d_node_update(void))

ASM_FUNC("asm/code_08136260/t3d_model_new.s", void t3d_model_new(void))

ASM_FUNC("asm/code_08136260/t3d_model_free.s", void t3d_model_free(void))

ASM_FUNC("asm/code_08136260/t3d_model_update.s", void t3d_model_update(void))

struct T3dF
{
    u8 filler0[0xC];
    struct T3dNode *root;
};

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

ASM_FUNC("asm/code_08136260/sub_08136BF4.s", void sub_08136BF4(void))

void sub_08136C38(void)
{
}

void sub_08136C3C(struct T3dL *obj, u32 flags)
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
    sub_08137278(v);
    v = &obj->unk18;
    v->cb10 = gAppearCallback10Class;
    sub_08137278(v);
    v = &obj->unk2C;
    v->cb10 = gAppearCallback10Class;
    sub_08137278(v);
    {
        struct Mat4 *m = &obj->unk40;

        m->cb40 = gAppearCallback40Class;
        sub_08136D48(m);
    }
    return obj;
}

void t3d_c_free(void)
{
}

void sub_08136CB0(struct T3dC *obj, u32 flags)
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
    sub_08137278(v);
    v = &obj->unk1C;
    v->cb10 = gAppearCallback10Class;
    sub_08137278(v);
    v = &obj->unk30;
    v->cb10 = gAppearCallback10Class;
    sub_08137278(v);
    v = &obj->unk44;
    v->cb10 = gAppearCallback10Class;
    sub_08137278(v);
    {
        struct Mat4 *m = &obj->unk58;

        m->cb40 = gAppearCallback40Class;
        sub_08136D48(m);
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

void sub_08136D48(struct Mat4 *m)
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

void sub_08136D70(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_translate(m, v->x, v->y, v->z);
}

void sub_08136D84(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_rotate_yxz(m, v->x, v->y, v->z);
}

void sub_08136D98(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_rotate_zxy(m, v->x, v->y, v->z);
}

void sub_08136DAC(struct Mat4 *m, struct Vec3 *v)
{
    mat4_apply_scale(m, v->x, v->y, v->z);
}

struct Mat4 *sub_08136DC0(struct Mat4 *dst, struct Mat4 *src)
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

ASM_FUNC("asm/code_08136260/mat4_add.s", void mat4_add(struct Mat4 *dst, struct Mat4 *a, struct Mat4 *b))

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

ASM_FUNC("asm/code_08136260/mat4_init_words.s", void mat4_init_words(struct Mat4 *m, s32 v0, s32 v1, s32 v2, s32 v3, s32 v4, s32 v5, s32 v6, s32 v7, s32 v8, s32 v9, s32 v10, s32 v11, s32 v12, s32 v13, s32 v14, s32 v15))

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

ASM_FUNC("asm/code_08136260/vec3_xform_to.s", void vec3_xform_to(void))

ASM_FUNC("asm/code_08136260/vec3_xform_inplace.s", void vec3_xform_inplace(void))

ASM_FUNC("asm/code_08136260/vec3_normalize_q24.s", void vec3_normalize_q24(void))

s32 vec3_length(struct Vec3 *v)
{
    return MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(v->x * v->x + v->y * v->y + v->z * v->z);
}

ASM_FUNC("asm/code_08136260/vec3_cross_self.s", void vec3_cross_self(void))

ASM_FUNC("asm/code_08136260/vec3_cross_2.s", void vec3_cross_2(void))

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

ASM_FUNC("asm/code_08136260/sub_08137284.s", void sub_08137284(void))

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

ASM_FUNC("asm/code_08136260/vec3_cross_self_2.s", void vec3_cross_self_2(void))

ASM_FUNC("asm/code_08136260/vec3_cross.s", void vec3_cross(void))

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

ASM_FUNC("asm/code_08136260/fade_tick.s", void fade_tick(void))

void task_remove_noop(void)
{
}

void obj_callback_18_task_remove(struct TaskObj *obj, u32 task)
{
    obj->cb18 = gTaskRemoveClass;
    task_remove(obj, task);
}

struct TaskObj *obj_callback_18_task_insert(struct TaskObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->cb18 = gTaskRemoveClass;
    return obj;
}

ASM_FUNC("asm/code_08136260/fade_end.s", void fade_end(void))

void obj_callback_18_remove_de60(struct TaskObj *obj, u32 task)
{
    obj->cb18 = gFadeClass;
    task_remove(obj, task);
}

ASM_FUNC("asm/code_08136260/fade_start.s", void fade_start(void))

s8 get_s8_28(u8 *obj)
{
    return *(s8 *)(obj + 0x28);
}

ASM_FUNC("asm/code_08136260/obj_random_pose.s", void obj_random_pose(void))

ASM_FUNC("asm/code_08136260/obj_pick_next_state.s", void obj_pick_next_state(void))

ASM_FUNC("asm/code_08136260/obj_find_active_state.s", void obj_find_active_state(void))

ASM_FUNC("asm/code_08136260/obj_begin_glide.s", void obj_begin_glide(void))
