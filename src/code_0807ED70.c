#include "global.h"
#include "ram_routines.h"

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
    u8 filler0[0x0A];
    u8 flags0A;
    u8 flags0B;
    u8 flags0C;
    u8 filler0D[0x54 - 0x0D];
    struct SpriteObj *objs;
    u8 filler58[0x228 - 0x58];
    struct SpriteObj *partsB[2];
    struct SpriteObj *partsA[2];
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
    u16 x;
    u16 y;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u8 flags12;
    u8 filler13[0x1F - 0x13];
    u8 flags1F;
};

struct SpriteObj
{
    u16 x;
    u16 y;
    u8 filler4[0x08 - 0x04];
    struct SpriteRecord *record;
    u8 filler0C[0x0E - 0x0C];
    u16 unkE;
    u8 filler10[0x13 - 0x10];
    u8 flags13;
    u8 filler14[0x16 - 0x14];
    u16 unk16;
    s32 unk18;
    u8 filler1C[0x1E - 0x1C];
    u8 flags1E;
    u8 flags1F;
    u8 filler20[0x21 - 0x20];
    u8 unk21;
    u8 filler22[0x28 - 0x22];
    void *ring;
    u8 filler2C[0x34 - 0x2C];
    struct SpriteObj *activeNext;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    void *stateFn;
    u8 filler50[0x58 - 0x50];
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
extern u8 *gUnk_03000FF4;
void mem_copy_fast(void *dst, const void *src, u32 n);

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
void place_targets(struct SpriteObj *obj, s32 col, s32 row, s32 z, s32 extra);
void set_step_and_offsets(struct SpriteObj *obj, s32 a, s32 b);
void obj_start_anim(struct SpriteObj *obj);
void oam_write_tiles(u32 tiles, s32 x, s32 y, u32 prio, u32 right);
void setup_obj_10bc(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
void obj_load_record(void *obj, u32 a, u32 b, u32 c, u32 d);
s32 obj_step_z(void *obj);
void task_begin_obj(void *task);
void task_step_z(void);
void task_slide_step(void);
void popup_rise_step(void);
void popup_rise_step_b(void);
u8 step_motion(void *obj);
void effect_spawn_at(u32 kind, s32 x, s32 y, s32 z, void *obj);
void task_finish_anim(void);

struct Lo77
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};

struct Lo5
{
    u8 lo5 : 5;
    u8 hi3 : 3;
};

struct Bits13
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

struct Bits0C
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 hi6 : 6;
};

struct Bits76
{
    u8 lo3 : 3;
    u8 mid3 : 3;
    u8 hi2 : 2;
};

struct Bits12
{
    u8 bit0 : 1;
    u8 mid2 : 2;
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

void obj_free_parts(struct SpriteObj *obj)
{
    struct SpriteObj *part;

    part = gUnk_03000FD8->partsA[*(s16 *)((u8 *)obj->ring + 0xEC) + 2];
    if (part != NULL)
    {
        obj_bind_slots(part);
        ((struct Bits13 *)&part->flags13)->bit2 = 0;
        ((struct Bits13 *)&part->flags13)->bit3 = 0;
        list_push_active(part);
        release_obj_131c(part);
        gUnk_03000FD8->partsA[*(s16 *)((u8 *)obj->ring + 0xEC) + 2] = NULL;
    }
    part = gUnk_03000FD8->partsB[*(s16 *)((u8 *)obj->ring + 0xEC) + 2];
    if (part != NULL)
    {
        obj_bind_slots(part);
        ((struct Bits13 *)&part->flags13)->bit2 = 0;
        ((struct Bits13 *)&part->flags13)->bit3 = 0;
        list_push_active(part);
        release_obj_131c(part);
        gUnk_03000FD8->partsB[*(s16 *)((u8 *)obj->ring + 0xEC) + 2] = NULL;
    }
}

ASM_FUNC("asm/code_0807ED70/body_set_pose.s", void body_set_pose(void))

extern u8 *gUnk_03000FF4;
void mem_copy_fast(void *dst, const void *src, u32 n);
ASM_FUNC("asm/code_0807ED70/hero_select_index.s", void hero_select_index(void))

ASM_FUNC("asm/code_0807ED70/sprite_objects_obj_show_at.s", void sprite_objects_obj_show_at(void))

void display_flush(void)
{
    if (gUnk_03000FD8->flags0C & 2)
    {
        sub_0811733C(gUnk_03000FD8->display, 1);
        ((struct Bits0C *)&gUnk_03000FD8->flags0C)->bit1 = 0;
    }
}

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

void obj_list_push_arg(struct SpriteObj *obj, u32 arg)
{
    if (obj->activeNext == NULL)
    {
        obj->activeNext = gUnk_03000FD8->objs;
        gUnk_03000FD8->objs = obj;
        obj->slots58[0] = (u32)partner_link_update;
        obj->slots58[1] = arg;
        obj->slots58[2] = arg;
        obj->slots58[3] = arg;
        ((struct Lo77 *)&obj->flags77)->lo3 = 0;
    }
}

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

void obj_start_pose(struct SpriteObj *obj)
{
    sprite_set_action(obj, 0xB, *(s16 *)((u8 *)obj->ring + 0xEC) == -1 ? 0x2039 : 0x2068, 0);
    ((struct Bits12 *)&obj->record->flags12)->mid2 = 1;
    place_targets(obj, obj->posX / 0x100 - 0x18, obj->posY / 0x100 + 6, 0, 0x100);
    set_step_and_offsets(obj, 0x400, 0x10);
    obj->stateFn = (void *)obj_start_anim;
}

void obj_update_pose(struct Task *task)
{
    struct SpriteObj *obj = task->obj;

    if (step_motion(obj) == 0)
    {
        if (*(s16 *)((u8 *)obj->ring + 0xEC) == -1)
            sprite_set_action(obj, 0, 0x2039, 0);
        else
            sprite_set_action(obj, 0, 0x2068, 0);
        ((struct Bits12 *)&obj->record->flags12)->mid2 = 1;
        effect_spawn_at(0x33, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        task->fn = (void *)task_finish_anim;
    }
}

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

void obj_timer_task(struct Task *task)
{
    struct SpriteObj *obj = task->obj;
    u16 timer = task->timer;

    if (task->timer > 0)
    {
        s32 t = timer - 1;

        task->timer = t;
        if ((t << 16) == 0)
            ((struct Bits76 *)&obj->flags76)->mid3 = 0;
    }
    else if (obj->posZ >= obj->unk18 && obj->posZ <= 0xC00)
    {
        sprite_set_action(obj, 0, 0, 0);
        task->fn = NULL;
    }
}

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

void oam_list_rows_b(struct OamRow *row, s32 count)
{
    u16 *dst;
    u16 v;

    dst = (u16 *)(gWorkBuf + 0x37E8);
    while (--count != -1)
    {
        v = row->hw0;
        *dst++ = (u8)v - 0x50;
        *dst++ = v & 0xFF00;
        v = row->hw2;
        *dst++ = (v & 0x1FF) - 0x78;
        *dst++ = v & 0xFE00;
        *dst++ = row->hw4;
        row->hw6 = 1;
        row = row->next;
    }
}

struct SpriteNode *sprite_node_take_link(struct NodeDesc *desc)
{
    struct SpriteState *st;
    struct SpriteNode *node;

    st = gUnk_03000FD8;
    node = st->nodeFree;
    st->nodeFree = node->next;
    node->next = st->nodeUsed;
    st->nodeUsed = node;
    ((struct Lo5 *)&node->u0C.flags0C)->lo5 = 0;
    desc->node = node;
    node->unk4 = desc->value;
    node->unk8 = (u32)&desc->unkC;
    return node;
}

struct SpriteNode *sprite_node_take(void)
{
    struct SpriteState *st = gUnk_03000FD8;
    struct SpriteNode *node = st->nodeFree;

    st->nodeFree = node->next;
    node->next = st->nodeUsed;
    st->nodeUsed = node;
    ((struct Lo5 *)&node->u0C.flags0C)->lo5 = 0;
    return node;
}

u8 *work_buf_entry_addr(u32 index)
{
    u8 **buf = &gWorkBuf;
    u32 offset = index * 0xD0;
    offset += 0x980;
    return *buf + offset;
}

s32 sprite_node_release(struct SpriteNode *node)
{
    s32 ret;

    if (node == NULL)
        return 0;
    ret = (s32)node->next;
    node->next = gUnk_03000FD8->nodePool;
    gUnk_03000FD8->nodePool = node;
    return ret;
}

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

void obj_release_node(struct SpriteObj *obj)
{
    if (obj != NULL)
    {
        obj_bind_slots(obj);
        ((struct Bits13 *)&obj->flags13)->bit2 = 0;
        ((struct Bits13 *)&obj->flags13)->bit3 = 0;
        list_push_active(obj);
        release_obj_131c(obj);
    }
}

void obj_detach_active(void *obj)
{
    obj_bind_slots(obj);
    list_remove_active(obj);
}

void obj_setup_load(struct SpriteObj *obj, u32 arg)
{
    obj_bind_slots(obj);
    ((struct Bits13 *)&obj->flags13)->bit2 = 0;
    ((struct Bits13 *)&obj->flags13)->bit3 = 0;
    setup_obj_10bc(obj, 0, arg, 1, -1, -1, -1, -1);
    ((struct Bits13 *)&obj->flags13)->bit2 = 1;
    ((struct Bits13 *)&obj->flags13)->bit3 = 1;
    obj_load_record(obj, 0, -1, 0, 0);
    list_remove_active(obj);
}

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
