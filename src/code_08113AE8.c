#include "global.h"

struct Obj;

struct Sprite
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x04];
    u8 mode;
    u8 filler13[0x0D];
    u8 unk20;
};

struct Scene
{
    u8 filler0[0x34];
    struct Owner *entry;
    struct Obj *main;
    u8 filler3C[0x40 - 0x3C];
    s32 unk40;
    u8 filler44[0x54 - 0x44];
    void (*cb54)(struct Obj *);
    u8 filler58[0x04];
    void (*cb5C)(struct Obj *);
    u8 filler60[0x1E];
    u8 unk7E;
    u8 unk7F;
    u8 filler80[0x24];
    s32 copyA4;
    s32 copyA8;
    s32 copyAC;
    u8 filler[0x28];
    s32 x;
    s32 y;
    s32 z;
    u8 fillerE4[0x08];
    u16 slot;
    u8 fillerEE[0x08];
    s16 unkF6;
    u8 fillerF8[0x1B];
    u8 mode;
};

struct WorldEntry
{
    u8 filler0[0x54];
    void (*cb54)(struct Obj *);
    u8 filler58[0x25];
    u8 level7D;
};

struct Owner
{
    u8 filler0[0x28];
    struct Scene *target;
    u8 filler2C[0x49];
    u8 level;
    u8 filler76[0x32];
    u32 unkA8;
};

struct KindBits
{
    u8 low : 5;
    u8 kind : 3;
};

struct SpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct World
{
    u8 filler0[0x0C];
    u8 flags;
    u8 filler0D[0x4B];
    struct Scene *scene;
    u8 filler5C[0x04];
    u8 *linkA;
    u8 *linkB;
    u8 filler68[0x18];
    u32 entries[0x20];
};

struct Obj
{
    u8 filler0[0x08];
    struct Sprite *sprite;
    u8 filler0C[0x04];
    s32 dispX;
    s32 dispY;
    s32 dispZ;
    s16 pixX;
    s16 pixY;
    s16 pixZ;
    u8 filler22[0x06];
    struct Scene *scene;
    struct Owner *owner;
    struct Obj *chain;
    u8 filler34[0x04];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x08];
    void (*callback)(struct Obj *);
    void (*cb50)(struct Obj *);
    void (*cb54)(struct Obj *);
    void (*cb58)(struct Obj *);
    void (*cb5C)(struct Obj *);
    void (*cb60)(struct Obj *);
    u8 filler64[0x11];
    u8 level;
    u8 filler76[0x02];
    u8 kind;
    u8 filler79[0x03];
    s16 stepY;
    u8 filler7E[0x06];
    s32 keyX;
    s32 keyY;
    u8 filler8C[0x06];
    u16 unk92;
    u8 filler94[0x04];
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    u32 phase;
    s16 timer;
    s16 slot;
    s16 unkB0;
    s16 unkB2;
    u8 fillerB4[0x04];
    u16 timerLong;
};

extern struct World *gUnk_03000FD8;
extern const u32 gSlotMotion[];
extern const u16 gModeStartX[];
extern const u16 gModeStartY[];
extern const u16 gUnk_08202218[];

void sprite_set_action(struct Obj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
u8 step_motion(struct Obj *obj);
void obj_release(struct Obj *obj);
u32 lookup_link_slot(struct Obj *obj, struct Obj *other);
void obj_set_scene_state(struct Obj *obj);
void obj_timer_anim_6(struct Obj *obj);
void obj_begin_wait(struct Obj *obj);
void obj_scene_move_or_stop(struct Obj *obj);
void effect_spawn_at(u32 effect, s32 x, s32 y, s32 h, struct Obj *obj);
void effect_from_table_obj(struct Obj *obj, u32 code);
void place_targets(struct Obj *obj, s32 x, s32 y, s32 z, s32 extra);
void set_motion_start(struct Obj *obj, u32 value);
void set_step_and_offsets(struct Obj *obj, u32 value, u32 step);
void obj_list_remove(struct Obj *obj);
void obj_list_push_default(struct Obj *obj);
struct Obj *obj_spawn_linked(struct Obj *owner, u32 kind, u32 flags);
void obj_stop_anim_clear_cb(struct Obj *obj);
void obj_start_anim1_timer(struct Obj *obj);
void obj_stop_anim(struct Obj *obj);
void obj_setup_motion_from_scene(struct Obj *obj);
void obj_query_contact_step(struct Obj *obj);
void screen_helpers_obj_show_at(struct Obj *obj);
void partner_link_update(struct Owner *owner, struct Obj *obj, u32 flags);
void obj_clear_when_unflagged(struct Obj *obj);
s32 resolve_contact_action(struct Obj *actor, struct Obj *target, u32 a, u32 b, u32 kind);

void callback_noop_idle(struct Obj *obj);
void obj_anim_slot_15_16(struct Obj *obj);
void obj_timer_countdown_anim17(struct Obj *obj);
void obj_timer_show(struct Obj *obj);
void obj_remove_target(struct Obj *obj);
void obj_begin_effect(struct Obj *obj);
void obj_wait_then_callback(struct Obj *obj);
void obj_wait_target_anim6(struct Obj *obj);
void obj_close_if_free(struct Obj *obj);
void obj_begin_when_free(struct Obj *obj);
void obj_reset_when_free(struct Obj *obj);
void obj_spawn_child(struct Obj *obj);
void obj_spawn_effect_part(struct Obj *obj);
void obj_circle_step(struct Obj *obj);
u32 obj_partner_step(struct Obj *obj, struct Obj *partner);
void obj_select_callback(struct Obj *obj);
void world_links_release(struct Obj *obj);
void obj_step_path(struct Obj *obj);
void obj_place_target_a(struct Obj *obj);
void obj_place_target_b(struct Obj *obj);
u32 obj_claim_partner(struct Obj *obj, struct Obj *partner);
u32 obj_link_to_partner(struct Obj *obj, struct Obj *partner);

void obj_spawn_child(struct Obj *obj)
{
    struct Obj *child;

    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, 0x18, 0, 0);
        child = obj_spawn_linked(obj, 0x403E, 0);
        child->posX = child->posX + 0xFFFFF400;
        child->dispX = child->posX;
        child->posY = child->posY + 0x200;
        child->dispY = child->posY;
        child->posZ = child->posZ + 0x3000;
        child->dispZ = child->posZ;
        child->stepY = 0x100;
        child->callback = obj_setup_motion_from_scene;
        ((struct KindBits *)&child->kind)->kind = 2;
        obj_list_push_default(child);
        child->cb5C = obj_query_contact_step;
        effect_spawn_at(0x28F8, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->callback = obj_stop_anim_clear_cb;
        set_slot_value(0xA0, -1);
    }
}

ASM_FUNC("asm/code_08113AE8/obj_setup_motion_from_scene.s", void obj_setup_motion_from_scene(struct Obj *obj))

void obj_motion_step_a(struct Obj *obj)
{
    obj->dispX = (((s32)obj->slot * obj->timer) << 8) / obj->unkB2 + ((s32)obj->pixX << 8);
    obj->dispY = (((s32)obj->unkB0 * obj->timer) << 8) / obj->unkB2 + ((s32)obj->pixY << 8);
    obj->dispZ += *(s16 *)&obj->unk92;
    obj->unk92 = obj->unk92 - 0xCC;
    obj->sprite->unkC += 0xB4C;
    obj->timer += 1;
    if (obj->dispZ <= 0x800)
    {
        obj->dispZ = 0x800;
        *(s16 *)&obj->unk92 = (-((*(s16 *)&obj->unk92) << 8)) / 0x180;
        set_slot_value(0x2F, -1);
    }
    if (obj->dispX / 0x100 <= -0x20)
    {
        obj_release(obj);
    }
}

ASM_FUNC("asm/code_08113AE8/obj_motion_step_b.s", void obj_motion_step_b(void))

ASM_FUNC("asm/code_08113AE8/obj_place_sprite_part.s", void obj_place_sprite_part(void))

ASM_FUNC("asm/code_08113AE8/obj_query_contact_step.s", void obj_query_contact_step(struct Obj *obj))

void obj_stop_anim_clear_cb(struct Obj *obj)
{
    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->callback = NULL;
    }
}

ASM_FUNC("asm/code_08113AE8/obj_link_flag_set.s", void obj_link_flag_set(void))

void obj_phase_wait_idle(struct Obj *obj)
{
    u32 done;

    if (obj->phase == 0)
    {
        effect_spawn_at(0x297A, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->phase = 10;
    }
    obj->phase -= 1;
    done = step_motion(obj);
    if (done == 0)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->callback = NULL;
    }
}

void obj_anim_mode5_timer3(struct Obj *obj)
{
    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, obj->scene->mode + 5, 0, 0);
        obj->timer = 3;
        obj->callback = obj_timer_anim_6;
        set_slot_value(0x8A, -1);
    }
}

u32 sprite_state_chain_f(struct Obj *obj)
{
    obj->callback = obj_anim_slot_15_16;
    obj->scene->mode = 0;
    return 1;
}

u32 sprite_state_scene_mode4(struct Obj *obj)
{
    obj->callback = obj_set_scene_state;
    obj->scene->mode = 4;
    return 1;
}

u32 sprite_state_scene_mode0(struct Obj *obj)
{
    obj->callback = obj_set_scene_state;
    obj->scene->mode = 0;
    return 1;
}

void obj_anim_slot_15_16(struct Obj *obj)
{
    u32 slot = (u16)~obj->owner->target->slot;
    u32 anim;

    anim = 0x15;
    if (slot != 0)
    {
        anim = 0x16;
    }
    sprite_set_action(obj, anim, 0, 0);
    obj->timer = 2;
    obj->callback = obj_timer_countdown_anim17;
}

void obj_timer_countdown_anim17(struct Obj *obj)
{
    u16 timer;

    if (obj->sprite->mode & 8)
    {
        timer = obj->timer;
        if (obj->timer != 0)
        {
            obj->timer = timer - 1;
        }
        else
        {
            sprite_set_action(obj, 0x17, 0, 0);
            ((struct SpriteBits *)&obj->sprite->mode)->mode = 1;
            obj->callback = obj_spawn_child;
        }
    }
}

ASM_FUNC("asm/code_08113AE8/obj_setup_world_links.s", void obj_setup_world_links(void))

void obj_timer_release_hold(struct Obj *obj)
{
    struct Owner *owner;
    u16 timer;

    obj->scene->mode = 1;
    timer = obj->timer;
    if (obj->timer != 0)
    {
        obj->timer = timer - 1;
    }
    else
    {
        obj->callback = obj_wait_target_anim6;
        obj = obj->chain->chain;
        owner = obj->owner;
        obj->level = owner->level - 1;
        sprite_set_action(obj, 5, 0, 0);
        ((struct SpriteBits *)&obj->sprite->mode)->mode = 1;
        effect_spawn_at(0x2ABF, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        effect_from_table_obj(obj, 0x18CE);
        obj->cb58 = (void (*)(struct Obj *))obj_claim_partner;
        obj->cb60 = (void (*)(struct Obj *))obj_link_to_partner;
        sound_stop_song(0x120);
    }
}

ASM_FUNC("asm/code_08113AE8/obj_spread_chain_to_hold.s", void obj_spread_chain_to_hold(void))

ASM_FUNC("asm/code_08113AE8/world_link_a_setup.s", void world_link_a_setup(void))

ASM_FUNC("asm/code_08113AE8/world_link_b_setup.s", void world_link_b_setup(void))

void obj_begin_effect(struct Obj *obj)
{
    struct Scene *scene;
    s32 mode;

    sprite_set_action(obj, 0x1E, 0, 0);
    ((struct SpriteBits *)&obj->sprite->mode)->mode = 1;
    obj->slot = 0;
    scene = obj->scene;
    mode = 0;
    if (obj->owner->target->y > 0x6C00)
    {
        mode = 1;
    }
    scene->mode = mode;
    effect_spawn_at(0x2AE3, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    obj->callback = obj_spawn_effect_part;
    set_slot_value(0xD5, -1);
}

ASM_FUNC("asm/code_08113AE8/obj_spawn_effect_part.s", void obj_spawn_effect_part(struct Obj *obj))

ASM_FUNC("asm/code_08113AE8/obj_circle_step.s", void obj_circle_step(struct Obj *obj))

void obj_finish_motion(struct Obj *obj);

u32 obj_partner_step(struct Obj *obj, struct Obj *partner)
{
    if (partner != partner->scene->main)
    {
        return 1;
    }
    if (obj->posX < partner->scene->unk40)
    {
        return -1;
    }
    if (obj->posX < obj->dispX)
    {
        return -1;
    }
    place_targets(obj, 0x50, obj->posY / 0x100, obj->posZ / 0x100, 0x100);
    set_motion_start(obj, gSlotMotion[obj->slot]);
    obj->scene->mode ^= 1;
    obj->callback = obj_finish_motion;
    ((u16 *)&obj->slot)[0] = ((u16 *)&obj->slot)[0] + 1;
    if (obj->slot >= obj->unk9C)
    {
        sprite_set_action(obj, 3, 0, 0);
        ((struct SpriteBits *)&obj->sprite->mode)->mode = 1;
    }
    effect_spawn_at(0x2AFB, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    set_slot_value(0x119, -1);
    return -1;
}

void obj_finish_motion(struct Obj *obj)
{
    u32 *field;
    s32 mode;
    u32 value;

    if ((obj->sprite->mode & 6) == 2)
    {
        if ((obj->sprite->mode & 8) != 0)
        {
            obj_release(obj);
            sound_stop_song(0x10C);
            return;
        }
    }
    if (step_motion(obj) == 0)
    {
        field = &obj->unk98;
        mode = obj->scene->mode;
        value = 0xC000;
        if (mode != 0)
        {
            value = 0x4000;
        }
        *field = value;
        obj->callback = obj_circle_step;
    }
}

void obj_release_when_motion_done(struct Obj *obj)
{
    if (step_motion(obj) == 0)
    {
        obj_release(obj);
        sound_stop_song(0x10C);
    }
}

void obj_start_anim1_timer(struct Obj *obj)
{
    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->timer = 0x28;
        obj->callback = obj_timer_show;
    }
}

void obj_stop_anim(struct Obj *obj)
{
    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->callback = NULL;
    }
}

ASM_FUNC("asm/code_08113AE8/obj_link_to_partner.s", u32 obj_link_to_partner(struct Obj *obj, struct Obj *partner))

ASM_FUNC("asm/code_08113AE8/obj_claim_partner.s", u32 obj_claim_partner(struct Obj *obj, struct Obj *partner))
