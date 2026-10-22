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
    u8 filler0[4];
    s32 posX;
    s32 posY;
    u8 filler0C[0x14 - 0x0C];
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    u16 vy;
    u8 filler1A[0x1C - 0x1A];
    u16 unk1C;
    u8 filler1E[0x20 - 0x1E];
    u16 unk20;
    u8 filler22[0x24 - 0x22];
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

struct Bits12
{
    u8 filler0[0x12];
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
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
void obj_release_sprite(void *obj);
u32 view_coord(u32 a, s32 b);
void cam_start_wobble(void *cam, s32 value);
void *heap_alloc2(u32 size, u32 pool, const u8 *tag, u32 zero);
void obj_finish_sprites(void *obj);
void *set_class_e8e0_ex(void *obj, void *rec, u32 idx);
void *set_class_e820_ex(void *obj, void *rec, u32 idx);
void *set_class_e760_ex(void *obj, void *rec, u32 idx);
void *set_class_e720_ex(void *obj, void *rec, u32 idx);
void *set_class_e6e0_ex(void *obj, void *rec, u32 idx);
void *set_class_e6a0_ex(void *obj, void *rec, u32 idx);
void *set_class_e660_ex(void *obj, void *rec, u32 idx);
void *set_class_e5e0_ex(void *obj, void *rec, u32 idx);
void *set_class_e5a0_ex(void *obj, void *rec, u32 idx);
void *set_class_e560_ex(void *obj, void *rec, u32 idx);
void *set_class_e520_ex(void *obj, void *rec, u32 idx);
void *set_class_e4e0_ex(void *obj, void *rec, u32 idx);
u32 obj_init_e8a0(void *obj, void *rec, u32 idx);
u32 obj_init_e620(void *obj, void *rec, u32 idx);
struct ClassObj *set_class_e7e0_ex(struct ClassObj *obj, void *rec, u16 idx);
struct ClassObj *set_class_e7a0_ex(struct ClassObj *obj, void *rec, u16 idx);
extern const u8 gHeapTagSpawnE8e0A[];
extern const u8 gHeapTagSpawnE8e0B[];
extern const u8 gHeapTagSpawnE8a0[];
extern const u8 gHeapTagSpawnE820[];
extern const u8 gHeapTagSpawnE7e0[];
extern const u8 gHeapTagSpawnE7a0[];
extern const u8 gSpawnTagKoum[];
extern const u8 gSpawnTagKoub[];
extern const u8 gSpawnTagCoin[];
extern const u8 gSpawnTagCgoa[];
extern const u8 gSpawnTagJump[];
extern const u8 gSpawnTagJumb[];
extern const u8 gSpawnTagPuku[];
extern const u8 gSpawnTagDenc[];
extern const u8 gSpawnTagLamp[];
extern const u8 gSpawnTagFlam[];

struct KlassInit
{
    u8 filler0[0x10];
    s16 thisOffset10;
    u16 filler12;
    void (*fn14)(void *self, u32 a, u32 b, u32 c, u32 d);
};

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

void *obj_spawn_by_kind(void *game, u8 *spec)
{
    struct ClassObj *obj;
    struct KlassInit *k;
    const u8 *tag;

    switch (spec[0xC])
    {
    case 0:
        tag = gHeapTagSpawnE8e0A;
        goto spawn_e8e0;
    case 1:
        tag = gHeapTagSpawnE8e0B;
    spawn_e8e0:
        obj = set_class_e8e0_ex(heap_alloc2(0x48, 1, tag, 0), spec, 0);
        k = (struct KlassInit *)obj->klass;
        k->fn14((u8 *)obj + k->thisOffset10, 0, 0, 0, 1);
        return obj;
    case 2:
        obj = (struct ClassObj *)obj_init_e8a0(heap_alloc2(0x44, 1, gHeapTagSpawnE8a0, 0), spec, 0);
        return obj;
    case 4:
        obj = set_class_e820_ex(heap_alloc2(0x40, 1, gHeapTagSpawnE820, 0), spec, 0);
        obj_finish_sprites(obj);
        return obj;
    case 5:
        obj = (struct ClassObj *)set_class_e7e0_ex(heap_alloc2(0x44, 1, gHeapTagSpawnE7e0, 0), spec, 0);
        return obj;
    case 6:
        obj = (struct ClassObj *)set_class_e7a0_ex(heap_alloc2(0x44, 1, gHeapTagSpawnE7a0, 0), spec, 0);
        return obj;
    case 7:
        obj = (struct ClassObj *)set_class_e760_ex(heap_alloc2(0x3C, 1, gSpawnTagKoum, 0), spec, 0);
        return obj;
    case 9:
        obj = (struct ClassObj *)set_class_e720_ex(heap_alloc2(0x3C, 1, gSpawnTagKoub, 0), spec, 0);
        return obj;
    case 8:
        obj = (struct ClassObj *)set_class_e6e0_ex(heap_alloc2(0x38, 1, gSpawnTagCoin, 0), spec, 0);
        return obj;
    case 16:
        obj = (struct ClassObj *)set_class_e6a0_ex(heap_alloc2(0x38, 1, gSpawnTagCoin, 0), spec, 0);
        return obj;
    case 11:
        obj = (struct ClassObj *)set_class_e660_ex(heap_alloc2(0x38, 1, gSpawnTagCgoa, 0), spec, 0);
        return obj;
    case 12:
        obj = (struct ClassObj *)obj_init_e620(heap_alloc2(0x3C, 1, gSpawnTagJump, 0), spec, 0);
        return obj;
    case 13:
        obj = (struct ClassObj *)set_class_e5e0_ex(heap_alloc2(0x38, 1, gSpawnTagJumb, 0), spec, 0);
        return obj;
    case 14:
        obj = (struct ClassObj *)set_class_e5a0_ex(heap_alloc2(0x38, 1, gSpawnTagPuku, 0), spec, 0);
        return obj;
    case 17:
        obj = (struct ClassObj *)set_class_e520_ex(heap_alloc2(0x40, 1, gSpawnTagDenc, 0), spec, 0);
        return obj;
    case 15:
        obj = (struct ClassObj *)set_class_e560_ex(heap_alloc2(0x3C, 1, gSpawnTagLamp, 0), spec, 0);
        return obj;
    case 18:
        obj = (struct ClassObj *)set_class_e4e0_ex(heap_alloc2(0x38, 1, gSpawnTagFlam, 0), spec, 0);
        return obj;
    default:
        return 0;
    }
}


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

u32 obj_cmd_follow_a(struct ClassObj *obj, u32 view, u16 speed, u32 cmd_arg, u8 mode)
{
    struct ObjClassVt *cls;
    struct SpriteObj *sprite;
    u8 *b2d;
    s32 dx;
    s32 spd;

    switch (mode)
    {
    case 3:
        cls = (struct ObjClassVt *)obj->klass;
        ((void (*)(void *))cls->fn34)((u8 *)obj + cls->thisOffset30);
        ((struct Bits12 *)obj->childA)->bit5 = 0;
        obj->state = 1;
        break;
    case 4:
        obj_release_sprite(obj);
        obj->state = 0;
        break;
    case 1:
    case 2:
        break;
    default:
        obj->angle = (s32)((s16)view_coord(view, obj->posX) << 8);
        switch (obj->state)
        {
        case 0:
        case 4:
            break;
        case 1:
            dx = obj->posX - ((struct CamChar *)*(u32 *)(gUnk_03001020 + 0x1C98))->x;
            if (dx <= 0x81FF)
            {
                obj->timer = 0xF;
                obj_set_sprite_kind(obj, 0x19);
                obj->state = 2;
                break;
            }
            ((struct Bits12 *)obj->childA)->bit5 = 0;
            obj->childA->counter = 10;
            obj->childA->lo2 = obj_get_sprite(obj)->lo2 - 1;
            break;
        case 2:
            obj->timer--;
            if ((s16)obj->timer > 0)
                break;
            obj_set_sprite_kind(obj, 0x1A);
            obj->unk14 += obj->unk1C;
            spd = (s16)speed;
            obj->vy += spd + obj->unk20;
            obj->posX += (s16)obj->unk14;
            obj->posY += (s16)obj->vy;
            if (obj->posY > (s32)obj->angle)
            {
                ((struct Bits12 *)obj->childA)->bit5 = 1;
                set_slot_value(0x44, -1);
                sprite = obj_get_sprite(obj);
                b2d = &obj->byte2D;
                sprite->lo2 = *b2d - 1;
                obj_get_sprite(obj)->b2 = *b2d - 1;
                obj_get_sprite(obj)->b4 = *b2d - 1;
                ((struct Bits12 *)obj->childB)->bit5 = 0;
                cam_start_wobble(*(void **)(gUnk_03001020 + 0x1CBC), (s16)obj->vy);
                obj->timer = 0x14;
                obj->state = 3;
            }
            break;
        case 3:
            obj->timer--;
            if ((s16)obj->timer > 0)
                break;
            ((struct Bits12 *)obj->childB)->bit5 = 1;
            obj->state = 4;
            break;
        }
        break;
    }
    return 0;
}

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
