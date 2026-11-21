#include "global.h"
#include "ram_routines.h"

struct SysHead0
{
    u16 unk0;
};

struct PartSprite
{
    u8 filler0[0xC];
    u16 anim;
    u8 filler0E[0x11 - 0x0E];
    u8 flags11;
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 frame20;
    u8 filler21[0x2C - 0x21];
    s16 unk2C;
};

struct Part
{
    u8 filler0[0x4];
    u32 unk4;
    struct PartSprite *sprite;
    struct SpriteNode *spriteNodes;
    s32 posA;
    s32 posB;
    s32 height;
    u8 filler1C[0x28 - 0x1C];
    struct Part *owner;
    struct Part *link;
    struct Part *next;
    u8 filler34[0x38 - 0x34];
    s32 baseA;
    s32 baseB;
    s32 baseH;
    s32 unk44;
    s32 unk48;
    void (*state)(struct Part *);
    u8 filler50[0x54 - 0x50];
    void (*ownerState)(struct Part *);
    u8 filler58[0x5C - 0x58];
    void *handler5C;
    void *handler60;
    void *handler64;
    void (*callback68)(struct Part *);
    u32 link6C;
    u8 filler70[0x74 - 0x70];
    u8 kind74;
    u8 filler75[0x76 - 0x75];
    u8 flags76;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 step7A;
    u16 size7C;
    struct Part *cand80;
    s32 targetA;
    s32 targetB;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 stepH;
    u8 filler98[0x9C - 0x98];
    s32 flag9C;
    s32 limA0;
    s32 limA4;
    s32 countA8;
    u16 timer;
    s16 velA;
    s16 velB;
    u16 velH;
    u8 filler0B4[0xD8 - 0xB4];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    u8 filler0E4[0xEC - 0xE4];
    s16 unkEC;
    u8 filler0EE[0x113 - 0xEE];
    u8 flag113;
};

struct SpriteNode
{
    struct SpriteNode *next;
    s32 x;
    s32 y;
    s32 h;
};

/* The task part_setup_render starts (task_create_list_48) to run part_refresh_tiles. */
struct PartTileTask
{
    u8 filler0[4];
    u32 active;             /* 0 ends the task */
    struct Part *part;
};

struct TileMap
{
    u8 filler0[0xC];
    u16 cells[16];          /* owner of each OBJ palette: 0xFFFF free, 0x1FFF lent to place_layer_tile */
};

struct OwnerSprite
{
    u8 filler0[0x11];
    u8 flags11;
};

struct StatusChild
{
    u8 filler0[0x18];
    u8 *classData;
};

struct StatusObj
{
    u8 filler0[0x18];
    u8 *classData;
    u8 filler1C[0x284 - 0x1C];
    void *ptr284;
    void *ptr288;
    u8 filler28C[0x290 - 0x28C];
    void *ptr290;
    u8 filler294[0x2B8 - 0x294];
    void *ptr2B8;
    void *ptr2BC;
    void *ptr2C0;
};

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

struct Flags76Bits
{
    u8 low7 : 7;
    u8 bit7 : 1;
};

// The spawned child takes over two flag bits of its parent.
static inline void part_child_flags(struct Part *obj, struct Part *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

extern u8 gClassStatusChild[];
extern u8 gClassStatusObj[];
extern void *gUnk_03001008;
extern u8 *gWorkBuf;
extern u8 gUnk_02000460[];
extern u8 gSys[];
extern struct TileMap *gModelCtx;

void run_pending_obj_callbacks(void);
void heap_free_a(void *p);
void heap_free_b(void *p);
void menu_obj_free(void *obj, u32 kind);
void destroy_map_obj_loader(void *obj, u32 kind);
void task_remove(void *node, u32 kind);

u8 step_motion(struct Part *obj);
void set_motion_from_value(struct Part *obj, u32 value);
void sprite_set_action(void *obj, u32 action, u32 arg, u32 extra);
s32 is_state_busy(void);
void task_insert(void *node, u8 kind);
void wait_for_index(struct Part *obj);
void start_motion(struct Part *obj);
void sound_stop_song(u32 id);
u32 query_contact(struct Part *obj);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void set_slot_value(u32 id, s32 value);
void set_motion_start(struct Part *obj, u32 speed);
void place_targets(struct Part *obj, s32 x, s32 y, s32 z, s32 w);
u8 candidates_advance_progress(struct Part *obj);
void obj_list_push_default(struct Part *obj);
void obj_list_remove(struct Part *obj);
void obj_release(struct Part *obj);
struct Part *obj_spawn(struct Part *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct Part *owner);
struct SpriteNode *sprite_node_alloc(s32 x, s32 y, s32 h);
void part_setup_render(struct Part *obj);
void on_arrive(struct Part *obj);

u32 set_state_motion_begin(struct Part *obj);
void motion_begin(struct Part *obj);
void motion_step_1(struct Part *obj);
void motion_step_2(struct Part *obj);
void motion_step_3(struct Part *obj);
void motion_end(struct Part *obj);
void part_launch_a(struct Part *obj);
void part_launch_b(struct Part *obj);
void part_flight_a(struct Part *obj);
void part_flight_b(struct Part *obj);
void part_fall(struct Part *obj);
void part_sink(struct Part *obj);
void part_settle(struct Part *obj);
void part_rest_idle(struct Part *obj);
void part_stop(struct Part *obj);
void part_rest_idle_b(struct Part *obj);
u32 set_state_part_wait_ready_8(struct Part *obj);
u32 set_state_part_wait_ready_4(struct Part *obj);
void part_wait_ready_8(struct Part *obj);
void part_wait_ready_4(struct Part *obj);
void part_hold_start(struct Part *obj);
void part_hold_from_9c(struct Part *obj);
void part_countdown_a(struct Part *obj);
void part_countdown_b(struct Part *obj);
void part_anim_wait(struct Part *obj);
void part_launch_c(struct Part *obj);
void refresh_slot_flags(void);
void place_layer_tile(s32 x, s32 y, s32 z, s32 flag, u32 tile);

void part_track(struct Part *obj);
u32 part_check_contact(struct Part *obj);
void part_land_check(struct Part *obj);
void part_owner_watch(struct Part *obj);
void part_owner_idle(struct Part *obj);
void part_owner_idle_b(struct Part *obj);
u32 part_check_link(struct Part *obj);
void part_offscreen(struct Part *obj);
void part_unlinked_idle(struct Part *obj);
void part_wait_ready_2(struct Part *obj);
void part_setup_sprite(struct Part *obj);
void part_motion_wait(struct Part *obj);
void part_fly_drop(struct Part *obj);
void part_refresh_tiles(struct PartTileTask *task);
void part_spawn_timed(struct Part *obj);
void part_update_nodes(struct Part *obj);

/* State of the shot part_launch_c fires: each frame it moves velA left and takes the y on the line
 * from its launch point (limA4, countA8) to its target (targetA, targetB). Once it is more than 32 px
 * past the left edge of the screen it is put back at x 0xF600 by its owner, taken off the object list
 * and goes on to part_land_check. */
void part_track(struct Part *obj)
{
    s32 screen[3];
    s32 spanX = obj->limA4 - obj->targetA;
    s32 spanY = obj->countA8 - obj->targetB;
    register s16 *speed asm("r4") = &obj->velA;

    obj->posA -= *speed;
    obj->posB = (obj->posA - obj->limA4) * spanY / spanX + obj->countA8;
    screen[0] = obj->posA / 0x100;
    screen[1] = obj->posB / 0x100;
    screen[2] = obj->height / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    if (screen[0] < -0x20)
    {
        obj->posA = 0xF600;
        obj->posB = obj->owner->unk44 - 0x600;
        obj->height = obj->owner->unk48 + 0x2A00;
        obj_list_remove(obj);
        obj->state = part_land_check;
    }
}

u32 part_check_contact(struct Part *obj)
{
    u32 result = query_contact(obj);
    u32 ret;

    if (result == 0)
    {
        obj->velA = obj->velA / 2;
        obj->velB = result;
        obj->velH = result;
        sprite_set_action(obj, 1, 0, 0);
        set_slot_value(0x11F, -1);
        sprite_set_action(&obj->owner->sprite, 0xC, 0, 0);
        obj->owner->ownerState = part_owner_watch;
        obj->state = part_offscreen;
        ret = 0;
    }
    else
    {
        ret = result;
    }
    return ret;
}

void part_land_check(struct Part *obj)
{
    obj->posA -= obj->velA;
    if (obj->posA <= obj->owner->baseH - 0xA00)
    {
        sprite_set_action(&obj->owner->sprite, 0xA, 0, 0);
        set_slot_value(0x2E, -1);
        obj->owner->ownerState = part_owner_idle;
        obj_release(obj);
    }
}

void part_owner_watch(struct Part *obj)
{
    if ((obj->sprite->flags12 & 8) == 0)
    {
        if (obj->sprite->unk2C / 16 == 0x28)
        {
            set_slot_value(0x7E, -1);
        }
    }
    else
    {
        obj->owner->flag113 = 1;
        {
            u8 flags = obj->sprite->flags12;

            obj->sprite->flags12 = flags | 0x10;
        }
        obj->state = 0;
    }
}

void part_owner_idle(struct Part *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->state = wait_for_index;
    }
}

void part_owner_idle_b(struct Part *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->state = part_unlinked_idle;
    }
}

u32 part_check_link(struct Part *obj)
{
    obj->owner->flag113 = 0;
    if (obj->link->owner->unkEC == -1)
    {
        obj->flag9C = 0;
    }
    else
    {
        obj->flag9C = 1;
    }
    obj->state = part_wait_ready_2;
    return 1;
}

void part_offscreen(struct Part *obj)
{
    s32 sx;
    s32 sy;
    s32 sz;

    obj->posA = obj->baseA - obj->velA;
    obj->height = obj->baseH - (s16)obj->velH;
    obj->velH += 0x20;
    sx = obj->posA / 0x100;
    sy = obj->posB / 0x100;
    sz = obj->height / 0x100;
    offset_three_words_camera(&sx, &sy, &sz, 0);
    if (sy > 0x110)
    {
        obj_release(obj);
    }
    else if (sx < -0x10)
    {
        obj_release(obj);
    }
}

void part_unlinked_idle(struct Part *obj)
{
    if (obj->next == 0)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->state = wait_for_index;
    }
}

void part_wait_ready_2(struct Part *obj)
{
    if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = part_setup_sprite;
    }
}

void part_setup_sprite(struct Part *obj)
{
    if (obj->sprite->flags12 & 8)
    {
        u8 *fp;
        obj->targetA = 0x8800;
        obj->targetB = 0x6800;
        obj->targetH = obj->baseH;
        fp = &obj->flags79;
        *fp |= 0x20;
        obj->size7C = 0x180;
        obj->step7A = 0;
        start_motion(obj);
        sprite_set_action(obj, 3, 0, 0);
        set_slot_value(0x6c, -1);
        obj->state = part_motion_wait;
    }
}

void part_motion_wait(struct Part *obj)
{
    if (obj->cand80 == 0)
    {
        sound_stop_song(0x6C);
        if (obj->flag9C == 0)
        {
            sprite_set_action(obj, 4, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 7, 0, 0);
        }
        obj->timer = 0xA;
        obj->sprite->frame20 = 0x10;
        set_slot_value(0x117, -1);
        obj->state = part_anim_wait;
    }
}

/*
 * Falls under gravity (targetH is the vertical speed, stepH the gravity, all
 * 8.8 fixed point). On reaching the ground: spawns effect 0x273D and sets
 * slot 0x8B, spawns a chain object 20 px left and 8 px up of the base
 * position with 19 sprite nodes (run by part_update_nodes), then plays
 * action 0xF and goes on to part_setup_render.
 */
void part_fly_drop(struct Part *obj)
{
    struct Part *chain;
    struct SpriteNode *node;
    struct SpriteNode *head;
    u16 *timer;
    s32 i;

    obj->height += obj->targetH;
    obj->targetH -= obj->stepH;
    if (obj->height <= 0)
    {
        effect_spawn_at(0x273D, obj->baseA / 0x100, obj->baseB / 0x100 + 2, obj->baseH / 0x100, obj);
        set_slot_value(0x8B, -1);

        chain = obj_spawn(obj->owner, 0, obj->baseA / 0x100 - 0x14, obj->baseB / 0x100 - 8,
                          obj->baseH / 0x100, 1, 1);
        chain->next = obj->next;
        obj->next = chain;
        chain->limA4 = 0;
        chain->link6C = obj->link6C;
        chain->link = obj->link;
        chain->targetA = chain->posA;
        chain->targetB = chain->posB;
        chain->velA = 0;
        timer = &obj->timer;
        /* push 19 nodes, all at the chain's base position */
        for (i = 0; i < 19; i++)
        {
            node = sprite_node_alloc(chain->baseA, chain->baseB, chain->baseH);
            head = chain->spriteNodes;
            node->next = head;
            chain->spriteNodes = node;
        }
        chain->state = part_update_nodes;

        sprite_set_action(obj, 0xF, 0, 0);
        *timer = 1;
        obj->state = part_setup_render;
    }
}

/* Task started by part_setup_render. While the part has chain objects (part->next list), redraws
   the background under each chain and under each of its trail nodes as sprites (place_layer_tile,
   two tiles each from tile 0x200). Once the chains are gone, it restores OBJ palette 15
   (gUnk_02000460, saved in gWorkBuf + 0x3CD0) and the palette owners part_setup_render changed,
   frees the palettes place_layer_tile borrowed, and ends. */
void part_refresh_tiles(struct PartTileTask *task)
{
    struct Part *part;
    struct Part *chain;
    struct SpriteNode *node;
    u32 tile;
    s32 i;

    part = task->part;
    if (part->next != 0)
    {
        tile = 0x200;
        refresh_slot_flags();
        for (chain = part->next; chain != 0; chain = chain->next)
        {
            /* the chain itself starts like a node (spriteNodes, then its position) */
            for (node = (struct SpriteNode *)&chain->spriteNodes; node != 0; node = node->next)
            {
                place_layer_tile(node->x / 0x100, node->y / 0x100, node->h / 0x100, 1, tile);
                tile += 2;
            }
        }
    }
    else
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gWorkBuf + 0x3CD0, gUnk_02000460, 0x20);
        ((struct SysHead0 *)gSys)->unk0 |= 0x8000;
        gModelCtx->cells[15] = gModelCtx->cells[14];
        gModelCtx->cells[14] = gModelCtx->cells[13];
        for (i = 0; i < 15; i++)
        {
            if (gModelCtx->cells[i] == 0x1FFF)
            {
                gModelCtx->cells[i] = 0xFFFF;
            }
        }
        task->active = 0;
    }
}

/* After the drop, every limA0 frames (counted in countA8) spawns one more chain object 20 px left
   and 8 px up of the base position with 19 sprite nodes (run by part_update_nodes), counting the
   chains in timer; once timer passes flag9C, plays action 0x10 and goes on to wait_for_index. */
void part_spawn_timed(struct Part *obj)
{
    struct Part *chain;
    struct SpriteNode *node;
    struct SpriteNode *head;
    s32 i;

    obj->countA8 += 1;
    if (obj->countA8 >= obj->limA0)
    {
        obj->countA8 = 0;
        obj->timer += 1;
        if ((s16)obj->timer <= obj->flag9C)
        {
            chain = obj_spawn(obj->owner, 0, obj->baseA / 0x100 - 0x14, obj->baseB / 0x100 - 8,
                              obj->baseH / 0x100, 1, 1);
            chain->next = obj->next;
            obj->next = chain;
            chain->flag9C = 0;
            chain->limA0 = 0;
            chain->limA4 = 0;
            chain->link6C = obj->link6C;
            chain->link = obj->link;
            chain->targetA = chain->posA;
            chain->targetB = chain->posB;
            chain->velA = 0;
            /* push 19 nodes, all at the chain's base position */
            for (i = 0; i < 19; i++)
            {
                node = sprite_node_alloc(chain->baseA, chain->baseB, chain->baseH);
                head = chain->spriteNodes;
                node->next = head;
                chain->spriteNodes = node;
            }
            chain->state = part_update_nodes;
        }
        else
        {
            sprite_set_action(obj, 0x10, 0, 0);
            obj->state = wait_for_index;
        }
    }
}


ASM_FUNC("asm/code_08133F08/part_update_nodes.s", void part_update_nodes(struct Part *obj))

