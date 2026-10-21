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

ASM_FUNC("asm/code_081272CC/obj_anim_step.s", void obj_anim_step(void))

ASM_FUNC("asm/code_081272CC/obj_build_sprite_list.s", void obj_build_sprite_list(void))

ASM_FUNC("asm/code_081272CC/draw_list_rows.s", u32 draw_list_rows(struct SaveScreen *obj, u32 oam))

ASM_FUNC("asm/code_081272CC/obj_redraw_box.s", void obj_redraw_box(struct SaveScreen *obj))

ASM_FUNC("asm/code_081272CC/update_entry_flags.s", void update_entry_flags(void))

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

ASM_FUNC("asm/code_081272CC/draw_text_line.s", void draw_text_line(void))
