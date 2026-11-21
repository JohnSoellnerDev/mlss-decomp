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
    void *buf1C;     /* 0x400 bytes: copy of both palette shadows (BG, then OBJ) */
    void *buf20;     /* 0x258 bytes: RAM copy of the code at menu_obj_routine */
    u8 count24;      /* number of struct TableEntry in entries2C */
    u8 filler25[0x28 - 0x25];
    u32 flags28;
    void *entries2C; /* count24 struct TableEntry */
    /* Palette fade (started by sub_08115F54, drawn with obj_draw_tile_rows): */
    u16 unk30;  /* fade colour (BGR555) */
    u8 unk32;   /* PALETTE_FADE_* flags */
    u8 filler33;
    u16 unk34;  /* fade length in frames, nonzero while a fade runs */
    u16 unk36;  /* BG palette rows to fade (bit n = row n) */
    u16 unk38;  /* OBJ palette rows to fade (bit n = row n) */
    u8 filler3A[0x3C - 0x3A];
    s32 unk3C;  /* extra 0xA0 colours to fade: source */
    s32 unk40;  /* extra 0xA0 colours to fade: destination */
    u16 unk44;
    u16 unk46;  /* fade length / 16 */
    u8 unk48;   /* level step per update: 1 (towards the colour) or 0xFF (away from it) */
    u8 filler49;
    u16 unk4A;  /* fade level: 0 = original colours, unk34 = all fade colour */
    u16 u4C;
    u8 filler4E[0x50 - 0x4E];
    void *unk50; /* the three entry points of the routines in buf20: buf20 + 0, + 0x88, + 0x13C */
    void *unk54;
    void *unk58;
};

/* MenuTable.unk32: palette fade flags. */
#define PALETTE_FADE_NO_OBJ 1 /* no OBJ rows to fade */
#define PALETTE_FADE_NO_BG  2 /* no BG rows to fade */
#define PALETTE_FADE_OUT    4 /* fade from the colours to the fade colour (else from the colour back) */

extern struct MenuWorld *gWorld;
extern u16 gSys[];
extern u8 gUnk_02000080[];
extern u8 gUnk_02000280[];
extern const u8 gMenuVtable[];
extern const u8 gClassDd28[];
extern const u8 gTagSdrw[];
extern const u8 gTagFade[];
extern const u8 gMenuTableVtable[];
extern const u8 gMenuObjBufTag[];
extern const u8 gMenuObjBuf2Tag[];
extern const u8 gMenuObjTableTag[];
extern const u8 menu_obj_routine[];
extern const u8 gMenuDescSmall[];
extern const u8 gMenuDescLarge[];

struct EntryRef
{
    u8 filler0[0xA];
    u16 id;
};

/* One sprite slot of a MenuTable (obj->entries2C[count24]); filled by sprite_alloc,
 * cleared by menu_entry_refresh. */
struct TableEntry
{
    u8 byte0;     /* struct TableBits0 */
    u8 byte1;     /* struct TableNib: lo4 = palette row, hi4 = second palette row */
    u8 byte2;     /* struct TableNib: lo4 = alternate palette row */
    u8 used3;     /* nonzero while the slot holds a sprite */
    u8 byte4;
    u8 byte5;
    u16 unk6;
    u16 unk8;
    u8 filler0A[0xC - 0xA];
    void *tiles0C; /* the 16 colours (0x20 bytes) the palette row held before the sprite took it */
    void *buf10;   /* alternate palette (0x20 bytes) when mid3 & 6 */
    void *buf14;   /* second alternate palette (0x20 bytes), may be NULL */
    u8 byte18;
    u8 byte19;
    u8 filler1A[0x1C - 0x1A];
};

/* TableEntry.byte0 */
struct TableBits0
{
    u8 lo2 : 2;  /* slot state, 0 = free */
    u8 bit2 : 1; /* 1: OBJ palette, 0: BG palette */
    u8 mid3 : 3; /* palette mode; bits 1-2 set: the slot has alternate palettes */
    u8 hi2 : 2;
};

struct TableNib
{
    u8 lo4 : 4;
    u8 hi4 : 4;
};

struct TableBits0Packed
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 mid3 : 3;
    u8 hi2 : 2;
} __attribute__((packed));

struct TableNibPacked
{
    u8 lo4 : 4;
    u8 hi4 : 4;
} __attribute__((packed));

extern const struct EntryRef *const gEntryLookup0[];
extern const struct EntryRef *const gEntryLookup1[];
extern const struct EntryRef *const gEntryLookup2[];
extern const struct EntryRef *const gEntryLookup3[];
extern const struct EntryRef *const gEntryLookup6[];
extern const struct EntryRef *const gEntryLookup7[];
extern const struct EntryRef *const gEntryLookup8[];
extern const struct EntryRef *const gEntryLookup9[];
extern const struct EntryRef *const gEntryLookupA[];

u8 step_motion(void *obj);
void sound_stop_song(u32 song);
void sprite_set_action(void *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void task_insert(void *node, u8 kind, const void *tag);
void task_remove(void *node, u32 kind);
void menu_obj_free(void *obj, u32 kind);
void obj_release(void *obj);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
u32 sub_obj_running(void *menu);
void screen_obj_setup(void *obj, u8 prio, const void *arg);
void open_screen(void *obj, u8 kind, const void *arg, u16 flags);
void create_menu_object(s32 a, u32 b);
void sub_08115F54(struct MenuTable *obj, u8 mode, u16 pos, u32 speed, u32 arg5, u32 arg6, s32 arg7, s32 arg8);
void *heap_alloc(u32 size, u32 mode, const u8 *tag, u32 flags);
void cpu_set(const void *src, void *dst, u32 ctl);
void heap_free_a(void *p);
void obj_draw_tile_rows(void *obj, u32 a, const void *tiles, u32 b, u32 c, u32 d, u32 e);
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

struct SysPad
{
    u8 filler0[0x2A];
    u16 keys;
    u8 filler2C[0x2E - 0x2C];
    u16 repeat;
};

static inline void menu_obj_release(struct MenuObj *obj)
{
    if (obj != 0) {
        obj->vtable = gMenuVtable;
        if (obj->child20 != 0) {
            task_remove(obj->child20, 3);
        }
        if (obj->child24 != 0) {
            menu_obj_free(obj->child24, 3);
        }
        task_remove(obj, 3);
    }
}

/*
 * Per-frame update of a three-entry menu. state1D: 0 = choosing, 1 = waiting for the fade-out
 * started by entry 0, 2 = closing and opening the menu object. In state 0, A picks the entry under
 * cursor1C (0: fade out first, 1: open the small screen, 2: open the large screen), SELECT closes the
 * menu, and up/down move the cursor with wrap-around. sub_08115F54 starts a palette fade (mode 4 =
 * fade out) on the fader in child24; sub_obj_running returns 0 once it has finished.
 */
void menu_obj_step(struct MenuObj *obj)
{
    u16 pressed;
    s32 choice;
    s32 moved;
    void *screen;
    s32 select;

    switch (obj->state1D) {
    case 0:
        pressed = ((struct SysPad *)gSys)->keys;
        if ((pressed & A_BUTTON) != 0) {
            choice = *(s8 *)&obj->cursor1C;
            switch (choice) {
            case 0:
                obj->state1D = 1;
                sub_08115F54((struct MenuTable *)obj->child24, 4, 0, 0x10, 0x7FFF, 0xFFFF, choice, choice);
                break;
            case 1:
                menu_obj_release(obj);
                screen = heap_alloc2(0x54, 0, gMenuDescSmall, 0);
                screen_obj_setup(screen, 8, gMenuDescSmall);
                break;
            case 2:
                menu_obj_release(obj);
                screen = heap_alloc2(0x164, 0, gMenuDescLarge, 0);
                open_screen(screen, 8, gMenuDescLarge, 0xFFFF);
                break;
            }
            break;
        }
        select = SELECT_BUTTON;
        select &= pressed;
        if (select != 0) {
            menu_obj_release(obj);
            break;
        }
        if ((((struct SysPad *)gSys)->repeat & DPAD_UP) != 0) {
            moved = obj->cursor1C - 1;
            obj->cursor1C = moved;
            if ((moved << 24) < 0) {
                obj->cursor1C = 2;
            }
        }
        if ((((struct SysPad *)gSys)->repeat & DPAD_DOWN) != 0) {
            moved = obj->cursor1C + 1;
            obj->cursor1C = moved;
            if ((s8)moved > 2) {
                obj->cursor1C = 0;
            }
        }
        break;
    case 1:
        if ((u8)sub_obj_running(obj->child24) == 0) {
            obj->state1D = 2;
        }
        break;
    case 2:
        menu_obj_release(obj);
        create_menu_object(-1, 0);
        break;
    }
}

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

/* Sets up a palette manager task: inserts it as task `kind`, allocates the palette backup (buf1C) and a
   RAM copy of the routines at menu_obj_routine (buf20, entry points in unk50/54/58), and
   `entryCount` zeroed struct TableEntry slots; no fade is running afterwards. */
struct MenuTable *menu_obj_init(struct MenuTable *obj, u8 kind, const u8 *tag, u8 entryCount)
{
    u32 entryWords;
    u16 zero;
    u16 *src = &zero;
    u8 count;

    count = entryCount;
    task_insert(obj, kind, tag);
    obj->vtable = gMenuTableVtable;
    obj->buf1C = heap_alloc(0x400, 0, gMenuObjBufTag, 0);
    obj->buf20 = heap_alloc(0x258, 0, gMenuObjBuf2Tag, 0);
    cpu_set(menu_obj_routine, obj->buf20, 0x04000096); /* 0x96 words */
    obj->unk50 = obj->buf20;
    obj->unk54 = (u8 *)obj->buf20 + 0x88;
    obj->unk58 = (u8 *)obj->buf20 + 0x13C;
    obj->count24 = count;
    obj->flags28 = 0;
    entryWords = (count << 3) - count; /* sizeof(struct TableEntry) / 4 per entry */
    obj->entries2C = heap_alloc(entryWords << 2, 0, gMenuObjTableTag, 0);
    zero = 0;
    cpu_set(src, obj->entries2C, ((entryWords << 1) & 0x001FFFFF) | 0x01000000); /* fill halfwords with 0 */
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

struct RecordTables
{
    void *table[11];
};

extern const u8 gRecordTableRom[];
extern const u8 gSpriteBufTagFirst[];
extern const u8 gSpriteBufTagSecond[];
extern const u8 gSpriteBufTagThird[];

static inline struct TableEntry *sprite_entry_at(struct MenuTable *obj, u32 offset)
{
    return (struct TableEntry *)((u8 *)obj->entries2C + offset);
}

static inline void *sprite_record(void *table, u32 keyX4)
{
    return *(void **)((u8 *)table + keyX4);
}

static inline u16 record_h(void *rec, u32 off)
{
    return *(u16 *)((u8 *)rec + off);
}

static inline u8 record_b(void *rec, u32 off)
{
    return *((u8 *)rec + off);
}

NONMATCH("asm/code_081152FC/sprite_alloc.s", u8 sprite_alloc(struct MenuTable *obj, u8 mode, u16 key, u8 attr))
{
    struct RecordTables tables;
    u32 slot;
    void *recTable;
    u32 slotX8;
    u32 keyX4;
    u32 off;
    u32 palette;
    u32 index;
    u32 keyTable;
    struct TableEntry *walk;
    struct TableEntry *e;
    void *buf;
    u32 paletteBits;

    tables = *(const struct RecordTables *)gRecordTableRom;
    if (mode == 0xFF) {
        index = 0;
        walk = (struct TableEntry *)obj->entries2C;
        while (index < obj->count24 && (walk->byte0 & 3) != 0) {
            index++;
            walk++;
        }
        if (index == obj->count24) {
            return 0xFF;
        }
        slot = index;
    } else {
        slot = mode;
    }

    keyTable = key >> 12;
    if (keyTable >= 11) {
        return 0xFF;
    }
    recTable = tables.table[keyTable];
    slotX8 = slot << 3;
    off = (slotX8 - slot) << 2;
    e = sprite_entry_at(obj, off);
    ((struct TableBits0 *)&e->byte0)->lo2 = 1;
    keyX4 = (key & 0xFFF) << 2;
    ((struct TableBits0 *)&e->byte0)->mid3 = (record_h(sprite_record(recTable, keyX4), 0) & 0x70) >> 4;

    if (attr != 0xFF) {
        ((struct TableBits0 *)&e->byte0)->bit2 = (attr >> 4) & 1;
        palette = attr & 0xF;
    } else {
        ((struct TableBits0 *)&e->byte0)->bit2 = (record_h(sprite_record(recTable, keyX4), 0) >> 7) & 1;
        palette = record_b(sprite_record(recTable, keyX4), 0) & 0xF;
    }
    ((struct TableNib *)&e->byte1)->lo4 = palette;

    ((struct TableNib *)&e->byte1)->hi4 = (record_h(sprite_record(recTable, keyX4), 2) & 0xF0) >> 4;
    ((struct TableNib *)&e->byte2)->lo4 = record_b(sprite_record(recTable, keyX4), 2) & 0xF;
    e->used3 = record_h(sprite_record(recTable, keyX4), 4);
    ((struct TableNib *)&e->byte4)->lo4 = ((record_h(sprite_record(recTable, keyX4), 6) & 0xF0) >> 4) & 0xF;
    ((struct TableNib *)&e->byte4)->hi4 = record_b(sprite_record(recTable, keyX4), 6) & 0xF;
    if (record_h(sprite_record(recTable, keyX4), 6) != 0) {
        e->byte5 = record_h(sprite_record(recTable, keyX4), 8);
    }
    e->unk6 = 0;
    e->unk8 = 0;
    e->byte18 = 1;
    ((struct TableNib *)&e->byte19)->lo4 = 0;
    ((struct TableNib *)&e->byte19)->hi4 = 0;

    buf = heap_alloc(0x20, 0, gSpriteBufTagFirst, 0);
    e->tiles0C = buf;

    obj->u4C = (((struct TableBits0 *)&e->byte0)->bit2 << 8) + (((struct TableNib *)&e->byte1)->lo4 << 4);
    cpu_set(gUnk_02000080 + (obj->u4C << 1), obj->buf1C + (obj->u4C << 1), 0x04000008);
    cpu_set(gUnk_02000080 + (obj->u4C << 1), e->tiles0C, 0x04000008);

    if ((((struct TableBits0 *)&e->byte0)->mid3 & 6) != 0) {
        buf = heap_alloc(0x20, 0, gSpriteBufTagSecond, 0);
        e->buf10 = buf;
        if (((struct TableNib *)&e->byte2)->lo4 == ((struct TableNib *)&e->byte1)->lo4
            || (e->byte0 & 4) != 0) {
            if ((*((u8 *)gSys + 0x888) & 6) == 0) {
                cpu_set((u8 *)sprite_record(recTable, keyX4) + 0xC, e->buf10, 0x04000008);
            } else {
                cpu_set((u8 *)sprite_record(recTable, keyX4) + 0x2C, e->buf10, 0x04000008);
            }
            if (((struct TableNib *)&e->byte1)->hi4 == ((struct TableNib *)&e->byte2)->lo4) {
                buf = heap_alloc(0x20, 0, gSpriteBufTagThird, 0);
                e->buf14 = buf;
                if ((*((u8 *)gSys + 0x888) & 6) == 0) {
                    cpu_set((u8 *)sprite_record(recTable, keyX4) + 0x4C, e->buf14, 0x04000008);
                } else {
                    cpu_set((u8 *)sprite_record(recTable, keyX4) + 0x6C, e->buf14, 0x04000008);
                }
            } else {
                e->buf14 = 0;
            }
        } else {
            obj->u4C = ((e->byte0 >> 2) & 1) << 8;
            obj->u4C = obj->u4C + (((struct TableNib *)&e->byte2)->lo4 << 4);
            cpu_set(gUnk_02000080 + (obj->u4C << 1), e->buf10, 0x04000008);
            if ((e->byte1 & 0xF0) == 0xF0 || ((struct TableNib *)&e->byte1)->hi4 == ((struct TableNib *)&e->byte1)->lo4) {
                e->buf14 = 0;
            } else {
                buf = heap_alloc(0x20, 0, gSpriteBufTagThird, 0);
                e->buf14 = buf;
                obj->u4C = (((e->byte0 >> 2) & 1) << 8) + (((struct TableNib *)&e->byte1)->hi4 << 4);
                cpu_set(gUnk_02000080 + (obj->u4C << 1), e->buf14, 0x04000008);
            }
        }
    }

    e = sprite_entry_at(obj, off);
    paletteBits = 1 << ((struct TableNib *)&e->byte1)->lo4;
    paletteBits <<= ((e->byte0 >> 2) & 1) << 4;
    obj->flags28 |= paletteBits;
    if ((e->byte0 & 0x38) == 0) {
        if (e->byte5 != 0) {
            e->unk8 = 0xFFFF;
        }
    }
    return (u8)slot;
}
END_NONMATCH

static inline struct TableEntry *menu_entry_at(struct MenuTable *obj, u32 offset)
{
    return (struct TableEntry *)(offset + (u32)obj->entries2C);
}

/* Frees sprite slot `slotIndex` of `obj` (the counterpart of sprite_alloc): releases its palette
 * row bit in flags28, optionally copies the row's saved colours back into the palette buffer and
 * marks the row dirty (gSys[0]: OBJ rows, gSys[1]: BG rows), frees its palette copies and clears
 * the slot. Returns 0 if the slot was already empty, 1 otherwise. */
u32 menu_entry_refresh(struct MenuTable *obj, u8 slotIndex, u8 restorePalette)
{
    u32 paletteBit;
    u32 palMode;
    u32 hasAltPalettes;
    struct TableEntry *entries;
    u32 slotOffset;
    u32 clearOffset;
    struct TableEntry *slot;
    struct TableEntry *clearEntries;

    entries = obj->entries2C;
    slotOffset = ((slotIndex << 3) - slotIndex) << 2; /* slotIndex * sizeof(struct TableEntry) */
    slot = (struct TableEntry *)(slotOffset + (u32)entries);
    if (slot->used3 == 0) {
        return 0;
    }
    paletteBit = (1 << ((struct TableNibPacked *)&slot->byte1)->lo4) << (((struct TableBits0Packed *)&slot->byte0)->bit2 << 4);
    obj->flags28 ^= paletteBit;
    /* first colour of the palette row: BG rows 0-15, OBJ rows 16-31 */
    obj->u4C = (((struct TableBits0Packed *)&slot->byte0)->bit2 << 8) + (((struct TableNibPacked *)&slot->byte1)->lo4 << 4);
    if (restorePalette != 0) {
        cpu_set(menu_entry_at(obj, slotOffset)->tiles0C, gUnk_02000080 + (obj->u4C << 1), 0x04000008);
        slot = menu_entry_at(obj, slotOffset);
        if (((struct TableBits0Packed *)&slot->byte0)->bit2) {
            gSys[0] |= 1 << ((struct TableNibPacked *)&slot->byte1)->lo4;
        } else {
            gSys[1] |= 1 << ((struct TableNibPacked *)&slot->byte1)->lo4;
        }
    }
    entries = obj->entries2C;
    slotOffset = ((slotIndex << 3) - slotIndex) << 2;
    slot = (struct TableEntry *)(slotOffset + (u32)entries);
    palMode = ((struct TableBits0Packed *)&slot->byte0)->mid3;
    hasAltPalettes = 6;
    hasAltPalettes &= palMode;
    if (hasAltPalettes != 0) {
        if (slot->buf14 != 0) {
            heap_free_a(slot->buf14);
        }
        if (menu_entry_at(obj, slotOffset)->buf10 != 0) {
            heap_free_a(menu_entry_at(obj, slotOffset)->buf10);
        }
    }
    clearEntries = obj->entries2C;
    clearOffset = ((slotIndex << 3) - slotIndex) << 2;
    if (((struct TableEntry *)(clearOffset + (u32)clearEntries))->tiles0C != 0) {
        heap_free_a(((struct TableEntry *)(clearOffset + (u32)clearEntries))->tiles0C);
    }
    ((struct TableBits0 *)&menu_entry_at(obj, clearOffset)->byte0)->lo2 = 0;
    ((struct TableBits0 *)&menu_entry_at(obj, clearOffset)->byte0)->bit2 = 0;
    ((struct TableBits0 *)&menu_entry_at(obj, clearOffset)->byte0)->mid3 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte1)->lo4 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte1)->hi4 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte2)->lo4 = 0;
    menu_entry_at(obj, clearOffset)->used3 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte4)->lo4 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte4)->hi4 = 0;
    menu_entry_at(obj, clearOffset)->byte5 = 0;
    menu_entry_at(obj, clearOffset)->unk6 = 0;
    menu_entry_at(obj, clearOffset)->unk8 = 0;
    menu_entry_at(obj, clearOffset)->byte18 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte19)->lo4 = 0;
    ((struct TableNib *)&menu_entry_at(obj, clearOffset)->byte19)->hi4 = 0;
    return 1;
}

/* Starts a palette fade on `obj` unless one is running: the BG rows in bgRows and the OBJ rows in
 * objRows (plus 0xA0 colours from extraSrc to extraDst, when both are set) blend towards `colour`
 * over `frames` frames (flags & PALETTE_FADE_OUT), or start as `colour` and blend back. Both
 * palette shadows (gUnk_02000080: BG rows, gUnk_02000280: OBJ rows) are saved in buf1C first. */
void sub_08115F54(struct MenuTable *obj, u8 mode, u16 colour, u32 frames, u32 bgRows, u32 objRows, s32 extraSrc, s32 extraDst)
{
    u8 *flags;
    u16 bgMask;
    u16 objMask;
    u16 length;
    u32 framesHi;

    framesHi = frames << 16;
    length = framesHi >> 16;
    bgMask = bgRows;
    objMask = objRows;
    if (obj->unk34 != 0) {
        return;
    }
    obj->unk32 = mode;
    flags = &obj->unk32;
    if (objMask == 0) {
        *flags = mode | PALETTE_FADE_NO_OBJ;
    }
    if (bgMask == 0) {
        *flags |= PALETTE_FADE_NO_BG;
    }
    obj->unk44 = 0;
    obj->unk30 = colour;
    obj->unk34 = length;
    obj->unk46 = framesHi >> 20;
    if ((*flags & PALETTE_FADE_OUT) != 0) {
        obj->unk48 = 1;
        obj->unk4A = 0;
    } else {
        obj->unk48 = 0xFF;
        obj->unk4A = length;
    }
    obj->unk36 = bgMask;
    obj->unk38 = objMask;
    obj->unk3C = extraSrc;
    obj->unk40 = extraDst;
    cpu_set(gUnk_02000080, obj->buf1C, 0x04000100);
    if ((*flags & PALETTE_FADE_OUT) != 0) {
        return;
    }
    /* fading back in: fill the rows with the fade colour now, and mark them for upload */
    obj_draw_tile_rows(obj, 0, gUnk_02000080, 0, obj->unk36, colour, 1);
    obj_draw_tile_rows(obj, 0, gUnk_02000280, 0, obj->unk38, colour, 1);
    gSys[1] |= obj->unk36;
    gSys[0] |= obj->unk38;
}

/* Looks up a graphic key (bits 12-15: bank + 1, bits 0-11: id) in the lookup table of its bank and
   returns its code, (table number << 12) | index in that table, or 0xFFFF when no entry of the
   table has that id. The first argument (gWorld->table248 at the call) is not used. */
u32 find_entry_code(u32 unused, u32 key)
{
    const struct EntryRef *const *entries;
    u32 shifted = key << 16;
    u16 id = shifted >> 16;
    u16 bank = (shifted >> 28) - 1;
    s32 count;
    u32 table;
    s32 i;

    switch (bank) {
    case 3:
        entries = gEntryLookup0;
        count = 0x30;
        table = 0;
        break;
    case 1:
        entries = gEntryLookup1;
        count = 0x1A;
        table = 1;
        break;
    case 10:
        entries = gEntryLookup9;
        count = 1;
        table = 9;
        break;
    case 2:
        entries = gEntryLookup2;
        count = 9;
        table = 2;
        break;
    case 7:
        entries = gEntryLookup7;
        count = 0xB;
        table = 7;
        break;
    case 8:
        entries = gEntryLookup8;
        count = 1;
        table = 8;
        break;
    case 0:
        entries = gEntryLookup3;
        count = 1;
        table = 3;
        break;
    case 9:
        entries = gEntryLookupA;
        count = 2;
        table = 0xA;
        break;
    case 4:
        entries = gEntryLookup6;
        count = 1;
        table = 6;
        break;
    default:
        entries = NULL;
        count = 0;
        table = 0;
        break;
    }
    for (i = 0; i < count; i++) {
        if (entries[i]->id == id) {
            return (u16)((table << 12) | i);
        }
    }
    return 0xFFFF;
}

ASM_FUNC("asm/code_081152FC/menu_table_update.s", void menu_table_update(void))
