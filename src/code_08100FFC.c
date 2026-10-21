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
    u8 filler0C[0x28 - 0x0C];
    struct WorldObj *anim28;
    u8 filler2C[0x4C - 0x2C];
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
    u8 filler[0xEC - 0xB8];
    s16 animIndex;
    u8 filler0EE[0xF4 - 0xEE];
    u8 unkF4;
    u8 filler0F5[0xF6 - 0xF5];
    u16 unkF6;
    u8 filler0F8[0xFC - 0xF8];
    u16 unkFC;
    u8 filler0FE[0x110 - 0xFE];
    u8 flags110;
    u8 filler111[0x11E - 0x111];
    u16 bits11E;
    u8 filler120[0x12B - 0x120];
    u8 mode12B;
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
    u8 filler248[0x2BF - 0x248];
    u8 level2BF;
    u8 figMode;
    u8 filler2C1[0x2D6 - 0x2C1];
    s16 timer2D6;
    u8 filler2D8[0x33F - 0x2D8];
    u8 heroSel33F;
    u8 heroSel340;
    u8 filler341[0x346 - 0x341];
    u8 menuMode;
    u8 counter347;
    u8 heroSel;
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
    u16 unkE;
    u8 filler10[0x14 - 0x10];
    u16 unk14;
    u8 filler16[0x3C - 0x16];
};

extern struct World *gWorld;
extern u8 *gWorkBuf;
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
void world_mode_step(void);
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

ASM_FUNC("asm/code_08100FFC/state_hero_menu_marker.s", void state_hero_menu_marker(void))

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
                modes |= 1 << (((u32)obj->mode76 << 29) >> 30);
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
                modes |= 1 << (((u32)obj->mode76 << 29) >> 30);
            }
            obj = obj->next;
        } while (obj != NULL);
    }
    world = gWorld;
    shifted = modes << 4;
    world->unk9 = (world->unk9 & 0xF) | shifted;
    return modes;
}

ASM_FUNC("asm/code_08100FFC/world_mode_step.s", void world_mode_step(void))


ASM_FUNC("asm/code_08100FFC/state_update_hero_markers.s", void state_update_hero_markers(void))

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
    mode = curObj->mode12B;
    if (mode == 5)
    {
        goto speed_timer;
    }
    if (mode > 5)
    {
        goto high_mode;
    }
    if (mode == 4)
    {
        goto speed_timer;
    }
    goto speed_done;
high_mode:
    if (mode != 6)
    {
        goto speed_done;
    }
speed_timer:
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

void state_choose_hero(struct StateOwner *owner)
{
    struct World *world;
    struct WorldObj *fig1;
    struct WorldObj *fig2;
    u32 hero;

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
            fig1 = world->fig1;
            fig2 = world->fig2;
            hero = 0;
            if ((fig1->ctrl7E & 6) == 2 && (fig2->ctrl7E & 6) == 2 && !(fig1->bits11E & 0x300) && !(fig2->bits11E & 0x300))
            {
                hero = (u16)hero_select_index();
            }
            if (hero != 0)
            {
                gWorld->heroSel33F = hero - 1;
                gWorld->heroSel340 = -hero + 2;
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

void state_check_slots(struct StateOwner *owner)
{
    struct World *world;
    struct WorldObj *obj;
    s32 count;
    u32 masked;

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
    for (count = 0; count < 8; count++)
    {
        obj = world->list58[count];
        masked = 6 & obj->ctrl7E;
        if (masked != 6 && masked != 0 && !(obj->flags110 & 0x80))
        {
            break;
        }
    }
    if (count == 8)
    {
        set_cur_flags_pending(0x100000);
    }
    set_cur_flags_pending(0x4000000);
}


NONMATCH("asm/code_08100FFC/state_wait_free_slots.s", void state_wait_free_slots(u32 *owner))
{
    struct World *world;
    u16 *bits1;
    u16 *bits2;
    u32 bits;

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
        bits1 = &world->fig1->bits11E;
        bits2 = &world->fig2->bits11E;
        bits = *bits1;
        bits |= *bits2;
        if (!(bits & 0x300))
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
END_NONMATCH

NONMATCH("asm/code_08100FFC/state_hero_fx_dispatch.s", void state_hero_fx_dispatch(struct StateOwner *owner))
{
    s32 slot;
    struct PartyHero *hero;
    u16 value;
    u16 value2;
    u32 byte4;
    void (*fx)(struct StateOwner *);

    slot = ~gWorld->fig1->animIndex;
    hero = (struct PartyHero *)(gParty + slot * 60);
    value = gWorld->list58[slot + 6]->unkF6;
    hero->unkE = value;
    hero->unk14 = gWorld->list58[slot + 6]->unkFC;
    if (value == 0)
    {
        hero->unkE = 1;
    }
    if (gWorld->fig2 != NULL)
    {
        slot = ~gWorld->fig2->animIndex;
        hero = (struct PartyHero *)(gParty + slot * 60);
        value2 = gWorld->list58[slot + 6]->unkF6;
        hero->unkE = value2;
        hero->unk14 = gWorld->list58[slot + 6]->unkFC;
        if (value2 == 0)
        {
            hero->unkE = 1;
        }
    }
    byte4 = gWorld->unk4;
    if (byte4 == 0 || gWorld->unk5 == 0)
    {
        fx = (void (*)(struct StateOwner *))state_effect_mode_2;
        if (byte4 != 0)
        {
            fx = (void (*)(struct StateOwner *))state_effect_release;
        }
    }
    else
    {
        fx = (void (*)(struct StateOwner *))state_clear_hero_flags;
    }
    fx(owner);
}
END_NONMATCH


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

NONMATCH("asm/code_08100FFC/state_setup_status_display.s", void state_setup_status_display(struct StateOwner *owner))
{
    struct World *world;
    struct FigSlot *fig_slot;
    u32 fig_kind;
    u32 value;
    u32 *entry;
    u8 *text;
    u32 id_a;
    u32 id_b;
    u32 id_c;

    world = gWorld;
    fig_kind = world->figMode & 3;
    if (fig_kind != 0)
    {
        fig_slot = *(struct FigSlot **)((u8 *)world + 0x80 + (world->figMode & 4));
        value = fig_slot->inner->unk14;
    }
    else
    {
        value = fig_kind;
    }
    draw_sprite_block(4, 0, 0x20, 0xFF, value);
    entry = gMapTable + gGlyphIndexTable[0];
    text = (u8 *)gMapTable + *entry;
    queue_display_entry(2, text, (void *)0x06008000, 0xE40, 0, 0);
    ensure_code_entry(0x7002);
    id_a = 0x2003;
    ensure_code_entry(id_a);
    id_b = 0x2050;
    ensure_code_entry(id_b);
    id_c = 0x401F;
    ensure_code_entry(id_c);
    alloc_code_entry(id_a);
    alloc_code_entry(id_b);
    alloc_code_entry(id_c);
    owner->unk10 = 0xC;
    owner->state = (u32)state_load_record_when_free;
}
END_NONMATCH

ASM_FUNC("asm/code_08100FFC/state_heroes_walk_to_end.s", void state_heroes_walk_to_end(void))
