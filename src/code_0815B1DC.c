#include "global.h"

struct LayerSetup
{
    u8 filler0[0x18];
    u16 unk18;
    u8 filler1A[0x20 - 0x1A];
};

extern const struct LayerSetup gSceneLayerSetupTable[];

struct CamChar
{
    u32 unk00;
    s32 x;
    s32 y;
};

struct ClassObj
{
    u8 filler0[0x18];
    u16 vy;
    u8 filler1A[0x24 - 0x1A];
    u8 state;
    u8 kind;
    u8 filler26[0x2C - 0x26];
    u8 byte2C;
    u8 byte2D;
    u8 filler2E[0x30 - 0x2E];
    void *klass;
    u16 classIdx;
    u16 timer;
    u32 angle;
    struct SpriteObj *childA;
    struct SpriteObj *childB;
};

struct SpriteObj
{
    u8 filler0[0x0E];
    u16 counter;
    u8 filler10[0x12 - 0x10];
    u8 flags12;
    u8 filler13[0x1F - 0x13];
    u8 lo2 : 2;
    u8 b2 : 2;
    u8 b4 : 2;
    u8 b6 : 2;
    u8 filler20;
    u8 kind;
};

struct ObjClassVt
{
    u8 filler0[0x30];
    s16 thisOffset30;
    u16 filler32;
    void (*fn34)(void *self, void *fn);
};

struct ClassVt
{
    u8 filler0[8];
    s16 thisOffset;
    u16 filler0A;
    void (*fn0C)(void *self, u32 mode);
};

struct PoolEntry
{
    u8 filler0[0x30];
    struct ClassVt *table;
};

struct GameVars
{
    u8 filler0[0x1898];
    struct PoolEntry *unk1898[0x100];
};

extern u8 gClassE7a0[];
extern u8 gClassE7e0[];
extern u8 *gUnk_03001020;
extern u8 gSys[];
extern u8 gUnk_0203FFB8[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u16 gUnk_02000010;
extern u16 gUnk_02000012;
extern u16 gUnk_02000014;
extern u16 gUnk_02000016;
extern u16 gUnk_0200001C;
extern u16 gUnk_0200001E;
extern u16 gUnk_02000050;
void update_obj_list(void);
void obj_list_pass_0a8c(void);
void obj_scroll_tile_ring(u8 *game, s32 x, s32 y);
extern const u8 gSlotTable[];
u32 mt19937_next(void);
u32 obj_step_by_state(struct ClassObj *obj, u32 view, s16 speed, u32 cmd_arg);
s32 rand_below(s32 n, u32 seed);
void set_slot_value(u32 id, s32 value);
void obj_step_pos18(void *pos, s32 delta);
struct SpriteObj *obj_get_sprite(void *obj);
void obj_set_sprite_kind(void *obj, u32 kind);
void obj_init_from_record(void *obj, void *record);
void obj_finish_sprites_c0(void *obj);
void obj_create_sprite(void *obj, u32 zero);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void obj_release_children_5(void *obj, u32 flags);
void obj_release_children_9(void *obj, u32 flags);
void window_destroy(void *win, u32 flags);
void obj_free_tile_bufs(void *obj);

ASM_FUNC("asm/code_0815B1DC/scene_init_objects.s", void scene_init_objects(void))

void scene_free_objects(struct GameVars *game)
{
    struct PoolEntry *entry;
    struct ClassVt *table;
    void **slot;
    s32 i;

    for (i = 0; i < 0x100; i++)
    {
        entry = game->unk1898[i];
        if (entry != 0)
        {
            table = entry->table;
            table->fn0C((u8 *)entry + table->thisOffset, 3);
        }
        game->unk1898[i] = 0;
    }

    slot = (void **)((u8 *)game + 0x1E88);
    if (*slot != 0)
    {
        obj_release_children_5(*slot, 3);
        *slot = 0;
    }
    slot = (void **)((u8 *)game + 0x1E8C);
    if (*slot != 0)
    {
        obj_release_children_9(*slot, 3);
        *slot = 0;
    }
    slot = (void **)((u8 *)game + 0x1E90);
    if (*slot != 0)
    {
        window_destroy(*slot, 3);
        *slot = 0;
    }
    obj_free_tile_bufs((u8 *)game + 0x1C);
    obj_free_tile_bufs((u8 *)game + 0x840);
    obj_free_tile_bufs((u8 *)game + 0x1064);
}

ASM_FUNC("asm/code_0815B1DC/transition_scene_step.s", void transition_scene_step(void))

void camera_follow_chars(u8 *game)
{
    u32 *pa = (u32 *)(game + 0x1C98);
    u32 *pb;
    s32 *camX;
    u32 a;
    u32 b;
    s32 ax;
    s32 bx;
    s32 x;
    s32 y;
    s32 dx;

    a = *pa;
    pb = (u32 *)(game + 0x1C9C);
    b = *pb;
    ax = ((struct CamChar *)a)->x;
    bx = ((struct CamChar *)b)->x;
    dx = ax - bx;
    *(s32 *)(game + 0x1CA4) = bx + dx / 2 - 0x7800;
    camX = (s32 *)(game + 0x1CA4);
    *(s32 *)(game + 0x1CA8) = (((struct CamChar *)*pb)->y + ((struct CamChar *)*pa)->y) / 2 - 0x8C00;

    x = *camX;
    if (x < 0)
    {
        *camX = 0;
    }
    else
    {
        s32 xs = x >> 8;
        s32 limit = gSceneLayerSetupTable[game[0x1EA2]].unk18 - 1;
        if (xs >= (limit << 8))
        {
            *camX = limit << 16;
        }
    }

    y = *(s32 *)(game + 0x1CA8);
    if (y < 0)
    {
        *(s32 *)(game + 0x1CA8) = 0;
    }
    else if ((y >> 8) > 0x5F)
    {
        *(s32 *)(game + 0x1CA8) = 0x6000;
    }
}

ASM_FUNC("asm/code_0815B1DC/obj_spawn_by_kind.s", void obj_spawn_by_kind(void))


ASM_FUNC("asm/code_0815B1DC/update_bg_scroll_shadows.s", void update_bg_scroll_shadows(void))

NONMATCH("asm/code_0815B1DC/obj_command.s", u32 obj_command(struct ClassObj *obj, u32 view, u16 speed, u32 cmd_arg, u8 cmd))
{
    struct ObjClassVt *cls;
    u8 *arg;
    struct SpriteObj *sprite;
    const u8 *slotTable;
    u8 kind;
    s32 idx;
    u32 random;

    switch (cmd)
    {
    case 1:
        cls = (struct ObjClassVt *)obj->klass;
        arg = (u8 *)obj + cls->thisOffset30;
        ((void (*)(void *))cls->fn34)(arg);
        break;
    case 2:
        break;
    case 5:
        if (((obj->timer ^ 1) & 1) != 0)
        {
            obj->timer = obj->timer | 1;
            *(u16 *)&obj->angle = 0x96;
            obj_step_pos18(*(void **)(gUnk_03001020 + 0x1CB8), -3);
            sprite = obj_get_sprite(obj);
            kind = sprite->kind;
            *((u8 *)obj + 0x42) = kind;
            if (obj->kind == 0)
                obj_set_sprite_kind(obj, 0x28);
            else
                obj_set_sprite_kind(obj, 0x29);
        }
        break;
    case 13:
        obj->state = 8;
        break;
    case 12:
        if (obj->kind == 0)
        {
            slotTable = gSlotTable;
            random = mt19937_next();
            idx = rand_below(0x24, random);
            set_slot_value(slotTable[idx], -1);
        }
        obj->vy = 0xFC18;
        obj->state = 2;
        break;
    default:
        return obj_step_by_state(obj, view, (s16)speed, cmd_arg);
    }
    return 0;
}
END_NONMATCH

ASM_FUNC("asm/code_0815B1DC/obj_step_by_state.s", u32 obj_step_by_state(struct ClassObj *obj, u32 view, s16 speed, u32 cmd_arg))

struct ClassObj *set_class_e7e0_ex(struct ClassObj *obj, void *rec, u16 idx)
{
    u32 fl;

    obj_init_from_record(obj, rec);
    obj->klass = gClassE7e0;
    obj->classIdx = idx;
    obj->state = 0;
    obj->timer = 0;
    obj->angle = 0;
    obj->childB = 0;
    obj->childA = 0;
    obj_create_sprite(obj, 0);
    obj->childA = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->childA, 0x1C, -1, 0, 0);
    sprite_update_cd0(obj->childA);
    obj->childA->flags12 |= 0x20;
    obj->childA->counter = obj->byte2C;
    obj->childA->lo2 = obj->byte2D - 1;
    obj->childA->b2 = obj->byte2D - 1;
    obj->childA->b4 = obj->byte2D - 1;
    obj->childA->b6 = obj->byte2D - 1;
    obj->childB = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->childB, 0x1B, -1, 0, 0);
    sprite_update_cd0(obj->childB);
    fl = obj->childB->flags12;
    obj->childB->flags12 = fl | 0x20;
    return obj;
}

ASM_FUNC("asm/code_0815B1DC/obj_cmd_follow_a.s", void obj_cmd_follow_a(void))

struct ClassObj *set_class_e7a0_ex(struct ClassObj *obj, void *rec, u16 idx)
{
    obj_init_from_record(obj, rec);
    obj->klass = gClassE7a0;
    obj->classIdx = idx;
    obj->state = 0;
    obj->timer = 0;
    obj->childB = 0;
    obj->childA = 0;
    obj_finish_sprites_c0(obj);
    obj->childA = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->childA, 0x20, -1, 0, 0);
    sprite_update_cd0(obj->childA);
    obj->childA->counter = 0;
    obj->childA->lo2 = 2;
    obj->childA->b2 = 2;
    obj->childA->b4 = 2;
    obj->childA->flags12 |= 0x20;
    obj->childB = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->childB, 0x1B, -1, 0, 0);
    sprite_update_cd0(obj->childB);
    {
        u32 fl = obj->childB->flags12;
        obj->childB->flags12 = fl | 0x20;
    }
    return obj;
}
