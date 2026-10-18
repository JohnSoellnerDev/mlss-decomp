#include "global.h"
#include "ram_routines.h"

struct SysBlock
{
    u8 filler0[0x28];
    u16 keys28;
    u16 keys2A;
    u8 filler2C[0x880 - 0x2C];
    void *oamCur880;
    u8 filler884[4];
    u8 flags888;
    u8 filler889;
    u8 flags88A;
};

struct Bits18
{
    u8 lo4 : 4;
    u8 field45 : 2;
    u8 hi2 : 2;
};

struct Bits28
{
    u8 lo2 : 2;
    u8 hi6 : 6;
};

struct Bits40
{
    u8 lo1 : 1;
    u8 hi7 : 7;
};

struct ModeBits
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

struct AnimRec
{
    u32 unk0;
    u16 unk4;
    u8 filler6[2];
    s32 unk8;
    u32 unkC;
};

struct AnimTask
{
    u8 filler0[0x18];
    const u8 *unk18;
    void *unk1C;
    u16 unk20;
    u8 filler22[6];
    u8 unk28;
};

struct Bits18b
{
    u8 lo1 : 1;
    u8 mid3 : 3;
    u8 hi4 : 4;
};

struct Flag1
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

struct Flag0
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct GfxBits
{
    u8 lo2 : 2;
    u8 pal : 4;
    u8 hi2 : 2;
};

struct AnimTick
{
    u8 filler0[0x18];
    const u8 *desc;
    u16 duration;
    u16 counter;
    u8 mode;
};

extern struct SysBlock gSys;
extern u16 gUnk_03000D3C;
extern u8 gUnk_03000D18[];
extern u8 gUnk_03000D28[];
extern u8 gUnk_03000D40;
extern u32 gUnk_03000FB8;
extern struct AnimTask *gCurObj;
extern const u8 gAnimDescA[];
extern const u8 gClassD758[];
extern const u8 gObjHandler[];
extern const u8 gTimerDesc[];
extern const u8 gHeapTagLinkWindow[];
extern const u8 gHeapTagTextboxScene[];
extern const u8 gHeapTagSceneCreate[];
void store_save_slot(void);
void task_remove(void *node, u32 kind);
void cpu_set(void *src, void *dst, u32 control);
void heap_free_a(void *p);
void set_slot_value(u32 id, s32 value);
void text_window_close(void);
void alloc_flag_block(void);
void alloc_var_block(void);
void free_flag_block(void);
void free_var_block(void);
void anim_global_create(u32 prio);
void serial_free_buffers(void);
void anim_global_free(void);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void init_scene_obj(void *obj, u32 n, const void *tag, u32 arg);
void textbox_open_scene(void *block, u32 n, const void *tag);
void scene_create(void *block, u32 n, const void *tag);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
u32 read_varlen_plus1(const void *p);
void set_blend_shadow(u32 unused, u32 mode, u32 a, u32 b);
extern const u8 gFontGlyphParts[];
void copy_shadow_to_io(void);
extern const u8 gHeapTagAnimTask[];
extern const u8 gTimerDesc[];
extern u32 gUnk_03000C84;
void *heap_alloc(u32 size, u32 a, const void *tag, u32 c);
void set_flag_2_2(void *p);
struct AnimTask *anim_task_create(void *obj, u32 prio, const void *data);
void task_insert(void *node, u8 kind, u32 data);

ASM_FUNC("asm/code_0801ABFC/serial_schedule.s", void serial_schedule(void))

ASM_FUNC("asm/code_0801ABFC/serial_timer_tick.s", void serial_timer_tick(void))

NONMATCH("asm/code_0801ABFC/serial_enter_phase_1.s", void serial_enter_phase_1(void))
{
    u8 *p = (u8 *)&gSys + 0x888;

    if ((*p & 6) == 2)
    {
        ((struct Bits18 *)gUnk_03000D18)->field45 = 1;
        ((struct Bits28 *)gUnk_03000D28)->lo2 = 0;
    }
}
END_NONMATCH

NONMATCH("asm/code_0801ABFC/serial_clear_phase.s", void serial_clear_phase(void))
{
    u8 *p = (u8 *)&gSys + 0x888;

    if ((*p & 6) == 2)
    {
        ((struct Bits18 *)gUnk_03000D18)->field45 = 0;
        ((struct Bits28 *)gUnk_03000D28)->lo2 = 0;
    }
}
END_NONMATCH

NONMATCH("asm/code_0801ABFC/serial_start_phase_2.s", void serial_start_phase_2(void))
{
    u8 *sys = (u8 *)&gSys;

    if ((sys[0x888] & 6) == 2)
    {
        if ((sys[0x88B] & 1) != 0)
        {
            ((struct Bits18 *)gUnk_03000D18)->field45 = 2;
        }
        {
            u8 *timer = gUnk_03000D28;

            *(u16 *)(timer + 4) = 0;
            ((struct Bits28 *)timer)->lo2 = 2;
        }
    }
}
END_NONMATCH

void serial_clear_timer(void)
{
    u32 zero = 0;
    cpu_set(&zero, gUnk_03000D28, 0x05000002);
}

void serial_timeout(void)
{
    REG_IME = 0;
    REG_IE &= 0xFFBF;
    REG_IME = 1;
    REG_IME = 0;
    REG_SIOCNT &= 0xFF7F;
    REG_IME = 1;
    REG_TM3CNT_H = 0;
    REG_TM3CNT_L = 0x8000;
    ((struct Bits18b *)gUnk_03000D18)->mid3 = 6;
}

u32 serial_slot_bits(u32 index, u32 unused1, u32 bits)
{
    switch (gUnk_03000D18[0] >> 6)
    {
    case 0:
        bits = index;
        break;
    case 1:
        bits = index << 2;
        break;
    case 2:
        bits = index << 4;
        break;
    case 3:
        bits = index << 6;
        break;
    }
    return bits;
}

ASM_FUNC("asm/code_0801ABFC/serial_rx_error.s", void serial_rx_error(void))

NONMATCH("asm/code_0801ABFC/init_vt_08cdd748_reset.s", void init_vt_08cdd748_reset(struct AnimTask *obj, u32 kind))
{
    obj->unk18 = gObjHandler;
    {
        u8 flags = gSys.flags88A;

        flags |= 8;
        gSys.flags88A = flags;
    }
    *(u16 *)0x02000000 = 0;
    *(u16 *)0x02000050 = 0;
    task_remove(obj, kind);
}
END_NONMATCH

void save_slot_store(void)
{
    store_save_slot();
}

ASM_FUNC("asm/code_0801ABFC/link_window_open.s", void link_window_open(void))

void serial_free_buffers(void)
{
    u32 zero;

    if ((gUnk_03000D40 & 1) != 0)
    {
        free_var_block();
        free_flag_block();
        anim_global_free();
        zero = 0;
        cpu_set(&zero, &gUnk_03000D40, 0x05000001);
        ((struct Bits40 *)&gUnk_03000D40)->lo1 = 0;
    }
}

void serial_alloc_buffers(u32 unused)
{
    if ((gUnk_03000D40 & 1) == 0)
    {
        anim_global_create(1);
        alloc_flag_block();
        alloc_var_block();
        gUnk_03000D40 |= 1;
    }
}

void serial_realloc_buffers(void)
{
    serial_free_buffers();
    if (gUnk_03000FB8 != 0)
    {
        text_window_close();
    }
    serial_alloc_buffers(0);
}

void start_scene_or_textbox(void)
{
    if (gSys.keys28 == 0x307)
    {
        void *block = heap_alloc2(0x4C, 0, gHeapTagTextboxScene, 0);

        textbox_open_scene(block, 8, gHeapTagTextboxScene);
    }
    else
    {
        void *block = heap_alloc2(0x4C, 0, gHeapTagSceneCreate, 0);

        scene_create(block, 8, gHeapTagSceneCreate);
    }
}

void update_link_slot(void)
{
    if ((gSys.keys2A & gUnk_03000D3C) != 0)
    {
        set_slot_value(0x50, -1);
    }
}

void set_link_slot_mask(u16 mask)
{
    gUnk_03000D3C = mask;
}

ASM_FUNC("asm/code_0801ABFC/init_vt_08cdd758.s", void init_vt_08cdd758(void))

ASM_FUNC("asm/code_0801ABFC/poll_spline_path.s", void poll_spline_path(void))

ASM_FUNC("asm/code_0801ABFC/vec4_normalize.s", void vec4_normalize(void))

ASM_FUNC("asm/code_0801ABFC/rotate_vec.s", void rotate_vec(void))

ASM_FUNC("asm/code_0801ABFC/vec3_normalize_0801B540.s", void vec3_normalize_0801B540(void))

ASM_FUNC("asm/code_0801ABFC/store_record.s", void store_record(void))

ASM_FUNC("asm/code_0801ABFC/quat_mul_pure_fixed.s", void quat_mul_pure_fixed(void))

ASM_FUNC("asm/code_0801ABFC/sub_0801B684.s", void sub_0801B684(void))

u32 *sub_0801B73C(u32 *rec, u32 a, u32 b, u32 c, u32 d)
{
    rec[0] = a;
    rec[1] = b;
    rec[2] = c;
    rec[3] = d;
    return rec;
}

void sub_0801B750(u32 *rec)
{
    rec[0] = 0;
    rec[1] = 0;
    rec[2] = 0;
    rec[3] = 0;
}

void init_anim_record(struct AnimRec *rec, u32 a, u16 b, s16 c)
{
    rec->unk0 = a;
    rec->unk4 = b;
    rec->unk8 = c;
    rec->unkC = 0xFFFF0000;
}

void clear_anim_record(struct AnimRec *rec)
{
    rec->unk0 = 0;
    rec->unk4 = 0;
    rec->unk8 = 0;
    rec->unkC = 0;
}

void anim_push_frame(u8 *rec)
{
    u16 *q;
    u32 flags;

    rec[0x1C]++;
    q = gSys.oamCur880;
    *q = rec[0x1D];
    q++;
    *q = rec[0x1C];
    q++;
    flags = (rec[0x20] << 12) | (rec[0x21] << 10) | *(u16 *)(rec + 0x1E);
    *q = flags;
    q += 2;
    gSys.oamCur880 = q;
}

ASM_FUNC("asm/code_0801ABFC/set_blend_shadow.s", void set_blend_shadow(u32 unused, u32 mode, u32 a, u32 b))

void anim_tick(struct AnimTick *obj)
{
    obj->counter++;
    set_blend_shadow((u32)obj, obj->mode, obj->counter, obj->duration);
    if (obj->counter >= obj->duration)
    {
        if (obj != 0)
        {
            obj->desc = gAnimDescA;
            task_remove(obj, 3);
        }
    }
}

void anim_task_stop(struct AnimTask *obj, u32 prio)
{
    obj->unk18 = gAnimDescA;
    task_remove(obj, prio);
}

struct AnimTask *anim_task_start(struct AnimTask *obj, u32 prio, const void *data, u32 mode, u32 duration)
{
    u8 kind = prio;
    u16 dur = duration;

    task_insert(obj, kind, (u32)data);
    obj->unk18 = gAnimDescA;
    if (dur == 0)
    {
        set_blend_shadow((u32)obj, mode, 1, 1);
        copy_shadow_to_io();
        if (obj != 0)
        {
            anim_task_stop(obj, 3);
        }
    }
    else
    {
        ((struct AnimTick *)obj)->duration = dur;
        ((struct AnimTick *)obj)->mode = mode;
        ((struct AnimTick *)obj)->counter = 0;
    }
    return obj;
}

void init_vt_08cdd758_remove(struct AnimTask *obj, u32 kind)
{
    obj->unk18 = gClassD758;
    task_remove(obj, kind);
}

ASM_FUNC("asm/code_0801ABFC/timer_draw.s", void timer_draw(void))

void obj_set_bytes_26_27(u8 *obj, u8 value)
{
    obj[0x26] = value;
    obj[0x27] = value;
    obj[0x28] |= 1;
}

void timer_set_time(u8 *obj, u8 h, u8 m, s8 s)
{
    u8 *p = obj + 0x22;
    u8 zero = 0;
    s32 hundredths;

    *p = h;
    p++;
    *p = m;
    p++;
    *p = s;
    hundredths = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(s * 100, 0x3C);
    obj[0x25] = hundredths;
    obj[0x29] = zero;
    ((struct Flag1 *)&obj[0x28])->b1 = 0;
}

void anim_set_gfx(struct AnimTask *obj, u16 index, u8 pal)
{
    u32 size;

    obj->unk20 = index;
    if (index != 0xFFFF)
    {
        size = read_varlen_plus1(gFontGlyphParts);
        dma_copy_or_queue(obj->unk1C, (void *)(index + 0x06010000), size, 0x20, 0);
    }
    ((struct GfxBits *)&obj->unk28)->pal = pal;
}

void anim_task_free(struct AnimTask *obj, u32 prio)
{
    obj->unk18 = gTimerDesc;
    if (obj->unk1C != 0)
    {
        heap_free_a(obj->unk1C);
    }
    task_remove(obj, prio);
}

struct AnimTask *anim_task_create(void *obj, u32 prio, const void *data)
{
    void *buf;
    struct AnimTask *t;

    task_insert(obj, (u8)prio, (u32)data);
    t = obj;
    t->unk18 = gTimerDesc;
    buf = heap_alloc(read_varlen_plus1(gFontGlyphParts), 1, gHeapTagAnimTask, 0);
    t->unk1C = buf;
    ((void (*)(const void *, void *))gUnk_03000C84)(gFontGlyphParts, buf);
    t->unk20 = 0;
    t->filler22[0] = 0;
    t->filler22[1] = 0;
    t->filler22[2] = 0;
    t->filler22[3] = 0;
    ((struct Flag0 *)&t->unk28)->b0 = 0;
    return t;
}

void anim_global_free(void)
{
    if (gCurObj != 0)
    {
        anim_task_free(gCurObj, 3);
        gCurObj = 0;
    }
}

void anim_global_create(u32 prio)
{
    if (gCurObj == 0)
    {
        gCurObj = anim_task_create(heap_alloc2(0x2C, 0, gHeapTagAnimTask, 0), (u8)prio, gHeapTagAnimTask);
        set_flag_2_2(gCurObj);
    }
}

ASM_FUNC("asm/code_0801ABFC/anim_load_gfx.s", void anim_load_gfx(void))

ASM_FUNC("asm/code_0801ABFC/clip_to_screen_rows.s", void clip_to_screen_rows(void))

ASM_FUNC("asm/code_0801ABFC/sub_0801C124.s", void sub_0801C124(void))
