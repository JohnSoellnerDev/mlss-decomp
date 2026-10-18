#include "global.h"

struct MenuTask
{
    u8 filler0[0x12];
    u8 flags12;
};

struct MenuTaskBits
{
    u8 filler0[0x12];
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct MenuSub
{
    u8 filler0[0xD8];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
};

struct MenuCursor
{
    u8 filler0[0x8];
    struct MenuTask *task;
    u8 filler0C[0x28 - 0x0C];
    struct MenuSub *sub;
    u8 filler2C[0x4];
    void *entry30;
};

struct MenuCell
{
    u8 filler0[0x8];
    struct MenuCursor cursor;
};

struct MenuIndex
{
    u8 filler0[0xEC];
    s16 index;
};

struct MenuScene
{
    u8 filler0[0x113];
    u8 mode113;
};

struct MenuEntry
{
    u8 filler0[0x28];
    struct MenuScene *scene;
};

struct MenuLink
{
    u8 filler0[0x8];
    struct MenuCursor cursor;
    u8 filler3C[0x54 - 0x3C];
    s32 unk54;
};

struct MenuWorld
{
    u8 filler0[0x60];
    struct MenuLink *link60;
    struct MenuLink *link64;
    u8 filler68[0x80 - 0x68];
    struct MenuCell *cells80[0x20];
};

struct MenuObj;

struct MenuOwner
{
    u8 filler0[0x30];
    struct MenuObj *target;
};

struct MenuObj
{
    u8 filler0[0x8];
    struct MenuTask *task;
    u8 filler0C[0x14 - 0x0C];
    struct MenuObj *parent14;
    const void *vtable;
    u8 cursor1C;
    u8 state1D;
    u8 filler1E[0x20 - 0x1E];
    struct MenuObj *child20;
    void *child24;
    struct MenuScene *scene;
    struct MenuEntry *entry;
    struct MenuOwner *owner;
    u8 filler34[0x38 - 0x34];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    void (*callback)(struct MenuObj *);
    void (*cb50)(struct MenuObj *);
    u8 filler54[0x76 - 0x54];
    u8 flags76;
    u8 filler77[0x9C - 0x77];
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    u8 fillerA8[0x4];
    u16 timerAC;
    u16 stepAE;
};

struct MenuTable
{
    u8 filler0[0x18];
    const void *vtable;
    void *buf1C;
    void *buf20;
    u8 count24;
    u8 filler25[0x28 - 0x25];
    u32 flags28;
    void *entries2C;
    u16 unk30;
    u8 unk32;
    u8 filler33;
    u16 unk34;
    u16 unk36;
    u16 unk38;
    u8 filler3A[0x3C - 0x3A];
    s32 unk3C;
    s32 unk40;
    u16 unk44;
    u16 unk46;
    u8 unk48;
    u8 filler49;
    u16 unk4A;
    u16 u4C;
    u8 filler4E[0x50 - 0x4E];
    void *unk50;
    void *unk54;
    void *unk58;
};

extern struct MenuWorld *gWorld;
extern u16 gSys[];
extern u8 gUnk_02000080[];
extern const u8 gMenuVtable[];
extern const u8 gClassDd28[];
extern const u8 gTagSdrw[];
extern const u8 gTagFade[];
extern const u8 gMenuTableVtable[];
extern const u8 gMenuObjBufTag[];
extern const u8 gMenuObjBuf2Tag[];
extern const u8 gMenuObjTableTag[];
extern const u8 menu_obj_routine[];

u8 step_motion(void *obj);
void sound_stop_song(u32 song);
void sprite_set_action(void *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void task_insert(void *node, u8 kind, const void *tag);
void task_remove(void *node, u32 kind);
void menu_obj_free(void *obj, u32 kind);
void obj_release(void *obj);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void cpu_set(const void *src, void *dst, u32 ctl);
void heap_free_a(void *p);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, void *obj);
u32 partner_link_update(struct MenuObj *obj, u32 arg, u32 flags);
void place_targets(struct MenuObj *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct MenuObj *obj, u32 arg);
void obj_timer_release_hold(struct MenuObj *obj);
void callback_noop_idle(struct MenuObj *obj);
void obj_setup_world_links(struct MenuObj *obj);

void obj_begin_wait(struct MenuObj *obj);
void obj_end_wait(struct MenuObj *obj);
void obj_place_from_entry(struct MenuObj *obj);
void obj_unlink_if_flagged(struct MenuObj *obj);
void obj_start_if_free(struct MenuObj *obj);
void screen_helpers_obj_release_when_idle(struct MenuObj *obj);
struct MenuTable *menu_obj_init(struct MenuTable *obj, u8 kind, const u8 *tag, u8 flag);

void obj_begin_wait(struct MenuObj *obj)
{
    sprite_set_action(obj->owner->target, 4, 0, 0);
    obj->timerAC = 0x20;
    obj->cb50 = obj_timer_release_hold;
    obj->callback = callback_noop_idle;
    set_slot_value(0x120, -1);
}

void obj_select_callback(struct MenuObj *obj)
{
    if (step_motion(obj->owner->target) == 0) {
        obj->callback = (obj->unk9C != 0) ? obj_place_from_entry : obj_end_wait;
    }
}

void world_links_release(struct MenuObj *obj)
{
    struct MenuCursor *cursor;

    sprite_set_action(obj, 0x1D, 0, 0);
    ((struct MenuTaskBits *)obj->task)->mode = 1;
    cursor = &gWorld->link60->cursor;
    while (cursor->entry30 != NULL) {
        obj_release(cursor->entry30);
    }
    ((struct MenuTaskBits *)cursor->task)->mode = 1;
    cursor = &gWorld->link64->cursor;
    while (cursor->entry30 != NULL) {
        obj_release(cursor->entry30);
    }
    ((struct MenuTaskBits *)cursor->task)->mode = 1;
    obj->callback = obj_unlink_if_flagged;
}

void screen_helpers_obj_show_at(struct MenuObj *obj)
{
    sprite_set_action(obj, 0x1B, 0, 0);
    ((struct MenuTaskBits *)obj->task)->mode = 1;
    effect_spawn_at(0x2AAF, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    obj->callback = obj_setup_world_links;
}

u32 obj_step_path(struct MenuObj *obj, u32 arg)
{
    u32 result = partner_link_update(obj, arg, 0xFF);

    if (result != 0) {
        return result;
    }
    obj->stepAE++;
    if ((s16)obj->stepAE >= obj->unk9C) {
        sprite_set_action(obj, 3, 0, 0);
        ((struct MenuTaskBits *)obj->task)->mode = 1;
        obj->callback = screen_helpers_obj_release_when_idle;
    }
    effect_spawn_at(0x2B03, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, (void *)arg);
    return 0;
}

void screen_helpers_obj_release_when_idle(struct MenuObj *obj)
{
    if (step_motion(obj) == 0 || (obj->task->flags12 & 8) != 0) {
        sound_stop_song(0x10C);
        obj_release(obj);
    }
}

void obj_clear_when_unflagged(struct MenuObj *obj)
{
    if ((obj->flags76 & 0x38) == 0) {
        obj->callback = NULL;
        obj->entry->scene->mode113 = 0;
    }
}

void obj_end_wait(struct MenuObj *obj)
{
    obj_begin_wait(obj);
    obj->scene->mode113 = 0;
}

void obj_place_from_entry(struct MenuObj *obj)
{
    struct MenuCursor *cursor;
    struct MenuSub *sub;

    obj->callback = obj_start_if_free;
    obj = obj->owner->target;
    cursor = &gWorld->cells80[((struct MenuIndex *)obj->entry->scene)->index + 2]->cursor;
    obj->entry = (struct MenuEntry *)cursor;
    sub = cursor->sub;
    place_targets(obj, sub->unkD8 / 0x100 + 0x1C, sub->unkDC / 0x100 + 1, sub->unkE0 / 0x100 + 0x18, 0x100);
    set_motion_start(obj, 0x200);
}

void obj_unlink_if_flagged(struct MenuObj *obj)
{
    if ((obj->task->flags12 & 8) != 0) {
        gWorld->link60->unk54 = 0;
        gWorld->link64->unk54 = 0;
        sprite_set_action(obj, -1, -1, 0);
        obj->callback = NULL;
    }
}

void obj_start_if_free(struct MenuObj *obj)
{
    if (step_motion(obj->owner->target) == 0) {
        sound_stop_song(0xD5);
        obj->callback = obj_end_wait;
        obj->callback(obj);
    }
}

ASM_FUNC("asm/code_081152FC/menu_obj_step.s", void menu_obj_step(void))

void owner_method_noop(void)
{
}

void sub_081157F4(struct MenuObj *obj, u32 kind)
{
    obj->vtable = gMenuVtable;
    if (obj->child20 != NULL) {
        task_remove(obj->child20, 3);
    }
    if (obj->child24 != NULL) {
        menu_obj_free(obj->child24, 3);
    }
    task_remove(obj, kind);
}

struct MenuObj *sub_08115828(struct MenuObj *obj, u8 kind, const void *tag)
{
    struct MenuObj *child;

    task_insert(obj, kind, tag);
    obj->vtable = gMenuVtable;
    obj->cursor1C = 0;
    obj->state1D = 0;
    child = heap_alloc2(0x1C, 0, gTagSdrw, 0);
    task_insert(child, 8, gTagSdrw);
    child->vtable = gClassDd28;
    obj->child20 = child;
    obj->child24 = menu_obj_init(heap_alloc2(0x5C, 0, gTagFade, 0), 8, gTagFade, 1);
    obj->child20->parent14 = obj;
    *(vu16 *)0x02000000 = 0x1140;
    return obj;
}

struct MenuObj *sub_081158A0(struct MenuObj *obj, u8 kind, const void *tag)
{
    task_insert(obj, kind, tag);
    obj->vtable = gClassDd28;
    return obj;
}

struct MenuTable *menu_obj_init(struct MenuTable *obj, u8 kind, const u8 *tag, u8 flag)
{
    u32 entries;
    u16 zero;
    u16 *src = &zero;
    u8 count;

    count = flag;
    task_insert(obj, kind, tag);
    obj->vtable = gMenuTableVtable;
    obj->buf1C = heap_alloc(0x400, 0, gMenuObjBufTag, 0);
    obj->buf20 = heap_alloc(0x258, 0, gMenuObjBuf2Tag, 0);
    cpu_set(menu_obj_routine, obj->buf20, 0x04000096);
    obj->unk50 = obj->buf20;
    obj->unk54 = (u8 *)obj->buf20 + 0x88;
    obj->unk58 = (u8 *)obj->buf20 + 0x13C;
    obj->count24 = count;
    obj->flags28 = 0;
    entries = (count << 3) - count;
    obj->entries2C = heap_alloc(entries << 2, 0, gMenuObjTableTag, 0);
    zero = 0;
    cpu_set(src, obj->entries2C, ((entries << 1) & 0x001FFFFF) | 0x01000000);
    obj->unk32 = 0;
    obj->unk30 = 0;
    obj->unk34 = 0;
    obj->unk46 = 0;
    obj->unk48 = 0;
    obj->unk4A = 0;
    obj->unk36 = 0;
    obj->unk38 = 0;
    obj->unk44 = 0;
    return obj;
}

ASM_FUNC("asm/code_081152FC/sprite_alloc.s", void sprite_alloc(void))

ASM_FUNC("asm/code_081152FC/menu_entry_refresh.s", void menu_entry_refresh(void))

ASM_FUNC("asm/code_081152FC/sub_08115F54.s", void sub_08115F54(void))

ASM_FUNC("asm/code_081152FC/find_entry_code.s", void find_entry_code(void))

ASM_FUNC("asm/code_081152FC/menu_table_update.s", void menu_table_update(void))
