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
    void *callback;
    u32 bank;
    u8 *draw;
    u32 value;
    vu32 *dma;
    u32 win;
    u8 *world;
    callback = *(void **)(gWorld + 0x260);
    ((void (*)(void *, u32, u32, u32))callback)(callback, 0x260, arg2, arg3);
    dma = (vu32 *)REG_ADDR_DMA0SAD;
    ((vu16 *)dma)[5] &= 0xC5FF;
    ((vu16 *)dma)[5] &= 0x7FFF;
    ((vu16 *)dma)[5];
    world = gWorld;
    bank = ((u32)*(u8 *)(world + 0xB) << 26) >> 31;
    draw = gWorkBuf;
    dma[0] = ((((bank << 2) + bank) << 7) + 0x3EF0) + (u32)draw;
    win = REG_ADDR_WIN0H;
    dma[1] = win;
    dma[2] = 0xA6600001;
    dma[2];
    bank = ((u32)*(u8 *)(world + 0xB) << 26) >> 31;
    value = *(u32 *)(((((bank << 2) + bank) << 7) + 0x3EF0) + (u32)draw);
    *(vu32 *)win = value;
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

ASM_FUNC("asm/code_080E2024/sub_080E2DE8.s", void sub_080E2DE8(void))

void sub_080E3294(struct OwnerObj *obj, u32 flags)
{
    heap_free(obj->unk4);
    heap_free(obj->unkC);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_080E2024/spin_cell_grid.s", void spin_cell_grid(void))

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
    u8 *base;
    u32 off;

    shadow = &gUnk_0200000C;
    *shadow = 0x1E04;
    shadow += 6;
    *shadow = 0;
    shadow = (u16 *)((u32)shadow + 2);
    *shadow = 8;
    for (i = 0; i < 0x150; i++)
    {
        off = i * 4;
        base = gWorkBuf + 0x5470;
        *(u32 *)(base + off) = gTileBlock5x2Table[i] + 0x80008000;
    }
    dma_copy_or_queue(gWorkBuf + 0x5470, (void *)0x0600F000, 0x540, 0x20, 0);
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gPaletteGrayPink, gUnk_02000180, 0x20);
    sys = gSys;
    *(u16 *)(sys + 2) = 0xFFFF;
    *(s32 *)(sys + 0x884) = -1;
}
END_NONMATCH
