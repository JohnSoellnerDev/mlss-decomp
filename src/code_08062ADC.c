#include "global.h"
#include "ram_routines.h"

struct ActorDesc
{
    u8 filler0[0x04];
    struct ActorPose *pose;
    void *node;
    u8 order;
};

struct ChainNode
{
    struct ChainNode *next;
    s32 x;
    s32 y;
    s32 h;
};

struct Lo5Bits
{
    u8 lo5 : 5;
    u8 hi3 : 3;
};

struct OamAffineParams
{
    u8 filler0[0x6];
    vu16 pa : 16;
    u8 filler8[0x6];
    vu16 pb : 16;
    u8 filler10[0x6];
    vu16 pc : 16;
    u8 filler18[0x6];
    vu16 pd : 16;
};

struct ActorPose
{
    u8 filler0[0x0C];
    u16 counter0C;
    u8 filler0E[0x11 - 0x0E];
    u8 flip;
    u8 flags;
};

struct ActorObj
{
    u8 filler0[0x04];
    struct ActorDesc *desc;
    struct ActorPose *pose;
    struct ChainNode *chainHead;
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
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
    u8 unk76;
    u8 flags77;
    u8 flags78;
    u8 flags79;
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

/* The part holds a nested object at +0x08 with the same layout as struct ActorObj. */
struct ActorPart
{
    u8 filler0[0x08];
    struct ActorObj sub8;
    u8 filler0BC[0xD8 - 0xBC];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
};

static inline void set_rebuild_flag(struct ActorObj *obj)
{
    u8 *flags = &obj->flags79;

    *flags = *flags | 0x20;
}

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

struct ActionBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct PoseBits12
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

// The child takes over bits 5 and 6 of the flag byte at +0x7F of its part.
static inline void copy_part_flags(struct ActorObj *child, struct ActorPart *part)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)&part->sub8)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)&part->sub8)->bit5;
}

// The spawned child takes over the kind and two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child, u32 kind)
{
    ((struct Flags78Bits *)&child->flags78)->kind = kind >> 5;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

void obj_wait_ready_action3(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj);
u32 is_state_busy(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_wait_ready_action2_next(struct ActorObj *obj);
void obj_action3_place_sprite(struct ActorObj *obj);
void obj_begin_chain(struct ActorObj *obj);
void obj_motion_fast_start(struct ActorObj *obj);
void obj_free_place_remove(struct ActorObj *obj);
void obj_free_clear_remove(struct ActorObj *obj);
void chain_obj_spawn_child_launch(struct ActorObj *obj);
void obj_begin_move_fixed_x(struct ActorObj *obj);
void obj_notify_action5_or_7(struct ActorObj *obj);
void obj_timer_action6_or_8(struct ActorObj *obj);
void chain_obj_index_wait_action4(struct ActorObj *obj);
void obj_pose_action_b(struct ActorObj *obj);
void chain_obj_wait_ready_action2(struct ActorObj *obj);
void obj_begin_linked_wait(struct ActorObj *obj);
void obj_timer_action_a_idle(struct ActorObj *obj);
void obj_timer_action5(struct ActorObj *obj);
void obj_timer_action6(struct ActorObj *obj);
void obj_timer_action6_spawn(struct ActorObj *obj);
void obj_spawn_child_11c(struct ActorObj *obj);
void obj_wait_ready_place(struct ActorObj *obj);
void obj_timer_place_a(struct ActorObj *obj);
void obj_action_e_idle(struct ActorObj *obj);
void obj_place_and_notify(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void obj_index_wait_action5(struct ActorObj *obj);
void obj_motion_slow_start(struct ActorObj *obj);
void obj_timer_effect_b(struct ActorObj *obj);
void obj_action7_wait(struct ActorObj *obj);
void obj_arc_motion_step(struct ActorObj *obj);
void obj_counter_release(struct ActorObj *obj);
void obj_index_action5(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_alloc_anim(struct ActorObj *obj);
u32 obj_query_part3(struct ActorObj *obj);
u32 obj_unlink_end(struct ActorObj *obj);
void obj_idle_action8(struct ActorObj *obj);
s32 chain_metric(struct ActorObj *obj);
void obj_list_push_arg(struct ActorObj *obj, u32 arg);
void obj_chain_cull(struct ActorObj *obj);
void chain_link_end(struct ActorObj *obj);
void chain_link_step(struct ActorObj *obj);
u32 obj_lookup_action1(struct ActorObj *obj, struct ActorObj *partner);
void obj_action_a_timer(struct ActorObj *obj);
void obj_sync_target_place(struct ActorObj *obj);
extern u8 gSys[];
u8 poll_spline_path(u32 buf, s32 *x, s32 *h);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
extern s32 gUnk_03000E40;
extern s32 gUnk_03000E44;
void obj_stop(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
void heap_free_b(void *p);
extern u32 gUnk_03000E18;
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
void set_slot_value(u32 id, s32 value);
void start_motion(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, struct ActorObj *partner);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, u32 a, u32 b, u32 code);
u32 mt19937_next(void);
struct ChainNode *sprite_node_alloc(s32 x, s32 y, s32 h);
struct ActorDesc *sprite_node_take(void);
void obj_chain_wait(struct ActorObj *obj);
extern u8 *gWorld;

void obj_wait_timer_sprite_notify(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t++;
    obj->timer = t;
    if ((s16)t > 15)
    {
        effect_spawn_at(0xED2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0;
    }
    if (obj->index == 0)
    {
        sound_stop_song(0x81);
        obj->timer = 8;
        obj->state = obj_timer_action_a_idle;
    }
}

u32 obj_try_link_slot(struct ActorObj *obj, struct ActorObj *partner)
{
    u32 result = lookup_slot(obj, partner);

    if (result != 0)
    {
        return result;
    }
    sound_stop_song(0x81);
    resolve_contact_action(partner, obj, obj->posX / 0x100, obj->posH / 0x100,
                           partner->part->slot != -1 ? 0x13 : 0x12);
    obj->state = wait_for_index;
    return 0;
}

u32 obj_release_link_idle(struct ActorObj *obj, struct ActorObj *partner)
{
    sound_stop_song(0x81);
    partner_link_update(obj, partner, 0xFF);
    obj->state = wait_for_index;
    return 0;
}

void obj_timer_action_a_idle(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->state = wait_for_index;
    }
}

u32 obj_param_from_slot_c(struct ActorObj *obj)
{
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = obj_wait_ready_action2_next;
    return 1;
}

void obj_wait_ready_action2_next(struct ActorObj *obj)
{
    if (is_state_busy(obj) == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_action3_place_sprite;
    }
}

void obj_action3_place_sprite(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        effect_spawn_at(0xE92, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_begin_move_fixed_x;
        set_slot_value(0x127, -1);
    }
}

void obj_begin_move_fixed_x(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->targetX = 0x6800;
        obj->targetY = obj->paramA0 << 8;
        obj->targetH = obj->baseH;
        set_rebuild_flag(obj);
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_notify_action5_or_7;
        set_slot_value(0x81, -1);
    }
}

void obj_notify_action5_or_7(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x81);
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 7, 0, 0);
        }
        obj->timer = 4;
        obj->state = obj_timer_action6_or_8;
    }
}

void obj_timer_action6_or_8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) < 0)
        {
            if (obj->param9C == 0)
            {
                sprite_set_action(obj, 6, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 8, 0, 0);
            }
            obj->state = obj_begin_linked_wait;
        }
    }
}

ASM_FUNC("asm/code_08062ADC/obj_begin_chain.s", void obj_begin_chain(struct ActorObj *obj))

ASM_FUNC("asm/code_08062ADC/chain_metric.s", s32 chain_metric(struct ActorObj *obj))

void chain_link_step(struct ActorObj *obj)
{
    struct ActorPart *part;
    struct ActorObj *tpl;
    struct ActorObj *child;
    s32 length;

    obj->dx += 0x280;
    length = chain_metric(obj);
    if (((s16)obj->dx << 1) < length)
        return;
    part = obj->part;
    tpl = (struct ActorObj *)((u8 *)part + 8);
    child = obj_spawn(part, 0x40D9, part->sub8.targetX / 0x100, part->sub8.targetY / 0x100,
                      part->sub8.targetH / 0x100, 0, 1);
    sprite_set_action(child, 1, 0, 0);
    child->tag = 0xFF;
    ((struct Flags79Bit0 *)&child->flags79)->bit0 = 1;
    ((struct ActionBits *)&child->unk76)->mode = 3;
    child->owner = tpl->owner;
    child->unk6C = tpl->unk6C;
    obj_list_push_arg(child, 0);
    copy_part_flags(child, part);
    ((struct PoseBits12 *)&child->pose->flags)->mode = 0;
    child->state = obj_chain_cull;
    child->timer = 6;
    obj->child = child;
    obj->timer = 0x10;
    obj->state = chain_link_end;
}

void chain_link_end(struct ActorObj *obj)
{
    u8 *base;
    u8 *oamBase;
    u32 bank;
    u32 index;
    u8 *oam;
    s32 dirX;
    s32 dirY;

    chain_metric(obj);
    obj->timer--;
    if ((s16)obj->timer <= 0x12)
    {
        dirX = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->part->sub8.dx * 18, (s16)obj->timer);
        dirY = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->part->sub8.dy * 18, (s16)obj->timer);
        base = gSys;
        bank = (((u32)*(base + 0x888) << 28) >> 31) << 10;
        index = ((u32)obj->desc->order << 27) >> 22;
        oamBase = base + 0x80;
        oam = (u8 *)(bank + (index + (u32)oamBase));
        ((struct OamAffineParams *)oam)->pa = dirX;
        ((struct OamAffineParams *)oam)->pb = dirY;
        ((struct OamAffineParams *)oam)->pc = -dirY;
        ((struct OamAffineParams *)oam)->pd = dirX;
        if ((s16)obj->timer == 1)
        {
            obj->part->sub8.timer = 0;
            obj_release(obj);
        }
    }
}
