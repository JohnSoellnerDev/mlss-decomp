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

struct Lo2Bits
{
    u8 lo2 : 2;
    u8 rest : 6;
};

struct Prio4Bits
{
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct Triple
{
    s32 x;
    s32 y;
    s32 z;
};

struct DialogObj
{
    s16 *sprite;
    u8 filler4[0x4];
    s32 unk8;
    s32 unkC;
    const u8 *text;
    u8 unk14;
    u8 unk15;
    u8 filler16[0x2];
    s32 unk18;
    s32 unk1C;
    u8 filler20[0x8];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 filler38[0x14];
    void (*unk4C)(void *);
    u8 filler50[0x8];
    const u8 *unk58;
    s32 unk5C;
    u32 state;
    s32 count;
    s32 idx;
    s32 maxIdx;
    s32 pos[4][2];
    u8 filler90[0x10];
    u16 soundOn;
    s16 soundId;
};

extern const u8 gTweenGroupVtable[];
extern const u8 gClassEad8[];
extern const u8 gClassEae8[];
extern s16 gUnk_0300102C;
extern const u8 gUnk_082138FA[];
extern const u8 gObjScript08213a04[];
extern u8 gSys[];
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
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void tween_groups_obj_start_move(struct Obj *obj, u32 dist, u32 target, ByteArg useRam);
void obj_list_drain(struct ObjList *list);
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

static inline u32 child_is_done(struct Obj *child)
{
    if (child->unk44 == 0)
    {
        return 1;
    }
    if (child->unk40 == 1)
    {
        return 1;
    }
    return 0;
}
void cpu_fast_set(void *src, void *dst, u32 control);

void obj_set_child_table(struct Obj *obj)
{
    const void **table = gChildTableByLang;
    const void **entry = table + gSys[0x8C0];
    struct Obj *child = (struct Obj *)obj->unkC;

    child->unk44 = *entry;
    child->unk48 = 0;
    child->unk40 = 0;
}

void obj_load_scaled_anims(struct Obj *obj, u32 value)
{
    s32 (*div)(s32, s32);
    s32 (*scale)(s32, s32);
    s32 anim;

    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424);
    anim = div(scale(value, 0x3E8), 0x64);
    obj_load_record(obj->unk8->unk10, anim, 0, 0, 0);
    anim = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(
        MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424)(value, 0x64), 0xA);
    obj_load_record(obj->unk4->unk10, anim, 0, 0, 0);
    anim = MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424)(value, 0xA);
    obj_load_record(((struct Obj *)obj->unk0)->unk10, anim, 0, 0, 0);
}

void obj_init_children_sprites(struct Obj *obj)
{
    struct Obj **children = (struct Obj **)obj;
    u32 i = 0;
    const void *script = gObjScript08213b8a;

    obj_set_class_38fa(children[0], 6, i * 8);
    children[0]->unk44 = script;
    children[0]->unk48 = 0;
    i++;
    obj_set_class_38fa(children[1], 6, i * 8);
    children[1]->unk44 = script;
    children[1]->unk48 = 0;
    i++;
    obj_set_class_38fa(children[2], 6, i * 8);
    children[2]->unk44 = script;
    children[2]->unk48 = 0;
    i++;
    obj_set_class_38fa(children[3], 6, i * 8);
    children[3]->unk44 = script;
    children[3]->unk48 = 0;
}

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

NONMATCH("asm/code_08164468/obj_triples_done.s", u32 obj_triples_done(struct Obj *obj))
{
    s32 group = 0;
    u32 off = 0;
    u32 base = (u32)&obj->unk4;
    while (1)
    {
        struct Obj *child;
        u32 o;
        child = *(struct Obj **)(base + off);
        if (!child_is_done(child))
        {
            return 0;
        }
        o = off;
        child = *(struct Obj **)(base + (o + 4));
        if (!child_is_done(child))
        {
            return 0;
        }
        child = *(struct Obj **)(base + (o + 8));
        if (!child_is_done(child))
        {
            return 0;
        }
        off += 0xC;
        group++;
        if (group > 1)
        {
            break;
        }
    }
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_08164468/obj_init_triples_class.s", void obj_init_triples_class(struct Obj *obj))
{
    s32 group = 0;
    u32 base = (u32)&obj->unk4;
    s32 next;

    while (1)
    {
        struct Obj *child;
        struct Obj **first;
        struct Obj **entry;
        s32 twice = group << 1;

        next = group + 1;
        entry = (struct Obj **)(((twice + group) << 2) + base);
        first = entry;
        child = *entry++;
        child->unk44 = gObjScript08213a04;
        child->unk48 = 0;
        child->unk40 = 0;
        child = *entry;
        child->unk44 = gObjScript08213a04;
        child->unk48 = 0;
        child->unk40 = 0;
        child = first[2];
        child->unk44 = gObjScript08213a04;
        child->unk48 = 0;
        child->unk40 = 0;
        group = next;
        if (group > 1)
        {
            break;
        }
    }
}
END_NONMATCH

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

void obj_refresh_triples(struct Obj *obj)
{
    s32 group = 0;
    u32 base = (u32)&obj->unk4;
    s32 next;

    while (1)
    {
        s32 twice = group << 1;
        struct Obj **entry;
        struct Obj **first;

        next = group + 1;
        entry = (struct Obj **)(((twice + group) << 2) + base);
        first = entry;

        obj_script_step(*entry);
        update_obj_sprite_pos(*entry++, 0, 0);
        obj_script_step(*entry);
        update_obj_sprite_pos(*entry, 0, 0);
        obj_script_step(first[2]);
        update_obj_sprite_pos(first[2], 0, 0);
        group = next;
        if (group > 1)
        {
            break;
        }
    }
}

void obj_run_triples_free(struct Obj *obj, u32 flags)
{
    s32 group = 0;
    u32 base = (u32)&obj->unk4;
    s32 next;

    while (1)
    {
        s32 twice = group << 1;
        struct Obj **entry;
        struct Obj **first;

        next = group + 1;
        entry = (struct Obj **)(((twice + group) << 2) + base);
        first = entry;
        obj_run_behavior(*entry);
        obj_run_behavior(entry[1]);
        obj_run_behavior(first[2]);
        group = next;
        if (group > 1)
        {
            break;
        }
    }
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

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
    anim = gAnimLangSingleChild[gSys[0x8C0]];
    obj_load_record(sprite, anim, 0, 0, 0);
    child = obj->unk4;
    child->unk44 = gObjScript08213936;
    child->unk48 = 0;
    child->unk40 = 0;
}

NONMATCH("asm/code_08164468/obj_wait_children_done.s", void obj_wait_children_done(struct Obj *obj))
{
    struct Obj **base;
    struct Obj **slot;
    u32 count;

    if (obj->unk0 == 0)
    {
        return;
    }
    if (obj->unk0 != 1)
    {
        return;
    }
    count = 0;
    slot = (struct Obj **)&obj->unk4;
    base = slot;
    do
    {
        if (child_is_done(*slot) != 0)
        {
            count++;
        }
        else
        {
            obj_script_step(*slot);
            update_obj_sprite_pos(*slot, 0, 0);
        }
        slot++;
    } while ((s32)slot <= (s32)(base + 2));
    if (count == 3)
    {
        obj->unk0 = 0;
    }
}
END_NONMATCH

NONMATCH("asm/code_08164468/obj_run_children3_free.s", void obj_run_children3_free(struct Obj *obj, u32 flags))
{
    struct Obj **children = (struct Obj **)&obj->unk4;

    obj_run_behavior(obj->unk4);
    obj_run_behavior(children[1]);
    obj_run_behavior((struct Obj *)obj->unkC);
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}
END_NONMATCH

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

void obj_set_home_params(struct Obj *obj, u32 a, u32 b, u32 c, ByteArg flag)
{
    u32 f = flag.value;
    struct Triple *dst;
    struct Triple *src;
    struct Triple *home;

    obj->unk5A &= 0xFFFC;
    dst = (struct Triple *)&obj->unk30;
    src = (struct Triple *)&obj->unk18;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    home = (struct Triple *)&obj->unk24;
    home->x = a;
    home->y = b;
    home->z = c;
    if (f != 0)
    {
        obj->unk5A |= 1;
    }
}

void obj_set_class_38fa(struct Obj *obj, u32 mode, u32 offset)
{
    obj->unk4C = 0;
    obj->unk4A = offset;
    obj->unk44 = gUnk_082138FA;
    obj->unk48 = 0;
    obj->unk40 = mode;
}

void obj_setup_move(struct Obj *obj, u32 a, u32 b, u32 c, u32 dist, u32 target, ByteArg flag, ByteArg useRam)
{
    u32 f = flag.value;
    u32 ram = useRam.value;

    obj_set_home_params(obj, a, b, c, (u8)f);
    tween_groups_obj_start_move(obj, dist, target, (u8)ram);
    obj_set_class_38fa(obj, 4, target);
}

void obj_setup_slot(struct Obj *obj, u32 a, u32 b, u32 c, u32 offset, ByteArg flag)
{
    obj_set_home_params(obj, a, b, c, flag);
    obj_set_class_38fa(obj, 3, offset);
}

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

void obj_create_sprite_14(struct Obj *obj, u32 a, u32 b, u32 c, s32 anim, u32 prio)
{
    obj->unk14 = create_obj_de4(a, b, c, -1, -1, -1, -1);
    obj_load_record(obj->unk14, anim, -1, 0, 0);
    sprite_update_cd0(obj->unk14);
    ((struct Lo2Bits *)&((struct Sprite *)obj->unk14)->unk1F)->lo2 = prio & 3;
    ((struct Sprite *)obj->unk14)->unkE = 0xFFFF;
}

void obj_create_sprite_10(struct Obj *obj, u32 a, u32 b, u32 c, s32 anim, u32 prio)
{
    struct Prio4Bits *bits;

    obj->unk10 = create_obj_de4(a, b, c, -1, -1, -1, -1);
    obj_load_record(obj->unk10, anim, -1, 0, 0);
    sprite_update_cd0(obj->unk10);
    bits = (struct Prio4Bits *)&((struct Sprite *)obj->unk10)->unk1F;
    bits->f6 = prio;
    bits->f4 = prio;
    bits->f2 = prio;
    bits->f0 = prio;
    ((struct Sprite *)obj->unk10)->unkE = 0;
    obj->unk14 = 0;
}

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

NONMATCH("asm/code_08164468/smoothstep_lerp.s", u32 smoothstep_lerp(s32 start, s32 end, s32 steps, s32 step))
{
    s32 off;
    s32 (*div)(s32, s32);
    s32 q1;
    s32 q2;
    s32 q3;
    s32 q4;
    s32 q5;
    s32 d3;

    if (steps <= 0)
    {
        steps = 1;
    }
    if (step < 0)
    {
        step = 0;
    }
    if (step > steps)
    {
        step = steps;
    }
    off = (u32)iwram_div - (u32)iwram_sqrt;
    div = (s32 (*)(s32, s32))(gMathRoutines + off);
    d3 = end * 3;
    d3 = d3 - start * 3;
    q1 = div(d3 * step, steps);
    q2 = div(step * q1, steps);
    div = (s32 (*)(s32, s32))(gMathRoutines + off);
    q3 = div((start - end) * 2 * step, steps);
    q4 = div(step * q3, steps);
    q5 = div(step * q4, steps);
    return q5 + q2 + start;
}
END_NONMATCH

u32 rand_scaled(u32 count, u32 base)
{
    u32 seed;
    u32 r;
    u32 n;
    u32 (*fn)(u32, u32, u32);

    seed = gUnk_03001028 * 0x41c64e6d + 0x3039;
    gUnk_03001028 = seed;
    fn = MATH_ROUTINE(u32 (*)(u32, u32, u32), iwram_math_08199424);
    r = (seed & 0x7fffffff) >> 16;
    n = count - 1;
    return fn(r, base - n, n) + count;
}

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

void obj_release_tiles(void *obj);
void voice_setup_params(void *voice, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i);
void voice_set_data_ptr(void *voice, u32 a, u32 b, void *ptr);
void obj_flush_tiles(void *obj);
void window_layout_plain(s32 *win, u8 *src, s32 x, s32 y)
{
    obj_release_tiles(win);
    win[10] = x;
    if (x < 4)
    {
        win[10] = 4;
    }
    win[11] = y;
    if (y < 4)
    {
        win[11] = 4;
    }
    win[12] = src[0];
    win[13] = src[1];
    if (win[10] + (win[12] << 3) > 0xEC)
    {
        win[10] = 0xEC - (win[12] << 3);
    }
    if (win[11] + (win[13] << 3) > 0x9C)
    {
        win[11] = 0x9C - (win[13] << 3);
    }
    voice_setup_params(win + 2, win[8], 0x20, 0xC, (((0x20 - win[12]) << 27) + 0xC000000U) >> 24, 0, 0, 6, 1, 0);
    voice_set_data_ptr(win + 2, 0, 6, src + 2);
    win[23] = 0;
    win[22] = 0;
    win[25] = 0;
    win[26] = 0;
    win[27] = 0;
    win[24] = 0;
    obj_release_slots((void *)win[0]);
    *(s16 *)win[20] = -(s16)win[10];
    *(s16 *)win[21] = -(s16)win[11];
    obj_flush_tiles(win);
}

extern void frame_9slice_tiles(void);

NONMATCH("asm/code_08164468/win_set_layout.s", void win_set_layout(s32 *win, u8 *src, s32 x, s32 y))
{
    s32 w;
    s32 h;
    s32 style;

    obj_release_tiles(win);
    win[10] = x;
    if (x < 4)
    {
        win[10] = 4;
    }
    win[11] = y;
    if (y < 4)
    {
        win[11] = 4;
    }
    w = src[0] + 4;
    if (w > 3)
    {
        if (w > 0x1E)
        {
            w = 0x1E;
        }
    }
    else
    {
        w = 3;
    }
    win[12] = w;
    style = src[1];
    if (style != 2 && style != 4 && style != 5)
    {
        h = style + 1;
    }
    h = style + 2;
    if (h > 3)
    {
        if (h > 0xC)
        {
            h = 0xC;
        }
    }
    else
    {
        h = 3;
    }
    win[13] = h;
    if (win[10] + (win[12] << 3) > 0xEC)
    {
        win[10] = 0xEC - (win[12] << 3);
    }
    if (win[11] + (win[13] << 3) > 0x9C)
    {
        win[11] = 0x9C - (win[13] << 3);
    }
    voice_setup_params(win + 2, win[8], 0x20, 0xC, (((0x20 - win[12]) << 27) + 0xC000000U) >> 24, 0, 0, 6, 1, 2);
    voice_set_data_ptr(win + 2, 0, 6, src + 2);
    ((void (*)(void *))(win[19] + ((u32)frame_9slice_tiles - (u32)frame_9slice_tiles)))((u8 *)win + 0x20);
    win[23] = 0;
    win[22] = 0;
    win[25] = 0;
    win[26] = 0;
    win[27] = 0;
    win[24] = 0;
    obj_release_slots((void *)win[0]);
    *(s16 *)win[20] = -(s16)win[10];
    *(s16 *)win[21] = -(s16)win[11];
    obj_flush_tiles(win);
}
END_NONMATCH

void win_set_layout(s32 *win, u8 *src, s32 x, s32 y);
ASM_FUNC("asm/code_08164468/win_draw_text.s", void win_draw_text(void))

ASM_FUNC("asm/code_08164468/obj_dialog_step.s", void obj_dialog_step(void))
