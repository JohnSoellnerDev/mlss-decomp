#include "global.h"
#include "ram_routines.h"

struct Record
{
    u8 filler0[0x1A];
    u16 unk1A;
    u8 filler1C[0x2E - 0x1C];
    u16 unk2E;
    u16 unk30;
    u16 unk32;
    u16 unk34;
    u16 speeds[4];
    u8 unk3E;
};

struct Partner
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
};

struct OwnerFlags
{
    u8 unk0 : 2;
    u8 bit2 : 1;
    u8 unk3 : 5;
};

struct Owner
{
    u8 filler0[0x28];
    struct Partner *party[4];
    u8 filler38[0x10B - 0x38];
    u8 unk10B;
    u8 filler10C[0x208 - 0x10C];
    u8 flags208;
};

struct Sprite
{
    u8 filler0[0x11];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x20 - 0x13];
    s8 unk20;
    u8 unk21;
    u8 filler22[0x23 - 0x22];
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    s16 unk2A;
};

struct Flags8
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

typedef void (*EnemyFn)();

struct HitBox
{
    u8 filler0[0x34];
    u16 linkIndex;
    u8 filler36[0x38 - 0x36];
    s16 hitBottom;
    s16 hitTop;
    s16 hitLeft;
    s16 hitRight;
};

struct Enemy
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x34 - 0x25];
    s32 prevX;
    s32 prevY;
    u8 filler3C[0x100 - 0x3C];
    u16 wanderDist[4];
    u16 unk108[4];
    u8 filler110[0x128 - 0x110];
    u16 unk128[4];
    u16 wanderSteps[2];
    u16 linkIndex;
    u8 filler136[0x138 - 0x136];
    s16 hitBottom;
    s16 hitTop;
    s16 hitLeft;
    s16 hitRight;
    u8 filler140[0x198 - 0x140];
    EnemyFn unk198;
    EnemyFn unk19C;
    EnemyFn unk1A0[2];
    u8 filler1A8[0x1AC - 0x1A8];
    EnemyFn unk1AC;
    struct Record *rec;
    s16 *box;
    s16 vecX;
    s16 vecY;
    u8 filler1BC[0x1BE - 0x1BC];
    s16 hopValue;
    u32 hopDist;
    u8 filler1C4[0x1D0 - 0x1C4];
    u16 index;
    u16 unk1D2;
    u16 unk1D4;
    u8 unk1D6;
    u8 filler1D7[0x1D8 - 0x1D7];
    u16 unk1D8;
    u8 filler1DA[0x1DC - 0x1DA];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDZ;
    u8 filler1EC[0x20D - 0x1EC];
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x214 - 0x211];
    u8 unk214;
    u8 filler215[0x216 - 0x215];
    u8 unk216;
    u8 filler217[0x239 - 0x217];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u32 hopZ0;
    u8 filler250[0x254 - 0x250];
    u32 hopSpeed0;
    s32 jumpVz;
    s32 jumpFrames;
    u8 filler260[0x294 - 0x260];
    struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 orbitCenX;
    s32 orbitCenY;
    u8 filler2C4[0x2C8 - 0x2C4];
    s32 orbitCenZ;
    s32 orbitRadX;
    s32 orbitRadY;
    u8 filler2D4[0x2F4 - 0x2D4];
    u32 unk2F4;
    s16 unk2F8;
    u8 filler2FA[0x344 - 0x2FA];
    struct Enemy *body;
};

u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 value);
void obj_set_state(void *obj, u32 state);
void obj_sync_sub_slot(void *obj, u32 value);
void obj_set_cell_coord(struct Enemy *ent, u32 cell, u32 arg);
void flags_obj_set_mode(struct Enemy *ent, u32 mode);
void obj_apply_state_a(void *obj);
u8 find_active_entry(struct Owner *owner, u8 *idx);
void run_state_handler(void);
void tick_store(void);
void obj_pick_next_state(void);
void obj_random_pose(void);
void obj_random_offset(struct Enemy *obj, u32 dir);
void obj_random_offset_step(struct Enemy *obj);
void obj_start_move_to_target(struct Enemy *obj, u32 mode, s32 x, s32 y);
void obj_start_move_quarter(struct Enemy *obj, u32 mode, s32 x, s32 y);
void obj_step_toward_target_08138FDC(struct Enemy *obj);
void obj_pick_move_vector(struct Enemy *obj, struct Partner *entry);
void obj_move_interp(struct Enemy *obj);
void obj_random_facing_target(struct Enemy *obj);
void obj_velocity_from_facing(struct Enemy *obj);
void obj_try_start_hop(struct Enemy *obj);

void obj_hop_landing_step(struct Enemy *obj);
void obj_orbit_circle_step(struct Enemy *obj);
u32 obj_pick_dir_toward_owner(struct Owner *owner, struct Enemy *obj);
void load_record_fields(struct Enemy *obj, s16 index);
void load_record_fields_scaled(struct Enemy *obj, s16 index);
void load_hit_box(struct Enemy *obj, s16 index);
void load_path_points(struct Enemy *obj, s16 index);
void load_part_offsets(struct Enemy *obj, s16 index);
u32 advance_move_step(void *ctx, struct Enemy *obj);
u32 position_blocked(struct Owner *owner, struct Enemy *obj, u32 x, u32 y, u32 z);
u32 div_scaled_fixed(u32 a, u32 x, u32 b, u32 k);
struct Partner *owner_entry(struct Owner *owner, u16 index);
void set_timer_from_table(struct Enemy *obj, s16 index);
void load_hit_box_bytes(struct Enemy *obj, s16 index);
void load_hit_box_words(struct Enemy *obj, s16 index);
void load_record_words_128(struct Enemy *obj, s16 index);
void obj_pick_random_entry(struct Enemy *obj);
void tick_fire_indirect(void);
void sub_08139F08(void);
void body_timer_tick(void *ctx, struct Enemy *ent, EnemyFn *out);
void tick_body_timer(void *ctx, struct Enemy *ent, EnemyFn *out);
void update_hop_phase(struct Enemy *obj);
void obj_jump_arc_step(struct Enemy *obj);
void stub_nop_0813A064(void);
void start_linked_state(struct Owner *owner, struct Enemy *obj);
void set_flags_1d6_copy(void *ctx, struct Enemy *obj, EnemyFn *out);
void advance_handler_slot(void *ctx, struct Enemy *obj, EnemyFn *out);
void link_to_owner_entry(struct Owner *owner, struct Enemy *obj, EnemyFn *out);
void move_step_handler(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_advance(void *ctx, struct Enemy *obj);
void enter_run_state(void *ctx, struct Enemy *obj);
void setup_random_offset_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void enter_idle_hop_state(void *ctx, struct Enemy *obj);
void restart_run_state(void *ctx, struct Enemy *obj);
void set_flag_enter_idle_hop(void *ctx, struct Enemy *obj);
void setup_pick_next_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void setup_owner_offset_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void update_facing_from_motion(void *ctx, struct Enemy *obj);
void move_handler_random_wait(void *ctx, struct Enemy *obj, EnemyFn *out);
void start_random_move_wait(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_hop_landing(void *ctx, struct Enemy *obj);
void obj_callback_apply_and_run(void *ctx, struct Enemy *obj, EnemyFn *slot);
void setup_random_offset_settle(void *ctx, struct Enemy *obj, EnemyFn *out);
void move_handler_timed_callback(void *ctx, struct Enemy *obj, EnemyFn *out);
void setup_offset_target_state(void *ctx, struct Enemy *obj, EnemyFn *out);
void move_handler_timed_settle(void *ctx, struct Enemy *obj, EnemyFn *out);
void setup_random_offset_short(void *ctx, struct Enemy *obj, EnemyFn *out);
void obj_state_run_hop_phase(void *ctx, struct Enemy *obj);
void obj_link_sprites(void);
u16 angle_of_vector16(s32 x, s32 y);
void obj_set_stage(struct Enemy *ent, u32 stage);
void call_start_linked_state(void *ctx, struct Enemy *obj);
void enter_run_state_a311(void *ctx, struct Enemy *obj);
void enter_state_a6cd(void *ctx, struct Enemy *obj);
void enter_state_a3b5(void *ctx, struct Enemy *obj);
void enter_state_hop_landing(void *ctx, struct Enemy *obj);
void enter_run_state_a511(void *ctx, struct Enemy *obj);
void enter_run_state_76b5(void *ctx, struct Enemy *obj);
void enter_run_state_a5d5(void *ctx, struct Enemy *obj);
void enter_idle_hop_a689(void *ctx, struct Enemy *obj);
void reset_record_fields(struct Enemy *obj);
void obj_box_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index);
void obj_box_mirror_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index);
void load_box_oriented(s16 *out, struct Enemy *obj, u32 flags, u32 offset);
void load_hop_entry(struct Enemy *ent, s16 index);

void obj_hop_landing_step(struct Enemy *obj)
{
    if (obj->jumpFrames != -1)
    {
        obj->jumpFrames++;
        obj->unk18 = div_scaled_fixed(obj->jumpFrames, obj->hopDist, obj->hopValue, obj->hopSpeed0) + obj->hopZ0 - obj->posZ;
        if (obj->unk18 <= 0)
        {
            obj->unk18 = 0;
            obj->jumpFrames = -1;
            ((struct Flags8 *)&obj->unk2B5)->bit3 = 0;
        }
    }
}

ASM_FUNC("asm/code_081393B0/obj_orbit_circle_step.s", void obj_orbit_circle_step(struct Enemy *obj))

ASM_FUNC("asm/code_081393B0/obj_pick_dir_toward_owner.s", u32 obj_pick_dir_toward_owner(struct Owner *owner, struct Enemy *obj))

ASM_FUNC("asm/code_081393B0/load_record_fields.s", void load_record_fields(struct Enemy *obj, s16 index))

ASM_FUNC("asm/code_081393B0/load_record_fields_scaled.s", void load_record_fields_scaled(struct Enemy *obj, s16 index))

ASM_FUNC("asm/code_081393B0/load_hit_box.s", void load_hit_box(struct Enemy *obj, s16 index))

ASM_FUNC("asm/code_081393B0/load_path_points.s", void load_path_points(struct Enemy *obj, s16 index))

ASM_FUNC("asm/code_081393B0/load_part_offsets.s", void load_part_offsets(struct Enemy *obj, s16 index))

ASM_FUNC("asm/code_081393B0/advance_move_step.s", u32 advance_move_step(void *ctx, struct Enemy *obj))

ASM_FUNC("asm/code_081393B0/position_blocked.s", u32 position_blocked(struct Owner *owner, struct Enemy *obj, u32 x, u32 y, u32 z))

ASM_FUNC("asm/code_081393B0/div_scaled_fixed.s", u32 div_scaled_fixed(u32 a, u32 x, u32 b, u32 k))

ASM_FUNC("asm/code_081393B0/owner_entry.s", struct Partner *owner_entry(struct Owner *owner, u16 index))

void load_hop_entry(struct Enemy *ent, s16 index)
{
    s16 *entry = (s16 *)ent->rec + index;

    ent->unk2F4 = entry[0];
    ent->unk2F8 = ((u16 *)entry)[1];
}

void set_timer_from_table(struct Enemy *ent, s16 index)
{
    ent->unk1D4 = ((u16 *)ent->rec)[index];
}

void load_hit_box_bytes(struct Enemy *ent, s16 index)
{
    struct HitBox *hit = (struct HitBox *)((u8 *)ent + 0x100);
    s8 *entry = (s8 *)ent->rec + index * 2;

    hit->hitLeft = (entry[0] << 4) + 8;
    entry++;
    hit->hitBottom = (entry[0] << 4) + 8;
    entry++;
    hit->hitRight = (entry[0] << 4) + 8;
    hit->hitTop = (entry[1] << 4) + 8;
}

void load_hit_box_words(struct Enemy *ent, s16 index)
{
    struct HitBox *hit = (struct HitBox *)((u8 *)ent + 0x100);
    s16 *entry = ent->box + index;
    s32 v = entry[0];

    hit->hitLeft = (v << 4) + 8;
    entry++;
    v = entry[0];
    hit->hitBottom = (v << 4) + 8;
    entry++;
    v = entry[0];
    hit->hitRight = (v << 4) + 8;
    v = entry[1];
    hit->hitTop = (v << 4) + 8;
}

void load_record_words_128(struct Enemy *ent, s16 index)
{
    u16 *src = (u16 *)ent->rec + index;
    s16 i;

    for (i = 0; i < 4; i++, src++)
    {
        ent->unk128[i] = *src;
    }
}

void obj_pick_random_entry(struct Enemy *ent)
{
    u16 *dist = ent->wanderDist;
    struct Sprite *sprite;
    s32 pick;
    u16 value;

    pick = sub_081DE850(4, mt19937_next());
    sprite = ent->sprite;
    value = ent->unk108[(u16)pick];
    ent->unk239 = value;
    sprite->unk20 = value;
    dist += (u16)pick;
    ent->unk23E = *dist;
}

void tick_fire_indirect(void)
{
}

void sub_08139F08(void)
{
}

ASM_FUNC("asm/code_081393B0/body_timer_tick.s", void body_timer_tick(void *ctx, struct Enemy *ent, EnemyFn *out))

void tick_body_timer(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    struct Enemy *body = ent->body;
    s32 t = body->unk1D2 - 1;

    body->unk1D2 = t;
    if ((u16)t == 0)
    {
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
        *out = body->unk1AC;
    }
}

ASM_FUNC("asm/code_081393B0/update_hop_phase.s", void update_hop_phase(struct Enemy *obj))

void obj_jump_arc_step(struct Enemy *obj)
{
    s32 flags;

    if (obj->jumpFrames != -1)
    {
        obj->unk18 += obj->jumpVz;
        obj->jumpVz -= obj->accel;
        obj->jumpFrames++;
        if (obj->unk18 <= 0)
        {
            obj->unk18 = 0;
            obj->jumpFrames = -1;
            ((struct Flags8 *)&obj->unk2B5)->bit3 = 0;
        }
    }
}

void stub_nop_0813A064(void)
{
}

void start_linked_state(struct Owner *owner, struct Enemy *obj)
{
    owner_entry(owner, ((struct HitBox *)((u8 *)obj + 0x100))->linkIndex);
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, 0);
    obj_set_cell_coord(obj, (((u32)obj->unk24 << 29) >> 30) + 4, 0);
    flags_obj_set_mode(obj, 0x10);
    obj->unk198 = obj_link_sprites;
}

void set_flags_1d6_copy(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
    ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
    out[0] = out[1];
}

void advance_handler_slot(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    if (ent->unk214 & 0x40)
    {
        obj_apply_state_a(ent);
    }
    out[0] = out[1];
}

ASM_FUNC("asm/code_081393B0/link_to_owner_entry.s", void link_to_owner_entry(struct Owner *owner, struct Enemy *obj, EnemyFn *out))

void move_step_handler(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    if ((u8)advance_move_step(ctx, ent) == 1)
    {
        obj_move_interp(ent);
    }
    else
    {
        ((struct Flags8 *)&ent->unk1D6)->bit1 = 0;
        ((struct Flags8 *)&ent->unk1D6)->bit0 = 1;
        out[0] = out[1];
    }
}

void obj_state_run_advance(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot = &ent->unk1A0[0];
    EnemyFn fn = slot[0];
    u8 flags;

    fn(ctx, ent, slot);
    flags = ent->unk1D6;
    if ((flags & 2) == 0 && (flags << 31) != 0)
    {
        ent->unk198 = ent->unk19C;
    }
}

void enter_run_state(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    load_record_fields(ent, 0);
    load_path_points(ent, 0);
    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_offset_target_state;
    ent->unk1A0[1] = setup_offset_target_state;
    fn(ctx, ent);
}

void setup_random_offset_state(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    u32 dir = obj_pick_dir_toward_owner(ctx, ent);

    obj_random_offset(ent, (u16)dir);
    obj_start_move_to_target(ent, 1, ent->vecX << 8, ent->vecY << 8);
    obj_step_toward_target_08138FDC(ent);
    *out = move_handler_timed_settle;
}

void enter_idle_hop_state(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot;
    EnemyFn fn;

    load_record_fields(ent, 0);
    load_hit_box(ent, 0);
    ent->unk198 = obj_state_run_hop_phase;
    slot = &ent->unk1A0[0];
    ent->unk1A0[0] = setup_random_offset_state;
    ent->unk1A0[1] = setup_random_offset_state;
    fn = slot[0];
    fn(ctx, ent, slot);
    if (ent->unk2F8 != 0)
    {
        update_hop_phase(ent);
    }
}

void restart_run_state(void *ctx, struct Enemy *ent)
{
    ((struct Flags8 *)&ent->unk2B6)->bit6 = 0;
    enter_run_state(ctx, ent);
}

void set_flag_enter_idle_hop(void *ctx, struct Enemy *ent)
{
    int flags = ent->unk2B6;

    flags |= 0x40;
    ent->unk2B6 = flags;
    enter_idle_hop_state(ctx, ent);
}

void setup_pick_next_state(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    struct Partner *entry = owner_entry(ctx, ((struct HitBox *)((u8 *)obj + 0x100))->linkIndex);

    obj_pick_move_vector(obj, entry);
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_try_start_hop(obj);
    obj_step_toward_target_08138FDC(obj);
    *out = obj_pick_next_state;
}

void setup_owner_offset_state(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    struct Partner *entry = owner_entry(ctx, ((struct HitBox *)((u8 *)ent + 0x100))->linkIndex);

    obj_pick_move_vector(ent, entry);
    obj_start_move_to_target(ent, 1, ent->vecX << 8, ent->vecY << 8);
    obj_step_toward_target_08138FDC(ent);
    *out = move_handler_timed_settle;
}

void update_facing_from_motion(void *ctx, struct Enemy *obj)
{
    s32 dx;
    s32 dy;
    u32 stage;

    obj_orbit_circle_step(obj);
    dx = obj->posX - obj->prevX;
    dy = obj->posY - obj->prevY;
    if ((dx | dy) != 0)
    {
        stage = ((s32)((u16)angle_of_vector16(dx, dy) + 0x1000) >> 13) & 7;
    }
    else
    {
        stage = 0xFFFF;
    }
    if (stage != (((u32)obj->unk24 << 29) >> 29))
    {
        obj_set_stage(obj, (u8)stage);
    }
    obj->vx = 1;
}

ASM_FUNC("asm/code_081393B0/move_handler_random_wait.s", void move_handler_random_wait(void *ctx, struct Enemy *obj, EnemyFn *out))

void start_random_move_wait(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    obj_random_facing_target(ent);
    obj_velocity_from_facing(ent);
    obj_try_start_hop(ent);
    obj_step_toward_target_08138FDC(ent);
    *out = move_handler_random_wait;
}

void obj_state_run_hop_landing(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot = &ent->unk1A0[0];
    EnemyFn fn = slot[0];

    fn(ctx, ent, slot);
    if (ent->unk2B5 & 8)
    {
        obj_hop_landing_step(ent);
    }
}

void obj_callback_apply_and_run(void *ctx, struct Enemy *ent, EnemyFn *slot)
{
    if (ent->unk214 & 0x40)
    {
        obj_apply_state_a(ent);
    }
    slot[1](ctx, ent, slot);
}

void setup_random_offset_settle(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    obj_random_offset(obj, 0xFF);
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_try_start_hop(obj);
    obj_step_toward_target_08138FDC(obj);
    obj_hop_landing_step(obj);
    *out = obj_pick_next_state;
}

ASM_FUNC("asm/code_081393B0/move_handler_timed_callback.s", void move_handler_timed_callback(void *ctx, struct Enemy *obj, EnemyFn *out))

void setup_offset_target_state(void *ctx, struct Enemy *ent, EnemyFn *out)
{
    obj_random_offset_step(ent);
    obj_start_move_quarter(ent, 0, ent->vecX << 8, ent->vecY << 8);
    obj_move_interp(ent);
    *out = move_handler_timed_callback;
}

ASM_FUNC("asm/code_081393B0/move_handler_timed_settle.s", void move_handler_timed_settle(void *ctx, struct Enemy *obj, EnemyFn *out))

void setup_random_offset_short(void *ctx, struct Enemy *obj, EnemyFn *out)
{
    obj_random_offset(obj, 0xFF);
    obj_start_move_to_target(obj, 1, obj->vecX << 8, obj->vecY << 8);
    obj_step_toward_target_08138FDC(obj);
    *out = move_handler_timed_settle;
}

void obj_state_run_hop_phase(void *ctx, struct Enemy *ent)
{
    EnemyFn *slot = &ent->unk1A0[0];
    EnemyFn fn = slot[0];

    fn(ctx, ent, slot);
    if (ent->unk2F8 != 0)
    {
        update_hop_phase(ent);
    }
}

void call_start_linked_state(void *ctx, struct Enemy *ent)
{
    start_linked_state(ctx, ent);
}

void enter_run_state_a311(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_pick_next_state;
    ent->unk1A0[1] = setup_pick_next_state;
    ent->unk1AC = obj_callback_apply_and_run;
    fn(ctx, ent);
}

void enter_state_a6cd(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = obj_state_run_hop_phase;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_owner_offset_state;
    ent->unk1A0[1] = setup_owner_offset_state;
    fn(ctx, ent);
}

void enter_state_a3b5(void *ctx, struct Enemy *ent)
{
    ent->unk198 = update_facing_from_motion;
    ent->unk198(ctx, ent);
}

void enter_state_hop_landing(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = obj_state_run_hop_landing;
    fn = ent->unk198;
    ent->unk1A0[0] = start_random_move_wait;
    ent->unk1A0[1] = start_random_move_wait;
    ent->unk1AC = start_random_move_wait;
    fn(ctx, ent);
}

void enter_run_state_a511(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_random_offset_settle;
    ent->unk1A0[1] = setup_random_offset_settle;
    ent->unk1AC = obj_callback_apply_and_run;
    fn(ctx, ent);
}

void enter_run_state_76b5(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = obj_random_pose;
    ent->unk1A0[1] = obj_random_pose;
    fn(ctx, ent);
}

void enter_run_state_a5d5(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = run_state_handler;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_offset_target_state;
    ent->unk1A0[1] = setup_offset_target_state;
    fn(ctx, ent);
}

void enter_idle_hop_a689(void *ctx, struct Enemy *ent)
{
    EnemyFn fn;

    ent->unk198 = obj_state_run_hop_phase;
    fn = ent->unk198;
    ent->unk1A0[0] = setup_random_offset_short;
    ent->unk1A0[1] = setup_random_offset_short;
    fn(ctx, ent);
}

ASM_FUNC("asm/code_081393B0/reset_record_fields.s", void reset_record_fields(struct Enemy *obj))

ASM_FUNC("asm/code_081393B0/obj_box_to_stack.s", void obj_box_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index))

ASM_FUNC("asm/code_081393B0/obj_box_mirror_to_stack.s", void obj_box_mirror_to_stack(s16 *out, struct Enemy *obj, u16 flags, u16 index))

ASM_FUNC("asm/code_081393B0/load_box_oriented.s", void load_box_oriented(s16 *out, struct Enemy *obj, u32 flags, u32 offset))
