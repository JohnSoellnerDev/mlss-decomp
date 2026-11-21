#include "global.h"

void obj_clear_offset_14(void *obj);
void obj_set_char_offset(void *obj);
void cam_update(void *obj, u32 mode);
void obj_set_sprite_kind(void *obj, u32 kind);
void obj_place_on_screen(void *obj, s32 *pos);
void obj_release_all(void *obj, u32 mode);
void obj_release_child(void *obj, u32 mode);
void obj_release_two_children(void *obj, u32 mode);
void window_destroy(void *win, u32 flags);
void obj_free_tile_bufs(void *obj);
void set_slot_value(u32 id, s32 value);
s32 view_coord(void *ctx, s32 pos);
void obj_set_vy_if_free(void *obj, s32 vy);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *obj_create_e3c0(void *obj, void *record, u32 zero);
void *obj_init_e380(void *obj, void *record, u32 zero);
void *set_class_e300_ex(void *obj, void *record, u32 zero);
void transition_states_obj_setup_sprites(void *obj);
void *obj_create_e2c0(void *obj, void *record, u32 zero);
void *obj_create_e280(void *obj, void *record, u32 zero);
void *obj_create_e240_random(void *obj, void *record, u32 zero);
void *obj_create_e200(void *obj, void *record, u32 zero);
void *set_class_e1c0_ex(void *obj, void *record, u32 zero);
void *set_class_e180_ex(void *obj, void *record, u32 zero);
void *set_class_e140_ex(void *obj, void *record, u32 zero);
void *obj_init_e100(void *obj, void *record, u32 zero);
void *set_class_e0c0_ex(void *obj, void *record, u32 zero);
void *obj_init_e080(void *obj, void *record, u32 zero);
void *set_class_e040_ex(void *obj, void *record, u32 zero);
void *set_class_e000_ex(void *obj, void *record, u32 zero);
void *set_class_dfc0_ex(void *obj, void *record, u32 zero);
void *set_class_df80_ex(void *obj, void *record, u32 zero);

struct CharObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[0x36 - 0xC];
    u16 flags; /* CHAR_FLAG_* */
};

struct CamTarget
{
    u8 filler0[0xC];
    s32 ofsX;
};

struct GameState
{
    u8 filler0[0x1C];
    u8 tileBufs[3][0x824]; /* each freed with obj_free_tile_bufs */
    u8 filler1888[0x1898 - 0x1888];
    void *objects[0x100]; /* struct ReleaseObj: the scene's objects, NULL where free */
    struct CharObj *charA;
    struct CharObj *charB;
    void *unk1CA0;
    void *unk1CA4;
    void *window;
    u8 filler1CAC[0x1CB0 - 0x1CAC];
    void *mainObj;
    struct CamTarget *camTarget;
    s32 camX;
    s32 camY;
    u8 filler1CC0[0x1CC4 - 0x1CC0];
    u16 flags;
    u8 filler1CC6[0x1F20 - 0x1CC6];
    u8 sceneState;
};

struct SceneStateRow
{
    u8 filler0[0x18];
    u16 unk18;
    u8 filler1A[6];
};

/* a position in 24.8 fixed point */
struct Vec2
{
    s32 x;
    s32 y;
};

struct ModeTable
{
    u8 filler0[0x30];
    s16 off30;
    u8 filler32[0x34 - 0x32];
    void (*fn34)(void *);
    s16 off38;
    u8 filler3A[0x3C - 0x3A];
    void (*fn3C)(void *);
};

struct SpriteBits1F
{
    u8 filler0[0x1F];
    u8 lo2 : 2;
    u8 b2 : 2;
    u8 b4 : 2;
    u8 hi2 : 2;
};

struct ObjRecord
{
    u8 filler0[0xC];
    u8 kind;
};

struct CreateTable
{
    u8 filler0[0x10];
    s16 off10;
    u8 filler12[0x14 - 0x12];
    void (*fn14)(void *obj, u32 a, u32 b, u32 c, u32 d);
};

struct CreateObj
{
    u8 filler0[0x30];
    struct CreateTable *table;
};

extern const u8 gHeapTagObjKind0[];
extern const u8 gHeapTagObjKind1[];
extern const u8 gHeapTagObjKind2[];
extern const u8 gHeapTagObjKind4[];
extern const u8 gHeapTagObjKind5[];
extern const u8 gHeapTagObjKind6[];
extern const u8 gHeapTagObjKind7[];
extern const u8 gHeapTagObjKind8[];
extern const u8 gHeapTagObjKind9[];
extern const u8 gHeapTagObjE140[];
extern const u8 gHeapTagObjE100[];
extern const u8 gHeapTagObjE0c0[];
extern const u8 gHeapTagObjE080[];
extern const u8 gHeapTagObjE040[];
extern const u8 gHeapTagObjE000[];
extern const u8 gHeapTagObjDfc0[];
extern const u8 gHeapTagObjDf80[];

struct StepSub
{
    u8 filler0[0x12];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

/* A field object that drops when a hero comes near and lands on the ground
   (obj_mode_step). Positions are 24.8 fixed point; the u16 speeds are read as s16. */
struct DropObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[0x14 - 0x0C];
    u16 vx;
    u8 filler16[0x18 - 0x16];
    u16 vy;
    u8 filler1A[0x1C - 0x1A];
    u16 accelX;
    u8 filler1E[0x20 - 0x1E];
    u16 accelY;
    u8 filler22[0x24 - 0x22];
    u8 state; /* DROP_STATE_* */
    u8 filler25[0x2D - 0x25];
    u8 landedBits; /* goes into the sprite's three 2-bit fields at 0x1F on landing */
    u8 filler2E[0x30 - 0x2E];
    struct ModeTable *table;
    u8 filler34[0x36 - 0x34];
    u16 timer;
    s32 groundY; /* ground height under x, refreshed every frame */
    u8 filler3C[0x40 - 0x3C];
    struct StepSub *sub;
};

enum
{
    DROP_STATE_IDLE,    /* not armed */
    DROP_STATE_ARMED,   /* waits for hero A to come within 0x82 px */
    DROP_STATE_SHAKING, /* shakes for 15 frames, then falls */
    DROP_STATE_LANDED,  /* landed: 20 frames before it settles */
    DROP_STATE_DONE,
};

struct HalfBit0
{
    u16 b0 : 1;
    u16 rest : 15;
};

/* A hero of the scene (gGamePtr->charA / charB) as its message handler sees it. */
struct HeroObj
{
    u8 filler0[0x18];
    u16 vy;
    u8 filler1A[0x24 - 0x1A];
    u8 state;
    u8 heroId; /* 0 or 1: picks the hurt sprite kind */
    u8 filler26[0x30 - 0x26];
    struct ModeTable *table;
    u8 filler34[0x36 - 0x34];
    u16 flags; /* CHAR_FLAG_* */
    u16 hurtTimer;
    u8 filler3A[0x42 - 0x3A];
    u8 savedSpriteByte21;
};

#define CHAR_FLAG_HURT 1

/* A C++ (GCC 2) vtable: entry 1 is the destructor; off8 is the `this` adjustment it needs. */
struct ReleaseTable
{
    u8 filler0[8];
    s16 off8;
    u8 filler0A[0xC - 0xA];
    void (*release)(void *obj, u32 mode); /* mode 3: destroy and free */
};

struct ReleaseObj
{
    u8 filler0[0x30];
    struct ReleaseTable *table; /* vtable */
};

extern const struct SceneStateRow gSceneStateTable_083CCAC0[];
extern struct GameState *gGamePtr;

void *obj_get_sprite(void *obj);
void obj_add_value(void *obj, s32 value);
u32 obj_tick_state(void *obj, void *ctx, s16 speed, u32 arg3);

// Destroys and frees everything the scene holds: its 256 objects (through each one's virtual
// destructor), the main object, the objects at 0x1CA0/0x1CA4, the window, and the three tile buffers.
// Mode 3 asks each destructor to free the memory as well.
void scene_release_objects(struct GameState *state)
{
    s32 i;
    struct ReleaseObj *obj;
    struct ReleaseTable *vtable;

    for (i = 0; i < 0x100; i++)
    {
        obj = state->objects[i];
        if (obj != 0)
        {
            vtable = obj->table;
            vtable->release((u8 *)obj + vtable->off8, 3);
        }
        state->objects[i] = 0;
    }
    if (state->mainObj != 0)
    {
        obj_release_all(state->mainObj, 3);
        state->mainObj = 0;
    }
    if (state->unk1CA0 != 0)
    {
        obj_release_child(state->unk1CA0, 3);
        state->unk1CA0 = 0;
    }
    if (state->unk1CA4 != 0)
    {
        obj_release_two_children(state->unk1CA4, 3);
        state->unk1CA4 = 0;
    }
    if (state->window != 0)
    {
        window_destroy(state->window, 3);
        state->window = 0;
    }
    obj_free_tile_bufs(state->tileBufs[0]);
    obj_free_tile_bufs(state->tileBufs[1]);
    obj_free_tile_bufs(state->tileBufs[2]);
}

ASM_FUNC("asm/code_081529C4/transition_scene_update.s", void transition_scene_update(void))

/* While the scene's flag 8 is set, lets the player steer the spotlight (camTarget, an iris
 * window) with the d-pad, and while hero A isn't hurt turns the hero towards it: sprite kind 0
 * when the light is more than 10 px to the right, 4 above 4 px, 5 down to -4 px, else 6.
 * Then draws the light around hero A's position on screen (relative to the camera). */
void game_update_camera(struct GameState *game)
{
    struct Vec2 heroPos;
    s32 *screenPos;
    s32 lightX;

    if (game->flags & 8)
    {
        cam_update(game->camTarget, 1);
        if (((game->charA->flags ^ CHAR_FLAG_HURT) & CHAR_FLAG_HURT) != 0)
        {
            lightX = game->camTarget->ofsX >> 8;
            if (lightX > 10)
            {
                obj_set_sprite_kind(game->charA, 0);
            }
            else if (lightX > 4)
            {
                obj_set_sprite_kind(game->charA, 4);
            }
            else if (lightX > -5)
            {
                obj_set_sprite_kind(game->charA, 5);
            }
            else
            {
                obj_set_sprite_kind(game->charA, 6);
            }
        }
    }

    heroPos = *(struct Vec2 *)&game->charA->x;
    screenPos = &heroPos.x;
    screenPos[0] -= game->camX;
    screenPos[1] -= game->camY;
    obj_place_on_screen(game->camTarget, screenPos);
}

void game_clamp_camera(struct GameState *game)
{
    s32 dx;
    s32 cam;
    s32 *cam_p;
    s32 limit;
    s32 cam_y;

    dx = game->charA->x - game->charB->x;
    cam_p = &game->camX;
    cam = game->charB->x + dx / 2 - 0x7800;
    *cam_p = cam;
    if (cam < 0)
    {
        *cam_p = 0;
    }
    else
    {
        s32 hi = cam >> 8;

        limit = gSceneStateTable_083CCAC0[game->sceneState].unk18 - 1;
        if (hi >= (limit << 8))
        {
            *cam_p = limit << 16;
        }
    }

    cam_y = game->camY;
    if (cam_y < 0)
    {
        game->camY = 0;
    }
    else if ((cam_y >> 8) > 0x5F)
    {
        game->camY = 0x6000;
    }
}

/* Creates the field object a placement record describes: allocates an object of the size its
   kind needs (record->kind picks the class and the heap tag) and runs that class's constructor
   on it. Kinds 0 and 1 share one class and also get their class's slot-0x10 message right away;
   kind 4 sets up its sprites. Kinds 3, 10 and unknown kinds create nothing and return NULL. */
void *obj_create(void *unused, struct ObjRecord *record)
{
    void *obj;
    const void *heapTag;
    struct CreateTable *klass;

    switch (record->kind)
    {
    case 0:
        heapTag = gHeapTagObjKind0;
        goto generic;
    case 1:
        heapTag = gHeapTagObjKind1;
    generic:
        obj = obj_create_e3c0(heap_alloc2(0x48, 1, heapTag, 0), record, 0);
        klass = ((struct CreateObj *)obj)->table;
        klass->fn14((u8 *)obj + klass->off10, 0, 0, 0, 1);
        return obj;
    case 2:
        obj = obj_init_e380(heap_alloc2(0x44, 1, gHeapTagObjKind2, 0), record, 0);
        return obj;
    case 4:
        obj = set_class_e300_ex(heap_alloc2(0x40, 1, gHeapTagObjKind4, 0), record, 0);
        transition_states_obj_setup_sprites(obj);
        return obj;
    case 5:
        obj = obj_create_e2c0(heap_alloc2(0x44, 1, gHeapTagObjKind5, 0), record, 0);
        return obj;
    case 6:
        obj = obj_create_e280(heap_alloc2(0x44, 1, gHeapTagObjKind6, 0), record, 0);
        return obj;
    case 7:
        obj = obj_create_e240_random(heap_alloc2(0x3C, 1, gHeapTagObjKind7, 0), record, 0);
        return obj;
    case 9:
        obj = obj_create_e200(heap_alloc2(0x3C, 1, gHeapTagObjKind9, 0), record, 0);
        return obj;
    case 8:
        obj = set_class_e1c0_ex(heap_alloc2(0x38, 1, gHeapTagObjKind8, 0), record, 0);
        return obj;
    case 16:
        /* shares kind 8's heap tag */
        obj = set_class_e180_ex(heap_alloc2(0x38, 1, gHeapTagObjKind8, 0), record, 0);
        return obj;
    case 11:
        obj = set_class_e140_ex(heap_alloc2(0x38, 1, gHeapTagObjE140, 0), record, 0);
        return obj;
    case 12:
        obj = obj_init_e100(heap_alloc2(0x3C, 1, gHeapTagObjE100, 0), record, 0);
        return obj;
    case 13:
        obj = set_class_e0c0_ex(heap_alloc2(0x38, 1, gHeapTagObjE0c0, 0), record, 0);
        return obj;
    case 14:
        obj = obj_init_e080(heap_alloc2(0x40, 1, gHeapTagObjE080, 0), record, 0);
        return obj;
    case 17:
        obj = set_class_e000_ex(heap_alloc2(0x40, 1, gHeapTagObjE000, 0), record, 0);
        return obj;
    case 15:
        obj = set_class_e040_ex(heap_alloc2(0x3C, 1, gHeapTagObjE040, 0), record, 0);
        return obj;
    case 18:
        obj = set_class_dfc0_ex(heap_alloc2(0x38, 1, gHeapTagObjDfc0, 0), record, 0);
        return obj;
    case 19:
        obj = set_class_df80_ex(heap_alloc2(0x3C, 1, gHeapTagObjDf80, 0), record, 0);
        return obj;
    case 3:
    case 10:
    default:
        return NULL;
    }
}

void game_set_chars_active(struct GameState *game, u32 on)
{
    u16 clearMask = 0xFFEF;
    u16 setMask = 9;
    if (on != 0)
    {
        game->flags = (game->flags & 0xFFF6) | 0x10;
        obj_clear_offset_14(game->charA);
        obj_clear_offset_14(game->charB);
    }
    else
    {
        game->flags = (game->flags | setMask) & clearMask;
        obj_set_char_offset(game->charA);
        obj_set_char_offset(game->charB);
    }
}

ASM_FUNC("asm/code_081529C4/scene_update_screen.s", void scene_update_screen(void))

// Message handler of a scene hero. Message 1 calls the class's slot-0x34 method, 2 is ignored.
// Message 5 hurts the hero unless it is already hurt: it sets the hurt flag for 100 frames,
// keeps sprite byte 0x21, shows the hurt sprite (kind 0x28 or 0x29 by hero) and takes 3 off
// the scene's counter (obj_add_value). 12 starts a jump (vy = -1000, state 2), 13 sets state 8.
// Anything else goes to the base handler, obj_tick_state.
u32 obj_mode_switch(struct HeroObj *hero, void *ctx, u16 speed, u32 arg3, u8 msg)
{
  int notHurt;
  switch (msg)
  {
    case 1:
    {
      struct ModeTable *table = hero->table;
      table->fn34(((u8 *) hero) + table->off30);
      break;
    }

    case 2:
      break;

    case 5:
    {
      void *sprite;
      u16 hurtFlag;
      notHurt = hero->flags ^ CHAR_FLAG_HURT;
      if ((((long) ((u16) notHurt)) & CHAR_FLAG_HURT) != 0)
      {
        hurtFlag = CHAR_FLAG_HURT;
        hero->flags = hero->flags | hurtFlag;
        hero->hurtTimer = 100;
        sprite = obj_get_sprite(hero);
        hero->savedSpriteByte21 = ((u8 *) sprite)[0x21];
        if (hero->heroId == 0)
        {
          obj_set_sprite_kind(hero, 0x28);
        }
        else
        {
          obj_set_sprite_kind(hero, 0x29);
        }
        obj_add_value(gGamePtr->mainObj, -3);
      }
      break;
    }

    case 13:
      hero->state = 8;
      break;

    case 12:
      hero->vy = -1000;
      hero->state = 2;
      break;

    default:
      return obj_tick_state(hero, ctx, speed, arg3);

  }

  return 0;
}

ASM_FUNC("asm/code_081529C4/obj_tick_state.s", u32 obj_tick_state(void *obj, void *ctx, s16 speed, u32 arg3))

/* Handler of a dropping object. Message 3 arms it (table slot 0x34), message 4 disarms it
   (slot 0x3C) and sets sub->bit5; anything else but 1 and 2 is a frame update: armed, it
   waits until it is at most 0x81 px right of hero A, shakes for 15 frames (sprite kind 0x19),
   then falls (kind 0x1A) with its own acceleration plus `gravity` until it is below the ground
   height under it (view_coord). Landing plays sound 0x38, knocks the camera target with the
   fall speed and clears sub->bit5 for 20 frames. */
u32 obj_mode_step(struct DropObj *obj, void *ctx, u16 gravity, u32 arg3, u8 mode)
{
    struct ModeTable *table;
    s32 timer;
    s32 dist_x;
    s32 ground;
    s32 accel;

    switch (mode)
    {
    case 1:
    case 2:
        break;
    case 3:
        table = obj->table;
        table->fn34((u8 *)obj + table->off30);
        obj->state = DROP_STATE_ARMED;
        break;
    case 4:
        table = obj->table;
        table->fn3C((u8 *)obj + table->off38);
        obj->sub->bit5 = 1;
        obj->state = DROP_STATE_IDLE;
        break;
    default:
        ground = view_coord(ctx, obj->x);
        obj->groundY = (s32)((u32)ground << 16) >> 8;
        switch (obj->state)
        {
        case DROP_STATE_IDLE:
        case DROP_STATE_DONE:
            break;
        case DROP_STATE_ARMED:
            dist_x = obj->x - gGamePtr->charA->x;
            if (dist_x <= 0x81FF)
            {
                obj->timer = 15;
                obj_set_sprite_kind(obj, 0x19);
                obj->state = DROP_STATE_SHAKING;
            }
            break;
        case DROP_STATE_SHAKING:
            timer = obj->timer;
            timer--;
            obj->timer = timer;
            if ((s16)timer > 0)
            {
                break;
            }
            obj_set_sprite_kind(obj, 0x1A);
            obj->vx += obj->accelX;
            accel = (s16)gravity;
            accel += obj->accelY;
            obj->vy += accel;
            obj->x += (s16)obj->vx;
            obj->y += (s16)obj->vy;
            if (obj->y > obj->groundY)
            {
                set_slot_value(0x38, -1);
                ((struct SpriteBits1F *)obj_get_sprite(obj))->lo2 = obj->landedBits;
                ((struct SpriteBits1F *)obj_get_sprite(obj))->b2 = obj->landedBits;
                ((struct SpriteBits1F *)obj_get_sprite(obj))->b4 = obj->landedBits;
                obj->sub->bit5 = 0;
                obj_set_vy_if_free(gGamePtr->camTarget, (s16)obj->vy);
                obj->timer = 20;
                obj->state = DROP_STATE_LANDED;
            }
            break;
        case DROP_STATE_LANDED:
            timer = obj->timer;
            timer--;
            obj->timer = timer;
            if ((s16)timer > 0)
            {
                break;
            }
            obj->sub->bit5 = 1;
            obj->state = DROP_STATE_DONE;
            break;
        default:
            break;
        }
        break;
    }
    return 0;
}

/* Handler of a dropping object that watches hero B (obj_mode_step watches hero A). Message 3 arms
   it (table slot 0x34), message 4 disarms it (slot 0x3C) and sets sub->bit5; anything else but 1
   and 2 is a frame update: armed, it waits until it is at most 0x81 px right of hero B, shakes for
   15 frames (sprite kind 0x19), then falls (kind 0x1A) with its own acceleration plus `gravity`
   until it is below the ground height under it (view_coord). Landing plays sound 0x38 and clears
   sub->bit5 for 20 frames. */
u32 obj_mode_step_alt(struct DropObj *obj, void *ctx, u16 gravity, u32 arg3, u8 mode)
{
    struct ModeTable *table;
    s32 timer;
    s32 dist_x;
    s32 ground;
    s32 accel;

    switch (mode)
    {
    case 1:
    case 2:
        break;
    case 3:
        table = obj->table;
        table->fn34((u8 *)obj + table->off30);
        obj->state = DROP_STATE_ARMED;
        break;
    case 4:
        table = obj->table;
        table->fn3C((u8 *)obj + table->off38);
        obj->sub->bit5 = 1;
        obj->state = DROP_STATE_IDLE;
        break;
    default:
        ground = view_coord(ctx, obj->x);
        obj->groundY = (s32)((u32)ground << 16) >> 8;
        switch (obj->state)
        {
        case DROP_STATE_IDLE:
        case DROP_STATE_DONE:
            break;
        case DROP_STATE_ARMED:
            dist_x = obj->x - gGamePtr->charB->x;
            if (dist_x <= 0x81FF)
            {
                obj->timer = 15;
                obj_set_sprite_kind(obj, 0x19);
                obj->state = DROP_STATE_SHAKING;
            }
            break;
        case DROP_STATE_SHAKING:
            timer = obj->timer;
            timer--;
            obj->timer = timer;
            if ((s16)timer > 0)
            {
                break;
            }
            obj_set_sprite_kind(obj, 0x1A);
            obj->vx += obj->accelX;
            accel = (s16)gravity;
            accel += obj->accelY;
            obj->vy += accel;
            obj->x += (s16)obj->vx;
            obj->y += (s16)obj->vy;
            if (obj->y > obj->groundY)
            {
                set_slot_value(0x38, -1);
                obj->sub->bit5 = 0;
                obj->timer = 20;
                obj->state = DROP_STATE_LANDED;
            }
            break;
        case DROP_STATE_LANDED:
            timer = obj->timer;
            timer--;
            obj->timer = timer;
            if ((s16)timer > 0)
            {
                break;
            }
            obj->sub->bit5 = 1;
            obj->state = DROP_STATE_DONE;
            break;
        default:
            break;
        }
        break;
    }
    return 0;
}

ASM_FUNC("asm/code_081529C4/obj_behavior_e220.s", void obj_behavior_e220(void))
