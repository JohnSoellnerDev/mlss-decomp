#include "global.h"
#include "ram_routines.h"

struct Obj
{
    u32 unk0;
    struct Obj *unk4;
    struct Obj *unk8;
    const void *unkC;
    void *unk10;
    void *unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u32 unk40;
    const void *unk44;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
    u8 filler4E[0x52 - 0x4E];
    u16 unk52;
    u16 unk54;
    u16 unk56;
    u16 unk58;
    u16 unk5A;
    u8 filler5C[0x64 - 0x5C];
    void *unk64;
};

struct ObjList
{
    struct Obj *head;
    struct Obj *tail;
    u32 count;
};

struct ObjClass
{
    u8 filler0[8];
    s16 unk8;
    u16 unkA;
    void (*unkC)(void *, u32);
};

struct Sprite
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10[0x1F - 0x10];
    u8 unk1F;
};

struct Triple
{
    s32 x;
    s32 y;
    s32 z;
};

extern const u8 gTweenGroupVtable[];
extern const u8 gClassEad8[];
extern const u8 gClassEae8[];
extern s16 gUnk_0300102C;
extern const u8 gUnk_082138FA[];
extern const u8 gObjScript08213a04[];
extern u8 gUnk_0300034C[];
extern const void *gChildTableByLang[];
extern const u8 gClassEac8[];
extern const u8 gObjScript08213936[];
extern const u8 gObjScript08213b8a[];
extern const s16 gAnimLangSingleChild[];
extern const u8 gPackedGfxBufferTag[];
extern u32 gUnk_03001028;
extern void (*gUnk_03000C84)(const u8 *src, void *dst);

void obj_set_class_38fa(struct Obj *obj, u32 mode, u32 offset);
u32 obj_script_step(struct Obj *obj);
void update_obj_sprite_pos(struct Obj *obj, u32 camX, u32 camY);
void obj_release_slots(void *sprite);
void sprite_update_cd0(void *sprite);
void heap_free_b(void *ptr);
void tween_groups_obj_start_move(struct Obj *obj, u32 dist, u32 target, u32 useRam);
void obj_list_drain(struct ObjList *list);
void obj_set_home_params(struct Obj *obj, u32 a, u32 b, u32 c, u8 flag);
void release_obj_131c(void *sprite);
void obj_load_record(void *sprite, s32 anim, u32 a, u32 b, u32 c);
u32 read_varlen_plus1(const u8 *src);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void heap_free(void *ptr);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);

static inline void obj_run_behavior(struct Obj *child)
{
    if (child != 0)
    {
        struct ObjClass *cls = (struct ObjClass *)child->unkC;

        cls->unkC((u8 *)child + cls->unk8, 3);
    }
}
void cpu_fast_set(void *src, void *dst, u32 control);

void obj_set_child_table(struct Obj *obj)
{
    const void **table = gChildTableByLang;
    const void **entry = table + gUnk_0300034C[0x8C0];
    struct Obj *child = (struct Obj *)obj->unkC;

    child->unk44 = *entry;
    child->unk48 = 0;
    child->unk40 = 0;
}

ASM_FUNC("asm/code_08164468/obj_load_scaled_anims.s", void obj_load_scaled_anims(void))

ASM_FUNC("asm/code_08164468/obj_init_children_sprites.s", void obj_init_children_sprites(struct Obj *obj))

void obj_refresh_children(struct Obj *obj)
{
    struct Obj **children = (struct Obj **)obj;

    obj_script_step(children[0]);
    update_obj_sprite_pos(children[0], 0, 0);
    obj_script_step(children[1]);
    update_obj_sprite_pos(children[1], 0, 0);
    obj_script_step(children[2]);
    update_obj_sprite_pos(children[2], 0, 0);
    obj_script_step(children[3]);
    update_obj_sprite_pos(children[3], 0, 0);
}

void obj_run_children_free(struct Obj *obj, u32 flags)
{
    obj_run_behavior((struct Obj *)obj->unk0);
    obj_run_behavior(obj->unk4);
    obj_run_behavior(obj->unk8);
    obj_run_behavior((struct Obj *)obj->unkC);
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_08164468/obj_triples_done.s", void obj_triples_done(void))

ASM_FUNC("asm/code_08164468/obj_init_triples_class.s", void obj_init_triples_class(struct Obj *obj))

void obj_set_child_class_3a04(struct Obj *obj, u32 group, u32 slot)
{
    u32 idx = slot - 1;
    u32 off = (group * 3 + idx) * 4;
    struct Obj **children = &obj->unk4;
    struct Obj *child = *(struct Obj **)((u8 *)children + off);

    child->unk44 = gObjScript08213a04;
    child->unk48 = 0;
    child->unk40 = 0;
}

ASM_FUNC("asm/code_08164468/obj_refresh_triples.s", void obj_refresh_triples(struct Obj *obj))

ASM_FUNC("asm/code_08164468/obj_run_triples_free.s", void obj_run_triples_free(void))

u32 obj_slot0_is_empty(struct Obj *obj)
{
    u32 empty = 0;

    if (obj->unk0 == 0)
    {
        empty = 1;
    }
    return empty;
}

void obj_init_single(struct Obj *obj)
{
    struct Obj *child;
    void *sprite;
    s32 anim;

    obj->unk0 = 1;
    sprite = obj->unk4->unk10;
    anim = gAnimLangSingleChild[gUnk_0300034C[0x8C0]];
    obj_load_record(sprite, anim, 0, 0, 0);
    child = obj->unk4;
    child->unk44 = gObjScript08213936;
    child->unk48 = 0;
    child->unk40 = 0;
}

ASM_FUNC("asm/code_08164468/obj_wait_children_done.s", void obj_wait_children_done(struct Obj *obj))

ASM_FUNC("asm/code_08164468/obj_run_children3_free.s", void obj_run_children3_free(struct Obj *obj, u32 flags))

void obj_list_update_pos(struct ObjList *head, u32 camX, u32 camY)
{
    struct Obj *node = head->head;

    while (node != 0)
    {
        update_obj_sprite_pos(node, camX, camY);
        node = node->unk8;
    }
}

void tween_groups_obj_destroy(struct Obj *obj, u32 flags)
{
    obj->unkC = gClassEad8;
    obj_list_drain((struct ObjList *)obj);
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void obj_init_empty(struct Obj *obj)
{
    obj->unk4 = 0;
    obj->unk0 = 0;
    obj->unk8 = 0;
    obj->unkC = gTweenGroupVtable;
}

ASM_FUNC("asm/code_08164468/obj_set_home_params.s", void obj_set_home_params(struct Obj *obj, u32 a, u32 b, u32 c, u8 flag))

void obj_set_class_38fa(struct Obj *obj, u32 mode, u32 offset)
{
    obj->unk4C = 0;
    obj->unk4A = offset;
    obj->unk44 = gUnk_082138FA;
    obj->unk48 = 0;
    obj->unk40 = mode;
}

ASM_FUNC("asm/code_08164468/obj_setup_move.s", void obj_setup_move(struct Obj *obj, u32 a, u32 b, u32 c, u32 dist, u32 target, u8 flag, u8 useRam))

ASM_FUNC("asm/code_08164468/obj_setup_slot.s", void obj_setup_slot(struct Obj *obj, u32 a, u32 b, u32 c, u32 offset, u8 flag))

void tween_start_set_cls38fa(struct Obj *obj, u32 dist, u32 target, u8 useRam)
{
    tween_groups_obj_start_move(obj, dist, target, useRam);
    obj_set_class_38fa(obj, 2, target);
}

void obj_init_class_38fa(struct Obj *obj, u32 offset)
{
    obj_set_class_38fa(obj, 6, offset);
}

void obj_set_table_44(struct Obj *obj, void *table)
{
    obj->unk44 = table;
    obj->unk48 = 0;
}

u32 obj_set_table_and_init(struct Obj *obj, const void *table)
{
    obj->unk44 = table;
    obj->unk48 = 0;
    obj->unk40 = 0;
    return obj_script_step(obj);
}

void obj_set_table_fields(struct Obj *obj, void *table)
{
    obj->unk44 = table;
    obj->unk48 = 0;
    obj->unk40 = 0;
}

u32 obj_is_done(struct Obj *obj)
{
    if (obj->unk44 == 0)
    {
        return 1;
    }
    if (obj->unk40 == 1)
    {
        return 1;
    }
    return 0;
}

void obj_release_sprites(struct Obj *obj)
{
    if (obj->unk10 != 0)
    {
        obj_release_slots(obj->unk10);
    }
    if (obj->unk14 != 0)
    {
        obj_release_slots(obj->unk14);
    }
}

void obj_update_sprites(struct Obj *obj)
{
    if (obj->unk10 != 0)
    {
        sprite_update_cd0(obj->unk10);
    }
    if (obj->unk14 != 0)
    {
        sprite_update_cd0(obj->unk14);
    }
}

void obj_set_fields_18(struct Obj *obj, s32 x, s32 y, s32 z)
{
    obj->unk18 = x;
    obj->unk1C = y;
    obj->unk20 = z;
}

void obj_copy_mode_fields(struct Obj *obj)
{
    obj->unk54 = obj->unk52;
    obj->unk58 = obj->unk56;
}

ASM_FUNC("asm/code_08164468/obj_create_sprite_14.s", void obj_create_sprite_14(struct Obj *obj, u32 a, u32 b, u32 c, s32 anim, u32 prio))

ASM_FUNC("asm/code_08164468/obj_create_sprite_10.s", void obj_create_sprite_10(void))

void obj_set_frame_table(struct Obj *obj, void *table)
{
    obj->unk64 = table;
}

void obj_destroy_sprites(struct Obj *obj, u32 flags)
{
    obj->unkC = gClassEac8;
    if (obj->unk10 != 0)
    {
        release_obj_131c(obj->unk10);
    }
    if (obj->unk14 != 0)
    {
        release_obj_131c(obj->unk14);
    }
    obj->unkC = gClassEae8;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

u32 gate_flag_set(u16 mask)
{
    u32 v = gUnk_0300102C & mask;
    return (-v | v) >> 31;
}

void clear_gate_flags(void)
{
    gUnk_0300102C = 0;
}

u32 obj_list_count_value(struct ObjList *head, u32 value)
{
    struct Obj *node = head->head;
    u32 count = 0;

    while (node != 0)
    {
        if (node->unk0 == value)
        {
            count++;
        }
        node = node->unk8;
    }
    return count;
}

void obj_list_drain(struct ObjList *list)
{
    while (list->head != 0)
    {
        struct Obj *node = list->head;
        struct Obj *prev = node->unk4;

        if (prev != 0)
        {
            if (node->unk8 != 0)
            {
                prev->unk8 = node->unk8;
                node->unk8->unk4 = node->unk4;
            }
            else
            {
                prev->unk8 = 0;
                list->tail = node->unk4;
            }
        }
        else
        {
            if (node->unk8 != 0)
            {
                node->unk8->unk4 = prev;
                list->head = node->unk8;
            }
            else
            {
                list->tail = 0;
                list->head = 0;
            }
        }
        list->count--;
        obj_run_behavior(node);
    }
}

void obj_list_remove_call(struct ObjList *list, struct Obj *node)
{
    struct Obj *prev = node->unk4;

    if (prev != 0)
    {
        if (node->unk8 != 0)
        {
            prev->unk8 = node->unk8;
            node->unk8->unk4 = node->unk4;
        }
        else
        {
            prev->unk8 = 0;
            list->tail = node->unk4;
        }
    }
    else
    {
        if (node->unk8 != 0)
        {
            node->unk8->unk4 = prev;
            list->head = node->unk8;
        }
        else
        {
            list->tail = 0;
            list->head = 0;
        }
    }
    list->count--;
    if (node != 0)
    {
        struct ObjClass *cls = (struct ObjClass *)node->unkC;

        cls->unkC((u8 *)node + cls->unk8, 3);
    }
}

void obj_list_unlink(struct ObjList *list, struct Obj *node)
{
    struct Obj *prev = node->unk4;

    if (prev != 0)
    {
        if (node->unk8 != 0)
        {
            prev->unk8 = node->unk8;
            node->unk8->unk4 = node->unk4;
        }
        else
        {
            prev->unk8 = 0;
            list->tail = node->unk4;
        }
    }
    else
    {
        if (node->unk8 != 0)
        {
            node->unk8->unk4 = prev;
            list->head = node->unk8;
        }
        else
        {
            list->tail = 0;
            list->head = 0;
        }
    }
    list->count--;
}

void list_insert(struct ObjList *list, struct Obj *node)
{
    struct Obj *cur = list->head;

    while (cur != 0)
    {
        if (cur == node)
        {
            return;
        }
        cur = cur->unk8;
    }
    if (list->head == 0)
    {
        node->unk8 = 0;
        node->unk4 = 0;
        list->tail = node;
        list->head = node;
    }
    else
    {
        list->tail->unk8 = node;
        node->unk4 = list->tail;
        node->unk8 = 0;
        list->tail = node;
    }
    list->count++;
}

void list_destroy(struct Obj *list, u32 flags)
{
    list->unkC = gClassEad8;
    obj_list_drain((struct ObjList *)list);
    if ((flags & 1) != 0)
    {
        heap_free_b(list);
    }
}

void init_vt_08cdead8(struct Obj *obj)
{
    obj->unkC = gClassEad8;
    obj->unk4 = 0;
    obj->unk0 = 0;
    obj->unk8 = 0;
}

void obj_class_eae8_free(struct Obj *obj, u32 flags)
{
    obj->unkC = gClassEae8;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void init_vt_08cdeae8(struct Obj *obj, u32 value)
{
    obj->unkC = gClassEae8;
    obj->unk8 = 0;
    obj->unk4 = 0;
    obj->unk0 = value;
}

void palette_fill(u16 color)
{
    u32 value = color | (color << 16);

    cpu_fast_set(&value, (void *)0x05000000, 0x01000080);
    value = color | (color << 16);
    cpu_fast_set(&value, (void *)0x05000200, 0x01000080);
}

void vram_load_packed(const u8 *src, void *dst)
{
    u32 len = read_varlen_plus1(src);
    void *buf = heap_alloc_raw(1, len, gPackedGfxBufferTag);

    gUnk_03000C84(src, buf);
    dma_copy_or_queue(buf, dst, len, 0x10, 2);
    heap_free(buf);
}

s32 lerp_step_08164EE0(s32 a, s32 b, s32 steps, s32 t)
{
    s32 n = steps;

    if (n <= 0)
    {
        n = 1;
    }
    if (t < 0)
    {
        t = 0;
    }
    if (t > n)
    {
        t = n;
    }
    return a + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((b - a) * t, n);
}

ASM_FUNC("asm/code_08164468/smoothstep_lerp.s", u32 smoothstep_lerp(s32 start, s32 end, s32 steps, s32 step))

ASM_FUNC("asm/code_08164468/rand_scaled.s", u32 rand_scaled(u32 count, u32 base))

void vec3_sub_ops(struct Triple *dst, struct Triple *src)
{
    dst->x -= src->x;
    dst->y -= src->y;
    dst->z -= src->z;
}

void sub_08165020(struct Triple *dst, struct Triple *a, struct Triple *b)
{
    dst->x = a->x - b->x;
    dst->y = a->y - b->y;
    dst->z = a->z - b->z;
}

void triple_add(struct Triple *dst, struct Triple *src)
{
    dst->x += src->x;
    dst->y += src->y;
    dst->z += src->z;
}

void vec3_add_to(struct Triple *dst, struct Triple *a, struct Triple *b)
{
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}

void triple_copy(struct Triple *dst, struct Triple *src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void triple_set(struct Triple *dst, s32 a, s32 b, s32 c)
{
    dst->x = a;
    dst->y = b;
    dst->z = c;
}

s32 obj_field_getters(struct Obj *obj)
{
    return obj->unk1C + obj->unk3C + obj->unk20;
}

u32 sub_081650A4(struct Obj *obj)
{
    return obj->unk20;
}

s32 sub_081650A8(struct Obj *obj)
{
    return obj->unk1C + obj->unk3C;
}

u32 sub_081650B4(struct Obj *obj)
{
    return obj->unk18;
}

u32 sub_081650B8(struct Obj *obj)
{
    return (u32)obj->unk8;
}

u32 sub_081650BC(struct Obj *obj)
{
    return (u32)obj->unk4;
}

u32 sub_081650C0(struct Obj *obj)
{
    return obj->unk0;
}

ASM_FUNC("asm/code_08164468/tilemap_obj_init.s", void tilemap_obj_init(void))

ASM_FUNC("asm/code_08164468/window_obj_setup.s", void window_obj_setup(void))

ASM_FUNC("asm/code_08164468/window_build_tiles.s", void window_build_tiles(void))

ASM_FUNC("asm/code_08164468/window_layout_plain.s", void window_layout_plain(void))

ASM_FUNC("asm/code_08164468/win_set_layout.s", void win_set_layout(void))

ASM_FUNC("asm/code_08164468/win_draw_text.s", void win_draw_text(void))

ASM_FUNC("asm/code_08164468/obj_dialog_step.s", void obj_dialog_step(void))
