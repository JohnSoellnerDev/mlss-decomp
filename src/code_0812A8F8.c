#include "global.h"
#include "ram_routines.h"

struct KeyBlock
{
    u8 filler0[8];
    u8 repeatDelay8;
    u8 repeatRate9;
    u8 filler10[0x3C - 0x0A];
    u16 enableMask3C;
};

struct SubObj
{
    u8 filler0[0x14];
    void *owner14;
    const void *cb18;
};

struct MenuObj
{
    u8 filler0[0x18];
    const void *cb18;
    struct SubObj *sub1C;
    struct SubObj *sub20;
    void *owner24;
    void *anim28;
    u8 filler2C[0x38 - 0x2C];
    void *block38;
    struct KeyBlock *keys3C;
    void *unk40;
    void *unk44;
    void *unk48;
    void *unk4C;
    void *unk50;
    void *unk54;
    void *unk58;
    u8 filler5C[0x69 - 0x5C];
    u8 flag69;
    u8 filler6A[0x8E - 0x6A];
    u16 keyMask8E;
    u8 repeatDelay90;
    u8 repeatRate91;
    u8 filler92[0xA2 - 0x92];
    u16 dispcntCopyA2;
    u8 fillerA4[0xBD - 0xA4];
    u8 flagsBD;
    u8 flagsBE;
};

struct StatusCtx
{
    u8 filler0[0x14];
    struct StatusView *view14;
};

struct ObjLists
{
    u8 filler0[0x3C];
    void *head3C;
};

struct StatusView
{
    u8 state0;
    u8 filler1[0x5C - 0x1];
    void *owner5C;
    u8 filler60[0x67 - 0x60];
    u8 unk67;
    u8 filler68[0x74 - 0x68];
    u8 unk74;
    u8 filler75[0x7C - 0x75];
    u8 unk7C;
    u8 filler7D[0x80 - 0x7D];
    u16 unk80;
    u8 filler82[0x84 - 0x82];
    u8 unk84;
    u8 unk85;
    u8 unk86;
    u8 unk87;
    u8 filler88[0xAF - 0x88];
    u8 flagsAF;
    u8 flagsB0;
    u8 unkB1;
};

extern const u8 gSaveMenuChildClass[];
extern const u8 gScreenCallbacks[];
extern const u8 gSubCallbacks[];
extern const u8 gScreenTagTask[];
extern const u8 gScreenTagMenu[];
extern const u8 gScreenTagAnim[];
extern u8 gUnk_03000388[];
extern u8 gSys[];
extern u8 *gParty;
extern u32 gUnk_03000D40;
void task_insert(void *node, u8 kind, ...);
u32 sub_obj_running(void *menu);
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void *draw_two_values(void *ctx, u8 a, u8 b, u8 mask);
void *text_sprites_b(void *oam, u32 a, u32 b, u32 c);
void *draw_member_status(void *oam, u32 a, u32 b);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
struct SubObj *menu_obj_init(void *block, u32 a, const void *tag, u32 c);
void *clear_anim_record(void *block);
u32 dma_cancel_request(u32 dst);
void run_pending_obj_callbacks(void);
void owner_destroy(void *owner, u32 mode);
void heap_free_a(void *p);
void heap_free_b(void *p);
void menu_obj_free(void *sub, u32 mode);
void task_remove(void *node, u32 kind);
void update_obj_list(void);
void obj_list_pass_0a8c(void);
void status_view_redraw(struct StatusView *view);
void *member_panel_draw(struct StatusView *view, void *oam);
extern u8 gUnk_0203FFB8[];
extern void *gUnk_03000D48;
extern void *gUnk_03000D4C;

ASM_FUNC("asm/code_0812A8F8/member_panel_draw.s", void *member_panel_draw(struct StatusView *view, void *oam))



ASM_FUNC("asm/code_0812A8F8/status_view_redraw.s", void status_view_redraw(struct StatusView *view))

ASM_FUNC("asm/code_0812A8F8/window_state_update.s", void window_state_update(void))

ASM_FUNC("asm/code_0812A8F8/status_screen_update.s", void status_screen_update(void))

u32 sub_0812B634(struct MenuObj *obj)
{
    u8 busy = sub_obj_running(obj->sub20);

    if (busy == 0)
    {
        obj->flag69 = busy;
        return 0;
    }
    return 1;
}

void sub_0812B658(struct MenuObj *obj, u16 pos)
{
    sub_08115F54(obj->sub20, 4, pos, 8, 0xFFFF, 0xFFFF, 0, 0);
    obj->flag69 = 1;
}

void sub_0812B68C(struct MenuObj *obj)
{
    u8 *base;

    base = gSys;
    *(u32 *)(base + 0x884) = -1;
    sub_08115F54(obj->sub20, 0, 0, 8, 0xFFFF, 0xFFFF, 0, 0);
    *(u16 *)0x02000000 = obj->dispcntCopyA2;
    obj->flag69 = 1;
}

NONMATCH("asm/code_0812A8F8/draw_two_values_dispatch.s", void *draw_two_values_dispatch(struct StatusView *view, void *oam))
{
    u8 a;
    u8 b;
    u8 flag;

    if (view->unkB1 != 0)
    {
        a = gParty[0x30];
        b = gParty[0x6C];
        flag = view->flagsAF & 2;
    }
    else
    {
        a = view->unk84;
        b = view->unk85;
        flag = view->flagsAF & 1;
    }
    return draw_two_values(oam, a, b, flag);
}
END_NONMATCH

void *sub_0812B730(void *obj, u8 kind)
{
    task_insert(obj, kind);
    ((struct MenuObj *)obj)->cb18 = gSaveMenuChildClass;
    return obj;
}

ASM_FUNC("asm/code_0812A8F8/menu_screen_init.s", void menu_screen_init(void))

void menu_screen_destroy(struct MenuObj *obj, u32 arg)
{
    void *tail;

    obj->cb18 = gScreenCallbacks;
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000048 = 0;
    *(u16 *)0x0200004A = 0;
    *(u16 *)0x02000050 = 0;
    dma_cancel_request(0x0600D000);
    run_pending_obj_callbacks();
    if (obj->owner24 != 0)
    {
        owner_destroy(obj->owner24, 3);
    }
    if (obj->block38 != 0)
    {
        heap_free_a(obj->block38);
    }
    if (obj->flagsBD == 0)
    {
        if (obj->unk58 != 0)
        {
            heap_free_a(obj->unk58);
        }
        if (obj->unk54 != 0)
        {
            heap_free_a(obj->unk54);
        }
        tail = obj->unk40;
    }
    else
    {
        if ((obj->flagsBE & 1) == 0)
        {
            goto third;
        }
        if (obj->unk50 != 0)
        {
            heap_free_a(obj->unk50);
        }
        if (obj->unk4C != 0)
        {
            heap_free_a(obj->unk4C);
        }
        if (obj->unk48 != 0)
        {
            heap_free_a(obj->unk48);
        }
        tail = obj->unk44;
    }
    if (tail != 0)
    {
        heap_free_a(tail);
    }
    goto rest;
third:
    if (obj->unk58 != 0)
    {
        heap_free_a(obj->unk58);
    }
    tail = obj->unk40;
    if (tail != 0)
    {
        heap_free_a(tail);
    }
rest:
    heap_free_b(obj->anim28);
    obj->anim28 = 0;
    if (obj->sub20 != 0)
    {
        menu_obj_free(obj->sub20, 3);
    }
    obj->sub20 = 0;
    if (obj->sub1C != 0)
    {
        task_remove(obj->sub1C, 3);
    }
    obj->sub1C = 0;
    task_remove(obj, arg);
}

ASM_FUNC("asm/code_0812A8F8/menu_screen_state_step.s", void menu_screen_state_step(void))
