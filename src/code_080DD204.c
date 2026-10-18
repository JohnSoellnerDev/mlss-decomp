#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 busy;
    u8 filler22[0x23 - 0x22];
    u8 record23;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 partner;
    u8 filler0EE[0x113 - 0xEE];
    u8 unk113;
};

struct ActorPoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u32 unk4;
    u32 unk8;
    u32 unkC;
};

struct ActorObj
{
    u8 filler0[0x04];
    struct SpriteNode *node4;
    struct ActorPose *pose;
    struct SpriteNode *node0C;
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x22 - 0x1C];
    u16 fxLife;
    u8 filler24[0x28 - 0x24];
    struct ActorPart *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct ActorObj *);
    u8 filler50[0x54 - 0x50];
    void (*savedState)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u16 progress;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x92 - 0x90];
    s16 unk92;
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 spawnLeft;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
    u8 filler0B4[0xC3 - 0xB4];
    s8 unkC3;
    s8 unkC4;
    s8 unkC5;
};

struct ObjFlags79Bit0
{
    u8 filler0[0x79];
    u8 bit0 : 1;
    u8 rest : 7;
};

struct ObjFlags79
{
    u8 filler0[0x79];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FxTask
{
    u8 filler0[4];
    void *unk4;
    void *unk8;
    u8 filler0C[0x10 - 0x0C];
    u16 counter;
    u8 filler12[0x18 - 0x12];
    u16 unk18;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct FxRec
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 0x4];
    u16 unkE;
};

struct FxBit12
{
    u8 filler0[0x12];
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct FxBits1F
{
    u8 filler0[0x1F];
    u8 a : 2;
    u8 b : 2;
    u8 c : 2;
    u8 d : 2;
};

void obj_release(struct ActorObj *obj);

u32 is_state_busy(void);
void release_obj_131c(void *p);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void on_arrive(struct ActorObj *obj);
void obj_active_part_action_e_then_d43c(struct ActorObj *obj);
void obj_active_action_e_then_d090(struct ActorObj *obj);
void obj_active_pair_action_c_then_e2f8(struct ActorObj *obj);
void obj_pose_active_idle_arrive_b(struct ActorObj *obj);
void obj_float_offscreen_finish(struct ActorObj *obj);
void move_states_obj_pose_active_spawn_child(struct ActorObj *obj);
void set_slot_value(u32 id, s32 value);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void obj_active_drop_to_ground_then_cf58(struct ActorObj *obj);
void obj_active_action_b_then_e2b4(struct ActorObj *obj);
void obj_drop_land_stop(struct ActorObj *obj);
void obj_drop_land_spawn_child(struct ActorObj *obj);
void obj_arrive_toggle_pose(struct ActorObj *obj);
void obj_timer_release_child_start_motion(struct ActorObj *obj);
void obj_partner_action_timer(struct ActorObj *obj);
void obj_timed_bounce_or_finish(struct ActorObj *obj);
void obj_begin_fall_to_owner(struct ActorObj *obj);
u32 obj_slot_found_recoil(struct ActorObj *obj, u32 slot);
void obj_settle_step_land_slot69(struct ActorObj *obj);
extern u8 *gWorld;
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void place_targets(struct ActorObj *obj, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct ActorObj *obj, u32 offsets);
s32 find_frame_entry(struct ActorObj *obj, s32 index);
void obj_list_push_default(struct ActorObj *obj);
u32 link_slot_lookup_clear_11c(struct ActorObj *obj, u32 a, u32 b);
void obj_action8_slot126_timer(struct ActorObj *obj);
u32 obj_random_slot_link_then_next(struct ActorObj *obj, struct ActorObj *arg);
void change_item_count(u32 item, u32 count);
void obj_lower_height_512(struct ActorObj *obj);
extern u8 *gParty;
u32 mt19937_next(void);
void start_motion(struct ActorObj *obj);
u32 rand_below(u32 n, u32 r);
void sound_stop_song(u32 song);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_release(struct ActorObj *obj);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void *task_create_list_4c(void (*fn)(struct FxTask *), void *obj);
void fx_task_tick_release(struct FxTask *task);
void obj_spawn_aimed_child_pair(struct ActorObj *obj);
void obj_child_trail_update(struct ActorObj *obj);
void obj_begin_fall_to_owner_alt(struct ActorObj *obj);
struct SpriteNode *sprite_node_take(void);
void obj_list_push_arg(struct ActorObj *obj, u32 arg);
void obj_fall_spawn_fx_offscreen(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot, ...);
void obj_active_spawn_then_e1fc(struct ActorObj *obj);
void obj_active_spawn_then_e278(struct ActorObj *obj);

static inline void pose_set_mode_one(struct ActorObj *obj)
{
    ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
}

static inline void obj_base_effect(struct ActorObj *obj)
{
    effect_spawn_at(0xCAC, obj->baseX / 0x100 - 0x14, obj->baseY / 0x100 + 1, obj->baseH / 0x100 + 0x10, obj->owner);
}

void obj_wait_free_then_df7c(struct ActorObj *obj);
void obj_wait_free_then_dfa8(struct ActorObj *obj);
void obj_wait_free_then_dfd4(struct ActorObj *obj);
void obj_wait_free_then_e000(struct ActorObj *obj);
void obj_action_d_pose_mode1_then_e02c(struct ActorObj *obj);
void obj_action_d_pose_mode1_then_e0b4(struct ActorObj *obj);
void obj_action_9_pose_mode1_then_e104(struct ActorObj *obj);
void obj_action_9_pose_mode1_then_e180(struct ActorObj *obj);

NONMATCH("asm/code_080DD204/obj_timed_spawn_child.s", void obj_timed_spawn_child(struct ActorObj *obj))
{
    struct ActorObj *child;
    u16 timer;

    s16 *dxp = &obj->dx;
    s16 *dyp = &obj->dy;
    u16 *dhp = (u16 *)&obj->dh;

    *dhp = *dhp + obj->dhStep;
    obj->posX = obj->baseX + *dxp;
    obj->posY = obj->baseY + *dyp;
    obj->posH = obj->baseH + (s16)*dhp;
    timer = obj->timer;
    if ((s16)obj->timer > 0)
    {
        obj->timer = timer - 1;
        return;
    }
    if (obj->spawnLeft > 0)
    {
        obj->spawnLeft = obj->spawnLeft - 1;
        child = obj_spawn(obj->data, 0x417E, obj->targetX, obj->targetY, obj->targetH,
                          ((u32)obj->pose->byte11 << 25) >> 31, 1);
        child->child = obj->child;
        obj->child = child;
        ((struct ObjFlags79Bit0 *)child)->bit0 = 1;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        child->state = obj_begin_fall_to_owner_alt;
        child->handler68 = on_arrive;
        child->childValue = obj->childValue;
        child->owner = obj->owner;
        child->param9C = obj->param9C;
        child->paramA0 = obj->paramA0;
        child->paramA4 = obj->paramA4;
        child->spawnLeft = obj->spawnLeft;
        child->byte74 = 0xFF;
        sprite_set_action(child, 0, 0, 0);
        child->timer = 2;
        child->targetX = obj->targetX;
        child->targetY = obj->targetY;
        child->targetH = obj->targetH;
    }
    obj->state = obj_timed_bounce_or_finish;
}
END_NONMATCH

NONMATCH("asm/code_080DD204/obj_timed_bounce_or_finish.s", void obj_timed_bounce_or_finish(struct ActorObj *obj))
{
    s16 *dxp = &obj->dx;
    s16 *dyp = &obj->dy;
    u16 *dhp = (u16 *)&obj->dh;

    *dhp = *dhp + obj->dhStep;
    obj->posX = obj->baseX + *dxp;
    obj->posY = obj->baseY + *dyp;
    obj->posH = obj->baseH + (s16)*dhp;
    if ((s16)obj->timer > 0)
    {
        obj->timer = obj->timer - 1;
        if (obj->timer == 0)
        {
            if (lookup_slot(obj, 0, 0) != 0)
            {
                return;
            }
            obj->dx = -obj->dx / 2;
            obj->dy = 0;
            obj->dh = 0;
            if (obj->child != 0)
            {
                obj->child->timer = 8;
            }
            obj->state = obj_float_offscreen_finish;
            sprite_set_action(obj, 0, 0, 0);
            return;
        }
    }
    if ((s16)obj->dh > 0)
    {
        return;
    }
    if (obj->posH > obj->ground)
    {
        return;
    }
    obj->posH = obj->ground;
    sound_stop_song(0x117);
    obj_release(obj);
}
END_NONMATCH

void obj_drop_land_spawn_child(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 t;

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
    obj->dh = obj->dh + obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if ((s16)obj->dh > 0)
    {
        return;
    }
    child = obj_spawn(obj->data, 0x4138, obj->baseX / 0x100 - 0x25, obj->baseY / 0x100 - 1,
                      obj->baseH / 0x100 + 0x26, ((u32)obj->pose->byte11 << 25) >> 31, 1);
    child->child = obj->child;
    obj->child = child;
    ((struct Flags78Bits *)&child->dir)->kind = 1;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
    child->state = obj_begin_fall_to_owner;
    child->handler68 = on_arrive;
    child->childValue = obj->childValue;
    child->owner = obj->owner;
    obj_list_push_default(child);
    child->handler60 = (void (*)(struct ActorObj *))obj_slot_found_recoil;
    child->param9C = obj->param9C;
    child->paramA0 = obj->paramA0;
    child->paramA4 = obj->paramA4;
    child->spawnLeft = obj->spawnLeft;
    child->byte74 = 0xFF;
    sprite_set_action(child, 0, 0, 0);
    child->timer = 2;
    obj->childValue = 0;
    sprite_set_action(obj, 0xF, 0, 0);
    pose_set_mode_one(obj);
    obj->state = obj_settle_step_land_slot69;
}

void obj_settle_step_land_slot69(struct ActorObj *obj)
{
    s32 dxt = obj->dx * 253;
    s32 dyt;
    s32 h;

    if (dxt < 0)
    {
        dxt += 0xFF;
    }
    obj->dx = dxt >> 8;
    dyt = obj->dy * 253;
    if (dyt < 0)
    {
        dyt += 0xFF;
    }
    obj->dy = dyt >> 8;
    obj->dh = obj->dh + obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    h = obj->baseH + obj->dh;
    obj->posH = h;
    if (obj->dh > 0)
    {
        return;
    }
    if (h > obj->data->targetH)
    {
        return;
    }
    obj->posH = obj->data->targetH;
    set_slot_value(0x69, -1);
    sprite_set_action(obj, 0, 0, 0);
    on_arrive(obj);
}

NONMATCH("asm/code_080DD204/obj_fall_spawn_fx_then_finish.s", void obj_fall_spawn_fx_then_finish(struct ActorObj *obj))
{
    s16 *dxp = &obj->dx;
    s16 *dyp = &obj->dy;
    void *fx;
    struct FxTask *task;

    obj->dh = obj->dh + obj->dhStep;
    obj->posX = obj->baseX + *dxp;
    obj->posY = obj->baseY + *dyp;
    obj->posH = obj->baseH + obj->dh;
    if ((s16)obj->timer > 0)
    {
        obj->timer = obj->timer - 1;
    }
    else
    {
        obj->timer = 2;
        fx = create_obj_de4(0, 0x417E, 1, -1, -1, -1, -1);
        obj_load_record(fx, 0, -1, 0, 0);
        sprite_update_cd0(fx);
        obj_load_record(fx, 0, obj->pose->record23, 0, 0);
        ((struct FxRec *)fx)->unk0 = ((struct FxRec *)obj->pose)->unk0;
        ((struct FxRec *)fx)->unk2 = ((struct FxRec *)obj->pose)->unk2;
        ((struct FxRec *)fx)->unkE = ((struct FxRec *)obj->pose)->unkE;
        ((struct FxBit12 *)fx)->bit4 = 1;
        ((struct FxBits1F *)fx)->a = ((struct FxBits1F *)obj->pose)->a;
        ((struct FxBits1F *)fx)->b = ((struct FxBits1F *)obj->pose)->b;
        ((struct FxBits1F *)fx)->c = ((struct FxBits1F *)obj->pose)->c;
        ((struct FxBits1F *)fx)->d = ((struct FxBits1F *)obj->pose)->d;
        task = task_create_list_4c(fx_task_tick_release, fx);
        task->counter = 0;
        task->unk18 = 0;
    }
    if ((s16)obj->dh > 0)
    {
        return;
    }
    if (obj->posH > obj->ground)
    {
        return;
    }
    obj->posH = obj->ground;
    sound_stop_song(0x117);
    obj_release(obj);
}
END_NONMATCH

void obj_fall_spawn_fx_offscreen(struct ActorObj *obj)
{
    s32 sx;
    s32 sy;
    s32 sh;
    void *fx;
    struct FxTask *task;

    obj->posX = obj->baseX + obj->dx;
    obj->posH = obj->baseH + obj->dh;
    obj->dh = obj->dh - 0x5E;
    sx = obj->posX / 0x100;
    sy = obj->posY / 0x100;
    sh = obj->posH / 0x100;
    offset_three_words_camera(&sx, &sy, &sh, 0);
    if ((s16)obj->timer > 0)
    {
        obj->timer = obj->timer - 1;
    }
    else
    {
        obj->timer = 2;
        fx = create_obj_de4(0, 0x417E, 1, -1, -1, -1, -1);
        obj_load_record(fx, 0, -1, 0, 0);
        sprite_update_cd0(fx);
        obj_load_record(fx, 0, obj->pose->record23, 0, 0);
        ((struct FxRec *)fx)->unk0 = ((struct FxRec *)obj->pose)->unk0;
        ((struct FxRec *)fx)->unk2 = ((struct FxRec *)obj->pose)->unk2;
        ((struct FxRec *)fx)->unkE = ((struct FxRec *)obj->pose)->unkE;
        ((struct FxBit12 *)fx)->bit4 = 1;
        ((struct FxBits1F *)fx)->a = ((struct FxBits1F *)obj->pose)->a;
        ((struct FxBits1F *)fx)->b = ((struct FxBits1F *)obj->pose)->b;
        ((struct FxBits1F *)fx)->c = ((struct FxBits1F *)obj->pose)->c;
        ((struct FxBits1F *)fx)->d = ((struct FxBits1F *)obj->pose)->d;
        task = task_create_list_4c(fx_task_tick_release, fx);
        task->counter = 0;
        task->unk18 = 0;
    }
    if (sx < -0x10 || sy > 0x110)
    {
        sound_stop_song(0x117);
        obj_release(obj);
    }
}

ASM_FUNC("asm/code_080DD204/obj_spawn_aimed_child_pair.s", void obj_spawn_aimed_child_pair(struct ActorObj *obj))

ASM_FUNC("asm/code_080DD204/obj_child_trail_update.s", void obj_child_trail_update(struct ActorObj *obj))

void fx_task_tick_release(struct FxTask *task)
{
    s32 count = task->counter + 1;

    task->counter = count;
    if ((s16)count > 9)
    {
        release_obj_131c(task->unk8);
        task->unk4 = 0;
    }
}

u32 obj_slot_found_recoil(struct ActorObj *obj, u32 slot)
{
    u32 found = lookup_slot(obj, slot);

    if (found == 0)
    {
        obj->dx = -obj->dx / 2;
        obj->dy = found;
        obj->dh = found;
        obj->state = obj_fall_spawn_fx_offscreen;
        sprite_set_action(obj, 0, 0, 0);
    }
    return found;
}

void obj_fall_spawn_fx_then_finish(struct ActorObj *obj);
void obj_begin_fall_to_owner(struct ActorObj *obj)
{
    s32 *ground = &obj->ground;
    struct ActorPart *data = obj->owner->data;
    s32 targetH = data->targetH;
    s32 offY = -1;
    s32 offH = -8;

    *ground = targetH;
    obj->dhStep = jump_velocity(&obj->dx, (s16 *)((u8 *)ground + 0x18), &obj->dh,
                                data->targetX - obj->baseX,
                                data->targetY - (obj->baseY + offY),
                                targetH - (obj->baseH + offH), 0x4800, obj->param9C, 0x100, 0);
    obj->state = obj_fall_spawn_fx_then_finish;
}

void obj_begin_fall_to_owner_alt(struct ActorObj *obj)
{
    s32 *ground = &obj->ground;
    struct ActorPart *data = obj->owner->data;
    s32 targetH = data->targetH;
    s32 offY = 3;
    s32 offH = -8;

    *ground = targetH;
    obj->dhStep = jump_velocity(&obj->dx, (s16 *)((u8 *)ground + 0x18), &obj->dh,
                                data->targetX - obj->baseX,
                                data->targetY - (obj->baseY + offY),
                                targetH - (obj->baseH + offH), 0x4800, obj->param9C, 0x100, 0);
    obj->state = obj_timed_spawn_child;
}
