#include "global.h"
#include "ram_routines.h"

struct RawKeys
{
    u8 filler0[0x28];
    u16 keys;
    u16 keys2A;
};

extern struct RawKeys gSys;

extern const u8 gStackClass[];
extern const u8 gHeapTagStackPart1[];
extern const u8 gHeapTagStackPart2[];
extern const u8 gHeapTagStackPart3[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *obj_init_big(void *obj, u32 first);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void list_insert(void *list, void *node);
void write_var(u16 id, u16 value);
void tween_groups_set_flag_bit(u16 index, u8 value);
void timer_set(void *timer, u32 limit);
void anim_task_start(void *task, u32 a, const void *table, u32 b, u32 c);
extern const u8 gHeapTagAnimTaskData[];

struct ChildPart
{
    u8 filler0[0xC];
    u32 classC;
    u8 *sprite;
    u8 filler14[0x18 - 0x14];
    s32 x;
    s32 y;
    u8 filler20[0x6C - 0x20];
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 filler7C[0x84 - 0x7C];
    s32 unk84;
    u8 filler88[0x8C - 0x88];
    s32 unk8C;
};

struct StageClass
{
    u8 filler0[0x10];
    s16 thisOffset;
    u16 filler12;
    void (*behavior)(void *, u32);
};

struct SpawnAnim
{
    void *unk0;
    void *unk4;
    u8 filler8[0x18 - 0x8];
    s32 unk18;
};

struct SpawnNode2C
{
    u8 filler0[0x88];
    s32 unk88;
};

struct PuffNode
{
    u8 filler0[0xC];
    u32 classC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 filler24[0x6C - 0x24];
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
};

struct StageObj
{
    u8 filler0[0x18];
    struct StageClass *cls;
    void *list1C;
    u8 filler20[0x28 - 0x20];
    void *klass28;
    struct SpawnNode2C *node2C;
    struct ChildPart *part30;
    struct ChildPart *part34;
    struct PlacedObj *placed38;
    struct SpawnAnim *anim3C;
    void *shadowSrc;
    struct ChildPart *part44;
    struct ChildPart *part48;
    s32 timer4C;
    s32 unk50;
    s32 stage;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    u16 unk68;
    u16 unk6A;
    u16 unk6C;
};

extern u16 gUnk_02000050[];

struct PlacedObj
{
    u8 filler0[0x18];
    s32 unk18;
    u8 filler1C[0x34 - 0x1C];
    s32 unk34;
    u8 filler38[0x50 - 0x38];
    s32 unk50;
};

struct ChildObj
{
    s32 kind;
    u8 filler4[0x8 - 0x4];
    struct ChildObj *next;
    u32 classC;
    u8 *sprite;
    u8 filler14[0x18 - 0x14];
    s32 unk18;
    s32 unk1C;
    u8 filler20[0x2C - 0x20];
    s32 unk2C;
    struct ChildPart *part30;
    struct ChildPart *part34;
    struct PlacedObj *placed38;
    u8 filler3C[0x48 - 0x3C];
    struct ChildPart *part48;
    u8 filler4C[0x60 - 0x4C];
    s32 unk60;
    s32 unk64;
    u8 filler68[0x6C - 0x68];
    struct ChildPart *part6C;
    struct ChildPart *part70;
    struct ChildPart *part74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
};
void obj_sync_children_pos(struct ChildObj *obj);
void obj_step_bounce(struct ChildPart *part);
void obj_set_flag88_slot97(struct ChildObj *obj);
void obj_clear_flag88_slot97(struct ChildObj *obj);
void obj_aim_children(struct ChildObj *obj);
void obj_step_stage2(struct StageObj *obj);
void obj_spawn_phase(struct StageObj *obj);
void obj_step_stage4(struct StageObj *obj);
u32 rand_scaled(u32 a, u32 b);
extern const u8 gChaseReturnTable[];
void set_slot_value(u32 id, s32 value);
void obj_place_children(struct PlacedObj *p, u32 mode);
void obj_step_child_speeds(struct ChildObj *obj);
void obj_move_child_step(struct ChildObj *obj);
s32 obj_chase_step(struct ChildObj *node, s32 a1, s32 a2, void *a3, void *a4);
u32 timer_tick(void *timer);
void palette_fill(u32 v);
void slot_reset(u32 idx);
void set_slot_target(u32 a, u32 b, u32 c);
void obj_copy_pos_to_shadow(void *src);
void obj_tween_state(struct StageObj *obj);
void obj_step_children(struct ChildObj *obj);

ASM_FUNC("asm/code_0816CA84/obj_create_pair_children.s", void obj_create_pair_children(void))

void obj_step_child_speeds(struct ChildObj *obj)
{
    struct ChildPart *part;
    s32 v;

    if (gSys.keys & 2)
    {
        part = obj->part6C;
        v = part->unk70 + 0x20;
        part->unk70 = v;
        if (v > 0x100)
            part->unk70 = 0x100;
        obj_step_bounce(part);
    }
    else
    {
        part = obj->part6C;
        v = part->unk70 - 0x20;
        part->unk70 = v;
        if (v < -0x200)
            part->unk70 = -0x200;
        obj_step_bounce(part);
    }

    if (gSys.keys & 1)
    {
        part = obj->part70;
        v = part->unk70 + 0x20;
        part->unk70 = v;
        if (v > 0x100)
            part->unk70 = 0x100;
        obj_step_bounce(part);
    }
    else
    {
        part = obj->part70;
        v = part->unk70 - 0x20;
        part->unk70 = v;
        if (v < -0x200)
            part->unk70 = -0x200;
        obj_step_bounce(part);
    }

    if (gSys.keys & 3)
        obj_set_flag88_slot97(obj);
    else
        obj_clear_flag88_slot97(obj);
    obj_aim_children(obj);
}

ASM_FUNC("asm/code_0816CA84/obj_aim_children.s", void obj_aim_children(struct ChildObj *obj))

struct ChildObj *obj_create_stack(struct ChildObj *obj, void *list, s32 x, s32 y, s32 z)
{
    u8 *sprA;
    u8 *sprB;

    obj_init_big(obj, 2);
    obj->classC = (u32)gStackClass;
    obj->part6C = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagStackPart1, 0), 3);
    list_insert(list, obj->part6C);
    obj_create_sprite_10(obj->part6C, 4, 0x5007, 0, 3, 2);
    obj->part70 = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagStackPart2, 0), 4);
    list_insert(list, obj->part70);
    obj_create_sprite_10(obj->part70, 4, 0x5007, 0, 4, 2);
    obj->part74 = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagStackPart3, 0), 5);
    list_insert(list, obj->part74);
    if (x > 0x7800)
    {
        sprA = obj->part6C->sprite;
        sprB = obj->part70->sprite;
        sprB[0x11] |= 0x40;
        sprA[0x11] |= 0x40;
        obj_create_sprite_10(obj->part74, 4, 0x5007, 0, 0xC, 2);
    }
    else
    {
        obj_create_sprite_10(obj->part74, 4, 0x5007, 0, 9, 2);
    }
    obj->unk18 = x;
    obj->unk1C = y;
    obj->unk7C = 0;
    obj->unk80 = z;
    obj->unk84 = 0;
    obj->unk88 = 0;
    obj->unk78 = 1;
    obj->unk8C = 0;
    obj_sync_children_pos(obj);
    return obj;
}

void obj_move_child_step(struct ChildObj *obj)
{
    s32 pos;
    s32 limit;
    s32 *step;
    s32 v;
    s32 next;
    unsigned long long wide;

    step = &obj->unk80;
    pos = obj->unk1C + *step;
    obj->unk1C = pos;
    if (pos <= 0x3FFF)
    {
        wide = 0x3FFF;
        limit = wide;
        limit++;
    }
    else
    {
        limit = 0x8000;
        if (pos <= 0x8000)
        {
            goto state;
        }
    }
    obj->unk1C = limit;
    *step = -*step;
state:
    if (obj->unk78 == 2)
    {
        goto grow;
    }

    if (obj->unk78 <= 2)
    {
        goto sync;
    }
    if (obj->unk78 == 3)
    {
        goto shrink;
    }
    goto sync;
grow:
    v = obj->unk84 + 0x100;
    obj->unk84 = v;
    obj->unk88 = v;
    if (obj->unk84 <= 0x1800)
    {
        goto sync;
    }
    obj->unk88 = 0x1800;
    obj->unk84 = 0x1800;
    next = 0;
    goto store;
shrink:
    v = obj->unk84 - 0x100;
    obj->unk84 = v;
    obj->unk88 = v;
    if (obj->unk84 >= 0)
    {
        goto sync;
    }
    obj->unk88 = 0;
    obj->unk84 = 0;
    next = 1;
store:
    obj->unk78 = next;

sync:
    obj_sync_children_pos(obj);
}

void obj_sync_children_pos(struct ChildObj *obj)
{
    s32 counter;
    s32 sin;
    s32 pos;
    u8 *spr;
    struct ChildPart *target;

    obj->part6C->x = obj->unk18;
    obj->part6C->y = obj->unk1C - obj->unk84;
    obj->part70->x = obj->unk18;
    obj->part70->y = obj->unk1C + obj->unk88;

    counter = obj->unk8C;
    if (counter != 0)
    {
        counter += 3;
        obj->unk8C = counter;
        if (counter > 0x80)
            obj->unk8C = 0;
    }

    spr = obj->part6C->sprite;
    if (spr[0x11] & 0x40)
    {
        sin = gSineTable[*(u8 *)&obj->unk8C];
        target = obj->part74;
        if (sin < 0)
            sin += 0x3F;
        sin >>= 6;
        sin <<= 5;
        sin += -0x2800;
        pos = obj->unk18 - sin;
    }
    else
    {
        sin = gSineTable[*(u8 *)&obj->unk8C];
        target = obj->part74;
        if (sin < 0)
            sin += 0x3F;
        sin >>= 6;
        sin <<= 5;
        sin += -0x2800;
        pos = obj->unk18 + sin;
    }
    target->x = pos;
    target->y = obj->unk1C;
}

ASM_FUNC("asm/code_0816CA84/obj_chase_step.s", s32 obj_chase_step(struct ChildObj *node, s32 a1, s32 a2, void *a3, void *a4))

ASM_FUNC("asm/code_0816CA84/obj_scene_init.s", void obj_scene_init(void))

void obj_step_stages(struct StageObj *obj)
{
    u16 *ram;

    rand_scaled(0, 2);
    switch (obj->stage)
    {
    case 0:
        {
            s32 off = 0x884;
            *(s32 *)((u8 *)&gSys + off) = -1;
        }
        set_slot_target(0, 5, -1);
        obj->stage = 1;
        break;
    case 1:
        if ((u8)timer_tick((u8 *)obj + 0x4C))
            obj->stage = 2;
        break;
    case 2:
        obj_step_stage2(obj);
        break;
    case 3:
        obj_spawn_phase(obj);
        break;
    case 4:
        obj_step_stage4(obj);
        break;
    case 5:
        obj_tween_state(obj);
        break;
    case 6:
        if ((u8)timer_tick((u8 *)obj + 0x4C))
            obj->stage = 7;
        break;
    case 7:
        palette_fill(0);
        slot_reset(0);
        ram = (u16 *)gUnk_02000050;
        *ram = 0;
        ram += 2;
        *ram = 0;
        if (obj != 0)
            obj->cls->behavior((u8 *)obj + obj->cls->thisOffset, 3);
        return;
    default:
        break;
    }
    obj_copy_pos_to_shadow(obj->shadowSrc);
}

void obj_step_children(struct ChildObj *obj)
{
    struct ChildObj *node;
    struct ChildObj *next;
    s32 r;
    struct ChildPart *p;
    struct PlacedObj *placed;
    u8 *spr;
    s32 b;

    node = *(struct ChildObj **)obj->unk1C;
    if (node == 0)
        goto place;
    do
    {
        next = node->next;
        if (node->kind == 2)
            goto move;
        if (node->kind > 2)
            goto upper;
        if (node->kind == 0)
            goto speeds;
        goto after;

    upper:
        if (node->kind == 6)
            goto chase;
        goto after;

    speeds:
        obj_step_child_speeds(node);
        goto after;

    move:
        obj_move_child_step(node);
        goto after;

    chase:
        r = obj_chase_step(node, obj->unk1C, obj->unk2C, obj->part30, obj->part34);
        if (r == (s8)gChaseReturnTable[2])
        {
            obj->unk64 += 0x3C;
            spr = obj->part48->sprite;
            b = spr[0x12];
            spr[0x12] = b & -0x11;
        }
        if (r < 0)
            goto negative;
        if (r <= 0)
            goto after;
        set_slot_value(0x2E, -1);
        placed = obj->placed38;
        placed->unk18 += r;
        if (placed->unk18 < 0)
            placed->unk18 = 0;
        if (placed->unk18 > 0x3E7)
            placed->unk18 = 0x3E7;
        if (placed->unk34 < placed->unk18)
        {
            placed->unk34 = placed->unk18;
            placed->unk50 = 1;
        }
        goto after;

    negative:
        placed = obj->placed38;
        placed->unk18 -= r;
        if (placed->unk18 < 0)
            placed->unk18 = 0;
        if (placed->unk18 > 0x3E7)
            placed->unk18 = 0x3E7;
        if (placed->unk34 < placed->unk18)
        {
            placed->unk34 = placed->unk18;
            placed->unk50 = 1;
        }
        if (obj->unk60 == 0)
        {
            p = obj->part30;
            if (p->unk78 == 0)
            {
                p->unk78 = 3;
                p->unk8C += 0x10;
                set_slot_value(0x40, -1);
            }
        }
        else
        {
            p = obj->part34;
            if (p->unk78 == 0)
            {
                p->unk78 = 3;
                p->unk8C += 0x10;
                set_slot_value(0x40, -1);
            }
        }

    after:
        node = next;
    } while (node != 0);

place:
    obj_place_children(obj->placed38, 0);
}

ASM_FUNC("asm/code_0816CA84/obj_step_stage2.s", void obj_step_stage2(struct StageObj *obj))

ASM_FUNC("asm/code_0816CA84/obj_spawn_phase.s", void obj_spawn_phase(struct StageObj *obj))

void write_var(u16 id, u16 value);
void tween_groups_set_flag_bit(u16 index, u8 value);
void timer_set(void *timer, u32 limit);
void anim_task_start(void *task, u32 a, const void *table, u32 b, u32 c);
extern const u8 gHeapTagAnimTaskData[];
ASM_FUNC("asm/code_0816CA84/obj_step_stage4.s", void obj_step_stage4(struct StageObj *obj))
