#include "global.h"
#include "ram_routines.h"

struct SpriteNode;
struct SpriteObj;

struct OamRow
{
    u16 hw0;
    u16 hw2;
    u16 hw4;
    u16 hw6;
    u8 filler8[8];
    struct OamRow *next;
};

struct SpriteState
{
    u8 filler0[0x0A];
    u8 flags0A;
    u8 flags0B;
    u8 flags0C;
    u8 filler0D[0x1C - 0x0D];
    u8 *slotFlags;
    u8 filler20[0x54 - 0x20];
    struct SpriteObj *objs;
    u8 filler58[0x228 - 0x58];
    struct SpriteObj *partsB[2];
    struct SpriteObj *partsA[2];
    struct SpriteNode *nodeFree;
    struct SpriteNode *nodeUsed;
    struct SpriteNode *nodePool;
    u8 filler244[0x248 - 0x244];
    void *display;
    void *camera;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u32 unk4;
    u32 unk8;
    union
    {
        u8 flags0C;
        u32 unkC;
    } u0C;
};

struct SpriteRecord
{
    u16 x;
    u16 y;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u8 flags12;
    u8 filler13[0x1F - 0x13];
    u8 flags1F;
};

struct SpriteObj
{
    u16 x;
    u16 y;
    u8 filler4[0x08 - 0x04];
    struct SpriteRecord *record;
    u8 filler0C[0x0E - 0x0C];
    u16 unkE;
    u8 filler10[0x13 - 0x10];
    u8 flags13;
    u8 filler14[0x16 - 0x14];
    u16 unk16;
    s32 unk18;
    u8 filler1C[0x1E - 0x1C];
    u8 flags1E;
    u8 flags1F;
    u8 filler20[0x21 - 0x20];
    u8 unk21;
    u8 filler22[0x28 - 0x22];
    void *ring;
    u8 filler2C[0x34 - 0x2C];
    struct SpriteObj *activeNext;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler44[0x4C - 0x44];
    void *stateFn;
    u8 filler50[0x58 - 0x50];
    u32 slots58[4];
    u8 filler68[0x76 - 0x68];
    u8 flags76;
    u8 flags77;
};

struct TaskData
{
    u8 filler0[0x04];
    s32 posX;
    s32 posY;
    u8 filler0C[0x16 - 0x0C];
    u16 count;
    s16 posZ;
};

struct Task
{
    struct Task *next;
    void *fn;
    struct SpriteObj *obj;
    struct TaskData *data;
    s16 timer;
};

extern struct SpriteState *gWorld;
extern u8 *gWorkBuf;
extern u8 *gMemRoutines;
extern u16 gSys;
extern u8 *gParty;
void mem_copy_fast(void *dst, const void *src, u32 n);

void sub_08115F54(void *display, u32 kind, u32 x, u32 y, u32 a, u32 b, u32 c, u32 d);
void sub_08117380(void *display);
void sub_0811733C(void *display, u32 value);
void write_screen_entry(void *camera, u32 value);
void obj_bind_slots(void *obj);
void list_remove_active(void *obj);
void list_push_active(void *obj);
void release_obj_131c(void *obj);
void offset_words_by_camera(void *camera, u32 a, u32 b, u32 d, u32 e);
void offset_three_words(void *camera, u32 a, u32 b, u32 d, u32 e);
void write_entry_offsets(void *camera, u32 a, u32 b, u32 d, u32 e, u32 f);
void partner_link_update(void);
void lookup_part(void);
void lookup_link_slot(void);
void sprite_set_action(void *obj, u32 a, u32 b, u32 c);
void place_targets(struct SpriteObj *obj, s32 col, s32 row, s32 z, s32 extra);
void set_step_and_offsets(struct SpriteObj *obj, s32 a, s32 b);
void obj_start_anim(struct SpriteObj *obj);
void oam_write_tiles(u32 tiles, s32 x, s32 y, u32 prio, u32 right);
void setup_obj_10bc(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
void obj_load_record(void *obj, u32 a, u32 b, u32 c, u32 d);
s32 obj_step_z(void *obj);
void task_begin_obj(void *task);
void task_step_z(void);
void task_slide_step(void);
void popup_rise_step(void);
void popup_rise_step_b(void);
u8 step_motion(void *obj);
void effect_spawn_at(u32 kind, s32 x, s32 y, s32 z, void *obj);
void task_finish_anim(void);
void *spawn_obj_by_id(u32 id, u32 a, u32 b, u32 c);
void sprite_entry_update(void *obj);
u32 atan2_angle(s32 x, s32 y);
s32 number_right_edge(s32 a, s32 b);

struct Lo77
{
    u8 lo3 : 3;
    u8 hi5 : 5;
};

struct Lo5
{
    u8 lo5 : 5;
    u8 hi3 : 3;
};

struct Bits13
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

struct Bits0C
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 hi6 : 6;
};

struct Bits76
{
    u8 lo3 : 3;
    u8 mid3 : 3;
    u8 hi2 : 2;
};

struct Bits12
{
    u8 bit0 : 1;
    u8 mid2 : 2;
    u8 hi5 : 5;
};

struct Bits1F
{
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 2;
    u8 f3 : 2;
};

struct NodeDesc
{
    u32 unk0;
    struct SpriteNode *node;
    u32 value;
    u32 unkC;
};

struct DispObj
{
    u8 filler0[0x1C];
    u8 *rows;
    u8 filler20[0x30 - 0x20];
    u16 unk30;
    u8 filler32[0x34 - 0x32];
    u16 unk34;
    u16 unk36;
    u8 filler38[0x4A - 0x38];
    u16 unk4A;
};

struct SysHalves
{
    u16 w0;
    u16 w2;
};

extern u8 gUnk_02000080[];
u8 sub_obj_running(void *obj);
void obj_draw_tile_rows(void *disp, void *rows, void *palette, u32 res, u32 height, u32 p30, u32 zero);

static inline __typeof__(gWorld->display) get_world_display(void)
{
    return gWorld->display;
}

NONMATCH("asm/code_0807ED70/sprite_objects_draw_obj_tiles.s", void sprite_objects_draw_obj_tiles(u32 col, u32 row))
{
    if (gWorld->flags0C & 4)
    {
        if (sub_obj_running(gWorld->display) == 1)
        {
            u16 mark = ((struct DispObj *)gWorld->display)->unk34;

            sub_0811733C(gWorld->display, 1);
            ((struct DispObj *)gWorld->display)->unk34 = mark;
        }
        else
        {
            u8 busy = sub_obj_running(gWorld->display);
            u16 res;

            if (busy != 0)
                return;
            res = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)((row << 8) + ((s32)col / 2), col);
            sub_08115F54(gWorld->display, 4, 0, res, 0xFF, busy, busy, busy);
        }
        ((struct DispObj *)gWorld->display)->unk4A = row;
        {
            struct DispObj *disp = gWorld->display;
            u16 res = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(disp->unk4A << 5, disp->unk34);

            disp = get_world_display();
            obj_draw_tile_rows(disp, disp->rows, gUnk_02000080, res, disp->unk36, disp->unk30, 0);
        }
        ((struct SysHalves *)&gSys)->w2 |= ((struct DispObj *)gWorld->display)->unk36;
        sub_08117380(gWorld->display);
        gWorld->flags0C |= 2;
    }
}
END_NONMATCH

static inline u32 part_key(struct SpriteObj *part)
{
    return part->unk16 | ((((u32)part->flags1E << 28) >> 16) + 0x1000);
}

static inline void part_copy_flags(struct SpriteObj *body, struct SpriteObj *part)
{
    ((struct Bits1F *)&part->flags1F)->f0 = ((struct Bits1F *)&body->record->flags1F)->f0;
    ((struct Bits1F *)&part->flags1F)->f1 = ((struct Bits1F *)&body->record->flags1F)->f1;
    ((struct Bits1F *)&part->flags1F)->f2 = ((struct Bits1F *)&body->record->flags1F)->f2;
    ((struct Bits1F *)&part->flags1F)->f3 = ((struct Bits1F *)&body->record->flags1F)->f3;
    part->unkE = body->record->unkE + 0xFF;
}

NONMATCH("asm/code_0807ED70/set_body_part.s", void set_body_part(struct SpriteObj *body, s32 kind, s32 x, s32 y, s32 step))
{
    struct SpriteObj *partB;
    struct SpriteObj *part;
    s32 sub;
    s32 index;
    s32 dir;
    s32 mode;

    part = gWorld->partsA[*(s16 *)((u8 *)body->ring + 0xEC) + 2];
    if ((kind & 0xFF000000) == 0xFF000000)
    {
        mode = gWorld->flags0A & 0xC0;
        if (mode == 0x80)
            kind &= 0x00FFFFFF;
        else
        {
            kind = -2;
            if (mode == 0x40)
                kind++;
        }
    }
    sub = -1;
    if (kind != -1)
    {
        if (kind == 1)
            sub = 0;
        else if (kind == 2)
            sub = 1;
        if (sub != -1)
        {
            if (part == NULL)
            {
                part = spawn_obj_by_id(0x20B1, sub, 0, 1);
                sprite_entry_update(part);
                part_copy_flags(body, part);
            }
            else if (part->unk21 != sub)
            {
                obj_load_record(part, sub, -1, 0, 0);
                sprite_entry_update(part);
            }
        }
    }
    if (kind != -2)
    {
        part->x = body->record->x;
        part->y = body->record->y;
        gWorld->partsA[*(s16 *)((u8 *)body->ring + 0xEC) + 2] = part;
    }
    if ((gWorld->flags0C & 0x10) == 0)
        return;
    partB = gWorld->partsB[*(s16 *)((u8 *)body->ring + 0xEC) + 2];
    if ((kind & 0xFF000000) == 0xFF000000)
    {
        mode = gWorld->flags0A & 0xC0;
        if (mode == 0x80)
            kind &= 0x00FFFFFF;
        else
        {
            if (mode != 0x40)
                return;
            kind = -1;
        }
    }
    if (kind == -1)
    {
        s32 angle = atan2_angle(x, y);

        dir = ((angle / 16 + 5) / 2) & 7;
        if (partB == NULL)
        {
            partB = spawn_obj_by_id(0x20B2, dir, 0, 1);
            sprite_entry_update(partB);
            partB->unkE = 0;
        }
        else
        {
            if (part_key(partB) != 0x20B2)
            {
                obj_bind_slots(partB);
                ((struct Bits13 *)&partB->flags13)->bit2 = 0;
                ((struct Bits13 *)&partB->flags13)->bit3 = 0;
                setup_obj_10bc(partB, 0, 0x20B2, 1, kind, kind, kind, kind);
                ((struct Bits13 *)&partB->flags13)->bit2 = 1;
                ((struct Bits13 *)&partB->flags13)->bit3 = 1;
                obj_load_record(partB, 0, kind, 0, 0);
                list_remove_active(partB);
            }
            else if (partB->unk21 == dir)
                goto tail_b;
            obj_load_record(partB, dir, kind, 0, 0);
            sprite_entry_update(partB);
        }
    }
    else
    {
        index = 0;
        switch (kind)
        {
        case 0x60:
            index = 0x11;
            break;
        case 0x10:
            index = 0xC;
            break;
        case 1:
            index = 2;
            break;
        case 0:
            index = 0xA;
            break;
        case 2:
            index = 3;
            break;
        case 3:
            index = 0x1A - (step << 1);
            goto add_step;
        case 0x30:
            index = 0x13;
            break;
        case 0x20:
            index = 0xE;
            break;
        case 0x40:
            index = 0xB;
            break;
        case 0x50:
            index = 0xF;
            break;
        case 0xB0:
            index = 0x18;
            break;
        case 0x80:
            index = 0xD;
            break;
        case 0x70:
            index = 0x16;
            break;
        case 0x90:
            index = 0x12;
            break;
        case 0xA0:
            index = 0x14;
            break;
        case 0xD0:
            index = 0x15;
            break;
        case 0xC0:
            index = 0x10;
            break;
        case 0xE0:
            index = 0x17;
            break;
        case 0xF0:
            index = 0x19;
            break;
        }
add_step:
        index += step << 1;
        if (partB == NULL)
        {
            partB = spawn_obj_by_id(0x2083, index, 0, 1);
            sprite_entry_update(partB);
            partB->unkE = 0;
        }
        else if (partB->unk21 != index)
        {
            obj_load_record(partB, index, -1, 0, 0);
            sprite_entry_update(partB);
        }
    }
tail_b:
    partB->x = body->record->x + x;
    partB->y = body->record->y + y;
    gWorld->partsB[*(s16 *)((u8 *)body->ring + 0xEC) + 2] = partB;
}
END_NONMATCH

void obj_free_parts(struct SpriteObj *obj)
{
    struct SpriteObj *part;

    part = gWorld->partsA[*(s16 *)((u8 *)obj->ring + 0xEC) + 2];
    if (part != NULL)
    {
        obj_bind_slots(part);
        ((struct Bits13 *)&part->flags13)->bit2 = 0;
        ((struct Bits13 *)&part->flags13)->bit3 = 0;
        list_push_active(part);
        release_obj_131c(part);
        gWorld->partsA[*(s16 *)((u8 *)obj->ring + 0xEC) + 2] = NULL;
    }
    part = gWorld->partsB[*(s16 *)((u8 *)obj->ring + 0xEC) + 2];
    if (part != NULL)
    {
        obj_bind_slots(part);
        ((struct Bits13 *)&part->flags13)->bit2 = 0;
        ((struct Bits13 *)&part->flags13)->bit3 = 0;
        list_push_active(part);
        release_obj_131c(part);
        gWorld->partsB[*(s16 *)((u8 *)obj->ring + 0xEC) + 2] = NULL;
    }
}

ASM_FUNC("asm/code_0807ED70/body_set_pose.s", void body_set_pose(void))

extern u8 *gParty;
void mem_copy_fast(void *dst, const void *src, u32 n);
ASM_FUNC("asm/code_0807ED70/hero_select_index.s", void hero_select_index(void))

struct ShowRec
{
    u8 filler0[0x10];
    s32 posY;
    u8 filler14[0x18 - 0x14];
    s32 posX;
    u8 filler1C[0xB8 - 0x1C];
    u16 unkB8;
    u8 fillerBA[0xC3 - 0xBA];
    s8 offY;
    u8 fillerC4;
    s8 offX;
};

void obj_stop(void *obj);
void apply_partner_effect(void *a, void *b, u32 strength, s32 c, s32 x, s32 z, s32 d);

s32 sprite_objects_obj_show_at(void *target, struct SpriteObj *obj, u32 arg)
{
    void *ring;
    struct ShowRec *rec;

    rec = (struct ShowRec *)obj;
    ring = obj->ring;
    if (ring == NULL)
        return 0;
    if (obj->record == NULL)
        rec = (struct ShowRec *)((u8 *)ring + 8);
    apply_partner_effect(target, rec, arg, 0, rec->posY / 0x100 + rec->offY,
                         rec->posX / 0x100 + rec->offX, 0);
    rec->unkB8 = 0x193;
    if (*((u8 *)ring + 0x111) & 2)
        return 0;
    if (*(s16 *)((u8 *)ring + 0xF6) != 0)
        return 0;
    obj_stop((u8 *)ring + 8);
    return -1;
}

void display_flush(void)
{
    if (gWorld->flags0C & 2)
    {
        sub_0811733C(gWorld->display, 1);
        ((struct Bits0C *)&gWorld->flags0C)->bit1 = 0;
    }
}

void obj_list_remove(struct SpriteObj *obj)
{
    struct SpriteObj *prev = NULL;
    struct SpriteObj *cur = gWorld->objs;

    while (cur != NULL)
    {
        struct SpriteObj *next = cur->activeNext;
        if (cur == obj)
        {
            if (prev != NULL)
                prev->activeNext = next;
            else
                gWorld->objs = next;
            obj->slots58[0] = 0;
            obj->slots58[1] = 0;
            obj->slots58[2] = 0;
            obj->slots58[3] = 0;
            obj->activeNext = NULL;
            return;
        }
        prev = cur;
        cur = next;
    }
}

void obj_list_push_arg(struct SpriteObj *obj, u32 arg)
{
    if (obj->activeNext == NULL)
    {
        obj->activeNext = gWorld->objs;
        gWorld->objs = obj;
        obj->slots58[0] = (u32)partner_link_update;
        obj->slots58[1] = arg;
        obj->slots58[2] = arg;
        obj->slots58[3] = arg;
        ((struct Lo77 *)&obj->flags77)->lo3 = 0;
    }
}

void obj_list_push_default(struct SpriteObj *obj)
{
    if (obj->activeNext == NULL)
    {
        obj->activeNext = gWorld->objs;
        gWorld->objs = obj;
        obj->slots58[0] = (u32)partner_link_update;
        obj->slots58[1] = (u32)lookup_part;
        obj->slots58[2] = (u32)lookup_link_slot;
        ((struct Lo77 *)&obj->flags77)->lo3 = 0;
    }
}

void obj_start_pose(struct SpriteObj *obj)
{
    sprite_set_action(obj, 0xB, *(s16 *)((u8 *)obj->ring + 0xEC) == -1 ? 0x2039 : 0x2068, 0);
    ((struct Bits12 *)&obj->record->flags12)->mid2 = 1;
    place_targets(obj, obj->posX / 0x100 - 0x18, obj->posY / 0x100 + 6, 0, 0x100);
    set_step_and_offsets(obj, 0x400, 0x10);
    obj->stateFn = (void *)obj_start_anim;
}

void obj_update_pose(struct Task *task)
{
    struct SpriteObj *obj = task->obj;

    if (step_motion(obj) == 0)
    {
        if (*(s16 *)((u8 *)obj->ring + 0xEC) == -1)
            sprite_set_action(obj, 0, 0x2039, 0);
        else
            sprite_set_action(obj, 0, 0x2068, 0);
        ((struct Bits12 *)&obj->record->flags12)->mid2 = 1;
        effect_spawn_at(0x33, obj->posX / 0x100, obj->posY / 0x100, obj->posZ / 0x100, obj);
        task->fn = (void *)task_finish_anim;
    }
}

void obj_step_task(struct Task *task)
{
    struct SpriteObj *obj = task->obj;
    u8 done = obj_step_z(obj);

    if (obj->record->flags12 & 8)
    {
        if (done == 0)
            task_begin_obj(task);
        else
            task->fn = (void *)task_step_z;
    }
}

void obj_timer_task(struct Task *task)
{
    struct SpriteObj *obj = task->obj;
    u16 timer = task->timer;

    if (task->timer > 0)
    {
        s32 t = timer - 1;

        task->timer = t;
        if ((t << 16) == 0)
            ((struct Bits76 *)&obj->flags76)->mid3 = 0;
    }
    else if (obj->posZ >= obj->unk18 && obj->posZ <= 0xC00)
    {
        sprite_set_action(obj, 0, 0, 0);
        task->fn = NULL;
    }
}

void display_pause(void)
{
    sub_08117380(gWorld->display);
}

void display_finish(u32 value)
{
    sub_0811733C(gWorld->display, value);
}

void draw_sprite_block(u32 kind, u32 x, u32 y, u32 w, u32 h)
{
    u16 px = x;
    u16 pw = w;
    u16 ph = h;

    sub_08115F54(gWorld->display, (u8)kind, px, (u16)y, pw, ph, 0, 0);
}

void draw_sprite_small(u32 kind, u32 x, u32 y)
{
    u16 px = x;

    sub_08115F54(gWorld->display, (u8)kind, px, (u16)y, 0xFF, gSys & 0x6000, 0, 0);
}

u16 number_popup_raster(void)
{
    return *(vu16 *)REG_ADDR_DISPSTAT & 2;
}

void raster_irq_default(void)
{
}

NONMATCH("asm/code_0807ED70/oam_write_tiles.s", void oam_write_tiles(u32 tiles, s32 x, s32 y, u32 prio, u32 right))
{
    u8 *base = (u8 *)&gSys;
    u16 *oam;
    u32 attr0 = (y & 0xFF) | 0xFFFF8000;
    u32 shift;

    if (right != 0)
        x = number_right_edge(tiles, x);
    oam = (u16 *)*(u32 *)(base + 0x880);
    shift = prio << 12;
    do
    {
        x = (x - 8) & 0x1FF;
        oam[0] = attr0;
        oam[1] = x;
        oam[2] = (((tiles & 0xF) << 1) + 0x49) | shift;
        oam += 4;
        tiles >>= 4;
    } while (tiles != 0);
    *(u32 *)(base + 0x880) = (u32)oam;
}
END_NONMATCH

void task_anim_countdown(struct Task *task)
{
    struct TaskData *data = task->data;

    {
        int b = gWorld->flags0B;

        b |= 2;
        gWorld->flags0B = b;
    }
    {
        s32 count = data->count - 1;

        data->count = count;
        if ((u16)count == 0)
        {
            data->count = 0x14;
            task->fn = (void *)task_slide_step;
        }
    }
}

void task_draw_step_a(struct Task *task)
{
    struct TaskData *data = task->data;
    struct SpriteObj *obj = task->obj;

    oam_write_tiles((u32)obj, data->posX / 0x100, data->posY / 0x100, data->posZ, 0);
    {
        int b = gWorld->flags0B;

        b |= 2;
        gWorld->flags0B = b;
    }
    {
        s32 count = data->count - 1;

        data->count = count;
        if ((u16)count == 0)
        {
            data->count = 0xE;
            task->fn = (void *)(popup_rise_step_b);
        }
    }
}

void task_draw_step_b(struct Task *task)
{
    struct TaskData *data = task->data;
    struct SpriteObj *obj = task->obj;

    oam_write_tiles((u32)obj, data->posX / 0x100, data->posY / 0x100, data->posZ, 0);
    {
        int b = gWorld->flags0B;

        b |= 2;
        gWorld->flags0B = b;
    }
    {
        s32 count = data->count - 1;

        data->count = count;
        if ((u16)count == 0)
        {
            data->count = 0xE;
            task->fn = (void *)(popup_rise_step);
        }
    }
}

void oam_list_rows_a(struct OamRow *row, s32 count, u32 layer)
{
    u16 *dst;
    u32 layerBits;
    u16 attr1;
    u16 attr2;

    dst = (u16 *)(gWorkBuf + 0x37E8);
    layerBits = layer << 9;
    while (--count != -1)
    {
        attr1 = row->hw0;
        *dst++ = *(u8 *)row - 0x50;
        *dst++ = (attr1 & 0xFF00) | 0x100;
        attr2 = row->hw2;
        *dst++ = (attr2 & 0x1FF) - 0x78;
        *dst++ = (attr2 & 0xC000) | layerBits;
        *dst++ = row->hw4;
        row->hw6 = 1;
        row = row->next;
    }
}

void oam_list_rows_b(struct OamRow *row, s32 count)
{
    u16 *dst;
    u16 v;

    dst = (u16 *)(gWorkBuf + 0x37E8);
    while (--count != -1)
    {
        v = row->hw0;
        *dst++ = (u8)v - 0x50;
        *dst++ = v & 0xFF00;
        v = row->hw2;
        *dst++ = (v & 0x1FF) - 0x78;
        *dst++ = v & 0xFE00;
        *dst++ = row->hw4;
        row->hw6 = 1;
        row = row->next;
    }
}

struct SpriteNode *sprite_node_take_link(struct NodeDesc *desc)
{
    struct SpriteState *st;
    struct SpriteNode *node;

    st = gWorld;
    node = st->nodeFree;
    st->nodeFree = node->next;
    node->next = st->nodeUsed;
    st->nodeUsed = node;
    ((struct Lo5 *)&node->u0C.flags0C)->lo5 = 0;
    desc->node = node;
    node->unk4 = desc->value;
    node->unk8 = (u32)&desc->unkC;
    return node;
}

struct SpriteNode *sprite_node_take(void)
{
    struct SpriteState *st = gWorld;
    struct SpriteNode *node = st->nodeFree;

    st->nodeFree = node->next;
    node->next = st->nodeUsed;
    st->nodeUsed = node;
    ((struct Lo5 *)&node->u0C.flags0C)->lo5 = 0;
    return node;
}

u8 *work_buf_entry_addr(u32 index)
{
    u8 **buf = &gWorkBuf;
    u32 offset = index * 0xD0;
    offset += 0x980;
    return *buf + offset;
}

s32 sprite_node_release(struct SpriteNode *node)
{
    s32 ret;

    if (node == NULL)
        return 0;
    ret = (s32)node->next;
    node->next = gWorld->nodePool;
    gWorld->nodePool = node;
    return ret;
}

struct SpriteNode *sprite_node_alloc(u32 a, u32 b, u32 d)
{
    struct SpriteNode *node = gWorld->nodePool;

    gWorld->nodePool = node->next;
    ((void (*)(u32, struct SpriteNode *, u32))gMemRoutines)(0, node, 0x1C);
    node->unk4 = a;
    node->unk8 = b;
    node->u0C.unkC = d;
    return node;
}

struct SpriteNode *sprite_node_alloc_default(void)
{
    struct SpriteNode *node = gWorld->nodePool;

    gWorld->nodePool = node->next;
    ((void (*)(u32, struct SpriteNode *, u32))gMemRoutines)(0, node, 0x1C);
    return node;
}

void obj_release_node(struct SpriteObj *obj)
{
    if (obj != NULL)
    {
        obj_bind_slots(obj);
        ((struct Bits13 *)&obj->flags13)->bit2 = 0;
        ((struct Bits13 *)&obj->flags13)->bit3 = 0;
        list_push_active(obj);
        release_obj_131c(obj);
    }
}

void obj_detach_active(void *obj)
{
    obj_bind_slots(obj);
    list_remove_active(obj);
}

void obj_setup_load(struct SpriteObj *obj, u32 arg)
{
    obj_bind_slots(obj);
    ((struct Bits13 *)&obj->flags13)->bit2 = 0;
    ((struct Bits13 *)&obj->flags13)->bit3 = 0;
    setup_obj_10bc(obj, 0, arg, 1, -1, -1, -1, -1);
    ((struct Bits13 *)&obj->flags13)->bit2 = 1;
    ((struct Bits13 *)&obj->flags13)->bit3 = 1;
    obj_load_record(obj, 0, -1, 0, 0);
    list_remove_active(obj);
}

void offset_words_camera(u32 a, u32 b, u32 d, u32 e)
{
    offset_words_by_camera(gWorld->camera, a, b, d, e);
}

void offset_three_words_camera(u32 a, u32 b, u32 d, u32 e)
{
    offset_three_words(gWorld->camera, a, b, d, e);
}

void unknown_code_08080474(u32 value)
{
    write_screen_entry(gWorld->camera, value);
}

void write_entry_offsets_camera(u32 a, u32 b, u32 d, u32 e, u32 f)
{
    write_entry_offsets(gWorld->camera, a, b, d, e, f);
}

struct SlotFlags
{
    u8 unk0 : 1;
    s8 kind : 3;
    u8 hi4 : 4;
};

struct SlotGroupFlags
{
    u8 filler0[2];
    u8 flagsA;
    u8 filler3[3];
    u8 flagsB;
    u8 filler7[3];
    u8 flagsC;
    u8 filler0B;
};

struct SlotGroupObjs
{
    u8 *a;
    u8 *b;
    u8 *c;
};

static inline struct SpriteRecord *slot_entry_record(u8 *ent)
{
    return *(struct SpriteRecord **)(ent + 0x10);
}

static inline s32 slot_entry_kind(u8 *ent)
{
    return *(s16 *)(ent + 0xEC);
}

NONMATCH("asm/code_0807ED70/find_slot_by_kind.s", s32 find_slot_by_kind(s32 kind))
{
    struct SlotGroupObjs *objs;
    struct SlotGroupFlags *flags;
    u8 *ent;
    s32 group;
    s32 index;
    struct SpriteState *st;
    u8 *table;

    st = gWorld;
    group = 0;
    table = st->slotFlags;
    objs = (struct SlotGroupObjs *)((u8 *)st + 0x58);
    flags = (struct SlotGroupFlags *)(table + 8);
    do
    {
        if ((u8)((struct SlotFlags *)&flags->flagsA)->kind <= 2)
        {
            ent = objs->a;
            if ((slot_entry_record(ent)->flags12 & 0x20) != 0)
            {
                if (kind < 0)
                    return group;
                if (slot_entry_kind(ent) == kind)
                    return group;
            }
        }
        index = group + 1;
        if ((u8)((struct SlotFlags *)&flags->flagsB)->kind <= 2)
        {
            ent = objs->b;
            if ((slot_entry_record(ent)->flags12 & 0x20) != 0)
            {
                if (kind < 0)
                    return index;
                if (slot_entry_kind(ent) == kind)
                    return index;
            }
        }
        index = group + 2;
        if ((u8)((struct SlotFlags *)&flags->flagsC)->kind <= 2)
        {
            ent = objs->c;
            if ((slot_entry_record(ent)->flags12 & 0x20) != 0)
            {
                if (kind < 0)
                    return index;
                if (slot_entry_kind(ent) == kind)
                    return index;
            }
        }
        objs++;
        flags++;
        group += 3;
    } while (group < 6);
    return -1;
}
END_NONMATCH
