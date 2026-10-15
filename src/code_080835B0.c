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
    u8 filler13[0x18 - 0x13];
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
    u8 filler2[0x26 - 2];
    u16 unk26;
    u8 filler28[0x30 - 0x28];
    struct PartObj *head;
    struct PartObj *tail;
};

extern struct ActiveMgr *gUnk_03000D74;
extern const s16 gOffsetRows[];
extern const u8 gMapObjDescriptor[];
extern u8 *gWorkBuf;
extern u8 *gUnk_03000FD8;
extern u8 gUnk_02000420[];
void obj_setup_map_object(struct PartObj *obj, u32 prio, u32 arg, u32 cfg, s32 a, s32 b, u32 ptr);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void unknown_code_08084032(struct PartObj *obj, s32 x, s32 y);

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

struct PartObj *unknown_code_0808360c(struct PartObj *obj, u8 prio, u32 arg, u8 cfg, u32 ptr)
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

ASM_FUNC("asm/code_080835B0/unknown_code_08084032.s", void unknown_code_08084032(struct PartObj *obj, s32 x, s32 y))

ASM_FUNC("asm/code_080835B0/draw_text.s", void draw_text(void))

ASM_FUNC("asm/code_080835B0/vram_slots_draw_number.s", void vram_slots_draw_number(void))

ASM_FUNC("asm/code_080835B0/find_frame_entry.s", void find_frame_entry(void))

ASM_FUNC("asm/code_080835B0/refresh_sprite_state.s", void refresh_sprite_state(void))

void sprite_refresh_display(void)
{
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gUnk_02000420, gWorkBuf + 0x3C90, 0x20);
    gUnk_03000D74->unk26 = 0xFFFF;
    gUnk_03000FD8[0xC] |= 1;
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

NONMATCH("asm/code_080835B0/state_bits.s", s32 state_bits(struct PartObj *obj, s32 kind))
{
    s32 byte = obj->unkF0[0x1D];

    if (kind == 1)
    {
        return (byte >> 4) & 3;
    }
    if (kind > 1)
    {
        if (kind == 2)
        {
            return (byte >> 2) & 3;
        }
        return -1;
    }
    if (kind == 0)
    {
        return (u32)byte >> 6;
    }
    return -1;
}
END_NONMATCH

ASM_FUNC("asm/code_080835B0/measure_text.s", void measure_text(void))

NONMATCH("asm/code_080835B0/to_bcd.s", s32 to_bcd(s32 value))
{
    s32 rest = value;
    s32 digits = 1;
    s32 place;
    s32 bcd;
    s32 digit;

    place = 10;
    if (rest > 9)
    {
        do
        {
            digits++;
            place = place * 10;
        } while (place - 1 < rest);
    }
    bcd = 0;
    digits--;
    if (digits != -1)
    {
        do
        {
            rest = rest * 10;
            digit = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(rest, place);
            rest -= digit * place;
            bcd = (bcd << 4) | digit;
            digits--;
        } while (digits != -1);
    }
    return bcd;
}
END_NONMATCH

ASM_FUNC("asm/code_080835B0/unknown_code_08084840.s", void unknown_code_08084840(void))

void sub_080849AC(struct PartObj *obj, s32 a, s32 b, s32 c)
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

void unknown_code_08084a0a(struct PartObj *node, struct PartObj *repl)
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
        gUnk_03000D74->head = repl;
    }
    if (next != 0)
    {
        next->prev = repl;
    }
    else
    {
        gUnk_03000D74->tail = repl;
    }
}

void sub_08084A48(struct PartObj *node, struct PartObj *other)
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
    if (node == gUnk_03000D74->head)
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
        gUnk_03000D74->head = node;
    }
    other->prev = node;
    node->prev = prev;
    node->next = other;
    gUnk_03000D74->count++;
}

void sub_08084A8C(struct PartObj *node)
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
    if (node == gUnk_03000D74->head)
    {
        return;
    }
    tail = gUnk_03000D74->tail;
    if (tail != 0)
    {
        tail->next = node;
    }
    else
    {
        gUnk_03000D74->head = node;
    }
    node->prev = tail;
    gUnk_03000D74->count++;
    gUnk_03000D74->tail = node;
}

NONMATCH("asm/code_080835B0/list_push_active.s", void list_push_active(struct PartObj *node))
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
    head = gUnk_03000D74->head;
    if (node == head)
    {
        return;
    }
    if (head == 0)
    {
        gUnk_03000D74->tail = node;
    }
    else
    {
        head->prev = node;
    }
    node->next = head;
    gUnk_03000D74->count++;
    gUnk_03000D74->head = node;
}
END_NONMATCH

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
        if (node != gUnk_03000D74->head)
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
        gUnk_03000D74->head = next;
    }
    if (next != 0)
    {
        next->prev = prev;
    }
    else
    {
        gUnk_03000D74->tail = prev;
    }
    ((struct Bits11 *)&node->flags11)->bit4 = 0;
    node->prev = 0;
    node->next = 0;
    gUnk_03000D74->count--;
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

ASM_FUNC("asm/code_080835B0/load_vram_tiles.s", void load_vram_tiles(void))

