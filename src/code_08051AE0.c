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

ASM_FUNC("asm/code_08051AE0/window_place.s", void window_place(void))

ASM_FUNC("asm/code_08051AE0/window_anim_step.s", void window_anim_step(void))

ASM_FUNC("asm/code_08051AE0/textbox_open.s", void textbox_open(void))

ASM_FUNC("asm/code_08051AE0/textbox_task_step.s", void textbox_task_step(void))

ASM_FUNC("asm/code_08051AE0/apply_display_flags.s", void apply_display_flags(void))

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

ASM_FUNC("asm/code_08051AE0/textbox_draw_rows.s", void textbox_draw_rows(void))
