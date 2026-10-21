#include "global.h"

struct SceneObj
{
    u8 filler0[0x18];
    u8 *unk18;
    u8 filler1C[0x3C - 0x1C];
    void *unk3C;
};

struct MapObj
{
    u8 filler0[0x18];
    void *unk18;
    u8 filler1C[0x94 - 0x1C];
    void *unk94;
    void *unk98;
    void *unk9C;
    void *unkA0;
    void *unkA4;
    void *unkA8;
    u8 fillerAC[0xB8 - 0xAC];
    void *unkB8;
};

struct ObjLists
{
    u8 filler0[0x3C];
    u32 root;
};

struct TrackObj
{
    u8 filler0[0x1E];
    u16 selector;
    u8 filler20[0x4E - 0x20];
    u16 posA[2];
    s16 camX;
    u16 posB[2];
    s16 camY;
    u16 dstA[2];
    s16 velX;
    u16 dstB[2];
    s16 velY;
    s16 stepA[2];
    u16 deltaX;
    s16 stepB[2];
    u16 deltaY;
    u16 baseA[2];
    u8 filler76[0x78 - 0x76];
    u16 baseB[2];
    u8 filler7C[0x7E - 0x7C];
    s16 scaleA[2];
    u8 filler82[0x84 - 0x82];
    s16 scaleB[2];
    u8 filler88[0x8A - 0x88];
    u8 flags[2];
    u8 filler8C;
    u8 phase[2];
    u8 filler8F;
    u8 rate[2];
};

struct MapStateScroll
{
    u8 filler0[0xC];
    u16 scroll;
};

extern u8 gSceneChildClass[];
extern u8 gMapSceneClass[];
extern u8 gObjDigitsClass[];
extern u8 gMapObjClass[];
extern void *gUnk_03000DA0;
extern u8 gUnk_02000080[];
void obj_stop_line_scroll(u8 *obj);
void dma_cancel_request(u32 addr);
void heap_free_a(void *p);
void heap_free_b(void *p);
void frame_update(void);
extern u8 gSceneStepClass[];
extern u8 gSys[];
extern struct ObjLists gUnk_0203FFB8;
extern u32 (*gUnk_03000D48)();
extern u32 (*gUnk_03000D4C)();
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void run_pending_obj_callbacks(void);
void obj_list_pass_0a8c(void);
extern u8 gUnk_03000DD0[];
extern u8 gUnk_0203FFF8[];
void set_irq_callback(u32 kind, void *cb);
u8 *ptr_03000ffc_plus_1ff8(void);
u32 eeprom_save_slots(void *rec, u32 mode);
extern const u8 gColumnTable[];
extern s16 gWaveX[];
extern s16 gWaveY[];
void scroll_map_slot(struct TrackObj *obj, u32 slot, s16 a, s16 b);

ASM_FUNC("asm/code_0805799C/oam_draw_unit_markers.s", void oam_draw_unit_markers(void))

NONMATCH("asm/code_0805799C/set_obj_digits.s", u8 *set_obj_digits(u8 *obj, u8 kind, u32 unused2, u32 value, u8 small))
{
    u8 *p;

    task_insert(obj, kind);
    ((struct MapObj *)obj)->unk18 = gObjDigitsClass;
    p = obj + 0x21;
    *p = value % 10;
    value = value / 10;
    p--;
    *p = value % 10;
    value = value / 10;
    obj[0x1F] = value % 10;
    value = value / 10;
    obj[0x1E] = value % 10;
    value = value / 10;
    obj[0x1D] = value % 10;
    value = value / 10;
    obj[0x1C] = value % 10;
    p += 2;
    obj[0x23] = small % 10;
    *p = (u8)(small / 10) % 10;
    return obj;
}
END_NONMATCH

NONMATCH("asm/code_0805799C/map_scene_free.s", void map_scene_free(struct MapObj *obj, u32 kind))
{
    vu16 *dma;
    u8 *sys;
    u16 zero;

    obj->unk18 = gMapSceneClass;
    dma = (vu16 *)REG_ADDR_DMA0SAD;
    dma[5] = dma[5] & 0xC5FF;
    dma[5] = dma[5] & 0x7FFF;
    (void)dma[5];
    set_irq_callback(2, NULL);
    if (obj->unkB8 != NULL)
    {
        task_remove(obj->unkB8, 3);
    }
    heap_free_b(gUnk_03000DA0);
    run_pending_obj_callbacks();
    *(u16 *)gUnk_02000080 = 0x7FFF;
    sys = gSys;
    *(u16 *)(sys + 2) = *(u16 *)(sys + 2) | 1;
    zero = 0;
    *(u16 *)0x02000000 = zero;
    sys[0x31] = 2;
    frame_update();
    task_remove(obj, kind);
}
END_NONMATCH

void flip_oam_buffer(void)
{
    u8 *base = gSys;
    u32 *oamPtr = (u32 *)(base + 0x880);
    u32 oam = *oamPtr;

    obj_list_pass_0a8c();
    gUnk_03000D48(gUnk_0203FFB8.root, gUnk_0203FFB8.root + 0xA14);
    oam = gUnk_03000D4C(gUnk_0203FFB8.root, oam);
    *oamPtr = oam;
}

void scene_step_free(struct SceneObj *obj, u32 kind)
{
    obj->unk18 = gSceneStepClass;
    run_pending_obj_callbacks();
    if (obj->unk3C != NULL)
    {
        task_remove(obj->unk3C, 3);
    }
    task_remove(obj, kind);
}

struct SceneObj *scene_child_init(struct SceneObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->unk18 = gSceneChildClass;
    return obj;
}

void sio_send_callback(void)
{
    REG_SIOMLT_SEND = 0xFEFE;
}

void sio_reset_and_ack(void)
{
    REG_IME = 0;
    REG_IE = REG_IE & 0xFF7F;
    REG_IF = REG_IF | 0x80;
    REG_IME = 1;
    REG_IME = 0;
    REG_SIOCNT = 0x2003;
    REG_IME = 1;
}

void sio_multi_setup(void)
{
    set_irq_callback(1, sio_send_callback);
    REG_IME = 0;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT = REG_SIOCNT | 0x4003;
    REG_SIOMLT_SEND = 0xFEFE;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE = REG_IE | 0x80;
    REG_IF = REG_IF | 0x80;
    REG_IME = 1;
}

u32 check_warm_reset_counter(void)
{
    u8 *rec;
    u8 *fresh;
    u32 saved;
    u8 savedB;
    u32 changed;
    u8 result;

    rec = ptr_03000ffc_plus_1ff8();
    saved = *(u32 *)rec;
    savedB = rec[4];
    changed = 0;
    if (*(u32 *)gUnk_0203FFF8 > saved)
    {
        changed = 1;
        *(u32 *)rec = *(u32 *)gUnk_0203FFF8;
    }
    if (gUnk_0203FFF8[4] > savedB)
    {
        changed = 1;
        rec[4] = gUnk_0203FFF8[4];
    }
    if (changed != 0)
    {
        result = (u8)eeprom_save_slots(rec, 0);
        if (result == 0)
        {
            fresh = ptr_03000ffc_plus_1ff8();
            *(u32 *)fresh = 0x4E20;
            fresh[4] = 3;
            *(u32 *)gUnk_0203FFF8 = 0x4E20;
            gUnk_0203FFF8[4] = fresh[4];
        }
        return result;
    }
    return 1;
}

ASM_FUNC("asm/code_0805799C/map_obj_start.s", void map_obj_start(void))

void obj_release_parts(struct MapObj *obj, u32 arg)
{
    obj->unk18 = gMapObjClass;
    if (((u8 *)obj)[0x584] != 0)
    {
        obj_stop_line_scroll((u8 *)obj);
    }
    dma_cancel_request(0x06000000);
    if (obj->unkA4 != NULL)
    {
        heap_free_a(obj->unkA4);
    }
    if (obj->unkA0 != NULL)
    {
        heap_free_a(obj->unkA0);
    }
    if (obj->unk9C != NULL)
    {
        heap_free_a(obj->unk9C);
    }
    if (obj->unkA8 != NULL)
    {
        heap_free_a(obj->unkA8);
    }
    if (obj->unk98 != NULL)
    {
        heap_free_a(obj->unk98);
    }
    if (obj->unk94 != NULL)
    {
        heap_free_a(obj->unk94);
    }
    task_remove(obj, arg);
}

ASM_FUNC("asm/code_0805799C/init_obj_bg_layers.s", void init_obj_bg_layers(void))

ASM_FUNC("asm/code_0805799C/clamp_camera_to_map.s", void clamp_camera_to_map(void))

ASM_FUNC("asm/code_0805799C/map_obj_slots_obj_state_step.s", void map_obj_slots_obj_state_step(void))

NONMATCH("asm/code_0805799C/offset_obj_pos_by_mode.s", void offset_obj_pos_by_mode(struct TrackObj *obj, u8 kind, s16 dx, s16 dy))
{
    s32 v;
    u16 *pos;

    switch (kind)
    {
    case 0:
        if ((((struct MapStateScroll *)gUnk_03000DD0)->scroll & 0xC000) != 0x8000)
        {
            return;
        }
        pos = obj->baseA;
        break;
    case 1:
        if ((((struct MapStateScroll *)gUnk_03000DD0)->scroll & 0x3000) != 0x2000)
        {
            return;
        }
        pos = &obj->baseA[1];
        break;
    default:
        return;
    }
    v = dx;
    pos[0] = v + pos[0];
    pos += 3;
    v = dy;
    pos[0] = v + pos[0];
}
END_NONMATCH

ASM_FUNC("asm/code_0805799C/obj_slots_update.s", void obj_slots_update(struct TrackObj *obj))

NONMATCH("asm/code_0805799C/obj_slots_motion.s", void obj_slots_motion(struct TrackObj *obj))
{
    u16 i;

    for (i = 0; i < 2; i++)
    {
        u8 *cols;
        u16 entry;
        u16 *dst;
        u16 val;

        cols = (u8 *)gColumnTable;
        entry = *(u16 *)(cols + (((2 - i) << 1) + (obj->selector << 3)));

        if (entry != 0xFFFF)
        {
            u8 *sa;
            u8 *sb;
            s32 jump;
            u32 off;

            jump = obj->flags[i] & 0x80;
            off = i << 1;
            sa = (u8 *)obj->stepA;
            sb = (u8 *)obj->stepB;
            if (jump != 0)
            {
                s32 prodB;
                u8 sum;

                sum = obj->rate[i] + obj->phase[i];
                obj->phase[i] = sum;
                *(s16 *)(sa + off) = gWaveX[obj->flags[i] & 0xF] * (sum >> 2);
                *(s16 *)(sb + off) = prodB = gWaveY[obj->flags[i] & 0xF] * (obj->phase[i] >> 2);
                if (*(s16 *)(sa + off) != 0 || (prodB << 16) != 0)
                {
                    scroll_map_slot(obj, i, *(s16 *)(sa + off), *(s16 *)(sb + off));
                    obj->phase[i] = 0;
                }
            }
            *(s16 *)(sa + off) = 0;
            dst = (u16 *)(sb + off);
            val = 0;
        }
        else
        {
            obj->posA[i] = *(u16 *)&obj->camX;
            obj->posB[i] = *(u16 *)&obj->camY;
            obj->dstA[i] = obj->posA[i];
            dst = &obj->dstB[i];
            val = obj->posB[i];
        }
        *dst = val;
    }
}
END_NONMATCH

void calc_obj_scaled_pos(struct TrackObj *obj)
{
    u16 i;

    for (i = 0; i < 2; i++)
    {
        obj->posA[i] = obj->baseA[i] + ((obj->camX * obj->scaleA[i]) >> 2);
        obj->posB[i] = obj->baseB[i] + ((obj->camY * obj->scaleB[i]) >> 2);
    }
}

ASM_FUNC("asm/code_0805799C/load_map_layout.s", void load_map_layout(void))

ASM_FUNC("asm/code_0805799C/redraw_map_columns.s", void redraw_map_columns(void))

ASM_FUNC("asm/code_0805799C/load_map_tilemap_layers.s", void load_map_tilemap_layers(void))

ASM_FUNC("asm/code_0805799C/draw_metatile_2x2.s", void draw_metatile_2x2(void))

ASM_FUNC("asm/code_0805799C/scroll_map_slot.s", void scroll_map_slot(struct TrackObj *obj, u32 slot, s16 a, s16 b))
