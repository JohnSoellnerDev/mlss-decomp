#include "global.h"

struct Sprite
{
    s16 x;
    s16 y;
    u8 filler4[0x12 - 0x4];
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
};

struct Obj
{
    u8 filler0[0x10];
    struct Sprite *link;
};

struct ModeBlock
{
    u8 filler0[0xA];
    u8 mode;
};

struct Owner
{
    u32 unk0;
    void (*state)(struct Owner *);
    struct Sprite *sprite;
    struct Obj *target;
    s16 timer;
    s16 posX;
    s16 posY;
    s16 index;
    s16 scrollStep;
    u8 filler1A[0x6C - 0x1A];
    struct ModeBlock *block;
};

struct Effect
{
    u8 filler0[5];
    u8 flags5;
};

struct Scene
{
    u8 filler0[0x40];
    u16 scroll;
};

struct World
{
    struct Scene *scene;
    u8 filler4[0xB - 0x4];
    u8 flags0B;
    u8 filler0C[0x4C - 0xC];
    void *listHead;
    u8 filler50[0x58 - 0x50];
    struct Obj *list58[8];
    u8 filler78[0x7C - 0x78];
    u8 *curObj;
    struct Obj *fig80[2];
    u8 filler88[0x244 - 0x88];
    u32 unk244;
    struct Menu *menu248;
    void *ptr24C;
    void *mapSrc250;
    u8 filler254[0x278 - 0x254];
    struct Owner *obj278;
    u32 unk27C;
    u32 unk280;
    u8 filler284[0x2BF - 0x284];
    u8 flags2BF;
    u8 figMode2C0;
    u8 filler2C1[0x2D8 - 0x2C1];
    struct Effect *oldEffect;
    u8 filler2DC[0x2F9 - 0x2DC];
    u8 mode2F9;
    u8 filler2FA[0x310 - 0x2FA];
    void *task310;
    u8 filler314[0x33F - 0x314];
    u8 hero33F;
    u8 unk340;
    u8 filler341;
    u8 pose342;
    u8 filler343[0x346 - 0x343];
    u8 menuMode346;
    u8 filler347[0x349 - 0x347];
    u8 unk349;
    u8 filler34A[0x364 - 0x34A];
    void *savedList;
    u8 filler368[0x374 - 0x368];
    void *listOut374;
    u8 filler378[4];
    void *scrollObj37C;
    u8 filler380[0x3A5 - 0x380];
    u8 mode3A5;
};struct SavedList
{
    u8 filler0[4];
    void *unk4;
    void *unk8;
};

struct Link2
{
    u8 filler0[0x14];
    u16 unk14;
};

struct Cand
{
    u8 filler0[0x10];
    struct Link2 *link;
};

struct Menu
{
    u8 filler0[0x44];
    u16 unk44;
};

struct Pos
{
    u8 filler0[0x18];
    s32 x;
    s32 y;
    s32 z;
};

struct Scroll
{
    u8 filler0[0x10];
    s32 pos10;
};

struct Callback
{
    u8 filler0[0x4C];
    void (*fn)(void *);
};

struct Entry
{
    u8 filler0[0xC];
    void *unkC;
    u8 filler10[0x12 - 0x10];
    u16 unk12;
};

struct Chance
{
    u8 filler0[0x28];
    u16 unk28;
    u16 unk2A;
};

struct Work
{
    u8 filler0[0x8E58];
    void (*savedState)(struct Owner *);
};

extern struct World *gUnk_03000FD8;
extern struct Work *gUnk_03000FDC;
extern const u16 gEffectCodeTable[];

void update_slot_markers(void);
void display_pause(void);
void draw_digits_49_p11(s32 a, s32 b, s32 c, s32 d);
void state_hero_select_confirm(struct Owner *owner);
void state_menu_scroll_stop(struct Owner *owner);
void state_hero_anim_then_notify(struct Owner *owner);
void state_obj_release_when_free(struct Owner *owner);
void state_place_sprite_at_obj(struct Owner *owner);
void state_wait_result_then_close(struct Owner *owner);
void state_fig_flags_reset(struct Owner *owner);
void state_update_hero_markers(struct Owner *owner);
void state_marker_text_place(struct Owner *owner);
void state_apply_hero_stats(struct Owner *owner);
void obj_detach_active(struct Sprite *sprite);
void set_cur_flags_pending(u32 flags);
void set_cur_flags_both(u32 flags);
void sprite_load_tables(u32 index);
void load_table_entries(u32 value);
u8 is_state_busy(void);
void candidate_dispatch(void);
s32 sub_081DE850(s32 n, u32 value);
u32 mt19937_next(void);
void symbol_add(u32 key, u32 count);
void state_wait_free_then_2199(struct Owner *owner);
void draw_fixed_labels(void);
void release_marker_object(void);
void obj_release(void *obj);
void offset_three_words(void *a, s32 *x, s32 *y, s32 *z, u32 w);
void state_init_map_tiles(struct Owner *owner);
void state_hero_entry_timeline(struct Owner *owner);
void copy_map_rect_add(void *src, u32 a, u32 b, u32 c, u32 d, u32 e, void *f, u32 g);
void state_marker_show(struct Owner *owner);
void state_scroll_clamp_then_finish(struct Owner *owner);
void state_clear_menu_flag_and_end(struct Owner *owner);
void state_add_points_to_hero(struct Owner *owner);
void state_update_bits(struct Owner *owner);
void state_choose_hero(struct Owner *owner);
void state_move(struct Owner *owner);
void state_menu_build(struct Owner *owner);
void state_marker_gfx_place(struct Owner *owner);
void figure_sync_step(struct Owner *owner);
u8 step_motion(void *obj);
void effect_spawn_at(u32 kind, s32 x, s32 y, s32 z, void *sprite);
void ensure_code_entry(u32 id);
void alloc_code_entry(u32 id);
struct Owner *task_create_list_4c(void (*state)(struct Owner *), u32 arg);
extern void (*gMemRoutines)(u32 a, void *b, u32 c);
extern u8 gUnk_02000080;
extern u8 gUnk_02000280;
extern u16 gUnk_0300034C[];
extern u16 gUnk_0200000C;
extern u16 gUnk_0200001C;
extern u16 gUnk_0200001E;
extern const u16 gGlyphIndexTable[];
extern const u32 gUnk_089F9A5C[];
void *spawn_obj_by_id(s32 id, u32 a, u32 b, u32 c);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void state_heroes_walk_to_end(struct Owner *owner);
s32 sprite_node_release(void);
void obj_release_node(void *node);
u8 world_list_idle(void);
void set_mark_state(u32 a, u32 b);
void world_objects_tick(struct Owner *owner);
void reset_entry_part(u32 a, u32 b, u32 c);
void pair_begin(void *obj);
void change_item_count(u32 a, s32 b);
void set_cur_flags(u32 flags);
s32 number_right_edge(s32 a, s32 b);
void draw_digits_c0_p11(s32 a, s32 b, s32 c, s32 d);
u8 sub_08117390(void *menu);
struct Cand *active_candidate(void *target);
void draw_sprite_block(u32 a, u32 b, u32 c, u32 d, u32 e);
void state_wait_menu_ready(struct Owner *owner);
void state_cursor_anim_step(struct Owner *owner);
void state_wait_then_clear(struct Owner *owner);
void state_camera(struct Owner *owner);
void state_select_pose(struct Owner *owner);
void state_menu_slide_in(struct Owner *owner);
void state_play_result_effects(struct Owner *owner);
void state_menu_scroll_begin(struct Owner *owner);
void sprite_set_action(struct Owner *obj, u32 action, u32 a2, u32 a3);
void set_mark_callbacks(void);
void release_slot_objects(void);
void set_slot_target(u32 slot, u32 target, s32 value);
void set_slot_value(u32 slot, s32 value);
u32 draw_digit_cycle(struct Owner *obj, s32 n);
struct Entry *list_add_task(void *list, void (*state)(struct Owner *));
void state_figure_jump_start(struct Owner *owner);
void state_heroes_anim_stop(struct Owner *owner);
void state_draw_value_step(struct Owner *owner);
void world_mode_step(struct Owner *owner);
void noop_stub_080826AC(void);
void run_sequence_of_obj_b(void *obj);

void state_menu_scroll_wait(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;
    u16 *scrollp = &world->scene->scroll;
    s32 scroll = *scrollp - 1;
    s32 step = owner->scrollStep;

    if (step < 0)
    {
        step += 0xFF;
    }
    step >>= 8;
    *scrollp = scroll - step;
    owner->scrollStep += 0x19;
    if (owner->timer != 0)
    {
        owner->timer--;
    }
    else
    {
        set_slot_value((world->figMode2C0 & 4) ? 0xFA : 0xE8, -1);
        owner->timer = 0x28;
        owner->state = state_menu_scroll_begin;
    }
}

void menu_init_clear(void)
{
    gUnk_03000FD8->menu248->unk44 = 0;
    gMemRoutines(0, &gUnk_02000080, 0x200);
    gMemRoutines(0, &gUnk_02000280, 0x200);
    gUnk_0300034C[0] = 0xFFFF;
    gUnk_0300034C[1] = 0xFFFF;
    task_create_list_4c(state_clear_menu_flag_and_end, 0)->timer = 0x20;
}

void state_clear_menu_flag_and_end(struct Owner *owner)
{
    u16 timer = owner->timer;
    s16 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct World *world = gUnk_03000FD8;
        s32 flags = world->flags2BF;
        s32 mask = ~0x20;

        mask &= flags;
        world->flags2BF = mask;
        owner->state = (void (*)(struct Owner *))(s32)signedTimer;
    }
}

ASM_FUNC("asm/code_08107268/state_menu_scroll_stop.s", void state_menu_scroll_stop(struct Owner *owner))

void state_menu_scroll_begin(struct Owner *owner)
{
    s32 timer = owner->timer;

    if (timer != 0)
    {
        gUnk_03000FD8->scene->scroll++;
        owner->timer--;
    }
    else
    {
        owner->scrollStep = timer;
        owner->state = state_menu_scroll_stop;
    }
}

ASM_FUNC("asm/code_08107268/state_menu_clear_vram.s", void state_menu_clear_vram(void))

void state_wait_menu_clear(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct World *world = gUnk_03000FD8;

        if ((world->unk27C | world->unk280) == 0)
        {
            display_pause();
            owner->timer = 0x20;
            owner->state = state_menu_slide_in;
        }
    }
}

ASM_FUNC("asm/code_08107268/menu_open_setup.s", void menu_open_setup(void))

void state_hero_sprite_check(struct Owner *owner)
{
    struct Obj *hero = gUnk_03000FD8->list58[gUnk_03000FD8->hero33F + 6];

    if (hero->link->flags & 8)
    {
        owner->timer = 8;
        owner->state = state_heroes_anim_stop;
        owner->state(owner);
    }
}

void state_hero_anim_then_notify(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        sprite_set_action((struct Owner *)((u8 *)gUnk_03000FD8->list58[gUnk_03000FD8->hero33F + 6] + 8), 2, 0, 0);
        owner->timer = 0x20;
        owner->state = state_marker_gfx_place;
        owner->state(owner);
        set_slot_value(gUnk_03000FD8->hero33F ? 0x103 : 0xF3, -1);
    }
}

void state_marker_reset_then_anim(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        obj_detach_active(owner->sprite);
        owner->timer = 0x20;
        owner->state = state_hero_anim_then_notify;
        owner->state(owner);
    }
}

void state_marker_reset_then_text(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        obj_detach_active(owner->sprite);
        owner->timer = 0x10;
        owner->state = state_marker_text_place;
        owner->state(owner);
    }
}

void state_queue_texts(struct Owner *owner)
{
    s32 id;

    ensure_code_entry(0x2038);
    ensure_code_entry(0x2067);
    id = gUnk_03000FD8->unk340 ? 0x204E : 0x2001;
    ensure_code_entry(id);
    alloc_code_entry(0x2038);
    alloc_code_entry(0x2067);
    alloc_code_entry(gUnk_03000FD8->unk340 ? 0x204E : 0x2001);
    owner->timer = 4;
    owner->state = state_marker_show;
    owner->state(owner);
}

void add_random_symbol_200(void)
{
    s32 roll = sub_081DE850(100, mt19937_next());
    s32 kind;

    kind = roll >= 0;
    if (roll > 0x5B)
    {
        kind++;
    }
    if (roll > 0x61)
    {
        kind++;
    }
    symbol_add(0x200, kind);
}

void chance_add_symbol(struct Chance *obj, s32 a, s32 b)
{
    u32 value;

    if (obj == 0)
    {
        return;
    }
    if (b == 0)
    {
        u32 bits = ((u8 *)obj)[0x2A];
        s32 chance = ((bits << 27) >> 27) + a;

        if (chance <= sub_081DE850(0x1F, mt19937_next()))
        {
            return;
        }
        value = obj->unk2A;
    }
    else
    {
        value = obj->unk28;
    }
    value >>= 5;
    if (value != 0)
    {
        symbol_add(value, 1);
    }
}

void copy_obj_mode_to_world(struct Owner *owner)
{
    gUnk_03000FD8->mode2F9 = owner->block->mode;
    update_slot_markers();
}

void set_world_mode_byte(u8 value)
{
    s32 flags;

    gUnk_03000FD8->mode3A5 = value;
    flags = gUnk_03000FD8->flags0B;
    gUnk_03000FD8->flags0B = flags | 0x40;
}

ASM_FUNC("asm/code_08107268/state_sprite_park.s", void state_sprite_park(void))

void state_wait_obj_active(struct Owner *owner)
{
    if (owner->target->link->flags & 8)
    {
        owner->state = state_hero_select_confirm;
    }
}

void state_pose_draw_free(struct Owner *owner)
{
    s32 y = (gUnk_03000FD8->pose342 << 4) + 0x28;
    s32 index = owner->index;
    s32 x = number_right_edge(index, 0xB8);

    draw_digits_c0_p11(index, x, y, 0);
    if (owner->sprite->flags & 8)
    {
        obj_release_node(owner->sprite);
        owner->timer = 0;
        owner->state = state_cursor_anim_step;
    }
}

void state_fx_step_counter(struct Owner *owner)
{
    u16 old;
    u32 t = draw_digit_cycle(owner, owner->posY * 3);
    u32 masked;

    owner->timer = t;
    masked = t & 0x7FF;
    if (masked == 0)
    {
        old = owner->posY;
        owner->posY = old + 1;
        if ((s16)old > 7)
        {
            owner->timer = masked;
            owner->state = state_draw_value_step;
        }
    }
}

void state_obj_anim_5_when_active(struct Owner *owner)
{
    struct Owner *obj = gUnk_03000FD8->obj278;

    if (obj->sprite->flags & 8)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->sprite->unk20 = 0x10;
        owner->state = state_select_pose;
    }
}

ASM_FUNC("asm/code_08107268/state_map_copy_and_list.s", void state_map_copy_and_list(void))

void draw_obj_at_xy(struct Owner *obj)
{
    draw_digits_49_p11(obj->index, obj->posX + 8, obj->posY - 8, 0);
}

void state_spawn_list_entry(struct Owner *owner)
{
    if (!is_state_busy())
    {
        struct Entry *entry = list_add_task(&gUnk_03000FD8->listHead, state_wait_result_then_close);

        s32 x = owner->posX;

        entry->unk12 = x;
        entry->unkC = owner->target;
        owner->state = state_figure_jump_start;
    }
}

void draw_entry_flags(struct Owner *obj)
{
    void *target = obj->target;
    s32 x = obj->posX;
    s32 y = obj->posY;
    u16 timer = obj->timer;

    draw_digits_49_p11((s32)target, x, y, ((s16)timer >> 12) & 1);
}

void state_load_record_when_free(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer > 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        u8 busy = is_state_busy();

        if (busy == 0)
        {
            void *obj = spawn_obj_by_id(0x7002, 0, 0, 0);

            obj_load_record(obj, 0, -1, 0, busy);
            display_pause();
            owner->state = state_heroes_walk_to_end;
        }
    }
}

ASM_FUNC("asm/code_08107268/state_clear_hero_flags.s", void state_clear_hero_flags(void))

void state_effect_mode_2(struct Owner *owner)
{
    struct Effect *effect = gUnk_03000FD8->oldEffect;
    s32 flags = effect->flags5;
    s32 mask = ~3;

    mask &= flags;
    flags = mask | 2;
    effect->flags5 = flags;
    owner->state = state_apply_hero_stats;
    set_cur_flags_pending(0x200000);
}

void state_effect_release(struct Owner *owner)
{
    struct Effect *effect = gUnk_03000FD8->oldEffect;
    s32 flags = effect->flags5;
    s32 mask = ~3;

    mask &= flags;
    effect->flags5 = mask;
    set_mark_callbacks();
    release_slot_objects();
    set_slot_target(1, 0xB, -1);
    owner->state = state_play_result_effects;
    set_cur_flags_pending(0x400000);
}

void state_wait_menu_ready(struct Owner *owner)
{
    if (world_list_idle())
    {
        struct World *world = gUnk_03000FD8;

        if ((world->flags2BF & 4) == 0)
        {
            if (world->menuMode346 == 2)
            {
                set_mark_state(0, 0);
                set_mark_state(1, 0);
            }
            owner->state = world_objects_tick;
        }
    }
}

void state_countdown_then_clear(struct Owner *owner)
{
    s32 timer = (u16)owner->timer - 1;

    owner->timer = timer;
    if ((s16)timer <= 0)
    {
        if (!is_state_busy())
        {
            update_slot_markers();
            owner->state = state_update_hero_markers;
        }
    }
}

void state_active_obj_release(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;

    if (world->task310 == 0)
    {
        u8 *cur = world->curObj;

        noop_stub_080826AC();
        run_sequence_of_obj_b(cur + 8);
        owner->state = world_mode_step;
        set_cur_flags_both(4);
    }
}

void state_obj_release_when_free(struct Owner *owner)
{
    if (!is_state_busy())
    {
        if ((gUnk_03000FD8->flags0B & 4) == 0)
        {
            pair_begin(gUnk_03000FD8->curObj + 8);
            change_item_count(gUnk_03000FD8->unk349, -1);
            gUnk_03000FD8->unk349 = 0xFF;
            owner->state = state_update_hero_markers;
            set_cur_flags(8);
        }
    }
}

void state_stop_menu_obj(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;

    if (world->task310 == 0)
    {
        sprite_load_tables(world->unk349);
        owner->state = state_obj_release_when_free;
        set_cur_flags_both(4);
    }
}

void state_free_saved_list(struct Owner *owner)
{
    void **slot = &gUnk_03000FD8->savedList;

    if (((struct SavedList *)*slot)->unk8 != 0)
    {
        do
        {
        } while (sprite_node_release() != 0);
    }
    obj_release_node(((struct SavedList *)*slot)->unk4);
    ((struct SavedList *)*slot)->unk4 = 0;
    *slot = 0;
    owner->state = gUnk_03000FDC->savedState;
}

void state_menu_open_active(struct Owner *owner)
{
    if (!is_state_busy())
    {
        s32 flags;

        owner->state = state_wait_menu_ready;
        set_cur_flags_both(0x100);
        reset_entry_part(gUnk_03000FD8->unk244, gUnk_03000FD8->curObj[0xF4],
                         *(u32 *)(gUnk_03000FD8->curObj + 0xE8));
        flags = gUnk_03000FD8->flags2BF;
        gUnk_03000FD8->flags2BF = flags | 4;
    }
}

ASM_FUNC("asm/code_08107268/state_show_figs.s", void state_show_figs(struct Owner *owner))

void state_fig_active_call(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;
    u32 bit = ((u32)world->figMode2C0 << 29) >> 31;
    s32 idx = 1;
    struct Obj *fig;

    idx ^= bit;
    fig = world->fig80[idx];
    if (fig->link->flags & 8)
    {
        owner->state = state_camera;
        owner->state(owner);
    }
}

void state_call_active_callback(struct Owner *owner)
{
    struct Callback *obj = (struct Callback *)(gUnk_03000FD8->curObj + 8);

    if (obj->fn != 0)
    {
        obj->fn(obj);
    }
    else
    {
        owner->state = gUnk_03000FDC->savedState;
        owner->state(owner);
    }
}

void state_call_saved_if_flag(struct Owner *owner)
{
    u8 *flags = gUnk_03000FD8->curObj + 0x122;

    if ((*flags & 0x20) == 0)
    {
        owner->state = gUnk_03000FDC->savedState;
        owner->state(owner);
    }
}

void state_obj_wait_then_select(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;
    u8 *cur = world->curObj;
    s32 flags = world->flags0B;

    if (flags & 2)
    {
        s32 mask = ~2;

        mask &= flags;
        world->flags0B = mask;
    }
    else if (*(s16 *)(cur + 0xF6) != 0)
    {
        if ((cur[0x7E] & 0x38) == 0)
        {
            owner->state = state_update_bits;
        }
    }
    else if (!is_state_busy())
    {
        owner->state = state_choose_hero;
    }
}

void state_restore_saved_when_flag(struct Owner *owner)
{
    struct World *world = gUnk_03000FD8;
    s32 flags = world->flags0B;

    if (flags & 2)
    {
        s32 mask = ~2;

        mask &= flags;
        world->flags0B = mask;
    }
    else
    {
        owner->state = gUnk_03000FDC->savedState;
    }
}

ASM_FUNC("asm/code_08107268/state_place_sprite_at_obj.s", void state_place_sprite_at_obj(struct Owner *owner))

void state_select_by_status(struct Owner *owner)
{
    s16 status = owner->scrollStep;
    u16 index;

    if (status != 1)
    {
        index = 0xE;
        if (status == 2)
        {
            index = 8;
        }
    }
    else
    {
        index = 0;
    }
    owner->index = index;
    owner->state = state_place_sprite_at_obj;
    load_table_entries(gEffectCodeTable[(u16)owner->index]);
}

void state_wait_result_then_close(struct Owner *owner)
{
    if (sub_08117390(gUnk_03000FD8->menu248) != 1)
    {
        void *target = owner->target;
        struct Cand *cand;

        set_mark_callbacks();
        cand = active_candidate(target);
        if (cand != 0)
        {
            draw_sprite_block(4, 0, 0x20, 0, cand->link->unk14);
        }
        owner->timer = 0xC;
        owner->state = state_wait_then_clear;
    }
}

void state_cursor_anim_step(struct Owner *owner)
{
    s32 y = (gUnk_03000FD8->pose342 << 4) + 0x28;
    s32 index = owner->index;
    s32 x = number_right_edge(index, 0xB8);

    draw_digits_c0_p11(index, x, y, 0);
    owner->timer++;
    if ((s16)owner->timer > 9)
    {
        list_add_task(&gUnk_03000FD8->listHead, state_scroll_clamp_then_finish);
        owner->timer = 0;
        owner->state = state_add_points_to_hero;
    }
}

void state_wait_free_then_2199(struct Owner *owner)
{
    if ((gUnk_03000FD8->flags0B & 4) == 0)
    {
        if (!is_state_busy())
        {
            candidate_dispatch();
            owner->state = state_fig_flags_reset;
        }
    }
}

void state_scroll_clamp_then_finish(struct Owner *owner)
{
    struct Scroll *scroll = gUnk_03000FD8->scrollObj37C;
    struct Scroll *obj = (struct Scroll *)gUnk_03000FD8->obj278;
    s32 v;
    s32 t;
    s32 p4;
    s32 p8;
    s32 pc;

    v = scroll->pos10 + 0x300;
    scroll->pos10 = v;
    obj->pos10 = v;
    t = scroll->pos10;
    if (t < 0)
    {
        t += 0xFF;
    }
    t >>= 8;
    p4 = t;
    p8 = 0;
    pc = 0;
    offset_three_words(gUnk_03000FD8->ptr24C, &p4, &p8, &pc, 0);
    if (p4 > 0x103)
    {
        release_marker_object();
        obj_release(scroll);
        gUnk_03000FD8->scrollObj37C = 0;
        owner->state = 0;
    }
}

void state_wait_then_clear(struct Owner *owner)
{
    s32 timer = owner->timer;

    if (timer != 0)
    {
        owner->timer--;
    }
    else
    {
        display_pause();
        owner->state = NULL;
    }
}

ASM_FUNC("asm/code_08107268/update_world_objects.s", void update_world_objects(void))

ASM_FUNC("asm/code_08107268/marker_flag100_update.s", void marker_flag100_update(void))

ASM_FUNC("asm/code_08107268/marker_flag1_update.s", void marker_flag1_update(void))

ASM_FUNC("asm/code_08107268/marker_flag2_update.s", void marker_flag2_update(void))

ASM_FUNC("asm/code_08107268/marker_cursor_update.s", void marker_cursor_update(void))

ASM_FUNC("asm/code_08107268/marker_flag400_update.s", void marker_flag400_update(void))

ASM_FUNC("asm/code_08107268/marker_flag800_update.s", void marker_flag800_update(void))
