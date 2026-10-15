#include "global.h"
#include "ram_routines.h"

struct Timer
{
    u16 limit;
    u16 counter;
};

struct TweenObj
{
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 filler22[2];
    u32 unk24;
    u32 unk28;
    u32 unk2C;
};

struct BigObj
{
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
    u16 unk4E;
    u16 unk50;
    u16 unk52;
    u16 unk54;
    u16 unk56;
    u16 unk58;
    u16 unk5A;
    u32 unk5C;
    u32 unk60;
    u32 unk64;
    u32 unk68;
};

extern u8 *gUnk_03000FC0;
extern u8 gClassEac8[];
extern u8 gBigObjTag[];
void heap_free_b(void *p);
void release_obj_131c(void *p);

ASM_FUNC("asm/code_08162C68/effect_step_d.s", void effect_step_d(void))

void obj_release_child_free_c(void *obj, u32 freeFlag)
{
    release_obj_131c(*(void **)obj);
    if (freeFlag & 1)
    {
        heap_free_b(obj);
    }
}

struct BigObj *obj_init_big(struct BigObj *obj, u32 first)
{
    u32 *cursor;
    u16 *half;

    obj->unk8 = 0;
    obj->unk4 = 0;
    obj->unk0 = first;
    obj->unkC = (u32)gClassEac8;
    obj->unk40 = 1;
    obj->unk14 = 0;
    obj->unk10 = 0;
    cursor = &obj->unk18;
    obj->unk18 = 0;
    cursor[1] = 0;
    cursor[2] = 0;
    cursor += 3;
    obj->unk24 = 0;
    cursor[1] = 0;
    cursor[2] = 0;
    cursor += 3;
    obj->unk30 = 0;
    cursor[1] = 0;
    cursor[2] = 0;
    obj->unk3C = 0;
    obj->unk44 = 0;
    half = (u16 *)(cursor + 6);
    half[0] = 0;
    half += 3;
    half[0] = 0;
    half -= 1;
    half[0] = 0;
    half -= 1;
    half[0] = 0;
    half += 3;
    half[0] = 0;
    half += 2;
    half[0] = 0;
    half -= 1;
    half[0] = 0;
    half += 3;
    half[0] = 0;
    half -= 1;
    half[0] = 0;
    half += 2;
    half[0] = 0;
    obj->unk60 = 0;
    obj->unk5C = 0;
    obj->unk64 = 0;
    obj->unk68 = (u32)gBigObjTag;
    return obj;
}

ASM_FUNC("asm/code_08162C68/update_obj_sprite_pos.s", void update_obj_sprite_pos(void))

ASM_FUNC("asm/code_08162C68/obj_set_anim_from_offset.s", void obj_set_anim_from_offset(struct BigObj *obj))

ASM_FUNC("asm/code_08162C68/obj_update_frame_flags.s", void obj_update_frame_flags(void))

ASM_FUNC("asm/code_08162C68/obj_script_step.s", void obj_script_step(void))

ASM_FUNC("asm/code_08162C68/tween_groups_obj_start_move.s", void tween_groups_obj_start_move(void))

ASM_FUNC("asm/code_08162C68/tween_groups_obj_step_move.s", void tween_groups_obj_step_move(void))

ASM_FUNC("asm/code_08162C68/create_obj_group_5005.s", void create_obj_group_5005(void))

ASM_FUNC("asm/code_08162C68/obj_init_pair.s", void obj_init_pair(void))

ASM_FUNC("asm/code_08162C68/create_obj_group_5001.s", void create_obj_group_5001(void))

ASM_FUNC("asm/code_08162C68/init_obj_table_classes.s", void init_obj_table_classes(void))

ASM_FUNC("asm/code_08162C68/create_obj_group_500b.s", void create_obj_group_500b(void))

ASM_FUNC("asm/code_08162C68/obj_init_children_classes.s", void obj_init_children_classes(void))

ASM_FUNC("asm/code_08162C68/obj_tween_step.s", void obj_tween_step(void))

u32 *flag_word_ptr(u16 index)
{
    return (u32 *)(gUnk_03000FC0 + 0x20 + ((index >> 5) << 2));
}

u32 get_flag_bit(u32 flagId)
{
    u32 packed = flagId << 16;
    u32 bit = (packed & 0x1F0000) >> 16;
    u8 *base = gUnk_03000FC0;
    u32 index = (packed >> 21) << 2;
    u32 *word;
    base += 0x20;
    word = (u32 *)(base + index);
    return (*word >> bit) & 1;
}

ASM_FUNC("asm/code_08162C68/tween_groups_set_flag_bit.s", void tween_groups_set_flag_bit(void))

ASM_FUNC("asm/code_08162C68/obj_copy_pos_to_shadow.s", void obj_copy_pos_to_shadow(void))

u32 obj_is_idle_081642B0(struct TweenObj *obj)
{
    u32 idle = 0;
    if (obj->unk24 == 0)
    {
        idle = 1;
    }
    return idle;
}

void obj_start_tween_len(struct TweenObj *obj, u32 x, u32 y)
{
    s32 dx;
    s32 dy;
    s32 len;

    obj->unk10 = obj->unk0;
    obj->unk14 = obj->unk4;
    obj->unk8 = x;
    obj->unkC = y;
    dx = ((s32)obj->unk0 - (s32)x) >> 8;
    dy = ((s32)obj->unk4 - (s32)y) >> 8;
    len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(dx * dx + dy * dy);
    obj->unk28 = len;
    if (len < 0)
    {
        obj->unk28 = 1;
    }
    obj->unk2C = 0;
    obj->unk24 = 2;
}

void obj_start_tween(struct TweenObj *obj, u32 x, u32 y, u32 span, u32 phase2)
{
    u32 phase;

    obj->unk10 = obj->unk0;
    obj->unk14 = obj->unk4;
    obj->unk8 = x;
    obj->unkC = y;
    obj->unk28 = span;
    obj->unk2C = 0;
    if (phase2 == 0)
    {
        obj->unk24 = 1;
    }
    else
    {
        obj->unk24 = 2;
    }
}

void obj_set_pos(struct TweenObj *obj, u32 x, u32 y)
{
    obj->unk10 = x;
    obj->unk8 = x;
    obj->unk0 = x;
    obj->unk14 = y;
    obj->unkC = y;
    obj->unk4 = y;
    obj->unk18 = (s32)x >> 8;
    obj->unk1C = (s32)y >> 8;
    obj->unk24 = 0;
}

void free_if_bit0(void *ptr, u32 flags)
{
    if (flags & 1)
    {
        heap_free_b(ptr);
    }
}

struct TweenObj *obj_clear(struct TweenObj *obj, u16 flags)
{
    obj->unk20 = flags;
    obj->unk24 = 0;
    obj->unk1C = 0;
    obj->unk18 = 0;
    obj->unk14 = 0;
    obj->unk10 = 0;
    obj->unkC = 0;
    obj->unk8 = 0;
    obj->unk4 = 0;
    obj->unk0 = 0;
    obj->unk2C = 0;
    obj->unk28 = 0;
    return obj;
}

u32 cmp_u16_pair_ge(struct Timer *p)
{
    u32 result = 0;
    if (p->counter >= p->limit)
    {
        result = 1;
    }
    return result;
}

u32 timer_tick(struct Timer *timer)
{
    u16 counter = timer->counter;
    u16 limit = timer->limit;
    u32 next;
    if (counter >= limit)
    {
        return 1;
    }
    next = counter + 1;
    timer->counter = next;
    if ((u16)next >= limit)
    {
        return 1;
    }
    return 0;
}

void timer_set(struct Timer *timer, u32 limit)
{
    u16 value = limit;
    timer->limit = value;
    if (value == 0)
    {
        timer->limit = 1;
    }
    timer->counter = 0;
}

void free_if_bit0_b(void *ptr, u32 flags)
{
    if (flags & 1)
    {
        heap_free_b(ptr);
    }
}

void timer_init_default(struct Timer *timer)
{
    timer->limit = 0x10;
    timer->counter = 0;
}

ASM_FUNC("asm/code_08162C68/obj_children_done.s", void obj_children_done(void))
