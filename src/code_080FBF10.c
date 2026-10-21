#include "global.h"

extern const u32 gEffectArgTable[];
extern const u32 gSceneObjDataPtrTable[];
extern const u32 gRecordPtrTable[];

struct SceneObj
{
    s8 kind;
    u8 filler1[0x0B];
    s32 x;
    s32 y;
    s32 z;
    s32 dz;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x1D6 - 0x25];
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
    u8 unk20D;
    u8 filler20E[0x352 - 0x20E];
    u8 unk352;
};

struct SceneSlot
{
    struct SceneObj *obj;
    u8 filler4[0x0B - 0x04];
    u8 flags;
    s16 box[6];
    s8 index;
};

struct Flags79Bits
{
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct TaskOwner
{
    u8 filler0[0x14];
    u8 *list;
    void *type;
};

extern u8 *gWorkBuf;
extern const u8 gTypeB[];
extern const u8 gTaskPickTable[];
extern const u8 gEffectIndexTable[];
extern u8 gSys[];
void task_insert(void *node, u8 kind, void *data);
void mt19937_seed(u32 state, u32 seed);

u32 point_in_box(s16 *bounds, struct SceneObj *obj, s32 zMaxIn, s32 zMinIn);

extern u8 *gWorld;
extern u8 *gMemRoutines;
extern void (*gUnk_03000FE0)(void);
void vcount_store(u8 *out);
void run_task_list(u8 *list);
void refresh_sprite_display(u8 mode);
void world_main_frame(void);
void world_init(void);
void load_vram_tiles(void *p);
void set_text_ptrs(u32 value);
void set_cur_flags_both(u32 value);
void world_objects_tick(u32 *owner);
void obj_list_remove(void *obj);
void clear_list_entries(void);
void state_wait_free_slots(void);
void state_select_target(void);
void release_timer_tiles(void);
void set_irq_callback(u32 kind, void *cb);
void obj_release(void *obj);
void free_task_state(void *state, u32 kind);
void destroy_map_obj(void *obj, u32 kind);
void menu_obj_free(void *obj, u32 kind);
void restore_display(void *obj, u32 kind);
void reset_obj_hook(void *obj, u32 kind);
void heap_free(void *p);
void run_pending_obj_callbacks(void);
void set_flags_2_3(void *p);
void task_remove(void *node, u32 kind);
extern u16 gUnk_02000010[];
struct FlagBit6
{
    u8 lo6 : 6;
    u8 b6 : 1;
    u8 b7 : 1;
};
struct Lo2Bits
{
    u8 lo2 : 2;
    u8 hi6 : 6;
};
struct Bit4Bits
{
    u8 lo4 : 4;
    u8 b4 : 1;
    u8 hi3 : 3;
};
void owner_release(u8 *owner, u32 mode);
u32 world_entry_ready(void);
void set_slot_target(u32 idx, u32 value, u32 mode);
u32 sys_table_898(u32 i);
void slot_reset(u32 idx);
u8 find_active_entry(u8 *owner, u8 *out);
u32 boxes_overlap(s16 *bounds, struct SceneObj *a, struct SceneObj *b, s32 zAbove, s32 zBelow);
u32 boxes_overlap_oriented(s16 *box, struct SceneObj *a, struct SceneObj *b, s32 zAbove, s32 zBelow);

extern const u8 gTeeheeValleyRodsAndOrbsAnim[];
u32 find_def_index(u16 bank, u8 *out, u8 *ptr)
{
    u8 *header;
    u8 *base;
    u8 *entry;
    u16 key;
    u16 cur;

    header = (u8 *)*(gRecordPtrTable + bank);
    base = header - ((header[3] << 8) | header[2]);
    entry = (u8 *)(gTeeheeValleyRodsAndOrbsAnim + 0x15E1E);
    key = *(u16 *)(base + ptr[0x222] * 2);
    cur = entry[1] << 8 | entry[0];
    for (*out = 0; cur != 0; (*out)++)
    {
        if (cur == key)
        {
            return 1;
        }
        entry += 9;
        cur = entry[1] << 8 | entry[0];
    }
    return 0;
}

u32 object_def_mode(u16 bank, u8 index)
{
    u8 *header;
    u8 *base;
    u8 *entry;

    if (index <= 1)
    {
        return 0;
    }
    header = (u8 *)*(gRecordPtrTable + bank);
    base = header - ((header[7] << 8) | header[6]);
    entry = base + index * 20;
    return (entry[0x12] >> 3) & 7;
}

u8 bank_byte1_high(u16 bank, u8 *out)
{
    u8 *header = (u8 *)*(gRecordPtrTable + bank);

    *out = header[1] >> 3;
    return *out;
}

u8 *bank_data_5bit(u16 bank, u8 *out)
{
    u8 *header = (u8 *)*(gRecordPtrTable + bank);

    *out = ((header[1] << 2) | (header[0] >> 6)) & 0x1F;
    return header - ((header[5] << 8) | header[4]);
}

u8 *bank_data_6bit(u16 bank, u8 *out)
{
    u8 *header = (u8 *)*(gRecordPtrTable + bank);

    *out = header[0] & 0x3F;
    return header - ((header[3] << 8) | header[2]);
}

u32 table_083d8e28(u16 i)
{
    return *(gEffectArgTable + i);
}

u32 table_085213d0(u16 i)
{
    return *(gSceneObjDataPtrTable + i);
}

u8 zone_hit_sub_full(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if ((u8)point_in_box(bounds, obj, 0x7FFF, 0) == 0)
    {
        return 0;
    }
    bounds = (s16 *)((u8 *)slot + 0x1C);
    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return boxes_overlap_oriented(bounds, obj, entry, 0x7FFF, 0x7FFF) & 0xFF;
}
u8 zone_hit_sub_offsets(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if ((u8)point_in_box(bounds, obj, 0x7FFF, 0) == 0)
    {
        return 0;
    }
    bounds = (s16 *)((u8 *)slot + 0x1C);
    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return boxes_overlap_oriented(bounds, obj, entry, bounds[4], bounds[5]) & 0xFF;
}
NONMATCH("asm/code_080FBF10/zone_hit_box_full.s", u8 zone_hit_box_full(u8 *owner, struct SceneSlot *slot))
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    if ((u8)point_in_box(bounds, entry, 0x7FFF, 0) == 0)
    {
        return 0;
    }
    return boxes_overlap_oriented((s16 *)((u8 *)bounds + 0x10), obj, entry, 0x7FFF, 0x7FFF) & 0xFF;
}
END_NONMATCH

u8 zone_hit_box_offsets(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    if ((u8)point_in_box(bounds, entry, 0x7FFF, 0) == 0)
    {
        return 0;
    }
    bounds = (s16 *)((u8 *)bounds + 0x10);
    return boxes_overlap_oriented(bounds, obj, entry, bounds[4], bounds[5]) & 0xFF;
}

ASM_FUNC("asm/code_080FBF10/zone_facing_entry.s", void zone_facing_entry(void))

ASM_FUNC("asm/code_080FBF10/boxes_overlap.s", u32 boxes_overlap(s16 *bounds, struct SceneObj *a, struct SceneObj *b, s32 zAbove, s32 zBelow))

ASM_FUNC("asm/code_080FBF10/boxes_overlap_oriented.s", u32 boxes_overlap_oriented(s16 *box, struct SceneObj *a, struct SceneObj *b, s32 zAbove, s32 zBelow))

NONMATCH("asm/code_080FBF10/find_active_entry.s", u8 find_active_entry(u8 *owner, u8 *out))
{
    u32 select;
    u32 sel;
    struct SceneObj *entry;
    u32 ready;
    u8 *slot;
    struct SceneObj *other;

    select = owner[0x208];
    sel = ((u32)select << 29) >> 31;
    entry = *(struct SceneObj **)(owner + 0x28 + (sel << 2));
    ready = ((u32)entry->unk20C << 31) >> 31;
    ready &= ((u32)entry->unk352 << 28) >> 31;
    ready &= ((u32)entry->unk20D << 30) >> 31;
    if (ready != 0)
    {
        *out = sel;
    }
    else
    {
        slot = owner + 0x28;
        if ((select & 4) == 0)
        {
            slot += 4;
        }
        other = *(struct SceneObj **)slot;
        ready = ((u32)other->unk20C << 31) >> 31;
        ready &= ((u32)other->unk352 << 28) >> 31;
        if (ready == 0)
        {
            return 0;
        }
        *out = ((select >> 2) ^ 1) & 1;
    }
    return 1;
}
END_NONMATCH

u32 point_in_box(s16 *bounds, struct SceneObj *obj, s32 zMaxIn, s32 zMinIn)
{
    u32 v;
    u32 px;
    u32 py;
    u32 pz;
    u32 z;
    u16 zMax;
    u16 zMin;

    zMax = zMaxIn;
    zMin = zMinIn;
    v = obj->x;
    if ((s32)v < 0)
    {
        v += 0xFF;
    }
    px = (v << 8) >> 16;
    v = obj->y;
    if ((s32)v < 0)
    {
        v += 0xFF;
    }
    py = (v << 8) >> 16;
    z = obj->z + obj->dz;
    if ((s32)z < 0)
    {
        z += 0xFF;
    }
    pz = (z << 8) >> 16;
    if ((s16)px < bounds[0])
    {
        return 0;
    }
    if ((s16)px > bounds[1])
    {
        return 0;
    }
    if ((s16)py < bounds[3])
    {
        return 0;
    }
    if ((s16)py > bounds[2])
    {
        return 0;
    }
    if ((s16)pz < (s16)zMin)
    {
        return 0;
    }
    if ((s16)pz > (s16)zMax)
    {
        return 0;
    }
    return 1;
}

u32 slot_obj_flag_1d6(u32 owner, struct SceneSlot *slot)
{
    if ((slot->obj->unk1D6 & 1) != 0)
    {
        return 1;
    }
    return 0;
}

u32 slot_obj_not_bits_2_3(u32 owner, struct SceneSlot *slot)
{
    u32 flags = slot->obj->unk1D6;
    u32 bit2 = (flags << 29) >> 31;
    u32 bit3 = (flags << 28) >> 31;

    if ((bit3 & bit2) != 0)
    {
        return 0;
    }
    return 1;
}

u8 zone_oriented_offsets(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return boxes_overlap_oriented(bounds, obj, entry, bounds[4], bounds[5]) & 0xFF;
}

u8 zone_oriented_full(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return boxes_overlap_oriented(bounds, obj, entry, 0x7FFF, 0x7FFF) & 0xFF;
}

u8 zone_overlap_offsets(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return boxes_overlap(bounds, obj, entry, bounds[4], bounds[5]) & 0xFF;
}

u8 zone_overlap_full(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return boxes_overlap(bounds, obj, entry, 0x7FFF, 0x7FFF) & 0xFF;
}

u8 zone_point_offsets(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return point_in_box(bounds, entry, bounds[4], bounds[5]) & 0xFF;
}

u8 zone_point_full(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    u8 found;
    s32 index;
    struct SceneObj *entry;

    if (((s8 *)bounds)[12] < 0)
    {
        if ((slot->flags & 2) == 0)
        {
            if (find_active_entry(owner, &found) == 0)
            {
                return 0;
            }
        }
        else
        {
            u32 sel = (((u32)owner[0x208]) << 29) >> 31;

            found = sel;
        }
        index = found;
    }
    else
    {
        index = ((s8 *)bounds)[12];
    }
    entry = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    return point_in_box(bounds, entry, 0x7FFF, 0) & 0xFF;
}

u8 slot_obj_in_box_z(u32 owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;

    return point_in_box(bounds, slot->obj, bounds[4], bounds[5]) & 0xFF;
}

u8 slot_obj_in_box(u32 owner, struct SceneSlot *slot)
{
    struct SceneObj *obj = slot->obj;

    return point_in_box(slot->box, obj, 0x7FFF, 0) & 0xFF;
}

u32 slot_kind_matches(u8 *owner, struct SceneSlot *slot)
{
    struct SceneObj *obj = slot->obj;
    u8 *base = (u8 *)slot->box;
    s32 want;

    if (((s8 *)base)[12] >= 0)
    {
        want = base[12];
    }
    else
    {
        want = (((u32)owner[0x208]) << 29) >> 31;
    }
    if (obj->kind == want)
    {
        return 1;
    }
    return 0;
}

extern u32 gUnk_03000FFC;
extern u32 gUnk_03000D40;
extern u32 gUnk_03000FC0;
void cpu_set(const void *src, void *dst, u32 ctl);
void load_save_slot(void)
{
    u8 *base = (u8 *)gUnk_03000FFC + (((gUnk_03000D40 << 0x1C) >> 0x1E) * 0x6F8 + 0x10);
    {
        vu32 *dma = (vu32 *)REG_ADDR_DMA3SAD;
        dma[0] = (u32)(base + 0x1DC);
        dma[1] = (u32)((u8 *)gUnk_03000FC0 + 0x20);
        dma[2] = 0x840000DB;
        dma[2];
    }
    cpu_set(base + 0x548, (u8 *)gUnk_03000FC0 + 0x394, 0xD4);
}

void store_save_slot(void)
{
    u8 *base = (u8 *)gUnk_03000FFC + (((gUnk_03000D40 << 0x1C) >> 0x1E) * 0x6F8 + 0x10);
    {
        vu32 *dma = (vu32 *)REG_ADDR_DMA3SAD;
        dma[0] = (u32)((u8 *)gUnk_03000FC0 + 0x20);
        dma[1] = (u32)(base + 0x1DC);
        dma[2] = 0x840000DB;
        dma[2];
    }
    cpu_set((u8 *)gUnk_03000FC0 + 0x394, base + 0x548, 0xD4);
}

struct TaskOwner *effect_start(struct TaskOwner *owner, u32 kind, u32 arg, u8 *handle)
{
    u8 *sys;
    u8 *flags;
    u8 value;

    task_insert(owner, kind, (void *)arg);
    owner->type = (void *)gTypeB;
    gWorkBuf = handle;
    sys = gSys;
    mt19937_seed(*(u32 *)(sys + 0x8D0), *(u32 *)(sys + 0x1C));
    *(u16 *)(sys + 0x78) |= 0x3FF;
    flags = sys + 0x79;
    ((struct Flags79Bits *)flags)->mid2 = 0;
    ((struct Flags79Bits *)flags)->bit4 = 0;
    ((struct Flags79Bits *)flags)->bit5 = 0;
    sys[0x44] = 10;
    sys[0x45] = 8;
    gUnk_03000FE0 = world_init;
    if ((s8)handle[2] <= 0)
    {
        {
            const u8 *table = gTaskPickTable;

            value = (table + (*(u16 *)handle << 5))[3];
        }
        handle[2] = value;
        if ((s8)value <= 0)
        {
            value = gEffectIndexTable[*(u16 *)(owner->list + 0x124)];
            handle[2] = value;
        }
    }
    return owner;
}

void owner_release(u8 *owner, u32 mode)
{
    u8 *sys;
    u16 *p;
    u8 *list;
    u8 *world;

    ((struct TaskOwner *)owner)->type = (void *)gTypeB;
    gSys[0x44] = 10;
    gSys[0x45] = 2;
    if (gWorld != NULL)
    {
        release_timer_tiles();
        world = gWorld;
        set_irq_callback(2, *(void **)(world + 0x25C));
        world = gWorld;
        list = *(u8 **)(world + 0x78);
        if (list != NULL)
        {
            do
            {
                obj_release(list);
                list = *(u8 **)list;
            } while (list != NULL);
        }
        if (*(void **)(gWorld + 0x244) != NULL)
            free_task_state(*(void **)(gWorld + 0x244), 3);
        if (*(void **)(gWorld + 0x24C) != NULL)
            destroy_map_obj(*(void **)(gWorld + 0x24C), 3);
        if (*(void **)(gWorld + 0x248) != NULL)
            menu_obj_free(*(void **)(gWorld + 0x248), 3);
        if (*(void **)(gWorld + 0x250) != NULL)
            restore_display(*(void **)(gWorld + 0x250), 3);
        if (*(void **)(gWorld + 0x2F0) != NULL)
            reset_obj_hook(*(void **)(gWorld + 0x2F0), 3);
        p = gUnk_02000010;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        *p++ = 0;
        heap_free(gWorld);
        gWorld = NULL;
        run_pending_obj_callbacks();
    }
    if (gWorkBuf != NULL)
    {
        heap_free(gWorkBuf);
        gWorkBuf = NULL;
    }
    set_flags_2_3(((struct TaskOwner *)owner)->list);
    sys = gSys;
    ((struct FlagBit6 *)(sys + 0x79))->b6 = 0;
    task_remove(owner, mode);
}

void world_tick(void);
ASM_FUNC("asm/code_080FBF10/world_init.s", void world_init(void))

ASM_FUNC("asm/code_080FBF10/world_main_frame.s", void world_main_frame(void))

void world_tick(void)
{
    u8 vcount;
    u8 *world;
    u8 *arg;
    void (*stateFn)(u8 *);

    vcount_store(&vcount);
    world = gWorld;
    arg = world + 0x20;
    stateFn = *(void (**)(u8 *))(world + 0x24);
    stateFn(arg);
    run_task_list(gWorld + 0x4C);
    refresh_sprite_display(vcount);
    if (*(u32 *)(gWorld + 0x24C) != 0)
    {
        gUnk_03000FE0 = world_main_frame;
    }
}

void world_run_callback(u8 *owner)
{
    gUnk_03000FE0();
    if (gUnk_03000FE0 == 0)
    {
        if (owner != 0)
        {
            owner_release(owner, 3);
        }
    }
}

void owner_wait_entry(u8 *owner)
{
    u8 *old;
    u8 flag;
    s32 level;

    if ((u8)world_entry_ready() == 0)
    {
        old = *(u8 **)(gWorld + 0x2D8);
        flag = old[3];
        level = ((s8 *)old)[3];
        if (level > 0)
        {
            set_slot_target(1, level, -1);
        }
        else if (level == -1 || ((u8)(flag + 3) <= 1 && sys_table_898(1) != 0xA))
        {
            set_slot_target(1, 0xA, -1);
        }
        else
        {
            old = *(u8 **)(gWorld + 0x2D8);
            if (((s8 *)old)[3] == 0)
            {
                slot_reset(1);
            }
        }
        *(u32 *)(owner + 4) = 0;
    }
}

void state_prepare_world(u32 *slot);
ASM_FUNC("asm/code_080FBF10/world_begin_scene.s", void world_begin_scene(void))

NONMATCH("asm/code_080FBF10/state_prepare_world.s", void state_prepare_world(u32 *slot))
{
    u8 ready;

    ready = world_entry_ready();
    if (ready == 0)
    {
        load_vram_tiles(*(void **)(gWorld + 0x250));
        set_text_ptrs(0);
        *(u8 *)(gWorld + 0x34A) = 0xFF;
        *(u8 *)(gWorld + 0x34C) = 0xFF;
        *(u8 *)(gWorld + 0x34E) = 0xFF;
        *(u8 *)(gWorld + 0x350) = 0x80;
        *(u8 *)(gWorld + 0x34B) = 0xFF;
        *(u8 *)(gWorld + 0x34D) = 0xFF;
        *(u8 *)(gWorld + 0x34F) = 0xFF;
        *(u8 *)(gWorld + 0x351) = 0x80;
        ((void (*)(u32, void *, u32))gMemRoutines)(0x33333333, (void *)0x06017600, 0x200);
        ((struct Lo2Bits *)(gWorld + 0xA))->lo2 = 2;
        set_cur_flags_both(1);
        *(u16 *)(*(u8 **)(gWorld + 0x2D8) + 0xA) = ready;
        slot[1] = (u32)world_objects_tick;
    }
}
END_NONMATCH

void world_objects_tick(u32 *owner)
{
    s32 i;
    u8 *obj;
    u8 *list;
    s32 off;
    u32 zero;
    void (*next)(void);
    void (*cb)(u8 *);

    for (i = 0, zero = 0; i < 8; i++)
    {
        list = gWorld;
        off = i << 2;
        list = list + 0x58;
        obj = *(u8 **)(list + off);
        cb = *(void (**)(u8 *))(obj + 0x70);
        if (cb != 0)
        {
            cb(obj + 8);
        }
        if (*(void **)(obj + 0x10) != 0)
        {
            obj_list_remove(obj + 8);
            *(u16 *)(obj + 0xB6) = zero;
            *(u16 *)(obj + 0xB8) = zero;
            *(u16 *)(obj + 0xBA) = zero;
            obj[0x7E] &= 0x7F;
            ((struct Bit4Bits *)(obj + 0x80))->b4 = 0;
            *(u32 *)(obj + 0x74) = zero;
            *(u32 *)(obj + 0x70) = zero;
            *(u32 *)(obj + 0x5C) = zero;
        }
    }
    clear_list_entries();
    if (gWorld[4] == 0 || gWorld[5] == 0)
    {
        next = state_wait_free_slots;
    }
    else
    {
        next = state_select_target;
    }
    owner[1] = (u32)next;
}

ASM_FUNC("asm/code_080FBF10/state_select_target.s", void state_select_target(void))
