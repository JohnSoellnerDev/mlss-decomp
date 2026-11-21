#include "global.h"

struct Obj
{
    void *unk00;
    void *klass04;
    void *unk08;
    void *child0C;
    void *unk10;
    void *unk14;
    u8 filler18[0x1A - 0x18];
    u8 unk1A;
};

struct ClassVt
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*fn0C)(void *self, u32 mode);
};

/* A sprite of the transition's banners */
struct Part
{
    u16 unk00;  /* X, px */
    u16 unk02;  /* Y, px */
    u16 unk04;  /* scale (0x100 = full size) */
};

/* The scene transition's banner: five sprites side by side, 16 px apart, moved by obj_anim_e620. */
struct Anim
{
    u16 unk00;          /* frames it has waited on screen */
    u8 filler02[2];
    void *child04;      /* the five parts (struct Part), left to right */
    void *child08;
    void *child0C;
    void *child10;
    void *child14;
    u8 filler18[0x36 - 0x18];
    s16 state36;        /* 0 sliding in, 1 waiting, 2 sliding out */
    s32 pos38;          /* X of the leftmost part, 8.8 */
    u8 filler3C[4];
    s32 speed40;        /* X speed, 8.8 px a frame */
    u8 filler44[0x48 - 0x44];
    s32 accel48;
};

/* what obj_anim_e620 returns */
enum BannerResult
{
    BANNER_BUSY = 0,
    BANNER_GONE = 2,    /* it has left the screen and its parts are released */
    BANNER_LEAVING = 3, /* the wait is over, it starts to slide out (returned once) */
};

struct Obj9
{
    void *unk00;
    void *unk04;
    void *unk08;
    void *unk0C;
    void *unk10;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    void *unk24;
};

/* The second banner: nine sprites in a row (a gap after the fourth) that pop up one after the
   other, growing to full size (obj_timer_e7c8). */
struct Group9
{
    u16 unk00;          /* frames left, from 1000 */
    u8 filler02[2];
    void *parts04[9];   /* struct Part, left to right */
    u16 unk28[9];       /* the part has reached full size */
    u16 unk3A[9];       /* frames before the part starts to grow */
    u16 unk4C[9];       /* the part's growth phase (gCosineTable index, +4 a frame) */
};

extern const u8 gClassE440[];
extern const u8 gTransitionTagPhaz[];
extern const u8 gClassE460[];

void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void release_obj_131c(void *p);
void sprite_update_cd0(void *sprite);
void heap_free_b(void *p);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
s32 transition_scene_step(u8 *game);
void *transition_children_obj_init_scan_pool(struct Obj *obj, u8 *game);
void obj_reset_e920(struct Obj *obj);
void obj_init_e4a0(struct Obj *obj);
void obj_release_slots(void *sprite);

extern const u8 gReleaseVtableE920[];
extern const u8 gClassE4a0[];
extern const u8 gClassE480[];

struct GameModeFlags
{
    u16 mode : 5;
    u16 bit5 : 1;
    u16 bit6 : 1;
    u16 sub7 : 3;
    u16 sub10 : 5;
    u16 bit15 : 1;
};

struct LayerSetupRec
{
    u8 filler0[0x1C];
    u16 unk1C;
    u8 filler1E[2];
};

struct TransChar
{
    void *sprite;
    s32 posX;
    s32 posY;
    u8 filler0C[0x14 - 0x0C];
    s16 speedX;
    u8 filler16[0x24 - 0x16];
    u8 state;
    u8 filler25;
};

/* Iris-in step of the scene transition: once the banner has dropped in, both heroes start
   running and a circle closes on them over 30 frames. */
struct TransIrisStep
{
    s32 state;
    const struct ClassVt *klass04;
    s32 unk08;
    s32 timer;          /* frames of the iris left */
    s32 radius;         /* 8.8 */
    s32 unk14;
    s32 stepX;          /* added to the iris offset each frame */
    s32 stepY;
    s32 shrink;         /* taken off the radius each frame */
    s32 bg2Priority;    /* BG2CNT priority before the iris */
};

/* The scene's hblank circle (code_081599D8 CamObj): centred on a point plus this offset. */
struct TransIris
{
    u8 filler0[0x508];
    s32 offsetX;
    s32 offsetY;
};

struct TransScene
{
    u8 filler0[0x1C98];
    struct TransChar *heroA;
    struct TransChar *heroB;
    u8 filler1CA0[0x1CB4 - 0x1CA0];
    u16 flags;
    u8 filler1CB6[0x1CBC - 0x1CB6];
    struct TransIris *iris;
    u8 filler1CC0[0x1E88 - 0x1CC0];
    struct Anim *banner;        /* the five-part banner obj_anim_e620 moves */
    struct Obj9 *group9;        /* nine more sprites (released by obj_release_children_9) */
    u8 filler1E90[0x1E94 - 0x1E90];
    void *step;                 /* the transition step running now */
};

#define BG2CNT_SHADOW (*(vu16 *)&gUnk_0200000C)
#define DISPCNT_SHADOW (*(u16 *)EWRAM_START)

extern const struct LayerSetupRec gSceneLayerSetupTable[];
extern u8 *gUnk_03001020;
extern u8 gGameMode;
extern u16 gUnk_0200000C;

void obj_set_sprite_kind(void *obj, s16 kind);
void cam_set_factor(void *cam, s32 factor);
void cam_build_line_table(void *cam, void *arg);

static inline void trans_char_start(struct TransChar *obj)
{
    if (obj->filler25 == 0)
        obj->speedX = gSceneLayerSetupTable[*((u8 *)gUnk_03001020 + 0x1EA2)].unk1C;
    else
        obj->speedX = gSceneLayerSetupTable[*((u8 *)gUnk_03001020 + 0x1EA2)].unk1C - 0x64;
    obj->state = 2;
}

ASM_FUNC("asm/code_0815E288/sprite_group5_init.s", void sprite_group5_init(void))

ASM_FUNC("asm/code_0815E288/sprite_group5_update.s", void sprite_group5_update(void))

ASM_FUNC("asm/code_0815E288/obj_create_group_e4a0.s", void obj_create_group_e4a0(void))

/*
 * Moves the transition banner one frame: it slides in with its speed falling by accel48 a frame
 * until the speed turns negative, waits 60 frames (then returns BANNER_LEAVING), and slides out
 * speeding up until it is past X 240, when its parts are released (BANNER_GONE).
 */
s32 obj_anim_e620(struct Anim *banner)
{
    void **parts;

    switch (banner->state36)
    {
    case 0:
        banner->speed40 = banner->speed40 - banner->accel48;
        banner->pos38 = banner->pos38 + banner->speed40;
        ((struct Part *)banner->child04)->unk00 = banner->pos38 >> 8;
        ((struct Part *)banner->child08)->unk00 = (banner->pos38 >> 8) + 0x10;
        ((struct Part *)banner->child0C)->unk00 = (banner->pos38 >> 8) + 0x20;
        ((struct Part *)banner->child10)->unk00 = (banner->pos38 >> 8) + 0x30;
        ((struct Part *)banner->child14)->unk00 = (banner->pos38 >> 8) + 0x40;
        if (banner->speed40 < 0)
        {
            banner->unk00 = 0;
            banner->state36 = 1;
        }
        return BANNER_BUSY;
    case 1:
        banner->unk00++;
        if ((s16)banner->unk00 > 59)
        {
            banner->unk00 = 0;
            banner->state36 = 2;
            return BANNER_LEAVING;
        }
        return BANNER_BUSY;
    case 2:
        banner->speed40 = banner->speed40 + banner->accel48;
        banner->pos38 = banner->pos38 + banner->speed40;
        parts = &banner->child04;
        ((struct Part *)banner->child04)->unk00 = banner->pos38 >> 8;
        ((struct Part *)parts[1])->unk00 = (banner->pos38 >> 8) + 0x10;
        ((struct Part *)banner->child0C)->unk00 = (banner->pos38 >> 8) + 0x20;
        ((struct Part *)banner->child10)->unk00 = (banner->pos38 >> 8) + 0x30;
        ((struct Part *)banner->child14)->unk00 = (banner->pos38 >> 8) + 0x40;
        if (banner->pos38 > 0xEFFF) /* past X 240 */
        {
            obj_release_slots(banner->child04);
            obj_release_slots(parts[1]);
            obj_release_slots(banner->child0C);
            obj_release_slots(banner->child10);
            obj_release_slots(banner->child14);
            return BANNER_GONE;
        }
        return BANNER_BUSY;
    }
    return BANNER_BUSY;
}

/* Sets up the transition's second banner: nine sprites (records 10-18) 16 px apart from X 0x28 at
   Y 0x3C, with a 16 px gap after the fourth, each waiting 5 frames longer than the one before it
   to grow in; the banner lasts 1000 frames. */
struct Group9 *obj_create_group_e714(struct Group9 *group)
{
    s32 index;
    s32 next;
    s32 none;
    u32 zero;
    void *part;

    index = 0;
    none = -1;
    do
    {
        part = create_obj_de4(4, 0x5005, 0, none, none, none, none);
        group->parts04[index] = part;
        zero = 0;
        if (index > 3)
        {
            ((struct Part *)part)->unk00 = ((index + 1) << 4) + 0x28;
            next = index + 1;
        }
        else
        {
            ((struct Part *)part)->unk00 = (index << 4) + 0x28;
            next = index + 1;
        }
        ((struct Part *)group->parts04[index])->unk02 = 0x3C;
        obj_load_record(group->parts04[index], index + 0xA, -1, 0, zero);
        group->unk28[index] = zero;
        group->unk3A[index] = (index << 2) + index; /* index * 5 */
        group->unk4C[index] = zero;
        index = next;
    } while (index <= 8);
    group->unk00 = 1000;
    return group;
}

ASM_FUNC("asm/code_0815E288/obj_timer_e7c8.s", void obj_timer_e7c8(void))

ASM_FUNC("asm/code_0815E288/obj_create_group_e890.s", void obj_create_group_e890(void))

ASM_FUNC("asm/code_0815E288/obj_timer_ea04.s", void obj_timer_ea04(void))

ASM_FUNC("asm/code_0815E288/transition_setup.s", void transition_setup(void))

/* Transition step: shows the banner, and when it starts to leave, sets both heroes running
   and closes an iris on hero A over 30 frames, its centre drifting from the screen centre
   towards hero B (hero A's sprite turns to follow it). Once the banner is gone, hands over to
   the next step (class E480). */
s32 transition_children_transition_step_b(struct TransIrisStep *step, struct TransScene *scene)
{
    struct Anim *banner;
    void **parts;
    s32 offsetX;
    const struct ClassVt *cls;
    struct Obj *next;

    transition_scene_step((u8 *)scene);
    switch (step->state)
    {
    case 0:
        DISPCNT_SHADOW &= ~DISPCNT_BG2_ON;
        step->unk08 = 0;
        step->state++;
    case 1:
        banner = scene->banner;
        parts = &banner->child04;
        sprite_update_cd0(banner->child04);
        sprite_update_cd0(parts[1]);
        sprite_update_cd0(banner->child0C);
        sprite_update_cd0(banner->child10);
        sprite_update_cd0(banner->child14);
        step->state++;
    case 2:
        if (obj_anim_e620(scene->banner) != BANNER_LEAVING) /* still sliding in or waiting */
            break;
        trans_char_start(scene->heroA);
        trans_char_start(scene->heroB);
        if (((struct GameModeFlags *)&gGameMode)->bit5)
            DISPCNT_SHADOW |= DISPCNT_BG2_ON;
        step->bg2Priority = BG2CNT_SHADOW & BGCNT_PRIORITY_MASK;
        if (((struct GameModeFlags *)&gGameMode)->sub7 == 3)
            BG2CNT_SHADOW &= ~BGCNT_PRIORITY_MASK;
        step->stepX = (((struct TransScene *)gUnk_03001020)->heroB->posX - 0x7800) / 30;
        step->stepY = (((struct TransScene *)gUnk_03001020)->heroB->posY - 0x5000) / 30;
        step->radius = 0x7E00;
        step->shrink = 0x288;
        step->timer = 30;
        step->state++;
    case 3:
        obj_anim_e620(scene->banner);
        if (--step->timer > 0)
        {
            scene->flags |= 1;
            cam_set_factor(scene->iris, step->radius >> 8);
            cam_build_line_table(scene->iris, &scene->heroA->posX);
            scene->iris->offsetX += step->stepX;
            scene->iris->offsetY += step->stepY;
            step->radius -= step->shrink;
            if (step->radius <= 0x3200)
                step->radius = 0x3200;
            offsetX = scene->iris->offsetX >> 8;
            if (offsetX > 10)
                obj_set_sprite_kind(scene->heroA, 0);
            else if (offsetX > 4)
                obj_set_sprite_kind(scene->heroA, 4);
            else if (offsetX > -5)
                obj_set_sprite_kind(scene->heroA, 5);
            else
                obj_set_sprite_kind(scene->heroA, 6);
            break;
        }
        step->state++;
    case 4:
        if (obj_anim_e620(scene->banner) == 2) /* banner gone */
        {
            cls = step->klass04;
            cls->fn0C((u8 *)step + cls->thisOffset, 3);
            next = heap_alloc2(8, 1, gTransitionTagPhaz, 0);
            obj_reset_e920(next);
            next->klass04 = (void *)gClassE480;
            scene->step = next;
        }
        break;
    }
    return 1;
}

s32 transition_spawn_child(struct Obj *obj, u8 *game)
{
    struct ClassVt *cls;
    void *block;
    s32 mode;

    if (obj->unk00 == 0)
    {
        mode = transition_scene_step(game);
        if (mode == 1)
        {
            cls = obj->klass04;
            cls->fn0C((u8 *)obj + cls->thisOffset, 3);
            *(void **)(game + 0x1E94) = transition_children_obj_init_scan_pool(heap_alloc2(0x1C, 1, gTransitionTagPhaz, 0), game);
        }
        else if (mode == 2)
        {
            cls = obj->klass04;
            cls->fn0C((u8 *)obj + cls->thisOffset, 3);
            block = heap_alloc2(0x18, 1, gTransitionTagPhaz, 0);
            obj_reset_e920(block);
            ((struct Obj *)block)->klass04 = (void *)gClassE440;
            *(void **)(game + 0x1E94) = block;
        }
    }
    return 1;
}

struct Entry25
{
    u8 filler0[0x25];
    u8 type25;
};

struct PoolGame
{
    u8 filler0[0x1898];
    struct Entry25 *pool[0x100];
};

/* First object of the given kind in the game's object pool, or 0. */
static inline struct Entry25 *pool_find_kind(struct PoolGame *game, u8 kind)
{
    s32 i;

    for (i = 0; i <= 0xFF; i++)
    {
        if (game->pool[i] != 0 && game->pool[i]->type25 == kind)
            return game->pool[i];
    }
    return 0;
}

/* Sets up the transition step of class E460 (transition_spawn_child makes it on mode 1): clears
   its fields, finds the scene's pool objects of kind 0xC and 0xD, and shows (sprite_update_cd0)
   each of the nine sprites of the scene's group9 that exists. Returns the step. */
void *transition_children_obj_init_scan_pool(struct Obj *step, u8 *game)
{
    struct Obj9 *group;
    void **sprites;

    step->unk00 = 0;
    step->klass04 = (void *)gClassE460;
    step->unk10 = 0;
    step->unk14 = 0;
    *(void **)&step->filler18 = 0;
    step->unk08 = pool_find_kind((struct PoolGame *)game, 0xC);
    step->child0C = pool_find_kind((struct PoolGame *)game, 0xD);
    group = ((struct TransScene *)game)->group9;
    sprites = &group->unk04;
    if (group->unk04 != 0)
        sprite_update_cd0(group->unk04);
    if (sprites[1] != 0)
        sprite_update_cd0(sprites[1]);
    if (group->unk0C != 0)
        sprite_update_cd0(group->unk0C);
    if (group->unk10 != 0)
        sprite_update_cd0(group->unk10);
    if (group->unk14 != 0)
        sprite_update_cd0(group->unk14);
    if (group->unk18 != 0)
        sprite_update_cd0(group->unk18);
    if (group->unk1C != 0)
        sprite_update_cd0(group->unk1C);
    if (group->unk20 != 0)
        sprite_update_cd0(group->unk20);
    if (group->unk24 != 0)
        sprite_update_cd0(group->unk24);
    return step;
}
ASM_FUNC("asm/code_0815E288/transition_children_transition_step_d.s", void transition_children_transition_step_d(void))

ASM_FUNC("asm/code_0815E288/transition_step_e.s", void transition_step_e(void))

void obj_init_e4a0(struct Obj *obj)
{
    obj->unk00 = 0;
    obj->klass04 = (void *)gClassE4a0;
    obj->unk08 = 0;
    obj->child0C = 0;
    obj->unk10 = (void *)0x7E00;
    obj->unk14 = 0;
}

void obj_reset_e920(struct Obj *obj)
{
    obj->klass04 = (void *)gReleaseVtableE920;
    obj->unk00 = 0;
}

void obj_free_slots_flag(struct Obj *obj, u32 flags)
{
    if (obj->unk00 != 0)
        release_obj_131c(obj->unk00);
    if (obj->klass04 != 0)
        release_obj_131c(obj->klass04);
    if (obj->unk08 != 0)
        release_obj_131c(obj->unk08);
    if (obj->child0C != 0)
        release_obj_131c(obj->child0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_free_ptr_fields_d(struct Obj9 *obj)
{
    void **p = &obj->unk04;

    if (obj->unk04 != 0)
        sprite_update_cd0(obj->unk04);
    if (p[1] != 0)
        sprite_update_cd0(p[1]);
    if (obj->unk0C != 0)
        sprite_update_cd0(obj->unk0C);
    if (obj->unk10 != 0)
        sprite_update_cd0(obj->unk10);
    if (obj->unk14 != 0)
        sprite_update_cd0(obj->unk14);
    if (obj->unk18 != 0)
        sprite_update_cd0(obj->unk18);
    if (obj->unk1C != 0)
        sprite_update_cd0(obj->unk1C);
    if (obj->unk20 != 0)
        sprite_update_cd0(obj->unk20);
    if (obj->unk24 != 0)
        sprite_update_cd0(obj->unk24);
}

void obj_release_children_9(struct Obj9 *obj, u32 flags)
{
    void **p = &obj->unk04;

    if (obj->unk04 != 0)
        release_obj_131c(obj->unk04);
    if (p[1] != 0)
        release_obj_131c(p[1]);
    if (obj->unk0C != 0)
        release_obj_131c(obj->unk0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (obj->unk14 != 0)
        release_obj_131c(obj->unk14);
    if (obj->unk18 != 0)
        release_obj_131c(obj->unk18);
    if (obj->unk1C != 0)
        release_obj_131c(obj->unk1C);
    if (obj->unk20 != 0)
        release_obj_131c(obj->unk20);
    if (obj->unk24 != 0)
        release_obj_131c(obj->unk24);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_free_ptr_fields_a(struct Obj *obj)
{
    void **p = &obj->klass04;

    sprite_update_cd0(obj->klass04);
    sprite_update_cd0(p[1]);
    sprite_update_cd0(obj->child0C);
    sprite_update_cd0(obj->unk10);
    sprite_update_cd0(obj->unk14);
}

void obj_release_children_5(struct Obj *obj, u32 flags)
{
    void **p = &obj->klass04;

    if (obj->klass04 != 0)
        release_obj_131c(obj->klass04);
    if (p[1] != 0)
        release_obj_131c(p[1]);
    if (obj->child0C != 0)
        release_obj_131c(obj->child0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (obj->unk14 != 0)
        release_obj_131c(obj->unk14);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_free_ptr_fields_b(struct Obj *obj)
{
    void **p = &obj->klass04;

    sprite_update_cd0(obj->klass04);
    sprite_update_cd0(p[1]);
    sprite_update_cd0(obj->child0C);
    sprite_update_cd0(obj->unk10);
    sprite_update_cd0(obj->unk14);
}

void obj_free_ptr_fields_c(struct Obj *obj, u32 flags)
{
    void **p = &obj->klass04;

    if (obj->klass04 != 0)
        release_obj_131c(obj->klass04);
    if (p[1] != 0)
        release_obj_131c(p[1]);
    if (obj->child0C != 0)
        release_obj_131c(obj->child0C);
    if (obj->unk10 != 0)
        release_obj_131c(obj->unk10);
    if (obj->unk14 != 0)
        release_obj_131c(obj->unk14);
    if (flags & 1)
        heap_free_b(obj);
}

void obj_set_child_flag20(struct Obj *obj)
{
    ((u8 *)obj->child0C)[0x12] |= 0x20;
    obj->unk1A = 0;
}
