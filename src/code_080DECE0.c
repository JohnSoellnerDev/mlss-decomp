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
    s16 partner;
    u8 filler0EE[0x113 - 0xEE];
    u8 codeByte;
};

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
    u8 sysFlags;
};

struct FrameRec
{
    u8 filler0[2];
    s16 index;
    u16 delay;
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

struct FxMove
{
    u8 filler0[0x4];
    s32 active;
    struct ActorObj *owner;
    u16 *script;
    u16 timer;
    s16 x;
    s16 y;
    s16 h;
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

void obj_state_spawn_child(struct ActorObj *obj)
{
    struct ActorPose *pose = obj->pose;

    if (pose->flags & 8)
    {
        struct ActorData *data = obj->data;

        if (data->codeByte & 0x80)
        {
            struct ActorObj *child;

            child = obj_spawn(data, *(u16 *)&obj->codeId, obj->baseX / 0x100 - 0x14,
                              obj->baseY / 0x100 - 1, obj->baseH / 0x100 + 0x14,
                              ((u32)pose->byte11 << 25) >> 31, 1);
            obj->child = child;
            child->kind78 = child->kind78 & 0x1F;
            copy_child_flags(obj, child);
            child->state = obj_begin_jump_to_data;
            child->handler68 = on_arrive;
            child->childValue = obj->childValue;
            child->owner = obj->owner;
            obj_list_push_default(child);
            child->handler58 = obj_unlink_release_code;
            child->param9C = obj->param9C;
            child->paramA0 = obj->paramA0;
            child->paramA4 = obj->paramA4;
            child->codeId = obj->codeId;
            child->byte74 = 0xFF;
            sprite_set_action(child, 0, 0, 0);
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


NONMATCH("asm/code_080DECE0/obj_begin_move_pair.s", void obj_begin_move_pair(struct ActorObj *obj))
{
    set_pair_target(obj);
    obj->motionSpeed = obj->paramA0 << 8;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x11C, -1);
    sprite_set_action(obj, 0xD, 0, 0);
    obj->state = obj_wait_link_then_timer;
}
END_NONMATCH

void obj_timer_begin_move_to_data(struct ActorObj *obj)
{
    u32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s32)(timer << 16) < 0)
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

NONMATCH("asm/code_080DECE0/obj_timer_release_move_pair.s", void obj_timer_release_move_pair(struct ActorObj *obj))
{
    if (obj->link == 0)
    {
        u32 timer = obj->timer - 1;

        obj->timer = timer;
        if ((s32)(timer << 16) < 0)
        {
            sound_stop_song(0x11A);
            obj_list_remove(obj);
            set_pair_target(obj);
            obj->motionSpeed = 0x200;
            obj->motionStart = 0;
            start_motion(obj);
            sprite_set_action(obj, 0xD, 0, 0);
            obj->state = obj_state_idle_if_unlinked;
        }
    }
}
END_NONMATCH

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
    if (obj->data->codeByte & 0x80)
    {
        change_item_count(obj->data->codeByte & 0x7F, 1);
        obj->data->codeByte = 0;
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

NONMATCH("asm/code_080DECE0/obj_load_code_entry_then_wait.s", void obj_load_code_entry_then_wait(struct ActorObj *obj))
{
    void (*next)(struct ActorObj *);
    u8 *codeByte = &obj->data->codeByte;

    if ((*codeByte & 0x80) == 0 || *codeByte == 0xFF)
    {
        next = 0;
    }
    else
    {
        u32 *codeIdSlot = &obj->codeId;
        u8 *entry = gItemTable + ((*codeByte & 0x7F) << 4);
        u32 id = (entry[2] & 7) + 0x4188;

        *codeIdSlot = id;
        ensure_code_entry(id);
        next = obj_wait_not_busy_action10;
    }
    obj->state = next;
}
END_NONMATCH

void obj_select_code_entry_world(struct ActorObj *obj)
{
    u32 index = obj->data->codeByte;
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

ASM_FUNC("asm/code_080DECE0/fx_script_step.s", void fx_script_step(void))

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

void fx_tick_move_records(struct FxMove *fx)
{
    struct ActorObj *owner;
    struct ActorObj *node;
    struct MoveNode *list_head;
    struct MoveNode *first;
    struct MoveNode *cur;
    s32 counter;
    s32 timer;
    s32 x;
    s32 y;
    s32 h;

    counter = (s16)(fx->timer = fx->timer + 1);
    if (counter == 1)
    {
        u16 *script = fx->script;

        fx->x = script[0] + 1;
        fx->y = script[1] + 1;
        fx->h = script[2] + 1;
    }
    owner = fx->owner;
    timer = (s16)fx->timer;
    x = fx->x;
    y = fx->y;
    h = fx->h;
    move_record_by_table(owner, x, y);
    if (owner->data != 0 && (u8 *)owner->data + 8 == (u8 *)owner)
    {
        node = owner->child;
        if (node != 0)
        {
            do
            {
                if ((node->flags79 & 8) != 0)
                {
                    move_record_by_table(node, x, y);
                }
                node = node->child;
            } while (node != 0);
        }
        list_head = (struct MoveNode *)owner->data;
        first = list_head->next;
        if (first != 0)
        {
            cur = first;
            if (cur != list_head)
            {
                do
                {
                    if (cur->tag != owner->pose)
                    {
                        move_record_by_table(cur->record, x, y);
                    }
                    cur = cur->next;
                } while (cur != list_head);
            }
        }
    }
    if (x != 0)
    {
        s16 *row = &gDeltaTable[x];

        if (row[2] < 0)
        {
            fx->x = 0;
        }
        else if (row[2] < timer)
        {
            fx->x = fx->x + 3;
        }
    }
    if (y != 0)
    {
        s16 *row = &gDeltaTable[y];

        if (row[2] < 0)
        {
            fx->y = 0;
        }
        else if (row[2] < timer)
        {
            fx->y = fx->y + 3;
        }
    }
    if (h != 0)
    {
        s16 *col = &gDeltaTable[h];

        if (col[0] < timer)
        {
            if (col[2] < 0)
            {
                fx->h = 0;
            }
            else
            {
                fx->h = fx->h + 2;
            }
        }
        else if (col[0] == timer)
        {
            if ((owner->pose->flags & 0x20) == 0)
            {
                sprite_set_action(owner, col[1], 0, 0);
            }
        }
    }
    if (fx->x == 0 && *(s32 *)&fx->y == 0)
    {
        fx->active = 0;
    }
}

u32 spawn_random_particle(struct Particle *obj, s16 *src)
{
    u32 rnd;
    u32 angle;
    u32 angleSq;
    s32 length;
    u32 k;
    s32 v;
    s32 t;
    s32 dirX;
    s32 dirY;
    s32 base;

    rnd = mt19937_next();
    angle = rnd & 0xFF;
    rnd = rnd >> 8;
    angleSq = angle * angle;
    length = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(~(angleSq * angleSq));
    k = rnd & 0xFF;
    rnd = rnd >> 8;
    v = gCosineTable[k & 0xFF];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = length * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    dirX = t >> 8;
    v = gSineTable[k & 0xFF];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = length * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    dirY = t >> 8;
    base = (rnd & 0xFF) + src[0];
    rnd = rnd >> 8;
    if ((rnd & 1) != 0)
    {
        angleSq = -angleSq;
    }
    t = dirX * base;
    if (t < 0)
    {
        t += 0x1FFF;
    }
    obj->posX = t >> 13;
    t = angleSq * base;
    if (t < 0)
    {
        t += 0x1FFF;
    }
    obj->posH = t >> 13;
    t = dirY * base;
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

NONMATCH("asm/code_080DECE0/init_drift_particle.s", u32 init_drift_particle(struct Particle *obj, s16 *src))
{
    u32 rnd;
    u32 scaleA;
    u32 hi;
    u32 scaleB;
    u32 yScaled;
    u32 idx;
    s32 t;
    s32 v;

    obj->posX = 0;
    obj->posY = 0;
    obj->posH = -0x400;
    rnd = mt19937_next();
    scaleA = (0 - ((rnd & 0xFF) + 0x300)) >> 2;
    obj->dh = ((u32)src[0] * scaleA) >> 8;
    hi = rnd >> 8;
    idx = hi;
    idx &= 0xFF;
    hi = hi >> 8;
    scaleB = ((hi & 0xFF) + 0x300) >> 2;
    yScaled = ((u32)src[1] * scaleB) >> 8;
    v = gCosineTable[idx & 0xFF];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = yScaled * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->x = t >> 8;
    v = gSineTable[idx & 0xFF];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = yScaled * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->y = t >> 8;
    obj->timer = 1;
    return 5;
}
END_NONMATCH

void step_particle(struct Particle *obj, s16 *params)
{
    s32 t;
    s32 v;
    s32 radius;

    obj->posH = obj->posH - obj->dh;
    obj->x = (u16)obj->x + (u16)obj->dx;
    obj->y = (u16)obj->y + (u16)params[3];

    t = obj->dh * params[4];
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->dh = t >> 8;

    t = obj->dx * params[5];
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->dx = t >> 8;

    radius = obj->x;
    v = gCosineTable[*(u8 *)&obj->y];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = radius * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->posX = t >> 8;

    v = gSineTable[*(u8 *)&obj->y];
    if (v < 0)
    {
        v += 0x3F;
    }
    t = radius * (v >> 6);
    if (t < 0)
    {
        t += 0xFF;
    }
    obj->posY = t >> 8;
    obj->timer = (u16)obj->timer - 1;
}

ASM_FUNC("asm/code_080DECE0/orbit_point.s", void orbit_point(void))

NONMATCH("asm/code_080DECE0/start_obj_frame.s", void start_obj_frame(struct FrameObj *obj))
{
    struct FrameRec *rec = obj->rec;
    const u16 *tbl;
    void (*copy)(u8 *, u8 *, u32);
    u32 frame;
    u32 frame2;
    s32 next;
    u32 off;
    s32 counter;

    tbl = gTileHalfTable;
    frame = (u32)gUnk_089F46B0 + *(u32 *)(gUnk_089F46B0 + (tbl[rec->index] << 2));
    queue_display_entry(2, (void *)frame, (void *)0x06016800, 0x100, 0, 0);
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    next = rec->index + 1;
    if ((((struct SysFlagArea *)gSys)->sysFlags & 6) != 0)
    {
        off = (next + 1) << 1;
    }
    else
    {
        off = next << 1;
    }
    frame2 = (u32)gUnk_089F46B0 + *(u32 *)(gUnk_089F46B0 + (*(u16 *)((u8 *)tbl + off) << 2));
    copy((u8 *)frame2, gUnk_02000460, 0x20);
    *(u16 *)gSys |= 0x8000;
    counter = gWorld[9];
    gWorld[9] = (counter & -16) | (((((u32)counter << 28) >> 28) + 1) & 0xF);
    obj->value8 = 0;
    obj->delay = rec->delay;
    obj->zero18 = 0;
    obj->callback = (u32)effect_obj_frame;
}
END_NONMATCH

ASM_FUNC("asm/code_080DECE0/effect_obj_frame.s", void effect_obj_frame(void))
