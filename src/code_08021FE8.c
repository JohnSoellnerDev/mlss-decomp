#include "global.h"

struct ObjListHeadX
{
    u8 filler0[0x3C];
    void *buf3C;
};

struct SysBits88A
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct ModelCtx;

struct SceneObj
{
    u8 filler0[0x18];
    const void *cls18;
    void *key1C;
    void *attr20;
    void *ptr24;
    u8 filler28[0x1FC - 0x28];
    void *sub1FC;
    u8 filler200[0x23C - 0x200];
    void *ptr23C;
    u8 filler240[0x304 - 0x240];
    void *sub304;
    void *sub308;
    void *sub30C;
    void *sub310;
    void *sub314;
    u8 filler318[0x31C - 0x318];
    void *sub31C;
    void *sub320;
};

extern struct ObjListHeadX gUnk_0203FFB8;
extern struct ModelCtx *gModelCtx;
extern const u8 gSceneObjClass[];
extern const u8 gEntryUpdateTaskClass[];
extern u8 gSys[];
void *heap_free_a(void *p);
void release_timers_0908(void *arg);
void menu_obj_free(void *obj, u32 kind);
void task_remove(void *node, u32 kind);
void free_block_if_bit0(void *block, u32 flags);
void map_view_map_obj_destroy(void *obj, u32 arg);
void obj_release_parts(void *parts, u32 mode);
void destroy_obj_children(struct SceneObj *obj);
void heap_free_b(void *p);
void teardown_effect_task(void *obj, u32 flags);
void task_slot_release(void *slot, u32 flags);
void free_effect_state(void *p);
void set_irq_callback(u32 kind, void *cb);
void task_insert(void *node, u8 kind, u32 data);
void *heap_alloc2(u32 size, u32 pool, const void *tag, u32 clear);
void *heap_alloc(u32 size, u32 mode, const void *tag, u32 flags);
void *task_slot_attach(void *heap, u32 a, u32 b, const void *tag);
void *init_effect_state(void *heap, u32 a, u32 b, const void *tag);
void set_flag_2_2(void *p);
void mt19937_seed(u32 state, u32 seed);
void init_obj_slots(struct SceneObj *obj);
extern const u8 gTagTask[];
extern const u8 gTagEffect[];
extern const u8 gHeapTagObjBuf1c[];
extern const u8 gHeapTagObjBuf20[];
extern const u8 gHeapTagObjBuf23c[];
extern const u8 gHeapTagObjBuf24[];
extern void *gUnk_03000D7C;
extern void *gUnk_03000D78;
extern u8 *gEffectState;

void run_pending_obj_callbacks(void)
{
    void *pending = gUnk_0203FFB8.buf3C;
    void *arg = pending;

    if (pending != 0)
    {
        arg = heap_free_a(pending);
    }
    release_timers_0908(arg);
    gModelCtx = 0;
}

typedef union __attribute__((packed)) { u8 value; } ByteArgX __attribute__((transparent_union));

static inline void and8(u8 *p, s32 m)
{
    s32 b = *p;

    b &= m;
    *p = b;
}

static inline void or8(u8 *p, s32 m)
{
    s32 b = *p;

    b |= m;
    *p = b;
}

static inline void and16(u16 *p, s32 m)
{
    s32 b = *p;

    b &= m;
    *p = b;
}

struct InitLocals
{
    u32 a5;
    u32 a6;
    u8 *pf8;
    u8 flag;
    u8 filler[3];
    u8 *p246;
};

NONMATCH("asm/code_08021FE8/init_scene_obj.s", struct SceneObj *init_scene_obj(struct SceneObj *obj, u8 kind, u32 arg2, u32 speed, ByteArgX arg5, ByteArgX arg6))
{
    u32 noSpeed;
    u8 *o;
    u8 *pf9;
    u8 *sys;
    u8 *p208;
    u8 *p2b8;
    void **slot;
    void *task;
    void *effect;
    struct InitLocals L;
    u32 zero;
    u8 *base;

    o = (u8 *)obj;
    L.a5 = arg5.value;
    L.a6 = arg6.value;
    task_insert(obj, kind, arg2);
    obj->cls18 = gSceneObjClass;
    noSpeed = 0;
    if (speed == 0xFFFFFFFF)
    {
        noSpeed = 1;
    }
    L.pf8 = o + 0xF8;
    L.flag = 1;
    *L.pf8 = (*L.pf8 & ~2) | (noSpeed << 1);

    slot = (void **)&obj->sub320;
    task = heap_alloc2(0x24, 0, gTagTask, 0);
    task = task_slot_attach(task, 1, 7, gTagTask);
    *slot = task;
    *(struct SceneObj **)(*(u8 **)task + 0x14) = obj;
    set_flag_2_2(*(void **)task);

    slot = (void **)&obj->sub31C;
    effect = heap_alloc2(0x24, 0, gTagEffect, 0);
    effect = init_effect_state(effect, 1, 6, gTagEffect);
    *slot = effect;
    *(struct SceneObj **)(*(u8 **)effect + 0x14) = obj;
    set_flag_2_2(*(void **)effect);

    sys = gSys;
    mt19937_seed(*(u32 *)(sys + 0x8D0), *(u32 *)(sys + 0x1C));

    gUnk_03000D7C = obj;
    gUnk_03000D78 = obj->sub31C;
    *(u16 *)0x02000000 = 0;
    sys = gSys;
    *(u8 **)(o + 0xD0) = sys + 0x3C;

    obj->key1C = heap_alloc2(0x2C0, 1, gHeapTagObjBuf1c, 1);
    obj->attr20 = heap_alloc2(0x574, 1, gHeapTagObjBuf20, 1);
    obj->ptr23C = heap_alloc(0x1000, 1, gHeapTagObjBuf23c, 1);
    obj->ptr24 = heap_alloc(0x400, 1, gHeapTagObjBuf24, 1);

    and8(L.pf8, ~5);
    and8(L.pf8, ~1);
    pf9 = o + 0xF9;
    *pf9 = (*pf9 & ~8) | ((L.a5 & L.flag) << 3);
    p208 = o + 0x208;
    or8(p208, L.flag);
    and8(pf9, ~3);
    and8(pf9, ~4);

    zero = 0;
    *(u32 *)(o + 0xF0) = zero;
    *(u32 *)(o + 0xE4) = zero;
    p2b8 = o + 0x2B8;
    and8(p2b8, ~1);
    and8(p2b8, ~0x10);
    and8(p2b8, ~0x20);
    *(u32 *)(o + 0x2CC) = zero;
    *(u16 *)(o + 0x11E) = zero;
    and16((u16 *)(o + 0x120), 0xFC00);
    and8(o + 0x121, ~0x3C);
    *(u16 *)(o + 0x122) = zero;
    *(u16 *)(o + 0x124) = speed;
    *(u16 *)(o + 0x128) = zero;
    or8(o + 0xFC, 0x780);
    and8(o + 0xFD, ~8);

    L.p246 = o + 0x246;
    and8(L.p246, ~1);
    and8(o + 0x291, ~3);
    and8(o + 0x2A8, ~0x40);
    *(u8 *)(o + 0x2B9) = 0xFF;
    and8(p2b8, ~0x40);
    *(u8 *)(o + 0x12B) = 0xFF;
    *(u8 *)(o + 0x12C) = ((*(u8 *)(o + 0x12C) & 3) | 0xF8) & ~2;
    and8(o + 0x12D, ~0x31);
    and8(o + 0xFC, ~4);
    *(u8 *)(o + 0x10A) = 0;
    and8(o + 0xFC, ~1);
    *(u16 *)(o + 0x12E) = zero;
    *(u16 *)(o + 0x130) = zero;

    *pf9 = ((*pf9 & ~0x20) & ~0x10) | (((L.flag ^ L.a5) & L.flag) << 4);
    *(u16 *)(o + 0x27A) = zero;
    *(u16 *)(o + 0x27C) = zero;
    *(u16 *)(o + 0x27E) = zero;
    *(u16 *)(o + 0x280) = zero;
    *(u16 *)(o + 0x282) = zero;
    *(u16 *)(o + 0x284) = zero;
    *(u8 *)(o + 0x28A) = 0x68;
    *(u8 *)(o + 0x28B) = 0x88;
    *(u8 *)(o + 0x28C) = 0x50;
    *(u8 *)(o + 0x28D) = 0x6A;
    *(u8 *)(o + 0x28E) = 0x26;
    *(u8 *)(o + 0x28F) = 0x6A;

    and8(o + 0x208, ~4);
    or8(o + 0x208, 8);
    and8(o + 0x209, ~1);
    and8(o + 0x208, ~0x70);
    and8(o + 0x208, 0x7F);
    or8(L.pf8, 0x10);
    and8(o + 0x209, ~1);
    *(u32 *)(o + 0x208) = (*(u32 *)(o + 0x208) & 0xFFFE7FFF) | 0x8000;
    and8(o + 0x20A, ~0x7E);
    or8(o + 0x2D8, 0x1F);
    *(u16 *)(o + 0x240) = 0x1000;
    and16((u16 *)(o + 0x20A), 0xFE7F);
    *(u16 *)(o + 0x242) = zero;
    *(u16 *)(o + 0x244) = zero;
    *(u8 *)(o + 0x20B) = (*(u8 *)(o + 0x20B) & 0xF) | 2;
    and8(o + 0x209, ~0x7E);
    and8(o + 0xF9, 0x7F);
    or8(o + 0xFA, 0x10);

    *(u8 *)(o + 0x26C) = 0;
    and8(L.p246, ~0x1C);
    and8(o + 0x2A9, 0x3F);
    *L.pf8 = (*L.pf8 & ~8) | ((L.a6 & L.flag) << 3);
    *(u16 *)(o + 0x1FA) = 0xFF00;
    *(u32 *)(o + 0x304) = zero;
    *(u32 *)(o + 0x308) = zero;
    *(u32 *)(o + 0x30C) = zero;
    *(u32 *)(o + 0x310) = zero;
    *(u32 *)(o + 0x314) = zero;
    *(u16 *)(o + 0x1F6) = 0xFFFF;
    *(u8 *)(o + 0x1F8) = 0;
    *(u16 *)(o + 0x2B2) = 0xFFFE;
    *(u32 *)(o + 0x318) = zero;
    init_obj_slots(obj);

    sys = gSys;
    ((struct SysBits88A *)&sys[0x88A])->bit5 = 1;
    base = gEffectState;
    *(u32 *)(base + 0x528) = 0;
    *(u32 *)(base + 0x530) = 0;
    return obj;
}
END_NONMATCH

void obj_teardown_subs(struct SceneObj *obj, u32 param)
{
    struct SceneObj *sub;
    u8 *sys;

    obj->cls18 = gSceneObjClass;
    if (obj->sub314 != 0)
    {
        menu_obj_free(obj->sub314, 3);
    }
    if (obj->sub30C != 0)
    {
        task_remove(obj->sub30C, 3);
    }
    sub = obj->sub310;
    if (sub != 0)
    {
        sub->cls18 = gEntryUpdateTaskClass;
        task_remove(sub, 3);
    }
    if (obj->sub1FC != 0)
    {
        free_block_if_bit0(obj->sub1FC, 3);
    }
    if (obj->sub308 != 0)
    {
        map_view_map_obj_destroy(obj->sub308, 3);
    }
    if (obj->sub304 != 0)
    {
        obj_release_parts(obj->sub304, 3);
    }
    destroy_obj_children(obj);
    heap_free_b(obj->key1C);
    heap_free_b(obj->attr20);
    heap_free_b(obj->ptr23C);
    heap_free_b(obj->ptr24);
    if (obj->sub31C != 0)
    {
        teardown_effect_task(obj->sub31C, 3);
    }
    if (obj->sub320 != 0)
    {
        task_slot_release(obj->sub320, 3);
    }
    free_effect_state(obj->sub31C);
    run_pending_obj_callbacks();
    set_irq_callback(2, 0);
    sys = gSys;
    ((struct SysBits88A *)&sys[0x88A])->bit5 = 0;
    task_remove(obj, param);
}

ASM_FUNC("asm/code_08021FE8/scene_obj_state_dispatch.s", void scene_obj_state_dispatch(void))
