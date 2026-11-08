#include "global.h"
#include "ram_routines.h"

void task_insert(void *node, u8 kind, u32 data);
void task_remove(void *node, u32 kind);
void run_pending_obj_callbacks(void);
void obj_release_parts(void *parts, u32 mode);
void menu_obj_free(void *obj, u32 mode);
void clamp_camera_to_map(u8 *obj, s16 x, s16 y);
void voice_setup_params(void *track, void *src, u32 pitch, u32 a3, u32 a4, u32 a5, u32 a6,
                        u32 a7, u32 a8, u32 a9);
u32 voice_set_data_ptr(void *track, u32 a1, u32 a2, u32 ptr);
u32 track_run(void *track);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void cpu_fast_set(const void *src, void *dst, u32 ctl);
extern u8 gWindowStateClass[];
extern u8 gWindowSubBlockClass[];
extern u8 gSys[];
extern u32 gVoiceListTable[];
extern u8 *gBgRequestPtr;
struct WinObj;
struct TextBox;
void frame_update(void);
void serial_link_enable(void);
void *heap_alloc(u32 size, u32 mode, const void *tag, u32 flags);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void heap_free_a(void *p);
u32 read_varlen_plus1(const void *src);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *sprite, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
void init_obj_09a8(u32 a, u32 b, u32 c, u32 d);
void driver_tables_init(void *block, const void *params);
void set_slot_target(u32 a, u32 b, s32 c);
void set_slot_fields(u32 a, u32 b, u32 c);
void load_tiles_and_voice(struct WinObj *obj, u32 flag);
void textbox_draw_rows(struct TextBox *tb, u32 row);
void window_place(void *window, void *data, u32 x, u32 y, u32 w, u32 h, u32 flags);
u32 apply_display_flags(struct TextBox *tb);
void update_obj_list(void);
void set_slot_value(u32 a, s32 b);
void textbox_setup_tiles(struct TextBox *tb);
void textbox_place(struct TextBox *tb);
void textbox_place_anchor(struct TextBox *tb, u32 a, u32 b);
void serial_schedule(u32 a, u32 b, u32 c);
void serial_link_disable(void);
void heap_free_b(void *p);
void obj_release_slots(void *sprite);
void scene_start_task(void *node, u32 kind, const void *tag, u32 arg);
extern const u8 gTagSceneObj[];
extern u16 gBoxWidthTable[];
extern u8 iwram_overlay[];
extern u8 *gUnk_03000FFC;
extern u32 (*gUnk_03000C84)();
extern const u8 gDescTextbox[];
extern const u8 gDescChild[];
extern const u8 gTagOtdr[];
extern const u8 gTagWinMap[];
extern const u8 gTagDecomp[];
extern const u8 gTagDriver081e37c4[];
extern const u8 gTagVoice[];
extern const u8 gTagTextBuf[];
extern const u8 gTagBwsw[];
extern const u8 gDriverParams[];
extern const u8 gSharedBg083a26d4[];
extern const u8 gOptionsMenuSheet[];
extern const u8 gTextboxFrameTilemap[];
extern const u8 gTextboxObjPaletteA[];
extern const u8 gTextboxObjPaletteB[];
extern const u8 gPaletteRomA[];
extern const u8 gPaletteRomB[];
extern u8 gUnk_02000280[];
extern u16 gUnk_02000008;
extern u16 gUnk_02000010;
extern u16 gUnk_02000050;
extern u16 gUnk_02000054;
extern u16 gUnk_02000080;

struct BoxFlags
{
    u8 mode : 1;
    u8 lang : 3;
    u8 bitA : 1;
    u8 bitB : 1;
    u8 bitC : 1;
    u8 hi : 1;
} __attribute__((packed));

struct TextBox
{
    u8 state;
    u8 filler1[0x14 - 1];
    void *unk14;
    const void *unk18;
    u8 unk1C;
    u8 unk1D;
    u8 filler1E[0x20 - 0x1E];
    struct TextChild *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    void *unk30;
    void *unk34;
    void *unk38;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 filler3F;
    void *unk40;
    u8 unk44;
    u8 unk45;
    u8 filler46[0x48 - 0x46];
    u8 unk48[8];
    void *unk50;
};

struct TextChild
{
    u8 filler0[0x14];
    struct TextBox *owner;
    const void *desc;
};

struct AnimWin
{
    u8 mode;
    s8 count;
    u8 base0;
    u8 base1;
    u8 speed0;
    u8 speed1;
};

struct BgReqBits
{
    u8 filler0;
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

struct Track
{
    u8 filler0[8];
    u8 *cursor;
};

struct SysBusy
{
    u8 filler0[0x888];
    u8 busy;
};

struct WinObj
{
    u8 filler0[0x18];
    u32 klass18;
    s16 unk1C;
    s16 unk1E;
    u16 unk20;
    u16 unk22;
    u16 unk24;
    u16 unk26;
    u8 *unk28;
    u8 filler2C[0xC];
    struct Track *unk38;
    u8 unk3C;
    u8 filler3D[3];
    u8 *unk40;
    u8 *unk44;
    u8 *unk48;
};

void scroll_clamp(struct WinObj *w)
{
    s32 v;
    s32 x;

    if (w->unk1C < 0)
    {
        v = 0;
    }
    else
    {
        x = w->unk1C;
        v = w->unk20 - 0xF0;
        if (x <= v)
        {
            goto skip_x;
        }
    }
    w->unk1C = v;
skip_x:
    if (w->unk1E < 0)
    {
        v = 0;
    }
    else
    {
        x = w->unk1E;
        v = w->unk22 - 0xA0;
        if (x <= v)
        {
            goto skip_y;
        }
    }
    w->unk1E = v;
skip_y:
    clamp_camera_to_map(w->unk44, w->unk1C, w->unk1E);
}

void window_free(struct WinObj *w, u32 kind)
{
    w->klass18 = (u32)gWindowStateClass;
    run_pending_obj_callbacks();
    if (w->unk44 != 0)
    {
        obj_release_parts(w->unk44, 3);
    }
    if (w->unk40 != 0)
    {
        *(u32 *)(w->unk40 + 0x18) = (u32)gWindowSubBlockClass;
        task_remove(w->unk40, 3);
    }
    if (w->unk48 != 0)
    {
        menu_obj_free(w->unk48, 3);
    }
    task_remove(w, kind);
}

void window_sub_block_remove(void *obj, u32 kind)
{
    *(u32 *)((u8 *)obj + 0x18) = (u32)gWindowSubBlockClass;
    task_remove(obj, kind);
}

void *window_sub_block_init(void *obj, u8 kind, u32 data)
{
    task_insert(obj, kind, data);
    *(u32 *)((u8 *)obj + 0x18) = (u32)gWindowSubBlockClass;
    return obj;
}

ASM_FUNC("asm/code_08051AE0/window_place.s", void window_place(void *window, void *data, u32 x, u32 y, u32 w, u32 h, u32 flags))

NONMATCH("asm/code_08051AE0/window_anim_step.s", void window_anim_step(struct AnimWin *win))
{
    s32 countdown;
    s32 prod;
    s32 next;
    u32 speedA;
    u8 scaledA;
    u8 scaledB;
    u8 mode;
    u8 count;

    if (win->count == 0)
    {
        return;
    }
    speedA = win->speed0;
    countdown = win->count;
    prod = speedA * countdown;
    scaledA = prod / 0x10;
    prod = win->speed1 * countdown;
    scaledB = prod / 0x10;

    gBgRequestPtr[0x40] = scaledA + ((speedA >> 1) + win->base0);
    gBgRequestPtr[0x41] = ((win->speed0 >> 1) + win->base0) - scaledA;
    gBgRequestPtr[0x44] = scaledB + ((win->speed1 >> 1) + win->base1);
    gBgRequestPtr[0x45] = ((win->speed1 >> 1) + win->base1) - scaledB;

    mode = win->mode & 1;
    if (mode == 0)
    {
        count = win->count;
        if (win->count == 8)
        {
            ((struct BgReqBits *)gBgRequestPtr)->bit5 = 0;
            win->count = mode;
        }
        else
        {
            win->count = count + 1;
        }
    }
    else
    {
        next = (u8)win->count - 1;
        win->count = next;
        if ((next << 24) == 0)
        {
            ((struct BgReqBits *)gBgRequestPtr)->bit5 = 0;
            *(u16 *)0x02000000 &= 0xFDFF;
        }
    }
}
END_NONMATCH

NONMATCH("asm/code_08051AE0/textbox_open.s", void textbox_open(struct TextBox *tb, u8 prio, u32 tag, u32 mode))
{
    struct TextChild *child;
    u32 v2;
    u32 v3;
    u32 zero;
    u32 timer;
    u8 *buf;
    u8 *map;
    u8 *row;
    u8 *next_row;
    u8 *col_a;
    u8 *col_b;
    u32 tile;
    s32 count;
    s32 next_i;
    s32 i;
    u32 n;
    u32 aligned;
    u32 zeroWord;
    void *sprite;

    task_insert(tb, prio, tag);
    tb->unk18 = gDescTextbox;
    gUnk_02000050 = 0xBF;
    timer = 0x10;
    gUnk_02000054 = timer;
    zero = 0;
    *(u16 *)((u8 *)&gUnk_02000054 - 0x54) = zero;
    frame_update();
    gSys[0x31] = 8;
    tb->unk3C = (tb->unk3C & ~1) | (mode & 1);

    serial_link_enable();
    child = heap_alloc2(0x1C, 0, gTagOtdr, 0);
    task_insert(child, 8, (u32)gTagOtdr);
    child->desc = gDescChild;
    tb->unk20 = child;
    child->owner = tb;
    tb->unk3D = zero;
    tb->unk1C = timer;
    tb->unk40 = heap_alloc(0x800, 1, gTagWinMap, 0);

    v2 = (tb->unk3C & ~0xE) | ((gSys[0x8C0] & 7) << 1);
    tb->unk3C = v2;
    v3 = (((u32)gSys[0x88A] << 27) >> 31) | ((((u32)gSys[0x88B] << 31) >> 31) << 1);
    v3 = v3 | ((((u32)gUnk_03000FFC[8] << 26) >> 31) << 2);
    tb->unk3C = (v2 & ~0x70) | (v3 << 4);
    tb->unk1D = zero;

    ((void (*)(u32, u8 *, u32))gMemRoutines)(zero, (u8 *)tb->unk48, 8);
    buf = heap_alloc(0x8000, 1, gTagDecomp, 0);
    gUnk_03000C84(gSharedBg083a26d4, buf);
    n = read_varlen_plus1(gSharedBg083a26d4);
    cpu_fast_set(buf, (void *)0x06000000, (n >> 2) & 0x1FFFFF);
    gUnk_03000C84(gOptionsMenuSheet, buf);
    n = read_varlen_plus1(gOptionsMenuSheet);
    cpu_fast_set(buf, (void *)0x06010000, (n >> 2) & 0x1FFFFF);

    if (mode == 0)
    {
        cpu_fast_set(gTextboxFrameTilemap, buf, 0x200);
        row = buf + 0x140;
        count = 4;
        do
        {
            *(u16 *)(row + 8) = 1;
            *(u16 *)(row + 0x48) = 3;
            *(u16 *)(row + 0x32) = 0x401;
            *(u16 *)(row + 0x72) = 0x403;
            count--;
            next_row = row + 0xC0;
            col_a = row + 0x0A;
            col_b = row + 0x4A;
            i = 0x13;
            do
            {
                s32 k;
                for (k = 0; k < 10; k++)
                {
                    ((u16 *)col_a)[k] = 2;
                    ((u16 *)col_b)[k] = 0x11;
                }
                col_a += 0x14;
                col_b += 0x14;
                i -= 0xA;
            } while (i >= 0);
            row = next_row;
        } while (count != 0);
        cpu_fast_set(buf, (void *)0x0600D800, 0x200);
    }

    if (buf != 0)
    {
        heap_free_a(buf);
    }
    init_obj_09a8(0x3000, 8, 0, 0x80);
    sprite = create_obj_de4(5, 0x6005, 1, -1, -1, -1, -1);
    tb->unk50 = sprite;
    obj_load_record(tb->unk50, 0, -1, 0, 0);
    if (mode == 0)
    {
        sprite_update_cd0(tb->unk50);
        *(u16 *)tb->unk50 = 0x28;
        *(u16 *)((u8 *)tb->unk50 + 2) = 0x2F;
    }

    if ((((struct SysBusy *)gSys)->busy & 6) == 0)
    {
        cpu_fast_set(gTextboxObjPaletteA, gUnk_02000280, 0x10);
        cpu_fast_set(gPaletteRomA, &gUnk_02000080, 0x78);
    }
    else
    {
        cpu_fast_set(gTextboxObjPaletteB, gUnk_02000280, 0x10);
        cpu_fast_set(gPaletteRomB, &gUnk_02000080, 0x78);
    }

    if (mode == 1)
    {
        gUnk_02000080 = 0x7FFF;
    }
    *(u16 *)gSys = 0xFFFF;
    *(u16 *)(gSys + 2) = 0xFFFF;
    tb->unk34 = heap_alloc((u32)0x0819A4F0 - (u32)iwram_overlay, 0, gTagDriver081e37c4, 1);
    driver_tables_init(tb->unk34, gDriverParams);
    tb->unk38 = heap_alloc2(0x14, 0, gTagVoice, 1);
    tb->unk24 = heap_alloc(0x0001C21F, 1, gTagTextBuf, 1);
    aligned = ((u32)tb->unk24 + 0x1F) & ~0x1F;
    tb->unk28 = (void *)aligned;
    tb->unk2C = (void *)(aligned + 0xE100);
    map = heap_alloc(0xF00, 1, gTagBwsw, 1);
    tb->unk30 = map;

    row = map + 0x140;
    i = 0;
    do
    {
        s8 cnt;

        tile = (((i << 6) + 0x80) | 0xFFFFE000) & 0xFFFF;
        next_i = i + 1;
        next_row = row + 0xC0;
        col_b = row + 0x48;
        col_a = row + 8;
        cnt = 0x15;
        do
        {
            *(u16 *)col_a = tile;
            *(u16 *)col_b = tile + 0x20;
            tile = (tile + 1) & 0xFFFF;
            *(u16 *)(col_a + 2) = tile;
            *(u16 *)(col_b + 2) = tile + 0x20;
            col_b += 4;
            col_a += 4;
            cnt -= 2;
            tile = (tile + 1) & 0xFFFF;
        } while (cnt >= 0);
        i = next_i;
        row = next_row;
    } while (i <= 3);

    cpu_fast_set(map, (void *)0x0600D000, 0x200);
    zeroWord = 0;
    cpu_fast_set(&zeroWord, (void *)0x06004000, 0x01000008);

    load_tiles_and_voice((struct WinObj *)tb, 0);
    ((void (*)(u32, u8 *, u32))gMemRoutines)(0, tb->unk2C, 0xE100);
    textbox_draw_rows(tb, 1);
    frame_update();
    textbox_draw_rows(tb, 2);
    frame_update();
    textbox_draw_rows(tb, 3);
    frame_update();
    textbox_draw_rows(tb, 4);
    frame_update();
    textbox_draw_rows(tb, 5);
    frame_update();
    textbox_draw_rows(tb, 6);
    frame_update();
    textbox_draw_rows(tb, 7);
    frame_update();

    if (mode == 0)
    {
        serial_link_enable();
    }
    else
    {
        window_place(tb->unk48, tb->unk40, 9, 4, 0xC, 0xD, 0xC800);
        tb->unk3E = ((u32)tb->unk3C << 28) >> 29;
        tb->state = 2;
    }

    {
        u16 *bg = &gUnk_02000008;
        u16 *shadow = &gUnk_02000010;
        bg[0] = 0x1805;
        bg[1] = 0x1901;
        bg[2] = 0x1A03;
        bg[3] = 0x1B03;
        shadow[0] = 0;
        shadow[1] = 0;
        shadow[2] = 0;
        shadow[3] = 0;
        shadow[4] = 0;
        shadow[5] = 0;
        shadow[6] = 0;
        shadow[7] = 0;
    }

    if (mode == 0)
    {
        *(u16 *)0x04000000 = 0x1C40;
        set_slot_target(0, 0x1E, -1);
        set_slot_fields(0, 0xFF, 0x10);
    }
    else
    {
        *(u16 *)0x04000000 = 0x3240;
        *(u16 *)0x04000054 = 0;
    }
}
END_NONMATCH

static inline void tb_reset_slots(struct TextBox *tb)
{
    tb->unk1C = 0x10;
    set_slot_fields(0, 0, 0x10);
}

/* Starts the closing animation when the button is pressed; the caller then updates the list. */
static inline void tb_close_step(struct TextBox *tb)
{
    if ((*(u16 *)(gSys + 0x2A) & 1) == 0)
    {
        return;
    }
    *(u16 *)tb->unk50 = 0x28;
    *(u16 *)((u8 *)tb->unk50 + 2) = ((s8)tb->unk3D) * 24 + 0x2F;
    gBgRequestPtr[1] |= 0x20;
    tb->unk48[0] |= 1;
    tb->unk48[1] = 7;
    *(u16 *)0x04000000 &= 0xFEFF;
    gSys[0x88B] |= 1;
    tb->unk1D = 0x10;
    serial_schedule(0, 0x10, 0);
    set_slot_value(0x60, -1);
    tb->state = 1;
}

/* Frame-end tail of the animation states: sprite and display update for anim frame 0. */
static inline void tb_anim_tail(struct TextBox *tb, s8 anim)
{
    if (anim != 0)
    {
        return;
    }
    *(u16 *)tb->unk50 = tb->unk44 + 6;
    *(u16 *)((u8 *)tb->unk50 + 2) = tb->unk45 + 7;
    *(u16 *)0x04000000 |= 0x100;
}

/* Frees the textbox buffers and hands over to the next scene. */
static inline void tb_teardown(struct TextBox *tb, u32 scene_arg, u16 *bldcnt)
{
    void *scene;

    tb->unk18 = gDescTextbox;
    run_pending_obj_callbacks();
    if (tb->unk20 != 0)
    {
        task_remove(tb->unk20, 3);
    }
    if (tb->unk40 != 0)
    {
        heap_free_a(tb->unk40);
    }
    if (tb->unk24 != 0)
    {
        heap_free_a(tb->unk24);
    }
    if (tb->unk30 != 0)
    {
        heap_free_a(tb->unk30);
    }
    if (tb->unk34 != 0)
    {
        heap_free_a(tb->unk34);
    }
    heap_free_b(tb->unk38);
    if ((tb->unk3C & 1) == 0)
    {
        serial_link_disable();
    }
    gSys[0x31] = 2;
    *(u16 *)0x04000000 = 0;
    *bldcnt = 0x10;
    task_remove(tb, 3);
    scene = heap_alloc2(0xE4, 0, gTagSceneObj, 0);
    scene_start_task(scene, 8, gTagSceneObj, scene_arg);
}

NONMATCH("asm/code_08051AE0/textbox_task_step.s", void textbox_task_step(struct TextBox *tb))
{
    u8 state;
    u16 keys;
    s32 cur;
    u32 fl;
    u32 x;
    u32 lang;
    s8 anim;
    u32 flags_bits;

    state = tb->state;
    if (state > 8)
    {
        update_obj_list();
        return;
    }
    switch (state)
    {
    case 0:
        tb->unk1C--;
        gUnk_02000054 = (s8)tb->unk1C;
        if ((s8)tb->unk1C != 0)
        {
            update_obj_list();
            return;
        }
        tb->state = 1;
        update_obj_list();
        return;

    case 1:
        if ((s8)tb->unk1D > 0)
        {
            tb->unk1D--;
        }
        anim = tb->unk48[1];
        if (anim > 0)
        {
            if ((u8)(anim - 1) <= 6)
            {
                textbox_draw_rows(tb, 8 - anim);
            }
            window_anim_step((struct AnimWin *)&tb->unk48);
            update_obj_list();
            return;
        }
        if ((s8)tb->unk1D != 0)
        {
            update_obj_list();
            return;
        }
        keys = *(u16 *)(gSys + 0x2A);
        if (keys & 1)
        {
            if ((s8)tb->unk3D != 0)
            {
                update_obj_list();
                return;
            }
            window_place(&tb->unk48, tb->unk40, 9, 4, 0xC, 0xD, 0xC800);
            tb->unk3E = ((u32)tb->unk3C << 28) >> 29;
            set_slot_value(0x60, -1);
            tb->state = 2;
            update_obj_list();
            return;
        }
        if (keys & 2)
        {
            if (apply_display_flags(tb) != 0)
            {
                tb_reset_slots(tb);
                tb->state = 8;
            }
            else
            {
                tb->state = 6;
            }
            update_obj_list();
            return;
        }
        keys = *(u16 *)(gSys + 0x2E);
        if (keys & 0x40)
        {
            tb->unk3D--;
            if ((s8)tb->unk3D < 0)
            {
                tb->unk3D = 3;
            }
            set_slot_value(0x5F, -1);
        }
        else if (keys & 0x80)
        {
            tb->unk3D++;
            if ((s8)tb->unk3D > 3)
            {
                tb->unk3D = 0;
            }
            set_slot_value(0x5F, -1);
        }
        *(u16 *)((u8 *)tb->unk50 + 2) = ((s8)tb->unk3D) * 24 + 0x2F;
        cur = (s8)tb->unk3D;
        if (cur == 1)
        {
            fl = tb->unk3C;
            x = (fl >> 4) & 7;
            if ((x & 1) == 0)
            {
                if ((*(u16 *)(gSys + 0x2A) & 0x20) == 0)
                {
                    update_obj_list();
                    return;
                }
                tb->unk3C = (fl & ~0x70) | ((x | 1) << 4);
                lang = ((u32)tb->unk3C << 28) >> 29;
                textbox_place_anchor(tb, gBoxWidthTable[lang * 6], gBoxWidthTable[lang * 6 + 1]);
                set_slot_value(0x5F, -1);
                tb->state = 3;
                update_obj_list();
                return;
            }
            if ((*(u16 *)(gSys + 0x2A) & 0x10) == 0)
            {
                update_obj_list();
                return;
            }
            tb->unk3C = (fl & ~0x70) | ((x & 0xFE) << 4);
            set_slot_value(0x5F, -1);
            update_obj_list();
            return;
        }
        if (cur == 2)
        {
            fl = tb->unk3C;
            x = (fl >> 4) & 7;
            if ((gSys[0x888] & 6) != 2)
            {
                update_obj_list();
                return;
            }
            if ((2 & x) == 0)
            {
                if ((*(u16 *)(gSys + 0x2A) & 0x20) == 0)
                {
                    update_obj_list();
                    return;
                }
                tb->unk3C = (fl & ~0x70) | ((2 | x) << 4);
                lang = ((u32)tb->unk3C << 28) >> 29;
                textbox_place_anchor(tb, gBoxWidthTable[lang * 6 + 2], gBoxWidthTable[lang * 6 + 3]);
                set_slot_value(0x5F, -1);
                tb->state = 4;
                update_obj_list();
                return;
            }
            if ((*(u16 *)(gSys + 0x2A) & 0x10) == 0)
            {
                update_obj_list();
                return;
            }
            tb->unk3C = (fl & ~0x70) | ((x & 0xFD) << 4);
            gSys[0x88B] &= ~1;
            set_slot_value(0x5F, -1);
            update_obj_list();
            return;
        }
        if (cur == 3)
        {
            fl = tb->unk3C;
            x = (fl >> 4) & 7;
            if ((x & 4) == 0)
            {
                if ((*(u16 *)(gSys + 0x2A) & 0x20) == 0)
                {
                    update_obj_list();
                    return;
                }
                tb->unk3C = (fl & ~0x70) | ((4 | x) << 4);
                lang = ((u32)tb->unk3C << 28) >> 29;
                textbox_place_anchor(tb, gBoxWidthTable[lang * 6 + 4], gBoxWidthTable[lang * 6 + 5]);
                set_slot_value(0x5F, -1);
                tb->state = 5;
                update_obj_list();
                return;
            }
            if ((*(u16 *)(gSys + 0x2A) & 0x10) == 0)
            {
                update_obj_list();
                return;
            }
            tb->unk3C = (fl & ~0x70) | ((x & 0xFB) << 4);
            set_slot_value(0x5F, -1);
            update_obj_list();
            return;
        }
        update_obj_list();
        return;

    case 2:
        anim = tb->unk48[1];
        if (anim != 0)
        {
            window_anim_step((struct AnimWin *)&tb->unk48);
            anim = tb->unk48[1];
            if (anim != 0)
            {
                update_obj_list();
                return;
            }
            *(u16 *)tb->unk50 = 0x58;
            *(u16 *)((u8 *)tb->unk50 + 2) = ((((u32)tb->unk3C << 28) >> 29) * 18) + 0x2F;
            if (tb->unk3C & 1)
            {
                sprite_update_cd0(tb->unk50);
            }
            update_obj_list();
            return;
        }
        keys = *(u16 *)(gSys + 0x2A);
        if ((keys & 3) == 0)
        {
            keys = *(u16 *)(gSys + 0x2E);
            if (keys & 0x40)
            {
                if ((s8)tb->unk3E == 0)
                {
                    tb->unk3E = 4;
                }
                else
                {
                    tb->unk3E--;
                }
                set_slot_value(0x5F, -1);
            }
            else if (keys & 0x80)
            {
                if ((s8)tb->unk3E == 4)
                {
                    tb->unk3E = 0;
                }
                else
                {
                    tb->unk3E++;
                }
                set_slot_value(0x5F, -1);
            }
            *(u16 *)((u8 *)tb->unk50 + 2) = ((s8)tb->unk3E) * 18 + 0x2F;
            update_obj_list();
            return;
        }
        if (tb->unk3C & 1)
        {
            tb->unk3C = (tb->unk3C & ~0xE) | ((tb->unk3E & 7) << 1);
            if (apply_display_flags(tb) != 0)
            {
                gBgRequestPtr[1] |= 0x20;
                tb->unk48[0] |= 1;
                tb->unk48[1] = 7;
                tb->state = 8;
            }
            else
            {
                tb->state = 6;
            }
            set_slot_value(0x60, -1);
            obj_release_slots(tb->unk50);
            update_obj_list();
            return;
        }
        *(u16 *)tb->unk50 = 0x28;
        *(u16 *)((u8 *)tb->unk50 + 2) = ((s8)tb->unk3D) * 24 + 0x2F;
        if (keys & 1)
        {
            tb->unk3C = (tb->unk3C & ~0xE) | ((tb->unk3E & 7) << 1);
            load_tiles_and_voice((struct WinObj *)tb, 1);
        }
        gBgRequestPtr[1] |= 0x20;
        tb->unk48[0] |= 1;
        tb->unk48[1] = 7;
        set_slot_value(0x60, -1);
        tb->state = 1;
        update_obj_list();
        return;

    case 3:
        anim = tb->unk48[1];
        if (anim == 0)
        {
            tb_close_step(tb);
            update_obj_list();
            return;
        }
        window_anim_step((struct AnimWin *)&tb->unk48);
        anim = tb->unk48[1];
        if (anim == 6)
        {
            dma_copy_or_queue(tb->unk2C, (void *)0x06004020, 0x4B00, 0x20, 0);
            update_obj_list();
            return;
        }
        if (anim == 7)
        {
            dma_copy_or_queue(tb->unk30, (void *)0x0600C000, 0x500, 0x20, 0);
            update_obj_list();
            return;
        }
        tb_anim_tail(tb, anim);
        update_obj_list();
        return;

    case 4:
        anim = tb->unk48[1];
        if (anim != 0)
        {
            window_anim_step((struct AnimWin *)&tb->unk48);
            anim = tb->unk48[1];
            if (anim == 6)
            {
                dma_copy_or_queue((u8 *)tb->unk2C + 0x4B00, (void *)0x06004020, 0x4B00, 0x20, 0);
                update_obj_list();
                return;
            }
            if (anim == 7)
            {
                dma_copy_or_queue((u8 *)tb->unk30 + 0x500, (void *)0x0600C000, 0x500, 0x20, 0);
                update_obj_list();
                return;
            }
            tb_anim_tail(tb, anim);
            update_obj_list();
            return;
        }
        tb_close_step(tb);
        update_obj_list();
        return;

    case 5:
        anim = tb->unk48[1];
        if (anim == 0)
        {
            tb_close_step(tb);
            update_obj_list();
            return;
        }
        window_anim_step((struct AnimWin *)&tb->unk48);
        anim = tb->unk48[1];
        if (anim == 6)
        {
            dma_copy_or_queue((u8 *)tb->unk2C + 0x9600, (void *)0x06004020, 0x4B00, 0x20, 0);
            update_obj_list();
            return;
        }
        if (anim == 7)
        {
            dma_copy_or_queue((u8 *)tb->unk30 + 0xA00, (void *)0x0600C000, 0x500, 0x20, 0);
            update_obj_list();
            return;
        }
        tb_anim_tail(tb, anim);
        update_obj_list();
        return;

    case 6:
        textbox_setup_tiles(tb);
        textbox_place(tb);
        tb->unk1C = 0xF0;
        tb->state = 7;
        update_obj_list();
        return;

    case 7:
        anim = tb->unk48[1];
        if (anim != 0)
        {
            window_anim_step((struct AnimWin *)&tb->unk48);
            anim = tb->unk48[1];
            if (anim == 6)
            {
                dma_copy_or_queue(tb->unk2C, (void *)0x06004020, 0x4B00, 0x20, 0);
            }
            else if (anim == 7)
            {
                dma_copy_or_queue(tb->unk30, (void *)0x0600C000, 0x500, 0x20, 0);
            }
            else if (anim == 0)
            {
                *(u16 *)0x04000000 |= 0x100;
            }
            update_obj_list();
            return;
        }
        tb->unk1C--;
        if (tb->unk1C != 0 && (*(u16 *)(gSys + 0x2A) & 0xB) == 0)
        {
            update_obj_list();
            return;
        }
        if (tb->unk3C & 1)
        {
            gBgRequestPtr[1] |= 0x20;
            tb->unk48[0] |= 1;
            tb->unk48[1] = 7;
        }
        else
        {
            tb_reset_slots(tb);
        }
        tb->state = 8;
        update_obj_list();
        return;

    case 8:
        if ((tb->unk3C & 1) == 0)
        {
            tb->unk1C--;
            gUnk_02000054 = 0x10 - (s8)tb->unk1C;
            if ((s8)tb->unk1C != 0)
            {
                update_obj_list();
                return;
            }
            tb_teardown(tb, 3, (u16 *)0x04000050);
            return;
        }
        window_anim_step((struct AnimWin *)&tb->unk48);
        anim = tb->unk48[1];
        if (anim != 0)
        {
            update_obj_list();
            return;
        }
        tb_teardown(tb, 0, &gUnk_02000050);
        return;
    }
    update_obj_list();
}
END_NONMATCH

ASM_FUNC("asm/code_08051AE0/apply_display_flags.s", u32 apply_display_flags(struct TextBox *tb))

void load_tiles_and_voice(struct WinObj *obj, u32 flag)
{
    s32 voice;
    u32 last;

    ((void (*)(u32, u8 *, u32))gMemRoutines)(0, obj->unk28, 0x2000);
    voice = 0;
    do
    {
        if ((((struct SysBusy *)gSys)->busy & 6) != 2 && voice == 2)
        {
            voice_setup_params(obj->unk38, obj->unk28, 0x20, 4, 0, 0, 1, voice, 7, 0);
        }
        else
        {
            voice_setup_params(obj->unk38, obj->unk28, 0x20, 4, 0, 0, 1, 2, 5, 0);
        }
        voice_set_data_ptr(obj->unk38, 0, (u8)((voice << 4) + 2),
                           ((u32 *)gVoiceListTable[voice])[((u32)obj->unk3C << 28) >> 29] + 2);
        voice++;
        do
        {
            struct Track *track = obj->unk38;
            u8 *cursor = track->cursor;

            if (cursor[0] == 0xFF && cursor[1] == 0xB && cursor[2] == 1)
            {
                track->cursor = cursor + 3;
            }
            last = (u16)track_run(obj->unk38);
        } while (last != 0);
    } while (voice <= 3);

    if (flag != 0)
    {
        dma_copy_or_queue(obj->unk28, (void *)0x06001000, 0x2000, 0x20, last);
    }
    else
    {
        cpu_fast_set(obj->unk28, (void *)0x06001000, 0x800);
    }
}

ASM_FUNC("asm/code_08051AE0/textbox_draw_rows.s", void textbox_draw_rows(struct TextBox *tb, u32 row))
