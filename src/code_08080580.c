#include "global.h"
#include "ram_routines.h"

struct TaskNode
{
    struct TaskNode *next;
    void *fn;
    void *obj;
    void *data;
    u16 timer;
};

struct PartTable
{
    u8 filler0[2];
    u16 index;
};

struct SpritePart
{
    u8 filler0[0x7E];
    u8 mode;
    u8 filler7F[0xCD - 0x7F];
    s8 unkCD;
    u8 filler0CE[0xE4 - 0xCE];
    struct PartTable *table;
    u8 filler0E8[0x110 - 0xE8];
    u8 flags110;
};

struct SubInner
{
    u8 filler0[8];
    void *block8;
};

struct SubState
{
    u8 filler0[0x48];
    struct SubInner *inner48;
};

struct TaskState
{
    struct SubState *sub0;
    u8 filler4[0xA - 4];
    u8 unkA;
    u8 flagsB;
    u8 filler0C[0x3C - 0xC];
    struct TaskNode *freeList;
    struct TaskNode *list40;
    struct TaskNode *list44;
    struct TaskNode *list48;
    struct TaskNode *list4C;
    struct TaskNode *list50;
    u8 filler54[0x58 - 0x54];
    struct SpritePart *parts[8];
    u8 filler78[0x1F4 - 0x78];
    u32 unk1F4;
    u8 filler1F8[0x244 - 0x1F8];
    void *unk244;
    u32 unk248;
    void *unk24C;
    u8 *unk250;
    u8 filler254[0x2B6 - 0x254];
    u16 unk2B6;
    u8 unk2B8;
    u8 filler2B9[0x2BA - 0x2B9];
    u8 unk2BA;
    u8 filler2BB[0x2BE - 0x2BB];
    u8 unk2BE;
    u8 flags2BF;
};

struct RingNode
{
    struct RingNode *next;
    u32 value;
    u8 filler8[8];
    u32 link;
    u8 filler14[0xE4 - 0x14];
    u16 *part;
};

struct RingHead
{
    u8 filler0[0xEC];
    s16 kind;
    u8 filler2[0x111 - 0xEE];
    u8 flags;
};

struct SpriteRec
{
    u8 filler0[0x12];
    u8 state;
};

struct RecBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct SlideData
{
    u8 filler0[4];
    s32 posA;
    s32 posB;
    u8 filler0C[0x12 - 0x0C];
    s16 step;
    u8 filler14[0x16 - 0x14];
    u16 count;
};

struct SpriteObj
{
    u8 filler0[8];
    struct SpriteRec *rec;
    u8 filler0C[0x10 - 0x0C];
    s32 unk10;
    s32 unk14;
    s32 zSum;
    u8 filler1C[0x28 - 0x1C];
    struct RingHead *ring;
    u8 filler2C[0x38 - 0x2C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    void *stateFn;
    u8 filler50[0x79 - 0x50];
    u8 flags;
    u8 filler7A[0x80 - 0x7A];
    u32 checkA;
    u8 filler84[0x94 - 0x84];
    u32 checkB;
    u8 filler98[0xB2 - 0x98];
    u16 stepZ;
};

extern struct TaskState *gUnk_03000FD8;
void task_begin_obj(struct TaskNode *task);
void obj_release(void *obj);
void task_wait_idle(struct TaskNode *task);
void obj_finish_anim(struct SpriteObj *obj);
void task_slide_back(struct TaskNode *task);
void popup_wave_step(void);
u8 motion_step(struct SpriteObj *obj);
u8 advance_height(struct SpriteObj *obj);
u8 step_motion(struct SpriteObj *obj);
void sprite_set_action(struct SpriteObj *obj, u32 action, u32 anim, u32 a3);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 z, struct SpriteObj *owner);
void set_slot_value(u32 id, s32 value);
void set_cur_flags_pending(u32 flags);
void oam_write_tiles(void *tiles, s32 x, s32 y, u32 prio, u32 right);
u8 poll_flag_bd0(void);
u8 is_state_busy(void);
void release_block(void *block);
void set_flags_2_3(void *p);
u32 create_text_entry(void *obj, u32 a, u32 b, u32 c, u32 d);
void setup_bg_entry(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
void reset_root_params(void *obj, u32 p);
void state_clear_mode_bit(void);
void run_obj_entry(u32 arg);
void state_set_flags_and_run(void);
void owner_wait_entry(void);
void state_show_and_scale(void);
void state_random_step(void);
void state_set_anim_and_draw(void);
void state_draw_pose(void);
extern const u8 gTaskPickTable[];
void reset_display(u32 a);
void load_table_entries(u32 code);
void clear_obj_bg_vram(void);
void destroy_map_obj(void *obj, u32 param);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *create_map_object(void *mem, u32 prio, const void *tag, u32 cfg, u32 a, u32 b, u32 ptr);
extern const u8 gMapObjTag[];
void task_init_map(struct TaskNode *task);

NONMATCH("asm/code_08080580/unknown_code_08080580.s", u32 unknown_code_08080580(u32 mode))
{
    u32 count;
    struct SpritePart **entry;
    s32 k;

    count = 0;
    entry = gUnk_03000FD8->parts;
    for (k = 7; k >= 0; k -= 4)
    {
        if (((((u32)entry[0]->mode << 29) >> 30) == mode) && (entry[0]->flags110 & 0x20) == 0)
        {
            count++;
        }
        if (((((u32)entry[1]->mode << 29) >> 30) == mode) && (entry[1]->flags110 & 0x20) == 0)
        {
            count++;
        }
        if (((((u32)entry[2]->mode << 29) >> 30) == mode) && (entry[2]->flags110 & 0x20) == 0)
        {
            count++;
        }
        if (((((u32)entry[3]->mode << 29) >> 30) == mode) && (entry[3]->flags110 & 0x20) == 0)
        {
            count++;
        }
        entry += 4;
    }
    return count;
}
END_NONMATCH

void list_splice_pair(struct RingNode *head_a, struct RingNode *head_b)
{
    struct RingNode *next_a = head_a->next;
    struct RingNode *link_b = head_b->next;
    struct RingNode *node;

    if (next_a == 0 || link_b == 0)
    {
        if (next_a == 0)
        {
            next_a = head_a;
        }
        if (link_b == 0)
        {
            link_b = head_b;
        }
        head_b->next = next_a;
        head_a->next = link_b;
    }

    node = head_a;
    for (;;)
    {
        if (*node->part != 0)
        {
            break;
        }
        node = node->next;
        if (node == head_a)
        {
            return;
        }
    }
    if (*head_a->part == 0)
    {
        head_a->link = node->value;
    }
    if (*head_b->part == 0)
    {
        head_b->link = node->value;
    }
}

void task_list_remove(struct RingNode *head)
{
    if (head != 0)
    {
        struct RingNode *first = head->next;
        u8 *obj = (u8 *)head + 8;

        if (first != 0)
        {
            struct RingNode *node = first;
            struct RingNode *next;

            for (;;)
            {
                if (node->link == head->value)
                {
                    node->link = node->value;
                }
                next = node->next;
                if (next == head)
                {
                    break;
                }
                node = next;
            }
            node->next = first;
            head->next = 0;
        }
        obj_release(obj);
    }
}

void task_spawn_wait_idle(struct SpriteObj *obj)
{
    u8 *flags = &obj->flags;
    struct TaskState *st;
    struct TaskNode *node;
    void *fn;

    *flags |= 0x10;
    st = gUnk_03000FD8;
    fn = (void *)task_wait_idle;
    node = st->freeList;
    st->freeList = node->next;
    node->next = st->list4C;
    node->fn = fn;
    node->timer = 0;
    st->list4C = node;
    node->obj = obj;
}

NONMATCH("asm/code_08080580/task_wait_idle.s", void task_wait_idle(struct TaskNode *task))
{
    u8 *flags;
    struct SpriteObj *obj = task->obj;
    u8 busy;

    flags = &obj->flags;
    {
        s32 f = *flags;

        f &= -0x11;
        *flags = f;
    }
    if (obj->checkA != 0)
    {
        if (motion_step(obj) == 0)
        {
            *flags |= 0x10;
        }
    }
    if (obj->checkB != 0)
    {
        if (advance_height(obj) == 0)
        {
            obj->flags |= 0x10;
        }
    }
    busy = obj->flags & 0x10;
    if (busy == 0)
    {
        task->fn = (void *)(u32)busy;
    }
}
END_NONMATCH

struct TaskNode *task_push_list_50(void *next, void *obj)
{
    struct TaskState *st = gUnk_03000FD8;
    struct TaskNode *node = st->freeList;

    st->freeList = node->next;
    node->next = st->list50;
    node->fn = next;
    node->timer = 0;
    st->list50 = node;
    node->obj = obj;
    return node;
}

struct TaskNode *task_create_list_4c(void *next, void *obj)
{
    struct TaskState *st = gUnk_03000FD8;
    struct TaskNode *node = st->freeList;

    st->freeList = node->next;
    node->next = st->list4C;
    node->fn = next;
    node->timer = 0;
    st->list4C = node;
    node->obj = obj;
    return node;
}

struct TaskNode *task_create_list_48(void *next, void *obj)
{
    struct TaskState *st = gUnk_03000FD8;
    struct TaskNode *node = st->freeList;

    st->freeList = node->next;
    node->next = st->list48;
    node->fn = next;
    node->timer = 0;
    st->list48 = node;
    node->obj = obj;
    return node;
}

struct TaskNode *task_create_list_44(void *next)
{
    struct TaskState *st = gUnk_03000FD8;
    struct TaskNode *node = st->freeList;

    st->freeList = node->next;
    node->next = st->list44;
    node->fn = next;
    node->timer = 0;
    st->list44 = node;
    return node;
}

struct TaskNode *task_create_list_40(void *next)
{
    struct TaskState *st = gUnk_03000FD8;
    struct TaskNode *node = st->freeList;

    st->freeList = node->next;
    node->next = st->list40;
    node->fn = next;
    node->timer = 0;
    st->list40 = node;
    return node;
}

struct TaskNode *list_add_task(struct TaskNode **list, void *next)
{
    struct TaskState *st = gUnk_03000FD8;
    struct TaskNode *node = st->freeList;

    st->freeList = node->next;
    node->next = *list;
    node->fn = next;
    node->timer = 0;
    *list = node;
    return node;
}

NONMATCH("asm/code_08080580/run_task_list.s", void run_task_list(struct TaskNode **list))
{
    struct TaskNode *prev;
    struct TaskNode *node;
    struct TaskNode *next;

    prev = 0;
    node = *list;
    while (node != 0)
    {
        next = node->next;

        if (node->fn != 0)
        {
            ((void (*)(struct TaskNode *))node->fn)(node);
        }
        if (node->fn != 0)
        {
            prev = node;
        }
        else
        {
            if (prev != 0)
            {
                prev->next = next;
            }
            else if (*list == node)
            {
                *list = next;
            }
            else
            {
                struct TaskNode *cur = *list;
                struct TaskNode *before;

                do
                {
                    before = cur;
                    cur = before->next;
                } while (cur != node);
                before->next = next;
            }
            {
                struct TaskState *st = gUnk_03000FD8;

                node->next = st->freeList;
                st->freeList = node;
            }
        }
        node = next;
    }
}
END_NONMATCH

void obj_start_anim(struct SpriteObj *obj)
{
    u32 anim;

    if (step_motion(obj) == 0)
    {
        s32 kind = obj->ring->kind;

        if (kind == -1)
        {
            anim = 0x2039;
        }
        else
        {
            anim = 0x2068;
        }
        sprite_set_action(obj, 0xC, anim, 0);
        ((struct RecBits *)&obj->rec->state)->mode = 1;
        effect_spawn_at(0x33, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->stateFn = (void *)obj_finish_anim;
    }
}

void task_finish_anim(struct TaskNode *task)
{
    struct SpriteObj *obj = task->obj;

    if ((obj->rec->state & 8) != 0)
    {
        if (obj->ring->kind == -1)
        {
            sprite_set_action(obj, 1, 0x2039, 0);
            set_slot_value(0x14A, -1);
        }
        else
        {
            sprite_set_action(obj, 1, 0x2068, 0);
            set_slot_value(0x14E, -1);
        }
        ((struct RecBits *)&obj->rec->state)->mode = 0;
        set_cur_flags_pending(0x80000);
        ((struct RecBits *)&obj->ring->flags)->unk0 = 0;
        task->fn = 0;
    }
}

u8 obj_step_z(struct SpriteObj *obj)
{
    u8 result;

    if (obj->posZ <= 0)
    {
        result = 0;
    }
    else
    {
        s32 sum;

        obj->stepZ -= 0x5E;
        sum = obj->posZ + (s16)obj->stepZ;
        obj->zSum = sum;
        if (sum <= 0)
        {
            obj->stepZ = 0;
            obj->zSum = 0;
            result = 0;
        }
        else
        {
            result = 1;
        }
    }
    return result;
}

void task_step_z(struct TaskNode *task)
{
    if (obj_step_z(task->obj) == 0)
    {
        task_begin_obj(task);
    }
}

void task_slide_step(struct TaskNode *task)
{
    struct SlideData *data = task->data;

    data->posB += data->step;
    oam_write_tiles(task->obj, data->posA / 0x100, data->posB / 0x100, 0xE, 0);
    {
        u8 *flagsB = &gUnk_03000FD8->flagsB;

        *flagsB |= 2;
    }
    data->count -= 1;
    if (data->count == 0)
    {
        data->count = 0x1E;
        task->fn = (void *)task_slide_back;
    }
}

void obj_finish_anim(struct SpriteObj *obj)
{
    if ((obj->rec->state & 8) != 0)
    {
        if (obj->ring->kind == -1)
        {
            sprite_set_action(obj, 1, 0x2039, 0);
        }
        else
        {
            sprite_set_action(obj, 1, 0x2068, 0);
        }
        ((struct RecBits *)&obj->rec->state)->mode = 0;
        obj->stateFn = 0;
    }
}

void task_slide_back(struct TaskNode *task)
{
    struct SlideData *data = task->data;

    oam_write_tiles(task->obj, data->posA / 0x100, data->posB / 0x100, 0xE, 0);
    {
        u8 *flagsB = &gUnk_03000FD8->flagsB;

        *flagsB |= 2;
    }
    if ((s16)data->count > 0)
    {
        data->count = data->count - 1;
    }
    else
    {
        data->step = 0;
        data->count = 0x16;
        task->fn = (void *)popup_wave_step;
    }
}

ASM_FUNC("asm/code_08080580/task_init_sprites.s", void task_init_sprites(void))

void task_wait_poll_flag(struct TaskNode *task)
{
    if (poll_flag_bd0() == 0)
    {
        void **slot;
        void *mem;
        void *obj;
        u8 mode;

        reset_display((u32)gUnk_03000FD8->unk250);
        clear_obj_bg_vram();
        if (gUnk_03000FD8->unk24C != 0)
        {
            destroy_map_obj(gUnk_03000FD8->unk24C, 3);
        }
        slot = &gUnk_03000FD8->unk24C;
        mem = heap_alloc2(0x77C, 0, gMapObjTag, 0);
        mode = gUnk_03000FD8->unk2B8;
        obj = create_map_object(mem, 9, gMapObjTag, mode, 0x10, 0x50, gUnk_03000FD8->unk248);
        *slot = obj;
        task->fn = (void *)task_init_map;
    }
}

ASM_FUNC("asm/code_08080580/task_init_map.s", void task_init_map(struct TaskNode *task))

ASM_FUNC("asm/code_08080580/sprite_update_parts.s", void sprite_update_parts(void))

ASM_FUNC("asm/code_08080580/task_run_parts.s", void task_run_parts(void))

ASM_FUNC("asm/code_08080580/task_spawn_objects.s", void task_spawn_objects(void))

ASM_FUNC("asm/code_08080580/task_load_part_codes.s", void task_load_part_codes(void))

ASM_FUNC("asm/code_08080580/task_free_blocks.s", void task_free_blocks(void))

ASM_FUNC("asm/code_08080580/task_state_machine.s", void task_state_machine(void))

void sprite_load_active_tables(void)
{
    s32 i;

    load_table_entries(0x17B);
    for (i = 0; i <= 7; i += 2)
    {
        if (gUnk_03000FD8->unk1F4 != 0 && i >= 0 && i <= 5)
        {
            struct SpritePart *part = gUnk_03000FD8->parts[i];
            u8 m = part->mode & 6;

            if (m == 4 || m == 2)
            {
                if (part->table != 0)
                {
                    load_table_entries(part->table->index);
                }
            }
        }
        {
            s32 j = i + 1;

            if (gUnk_03000FD8->unk1F4 != 0 && j >= 0 && j <= 5)
            {
                struct SpritePart *part = gUnk_03000FD8->parts[j];
                u8 m = part->mode & 6;

                if (m == 4 || m == 2)
                {
                    if (part->table != 0)
                    {
                        load_table_entries(part->table->index);
                    }
                }
            }
        }
    }
}

NONMATCH("asm/code_08080580/sprite_sum_parts.s", void sprite_sum_parts(struct SpriteObj *obj))
{
    s32 sumX = 0;
    s32 sumY = 0;
    s32 top = 0;
    s32 count = 0;
    u8 **routines = &gMathRoutines;
    struct SpritePart **entry = gUnk_03000FD8->parts;
    s32 k;

    for (k = 5; k >= 0; k -= 3)
    {
        struct SpritePart *part;
        struct SpriteObj *pose;
        s32 h;

        part = entry[0];
        pose = (struct SpriteObj *)((u8 *)part + 8);
        if ((part->mode & 6) == 4)
        {
            sumX += pose->posX;
            sumY += pose->posY;
            h = pose->posZ + ((s32)part->unkCD << 8);
            if (top < h)
            {
                top = h;
            }
            count++;
        }
        part = entry[1];
        pose = (struct SpriteObj *)((u8 *)part + 8);
        if ((part->mode & 6) == 4)
        {
            sumX += pose->posX;
            sumY += pose->posY;
            h = pose->posZ + ((s32)part->unkCD << 8);
            if (top < h)
            {
                top = h;
            }
            count++;
        }
        part = entry[2];
        pose = (struct SpriteObj *)((u8 *)part + 8);
        if ((part->mode & 6) == 4)
        {
            sumX += pose->posX;
            sumY += pose->posY;
            h = pose->posZ + ((s32)part->unkCD << 8);
            if (top < h)
            {
                top = h;
            }
            count++;
        }
        entry += 3;
    }
    sumX = ((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(sumX, count);
    sumY = ((s32 (*)(s32, s32))(*routines + ((u32)iwram_div - (u32)iwram_sqrt)))(sumY, count);
    obj->unk10 = sumX;
    obj->posX = sumX;
    obj->unk14 = sumY;
    obj->posY = sumY;
    obj->zSum = top;
    obj->posZ = top;
}
END_NONMATCH
