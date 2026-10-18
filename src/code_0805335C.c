#include "global.h"

struct SysFlags888
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct TaskDescObj
{
    u8 filler0[0x18];
    const void *desc;
};

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

struct TextboxObj
{
    u8 state;
    u8 filler1[0x18 - 1];
    const void *unk18;
    u8 unk1C;
    u8 filler1D[0x20 - 0x1D];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    void *unk30;
    void *unk34;
    void *unk38;
    u8 unk3C;
    u8 filler3D[0x40 - 0x3D];
    void *unk40;
    u8 unk44;
    u8 unk45;
    u8 filler46[0x48 - 0x46];
    u8 unk48[0x10];
};

void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void heap_free_a(void *p);
void heap_free_b(void *p);
void run_pending_obj_callbacks(void);
void serial_link_disable(void);
void obj_list_pass_0a8c(void);
extern const u8 gTaskDescrBox[];
extern const u8 gDescChild[];
extern const u8 gTextboxTaskClass[];
extern const u8 gDescTextbox[];
extern const u8 gTaskDescrTextbox[];
extern u16 gUnk_02000050;
extern u8 gSys[];
extern struct ObjLists gUnk_0203FFB8;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
void window_place(void *window, void *data, u32 x, u32 y, u32 w, u32 h, u32 flags);
extern u8 *const gBgRequestPtr;
extern const u8 gBoxStyleSizes[];
extern const u8 gTagSceneObj[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void scene_start_task(void *node, u32 kind, const void *tag, u32 arg);
void window_anim_step(void *window);
extern const u8 gLangBoxSizes[];

ASM_FUNC("asm/code_0805335C/textbox_setup_tiles.s", void textbox_setup_tiles(void))

ASM_FUNC("asm/code_0805335C/textbox_place.s", void textbox_place(void))

ASM_FUNC("asm/code_0805335C/textbox_emit_oam.s", void textbox_emit_oam(void))

ASM_FUNC("asm/code_0805335C/textbox_open_scene.s", void textbox_open_scene(void))

NONMATCH("asm/code_0805335C/textbox_free.s", void textbox_free(struct TextboxObj *obj, u32 kind))
{
    u8 *base;
    struct SysFlags888 *flags;
    u16 *word;

    obj->unk18 = gTaskDescrTextbox;
    if (obj->unk34 != NULL)
        task_remove(obj->unk34, 3);
    if (obj->unk38 != NULL)
        heap_free_a(obj->unk38);
    if (obj->unk20 != NULL)
        heap_free_a(obj->unk20);
    if (obj->unk28 != NULL)
        heap_free_a(obj->unk28);
    if (obj->unk2C != NULL)
        heap_free_a(obj->unk2C);
    heap_free_b(obj->unk30);
    run_pending_obj_callbacks();
    base = gSys;
    flags = (struct SysFlags888 *)(base + 0x888);
    flags->bit5 = 0;
    flags->bit6 = 0;
    flags->bit3 = 0;
    base[0x31] = 2;
    word = (u16 *)0x02000000;
    *word = 0;
    word[0x28] = 0;
    task_remove(obj, kind);
}
END_NONMATCH

ASM_FUNC("asm/code_0805335C/textbox_step.s", void textbox_step(void))

ASM_FUNC("asm/code_0805335C/textbox_create.s", void textbox_create(void))

NONMATCH("asm/code_0805335C/textbox_update.s", void textbox_update(struct TextboxObj *obj))
{
    switch (obj->state)
    {
    case 0:
    {
        s32 anim;
        s32 timer;

        anim = ((s8 *)&obj->unk38)[1];
        if (anim > 0)
        {
            window_anim_step(&obj->unk38);
            break;
        }
        timer = obj->unk1C - 1;
        obj->unk1C = timer;
        if ((timer << 24) != 0 && (*(u16 *)(gSys + 0x2A) & 0xB) == 0)
            break;
        gBgRequestPtr[1] |= 0x20;
        ((u8 *)&obj->unk38)[0] |= 1;
        ((u8 *)&obj->unk38)[1] = 7;
        obj->state = 1;
        break;
    }
    case 1:
    {
        s32 anim;

        window_anim_step(&obj->unk38);
        anim = ((s8 *)&obj->unk38)[1];
        if (anim != 0)
            break;
        if (obj != NULL)
        {
            obj->unk18 = gTextboxTaskClass;
            if (obj->unk34 != NULL)
                heap_free_a(obj->unk34);
            if (obj->unk20 != NULL)
                heap_free_a(obj->unk20);
            if (obj->unk28 != NULL)
                heap_free_a(obj->unk28);
            if (obj->unk2C != NULL)
                heap_free_a(obj->unk2C);
            heap_free_b(obj->unk30);
            *(u16 *)0x02000000 = 0;
            *(u16 *)0x02000050 = 0;
            task_remove(obj, 3);
        }
        scene_start_task(heap_alloc2(0xE4, 0, gTagSceneObj, 0), 8, gTagSceneObj, 2);
        break;
    }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0805335C/textbox_place_style.s", void textbox_place_style(void))

void textbox_task_free(struct TextboxObj *obj, u32 kind)
{
    obj->unk18 = gTextboxTaskClass;
    if (obj->unk34 != NULL)
        heap_free_a(obj->unk34);
    if (obj->unk20 != NULL)
        heap_free_a(obj->unk20);
    if (obj->unk28 != NULL)
        heap_free_a(obj->unk28);
    if (obj->unk2C != NULL)
        heap_free_a(obj->unk2C);
    heap_free_b(obj->unk30);
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000050 = 0;
    task_remove(obj, kind);
}

void oam_emit_objects(void)
{
    u8 *base;
    u32 *oam_slot;
    u32 old;

    base = gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    obj_list_pass_0a8c();
    gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
    old = gUnk_03000D4C(gUnk_0203FFB8.root, old);
    *oam_slot = old;
}

void textbox_place_rect(struct TextboxObj *obj, s32 w, s32 h)
{
    u32 x;
    u32 y;
    void *window;
    void *data;

    x = 0xF - w / 2;
    y = 0xA - h / 2;
    window = &obj->unk40;
    data = obj->unk38;
    window_place(window, data, (u8)x, (u8)y, (u8)w, (u8)h, 0xE000);
    *(gBgRequestPtr + 0x4A) |= 0x10;
}

void textbox_place_anchor(struct TextboxObj *obj, s32 w, s32 h)
{
    u32 x;
    u32 y;

    x = (u8)(0xF - w / 2);
    y = (u8)(0xA - h / 2);
    window_place(&obj->unk48, obj->unk40, x, y, (u8)w, (u8)h, 0xC800);
    obj->unk44 = (u8)((x << 3) + (w << 2) - 0x10);
    obj->unk45 = (u8)(((y + h) << 3) - 0x16);
}

void textbox_free_disable_link(struct TextboxObj *obj, u32 kind)
{
    obj->unk18 = gDescTextbox;
    run_pending_obj_callbacks();
    if (obj->unk20 != NULL)
        task_remove(obj->unk20, 3);
    if (obj->unk40 != NULL)
        heap_free_a(obj->unk40);
    if (obj->unk24 != NULL)
        heap_free_a(obj->unk24);
    if (obj->unk30 != NULL)
        heap_free_a(obj->unk30);
    if (obj->unk34 != NULL)
        heap_free_a(obj->unk34);
    heap_free_b(obj->unk38);
    if ((obj->unk3C & 1) == 0)
        serial_link_disable();
    gSys[0x31] = 2;
    *(u16 *)0x02000000 = 0;
    gUnk_02000050 = 0x10;
    task_remove(obj, kind);
}

struct TaskDescObj *task_descr_box_init(struct TaskDescObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->desc = gTaskDescrBox;
    return obj;
}

struct TaskDescObj *desc_child_init(struct TaskDescObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->desc = gDescChild;
    return obj;
}

ASM_FUNC("asm/code_0805335C/overlay_init.s", void overlay_init(void))
