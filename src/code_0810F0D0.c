#include "global.h"

struct ActionRecord
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 display;
};

struct ActionFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 high : 4;
};

struct ActionPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler0E4[0xEC - 0xE4];
    s16 kind;
};

struct ActionObj
{
    u8 filler0[0x08];
    struct ActionRecord *record;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    s16 unk1C;
    s16 unk1E;
    u8 filler20[0x28 - 0x20];
    struct ActionPart *part;
    struct ActionObj *owner;
    struct ActionObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActionObj *);
    u8 filler50[0x58 - 0x50];
    void *callback58;
    void *callback5C;
    void (*handler)(struct ActionObj *);
    u8 filler64[0x75 - 0x64];
    u8 timer;
    u8 ownerFlags;
    u8 markerFlags;
    u8 filler78;
    u8 childFlags;
    u8 filler7A[0x7C - 0x7A];
    s16 motionSpeed;
    u8 filler7E[0x80 - 0x7E];
    s32 link;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler90[0x92 - 0x90];
    u16 effectCount;
    s32 accum;
    s32 effectTimer;
    s32 variant;
    s32 scale;
    s32 display;
    s32 level;
    s16 counter;
    s16 dirX;
    s16 dirY;
    s16 steps;
};

void place_targets(struct ActionObj *obj, s32 x, s32 y, s32 z, s32 extra);
void set_motion_start(struct ActionObj *obj, s32 speed);
void sprite_set_action(struct ActionObj *obj, s32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void wait_move_then_action_15(struct ActionObj *obj);
u8 step_motion(struct ActionObj *obj);
void obj_list_remove(struct ActionObj *obj);
void sound_stop_song(u32 song);
void wait_move_then_action_8(struct ActionObj *obj);
void wait_move_then_place(struct ActionObj *obj);
struct ActionObj *obj_spawn(struct ActionPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void wait_child_then_progress(struct ActionObj *obj);
void partner_link_update(struct ActionObj *obj, struct ActionObj *partner, u32 mode);
u8 candidates_advance_progress(struct ActionObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActionObj *owner);
void obj_release(struct ActionObj *obj);
void wait_move_then_clear(struct ActionObj *obj);
void wait_move_then_start_progress(struct ActionObj *obj);

ASM_FUNC("asm/code_0810F0D0/attach_marker_to_part.s", void attach_marker_to_part(void))

ASM_FUNC("asm/code_0810F0D0/action_factor.s", void action_factor(void))

ASM_FUNC("asm/code_0810F0D0/start_part_place.s", void start_part_place(void))

void start_part_place_b(struct ActionObj *obj)
{
    struct ActionRecord *rec = obj->record;
    if (rec->flags & 8)
    {
        struct ActionPart *part = obj->owner->part;
        s32 t;
        s32 x;
        s32 y;
        s32 z;
        struct ActionRecord *rec2;

        t = part->targetX;
        if (t < 0)
            t += 0xFF;
        x = (t >> 8) + 0x22;
        t = part->targetY;
        if (t < 0)
            t += 0xFF;
        y = t >> 8;
        t = part->targetZ;
        if (t < 0)
            t += 0xFF;
        z = t >> 8;
        place_targets(obj, x, y, z, 0x100);
        set_motion_start(obj, obj->scale);
        sprite_set_action(obj, 5, 0, 0);
        rec2 = obj->record;
        rec2->display = obj->display;
        set_slot_value(0x112, -1);
        obj->state = wait_move_then_action_15;
    }
}

void action_part_states_spawn_part_child(struct ActionObj *obj)
{
    struct ActionRecord *rec = obj->record;
    if (rec->flags & 8)
    {
        struct ActionObj *owner;
        struct ActionObj *child;
        s32 pk;
        u32 kind;
        s32 x;
        s32 y;

        sprite_set_action(obj, 0x11, 0, 0);
        ((struct ActionFlagBits *)&obj->record->flags)->mode = 1;
        owner = obj->owner;
        pk = obj->part->kind;
        kind = 0x40E1;
        if (pk == 0x3C)
            kind--;
        x = owner->posX;
        if (x < 0)
            x += 0xFF;
        x >>= 8;
        y = owner->posY;
        if (y < 0)
            y += 0xFF;
        y >>= 8;
        child = obj_spawn(obj->part, kind, x, y, 0, ((u32)obj->record->unk11 << 25) >> 31, 1);
        obj->child = child;
        sprite_set_action(child, 2, 0, 0);
        ((struct ActionFlagBits *)&child->record->flags)->mode = 1;
        ((struct ActionFlagBits *)&child->childFlags)->bit0 = 1;
        child->timer = owner->timer - 1;
        obj->state = wait_child_then_progress;
    }
}

void wait_child_then_place(struct ActionObj *obj)
{
    struct ActionObj *owner = obj->owner;
    if ((owner->ownerFlags & 0x80) == 0)
    {
        if (owner->posZ == 0)
            partner_link_update(obj, owner, 0xFF);
    }
    if (candidates_advance_progress(obj) == 0)
    {
        struct ActionObj *child = obj->child;
        struct ActionPart *part;
        u32 placeId;
        s32 kind;
        s32 t;
        s32 x;
        s32 y;
        s32 z;

        kind = obj->part->kind;
        placeId = 0x216E;
        if (kind == 0x3C)
            placeId -= 9;
        x = child->baseX;
        if (x < 0)
            x += 0xFF;
        x >>= 8;
        y = child->baseY;
        if (y < 0)
            y += 0xFF;
        y >>= 8;
        z = child->baseZ;
        if (z < 0)
            z += 0xFF;
        z >>= 8;
        effect_spawn_at(placeId, x, y, z, child);
        obj_release(child);
        sound_stop_song(0x10F);
        part = obj->part;
        t = part->targetX;
        if (t < 0)
            t += 0xFF;
        x = t >> 8;
        t = part->targetY;
        if (t < 0)
            t += 0xFF;
        y = t >> 8;
        t = part->targetZ;
        if (t < 0)
            t += 0xFF;
        z = t >> 8;
        place_targets(obj, x, y, z, 0x100);
        set_motion_start(obj, 0x300);
        set_slot_value(0x11B, -1);
        obj->state = wait_move_then_clear;
    }
}

ASM_FUNC("asm/code_0810F0D0/start_part_place_list.s", void start_part_place_list(void))

void start_part_place_c(struct ActionObj *obj)
{
    struct ActionRecord *rec = obj->record;
    if (rec->flags & 8)
    {
        struct ActionPart *part = obj->owner->part;
        s32 t;
        s32 x;
        s32 y;
        s32 z;
        struct ActionRecord *rec2;

        t = part->targetX;
        if (t < 0)
            t += 0xFF;
        x = (t >> 8) + 0x22;
        t = part->targetY;
        if (t < 0)
            t += 0xFF;
        y = t >> 8;
        t = part->targetZ;
        if (t < 0)
            t += 0xFF;
        z = t >> 8;
        place_targets(obj, x, y, z, 0x100);
        set_motion_start(obj, obj->scale);
        sprite_set_action(obj, 5, 0, 0);
        rec2 = obj->record;
        rec2->display = obj->display;
        set_slot_value(0x112, -1);
        obj->state = wait_move_then_start_progress;
    }
}

static inline void place_left_targets(struct ActionObj *obj, s32 y, s32 z)
{
    place_targets(obj, -0x20, y, z, 0x100);
}

u32 effect_from_table_obj(struct ActionObj *obj, u32 id);
void action_handler_return_minus1(struct ActionObj *obj);

void start_part_offset_effect(struct ActionObj *obj)
{
    struct ActionRecord *rec = obj->record;
    if (rec->flags & 8)
    {
        struct ActionPart *part = obj->owner->part;
        s32 t;
        s32 y;
        s32 z;

        t = part->targetY;
        if (t < 0)
            t += 0xFF;
        y = t >> 8;
        t = part->targetZ;
        if (t < 0)
            t += 0xFF;
        z = t >> 8;
        place_left_targets(obj, y, z);
        set_motion_start(obj, 0x300);
        sprite_set_action(obj, 0xC, 0, 0);
        obj->timer = obj->owner->timer - 1;
        effect_from_table_obj(obj, 0x1830);
        obj->handler = action_handler_return_minus1;
        set_slot_value(0x86, -1);
        obj->state = wait_move_then_place;
    }
}

void wait_move_then_place(struct ActionObj *obj)
{
    if (!step_motion(obj))
    {
        struct ActionPart *part;
        s32 t;
        s32 x;
        s32 y;
        s32 z;
        struct ActionRecord *rec2;

        obj_list_remove(obj);
        sound_stop_song(0x86);
        part = obj->part;
        obj->posX = 0x10000;
        obj->baseX = 0x10000;
        t = part->targetY;
        obj->posY = t;
        obj->baseY = t;
        t = part->targetZ;
        obj->posZ = t;
        obj->baseZ = t;
        t = part->targetX;
        if (t < 0)
            t += 0xFF;
        x = t >> 8;
        t = part->targetY;
        if (t < 0)
            t += 0xFF;
        y = t >> 8;
        t = part->targetZ;
        if (t < 0)
            t += 0xFF;
        z = t >> 8;
        place_targets(obj, x, y, z, 0x100);
        set_motion_start(obj, 0x200);
        sprite_set_action(obj, 5, 0, 0);
        rec2 = obj->record;
        rec2->display = obj->display;
        obj->state = wait_move_then_action_8;
    }
}
