#include "global.h"

struct EffectObj
{
    u32 unk0;
    u32 unk4;
    u8 filler8[0x4];
    u32 unkC;
    u8 filler10[0x4];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u8 filler2C[0x8];
    u32 unk34;
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u8 filler44[0x8];
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u32 unk58;
    u8 filler5C[0xC];
    u16 unk68;
    u8 filler6A[0x2];
    u32 unk6C;
    u32 unk70;
    u32 unk74;
    u32 unk78;
    u32 unk7C;
    u8 filler80[0x8];
    u32 unk88;
};

extern const u8 gParticleClass[];
extern const u8 gStackClass[];
extern const u8 gPairClass[];
extern const u8 gPairPartClass[];
extern const u8 gPartClass[];
extern const u8 gStageClass[];
extern const s8 gPuffBaseTable[];
extern void *gUnk_03000D48;
extern void *gUnk_03000D4C;
extern u8 gUnk_0203FFB8[];
struct KeyRam
{
    u8 filler0[0x2A];
    u16 held;
};

extern struct KeyRam gUnk_0300034C;

void obj_destroy_sprites(void *obj, u32 arg);
void task_remove(void *node, u32 kind);
void task_insert(void *node, u8 kind);
void heap_free_b(void *p);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_list_pass_0a8c(void);
u8 obj_is_done(void *obj);
void *obj_init_big(void *obj, u32 first);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
u32 rand_scaled(u32 count, u32 base);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void list_insert(void *list, void *node);
void obj_update_anim_pair(struct EffectObj *obj, u32 a, u32 b);
void obj_set_wait_60(struct EffectObj *obj);
u8 obj_is_idle_081642B0(u32 anim);
void obj_start_tween(u32 anim, u32 a, u32 b, u32 c, u32 d);
void obj_tween_step(u32 anim);
void free_if_bit0(void *ptr, u32 flags);
void free_if_bit0_b(void *ptr, u32 flags);
void window_destroy(void *window, u32 flags);
void obj_run_callbacks(void);
void set_flags_2_3(u32 hook);

struct ClassA
{
    u8 filler0[0x10];
    s16 thisOffset;
    u16 filler12;
    void (*update)(u32, u32);
};

struct ClassB
{
    u8 filler0[0x8];
    s16 thisOffset;
    u16 filler8;
    void (*update)(u32, u32);
};

// Halves a value, rounding toward zero.
static inline s32 div2_trunc(s32 v)
{
    u32 sign = (u32)v >> 31;
    return (s32)(v + sign) >> 1;
}

void obj_tween_state(struct EffectObj *obj)
{
    u16 phase = obj->unk68;

    switch (phase)
    {
    case 0:
        obj_start_tween(obj->unk40, 0, 0, 0x14, phase);
        obj->unk68++;
        break;
    case 1:
        if (obj_is_idle_081642B0(obj->unk40))
        {
            obj->unk68++;
        }
        break;
    case 2:
        if ((gUnk_0300034C.held & 8) != 0)
        {
            set_slot_value(0x37, -1);
            obj_start_tween(obj->unk40, 0, 0xFFFF6000, 0x14, 0);
            obj->unk68++;
        }
        break;
    case 3:
        if (obj_is_idle_081642B0(obj->unk40))
        {
            obj->unk54 = obj->unk58;
        }
        break;
    }
    obj_tween_step(obj->unk40);
}

struct FrameList
{
    u8 filler0[0x3C];
    u32 owner;
};

void obj_frame_callback(void)
{
    struct FrameList *list;
    void **hook;
    u8 *base;
    u32 owner;

    obj_list_pass_0a8c();
    ((void (*)(u32, u32))gUnk_03000D48)((list = (struct FrameList *)gUnk_0203FFB8)->owner,
        list->owner + 0xA14);
    hook = &gUnk_03000D4C;
    owner = list->owner;
    base = (u8 *)&gUnk_0300034C;
    *(u32 *)(base + 0x880) = ((u32 (*)(u32, u32))*hook)(owner, *(u32 *)(base + 0x880));
}

void obj_destroy_parts(struct EffectObj *obj, u32 flags)
{
    struct EffectObj *owner;
    struct EffectObj *sprite;
    struct ClassA *cls;
    struct ClassB *sprite_cls;

    obj->unk18 = (u32)gStageClass;
    if (obj->unk3C != 0)
    {
        heap_free_b((void *)obj->unk3C);
    }
    if (obj->unk38 != 0)
    {
        heap_free_b((void *)obj->unk38);
    }
    owner = (struct EffectObj *)obj->unk20;
    if (owner != 0)
    {
        cls = (struct ClassA *)owner->unk18;
        cls->update((u32)owner + cls->thisOffset, 3);
    }
    if (obj->unk40 != 0)
    {
        free_if_bit0((void *)obj->unk40, 3);
    }
    if (obj->unk28 != 0)
    {
        window_destroy((void *)obj->unk28, 3);
    }
    sprite = (struct EffectObj *)obj->unk1C;
    if (sprite != 0)
    {
        sprite_cls = (struct ClassB *)sprite->unkC;
        sprite_cls->update((u32)sprite + sprite_cls->thisOffset, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(obj->unk14);
    free_if_bit0_b((void *)&obj->unk4C, 2);
    task_remove(obj, flags);
}

void obj_destroy_ec08(struct EffectObj *obj, u32 arg)
{
    obj->unkC = (u32)gParticleClass;
    obj_destroy_sprites(obj, arg);
}

struct EffectObj *init_vt_08cdec08(struct EffectObj *obj, u32 a)
{
    obj_init_big(obj, 6);
    obj->unkC = (u32)gParticleClass;
    obj->unk74 = a;
    obj->unk78 = 0;
    obj_create_sprite_10(obj, 4, 0x5007, 0, a + 5, 2);
    obj->unk7C = (s32)gPuffBaseTable[obj->unk74] << 8;
    obj->unk18 = rand_scaled(0x58, 0x98) << 8;
    obj->unk1C = 0xFFFFF800;
    obj->unk20 = 0;
    obj->unk6C = rand_scaled(0xFFFFFF00, 0x100);
    obj->unk70 = 0;
    return obj;
}

u32 obj_flag78_test(struct EffectObj *obj)
{
    if (obj->unk78 == 1)
    {
        return 1;
    }
    return 0;
}

u32 sub_0816E780(struct EffectObj *obj)
{
    if (obj->unk78 == 0)
    {
        return 1;
    }
    return 0;
}

void obj_destroy_ec18(struct EffectObj *obj, u32 arg)
{
    obj->unkC = (u32)gStackClass;
    obj_destroy_sprites(obj, arg);
}

void obj_clear_flag88_slot97(struct EffectObj *obj)
{
    if (obj->unk88 != 0)
    {
        obj->unk88 = 0;
        sound_stop_song(0x97);
    }
}

void obj_set_flag88_slot97(struct EffectObj *obj)
{
    if (obj->unk88 == 0)
    {
        obj->unk88 = 1;
        set_slot_value(0x97, -1);
    }
}

void obj_destroy_ec28(struct EffectObj *obj, u32 arg)
{
    obj->unkC = (u32)gPairClass;
    obj_destroy_sprites(obj, arg);
}

void obj_pos_copy_18_1c(struct EffectObj *obj)
{
    obj->unk18 = obj->unk74;
    obj->unk1C = obj->unk78 + 0x2000;
}

void obj_step_bounce(struct EffectObj *obj)
{
    s32 pos = obj->unk78;
    s32 vel = obj->unk70;

    pos += vel;
    obj->unk78 = pos;
    if (pos <= 0x57FF)
    {
        obj->unk78 = 0x5800;
        obj->unk70 = div2_trunc(-vel);
    }
    if ((s32)obj->unk78 > 0x7800)
    {
        obj->unk78 = 0x7800;
        obj->unk70 = div2_trunc(-(s32)obj->unk70);
    }
}

void obj_step70_add20_sub20(struct EffectObj *obj)
{
    obj->unk70 += 0x20;
    if ((s32)obj->unk70 > 0x100)
    {
        obj->unk70 = 0x100;
    }
    obj_step_bounce(obj);
}

void sub_0816E86C(struct EffectObj *obj)
{
    obj->unk70 -= 0x20;
    if ((s32)obj->unk70 < -0x200)
    {
        obj->unk70 = -0x200;
    }
    obj_step_bounce(obj);
}

void obj_destroy_ec38(struct EffectObj *obj, u32 arg)
{
    obj->unkC = (u32)gPairPartClass;
    obj_destroy_sprites(obj, arg);
}

struct EffectObj *init_vt_08cdec38(struct EffectObj *obj, u32 a, u32 b)
{
    obj_init_big(obj, 1);
    obj->unkC = (u32)gPairPartClass;
    obj_create_sprite_10(obj, 4, 0x5007, 0, a, 2);
    obj->unk70 = 0;
    obj->unk6C = 0;
    obj->unk74 = b;
    obj->unk78 = 0x7800;
    return obj;
}

void obj_clear_class_flag50(struct EffectObj *obj)
{
    obj->unk18 = 0;
    obj->unk50 = 0;
}

void obj_timer_clamp_18(struct EffectObj *obj, s32 delta)
{
    obj->unk18 += delta;
    if ((s32)obj->unk18 < 0)
    {
        obj->unk18 = 0;
    }
    if ((s32)obj->unk18 > 0x3E7)
    {
        obj->unk18 = 0x3E7;
    }
    if ((s32)obj->unk34 < (s32)obj->unk18)
    {
        obj->unk34 = obj->unk18;
        obj->unk50 = 1;
    }
}

void obj_free_flag_e924(void *p, u32 flags)
{
    if (flags & 1)
    {
        heap_free_b(p);
    }
}

u32 sub_0816E93C(struct EffectObj *obj)
{
    if (obj->unk18 == 0)
    {
        if (obj_is_done((void *)obj->unk0))
        {
            if (obj_is_done((void *)obj->unk4))
            {
                return 1;
            }
        }
    }
    return 0;
}

void obj_set_wait_60(struct EffectObj *obj)
{
    obj->unk18 = 0x3C;
    obj->unk1C = 0;
}

void obj_free_flag_e976(void *p, u32 flags)
{
    if (flags & 1)
    {
        heap_free_b(p);
    }
}

ASM_FUNC("asm/code_0816E57C/obj_init_pair_e990.s", void obj_init_pair_e990(void))

void obj_remove_ebf0(struct EffectObj *obj, u32 flags)
{
    obj->unk18 = (u32)gPartClass;
    task_remove(obj, flags);
}

struct EffectObj *init_vt_08cdebf0(struct EffectObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = (u32)gPartClass;
    obj->unk20 = 0;
    obj->unk1C = 0;
    return obj;
}

ASM_FUNC("asm/code_0816E57C/obj_draw_counter.s", void obj_draw_counter(void))

ASM_FUNC("asm/code_0816E57C/obj_restore_counter_tiles.s", void obj_restore_counter_tiles(void))

ASM_FUNC("asm/code_0816E57C/obj_create_hud_counter.s", void obj_create_hud_counter(void))

ASM_FUNC("asm/code_0816E57C/obj_build_jitter_table.s", void obj_build_jitter_table(void))

ASM_FUNC("asm/code_0816E57C/obj_setup_slots_6x3.s", void obj_setup_slots_6x3(void))

ASM_FUNC("asm/code_0816E57C/obj_fill_row_shuffled.s", void obj_fill_row_shuffled(void))

ASM_FUNC("asm/code_0816E57C/obj_fill_col_shuffled.s", void obj_fill_col_shuffled(void))

ASM_FUNC("asm/code_0816E57C/owner_update_children.s", void owner_update_children(void))
