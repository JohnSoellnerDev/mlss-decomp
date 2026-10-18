/* agbcc: -funroll-loops */
#include "global.h"
#include "ram_routines.h"

struct PartObj
{
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u8 filler0C[0x11 - 0x0C];
    u8 flags11;
    u8 flags12;
    u8 filler13[0x14 - 0x13];
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    const u8 *descriptor;
    u8 filler1C[0x1F - 0x1C];
    u8 unk1F;
    u8 filler20[0x40 - 0x20];
    u8 *block;
    u8 filler44[0x4C - 0x44];
    u8 *sprite;
    u8 filler50[0x64 - 0x50];
    struct PartObj *prev;
    struct PartObj *next;
    u8 filler6C[0xE4 - 0x6C];
    u8 *state;
    u8 fillerE8[0xF0 - 0xE8];
    u8 *unkF0;
    u8 filler0F4[0x774 - 0xF4];
    u16 unk774;
    u16 unk776;
    u16 unk778;
    u16 unk77A;
};

struct Bits11
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct Bits12
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct ActiveMgr
{
    u8 filler0;
    u8 count;
    u8 filler2[0x24 - 2];
    u16 unk24;
    u16 unk26;
    u8 filler28[0x30 - 0x28];
    struct PartObj *head;
    struct PartObj *tail;
};

extern struct ActiveMgr *gModelCtx;
extern const s16 gOffsetRows[];
extern const u8 gMapObjDescriptor[];
extern u8 *gWorkBuf;
extern u8 *gWorld;
extern u8 gUnk_02000420[];
extern const u8 gUnk_083BA8A0[];
extern const u8 gUnk_083BA8AC[];
extern u8 gGlyphIndexTable[];
extern u32 gMapTable[];
extern u8 gSys[];
extern u8 gUnk_02000180[];
u32 table_085213b8(u32 index);
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void psg_wave_setup(u8 *base, u32 width, u32 x, u32 y, u32 glyph, s32 ch, u32 pal, u32 zero);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
extern const u8 gSpriteObjectTag[];
void obj_setup_map_object(struct PartObj *obj, u32 prio, u32 arg, u32 cfg, s32 a, s32 b, u32 ptr);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void unknown_code_08084032(struct PartObj *obj, s32 x, s32 y);

struct SysBase
{
    u8 filler0[2];
    u16 unk2;
    u8 filler4[0x888 - 4];
    u8 flags888;
};

struct PoseBits11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct PoseBits12
{
    u8 b0 : 1;
    u8 mid : 2;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 hi3 : 3;
};

struct PartObj *create_map_object(struct PartObj *obj, u8 prio, u32 arg, u8 cfg, s16 a, s16 b, u32 ptr)
{
    obj_setup_map_object(obj, prio, arg, cfg, a, b, ptr);
    obj->descriptor = gMapObjDescriptor;
    obj->unk778 = 0;
    obj->unk774 = 0;
    obj->unk77A = 0;
    obj->unk776 = 0;
    return obj;
}

struct PartObj *map_object_init_default(struct PartObj *obj, u8 prio, u32 arg, u8 cfg, u32 ptr)
{
    obj_setup_map_object(obj, prio, arg, cfg, 0, 0, ptr);
    obj->descriptor = gMapObjDescriptor;
    obj->unk778 = 0;
    obj->unk774 = 0;
    obj->unk77A = 0;
    obj->unk776 = 0;
    return obj;
}

ASM_FUNC("asm/code_080835B0/sprite_set_action.s", void sprite_set_action(void))

ASM_FUNC("asm/code_080835B0/apply_pose_to_parts.s", void apply_pose_to_parts(void))

NONMATCH("asm/code_080835B0/unknown_code_08084032.s", void unknown_code_08084032(struct PartObj *obj, s32 x, s32 y))
{
    s32 w;
    s32 h;

    w = gUnk_083BA8A0[(obj->unk2 >> 14) + ((obj->unk0 >> 14) << 2)];
    h = gUnk_083BA8AC[(obj->unk2 >> 14) + ((obj->unk0 >> 14) << 2)];
    if (x < -w || x > 0xEF || y < -h || y > 0x9F)
    {
        obj->unk6 = 1;
        return;
    }
    obj->unk2 = (obj->unk2 & 0xFE00) | (x & 0x1FF);
    obj->unk0 = (obj->unk0 & 0xFF00) | (y & 0xFF);
}
END_NONMATCH

ASM_FUNC("asm/code_080835B0/draw_text.s", void draw_text(void))

ASM_FUNC("asm/code_080835B0/vram_slots_draw_number.s", void vram_slots_draw_number(void))

ASM_FUNC("asm/code_080835B0/find_frame_entry.s", void find_frame_entry(void))

ASM_FUNC("asm/code_080835B0/refresh_sprite_state.s", void refresh_sprite_state(void))

void sprite_refresh_display(void)
{
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gUnk_02000420, gWorkBuf + 0x3C90, 0x20);
    gModelCtx->unk26 = 0xFFFF;
    gWorld[0xC] |= 1;
}

s32 signed_byte_e4(struct PartObj *part)
{
    return (s8)part->state[6];
}

void apply_offset_row(struct PartObj *obj, u32 row)
{
    u8 *sprite;
    const s16 *entry;
    s32 dx;
    s32 dy;
    s32 dz;

    if (obj == 0)
    {
        return;
    }
    sprite = obj->sprite;
    if (sprite == 0)
    {
        return;
    }
    entry = (const s16 *)((u8 *)gOffsetRows + (row << 1));
    dx = entry[0] << 5;
    dy = entry[1] << 5;
    dz = entry[2] << 5;
    if (dy < 0)
    {
        MEM_ROUTINE(void (*)(u32, u8 *, u32), iwram_fill)(0, sprite + dz, dx);
    }
    else
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(sprite + dy, sprite + dz, dx);
    }
}

s32 state_bits(struct PartObj *obj, s32 kind)
{
    s32 byte = obj->unkF0[0x1D];

    switch (kind)
    {
    case 0:
        return (u32)byte >> 6;
    case 1:
        return (byte >> 4) & 3;
    case 2:
        return (byte >> 2) & 3;
    }
    return -1;
}

s32 measure_text(u32 index, const u8 *str, s32 spacing)
{
    u32 *table;
    s32 total;
    s32 c;

    total = 0;
    table = *(u32 **)table_085213b8(index);
    for (;;)
    {
        c = *str++;
        if (c == 0xFF)
        {
            str++;
            continue;
        }
        if (c > 0xFB)
        {
            s32 font = 0xFF - c;
            u32 ch = *str;
            u32 v = ((table + font)[(ch >> 3) + 1] >> ((ch & 7) << 2)) & 0xF;
            s32 w = spacing + 1;

            total += v + w;
        }
        else if (c != 0)
        {
            u32 v = (table[c / 8 + 1] >> ((c & 7) << 2)) & 0xF;
            s32 w = spacing + 1;

            total += v + w;
        }
        else
        {
            return total;
        }
    }
}

s32 to_bcd(s32 value)
{
    s32 digits = 1;
    s32 place = 10;
    s32 bcd;
    s32 digit;

    while (place - 1 < value)
    {
        digits++;
        place *= 10;
    }
    bcd = 0;
    while (--digits != -1)
    {
        value *= 10;
        digit = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(value, place);
        value -= digit * place;
        bcd = (bcd << 4) | digit;
    }
    return bcd;
}

ASM_FUNC("asm/code_080835B0/unknown_code_08084840.s", void unknown_code_08084840(void))

void sprite_set_camera_offset(struct PartObj *obj, s32 a, s32 b, s32 c)
{
    s32 v[3];
    u32 w;

    v[0] = a;
    v[1] = b;
    v[2] = c;
    offset_three_words_camera(&v[0], &v[1], &v[2], 0);
    unknown_code_08084032(obj, v[0], v[1]);
    w = v[2] | 0x40000000;
    obj->unk8 = w;
    obj->unk4 = (obj->unk4 & 0xF3FF) | ((w >> 30) << 10);
}

u32 block_flag_byte(struct PartObj *obj)
{
    u8 *block = obj->block;
    u8 value = block[0];

    if (value & 0x80)
    {
        value = block[7];
    }
    return value;
}

void list_replace_active(struct PartObj *node, struct PartObj *repl)
{
    struct PartObj *next = node->next;
    struct PartObj *prev = node->prev;

    node->next = 0;
    node->prev = 0;
    repl->next = next;
    repl->prev = prev;
    if (prev != 0)
    {
        prev->next = repl;
    }
    else
    {
        gModelCtx->head = repl;
    }
    if (next != 0)
    {
        next->prev = repl;
    }
    else
    {
        gModelCtx->tail = repl;
    }
}

void list_insert_before_active(struct PartObj *node, struct PartObj *other)
{
    struct PartObj *prev;

    if (node == 0)
    {
        return;
    }
    if (node->next != node->prev)
    {
        return;
    }
    if (node == gModelCtx->head)
    {
        return;
    }
    prev = other->prev;
    if (prev != 0)
    {
        prev->next = node;
    }
    else
    {
        gModelCtx->head = node;
    }
    other->prev = node;
    node->prev = prev;
    node->next = other;
    gModelCtx->count++;
}

void list_append_active(struct PartObj *node)
{
    struct PartObj *tail;

    if (node == 0)
    {
        return;
    }
    if (node->next != node->prev)
    {
        return;
    }
    if (node == gModelCtx->head)
    {
        return;
    }
    tail = gModelCtx->tail;
    if (tail != 0)
    {
        tail->next = node;
    }
    else
    {
        gModelCtx->head = node;
    }
    node->prev = tail;
    gModelCtx->count++;
    gModelCtx->tail = node;
}

extern const u8 gUnk_083BA8A0[];
extern const u8 gUnk_083BA8AC[];
u32 table_085213b8(u32 index);
void list_push_active(struct PartObj *node)
{
    struct PartObj *head;

    if (node == 0)
    {
        return;
    }
    if (node->next != node->prev)
    {
        return;
    }
    if (node == gModelCtx->head)
    {
        return;
    }
    head = gModelCtx->head;
    if (head != 0)
    {
        head->prev = node;
    }
    else
    {
        gModelCtx->tail = node;
    }
    node->next = head;
    gModelCtx->count++;
    gModelCtx->head = node;
}

void list_remove_active(struct PartObj *node)
{
    struct PartObj *next;
    struct PartObj *prev;

    if (node == 0)
    {
        return;
    }
    ((struct Bits12 *)&node->flags12)->bit5 = 1;
    next = node->next;
    prev = node->prev;
    if (next == prev)
    {
        if (node != gModelCtx->head)
        {
            return;
        }
    }
    if (prev != 0)
    {
        prev->next = next;
    }
    else
    {
        gModelCtx->head = next;
    }
    if (next != 0)
    {
        next->prev = prev;
    }
    else
    {
        gModelCtx->tail = prev;
    }
    ((struct Bits11 *)&node->flags11)->bit4 = 0;
    node->prev = 0;
    node->next = 0;
    gModelCtx->count--;
}

NONMATCH("asm/code_080835B0/spawn_obj_by_id.s", struct PartObj *spawn_obj_by_id(u16 id, u32 variant, u32 attr, u32 mode))
{
    struct PartObj *obj;
    s32 flags;
    u32 low;
    u32 top;

    obj = create_obj_de4(0, id, mode, -1, -1, -1, -1);
    obj_load_record(obj, variant, -1, 0, 0);
    low = attr & 3;
    flags = obj->unk1F;
    flags = (flags & -4) | low;
    flags = (flags & -13) | (low << 2);
    flags = (flags & -49) | (low << 4);
    top = attr << 6;
    obj->unk1F = (flags & 0x3F) | top;
    if (mode == 1)
    {
        list_remove_active(obj);
    }
    return obj;
}
END_NONMATCH



ASM_FUNC("asm/code_080835B0/init_sprite_object.s", struct PartObj *init_sprite_object(struct PartObj *obj))

NONMATCH("asm/code_080835B0/load_vram_tiles.s", void load_vram_tiles(void))
{
    u32 idx;
    u32 *entry;
    void (*copy)(u8 *, u8 *, u32);
    u32 row;
    u32 flags;

    MEM_ROUTINE(void (*)(u32, u8 *, u32), iwram_fill)(0, (u8 *)0x06004000, 0x9000);
    MEM_ROUTINE(void (*)(u32, u8 *, u32), iwram_fill)(0, (u8 *)0x0600F000, 0x1000);
    idx = *(u16 *)gGlyphIndexTable;
    entry = gMapTable + idx;
    queue_display_entry(2, (u8 *)gMapTable + *entry, (u8 *)0x06008000, 0x2C0, 0, 0);
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    flags = ((struct SysBase *)gSys)->flags888;
    row = 0x12;
    if ((6 & flags) != 0)
    {
        row = 0x14;
    }
    idx = *(u16 *)(gGlyphIndexTable + row);
    entry = gMapTable + idx;
    copy((u8 *)gMapTable + *entry, gUnk_02000180, 0xC0);
    ((struct SysBase *)gSys)->unk2 |= 0x3F00;
}
END_NONMATCH

