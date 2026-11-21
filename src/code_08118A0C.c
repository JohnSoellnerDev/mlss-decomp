#include "global.h"

struct WindowObj;

struct WindowChild
{
    u8 filler0[0x14];
    struct WindowObj *unk14;
    const void *cls18;
};

struct WindowObj
{
    u8 filler0[0x14];
    struct WindowObj *unk14;
    const void *cls18;
    struct WindowChild *child1C;
    void *menu20;
    void *owner24;
    void *sub28;
    void *buf2C;
    u8 filler30[0x40 - 0x30];
    void *ptr40;
    u8 filler44[0x84 - 0x44];
    void *ptr84;
    void *p88;
    u8 filler8C[0x144 - 0x8C];
    void *p144;
    u8 filler148[0x14F - 0x148];
    u8 state14F;
    u8 unk150;
    u8 flags151;
    u8 byte152;
    u8 byte153;
    u8 filler154[0x156 - 0x154];
    u8 sub156;
    u8 filler157[0x158 - 0x157];
    void *p158;
    u8 filler15C[0x180 - 0x15C];
    u8 *settings180; /* gUnk_03000388: [8] key repeat delay, [9] repeat rate, +0x3C enabled keys (10 bits) */
    u16 shadow184;   /* enabled keys before the window opened */
    u8 unk186;       /* key repeat delay before the window opened */
    u8 unk187;       /* key repeat rate before the window opened */
    u8 filler188[0x194 - 0x188];
    u8 b194;
    u8 filler195[0x19E - 0x195];
    u8 b19E;
    u8 b19F;
    u8 filler1A0[0x1A4 - 0x1A0];
    u8 b1A4;
    u8 filler1A5[0x1A6 - 0x1A5];
    u8 b1A6;
    u8 filler1A7[0x1E0 - 0x1A7];
    void *p1E0[11];
    u8 filler20C[0x220 - 0x20C];
    void *p220;
    u8 filler224[0x258 - 0x224];
    void *p258;
    void *p25C;
};

struct MenuHead
{
    u8 filler0[0x34];
    u16 unk34;
    u8 filler36[0x4A - 0x36];
    u16 unk4A;
};

struct SpriteRec
{
    u8 filler0[0x12];
    u8 b12;
};

extern u8 gUnk_03000388[]; /* key input settings: [8] repeat delay, [9] repeat rate, +0x3C enabled keys */

#define KEY_INPUT_ENABLED_KEYS 0x3C /* u16 offset in gUnk_03000388 */
#define KEYS_ALL 0x3FF              /* all 10 GBA buttons */
extern u8 *gUnk_03000FC0;
extern const u8 gWindowObjClass[];
extern const u8 gWindowChildClass[];
extern const u8 gMenuChildTag[];
extern const u8 gMenuObjTag[];
extern const u8 gMenuBufTag[];

void task_insert(void *node, u8 kind, u32 data);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *menu_obj_init(void *obj, u32 kind, const void *tag, u32 flag);
void *clear_anim_record(void *block);
void sub_08117380(void *menu);
void sub_0811733C(void *menu, u32 a);
u32 sub_obj_running(void *menu);
u32 state_mode(void *owner);
void clear_slot_marks(void *owner);
u8 menu_cursor_state0(struct WindowObj *obj);
void menu_reel_state1(struct WindowObj *obj);
u32 poll_spline_path(void *buf, void *x, void *h);
void obj_load_record(void *rec, u32 a, s32 b, u32 c, u32 d, void *e, void *f, void *g);
void sprite_update_cd0(void *sprite);
void char_menu_cell(u32 idx, u32 ch, u32 attr, u32 zero);
void set_slot_value(u32 id, s32 value);
void char_menu_cursor_step(struct WindowObj *obj);
void obj_sprite_state_step(struct WindowObj *obj);
void obj_sprite_refresh(struct WindowObj *obj);
void heap_free_a(void *p);
void heap_free_b(void *p);
void owner_destroy(void *owner, u32 mode);
void menu_obj_free(void *sub, u32 mode);
void obj_free_buffers_and_remove(void *sub, u32 mode);
void run_pending_obj_callbacks(void);
void task_remove(void *node, u32 kind);
u8 test_flag(u8 *table, u8 *tableB, u16 id);

/* Constructor of the window task: inserts it, saves the key input settings (enabled keys, repeat delay
 * and rate) and switches to all keys with repeat 10/8, blanks the display, and creates its child task,
 * menu object and animation record. screen_obj_destroy is its destructor. */
struct WindowObj *window_obj_init(struct WindowObj *obj, u8 prio, u32 arg, u32 flags)
{
    struct WindowChild *child;
    u16 enabledKeys;

    task_insert(obj, prio, arg);
    obj->cls18 = gWindowObjClass;
    obj->flags151 = flags;
    obj->settings180 = gUnk_03000388;
    enabledKeys = *(u16 *)(gUnk_03000388 + KEY_INPUT_ENABLED_KEYS);
    obj->shadow184 = ((u32)enabledKeys << 22) >> 22;
    *(u16 *)(gUnk_03000388 + KEY_INPUT_ENABLED_KEYS) = enabledKeys | KEYS_ALL;
    obj->unk186 = obj->settings180[8];
    obj->unk187 = obj->settings180[9];
    obj->settings180[8] = 10;
    obj->settings180[9] = 8;
    REG_BUF_DISPCNT = 0;
    REG_BUF_BLDCNT = 0;
    child = (struct WindowChild *)heap_alloc2(0x1C, 0, gMenuChildTag, 1);
    task_insert(child, 8, (u32)gMenuChildTag);
    child->cls18 = gWindowChildClass;
    obj->child1C = child;
    obj->menu20 = menu_obj_init(heap_alloc2(0x5C, 0, gMenuObjTag, 1), 8, gMenuObjTag, 8);
    obj->buf2C = clear_anim_record(heap_alloc2(0x10, 0, gMenuBufTag, 1));
    obj->child1C->unk14 = obj;
    return obj;
}

// Destructor of the window object built by window_obj_init: blanks the display (DISPCNT,
// windows and blending off), frees the object's buffers, its owner, menu and child task,
// then removes the object itself from the task list.
void screen_obj_destroy(struct WindowObj *obj, u32 flags)
{
    void **buf88;
    s32 i;

    obj->cls18 = gWindowObjClass;
    REG_BUF_DISPCNT = 0;
    REG_BUF_WININ = 0;
    REG_BUF_WINOUT = 0;
    REG_BUF_BLDCNT = 0;
    for (i = 10; i >= 0; i--)
    {
        if (obj->p1E0[i] != NULL)
            heap_free_a(obj->p1E0[i]);
    }
    if (test_flag((u8 *)gUnk_03000FC0 + 0x38C, 0, 0x225) << 24 != 0)
    {
        if (obj->flags151 == 0xFF && obj->sub28 != NULL)
            obj_free_buffers_and_remove(obj->sub28, 3);
    }
    if (obj->owner24 != NULL)
        owner_destroy(obj->owner24, 3);
    run_pending_obj_callbacks();
    buf88 = &obj->p88;
    if (obj->p25C != NULL)
        heap_free_a(obj->p25C);
    if (obj->p258 != NULL)
        heap_free_a(obj->p258);
    if (obj->p158 != NULL)
        heap_free_a(obj->p158);
    if (*buf88 != NULL)
        heap_free_a(*buf88);
    if (obj->p144 != NULL)
        heap_free_a(obj->p144);
    if (obj->p220 != NULL)
        heap_free_a(obj->p220);
    heap_free_b(obj->buf2C);
    if (obj->menu20 != NULL)
        menu_obj_free(obj->menu20, 3);
    if (obj->child1C != NULL)
        task_remove(obj->child1C, 3);
    task_remove(obj, flags);
}

ASM_FUNC("asm/code_08118A0C/window_obj_step.s", void window_obj_step(void))

ASM_FUNC("asm/code_08118A0C/screen_helpers_obj_state_step.s", void screen_helpers_obj_state_step(void))

ASM_FUNC("asm/code_08118A0C/menu_cursor_state0.s", u8 menu_cursor_state0(struct WindowObj *obj))
