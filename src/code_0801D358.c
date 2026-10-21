#include "global.h"

struct HookObj14
{
    s32 x;
    s32 y;
    u8 filler8[0xC];
    void (*hook)(s32, s32, s32);
};

ASM_FUNC("asm/code_0801D358/sub_0801D358.s", void sub_0801D358(void))

ASM_FUNC("asm/code_0801D358/draw_edge.s", void draw_edge(void *obj, s16 x0, s16 y0, s16 x1, s16 y1))

ASM_FUNC("asm/code_0801D358/fill_span_rows.s", void fill_span_rows(void *obj, u8 color))

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void dma_cancel_request(void *dst);
void heap_free_a(void *p);
void heap_free_b(void *p);

struct DmaObj
{
    u8 *buf0;
    u8 *buf4;
    u8 *buf8;
    u16 vramOff;
    u8 filler0E[6];
    u8 *buf14;
    u8 *buf18;
};

struct GradObj
{
    u8 filler0[4];
    u16 *buf4;
    u8 filler8[4];
    u16 level;
    u16 vramOff;
};

void anim_dma_upload(struct DmaObj *obj)
{
    REG_IME = 0;
    dma_copy_or_queue(obj->buf4, (void *)(0x06000000 + obj->vramOff), 0x1400, 0x20, 0);
    REG_IME = 1;
    REG_IME = 0;
    dma_copy_or_queue(obj->buf4 + 0x1400, (void *)(0x06001400 + obj->vramOff), 0x1400, 0x20, 0);
    REG_IME = 1;
    REG_IME = 0;
    dma_copy_or_queue(obj->buf4 + 0x2800, (void *)(0x06002800 + obj->vramOff), 0x1400, 0x20, 0);
    REG_IME = 1;
    REG_IME = 0;
    dma_copy_or_queue(obj->buf4 + 0x3C00, (void *)(0x06003C00 + obj->vramOff), 0x1400, 0x20, 0);
    REG_IME = 1;
}

void cpu_fast_set(const void *src, void *dst, u32 ctl);

NONMATCH("asm/code_0801D358/fill_block_word.s", void fill_block_word(void **slot, u32 value))
{
    u32 word;
    u32 v = value;

    word = (v << 4) | v;
    word |= v << 8;
    word |= v << 12;
    word |= v << 16;
    word |= v << 20;
    word |= v << 24;
    word |= v << 28;
    cpu_fast_set(&word, *slot, 0x01001400);
}
END_NONMATCH

void obj_call_hook_14(struct HookObj14 *obj)
{
    s32 x = obj->x;
    s32 y = obj->y;
    void (*hook)(s32, s32, s32) = obj->hook;
    hook(x, y, x);
}

ASM_FUNC("asm/code_0801D358/draw_quad_face.s", void draw_quad_face(void))

void draw_edge(void *obj, s16 x0, s16 y0, s16 x1, s16 y1);
void fill_span_rows(void *obj, u8 color);

ASM_FUNC("asm/code_0801D358/draw_triangle_checked.s", void draw_triangle_checked(void))

NONMATCH("asm/code_0801D358/build_gradient_table.s", void build_gradient_table(struct GradObj *obj))
{
    struct GradObj *g = obj;
    u16 *p = g->buf4;
    s32 i;

    for (i = 0; i <= 0x27F; i++)
    {
        s32 entry = (((g->level & 0x7FFF) >> 5) + i) | 0xFFFFF000;
        *p = entry;
        p++;
    }
    dma_copy_or_queue(g->buf4, (void *)(0x06000000 + g->vramOff), 0x500, 0x20, 0);
}
END_NONMATCH

void obj_dma_cancel_and_free(struct DmaObj *obj, u32 flags)
{
    REG_IME = 0;
    dma_cancel_request((void *)(0x06000000 + obj->vramOff));
    REG_IME = 1;
    REG_IME = 0;
    dma_cancel_request((void *)(0x06001400 + obj->vramOff));
    REG_IME = 1;
    REG_IME = 0;
    dma_cancel_request((void *)(0x06002800 + obj->vramOff));
    REG_IME = 1;
    REG_IME = 0;
    dma_cancel_request((void *)(0x06003C00 + obj->vramOff));
    REG_IME = 1;
    if (obj->buf18 != 0)
    {
        heap_free_a(obj->buf18);
    }
    if (obj->buf14 != 0)
    {
        heap_free_a(obj->buf14);
    }
    if (obj->buf8 != 0)
    {
        heap_free_a(obj->buf8);
    }
    if (obj->buf0 != 0)
    {
        heap_free_a(obj->buf0);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_0801D358/setup_sprite_buffers.s", void setup_sprite_buffers(void))

ASM_FUNC("asm/code_0801D358/obj_load_record.s", void obj_load_record(void))

ASM_FUNC("asm/code_0801D358/decode_e400.s", void decode_e400(void))

struct SpriteStepObj
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0xD];
    s8 step20;
    u8 filler21;
    u8 frame22;
    u8 cell23;
    u8 filler24;
    u8 count25;
    u8 filler26[3];
    u8 unk29;
    s16 remaining2A;
    s16 travelled2C;
    u8 filler2E[0xA];
    u8 *tab38;
    u8 *table3C;
    s32 result40;
    u8 *ptr44;
    u8 **ptr48;
    u8 filler4C[0x10];
    void (*cb5C)(void *, s32);
    s32 cbArg60;
};

NONMATCH("asm/code_0801D358/sprite_step_e6a0.s", void sprite_step_e6a0(void *spr))
{
    struct SpriteStepObj *s = spr;
    u8 *table;
    s32 count;
    s32 frame;
    s32 travelled;
    s32 dir;
    s32 remaining;
    s32 cell;
    s32 step;
    s32 store;
    s32 v;
    s32 flags;
    s32 idx;
    s32 t;
    u8 *p20;
    u8 *p22;
    u8 *p23;

    if (s->ptr48 != 0 && *(s32 *)s->ptr48 != 0)
    {
        goto tail;
    }
    if (s->ptr44 != 0 && *s->ptr44 != 1)
    {
        goto tail;
    }
    flags = s->flags12;
    if ((flags & 0x30) != 0)
    {
        goto tail;
    }
    dir = ((u32)flags << 28) >> 31;
    t = -9;
    t &= flags;
    s->flags12 = t;
    table = s->table3C;
    count = s->count25;
    travelled = s->travelled2C;
    remaining = s->remaining2A;
    cell = s->cell23;
    frame = s->frame22;
    step = s->step20;
    p23 = &s->cell23;
    p22 = &s->frame22;
    p20 = (u8 *)&s->step20;
    if (step < 0)
    {
        step = -step;
    }
    travelled += step;
    remaining -= step;
    if (remaining <= 0)
    {
        dir ^= 1;
        do
        {
            v = *(s8 *)p20;
            if (v <= 0)
            {
                cell--;
                if (cell < 0)
                {
                    cell = count - 1;
                    travelled = 0;
                    s->flags12 = (s->flags12 & -9) | (dir << 3);
                }
            }
            else
            {
                cell++;
                if (cell >= count)
                {
                    cell = 0;
                    travelled = 0;
                    s->flags12 = (s->flags12 & -9) | (dir << 3);
                }
            }
            idx = cell * 2 + 1;
            frame = table[idx];
            remaining += table[idx + 1] << 4;
        } while (remaining <= 0);
    }
    store = 1;
    flags = s->flags12;
    if ((flags & 8) != 0)
    {
        if ((flags & 6) == 2)
        {
            s->flags12 = flags | 0x10;
            cell = count - 1;
            frame = table[cell * 2 + 1];
            remaining = 1;
            travelled = 0;
        }
        else if ((flags & 6) == 4)
        {
            s->flags12 = flags | 0x20;
            store = 0;
        }
    }
    if (store != 0)
    {
        u8 *tab = s->tab38;
        s32 off = tab[0] | (tab[1] << 8);
        u8 *base = tab - off;
        u8 *ea = base + (frame << 1);
        s32 val = ea[1] | (ea[2] << 8);
        s->result40 = (s32)(base + val);
        s->travelled2C = travelled;
        s->remaining2A = remaining;
        *p23 = cell;
        *p22 = frame;
    }
tail:
    if (s->cb5C != 0)
    {
        if ((s->unk29 & 2) != 0 || (s->flags12 & 0x28) == 8)
        {
            s->cb5C(s, s->cbArg60);
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0801D358/obj_gfx_step.s", void obj_gfx_step(void))
