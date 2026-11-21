#include "global.h"

struct Task
{
    struct Task *next;
    u32 fn;
    u8 *data;
    struct PopupNode *node;
    u16 timer;
};

/* A rising number's position and motion; taken from World.nodePool, back there when it's gone. */
struct PopupNode
{
    struct PopupNode *next;
    s32 x;      /* screen position, 8.8 fixed point */
    s32 y;
    u8 *out;    /* set to 1 while the number shows, 0 once it's gone (may be NULL) */
    s16 vx;     /* speed per frame, 8.8 fixed point */
    u16 vy;
    u8 filler14[2];
    u16 life;   /* frames left in the current step (rise, then fade) */
    s16 kind;   /* OBJ palette (POPUP_PAL_*) */
};

struct PopupDirtyBits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
};

struct SpriteRing;

struct SpritePart
{
    u8 filler0[0x28];
    struct SpriteRing *ring;
    u8 filler2C[0x76 - 0x2C];
    u8 flags76;
    u8 index;
    u8 filler78[0x94 - 0x78];
    s32 unk94;
    u8 filler98[0xB2 - 0x98];
    u16 unkB2;
};

struct PartIndexBits
{
    u8 low : 3;
    u8 high : 5;
};

struct LevelBits
{
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 hi4 : 4;
};

struct ModeBits
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

/* World.slotA / slotB: a part at +8 (OBJ_SLOT_PART), and the object linked to it. */
struct ObjSlot
{
    u8 filler0[0x38];
    struct SpriteObj *link;
    u8 filler3C[0x7E - 0x3C];
    u8 flags7E;     /* the part's flags76: ModeBits.mode 1 while it is in play */
};
#define OBJ_SLOT_PART(slot) ((struct SpritePart *)((u8 *)(slot) + 8))

struct SpriteObj;

typedef s32 (*ObjHandler)(struct SpriteObj *obj, struct SpritePart *part);

struct RingFlagsBits
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct Flags76Bits
{
    u8 lo3 : 3;
    u8 mid3 : 3;
    u8 hi2 : 2;
};

struct RecordStateBits
{
    u8 lo1 : 1;
    u8 mid2 : 2;
    u8 hi5 : 5;
};

struct SpriteRecord
{
    u8 filler0[0x12];
    u8 state;
};

struct SpriteRing
{
    u8 filler0[0xEC];
    s16 kind;
    u8 fillerEE[0xF6 - 0xEE];
    s16 f6;
    u8 fillerF8[0x111 - 0xF8];
    u8 flags111;
    u8 filler112[0x11E - 0x112];
    u16 flags11E;
};

struct SpriteObj
{
    u8 filler0[0x08];
    struct SpriteRecord *record;
    u8 filler0C[0x28 - 0x0C];
    struct SpriteRing *ring;
    u8 filler2C[0x30 - 0x2C];
    struct SpriteObj *parent;
    struct SpriteObj *activeNext;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    u32 stateFn;
    u8 filler50[0x54 - 0x50];
    u32 unk54;
    ObjHandler handlers[4];
    u32 unk68;
    u8 filler6C[0x76 - 0x6C];
    u8 flags76;
    u8 flags77;
    u8 filler78[0xB8 - 0x78];
    u16 b8;
    u8 fillerBA[0xBE - 0xBA];
    u8 popupShown; /* 1 while number_popup shows this object's number */
    u8 fillerBF[0xC3 - 0xBF];
    u8 ofsX;
    u8 ofsY;
    u8 ofsZ;
};

// A scanline (VCOUNT) interrupt entry. Nodes come from the same free pool as tasks
// (World.taskFree) and share their first two fields; the list is sorted by line.
struct RasterNode
{
    struct RasterNode *next;
    u32 fn; // run once per frame by task_frame_update; 0 frees the node
    u8 filler8[4];
    void (*handler)(struct RasterNode *node); // run by raster_irq_vcount at the line
    s16 line;
};

struct World
{
    u8 filler0[0x0B];
    u8 dirty;
    u8 flagsC;
    u8 levelD;
    u8 filler0E[0x12 - 0x0E];
    u16 flags12;
    u8 filler14[0x3C - 0x14];
    struct Task *taskFree;
    u8 filler40[0x48 - 0x40];
    struct Task *taskList;
    struct Task *list4C;
    struct Task *frameTasks;
    struct SpriteObj *objs;
    u8 filler58[0x70 - 0x58];
    struct SpriteObj *unk70;
    struct SpriteObj *unk74;
    u8 filler78[0x80 - 0x78];
    struct ObjSlot *slotA;
    struct ObjSlot *slotB;
    u8 filler88[0x240 - 0x88];
    struct PopupNode *nodePool;
    u8 filler244[0x24C - 0x244];
    void *camera;
    void *copyQueue;
    struct RasterNode *rasterList;
    struct RasterNode *rasterCur;
    void (*frameHook)(void);
};

extern struct World *gWorld;
void sprite_set_action(struct SpriteObj *obj, s32 index, s32 value, s32 flags);
void set_slot_value(s32 slot, s32 value);
void load_table_entries(s32 count);
void release_object(struct SpriteRing *ring);
void place_targets(struct SpriteObj *obj, s32 col, s32 row, s32 z, s32 extra);
void set_motion_start(struct SpriteObj *obj, s32 value);
void task_release_obj(struct Task *task);
extern void obj_update_pose(void);
void raster_irq_vcount(void);
void obj_run_part(struct SpriteObj *obj, struct SpritePart *part);
s32 obj_try_run_part(struct SpriteObj *obj, struct SpritePart *part);
void popup_rise_step_b(struct Task *task);
void set_irq_callback(u32 kind, void *cb);
#define IRQ_SLOT_VCOUNT 3 // set_irq_callback
void raster_irq_default(void);
extern vu16 gUnk_02000004; // shadow of REG_DISPSTAT
void oam_write_tiles(u8 *digits, s32 x, s32 y, s32 kind, s32 flags);
void oam_write_number(u8 *digits, s32 x, s32 y, s32 scale, s32 flags);
void popup_fade_step(struct Task *task);
void popup_fade_step_b(struct Task *task);
void effect_follow(void);
u32 search_frame_pairs(struct SpriteObj *obj, struct SpritePart *part);
extern u8 *gMemRoutines;
extern void task_draw_step_a(void);
void offset_three_words(void *camera, s32 *a, s32 *b, s32 *c, u32 d);
u8 *to_bcd(u32 value);
s32 number_popup_raster(void);
struct Task *number_popup_spawn(u32 value, s32 x, s32 y, s32 z, s32 kind);
extern u8 gSys[];
extern u8 gGlyphIndexTable[];
extern u32 gMapTable[];
extern u8 gUnk_02000440[]; /* shadow of OBJ palette 14 */
extern u8 iwram_copy[];

/* OBJ palettes of the number popups (number_popup's kind) */
#define POPUP_PAL_NUMBER 11
#define POPUP_PAL_SPECIAL 14 /* gUnk_02000440, loaded before use */
extern u8 iwram_fill[];
extern void task_anim_countdown(void);
extern void task_draw_step_b(void);
void popup_rise_step(struct Task *task);

/* Shows `value` as a number that pops up from the world point (x, y, z) in OBJ palette `kind` and
 * sets *out to 1 until it's gone. Side 1 is number_popup_spawn's (up and to the left); otherwise
 * it starts 8 px right of and 16 px above the point, flies up and right, and slows down for 14
 * frames (popup_rise_step) before it fades. Returns the task that draws it. */
struct Task *number_popup(u32 value, s32 x, s32 y, s32 z, s32 side, s32 kind, u8 *out)
{
    struct Task *task;
    struct PopupNode *node;
    struct World *world;
    struct World **pworld;
    u32 drawFn;

    if (side == 1)
    {
        return number_popup_spawn(value, x, y, z, kind);
    }
    pworld = &gWorld;
    /* take a task from the free list and put it at the head of the task list */
    world = *pworld;
    drawFn = (u32)task_draw_step_b;
    task = world->taskFree;
    world->taskFree = task->next;
    task->next = world->taskList;
    task->fn = drawFn;
    task->timer = 0;
    world->taskList = task;
    node = (*pworld)->nodePool;
    (*pworld)->nodePool = node->next;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, node, sizeof(struct PopupNode));
    /* world point to screen */
    offset_three_words((*pworld)->camera, &x, &y, &z, 0);
    if (out != NULL)
        *out = 1;
    node->out = out;
    node->x = (x + 8) << 8;
    node->vx = 0x100;
    node->y = (y - 0x10) << 8;
    node->vy = 0xFD9A;
    node->kind = kind;
    node->life = 14;
    task->fn = (u32)popup_rise_step;
    task->data = to_bcd(value);
    task->node = node;
    return task;
}

void popup_rise_step(struct Task *task)
{
    struct PopupNode *node = task->node;
    s32 t;

    node->vx = (node->vx * 0xFA) / 0x100;
    node->vy += 0x3D;
    node->x += node->vx;
    node->y += (s16)node->vy;
    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    if ((u16)t == 0)
    {
        node->life = 0x18;
        task->fn = (u32)popup_fade_step;
    }
}

void popup_fade_step(struct Task *task)
{
    struct PopupNode *node = task->node;
    u8 *out;
    s16 life;
    s32 t;

    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    life = t;
    if (life == 0)
    {
        out = node->out;
        if (out != NULL)
            *out = 0;
        if (node != NULL)
        {
            node->next = gWorld->nodePool;
            gWorld->nodePool = node;
        }
        task->fn = life;
    }
}

/* number_popup's side 1: shows `value` as a number that pops up from the world point (x, y, z)
 * in OBJ palette `kind`, starting 12 px left of and 24 px above it, flying up and left and
 * slowing down for 14 frames (popup_rise_step_b) before it fades. Its node's `out` stays NULL.
 * Returns the task that draws it. */
struct Task *number_popup_spawn(u32 value, s32 x, s32 y, s32 z, s32 kind)
{
    struct Task *task;
    struct PopupNode *node;
    struct World *world;
    struct World **pworld;
    u32 drawFn;

    pworld = &gWorld;
    /* take a task from the free list and put it at the head of the task list */
    world = *pworld;
    drawFn = (u32)task_draw_step_a;
    task = world->taskFree;
    world->taskFree = task->next;
    task->next = world->taskList;
    task->fn = drawFn;
    task->timer = 0;
    world->taskList = task;
    node = (*pworld)->nodePool;
    (*pworld)->nodePool = node->next;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, node, sizeof(struct PopupNode));
    /* world point to screen */
    offset_three_words((*pworld)->camera, &x, &y, &z, 0);
    node->x = (x - 12) << 8;
    node->vx = -0xCC;
    node->y = (y - 24) << 8;
    node->vy = -0x166;
    node->kind = kind;
    node->life = 14;
    task->fn = (u32)popup_rise_step_b;
    task->data = to_bcd(value);
    task->node = node;
    return task;
}

void popup_rise_step_b(struct Task *task)
{
    struct PopupNode *node = task->node;
    s32 t;

    node->vx = (node->vx * 0xFA) / 0x100;
    node->vy += 0x3D;
    node->x += node->vx;
    node->y += (s16)node->vy;
    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    if ((u16)t == 0)
    {
        node->life = 0x18;
        task->fn = (u32)popup_fade_step_b;
    }
}

void popup_fade_step_b(struct Task *task)
{
    struct PopupNode *node = task->node;
    u8 *out;
    s16 life;
    s32 t;

    oam_write_tiles(task->data, node->x / 0x100, node->y / 0x100, node->kind, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    t = node->life - 1;
    node->life = t;
    life = t;
    if (life == 0)
    {
        out = node->out;
        if (out != NULL)
            *out = 0;
        if (node != NULL)
        {
            node->next = gWorld->nodePool;
            gWorld->nodePool = node;
        }
        task->fn = life;
    }
}

NONMATCH("asm/code_0807D478/popup_digit_spawn.s", struct Task *popup_digit_spawn(u32 digit, s32 x, s32 y, s32 z, u32 flag))
{
    struct World **pworld;
    struct PopupNode *node;
    struct Task *task;
    struct World *state;
    u32 glyphIndex;
    u32 idx;
    u32 sysVal;
    u8 *table;
    s32 xStart;
    u32 fn;
    void (*copy)(u8 *, u8 *, u32);

    copy = (void (*)(u8 *, u8 *, u32))(gMemRoutines + ((u32)iwram_copy - (u32)iwram_fill));
    table = gGlyphIndexTable;
    glyphIndex = ((gSys[0x888] & 6) != 0) ? 0x1C : 0x1A;
    idx = *(u16 *)(table + glyphIndex);
    copy((u8 *)gMapTable + *(u32 *)((u8 *)gMapTable + (idx << 2)), gUnk_02000440, 0x20);
    sysVal = *(u16 *)gSys;
    *(u16 *)gSys = sysVal | 0x4000;
    pworld = &gWorld;
    state = *pworld;
    fn = (u32)task_anim_countdown;
    task = state->taskFree;
    state->taskFree = task->next;
    task->next = state->taskList;
    task->fn = fn;
    task->timer = 0;
    state->taskList = task;
    node = (*pworld)->nodePool;
    (*pworld)->nodePool = node->next;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, node, 0x1C);
    offset_three_words((*pworld)->camera, &x, &y, &z, 0);
    if (digit <= 9)
        xStart = x + 4;
    else
        xStart = x + 8;
    node->x = xStart;
    node->x = node->x << 8;
    node->y = y << 8;
    node->vy = 0xFF00;
    node->life = 0xA;
    task->data = to_bcd(digit);
    task->node = node;
    return task;
}
END_NONMATCH
void popup_wave_step(struct Task *task)
{
    struct PopupNode *node = task->node;
    u8 *digits;
    s32 t;

    node->vy -= 0xB4;
    node->y += (s16)node->vy;
    t = node->life - 1;
    node->life = t;
    digits = task->data;
    oam_write_number(digits, node->x / 0x100, node->y / 0x100, ((s16)node->life * 2 + (s16)node->life) * 4, 0);
    ((struct PopupDirtyBits *)&gWorld->dirty)->bit1 = 1;
    if (node->y / 0x100 <= -16)
    {
        if (node != NULL)
        {
            node->next = gWorld->nodePool;
            gWorld->nodePool = node;
        }
        task->fn = 0;
    }
}

ASM_FUNC("asm/code_0807D478/oam_write_number.s", void oam_write_number(u8 *digits, s32 x, s32 y, s32 scale, s32 flags))

// Turns on the VCOUNT interrupt and adds a raster node (taken from the task free pool) that calls
// `handler` when the screen reaches scanline `line`. The list stays sorted by line, a new node going
// after those with the same line. `frameFn` runs once per frame (task_frame_update) until it sets
// node->fn to 0; without one the node gets the empty raster_irq_default and stays until removed.
struct RasterNode *raster_line_add(void (*handler)(struct RasterNode *node), s32 line, void *frameFn)
{
    struct RasterNode *node;
    struct RasterNode *cur;
    struct RasterNode *prev;
    vu16 *irqReg;

    set_irq_callback(IRQ_SLOT_VCOUNT, raster_irq_vcount);
    REG_IME = 0;
    gUnk_02000004 &= 0xFF; // clear the VCOUNT line setting
    gUnk_02000004 |= DISPSTAT_VCOUNT_IRQ;
    irqReg = (vu16 *)REG_ADDR_IE;
    *irqReg |= IRQ_VCOUNT;
    irqReg++; // REG_IF
    *irqReg |= IRQ_VCOUNT;
    REG_IME = 1;

    node = (struct RasterNode *)gWorld->taskFree;
    gWorld->taskFree = (struct Task *)node->next;

    cur = gWorld->rasterList;
    prev = NULL;
    while (cur != NULL && line >= cur->line)
    {
        prev = cur;
        cur = cur->next;
    }
    node->next = cur;
    if (prev == NULL)
        goto insert_at_head;
    prev->next = node;
linked:
    if (frameFn == NULL)
        goto no_frame_fn;
    node->fn = (u32)frameFn;
    goto set_fields;
insert_at_head:
    gWorld->rasterList = node;
    goto linked;
no_frame_fn:
    node->fn = (u32)raster_irq_default;
set_fields:
    node->handler = handler;
    node->line = line;
    return node;
}

void flush_copy_queue(u8 *obj);
// Runs once per frame: calls the frame hook, then every frame task and every raster
// node's per-frame callback, returning to the free pool those whose fn is (or became) 0.
// Then re-arms the VCOUNT interrupt for the first raster line, or turns it off when
// no raster nodes are left.
void task_frame_update(void)
{
    struct Task **head;
    struct Task *task;
    struct Task *next;
    struct Task *prev;
    struct Task *cur;
    struct RasterNode **rhead;
    struct RasterNode *node;
    struct RasterNode *rnext;
    struct RasterNode *rprev;
    struct RasterNode *rcur;
    struct RasterNode *first;
    struct World *world;

    if (gWorld->frameHook != NULL)
        gWorld->frameHook();

    head = &gWorld->frameTasks;
    prev = NULL;
    task = *head;
    while (task != NULL)
    {
        next = task->next;
        if (task->fn != 0)
        {
            ((void (*)(struct Task *))task->fn)(task);
            if (task->fn != 0)
            {
                prev = task;
                goto keep_task;
            }
        }
        // Finished: unlink it and put it back on the free list.
        if (prev != NULL)
        {
            prev->next = next;
        }
        else
        {
            cur = *head;
            if (cur == task)
            {
                *head = next;
            }
            else
            {
                do
                {
                    prev = cur;
                    cur = prev->next;
                } while (cur != task);
                prev->next = next;
            }
        }
        task->next = gWorld->taskFree;
        gWorld->taskFree = task;
    keep_task:
        task = next;
    }

    if (gWorld->copyQueue != NULL)
        flush_copy_queue(gWorld->copyQueue);

    if (gWorld->rasterList != NULL)
    {
        rhead = &gWorld->rasterList;
        rprev = NULL;
        node = *rhead;
        do
        {
            rnext = node->next;
            if (node->fn != 0)
            {
                ((void (*)(struct RasterNode *))node->fn)(node);
                if (node->fn != 0)
                {
                    rprev = node;
                    goto keep_node;
                }
            }
            if (rprev != NULL)
            {
                rprev->next = rnext;
            }
            else
            {
                rcur = *rhead;
                if (rcur == node)
                {
                    *rhead = rnext;
                }
                else
                {
                    do
                    {
                        rprev = rcur;
                        rcur = rprev->next;
                    } while (rcur != node);
                    rprev->next = rnext;
                }
            }
            node->next = (struct RasterNode *)gWorld->taskFree;
            gWorld->taskFree = (struct Task *)node;
        keep_node:
            node = rnext;
        } while (node != NULL);

        world = gWorld;
        first = world->rasterList;
        if (first != NULL)
        {
            REG_IME = 0;
            REG_DISPSTAT &= 0xFF;
            REG_DISPSTAT = (first->line << DISPSTAT_VCOUNT_SHIFT) | DISPSTAT_VCOUNT_IRQ | REG_DISPSTAT;
            REG_IE |= IRQ_VCOUNT;
            REG_IF |= IRQ_VCOUNT;
            REG_IME = 1;
            world->rasterCur = first;
        }
        else
        {
            set_irq_callback(IRQ_SLOT_VCOUNT, NULL);
            REG_IME = 0;
            gUnk_02000004 &= ~DISPSTAT_VCOUNT_IRQ;
            REG_IE &= ~IRQ_VCOUNT;
            REG_IF |= IRQ_VCOUNT;
            REG_IME = 1;
        }
    }
}

NONMATCH("asm/code_0807D478/raster_irq_vcount.s", void raster_irq_vcount(void))
{
    struct RasterNode *node;
    struct RasterNode *next;
    s32 nextLine;
    vu16 *regDispstat;
    s32 cur;
    s32 (*waitFn)(void);
    u32 v;
    vu16 *regIme;
    vu16 *regIe;
    u32 vcountBit;

    regIme = (vu16 *)0x04000208;
    node = gWorld->rasterCur;
    cur = node->line;
    waitFn = number_popup_raster;
    regDispstat = (vu16 *)0x04000004;
    regIe = (vu16 *)0x04000200;
    vcountBit = 4;
again:
    if (node->handler != NULL)
        node->handler(node);
    next = node->next;
    if (next == NULL)
        goto done;
    nextLine = next->line;
    if (cur >= nextLine)
        goto advance;
    gWorld->rasterCur = next;
    v = nextLine << 8;
    do
    {
    } while (waitFn() == 0);
    *regIme = 0;
    *regDispstat = *regDispstat & 0xFF;
    v |= 0x20;
    v |= *regDispstat;
    *regDispstat = v;
    *regIe |= vcountBit;
    REG_IF |= vcountBit;
    *regIme = 1;
    goto done;
advance:
    node = next;
    goto again;
done:
    return;
}
END_NONMATCH

/* Pops up `value` (e.g. HP lost or gained) at the object's position plus (x, y, z). Kinds 0-2: a
 * number in OBJ palette 11; kind 4: the same in palette 14, which it loads first (and sets bit
 * 0x4000 of gWorld->flags12 and of gSys's first halfword); kind 3: popup_digit_spawn. The number
 * flies left when the object's ring kind is negative. Nothing for a negative value. */
void obj_draw_part(struct SpriteObj *obj, s32 value, s32 kind, s32 x, s32 y, s32 z)
{
    void (*copy)(u8 *, u8 *, u32);
    u8 *table;
    u32 paletteEntry;

    if (value < 0)
        return;
    x += obj->posX / 0x100 + (s8)obj->ofsX;
    y += obj->posY / 0x100 + (s8)obj->ofsY;
    z += obj->posZ / 0x100 + (s8)obj->ofsZ;
    switch (kind)
    {
    case 0:
    case 1:
    case 2:
        if (obj->ring->kind < 0)
            number_popup(value, x, y, z, 1, POPUP_PAL_NUMBER, &obj->popupShown);
        else
            number_popup(value, x, y, z, 0, POPUP_PAL_NUMBER, &obj->popupShown);
        break;
    case 4:
        gWorld->flags12 |= 0x4000;
        if (obj->ring->kind < 0)
            number_popup(value, x, y, z, 1, POPUP_PAL_SPECIAL, &obj->popupShown);
        else
            number_popup(value, x, y, z, 0, POPUP_PAL_SPECIAL, &obj->popupShown);
        copy = (void (*)(u8 *, u8 *, u32))(gMemRoutines + ((u32)iwram_copy - (u32)iwram_fill));
        table = gGlyphIndexTable;
        paletteEntry = ((gSys[0x888] & 6) != 0) ? 0x24 : 0x22;
        copy((u8 *)gMapTable + *(u32 *)((u8 *)gMapTable + (*(u16 *)(table + paletteEntry) << 2)), gUnk_02000440, 0x20);
        ((struct { u16 unk0; } *)gSys)->unk0 |= 0x4000;
        break;
    case 3:
        if (obj->ring->kind < 0)
            popup_digit_spawn(value, x, y, z, 1);
        else
            popup_digit_spawn(value, x, y, z, 0);
        break;
    }
}

ASM_FUNC("asm/code_0807D478/draw_obj_pose.s", void draw_obj_pose(void))

ASM_FUNC("asm/code_0807D478/sprite_objects_obj_set_mode.s", void sprite_objects_obj_set_mode(void))
/* A task's one-shot start for its object: clears flags76's middle bits; when the object's ring has
 * f6 == 0 it sets bit 0 of the ring's flags111, hands the object to task_release_obj and loads 0x24
 * table entries. Otherwise it plays action 0 (0x2000 for a ring of kind -1, else 0x204D), plays
 * slot 0x47 when bit 0x200 of the ring's flags11E is set, and ends the task. */
void task_begin_obj(struct Task *task)
{
    struct SpriteObj *obj = (struct SpriteObj *)task->data;

    ((struct Flags76Bits *)&obj->flags76)->mid3 = 0;
    if (obj->ring->f6 == 0)
    {
        ((struct RingFlagsBits *)&obj->ring->flags111)->bit0 = 1;
        task_release_obj(task);
        load_table_entries(0x24);
        return;
    }
    if (obj->ring->kind == -1)
        sprite_set_action(obj, 0, 0x2000, 0);
    else
        sprite_set_action(obj, 0, 0x204D, 0);
    if (obj->ring->flags11E & 0x200)
    {
        if (obj->ring->kind == -1)
            set_slot_value(0x47, -1);
        else
            set_slot_value(0x47, -1);
    }
    task->fn = 0;
}

/* A task step for the object in task->data: releases its ring, clears its state handler, plays
 * action 1 (pose 0x2028 for a ring of kind -1, else 0x2057) with song 0x71 (set_slot_value plays a
 * song), clears its record's state bits 1-2, starts it moving 22 px to the left at speed 0x80 and
 * hands the task on to obj_update_pose. */
void task_release_obj(struct Task *task)
{
    struct SpriteObj *obj = (struct SpriteObj *)task->data;
    struct SpriteRecord *record;

    release_object(obj->ring);
    obj->stateFn = 0;
    obj->unk68 = 0;
    if (obj->ring->kind == -1)
    {
        sprite_set_action(obj, 1, 0x2028, 0);
        set_slot_value(0x71, -1);
    }
    else
    {
        sprite_set_action(obj, 1, 0x2057, 0);
        set_slot_value(0x71, -1);
    }
    record = obj->record;
    ((struct RecordStateBits *)&record->state)->mid2 = 0;
    place_targets(obj, obj->posX / 0x100 - 0x16, obj->posY / 0x100, obj->posZ / 0x100, 0x100);
    set_motion_start(obj, 0x80);
    task->fn = (u32)obj_update_pose;
}

ASM_FUNC("asm/code_0807D478/obj_run_part.s", void obj_run_part(struct SpriteObj *obj, struct SpritePart *part))

/*
 * Runs part's contact handler on obj: part->index (low 3 bits) picks the handler slot of obj, or
 * of its parent when obj is an effect following it (effect_follow). Only when that slot is set and
 * the two touch (search_frame_pairs) is the handler called; if the handler returns 0, obj is taken
 * off the active list with its handlers cleared. Returns 1 when the handler ran and returned 0, else 0.
 */
s32 obj_try_run_part(struct SpriteObj *obj, struct SpritePart *part)
{
    struct SpriteObj *owner;
    struct SpriteObj *cur;
    struct SpriteObj *prev;
    struct SpriteObj *next;
    ObjHandler handler;
    ObjHandler *handlers;
    u32 index;
    u32 offset;

    index = part->index;
    if ((index & 7) == 0)
        goto none;
    if (obj->stateFn == (u32)effect_follow)
        owner = obj->parent;
    else
        owner = obj;
    offset = (u32)(index << 29) >> 27; /* (index & 7) * sizeof(ObjHandler) */
    handlers = owner->handlers;
    handler = *(ObjHandler *)((u8 *)handlers + offset);
    if (handler == NULL)
        goto none;
    if ((index = search_frame_pairs(obj, part)) == 0)
        goto none;
    if (owner != obj)
        owner->unk54 = 0;
    if (handler(owner, part) != 0)
        goto none;

    /* unlink obj from the active list */
    prev = NULL;
    cur = gWorld->objs;
    if (cur != NULL)
    {
        do
        {
            next = cur->activeNext;
            if (cur == obj)
            {
                if (prev != NULL)
                    prev->activeNext = next;
                else
                    gWorld->objs = next;
                obj->handlers[0] = NULL;
                obj->handlers[1] = NULL;
                obj->handlers[2] = NULL;
                obj->handlers[3] = NULL;
                obj->activeNext = NULL;
                break;
            }
            prev = cur;
            cur = next;
        } while (cur != NULL);
    }
    return 1;
none:
    return 0;
}

/* Clears World.levelD's middle bits, then runs every object on the active list against slot A's
 * and slot B's part while that slot is in play (mode 1): through the slot's linked object when it
 * is in play too and obj_try_run_part takes it, else on the slot's own part. */
void update_active_objs(void)
{
    struct SpriteObj *obj;
    struct SpriteObj *next;
    struct ObjSlot *slot;
    struct SpriteObj *link;

    ((struct LevelBits *)&gWorld->levelD)->mid2 = 0;
    obj = gWorld->objs;
    while (obj != NULL)
    {
        next = obj->activeNext; /* obj_try_run_part may unlink obj */
        slot = gWorld->slotA;
        if (slot != NULL && ((struct ModeBits *)&slot->flags7E)->mode == 1)
        {
            link = slot->link;
            if (link == NULL || ((struct ModeBits *)&link->flags76)->mode != 1)
                obj_run_part(obj, OBJ_SLOT_PART(slot));
            else if (obj_try_run_part(obj, (struct SpritePart *)link) == 0)
                obj_run_part(obj, OBJ_SLOT_PART(slot));
        }
        slot = gWorld->slotB;
        if (slot != NULL && ((struct ModeBits *)&slot->flags7E)->mode == 1)
        {
            link = slot->link;
            if (link == NULL || ((struct ModeBits *)&link->flags76)->mode != 1)
                obj_run_part(obj, OBJ_SLOT_PART(slot));
            else if (obj_try_run_part(obj, (struct SpritePart *)link) == 0)
                obj_run_part(obj, OBJ_SLOT_PART(slot));
        }
        obj = next;
    }
}

ASM_FUNC("asm/code_0807D478/unknown_mario_bros_code_0807e996.s", void unknown_mario_bros_code_0807e996(void))
