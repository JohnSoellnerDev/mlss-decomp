#include "global.h"

ASM_FUNC("asm/code_08155E7C/transition_step_a.s", void transition_step_a(void))

struct TransSub
{
    u8 filler0[8];
    s16 offset8;
    u16 filler0A;
    void (*fn0C)(void *pos, s32 mode);
};

struct TransStep
{
    s32 state;
    struct TransSub *sub;
    s32 counter;
};

struct TransRec
{
    u8 filler0[4];
    u32 klass4;
    u8 filler8[0xC];
    s32 unk14;
};

extern u8 gGameMode[];
extern u8 *gGamePtr;
extern u8 *gUnk_03001014;
extern u8 gSys[];
extern u8 gText[];
extern u8 gUnk_02000050[];
extern u16 gUnk_02000008;
extern u16 gUnk_02000004;
extern u16 gUnk_02000080;
extern u8 gSceneStateTable_083CCAC0[];
extern const u8 gTransitionClassDec0[];
extern const u8 gHeapTagTransitionObjDec0[];
extern const u8 gHeapTagTransitionObjDea0[];
extern const u8 gHeapTagScanPoolObj[];
extern const u8 gHeapTagWindowObj[];

void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void game_var_write(s32 index, u8 value);
void sync_script_vars(void);
s32 transition_scene_update(u8 *game);
void obj_reset_e400(void *obj);
void *obj_init_alloc(void *obj);
void *transition_states_obj_init_scan_pool(void *obj, u8 *game);
s32 obj_flush_anim(const void *klass);
void obj_release_and_flush(void *obj, u32 mode);
void window_destroy(void *win, u32 flags);
void *window_obj_setup(void *a, u32 b, u32 c, u32 d, u32 e);
void win_set_layout(void *win, const void *layout, u32 x, u32 y);
void obj_free_tile_bufs(void *obj);
void obj_setup_tiles(void *obj, const void *tbl, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
void dma3_copy_tilemap(void *obj);
void set_irq_callback(u32 kind, void *cb);
void hblank_iris_window(void);

#define DISPCNT_SHADOW (*(u16 *)0x02000000)
#define GAME_1064 0x1064
#define GAME_1CA8 0x1CA8
#define GAME_1CAC 0x1CAC
#define GAME_FLAGS 0x1CC4
#define GAME_STATE 0x1F20

NONMATCH("asm/code_08155E7C/transition_states_transition_step_b.s", s32 transition_states_transition_step_b(struct TransStep *obj, u8 *game))
{
    s32 status;
    s32 mode;
    void *mem;
    void *created;
    struct TransSub *sub;
    void **slot;
    void *ob;
    u16 flags;
    s32 counter;

    switch (obj->state) {
    case 0:
        status = transition_scene_update(game);
        if (status != 0)
            sync_script_vars();
        if (status != 1)
            goto check_status_23;
        mode = (s32)((u32)gGameMode[0] << 27) >> 27;
        if (mode <= 5)
            goto path_b;
        if (gGamePtr[GAME_STATE] <= 4)
            goto path_b;
    path_a:
        game_var_write(0x1D, 1);
        sub = obj->sub;
        sub->fn0C((u8 *)obj + sub->offset8, 3);
        mem = heap_alloc2(0x18, 1, gHeapTagTransitionObjDec0, 0);
        obj_reset_e400(mem);
        ((struct TransRec *)mem)->klass4 = (u32)gTransitionClassDec0;
        ((struct TransRec *)mem)->unk14 = 0;
        *(void **)(game + GAME_1CAC) = mem;
        return 1;
    path_b:
        sub = obj->sub;
        sub->fn0C((u8 *)obj + sub->offset8, 3);
        mem = heap_alloc2(0x1C, 1, gHeapTagScanPoolObj, 0);
        created = transition_states_obj_init_scan_pool(mem, game);
        *(void **)(game + GAME_1CAC) = created;
        return 1;
    check_status_23:
        if (status == 2)
            goto path_a;
        if (status != 3)
            return 1;
        mode = (s32)((u32)gGameMode[0] << 27) >> 27;
        if (mode <= 5) {
            game_var_write(0x1D, 0);
        } else {
            ob = *(void **)(gGamePtr + 0x1CB0);
            if (*(s16 *)((u8 *)ob + 0x1C) != 0)
                game_var_write(0x1D, 1);
            else
                game_var_write(0x1D, 0);
        }
        sub = obj->sub;
        sub->fn0C((u8 *)obj + sub->offset8, 3);
        mem = heap_alloc2(0x18, 1, gHeapTagTransitionObjDea0, 0);
        created = obj_init_alloc(mem);
        *(void **)(game + GAME_1CAC) = created;
        return 1;
    case 1:
        obj_free_tile_bufs(game + GAME_1064);
        DISPCNT_SHADOW &= 0xFEFF;
        flags = *(u16 *)(game + GAME_FLAGS);
        *(u16 *)(game + GAME_FLAGS) = flags & 0xFFFB;
        obj->state++;
        return 1;
    case 2:
        DISPCNT_SHADOW |= 0x100;
        slot = (void **)(game + GAME_1CA8);
        mem = heap_alloc2(0xA4, 0, gHeapTagWindowObj, 0);
        mem = window_obj_setup(mem, 0, 2, 0x1C, 0xD);
        *slot = mem;
        *(u16 *)((u8 *)mem + 0xA2) = 0x6F;
        win_set_layout(*slot, (void *)((u32 *)(gText + 0x11C518))[gSys[0x8C0]], 0x35, 0x3D);
        obj->state++;
        return 1;
    case 3:
        slot = (void **)(game + GAME_1CA8);
        status = obj_flush_anim(*slot);
        if (status != -1) {
            obj_release_and_flush(*slot, 0);
            if (status == 1) {
                obj->state = 0xA;
            } else if (status == 2 || status == 0x3E7) {
                DISPCNT_SHADOW &= 0xFEFF;
                obj->state++;
            }
        }
        return 1;
    case 4:
        slot = (void **)(game + GAME_1CA8);
        if (*slot != 0)
            window_destroy(*slot, 3);
        *slot = 0;
        DISPCNT_SHADOW &= 0xFEFF;
        obj->state++;
        return 1;
    case 5:
        obj_setup_tiles(game + GAME_1064,
            *(const void **)((u8 *)gSceneStateTable_083CCAC0 + 0x14 + (game[GAME_STATE] << 5)),
            0x60, 0x20, 0, 0, 0x0600E000, 0);
        dma3_copy_tilemap(game + GAME_1064);
        gUnk_02000008 = 0x1C03;
        DISPCNT_SHADOW |= 0x100;
        flags = *(u16 *)(game + GAME_FLAGS);
        *(u16 *)(game + GAME_FLAGS) = flags | 4;
        obj->state = 0;
        return 1;
    case 10:
        slot = (void **)(game + GAME_1CA8);
        if (*slot != 0)
            window_destroy(*slot, 3);
        *slot = 0;
        obj->counter = 0;
        *(u16 *)(gUnk_03001014 + 2) = 0;
        REG_IME = 0;
        gUnk_02000004 &= 0xFFEF;
        REG_IE &= 0xFFFD;
        REG_IF |= 2;
        REG_IME = 1;
        set_irq_callback(4, hblank_iris_window);
        REG_IME = 0;
        gUnk_02000004 |= 0x10;
        REG_IE |= 2;
        REG_IF |= 2;
        REG_IME = 1;
        obj->state = 0xB;
        return 1;
    case 11:
        *(u16 *)(gUnk_03001014 + 2) = (s16)(obj->counter >> 8);
        counter = obj->counter + 0x200;
        obj->counter = counter;
        if (counter <= 0x5000)
            return 1;
        game_var_write(0x1D, 0);
        if ((gGameMode[1] & 0x80) != 0) {
            *(u16 *)gUnk_02000050 = 0;
            *(u16 *)(gUnk_02000050 + 4) = 0;
        } else {
            *(u16 *)gUnk_02000050 = 0xDF;
            REG_BLDCNT = *(u16 *)gUnk_02000050;
            *(u16 *)(gUnk_02000050 + 4) = 0x1F;
            REG_BLDY = *(u16 *)(gUnk_02000050 + 4);
            gUnk_02000080 = 0;
        }
        sub = obj->sub;
        sub->fn0C((u8 *)obj + sub->offset8, 3);
        return 0;
    default:
        return 1;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_08155E7C/obj_step_ddf00.s", void obj_step_ddf00(void))
