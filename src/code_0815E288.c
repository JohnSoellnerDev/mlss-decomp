#include "global.h"

struct Obj
{
    void *unk00;
    void *klass04;
    void *unk08;
    void *child0C;
    void *unk10;
    void *unk14;
    u8 filler18[0x1A - 0x18];
    u8 unk1A;
};

struct ClassVt
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*fn0C)(void *self, u32 mode);
};

struct Part
{
    u16 unk00;
    u16 unk02;
    u16 unk04;
};

struct Anim
{
    u16 unk00;
    u8 filler02[2];
    void *child04;
    void *child08;
    void *child0C;
    void *child10;
    void *child14;
    u8 filler18[0x36 - 0x18];
    s16 state36;
    s32 pos38;
    u8 filler3C[4];
    s32 speed40;
    u8 filler44[0x48 - 0x44];
    s32 accel48;
};

struct Obj9
{
    void *unk00;
    void *unk04;
    void *unk08;
    void *unk0C;
    void *unk10;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    void *unk24;
};

struct Group9
{
    u16 unk00;
    u8 filler02[2];
    void *parts04[9];
    u16 unk28[9];
    u16 unk3A[9];
    u16 unk4C[9];
};

extern const u8 gClassE440[];
extern const u8 gTransitionTagPhaz[];

void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void release_obj_131c(void *p);
void sprite_update_cd0(void *sprite);
void heap_free_b(void *p);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
s32 transition_scene_step(u8 *game);
void *transition_children_obj_init_scan_pool(void *obj, u8 *game);
void obj_reset_e920(struct Obj *obj);
void obj_init_e4a0(struct Obj *obj);
void obj_release_slots(void *sprite);

extern const u8 gReleaseVtableE920[];
extern const u8 gClassE4a0[];

ASM_FUNC("asm/code_0815E288/sprite_group5_init.s", void sprite_group5_init(void))

ASM_FUNC("asm/code_0815E288/sprite_group5_update.s", void sprite_group5_update(void))

ASM_FUNC("asm/code_0815E288/obj_create_group_e4a0.s", void obj_create_group_e4a0(void))

s32 obj_anim_e620(struct Anim *obj)
{
    void **kids;

    switch (obj->state36)
    {
    case 0:
        obj->speed40 = obj->speed40 - obj->accel48;
        obj->pos38 = obj->pos38 + obj->speed40;
        ((struct Part *)obj->child04)->unk00 = obj->pos38 >> 8;
        ((struct Part *)obj->child08)->unk00 = (obj->pos38 >> 8) + 0x10;
        ((struct Part *)obj->child0C)->unk00 = (obj->pos38 >> 8) + 0x20;
        ((struct Part *)obj->child10)->unk00 = (obj->pos38 >> 8) + 0x30;
        ((struct Part *)obj->child14)->unk00 = (obj->pos38 >> 8) + 0x40;
        if (obj->speed40 < 0)
        {
            obj->unk00 = 0;
            obj->state36 = 1;
        }
        return 0;
    case 1:
        obj->unk00++;
        if ((s16)obj->unk00 > 0x3B)
        {
            obj->unk00 = 0;
            obj->state36 = 2;
            return 3;
        }
        return 0;
    case 2:
        obj->speed40 = obj->speed40 + obj->accel48;
        obj->pos38 = obj->pos38 + obj->speed40;
        kids = &obj->child04;
        ((struct Part *)obj->child04)->unk00 = obj->pos38 >> 8;
        ((struct Part *)kids[1])->unk00 = (obj->pos38 >> 8) + 0x10;
        ((struct Part *)obj->child0C)->unk00 = (obj->pos38 >> 8) + 0x20;
        ((struct Part *)obj->child10)->unk00 = (obj->pos38 >> 8) + 0x30;
        ((struct Part *)obj->child14)->unk00 = (obj->pos38 >> 8) + 0x40;
        if (obj->pos38 > 0xEFFF)
        {
            obj_release_slots(obj->child04);
            obj_release_slots(kids[1]);
            obj_release_slots(obj->child0C);
            obj_release_slots(obj->child10);
            obj_release_slots(obj->child14);
            return 2;
        }
        return 0;
    }
    return 0;
}

NONMATCH("asm/code_0815E288/obj_create_group_e714.s", struct Group9 *obj_create_group_e714(struct Group9 *group))
{
    s32 index;
    s32 next;
    s32 none;
    u32 zero;
    void **parts;
    void *child;

    index = 0;
    none = -1;
    parts = group->parts04;
    zero = 0;
    do
    {
        child = create_obj_de4(4, 0x5005, 0, none, none, none, none);
        parts[index] = child;
        if (index > 3)
        {
            next = index + 1;
            ((struct Part *)child)->unk00 = (next << 4) + 0x28;
        }
        else
        {
            ((struct Part *)child)->unk00 = (index << 4) + 0x28;
            next = index + 1;
        }
        ((struct Part *)parts[index])->unk02 = 0x3C;
        obj_load_record(parts[index], index + 0xA, -1, 0, zero);
        group->unk28[index] = zero;
        group->unk3A[index] = (index << 2) + index;
        group->unk4C[index] = zero;
        index = next;
    } while (index <= 8);
    group->unk00 = 0x3E8;
    return group;
}
END_NONMATCH

ASM_FUNC("asm/code_0815E288/obj_timer_e7c8.s", void obj_timer_e7c8(void))

ASM_FUNC("asm/code_0815E288/obj_create_group_e890.s", void obj_create_group_e890(void))

ASM_FUNC("asm/code_0815E288/obj_timer_ea04.s", void obj_timer_ea04(void))

ASM_FUNC("asm/code_0815E288/transition_setup.s", void transition_setup(void))

ASM_FUNC("asm/code_0815E288/transition_children_transition_step_b.s", void transition_children_transition_step_b(void))

s32 transition_spawn_child(struct Obj *obj, u8 *game)
{
    struct ClassVt *cls;
    void *block;
    s32 mode;

    if (obj->unk00 == 0)
    {
        mode = transition_scene_step(game);
        if (mode == 1)
        {
            cls = obj->klass04;
            cls->fn0C((u8 *)obj + cls->thisOffset, 3);
            *(void **)(game + 0x1E94) = transition_children_obj_init_scan_pool(heap_alloc2(0x1C, 1, gTransitionTagPhaz, 0), game);
        }
        else if (mode == 2)
        {
            cls = obj->klass04;
            cls->fn0C((u8 *)obj + cls->thisOffset, 3);
            block = heap_alloc2(0x18, 1, gTransitionTagPhaz, 0);
            obj_reset_e920(block);
            ((struct Obj *)block)->klass04 = (void *)gClassE440;
            *(void **)(game + 0x1E94) = block;
        }
    }
    return 1;
}

ASM_FUNC("asm/code_0815E288/transition_children_obj_init_scan_pool.s", void *transition_children_obj_init_scan_pool(void *obj, u8 *game))
ASM_FUNC("asm/code_0815E288/transition_children_transition_step_d.s", void transition_children_transition_step_d(void))

ASM_FUNC("asm/code_0815E288/transition_step_e.s", void transition_step_e(void))

void obj_init_e4a0(struct Obj *obj)
{
    obj->unk00 = 0;
    obj->klass04 = (void *)gClassE4a0;
    obj->unk08 = 0;
    obj->child0C = 0;
    obj->unk10 = (void *)0x7E00;
    obj->unk14 = 0;
}

void obj_reset_e920(struct Obj *obj)
{
    obj->klass04 = (void *)gReleaseVtableE920;
    obj->unk00 = 0;
}

void obj_free_slots_flag(struct Obj *obj, u32 flags)
{
    if (obj->unk00 != 0)
        release_obj_131c(obj->unk00);
    if (obj->klass04 != 0)
        release_obj_131c(obj->klass04);
    if (obj->unk08 != 0)
        release_obj_131c(obj->unk08);
    if (obj->child0C != 0)
        release_obj_131c(obj->child0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_free_ptr_fields_d(struct Obj9 *obj)
{
    void **p = &obj->unk04;

    if (obj->unk04 != 0)
        sprite_update_cd0(obj->unk04);
    if (p[1] != 0)
        sprite_update_cd0(p[1]);
    if (obj->unk0C != 0)
        sprite_update_cd0(obj->unk0C);
    if (obj->unk10 != 0)
        sprite_update_cd0(obj->unk10);
    if (obj->unk14 != 0)
        sprite_update_cd0(obj->unk14);
    if (obj->unk18 != 0)
        sprite_update_cd0(obj->unk18);
    if (obj->unk1C != 0)
        sprite_update_cd0(obj->unk1C);
    if (obj->unk20 != 0)
        sprite_update_cd0(obj->unk20);
    if (obj->unk24 != 0)
        sprite_update_cd0(obj->unk24);
}

void obj_release_children_9(struct Obj9 *obj, u32 flags)
{
    void **p = &obj->unk04;

    if (obj->unk04 != 0)
        release_obj_131c(obj->unk04);
    if (p[1] != 0)
        release_obj_131c(p[1]);
    if (obj->unk0C != 0)
        release_obj_131c(obj->unk0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (obj->unk14 != 0)
        release_obj_131c(obj->unk14);
    if (obj->unk18 != 0)
        release_obj_131c(obj->unk18);
    if (obj->unk1C != 0)
        release_obj_131c(obj->unk1C);
    if (obj->unk20 != 0)
        release_obj_131c(obj->unk20);
    if (obj->unk24 != 0)
        release_obj_131c(obj->unk24);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_free_ptr_fields_a(struct Obj *obj)
{
    void **p = &obj->klass04;

    sprite_update_cd0(obj->klass04);
    sprite_update_cd0(p[1]);
    sprite_update_cd0(obj->child0C);
    sprite_update_cd0(obj->unk10);
    sprite_update_cd0(obj->unk14);
}

void obj_release_children_5(struct Obj *obj, u32 flags)
{
    void **p = &obj->klass04;

    if (obj->klass04 != 0)
        release_obj_131c(obj->klass04);
    if (p[1] != 0)
        release_obj_131c(p[1]);
    if (obj->child0C != 0)
        release_obj_131c(obj->child0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (obj->unk14 != 0)
        release_obj_131c(obj->unk14);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_free_ptr_fields_b(struct Obj *obj)
{
    void **p = &obj->klass04;

    sprite_update_cd0(obj->klass04);
    sprite_update_cd0(p[1]);
    sprite_update_cd0(obj->child0C);
    sprite_update_cd0(obj->unk10);
    sprite_update_cd0(obj->unk14);
}

void obj_free_ptr_fields_c(struct Obj *obj, u32 flags)
{
    void **p = &obj->klass04;

    if (obj->klass04 != 0)
        release_obj_131c(obj->klass04);
    if (p[1] != 0)
        release_obj_131c(p[1]);
    if (obj->child0C != 0)
        release_obj_131c(obj->child0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (obj->unk14 != 0)
        release_obj_131c(obj->unk14);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_set_child_flag20(struct Obj *obj)
{
    ((u8 *)obj->child0C)[0x12] |= 0x20;
    obj->unk1A = 0;
}
