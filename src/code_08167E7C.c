#include "global.h"
#include "ram_routines.h"

extern const u8 gClassEb10[];
extern const u8 gClassEb28[];
extern const u8 gClassEb38[];
extern const u8 gClassEb48[];
extern const u8 gClassEb58[];
extern const s8 gDirTab[];
extern const u8 gLoadRecordIds[];
extern const u8 gFrameRecordOffsets[];
extern const u8 gObjScript08216806[];
extern const u32 gPhase4ChildETable[];
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();

void obj_destroy_sprites(void *obj, u32 flags);
void task_remove(void *node, u32 kind);
void task_insert(void *node, u8 kind);
void heap_free(void *ptr);
void heap_free_b(void *ptr);
u32 arc_tan2(s32 x, s32 y);
void obj_set_table_fields(void *obj, u32 table);
void obj_list_pass_0a8c(void);
void obj_load_record(void *obj, u32 record, s32 flags, u32 arg3, u32 arg4);
void obj_update_sprites(void *obj);
void obj_init_big(void *obj, u32 a, u32 b, u32 c);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_create_sprite_14(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_set_table_and_init(void *obj, u32 table);
void obj_update_anim_state(void *obj);
void grid_lines_obj_draw_grid(void *obj);
void obj_set_fields_18(void *obj, u32 a, u32 b, u32 c);
void obj_init_class_38fa(void *obj, u32 value);
void obj_set_frame_table(void *obj, const void *table);
void obj_refresh_flags(void *obj, u32 value);
void obj_release_sprites(void *obj);
void triple_copy(void *dst, const void *src);
void obj_clear_buffer(void *grid);
void obj_set_offset(void *grid, s32 x, s32 y);
void obj_child_ram_call(void *grid, s32 *args, u32 kind);
void obj_upload_tiles(void *grid);

struct AvgSample { s32 v; s32 pad[2]; };
struct AvgEntry
{
    u32 v00;
    u8 filler4[8];
    u32 v0C;
    u8 filler10[8];
    u32 v18;
    u8 filler20[8];
    u32 v24;
    u8 filler28[8];
    u32 v30;
    u8 filler34[8];
};

struct SysState
{
    u8 filler0[0x2A];
    u16 rawKeysPressed;
    u8 filler2C[0x880 - 0x2C];
    u32 oamCurrent;
};

extern struct SysState gSys;

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

extern struct ObjLists gUnk_0203FFB8;

struct GridSprite
{
    u8 filler0[0xC];
    u16 word0C;
    u8 filler0E[0x12 - 0xE];
    u8 attr12;
};

struct SpriteModeBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct GridObj
{
    u8 filler0[8];
    void *unk8;
    void *cls;
    void *unk10;
    void *unk14;
    void *unk18;
    u32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 filler30[0x3C - 0x30];
    void *anchor3C;
    void *points40;
    void *grid44;
    u8 filler48[0x52 - 0x48];
    s16 dir52;
    u16 unk54;
    u16 mode56;
    u16 mode58;
    u8 filler5A[0x6C - 0x5A];
    u16 flags;
    u16 counter;
    u8 filler70[0x74 - 0x70];
    s32 offY;
    s32 offX;
    s32 scale;
    u8 filler80[0x21C - 0x80];
    u32 unk21C;
    u32 unk220;
    u32 unk224;
    u8 filler228[0x230 - 0x228];
    void *path230;
    void *path234;
};

struct GridGroup
{
    struct GridObj *objA;
    struct GridObj *objB;
    struct GridObj *objC;
    struct GridObj *objD;
    struct GridObj *objE;
    s32 base14;
    s32 base18;
    s32 base1C;
    u8 filler20[0x2C - 0x20];
    s32 v2C;
    s32 v30;
    s32 v34;
    s32 out38;
    u8 filler3C[0x40 - 0x3C];
    s32 out40;
    s32 out44;
    u8 filler48[0x4C - 0x48];
    s32 out4C;
};

void obj_script_step(struct GridObj *obj, u32 arg);
u8 obj_is_done(struct GridObj *obj);
void obj_list_remove_call(u32 arg, struct GridObj *obj);
void obj_place_child(struct GridObj *obj, u32 mode, u32 attr, s32 *base, u32 record);

ASM_FUNC("asm/code_08167E7C/obj_key_state_update.s", void obj_key_state_update(void))

ASM_FUNC("asm/code_08167E7C/obj_phase4_step.s", void obj_phase4_step(void))

ASM_FUNC("asm/code_08167E7C/grid_lines_obj_draw_grid.s", void grid_lines_obj_draw_grid(void *obj))

ASM_FUNC("asm/code_08167E7C/obj_frame_update.s", void obj_frame_update(void))

void obj_set_dir_to_point(u32 unused0, struct GridObj *obj, s32 x, s32 y)
{
    s32 dx = (s32)((u32)((s32)obj->unk18 - x) << 8) >> 16;
    s32 dy = (s32)((u32)((s32)obj->unk20 - y) << 8) >> 16;
    u32 angle = (u16)arc_tan2(dx, dy);
    u32 sector = (angle + 0x1000) >> 13;
    obj->dir52 = gDirTab[sector];
}

ASM_FUNC("asm/code_08167E7C/obj_copy_end_points.s", void obj_copy_end_points(void))

void obj_set_mode_if_flag8(struct GridObj *obj)
{
    if (gSys.rawKeysPressed & 8)
    {
        *(u32 *)&obj->unk54 = 4;
    }
}

void obj_set_child_tables_28_2c(struct GridObj *obj, u32 table_a, u32 table_b)
{
    obj_set_table_fields(obj->unk28, table_a);
    obj_set_table_fields(obj->unk2C, table_b);
}

void obj_set_child_tables(struct GridObj *obj, u32 table_a, u32 table_b)
{
    obj_set_table_fields(obj->unk20, table_a);
    obj_set_table_fields(obj->unk24, table_b);
}

void obj_phase4_child_setup(struct GridObj *obj, u32 arg)
{
    u32 idx = arg;
    void *sub = obj->unk8;

    obj_set_table_fields(sub, gPhase4ChildETable[idx - 2]);
    idx -= 2;
    obj_load_record(((struct GridObj *)obj->cls)->unk10, gLoadRecordIds[idx], -1, 0, 0);
    obj_update_sprites(obj->cls);
    obj_set_table_fields(obj->cls, (u32)gObjScript08216806);
}

void obj_class_eb28_free(struct GridObj *obj, u32 free_flag)
{
    obj->unk10 = (void *)gClassEb28;
    if (free_flag & 1)
    {
        heap_free_b(obj);
    }
}

void obj_class_eb38_destroy(struct GridObj *obj, u32 flags)
{
    obj->cls = (void *)gClassEb38;
    obj_destroy_sprites(obj, flags);
}

struct GridObj *init_vt_08cdeb38(struct GridObj *obj, u32 table, u32 a2, u32 a3)
{
    obj_init_big(obj, 1, a2, a3);
    obj->cls = (void *)gClassEb38;
    obj_create_sprite_10(obj, 4, 0x5001, 0, 0, 3);
    obj_create_sprite_14(obj, 0, 0x1000, 0, 0, 3);
    obj_set_table_and_init(obj, table);
    obj_update_anim_state(obj);
    return obj;
}

void obj_reset_flags(struct GridObj *obj)
{
    obj->flags = 0;
    obj->counter = 0;
}

void obj_class_eb48_destroy(struct GridObj *obj, u32 flags)
{
    obj->cls = (void *)gClassEb48;
    obj_destroy_sprites(obj, flags);
}

struct GridObj *init_vt_08cdeb48(struct GridObj *obj, u32 a1, u32 a2, u32 a3)
{
    obj_init_big(obj, 0, a2, a3);
    obj->cls = (void *)gClassEb48;
    obj_reset_flags(obj);
    obj_create_sprite_10(obj, 0, a1, 1, 0, 2);
    obj_create_sprite_14(obj, 0, 0x1000, 0, 0, 3);
    obj_set_table_and_init(obj, a2);
    obj_set_frame_table(obj, gFrameRecordOffsets);
    obj_refresh_flags(obj, 0);
    return obj;
}

void obj_step_remove_when_done(struct GridObj *obj, u32 arg)
{
    obj_script_step(obj, arg);
    if (((struct GridSprite *)obj->unk10)->attr12 & 0x10)
    {
        if (obj_is_done(obj))
        {
            obj_list_remove_call(arg, obj);
        }
    }
}

void obj_class_eb58_destroy(struct GridObj *obj, u32 flags)
{
    obj->cls = (void *)gClassEb58;
    obj_destroy_sprites(obj, flags);
}

struct GridObj *init_vt_08cdeb58(struct GridObj *obj, u32 a1, u32 a2, u32 a3, u32 a4)
{
    obj_init_big(obj, 2, a2, a3);
    obj->cls = (void *)gClassEb58;
    obj_create_sprite_10(obj, 2, 0x8004, 0, a1, 1);
    ((struct SpriteModeBits *)&((struct GridSprite *)obj->unk10)->attr12)->mode = 1;
    obj_set_fields_18(obj, a2, a3, a4);
    obj_init_class_38fa(obj, 0x1E);
    return obj;
}

s32 avg_fields_sdiv15_a(void *obj)
{
    s32 sum = 0;
    struct AvgSample *e = (struct AvgSample *)((u8 *)obj + 8);
    s32 i;

    for (i = 0; i < 15; i++)
        sum += e[i].v;
    return sum / 15;
}

s32 avg_fields_sdiv15_b(void *obj)
{
    s32 sum = 0;
    struct AvgSample *e = (struct AvgSample *)obj;
    s32 i;

    for (i = 0; i < 15; i++)
        sum += e[i].v;
    return sum / 15;
}

ASM_FUNC("asm/code_08167E7C/path_step_loop_call.s", void path_step_loop_call(void))

NONMATCH("asm/code_08167E7C/path_step_main_call.s", void path_step_main_call(struct GridObj *obj))
{
    ((void (*)(void *))(*(u32 *)((u8 *)obj + 0x234)))(obj);
}
END_NONMATCH

void path_free(struct GridObj *obj, u32 free_flag)
{
    heap_free(obj->path230);
    if (free_flag & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_08167E7C/path_init.s", void path_init(void))

void obj_remove_task_eb10(struct GridObj *obj, u32 flags)
{
    obj->unk18 = (void *)gClassEb10;
    task_remove(obj, flags);
}

struct GridObj *init_vt_08cdeb10(struct GridObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = (void *)gClassEb10;
    obj->unk20 = 0;
    obj->unk1C = 0;
    return obj;
}

void obj_place_child(struct GridObj *obj, u32 mode, u32 attr, s32 *base, u32 record)
{
    s32 scale = obj->scale;
    s32 next;
    u32 word;

    obj->unk18 = (void *)(base[0] - scale * obj->offX);
    obj->unk20 = (void *)(base[2] + scale * obj->offY);
    word = attr & 0xFF;
    ((struct GridSprite *)obj->unk10)->word0C = word << 8;
    if ((s16)obj->counter != 0)
    {
        if (obj->mode56 != 4)
        {
            obj->mode56 = 4;
            obj_load_record(obj->unk10, record + 1, 0, 0, 0);
        }
        if (((s32)((u32)obj->counter << 16) >> 17) & 1)
        {
            obj_release_sprites(obj);
        }
        else
        {
            obj_update_sprites(obj);
        }
        next = obj->counter - 1;
        obj->counter = next;
        if ((s32)((u32)next << 16) <= 0)
        {
            obj->counter = 0;
            obj_update_sprites(obj);
        }
    }
    else if (mode != obj->mode58)
    {
        obj_load_record(obj->unk10, record, 0, 0, 0);
        obj->mode56 = mode;
        if (mode == 0)
        {
            u8 flags = ((struct GridSprite *)obj->unk10)->attr12;

            ((struct GridSprite *)obj->unk10)->attr12 = flags | 0x10;
        }
    }
}

ASM_FUNC("asm/code_08167E7C/create_obj_group_main.s", void create_obj_group_main(void))

ASM_FUNC("asm/code_08167E7C/obj_place_pair.s", void obj_place_pair(void))
