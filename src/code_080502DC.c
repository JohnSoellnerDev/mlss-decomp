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
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u32 sub2C;
    u32 sub30;
    u32 sub34;
    u32 sub38;
    u8 *frame3C;
    u8 *unk40;
    u8 *unk44;
    u8 *unk48;
};

struct FramePos
{
    u16 x;
    u16 y;
};

struct Lo4Bits
{
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 rest : 4;
};

struct Mid8Bits
{
    u16 b0 : 1;
    u16 mid : 8;
    u16 rest : 7;
};

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

struct Mid2At1
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct SpriteFlags
{
    u8 filler0[0x1F];
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 b45 : 2;
    u8 b67 : 2;
};

struct Bits8
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct Word20C
{
    u32 lo15 : 15;
    u32 mv : 3;
    u32 rest : 14;
};

struct Bits33C
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 mid3 : 3;
    u8 rest : 2;
};

struct Mid4At6
{
    u16 lo6 : 6;
    u16 mid4 : 4;
    u16 rest : 6;
};

struct ObjInitS
{
    u8 filler0[0x23B];
    u8 unk23B;
    u8 filler23C[0x338 - 0x23C];
    struct ObjVtable *vtable;
    u8 flags;
    u8 filler33D;
    u16 mask;
    s16 timer340;
    u16 timerB342;
    s16 unk344;
    u32 unk348;
    u32 unk34C;
};

extern u8 *gUnk_03000FC0;
extern const u16 gSpriteSelectAnimTable[];
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
s32 rand_below(s32 range, u32 seed);
void wander_to_edge(u8 *obj, u32 a, s32 b);
u32 angle_of_vector16(s32 x, s32 y);
void obj_random_enter(u32 owner, u8 *obj, u32 *state);
void obj_release_slots(u32 sprite);
void sprite_update_cd0(u32 sprite);
u8 step_pos_xy_c(u8 *obj);
u32 zone_overlap_code(u32 owner, u8 *obj, u32 c);
void tick_store(void);
void unlink_d54(void *ctl);
void obj_load_record(void *rec, u32 a, u32 b, u32 c, u32 d);

extern const u8 gObjBaseClass[];
extern const u8 gObjForwardChildClass[];
extern const u8 gObjAttachedChildClass[];
extern const u8 gObjFlagsClass[];
extern const u8 gObjStateClass[];
u8 test_flag(u8 *table, u8 *tableB, u16 id);

extern u8 gSys[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];

NONMATCH("asm/code_080502DC/obj_set_mode_4.s", void obj_set_mode_4(u8 *obj))
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u32 bit;
    u32 mid;

    ((struct Bit0 *)(obj + 0x20C))->bit0 = 0;
    ((struct Mid2At1 *)(obj + 0x33C))->mode = 2;
    if ((obj[0x33E] & 3) != 0)
    {
        ((struct Lo4Bits *)(obj + 0x33E))->lo2 = 0;
        *(u32 *)(obj + 0x18) = 0;
        bit = ((u32)obj[0x33E] << 29) >> 31;
        ((struct Bits8 *)(obj + 0x20D))->b5 = bit;
        obj[2] = obj[0x33F];
        obj[0x23A] = 0;
        mid = ((u32)obj[0x33C] << 25) >> 29;
        ((struct Mode3 *)(obj + 0x33D))->mode = mid;
        ((struct Bits8 *)(obj + 0x33C))->b3 = 0;
    }
    entry = (struct VtSlot *)((struct ChildObj *)obj)->vtable;
    entry = (struct VtSlot *)((u8 *)entry + 0x80);
    entry->fn(obj + entry->offset, 1);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL && (((u8 *)child)[0x20F] & 2))
    {
        ((struct Bit0 *)((u8 *)child + 0x20C))->bit0 = 0;
        child = ((struct ChildObj *)obj)->child;
        entry = (struct VtSlot *)child->vtable;
        entry = (struct VtSlot *)((u8 *)entry + 0x80);
        entry->fn((u8 *)child + entry->offset, 1);
    }
}
END_NONMATCH

NONMATCH("asm/code_080502DC/obj_update_flags_from_child.s", void obj_update_flags_from_child(u8 *obj))
{
    u8 **childSlot;
    u8 *child;
    u32 link;
    u32 t;

    link = obj[0x33C] & 6;
    if (link != 0)
    {
        if ((*(u16 *)(obj + 0x212) & 0x180) == 0)
        {
            if (link == 4)
            {
                ((struct Bit0 *)(obj + 0x20C))->bit0 = 1;
                childSlot = (u8 **)(obj + 0x344);
                child = *childSlot;
                if (child != NULL)
                {
                    if (child[0x20F] & 2)
                    {
                        ((struct Bit0 *)(child + 0x20C))->bit0 = 1;
                        child = *childSlot;
                        t = child[0x33C];
                        if (2 & t)
                        {
                            t = t << 27;
                            ((struct Mode3 *)(child + 0x214))->mode = t >> 29;
                            child = *childSlot;
                            ((struct Bit1 *)(child + 0x33C))->bit1 = 0;
                        }
                    }
                }
                if (obj[0x33C] & 0x80)
                {
                    t = obj[0x33D];
                    t = t << 29;
                    ((struct Mode3 *)(obj + 0x214))->mode = t >> 29;
                    ((struct Bits8 *)(obj + 0x33C))->b7 = 0;
                }
                ((struct Bit0 *)(obj + 0x33C))->bit0 = 1;
            }
            ((struct Mid2At1 *)(obj + 0x33C))->mode = 0;
        }
    }
}
END_NONMATCH

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

static inline void obj_base_teardown(u8 *obj, u32 flags)
{
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjBaseClass;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_teardown(u8 *obj, u32 flags)
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
    obj_base_teardown(obj, flags);
}

u32 obj_init_state(u8 *obj, u32 a, u32 b)
{
    struct ObjInitS *o;
    u16 m;

    obj_init(obj, a, b);
    o = (struct ObjInitS *)obj;
    o->vtable = (struct ObjVtable *)gObjStateClass;
    ((struct Bits33C *)&o->flags)->b0 = 1;
    ((struct Bits33C *)&o->flags)->mid3 = 0;
    ((struct Bits33C *)&o->flags)->b1 = 0;
    ((struct Bits33C *)&o->flags)->b2 = 0;
    ((struct Mid4At6 *)&o->flags)->mid4 = 0;
    o->unk23B = 0;
    m = o->mask;
    o->mask = m | 0x1FE;
    o->timer340 = 1;
    o->timerB342 = 0xFFFF;
    o->unk344 = -1;
    o->unk348 = 0;
    o->unk34C = 0;
    return (u32)obj;
}

ASM_FUNC("asm/code_080502DC/obj_init_from_entry.s", void obj_init_from_entry(void))

ASM_FUNC("asm/code_080502DC/obj_replace_sprite.s", void obj_replace_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_apply_pending_sprite.s", void obj_apply_pending_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_apply_anim_code.s", void obj_apply_anim_code(void))

NONMATCH("asm/code_080502DC/obj_activate_sprite.s", void obj_activate_sprite(u8 *obj))
{
    u8 *flags;
    u8 f;
    u32 one;
    u32 k;
    struct SpriteObj **pslot;
    struct SpriteObj **cslot;
    struct SpriteObj *tmp;

    flags = obj + 0x33C;
    f = *flags;
    one = 1;
    if (f & one)
    {
        return;
    }
    f |= 1;
    *flags = f;
    pslot = (struct SpriteObj **)(obj + 0x34C);
    if (*pslot != NULL)
    {
        ((struct Bits33C *)flags)->mid3 = 0;
        cslot = (struct SpriteObj **)(obj + 0x294);
        unlink_d54(*cslot);
        obj_queue_update(*pslot);
        (*pslot)->unkE = (*cslot)->unkE;
        ((struct SpriteFlags *)*pslot)->lo2 = ((struct SpriteFlags *)*cslot)->lo2;
        ((struct SpriteFlags *)*pslot)->b23 = ((struct SpriteFlags *)*cslot)->b23;
        ((struct SpriteFlags *)*pslot)->b45 = ((struct SpriteFlags *)*cslot)->b45;
        ((struct SpriteFlags *)*pslot)->b67 = ((struct SpriteFlags *)*cslot)->b67;
        tmp = *pslot;
        *pslot = *cslot;
        *cslot = tmp;
        if (obj[0x20F] & 2)
        {
            return;
        }
        unlink_d54(tmp);
    }
    else
    {
        obj_load_record(*(void **)(obj + 0x294), gSpriteSelectAnimTable[(((u32)f << 26) >> 29) * 2 + 1], 1, 0, 0);
        set_flag(gUnk_03000FC0 + 0x38C, 0, (u16)(*(u16 *)(obj + 0x344) + 0x1360), 0);
        ((struct Mid4At6 *)flags)->mid4 = 0;
        obj[0x23B] = 0;
        k = 0x1FE;
        *(u16 *)(obj + 0x33E) = *(u16 *)(obj + 0x33E) | k;
        *(u16 *)(obj + 0x340) = one;
        *(u16 *)(obj + 0x342) = 0xFFFF;
    }
}
END_NONMATCH

void obj_commit_sprite(u8 *obj)
{
    struct SpriteObj **pslot;
    struct SpriteObj **cslot;
    struct SpriteObj *tmp;

    if ((obj[0x33C] & 1) == 0)
    {
        return;
    }
    ((struct Bit0 *)(obj + 0x33C))->bit0 = 0;
    ((struct ObjTimer *)obj)->unk23B = 0;
    pslot = (struct SpriteObj **)(obj + 0x34C);
    if (*pslot != NULL)
    {
        cslot = (struct SpriteObj **)(obj + 0x294);
        unlink_d54(*cslot);
        obj_queue_update(*pslot);
        (*pslot)->unkE = (*cslot)->unkE;
        ((struct SpriteFlags *)*pslot)->lo2 = ((struct SpriteFlags *)*cslot)->lo2;
        ((struct SpriteFlags *)*pslot)->b23 = ((struct SpriteFlags *)*cslot)->b23;
        ((struct SpriteFlags *)*pslot)->b45 = ((struct SpriteFlags *)*cslot)->b45;
        ((struct SpriteFlags *)*pslot)->b67 = ((struct SpriteFlags *)*cslot)->b67;
        tmp = *pslot;
        *pslot = *cslot;
        *cslot = tmp;
    }
    else
    {
        obj_load_record(*(void **)(obj + 0x294), 3, 0, 0, (u32)*pslot);
    }
}

extern const u16 gSpriteSelectAnimTable[];
u8 test_flag(u8 *table, u8 *tableB, u16 id);
void obj_commit_sprite(u8 *obj);
extern const u8 gObjAnimOffsetTable[];
extern const u8 gObjAnimWaveTable[];

NONMATCH("asm/code_080502DC/obj_step_anim.s", void obj_step_anim(u8 *obj, u32 index))
{
    u8 *flags;
    u16 *mask;
    u16 *counter;
    u32 m;
    u32 nv;
    u32 k;
    s32 step;
    u8 v;
    u8 r;

    flags = obj + 0x33C;
    if (*flags & 1)
    {
        mask = (u16 *)(obj + 0x33E);
        m = *mask;
        k = 0x1FE;
        if ((k & m) == k)
        {
            v = gObjAnimOffsetTable[(index >> 3) & 3];
            obj[0x23B] = v;
        }
        else
        {
            step = ((s32)m << 23) >> 24;
            if (step >= 0)
            {
                nv = (m & 0xFE01) | (((step + 1) & 0xFF) << 1);
                *mask = nv;
                v = gObjAnimWaveTable[((s32)nv << 23) >> 24];
                obj[0x23B] = v;
                if (v == 0)
                {
                    *mask = *mask | k;
                    if (*flags & 4)
                    {
                        if (*(s16 *)(obj + 0x342) == 0)
                        {
                            obj_commit_sprite(obj);
                        }
                    }
                }
            }
        }
        counter = (u16 *)(obj + 0x342);
        if ((s16)*counter > 0)
        {
            *counter = *counter - 1;
        }
    }
    else if (*flags & 4)
    {
        r = test_flag(gUnk_03000FC0 + 0x38C, 0, (u16)(*(u16 *)(obj + 0x344) + 0x1360));
        if (r == 0)
        {
            obj_load_record(*(void **)(obj + 0x294), gSpriteSelectAnimTable[((*flags >> 3) & 7) * 2 + 1], 0, 0, 0);
        }
    }
}
END_NONMATCH

void obj_clear_mask_if_timer(u8 *obj)
{
    struct ObjTimer *o;
    int f;

    o = (struct ObjTimer *)obj;
    if (o->timer340 != 0)
    {
        o->unk23B = 0;
        ((struct Mid8Bits *)&o->unk33E)->mid = 0;
    }
}

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

u32 obj_init_flags(u8 *obj, u32 a, u32 b)
{
    obj_init(obj, a, b);
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjFlagsClass;
    ((struct Bits8 *)(obj + 0x20C))->b5 = 0;
    ((struct Bits8 *)(obj + 0x20C))->b6 = 0;
    ((struct Bits8 *)(obj + 0x20C))->b7 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b0 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b1 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b3 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b4 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b6 = 0;
    ((struct Word20C *)(obj + 0x20C))->mv = 0;
    ((struct Bits8 *)(obj + 0x20E))->b2 = 1;
    ((struct Bits8 *)(obj + 0x20F))->b0 = 0;
    ((struct MainObj *)obj)->unk214_13 = 0;
    return (u32)obj;
}

void script_init_obj_sync_sprite_pos(u8 *obj)
{
    if (((struct MainObj *)obj)->flags20F & 2)
    {
        ((struct MainObj *)obj)->sprite->posX = ((struct MainObj *)obj)->x / 0x100;
        ((struct MainObj *)obj)->sprite->posY = ((struct MainObj *)obj)->y / 0x100;
        ((struct MainObj *)obj)->sprite->unkE = ((struct MainObj *)obj)->unk214_13 << 12;
    }
}

void script_init_obj_setup_sprite(u8 *obj, u32 a, u32 b, u32 d)
{
    struct SpriteObj *sprite;
    int f;

    obj_create_sub(obj, a, b, d);
    sprite = ((struct MainObj *)obj)->sprite;
    ((struct Lo4Bits *)&sprite->flags1F)->b23 = 0;
    ((struct Lo4Bits *)&sprite->flags1F)->lo2 = 0;
}

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
    u16 entry;
    u32 off;
    u8 *base;
    s16 value;
    u16 shade;
    u16 *pa;
    u16 *pb;
    u16 xa;

    slot = (s16 *)(obj + 0x100);
    off = rand_below(2, mt19937_next()) << 1;
    base = obj + 0x130;
    entry = *(u16 *)(base + off);
    slot += rand_below(4, mt19937_next());
    value = *slot;
    wander_to_edge(obj, entry, value);
    pa = (u16 *)(obj + 0x242);
    pb = (u16 *)(obj + 0x244);
    xa = *pa;
    if (xa | *pb)
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

void obj_random_enter(u32 owner, u8 *obj, u32 *state)
{
    u8 flag;
    u32 idx;
    u16 *tbl;
    u16 val;

    flag = step_pos_xy_c(obj);
    if ((u8)zone_overlap_code(owner, obj, 0) != 0)
    {
        ((struct Mode3 *)(obj + 0x214))->mode = 4;
    }
    else if (flag == 0)
    {
        idx = rand_below(4, mt19937_next()) * 2;
        tbl = (u16 *)(obj + 0x128);
        val = *(u16 *)((u8 *)tbl + idx);
        *(u16 *)(obj + 0x1D2) = val;
        *(u16 *)(obj + 0x244) = flag;
        *(u16 *)(obj + 0x242) = flag;
        *state = (u32)tick_store;
        *(u32 *)((u8 *)state + 0xC) = (u32)obj_random_pick;
    }
}

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

void obj_call_hook_1a0(u32 a0, u8 *obj)
{
    u8 *slot = obj + 0x1A0;
    void (*handler)(u32, u8 *, u8 *) = *(void (**)(u32, u8 *, u8 *))slot;

    handler(a0, obj, slot);
}

void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void task_insert(void *node, u8 kind, u32 data);
void set_flag_2_2(void *obj);
void *menu_obj_init(void *obj, u32 a, const void *table, u32 c);
void *map_obj_start(void *obj, u32 a, const void *table, u32 c, s32 x, s32 y, void *d);
void init_obj_09a8(u16 a, u8 b, u8 c, u8 d);
extern u8 gWindowStateClass[];
extern u8 gWindowSubBlockClass[];
extern u8 gWindowCreateBlockTag[];
extern u8 gWindowCreateBlock2Tag[];
extern u8 gWindowCreateSpriteTag[];
extern u16 gWindowSizeTable[];
struct WinObj *window_create(struct WinObj *w, u8 kind, u32 a, u16 id)
{
    u16 *entry;
    u8 *block;
    u8 *menu;
    u8 *map;
    u8 *base;

    task_insert(w, kind, a);
    *(u32 *)((u8 *)w + 0x18) = (u32)gWindowStateClass;
    *(u16 *)0x02000000 = 0;
    base = gSys;
    *(u32 *)(base + 0x884) = 0;
    w->unk2A = id;
    w->unk24 = 0x200;
    w->unk1C = 0;
    w->unk1E = 0;
    w->unk20 = 0;
    w->unk22 = 0;
    w->unk26 = 0x78;
    w->unk28 = 0x50;
    for (entry = gWindowSizeTable; entry[0] != 0xFFFF; entry += 4)
    {
        if (entry[0] == id)
        {
            w->unk24 = entry[1];
            w->unk26 = entry[2];
            w->unk28 = entry[3];
            break;
        }
    }
    block = heap_alloc2(0x1C, 0, gWindowCreateBlockTag, 1);
    task_insert(block, 8, (u32)gWindowCreateBlockTag);
    *(u32 *)(block + 0x18) = (u32)gWindowSubBlockClass;
    w->unk40 = block;
    *(struct WinObj **)(block + 0x14) = w;
    set_flag_2_2(block);
    menu = menu_obj_init(heap_alloc2(0x5C, 0, gWindowCreateBlock2Tag, 0), 8, gWindowCreateBlock2Tag, 8);
    w->unk48 = menu;
    *(struct WinObj **)(menu + 0x14) = w;
    set_flag_2_2(menu);
    map = map_obj_start(heap_alloc2(0x59C, 0, gWindowCreateSpriteTag, 0), 8, gWindowCreateSpriteTag, w->unk24, w->unk1C, w->unk1E, w->unk48);
    w->unk44 = map;
    *(struct WinObj **)(map + 0x14) = w;
    init_obj_09a8(0x15C0, 0, 1, 0x80);
    return w;
}

void scene_update_sprites(u8 *obj);
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

    base = (u8 *)&gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHeadX *)gUnk_0203FFB8)->head3C,
        ((struct ListHeadX *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHeadX *)gUnk_0203FFB8;
    old = ((u32 (*)(u32, u32))gUnk_03000D4C)(list->head3C, old);
    *oam_slot = old;
}
