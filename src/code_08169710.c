#include "global.h"

struct SubObj
{
    u8 filler0[0xE];
    u16 unkE;
};

struct SpriteObj
{
    u8 filler0[0x10];
    struct SubObj *unk10;
    u8 filler14[4];
    u32 unk18;
    u32 unk1C;
    u8 filler20[0x56 - 0x20];
    u16 unk56;
    u16 unk58;
};

struct CounterGroup
{
    s32 unk0;
    s32 unk4;
    s32 unk8;
    struct SpriteObj *unkC;
    struct SpriteObj *unk10;
};

struct VtA
{
    u8 filler0[8];
    s16 thisOff;
    u16 filler_a;
    void (*fn)(void *, u32);
};

struct VtB
{
    u8 filler0[0x10];
    s16 thisOff;
    u16 filler_b;
    void (*fn)(void *, u32);
};

struct ObjA
{
    u8 filler0[0xC];
    struct VtA *vt;
};

struct ObjB
{
    u8 filler0[0x18];
    struct VtB *vt;
};

struct HolderObj
{
    u8 filler0[0xC];
    struct ObjA *unkC;
    struct ObjA *unk10;
};

struct DestroyObj
{
    u8 filler0[0x14];
    void *unk14;
    void *unk18;
    struct ObjA *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    struct ObjB *unk2C;
    void *unk30;
    u8 filler34[4];
    void *unk38;
    struct HolderObj *unk3C;
    u8 filler40[0x6C - 0x40];
    void *unk6C;
    u32 unk70;
};

struct ProjObj
{
    u8 filler0[0xC];
    void *unkC;
    u8 filler10[0x6C - 0x10];
    u32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u32 unk7C;
    s32 unk80;
};

struct PosXY
{
    s32 x;
    u8 filler4[4];
    s32 y;
};

extern s16 gSineTable[];
extern s16 gCosineTable[];
extern const u8 gClassEbb8[];
u32 rand_scaled(u32 count, u32 base);
u32 arc_tan2(s32 x, s32 y);
void obj_set_fields_18(void *obj, s32 x, s32 y, s32 z);

struct StateObj
{
    u8 filler0[0x18];
    struct VtB *unk18;
    void *unk1C;
    void *unk20;
    void *unk24;
    u8 filler28[8];
    void *unk30;
    u8 filler34[0x70 - 0x34];
    s32 unk70;
    u8 filler74[0x474 - 0x74];
    u32 unk474;
    u8 filler478[0x484 - 0x478];
    u16 unk484;
};

extern u16 gUnk_02000050[];
void palette_fill(u32 v);
void slot_reset(u32 id);
void set_slot_target(u32 a, u32 b);
void write_var(u16 id, u16 value);
u8 timer_tick(void *timer);
void obj_copy_pos_to_shadow(void *src);
void grid_lines_obj_step_motion(void *obj, void *region, void *list);
void child_states_obj_run_stage(void *obj);
void obj_run_phases(void *obj);
void obj_run_substates(void *obj);
void child_states_obj_state4_if_flag8(void *obj);
void obj_update_children(void *obj);


extern const u8 gClassEb68[];
void set_irq_callback(u32 kind, void *cb);
void free_if_bit0(void *ptr, u32 flags);
void free_if_bit0_b(void *ptr, u32 flags);
void heap_free_b(void *p);
void obj_run_children_free(void *obj, u32 flags);
void obj_run_children3_free(void *obj, u32 flags);
void window_destroy(void *win, u32 flags);
void obj_run_callbacks(void);
void set_flags_2_3(void *p);
void task_remove(void *node, u32 kind);

extern const u8 gHeapTagProjectile[];
extern const u8 gHeapTagCounterB[];
extern const s16 gCounterSpriteRecordByLang[];
extern u8 gUnk_0300034C[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *obj_init_big(void *obj, u32 first);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_counter_step(struct CounterGroup *group);
void obj_copy_mode_fields(void *obj);
void update_obj_sprite_pos(void *obj, u32 camX, u32 camY);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
s32 lerp_step_08164EE0(s32 a, s32 b, s32 steps, s32 t);

ASM_FUNC("asm/code_08169710/grid_lines_obj_step_motion.s", void grid_lines_obj_step_motion(void *obj, void *region, void *list))

struct ProjObj *obj_init_projectile(struct ProjObj *obj, struct PosXY *src, struct PosXY *dst, u32 mode)
{
    s32 dx;
    s32 dy;
    u32 angle;
    s32 sinV;
    s32 cosV;

    obj_init_big(obj, 5);
    obj->unkC = (void *)gClassEbb8;
    dx = (s32)((u32)(dst->x - src->x) << 8) >> 16;
    dy = (s32)((u32)(dst->y - src->y) << 8) >> 16;
    angle = ((u32)arc_tan2(dx, dy) << 16) >> 24;
    obj->unk7C = angle;
    sinV = gSineTable[angle] / 0x40;
    cosV = gCosineTable[angle] / 0x40;
    obj->unk74 = 0;
    obj->unk80 = 0x78;
    if (mode != 0)
    {
        obj_create_sprite_10(obj, 4, 0x5006, 0, 7, 2);
        obj->unk6C = 1;
        obj->unk70 = (cosV * 9) >> 3;
        obj->unk78 = (sinV * 9) >> 3;
        obj_set_fields_18(obj, src->x + obj->unk70, 0, src->y + obj->unk78);
    }
    else
    {
        obj_create_sprite_10(obj, 4, 0x5006, 0, 9, 2);
        obj->unk6C = mode;
        obj->unk70 = (cosV * 0xD0) >> 8;
        obj->unk78 = (sinV * 0xD0) >> 8;
    }
    return obj;
}

ASM_FUNC("asm/code_08169710/obj_line_bounce_step.s", void obj_line_bounce_step(void))

ASM_FUNC("asm/code_08169710/obj_check_contact.s", void obj_check_contact(void))
ASM_FUNC("asm/code_08169710/obj_init_bullet.s", void obj_init_bullet(void))

ASM_FUNC("asm/code_08169710/obj_node_update.s", void obj_node_update(void))

struct CounterGroup *obj_init_counter(struct CounterGroup *group)
{
    group->unkC = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagProjectile, 0), 0);
    obj_create_sprite_10(group->unkC, 4, 0x5006, 0, 0xE, 2);
    group->unkC->unk1C = 0xA000;
    group->unk10 = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagCounterB, 0), 0);
    obj_create_sprite_10(group->unk10, 4, 0x5006, 0, gCounterSpriteRecordByLang[gUnk_0300034C[0x8C0]], 2);
    group->unk10->unk18 = 0xE000;
    group->unk10->unk1C = 0xA000;
    group->unk8 = 0;
    group->unk4 = 0;
    group->unk0 = 0x1C20;
    obj_counter_step(group);
    return group;
}

void obj_counter_step(struct CounterGroup *group)
{
    s32 rem;
    s32 cur;
    u16 *modeAddr;
    u16 mode;

    obj_copy_mode_fields(group->unkC);
    cur = group->unk4;
    if (cur < group->unk0)
    {
        rem = group->unk8;
        if (rem != 0)
        {
            group->unk8 = rem - 1;
            group->unk4 = cur + 4;
            modeAddr = &group->unkC->unk56;
            rem = 1;
        }
        else
        {
            group->unk4 = cur + 1;
            modeAddr = &group->unkC->unk56;
        }
        *modeAddr = rem;
        group->unkC->unk18 = lerp_step_08164EE0(0x1000, 0xE000, group->unk0, group->unk4);
    }
    else
    {
        group->unkC->unk56 = 0;
    }
    if (group->unkC->unk56 != group->unkC->unk58)
    {
        mode = group->unkC->unk56;
        switch (mode)
        {
        case 0:
            obj_load_record(group->unkC->unk10, 0xE, 0, 0, mode);
            break;
        case 1:
            obj_load_record(group->unkC->unk10, 0x14, 0, 0, 0);
            break;
        }
    }
    update_obj_sprite_pos(group->unkC, 0, 0);
    group->unkC->unk10->unkE = 0;
    update_obj_sprite_pos(group->unk10, 0, 0);
    group->unk10->unk10->unkE = 4;
}

ASM_FUNC("asm/code_08169710/obj_clamp_pos.s", void obj_clamp_pos(void *region, void *a, void *b))

ASM_FUNC("asm/code_08169710/obj_init_screen.s", void obj_init_screen(void))

void obj_destroy_all(struct DestroyObj *obj, u32 freeMem)
{
    struct HolderObj *holder;
    struct ObjA *sub;
    struct ObjB *subB;

    obj->unk18 = (void *)gClassEb68;
    set_irq_callback(2, 0);
    REG_DMA0CNT_H = 0;
    if (obj->unk30 != 0)
    {
        free_if_bit0(obj->unk30, 3);
    }
    if (obj->unk20 != 0)
    {
        heap_free_b(obj->unk20);
    }
    if (obj->unk6C != 0)
    {
        obj_run_children_free(obj->unk6C, 3);
    }
    holder = obj->unk3C;
    if (holder != 0)
    {
        if (holder->unk10 != 0)
        {
            holder->unk10->vt->fn((u8 *)holder->unk10 + holder->unk10->vt->thisOff, 3);
        }
        if (holder->unkC != 0)
        {
            holder->unkC->vt->fn((u8 *)holder->unkC + holder->unkC->vt->thisOff, 3);
        }
        heap_free_b(holder);
    }
    if (obj->unk24 != 0)
    {
        heap_free_b(obj->unk24);
    }
    if (obj->unk28 != 0)
    {
        obj_run_children3_free(obj->unk28, 3);
    }
    subB = obj->unk2C;
    if (subB != 0)
    {
        subB->vt->fn((u8 *)subB + subB->vt->thisOff, 3);
    }
    if (obj->unk38 != 0)
    {
        window_destroy(obj->unk38, 3);
    }
    sub = obj->unk1C;
    if (sub != 0)
    {
        sub->vt->fn((u8 *)sub + sub->vt->thisOff, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(obj->unk14);
    free_if_bit0_b(&obj->unk70, 2);
    task_remove(obj, freeMem);
}

NONMATCH("asm/code_08169710/child_states_obj_run_state.s", void child_states_obj_run_state(struct StateObj *obj))
{
    rand_scaled(0, 2);
    switch (obj->unk474)
    {
    case 0:
        {
            u8 *slot = gUnk_0300034C;
            *(s32 *)(slot + 0x884) = -1;
        }
        set_slot_target(0, 3);
        obj->unk474 = 1;
    case 1:
        if (timer_tick(&obj->unk70))
        {
            obj->unk474 = 2;
        }
        else
        {
            grid_lines_obj_step_motion(obj->unk20, obj->unk24, obj->unk1C);
        }
        break;
    case 2:
        child_states_obj_run_stage(obj);
        break;
    case 3:
        obj_run_phases(obj);
        break;
    case 4:
        obj_run_substates(obj);
        break;
    case 5:
        child_states_obj_state4_if_flag8(obj);
        break;
    case 6:
        if (timer_tick(&obj->unk70))
        {
            obj->unk474 = 7;
        }
        break;
    case 7:
        palette_fill(0);
        slot_reset(0);
        write_var(0x1CE1, obj->unk484);
        {
            u16 *flag = gUnk_02000050;
            *flag = 0;
            flag += 2;
            *flag = 0;
        }
        if (obj != 0)
        {
            obj->unk18->fn((u8 *)obj + obj->unk18->thisOff, 3);
        }
        return;
    }
    obj_copy_pos_to_shadow(obj->unk30);
    obj_update_children(obj);
}
END_NONMATCH
