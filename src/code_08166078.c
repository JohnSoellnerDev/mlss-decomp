#include "global.h"

extern u8 fill_rect_4bpp_clip[];
extern u8 span_fill_rows[];
extern u8 span_extents_line[];
extern u8 gUnk_08002BE8[];
extern u8 dot2x2_4bpp_clip[];
extern u8 gUnk_080026F0[];
extern u8 dot3x3_4bpp_clip[];
extern u8 line_3px_4bpp[];
extern u8 line_1px_4bpp[];

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void dma_cancel_request(void *p);
void cpu_fast_set(const void *src, void *dst, u32 control);
void heap_free_b(void *p);
void obj_release_slots(u32 sprite);
void release_obj_131c(u32 handle);
void heap_free(void *p);
void text_state_free(void);
u32 smoothstep_lerp(s32 start, s32 end, s32 steps, s32 step);
u32 lerp_step_08164EE0(s32 start, s32 end, s32 steps, s32 step);
void write_var(u16 id, u16 value);
void tween_groups_set_flag_bit(u16 index, u8 value);
void obj_run_triples_free(void *obj, u32 flags);
void free_if_bit0(void *ptr, u32 flags);
void obj_run_children3_free(void *obj, u32 flags);
void obj_run_callbacks(void);
void set_flags_2_3(u8 *p);
void free_if_bit0_b(void *ptr, u32 flags);
void task_remove(void *node, u32 kind);
extern const u8 gClassEaf8[];
extern u8 gSys[];
extern u8 gUnk_02000050[];
u8 timer_tick(void *timer);
void palette_fill(u32 v);
void slot_reset(u32 id);
void set_slot_target(u32 a, u32 b, u32 c);
void obj_copy_pos_to_shadow(void *src);
void obj_copy_end_points(u8 *obj);
void obj_key_state_update(u8 *obj);
void obj_phase4_step(u8 *obj);
void obj_set_mode_if_flag8(u8 *obj);
void obj_panel_sequence(u8 *obj);
void *heap_alloc2(u32 size, u32 flags, const u8 *tag, u32 zero);
void *obj_init_big(void *p, u32 kind);
void list_insert(void *list, void *node);
void obj_create_sprite_10(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e);
void obj_set_table_and_init(void *obj, const void *table);
extern const u8 gClassEb28[];
extern const u8 gHeapTagFourChild0[];
extern const u8 gHeapTagFourChild1[];
extern const u8 gHeapTagFourChild2[];
extern const u8 gHeapTagFourChild3[];
extern const u8 gObjScript08216582[];
extern const u8 gObjScript08216578[];
extern const u8 gObjScript0821658c[];
extern const u8 gObjScript0821659a[];

struct VtSlot
{
    s16 offset;
    u16 filler2;
    void (*fn)(void *, u32);
};
void obj_release_sprites(void *obj);
void obj_update_sprites(void *obj);
void obj_update_frame_flags(void *obj);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_set_anim_from_offset(void *obj);
extern const u8 gFrameRecordOffsets[];
extern const u8 gKeyRecOffsets[];
u32 obj_dialog_step(u8 *obj);

u32 obj_slide_advance(u8 *obj)
{
    if (*(s32 *)(obj + 0x9C) > *(s32 *)(obj + 0x98))
        return 1;
    *(s16 *)*(void **)(obj + 0x50) = -smoothstep_lerp(*(s32 *)(obj + 0x28), *(s32 *)(obj + 0x90), *(s32 *)(obj + 0x98), *(s32 *)(obj + 0x9C));
    *(s16 *)*(void **)(obj + 0x54) = -smoothstep_lerp(*(s32 *)(obj + 0x2C), *(s32 *)(obj + 0x94), *(s32 *)(obj + 0x98), *(s32 *)(obj + 0x9C));
    *(s32 *)(obj + 0x9C) += 1;
    return 0;
}

void obj_slide_setup(u8 *obj, s32 x, s32 y, s32 steps)
{
    s32 w;
    s32 h;

    *(s32 *)(obj + 0x9C) = 0;
    *(s32 *)(obj + 0x98) = steps;
    if (x <= 3)
        x = 4;
    if (y <= 3)
        y = 4;
    w = *(s32 *)(obj + 0x30) << 3;
    if (x + w > 0xEC)
        x = 0xEC - w;
    h = *(s32 *)(obj + 0x34) << 3;
    if (y + h > 0x9C)
        y = 0x9C - h;
    *(s32 *)(obj + 0x90) = x;
    *(s32 *)(obj + 0x94) = y;
    obj_release_slots(*(u32 *)obj);
    obj_release_slots(*(u32 *)(obj + 4));
}

static inline void obj_dma_flush(u8 *obj)
{
    dma_cancel_request(*(void **)(obj + 0x38));
    dma_copy_or_queue(*(void **)(obj + 0x20), *(void **)(obj + 0x38), 0x3000, 0x20, 0);
}

u32 obj_flush_anim(u8 *obj)
{
    u32 status;

    status = obj_dialog_step(obj);
    obj_dma_flush(obj);
    return status;
}

void obj_release_tiles(u8 *obj)
{
    u32 zero;

    zero = 0;
    cpu_fast_set(&zero, *(void **)(obj + 0x20), 0x01000C00);
    obj_release_slots(*(u32 *)obj);
    obj_release_slots(*(u32 *)(obj + 4));
}

void obj_release_and_flush(u8 *obj, u8 flush)
{
    obj_release_tiles(obj);
    if (flush != 0)
        obj_dma_flush(obj);
}

void obj_flush_tiles(u8 *obj)
{
    dma_cancel_request(*(void **)(obj + 0x38));
    dma_copy_or_queue(*(void **)(obj + 0x20), *(void **)(obj + 0x38), 0x3000, 0x20, 0);
}

// Same result as the original; the original also keeps a dead test on p[1] first, which GCC drops.
NONMATCH("asm/code_08166078/state_clamp_pair.s", s32 state_clamp_pair(u32 unused, u8 *p))
{
    s32 v;

    v = p[1] + 2;
    if (v > 3)
    {
        if (v > 0xC)
            v = 0xC;
    }
    else
    {
        v = 3;
    }
    return v;
}
END_NONMATCH

s32 sub_08166200(u32 unused, u8 *p)
{
    s32 v;

    v = p[0] + 4;
    if (v > 3)
    {
        if (v > 0x1E)
            v = 0x1E;
    }
    else
    {
        v = 3;
    }
    return v;
}

void window_destroy(u8 *obj, u32 flag)
{
    *(u16 *)*(void **)(obj + 0x50) = 0;
    *(u16 *)*(void **)(obj + 0x54) = 0;
    release_obj_131c(*(u32 *)obj);
    release_obj_131c(*(u32 *)(obj + 4));
    if (*(void **)(obj + 0x48) != NULL)
        heap_free(*(void **)(obj + 0x48));
    if (*(void **)(obj + 0x44) != NULL)
        heap_free(*(void **)(obj + 0x44));
    if (*(void **)(obj + 0x1C) != NULL)
        heap_free(*(void **)(obj + 0x1C));
    if (*(void **)(obj + 0x24) != NULL)
        heap_free(*(void **)(obj + 0x24));
    text_state_free();
    if ((flag & 1) != 0)
        heap_free_b(obj);
}

void rom_offset_call_a(u8 *obj, u32 a)
{
    void (*fn)(void *, void *, u32);
    u32 mode;

    mode = (u16)a;
    if (*(u8 *)(obj + 0x34) != 0)
    {
        fn = (void (*)(void *, void *, u32))(*(u8 **)(obj + 0x20) + ((u32)span_fill_rows - (u32)fill_rect_4bpp_clip));
        fn(*(void **)(obj + 4), *(void **)(obj + 0xc), mode);
    }
}

void rom_offset_call_b(u8 *obj, u32 a)
{
    u32 off;

    if (*(u8 *)(obj + 0x34) != 0)
    {
        off = (u32)span_extents_line - (u32)fill_rect_4bpp_clip;
        ((void (*)(void *, u32))(*(u8 **)(obj + 0x20) + off))(*(void **)(obj + 0xc), a);
        ((void (*)(void *, u32))(*(u8 **)(obj + 0x20) + off))(*(void **)(obj + 0xc), a + 8);
        ((void (*)(void *, u32))(*(u8 **)(obj + 0x20) + off))(*(void **)(obj + 0xc), a + 0x10);
    }
}

void rom_offset_call_c(u8 *obj)
{
    void (*fn)(void *, u32, s32);

    if (*(u8 *)(obj + 0x34) != 0)
    {
        fn = (void (*)(void *, u32, s32))(*(u8 **)(obj + 0x20) + ((u32)gUnk_08002BE8 - (u32)fill_rect_4bpp_clip));
        fn(*(void **)(obj + 0xc), 10000, -10000);
    }
}

void tilemap_write_entry(u8 *obj, u32 index, u32 tile, u32 pal, u32 hi)
{
    u32 value;
    u16 *table;
    u16 *dst;

    value = (u16)tile;
    pal <<= 16;
    hi <<= 16;
    table = *(u16 **)(obj + 0x18);
    dst = (u16 *)((index << 1) + (u32)table);
    value |= pal >> 11;
    value |= hi >> 6;
    *dst = value;
}

void palette_fade16(u8 *obj, u32 red, u32 green, u32 blue)
{
    u16 table[16];
    u32 red_end;
    u32 g_end;
    u32 b_end;
    u32 step;
    u32 next;
    u16 *out;
    u32 r_a;
    u32 g_a;
    u32 b_a;
    u32 r_b;
    u32 g_b;
    u32 b_b;

    red_end = (u16)red;
    g_end = (u16)green;
    b_end = (u16)blue;
    step = 0;
    out = table;
    do
    {
        r_a = lerp_step_08164EE0(0, red_end, 0x10, step);
        g_a = lerp_step_08164EE0(0, g_end, 0x10, step);
        b_a = lerp_step_08164EE0(0, b_end, 0x10, step);
        g_a <<= 5;
        g_a |= r_a;
        *out = (b_a << 10) | g_a;
        next = step + 1;
        r_b = lerp_step_08164EE0(0, red_end, 0x10, next);
        g_b = lerp_step_08164EE0(0, g_end, 0x10, next);
        b_b = lerp_step_08164EE0(0, b_end, 0x10, next);
        g_b <<= 5;
        g_b |= r_b;
        out[1] = (b_b << 10) | g_b;
        out += 2;
        step += 2;
    } while ((s32)step <= 15);
    cpu_fast_set(table, *(void **)(obj + 0x18), 8);
}

void rom_offset_call_d(u8 *obj, u32 a, u16 b)
{
    void (*fn)(void *, u32, u32);

    fn = (void (*)(void *, u32, u32))(*(u8 **)(obj + 0x20) + ((u32)line_1px_4bpp - (u32)fill_rect_4bpp_clip));
    fn(*(void **)(obj + 4), a, b);
}

void obj_child_ram_call(u8 *obj, u32 a, u16 b)
{
    void (*fn)(void *, u32, u32);

    fn = (void (*)(void *, u32, u32))(*(u8 **)(obj + 0x20) + ((u32)line_3px_4bpp - (u32)fill_rect_4bpp_clip));
    fn(*(void **)(obj + 4), a, b);
}

void rom_offset_call_e(u8 *obj, u32 a, u32 b, u16 c)
{
    void (*fn)(void *, u32, u32, u32);

    fn = (void (*)(void *, u32, u32, u32))(*(u8 **)(obj + 0x20) + ((u32)dot2x2_4bpp_clip - (u32)fill_rect_4bpp_clip));
    fn(*(void **)(obj + 4), a, b, c);
}

void rom_offset_call_f(u8 *obj, u32 a, u32 b, u16 c)
{
    void (*fn)(void *, u32, u32, u32);

    fn = (void (*)(void *, u32, u32, u32))(*(u8 **)(obj + 0x20) + ((u32)gUnk_080026F0 - (u32)fill_rect_4bpp_clip));
    fn(*(void **)(obj + 4), a, b, c);
}

void rom_offset_call_g(u8 *obj, u32 a, u32 b, u16 c)
{
    void (*fn)(void *, u32, u32, u32);

    fn = (void (*)(void *, u32, u32, u32))(*(u8 **)(obj + 0x20) + ((u32)dot3x3_4bpp_clip - (u32)fill_rect_4bpp_clip));
    fn(*(void **)(obj + 4), a, b, c);
}

// The original subtracts two symbol names that sit at the same ROM address; only one exists in
// config/symbols.txt, so the difference folds to 0 here.
extern void fill_rect_4bpp_clip_routine(void);

void rom_offset_call_h(u8 *obj, u32 a, u16 b)
{
    void (*fn)(void *, u32, u32);

    fn =(void (*)(void *, u32, u32))(*(u8 **)(obj + 0x20) + ((u32)fill_rect_4bpp_clip_routine - (u32)fill_rect_4bpp_clip));
    fn(*(void **)(obj + 4), a, b);
}

void obj_clear_buffer(u8 *obj)
{
    u32 zero;

    zero = 0;
    cpu_fast_set(&zero, *(void **)(obj + 4), 0x01000800);
}

void obj_upload_tiles(u8 *obj)
{
    dma_copy_or_queue(*(void **)(obj + 4), *(void **)(obj + 0x10), 0x2000, 0x20, 0);
    *(u16 *)*(void **)(obj + 0x2C) = *(u32 *)(obj + 0x24);
    *(u16 *)*(void **)(obj + 0x30) = *(u32 *)(obj + 0x28);
}

void obj_set_offset(u8 *obj, u32 x, u32 y)
{
    *(u32 *)(obj + 0x24) = x;
    *(u32 *)(obj + 0x28) = y;
}

void obj_free_pair_bufs(u8 *obj, u32 flag)
{
    heap_free(*(void **)(obj + 0x1C));
    if (*(void **)(obj + 8) != NULL)
        heap_free(*(void **)(obj + 8));
    heap_free(*(void **)obj);
    if ((flag & 1) != 0)
        heap_free_b(obj);
}

// Same bits as the original; only the registers of the value and the mask are swapped.
NONMATCH("asm/code_08166078/obj_refresh_flags.s", void obj_refresh_flags(u8 *obj, s32 value))
{
    u32 flags;
    u32 mask;
    u8 *sprite;
    u32 v;

    flags = (u32)*(u16 *)(obj + 0x6C) << 16;
    if (flags != 0)
    {
        if (((flags >> 17) & 1) != 0)
            obj_release_sprites(obj);
        else
            obj_update_sprites(obj);
    }
    else
    {
        obj_update_sprites(obj);
    }
    obj_update_frame_flags(obj);
    if (value < *(s32 *)(obj + 0x3C) - 0x800)
    {
        sprite = *(u8 **)(obj + 0x10);
        v = sprite[0x1F];
        v |= 0xC0;
        v |= 0x30;
        v |= 0xC;
        mask = 3;
    }
    else
    {
        sprite = *(u8 **)(obj + 0x10);
        v = 0x3F & sprite[0x1F];
        v |= 0x80;
        v &= -0x31;
        v |= 0x20;
        v &= -0xD;
        v |= 8;
        v &= -4;
        mask = 2;
    }
    sprite[0x1F] = v | mask;
}
END_NONMATCH

void obj_update_anim_state(u8 *obj)
{
    s32 idx;
    const u8 *frame;
    void *rec;

    if (!(*(u16 *)(obj + 0x56) == *(u16 *)(obj + 0x58) && *(u16 *)(obj + 0x52) == *(u16 *)(obj + 0x54)))
    {
        idx = *(u16 *)(obj + 0x56);
        if (idx <= 3)
        {
            if (idx < 0)
                goto skip_record;
            rec = *(void **)(obj + 0x10);
            frame = gFrameRecordOffsets + 4;
            obj_load_record(rec, frame[*(u16 *)(obj + 0x52)] + (gKeyRecOffsets + 1)[idx], 0, 0, 0);
            if (*(u16 *)(obj + 0x56) == 0)
                *(u8 *)(*(u8 **)(obj + 0x10) + 0x12) |= 0x10;
        }
skip_record:
        ;
    }
    if (*(s32 *)(obj + 0x20) <= 0x87FF)
    {
        *(u8 *)(*(u8 **)(obj + 0x10) + 0x1F) |= 3;
    }
    else
    {
        s32 b;
        s32 v;

        b = *(u8 *)(*(u8 **)(obj + 0x10) + 0x1F);
        v = -4;
        v &= b;
        v |= 2;
        *(u8 *)(*(u8 **)(obj + 0x10) + 0x1F) = v;
    }
    obj_set_anim_from_offset(obj);
}

u32 obj_create_four_children(u8 *obj, u8 *list)
{
    *(const u8 **)(obj + 0x10) = gClassEb28;
    *(void **)(obj + 4) = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild0, 0), 3);
    list_insert(list, *(void **)(obj + 4));
    *(void **)obj = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild1, 0), 4);
    list_insert(list, *(void **)obj);
    *(void **)(obj + 8) = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild2, 0), 5);
    list_insert(list, *(void **)(obj + 8));
    *(void **)(obj + 0xC) = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild3, 0), 6);
    list_insert(list, *(void **)(obj + 0xC));
    obj_create_sprite_10(*(void **)(obj + 4), 4, 0x5002, 0, 1, 3);
    obj_create_sprite_10(*(void **)obj, 4, 0x5002, 0, 2, 3);
    obj_create_sprite_10(*(void **)(obj + 8), 4, 0x5002, 0, 0xA, 1);
    obj_create_sprite_10(*(void **)(obj + 0xC), 4, 0x5002, 0, 0xA, 1);
    obj_set_table_and_init(*(void **)(obj + 4), gObjScript08216582);
    obj_set_table_and_init(*(void **)obj, gObjScript08216578);
    obj_set_table_and_init(*(void **)(obj + 8), gObjScript0821658c);
    obj_set_table_and_init(*(void **)(obj + 0xC), gObjScript0821659a);
    return (u32)obj;
}

ASM_FUNC("asm/code_08166078/scene_init_kind2.s", void scene_init_kind2(void))

void obj_end_sequence(u8 *obj, u32 arg)
{
    u8 *ext;
    u8 *child;
    struct VtSlot *slot;

    *(void **)(obj + 0x18) = (void *)gClassEaf8;
    write_var(0x1CDE, (u16)*(u32 *)(obj + 0x5C));
    if (*(s32 *)(obj + 0x5C) > 1)
        tween_groups_set_flag_bit(0x1F4, 1);
    ext = *(u8 **)(obj + 0x40);
    if (ext != NULL)
    {
        heap_free(*(void **)(ext + 0x230));
        heap_free_b(ext);
    }
    if (*(void **)(obj + 0x34) != NULL)
        obj_run_triples_free(*(void **)(obj + 0x34), 3);
    child = *(u8 **)(obj + 0x30);
    if (child != NULL)
    {
        slot = (struct VtSlot *)(*(u8 **)(child + 0x10) + 8);
        slot->fn(child + slot->offset, 3);
    }
    if (*(void **)(obj + 0x3C) != NULL)
        free_if_bit0(*(void **)(obj + 0x3C), 3);
    if (*(void **)(obj + 0x38) != NULL)
        obj_run_children3_free(*(void **)(obj + 0x38), 3);
    child = *(u8 **)(obj + 0x4C);
    if (child != NULL)
    {
        slot = (struct VtSlot *)(*(u8 **)(child + 0x18) + 0x10);
        slot->fn(child + slot->offset, 3);
    }
    if (*(void **)(obj + 0x44) != NULL)
        obj_free_pair_bufs(*(u8 **)(obj + 0x44), 3);
    if (*(void **)(obj + 0x48) != NULL)
        window_destroy(*(u8 **)(obj + 0x48), 3);
    child = *(u8 **)(obj + 0x1C);
    if (child != NULL)
    {
        slot = (struct VtSlot *)(*(u8 **)(child + 0xC) + 8);
        slot->fn(child + slot->offset, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(*(u8 **)(obj + 0x14));
    free_if_bit0_b(obj + 0x50, 2);
    task_remove(obj, arg);
}

void obj_state_machine(u8 *obj)
{
    u32 state;
    u32 off;
    u8 *dst;
    u16 *clear_ptr;
    struct VtSlot *slot;

    off = 0x884;
    state = *(u32 *)(obj + 0x54);
    switch (state)
    {
    case 0:
        *(s32 *)(gSys + off) = -1;
        set_slot_target(0, 4, -1);
        *(u32 *)(obj + 0x54) = 1;
        break;
    case 1:
        if (timer_tick(obj + 0x50) != 0)
            *(u32 *)(obj + 0x54) = 2;
        else
            obj_copy_end_points(obj);
        break;
    case 2:
        obj_panel_sequence(obj);
        break;
    case 3:
        obj_key_state_update(obj);
        break;
    case 4:
        obj_phase4_step(obj);
        break;
    case 5:
        obj_set_mode_if_flag8(obj);
        break;
    case 6:
        if (timer_tick(obj + 0x50) != 0)
            *(u32 *)(obj + 0x54) = 7;
        break;
    case 7:
        palette_fill(0);
        slot_reset(0);
        clear_ptr = (u16 *)gUnk_02000050;
        *clear_ptr = 0;
        clear_ptr += 2;
        *clear_ptr = 0;
        if (obj != NULL)
        {
            slot = (struct VtSlot *)(*(u8 **)(obj + 0x18) + 0x10);
            slot->fn(obj + slot->offset, 3);
        }
        return;
    default:
        break;
    }
    dst = *(u8 **)(*(u8 **)(obj + 0x2C) + 0x10);
    *(u16 *)(dst + 0x14) = *(u16 *)(*(u8 **)(*(u8 **)(obj + 0x28) + 0x10) + 0x14) << 1;
    obj_copy_pos_to_shadow(*(void **)(obj + 0x3C));
}

ASM_FUNC("asm/code_08166078/obj_update_grid_children.s", void obj_update_grid_children(void))

ASM_FUNC("asm/code_08166078/obj_panel_sequence.s", void obj_panel_sequence(u8 *obj))
