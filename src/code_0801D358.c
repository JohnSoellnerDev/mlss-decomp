#include "global.h"
#include "ram_routines.h"

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

void fill_block_word(void **slot, u32 value)
{
    register u32 v asm("r3") = value;
    register u32 word asm("r1");
    u32 t;
    u32 buf;

    word = (v << 4) | v;
    t = v << 8;
    word |= t;
    t = v << 12;
    word |= t;
    t = v << 16;
    word |= t;
    t = v << 20;
    word |= t;
    t = v << 24;
    word |= t;
    t = v << 28;
    word |= t;
    buf = word;
    cpu_fast_set(&buf, *slot, 0x01001400);
}

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

void build_gradient_table(struct GradObj *obj)
{
    struct GradObj *g = obj;
    u16 *p = g->buf4;
    s32 i;

    for (i = 0; i <= 0x27F; i++)
    {
        *p = (((g->level & 0x7FFF) >> 5) + i) | 0xF000;
        p++;
    }
    dma_copy_or_queue(g->buf4, (void *)(0x06000000 + g->vramOff), 0x500, 0x20, 0);
}

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

struct Bits12
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 hi2 : 2;
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
    ((struct Bits12 *)&s->flags12)->bit3 = 0;
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
                    ((struct Bits12 *)&s->flags12)->bit3 = dir;
                }
            }
            else
            {
                cell++;
                if (cell >= count)
                {
                    cell = 0;
                    travelled = 0;
                    ((struct Bits12 *)&s->flags12)->bit3 = dir;
                }
            }
            idx = cell * 2 + 1;
            frame = table[idx];
            remaining += table[idx + 1] << 4;
        } while (remaining <= 0);
    }
    store = 1;
    flags = s->flags12;
    if (((struct Bits12 *)&s->flags12)->bit3)
    {
        if (((struct Bits12 *)&s->flags12)->mode == 1)
        {
            ((struct Bits12 *)&s->flags12)->bit4 = 1;
            cell = count - 1;
            frame = table[cell * 2 + 1];
            remaining = 1;
            travelled = 0;
        }
        else if (((struct Bits12 *)&s->flags12)->mode == 2)
        {
            ((struct Bits12 *)&s->flags12)->bit5 = 1;
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

struct GfxObj
{
    u8 filler0[4];
    s16 h04;
    s16 h06;
    s16 h08;
    s16 h0A;
    u16 h0C;
    u8 filler0E[2];
    u8 b10;
    u8 b11;
    u8 b12;
    u8 b13;
    u16 h14;
    u8 filler16[0xC];
    u8 b22;
    u8 filler23[3];
    u8 b26;
    u8 filler27;
    u8 b28;
    u8 b29;
    u8 filler2A[4];
    u16 h2E;
    u16 h30;
    u8 filler32[2];
    u32 w34;
    u8 filler38[8];
    u8 *p40;
    u8 *p44;
    u32 *p48;
    u8 *p4C;
    u8 *p50;
    u8 *p54;
    u8 *p58;
};

typedef u32 (*GfxCmdFn)(u32 value, struct GfxObj *obj, u8 **cur, s32 arg, u8 *endp, s32 *slot, s32 k, u32 ctr);
typedef s32 (*MathDivFn)(s32 num, s32 den);

extern u8 gSys[];
extern u8 gUnk_02000280[];
extern u32 gUnk_03000D58[];

void cpu_set(const void *src, void *dst, u32 ctl);
void table_register(void *p);
void obj_affine_set(void *params, void *dst, u32 count, u32 stride);
u32 read_varlen_plus1(const void *src);

NONMATCH("asm/code_0801D358/obj_gfx_step.s", u32 obj_gfx_step(struct GfxObj *obj, u32 value, u32 ctr))
{
    u8 *cur;
    s32 slotw;
    s32 k;
    u8 *endp;
    s32 r3;
    s32 i;
    u32 j;
    s32 n;
    s32 bit;
    u32 mask;
    u8 *oam;
    u8 *oamBase;
    u32 aA[2];
    u32 aB[2];

    if (obj->p50 != 0)
    {
        if (*obj->p48 == 1)
        {
            s32 len = ((u32)*(u16 *)&obj->b28 << 23) >> 18;
            s32 d;
            s32 n;

            n = read_varlen_plus1(obj->p50);
            d = len - n;
            if (d > 0)
            {
                u32 zero = 0;

                cpu_fast_set(&zero, obj->p4C + n, (((u32)d << 9) >> 11) | 0x01000000);
            }
            *(u16 *)&obj->b28 = (*(u16 *)&obj->b28 & -0x200) | ((n / 0x20) & 0x1FF);
            table_register(obj->p48);
        }
        else if (*obj->p48 == 0)
        {
            obj->p50 = 0;
            obj->b12 = (obj->b12 & 0x3F) | ((((gSys[0x888] >> 5) & 1) + 1) << 6);
        }
    }

    if (obj->p48 != 0 && *obj->p48 != 0)
    {
        return value;
    }
    if (obj->p44 != 0)
    {
        if (*obj->p44 != 1)
        {
            return value;
        }
        obj->h30 = 0;
    }
    else
    {
        u32 m = obj->b12 & 0xC0;

        if (m != 0)
        {
            if (m == 0x40)
            {
                s32 len2;

                if (obj->b10 & 1)
                {
                    len2 = obj->h30;
                }
                else
                {
                    len2 = ((u32)*(u16 *)&obj->b28 << 23) >> 18;
                }
                dma_copy_or_queue(obj->p4C, (void *)(0x06010000 + obj->h2E), len2, 0x20, 0);
            }
            obj->b12 = (obj->b12 & 0x3F) | ((((obj->b12 >> 6) - 1)) << 6);
        }
    }

    if (obj->b12 & 0x20)
    {
        return value;
    }

    {
        s32 mode = obj->b13 & 3;

        if (mode != 0)
        {
            if (mode == 1)
            {
                mask = obj->h14;
                bit = 0;
                for (j = 0; j < (obj->b13 >> 4); j++)
                {
                    while (((mask ^ 1) & 1) != 0)
                    {
                        bit++;
                        mask >>= 1;
                    }
                    cpu_set(obj->p54 + (j << 5), gUnk_02000280 + (bit << 5), 0x04000008);
                    *(u16 *)gSys |= 1 << bit;
                    bit++;
                    mask >>= 1;
                }
            }
            obj->b13 = (obj->b13 & ~3) | (((obj->b13 & 3) - 1) & 3);
        }
    }

    k = (obj->b10 >> 1) & 0x1F;
    *(u16 *)&obj->b10 &= 0xFC3F;
    if (obj->p44 != 0)
    {
        endp = obj->p44 + (obj->b22 * obj->b26 + 1);
    }
    else
    {
        endp = 0;
    }

    cur = obj->p40;
    r3 = *cur++;
    if (r3 & 0x80)
    {
        r3 &= 0x7F;
        if (obj->b29 & 8)
        {
            cur += r3 * 6;
            k += r3;
            *(u16 *)&obj->b10 = (*(u16 *)&obj->b10 & 0xFC3F) | (((((*(u16 *)&obj->b10) >> 6) & 0xF) + r3) & 0xF) << 6;
        }
        else
        {
            oamBase = gSys + 0x86 + ((((gSys[0x888] >> 3) & 1)) << 10);
            oam = oamBase + (k << 5);
            if (r3 != 0)
            {
                n = r3;
                do
                {
                    u16 q;
                    s32 a;
                    u16 c;

                    a = (s16)(cur[2] | (cur[3] << 8));
                    q = MATH_ROUTINE(MathDivFn, iwram_div)(0x01000000, a * obj->h04);
                    if (obj->b11 & 0x40)
                    {
                        q = -q;
                    }
                    aA[0] = (aA[0] & 0xFFFF0000) | q;
                    a = (s16)(cur[4] | (cur[5] << 8));
                    q = MATH_ROUTINE(MathDivFn, iwram_div)(0x01000000, a * obj->h06);
                    if (obj->b11 & 0x80)
                    {
                        q = -q;
                    }
                    aA[0] = (aA[0] & 0xFFFF) | ((u32)q << 16);
                    c = cur[0] | (cur[1] << 8);
                    if (obj->b11 & 0x40)
                    {
                        c = -c;
                    }
                    if (obj->b11 & 0x80)
                    {
                        c = -c;
                    }
                    aA[1] = (aA[1] & 0xFFFF0000) | (u16)(obj->h0C - (s16)c);
                    obj_affine_set(aA, oam, 1, 8);
                    if (obj->h08 != 256)
                    {
                        s16 *ov = (s16 *)oam;

                        ov[0] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[0] << 8, obj->h08);
                        ov[8] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[8] << 8, obj->h08);
                    }
                    if (obj->h0A != 256)
                    {
                        s16 *ov = (s16 *)oam;

                        ov[4] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[4] << 8, obj->h0A);
                        ov[12] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[12] << 8, obj->h0A);
                    }
                    oam += 0x20;
                    k++;
                    *(u16 *)&obj->b10 = (*(u16 *)&obj->b10 & 0xFC3F) | (((((*(u16 *)&obj->b10) >> 6) & 0xF) + 1) & 0xF) << 6;
                    cur += 6;
                } while (--n != 0);
            }
        }
        r3 = *cur++;
    }

    {
        u8 op = *cur++;

        obj->w34 = value;
        slotw = -1;
        value = ((GfxCmdFn)gUnk_03000D58[op])(value, obj, &cur, r3, endp, &slotw, k, ctr);
    }

    if ((obj->b29 & 8) == 0)
    {
        oamBase = gSys + 0x86 + ((((gSys[0x888] >> 3) & 1)) << 10);
        for (i = 0; i < 4; i++)
        {
            s32 v = ((s8 *)&slotw)[i];
            u16 pa;
            u16 pd;

            if (v == -1)
            {
                continue;
            }
            oam = oamBase + (v << 5);
            pa = *(u16 *)&obj->h04;
            if (obj->h04 != 256)
            {
                pa = MATH_ROUTINE(MathDivFn, iwram_div)(0x10000, obj->h04);
            }
            if (i & 1)
            {
                pa = -pa;
            }
            aB[0] = (aB[0] & 0xFFFF0000) | pa;
            pd = *(u16 *)&obj->h06;
            if (obj->h06 != 256)
            {
                pd = MATH_ROUTINE(MathDivFn, iwram_div)(0x10000, obj->h06);
            }
            if (i & 2)
            {
                pd = -pd;
            }
            aB[0] = (aB[0] & 0xFFFF) | ((u32)pd << 16);
            aB[1] = (aB[1] & 0xFFFF0000) | obj->h0C;
            obj_affine_set(aB, oam, 1, 8);
            if (obj->h08 != 256)
            {
                s16 *ov = (s16 *)oam;

                ov[0] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[0] << 8, obj->h08);
                ov[8] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[8] << 8, obj->h08);
            }
            if (obj->h0A != 256)
            {
                s16 *ov = (s16 *)oam;

                ov[4] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[4] << 8, obj->h0A);
                ov[12] = MATH_ROUTINE(MathDivFn, iwram_div)(ov[12] << 8, obj->h0A);
            }
        }
    }

    if (endp == 0)
    {
        return value;
    }
    {
        u8 *p5;
        s32 flip;
        s32 hp;
        u32 acc = 0;
        s32 cnt;

        p5 = (gSys[0x888] & 0x20) ? obj->p58 : cur;
        flip = (obj->b29 >> 2) & 1;
        if (p5 != 0)
        {
            cnt = *p5;
            p5++;
            hp = obj->h2E;
            if (cnt > 0)
            {
                if (cnt & 1)
                {
                    s32 s1 = p5[0] | ((p5[1] & 3) << 8);
                    s32 len = p5[2] << 5;

                    if (flip == 0)
                    {
                        dma_copy_or_queue(obj->p4C + (s1 << 5), (void *)(0x06010000 + hp), len, 0x20, acc);
                    }
                    hp += len;
                    acc = len;
                    p5 += 3;
                    n = cnt - 1;
                }
                else
                {
                    n = cnt;
                }
                while (n != 0)
                {
                    s32 s1 = p5[0] | ((p5[1] & 3) << 8);
                    s32 len = p5[2] << 5;

                    if (flip == 0)
                    {
                        dma_copy_or_queue(obj->p4C + (s1 << 5), (void *)(0x06010000 + hp), len, 0x20, 0);
                    }
                    hp += len;
                    acc += len;
                    s1 = p5[3] | ((p5[4] & 3) << 8);
                    len = p5[5] << 5;
                    if (flip == 0)
                    {
                        dma_copy_or_queue(obj->p4C + (s1 << 5), (void *)(0x06010000 + hp), len, 0x20, 0);
                    }
                    hp += len;
                    acc += len;
                    p5 += 6;
                    n -= 2;
                }
            }
        }
        obj->h30 = acc;
        obj->p58 = (gSys[0x888] & 0x20) ? cur : 0;
    }
    return value;
}
END_NONMATCH
