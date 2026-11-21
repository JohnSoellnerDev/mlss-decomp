#include "global.h"
#include "ram_routines.h"

struct FigWorld
{
    u8 filler0[0xB];
    u8 unkB;
    u8 flags0C;
    u8 filler0D[0x40 - 0xD];
    u32 unk40;
    u8 filler44[0x58 - 0x44];
    u8 *list[8];
    u8 filler78[0x7C - 0x78];
    u8 *cur;
    u8 *fig[2];
    u8 filler88[0x2C0 - 0x88];
    u8 flags2C0;
};

struct FigPart
{
    u8 filler0[8];
    u8 *unk8;
    u8 filler0C[4];
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u8 filler1C[0x28 - 0x1C];
    u8 *unk28;
};

struct FigBits
{
    u8 mode : 2;
    u8 bit2 : 1;
} __attribute__((packed));

struct StateOwner
{
    u8 filler0[4];
    u32 callback;
    u8 filler8[0x12 - 8];
    u16 unk12;
    u16 unk14;
    u8 filler16[0x18 - 0x16];
    s16 unk18;
};

struct ObjCounter121
{
    u8 filler0[0x121];
    u8 lo4 : 4;
    u8 counter : 3;
    u8 hi : 1;
};

struct ObjFlags122
{
    u8 filler0[0x122];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

// A battler's status bits at 0x11E and the 3-bit countdowns after them (named by their bit in the
// word at 0x120) that clear them, one step per turn (state_update_bits).
struct ObjTimers
{
    u8 filler0[0x10A];
    u16 unk10A;         // a multiplier (0x100 = 1.0) that status bits 2-3 change
    u16 unk10C;         // the same for status bits 4-5
    u8 filler10E[0x11C - 0x10E];
    u16 unk11C;         // the same for status bits 6-7
    u16 status;         // bits 0-1: can't act (state_wait_menu wobbles it), 2-3, 4-5, 6-7: see above,
                        // 8-9: timed by timer12 (state_start); with them set the turn goes to
                        // state_select_target, 12-15: cleared by timer18
    u8 timer0 : 3;      // status bit 0
    u8 timer3 : 3;      // status bits 2-3
    u16 timer6 : 3;     // status bits 4-5
    u8 timer9 : 3;      // status bits 6-7
    u8 timer12 : 3;     // status bits 8-9
    u32 timer15 : 3;    // an animation mode, ended by anim_end_mode
    u8 timer18 : 3;     // status bits 12-15
    u8 bit21 : 1;
    u8 hi22 : 2;
};

struct ObjMode7E
{
    u8 filler0[0x7E];
    u8 lo : 1;
    u8 mode : 2;
    u8 hi : 5;
};

struct FigFlags2C0
{
    u8 filler0[0x2C0];
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct ObjFlags11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi : 1;
};

// HeroData.slot (Battler.slot) of the brothers
#define SLOT_HERO_A -1
#define SLOT_HERO_B -2

// First sprite image of each bro's battle set (sprite_set_action's arg); hero B's is 0x2F further on.
#define IMAGES_HERO_A 0x2039
#define IMAGES_HERO_B 0x2068

// One of the bros in battle (World.fig[]). Its sprite body (a FigPart, the object the slot_*
// functions move) starts at +8; positions are 24.8 fixed point.
struct Battler
{
    u8 filler0[8];
    struct FigPart body;
    u8 filler34[0x40 - 0x34];
    s32 posX;           // body + 0x38
    s32 posY;
    u8 filler48[0x7E - 0x48];
    u8 lo7E : 1;
    u8 mode : 2;        // 1: standing on the field (struct ObjMode7E)
    u8 hi7E : 5;
    u8 filler7F[0x8C - 0x7F];
    s32 destX;          // where slot_set_vec sends it
    u8 filler90[0x9C - 0x90];
    s32 speed;
    u8 fillerA0[0xD8 - 0xA0];
    s32 homeX;          // its place in the battle line
    u8 fillerDC[0xEC - 0xDC];
    s16 slot;           // SLOT_HERO_A or SLOT_HERO_B
    u8 fillerEE[0xF4 - 0xEE];
    u8 slotIndex;       // its index for the slot_* functions
    u8 fillerF5;
    u16 hp;
    s16 maxHp;
    u8 fillerFA[0xFC - 0xFA];
    u16 bp;
    s16 maxBp;
    u8 filler100[0x11E - 0x100];
    u16 status;         // see struct ObjTimers
};

// A Battler's sprite body (Battler.body) with the position fields that follow it.
struct BattlerBody
{
    struct FigPart part;
    u8 filler2C[0x38 - 0x2C];
    s32 x;              // Battler.posX
    s32 y;              // Battler.posY
};

struct Owner;

struct ObjMode12
{
    u8 filler0[0x12];
    u8 lo : 1;
    u8 mode : 2;
    u8 hi : 5;
};

struct FigObj
{
    u8 filler0[4];
    u32 unk4;
    u32 unk8;
    u8 filler0C[0x10 - 0xC];
    u16 unk10;
};

extern struct FigWorld *gWorld;
extern u8 *gWorkBuf;
extern u8 gSys[];
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];
extern u8 gUnk_02000440[];
u8 step_motion(struct FigPart *part);
void sprite_set_action(struct FigPart *part, u32 action, u32 arg, u32 extra);
u8 is_state_busy(void);
void set_slot_value(u32 slot, u32 value);
void state_fig_active_call(void);
void state_update_bits(struct StateOwner *owner);
void state_obj_wait_then_select(struct Owner *owner);
void sprite_load_active_tables(void);
void hero_random_offset(u8 *obj);
void figure_pair_step(struct FigObj *obj);
void anim_bit100_release(u8 *obj);
void place_and_start_action5(u8 *obj, u32 a, u32 b);
void state_call_saved_if_flag(void);
void state_call_active_callback(void);
void figure_pair_place(struct StateOwner *owner);
void draw_obj_pose(u8 *obj);
void release_slot_objects(void);
void place_targets(u8 *obj, u32 a, u32 b, u32 c, u32 d);
void set_step_and_offsets(u8 *obj, u32 a, u32 b);
void state_show_figs(struct StateOwner *owner);
void state_wait_menu(struct StateOwner *owner);
void state_ready_check(struct StateOwner *owner);
void figure_pair_begin(struct StateOwner *owner);
void state_select_by_status(struct StateOwner *owner);
s32 signed_byte_e4(void *part);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void state_restore_saved_when_flag(void);
void refresh_sprite_state(void);
void noop_stub_080826AC(void);
void world_objects_tick(void);
void state_menu_open_active(void);
void state_select_targets(void);
void create_status_objects(void);
void hero_select_begin(void);
void figure_status_enter(struct StateOwner *owner);
void slot_set_vec(u32 idx, s32 x, s32 y, s32 z, u32 flag);
void slot_set_timer(u32 idx, u32 value);
void slot_start_motion(u32 idx, u32 flag);

/* Starts the active bro's turn: the turn goes on to state_ready_check when owner->unk18 is set, else to
   state_wait_menu. Status bits 8-9 (0x100, 0x200) count down in timer12; when it runs out, bit 8 is
   lifted by anim_bit100_release, or for bit 9 the bro is put back on the ground with action 5 and the
   bits are cleared. That turn first runs the follow-up (state_call_saved_if_flag /
   state_call_active_callback) and keeps the chosen state as the saved one (gWorkBuf + 0x8E58). */
void state_start(struct StateOwner *owner)
{
    u8 *battler;
    u16 *status;
    u32 next;
    s32 kind;

    battler = gWorld->cur;
    sprite_load_active_tables();
    kind = owner->unk18;
    next = (u32)state_wait_menu;
    if (kind != 0)
        next = (u32)state_ready_check;
    owner->callback = next;
    ((struct ObjFlags122 *)battler)->bit5 = 0;
    status = &((struct ObjTimers *)battler)->status;
    if (*status & 0x100)
    {
        u32 left = ((struct ObjTimers *)battler)->timer12 - 1;

        ((struct ObjTimers *)battler)->timer12 = left;
        if ((left & 7) == 0)
        {
            anim_bit100_release(battler);
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)state_call_saved_if_flag;
        }
    }
    else if (*status & 0x200)
    {
        u32 left = ((struct ObjTimers *)battler)->timer12 - 1;

        ((struct ObjTimers *)battler)->timer12 = left;
        if ((left & 7) == 0)
        {
            place_and_start_action5(battler + 8, 0x10, 0x500);
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)state_call_active_callback;
            *status &= 0xFCFF;
        }
    }
}

void state_ready_check(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *fig1;
    u8 *fig2;
    void (*fn)(struct StateOwner *);

    world = gWorld;
    if (*(s16 *)(world->cur + 0xEC) < 0 && (world->flags0C & 0x80))
    {
        fig1 = world->fig[0] + 0x11E;
        fig2 = world->fig[1] + 0x11E;
        if (((*(u16 *)fig1 | *(u16 *)fig2) & 0x100) == 0)
        {
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)figure_pair_begin;
            return;
        }
    }
    owner->unk12 = 2;
    owner->unk14 = 2;
    fn = state_select_by_status;
    owner->callback = (u32)fn;
    *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_wait_menu;
    fn(owner);
}

/* Heals the active bro after its effect (state_place_sprite_at_obj): owner->unk18 bit 0 adds owner->unk12 HP,
 * bit 1 adds owner->unk14 BP, each shown as popup digits and capped at the maximum. */
void state_move(struct StateOwner *owner)
{
    struct FigWorld *world;
    struct Battler *bro;
    struct BattlerBody *body;
    s32 kind;

    world = gWorld;
    if (world->unk40 != 0)
        return;
    bro = (struct Battler *)world->cur;
    body = (struct BattlerBody *)&bro->body;
    kind = owner->unk18;
    switch (kind)
    {
    case 1:
        popup_digit_spawn((s16)owner->unk12, body->x / 0x100, body->y / 0x100, signed_byte_e4(bro), kind);
        break;
    case 2:
        popup_digit_spawn((s16)owner->unk14, body->x / 0x100, body->y / 0x100, signed_byte_e4(bro), 1);
        break;
    case 3:
        popup_digit_spawn((s16)owner->unk12, body->x / 0x100 + 0xC, body->y / 0x100 + 4, signed_byte_e4(bro), 1);
        popup_digit_spawn((s16)owner->unk14, body->x / 0x100 - 0xC, body->y / 0x100 - 4, signed_byte_e4(bro), 1);
        break;
    }
    if (owner->unk18 & 1)
    {
        bro->hp += owner->unk12;
        if ((s16)bro->hp > bro->maxHp)
            bro->hp = bro->maxHp;
    }
    if (owner->unk18 & 2)
    {
        bro->bp += owner->unk14;
        if ((s16)bro->bp > bro->maxBp)
            bro->bp = bro->maxBp;
    }
    owner->callback = (u32)state_restore_saved_when_flag;
}

/* Battle state before the active battler picks an action. A bro's turn (slot < 0) with a joint move
   pending (flags0C bit 7) and neither bro under status bit 8 starts the joint move
   (figure_pair_begin, keeping this state as the saved one). Otherwise a battler that can't act
   (status bits 0-1) and has more than this turn of it left (timer0 != 1) wobbles in place and the
   turn goes to state_obj_wait_then_select; else on to state_update_bits. */
void state_wait_menu(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *battler;
    u16 *statusA;
    u16 *statusB;

    world = gWorld;
    battler = world->cur;
    if (((struct Battler *)battler)->slot < 0 && (world->flags0C & 0x80))
    {
        statusA = &((struct Battler *)world->fig[0])->status;
        statusB = &((struct Battler *)world->fig[1])->status;
        if (((*statusA | *statusB) & 0x100) == 0)
        {
            *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
            owner->callback = (u32)figure_pair_begin;
            return;
        }
    }
    sprite_load_active_tables();
    /* status bits 0-1, and the low byte of the countdown word after it (timer0) */
    if ((((struct Battler *)battler)->status & 3) != 0 && (*(u8 *)(battler + 0x120) & 7) != 1)
    {
        battler += 8;
        hero_random_offset(battler);
        draw_obj_pose(battler);
        owner->callback = (u32)state_obj_wait_then_select;
        return;
    }
    owner->callback = (u32)state_update_bits;
}

void anim_stop_mode(u8 *obj);
void anim_end_mode(u8 *obj);
void state_select_target(void);
void state_camera(struct StateOwner *owner);
void state_message(struct StateOwner *owner);
/* Battle state at the start of the current battler's turn: picks the next state (state_camera, or
   state_message when flags2C0 is 1, flags0C bit 7 is clear and neither hero has status bits 8-9),
   then counts down the battler's status timers. A stat timer that runs out clears its status bits
   and resets its multiplier to 0x100; when the bit-0 status runs out the animation stops and,
   unless bits 8-9 are set, the chosen state runs after state_call_saved_if_flag. With status bits
   8-9 set the turn goes to state_select_target. */
void state_update_bits(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *battler;

    world = gWorld;
    battler = world->cur;
    if ((world->flags2C0 & 3) != 1 || (world->flags0C & 0x80) || ((*(u16 *)(world->fig[0] + 0x11E) | *(u16 *)(world->fig[1] + 0x11E)) & 0x300))
        owner->callback = (u32)state_camera;
    else
        owner->callback = (u32)state_message;

    if (((struct ObjTimers *)battler)->timer18)
    {
        u32 left = ((struct ObjTimers *)battler)->timer18 - 1;

        ((struct ObjTimers *)battler)->timer18 = left;
        if ((left & 7) == 0)
            ((struct ObjTimers *)battler)->status &= 0xFFF;
    }
    if (((struct ObjTimers *)battler)->status & 0xC)
    {
        u32 left = ((struct ObjTimers *)battler)->timer3 - 1;

        ((struct ObjTimers *)battler)->timer3 = left;
        if ((left & 7) == 0)
        {
            ((struct ObjTimers *)battler)->status &= 0xFFF3;
            ((struct ObjTimers *)battler)->unk10A = 0x100;
        }
    }
    if (((struct ObjTimers *)battler)->status & 0x30)
    {
        u32 left = ((struct ObjTimers *)battler)->timer6 - 1;

        ((struct ObjTimers *)battler)->timer6 = left;
        if ((left & 7) == 0)
        {
            ((struct ObjTimers *)battler)->status &= 0xFFCF;
            ((struct ObjTimers *)battler)->unk10C = 0x100;
        }
    }
    if (((struct ObjTimers *)battler)->status & 0xC0)
    {
        u32 left = ((struct ObjTimers *)battler)->timer9 - 1;

        ((struct ObjTimers *)battler)->timer9 = left;
        if ((left & 7) == 0)
        {
            ((struct ObjTimers *)battler)->status &= 0xFF3F;
            ((struct ObjTimers *)battler)->unk11C = 0x100;
        }
    }
    if (((struct ObjTimers *)battler)->status & 1)
    {
        u32 left = ((struct ObjTimers *)battler)->timer0 - 1;

        ((struct ObjTimers *)battler)->timer0 = left;
        if ((left & 7) == 0)
        {
            if ((((struct ObjTimers *)battler)->status & 0x300) == 0)
            {
                // Run the chosen state after state_call_saved_if_flag.
                *(u32 *)(gWorkBuf + 0x8E58) = owner->callback;
                owner->callback = (u32)state_call_saved_if_flag;
                ((struct ObjFlags122 *)battler)->bit5 = 0;
            }
            else
            {
                ((struct ObjFlags122 *)battler)->bit5 = 1;
            }
            anim_stop_mode(battler);
        }
    }
    if (*(u32 *)(battler + 0x120) & 0x38000) // timer15
    {
        u32 left = ((struct ObjTimers *)battler)->timer15 - 1;

        ((struct ObjTimers *)battler)->timer15 = left;
        if ((left & 7) == 0)
        {
            ((struct ObjTimers *)battler)->timer15 = 1;
            anim_end_mode(battler);
        }
    }
    if (((struct ObjTimers *)battler)->status & 0x300)
        owner->callback = (u32)state_select_target;
}

/*
 * Battle state after the turn's status update (state_update_bits); waits while a state is busy.
 * When the current battler is in mode 2 and has something pending at +0xE8, the brothers standing
 * on the field get their +0x74 cleared, flags2C0 bit 4 is cleared, bit 6 of +0x122 is cleared on
 * all 8 battlers in the list, flags2C0 bit 3 is set and the turn goes to state_menu_open_active
 * (also kept as the saved state); with nothing pending it goes to world_objects_tick. In any other
 * mode the sprites are refreshed and state_select_targets runs right away.
 */
void state_camera(struct StateOwner *owner)
{
    u8 *battler;
    u8 *bro1;
    u8 *bro2;
    u32 busy;
    s32 mask;
    s32 off;
    void (*next)(struct StateOwner *);

    battler = gWorld->cur;
    busy = is_state_busy();
    if (busy != 0)
        return;
    if (((struct ObjMode7E *)battler)->mode == 2)
    {
        if (*(u32 *)(battler + 0xE8) != 0)
        {
            bro1 = gWorld->fig[0];
            bro2 = gWorld->fig[1];
            if (bro1 != 0 && ((struct ObjMode7E *)bro1)->mode == 1)
                *(u32 *)(bro1 + 0x74) = busy; /* 0 */
            if (bro2 != 0 && ((struct ObjMode7E *)bro2)->mode == 1)
                *(u32 *)(bro2 + 0x74) = 0;
            ((struct FigFlags2C0 *)gWorld)->bit4 = 0;
            noop_stub_080826AC();
            /* the saved state, run by state_call_saved_if_flag and the like */
            *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_menu_open_active;
            owner->callback = (u32)state_menu_open_active;
            off = 0x122;
            mask = ~0x40;
            *(gWorld->list[0] + off) &= mask;
            *(gWorld->list[1] + off) &= mask;
            *(gWorld->list[2] + off) &= mask;
            *(gWorld->list[3] + off) &= mask;
            *(gWorld->list[4] + off) &= mask;
            *(gWorld->list[5] + off) &= mask;
            *(gWorld->list[6] + off) &= mask;
            *(gWorld->list[7] + off) &= mask;
            ((struct FigFlags2C0 *)gWorld)->bit3 = 1;
        }
        else
        {
            owner->callback = (u32)world_objects_tick;
        }
    }
    else
    {
        refresh_sprite_state();
        next = (void (*)(struct StateOwner *))state_select_targets;
        owner->callback = (u32)next;
        next(owner);
    }
}

void state_message(struct StateOwner *owner)
{
    struct FigWorld *world;
    struct FigPart *part;
    u32 text;
    u32 slot;

    if (is_state_busy())
        return;
    if (gWorld->unkB & 4)
        return;
    slot = (gWorld->flags2C0 & 4) ? 3 : 0x15;
    set_slot_value(slot, -1);
    world = gWorld;
    part = (struct FigPart *)(world->fig[1 ^ ((struct FigBits *)&world->flags2C0)->bit2] + 8);
    text = (world->flags2C0 & 4) ? 0x2039 : 0x2068;
    sprite_set_action(part, 2, text, 0);
    ((struct FigBits *)&gWorld->flags2C0)->mode = 2;
    owner->callback = (u32)state_fig_active_call;
}

/* Unless gWorld->unkB bit 2 is set: frees the slot objects, then the bro who isn't taking the turn
   (hero A when flags2C0 bit 2 says hero B's turn, else hero B) plays action 3 with its images and
   jumps to its screen spot (0x28, 0x7C) or (0x36, 0x5C) with step 0x400 and offsets 0x20. The state
   goes on to state_show_figs (run once right away), which waits for it to land; sound 0x2B plays
   (set_slot_value). */
void state_setup_menu(struct StateOwner *owner)
{
    struct FigWorld *world;
    u8 *body;
    u32 dest_x;
    u32 dest_y;
    u32 images;
    void (*next)(struct StateOwner *);

    if (gWorld->unkB & 4)
        return;
    release_slot_objects();
    world = gWorld;
    if (world->flags2C0 & 4)
    {
        body = world->fig[0] + 8;
        dest_x = 0x28;
        dest_y = 0x7C;
        images = IMAGES_HERO_A;
    }
    else
    {
        body = world->fig[1] + 8;
        dest_x = 0x36;
        dest_y = 0x5C;
        images = IMAGES_HERO_B;
    }
    sprite_set_action((struct FigPart *)body, 3, images, 0);
    place_targets(body, dest_x, dest_y, 0, 0x100);
    set_step_and_offsets(body, 0x400, 0x20);
    next = state_show_figs;
    owner->callback = (u32)next;
    next(owner);
    set_slot_value(0x2B, -1);
}


/* Waits for the other bro (fig[1 ^ flags2C0 bit 2]) to finish its animation. Meanwhile, while its
   sprite asks for it (+0x23), it is drawn one layer over the active bro (Battler +0x7D). Once the
   animation has ended (sprite +0x12 bit 3): flags0C bit 7 is set, both bros start their next
   animation (the second bro's set is 0x2F further on), the other bro's position becomes its home
   (its data's +0xD8 x/y/h) and the state goes back to the saved one (unk8). */
void figure_sync_step(struct FigObj *obj)
{
    struct FigWorld *world;
    struct FigPart *part_other;
    struct FigPart *part_active;
    u8 *fig_other;
    u8 *fig_active;
    s32 second_bro;
    u32 anim;
    u32 order;
    u32 home_x;
    u32 home_y;
    u32 home_h;

    world = gWorld;
    fig_other = world->fig[1 ^ ((struct FigBits *)&world->flags2C0)->bit2];
    part_other = (struct FigPart *)(fig_other + 8);
    fig_active = world->fig[((struct FigBits *)&world->flags2C0)->bit2];
    part_active = (struct FigPart *)(fig_active + 8);
    if (part_other->unk8[0x23] != 0)
    {
        order = fig_active[0x7D] + 1;
        fig_other[0x7D] = order;
    }
    if (part_other->unk8[0x12] & 8)
    {
        {
            s32 flags = gWorld->flags0C;
            flags |= 0x80;
            gWorld->flags0C = flags;
        }
        second_bro = gWorld->flags2C0 & 4;
        anim = 0x2069;
        if (second_bro)
            anim -= 0x2F;
        sprite_set_action(part_other, 0, anim, 0);
        second_bro = gWorld->flags2C0 & 4;
        anim = 0x203B;
        if (second_bro)
            anim += 0x2F;
        sprite_set_action(part_active, 0, anim, 0);
        home_x = (u32)part_other->unk28 + 0xD8;
        *(u32 *)home_x = part_other->unk10;
        home_y = home_x + 4;
        *(u32 *)home_y = part_other->unk14;
        home_h = home_y + 4;
        *(u32 *)home_h = part_other->unk18;
        obj->unk4 = obj->unk8;
    }
}

/* Starts the bros' joint move: clears flags0C bit 7, plays mode 5 on the other bro and mode 10 on the
   active one, each with its own image (hero A's images start at 0x2039, hero B's 0x2F further on),
   sets both sprites' +0x12 mode to 1, moves the active bro 22 px back (x is 24.8 fixed point) and
   goes on to figure_pair_place. */
void figure_pair_begin(struct StateOwner *owner)
{
    struct FigWorld *world;
    struct FigPart *part_other;
    struct FigPart *part_active;
    u8 *sprite_other;
    u8 *sprite_active;
    u32 image;
    s32 hero_b_active;

    world = gWorld;
    part_other = (struct FigPart *)(world->fig[1 ^ ((struct FigBits *)&world->flags2C0)->bit2] + 8);
    part_active = (struct FigPart *)(world->fig[((struct FigBits *)&world->flags2C0)->bit2] + 8);
    world->flags0C &= 0x7F;
    hero_b_active = gWorld->flags2C0 & 4;
    image = 0x2068;
    if (hero_b_active)
        image -= 0x2F;
    sprite_set_action(part_other, 5, image, 0);
    hero_b_active = gWorld->flags2C0 & 4;
    image = 0x2039;
    if (hero_b_active)
        image += 0x2F;
    sprite_set_action(part_active, 0xA, image, 0);
    sprite_other = part_other->unk8;
    sprite_active = part_active->unk8;
    ((struct ObjMode12 *)sprite_active)->mode = 1;
    ((struct ObjMode12 *)sprite_other)->mode = 1;
    part_active->unk10 -= 0x1600;
    owner->callback = (u32)figure_pair_place;
}

/* Once the other bro's animation has ended (sprite +0x12 bit 3): the other bro plays mode 6 and the
   active one mode 1, the other bro jumps to screen spot (0x28, 0x7C), or (0x36, 0x5C) when hero B is
   the active one, with step 0x400 and offsets 0x20, the state goes on to figure_pair_step (run once
   right away) and sound 0x2B plays. */
void figure_pair_place(struct StateOwner *owner)
{
    struct FigWorld *world;
    struct FigPart *part_other;
    struct FigPart *part_active;
    u32 dest_x;
    u32 dest_y;
    u8 hero_b_active;
    void (*next)(struct StateOwner *);

    world = gWorld;
    part_other = (struct FigPart *)(world->fig[1 ^ ((struct FigBits *)&world->flags2C0)->bit2] + 8);
    if (part_other->unk8[0x12] & 8)
    {
        part_active = (struct FigPart *)(world->fig[((struct FigBits *)&world->flags2C0)->bit2] + 8);
        sprite_set_action(part_other, 6, 0, 0);
        sprite_set_action(part_active, 1, 0, 0);
        hero_b_active = gWorld->flags2C0 & 4;
        dest_x = 0x28;
        if (hero_b_active != 0)
            dest_x = 0x36;
        dest_y = 0x7C;
        if (hero_b_active != 0)
            dest_y = 0x5C;
        place_targets((u8 *)part_other, dest_x, dest_y, 0, 0x100);
        set_step_and_offsets((u8 *)part_other, 0x400, 0x20);
        next = (void (*)(struct StateOwner *))figure_pair_step;
        owner->callback = (u32)next;
        next(owner);
        set_slot_value(0x2B, -1); /* plays sound 0x2B */
    }
}

void figure_pair_step(struct FigObj *obj)
{
    struct FigWorld *world;
    struct FigPart *part;
    u32 addr;
    u32 addr2;
    u32 addr3;
    u32 sel;
    u32 bit;

    world = gWorld;
    bit = ((u32)world->flags2C0 << 29) >> 31;
    sel = 1;
    sel ^= bit;
    part = (struct FigPart *)(world->fig[sel] + 8);
    if (!step_motion(part))
    {
        sprite_set_action(part, 0, -1, 0);
        addr = (u32)part->unk28 + 0xD8;
        *(u32 *)addr = part->unk10;
        addr2 = addr + 4;
        *(u32 *)addr2 = part->unk14;
        addr3 = addr2 + 4;
        *(u32 *)addr3 = part->unk18;
        ((struct FigBits *)&gWorld->flags2C0)->mode = 2;
        obj->unk4 = *(u32 *)(gWorkBuf + 0x8E58);
        obj->unk10 = 0xA;
    }
}

ASM_FUNC("asm/code_080FD8D8/state_select_targets.s", void state_select_targets(void))

/* Starts moving each bro that is on the field and free (status bits 8-9 clear) to a spot beside
   its home X over 4 frames, with its move animation and facing the way it goes: in front of home
   when it's the current battler, behind it otherwise (alone, hero A goes a little in front). Then
   the state goes on to figure_status_enter. */
void figure_vectors_setup(struct StateOwner *owner)
{
    struct FigWorld *world;
    struct Battler *bro;
    s32 i;
    u32 held;
    u32 slot;
    s32 x;
    s32 y;

    for (i = 0; i < 2; i++)
    {
        world = gWorld;
        bro = (struct Battler *)world->fig[i];
        if (bro == 0)
            continue;
        if (bro->mode != 1)
            continue;
        held = bro->status & 0x300;
        if (held != 0)
            continue;
        bro->speed = held; // 0
        if (i == 1)
        {
            if (world->cur == (u8 *)bro)
            {
                slot = bro->slotIndex;
                x = bro->homeX / 0x100 + 0x12;
                y = bro->posY / 0x100;
                slot_set_vec(slot, x, y, 0, i);
            }
            else
            {
                slot = bro->slotIndex;
                x = bro->homeX / 0x100 - 0x10;
                y = bro->posY / 0x100;
                slot_set_vec(slot, x, y, 0, i);
            }
        }
        else if (world->fig[1] != 0)
        {
            if (world->cur == (u8 *)bro)
            {
                slot = bro->slotIndex;
                x = bro->homeX / 0x100 + 4;
            }
            else
            {
                slot = bro->slotIndex;
                x = bro->homeX / 0x100 - 0x1E;
            }
            y = bro->posY / 0x100;
            slot_set_vec(slot, x, y, 0, 1);
        }
        else
        {
            slot = bro->slotIndex;
            x = bro->homeX / 0x100 + 0xB;
            y = bro->posY / 0x100;
            slot_set_vec(slot, x, y, 0, 1);
        }
        slot_set_timer(bro->slotIndex, 4);
        slot_start_motion(bro->slotIndex, 0);
        if (bro->slot == SLOT_HERO_A)
            sprite_set_action(&bro->body, 0, 0x2002, 0);
        else
            sprite_set_action(&bro->body, 0, 0x204F, 0);
        // Moving left: flip the sprite.
        if (bro->destX < bro->posX)
            ((struct ObjFlags11 *)bro->body.unk8)->bit6 = 1;
    }
    owner->callback = (u32)figure_status_enter;
}

/* Waits until neither bro's body is busy (flag 0x10 at +0x81), then sets each bro on the field
   to its idle action (action 1 for the current battler, -1 for the other) facing right again,
   loads OBJ palette 14 (entry 0xF, or 0x10 in link mode) into the palette shadow, creates the
   status objects and goes on to hero_select_begin. */
void figure_status_enter(struct StateOwner *owner)
{
    struct FigWorld *world;
    struct Battler *bro;
    u32 i;
    u32 paletteIndex;
    void (*copy)(u8 *, u8 *, u32);

    if (gWorld->fig[0] != 0 && (gWorld->fig[0][0x81] & 0x10) != 0)
        return;
    if (gWorld->fig[1] != 0 && (gWorld->fig[1][0x81] & 0x10) != 0)
        return;
    for (i = 0; i < 2; i++)
    {
        world = gWorld;
        bro = (struct Battler *)world->fig[i];
        if (bro != 0 && bro->mode == 1)
        {
            if ((u8 *)bro == world->cur)
                sprite_set_action(&bro->body, 1, -1, 0);
            else
                sprite_set_action(&bro->body, -1, -1, 0);
            // Face right: clear the sprite's flip bit.
            ((struct ObjFlags11 *)bro->body.unk8)->bit6 = 0;
        }
    }
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    paletteIndex = gGlyphIndexTable[(gSys[0x888] & 6) ? 0x10 : 0xF];
    copy((u8 *)gMapTable + *(gMapTable + paletteIndex), gUnk_02000440, 0x20);
    // The palette shadow changed.
    ((struct { u16 unk0; } *)gSys)->unk0 |= 0x4000;
    create_status_objects();
    owner->callback = (u32)hero_select_begin;
}
ASM_FUNC("asm/code_080FD8D8/hero_select_begin.s", void hero_select_begin(void))
