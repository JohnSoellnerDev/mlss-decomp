#include "global.h"
#include "ram_routines.h"

struct World
{
    u8 filler0[4];
    u8 unk4;
    u8 filler5[0x18 - 5];
    u16 unk18;
    u8 filler1A[0x1C - 0x1A];
    u8 *unk1C;
    u8 filler20[0x24 - 0x20];
    void (*callback24)(void);
    u8 filler28[0x58 - 0x28];
    u8 *slots[8];
    u8 *unk78;
    u8 *curObj;
    u8 *fig1; /* the two battle figures (SlotObj; hp at +0xF6, sprite object at +8) */
    u8 *fig2;
    u8 filler88[0x248 - 0x88];
    u8 *unk248; /* the screen's palette fader (sub_08115F54's obj) */
    void *focus; /* the map object the camera follows */
    u8 filler250[0x2B4 - 0x250];
    u16 unk2B4;
    u8 filler2B6[0x2BF - 0x2B6];
    u8 unk2BF;
    u8 filler2C0[0x2E0 - 0x2C0];
    u32 partGfx[3]; /* graphics blocks cmd_world_slot loaded (lookup_or_alloc_13b4) */
    u8 filler2EC[0x2EC - 0x2EC];
    u16 unk2EC;
    u16 unk2EE;
    u8 filler2F0[0x310 - 0x2F0];
    u8 *statusObj; /* the status display's owner (state_status_release) */
    u8 filler314[0x346 - 0x314];
    u8 unk346;
    u8 filler347[0x350 - 0x347];
    u8 unk350;
    u8 unk351;
    u8 filler352[0x354 - 0x352];
    /* Camera scroll, run by world_scroll_step: the focus moves from (scrollStartX, scrollStartY)
     * by (scrollDX, -scrollDY), scrollSpeed sixteenths of a pixel per frame. */
    s16 scrollStartX;
    s16 scrollStartY;
    s16 scrollDX;
    s16 scrollDY;
    u16 scrollSpeed;
    u16 scrollProgress; /* distance covered so far, in sixteenths of a pixel */
};

extern struct World *gWorld;

/* The object in one of gWorld's slots (0x130 bytes; its sprite object starts at +8). */
struct SlotObj
{
    u8 filler0[0x10];
    u8 *sprite;
    u8 filler14[0x7E - 0x14];
    u8 flags7E; /* bits 1-2 (SLOT_LOCK_MASK): both set = scripts can't change its fields */
    u8 filler7F[0xA4 - 0x7F];
    u32 unkA4;
    u32 unkA8;
    u32 unkAC;
    u8 fillerB0[0xBC - 0xB0];
    u32 unkBC;
    u16 unkC0;
    u16 unkC2;
    u16 pointX; /* a point in whole pixels; slot_follow_source offsets it */
    u16 pointY;
    u16 pointZ;
    u8 fillerCA[0xD8 - 0xCA];
    s32 homeX; /* where place_part_in_slot put it, in 1/256 pixels */
    s32 homeY;
    s32 homeZ;
    u8 fillerE4[0xEC - 0xE4];
    s16 partId; /* gPartTemplates index, or < 0 for a party member */
    u8 fillerEE[0xF6 - 0xEE];
    u16 hp;
    u16 maxHp;
    u8 fillerFA[0xFC - 0xFA];
    u16 unkFC;
    u8 fillerFE[0x102 - 0xFE];
    u16 unk102;
    u16 defense;
    u16 unk106;
    u8 filler108[0x10E - 0x108];
    u16 unk10E;
    u8 flags110lo : 6;
    u8 placeFlag : 1; /* the flag place_part_in_slot was given */
    u8 flags110hi : 1;
    u8 status; /* bit 1 (SLOT_STATUS_BUSY): it can't be stopped this frame */
    u8 filler112[0x11E - 0x112];
    u16 motionFlags; /* which anim_set_motion_* moves are running */
};

/* The sprite object at +8 of a SlotObj. */
struct SlotSprite
{
    u8 filler0[0x10];
    s32 x; /* 1/256 pixels */
    s32 y;
    s32 height;
    u8 filler1C[0x38 - 0x1C];
    s32 unk38; /* another x, y, z, in 1/256 pixels */
    s32 unk3C;
    s32 unk40;
};

#define SLOT_STATUS_BUSY 2
#define SLOT_LOCK_MASK 6 /* SlotObj.flags7E bits 1-2 */

/* Which field cmd_slot_set_field writes (cmd[2]). */
enum SlotField
{
    SLOT_FIELD_HP = 8,
    SLOT_FIELD_102 = 9,       /* party members only */
    SLOT_FIELD_106 = 10,
    SLOT_FIELD_FC = 11,       /* party members only */
    SLOT_FIELD_DEFENSE = 15,
    SLOT_FIELD_ANIM = 17,     /* runs anim_dispatch instead */
    SLOT_FIELD_A4 = 18,
    SLOT_FIELD_A8 = 19,
    SLOT_FIELD_AC = 20,
    SLOT_FIELD_10E = 24,
    SLOT_FIELD_LOCK = 25,     /* SlotObj.flags7E bits 1-2 */
    SLOT_FIELD_WORLD_18 = 0x1F /* gWorld->unk18, no slot */
};

/* The script thread a command runs in. */
struct ScriptThread
{
    u32 *pc;
    u8 filler4[0x14 - 0x4];
    u32 *cmdStart; /* the command being run: pc = cmdStart runs it again next frame */
};

/* Flags in the u16 at +0xA0 of a script object. */
#define SCRIPT_OBJ_SCROLLING 0x100 /* world_scroll_step moves the camera each frame */

/* Three 10-bit fields packed in the word at gWorld+0x2EC: which graphics entry cmd_world_slot loads
 * into gWorld->partGfx[0-2]. */
struct WorldBits2EC
{
    u8 filler0[0x2EC];
    u32 partGfxIndex0 : 10;
    u32 partGfxIndex1 : 10;
    u32 partGfxIndex2 : 10;
    u32 rest : 2;
};

struct SlotByteFD
{
    u8 filler0[0xFD];
    u8 lo : 4;
    u8 hi : 4;
};

struct SlotBits7F
{
    u8 filler0[0x7F];
    u8 lo7 : 7;
    u8 bit7 : 1;
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

struct WorldBits2BF
{
    u8 filler0[0x2BF];
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct OwnerState
{
    u8 filler0[0x179C];
    u8 slotCode;
};

struct Owner
{
    u8 filler0[0x2C];
    struct OwnerState *state;
};

/* One of the 4 overlay sprites a script can show (cmd_spawn_obj claims a free one). */
struct SlotEntry
{
    struct SlotPos *sprite;
    u16 baseX;   /* screen position the script asked for */
    u16 baseY;
    u16 offsetX; /* added to it: the followed entity's position, or 0 */
    u16 offsetY;
};

struct SlotPos
{
    u16 x;
    u16 y;
    u8 filler4[0xA];
    u16 depth; /* 0x7FFF - y: lower sprites draw in front */
};

/* Moves entry idx's sprite to its base position plus its offset. */
static inline void slot_entry_place_sprite(u8 **table, u8 idx)
{
    u32 off = (u8)idx << 4;
    u8 *base = *table;
    struct SlotPos *sprite;
    struct SlotEntry *entry;
    u32 y;

    base += off;
    entry = (struct SlotEntry *)base;
    sprite = entry->sprite;
    sprite->x = entry->baseX + entry->offsetX;
    y = entry->offsetY;
    base = (u8 *)(u32)entry->baseY;
    y += (u32)base;
    sprite->y = y;
    sprite->depth = 0x7FFF - y;
}

void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, u8 *owner);
void list_splice_pair(u8 *entry, u8 *head);
s32 anim_set_motion_a(u8 *obj, u16 speed, s32 extra);
s32 anim_set_motion_b(u8 *obj, u16 speed, s32 extra);
s32 anim_set_motion_c(u8 *obj, u16 speed, s32 extra);
void obj_stop(u8 *obj);
void obj_draw_part(u8 *obj, s32 amount, s32 kind, s32 a3, s32 a4, s32 a5);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
extern u8 *gWorkBuf;
extern u8 gUnk_02000080[];
void menu_open_setup(void);
void sub_08115F54(u8 *a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);
extern u8 gClaimKindTable[];
void slot_pos_offset(u8 *obj, s32 idx, s32 *x, s32 *y, s32 z);
s32 cmd_spawn_obj(u8 *table, u32 art, u32 sub, u32 width, s32 a, s32 b, s32 c);
void state_status_release(u8 *value);
void release_marker_object(void);
void spawn_linked_children(void);
void spawn_timer_child(void);
void sprite_set_action(u8 *obj, u32 a, u32 b, u32 c);
void obj_track_target(u8 *target);
void state_update_hero_markers(void);
void anim_dispatch(u8 *obj, s32 value, u32 a, u32 b);
struct PartTemplate
{
    u8 filler0[8];
    u8 *sprite;
    u8 filler0C[0x2C - 0x0C];
};

extern const struct PartTemplate gPartTemplates[];

static inline u8 *slot_entity(s32 idx)
{
    if (idx < 8)
        return gWorld->slots[idx] + 8;
    return (u8 *)((struct { u8 data[0xD0]; } *)(gWorkBuf + 0x980) + idx - 8);
}
u32 lookup_or_alloc_13b4(u32 a, u32 key, s32 b, s32 c);
void release_block(void *block);
void place_part_in_slot(u32 effect, s32 slot, s32 x, s32 y, s32 z, s32 a, s32 b);
void noop_callback_0808261C(void);
void run_slot_script(u32 index);
void sprite_entry_update(u8 *obj);
void release_object(u8 *obj);
void release_object_data(u8 *obj);
void task_list_remove(u8 *obj);
void process_record_ring(u8 *obj);
void ensure_code_entry(u32 a);
void alloc_code_entry(u32 a);
void load_table_entries(u16 a);
void load_table_entries_typed(u16 a);
void obj_detach_active(u8 *obj);

/* one entry of the motion table: three signed 10-bit values and a flag */
struct MotionEntry
{
    s32 x : 10;
    s32 y : 10;
    s32 z : 10;
    u32 flag : 1;
    u32 bit31 : 1;
};

extern const struct MotionEntry gRomMotionTable[][6];
void set_var(u8 *mid, u8 *low, u8 *flagsA, u8 *flagsB, s16 index, u32 value);

/* Script command: sets variable cmd[0] (set_var) to a value of slot cmd[2], picked by cmd[1]:
   0-2: its entity's x, y or height in whole pixels (slots 0-7 are gWorld's, 8 and up gWorkBuf's),
   3: the halfword at +0xEC of the slot, 4-6: its x, y or z in gRomMotionTable for the current
   layout (gWorld->unk1C[1]), 7-9: the same plus the slot's offsets (bytes 4-6 of its data at +0xE4). */
u32 cmd_set_var_from_slot_field(struct Owner *owner, u8 *dst, s32 *cmd, u8 *extra)
{
    u16 value;
    u8 *entity;

    switch (cmd[1])
    {
    case 0:
        if (cmd[2] <= 7)
            entity = gWorld->slots[cmd[2]] + 8;
        else
            entity = (u8 *)((struct { u8 data[0xD0]; } *)(gWorkBuf + 0x980) + cmd[2] - 8);
        value = *(s32 *)(entity + 0x10) / 0x100;
        break;
    case 1:
        if (cmd[2] <= 7)
            entity = gWorld->slots[cmd[2]] + 8;
        else
            entity = (u8 *)((struct { u8 data[0xD0]; } *)(gWorkBuf + 0x980) + cmd[2] - 8);
        value = *(s32 *)(entity + 0x14) / 0x100;
        break;
    case 2:
        if (cmd[2] <= 7)
            entity = gWorld->slots[cmd[2]] + 8;
        else
            entity = (u8 *)((struct { u8 data[0xD0]; } *)(gWorkBuf + 0x980) + cmd[2] - 8);
        value = *(s32 *)(entity + 0x18) / 0x100;
        break;
    case 3:
        value = *(u16 *)(gWorld->slots[cmd[2]] + 0xEC);
        break;
    case 4:
        value = gRomMotionTable[gWorld->unk1C[1]][cmd[2]].x;
        break;
    case 5:
        value = gRomMotionTable[gWorld->unk1C[1]][cmd[2]].y;
        break;
    case 6:
        value = gRomMotionTable[gWorld->unk1C[1]][cmd[2]].z;
        break;
    case 7:
        value = gRomMotionTable[gWorld->unk1C[1]][cmd[2]].x
              + *(s8 *)(*(u8 **)(gWorld->slots[cmd[2]] + 0xE4) + 4);
        break;
    case 8:
        value = gRomMotionTable[gWorld->unk1C[1]][cmd[2]].y
              + *(s8 *)(*(u8 **)(gWorld->slots[cmd[2]] + 0xE4) + 5);
        break;
    case 9:
        value = gRomMotionTable[gWorld->unk1C[1]][cmd[2]].z
              + *(s8 *)(*(u8 **)(gWorld->slots[cmd[2]] + 0xE4) + 6);
        break;
    }
    set_var(extra, dst + 0x18, (u8 *)owner->state + 0x1794, dst + 0xA8, *(s16 *)cmd, (s16)value);
    return 1;
}


/*
 * Script command: sets field cmd[2] (enum SlotField) of slot cmd[0] to cmd[1]. Field 0x1F sets
 * gWorld->unk18 instead; a slot whose lock bits are both set is left alone, and fields 9 and 11
 * are only written for party members.
 */
u32 cmd_slot_set_field(u32 unused0, u32 unused1, u32 *cmd)
{
    struct SlotObj *slot;
    s32 field;
    u8 *flags;
    u32 bits;
    s32 mask;

    mask = ~SLOT_LOCK_MASK;
    field = cmd[2];
    if (field == SLOT_FIELD_WORLD_18)
    {
        gWorld->unk18 = cmd[1];
        return 1;
    }
    slot = (struct SlotObj *)gWorld->slots[cmd[0]];
    if ((slot->flags7E & SLOT_LOCK_MASK) == SLOT_LOCK_MASK)
    {
        return 1;
    }
    switch (field)
    {
    case SLOT_FIELD_HP:
        {
            u32 value = cmd[1];
            slot->hp = value;
        }
        break;
    case SLOT_FIELD_102:
        if (slot->partId < 0)
        {
            {
                u32 value = cmd[1];
                slot->unk102 = value;
            }
        }
        break;
    case SLOT_FIELD_106:
        {
            u32 value = cmd[1];
            slot->unk106 = value;
        }
        break;
    case SLOT_FIELD_FC:
        if (slot->partId < 0)
        {
            {
                u32 value = cmd[1];
                slot->unkFC = value;
            }
        }
        break;
    case SLOT_FIELD_DEFENSE:
        {
            u32 value = cmd[1];
            slot->defense = value;
        }
        break;
    case SLOT_FIELD_A4:
        slot->unkA4 = cmd[1];
        break;
    case SLOT_FIELD_A8:
        slot->unkA8 = cmd[1];
        break;
    case SLOT_FIELD_AC:
        slot->unkAC = cmd[1];
        break;
    case SLOT_FIELD_10E:
        {
            u32 value = cmd[1];
            slot->unk10E = value;
        }
        break;
    case SLOT_FIELD_LOCK:
        {
            u32 value = cmd[1];

            flags = &slot->flags7E;
            bits = (value & 3) << 1;
            *flags = (*flags & mask) | bits;
        }
        break;
    case SLOT_FIELD_ANIM:
        anim_dispatch((u8 *)slot, cmd[1], 0xC8, 0);
        break;
    }
    return 1;
}

u32 cmd_slot_link_list(u32 unused0, u32 unused1, u32 *cmd)
{
    u32 *cursor;
    u8 *obj;
    u8 *entry;
    s32 mode;
    s32 s;

    cursor = cmd;
    entry = gWorld->slots[*cursor++];
    obj = entry + 8;
    mode = *cursor++;
    switch (mode)
    {
    case 0:
        ((struct SlotBits7F *)entry)->bit3 = ((struct SlotBits7F *)entry)->bit2;
        ((struct SlotBits7F *)entry)->bit2 = 1;
        effect_spawn_at(*cursor, *(s32 *)(obj + 0x38) / 0x100, *(s32 *)(obj + 0x3C) / 0x100,
                        *(s32 *)(obj + 0x40) / 0x100, obj);
        break;
    case 1:
        list_splice_pair(entry, gWorld->slots[*cursor]);
        break;
    case 2:
        s = *cursor;
        *(u16 *)(entry + 0x124) = s;
        break;
    }
    return 1;
}

/* Script command: args are { slot, stat, factor }. Clears the battler's raised/lowered state of
 * stat change `stat` (0: A, 1: B, 2: C, bits 0x0C/0x30/0xC0 at +0x11E) and applies it again with
 * the 8.8 `factor` through anim_set_motion_a/b/c, with a chance that always passes. Returns 1. */
u32 slot_cmds_cmd_slot_set_motion(u32 unused0, u32 unused1, u32 *args)
{
    struct World *world;
    u32 *arg;
    u8 *battler;
    s32 stat;

    arg = args;
    world = gWorld;
    battler = world->slots[*arg++];
    stat = *arg++;
    switch (stat)
    {
    case 0:
        *(u16 *)(battler + 0x11E) = *(u16 *)(battler + 0x11E) & 0xFFF3;
        anim_set_motion_a(battler, *(u16 *)arg, 0x7FFFFFFF);
        break;
    case 1:
        *(u16 *)(battler + 0x11E) = *(u16 *)(battler + 0x11E) & 0xFFCF;
        anim_set_motion_b(battler, *(u16 *)arg, 0x7FFFFFFF);
        break;
    case 2:
        *(u16 *)(battler + 0x11E) = *(u16 *)(battler + 0x11E) & 0xFF3F;
        anim_set_motion_c(battler, *(u16 *)arg, 0x7FFFFFFF);
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F5884/cmd_slot_spawn_a.s", void cmd_slot_spawn_a(void))

ASM_FUNC("asm/code_080F5884/cmd_slot_spawn_b.s", void cmd_slot_spawn_b(void))

/* Script command: cmd[0] picks what to do with the object in slot cmd[1]:
   0: place its part again at its home position and rerun its script, 1: stop it, 2: release it and
   take it off the task list, 3: release_object_data, 4: release_object, 5: stop it and every object
   linked in its ring (process_record_ring). 1 and 5 wait (run the command again next frame) while
   it is busy; returns 0 then, 1 when done. */
u32 cmd_slot_release(u32 unused0, struct ScriptThread *thread, u32 *cmd)
{
    u32 *args;
    u32 action;
    struct SlotObj *slot;
    struct SlotObj *home;
    u8 busy;

    args = cmd;
    action = *args++;
    switch (action)
    {
    case 0:
        home = (struct SlotObj *)gWorld->slots[args[0]];
        place_part_in_slot(args[0], home->partId, home->homeX / 0x100, home->homeY / 0x100,
                           home->homeZ / 0x100, home->placeFlag, 0);
        run_slot_script(args[0]);
        sprite_entry_update(home->sprite);
        break;
    case 1:
        slot = (struct SlotObj *)gWorld->slots[args[0]];
        busy = slot->status & SLOT_STATUS_BUSY;
        if (busy != 0)
        {
            thread->pc = thread->cmdStart;
            return 0;
        }
        obj_stop((u8 *)slot + 8);
        slot->motionFlags = busy;
        break;
    case 3:
        release_object_data(gWorld->slots[args[0]]);
        break;
    case 2:
        slot = (struct SlotObj *)gWorld->slots[args[0]];
        release_object((u8 *)slot);
        task_list_remove((u8 *)slot);
        break;
    case 4:
        release_object(gWorld->slots[args[0]]);
        break;
    case 5:
        slot = (struct SlotObj *)gWorld->slots[args[0]];
        busy = slot->status & SLOT_STATUS_BUSY;
        if (busy != 0)
        {
            thread->pc = thread->cmdStart;
            return 0;
        }
        process_record_ring((u8 *)slot + 8);
        slot->motionFlags = busy;
        break;
    }
    return 1;
}

/* Script command: changes the HP of the object in slot cmd[0]. cmd[1] 0: takes cmd[2] HP off;
   if that leaves none, stops it and sets its HP to 0 (waits, running the command again next frame,
   while it is busy). cmd[1] 1: gives cmd[2] HP back, up to its maximum. Then shows the amount on
   its sprite (kind 0: damage, 3: healing). Returns 0 while waiting, 1 when done. */
u32 cmd_slot_move_step(u32 unused0, struct ScriptThread *thread, u32 *cmd)
{
    struct SlotObj *slot;
    s32 op;
    s32 amount;
    s32 kind;
    s32 hp;
    s32 maxHp;

    slot = (struct SlotObj *)gWorld->slots[*cmd++];
    op = *cmd++;
    switch (op)
    {
    case 0:
        amount = *cmd;
        kind = 0;
        if ((s16)slot->hp <= amount)
        {
            if (slot->status & SLOT_STATUS_BUSY)
            {
                thread->pc = thread->cmdStart;
                return 0;
            }
            obj_stop((u8 *)slot + 8);
            slot->hp = kind;
            slot->motionFlags = kind;
        }
        else
        {
            slot->hp = slot->hp - amount;
        }
        break;
    case 1:
        amount = *cmd;
        kind = 3;
        hp = (s16)slot->hp;
        if (hp + amount > (maxHp = (s16)slot->maxHp))
        {
            amount = maxHp - hp;
        }
        slot->hp = slot->hp + amount;
        break;
    }
    obj_draw_part((u8 *)slot + 8, amount, kind, 0, 0, 0);
    return 1;
}


/* Script command: cmd[0] 0 or 2 sets, 1 or 3 clears flag 0x10 on the current object (gWorld->curObj).
 * The object's sprite object starts at +8, so +0x80 is that sprite object's flags byte (+0x78) and
 * +0x10 its pose (+8), whose flags byte is at +0x12; +0x38 is the sprite object's child (+0x30), which
 * gets the same flag on its own flags byte and pose. Returns 1 (done). */
u32 cmd_slot_set_bits(u32 unused0, u32 unused1, u32 *cmd)
{
    u8 *cur;
    u8 *child;
    s32 mode;

    cur = gWorld->curObj;
    child = *(u8 **)(cur + 0x38);
    mode = *cmd;
    switch (mode)
    {
    case 0:
    case 2:
    {
        u8 *pose;

        cur[0x80] |= 0x10;
        pose = *(u8 **)(cur + 0x10);
        if (pose != 0)
        {
            pose[0x12] |= 0x10;
        }
        if (child != 0)
        {
            child[0x78] |= 0x10;
            pose = *(u8 **)(child + 8);
            if (pose != 0)
            {
                pose[0x12] |= 0x10;
            }
        }
    }
        break;
    case 1:
    case 3:
    {
        s8 *pose;

        ((s8 *)cur)[0x80] &= ~0x10;
        pose = *(s8 **)(cur + 0x10);
        if (pose != 0)
        {
            pose[0x12] &= ~0x10;
        }
        if (child != 0)
        {
            ((s8 *)child)[0x78] &= ~0x10;
            pose = *(s8 **)(child + 8);
            if (pose != 0)
            {
                pose[0x12] &= ~0x10;
            }
        }
    }
        break;
    }
    return 1;
}


u32 cmd_slot_set_bit_pair(u32 unused0, u8 *dst, u32 *cmd)
{
    u8 *p;
    s32 w1;
    s32 mode;

    if (cmd[0] == 0x3F)
    {
        cmd[0] = ((struct SlotByteFD *)dst)->lo;
    }
    p = gWorld->slots[*cmd++];
    w1 = *cmd;
    mode = w1 >> 1;
    switch (mode)
    {
    case 0:
    {
        u8 *q;
        u8 *r;
        s32 bit;
        s32 hi;
        s32 lo;
        s32 flags;

        bit = w1 & 1;
        q = p + 0x7F;
        hi = bit << 7;
        *q = (*q & 0x7F) | hi;
        r = p + 0x80;
        flags = *r;
        flags = (flags & -2) | bit;
        lo = bit << 1;
        flags = (flags & -3) | lo;
        *r = flags;
    }
        break;
    case 1:
    {
        s32 flags;
        s32 old;
        s32 both;
        s32 hi;

        both = w1 & mode;
        p += 0x80;
        hi = both << 2;
        old = *p;
        {
            s32 mask = -5;

            mask &= old;
            flags = mask | hi;
        }
        both <<= 3;
        flags = (flags & -9) | both;
        *p = flags;
    }
        break;
    }
    return 1;
}

/* Script command on the world's slots, by cmd[0]:
   0: (re)loads graphics block args[0] (0-2): entry partGfxIndex<n> of part args[1]'s sprite bank,
      releasing the block loaded there before;
   1: nothing (when args[0] >= 0);
   2: places part args[0] in slot args[1] at the slot's home position (place flag 1, last
      argument args[2] == 0) and runs the slot's script. Always returns 1. */
u32 cmd_world_slot(u32 unused0, u32 unused1, u32 *cmd)
{
    s32 value;
    u32 *args;
    s32 action;
    u32 old;
    u32 block;
    struct SlotObj *home;

    args = cmd;
    action = *args++;
    switch (action)
    {
    case 0:
        if (args[0] == 0)
            value = ((struct WorldBits2EC *)gWorld)->partGfxIndex0;
        else if (args[0] == 1)
            value = ((struct WorldBits2EC *)gWorld)->partGfxIndex1;
        else
            value = ((struct WorldBits2EC *)gWorld)->partGfxIndex2;
        old = gWorld->partGfx[args[0]];
        if (old != 0)
        {
            release_block((void *)old);
        }
        block = lookup_or_alloc_13b4(*(u16 *)gPartTemplates[args[1]].sprite, value, -1, 1);
        gWorld->partGfx[args[0]] = block;
        break;
    case 1:
        if ((s32)args[0] >= 0)
        {
            noop_callback_0808261C();
        }
        break;
    case 2:
        home = (struct SlotObj *)gWorld->slots[args[1]];
        /* value: the place flag */
        place_part_in_slot(args[1], args[0], home->homeX / 0x100, home->homeY / 0x100,
                           home->homeZ / 0x100, 1, value = (args[2] == 0));
        run_slot_script(args[1]);
        break;
    }
    return 1;
}


ASM_FUNC("asm/code_080F5884/cmd_slot_table.s", void cmd_slot_table(void))

/* Script command: makes slot cmd[0] take over another slot's motion. The source is the slot in the
   low nibble of the object's byte 0xFD while the owner's slot code is 0-6, else slot code - 7. Copies
   the source's fields 0xBC-0xC2, sets the point to the source's moved by the distance between the two
   sprites (unk38-unk40, in whole pixels), then clears unkBC. Always returns 1. */
u32 slot_follow_source(struct Owner *owner, u8 *obj, u32 *cmd)
{
    u8 srcIdx;
    struct SlotObj *dst;
    register struct SlotObj *src asm("r3");
    struct SlotSprite *dstSprite;
    struct SlotSprite *srcSprite;
    s32 dist;
    u16 v;
    u16 t;
    u16 *field;

    if (owner->state->slotCode <= 6)
    {
        srcIdx = ((struct SlotByteFD *)obj)->lo;
    }
    else
    {
        srcIdx = owner->state->slotCode - 7;
    }
    dst = (struct SlotObj *)gWorld->slots[*cmd];
    dstSprite = (struct SlotSprite *)((u8 *)dst + 8);
    src = (struct SlotObj *)gWorld->slots[srcIdx];
    srcSprite = (struct SlotSprite *)((u8 *)src + 8);
    dst->unkBC = src->unkBC;
    v = src->unkC0;
    dst->unkC0 = v;
    t = src->unkC2;
    dst->unkC2 = t;
    field = &src->pointX;
    dist = dstSprite->unk38 - srcSprite->unk38;
    if (dist < 0)
    {
        dist += 0xFF;
    }
    v = (dist >> 8) + *field;
    dst->pointX = v;
    field = &src->pointY;
    dist = dstSprite->unk3C - srcSprite->unk3C;
    if (dist < 0)
    {
        dist += 0xFF;
    }
    v = (dist >> 8) + *field;
    field = &dst->pointY;
    *field = v;
    field = &src->pointZ;
    dist = dstSprite->unk40 - srcSprite->unk40;
    if (dist < 0)
    {
        dist += 0xFF;
    }
    v = (dist >> 8) + *field;
    dst->pointZ = v;
    dst->unkBC = 0;
    return 1;
}


ASM_FUNC("asm/code_080F5884/cmd_set_ctx_fields.s", void cmd_set_ctx_fields(void))


/* Script command: scroll the camera cmd[1] pixels in direction cmd[0] (0-7, the odd ones
 * diagonal, where each axis gets distance * sqrt(2) / 2). cmd[2] is the speed: whole pixels per
 * frame, or sixteenths of a pixel with bit 0x40 set. */
u32 cmd_world_scroll(u32 unused0, u8 *obj, u32 *cmd)
{
    s32 sqrt512;
    s32 scaled;
    s32 diagonal;

    sqrt512 = ((s32 (*)(s32))gMathRoutines)(0x200);
    scaled = sqrt512 * cmd[1];
    if (scaled < 0)
    {
        scaled += 0x1F;
    }
    diagonal = scaled >> 5;
    gWorld->scrollDY = 0;
    gWorld->scrollDX = 0;
    switch (cmd[0])
    {
    case 0:
        gWorld->scrollDY = -cmd[1];
        break;
    case 1:
        gWorld->scrollDX = diagonal;
        gWorld->scrollDY = -diagonal;
        break;
    case 2:
        gWorld->scrollDX = cmd[1];
        break;
    case 3:
        gWorld->scrollDX = diagonal;
        gWorld->scrollDY = diagonal;
        break;
    case 4:
        gWorld->scrollDY = cmd[1];
        break;
    case 5:
        gWorld->scrollDX = -diagonal;
        gWorld->scrollDY = diagonal;
        break;
    case 6:
        gWorld->scrollDX = -cmd[1];
        break;
    case 7:
        gWorld->scrollDX = -diagonal;
        gWorld->scrollDY = -diagonal;
        break;
    }
    gWorld->scrollStartX = obj_last_x(gWorld->focus);
    gWorld->scrollStartY = obj_last_y(gWorld->focus);
    gWorld->scrollSpeed = (cmd[2] & 0x40) ? (cmd[2] & 0xF) : (cmd[2] << 4);
    gWorld->scrollProgress = 0;
    {
        register u16 *flags asm("r0") = (u16 *)(obj + 0xA0);
        *flags |= cmd[3] ? (SCRIPT_OBJ_SCROLLING | 0x400) : SCRIPT_OBJ_SCROLLING;
    }
    return 0;
}

/* Script command: scrolls the camera from the focus object's position by (cmd[0], -cmd[1]) pixels.
 * cmd[2] is the speed, in whole pixels per frame, or with bit 0x40 set, its low 4 bits in sixteenths of
 * a pixel. Starts world_scroll_step on the script object (SCRIPT_OBJ_SCROLLING), with flag 0x400 too
 * when cmd[3] is nonzero. */
u32 cmd_set_scroll(u32 unused0, u8 *obj, u32 *cmd)
{
    struct World *world;
    u32 *arg;

    gWorld->scrollStartX = obj_last_x(gWorld->focus);
    gWorld->scrollStartY = obj_last_y(gWorld->focus);
    world = gWorld;
    arg = cmd;
    world->scrollDX = *arg++;
    world->scrollDY = *arg++;
    world->scrollSpeed = (*arg & 0x40) ? (*arg & 0xF) : (*arg << 4);
    gWorld->scrollProgress = 0;
    {
        register u16 *flags asm("r0") = (u16 *)(obj + 0xA0);
        *flags |= arg[1] ? (SCRIPT_OBJ_SCROLLING | 0x400) : SCRIPT_OBJ_SCROLLING;
    }
    return 0;
}

/* Script command: cmd[0] bit 0 clear only sets gWorld bit 0x2BF.5. With it set, when bit 2 is set or
   gWorld->unk4 is nonzero, saves both palette shadows (0x400 bytes) at gWorkBuf + 0x3AF0, sets bit
   0x2BF.3 and fades the whole screen out over 16 frames, to white with bit 1 or else to black;
   otherwise opens the menu (menu_open_setup). */
u32 cmd_flags_show(u32 unused0, u32 unused1, u32 *cmd)
{
    u32 flags;
    s32 colour;
    u8 *fader;

    flags = *cmd;
    if ((flags & 1) != 0)
    {
        if ((flags & 4) != 0 || gWorld->unk4 != 0)
        {
            MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gUnk_02000080, gWorkBuf + 0x3AF0, 0x400);
            ((struct WorldBits2BF *)gWorld)->bit3 = 1;
            fader = gWorld->unk248;
            if ((*cmd & 2) != 0)
            {
                colour = 0x7FFF;
            }
            else
            {
                colour = 0;
            }
            sub_08115F54(fader, 4 /* PALETTE_FADE_OUT */, colour, 0x10, 0xFFFF, 0xFFFF, 0, 0);
        }
        else
        {
            menu_open_setup();
        }
    }
    else
    {
        ((struct WorldBits2BF *)gWorld)->bit5 = 1;
    }
    return 1;
}

/* Script command: cmd[0] picks what to do. 0 sets gWorld bytes 0x350/0x351 to 0x80. 1 ends the
 * current turn's display: releases the status display and the marker, respawns the linked and
 * timer children, resets the active object's sprite, sends each battle figure that still has hp
 * back to its home spot (obj_track_target) and hands gWorld back to state_update_hero_markers.
 * 2 clears gWorld->unk2B4 and bit 3 of the byte at 0x2C1. Always returns 1 (done). */
u32 list_slot_ops_cmd_world_mode(u32 unused0, u32 unused1, s32 *cmd)
{
    u8 *fig;

    switch (*cmd)
    {
    case 0:
        gWorld->unk350 = gWorld->unk351 = 0x80;
        break;
    case 1:
        state_status_release(gWorld->statusObj);
        release_marker_object();
        spawn_linked_children();
        spawn_timer_child();
        sprite_set_action(gWorld->curObj + 8, 0, 0, 0);
        gWorld->unk346 = 0xFF;
        fig = gWorld->fig1;
        if (fig != 0 && *(s16 *)(fig + 0xF6) != 0)
        {
            obj_track_target(fig + 8);
        }
        fig = gWorld->fig2;
        if (fig != 0 && *(s16 *)(fig + 0xF6) != 0)
        {
            obj_track_target(fig + 8);
        }
        gWorld->callback24 = state_update_hero_markers;
        break;
    case 2:
        gWorld->unk2B4 = 0;
        *((s8 *)gWorld + 0x2C1) &= -9;
        break;
    }
    return 1;
}

ASM_FUNC("asm/code_080F5884/draw_list_item.s", void draw_list_item(void))

/* Script command: shows an overlay sprite. args[0] is an entity index, args[1] the sprite kind,
 * args[2] nonzero to place it relative to that entity (its pixel position, raised by its 0xC5
 * byte), args[3]/args[4] the x/y position and args[5] the duration (0xFF: the kind's default
 * from gClaimKindTable). Claims a free entry of the owner's sprite table (owner + 0x28) and
 * places its sprite. Always returns 1 (done). */
u32 cmd_spawn_item(u8 *owner, u32 unused, s32 *args)
{
    s32 offsetX;
    s32 offsetY;
    u8 slot;

    offsetY = 0;
    offsetX = 0;
    if (args[2] != 0)
    {
        slot_pos_offset(owner, *(s16 *)args, &offsetX, &offsetY, 0);
        offsetY -= (s8)slot_entity(args[0])[0xC5];
    }
    if (args[5] == 0xFF)
    {
        args[5] = gClaimKindTable[args[1]];
    }
    slot = cmd_spawn_obj(*(u8 **)(owner + 0x28), (u8)args[0], (u8)args[1], (u8)args[2],
                         *(s16 *)((u8 *)args + 0xC), *(s16 *)((u8 *)args + 0x10), (u8)args[5]);
    if (slot != 0x3F)
    {
        struct SlotEntry *entry = (struct SlotEntry *)(**(u8 ***)(owner + 0x28) + (slot << 4));

        entry->offsetX = offsetX;
        entry->offsetY = offsetY;
        slot_entry_place_sprite(*(u8 ***)(owner + 0x28), slot);
    }
    return 1;
}

