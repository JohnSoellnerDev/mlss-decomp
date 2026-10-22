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

struct StepObj
{
    u8 filler0[4];
    s32 unk04;
    s32 unk08;
    u8 filler0C[0x14 - 0x0C];
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    u16 unk18;
    u8 filler1A[0x1C - 0x1A];
    u16 unk1C;
    u8 filler1E[0x20 - 0x1E];
    u16 unk20;
    u8 filler22[0x24 - 0x22];
    u8 unk24;
    u8 filler25[0x2D - 0x25];
    u8 unk2D;
    u8 filler2E[0x30 - 0x2E];
    struct ModeTable *table;
    u8 filler34[0x36 - 0x34];
    u16 unk36;
    s32 unk38;
    u8 filler3C[0x40 - 0x3C];
    struct StepSub *sub;
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

struct ReleaseTable
{
    u8 filler0[8];
    s16 off8;
    u8 filler0A[0xC - 0xA];
    void (*release)(void *obj, u32 mode);
};

struct ReleaseObj
{
    u8 filler0[0x30];
    struct ReleaseTable *table;
};

extern const struct SceneStateRow gSceneStateTable_083CCAC0[];
extern struct GameState *gGamePtr;

void *obj_get_sprite(void *obj);
void obj_add_value(void *obj, s32 value);
u32 obj_tick_state(void *obj, void *ctx, s16 speed, u32 arg3);

void scene_release_objects(struct GameState *state)
{
    s32 i;
    struct ReleaseObj *obj;
    struct ReleaseTable *table;

    for (i = 0; i < 0x100; i++)
    {
        obj = state->list1898[i];
        if (obj != 0)
        {
            table = obj->table;
            table->release((u8 *)obj + table->off8, 3);
        }
        state->list1898[i] = 0;
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
    if (state->unk1CA8 != 0)
    {
        window_destroy(state->unk1CA8, 3);
        state->unk1CA8 = 0;
    }
    obj_free_tile_bufs((u8 *)state + 0x1C);
    obj_free_tile_bufs((u8 *)state + 0x840);
    obj_free_tile_bufs((u8 *)state + 0x1064);
}

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

void *obj_create(void *unused, struct ObjRecord *record)
{
    void *obj;
    const void *tag;
    struct CreateTable *table;

    switch (record->kind)
    {
    case 0:
        tag = gHeapTagObjKind0;
        goto generic;
    case 1:
        tag = gHeapTagObjKind1;
    generic:
        obj = obj_create_e3c0(heap_alloc2(0x48, 1, tag, 0), record, 0);
        table = ((struct CreateObj *)obj)->table;
        table->fn14((u8 *)obj + table->off10, 0, 0, 0, 1);
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
        return 0;
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

u32 obj_mode_step(struct StepObj *obj, void *ctx, u16 speed, u32 arg3, u8 mode)
{
    struct ModeTable *table;
    s32 t;
    s32 diff;
    s32 view;
    s32 accel;

    switch (mode)
    {
    case 1:
    case 2:
        break;
    case 3:
        table = obj->table;
        table->fn34((u8 *)obj + table->off30);
        obj->unk24 = 1;
        break;
    case 4:
        table = obj->table;
        table->fn3C((u8 *)obj + table->off38);
        obj->sub->bit5 = 1;
        obj->unk24 = 0;
        break;
    default:
        view = view_coord(ctx, obj->unk04);
        obj->unk38 = (s32)((u32)view << 16) >> 8;
        switch (obj->unk24)
        {
        case 0:
        case 4:
            break;
        case 1:
            diff = obj->unk04 - gGamePtr->charA->x;
            if (diff <= 0x81FF)
            {
                obj->unk36 = 0xF;
                obj_set_sprite_kind(obj, 0x19);
                obj->unk24 = 2;
            }
            break;
        case 2:
            t = obj->unk36;
            t--;
            obj->unk36 = t;
            if ((s16)t > 0)
            {
                break;
            }
            obj_set_sprite_kind(obj, 0x1A);
            obj->unk14 += obj->unk1C;
            accel = (s16)speed;
            accel += obj->unk20;
            obj->unk18 += accel;
            obj->unk04 += (s16)obj->unk14;
            obj->unk08 += (s16)obj->unk18;
            if (obj->unk08 > obj->unk38)
            {
                set_slot_value(0x38, -1);
                ((struct SpriteBits1F *)obj_get_sprite(obj))->lo2 = obj->unk2D;
                ((struct SpriteBits1F *)obj_get_sprite(obj))->b2 = obj->unk2D;
                ((struct SpriteBits1F *)obj_get_sprite(obj))->b4 = obj->unk2D;
                obj->sub->bit5 = 0;
                obj_set_vy_if_free(gGamePtr->camTarget, (s16)obj->unk18);
                obj->unk36 = 0x14;
                obj->unk24 = 3;
            }
            break;
        case 3:
            t = obj->unk36;
            t--;
            obj->unk36 = t;
            if ((s16)t > 0)
            {
                break;
            }
            obj->sub->bit5 = 1;
            obj->unk24 = 4;
            break;
        default:
            break;
        }
        break;
    }
    return 0;
}

u32 obj_mode_step_alt(struct StepObj *obj, void *ctx, u16 speed, u32 arg3, u8 mode)
{
    struct ModeTable *table;
    s32 t;
    s32 diff;
    s32 view;
    s32 accel;

    switch (mode)
    {
    case 1:
    case 2:
        break;
    case 3:
        table = obj->table;
        table->fn34((u8 *)obj + table->off30);
        obj->unk24 = 1;
        break;
    case 4:
        table = obj->table;
        table->fn3C((u8 *)obj + table->off38);
        obj->sub->bit5 = 1;
        obj->unk24 = 0;
        break;
    default:
        view = view_coord(ctx, obj->unk04);
        obj->unk38 = (s32)((u32)view << 16) >> 8;
        switch (obj->unk24)
        {
        case 0:
        case 4:
            break;
        case 1:
            diff = obj->unk04 - gGamePtr->charB->x;
            if (diff <= 0x81FF)
            {
                obj->unk36 = 0xF;
                obj_set_sprite_kind(obj, 0x19);
                obj->unk24 = 2;
            }
            break;
        case 2:
            t = obj->unk36;
            t--;
            obj->unk36 = t;
            if ((s16)t > 0)
            {
                break;
            }
            obj_set_sprite_kind(obj, 0x1A);
            obj->unk14 += obj->unk1C;
            accel = (s16)speed;
            accel += obj->unk20;
            obj->unk18 += accel;
            obj->unk04 += (s16)obj->unk14;
            obj->unk08 += (s16)obj->unk18;
            if (obj->unk08 > obj->unk38)
            {
                set_slot_value(0x38, -1);
                obj->sub->bit5 = 0;
                obj->unk36 = 0x14;
                obj->unk24 = 3;
            }
            break;
        case 3:
            t = obj->unk36;
            t--;
            obj->unk36 = t;
            if ((s16)t > 0)
            {
                break;
            }
            obj->sub->bit5 = 1;
            obj->unk24 = 4;
            break;
        default:
            break;
        }
        break;
    }
    return 0;
}

ASM_FUNC("asm/code_081529C4/obj_behavior_e220.s", void obj_behavior_e220(void))
