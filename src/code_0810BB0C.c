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

struct Bits1F
{
    u8 low : 2;
    u8 mid : 2;
    u8 bits45 : 2;
    u8 top : 2;
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

struct PanelInfo
{
    u8 filler0[0x16];
    s16 unk16;
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
void retract_window_panel(struct Obj *obj);
void release_tile_slot(void *obj, u8 idx);
void release_slot_object(u32 idx);
void *spawn_obj_by_id(u32 id, u32 variant, u32 attr, u32 mode);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void timer_object_state_db08(struct Obj *obj);
void battle_bg_scroll_callback(void);
extern u16 gUnk_02000048;

static inline void *world_slot4(void)
{
    return gWorld->slot4Tiles;
}

ASM_FUNC("asm/code_0810BB0C/draw_world_effect_rows.s", void draw_world_effect_rows(void))

ASM_FUNC("asm/code_0810BB0C/draw_obj_display.s", void draw_obj_display(void))

void slide_window_panel(struct Obj *obj)
{
    s32 height;
    s32 bg2;
    s32 t;
    s32 y;
    s32 top;
    s32 bottom;
    s32 first;
    s32 second;
    u32 win;
    vu16 *shadow;
    vu32 *win0;

    height = obj->unk14 + 3;
    obj->unk14 = height;
    if ((s16)height > 0x20)
    {
        obj->unk14 = 0x18;
    }
    else if ((s16)height > 0x18)
    {
        obj->unk14 = 0x20;
        if (gWorld->timerObj != 0)
        {
            release_tile_slot(world_slot4(), 4);
            release_tile_slot(world_slot4(), 5);
            gWorld->timerObj->unk4 = timer_object_state_db08;
        }
        obj->unkC = (struct Obj *)battle_bg_scroll_callback;
    }
    bg2 = ((struct PanelInfo *)gWorld->timerObj->unkC)->unk16;
    REG_BG2VOFS = bg2 - ((t = obj->unk14) + 6);
    REG_BG3VOFS = gUnk_0200001E - obj->unk14;
    win0 = (vu32 *)REG_ADDR_WIN0V;
    shadow = &gUnk_0200001A;
    first = shadow[0];
    y = (s16)obj->unk14;
    top = y - 0x99;
    win = first - top;
    second = shadow[0];
    bottom = y - 0x8E;
    win |= (second - bottom) << 8;
    win |= ((shadow[0] - top) | ((shadow[0] - bottom) << 8)) << 16;
    *win0 = win;
}

void setup_status_panel(void)
{
    struct Obj *line;
    u32 zero;

    zero = 0;
    release_slot4_tiles();
    set_tile_slot(world_slot4(), 0, zero, 0xD, zero, 0x12, 3);
    draw_slot_frame(world_slot4(), 1, 0, 0);
    draw_slot_text(world_slot4(), 0, 7, 6, (void *)zero, 0x91);
    line = raster_line_add(bg_slide_out, 0, 0);
    line->unk14 = 0x18;
    raster_line_add(bg_slide_timer, 2, 0)->unk8 = (struct Child *)line;
    raster_line_add(bg_restore_while_busy, 0x18, 0)->unk8 = (struct Child *)line;
}

NONMATCH("asm/code_0810BB0C/create_status_objects.s", void create_status_objects(void))
{
    struct Obj *head;
    struct Obj *sub;
    u16 *reg;

    if (gWorld->timerObj == 0)
    {
        if (gWorld->timerObj == 0)
        {
            head = task_create_list_4c(scroll_text_step, 0);
            gWorld->timerObj = head;
            gWorld->flags0B |= 0x40;
            sub = raster_line_add(timer_window_setup, 0x86, update_voice_message);
            gWorld->timerObj->unkC = sub;
            sub->unk16 = gUnk_0200001A;
            reg = (u16 *)&gUnk_02000048;
            *reg = 0x3F3F;
            *(reg += 1) = 0x3F3B;
        }
        raster_line_add(retract_window_panel, 0x88, 0)->unk14 = 0x18;
    }
}
END_NONMATCH

void retract_window_panel(struct Obj *obj)
{
    u16 old;
    s32 bg2;
    s32 old3;
    s32 height;
    s32 y;
    s32 top;
    s32 bottom;
    s32 first;
    s32 second;
    u32 win;
    vu16 *shadow;
    vu32 *win0;

    old = obj->unk14;
    height = old - 3;
    obj->unk14 = height;
    if ((s16)height <= 0)
    {
        obj->unk4 = 0;
    }
    else
    {
        bg2 = ((struct PanelInfo *)gWorld->timerObj->unkC)->unk16;
        REG_BG2VOFS = bg2 - (old3 = old + 3);
        REG_BG3VOFS = gUnk_0200001E - obj->unk14;
        win0 = (vu32 *)REG_ADDR_WIN0V;
        shadow = &gUnk_0200001A;
        first = shadow[0];
        y = (s16)obj->unk14;
        top = y - 0x99;
        win = first - top;
        second = shadow[0];
        bottom = y - 0x8E;
        win |= (second - bottom) << 8;
        win |= ((shadow[0] - top) | ((shadow[0] - bottom) << 8)) << 16;
        *win0 = win;
    }
}
NONMATCH("asm/code_0810BB0C/spawn_slot_marker.s", struct Obj *spawn_slot_marker(u32 index))
{
    struct Fig *fig;
    struct Obj *marker;
    u16 h;

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

NONMATCH("asm/code_0810BB0C/update_slot_display.s", void update_slot_display(u32 index))
{
    struct Obj *obj;
    struct Obj *slot;
    struct Obj *created;
    struct World *world;
    struct Fig *fig;
    struct SlotChild *rec;
    s32 frameCount;
    s32 t;
    s32 stored;
    u32 slotOff;
    u8 *slotBase;
    u8 state;

    world = gWorld;
    slotOff = index << 2;
    if ((world->flags0B & 8) != 0)
    {
        return;
    }
    slotBase = (u8 *)world + 0x284;
    slot = *(struct Obj **)(slotBase + slotOff);
    if (slot != 0)
    {
        obj = slot;
    }
    else
    {
        created = list_add_task((u8 *)world + 0x48, step_slot_object);
        created->unk8 = (struct Child *)slot;
        created->unk16 = 0;
        slotBase = (u8 *)gWorld + 0x284;
        *(struct Obj **)(slotBase + slotOff) = created;
        obj = created;
    }
    if (obj == 0)
    {
        return;
    }
    if (gWorld->stateByte == 0)
    {
        release_slot_object(index);
        return;
    }
    fig = gWorld->figs[index];
    state = gWorld->stateByte;
    if (state == 2)
    {
        frameCount = (fig->unk11E & 0x2000) ? 0x1D : 0x19;
    }
    else
    {
        frameCount = (fig->unk11E & 0x1000) ? 0x1B : 0x17;
    }
    if (frameCount > 3)
    {
        state |= 0x20;
    }
    t = frameCount - 1;
    frameCount = t - fig->unkEC;
    stored = ((s32)((u32)obj->unk16 << 16) >> 17) & 0x1F;
    if (stored == state)
    {
        return;
    }
    if (obj->unk8 != 0)
    {
        rec = (struct SlotChild *)obj->unk8;
    }
    else
    {
        rec = (struct SlotChild *)spawn_obj_by_id(0x7001, 0, 2, 1);
        obj->unk8 = (struct Child *)rec;
    }
    obj_load_record(rec, frameCount, -1, 0, 0);
    rec->unk0 = fig->unkD8 / 0x100 - 0xE;
    rec->unk2 = (fig->unkDC - fig->unkE0) / 0x100 - 0x34;
    ((struct Bits1F *)&rec->unk1F)->top = 1;
    ((struct Bits1F *)&rec->unk1F)->bits45 = 1;
    ((struct Bits1F *)&rec->unk1F)->mid = 1;
    ((struct Bits1F *)&rec->unk1F)->low = 1;
    if ((fig->unk11E & 0x300) != 0)
    {
        ((struct Bits11 *)&rec->unk11)->mode = 1;
    }
    stored = ((s32)((u32)obj->unk16 << 16) >> 17) & 0xF;
    if (stored == gWorld->stateByte && (s16)rec->unk4 == 0x100)
    {
        sprite_entry_update((struct SlotChild *)rec);
        obj->unk4 = clear_slot_entry;
    }
    else
    {
        if (stored != gWorld->stateByte)
        {
            rec->unk6 = 0x400;
            rec->unk4 = 0x400;
        }
        ((struct Bits12 *)&rec->unk12)->bit4 = 1;
        obj->unk4 = step_slot_object;
    }
    obj->unk16 = (state << 1) | index;
}
END_NONMATCH

void clear_slot_entry(struct Obj *obj)
{
    struct SlotChild *sub;
    struct Fig *rec;
    struct Obj *entry;
    s16 idx;
    s32 flag2;
    s32 k;

    idx = obj->unk16 & 1;
    if ((gWorld->flags0B & 0x80) != 0)
    {
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
        if (gWorld->slotObjs[idx] == 0)
        {
            return;
        }
        rec = gWorld->figs[idx];
        sub = (struct SlotChild *)obj->unk8;
        if ((gWorld->flags2BF & 1) == 0 || (rec->unk11E & 0x300) != 0)
        {
            flag2 = 1;
        }
        else
        {
            flag2 = 0;
        }
        ((struct Bits11 *)&sub->unk11)->mode = flag2;
        if (((struct Bits7E *)&rec->unk7E)->mode == 1)
        {
            return;
        }
        k = idx;
        entry = gWorld->slotObjs[k];
        if (entry != 0)
        {
            obj_release_node(entry->unk8);
            gWorld->slotObjs[idx] = 0;
            entry->unk4 = 0;
        }
    }
}

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
