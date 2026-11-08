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

struct HudSprite
{
    u8 filler0[0xC];
    const void *classPtr;
    u8 filler10[0x18 - 0x10];
    u32 unk18;
    u8 filler1C[0x20 - 0x1C];
    u32 unk20;
    u8 filler24[0x52 - 0x24];
    u16 unk52;
    u8 filler54[0x6C - 0x54];
    u32 unk6C;
    u32 unk70;
    u16 unk74;
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

NONMATCH("asm/code_081702A4/scene_build_hud.s", struct HudScene *scene_build_hud(struct HudScene *scene, u8 prio, u32 taskArg))
{
    u32 found;
    u32 cpuZero;
    s32 noParent;
    s32 limit;
    u32 slot;
    u32 spriteType;
    s32 value;
    void *list;
    void *block;
    void *window;
    void *sprite;
    void *pair;
    void *counters;
    struct HudTask *task;
    void *hud;
    u32 *counter;
    u16 *shadow;

    task_insert(scene, prio, taskArg);
    scene->classPtr = gClassScene;
    timer_init_default(&scene->timer40);
    clear_gate_flags();
    timer_set(&scene->timer40, 0x20);
    block = heap_alloc2(0x24, 0, gAnim9fc0, 0);
    anim_task_start(block, 8, gAnim9fc0, 0, 0x20);
        scene->flags50 = 0;
    if (get_flag_bit(0x1F2) == 0)
    {
        tween_groups_set_flag_bit(0x1F2, 1);
        scene->flags50 |= 0x11;
    }
    if (get_flag_bit(0x1F3) != 0)
        scene->flags50 |= 8;

    noParent = -1;
    slot = 0x1001;
    lookup_or_alloc_13b4(0, slot, noParent, 1);
    lookup_or_alloc_14b8(0, slot, noParent, 1);
    find_or_alloc_obj_slots(0, slot, 0, 0, 0, &found, noParent);
    slot++;
    lookup_or_alloc_13b4(0, slot, noParent, 1);
    lookup_or_alloc_14b8(0, slot, noParent, 1);
    find_or_alloc_obj_slots(0, slot, 0, 0, 0, &found, noParent);
    slot += 0x5D;
    lookup_or_alloc_13b4(0, slot, noParent, 1);
    lookup_or_alloc_14b8(0, slot, noParent, 1);
    slot += 0x1E;
    lookup_or_alloc_13b4(0, slot, noParent, 1);
    lookup_or_alloc_14b8(0, slot, noParent, 1);
    slot -= 0x20;
    lookup_or_alloc_13b4(0, slot, noParent, 1);
    lookup_or_alloc_14b8(0, slot, noParent, 1);
    slot += 0x1E;
    lookup_or_alloc_13b4(0, slot, noParent, 1);
    lookup_or_alloc_14b8(0, slot, noParent, 1);
    slot = 0x5009;
    lookup_or_alloc_13b4(4, slot, noParent, 1);
    lookup_or_alloc_14b8(4, slot, noParent, 1);
    find_or_alloc_obj_slots(4, slot, 0, 0, 0, &found, noParent);

    scene->list = obj_init_empty(heap_alloc2(0x10, 0, gHeapTagSceneList, 0));
    window = window_obj_setup(heap_alloc2(0xA4, 0, gHeapTagHudWindow, 0), 1, 2, 0x17, 0xE);
    scene->window = window;
    *(u16 *)((u8 *)window + 0xA2) = 0x93;
    vram_load_packed(gMinigameResultLabels, (void *)0x06000000);
    vram_load_packed(gHudScreenMap7000, (void *)0x06007000);
    vram_load_packed(gHudScreenMap6800, (void *)0x06006800);
    vram_load_packed(gSparkleStars, (void *)0x06004000);
    if ((((u8 *)&gSys)[0x888] & 6) == 2)
    {
        vram_load_packed(gUnidentifiedDitherStrip0821b851, gUnk_02000080);
        vram_load_packed(gUnk_0821BA44 + 1, gUnk_02000260);
    }
    else
    {
        vram_load_packed(gUnidentifiedDitherStrip0821b735, gUnk_02000080);
        vram_load_packed(gUnk_0821BA20, gUnk_02000260);
    }
    cpuZero = 0;
    cpu_set(&cpuZero, (void *)0x06007800, 0x05000200);
    shadow = (u16 *)gUnk_02000008;
    *shadow = 0xF04;
    shadow += 2;
    *shadow = 0xE02;
    shadow += 1;
    *shadow = 0xD03;
    shadow -= 7;
    *shadow = 0x1F40;

    task = (struct HudTask *)heap_alloc2(0x24, 0, gHeapTagHudTask, 0);
    task_insert(task, 8, (u32)gHeapTagHudTask);
    task->classPtr = gClassTask;
    task->unk20 = 0;
    task->unk1C = 0;
    scene->task = task;
    task->scene = scene;
    scene->unk44 = 0;
    scene->unk48 = 0;

    list = scene->list;
    sprite = heap_alloc2(0x78, 0, gHeapTagHudSpriteA, 0);
    obj_init_big(sprite, 0);
    ((struct HudSprite *)sprite)->classPtr = gClassSpriteA;
    spriteType = 3;
    obj_create_sprite_10(sprite, 0, 0x1001, 1, 0, spriteType);
    obj_set_frame_table(sprite, gSpriteFrameBaseByMode);
    ((struct HudSprite *)sprite)->unk52 = 2;
    obj_update_frame_flags(sprite);
    ((struct HudSprite *)sprite)->unk18 = 0x4800;
    ((struct HudSprite *)sprite)->unk20 = 0x2400;
    ((struct HudSprite *)sprite)->unk6C = 0;
    ((struct HudSprite *)sprite)->unk70 = 0;
    ((struct HudSprite *)sprite)->unk74 = 0;
    scene->spriteA = sprite;
    list_insert(list, sprite);

    list = scene->list;
    sprite = heap_alloc2(0x78, 0, gHeapTagHudSpriteB, 0);
    obj_init_big(sprite, 1);
    ((struct HudSprite *)sprite)->classPtr = gClassSpriteB;
    obj_create_sprite_10(sprite, 0, 0x1002, 1, 0, spriteType);
    obj_set_frame_table(sprite, gSpriteFrameBaseByMode);
    ((struct HudSprite *)sprite)->unk52 = 2;
    obj_update_frame_flags(sprite);
    ((struct HudSprite *)sprite)->unk18 = 0x3000;
    ((struct HudSprite *)sprite)->unk20 = 0x3C00;
    ((struct HudSprite *)sprite)->unk6C = 0;
    ((struct HudSprite *)sprite)->unk70 = 0;
    ((struct HudSprite *)sprite)->unk74 = 0;
    scene->spriteB = sprite;
    list_insert(list, sprite);

    list = scene->list;
    pair = obj_init_sprite_pair(heap_alloc2(0x74, 0, gHeapTagHudPair, 0), scene->list);
    scene->pair = pair;
    list_insert(list, pair);

    list = scene->list;
    counters = heap_alloc2(0x78, 0, gHeapTagHudCounters, 0);
    obj_init_big(counters, 0xD);
    *(const void **)((u8 *)counters + 0xC) = gClassCounter;
    obj_init_counters(counters);
    obj_restore_counter_tiles(counters);
    scene->counters = counters;
    list_insert(list, counters);

    scene->clearBlock = obj_clear(heap_alloc2(0x30, 0, gHeapTagHudCounterB, 0), 0xD);
    hud = obj_create_hud_counter(heap_alloc2(0x110, 0, gHeapTagHudCounterObj, 0), scene->list);
    scene->hud = hud;
    limit = 9;
    scene->limit = limit;
    tween_groups_set_flag_bit(0x1EC, 0);
    write_var(0x1CDC, 0);
    value = (u16)read_var(0x1CE2);
    counter = (u32 *)((u8 *)hud + 0xDC);
    *counter = value;
    if (value <= 8)
        *counter = limit;
    obj_list_update_pos(scene->list, 0, 0);
    *(u16 *)gUnk_02000080 = 0;
    *(u16 *)((u8 *)&gSys + 2) = 0xFFFF;
    return scene;
}
END_NONMATCH

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
