#include "global.h"
#include "ram_routines.h"

struct ClassVt
{
    u8 filler0[8];
    s16 unk08;
    u8 filler0A[2];
    void (*unk0C)(void *, s32);
};

struct ObjCore
{
    u8 filler0[0xC];
    u16 unk0C;
    u8 filler0E[4];
    u8 unk12;
};

struct ObjBit5
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct Sprite
{
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};

struct SubObj
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0xE];
    u8 unk12;
};

struct SceneObj
{
    struct ObjCore *core;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s16 unk14;
    u8 filler16[2];
    s16 unk18;
    u8 filler1A[2];
    u16 unk1C;
    u8 filler1E[6];
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 filler27;
    u8 unk28;
    u8 unk29;
    u8 filler2A[6];
    const void *unk30;
    u16 unk34;
    u16 unk36;
    u32 unk38;
    struct SceneObj *unk3C;
    struct SubObj *unk40;
    s32 unk44;
};

struct Pair
{
    s32 unk00;
    s32 unk04;
};

struct GameVars
{
    u8 filler0[0x1898];
    void *unk1898[0x100];
    struct SceneObj *unk1C98;
    u8 filler1C9C[0x1CAC - 0x1C9C];
    void *unk1CAC;
    u8 filler1CB0[4];
    void *unk1CB4;
    s32 unk1CB8;
    s32 unk1CBC;
    u8 filler1CC0[4];
    u16 unk1CC4;
    u8 filler1CC6[0x1F20 - 0x1CC6];
    u8 unk1F20;
    u8 filler1F21[7];
    s16 unk1F28;
    u16 unk1F2A;
};

struct SceneStateEntry
{
    u8 filler0[0x1C];
    u16 unk1C;
    u8 filler1E[2];
};

struct ClassObj
{
    u8 filler0[4];
    const void *unk04;
    s32 unk08;
    u8 filler0C[8];
    s32 unk14;
    const void *unk18;
};

struct CamObj
{
    u8 filler0[0x500];
    s32 unk500;
    s32 unk504;
    s32 unk508;
    s32 unk50C;
    s16 unk510;
    u8 filler512[2];
    u16 unk514;
    u8 filler516[2];
    u16 unk518;
    u8 filler51A[2];
    s16 unk51C;
    u8 filler51E[2];
    u16 unk520;
    u8 filler522[2];
    void *unk524;
    s16 unk528;
    u8 filler52A[2];
    s32 unk52C;
    s32 unk530;
    s32 unk534;
    s32 unk538;
    s32 unk53C;
};

extern const u8 gClassE400[];
extern const u8 gTransitionClassDec0[];
extern const u8 gTransitionClassDf20[];
extern const u8 gTransitionClassDf60[];
extern const u8 gSceneDisplayClass[];
void task_insert(void *node, u8 kind);
void heap_free_b(void *p);
void slot_reset(u32 id);
void set_irq_callback(u32 kind, void *cb);
void scene_free_objects(struct SceneObj *obj);
void obj_release_children_4(void *sub, u32 mode);
void task_remove(void *node, u32 kind);
void heap_free(void *p);
void set_flags_2_3(u32 v);
void obj_run_callbacks(void);
void obj_free_tiles(void *p, u32 n);
extern u16 gUnk_02000004;
extern void *gUnk_0300101C;
extern u8 *gUnk_03001020;
extern const u8 gClassE420[];
void obj_reset_e400(void *obj);
void cam_build_line_table(struct CamObj *cam, struct Pair *arg);
void cam_pan_by_keys(struct CamObj *cam);
void *obj_create(void *game);
void obj_place_on_screen(void *obj, s32 *pos);

struct KeyState
{
    u16 unk00;
    u16 unk02;
    u8 filler04[0x24];
    u16 held;
};

struct SceneVt
{
    u8 filler0[0x10];
    s16 unk10;
    u8 filler12[2];
    s32 (*unk14)(void *, void *);
};

struct NewObj
{
    u8 filler0[4];
    const struct SceneVt *unk04;
};

struct LineTables
{
    u32 unk0;
    u16 unk4[0xE4];
    u16 unk1CC[0xE4];
};

extern struct KeyState gSys;
extern u8 gUnk_02000048[];
extern u8 gUnk_02000050[];
extern u8 *gUnk_03001014;
extern struct GameVars *gGamePtr;
void scene_teardown(struct GameVars *game, s32 mode);
void dma3_copy_tilemap(void *obj);
extern const struct SceneStateEntry gSceneStateTable_083CCAC0[];
void obj_update_speed(struct SceneObj *obj);

extern const u8 gClassE280[];
extern const u8 gClassE2c0[];
extern const u8 gClassE3c0[];

struct Sprite *obj_get_sprite(void *obj);
void obj_create_sprite(void *obj, u32 zero);
void obj_init_from_record(void *obj, void *record);
void obj_reset(void *obj, u32 arg);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void release_obj_131c(void *sub);
void obj_set_sprite_kind(void *obj, u32 kind);
void sprite_set_pos_rel_cam(void *obj, u32 cam);
void obj_release_sprite(void *obj);
void obj_move_by_speed(void *obj, s32 extra);
void set_slot_value(u32 slot, s32 value);
u32 view_coord(u32 a, s32 b);

void obj_create_sprite_c0(struct SceneObj *obj)
{
    struct Sprite *sprite;
    u16 value;

    obj_create_sprite(obj, 0);
    sprite = obj_get_sprite(obj);
    value = 0xC0;
    sprite->unk04 = value;
    sprite = obj_get_sprite(obj);
    sprite->unk06 = value;
}

void obj_reset_e280(struct SceneObj *obj, u32 arg)
{
    obj->unk30 = gClassE280;
    if (obj->unk40 != 0)
    {
        release_obj_131c(obj->unk40);
        obj->unk40 = 0;
    }
    obj_reset(obj, arg);
}

struct SceneObj *obj_create_e280(struct SceneObj *obj, void *record, u16 kind)
{
    struct SubObj *sub;
    u32 flags;

    obj_init_from_record(obj, record);
    obj->unk30 = gClassE280;
    obj->unk34 = kind;
    obj->unk24 = 0;
    obj->unk36 = 0;
    obj->unk40 = 0;
    obj->unk3C = 0;
    obj_create_sprite_c0(obj);
    obj->unk40 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk40, 0x1B, -1, 0, 0);
    sprite_update_cd0(obj->unk40);
    sub = obj->unk40;
    flags = sub->unk12;
    flags = flags | 0x20;
    sub->unk12 = flags;
    return obj;
}

void obj_init_from_pair_b(struct SceneObj *obj, struct Pair *pair)
{
    s32 b = pair->unk04;
    s32 a = pair->unk00;
    obj->unk04 = a;
    obj->unk08 = b;
    obj->unk0C = a;
    obj->unk10 = b;
    obj->unk14 = 0;
    obj->unk18 = 0;
    obj->unk24 = 0;
}

void obj_update_sub_pos_e2c0(struct SceneObj *obj, u32 cam)
{
    struct Sprite *sprite;

    sprite_set_pos_rel_cam(obj, cam);
    if (obj->unk40 != 0)
    {
        sprite = obj_get_sprite(obj);
        obj->unk40->unk00 = sprite->unk00;
        sprite = obj_get_sprite(obj);
        obj->unk40->unk02 = sprite->unk02;
    }
}

void obj_reset_e2c0(struct SceneObj *obj, u32 arg)
{
    obj->unk30 = gClassE2c0;
    if (obj->unk40 != 0)
    {
        release_obj_131c(obj->unk40);
        obj->unk40 = 0;
    }
    obj_reset(obj, arg);
}

struct SceneObj *obj_create_e2c0(struct SceneObj *obj, void *record, u16 kind)
{
    struct SubObj *sub;
    u32 flags;

    obj_init_from_record(obj, record);
    obj->unk30 = gClassE2c0;
    obj->unk34 = kind;
    obj->unk24 = 0;
    obj->unk36 = 0;
    obj->unk38 = 0;
    obj->unk40 = 0;
    obj->unk3C = 0;
    obj_create_sprite(obj, 0);
    obj->unk40 = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->unk40, 0x1B, -1, 0, 0);
    sprite_update_cd0(obj->unk40);
    sub = obj->unk40;
    flags = sub->unk12;
    flags = flags | 0x20;
    sub->unk12 = flags;
    return obj;
}

void obj_set_mode_flag25_de(struct SceneObj *obj)
{
    if (obj->unk25 == 0)
    {
        obj_set_sprite_kind(obj, 0xD);
    }
    else
    {
        obj_set_sprite_kind(obj, 0xE);
    }
}

void obj_set_mode_flag25_bc(struct SceneObj *obj)
{
    if (obj->unk25 == 0)
    {
        obj_set_sprite_kind(obj, 0xB);
    }
    else
    {
        obj_set_sprite_kind(obj, 0xC);
    }
}

void obj_update_pos_e3c0(struct SceneObj *obj, u32 cam)
{
    if ((obj->unk36 & 0x800) == 0)
    {
        sprite_set_pos_rel_cam(obj, cam);
    }
}

void obj_set_flag36_bit11(struct SceneObj *obj, u32 flag)
{
    if (flag != 0)
    {
        obj->unk36 &= 0xF7FF;
    }
    else
    {
        obj->unk36 |= 0x800;
    }
}

s32 obj_pos_less_than_limit(struct SceneObj *obj)
{
    if (obj->unk08 < obj->unk44)
    {
        return 1;
    }
    return 0;
}

void obj_copy_pair(struct SceneObj *obj, struct Pair *pair)
{
    s32 b = pair->unk04;
    s32 a = pair->unk00;
    obj->unk04 = a;
    obj->unk08 = b;
    obj->unk0C = a;
    obj->unk10 = b;
}

void obj_clear_state_flags(struct SceneObj *obj)
{
    obj->core->unk0C = 0;
    ((struct ObjBit5 *)&obj->core->unk12)->bit5 = 0;
    obj->unk36 &= 0xFFFE;
    obj->unk14 = 0;
}

void obj_clear_offset_14(struct SceneObj *obj)
{
    obj->unk14 = 0;
}

void obj_set_char_offset(struct SceneObj *obj)
{
    s32 offset;

    if (obj->unk25 == 0)
    {
        offset = gSceneStateTable_083CCAC0[gGamePtr->unk1F20].unk1C;
    }
    else
    {
        offset = gSceneStateTable_083CCAC0[gGamePtr->unk1F20].unk1C - 0x64;
    }
    obj->unk14 = offset;
    obj->unk24 = 2;
}

void obj_init_value_9(struct SceneObj *obj, u32 arg)
{
    s32 view = (s32)(view_coord(arg, obj->unk04) << 16) >> 8;
    obj->unk44 = view;
    obj->unk08 = view;
    obj->unk24 = 9;
}

void obj_update_view_pos(struct SceneObj *obj, u32 pos)
{
    s32 view = (s32)(view_coord(pos, obj->unk04) << 16) >> 8;

    obj->unk44 = view;
    if (obj->unk08 > view)
    {
        obj->unk08 = view;
        obj->unk14 = obj->unk04 - obj->unk0C;
        obj->unk18 = view - obj->unk10;
        obj->unk0C = obj->unk04;
        obj->unk10 = view;
    }
    else
    {
        obj_update_speed(obj);
    }
}

void obj_step_by_speed(struct SceneObj *obj, u32 unused, s32 speed)
{
    obj_move_by_speed(obj, speed);
}

void obj_clamp_value_8(struct SceneObj *obj, s32 value)
{
    if (obj->unk08 > value)
    {
        obj->unk08 = value;
        obj->unk18 = 0;
    }
}

void obj_teardown_e3c0(struct SceneObj *obj, u32 arg)
{
    struct SceneObj *child;
    const struct ClassVt *vt;

    obj->unk30 = gClassE3c0;
    if (obj->unk3C != 0)
    {
        obj_release_sprite(obj->unk3C);
        child = obj->unk3C;
        if (child != 0)
        {
            vt = child->unk30;
            vt->unk0C((u8 *)child + vt->unk08, 3);
        }
    }
    obj_reset(obj, arg);
}

struct SceneObj *obj_create_e3c0(struct SceneObj *obj, void *record, u16 flags)
{
    obj_init_from_record(obj, record);
    obj->unk30 = gClassE3c0;
    obj->unk24 = 0;
    obj->unk34 = 0;
    obj->unk36 = flags;
    *(u16 *)&obj->unk40 = 0;
    obj->unk3C = 0;
    obj->unk44 = 0;
    obj_create_sprite(obj, 0);
    return obj;
}

void game_countdown_1f2a(struct GameVars *game)
{
    s32 t;

    t = game->unk1F2A - 1;
    game->unk1F2A = t;
    if ((s16)t < 0)
    {
        game->unk1F28 = 0;
    }
}

void game_set_1f28(struct GameVars *game, s32 value)
{
    if (game->unk1F28 != value)
    {
        game->unk1F28 = value;
        game->unk1F2A = 0x5A;
        set_slot_value(value, -1);
    }
}

NONMATCH("asm/code_081599D8/set_win_blend_shadow.s", void set_win_blend_shadow(void *unused, s32 mode))
{
    vu16 *p;

    if (mode != 0)
    {
        *(vu16 *)0x02000000 |= 0x400;
        *(vu16 *)0x02000000 |= 0x2000;
        p = (vu16 *)gUnk_02000048;
        *p = 0x3F;
        p++;
        *p = 0x1F;
        p += 3;
        *p = 0x3F44;
        p++;
        *p = 0x1008;
        p -= 9;
        *p = 0;
        p += 2;
        *p = 0xA0;
    }
    else
    {
        *(vu16 *)0x02000000 &= 0xFBFF;
        p = (vu16 *)gUnk_02000050;
        *p = mode;
        p += 2;
        *p = mode;
    }
}
END_NONMATCH

void store_at_1cac(struct GameVars *game, void *value)
{
    game->unk1CAC = value;
}

struct SceneObj *pool_find_kind2(struct GameVars *game, s32 kind, s32 sub)
{
    s32 i = 0;
    void **p = game->unk1898;
    struct SceneObj *obj;

    do
    {
        obj = p[0];
        if (obj != 0 && obj->unk25 == kind && obj->unk26 == sub) return obj;
        obj = p[1];
        if (obj != 0 && obj->unk25 == kind && obj->unk26 == sub) return obj;
        obj = p[2];
        if (obj != 0 && obj->unk25 == kind && obj->unk26 == sub) return obj;
        obj = p[3];
        if (obj != 0 && obj->unk25 == kind && obj->unk26 == sub) return obj;
        p += 4;
        i += 4;
    } while (i <= 0xFF);
    return 0;
}

struct SceneObj *sub_08159F18(struct GameVars *game, s32 kind)
{
    s32 i = 0;
    void **p = game->unk1898;
    struct SceneObj *obj;

    for (; i <= 0xFF; i++)
    {
        obj = p[0];
        if (obj != 0 && obj->unk25 == kind) return obj;
        p++;
    }
    return 0;
}

NONMATCH("asm/code_081599D8/sub_08159F78.s", void *sub_08159F78(struct GameVars *game))
{
    void **p;
    s32 i;
    s32 slot;

    for (i = 0, p = game->unk1898; i <= 0xFF; p += 8, i += 8)
    {
        if (p[0] == 0)
            goto store;
        slot = i + 1;
        if (p[1] == 0)
            goto found;
        slot = i + 2;
        if (p[2] == 0)
            goto found;
        slot = i + 3;
        if (p[3] == 0)
            goto found;
        slot = i + 4;
        if (p[4] == 0)
            goto found;
        slot = i + 5;
        if (p[5] == 0)
            goto found;
        slot = i + 6;
        if (p[6] == 0)
            goto found;
        slot = i + 7;
        if (p[7] == 0)
        {
found:
            i = slot;
            goto store;
        }
    }
    slot = -1;
    i = slot;
store:
    return game->unk1898[i] = obj_create(game);
}
END_NONMATCH

s32 pool_first_free_slot(struct GameVars *game)
{
    void **p;
    s32 i;

    i = 0;
    p = game->unk1898;
    for (; i <= 0xFF; i++)
    {
        if (p[0] == 0)
            return i;
        p++;
    }
    return -1;
}

void sub_0815A054(struct GameVars *game)
{
    struct Pair pos;
    s32 *p;
    pos = *(struct Pair *)&game->unk1C98->unk04;
    p = &pos.unk00;
    p[0] -= game->unk1CB8;
    p[1] -= game->unk1CBC;
    obj_place_on_screen(game->unk1CB4, p);
}

void transition_end_check(struct GameVars *game)
{
    struct NewObj *x = game->unk1CAC;
    const struct SceneVt *vt = x->unk04;
    s32 result = vt->unk14((u8 *)x + vt->unk10, game);

    if (result == 0 && game != 0)
    {
        scene_teardown(game, 3);
    }
}

void irq_hblank_line(void)
{
    u16 scan = *(vu16 *)0x04000006;
    *(vu16 *)0x04000018 = 0;
    *(vu16 *)0x0400001A = 0x80 - scan;
    *(vu16 *)0x04000040 = ((struct LineTables *)gUnk_03001014)->unk4[scan];
    if ((gGamePtr->unk1CC4 & 4) == 0)
    {
        *(vu16 *)0x05000000 = ((struct LineTables *)gUnk_03001014)->unk1CC[scan];
    }
}

void irq_copy_tilemaps(void)
{
    *(u16 *)0x04000040 = *(u16 *)(gUnk_03001014 + 4);
    dma3_copy_tilemap((u8 *)gGamePtr + 0x1C);
    dma3_copy_tilemap((u8 *)gGamePtr + 0x840);
    if ((gGamePtr->unk1CC4 & 4) != 0)
    {
        dma3_copy_tilemap((u8 *)gGamePtr + 0x1064);
    }
    gSys.unk02 |= 0x7FFF;
}

struct ClassObj *obj_set_class_de90(struct ClassObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = gSceneDisplayClass;
    return obj;
}

void stub_nop_0815a1b8(void)
{
}

void stub_nop_0815a1bc(void)
{
}

void obj_free_ddec0(struct ClassObj *obj, u32 flags)
{
    obj->unk04 = gClassE400;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

struct ClassObj *obj_set_class_dec0(struct ClassObj *obj)
{
    obj_reset_e400(obj);
    obj->unk04 = gTransitionClassDec0;
    obj->unk14 = 0;
    return obj;
}

void stub_nop_0815a1fc(void)
{
}

void obj_free_ddee0(struct ClassObj *obj, u32 flags)
{
    obj->unk04 = gClassE400;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void stub_nop_0815a220(void)
{
}

void obj_free_ddf00(struct ClassObj *obj, u32 flags)
{
    obj->unk04 = gClassE400;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void stub_nop_0815a244(void)
{
}

void obj_free_ddf20(struct ClassObj *obj, u32 flags)
{
    obj->unk04 = gClassE400;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

struct ClassObj *obj_set_class_df20(struct ClassObj *obj)
{
    obj_reset_e400(obj);
    obj->unk04 = gTransitionClassDf20;
    obj->unk08 = 0;
    return obj;
}

void stub_nop_0815a284(void)
{
}

void obj_free_ddf40(struct ClassObj *obj, u32 flags)
{
    obj->unk04 = gClassE400;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

void stub_nop_0815a2a8(void)
{
}

void obj_free_ddf60(struct ClassObj *obj, u32 flags)
{
    obj->unk04 = gClassE400;
    if ((flags & 1) != 0)
    {
        heap_free_b(obj);
    }
}

struct ClassObj *obj_init_class_df60(struct ClassObj *obj)
{
    obj_reset_e400(obj);
    obj->unk04 = gTransitionClassDf60;
    return obj;
}

ASM_FUNC("asm/code_081599D8/obj_init_ext_fields.s", void obj_init_ext_fields(void))

void cam_pan_by_keys(struct CamObj *cam)
{
    if (gSys.held & 0x10)
        cam->unk508 += 0x400;
    if (gSys.held & 0x20)
        cam->unk508 -= 0x400;
    if (gSys.held & 0x80)
        cam->unk50C += 0x400;
    if (gSys.held & 0x40)
        cam->unk50C -= 0x400;
}

void cam_scroll_anim(struct CamObj *cam, u8 enable)
{
    cam_pan_by_keys(cam);
    if ((cam->unk528 & 1) != 0)
    {
        if (enable != 0)
        {
            if ((s16)(cam->unk528 & 4) != 0)
            {
                cam->unk530 += cam->unk538;
                if (cam->unk530 >= cam->unk534)
                {
                    cam->unk530 = cam->unk534;
                    cam->unk528 &= -5;
                }
            }
            else
            {
                cam->unk530 -= 0x20;
                if (cam->unk530 < 0)
                {
                    cam->unk530 = 0;
                }
            }
        }
    }
}

void cam_follow_target(struct CamObj *cam, struct Pair *target)
{
    s32 x = cam->unk508 + target->unk00;
    s32 y = cam->unk50C + target->unk04;
    u16 uVar1;
    s32 sVar1;
    s32 t;
    s32 c;

    if (x > 0xF000)
    {
        cam->unk508 = 0xF000 - target->unk00;
    }
    else if (x < 0)
    {
        cam->unk508 = -target->unk00;
    }
    if (y > 0xA000)
    {
        cam->unk50C = 0xA000 - target->unk04;
    }
    else if (y < 0)
    {
        cam->unk50C = -target->unk04;
    }
    uVar1 = cam->unk518;
    if ((s16)cam->unk518 > 0)
    {
        t = uVar1 + 0xC;
        cam->unk518 = t;
        if ((s16)t > 0xFF)
        {
            cam->unk518 = 0;
        }
        c = gCosineTable[*(u8 *)&cam->unk518];
        if (c < 0)
        {
            c += 0x3F;
        }
        sVar1 = (c >> 6) * 3;
        cam->unk51C = sVar1;
        cam->unk514 = sVar1;
        cam->unk508 += cam->unk510;
        cam->unk50C += (s16)cam->unk514;
    }
    cam->unk500 = -(target->unk00 + cam->unk508);
    cam->unk504 = -(target->unk04 + cam->unk50C);
    if (REG_VCOUNT > 0x9F)
    {
        cam_build_line_table(cam, target);
    }
}

ASM_FUNC("asm/code_081599D8/cam_build_line_table.s", void cam_build_line_table(struct CamObj *cam, struct Pair *arg))

void cam_set_factor(struct CamObj *cam, s32 factor)
{
    cam->unk52C = factor;
}

void cam_start_scroll_to(struct CamObj *cam, s32 limit)
{
    cam->unk528 |= 4;
    cam->unk534 = limit;
    cam->unk538 = (limit - cam->unk530) / 10;
}

void call_zeroed_arg_block_b(struct CamObj *cam)
{
    struct Pair block;

    block.unk04 = 0;
    block.unk00 = 0;
    cam_build_line_table(cam, &block);
}

void obj_clear_halfword_528_bit0(struct CamObj *obj)
{
    *(s16 *)&obj->unk528 &= ~1;
}

void cam_enable_scroll(struct CamObj *cam)
{
    cam->unk528 |= 1;
}

void cam_start_wobble(struct CamObj *cam, s32 value)
{
    cam->unk518 = 1;
    cam->unk514 = value;
}

NONMATCH("asm/code_081599D8/cam_target_within_range.s", s32 cam_target_within_range(struct CamObj *cam, struct Pair *target, struct Pair *origin))
{
    s32 ox;
    s32 oy;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 acc;
    s32 limit;
    s32 *fp;

    ox = origin->unk00 + cam->unk508;
    oy = origin->unk04 + cam->unk50C;
    dx = target->unk00 - ox;
    dy = target->unk04 - oy;
    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
    fp = &cam->unk52C;
    acc = cam->unk530;
    if (acc < 0)
    {
        acc += 0xFF;
    }
    acc >>= 8;
    limit = acc * *fp * 3;
    if (limit < 0)
    {
        limit += 3;
    }
    if (dist < (limit >> 2))
    {
        return 1;
    }
    return 0;
}
END_NONMATCH

void cam_setup_relative(struct CamObj *cam, struct Pair *a, struct Pair *b, s32 factor)
{
    struct Pair *off;

    cam->unk52C = factor;
    off = (struct Pair *)&cam->unk508;
    *off = *b;
    off->unk00 = a->unk00 - off->unk00;
    cam->unk50C = a->unk04 - cam->unk50C;
    cam_build_line_table(cam, b);
    cam_enable_scroll(cam);
}

void stub_nop_0815a8a8(void)
{
}

void cam_free(struct CamObj *cam, u32 flags)
{
    if (cam->unk524 != 0)
    {
        release_obj_131c(cam->unk524);
    }
    if ((flags & 1) != 0)
    {
        heap_free_b(cam);
    }
}

ASM_FUNC("asm/code_081599D8/bg_update_wave.s", void bg_update_wave(void))

extern u8 *gUnk_03001020;
ASM_FUNC("asm/code_081599D8/irq_hblank_fx_bg.s", void irq_hblank_fx_bg(void))

ASM_FUNC("asm/code_081599D8/irq_hblank_fx_blend.s", void irq_hblank_fx_blend(void))

NONMATCH("asm/code_081599D8/obj_overlap_box.s", s32 obj_overlap_box(struct SceneObj *a, struct SceneObj *b))
{
    s32 aw;
    s32 ah;
    s32 bw;
    s32 bh;
    s32 d;
    s32 ax;
    s32 bx;
    s32 ay;
    s32 by;
    u32 dxp;
    u32 dyp;

    aw = a->unk28 >> 1;
    ah = a->unk29 >> 1;
    bw = b->unk28 >> 1;
    bh = b->unk29 >> 1;
    ax = a->unk04;
    bx = b->unk04;
    if (ax > bx)
    {
        d = ax - bx;
        dxp = ((u32)d << 8) >> 16;
    }
    else if (ax < bx)
    {
        d = bx - ax;
        dxp = ((u32)d << 8) >> 16;
    }
    else
    {
        dxp = 0;
    }
    ay = (a->unk08 >> 8) - ah;
    by = (b->unk08 >> 8) - bh;
    ay = (s16)ay;
    by = (s16)by;
    if (ay > by)
    {
        d = ay - by;
        dyp = ((u32)d << 16) >> 16;
    }
    else if (ay < by)
    {
        d = by - ay;
        dyp = ((u32)d << 16) >> 16;
    }
    else
    {
        dyp = 0;
    }
    if ((s16)dxp > aw + bw)
    {
        return 0;
    }
    if ((s16)dyp > ah + bh)
    {
        return 0;
    }
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_081599D8/transition_scene_init.s", void transition_scene_init(void))

void transition_exit(struct SceneObj *obj, u32 task)
{
    void *sub;
    struct CamObj *cam;

    *(u32 *)&obj->unk18 = (u32)gClassE420;
    slot_reset(0);
    set_irq_callback(2, 0);
    REG_IME = 0;
    gUnk_02000004 &= 0xFFEF;
    REG_IE &= 0xFFFD;
    REG_IF |= 2;
    REG_IME = 1;
    set_irq_callback(4, 0);
    scene_free_objects(obj);
    sub = *(void **)((u8 *)obj + 0x1CB8);
    if (sub != 0)
    {
        obj_release_children_4(sub, 3);
    }
    sub = *(void **)((u8 *)obj + 0x1EA4);
    if (sub != 0)
    {
        task_remove(sub, 3);
    }
    cam = *(struct CamObj **)((u8 *)obj + 0x1CBC);
    if (cam != 0)
    {
        cam_free(cam, 3);
    }
    heap_free(gUnk_0300101C);
    gSys.unk02 = 0xFFFF;
    ((struct ObjBit5 *)((u8 *)&gSys + 0x888))->bit5 = 0;
    set_flags_2_3(*(u32 *)((u8 *)obj + 0x14));
    obj_run_callbacks();
    obj_free_tiles((u8 *)obj + 0x1064, 2);
    obj_free_tiles((u8 *)obj + 0x840, 2);
    obj_free_tiles((u8 *)obj + 0x1C, 2);
    task_remove(obj, task);
}
