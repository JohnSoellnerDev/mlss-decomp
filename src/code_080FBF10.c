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
    u8 unk24; // bits 0-2: facing direction (FACING_DIR), 0-7 in 45° steps like angle >> 13
    u8 filler25[0x1D6 - 0x25];
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
    u8 unk20D;
    u8 filler20E[0x216 - 0x20E];
    u8 unk216;
    u8 filler217[0x352 - 0x217];
    u8 unk352;
};

// The facing direction (0-7) in a SceneObj.unk24 byte.
#define FACING_DIR(bits) (((u32)(bits) << 29) >> 29)

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
void run_task_list(void *list);
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
extern u16 gUnk_02000010[]; // shadow copies of the BG0-3 scroll registers (HOFS, VOFS each)
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

struct ObjListRoot
{
    u8 filler0[0x3C];
    u32 head3C;
};

// The part of the system area (gSys) world_main_frame uses.
struct WorldSys
{
    u8 filler0[0x2A];
    u16 keysNew;
    u8 filler2C[0x3E - 0x2C];
    u16 unk3E;
    u8 filler40[0x79 - 0x40];
    u8 unk79_0 : 6;
    u8 busy79 : 1;
    u8 unk79_7 : 1;
    u8 filler7A[0x880 - 0x7A];
    u32 oamCur;
    u32 palMask;
    u8 unk888_0 : 4;
    u8 oamHold : 1;
    u8 unk888_5 : 3;
};
#define WORLD_SYS ((struct WorldSys *)gSys)

// GBA key bits (REG_KEYINPUT).
#define KEY_SELECT 0x0004
#define KEY_START 0x0008

// Sound played for a key the world doesn't take right now
// (set_slot_value queues a song and sets its volume, -1: the current one).
#define SE_KEY_BLOCKED 0x15F

// The sprite a world object draws with.
struct WorldPartGfx
{
    u8 filler0[0xA];
    u16 scale; // 0x100 = 1.0
};

// One of the eight objects of the world (gWorld->parts).
struct WorldPart
{
    u8 filler0[0x10];
    struct WorldPartGfx *gfx;
};

struct WorldUnk7C
{
    u8 filler0[0xEC];
    s16 unkEC;
};

// The record WorldState.unk2D8 points to.
struct WorldUnk2D8
{
    u8 filler0[3];
    s8 unk3;            // owner_wait_entry: > 0 a slot target level, -1 the default (0xA)
    u8 filler4[0xA - 0x4];
    u16 unkA;           // cleared by state_prepare_world
};

// The world state gWorld points to, as far as world_main_frame uses it.
struct WorldState
{
    u8 filler0[7];
    u8 unk7;
    u8 filler8[0xA - 0x8];
    u8 modeA : 2;       // set to 2 by state_prepare_world
    u8 unkA_hi : 6;
    u8 fillerB[0x10 - 0xB];
    u8 holdTimer;       // frames left to wait while the system holds the display
    u8 filler11[0x20 - 0x11];
    u32 stateTask;      // the block the state functions get
    void (*state)(u32 *task); // runs once a frame; NULL ends the world
    u8 filler28[0x40 - 0x28];
    void *taskLists[4]; // run at different points of the frame
    u8 filler50[0x58 - 0x50];
    struct WorldPart *parts[8];
    u8 *objList;        // the world's objects, linked through their first word
    struct WorldUnk7C *unk7C;
    u8 filler80[0x244 - 0x80];
    void *taskState;    // free_task_state
    void *menuObj;      // menu_obj_free
    void *mapObj;       // sync_obj_impulse, destroy_map_obj
    void *slotBlocks;   // free_slot_blocks, restore_display
    u8 filler254[0x25C - 0x254];
    void *savedIrq2;    // IRQ 2 callback to put back when the world ends
    u8 filler260[0x2B0 - 0x260];
    u32 frameCount;
    u16 buzzKeys;       // newly pressed keys that only play the "can't" sound
    u8 filler2B6[0x2BE - 0x2B6];
    u8 unk2BE_lo : 6;
    u8 keyBit0 : 1;     // copies of gSys unk3E bits 0 and 1
    u8 keyBit1 : 1;
    u8 filler2BF[0x2C1 - 0x2BF];
    u8 flags2C1;        // bit 3: START/SELECT allowed
    u8 filler2C2[0x2D8 - 0x2C2];
    struct WorldUnk2D8 *unk2D8;
    u8 filler2DC[0x2F0 - 0x2DC];
    void *objHook;      // reset_obj_hook
    u8 filler2F4[0x2F8 - 0x2F4];
    u8 paused;          // nonzero: the state and the objects don't run
    u8 filler2F9[0x34A - 0x2F9];
    u8 unk34A[2];       // four bytes per brother, reset by state_prepare_world
    u8 unk34C[2];
    u8 unk34E[2];
    u8 unk350[2];
};
#define WORLD ((struct WorldState *)gWorld)
struct Bit4Bits
{
    u8 lo4 : 4;
    u8 b4 : 1;
    u8 hi3 : 3;
};
struct Lo3Bits
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};
u16 angle_of_vector16(s32 x, s32 y);
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
u8 zone_hit_box_full(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
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
        entry = *(struct SceneObj **)(owner + 0x28 + (found << 2));
    }
    else
    {
        entry = *(struct SceneObj **)(owner + 0x28 + (((s8 *)bounds)[12] << 2));
    }
    if ((u8)point_in_box(bounds, entry, 0x7FFF, 0) == 0)
    {
        return 0;
    }
    bounds = (s16 *)((u8 *)bounds + 0x10);
    return boxes_overlap_oriented(bounds, obj, entry, 0x7FFF, 0x7FFF) & 0xFF;
}

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

/* Returns 1 if the party member the zone is tied to (slot index, or the active one when it's
 * negative) faces the zone's object: its facing direction is within one octant of the
 * direction from it to the object. */
u32 zone_facing_entry(u8 *owner, struct SceneSlot *slot)
{
    s16 *bounds = slot->box;
    struct SceneObj *obj = slot->obj;
    u8 found;
    s32 index;
    struct SceneObj *member;
    u16 angle;
    u32 octant;
    u32 dir;
    u8 bits;
    s32 low;

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
    member = *(struct SceneObj **)(owner + 0x28 + (index << 2));
    angle = angle_of_vector16(obj->x - member->x, obj->y - member->y);
    octant = ((u32)angle << 16) >> 29;
    switch (octant)
    {
    case 0: /* facing 7, 0 or 1 */
        dir = FACING_DIR(member->unk24) - 2;
        goto above4;
    case 1: /* facing 0-2 */
        dir = FACING_DIR(member->unk24);
        goto atmost2;
    case 2: /* facing 1-3 */
        bits = member->unk24;
        low = 7;
        low &= bits;
        if (low == 0)
        {
            goto fail;
        }
        if (FACING_DIR(bits) > 3)
        {
            goto fail;
        }
    ok:
        return 1;
    case 3: /* facing 2-4 */
        dir = FACING_DIR(member->unk24) - 2;
        goto atmost2;
    case 4: /* facing 3-5 */
        dir = FACING_DIR(member->unk24) - 3;
        goto atmost2;
    case 5: /* facing 4-6 */
        dir = FACING_DIR(member->unk24) - 4;
    atmost2:
        if (dir > 2)
        {
            goto fail;
        }
        goto ok;
    case 6: /* facing 5-7 */
        dir = FACING_DIR(member->unk24);
    above4:
        if (dir <= 4)
        {
            goto fail;
        }
        goto ok;
    case 7: /* facing 6, 7 or 0 */
        bits = member->unk24;
        if (FACING_DIR(bits) > 5)
        {
            goto ok;
        }
        low = 7;
        low &= bits;
        if (low == 0)
        {
            goto ok;
        }
        goto fail;
    default:
        goto fail;
    }
fail:
    return 0;
}

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
    if (ready == 0)
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
    else
    {
        *out = sel;
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

/* Starts the battle task (start_battle_block passes its battle parameters): inserts the task,
 * keeps the parameters in gWorkBuf, reseeds the random generator, resets the system's display
 * flags and makes world_init the next frame function. When the parameters' byte 2 isn't set
 * (<= 0), it comes from byte 3 of the battle's 32-byte gTaskPickTable entry, or from
 * gEffectIndexTable when that is unset too. */
struct TaskOwner *effect_start(struct TaskOwner *owner, u32 kind, u32 tag, u8 *params)
{
    u8 *sys;
    u8 *flags;
    u8 value;

    task_insert(owner, kind, (void *)tag);
    owner->type = (void *)gTypeB;
    gWorkBuf = params;
    sys = gSys;
    mt19937_seed(*(u32 *)(sys + 0x8D0), *(u32 *)(sys + 0x1C)); /* the generator's state buffer */
    *(u16 *)(sys + 0x78) |= 0x3FF;
    flags = sys + 0x79;
    ((struct Flags79Bits *)flags)->mid2 = 0;
    ((struct Flags79Bits *)flags)->bit4 = 0;
    ((struct Flags79Bits *)flags)->bit5 = 0;
    sys[0x44] = 10;
    sys[0x45] = 8;
    gUnk_03000FE0 = world_init;
    if ((s8)params[2] <= 0)
    {
        {
            const u8 *table = gTaskPickTable;

            /* entry of the battle id params[0..1] */
            value = (table + (*(u16 *)params << 5))[3];
        }
        params[2] = value;
        if ((s8)value <= 0)
        {
            value = gEffectIndexTable[*(u16 *)(owner->list + 0x124)];
            params[2] = value;
        }
    }
    return owner;
}

// Ends the world task owner: puts back IRQ 2's callback, releases the world's objects and frees
// what the world holds (task state, map object, menu, display slots, object hook), clears the BG
// scroll shadows, frees the world and the work buffer, resumes the task that started it and
// removes the owner's own task.
void owner_release(u8 *owner, u32 mode)
{
    u8 *sys;
    u16 *scroll;
    u8 *obj;
    struct WorldState *world;

    ((struct TaskOwner *)owner)->type = (void *)gTypeB;
    gSys[0x44] = 10;
    gSys[0x45] = 2;
    if (gWorld != NULL)
    {
        release_timer_tiles();
        world = WORLD;
        set_irq_callback(2, world->savedIrq2);
        world = WORLD;
        obj = world->objList;
        if (obj != NULL)
        {
            do
            {
                obj_release(obj);
                obj = *(u8 **)obj;
            } while (obj != NULL);
        }
        if (WORLD->taskState != NULL)
            free_task_state(WORLD->taskState, 3);
        if (WORLD->mapObj != NULL)
            destroy_map_obj(WORLD->mapObj, 3);
        if (WORLD->menuObj != NULL)
            menu_obj_free(WORLD->menuObj, 3);
        if (WORLD->slotBlocks != NULL)
            restore_display(WORLD->slotBlocks, 3);
        if (WORLD->objHook != NULL)
            reset_obj_hook(WORLD->objHook, 3);
        scroll = gUnk_02000010;
        *scroll++ = 0;
        *scroll++ = 0;
        *scroll++ = 0;
        *scroll++ = 0;
        *scroll++ = 0;
        *scroll++ = 0;
        *scroll++ = 0;
        *scroll++ = 0;
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

extern u8 *gModelCtx;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
extern u8 gUnk_0203FFB8[];
void poll_keys(void);
void set_slot_value(u32 id, s32 value);
void sync_obj_impulse(void *mapObj);
void free_slot_blocks(void *slotBlocks);
void sprite_frame_update(void);
void update_sprite_list(void);
void obj_list_pass_0a8c(void);
void draw_sprite_nodes(void);
void oam_link_slot_entries(void);
void update_world_objects(void);
u32 emit_active_parts_oam(u32 oam);
void update_obj_list(void);
void update_slot_sprites(void);
void update_active_objs(void);
// One frame of the running world: reads the keys (buzzing for blocked ones), waits out a display hold,
// runs the task lists, draws the sprites and models into OAM and, unless paused, runs the world's
// state function and objects. Once the state function is NULL the world ends and the frame
// function is cleared.
void world_main_frame(void)
{
    u8 vcount;
    u8 *node;

    vcount_store(&vcount);
    poll_keys();
    WORLD_SYS->busy79 = 1;
    if (WORLD_SYS->keysNew & WORLD->buzzKeys)
        set_slot_value(SE_KEY_BLOCKED, -1);
    if ((WORLD->flags2C1 & 8) == 0 && (WORLD_SYS->keysNew & (KEY_START | KEY_SELECT)))
        set_slot_value(SE_KEY_BLOCKED, -1);
    gWorld[7] |= 0x1F; // WORLD->unk7, written as a byte so it matches
    if (WORLD_SYS->palMask != 0 && WORLD->holdTimer != 0)
    {
        WORLD->holdTimer--;
        if (WORLD->holdTimer == 0)
            WORLD_SYS->oamHold = 0;
        else if (WORLD_SYS->oamHold)
            return;
    }
    if (WORLD->mapObj != NULL)
        sync_obj_impulse(WORLD->mapObj);
    if (WORLD->slotBlocks != NULL)
        free_slot_blocks(WORLD->slotBlocks);
    sprite_frame_update();
    run_task_list(&WORLD->taskLists[0]);
    update_sprite_list();
    obj_list_pass_0a8c();
    run_task_list(&WORLD->taskLists[1]);
    run_task_list(&WORLD->taskLists[2]);
    draw_sprite_nodes();
    if (gModelCtx[2] != 0)
        gUnk_03000D48(*(u32 *)(gModelCtx + 0x3C), *(u32 *)(gModelCtx + 0x38));
    oam_link_slot_entries();
    update_world_objects();
    if (gModelCtx[2] != 0)
    {
        // Unlink the model while its parts are emitted, then link it back.
        node = *(u8 **)(*(u8 **)(gModelCtx + 0x38) + 0xC);
        *(u32 *)(node + 0x10) = 0;
        WORLD_SYS->oamCur = gUnk_03000D4C(((struct ObjListRoot *)gUnk_0203FFB8)->head3C, WORLD_SYS->oamCur);
        *(u32 *)(node + 0x10) = *(u32 *)(gModelCtx + 0x38);
    }
    WORLD_SYS->oamCur = emit_active_parts_oam(WORLD_SYS->oamCur);
    update_obj_list();
    update_slot_sprites();
    if (WORLD->paused == 0)
    {
        struct WorldPart **parts;
        u16 scale;
        u32 bit;

        bit = WORLD_SYS->unk3E & 1;
        WORLD->keyBit0 = bit;
        WORLD->keyBit1 = (WORLD_SYS->unk3E >> 1) & 1;
        if (WORLD->unk7C->unkEC == -2)
        {
            u32 tmp = WORLD->keyBit0;

            WORLD->keyBit0 = WORLD->keyBit1;
            WORLD->keyBit1 = tmp;
        }
        // Every object's sprite back to full size.
        parts = WORLD->parts;
        scale = 0x100;
        if (parts[0] != NULL && parts[0]->gfx != NULL)
            parts[0]->gfx->scale = scale;
        if (parts[1] != NULL && parts[1]->gfx != NULL)
            parts[1]->gfx->scale = scale;
        if (parts[2] != NULL && parts[2]->gfx != NULL)
            parts[2]->gfx->scale = scale;
        if (parts[3] != NULL && parts[3]->gfx != NULL)
            parts[3]->gfx->scale = scale;
        if (parts[4] != NULL && parts[4]->gfx != NULL)
            parts[4]->gfx->scale = scale;
        if (parts[5] != NULL && parts[5]->gfx != NULL)
            parts[5]->gfx->scale = scale;
        if (parts[6] != NULL && parts[6]->gfx != NULL)
            parts[6]->gfx->scale = scale;
        if (parts[7] != NULL && parts[7]->gfx != NULL)
            parts[7]->gfx->scale = scale;
        WORLD->state(&WORLD->stateTask);
        update_active_objs();
        WORLD->frameCount++;
    }
    run_task_list(&WORLD->taskLists[3]);
    refresh_sprite_display(vcount);
    if (WORLD->state == NULL)
    {
        set_irq_callback(3, 0);
        gUnk_03000FE0 = 0;
    }
}

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

/* State: once the world entry is ready, loads the VRAM tiles, clears the text pointers, resets both
 * brothers' bytes at 0x34A, fills 0x200 bytes of sprite VRAM (0x06017600) with 0x33, sets world mode
 * bits 2 and the current flags, and moves the task on to world_objects_tick. */
void state_prepare_world(u32 *task)
{
    u8 ready;
    s32 bro;

    ready = world_entry_ready();
    if (ready == 0)
    {
        load_vram_tiles(WORLD->slotBlocks);
        set_text_ptrs(0);
        for (bro = 0; bro < 2; bro++)
        {
            WORLD->unk34A[bro] = 0xFF;
            WORLD->unk34C[bro] = 0xFF;
            WORLD->unk34E[bro] = 0xFF;
            WORLD->unk350[bro] = 0x80;
        }
        ((void (*)(u32, void *, u32))gMemRoutines)(0x33333333, (void *)0x06017600, 0x200);
        WORLD->modeA = 2;
        set_cur_flags_both(1);
        WORLD->unk2D8->unkA = ready;
        task[1] = (u32)world_objects_tick; /* the task's state */
    }
}

/* Ends the world's eight objects (WORLD->parts; each holds an actor at +8): runs each one's
   callback once, and takes each one that still has a sprite off the object list, clearing its motion
   (+0xB6..+0xBB), flag bits and callbacks. Then clears the list entries and goes on to
   state_select_target, or to state_wait_free_slots while world byte 4 or 5 is 0. */
void world_objects_tick(u32 *owner)
{
    s32 i;
    u8 *part;
    u8 *parts;
    s32 off;
    u32 zero;
    void (*next)(void);
    void (*callback)(u8 *);

    for (i = 0, zero = 0; i < 8; i++)
    {
        parts = gWorld;
        off = i << 2;
        parts = parts + 0x58; /* WORLD->parts */
        part = *(u8 **)(parts + off);
        callback = *(void (**)(u8 *))(part + 0x70);
        if (callback != 0)
        {
            callback(part + 8);
        }
        /* WorldPart.gfx: the actor has a sprite */
        if (*(void **)(part + 0x10) != 0)
        {
            obj_list_remove(part + 8);
            *(u16 *)(part + 0xB6) = zero;
            *(u16 *)(part + 0xB8) = zero;
            *(u16 *)(part + 0xBA) = zero;
            part[0x7E] &= 0x7F;
            ((struct Bit4Bits *)(part + 0x80))->b4 = 0;
            *(u32 *)(part + 0x74) = zero;
            *(u32 *)(part + 0x70) = zero;
            *(u32 *)(part + 0x5C) = zero;
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
    owner[1] = (u32)next; /* the owner task's state */
}

ASM_FUNC("asm/code_080FBF10/state_select_target.s", void state_select_target(void))
