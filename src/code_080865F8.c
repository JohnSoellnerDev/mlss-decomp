#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0x11];
    u8 flags11;
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 amount20;
    u8 waitFlag21;
    u8 filler22[0xBA - 0x22];
    u16 unkBA;
};

struct Rec
{
    struct Rec *ringNext;
    u8 filler4[0x38 - 0x4];
    struct Actor *field38;
    u8 filler3C[0x78 - 0x3C];
    u16 arrive78;
    u8 filler7A[0x7C - 0x7A];
    u8 unk7C;
    u8 unk7D;
    u8 flags7E;
    u8 filler7F[0x81 - 0x7F];
    u8 flags81;
    u8 filler82[0xD8 - 0x82];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 slot;
    u8 fillerEE[0xF6 - 0xEE];
    s16 timer;
    u8 filler0F8[0x111 - 0xF8];
    u8 status111;
};

struct CandMgr
{
    u8 filler0[0x8];
    u8 status08;
    u8 filler09[0xC - 0x9];
    u8 flags0C;
    u8 filler0D[0x70 - 0x0D];
    struct Rec *candA;
    struct Rec *candB;
    u8 filler78[0x7C - 0x78];
    struct Rec *cand7C;
};

struct Actor
{
    u8 filler0[0x4];
    void (*routine4)(struct Actor *);
    struct Pose *pose;
    u8 filler0C[0x28 - 0x0C];
    struct Rec *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct Actor *);
    void (*savedState)(struct Actor *);
    s32 field54;
    u8 filler58[0x68 - 0x58];
    struct Actor *activeNext;
    struct Actor *link;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 flags77;
    u8 flags78;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s16 progress;
    u32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0xA8 - 0x90];
    s32 waitCount;
    u16 timer;
    u8 fillerAE[0xF0 - 0xAE];
    void *dataPtr;
    u8 counter;
};

struct PoseBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct CandBits
{
    u8 low : 4;
    u8 high : 4;
};

struct FlagBits81
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
};

extern struct CandMgr *gUnk_03000FD8;
extern const u16 gActionCodes[];

void place_part_sprite(struct Actor *obj);
s32 is_state_busy(void);
void release_object(void *obj);
void task_list_remove(struct Actor *obj);
void pair_begin_setup(void *p);
void pair_begin_setup_b(void *p);
void place_targets(struct Actor *obj, s32 x, s32 y, s32 z, s32 w);
void set_step_and_offsets(struct Actor *obj, s32 a, s32 b);
void start_action_5(struct Actor *obj);
void sprite_set_action(struct Actor *obj, s32 a, s32 b, s32 c);
void task_create_list_4c(void (*fn)(struct Actor *), void *obj);
void task_release_obj(struct Actor *obj);
void noop_state_080883dc(struct Actor *obj);
void obj_step_velocity(struct Actor *obj);
void set_targets_from_record(struct Actor *obj);
void sub_08087C7C(struct Actor *obj);
void obj_stop_step(struct Actor *obj);
void obj_list_remove(struct Actor *obj);
void set_cur_flags_both(s32 flags);
void start_motion(struct Actor *obj);
void link_index_state(struct Actor *obj);
void mark_candidate_pair(struct Actor *obj);
void stop_unless_flagged(struct Actor *obj);
void timed_motion_done(struct Actor *obj);
void clear_candidate_pair(struct Actor *obj);
void *find_lowest_order_pose(struct Actor *obj);
void finish_step_and_place(struct Actor *obj);
void obj_place_step(struct Actor *obj);
void set_slot_value(s32 a, s32 b);
s32 resolve_contact_action(struct Actor *a, struct Actor *b, s32 c, s32 d, s32 e);
void wait_pose_flag(struct Actor *obj);
void set_motion_start(struct Actor *obj, s32 a);
void enter_finish_rebuild(struct Actor *obj);

ASM_FUNC("asm/code_080865F8/sub_080865F8.s", void sub_080865F8(void))

ASM_FUNC("asm/code_080865F8/contact_face.s", void contact_face(void))

ASM_FUNC("asm/code_080865F8/obj_step_velocity.s", void obj_step_velocity(struct Actor *obj))

ASM_FUNC("asm/code_080865F8/obj_attach.s", void obj_attach(void))

ASM_FUNC("asm/code_080865F8/obj_attach_entry.s", void obj_attach_entry(void))

ASM_FUNC("asm/code_080865F8/obj_track_target.s", void obj_track_target(void))

ASM_FUNC("asm/code_080865F8/obj_arrive_check.s", void obj_arrive_check(void))

s32 obj_stop(struct Actor *obj)
{
    s32 mode = obj->mode & 6;
    struct Rec *owner;
    u8 busy;

    if (mode == 6)
        return 0;
    if (mode == 2)
        return 0;
    owner = obj->data;
    if (owner == NULL)
        return 0;
    busy = owner->status111 & 2;
    if (busy != 0)
        return 0;
    obj->state = sub_08087C7C;
    obj->index = busy;
    task_create_list_4c(obj_stop_step, owner);
    {
        s32 status = owner->status111;

        owner->status111 = status | 1;
    }
    release_object(obj->data);
    return 1;
}

ASM_FUNC("asm/code_080865F8/obj_slot_update.s", void obj_slot_update(void))

ASM_FUNC("asm/code_080865F8/effect_from_table_obj.s", void effect_from_table_obj(void))

ASM_FUNC("asm/code_080865F8/effect_follow.s", void effect_follow(void))

void obj_place_start(struct Actor *obj)
{
    s32 code = gActionCodes[5];
    s32 x;
    s32 y;

    if (obj->data->slot == -2)
        code += 1;
    sprite_set_action(obj, 0, code, 0);
    ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
    obj->waitCount = 3;
    x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetX + obj->baseX * 2) / 0x100, 3);
    y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetY + obj->baseY * 2) / 0x100, 3);
    place_targets(obj, x, y, 0, 0x100);
    set_step_and_offsets(obj, 0x700, (obj->baseH / 0x100) + 0x18);
    obj->waitCount--;
    obj->state = obj_place_step;
}

ASM_FUNC("asm/code_080865F8/obj_place_step.s", void obj_place_step(struct Actor *obj))

ASM_FUNC("asm/code_080865F8/obj_finish_placement.s", void obj_finish_placement(void))

ASM_FUNC("asm/code_080865F8/lookup_part.s", void lookup_part(void))

ASM_FUNC("asm/code_080865F8/lookup_slot.s", void lookup_slot(void))

ASM_FUNC("asm/code_080865F8/find_lowest_order_pose.s", void *find_lowest_order_pose(struct Actor *obj))

ASM_FUNC("asm/code_080865F8/arbitrate_candidates.s", void arbitrate_candidates(void))

void clear_candidate_pair(struct Actor *obj)
{
    struct CandMgr *mgr = gUnk_03000FD8;
    struct Rec *a = mgr->candA;
    struct Actor *objA = (struct Actor *)((u8 *)a + 8);
    struct Rec *b = mgr->candB;
    struct Actor *objB = (struct Actor *)((u8 *)b + 8);

    if ((a->flags7E & 6) == 2 && objA->state != NULL)
        return;
    if ((b->flags7E & 6) == 2 && objB->state != NULL)
        return;
    a->unk7C = 0;
    a->unk7D = 0;
    b->unk7C = 0;
    b->unk7D = 0;
    ((struct FlagBits81 *)&a->flags81)->bit2 = 0;
    ((struct FlagBits81 *)&b->flags81)->bit2 = 0;
    obj->routine4 = NULL;
}

void schedule_pose_task(struct Actor *obj)
{
    struct Rec *rec;
    struct Actor *child;
    s32 flags;

    if (obj == NULL)
        return;
    flags = 0;
    rec = obj->data;
    if (rec->ringNext == NULL)
        flags = 1;
    else if (rec->ringNext == rec)
        flags = 1;
    child = obj->child;
    for (;;)
    {
        if (child == NULL)
        {
            flags |= 2;
            break;
        }
        if (child->flags & 8)
            break;
        child = child->child;
    }
    if (flags == 3)
        task_create_list_4c(clear_candidate_pair, obj);
    else
        task_create_list_4c(mark_candidate_pair, find_lowest_order_pose(obj));
}

s32 state_clear_08087958(struct Actor *obj)
{
    obj->state = NULL;
    return 1;
}

ASM_FUNC("asm/code_080865F8/lookup_link_slot.s", void lookup_link_slot(void))

s32 query_contact(struct Actor *obj, struct Actor *other, s32 kind)
{
    struct CandMgr *mgr;

    if (kind != 3)
        return 1;
    if ((obj->mode & 6) == 4 && obj->link != NULL && (((u8 *)obj->link)[9] & 0x80) == 0)
        return 1;
    set_slot_value(0x2A, -1);
    if (other->data->slot == -1)
    {
        mgr = gUnk_03000FD8;
        ((struct CandBits *)&mgr->status08)->low = 1;
    }
    if (other->data->slot == -2)
    {
        mgr = gUnk_03000FD8;
        ((struct CandBits *)&mgr->status08)->low = 4;
    }
    return 0;
}

void start_placement(struct Actor *obj)
{
    s32 flags;

    place_targets(obj, (obj->baseX / 0x100) - 0x40, obj->baseY / 0x100, 0, 0x100);
    set_step_and_offsets(obj, 0x700, (obj->baseH / 0x100) + 0x18);
    sprite_set_action(obj, 0, gActionCodes[0] + ~obj->data->slot, 0);
    flags = obj->pose->flags12;
    obj->pose->flags12 = flags | 0x10;
    obj->state = finish_step_and_place;
}

void timed_wait_step(struct Actor *obj)
{
    s32 timer = obj->data->timer;
    void (*next)(struct Actor *);

    if (timer == 0)
    {
        s32 status = obj->data->status111;

        obj->data->status111 = status | 1;
        task_create_list_4c(task_release_obj, obj);
        obj->waitCount = timer;
        next = noop_state_080883dc;
    }
    else
    {
        if (obj->waitCount > 0)
        {
            obj->waitCount--;
            return;
        }
        sprite_set_action(obj, 0, -1, 0);
        ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
        next = NULL;
    }
    obj->state = next;
}

void place_and_start_action5(struct Actor *obj, s32 a, s32 b)
{
    place_targets(obj, obj->baseX / 0x100, obj->baseY / 0x100, 0, 0x100);
    set_step_and_offsets(obj, b, a);
    obj->state = NULL;
    start_action_5(obj);
}

void candidate_dispatch(void)
{
    struct CandMgr *mgr = gUnk_03000FD8;

    if (mgr->flags0C & 0x80)
        pair_begin_setup_b((u8 *)mgr->cand7C + 8);
    else
        pair_begin_setup((u8 *)mgr->cand7C + 8);
}

void release_object_data(struct Actor *obj)
{
    if (obj != NULL)
    {
        release_object(obj);
        task_list_remove(obj);
        if (obj->counter <= 5)
            obj->dataPtr = NULL;
    }
}

void motion_timed(struct Actor *obj, s32 a, s32 w)
{
    struct Pose *pose;

    sprite_set_action(obj, a, 0, 0);
    pose = obj->pose;
    if (w != 0)
    {
        w = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(w << 4, (s16)obj->timer);
        if (w <= 0)
            w = 1;
        if (w > 0x7E)
            w = 0x7F;
        pose->amount20 = w;
        ((struct PoseBits *)&pose->flags12)->mode = 1;
    }
    else
    {
        ((struct PoseBits *)&pose->flags12)->mode = 0;
    }
    obj->state = timed_motion_done;
}

void obj_stop_step(struct Actor *obj)
{
    obj->pose->unkBA = 0;
    obj->routine4 = place_part_sprite;
}

void sub_08087C7C(struct Actor *obj)
{
}

ASM_FUNC("asm/code_080865F8/process_record_ring.s", void process_record_ring(void))

void set_targets_from_record(struct Actor *obj)
{
    obj->targetX = obj->data->targetX;
    obj->targetY = obj->data->targetY;
    obj->targetH = obj->data->targetH;
    {
        u8 *flagsPtr = &obj->flags;

        *flagsPtr |= 0x20;
    }
    obj->motionSpeed = 0x800;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = link_index_state;
}

void wait_for_index(struct Actor *obj)
{
    if (obj->index == 0)
        obj->state = set_targets_from_record;
}

void wait_pose_flag(struct Actor *obj)
{
    if (obj->pose->waitFlag21 == 0)
        obj->state = set_targets_from_record;
}

void on_arrive(struct Actor *obj)
{
    struct Rec *rec = obj->data;

    if (rec != NULL && obj == (struct Actor *)((u8 *)rec + 8) && obj->pose != NULL)
    {
        rec->arrive78 = 0;
        sprite_set_action(obj, -1, -1, 0);
    }
    obj_list_remove(obj);
    obj->activeNext = NULL;
    obj->field54 = 0;
    obj->state = NULL;
    obj->link = NULL;
    obj->owner = NULL;
    if ((obj->mode & 6) == 2)
        set_cur_flags_both(0x40000);
}

void poll_link_routine(struct Actor *obj)
{
    if (is_state_busy() == 0)
        obj->link->routine4(obj);
}

void candidates_countdown_then_state(struct Actor *obj)
{
    u16 t = obj->timer;

    if ((s16)obj->timer > 0)
        obj->timer = t - 1;
    else
        obj->state = obj_step_velocity;
}
