#include "global.h"
#include "ram_routines.h"

struct MsBits
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 rest : 3;
};

struct PoseTail
{
    u8 filler0[0xC4];
    s8 unkC4; /* aim point offset Y (pixels), as Obj.unkCC */
    s8 unkC5; /* aim point offset height, as Obj.unkCD */
};

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
    s32 posX;   /* position, 1/0x100 pixels */
    s32 posY;
    s32 posH;
    u8 filler24[0x7E - 0x24];
    u8 unk7E;
    u8 filler7F[0xCC - 0x7F];
    s8 unkCC;
    s8 unkCD;
    u8 filler0CE[0xD8 - 0xCE];
    s32 destX;  /* where state_move_to_target takes it */
    s32 destY;
    s32 destH;
    u8 filler0E4[0xF5 - 0xE4];
    u8 level;   // battle copies of the hero's PartyMember stats
    u16 hp;
    u16 maxHp;
    u8 filler0FA[0xFC - 0xFA];
    u16 bp;
    u16 maxBp;
    u8 filler100[0x102 - 0x100];
    u16 unk102; // PartyMember stat1C
    u16 unk104; // PartyMember stat24
    u16 unk106; // PartyMember stat20
    u16 unk108; // PartyMember stat28
    u8 filler10A[0x111 - 0x10A];
    u8 unk111;
    u8 filler112[0x12B - 0x112];
    u8 unk12B; // PartyMember unk35 + 1
    u8 unk12C; // PartyMember unk37 + 1
    u8 unk12D; // PartyMember unk38 + 1
};

struct CurObj
{
    u8 filler0[0xEC];
    s16 notPartyIndex; // ~index of the hero's record in gParty
};

// A hero's record in gParty (0x3C bytes each).
struct PartyMember
{
    u8 filler0[0xE];
    u16 hp;
    u16 unk10;
    u16 maxHp;
    u16 bp;
    u16 unk16;
    u16 maxBp;
    u8 filler1A[0x1C - 0x1A];
    u16 stat1C; // POW, DEF, SPEED and STACHE are presumably these four
    u8 filler1E[0x20 - 0x1E];
    u16 stat20;
    u8 filler22[0x24 - 0x22];
    u16 stat24;
    u8 filler26[0x28 - 0x26];
    u16 stat28;
    u8 filler2A[0x30 - 0x2A];
    u8 level;
    u8 filler31[0x35 - 0x31];
    u8 unk35;
    u8 filler36;
    u8 unk37;
    u8 unk38;
    u8 filler39[0x3C - 0x39];
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
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

/* A task list_add_task adds; it runs its state with itself as the Owner */
struct Entry
{
    u8 filler0[0xC];
    void *target; /* Owner.target of the task */
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

struct OrbitNode
{
    struct OrbitNode *next;
    s32 x;
    u8 filler8[0x4];
    s32 y;
};

struct OrbitRoot
{
    u8 filler0[0xC];
    struct OrbitNode head;
    u8 filler1C[0x9C - 0x1C];
    s32 scale;
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
    struct Obj *obj24C;
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
    u8 filler374[0x378 - 0x374];
    struct OrbitRoot *orbit378;
    u8 filler37C[0x398 - 0x37C];
    u32 unk398[3];
    u8 state3A4;
    u8 unk3A5;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gSys[];
#define SYS_LANGUAGE 0x8C0 /* gSys[SYS_LANGUAGE]: language index 0-4 */
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];

extern const u8 gClaimKindTable[];
extern const u32 gLangTextA[];
extern const u32 gLangTextB[];
extern const u32 *gHeroEntryTextTable[][3];

void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_setup_load(void *obj, u32 a);
void *heap_alloc2(u32 size, u32 a, void *table, u32 c);
/* starts a screen wipe task of shape `shape` lasting `duration` frames (see code_08079F40.c) */
void *spawn_kind_sprite(void *task, u32 taskKind, void *taskData, u32 shape, u32 duration,
                        void *releaseArg, s32 window, u32 windowArg, void (*release)(void));
/* gWorkBuf + WORK_WIPE_POINT: three s16 (x, y, size) a wipe is centred on */
#define WORK_WIPE_POINT 0x8E10
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
s32 move_step(s16 *outX, s16 *outY, s16 *outH, s32 dx, s32 dy, s32 dh, s32 speed);
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
void copy_map_rect_add(void *win, u32 map, u32 x, u32 y, u32 w, u32 h, void *src, u32 add);
void state_check_slots(struct Owner *owner);
void state_hero_sprite_check(struct Owner *owner);
void state_marker_reset_then_text(struct Owner *owner);
void state_marker_place_hero(struct Owner *owner);
void state_toggle_map_tiles(struct Owner *owner);
void state_menu_alloc_scene(struct Owner *owner);
void state_hero_anim_when_active(struct Owner *owner);

/* Level-up screen, once A is pressed: recalculates the shown hero's stats and gives its figure the new
 * level, then state_assign_figure looks for a hero with another level up. If it's the other hero, a
 * task moves this figure back (state_move_to_target) and the screen goes on to state_spawn_list_entry;
 * the same hero again goes to state_figure_jump_start. No more level ups: state_apply_hero_stats. */
void state_hero_select_confirm(struct Owner *owner)
{
    struct PartyMember *member;
    struct Obj *figure;
    struct Entry *task;
    s32 partyIndex;

    if ((((struct SysKeys3E *)gSys)->keys ^ A_BUTTON) & A_BUTTON)
    {
        return;
    }
    partyIndex = owner->posX;
    member = (struct PartyMember *)(gParty + partyIndex * 0x3C);
    recalc_char_stats(partyIndex);
    figure = owner->target;
    figure->level = member->level;
    state_assign_figure(owner);
    if (owner->state != state_apply_hero_stats)
    {
        if (owner->target != figure)
        {
            task = list_add_task(&gWorld->listHead, state_move_to_target);
            task->target = figure;
            display_finish(1, -1);
            gWorld->menu248->unk34 = 0x20;
            gWorld->menu248->unk4A = 0xC;
            owner->state = state_spawn_list_entry;
        }
        else
        {
            owner->state = state_figure_jump_start;
        }
    }
}

/* Draws frame 0 of the animated border strip (gMapTable entry gGlyphIndexTable[0x27], an offset
 * from gMapTable) as the 30x2-cell top (row 0) and bottom (row 18) of tilemap 1 with palette 8
 * (+0x8000), then lets state_toggle_map_tiles flip between its two frames (posX), timer from 0. */
void state_init_map_tiles(struct Owner *owner)
{
    copy_map_rect_add(gWorld->mapSrc250, 1, 0, 0, 0x1E, 2,
                      (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[0x27] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x8000);
    copy_map_rect_add(gWorld->mapSrc250, 1, 0, 0x12, 0x1E, 2,
                      (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[0x27] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x8000);
    owner->posX = 0; /* border frame */
    owner->timer = 0;
    owner->state = state_toggle_map_tiles;
}

NONMATCH("asm/code_08105A30/state_toggle_map_tiles.s", void state_toggle_map_tiles(struct Owner *owner))
{
    u16 timer = owner->timer;
    s32 step = timer + 1;

    owner->timer = step;
    if ((s16)step > 12)
    {
        owner->timer = 0;
        owner->posX = (u16)owner->posX ^ 1;
        copy_map_rect_add(gWorld->mapSrc250, 1, 0, 0, 0x1E, 2,
                          (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[0x27] << 2) + (u32)gMapTable) + ((s32)owner->posX << 2) + (u32)gMapTable), 0x8000);
        copy_map_rect_add(gWorld->mapSrc250, 1, 0, 0x12, 0x1E, 2,
                          (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[0x27] << 2) + (u32)gMapTable) + ((s32)owner->posX << 2) + (u32)gMapTable), 0x8000);
    }
}
END_NONMATCH

/* Task state: moves the owner's figure toward its destination at 4 pixels a frame (move_step). Once
 * it is within 18 pixels it snaps onto the destination, sets all four 2-bit fields of its sprite's
 * byte 0x1F to 2 and ends the task. */
void state_move_to_target(struct Owner *owner)
{
    struct Obj *obj;
    s8 *flags;
    u8 *ufl;
    s16 stepX;
    s16 stepY;
    s16 stepH;

    obj = owner->target;
    if (move_step(&stepX, &stepY, &stepH, obj->destX - obj->posX, obj->destY - obj->posY, obj->destH - obj->posH, 0x400) > 0x480)
    {
        obj->posX += stepX;
        obj->posY += stepY;
        obj->posH += stepH;
    }
    else
    {
        flags = (s8 *)&obj->link->unk1F;
        *flags = (*flags & ~0x03) | 0x02;
        flags = (s8 *)&obj->link->unk1F;
        *flags = (*flags & ~0x0C) | 0x08;
        flags = (s8 *)&obj->link->unk1F;
        *flags = (*flags & ~0x30) | 0x20;
        ufl = &obj->link->unk1F;
        *ufl = (*ufl & 0x3F) | 0x80;
        obj->posX = obj->destX;
        obj->posY = obj->destY;
        obj->posH = obj->destH;
        owner->state = 0;
    }
}

NONMATCH("asm/code_08105A30/state_draw_pose_digits.s", void state_draw_pose_digits(struct Owner *owner))
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;
    u32 digits;
    u32 cell;
    u32 vram;
    u8 **mr;
    s32 idx;
    u8 *hero;

    if (signedTimer > 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        idx = owner->posX;
        hero = gParty + idx * 0x3C;
        digits = to_bcd(hero[0x30]);
        gWorld->digit370->unk8 = digits;
        gWorld->digit370->unk10 = 0x1E;
        mr = &gMemRoutines;
        vram = 0x06010000;
        ((void (*)(u32, void *, u32))*mr)(0, (void *)0x06017200, 0x400);
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)((((cell = (digits & 0xF) << 1) + 0x49) << 5) + vram), (u8 *)(vram + 0x72C0), 0x20);
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)(((cell + 0x4A) << 5) + vram), (u8 *)(vram + 0x7340), 0x20);
        digits >>= 4;
        if (digits != 0)
        {
            u32 hcell;

            MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)((((hcell = (digits &= 0xF) << 1) + 0x49) << 5) + 0x06010000), (u8 *)0x060174A0, 0x20);
            MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)(((hcell + 0x4A) << 5) + 0x06010000), (u8 *)0x06017520, 0x20);
        }
        owner->state = state_build_hero_sprites;
    }
}

END_NONMATCH

ASM_FUNC("asm/code_08105A30/state_build_hero_sprites.s", void state_build_hero_sprites(struct Owner *owner))

void obj_release(void *obj);
s32 obj_last_x(void *obj);

NONMATCH("asm/code_08105A30/state_orbit_sprites.s", void state_orbit_sprites(struct Owner *owner))
{
    struct OrbitRoot *chain;
    struct OrbitNode *node;
    s32 *radius;
    s32 i;
    u32 angle;
    s32 centerX;

    chain = gWorld->orbit378;
    radius = &chain->scale;
    *radius += 6;
    node = &chain->head;
    for (i = 0; i < 12; i++)
    {
        angle = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(i << 8, 0xC);
        centerX = ((s32)(obj_last_x(gWorld->obj24C) << 16) >> 8) + 0x7800;
        node->x = centerX + (gCosineTable[angle & 0xFF] / 0x40) * *radius;
        node->y = (gSineTable[angle & 0xFF] / 0x40) * *radius + 0x5000;
        node = node->next;
    }
    if (*radius > 159)
    {
        obj_release(chain);
        gWorld->orbit378 = 0;
        owner->timer = 0x28;
        owner->state = 0;
    }
}
END_NONMATCH

void set_cur_flags_pending(u32 flags);
void state_menu_open_wait(struct Owner *owner);
// Copies each hero's level, HP, BP and stats from its gParty record into its battle figure
// (an HP of 0 becomes 1), raises the pending flag 0x10 and moves on to state_menu_open_wait.
void state_apply_hero_stats(struct Owner *owner)
{
    u8 *recordEnd;
    struct PartyMember *member;
    struct Obj *fig;
    u16 hp;
    u16 partyHp;

    // The figure keeps ~index, so -notPartyIndex records in is the end of the hero's record.
    fig = gWorld->fig80[0];
    recordEnd = gParty - ((struct CurObj *)fig)->notPartyIndex * 0x3C;
    member = (struct PartyMember *)(recordEnd - 0x3C);
    ((struct MsBits *)&fig->unk7E)->mode = 1;
    fig->level = ((struct PartyMember *)recordEnd)[-1].level;
    partyHp = member->hp;
    hp = 1;
    if (partyHp != 0)
    {
        hp = partyHp;
    }
    fig->hp = hp;
    fig->maxHp = member->maxHp;
    fig->unk106 = member->stat20;
    fig->unk102 = member->stat1C;
    fig->unk104 = member->stat24;
    fig->unk108 = member->stat28;
    fig->bp = member->bp;
    fig->maxBp = member->maxBp;
    fig->unk12B = ((struct PartyMember *)recordEnd)[-1].unk35 + 1;
    fig->unk12C = ((struct PartyMember *)recordEnd)[-1].unk37 + 1;
    fig->unk12D = ((struct PartyMember *)recordEnd)[-1].unk38 + 1;

    fig = gWorld->fig80[1];
    if (fig != 0)
    {
        recordEnd = gParty - ((struct CurObj *)fig)->notPartyIndex * 0x3C;
        member = (struct PartyMember *)(recordEnd - 0x3C);
        ((struct MsBits *)&fig->unk7E)->mode = 1;
        fig->level = ((struct PartyMember *)recordEnd)[-1].level;
        partyHp = member->hp;
        hp = 1;
        if (partyHp != 0)
        {
            hp = partyHp;
        }
        fig->hp = hp;
        fig->maxHp = member->maxHp;
        fig->unk106 = member->stat20;
        fig->unk102 = member->stat1C;
        fig->unk104 = member->stat24;
        fig->unk108 = member->stat28;
        fig->bp = member->bp;
        fig->maxBp = member->maxBp;
        fig->unk12B = ((struct PartyMember *)recordEnd)[-1].unk35 + 1;
        fig->unk12C = ((struct PartyMember *)recordEnd)[-1].unk37 + 1;
        fig->unk12D = ((struct PartyMember *)recordEnd)[-1].unk38 + 1;
    }
    set_cur_flags_pending(0x10);
    owner->timer = 1;
    owner->state = state_menu_open_wait;
}

// Waits out owner->timer, then for flag 2 of gWorld->flags2BF with the menu no longer running. Then
// it either opens the menu (gWorld->unk4 is 0 and figMode2C0 bit 0x20 is clear) or, unless flag 0x20
// is set, copies each hero figure's HP and BP back into its gParty record (an HP of 0 becomes 1),
// turns the serial link off and ends the task. The counterpart of state_apply_hero_stats.
void state_menu_open_wait(struct Owner *owner)
{
    s32 partyIndex;
    struct PartyMember *member;

    if (owner->timer != 0)
    {
        owner->timer--;
    }
    else if ((gWorld->flags2BF & 2) != 0 && sub_obj_running(gWorld->menu248) == 0)
    {
        if (gWorld->unk4 == 0 && (gWorld->figMode2C0 & 0x20) == 0)
        {
            menu_open_setup();
            return;
        }
        if ((gWorld->flags2BF & 0x20) == 0)
        {
            slot_reset(1);
            partyIndex = ~((struct CurObj *)gWorld->fig80[0])->notPartyIndex;
            member = (struct PartyMember *)(gParty + partyIndex * 0x3C);
            member->hp = gWorld->list58[partyIndex + 6]->hp;
            member->bp = gWorld->list58[partyIndex + 6]->bp;
            if (member->hp == 0)
            {
                member->hp = 1;
            }
            if (gWorld->fig80[1] != 0)
            {
                partyIndex = ~((struct CurObj *)gWorld->fig80[1])->notPartyIndex;
                member = (struct PartyMember *)(gParty + partyIndex * 0x3C);
                member->hp = gWorld->list58[partyIndex + 6]->hp;
                member->bp = gWorld->list58[partyIndex + 6]->bp;
                if (member->hp == 0)
                {
                    member->hp = 1;
                }
            }
            serial_link_disable();
            owner->state = 0;
        }
    }
}

/* Loads the three texts next_text_id cycles through (unk398) from entry `entry` of
 * gHeroEntryTextTable in the current language (0 where the entry has none), restarting the
 * cycle (state3A4) first when unk290 is set. */
void set_text_ptrs(u32 entry)
{
    if (gWorld->unk290 != 0)
    {
        gWorld->state3A4 = 0;
    }
    gWorld->unk398[0] = gHeroEntryTextTable[entry][0] != NULL ? gHeroEntryTextTable[entry][0][gSys[SYS_LANGUAGE]] : 0;
    gWorld->unk398[1] = gHeroEntryTextTable[entry][1] != NULL ? gHeroEntryTextTable[entry][1][gSys[SYS_LANGUAGE]] : 0;
    gWorld->unk398[2] = gHeroEntryTextTable[entry][2] != NULL ? gHeroEntryTextTable[entry][2][gSys[SYS_LANGUAGE]] : 0;
}

/* Returns the next non-zero text of the three set_text_ptrs loaded, cycling state3A4 through
 * 0, 1, 2. The second text is a table of 8 texts per hero: it is resolved with unk3A5, taking
 * hero 1's half when the current object is party member 1 (notPartyIndex ~1), and is first swapped
 * for gLangTextB when it is gLangTextA and unk33E is at most 1. */
u32 next_text_id(void)
{
    u32 text;
    u8 index;

    do
    {
        index = gWorld->state3A4;
        text = gWorld->unk398[index];
        if (index == 2)
        {
            gWorld->state3A4 = 0;
        }
        else
        {
            gWorld->state3A4 = index + 1;
        }
    } while (text == 0);

    /* the text just taken was the second one */
    if (gWorld->state3A4 == 2)
    {
        if (gWorld->unk33E <= 1)
        {
            if (text == gLangTextA[gSys[SYS_LANGUAGE]])
            {
                text = gLangTextB[gSys[SYS_LANGUAGE]];
            }
        }
        if (gWorld->curObj->notPartyIndex == ~1)
        {
            text = ((u32 *)text)[gWorld->unk3A5 + 8];
        }
        else
        {
            text = ((u32 *)text)[gWorld->unk3A5];
        }
    }
    return text;
}

/* Returns 1 when the battle is idle, so a state may go on: gWorld->unk40 and unk44 are clear and
   none of the 8 slot objects has unk111 bit 0 or any of unk7E bits 3-5 set. If flags0B bit 1 is
   set, it clears it and returns 0, so the caller waits one more frame. Returns 0 otherwise. */
u8 world_list_idle(void)
{
    struct Obj **slot;
    s32 flags;
    s32 mask;
    s32 i;

    if (gWorld->unk40 != 0)
    {
        return 0;
    }
    if (gWorld->unk44 != 0)
    {
        return 0;
    }
    flags = gWorld->flags0B;
    if (flags & 2)
    {
        mask = ~2;
        mask &= flags;
        gWorld->flags0B = mask;
        return 0;
    }
    /* the slot table, four slots per pass */
    i = 0;
    slot = &gWorld->list58[0];
    for (; i < 8; i += 4)
    {
        if (slot[0]->unk111 & 1)
        {
            return 0;
        }
        if (slot[0]->unk7E & 0x38)
        {
            return 0;
        }
        if (slot[1]->unk111 & 1)
        {
            return 0;
        }
        if (slot[1]->unk7E & 0x38)
        {
            return 0;
        }
        if (slot[2]->unk111 & 1)
        {
            return 0;
        }
        if (slot[2]->unk7E & 0x38)
        {
            return 0;
        }
        if (slot[3]->unk111 & 1)
        {
            return 0;
        }
        if (slot[3]->unk7E & 0x38)
        {
            return 0;
        }
        slot += 4;
    }
    return 1;
}

ASM_FUNC("asm/code_08105A30/symbol_add.s", void symbol_add(void))

ASM_FUNC("asm/code_08105A30/oam_add_status_icons.s", void oam_add_status_icons(void))

/* Marker sequence, first step: once the timer runs out and no state is busy, hero hero33F takes
 * action 0 (sprite 0x2067 or 0x2038), a new marker sprite 0x8004 appears at that hero's aim point
 * (animation mode 1), and its sound (0xFD or 0xEF) plays. After gClaimKindTable[3] frames
 * state_marker_reset follows. */
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
        struct Obj *hero = gWorld->list58[gWorld->hero33F + 6];
        /* the hero's body, at Obj + 8 */
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker;
        s32 x;
        s32 y;
        s32 groundY;
        s32 flags;
        s32 mask;

        sprite_set_action((u8 *)hero + 8, 0, gWorld->hero33F ? 0x2067 : 0x2038, 0);
        marker = spawn_obj_by_id(0x8004, 3, 2, 0);
        owner->sprite = marker;
        sprite_entry_update(marker);
        /* positions are in 1/256 pixels; the screen y is y - z */
        x = pose->x;
        if (x < 0)
        {
            x += 0xFF;
        }
        marker->x = x >> 8;
        groundY = pose->y - pose->z;
        if (groundY < 0)
        {
            groundY += 0xFF;
        }
        y = groundY >> 8;
        marker->y = hero->unkCC + y - hero->unkCD;
        /* MsBits mode (bits 1-2) = 1 */
        flags = marker->flags;
        mask = ~6;
        mask &= flags;
        mask |= 2;
        marker->flags = mask;
        owner->timer = gClaimKindTable[3];
        owner->state = state_marker_reset;
        set_slot_value(gWorld->hero33F ? 0xFD : 0xEF, -1);
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
        sprite_set_action((u8 *)gWorld->list58[gWorld->hero340 + 6] + 8,
                          gWorld->hero340 ? 1 : 4, gWorld->hero340 ? 0x204E : 0x2001, 0);
        owner->timer = 0x10;
        owner->state = state_marker_place_hero;
        owner->state(owner);
    }
}

/* Marker sequence: once the timer runs out, restarts the marker sprite on animation 1 and places it
 * at hero hero340's aim point (animation mode 1). Plays sound 0x29, then
 * state_marker_reset_then_text follows after gClaimKindTable[1] frames. */
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
        struct Obj *hero = gWorld->list58[gWorld->hero340 + 6];
        /* the hero's body, at Obj + 8 */
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker = owner->sprite;
        s32 x;
        s32 y;
        s32 flags;
        s32 mask;

        /* signedTimer is 0 here */
        obj_load_record(marker, 1, -1, 0, signedTimer);
        sprite_entry_update(marker);
        /* positions are in 1/256 pixels; the screen y is y - z */
        x = pose->x;
        if (x < 0)
        {
            x += 0xFF;
        }
        marker->x = x >> 8;
        y = pose->y - pose->z;
        if (y < 0)
        {
            y += 0xFF;
        }
        y >>= 8;
        marker->y = hero->unkCC + y - hero->unkCD;
        /* MsBits mode (bits 1-2) = 1 */
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

/* Marker sequence: once the timer runs out, plays action 1 on hero hero33F's body and turns the
   marker sprite into the text graphic for the current language (anim 6), placed at the hero's
   aim point. Plays sound 0x155 and goes straight on to state_marker_reset_then_anim with a
   0x40-frame timer. */
void state_marker_text_place(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        /* the hero's body, at Obj + 8 */
        struct Owner *body = (struct Owner *)((u8 *)gWorld->list58[gWorld->hero33F + 6] + 8);
        struct Sprite *marker = owner->sprite;
        s32 keep;
        s32 x;
        s32 y;

        sprite_set_action(body, 1, 0, 0);
        ((struct MsBits *)&body->sprite->flags)->mode = 1;
        switch (gSys[SYS_LANGUAGE])
        {
        case 0:
            obj_setup_load(marker, 0x2088);
            break;
        case 1:
            obj_setup_load(marker, 0x20A2);
            break;
        case 2:
            obj_setup_load(marker, 0x209E);
            break;
        case 3:
            obj_setup_load(marker, 0x209A);
            break;
        case 4:
            obj_setup_load(marker, 0x20A6);
            break;
        }
        keep = -1;
        obj_load_record(marker, 6, keep, 0, 0);
        sprite_entry_update(marker);
        x = ((struct Pos38 *)body)->x;
        if (x < 0)
        {
            x += 0xFF;
        }
        marker->x = x >> 8;
        y = ((struct Pos38 *)body)->y - ((struct Pos38 *)body)->z;
        if (y < 0)
        {
            y += 0xFF;
        }
        y >>= 8;
        marker->y = ((struct PoseTail *)body)->unkC4 + y - ((struct PoseTail *)body)->unkC5;
        ((struct MsBits *)&marker->flags)->mode = 1;
        set_slot_value(0x155, keep);
        owner->timer = 0x40;
        owner->state = state_marker_reset_then_anim;
        owner->state(owner);
    }
}

/* Marker sequence: once the timer runs out, turns the marker sprite back into graphic 0x8004
 * (animation 0) and places it at hero hero340's aim point (animation mode 1). Plays sound 0x28,
 * then state_hero_anim_notify follows after gClaimKindTable[0] frames. */
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
        struct Obj *hero = gWorld->list58[gWorld->hero340 + 6];
        /* the hero's body, at Obj + 8 */
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker = owner->sprite;
        s32 x;
        s32 y;
        s32 flags;
        s32 mask;

        obj_setup_load(marker, 0x8004);
        /* signedTimer is 0 here */
        obj_load_record(marker, 0, -1, 0, signedTimer);
        sprite_entry_update(marker);
        /* positions are in 1/256 pixels; the screen y is y - z */
        x = pose->x;
        if (x < 0)
        {
            x += 0xFF;
        }
        marker->x = x >> 8;
        y = pose->y - pose->z;
        if (y < 0)
        {
            y += 0xFF;
        }
        y >>= 8;
        marker->y = hero->unkCC + y - hero->unkCD;
        /* MsBits mode (bits 1-2) = 1 */
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
        o8 = (struct Owner *)((u8 *)gWorld->list58[gWorld->hero340 + 6] + 8);
        sprite_set_action(o8, 3, gWorld->hero340 ? 0x2067 : 0x2038, 0);
        flags = o8->sprite->flags;
        mask = ~6;
        mask &= flags;
        mask |= 2;
        o8->sprite->flags = mask;
        owner->state = state_hero_anim_when_active;
        set_slot_value(gWorld->hero340 ? 0x102 : 0xE9, -1);
    }
}

/* Marker sequence, after state_hero_anim_notify: once hero hero340's action 3 has finished (sprite flag 8),
 * that hero goes back to its standing action (action 4, sprite 0x2001 for hero A; action 1, 0x204E for
 * hero B) and hero hero33F's body takes action 3 in animation mode 1. Sound 0x53 plays and
 * state_hero_sprite_check follows. */
void state_hero_anim_when_active(struct Owner *owner)
{
    u8 heroIndex;
    struct Owner *body;

    heroIndex = gWorld->hero340;
    body = (struct Owner *)((u8 *)gWorld->list58[heroIndex + 6] + 8);
    if ((body->sprite->flags & 8) != 0)
    {
        sprite_set_action(body, gWorld->hero340 ? 1 : 4, gWorld->hero340 ? 0x204E : 0x2001, 0);
        body = (struct Owner *)((u8 *)gWorld->list58[gWorld->hero33F + 6] + 8);
        sprite_set_action(body, 3, 0, 0);
        ((struct MsBits *)&body->sprite->flags)->mode = 1;
        owner->state = state_hero_sprite_check;
        set_slot_value(0x53, -1);
    }
}

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

        sprite_set_action((u8 *)gWorld->list58[gWorld->hero33F + 6] + 8, -1, -1, 0);
        sprite_set_action((u8 *)gWorld->list58[gWorld->hero340 + 6] + 8, -1, -1, 0);
        flags = gWorld->flags0B;
        mask = ~1;
        mask &= flags;
        gWorld->flags0B = mask;
        owner->state = state_check_slots;
    }
}

static inline struct Menu *menu_root(void)
{
    return gWorld->menu248;
}

/* Owner state: after owner->timer runs out and nothing else is busy, refreshes the 8 entries of
 * the menu (menu248), spawns the menu sprite 0x7009 (in the current language) above the screen at
 * x 0x78, starts display effect 4 (level 0x40) on every palette except the ones the heroes'
 * figures and the menu sprite use (their unk14 masks), sets the mark callbacks and waits 16
 * frames in state_wait_menu_clear. */
void state_menu_build(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;
    s32 i;
    s32 entry;
    struct Sprite *menu;
    u32 heroMask;
    u32 menuMask;
    s32 objMask;
    u32 bgMask;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else if (!is_state_busy())
    {
        i = 0;
        do
        {
            menu_entry_refresh(menu_root(), (u8)i, 0);
            entry = i + 1;
            menu_entry_refresh(menu_root(), (u8)entry, 0);
            entry = i + 2;
            menu_entry_refresh(menu_root(), (u8)entry, 0);
            entry = i + 3;
            menu_entry_refresh(menu_root(), (u8)entry, 0);
            i += 4;
            if (i > 7)
            {
                break;
            }
        } while (1);

        menu = spawn_obj_by_id(0x7009, gSys[SYS_LANGUAGE], 0, 1);
        gWorld->menu368 = menu;
        sprite_entry_update(menu);
        menu->x = 0x78;
        menu->y = 0xFFF0; /* -16: just above the screen, state_menu_slide_in brings it down */
        menu->unk0E = 0;
        ((struct MsBits *)&menu->flags)->b4 = 1;
        bgMask = 0xFFFF;
        heroMask = gWorld->fig80[0]->link->unk14;
        menuMask = menu->unk14;
        if (gWorld->fig80[1] != 0)
        {
            objMask = ~(gWorld->fig80[1]->link->unk14 | heroMask | menuMask);
        }
        else
        {
            objMask = ~(heroMask | menuMask);
        }
        draw_sprite_block(4, 0, 0x40, bgMask, (u16)objMask);
        set_mark_callbacks();
        owner->timer = 0x10;
        owner->state = state_wait_menu_clear;
    }
}

void state_menu_slide_in(struct Owner *owner)
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else
    {
        struct Sprite *menu = gWorld->menu368;
        s32 ny;
        s32 flags;
        s32 mask;

        ny = *(u16 *)&menu->y + 0x10;
        *(u16 *)&menu->y = ny;
        if ((s16)ny > 0x5F)
        {
            menu->y = 0x60;
            ((struct MsBits *)&menu->flags)->b4 = 0;
            ((struct MsBits *)&menu->flags)->mode = 1;
            owner->timer = 0x60;
            owner->state = state_menu_alloc_scene;
            set_slot_value(0x8B, -1);
        }
    }
}

/* Owner state: once owner->timer runs out, starts a 360-frame screen wipe (shape 15) centred
 * 8 pixels above the leading hero's figure (the second one when it exists and figMode2C0 bit 2
 * is set) that calls menu_init_clear when it ends, then moves on to state_menu_clear_vram. */
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
        struct World *world = gWorld;
        u32 heroIdx;
        struct Obj *hero;
        struct Pos38 *pose;
        u8 *work;
        void *task;
        s32 screenX;
        s32 screenY;
        s32 height;
        u8 second;

        heroIdx = 0;
        if (world->fig80[1] != 0)
        {
            second = world->figMode2C0 & 4;
            heroIdx = second != 0;
        }
        hero = world->fig80[heroIdx];
        pose = (struct Pos38 *)((u8 *)hero + 8);
        work = gWorkBuf;
        screenX = pose->x;
        if (screenX < 0)
        {
            screenX += 0xFF;
        }
        screenX >>= 8;
        *(s16 *)(work + WORK_WIPE_POINT) = screenX;
        height = pose->y - pose->z;
        if (height < 0)
        {
            height += 0xFF;
        }
        screenY = height >> 8;
        screenY -= 8;
        *(s16 *)(work + WORK_WIPE_POINT + 2) = screenY;
        *(s16 *)(work + WORK_WIPE_POINT + 4) = 0xF0;
        task = heap_alloc2(0x44, 0, gSceneTable0820218c, 0);
        world->scene = spawn_kind_sprite(task, 7, gSceneTable0820218c, 15, 360,
                                         gWorkBuf + WORK_WIPE_POINT, signedTimer, 0x37, menu_init_clear);
        owner->state = state_menu_clear_vram;
    }
}
