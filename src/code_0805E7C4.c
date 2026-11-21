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

#define POSE_FLAG_ANIM_DONE 0x08 // MapPose.flags: the current animation has finished

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

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
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

// When the current animation ends: switches to action 5, plays effect 0x12E2 and
// spawns the attack: a head object (0x412E) trailing a chain of 11 sprite nodes, whose
// movement depends on param9C (orbit out, slide left, slide fast left), plus a holder
// (0x700C) and two pieces (0x412F) placed from offsets. Then waits in map_obj_settle.
void map_obj_action5_spawn(struct MapObj *obj)
{
    struct MapObj *head;
    struct MapObj *holder;
    struct MapObj *piece0;
    struct MapObj *piece1;
    struct ChainNode *node;
    struct ChainNode *tail;
    s32 pattern;
    s32 i;

    if (obj->pose->flags & POSE_FLAG_ANIM_DONE)
    {
        sprite_set_action(obj, 5, 0, 0);
        effect_spawn_at(0x12E2, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        head = obj_spawn(obj->part, 0x412E, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                         obj->baseH / 0x100 + 0x30, ((u32)obj->pose->flip << 25) >> 31, 1);
        sprite_set_action(head, 1, 0, 0);
        gUnk_03000E14 = 1;
        gUnk_03000E15 = 0;
        obj->sprite = head;
        ((struct Flags79Bit0 *)&head->flags79)->bit0 = 1;
        ((struct ObjFlags77 *)head)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)head)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        head->handler68 = on_arrive;
        head->link = obj->link;
        head->owner = obj->owner;
        head->tag = obj->tag;
        pattern = obj->param9C;

        switch (pattern)
        {
        case 0:
            head->targetX = obj->baseX - 0x1000;
            head->targetY = obj->baseY + 0x200;
            head->targetH = obj->baseH + 0x3000;
            head->timer = 0;
            head->param9C = 0;
            head->angle = (arc_tan2((s16)((0x4000 - head->targetX) / 0x100),
                                    (s16)((0x4000 - head->targetY) / 0x100)) << 16) >> 24;
            head->radius = 0x200;
            head->state = map_obj_orbit_out;
            break;
        case 1:
            head->dx = obj->baseX / 0x100 - 0x10;
            head->dy = obj->baseY / 0x100 + 2;
            head->dh = obj->baseH / 0x100 + 0x30;
            head->targetX = head->dx << 8;
            head->targetY = head->dy << 8;
            head->targetH = head->dh << 8;
            head->radius = obj->owner->part->targetX + 0x3000;
            head->wait = obj->owner->part->targetY;
            head->param9C = 0xC;
            head->angle = 0x180;
            head->timer = 0;
            head->state = map_obj_slide_left;
            break;
        case 2:
            head->dx = obj->baseX / 0x100 - 0x10;
            head->dy = obj->baseY / 0x100 + 2;
            head->dh = obj->baseH / 0x100 + 0x30;
            head->targetX = head->dx << 8;
            head->targetY = head->dy << 8;
            head->targetH = head->dh << 8;
            head->radius = obj->owner->part->targetX + 0x2800;
            head->wait = obj->owner->part->targetY;
            head->param9C = 0x1000;
            head->angle = 0x160;
            head->timer = 0;
            head->state = map_obj_slide_fast_left;
            break;
        }

        tail = (struct ChainNode *)&head->chain0C;
        node = sprite_node_take();
        head->node4 = node;
        node->pose = head->pose;
        node->slot = &head->chain0C;
        for (i = 0; i < 11; i++)
        {
            tail->next = sprite_node_alloc(head->posX, head->posY, head->posH);
            tail = tail->next;
        }
        tail->next = 0;

        holder = obj_spawn(head->part, 0x700C, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2, 0,
                           ((u32)obj->pose->flip << 25) >> 31, 1);
        head->sprite = holder;
        ((struct Flags79Bit0 *)&holder->flags79)->bit0 = 1;
        holder->tag = obj->tag;
        holder->filler75[0] = 0x7F;
        holder->state = 0;

        piece0 = obj_spawn(holder->part, 0x412F, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                           obj->baseH / 0x100 + 0x30, ((u32)obj->pose->flip << 25) >> 31, 1);
        holder->sprite = piece0;
        ((struct Flags79Bit0 *)&piece0->flags79)->bit0 = 1;
        ((struct ObjFlags77 *)piece0)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)piece0)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        piece0->handler68 = on_arrive;
        piece0->link = obj->link;
        piece0->owner = obj->owner;
        obj_list_push_default(piece0);
        piece0->tag = obj->tag;
        piece0->param9C = 0;
        piece0->dx = obj->baseX / 0x100 - 0x10;
        piece0->dy = obj->baseY / 0x100 + 2;
        piece0->dh = obj->baseH / 0x100 + 0x30;
        piece0->timer = 0;
        piece0->state = map_obj_place_from_offsets;

        piece1 = obj_spawn(piece0->part, 0x412F, obj->baseX / 0x100 - 0x10, obj->baseY / 0x100 + 2,
                           obj->baseH / 0x100 + 0x30, ((u32)obj->pose->flip << 25) >> 31, 1);
        piece0->sprite = piece1;
        ((struct Flags79Bit0 *)&piece1->flags79)->bit0 = 1;
        ((struct ObjFlags77 *)piece1)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)piece1)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        piece1->handler68 = on_arrive;
        piece1->link = obj->link;
        piece1->owner = obj->owner;
        obj_list_push_default(piece1);
        piece1->tag = obj->tag;
        piece1->param9C = 0;
        piece1->dx = obj->baseX / 0x100 - 0x10;
        piece1->dy = obj->baseY / 0x100 + 2;
        piece1->dh = obj->baseH / 0x100 + 0x30;
        piece1->timer = 1;
        piece1->state = map_obj_place_from_offsets;

        obj->state = map_obj_settle;
        set_slot_value(0x110, -1);
    }
}

/* Chain head, first move of pattern 0: draws the chain, then moves the head `radius` (24.8) per frame
 * along `angle` toward (0x40, 0x40) px, while its wave (param9C, up to 0x38) widens and its phase
 * (timer) advances. Once there it holds still in map_obj_aim_down. */
void map_obj_orbit_out(struct MapObj *obj)
{
    s32 *angle;
    const s16 *cosTable;
    s32 speed;
    s32 mask;

    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    speed = obj->radius / 0x100;
    cosTable = gCosineTable;
    angle = &obj->angle;
    mask = 0xFF;
    obj->targetX += speed * (cosTable[(u8)*angle] / 0x40);
    speed = obj->radius / 0x100;
    obj->targetY += speed * (gSineTable[*angle & mask] / 0x40);
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

/* Chain head, pattern 0, between map_obj_orbit_out and the fall: draws the chain while its wave
 * (param9C, up to 0x38) widens and its phase (timer) advances; once `wait` runs out it aims the
 * head at (0, 0xF0) px (8-bit angle), sets its speed to 1.5 px (0x180, 24.8) and goes on to
 * map_obj_orbit_wait. */
void map_obj_aim_down(struct MapObj *obj)
{
    s32 toX;
    s32 toY;

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
        toX = (s16)((0 - obj->targetX) / 0x100);
        toY = (s16)((0xF000 - obj->targetY) / 0x100);
        {
            u32 angle16 = arc_tan2(toX, toY);
            obj->angle = (angle16 << 16) >> 24;
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

/* Chain head, last move of pattern 0 (after map_obj_aim_down aimed it at (0, 0xF0) px): draws the
 * chain and moves the head `radius` (24.8) per frame along `angle` while its wave phase (timer)
 * advances; once it is below y 0xF0 px (off the bottom of the screen) the head and its chain are
 * released. */
void map_obj_fall(struct MapObj *obj)
{
    s32 *speedPtr;
    s32 *anglePtr;
    const s16 *cosTable;
    s32 speed;
    s32 mask;
    s32 speedY;
    s32 delta;

    obj_orbit_place(obj, obj->targetX, obj->targetY, obj->targetH, 0x22, (s16)obj->timer, obj->param9C);
    obj_set_sprite_offsets(obj, obj->targetX, obj->targetY, obj->targetH, 0x22);
    speedPtr = &obj->radius;
    speed = *speedPtr / 0x100;
    cosTable = gCosineTable;
    anglePtr = &obj->angle;
    mask = 0xFF;
    delta = speed * (cosTable[(u8)*anglePtr] / 0x40);
    obj->targetX += delta;
    speedY = *speedPtr / 0x100;
    delta = speedY * (gSineTable[*anglePtr & mask] / 0x40);
    obj->targetY += delta;
    obj->timer = (obj->timer + 4) & mask;
    if (obj->targetY > 0xF000)
    {
        map_obj_release_chain(obj);
    }
}

/* Chain head, first move of pattern 1: moves the head 1.5 px (0x180, 24.8) left per frame along the
 * straight line from its start (dx, dy, in px) to (radius, wait) (the owner's part, 0x30 px to the
 * right), drawing the chain while its wave phase (timer) advances. Past the end point it goes on
 * to map_obj_orbit_dy_down. */
void map_obj_slide_left(struct MapObj *obj)
{
    s32 startX;
    s32 startY;
    s32 travelled;
    s32 spanX;
    s32 spanY;
    s32 dropY;
    s32 *targetY;

    obj->targetX -= 0x180;
    startX = (s16)obj->dx << 8;
    travelled = startX - obj->targetX;
    spanX = startX - obj->radius;
    startY = (s16)obj->dy << 8;
    spanY = startY - obj->wait;
    targetY = &obj->targetY;
    dropY = (spanY * travelled) / spanX;
    *targetY = startY - dropY;
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

/* Chain head, pause before the second move: keeps the head where it is and draws the chain with
 * its wave phase (timer) advancing 4 a frame while dy counts down. Then dy becomes the start Y (in
 * px) of the next slide, the current end Y (wait), sound 0x10F is queued and the head goes on to
 * map_obj_slide_by_angle. */
void map_obj_orbit_dy_reset(struct MapObj *head)
{
    obj_orbit_place(head, head->targetX, head->targetY, head->targetH, 0x22, (s16)head->timer, head->param9C);
    obj_set_sprite_offsets(head, head->targetX, head->targetY, head->targetH, 0x22);
    head->timer = (head->timer + 4) & 0xFF;
    head->dy--;
    if (head->dy <= 0)
    {
        head->dy = head->wait / 0x100;
        head->state = map_obj_slide_by_angle;
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

/* Chain head, pause before the last slide (pattern 1): keeps the head where it is and draws the chain
 * with its wave phase (timer) advancing 4 a frame while dh counts down. Then the chain length (dh)
 * goes back to 0x22, `angle` becomes the slide speed (0x180: 1.5 px, 24.8), sound 0x10F is queued
 * and the head goes on to map_obj_slide_drop. */
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
    s32 dxs;
    s32 dys;
    s32 a;
    s32 b;
    s32 t;
    s32 q;
    s32 *ty;

    obj->targetX += obj->angle;
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

/* Chain head: slides left 2.5 px (0x280, 24.8) per frame along the straight line from its start
 * (dx, dy px) to its end point (radius, wait; 24.8), with y following x on that line, and draws the
 * chain with the wave's phase (timer) advancing by 2. Once past the end it stops there and holds for
 * 0x96 frames (map_obj_orbit_dh_hold). */
void map_obj_slide_fast_left(struct MapObj *obj)
{
    s32 startX;
    s32 startY;
    s32 doneX;
    s32 spanX;
    s32 spanY;
    s32 doneY;
    s32 *y;

    obj->targetX -= 0x280;
    startX = (s16)obj->dx << 8;
    doneX = startX - obj->targetX;
    spanX = startX - obj->radius;
    startY = (s16)obj->dy << 8;
    spanY = startY - obj->wait;
    y = &obj->targetY;
    doneY = (spanY * doneX) / spanX;
    *y = startY - doneY;
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

/* Chain head, after its hold: draws the chain with the wave's phase (timer) advancing by 4 while dh
 * counts down; then sets up the drift to come (wait = 32 px down if it is in the top 0x60 px, else
 * 32 px up; angle = the wave width's change per drift step; dx = 0x160 slide speed, dy = 0x22),
 * plays sound 0x10F and slides in along X (map_obj_slide_in_x). */
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

/* Chain head, after sliding in from the right: moves 1 px per frame toward its Y goal (`wait` holds
 * the distance left, signed; down while it is positive, up while negative) while the wave width
 * (param9C, 24.8) changes by `angle` per frame, and goes on to map_obj_drift_x once there. Draws the
 * chain with the wave's phase (timer) advancing by 2, sets dx to 0x160 for the next slide and keeps
 * sound 0x10F playing. */
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
