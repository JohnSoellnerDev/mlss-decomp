#include "global.h"

struct SubBits12
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct SubObj
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[0xE - 8];
    u16 unkE;
    u8 filler10[2];
    u8 unk12;
};

struct SpriteObj
{
    u8 filler0[0x10];
    struct SubObj *unk10;
    u8 filler14[4];
    u32 unk18;
    u32 unk1C;
    u8 filler20[0x56 - 0x20];
    u16 unk56;
    u16 unk58;
};

struct CounterGroup
{
    s32 unk0;
    s32 unk4;
    s32 unk8;
    struct SpriteObj *unkC;
    struct SpriteObj *unk10;
};

struct VtA
{
    u8 filler0[8];
    s16 thisOff;
    u16 filler_a;
    void (*fn)(void *, u32);
};

struct VtB
{
    u8 filler0[0x10];
    s16 thisOff;
    u16 filler_b;
    void (*fn)(void *, u32);
};

struct ObjA
{
    u8 filler0[0xC];
    struct VtA *vt;
};

struct ObjB
{
    u8 filler0[0x18];
    struct VtB *vt;
};

struct HolderObj
{
    u8 filler0[0xC];
    struct ObjA *unkC;
    struct ObjA *unk10;
};

struct DestroyObj
{
    u8 filler0[0x14];
    void *unk14;
    void *unk18;
    struct ObjA *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    struct ObjB *unk2C;
    void *unk30;
    u8 filler34[4];
    void *unk38;
    struct HolderObj *unk3C;
    u8 filler40[0x6C - 0x40];
    void *unk6C;
    u32 unk70;
};

struct ProjObj
{
    u8 filler0[0xC];
    void *unkC;
    struct SubObj *unk10;
    u8 filler14[0x6C - 0x14];
    u32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u32 unk7C;
    s32 unk80;
    s32 unk84;
    u16 unk88;
    u16 unk8A;
    u16 unk8C;
    u16 unk8E;
    u16 unk90;
    u8 filler92[6];
    u16 unk98;
};

struct PosXY
{
    s32 x;
    u8 filler4[4];
    s32 y;
};

struct GridPart
{
    u8 filler0[0x18];
    struct PosXY pos18;
    u8 filler24[0x6C - 0x24];
    u16 flags6C;
    s16 ctr6E;
    u8 filler70[4];
    s32 unk74;
    s32 unk78;
};

struct GridLinesObj
{
    struct GridPart *unk0;
    struct GridPart *unk4;
    u8 filler8[0xC];
    struct PosXY pos14;
    struct PosXY vel20;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 filler38[0x50 - 0x38];
    s32 unk50;
};

extern s16 gSineTable[];
extern s16 gCosineTable[];
extern const u8 gClassEbb8[];
u32 rand_scaled(u32 count, u32 base);
u32 arc_tan2(s32 x, s32 y);
void obj_set_fields_18(void *obj, s32 x, s32 y, s32 z);
extern const u8 gClassEba8[];
extern const u8 gObjScript08217b64[];
typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
void obj_setup_slot(void *obj, u32 a, u32 b, u32 c, s32 offset, ByteArg flag);
void obj_set_table_44(void *obj, const void *table);
void triple_set(void *dst, s32 a, s32 b, s32 c);
extern u16 gUnk_03000374[];
extern const s16 gKeyStepTable[];
extern const u8 gHeapTagProjectile[];
struct ProjObj *obj_init_projectile(struct ProjObj *obj, struct PosXY *src, struct PosXY *dst, u32 mode);
void list_insert(void *list, void *node);
void obj_set_speed_from(void *obj, void *other);
void obj_script_step(void *obj);
void obj_place_pair(void *obj, u32 mode);
void obj_clamp_pos(void *region, void *a, void *b);
void obj_copy_mode_fields(void *obj);

struct StateObj
{
    u8 filler0[0x18];
    struct VtB *unk18;
    void *unk1C;
    void *unk20;
    void *unk24;
    u8 filler28[8];
    void *unk30;
    u8 filler34[0x70 - 0x34];
    s32 unk70;
    u8 filler74[0x474 - 0x74];
    u32 unk474;
    u8 filler478[0x484 - 0x478];
    u16 unk484;
};

extern u16 gUnk_02000050[];
void palette_fill(u32 v);
void slot_reset(u32 id);
void set_slot_target(u32 a, u32 b);
void write_var(u16 id, u16 value);
u8 timer_tick(void *timer);
void obj_copy_pos_to_shadow(void *src);
void grid_lines_obj_step_motion(struct GridLinesObj *obj, void *region, void *list);
void child_states_obj_run_stage(void *obj);
void obj_run_phases(void *obj);
void obj_run_substates(void *obj);
void child_states_obj_state4_if_flag8(void *obj);
void obj_update_children(void *obj);


extern const u8 gClassEb68[];
void set_irq_callback(u32 kind, void *cb);
void free_if_bit0(void *ptr, u32 flags);
void free_if_bit0_b(void *ptr, u32 flags);
void heap_free_b(void *p);
void obj_run_children_free(void *obj, u32 flags);
void obj_run_children3_free(void *obj, u32 flags);
void window_destroy(void *win, u32 flags);
void obj_run_callbacks(void);
void set_flags_2_3(void *p);
void task_remove(void *node, u32 kind);

extern const u8 gHeapTagProjectile[];
extern const u8 gHeapTagCounterB[];
extern const s16 gCounterSpriteRecordByLang[];
extern u8 gSys[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *obj_init_big(void *obj, u32 first);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_counter_step(struct CounterGroup *group);
void obj_copy_mode_fields(void *obj);
void update_obj_sprite_pos(void *obj, u32 camX, u32 camY);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
s32 lerp_step_08164EE0(s32 a, s32 b, s32 steps, s32 t);

struct NodeSprite
{
    u8 filler0[4];
    u16 unk4;
    u16 unk6;
    u8 filler8[4];
    u16 unkC;
    u8 fillerE[4];
    u8 unk12;
};

struct NodeAttr
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 b3 : 1;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct NodeOwner
{
    u8 filler0[0x14];
    s32 unk14;
    u8 filler18[4];
    s32 unk1C;
    u8 filler20[0x50 - 0x20];
    u32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
};

struct NodeObj
{
    u32 unk0;
    u8 filler4[8];
    struct VtA *unkC;
    struct NodeSprite *unk10;
    u8 filler14[4];
    struct PosXY pos18;
    u8 filler24[0x6C - 0x24];
    s32 unk6C;
    u8 filler70[4];
    s32 unk74;
    s32 unk78;
    u32 unk7C;
    s32 unk80;
    s32 unk84;
    u16 unk88;
    u16 unk8A;
    s16 unk8C;
    u16 unk8E;
    u8 filler90[2];
    u16 unk92;
    void *unk94;
    u8 filler96[2];
    u16 unk98;
};

extern const u8 gHeapTagRingShotSub[];
u8 obj_is_done(void *obj);
void set_slot_value(u32 id, s32 value);
u32 smoothstep_lerp(s32 start, s32 end, s32 steps, s32 step);
void obj_release_sprites(void *obj);
void obj_update_sprites(void *obj);
void child_states_obj_orbit_step(void *obj);
void obj_list_unlink(void *list, void *obj);
void *obj_init_sprite_offset(void *obj, void *src, s32 dx, s32 dz);


NONMATCH("asm/code_08169710/grid_lines_obj_step_motion.s", void grid_lines_obj_step_motion(struct GridLinesObj *obj, void *region, void *list))
{
    u16 *keys = gUnk_03000374;
    u32 flag;
    s32 stepY;
    u32 angleDelta;
    s32 stepX;
    u32 idx;
    u32 low;
    u16 pressed;
    struct ProjObj *proj;
    struct GridPart *childA;
    struct GridPart *childB;
    s32 sinSpeed;
    s32 cosSpeed;

    flag = 0;
    angleDelta = 0;
    stepY = 0;
    stepX = 0;
    if (obj->unk50 == 0)
    {
        if ((keys[0] & 0xF0) != 0)
        {
            idx = (keys[0] >> 4) & 0xF;
            stepX = *(s16 *)((u8 *)gKeyStepTable + (idx << 2));
            stepY = *(s16 *)((u8 *)(gKeyStepTable + 1) + (idx << 2));
        }
        if (obj->unk0->ctr6E + obj->unk4->ctr6E > 0)
        {
            if (obj->unk0->ctr6E != 0)
            {
                angleDelta = 4;
            }
            if (obj->unk4->ctr6E != 0)
            {
                angleDelta -= 4;
            }
        }
        else
        {
            if ((keys[0] & 2) != 0)
            {
                angleDelta = 2;
            }
            if ((keys[0] & 1) != 0)
            {
                angleDelta -= 2;
            }
        }
        if (stepX != 0 || stepY != 0 || angleDelta != 0)
        {
            flag = 1;
        }
        pressed = keys[1];
        low = 0x100;
        low &= pressed;
        if (low != 0 && (obj->unk0->flags6C & 1))
        {
            proj = obj_init_projectile(heap_alloc2(0x84, 0, gHeapTagProjectile, 0), &obj->unk0->pos18, &obj->pos14, 1);
            list_insert(list, proj);
            obj_set_speed_from(proj, obj->unk0);
        }
        else
        {
            low = 0x200;
            low &= pressed;
            if (low != 0 && (obj->unk4->flags6C & 1))
            {
                proj = obj_init_projectile(heap_alloc2(0x84, 0, gHeapTagProjectile, 0), &obj->unk4->pos18, &obj->pos14, 1);
                list_insert(list, proj);
                obj_set_speed_from(proj, obj->unk4);
            }
        }
    }
    obj->unk2C = (obj->unk2C + angleDelta) & 0xFF;
    childA = obj->unk0;
    childB = obj->unk4;
    sinSpeed = gSineTable[obj->unk2C & 0xFF] / 0x40;
    obj->unk30 = sinSpeed;
    childB->unk74 = sinSpeed;
    childA->unk74 = sinSpeed;
    cosSpeed = gCosineTable[obj->unk2C & 0xFF] / 0x40;
    obj->unk34 = cosSpeed;
    childB->unk78 = cosSpeed;
    childA->unk78 = cosSpeed;
    obj->vel20.x = stepX;
    obj->vel20.y = stepY;
    obj_clamp_pos(region, &obj->pos14, &obj->vel20);
    obj_copy_mode_fields(obj->unk0);
    obj_script_step(obj->unk0);
    obj_copy_mode_fields(obj->unk4);
    obj_script_step(obj->unk4);
    obj_place_pair(obj, flag);
}
END_NONMATCH

struct ProjObj *obj_init_projectile(struct ProjObj *obj, struct PosXY *src, struct PosXY *dst, u32 mode)
{
    s32 dx;
    s32 dy;
    u32 angle;
    s32 sinV;
    s32 cosV;

    obj_init_big(obj, 5);
    obj->unkC = (void *)gClassEbb8;
    dx = (s32)((u32)(dst->x - src->x) << 8) >> 16;
    dy = (s32)((u32)(dst->y - src->y) << 8) >> 16;
    angle = ((u32)arc_tan2(dx, dy) << 16) >> 24;
    obj->unk7C = angle;
    sinV = gSineTable[angle] / 0x40;
    cosV = gCosineTable[angle] / 0x40;
    obj->unk74 = 0;
    obj->unk80 = 0x78;
    if (mode != 0)
    {
        obj_create_sprite_10(obj, 4, 0x5006, 0, 7, 2);
        obj->unk6C = 1;
        obj->unk70 = (cosV * 9) >> 3;
        obj->unk78 = (sinV * 9) >> 3;
        obj_set_fields_18(obj, src->x + obj->unk70, 0, src->y + obj->unk78);
    }
    else
    {
        obj_create_sprite_10(obj, 4, 0x5006, 0, 9, 2);
        obj->unk6C = mode;
        obj->unk70 = (cosV * 0xD0) >> 8;
        obj->unk78 = (sinV * 0xD0) >> 8;
    }
    return obj;
}

ASM_FUNC("asm/code_08169710/obj_line_bounce_step.s", void obj_line_bounce_step(void))

ASM_FUNC("asm/code_08169710/obj_check_contact.s", void obj_check_contact(void))
struct ProjObj *obj_init_bullet(struct ProjObj *obj, u32 base, u32 offset, u32 angle, s32 speedA, s32 speedB, s32 extra)
{
    u8 one = 1;
    s32 slotX;
    s32 slotY;
    s32 posX;
    s32 posY;
    struct SubObj *sub;

    obj_init_big(obj, 3);
    obj->unkC = (void *)gClassEba8;
    obj_create_sprite_10(obj, 4, 0x5006, 0, 5, 2);
    ((struct SubBits12 *)&obj->unk10->unk12)->bit4 = 1;
    sub = obj->unk10;
    sub->unk6 = 0x200;
    sub->unk4 = 0x200;
    obj->unk8E = 0;
    obj->unk8A = 0;
    obj->unk7C = angle;
    obj->unk78 = angle;
    obj->unk80 = speedA;
    obj->unk84 = speedB;
    obj->unk90 = 0;
    obj->unk98 = 0;
    obj->unk88 = rand_scaled(0, 0x80);
    triple_set(&obj->unk6C, base, 0, offset);
    posX = base + (((gSineTable[angle & 0xFF] / 0x40) * speedA) << 1);
    posY = (((gCosineTable[angle & 0xFF] / 0x40) * speedB) << 1) + offset;
    obj_set_fields_18(obj, posX, 0, posY);
    slotX = base + (gSineTable[angle & 0xFF] / 0x40) * speedA;
    slotY = (gCosineTable[angle & 0xFF] / 0x40) * speedB + offset;
    obj_setup_slot(obj, slotX, 0, slotY, extra + 0x3C, one);
    obj_set_table_44(obj, gObjScript08217b64);
    obj->unk8C = 0;
    return obj;
}

NONMATCH("asm/code_08169710/obj_node_update.s", u32 obj_node_update(struct NodeObj *obj, struct NodeOwner *owner, u32 turn, void *list))
{
    struct ProjObj *proj;
    struct NodeSprite *sprite;
    struct VtA *cls;
    void *sub;
    void *spawned;
    void *projMem;
    u32 mode;
    u32 idx;
    u32 angle;
    s32 sinV;
    s32 cosV;
    s32 dx;
    s32 dz;
    s32 slotX;
    s32 slotZ;
    s32 scale;
    u16 steps;
    u16 count;
    u8 zeroFlag = 0;
    u8 oneFlag = 1;

    angle = (turn + obj->unk78) & 0xFF;
    obj->unk7C = angle;
    obj_copy_mode_fields(obj);
    obj_script_step(obj);
    switch (obj->unk8C)
    {
    case 0:
        if ((u8)obj_is_done(obj) != 0)
        {
            set_slot_value(0x120, -1);
            obj->unk0 = 3;
            obj->unk8C = 1;
            sprite = obj->unk10;
            sprite->unk6 = 0x100;
            sprite->unk4 = 0x100;
        }
        else
        {
            scale = smoothstep_lerp(0x200, 0x100, 0x3C, (s16)obj->unk8E);
            sprite = obj->unk10;
            sprite->unk6 = scale;
            sprite->unk4 = scale;
            obj->unk8E++;
        }
        break;
    case 1:
        mode = owner->unk50;
        if (mode == 2)
        {
            obj->unk8C = 6;
            obj->unk0 = 4;
        }
        else if (mode != 3)
        {
            obj->unk88++;
            child_states_obj_orbit_step(obj);
        }
        break;
    case 4:
        mode = owner->unk50;
        if (mode == 2)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
            {
                cls = obj->unkC;
                cls->fn((u8 *)obj + cls->thisOff, 3);
            }
            return 0;
        }
        if (mode != 3)
        {
            child_states_obj_orbit_step(obj);
        }
        count = obj->unk8E + 1;
        obj->unk8E = count;
        if ((s16)count > 29)
        {
            projMem = heap_alloc2(0x84, 0, gHeapTagProjectile, 0);
            proj = obj_init_projectile(projMem, &obj->pos18, obj->unk94, obj->unk92);
            list_insert(list, proj);
            if (obj->unk98 > 1)
            {
                obj_set_fields_18(proj, obj->pos18.x + (proj->unk70 << 3), 0, obj->pos18.y + (proj->unk78 << 3));
                obj_load_record(obj->unk10, 0x11, 0, 0, 0);
            }
            else
            {
                obj_set_fields_18(proj, obj->pos18.x + (proj->unk70 << 4), 0, obj->pos18.y + (proj->unk78 << 4));
                obj_load_record(obj->unk10, 5, 0, 0, 0);
            }
            ((struct NodeAttr *)&obj->unk10->unk12)->mode = 1;
            obj->unk8C = 5;
        }
        break;
    case 5:
        mode = owner->unk50;
        if (mode == 2)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
            {
                cls = obj->unkC;
                cls->fn((u8 *)obj + cls->thisOff, 3);
            }
            return 0;
        }
        if (mode != 3)
        {
            child_states_obj_orbit_step(obj);
        }
        if (((struct NodeAttr *)&obj->unk10->unk12)->bit4 != 0)
        {
            obj->unk88 = rand_scaled(0, 0x80);
            if (obj->unk98 > 1)
            {
                obj_load_record(obj->unk10, 0x11, 0, 0, 0);
            }
            else
            {
                obj_load_record(obj->unk10, 5, 0, 0, 0);
            }
            ((struct NodeAttr *)&obj->unk10->unk12)->bit4 = 1;
            obj->unk8C = 1;
        }
        break;
    case 2:
        set_slot_value(0x40, -1);
        owner->unk5C = -owner->unk5C;
        owner->unk54++;
        owner->unk58++;
        if (obj->unk98 > 1)
        {
            obj_load_record(obj->unk10, 0x12, 0, 0, 0);
        }
        else
        {
            obj_load_record(obj->unk10, 6, 0, 0, 0);
        }
        idx = obj->unk7C & 0xFF;
        sinV = gSineTable[idx];
        if (sinV < 0)
        {
            sinV += 0x3F;
        }
        slotX = obj->unk6C + ((((sinV >> 6)) * obj->unk80) << 1);
        cosV = gCosineTable[idx];
        if (cosV < 0)
        {
            cosV += 0x3F;
        }
        slotZ = obj->unk74 + ((((cosV >> 6)) * obj->unk84) << 1);
        obj_setup_slot(obj, slotX, 0, slotZ, 0x3C, zeroFlag);
        obj->unk8C = 3;
        /* fall through */
    case 3:
        steps = obj->unk8A + 1;
        obj->unk8A = steps;
        if ((s16)steps > 59)
        {
            obj->unk8E = 0;
            obj_release_sprites(obj);
            obj->unk8C = 7;
            break;
        }
        if ((steps & 7) == 1)
        {
            sub = heap_alloc2(0x6C, 1, gHeapTagRingShotSub, 0);
            idx = obj->unk7C & 0xFF;
            sinV = gSineTable[idx];
            if (sinV < 0)
            {
                sinV += 0x3F;
            }
            dx = (sinV >> 6) << 3;
            cosV = gCosineTable[idx];
            if (cosV < 0)
            {
                cosV += 0x3F;
            }
            dz = (cosV >> 6) << 3;
            spawned = obj_init_sprite_offset(sub, &obj->pos18, dx, dz);
            list_insert(list, spawned);
        }
        break;
    case 6:
        steps = obj->unk8A + 1;
        obj->unk8A = steps;
        if ((s16)steps > 59)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
            {
                cls = obj->unkC;
                cls->fn((u8 *)obj + cls->thisOff, 3);
            }
            return 0;
        }
        scale = smoothstep_lerp(0x100, 0x10, 0x3C, (s16)obj->unk8A);
        sprite = obj->unk10;
        sprite->unk6 = scale;
        sprite->unk4 = scale;
        break;
    case 7:
        mode = owner->unk50;
        if (mode == 2)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
            {
                cls = obj->unkC;
                cls->fn((u8 *)obj + cls->thisOff, 3);
            }
            return 0;
        }
        count = obj->unk8E + 1;
        obj->unk8E = count;
        if ((s16)count > 119)
        {
            obj->unk8A = 0;
            obj->unk8E = 0;
            obj->unk8C = 8;
        }
        break;
    case 8:
        mode = owner->unk50;
        if (mode == 2)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
            {
                cls = obj->unkC;
                cls->fn((u8 *)obj + cls->thisOff, 3);
            }
            return 0;
        }
        obj->unk88 = rand_scaled(0, 0x80);
        obj_update_sprites(obj);
        if (obj->unk98 > 1)
        {
            obj_load_record(obj->unk10, 0x11, 0, 0, 0);
        }
        else
        {
            obj_load_record(obj->unk10, 5, 0, 0, 0);
        }
        ((struct NodeAttr *)&obj->unk10->unk12)->bit4 = 1;
        idx = obj->unk7C & 0xFF;
        sinV = gSineTable[idx];
        if (sinV < 0)
        {
            sinV += 0x3F;
        }
        slotX = obj->unk6C + ((((sinV >> 6)) * obj->unk80) << 1);
        cosV = gCosineTable[idx];
        if (cosV < 0)
        {
            cosV += 0x3F;
        }
        slotZ = obj->unk74 + ((((cosV >> 6)) * obj->unk84) << 1);
        obj_set_fields_18(obj, slotX, 0, slotZ);
        idx = obj->unk7C & 0xFF;
        sinV = gSineTable[idx];
        if (sinV < 0)
        {
            sinV += 0x3F;
        }
        slotX = obj->unk6C + (sinV >> 6) * obj->unk80;
        cosV = gCosineTable[idx];
        if (cosV < 0)
        {
            cosV += 0x3F;
        }
        slotZ = obj->unk74 + (cosV >> 6) * obj->unk84;
        obj_setup_slot(obj, slotX, 0, slotZ, 0x3C, oneFlag);
        obj->unk8C = 0;
        break;
    default:
        break;
    }
    obj->unk10->unkC = arc_tan2((s32)((u32)(owner->unk1C - obj->pos18.y) << 8) >> 16,
                                (s32)((u32)(owner->unk14 - obj->pos18.x) << 8) >> 16);
    return obj->unk8C == 1;
}
END_NONMATCH

struct CounterGroup *obj_init_counter(struct CounterGroup *group)
{
    group->unkC = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagProjectile, 0), 0);
    obj_create_sprite_10(group->unkC, 4, 0x5006, 0, 0xE, 2);
    group->unkC->unk1C = 0xA000;
    group->unk10 = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagCounterB, 0), 0);
    obj_create_sprite_10(group->unk10, 4, 0x5006, 0, gCounterSpriteRecordByLang[gSys[0x8C0]], 2);
    group->unk10->unk18 = 0xE000;
    group->unk10->unk1C = 0xA000;
    group->unk8 = 0;
    group->unk4 = 0;
    group->unk0 = 0x1C20;
    obj_counter_step(group);
    return group;
}

void obj_counter_step(struct CounterGroup *group)
{
    s32 rem;
    s32 cur;
    u16 *modeAddr;
    u16 mode;

    obj_copy_mode_fields(group->unkC);
    cur = group->unk4;
    if (cur < group->unk0)
    {
        rem = group->unk8;
        if (rem != 0)
        {
            group->unk8 = rem - 1;
            group->unk4 = cur + 4;
            modeAddr = &group->unkC->unk56;
            rem = 1;
        }
        else
        {
            group->unk4 = cur + 1;
            modeAddr = &group->unkC->unk56;
        }
        *modeAddr = rem;
        group->unkC->unk18 = lerp_step_08164EE0(0x1000, 0xE000, group->unk0, group->unk4);
    }
    else
    {
        group->unkC->unk56 = 0;
    }
    if (group->unkC->unk56 != group->unkC->unk58)
    {
        mode = group->unkC->unk56;
        switch (mode)
        {
        case 0:
            obj_load_record(group->unkC->unk10, 0xE, 0, 0, mode);
            break;
        case 1:
            obj_load_record(group->unkC->unk10, 0x14, 0, 0, 0);
            break;
        }
    }
    update_obj_sprite_pos(group->unkC, 0, 0);
    group->unkC->unk10->unkE = 0;
    update_obj_sprite_pos(group->unk10, 0, 0);
    group->unk10->unk10->unkE = 4;
}

ASM_FUNC("asm/code_08169710/obj_clamp_pos.s", void obj_clamp_pos(void *region, void *a, void *b))

ASM_FUNC("asm/code_08169710/obj_init_screen.s", void obj_init_screen(void))

void obj_destroy_all(struct DestroyObj *obj, u32 freeMem)
{
    struct HolderObj *holder;
    struct ObjA *sub;
    struct ObjB *subB;

    obj->unk18 = (void *)gClassEb68;
    set_irq_callback(2, 0);
    REG_DMA0CNT_H = 0;
    if (obj->unk30 != 0)
    {
        free_if_bit0(obj->unk30, 3);
    }
    if (obj->unk20 != 0)
    {
        heap_free_b(obj->unk20);
    }
    if (obj->unk6C != 0)
    {
        obj_run_children_free(obj->unk6C, 3);
    }
    holder = obj->unk3C;
    if (holder != 0)
    {
        if (holder->unk10 != 0)
        {
            holder->unk10->vt->fn((u8 *)holder->unk10 + holder->unk10->vt->thisOff, 3);
        }
        if (holder->unkC != 0)
        {
            holder->unkC->vt->fn((u8 *)holder->unkC + holder->unkC->vt->thisOff, 3);
        }
        heap_free_b(holder);
    }
    if (obj->unk24 != 0)
    {
        heap_free_b(obj->unk24);
    }
    if (obj->unk28 != 0)
    {
        obj_run_children3_free(obj->unk28, 3);
    }
    subB = obj->unk2C;
    if (subB != 0)
    {
        subB->vt->fn((u8 *)subB + subB->vt->thisOff, 3);
    }
    if (obj->unk38 != 0)
    {
        window_destroy(obj->unk38, 3);
    }
    sub = obj->unk1C;
    if (sub != 0)
    {
        sub->vt->fn((u8 *)sub + sub->vt->thisOff, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(obj->unk14);
    free_if_bit0_b(&obj->unk70, 2);
    task_remove(obj, freeMem);
}

NONMATCH("asm/code_08169710/child_states_obj_run_state.s", void child_states_obj_run_state(struct StateObj *obj))
{
    rand_scaled(0, 2);
    switch (obj->unk474)
    {
    case 0:
        {
            u8 *slot = gSys;
            *(s32 *)(slot + 0x884) = -1;
        }
        set_slot_target(0, 3);
        obj->unk474 = 1;
    case 1:
        if (timer_tick(&obj->unk70))
        {
            obj->unk474 = 2;
        }
        else
        {
            grid_lines_obj_step_motion(obj->unk20, obj->unk24, obj->unk1C);
        }
        break;
    case 2:
        child_states_obj_run_stage(obj);
        break;
    case 3:
        obj_run_phases(obj);
        break;
    case 4:
        obj_run_substates(obj);
        break;
    case 5:
        child_states_obj_state4_if_flag8(obj);
        break;
    case 6:
        if (timer_tick(&obj->unk70))
        {
            obj->unk474 = 7;
        }
        break;
    case 7:
        palette_fill(0);
        slot_reset(0);
        write_var(0x1CE1, obj->unk484);
        {
            u16 *flag = gUnk_02000050;
            *flag = 0;
            flag += 2;
            *flag = 0;
        }
        if (obj != 0)
        {
            obj->unk18->fn((u8 *)obj + obj->unk18->thisOff, 3);
        }
        return;
    }
    obj_copy_pos_to_shadow(obj->unk30);
    obj_update_children(obj);
}
END_NONMATCH
