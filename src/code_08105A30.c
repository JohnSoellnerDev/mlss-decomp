#include "global.h"
#include "ram_routines.h"

struct Sprite
{
    s16 x;
    s16 y;
    u8 filler4[0x8 - 0x4];
    u32 unk8;
    u8 filler0C[0xE - 0xC];
    u16 unk0E;
    u16 unk10;
    u8 flags;
    u8 filler13[0x14 - 0x13];
    u16 unk14;
    u8 filler16[0x1F - 0x16];
    u8 unk1F;
    u8 unk20;
};

struct Obj
{
    u8 filler0[0x10];
    struct Sprite *link;
    u8 filler14[0x18 - 0x14];
    s32 pos18;
    s32 pos1C;
    s32 pos20;
    u8 filler24[0x7E - 0x24];
    u8 unk7E;
    u8 filler7F[0xCC - 0x7F];
    s8 unkCC;
    s8 unkCD;
    u8 filler0CE[0xD8 - 0xCE];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    u8 filler0E4[0xF5 - 0xE4];
    u8 unkF5;
    u16 unkF6;
    u8 filler0F8[0xFC - 0xF8];
    u16 unkFC;
    u8 filler0FE[0x111 - 0xFE];
    u8 unk111;
};

struct CurObj
{
    u8 filler0[0xEC];
    s16 unkEC;
};

struct Pos38
{
    u8 filler0[0x38];
    s32 x;
    s32 y;
    s32 z;
};

struct Menu
{
    u8 filler0[0x34];
    u16 unk34;
    u8 filler36[0x4A - 0x36];
    u16 unk4A;
};

struct Entry
{
    u8 filler0[0xC];
    void *unkC;
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
    void *block;
};

struct World
{
    void *scene;
    u8 unk4;
    u8 filler5[0xB - 0x5];
    u8 flags0B;
    u8 filler0C[0x40 - 0xC];
    u32 unk40;
    u32 unk44;
    u8 filler48[0x4C - 0x48];
    void *listHead;
    u8 filler50[0x58 - 0x50];
    struct Obj *list58[8];
    u8 filler78[0x7C - 0x78];
    struct CurObj *curObj;
    struct Obj *fig80[2];
    u8 filler88[0x248 - 0x88];
    struct Menu *menu248;
    u8 filler24C[0x250 - 0x24C];
    void *mapSrc250;
    u8 filler254[0x290 - 0x254];
    u32 unk290;
    u8 filler294[0x2BF - 0x294];
    u8 flags2BF;
    u8 figMode2C0;
    u8 filler2C1[0x33E - 0x2C1];
    u8 unk33E;
    u8 hero33F;
    u8 hero340;
    u8 filler341[0x368 - 0x341];
    struct Sprite *menu368;
    u8 filler36C[0x370 - 0x36C];
    struct Sprite *digit370;
    u8 filler374[0x398 - 0x374];
    u32 unk398[3];
    u8 state3A4;
    u8 unk3A5;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern u8 gUnk_0300034C[];
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];

extern const u8 gClaimKindTable[];
extern const u32 gLangTextA[];
extern const u32 gLangTextB[];
extern const u32 gHeroEntryTextTable[];

void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_setup_load(void *obj, u32 a);
void *heap_alloc2(u32 size, u32 a, void *table, u32 c);
void *spawn_kind_sprite(void *buf, u32 kind, void *table, u32 a, u32 b, void *c, s32 d, u32 e,
                        void (*f)(void));
void menu_init_clear(void);
void state_menu_clear_vram(struct Owner *owner);
extern u8 gSceneTable0820218c[];
extern u8 *gWorkBuf;
u8 is_state_busy(void);
u8 sub_obj_running(void *menu);
u32 to_bcd(u8 value);
void state_build_hero_sprites(struct Owner *owner);
void menu_entry_refresh(void *menu, u32 idx, u32 zero);
void draw_sprite_block(u32 a, u32 b, u32 c, u32 d, u32 e);
void set_mark_callbacks(void);
void state_wait_menu_clear(struct Owner *owner);
void menu_open_setup(void);
void slot_reset(u32 value);
void serial_link_disable(void);
struct Sprite *spawn_obj_by_id(s32 id, u32 a, u32 b, u32 c);
void state_marker_reset(struct Owner *owner);
void state_marker_reset_then_anim(struct Owner *owner);
s32 move_step(s16 *outH, s16 *outX, s16 *outY, s32 dx, s32 dy, s32 dz, s32 speed);
void sprite_entry_update(void *obj);
void state_hero_anim_notify(struct Owner *owner);
void recalc_char_stats(u8 idx);
void state_assign_figure(struct Owner *owner);
struct Entry *list_add_task(void *list, void (*state)(struct Owner *));
void display_finish(u32 a, s32 b);
void state_spawn_list_entry(struct Owner *owner);
void state_figure_jump_start(struct Owner *owner);
void state_apply_hero_stats(struct Owner *owner);
void state_move_to_target(struct Owner *owner);
void state_check_slots(struct Owner *owner);

void obj_detach_active(struct Sprite *sprite);
void obj_release_node(void *node);
void sprite_set_action(void *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 slot, s32 value);
void copy_map_rect_add(void *src, u32 a, u32 b, u32 c, u32 d, u32 e, void *f, u32 g);
void state_check_slots(struct Owner *owner);
void state_hero_sprite_check(struct Owner *owner);
void state_marker_reset_then_text(struct Owner *owner);
void state_marker_place_hero(struct Owner *owner);
void state_toggle_map_tiles(struct Owner *owner);
void state_menu_alloc_scene(struct Owner *owner);
void state_hero_anim_when_active(struct Owner *owner);

ASM_FUNC("asm/code_08105A30/state_hero_select_confirm.s", void state_hero_select_confirm(void))

ASM_FUNC("asm/code_08105A30/state_init_map_tiles.s", void state_init_map_tiles(struct Owner *owner))

ASM_FUNC("asm/code_08105A30/state_toggle_map_tiles.s", void state_toggle_map_tiles(struct Owner *owner))

ASM_FUNC("asm/code_08105A30/state_move_to_target.s", void state_move_to_target(struct Owner *owner))

ASM_FUNC("asm/code_08105A30/state_draw_pose_digits.s", void state_draw_pose_digits(void))

ASM_FUNC("asm/code_08105A30/state_build_hero_sprites.s", void state_build_hero_sprites(struct Owner *owner))

ASM_FUNC("asm/code_08105A30/state_orbit_sprites.s", void state_orbit_sprites(void))

ASM_FUNC("asm/code_08105A30/state_apply_hero_stats.s", void state_apply_hero_stats(struct Owner *owner))

ASM_FUNC("asm/code_08105A30/state_menu_open_wait.s", void state_menu_open_wait(void))

ASM_FUNC("asm/code_08105A30/set_text_ptrs.s", void set_text_ptrs(void))

ASM_FUNC("asm/code_08105A30/next_text_id.s", void next_text_id(void))

u8 world_list_idle(void)
{
    struct Obj **list;
    s32 flags;
    s32 mask;
    s32 i;

    if (gUnk_03000FD8->unk40 != 0)
    {
        return 0;
    }
    if (gUnk_03000FD8->unk44 != 0)
    {
        return 0;
    }
    flags = gUnk_03000FD8->flags0B;
    if (flags & 2)
    {
        mask = ~2;
        mask &= flags;
        gUnk_03000FD8->flags0B = mask;
        return 0;
    }
    i = 0;
    list = &gUnk_03000FD8->list58[0];
    for (; i < 8; i += 4)
    {
        if (list[0]->unk111 & 1)
        {
            return 0;
        }
        if (list[0]->unk7E & 0x38)
        {
            return 0;
        }
        if (list[1]->unk111 & 1)
        {
            return 0;
        }
        if (list[1]->unk7E & 0x38)
        {
            return 0;
        }
        if (list[2]->unk111 & 1)
        {
            return 0;
        }
        if (list[2]->unk7E & 0x38)
        {
            return 0;
        }
        if (list[3]->unk111 & 1)
        {
            return 0;
        }
        if (list[3]->unk7E & 0x38)
        {
            return 0;
        }
        list += 4;
    }
    return 1;
}

ASM_FUNC("asm/code_08105A30/symbol_add.s", void symbol_add(void))

ASM_FUNC("asm/code_08105A30/oam_add_status_icons.s", void oam_add_status_icons(void))

void state_marker_show(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else if (!is_state_busy())
    {
        struct Obj *hero = gUnk_03000FD8->list58[gUnk_03000FD8->hero33F + 6];
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker;
        s32 vx;
        s32 vy;
        s32 t;
        s32 flags;
        s32 mask;

        sprite_set_action((u8 *)hero + 8, 0, gUnk_03000FD8->hero33F ? 0x2067 : 0x2038, 0);
        marker = spawn_obj_by_id(0x8004, 3, 2, 0);
        owner->sprite = marker;
        sprite_entry_update(marker);
        vx = pose->x;
        if (vx < 0)
        {
            vx += 0xFF;
        }
        marker->x = vx >> 8;
        t = pose->y - pose->z;
        if (t < 0)
        {
            t += 0xFF;
        }
        vy = t >> 8;
        marker->y = hero->unkCC + vy - hero->unkCD;
        flags = marker->flags;
        mask = ~6;
        mask &= flags;
        mask |= 2;
        marker->flags = mask;
        owner->timer = gClaimKindTable[3];
        owner->state = state_marker_reset;
        set_slot_value(gUnk_03000FD8->hero33F ? 0xFD : 0xEF, -1);
    }
}

void state_marker_reset(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        obj_detach_active(owner->sprite);
        sprite_set_action((u8 *)gUnk_03000FD8->list58[gUnk_03000FD8->hero340 + 6] + 8,
                          gUnk_03000FD8->hero340 ? 1 : 4, gUnk_03000FD8->hero340 ? 0x204E : 0x2001, 0);
        owner->timer = 0x10;
        owner->state = state_marker_place_hero;
        owner->state(owner);
    }
}

void state_marker_place_hero(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct Obj *hero = gUnk_03000FD8->list58[gUnk_03000FD8->hero340 + 6];
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker = owner->sprite;
        s32 vx;
        s32 vy;
        s32 flags;
        s32 mask;

        obj_load_record(marker, 1, -1, 0, signedTimer);
        sprite_entry_update(marker);
        vx = pose->x;
        if (vx < 0)
        {
            vx += 0xFF;
        }
        marker->x = vx >> 8;
        vy = pose->y - pose->z;
        if (vy < 0)
        {
            vy += 0xFF;
        }
        vy >>= 8;
        marker->y = hero->unkCC + vy - hero->unkCD;
        flags = marker->flags;
        mask = ~6;
        mask &= flags;
        mask |= 2;
        marker->flags = mask;
        owner->timer = gClaimKindTable[1];
        owner->state = state_marker_reset_then_text;
        set_slot_value(0x29, -1);
    }
}

ASM_FUNC("asm/code_08105A30/state_marker_text_place.s", void state_marker_text_place(void))

void state_marker_gfx_place(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct Obj *hero = gUnk_03000FD8->list58[gUnk_03000FD8->hero340 + 6];
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker = owner->sprite;
        s32 vx;
        s32 vy;
        s32 flags;
        s32 mask;

        obj_setup_load(marker, 0x8004);
        obj_load_record(marker, 0, -1, 0, signedTimer);
        sprite_entry_update(marker);
        vx = pose->x;
        if (vx < 0)
        {
            vx += 0xFF;
        }
        marker->x = vx >> 8;
        vy = pose->y - pose->z;
        if (vy < 0)
        {
            vy += 0xFF;
        }
        vy >>= 8;
        marker->y = hero->unkCC + vy - hero->unkCD;
        flags = marker->flags;
        mask = ~6;
        mask &= flags;
        mask |= 2;
        marker->flags = mask;
        owner->timer = gClaimKindTable[0];
        owner->state = state_hero_anim_notify;
        set_slot_value(0x28, -1);
    }
}

void state_hero_anim_notify(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct Owner *o8;
        s32 flags;
        s32 mask;

        obj_release_node(owner->sprite);
        o8 = (struct Owner *)((u8 *)gUnk_03000FD8->list58[gUnk_03000FD8->hero340 + 6] + 8);
        sprite_set_action(o8, 3, gUnk_03000FD8->hero340 ? 0x2067 : 0x2038, 0);
        flags = o8->sprite->flags;
        mask = ~6;
        mask &= flags;
        mask |= 2;
        o8->sprite->flags = mask;
        owner->state = state_hero_anim_when_active;
        set_slot_value(gUnk_03000FD8->hero340 ? 0x102 : 0xE9, -1);
    }
}

ASM_FUNC("asm/code_08105A30/state_hero_anim_when_active.s", void state_hero_anim_when_active(struct Owner *owner))

void state_heroes_anim_stop(struct Owner *owner)
{
    u16 timer = owner->timer;

    if (owner->timer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        s32 flags;
        s32 mask;

        sprite_set_action((u8 *)gUnk_03000FD8->list58[gUnk_03000FD8->hero33F + 6] + 8, -1, -1, 0);
        sprite_set_action((u8 *)gUnk_03000FD8->list58[gUnk_03000FD8->hero340 + 6] + 8, -1, -1, 0);
        flags = gUnk_03000FD8->flags0B;
        mask = ~1;
        mask &= flags;
        gUnk_03000FD8->flags0B = mask;
        owner->state = state_check_slots;
    }
}

ASM_FUNC("asm/code_08105A30/state_menu_build.s", void state_menu_build(void))

ASM_FUNC("asm/code_08105A30/state_menu_slide_in.s", void state_menu_slide_in(struct Owner *owner))

void state_menu_alloc_scene(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct World *world = gUnk_03000FD8;
        u32 figIdx;
        struct Obj *fig;
        struct Pos38 *pose;
        u8 *work;
        void *buf;
        s32 vx;
        s32 vy;
        s32 t;
        u8 bit;

        figIdx = 0;
        if (world->fig80[1] != 0)
        {
            bit = world->figMode2C0 & 4;
            figIdx = bit != 0;
        }
        fig = world->fig80[figIdx];
        pose = (struct Pos38 *)((u8 *)fig + 8);
        work = gWorkBuf;
        vx = pose->x;
        if (vx < 0)
        {
            vx += 0xFF;
        }
        vx >>= 8;
        *(s16 *)(work + 0x8E10) = vx;
        t = pose->y - pose->z;
        if (t < 0)
        {
            t += 0xFF;
        }
        vy = t >> 8;
        vy -= 8;
        *(s16 *)(work + 0x8E12) = vy;
        *(s16 *)(work + 0x8E14) = 0xF0;
        buf = heap_alloc2(0x44, 0, gSceneTable0820218c, 0);
        world->scene = spawn_kind_sprite(buf, 7, gSceneTable0820218c, 0xF, 0x168,
                                         gWorkBuf + 0x8E10, signedTimer, 0x37, menu_init_clear);
        owner->state = state_menu_clear_vram;
    }
}
