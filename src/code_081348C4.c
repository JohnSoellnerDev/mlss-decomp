#include "global.h"
#include "ram_routines.h"

void heap_free_b(void *p);
void release_obj_131c(void *p);
extern const u8 gVtDe08[];
extern const u8 gAppearSubClass[];
extern const u8 gAppearCallback8Class[];
extern const u8 gUnknownObjInitTableA[];
extern const u8 gUnknownObjInitTableB[];
extern const u16 gBldcntModeTable[];
extern const s32 gZoneTable[];

struct ListNode
{
    struct ListNode *prev;
    struct ListNode *next;
};

struct ObjList
{
    struct ListNode *head;
    struct ListNode *tail;
    u32 count;
    const void *vtable;
};

struct ObjCallback8
{
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u8 filler6[0x8 - 0x6];
    const void *vtable;
};

struct ObjRecord
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0xC - 0x04];
    u16 unk0C;
    u16 unk0E;
    u8 filler10[0x12 - 0x10];
    u8 flags12;
};

struct ItemObj
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0x21 - 0x04];
    u8 unk21;
};

struct AppearObj
{
    u8 filler0[0x8];
    struct ObjRecord *unk08;
    u8 filler0C[0x18 - 0x0C];
    s32 unk18;
    u8 filler1C[0x28 - 0x1C];
    void *unk28;
    u8 filler2C[0x38 - 0x2C];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x4C - 0x44];
    void (*unk4C)(struct AppearObj *obj);
    u8 filler50[0x79 - 0x50];
    u8 unk79;
    u16 unk7A;
    u16 unk7C;
    u32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    u8 filler90[0x92 - 0x90];
    u16 unk92;
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u16 unkAC;
};

struct AnimSub
{
    u32 unk00;
    u32 unk04;
    struct ObjRecord *unk08;
    struct ItemObj *unk0C;
    u32 unk10;
    u32 unk14;
    void *unk18;
    u8 unk1C;
    u8 unk1D;
    u16 unk1E;
    u16 unk20;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 filler25[0x28 - 0x25];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    const void *vtable;
};

void init_appear_obj_base(struct AnimSub *sub);
void list_drain(struct ObjList *list);
s8 blend_fade_step(struct ObjCallback8 *obj);
s8 blend_timer_done(struct ObjCallback8 *obj);
s32 lerp_step_081359D8(s32 from, s32 to, s32 steps, s32 step);
s32 is_state_busy(void);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void obj_release_slots(void *sprite);
void sprite_set_action(struct AppearObj *obj, u32 action, u32 arg, u32 extra);
void start_motion(struct AppearObj *obj);
void wait_for_index(struct AppearObj *obj);
void obj_appear_sprite(struct AppearObj *obj);
void part_fly_drop(struct AppearObj *obj);
void part_fade_3(struct AppearObj *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct AppearObj *owner);
void obj_track_target(u8 *target);
void set_slot_value(u32 id, s32 value);
u32 mt19937_next(void);
void anim_update_frame(struct AnimSub *sub);
void obj_zone_update(struct AnimSub *sub);
void anim_place_record(struct AnimSub *sub, s32 off_x, s32 off_y);
void sub_08135940(struct ObjCallback8 *obj, u16 a, u16 b);

void enter_wait_index_state(struct AppearObj *obj)
{
    if (obj->unk08->flags12 & 8)
    {
        sprite_set_action(obj, 0x10, 0, 0);
        obj->unk4C = wait_for_index;
    }
}

ASM_FUNC("asm/code_081348C4/part_setup_render.s", void part_setup_render(void))

void part_fade_2(struct AppearObj *obj)
{
    if (obj->unk08->flags12 & 8)
    {
        if ((s16)obj->unkAC < obj->unk9C)
        {
            obj->unk18 = obj->unk18 + 0x1000;
            sprite_set_action(obj, 0xC, 0, 0);
            set_slot_value(0xD5, -1);
            obj->unk4C = part_fade_3;
        }
        else
        {
            obj->unkAC = 0;
            sprite_set_action(obj, 0xE, 0, 0);
            obj->unk4C = part_fly_drop;
        }
    }
}

void part_fade_3(struct AppearObj *obj)
{
    obj->unk08->unk0C = obj->unk08->unk0C + 0x1400;
    obj->unkA8 = obj->unkA8 + 1;
    if (obj->unkA8 >= obj->unkA4)
    {
        obj->unkAC = obj->unkAC + 1;
        obj->unk08->unk0C = 0;
        obj->unkA8 = 0;
        obj->unk18 = obj->unk18 + 0xFFFFF000;
        obj->unk4C = part_fade_2;
        sprite_set_action(obj, 0xD, 0, 0);
    }
}

void part_drop_wait(struct AppearObj *obj)
{
    obj->unk18 = obj->unk18 + obj->unk8C;
    obj->unk8C = obj->unk8C - obj->unk94;
    if (obj->unk8C <= 0)
    {
        obj->unk18 = obj->unk18 + 0x1000;
        obj->unkA4 = 0xC;
        obj->unkA8 = 0;
        sprite_set_action(obj, 0xC, 0, 0);
        set_slot_value(0xD5, -1);
        obj->unk4C = part_fade_3;
    }
}

void obj_appear_sprite(struct AppearObj *obj)
{
    if (obj->unk08->flags12 & 8)
    {
        effect_spawn_at(0x2734, obj->unk38 / 0x100, obj->unk3C / 0x100 + 2, obj->unk40 / 0x100, obj);
        obj->unk8C = 0x800;
        obj->unk94 = 0x80;
        sprite_set_action(obj, 0xB, 0, 0);
        set_slot_value(0x69, -1);
        obj->unk4C = part_drop_wait;
    }
}

void obj_init_unindexed(struct AppearObj *obj)
{
    if (obj->unk80 == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->unk4C = obj_appear_sprite;
    }
}

void obj_start_motion_when_idle(struct AppearObj *obj)
{
    s32 busy = is_state_busy();
    s32 flags;
    if (busy == 0)
    {
        obj->unk84 = obj->unkA4 << 8;
        obj->unk88 = 0x6A00;
        obj->unk8C = obj->unk40;
        flags = obj->unk79;
        flags = flags | 0x20;
        obj->unk79 = flags;
        obj->unk7C = 0x280;
        obj->unk7A = busy;
        start_motion(obj);
        sprite_set_action(obj, 9, 0, 0);
        obj->unk4C = obj_init_unindexed;
    }
}

u32 obj_randomize_timer(struct AppearObj *obj)
{
    if (obj->unk9C == 0)
    {
        obj->unk9C = (mt19937_next() & 3) + 1;
        obj->unkA0 = 0x28;
        obj->unkA4 = 0x8C;
    }
    obj->unkAC = 0;
    obj->unk4C = obj_start_motion_when_idle;
    return 1;
}

ASM_FUNC("asm/code_081348C4/obj_pick_follow_mode.s", void obj_pick_follow_mode(void))

ASM_FUNC("asm/code_081348C4/appear_motion_obj_fall_step.s", void appear_motion_obj_fall_step(void))

ASM_FUNC("asm/code_081348C4/appear_motion_obj_orbit_step.s", void appear_motion_obj_orbit_step(void))

ASM_FUNC("asm/code_081348C4/obj_orbit_step_fixed.s", void obj_orbit_step_fixed(void))

ASM_FUNC("asm/code_081348C4/obj_step_toward_target_081350A0.s", void obj_step_toward_target_081350A0(void))

ASM_FUNC("asm/code_081348C4/sub_0813516C.s", void sub_0813516C(void))

u32 obj_step_accum_clamp(struct AppearObj *obj)
{
    s16 accum;
    s32 height;

    obj->unk92 = obj->unk92 + obj->unk94;
    accum = obj->unk92;
    height = obj->unk40 + accum;
    obj->unk18 = height;
    if (obj->unk98 < height)
    {
        return 0;
    }
    obj->unk18 = obj->unk98;
    obj->unk94 = 0;
    return 1;
}

void sub_08135290(struct AppearObj *obj)
{
    u8 *p;

    p = obj->unk28;
    if (*(s16 *)(p + 0xEC) == -1)
    {
        if (*(s16 *)(p + 0x226) != 0)
        {
            obj_track_target(p + 0x138);
        }
    }
    else
    {
        if (*(s16 *)(p - 0x3A) != 0)
        {
            obj_track_target(p - 0x128);
        }
    }
}

ASM_FUNC("asm/code_081348C4/item_attach_sprite_8.s", void item_attach_sprite_8(void))

ASM_FUNC("asm/code_081348C4/anim_update_frame.s", void anim_update_frame(struct AnimSub *sub))

ASM_FUNC("asm/code_081348C4/anim_place_record.s", void anim_place_record(struct AnimSub *sub, s32 off_x, s32 off_y))

ASM_FUNC("asm/code_081348C4/scene_obj_list_pass.s", void scene_obj_list_pass(void))

u32 list_get_head_tail_count(struct ObjList *list)
{
    return list->count;
}

struct ListNode *sub_081354D0(struct ObjList *list)
{
    return list->tail;
}

struct ListNode *sub_081354D4(struct ObjList *list)
{
    return list->head;
}

u32 list_contains_node(struct ObjList *list, struct ListNode *node)
{
    struct ListNode *cur = list->head;
    while (cur != NULL)
    {
        if (cur == node)
        {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

void list_drain(struct ObjList *list)
{
    struct ListNode *node;

    while (list->head != NULL)
    {
        node = list->head;
        if (list_contains_node(list, node))
        {
            if (node->prev != NULL)
            {
                if (node->next != NULL)
                {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
                else
                {
                    node->prev->next = NULL;
                    list->tail = node->prev;
                }
            }
            else
            {
                if (node->next != NULL)
                {
                    node->next->prev = NULL;
                    list->head = node->next;
                }
                else
                {
                    list->tail = NULL;
                    list->head = NULL;
                }
            }
            list->count--;
        }
    }
}

void list_remove_node(struct ObjList *list, struct ListNode *node)
{
    if (list_contains_node(list, node))
    {
        if (node->prev != NULL)
        {
            if (node->next != NULL)
            {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            else
            {
                node->prev->next = NULL;
                list->tail = node->prev;
            }
        }
        else
        {
            if (node->next != NULL)
            {
                node->next->prev = NULL;
                list->head = node->next;
            }
            else
            {
                list->tail = NULL;
                list->head = NULL;
            }
        }
        list->count--;
    }
}

void sub_08135590(struct ObjList *list, struct ListNode *node)
{
    if (!list_contains_node(list, node))
    {
        if (list->head == NULL)
        {
            node->next = NULL;
            node->prev = NULL;
            list->tail = node;
            list->head = node;
        }
        else
        {
            list->tail->next = node;
            node->prev = list->tail;
            node->next = NULL;
            list->tail = node;
        }
        list->count++;
    }
}

void obj_list_drain_free(struct ObjList *obj, u32 flags)
{
    obj->vtable = gVtDe08;
    list_drain(obj);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void init_obj_list_vt_de08(struct ObjList *list)
{
    list->vtable = gVtDe08;
    list->tail = NULL;
    list->head = NULL;
    list->count = 0;
}

u32 obj_is_idle_08135604(struct AnimSub *sub)
{
    if (sub->unk18 == NULL)
    {
        return 1;
    }
    if (sub->unk22 == 0)
    {
        return 1;
    }
    return 0;
}

ASM_FUNC("asm/code_081348C4/obj_zone_update.s", void obj_zone_update(struct AnimSub *sub))

u32 anim_refresh(struct AnimSub *sub)
{
    anim_update_frame(sub);
    obj_zone_update(sub);
    anim_place_record(sub, 0, 0);
    return 0;
}

u32 anim_refresh_entry(struct AnimSub *sub)
{
    return anim_refresh(sub);
}

u8 obj_field_accessors_10_30(struct AnimSub *sub)
{
    return sub->unk1C;
}

void sub_081356D4(struct AnimSub *sub, u32 x, u32 y)
{
    sub->unk38 = x;
    sub->unk3C = y;
}

void sub_081356DC(struct AnimSub *sub, u8 v)
{
    sub->unk1C = v;
}

u32 sub_081356E0(struct AnimSub *sub)
{
    return sub->unk2C + sub->unk30;
}

u32 sub_081356EC(struct AnimSub *sub)
{
    return sub->unk30;
}

u32 sub_081356F0(struct AnimSub *sub)
{
    return sub->unk2C;
}

u32 sub_081356F4(struct AnimSub *sub)
{
    return sub->unk28;
}

void sub_081356F8(struct AnimSub *sub, u32 *out)
{
    out[0] = sub->unk28;
    out[1] = sub->unk2C;
    out[2] = sub->unk30;
}

void sub_08135708(struct AnimSub *sub, u32 a, u32 b, u32 c)
{
    sub->unk28 = a;
    sub->unk2C = b;
    sub->unk30 = c;
}

u16 sub_08135710(struct AnimSub *sub)
{
    return sub->unk1E;
}

u32 sub_08135714(struct AnimSub *sub)
{
    return sub->unk1E >> 8;
}

void sub_0813571C(struct AnimSub *sub, u16 v)
{
    sub->unk1E = v;
}

void sub_08135720(struct AnimSub *sub, u32 v)
{
    sub->unk1E = v << 8;
}

void sub_08135728(struct AnimSub *sub, u32 v)
{
    sub->unk10 = v;
}

void sub_0813572C(struct AnimSub *sub, u32 v)
{
    sub->unk14 = v;
}

void release_item_c(struct AnimSub *sub)
{
    if (sub->unk0C != NULL)
    {
        release_obj_131c(sub->unk0C);
        sub->unk0C = NULL;
    }
}

void release_item_8(struct AnimSub *sub)
{
    if (sub->unk08 != NULL)
    {
        release_obj_131c(sub->unk08);
        sub->unk08 = NULL;
    }
}

ASM_FUNC("asm/code_081348C4/item_attach_sprite_c.s", void item_attach_sprite_c(void))

void init_appear_obj_base(struct AnimSub *sub)
{
    sub->unk00 = 0;
    sub->unk04 = 0;
    sub->unk08 = NULL;
    sub->unk0C = NULL;
    sub->unk10 = (u32)gUnknownObjInitTableA;
    sub->unk14 = (u32)gUnknownObjInitTableB;
    sub->unk18 = NULL;
    sub->unk20 = 0;
    sub->unk1E = 0;
    sub->unk1D = 0xFF;
    sub->unk1C = -1;
    sub->unk24 = 0;
    sub->unk28 = 0;
    sub->unk2C = 0;
    sub->unk30 = 0;
    sub->unk34 = 0;
    sub->unk22 = 0;
    sub->unk23 = 0;
}

void release_items_free(struct AnimSub *sub, u32 flags)
{
    sub->vtable = gAppearSubClass;
    release_item_8(sub);
    release_item_c(sub);
    if (flags & 1)
    {
        heap_free_b(sub);
    }
}

ASM_FUNC("asm/code_081348C4/init_appear_sub_item.s", void init_appear_sub_item(void))

struct AnimSub *init_appear_sub_base(struct AnimSub *sub)
{
    sub->vtable = gAppearSubClass;
    init_appear_obj_base(sub);
    return sub;
}

s8 blend_fade_step(struct ObjCallback8 *obj)
{
    u16 level;

    level = 0x10;
    if ((s8)blend_timer_done(obj) != 0)
    {
        return 1;
    }
    obj->unk2 = obj->unk2 + 1;
    switch (obj->unk0)
    {
    case 0:
    case 2:
        level = lerp_step_081359D8(0x10, 0, obj->unk4, obj->unk2);
        break;
    case 1:
    case 3:
        level = lerp_step_081359D8(0, 0x10, obj->unk4, obj->unk2);
        break;
    }
    *(u16 *)0x04000050 = gBldcntModeTable[obj->unk0];
    *(u16 *)0x04000054 = level;
    return (s8)blend_timer_done(obj);
}

s8 blend_timer_done(struct ObjCallback8 *obj)
{
    if (obj->unk2 >= obj->unk4)
    {
        return 1;
    }
    return 0;
}

void sub_08135940(struct ObjCallback8 *obj, u16 a, u16 b)
{
    if (a > 3)
    {
        a = 3;
    }
    obj->unk0 = a;
    obj->unk2 = 0;
    obj->unk4 = b;
    if (b == 0)
    {
        obj->unk4 = 1;
        blend_fade_step(obj);
    }
}

void sub_0813596C(struct ObjCallback8 *obj)
{
    obj->unk0 = 0;
    obj->unk2 = 0;
    obj->unk4 = 0x10;
}

void obj_callback_8_free(struct ObjCallback8 *obj, u32 flags)
{
    obj->vtable = gAppearCallback8Class;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct ObjCallback8 *init_obj_callback_8_timer(struct ObjCallback8 *obj, u16 a, u16 b)
{
    obj->vtable = gAppearCallback8Class;
    sub_08135940(obj, a, b);
    return obj;
}

struct ObjCallback8 *init_obj_callback_8(struct ObjCallback8 *obj)
{
    obj->vtable = gAppearCallback8Class;
    sub_0813596C(obj);
    return obj;
}

s32 lerp_step_081359D8(s32 from, s32 to, s32 steps, s32 step)
{
    if (steps <= 0)
    {
        steps = 1;
    }
    if (step < 0)
    {
        step = 0;
    }
    if (step >= steps)
    {
        step = steps;
    }
    return from + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((to - from) * step, steps);
}

ASM_FUNC("asm/code_081348C4/interp_cubic_steps.s", void interp_cubic_steps(void))

ASM_FUNC("asm/code_081348C4/mat4_mul.s", void mat4_mul(void))

ASM_FUNC("asm/code_081348C4/mat4_apply_scale.s", void mat4_apply_scale(void))

ASM_FUNC("asm/code_081348C4/mat4_apply_rotate_zxy.s", void mat4_apply_rotate_zxy(void))

ASM_FUNC("asm/code_081348C4/mat4_apply_rotate_yxz.s", void mat4_apply_rotate_yxz(void))
