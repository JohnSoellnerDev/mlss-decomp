#include "global.h"
#include "ram_routines.h"

struct ScrollSlot
{
    u8 filler0[0x10];
    u16 unk10;
    s16 unk12;
    u8 filler14[2];
    u16 unk16;
};

struct SlotSource
{
    u8 filler0[0x10];
    u16 unk10;
};

struct StripObj
{
    u8 filler0[0x4C];
    u8 *dst;
};

struct ObjParams
{
    u8 filler0[8];
    u32 unk8;
    u8 filler0C[4];
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
};

struct ObjParamSrc
{
    s16 a;
    s16 b;
    s16 c;
};

struct ObjStripRec
{
    u8 filler0[4];
    u32 unk4;
    struct StripRec *rec;
};

struct StripRec
{
    s16 count;
    s16 first;
    s16 second;
};

struct StreamObj
{
    u8 filler0[4];
    void (*state)(void);
    u8 filler8[4];
    u16 *stream;
    u16 unk10;
    u16 unk12;
    u16 unk14;
};

/* One node of the array effect_node_spawn_type3 allocates (0x28 bytes). */
struct EffectNode
{
    u16 kind;       /* 0x900 for every node */
    u8 linked;      /* 1 for every node but the first */
    u8 filler03[0x10 - 0x03];
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x28 - 0x1C];
};

struct World
{
    u8 filler0[0xB];
    u8 flags0B;
    u8 filler0C;
    u8 flags;
    u8 filler0E[0x24C - 0xE];
    u32 item24C;
    u8 filler250[0x260 - 0x250];
    u32 savedIrq2;    /* IRQ 2 callback saved by enter_special_mode; nonzero while that mode is on */
    u8 filler264[0x26C - 0x264];
    u16 tile26C;
    u16 tile26E;
    u8 pointCount;    /* number of special-mode points (RecXYZ at gWorkBuf + 0x52F0) */
    u8 filler271[0x274 - 0x271];
    struct ScrollBlock *scrollBlock;
};

struct SysHead
{
    u8 filler0[2];
    u16 unk2;
};

/* A special-mode point, 8.8 fixed point. */
struct RecXYZ
{
    s32 x;
    s32 y;
    s32 z;
};

/* Offset in gWorkBuf of the special mode's RecXYZ point list (World.pointCount entries). */
#define WORKBUF_SPECIAL_POINTS 0x52F0

struct ScrollBlock
{
    u8 filler0[0x10];
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u8 filler16[2];
    u16 unk18;
};

extern struct World *gWorld;
extern const u16 gTileHalfTable[];
extern const u8 gUnk_089F46B0[];
extern u16 gUnk_02000018;
extern u16 gUnk_0200001A;
extern u16 gUnk_02000050;
extern u8 *gWorkBuf;
extern u16 gUnk_0200004A;
extern u16 gUnk_0200025C;
extern u16 gUnk_02000040;
extern u8 gSys[];

void reset_display_step(void);
void add_obj_target_offset(u32 item, s32 a, s32 b);
void update_scroll_shadows(void *task);
void *task_create_list_40(void (*entry)(void *));
void set_irq_callback(u32 kind, void *cb);
u32 mt19937_next(void);
void dma_win0_from_buffer(void);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
extern const u8 gTagYbtb[];

void advance_path(void);

void stream_next_halfword(struct StreamObj *obj)
{
    struct World *world = gWorld;
    u16 *stream;

    if (world->flags & 2)
    {
        obj->state = 0;
    }
    else
    {
        world->flags |= 2;
        stream = obj->stream;
        obj->unk12 = 0;
        obj->unk14 = 0;
        obj->unk10 = *stream++;
        obj->stream = stream;
        obj->state = (void (*)(void))advance_path;
    }
}

/* While the special mode is on, moves each of its points by (dx, dy, dz) whole pixels. */
void add_offset_to_records(s32 dx, s32 dy, s32 dz)
{
    struct World *world = gWorld;
    struct RecXYZ *points;
    s32 count;
    s32 i;

    if (world->savedIrq2 != 0)
    {
        points = (struct RecXYZ *)(gWorkBuf + WORKBUF_SPECIAL_POINTS);
        count = world->pointCount;
        for (i = 0; i < count; i++)
        {
            points[i].x += dx << 8;
            points[i].y += dy << 8;
            points[i].z += dz << 8;
        }
    }
}

/* While the special mode is on, loads its point list from a stream: a count, then x, y, z of each
   point in whole pixels, stored as 8.8 fixed point. */
void load_records_from_stream(s16 *stream)
{
    struct World *world;
    struct RecXYZ *points;
    s32 count;
    s32 i;

    world = gWorld;
    if (world->savedIrq2 != 0)
    {
        points = (struct RecXYZ *)(gWorkBuf + 0x52F0);
        count = *stream++;
        world->pointCount = count;
        for (i = 0; i < count; i++)
        {
            points[i].x = *stream++ << 8;
            points[i].y = *stream++ << 8;
            points[i].z = *stream++ << 8;
        }
    }
}

void reset_display_step(void)
{
    ((void (*)(void))gWorld->savedIrq2)();
    REG_WIN0H = 0;
    REG_WIN1H = 0;
    ((void (*)(u32, u8 *, u32))gMemRoutines)(0xE1FFE1FF, (u8 *)0x0600F000, 0x500);
    set_irq_callback(2, dma_win0_from_buffer);
}

/* Allocates an array of `count` effect nodes, all of kind 0x900 at (x, y, z); every node but the
 * first is marked linked. */
struct EffectNode *effect_node_spawn_type3(s32 count, s32 x, s32 y, s32 z)
{
    struct EffectNode *nodes;
    struct EffectNode *node;
    s32 i;

    nodes = heap_alloc_raw(1, count * sizeof(struct EffectNode), gTagYbtb);
    for (i = 0; i < count; i++)
    {
        node = &nodes[i];
        node->x = x;
        node->y = y;
        node->z = z;
        node->kind = 0x900;
        node->linked = 1;
    }
    nodes->linked = 0;
    return nodes;
}

s32 sub_080E1FA4(struct ScrollSlot *dst, s32 unused, struct SlotSource *src)
{
    dst->unk10 = 0x190;
    dst->unk12 = -src->unk10;
    dst->unk16 = 0x1F4;
    return 1;
}

s32 randomize_obj_params(struct ObjParams *p, struct ObjParamSrc *src)
{
    u32 rnd;

    p->unk8 = 0;
    rnd = mt19937_next();
    p->unk16 = (((rnd & 0xFF) + 0x100) * src->a) >> 8;
    rnd >>= 8;
    p->unk12 = (((rnd & 0xFF) + 0x100) * src->b) >> 8;
    rnd >>= 8;
    p->unk18 = (((rnd & 0xFF) + 0x100) * src->c) >> 8;
    p->unk10 = 0;
    rnd >>= 8;
    p->unk14 = rnd;
    return (rnd & 1) + 1;
}
