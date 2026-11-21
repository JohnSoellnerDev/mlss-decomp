#include "global.h"
#include "ram_routines.h"

struct Obj
{
    u32 unk0;
    struct Obj *unk4;
    struct Obj *unk8;
    const void *unkC;
    void *unk10;
    void *unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u32 unk40;
    const void *unk44;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
    u8 filler4E[0x52 - 0x4E];
    u16 unk52;
    u16 unk54;
    u16 unk56;
    u16 unk58;
    u16 unk5A;
    u8 filler5C[0x64 - 0x5C];
    void *unk64;
};

struct ObjList
{
    struct Obj *head;
    struct Obj *tail;
    u32 count;
};

struct ObjClass
{
    u8 filler0[8];
    s16 unk8;
    u16 unkA;
    void (*unkC)(void *, u32);
};

struct Sprite
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10[0x1F - 0x10];
    u8 unk1F;
};

/* two groups of three child objects after the first word */
struct Triples
{
    u32 unk0;
    struct Obj *kids[2][3];
};

struct Lo2Bits
{
    u8 lo2 : 2;
    u8 rest : 6;
};

struct Prio4Bits
{
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct Triple
{
    s32 x;
    s32 y;
    s32 z;
};

/* sprite position; written through the union, so the stores may alias any field */
union SprPos
{
    s16 xy[2];
};

// A text window that prints its text one character a frame (obj_dialog_step).
struct DialogObj
{
    union SprPos *sprite; // the "more" arrow, or the cursor of a choice
    u8 filler4[0x4];
    s32 unk8;             // +8..+0x1C: the text run (struct Voice)
    s32 unkC;
    const u8 *text;       // the next character or DLG_* code
    u8 unk14;             // pen x in the window
    u8 unk15;             // pen y in the window
    u8 filler16[0x2];
    s32 unk18;
    s32 unk1C;
    u8 filler20[0x8];
    s32 unk28;            // window x in pixels
    s32 unk2C;            // window y in pixels
    s32 unk30;            // window size in tiles: the "more" arrow's x uses it,
    s32 unk34;            // its y uses this, and so does DLG_ALIGN (for the line's width)
    u8 filler38[0x14];
    void (*unk4C)(void *); // the frame drawing routine (frame_9slice_tiles), for a new page
    u8 filler50[0x8];
    const u8 *unk58;      // where DLG_END goes on after a DLG_BRANCH, NULL for the real end
    s32 unk5C;            // frames to wait before the next character
    u32 state;            // DIALOG_*
    s32 count;            // choices placed so far (DLG_CHOICE)
    s32 idx;              // the choice the cursor is on
    s32 maxIdx;           // the choice the cursor starts on
    s32 pos[4][2];        // where the cursor goes for each choice
    u8 filler90[0x10];
    u16 soundOn;          // the typing sound is playing
    s16 soundId;          // the typing sound
};

// DialogObj.state
#define DIALOG_PRINTING 0 // a character a frame
#define DIALOG_DONE     1
#define DIALOG_WAIT_KEY 2 // the "more" arrow shows until A
#define DIALOG_CHOOSE   3 // the cursor moves between the choices
#define DIALOG_FAST     4 // A was pressed: everything up to the next stop at once

// Text codes: 0xFF00 | code (any other character prints).
#define DLG_CODE_MASK 0xFFE0
#define DLG_CODE      0xFF00
#define DLG_CHAR      0  // (not a code)
#define DLG_PAGE      1  // clear the window and start again at the top
#define DLG_FAST      3  // print the rest up to the next stop at once
#define DLG_CHOICE    4  // a choice starts here
#define DLG_END       10
#define DLG_ALIGN     11 // + byte: the line is left-aligned (0), centred (1) or right-aligned (2)
#define DLG_WAIT      12 // + byte: frames to wait
#define DLG_BRANCH    13 // + byte: goes to one of two texts (by whether the byte is 0), then back
#define DLG_STOP      17 // + byte: wait for A (byte 1: another arrow), or let the player choose

// What obj_dialog_step returns, besides the choice (1 for the first).
#define DIALOG_BUSY   (-1)
#define DIALOG_CANCEL 999 // B on a choice

extern const u8 gTweenGroupVtable[];
extern const u8 gClassEad8[];
extern const u8 gClassEae8[];
extern s16 gUnk_0300102C;
extern const u8 gUnk_082138FA[];
extern const u8 gObjScript08213a04[];
extern u8 gSys[];
extern const void *gChildTableByLang[];
extern const u8 gClassEac8[];
extern const u8 gObjScript08213936[];
extern const u8 gObjScript08213b8a[];
extern const s16 gAnimLangSingleChild[];
extern const u8 gPackedGfxBufferTag[];
extern u32 gUnk_03001028;
extern void (*gUnk_03000C84)(const u8 *src, void *dst);

void obj_set_class_38fa(struct Obj *obj, u32 mode, u32 offset);
u32 obj_script_step(struct Obj *obj);
void update_obj_sprite_pos(struct Obj *obj, u32 camX, u32 camY);
void obj_release_slots(void *sprite);
void sprite_update_cd0(void *sprite);
void heap_free_b(void *ptr);
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void tween_groups_obj_start_move(struct Obj *obj, u32 dist, u32 target, ByteArg useRam);
void obj_list_drain(struct ObjList *list);
void release_obj_131c(void *sprite);
void obj_load_record(void *sprite, s32 anim, u32 a, u32 b, u32 c);
u32 read_varlen_plus1(const u8 *src);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void heap_free(void *ptr);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);

static inline void obj_run_behavior(struct Obj *child)
{
    if (child != 0)
    {
        struct ObjClass *cls = (struct ObjClass *)child->unkC;

        cls->unkC((u8 *)child + cls->unk8, 3);
    }
}

static inline u32 child_is_done(struct Obj *child)
{
    if (child->unk44 == 0)
    {
        return 1;
    }
    if (child->unk40 == 1)
    {
        return 1;
    }
    return 0;
}

/* a child is finished when it has no script left or its mode is 1 */
static inline u32 child_finished(struct Obj *child)
{
    if (child->unk44)
    {
        if (child->unk40 == 1)
        {
            return 1;
        }
        return 0;
    }
    return 1;
}
void cpu_fast_set(void *src, void *dst, u32 control);

void obj_set_child_table(struct Obj *obj)
{
    const void **table = gChildTableByLang;
    const void **entry = table + gSys[0x8C0];
    struct Obj *child = (struct Obj *)obj->unkC;

    child->unk44 = *entry;
    child->unk48 = 0;
    child->unk40 = 0;
}

/* Shows the last three decimal digits of value on three digit sprites: each digit (0-9) is
 * loaded as that sprite's animation, hundreds on obj->unk8, tens on obj->unk4, ones on obj->unk0. */
void obj_load_scaled_anims(struct Obj *obj, u32 value)
{
    s32 (*div)(s32, s32);
    s32 (*mod)(s32, s32);
    s32 digit;

    div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
    mod = MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424);
    digit = div(mod(value, 1000), 100);
    obj_load_record(obj->unk8->unk10, digit, 0, 0, 0);
    digit = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(
        MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424)(value, 100), 10);
    obj_load_record(obj->unk4->unk10, digit, 0, 0, 0);
    digit = MATH_ROUTINE(s32 (*)(s32, s32), iwram_math_08199424)(value, 10);
    obj_load_record(((struct Obj *)obj->unk0)->unk10, digit, 0, 0, 0);
}

void obj_init_children_sprites(struct Obj *obj)
{
    struct Obj **children = (struct Obj **)obj;
    u32 i = 0;
    const void *script = gObjScript08213b8a;

    obj_set_class_38fa(children[0], 6, i * 8);
    children[0]->unk44 = script;
    children[0]->unk48 = 0;
    i++;
    obj_set_class_38fa(children[1], 6, i * 8);
    children[1]->unk44 = script;
    children[1]->unk48 = 0;
    i++;
    obj_set_class_38fa(children[2], 6, i * 8);
    children[2]->unk44 = script;
    children[2]->unk48 = 0;
    i++;
    obj_set_class_38fa(children[3], 6, i * 8);
    children[3]->unk44 = script;
    children[3]->unk48 = 0;
}

void obj_refresh_children(struct Obj *obj)
{
    struct Obj **children = (struct Obj **)obj;

    obj_script_step(children[0]);
    update_obj_sprite_pos(children[0], 0, 0);
    obj_script_step(children[1]);
    update_obj_sprite_pos(children[1], 0, 0);
    obj_script_step(children[2]);
    update_obj_sprite_pos(children[2], 0, 0);
    obj_script_step(children[3]);
    update_obj_sprite_pos(children[3], 0, 0);
}

void obj_run_children_free(struct Obj *obj, u32 flags)
{
    obj_run_behavior((struct Obj *)obj->unk0);
    obj_run_behavior(obj->unk4);
    obj_run_behavior(obj->unk8);
    obj_run_behavior((struct Obj *)obj->unkC);
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

/* only three branch targets differ: the original keeps the jumps to the shared "done" flag */
u32 obj_triples_done(struct Obj *obj)
{
    s32 g, j;
    for (g = 0; g < 2; g++)
        for (j = 0; j < 3; j++)
        {
            struct Obj *child = ((struct Triples *)obj)->kids[g][j];
            u32 done, state;
            if (!child->unk44)
                goto finished;
            state = child->unk40;
            done = 0;
            if (state == 1)
            {
finished:
                done = 1;
            }
            if (!done)
                return 0;
        }
    return 1;
}

/* point a child at a script and restart it */
static inline void child_set_script(struct Obj *child, const u8 *script)
{
    child->unk44 = script;
    child->unk48 = 0;
    child->unk40 = 0;
}

void obj_init_triples_class(struct Obj *obj)
{
    s32 g;
    s32 j;

    for (g = 0; g < 2; g++)
    {
        for (j = 0; j < 3; j++)
        {
            child_set_script(((struct Triples *)obj)->kids[g][j], gObjScript08213a04);
        }
    }
}

void obj_set_child_class_3a04(struct Obj *obj, u32 group, u32 slot)
{
    u32 idx = slot - 1;
    u32 off = (group * 3 + idx) * 4;
    struct Obj **children = &obj->unk4;
    struct Obj *child = *(struct Obj **)((u8 *)children + off);

    child->unk44 = gObjScript08213a04;
    child->unk48 = 0;
    child->unk40 = 0;
}

void obj_refresh_triples(struct Obj *obj)
{
    s32 group = 0;
    u32 base = (u32)&obj->unk4;
    s32 next;

    while (1)
    {
        s32 twice = group << 1;
        struct Obj **entry;
        struct Obj **first;

        next = group + 1;
        entry = (struct Obj **)(((twice + group) << 2) + base);
        first = entry;

        obj_script_step(*entry);
        update_obj_sprite_pos(*entry++, 0, 0);
        obj_script_step(*entry);
        update_obj_sprite_pos(*entry, 0, 0);
        obj_script_step(first[2]);
        update_obj_sprite_pos(first[2], 0, 0);
        group = next;
        if (group > 1)
        {
            break;
        }
    }
}

void obj_run_triples_free(struct Obj *obj, u32 flags)
{
    s32 group = 0;
    u32 base = (u32)&obj->unk4;
    s32 next;

    while (1)
    {
        s32 twice = group << 1;
        struct Obj **entry;
        struct Obj **first;

        next = group + 1;
        entry = (struct Obj **)(((twice + group) << 2) + base);
        first = entry;
        obj_run_behavior(*entry);
        obj_run_behavior(entry[1]);
        obj_run_behavior(first[2]);
        group = next;
        if (group > 1)
        {
            break;
        }
    }
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

u32 obj_slot0_is_empty(struct Obj *obj)
{
    u32 empty = 0;

    if (obj->unk0 == 0)
    {
        empty = 1;
    }
    return empty;
}

void obj_init_single(struct Obj *obj)
{
    struct Obj *child;
    void *sprite;
    s32 anim;

    obj->unk0 = 1;
    sprite = obj->unk4->unk10;
    anim = gAnimLangSingleChild[gSys[0x8C0]];
    obj_load_record(sprite, anim, 0, 0, 0);
    child = obj->unk4;
    child->unk44 = gObjScript08213936;
    child->unk48 = 0;
    child->unk40 = 0;
}

void obj_wait_children_done(struct Obj *obj)
{
    struct Obj **slot;
    u32 count, done, state;
    s32 i;
    if (obj->unk0 == 0)
        return;
    if (obj->unk0 != 1)
        return;
    count = 0;
    slot = (struct Obj **)&obj->unk4;
    for (i = 0; i < 3; i++)
    {
        struct Obj *child = slot[i];
        if (!child->unk44)
            goto finished;
        state = child->unk40;
        done = 0;
        if (state == 1)
        {
finished:
            done = 1;
        }
        if (done)
            count++;
        else
        {
            obj_script_step(slot[i]);
            update_obj_sprite_pos(slot[i], 0, 0);
        }
    }
    if (count == 3)
        obj->unk0 = 0;
}

void obj_run_children3_free(struct Obj *obj, u32 flags)
{
  register struct Obj *parent asm("r5");
  register struct Obj **slot asm("r4") = (struct Obj **) (&obj->unk4);
  obj_run_behavior(obj->unk4);
  obj_run_behavior(slot[1]);
  obj_run_behavior((struct Obj *) obj->unkC);
  if (flags & 1)
  {
    heap_free_b(obj);
  }
}

void obj_list_update_pos(struct ObjList *head, u32 camX, u32 camY)
{
    struct Obj *node = head->head;

    while (node != 0)
    {
        update_obj_sprite_pos(node, camX, camY);
        node = node->unk8;
    }
}

void tween_groups_obj_destroy(struct Obj *obj, u32 flags)
{
    obj->unkC = gClassEad8;
    obj_list_drain((struct ObjList *)obj);
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void obj_init_empty(struct Obj *obj)
{
    obj->unk4 = 0;
    obj->unk0 = 0;
    obj->unk8 = 0;
    obj->unkC = gTweenGroupVtable;
}

void obj_set_home_params(struct Obj *obj, u32 a, u32 b, u32 c, ByteArg flag)
{
    u32 f = flag.value;
    struct Triple *dst;
    struct Triple *src;
    struct Triple *home;

    obj->unk5A &= 0xFFFC;
    dst = (struct Triple *)&obj->unk30;
    src = (struct Triple *)&obj->unk18;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    home = (struct Triple *)&obj->unk24;
    home->x = a;
    home->y = b;
    home->z = c;
    if (f != 0)
    {
        obj->unk5A |= 1;
    }
}

void obj_set_class_38fa(struct Obj *obj, u32 mode, u32 offset)
{
    obj->unk4C = 0;
    obj->unk4A = offset;
    obj->unk44 = gUnk_082138FA;
    obj->unk48 = 0;
    obj->unk40 = mode;
}

void obj_setup_move(struct Obj *obj, u32 a, u32 b, u32 c, u32 dist, u32 target, ByteArg flag, ByteArg useRam)
{
    u32 f = flag.value;
    u32 ram = useRam.value;

    obj_set_home_params(obj, a, b, c, (u8)f);
    tween_groups_obj_start_move(obj, dist, target, (u8)ram);
    obj_set_class_38fa(obj, 4, target);
}

void obj_setup_slot(struct Obj *obj, u32 a, u32 b, u32 c, u32 offset, ByteArg flag)
{
    obj_set_home_params(obj, a, b, c, flag);
    obj_set_class_38fa(obj, 3, offset);
}

void tween_start_set_cls38fa(struct Obj *obj, u32 dist, u32 target, u8 useRam)
{
    tween_groups_obj_start_move(obj, dist, target, useRam);
    obj_set_class_38fa(obj, 2, target);
}

void obj_init_class_38fa(struct Obj *obj, u32 offset)
{
    obj_set_class_38fa(obj, 6, offset);
}

void obj_set_table_44(struct Obj *obj, void *table)
{
    obj->unk44 = table;
    obj->unk48 = 0;
}

u32 obj_set_table_and_init(struct Obj *obj, const void *table)
{
    obj->unk44 = table;
    obj->unk48 = 0;
    obj->unk40 = 0;
    return obj_script_step(obj);
}

void obj_set_table_fields(struct Obj *obj, void *table)
{
    obj->unk44 = table;
    obj->unk48 = 0;
    obj->unk40 = 0;
}

u32 obj_is_done(struct Obj *obj)
{
    if (obj->unk44 == 0)
    {
        return 1;
    }
    if (obj->unk40 == 1)
    {
        return 1;
    }
    return 0;
}

void obj_release_sprites(struct Obj *obj)
{
    if (obj->unk10 != 0)
    {
        obj_release_slots(obj->unk10);
    }
    if (obj->unk14 != 0)
    {
        obj_release_slots(obj->unk14);
    }
}

void obj_update_sprites(struct Obj *obj)
{
    if (obj->unk10 != 0)
    {
        sprite_update_cd0(obj->unk10);
    }
    if (obj->unk14 != 0)
    {
        sprite_update_cd0(obj->unk14);
    }
}

void obj_set_fields_18(struct Obj *obj, s32 x, s32 y, s32 z)
{
    obj->unk18 = x;
    obj->unk1C = y;
    obj->unk20 = z;
}

void obj_copy_mode_fields(struct Obj *obj)
{
    obj->unk54 = obj->unk52;
    obj->unk58 = obj->unk56;
}

void obj_create_sprite_14(struct Obj *obj, u32 a, u32 b, u32 c, s32 anim, u32 prio)
{
    obj->unk14 = create_obj_de4(a, b, c, -1, -1, -1, -1);
    obj_load_record(obj->unk14, anim, -1, 0, 0);
    sprite_update_cd0(obj->unk14);
    ((struct Lo2Bits *)&((struct Sprite *)obj->unk14)->unk1F)->lo2 = prio & 3;
    ((struct Sprite *)obj->unk14)->unkE = 0xFFFF;
}

void obj_create_sprite_10(struct Obj *obj, u32 a, u32 b, u32 c, s32 anim, u32 prio)
{
    struct Prio4Bits *bits;

    obj->unk10 = create_obj_de4(a, b, c, -1, -1, -1, -1);
    obj_load_record(obj->unk10, anim, -1, 0, 0);
    sprite_update_cd0(obj->unk10);
    bits = (struct Prio4Bits *)&((struct Sprite *)obj->unk10)->unk1F;
    bits->f6 = prio;
    bits->f4 = prio;
    bits->f2 = prio;
    bits->f0 = prio;
    ((struct Sprite *)obj->unk10)->unkE = 0;
    obj->unk14 = 0;
}

void obj_set_frame_table(struct Obj *obj, void *table)
{
    obj->unk64 = table;
}

void obj_destroy_sprites(struct Obj *obj, u32 flags)
{
    obj->unkC = gClassEac8;
    if (obj->unk10 != 0)
    {
        release_obj_131c(obj->unk10);
    }
    if (obj->unk14 != 0)
    {
        release_obj_131c(obj->unk14);
    }
    obj->unkC = gClassEae8;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

u32 gate_flag_set(u16 mask)
{
    u32 v = gUnk_0300102C & mask;
    return (-v | v) >> 31;
}

void clear_gate_flags(void)
{
    gUnk_0300102C = 0;
}

u32 obj_list_count_value(struct ObjList *head, u32 value)
{
    struct Obj *node = head->head;
    u32 count = 0;

    while (node != 0)
    {
        if (node->unk0 == value)
        {
            count++;
        }
        node = node->unk8;
    }
    return count;
}

void obj_list_drain(struct ObjList *list)
{
    while (list->head != 0)
    {
        struct Obj *node = list->head;
        struct Obj *prev = node->unk4;

        if (prev != 0)
        {
            if (node->unk8 != 0)
            {
                prev->unk8 = node->unk8;
                node->unk8->unk4 = node->unk4;
            }
            else
            {
                prev->unk8 = 0;
                list->tail = node->unk4;
            }
        }
        else
        {
            if (node->unk8 != 0)
            {
                node->unk8->unk4 = prev;
                list->head = node->unk8;
            }
            else
            {
                list->tail = 0;
                list->head = 0;
            }
        }
        list->count--;
        obj_run_behavior(node);
    }
}

void obj_list_remove_call(struct ObjList *list, struct Obj *node)
{
    struct Obj *prev = node->unk4;

    if (prev != 0)
    {
        if (node->unk8 != 0)
        {
            prev->unk8 = node->unk8;
            node->unk8->unk4 = node->unk4;
        }
        else
        {
            prev->unk8 = 0;
            list->tail = node->unk4;
        }
    }
    else
    {
        if (node->unk8 != 0)
        {
            node->unk8->unk4 = prev;
            list->head = node->unk8;
        }
        else
        {
            list->tail = 0;
            list->head = 0;
        }
    }
    list->count--;
    if (node != 0)
    {
        struct ObjClass *cls = (struct ObjClass *)node->unkC;

        cls->unkC((u8 *)node + cls->unk8, 3);
    }
}

void obj_list_unlink(struct ObjList *list, struct Obj *node)
{
    struct Obj *prev = node->unk4;

    if (prev != 0)
    {
        if (node->unk8 != 0)
        {
            prev->unk8 = node->unk8;
            node->unk8->unk4 = node->unk4;
        }
        else
        {
            prev->unk8 = 0;
            list->tail = node->unk4;
        }
    }
    else
    {
        if (node->unk8 != 0)
        {
            node->unk8->unk4 = prev;
            list->head = node->unk8;
        }
        else
        {
            list->tail = 0;
            list->head = 0;
        }
    }
    list->count--;
}

void list_insert(struct ObjList *list, struct Obj *node)
{
    struct Obj *cur = list->head;

    while (cur != 0)
    {
        if (cur == node)
        {
            return;
        }
        cur = cur->unk8;
    }
    if (list->head == 0)
    {
        node->unk8 = 0;
        node->unk4 = 0;
        list->tail = node;
        list->head = node;
    }
    else
    {
        list->tail->unk8 = node;
        node->unk4 = list->tail;
        node->unk8 = 0;
        list->tail = node;
    }
    list->count++;
}

void list_destroy(struct Obj *list, u32 flags)
{
    list->unkC = gClassEad8;
    obj_list_drain((struct ObjList *)list);
    if ((flags & 1) != 0)
    {
        heap_free_b(list);
    }
}

void init_vt_08cdead8(struct Obj *obj)
{
    obj->unkC = gClassEad8;
    obj->unk4 = 0;
    obj->unk0 = 0;
    obj->unk8 = 0;
}

void obj_class_eae8_free(struct Obj *obj, u32 flags)
{
    obj->unkC = gClassEae8;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void init_vt_08cdeae8(struct Obj *obj, u32 value)
{
    obj->unkC = gClassEae8;
    obj->unk8 = 0;
    obj->unk4 = 0;
    obj->unk0 = value;
}

void palette_fill(u16 color)
{
    u32 value = color | (color << 16);

    cpu_fast_set(&value, (void *)0x05000000, 0x01000080);
    value = color | (color << 16);
    cpu_fast_set(&value, (void *)0x05000200, 0x01000080);
}

void vram_load_packed(const u8 *src, void *dst)
{
    u32 len = read_varlen_plus1(src);
    void *buf = heap_alloc_raw(1, len, gPackedGfxBufferTag);

    gUnk_03000C84(src, buf);
    dma_copy_or_queue(buf, dst, len, 0x10, 2);
    heap_free(buf);
}

s32 lerp_step_08164EE0(s32 a, s32 b, s32 steps, s32 t)
{
    s32 n = steps;

    if (n <= 0)
    {
        n = 1;
    }
    if (t < 0)
    {
        t = 0;
    }
    if (t > n)
    {
        t = n;
    }
    return a + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((b - a) * t, n);
}

NONMATCH("asm/code_08164468/smoothstep_lerp.s", u32 smoothstep_lerp(s32 start, s32 end, s32 steps, s32 step))
{
    s32 off;
    s32 (*div)(s32, s32);
    s32 q1;
    s32 q2;
    s32 q3;
    s32 q4;
    s32 q5;
    s32 d3;

    if (steps <= 0)
    {
        steps = 1;
    }
    if (step < 0)
    {
        step = 0;
    }
    if (step > steps)
    {
        step = steps;
    }
    off = (u32)iwram_div - (u32)iwram_sqrt;
    div = (s32 (*)(s32, s32))(gMathRoutines + off);
    d3 = end * 3;
    d3 = d3 - start * 3;
    q1 = div(d3 * step, steps);
    q2 = div(step * q1, steps);
    div = (s32 (*)(s32, s32))(gMathRoutines + off);
    q3 = div((start - end) * 2 * step, steps);
    q4 = div(step * q3, steps);
    q5 = div(step * q4, steps);
    return q5 + q2 + start;
}
END_NONMATCH

u32 rand_scaled(u32 count, u32 base)
{
    u32 seed;
    u32 r;
    u32 n;
    u32 (*fn)(u32, u32, u32);

    seed = gUnk_03001028 * 0x41c64e6d + 0x3039;
    gUnk_03001028 = seed;
    fn = MATH_ROUTINE(u32 (*)(u32, u32, u32), iwram_math_08199424);
    r = (seed & 0x7fffffff) >> 16;
    n = count - 1;
    return fn(r, base - n, n) + count;
}

void vec3_sub_ops(struct Triple *dst, struct Triple *src)
{
    dst->x -= src->x;
    dst->y -= src->y;
    dst->z -= src->z;
}

void sub_08165020(struct Triple *dst, struct Triple *a, struct Triple *b)
{
    dst->x = a->x - b->x;
    dst->y = a->y - b->y;
    dst->z = a->z - b->z;
}

void triple_add(struct Triple *dst, struct Triple *src)
{
    dst->x += src->x;
    dst->y += src->y;
    dst->z += src->z;
}

void vec3_add_to(struct Triple *dst, struct Triple *a, struct Triple *b)
{
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}

void triple_copy(struct Triple *dst, struct Triple *src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void triple_set(struct Triple *dst, s32 a, s32 b, s32 c)
{
    dst->x = a;
    dst->y = b;
    dst->z = c;
}

s32 obj_field_getters(struct Obj *obj)
{
    return obj->unk1C + obj->unk3C + obj->unk20;
}

u32 sub_081650A4(struct Obj *obj)
{
    return obj->unk20;
}

s32 sub_081650A8(struct Obj *obj)
{
    return obj->unk1C + obj->unk3C;
}

u32 sub_081650B4(struct Obj *obj)
{
    return obj->unk18;
}

u32 sub_081650B8(struct Obj *obj)
{
    return (u32)obj->unk8;
}

u32 sub_081650BC(struct Obj *obj)
{
    return (u32)obj->unk4;
}

u32 sub_081650C0(struct Obj *obj)
{
    return obj->unk0;
}

ASM_FUNC("asm/code_08164468/tilemap_obj_init.s", void tilemap_obj_init(void))

ASM_FUNC("asm/code_08164468/window_obj_setup.s", void window_obj_setup(void))

ASM_FUNC("asm/code_08164468/window_build_tiles.s", void window_build_tiles(void))

void obj_release_tiles(void *obj);
void voice_setup_params(void *voice, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i);
void voice_set_data_ptr(void *voice, u32 a, u32 b, void *ptr);
void obj_flush_tiles(void *obj);

/* The window setup code indexes a DialogObj as an array of s32 words. */
enum DialogWord
{
    WIN_SPRITE = 0,         /* the "more" arrow / choice cursor */
    WIN_VOICE = 2,          /* +8: the text run (struct Voice) */
    WIN_FRAME = 8,          /* +0x20: the frame's tile block, drawn by WIN_FRAME_ROUTINE */
    WIN_X = 10,             /* window x in pixels */
    WIN_Y = 11,             /* window y in pixels */
    WIN_W = 12,             /* window width in tiles */
    WIN_H = 13,             /* window height in tiles */
    WIN_FRAME_ROUTINE = 19, /* frame_9slice_tiles, copied to RAM */
    WIN_SCROLL_X = 20,      /* s16 *: the BG x offset that puts the window at WIN_X */
    WIN_SCROLL_Y = 21,      /* s16 *: the BG y offset that puts the window at WIN_Y */
    WIN_BRANCH_RET = 22,    /* DialogObj.unk58 */
    WIN_WAIT = 23,          /* DialogObj.unk5C */
    WIN_STATE = 24,
    WIN_COUNT = 25,
    WIN_IDX = 26,
    WIN_MAX_IDX = 27,
};

/* Windows stay 4 px inside the 240x160 screen. */
#define WIN_MARGIN 4
#define WIN_MAX_RIGHT  (240 - WIN_MARGIN)
#define WIN_MAX_BOTTOM (160 - WIN_MARGIN)

/*
 * Lays a frameless window out for a text block: src[0] is the width and src[1] the height in
 * tiles, the text follows at src + 2. The window is kept on screen, the dialog state is reset
 * and the BG is scrolled so the window shows at (x, y). Unlike win_set_layout, no frame is drawn.
 */
void window_layout_plain(s32 *win, u8 *src, s32 x, s32 y)
{
    obj_release_tiles(win);
    win[WIN_X] = x;
    if (x < WIN_MARGIN)
    {
        win[WIN_X] = WIN_MARGIN;
    }
    win[WIN_Y] = y;
    if (y < WIN_MARGIN)
    {
        win[WIN_Y] = WIN_MARGIN;
    }
    win[WIN_W] = src[0];
    win[WIN_H] = src[1];
    if (win[WIN_X] + (win[WIN_W] << 3) > WIN_MAX_RIGHT)
    {
        win[WIN_X] = WIN_MAX_RIGHT - (win[WIN_W] << 3);
    }
    if (win[WIN_Y] + (win[WIN_H] << 3) > WIN_MAX_BOTTOM)
    {
        win[WIN_Y] = WIN_MAX_BOTTOM - (win[WIN_H] << 3);
    }
    voice_setup_params(win + WIN_VOICE, win[WIN_FRAME], 0x20, 0xC,
                       (((0x20 - win[WIN_W]) << 27) + 0xC000000U) >> 24, 0, 0, 6, 1, 0);
    voice_set_data_ptr(win + WIN_VOICE, 0, 6, src + 2);
    win[WIN_WAIT] = 0;
    win[WIN_BRANCH_RET] = 0;
    win[WIN_COUNT] = 0;
    win[WIN_IDX] = 0;
    win[WIN_MAX_IDX] = 0;
    win[WIN_STATE] = 0;
    obj_release_slots((void *)win[WIN_SPRITE]);
    *(s16 *)win[WIN_SCROLL_X] = -(s16)win[WIN_X];
    *(s16 *)win[WIN_SCROLL_Y] = -(s16)win[WIN_Y];
    obj_flush_tiles(win);
}

extern void frame_9slice_tiles(void);
extern void frame_9slice_tiles_routine(void);

/*
 * Lays a text window out for a text block: src[0] is the text width (the window gets 4 tiles
 * more, 3-30 tiles), src[1] the line height (+2, 3-12 tiles), the text follows at src + 2.
 * The window is kept on screen, its frame is redrawn, the dialog state is reset and the BG
 * is scrolled so the window shows at (x, y).
 */
void win_set_layout(s32 *win, u8 *src, s32 x, s32 y)
{
    s32 w;
    s32 h;
    s32 lines;

    obj_release_tiles(win);
    win[WIN_X] = x;
    if (x < WIN_MARGIN)
    {
        win[WIN_X] = WIN_MARGIN;
    }
    win[WIN_Y] = y;
    if (y < WIN_MARGIN)
    {
        win[WIN_Y] = WIN_MARGIN;
    }
    w = src[0] + 4;
    if (w > 3)
    {
        if (w > 30)
        {
            w = 30;
        }
    }
    else
    {
        w = 3;
    }
    win[WIN_W] = w;
    lines = src[1];
    // Both branches are the same; the original has the test.
    if (lines == 2 || lines == 4 || lines == 5 || lines + 1 > 12)
    {
        h = lines + 2;
    }
    else
    {
        h = lines + 2;
    }
    if (h > 3)
    {
        if (h > 12)
        {
            h = 12;
        }
    }
    else
    {
        h = 3;
    }
    win[WIN_H] = h;
    if (win[WIN_X] + (win[WIN_W] << 3) > WIN_MAX_RIGHT)
    {
        win[WIN_X] = WIN_MAX_RIGHT - (win[WIN_W] << 3);
    }
    if (win[WIN_Y] + (win[WIN_H] << 3) > WIN_MAX_BOTTOM)
    {
        win[WIN_Y] = WIN_MAX_BOTTOM - (win[WIN_H] << 3);
    }
    voice_setup_params(win + WIN_VOICE, win[WIN_FRAME], 0x20, 0xC,
                       (((0x20 - win[WIN_W]) << 27) + 0xC000000U) >> 24, 0, 0, 6, 1, 2);
    voice_set_data_ptr(win + WIN_VOICE, 0, 6, src + 2);
    // Calls the RAM copy of frame_9slice_tiles_routine.
    ((void (*)(void *))(win[WIN_FRAME_ROUTINE] + ((u32)frame_9slice_tiles_routine - (u32)frame_9slice_tiles)))((u8 *)win + 0x20);
    win[WIN_WAIT] = 0;
    win[WIN_BRANCH_RET] = 0;
    win[WIN_COUNT] = 0;
    win[WIN_IDX] = 0;
    win[WIN_MAX_IDX] = 0;
    win[WIN_STATE] = 0;
    obj_release_slots((void *)win[WIN_SPRITE]);
    *(s16 *)win[WIN_SCROLL_X] = -(s16)win[WIN_X];
    *(s16 *)win[WIN_SCROLL_Y] = -(s16)win[WIN_Y];
    obj_flush_tiles(win);
}

void win_set_layout(s32 *win, u8 *src, s32 x, s32 y);
ASM_FUNC("asm/code_08164468/win_draw_text.s", void win_draw_text(void))

struct DlgInput
{
    u8 filler0[2];
    u16 keys; // newly pressed
};

#define DLG_KEY_A          0x01
#define DLG_KEY_B          0x02
#define DLG_KEYS_PREVIOUS  0x60 // left, up
#define DLG_KEYS_NEXT      0x90 // right, down

/* text voice state embedded in the dialog object at +8 */
struct Voice
{
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
};

extern u8 gUnk_03000374[];
extern const u8 *gDialogJumpTargetFlagNonzero;
extern const u8 *gDialogJumpTargetFlagZero;
extern void frame_9slice_tiles_routine(void);
u32 track_run(void *track);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void mem_copy_fast(void *dst, void *src, u32 len);
u32 layout_text_run();

/* One frame of a text window: prints the next character (all of them up to the next stop once
   A is pressed), with the typing sound, and runs the text codes (DLG_*). Returns DIALOG_BUSY
   while it runs, 0 once the text has ended, and after a choice the chosen one (1 for the
   first) or DIALOG_CANCEL for B. */
u32 obj_dialog_step(struct DialogObj *win)
{
    struct DlgInput *input = (struct DlgInput *)gUnk_03000374;
    s32 c; /* the character or text code, then a code's byte argument */

    while (1)
    {
        switch (win->state)
        {
        case DIALOG_PRINTING:
        case DIALOG_FAST:
            if (win->unk5C != 0)
            {
                if ((input->keys & DLG_KEY_A) == 0)
                {
                    win->unk5C--;
                    return DIALOG_BUSY;
                }
                win->unk5C = 0;
                win->state = DIALOG_FAST;
            }
            {
                u32 code;

                c = (u16)track_run((u8 *)win + 8);
                if (c == 0)
                {
                    obj_release_slots(win->sprite);
                    win->state = DIALOG_DONE;
                    continue;
                }
                code = ((c & DLG_CODE_MASK) == DLG_CODE) ? (c & 0x1F) : DLG_CHAR;
                switch (code)
                {
                case DLG_CHAR:
                    if (win->state == DIALOG_FAST)
                    {
                        continue;
                    }
                    if (input->keys & DLG_KEY_A)
                    {
                        win->state = DIALOG_FAST;
                    }
                    if (win->soundOn == 0)
                    {
                        set_slot_value(win->soundId, -1);
                        win->soundOn = 1;
                    }
                    win->unk5C = 1;
                    return DIALOG_BUSY;
                case DLG_PAGE:
                    obj_release_slots(win->sprite);
                    ((void (*)(void *))((u32)win->unk4C + ((u32)frame_9slice_tiles_routine - (u32)frame_9slice_tiles)))((u8 *)win + 0x20);
                    win->unk14 = 0xC;
                    win->unk15 = 0x14;
                    win->unk5C = 0;
                    win->unk58 = 0;
                    win->count = 0;
                    win->idx = win->maxIdx;
                    win->text++;
                    win->state = DIALOG_PRINTING;
                    return DIALOG_BUSY;
                case DLG_FAST:
                    win->state = DIALOG_FAST;
                    continue;
                case DLG_CHOICE:
                    win->pos[win->count][0] = win->unk28 + win->unk14 + 0xB;
                    win->pos[win->count][1] = win->unk2C + win->unk15 - 7;
                    if (win->count == 0)
                    {
                        if (win->soundOn != 0)
                        {
                            sound_stop_song(win->soundId);
                            win->soundOn = 0;
                        }
                    }
                    if (win->count == win->maxIdx)
                    {
                        obj_load_record(win->sprite, 0, -1, 0, 0);
                        sprite_update_cd0(win->sprite);
                        win->sprite->xy[0] = win->pos[win->count][0];
                        win->sprite->xy[1] = win->pos[win->count][1];
                    }
                    win->count++;
                    continue;
                case DLG_END:
                    if (win->soundOn != 0)
                    {
                        sound_stop_song(win->soundId);
                        win->soundOn = 0;
                    }
                    if (win->unk58 == 0)
                    {
                        obj_release_slots(win->sprite);
                        win->state = DIALOG_DONE;
                        return DIALOG_BUSY;
                    }
                    win->text = win->unk58;
                    win->unk58 = 0;
                    continue;
                case DLG_ALIGN:
                {
                    struct Voice run;
                    s32 x;
                    s32 width;

                    c = *win->text;
                    win->text++;
                    mem_copy_fast(&run, (u8 *)win + 8, 0x14);
                    width = layout_text_run(run, 0) & 0xFFFF;
                    x = 0;
                    switch (c)
                    {
                    case 0:
                        x = 9;
                        break;
                    case 1:
                        x = ((((win->unk34 - 1) << 19) >> 16) - width + 0xA) >> 1;
                        break;
                    case 2:
                        x = (((win->unk34 - 1) << 16) >> 13) - width - 4;
                        break;
                    }
                    voice_set_data_ptr((u8 *)win + 8, 0, (u8)x, (void *)win->text);
                    continue;
                }
                case DLG_WAIT:
                    win->unk5C = *win->text;
                    win->text++;
                    if (win->soundOn != 0)
                    {
                        sound_stop_song(win->soundId);
                        win->soundOn = 0;
                    }
                    continue;
                case DLG_BRANCH:
                {
                    c = *win->text;

                    win->text++;
                    win->unk58 = win->text;
                    if (c != 0)
                    {
                        win->text = gDialogJumpTargetFlagNonzero;
                    }
                    else
                    {
                        win->text = gDialogJumpTargetFlagZero;
                    }
                    continue;
                }
                case DLG_STOP:
                    if (win->count != 0)
                    {
                        win->state = DIALOG_CHOOSE;
                        win->text++;
                        return DIALOG_BUSY;
                    }
                    if (win->soundOn != 0)
                    {
                        sound_stop_song(win->soundId);
                        win->soundOn = 0;
                    }
                    if (*win->text == 1)
                    {
                        obj_load_record(win->sprite, 6, -1, 0, 0);
                    }
                    else
                    {
                        obj_load_record(win->sprite, 1, -1, 0, 0);
                    }
                    sprite_update_cd0(win->sprite);
                    // the arrow goes in the window's bottom right corner
                    win->sprite->xy[0] = win->unk28 + (win->unk30 << 3) - 8;
                    win->sprite->xy[1] = win->unk2C + (win->unk34 << 3) - 8;
                    win->state = DIALOG_WAIT_KEY;
                    win->text++;
                    return DIALOG_BUSY;
                default:
                    obj_release_slots(win->sprite);
                    win->state = DIALOG_DONE;
                    return DIALOG_BUSY;
                }
            }
            break;
        case DIALOG_WAIT_KEY:
            if (input->keys & DLG_KEY_A)
            {
                obj_release_slots(win->sprite);
                win->state = DIALOG_PRINTING;
                continue;
            }
            return DIALOG_BUSY;
        case DIALOG_CHOOSE:
            if (input->keys & DLG_KEYS_PREVIOUS)
            {
                win->idx--;
                if (win->idx < 0)
                {
                    win->idx = win->count - 1;
                }
                set_slot_value(0x4E, -1);
            }
            else if (input->keys & DLG_KEYS_NEXT)
            {
                win->idx++;
                if (win->idx >= win->count)
                {
                    win->idx = 0;
                }
                set_slot_value(0x4E, -1);
            }
            win->sprite->xy[0] = win->pos[win->idx][0];
            win->sprite->xy[1] = win->pos[win->idx][1];
            if (input->keys & DLG_KEY_A)
            {
                set_slot_value(0x60, -1);
                obj_release_slots(win->sprite);
                win->state = DIALOG_PRINTING;
                return win->idx + 1;
            }
            if (input->keys & DLG_KEY_B)
            {
                set_slot_value(0x60, -1);
                obj_release_slots(win->sprite);
                win->state = DIALOG_PRINTING;
                return DIALOG_CANCEL;
            }
            return DIALOG_BUSY;
        case DIALOG_DONE:
        default:
            return 0;
        }
    }
}
