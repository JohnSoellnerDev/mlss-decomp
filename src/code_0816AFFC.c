#include "global.h"
#include "ram_routines.h"

void heap_free_b(void *p);
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void obj_destroy_sprites(u8 *obj, u32 flags);
void obj_create_sprite_10(u8 *obj, u32 a, u32 b, u32 c, u32 d, u32 e);
void obj_set_fields_18(u8 *obj, s32 x, s32 y, s32 z);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void list_insert(void *list, void *node);
void obj_clear_class_flag50(u8 *obj);
void obj_place_children(u8 *obj, u32 flag);
void obj_script_step(u8 *obj);
typedef u32 (*MathFn2)(u32, u32);
typedef u32 (*MathFn3)(u32, u32, u32);
typedef u32 (*MathFn4)(u32, u32, u32, u32);
typedef u8 *(*InitBigFn)(void *, u32);
extern const u8 gHeapTagGroupChild[];
u8 obj_is_done(u8 *obj);
void obj_set_pos(void *obj, u32 x, u32 y);
void update_obj_sprite_pos(void *obj, u32 a, u32 b);
void obj_load_scaled_anims(u32 a, u32 b);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_setup_slot(void *obj, u32 a, u32 b, u32 c, s32 offset, ByteArg flag);
void obj_list_pass_0a8c(void);
void obj_list_unlink(u32 flags, void *obj);
void obj_init_big(u8 *obj, u32 a, u32 b, u32 c);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void set_slot_value(u32 id, s32 value);
extern const u8 gClassEb80[];
extern const u8 gClassEb98[];
extern const u8 gClassEba8[];
extern const u8 gClassEbb8[];
extern const u8 gClassEbc8[];
struct SysVars
{
    u8 filler0[0x2A];
    u16 keys2A;
};

extern struct SysVars gSys;
extern u8 *gUnk_03001030;
extern u32 (*gUnk_03000D48)(u32, u32);
extern u32 (*gUnk_03000D4C)(u32, u32);

struct ObjList
{
    u8 filler0[0x3C];
    u32 head3C;
};

extern struct ObjList gUnk_0203FFB8;

struct VtSlot8
{
    s16 offset;
    u16 filler2;
    void (*fn)(void *, u32);
};

struct ObjVtable
{
    u8 filler0[8];
    struct VtSlot8 slot8;
};

struct ChildObj
{
    u8 filler0[0xC];
    struct ObjVtable *vtable;
    u8 filler10[2];
    u8 flags12;
};

struct ObjChildren
{
    u8 filler0[0xC];
    struct ChildObj *child0C;
    struct ChildObj *child10;
};

struct ObjWithVt
{
    u8 filler0[0xC];
    struct ObjVtable *vtable;
    struct ChildObj *child10;
};

ASM_FUNC("asm/code_0816AFFC/child_states_obj_run_stage.s", void child_states_obj_run_stage(void))

ASM_FUNC("asm/code_0816AFFC/obj_run_phases.s", void obj_run_phases(void))

ASM_FUNC("asm/code_0816AFFC/obj_run_substates.s", void obj_run_substates(void))

struct ListElem
{
    u32 state;
    u8 filler4[4];
    struct ListElem *next;
    u8 filler0C[4];
    u8 *sprite;
};

void obj_update_children(u8 *obj)
{
    u8 *motion;
    u8 *link;
    s32 x;
    s32 y;
    s32 clamped;
    struct ListElem *elem;
    u32 *counter;
    u32 idx;
    s32 sine;
    u8 *slot;
    u8 *base;
    u8 **slotPtr;
    const s16 *sineTab;

    motion = *(u8 **)(obj + 0x20);
    x = (*(s32 *)(motion + 0x14) + 0x8000) / 2 - 0x7800;
    y = (*(s32 *)(motion + 0x1C) + 0x8000) / 2 - 0x5000;
    if (x > 0)
    {
        clamped = x;
        if (clamped > 0x1000)
            clamped = 0x1000;
    }
    else
        clamped = 0;
    x = clamped;
    if (y > 0)
    {
        if (y > 0x6000)
            y = 0x6000;
    }
    else
        y = 0;
    link = *(u8 **)(obj + 0x30);
    obj_set_pos(link, x, y);
    elem = *(struct ListElem **)*(u8 **)(obj + 0x1C);
    base = obj + 0x74;
    while (elem != NULL)
    {
        link = *(u8 **)(obj + 0x30);
        update_obj_sprite_pos(elem, *(u32 *)(link + 0x18), *(u32 *)(link + 0x1C));
        switch (elem->state)
        {
        case 0:
        case 1:
            *(u16 *)(elem->sprite + 0xE) = 0x40;
            break;
        case 7:
            *(u16 *)(elem->sprite + 0xE) = 0x20;
            break;
        case 2:
            *(u16 *)(elem->sprite + 0xE) = 0x80;
            break;
        case 3:
        case 4:
            *(u16 *)(elem->sprite + 0xE) = 0x80;
            break;
        case 5:
        case 6:
            *(u16 *)(elem->sprite + 0xE) = 0x10;
            break;
        }
        elem = elem->next;
    }
    if (*(u32 *)(obj + 0x490) & 8)
        obj_load_scaled_anims(*(u32 *)(obj + 0x6C), *(u32 *)(*(u8 **)(obj + 0x20) + 0x54));
    counter = (u32 *)(obj + 0x48C);
    *counter = (*counter + 1) & 0xFF;
    sineTab = gSineTable;
    idx = *counter & 0xFF;
    sine = sineTab[idx];
    if (sine < 0)
        sine += 0x3F;
    sine = (sine >> 10) + 0x10;
    slotPtr = (u8 **)&gUnk_03001030;
    if (sine > 1)
    {
        if (sine > 0x1F)
            sine = 0x1F;
        slot = base + sine * 4;
    }
    else
        slot = obj + 0x78;
    *slotPtr = slot;
}

void child_states_obj_list_pass_store(void)
{
    struct ObjList *list;
    u32 (**hook)(u32, u32);
    u8 *base;
    u32 head;

    obj_list_pass_0a8c();
    gUnk_03000D48(gUnk_0203FFB8.head3C, gUnk_0203FFB8.head3C + 0xA14);
    list = &gUnk_0203FFB8;
    hook = &gUnk_03000D4C;
    head = list->head3C;
    base = (u8 *)&gSys;
    *(u32 *)(base + 0x880) = (*hook)(head, *(u32 *)(base + 0x880));
}

void obj_clear_flag490_8(u8 *obj)
{
    *(u32 *)(obj + 0x490) &= -9;
}

void obj_set_flag490_8(u8 *obj)
{
    *(u32 *)(obj + 0x490) |= 8;
}

void child_states_obj_state4_if_flag8(u8 *obj)
{
    if (gSys.keys2A & 8)
        *(u32 *)(obj + 0x474) = 4;
}

void obj_helpers_c0f8(u8 *obj, u32 flag)
{
    if (flag & 1)
        heap_free_b(obj);
}

void sub_0816C110(void)
{
}

void sub_0816C114(u8 *obj, s32 v)
{
    *(s32 *)(obj + 8) += v;
}

s32 sub_0816C11C(u8 *obj)
{
    if (*(s32 *)(obj + 4) >= *(s32 *)obj)
        return 1;
    else
        return 0;
}

void sub_0816C130(u8 *obj)
{
    *(u32 *)(obj + 4) = 0;
    *(u32 *)(obj + 8) = 0;
}

void sub_0816C138(struct ObjChildren *obj, u32 flags)
{
    struct ChildObj *child;

    child = obj->child10;
    if (child != NULL)
        child->vtable->slot8.fn((u8 *)child + child->vtable->slot8.offset, 3);
    child = obj->child0C;
    if (child != NULL)
        child->vtable->slot8.fn((u8 *)child + child->vtable->slot8.offset, 3);
    if (flags & 1)
        heap_free_b(obj);
}

void sub_0816C180(struct ObjWithVt *obj, u32 flags)
{
    struct ChildObj *child;

    child = obj->child10;
    if (child->flags12 & 0x10)
    {
        obj_list_unlink(flags, obj);
        if (obj != NULL)
            obj->vtable->slot8.fn((u8 *)obj + obj->vtable->slot8.offset, 3);
    }
}

void obj_destroy_eb98(u8 *obj, u32 flags)
{
    *(u32 *)(obj + 0xC) = (u32)gClassEb98;
    obj_destroy_sprites(obj, flags);
}

struct SpriteAttr12
{
    u8 filler0[0x12];
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

u8 *obj_init_sprite_offset(u8 *obj, s32 *src, s32 dx, s32 dz)
{
    struct SpriteAttr12 *sprite;

    obj_init_big(obj, 6, dx, dz);
    *(u32 *)(obj + 0xC) = (u32)gClassEb98;
    obj_create_sprite_10(obj, 3, 0x415A, 0, 0, 2);
    sprite = (struct SpriteAttr12 *)*(u8 **)(obj + 0x10);
    sprite->mode = 1;
    obj_set_fields_18(obj, src[0] - dx, src[1], src[2] - dz);
    return obj;
}

void obj_init_fields_8c_load(u8 *obj, u32 a1, u16 a2)
{
    s32 zero;

    *(u32 *)(obj + 0x94) = a1;
    *(u16 *)(obj + 0x92) = (zero = 0, a2);
    *(u16 *)(obj + 0x8E) = zero;
    *(u16 *)(obj + 0x8C) = 4;
    if (*(u16 *)(obj + 0x98) > 1)
        obj_load_record(*(void **)(obj + 0x10), 0x13, 0, 0, zero);
    else
        obj_load_record(*(void **)(obj + 0x10), 0x10, 0, 0, zero);
}

void child_states_obj_orbit_step(u8 *obj)
{
    u8 zero = 0;
    s32 sinv;
    s32 cosv;
    s32 x;
    s32 y;
    u32 idx;
    const s16 *sineTab;

    if (obj_is_done(obj))
    {
        sineTab = gSineTable;
        idx = (*(u32 *)(obj + 0x7C)) & 0xFF;
        sinv = sineTab[idx];
        if (sinv < 0)
            sinv += 0x3F;
        x = *(s32 *)(obj + 0x6C) + (sinv >> 6) * *(s32 *)(obj + 0x80);
        cosv = gCosineTable[idx];
        if (cosv < 0)
            cosv += 0x3F;
        y = *(s32 *)(obj + 0x74) + (cosv >> 6) * *(s32 *)(obj + 0x84);
        obj_setup_slot(obj, x, 0, y, 0x16, zero);
    }
}

void obj_destroy_eba8(u8 *obj, u32 flags)
{
    *(u32 *)(obj + 0xC) = (u32)gClassEba8;
    obj_destroy_sprites(obj, flags);
}

NONMATCH("asm/code_0816AFFC/obj_near_mark.s", u32 obj_near_mark(u8 *obj, u8 *other))
{
    u16 *flags;
    s32 one;
    s32 dx;
    s32 dy;
    s32 dist;

    one = 1;
    flags = (u16 *)(other + 0x6C);
    if ((*flags & one) != 0)
        return 0;
    dx = *(s32 *)(obj + 0x18) - *(s32 *)(other + 0x18);
    dy = *(s32 *)(obj + 0x20) - *(s32 *)(other + 0x20);
    dist = (dx * dx + dy * dy) >> 8;
    if (dist > 0x8FFF)
        return 0;
    set_slot_value(0x37, -1);
    *flags = *flags | one;
    *(u32 *)(obj + 0x6C) = 6;
    return 1;
}
END_NONMATCH

void obj_set_speed_from(u8 *obj, u8 *other)
{
    u16 *flags;
    s32 mask;
    u16 cleared;
    s32 zero;
    s32 vy;

    flags = (u16 *)(other + 0x6C);
    mask = -2;
    cleared = mask & *flags;
    zero = 0;
    *flags = cleared;
    set_slot_value(0x4C, -1);
    *(u32 *)(obj + 0x80) = 0x78;
    obj_load_record(*(void **)(obj + 0x10), 0xB, 0, 0, zero);
    vy = *(s32 *)(other + 0x7C);
    *(s32 *)(obj + 0x70) = -vy * (*(s32 *)(other + 0x78) >> 3);
    *(s32 *)(obj + 0x78) = vy * (*(s32 *)(other + 0x74) >> 3);
    *(u32 *)(obj + 0x6C) = 3;
}

void obj_destroy_ebb8(u8 *obj, u32 flags)
{
    *(u32 *)(obj + 0xC) = (u32)gClassEbb8;
    obj_destroy_sprites(obj, flags);
}

struct FlagHolder
{
    u8 filler0[0x6C];
    u16 flags6C;
};

struct ObjFlagPair
{
    struct FlagHolder *first;
    struct FlagHolder *second;
    u8 filler8[0x50];
    u32 unk58;
    u32 unk5C;
};

void obj_clear_flags_6c(struct ObjFlagPair *obj)
{
    s32 mask;

    obj->unk5C = 0x10;
    obj->unk58 = 0;
    {
        u16 *flags = &obj->first->flags6C;

        mask = -10;
        *flags = mask & *flags;
    }
    obj->second->flags6C = mask & obj->second->flags6C;
}

void sub_0816C3F4(u8 *obj, u32 flag)
{
    if (flag & 1)
        heap_free_b(obj);
}

void obj_destroy_ebc8(u8 *obj, u32 flags)
{
    *(u32 *)(obj + 0xC) = (u32)gClassEbc8;
    obj_destroy_sprites(obj, flags);
}

u8 *init_vt_08cdebc8(u8 *obj, u32 a, u32 b, u32 c)
{
    s32 zero;

    obj_init_big(obj, a, b, c);
    *(u32 *)(obj + 0xC) = (u32)gClassEbc8;
    *(u16 *)(obj + 0x6C) = zero = 0;
    *(u16 *)(obj + 0x6E) = zero;
    *(u16 *)(obj + 0x70) = c;
    *(u32 *)(obj + 0x74) = zero;
    *(u32 *)(obj + 0x78) = 0x100;
    *(u32 *)(obj + 0x7C) = b;
    return obj;
}

NONMATCH("asm/code_0816AFFC/start_hblank_scroll_dma.s", void start_hblank_scroll_dma(void))
{
    vu16 *bg3hofs = (vu16 *)REG_ADDR_BG3HOFS;
    vu16 *bg3vofs;
    u16 *table = (u16 *)gUnk_03001030;
    vu32 *dma;

    *bg3hofs = table[0];
    bg3vofs = (vu16 *)REG_ADDR_BG3VOFS;
    *bg3vofs = table[1];
    dma = (vu32 *)((u8 *)bg3vofs + 0x92);
    REG_DMA0CNT_H = 0;
    dma[0] = (u32)(table + 2);
    dma[1] = (u32)bg3hofs;
    dma[2] = 0xA6600001;
}
END_NONMATCH

void obj_remove_eb80(u8 *obj, u32 flags)
{
    *(u32 *)(obj + 0x18) = (u32)gClassEb80;
    task_remove(obj, flags);
}

u8 *init_vt_08cdeb80(u8 *obj, u8 kind)
{
    task_insert(obj, kind);
    *(u32 *)(obj + 0x18) = (u32)gClassEb80;
    *(u32 *)(obj + 0x20) = 0;
    *(u32 *)(obj + 0x1c) = 0;
    return obj;
}

u32 obj_flag_helpers_90_6c(u8 *obj, u32 mask)
{
    u32 t = *(u16 *)(obj + 0x90) & mask;
    return (-t | t) >> 31;
}

void sub_0816C4E0(u8 *obj, u16 mask)
{
    *(u16 *)(obj + 0x90) &= ~mask;
}

void sub_0816C4F0(u8 *obj, u16 mask)
{
    *(u16 *)(obj + 0x90) |= mask;
}

u32 sub_0816C500(u8 *obj, u32 mask)
{
    u32 t = *(u16 *)(obj + 0x6C) & mask;
    return (-t | t) >> 31;
}

void sub_0816C510(u8 *obj, u16 mask)
{
    *(u16 *)(obj + 0x6C) &= ~mask;
}

void sub_0816C520(u8 *obj, u16 mask)
{
    *(u16 *)(obj + 0x6C) |= mask;
}

NONMATCH("asm/code_0816AFFC/obj_update_anim_pair.s", void obj_update_anim_pair(u8 *obj, u32 phase, u32 flag))
{
    s32 vals[2];
    s32 idx;
    s32 off;
    u8 phase8;
    s32 timer;
    s32 count;
    s32 v1;
    s32 (*divFn)(s32, s32);

    phase8 = phase;
    if ((u8)flag != 0)
    {
        timer = *(s32 *)(obj + 0x1C) + 1;
        *(s32 *)(obj + 0x1C) = timer;
        if (timer > 59)
        {
            *(s32 *)(obj + 0x1C) = 0;
            count = *(s32 *)(obj + 0x18) - 1;
            *(s32 *)(obj + 0x18) = count;
            if (count < 0)
                *(s32 *)(obj + 0x18) = 0;
        }
    }
    divFn = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    v1 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424)(*(s32 *)(obj + 0x18), 0x64);
    vals[0] = divFn(v1, 0xA);
    vals[1] = MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424)(*(s32 *)(obj + 0x18), 0xA);

    if (phase8 != 0)
    {
        *(s32 *)(obj + 0x10) = vals[0];
        *(s32 *)(obj + 8) = vals[0];
        obj_load_record(*(void **)(*(u8 **)obj + 0x10), vals[0] + 0x1B, 0, 0, 0);
        obj_set_fields_18(*(u8 **)obj, *(s32 *)(*(u8 **)obj + 0x18), 0x3000, 0);
        *(s32 *)(obj + 0x14) = vals[1];
        *(s32 *)(obj + 0xC) = vals[1];
        obj_load_record(*(void **)(*(u8 **)(obj + 4) + 0x10), vals[1] + 0x1B, 0, 0, 0);
        obj_set_fields_18(*(u8 **)(obj + 4), *(s32 *)(*(u8 **)(obj + 4) + 0x18), 0x3000, 0);
    }
    else
    {
        for (idx = 0; idx < 2; idx++)
        {
            off = idx * 4;
            if (obj_is_done(*(u8 **)(obj + off)))
            {
                if (*(s32 *)(obj + 8 + off) != vals[idx])
                {
                    *(s32 *)(obj + 0x10 + off) = *(s32 *)(obj + 8 + off);
                    *(s32 *)(obj + 8 + off) = *(s32 *)(obj + 8 + off) - 1;
                    if (*(s32 *)(obj + 8 + off) < 0)
                        *(s32 *)(obj + 8 + off) = 9;
                    obj_load_record(*(void **)(*(u8 **)(obj + off) + 0x10), *(s32 *)(obj + 8 + off) + 0x1B, 0, 0, 0);
                    obj_set_fields_18(*(u8 **)(obj + off), *(s32 *)(*(u8 **)(obj + off) + 0x18), 0x2000, 0);
                    obj_setup_slot(*(u8 **)(obj + off), *(s32 *)(*(u8 **)(obj + off) + 0x18), 0x3000, 0, 6, (u8)1);
                }
            }
            else
                obj_script_step(*(u8 **)(obj + off));
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0816AFFC/obj_create_group.s", void obj_create_group(void))

ASM_FUNC("asm/code_0816AFFC/obj_place_children.s", void obj_place_children(u8 *obj, u32 flag))
