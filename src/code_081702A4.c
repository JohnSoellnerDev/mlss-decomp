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
    u8 filler74[0xD8 - 0x74];
    s32 numD8;
    s32 numDC;
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

struct SlotPair
{
    void *a;
    u8 filler4[0x14];
    void *b;
};

ASM_FUNC("asm/code_081702A4/obj_setup_row_slots_a.s", void obj_setup_row_slots_a(void))


ASM_FUNC("asm/code_081702A4/obj_setup_row_slots_b.s", void obj_setup_row_slots_b(void))

ASM_FUNC("asm/code_081702A4/obj_setup_row_slots_c.s", void obj_setup_row_slots_c(void))

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


ASM_FUNC("asm/code_081702A4/draw_two_numbers.s", void draw_two_numbers(struct SlotObj *obj))

ASM_FUNC("asm/code_081702A4/hud_update_tiles.s", void hud_update_tiles(void))


ASM_FUNC("asm/code_081702A4/obj_reset_hud_tiles.s", void obj_reset_hud_tiles(void))

ASM_FUNC("asm/code_081702A4/obj_update_slot.s", void obj_update_slot(void))

ASM_FUNC("asm/code_081702A4/obj_update_slot_2.s", void obj_update_slot_2(void))

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

NONMATCH("asm/code_081702A4/obj_update_sprite_mode.s", void obj_update_sprite_mode(struct SlotObj *obj, struct SlotOwner *owner, u32 valB, u32 valA))
{
    obj_copy_mode_fields(obj);
    if ((gSys.keys & 1) && !(owner->state & 0x80))
        obj->mode = 5;
    else
        obj->mode = 0;
    if (obj->prevMode != obj->mode)
    {
        if (obj->mode == 0)
        {
            u8 *spr = obj->childA->sprite;
            spr[0x12] = (spr[0x12] & -7) | 2;
            spr = obj->childB->sprite;
            spr[0x12] = (spr[0x12] & -7) | 2;
        }
        else if (obj->mode == 5)
        {
            u8 *spr = obj->childA->sprite;
            spr[0x12] &= -0x11;
            spr = obj->childB->sprite;
            spr[0x12] &= -0x11;
            spr = obj->childA->sprite;
            spr[0x12] &= -7;
            spr = obj->childB->sprite;
            spr[0x12] &= -7;
        }
    }
    obj->childB->unk18 = valB;
    obj->childA->unk20 = valA - 0x500;
}
END_NONMATCH

ASM_FUNC("asm/code_081702A4/scene_build_hud.s", void scene_build_hud(void))

void slot_hud_obj_run_state(struct SlotObj *obj)
{
    struct SlotLink *link;
    u8 *ram;
    u16 *ram16;

    rand_scaled(0, 2);
    switch (obj->state)
    {
    case 0:
        ram = (u8 *)&gSys;
        *(s32 *)(ram + 0x884) = -1;
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
        ram16 = gUnk_02000050;
        *ram16 = 0;
        ram16 += 2;
        *ram16 = 0;
        if (obj == NULL)
            return;
        link = obj->link;
        link->routine((u8 *)obj + link->offset, 3);
        return;
    default:
        break;
    }
    obj_copy_pos_to_shadow(obj->shadowSrc);
}

ASM_FUNC("asm/code_081702A4/slot_hud_obj_run_stage.s", void slot_hud_obj_run_stage(struct SlotObj *obj))
