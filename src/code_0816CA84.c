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
    s32 unk88;
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
    s32 unk1C;
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

struct PartA
{
    u8 filler0[0x6C];
    struct ChildPart *bounce1;
    struct ChildPart *bounce2;
    u8 filler74[0x88 - 0x74];
    s32 unk88;
};

struct Rec10
{
    s16 x;
    s16 y;
};

struct StageTarget
{
    u8 filler0[0x10];
    struct Rec10 *rec;
};

struct StageObj
{
    u8 filler0[0x18];
    struct StageClass *cls;
    void *children;
    u8 filler20[0x28 - 0x20];
    void *sprite;
    struct PartA *partA;
    struct ChildPart *part30;
    struct ChildPart *part34;
    struct PlacedObj *placed38;
    struct SpawnAnim *anim3C;
    void *shadowSrc;
    struct StageTarget *target;
    struct ChildPart *part48;
    s32 timer4C;
    s32 lastCount;
    s32 stage;
    s32 nextStage;
    s32 substate;
    s32 unk60;
    s32 tick;
    u16 phaseHw;
    u16 unk6A;
    u16 unk6C;
    u16 flags6E;
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
void obj_script_step(void *obj);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_setup_slot(void *part, s32 pos, s32 a, s32 b, s32 c, ByteArg d);
u32 obj_is_done(void *obj);
u32 get_flag_bit(u32 id);
void win_draw_text(void *spr, u32 image, s32 x, s32 y, u32 flag);
void obj_load_record(void *rec, u32 id, u32 a, u32 b, u32 c);
void obj_init_class_38fa(void *part, u32 v);
void obj_list_update_pos(void *children, u32 a, u32 b);
void obj_update_anim_pair(void *anim, u32 a, u32 b);
void update_obj_list(void);
void sound_stop_song(u32 id);
u32 obj_flush_anim(void *sprite);
void obj_release_and_flush(void *sprite, u32 mode);
extern const u8 gText[];

static inline u32 sys_lang(void)
{
    return *((u8 *)&gSys + 0x8C0);
}

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

NONMATCH("asm/code_0816CA84/obj_move_child_step.s", void obj_move_child_step(struct ChildObj *obj))
{
    s32 pos;
    s32 limit;
    s32 *step;
    s32 cur;
    s32 v;
    s32 next;

    step = &obj->unk80;
    cur = obj->unk1C;
    pos = *step;
    pos += cur;
    obj->unk1C = pos;
    if (pos <= 0x3FFF)
    {
        limit = 0x3FFF;
        limit++;
    }
    else
    {
        limit = 0x8000;
        if (pos <= 0x8000)
            goto state;
    }
    obj->unk1C = limit;
    *step = -*step;

state:
    if (obj->unk78 == 2)
        goto grow;
    if (obj->unk78 <= 2)
        goto sync;
    if (obj->unk78 == 3)
        goto shrink;
    goto sync;

grow:
    v = obj->unk84 + 0x100;
    obj->unk84 = v;
    obj->unk88 = v;
    if (obj->unk84 <= 0x1800)
        goto sync;
    obj->unk88 = 0x1800;
    obj->unk84 = 0x1800;
    next = 0;
    goto store;

shrink:
    v = obj->unk84 - 0x100;
    obj->unk84 = v;
    obj->unk88 = v;
    if (obj->unk84 >= 0)
        goto sync;
    obj->unk88 = 0;
    obj->unk84 = 0;
    next = 1;

store:
    obj->unk78 = next;

sync:
    obj_sync_children_pos(obj);
}
END_NONMATCH

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

NONMATCH("asm/code_0816CA84/obj_step_stage2.s", void obj_step_stage2(struct StageObj *obj))
{
    s32 sub;
    struct ChildPart *bounce;
    struct PartA *a;
    s32 limit;
    s32 off;
    s32 x;
    s32 y;
    s32 sv;
    s32 v;
    u32 done;
    u32 image;
    u32 flushed;
    u8 flag;
    void *sprite;
    struct Rec10 *rec;
    u8 zero;
    u8 one;

    sub = obj->substate;
    if (sub <= 10)
    {
        if (sub >= 7)
        {
            if ((gSys.keys2A & 8) != 0)
            {
                a = obj->partA;
                if (a->unk88 != 0)
                {
                    a->unk88 = 0;
                    sound_stop_song(0x97);
                }
                set_slot_value(0x37, -1);
                obj->nextStage = 2;
                obj->stage = 5;
                obj->phaseHw = 0;
                return;
            }
        }
    }
    obj_script_step(obj->target);
    bounce = obj->partA->bounce1;
    bounce->unk70 += 0x20;
    limit = 0x100;
    if (bounce->unk70 > limit)
        bounce->unk70 = limit;
    obj_step_bounce(bounce);
    bounce = obj->partA->bounce2;
    bounce->unk70 += 0x20;
    if (bounce->unk70 > limit)
        bounce->unk70 = limit;
    obj_step_bounce(bounce);
    obj_aim_children((struct ChildObj *)obj->partA);
    obj_script_step(obj->part30);
    obj_script_step(obj->part34);
    obj_place_children(obj->placed38, 0);
    obj_update_anim_pair(obj->anim3C, 0, 0);

    switch (obj->substate)
    {
    case 0:
        zero = 0;
        obj->tick = 0;
        obj->placed38->unk18 = 0;
        obj->placed38->unk50 = 0;
        obj->anim3C->unk18 = 0x3C;
        obj->anim3C->unk1C = 0;
        if ((obj->flags6E & 1) != 0)
        {
            obj_setup_slot(obj->part30, obj->part30->x, 0x6000, 0, 0x3C, zero);
            obj_setup_slot(obj->part34, obj->part34->x, 0x6000, 0, 0x3C, zero);
            obj->substate = 7;
            break;
        }
        one = 1;
        obj->substate = 1;
        obj->flags6E |= 1;
        goto count_up;
    case 1:
    count_up:
        off = obj->part34->unk84 + 0x100;
        obj->part34->unk84 = off;
        if (off > 9215)
        {
            obj_load_record(obj->target->rec, 0xA, 0, 0, 0);
            one = 1;
            obj_setup_slot(obj->target, 0xFC00, 0x7000, 0, 0x5A, one);
            obj->substate = 2;
        }
        break;
    case 2:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
        {
            flag = get_flag_bit(0x1F1);
            if (flag == 0)
            {
                sprite = obj->sprite;
                image = *(u32 *)((u8 *)gText + 0x11C824 + (sys_lang() << 2));
                rec = obj->target->rec;
                x = rec->x - 8;
                y = rec->y - 0x10;
                win_draw_text(sprite, image, x, y, flag);
                obj_load_record(obj->target->rec, 0xB, 0, 0, flag);
                obj->substate = 3;
            }
            else
            {
                sprite = obj->sprite;
                image = *(u32 *)((u8 *)gText + 0x11C838 + (sys_lang() << 2));
                rec = obj->target->rec;
                x = rec->x - 8;
                y = rec->y - 0x10;
                win_draw_text(sprite, image, x, y, 0);
                obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
                obj->substate = 4;
            }
        }
        break;
    case 3:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 0)
        {
            obj_release_and_flush(obj->sprite, 0);
            image = *(u32 *)((u8 *)gText + 0x11C838 + (sys_lang() << 2));
            rec = obj->target->rec;
            x = rec->x - 8;
            y = rec->y - 0x10;
            win_draw_text(obj->sprite, image, x, y, flushed);
            obj_load_record(obj->target->rec, 0xB, 0, 0, flushed);
            obj->substate = 4;
        }
        break;
    case 4:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 0)
        {
            obj_release_and_flush(obj->sprite, 0);
            obj_load_record(obj->target->rec, 0xA, 0, 0, flushed);
            one = 1;
            obj_setup_slot(obj->target, 0x12000, 0x7000, 0, 0x1E, one);
            obj->substate = 5;
        }
        break;
    case 5:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
            obj->substate = 6;
        break;
    case 6:
        off = obj->part34->unk84 - 0x100;
        obj->part34->unk84 = off;
        if (off <= 0)
        {
            zero = 0;
            obj->part34->unk84 = 0;
            obj_setup_slot(obj->part30, obj->part30->x, 0x6000, 0, 0x3C, zero);
            obj_setup_slot(obj->part34, obj->part34->x, 0x6000, 0, 0x3C, zero);
            obj->substate = 7;
        }
        break;
    case 7:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj_init_class_38fa(obj->part30, 0x1E);
            obj->substate = 8;
        }
        break;
    case 8:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj->substate = 9;
            obj_init_class_38fa(obj->part30, 0x40);
        }
        break;
    case 9:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj->substate = 10;
            obj_init_class_38fa(obj->part30, 0x1E);
            obj->part34->unk88 = 0;
            obj->part34->unk84 = 0;
            obj->part30->unk88 = 0;
            obj->part30->unk84 = 0;
        }
        else
        {
            sv = gSineTable[((*(s16 *)((u8 *)obj->part30 + 0x4C)) * 4) & 0xFF];
            if (sv < 0)
                sv += 0x3F;
            v = (sv >> 6) << 4;
            if (v < 0)
                v = -v;
            obj->part34->unk88 = v;
            obj->part34->unk84 = v;
            obj->part30->unk88 = v;
            obj->part30->unk84 = v;
        }
        break;
    case 10:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            obj->stage = 3;
            obj->substate = 0;
        }
        break;
    default:
        break;
    }
    obj_sync_children_pos((struct ChildObj *)obj->part30);
    obj_sync_children_pos((struct ChildObj *)obj->part34);
    obj_list_update_pos(obj->children, 0, 0);
    update_obj_list();
}
END_NONMATCH

ASM_FUNC("asm/code_0816CA84/obj_spawn_phase.s", void obj_spawn_phase(struct StageObj *obj))

void write_var(u16 id, u16 value);
void tween_groups_set_flag_bit(u16 index, u8 value);
void timer_set(void *timer, u32 limit);
void anim_task_start(void *task, u32 a, const void *table, u32 b, u32 c);
extern const u8 gHeapTagAnimTaskData[];
NONMATCH("asm/code_0816CA84/obj_step_stage4.s", void obj_step_stage4(struct StageObj *obj))
{
    struct ChildObj *node;
    struct ChildObj *next;
    struct ChildPart *bounce;
    struct PlacedObj *placed;
    s32 chase_count;
    s32 delta;
    s32 limit;
    s32 off;
    s32 x;
    s32 y;
    s32 pflag;
    u32 done;
    u32 flushed;
    u32 image;
    u8 flag;
    u8 zero;
    u8 one;
    void *sprite;

    obj_script_step(obj->target);
    obj_script_step(obj->part30);
    obj_script_step(obj->part34);
    bounce = obj->partA->bounce1;
    bounce->unk70 -= 0x20;
    if (bounce->unk70 < -512)
        bounce->unk70 = -512;
    obj_step_bounce(bounce);
    bounce = obj->partA->bounce2;
    bounce->unk70 += 0x20;
    limit = 0x100;
    if (bounce->unk70 > limit)
        bounce->unk70 = limit;
    obj_step_bounce(bounce);
    obj_aim_children((struct ChildObj *)obj->partA);

    chase_count = 0;
    node = *(struct ChildObj **)obj->children;
    while (node != 0)
    {
        next = node->next;
        if (node->kind == 6)
        {
            chase_count++;
            delta = obj_chase_step(node, (s32)obj->children, (s32)obj->partA, obj->part30, obj->part34);
            if (delta < 0)
                delta = -delta;
            placed = obj->placed38;
            placed->unk18 += delta;
            if (placed->unk18 < 0)
                placed->unk18 = 0;
            if (placed->unk18 > 0x3E7)
                placed->unk18 = 0x3E7;
            if (placed->unk34 < placed->unk18)
            {
                placed->unk34 = placed->unk18;
                placed->unk50 = 1;
            }
        }
        node = next;
    }
    obj_place_children(obj->placed38, 0);

    switch (obj->substate)
    {
    case 0:
        zero = 0;
        obj->tick = 0;
        obj_load_record(obj->part48->sprite, 0x25, 0, 0, zero);
        ((u8 *)obj->part48->sprite)[0x12] |= 0x10;
        if (obj->partA->unk88 == 0)
        {
            obj->partA->unk88 = 1;
            set_slot_value(0x97, -1);
        }
        obj_setup_slot(obj->part30, obj->part30->x, 0x8000, 0, 0x3C, zero);
        obj_setup_slot(obj->part34, obj->part34->x, 0x8000, 0, 0x3C, zero);
        obj->substate = 1;
        break;
    case 1:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            if (obj->partA->unk88 != 0)
            {
                obj->partA->unk88 = 0;
                sound_stop_song(0x97);
            }
            obj_init_class_38fa(obj->part30, 0x3C);
            obj->substate = 2;
        }
        break;
    case 2:
        done = obj_is_done(obj->part30);
        if ((done << 24) != 0)
        {
            if (chase_count == 0)
                obj->substate = 3;
        }
        break;
    case 3:
        off = obj->part34->unk84 + 0x100;
        obj->part34->unk84 = off;
        if (off > 9215)
        {
            obj_load_record(obj->target->rec, 0xA, 0, 0, 0);
            one = 1;
            obj_setup_slot(obj->target, 0xFC00, 0x7000, 0, 0x5A, one);
            obj->substate = 4;
        }
        break;
    case 4:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
        {
            placed = obj->placed38;
            if (obj->lastCount < placed->unk18)
            {
                obj->lastCount = placed->unk18;
                write_var(0x1CDD, placed->unk18);
                tween_groups_set_flag_bit(0x1ED, 1);
            }
            placed = obj->placed38;
            pflag = placed->unk50;
            if (pflag == 0)
            {
                sprite = obj->sprite;
                image = *(u32 *)((u8 *)gText + 0x11C9A0 + (sys_lang() << 2));
                x = obj->target->rec->x - 8;
                y = obj->target->rec->y - 0x10;
                win_draw_text(sprite, image, x, y, 0);
                obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
                obj->substate = 7;
            }
            else
            {
                write_var(0x1CE3, placed->unk34);
                flag = get_flag_bit(0x1F1);
                if (flag == 0)
                {
                    tween_groups_set_flag_bit(0x1F1, 1);
                    sprite = obj->sprite;
                    image = *(u32 *)((u8 *)gText + 0x11C860 + (sys_lang() << 2));
                    x = obj->target->rec->x - 8;
                    y = obj->target->rec->y - 0x10;
                    win_draw_text(sprite, image, x, y, flag);
                    obj_load_record(obj->target->rec, 0xB, 0, 0, flag);
                    obj->substate = 8;
                }
                else
                {
                    sprite = obj->sprite;
                    image = *(u32 *)((u8 *)gText + 0x11C9B4 + (sys_lang() << 2));
                    x = obj->target->rec->x - 8;
                    y = obj->target->rec->y - 0x10;
                    win_draw_text(sprite, image, x, y, 0);
                    obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
                    obj->substate = 7;
                }
            }
        }
        break;
    case 5:
        done = obj_is_done(obj->target);
        if ((done << 24) != 0)
            obj->substate = 6;
        break;
    case 6:
        off = obj->part34->unk84 - 0x100;
        obj->part34->unk84 = off;
        if (off <= 0)
        {
            obj->part34->unk84 = 0;
            obj->substate = 0;
            obj->stage = 2;
        }
        break;
    case 7:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 2)
            goto reload;
        if ((s32)flushed > 2)
        {
            if (flushed == 0x3E7)
                goto reload;
            break;
        }
        if (flushed == 1)
        {
            obj_release_and_flush(obj->sprite, 0);
            obj_load_record(obj->target->rec, 0xA, 0, 0, 0);
            obj_setup_slot(obj->target, 0x12000, 0x7000, 0, 0x1E, (u8)flushed);
            obj->substate = 5;
        }
        break;
    reload:
        obj_load_record(obj->target->rec, 0xB, 0, 0, 0);
        obj_release_and_flush(obj->sprite, 0);
        image = *(u32 *)((u8 *)gText + 0x11C874 + (sys_lang() << 2));
        x = obj->target->rec->x - 8;
        y = obj->target->rec->y - 0x10;
        win_draw_text(obj->sprite, image, x, y, 0);
        obj->substate = 8;
        break;
    case 8:
        flushed = obj_flush_anim(obj->sprite);
        if (flushed == 0)
        {
            obj_release_and_flush(obj->sprite, 0);
            timer_set(&obj->timer4C, 0x20);
            anim_task_start(heap_alloc2(0x24, 0, gHeapTagAnimTaskData, 0), 8, gHeapTagAnimTaskData, 1, 0x20);
            obj->stage = 6;
        }
        break;
    default:
        break;
    }
    obj_sync_children_pos((struct ChildObj *)obj->part30);
    obj_sync_children_pos((struct ChildObj *)obj->part34);
    obj_list_update_pos(obj->children, 0, 0);
    update_obj_list();
}
END_NONMATCH
