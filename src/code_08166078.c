#include "global.h"
#include "rope_jump.h"

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
extern u16 gUnk_02000050[]; // REG_BUF_BLDCNT; [2] is the copy of REG_BLDY
u8 timer_tick(void *timer);
void palette_fill(u32 v);
void slot_reset(u32 id);
void set_slot_target(u32 a, u32 b, u32 c);
void obj_copy_pos_to_shadow(void *src);
void obj_copy_end_points(struct Phase4Obj *obj);
void obj_key_state_update(struct Phase4Obj *obj);
void obj_phase4_step(struct Phase4Obj *obj);
void obj_set_mode_if_flag8(struct Phase4Obj *obj);
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

/* An object made of four sprite children (art 0x5002); obj_create_four_children makes them in
 * the order A, B, C, D. */
struct FourChildObj
{
    void *childB;
    void *childA;
    void *childC;
    void *childD;
    const u8 *cls;
};

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
s32 state_clamp_pair(u32 unused, u8 *p)
{
    s32 style;
    s32 v;

    style = p[1];
    if (style == 2 || style == 4 || style == 5 || ++style > 0xC)
    {
        v = p[1] + 2;
    }
    else
    {
        v = p[1] + 2;
    }
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

/* Fills the 16-color palette at obj+0x18 with a ramp from black (color 0) toward (red, green, blue)
   in 16 steps, each color an RGB555 value from lerp_step_08164EE0; built two colors per loop pass
   on the stack and copied with one CpuFastSet of 8 words. */
void palette_fade16(u8 *obj, u32 red, u32 green, u32 blue)
{
    u16 colors[16];
    u32 red_end;
    u32 green_end;
    u32 blue_end;
    u32 step;
    u32 next;
    u16 *out;
    u32 r_even;
    u32 g_even;
    u32 b_even;
    u32 r_odd;
    u32 g_odd;
    u32 b_odd;

    red_end = (u16)red;
    green_end = (u16)green;
    blue_end = (u16)blue;
    step = 0;
    out = colors;
    do
    {
        r_even = lerp_step_08164EE0(0, red_end, 0x10, step);
        g_even = lerp_step_08164EE0(0, green_end, 0x10, step);
        b_even = lerp_step_08164EE0(0, blue_end, 0x10, step);
        g_even <<= 5;
        g_even |= r_even;
        *out = (b_even << 10) | g_even;
        next = step + 1;
        r_odd = lerp_step_08164EE0(0, red_end, 0x10, next);
        g_odd = lerp_step_08164EE0(0, green_end, 0x10, next);
        b_odd = lerp_step_08164EE0(0, blue_end, 0x10, next);
        g_odd <<= 5;
        g_odd |= r_odd;
        out[1] = (b_odd << 10) | g_odd;
        out += 2;
        step += 2;
    } while ((s32)step <= 15);
    cpu_fast_set(colors, *(void **)(obj + 0x18), 8);
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
void obj_refresh_flags(u8 *obj, s32 value)
{
    struct PrioBits
    {
        u8 f0 : 2;
        u8 f1 : 2;
        u8 f2 : 2;
        u8 f3 : 2;
    } __attribute__((packed));
    u32 flags;
    u8 *sprite;

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
        struct PrioBits bits;

        sprite = *(u8 **)(obj + 0x10);
        bits = *(struct PrioBits *)(sprite + 0x1F);
        bits.f3 = 3;
        bits.f2 = 3;
        bits.f1 = 3;
        bits.f0 = 3;
        *(struct PrioBits *)(sprite + 0x1F) = bits;
    }
    else
    {
        struct PrioBits bits;

        sprite = *(u8 **)(obj + 0x10);
        bits = *(struct PrioBits *)(sprite + 0x1F);
        bits.f3 = 2;
        bits.f2 = 2;
        bits.f1 = 2;
        bits.f0 = 2;
        *(struct PrioBits *)(sprite + 0x1F) = bits;
    }
}

/*
 * Jump-rope figure: when its animation mode or facing has changed, loads the sprite record for
 * the pair (modes 0-3; mode 0 also sets bit 4 of the sprite's attr12). Draws the sprite behind the
 * rope (priority 3) while the figure is at y <= 0x87FF and in front of it (2) otherwise, then
 * updates its shadow for the jump height (obj_set_anim_from_offset).
 */
void obj_update_anim_state(struct GridObj *obj)
{
    s32 mode;
    const u8 *dirRecords;
    void *sprite;

    if (!(obj->mode56 == obj->mode58 && obj->dir52 == obj->unk54))
    {
        mode = obj->mode56;
        if (mode <= 3)
        {
            if (mode < 0)
                goto skip_record;
            sprite = obj->unk10;
            dirRecords = gFrameRecordOffsets + 4;
            obj_load_record(sprite, dirRecords[obj->dir52] + (gKeyRecOffsets + 1)[mode], 0, 0, 0);
            if (obj->mode56 == 0)
                *(u8 *)&((struct GridSprite *)obj->unk10)->attr12 |= 0x10;
        }
skip_record:
        ;
    }
    if ((s32)obj->unk20 <= 0x87FF)
    {
        *(u8 *)&((struct GridSprite *)obj->unk10)->prio1F |= 3;
    }
    else
    {
        s32 bits;
        s32 prio;

        bits = ((struct GridSprite *)obj->unk10)->prio1F;
        prio = -4;
        prio &= bits;
        prio |= 2;
        ((struct GridSprite *)obj->unk10)->prio1F = prio;
    }
    obj_set_anim_from_offset(obj);
}

/* Sets obj's class to gClassEb28 and gives it four 0x6C-byte children (kinds 3-6), each put in
 * list, with a sprite of art 0x5002 (A: anim 1, B: anim 2, both priority 3; C and D: anim 10,
 * priority 1) and its own object script. Returns obj. */
u32 obj_create_four_children(struct FourChildObj *obj, u8 *list)
{
    obj->cls = gClassEb28;
    obj->childA = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild0, 0), 3);
    list_insert(list, obj->childA);
    obj->childB = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild1, 0), 4);
    list_insert(list, obj->childB);
    obj->childC = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild2, 0), 5);
    list_insert(list, obj->childC);
    obj->childD = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagFourChild3, 0), 6);
    list_insert(list, obj->childD);
    obj_create_sprite_10(obj->childA, 4, 0x5002, 0, 1, 3);
    obj_create_sprite_10(obj->childB, 4, 0x5002, 0, 2, 3);
    obj_create_sprite_10(obj->childC, 4, 0x5002, 0, 0xA, 1);
    obj_create_sprite_10(obj->childD, 4, 0x5002, 0, 0xA, 1);
    obj_set_table_and_init(obj->childA, gObjScript08216582);
    obj_set_table_and_init(obj->childB, gObjScript08216578);
    obj_set_table_and_init(obj->childC, gObjScript0821658c);
    obj_set_table_and_init(obj->childD, gObjScript0821659a);
    return (u32)obj;
}

ASM_FUNC("asm/code_08166078/scene_init_kind2.s", void scene_init_kind2(void))

/* Destructor of the jump-rope minigame: saves the levels cleared to var 0x1CDE (and sets tween
 * flag 0x1F4 after more than one), destroys and frees every child object, the windows and the
 * timer, then removes the task (flags bit 0: free it too). */
void obj_end_sequence(struct Phase4Obj *obj, u32 flags)
{
    struct GridObj *path;
    u8 *child;
    struct VtSlot *slot;

    obj->cls = (const struct RopeJumpClass *)gClassEaf8;
    write_var(0x1CDE, (u16)obj->unk5C);
    if (obj->unk5C > 1)
        tween_groups_set_flag_bit(0x1F4, 1);
    path = obj->path40;
    if (path != NULL)
    {
        heap_free(path->path230);
        heap_free_b(path);
    }
    if (obj->unk34 != NULL)
        obj_run_triples_free(obj->unk34, 3);
    child = (u8 *)obj->sub30;
    if (child != NULL)
    {
        /* the child's class at +0x10, its destructor slot at +8 */
        slot = (struct VtSlot *)(*(u8 **)(child + 0x10) + 8);
        slot->fn(child + slot->offset, 3);
    }
    if (obj->unk3C != NULL)
        free_if_bit0(obj->unk3C, 3);
    if (obj->unk38 != NULL)
        obj_run_children3_free(obj->unk38, 3);
    child = obj->unk4C;
    if (child != NULL)
    {
        slot = (struct VtSlot *)(*(u8 **)(child + 0x18) + 0x10);
        slot->fn(child + slot->offset, 3);
    }
    if (obj->unk44 != NULL)
        obj_free_pair_bufs(obj->unk44, 3);
    if (obj->win48 != NULL)
        window_destroy(obj->win48, 3);
    child = obj->unk1C;
    if (child != NULL)
    {
        slot = (struct VtSlot *)(*(u8 **)(child + 0xC) + 8);
        slot->fn(child + slot->offset, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(obj->unk14);
    free_if_bit0_b(&obj->wait, 2);
    task_remove(obj, flags);
}

// The jump-rope minigame, one frame: runs the current phase (ROPE_JUMP_PHASE_*), then keeps
// turner B's sprite word 0x14 at twice turner A's and moves the scroll tween's shadow.
void obj_state_machine(struct Phase4Obj *obj)
{
    u32 phase;
    u32 palMaskOffset;
    struct GridSprite *spriteB;
    u16 *blend;

    palMaskOffset = 0x884; // gSys: mask of the palettes to reload
    phase = obj->unk54;
    switch (phase)
    {
    case ROPE_JUMP_PHASE_START:
        *(s32 *)((u8 *)gSys + palMaskOffset) = -1;
        set_slot_target(0, 4, -1); // song 4 on player slot 0
        obj->unk54 = ROPE_JUMP_PHASE_FADE_IN;
        break;
    case ROPE_JUMP_PHASE_FADE_IN:
        if (timer_tick(&obj->wait) != 0)
            obj->unk54 = ROPE_JUMP_PHASE_MENU;
        else
            obj_copy_end_points(obj);
        break;
    case ROPE_JUMP_PHASE_MENU:
        obj_panel_sequence((u8 *)obj);
        break;
    case ROPE_JUMP_PHASE_TUTORIAL:
        obj_key_state_update(obj);
        break;
    case ROPE_JUMP_PHASE_PLAY:
        obj_phase4_step(obj);
        break;
    case ROPE_JUMP_PHASE_PAUSED:
        obj_set_mode_if_flag8(obj);
        break;
    case ROPE_JUMP_PHASE_FADE_OUT:
        if (timer_tick(&obj->wait) != 0)
            obj->unk54 = ROPE_JUMP_PHASE_EXIT;
        break;
    case ROPE_JUMP_PHASE_EXIT:
        palette_fill(0);
        slot_reset(0);
        blend = gUnk_02000050;
        *blend = 0; // BLDCNT
        blend += 2;
        *blend = 0; // BLDY
        if (obj != NULL)
            obj->cls->destroy((u8 *)obj + obj->cls->thisOffset, 3);
        return;
    default:
        break;
    }
    spriteB = obj->unk2C->unk10;
    spriteB->unk14 = ((struct GridSprite *)obj->unk28->unk10)->unk14 << 1;
    obj_copy_pos_to_shadow(obj->unk3C);
}

ASM_FUNC("asm/code_08166078/obj_update_grid_children.s", void obj_update_grid_children(void))

ASM_FUNC("asm/code_08166078/obj_panel_sequence.s", void obj_panel_sequence(u8 *obj))
