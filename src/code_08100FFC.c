#include "global.h"

struct ChooseBit0
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct ChooseBit4
{
    u8 lo : 4;
    u8 b4 : 1;
    u8 rest : 3;
};

struct SysInput
{
    u8 filler0[0x3E];
    u16 keys3E;
};

#define KEY_R 0x100
#define KEY_L 0x200

/* sprite resources of the heroes' figures (MARIO_ANIM_BASE + 3, LUIGI_ANIM_BASE + 3) */
#define MARIO_FIG_SPRITE 0x2003
#define LUIGI_FIG_SPRITE 0x2050

struct FlagBit3
{
    u8 lo : 3;
    u8 b3 : 1;
    u8 rest : 4;
};

struct ChainBit5
{
    u8 lo : 5;
    u8 b5 : 1;
    u8 rest : 2;
};

struct WorldObj
{
    struct WorldObj *next;
    u8 filler4[0x08 - 0x04];
    u8 *sprite8;
    u8 filler0C[0x18 - 0x0C];
    s32 x18;
    u8 filler1C[0x28 - 0x1C];
    struct WorldObj *anim28;
    u8 filler2C[0x34 - 0x2C];
    struct HeroActor *target34;
    u8 filler38[0x4C - 0x38];
    u32 state4C;
    u8 filler50[0x54 - 0x50];
    u32 unk54;
    u8 filler58[0x76 - 0x58];
    u8 mode76;
    u8 flags77;
    u8 flags78;
    u8 flags79;
    u16 unk7A;
    u8 filler7C[0x7E - 0x7C];
    u8 ctrl7E;
    u8 flags7F;
    u32 handle80;
    u32 unk84;
    u32 unk88;
    u32 unk8C;
    u8 filler90[0x92 - 0x90];
    u16 unk92;
    u32 unk94;
    u32 unk98;
    u8 filler9C[0xAE - 0x9C];
    u16 unkAE;
    u16 unkB0;
    u16 unkB2;
    u32 unkB4;
    u8 fillerB8[0xD8 - 0xB8];
    s32 xD8;
    u8 fillerDC[0xEC - 0xDC];
    s16 animIndex;
    u8 filler0EE[0xF4 - 0xEE];
    u8 unkF4;
    u8 filler0F5[0xF6 - 0xF5];
    u16 hp;
    s16 maxHp;
    u8 filler0FA[0xFC - 0xFA];
    u16 unkFC;
    u8 filler0FE[0x110 - 0xFE];
    u8 flags110;
    u8 filler111[0x11E - 0x111];
    u16 bits11E;
    u8 filler120[0x122 - 0x120];
    u8 flags122;
    u8 filler123[0x12B - 0x123];
    u8 mode12B;
};

/* The bit for the object's mode (mode76 bits 1-2), for the masks of busy modes. */
static inline u32 obj_mode_bit(struct WorldObj *obj)
{
    return 1 << (((u32)obj->mode76 << 29) >> 30);
}

/* the actor part of a world object, at +8 */
struct HeroActor
{
    u8 filler0[0x08];
    u8 *pose;
    u8 filler0C[0x28 - 0x0C];
    struct WorldObj *owner28;
    struct HeroActor *target2C;
    u8 filler30[0x77 - 0x30];
    u8 flags77;
};

struct World
{
    u8 filler0[4];
    u8 unk4;
    u8 unk5;
    u8 filler6[2];
    u8 phase8;
    u8 unk9;
    u8 filler0A;
    u8 flags0B;
    u8 flags0C;
    u8 filler0D[0x58 - 0x0D];
    struct WorldObj *list58[8];
    struct WorldObj *chain;
    struct WorldObj *curObj;
    struct WorldObj *fig1;
    struct WorldObj *fig2;
    u8 filler88[0x1B8 - 0x88];
    u8 *table1B8;
    u8 filler1BC[0x244 - 0x1BC];
    void *unk244;
    u8 filler248[0x24C - 0x248];
    void *focus24C;
    void *slot4;
    u8 filler254[0x278 - 0x254];
    struct HeroActor *leader278;
    u8 filler27C[0x2BD - 0x27C];
    u8 mode2BD;
    u8 flags2BE;
    u8 level2BF;
    u8 figMode;
    u8 filler2C1[0x2C4 - 0x2C1];
    u8 slot2C4[2][3];
    u8 slot2CA[2][3];
    u8 slot2D0[2][3];
    s16 timer2D6;
    u8 filler2D8[0x310 - 0x2D8];
    void *status310;
    u8 filler314[0x31C - 0x314];
    u8 entry31C[0x33E - 0x31C];
    u8 entry33E;
    u8 heroSel33F;
    u8 heroSel340;
    u8 filler341;
    u8 flags342;
    u8 filler343[0x345 - 0x343];
    u8 entry345;
    u8 menuMode;
    u8 counter347;
    u8 heroSel;
    u8 order349[5];
    u8 order34E[2];
    u8 filler350[0x368 - 0x350];
    u8 *marker368;
};

struct FigInner
{
    u8 filler0[0x14];
    u16 unk14;
};

struct FigSlot
{
    u8 filler0[4];
    struct FigInner *inner;
};

struct StateOwner
{
    u8 filler0[4];
    u32 state;
    u8 filler8[0x10 - 0x08];
    u16 unk10;
    s16 speed;
    u8 filler14[0x16 - 0x14];
    u16 index;
    s16 kind;
};

struct PartyHero
{
    u8 filler0[0x0E];
    u16 hp;
    u8 filler10[0x14 - 0x10];
    u16 unk14; /* gets the battle object's unkFC */
    u8 filler16[0x3C - 0x16];
};

struct FlagBits56
{
    u8 lo5 : 5;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 rest : 1;
};

struct NibbleLo
{
    u8 lo4 : 4;
    u8 hi4 : 4;
};

struct WorldPart
{
    u8 filler0[0xFC];
    s16 fc;
};

extern struct World *gWorld;
extern u8 *gWorkBuf;

void sprite_set_action(struct HeroActor *actor, u32 action, u32 a2, u32 a3);

static inline void hero_actor_walk(struct HeroActor *actor)
{
    if (actor->pose[0x21] == 0)
        sprite_set_action(actor, 1, 0, 0);
    if (actor->pose[0x21] == 0xE)
        sprite_set_action(actor, 1, 0, 0);
}

static inline void hero_actor_stand(struct HeroActor *actor)
{
    if (actor->pose[0x21] == 1)
    {
        sprite_set_action(actor, 0, 0, 0);
        if (actor->pose[0x21] == 1)
            sprite_set_action(actor, 0xE, 0, 0);
    }
}
extern u8 *gParty;
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];

void state_apply_hero_stats(void *owner);
u32 process_object_chain(void);
u8 world_list_idle(void);
void set_mark_state(u32 a, u32 b);
void spawn_timer_child(void);
void world_objects_tick(u32 *owner);
void figure_pair_begin(void);
void state_hero_fx_dispatch(struct StateOwner *owner);
void state_effect_mode_2(void);
void state_effect_release(void);
void state_clear_hero_flags(void);
void state_choose_hero(struct StateOwner *owner);
void state_update_hero_markers(void);
void world_mode_step(struct StateOwner *owner);
void draw_obj_pose(struct WorldObj *obj);
void motion_step(struct WorldObj *obj);
u32 is_state_busy(void);
void set_cur_flags(u32 flags);
void state_check_slots(struct StateOwner *owner);
void state_queue_texts(void);
void clear_cur_flags(u32 bits);
u32 hero_select_index(void);
void state_wait_menu_ready(void);
void state_wait_free_slots(u32 *owner);
void set_cur_flags_pending(u32 flags);
u16 read_entry_half_1a0(void *obj, u8 idx);
void state_place_sprite_at_obj(void);
void load_table_entries(u32 code);
void draw_sprite_block(u32 a, u32 b, u32 c, u32 d, u32 e);
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void ensure_code_entry(u32 key);
void alloc_code_entry(u32 id);
void state_load_record_when_free(void);
extern const u16 gEffectCodeTable[];

u32 pick_target_by_input(struct WorldObj *obj, s32 mode);
void show_slot4_text_centered(u8 *text);
void place_at_active(u32 kind);
void obj_detach_active(void *obj);
void refresh_world_object(struct WorldObj *obj, u32 kind);
void sprite_entry_update(void *entry);
void hero_marker_sync(void);
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroActor *actor, u32 action, u32 a2, u32 a3);
void obj_release_node(void *node);
void state_status_release(void *status);
void release_marker_object(void);
void spawn_linked_children(void);
void sprite_refresh_display(void);
void hero_select_input(void);
s32 slot_marker_step(u32 cursor);
void set_world_mode_byte(u32 mode);
void set_world_effect(u32 effect);
void status_anim_wait(void);
s32 set_status_entry(u32 a, u8 *b, u32 c);
void setup_status_panel(void);
void state_stop_menu_obj(void);
void state_free_saved_list(void);
void hero_select_frame(void);
extern u8 **gHeroMenuTextA[];
extern u8 **gStatusTextTable[];
extern u8 gSys[];

struct SpriteFlags12
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hi : 3;
};

struct Bits3to5
{
    u8 lo : 3;
    u8 mid : 3;
    u8 hi : 2;
};

/* gWorld->flags2BE as bitfields */
struct WorldBits2BE
{
    u8 filler0[0x2BE];
    u8 lo : 3;
    u8 mid : 3;
    u8 hi : 2;
};

static inline void fig_sprite_action_0(struct WorldObj *fig, u32 action)
{
    sprite_set_action((struct HeroActor *)&fig->sprite8, 0, action, 0);
}

/*
 * Battle state: the acting bro picks the target of a command that acts on both
 * bros (gWorld->marker368 is the cursor sprite over the partner). The d-pad moves
 * the target; once one is picked, confirming stores the command for this bro and
 * starts it (or buzzes when neither bro could use it: both at full HP); the cancel
 * block or R/L goes back to the command blocks.
 */
void state_hero_menu_marker(struct StateOwner *owner)
{
    struct WorldObj *bro;
    struct HeroActor *target;
    struct HeroActor *broActor;
    struct WorldObj *bro1;
    struct WorldObj *bro2;
    s32 broIndex;
    s32 blockResult;
    u8 locked;
    s32 i;

    bro = gWorld->curObj;
    target = bro->target34;
    pick_target_by_input(bro, gWorld->mode2BD);
    if (target != bro->target34)
    {
        /* the target changed: update the help text, the cursor and the marker */
        target = bro->target34;
        if (target == NULL)
        {
            show_slot4_text_centered(gHeroMenuTextA[gSys[0x8C0]][0]);
            place_at_active(0xF);
            ((struct FlagBit3 *)&gWorld->curObj->flags7F)->b3 = 0;
            if (gWorld->marker368 != NULL)
                obj_detach_active(gWorld->marker368);
        }
        else
        {
            show_slot4_text_centered(gStatusTextTable[gSys[0x8C0]][2]);
            refresh_world_object(target->owner28, 0x11);
            ((struct FlagBit3 *)&gWorld->curObj->flags7F)->b3 = 1;
            if (gWorld->marker368 != NULL)
            {
                sprite_entry_update(gWorld->marker368);
                gWorld->marker368[0x23] = 0;
                /* draw the marker just above the leader's sprite */
                *(u16 *)(gWorld->marker368 + 0x2A) = *(u16 *)(gWorld->leader278->pose + 0x2A) + 1;
            }
        }
        hero_marker_sync();
        set_slot_value(0x5F, -1);
    }
    if (target == NULL)
        hero_actor_walk((struct HeroActor *)&bro->sprite8);
    else
        hero_actor_stand((struct HeroActor *)&bro->sprite8);
    broActor = (struct HeroActor *)&bro->sprite8;
    if ((gWorld->flags2BE & 0x40) && target != NULL)
    {
        /* confirmed: remember the chosen command for this bro */
        broIndex = ~gWorld->curObj->animIndex;
        gWorld->order34E[broIndex] = gWorld->order349[0];
        gWorld->slot2CA[broIndex][0] = gWorld->slot2C4[0][0];
        gWorld->slot2D0[broIndex][0] = gWorld->slot2C4[1][0];
        if (gWorld->marker368 != NULL)
        {
            obj_release_node(gWorld->marker368);
            gWorld->marker368 = NULL;
        }
        bro1 = gWorld->fig1;
        bro2 = gWorld->fig2;
        /* only useful if a bro still standing is below max HP */
        if (!(((bro1->ctrl7E & 6) == 2 && (s16)bro1->hp < bro1->maxHp)
            || (bro2 != NULL && (bro2->ctrl7E & 6) == 2 && (s16)bro2->hp < bro2->maxHp)))
        {
            set_slot_value(0x47, -1);
        }
        else
        {
            *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_stop_menu_obj;
            owner->state = (u32)state_free_saved_list;
            for (i = 0; i < 8; i++)
                ((struct FlagBits56 *)&gWorld->list58[i]->flags122)->b6 = 0;
            ((struct FlagBit3 *)&gWorld->figMode)->b3 = 1;
            state_status_release(gWorld->status310);
            release_marker_object();
            spawn_linked_children();
            spawn_timer_child();
            sprite_refresh_display();
        }
    }
    else
    {
        hero_select_input();
        blockResult = slot_marker_step(0);
        locked = gWorld->level2BF & 0x10;
        if (locked == 0 && (blockResult < 0 || (((struct SysInput *)gSys)->keys3E & (KEY_R | KEY_L))))
        {
            /* back out to the command blocks */
            if (gWorld->marker368 != NULL)
            {
                obj_release_node(gWorld->marker368);
                gWorld->marker368 = NULL;
            }
            hero_actor_stand(broActor);
            broActor->target2C = NULL;
            ((struct FlagBit3 *)&broActor->flags77)->b3 = 1;
            gWorld->entry345 |= 0x80;
            set_world_mode_byte(6);
            set_world_effect(2);
            status_anim_wait();
            set_status_entry(gWorld->entry33E, gWorld->entry31C, 0);
            *(u32 *)(gWorkBuf + 0x8E58) = (u32)hero_select_frame;
            owner->state = (u32)state_free_saved_list;
            if (gWorld->flags342 & 0x80)
                setup_status_panel();
            set_slot_value(0x61, -1);
        }
    }
}

/* Runs one frame of the world's object chain: draws the objects that have a pose, steps every object's motion and
 * runs its state handler (clearing its motion fields once the state ends). Returns the mask of the modes of the
 * objects drawn or still running a state, also kept in the top nibble of gWorld->unk9. */
u32 process_object_chain(void)
{
    struct World *world;
    struct WorldObj *obj;
    u32 modes;
    u32 shifted;

    modes = 0;
    obj = gWorld->chain;
    if (obj != NULL)
    {
        do
        {
            if (obj->unkB4 != 0)
            {
                draw_obj_pose(obj);
                modes |= obj_mode_bit(obj);
            }
            obj = obj->next;
        } while (obj != NULL);
    }
    obj = gWorld->chain;
    if (obj != NULL)
    {
        do
        {
            if (obj->state4C != 0)
            {
                if (!(obj->flags78 & 0x10))
                {
                    if (obj->anim28 != NULL && !(obj->flags79 & 0x10) && obj->handle80 != 0)
                    {
                        motion_step(obj);
                    }
                    ((void (*)(struct WorldObj *))obj->state4C)(obj);
                    if (obj->state4C == 0)
                    {
                        /* the state ended: stop its motion */
                        if (obj->anim28->animIndex < 0)
                        {
                            obj->sprite8[0x20] = 0x10;
                        }
                        ((struct ChainBit5 *)&obj->flags79)->b5 = 0;
                        *(u16 *)&obj->unk7A = 0;
                        *(u16 *)&obj->ctrl7E = 0;
                        obj->handle80 = 0;
                        obj->unk84 = 0;
                        obj->unk88 = 0;
                        obj->unk8C = 0;
                        obj->unkAE = 0;
                        obj->unkB0 = 0;
                        obj->unkB2 = 0;
                        obj->unk92 = 0;
                        obj->unk94 = 0;
                        obj->unk98 = 0;
                    }
                }
            }
            else if (obj->anim28 != NULL && !(obj->flags79 & 0x10) && obj->handle80 != 0)
            {
                motion_step(obj);
            }
            obj = obj->next;
        } while (obj != NULL);
    }
    obj = gWorld->chain;
    if (obj != NULL)
    {
        do
        {
            if (obj->state4C != 0)
            {
                modes |= obj_mode_bit(obj);
            }
            obj = obj->next;
        } while (obj != NULL);
    }
    world = gWorld;
    shifted = modes << 4;
    world->unk9 = (world->unk9 & 0xF) | shifted;
    return modes;
}

NONMATCH("asm/code_08100FFC/world_mode_step.s", void world_mode_step(struct StateOwner *owner))
{
    struct World *world;
    struct WorldObj *entry;
    u8 *obj;
    struct WorldObj *part;
    void (*method)(u8 *);
    s32 partFc;
    s32 delta;
    u8 *rec;
    u32 steps;
    u32 value;
    u32 phase;

    if ((u8)is_state_busy() != 0)
    {
        return;
    }
    if (gWorld->flags0B & 4)
    {
        return;
    }
    entry = gWorld->curObj;
    obj = (u8 *)entry + 8;
    method = (void (*)(u8 *))((u32 *)*(u32 **)(obj + 0x6C))[1];
    method(obj);
    ((struct FlagBits56 *)&entry->flags7F)->b6 = 0;
    ((struct FlagBits56 *)&entry->flags7F)->b5 = 0;
    world = gWorld;
    if (world->menuMode == 1)
    {
        steps = (u8)((gWorld->heroSel & 0x7F) / 3) + 1;
        ((struct NibbleLo *)&gWorld->phase8)->lo4 = steps | (steps << 2);
        part = *(struct WorldObj **)(obj + 0x28);
        partFc = ((struct WorldPart *)part)->fc;
        if ((gWorld->level2BF >> 6) <= 1)
        {
            rec = gWorld->table1B8 + gWorld->heroSel * 24;
            delta = rec[0x10];
        }
        else
        {
            rec = gWorld->table1B8 + gWorld->heroSel * 24;
            delta = rec[0x11];
        }
        ((struct WorldPart *)part)->fc = partFc - delta;
    }
    else
    {
        s32 lowMask = 0xF;
        s32 highMask = -16;
        value = ((world->counter347 & 0x7F) + 1) & lowMask;
        world->phase8 = (highMask & world->phase8) | value;
        world = gWorld;
        if (world->curObj->animIndex == -2)
        {
            phase = world->phase8;
            world->phase8 = (highMask & phase) | (((phase << 28) >> 26) & lowMask);
        }
    }
    owner->state = (u32)state_update_hero_markers;
    set_cur_flags(8);
}
END_NONMATCH


ASM_FUNC("asm/code_08100FFC/state_update_hero_markers.s", void state_update_hero_markers(void))

/* Battle turn state: waits (re-arming itself) until the world's object list is idle. Then sets the
   owner's speed: for an acting object in mode 4-6 it is timer2D6 / 10, at least 1. Clears timer2D6,
   makes state_choose_hero the state to come back to (gWorkBuf + 0x8E58), picks the effect index for
   the owner's kind (1: 0, 2: 8, else 0xE), loads that effect's table entries and goes on to
   state_place_sprite_at_obj. */
void state_hero_anim_begin(struct StateOwner *owner)
{
    struct World *world;
    struct WorldObj *curObj;
    s32 mode;
    s32 index;

    owner->state = (u32)state_hero_anim_begin;
    if (world_list_idle() == 0)
    {
        return;
    }
    world = gWorld;
    curObj = world->curObj;
    owner->speed = 0;
    /* modes 4, 5 and 6 take their speed from the timer (the compare tree of a switch) */
    mode = curObj->mode12B;
    if (mode == 5)
    {
        goto speed_from_timer;
    }
    if (mode > 5)
    {
        goto above_5;
    }
    if (mode == 4)
    {
        goto speed_from_timer;
    }
    goto speed_done;
above_5:
    if (mode != 6)
    {
        goto speed_done;
    }
speed_from_timer:
    owner->speed = world->timer2D6 / 10;
speed_done:
    if (owner->speed <= 0)
    {
        owner->speed = 1;
    }
    owner->kind = 1;
    gWorld->timer2D6 = 0;
    *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_choose_hero;
    if (owner->kind != 1)
    {
        index = 0xE;
        if (owner->kind == 2)
        {
            index = 8;
        }
    }
    else
    {
        index = 0;
    }
    owner->index = index;
    owner->state = (u32)state_place_sprite_at_obj;
    load_table_entries(gEffectCodeTable[owner->index]);
}

/*
 * Waits (re-arming itself) until the world's object list is idle, then goes on to
 * state_check_slots. When the acting object is no hero (animIndex >= 0) it clears cur flag 0x200
 * and queues 0x4000. Otherwise it clears cur flag 8 and queues 0x80; if world flags0B bit 0 is set
 * and both figures stand on the field (ctrl7E mode 2) with neither bits11E 0x300 set, it asks
 * hero_select_index for a pick: with one (1 or 2) it stores it in heroSel33F / heroSel340 and goes
 * on to state_queue_texts instead, without one it clears flags0B bit 0. Always clears flags0C bit 4.
 */
void state_choose_hero(struct StateOwner *owner)
{
    struct World *world;
    struct WorldObj *figA;
    struct WorldObj *figB;
    u32 pick;

    owner->state = (u32)state_choose_hero;
    if (world_list_idle() == 0)
    {
        return;
    }
    owner->state = (u32)state_check_slots;
    world = gWorld;
    if (world->curObj->animIndex >= 0)
    {
        clear_cur_flags(0x200);
        set_cur_flags_pending(0x4000);
    }
    else
    {
        if (world->flags0B & 1)
        {
            figA = world->fig1;
            figB = world->fig2;
            pick = 0;
            if ((figA->ctrl7E & 6) == 2 && (figB->ctrl7E & 6) == 2 && !(figA->bits11E & 0x300) && !(figB->bits11E & 0x300))
            {
                pick = (u16)hero_select_index();
            }
            if (pick != 0)
            {
                gWorld->heroSel33F = pick - 1;
                gWorld->heroSel340 = -pick + 2;
                owner->state = (u32)state_queue_texts;
            }
            else
            {
                world = gWorld;
                ((struct ChooseBit0 *)&world->flags0B)->b0 = 0;
            }
            clear_cur_flags(8);
            set_cur_flags_pending(0x80);
        }
        else
        {
            clear_cur_flags(8);
            set_cur_flags_pending(0x80);
        }
    }
    world = gWorld;
    ((struct ChooseBit4 *)&world->flags0C)->b4 = 0;
}

/*
 * Battle turn state: waits while the acting object's script entry (unk244's entry curObj->unkF4)
 * still has its 0x1A0 half set. If either side's count (world unk4 / unk5) has run out, it goes on to
 * state_wait_free_slots; otherwise to state_wait_menu_ready, queueing cur flag 0x100000 when none of
 * the 8 battle objects is on the field (ctrl7E mode 2 or 4) without flags110 bit 7, and always 0x4000000.
 */
void state_check_slots(struct StateOwner *owner)
{
    struct World *world;
    struct WorldObj *obj;
    s32 i;
    u32 mode;

    if ((u16)read_entry_half_1a0(gWorld->unk244, gWorld->curObj->unkF4) != 0)
    {
        return;
    }
    world = gWorld;
    if (world->unk4 == 0 || world->unk5 == 0)
    {
        owner->state = (u32)state_wait_free_slots;
        return;
    }
    owner->state = (u32)state_wait_menu_ready;
    for (i = 0; i < 8; i++)
    {
        obj = world->list58[i];
        mode = 6 & obj->ctrl7E;
        if (mode != 6 && mode != 0 && !(obj->flags110 & 0x80))
        {
            break;
        }
    }
    if (i == 8)
    {
        /* nobody can act */
        set_cur_flags_pending(0x100000);
    }
    set_cur_flags_pending(0x4000000);
}


/*
 * Battle-end state (state_check_slots goes here once world unk4 or unk5 has run out): waits until
 * the world's object list is idle and flags0B bit 2 is clear. With a joint move pending (flags0C
 * bit 7), unk5 at 0 and neither bro under status bits 8-9 (bits11E 0x300), it starts the bros'
 * joint move first (figure_pair_begin), coming back here after it. Otherwise it clears both hero
 * markers in menu mode 2, spawns the timer child, ticks the world's objects and goes on to
 * state_hero_fx_dispatch. owner[1] is the owner's state.
 */
void state_wait_free_slots(u32 *owner)
{
    struct World *world;
    u16 *statusA;
    u16 *statusB;

    if (world_list_idle() == 0)
    {
        return;
    }
    world = gWorld;
    if (world->flags0B & 4)
    {
        return;
    }
    if ((world->flags0C & 0x80) && world->unk5 == 0)
    {
        statusA = &world->fig1->bits11E;
        statusB = &world->fig2->bits11E;
        if (!((*statusA | *statusB) & 0x300))
        {
            *(u32 *)(gWorkBuf + 0x8E58) = (u32)state_wait_free_slots;
            owner[1] = (u32)figure_pair_begin;
            return;
        }
    }
    if (gWorld->menuMode == 2)
    {
        set_mark_state(0, 0);
        set_mark_state(1, 0);
    }
    spawn_timer_child();
    world_objects_tick(owner);
    owner[1] = (u32)state_hero_fx_dispatch;
}

/* Copies each bro's battle HP (at least 1) and unkFC back into the party data (gParty, 0x3C bytes
 * per bro; the battle objects of bro 0 and 1 are list58[6] and [7]). Then goes on, by the world's
 * bytes 4 and 5: state_effect_mode_2 when byte 4 is 0, state_effect_release when only byte 5 is 0,
 * else state_clear_hero_flags. */
void state_hero_fx_dispatch(struct StateOwner *owner)
{
    s32 broIndex;
    struct PartyHero *hero;
    u32 byte4;
    void (*next)(struct StateOwner *);

    broIndex = ~gWorld->fig1->animIndex;
    hero = (struct PartyHero *)(gParty + broIndex * 60);
    hero->hp = gWorld->list58[broIndex + 6]->hp;
    hero->unk14 = gWorld->list58[broIndex + 6]->unkFC;
    if (hero->hp == 0)
    {
        hero->hp = 1;
    }
    if (gWorld->fig2 != NULL)
    {
        broIndex = ~gWorld->fig2->animIndex;
        hero = (struct PartyHero *)(gParty + broIndex * 60);
        hero->hp = gWorld->list58[broIndex + 6]->hp;
        hero->unk14 = gWorld->list58[broIndex + 6]->unkFC;
        if (hero->hp == 0)
        {
            hero->hp = 1;
        }
    }
    byte4 = gWorld->unk4;
    if (byte4 == 0 || gWorld->unk5 == 0)
    {
        next = (void (*)(struct StateOwner *))state_effect_mode_2;
        if (byte4 != 0)
        {
            next = (void (*)(struct StateOwner *))state_effect_release;
        }
    }
    else
    {
        next = (void (*)(struct StateOwner *))state_clear_hero_flags;
    }
    next(owner);
}


ASM_FUNC("asm/code_08100FFC/state_play_result_effects.s", void state_play_result_effects(void))

void state_fig_flags_reset(void *owner)
{
    u32 modes = process_object_chain();

    if (!(modes & 2))
    {
        gWorld->heroSel = 0xFF;
    }
    if (modes != 0)
    {
        struct WorldObj *fig = gWorld->fig1;
        if (fig != NULL && fig->unk54 == 0)
        {
            fig->bits11E &= 0xF3FF;
        }
        fig = gWorld->fig2;
        if (fig != NULL && fig->unk54 == 0)
        {
            fig->bits11E &= 0xF3FF;
        }
    }
    else
    {
        state_apply_hero_stats(owner);
    }
}

/*
 * Sets up the status display: draws the leading hero's block (fig2's when figMode bit 2 is set,
 * a blank one when figMode bits 0-1 are clear), queues the panel's tiles for VRAM at 0x06008000,
 * preloads object 0x7002 and the figures' sprites, then waits 12 frames in
 * state_load_record_when_free.
 */
void state_setup_status_display(struct StateOwner *owner)
{
    struct World *world;
    struct FigSlot **leader;
    register u32 figMode asm("r0");
    u32 shown;
    u32 secondOffset;
    u32 glyphIndex;
    u32 *tileOffset;
    u8 *tiles;
    u32 marioSprite;
    u32 luigiSprite;
    u32 extraSprite;

    world = gWorld;
    figMode = world->figMode;
    shown = figMode & 3;
    if (shown != 0)
    {
        secondOffset = figMode & 4;
        leader = (struct FigSlot **)&world->fig1;
        leader = (struct FigSlot **)((u8 *)leader + secondOffset);
        draw_sprite_block(4, 0, 0x20, 0xFF, (*leader)->inner->unk14);
        glyphIndex = gGlyphIndexTable[0];
    }
    else
    {
        draw_sprite_block(4, 0, 0x20, 0xFF, shown);
        glyphIndex = gGlyphIndexTable[0];
    }
    tileOffset = gMapTable + glyphIndex;
    tiles = (u8 *)gMapTable + *tileOffset;
    queue_display_entry(2, tiles, (void *)0x06008000, 0xE40, 0, 0);
    ensure_code_entry(0x7002);
    marioSprite = MARIO_FIG_SPRITE;
    ensure_code_entry(marioSprite);
    luigiSprite = LUIGI_FIG_SPRITE;
    ensure_code_entry(luigiSprite);
    extraSprite = 0x401F;
    ensure_code_entry(extraSprite);
    alloc_code_entry(marioSprite);
    alloc_code_entry(luigiSprite);
    alloc_code_entry(extraSprite);
    owner->unk10 = 12;
    owner->state = (u32)state_load_record_when_free;
}

s32 obj_last_x(void *obj);
void copy_map_rect_add(void *src, u32 a, u32 b, u32 c, u32 d, u32 e, void *f, u32 g);
void set_tile_slot(void *slot, u32 idx, u32 flag, u32 value, u32 p5, u32 p6, u32 p7);
void fill_slot(void *slot, u32 idx, u32 value);
void state_draw_rows(void);
void state_heroes_walk_to_end(struct StateOwner *owner)
{
    struct World *world;
    struct WorldObj *fig;
    s32 walking;
    s32 i;
    s32 n;
    s16 anim;

    walking = 0;
    for (i = 0; i < 2; i++)
    {
        world = gWorld;
        fig = (&world->fig1)[i];
        if (fig != NULL && (fig->ctrl7E & 6) == 2)
        {
            fig->x18 += -0x300;
            if (fig->x18 < ((s16)obj_last_x(world->focus24C) << 8) + 0x2000)
                fig->xD8 = fig->x18 = ((s16)obj_last_x(gWorld->focus24C) << 8) + 0x2000;
            else
                walking = 1;
        }
    }
    if (walking)
        return;

    if (gWorld->fig2 != NULL)
    {
        if ((gWorld->fig1->ctrl7E & 6) == 2)
            copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6,
                (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x9000);
        if ((gWorld->fig2->ctrl7E & 6) == 2)
            copy_map_rect_add(gWorld->slot4, 1, 7, 0xB, 7, 6,
                (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable) + 0x180, 0x9000);
    }
    else
    {
        anim = gWorld->fig1->animIndex;
        if (anim == -1)
            copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6,
                (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x9000);
        else if (anim == -2)
            copy_map_rect_add(gWorld->slot4, 1, 7, 5, 7, 6,
                (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2) + (u32)gMapTable) + (u32)gMapTable) + 0x180, 0x9000);
    }

    n = 3;
    if (((struct WorldBits2BE *)gWorld)->mid != 0)
        n = (((struct WorldBits2BE *)gWorld)->mid * 11 + 1) / 8 + 2;
    copy_map_rect_add(gWorld->slot4, 1, 0xE, 5, 0x10, n - 1,
        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0xE, 0x8000);
    copy_map_rect_add(gWorld->slot4, 1, 0xE, n + 4, 0x10, 1,
        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x18E, 0x8000);
    copy_map_rect_add(gWorld->slot4, 1, 0xE, n + 5, 0x10, 1,
        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x1CE, 0x8000);
    copy_map_rect_add(gWorld->slot4, 1, 0xE, 0xD, 7, 4,
        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x20E, 0x8000);
    copy_map_rect_add(gWorld->slot4, 1, 0x15, 0xD, 7, 4,
        ({ u32 gi = (u32)gGlyphIndexTable[gSys[0x8C0] == 2 ? 0x21 : gSys[0x8C0] == 3 ? 0x22 : gSys[0x8C0] == 4 ? 0x23 : 0x20] << 2; register u32 m asm("r1") = (u32)gMapTable; (u8 *)(m + *(u32 *)(gi + m)); }) + 0x21C, 0x9000);
    set_tile_slot(gWorld->slot4, 2, 0, 0xF, 6, 0xF, 0xB);
    fill_slot(gWorld->slot4, 2, 0x90);

    fig = gWorld->fig1;
    if (fig != NULL && (fig->ctrl7E & 6) == 2)
    {
        if (fig->animIndex == -1)
            fig_sprite_action_0(fig, 0x2003);
        else
            fig_sprite_action_0(fig, 0x2050);
        ((struct SpriteFlags12 *)(((struct HeroActor *)&fig->sprite8)->pose + 0x12))->bit4 = 0;
        ((struct SpriteFlags12 *)(((struct HeroActor *)&fig->sprite8)->pose + 0x12))->mode = 0;
    }
    fig = gWorld->fig2;
    if (fig != NULL && (fig->ctrl7E & 6) == 2)
    {
        if (fig->animIndex == -1)
            fig_sprite_action_0(fig, 0x2003);
        else
            fig_sprite_action_0(fig, 0x2050);
        ((struct SpriteFlags12 *)(((struct HeroActor *)&fig->sprite8)->pose + 0x12))->bit4 = 0;
        ((struct SpriteFlags12 *)(((struct HeroActor *)&fig->sprite8)->pose + 0x12))->mode = 0;
    }
    owner->unk10 = 0;
    owner->state = (u32)state_draw_rows;
}
