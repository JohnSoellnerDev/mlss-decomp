#include "global.h"

struct Sprite
{
    u8 filler0[0x12];
    u8 flags;
};

struct Figure
{
    u8 filler0[0x10];
    struct Sprite *link;
    u8 filler14[0x7E - 0x14];
    u8 ctrl;
    u8 filler7F[0xEC - 0x7F];
    s16 anim;
};

struct DisplaySub
{
    u8 filler0[0x4];
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
};

struct DisplayEntry
{
    u8 filler0[0x8];
    void *data;
    struct DisplaySub *sub;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 mode18;
};

struct Owner
{
    u8 filler0[0x4];
    void (*state)(struct Owner *);
    u8 filler8[0x4];
    struct Figure *fig;
    u16 timer;
    u16 index;
    s16 posY;
    s16 unk16;
    u16 phase18;
};


struct ListNode
{
    u8 filler0[0x4];
    u32 unk4;
};

struct World
{
    u8 filler0[0x07];
    u8 digitCtr;
    u8 spriteMode;
    u8 filler9[0x48 - 0x09];
    u8 list48[0x10];
    u8 filler58[0x80 - 0x58];
    struct Figure *fig[2];
    u8 filler88[0x250 - 0x88];
    void *slot4;
    u8 filler254[0x2D4 - 0x254];
    u16 timer2D4;
    u8 filler2D6[0x368 - 0x2D6];
    void *menuObj;
    void *obj36C;
    struct ListNode *digitObj;
    struct ListNode *listOut;
    void *spriteChain;
    void *scrollObj;
    struct ListNode *entry380[6];
};

extern struct World *gWorld;
extern u8 gSys[];
void sprite_set_action(void *obj, u32 action, u32 a2, u32 a3);
struct DisplayEntry *list_add_task(void *list, void (*state)(struct DisplayEntry *));
struct DisplaySub *sprite_node_alloc_default(void);
u32 to_bcd(u32 value);
void state_display_entry_draw(struct DisplayEntry *entry);
void state_spawn_list_entry(struct Owner *owner);
void state_show_hero_label(struct Owner *owner);
void state_apply_hero_stats(struct Owner *owner);
void clear_map_rect(void *slot, s32 a, s32 b, s32 c, s32 d, s32 e);
void obj_release(void *obj);
void obj_release_node(void *node);
extern u8 *gParty;
extern const u8 gLevelExpTable[];
void ensure_code_entry(u32 id);
void alloc_code_entry(u32 id);
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void draw_digits_49_p11(s32 a, s32 b, s32 c, s32 d);
extern u8 gSys[];
extern u16 gGlyphIndexTable[];
extern u32 gMapTable[];

ASM_FUNC("asm/code_08102934/state_draw_rows.s", void state_draw_rows(void))

ASM_FUNC("asm/code_08102934/create_display_entry.s", struct DisplayEntry *create_display_entry(void *data, u32 value, u32 stat, u32 mode, s32 s0, s32 s1, s32 s2, s32 s3, s32 s4, u32 s5, u32 s6, u32 s7, s32 width))

ASM_FUNC("asm/code_08102934/state_display_entry_draw.s", void state_display_entry_draw(struct DisplayEntry *entry))

ASM_FUNC("asm/code_08102934/state_create_display_entries.s", void state_create_display_entries(void))

ASM_FUNC("asm/code_08102934/obj_apply_world_mode.s", void obj_apply_world_mode(void))

ASM_FUNC("asm/code_08102934/state_result_wait_confirm.s", void state_result_wait_confirm(void))

ASM_FUNC("asm/code_08102934/state_assign_figure.s", void state_assign_figure(void))

void state_show_hero_label(struct Owner *owner)
{
    u32 msg;
    u16 *glyphs;
    u32 *maps;
    u32 idx;

    ensure_code_entry(0x7003);
    alloc_code_entry(0x7003);
    switch (gSys[0x8C0])
    {
    case 0:
        msg = 0x7004;
        break;
    case 1:
        msg = 0x7005;
        break;
    case 2:
        msg = 0x7006;
        break;
    case 3:
        msg = 0x7007;
        break;
    case 4:
        msg = 0x7008;
        break;
    }
    ensure_code_entry(msg);
    alloc_code_entry(msg);
    glyphs = gGlyphIndexTable;
    idx = glyphs[1] << 2;
    maps = gMapTable;
    queue_display_entry(2, (u8 *)maps + *(u32 *)((u8 *)maps + idx), (void *)0x06008000, 0x920, 0, 0);
    idx = gSys[0x8C0];
    idx += 2;
    queue_display_entry(2, (u8 *)maps + maps[glyphs[idx]], (void *)0x06008920, 0x520, 0, 0);
    owner->timer = 0;
    owner->phase18 = 3;
    owner->state = state_spawn_list_entry;
}

ASM_FUNC("asm/code_08102934/state_figure_jump_start.s", void state_figure_jump_start(void))

ASM_FUNC("asm/code_08102934/task_countdown_digits.s", void task_countdown_digits(void))

ASM_FUNC("asm/code_08102934/state_digits_slide.s", void state_digits_slide(void))
