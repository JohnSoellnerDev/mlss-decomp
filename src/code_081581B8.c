#include "global.h"
#include "ram_routines.h"

struct SpriteObj
{
    u16 x;
    u16 y;
    u16 unk04;
    u16 unk06;
    u8 filler08[0x0C - 0x08];
    u16 unk0C;
    u8 filler0E[0x12 - 0x0E];
    u8 unk12;
};

struct Bits12Field
{
    u8 b0 : 1;
    u8 mode : 2;
};

struct Pos16
{
    u16 x;
    u16 y;
};

struct CamPos
{
    s32 x;
    s32 y;
};

struct ObjPair
{
    s32 first;
    s32 second;
};

struct ScrollSub
{
    s32 unk00;
    s32 unk04;
};

struct ObjClass
{
    u8 filler0[0x30];
    s16 thisOffset30;
    u16 filler32;
    void (*fn34)(void *self);
    s16 thisOffset38;
    u16 filler3A;
    void (*fn3C)(void *self);
};

struct ObjKlassA
{
    u8 filler0[0x10];
    s16 thisOffset10;
    u16 filler12;
    void (*fn14)(void *self);
};

struct ClassVt
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*fn0C)(void *self, u32 mode);
};

struct TransObj
{
    union
    {
        u16 half;
        s32 word;
    } unk00;
    s32 unk04;
    union
    {
        u16 half;
        s32 word;
    } unk08;
    s32 unk0C;
    s32 unk10;
    union
    {
        u16 half;
        s16 shalf;
        s32 word;
    } unk14;
    union
    {
        u16 half;
        s16 shalf;
        s32 word;
    } unk18;
    union
    {
        s16 half;
        u16 uhalf;
        s32 word;
    } unk1C;
    union
    {
        s16 half;
        u16 uhalf;
        s32 word;
    } unk20;
    union
    {
        u8 byte;
        s32 word;
        u8 bytes[4];
    } unk24;
    u8 filler28[0x30 - 0x28];
    const void *klass30;
    union
    {
        struct TransObj *child;
        struct SpriteObj *sprite;
        s32 word;
        struct
        {
            s16 kind;
            u16 unk36;
        } pair;
    } unk34;
    union
    {
        u16 half;
        s32 word;
    } unk38;
    union
    {
        u16 half;
        s32 word;
    } unk3C;
    struct Pos16 *unk40;
    u8 filler44[0x4C - 0x44];
    struct ScrollSub *unk4C;
};

extern const u8 gTransitionClassDf40[];
extern const u8 gTransitionClassDf80[];
extern const u8 gTransitionClassDfc0[];
extern const u8 gTransitionClassE000[];
extern const u8 gTransitionClassE040[];
extern const u8 gTransitionClassE080[];
extern const u8 gTransitionClassE0c0[];
extern const u8 gClassE100[];
extern const u8 gClassE140[];
extern const u8 gClassE180[];
extern const u8 gClassE1c0[];
extern const u8 gClassE200[];
extern const u8 gClassE240[];
extern const u8 gClassE300[];
extern const u8 gClassE340[];
extern const u8 gClassE400[];
extern const u8 gClassE380[];
extern const u8 gTransitionClassDea0[];
extern const u8 gTransitionClassDf00[];
extern const u8 gHeapTagObjInitBuf[];
extern u8 gGameMode;
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
s32 effect_init_sprites_b(void *p);
s32 effect_init_sprites(void *p);

void sprite_set_pos_rel_cam(struct TransObj *obj, void *cam);
void obj_reset(struct TransObj *obj, u32 flags);
void obj_init_from_record(struct TransObj *obj, u32 arg);
void obj_create_sprite(struct TransObj *obj, u32 zero);
s32 obj_flush_anim(const void *klass);
void release_obj_131c(void *p);
u32 mt19937_next(void);
void heap_free_b(void *p);
struct SpriteObj *obj_get_sprite(struct TransObj *obj);
void obj_set_sprite_kind(struct TransObj *obj, u32 kind);
void obj_update_digits(struct TransObj *obj);
u32 view_coord(u32 a, s32 b);
void obj_release_sprite(struct TransObj *obj);
void effect_init_sprite_pair(void *p);
extern const u8 gTransitionClassDee0[];
void obj_state_load_step(struct TransObj *obj);
void obj_track_char_a(struct TransObj *obj);
u32 rand_below(u32 n, u32 r);
void obj_release_child_free(void *p, u32 mode);
void obj_release_child_free_c(void *p, u32 mode);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_init_from_record(struct TransObj *obj, u32 arg);

extern u8 *gGamePtr;
extern s16 *gUnk_03001014;
void win_draw_text(void *target, u32 arg, s32 dx, u32 y, u32 flag);

struct TransCamObj
{
    u8 filler0[0x30];
    s32 unk30;
};

struct TransRecC
{
    u32 state;
    const struct ClassVt *table;
    u32 unk08;
    s32 fade;
    s32 timer;
    void *aux;
};

extern const u8 gHeapTagTransitionAux[];
struct TransRecD
{
    u32 state;
    const struct ClassVt *table;
    void *effA;
    void *effD;
    s32 fade;
    s32 timer;
    u32 unk18;
};

struct TransObj1C
{
    u8 filler0[0x1C];
    u32 unk1C;
};

struct GameModeSigned
{
    s8 mode : 5;
};

u32 effect_step_a(void *obj);
u32 effect_step_d(void *obj);
void obj_set_behavior_table(struct TransCamObj *obj, s32 value);
void obj_place_on_screen(struct TransCamObj *obj, s32 *pos);
void *effect_create_sprites(void *obj);
u32 effect_step_c(void *obj);
void obj_release_child_free_b(void *obj, u32 flags);

struct TransChar
{
    struct SpriteObj *sprite;
    s32 x;
    s32 y;
    s32 prevX;
    s32 prevY;
    u16 vx;
    u8 filler16[0x18 - 0x16];
    u16 vspd;
    u8 filler1A[0x25 - 0x1A];
    u8 unk25;
    u8 filler26[0x30 - 0x26];
    const struct TransClass *klass;
    u8 filler34[0x36 - 0x34];
    u16 flags36;
    u8 filler38[0x44 - 0x38];
    s32 limitY;
};

struct TransClass
{
    u8 filler0[0x10];
    s16 thisOffset10;
    u16 filler12;
    void (*fn14)(void *self, void *view, s32 speed, void *cam, u32 seq);
    s16 thisOffset18;
    u16 filler1A;
    void (*fn1C)(void *self, void *cam);
};

struct TransRec
{
    u32 state;
    const struct ClassVt *table;
    struct TransChar *objA;
    struct TransChar *objB;
    s32 timer;
    u8 filler14[4];
    s32 fade;
};

struct TransGame
{
    u8 filler0[0x1C];
    u8 viewA[0x840 - 0x1C];
    u8 viewB[0x1898 - 0x840];
    struct TransChar *pool[0x100];
    struct TransChar *charA;
    struct TransChar *charB;
    u8 filler1CA0[0x1CA4 - 0x1CA0];
    void *effect;
    u8 filler1CA8[0x1CAC - 0x1CA8];
    void *newObj;
    struct TransObj1C *unk1CB0;
    struct TransCamObj *camObj;
    s32 camX;
    s32 camY;
    u8 filler1CC0[0x1CC4 - 0x1CC0];
    u16 flags;
    u8 filler1CC6[0x1F12 - 0x1CC6];
    s16 speed;
    u8 filler1F14[0x1F20 - 0x1F14];
    u8 state;
    u8 filler1F21[0x1F2C - 0x1F21];
    u16 unk1F2C;
};

struct SpriteBit12
{
    u8 filler0[0x12];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct Half36Bit
{
    u16 b0 : 1;
    u16 rest : 15;
};

extern vu16 gUnk_02000050; // shadow of REG_BLDCNT
extern vu16 gUnk_02000054; // shadow of REG_BLDY
extern vu16 gUnk_02000004; // shadow of REG_DISPSTAT
extern vu16 gUnk_02000080;

#define BLDCNT_DARKEN_ALL   0xDF // darken BG0-3 and sprites
#define BLDY_BLACK          0x1F // as dark as it goes
#define DISPSTAT_HBLANK_IRQ 0x10
#define IRQ_HBLANK          0x02 // REG_IE, REG_IF
#define IRQ_SLOT_HBLANK     4    // set_irq_callback

struct LineScroll
{
    s16 y;
    s16 x;
};
extern const u8 gHeapTagTransitionObjDf60[];
extern const u8 gTransitionClassDf60[];
void slot_reset(u32 id);
void scene_release_objects(void *state);
void set_irq_callback(u32 kind, void *cb);
void hblank_iris_window(void);
s32 effect_step_b(void *p);
void obj_move_by_speed(struct TransChar *o, s32 speed);
void obj_update_speed(struct TransChar *o);
void obj_reset_e400(struct TransObj *obj);

static inline void trans_reset_char(struct TransChar *o)
{
    o->sprite->unk0C = 0;
    ((struct SpriteBit12 *)o->sprite)->bit5 = 0;
    o->flags36 &= 0xFFFE;
    o->vx = 0;
}

static inline void trans_track_view(struct TransChar *o, void *view)
{
    s32 vy = (s32)(view_coord((u32)view, o->x) << 16) >> 8;
    o->limitY = vy;
    if (o->y > vy)
    {
        o->y = vy;
        o->vx = o->x - o->prevX;
        o->vspd = vy - o->prevY;
        o->prevX = o->x;
        o->prevY = vy;
    }
    else
    {
        obj_update_speed(o);
    }
}

static inline void trans_table_call(struct TransRec *rec)
{
    const struct ClassVt *t = rec->table;
    t->fn0C((u8 *)rec + t->thisOffset, 3);
}

/* First object of the given kind in the game's object pool, or 0. */
static inline struct TransChar *trans_find_kind(struct TransGame *game, u8 kind)
{
    s32 i;

    for (i = 0; i <= 0xFF; i++)
    {
        if (game->pool[i] != 0 && game->pool[i]->unk25 == kind)
            return game->pool[i];
    }
    return 0;
}

/* Constructor of transition D (see transition_states_transition_step_d): starts it at state 0
 * with its class table, clears the fade and timer, takes the first pool objects of kind 0xC and
 * 0xD as its two effects (effA, effD), sets up the scene effect's sprite pair and copies
 * unk1CB0->unk1C into the scene's unk1F2C. Returns the record. */
struct TransRecD *transition_states_obj_init_scan_pool(struct TransRecD *trans, struct TransGame *scene)
{
    trans->state = 0;
    trans->table = (const struct ClassVt *)gTransitionClassDee0;
    trans->fade = 0;
    trans->timer = 0;
    trans->unk18 = 0;
    trans->effA = trans_find_kind(scene, 0xC);
    trans->effD = trans_find_kind(scene, 0xD);
    effect_init_sprite_pair(scene->effect);
    scene->unk1F2C = scene->unk1CB0->unk1C;
    return trans;
}

static inline u32 trans_char_falling(struct TransChar *o)
{
    s32 y = o->y;
    s32 limit = o->limitY;

    return y < limit;
}

static inline void trans_sync_cam(struct TransChar *o, struct TransGame *game)
{
    const struct TransClass *k = o->klass;

    k->fn1C((u8 *)o + k->thisOffset18, &game->camX);
}

/* One frame of a scene transition: 0-3 fall through (reset both characters, wait 45 frames,
 * wait until both have landed, run the effect, then start the closing animations), 4 waits 65
 * frames and turns on the HBlank iris window, 5 closes the iris, blacks the screen out and ends
 * the scene, starting the next one unless it was the last. Returns 0 once the last has ended. */
u32 transition_state_step(struct TransRec *trans, struct TransGame *scene)
{
    vu16 *reg;

    switch (trans->state)
    {
    case 0:
        scene->flags &= 0xFFF6;
        trans_reset_char(scene->charA);
        trans_reset_char(scene->charB);
        trans->timer = 45;
        trans->state++;
        /* fall through */
    case 1:
        if (--trans->timer > 0)
            goto move;
        trans->state++;
        /* fall through */
    case 2:
        if (trans_char_falling(scene->charA))
            goto move;
        if (trans_char_falling(scene->charB))
            goto move;
        trans->state++;
        /* fall through */
    case 3:
        if (effect_step_b(scene->effect) == 0)
            goto move;
        scene->charA->klass->fn14((u8 *)scene->charA + scene->charA->klass->thisOffset10, scene->viewA, scene->speed, &scene->camX, 0xC);
        scene->charB->klass->fn14((u8 *)scene->charB + scene->charB->klass->thisOffset10, scene->viewB, scene->speed, &scene->camX, 0xC);
        trans->objA->klass->fn14((u8 *)trans->objA + trans->objA->klass->thisOffset10, scene->viewA, scene->speed, &scene->camX, 0xC);
        trans->objB->klass->fn14((u8 *)trans->objB + trans->objB->klass->thisOffset10, scene->viewA, scene->speed, &scene->camX, 0xC);
        trans->timer = 65;
        trans->state++;
        goto move;
    case 4:
        trans->objA->klass->fn14((u8 *)trans->objA + trans->objA->klass->thisOffset10, scene->viewA, scene->speed, &scene->camX, 0);
        trans->objB->klass->fn14((u8 *)trans->objB + trans->objB->klass->thisOffset10, scene->viewA, scene->speed, &scene->camX, 0);
        if (--trans->timer > 0)
            goto move;
        reg = &gUnk_02000050;
        *reg = BLDCNT_DARKEN_ALL;
        reg += 2;
        *reg = BLDY_BLACK;
        trans->fade = 0;
        ((struct LineScroll *)gUnk_03001014)->x = 0;
        /* swap in the iris window's HBlank handler with interrupts off */
        REG_IME = 0;
        gUnk_02000004 &= ~DISPSTAT_HBLANK_IRQ;
        REG_IE &= ~IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        set_irq_callback(IRQ_SLOT_HBLANK, (void *)hblank_iris_window);
        REG_IME = 0;
        gUnk_02000004 |= DISPSTAT_HBLANK_IRQ;
        REG_IE |= IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        trans->state++;
        goto move;
    case 5:
        ((struct LineScroll *)gUnk_03001014)->x = trans->fade >> 8;
        trans->fade += 0x200;
        if (trans->fade <= 0x5000)
            goto move;
        gUnk_02000050 = BLDCNT_DARKEN_ALL;
        REG_BLDCNT = gUnk_02000050;
        gUnk_02000054 = BLDY_BLACK;
        reg = &REG_BLDY;
        *reg = gUnk_02000054;
        gUnk_02000080 = 0;
        scene_release_objects(scene);
        slot_reset(0);
        scene->state++;
        if (scene->state > 5)
        {
            scene->state = 6;
            gUnk_02000054 = 0;
            trans_table_call(trans);
            return 0;
        }
        trans_table_call(trans);
        {
            void *obj = heap_alloc2(8, 1, gHeapTagTransitionObjDf60, 0);

            obj_reset_e400((struct TransObj *)obj);
            ((void **)obj)[1] = (void *)gTransitionClassDf60;
            scene->newObj = obj;
        }
        return 1;
    default:
        break;
    }

move:
    {
        s32 speed = scene->speed;

        obj_move_by_speed(scene->charA, speed);
    }
    {
        s32 speed = scene->speed;

        obj_move_by_speed(scene->charB, speed);
    }
    trans_track_view(scene->charA, scene->viewA);
    trans_track_view(scene->charB, scene->viewB);
    trans_sync_cam(scene->charA, scene);
    trans_sync_cam(scene->charB, scene);
    return 1;
}

static inline void trans_c_table_call(struct TransRecC *rec)
{
    const struct ClassVt *t = rec->table;

    t->fn0C((u8 *)rec + t->thisOffset, 3);
}

static inline void trans_place_cam_obj(struct TransGame *game)
{
    struct CamPos pos;
    s32 *p;

    pos = *(struct CamPos *)&game->charA->x;
    p = &pos.x;
    p[0] -= game->camX;
    p[1] -= game->camY;
    obj_place_on_screen(game->camObj, p);
}

struct GameModeBytes
{
    u8 flags0;
    u8 flags1;
};

/* One frame of a scene transition, like transition_state_step but with a camera object that
 * slides in from the left (state 2) before the effect runs: 0-1 reset both characters and wait
 * 45 frames, 2 slides the camera object across the screen and then sets both characters' closing
 * sprites, 3 runs the effect and turns on the HBlank iris window, 4 closes the iris, 5 waits 60
 * frames, restores (or clears) the blend shadows and ends. Returns 0 once it has ended. */
u32 transition_step_c(struct TransRecC *trans, struct TransGame *scene)
{
    vu16 *reg;
    vu16 *shadow;
    u32 mode;
    u32 done;
    u8 keepBlend;

    switch (trans->state)
    {
    case 0:
        shadow = &scene->flags;
        scene->flags = *shadow & 0xFFF6;
        trans_reset_char(scene->charA);
        trans_reset_char(scene->charB);
        trans->timer = 45;
        trans->state++;
        /* fall through */
    case 1:
        if (--trans->timer > 0)
            goto move;
        trans->state++;
        /* fall through */
    case 2:
        {
            s32 camX = scene->camObj->unk30 + 4;

            obj_set_behavior_table(scene->camObj, camX);
            trans_place_cam_obj(scene);
            if (camX <= 240)
                goto move;
        }
        if (scene->charA->unk25 == 0)
            obj_set_sprite_kind((struct TransObj *)scene->charA, 0xB);
        else
            obj_set_sprite_kind((struct TransObj *)scene->charA, 0xC);
        if (scene->charB->unk25 == 0)
            obj_set_sprite_kind((struct TransObj *)scene->charB, 0xB);
        else
            obj_set_sprite_kind((struct TransObj *)scene->charB, 0xC);
        mode = gGameMode & 0x1F;
        if (mode == 1 || mode == 6)
            trans->aux = effect_create_sprites(heap_alloc2(0x10, 1, gHeapTagTransitionAux, 0));
        else
            effect_init_sprite_pair(scene->effect);
        trans->state++;
        goto move;
    case 3:
        mode = gGameMode & 0x1F;
        if (mode == 1 || mode == 6)
            done = effect_step_c(trans->aux);
        else
            done = effect_step_b(scene->effect);
        if (done == 0)
            goto move;
        reg = &gUnk_02000050;
        *reg = BLDCNT_DARKEN_ALL;
        reg += 2;
        *reg = BLDY_BLACK;
        trans->fade = 0;
        ((struct LineScroll *)gUnk_03001014)->x = 0;
        /* swap in the iris window's HBlank handler with interrupts off */
        REG_IME = 0;
        gUnk_02000004 &= ~DISPSTAT_HBLANK_IRQ;
        REG_IE &= ~IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        set_irq_callback(IRQ_SLOT_HBLANK, (void *)hblank_iris_window);
        REG_IME = 0;
        gUnk_02000004 |= DISPSTAT_HBLANK_IRQ;
        REG_IE |= IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        trans->state++;
        goto move;
    case 4:
        ((struct LineScroll *)gUnk_03001014)->x = trans->fade >> 8;
        trans->fade += 0x400;
        if (trans->fade <= 0x5000)
            goto move;
        trans->timer = 60;
        trans->state++;
        /* fall through */
    case 5:
        if (--trans->timer > 0)
            goto move;
        keepBlend = ((struct GameModeBytes *)&gGameMode)->flags1 & 0x80;
        if (keepBlend)
        {
            vu16 *blend = &gUnk_02000050;

            *blend = 0;
            blend += 2;
            *blend = 0;
        }
        else
        {
            /* black screen: set the shadows and copy them to BLDCNT/BLDY */
            shadow = &gUnk_02000050;
            *shadow = BLDCNT_DARKEN_ALL;
            reg = &REG_BLDCNT;
            *reg = *shadow;
            shadow += 2;
            *shadow = BLDY_BLACK;
            reg += 2;
            *reg = *shadow;
            gUnk_02000080 = keepBlend;
        }
        if (trans->aux != 0)
            obj_release_child_free_b(trans->aux, 3);
        trans_c_table_call(trans);
        return 0;
    }

move:
    {
        s32 speed = scene->speed;

        obj_move_by_speed(scene->charA, speed);
    }
    {
        s32 speed = scene->speed;

        obj_move_by_speed(scene->charB, speed);
    }
    trans_track_view(scene->charA, scene->viewA);
    trans_track_view(scene->charB, scene->viewB);
    trans_sync_cam(scene->charA, scene);
    trans_sync_cam(scene->charB, scene);
    return 1;
}

static inline void trans_d_table_call(struct TransRecD *rec)
{
    const struct ClassVt *t = rec->table;

    t->fn0C((u8 *)rec + t->thisOffset, 3);
}

/* One frame of a scene transition, like transition_state_step but with the record's own two
 * effects: 0-1 reset both characters and wait until both have landed, 2 waits 45 frames and sets
 * both characters' closing sprites, 3 runs the effect (effD in game modes up to 5, else effA) and
 * turns on the HBlank iris window, 4 closes the iris, 5 waits 60 frames, 6 turns the HBlank
 * handler off again, restores (or clears) the blend shadows and ends. Returns 0 once it has ended. */
u32 transition_states_transition_step_d(struct TransRecD *trans, struct TransGame *scene)
{
    vu16 *reg;
    u32 done;
    u8 keepBlend;

    switch (trans->state)
    {
    case 0:
        scene->flags &= 0xFFF6;
        trans_reset_char(scene->charA);
        trans_reset_char(scene->charB);
        trans->state++;
        /* fall through */
    case 1:
        if (trans_char_falling(scene->charA))
            goto move;
        if (trans_char_falling(scene->charB))
            goto move;
        trans->timer = 45;
        trans->state++;
        /* fall through */
    case 2:
        if (--trans->timer > 0)
            goto move;
        if (((struct TransGame *)gGamePtr)->charA->unk25 == 0)
            obj_set_sprite_kind((struct TransObj *)((struct TransGame *)gGamePtr)->charA, 0xD);
        else
            obj_set_sprite_kind((struct TransObj *)((struct TransGame *)gGamePtr)->charA, 0xE);
        if (((struct TransGame *)gGamePtr)->charB->unk25 == 0)
            obj_set_sprite_kind((struct TransObj *)((struct TransGame *)gGamePtr)->charB, 0xD);
        else
            obj_set_sprite_kind((struct TransObj *)((struct TransGame *)gGamePtr)->charB, 0xE);
        trans->state++;
        /* fall through */
    case 3:
        if (((struct GameModeSigned *)&gGameMode)->mode <= 5)
            done = effect_step_d(trans->effD);
        else
            done = effect_step_a(trans->effA);
        if (done == 0)
            goto move;
        trans->fade = 0;
        ((struct LineScroll *)gUnk_03001014)->x = 0;
        /* swap in the iris window's HBlank handler with interrupts off */
        REG_IME = 0;
        gUnk_02000004 &= ~DISPSTAT_HBLANK_IRQ;
        REG_IE &= ~IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        set_irq_callback(IRQ_SLOT_HBLANK, (void *)hblank_iris_window);
        REG_IME = 0;
        gUnk_02000004 |= DISPSTAT_HBLANK_IRQ;
        REG_IE |= IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        trans->state++;
        /* fall through */
    case 4:
        ((struct LineScroll *)gUnk_03001014)->x = trans->fade >> 8;
        trans->fade += 0x400;
        if (trans->fade <= 0x5000)
            goto move;
        trans->timer = 60;
        trans->state++;
        /* fall through */
    case 5:
        if (--trans->timer > 0)
            goto move;
        trans->state++;
        /* fall through */
    case 6:
        /* remove the HBlank handler again */
        set_irq_callback(IRQ_SLOT_HBLANK, 0);
        REG_IME = 0;
        reg = &gUnk_02000004;
        *reg &= ~DISPSTAT_HBLANK_IRQ;
        REG_IE &= ~IRQ_HBLANK;
        REG_IF |= IRQ_HBLANK;
        REG_IME = 1;
        keepBlend = ((struct GameModeBytes *)&gGameMode)->flags1 & 0x80;
        if (keepBlend)
        {
            vu16 *blend = &gUnk_02000050;

            *blend = 0;
            blend += 2;
            *blend = 0;
        }
        else
        {
            /* black screen: set the shadows and copy them to BLDCNT/BLDY */
            reg = &gUnk_02000050;
            *reg = BLDCNT_DARKEN_ALL;
            REG_BLDCNT = *reg;
            reg += 2;
            *reg = BLDY_BLACK;
            REG_BLDY = *reg;
            gUnk_02000080 = keepBlend;
        }
        trans_d_table_call(trans);
        return 0;
    }

move:
    {
        s32 speed = scene->speed;

        obj_move_by_speed(scene->charA, speed);
    }
    {
        s32 speed = scene->speed;

        obj_move_by_speed(scene->charB, speed);
    }
    trans_track_view(scene->charA, scene->viewA);
    trans_track_view(scene->charB, scene->viewB);
    trans_sync_cam(scene->charA, scene);
    trans_sync_cam(scene->charB, scene);
    return 1;
}

void transition_states_obj_destroy(struct TransObj *obj, u32 flags)
{
    obj->unk04 = (s32)gTransitionClassDea0;
    if (obj->unk08.word != 0)
    {
        obj_release_child_free((void *)obj->unk08.word, 3);
    }
    if (obj->unk0C != 0)
    {
        obj_release_child_free_c((void *)obj->unk0C, 3);
    }
    obj->unk04 = (s32)gClassE400;
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct TransObj *obj_init_alloc(struct TransObj *obj)
{
    s32 level;
    void *buf;

    obj->unk00.word = 0;
    obj->unk04 = (s32)gTransitionClassDea0;
    obj->unk10 = 0;
    obj->unk14.word = 0;
    obj->unk08.word = 0;
    obj->unk0C = 0;
    level = ((s32)((u32)gGameMode << 27)) >> 27;
    if (level <= 5)
    {
        buf = heap_alloc2(0x34, 1, gHeapTagObjInitBuf, 0);
        obj->unk0C = effect_init_sprites_b(buf);
    }
    else
    {
        buf = heap_alloc2(0x34, 1, gHeapTagObjInitBuf, 0);
        obj->unk08.word = effect_init_sprites(buf);
    }
    return obj;
}

void obj_update_scroll(struct TransObj *obj, u32 arg)
{
    struct ScrollSub *sub = obj->unk4C;
    u8 *game = gGamePtr;
    s32 *camX = (s32 *)(game + 0x1CB8);
    s32 subX = sub->unk04;
    s32 dx = (subX - *camX) >> 8;
    void *target = *(void **)(game + 0x1CA8);

    win_draw_text(target, arg, dx, 0x50, 1);
}

s32 obj_is_ready(struct TransObj *obj)
{
    if (obj_flush_anim(obj->klass30) != 0)
    {
        return 1;
    }
    return 0;
}

struct TransObj *obj_init_find_type13(struct TransObj *obj)
{
    u8 *game;
    u32 *block;
    s32 i;
    void *found;

    obj->unk00.word = 0;
    obj->unk04 = (s32)gTransitionClassDf00;
    obj->unk08.word = 0;
    obj->unk1C.word = 0;
    obj->klass30 = 0;
    obj->unk38.word = 0;
    game = gGamePtr;
    i = 0;
    block = (u32 *)(game + 0x1898);
    for (; i <= 0xFF; i++)
    {
        if (block[0] != 0 && ((u8 *)block[0])[0x25] == 0x13)
        {
            found = (void *)block[0];
            goto done;
        }
        block++;
    }
    found = 0;
done:
    obj->unk4C = found;
    return obj;
}

void obj_init_basic(struct TransObj *obj)
{
    obj->unk00.word = 0;
    obj->unk04 = (s32)gTransitionClassDf40;
    obj->unk08.word = 0;
    obj->unk0C = 0;
    obj->unk10 = 0x7E00;
    obj->unk14.word = 0;
}

void obj_reset_e400(struct TransObj *obj)
{
    obj->unk04 = (s32)gClassE400;
    obj->unk00.word = 0;
}

void obj_release_child(struct TransObj *obj, u32 flags)
{
    if (obj->unk04 != 0)
    {
        release_obj_131c((void *)obj->unk04);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

struct TransObj *obj_init_timer(struct TransObj *obj)
{
    s32 v = 0x1E;
    s32 timer;

    obj->unk1C.word = v;
    obj->unk0C = 0;
    obj->unk10 = 0x3C00;
    timer = ((s32 (*)(s32))gMathRoutines)(((v << 4) - v) << 12);
    obj->unk14.word = timer;
    obj->unk18.word = 0;
    obj->unk04 = 0;
    obj->unk00.half = 0;
    obj->unk08.half = 0;
    return obj;
}

void obj_update_digits(struct TransObj *obj)
{
    void *rec;
    s32 hundreds;
    s32 tens;
    s32 ones;

    rec = (void *)obj->unk00.word;
    hundreds = obj->unk1C.word / 0x64;
    obj_load_record(rec, hundreds + (s32)obj->klass30, 0, 0, 0);
    rec = (void *)obj->unk04;
    tens = (obj->unk1C.word / 0xA) % 0xA;
    obj_load_record(rec, tens + (s32)obj->klass30, 0, 0, 0);
    rec = (void *)obj->unk08.word;
    ones = obj->unk1C.word % 0xA;
    obj_load_record(rec, ones + (s32)obj->klass30, 0, 0, 0);
}

void obj_clear_field20(struct TransObj *obj)
{
    obj->unk20.word = 0;
}

void obj_add_value(struct TransObj *obj, s16 value)
{
    if (obj->unk1C.word + value < 0)
    {
        obj->unk1C.word = 0;
    }
    else
    {
        obj->unk1C.word = obj->unk1C.word + value;
    }
    if (value < 0)
    {
        obj->unk24.word = 0x64;
        obj->unk20.word = 1;
    }
    else if (value > 0)
    {
        obj->unk24.word = 0x64;
        obj->unk20.word = 2;
    }
    obj_update_digits(obj);
}

s32 obj_get_value(struct TransObj *obj)
{
    return obj->unk1C.half;
}

void obj_set_value(struct TransObj *obj, s16 value)
{
    if (value < 0)
    {
        obj->unk1C.word = 0;
    }
    else
    {
        obj->unk1C.word = value;
    }
    obj_update_digits(obj);
}

void obj_release_all(struct TransObj *obj, u32 flags)
{
    if (obj->unk00.word != 0)
    {
        release_obj_131c((void *)obj->unk00.word);
    }
    if (obj->unk04 != 0)
    {
        release_obj_131c((void *)obj->unk04);
    }
    if (obj->unk08.word != 0)
    {
        release_obj_131c((void *)obj->unk08.word);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

s32 transition_states_obj_move_step(struct TransObj *obj, u32 ctx, u16 speed, u32 unused, u8 mode)
{
    const struct ObjClass *klass;
    s32 step;

    if (mode == 3)
    {
        goto call_fn34;
    }
    if (mode == 4)
    {
        goto call_fn3c;
    }
    obj_state_load_step(obj);
    obj_track_char_a(obj);
    obj->unk14.half = obj->unk14.half + obj->unk1C.uhalf;
    step = (s16)speed;
    obj->unk18.half = step + obj->unk18.half;
    obj->unk04 += obj->unk14.shalf;
    obj->unk08.word += obj->unk18.shalf;
    if (obj->unk08.word > (s32)(view_coord(ctx, obj->unk04) << 16) >> 8)
    {
        obj->unk18.half = 0;
        obj->unk08.word = (s32)(view_coord(ctx, obj->unk04) << 16) >> 8;
    }
    goto done;

call_fn34:
    klass = obj->klass30;
    klass->fn34((u8 *)obj + klass->thisOffset30);
    goto done;

call_fn3c:
    klass = obj->klass30;
    klass->fn3C((u8 *)obj + klass->thisOffset38);

done:
    return 0;
}

s32 obj_byte24_is_zero(struct TransObj *obj)
{
    return obj->unk24.byte == 0;
}

void obj_init_timed_state(struct TransObj *obj)
{
    obj->unk18.half = -*(u16 *)(gGamePtr + 0x1F1C);
    obj_load_record((void *)obj->unk00.word, 2, 0, 0, 0);
    obj->unk34.pair.unk36 = 0xA;
    obj->unk24.byte = 2;
}

void set_class_df80(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gTransitionClassDf80;
    obj_reset(obj, arg);
}

struct TransObj *set_class_df80_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gTransitionClassDf80;
    obj->unk34.pair.kind = kind;
    obj->unk34.pair.unk36 = 0;
    obj->unk38.half = 0;
    obj_create_sprite(obj, 0);
    return obj;
}

s32 transition_states_obj_move_step_b(struct TransObj *obj, u32 ctx, u16 speed, u32 unused, u8 mode)
{
    const struct ObjClass *klass;
    s32 step = speed;
    s32 limit;

    if (mode == 3)
    {
        goto call_fn34;
    }
    if (mode == 4)
    {
        goto call_fn3c;
    }
    obj->unk14.half = obj->unk14.half + obj->unk1C.uhalf;
    step = (s16)step;
    step += obj->unk20.uhalf;
    obj->unk18.half = obj->unk18.half + step;
    obj->unk04 += obj->unk14.shalf;
    obj->unk08.word += obj->unk18.shalf;
    limit = (s32)(view_coord(ctx, obj->unk04) << 16) >> 8;
    if (obj->unk08.word > limit)
    {
        obj->unk08.word = (s32)(view_coord(ctx, obj->unk04) << 16) >> 8;
    }
    goto done;

call_fn34:
    klass = obj->klass30;
    klass->fn34((u8 *)obj + klass->thisOffset30);
    goto done;

call_fn3c:
    klass = obj->klass30;
    klass->fn3C((u8 *)obj + klass->thisOffset38);

done:
    return 0;
}

void set_class_dfc0(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gTransitionClassDfc0;
    obj_reset(obj, arg);
}

struct TransObj *set_class_dfc0_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gTransitionClassDfc0;
    obj->unk34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

void stub_nop_0815939C(void)
{
}

s32 obj_dispatch_mode(struct TransObj *obj, u32 a, u32 b, u32 c, u8 mode)
{
    const struct ObjClass *klass;
    void *self;

    switch (mode)
    {
    case 3:
        klass = obj->klass30;
        self = (u8 *)obj + klass->thisOffset30;
        klass->fn34(self);
        break;
    case 4:
        klass = obj->klass30;
        self = (u8 *)obj + klass->thisOffset38;
        klass->fn3C(self);
        obj->unk24.byte = 1;
        break;
    }
    return 0;
}

void set_class_e040(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gTransitionClassE040;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e040_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gTransitionClassE040;
    obj->unk34.pair.kind = kind;
    obj_create_sprite(obj, 0);
    return obj;
}

void set_class_e000(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gTransitionClassE000;
    obj_reset(obj, arg);
}

struct TransObj *set_class_e000_ex(struct TransObj *obj, u32 arg, u16 kind)
{
    obj_init_from_record(obj, arg);
    obj->klass30 = gTransitionClassE000;
    obj->unk34.pair.kind = kind;
    {
        u32 pos = obj->unk04;
        s32 pos2 = obj->unk08.word;
        obj->unk38.word = pos;
        obj->unk3C.word = pos2;
    }
    obj_create_sprite(obj, 0);
    return obj;
}

void obj_sync_sprite_scroll(struct TransObj *obj, struct CamPos *cam)
{
    struct SpriteObj *sprite;

    sprite_set_pos_rel_cam(obj, cam);
    sprite = obj->unk34.sprite;
    if (sprite != 0)
    {
        s32 camX = cam->x;

        sprite->x = (obj->unk04 - camX) >> 8;
        sprite = obj->unk34.sprite;
        {
            s32 camY = cam->y;

            sprite->y = (((s32)*gUnk_03001014 << 8) - camY) >> 8;
        }
    }
}

void set_class_e080(struct TransObj *obj, u32 arg)
{
    obj->klass30 = gTransitionClassE080;
    obj_reset(obj, arg);
}

s32 obj_mode_update(struct TransObj *obj, u32 ctx, u16 speed, u32 unused, u8 mode)
{
    const struct ObjClass *klass;
    struct SpriteObj *sprite;

    switch (mode)
    {
    case 3:
        klass = obj->klass30;
        klass->fn34((u8 *)obj + klass->thisOffset30);
        obj->unk24.byte = 1;
        break;
    case 4:
        obj_release_sprite(obj);
        break;
    case 0xc:
        obj->unk24.byte = 2;
        break;
    default:
        switch (obj->unk24.byte)
        {
        case 0:
            break;
        case 1:
            obj->unk08.word = (s32)(view_coord(ctx, obj->unk04) << 16) >> 8;
            break;
        case 2:
            obj_set_sprite_kind(obj, 0x3c);
            sprite = obj_get_sprite(obj);
            ((struct Bits12Field *)&sprite->unk12)->mode = 1;
            break;
        }
    }
    return 0;
}
