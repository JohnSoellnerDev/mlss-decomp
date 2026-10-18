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

struct BitsFC16
{
    u16 low3 : 3;
    u16 mode : 4;
    u16 bit7 : 1;
} __attribute__((packed));

struct Sub200
{
    u8 filler0[0x18];
    s32 unk18;
};

struct Entity
{
    u8 filler0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 byte24;
    u8 filler25[0x24C - 0x25];
    s32 unk24C;
    u8 filler250[0x25C - 0x250];
    s32 unk25C;
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
    void *sub0A8[8];
    u8 filler0C8[0xE4 - 0xC8];
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
          u16 half12E;
      u16 half130;
      u8 filler132[0x1FA - 0x132];
    u16 timer1FA;
    void *sub1FC;
    struct Sub200 *sub200;
    void *sub204;
    u8 flags208;
    u8 filler209;
    u16 half20A;
    u8 filler20C[0x244 - 0x20C];
    u16 half244;
    u8 flags246;
          u8 filler247[0x274 - 0x247];
      u16 half274;
      u16 half276;
      u16 half278;
      u16 half27A;
      u16 half27C;
      u16 half27E;
      u16 half280;
      u8 filler282[0x28A - 0x282];
      u8 byte28A;
      u8 byte28B;
      u8 byte28C;
      u8 byte28D;
      u8 filler28E[0x291 - 0x28E];
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
    u8 byte2B9;
    u8 filler2BA[0x2BC - 0x2BA];
    s32 unk2BC;
    s32 unk2C0;
    s32 unk2C4;
    s32 unk2C8;
    s32 unk2CC;
    u32 unk2D0;
    u32 unk2D4;
    u8 flags2D8;
    u8 filler2D9[0x2DA - 0x2D9];
    u16 half2DA;
    s32 unk2DC;
    s32 unk2E0;
    s32 unk2E4;
    s32 unk2E8;
    u8 filler2EC[0x304 - 0x2EC];
    void *sub304;
    void *sub308;
    void *sub30C;
    void *sub310;
    void *sub314;
    void *sub318;
    void *sub31C;
    void *sub320;
};

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
extern u8 gSys[];
extern void *gCurObj;
extern u8 gFlowState;
extern u32 gUnk_03000C78;
void sub_0800063C(void);
extern const u8 gTagSign[];
extern const u8 gTagOver[];
extern const u8 gTagLoad[];
extern u8 *gMemRoutines;
extern u8 *gUnk_03000FC0;
extern u8 gUnk_03000D40;

void serial_link_disable(void);
void set_flag_2_2(void *obj);
void pair_set_mode(void *list, u32 mode, u32 flag);
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

extern u8 *const gBgRequestPtr;
extern u16 gUnk_02000008;
void set_flags_2_3(void *p);
void anim_set_gfx(void *obj, u32 half, u32 field);
void pair_ring_pair_step(struct SceneObj *obj);

struct Bit1Of291
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

struct Bits3Of8
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 rest : 5;
};

struct Bits1C246
{
    u8 low2 : 2;
    u8 f3 : 3;
    u8 rest : 3;
};

struct SysLow10
{
    u8 filler0[0x78];
    u16 low10 : 10;
    u16 rest : 6;
};

struct BgReqBits
{
    u8 filler0;
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

NONMATCH("asm/code_08026EA8/apply_display_state.s", void apply_display_state(struct SceneObj *obj))
{
    *(u16 *)0x02000000 = 0x1F40;
    if (obj->flags291 & 2)
    {
        gUnk_02000008 = obj->half294;
        ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)0x0600C000, 0x20);
        ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)0x0600C800, 0x800);
        ((struct Bit1Of291 *)&obj->flags291)->b1 = 0;
    }
    ((struct BgReqBits *)gBgRequestPtr)->bit5 = 0;
    if (obj->flags2B8 & 1)
    {
        if (obj->flags2B8 & 0x10)
        {
            if ((((struct SysLow10 *)gSys)->low10 & 0xF0) == 0xF0)
            {
                pair_ring_pair_step(obj);
            }
            else if ((obj->flags246 & 0x1C) == 0x14)
            {
                ((struct Bits1C246 *)&obj->flags246)->f3 = 0;
                pair_set_mode(obj, 0, 0);
            }
        }
        ((struct Bits3Of8 *)&obj->flags2B8)->b0 = 0;
    }
    {
        u32 t = obj->half20A & 0x180;

        if (t == 0x100 || (t == 0 && (((struct SysLow10 *)gSys)->low10 & 0x33) == 0))
        {
            obj->half244 = 0xFFC8;
        }
    }
    if (obj->flags12A == 0)
    {
        obj->flags12A = 1;
        ((struct Bits3Of8 *)&obj->flagsF8)->b1 = 0;
        ((struct Bits3Of8 *)&obj->flagsF8)->b2 = 0;
    }
    if (obj->flagsFC & 1)
    {
        set_slot_fields(0, obj->timer1FA >> 8, 8);
        ((struct Bits3Of8 *)&obj->flagsFC)->b0 = 0;
    }
    {
        u32 b28 = ((u8 *)gCurObj)[0x28];

        if (b28 & 1)
        {
            anim_set_gfx(gCurObj, *(u16 *)((u8 *)gCurObj + 0x20), (((u32)b28 << 26) >> 28));
        }
    }
    set_flags_2_3(gCurObj);
}
END_NONMATCH

void apply_display_state(struct SceneObj *obj);
ASM_FUNC("asm/code_08026EA8/set_object_mode.s", void set_object_mode(void))

ASM_FUNC("asm/code_08026EA8/object_setup_wipe.s", void object_setup_wipe(void))

u32 sub_obj_running(void *menu);
u32 task_is_linked(void *task);
u32 object_mode_check(struct SceneObj *obj)
{
    u32 ret;

    if (((struct BitsFC16 *)&obj->flagsFC)->mode <= 2)
    {
        ret = sub_obj_running(obj->sub314);
        if ((u8)ret)
            ret = 1;
        return ret;
    }
    else if (((struct BitsFC16 *)&obj->flagsFC)->mode >= 3 && ((struct BitsFC16 *)&obj->flagsFC)->mode <= 5)
    {
        u8 byte = ((u8 *)obj->sub1FC)[0xA];

        if (byte)
            byte = 1;
        return byte;
    }
    else if (((struct BitsFC16 *)&obj->flagsFC)->mode >= 6 && ((struct BitsFC16 *)&obj->flagsFC)->mode <= 7)
    {
        return (u8)task_is_linked(obj->sub318);
    }
}

ASM_FUNC("asm/code_08026EA8/object_teardown.s", void object_teardown(struct SceneObj *obj))

void object_start_kind(struct SceneObj *obj)
{
    u8 *base;

    set_flag_2_2(gCurObj);
    serial_link_disable();
    base = gSys;
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
    gFlowState = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
    create_menu_object((s8)obj->state12C >> 2, 0);
}

void object_start_flag(struct SceneObj *obj)
{
    u8 *base;

    set_flag_2_2(gCurObj);
    serial_link_disable();
    base = gSys;
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
    gFlowState = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
    create_menu_object(-1, 1);
}

void object_start_sign(struct SceneObj *obj)
{
    void *block;
    u8 *base;

    serial_link_disable();
    base = gSys;
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
    gFlowState = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
    block = heap_alloc2(0x4C, 0, gTagSign, 0);
    window_create(block, 8, gTagSign, obj->half124);
}

void obj_start_scene_mode2(struct SceneObj *obj)
{
    u32 f;
    u8 *base;

    serial_link_disable();
    base = gSys;
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
    gFlowState = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C;
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

void pair_set_mode(void *list, u32 mode, u32 flag);
void obj_reinit(void *list, u16 id, u32 flag, u8 b, s32 x, s32 y, s32 z, s32 zero, s32 w, s32 v, ByteArg f);
ASM_FUNC("asm/code_08026EA8/draw_entity_sprite.s", void draw_entity_sprite(void))

ASM_FUNC("asm/code_08026EA8/obj_reinit.s", void obj_reinit(void *list, u16 id, u32 flag, u8 b, s32 x, s32 y, s32 z, s32 zero, s32 w, s32 v, ByteArg f))

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
