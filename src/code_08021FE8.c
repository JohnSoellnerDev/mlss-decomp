#include "global.h"

struct ObjListHeadX
{
    u8 filler0[0x3C];
    void *buf3C;
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

ASM_FUNC("asm/code_08021FE8/init_scene_obj.s", void init_scene_obj(void))

NONMATCH("asm/code_08021FE8/obj_teardown_subs.s", void obj_teardown_subs(struct SceneObj *obj, u32 param))
{
    struct SceneObj *sub;
    u32 flags;

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
    flags = -0x21 & gSys[0x88A];
    gSys[0x88A] = flags;
    task_remove(obj, param);
}
END_NONMATCH

ASM_FUNC("asm/code_08021FE8/scene_obj_state_dispatch.s", void scene_obj_state_dispatch(void))
