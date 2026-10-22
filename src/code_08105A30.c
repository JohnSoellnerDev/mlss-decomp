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
    s8 unkC4;
    s8 unkC5;
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
    u16 unkF8;
    u8 filler0FA[0xFC - 0xFA];
    u16 unkFC;
    u16 unkFE;
    u8 filler100[0x102 - 0x100];
    u16 unk102;
    u16 unk104;
    u16 unk106;
    u16 unk108;
    u8 filler10A[0x111 - 0x10A];
    u8 unk111;
    u8 filler112[0x12B - 0x112];
    u8 unk12B;
    u8 unk12C;
    u8 unk12D;
};

struct CurObj
{
    u8 filler0[0xEC];
    s16 unkEC;
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

void state_hero_select_confirm(struct Owner *owner)
{
    u8 *hero;
    struct Obj *obj;
    struct Entry *task;
    s32 idx;

    if ((((struct SysKeys3E *)gSys)->keys ^ 1) & 1)
    {
        return;
    }
    idx = owner->posX;
    hero = gParty + idx * 0x3C;
    recalc_char_stats(idx);
    obj = owner->target;
    obj->unkF5 = hero[0x30];
    state_assign_figure(owner);
    if (owner->state != state_apply_hero_stats)
    {
        if (owner->target != obj)
        {
            task = list_add_task(&gWorld->listHead, state_move_to_target);
            task->unkC = obj;
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

void state_init_map_tiles(struct Owner *owner)
{
    copy_map_rect_add(gWorld->mapSrc250, 1, 0, 0, 0x1E, 2,
                      (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[0x27] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x8000);
    copy_map_rect_add(gWorld->mapSrc250, 1, 0, 0x12, 0x1E, 2,
                      (u8 *)(*(u32 *)(((u32)gGlyphIndexTable[0x27] << 2) + (u32)gMapTable) + (u32)gMapTable), 0x8000);
    owner->posX = 0;
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

void state_move_to_target(struct Owner *owner)
{
    struct Obj *obj;
    s8 *flags;
    u8 *ufl;
    s16 h;
    s16 x;
    s16 y;

    obj = owner->target;
    if (move_step(&h, &x, &y, obj->unkD8 - obj->pos18, obj->unkDC - obj->pos1C, obj->unkE0 - obj->pos20, 0x400) > 0x480)
    {
        obj->pos18 += h;
        obj->pos1C += x;
        obj->pos20 += y;
    }
    else
    {
        flags = (s8 *)&obj->link->unk1F;
        *flags = (*flags & -4) | 2;
        flags = (s8 *)&obj->link->unk1F;
        *flags = (*flags & -0xD) | 8;
        flags = (s8 *)&obj->link->unk1F;
        *flags = (*flags & -0x31) | 0x20;
        ufl = &obj->link->unk1F;
        *ufl = (*ufl & 0x3F) | 0x80;
        obj->pos18 = obj->unkD8;
        obj->pos1C = obj->unkDC;
        obj->pos20 = obj->unkE0;
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
void state_apply_hero_stats(struct Owner *owner)
{
    u8 *rec;
    u8 *tbl;
    struct Obj *obj;
    u16 sVar2;
    u16 v;

    obj = gWorld->fig80[0];
    rec = gParty - ((struct CurObj *)obj)->unkEC * 0x3C;
    tbl = rec - 0x3C;
    ((struct MsBits *)&obj->unk7E)->mode = 1;
    obj->unkF5 = rec[-0xC];
    v = *(u16 *)(tbl + 0xE);
    sVar2 = 1;
    if (v != 0)
    {
        sVar2 = v;
    }
    obj->unkF6 = sVar2;
    obj->unkF8 = *(u16 *)(tbl + 0x12);
    obj->unk106 = *(u16 *)(tbl + 0x20);
    obj->unk102 = *(u16 *)(tbl + 0x1C);
    obj->unk104 = *(u16 *)(tbl + 0x24);
    obj->unk108 = *(u16 *)(tbl + 0x28);
    obj->unkFC = *(u16 *)(tbl + 0x14);
    obj->unkFE = *(u16 *)(tbl + 0x18);
    obj->unk12B = rec[-7] + 1;
    obj->unk12C = rec[-5] + 1;
    obj->unk12D = rec[-4] + 1;

    obj = gWorld->fig80[1];
    if (obj != 0)
    {
        rec = gParty - ((struct CurObj *)obj)->unkEC * 0x3C;
        tbl = rec - 0x3C;
        ((struct MsBits *)&obj->unk7E)->mode = 1;
        obj->unkF5 = rec[-0xC];
        v = *(u16 *)(tbl + 0xE);
        sVar2 = 1;
        if (v != 0)
        {
            sVar2 = v;
        }
        obj->unkF6 = sVar2;
        obj->unkF8 = *(u16 *)(tbl + 0x12);
        obj->unk106 = *(u16 *)(tbl + 0x20);
        obj->unk102 = *(u16 *)(tbl + 0x1C);
        obj->unk104 = *(u16 *)(tbl + 0x24);
        obj->unk108 = *(u16 *)(tbl + 0x28);
        obj->unkFC = *(u16 *)(tbl + 0x14);
        obj->unkFE = *(u16 *)(tbl + 0x18);
        obj->unk12B = rec[-7] + 1;
        obj->unk12C = rec[-5] + 1;
        obj->unk12D = rec[-4] + 1;
    }
    set_cur_flags_pending(0x10);
    owner->timer = 1;
    owner->state = state_menu_open_wait;
}

void state_menu_open_wait(struct Owner *owner)
{
    s32 idx;
    struct Sprite *entry;
    u16 v;

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
            idx = ~((struct CurObj *)gWorld->fig80[0])->unkEC;
            entry = (struct Sprite *)(gParty + idx * 0x3C);
            entry->unk0E = gWorld->list58[idx + 6]->unkF6;
            entry->unk14 = gWorld->list58[idx + 6]->unkFC;
            if (entry->unk0E == 0)
            {
                entry->unk0E = 1;
            }
            if (gWorld->fig80[1] != 0)
            {
                idx = ~((struct CurObj *)gWorld->fig80[1])->unkEC;
                entry = (struct Sprite *)(gParty + idx * 0x3C);
                entry->unk0E = gWorld->list58[idx + 6]->unkF6;
                entry->unk14 = gWorld->list58[idx + 6]->unkFC;
                if (entry->unk0E == 0)
                {
                    entry->unk0E = 1;
                }
            }
            serial_link_disable();
            owner->state = 0;
        }
    }
}

ASM_FUNC("asm/code_08105A30/set_text_ptrs.s", void set_text_ptrs(void))

u32 next_text_id(void)
{
    u32 text;
    u8 step;

    do
    {
        step = gWorld->state3A4;
        text = gWorld->unk398[step];
        if (step == 2)
        {
            gWorld->state3A4 = 0;
        }
        else
        {
            gWorld->state3A4 = step + 1;
        }
    } while (text == 0);

    if (gWorld->state3A4 == 2)
    {
        if (gWorld->unk33E <= 1)
        {
            if (text == gLangTextA[gSys[0x8C0]])
            {
                text = gLangTextB[gSys[0x8C0]];
            }
        }
        if (gWorld->curObj->unkEC == -2)
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

u8 world_list_idle(void)
{
    struct Obj **list;
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
    i = 0;
    list = &gWorld->list58[0];
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
        struct Obj *hero = gWorld->list58[gWorld->hero33F + 6];
        struct Pos38 *pose = (struct Pos38 *)((u8 *)hero + 8);
        struct Sprite *marker;
        s32 vx;
        s32 vy;
        s32 t;
        s32 flags;
        s32 mask;

        sprite_set_action((u8 *)hero + 8, 0, gWorld->hero33F ? 0x2067 : 0x2038, 0);
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
        struct Owner *pose = (struct Owner *)((u8 *)gWorld->list58[gWorld->hero33F + 6] + 8);
        struct Sprite *marker = owner->sprite;
        s32 noValue;
        s32 vx;
        s32 vy;

        sprite_set_action(pose, 1, 0, 0);
        ((struct MsBits *)&pose->sprite->flags)->mode = 1;
        switch (gSys[0x8C0])
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
        noValue = -1;
        obj_load_record(marker, 6, noValue, 0, 0);
        sprite_entry_update(marker);
        vx = ((struct Pos38 *)pose)->x;
        if (vx < 0)
        {
            vx += 0xFF;
        }
        marker->x = vx >> 8;
        vy = ((struct Pos38 *)pose)->y - ((struct Pos38 *)pose)->z;
        if (vy < 0)
        {
            vy += 0xFF;
        }
        vy >>= 8;
        marker->y = ((struct PoseTail *)pose)->unkC4 + vy - ((struct PoseTail *)pose)->unkC5;
        ((struct MsBits *)&marker->flags)->mode = 1;
        set_slot_value(0x155, noValue);
        owner->timer = 0x40;
        owner->state = state_marker_reset_then_anim;
        owner->state(owner);
    }
}

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

void state_hero_anim_when_active(struct Owner *owner)
{
    u8 idx;
    struct Owner *pose;

    idx = gWorld->hero340;
    pose = (struct Owner *)((u8 *)gWorld->list58[idx + 6] + 8);
    if ((pose->sprite->flags & 8) != 0)
    {
        sprite_set_action(pose, gWorld->hero340 ? 1 : 4, gWorld->hero340 ? 0x204E : 0x2001, 0);
        pose = (struct Owner *)((u8 *)gWorld->list58[gWorld->hero33F + 6] + 8);
        sprite_set_action(pose, 3, 0, 0);
        ((struct MsBits *)&pose->sprite->flags)->mode = 1;
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

NONMATCH("asm/code_08105A30/state_menu_build.s", void state_menu_build(struct Owner *owner))
{
    u16 timer = owner->timer;
    s32 signedTimer = owner->timer;
    s32 entry;
    s32 idx;
    struct Sprite *obj;
    u32 a;
    u32 b;
    s32 value;

    if (signedTimer != 0)
    {
        owner->timer = timer - 1;
    }
    else if (!is_state_busy())
    {
        entry = 0;
        do
        {
            menu_entry_refresh(menu_root(), (u8)entry, 0);
            idx = entry + 1;
            menu_entry_refresh(menu_root(), (u8)idx, 0);
            idx = entry + 2;
            menu_entry_refresh(menu_root(), (u8)idx, 0);
            idx = entry + 3;
            menu_entry_refresh(menu_root(), (u8)idx, 0);
            entry += 4;
            if (entry > 7)
            {
                break;
            }
        } while (1);

        obj = spawn_obj_by_id(0x7009, gSys[0x8C0], 0, 1);
        gWorld->menu368 = obj;
        sprite_entry_update(obj);
        obj->x = 0x78;
        obj->y = 0xFFF0;
        obj->unk0E = 0;
        ((struct MsBits *)&obj->flags)->b4 = 1;
        a = gWorld->fig80[0]->link->unk14;
        b = obj->unk14;
        if (gWorld->fig80[1] != 0)
        {
            value = ~(gWorld->fig80[1]->link->unk14 | a | b);
        }
        else
        {
            value = ~(a | b);
        }
        draw_sprite_block(4, 0, 0x40, 0xFFFF, (u16)value);
        set_mark_callbacks();
        owner->timer = 0x10;
        owner->state = state_wait_menu_clear;
    }
}
END_NONMATCH

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
