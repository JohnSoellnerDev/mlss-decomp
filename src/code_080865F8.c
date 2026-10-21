#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10;
    u8 flags11;
    u8 flags12;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F;
    u8 amount20;
    u8 waitFlag21;
          u8 filler22;
      u8 unk23;
      u8 filler24[0xBA - 0x24];
      u16 unkBA;
};

struct Rec
{
    struct Rec *ringNext;
          u8 filler4[0x10 - 0x4];
      struct Pose *pose10;
      u8 filler14[0x38 - 0x14];
      struct Actor *field38;
          u8 filler3C[0x4C - 0x3C];
      void (*state)(struct Actor *);
      u8 filler50[0x78 - 0x50];
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
          u8 fillerEE[0xF0 - 0xEE];
      struct SlotTable *table;
      u8 fillerF4[0xF6 - 0xF4];
      s16 timer;
      u8 filler0F8[0x110 - 0xF8];
      u8 status110;
      u8 status111;
};

struct SlotEntry
{
    u8 filler0[0x8];
    u8 flags8;
    u8 filler9[0xE - 0x9];
    s16 offE;
    s16 off10;
    u16 unk12;
};

struct SlotTable
{
    u8 filler0[0xC];
    struct SlotEntry *entries;
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
          struct Rec *list78;
      struct Rec *cand7C;
};

struct Actor
{
    u8 filler0[0x4];
    void (*routine4)(struct Actor *);
    struct Pose *pose;
          u8 filler0C[0x10 - 0x0C];
      s32 posX;
      s32 posY;
      s32 posZ;
      u8 filler1C[0x28 - 0x1C];
      struct Rec *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 unk44;
        u8 unk45;
        u8 unk46;
        u8 unk47;
        u8 unk48;
        u8 unk49;
        u8 filler4A[0x4C - 0x4A];
    void (*state)(struct Actor *);
    void (*savedState)(struct Actor *);
    s32 field54;
    s32 field58;
        s32 field5C;
        s32 field60;
        s32 field64;
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
                  u8 filler90[0x94 - 0x90];
        s32 unk94;
        s32 unk98;
        s32 unk9C;
        s32 unkA0;
        s32 unkA4;
      s32 waitCount;
      u16 timer;
      s16 offX;
      s16 offY;
      s16 offZ;
      u8 fillerB4[0xC8 - 0xB4];
      u16 unkC8;
      u16 unkCA;
      u8 unkCC;
      u8 fillerCD[0xF0 - 0xCD];
    void *dataPtr;
    u8 counter;
};

void on_arrive(struct Actor *obj);
void poll_link_routine(struct Actor *obj);

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

struct PoseFlags11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FlagBits81
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
};

extern struct CandMgr *gWorld;
extern const u16 gActionCodes[];

void place_part_sprite(struct Actor *obj);
void effect_follow(struct Actor *obj);
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
void noop_state_08087c7c(struct Actor *obj);
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
void obj_release(struct Actor *obj);
u32 arc_tan2(s32 x, s32 y);
extern const s16 gOffsetRows[];

void sub_08087FCC(struct Actor *obj, s32 divisor);

NONMATCH("asm/code_080865F8/sub_080865F8.s", void sub_080865F8(struct Actor *obj, s32 x, s32 y, s32 z, s32 h))
{
    s32 *p94;
    s32 *p98;
    s32 pz;
    s32 zt;
    s32 f1;
    s32 f2;
    s32 neg;

    x <<= 8;
    y <<= 8;
    z <<= 8;
    h <<= 8;
    obj->targetX = x;
    obj->targetY = y;
    obj->offX = (x - obj->posX) / 4;
    obj->offY = (y - obj->posY) / 4;
    if (h < z || h < obj->posZ)
    {
        obj->targetH = z;
        obj->offZ = (z - obj->posZ) / 4;
        obj->unk94 = 0;
    }
    else
    {
        obj->targetH = h;
        obj->offZ = 0;
        p94 = &obj->unk94;
        p98 = &obj->unk98;
        pz = obj->posZ;
        zt = obj->targetH;
        neg = 0;
        if (zt < pz)
        {
            if (zt < z)
            {
                zt = pz + z - zt;
                neg = 1;
            }
            else
                zt = pz;
        }
        else if (zt < z)
            zt = z;
        f1 = ((s32 (*)(s32))gMathRoutines)((zt - z) << 8);
        f2 = ((s32 (*)(s32))gMathRoutines)((zt - pz) << 8);
        *p94 = f1 + f2;
        *p98 = f2;
        if (neg)
        {
            *p94 = -*p94;
            *p98 = -*p98;
        }
    }
    obj->progress = 0x4000;
    sub_08087FCC(obj, 0x20);
}
END_NONMATCH

ASM_FUNC("asm/code_080865F8/contact_face.s", void contact_face(void))

void obj_step_velocity(struct Actor *obj)
{
    struct Rec *rec = obj->data;
    s32 dx = rec->targetX - obj->baseX;
    s32 dh;
    s32 cellsX;
    s32 cellsH;

    if (dx < 0)
        dx += 0x1F;
    cellsX = dx >> 5;
    dh = rec->targetH - obj->baseH;
    if (dh < 0)
        dh += 0x1F;
    cellsH = dh >> 5;
    *(u16 *)&obj->offX += cellsX;
    *(u16 *)&obj->offZ += cellsH;
    obj->offX = (obj->offX * 0xF2) / 0x100;
    obj->offZ = (obj->offZ * 0xF2) / 0x100;
    obj->posX += obj->offX;
    obj->posZ += obj->offZ;
    if (obj->offX * obj->offX + cellsX * cellsX <= 0x40 && obj->offZ * obj->offZ + cellsH * cellsH <= 0x40)
    {
        obj->posX = obj->data->targetX;
        obj->posZ = obj->data->targetH;
        obj->state = NULL;
    }
}

ASM_FUNC("asm/code_080865F8/obj_attach.s", void obj_attach(void))

ASM_FUNC("asm/code_080865F8/obj_attach_entry.s", void obj_attach_entry(void))

void obj_arrive_check(struct Actor *obj);
ASM_FUNC("asm/code_080865F8/obj_track_target.s", void obj_track_target(struct Actor *obj))

NONMATCH("asm/code_080865F8/obj_arrive_check.s", void obj_arrive_check(struct Actor *obj))
{
    struct Rec *rec;
    s32 newX;
    s32 newY;
    s32 newH;
    s32 tgtX;
    s32 tgtY;
    s32 tgtH;
    s32 dxq;
    s32 dyq;
    s32 dhq;
    s32 bxq;
    s32 byq;
    s32 bhq;
    s32 d2;
    s32 b2;
    s32 flags;
    struct Pose *pose;

    newX = obj->baseX + obj->offX;
    obj->posX = newX;
    newY = obj->baseY + obj->offY;
    obj->posY = newY;
    newH = obj->baseH + obj->offZ;
    obj->posZ = newH;
    rec = obj->data;
    tgtX = rec->targetX;
    dxq = (newX - tgtX) / 0x10;
    tgtY = rec->targetY;
    dyq = (newY - tgtY) / 0x10;
    tgtH = rec->targetH;
    dhq = (newH - tgtH) / 0x10;
    bxq = (obj->baseX - tgtX) / 0x10;
    byq = (obj->baseY - tgtY) / 0x10;
    bhq = (obj->baseH - tgtH) / 0x10;
    d2 = dxq * dxq + dyq * dyq + dhq * dhq;
    b2 = bxq * bxq + byq * byq + bhq * bhq;
    if (d2 >= b2)
    {
        obj->posX = tgtX;
        obj->posY = rec->targetY;
        obj->posZ = rec->targetH;
        sprite_set_action(obj, -1, -1, 0);
        pose = obj->pose;
        flags = pose->flags11;
        flags &= -0x41;
        pose->flags11 = flags;
        obj->field58 = 0;
        obj->state = obj->savedState;
        obj->savedState = NULL;
    }
}
END_NONMATCH

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
    obj->state = noop_state_08087c7c;
    obj->index = busy;
    task_create_list_4c(obj_stop_step, owner);
    {
        s32 status = owner->status111;

        owner->status111 = status | 1;
    }
    release_object(obj->data);
    return 1;
}

void obj_track_target(struct Actor *obj);
void start_placement(struct Actor *obj);
ASM_FUNC("asm/code_080865F8/obj_slot_update.s", void obj_slot_update(void))

ASM_FUNC("asm/code_080865F8/effect_from_table_obj.s", void effect_from_table_obj(void))

void effect_follow(struct Actor *obj)
{
    struct Pose *pose;
    struct Rec *cur;
    const s16 *entry;
    s32 targetX;
    s32 x;
    s32 rowOff;
    const u8 *base;

    pose = obj->child->pose;
    if (pose == NULL || pose->waitFlag21 != obj->motionSpeed || (pose->flags12 & 0x20) != 0)
    {
        obj_release(obj);
        return;
    }
    cur = gWorld->list78;
    for (;;)
    {
        if (cur == NULL)
        {
            obj_release(obj);
            return;
        }
        if (cur != (struct Rec *)obj && cur->state != NULL)
            break;
        cur = cur->ringNext;
    }
    base = (const u8 *)gOffsetRows + (obj->motionStart << 1);
    rowOff = pose->unk23 * 6 + 12;
    entry = (const s16 *)(base + rowOff);
    targetX = obj->targetX;
    if ((pose->flags11 & 0x40) != 0)
        x = targetX - (entry[0] << 8);
    else
        x = targetX + (entry[0] << 8);
    obj->posX = x;
    obj->posY = obj->targetY + (entry[1] << 8);
    obj->posZ = obj->targetH + (entry[2] << 8);
}

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

u8 step_motion(struct Actor *obj);
void timed_wait_step(struct Actor *obj);
void obj_place_step(struct Actor *obj)
{
    s32 n;
    s32 *count;
    s32 x;
    s32 y;

    if (step_motion(obj) == 0)
    {
        count = &obj->waitCount;
        n = *count;
        if (n != 0)
        {
            x = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetX + obj->baseX * (n - 1)) / 0x100, n);
            y = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->data->targetY + obj->baseY * (n - 1)) / 0x100, n);
            place_targets(obj, x, y, 0, 0x100);
            set_step_and_offsets(obj, 0x700, (obj->baseH / 0x100) + 0x14);
            *count = *count - 1;
        }
        else
        {
            *count = 0x14;
            obj->state = timed_wait_step;
        }
    }
}

void obj_finish_placement(struct Actor *obj)
{
    struct Rec *rec;
    s32 action;

    if ((obj->pose->flags12 & 8) != 0)
    {
        rec = obj->data;
        place_targets(obj, rec->targetX / 0x100, rec->targetY / 0x100, rec->targetH / 0x100, 0x100);
        set_motion_start(obj, 0x400);
        action = (rec->slot == -1) ? 0x2002 : 0x204F;
        sprite_set_action(obj, 0, action, 0);
        ((struct PoseFlags11 *)&obj->pose->flags11)->bit6 = 1;
        ((struct PoseBits *)&obj->pose->flags12)->mode = 0;
        obj->state = NULL;
        enter_finish_rebuild(obj);
    }
}

ASM_FUNC("asm/code_080865F8/lookup_part.s", void lookup_part(void))

s32 lookup_slot(struct Actor *obj, struct Actor *other)
{
    s32 otherX;
    s32 otherH;
    s32 objX;
    s32 objH;
    s32 target;
    s32 limit;
    s32 wrapped;
    struct CandMgr *mgr;

    if (other != other->data->field38)
        return 1;
    otherX = other->posX / 0x100;
    otherH = (-other->posZ) / 0x100;
    objX = obj->posX / 0x100;
    objH = (-obj->posZ) / 0x100;
    target = (u16)arc_tan2((s16)(objX - otherX), (s16)(objH - otherH));
    wrapped = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((-other->unkA0) << 8, 0x168);
    limit = wrapped & 0xFFFF;
    if ((other->data->pose10->flags11 & 0x40) != 0)
        target = (target + 0x8000) & 0xFFFF;
    if (target < limit)
        target += 0xFFFF;
    if (target - limit > 0x8000)
        return -1;
    set_slot_value(0x7D, -1);
    if (other->data->slot == -1)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 1;
    }
    if (other->data->slot == -2)
    {
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 4;
    }
    return 0;
}

void *find_lowest_order_pose(struct Actor *obj)
{
    struct Pose *best = obj->pose;
    struct Actor *child;
    struct Rec *head;
    struct Rec *rec;
    struct Rec *next;

    for (child = obj->child; child != NULL; child = child->child)
    {
        if ((child->flags & 8) && child->pose->unkE < best->unkE)
            best = child->pose;
    }
    head = obj->data;
    rec = head->ringNext;
    if (rec != NULL)
    {
        while (rec != head)
        {
            if (rec->pose10->unkE < best->unkE)
                best = rec->pose10;
            child = rec->field38;
            next = rec->ringNext;
            for (; child != NULL; child = child->child)
            {
                if ((child->flags & 8) && child->pose->unkE < best->unkE)
                    best = child->pose;
            }
            rec = next;
        }
    }
    return best;
}

ASM_FUNC("asm/code_080865F8/arbitrate_candidates.s", void arbitrate_candidates(void))

void clear_candidate_pair(struct Actor *obj)
{
    struct CandMgr *mgr = gWorld;
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

s32 lookup_slot();
s32 lookup_link_slot(struct Actor *a, struct Actor *b)
{
    struct Rec *rec;
    struct Actor *cur;
    s32 result;
    u8 found;

    result = lookup_slot(a, b);
    if (result != 0)
        return result;
    result = resolve_contact_action(b, a, 0, 0, (b->data->slot == -1) ? 0x8032012 : 0x8032013);
    if (result > 0)
        return result;
    rec = a->data;
    if (rec != NULL)
    {
        found = 0;
        if ((void *)a != (u8 *)rec + 8)
        {
            cur = rec->field38;
            if (cur != NULL)
            {
                if (a == cur)
                    goto set;
                do
                {
                    cur = cur->child;
                    if (cur == NULL)
                        goto check;
                } while (a != cur);
                found = 1;
            }
        check:
            if (!found)
                return 0;
        }
    set:
        a->index = 0;
        a->state = wait_pose_flag;
    }
    return 0;
}

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
        mgr = gWorld;
        ((struct CandBits *)&mgr->status08)->low = 1;
    }
    if (other->data->slot == -2)
    {
        mgr = gWorld;
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
    struct CandMgr *mgr = gWorld;

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

void noop_state_08087c7c(struct Actor *obj)
{
}

void obj_release(struct Actor *obj);
extern const s16 gOffsetRows[];
NONMATCH("asm/code_080865F8/process_record_ring.s", s32 process_record_ring(struct Actor *obj))
{
    struct Rec *next;
    struct Rec *rec = obj->data;
    struct Rec *cur;

    if ((next = rec->ringNext) != NULL)
    {
        do
        {
            cur = next;
            if ((cur->status111 & 2) != 0)
                task_create_list_4c(stop_unless_flagged, cur);
            else
                obj_stop((struct Actor *)((u8 *)cur + 8));
        } while ((next = cur->ringNext) != rec);
    }
    if ((rec->status111 & 2) != 0)
        task_create_list_4c(stop_unless_flagged, rec);
    else
        obj_stop(obj);
    return 1;
}
END_NONMATCH

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
