#include "global.h"

struct FigSub
{
    u8 filler0[4];
    s8 unk4;
    s8 unk5;
    s8 unk6;
};

struct Marker
{
    struct Marker *link;
    u32 x;
    u32 y;
    u32 z;
    u8 filler10[0x1B - 0x10];
    u8 flags1B;
};

struct Fig
{
    u8 filler0[0x34];
    u8 *target34;
    u8 filler38[0x40 - 0x38];
    s32 x40;
    s32 y44;
    s32 z48;
    u8 filler4C[0x74 - 0x4C];
    const u8 *row74;
    u8 filler78[0x7E - 0x78];
    u8 ctrl7E;
    u8 flags7F;
    u8 filler80[0xE0 - 0x80];
    s32 unkE0;
    struct FigSub *sub;
    u8 fillerE8[0xF0 - 0xE8];
    u32 *tableF0;
    u8 slotF4;
    u8 filler0F5[0x110 - 0xF5];
    u8 flags110;
};

struct MenuOwner
{
    u8 filler0[4];
    void (*state)(struct MenuOwner *);
    void (*state2)(struct MenuOwner *);
};

struct SpriteRef
{
    u16 h0;
    u16 h2;
    u8 filler4[0x21 - 4];
    u8 b21;
    u8 filler22;
    u8 b23;
    u8 filler24[0x2A - 0x24];
    u16 h2A;
};

struct Node
{
    u32 unk0;
    struct SpriteRef *sprite;
    struct Node *next;
};

struct HeroRef
{
    u8 filler0[8];
    struct SpriteRef *ref;
    u8 filler0C[0x80 - 0x0C];
    u32 marker80;
};

struct Panel
{
    u8 filler0[0x10];
    u16 a10;
    u8 filler12[0x18 - 0x12];
    u16 a18;
};

struct World
{
    u8 filler0[0x24];
    void (*fn24)(struct MenuOwner *);
    u8 filler28[0x58 - 0x28];
    struct Fig *list[8];
    u8 filler78[0x7C - 0x78];
    struct Fig *active;
    u8 filler80[0x1B8 - 0x80];
    u32 unk1B8;
    u8 *target1BC;
    u8 filler1C0[0x278 - 0x1C0];
    struct HeroRef *hero278;
    u8 filler27C[0x2BD - 0x27C];
    u8 mode2BD;
    u8 filler2BE[0x2C0 - 0x2BE];
    u8 b2C0;
    u8 filler2C1[0x2DC - 0x2C1];
    u8 *rowPtr2DC;
    u8 filler2E0[0x310 - 0x2E0];
    struct Panel *panel310;
    u8 filler314[0x31C - 0x314];
    u8 table31C[0x33E - 0x31C];
    u8 cursorB33E;
    u8 unk33F;
    u8 filler340[0x342 - 0x340];
    u8 cursor342;
    u8 filler343[0x345 - 0x343];
    u8 unk345;
    u8 mode346;
    u8 sel347;
    u8 filler348[0x352 - 0x348];
    s8 cursor352;
    u8 filler353[0x364 - 0x353];
    struct Node *node364;
    struct SpriteRef *marker368;
};

extern struct World *gWorld;
extern u8 gSys[];
extern const u8 gAnim278[];
extern const u8 gAnim374[];
extern const u32 gStatusTable[];
extern u8 gStatusRowA[];
extern u8 gStatusRowB[];

void hero_marker_sync(void);
void hero_marker_create(struct MenuOwner *owner);
void state_hero_menu_plain(struct MenuOwner *owner);
void state_hero_menu_marker(struct MenuOwner *owner);
u32 pick_target_by_input(struct Fig *obj, s32 mode);
void show_slot4_text_centered(u32 value);
void status_anim_start(void);
s32 set_status_entry(u32 a, u32 b, u32 c);
void clear_world_effect(void);
void refresh_world_object(struct Fig *obj, s32 mode);
void set_world_mode_byte(u8 value);
void set_slot_value(u32 id, s32 value);
struct Node *sprite_node_take(void);
struct Node *sprite_node_alloc(u32 a, u32 b, u32 c);
struct SpriteRef *spawn_obj_by_id(u32 id, u32 a, u32 b, u32 c);
void sprite_entry_update(struct SpriteRef *sprite);
struct Fig *active_candidate(struct Fig *obj);
void hero_select_input(void);
void hero_select_target(void);
u32 status_cursor_step(u32 a, u32 b);
void show_slot4_text_left(u32 value);
void set_world_effect(s32 value);
void release_slot4_tiles(void);
void queue_obj_upload_entry(void);
void state_status_release(struct Panel *panel);
void release_marker_object(void);
void queue_mark_display(void);
s32 slot_marker_step(u32 cursor);
void set_status_word(void);
void state_setup_menu(struct MenuOwner *owner);
void state_wait_free_slots(struct MenuOwner *owner);
void hero_select_cursor(struct MenuOwner *owner);
void hero_select_pick_step(struct MenuOwner *owner);
void hero_select_frame(struct MenuOwner *owner);


ASM_FUNC("asm/code_080FF1DC/hero_select_step.s", void hero_select_step(void))

NONMATCH("asm/code_080FF1DC/hero_select_target.s", void hero_select_target(void))
{
    struct World *world;
    struct Fig *active;
    struct Fig *best;
    struct Fig *obj;
    s8 *cursor;
    u8 *sys;
    s32 i;
    s32 dNew;
    s32 dBest;
    u32 *tab;
    u32 *entry;
    s32 flags;
    s32 sel;
    s32 v;

    active = gWorld->active;
    if ((u8)pick_target_by_input(active, 2) != 0)
    {
        best = 0;
        i = 0;
        sys = gSys;
        world = gWorld;
        cursor = &world->cursor352;
        for (; i < 8; i++)
        {
            obj = world->list[i];
            if ((obj->ctrl7E & 6) != 4)
                continue;
            if (obj->flags110 & 0x20)
                continue;
            if (*cursor == obj->slotF4)
            {
                best = obj;
                break;
            }
            if (best != 0)
            {
                dNew = obj->x40 / 0x100 + obj->sub->unk4;
                dBest = best->x40;
                if (dBest < 0)
                    dBest += 0xFF;
                dBest = (dBest >> 8) + best->sub->unk4;
                if (dNew >= dBest)
                    continue;
            }
            best = obj;
        }
        gWorld->target1BC = (u8 *)best + 8;
        gWorld->active->target34 = (u8 *)best + 8;
        tab = best->tableF0;
        entry = (u32 *)((sys[0x8C0] << 2) + tab[0]);
        show_slot4_text_centered(*(u32 *)*entry);
        status_anim_start();
        set_status_entry(1, 0, 0);
        clear_world_effect();
        refresh_world_object(best, 0xD);
        set_world_mode_byte(2);
        sel = gWorld->sel347 & 0x1F;
        flags = active->flags110;
        flags = (flags & -0x20) | sel;
        active->flags110 = flags;
        v = ((u32)flags << 27) >> 27;
        if (active == gWorld->list[6])
            active->row74 = gAnim278 + v * 12;
        else
            active->row74 = gAnim374 + v * 12;
        gWorld->mode2BD = 2;
        set_slot_value(0x77, -1);
        gWorld->fn24 = hero_marker_create;
    }
}
END_NONMATCH


ASM_FUNC("asm/code_080FF1DC/hero_select_cursor.s", void hero_select_cursor(struct MenuOwner *owner))

ASM_FUNC("asm/code_080FF1DC/hero_select_pick_step.s", void hero_select_pick_step(struct MenuOwner *owner))

ASM_FUNC("asm/code_080FF1DC/hero_select_frame.s", void hero_select_frame(struct MenuOwner *owner))

ASM_FUNC("asm/code_080FF1DC/hero_marker_sync.s", void hero_marker_sync(void))

NONMATCH("asm/code_080FF1DC/hero_marker_create.s", void hero_marker_create(struct MenuOwner *owner))
{
    struct World *world;
    struct Panel *panel;
    struct HeroRef *hero;
    struct Node *node;
    struct Fig **slot;
    struct Fig *obj;
    struct Fig *cand;
    struct SpriteRef *sprite;
    u32 prev;
    u32 kind;
    u32 mode;
    s32 i;

    world = gWorld;
    hero = world->hero278;
    prev = hero->marker80;
    if (prev != 0)
        return;
    slot = (struct Fig **)((u8 *)world + 0x58);
    gWorld->node364 = sprite_node_take();
    gWorld->node364->sprite = spawn_obj_by_id(0x7001, 0x16, 1, 1);
    sprite_entry_update(gWorld->node364->sprite);
    mode = gWorld->mode2BD;
    if (mode != 3)
    {
        kind = mode & 0x7F;
        gWorld->node364->next = (struct Node *)prev;
        for (i = 7; i >= 0; i -= 2)
        {
            obj = slot[0];
            if ((((u32)obj->ctrl7E << 29) >> 30) == kind)
            {
                if (!(obj->flags110 & 0x20))
                {
                    node = sprite_node_alloc(0x80000000, 0, 0);
                    world = gWorld;
                    node->unk0 = (u32)world->node364->next;
                    world->node364->next = node;
                }
            }
            obj = slot[1];
            if ((((u32)obj->ctrl7E << 29) >> 30) == kind)
            {
                if (!(obj->flags110 & 0x20))
                {
                    node = sprite_node_alloc(0x80000000, 0, 0);
                    world = gWorld;
                    node->unk0 = (u32)world->node364->next;
                    world->node364->next = node;
                }
            }
            slot += 2;
        }
    }
    else
    {
        node = sprite_node_alloc(0x80000000, 0, 0);
        node->unk0 = prev;
        gWorld->node364->next = node;
    }
    hero_marker_sync();
    world = gWorld;
    panel = world->panel310;
    panel->a10 = 0;
    panel->a18 = 0;
    if (world->mode2BD & 0x80)
    {
        cand = active_candidate(world->active);
        if (cand != 0)
        {
            sprite = spawn_obj_by_id(0x7001, gWorld->hero278->ref->b21, 2, 1);
            sprite_entry_update(sprite);
            sprite->h0 = (cand->x40 / 0x100) + 0x20;
            sprite->h2 = (cand->y44 / 0x100) - 4;
            sprite->b23 = gWorld->hero278->ref->b23;
            sprite->h2A = gWorld->hero278->ref->h2A + 1;
            gWorld->marker368 = sprite;
        }
        owner->state = state_hero_menu_marker;
    }
    else
    {
        owner->state = state_hero_menu_plain;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080FF1DC/state_hero_menu_plain.s", void state_hero_menu_plain(struct MenuOwner *owner))
