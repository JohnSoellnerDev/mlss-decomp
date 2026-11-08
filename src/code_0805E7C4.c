#include "global.h"

struct MapPose
{
    u8 filler0[0x4];
    u16 unk4;
    u16 unk6;
    u8 filler8[0x11 - 0x8];
    u8 flip;
    u8 flags;
};

struct MapPart
{
    u8 filler0[0xB4];
    s16 flagB4;
    u8 fillerB6[0xD8 - 0xB6];
    s32 targetX;
    s32 targetY;
    s32 targetH;
};

struct AreaEntry
{
    u8 area;
    u8 unk1;
    u8 filler2[22];
};

struct Block24
{
    u8 bytes[24];
};

struct ChainNode
{
    struct ChainNode *next;
    struct MapPose *pose;
    struct ChainNode **slot;
};

struct Bits77
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct MapObj
{
    u8 filler0[0x04];
    struct ChainNode *node4;
    struct MapPose *pose;
    struct ChainNode *chain0C;
    s32 posX;
    s32 posY;
    s32 posH;
    u16 loadPhase;
    u16 loadSel;
    u8 filler20[0x28 - 0x20];
    struct MapPart *part;
    struct MapObj *owner;
    struct MapObj *sprite;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct MapObj *);
    u8 filler50[0x68 - 0x50];
    void (*handler68)(struct MapObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 filler75[0x77 - 0x75];
    u8 bits77;
    u8 mode78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    void *bgMap;
    void *unk98;
    s32 param9C;
    s32 radius;
    s32 wait;
    s32 angle;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

void map_obj_begin_move(struct MapObj *obj);
void map_obj_wait_index(struct MapObj *obj);
void map_obj_wait_ready(struct MapObj *obj);
void map_obj_place_and_count(struct MapObj *obj);
void map_obj_count_down(struct MapObj *obj);
void map_view_map_obj_setup_sprites(struct MapObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct MapObj *owner);
void sprite_set_action(struct MapObj *obj, u32 action, u32 arg, u32 extra);
void start_motion(struct MapObj *obj);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
s32 is_state_busy(void);
void wait_for_index(struct MapObj *obj);
void load_layer_columns(struct MapObj *obj);
void load_layer12_tiles(struct MapObj *obj, u32 unused);
void draw_layer_metatiles(struct MapObj *obj);
void map_view_load_map_state(struct MapObj *obj, u16 sel);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void task_remove(void *node, u32 kind);
void heap_free_a(void *p);
u32 dma_cancel_request(u32 dst);
void obj_release(struct MapObj *obj);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void slide_out_step(struct MapObj *obj);
void map_obj_drift_y(struct MapObj *obj);
void map_obj_orbit_dy_reset(struct MapObj *obj);
void map_obj_orbit_dh_reset(struct MapObj *obj);
void map_obj_fall(struct MapObj *obj);
void map_obj_orbit_dy_down(struct MapObj *obj);
void map_obj_orbit_dh_down(struct MapObj *obj);
void map_obj_slide_in_x(struct MapObj *obj);
void map_obj_orbit_wait(struct MapObj *obj);
u32 arc_tan2(s32 x, s32 y);
void map_obj_drift_x(struct MapObj *obj);
void map_obj_orbit_dh_hold(struct MapObj *obj);
void map_obj_slide_by_angle(struct MapObj *obj);
void map_obj_slide_drop(struct MapObj *obj);
void map_obj_orbit_dh_turn(struct MapObj *obj);
void map_obj_aim_down(struct MapObj *obj);
void obj_orbit_place(struct MapObj *obj, s32 x, s32 y, s32 h, s32 amp, s32 timer, s32 param);
void obj_set_sprite_offsets(struct MapObj *obj, s32 x, s32 y, s32 h, s32 amp);

extern const struct AreaEntry gAreaEntryTable[];
extern u8 gUnk_03000DD0[];
extern const u32 gObjectTilesets[];
extern const u8 gMapObjLoaderClass[];
extern const s16 gCosineTable[];
extern const s16 gSineTable[];
extern u8 gUnk_03000E14;
extern u8 gUnk_03000E15;

struct MapObj *obj_spawn(struct MapPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
struct ChainNode *sprite_node_take(void);
struct ChainNode *sprite_node_alloc(s32 x, s32 y, s32 h);
void obj_list_push_default(struct MapObj *obj);
void on_arrive(struct MapObj *obj);
void map_obj_slide_left(struct MapObj *obj);
void map_obj_slide_fast_left(struct MapObj *obj);
void map_obj_orbit_out(struct MapObj *obj);
void map_obj_place_from_offsets(struct MapObj *obj);
void map_obj_settle(struct MapObj *obj);

static inline void map_obj_release_chain(struct MapObj *obj)
{
    struct MapObj *sprite = obj->sprite;
    struct MapObj *sub = sprite->sprite;

    sub->param9C = 1;
    sub->sprite->param9C = 1;
    obj_release(sprite);
    obj_release(obj);
}

NONMATCH("asm/code_0805E7C4/map_obj_action5_spawn.s", void map_obj_action5_spawn(struct MapObj *obj))
{
    struct MapObj *child;
    struct MapObj *mid;
    struct MapObj *tip;
    struct MapObj *tip2;
    struct ChainNode *first;
    struct ChainNode *tail;
    struct ChainNode *next;
    struct Bits77 *srcBits;
    u8 *tagAddr;
    s32 count;
    s32 param;
    s32 t;
    s32 dx;
    s32 dy;
    u32 angle;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 5, 0, 0);
        effect_spawn_at(0x12E2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->part, 0x412E, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                          obj->baseH / 0x100 + 0x30, ((u32)obj->pose->flip << 25) >> 31, 1);
        sprite_set_action(child, 1, 0, 0);
        gUnk_03000E14 = 1;
        gUnk_03000E15 = 0;
        obj->sprite = child;
        child->flags79 |= 1;
        srcBits = (struct Bits77 *)&obj->bits77;
        ((struct Bits77 *)&child->bits77)->bit6 = srcBits->bit6;
        ((struct Bits77 *)&child->bits77)->bit5 = srcBits->bit5;
        child->handler68 = on_arrive;
        child->link = obj->link;
        child->owner = obj->owner;
        tagAddr = &obj->tag;
        child->tag = *tagAddr;
        param = obj->param9C;

        switch (param)
        {
        case 0:
            child->targetX = obj->baseX - 0x1000;
            child->targetY = obj->baseY + 0x200;
            child->targetH = obj->baseH + 0x3000;
            child->timer = 0;
            child->param9C = 0;
            t = 0x4000 - child->targetX;
            if (t < 0)
            {
                t += 0xFF;
            }
            dx = (t << 8) >> 16;
            t = 0x4000 - child->targetY;
            if (t < 0)
            {
                t += 0xFF;
            }
            dy = (t << 8) >> 16;
            angle = arc_tan2(dx, dy);
            child->angle = (angle << 16) >> 24;
            child->radius = 0x200;
            child->state = map_obj_orbit_out;
            break;
        case 1:
            child->dx = obj->baseX / 0x100 - 0x10;
            child->dy = obj->baseY / 0x100 + 2;
            child->dh = obj->baseH / 0x100 + 0x30;
            child->targetX = (s32)child->dx << 8;
            child->targetY = (s32)child->dy << 8;
            child->targetH = (s32)child->dh << 8;
            child->radius = obj->owner->part->targetX + 0x3000;
            child->wait = obj->owner->part->targetY;
            child->param9C = 0xC;
            child->angle = 0x180;
            child->timer = 0;
            child->state = map_obj_slide_left;
            break;
        case 2:
            child->dx = obj->baseX / 0x100 - 0x10;
            child->dy = obj->baseY / 0x100 + 2;
            child->dh = obj->baseH / 0x100 + 0x30;
            child->targetX = (s32)child->dx << 8;
            child->targetY = (s32)child->dy << 8;
            child->targetH = (s32)child->dh << 8;
            child->radius = obj->owner->part->targetX + 0x2800;
            child->wait = obj->owner->part->targetY;
            child->param9C = 0x1000;
            child->angle = 0x160;
            child->timer = 0;
            child->state = map_obj_slide_fast_left;
            break;
        }

        child->node4 = sprite_node_take();
        child->node4->pose = child->pose;
        child->node4->slot = &child->chain0C;

        first = sprite_node_alloc(child->posX, child->posY, child->posH);
        child->chain0C = first;
        tail = first;
        tail->next = next = sprite_node_alloc(child->posX, child->posY, child->posH);
        tail = next;
        tail->next = next = sprite_node_alloc(child->posX, child->posY, child->posH);
        tail = next;
        count = 7;
        do
        {
            tail->next = next = sprite_node_alloc(child->posX, child->posY, child->posH);
            tail = next;
            tail->next = next = sprite_node_alloc(child->posX, child->posY, child->posH);
            tail = next;
            tail->next = next = sprite_node_alloc(child->posX, child->posY, child->posH);
            tail = next;
            tail->next = next = sprite_node_alloc(child->posX, child->posY, child->posH);
            tail = next;
            count -= 4;
        } while (count >= 0);
        tail->next = 0;

        mid = obj_spawn(child->part, 0x700C, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2, 0,
                        ((u32)obj->pose->flip << 25) >> 31, 1);
        child->sprite = mid;
        mid->flags79 |= 1;
        mid->tag = *tagAddr;
        mid->filler75[0] = 0x7F;
        mid->state = 0;

        tip = obj_spawn(mid->part, 0x412F, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                        obj->baseH / 0x100 + 0x30, ((u32)obj->pose->flip << 25) >> 31, 1);
        mid->sprite = tip;
        tip->flags79 |= 1;
        ((struct Bits77 *)&tip->bits77)->bit6 = srcBits->bit6;
        ((struct Bits77 *)&tip->bits77)->bit5 = srcBits->bit5;
        tip->handler68 = on_arrive;
        tip->link = obj->link;
        tip->owner = obj->owner;
        obj_list_push_default(tip);
        tip->tag = *tagAddr;
        tip->param9C = 0;
        tip->dx = obj->baseX / 0x100 - 0x10;
        tip->dy = obj->baseY / 0x100 + 2;
        tip->dh = obj->baseH / 0x100 + 0x30;
        tip->timer = 0;
        tip->state = map_obj_place_from_offsets;

        tip2 = obj_spawn(tip->part, 0x412F, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                         obj->baseH / 0x100 + 0x30, ((u32)obj->pose->flip << 25) >> 31, 1);
        tip->sprite = tip2;
        tip2->flags79 |= 1;
        ((struct Bits77 *)&tip2->bits77)->bit6 = srcBits->bit6;
        ((struct Bits77 *)&tip2->bits77)->bit5 = srcBits->bit5;
        tip2->handler68 = on_arrive;
        tip2->link = obj->link;
        tip2->owner = obj->owner;
        obj_list_push_default(tip2);
        tip2->tag = *tagAddr;
        tip2->param9C = 0;
        tip2->dx = obj->baseX / 0x100 - 0x10;
        tip2->dy = obj->baseY / 0x100 + 2;
        tip2->dh = obj->baseH / 0x100 + 0x30;
        tip2->timer = 1;
        tip2->state = map_obj_place_from_offsets;

        obj->state = map_obj_settle;
        set_slot_value(0x110, -1);
    }
}
END_NONMATCH

void map_obj_orbit_out(struct MapObj *obj)
{
    s32 *ang;
    const s16 *ct;
    s32 step;
    s32 mask;

    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    step = obj->radius / 0x100;
    ct = gCosineTable;
    ang = &obj->angle;
    mask = 0xFF;
    obj->targetX += step * (ct[(u8)*ang] / 0x40);
    step = obj->radius / 0x100;
    obj->targetY += step * (gSineTable[*ang & mask] / 0x40);
    if (obj->param9C <= 0x37)
    {
        obj->param9C++;
    }
    obj->timer = (obj->timer + 2) & mask;
    if (obj->targetX <= 0x4000)
    {
        if (obj->targetY <= 0x4000)
        {
            obj->targetX = 0x4000;
            obj->targetY = 0x4000;
            obj->wait = 0;
            obj->state = map_obj_aim_down;
        }
    }
}

void map_obj_aim_down(struct MapObj *obj)
{
    s32 x;
    s32 y;

    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    if (obj->param9C <= 0x37)
    {
        obj->param9C++;
    }
    obj->wait--;
    if (obj->wait <= 0)
    {
        obj->state = map_obj_orbit_wait;
        x = (s16)((-obj->targetX) / 0x100);
        y = (s16)((0xF000 - obj->targetY) / 0x100);
        {
            u32 a = arc_tan2(x, y);
            obj->angle = (a << 16) >> 24;
        }
        obj->radius = 0x180;
        obj->timer = 0;
    }
}

void map_obj_orbit_wait(struct MapObj *obj)
{
    s32 t;

    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    t = ((obj->timer + 4) & 0xFF) - 1;
    obj->timer = t;
    if (t <= 0)
    {
        obj->state = map_obj_fall;
        set_slot_value(0x10F, -1);
    }
}

NONMATCH("asm/code_0805E7C4/map_obj_fall.s", void map_obj_fall(struct MapObj *obj))
{
    s32 *ang;
    const s16 *ct;
    s32 step;
    s32 mask;

    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    step = obj->radius / 0x100;
    ct = gCosineTable;
    ang = &obj->angle;
    mask = 0xFF;
    obj->targetX += step * (ct[(u8)*ang] / 0x40);
    step = obj->radius / 0x100;
    obj->targetY += step * (gSineTable[*ang & mask] / 0x40);
    obj->timer = (obj->timer + 4) & mask;
    if (obj->targetY > 0xF000)
    {
        map_obj_release_chain(obj);
    }
}
END_NONMATCH

void map_obj_slide_left(struct MapObj *obj)
{
    s32 dxs;
    s32 dys;
    s32 a;
    s32 b;
    s32 t;
    s32 q;
    s32 *ty;

    obj->targetX -= 0x180;
    dxs = (s16)obj->dx << 8;
    a = dxs - obj->targetX;
    b = dxs - obj->radius;
    dys = (s16)obj->dy << 8;
    t = dys - obj->wait;
    ty = &obj->targetY;
    q = (t * a) / b;
    *ty = dys - q;
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    if (obj->targetX < obj->radius)
    {
        obj->radius -= 0x30;
        obj->dy = 0;
        obj->state = map_obj_orbit_dy_down;
    }
}

void map_obj_orbit_dy_down(struct MapObj *obj)
{
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    obj->dy--;
    if (obj->dy <= 0)
    {
        obj->dy = 0;
        obj->state = map_obj_orbit_dy_reset;
    }
}

void map_obj_orbit_dy_reset(struct MapObj *obj)
{
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 4) & 0xFF;
    obj->dy--;
    if (obj->dy <= 0)
    {
        obj->dy = obj->wait / 0x100;
        obj->state = map_obj_slide_by_angle;
        set_slot_value(0x10F, -1);
    }
}

NONMATCH("asm/code_0805E7C4/map_obj_slide_by_angle.s", void map_obj_slide_by_angle(struct MapObj *obj))
{
    s32 dxs;
    s32 dys;
    s32 a;
    s32 b;
    s32 t;
    s32 q;
    s32 *ty;
    s32 ang;

    ang = obj->angle;
    obj->targetX -= ang;
    dxs = (s16)obj->dx << 8;
    a = dxs - obj->targetX;
    b = dxs - obj->radius;
    dys = (s16)obj->dy << 8;
    t = dys - obj->wait;
    ty = &obj->targetY;
    q = (t * a) / b;
    *ty = dys - q;
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 4) & 0xFF;
    if (obj->targetX < obj->owner->part->targetX - 0x3000)
    {
        obj->state = map_obj_orbit_dh_down;
        obj->dh = 0x30;
    }
}
END_NONMATCH

void map_obj_orbit_dh_down(struct MapObj *obj)
{
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    obj->dh--;
    if (obj->dh <= 0)
    {
        obj->state = map_obj_orbit_dh_reset;
        obj->dh = 0;
    }
}

void map_obj_orbit_dh_reset(struct MapObj *obj)
{
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 4) & 0xFF;
    obj->dh--;
    if (obj->dh <= 0)
    {
        obj->state = map_obj_slide_drop;
        obj->dh = 0x22;
        obj->angle = 0x180;
        set_slot_value(0x10F, -1);
    }
}

NONMATCH("asm/code_0805E7C4/map_obj_slide_drop.s", void map_obj_slide_drop(struct MapObj *obj))
{
    s32 ang;
    s32 dxs;
    s32 dys;
    s32 a;
    s32 b;
    s32 t;
    s32 q;
    s32 *ty;

    ang = obj->angle;
    obj->targetX += ang;
    dxs = (s16)obj->dx << 8;
    a = dxs - obj->targetX;
    b = dxs - obj->radius;
    dys = (s16)obj->dy << 8;
    t = dys - obj->wait;
    ty = &obj->targetY;
    q = (t * a) / b;
    *ty = dys - q;
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, obj->dh, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, obj->dh);
    obj->timer = (obj->timer + 4) & 0xFF;
    if (obj->targetX > (((s16)obj->dx - 0x18) << 8))
    {
        gUnk_03000E15 = 2;
        obj->targetH -= 0x120;
        if (obj->dh > 0)
        {
            obj->dh--;
        }
        else
        {
            map_obj_release_chain(obj);
        }
    }
    if (obj->targetX > ((s16)obj->dx << 8))
    {
        map_obj_release_chain(obj);
    }
}
END_NONMATCH

void map_obj_slide_fast_left(struct MapObj *obj)
{
    s32 dxs;
    s32 dys;
    s32 a;
    s32 b;
    s32 t;
    s32 q;
    s32 *ty;

    obj->targetX -= 0x280;
    dxs = (s16)obj->dx << 8;
    a = dxs - obj->targetX;
    b = dxs - obj->radius;
    dys = (s16)obj->dy << 8;
    t = dys - obj->wait;
    ty = &obj->targetY;
    q = (t * a) / b;
    *ty = dys - q;
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C / 0x100);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    if (obj->targetX < obj->radius)
    {
        obj->targetX = obj->radius;
        obj->targetY = obj->wait;
        obj->dh = 0x96;
        obj->state = map_obj_orbit_dh_hold;
    }
}

void map_obj_orbit_dh_hold(struct MapObj *obj)
{
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C / 0x100);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    obj->dh--;
    if (obj->dh <= 0)
    {
        obj->dh = 0;
        obj->state = map_obj_orbit_dh_turn;
    }
}

void map_obj_orbit_dh_turn(struct MapObj *obj)
{
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C / 0x100);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 4) & 0xFF;
    obj->dh--;
    if (obj->dh <= 0)
    {
        if (obj->targetY <= 0x5FFF)
        {
            obj->wait = 0x2000;
        }
        else
        {
            obj->wait = -0x2000;
        }
        obj->angle = 0x800000 / obj->wait;
        obj->dx = 0x160;
        obj->dy = 0x22;
        obj->state = map_obj_slide_in_x;
        set_slot_value(0x10F, -1);
    }
}

NONMATCH("asm/code_0805E7C4/map_obj_slide_in_x.s", void map_obj_slide_in_x(struct MapObj *obj))
{
    obj->targetX -= obj->dx;
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C / 0x100);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 4) & 0xFF;
    if (obj->targetX <= 0x7FF)
    {
        obj->state = map_obj_drift_y;
    }
}
END_NONMATCH

void map_obj_drift_y(struct MapObj *obj)
{
    if (obj->wait >= 0)
    {
        obj->targetY += 0x100;
        obj->param9C += obj->angle;
        obj->wait -= 0x100;
        if (obj->wait <= 0)
        {
            obj->state = map_obj_drift_x;
        }
    }
    else
    {
        obj->targetY -= 0x100;
        obj->param9C += obj->angle;
        obj->wait += 0x100;
        if (obj->wait >= 0)
        {
            obj->state = map_obj_drift_x;
        }
    }
    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C / 0x100);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    obj->timer = (obj->timer + 2) & 0xFF;
    obj->dx = 0x160;
    set_slot_value(0x10F, -1);
}
