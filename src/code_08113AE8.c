#include "global.h"
#include "ram_routines.h"

struct Obj;

struct Sprite
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x03];
    u8 flags11;
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

struct SpriteNode
{
    u8 filler0[0x04];
    struct Sprite *sprite;
    struct SpreadPos *pos;
};

struct World
{
    u8 filler0[0x05];
    u8 unk5;
    u8 filler6[0x06];
    u8 flags;
    u8 filler0D[0x0F];
    u8 *unk1C;
    u8 filler20[0x38];
    struct Scene *scene;
    u8 filler5C[0x04];
    u8 *linkA;
    u8 *linkB;
    u8 filler68[0x18];
    u32 entries[0x20];
};

struct Obj
{
    u8 filler0[0x04];
    void *nodeTake4;
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
    s32 keyZ;
    u8 filler90[0x02];
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

struct ChainNode
{
    struct ChainNode *next;
    s32 x;
    s32 y;
    s32 z;
};

struct SpreadPos
{
    struct ChainNode *head;
    s32 x;
    s32 y;
    s32 z;
    s16 px;
    s16 py;
    s16 pz;
};

struct ListItem
{
    u8 filler0[0x04];
    struct Sprite *sprite;
    u8 filler8[0xD8 - 0x08];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 fillerE4[0x110 - 0xE4];
    u8 flags110;
};

extern struct World *gWorld;
extern const u32 gSlotMotion[];
extern const u16 gModeStartX[];
extern const u16 gModeStartY[];
extern const u16 gUnk_08202218[];
extern const u32 gSlotAngle[];
extern const u8 gRomMotionTable[];
extern s16 gCosineTable[];
extern s16 gSineTable[];

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
void world_link_a_setup(struct Obj *obj, u16 flag);
void world_link_b_setup(struct Obj *obj, u16 flag);
void obj_move_to_scene_target(struct Obj *obj);
void obj_setup_motion_from_scene(struct Obj *obj);
void obj_motion_step_a(struct Obj *obj);
u32 obj_query_contact_step(struct Obj *obj);
u32 query_contact(struct Obj *obj);
u32 place_part_in_slot(s32 slot, s32 mode, s32 a2, s32 a3, s32 s0, s32 s4, s32 s8);
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
void *sprite_node_take(void);
void *sprite_node_alloc(s32 x, s32 y, s32 h);
struct Obj *obj_spawn(struct Scene *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void link_state_clear_idle(struct Obj *obj);
void obj_circle_step(struct Obj *obj);
u32 obj_partner_step(struct Obj *obj, struct Obj *partner);
void obj_select_callback(struct Obj *obj);
void world_links_release(struct Obj *obj);
void obj_step_path(struct Obj *obj);
void obj_place_target_a(struct Obj *obj);
void obj_place_target_b(struct Obj *obj);
u32 obj_claim_partner(struct Obj *obj, struct Obj *partner);
u32 obj_link_to_partner(struct Obj *obj, struct Obj *partner);
void sprite_entry_update(struct Sprite *sprite);
void obj_place_sprite_part(struct Obj *obj);

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
        child->cb5C = (void (*)(struct Obj *))obj_query_contact_step;
        effect_spawn_at(0x28F8, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        obj->callback = obj_stop_anim_clear_cb;
        set_slot_value(0xA0, -1);
    }
}

void obj_setup_motion_from_scene(struct Obj *obj)
{
    struct Scene *scene;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 steps;
    s32 q;
    s32 *phase;

    scene = obj->scene->entry->target;
    obj->keyX = scene->x;
    obj->keyY = scene->y;
    obj->keyZ = 0x800;
    obj->pixX = obj->posX / 0x100;
    obj->pixY = obj->posY / 0x100;
    obj->pixZ = obj->posZ / 0x100;
    dx = obj->keyX - obj->posX;
    dy = obj->keyY - obj->posY;
    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
    steps = dist / 0x1CC;
    q = obj->posZ / (steps * steps);
    phase = (s32 *)&obj->phase;
    if (q <= 1)
    {
        if (q < -1)
            *phase = -q;
        else
            *phase = 2;
    }
    else
    {
        *phase = q;
    }
    obj->slot = dx / 0x100;
    obj->unkB0 = dy / 0x100;
    obj->unkB2 = steps;
    obj->timer = 0;
    obj->unk92 = 0x400;
    obj->level = ((struct WorldEntry *)scene)->level7D - 10;
    obj->callback = obj_motion_step_a;
}

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

void obj_motion_step_b(struct Obj *obj)
{
    obj->dispX = (((s32)obj->slot * obj->timer) << 8) / obj->unkB2 + ((s32)obj->pixX << 8);
    obj->dispY = (((s32)obj->unkB0 * obj->timer) << 8) / obj->unkB2 + ((s32)obj->pixY << 8);
    obj->sprite->unkC -= 0xB4C;
    obj->timer++;
    if (obj->unkB2 <= obj->timer)
    {
        obj->dispX = obj->keyX;
        obj->dispY = obj->keyY;
        obj->callback = obj_place_sprite_part;
    }
}

NONMATCH("asm/code_08113AE8/obj_place_sprite_part.s", void obj_place_sprite_part(struct Obj *obj))
{
    struct ListItem *item;
    struct Obj *part;
    s32 minusOne;
    u8 *base;
    s32 idx;

    base = (u8 *)gWorld;
    idx = obj->unk9C;
    item = ((struct ListItem **)(base + 0x58))[idx];
    part = (struct Obj *)&item->filler8;
    item->sprite->flags11 = (item->flags110 & 0x40) | (item->sprite->flags11 & -0x41);
    part->dispX = item->targetX;
    part->dispY = item->targetY;
    part->dispZ = item->targetZ;
    minusOne = -1;
    sprite_set_action(part, minusOne, minusOne, 0);
    sprite_entry_update(part->sprite);
    effect_spawn_at(0x2900, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
    obj_release(obj);
    obj->callback = NULL;
    set_slot_value(0xAD, minusOne);
}
END_NONMATCH

NONMATCH("asm/code_08113AE8/obj_query_contact_step.s", u32 obj_query_contact_step(struct Obj *obj))
{
    struct Obj *part;
    u8 *table;
    u8 *row;
    s32 index;
    s32 dx;
    s32 dy;
    s32 speed;
    s32 q;
    s32 *phase;
    s32 pa;
    s32 pb;
    s32 pc;
    u32 pd;
    u32 contact;
    u16 mid;
    s32 len;

    contact = query_contact(obj);
    if (contact != 0)
    {
        return contact;
    }
    obj->dispZ = 0x800;
    obj->unk92 = contact;
    index = obj->unk9C;
    if (index > 2)
    {
        table = gWorld->unk1C;
        row = (u8 *)gRomMotionTable + ((((table[1] << 1) + table[1]) << 3) + (index << 2));
        mid = *(u16 *)(table + (index << 2) + 8);
        pa = (s32)((u32)*(u16 *)row << 22) >> 22;
        pb = (s32)(*(u32 *)row << 12) >> 22;
        pc = (s32)((u32)*(u16 *)(row + 2) << 18) >> 22;
        pd = (*(u32 *)row << 1) >> 31;
        part = (struct Obj *)place_part_in_slot(index, mid, pa, pb, pc, pd, contact);
        ((u8 *)part)[0x7E] |= 6;
        gWorld->unk5 -= 1;
        obj->keyX = ((struct ListItem *)part)->targetX;
        obj->keyY = ((struct ListItem *)part)->targetY;
        obj->keyZ = ((struct ListItem *)part)->targetZ;
        obj->pixX = obj->posX / 0x100;
        obj->pixY = obj->posY / 0x100;
        dx = obj->keyX - obj->posX;
        dy = obj->keyY - obj->posY;
        len = ((s32 (*)(s32))gMathRoutines)(dx * dx + dy * dy);
        speed = len / 0x280;
        q = obj->posZ / (speed * speed);
        phase = (s32 *)&obj->phase;
        if (q <= 1)
        {
            if (q < -1)
                *phase = -q;
            else
                *phase = 2;
        }
        else
        {
            *phase = q;
        }
        obj->slot = dx / 0x100;
        obj->unkB0 = dy / 0x100;
        obj->unkB2 = speed;
        obj->timer = 0;
        obj->scene->mode = 1;
        obj->callback = obj_motion_step_b;
    }
    return 0;
}
END_NONMATCH

void obj_stop_anim_clear_cb(struct Obj *obj)
{
    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, -1, -1, 0);
        obj->callback = NULL;
    }
}

void sprite_entry_update(struct Sprite *sprite);
void obj_place_sprite_part(struct Obj *obj);
u32 obj_link_flag_set(struct Obj *obj, struct Obj *other)
{
    s32 frame = 0x10;
    u32 found;
    u8 *scene;

    found = lookup_link_slot(obj, other);
    if (found == 0)
    {
        scene = (u8 *)other->scene;
        *(scene + 0x7E) |= 0x80;
        *(scene + 0x7F) |= 0x20;
        obj->callback = obj_scene_move_or_stop;
        obj->sprite->unk20 = frame;
        return 0;
    }
    return found;
}

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

void world_links_idle_step(struct Obj *obj);

void obj_setup_world_links(struct Obj *obj)
{
    struct Obj *part;
    struct Owner *owner;
    u32 zero;

    if (obj->sprite->mode & 8)
    {
        sprite_set_action(obj, 0x1C, 0, 0);
        part = (struct Obj *)(gWorld->linkA + 8);
        part->scene->mode = 0;
        zero = 0;
        part->owner = obj->owner;
        part->unk9C = obj->unk9C;
        part->unkA0 = obj->unkA0;
        part->unkA4 = obj->unkA4;
        if (((struct SpriteBits *)&part->filler76)->mode == 2)
        {
            world_link_a_setup(part, obj->scene->mode & 1);
        }
        part = (struct Obj *)(gWorld->linkB + 8);
        part->scene->mode = zero;
        owner = obj->owner;
        part->owner = owner;
        part->unk9C = obj->unk9C;
        part->unkA0 = obj->unkA0;
        part->unkA4 = obj->unkA4;
        if (obj->scene->mode == 3)
        {
            part->owner = (struct Owner *)(gWorld->entries[(s16)owner->target->slot + 2] + 8);
        }
        if (((struct SpriteBits *)&part->filler76)->mode == 2)
        {
            world_link_b_setup(part, (obj->scene->mode >> 1) & 1);
        }
        obj->callback = world_links_idle_step;
    }
}

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

NONMATCH("asm/code_08113AE8/obj_spread_chain_to_hold.s", void obj_spread_chain_to_hold(struct Obj *obj))
{
    struct SpreadPos *pos;
    struct SpreadPos *holdPos;
    struct Obj *hold;
    struct ChainNode *node;
    s32 ox, oy, oz;
    s32 dx, dy, dz;
    s32 k;

    hold = obj->chain;
    holdPos = (struct SpreadPos *)&hold->filler0C;
    pos = (struct SpreadPos *)&obj->filler0C;

    hold->level = obj->level - 12;
    ox = pos->x;
    oy = pos->y;
    oz = pos->z;
    dx = holdPos->x + (s32)pos->px * 0x100 - ox;
    dy = holdPos->y + (s32)pos->py * 0x100 - oy;
    dz = holdPos->z + (s32)pos->pz * 0x100 - oz;
    node = pos->head;
    k = 1;
    if (node != NULL)
    {
        do
        {
            node->x = (dx * k) / 4 + ox;
            node->y = (dy * k) / 4 + oy;
            node->z = (dz * k) / 4 + oz;
            node = node->next;
            k++;
        } while (node != NULL);
    }
}
END_NONMATCH

NONMATCH("asm/code_08113AE8/world_link_a_setup.s", void world_link_a_setup(struct Obj *obj, u16 flag))
{
    struct Obj *link;
    struct Obj *part;
    struct SpriteNode *take;
    struct ChainNode *tail;
    struct SpreadPos *pos;
    struct ChainNode *n;
    s32 anim;
    s32 tmp;
    u32 zero;

    link = obj_spawn_linked(obj, 0x4031, 0);
    anim = (obj->unk9C != 0) ? 2 : 3;
    sprite_set_action(link, anim, 0, 0);
    tmp = obj->posX / 0x100 - 0x1C;
    zero = 0;
    link->pixX = tmp;
    link->pixY = obj->posY / 0x100 - 8;
    link->pixZ = obj->posZ / 0x100 + 0x20;
    link->posX = obj->posX - 0x1400;
    link->dispX = link->posX;
    link->posY = obj->posY - 0x800;
    link->dispY = link->posY;
    link->posZ = obj->posZ + 0x2000;
    link->dispZ = link->posZ;
    link->level = obj->level - 1;
    link->callback = NULL;
    ((struct KindBits *)&link->kind)->kind = 3;
    part = obj_spawn(obj->scene, 0x4033, obj->posX / 0x100 - 8, obj->posY / 0x100 - 4, obj->posZ / 0x100 + 0x28, ((u32)obj->sprite->flags11 << 25) >> 31, 1);
    sprite_set_action(part, 1, 0, 0);
    part->filler79[0] |= 1;
    part->callback = obj_spread_chain_to_hold;
    part->chain = obj->chain;
    obj->chain = part;
    pos = (struct SpreadPos *)&part->filler0C;
    pos->px = 8;
    pos->py = -2;
    pos->pz = zero;
    take = sprite_node_take();
    part->nodeTake4 = take;
    take->sprite = part->sprite;
    take->pos = pos;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    pos->head = n;
    tail = n;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    tail->next = n;
    tail = n;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    tail->next = n;
    tail = n;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    tail->next = n;
    n->next = (struct ChainNode *)zero;
    obj->scene->mode = 1;
    if (flag != 0)
    {
        obj->callback = obj_move_to_scene_target;
    }
    else
    {
        obj->callback = link_state_clear_idle;
    }
}
END_NONMATCH

struct Obj *obj_spawn(struct Scene *scene, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void *sprite_node_take(void);
void *sprite_node_alloc(s32 x, s32 y, s32 h);
void link_state_clear_idle(struct Obj *obj);
void obj_spread_chain_to_hold(struct Obj *obj);
NONMATCH("asm/code_08113AE8/world_link_b_setup.s", void world_link_b_setup(struct Obj *obj, u16 flag))
{
    struct Obj *link;
    struct Obj *part;
    struct SpriteNode *take;
    struct ChainNode *tail;
    struct SpreadPos *pos;
    struct ChainNode *n;
    s32 anim;
    s32 tmp;
    u32 zero;

    link = obj_spawn_linked(obj, 0x4032, 0);
    anim = (obj->unk9C != 0) ? 2 : 3;
    sprite_set_action(link, anim, 0, 0);
    tmp = obj->posX / 0x100 + 0x20;
    zero = 0;
    link->pixX = tmp;
    link->pixY = obj->posY / 0x100 + 8;
    link->pixZ = obj->posZ / 0x100 + 0x20;
    link->posX = obj->posX + 0x1800;
    link->dispX = link->posX;
    link->posY = obj->posY + 0x800;
    link->dispY = link->posY;
    link->posZ = obj->posZ + 0x2000;
    link->dispZ = link->posZ;
    link->level = obj->level - 1;
    link->callback = NULL;
    ((struct KindBits *)&link->kind)->kind = 3;
    part = obj_spawn(obj->scene, 0x4033, obj->posX / 0x100 + 8, obj->posY / 0x100 + 4, obj->posZ / 0x100 + 0x28, ((u32)obj->sprite->flags11 << 25) >> 31, 1);
    sprite_set_action(part, 1, 0, 0);
    part->filler79[0] |= 1;
    part->callback = obj_spread_chain_to_hold;
    part->chain = obj->chain;
    obj->chain = part;
    pos = (struct SpreadPos *)&part->filler0C;
    pos->px = 8;
    pos->py = -2;
    pos->pz = zero;
    take = sprite_node_take();
    part->nodeTake4 = take;
    take->sprite = part->sprite;
    take->pos = pos;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    pos->head = n;
    tail = n;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    tail->next = n;
    tail = n;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    tail->next = n;
    tail = n;
    n = sprite_node_alloc(part->dispX, part->dispY, part->dispZ);
    tail->next = n;
    n->next = (struct ChainNode *)zero;
    obj->scene->mode = 1;
    if (flag != 0)
    {
        obj->callback = obj_move_to_scene_target;
    }
    else
    {
        obj->callback = link_state_clear_idle;
    }
}
END_NONMATCH

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

NONMATCH("asm/code_08113AE8/obj_spawn_effect_part.s", void obj_spawn_effect_part(struct Obj *obj))
{
    struct Obj *piece;
    u8 *mode;
    s32 x;
    s32 y;
    s32 z;

    if ((obj->sprite->mode & 8) != 0)
    {
        sprite_set_action(obj, 0x1F, 0, 0);
        ((struct SpriteBits *)&obj->sprite->mode)->mode = 1;
        piece = obj_spawn_linked(obj, 0x4036, 0);
        x = gModeStartX[*(mode = &obj->scene->mode)];
        piece->posX = x << 8;
        piece->dispX = x << 8;
        y = gModeStartY[*mode];
        piece->posY = y << 8;
        piece->dispY = y << 8;
        z = gUnk_08202218[*mode];
        piece->posZ = z << 8;
        piece->dispZ = z << 8;
        piece->callback = obj_start_anim1_timer;
        effect_spawn_at(0x2AED, x, y, z, piece);
        obj->callback = obj_stop_anim;
        sound_stop_song(0xD5);
        set_slot_value(0x118, -1);
    }
}
END_NONMATCH

void obj_release_when_motion_done(struct Obj *obj);
NONMATCH("asm/code_08113AE8/obj_circle_step.s", void obj_circle_step(struct Obj *obj))
{
    s32 *ang;
    s32 *dst;
    s32 angle;
    s32 idx;
    s32 cs;
    s32 sn;
    s32 base;
    s32 next;

    if ((obj->sprite->mode & 6) == 2)
    {
        if ((obj->sprite->mode & 8) != 0)
        {
            obj_release(obj);
            sound_stop_song(0x10C);
            return;
        }
    }
    angle = obj->unk98 + 0x8000;
    ang = &obj->unk98;
    if (angle < 0)
    {
        angle = obj->unk98 + 0x80FF;
    }
    idx = (angle >> 8) & 0xFF;
    cs = gCosineTable[idx];
    obj->dispX = (cs / 0x40) * 0x18 + 0x5000;
    sn = gSineTable[idx];
    obj->dispY = (sn / 0x40) * 0x10 + 0x6C00;
    dst = &obj->unk98;
    base = *ang;
    if (obj->scene->mode != 0)
    {
        next = base + gSlotAngle[obj->slot];
    }
    else
    {
        next = base - gSlotAngle[obj->slot];
    }
    *dst = next;
    if ((u32)(*ang - 0x4000) > 0x8000)
    {
        place_targets(obj, -0x20, obj->posY / 0x100, obj->posZ / 0x100, 0x100);
        set_motion_start(obj, gSlotMotion[obj->slot]);
        obj->callback = obj_release_when_motion_done;
    }
}
END_NONMATCH

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

u32 obj_link_to_partner(struct Obj *obj, struct Obj *partner)
{
    if (partner != partner->scene->main)
    {
        return 1;
    }
    set_slot_value(0x7D, -1);
    resolve_contact_action(partner, obj, 0, 0, (s16)partner->scene->slot == -1 ? 0x8032012 : 0x8032013);
    obj->timerLong = 0x1B58;
    obj->phase = 1;
    obj->callback = NULL;
    obj->cb54 = NULL;
    obj->scene->cb54 = obj_remove_target;
    obj->scene->cb5C = NULL;
    obj->scene->mode = 1;
    sound_stop_song(0xD5);
    sound_stop_song(0x120);
    return 0;
}

NONMATCH("asm/code_08113AE8/obj_claim_partner.s", u32 obj_claim_partner(struct Obj *obj, struct Obj *partner))
{
    struct WorldEntry *entry;

    if (obj->phase == 0)
    {
        partner->owner = (struct Owner *)obj;
        partner->callback = obj_place_target_b;
        obj->scene->mode = 1;
        if ((gWorld->flags & 0x80) != 0)
        {
            entry = (struct WorldEntry *)gWorld->entries[(s16)partner->scene->slot + 2];
            entry->level7D = partner->level + 1;
            entry->cb54 = obj_place_target_a;
            gWorld->flags = gWorld->flags & 0x7F;
        }
        set_slot_value(0x85, -1);
    }
    return -1;
}
END_NONMATCH
