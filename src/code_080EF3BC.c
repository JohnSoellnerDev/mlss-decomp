#include "global.h"

extern u8 *gEffectState;
extern u8 *gUnk_03000FC0;
extern u8 *gUnk_03000FC8;
extern u8 *gUnk_03000FCC;
extern u8 *gCurObj;
extern const u8 gEffectPoolTag[];
extern u8 gTaskTemplate[];
extern u8 gClaimKindTable[];
extern const u8 gDefaultTaskDesc[];

struct TaskCopy
{
    u32 words[4];
};

struct EffectPoolBits
{
    u8 lo:2;
    u8 mode:4;
    u8 hi:2;
};

struct EffectSlotBits
{
    u8 flag:1;
    u8 rest:7;
};

struct EffectStateMask
{
    u8 filler0[0x54A];
    u16 slotMask;
};

void clear_slot_rect(u8 *rect, u32 n);
void copy_map_rects(u8 *rects, u32 n);
void heap_free(u8 *p);
void release_obj_parts(u8 *p, u32 n);
void free_ptr_pair(u8 *p, u32 n);
void reset_obj_params_b(u8 *obj, u8 *state, u32 arg, u32 zero, u32 one, u32 ff);
void obj_finish_move_step(void *obj);
void obj_stop_motion(void *obj);
void obj_apply_state_a(void *obj);
void set_var(u8 *vars, void *dst, u8 *buf, u32 zero, s32 e, u32 f);
void register_effect_slot(u32 first, u8 *slot, u32 *args);
void text_window_close(void);
void entity_camera_offset(u8 *list, u32 index, s32 *x, s32 *y);
u32 claim_table_slot(u8 *table, u32 a, u32 b, u32 c, s32 d, s32 e, u32 f);
void update_slot_bounds(u8 *pool, u32 slot);
void heap_free_b(void *p);
void task_remove(void *node, u32 kind);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *init_slot_table(void *state);
extern const u8 gBgLayerTag[];
extern const u8 gBgBufferTag[];
extern u16 gUnk_02000010[];
void *init_text_obj(void *holder, u32 arg1, u32 size, u32 arg3);
void flags_vars_setup_bg_layer(void *holder, u32 zero, u32 flags, void *block, u32 a4, u32 a5, void *a6, void *a7, void *a8);
u8 text_layer_step(u8 *owner);
u32 entry_table_value(u8 *obj, u32 idx);
u32 obj_collision_mask(u8 *owner, u32 idx);
void update_box_and_scroll(u8 *owner);
void update_owner_objects(u8 *owner);

/* Script command: shows an overlay sprite in the effect slot table (gUnk_03000FC8). args[0] is an
 * entity of the owner's scene, args[1] the sprite kind, args[2] nonzero to follow that entity
 * (offset by its camera position, raised by its +0x283 byte), args[3]/args[4] the x/y position and
 * args[5] the lifetime (0xFF: the kind's default from gClaimKindTable). Always returns 1 (done). */
u32 claim_effect_slot(u8 *owner, u32 unused, u32 *args)
{
    s32 offsetX;
    s32 offsetY;
    u32 entity = args[0];
    u8 *scene = *(u8 **)(owner + 0x14);
    u8 *entityObj = *(u8 **)(scene + entity * 4 + 0x28);
    u32 slot;

    offsetY = 0;
    offsetX = 0;
    if (args[2] != 0)
    {
        entity_camera_offset(scene, entity, &offsetX, &offsetY);
        offsetY = offsetY - entityObj[0x283];
    }
    if (args[5] == 0xFF)
    {
        args[5] = gClaimKindTable[args[1]];
    }
    slot = (u8)claim_table_slot(gUnk_03000FC8, (u8)args[0], (u8)args[1], (u8)args[2],
                                *(s16 *)((u8 *)args + 0xC), *(s16 *)((u8 *)args + 0x10), (u8)args[5]);
    /* 0x3F: no free slot */
    if (slot != 0x3F)
    {
        u8 *table = gUnk_03000FC8;
        u8 *entry = *(u8 **)table + (slot << 4);

        *(s16 *)(entry + 8) = offsetX;
        *(s16 *)(entry + 0xA) = offsetY;
        update_slot_bounds(table, slot);
    }
    return 1;
}

/* The bros pair (struct PairObj) a script owner works on: subA/subB at +0x200/+0x204. */
#define OWNER_PAIR(owner) (*(u8 **)((owner) + 0x14))

/* The pair's state byte (+0x12A); follow_anchor sets 3 while the bros walk to the anchor,
 * and states 3-5 mean the pair is still busy with such a move. */
#define PAIR_STATE(pair) (*(u8 *)((pair) + 0x12A))

/* Pair flags byte at +0x208: bit 3 and bit 4 (bit 4: the pair has reached its anchor). */
#define PAIR_FLAGS_208 0x208
struct AnchorBits4 { u8 lo4 : 4; u8 b4 : 1; u8 top : 3; };
struct AnchorBits3 { u8 lo3 : 3; u8 b3 : 1; u8 top : 4; };
void set_sub_pair_mode(u8 *target, u32 a, u32 b, u32 c);
void follow_anchor(u8 *target, u32 arg);

/* Script command "anchor": while the owner's bros pair is still busy with a move (state 3-5),
 * rewinds the script to this command and yields (returns 0). Otherwise cmds[0] picks:
 * 0-3: set_sub_pair_mode with bit 0 and bit 1 of the command; 4 / 7: follow the anchor
 * (follow_anchor 0 / 1); 5: clear flag bit 4, then set flag bit 3 to 0; 6: set flag bit 3.
 * Unknown commands do nothing. Returns 1 (go on with the script). */
u32 apply_anchor_command(u8 *owner, u8 *script, u32 *cmds)
{
    u8 *pair = OWNER_PAIR(owner);
    u32 cmd;

    if ((u8)(PAIR_STATE(pair) - 3) <= 2)
    {
        *(u32 *)script = *(u32 *)(script + 0x14);
        return 0;
    }
    cmd = cmds[0];
    if (cmd > 7)
    {
        return 1;
    }
    switch (cmd)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        set_sub_pair_mode(OWNER_PAIR(owner), cmd & 1, ((s32)cmd >> 1) & 1, 0);
        break;
    case 4:
        follow_anchor(OWNER_PAIR(owner), 0);
        break;
    case 5:
        ((struct AnchorBits4 *)(OWNER_PAIR(owner) + PAIR_FLAGS_208))->b4 = 0;
        /* fall through */
    case 6:
    {
        u8 *target = OWNER_PAIR(owner);
        u32 bit = cmds[0] - 5;
        ((struct AnchorBits3 *)(target + PAIR_FLAGS_208))->b3 = bit & 1;
        break;
    }
    case 7:
        follow_anchor(OWNER_PAIR(owner), 1);
        break;
    }
    return 1;
}

struct Bit1Bits { u8 lo1 : 1; u8 b1 : 1; u8 rest : 6; };

u32 pair_slot_kind(u8 *list);
u32 sub_0803C454(u8 *list);
u32 sub_0803C4EC(u8 *list);

/* Script command "branch if": tests condition `cmds[0]` on the owner's bros pair and, if it holds,
 * sets *next to the target in cmds[1]. 0-4: pair_slot_kind is nonzero / above 1 / 2 / 4 / 3;
 * 5: either bro's state byte (+0x368) is 9; 6: pair flag 0xF8 bit 1 (cleared afterwards);
 * 7: pair flag 0x2B8 bit 6; 8/9: sub_0803C454 / sub_0803C4EC; 10/11: pair flag 0xF8 bit 2 set /
 * clear. Unknown conditions do nothing. Always returns 1 (go on with the script). */
u32 check_object_condition(u8 *owner, u32 *next, u32 *cmds)
{
    u32 cond = *cmds++;

    if (cond > 0xB)
    {
        return 1;
    }
    switch (cond)
    {
    case 0:
        if (pair_slot_kind(OWNER_PAIR(owner)) != 0)
        {
            *next = *cmds;
        }
        break;
    case 1:
        if (pair_slot_kind(OWNER_PAIR(owner)) > 1)
        {
            *next = *cmds;
        }
        break;
    case 2:
        if (pair_slot_kind(OWNER_PAIR(owner)) == 2)
        {
            *next = *cmds;
        }
        break;
    case 3:
        if (pair_slot_kind(OWNER_PAIR(owner)) == 4)
        {
            *next = *cmds;
        }
        break;
    case 4:
        if (pair_slot_kind(OWNER_PAIR(owner)) == 3)
        {
            *next = *cmds;
        }
        break;
    case 5:
        if (*(u8 *)(*(u8 **)(*(u8 **)(OWNER_PAIR(owner) + 0x200) + 0x368)) == 9
            || *(u8 *)(*(u8 **)(*(u8 **)(OWNER_PAIR(owner) + 0x204) + 0x368)) == 9)
        {
            *next = *cmds;
        }
        break;
    case 6:
        if ((*(u8 *)(OWNER_PAIR(owner) + 0xF8) & 2) != 0)
        {
            *next = *cmds;
        }
        ((struct Bit1Bits *)(OWNER_PAIR(owner) + 0xF8))->b1 = 0;
        break;
    case 7:
        if ((*(u8 *)(OWNER_PAIR(owner) + 0x2B8) & 0x40) != 0)
        {
            *next = *cmds;
        }
        break;
    case 8:
        if ((u8)sub_0803C454(OWNER_PAIR(owner)) != 0)
        {
            *next = *cmds;
        }
        break;
    case 9:
        if ((u8)sub_0803C4EC(OWNER_PAIR(owner)) != 0)
        {
            *next = *cmds;
        }
        break;
    case 10:
        if ((*(u8 *)(OWNER_PAIR(owner) + 0xF8) & 4) != 0)
        {
            *next = *cmds;
        }
        break;
    case 11:
        if ((*(u8 *)(OWNER_PAIR(owner) + 0xF8) & 4) == 0)
        {
            *next = *cmds;
        }
        break;
    }
    return 1;
}

extern u8 *gMemRoutines;
extern const u8 gAreaEntryTable[];
extern u8 *gRecordPtrTable[];
void set_flag(u32 a, u32 b, u32 value, u32 flag);

/* Script command (area, record, value): sets the "done" flag of one of an area's object records,
   flag 0x960 + the record's id, to value; only records of kind 3 (byte 0x12 bits 3-5) have one.
   Area 0x3FF means every area: all 2048 of those flags (gUnk_03000FC0 + 0x14C..0x24C, which is
   flags 0x960..0x115F) are set (value != 0) or cleared. */
u32 apply_record_setting(u32 unused0, u32 unused1, u32 *cmds)
{
    if (cmds[0] != 0x3FF)
    {
        u32 area = cmds[0];
        const u8 *areaEntry = gAreaEntryTable + area * 24;
        u8 *recordSet;
        u8 *records;
        u8 *record;

        cmds++;
        /* areaEntry +0x10: the area's record set in gRecordPtrTable */
        recordSet = *(u8 **)(*(u16 *)(areaEntry + 0x10) * 4 + (u32)gRecordPtrTable);
        /* the set's bytes 6-7 hold how far back its 20-byte records start */
        records = recordSet - ((recordSet[7] << 8) | recordSet[6]);
        record = records + *cmds++ * 20;

        if (((record[0x12] >> 3) & 7) == 3)
        {
            u32 id = (((u32)record[0xF] << 24) | (record[0xE] << 16) | (record[0xD] << 8) | record[0xC]) >> 10;

            set_flag(0, 0, (u16)(id + 0x960), *(u8 *)cmds);
        }
    }
    else
    {
        register u8 *vars asm("r4") = gUnk_03000FC0;
        u32 *flagWord;
        u32 fill;

        flagWord = (u32 *)(vars + 0x14C);
        if (cmds[2] != 0)
            fill = -1;
        else
            fill = 0;
        cmds[0] = fill;
        *flagWord = fill;
        flagWord = (u32 *)(vars + 0x150);
        ((void (*)(u32, void *, u32))gMemRoutines)(cmds[0], flagWord, 0xFC);
        flagWord = (u32 *)(vars + 0x248);
        *flagWord = cmds[0];
    }
    return 1;
}

struct FlagBits7 { u8 lo7 : 7; u8 b7 : 1; };
struct FlagBits6 { u8 lo6 : 6; u8 b6 : 1; u8 top : 1; };
struct FlagBits5 { u8 lo5 : 5; u8 b5 : 1; u8 top : 2; };

static inline void set_flag_bit5(u8 *p, u32 v)
{
    ((struct FlagBits5 *)p)->b5 = v;
}

static inline void set_flag_bit6(u8 *p, u32 v)
{
    ((struct FlagBits6 *)p)->b6 = v;
}

static inline void set_flag_bit7(u8 *p, u32 v)
{
    ((struct FlagBits7 *)p)->b7 = v;
}

u32 entity_setup_set_flag_bit(u8 *obj, u32 unused, u32 *cmds)
{
    u8 *list = *(u8 **)(obj + 0x14);
    u32 idx;
    u32 off;
    u8 *slots;
    u8 *entry;
    s32 kind;

    idx = *cmds++;
    off = idx << 2;
    slots = list + 0x28;
    entry = *(u8 **)(slots + off);
    kind = *cmds++;

    switch (kind)
    {
    case 0:
    {
        u32 value = *cmds;
        set_flag_bit7(entry + 0x351, value);
        break;
    }
    case 1:
    {
        u32 value = *cmds;
        register u8 *p asm("r3") = list + 0x209;
        register u8 mask asm("r0") = 1;
        u32 v = (mask & value) << 6;
        u32 f = *p;
        register s32 clr asm("r0") = ~0x40;

        clr &= f;
        clr |= v;
        *p = clr;
        break;
    }
    case 2:
    {
        u32 value = *cmds;
        set_flag_bit5(list + 0x20B, value);
        break;
    }
    case 3:
    {
        register u8 *p asm("r3") = list + 0x26A;
        register u8 mask asm("r0") = 1;
        u32 v = (mask & *(u8 *)cmds) << 6;
        u32 f = *p;
        register s32 clr asm("r0") = ~0x40;

        clr &= f;
        clr |= v;
        *p = clr;
        break;
    }
    }
    return 1;
}

/* Script command: stores one field of the owner's field scene (owner + 0x14, the one whose
 * entity list set_var_from_source reads) in script variable cmd[0]. The word at cmd + 4
 * picks the field: 0 = u16 +0x124, 1 = s16 +0x27E, 2 = s16 +0x280, 3 = top 3 bits of
 * +0x26B, 4 = s16 +0x26E. */
u32 set_var_from_entry(u8 *owner, u8 *thread, s16 *cmd)
{
    switch (*(u32 *)((u8 *)cmd + 4))
    {
    case 0:
        set_var(gEffectState + 0x4B0, thread + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(u16 *)(*(u8 **)(owner + 0x14) + 0x124));
        break;
    case 1:
        set_var(gEffectState + 0x4B0, thread + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(s16 *)(*(u8 **)(owner + 0x14) + 0x27E));
        break;
    case 2:
        set_var(gEffectState + 0x4B0, thread + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(s16 *)(*(u8 **)(owner + 0x14) + 0x280));
        break;
    case 3:
        set_var(gEffectState + 0x4B0, thread + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(u8 *)(*(u8 **)(owner + 0x14) + 0x26B) >> 5);
        break;
    case 4:
        set_var(gEffectState + 0x4B0, thread + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd[0], *(s16 *)(*(u8 **)(owner + 0x14) + 0x26E));
        break;
    }
    return 1;
}

struct SignedId7 { s8 id : 7; u8 top : 1; };

/* Script command: read one property of a field entity into a script variable */
struct EntityPropCmd
{
    s16 var;      /* variable to set */
    u8 filler2[2];
    u32 property; /* enum EntityProperty */
    u32 entity;   /* index into the owner's entity list (list + 0x28) */
};

enum EntityProperty
{
    ENTITY_PROP_X,              /* world x, pixels */
    ENTITY_PROP_Y,              /* world y, pixels */
    ENTITY_PROP_HEIGHT,         /* height plus its offset, pixels */
    ENTITY_PROP_FACING,         /* one of 8 */
    ENTITY_PROP_HERO_BYTE54,    /* byte 0x54 of the hero (list + 0x200 / 0x204) with this id, else 0xFF */
    ENTITY_PROP_BYTE0,
    ENTITY_PROP_UNK23E,         /* high byte of +0x23E, or (bits 6-7 of its low byte >> 4) | 0x40 */
    ENTITY_PROP_SCREEN_X,
    ENTITY_PROP_SCREEN_Y,
    ENTITY_PROP_SPRITE_UNK23,   /* the sprite's byte 0x23 */
    ENTITY_PROP_TABLE_VALUE,    /* entry_table_value */
    ENTITY_PROP_COLLISION_MASK, /* obj_collision_mask */
};

/* Script command: stores one property of a field entity (position, facing, ...) in a script variable. */
u32 set_var_from_source(u8 *owner, u8 *dst, struct EntityPropCmd *cmd, u8 *vars)
{
    u8 *list = *(u8 **)(owner + 0x14);
    u32 off = cmd->entity << 2;
    u8 *entities = list + 0x28;
    u8 *entity = *(u8 **)(entities + off);
    u32 property = cmd->property;
    s32 value;
    s32 other;

    switch (property)
    {
    case ENTITY_PROP_X:
        value = *(s32 *)(entity + 0xC) / 0x100;
        break;
    case ENTITY_PROP_Y:
        value = *(s32 *)(entity + 0x10) / 0x100;
        break;
    case ENTITY_PROP_HEIGHT:
        value = (*(s32 *)(entity + 0x14) + *(s32 *)(entity + 0x18)) / 0x100;
        break;
    case ENTITY_PROP_FACING:
        value = ((u32)*(u8 *)(entity + 0x24) << 29) >> 29;
        break;
    case ENTITY_PROP_HERO_BYTE54:
    {
        u8 *heroA = *(u8 **)(*(u8 **)(owner + 0x14) + 0x200);
        u8 *heroB = *(u8 **)(*(u8 **)(owner + 0x14) + 0x204);
        if (((struct SignedId7 *)(heroA + 0x33C))->id == cmd->entity)
        {
            value = heroA[0x54];
        }
        else if (((struct SignedId7 *)(heroB + 0x33C))->id == cmd->entity)
        {
            value = heroB[0x54];
        }
        else
        {
            value = 0xFF;
        }
        break;
    }
    case ENTITY_PROP_BYTE0:
        value = *(u8 *)entity;
        break;
    case ENTITY_PROP_UNK23E:
    {
        s32 raw = ((s32)*(u16 *)(entity + 0x23E)) << 16;

        value = raw >> 24;
        if (value == 0)
        {
            value = ((raw >> 20) & 0xC) | 0x40;
        }
        break;
    }
    case ENTITY_PROP_SCREEN_X:
        entity_camera_offset(*(u8 **)(owner + 0x14), cmd->entity, &value, &other);
        break;
    case ENTITY_PROP_SCREEN_Y:
        entity_camera_offset(*(u8 **)(owner + 0x14), cmd->entity, &other, &value);
        break;
    case ENTITY_PROP_SPRITE_UNK23:
        value = *(u8 *)(*(u8 **)(entity + 0x294) + 0x23);
        break;
    case ENTITY_PROP_TABLE_VALUE:
        value = (u8)entry_table_value(*(u8 **)(owner + 0x14), cmd->entity);
        break;
    case ENTITY_PROP_COLLISION_MASK:
        value = obj_collision_mask(*(u8 **)(owner + 0x14), cmd->entity);
        break;
    }
    set_var(vars, dst + 0x18, gUnk_03000FC0 + 0x38C, 0, cmd->var, value);
    return 1;
}

u32 set_state_rect(u32 unused0, u32 unused1, u32 *cmds)
{
    u8 **state = &gEffectState;
    u32 index = *cmds++;
    u8 *entry = *state + (index * 0x2C + 0x320);
    u32 flags = *cmds++;
    u8 *rect = entry + (((flags & 1) << 4) + 0xC);

    if (flags & 2)
    {
        cmds[0] <<= 4;
        cmds[1] <<= 4;
        cmds[2] <<= 4;
        cmds[3] <<= 4;
    }
    *(u16 *)rect = *cmds++;
    *(u16 *)(rect + 6) = *cmds++;
    *(u16 *)(rect + 2) = *cmds++;
    *(u16 *)(rect + 4) = *cmds;
    return 1;
}

u32 set_state_rect_ext(u32 unused0, u32 unused1, u32 *cmds)
{
    u8 **state = &gEffectState;
    u32 index = *cmds++;
    u8 *entry = *state + (index * 0x2C + 0x320);
    u32 flags = *cmds++;
    u8 *target = entry + (((flags & 1) << 4) + 0xC);

    if (flags & 2)
    {
        cmds[0] <<= 4;
        cmds[1] <<= 4;
        cmds[2] <<= 4;
        cmds[3] <<= 4;
        cmds[4] <<= 4;
        cmds[5] <<= 4;
    }
    *(u16 *)target = *cmds++;
    *(u16 *)(target + 6) = *cmds++;
    *(u16 *)(target + 8) = *cmds++;
    *(u16 *)(target + 2) = *cmds++;
    *(u16 *)(target + 4) = *cmds++;
    *(u16 *)(target + 0xA) = *cmds;
    return 1;
}

u32 set_state_slot(u32 unused0, u32 value, u32 unused1, u32 *cmds)
{
    u8 *state = gEffectState;
    u8 *entry;

    ((struct EffectStateMask *)state)->slotMask |= 1 << *cmds;
    entry = state + (*cmds++ * 0x2C + 0x320);
    entry[9] = *cmds++;
    entry[0xA] = *cmds++;
    ((struct EffectSlotBits *)(entry + 0xB))->flag = *cmds++;
    entry[8] = *cmds++;
    *(u32 *)(entry + 4) = *cmds;
    if (value != 0)
    {
        *(u32 *)entry = value;
    }
    return 1;
}

/* gEffectState holds 4 script threads of EFFECT_SLOT_SIZE bytes; their u16 flags sit at +0xA0 */
#define SCRIPT_THREAD_RUNNING    0x0001 /* the thread is executing its script */
#define SCRIPT_THREAD_WAIT_TIMER 0x0010 /* waits until its countdown at +0x9C reaches 0 */
#define SCRIPT_THREAD_WAIT_OWNER 0x0100 /* waits until the owner's sprites have no pending bits */
#define SCRIPT_THREAD_3_OFFSET   0x1F8  /* the fourth thread, 3 * EFFECT_SLOT_SIZE */
#define EFFECT_CUR_THREAD        0x548  /* u8: index of the thread being stepped */

void step_status_1f8(u8 **obj);
void sprites_obj_step_move(u8 *list);
void decode_stream_run(u8 *record, u32 *buf, u8 *gfx, u8 *vars, u8 *extra, u32 kind);
u32 obj_command_dispatch(u8 *owner, u32 zero, u8 *record, u32 *buf);
u32 step_effect_slot(u8 *owner, u32 zero, u8 *record);

/* Steps script thread 3 (the fourth one in gEffectState) for one frame: updates its waits, and if
 * it is running and not waiting, runs its script commands until one yields or fails. While it
 * waits on the owner, the owner's sprites keep moving instead. */
void step_object_effects(u8 **obj)
{
  u8 *curThread = gEffectState + EFFECT_CUR_THREAD;
  u8 *thread;
  u8 *owner;
  u32 flags;
  u32 cmd[13];
  u32 done;
  step_status_1f8(obj);
  *curThread = 3;
  thread = gEffectState + SCRIPT_THREAD_3_OFFSET;
  flags = *((u16 *) (gEffectState + SCRIPT_THREAD_3_OFFSET + 0xA0));
  if ((flags & SCRIPT_THREAD_WAIT_OWNER) != 0)
  {
    sprites_obj_step_move(*((u8 **) ((*obj) + 0x14)));
    return;
  }
  if ((flags & (SCRIPT_THREAD_WAIT_TIMER | 0x4)) != 0)
  {
    return;
  }
  owner = *obj;
  {
    /* not running? */
    register u16 one asm("r1") = SCRIPT_THREAD_RUNNING;
    register u16 stopped asm("r0") = one ^ (u16)flags;
    if ((stopped & one) != 0)
    {
      return;
    }
  }
  done = (u8) step_effect_slot(owner, 0, thread);
  while (done == 0)
  {
    decode_stream_run(thread, cmd, gUnk_03000FC0, gEffectState + 0x4B0, gUnk_03000FC0 + 0x38C, done);
    if (((u8) obj_command_dispatch(owner, 0, thread, cmd)) == 0)
    {
      break;
    }
    done = (u8) step_effect_slot(owner, 0, thread);
  }
}

extern const u8 gEffectStateTag[];
extern u32 gTextExtraFlagPtrs[];
extern u32 gTextCountPtrs[];
extern u32 gTextKindPtrs[];
extern u32 gTextModePtrs[];
extern u32 gTextHalfPtrs[];
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void task_insert(void *node, u8 kind, u32 data);
/*
 * Constructor of the effect-script task (C++-style, with the task as a virtual base: self[0] points
 * to it). With inCharge set it also builds that base task (task_insert, at self + 4); then it installs
 * gTaskTemplate as the task's handler table, or, when a derived constructor owns the base, a stack
 * copy whose offset (+8) is adjusted to this subobject. It allocates and clears gEffectState (0x560
 * bytes), selects the default text tables (entry 0) in the var block, and clears the text layer
 * (gUnk_03000FCC), the slot table (gUnk_03000FC8) and gCurObj bytes 0x26-0x27.
 */
u8 *init_effect_state(u8 *self, u32 inCharge, u32 kind, u32 data)
{
    u32 vtblCopy[4];
    u8 *baseTask;
    u8 *task;
    u8 *copy;
    u8 *taskStart;
    u8 *vars;
    u8 *curObj;
    u8 prio;

    prio = kind;
    if (inCharge != 0)
    {
        baseTask = self + 4;
        *(u8 **)self = baseTask;
        task_insert(baseTask, prio, data);
        *(const u8 **)(baseTask + 0x18) = gDefaultTaskDesc;
    }
    task = *(u8 **)self;
    *(u8 **)(task + 0x18) = gTaskTemplate;
    if (inCharge == 0)
    {
        *(struct TaskCopy *)vtblCopy = *(struct TaskCopy *)gTaskTemplate;
        *(u8 **)(task + 0x18) = (u8 *)vtblCopy;
        copy = (u8 *)vtblCopy;
        taskStart = task - 4;
        *(u16 *)(copy + 8) = (u16)((u32)self - (u32)taskStart + *(u16 *)(gTaskTemplate + 8));
    }

    gEffectState = heap_alloc_raw(1, 0x560, gEffectStateTag);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gEffectState, 0x560);

    /* the default text tables */
    vars = gUnk_03000FC0;
    *(u32 *)(vars + 0x55C) = gTextExtraFlagPtrs[0];
    *(u32 *)(vars + 0x560) = gTextCountPtrs[0];
    *(u32 *)(vars + 0x564) = gTextKindPtrs[0];
    *(u32 *)(vars + 0x568) = gTextModePtrs[0];
    *(u32 *)(vars + 0x56C) = gTextHalfPtrs[0];

    gUnk_03000FCC = 0;
    gUnk_03000FC8 = 0;
    curObj = gCurObj;
    curObj[0x27] = 0;
    curObj[0x26] = 0;
    return self;
}

void update_object_states(u8 **task);
void step_bobbing_offset(u8 **task);
void advance_motion_slots(u8 **task);
NONMATCH("asm/code_080EF3BC/update_object_effects.s", void update_object_effects(u8 **obj))
{
    u8 *counter;
    u8 *slot;
    u8 *owner;
    u32 buf[13];
    u32 kind;

    counter = gEffectState + 0x548;
    update_object_states(obj);
    *counter = 0;
    slot = gEffectState;
    do
    {
        u16 flags = *(u16 *)(slot + 0xA0);

        if ((flags & 0x100) != 0)
        {
            sprites_obj_step_move(*(u8 **)(*obj + 0x14));
        }
        else if ((flags & 0x14) == 0)
        {
            owner = *obj;
            if ((((u16)flags ^ 1) & 1) == 0)
            {
                kind = (u8)step_effect_slot(owner, 0, slot);
                while (kind == 0)
                {
                    decode_stream_run(slot, buf, gUnk_03000FC0, gEffectState + 0x4B0, gUnk_03000FC0 + 0x38C, kind);
                    if ((u8)obj_command_dispatch(owner, 0, slot, buf) == 0)
                    {
                        break;
                    }
                    kind = (u8)step_effect_slot(owner, 0, slot);
                }
            }
        }
        (*counter)++;
        slot += 0xA8;
    } while (*counter <= 2);
    update_owner_objects(*obj);
    step_bobbing_offset(obj);
    advance_motion_slots(obj);
    if (*(u8 *)(*(u8 **)(gUnk_03000FCC + 4) + 0x1B2) != 0)
    {
        *(u8 *)(*(u8 **)(gUnk_03000FCC + 4) + 0x1B2) = text_layer_step(gUnk_03000FCC);
        update_box_and_scroll(gUnk_03000FCC);
    }
}
END_NONMATCH


ASM_FUNC("asm/code_080EF3BC/show_text_window.s", void show_text_window(void))

u32 slot_countdown(u8 *pool, u32 idx);

struct EffectSlotPos
{
    u8 filler0[8];
    s16 x;
    s16 y;
};

void update_effect_slots(u8 **task)
{
    u8 *slot;
    s16 i;
    s32 x;
    s32 y;
    u8 idx;
    struct EffectSlotPos *pp;

    i = 0;
    slot = *(u8 **)gUnk_03000FC8;
    for (; i <= 3; i++, slot += 0x10)
    {
        if ((u8)slot_countdown(gUnk_03000FC8, (idx = i)) != 0)
        {
            if (slot[0xE] != 0)
            {
                u8 *list = *(u8 **)(*task + 0x14);
                u32 off = slot[0xC] << 2;
                u8 *base = list + 0x28;
                u8 *z = *(u8 **)(base + off);
                entity_camera_offset(list, slot[0xC], &x, &y);
                y = y - z[0x283];
                pp = (struct EffectSlotPos *)slot;
                pp->x = x;
                pp->y = y;
            }
            update_slot_bounds(gUnk_03000FC8, idx);
        }
    }
}

void apply_key_filter(u8 *list);
/* Starts effect script `script` for a task: clears gEffectState's flag bits 1 and 3-5 (+0x55A),
 * zeroes its halfwords at +0x554/+0x556/+0x558 and sets the counter at +0x549 to 16, applies the
 * key filter to the task's scene, restarts each of the scene's members on its own script (member +0x58,
 * script pointer at +0x228), calls the task's virtual hook (vtable +8: this offset, +0xC: function)
 * and finally starts `script` in gEffectState. */
void start_effect(u8 **task, u32 script)
{
    u8 *scene;
    u8 **member;
    s16 left;
    u8 *vtbl;

    ((s8 *)gEffectState)[0x55A] &= ~0x02;
    ((s8 *)gEffectState)[0x55A] &= ~0x38;
    {
        u8 *state = gEffectState;
        *(u16 *)(state + 0x558) = 0;
        *(u16 *)(state + 0x556) = 0;
        *(u16 *)(state + 0x554) = 0;
        state[0x549] = 0x10;
    }
    apply_key_filter(*(u8 **)(*task + 0x14));
    scene = *(u8 **)(*task + 0x14);
    left = scene[0x10B]; /* member count */
    member = (u8 **)(scene + 0x28);
    if (left != 0)
    {
        do
        {
            reset_obj_params_b(*task + 0x1C, *member + 0x58, *(u32 *)(*member + 0x228), 0, 1, 0xFF);
            member++;
            left--;
        } while (left > 0);
    }
    vtbl = *(u8 **)(*task + 0x18);
    ((void (*)(u8 *))(*(u32 *)(vtbl + 0xC)))(*task + *(s16 *)(vtbl + 8));
    reset_obj_params_b(*task + 0x1C, gEffectState, script, 0, 1, 0xFF);
}

ASM_FUNC("asm/code_080EF3BC/register_effect_entry.s", void register_effect_entry(void))

/* Sets BG0 up as the text layer: zeroes its scroll, points BG0CNT at the given screen and
   character blocks (VRAM offsets), creates the text object (gUnk_03000FCC) on those tiles, fills
   the VRAM map and a new 0x800-byte map buffer (gEffectState + 0x4AC) with tile 0x180, clears the
   first tile and hands everything to flags_vars_setup_bg_layer. */
void update_setup_bg_layer(u32 unused, u16 screenOfs, u16 charOfs)
{
    u8 *state = gEffectState;
    u32 screen = screenOfs;
    u32 chars = charOfs;
    u16 *reg = gUnk_02000010; /* BG0HOFS shadow */
    u8 *mapBuf;
    u32 zero = 0;

    *reg = zero;
    reg++;
    *reg = zero; /* BG0VOFS */
    reg -= 5;
    /* BG0CNT shadow: screen base block in bits 8-12, character base block in bits 2-3 */
    *reg = (((screen >> 11) & 0x1F) << 8) | (((chars >> 14) & 3) << 2);
    gUnk_03000FCC = init_text_obj(heap_alloc2(8, 0, gBgLayerTag, zero), 0x06000020 + chars, 0x4AE0, 6);
    ((void (*)(u32, void *, u32))gMemRoutines)(0x01800180, (void *)(0x06000000 + screen), 0x800);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)(0x06000000 + chars), 0x20);
    mapBuf = heap_alloc_raw(1, 0x800, gBgBufferTag);
    *(u8 **)(gEffectState + 0x4AC) = mapBuf;
    ((void (*)(u32, void *, u32))gMemRoutines)(0x01800180, mapBuf, 0x800);
    flags_vars_setup_bg_layer(gUnk_03000FCC, 0, 0xF, *(u8 **)(gEffectState + 0x4AC), 2, 0x180, state + 0x540, state + 0x538, state + 0x528);
}


void cpu_set(const void *src, void *dst, u32 ctl);
/* CpuSet control: fill `count` halfwords with the value at src (bit 24: fixed source). */
#define CPU_SET_FILL_16(count) (0x01000000 | (count))

#define EFFECT_SLOT_SIZE 0xA8 /* gEffectState holds 3 effect-script slots of this size */

/* Clears the effect-script state for a new run: the three script slots (the loop's fourth pass only
   steps past them), the halfword at +0x544, 8 bytes of the var block at +0x38C and its 16 halfwords
   at +0x53C, the counters at +0x549/+0x54A/+0x550, flags 0x55A bits 0 and 6-7, and bits 5-6 of
   0x55B and of 0x55F (the screen shake's mode, BobbingState.mode). */
void reset_effect_state(void)
{
    u8 *slot = gEffectState;
    s16 i;
    vu16 zero;

    for (i = 0; i <= 3; i++)
    {
        if (i != 3)
        {
            ((void (*)(u32, void *, u32))gMemRoutines)(0, slot, EFFECT_SLOT_SIZE);
        }
        slot += EFFECT_SLOT_SIZE;
    }
    zero = 0;
    cpu_set((void *)&zero, gEffectState + 0x544, CPU_SET_FILL_16(1));
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FC0 + 0x38C, 8);
    zero = 0;
    cpu_set((void *)&zero, gUnk_03000FC0 + 0x53C, CPU_SET_FILL_16(0x10));
    ((s8 *)gEffectState)[0x55A] &= ~0x01;
    ((s8 *)gEffectState)[0x55A] &= 0x3F;
    {
        u8 *state = gEffectState;
        *(u32 *)(state + 0x550) = 0;
        *(u16 *)(state + 0x54A) = 0;
        state[0x549] = 0;
    }
    ((s8 *)gEffectState)[0x55B] &= ~0x60;
    ((s8 *)gEffectState)[0x55F] &= ~0x60;
}

/* Screen shake in gEffectState: the offset swings between -amp and +amp by step per frame. */
struct BobbingState
{
    u8 filler0[0x55D];
    s8 step;
    u16 count : 6;  /* swings left; 0 shakes until mode is set to BOB_MODE_SETTLE */
    u16 amp : 7;
    u16 mode : 2;
    u16 useAlt : 1; /* shake the y offset instead of x */
};

#define BOB_MODE_SETTLE 1 /* stop once the offset is back within one step of 0 */

/* The byte at gEffectState+0x55F, holding BobbingState.mode. */
struct BobbingModeByte
{
    u8 low5 : 5;
    u8 mode : 2;
    u8 high : 1;
};

/* Shake offsets of the scene (the object at *task + 0x14). */
struct BobOffsets
{
    u8 filler0[0x12E];
    s16 x;
    s16 y;
};

ASM_FUNC("asm/code_080EF3BC/update_object_states.s", void update_object_states(u8 **task))

/* Advance the screen shake by one frame: move the scene's x (or y) offset by step, bounce it
 * back at +-amp (counting the swings down), and in BOB_MODE_SETTLE end the shake once the
 * offset is within one step of 0. */
void step_bobbing_offset(u8 **task)
{
    u16 edge;
    u16 magnitude;
    s16 *offset;
    s16 value;

    if (((struct BobbingState *)gEffectState)->count == 0 && ((struct BobbingState *)gEffectState)->mode == 0)
        return;
    if (((struct BobbingState *)gEffectState)->useAlt)
        offset = &(*(struct BobOffsets **)(*task + 0x14))->y;
    else
        offset = &(*(struct BobOffsets **)(*task + 0x14))->x;
    *offset += ((struct BobbingState *)gEffectState)->step;
    if (*offset >= 0)
    {
        magnitude = *offset;
        edge = ((struct BobbingState *)gEffectState)->amp;
    }
    else
    {
        magnitude = -*offset;
        edge = -((struct BobbingState *)gEffectState)->amp;
    }
    if ((s16)magnitude >= ((struct BobbingState *)gEffectState)->amp)
    {
        if (((struct BobbingState *)gEffectState)->count != 0)
        {
            if (--((struct BobbingState *)gEffectState)->count == 0)
            {
                ((struct BobbingState *)gEffectState)->mode = BOB_MODE_SETTLE;
            }
        }
        do
        {
            *offset = edge;
        } while (0);
        ((struct BobbingState *)gEffectState)->step *= -1;
    }
    /* the same variable holds the offset of the mode byte */
    edge = 0x55F;
    if (((struct BobbingModeByte *)&gEffectState[edge])->mode == BOB_MODE_SETTLE)
    {
        edge = ((struct BobbingState *)gEffectState)->step;
        magnitude = *offset;
        value = *offset;
        if (value < 0)
            magnitude = value * -1;
        if ((s16)edge < 0)
            edge = (s16)edge * -1;
        if ((s16)magnitude - (s16)edge <= 0)
        {
            (*(struct BobOffsets **)(*task + 0x14))->y = 0;
            (*(struct BobOffsets **)(*task + 0x14))->x = 0;
            ((struct BobbingState *)*(u8 *volatile *)&gEffectState)->mode = 0;
        }
    }
}

/*
 * One of the four slots at gEffectState + 0x2A0 that move a point toward a target
 * (positions in 1/0x100 pixels). Slot i runs while bit i of gEffectState[0x55B]
 * bits 1-4 is set.
 */
struct MotionSlot
{
    s32 x;
    s32 y;
    s32 xBase;  /* x rounded down to a whole pixel, at most 0xFF behind it */
    s32 yBase;
    s16 xLim;   /* target, in pixels */
    s16 yLim;
    s32 vx;
    s32 vy;
    u8 filler1C[0x20 - 0x1C];
};

void offset_obj_pos_by_mode(u8 *obj, u32 kind, s32 dx, s32 dy);

/*
 * Moves each running motion slot one step toward its target, axis by axis, stopping
 * at it. Slots 1 and 2 move the owner's objects (list + 0x304) by the whole pixels
 * moved since their base; a slot that has reached its target on both axes stops.
 */
void advance_motion_slots(u8 **task)
{
    struct MotionSlot *slot;
    s32 i;
    s16 arrived;
    s32 pixelMask;
    s32 running;

    slot = (struct MotionSlot *)(gEffectState + 0x2A0);
    i = 0;
    pixelMask = ~0xFF;
    for (; i <= 3; slot++, i++)
    {
        running = ((u32)gEffectState[0x55B] << 27) >> 28;   /* bits 1-4 */
        if (((running >> i) & 1) != 0)
        {
            /* bit 0: x has arrived, bit 1: y has */
            arrived = 0;
            if (slot->vx >= 0)
            {
                if (slot->x - slot->xBase > 0xFF)
                {
                    slot->xBase = slot->x & pixelMask;
                }
                slot->x = slot->x + slot->vx;
                if (((s32)slot->xLim << 8) <= slot->x)
                {
                    slot->x = (s32)slot->xLim << 8;
                    arrived = 1;
                }
            }
            else
            {
                if (slot->xBase - slot->x > 0xFF)
                {
                    slot->xBase = slot->x & pixelMask;
                }
                slot->x = slot->x + slot->vx;
                if (((s32)slot->xLim << 8) >= slot->x)
                {
                    slot->x = (s32)slot->xLim << 8;
                    arrived = 1;
                }
            }
            if (slot->vy >= 0)
            {
                if (slot->y - slot->yBase > 0xFF)
                {
                    slot->yBase = slot->y & pixelMask;
                }
                slot->y = slot->y + slot->vy;
                if (((s32)slot->yLim << 8) <= slot->y)
                {
                    slot->y = (s32)slot->yLim << 8;
                    arrived = (s16)(arrived | 2);
                }
            }
            else
            {
                if (slot->yBase - slot->y > 0xFF)
                {
                    slot->yBase = slot->y & pixelMask;
                }
                slot->y = slot->y + slot->vy;
                if (((s32)slot->yLim << 8) >= slot->y)
                {
                    slot->y = (s32)slot->yLim << 8;
                    arrived = (s16)(arrived | 2);
                }
            }
            switch (i)
            {
            case 1:
            case 2:
            {
                u8 *list = *(u8 **)(*task + 0x14);
                u32 obj = *(u32 *)(list + 0x304);
                u8 kind = i - 1;
                s32 dx = slot->x - slot->xBase;
                s32 dy;
                s32 dxPixels;
                s32 dyPixels;
                if (dx < 0)
                {
                    dx += 0xFF;
                }
                dxPixels = ((s32)((u32)dx << 8)) >> 16;   /* (s16)(dx / 0x100) */
                dy = slot->y - slot->yBase;
                if (dy < 0)
                {
                    dy += 0xFF;
                }
                dyPixels = ((s32)((u32)dy << 8)) >> 16;
                offset_obj_pos_by_mode((u8 *)obj, kind, dxPixels, dyPixels);
            }
            }
            if (arrived == 3)
            {
                /* clear this slot's running bit */
                s32 bits = gEffectState[0x55B];
                s32 slots = ((u32)bits << 27) >> 28;
                s32 left = slots & ~(1 << i);
                s32 field = left << 1;
                gEffectState[0x55B] = (bits & ~0x1E) | field;
            }
        }
    }
}

void free_effect_state(void)
{
    heap_free(*(u8 **)(gEffectState + 0x4AC));
    *(u32 *)(gEffectState + 0x4AC) = 0;
    if (gUnk_03000FCC != 0)
    {
        release_obj_parts(gUnk_03000FCC, 3);
        gUnk_03000FCC = 0;
    }
    if (gUnk_03000FC8 != 0)
    {
        free_ptr_pair(gUnk_03000FC8, 3);
        gUnk_03000FC8 = 0;
    }
}

void init_effect_pool(u32 unused, u16 value, u8 flags)
{
    u8 *pool = gCurObj;

    *(u16 *)(pool + 0x20) = value;
    ((struct EffectPoolBits *)(pool + 0x28))->mode = flags;
    gUnk_03000FC8 = init_slot_table(heap_alloc2(4, 0, gEffectPoolTag, 0));
}

void unknown_thumb_080f0a54(u8 **owner, u8 idx, u32 arg)
{
    u8 *table = *(u8 **)(*owner + 0x14);
    u8 *ent = *(u8 **)(table + idx * 4 + 0x28);
    u8 *state = ent + 0x58;
    u16 *flags = (u16 *)(ent + 0xF8);

    if (*flags & 2)
    {
        obj_finish_move_step(ent);
    }
    if (*flags & 0x40)
    {
        obj_stop_motion(ent);
    }
    if (*flags & 0x20)
    {
        obj_apply_state_a(ent);
    }
    reset_obj_params_b(*owner + 0x1C, state, arg, 0, 1, 0xFF);
}

/* Offsets into gEffectState used by start_effect_slot */
#define EFFECT_THREAD_FLAGS(n)   ((n) * EFFECT_SLOT_SIZE + 0xA0) /* u16 flags of script thread n */
#define EFFECT_SCRIPT_VARS       0x4B0 /* the effect scripts' variable block */
#define EFFECT_SCRIPT_ID         0x54C /* u16: id of the last started script (cmd_copy_if_equal tests it) */
#define EFFECT_FLAGS_55A         0x55A /* u8: bit 0 lets thread 2 take a script when thread 1 is busy */
#define EFFECT_VAR_START_ARG     0x2F  /* script variable that receives the start argument */
#define START_THREAD_1_OR_2      2     /* register_effect_slot kind: thread 1, else thread 2 */

/* Starts effect script `script` on effect thread 1 (thread 2 if 1 is busy and 0x55A bit 0 allows it),
 * after putting `arg` in script variable 0x2F; records `scriptId` at +0x54C. Does nothing if no
 * thread can take it. */
void start_effect_slot(u32 *owner, u32 script, u8 arg, u16 scriptId)
{
    u8 **pstate = &gEffectState;
    u8 *state = *pstate;
    u32 startArgs[2];

    if (*(u16 *)(state + EFFECT_THREAD_FLAGS(1)) & SCRIPT_THREAD_RUNNING)
    {
        if (*(u16 *)(state + EFFECT_THREAD_FLAGS(2)) & SCRIPT_THREAD_RUNNING)
        {
            return;
        }
        if ((*(u8 *)(state + EFFECT_FLAGS_55A) & 1) == 0)
        {
            return;
        }
    }
    startArgs[0] = START_THREAD_1_OR_2;
    startArgs[1] = script;
    set_var(state + EFFECT_SCRIPT_VARS, 0, gUnk_03000FC0 + 0x38C, 0, EFFECT_VAR_START_ARG, arg);
    register_effect_slot(*owner, *pstate + EFFECT_SLOT_SIZE, startArgs);
    *(u16 *)(*pstate + EFFECT_SCRIPT_ID) = scriptId;
}

void refresh_bg_layer(void)
{
    clear_slot_rect(gUnk_03000FCC, 4);
    copy_map_rects(gUnk_03000FCC, 0x10);
}

void teardown_effect_task(u8 *obj, u32 flags)
{
    u8 *record = *(u8 **)obj;
    u32 task[4];
    u8 *owner;
    u8 *base;
    u8 *dst;

    *(u8 **)(record + 0x18) = gTaskTemplate;
    if (flags == 0)
    {
        *(struct TaskCopy *)task = *(struct TaskCopy *)gTaskTemplate;
        *(u8 **)(record + 0x18) = (u8 *)task;
        dst = (u8 *)task;
        base = record - 4;
        *(u16 *)(dst + 8) = (u16)((u32)obj - (u32)base + *(u16 *)(gTaskTemplate + 8));
    }

    text_window_close();
    heap_free(gEffectState);
    if (flags & 2)
    {
        owner = *(u8 **)obj;
        *(const u8 **)(owner + 0x18) = gDefaultTaskDesc;
        task_remove(owner, 0);
    }
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}
