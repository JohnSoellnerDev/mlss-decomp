#include "global.h"

struct Obj
{
    u8 filler0[4];
    void *unk4;
    struct Obj *unk8;
    struct Obj *unkC;
    s16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u8 filler1A[0x40 - 0x1A];
    s32 unk40;
    s32 unk44;
    u8 filler48[0x4C - 0x48];
    void *unk4C;
    u8 filler50[0x79 - 0x50];
    u8 unk79;
    u8 filler7A[0x80 - 0x7A];
    void *unk80;
    u8 filler84[0xE0 - 0x84];
    s32 unkE0;
};

struct Fig
{
    u8 filler0[0x7E];
    u8 unk7E;
    u8 filler7F[0xEC - 0x7F];
    s16 unkEC;
};

struct SysState
{
    u8 filler0[0x3E];
    u16 held;
    u8 filler40[0x880 - 0x40];
    u16 *oamPtr;
};

struct MarkerPos
{
    u8 filler0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x84 - 0x1C];
    s32 unk84;
    s32 unk88;
    s32 unk8C;
};

struct Flags79
{
    u8 low : 4;
    u8 bit4 : 1;
    u8 high : 3;
};

struct World
{
    u8 filler0[7];
    u8 modeByte;
    u8 filler8[0x0B - 0x08];
    u8 flags0B;
    u8 filler0C[0x0D - 0x0C];
    u8 flags0D;
    u8 filler0E[0x7C - 0x0E];
    struct Obj *active;
    struct Fig *fig1;
    struct Fig *fig2;
    u8 filler88[0x1F0 - 0x88];
    u8 *tileBank;
    u8 filler1F4[0x250 - 0x1F4];
    void *slot4Tiles;
    u8 filler254[0x278 - 0x254];
    struct Obj *markerObj;
    struct Obj *marks[2];
    struct Obj *slotObjs[2];
    struct Obj *effect;
    struct Obj *timerObj;
    u8 filler294[0x2BC - 0x294];
    u8 uploadState;
    u8 filler2BD[0x2C0 - 0x2BD];
    u8 figMode;
    u8 filler2C1[0x2F9 - 0x2C1];
    u8 stateByte;
    u8 filler2FA[0x2FC - 0x2FA];
    struct Obj *slotTable[4];
    u32 statusValue;
    struct Obj *statusObj;
    u16 counter314;
};

struct FigModeBits
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct PartnerState
{
    u8 filler0[0x7E];
    u8 unk7E;
    u8 filler7F[0xE4 - 0x7F];
    u8 *unkE4;
    u8 fillerE8[0xEC - 0xE8];
    s16 unkEC;
    u8 fillerEE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x104 - 0xF8];
    s16 unk104;
    u8 filler106[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
    u8 filler10E[0x111 - 0x10E];
    u8 unk111;
    u8 filler112[0x11E - 0x112];
    u16 unk11E;
    u8 unk120;
};

struct ObjAux
{
    u8 filler0[8];
    u8 unk8;
    u8 filler9[3];
    u16 unkC;
};

struct PartnerObj
{
    u8 filler0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x28 - 0x1C];
    struct PartnerState *unk28;
    u8 filler2C[4];
    void *unk30;
    u8 filler34[0x4C - 0x34];
    void *unk4C;
    u8 filler50[4];
    void *unk54;
    void (*unk58)(void *, void *, u32);
    u8 filler5C[0x6C - 0x5C];
    struct ObjAux *unk6C;
    u8 filler70[0xB2 - 0x70];
    u16 unkB2;
    u8 fillerB4[0xB8 - 0xB4];
    u16 unkB8;
    u16 unkBA;
    u16 unkBC;
    u16 unkBE;
    u16 unkC0;
    u8 unkC2;
};

extern struct World *gWorld;
extern u16 gUnk_0200001A;
extern u16 gUnk_0200001E;
extern u16 gUnk_02000048;

void slide_window_panel(void);
void panel_slide_end(void);
void bg_slide_in(struct Obj *obj);
void bg_slide_offset_callback(struct Obj *obj);
void bg_restore_after_sprite(struct Obj *obj);
void child_slide_to_bg2(struct Obj *obj);
void scroll_text_step(struct Obj *obj);
void voice_message_step(struct Obj *obj);
void start_voice_message(struct Obj *obj);
void status_slots_update(struct Obj *owner);
void status_slots_setup(struct Obj *owner);
void step_slot_object(void);
void update_slot_display(u32 index);
void place_world_object(s32 x, s32 y, s32 z, struct Obj *obj, u32 mode);
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
struct Obj *list_add_task(void *list, void *next);
struct Obj *task_create_list_4c(u32 next, u32 obj);
s32 measure_text(u32 font, void *text, u32 pad);
u32 obj_motion_start(struct Obj *obj, s32 x, s32 y, s32 height, s32 extra);
u32 dma_copy_or_queue(u32 src, u32 dst, u32 count, u32 width, u32 mode);
extern u8 *gWorkBuf;
extern struct SysState gSys;
void figure_select_action(u32 arg);
void set_tile_slot(void *slot, u32 idx, u32 flag, u32 value, u32 p5, u32 p6, u32 p7);
void draw_slot_frame(void *slot, u32 idx, u32 rect, u32 kind);
void draw_slot_text(void *slot, u32 idx, u32 x, u32 y, void *text, u32 color);
void copy_slot_tiles(void *slot, u32 idx, u8 *dst, u32 off, u32 count);
void draw_obj_display(void);
void draw_panel_values(void);
void draw_counter_display(void);
void countdown_world_counter(struct Obj *obj);
extern u8 gGaugeTableC[];
struct GlyphIndexTable
{
    u8 filler0[0xE];
    u16 index;
    u16 index10;
};

extern u32 gMapTable[];
extern struct GlyphIndexTable gGlyphIndexTable;
void spawn_slot_marker(u32 slot);
void update_slot_markers(void);
void obj_release(struct Obj *obj);
void obj_release_node(struct Obj *node);
void obj_stop(void *obj);
u32 mt19937_next(void);
u32 rand_below(u32 n, u32 rnd);
void set_slot_value(u32 id, s32 value);
void release_tile_slot(void *obj, u8 idx);
u8 motion_step(struct Obj *obj);
struct Obj *raster_line_add(void *fn, u32 kind, u32 arg);
void timer_window_setup(struct Obj *obj);
void update_voice_message(void);

void release_slot_objects(void)
{
    struct Obj *slot = gWorld->slotObjs[0];

    if (slot != 0)
    {
        obj_release_node(slot->unk8);
        gWorld->slotObjs[0] = 0;
        slot->unk4 = 0;
    }
    slot = gWorld->slotObjs[1];
    if (slot != 0)
    {
        obj_release_node(slot->unk8);
        gWorld->slotObjs[1] = 0;
        slot->unk4 = 0;
    }
}

void release_slot_object(u32 idx)
{
    struct Obj *slot = gWorld->slotObjs[idx];

    if (slot != 0)
    {
        obj_release_node(slot->unk8);
        gWorld->slotObjs[idx] = 0;
        slot->unk4 = 0;
    }
}

struct Obj *unknown_thumb_0810d74c(u32 index)
{
    struct Obj *slot;
    struct Obj *node;

    if (gWorld->flags0B & 8)
    {
        return 0;
    }
    slot = gWorld->slotObjs[index];
    if (slot != 0)
    {
        return slot;
    }
    node = list_add_task((u8 *)gWorld + 0x48, step_slot_object);
    node->unk8 = slot;
    node->unk16 = (u32)slot;
    gWorld->slotObjs[index] = node;
    return node;
}

void update_slot_markers(void)
{
    if ((gWorld->fig1->unk7E & 6) == 2)
    {
        update_slot_display(0);
    }
    else
    {
        release_slot_object(0);
    }
    if (gWorld->fig2 != 0)
    {
        if ((gWorld->fig2->unk7E & 6) == 2)
        {
            update_slot_display(1);
        }
        else
        {
            release_slot_object(1);
        }
    }
}

void set_slot_state(u32 state)
{
    gWorld->stateByte = state;
    update_slot_markers();
}

void unknown_thumb_0810d818(u32 index)
{
    struct Obj *mark = gWorld->marks[index];

    if (mark != 0)
    {
        mark->unk4 = panel_slide_end;
    }
}

void set_mark_callbacks(void)
{
    if (gWorld->marks[0] != 0)
    {
        gWorld->marks[0]->unk4 = panel_slide_end;
    }
    if (gWorld->marks[1] != 0)
    {
        gWorld->marks[1]->unk4 = panel_slide_end;
    }
}

NONMATCH("asm/code_0810D6C0/set_mark_state.s", void set_mark_state(u32 index, u32 value))
{
    struct Obj *mark = gWorld->marks[index];
    void *callback;

    if (mark != 0)
    {
        if ((s16)mark->unk16 != (s32)value)
        {
            mark->unk16 = value;
            dma_copy_or_queue((u32)(gWorld->tileBank + (((value * 3) << 8) + 0x1880)), 0x06010DE0, 0x300, 0x20, 0);
            if (value == 1)
            {
                callback = draw_counter_display;
            }
            else if ((s32)value > 1)
            {
                if (value != 2)
                {
                    return;
                }
                callback = draw_counter_display;
            }
            else if (value == 0)
            {
                callback = draw_panel_values;
            }
            else
            {
                return;
            }
            mark->unk4 = callback;
        }
    }
}
END_NONMATCH

void queue_mark_display(void)
{
    u16 idx = gGlyphIndexTable.index;
    u32 *entry = gMapTable + idx;
    u8 *text = (u8 *)gMapTable + *entry;

    queue_display_entry(0, text, gWorld->tileBank + 0x1880, 0x900, 0, 0);
}

void spawn_slot_markers(void)
{
    spawn_slot_marker(0);
    spawn_slot_marker(1);
}

void bg_restore_while_busy(struct Obj *obj)
{
    if (obj->unk8->unk4 == 0)
    {
        obj->unk4 = 0;
    }
    else
    {
        REG_BG2VOFS = gUnk_0200001A;
        REG_BG3VOFS = gUnk_0200001E;
    }
}

void bg_slide_timer(struct Obj *obj)
{
    s32 t = 0x19 - obj->unk8->unk14;

    obj->unk10 = t;
    if ((s16)t > 0x17)
    {
        obj->unk4 = 0;
    }
    else
    {
        REG_BG2VOFS = gUnk_0200001A - 0x18;
        REG_BG3VOFS = gUnk_0200001E - 0x18;
    }
}

void bg_slide_out(struct Obj *obj)
{
    s32 t = obj->unk14 - 3;

    obj->unk14 = t;
    if ((s16)t <= 0)
    {
        obj->unk4 = 0;
    }
    else
    {
        REG_BG2VOFS = gUnk_0200001A + t;
        REG_BG3VOFS = *(vu16 *)&gUnk_0200001E + obj->unk14;
    }
}

void battle_bg_scroll_callback(struct Obj *obj)
{
    *(vs16 *)REG_ADDR_BG2VOFS = -0x1A;
    REG_BG3VOFS = gUnk_0200001E - 0x20;
    obj->unk4 = 0;
}

void spawn_timer_child(void)
{
    struct Obj *child = raster_line_add(slide_window_panel, 0x86, 0);

    child->unk14 = 0;
}

void spawn_linked_children(void)
{
    struct Obj *first = raster_line_add(bg_slide_in, 0, 0);

    first->unk14 = 0;
    raster_line_add(bg_restore_after_sprite, 0x18, 0)->unk8 = first;
}

void clear_world_effect(void)
{
    struct Obj *effect = gWorld->effect;

    if (effect != 0)
    {
        gWorld->effect = 0;
        effect->unk4 = 0;
        release_tile_slot(gWorld->slot4Tiles, 2);
        release_tile_slot(gWorld->slot4Tiles, 3);
    }
}

void copy_gauge_tiles(struct Obj *obj)
{
    u8 *bank = gWorld->tileBank;
    s32 n = (s16)obj->unk16;
    s32 width;
    void *slot;
    u32 value;

    if (n > 5)
    {
        n = 5;
    }
    slot = gWorld->slot4Tiles;
    width = (n * 11 + 8) / 8;
    value = gGaugeTableC[(u32)obj->unkC & 0xF] * width;
    copy_slot_tiles(slot, 2, bank, 0, value);
    obj->unk4 = draw_obj_display;
}

void timer_object_state_db08(struct Obj *obj)
{
    obj->unkC->unk4 = 0;
    obj->unk4 = 0;
    gWorld->timerObj = 0;
}

void release_timer_tiles(void)
{
    if (gWorld->timerObj != 0)
    {
        release_tile_slot(gWorld->slot4Tiles, 4);
        release_tile_slot(gWorld->slot4Tiles, 5);
        gWorld->timerObj->unk4 = timer_object_state_db08;
    }
}

ASM_FUNC("asm/code_0810D6C0/timer_window_setup.s", void timer_window_setup(struct Obj *obj))

void voice_message_step(struct Obj *obj)
{
    struct Obj *slot = obj->unkC;

    slot->unk16 -= 3;
    if (*(vu16 *)&gUnk_0200001A - 0xF > (s16)slot->unk16)
    {
        slot->unk16 = *(vu16 *)&gUnk_0200001A - 0xF;
        start_voice_message(obj);
        obj->unk4 = child_slide_to_bg2;
    }
}

void scroll_text_step(struct Obj *obj)
{
    if (gWorld->flags0B & 0x40)
    {
        voice_message_step(obj);
    }
    else
    {
        u16 cur = obj->unk16;

        if ((s16)obj->unk16 > 0)
        {
            obj->unk16 = cur - 1;
        }
        else if ((s16)obj->unk18 == 0)
        {
            obj->unk12 += 0x40;
        }
    }
}

NONMATCH("asm/code_0810D6C0/start_timer_display.s", void start_timer_display(void))
{
    struct Obj *timer;
    struct Obj *child;
    u16 *win;

    if (gWorld->timerObj == 0)
    {
        timer = task_create_list_4c((u32)scroll_text_step, 0);
        gWorld->timerObj = timer;
        gWorld->flags0B = 0x40 | gWorld->flags0B;
        child = raster_line_add(timer_window_setup, 0x86, (u32)update_voice_message);
        gWorld->timerObj->unkC = child;
        child->unk16 = gUnk_0200001A;
        win = (u16 *)&gUnk_02000048;
        *win = 0x3F3F;
        win++;
        *win = 0x3F3B;
    }
}
END_NONMATCH

void timer_window_setup(struct Obj *obj);
void update_voice_message(void);
ASM_FUNC("asm/code_0810D6C0/show_slot4_text_centered.s", void show_slot4_text_centered(void))

void release_slot4_tiles(void)
{
    release_tile_slot(gWorld->slot4Tiles, 0);
    release_tile_slot(gWorld->slot4Tiles, 1);
}

void show_slot4_text_left(void *text)
{
    release_slot4_tiles();
    set_tile_slot(gWorld->slot4Tiles, 0, 0, 0xD, 0, 0x12, 3);
    draw_slot_frame(gWorld->slot4Tiles, 1, 0, 0);
    draw_slot_text(gWorld->slot4Tiles, 0, 7, 6, text, 0x91);
}

void upload_obj_tiles(u32 index)
{
    u16 idx = index;
    u8 state = gWorld->uploadState;

    if (state > 0x13)
    {
        dma_copy_or_queue((u32)(gWorkBuf + ((idx << 9) + 0x4E70)), 0x060113A0, 0x100, 0x20, 0);
    }
    else if ((u8)(state - 1) <= 0x11)
    {
        dma_copy_or_queue((u32)(gWorkBuf + ((((idx << 4) + 8) << 5) + 0x4E70)), 0x060113A0, 0x100, 0x20, 0);
    }
}

void queue_obj_upload_entry(void)
{
    u16 idx = gGlyphIndexTable.index10;
    u32 *entry = gMapTable + idx;
    u8 *text = (u8 *)gMapTable + *entry;

    queue_display_entry(0, text, gWorkBuf + 0x4E70, -1, 0, 0);
    gWorld->uploadState = 0;
}

void release_marker_object(void)
{
    struct Obj *marker = gWorld->markerObj;

    if (marker != 0)
    {
        marker->unk80 = 0;
        obj_release(marker);
        gWorld->markerObj = 0;
    }
}

void release_sprite_state(struct Obj *obj)
{
    struct Obj *sprite = obj->unk8;

    ((struct Flags79 *)&sprite->unk79)->bit4 = 0;
    if (sprite->unk80 != 0)
    {
        if (motion_step(sprite) == 0)
        {
            return;
        }
    }
    obj->unk4 = 0;
}

void place_at_active(struct Obj *obj)
{
    struct Obj *active = gWorld->active;

    place_world_object(active->unk40 / 0x100 - 4, active->unk44 / 0x100, active->unkE0 / 0x100 + 0x18, obj, 0x7001);
}

void set_marker_position(s32 x, s32 y, s32 z)
{
    struct MarkerPos *marker = (struct MarkerPos *)gWorld->markerObj;

    marker->unk84 = x << 8;
    marker->unk10 = x << 8;
    marker->unk88 = y << 8;
    marker->unk14 = y << 8;
    marker->unk8C = z << 8;
    marker->unk18 = z << 8;
}

s32 number_right_edge(u32 value, s32 x)
{
    s32 width = 0;

    do
    {
        value >>= 4;
        width += 4;
    } while (value != 0);
    return x + width;
}

void draw_digits_c0_p12(u32 value, u32 x, s32 y, s32 right)
{
    u16 *oam;

    if (right != 0)
    {
        x = number_right_edge(value, x);
    }
    y &= 0xFF;
    oam = gSys.oamPtr;
    do
    {
        s32 tile = (value & 0xF) * 2 + 0xC0;

        x = (x - 8) & 0x1FF;
        oam[0] = y | 0x8000;
        oam[1] = x;
        oam[2] = tile | 0xC000;
        oam += 4;
        value >>= 4;
    } while (value != 0);
    gSys.oamPtr = oam;
}

void draw_digits_49_p12(u32 value, u32 x, s32 y, s32 right)
{
    s16 *oam;

    if (right != 0)
    {
        x = number_right_edge(value, x);
    }
    y &= 0xFF;
    oam = (s16 *)gSys.oamPtr;
    do
    {
        s32 tile = (value & 0xF) * 2 + 0x49;

        x = (x - 8) & 0x1FF;
        oam[0] = y | 0x8000;
        oam[1] = x;
        oam[2] = tile | 0xC000;
        oam += 4;
        value >>= 4;
    } while (value != 0);
    gSys.oamPtr = (u16 *)oam;
}

void draw_digits_c0_p11(u32 value, u32 x, s32 y, s32 right)
{
    u16 *oam;

    if (right != 0)
    {
        x = number_right_edge(value, x);
    }
    y &= 0xFF;
    oam = gSys.oamPtr;
    do
    {
        s32 tile = (value & 0xF) * 2 + 0xC0;

        x = (x - 8) & 0x1FF;
        oam[0] = y | 0x8000;
        oam[1] = x;
        oam[2] = tile | 0xB000;
        oam += 4;
        value >>= 4;
    } while (value != 0);
    gSys.oamPtr = oam;
}

void draw_digits_49_p11(u32 value, u32 x, s32 y, s32 right)
{
    s16 *oam;

    if (right != 0)
    {
        x = number_right_edge(value, x);
    }
    y &= 0xFF;
    oam = (s16 *)gSys.oamPtr;
    do
    {
        s32 tile = (value & 0xF) * 2 + 0x49;

        x = (x - 8) & 0x1FF;
        oam[0] = y | 0x8000;
        oam[1] = x;
        oam[2] = tile | 0xB000;
        oam += 4;
        value >>= 4;
    } while (value != 0);
    gSys.oamPtr = (u16 *)oam;
}

u32 status_anim_wait(void)
{
    u32 value = gWorld->statusValue;

    gWorld->modeByte = (gWorld->modeByte & 0x1F) | 0xA0;
    while (value == 0)
    {
    }
    return obj_motion_start((struct Obj *)value, gWorld->active->unk40, gWorld->active->unk44, 0x3000, 0x3800);
}

NONMATCH("asm/code_0810D6C0/status_anim_start.s", u32 status_anim_start(void))
{
    struct World *world = gWorld;
    struct Obj **entry = (struct Obj **)((u8 *)world + 0x2FC) + world->statusObj->unk10;
    struct Obj *slot = *entry;
    struct Obj *active = world->active;
    s32 y = active->unk44 - 0x800;

    world->statusValue = (u32)slot;
    *entry = 0;
    return obj_motion_start(slot, active->unk40 - 0x2000, y, 0, 0x3800);
}
END_NONMATCH

void set_status_word(void)
{
    gWorld->statusObj->unk18 = 0x241;
}

void status_wait_slot(struct Obj *obj)
{
    struct Obj *slot;

    status_slots_update(obj);
    slot = gWorld->slotTable[gWorld->statusObj->unk10];
    if (slot->unk4C == 0)
    {
        gWorld->modeByte &= 0x1F;
        obj->unk4 = status_slots_update;
    }
}

s32 set_status_entry(u32 a, u32 b, u32 c)
{
    struct Obj *obj;

    obj = gWorld->statusObj;
    if (obj == 0)
    {
        obj = task_create_list_4c(0, 0);
        gWorld->statusObj = obj;
        obj->unk12 = a;
        obj->unk14 = c;
        obj->unkC = (struct Obj *)b;
        obj->unk18 = 0;
        status_slots_setup(obj);
    }
    else
    {
        obj->unk12 = a;
        obj->unk14 = c;
        obj->unkC = (struct Obj *)b;
        obj->unk4 = countdown_world_counter;
    }
}

void hero_select_input(void)
{
    if (gSys.held & 1)
    {
        if (gWorld->fig1->unkEC == -1)
        {
            figure_select_action(0);
        }
        else
        {
            figure_select_action(1);
        }
    }
    if (gSys.held & 2)
    {
        if (gWorld->fig1->unkEC == -2)
        {
            figure_select_action(0);
        }
        else
        {
            figure_select_action(1);
        }
    }
}

void bg_restore_after_sprite(struct Obj *obj)
{
    if (obj->unk8->unk4 == 0)
    {
        obj->unk4 = 0;
    }
    REG_BG2VOFS = gUnk_0200001A;
    REG_BG3VOFS = gUnk_0200001E;
}

ASM_FUNC("asm/code_0810D6C0/bg_slide_in.s", void bg_slide_in(struct Obj *obj))

void child_slide_to_bg2(struct Obj *obj)
{
    struct Obj *child = obj->unkC;

    child->unk16 += 3;
    if ((s16)child->unk16 > *(vu16 *)&gUnk_0200001A)
    {
        child->unk16 = *(vu16 *)&gUnk_0200001A;
        obj->unk4 = scroll_text_step;
    }
}

void countdown_world_counter(struct Obj *obj)
{
    s16 *counter = (s16 *)&gWorld->counter314;

    if ((*counter -= 0x400) > 0)
    {
        status_slots_update(obj);
    }
    else
    {
        *counter = 0;
        status_slots_setup(obj);
        status_slots_update(obj);
    }
}

void bg_slide_offset_callback(struct Obj *obj)
{
    REG_BG2VOFS = *(vu16 *)&gUnk_0200001A + obj->unk14;
    REG_BG3VOFS = *(vu16 *)&gUnk_0200001E + obj->unk14;
    obj->unk4 = 0;
}

ASM_FUNC("asm/code_0810D6C0/resolve_contact_action.s", void resolve_contact_action(void))

void apply_partner_effect(struct PartnerObj *a, struct PartnerObj *b, s32 strength, s32 c, s32 x, s32 z, s32 d);
ASM_FUNC("asm/code_0810D6C0/partner_link_update.s", void partner_link_update(void))

ASM_FUNC("asm/code_0810D6C0/hero_random_offset.s", void hero_random_offset(void))

ASM_FUNC("asm/code_0810D6C0/apply_partner_effect.s", void apply_partner_effect(struct PartnerObj *a, struct PartnerObj *b, s32 strength, s32 c, s32 x, s32 z, s32 d))
