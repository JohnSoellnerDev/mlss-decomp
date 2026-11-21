#include "global.h"
#include "ram_routines.h"

struct Enemy;

struct Sprite
{
    u8 filler0[0x11];
    u8 flags11;
    u8 filler12[0x20 - 0x12];
    u8 unk20;
    u8 unk21;
};

struct Enemy
{
    u8 filler0[0x2];
    u8 heading;
    u8 filler3[0xC - 0x3];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x34 - 0x25];
    s32 unk34;
    s32 unk38;
    u8 filler3C[0x54 - 0x3C];
    u8 unk54;
    u8 filler55[0xF8 - 0x55];
    u16 unkF8;
    u8 fillerFA[0x100 - 0xFA];
    u16 unk100[4];
    u16 unk108[4];
    u16 unk110[4];
    u16 unk118[4];
    u16 unk120[4];
    u16 unk128[4];      /* the waits (frames) to pick from at random after a move; 0: no wait */
    u8 unk130;
    u8 unk131;
    u8 filler132[0x134 - 0x132];
    u8 filler134[0x137 - 0x134];
    u8 unk137;
    s16 unk138;
    s16 unk13A;
    s16 unk13C;
    s16 unk13E;
    u8 filler140[0x198 - 0x140];
    void *unk198;
    u8 filler19C[0x1AC - 0x19C];
    u32 unk1AC;         /* the handler to go on to once the wait (unk1D2) is over */
    u8 filler1B0[0x1B8 - 0x1B0];
    s16 unk1B8;
    s16 unk1BA;
    u16 unk1BC;
    s16 unk1BE;
    s32 unk1C0;
    u16 unk1C4;
    u16 unk1C6;
    u16 unk1C8;
    u8 filler1CA[0x1CC - 0x1CA];
    s32 unk1CC;
    u16 unk1D0;
    u16 unk1D2;         /* wait timer: tick_store counts it down, then goes on to unk1AC */
    u8 filler1D4[0x1D6 - 0x1D4];
    u8 unk1D6;
    u8 filler1D7[0x20C - 0x1D7];
    u8 unk20C;
    u8 unk20D;
    u8 filler20E[0x216 - 0x20E];
    u8 unk216;
    u8 filler217[0x239 - 0x217];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    u16 unk23E;
    u8 filler240[0x242 - 0x240];
    u16 unk242;
    u16 unk244;
    u8 filler246[0x248 - 0x246];
    s32 unk248;
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    s32 unk258;
    s32 unk25C;
    u8 filler260[0x294 - 0x260];
    struct Sprite *sprite;
    u8 filler298[0x2B5 - 0x298];
    u8 unk2B5;
    u8 unk2B6;
    u8 filler2B7[0x2BC - 0x2B7];
    s32 unk2BC;
    s32 unk2C0;
    u8 filler2C4[0x2CC - 0x2C4];
    s32 unk2CC;
    s32 unk2D0;
    u8 filler2D4[0x2D8 - 0x2D4];
    s16 unk2D8;
    s16 unk2DA;
    u8 filler2DC[0x2DE - 0x2DC];
    u16 unk2DE;
    u16 unk2E0;
    u8 filler2E2[0x33C - 0x2E2];
    u8 unk33C;
};

struct Vec3
{
    s32 x;
    s32 y;
    s32 z;
    u8 filler0C[4];
    const void *cb10;
};

struct Mat4
{
    s32 m[16];
    const void *cb40;
};

struct TaskObj
{
    u8 filler0[0x14];
    void *owner14;
    const void *cb18;
};

struct Bit6Byte
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Bit01Byte
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct T3dF;

struct T3dNode
{
    struct T3dNode *prev0;
    struct T3dNode *child4;
    struct T3dNode *child8;
};

struct T3dL
{
    u8 filler0[4];
    struct Vec3 unk4;
    struct Vec3 unk18;
    struct Vec3 unk2C;
    struct Mat4 unk40;
};

struct T3dC
{
    u8 filler0[8];
    struct Vec3 unk8;
    struct Vec3 unk1C;
    struct Vec3 unk30;
    struct Vec3 unk44;
    struct Mat4 unk58;
};

struct Fade
{
    u8 filler0[0x18];
    const void *cb18;
    u8 unk1C;
    u8 filler1D[7];
    struct TaskObj *unk24;
    u8 unk28;  /* the cursor of the two-choice prompt: 0 or 1 (fade_tick) */
    u8 filler29;
    u16 unk2A; /* the BLDY shadow (gUnk_02000054) before fade_start, put back by fade_end */
    u16 unk2C; /* the BLDCNT shadow (gUnk_02000050) before fade_start, put back by fade_end */
    u16 unk2E; /* brightness, 8.8 fixed point: starts at 0x1000 (16) and drops 1/16 a frame */
};

struct T3dF
{
    s32 unk0;
    s32 unk4;
    s32 count8;
    struct T3dNode *root;
    u8 filler10[0x10];
    struct Vec3 unk20;
    struct Vec3 unk34;
    struct Mat4 unk48;
    struct Mat4 unk8C;
    s32 unkD0;
    void *unkD4;
    s32 unkD8;
    void *unkDC;
};

struct T3dModel
{
    struct T3dC *unk0;
    struct T3dL *unk4;
    s32 unk8;
    struct T3dNode *unkC;
    struct Mat4 unk10;
};

extern u16 gUnk_02000050[];
extern u16 gUnk_02000054;
extern u16 gSys[];
void fade_end(struct Fade *obj);
extern const u8 gAppearCallback10Class[];
extern const u8 gAppearCallback40Class[];
extern const u8 gFadeClass[];
extern const u8 gTaskRemoveClass[];
extern const u8 gHeapTagHdrw[];
extern const u8 gHeapTagHasd[];
extern const u8 gHeapTagT3dc[];
extern const u8 gHeapTagT3dl[];
extern const u8 gHeapTagT3df[];

void heap_free_a(void *p);
void heap_free_b(void *p);
void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void mat4_mul(struct Mat4 *out, struct Mat4 *a, struct Mat4 *b);
void mat4_apply_translate(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_apply_rotate_yxz(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_apply_rotate_zxy(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_apply_scale(struct Mat4 *m, s32 x, s32 y, s32 z);
void mat4_set_identity(struct Mat4 *m);
void vec3_zero(struct Vec3 *v);
void vec3_copy(struct Vec3 *dst, struct Vec3 *src);
void tree_walk_children(struct T3dNode *node);
void t3d_node_free(struct T3dNode *node, u32 flags);
struct T3dF *t3d_f_init(struct T3dF *f);
void t3d_node_update(struct T3dNode *node, struct Mat4 *m);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
u8 advance_move_step(void *ctx, struct Enemy *obj);
void obj_step_toward_target_08138FDC(struct Enemy *obj);
void obj_hop_landing_step(struct Enemy *obj);
void obj_set_state(void *obj, u32 state);
u32 mt19937_next(void);
u32 rand_below(u32 n, u32 r);
void tick_store(void);
void tick_fire(void);
void advance_handler_slot(void);
void obj_sync_sub_slot(void *obj, u32 slot);
void obj_set_cell_coord(void *obj, u32 cell, u32 arg);
u8 find_active_entry(void *ctx, u8 *idx);
void obj_start_move_to_target(struct Enemy *obj, u32 mode, s32 x, s32 y);
void obj_move_to_target(void);
void obj_start_move_quarter(struct Enemy *obj, u8 mode, s32 tx, s32 ty);
void obj_move_interp(struct Enemy *ent);
void move_handler_timed_callback(void);
extern const u8 gFrameAttrTable[];

struct SysKeys2A
{
    u8 filler0[0x2A];
    u16 keys;
};

/* One frame of the fade prompt: fades the screen in from black (unk2E, written to the BLDY shadow),
 * moves the two-choice cursor (unk28) with Up and Down, wrapping round, and ends the fade
 * (fade_end) on A with the cursor on choice 1, or on Select. */
void fade_tick(struct Fade *obj)
{
    s32 down;
    u16 newKeys;
    u16 up;
    s32 cursor;
    s8 choice;

    if (obj->unk2E != 0)
    {
        obj->unk2E -= 0x10;
        gUnk_02000054 = *(vu16 *)&obj->unk2E >> 8;
    }
    newKeys = ((struct SysKeys2A *)gSys)->keys;
    up = newKeys & DPAD_UP;
    if (up != 0)
    {
        cursor = obj->unk28 - 1;
        obj->unk28 = cursor;
        if ((cursor << 24) < 0)
        {
            obj->unk28 = 1;
        }
    }
    else
    {
        down = DPAD_DOWN;
        down &= newKeys;
        if (down)
        {
            cursor = obj->unk28 + 1;
            obj->unk28 = cursor;
            if ((s8)cursor > 1)
            {
                // up is 0 here: wrap round to the first choice
                obj->unk28 = up;
            }
        }
    }
    if ((((struct SysKeys2A *)gSys)->keys & A_BUTTON) != 0)
    {
        choice = obj->unk28;
        if (choice != 0)
        {
            if (choice == 1)
            {
                fade_end(obj);
            }
        }
    }
    if ((((struct SysKeys2A *)gSys)->keys & SELECT_BUTTON) != 0)
    {
        fade_end(obj);
    }
}

void task_remove_noop(void)
{
}

void obj_callback_18_task_remove(struct TaskObj *obj, u32 task)
{
    obj->cb18 = gTaskRemoveClass;
    task_remove(obj, task);
}

struct TaskObj *obj_callback_18_task_insert(struct TaskObj *obj, u8 kind, const void *tag)
{
    task_insert(obj, kind);
    obj->cb18 = gTaskRemoveClass;
    return obj;
}

void fade_end(struct Fade *obj)
{
    register vu16 *p asm("r1");

    p = gUnk_02000050;
    *p = obj->unk2C;
    p += 2;
    *p = obj->unk2A;
    if (obj->unk24 != 0)
    {
        obj_callback_18_task_remove(obj->unk24, 3);
    }
    obj->unk24 = 0;
    if (obj != 0)
    {
        obj->cb18 = gFadeClass;
        task_remove(obj, 3);
    }
}

void obj_callback_18_remove_de60(struct TaskObj *obj, u32 task)
{
    obj->cb18 = gFadeClass;
    task_remove(obj, task);
}

struct Fade *fade_start(struct Fade *obj, u8 kind)
{
    struct TaskObj *t;
    register vu16 *p asm("r1");

    task_insert(obj, kind);
    obj->cb18 = gFadeClass;
    obj->unk1C = 0;
    t = obj_callback_18_task_insert(heap_alloc2(0x1C, 0, gHeapTagHdrw, 0), 8, gHeapTagHasd);
    obj->unk24 = t;
    t->owner14 = obj;
    obj->unk28 = 0;
    obj->unk2E = 0x1000;
    p = gUnk_02000050;
    obj->unk2C = *p;
    *p = 0xFF;
    p += 2;
    obj->unk2A = *p;
    *p = 0x10;
    p -= 0x2A;
    *p = 0x1140;
    return obj;
}

s8 get_s8_28(u8 *obj)
{
    return *(s8 *)(obj + 0x28);
}

ASM_FUNC("asm/code_08137510/obj_random_pose.s", void obj_random_pose(void))

/*
 * Enemy state step: moves the enemy on (advance_move_step, and the hop landing while unk2B5 bit 3
 * is set). Once the move is over it picks one of its 4 waits (unk128) at random: a wait takes
 * pose unk137 & 7 and counts down in tick_store; no wait goes straight on to handlers[1].
 * handlers[0] is the enemy's next state handler.
 */
void obj_pick_next_state(void *ctx, struct Enemy *obj, u32 *handlers)
{
    u8 done;
    u32 seed;
    u32 pick;
    u32 next;

    done = 0;
    if (advance_move_step(ctx, obj) == 1)
    {
        obj_step_toward_target_08138FDC(obj);
    }
    else
    {
        u8 stopped = 0;
        if (obj->unk1BE == 0)
        {
            stopped = 1;
        }
        done = stopped;
    }
    if (obj->unk2B5 & 8)
    {
        obj_hop_landing_step(obj);
    }
    else
    {
        done = (done | *(u8 *)((u8 *)obj + 0x1BE)) & 0xFF;
    }
    if (done != 0)
    {
        seed = mt19937_next();
        pick = rand_below(4, seed);
        obj->unk1D2 = obj->unk128[pick];
        if (obj->unk1D2 != 0)
        {
            obj_set_state(obj, ((u32)obj->unk137 << 29) >> 29);
            next = (u32)tick_store;
        }
        else
        {
            next = handlers[1];
        }
        handlers[0] = next;
    }
}

/*
 * Enemy state step: lines the enemy up with the active member of ctx's pair
 * (find_active_entry, else ctx byte 0x208 bit 2). Along the axis its frame
 * attribute picks (kinds 0, 2: x; others: y) it starts moving toward that
 * member and sets the next state to obj_move_to_target (unk1D6 bit 1 = moving).
 * Already level with it: unk1D6 bit 0 = lined up, and the state goes on to
 * out[1].
 */
void obj_find_active_state(void *ctx, struct Enemy *obj, u32 *out)
{
    u8 member;
    struct Enemy *target;
    u8 *base58; /* obj + 0x58; base58 + 0x17E is obj->unk1D6 */
    u8 axis;

    base58 = (u8 *)obj + 0x58;
    if (find_active_entry(ctx, &member) == 0)
    {
        member = ((u32)((u8 *)ctx)[0x208] << 29) >> 31;
    }
    target = (struct Enemy *)((u32 *)((u8 *)ctx + 0x28))[member];
    axis = gFrameAttrTable[(((u32)obj->unk216 << 28) >> 30 << 4) + (((u32)obj->unk24 << 29) >> 29) + 8];
    switch (axis)
    {
    case 0:
    case 2:
        if (obj->posX > target->posX)
        {
            obj_start_move_to_target(obj, 1, 0x80000000, 0); /* left */
        }
        else if (obj->posX < target->posX)
        {
            obj_start_move_to_target(obj, 1, 0xFFFFF, 0); /* right */
        }
        else
        {
            ((struct Bit01Byte *)(base58 + 0x17E))->bit1 = 0;
            ((struct Bit01Byte *)(base58 + 0x17E))->bit0 = 1;
            out[0] = out[1];
            return;
        }
        break;
    case 1:
    default:
        if (obj->posY > target->posY)
        {
            obj_start_move_to_target(obj, 1, 0, 0x80000000); /* up */
        }
        else if (obj->posY >= target->posY)
        {
            ((struct Bit01Byte *)(base58 + 0x17E))->bit1 = 0;
            ((struct Bit01Byte *)(base58 + 0x17E))->bit0 = 1;
            out[0] = out[1];
            return;
        }
        else
        {
            obj_start_move_to_target(obj, 1, 0, 0xFFFFF); /* down */
        }
        break;
    }
    obj_step_toward_target_08138FDC(obj);
    out[0] = (u32)obj_move_to_target;
    ((struct Bit01Byte *)(base58 + 0x17E))->bit1 = 1;
    ((struct Bit01Byte *)(base58 + 0x17E))->bit0 = 0;
}

static inline void set_bit6_flag(struct Sprite *spr, u32 v)
{
    ((struct Bit6Byte *)&spr->flags11)->bit6 = v;
}

NONMATCH("asm/code_08137510/obj_begin_glide.s", void obj_begin_glide(void *ctx, struct Enemy *obj, u32 *out))
{
    struct Sprite *rec;
    u32 keep;
    u32 seed;
    u32 pick;
    u16 value;
    s32 maskClear0;
    s32 maskClear1;

    maskClear0 = -2;
    maskClear1 = -3;
    rec = obj->sprite;
    keep = ((u32)rec->flags11 << 25) >> 31;
    obj_set_state(obj, 1);
    obj_sync_sub_slot(obj, 0);
    obj_set_cell_coord(obj, rec->unk21 + 4, 0);
    set_bit6_flag(rec, keep);
    seed = mt19937_next();
    pick = rand_below(4, seed);
    value = obj->unk128[pick];
    obj->unk1D2 = value;
    if (value != 0)
    {
        out[0] = (u32)tick_fire;
        obj->unk1AC = (u32)advance_handler_slot;
        obj->unk1D6 = (obj->unk1D6 | 2) & maskClear0;
    }
    else
    {
        obj->unk1D6 = (obj->unk1D6 & maskClear1) | 1;
        obj->unkF8 &= 0xFBFF;
    }
}
END_NONMATCH
