#include "global.h"
#include "ram_routines.h"
#include "rope_jump.h"

extern const u8 gClassEb10[];
extern const u8 gClassEb28[];
extern const u8 gClassEb38[];
extern const u8 gClassEb48[];
extern const u8 gClassEb58[];
extern const s8 gDirTab[];
extern const u8 gLoadRecordIds[];
extern const u8 gFrameRecordOffsets[];
extern const u8 gObjScript08216806[];
extern const u32 gPhase4ChildETable[];
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
extern void path_step_main(void);
extern void path_step_main_routine(void);
extern void path_step_loop(void);

void obj_destroy_sprites(void *obj, u32 flags);
void task_remove(void *node, u32 kind);
void task_insert(void *node, u8 kind);
void heap_free(void *ptr);
void heap_free_b(void *ptr);
u32 arc_tan2(s32 x, s32 y);
void obj_set_table_fields(void *obj, u32 table);
void obj_list_pass_0a8c(void);
void obj_load_record(void *obj, u32 record, s32 flags, u32 arg3, u32 arg4);
void obj_update_sprites(void *obj);
void obj_init_big(void *obj, u32 a, u32 b, u32 c);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_create_sprite_14(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_set_table_and_init(void *obj, u32 table);
void obj_update_anim_state(void *obj);
void grid_lines_obj_draw_grid(void *obj);
void obj_set_fields_18(void *obj, u32 a, u32 b, u32 c);
void obj_init_class_38fa(void *obj, u32 value);
void obj_set_frame_table(void *obj, const void *table);
void obj_refresh_flags(void *obj, u32 value);
void obj_release_sprites(void *obj);
void triple_copy(void *dst, const void *src);
void obj_clear_buffer(void *grid);
void obj_set_offset(void *grid, s32 x, s32 y);
void obj_child_ram_call(void *grid, s32 *args, u32 kind);
void obj_upload_tiles(void *grid);

struct AvgSample { s32 v; s32 pad[2]; };
struct AvgEntry
{
    u32 v00;
    u8 filler4[8];
    u32 v0C;
    u8 filler10[8];
    u32 v18;
    u8 filler20[8];
    u32 v24;
    u8 filler28[8];
    u32 v30;
    u8 filler34[8];
};

struct SysState
{
    u8 filler0[0x2A];
    u16 rawKeysPressed;
    u8 filler2C[0x880 - 0x2C];
    u32 oamCurrent;
    u8 filler884[0x8C0 - 0x884];
    u8 lang;
};

extern struct SysState gSys;

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

extern struct ObjLists gUnk_0203FFB8;

struct SpriteModeBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

// A group of five objects: two ends (objA, objB) on either side of a middle one (objC),
// and two followers (objD, objE) that copy the ends' positions.
struct GridGroup
{
    struct GridObj *objA;
    struct GridObj *objB;
    struct GridObj *objC;
    struct GridObj *objD;
    struct GridObj *objE;
    s32 centerX;
    s32 centerY;
    s32 centerZ;
    u8 filler20[0x2C - 0x20];
    s32 angle;
    s32 dirZ; // the ends lie at center -/+ 18 * (dirX, -dirZ)
    s32 dirX;
    s32 endAX;
    u8 filler3C[0x40 - 0x3C];
    s32 endAZ;
    s32 endBX;
    u8 filler48[0x4C - 0x48];
    s32 endBZ;
};

// What obj_flush_anim returns once the player has answered the yes/no question.
#define ANSWER_YES      1
#define ANSWER_NO       2
#define ANSWER_CANCEL   999

struct TextPos
{
    s32 x;
    s32 y;
};

extern const u8 gText[];
extern const u32 gPhase4ChildCTable[];
extern const u32 gPhase4ChildDTable[];

#define PHASE4_TABLE(off) ((const u32 *)(gText + (off)))

// One text in each of the five languages.
struct LangText
{
    u32 text[5];
};

// The current language's text of the LangText at gText + off. As a struct member the array
// is indexed with an ARRAY_REF, which loads the table's address before the language.
#define LANG_TEXT(off) (((const struct LangText *)(gText + (off)))->text[gSys.lang])

void win_draw_text(void *win, u32 text, s32 x, s32 y, u32 flag);

// Draws the current language's entry of a text table at pos.
static inline void win_draw_lang_text_pos(void *win, const u32 *table, struct TextPos pos)
{ win_draw_text(win, table[gSys.lang], pos.x, pos.y, 0); }

static inline void child_reset_flags(struct GridObj *obj)
{
    obj->flags = 0;
    obj->counter = 0;
}

static inline void phase4_set_tables_20_24(struct Phase4Obj *obj, u32 table_a, u32 table_b)
{
    obj_set_table_fields(obj->unk20, table_a);
    obj_set_table_fields(obj->unk24, table_b);
}

static inline void phase4_set_tables_28_2c(struct Phase4Obj *obj, u32 table_a, u32 table_b)
{
    obj_set_table_fields(obj->unk28, table_a);
    obj_set_table_fields(obj->unk2C, table_b);
}

static inline void phase4_child_setup(struct Phase4Sub *sub, u32 arg)
{
    u32 idx = arg;

    obj_set_table_fields(sub->unk8, gPhase4ChildETable[idx - 2]);
    idx -= 2;
    obj_load_record(sub->unkC->unk10, gLoadRecordIds[idx], -1, 0, 0);
    obj_update_sprites(sub->unkC);
    obj_set_table_fields(sub->unkC, (u32)gObjScript08216806);
}

void obj_script_step(struct GridObj *obj, u32 arg);
u8 obj_is_done(struct GridObj *obj);
void obj_list_remove_call(u32 arg, struct GridObj *obj);
void obj_place_child(struct GridObj *obj, u32 mode, u32 attr, s32 *base, u32 record);

extern const u8 gObjScript082157b6[];
extern const u8 gObjScript082157c6[];
extern const u8 gObjScript082165c0[];
extern const u8 gObjScript08215896[];
extern const u8 gObjScript08215a94[];
extern const u8 gObjScript082165b4[];
extern const u8 gObjScript082165a8[];
extern const u8 gObjScript0821567c[];
extern const u8 gObjScript08215562[];
extern const u8 gObjScript08215588[];
extern const u8 gObjScript082155ae[];
extern const u8 gObjScript082155d0[];
extern const u8 gObjScript0821549c[];
extern const u8 gObjScript082154cc[];
extern const u8 gObjScript08215742[];
extern const u8 gObjScript08215782[];
extern const u8 gObjScript08215422[];
extern const u8 gObjScript0821546c[];
extern const u8 gObjScript082156ce[];
extern const u8 gObjScript0821570e[];
extern const u8 gObjScript0821563e[];
void obj_update_grid_children(struct Phase4Obj *obj);
void set_slot_value(u32 id, s32 value);
void obj_init_pair(void *obj, s32 kind);
void init_obj_table_classes(void *obj);
u8 obj_triples_done(void *obj);
void win_draw_text(void *win, u32 text, s32 x, s32 y, u32 flag);
s32 obj_flush_anim(void *win);
void obj_release_and_flush(void *win, u32 mode);
void obj_init_triples_class(void *obj);
void obj_start_tween_len(void *obj, u32 a, u32 b);
void obj_init_single(void *obj);
u8 obj_slot0_is_empty(void *obj);
void write_var(u16 id, u16 value);
void game_var_write(s32 index, u8 value);
void update_obj_list(void);
extern u16 gUnk_03000374[];
extern const u8 gObjScript08215556[];
extern const u8 gObjScript082157d6[];
extern const u8 gObjScript08215814[];
extern const u8 gObjScript08215852[];
void obj_slide_setup(void *win, s32 x, s32 y, s32 steps);
u8 obj_slide_advance(void *win);
void obj_key_state_update(struct Phase4Obj *obj)
{
    u16 *keys = gUnk_03000374;
    s32 pos0[2];
    s32 pos1[2];

    obj_update_grid_children(obj);
    pos0[0] = ((s16 *)obj->unk28->unk10)[0];
    pos0[1] = ((s16 *)obj->unk28->unk10)[1] - 0x20;
    pos1[0] = ((s16 *)obj->unk2C->unk10)[0];
    pos1[1] = ((s16 *)obj->unk2C->unk10)[1] - 0x10;
    switch (obj->state)
    {
    case 0:
        if (!obj_is_done(obj->unk28))
            break;
        obj->state = 1;
        obj->flags &= ~0x12;
        break;
    case 1:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C0E0), pos0[0], pos0[1], 1);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->flags &= ~1;
        obj->state = 2;
        break;
    case 2:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_slide_setup(obj->win48, 0, 0, 10);
        obj->state = 3;
        break;
    case 3:
        if (!obj_slide_advance(obj->win48))
            break;
        if (keys[1] & 1)
        {
            obj_set_table_fields(obj->unk20, (u32)gObjScript08215556);
            obj->state = 4;
        }
        else if (keys[1] & 2)
        {
            obj_set_table_fields(obj->unk24, (u32)gObjScript08215556);
            obj->state = 5;
        }
        break;
    case 4:
        if (!obj_is_done(obj->unk20))
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C0F4), pos1[0], pos1[1], 1);
        obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
        obj->state = 6;
        break;
    case 5:
        if (!obj_is_done(obj->unk24))
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C108), pos1[0], pos1[1], 2);
        obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
        obj->state = 1;
        break;
    case 6:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C11C), pos0[0], pos0[1], 2);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->state = 7;
        break;
    case 7:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_slide_setup(obj->win48, 0xF0, 0, 10);
        obj->state = 8;
        break;
    case 8:
        if (!obj_slide_advance(obj->win48))
            break;
        if (keys[1] & 2)
        {
            obj_set_table_fields(obj->unk24, (u32)gObjScript08215556);
            obj->state = 9;
        }
        else if (keys[1] & 1)
        {
            obj_set_table_fields(obj->unk20, (u32)gObjScript08215556);
            obj->state = 10;
        }
        break;
    case 9:
        if (!obj_is_done(obj->unk24))
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C0F4), pos1[0], pos1[1], 2);
        obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
        obj->state = 11;
        break;
    case 10:
        if (!obj_is_done(obj->unk20))
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C130), pos1[0], pos1[1], 1);
        obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
        obj->state = 6;
        break;
    case 11:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C144), pos0[0], pos0[1], 1);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->state = 12;
        break;
    case 12:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_release_and_flush(obj->win48, 0);
        phase4_set_tables_28_2c(obj, (u32)gObjScript082157d6, (u32)gObjScript082157d6);
        obj->flags |= 0xD;
        obj->state = 13;
        break;
    case 13:
        if (!obj_is_done(obj->unk28))
            break;
        obj->flags &= ~5;
        if (obj->unk20->counter != 0)
        {
            win_draw_text(obj->win48, LANG_TEXT(0x11C180), pos1[0], pos1[1], 1);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 12;
        }
        else
        {
            win_draw_text(obj->win48, LANG_TEXT(0x11C16C), pos1[0], pos1[1], 1);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 14;
        }
        child_reset_flags(obj->unk20);
        break;
    case 14:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C158), pos0[0], pos0[1], 2);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->state = 15;
        break;
    case 15:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_release_and_flush(obj->win48, 0);
        phase4_set_tables_28_2c(obj, (u32)gObjScript08215814, (u32)gObjScript08215814);
        obj->flags |= 5;
        obj->state = 16;
        break;
    case 16:
        if (!obj_is_done(obj->unk28))
            break;
        obj->flags &= ~5;
        if (obj->unk24->counter != 0)
        {
            win_draw_text(obj->win48, LANG_TEXT(0x11C180), pos1[0], pos1[1], 2);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 15;
        }
        else
        {
            win_draw_text(obj->win48, LANG_TEXT(0x11C16C), pos1[0], pos1[1], 2);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 17;
        }
        child_reset_flags(obj->unk24);
        break;
    case 17:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C194), pos0[0], pos0[1], 0);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->state = 18;
        break;
    case 18:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_release_and_flush(obj->win48, 0);
        phase4_set_tables_28_2c(obj, (u32)gObjScript08215852, (u32)gObjScript08215852);
        obj->flags |= 5;
        obj->state = 19;
        break;
    case 19:
        if (!obj_is_done(obj->unk28))
            break;
        obj->flags &= ~0xD;
        if (obj->unk20->counter != 0 || obj->unk24->counter != 0)
        {
            win_draw_text(obj->win48, LANG_TEXT(0x11C180), pos1[0], pos1[1], 0);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 18;
        }
        else
        {
            win_draw_text(obj->win48, LANG_TEXT(0x11C16C), pos1[0], pos1[1], 0);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 20;
        }
        child_reset_flags(obj->unk20);
        child_reset_flags(obj->unk24);
        break;
    case 20:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_text(obj->win48, LANG_TEXT(0x11C1A8), pos0[0], pos0[1], 0);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->state = 21;
        break;
    case 21:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_release_and_flush(obj->win48, 0);
        obj_start_tween_len(obj->unk3C, 0x800, 0x2000);
        obj->flags = (obj->flags | 0x11) & ~2;
        phase4_set_tables_20_24(obj, (u32)gObjScript0821549c, (u32)gObjScript082154cc);
        phase4_set_tables_28_2c(obj, (u32)gObjScript08215742, (u32)gObjScript08215782);
        obj->state = 22;
        break;
    case 22:
        if (!obj_is_done(obj->unk28))
            break;
        obj->unk54 = 2;
        obj->state = 0;
        break;
    }
    update_obj_list();
}

// Phase 4 of the jump-rope minigame, one frame: plays the chosen level (unk60) until the
// brothers clear it or one of them misses three times, then shows the result or asks whether
// to try again, and goes back to the menu (phase 2).
void obj_phase4_step(struct Phase4Obj *obj)
{
    struct TextPos aboveTurnerA;
    struct TextPos aboveTurnerB;

    aboveTurnerA.x = ((s16 *)obj->unk28->unk10)[0];
    aboveTurnerA.y = ((s16 *)obj->unk28->unk10)[1] - 0x18;
    aboveTurnerB.x = ((s16 *)obj->unk2C->unk10)[0];
    aboveTurnerB.y = ((s16 *)obj->unk2C->unk10)[1] - 0x10;
    obj_update_grid_children(obj);

    // A third miss by either brother stops the rope.
    if (!(obj->flags & ROPE_JUMP_NO_HITS)
        && (obj->unk20->counter > ROPE_JUMP_MAX_MISSES || obj->unk24->counter > ROPE_JUMP_MAX_MISSES))
    {
        set_slot_value(0x76, -1);
        phase4_set_tables_28_2c(obj, (u32)gObjScript082157b6, (u32)gObjScript082157c6);
        obj_set_table_fields(obj->sub30->unk4, (u32)gObjScript082165c0);
        obj->flags = (obj->flags & ~ROPE_JUMP_JUMP_INPUT) | ROPE_JUMP_NO_HITS;
        obj->state = 3;
    }

    switch (obj->state)
    {
    case 0: // turners ready: show the level banner and the miss marks
        if (!obj_is_done(obj->unk28))
            break;
        obj->flags &= ~ROPE_JUMP_NO_HITS;
        obj_init_pair(obj->unk38, obj->unk60);
        init_obj_table_classes(obj->unk34);
        obj->state = 1;
        break;
    case 1: // start swinging the level's rope pattern
        if (!obj_triples_done(obj->unk34))
            break;
        if (obj->unk60 == ROPE_JUMP_LEVELS)
        {
            phase4_set_tables_28_2c(obj, (u32)gObjScript08215896, (u32)gObjScript08215a94);
            obj_set_table_fields(obj->sub30->unk4, (u32)gObjScript082165b4);
            obj->unk64 = 0;
        }
        else
        {
            phase4_set_tables_28_2c(obj, gPhase4ChildCTable[obj->unk60 - 1], gPhase4ChildDTable[obj->unk60 - 1]);
            obj_set_table_fields(obj->sub30->unk4, (u32)gObjScript082165a8);
        }
        obj->flags |= ROPE_JUMP_JUMP_INPUT | ROPE_JUMP_FACE_ROPE;
        obj->state = 2;
        break;
    case 2: // pattern over: level 5 goes on to its next pattern, otherwise the level is cleared
        if (!obj_is_done(obj->unk28))
            break;
        if (obj->unk60 == ROPE_JUMP_LEVELS)
        {
            if (++obj->unk64 >= ROPE_JUMP_FINAL_PATTERNS)
            {
                win_draw_lang_text_pos(obj->win48, PHASE4_TABLE(0x11C2D4), aboveTurnerA);
                obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
                obj->flags &= ~(ROPE_JUMP_JUMP_INPUT | ROPE_JUMP_FACE_ROPE);
                obj->state = 9;
            }
            else
            {
                phase4_set_tables_28_2c(obj, gPhase4ChildCTable[obj->unk64], gPhase4ChildDTable[obj->unk64]);
                obj->state = 2;
            }
        }
        else
        {
            s32 cleared;

            win_draw_lang_text_pos(obj->win48, PHASE4_TABLE(0x11C2D4), aboveTurnerA);
            obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
            cleared = obj->unk5C + 1;
            if (cleared > 1)
            {
                if (cleared > ROPE_JUMP_LEVELS)
                    cleared = ROPE_JUMP_LEVELS;
            }
            else
            {
                cleared = 1;
            }
            obj->unk5C = cleared;
            obj->flags &= ~(ROPE_JUMP_JUMP_INPUT | ROPE_JUMP_FACE_ROPE);
            obj->state = 9;
        }
        break;
    case 3: // third miss: turner B comments, then the misses are reset
        if (!obj_is_done(obj->unk28))
            break;
        win_draw_lang_text_pos(obj->win48, PHASE4_TABLE(0x11C1BC), aboveTurnerB);
        obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
        child_reset_flags(obj->unk20);
        child_reset_flags(obj->unk24);
        obj->state = 4;
        break;
    case 4: // turner A asks whether to try again
        if (obj_flush_anim(obj->win48) != 0)
            break;
        win_draw_lang_text_pos(obj->win48, PHASE4_TABLE(0x11C89C), aboveTurnerA);
        obj_set_table_fields(obj->unk28, (u32)gObjScript0821567c);
        obj->state = 5;
        break;
    case 5: // the answer
        switch (obj_flush_anim(obj->win48))
        {
        case ANSWER_YES:
            obj_release_and_flush(obj->win48, 0);
            obj_init_triples_class(obj->unk34);
            phase4_set_tables_20_24(obj, (u32)gObjScript08215562, (u32)gObjScript08215588);
            obj->state = 6;
            break;
        case ANSWER_NO:
        case ANSWER_CANCEL:
            obj_release_and_flush(obj->win48, 0);
            obj_init_triples_class(obj->unk34);
            phase4_set_tables_20_24(obj, (u32)gObjScript082155ae, (u32)gObjScript082155d0);
            obj->state = 7;
            break;
        }
        break;
    case 6: // once the brothers are in place, the level starts over (unless the phase changed)
        if (!obj_is_done(obj->unk20))
            break;
        obj->state = 0;
        break;
    case 7: // gave up: everyone walks back and the view scrolls
        if (!obj_is_done(obj->unk20))
            break;
        if (obj->flags & ROPE_JUMP_ALT_ENDING)
        {
            phase4_set_tables_20_24(obj, (u32)gObjScript0821549c, (u32)gObjScript082154cc);
            phase4_set_tables_28_2c(obj, (u32)gObjScript08215742, (u32)gObjScript08215782);
        }
        else
        {
            phase4_set_tables_20_24(obj, (u32)gObjScript08215422, (u32)gObjScript0821546c);
            phase4_set_tables_28_2c(obj, (u32)gObjScript082156ce, (u32)gObjScript0821570e);
        }
        obj_start_tween_len(obj->unk3C, 0x800, 0x2000);
        obj->flags &= ~ROPE_JUMP_FACE_ROPE;
        obj->state = 8;
        break;
    case 8: // turner B's parting words, then back to the menu
        if (!obj_is_done(obj->unk28))
            break;
        win_draw_lang_text_pos(obj->win48, PHASE4_TABLE(0x11C1F8), aboveTurnerB);
        obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
        obj->flags &= ~ROPE_JUMP_ROPE_FOLLOWS;
        obj->unk54 = 2;
        obj->state = 6;
        break;
    case 9: // cleared: show the clear banner
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_release_and_flush(obj->win48, 0);
        obj_init_single(obj->unk38);
        obj_start_tween_len(obj->unk3C, 0x800, 0);
        obj->state = 10;
        break;
    case 10: // score the try, then save the level and the score
    {
        s32 score;

        if (!obj_slot0_is_empty(obj->unk38))
            break;
        phase4_set_tables_20_24(obj, (u32)gObjScript0821563e, (u32)gObjScript0821563e);
        score = 6 - obj->unk24->counter;
        score -= obj->unk20->counter;
        obj->unk68 = score;
        phase4_child_setup(obj->sub30, obj->unk68);
        write_var(0x1CD8, obj->unk60);
        game_var_write(0x11, obj->unk68);
        obj->state = 11;
        break;
    }
    case 11: // everyone walks back while the rope swings on
        if (!obj_is_done(obj->sub30->unk8))
            break;
        obj_start_tween_len(obj->unk3C, 0x800, 0x2000);
        obj->flags |= ROPE_JUMP_ROPE_FOLLOWS | ROPE_JUMP_NO_HITS;
        phase4_set_tables_20_24(obj, (u32)gObjScript08215422, (u32)gObjScript0821546c);
        phase4_set_tables_28_2c(obj, (u32)gObjScript082156ce, (u32)gObjScript0821570e);
        obj->state = 12;
        break;
    case 12: // back to the menu, after one more text from turner B if flagged
        if (!obj_is_done(obj->unk28))
            break;
        if (obj->flags & ROPE_JUMP_ALT_ENDING)
        {
            win_draw_lang_text_pos(obj->win48, PHASE4_TABLE(0x11C2FC), aboveTurnerB);
            obj_set_table_fields(obj->unk2C, (u32)gObjScript0821567c);
            obj->state = 13;
        }
        else
        {
            obj->unk54 = 2;
            obj->state = 6;
        }
        obj->flags &= ~ROPE_JUMP_ALT_ENDING;
        break;
    case 13:
        if (obj_flush_anim(obj->win48) != 0)
            break;
        obj_release_and_flush(obj->win48, 0);
        obj->unk54 = 2;
        obj->state = 6;
        break;
    }
    update_obj_list();
}

ASM_FUNC("asm/code_08167E7C/grid_lines_obj_draw_grid.s", void grid_lines_obj_draw_grid(void *obj))

void obj_frame_update(struct GridObj *obj)
{
    struct GridObj *child = obj->unk14;
    u32 state;
    u32 (**fnA)();

    obj_list_pass_0a8c();
    fnA = &gUnk_03000D48;
    state = gUnk_0203FFB8.root;
    (*fnA)((void *)state, (void *)(state + 0xA14));
    gSys.oamCurrent = gUnk_03000D4C((void *)gUnk_0203FFB8.root, gSys.oamCurrent);
    grid_lines_obj_draw_grid(child);
}

void obj_set_dir_to_point(u32 unused0, struct GridObj *obj, s32 x, s32 y)
{
    s32 dx = (s32)((u32)((s32)obj->unk18 - x) << 8) >> 16;
    s32 dy = (s32)((u32)((s32)obj->unk20 - y) << 8) >> 16;
    u32 angle = (u16)arc_tan2(dx, dy);
    u32 sector = (angle + 0x1000) >> 13;
    obj->dir52 = gDirTab[sector];
}

// Matches except for the routine offset: the original subtracts two different symbols at the same
// ROM address (config/symbols.txt has only path_step_main there). With one symbol the term folds to 0.
void obj_copy_end_points(struct GridObj *obj)
{
    struct GridObj *a;
    struct GridObj *b;
    u32 *table;
    u32 ax;
    u32 ay1;
    u32 ay;
    u32 bx;
    u32 by1;
    u32 by;
    u32 k;

    if ((*(u32 *)&obj->flags & 1) != 0)
    {
        a = obj->unk28;
        ax = (u32)a->unk18;
        ay1 = (u32)a->unk1C + (u32)a->anchor3C;
        ay = (u32)a->unk20;
        table = obj->points40;
        k = 0xFFFFF800;
        ay1 = ay1 + k;
        table[0] = ax;
        table[1] = ay1;
        table[2] = ay;
        b = obj->unk2C;
        bx = (u32)b->unk18;
        by1 = (u32)b->unk1C + (u32)b->anchor3C;
        by = (u32)b->unk20;
        by1 = by1 + k;
        table[42] = bx;
        table[43] = by1;
        table[44] = by;
        ((void (*)(void *))(*(u32 *)((u8 *)table + 0x234) + ((u32)path_step_main_routine - (u32)path_step_main)))(table);
    }
}

void obj_set_mode_if_flag8(struct GridObj *obj)
{
    if (gSys.rawKeysPressed & 8)
    {
        *(u32 *)&obj->unk54 = 4;
    }
}

void obj_set_child_tables_28_2c(struct GridObj *obj, u32 table_a, u32 table_b)
{
    obj_set_table_fields(obj->unk28, table_a);
    obj_set_table_fields(obj->unk2C, table_b);
}

void obj_set_child_tables(struct GridObj *obj, u32 table_a, u32 table_b)
{
    obj_set_table_fields(obj->unk20, table_a);
    obj_set_table_fields(obj->unk24, table_b);
}

void obj_phase4_child_setup(struct GridObj *obj, u32 arg)
{
    u32 idx = arg;
    void *sub = obj->unk8;

    obj_set_table_fields(sub, gPhase4ChildETable[idx - 2]);
    idx -= 2;
    obj_load_record(((struct GridObj *)obj->cls)->unk10, gLoadRecordIds[idx], -1, 0, 0);
    obj_update_sprites(obj->cls);
    obj_set_table_fields(obj->cls, (u32)gObjScript08216806);
}

void obj_class_eb28_free(struct GridObj *obj, u32 free_flag)
{
    obj->unk10 = (void *)gClassEb28;
    if (free_flag & 1)
    {
        heap_free_b(obj);
    }
}

void obj_class_eb38_destroy(struct GridObj *obj, u32 flags)
{
    obj->cls = (void *)gClassEb38;
    obj_destroy_sprites(obj, flags);
}

struct GridObj *init_vt_08cdeb38(struct GridObj *obj, u32 table, u32 a2, u32 a3)
{
    obj_init_big(obj, 1, a2, a3);
    obj->cls = (void *)gClassEb38;
    obj_create_sprite_10(obj, 4, 0x5001, 0, 0, 3);
    obj_create_sprite_14(obj, 0, 0x1000, 0, 0, 3);
    obj_set_table_and_init(obj, table);
    obj_update_anim_state(obj);
    return obj;
}

void obj_reset_flags(struct GridObj *obj)
{
    obj->flags = 0;
    obj->counter = 0;
}

void obj_class_eb48_destroy(struct GridObj *obj, u32 flags)
{
    obj->cls = (void *)gClassEb48;
    obj_destroy_sprites(obj, flags);
}

struct GridObj *init_vt_08cdeb48(struct GridObj *obj, u32 a1, u32 a2, u32 a3)
{
    obj_init_big(obj, 0, a2, a3);
    obj->cls = (void *)gClassEb48;
    obj_reset_flags(obj);
    obj_create_sprite_10(obj, 0, a1, 1, 0, 2);
    obj_create_sprite_14(obj, 0, 0x1000, 0, 0, 3);
    obj_set_table_and_init(obj, a2);
    obj_set_frame_table(obj, gFrameRecordOffsets);
    obj_refresh_flags(obj, 0);
    return obj;
}

void obj_step_remove_when_done(struct GridObj *obj, u32 arg)
{
    obj_script_step(obj, arg);
    if (((struct GridSprite *)obj->unk10)->attr12 & 0x10)
    {
        if (obj_is_done(obj))
        {
            obj_list_remove_call(arg, obj);
        }
    }
}

void obj_class_eb58_destroy(struct GridObj *obj, u32 flags)
{
    obj->cls = (void *)gClassEb58;
    obj_destroy_sprites(obj, flags);
}

struct GridObj *init_vt_08cdeb58(struct GridObj *obj, u32 a1, u32 a2, u32 a3, u32 a4)
{
    obj_init_big(obj, 2, a2, a3);
    obj->cls = (void *)gClassEb58;
    obj_create_sprite_10(obj, 2, 0x8004, 0, a1, 1);
    ((struct SpriteModeBits *)&((struct GridSprite *)obj->unk10)->attr12)->mode = 1;
    obj_set_fields_18(obj, a2, a3, a4);
    obj_init_class_38fa(obj, 0x1E);
    return obj;
}

s32 avg_fields_sdiv15_a(void *obj)
{
    s32 sum = 0;
    struct AvgSample *e = (struct AvgSample *)((u8 *)obj + 8);
    s32 i;

    for (i = 0; i < 15; i++)
        sum += e[i].v;
    return sum / 15;
}

s32 avg_fields_sdiv15_b(void *obj)
{
    s32 sum = 0;
    struct AvgSample *e = (struct AvgSample *)obj;
    s32 i;

    for (i = 0; i < 15; i++)
        sum += e[i].v;
    return sum / 15;
}

u32 path_step_loop_call(struct GridObj *obj, u32 a1, u32 a2, u32 a3, u32 a4)
{
    u32 args[3];
    u32 r;

    args[0] = a1;
    args[1] = a2;
    args[2] = a3;
    r = ((u32 (*)(void *, u32 *, u32))(*(u32 *)((u8 *)obj + 0x234) + ((u32)path_step_loop - (u32)path_step_main)))(obj, args, a4 * a4);
    return (-r | r) >> 31;
}

void path_step_main_call(struct GridObj *obj)
{
    ((void (*)(void *))(*(u32 *)((u8 *)obj + 0x234) + ((u32)path_step_main_routine - (u32)path_step_main)))(obj);
}

void path_free(struct GridObj *obj, u32 free_flag)
{
    heap_free(obj->path230);
    if (free_flag & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_08167E7C/path_init.s", void path_init(void))

void obj_remove_task_eb10(struct GridObj *obj, u32 flags)
{
    obj->unk18 = (void *)gClassEb10;
    task_remove(obj, flags);
}

struct GridObj *init_vt_08cdeb10(struct GridObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = (void *)gClassEb10;
    obj->unk20 = 0;
    obj->unk1C = 0;
    return obj;
}

/* Places one child of an object group: at the group's center (x, y, z) moved by the child's
   (-offX, +offY) times its scale, with its sprite turned to angle (a byte, stored as angle << 8).
   While the child's counter runs down it plays animation anim + 1 (mode 4) and blinks, hidden
   every other 2 frames, and is shown again once the counter ends. Otherwise, when mode differs
   from mode58, it plays animation anim (in mode 0 with bit 4 of the sprite's attr12 set). */
void obj_place_child(struct GridObj *obj, u32 mode, u32 angle, s32 *center, u32 anim)
{
    s32 scale = obj->scale;
    s32 left;
    u32 angle_byte;

    obj->unk18 = (void *)(center[0] - scale * obj->offX);
    obj->unk20 = (void *)(center[2] + scale * obj->offY);
    angle_byte = angle & 0xFF;
    ((struct GridSprite *)obj->unk10)->word0C = angle_byte << 8;
    if ((s16)obj->counter != 0)
    {
        if (obj->mode56 != 4)
        {
            obj->mode56 = 4;
            obj_load_record(obj->unk10, anim + 1, 0, 0, 0);
        }
        // blink: hidden while bit 1 of the counter is set
        if (((s32)((u32)obj->counter << 16) >> 17) & 1)
        {
            obj_release_sprites(obj);
        }
        else
        {
            obj_update_sprites(obj);
        }
        left = obj->counter - 1;
        obj->counter = left;
        if ((s32)((u32)left << 16) <= 0)
        {
            obj->counter = 0;
            obj_update_sprites(obj);
        }
    }
    else if (mode != obj->mode58)
    {
        obj_load_record(obj->unk10, anim, 0, 0, 0);
        obj->mode56 = mode;
        if (mode == 0)
        {
            u8 attr = ((struct GridSprite *)obj->unk10)->attr12;

            ((struct GridSprite *)obj->unk10)->attr12 = attr | 0x10;
        }
    }
}

ASM_FUNC("asm/code_08167E7C/create_obj_group_main.s", void create_obj_group_main(void))

/* Lays out a five-object group around its center: places the two ends (objA, objB) with
 * obj_place_child, moves each follower (objD, objE) to its end and shows it only while the
 * end's flag bit 0 is set, turns the middle object (objC) to the group's angle, and stores
 * the two end points at center -/+ 18 * (dirX, -dirZ). */
void obj_place_pair(struct GridGroup *group, u32 mode)
{
    u32 visibleBit = 1;
    s32 *center = &group->centerX;
    s32 side;
    s32 follower;
    s32 offX;
    s32 offZ;
    s32 centerX;
    s32 centerZ;

    for (side = 0; side < 2; side++)
    {
        /* computed apart from side so the compiler doesn't derive one address from the other */
        follower = (side & 1) + 3;
        obj_place_child((&group->objA)[side], mode, group->angle + 0x80, &group->centerX, side * 2 + 1);
        triple_copy(&(&group->objA)[follower]->unk18, &(&group->objA)[side]->unk18);
        if (((&group->objA)[side]->flags & visibleBit) != 0)
            obj_update_sprites((&group->objA)[follower]);
        else
            obj_release_sprites((&group->objA)[follower]);
    }
    triple_copy(&group->objC->unk18, center);
    ((struct GridSprite *)group->objC->unk10)->word0C = group->angle << 8;
    offX = group->dirX * 18;
    centerX = group->centerX;
    group->endAX = centerX - offX;
    offZ = group->dirZ * 18;
    centerZ = group->centerZ;
    group->endAZ = centerZ + offZ;
    group->endBX = centerX + offX;
    group->endBZ = centerZ - offZ;
}
