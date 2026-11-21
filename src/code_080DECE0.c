#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
};

struct PoseFlagBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct ObjFlags79
{
    u8 filler0[0x79];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct ActorData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 partner; /* -1: first of the pair (the second follows it), -2: second, 0: not there */
    u8 filler0EE[0x113 - 0xEE];
    u8 heldItem; /* bit 7: holds an item, bits 0-6: its gItemTable index (0xFF: none) */
    u8 filler114[0x130 - 0x114];
}; /* the pair's two ActorData sit next to each other */

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x28 - 0x1C];
    struct ActorData *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    s32 (*handler58)(struct ActorObj *);
    s32 (*handler5C)(struct ActorObj *, s32, s32);
    u8 filler60[0x68 - 0x60];
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x78 - 0x75];
    u8 kind78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 link;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 codeId;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

/* An effect script run by fx_script_step: a list of u16 commands, each followed by its
 * arguments and then the frames to wait (or one of the FX_WAIT_ codes) before the next. */
struct FxSpawnList
{
    s32 pad0;
    s32 active;
    struct ActorObj *owner;
    u16 *cursor;
    s16 delay;
    s16 baseX;
    s16 baseY;
    s16 baseH;
};

/* fx->delay values below 0: what to wait for before the next command */
enum FxWait
{
    FX_WAIT_END = -1,          /* the script is over */
    FX_WAIT_OWNER_IDLE = -2,   /* until the owner has no state left to run */
    FX_WAIT_NOT_BUSY = -3,     /* until is_state_busy() is false */
    FX_WAIT_OWNER_LANDED = -4, /* until the owner is at its target height */
};

/* Effect script commands; any other value spawns that effect at base + (x, y, h). */
enum FxOp
{
    FX_OP_OWNER_DONE = 0xFFFF,           /* stop the owner's data task */
    FX_OP_OWNER_DETACH = 0xFFFE,         /* detach the owner's pose */
    FX_OP_TIMED_SPRITE = 0xFFFD,         /* tile, value, frames */
    FX_OP_DISPLAY_FINISH = 0xFFFC,
    FX_OP_SPECIAL_MODE_ENTER = 0xFFFB,   /* mode */
    FX_OP_IRIS_ROWS = 0xFFFA,            /* x, y, h, size */
    FX_OP_SPECIAL_MODE_LEAVE = 0xFFF9,
    FX_OP_POINTS_LOAD = 0xFFF8,          /* index into gOffsetRows */
    FX_OP_POINTS_SCALE = 0xFFF7,         /* x, y, z */
    FX_OP_POINTS_ROTATE_SHIFTED = 0xFFF6, /* angle */
    FX_OP_POINTS_ROTATE_Y = 0xFFF5,      /* angle */
    FX_OP_POINTS_ROTATE = 0xFFF4,        /* angle */
    FX_OP_POINTS_OFFSET = 0xFFF3,        /* x, y, z */
    FX_OP_OUTLINE_ROWS = 0xFFF2,         /* x, y, h */
    FX_OP_FRAME_PAIR = 0xFFF1,           /* frame */
    FX_OP_FADE_CLEAR = 0xFFF0,
    FX_OP_SCROLL = 0xFFEF,               /* slot, value */
    FX_OP_STRIP_UPLOAD_TASK = 0xFFEE,    /* 3 words of strip */
    FX_OP_STRIP_UPLOAD_OWNER = 0xFFED,   /* 3 words of strip */
    FX_OP_SOUND = 0xFFEC,                /* sound id */
    FX_OP_SLOTS_REFRESH_TASK = 0xFFEB,   /* mask */
    FX_OP_SLOTS_REFRESH = 0xFFEA,        /* mask */
    FX_OP_OWNER_ACTION = 0xFFE9,         /* arg, action */
};

struct Particle
{
    u8 filler0[0x4];
    s32 posX;
    s32 posH;
    s32 posY;
    s16 x;
    s16 dh;
    s16 y;
    s16 timer;
    s16 dx;
};

struct SysFlagArea
{
    u8 filler0[0x888];
    u8 sysFlags; /* bits 1-2: the serial link mode, 0 when not linked */
};

/* One frame of a picture-archive animation (start_obj_frame). */
struct FrameRec
{
    u8 filler0[2];
    s16 index; /* gTileHalfTable entry of its tiles; its palette follows */
    u16 delay; /* frames it stays on screen */
};

struct FrameObj
{
    u8 filler0[4];
    u32 callback;
    s32 value8;
    struct FrameRec *rec;
    u16 delay;
    u8 filler12[6];
    u16 zero18;
};

/* Plays three keyframe tracks from gDeltaTable on its owner (see fx_tick_move_records). Each track is an
 * index into gDeltaTable, 0 once the track is over; script holds their first indices minus 1. */
struct FxMove
{
    u8 filler0[0x4];
    s32 active;
    struct ActorObj *owner;
    u16 *script;
    u16 timer;
    s16 x; /* offset track: (dx, dy, last frame) per key */
    s16 y; /* size track: (w, h, last frame) per key */
    s16 h; /* action track: (frame, action) per key */
};

struct MoveNode
{
    struct MoveNode *next;
    u8 filler4[0x4];
    u8 record[0x8];
    struct ActorPose *tag;
};

struct FxSpawnFlags1B
{
    u8 filler0[0x1B];
    u8 lo7 : 7;
    u8 bit7 : 1;
};

struct SpawnedBits11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct ObjFlags76
{
    u8 filler0[0x76];
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

struct DataFlags111
{
    u8 filler0[0x111];
    u8 bit0 : 1;
    u8 hi7 : 7;
};

struct WorldFlagsC
{
    u8 filler0[0xC];
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 hi6 : 6;
};

struct FxScriptTask
{
    u8 filler0[0xC];
    s32 valueC;
    u16 delay10;
};

void task_list_remove(void *node);
void set_cur_flags_pending(u32 flags);
void obj_detach_active(void *obj);
void draw_sprite_small(u32 kind, u32 x, u32 y);
struct FxScriptTask *task_create_list_40(void (*fn)(void));
void count_down_then_stop(void);
void display_finish(u32 a, s32 b);
void enter_special_mode(u32 mode);
void build_iris_rows(s32 x, s32 y, s32 z, u32 radius);
void load_records_from_stream(s16 *stream);
extern s16 gOffsetRows[];
void scale_point_list(s32 a, s32 b, s32 c);
void rotate_point_list_shifted(s32 angle);
void rotate_point_list_y(s32 angle);
void rotate_point_list(s32 angle);
void add_offset_to_records(s32 dx, s32 dy, s32 dz);
void rasterize_outline_rows(s32 x, s32 y, s32 z);
void leave_special_mode(void);
void show_frame_pair(u32 index);
void frame_copy_fade_clear(void);
void set_scroll_slot(s32 x, s32 y);
void task_push_list_50(void (*fn)(void), void *obj);
void upload_obj_strip(void);
void upload_strip_from_rec(void *obj, void *rec);
struct FxScriptTask *task_create_list_4c(void (*fn)(void), void *obj);
void alloc_slot_refresh_for_node(void);
void refresh_slots_for_mask(void *obj, s32 mask);

extern u8 gSys[];
extern s16 gDeltaTable[];
extern const u8 gPairFxTag[];
void move_record_by_table(void *obj, u32 step, u32 shape);
struct FxSpawnList *task_create_list_44(void *next);
void *spawn_obj_by_id(u32 id, u32 a, u32 b, u32 c);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
extern const u16 gTileHalfTable[];
extern const u8 gUnk_089F46B0[];
extern u8 gUnk_02000460[];
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void effect_obj_frame(void);

void obj_release(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void change_item_count(u32 id, s32 amount);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void on_arrive(struct ActorObj *obj);
extern u8 *gWorld;
extern u8 gItemTable[];
void set_slot_value(u32 id, s32 value);
void sprite_load_tables(u32 index);
void ensure_code_entry(u32 id);
u32 is_state_busy(void);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void obj_spawn_linked_child_jump(struct ActorObj *obj);
void obj_begin_move_pair(struct ActorObj *obj);
void obj_state_spawn_child(struct ActorObj *obj);
void obj_fall_settle_step(struct ActorObj *obj);
void obj_wait_idle_then_action12(struct ActorObj *obj);
void obj_pose_active_arrive_idle(struct ActorObj *obj);
void obj_wait_not_busy_action10(struct ActorObj *obj);
void obj_action10_pose_mode(struct ActorObj *obj);
void obj_pair_midpoint_move_then_f3b4(struct ActorObj *obj);
void obj_timer_pair_move_then_e640(struct ActorObj *obj);
void obj_timer_move_to_owner_then_f358(struct ActorObj *obj);
void obj_wait_not_busy_then_begin_pair(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorData *data, u32 kind, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_list_push_default(struct ActorObj *obj);
void obj_begin_jump_to_data(struct ActorObj *obj);
s32 obj_unlink_release_code(struct ActorObj *obj);
void obj_pose_active_arrive(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_state_idle_if_unlinked(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_wait_link_then_timer(struct ActorObj *obj);
u32 part_lookup_clear_11a(struct ActorObj *obj, u32 a, u32 b);
u32 part_unlink_clear_11a(struct ActorObj *obj, u32 a, u32 b);
void start_motion(struct ActorObj *obj);
void obj_load_code_entry_then_wait(struct ActorObj *obj);
void obj_select_code_entry_world(struct ActorObj *obj);
void obj_wait_not_busy_then_e321(struct ActorObj *obj);
void obj_finish_now(struct ActorObj *obj);
void obj_timer_begin_move_to_data(struct ActorObj *obj);
void obj_timer_release_move_pair(struct ActorObj *obj);
void obj_pose_active_action11(struct ActorObj *obj);
void obj_pose_active_action11_notify(struct ActorObj *obj);
static inline void copy_child_flags(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

static inline void obj_copy_target(struct ActorObj *obj, struct ActorData *src)
{
    obj->targetX = src->targetX;
    obj->targetY = src->targetY;
}

static inline void set_pair_target(struct ActorObj *obj)
{
    struct ActorData *owner_data = obj->owner->data;
    struct ActorData *a;
    struct ActorData *b;

    if (owner_data->partner == -1)
    {
        a = owner_data;
        b = (struct ActorData *)((u8 *)owner_data + 0x130);
    }
    else
    {
        a = (struct ActorData *)((u8 *)owner_data - 0x130);
        b = owner_data;
    }
    if (a->partner != 0)
    {
        if (b->partner != 0)
        {
            obj->targetX = (a->targetX + b->targetX) / 2;
            obj->targetY = (a->targetY + b->targetY) / 2;
        }
        else
        {
            obj->targetX = a->targetX;
            obj->targetY = a->targetY;
        }
    }
    else
    {
        obj->targetX = b->targetX;
        obj->targetY = b->targetY;
    }
    obj->targetH = obj->owner->data->targetH;
    obj->targetX += 0x4000;
    ((struct ObjFlags79 *)obj)->bit5 = 1;
}

/* Waits for the current sprite action to end. Then, if the actor holds an item, spawns the item's
 * object (kind codeId, facing the same way as the actor) 20 px behind and 20 px above its base,
 * which jumps to the owner's target (obj_begin_jump_to_data) and gives the item back if it is
 * released early (obj_unlink_release_code). The actor itself plays action 12 and goes on to
 * obj_pose_active_arrive. */
void obj_state_spawn_child(struct ActorObj *obj)
{
    struct ActorPose *pose = obj->pose;

    if (pose->flags & 8)
    {
        struct ActorData *data = obj->data;

        if (data->heldItem & 0x80)
        {
            struct ActorObj *item;

            item = obj_spawn(data, *(u16 *)&obj->codeId, obj->baseX / 0x100 - 0x14,
                             obj->baseY / 0x100 - 1, obj->baseH / 0x100 + 0x14,
                             ((u32)pose->byte11 << 25) >> 31, 1); /* bit 6: flipped */
            obj->child = item;
            item->kind78 = item->kind78 & 0x1F;
            copy_child_flags(obj, item);
            item->state = obj_begin_jump_to_data;
            item->handler68 = on_arrive;
            item->childValue = obj->childValue;
            item->owner = obj->owner;
            obj_list_push_default(item);
            item->handler58 = obj_unlink_release_code;
            item->param9C = obj->param9C;
            item->paramA0 = obj->paramA0;
            item->paramA4 = obj->paramA4;
            item->codeId = obj->codeId;
            item->byte74 = 0xFF;
            sprite_set_action(item, 0, 0, 0);
        }
        obj->childValue = 0;
        sprite_set_action(obj, 0xC, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_arrive;
    }
}

void obj_fall_settle_step(struct ActorObj *obj)
{
    s32 t;
    s32 h;

    t = obj->dx * 253;
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->dx = t >> 8;

    t = obj->dy * 253;
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->dy = t >> 8;

    obj->dh = (u16)obj->dh + obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    h = obj->baseH + obj->dh;
    obj->posH = h;
    if (obj->dh <= 0)
    {
        if (h <= obj->ground)
        {
            obj->posH = obj->ground;
            obj_release(obj);
        }
    }
}


/* Sends the object 64 px right of the owner pair's targets (their midpoint when both are there,
 * else the one that is) at the owner's target height, moving at speed paramA0; plays sound 0x11C
 * and action 0xD, then waits in obj_wait_link_then_timer. */
void obj_begin_move_pair(struct ActorObj *obj)
{
    struct ActorData *owner_data = obj->owner->data;
    struct ActorData *first;
    struct ActorData *second;
    register s32 *pTargetX asm("r3");
    register s32 *pLast asm("r2");
    register s32 *pTargetH asm("r1");

    if (owner_data->partner == -1)
    {
        first = owner_data;
        second = owner_data + 1;
    }
    else
    {
        first = owner_data - 1;
        second = owner_data;
    }
    if (first->partner != 0)
    {
        if (second->partner != 0)
        {
            pTargetX = &obj->targetX;
            *pTargetX = (first->targetX + second->targetX) / 2;
            pLast = &obj->targetY;
            *pLast = (first->targetY + second->targetY) / 2;
        }
        else
        {
            pLast = &obj->targetX;
            *pLast = first->targetX;
            obj->targetY = first->targetY;
            pTargetX = pLast;
        }
    }
    else
    {
        pLast = &obj->targetX;
        *pLast = second->targetX;
        obj->targetY = second->targetY;
        pTargetX = pLast;
    }
    pTargetH = &obj->targetH;
    *pTargetH = obj->owner->data->targetH;
    *pTargetX += 0x4000;
    /* never true: only keeps the pointers in r1-r3 */
    if (pTargetH == pLast)
        pTargetH = 0;
    ((struct ObjFlags79 *)obj)->bit5 = 1;
    obj->motionSpeed = obj->paramA0 << 8;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x11C, -1);
    sprite_set_action(obj, 0xD, 0, 0);
    obj->state = obj_wait_link_then_timer;
}

/* Counts the timer down; once it has run out: starts sound 0x11A, joins the object list with
 * contact handlers that stop that sound again (part_unlink_clear_11a / part_lookup_clear_11a),
 * moves at speed 0x200 to the owner's data target with action 0x12, and leaves 0x30 frames to
 * obj_timer_release_move_pair. */
void obj_timer_begin_move_to_data(struct ActorObj *obj)
{
    u32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s32)(timer << 16) < 0) /* (s16)timer < 0 */
    {
        set_slot_value(0x11A, -1);
        obj_list_push_default(obj);
        obj->handler58 = (s32 (*)(struct ActorObj *))part_unlink_clear_11a;
        obj->handler5C = (s32 (*)(struct ActorObj *, s32, s32))part_lookup_clear_11a;
        obj->targetX = obj->owner->data->targetX;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->owner->data->targetH;
        ((struct ObjFlags79 *)obj)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 0x12, 0, 0);
        obj->timer = 0x30;
        obj->state = obj_timer_release_move_pair;
    }
}

/* Once nothing is linked to it and its timer has run out: stops sound 0x11A, leaves the object
 * list and moves (speed 0x200, action 13) to 0x40 px past the pair's midpoint (or the one of the
 * pair still there) at the owner's target height, then idles until unlinked. */
void obj_timer_release_move_pair(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        u32 timer = obj->timer - 1;

        obj->timer = timer;
        if ((s32)(timer << 16) < 0)
        {
            struct ActorData *data;
            struct ActorData *first;
            struct ActorData *second;
            register s32 *targetX asm("r3");
            register s32 *lastSet asm("r2");
            register s32 *targetH asm("r1");

            sound_stop_song(0x11A);
            obj_list_remove(obj);
            data = obj->owner->data;
            if (data->partner == -1)
            {
                first = data;
                second = data + 1;
            }
            else
            {
                first = data - 1;
                second = data;
            }
            if (first->partner != 0)
            {
                if (second->partner != 0)
                {
                    targetX = &obj->targetX;
                    *targetX = (first->targetX + second->targetX) / 2;
                    lastSet = &obj->targetY;
                    *lastSet = (first->targetY + second->targetY) / 2;
                }
                else
                {
                    lastSet = &obj->targetX;
                    *lastSet = first->targetX;
                    obj->targetY = first->targetY;
                    targetX = lastSet;
                }
            }
            else
            {
                lastSet = &obj->targetX;
                *lastSet = second->targetX;
                obj->targetY = second->targetY;
                targetX = lastSet;
            }
            targetH = &obj->targetH;
            *targetH = obj->owner->data->targetH;
            *targetX += 0x4000;
            if (targetH == lastSet)
                targetH = 0;
            ((struct ObjFlags79 *)obj)->bit5 = 1;
            obj->motionSpeed = 0x200;
            obj->motionStart = 0;
            start_motion(obj);
            sprite_set_action(obj, 0xD, 0, 0);
            obj->state = obj_state_idle_if_unlinked;
        }
    }
}

void obj_state_idle_if_unlinked(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        obj->state = wait_for_index;
    }
}

u32 part_lookup_clear_11a(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x11A);
    return lookup_part(obj, a, b);
}

u32 part_unlink_clear_11a(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x11A);
    return partner_link_update(obj, a, b);
}

void obj_wait_link_then_timer(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        sound_stop_song(0x11C);
        sprite_set_action(obj, obj->owner->data->partner != -1 ? 0x11 : 0xF, 0, 0);
        obj->timer = obj->param9C;
        obj->state = obj_timer_begin_move_to_data;
    }
}

s32 obj_unlink_release_code(struct ActorObj *obj)
{
    obj->state = obj_finish_now;
    if (obj->data->heldItem & 0x80)
    {
        change_item_count(obj->data->heldItem & 0x7F, 1);
        obj->data->heldItem = 0;
    }
    return 1;
}

void obj_begin_jump_to_data(struct ActorObj *obj)
{
    s32 *ground = &obj->ground;
    struct ActorData *data = obj->owner->data;
    s32 targetH = data->targetH;

    *ground = targetH;
    obj->dhStep = jump_velocity(&obj->dx, (s16 *)((u8 *)ground + 0x18), &obj->dh,
                                data->targetX - obj->baseX, data->targetY - obj->baseY,
                                targetH - obj->baseH, 0x2800, 0x7CC, 0xFD, 0);
    obj->state = obj_fall_settle_step;
}

void obj_pose_active_arrive(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}

void obj_lower_height_512(struct ActorObj *obj)
{
    if (obj->posH > 0x200)
    {
        obj->posH = obj->posH - 0x200;
    }
    else
    {
        obj->posH = 0;
    }
}

void obj_arrive_toggle_pose(struct ActorObj *obj)
{
    struct ActorPose *pose;
    s32 flags;
    s32 bit;
    s32 cleared;

    if (obj->link == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        pose = obj->pose;
        flags = pose->byte11;
        bit = ((u32)flags << 25) >> 31;
        bit = bit ^ 1;
        bit = bit << 6;
        cleared = -0x41 & flags;
        cleared = cleared | bit;
        pose->byte11 = cleared;
        on_arrive(obj);
    }
}

void obj_action8_slot126_timer(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        set_slot_value(0x126, -1);
        obj->timer = 0x30;
        obj->state = obj_timer_pair_move_then_e640;
    }
}

u32 link_slot_lookup_clear_11c(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x11C);
    return lookup_link_slot(obj, a, b);
}

void obj_partner_action_timer(struct ActorObj *obj)
{
    if (obj->link == 0)
    {
        sprite_set_action(obj, obj->owner->data->partner != -1 ? 6 : 4, 0, 0);
        sound_stop_song(0x11C);
        obj->timer = obj->param9C;
        obj->state = obj_timer_move_to_owner_then_f358;
    }
}

s32 sprite_state_chain_e(struct ActorObj *obj)
{
    obj->state = obj_wait_not_busy_then_begin_pair;
    return 1;
}

s32 obj_enter_load_code_entry(struct ActorObj *obj)
{
    obj->state = obj_load_code_entry_then_wait;
    return 1;
}

s32 obj_enter_select_code_entry_world(struct ActorObj *obj)
{
    obj->state = obj_select_code_entry_world;
    return 1;
}

s32 obj_enter_wait_not_busy_then_e321(struct ActorObj *obj)
{
    obj->state = obj_wait_not_busy_then_e321;
    return 1;
}

s32 obj_random_next_state(struct ActorObj *obj)
{
    u32 pick = rand_below(4, mt19937_next());

    switch (pick)
    {
    case 0:
        obj->state = obj_wait_not_busy_then_e321;
        break;
    case 1:
        obj->state = obj_select_code_entry_world;
        break;
    case 2:
        obj->state = obj_load_code_entry_then_wait;
        break;
    case 3:
        obj->state = obj_wait_not_busy_then_begin_pair;
        break;
    }
    return 1;
}

void obj_wait_not_busy_then_begin_pair(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_begin_move_pair;
    }
}

void obj_finish_now(struct ActorObj *obj)
{
    obj_release(obj);
}

void obj_load_code_entry_then_wait(struct ActorObj *obj)
{
    u8 *codeByte = &obj->data->heldItem;
    u32 code = *codeByte;
    void (*next)(struct ActorObj *);

    if ((code & 0x80) == 0)
    {
        next = 0;
    }
    else if (code == 0xFF)
    {
        next = 0;
    }
    else
    {
        u32 mask = 0x7F;
        u32 *codeIdSlot = &obj->codeId;
        u8 *table = gItemTable;
        u32 id = (table[(mask & *(volatile u8 *)codeByte) * 16 + 2] & 7) + 0x4188;

        *codeIdSlot = id;
        ensure_code_entry(id);
        next = obj_wait_not_busy_action10;
    }
    obj->state = next;
}

void obj_select_code_entry_world(struct ActorObj *obj)
{
    u32 index = obj->data->heldItem;
    void (*next)(struct ActorObj *);

    if ((index & 0x80) == 0)
    {
        next = 0;
    }
    else if (index == 0xFF)
    {
        next = 0;
    }
    else
    {
        index = index & 0x7F;
        gWorld[0x349] = index;
        *(u8 **)(gWorld + 0x2DC) = gItemTable + (index << 4);
        sprite_load_tables(index);
        obj->motionSpeed = 0x100;
        next = obj_action10_pose_mode;
    }
    obj->state = next;
}

void obj_wait_not_busy_then_e321(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_pair_midpoint_move_then_f3b4;
    }
}

void obj_wait_not_busy_action10(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_action11;
    }
}

void obj_action10_pose_mode(struct ActorObj *obj)
{
    sprite_set_action(obj, 0xA, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
    obj->state = obj_pose_active_action11_notify;
}

void obj_pose_active_action11(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_state_spawn_child;
    }
}

void obj_pose_active_action11_notify(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        set_slot_value(0x128, -1);
        obj->state = obj_wait_idle_then_action12;
    }
}

void obj_wait_idle_then_action12(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        u32 busy = is_state_busy();

        if (busy == 0)
        {
            obj_spawn_linked_child_jump(obj);
            obj->childValue = busy;
            sprite_set_action(obj, 0xC, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
            obj->state = obj_pose_active_arrive_idle;
        }
    }
}

void obj_pose_active_arrive_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}

NONMATCH("asm/code_080DECE0/fx_obj_spawn.s", void fx_obj_spawn(u32 a, u32 b, u32 c, u32 d, void *next, u32 key, u32 recArg, u32 flag,
                  u32 count, u32 bit7Arg, u32 bit6Arg, u32 *src, u32 words))
{
    struct FxSpawnList *task;
    struct ActorObj *spawned;
    u32 keyId = key;
    u32 total;
    u32 *out;
    s32 cosv;
    s32 sinv;

    task = task_create_list_44(next);
    task->baseX = a;
    task->baseY = b;
    task->baseH = c;
    task->delay = (u16)d;
    ((struct FxSpawnFlags1B *)task)->bit7 = bit7Arg;
    spawned = spawn_obj_by_id((u16)keyId, recArg, 2, 1);
    ((struct SpawnedBits11 *)spawned)->bit6 = bit6Arg;
    task->owner = spawned;
    total = count;
    out = heap_alloc_raw(1, (count * 3 + words) << 2, gPairFxTag);
    task->cursor = (u16 *)out;
    words = words - 1;
    if (words != -1)
    {
        do
        {
            *out++ = *src++;
            words--;
        } while (words != -1);
    }
    if (flag == 0)
    {
        count = count - 1;
        if (count != -1)
        {
            s16 *cosTab = gCosineTable;

            do
            {
                cosv = cosTab[MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(count << 8, total) & 0xFF];
                *out++ = cosv / 0x40;
                *out++ = 0;
                sinv = gSineTable[MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(count << 8, total) & 0xFF];
                *out++ = sinv / 0x40;
                count = count - 1;
            } while (count != -1);
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080DECE0/effect_spawn_at.s", void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner))

/*
 * Runs an effect script for one frame: counts down the current wait (or checks its FX_WAIT_
 * condition), then executes commands until one is followed by a wait. See enum FxOp.
 */
void fx_script_step(struct FxSpawnList *fx)
{
    u16 *args;
    u32 op;

    for (;;)
    {
        if (fx->delay > 0)
        {
            fx->delay--;
            return;
        }
        switch (fx->delay)
        {
        case FX_WAIT_END:
            fx->active = 0;
            return;
        case FX_WAIT_OWNER_IDLE:
            if (fx->owner != 0 && fx->owner->state != 0 && ((struct ObjFlags76 *)fx->owner)->mode != 3)
                return;
            break;
        case FX_WAIT_NOT_BUSY:
            if ((u8)is_state_busy())
                return;
            break;
        case FX_WAIT_OWNER_LANDED:
            if (fx->owner != 0 && ((struct ObjFlags76 *)fx->owner)->mode != 3 && fx->owner->posH != fx->owner->data->targetH)
                return;
            break;
        }
        args = fx->cursor;
        op = *args++;
        switch (op)
        {
        case FX_OP_OWNER_DONE:
            if (fx->owner != 0)
            {
                struct ActorData *data = fx->owner->data;

                if (data != 0)
                {
                    ((struct DataFlags111 *)data)->bit0 = 0;
                    task_list_remove(data);
                    set_cur_flags_pending(0x2000);
                }
            }
            break;
        case FX_OP_OWNER_DETACH:
            if (fx->owner != 0 && fx->owner->data != 0)
                obj_detach_active(fx->owner->pose);
            break;
        case FX_OP_TIMED_SPRITE:
            /* only one at a time: gWorld[0xC] bit 1 is cleared by FX_OP_DISPLAY_FINISH */
            if (!(gWorld[0xC] & 2))
            {
                draw_sprite_small(4, args[0], (s16)args[1]);
                task_create_list_40(count_down_then_stop)->delay10 = args[2];
                gWorld[0xC] |= 2;
            }
            args += 3;
            break;
        case FX_OP_DISPLAY_FINISH:
            display_finish(1, -1);
            ((struct WorldFlagsC *)gWorld)->bit1 = 0;
            break;
        case FX_OP_SPECIAL_MODE_ENTER:
            enter_special_mode(args[0]);
            args++;
            break;
        case FX_OP_IRIS_ROWS:
            build_iris_rows(fx->baseX + (s16)args[0], fx->baseY + (s16)args[1], fx->baseH + (s16)args[2], (s16)args[3]);
            args += 4;
            break;
        case FX_OP_POINTS_LOAD:
            load_records_from_stream(&gOffsetRows[args[0]]);
            args++;
            break;
        case FX_OP_POINTS_SCALE:
            scale_point_list((s16)args[0], (s16)args[1], (s16)args[2]);
            args += 3;
            break;
        case FX_OP_POINTS_ROTATE_SHIFTED:
            rotate_point_list_shifted((s16)args[0]);
            args++;
            break;
        case FX_OP_POINTS_ROTATE_Y:
            rotate_point_list_y((s16)args[0]);
            args++;
            break;
        case FX_OP_POINTS_ROTATE:
            rotate_point_list((s16)args[0]);
            args++;
            break;
        case FX_OP_POINTS_OFFSET:
            add_offset_to_records((s16)args[0], (s16)args[1], (s16)args[2]);
            args += 3;
            break;
        case FX_OP_OUTLINE_ROWS:
            rasterize_outline_rows(fx->baseX + (s16)args[0], fx->baseY + (s16)args[1], fx->baseH + (s16)args[2]);
            args += 3;
            break;
        case FX_OP_SPECIAL_MODE_LEAVE:
            leave_special_mode();
            break;
        case FX_OP_FRAME_PAIR:
            show_frame_pair((s16)args[0]);
            args++;
            break;
        case FX_OP_FADE_CLEAR:
            frame_copy_fade_clear();
            break;
        case FX_OP_SCROLL:
            set_scroll_slot((s16)args[0], (s16)args[1]);
            args += 2;
            break;
        case FX_OP_STRIP_UPLOAD_TASK:
            task_push_list_50(upload_obj_strip, args);
            args += 3;
            break;
        case FX_OP_STRIP_UPLOAD_OWNER:
            if (fx->owner != 0)
                upload_strip_from_rec(fx->owner->pose, args);
            args += 3;
            break;
        case FX_OP_SOUND:
            set_slot_value((s16)args[0], -1);
            args++;
            break;
        case FX_OP_SLOTS_REFRESH_TASK:
        {
            struct ActorObj *owner = fx->owner;

            task_create_list_4c(alloc_slot_refresh_for_node, owner->pose)->valueC = (s16)args[0];
            args++;
            break;
        }
        case FX_OP_SLOTS_REFRESH:
            refresh_slots_for_mask(fx->owner->pose, (s16)args[0]);
            args++;
            break;
        case FX_OP_OWNER_ACTION:
            sprite_set_action(fx->owner, (s16)args[1], (s16)args[0], 0);
            args += 2;
            break;
        default:
            /* any other value is an effect to spawn */
            effect_spawn_at(op, fx->baseX + (s16)args[0], fx->baseY + (s16)args[1], fx->baseH + (s16)args[2], fx->owner);
            args += 3;
            break;
        }
        fx->delay = *args++;
        fx->cursor = args;
    }
}

/* Runs an effect spawn list for one frame. Each record is 5 u16s: effect type, x, y, h, frames to
 * wait before the next record. Records whose wait has run out spawn their effect at the list's
 * base point plus (x, y, h) for the owner, with x mirrored when the owner's pose is flipped
 * (byte11 bit 6); type 0xFFFD is spawned at (x, y, h) as given, without an owner. A negative wait
 * ends the list (active = 0). */
void fx_spawn_list_step(struct FxSpawnList *fx)
{
    while (fx->delay == 0)
    {
        u16 *rec = fx->cursor;
        u32 type = rec[0];

        if (type == 0xFFFD)
        {
            effect_spawn_at(type, (s16)rec[1], (s16)rec[2], (s16)rec[3], 0);
        }
        else
        {
            s32 x = fx->baseX;
            struct ActorObj *owner = fx->owner;

            if (owner != 0 && (owner->pose->byte11 & 0x40))
            {
                x -= (s16)rec[1];
            }
            else
            {
                x += (s16)rec[1];
            }
            effect_spawn_at(type, x, fx->baseY + (s16)rec[2], fx->baseH + (s16)rec[3], owner);
        }
        fx->delay = rec[4];
        fx->cursor = rec + 5;
    }
    if (fx->delay < 0)
    {
        fx->active = 0;
    }
    else
    {
        fx->delay--;
    }
}

/* Effect task, once per frame: moves and resizes the owner's sprite record (and those of its flagged
 * children and its data's other records) by the current offset and size keys from gDeltaTable, plays
 * the action track's sprite actions on their frames, steps each track on to its next key once its
 * last frame is past, and stops when the offset and the other two tracks are all over. */
void fx_tick_move_records(struct FxMove *fx)
{
    struct ActorObj *owner;
    struct ActorObj *child;
    struct MoveNode *listHead;
    struct MoveNode *first;
    struct MoveNode *cur;
    s32 frame;
    s32 timer;
    s32 offsetTrack;
    s32 sizeTrack;
    s32 actionTrack;

    frame = (s16)(fx->timer = fx->timer + 1);
    if (frame == 1)
    {
        u16 *script = fx->script;

        fx->x = script[0] + 1;
        fx->y = script[1] + 1;
        fx->h = script[2] + 1;
    }
    owner = fx->owner;
    timer = (s16)fx->timer;
    offsetTrack = fx->x;
    sizeTrack = fx->y;
    actionTrack = fx->h;
    move_record_by_table(owner, offsetTrack, sizeTrack);
    /* an owner that is its data's own object also moves its children and the data's record list */
    if (owner->data != 0 && (u8 *)owner->data + 8 == (u8 *)owner)
    {
        child = owner->child;
        if (child != 0)
        {
            do
            {
                if ((child->flags79 & 8) != 0)
                {
                    move_record_by_table(child, offsetTrack, sizeTrack);
                }
                child = child->child;
            } while (child != 0);
        }
        listHead = (struct MoveNode *)owner->data;
        first = listHead->next;
        if (first != 0)
        {
            cur = first;
            if (cur != listHead)
            {
                do
                {
                    if (cur->tag != owner->pose)
                    {
                        move_record_by_table(cur->record, offsetTrack, sizeTrack);
                    }
                    cur = cur->next;
                } while (cur != listHead);
            }
        }
    }
    if (offsetTrack != 0)
    {
        s16 *key = &gDeltaTable[offsetTrack];

        if (key[2] < 0)
        {
            fx->x = 0;
        }
        else if (key[2] < timer)
        {
            fx->x = fx->x + 3;
        }
    }
    if (sizeTrack != 0)
    {
        s16 *key = &gDeltaTable[sizeTrack];

        if (key[2] < 0)
        {
            fx->y = 0;
        }
        else if (key[2] < timer)
        {
            fx->y = fx->y + 3;
        }
    }
    if (actionTrack != 0)
    {
        s16 *key = &gDeltaTable[actionTrack];

        if (key[0] < timer)
        {
            /* key[2] is the next key's frame */
            if (key[2] < 0)
            {
                fx->h = 0;
            }
            else
            {
                fx->h = fx->h + 2;
            }
        }
        else if (key[0] == timer)
        {
            if ((owner->pose->flags & 0x20) == 0)
            {
                sprite_set_action(owner, key[1], 0, 0);
            }
        }
    }
    /* y and h read as one word: both tracks over */
    if (fx->x == 0 && *(s32 *)&fx->y == 0)
    {
        fx->active = 0;
    }
}

/* Particle init: picks a random direction from one mt19937 word, byte by byte. Byte 0 (e) gives
   the vertical part e*e and the horizontal length sqrt(~(e^4)), so both lie on one sphere of
   radius 0x10000; byte 1 is the heading around it; byte 2 plus src[0] is the distance; bit 24
   flips the vertical part downwards. The offset (distance * direction >> 13) goes into posX, posH
   and posY (plus src[1] px). The spin x and dh start at 0 and y at src[2] px; timer 1. */
u32 spawn_random_particle(struct Particle *obj, s16 *src)
{
    u32 rnd;
    u32 elev;
    u32 vert;
    s32 horiz;
    u32 heading;
    s32 v;
    s32 t;
    s32 dirX;
    s32 dirY;
    s32 dist;

    rnd = mt19937_next();
    elev = rnd & 0xFF;
    rnd = rnd >> 8;
    vert = elev * elev;
    horiz = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(~(vert * vert));
    heading = rnd & 0xFF;
    rnd = rnd >> 8;
    v = gCosineTable[heading & 0xFF];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = horiz * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    dirX = t >> 8;
    v = gSineTable[heading & 0xFF];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = horiz * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    dirY = t >> 8;
    dist = (rnd & 0xFF) + src[0];
    rnd = rnd >> 8;
    if ((rnd & 1) != 0)
    {
        vert = -vert;
    }
    t = dirX * dist;
    if (t < 0)
    {
        t += 0x1FFF;
    }
    obj->posX = t >> 13;
    t = vert * dist;
    if (t < 0)
    {
        t += 0x1FFF;
    }
    obj->posH = t >> 13;
    t = dirY * dist;
    if (t < 0)
    {
        t += 0x1FFF;
    }
    obj->posY = (t >> 13) + (src[1] << 8);
    obj->x = 0;
    obj->dh = 0;
    t = src[2];
    obj->y = t << 8;
    obj->timer = 1;
    return 1;
}

/* Particle init: starts it 4 px above its origin with a random rise and a random drift, from one
   mt19937 word byte by byte. Byte 0 scales src[0] by 0.75-1.0 into dh (negated); byte 1 is the
   heading; byte 2 scales src[1] by 0.75-1.0 into the drift speed, split by the heading's cos/sin
   (tables hold 1.0 as 0x4000) into x and y. timer 1; returns 5. */
u32 init_drift_particle(struct Particle *obj, s16 *src)
{
    u32 rnd;
    u32 riseScale;
    u32 speedScale;
    u32 speed;
    u32 heading;
    s32 t;
    s32 v;

    obj->posX = 0;
    obj->posY = 0;
    obj->posH = -0x400;
    rnd = mt19937_next();
    riseScale = (0 - ((rnd & 0xFF) + 0x300)) >> 2;
    obj->dh = ((u32)src[0] * riseScale) >> 8;
    rnd >>= 8;
    heading = rnd & 0xFF;
    rnd >>= 8;
    speedScale = ((rnd & 0xFF) + 0x300) >> 2;
    speed = ((u32)src[1] * speedScale) >> 8;
    v = gCosineTable[heading & 0xFF];
    if (v < 0)
        v += 0x3F;
    t = speed * (v >> 6);
    if (t < 0)
        t += 0xFF;
    obj->x = t >> 8;
    v = gSineTable[heading & 0xFF];
    if (v < 0)
        v += 0x3F;
    t = speed * (v >> 6);
    if (t < 0)
        t += 0xFF;
    obj->y = t >> 8;
    obj->timer = 1;
    return 5;
}

/* One frame of a particle that spirals out around its origin: in polar form, `x` is the radius,
   the low byte of `y` the angle and `dx` the radial speed. Rises by `dh`, turns by params[3]
   per frame, damps dh by params[4] / 256 and dx by params[5] / 256, then places it at
   (radius * cos, radius * sin) and counts its timer down. */
void step_particle(struct Particle *obj, s16 *params)
{
    s32 scaled;
    s32 trig;
    s32 radius;

    obj->posH = obj->posH - obj->dh;
    obj->x = (u16)obj->x + (u16)obj->dx;
    obj->y = (u16)obj->y + (u16)params[3];

    scaled = obj->dh * params[4];
    if (scaled < 0)
    {
        scaled += 0xFF;
    }
    obj->dh = scaled >> 8;

    scaled = obj->dx * params[5];
    if (scaled < 0)
    {
        scaled += 0xFF;
    }
    obj->dx = scaled >> 8;

    /* the tables hold 1.0 as 0x4000; >> 6 makes it 0x100 */
    radius = obj->x;
    trig = gCosineTable[*(u8 *)&obj->y];
    if (trig < 0)
    {
        trig += 0x3F;
    }
    scaled = radius * (trig >> 6);
    if (scaled < 0)
    {
        scaled += 0xFF;
    }
    obj->posX = scaled >> 8;

    trig = gSineTable[*(u8 *)&obj->y];
    if (trig < 0)
    {
        trig += 0x3F;
    }
    scaled = radius * (trig >> 6);
    if (scaled < 0)
    {
        scaled += 0xFF;
    }
    obj->posY = scaled >> 8;
    obj->timer = (u16)obj->timer - 1;
}

ASM_FUNC("asm/code_080DECE0/orbit_point.s", void orbit_point(void))

/* Shows frame obj->rec of a picture-archive animation on a sprite: queues the 0x100 bytes of tiles
 * of archive entry rec->index to OBJ VRAM 0x06016800 and copies the entry after it (the one after
 * that while linked) as sprite palette 15, marking that palette changed. Then waits rec->delay
 * frames in effect_obj_frame. */
void start_obj_frame(struct FrameObj *obj)
{
    struct FrameRec *rec = obj->rec;
    const u16 *entries = gTileHalfTable; /* archive entry numbers */
    void (*copy)(u8 *, u8 *, u32);
    s32 palEntry;
    u32 palEntryOfs;
    s32 zero;

    queue_display_entry(2, (void *)(gUnk_089F46B0 + *(u32 *)(gUnk_089F46B0 + (entries[rec->index] << 2))), (void *)0x06016800, 0x100, 0, 0);
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    palEntry = rec->index + 1;
    if ((((struct SysFlagArea *)gSys)->sysFlags & 6) != 0)
    {
        palEntryOfs = (palEntry + 1) << 1;
    }
    else
    {
        palEntryOfs = palEntry << 1;
    }
    /* gUnk_02000460: sprite palette 15 of the palette buffer */
    copy((u8 *)(gUnk_089F46B0 + *(u32 *)(gUnk_089F46B0 + (*(u16 *)((u8 *)entries + palEntryOfs) << 2))), gUnk_02000460, 0x20);
    ((struct { u16 dirtyPalettes; } *)gSys)->dirtyPalettes |= 1 << 15;
    zero = 0;
    ((struct { u8 filler0[9]; u8 lo4 : 4; u8 hi4 : 4; } *)gWorld)->lo4++;
    obj->value8 = zero;
    obj->delay = rec->delay;
    obj->zero18 = zero;
    obj->callback = (u32)effect_obj_frame;
}

ASM_FUNC("asm/code_080DECE0/effect_obj_frame.s", void effect_obj_frame(void))
