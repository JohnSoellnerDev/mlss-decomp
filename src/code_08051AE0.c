#include "global.h"

void task_insert(void *node, u8 kind, u32 data);
void task_remove(void *node, u32 kind);
void run_pending_obj_callbacks(void);
void obj_release_parts(void *parts, u32 mode);
void menu_obj_free(void *obj, u32 mode);
void clamp_camera_to_map(u8 *obj, s16 x, s16 y);
extern u8 gWindowStateClass[];
extern u8 gWindowSubBlockClass[];

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
    u16 unk28;
    u16 unk2A;
    u8 filler2C[0x10];
    u8 *frame3C;
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

ASM_FUNC("asm/code_08051AE0/window_place.s", void window_place(void))

ASM_FUNC("asm/code_08051AE0/window_anim_step.s", void window_anim_step(void))

ASM_FUNC("asm/code_08051AE0/textbox_open.s", void textbox_open(void))

ASM_FUNC("asm/code_08051AE0/textbox_task_step.s", void textbox_task_step(void))

ASM_FUNC("asm/code_08051AE0/apply_display_flags.s", void apply_display_flags(void))

ASM_FUNC("asm/code_08051AE0/load_tiles_and_voice.s", void load_tiles_and_voice(void))

ASM_FUNC("asm/code_08051AE0/textbox_draw_rows.s", void textbox_draw_rows(void))
