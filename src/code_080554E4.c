#include "global.h"

struct OverlayObj
{
    u8 filler0[0x18];
    void *vtable;
    u8 filler1C[4];
    void *heap20;
    u8 filler24[4];
    void *heap28;
    void *heap2C;
    void *task30;
};

void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void heap_free_a(void *p);
void heap_free_b(void *p);
void run_pending_obj_callbacks(void);
extern const u8 gOverlayVtable[];
extern const u8 gChildVtable[];

ASM_FUNC("asm/code_080554E4/overlay_update.s", void overlay_update(void))

ASM_FUNC("asm/code_080554E4/textbox_draw_oam.s", void textbox_draw_oam(void))

void overlay_free(struct OverlayObj *obj, u32 flags)
{
    obj->vtable = (void *)gOverlayVtable;
    if (obj->task30 != 0)
    {
        task_remove(obj->task30, 3);
    }
    if (obj->heap20 != 0)
    {
        heap_free_a(obj->heap20);
    }
    if (obj->heap28 != 0)
    {
        heap_free_a(obj->heap28);
    }
    heap_free_b(obj->heap2C);
    run_pending_obj_callbacks();
    task_remove(obj, flags);
}

struct OverlayObj *overlay_child_init(struct OverlayObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->vtable = (void *)gChildVtable;
    return obj;
}

ASM_FUNC("asm/code_080554E4/scene_create.s", void scene_create(void))

ASM_FUNC("asm/code_080554E4/scene_step.s", void scene_step(void))

ASM_FUNC("asm/code_080554E4/scene_start_task.s", void scene_start_task(void))

ASM_FUNC("asm/code_080554E4/scene_reset_sprites.s", void scene_reset_sprites(void))

ASM_FUNC("asm/code_080554E4/map_obj_draw_obj_setup_sprites.s", void map_obj_draw_obj_setup_sprites(void))

ASM_FUNC("asm/code_080554E4/hblank_scroll_table_dma.s", void hblank_scroll_table_dma(void))

ASM_FUNC("asm/code_080554E4/map_scene_update.s", void map_scene_update(void))
