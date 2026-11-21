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

struct SysFlags78
{
    u8 filler0[0x78];
    u16 low10 : 10;
    u16 b10 : 2;
    u16 b12 : 1;
    u16 b13 : 1;
    u16 rest : 2;
};

struct BgReqBits
{
    u8 filler0;
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

/* animation flags of the current object at +0x28 */
struct ObjFlags28
{
    u8 filler0[0x28];
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 pal : 4;
    u8 rest : 2;
};

/* Puts the screen back into the scene's normal display setup: mode 0 with BG0-3 and 1D sprites on;
   if flagged (flags291 bit 1), restores BG0CNT from half294 and clears BG0's first tile and its map;
   turns window 0 off; settles pending pair-ring state (flags2B8); then flushes the other pending
   flags (half244, flagsF8, the sound level from timer1FA) and refreshes the current object's gfx. */
void apply_display_state(struct SceneObj *obj)
{
    // REG_DISPCNT shadow
    *(u16 *)0x02000000 = DISPCNT_OBJ_ON | DISPCNT_BG3_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON
                       | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
    if (obj->flags291 & 2)
    {
        gUnk_02000008 = obj->half294; // REG_BG0CNT shadow
        ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)0x0600C000, 0x20);
        ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)0x0600C800, 0x800);
        ((struct Bit1Of291 *)&obj->flags291)->b1 = 0;
    }
    // DISPCNT bit 13 (window 0 on) in the buffered I/O copy
    ((struct BgReqBits *)gBgRequestPtr)->bit5 = 0;
    if (obj->flags2B8 & 1)
    {
        if (obj->flags2B8 & 0x10)
        {
            if ((((struct SysFlags78 *)gSys)->low10 & 0xF0) == 0xF0)
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
        u32 bits = obj->half20A & 0x180;

        if (bits == 0x100 || (bits == 0 && (((struct SysFlags78 *)gSys)->low10 & 0x33) == 0))
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
    if (((struct ObjFlags28 *)gCurObj)->bit0)
    {
        anim_set_gfx(gCurObj, *(u16 *)((u8 *)gCurObj + 0x20), ((struct ObjFlags28 *)gCurObj)->pal);
    }
    set_flags_2_3(gCurObj);
}

void apply_display_state(struct SceneObj *obj);
struct FcModeBits
{
    u8 low3 : 3;
    u8 mode : 4;
    u8 bit7 : 1;
};

struct SysHalf
{
    u16 h0;
    u16 h2;
};

void sync_and_draw_obj(struct SceneObj *obj, u32 load, u32 p2, u32 p3, u32 p5, u32 p6);
void flush_dirty_blocks(struct SceneObj *obj);
u32 init_bg_view(u8 *obj, u8 mode);
extern u16 gUnk_02000272;

/* Switches the scene object to a display mode (kept in flagsFC bits 3-6).
   1: restore the normal display setup, then as 2: sync and draw the object with the given
   32-byte block mask (1 flushes its pending blocks, 2 loads the masked ones).
   4: publish the object's pending block mask (unkF0, the dirty mask) to gSys (all ones when there
   is none), flush them, restore the display, save gUnk_02000272 and set flagsFD bit 3; then as
   3 and 5: set up the background view (view mode = mode - 3). */
void set_object_mode(struct SceneObj *obj, u32 mode, u32 drawArgA, u16 drawArgB, u16 blockMaskLo, u16 blockMaskHi)
{
    switch (mode)
    {
    case 1:
        apply_display_state(obj);
        /* fall through */
    case 2:
        obj->half128 = drawArgB;
        sync_and_draw_obj(obj, (mode != 1) ? 4 : 0, drawArgA, drawArgB, blockMaskLo, blockMaskHi);
        break;
    case 4:
        if (obj->unkF0 != 0)
        {
            ((struct SysHalf *)gSys)->h0 = obj->unkF0 >> 16;
            ((struct SysHalf *)gSys)->h2 = obj->unkF0;
            *(u32 *)(gSys + 0x884) = obj->unkF0;
            flush_dirty_blocks(obj);
        }
        else
        {
            ((struct SysHalf *)gSys)->h0 = 0xFFFF;
            ((struct SysHalf *)gSys)->h2 = 0xFFFF;
            *(u32 *)(gSys + 0x884) = 0xFFFFFFFF;
        }
        apply_display_state(obj);
        obj->half0FE = gUnk_02000272;
        {
            u32 flags = obj->flagsFD;

            flags |= 8;
            obj->flagsFD = flags;
        }
        /* fall through */
    case 3:
    case 5:
        gUnk_02000272 = 0;
        ((struct SysHalf *)gSys)->h2 |= 0x8000;
        init_bg_view(obj->sub1FC, (u8)(mode - 3));
        obj->half128 = 0;
        break;
    }
    ((struct FcModeBits *)&obj->flagsFC)->mode = mode;
}

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

void reset_pair_slots(struct SceneObj *obj);
void obj_update_entities(struct SceneObj *obj);
void process_obj_entries(struct SceneObj *obj);
void free_block_if_bit0(void *block, u32 flags);
void map_view_map_obj_destroy(void *obj, u32 arg);
void task_remove(void *node, u32 flags);
void free_effect_state(void *p);
void run_pending_obj_callbacks(void);
void menu_obj_free(void *obj, u32 kind);
extern const u8 gEntryUpdateTaskClass[];

/* flags at +0x120, copied from the system flags at +0x78 */
struct ObjFlags120
{
    u8 filler0[0x120];
    u16 low10 : 10;
    u16 b10 : 2;
    u16 b12 : 1;
    u16 b13 : 1;
    u16 rest : 2;
};

struct TaskCls
{
    u8 filler0[0x18];
    const u8 *cls18;
};

/* a child entity's class table (vtable); each entry is a this-adjustment and a method */
struct EntityClass
{
    u8 filler0[0x18];
    s16 teardownThisAdj; /* entry 3: called on every child when the scene object tears down */
    u8 filler1A[0x1C - 0x1A];
    void (*teardown)(void *self);
};

struct ChildEntity
{
    u8 filler0[0x338];
    struct EntityClass *cls;
};

/* Shuts a scene object down before it switches to another mode: settles its pairs, entities and
   entries, takes over the system flags at +0x78, destroys its bg view (sub1FC), map view (sub308),
   entry-update task (sub310), task (sub30C), effect state and menu (sub314), flushes pending
   callbacks, pauses its effect task (sub31C), task slot (sub320) and itself, runs every child
   entity's teardown method and marks it torn down (flagsF9 bit 7). */
void object_teardown(struct SceneObj *obj)
{
    s32 i;

    reset_pair_slots(obj);
    obj_update_entities(obj);
    process_obj_entries(obj);
    ((struct ObjFlags120 *)obj)->low10 = ((struct SysFlags78 *)gSys)->low10;
    ((struct ObjFlags120 *)obj)->b10 = ((struct SysFlags78 *)gSys)->b10;
    ((struct ObjFlags120 *)obj)->b12 = ((struct SysFlags78 *)gSys)->b12;
    ((struct ObjFlags120 *)obj)->b13 = ((struct SysFlags78 *)gSys)->b13;
    if (obj->sub1FC != NULL)
    {
        free_block_if_bit0(obj->sub1FC, 3);
        obj->sub1FC = NULL;
    }
    if (obj->sub308 != NULL)
    {
        map_view_map_obj_destroy(obj->sub308, 3);
        obj->sub308 = NULL;
    }
    if (obj->sub310 != NULL)
    {
        ((struct TaskCls *)obj->sub310)->cls18 = gEntryUpdateTaskClass;
        task_remove(obj->sub310, 3);
        obj->sub310 = NULL;
    }
    if (obj->sub30C != NULL)
    {
        task_remove(obj->sub30C, 3);
        obj->sub30C = NULL;
    }
    free_effect_state(obj->sub31C);
    if (obj->flagsF8 & 1)
    {
        run_pending_obj_callbacks();
        ((struct Bits3Of8 *)&obj->flagsF8)->b0 = 0;
    }
    if (obj->sub314 != NULL)
    {
        menu_obj_free(obj->sub314, 3);
        obj->sub314 = NULL;
    }
    // each of these holds its task node first
    set_flag_2_2(*(void **)obj->sub31C);
    set_flag_2_2(*(void **)obj->sub320);
    set_flag_2_2(obj);
    for (i = 0; i < obj->childCount; i++)
    {
        struct ChildEntity *child;
        struct EntityClass *cls;

        child = obj->entities[i];
        cls = child->cls;
        cls->teardown((u8 *)child + cls->teardownThisAdj);
    }
    {
        u32 flags = obj->flagsF9;

        flags |= 0x80;
        obj->flagsF9 = flags;
    }
}

/* Leaves the scene for the party menu: pauses the current task, stops the serial link, saves the
 * music volume (sound slot 0, gSys + 0x89C) in timer1FA and fades it to half over 8 frames
 * (flagsFC bit 0: restore it on return), tears the scene down, clears and pauses its parts (sub304),
 * removes the IRQ callback, switches decompression to queued jobs and opens the party menu window
 * with the signed field in state12C bits 2-7. */
void object_start_kind(struct SceneObj *obj)
{
    u8 *sys;

    set_flag_2_2(gCurObj);
    serial_link_disable();
    sys = gSys;
    obj->timer1FA = *(u16 *)(sys + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 flags = obj->flagsFC;

        flags |= 1;
        obj->flagsFC = flags;
    }
    object_teardown(obj);
    obj_clear_vram(obj->sub304);
    set_flag_2_2(obj->sub304);
    set_irq_callback(2, 0);
    obj->state = 2;
    gFlowState = 2;
    if (gUnk_03000C78 == 0)
        gUnk_03000C78 = (u32)sub_0800063C; /* queue decompression jobs */
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

/* Leaves the scene for the sign window: saves the music volume (sound slot 0, gSys + 0x89C) in
 * timer1FA and fades it to half over 8 frames (flagsFC bit 0: restore it on return), tears the scene
 * down and releases its parts, removes the IRQ callback, switches decompression to queued jobs and
 * opens the window (window_create, tag gTagSign) for sign half124. */
void object_start_sign(struct SceneObj *obj)
{
    void *window;
    u8 *sys;

    serial_link_disable();
    sys = gSys;
    obj->timer1FA = *(u16 *)(sys + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 flags = obj->flagsFC;

        flags |= 1;
        obj->flagsFC = flags;
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
        gUnk_03000C78 = (u32)sub_0800063C; /* queue decompression jobs */
    window = heap_alloc2(0x4C, 0, gTagSign, 0);
    window_create(window, 8, gTagSign, obj->half124);
}

/* Leaves the scene for the menu screen: stops the serial link, saves the music volume (sound slot 0,
 * gSys + 0x89C) in timer1FA and fades it to half over 8 frames (flagsFC bit 0: restore it on return),
 * tears the scene down and releases its parts, removes the IRQ callback, switches decompression to
 * queued jobs and opens the menu screen with the flag and mode set_scene_state left in flags12D
 * (bit 1 and bits 2-3). */
void obj_start_scene_mode2(struct SceneObj *obj)
{
    u32 request;
    u8 *sys;

    serial_link_disable();
    sys = gSys;
    obj->timer1FA = *(u16 *)(sys + 0x89C);
    set_slot_fields(0, 0x80, 8);
    {
        u32 flags = obj->flagsFC;

        flags |= 1;
        obj->flagsFC = flags;
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
        gUnk_03000C78 = (u32)sub_0800063C; /* queue decompression jobs */
    request = obj->flags12D;
    menu_screen_open((request << 30) >> 31, (request << 28) >> 30);
}

ASM_FUNC("asm/code_08026EA8/obj_reset_sprites_and_state.s", void obj_reset_sprites_and_state(void))

static inline void set_flags2a8_bit5(u8 *flags, u32 v)
{
    ((struct Bits2A8 *)flags)->bit5 = v;
}

/* Stores the setup of the next battle in the scene object: the id and two bytes at 0x2AC (the block
 * obj_start_battle hands to the battle), and the bit fields of flags2A8/2A9 (obj_start_battle copies
 * flags2A9's 3-bit field to 0x2B0). Then sets flags2A8 bit 6. */
void obj_set_draw_attrs(struct SceneObj *obj, u16 id, u8 index, u8 bit5, u32 field2, u32 byte2AE, u32 byte2AF, u32 bit7, u32 bit1, u32 bit0, u32 bit5Of2A9)
{
    obj->half2AC = id;
    obj->byte2AE = byte2AE;
    obj->byte2AF = byte2AF;
    ((struct Bits2A8 *)&obj->flags2A8)->bit7 = bit7;
    ((struct Bits2A9 *)&obj->flags2A9)->bit1 = bit1;
    ((struct Bits2A9 *)&obj->flags2A9)->bit0 = bit0;
    ((struct Bits2A8 *)&obj->flags2A8)->idx = index;
    set_flags2a8_bit5(&obj->flags2A8, bit5);
    ((struct Bits2A9 *)&obj->flags2A9)->field2 = field2;
    ((struct Bits2A9 *)&obj->flags2A9)->bit5 = bit5Of2A9;
    ((struct Bits2A8 *)&obj->flags2A8)->bit6 = 1;
}

ASM_FUNC("asm/code_08026EA8/obj_start_battle.s", void obj_start_battle(void))

void pair_set_mode(void *list, u32 mode, u32 flag);
void obj_reinit(void *list, u16 id, u32 flag, u8 b, s32 x, s32 y, s32 z, s32 zero, s32 w, s32 v, ByteArg f);
ASM_FUNC("asm/code_08026EA8/draw_entity_sprite.s", void draw_entity_sprite(void))

ASM_FUNC("asm/code_08026EA8/obj_reinit.s", void obj_reinit(void *list, u16 id, u32 flag, u8 b, s32 x, s32 y, s32 z, s32 zero, s32 w, s32 v, ByteArg f))

// Leaves the scene after a defeat: tears down its parts if bit 4 of flags12D asks for it, then
// opens the Game Over screen (overlay_init) when var 0x1BE5 holds its kind. Otherwise it frees the
// game state and clears gUnk_03000FC0 (0x570 bytes), then reloads the current save slot if it holds
// a save, or opens the file screen (open_screen) if it doesn't.
void scene_transition_step(struct SceneObj *obj)
{
    u8 gameOverKind;
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
    gameOverKind = read_var(0x1BE5);
    if (gameOverKind != 0)
    {
        set_flag_2_2(obj);
        obj->state = 2;
        block = heap_alloc2(0x68, 0, gTagOver, 0);
        overlay_init(block, 8, gTagOver, gameOverKind);
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
