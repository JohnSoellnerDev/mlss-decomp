#include "global.h"

struct SlotChild
{
    u8 filler0[0x10];
    u8 *sprite;
    u8 filler14[4];
    u32 unk18;
    u32 unk1C;
    u32 unk20;
};

struct SpriteFlags12
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hi : 3;
};

struct SlotLink
{
    u8 filler0[0x10];
    s16 offset;
    u8 filler12[2];
    void (*routine)(void *, u32);
};

struct SlotObj
{
    u8 filler0[0xC];
    const void *classPtr;
    u8 filler10[0x18 - 0x10];
    struct SlotLink *link;
    u8 filler1C[0x38 - 0x1C];
    void *shadowSrc;
    u8 filler3C[0x40 - 0x3C];
    u32 timer40;
    u32 state;
    u8 filler48[0x56 - 0x48];
    u16 mode;
    u16 prevMode;
    u8 filler5A[0x6C - 0x5A];
    struct SlotChild *childA;
    struct SlotChild *childB;
    u8 filler74[0xA8 - 0x74];
    void *slotsA8[12];
    s32 numD8;
    s32 numDC;
    u8 fillerE0[0xE8 - 0xE0];
    u16 flagsE8;
};

struct SlotOwner
{
    u8 filler0[0xE8];
    u16 state;
};

struct KeyPad
{
    u8 filler0[0x28];
    u16 keys;
    u16 keys2A;
    u8 filler2C[0x8C0 - 0x2C];
    u8 lang8C0;
};

extern struct KeyPad gSys;

void obj_copy_mode_fields(struct SlotObj *obj);
void *obj_init_big(void *obj, u32 first);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void list_insert(void *list, void *node);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_release_child_sprites(struct SlotObj *obj);

extern const u8 gClassEc78[];
extern const u8 gHeapTagSpritePair0[];
extern const u8 gHeapTagSpritePair1[];
extern u16 gUnk_02000050[];
extern s16 gLanguageRowOffsets[];
u32 rand_scaled(u32 a, u32 b);
u8 timer_tick(void *timer);
void palette_fill(u32 v);
void set_slot_target(u32 idx, u32 value, u32 mode);
void slot_reset(u32 idx);
void obj_copy_pos_to_shadow(void *src);
void obj_phase_slide_a(struct SlotObj *obj);
void obj_phase_count_b(struct SlotObj *obj);
void slot_hud_obj_state4_if_flag8(struct SlotObj *obj);
void slot_hud_obj_run_stage(struct SlotObj *obj);
void draw_two_numbers(struct SlotObj *obj);

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_setup_slot(void *obj, u32 a, u32 b, u32 c, s32 offset, ByteArg flag);
void obj_claim_slot_97(void *obj);
void obj_release_slot_97(void *obj);

struct HudSprite
{
    u8 filler0[0xC];
    const void *classPtr;
    u8 filler10[0x18 - 0x10];
    u32 unk18;
    u8 filler1C[0x20 - 0x1C];
    u32 unk20;
    u8 filler24[0x52 - 0x24];
    u16 unk52;      // the cursor's direction: 0 up, 2 right, 4 down, 6 left
    u8 filler54[0x56 - 0x54];
    u16 unk56;      // 1 once it starts a move
    u8 filler58[0x6C - 0x58];
    s32 unk6C;      // the row or column the cursor is on (0-5)
    u32 unk70;      // number of moves
    u16 unk74;      // bit 0: no shifting the row/column, bit 1: no moving
};

// Where the slot puzzle's row cursor draws its 2x2 marks in the BG map (two map rows per board row).
#define SLOT_MAP ((u16 *)0x0600680C)
// Where the slot puzzle's column cursor draws its 2x2 marks in the BG map (two map columns per board column).
#define SLOT_COLUMN_MAP ((u16 *)0x06006900)

// The HUD's BG map (32 entries a row). Off screen it keeps the tiles the HUD copies from: each
// language's 4x2-tile labels at row gLanguageRowOffsets[lang], the digits 0-9 (2 tiles tall) at
// HUD_DIGITS.
#define HUD_MAP ((u16 *)0x06007000)
#define HUD_MAP_AT(row, col) (HUD_MAP + (row) * 32 + (col))
#define HUD_DIGITS 0x290

void obj_update_frame_flags(void *obj);
void obj_script_step(void *obj);
u8 gate_flag_set(u16 mask);
u8 obj_is_done(void *obj);
void obj_set_table_fields(void *obj, const void *table);
void obj_setup_row_slots_a(struct SlotObj *obj, s32 group);
void obj_setup_row_slots_b(struct SlotObj *obj, s32 group);
void obj_setup_row_slots_c(struct SlotObj *obj, s32 group);
void obj_fill_slot_group(struct SlotObj *obj, u32 group);
extern const u8 gSlotHudUpScript[];
extern const u8 gSlotHudDownScript[];
extern const u8 gSlotHudLeftScript[];
extern const u8 gSlotHudRightScript[];

/* TRUE while any of the twelve slot objects is still alive. */
static inline u8 slots_in_use(struct SlotObj *obj)
{
    s32 i;

    for (i = 0; i < 12; i++)
    {
        if (obj->slotsA8[i] != 0)
            return TRUE;
    }
    return FALSE;
}

struct SlotPair
{
    void *a;
    u8 filler4[0x14];
    void *b;
};

ASM_FUNC("asm/code_081702A4/obj_setup_row_slots_a.s", void obj_setup_row_slots_a(struct SlotObj *obj, s32 group))


ASM_FUNC("asm/code_081702A4/obj_setup_row_slots_b.s", void obj_setup_row_slots_b(struct SlotObj *obj, s32 group))

ASM_FUNC("asm/code_081702A4/obj_setup_row_slots_c.s", void obj_setup_row_slots_c(struct SlotObj *obj, s32 group))

NONMATCH("asm/code_081702A4/obj_fill_slot_group.s", void obj_fill_slot_group(struct SlotObj *obj, u32 group))
{
    struct SlotPair *s;
    u8 *b;
    u8 *t;
    u8 *st;
    s32 i;
    u32 spawned;
    u32 shift;
    s32 limit;
    u32 ofs;
    u32 order;
    u32 cur;
    u16 flags;

    spawned = 0;
    i = 4;
    ofs = group << 2;
    t = (u8 *)obj + 8;
    b = (u8 *)(ofs + (u32)t);
    s = (struct SlotPair *)(b + 0x60);
    order = 0x8A00;
    shift = group << 12;
    limit = 5 - group;
    for (; i >= 0; i--)
    {
        cur = (u32)s->b;
        if (cur == 0 && s->a != 0)
        {
            obj_setup_slot(s->a, shift + 0x4600, 0, order, 8, (u8)cur);
            s->b = s->a;
            *(u16 *)((u8 *)s->b + 0x50) = limit;
            s->a = (void *)cur;
            spawned++;
        }
        s = (struct SlotPair *)((u8 *)s - 0x18);
        order -= 0x1000;
    }
    if (spawned != 0)
    {
        obj_claim_slot_97(obj);
        st = (u8 *)obj + 0xC0;
        *(u32 *)(st + ofs) = 8;
        flags = 0x28 | *(u16 *)((u8 *)obj + 0xE8);
    }
    else
    {
        obj_release_slot_97(obj);
        st = (u8 *)obj + 0xC0;
        *(u32 *)(st + ofs) = spawned;
        flags = 0xFF80 & *(u16 *)((u8 *)obj + 0xE8);
    }
    *(u16 *)((u8 *)obj + 0xE8) = flags;
}
END_NONMATCH


/* Draws the HUD's two numbers (numD8 and numDC, 0-999) as three 2-tile-tall digits each, copied
   from the digit tiles at HUD_DIGITS: the upper one at row 5, the lower one at row 15, column 2. */
void draw_two_numbers(struct SlotObj *obj)
{
    u16 *src;
    u16 *dst;
    s32 value;
    s32 digit;

    value = obj->numD8;
    digit = 0;
    while (value > 99)
    {
        value -= 100;
        digit++;
    }
    src = HUD_MAP + digit;
    src += HUD_DIGITS;
    dst = HUD_MAP_AT(5, 2);
    dst[0] = src[0];
    dst[0x20] = src[0x20];
    digit = 0;
    while (value > 9)
    {
        value -= 10;
        digit++;
    }
    src = HUD_MAP + digit;
    src += HUD_DIGITS;
    dst = HUD_MAP_AT(5, 3);
    dst[0] = src[0];
    dst[0x20] = src[0x20];
    src = HUD_MAP + value;
    src += HUD_DIGITS;
    dst = HUD_MAP_AT(5, 4);
    dst[0] = src[0];
    dst[0x20] = src[0x20];

    value = obj->numDC;
    digit = 0;
    while (value > 99)
    {
        value -= 100;
        digit++;
    }
    src = HUD_MAP + digit;
    src += HUD_DIGITS;
    dst = HUD_MAP_AT(15, 2);
    dst[0] = src[0];
    dst[0x20] = src[0x20];
    digit = 0;
    while (value > 9)
    {
        value -= 10;
        digit++;
    }
    src = HUD_MAP + digit;
    src += HUD_DIGITS;
    dst = HUD_MAP_AT(15, 3);
    dst[0] = src[0];
    dst[0x20] = src[0x20];
    src = HUD_MAP + value;
    src += HUD_DIGITS;
    dst = HUD_MAP_AT(15, 4);
    dst[0] = src[0];
    dst[0x20] = src[0x20];
}

/* Draws the labels of the HUD's two numbers in the current language, then the numbers. The upper
   number gets the 4x2 label at column 12 of the language's label row. For languages 2-4 the lower
   number gets the one at column 8; for the others column 8's label goes two rows higher and the
   lower number gets column 12's label too. */
void hud_update_tiles(struct SlotObj *obj)
{
    obj->flagsE8 |= 0x1000;
    switch (gSys.lang8C0)
    {
    case 2:
    case 3:
    case 4:
    {
        u16 *label = HUD_MAP + gLanguageRowOffsets[gSys.lang8C0] * 0x20;
        u16 *dst;

        label += 0xC;
        dst = HUD_MAP_AT(3, 0);
        dst[0] = label[0];
        dst[1] = label[1];
        dst[2] = label[2];
        dst[3] = label[3];
        *HUD_MAP_AT(4, 0) = label[0x20];
        *HUD_MAP_AT(4, 1) = label[0x21];
        *HUD_MAP_AT(4, 2) = label[0x22];
        *HUD_MAP_AT(4, 3) = label[0x23];
    }
    {
        u16 *label = HUD_MAP + gLanguageRowOffsets[gSys.lang8C0] * 0x20;
        u16 *dst;

        label += 8;
        dst = HUD_MAP_AT(13, 0);
        dst[0] = label[0];
        dst[1] = label[1];
        dst[2] = label[2];
        dst[3] = label[3];
        *HUD_MAP_AT(14, 0) = label[0x20];
        *HUD_MAP_AT(14, 1) = label[0x21];
        *HUD_MAP_AT(14, 2) = label[0x22];
        *HUD_MAP_AT(14, 3) = label[0x23];
    }
        break;
    default:
    {
        u16 *label = HUD_MAP + gLanguageRowOffsets[gSys.lang8C0] * 0x20;
        u16 *dst;

        label += 0xC;
        dst = HUD_MAP_AT(3, 0);
        dst[0] = label[0];
        dst[1] = label[1];
        dst[2] = label[2];
        dst[3] = label[3];
        *HUD_MAP_AT(4, 0) = label[0x20];
        *HUD_MAP_AT(4, 1) = label[0x21];
        *HUD_MAP_AT(4, 2) = label[0x22];
        *HUD_MAP_AT(4, 3) = label[0x23];
    }
    {
        u16 *label = HUD_MAP + gLanguageRowOffsets[gSys.lang8C0] * 0x20;
        u16 *dst;

        label += 8;
        dst = HUD_MAP_AT(11, 0);
        dst[0] = label[0];
        dst[1] = label[1];
        dst[2] = label[2];
        dst[3] = label[3];
        *HUD_MAP_AT(12, 0) = label[0x20];
        *HUD_MAP_AT(12, 1) = label[0x21];
        *HUD_MAP_AT(12, 2) = label[0x22];
        *HUD_MAP_AT(12, 3) = label[0x23];
    }
    {
        u16 *label = HUD_MAP + gLanguageRowOffsets[gSys.lang8C0] * 0x20;
        u16 *dst;

        label += 0xC;
        dst = HUD_MAP_AT(13, 0);
        dst[0] = label[0];
        dst[1] = label[1];
        dst[2] = label[2];
        dst[3] = label[3];
        *HUD_MAP_AT(14, 0) = label[0x20];
        *HUD_MAP_AT(14, 1) = label[0x21];
        *HUD_MAP_AT(14, 2) = label[0x22];
        *HUD_MAP_AT(14, 3) = label[0x23];
    }
        break;
    }
    draw_two_numbers(obj);
}


ASM_FUNC("asm/code_081702A4/obj_reset_hud_tiles.s", void obj_reset_hud_tiles(void))

/* Frame update of the slot puzzle's column cursor. Holding LEFT/RIGHT moves it to the column beside
   it (columns 0-5); holding A and pressing UP/DOWN shifts the whole column that way
   (obj_setup_row_slots_c moves it up, obj_fill_slot_group down) once no slot is still moving, and
   draws the column's arrow mark. While gate flag 1 is set the plain mark is drawn. */
void obj_update_slot(struct HudSprite *cursor, struct SlotObj *board)
{
    obj_copy_mode_fields((struct SlotObj *)cursor);
    obj_script_step(cursor);
    if (gate_flag_set(1))
    {
        s32 col = cursor->unk6C * 2 + 8;
        u16 *src = (u16 *)0x060068BC;
        u16 *dst = SLOT_COLUMN_MAP + col;

        dst[0] = src[0];
        dst[1] = src[1];
        dst[0x20] = *(u16 *)0x060068FC;
        dst[0x21] = *(u16 *)0x060068FE;
    }
    if (obj_is_done(cursor) && !(board->flagsE8 & 0x80))
    {
        if ((gSys.keys & A_BUTTON) && (((u8)cursor->unk74 ^ 1) & 1))
        {
            cursor->unk52 = 4;
            if (!slots_in_use(board))
            {
                if (gSys.keys2A & DPAD_UP)
                {
                    obj_setup_row_slots_c(board, cursor->unk6C);
                    {
                        s32 col = cursor->unk6C * 2 + 8;
                        u16 *src = (u16 *)0x0600693C;
                        u16 *dst = SLOT_COLUMN_MAP + col;

                        dst[0] = src[0];
                        dst[1] = src[1];
                        dst[0x20] = *(u16 *)0x0600697C;
                        dst[0x21] = *(u16 *)0x0600697E;
                    }
                    obj_set_table_fields(cursor, gSlotHudUpScript);
                }
                else if (gSys.keys2A & DPAD_DOWN)
                {
                    obj_fill_slot_group(board, cursor->unk6C);
                    {
                        s32 col = cursor->unk6C * 2 + 8;
                        u16 *src = (u16 *)0x060069BC;
                        u16 *dst = SLOT_COLUMN_MAP + col;

                        dst[0] = src[0];
                        dst[1] = src[1];
                        dst[0x20] = *(u16 *)0x060069FC;
                        dst[0x21] = *(u16 *)0x060069FE;
                    }
                    obj_set_table_fields(cursor, gSlotHudDownScript);
                }
            }
        }
        else if (!(cursor->unk74 & 2))
        {
            if (gSys.keys & DPAD_LEFT)
            {
                if (cursor->unk6C > 0)
                {
                    cursor->unk6C--;
                    obj_setup_slot(cursor, (cursor->unk6C << 12) + 0x4800, 0, 0x2400, 8, (u8)0);
                    cursor->unk56 = 1;
                    cursor->unk52 = 6;
                    obj_script_step(cursor);
                    cursor->unk70++;
                }
            }
            else if (gSys.keys & DPAD_RIGHT)
            {
                if (cursor->unk6C <= 4)
                {
                    cursor->unk6C++;
                    obj_setup_slot(cursor, (cursor->unk6C << 12) + 0x4800, 0, 0x2400, 8, (u8)0);
                    cursor->unk56 = 1;
                    cursor->unk52 = 2;
                    obj_script_step(cursor);
                    cursor->unk70++;
                }
            }
            else
            {
                cursor->unk52 = 4;
            }
        }
    }
    obj_update_frame_flags(cursor);
}

/* Frame update of the slot puzzle's row cursor. Holding UP/DOWN moves it to the row above or below
   (rows 0-5); holding A and pressing LEFT/RIGHT shifts the whole row that way (once no slot is
   still moving) and draws the row's arrow mark. While gate flag 2 is set the plain mark is drawn. */
void obj_update_slot_2(struct HudSprite *cursor, struct SlotObj *board)
{
    obj_copy_mode_fields((struct SlotObj *)cursor);
    obj_script_step(cursor);
    if (gate_flag_set(2))
    {
        s32 col = cursor->unk6C * 0x40 + 0xC0;
        u16 *src = (u16 *)0x06006D04;
        u16 *dst = SLOT_MAP + col;

        dst[0] = src[0];
        dst[1] = src[1];
        dst[0x20] = *(u16 *)0x06006D44;
        dst[0x21] = *(u16 *)0x06006D46;
    }
    if (obj_is_done(cursor) && !(board->flagsE8 & 0x80))
    {
        if ((gSys.keys & A_BUTTON) && (((u8)cursor->unk74 ^ 1) & 1))
        {
            cursor->unk52 = 2;
            if (!slots_in_use(board))
            {
                if (gSys.keys2A & DPAD_LEFT)
                {
                    obj_setup_row_slots_a(board, cursor->unk6C);
                    {
                        s32 col = cursor->unk6C * 0x40 + 0xC0;
                        u16 *src = (u16 *)0x06006D08;
                        u16 *dst = SLOT_MAP + col;

                        dst[0] = src[0];
                        dst[1] = src[1];
                        dst[0x20] = *(u16 *)0x06006D48;
                        dst[0x21] = *(u16 *)0x06006D4A;
                    }
                    obj_set_table_fields(cursor, gSlotHudLeftScript);
                }
                else if (gSys.keys2A & DPAD_RIGHT)
                {
                    obj_setup_row_slots_b(board, cursor->unk6C);
                    {
                        s32 col = cursor->unk6C * 0x40 + 0xC0;
                        u16 *src = (u16 *)0x06006D0C;
                        u16 *dst = SLOT_MAP + col;

                        dst[0] = src[0];
                        dst[1] = src[1];
                        dst[0x20] = *(u16 *)0x06006D4C;
                        dst[0x21] = *(u16 *)0x06006D4E;
                    }
                    obj_set_table_fields(cursor, gSlotHudRightScript);
                }
            }
        }
        else if (!(cursor->unk74 & 2))
        {
            if (gSys.keys & DPAD_UP)
            {
                if (cursor->unk6C > 0)
                {
                    cursor->unk6C--;
                    obj_setup_slot(cursor, 0x3000, 0, (cursor->unk6C << 12) + 0x3C00, 8, (u8)0);
                    cursor->unk56 = 1;
                    cursor->unk52 = 0;
                    obj_script_step(cursor);
                    cursor->unk70++;
                }
            }
            else if (gSys.keys & DPAD_DOWN)
            {
                if (cursor->unk6C <= 4)
                {
                    cursor->unk6C++;
                    obj_setup_slot(cursor, 0x3000, 0, (cursor->unk6C << 12) + 0x3C00, 8, (u8)0);
                    cursor->unk56 = 1;
                    cursor->unk52 = 4;
                    obj_script_step(cursor);
                    cursor->unk70++;
                }
            }
            else
            {
                cursor->unk52 = 2;
            }
        }
    }
    obj_update_frame_flags(cursor);
}

// Sets obj up as the HUD's sprite pair (class gClassEc78): two child objects, each with a sprite
// of sprite set 0x5009 (animations 0x11 and 0x10) and its flag bit 4 set (obj_update_sprite_mode
// clears it while A is held), placed at +0x18 = 0x6E00 and +0x20 = 0x5C00; both children go into
// list. Ends by releasing the children's sprite slots (obj_release_child_sprites).
struct SlotObj *obj_init_sprite_pair(struct SlotObj *obj, void *list)
{
    u8 *sprite;

    obj_init_big(obj, 0xA);
    obj->classPtr = gClassEc78;
    obj->childA = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagSpritePair0, 0), 0xB);
    list_insert(list, obj->childA);
    obj_create_sprite_10(obj->childA, 4, 0x5009, 0, 0x11, 2);
    obj->childA->unk18 = 0x6E00;
    sprite = obj->childA->sprite;
    sprite[0x12] |= 0x10;

    obj->childB = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagSpritePair1, 0), 0xC);
    list_insert(list, obj->childB);
    obj_create_sprite_10(obj->childB, 4, 0x5009, 0, 0x10, 2);
    obj->childB->unk20 = 0x5C00;
    sprite = obj->childB->sprite;
    sprite[0x12] |= 0x10;

    obj_release_child_sprites(obj);
    return obj;
}

/* Per-frame update of the HUD sprite pair (see obj_init_sprite_pair): mode 5 while A is held and
 * owner->state bit 7 is clear, else mode 0. On a change, mode 5 clears both sprites' flag bit 4
 * (set at init) and their 2-bit mode field, mode 0 sets that field to 1. Then childB->unk18 follows
 * x and childA->unk20 follows y - 0x500. */
void obj_update_sprite_mode(struct SlotObj *obj, struct SlotOwner *owner, u32 x, u32 y)
{
    obj_copy_mode_fields(obj);
    if ((gSys.keys & A_BUTTON) && !(owner->state & 0x80))
        obj->mode = 5;
    else
        obj->mode = 0;
    if (obj->prevMode != obj->mode)
    {
        switch (obj->mode)
        {
        case 5:
            ((struct SpriteFlags12 *)(obj->childA->sprite + 0x12))->bit4 = 0;
            ((struct SpriteFlags12 *)(obj->childB->sprite + 0x12))->bit4 = 0;
            ((struct SpriteFlags12 *)(obj->childA->sprite + 0x12))->mode = 0;
            ((struct SpriteFlags12 *)(obj->childB->sprite + 0x12))->mode = 0;
            break;
        case 0:
            ((struct SpriteFlags12 *)(obj->childA->sprite + 0x12))->mode = 1;
            ((struct SpriteFlags12 *)(obj->childB->sprite + 0x12))->mode = 1;
            break;
        }
    }
    obj->childB->unk18 = x;
    obj->childA->unk20 = y - 0x500;
}

struct HudScene
{
    u8 filler0[0x18];
    const void *classPtr;
    void *list;
    void *window;
    void *task;
    void *spriteA;
    void *spriteB;
    void *hud;
    void *pair;
    void *clearBlock;
    void *counters;
    u32 timer40;
    u32 unk44;
    u32 unk48;
    u32 limit;
    u16 flags50;
};

struct HudTask
{
    u8 filler0[0x14];
    struct HudScene *scene;
    const void *classPtr;
    u32 unk1C;
    u32 unk20;
};

extern const u8 gClassScene[];
extern const u8 gClassTask[];
extern const u8 gClassSpriteA[];
extern const u8 gClassSpriteB[];
extern const u8 gClassCounter[];
extern const u8 gAnim9fc0[];
extern const u8 gHeapTagSceneList[];
extern const u8 gHeapTagHudWindow[];
extern const u8 gHeapTagHudTask[];
extern const u8 gHeapTagHudSpriteA[];
extern const u8 gHeapTagHudSpriteB[];
extern const u8 gHeapTagHudPair[];
extern const u8 gHeapTagHudCounters[];
extern const u8 gHeapTagHudCounterB[];
extern const u8 gHeapTagHudCounterObj[];
extern const u8 gSpriteFrameBaseByMode[];
extern const u8 gMinigameResultLabels[];
extern const u8 gHudScreenMap7000[];
extern const u8 gHudScreenMap6800[];
extern const u8 gSparkleStars[];
extern const u8 gUnidentifiedDitherStrip0821b851[];
extern const u8 gUnidentifiedDitherStrip0821b735[];
extern const u8 gUnk_0821BA20[];
extern const u8 gUnk_0821BA44[];
extern u8 gUnk_02000008[];
extern u8 gUnk_02000080[];
extern u8 gUnk_02000260[];

void task_insert(void *node, u32 kind, u32 data);
void timer_init_default(void *timer);
void clear_gate_flags(void);
void timer_set(void *timer, u32 limit);
void anim_task_start(void *task, u32 a, const void *table, u32 b, u32 c);
u8 get_flag_bit(u32 flag);
void tween_groups_set_flag_bit(u16 index, u8 value);
void lookup_or_alloc_13b4(s32 a, s32 b, s32 c, s32 d);
void lookup_or_alloc_14b8(s32 a, s32 b, s32 c, s32 d);
void find_or_alloc_obj_slots(s32 a, s32 b, s32 c, s32 d, s32 e, void *out, s32 slot);
void *obj_init_empty(void *block);
void *window_obj_setup(void *block, u32 a, u32 b, u32 c, u32 d);
void vram_load_packed(const void *src, void *dst);
void cpu_set(void *src, void *dst, u32 control);
void obj_init_counters(void *obj);
void obj_restore_counter_tiles(void *obj);
void *obj_clear(void *block, u32 a);
void *obj_create_hud_counter(void *block, void *list);
void write_var(u16 id, u16 value);
u32 read_var(u16 id);
void obj_list_update_pos(void *list, u32 a, u32 b);
void obj_set_frame_table(void *obj, const void *table);
void obj_update_frame_flags(void *obj);

struct HudModeBits
{
    u8 lo : 1;
    u8 mode : 2;
    u8 hi : 5;
};

struct SysPalDirty
{
    u8 filler0[2];
    u16 palDirty;
};

struct HudCounterObj
{
    u8 filler0[0xDC];
    s32 value;
};

/* Sprite of a HUD figure: sprite id, frame table and frame mode 2. */
static inline void hud_sprite_setup(struct HudSprite *sprite, u32 id)
{
    obj_create_sprite_10(sprite, 0, id, 1, 0, 3);
    obj_set_frame_table(sprite, gSpriteFrameBaseByMode);
    sprite->unk52 = 2;
    obj_update_frame_flags(sprite);
}

/* Builds a minigame scene: its task and timer, the sprite graphics it uses, the text window, the
   background graphics and display setup, the two brothers' sprites, the sprite pair, the
   counters and the HUD counter (which starts at the saved var 0x1CE2, at least 9). On the first
   visit (flag 0x1F2) it sets the scene's flags 0x11. */
struct HudScene *scene_build_hud(struct HudScene *scene, u8 prio, u32 taskArg)
{
    u32 found;
    u32 zero;
    s32 spriteId;
    s32 noParent;
    u8 *sys;
    u16 winPrio;

    task_insert(scene, prio, taskArg);
    scene->classPtr = gClassScene;
    timer_init_default(&scene->timer40);
    clear_gate_flags();
    timer_set(&scene->timer40, 0x20);
    anim_task_start(heap_alloc2(0x24, 0, gAnim9fc0, 0), 8, gAnim9fc0, 0, 0x20);
    scene->flags50 = 0;
    if (get_flag_bit(0x1F2) == 0)
    {
        tween_groups_set_flag_bit(0x1F2, 1);
        scene->flags50 |= 0x11;
    }
    if (get_flag_bit(0x1F3) != 0)
        scene->flags50 |= 8;

    // tiles and palettes of the sprites 0x1001, 0x1002, 0x105F, 0x107D, 0x105D, 0x107B and 0x5009
    spriteId = 0x1001;
    noParent = -1;
    lookup_or_alloc_13b4(0, spriteId, noParent, 1);
    lookup_or_alloc_14b8(0, spriteId, noParent, 1);
    find_or_alloc_obj_slots(0, spriteId, 0, 0, 0, &found, noParent);
    spriteId++;
    lookup_or_alloc_13b4(0, spriteId, noParent, 1);
    lookup_or_alloc_14b8(0, spriteId, noParent, 1);
    find_or_alloc_obj_slots(0, spriteId, 0, 0, 0, &found, noParent);
    spriteId += 0x5D;
    lookup_or_alloc_13b4(0, spriteId, noParent, 1);
    lookup_or_alloc_14b8(0, spriteId, noParent, 1);
    spriteId += 0x1E;
    lookup_or_alloc_13b4(0, spriteId, noParent, 1);
    lookup_or_alloc_14b8(0, spriteId, noParent, 1);
    spriteId -= 0x20;
    lookup_or_alloc_13b4(0, spriteId, noParent, 1);
    lookup_or_alloc_14b8(0, spriteId, noParent, 1);
    spriteId += 0x1E;
    lookup_or_alloc_13b4(0, spriteId, noParent, 1);
    lookup_or_alloc_14b8(0, spriteId, noParent, 1);
    spriteId = 0x5009;
    lookup_or_alloc_13b4(4, spriteId, noParent, 1);
    lookup_or_alloc_14b8(4, spriteId, noParent, 1);
    find_or_alloc_obj_slots(4, spriteId, 0, 0, 0, &found, noParent);

    scene->list = obj_init_empty(heap_alloc2(0x10, 0, gHeapTagSceneList, 0));
    {
        void *window = window_obj_setup(heap_alloc2(0xA4, 0, gHeapTagHudWindow, 0), 1, 2, 0x17, 0xE);
        scene->window = window;
        winPrio = 0x93;
        *(u16 *)((u8 *)window + 0xA2) = winPrio;
    }

    // backgrounds: tiles at VRAM + 0 (BG2, BG3) and + 0x4000 (BG0), maps at + 0x7000 (BG2),
    // + 0x6800 (BG3) and + 0x7800 (BG0, cleared)
    vram_load_packed(gMinigameResultLabels, (void *)VRAM);
    vram_load_packed(gHudScreenMap7000, (void *)(VRAM + 0x7000));
    vram_load_packed(gHudScreenMap6800, (void *)(VRAM + 0x6800));
    vram_load_packed(gSparkleStars, (void *)(VRAM + 0x4000));
    sys = (u8 *)&gSys;
    if (((struct HudModeBits *)&sys[0x888])->mode == 1)
    {
        vram_load_packed(gUnidentifiedDitherStrip0821b851, gUnk_02000080);
        vram_load_packed(gUnk_0821BA44 + 1, gUnk_02000260);
    }
    else
    {
        vram_load_packed(gUnidentifiedDitherStrip0821b735, gUnk_02000080);
        vram_load_packed(gUnk_0821BA20, gUnk_02000260);
    }
    zero = 0;
    cpu_set(&zero, (void *)(VRAM + 0x7800), 0x05000200); // fill 0x200 words
    /* shadows of the display registers, written by address */
    *(u16 *)0x02000008 = 0xF04;  // BG0CNT: priority 0, tiles 0x4000, map 0x7800
    *(u16 *)0x0200000C = 0xE02;  // BG2CNT: priority 2, tiles 0, map 0x7000
    *(u16 *)0x0200000E = 0xD03;  // BG3CNT: priority 3, tiles 0, map 0x6800
    *(u16 *)0x02000000 = 0x1F40; // DISPCNT: mode 0, 1D sprites, BG0-3 and sprites on

    {
        struct HudTask *task = (struct HudTask *)heap_alloc2(0x24, 0, gHeapTagHudTask, 0);
        task_insert(task, 8, (u32)gHeapTagHudTask);
        task->classPtr = gClassTask;
        task->unk20 = 0;
        task->unk1C = 0;
        scene->task = task;
        task->scene = scene;
    }
    scene->unk44 = 0;
    scene->unk48 = 0;

    // the two brothers
    {
        void *list = scene->list;
        struct HudSprite *sprite = heap_alloc2(0x78, 0, gHeapTagHudSpriteA, 0);
        obj_init_big(sprite, 0);
        sprite->classPtr = gClassSpriteA;
        hud_sprite_setup(sprite, 0x1001);
        sprite->unk18 = 0x4800;
        sprite->unk20 = 0x2400;
        sprite->unk6C = 0;
        sprite->unk70 = 0;
        sprite->unk74 = 0;
        scene->spriteA = sprite;
        list_insert(list, sprite);
    }
    {
        void *list = scene->list;
        struct HudSprite *sprite = heap_alloc2(0x78, 0, gHeapTagHudSpriteB, 0);
        obj_init_big(sprite, 1);
        sprite->classPtr = gClassSpriteB;
        hud_sprite_setup(sprite, 0x1002);
        sprite->unk18 = 0x3000;
        sprite->unk20 = 0x3C00;
        sprite->unk6C = 0;
        sprite->unk70 = 0;
        sprite->unk74 = 0;
        scene->spriteB = sprite;
        list_insert(list, sprite);
    }
    {
        void *list = scene->list;
        void *pair = obj_init_sprite_pair(heap_alloc2(0x74, 0, gHeapTagHudPair, 0), scene->list);
        scene->pair = pair;
        list_insert(list, pair);
    }
    {
        void *list = scene->list;
        struct HudSprite *counters = heap_alloc2(0x78, 0, gHeapTagHudCounters, 0);
        obj_init_big(counters, 0xD);
        counters->classPtr = gClassCounter;
        obj_init_counters(counters);
        obj_restore_counter_tiles(counters);
        scene->counters = counters;
        list_insert(list, counters);
    }
    scene->clearBlock = obj_clear(heap_alloc2(0x30, 0, gHeapTagHudCounterB, 0), 0xD);
    scene->hud = obj_create_hud_counter(heap_alloc2(0x110, 0, gHeapTagHudCounterObj, 0), scene->list);
    scene->limit = 9;
    tween_groups_set_flag_bit(0x1EC, 0);
    write_var(0x1CDC, 0);
    ((struct HudCounterObj *)scene->hud)->value = (u16)read_var(0x1CE2);
    if (((struct HudCounterObj *)scene->hud)->value <= 8)
        ((struct HudCounterObj *)scene->hud)->value = 9;
    obj_list_update_pos(scene->list, 0, 0);
    *(u16 *)gUnk_02000080 = 0;
    ((struct SysPalDirty *)&gSys)->palDirty = 0xFFFF; // upload every palette
    return scene;
}

// The slot HUD object's per-frame state machine: 0 reloads every palette and starts song 3 on music
// player 0, 1 waits out the timer, 2-5 run the HUD's stages, 6 waits again, and 7 blacks out the
// palette, stops the music, clears the BLDCNT/BLDY shadows and deletes the object.
void slot_hud_obj_run_state(struct SlotObj *obj)
{
    struct SlotLink *link;
    u8 *sys;
    u16 *blend;

    rand_scaled(0, 2);
    switch (obj->state)
    {
    case 0:
        sys = (u8 *)&gSys;
        *(s32 *)(sys + 0x884) = -1; /* reload every palette */
        set_slot_target(0, 3, -1);
        obj->state = 1;
        /* fall through */
    case 1:
        if (timer_tick(&obj->timer40))
            obj->state = 2;
        break;
    case 2:
        slot_hud_obj_run_stage(obj);
        break;
    case 3:
        obj_phase_slide_a(obj);
        break;
    case 4:
        obj_phase_count_b(obj);
        break;
    case 5:
        slot_hud_obj_state4_if_flag8(obj);
        break;
    case 6:
        if (timer_tick(&obj->timer40))
            obj->state = 7;
        break;
    case 7:
        palette_fill(0);
        slot_reset(0);
        blend = gUnk_02000050; /* BLDCNT shadow, then BLDY */
        *blend = 0;
        blend += 2;
        *blend = 0;
        if (obj == NULL)
            return;
        link = obj->link;
        link->routine((u8 *)obj + link->offset, 3); /* delete */
        return;
    default:
        break;
    }
    obj_copy_pos_to_shadow(obj->shadowSrc);
}

ASM_FUNC("asm/code_081702A4/slot_hud_obj_run_stage.s", void slot_hud_obj_run_stage(struct SlotObj *obj))
