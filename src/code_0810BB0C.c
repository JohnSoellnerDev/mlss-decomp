#include "global.h"
#include "ram_routines.h"

struct FigBody
{
    u8 filler0[4];
    s8 ofsX;
    s8 ofsY;
};

struct Fig
{
    u8 filler0[0x28];
    struct Fig *part;
    u8 filler2C[0x34 - 0x2C];
    struct Fig *target;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 filler48[0x7E - 0x48];
    u8 unk7E;
    u8 filler7F[0xD8 - 0x7F];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    struct FigBody *body;
    u8 filler0E8[0xEC - 0xE8];
    s16 unkEC;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    s16 unkF8;
    s16 unkFA;
    s16 unkFC;
    s16 unkFE;
    u8 filler100[0x110 - 0x100];
    u8 unk110;
    u8 filler111[0x11E - 0x111];
    u16 unk11E;
    u32 unk120;
    u8 filler124[0x12C - 0x124];
    u8 unk12C;
};

struct Unit24
{
    u8 filler0[0x0C];
    u32 *names;
    u8 unk10;
    u8 unk11;
    u8 filler12[0x18 - 0x12];
};

struct SysKeys
{
    u8 filler0[0x42];
    u16 repeat;
    u8 filler44[0x8C0 - 0x44];
    u8 lang;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 flags0B;
    u8 filler0C[0x58 - 0x0C];
    struct Fig *actors[8];
    u8 filler78[0x7C - 0x78];
    struct Fig *active;
    struct Fig *figs[2];
    u8 filler88[0x1B8 - 0x88];
    struct Unit24 *units;
    struct Fig *defaultTarget;
    u8 filler1C0[0x1F0 - 0x1C0];
    void *tileBank;
    u8 filler1F4[0x250 - 0x1F4];
    void *slot4Tiles;
    u8 filler254[0x27C - 0x254];
    struct Obj *marks[2];
    struct Obj *slotObjs[2];
    u8 filler28C[0x290 - 0x28C];
    struct Obj *timerObj;
    u8 filler294[0x2BF - 0x294];
    u8 flags2BF;
    u8 filler2C0[0x2C2 - 0x2C0];
    s8 scroll[3];
    u8 filler2C5[0x2F9 - 0x2C5];
    u8 stateByte;
    u8 filler2FA[0x318 - 0x2FA];
    u32 tierBits;
    u8 filler31C[0x33F - 0x31C];
    u8 count0;
    u8 count1;
    u8 count2;
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
extern struct SysKeys gSys;
struct Fig *candidate_for_slot(struct Fig *actor);
void draw_text(void *bank, u32 a, u32 b, u32 y, u32 text, u32 pal);
void vram_slots_draw_number(void *bank, u32 a, u32 b, u32 y, u32 value, u32 pal, u32 p7, u32 p8);
u32 vcount_store(u32 *p);
void copy_gauge_tiles(void *obj);
void fill_slot(void *slot, u32 idx, u32 v);
void draw_obj_display(void *obj);
extern u8 gItemTable[];
extern u32 *gTextLangTable[];
extern u8 *gParty;
extern u8 gSlotTableX[];
extern u8 gSlotTableZ[];
extern u8 gGaugeTableC[];
extern u8 gGaugeTableD[];

struct EffRow
{
    u8 filler0[4];
    void (*unk4)(void *);
    u8 *items;
    u32 flags;
    s16 row;
    u8 filler12[4];
    u16 count;
};
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

NONMATCH("asm/code_0810BB0C/draw_world_effect_rows.s", void draw_world_effect_rows(struct EffRow *obj))
{
    struct World *world;
    struct Fig *active;
    struct Fig *fig;
    struct Unit24 *rec;
    u8 *items;
    void *bank;
    s32 kind;
    s32 row;
    s32 count;
    s32 scroll;
    s32 idx;
    s32 timeUp;
    s32 units;
    s32 extent;
    u32 item;
    u32 frame;
    u32 y;
    u32 palette;
    u32 value;
    u32 vslot;
    u32 text;
    u32 slot;
    u32 lang;
    u32 marked;
    u32 state;
    u32 heroValue;
    u32 tier;

    kind = obj->flags & 0xF;
    world = gWorld;
    bank = world->tileBank;
    row = obj->row;
    items = obj->items;

    if (kind == 1)
    {
        count = world->count1;
        obj->count = count;
    }
    else if (kind > 1)
    {
        if (kind == 2)
        {
            count = world->count2;
            obj->count = count;
        }
    }
    else if (kind == 0)
    {
        count = world->count0;
        obj->count = count;
    }

    if (row == 0)
    {
        MEM_ROUTINE(void (*)(u32, void *, u32), iwram_fill)(0, bank, 0x10E0);
    }
    scroll = (u16)world->scroll[kind];

    for (;;)
    {
        idx = (s16)scroll + row;
        if (idx >= count)
        {
            idx -= count;
        }
        item = items[idx];

        if (kind == 1)
        {
            rec = &world->units[item];
            if (((world->tierBits >> (item << 1)) & 2) != 0)
            {
                value = rec->unk11;
            }
            else
            {
                value = rec->unk10;
            }
            frame = value;
            active = world->active;
            palette = 1;
            if ((s32)value > (s16)active->unkFC)
            {
                palette = 3;
            }
            else
            {
                tier = active->unk11E >> 12;
                if ((s32)item <= 2)
                {
                    if ((tier & 1) != 0)
                    {
                        palette = 3;
                    }
                }
                else
                {
                    if ((tier & ((s32)item > 5 ? 4 : 2)) != 0)
                    {
                        palette = 3;
                    }
                }
            }
            y = (u16)(row * 0xB + 1);
            lang = gSys.lang;
            text = *(u32 *)((u32 *)rec->names)[lang];
            draw_text(bank, 0xC, 0, y, text, palette);
            vram_slots_draw_number(bank, 0xC, 0x60, y, value, palette, 1, 0);
        }
        else if (kind == 0)
        {
            active = world->active;
            palette = 1;
            if (row == 0)
            {
                if ((active->unk11E & 0x1000) != 0)
                {
                    palette = 3;
                }
            }
            else if (row == 1)
            {
                if ((active->unk11E & 0x2000) != 0)
                {
                    palette = 3;
                }
            }
            else if (row == 2)
            {
                if ((active->unk11E & 0x4000) != 0)
                {
                    palette = 3;
                }
            }
            y = (u16)(row * 0xB + 1);
            lang = gSys.lang;
            text = (u32)gTextLangTable[lang][item];
            draw_text(bank, 9, 0, y, text, palette);
        }
        else if (kind == 2)
        {
            heroValue = gParty[0x8A + item];
            palette = 3;
            if (heroValue != 0)
            {
                palette = 1;
            }
            frame = item << 4;
            marked = 0;
            for (slot = 0; slot < 2; slot++)
            {
                fig = world->figs[slot];
                if (fig == 0)
                {
                    continue;
                }
                state = (u32)gItemTable[frame] - 1;
                if (state > 0xD)
                {
                    continue;
                }
                switch (state)
                {
                case 0:
                    if ((fig->unk7E & 6) == 2 && fig->unkF6 < fig->unkF8)
                    {
                        marked = 1;
                    }
                    break;
                case 1:
                    if ((fig->unk7E & 6) == 2)
                    {
                        if (fig->unk12C == 0x13)
                        {
                            if ((fig->unk11E & 0x800) == 0)
                            {
                                marked = 1;
                            }
                        }
                        else if (fig->unk12C == 0x14)
                        {
                            if ((fig->unk11E & 0x400) == 0)
                            {
                                marked = 1;
                            }
                        }
                        else if ((fig->unk11E & 0xC00) != 0)
                        {
                            if ((fig->unk120 & 0x38000) != 0)
                            {
                                marked = 1;
                            }
                        }
                        if ((fig->unk11E & 0xF1AB) != 0)
                        {
                            marked = 1;
                        }
                    }
                    break;
                case 2:
                    if ((fig->unk7E & 6) == 6)
                    {
                        marked = 1;
                    }
                    break;
                case 10:
                    if ((fig->unk7E & 6) == 2 && fig->unkFC < fig->unkFE)
                    {
                        marked = 1;
                    }
                    break;
                case 11:
                    if ((fig->unk7E & 6) == 2 && (fig->unkF6 < fig->unkF8 || fig->unkFC < fig->unkFE))
                    {
                        marked = 1;
                    }
                    break;
                case 12:
                case 13:
                    marked = 1;
                    break;
                default:
                    break;
                }
            }
            if (!marked)
            {
                palette = 3;
            }
            y = (u16)(row * 0xB + 1);
            lang = gSys.lang;
            text = *(u32 *)(*(u32 *)(gItemTable + frame + 0xC) + (lang << 2));
            text = *(u32 *)text;
            draw_text(bank, 0xF, 8, y, text, palette);
            vram_slots_draw_number(bank, 0xF, 0x78, y, heroValue, palette, 1, 0);
        }
        row++;
        if (count <= row || row > 4)
        {
            timeUp = 0;
            break;
        }
        if ((vcount_store(&vslot) & 0xFF) > 0x46)
        {
            timeUp = 1;
            break;
        }
    }

    if (timeUp)
    {
        obj->row = row;
    }
    else if ((s32)obj->flags < 0)
    {
        obj->flags &= 0x7FFFFFFF;
        obj->row = 0;
        units = 0;
        if (kind == 1)
        {
            units = (world->count1 * 0xB + 8) >> 3;
        }
        else if (kind == 0)
        {
            units = (world->count0 * 0xB + 8) >> 3;
        }
        else if (kind == 2)
        {
            units = (world->count2 * 0xB + 8) >> 3;
        }
        extent = 9;
        if (!((s32)(gGaugeTableD[kind] - 2) < units))
        {
            extent = units + 2;
        }
        set_tile_slot(world->slot4Tiles, 2, 0, gSlotTableX[kind] + 1, gSlotTableZ[kind] + 1, gGaugeTableC[kind], extent);
        draw_slot_frame(world->slot4Tiles, 3, 2, 2);
        fill_slot(world->slot4Tiles, 2, 0x90);
        obj->unk4 = copy_gauge_tiles;
    }
    else
    {
        copy_gauge_tiles(obj);
    }
    draw_obj_display(obj);
}
END_NONMATCH

ASM_FUNC("asm/code_0810BB0C/draw_obj_display.s", void draw_obj_display(void *obj))

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

static inline __typeof__(gWorld->timerObj) get_world_timerObj(void)
{
    return gWorld->timerObj;
}

NONMATCH("asm/code_0810BB0C/create_status_objects.s", void create_status_objects(void))
{
    struct Obj *head;
    struct Obj *sub;
    u16 *reg;
    s32 fl;

    if (get_world_timerObj() == 0)
    {
        if (gWorld->timerObj == 0)
        {
            head = task_create_list_4c(scroll_text_step, 0);
            gWorld->timerObj = head;
            fl = gWorld->flags0B;
            fl |= 0x40;
            gWorld->flags0B = fl;
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
struct Obj *spawn_slot_marker(u32 index)
{
    struct Fig *fig;
    struct Obj *marker;

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
        marker->unk12 = 0x20;
    }
    else
    {
        if (fig->unkEC == -1)
        {
            marker->unk12 = 0x3C;
        }
        else
        {
            marker->unk12 = 4;
        }
    }
    marker->unk14 = 0xA0;
    marker->unk16 = 0;
    marker->unk18 = 0xFFFF;
    gWorld->marks[index] = marker;
    return marker;
}

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

NONMATCH("asm/code_0810BB0C/pick_target_by_input.s", u32 pick_target_by_input(struct Fig *actor, s32 mode))
{
    struct Fig *target;
    struct Fig *own;
    struct Fig *obj;
    struct Fig *bestAhead;
    struct Fig *bestBehind;
    s32 bestBehindQ;
    s32 bestScore;
    s32 keys;
    s32 tx;
    s32 ty;
    s32 k;
    s32 p;
    s32 q;
    s32 st;
    s32 sq;
    s32 sc;
    s32 root;

    target = actor->target;

    if (mode == 3)
    {
        if (target == 0)
        {
            if (gSys.repeat & 0x80)
            {
                target = (struct Fig *)((u8 *)candidate_for_slot(actor) + 8);
            }
        }
        else if (gSys.repeat & 0x40)
        {
            target = 0;
        }
    }
    else if (mode == 1)
    {
        own = (struct Fig *)((u8 *)actor + 8);
        if (target == own && (gSys.repeat & 0x40))
        {
            target = 0;
        }
        else if (target == 0)
        {
            if (gSys.repeat & 0x80)
            {
                target = own;
            }
        }
        else if (gSys.repeat & 0xF0)
        {
            if (gWorld->figs[1] != 0 && target == (struct Fig *)((u8 *)gWorld->figs[0] + 8))
            {
                if ((gWorld->figs[1]->unk7E & 6) == 2)
                {
                    target = (struct Fig *)((u8 *)gWorld->figs[1] + 8);
                }
            }
            else if ((gWorld->figs[0]->unk7E & 6) == 2)
            {
                target = (struct Fig *)((u8 *)gWorld->figs[0] + 8);
            }
        }
    }
    else if (mode == 0x81)
    {
        if (gSys.repeat & 0x40)
        {
            target = 0;
        }
        else if (gSys.repeat & 0x80)
        {
            target = (struct Fig *)((u8 *)actor + 8);
        }
    }
    else if (target == 0)
    {
        if (gSys.repeat & 0x10)
        {
            target = gWorld->defaultTarget;
        }
    }
    else
    {
        tx = target->unk38 / 0x100 + target->part->body->ofsX;
        ty = target->unk3C / 0x100 + target->part->body->ofsY;

        keys = gSys.repeat;
        bestAhead = 0;
        bestBehind = 0;
        bestBehindQ = 0;
        bestScore = 0x7FFFFFFF;

        if (keys & 0x40)
        {
            for (k = 0; k < 8; k++)
            {
                obj = gWorld->actors[k];
                st = obj->unk7E & 6;
                if (st == 6)
                {
                    continue;
                }
                p = ty - obj->unk44 / 0x100 - obj->body->ofsY;
                q = obj->unk40 / 0x100 + obj->body->ofsX - tx;
                if ((obj->unk110 & 0x20) == 0 && st == 4)
                {
                    sq = p * p + q * q;
                    if (p > 0)
                    {
                        sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sq, p);
                        if (bestScore > sc)
                        {
                            bestScore = sc;
                            bestAhead = obj;
                        }
                    }
                    else
                    {
                        root = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(sq);
                        sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(p * p, root);
                        if (sc > bestBehindQ)
                        {
                            bestBehindQ = sc;
                            bestBehind = obj;
                        }
                    }
                }
            }
        }
        else if (keys & 0x80)
        {
            for (k = 0; k < 8; k++)
            {
                obj = gWorld->actors[k];
                st = obj->unk7E & 6;
                if (st == 6)
                {
                    continue;
                }
                p = obj->unk44 / 0x100 + obj->body->ofsY - ty;
                q = obj->unk40 / 0x100 + obj->body->ofsX - tx;
                if ((obj->unk110 & 0x20) == 0 && st == 4)
                {
                    sq = p * p + q * q;
                    if (p > 0)
                    {
                        sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sq, p);
                        if (bestScore > sc)
                        {
                            bestScore = sc;
                            bestAhead = obj;
                        }
                    }
                    else
                    {
                        root = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(sq);
                        sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(p * p, root);
                        if (sc > bestBehindQ)
                        {
                            bestBehindQ = sc;
                            bestBehind = obj;
                        }
                    }
                }
            }
        }
        else if (keys & 0x20)
        {
            if (target == gWorld->defaultTarget)
            {
                target = 0;
            }
            else
            {
                for (k = 0; k < 8; k++)
                {
                    obj = gWorld->actors[k];
                    st = obj->unk7E & 6;
                    if (st == 6)
                    {
                        continue;
                    }
                    p = tx - obj->unk40 / 0x100 - obj->body->ofsX;
                    q = obj->unk44 / 0x100 + obj->body->ofsY - ty;
                    if ((obj->unk110 & 0x20) == 0 && st == 4)
                    {
                        sq = p * p + q * q;
                        if (p > 0)
                        {
                            sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sq, p);
                            if (bestScore > sc)
                            {
                                bestScore = sc;
                                bestAhead = obj;
                            }
                        }
                        else
                        {
                            root = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(sq);
                            sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(p * p, root);
                            if (sc > bestBehindQ)
                            {
                                bestBehindQ = sc;
                                bestBehind = obj;
                            }
                        }
                    }
                }
            }
        }
        else if (keys & 0x10)
        {
            for (k = 0; k < 8; k++)
            {
                obj = gWorld->actors[k];
                st = obj->unk7E & 6;
                if (st == 6)
                {
                    continue;
                }
                p = obj->unk40 / 0x100 + obj->body->ofsX - tx;
                q = obj->unk44 / 0x100 + obj->body->ofsY - ty;
                if ((obj->unk110 & 0x20) == 0 && st == 4)
                {
                    sq = p * p + q * q;
                    if (p > 0)
                    {
                        sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(sq, p);
                        if (bestScore > sc)
                        {
                            bestScore = sc;
                            bestAhead = obj;
                        }
                    }
                    else
                    {
                        root = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(sq);
                        sc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(p * p, root);
                        if (sc > bestBehindQ)
                        {
                            bestBehindQ = sc;
                            bestBehind = obj;
                        }
                    }
                }
            }
        }

        if (bestAhead != 0)
        {
            target = (struct Fig *)((u8 *)bestAhead + 8);
        }
        else if (bestBehind != 0)
        {
            target = (struct Fig *)((u8 *)bestBehind + 8);
        }
    }

    actor->target = target;
    return 1;
}
END_NONMATCH
