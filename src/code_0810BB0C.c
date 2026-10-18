#include "global.h"

struct Fig
{
    u8 filler0[0x7E];
    u8 unk7E;
    u8 filler7F[0xD8 - 0x7F];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    u8 filler0E4[0xEC - 0xE4];
    s16 unkEC;
    u8 filler0EE[0x11E - 0xEE];
    u16 unk11E;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 flags0B;
    u8 filler0C[0x80 - 0x0C];
    struct Fig *figs[2];
    u8 filler88[0x250 - 0x88];
    void *slot4Tiles;
    u8 filler254[0x27C - 0x254];
    struct Obj *marks[2];
    struct Obj *slotObjs[2];
    u8 filler28C[0x290 - 0x28C];
    struct Obj *timerObj;
    u8 filler294[0x2BF - 0x294];
    u8 flags2BF;
    u8 filler2C0[0x2F9 - 0x2C0];
    u8 stateByte;
};

struct Bits7E
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct Bits11
{
    u8 low : 2;
    u8 mode : 2;
    u8 high : 4;
};

struct Bits12
{
    u8 low : 4;
    u8 bit4 : 1;
    u8 high : 3;
};

struct Obj
{
    u8 filler0[4];
    void (*unk4)(struct Obj *);
    struct Child *unk8;
    struct Obj *unkC;
    u8 filler10[2];
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
};

struct Child
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[8];
    u16 *unk10;
    u8 filler14[0x7E - 0x14];
    u8 unk7E;
};

struct SlotChild
{
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u8 filler8[0x11 - 0x08];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 unk1F;
};

extern struct World *gWorld;
extern vu16 gUnk_0200001A;
extern u16 gUnk_0200001E;

void draw_panel_values(struct Obj *obj);
void clear_slot_entry(struct Obj *obj);
void panel_slide_step(struct Obj *obj);
void step_slot_object(struct Obj *obj);
void obj_release_node(void *node);
void sprite_entry_update(struct SlotChild *sub);
struct Obj *list_add_task(void *list, void *next);
struct Obj *raster_line_add(void *fn, u32 kind, void *arg);
struct Obj *task_create_list_4c(void *next, void *obj);
void release_slot4_tiles(void);
void set_tile_slot(void *slot, u32 idx, u32 flag, u32 value, u32 p5, u32 p6, u32 p7);
void draw_slot_frame(void *slot, u32 idx, u32 rect, u32 kind);
void draw_slot_text(void *slot, u32 idx, u32 x, u32 y, void *text, u32 color);
void bg_slide_out(void);
void bg_slide_timer(struct Obj *obj);
void bg_restore_while_busy(struct Obj *obj);
void scroll_text_step(struct Obj *obj);
void timer_window_setup(void);
void update_voice_message(void);
void retract_window_panel(void);
void release_tile_slot(void *obj, u8 idx);
void release_slot_object(u32 idx);
void *spawn_obj_by_id(u32 id, u32 variant, u32 attr, u32 mode);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void timer_object_state_db08(struct Obj *obj);
void battle_bg_scroll_callback(void);
extern u16 gUnk_02000048;

ASM_FUNC("asm/code_0810BB0C/draw_world_effect_rows.s", void draw_world_effect_rows(void))

ASM_FUNC("asm/code_0810BB0C/draw_obj_display.s", void draw_obj_display(void))

ASM_FUNC("asm/code_0810BB0C/slide_window_panel.s", void slide_window_panel(void))

ASM_FUNC("asm/code_0810BB0C/setup_status_panel.s", void setup_status_panel(void))

ASM_FUNC("asm/code_0810BB0C/create_status_objects.s", void create_status_objects(void))

ASM_FUNC("asm/code_0810BB0C/retract_window_panel.s", void retract_window_panel(void))

NONMATCH("asm/code_0810BB0C/spawn_slot_marker.s", struct Obj *spawn_slot_marker(u32 index))
{
    struct Fig *fig;
    struct Obj *marker;
    u32 h;

    fig = gWorld->figs[index];
    if (fig == 0)
    {
        return 0;
    }
    if (gWorld->marks[index] != 0)
    {
        return gWorld->marks[index];
    }
    marker = list_add_task((u8 *)gWorld + 0x48, panel_slide_step);
    marker->unk8 = (struct Child *)fig;
    if (gWorld->figs[1] == 0)
    {
        h = 0x20;
    }
    else
    {
        h = (fig->unkEC == -1) ? 0x3C : 4;
    }
    marker->unk12 = h;
    marker->unk14 = 0xA0;
    marker->unk16 = 0;
    marker->unk18 = 0xFFFF;
    gWorld->marks[index] = marker;
    return marker;
}
END_NONMATCH

ASM_FUNC("asm/code_0810BB0C/draw_panel_values.s", void draw_panel_values(struct Obj *obj))

ASM_FUNC("asm/code_0810BB0C/draw_counter_display.s", void draw_counter_display(void))

void panel_slide_step(struct Obj *obj)
{
    struct Child *sub;
    u16 h;

    if ((((u32)gWorld->modeByte << 30) >> 30) > 1)
    {
        draw_panel_values(obj);
        h = obj->unk14;
        if ((s16)obj->unk14 > 0x88)
        {
            obj->unk14 = h - 4;
            sub = obj->unk8;
            if (sub != 0 && ((struct Bits7E *)&sub->unk7E)->mode != 1)
            {
                sub->unk10[1] = h + 0x14;
            }
            if ((s16)obj->unk14 > 0x88)
            {
                return;
            }
        }
        obj->unk14 = 0x88;
        obj->unk4 = draw_panel_values;
    }
}

void panel_slide_end(struct Obj *obj)
{
    struct Obj **slot;
    struct Child *sub;
    u16 h;

    draw_panel_values(obj);
    h = obj->unk14;
    if ((s16)obj->unk14 <= 0x9F)
    {
        obj->unk14 = h + 4;
        sub = obj->unk8;
        if (sub != 0 && ((struct Bits7E *)&sub->unk7E)->mode != 1)
        {
            sub->unk10[1] = h + 0x1C;
        }
        if ((s16)obj->unk14 <= 0x9F)
        {
            return;
        }
    }
    slot = &gWorld->marks[0];
    if (obj->unk8 == (*slot)->unk8)
    {
        *slot = 0;
    }
    else
    {
        gWorld->marks[1] = 0;
    }
    obj->unk4 = 0;
}

ASM_FUNC("asm/code_0810BB0C/update_slot_display.s", void update_slot_display(void))

ASM_FUNC("asm/code_0810BB0C/clear_slot_entry.s", void clear_slot_entry(struct Obj *obj))

void step_slot_object(struct Obj *obj)
{
    struct SlotChild *sub;
    struct Obj *entry;
    s32 idx;
    s32 shrunk;

    if (gWorld->flags0B & 0x80)
    {
        idx = obj->unk16 & 1;
        entry = gWorld->slotObjs[idx];
        if (entry != 0)
        {
            obj_release_node(entry->unk8);
            gWorld->slotObjs[idx] = 0;
            entry->unk4 = 0;
        }
    }
    else
    {
        sub = (struct SlotChild *)obj->unk8;
        shrunk = sub->unk6 - 0x40;
        sub->unk6 = shrunk;
        sub->unk4 = shrunk;
        if ((s16)shrunk <= 0x100)
        {
            sub->unk6 = 0x100;
            sub->unk4 = 0x100;
            ((struct Bits12 *)&sub->unk12)->bit4 = 0;
            obj->unk4 = clear_slot_entry;
        }
        sprite_entry_update(sub);
    }
}

ASM_FUNC("asm/code_0810BB0C/pick_target_by_input.s", void pick_target_by_input(void))
