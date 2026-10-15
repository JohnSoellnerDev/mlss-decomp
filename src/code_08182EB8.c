#include "global.h"

struct PtRecord
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[0x34 - 0x8];
    u32 model;
};

struct PtObj
{
    u8 filler0[8];
    struct PtRecord *rec;
    u32 *data;
    u8 filler10[2];
    s16 posX;
    s16 posY;
    s16 posZ;
    u8 filler18[3];
    u8 flags;
};

struct PtLocal
{
    s32 x;
    s32 y;
    s32 z;
    u32 model;
    u32 saved;
};

struct ModelCtx
{
    u8 filler0[0x38];
    u32 model;
};

extern struct ModelCtx *gUnk_03000D74;

u32 block_flag_byte(struct PtRecord *rec);
void oam_list_rows_b(u32 model, u32 h);
void offset_words_camera(s32 *x, s32 *y, s32 *z, u32 w);
void oam_place_points(u32 *slot, u32 h, s32 x, s32 y, s32 z, u32 a, u32 b);


ASM_FUNC("asm/code_08182EB8/obj_spring_turn_points_2eb8.s", void obj_spring_turn_points_2eb8(void))


ASM_FUNC("asm/code_08182EB8/obj_spring_turn_points_3120.s", void obj_spring_turn_points_3120(void))


ASM_FUNC("asm/code_08182EB8/obj_spring_turn_points_3388.s", void obj_spring_turn_points_3388(void))


ASM_FUNC("asm/code_08182EB8/obj_spring_turn_points_35f0.s", void obj_spring_turn_points_35f0(void))


void obj_scroll_point_3858(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 offset;
    s32 counter;
    struct PtRecord *rec;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    offset = *pt++;
    counter = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        l.y = *pt++;
        l.z = *pt++;
        l.x = x + offset;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = l.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = l.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    offset += 0x300;
    counter += 8;
    rec->unk4 = counter;
    rec->unk6 = counter;
    pt = (s32 *)obj->data;
    *pt++ = offset;
    *pt = counter;
}


void obj_scroll_bounce_point_3978(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 offset;
    s32 height;
    s32 speed;
    s32 counter;
    struct PtRecord *rec;
    u32 h;
    u32 saved;
    s32 left;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    offset = *pt++;
    height = *pt++;
    speed = *pt++;
    counter = *pt++;
    rec = obj->rec;
    l.model = rec->model;
    h = block_flag_byte(rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.x = x + offset;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = l.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = l.x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    offset += 0x200;
    height += speed;
    speed += 0x80;
    if (height > 0) {
        speed = -0x600;
    }
    counter += 4;
    rec->unk4 = counter;
    rec->unk6 = counter;
    pt = (s32 *)obj->data;
    *pt++ = offset;
    *pt++ = height;
    *pt++ = speed;
    *pt = counter;
}


ASM_FUNC("asm/code_08182EB8/obj_grow_turn_points_3ac0.s", void obj_grow_turn_points_3ac0(void))


ASM_FUNC("asm/code_08182EB8/obj_damped_zoom_turn_points_3d24.s", void obj_damped_zoom_turn_points_3d24(void))


void obj_bounce_point_3fb0(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 height;
    s32 speed;
    s32 bounce;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    bounce = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    height += speed;
    speed += 0x40;
    if (height > 0) {
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        speed = bounce << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt++ = speed;
    *pt = bounce;
}


void obj_bounce_point_40dc(struct PtObj *obj)
{
    struct { s32 x; s32 y; s32 z; u32 model; } l;
    s32 *pt;
    s32 height;
    s32 speed;
    s32 bounce;
    u32 h;
    u32 saved;
    s32 *p1;
    s32 *p2;
    u32 *pslot;
    s32 left;
    s32 x;
    s32 y;
    s32 pos;
    s32 t;

    pt = (s32 *)obj->data;
    height = *pt++;
    speed = *pt++;
    bounce = *pt++;
    l.model = obj->rec->model;
    h = block_flag_byte(obj->rec);
    saved = 0;
    oam_list_rows_b(l.model, h);
    p1 = &l.y;
    p2 = &l.z;
    pslot = &l.model;
    for (left = 0; left < 1; left++) {
        x = *pt++;
        l.x = x;
        y = *pt++;
        l.y = y;
        l.z = *pt++;
        l.y = y + height;
        if (obj->flags & 0x80) {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos - t;
        } else {
            pos = obj->posX;
            t = x;
            if (t < 0) {
                t += 0xFF;
            }
            t >>= 8;
            l.x = pos + t;
        }
        pos = obj->posY;
        t = l.y;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.y = pos + t;
        pos = obj->posZ;
        t = l.z;
        if (t < 0) {
            t += 0xFF;
        }
        t >>= 8;
        l.z = pos + t;
        offset_words_camera(&l.x, p1, p2, 0);
        oam_place_points(pslot, h, l.x, l.y, l.z << 14, 0, 0);
        if (!saved) {
            l.model = gUnk_03000D74->model;
            saved = 1;
        }
    }
    if (saved) {
        gUnk_03000D74->model = l.model;
    }
    height += speed;
    speed += 0x40;
    if (height > 0) {
        height = 0;
        bounce++;
        if (bounce > 0) {
            bounce = 0;
        }
        speed = bounce << 8;
    }
    pt = (s32 *)obj->data;
    *pt++ = height;
    *pt++ = speed;
    *pt = bounce;
}


ASM_FUNC("asm/code_08182EB8/obj_bounce_zoom_point_a.s", void obj_bounce_zoom_point_a(void))


ASM_FUNC("asm/code_08182EB8/obj_bounce_zoom_point_b.s", void obj_bounce_zoom_point_b(void))


ASM_FUNC("asm/code_08182EB8/obj_bounce_zoom_model10.s", void obj_bounce_zoom_model10(void))

