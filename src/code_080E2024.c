#include "global.h"
#include "ram_routines.h"

struct Rec
{
    u16 x;
    u16 y;
    u16 w4;
    u16 h6;
    u8 unk8;
    u8 filler9[8];
    u8 flags11;
    u8 flags12;
    u8 filler13[0x78 - 0x13];
    u16 count78;
    u16 extra7A;
    u8 filler7C[2];
    u8 flags7E;
};

struct Bits7E
{
    u8 lo3 : 3;
    u8 mid3 : 3;
    u8 hi2 : 2;
};

struct StreamObj
{
    u8 filler0[4];
    void (*state)();
    struct Rec *rec;
    u16 *stream;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 filler18[0x5C - 0x18];
    u32 unk5C;
    u32 unk60;
};

struct PosObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    s32 z;
    s16 vx;
    s16 vy;
    s16 vz;
    u16 bounce;
};

struct FallParams
{
    u8 filler0[4];
    u16 gravity;
    s16 limit;
};

struct ScrollObj
{
    u8 filler0[0x10];
    u16 posX;
    u16 speedX;
    u16 speedY;
    u8 filler16[2];
    u16 posY;
};

struct OwnerObj
{
    u8 filler0[4];
    void *unk4;
    void *unk8;
    void *unkC;
};

struct CellObj
{
    u8 filler0[0x18];
    const u8 *vtable;
    u32 *unk1C;
    u16 unk20;
    u16 unk22;
    u8 unk24;
    u8 unk25;
    u16 unk26;
    u16 unk28;
};

extern const u8 gUnk_083910F0[];
extern const u16 gDeltaTable[];
extern const u8 gCellVtable[];
extern u16 gUnk_02000018;
extern u16 gUnk_0200001A;
extern u16 gUnk_0200000C;
extern u8 *gWorld;
extern u8 *gWorkBuf;
extern u8 gSys[];
extern u8 gPaletteGrayPink[];
extern u8 gUnk_02000180[];
extern const u32 gTileBlock5x2Table[];
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
extern void fx_spawn_list_step();
extern void fx_script_step();
void run_command_stream(struct StreamObj *obj);
void display_pause(void);
void heap_free(void *p);
void heap_free_b(void *p);
void task_remove(void *node, u32 kind);
void task_insert(void *node, u8 kind, u32 data);
void sprite_entry_update(u32 handle);
u32 spawn_obj_by_id(u16 id, s16 x, u32 c, u32 d);
void obj_release_node(struct Rec *rec);
void write_entry_offsets_camera(struct Rec *rec, s16 a, s16 b, s16 d, u32 e);
void sprite_set_action(void *obj, u32 a, u32 b, u32 c);
void effect_spawn_at(u16 cmd, s16 b, s16 c, s16 d, void *owner);
void release_or_update_rec(struct StreamObj *obj);

NONMATCH("asm/code_080E2024/fall_and_bounce.s", void fall_and_bounce(struct PosObj *obj, struct FallParams *params))
{
    s32 y;
    u16 vy;
    s32 limit;
    s32 reversed;

    obj->x = obj->x + obj->vx;
    y = obj->y + obj->vy;
    obj->y = y;
    obj->z = obj->z + obj->vz;
    vy = params->gravity + (u16)obj->vy;
    obj->vy = vy;
    if (y < 0)
    {
        return;
    }
    limit = params->limit;
    if ((s16)vy < params->limit)
    {
        obj->bounce = 0;
        return;
    }
    reversed = -obj->vy;
    reversed = reversed * 3;
    if (reversed < 0)
    {
        reversed += 3;
    }
    reversed >>= 2;
    obj->vy = limit + reversed;
    obj->y = 0;
}
END_NONMATCH

NONMATCH("asm/code_080E2024/advance_z.s", void advance_z(struct PosObj *obj, struct FallParams *params))
{
    s32 z;
    s32 vz;
    vz = obj->vz;
    z = obj->z;
    z += vz;
    obj->z = z;
    if (vz < 0)
    {
        if (z >= -32768)
        {
            return;
        }
    }
    else if ((params->limit << 8) >= z)
    {
        return;
    }
    obj->bounce = 0;
}
END_NONMATCH

void move_record_by_table(struct StreamObj *obj, u32 step, u32 shape)
{
    struct Rec *rec = obj->rec;
    if (rec == 0)
    {
        return;
    }
    if (step != 0)
    {
        const u16 *delta = &gDeltaTable[step];
        if (rec->flags11 & 0x40)
        {
            rec->x = rec->x - delta[0];
        }
        else
        {
            rec->x = rec->x + delta[0];
        }
        rec->y = rec->y + delta[1];
    }
    if (shape != 0)
    {
        const u16 *pair = &gDeltaTable[shape];
        rec->w4 = pair[0];
        rec->h6 = pair[1];
    }
}

void start_obj_from_stream(struct StreamObj *obj)
{
    u32 handle = spawn_obj_by_id(obj->stream[0], (s16)obj->stream[1], 2, 1);
    sprite_entry_update(handle);
    obj->rec = (struct Rec *)handle;
    obj->state = release_or_update_rec;
    release_or_update_rec(obj);
}

void start_stream_state_fe29(struct StreamObj *obj)
{
    u16 *stream = obj->stream;
    obj->unk10 = *stream;
    obj->stream = stream + 1;
    obj->state = fx_spawn_list_step;
    obj->state(obj);
}

void start_stream_state_232d(struct StreamObj *obj)
{
    u16 *stream = obj->stream;
    obj->unk10 = *stream;
    obj->stream = stream + 1;
    obj->state = run_command_stream;
    obj->state(obj);
}

void start_stream_state_faed(struct StreamObj *obj)
{
    u16 *stream = obj->stream;
    obj->unk10 = *stream;
    obj->stream = stream + 1;
    obj->state = fx_script_step;
    obj->state(obj);
}

void flush_pending_count(struct StreamObj *obj, struct Rec *rec)
{
    u32 count = rec->count78;
    u32 extra = rec->extra7A;
    rec->count78 = 0;
    rec->extra7A = 0;
    obj->unk5C = 0;
    obj->unk60 = 0;
    if (count != 0 && (rec->flags7E & 0x38) != 0)
    {
        sprite_set_action(&rec->unk8, extra, count, 0);
        ((struct Bits7E *)&rec->flags7E)->mid3 = 0;
    }
}

void count_down_then_stop(struct StreamObj *obj)
{
    u16 timer = obj->unk10;
    if ((s16)obj->unk10 > 0)
    {
        obj->unk10 = timer - 1;
    }
    else
    {
        display_pause();
        obj->state = 0;
    }
}

u32 load_table_value(s32 *out, u32 index)
{
    const u8 *table = gUnk_083910F0;
    u32 offset = index << 1;
    *out = *(s16 *)(table + offset);
    table += 2;
    return offset + (u32)table;
}

void update_scroll_shadows(struct ScrollObj *obj)
{
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;
    obj->posX = obj->posX + obj->speedX;
    obj->posY = obj->posY + obj->speedY;
    x = (s16)obj->posX;
    if (x < 0)
    {
        x += 0xFF;
    }
    sx = x >> 8;
    gUnk_02000018 = sx;
    y = (s16)obj->posY;
    if (y < 0)
    {
        y += 0xFF;
    }
    sy = y >> 8;
    gUnk_0200001A = sy;
}

NONMATCH("asm/code_080E2024/dma_win0_from_buffer.s", void dma_win0_from_buffer(u32 arg0, u32 arg1, u32 arg2, u32 arg3))
{
    u32 callback;
    u32 bank;
    u32 win;
    vu32 *dma;

    callback = *(u32 *)(gWorld + 0x260);
    ((void (*)(u32, u32, u32, u32))callback)(callback, 0x260, arg2, arg3);
    dma = (vu32 *)0x040000B0;
    ((vu16 *)dma)[5] &= 0xC5FF;
    ((vu16 *)dma)[5] &= 0x7FFF;
    ((vu16 *)dma)[5];
    bank = ((u32)*(u8 *)(gWorld + 0xB) << 26) >> 31;
    dma[0] = ((((bank << 2) + bank) << 7) + 0x3EF0) + (u32)gWorkBuf;
    win = 0x04000040;
    dma[1] = win;
    dma[2] = 0xA6600001;
    dma[2];
    bank = ((u32)*(u8 *)(gWorld + 0xB) << 26) >> 31;
    *(vu32 *)win = *(u32 *)((((((bank << 2) + bank) << 7) + (u32)gWorkBuf)) + 0x3EF0);
}
END_NONMATCH

void release_or_update_rec(struct StreamObj *obj)
{
    struct Rec *rec = obj->rec;
    u32 flags = rec->flags12 & 8;
    if (flags == 0)
    {
        write_entry_offsets_camera(rec, obj->unk12, obj->unk14, obj->unk16, flags);
    }
    else
    {
        obj_release_node(rec);
        obj->state = 0;
    }
}

void run_command_stream(struct StreamObj *obj)
{
    if ((s16)obj->unk10 == 0)
    {
        u16 *stream;
        do
        {
            stream = obj->stream;
            effect_spawn_at(stream[0], obj->unk12, obj->unk14, obj->unk16, obj->rec);
            obj->unk10 = stream[1];
            obj->stream = stream + 2;
        } while (obj->unk10 == 0);
    }
    {
        u16 timer = obj->unk10;
        if ((s16)obj->unk10 < 0)
        {
            obj->state = 0;
        }
        else
        {
            obj->unk10 = timer - 1;
        }
    }
}

ASM_FUNC("asm/code_080E2024/unknown_thumb_080e237c.s", void unknown_thumb_080e237c(void))

ASM_FUNC("asm/code_080E2024/sub_080E2598.s", void sub_080E2598(void))

void sub_080E2BB8(struct OwnerObj *obj, u32 flags)
{
    heap_free(obj->unk4);
    heap_free(obj->unk8);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_080E2024/sub_080E2BE0.s", void sub_080E2BE0(void))

struct ModelEntryA
{
    s32 e0;
    s32 e1;
    s32 e2;
    s32 *src;
};

struct ModelEntryB
{
    s32 *p0;
    s32 *p1;
    s32 *p2;
    u16 tile;
    u16 pad;
};

struct SpinModel
{
    u16 countA;
    u16 countB;
    struct ModelEntryA *tableA;
    u8 b8;
    u8 b9;
    u8 filler0A[2];
    struct ModelEntryB *tableB;
    s32 k16;
    s32 k14;
    s32 k18;
    s32 scale1c;
    s32 scale20;
    s32 off24;
    s32 off28;
    s32 off2c;
};

struct OamOut
{
    u16 a0;
    u16 a1;
    u16 a2;
    u16 a6;
    u32 a8;
    u8 filler0C[4];
    struct OamOut *next;
};

extern u8 *gModelCtx;
extern u8 gUnk_030003CC[];
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);

NONMATCH("asm/code_080E2024/sub_080E2DE8.s", void sub_080E2DE8(struct SpinModel *arg, u32 *arg1))
{
    struct ModelEntryA *ea;
    struct ModelEntryB *eb;
    struct OamOut *out;
    vu16 *cell16;
    u8 *cell;
    s32 pos[3];
    s32 i;
    s32 k;
    s32 e0;
    s32 e1;
    s32 e2;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 a1;
    s32 b1;
    s32 a2;
    s32 b2;
    s32 det;
    s32 sp20;
    s32 sp24;
    s32 r7;
    s32 r6;
    s32 r4;
    s32 r2;
    u32 base;

    out = *(struct OamOut **)(gModelCtx + 0x38);
    for (i = 0; i < arg->countA; i++)
    {
        ea = &arg->tableA[i];
        ea->e0 = ea->src[0];
        ea->e1 = ea->src[1];
        ea->e2 = ea->src[2];
        k = (u8)(arg->k14 / 0x100);
        if (k != 0)
        {
            e0 = ea->e0;
            e2 = ea->e2;
            ea->e2 = (e2 * (gCosineTable[k] / 0x40) - (gSineTable[k] / 0x40) * e0) / 0x100;
            ea->e0 = (e0 * (gCosineTable[k] / 0x40) + e2 * (gSineTable[k] / 0x40)) / 0x100;
        }
        k = (u8)(arg->k16 / 0x100);
        if (k != 0)
        {
            e1 = ea->e1;
            e2 = ea->e2;
            ea->e1 = (e1 * (gCosineTable[k] / 0x40) - e2 * (gSineTable[k] / 0x40)) / 0x100;
            ea->e2 = (e2 * (gCosineTable[k] / 0x40) + e1 * (gSineTable[k] / 0x40)) / 0x100;
        }
        k = (u8)(arg->k18 / 0x100);
        if (k != 0)
        {
            e0 = ea->e0;
            e1 = ea->e1;
            ea->e0 = (e0 * (gCosineTable[k] / 0x40) - e1 * (gSineTable[k] / 0x40)) / 0x100;
            ea->e1 = (e1 * (gCosineTable[k] / 0x40) + e0 * (gSineTable[k] / 0x40)) / 0x100;
        }
        ea->e0 = ea->e0 * arg->scale1c / 0x100;
        ea->e1 = ea->e1 * arg->scale1c / 0x100;
        ea->e2 = ea->e2 * arg->scale20 / 0x100;
    }

    base = *arg1;
    sp20 = base + arg->b9;
    sp24 = (arg->b8 << 12) + 0x800;
    for (i = 0; i < arg->countB; i++)
    {
        eb = &arg->tableB[i];
        x0 = eb->p0[0];
        y0 = -eb->p0[1];
        x1 = eb->p1[0];
        y1 = -eb->p1[1];
        x2 = eb->p2[0];
        y2 = -eb->p2[1];
        a1 = (((x1 - x0) * 31) << 3) / 0x100;
        b1 = (((y1 - y0) * 31) << 3) / 0x100;
        a2 = (((x2 - x0) * 31) << 3) / 0x100;
        b2 = (((y2 - y0) * 31) << 3) / 0x100;
        det = (b2 * a1 - b1 * a2) / 0x100;
        if (det > 0)
        {
            pos[0] = ((x1 + x2 + 0x100) / 0x200) - 0x10;
            pos[1] = ((y1 + y2 + 0x100) / 0x200) - 0x10;
            pos[0] += (arg->off24 + 0x80) / 0x100;
            pos[1] += (arg->off28 + 0x80) / 0x100;
            pos[2] = (arg->off2c + 0x80) / 0x100;
            offset_three_words_camera(&pos[0], &pos[1], &pos[2], 0);
            if (pos[0] >= -0x10 && pos[1] >= -0x10 && pos[0] <= 0x100 && pos[1] <= 0xB0)
            {
                r7 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(b2 << 12, det);
                if (r7 <= 0x7FFF && r7 >= -0x7FFF)
                {
                    r6 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((-a2) << 12, det);
                    if (r6 <= 0x7FFF && r6 >= -0x7FFF)
                    {
                        r4 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((-b1) << 12, det);
                        if (r4 <= 0x7FFF && r4 >= -0x7FFF)
                        {
                            r2 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a1 << 12, det);
                            if (r2 <= 0x7FFF && r2 >= -0x7FFF)
                            {
                                cell = gUnk_030003CC + (sp20 << 5) + ((((u32)gSys[0x888] << 0x1C) >> 0x1F) << 10);
                                cell16 = (vu16 *)cell;
                                cell16[3];
                                cell16[3] = r7;
                                cell16[7];
                                cell16[7] = r6;
                                cell16[11];
                                cell16[11] = r4;
                                cell16[15];
                                cell16[15] = r2;
                                out->a0 = (pos[1] & 0xFF) | 0x300;
                                out->a1 = (pos[0] & 0x1FF) | 0x4000 | (sp20 << 9);
                                sp20++;
                                out->a2 = sp24 | eb->tile;
                                out->a8 = (pos[2] << 14) | 0x80000000;
                                out->a6 = 0;
                                out = out->next;
                            }
                        }
                    }
                }
            }
        }
    }
    *arg1 = base;
    *(u16 *)gSys |= 1 << arg->b8;
    *(struct OamOut **)(gModelCtx + 0x38) = out;
}
END_NONMATCH

void sub_080E3294(struct OwnerObj *obj, u32 flags)
{
    heap_free(obj->unk4);
    heap_free(obj->unkC);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

NONMATCH("asm/code_080E2024/spin_cell_grid.s", void spin_cell_grid(struct CellObj *obj))
{
    u8 *cell;
    vu16 *cell16;
    u16 *dst;
    u32 attr2;
    s32 frame;
    s32 base;
    s32 palette;
    s32 cols;
    s32 rows;
    s32 speed;
    s32 step;
    u8 grid;
    s32 cellW;
    s32 cellH;
    s32 mask;
    s32 k;
    s32 t;
    s32 c;
    s32 p;
    s32 q;
    s32 a;
    s32 b;
    s32 cc;
    s32 d;
    s32 det;
    s32 bit3;
    s32 yacc;
    s32 yattr;
    s32 x;
    s32 xStart;
    s32 idx;
    s32 i;
    s32 j;

    frame = obj->unk20;
    base = obj->unk22;
    grid = obj->unk24;
    palette = ((u32)grid << 28) >> 28;
    cols = grid >> 4;
    rows = ((u32)obj->unk25 << 28) >> 28;
    speed = (s16)obj->unk26;
    step = (s16)obj->unk28;
    cellW = (cols + 0xEF) / cols;
    cellH = (rows + 0x9F) / rows;
    mask = 0;
    if (speed != 0)
    {
        obj->unk20 = frame + 1;
    }
    for (k = 0x21 - (cols + rows); k <= 0x1F; k++)
    {
        t = frame * speed + k * step - (step << 5);
        if (t <= 0)
        {
            bit3 = ((u32)gSys[0x888] << 0x1C) >> 0x1F;
            cell = gSys + 0x80 + (k << 5) + (bit3 << 10);
            cell16 = (vu16 *)cell;
            a = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x2000, cellW);
            cell16[3];
            cell16[3] = a;
            cell16[7];
            cell16[7] = 0;
            cell16[11];
            cell16[11] = 0;
            a = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x2000, cellH);
            cell16[15];
            cell16[15] = a;
            mask |= 1 << k;
        }
        else if (t <= 0x3F)
        {
            c = gCosineTable[t & 0xFF] / 0x40;
            p = c + 0x100;
            a = (cellW * p) / 2;
            q = c - 0x100;
            cc = (cellH * q) / 2;
            d = (cellW * q) / 2;
            b = (cellH * p) / 2;
            det = (a * b - cc * d) / 0x100;
            a = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((b * 31) << 8, det);
            b = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((d - (d << 5)) << 8, det);
            cc = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((cc - (cc << 5)) << 8, det);
            d = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((a << 5) - a) << 8, det);
            bit3 = ((u32)gSys[0x888] << 0x1C) >> 0x1F;
            cell = gSys + 0x80 + (k << 5) + (bit3 << 10);
            cell16 = (vu16 *)cell;
            cell16[3];
            cell16[3] = a;
            cell16[7];
            cell16[7] = b;
            cell16[11];
            cell16[11] = cc;
            cell16[15];
            cell16[15] = d;
            mask |= 1 << k;
        }
    }
    if (mask != 0)
    {
        dst = *(u16 **)(gSys + 0x880);
        attr2 = palette << 12;
        attr2 |= base;
        yacc = (cellH - 0x20) / 2;
        xStart = (cellW - 0x20) / 2;
        for (i = 0; i < rows; i++)
        {
            yacc &= 0xFF;
            x = xStart;
            if (cols != 0)
            {
                yattr = yacc | 0x100;
                idx = 31 - i;
                for (j = 0; j < cols; j++)
                {
                    if (mask & (1 << idx))
                    {
                        x &= 0x1FF;
                        dst[0] = yattr;
                        dst[1] = x | 0xFFFF8000 | (idx << 9);
                        dst[2] = *(u16 *)&attr2;
                        dst += 4;
                    }
                    x += cellW;
                    idx--;
                }
            }
            yacc += cellH;
        }
        *(u16 **)(gSys + 0x880) = dst;
    }
    else if (obj != 0)
    {
        obj->vtable = gCellVtable;
        if (obj->unk1C != 0)
        {
            *obj->unk1C = mask;
        }
        task_remove(obj, 3);
    }
}
END_NONMATCH

void reset_obj_hook(struct CellObj *obj, u32 kind)
{
    obj->vtable = gCellVtable;
    if (obj->unk1C != 0)
    {
        *obj->unk1C = 0;
    }
    task_remove(obj, kind);
}

struct CellObj *unknown_thumb_080e3738(struct CellObj *obj, u32 kind, u32 data, u32 a4, u32 a5)
{
    u8 *p;
    task_insert(obj, kind, data);
    obj->vtable = gCellVtable;
    obj->unk20 = 0;
    obj->unk1C = 0;
    obj->unk22 = a4;
    p = &obj->unk24;
    *p = (a5 & 0xF) | 0xC0;
    p++;
    *p = 5;
    obj->unk26 = 3;
    obj->unk28 = 6;
    return obj;
}

NONMATCH("asm/code_080E2024/sub_080E377C.s", void sub_080E377C(void))
{
    u16 *shadow;
    s32 i;
    u8 *sys;
    u16 ff;
    u8 *root;
    u8 *base;
    u32 off;

    shadow = &gUnk_0200000C;
    *shadow = 0x1E04;
    shadow += 6;
    *shadow = 0;
    shadow++;
    *shadow = 8;
    for (i = 0; i < 0x150; i++)
    {
        root = gWorkBuf;
        off = i * 4;
        base = root + 0x5470;
        *(u32 *)(base + off) = gTileBlock5x2Table[i] + 0x80008000;
    }
    dma_copy_or_queue(gWorkBuf + 0x5470, (void *)0x0600F000, 0x540, 0x20, 0);
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gPaletteGrayPink, gUnk_02000180, 0x20);
    sys = gSys;
    ff = 0xFFFF;
    *(u16 *)(sys + 2) = ff;
    *(s32 *)(sys + 0x884) = -1;
}
END_NONMATCH
