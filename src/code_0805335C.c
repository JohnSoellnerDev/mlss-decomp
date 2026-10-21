#include "global.h"

struct SysFlags888
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct TaskDescObj
{
    u8 filler0[0x18];
    const void *desc;
};

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

struct TextboxObj
{
    u8 state;
    u8 filler1[0x14 - 1];
    void *unk14;
    const void *unk18;
    u8 unk1C;
    u8 filler1D[0x20 - 0x1D];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    void *unk30;
    void *unk34;
    void *unk38;
    u8 unk3C;
    u8 filler3D[0x40 - 0x3D];
    void *unk40;
    u8 unk44;
    u8 unk45;
    u8 filler46[0x48 - 0x46];
    u8 unk48[0x10];
};

void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void heap_free_a(void *p);
void heap_free_b(void *p);
void run_pending_obj_callbacks(void);
void serial_link_disable(void);
void obj_list_pass_0a8c(void);
extern const u8 gTaskDescrBox[];
extern const u8 gDescChild[];
extern const u8 gTextboxTaskClass[];
extern const u8 gDescTextbox[];
extern const u8 gTaskDescrTextbox[];
extern u16 gUnk_02000050;
extern u8 gSys[];
struct SysKeys2A { u8 filler0[0x2A]; u16 keys; };
extern struct ObjLists gUnk_0203FFB8;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
void window_place(void *window, void *data, u32 x, u32 y, u32 w, u32 h, u32 flags);
extern u8 *const gBgRequestPtr;
extern const u8 gBoxStyleSizes[];
extern const u8 gTagSceneObj[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void scene_start_task(void *node, u32 kind, const void *tag, u32 arg);
void window_anim_step(void *window);
extern const u8 gLangBoxSizes[];
extern const u8 gText[];
extern u16 gUnk_02000008;
extern u16 gUnk_02000010;
extern u16 gUnk_02000054;
extern u16 gUnk_02000080;
void voice_setup_params(void *voice, void *block, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
void voice_set_data_ptr(void *voice, u32 a, u32 b, void *ptr);
s32 track_run(void *voice);
void frame_update(void);
void *heap_alloc(u32 size, u32 a, const void *tag, u32 c);
u32 read_varlen_plus1(const void *src);
void cpu_fast_set(const void *src, void *dst, u32 ctl);
void driver_tables_init(void *block, const void *params);
void textbox_place_style(struct TextboxObj *obj);
extern u8 iwram_overlay[];
extern const u8 gDriverParams[];
extern const u8 gTagWinMap[];
extern const u8 gTagDecomp[];
extern const u8 gTagDriver081e37c4[];
extern const u8 gTagVoice[];
extern const u8 gTagTextBuf[];
extern const u8 gTagBwsw[];
extern const u8 gSharedBg083a26d4[];
extern const u8 gPaletteRomA[];
extern const u8 gPaletteRomB[];
extern u32 (*gUnk_03000C84)();

struct VoiceTrack
{
    u8 filler0[8];
    u8 *data;
};

ASM_FUNC("asm/code_0805335C/textbox_setup_tiles.s", void textbox_setup_tiles(void))

ASM_FUNC("asm/code_0805335C/textbox_place.s", void textbox_place(void))

ASM_FUNC("asm/code_0805335C/textbox_emit_oam.s", void textbox_emit_oam(void))

ASM_FUNC("asm/code_0805335C/textbox_open_scene.s", void textbox_open_scene(void))

NONMATCH("asm/code_0805335C/textbox_free.s", void textbox_free(struct TextboxObj *obj, u32 kind))
{
    u8 *base;
    struct SysFlags888 *flags;
    u16 *word;

    obj->unk18 = gTaskDescrTextbox;
    if (obj->unk34 != NULL)
        task_remove(obj->unk34, 3);
    if (obj->unk38 != NULL)
        heap_free_a(obj->unk38);
    if (obj->unk20 != NULL)
        heap_free_a(obj->unk20);
    if (obj->unk28 != NULL)
        heap_free_a(obj->unk28);
    if (obj->unk2C != NULL)
        heap_free_a(obj->unk2C);
    heap_free_b(obj->unk30);
    run_pending_obj_callbacks();
    base = gSys;
    flags = (struct SysFlags888 *)(base + 0x888);
    flags->bit5 = 0;
    flags->bit6 = 0;
    flags->bit3 = 0;
    base[0x31] = 2;
    word = (u16 *)0x02000000;
    *word = 0;
    word[0x28] = 0;
    task_remove(obj, kind);
}
END_NONMATCH

ASM_FUNC("asm/code_0805335C/textbox_step.s", void textbox_step(void))

NONMATCH("asm/code_0805335C/textbox_create.s", struct TextboxObj *textbox_create(struct TextboxObj *obj, u32 kind))
{
    u32 zero;
    void *gfx;
    u8 *base;
    u32 langByte;
    u32 lang;
    u32 langTrack;
    s32 margin;
    u16 boxW;
    u16 *map;
    u16 *p;
    u16 *q;
    void *aligned;
    u16 tile;
    s32 x;
    s32 y;

    task_insert(obj, kind & 0xFF);
    obj->unk18 = gTextboxTaskClass;
    p = &gUnk_02000050;
    *p = 0xBF;
    p += 2;
    *p = 0x10;
    p -= 0x2A;
    *p = 0;
    frame_update();
    obj->unk1C = 0xF0;
    obj->unk34 = heap_alloc(0x800, 1, gTagWinMap, 0);
    gfx = heap_alloc(0x8000, 1, gTagDecomp, 0);
    gUnk_03000C84(gSharedBg083a26d4, gfx);
    cpu_fast_set(gfx, (void *)0x06000000, (read_varlen_plus1(gSharedBg083a26d4) << 9) >> 11);
    if (gfx != NULL)
        heap_free_a(gfx);
    if ((gSys[0x888] & 6) == 0)
    {
        cpu_fast_set(gPaletteRomA, &gUnk_02000080, 0x78);
    }
    else
    {
        cpu_fast_set(gPaletteRomB, &gUnk_02000080, 0x78);
    }
    gUnk_02000080 = 0x7FFF;
    base = gSys;
    *(u16 *)(base + 2) = 0xFFFF;
    obj->unk2C = heap_alloc((u32)0x0819A4F0 - (u32)iwram_overlay, 0, gTagDriver081e37c4, 1);
    driver_tables_init(obj->unk2C, gDriverParams);
    obj->unk30 = heap_alloc2(0x14, 0, gTagVoice, 1);
    obj->unk20 = heap_alloc(0x1001F, 1, gTagTextBuf, 1);
    obj->unk24 = (void *)(((u32)obj->unk20 + 0x1F) & 0xFFFFFFE0);
    obj->unk28 = heap_alloc(0x2000, 1, gTagBwsw, 1);
    zero = 0;
    cpu_fast_set(&zero, (void *)0x06004000, 0x01000008);
    langByte = base[0x8C0];
    lang = 0;
    if (langByte != 7)
        lang = langByte;
    langTrack = ((const u32 *)(gText + 0x110DD0))[lang];
    boxW = *(const u16 *)(gLangBoxSizes + (base[0x8C0] << 2));
    voice_setup_params(obj->unk30, obj->unk24, boxW, 0, 0, 0, 1, 2, 5, 0);
    voice_set_data_ptr(obj->unk30, 0, 0x48, (void *)(langTrack + 2));
    do
    {
        struct VoiceTrack *track = obj->unk30;
        u8 *data = track->data;

        if (data[0] == 0xFF && data[1] == 0x0B && data[2] == 1)
            track->data = data + 3;
    } while ((track_run(obj->unk30) << 16) != 0);
    map = obj->unk28;
    tile = 0xE001;
    y = 0;
    aligned = obj->unk24;
    margin = (0x1E - boxW) / 2;
    for (; y < 16; y++)
    {
        map += margin;
        if (boxW != 0)
        {
            for (x = 0; x < boxW; x++)
            {
                *map++ = tile++;
            }
        }
        map += margin + 2;
    }
    cpu_fast_set(aligned, (void *)0x06004020, 0x1000);
    cpu_fast_set(obj->unk28, (void *)0x06002000, 0x200);
    p = &gUnk_02000008;
    *p = 0x406;
    p++;
    *p = 0x1C02;
    q = &gUnk_02000010;
    *q = 0;
    q++;
    *q = 0;
    q++;
    *q = 0;
    q++;
    *q = 0;
    textbox_place_style(obj);
    p = &gUnk_02000050;
    *p = 0xBF;
    gUnk_02000054 = 0;
    p -= 0x28;
    *p = 0x3340;
    return obj;
}
END_NONMATCH

void textbox_update(struct TextboxObj *obj)
{
    switch (obj->state)
    {
    case 0:
    {
        s32 anim;
        s32 timer;

        anim = ((s8 *)&obj->unk38)[1];
        if (anim > 0)
        {
            window_anim_step(&obj->unk38);
            break;
        }
        timer = obj->unk1C - 1;
        obj->unk1C = timer;
        if ((timer << 24) != 0 && (((struct SysKeys2A *)gSys)->keys & 0xB) == 0)
            break;
        gBgRequestPtr[1] |= 0x20;
        ((u8 *)&obj->unk38)[0] |= 1;
        ((u8 *)&obj->unk38)[1] = 7;
        obj->state = 1;
        break;
    }
    case 1:
    {
        s32 anim;

        window_anim_step(&obj->unk38);
        anim = ((s8 *)&obj->unk38)[1];
        if (anim != 0)
            break;
        if (obj != NULL)
        {
            obj->unk18 = gTextboxTaskClass;
            if (obj->unk34 != NULL)
                heap_free_a(obj->unk34);
            if (obj->unk20 != NULL)
                heap_free_a(obj->unk20);
            if (obj->unk28 != NULL)
                heap_free_a(obj->unk28);
            if (obj->unk2C != NULL)
                heap_free_a(obj->unk2C);
            heap_free_b(obj->unk30);
            *(u16 *)0x02000000 = 0;
            *(u16 *)0x02000050 = 0;
            task_remove(obj, 3);
        }
        scene_start_task(heap_alloc2(0xE4, 0, gTagSceneObj, 0), 8, gTagSceneObj, 2);
        break;
    }
    }
}

ASM_FUNC("asm/code_0805335C/textbox_place_style.s", void textbox_place_style(struct TextboxObj *obj))

void textbox_task_free(struct TextboxObj *obj, u32 kind)
{
    obj->unk18 = gTextboxTaskClass;
    if (obj->unk34 != NULL)
        heap_free_a(obj->unk34);
    if (obj->unk20 != NULL)
        heap_free_a(obj->unk20);
    if (obj->unk28 != NULL)
        heap_free_a(obj->unk28);
    if (obj->unk2C != NULL)
        heap_free_a(obj->unk2C);
    heap_free_b(obj->unk30);
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000050 = 0;
    task_remove(obj, kind);
}

void oam_emit_objects(void)
{
    u8 *base;
    u32 *oam_slot;
    u32 old;

    base = gSys;
    oam_slot = (u32 *)(base + 0x880);
    old = *oam_slot;
    obj_list_pass_0a8c();
    gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
    old = gUnk_03000D4C(gUnk_0203FFB8.root, old);
    *oam_slot = old;
}

void textbox_place_rect(struct TextboxObj *obj, s32 w, s32 h)
{
    u32 x;
    u32 y;
    void *window;
    void *data;

    x = 0xF - w / 2;
    y = 0xA - h / 2;
    window = &obj->unk40;
    data = obj->unk38;
    window_place(window, data, (u8)x, (u8)y, (u8)w, (u8)h, 0xE000);
    *(gBgRequestPtr + 0x4A) |= 0x10;
}

void textbox_place_anchor(struct TextboxObj *obj, s32 w, s32 h)
{
    u32 x;
    u32 y;

    x = (u8)(0xF - w / 2);
    y = (u8)(0xA - h / 2);
    window_place(&obj->unk48, obj->unk40, x, y, (u8)w, (u8)h, 0xC800);
    obj->unk44 = (u8)((x << 3) + (w << 2) - 0x10);
    obj->unk45 = (u8)(((y + h) << 3) - 0x16);
}

void textbox_free_disable_link(struct TextboxObj *obj, u32 kind)
{
    obj->unk18 = gDescTextbox;
    run_pending_obj_callbacks();
    if (obj->unk20 != NULL)
        task_remove(obj->unk20, 3);
    if (obj->unk40 != NULL)
        heap_free_a(obj->unk40);
    if (obj->unk24 != NULL)
        heap_free_a(obj->unk24);
    if (obj->unk30 != NULL)
        heap_free_a(obj->unk30);
    if (obj->unk34 != NULL)
        heap_free_a(obj->unk34);
    heap_free_b(obj->unk38);
    if ((obj->unk3C & 1) == 0)
        serial_link_disable();
    gSys[0x31] = 2;
    *(u16 *)0x02000000 = 0;
    gUnk_02000050 = 0x10;
    task_remove(obj, kind);
}

struct TaskDescObj *task_descr_box_init(struct TaskDescObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->desc = gTaskDescrBox;
    return obj;
}

struct TaskDescObj *desc_child_init(struct TaskDescObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->desc = gDescChild;
    return obj;
}

ASM_FUNC("asm/code_0805335C/overlay_init.s", void overlay_init(void))
