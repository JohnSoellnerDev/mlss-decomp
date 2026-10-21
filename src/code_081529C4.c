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

struct CharObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[0x36 - 0xC];
    u16 unk36;
};

struct CamTarget
{
    u8 filler0[0xC];
    s32 ofsX;
};

struct GameState
{
    u8 filler0[0x1898];
    void *list1898[0x100];
    struct CharObj *charA;
    struct CharObj *charB;
    void *unk1CA0;
    void *unk1CA4;
    void *unk1CA8;
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

struct Pair
{
    s32 unk00;
    s32 unk04;
};

struct ModeTable
{
    u8 filler0[0x30];
    s16 off30;
    u8 filler32[0x34 - 0x32];
    void (*fn34)(void *);
};

struct ModeObj
{
    u8 filler0[0x18];
    u16 unk18;
    u8 filler1A[0x24 - 0x1A];
    u8 unk24;
    u8 unk25;
    u8 filler26[0x30 - 0x26];
    struct ModeTable *table;
    u8 filler34[0x36 - 0x34];
    u16 unk36;
    u16 unk38;
    u8 filler3A[0x42 - 0x3A];
    u8 unk42;
};

extern const struct SceneStateRow gSceneStateTable_083CCAC0[];
extern struct GameState *gGamePtr;

void *obj_get_sprite(void *obj);
void obj_add_value(void *obj, s32 value);
u32 obj_tick_state(void *obj, void *ctx, s16 speed, u32 arg3);

ASM_FUNC("asm/code_081529C4/scene_release_objects.s", void scene_release_objects(void))

ASM_FUNC("asm/code_081529C4/transition_scene_update.s", void transition_scene_update(void))

void game_update_camera(struct GameState *game)
{
    struct Pair pos;
    s32 *p;
    s32 ofsX;

    if (game->flags & 8)
    {
        cam_update(game->camTarget, 1);
        if (((game->charA->unk36 ^ 1) & 1) != 0)
        {
            ofsX = game->camTarget->ofsX >> 8;
            if (ofsX > 10)
            {
                obj_set_sprite_kind(game->charA, 0);
            }
            else if (ofsX > 4)
            {
                obj_set_sprite_kind(game->charA, 4);
            }
            else if (ofsX > -5)
            {
                obj_set_sprite_kind(game->charA, 5);
            }
            else
            {
                obj_set_sprite_kind(game->charA, 6);
            }
        }
    }

    pos = *(struct Pair *)&game->charA->x;
    p = &pos.unk00;
    p[0] -= game->camX;
    p[1] -= game->camY;
    obj_place_on_screen(game->camTarget, p);
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

ASM_FUNC("asm/code_081529C4/obj_create.s", void obj_create(void))

NONMATCH("asm/code_081529C4/game_set_chars_active.s", void game_set_chars_active(struct GameState *game, u32 on))
{
    if (on != 0)
    {
        game->flags = (game->flags & 0xFFF6) | 0x10;
        obj_clear_offset_14(game->charA);
        obj_clear_offset_14(game->charB);
    }
    else
    {
        game->flags = (game->flags | 9) & 0xFFEF;
        obj_set_char_offset(game->charA);
        obj_set_char_offset(game->charB);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_081529C4/scene_update_screen.s", void scene_update_screen(void))

NONMATCH("asm/code_081529C4/obj_mode_switch.s", u32 obj_mode_switch(struct ModeObj *obj, void *ctx, u16 speed, u32 arg3, u8 mode))
{
    switch (mode)
    {
    case 1:
    {
        struct ModeTable *table = obj->table;

        table->fn34((u8 *)obj + table->off30);
        break;
    }
    case 2:
        break;
    case 5:
    {
        void *sprite;

        if (((obj->unk36 ^ 1) & 1) != 0)
        {
            obj->unk36 = obj->unk36 | 1;
            obj->unk38 = 0x64;
            sprite = obj_get_sprite(obj);
            obj->unk42 = ((u8 *)sprite)[0x21];
            if (obj->unk25 == 0)
            {
                obj_set_sprite_kind(obj, 0x28);
            }
            else
            {
                obj_set_sprite_kind(obj, 0x29);
            }
            obj_add_value(gGamePtr->mainObj, -3);
        }
        break;
    }
    case 13:
        obj->unk24 = 8;
        break;
    case 12:
        obj->unk18 = 0xFC18;
        obj->unk24 = 2;
        break;
    default:
        return obj_tick_state(obj, ctx, speed, arg3);
    }
    return 0;
}
END_NONMATCH

ASM_FUNC("asm/code_081529C4/obj_tick_state.s", u32 obj_tick_state(void *obj, void *ctx, s16 speed, u32 arg3))

ASM_FUNC("asm/code_081529C4/obj_mode_step.s", void obj_mode_step(void))

ASM_FUNC("asm/code_081529C4/obj_mode_step_alt.s", void obj_mode_step_alt(void))

ASM_FUNC("asm/code_081529C4/obj_behavior_e220.s", void obj_behavior_e220(void))
