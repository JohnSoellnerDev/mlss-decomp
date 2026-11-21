#include "global.h"

struct TaskObj
{
    u8 filler0[0x18];
    const void *cb18;
};

extern const u8 gEntryUpdateTaskClass[];
extern const u8 gObjUpdateOamTaskClass[];

struct FieldSub
{
    s16 posX;
    s16 posY;
    u16 unk4;
    u16 unk6;
    u8 filler8[0x11 - 0x8];
    u8 unk11;
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 filler24[0x2A - 0x24];
    u16 unk2A;
    u16 unk2C;
};

struct SubBits11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags213Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct SubBits1F
{
    u8 filler0[0x1F];
    u8 lo2 : 2;
    u8 mid2 : 2;
    u8 hi4 : 4;
};

struct SubBits12
{
    u8 filler0[0x12];
    u8 bit0 : 1;
    u8 pair12 : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

struct Flags213Dirty
{
    u8 bit0 : 1;
    u8 dirty : 4;
    u8 hi3 : 3;
};

struct Flags298Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

struct Flags299Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct Flags20EBits
{
    u8 lo3 : 3;
    u8 mid3 : 3;
    u8 hi2 : 2;
};

struct ByteBits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Word214Bits
{
    u32 lo3 : 3;
    u32 mid10 : 10;
    u32 f13 : 4;
    u32 hi15 : 15;
};

/* the sub sprite's state saved in its field object (obj_copy_sub_fields) so the sprite can
 * be rebuilt from it later; each comment names the FieldSub field it is saved from */
struct SubSpriteSave
{
    u8 filler0[0x298];
    u8 valid : 1;  /* the fields below hold a saved state */
    u8 mirror : 1; /* 0x11 bit 6 */
    u8 lo1F : 2;   /* 0x1F bits 0-1 */
    u8 mid1F : 2;  /* 0x1F bits 2-3 */
    u8 pair12 : 2; /* 0x12 bits 1-2 */
    u8 bit12_3 : 1; /* 0x12 bit 3 */
    u8 still : 1;   /* 0x12 bit 4: the object stands still */
    u8 rest299 : 6;
    u8 filler29A;
    u8 anim;       /* 0x21: the record obj_load_record loaded */
    u8 frame;      /* 0x23 */
    u8 filler29D;
    u16 unk2A;     /* 0x2A */
    u16 unk2C;     /* 0x2C */
    u16 unk04;     /* 0x04 */
    u16 unk06;     /* 0x06 */
};

struct FieldVtable
{
    u8 filler0[0x38];
    s16 off38;
    u8 filler3A[0x2];
    void (*fn3C)(u8 *, u32);
};

struct FieldObj
{
    u8 filler0[0x1];
    u8 unk01;
    u8 unk02;
    u8 filler3;
    s16 state4;
    u8 filler6[0x6];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x28 - 0x25];
    u8 unk28;
    s8 unk29;
    u8 filler2A[0x2C - 0x2A];
    s16 unk2C;
    u8 filler2E[0x34 - 0x2E];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 filler44[0x4C - 0x44];
    u8 unk4C;
    u8 filler4D[0x50 - 0x4D];
    u16 unk50;
    u16 unk52;
    u8 filler54[0x20C - 0x54];
    u8 flags20C;
    u8 flags20D;
    u8 flags20E;
    u8 flags20F;
    u8 flags210;
    u8 filler211[0x213 - 0x211];
    u8 flags213;
    u8 state214;
    u8 filler215;
    u8 flags216;
    u8 filler217;
    u16 id218;
    u8 filler21A[0x21E - 0x21A];
    u16 extra21E;
    u8 filler220[0x230 - 0x220];
    s32 unk230;
    s32 unk234;
    s8 unk238;
    u8 unk239;
    u8 filler23A[0x23D - 0x23A];
    u8 unk23D;
    u16 moveSpeed;           /* walking speed; the air speed while jumping */
    u16 savedMoveSpeed;      /* moveSpeed before the jump, restored on landing */
    s16 unk242;
    s16 unk244;
    u8 filler246[0x248 - 0x246];
    s32 gravity;             /* taken off velocityZ each frame of a jump */
    s32 jumpStartZ;          /* unk14 + unk18 when the jump started */
    s32 jumpStartHeight;     /* unk18 when the jump started */
    s32 jumpVelocity;        /* vertical speed the jump started with */
    s32 velocityZ;           /* current vertical speed, added to unk18 each frame */
    s32 jumpFrame;           /* frames since the jump started, -1 = not jumping */
    u8 filler260[0x262 - 0x260];
    u16 jumpMoveSpeed;       /* default air speed of a jump */
    s16 defaultJumpVelocity; /* default jumpVelocity (0x52C) */
    s16 defaultGravity;      /* default gravity (0x75) */
    s16 unk268;
    u8 filler26A[0x26C - 0x26A];
    u16 spriteId;   /* sprite graphics id, 0xFFFF = none */
    s8 spriteSlot;  /* slot from sprite_alloc, -1 (0xFF) = none */
    u8 filler26F[0x294 - 0x26F];
    struct FieldSub *sub;
    u8 subSave[0x2A6 - 0x298]; /* struct SubSpriteSave */
    u8 filler2A6[0x2B5 - 0x2A6];
    u8 flags2B5;
    u8 filler2B6;
    u8 flags2B7;
    u8 flags2B8;
    u8 filler2B9[0x338 - 0x2B9];
    struct FieldVtable *vt338;
};

extern const u32 gTableEntryRows[];
extern const u8 gFrameAttrTable[];

void task_insert(void *node, u8 kind);
void task_remove(void *node, u32 kind);
void cpu_set(const void *src, void *dst, u32 ctl);
void obj_reset_move_state(struct FieldObj *obj);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_set_motion_params(struct FieldObj *obj, u16 a, u32 b, u32 d, u32 e);
void load_object_def(u16 def, u8 index, struct FieldObj *obj);
extern const u8 gObjBaseClass[];
extern const u8 gSpriteFacingTable[];
extern u8 gUnk_02000001[];
void setup_obj_10bc(void *obj, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);

void update_effect_slots(u32 effect);
void obj_list_pass_0a8c(void);
u32 build_sprite_oam(u8 *ent, u32 oam, u32 idx, u32 flip);
extern u8 gSys[];
extern u8 gUnk_0203FFB8[];
extern u32 gUnk_03000D48;
extern u32 gUnk_03000D4C;
extern u8 gUnk_02000110[];
extern const u8 gCarrierTrackStepTable[];

NONMATCH("asm/code_0803F510/obj_update_oam.s", void obj_update_oam(u8 *ent))
{
    u8 *obj;
    u32 oam;

    obj = *(u8 **)(ent + 0x14);
    update_effect_slots(*(u32 *)(obj + 0x31C));
    oam = *(u32 *)(gSys + 0x880);
    if (*(s16 *)(obj + 0x244) > -56)
    {
        u32 idx;
        s32 parts;
        u32 emitted;
        u32 flip;

        idx = (*(u8 *)(obj + 0x208) >> 2) & 1;
        parts = 1;
        if ((((*(u8 *)(obj + 0x2D8) & 0x1F) == 0x1F) && (*(u16 *)(gSys + 0x78) & 8) != 0)
            || (*(u16 *)(obj + 0x208) & 0x110) != 0)
        {
            parts = 2;
        }
        emitted = 0;
        if (!(0 < parts))
        {
            goto parts_done;
        }
        if (parts <= 0)
        {
            goto last_part;
        }
        if (parts == 0)
        {
            goto loop_head;
        }
        if (parts <= 1)
        {
            goto last_part;
        }
        if (!(parts <= 2))
        {
            oam = build_sprite_oam(ent, oam, idx, 0);
            idx ^= 1;
            emitted++;
        }
        flip = 0;
        if (emitted == 1 && (*(u32 *)(obj + 0x208) & (u32)gUnk_02000110) == 0x02000000)
        {
            flip = 1;
        }
        oam = build_sprite_oam(ent, oam, idx, flip);
        idx ^= 1;
        emitted++;
    last_part:
        flip = 0;
        if (emitted == 1 && (*(u32 *)(obj + 0x208) & (u32)gUnk_02000110) == 0x02000000)
        {
            flip = 1;
        }
        oam = build_sprite_oam(ent, oam, idx, flip);
        idx ^= 1;
        emitted++;
        if ((s32)emitted >= parts)
        {
            goto parts_done;
        }
    loop_head:
        do
        {
            flip = 0;
            if (emitted == 1 && (*(u32 *)(obj + 0x208) & (u32)gUnk_02000110) == 0x02000000)
            {
                flip = 1;
            }
            oam = build_sprite_oam(ent, oam, idx, flip);
            idx ^= 1;
            flip = 0;
            if (emitted == 0 && (*(u32 *)(obj + 0x208) & (u32)gUnk_02000110) == 0x02000000)
            {
                flip = 1;
            }
            oam = build_sprite_oam(ent, oam, idx, flip);
            idx ^= 1;
            flip = 0;
            if (emitted + 2 == 1 && (*(u32 *)(obj + 0x208) & (u32)gUnk_02000110) == 0x02000000)
            {
                flip = 1;
            }
            oam = build_sprite_oam(ent, oam, idx, flip);
            idx ^= 1;
            flip = 0;
            if (emitted + 3 == 1 && (*(u32 *)(obj + 0x208) & (u32)gUnk_02000110) == 0x02000000)
            {
                flip = 1;
            }
            oam = build_sprite_oam(ent, oam, idx, flip);
            idx ^= 1;
            emitted += 4;
        } while ((s32)emitted < parts);
    }
parts_done:
    {
        u32 anim;
        u32 bits;
        u32 next;

        anim = *(u8 *)(obj + 0x209);
    if ((anim & 0x1E) != 0)
    {
        next = (((anim << 27) >> 28) + 1) & 0xF;
        bits = (anim & 0xFFFFFFE1) | (next << 1);
        *(u8 *)(obj + 0x209) = bits;
        if ((bits & 0x1E) == 0x12)
        {
            *(u8 *)(obj + 0x209) = bits & 0xFFFFFFE1;
        }
    }
    }
    obj_list_pass_0a8c();

    {
    u32 rideA;
    u32 rideB;
    u32 carrier;
    u32 state;
    u32 target;
    u32 sub;
    u32 node;
    u32 n;
    u32 rem;
    u32 row;
    u32 head;
    u32 kind;
    u32 children;

    rideA = *(u32 *)(obj + 0x200);
    rideB = *(u32 *)(obj + 0x204);
    carrier = 0;
    if (*(u16 *)(rideA + 0x218) == 0x1025 || *(u16 *)(rideA + 0x218) == 0x1029
        || *(u16 *)(rideA + 0x218) == 0x103F)
    {
        carrier = rideA;
    }
    else if (*(u16 *)(rideB + 0x218) == 0x1025 || *(u16 *)(rideB + 0x218) == 0x103F)
    {
        carrier = rideB;
    }
    target = rideA;
    if (carrier != 0)
    {
        state = *(u16 *)(carrier + 4);
        if ((u16)(state - 8) <= 1 || (u16)(state - 0x59) <= 6 || (u16)(state - 0x3B) <= 1
            || (u16)(state - 0x80) <= 3 || (u16)(state - 0x88) <= 3)
        {
            kind = *(u8 *)(carrier + 0x24) & 7;
            if (!(kind <= 1) && !(kind > 6))
            {
                sub = *(u32 *)(carrier + 0x294);
                if (*(u8 *)(sub + 0x21) > 4 || *(u8 *)(sub + 0x23) > 4)
                {
                    if ((u16)(state - 8) <= 1)
                    {
                        children = (u32)obj + 0x28;
                        target = *(u32 *)(children + (((s32)((u32)*(u8 *)(carrier + 0x33C) << 25)) >> 23));
                    }
                    if (state == 8)
                    {
                        row = (*(u16 *)(carrier + 0x218) != 0x1029) ? 8 : 0x18;
                    }
                    else
                    {
                        row = (*(u16 *)(carrier + 0x218) != 0x1029) ? 0 : 0x10;
                    }
                    sub = *(u32 *)(carrier + 0x294);
                    node = *(u32 *)(sub + 0x34);
                    n = *(u8 *)(row + (*(u8 *)(carrier + 0x24) & 7) + gCarrierTrackStepTable);
                    if (n != 0)
                    {
                        rem = (0 - n) & 3;
                        if (rem != 0)
                        {
                            if (!((s32)rem >= 3))
                            {
                                if (!((s32)rem >= 2))
                                {
                                    node = *(u32 *)(node + 0x10);
                                    n--;
                                }
                                node = *(u32 *)(node + 0x10);
                                n--;
                            }
                            node = *(u32 *)(node + 0x10);
                            n--;
                            if (n == 0)
                            {
                                goto walked;
                            }
                        }
                        do
                        {
                            node = *(u32 *)(node + 0x10);
                            node = *(u32 *)(node + 0x10);
                            node = *(u32 *)(node + 0x10);
                            node = *(u32 *)(node + 0x10);
                            n -= 4;
                        } while (n != 0);
                    }
                walked:
                    head = *(u32 *)(target + 0x294);
                    head = *(u32 *)(head + 0x34);
                    *(u32 *)(node + 8) = *(u32 *)(head + 8) - 1;
                }
            }
        }
    }

    }

    {
    u32 ctx;
    u32 list;
    u32 children;
    u32 slot;
    u32 i;
    u32 child;
    u32 desc;
    u32 arg;
    u32 routine;
    u32 head;

    ctx = (u32)gUnk_0203FFB8;
    list = *(u32 *)(ctx + 0x38);
    children = (u32)obj + 0x28;
    slot = children;
    i = 0;
    while ((s32)i < (s32)*(u8 *)(obj + 0x10B))
    {
        child = *(u32 *)slot;
        if (child != 0)
        {
            desc = *(u32 *)(child + 0x338) + 0x50;
            arg = child + *(s16 *)desc;
            routine = *(u32 *)(desc + 4);
            list = ((u32(*)(u32, u32, u32))routine)(arg, list, children);
        }
        i++;
        slot += 4;
    }
    *(u32 *)(ctx + 0x38) = list;
    head = *(u32 *)(ctx + 0x3C);
    ((void (*)(u32, u32))gUnk_03000D48)(head, head + 0xA14);
    oam = ((u32(*)(u32, u32))gUnk_03000D4C)(*(u32 *)(ctx + 0x3C), oam);
    }
    *(u32 *)(gSys + 0x880) = oam;
}
END_NONMATCH

ASM_FUNC("asm/code_0803F510/build_sprite_oam.s", u32 build_sprite_oam(u8 *ent, u32 oam, u32 idx, u32 flip))

ASM_FUNC("asm/code_0803F510/tile_query.s", void tile_query(void))

/* Turns frame `frame` (group * directions + direction) of sprite entry `index` in `bank` into the
   record to load and whether to draw it flipped. Entry word bits 29-30 give how many directions the
   frames face (0: one, 1: four, 2/3: eight), bit 28 whether every direction has its own frame (else
   mirrored ones share, 3 or 5 frames a group); gFrameAttrTable gives the frame within the group
   (bits 0-6) and the flip (bit 7). A one-direction sprite loads record `frame` unflipped. */
void decode_coord_mode(u32 bank, u32 index, s32 frame, u8 *outRecord, u8 *outFlip)
{
    u32 entryWord;
    u32 dirMode;
    u32 allDirsDrawn;
    s32 groupStart;
    u8 attr;

    entryWord = *(u32 *)(gTableEntryRows[bank] + (index << 2));
    dirMode = (entryWord << 1) >> 30;
    allDirsDrawn = (entryWord << 3) >> 31;
    if (dirMode != 0)
    {
        // Split frame into the group's first record and the direction (four-direction ones doubled).
        if (allDirsDrawn == 0)
        {
            if (dirMode == 1)
            {
                groupStart = (frame / 4) * 3;
                frame -= (frame / 4) * 4;
                frame *= 2;
            }
            else
            {
                groupStart = (frame / 8) * 5;
                frame -= (frame / 8) * 8;
            }
        }
        else
        {
            if (dirMode == 1)
            {
                groupStart = (frame / 4) * 4;
                frame -= groupStart;
                frame *= 2;
            }
            else
            {
                groupStart = (frame / 8) * 8;
                frame -= groupStart;
            }
        }
        attr = gFrameAttrTable[(dirMode << 4) + (allDirsDrawn << 3) + frame];
        *outRecord = groupStart + (attr & 0x7F);
        *outFlip = attr >> 7;
    }
    else
    {
        *outRecord = frame;
        *outFlip = dirMode;
    }
}

void entry_update_task_remove(struct TaskObj *obj, u32 kind)
{
    obj->cb18 = gEntryUpdateTaskClass;
    task_remove(obj, kind);
}

struct TaskObj *entry_update_task_insert(struct TaskObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->cb18 = gEntryUpdateTaskClass;
    return obj;
}

struct TaskObj *obj_update_oam_task_insert(struct TaskObj *obj, u8 kind)
{
    task_insert(obj, kind);
    obj->cb18 = gObjUpdateOamTaskClass;
    return obj;
}

/* Constructor of a field object (0x33C bytes): installs the base class's vtable, sets the
 * default flags, ids and motion parameters, then loads entry `index` of object def `def`
 * into it unless `def` is -1. */
struct FieldObj *obj_init(struct FieldObj *obj, s32 def, u32 index)
{
    obj->vt338 = (struct FieldVtable *)gObjBaseClass;
    obj->unk01 = 0xFF;
    obj->unk29 = -1;
    ((struct ByteBits *)&obj->flags20C)->bit0 = 1;
    ((struct Flags20EBits *)&obj->flags20E)->mid3 = 1;
    ((struct ByteBits *)&obj->flags20F)->bit1 = 1;
    ((struct ByteBits *)&obj->flags210)->bit0 = 0;
    ((struct ByteBits *)&obj->flags210)->bit1 = 1;
    ((struct ByteBits *)&obj->flags210)->bit2 = 1;
    ((struct ByteBits *)&obj->flags210)->bit3 = 1;
    ((struct ByteBits *)&obj->flags210)->bit6 = 1;
    ((struct Lo3Bits *)&obj->flags2B5)->lo3 = 0;
    ((struct Lo3Bits *)&obj->flags2B7)->rest = 0x1F;
    ((struct Lo3Bits *)&obj->flags2B8)->lo3 = 7;
    ((struct Word214Bits *)&obj->state214)->lo3 = 3;
    ((struct Word214Bits *)&obj->state214)->f13 = 0xE;
    ((struct Flags298Bits *)&obj->flags216)->bit1 = 1;
    ((struct Flags298Bits *)&obj->flags216)->f2 = 0;
    obj->unk230 = -1;
    obj->unk234 = -1;
    obj->unk238 = -1;
    obj->unk239 = 0x10;
    obj->unk23D = 0;
    obj->spriteId = 0xFFFF;
    obj->spriteSlot = -1;
    obj->unk268 = -1;
    obj_set_motion_params(obj, 0x200, 0x200, 0x52C, 0x75);
    if (def != -1)
    {
        load_object_def(def, index, obj);
    }
    return obj;
}

void obj_refresh_dirty(struct FieldObj *obj)
{
    s32 dirty;

    if (!(obj->flags216 & 0x20))
    {
        cpu_set(obj, &obj->unk28, 0x0400000A);
        dirty = obj->unk34 != obj->unkC;
        if (obj->unk38 != obj->unk10)
        {
            dirty |= 2;
        }
        if (obj->unk3C != obj->unk14)
        {
            dirty |= 4;
        }
        if (obj->unk40 != obj->unk18)
        {
            dirty |= 8;
        }
        ((struct Flags213Dirty *)&obj->flags213)->dirty = dirty & 0xF;
        obj->unk50 = obj->sub->posX;
        obj->unk52 = obj->sub->posY;
    }
}

/* Starts a jump: height is in pixels (-1: the default jump velocity, 0: none, 30: velocity 0x52C,
   otherwise the starting speed that just reaches that height at this gravity), gravity -1 takes the
   default gravity and airSpeed -1 the default air speed; the walking speed is saved for the landing. */
void motion_obj_set_move_target(struct FieldObj *obj, s32 height, s32 gravity, s32 airSpeed)
{
    s32 *pGravity;
    u16 *pSpeed;
    s32 vel;
    s32 dist;
    s32 rising;

    pGravity = &obj->gravity;
    if (gravity == -1)
    {
        *pGravity = obj->defaultGravity;
    }
    else
    {
        *pGravity = gravity;
    }
    if (height == -1)
    {
        obj->jumpVelocity = obj->velocityZ = obj->defaultJumpVelocity;
    }
    else if (height == 0)
    {
        obj->jumpVelocity = obj->velocityZ = 0;
    }
    else if (height == 30)
    {
        obj->jumpVelocity = obj->velocityZ = 0x52C;
    }
    else
    {
        /* Add up the speeds of a fall from rest until it covers the height, then take off the
           part of the last frame that overshoots it. */
        height <<= 8;
        vel = 0;
        dist = 0;
        do
        {
            vel += obj->gravity;
            dist += vel;
        } while (dist < height);
        dist -= height;
        if (dist > 0)
        {
            dist <<= 8;
            dist /= vel;
            vel -= obj->gravity * dist / 0x100;
        }
        obj->jumpVelocity = vel;
        obj->velocityZ = vel;
    }
    rising = 0;
    if (obj->velocityZ > 0)
    {
        rising = 1;
    }
    ((struct Flags213Bits *)&obj->flags213)->bit5 = rising;
    obj->jumpFrame = 0;
    obj->jumpStartZ = obj->unk14 + obj->unk18;
    obj->jumpStartHeight = obj->unk18;
    pSpeed = &obj->moveSpeed;
    obj->savedMoveSpeed = *pSpeed;
    if (airSpeed == -1)
    {
        *pSpeed = obj->jumpMoveSpeed;
    }
    else
    {
        *pSpeed = airSpeed;
    }
}

/* Starts a jump with a given launch speed: sets gravity and jumpVelocity (-1 takes the object's
   defaults), starts velocityZ at jumpVelocity, records where the jump started, saves moveSpeed and
   switches it to airSpeed (-1: jumpMoveSpeed), and sets flags213 bit 5 while rising. */
void obj_init_motion(struct FieldObj *obj, s32 velocity, s32 gravity, s32 airSpeed)
{
    s32 *pGravity;
    s32 *pVelocity;
    u16 *pSpeed;
    s32 rising;

    pGravity = &obj->gravity;
    if (gravity == -1)
    {
        *pGravity = obj->defaultGravity;
    }
    else
    {
        *pGravity = gravity;
    }
    pVelocity = &obj->jumpVelocity;
    if (velocity == -1)
    {
        *pVelocity = obj->defaultJumpVelocity;
    }
    else
    {
        *pVelocity = velocity;
    }
    obj->velocityZ = obj->jumpVelocity;
    obj->jumpFrame = 0;
    obj->jumpStartZ = obj->unk14 + obj->unk18;
    obj->jumpStartHeight = obj->unk18;
    pSpeed = &obj->moveSpeed;
    obj->savedMoveSpeed = *pSpeed;
    if (airSpeed == -1)
    {
        *pSpeed = obj->jumpMoveSpeed;
    }
    else
    {
        *pSpeed = airSpeed;
    }
    rising = 0;
    if (obj->velocityZ > 0)
    {
        rising = 1;
    }
    ((struct Flags213Bits *)&obj->flags213)->bit5 = rising;
}

void motion_obj_step_motion(struct FieldObj *obj)
{
    s32 *counter;

    counter = &obj->jumpFrame;
    if (*counter >= 0)
    {
        obj->unk18 += obj->velocityZ;
        obj->velocityZ -= obj->gravity;
        if (!(obj->flags20D & 0x20))
        {
            if (obj->unk18 <= 0)
            {
                obj_reset_move_state(obj);
                obj->unk18 = 0;
                return;
            }
        }
        else
        {
            if (obj->unk14 + obj->unk18 <= obj->jumpStartZ)
            {
                obj_reset_move_state(obj);
                if (obj->unk14 + obj->unk18 < 0)
                {
                    obj->unk18 = -obj->unk14;
                }
                return;
            }
        }
        *counter += 1;
    }
}

void obj_update_state(struct FieldObj *obj)
{
    s32 y;

    if ((obj->flags20C & 1) && !(obj->flags216 & 0x20))
    {
        if (obj->jumpFrame >= 0)
        {
            if (obj->unk14 + obj->unk18 >= obj->unk3C + obj->unk40)
                obj->state4 = 2;
            else
                obj->state4 = 3;
        }
        else if (obj->unk242 || (y = obj->unk244))
        {
            obj->state4 = 1;
        }
        else
        {
            obj->state4 = y;
        }
    }
}

ASM_FUNC("asm/code_0803F510/obj_create_sub.s", void obj_create_sub(void))

void obj_update_special_flag(struct FieldObj *obj)
{
    u32 special;
    u32 flags;
    u8 *pf;

    special = 0;
    if ((u16)(obj->id218 - 0x1001) <= 1 || obj->id218 == 0x1028)
    {
        special = 1;
    }
    pf = &obj->flags216;
    special <<= 7;
    flags = (*pf & 0x7F) | special;
    *pf = flags;
    setup_obj_10bc(obj->sub, 0, obj->id218, (flags >> 1) & 1, -1, -1, -1, obj->extra21E);
}

ASM_FUNC("asm/code_0803F510/obj_refresh_anim_state.s", void obj_refresh_anim_state(void))

/* Saves the sub sprite's state (mirror and flag bits, animation, frame and fields) into the
 * object's SubSpriteSave at 0x298 and marks the save valid, so the sprite can be rebuilt later. */
void obj_copy_sub_fields(struct FieldObj *obj)
{
    struct SubSpriteSave *save;
    u32 bits;

    save = (struct SubSpriteSave *)obj;
    bits = ((struct SubBits11 *)obj->sub)->bit6;
    save->mirror = bits;
    bits = ((struct SubBits1F *)obj->sub)->lo2;
    save->lo1F = bits;
    bits = ((struct SubBits1F *)obj->sub)->mid2;
    save->mid1F = bits;
    bits = ((struct SubBits12 *)obj->sub)->pair12;
    save->pair12 = bits;
    bits = ((struct SubBits12 *)obj->sub)->bit3;
    save->bit12_3 = bits;
    bits = ((struct SubBits12 *)obj->sub)->bit4;
    save->still = bits;
    save->anim = obj->sub->unk21;
    save->frame = obj->sub->unk23;
    save->unk2A = obj->sub->unk2A;
    save->unk2C = obj->sub->unk2C;
    save->unk04 = obj->sub->unk4;
    save->unk06 = obj->sub->unk6;
    save->valid = 1;
}

/* state214 bits 0-2: how the object moves (0-1 and 4: on the ground, 2-3 and 5: free motion) */
#define FIELD_MOTION_KIND(obj) (((u32)(obj)->state214 << 29) >> 29)
/* unk24 / unk4C bits 0-2: the facing (one of 8) now / when the animation was last loaded */
#define FIELD_FACING_BITS(b) ((u32)(b) << 29)
#define FIELD_FLAG216_NO_UPDATE 0x20 /* obj_update_state and obj_refresh_motion_state skip it */
#define FIELD_FLAG216_FACING_MODE 0xC

/*
 * Keeps a field object's animation in step with how it moves: picks its facing from its angle,
 * reloads the animation (through the vtable, keeping the sub's pair12 bits) when the facing or
 * the move state changed or `force` is set, and tells the sprite whether the object stands still.
 * Only for objects whose flags20C bit 0 and flags20F bit 1 are both set (mask 0x02000001).
 */
void obj_refresh_motion_state(struct FieldObj *obj, u32 force)
{
    s32 reload;
    s32 still;
    u32 savedPair;
    u32 dir;
    u32 facing;
    u32 flags216;
    u32 facingMode;
    struct FieldVtable *vt;
    u8 *facingTable;

    if ((*(u32 *)((u8 *)obj + 0x20C) & (u32)gUnk_02000001) == (u32)gUnk_02000001)
    {
        flags216 = obj->flags216;
        if (!(flags216 & FIELD_FLAG216_NO_UPDATE))
        {
            if (FIELD_MOTION_KIND(obj) > 2)
            {
                /* 16 directions from the angle's high nibble; the second half of the table is
                   for every facing mode but 4 */
                facingTable = (u8 *)gSpriteFacingTable;
                dir = obj->unk02 >> 4;
                facingMode = FIELD_FLAG216_FACING_MODE;
                facingMode &= flags216;
                if (facingMode != 4)
                {
                    dir += 0x10;
                }
                facing = facingTable[dir];
                ((struct Lo3Bits *)&obj->unk24)->lo3 = facing;
            }
            reload = force;
            still = 0;
            switch (FIELD_MOTION_KIND(obj))
            {
            case 0:
            case 1:
            case 4:
                if (FIELD_FACING_BITS(obj->unk24) != FIELD_FACING_BITS(obj->unk4C))
                {
                    reload = 1;
                }
                if ((obj->state214 & 7) == 0)
                {
                    still = 1;
                }
                break;
            case 2:
            case 3:
            case 5:
                /* state4 is the move state obj_update_state picks; unk2C the one last shown */
                if (obj->state4 != obj->unk2C)
                {
                    reload = 1;
                }
                else if (FIELD_FACING_BITS(obj->unk24) != FIELD_FACING_BITS(obj->unk4C))
                {
                    reload = 1;
                }
                if (obj->unk242 == 0 && obj->unk244 == 0)
                {
                    still = 1;
                }
                else if ((obj->state214 & 7) == 5)
                {
                    still = 1;
                }
                break;
            }
            if (reload != 0)
            {
                savedPair = ((struct SubBits12 *)obj->sub)->pair12;
                vt = obj->vt338;
                vt->fn3C((u8 *)obj + vt->off38, 1);
                ((struct SubBits12 *)obj->sub)->pair12 = savedPair;
            }
            ((struct SubBits12 *)obj->sub)->bit4 = still;
        }
    }
}

/* Points the field object's sub sprite at the record for its facing: with a facing mode in
 * flags216 (bits 2-3: directions, bit 4: every direction drawn, as in decode_coord_mode) the
 * record and flip come from gFrameAttrTable for the facing in unk24 bits 0-2, else the sub keeps
 * its record, unflipped. Reloads the record when it or the flip changed, or when `force` is set. */
void obj_sync_sub_state(struct FieldObj *obj, u8 force)
{
    u32 flags;
    u8 record;
    u32 flip;
    u8 attr;

    flags = obj->flags216;
    if (!(flags & FIELD_FLAG216_FACING_MODE))
    {
        record = obj->sub->unk21;
        flip = 0;
    }
    else
    {
        /* [dirMode][allDirsDrawn][facing] */
        attr = gFrameAttrTable[((((flags << 28) >> 30) << 4)) + ((((flags << 27) >> 31) << 3)) + (((u32)obj->unk24 << 29) >> 29)];
        record = attr & 0x7F;
        flip = attr >> 7;
    }
    if (obj->sub->unk21 != record || ((struct SubBits11 *)obj->sub)->bit6 != flip || force != 0)
    {
        obj_load_record(obj->sub, record, 0, 0, 0);
        ((struct SubBits11 *)obj->sub)->bit6 = flip;
    }
}

ASM_FUNC("asm/code_0803F510/obj_update_screen_pos.s", void obj_update_screen_pos(void))
