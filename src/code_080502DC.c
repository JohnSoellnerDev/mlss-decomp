#include "global.h"

struct VtSlot
{
    s16 offset;
    u16 filler2;
    u32 (*fn)();
};

struct ObjVtable
{
    u8 filler0[8];
    struct VtSlot slot8;
    struct VtSlot slot10;
    struct VtSlot slot18;
    s16 thisOffset;
    u16 filler22;
    void (*update)(void *);
    struct VtSlot slot28;
    struct VtSlot slot30;
    u8 filler38[8];
    struct VtSlot slot40;
    struct VtSlot slot48;
    struct VtSlot slot50;
    struct VtSlot slot58;
    struct VtSlot slot60;
    struct VtSlot slot68;
    struct VtSlot slot70;
    u8 filler78[0x10];
    struct VtSlot slot88;
};

struct ChildObj
{
    u8 filler0[0x22C];
    u32 unk22C_0 : 10;
    u32 unk22C_10 : 12;
    u32 unk22C_22 : 10;
    u8 filler230[0x338 - 0x230];
    struct ObjVtable *vtable;
    u8 filler33C[2];
    u8 flags;
    u8 filler33F[0x344 - 0x33F];
    struct ChildObj *child;
};

struct SpriteObj
{
    s16 posX;
    s16 posY;
    u8 filler4[0xA];
    u16 unkE;
    u8 filler10[0x1F - 0x10];
    u8 flags1F;
};

struct MainObj
{
    u8 filler0[0x0C];
    s32 x;
    s32 y;
    u8 filler14[0x20F - 0x14];
    u8 flags20F;
    u8 filler210[0x214 - 0x210];
    u32 unk214_0 : 13;
    u32 unk214_13 : 4;
    u32 unk214_17 : 15;
    u8 filler218[0x23D - 0x218];
    u8 unk23D;
    u8 filler23E[0x294 - 0x23E];
    struct SpriteObj *sprite;
};

struct ListHeadX
{
    u8 filler0[0x3C];
    u32 head3C;
};

struct Bit0
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct Bit1
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct ObjTimer
{
    u8 filler0[0x23B];
    u8 unk23B;
    u8 filler23C[0x33E - 0x23C];
    u16 unk33E;
    s16 timer340;
};

struct HookObj
{
    u8 filler0[0x198];
    void (*hook198)();
    u8 filler19C[4];
    void (*hook1A0)();
};

struct WinObj
{
    u8 filler0[0x1C];
    s16 unk1C;
    s16 unk1E;
    u16 unk20;
    u16 unk22;
    u8 filler24[2];
    u16 unk26;
    u16 unk28;
    u8 filler2A[2];
    u32 sub2C;
    u32 sub30;
    u32 sub34;
    u32 sub38;
    u8 *frame3C;
};

struct FramePos
{
    u16 x;
    u16 y;
};

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

extern u8 *gUnk_03000FC0;
void set_flag(u8 *flags, u32 a, u32 b, u32 c);
void reset_record_fields(u8 *obj);
void obj_random_pick(u32 r0, u8 *obj, u32 *state);
void obj_bind_callbacks(u32 arg, u8 *obj);
void obj_call_hook_1a0(u32 a0, u8 *obj);
void obj_create_sub(u8 *obj, u32 a, u32 b, u32 d);
void obj_copy_sub_fields(u8 *obj);
void obj_refresh_dirty(u8 *obj);
void obj_refresh_motion_state(u8 *obj, u32 a);
void obj_update_screen_pos(u8 *obj, u32 a, u32 b, u32 d);
void obj_update_cell_levels(u8 *obj);
u32 obj_emit_entry(u8 *obj, u32 a, u32 b);
void obj_reset_link_fields(u8 *obj);
void obj_resolve_contacts(u8 *obj, u32 a, u32 b, u32 d);
u32 obj_step_toward_goal(u8 *obj);
void obj_update_link_pos(u8 *obj, u32 a, u32 b);
void obj_step_sequence(u8 *obj);
void obj_sync_sprite_link(u8 *obj);
void obj_queue_update(struct SpriteObj *sprite);
void obj_list_pass_0a8c(void);
void obj_init(u8 *obj, u32 a, u32 b);
void init_object_from_def(u32 d, u8 *child, u8 *obj);
void heap_free_b(void *p);
u32 mt19937_next(void);
s32 sub_081DE850(s32 range, u32 seed);
void wander_to_edge(u8 *obj, u32 a, s32 b);
u32 angle_of_vector16(s32 x, s32 y);
void obj_random_enter(u32 owner, u8 *obj, u32 *state);
void obj_release_slots(u32 sprite);
void sprite_update_cd0(u32 sprite);
u8 step_pos_xy_c(u8 *obj);
u32 zone_overlap_code(u32 owner, u8 *obj, u32 c);
void tick_store(void);

extern const u8 gObjBaseClass[];
extern const u8 gObjForwardChildClass[];
extern const u8 gObjAttachedChildClass[];
extern const u8 gObjFlagsClass[];
extern const u8 gObjStateClass[];

extern u8 gUnk_0300034C[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];

ASM_FUNC("asm/code_080502DC/obj_set_mode_4.s", void obj_set_mode_4(void))

ASM_FUNC("asm/code_080502DC/obj_update_flags_from_child.s", void obj_update_flags_from_child(void))

u32 obj_attach_child(u8 *obj, u32 a, u32 b, u32 d, u8 *child)
{
    u8 d8;

    d8 = d;
    obj_init(obj, a, b);
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjAttachedChildClass;
    init_object_from_def(d8, child, obj);
    ((struct Bit0 *)(obj + 0x20C))->bit0 = 0;
    ((struct Bit1 *)(obj + 0x20F))->bit1 = 0;
    ((struct ChildObj *)obj)->child = (struct ChildObj *)child;
    return (u32)obj;
}

void forward_child_70(u8 *obj, u32 a, u32 b)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u8 b8;

    b8 = b;
    obj_update_link_pos(obj, a, b8);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x70);
        entry->fn((u8 *)child + entry->offset, a, b8);
    }
}

u32 forward_child_68(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u32 result;

    result = obj_step_toward_goal(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x68);
        result = entry->fn((u8 *)child + entry->offset);
    }
    return result;
}

void forward_child_60(u8 *obj, u32 a, u32 b, u32 d)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u8 d8;

    d8 = d;
    obj_resolve_contacts(obj, a, b, d8);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x60);
        entry->fn((u8 *)child + entry->offset, a, b, d8);
    }
}

void forward_child_58(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;

    obj_reset_link_fields(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x58);
        entry->fn((u8 *)child + entry->offset);
    }
}

u32 forward_child_50(u8 *obj, u32 a, u32 b)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u32 result;

    result = obj_emit_entry(obj, a, b);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x50);
        result = entry->fn((u8 *)child + entry->offset, a, b);
    }
    return result;
}

void forward_child_88(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;

    obj_step_sequence(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x88);
        entry->fn((u8 *)child + entry->offset);
    }
}

void forward_child_48(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;

    obj_update_cell_levels(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x48);
        entry->fn((u8 *)child + entry->offset);
    }
}

void forward_child_40(u8 *obj, u32 a, u32 b, u32 d)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    s32 x;
    s32 y;

    x = (s16)a;
    y = (s16)b;
    obj_update_screen_pos(obj, x, y, d);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x40);
        entry->fn((u8 *)child + entry->offset, x, y, d);
    }
}

void forward_child_flag_20(u8 *obj)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_sync_sprite_link(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        if (((u8 *)child)[0x20C] & 1)
        {
            vt = child->vtable;
            vt->update((u8 *)child + vt->thisOffset);
        }
    }
}

void forward_child_28(u8 *obj)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_refresh_dirty(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot28.fn((u8 *)child + vt->slot28.offset);
    }
}

void forward_child_30(u8 *obj, u32 a)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_refresh_motion_state(obj, a);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot30.fn((u8 *)child + vt->slot30.offset, a);
    }
}

void forward_child_18(u8 *obj)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_copy_sub_fields(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot18.fn((u8 *)child + vt->slot18.offset);
    }
}

void forward_child_10(u8 *obj, u32 a, u32 b, u32 d)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_create_sub(obj, a, b, d);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot10.fn((u8 *)child + vt->slot10.offset, a, b, d);
    }
}

void obj_update_child(struct ChildObj *obj)
{
    struct ObjVtable *vtable;

    set_flag(gUnk_03000FC0 + 0x38C, 0, obj->unk22C_10 + 0x960, 0);
    if (obj->flags & 0x10)
    {
        ((u8 *)obj->child)[0x20C] |= 1;
        vtable = obj->vtable;
        vtable->update((u8 *)obj + vtable->thisOffset);
    }
}

NONMATCH("asm/code_080502DC/obj_teardown.s", void obj_teardown(u8 *obj, u32 flags))
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjForwardChildClass;
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot8.fn((u8 *)child + vt->slot8.offset, 3);
    }
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjBaseClass;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080502DC/obj_init_state.s", void obj_init_state(void))

ASM_FUNC("asm/code_080502DC/obj_init_from_entry.s", void obj_init_from_entry(void))

ASM_FUNC("asm/code_080502DC/obj_replace_sprite.s", void obj_replace_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_apply_pending_sprite.s", void obj_apply_pending_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_apply_anim_code.s", void obj_apply_anim_code(void))

ASM_FUNC("asm/code_080502DC/obj_activate_sprite.s", void obj_activate_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_commit_sprite.s", void obj_commit_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_step_anim.s", void obj_step_anim(void))

NONMATCH("asm/code_080502DC/obj_clear_mask_if_timer.s", void obj_clear_mask_if_timer(u8 *obj))
{
    struct ObjTimer *o;
    int f;

    o = (struct ObjTimer *)obj;
    if (o->timer340 != 0)
    {
        o->unk23B = 0;
        f = 0xFFFFFE01;
        f = f & o->unk33E;
        o->unk33E = f;
    }
}
END_NONMATCH

void script_init_obj_refresh_sprite(u8 *obj)
{
    struct MainObj *o;
    int f;

    o = (struct MainObj *)obj;
    f = o->flags20F;
    f = f | 2;
    o->flags20F = f;
    obj_queue_update(o->sprite);
    o->unk23D = 7;
}

ASM_FUNC("asm/code_080502DC/obj_init_flags.s", void obj_init_flags(void))

void script_init_obj_sync_sprite_pos(u8 *obj)
{
    if (((struct MainObj *)obj)->flags20F & 2)
    {
        ((struct MainObj *)obj)->sprite->posX = ((struct MainObj *)obj)->x / 0x100;
        ((struct MainObj *)obj)->sprite->posY = ((struct MainObj *)obj)->y / 0x100;
        ((struct MainObj *)obj)->sprite->unkE = ((struct MainObj *)obj)->unk214_13 << 12;
    }
}

NONMATCH("asm/code_080502DC/script_init_obj_setup_sprite.s", void script_init_obj_setup_sprite(u8 *obj, u32 a, u32 b, u32 d))
{
    struct SpriteObj *sprite;
    int f;

    obj_create_sub(obj, a, b, d);
    sprite = ((struct MainObj *)obj)->sprite;
    f = -13;
    f = f & sprite->flags1F;
    f = f & -4;
    sprite->flags1F = f;
}
END_NONMATCH

void stub_nop_080514BE(void)
{
}

void sub_080514C4(void)
{
}

void sub_080514C8(void)
{
}

void sub_080514CC(void)
{
}

void sub_080514D0(void)
{
}

void sub_080514D4(void)
{
}

NONMATCH("asm/code_080502DC/obj_random_pick.s", void obj_random_pick(u32 r0, u8 *obj, u32 *state))
{
    s16 *slot;
    u32 idx;
    u32 entry;
    s16 value;
    u16 shade;

    slot = (s16 *)(obj + 0x100);
    idx = sub_081DE850(2, mt19937_next());
    entry = ((u16 *)(obj + 0x130))[idx];
    idx = sub_081DE850(4, mt19937_next());
    slot += idx;
    value = *slot;
    wander_to_edge(obj, entry, value);
    if (*(u16 *)(obj + 0x242) | *(u16 *)(obj + 0x244))
    {
        shade = ((u16)angle_of_vector16(*(s16 *)(obj + 0x242), *(s16 *)(obj + 0x244)) + 0x10) >> 8;
    }
    else
    {
        shade = 0xFF;
    }
    obj[2] = shade;
    ((struct Mode3 *)(obj + 0x214))->mode = 3;
    *state = (u32)obj_random_enter;
}
END_NONMATCH

NONMATCH("asm/code_080502DC/obj_random_enter.s", void obj_random_enter(u32 owner, u8 *obj, u32 *state))
{
    u8 flag;
    u32 idx;

    flag = step_pos_xy_c(obj);
    if ((u8)zone_overlap_code(owner, obj, 0) != 0)
    {
        ((struct Mode3 *)(obj + 0x214))->mode = 4;
    }
    else if (flag == 0)
    {
        idx = sub_081DE850(4, mt19937_next());
        *(u16 *)(obj + 0x1D2) = ((u16 *)(obj + 0x128))[idx];
        *(u16 *)(obj + 0x244) = flag;
        *(u16 *)(obj + 0x242) = flag;
        *state = (u32)tick_store;
        *(u32 *)((u8 *)state + 0xC) = (u32)obj_random_pick;
    }
}
END_NONMATCH

void obj_bind_callbacks(u32 arg, u8 *obj)
{
    reset_record_fields(obj);
    ((struct HookObj *)obj)->hook198 = obj_call_hook_1a0;
    ((struct HookObj *)obj)->hook1A0 = obj_random_pick;
}

void obj_bind_callbacks_entry(u32 arg, u8 *obj)
{
    obj_bind_callbacks(arg, obj);
}

NONMATCH("asm/code_080502DC/obj_call_hook_1a0.s", void obj_call_hook_1a0(u32 a0, u8 *obj))
{
    void (*hook)(u32, u8 *);

    hook = ((struct HookObj *)obj)->hook1A0;
    hook(a0, obj);
}
END_NONMATCH

ASM_FUNC("asm/code_080502DC/window_create.s", void window_create(void))

ASM_FUNC("asm/code_080502DC/window_state_step.s", void window_state_step(void))

void scene_update_sprites(u8 *obj)
{
    struct WinObj *w;

    w = (struct WinObj *)obj;
    ((struct FramePos *)w->frame3C)->x = w->unk26 - w->unk1C;
    ((struct FramePos *)w->frame3C)->y = w->unk28 - w->unk1E;
    if (w->unk1E == 0)
    {
        obj_release_slots(w->sub2C);
    }
    else
    {
        sprite_update_cd0(w->sub2C);
    }
    if (w->unk1C == w->unk20 - 0xF0)
    {
        obj_release_slots(w->sub30);
    }
    else
    {
        sprite_update_cd0(w->sub30);
    }
    if (w->unk1E == w->unk22 - 0xA0)
    {
        obj_release_slots(w->sub34);
    }
    else
    {
        sprite_update_cd0(w->sub34);
    }
    if (w->unk1C == 0)
    {
        obj_release_slots(w->sub38);
    }
    else
    {
        sprite_update_cd0(w->sub38);
    }
}

void obj_emit_oam(void)
{
    u8 *base;
    u32 *oam_slot;
    struct ListHeadX *list;
    u32 old;

    base = (u8 *)&gUnk_0300034C;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHeadX *)gUnk_0203FFB8)->head3C,
        ((struct ListHeadX *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHeadX *)gUnk_0203FFB8;
    old = ((u32 (*)(u32, u32))gUnk_03000D4C)(list->head3C, old);
    *oam_slot = old;
}
