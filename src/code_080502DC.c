#include "global.h"

struct VtSlot
{
    s16 offset;
    u16 filler2;
    u32 (*fn)();
};

struct ObjVtable
{
    u8 filler0[8];
    struct VtSlot slot8;
    struct VtSlot slot10;
    struct VtSlot slot18;
    s16 thisOffset;
    u16 filler22;
    void (*update)(void *);
    struct VtSlot slot28;
    struct VtSlot slot30;
    u8 filler38[8];
    struct VtSlot slot40;
    struct VtSlot slot48;
    struct VtSlot slot50;
    struct VtSlot slot58;
    struct VtSlot slot60;
    struct VtSlot slot68;
    struct VtSlot slot70;
    u8 filler78[0x10];
    struct VtSlot slot88;
};

struct ChildObj
{
    u8 filler0[0x22C];
    u32 unk22C_0 : 10;
    u32 unk22C_10 : 12;
    u32 unk22C_22 : 10;
    u8 filler230[0x338 - 0x230];
    struct ObjVtable *vtable;
    u8 filler33C[2];
    u8 flags;
    u8 filler33F[0x344 - 0x33F];
    struct ChildObj *child;
};

struct SpriteObj
{
    s16 posX;
    s16 posY;
    u8 filler4[0xA];
    u16 unkE;
    u8 filler10[0x1F - 0x10];
    u8 flags1F;
};

struct MainObj
{
    u8 filler0[0x0C];
    s32 x;
    s32 y;
    u8 filler14[0x20F - 0x14];
    u8 flags20F;
    u8 filler210[0x214 - 0x210];
    u32 unk214_0 : 13;
    u32 unk214_13 : 4;
    u32 unk214_17 : 15;
    u8 filler218[0x23D - 0x218];
    u8 unk23D;
    u8 filler23E[0x294 - 0x23E];
    struct SpriteObj *sprite;
};

struct ListHeadX
{
    u8 filler0[0x3C];
    u32 head3C;
};

struct Bit0
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct Bit1
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct ObjTimer
{
    u8 filler0[0x23B];
    u8 unk23B;
    u8 filler23C[0x33E - 0x23C];
    u16 unk33E;
    s16 timer340;
};

struct HookObj
{
    u8 filler0[0x198];
    void (*hook198)();
    u8 filler19C[4];
    void (*hook1A0)();
};

struct FramePos
{
    u16 x;
    u16 y;
};

/* The map screen: a map larger than the screen, scrolled with the d-pad, with a marker
   on it and an arrow at each edge there is more map beyond. */
struct MapWindow
{
    u8 filler0[0x1C];
    s16 scrollX;
    s16 scrollY;
    u16 mapWidth;       /* pixels */
    u16 mapHeight;
    u16 mapLayout;      /* given to map_obj_start */
    u16 markerX;        /* on the map */
    u16 markerY;
    u16 id;             /* looked up in gWindowSizeTable */
    u32 arrowUp;        /* edge arrows, in gWindowSpritePoints order */
    u32 arrowRight;
    u32 arrowDown;
    u32 arrowLeft;
    u8 *marker;
    u8 *block;
    u8 *map;
    u8 *fader;          /* fades the screen in and out */
};

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

struct Lo4Bits
{
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 rest : 4;
};

struct Mid8Bits
{
    u16 b0 : 1;
    u16 mid : 8;
    u16 rest : 7;
};

struct Mode3
{
    u8 mode : 3;
    u8 rest : 5;
};

struct Mid2At1
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct SpriteFlags
{
    u8 filler0[0x1F];
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 b45 : 2;
    u8 b67 : 2;
};

struct Bits8
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct Word20C
{
    u32 lo15 : 15;
    u32 mv : 3;
    u32 rest : 14;
};

struct Bits33C
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 mid3 : 3;
    u8 rest : 2;
};

struct Mid4At6
{
    u16 lo6 : 6;
    u16 mid4 : 4;
    u16 rest : 6;
};

/* animation step counter in bits 1-8 of the halfword at 0x33E */
struct AnimStepP
{
    u16 b0 : 1;
    s16 step : 8;
    u16 rest : 7;
};

/* a sprite's attributes saved in its object (obj_save_sprite_attrs, obj_copy_sub_fields) so a
 * new sprite can take them over; each field names the SpriteObj field it is saved from */
struct ObjShadow298
{
    u8 filler0[0x298];
    u8 active : 1;   /* the fields below hold a saved sprite */
    u8 mirror : 1;   /* 0x11 bit 6 */
    u8 b23 : 2;      /* 0x1F bits 0-1 */
    u8 b45 : 2;      /* 0x1F bits 2-3 */
    u8 b67 : 2;      /* 0x12 bits 1-2 */
    u8 b0 : 1;       /* 0x12 bit 3 */
    u8 b1 : 1;       /* 0x12 bit 4 */
    u8 rest299 : 6;
    u8 filler29A;
    u8 unk29B;       /* 0x21 */
    u8 unk29C;       /* 0x23 */
    u8 filler29D;
    u16 unk29E;      /* 0x2A */
    u16 unk2A0;      /* 0x2C */
    u16 unk2A2;      /* 0x04 */
    u16 unk2A4;      /* 0x06 */
};

/* object flag bits at 0x33C that obj_replace_sprite tests */
#define OBJ_FLAG_HIDDEN      1 /* no pending sprite takes over the old one's attributes */
#define OBJ_FLAG_REPLACE_NOW 4 /* the sprite is replaced at once, no pending sprite */

/* animation level in bits 10-16 of the word at 0x33C (a hit block: how many of its drop it gave) */
struct ObjAnimWord33C
{
    u8 filler0[0x33C];
    u32 lo10 : 10;
    u32 level : 7;
    u32 rest : 15;
};

/* animation frame in bits 6-9 of the halfword at 0x33C (a hit block: what its drop looks like, drop_roll bits 0-3) */
struct ObjAnimHalf33C
{
    u8 filler0[0x33C];
    u16 lo6 : 6;
    u16 frame : 4;
    u16 rest : 6;
};

u32 drop_roll(u16 index, u8 kind, u8 flag);

/* a hit block's repeat hits (obj_apply_anim_code) */
struct ObjTimers340
{
    u8 filler0[0x340];
    s16 timer340; /* hits left */
    s16 timer342; /* frames left for the next hit, -1 before the first one */
    u16 unk344;
};

/* width field of a sprite, raised to the measured size of its graphics */
struct SpriteW28
{
    u8 filler0[0x28];
    u16 width : 9;
    u16 rest : 7;
};

void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
u32 read_varlen_plus1(const void *src);
void obj_load_part_shape(u8 *obj);

/* window task header: state, linked block and class pointer */
struct WinHead
{
    u8 state;
    u8 filler1[0x13];
    u8 *block14;
    u32 class18;
};

/* held and newly pressed keys */
struct SysKeys28
{
    u8 filler0[0x28];
    u16 held;
    u16 pressed;
};

extern u8 gUnk_03000C80[];
extern struct FramePos gWindowSpritePoints[];
extern s8 gWindowScrollTable[];
void set_flags_2_3(void *p);
void copy_track_limits(u8 *obj, u16 track, u16 *outA, u16 *outB);
void scroll_clamp(struct MapWindow *window);
void sub_08115F54(void *sub, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
u8 sub_obj_running(void *menu);
void set_slot_value(u32 id, s32 value);
void run_pending_obj_callbacks(void);
void obj_release_parts(void *parts, u32 mode);
void task_remove(void *node, u32 kind);
void menu_obj_free(void *obj, u32 kind);
void update_obj_list(void);

/* flag byte at 0x20C */
struct Flags20C
{
    u8 b0 : 1;
    u8 mid : 3;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

void obj_set_or_add_pos(u8 *obj, s32 x, s32 y, s32 z, u32 copy, u32 add);

/* state bits of an object and the link flags that update them */
struct ObjLink33C
{
    u8 filler0[0x214];
    u8 state : 3;
    u8 rest214 : 5;
    u8 filler215[0x33C - 0x215];
    u8 b0 : 1;
    u8 b1 : 1;
    u8 childState : 3;
    u8 rest : 2;
    u8 restore : 1;
    u8 saved : 3;
    u8 rest33D : 5;
};

struct ObjInitS
{
    u8 filler0[0x23B];
    u8 unk23B;
    u8 filler23C[0x338 - 0x23C];
    struct ObjVtable *vtable;
    u8 flags;
    u8 filler33D;
    u16 mask;
    s16 timer340;
    u16 timerB342;
    s16 unk344;     /* hit block: its id (BLOCK_HIT_FLAG), -1 for none */
    u32 unk348;     /* hit block: its 6-byte map entry (obj_init_from_entry) */
    u32 unk34C;     /* pending sprite (obj_activate_sprite) */
};

/* the object's pending sprite (ObjInitS.unk34C), built in the background and swapped in later */
#define OBJ_PENDING_SPRITE(obj) (*(struct SpriteObj **)((obj) + 0x34C))

extern u8 *gUnk_03000FC0;
/* the game's flags (test_flag, set_flag) */
#define GAME_FLAGS (gUnk_03000FC0 + 0x38C)
/* game flag set once the map's hit block `id` has been hit */
#define BLOCK_HIT_FLAG(id) ((u16)((id) + 0x1360))
extern const u16 gSpriteSelectAnimTable[];
void set_flag(u8 *flags, u32 a, u32 b, u32 c);
void reset_record_fields(u8 *obj);
void obj_random_pick(u32 r0, u8 *obj, u32 *state);
void obj_bind_callbacks(u32 arg, u8 *obj);
void obj_call_hook_1a0(u32 a0, u8 *obj);
void obj_create_sub(u8 *obj, u32 a, u32 b, u32 d);
void obj_copy_sub_fields(u8 *obj);
void obj_refresh_dirty(u8 *obj);
void obj_refresh_motion_state(u8 *obj, u32 a);
void obj_update_screen_pos(u8 *obj, u32 a, u32 b, u32 d);
void obj_update_cell_levels(u8 *obj);
u32 obj_emit_entry(u8 *obj, u32 a, u32 b);
void obj_reset_link_fields(u8 *obj);
void obj_resolve_contacts(u8 *obj, u32 a, u32 b, u32 d);
u32 obj_step_toward_goal(u8 *obj);
void obj_update_link_pos(u8 *obj, u32 a, u32 b);
void obj_step_sequence(u8 *obj);
void obj_sync_sprite_link(u8 *obj);
void obj_queue_update(struct SpriteObj *sprite);
void obj_list_pass_0a8c(void);
void obj_init(u8 *obj, u32 a, u32 b);
void init_object_from_def(u32 d, u8 *child, u8 *obj);
void heap_free_b(void *p);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
void wander_to_edge(u8 *obj, u16 a, s32 b);
u32 angle_of_vector16(s32 x, s32 y);
void obj_random_enter(u32 owner, u8 *obj, u32 *state);
void obj_release_slots(u32 sprite);
void sprite_update_cd0(u32 sprite);
u8 step_pos_xy_c(u8 *obj);
u32 zone_overlap_code(u32 owner, u8 *obj, u32 c);
void tick_store(void);
void unlink_d54(void *ctl);
void obj_load_record(void *rec, u32 a, u32 b, u32 c, u32 d);

extern const u8 gObjBaseClass[];
extern const u8 gObjForwardChildClass[];
extern const u8 gObjAttachedChildClass[];
extern const u8 gObjFlagsClass[];
extern const u8 gObjStateClass[];
u8 test_flag(u8 *table, u8 *tableB, u16 id);

extern u8 gSys[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];

/* Suspends the object: deactivates it (0x20C bit 0) and puts it in link mode 2, which
   obj_update_flags_from_child later undoes. A move still running (0x33E bits 0-1) is ended first:
   the object lands (0x18 = 0) and the state to restore is saved. Then vtable slot 0x80 is called
   with 1 on the object and, if one is linked to it (0x20F bit 1), on its child, deactivated too. */
void obj_set_mode_4(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *slot;
    u32 flag;
    u32 savedState;

    ((struct Bit0 *)(obj + 0x20C))->bit0 = 0;
    ((struct Mid2At1 *)(obj + 0x33C))->mode = 2;
    if ((obj[0x33E] & 3) != 0)
    {
        ((struct Lo4Bits *)(obj + 0x33E))->lo2 = 0;
        *(u32 *)(obj + 0x18) = 0;
        flag = ((u32)obj[0x33E] << 29) >> 31;
        ((struct Bits8 *)(obj + 0x20D))->b5 = flag;
        obj[2] = obj[0x33F];
        obj[0x23A] = 0;
        savedState = ((u32)obj[0x33C] << 25) >> 29;
        ((struct ObjLink33C *)obj)->saved = savedState;
        ((struct Bits8 *)(obj + 0x33C))->b3 = 0;
    }
    slot = (struct VtSlot *)((u8 *)((struct ChildObj *)obj)->vtable + 0x80);
    slot->fn(obj + slot->offset, 1);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL && (((u8 *)child)[0x20F] & 2))
    {
        struct ChildObj *linked;
        struct VtSlot *childSlot;

        ((struct Bit0 *)((u8 *)child + 0x20C))->bit0 = 0;
        linked = ((struct ChildObj *)obj)->child;
        childSlot = (struct VtSlot *)((u8 *)linked->vtable + 0x80);
        childSlot->fn((u8 *)linked + childSlot->offset, 1);
    }
}

/* Ends the object's link mode (0x33C bits 1-2) unless it is busy (0x212 bits 7-8). From mode 2,
   which obj_set_mode_4 suspends the object with, it is resumed first: it and its linked child
   (0x20F bit 1) are reactivated (0x20C bit 0), the child takes its pending state (0x33C bit 1), the
   object gets back the state obj_set_mode_4 saved (0x33D bit 7), and 0x33C bit 0 is set. */
void obj_update_flags_from_child(u8 *obj)
{
    u8 **pChild;
    u32 linkMode;

    linkMode = obj[0x33C] & 6;
    if (linkMode != 0)
    {
        if ((*(u16 *)(obj + 0x212) & 0x180) == 0)
        {
            if (linkMode == 4)
            {
                ((struct Bit0 *)(obj + 0x20C))->bit0 = 1;
                pChild = (u8 **)(obj + 0x344);
                if (*pChild != NULL)
                {
                    if ((*pChild)[0x20F] & 2)
                    {
                        ((struct Bit0 *)((*pChild) + 0x20C))->bit0 = 1;
                        /* a linked child takes over its pending state */
                        if (((struct ObjLink33C *)*pChild)->b1)
                        {
                            ((struct ObjLink33C *)*pChild)->state = ((struct ObjLink33C *)*pChild)->childState;
                            ((struct ObjLink33C *)*pChild)->b1 = 0;
                        }
                    }
                }
                if (((struct ObjLink33C *)obj)->restore)
                {
                    ((struct ObjLink33C *)obj)->state = ((struct ObjLink33C *)obj)->saved;
                    ((struct ObjLink33C *)obj)->restore = 0;
                }
                ((struct Bit0 *)(obj + 0x33C))->bit0 = 1;
            }
            ((struct Mid2At1 *)(obj + 0x33C))->mode = 0;
        }
    }
}

u32 obj_attach_child(u8 *obj, u32 a, u32 b, u32 d, u8 *child)
{
    u8 d8;

    d8 = d;
    obj_init(obj, a, b);
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjAttachedChildClass;
    init_object_from_def(d8, child, obj);
    ((struct Bit0 *)(obj + 0x20C))->bit0 = 0;
    ((struct Bit1 *)(obj + 0x20F))->bit1 = 0;
    ((struct ChildObj *)obj)->child = (struct ChildObj *)child;
    return (u32)obj;
}

void forward_child_70(u8 *obj, u32 a, u32 b)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u8 b8;

    b8 = b;
    obj_update_link_pos(obj, a, b8);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x70);
        entry->fn((u8 *)child + entry->offset, a, b8);
    }
}

u32 forward_child_68(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u32 result;

    result = obj_step_toward_goal(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x68);
        result = entry->fn((u8 *)child + entry->offset);
    }
    return result;
}

void forward_child_60(u8 *obj, u32 a, u32 b, u32 d)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u8 d8;

    d8 = d;
    obj_resolve_contacts(obj, a, b, d8);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x60);
        entry->fn((u8 *)child + entry->offset, a, b, d8);
    }
}

void forward_child_58(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;

    obj_reset_link_fields(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x58);
        entry->fn((u8 *)child + entry->offset);
    }
}

u32 forward_child_50(u8 *obj, u32 a, u32 b)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    u32 result;

    result = obj_emit_entry(obj, a, b);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x50);
        result = entry->fn((u8 *)child + entry->offset, a, b);
    }
    return result;
}

void forward_child_88(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;

    obj_step_sequence(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x88);
        entry->fn((u8 *)child + entry->offset);
    }
}

void forward_child_48(u8 *obj)
{
    struct ChildObj *child;
    struct VtSlot *entry;

    obj_update_cell_levels(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x48);
        entry->fn((u8 *)child + entry->offset);
    }
}

void forward_child_40(u8 *obj, u32 a, u32 b, u32 d)
{
    struct ChildObj *child;
    struct VtSlot *entry;
    s32 x;
    s32 y;

    x = (s16)a;
    y = (s16)b;
    obj_update_screen_pos(obj, x, y, d);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        entry = (struct VtSlot *)((u8 *)child->vtable + 0x40);
        entry->fn((u8 *)child + entry->offset, x, y, d);
    }
}

void forward_child_flag_20(u8 *obj)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_sync_sprite_link(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        if (((u8 *)child)[0x20C] & 1)
        {
            vt = child->vtable;
            vt->update((u8 *)child + vt->thisOffset);
        }
    }
}

void forward_child_28(u8 *obj)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_refresh_dirty(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot28.fn((u8 *)child + vt->slot28.offset);
    }
}

void forward_child_30(u8 *obj, u32 a)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_refresh_motion_state(obj, a);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot30.fn((u8 *)child + vt->slot30.offset, a);
    }
}

void forward_child_18(u8 *obj)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_copy_sub_fields(obj);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot18.fn((u8 *)child + vt->slot18.offset);
    }
}

void forward_child_10(u8 *obj, u32 a, u32 b, u32 d)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    obj_create_sub(obj, a, b, d);
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot10.fn((u8 *)child + vt->slot10.offset, a, b, d);
    }
}

void obj_update_child(struct ChildObj *obj)
{
    struct ObjVtable *vtable;

    set_flag(gUnk_03000FC0 + 0x38C, 0, obj->unk22C_10 + 0x960, 0);
    if (obj->flags & 0x10)
    {
        ((u8 *)obj->child)[0x20C] |= 1;
        vtable = obj->vtable;
        vtable->update((u8 *)obj + vtable->thisOffset);
    }
}

static inline void obj_base_teardown(u8 *obj, u32 flags)
{
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjBaseClass;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

void obj_teardown(u8 *obj, u32 flags)
{
    struct ChildObj *child;
    struct ObjVtable *vt;

    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjForwardChildClass;
    child = ((struct ChildObj *)obj)->child;
    if (child != NULL)
    {
        vt = child->vtable;
        vt->slot8.fn((u8 *)child + vt->slot8.offset, 3);
    }
    obj_base_teardown(obj, flags);
}

/* Constructs an object of class gObjStateClass: the base object (obj_init, loading definition def
 * unless it is -1), then clears its state flags, sets the mask bits 1-8, timer340 to 1 and leaves
 * it without a hit block (unk344 -1) or pending sprite. Returns obj. */
u32 obj_init_state(u8 *obj, u32 def, u32 index)
{
    struct ObjInitS *self;
    u16 mask;

    obj_init(obj, def, index);
    self = (struct ObjInitS *)obj;
    self->vtable = (struct ObjVtable *)gObjStateClass;
    ((struct Bits33C *)&self->flags)->b0 = 1;
    ((struct Bits33C *)&self->flags)->mid3 = 0;
    ((struct Bits33C *)&self->flags)->b1 = 0;
    ((struct Bits33C *)&self->flags)->b2 = 0;
    ((struct Mid4At6 *)&self->flags)->mid4 = 0;
    self->unk23B = 0;
    mask = self->mask;
    self->mask = mask | 0x1FE;
    self->timer340 = 1;
    self->timerB342 = 0xFFFF;
    self->unk344 = -1;
    self->unk348 = 0;
    self->unk34C = 0;
    return (u32)obj;
}

/* Sets up a hit block from its 6-byte map entry: places it at the entry's tile position, sets its
   object flags and, unless its BLOCK_HIT_FLAG is already set, makes it hittable with the animation
   set its kind (entry[0]: 0x20, 0x40 or other) picks. Remembers `blockId` and the entry. */
u32 obj_init_from_entry(u8 *obj, u8 *entry, u16 blockId, u32 a, u32 b)
{
    s32 x;
    s32 y;
    s32 z;
    s32 fine;
    u32 animSet;
    u16 mask;

    obj_init(obj, a, b);
    ((struct ObjInitS *)obj)->vtable = (struct ObjVtable *)gObjStateClass;

    /* tile position * 16 (z: * 8) plus a signed 5-bit offset packed into bytes 3-5 */
    fine = (entry[3] >> 7) | ((entry[4] << 1) & 0x1E);
    x = ((entry[1] << 4) + ((entry[4] & 8) ? fine | -0x20 : fine)) << 8;
    fine = (entry[4] >> 4) | ((entry[5] << 4) & 0x10);
    y = ((entry[2] << 4) + ((entry[5] & 1) ? fine | -0x20 : fine)) << 8;
    fine = entry[5] >> 1;
    z = (((entry[3] & 0x7F) << 3) + ((entry[5] & 0x20) ? fine | -0x20 : fine)) << 8;
    obj_set_or_add_pos(obj, x, y, z, 0, 0);

    ((struct Bits8 *)(obj + 0x20F))->b1 = 1;
    ((struct Bits8 *)(obj + 0x20F))->b2 = 1;
    ((struct Flags20C *)(obj + 0x20C))->mid = 0;
    ((struct Flags20C *)(obj + 0x20C))->b4 = 0;
    ((struct Flags20C *)(obj + 0x20C))->b5 = 1;
    ((struct Flags20C *)(obj + 0x20C))->b6 = 1;
    ((struct Flags20C *)(obj + 0x20C))->b7 = 1;
    ((struct Bits8 *)(obj + 0x20D))->b0 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b1 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b3 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b4 = 1;
    ((struct Bits8 *)(obj + 0x20D))->b5 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b6 = 0;
    ((struct Word20C *)(obj + 0x20C))->mv = 3;
    ((struct Bits8 *)(obj + 0x20E))->b2 = 1;
    ((struct Bits33C *)(obj + 0x20E))->mid3 = 7;
    ((struct Bits8 *)(obj + 0x20E))->b7 = 0;
    obj[0x222] = 0;
    obj[0x223] = 0;
    ((struct Bits8 *)(obj + 0x216))->b1 = 0;
    obj[0x55] = 0;
    *(u32 *)(obj + 0x228) = 0;
    *(u32 *)(obj + 0x22C) = 0;
    ((struct Bits8 *)(obj + 0x20C))->b0 = 1;
    ((struct Bits8 *)(obj + 0x20F))->b0 = 0;
    ((struct Mode3 *)(obj + 0x214))->mode = 1;

    if (test_flag(GAME_FLAGS, 0, BLOCK_HIT_FLAG(blockId)) == 0)
    {
        /* not hit yet: hittable, animation set (gSpriteSelectAnimTable) by kind */
        animSet = entry[0] == 0x20 ? 2 : entry[0] == 0x40;
        ((struct Bits33C *)&((struct ObjInitS *)obj)->flags)->mid3 = animSet;
        ((struct Bits33C *)&((struct ObjInitS *)obj)->flags)->b0 = 1;
    }
    else
    {
        ((struct Bits33C *)&((struct ObjInitS *)obj)->flags)->b0 = 0;
    }
    ((struct Bits33C *)&((struct ObjInitS *)obj)->flags)->b1 = 0;
    ((struct Bits33C *)&((struct ObjInitS *)obj)->flags)->b2 = 1;
    ((struct Mid4At6 *)&((struct ObjInitS *)obj)->flags)->mid4 = 0;
    ((struct ObjInitS *)obj)->unk23B = 0;
    mask = ((struct ObjInitS *)obj)->mask;
    ((struct ObjInitS *)obj)->mask = mask | 0x1FE;
    ((struct ObjInitS *)obj)->timer340 = 1;
    ((struct ObjInitS *)obj)->timerB342 = 0xFFFF;
    ((struct ObjInitS *)obj)->unk344 = blockId;
    ((struct ObjInitS *)obj)->unk348 = (u32)entry;
    ((struct ObjInitS *)obj)->unk34C = 0;
    return (u32)obj;
}

/*
 * Gives the object a new sprite. Normally (flag bit 2 clear) obj_create_sub builds it from
 * the given tables; unless the object is hidden (flag bit 0) a blank pending sprite then
 * takes over the old sprite's attributes and the two are swapped, so the old one stays
 * pending. With flag bit 2 set the sprite is replaced at once by the one its kind (flag
 * bits 3-5, gSpriteSelectAnimTable) selects, with the shadow attributes if there are any.
 */
void obj_replace_sprite(u8 *obj, u32 sprite_ids, u32 anim_ids, u32 codes)
{
    u8 *flags;
    u8 f;
    u8 replace_now;
    struct SpriteObj *swap;
    struct SpriteObj *spr;
    u32 sel;
    u32 anim;
    u32 width;
    u8 *gfx;

    flags = obj + 0x33C;
    replace_now = (f = *flags) & 4;
    if (replace_now == 0)
    {
        obj_create_sub(obj, sprite_ids, anim_ids, codes);
        if (*(u16 *)(obj + 0x218) == 0x8000)
        {
            return;
        }
        OBJ_PENDING_SPRITE(obj) = create_obj_de4(0, 0x8000, 0, -1, -1, -1, -1);
        obj_load_record(OBJ_PENDING_SPRITE(obj), 3, -1, 0, replace_now);
        if (*flags & 1)
        {
            return;
        }
        OBJ_PENDING_SPRITE(obj)->unkE = ((struct MainObj *)obj)->sprite->unkE;
        ((struct SpriteFlags *)OBJ_PENDING_SPRITE(obj))->lo2 = ((struct SpriteFlags *)((struct MainObj *)obj)->sprite)->lo2;
        ((struct SpriteFlags *)OBJ_PENDING_SPRITE(obj))->b23 = ((struct SpriteFlags *)((struct MainObj *)obj)->sprite)->b23;
        ((struct SpriteFlags *)OBJ_PENDING_SPRITE(obj))->b45 = ((struct SpriteFlags *)((struct MainObj *)obj)->sprite)->b45;
        ((struct SpriteFlags *)OBJ_PENDING_SPRITE(obj))->b67 = ((struct SpriteFlags *)((struct MainObj *)obj)->sprite)->b67;
        swap = OBJ_PENDING_SPRITE(obj);
        OBJ_PENDING_SPRITE(obj) = ((struct MainObj *)obj)->sprite;
        ((struct MainObj *)obj)->sprite = swap;
        return;
    }
    if (f & 1)
    {
        sel = gSpriteSelectAnimTable[(((u32)f << 26) >> 29) * 2];
        anim = gSpriteSelectAnimTable[(((u32)f << 26) >> 29) * 2 + 1];
    }
    else
    {
        sel = 0x8000;
        anim = 3;
    }
    spr = create_obj_de4(2, sel, 0, -1, -1, -1, -1);
    ((struct Lo4Bits *)(obj + 0x216))->b23 = 0;
    ((struct Bits8 *)(obj + 0x216))->b4 = 0;
    obj_load_record(spr, anim, -1, 0, 0);
    /* widen the sprite to its graphics' size */
    gfx = *(u8 **)((u8 *)spr + 0x48);
    if (gfx != NULL)
    {
        width = read_varlen_plus1(*(void **)(gfx + 4)) >> 5;
        if ((s32)width > (s32)((struct SpriteW28 *)spr)->width)
        {
            ((struct SpriteW28 *)spr)->width = width;
        }
    }
    if (((struct ObjShadow298 *)obj)->active)
    {
        ((struct SpriteFlags *)spr)->lo2 = ((struct ObjShadow298 *)obj)->b23;
        ((struct SpriteFlags *)spr)->b23 = ((struct ObjShadow298 *)obj)->b45;
    }
    else
    {
        ((struct SpriteFlags *)spr)->b23 = 1;
        ((struct SpriteFlags *)spr)->lo2 = 1;
    }
    ((struct MainObj *)obj)->sprite = spr;
    ((struct ObjShadow298 *)obj)->active = 0;
    obj_load_part_shape(obj);
}

/* Saves a sprite's attributes into the object's shadow (ObjShadow298): those of the pending
 * sprite when there is one (the object neither hidden nor replacing at once), otherwise
 * obj_copy_sub_fields saves the current sprite's. */
void obj_apply_pending_sprite(u8 *obj)
{
    struct ObjShadow298 *saved;
    u8 **pending;

    saved = (struct ObjShadow298 *)obj;
    if ((obj[0x33C] & (OBJ_FLAG_HIDDEN | OBJ_FLAG_REPLACE_NOW)) == 0)
    {
        pending = (u8 **)&OBJ_PENDING_SPRITE(obj);
        if (*pending != NULL)
        {
            saved->mirror = ((u32)(*pending)[0x11] << 25) >> 31;
            saved->b23 = ((u32)(*pending)[0x1F] << 30) >> 30;
            saved->b45 = ((u32)(*pending)[0x1F] << 28) >> 30;
            saved->b67 = ((u32)(*pending)[0x12] << 29) >> 30;
            saved->b0 = ((u32)(*pending)[0x12] << 28) >> 31;
            saved->b1 = ((u32)(*pending)[0x12] << 27) >> 31;
            saved->unk29B = (*pending)[0x21];
            saved->unk29C = (*pending)[0x23];
            saved->unk29E = *(u16 *)(*pending + 0x2A);
            saved->unk2A0 = *(u16 *)(*pending + 0x2C);
            saved->unk2A2 = *(u16 *)(*pending + 4);
            saved->unk2A4 = *(u16 *)(*pending + 6);
            saved->active = 1;
            return;
        }
    }
    obj_copy_sub_fields(obj);
}

/* A hit block was hit: rolls its drop (drop_roll on its map entry) and keeps what the drop looks
 * like. Drops 1 and 2 given more than once make it a multi-hit block: the first hit allows
 * count - 1 more within count * 17 frames, each further hit uses one up. Any other drop keeps its
 * count. Then marks the block as hit (BLOCK_HIT_FLAG) and returns 1; 0 if nothing was rolled. */
u32 obj_apply_anim_code(u8 *obj, u32 arg)
{
    u8 *mapEntry;
    u16 drop;
    u32 count;
    u32 look;

    mapEntry = *(u8 **)(obj + 0x348);
    /* the rolled drop: its look in bits 0-3, bit 4 = rolled, its count in bits 5-11 */
    drop = drop_roll((mapEntry[7] << 8) | mapEntry[6], mapEntry[0], arg);
    count = (drop >> 5) & 0x7F;
    look = drop & 0xF;
    ((struct ObjAnimHalf33C *)obj)->frame = look;
    if (drop & 0x10)
    {
        if (((struct ObjAnimHalf33C *)obj)->frame != 0 && ((struct ObjAnimHalf33C *)obj)->frame <= 2)
        {
            if (count > 1)
            {
                if (((struct ObjTimers340 *)obj)->timer342 == -1)
                {
                    ((struct ObjTimers340 *)obj)->timer342 = count * 17;
                    ((struct ObjTimers340 *)obj)->timer340 = count - 1;
                }
                else if (--((struct ObjTimers340 *)obj)->timer340 == 0)
                {
                    ((struct ObjTimers340 *)obj)->timer342 = 0;
                }
            }
            else
            {
                ((struct ObjTimers340 *)obj)->timer340 = 0;
                ((struct ObjTimers340 *)obj)->timer342 = 0;
            }
            ((struct ObjAnimWord33C *)obj)->level = 0;
        }
        else
        {
            ((struct ObjAnimWord33C *)obj)->level = count;
            ((struct ObjTimers340 *)obj)->timer340 = 0;
            ((struct ObjTimers340 *)obj)->timer342 = 0;
        }
        set_flag(GAME_FLAGS, 0, BLOCK_HIT_FLAG(*(u16 *)(obj + 0x344)), 1);
        return 1;
    }
    return 0;
}

/* Makes a hit block hittable again (bit 0 of its flags at 0x33C), unless it already is. With a pending
   sprite, that one is swapped in with the current sprite's attributes (and the old one unlinked unless
   bit 1 of 0x20F keeps it); otherwise its kind's animation (flag bits 3-5, gSpriteSelectAnimTable)
   is loaded, the block's hit flag cleared and its animation state reset. */
void obj_activate_sprite(u8 *obj)
{
    u8 *blockFlags;
    u8 armed;
    u16 mask;
    struct ObjInitS *block;
    struct SpriteObj **pending;
    struct SpriteObj **current;
    struct SpriteObj *swap;

    blockFlags = obj + 0x33C;
    if (*blockFlags & 1)
    {
        return;
    }
    armed = *blockFlags | 1;
    *blockFlags = armed;
    pending = (struct SpriteObj **)(obj + 0x34C);
    if (*pending != NULL)
    {
        ((struct Bits33C *)blockFlags)->mid3 = 0;
        current = (struct SpriteObj **)(obj + 0x294);
        unlink_d54(*current);
        obj_queue_update(*pending);
        (*pending)->unkE = (*current)->unkE;
        ((struct SpriteFlags *)*pending)->lo2 = ((struct SpriteFlags *)*current)->lo2;
        ((struct SpriteFlags *)*pending)->b23 = ((struct SpriteFlags *)*current)->b23;
        ((struct SpriteFlags *)*pending)->b45 = ((struct SpriteFlags *)*current)->b45;
        ((struct SpriteFlags *)*pending)->b67 = ((struct SpriteFlags *)*current)->b67;
        swap = *pending;
        *pending = *current;
        *current = swap;
        if (obj[0x20F] & 2)
        {
            return;
        }
        unlink_d54(swap);
    }
    else
    {
        obj_load_record(*(void **)(obj + 0x294), gSpriteSelectAnimTable[(((u32)armed << 26) >> 29) * 2 + 1], 1, 0, 0);
        set_flag(GAME_FLAGS, 0, BLOCK_HIT_FLAG(*(u16 *)(obj + 0x344)), 0);
        block = (struct ObjInitS *)obj;
        ((struct Mid4At6 *)&block->flags)->mid4 = 0;
        block->unk23B = 0;
        mask = block->mask;
        block->mask = mask | 0x1FE;
        block->timer340 = 1;
        block->timerB342 = 0xFFFF;
    }
}

/* A hit block is spent (the reverse of obj_activate_sprite): clears its hittable bit (bit 0 of its
   flags at 0x33C) and its animation offset 0x23B. With a pending sprite, the current one is
   unlinked and the pending one swapped in with the current sprite's attributes; otherwise the
   current sprite loads animation 3. Does nothing when the block isn't hittable. */
void obj_commit_sprite(u8 *obj)
{
    struct SpriteObj **pending;
    struct SpriteObj **current;
    struct SpriteObj *swap;

    if ((obj[0x33C] & 1) == 0)
    {
        return;
    }
    ((struct Bit0 *)(obj + 0x33C))->bit0 = 0;
    ((struct ObjTimer *)obj)->unk23B = 0;
    pending = (struct SpriteObj **)(obj + 0x34C);
    if (*pending != NULL)
    {
        current = (struct SpriteObj **)(obj + 0x294);
        unlink_d54(*current);
        obj_queue_update(*pending);
        (*pending)->unkE = (*current)->unkE;
        ((struct SpriteFlags *)*pending)->lo2 = ((struct SpriteFlags *)*current)->lo2;
        ((struct SpriteFlags *)*pending)->b23 = ((struct SpriteFlags *)*current)->b23;
        ((struct SpriteFlags *)*pending)->b45 = ((struct SpriteFlags *)*current)->b45;
        ((struct SpriteFlags *)*pending)->b67 = ((struct SpriteFlags *)*current)->b67;
        swap = *pending;
        *pending = *current;
        *current = swap;
    }
    else
    {
        /* *pending is NULL here: the last argument is 0 */
        obj_load_record(*(void **)(obj + 0x294), 3, 0, 0, (u32)*pending);
    }
}

extern const u16 gSpriteSelectAnimTable[];
u8 test_flag(u8 *table, u8 *tableB, u16 id);
void obj_commit_sprite(u8 *obj);
extern const u8 gObjAnimOffsetTable[];
extern u8 gObjAnimWaveTable[]; /* not const: its frames are tested as plain bytes */

/*
 * Per-frame animation of a hit block. While it's hittable (bit 0 of its flags at 0x33C), 0x23B
 * cycles through gObjAnimOffsetTable (every 8 frames of `frame`) when idle (step bits 1-8 of 0x33E
 * all set), or follows gObjAnimWaveTable one step a frame after a hit; when the wave reaches 0 the
 * block goes idle again, and a block with bit 2 set whose timer at 0x342 has run out is spent
 * (obj_commit_sprite). The timer counts down every frame. A block that isn't hittable but has
 * bit 2 set reloads its kind's animation (flag bits 3-5, gSpriteSelectAnimTable) until its hit
 * flag is set.
 */
void obj_step_anim(u8 *obj, u32 frame)
{
    struct ObjInitS *block;
    u8 *blockFlags;
    u16 *mask;
    struct AnimStepP *anim;
    s16 *timer;

    block = (struct ObjInitS *)obj;
    blockFlags = obj + 0x33C;
    if (*blockFlags & 1)
    {
        mask = (u16 *)(obj + 0x33E);
        anim = (struct AnimStepP *)mask;
        /* step a running animation wave, or show the offset frame when idle */
        if ((*mask & 0x1FE) == 0x1FE)
        {
            block->unk23B = gObjAnimOffsetTable[(frame >> 3) & 3];
        }
        else if (anim->step >= 0)
        {
            anim->step++;
            block->unk23B = gObjAnimWaveTable[anim->step];
            if (block->unk23B == 0)
            {
                *mask |= 0x1FE;
                if (*blockFlags & 4)
                {
                    if (*(s16 *)(obj + 0x342) == 0)
                    {
                        obj_commit_sprite(obj);
                    }
                }
            }
        }
        timer = (s16 *)(obj + 0x342);
        if (*timer > 0)
        {
            (*timer)--;
        }
    }
    else if (*blockFlags & 4)
    {
        if (test_flag(GAME_FLAGS, 0, BLOCK_HIT_FLAG(*(u16 *)(obj + 0x344))) == 0)
        {
            obj_load_record(*(void **)(obj + 0x294), gSpriteSelectAnimTable[(((u32)*blockFlags << 26) >> 29) * 2 + 1], 0, 0, 0);
        }
    }
}

void obj_clear_mask_if_timer(u8 *obj)
{
    struct ObjTimer *o;
    int f;

    o = (struct ObjTimer *)obj;
    if (o->timer340 != 0)
    {
        o->unk23B = 0;
        ((struct Mid8Bits *)&o->unk33E)->mid = 0;
    }
}

void script_init_obj_refresh_sprite(u8 *obj)
{
    struct MainObj *o;
    int f;

    o = (struct MainObj *)obj;
    f = o->flags20F;
    f = f | 2;
    o->flags20F = f;
    obj_queue_update(o->sprite);
    o->unk23D = 7;
}

/* Constructs an object of class gObjFlagsClass: the base object (obj_init, loading definition def
 * unless it is -1), then clears most of the object flags at 0x20C-0x20F (0x20E bit 2 set) and the
 * 4-bit field unk214_13 (which goes to bits 12-15 of the sprite's unkE). Returns obj. */
u32 obj_init_flags(u8 *obj, u32 def, u32 index)
{
    obj_init(obj, def, index);
    ((struct ChildObj *)obj)->vtable = (struct ObjVtable *)gObjFlagsClass;
    ((struct Bits8 *)(obj + 0x20C))->b5 = 0;
    ((struct Bits8 *)(obj + 0x20C))->b6 = 0;
    ((struct Bits8 *)(obj + 0x20C))->b7 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b0 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b1 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b3 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b4 = 0;
    ((struct Bits8 *)(obj + 0x20D))->b6 = 0;
    ((struct Word20C *)(obj + 0x20C))->mv = 0;
    ((struct Bits8 *)(obj + 0x20E))->b2 = 1;
    ((struct Bits8 *)(obj + 0x20F))->b0 = 0;
    ((struct MainObj *)obj)->unk214_13 = 0;
    return (u32)obj;
}

void script_init_obj_sync_sprite_pos(u8 *obj)
{
    if (((struct MainObj *)obj)->flags20F & 2)
    {
        ((struct MainObj *)obj)->sprite->posX = ((struct MainObj *)obj)->x / 0x100;
        ((struct MainObj *)obj)->sprite->posY = ((struct MainObj *)obj)->y / 0x100;
        ((struct MainObj *)obj)->sprite->unkE = ((struct MainObj *)obj)->unk214_13 << 12;
    }
}

void script_init_obj_setup_sprite(u8 *obj, u32 a, u32 b, u32 d)
{
    struct SpriteObj *sprite;
    int f;

    obj_create_sub(obj, a, b, d);
    sprite = ((struct MainObj *)obj)->sprite;
    ((struct Lo4Bits *)&sprite->flags1F)->b23 = 0;
    ((struct Lo4Bits *)&sprite->flags1F)->lo2 = 0;
}

void stub_nop_080514BE(void)
{
}

void sub_080514C4(void)
{
}

void sub_080514C8(void)
{
}

void sub_080514CC(void)
{
}

void sub_080514D0(void)
{
}

void sub_080514D4(void)
{
}

/* Wander state: starts a move in a random free direction (wander_to_edge), a random one of the two
 * move distances at +0x130 at a random one of the four speeds at +0x100, faces the object along its
 * velocity (+0x242/+0x244; 0xFF when still), sets walk mode 3 and continues in obj_random_enter. */
void obj_random_pick(u32 owner, u8 *obj, u32 *state)
{
    u32 dist;
    u8 *speeds;
    u32 distOfs;
    u8 *dists;
    s16 speed;
    u16 heading;
    u16 *vx;
    u16 *vy;
    u16 vxRaw;

    speeds = obj + 0x100;
    distOfs = rand_below(2, mt19937_next()) << 1;
    dists = obj + 0x130;
    dist = *(u16 *)(dists + distOfs);
    speed = *(s16 *)(speeds + (rand_below(4, mt19937_next()) << 1));
    wander_to_edge(obj, dist, speed);
    vx = (u16 *)(obj + 0x242);
    vy = (u16 *)(obj + 0x244);
    vxRaw = *vx;
    if (vxRaw | *vy)
    {
        /* 256-step heading, rounded */
        heading = ((u16)angle_of_vector16(*(s16 *)(obj + 0x242), *(s16 *)(obj + 0x244)) + 0x10) >> 8;
    }
    else
    {
        heading = 0xFF;
    }
    obj[2] = heading;
    ((struct Mode3 *)(obj + 0x214))->mode = 3;
    *state = (u32)obj_random_enter;
}

void obj_random_enter(u32 owner, u8 *obj, u32 *state)
{
    u8 flag;
    u32 idx;
    u16 *tbl;
    u16 val;

    flag = step_pos_xy_c(obj);
    if ((u8)zone_overlap_code(owner, obj, 0) != 0)
    {
        ((struct Mode3 *)(obj + 0x214))->mode = 4;
    }
    else if (flag == 0)
    {
        idx = rand_below(4, mt19937_next()) * 2;
        tbl = (u16 *)(obj + 0x128);
        val = *(u16 *)((u8 *)tbl + idx);
        *(u16 *)(obj + 0x1D2) = val;
        *(u16 *)(obj + 0x244) = flag;
        *(u16 *)(obj + 0x242) = flag;
        *state = (u32)tick_store;
        *(u32 *)((u8 *)state + 0xC) = (u32)obj_random_pick;
    }
}

void obj_bind_callbacks(u32 arg, u8 *obj)
{
    reset_record_fields(obj);
    ((struct HookObj *)obj)->hook198 = obj_call_hook_1a0;
    ((struct HookObj *)obj)->hook1A0 = obj_random_pick;
}

void obj_bind_callbacks_entry(u32 arg, u8 *obj)
{
    obj_bind_callbacks(arg, obj);
}

void obj_call_hook_1a0(u32 a0, u8 *obj)
{
    u8 *slot = obj + 0x1A0;
    void (*handler)(u32, u8 *, u8 *) = *(void (**)(u32, u8 *, u8 *))slot;

    handler(a0, obj, slot);
}

void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void task_insert(void *node, u8 kind, u32 data);
void set_flag_2_2(void *obj);
void *menu_obj_init(void *obj, u32 a, const void *table, u32 c);
void *map_obj_start(void *obj, u32 a, const void *table, u32 c, s32 x, s32 y, void *d);
void init_obj_09a8(u16 a, u8 b, u8 c, u8 d);
extern u8 gWindowStateClass[];
extern u8 gWindowSubBlockClass[];
extern u8 gWindowCreateBlockTag[];
extern u8 gWindowCreateBlock2Tag[];
extern u8 gWindowCreateSpriteTag[];
extern u16 gWindowSizeTable[];
/* Sets up the map screen task: blanks the display, looks up the map layout and marker
   position for this id, then creates its sub-task, the screen fader and the map object. */
struct MapWindow *window_create(struct MapWindow *window, u8 kind, u32 taskData, u16 id)
{
    u16 *entry;
    u8 *block;
    u8 *fader;
    u8 *map;
    u8 *sys;

    task_insert(window, kind, taskData);
    ((struct WinHead *)window)->class18 = (u32)gWindowStateClass;
    *(u16 *)0x02000000 = 0; /* display control: everything off */
    sys = gSys;
    *(u32 *)(sys + 0x884) = 0; /* no palettes to reload */
    window->id = id;
    window->mapLayout = 0x200;
    window->scrollX = 0;
    window->scrollY = 0;
    window->mapWidth = 0;
    window->mapHeight = 0;
    window->markerX = SCREEN_WIDTH / 2;
    window->markerY = SCREEN_HEIGHT / 2;
    /* gWindowSizeTable: {id, layout, marker x, marker y} records, ended by 0xFFFF */
    for (entry = gWindowSizeTable; entry[0] != 0xFFFF; entry += 4)
    {
        if (entry[0] == id)
        {
            window->mapLayout = entry[1];
            window->markerX = entry[2];
            window->markerY = entry[3];
            break;
        }
    }
    block = heap_alloc2(0x1C, 0, gWindowCreateBlockTag, 1);
    task_insert(block, 8, (u32)gWindowCreateBlockTag);
    ((struct WinHead *)block)->class18 = (u32)gWindowSubBlockClass;
    window->block = block;
    *(struct MapWindow **)(block + 0x14) = window;
    set_flag_2_2(block);
    fader = menu_obj_init(heap_alloc2(0x5C, 0, gWindowCreateBlock2Tag, 0), 8, gWindowCreateBlock2Tag, 8);
    window->fader = fader;
    *(struct MapWindow **)(fader + 0x14) = window;
    set_flag_2_2(fader);
    map = map_obj_start(heap_alloc2(0x59C, 0, gWindowCreateSpriteTag, 0), 8, gWindowCreateSpriteTag,
                        window->mapLayout, window->scrollX, window->scrollY, window->fader);
    window->map = map;
    *(struct MapWindow **)(map + 0x14) = window;
    init_obj_09a8(0x15C0, 0, 1, 0x80);
    return window;
}

void scene_update_sprites(u8 *obj);
/* Task of the map screen: once the map is loaded, creates the edge arrows and the marker,
   scrolls so the marker is in the middle, fades in, scrolls with the d-pad until B is
   pressed, fades out and frees everything. */
void window_state_step(struct MapWindow *window)
{
    u32 *arrows;
    u16 *width;
    u16 *height;
    s32 i;
    u32 dir;
    u8 *sys;
    struct FramePos *points;

    switch (((struct WinHead *)window)->state)
    {
    case 0:
        if (*(s16 *)(window->map + 0x1C) != 0) /* map still loading */
        {
            break;
        }
        i = 0;
        width = &window->mapWidth;
        height = &window->mapHeight;
        arrows = &window->arrowUp;
        points = gWindowSpritePoints;
        for (; i < 4; i++)
        {
            arrows[i] = (u32)create_obj_de4(0, 0x80FE, 1, -1, -1, -1, -1);
            obj_load_record((void *)arrows[i], i, -1, 0, 0);
            ((struct FramePos *)arrows[i])->x = points[i].x;
            ((struct FramePos *)arrows[i])->y = points[i].y;
        }
        window->marker = create_obj_de4(0, 0x80FD, 1, -1, -1, -1, -1);
        obj_load_record(window->marker, 0, -1, 0, 0);
        ((struct SpriteFlags *)window->marker)->lo2 = 2;
        set_flags_2_3(window->block);
        copy_track_limits(window->map, 2, width, height);
        ((struct WinHead *)window)->state = 1;
        break;
    case 1:
        if (gUnk_03000C80[0] != 0)
        {
            break;
        }
        set_flags_2_3(window->fader);
        sprite_update_cd0((u32)window->marker);
        window->scrollX = window->markerX - 0x78;
        window->scrollY = window->markerY - 0x58;
        scroll_clamp(window);
        scene_update_sprites((u8 *)window);
        ((struct WinHead *)window)->state = 2;
        break;
    case 2:
        if (window->marker[0x13] & 3)
        {
            break;
        }
        sys = gSys;
        *(u32 *)(sys + 0x884) = -1; /* reload every palette */
        sub_08115F54(window->fader, 0, 0, 0x10, 0xFFFF, 0xFFFF, 0, 0);
        *(u16 *)0x02000000 = DISPCNT_OBJ_ON | DISPCNT_BG3_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON
                           | DISPCNT_OBJ_1D_MAP;
        ((struct WinHead *)window)->state = 3;
        break;
    case 3:
        if (sub_obj_running(window->fader) == 0) /* faded in */
        {
            ((struct WinHead *)window)->state = 4;
        }
        break;
    case 4:
        if (((struct SysKeys28 *)gSys)->pressed & B_BUTTON)
        {
            sub_08115F54(window->fader, 4, 0, 0x10, 0xFFFF, 0xFFFF, 0, 0);
            set_slot_value(0x61, -1);
            ((struct WinHead *)window)->state = 5;
        }
        else
        {
            /* gWindowScrollTable: an (x, y) step for each combination of d-pad keys */
            dir = (((struct SysKeys28 *)gSys)->held & (DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN)) >> 4;
            window->scrollX += gWindowScrollTable[dir * 2];
            window->scrollY += gWindowScrollTable[dir * 2 + 1];
        }
        scroll_clamp(window);
        scene_update_sprites((u8 *)window);
        break;
    case 5:
        if (sub_obj_running(window->fader) != 0) /* still fading out */
        {
            break;
        }
        set_flags_2_3(((struct WinHead *)window)->block14);
        if (window != NULL)
        {
            ((struct WinHead *)window)->class18 = (u32)gWindowStateClass;
            run_pending_obj_callbacks();
            if (window->map != NULL)
            {
                obj_release_parts(window->map, 3);
            }
            if (window->block != NULL)
            {
                *(u32 *)(window->block + 0x18) = (u32)gWindowSubBlockClass;
                task_remove(window->block, 3);
            }
            if (window->fader != NULL)
            {
                menu_obj_free(window->fader, 3);
            }
            task_remove(window, 3);
        }
        break;
    }
    if ((u8)(((struct WinHead *)window)->state - 3) <= 2)
    {
        update_obj_list();
    }
}

void scene_update_sprites(u8 *obj)
{
    struct MapWindow *w;

    /* marker follows the scroll; each edge arrow shows only while there is map beyond it */
    w = (struct MapWindow *)obj;
    ((struct FramePos *)w->marker)->x = w->markerX - w->scrollX;
    ((struct FramePos *)w->marker)->y = w->markerY - w->scrollY;
    if (w->scrollY == 0)
    {
        obj_release_slots(w->arrowUp);
    }
    else
    {
        sprite_update_cd0(w->arrowUp);
    }
    if (w->scrollX == w->mapWidth - SCREEN_WIDTH)
    {
        obj_release_slots(w->arrowRight);
    }
    else
    {
        sprite_update_cd0(w->arrowRight);
    }
    if (w->scrollY == w->mapHeight - SCREEN_HEIGHT)
    {
        obj_release_slots(w->arrowDown);
    }
    else
    {
        sprite_update_cd0(w->arrowDown);
    }
    if (w->scrollX == 0)
    {
        obj_release_slots(w->arrowLeft);
    }
    else
    {
        sprite_update_cd0(w->arrowLeft);
    }
}

void obj_emit_oam(void)
{
    u8 *base;
    u32 *oam_slot;
    struct ListHeadX *list;
    u32 old;

    base = (u8 *)&gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)(((struct ListHeadX *)gUnk_0203FFB8)->head3C,
        ((struct ListHeadX *)gUnk_0203FFB8)->head3C + 0xA14);
    list = (struct ListHeadX *)gUnk_0203FFB8;
    old = ((u32 (*)(u32, u32))gUnk_03000D4C)(list->head3C, old);
    *oam_slot = old;
}
