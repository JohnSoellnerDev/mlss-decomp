#include "global.h"
#include "ram_routines.h"

struct TaskObj
{
    u8 filler0[0x18];
    const void *classPtr18;
};

struct SaveScreen
{
    u8 unk0;
    u8 filler1[0x13];
    void *unk14;
    u8 filler18[0x48 - 0x18];
    void *unk48;
    u8 filler4C[0xE4 - 0x4C];
    u16 unkE4;
    u16 unkE6;
    u8 filler0E8[0xEE - 0xE8];
    u8 unkEE;
    u8 unkEF;
    u8 filler0F0[0xF1 - 0xF0];
    u8 unkF1;
    u8 filler0F2[0xF3 - 0xF2];
    u8 unkF3;
    u8 filler0F4[0xFA - 0xF4];
    s8 unkFA;
    u8 filler0FB[0x154 - 0xFB];
    u8 unk154[3];
    u8 unk157;
    u8 filler158[0x15C - 0x158];
    void *child15C;
};

struct ObjList
{
    u8 filler0[0x3C];
    void *unk3C;
};

struct SaveEntry
{
    u8 filler0[0x12];
    u16 unk12;
    u8 filler14[0x17 - 0x14];
    u8 unk17;
    u8 filler18[0x1C - 0x18];
};

struct SysPal
{
    u8 filler0[0x880];
    u32 unk880;
    u32 unk884;
};

struct Quad
{
    u32 w[4];
};

struct SpriteListObj
{
    u8 filler0[0x1C];
    void *unk1C;
    void *unk20[6];
    void *unk38[4];
    u8 filler48[0xEC - 0x48];
    u16 unkEC;
    u8 filler0EE[0xF1 - 0xEE];
    u8 unkF1;
    u8 unkF2;
    u8 filler0F3[0xFA - 0xF3];
    s8 unkFA;
    u8 filler0FB[0x100 - 0xFB];
    u8 unk100[3][0x1C];
};

struct SpriteObj
{
    u16 attr0;
    u16 attr1;
    u8 filler4[0xA];
    u16 attr2;
    u8 unk10;
    u8 filler11;
    u8 unk12;
    u8 filler13[0xC];
    u8 unk1F;
    u8 filler20[9];
    u8 unk29;
};

struct Bits1F
{
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
} __attribute__((packed));

struct Bits10
{
    u8 b0 : 1;
    u8 b1_5 : 5;
    u8 b6_7 : 2;
} __attribute__((packed));

struct Bits12
{
    u8 b0 : 1;
    u8 b1_2 : 2;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5_7 : 3;
} __attribute__((packed));

struct Bits29
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2_4 : 3;
    u8 b5 : 1;
    u8 b6_7 : 2;
} __attribute__((packed));

static inline struct Bits1F *bits1F(void *p)
{
    return (struct Bits1F *)&((struct SpriteObj *)p)->unk1F;
}

static inline struct Bits10 *bits10(void *p)
{
    return (struct Bits10 *)&((struct SpriteObj *)p)->unk10;
}

static inline struct Bits12 *bits12(void *p)
{
    return (struct Bits12 *)&((struct SpriteObj *)p)->unk12;
}

static inline struct Bits29 *bits29(void *p)
{
    return (struct Bits29 *)&((struct SpriteObj *)p)->unk29;
}

extern const struct Quad gSetupTable[];
extern u8 gSys[];
extern u32 gUnk_03000FFC;
extern const u8 gSaveScreenChildClass[];
extern u8 gUnk_0203FFB8[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;

void task_insert(void *node, u8 kind, u32 data);
void save_slot_info(void *out, u8 slot);
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
u32 sub_obj_running(void *menu);
void set_slot_fields(u32 idx, u8 level, u8 period);
void set_slot_target(u32 idx, u32 value, u32 mode);
u32 draw_two_values(void *oam, u8 a, u8 b, u32 flag);
void update_obj_list(void);
void obj_list_pass_0a8c(void);
void obj_redraw_box(struct SaveScreen *obj);
u32 draw_list_rows(struct SaveScreen *obj, u32 oam);
void cpu_set(const void *src, void *dst, u32 ctl);
u32 table_085213b8(u32 index);
void psg_wave_setup(u8 *base, u32 width, u32 x, u32 y, u32 glyph, s32 ch, u32 pal, u32 zero);
u32 text_sprites_b(u32 oam, u32 a, u32 b, u32 c);
u32 draw_member_status(u32 oam, u32 a, u32 b);
u32 draw_sprite(u32 oam, u32 x, u32 size, u32 zero, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u32 a10);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_update_cd0(void *sprite);
u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g);

ASM_FUNC("asm/code_081272CC/obj_anim_step.s", void obj_anim_step(void))

NONMATCH("asm/code_081272CC/obj_build_sprite_list.s", void obj_build_sprite_list(struct SpriteListObj *obj))
{
    struct Quad tableA;
    struct Quad tableB;
    u8 *flag;
    s32 i;
    s32 k;
    s32 xoff;
    s32 zero;

    tableA = gSetupTable[0];
    tableB = *(struct Quad *)((u8 *)gSetupTable + 0x10);
    flag = &obj->unkF2;
    if (*flag == 0)
    {
        find_or_alloc_obj_slots(5, 0x6003, 0, 0, 0, (u8 *)obj + 0x4C, -1);
        obj->unk1C = create_obj_de4(5, 0x6005, 0, -1, -1, -1, -1);
        obj_load_record(obj->unk1C, 0, -1, 0, 0);
        sprite_update_cd0(obj->unk1C);
        ((struct SpriteObj *)obj->unk1C)->attr0 = ((s8)obj->unkFA << 5) + 0x20;
        ((struct SpriteObj *)obj->unk1C)->attr1 = 0x16;
        ((struct SpriteObj *)obj->unk1C)->attr2 = (u16)0xEFFF;
        bits1F(obj->unk1C)->f0 = 0;
        bits1F(obj->unk1C)->f2 = 0;
        bits1F(obj->unk1C)->f4 = 0;
        bits1F(obj->unk1C)->f6 = 0;
        bits10(obj->unk1C)->b1_5 = 0;
        bits29(obj->unk1C)->b1 = 0;
        bits29(obj->unk1C)->b5 = 0;
        *flag = 1;
    }
    else if (*flag == 1)
    {
        obj->unk38[obj->unkEC] = create_obj_de4(5, 0x6005, 0, -1, -1, -1, -1);
        obj_load_record(obj->unk38[obj->unkEC], obj->unkEC + 3, -1, 0, 0);
        sprite_update_cd0(obj->unk38[obj->unkEC]);
        ((struct SpriteObj *)obj->unk38[obj->unkEC])->attr0 = tableA.w[obj->unkEC];
        ((struct SpriteObj *)obj->unk38[obj->unkEC])->attr1 = tableB.w[obj->unkEC];
        ((struct SpriteObj *)obj->unk38[obj->unkEC])->attr2 = (u16)0xEFFF;
        bits1F(obj->unk38[obj->unkEC])->f0 = 1;
        bits1F(obj->unk38[obj->unkEC])->f2 = 1;
        bits1F(obj->unk38[obj->unkEC])->f4 = 1;
        bits1F(obj->unk38[obj->unkEC])->f6 = 1;
        bits10(obj->unk38[obj->unkEC])->b1_5 = 0;
        bits29(obj->unk38[obj->unkEC])->b1 = 0;
        bits29(obj->unk38[obj->unkEC])->b5 = 0;
        bits12(obj->unk38[obj->unkEC])->b4 = 1;
        bits12(obj->unk38[obj->unkEC])->b1_2 = 1;
        obj->unkEC++;
        if (obj->unkEC == 4)
            *flag = 2;
    }
    else
    {
        for (i = 0; i < 3; i++)
        {
            u8 *ent = obj->unk100[i];
            void **slot = &obj->unk20[2 * i];
            xoff = 0;
            for (k = 0; k < 2; k++)
            {
                s32 g;

                if (*(u16 *)ent != 0 && ((g = obj->unk100[i][0x18]) == 2 || g == k))
                {
                    *slot = create_obj_de4(0, *(u16 *)ent, 1, -1, -1, -1, -1);
                    obj_load_record(*slot, ent[2], -1, 0, 0);
                    obj_load_record(*slot, ent[2], ent[3], 0, 0);
                    sprite_update_cd0(*slot);
                    ((struct SpriteObj *)*slot)->attr0 = 0xBC - xoff;
                    ((struct SpriteObj *)*slot)->attr1 = (i != 0) ? 0xB4 : 0x68;
                    ((struct SpriteObj *)*slot)->attr2 = (u16)0xEFFF;
                    bits1F(*slot)->f0 = 3;
                    bits1F(*slot)->f2 = 3;
                    bits1F(*slot)->f4 = 3;
                    bits1F(*slot)->f6 = 3;
                    bits10(*slot)->b1_5 = 0;
                    bits29(*slot)->b1 = 0;
                    bits29(*slot)->b5 = 0;
                    bits12(*slot)->b4 = 1;
                    bits12(*slot)->b1_2 = 1;
                }
                else
                {
                    s32 bit = (obj->unkF1 >> i) & 1;
                    if (bit != 0)
                    {
                        find_or_alloc_obj_slots(5, 0x6002, 0, 0, 0, (u8 *)obj + 0x4C, -1);
                        *slot = create_obj_de4(5, 0x6006, 1, -1, -1, -1, -1);
                        obj_load_record(*slot, k, -1, 0, 0);
                        sprite_update_cd0(*slot);
                        ((struct SpriteObj *)*slot)->attr0 = 0xBC - xoff;
                        ((struct SpriteObj *)*slot)->attr1 = (i != 0) ? 0xB4 : 0x68;
                        ((struct SpriteObj *)*slot)->attr2 = (u16)0xEFFF;
                        bits1F(*slot)->f0 = 3;
                        bits1F(*slot)->f2 = 3;
                        bits1F(*slot)->f4 = 3;
                        bits1F(*slot)->f6 = 3;
                        bits10(*slot)->b1_5 = 0;
                        bits29(*slot)->b1 = 0;
                        bits29(*slot)->b5 = 0;
                    }
                    else
                    {
                        *slot = (void *)bit;
                    }
                }
                xoff += 0x28;
                ent += 8;
                slot++;
            }
        }
        zero = 0;
        update_obj_list();
        obj_list_pass_0a8c();
        ((void (*)(void *, void *))gUnk_03000D48)(((struct ObjList *)gUnk_0203FFB8)->unk3C,
            (u8 *)((struct ObjList *)gUnk_0203FFB8)->unk3C + 0xA14);
        *flag = zero;
        obj->unkEC = zero;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_081272CC/draw_list_rows.s", u32 draw_list_rows(struct SaveScreen *obj, u32 oam))

struct RedrawObj
{
    u8 filler0[0x54];
    void *unk54;
    void *unk58;
    void *unk5C;
    void *unk60;
    u8 filler64[0xBC - 0x64];
    u8 *unkBC;
    u8 filler0C0[0xE8 - 0xC0];
    u16 unkE8;
    u16 unkEA;
    u8 filler0EC[0xEE - 0xEC];
    u8 unkEE;
    u8 filler0EF[0xF0 - 0xEF];
    u8 unkF0;
    u8 unkF1;
    u8 filler0F2[0xFA - 0xF2];
    s8 unkFA;
    u8 unkFB;
    u8 filler0FC[0x100 - 0xFC];
    u8 unk100[3][0x1C];
    u8 filler154[0x160 - 0x154];
    u8 *unk160;
};

extern void *gLangTracksA[];
extern void *gLangTracksB[];

void draw_text(void *buf, u32 width, u32 x, u32 y, void *text, u32 palette);
void *free_list_alloc(void *heap, u32 size);
u32 vram_tile_index(void *addr);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void free_list_move(void *head, void *block);
void fill_tile_rect(void *map, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
u32 area_table_lookup_081240AC(u16 entry);
void voice_setup_params(void *voice, void *src, u32 art, u32 zero, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
void *voice_set_data_ptr(void *voice, u32 a, u32 b, void *ptr);
u32 track_run(void *track);

static inline s32 track_more(const u8 *p)
{
    return p[0] != 0 || p[-1] == 0xFF;
}

static inline u32 cfg_value(struct RedrawObj *obj)
{
    u8 *x = obj->unk160;
    u8 *y = *(u8 **)(x + 0x28);
    u8 *z = *(u8 **)(y + 4);
    return *(u32 *)(z + 0x194);
}

NONMATCH("asm/code_081272CC/obj_redraw_box.s", void obj_redraw_box(struct SaveScreen *self))
{
    struct RedrawObj *obj = (struct RedrawObj *)self;
    u8 mode = obj->unkEE;
    u8 *langp;
    void **tbl;
    void **tblB;
    void *voice;
    u32 r5;
    u32 r6;
    u32 idx9;

    if (mode == 1)
    {
        langp = gSys + 0x8C0;
        tbl = gLangTracksA[*langp];
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk58, 0xF20);
        if (obj->unkF0 > 1)
        {
            voice = (u8 *)obj + 0x64;
            idx9 = (((u32)(mode & obj->unkF0)) << 26) + 0x07000000;
            idx9 >>= 24;
            voice_setup_params(voice, obj->unk58, 0xB, 0, 0, 0, 0, 2, 5, 4);
            voice_set_data_ptr(voice, 0, 4, tbl[idx9]);
            r6 = 0;
            if (track_more(tbl[idx9]))
            {
                do
                {
                    track_run(voice);
                    r6++;
                } while (track_more((u8 *)tbl[idx9] + r6));
            }
        }
        else
        {
            if (obj->unkEA != 0)
            {
                voice = (u8 *)obj + 0x64;
                voice_setup_params(voice, obj->unk58, 0xB, 0, 0, 0, 0, 2, 9, 4);
                voice_set_data_ptr(voice, 0, 4, tbl[8]);
                r5 = 0;
                if (track_more(tbl[8]))
                {
                    do
                    {
                        track_run(voice);
                        r5++;
                    } while (track_more((u8 *)tbl[8] + r5));
                }
            }
            else if (obj->unkE8 == 0xFFFF)
            {
                tblB = gLangTracksB[*langp];
                voice = (u8 *)obj + 0x64;
                voice_setup_params(voice, obj->unk58, 0xB, 0, 0, 0, 0, 2, 9, 4);
                voice_set_data_ptr(voice, 0, 0xB, tblB[7]);
                r6 = 0;
                if (*(u8 *)tbl[7] != 0 || ((u8 *)tblB[7])[-1] == 0xFF)
                {
                    do
                    {
                        track_run(voice);
                        r6++;
                    } while (*((u8 *)tbl[7] + r6) != 0 || ((u8 *)tblB[7] + r6)[-1] == 0xFF);
                }
            }
            else
            {
                u8 f = obj->unkFB;
                s32 drawText = 0;

                if (f & 0x20)
                {
                    voice = (u8 *)obj + 0x64;
                    voice_setup_params(voice, obj->unk58, 0xB, 0, 0, 0, 0, 2, 9, 4);
                    voice_set_data_ptr(voice, 0, 4, tbl[10]);
                    r5 = 0;
                    if (track_more(tbl[10]))
                    {
                        do
                        {
                            track_run(voice);
                            r5++;
                        } while (track_more((u8 *)tbl[10] + r5));
                    }
                    drawText = 1;
                }
                else if (f & 0x10)
                {
                    voice = (u8 *)obj + 0x64;
                    voice_setup_params(voice, obj->unk58, 0xB, 0, 0, 0, 0, 2, 5, 4);
                    voice_set_data_ptr(voice, 0, 4, tbl[9]);
                    r5 = 0;
                    if (track_more(tbl[9]))
                    {
                        do
                        {
                            track_run(voice);
                            r5++;
                        } while (track_more((u8 *)tbl[9] + r5));
                    }
                    drawText = 1;
                }
                else if (f & 0xFF)
                {
                    voice = (u8 *)obj + 0x64;
                    voice_setup_params(voice, obj->unk58, 0xB, 0, 0, 0, 0, 2, 5, 4);
                    voice_set_data_ptr(voice, 0, 4, tbl[6]);
                    r6 = 0;
                    if (track_more(tbl[6]))
                    {
                        do
                        {
                            track_run(voice);
                            r6++;
                        } while (track_more((u8 *)tbl[6] + r6));
                    }
                }
                else
                {
                    u32 y = 0x40000;
                    void **tp = tbl;

                    for (r6 = 0; r6 <= 3; r6 += 2)
                    {
                        if (((obj->unkF1 >> obj->unkFA) & 1) != 0 || r6 == 0 || r6 == 3)
                            draw_text(obj->unk58, 0xB, 0, y >> 16, tp[0], 5);
                        if (((obj->unkF1 >> obj->unkFA) & 1) != 0 || r6 + 1 == 0 || r6 + 1 == 3)
                            draw_text(obj->unk58, 0xB, 0, (y + 0xC0000) >> 16, tp[1], 5);
                        tp += 2;
                        y += 0x180000;
                    }
                }

                if (drawText)
                {
                    draw_text(obj->unk58, 0xB, 0xC, 0x30, tbl[12], 5);
                    draw_text(obj->unk58, 0xB, 0xC, 0x3C, tbl[13], 5);
                }
            }
        }

        /* common tail of the mode 1 path */
        dma_copy_or_queue(obj->unk58, (u8 *)obj->unk58 - cfg_value(obj), 0xF20, 0x20, 0);
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk5C, 0x8A0);
        {
            s32 fa = obj->unkFA;
            u16 entry = *(u16 *)(obj->unk100[fa] + 0x12);

            if (entry == 0xFFFF)
            {
                tblB = gLangTracksB[*langp];
                voice = (u8 *)obj + 0xB4;
                voice_setup_params(voice, obj->unk5C, 0x17, 0, 0, 0, 0, 2, 0xB, 4);
                voice_set_data_ptr(voice, 0, 8, tblB[10]);
                do
                {
                    u8 *cp = obj->unkBC;
                    if (cp[0] == 0xFF && cp[1] == 0)
                        obj->unkBC = cp + 2;
                } while ((u16)track_run(voice) != 0);
            }
            else if ((obj->unkF1 >> fa) & 1)
            {
                voice = (u8 *)obj + 0xB4;
                voice_setup_params(voice, obj->unk5C, 0x17, 0, 0, 0, 0, 2, 0xB, 4);
                voice_set_data_ptr(voice, 0, 8, (void *)area_table_lookup_081240AC(entry));
                do
                {
                    u8 *cp = obj->unkBC;
                    if (cp[0] == 0xFF && cp[1] == 0)
                        obj->unkBC = cp + 2;
                } while ((u16)track_run(voice) != 0);
            }
        }
        dma_copy_or_queue(obj->unk5C, (u8 *)obj->unk5C - cfg_value(obj), 0x8A0, 0x20, 0);
    }
    else if (mode == 2)
    {
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk58, 0xF20);
        free_list_move(obj->unk60, (u8 *)obj->unk58 - cfg_value(obj));
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk5C, 0x8A0);
        free_list_move(obj->unk60, (u8 *)obj->unk5C - cfg_value(obj));
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk54, 0x800);
    }
    else if (mode == 3)
    {
        u32 tile;
        void *block;

        block = free_list_alloc(obj->unk60, 0xF20);
        obj->unk58 = (u8 *)block + cfg_value(obj);
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk58, 0xF20);
        tile = (vram_tile_index((u8 *)obj->unk58 - cfg_value(obj)) | 0xE000) & 0xFFFF;
        fill_tile_rect(obj->unk54, 0x20, 0x20, 0xB, 0xB, tile, 1);
        block = free_list_alloc(obj->unk60, 0x8A0);
        obj->unk5C = (u8 *)block + cfg_value(obj);
        ((void (*)(u32, void *, u32))gMemRoutines)(0, obj->unk5C, 0x8A0);
        tile = (vram_tile_index((u8 *)obj->unk5C - cfg_value(obj)) | 0xE000) & 0xFFFF;
        fill_tile_rect(obj->unk54, 0x20, 0x78, 0x17, mode, tile, 1);
    }

    obj->unkEE = 0;
    MEM_ROUTINE(void (*)(void *, void *, u32), iwram_copy)(obj->unk54, (void *)0x0600D000, 0x800);
}
END_NONMATCH

NONMATCH("asm/code_081272CC/update_entry_flags.s", void update_entry_flags(struct SaveScreen *obj))
{
    u8 k;
    u16 src;
    struct SaveEntry *entry = (struct SaveEntry *)((u8 *)obj + 0x100);
    u8 *count = &obj->unk157;
    u32 listOff = 0;
    s32 bit = 1;
    *count = 0;
    obj->unkF3 = 0;
    for (k = 0; k <= 2; k++)
    {
        s32 visible = (obj->unkF1 >> k) & bit;
        if (visible == 0)
        {
            obj->unk154[*count] = k;
            (*count)++;
            src = visible;
            cpu_set(&src, entry, 0x0100000E);
            entry->unk17 = 0;
            if (((u8 *)obj->unk48 + listOff)[0x10] == 0xFF)
                entry->unk12 = 0xFFFF;
        }
        else
        {
            save_slot_info(entry, k);
            if (entry->unk12 == 0x113)
            {
                u8 *f3 = &obj->unkF3;
                *f3 |= bit << k;
            }
        }
        entry++;
        listOff += 0x6F8;
    }
}
END_NONMATCH

void update_screen(struct SaveScreen *obj)
{
    u32 sysBase = (u32)gSys;
    u32 *oamPtr = (u32 *)(sysBase + 0x880);
    u32 oam = *oamPtr;
    struct SaveScreen *sub = (struct SaveScreen *)obj->unk14;
    if (sub->unk0 > 3)
    {
        u8 flags;
        s8 sel;
        if (sub->unkEE != 0)
            obj_redraw_box(sub);
        flags = sub->unkF1;
        sel = sub->unkFA;
        if ((flags >> sel) & 1)
        {
            u8 *entry = (u8 *)sub + sel * 0x1C;
            u8 a1 = entry[0x114];
            u8 a2 = entry[0x115];
            u8 *f3 = &sub->unkF3;
            oam = draw_two_values((void *)oam, a1, a2, (1 << sel) & *f3);
        }
        update_obj_list();
        obj_list_pass_0a8c();
        ((void (*)(void *, void *))gUnk_03000D48)(((struct ObjList *)gUnk_0203FFB8)->unk3C,
            (u8 *)((struct ObjList *)gUnk_0203FFB8)->unk3C + 0xA14);
        oam = ((u32 (*)(void *, u32))gUnk_03000D4C)(((struct ObjList *)gUnk_0203FFB8)->unk3C, oam);
        oam = draw_list_rows(sub, oam);
        *oamPtr = oam;
    }
}

void *create_child_task(void *task, u8 kind, u32 data)
{
    task_insert(task, kind, data);
    ((struct TaskObj *)task)->classPtr18 = gSaveScreenChildClass;
    return task;
}

u32 child_busy(struct SaveScreen *obj)
{
    u32 result;
    u8 running = sub_obj_running(obj->child15C);
    if (running != 0)
        result = 1;
    else
    {
        obj->unkEF = running;
        result = 0;
    }
    return result;
}

void start_child_anim(struct SaveScreen *obj, u16 value)
{
    sub_08115F54(obj->child15C, 4, value, 0x10, 0xFFFF, 0xFFFF, 0, 0);
    set_slot_fields(0, 0, 0x10);
    obj->unkEF = 1;
}

void reset_child_anim(struct SaveScreen *obj)
{
    ((struct SysPal *)gSys)->unk884 = -1;
    sub_08115F54(obj->child15C, 0, obj->unkE4, 0x10, 0xFFFF, 0xFFFF, 0, 0);
    set_slot_target(0, 0x1E, 0);
    set_slot_fields(0, 0xFF, 0x10);
    *(u16 *)0x02000000 = obj->unkE6;
    obj->unkEF = 1;
}

u32 draw_slot_two_values(struct SaveScreen *obj, void *oam)
{
    s8 idx = obj->unkFA;
    u8 *entry = (u8 *)obj + idx * 0x1C;
    u8 a1 = entry[0x114];
    u8 a2 = entry[0x115];
    u8 *flags = &obj->unkF3;
    return draw_two_values(oam, a1, a2, (1 << idx) & *flags);
}

NONMATCH("asm/code_081272CC/save_slot_info.s", void save_slot_info(void *out, u8 slot))
{
    u8 *base = (u8 *)gUnk_03000FFC;
    u32 off = slot * 0x6F8;
    u8 *at = base + off;
    u32 t;
    u8 *pt;
    u8 *fr;
    u8 *s;
    u32 w;
    u32 status;

    ((u8 *)out)[0x14] = at[0x60];
    ((u8 *)out)[0x15] = at[0x9C];
    *(long long *)out = *(long long *)(at + 0xA8);
    *(long long *)((u8 *)out + 8) = *(long long *)(at + 0xB0);
    *(u16 *)((u8 *)out + 0x10) = *(u16 *)(at + 0xB8);
    t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(*(u32 *)(fr = (pt = base + 0x700) + off), 216000);
    if (t > 0x63)
    {
        ((u8 *)out)[0x17] = 0x3B;
        ((u8 *)out)[0x16] = 0x63;
    }
    else
    {
        ((u8 *)out)[0x16] = t;
        t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(*(u32 *)fr, 0xE10);
        ((u8 *)out)[0x17] = MATH_ROUTINE(u32 (*)(u32, u32), iwram_math_08199424)(t, 0x3C);
    }
    s = base + slot * 0x6F8;
    w = ((((u32)s[0x14] << 28) >> 31) * 12) + slot * 0x6F8;
    *(u16 *)((u8 *)out + 0x12) = *(u16 *)(base + w + 0x18);
    status = ((s32)(s[0x14] << 26) < 0 || (s32)(s[0x16] << 29) < 0) ? 2 : ((u32)s[0x14] << 28) >> 31;
    ((u8 *)out)[0x18] = status;
}
END_NONMATCH

NONMATCH("asm/code_081272CC/draw_text_line.s", void draw_text_line(u8 *dst, u8 width, u16 x, u16 y, u8 *text, u8 palette))
{
    u32 *fonts;
    u32 *font0;
    s32 code;
    u32 font;
    u32 *slot;
    u32 w;

    fonts = (u32 *)table_085213b8(1);
    font0 = (u32 *)fonts[0];
    y = ((font0[0] & 0xF) << 2) + y;
    y++;
    palette &= 0xF;
    while (*text != 0)
    {
        if ((s32)(x + 8) > (s32)(width << 3))
            break;
        do
        {
            code = *text;
            text++;
        } while (code == 0xFF);
        font = 0xFF - code;
        if (font == 0xFF)
            break;
        if ((s32)font > 1)
            font = 0;
        else
        {
            if (fonts[font] == 0)
                continue;
            code = *text;
            text++;
        }
        slot = &fonts[font];
        psg_wave_setup(dst, width, x, y, *slot, code, palette, 0);
        w = *(u32 *)((u8 *)*slot + ((code >> 3) << 2) + 4);
        w = w >> ((code & 7) << 2);
        x = (x + 1 + (w & 0xF)) & 0xFFFF;
    }
}
END_NONMATCH
