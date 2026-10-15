#include "global.h"

struct Bits2A8
{
    u8 idx : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Bits2A9
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 field2 : 3;
    u8 bit5 : 1;
    u8 rest : 2;
};

struct Sub200
{
    u8 filler0[0x18];
    s32 unk18;
};

struct Bits12D
{
    u8 low4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct SceneObj
{
    u8 state;
    u8 filler1[0x28 - 0x01];
    void *entities[32];
    u8 filler0A8[0xE4 - 0xA8];
    void *sub0E4;
    u8 filler0E8[0xF0 - 0xE8];
    u32 unkF0;
    u8 filler0F4[0xF8 - 0xF4];
    u8 flagsF8;
    u8 flagsF9;
    u8 filler0FA[0xFC - 0xFA];
    u8 flagsFC;
    u8 flagsFD;
    u16 half0FE;
    u8 filler100[0x102 - 0x100];
    u16 half102;
    u16 half104;
    u16 half106;
    u16 half108;
    u8 byte10A;
    u8 childCount;
    u8 filler10C[0x120 - 0x10C];
    u16 half120;
    u8 filler122[0x124 - 0x122];
    u16 half124;
    u8 filler126[0x128 - 0x126];
    u16 half128;
    u8 flags12A;
    u8 filler12B;
    u8 state12C;
    u8 flags12D;
    u8 filler12E[0x1FA - 0x12E];
    u16 timer1FA;
    void *sub1FC;
    struct Sub200 *sub200;
    void *sub204;
    u8 filler208[0x20A - 0x208];
    u16 half20A;
    u8 filler20C[0x244 - 0x20C];
    u16 half244;
    u8 flags246;
    u8 filler247[0x291 - 0x247];
    u8 flags291;
    u8 filler292[0x294 - 0x292];
    u16 half294;
    u8 filler296[0x2A8 - 0x296];
    u8 flags2A8;
    u8 flags2A9;
    u8 filler2AA[0x2AC - 0x2AA];
    u16 half2AC;
    u8 byte2AE;
    u8 byte2AF;
    u8 filler2B0[0x2B8 - 0x2B0];
    u8 flags2B8;
    u8 filler2B9[0x2D0 - 0x2B9];
    u32 unk2D0;
    u32 unk2D4;
    u8 filler2D8[0x304 - 0x2D8];
    void *sub304;
    void *sub308;
    void *sub30C;
    void *sub310;
    void *sub314;
    void *sub318;
    void *sub31C;
    void *sub320;
};

extern u8 gUnk_0300034C[];
extern void *gUnk_03000D44;
extern u8 gUnk_03000C24;
extern u32 gUnk_03000C78;
extern const u8 gTagSign[];
extern const u8 gTagOver[];
extern const u8 gTagLoad[];
extern u8 *gMemRoutines;
extern u8 *gUnk_03000FC0;
extern u8 gUnk_03000D40;

void serial_link_disable(void);
void set_flag_2_2(void *obj);
void set_slot_fields(u32 idx, u8 level, u8 period);
void object_teardown(struct SceneObj *obj);
void obj_clear_vram(void *obj);
void set_irq_callback(u32 kind, void *cb);
void create_menu_object(s32 a, u32 b);
void obj_release_parts(void *parts, u32 mode);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void window_create(void *block, u32 a, const void *tag, u32 timer);
void menu_screen_open(u32 a, u32 b);
u32 read_var(u16 id);
void overlay_init(void *block, u32 a, const void *tag, u32 b);
void state_free(void);
void state_load(void);
u32 save_slot_in_use(void);
void load_save_slot(void);
void link_window_open(u32 a);
void open_screen(void *block, u32 a, const void *tag, u32 c);
void obj_teardown_subs(struct SceneObj *obj, u32 mode);

ASM_FUNC("asm/code_08026EA8/apply_display_state.s", void apply_display_state(struct SceneObj *obj))

ASM_FUNC("asm/code_08026EA8/set_object_mode.s", void set_object_mode(void))

ASM_FUNC("asm/code_08026EA8/object_setup_wipe.s", void object_setup_wipe(void))

ASM_FUNC("asm/code_08026EA8/object_mode_check.s", void object_mode_check(void))

ASM_FUNC("asm/code_08026EA8/object_teardown.s", void object_teardown(struct SceneObj *obj))

void object_start_kind(struct SceneObj *obj)
{
    u8 *base;

    set_flag_2_2(gUnk_03000D44);
    serial_link_disable();
    base = gUnk_0300034C;
    obj->timer1FA = *(u16 *)(base + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 t = obj->flagsFC;

        t |= 1;
        obj->flagsFC = t;
    }
    object_teardown(obj);
    obj_clear_vram(obj->sub304);
    set_flag_2_2(obj->sub304);
    set_irq_callback(2, 0);
    obj->state = 2;
    gUnk_03000C24 = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = 0x0800063C;
    create_menu_object((s8)obj->state12C >> 2, 0);
}

void object_start_flag(struct SceneObj *obj)
{
    u8 *base;

    set_flag_2_2(gUnk_03000D44);
    serial_link_disable();
    base = gUnk_0300034C;
    obj->timer1FA = *(u16 *)(base + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 t = obj->flagsFC;

        t |= 1;
        obj->flagsFC = t;
    }
    object_teardown(obj);
    obj_clear_vram(obj->sub304);
    set_flag_2_2(obj->sub304);
    set_irq_callback(2, 0);
    obj->state = 2;
    gUnk_03000C24 = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = 0x0800063C;
    create_menu_object(-1, 1);
}

void object_start_sign(struct SceneObj *obj)
{
    void *block;
    u8 *base;

    serial_link_disable();
    base = gUnk_0300034C;
    obj->timer1FA = *(u16 *)(base + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 t = obj->flagsFC;

        t |= 1;
        obj->flagsFC = t;
    }
    object_teardown(obj);
    if (obj->sub304 != 0)
    {
        obj_release_parts(obj->sub304, 3);
        obj->sub304 = 0;
    }
    set_irq_callback(2, 0);
    obj->state = 2;
    gUnk_03000C24 = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = 0x0800063C;
    block = heap_alloc2(0x4C, 0, gTagSign, 0);
    window_create(block, 8, gTagSign, obj->half124);
}

void obj_start_scene_mode2(struct SceneObj *obj)
{
    u32 f;
    u8 *base;

    serial_link_disable();
    base = gUnk_0300034C;
    obj->timer1FA = *(u16 *)(base + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 t = obj->flagsFC;

        t |= 1;
        obj->flagsFC = t;
    }
    object_teardown(obj);
    if (obj->sub304 != 0)
    {
        obj_release_parts(obj->sub304, 3);
        obj->sub304 = 0;
    }
    set_irq_callback(2, 0);
    obj->state = 2;
    gUnk_03000C24 = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = 0x0800063C;
    f = obj->flags12D;
    menu_screen_open((f << 30) >> 31, (f << 28) >> 30);
}

ASM_FUNC("asm/code_08026EA8/obj_reset_sprites_and_state.s", void obj_reset_sprites_and_state(void))

NONMATCH("asm/code_08026EA8/obj_set_draw_attrs.s", void obj_set_draw_attrs(struct SceneObj *obj, u16 value, u8 idx, u8 flag, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u32 a10))
{
    obj->half2AC = value;
    obj->byte2AE = a5;
    obj->byte2AF = a6;
    ((struct Bits2A8 *)&obj->flags2A8)->bit7 = a7;
    ((struct Bits2A9 *)&obj->flags2A9)->bit1 = a8;
    ((struct Bits2A9 *)&obj->flags2A9)->bit0 = a9;
    ((struct Bits2A8 *)&obj->flags2A8)->idx = idx;
    ((struct Bits2A8 *)&obj->flags2A8)->bit5 = flag;
    ((struct Bits2A9 *)&obj->flags2A9)->field2 = a4;
    ((struct Bits2A9 *)&obj->flags2A9)->bit5 = a10;
    ((struct Bits2A8 *)&obj->flags2A8)->bit6 = 1;
}
END_NONMATCH

ASM_FUNC("asm/code_08026EA8/obj_start_battle.s", void obj_start_battle(void))

ASM_FUNC("asm/code_08026EA8/draw_entity_sprite.s", void draw_entity_sprite(void))

ASM_FUNC("asm/code_08026EA8/obj_reinit.s", void obj_reinit(void))

void scene_transition_step(struct SceneObj *obj)
{
    u8 over;
    void *block;

    serial_link_disable();
    if (obj->flags12D & 0x10)
    {
        object_teardown(obj);
        if (obj->sub304 != 0)
        {
            obj_release_parts(obj->sub304, 3);
            obj->sub304 = 0;
        }
        set_irq_callback(2, 0);
        ((struct Bits12D *)&obj->flags12D)->bit4 = 0;
    }
    over = read_var(0x1BE5);
    if (over != 0)
    {
        set_flag_2_2(obj);
        obj->state = 2;
        block = heap_alloc2(0x68, 0, gTagOver, 0);
        overlay_init(block, 8, gTagOver, over);
    }
    else
    {
        if (obj != 0)
            obj_teardown_subs(obj, 3);
        state_free();
        ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FC0, 0x570);
        if ((u8)save_slot_in_use() != 0)
        {
            gUnk_03000D40 |= 2;
            load_save_slot();
            state_load();
            link_window_open(1);
        }
        else
        {
            block = heap_alloc2(0x164, 0, gTagLoad, 0);
            open_screen(block, 8, gTagLoad, 0);
        }
    }
}

ASM_FUNC("asm/code_08026EA8/update_entity_render.s", void update_entity_render(void))

ASM_FUNC("asm/code_08026EA8/camera_track_entity.s", void camera_track_entity(void))
