#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 phase;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 cursorX;
    s16 cursorY;
    u8 filler3[0x104 - 0xFA];
    s16 unk104;
    u8 filler4[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
};

struct PoseBits
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 top : 1;
};

struct ObjBits78
{
    u8 low5 : 5;
    u8 top3 : 3;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gWorld;
extern u32 gUnk_03000E3C;
extern u8 gUnk_03000E7C;
extern u8 gUnk_03000E7D;
extern const u8 gStepSequenceTable[];

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x28 - 0x1C];
    struct ActorPart *part;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x54 - 0x50];
    void (*handler54)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    struct ActorPose *link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_arc_start(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_anim5_then_spawn(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_place_at_base_when_index_zero(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_timer_follow_owner(struct ActorObj *obj);
void obj_start_move_right(struct ActorObj *obj);
void obj_copy_bytes_sprite_88(struct ActorObj *obj);
void obj_init_pair_motion(struct ActorObj *obj);
void obj_anim6_wait4(struct ActorObj *obj);
void obj_move_to_part_x_offset(struct ActorObj *obj);
void obj_spawn_child_4115(struct ActorObj *obj);
void obj_spawn_child_4113(struct ActorObj *obj);
void obj_place_sprite_anim9(struct ActorObj *obj);
void obj_link_owner_go(struct ActorObj *obj);
void obj_lookup_slot_start(struct ActorObj *obj);
void obj_setup_wait_target(struct ActorObj *obj);
void obj_lookup_part_setup(struct ActorObj *obj);
void obj_fall_to_ground(struct ActorObj *obj);
void obj_wave_left(struct ActorObj *obj);
void obj_timer_to_move(struct ActorObj *obj);
void obj_wave_left_to_target(struct ActorObj *obj);
void obj_wave_rise_stop(struct ActorObj *obj);
void obj_wave_fall_stop(struct ActorObj *obj);
void obj_move_by_path(void);
void obj_count_down_exit(struct ActorObj *obj);
void obj_timer_table_next(struct ActorObj *obj);
void obj_link_then_finish(struct ActorObj *obj);
void obj_timer_finish(struct ActorObj *obj);
void obj_arc_step_clamped(struct ActorObj *obj);
void obj_arc_to_base_sprite(struct ActorObj *obj);
void obj_pose_idle(struct ActorObj *obj);

static inline s32 arc_px(s32 v)
{
    if (v < 0)
    {
        v += 0xFF;
    }
    return v >> 8;
}
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_index_zero_count(struct ActorObj *obj);
void obj_notify_and_show(struct ActorObj *obj);
void obj_lookup_part_move(struct ActorObj *obj);
void obj_setup_part_motion(struct ActorObj *obj);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 mt19937_next(void);
void obj_list_remove(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_anim_a_wait60(struct ActorObj *obj);
void obj_countdown_then_anim_b(struct ActorObj *obj);
void obj_wait_then_spawn_child(struct ActorObj *obj);
void side_walk_obj_timer_spawn_effect(struct ActorObj *obj);
void obj_pose_release_partner(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
s32 menu_cursor_pos(s32 x, s32 y, u32 n);
s32 signed_byte_e4(void *part);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void obj_pose_action_5(struct ActorObj *obj);

void obj_spawn_child_arc(struct ActorObj *obj)
{
    struct ActorObj *child;
    s32 dx;
    s32 dy;
    s32 top;
    s32 k;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        effect_spawn_at(0x2041, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->part, 0x40E3, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100,
                          obj->baseH / 0x100, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        child->byte74 = obj->byte74;
        child->byte75 = obj->byte75;
        top = obj->dir >> 5;
        ((struct ObjBits78 *)&child->dir)->top3 = top;
        child->state = obj_arc_step_clamped;
        child->targetX = obj->param9C;
        child->targetY = obj->paramA0;
        child->targetH = 0;
        child->dx = arc_px(child->posX);
        child->dy = arc_px(child->posY);
        child->dh = arc_px(child->posH);
        dx = child->targetX - ((s16)child->dx << 8);
        dy = child->targetY - ((s16)child->dy << 8);
        MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
        k = 0x2C;
        child->param9C = -0x21;
        child->paramA0 = k;
        child->paramA4 = dx;
        child->paramA8 = dy;
        child->timer = 0;
        set_slot_value(0x13B, -1);
        obj->state = obj_pose_idle;
    }
}

void obj_arc_step_clamped(struct ActorObj *obj)
{
    s32 x;
    s32 d;
    s32 h;
    s32 bias;

    x = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->posX = x;
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    d = (s16)obj->timer - obj->paramA0 / 2;
    h = d * d;
    h = h * obj->param9C;
    bias = ((s16)obj->dh << 8) + 0x4000;
    obj->posH = h + bias;
    if (x <= obj->targetX)
    {
        obj->posX = obj->targetX;
    }
    if (obj->targetY > ((s16)obj->dy << 8))
    {
        if (obj->posY >= obj->targetY)
        {
            obj->posY = obj->targetY;
        }
    }
    else if (obj->posY <= obj->targetY)
    {
        obj->posY = obj->targetY;
    }
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            effect_spawn_at(0x2049, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            set_slot_value(0x65, -1);
            obj->posH = 0;
            obj->timer = 0x20;
            obj->state = obj_timer_finish;
        }
    }
}

void obj_countdown_place_and_setup(struct ActorObj *obj)
{
    struct ActorObj *owner;
    struct ActorPart *part;
    struct ActorPart *ownerPart;
    s32 t;
    s32 scaleA;
    s32 scaleB;
    s32 valueA;
    s32 valueB;
    s32 amount;
    s32 step;
    s32 v3;
    s32 div;
    s32 cursorX;
    s32 signByte;
    s32 setupX;

    if (!(obj->owner->pose->flags & 0x20))
    {
        obj->owner->posX += 0x100;
    }
    obj->param9C--;
    if (obj->param9C <= 0)
    {
        effect_spawn_at(0x202E, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->param9C = 0xA;
    }
    if (obj->pose->flags & 8)
    {
        t = obj->timer - 1;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            set_slot_value(0xD5, -1);
            sprite_set_action(obj, 4, 0, 0);
            effect_spawn_at(0x2059, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            owner = obj->owner;
            scaleA = obj->link->phase;
            part = obj->part;
            scaleB = part->unk10A;
            ownerPart = owner->part;
            valueA = ownerPart->unk104;
            valueB = ownerPart->unk10C;
            amount = scaleA * 0x66;
            amount = arc_px(amount * scaleB);
            v3 = (valueA << 1) + valueA;
            step = v3 + (v3 << 4);
            amount = amount - arc_px(valueB * step);
            amount = arc_px(amount);
            if (amount <= 0)
            {
                amount = 1;
            }
            div = (((amount << 4) - amount) << 2) / 0x64;
            cursorX = menu_cursor_pos(part->cursorX, part->cursorY, div);
            signByte = signed_byte_e4(obj->part);
            setupX = cursorX - obj->part->cursorX;
            popup_digit_spawn(setupX, arc_px(obj->baseX), arc_px(obj->baseY), signByte, 1);
            obj->part->cursorX = cursorX;
            obj->state = obj_pose_action_5;
        }
    }
}

void obj_spawn_child_from_table(struct ActorObj *obj)
{
    struct ActorObj *partner;
    struct ActorObj *child;
    struct ActorPart *part;
    s32 index;

    index = gStepSequenceTable[gUnk_03000E7D + gUnk_03000E7C * 6];
    partner = (struct ActorObj *)(gWorld->partners[index + 6] + 8);
    part = obj->part;
    child = obj_spawn(part, 0x40E8, partner->part->targetX / 0x100,
                      partner->part->targetY / 0x100 + 4, partner->part->targetH / 0x100,
                      ((u32)obj->pose->unk11 << 25) >> 31, 1);
    obj->child = child;
    child->byte74 = obj->byte74;
    child->byte75 = obj->byte75;
    *(u8 *)&child->flags |= 1;
    child->state = 0;
    if (partner->part->targetY <= 0x67FF)
    {
        sprite_set_action(obj, 0xA, 0, 0);
    }
    else
    {
        sprite_set_action(obj, 8, 0, 0);
    }
    obj->timer = 0x12;
    obj->state = obj_timer_table_next;
}

ASM_FUNC("asm/code_0806F354/obj_spawn_child_seq.s", void obj_spawn_child_seq(void))

void obj_arc_to_base_sprite(struct ActorObj *obj)
{
    s32 x;
    s32 d;
    s32 h;
    s32 bias;

    obj->pose->phase = obj->targetH;
    obj->targetH += 0x400;
    x = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->posX = x;
    obj->posY = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    d = (s16)obj->timer - obj->paramA0 / 2;
    h = d * d;
    h = h * obj->param9C;
    bias = ((s16)obj->dh << 8) + 0x3000;
    obj->posH = h + bias;
    obj->timer++;
    if ((s16)obj->timer >= obj->paramA0 / 2)
    {
        if (obj->posH <= 0)
        {
            obj->posH = 0;
            set_slot_value(0x9C, -1);
            effect_spawn_at(0x2069, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            obj->dx = 0;
            obj->pose->phase = 0;
            sprite_set_action(obj, 0, 0x40E6, 0);
            obj->timer = 0xE;
            obj->state = obj_link_then_finish;
        }
    }
}
