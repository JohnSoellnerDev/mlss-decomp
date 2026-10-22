#include "global.h"
#include "ram_routines.h"

struct TaskObj
{
    u8 filler0[0x18];
    const void *classPtr18;
};

struct SaveScreen
{
    u8 unk0;
    u8 filler1[0x13];
    void *unk14;
    u8 filler18[0x48 - 0x18];
    void *unk48;
    u8 filler4C[0xE4 - 0x4C];
    u16 unkE4;
    u16 unkE6;
    u8 filler0E8[0xEE - 0xE8];
    u8 unkEE;
    u8 unkEF;
    u8 filler0F0[0xF1 - 0xF0];
    u8 unkF1;
    u8 filler0F2[0xF3 - 0xF2];
    u8 unkF3;
    u8 filler0F4[0xFA - 0xF4];
    s8 unkFA;
    u8 filler0FB[0x154 - 0xFB];
    u8 unk154[3];
    u8 unk157;
    u8 filler158[0x15C - 0x158];
    void *child15C;
};

struct ObjList
{
    u8 filler0[0x3C];
    void *unk3C;
};

struct SaveEntry
{
    u8 filler0[0x12];
    u16 unk12;
    u8 filler14[0x17 - 0x14];
    u8 unk17;
    u8 filler18[0x1C - 0x18];
};

struct SysPal
{
    u8 filler0[0x880];
    u32 unk880;
    u32 unk884;
};

extern u8 gSys[];
extern u32 gUnk_03000FFC;
extern const u8 gSaveScreenChildClass[];
extern u8 gUnk_0203FFB8[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;

void task_insert(void *node, u8 kind, u32 data);
void save_slot_info(void *out, u8 slot);
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
u32 sub_obj_running(void *menu);
void set_slot_fields(u32 idx, u8 level, u8 period);
void set_slot_target(u32 idx, u32 value, u32 mode);
u32 draw_two_values(void *oam, u8 a, u8 b, u32 flag);
void update_obj_list(void);
void obj_list_pass_0a8c(void);
void obj_redraw_box(struct SaveScreen *obj);
u32 draw_list_rows(struct SaveScreen *obj, u32 oam);
void cpu_set(const void *src, void *dst, u32 ctl);
u32 table_085213b8(u32 index);
void psg_wave_setup(u8 *base, u32 width, u32 x, u32 y, u32 glyph, s32 ch, u32 pal, u32 zero);

ASM_FUNC("asm/code_081272CC/obj_anim_step.s", void obj_anim_step(void))

ASM_FUNC("asm/code_081272CC/obj_build_sprite_list.s", void obj_build_sprite_list(void))

ASM_FUNC("asm/code_081272CC/draw_list_rows.s", u32 draw_list_rows(struct SaveScreen *obj, u32 oam))

ASM_FUNC("asm/code_081272CC/obj_redraw_box.s", void obj_redraw_box(struct SaveScreen *obj))

NONMATCH("asm/code_081272CC/update_entry_flags.s", void update_entry_flags(struct SaveScreen *obj))
{
    u8 k;
    u16 src;
    struct SaveEntry *entry = (struct SaveEntry *)((u8 *)obj + 0x100);
    u8 *count = &obj->unk157;
    u32 listOff = 0;
    s32 bit = 1;
    *count = 0;
    obj->unkF3 = 0;
    for (k = 0; k <= 2; k++)
    {
        s32 visible = (obj->unkF1 >> k) & bit;
        if (visible == 0)
        {
            obj->unk154[*count] = k;
            (*count)++;
            src = visible;
            cpu_set(&src, entry, 0x0100000E);
            entry->unk17 = 0;
            if (((u8 *)obj->unk48 + listOff)[0x10] == 0xFF)
                entry->unk12 = 0xFFFF;
        }
        else
        {
            save_slot_info(entry, k);
            if (entry->unk12 == 0x113)
            {
                u8 *f3 = &obj->unkF3;
                *f3 |= bit << k;
            }
        }
        entry++;
        listOff += 0x6F8;
    }
}
END_NONMATCH

void update_screen(struct SaveScreen *obj)
{
    u32 sysBase = (u32)gSys;
    u32 *oamPtr = (u32 *)(sysBase + 0x880);
    u32 oam = *oamPtr;
    struct SaveScreen *sub = (struct SaveScreen *)obj->unk14;
    if (sub->unk0 > 3)
    {
        u8 flags;
        s8 sel;
        if (sub->unkEE != 0)
            obj_redraw_box(sub);
        flags = sub->unkF1;
        sel = sub->unkFA;
        if ((flags >> sel) & 1)
        {
            u8 *entry = (u8 *)sub + sel * 0x1C;
            u8 a1 = entry[0x114];
            u8 a2 = entry[0x115];
            u8 *f3 = &sub->unkF3;
            oam = draw_two_values((void *)oam, a1, a2, (1 << sel) & *f3);
        }
        update_obj_list();
        obj_list_pass_0a8c();
        ((void (*)(void *, void *))gUnk_03000D48)(((struct ObjList *)gUnk_0203FFB8)->unk3C,
            (u8 *)((struct ObjList *)gUnk_0203FFB8)->unk3C + 0xA14);
        oam = ((u32 (*)(void *, u32))gUnk_03000D4C)(((struct ObjList *)gUnk_0203FFB8)->unk3C, oam);
        oam = draw_list_rows(sub, oam);
        *oamPtr = oam;
    }
}

void *create_child_task(void *task, u8 kind, u32 data)
{
    task_insert(task, kind, data);
    ((struct TaskObj *)task)->classPtr18 = gSaveScreenChildClass;
    return task;
}

u32 child_busy(struct SaveScreen *obj)
{
    u32 result;
    u8 running = sub_obj_running(obj->child15C);
    if (running != 0)
        result = 1;
    else
    {
        obj->unkEF = running;
        result = 0;
    }
    return result;
}

void start_child_anim(struct SaveScreen *obj, u16 value)
{
    sub_08115F54(obj->child15C, 4, value, 0x10, 0xFFFF, 0xFFFF, 0, 0);
    set_slot_fields(0, 0, 0x10);
    obj->unkEF = 1;
}

void reset_child_anim(struct SaveScreen *obj)
{
    ((struct SysPal *)gSys)->unk884 = -1;
    sub_08115F54(obj->child15C, 0, obj->unkE4, 0x10, 0xFFFF, 0xFFFF, 0, 0);
    set_slot_target(0, 0x1E, 0);
    set_slot_fields(0, 0xFF, 0x10);
    *(u16 *)0x02000000 = obj->unkE6;
    obj->unkEF = 1;
}

u32 draw_slot_two_values(struct SaveScreen *obj, void *oam)
{
    s8 idx = obj->unkFA;
    u8 *entry = (u8 *)obj + idx * 0x1C;
    u8 a1 = entry[0x114];
    u8 a2 = entry[0x115];
    u8 *flags = &obj->unkF3;
    return draw_two_values(oam, a1, a2, (1 << idx) & *flags);
}

NONMATCH("asm/code_081272CC/save_slot_info.s", void save_slot_info(void *out, u8 slot))
{
    u8 *base = (u8 *)gUnk_03000FFC;
    u32 off = slot * 0x6F8;
    u8 *at = base + off;
    u32 t;
    u8 *pt;
    u8 *fr;
    u8 *s;
    u32 w;
    u32 status;

    ((u8 *)out)[0x14] = at[0x60];
    ((u8 *)out)[0x15] = at[0x9C];
    *(long long *)out = *(long long *)(at + 0xA8);
    *(long long *)((u8 *)out + 8) = *(long long *)(at + 0xB0);
    *(u16 *)((u8 *)out + 0x10) = *(u16 *)(at + 0xB8);
    t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(*(u32 *)(fr = (pt = base + 0x700) + off), 216000);
    if (t > 0x63)
    {
        ((u8 *)out)[0x17] = 0x3B;
        ((u8 *)out)[0x16] = 0x63;
    }
    else
    {
        ((u8 *)out)[0x16] = t;
        t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(*(u32 *)fr, 0xE10);
        ((u8 *)out)[0x17] = MATH_ROUTINE(u32 (*)(u32, u32), iwram_math_08199424)(t, 0x3C);
    }
    s = base + slot * 0x6F8;
    w = ((((u32)s[0x14] << 28) >> 31) * 12) + slot * 0x6F8;
    *(u16 *)((u8 *)out + 0x12) = *(u16 *)(base + w + 0x18);
    status = ((s32)(s[0x14] << 26) < 0 || (s32)(s[0x16] << 29) < 0) ? 2 : ((u32)s[0x14] << 28) >> 31;
    ((u8 *)out)[0x18] = status;
}
END_NONMATCH

NONMATCH("asm/code_081272CC/draw_text_line.s", void draw_text_line(u8 *dst, u8 width, u16 x, u16 y, u8 *text, u8 palette))
{
    u32 *fonts;
    u32 *font0;
    s32 code;
    u32 font;
    u32 *slot;
    u32 w;

    fonts = (u32 *)table_085213b8(1);
    font0 = (u32 *)fonts[0];
    y = ((font0[0] & 0xF) << 2) + y;
    y++;
    palette &= 0xF;
    while (*text != 0)
    {
        if ((s32)(x + 8) > (s32)(width << 3))
            break;
        do
        {
            code = *text;
            text++;
        } while (code == 0xFF);
        font = 0xFF - code;
        if (font == 0xFF)
            break;
        if ((s32)font > 1)
            font = 0;
        else
        {
            if (fonts[font] == 0)
                continue;
            code = *text;
            text++;
        }
        slot = &fonts[font];
        psg_wave_setup(dst, width, x, y, *slot, code, palette, 0);
        w = *(u32 *)((u8 *)*slot + ((code >> 3) << 2) + 4);
        w = w >> ((code & 7) << 2);
        x = (x + 1 + (w & 0xF)) & 0xFFFF;
    }
}
END_NONMATCH
