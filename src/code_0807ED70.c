#include "global.h"

struct SpriteNode;
struct SpriteObj;

struct OamRow
{
    u16 hw0;
    u16 hw2;
    u16 hw4;
    u16 hw6;
    u8 filler8[8];
    struct OamRow *next;
};

struct SpriteState
{
    u8 filler0[0x0B];
    u8 flags0B;
    u8 flags0C;
    u8 filler0D[0x54 - 0x0D];
    struct SpriteObj *objs;
    u8 filler58[0x238 - 0x58];
    struct SpriteNode *nodeFree;
    struct SpriteNode *nodeUsed;
    struct SpriteNode *nodePool;
    u8 filler244[0x248 - 0x244];
    void *display;
    void *camera;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u32 unk4;
    u32 unk8;
    union
    {
        u8 flags0C;
        u32 unkC;
    } u0C;
};

struct SpriteRecord
{
    u8 filler0[0x12];
    u8 flags12;
};

struct SpriteObj
{
    u8 filler0[0x08];
    struct SpriteRecord *record;
    u8 filler0C[0x13 - 0x0C];
    u8 flags13;
    u8 filler14[0x18 - 0x14];
    s32 unk18;
    u8 filler1C[0x34 - 0x1C];
    struct SpriteObj *activeNext;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x58 - 0x44];
    u32 slots58[4];
    u8 filler68[0x76 - 0x68];
    u8 flags76;
    u8 flags77;
};

struct TaskData
{
    u8 filler0[0x04];
    s32 posX;
    s32 posY;
    u8 filler0C[0x16 - 0x0C];
    u16 count;
    s16 posZ;
};

struct Task
{
    struct Task *next;
    void *fn;
    struct SpriteObj *obj;
    struct TaskData *data;
    s16 timer;
};

extern struct SpriteState *gUnk_03000FD8;
extern u8 *gWorkBuf;
extern u8 *gMemRoutines;
extern u16 gUnk_0300034C;

void sub_08115F54(void *display, u32 kind, u32 x, u32 y, u32 a, u32 b, u32 c, u32 d);
void sub_08117380(void *display);
void sub_0811733C(void *display, u32 value);
void write_screen_entry(void *camera, u32 value);
void obj_bind_slots(void *obj);
void list_remove_active(void *obj);
void list_push_active(void *obj);
void release_obj_131c(void *obj);
void offset_words_by_camera(void *camera, u32 a, u32 b, u32 d, u32 e);
void offset_three_words(void *camera, u32 a, u32 b, u32 d, u32 e);
void write_entry_offsets(void *camera, u32 a, u32 b, u32 d, u32 e, u32 f);
void partner_link_update(void);
void lookup_part(void);
void lookup_link_slot(void);
void sprite_set_action(void *obj, u32 a, u32 b, u32 c);
s32 number_right_edge(u32 tiles);
void oam_write_tiles(u32 tiles, s32 x, s32 y, u32 prio, u32 right);
void setup_obj_10bc(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
void obj_load_record(void *obj, u32 a, u32 b, u32 c, u32 d);
s32 obj_step_z(void *obj);
void task_begin_obj(void *task);
void task_step_z(void);
void task_slide_step(void);
void popup_rise_step(void);
void popup_rise_step_b(void);

struct Lo77
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};

struct NodeDesc
{
    u32 unk0;
    struct SpriteNode *node;
    u32 value;
    u32 unkC;
};

ASM_FUNC("asm/code_0807ED70/sprite_objects_draw_obj_tiles.s", void sprite_objects_draw_obj_tiles(void))

ASM_FUNC("asm/code_0807ED70/set_body_part.s", void set_body_part(void))

ASM_FUNC("asm/code_0807ED70/obj_free_parts.s", void obj_free_parts(void))

ASM_FUNC("asm/code_0807ED70/body_set_pose.s", void body_set_pose(void))

ASM_FUNC("asm/code_0807ED70/hero_select_index.s", void hero_select_index(void))

ASM_FUNC("asm/code_0807ED70/sprite_objects_obj_show_at.s", void sprite_objects_obj_show_at(void))

ASM_FUNC("asm/code_0807ED70/display_flush.s", void display_flush(void))

void obj_list_remove(struct SpriteObj *obj)
{
    struct SpriteObj *prev = NULL;
    struct SpriteObj *cur = gUnk_03000FD8->objs;

    while (cur != NULL)
    {
        struct SpriteObj *next = cur->activeNext;
        if (cur == obj)
        {
            if (prev != NULL)
                prev->activeNext = next;
            else
                gUnk_03000FD8->objs = next;
            obj->slots58[0] = 0;
            obj->slots58[1] = 0;
            obj->slots58[2] = 0;
            obj->slots58[3] = 0;
            obj->activeNext = NULL;
            return;
        }
        prev = cur;
        cur = next;
    }
}

ASM_FUNC("asm/code_0807ED70/obj_list_push_arg.s", void obj_list_push_arg(void))

void obj_list_push_default(struct SpriteObj *obj)
{
    if (obj->activeNext == NULL)
    {
        obj->activeNext = gUnk_03000FD8->objs;
        gUnk_03000FD8->objs = obj;
        obj->slots58[0] = (u32)partner_link_update;
        obj->slots58[1] = (u32)lookup_part;
        obj->slots58[2] = (u32)lookup_link_slot;
        ((struct Lo77 *)&obj->flags77)->lo3 = 0;
    }
}

ASM_FUNC("asm/code_0807ED70/obj_start_pose.s", void obj_start_pose(void))

ASM_FUNC("asm/code_0807ED70/obj_update_pose.s", void obj_update_pose(void))

void obj_step_task(struct Task *task)
{
    struct SpriteObj *obj = task->obj;
    u8 done = obj_step_z(obj);

    if (obj->record->flags12 & 8)
    {
        if (done == 0)
            task_begin_obj(task);
        else
            task->fn = (void *)task_step_z;
    }
}

ASM_FUNC("asm/code_0807ED70/obj_timer_task.s", void obj_timer_task(void))

void display_pause(void)
{
    sub_08117380(gUnk_03000FD8->display);
}

void display_finish(u32 value)
{
    sub_0811733C(gUnk_03000FD8->display, value);
}

void draw_sprite_block(u32 kind, u32 x, u32 y, u32 w, u32 h)
{
    u16 px = x;
    u16 pw = w;
    u16 ph = h;

    sub_08115F54(gUnk_03000FD8->display, (u8)kind, px, (u16)y, pw, ph, 0, 0);
}

void draw_sprite_small(u32 kind, u32 x, u32 y)
{
    u16 px = x;

    sub_08115F54(gUnk_03000FD8->display, (u8)kind, px, (u16)y, 0xFF, gUnk_0300034C & 0x6000, 0, 0);
}

u16 number_popup_raster(void)
{
    return *(vu16 *)REG_ADDR_DISPSTAT & 2;
}

void sub_0807FFF4(void)
{
}

ASM_FUNC("asm/code_0807ED70/oam_write_tiles.s", void oam_write_tiles(u32 tiles, s32 x, s32 y, u32 prio, u32 right))

void task_anim_countdown(struct Task *task)
{
    struct TaskData *data = task->data;

    {
        int b = gUnk_03000FD8->flags0B;

        b |= 2;
        gUnk_03000FD8->flags0B = b;
    }
    {
        s32 count = data->count - 1;

        data->count = count;
        if ((u16)count == 0)
        {
            data->count = 0x14;
            task->fn = (void *)task_slide_step;
        }
    }
}

void task_draw_step_a(struct Task *task)
{
    struct TaskData *data = task->data;
    struct SpriteObj *obj = task->obj;

    oam_write_tiles((u32)obj, data->posX / 0x100, data->posY / 0x100, data->posZ, 0);
    {
        int b = gUnk_03000FD8->flags0B;

        b |= 2;
        gUnk_03000FD8->flags0B = b;
    }
    {
        s32 count = data->count - 1;

        data->count = count;
        if ((u16)count == 0)
        {
            data->count = 0xE;
            task->fn = (void *)(popup_rise_step_b);
        }
    }
}

void task_draw_step_b(struct Task *task)
{
    struct TaskData *data = task->data;
    struct SpriteObj *obj = task->obj;

    oam_write_tiles((u32)obj, data->posX / 0x100, data->posY / 0x100, data->posZ, 0);
    {
        int b = gUnk_03000FD8->flags0B;

        b |= 2;
        gUnk_03000FD8->flags0B = b;
    }
    {
        s32 count = data->count - 1;

        data->count = count;
        if ((u16)count == 0)
        {
            data->count = 0xE;
            task->fn = (void *)(popup_rise_step);
        }
    }
}

ASM_FUNC("asm/code_0807ED70/oam_list_rows_a.s", void oam_list_rows_a(void))

ASM_FUNC("asm/code_0807ED70/oam_list_rows_b.s", void oam_list_rows_b(void))

ASM_FUNC("asm/code_0807ED70/sprite_node_take_link.s", void sprite_node_take_link(void))

ASM_FUNC("asm/code_0807ED70/sprite_node_take.s", void sprite_node_take(void))

u8 *work_buf_entry_addr(u32 index)
{
    u8 **buf = &gWorkBuf;
    u32 offset = index * 0xD0;
    offset += 0x980;
    return *buf + offset;
}

ASM_FUNC("asm/code_0807ED70/sprite_node_release.s", void sprite_node_release(void))

struct SpriteNode *sprite_node_alloc(u32 a, u32 b, u32 d)
{
    struct SpriteNode *node = gUnk_03000FD8->nodePool;

    gUnk_03000FD8->nodePool = node->next;
    ((void (*)(u32, struct SpriteNode *, u32))gMemRoutines)(0, node, 0x1C);
    node->unk4 = a;
    node->unk8 = b;
    node->u0C.unkC = d;
    return node;
}

struct SpriteNode *sprite_node_alloc_default(void)
{
    struct SpriteNode *node = gUnk_03000FD8->nodePool;

    gUnk_03000FD8->nodePool = node->next;
    ((void (*)(u32, struct SpriteNode *, u32))gMemRoutines)(0, node, 0x1C);
    return node;
}

ASM_FUNC("asm/code_0807ED70/obj_release_node.s", void obj_release_node(void))

void obj_detach_active(void *obj)
{
    obj_bind_slots(obj);
    list_remove_active(obj);
}

ASM_FUNC("asm/code_0807ED70/obj_setup_load.s", void obj_setup_load(void))

void offset_words_camera(u32 a, u32 b, u32 d, u32 e)
{
    offset_words_by_camera(gUnk_03000FD8->camera, a, b, d, e);
}

void offset_three_words_camera(u32 a, u32 b, u32 d, u32 e)
{
    offset_three_words(gUnk_03000FD8->camera, a, b, d, e);
}

void unknown_code_08080474(u32 value)
{
    write_screen_entry(gUnk_03000FD8->camera, value);
}

void write_entry_offsets_camera(u32 a, u32 b, u32 d, u32 e, u32 f)
{
    write_entry_offsets(gUnk_03000FD8->camera, a, b, d, e, f);
}

ASM_FUNC("asm/code_0807ED70/find_slot_by_kind.s", void find_slot_by_kind(void))
