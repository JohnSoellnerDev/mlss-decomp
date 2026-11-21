#include "global.h"

struct SyncSub
{
    u8 filler0[2];
    u8 modeBits;
    u8 filler3;
    s16 unk4;
    u8 filler6[0x26 - 6];
    u8 poseBits;
    u8 filler27[0x54 - 0x27];
    u8 group;
    u8 filler55[0x25C - 0x55];
    s32 unk25C;
    u8 filler260[0x353 - 0x260];
    u8 selBits;
    u8 filler354[0x358 - 0x354];
    u8 flags358;
    u8 filler359;
    u8 flags35A;
    u8 flags35B;
};

struct SysKeys78
{
    u8 filler0[0x78];
    u16 keyEnable : 10;
};

struct PartyMember
{
    u8 filler0[0xE];
    u16 unkE;
    u8 filler10[0x3C - 0x10];
};

struct Party
{
    struct PartyMember members[2];
};

extern u8 gSys[];
extern u8 *gParty;
void obj_update_stage3_id(struct SyncSub *sub);
void obj_start_state_4(struct SyncSub *sub);

struct SyncPair
{
    u8 filler0[0x200];
    struct SyncSub *subs[2];
};

struct SyncMode
{
    u8 lo5 : 5;
    u8 mode : 3;
} __attribute__((packed));

struct SyncPose
{
    u8 lo2 : 2;
    u8 pose : 2;
    u8 hi4 : 4;
} __attribute__((packed));

struct SyncSel
{
    u8 lo4 : 4;
    u8 sel : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct SyncSubSel
{
    u8 filler0[0x353];
    u8 lo4 : 4;
    u8 sel : 2;
    u8 hi2 : 2;
} __attribute__((packed));

/* Sets the second sub-object's sel (+0x353 bits 4-5) from the first one's. A first with sel 1-3
 * that isn't in pose 3 (+0x26 bits 2-3) passes its sel on unchanged. Otherwise, if the second is
 * in pose 3, it gets a base (0 for sel 0, or sel 1 with unk25C == -1; 1 for the rest) plus 1
 * unless it faces direction 2 or 6 (+2 bits 5-7, 8-way); if not, it gets 0 for sel 0-1 and 1
 * for sel 2-3. */
void sync_mode_353(struct SyncPair *pair)
{
    struct SyncSub *second;
    struct SyncSub *first;
    u16 sel;
    u8 facing;

    second = pair->subs[1];
    facing = ((struct SyncMode *)&second->modeBits)->mode;
    first = pair->subs[0];
    if (((struct SyncSel *)&first->selBits)->sel != 0)
    {
        if (((struct SyncPose *)&first->poseBits)->pose == 3)
        {
            if (((struct SyncSel *)&first->selBits)->sel == 1)
            {
                if (((struct SyncPose *)&second->poseBits)->pose == 3)
                {
                    if (first->unk25C == -1)
                    {
                        if (facing == 6 || facing == 2)
                            ((struct SyncSel *)&second->selBits)->sel = 0;
                        else
                            ((struct SyncSel *)&second->selBits)->sel = 1;
                    }
                    else
                    {
                        if (facing == 6 || facing == 2)
                        {
                            sel = ((struct SyncSel *)&first->selBits)->sel;
                            ((struct SyncSel *)&second->selBits)->sel = sel;
                        }
                        else
                            ((struct SyncSubSel *)second)->sel = ((struct SyncSel *)&first->selBits)->sel + 1;
                    }
                }
                else
                {
                    ((struct SyncSel *)&second->selBits)->sel = 0;
                }
            }
            else
            {
                if (((struct SyncPose *)&second->poseBits)->pose == 3)
                {
                    if (facing == 6 || facing == 2)
                        ((struct SyncSel *)&second->selBits)->sel = 1;
                    else
                        ((struct SyncSel *)&second->selBits)->sel = 2;
                }
                else
                {
                    ((struct SyncSel *)&second->selBits)->sel = 1;
                }
            }
        }
        else
        {
            sel = ((struct SyncSel *)&first->selBits)->sel;
            ((struct SyncSel *)&second->selBits)->sel = sel;
        }
    }
    else
    {
        if (((struct SyncPose *)&second->poseBits)->pose == 3)
        {
            if (facing == 6 || facing == 2)
            {
                sel = ((struct SyncSel *)&first->selBits)->sel;
                ((struct SyncSel *)&second->selBits)->sel = sel;
            }
            else
                ((struct SyncSubSel *)second)->sel = ((struct SyncSel *)&first->selBits)->sel + 1;
        }
        else
        {
            ((struct SyncSel *)&second->selBits)->sel = 0;
        }
    }
}

// Per-frame step for the pair's two sub-objects. While flags358 bit 3 is clear and the A, B and
// D-pad keys are all enabled in gSys, a sub-object that is idle (flags35B bits 3-6 clear, unk4 0,
// party member unkE <= 1) is put into state 4. Any other sub-object with flags35A bit 2 set gets
// its stage-3 id update instead.
void update_sub_objects(struct SyncPair *pair)
{
    if ((pair->subs[0]->flags358 & 8)
        || (((struct SysKeys78 *)gSys)->keyEnable & (A_BUTTON | B_BUTTON | DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN))
               != (A_BUTTON | B_BUTTON | DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN))
    {
        if (pair->subs[0]->flags35A & 4)
            obj_update_stage3_id(pair->subs[0]);
        if (pair->subs[1]->flags35A & 4)
            obj_update_stage3_id(pair->subs[1]);
    }
    else
    {
        if ((pair->subs[0]->flags35B & 0x78) == 0 && pair->subs[0]->unk4 == 0
            && ((struct Party *)gParty)->members[pair->subs[0]->group].unkE <= 1)
            obj_start_state_4(pair->subs[0]);
        else if (pair->subs[0]->flags35A & 4)
            obj_update_stage3_id(pair->subs[0]);
        if ((pair->subs[1]->flags35B & 0x78) == 0 && pair->subs[1]->unk4 == 0
            && ((struct Party *)gParty)->members[pair->subs[1]->group].unkE <= 1)
            obj_start_state_4(pair->subs[1]);
        else if (pair->subs[1]->flags35A & 4)
            obj_update_stage3_id(pair->subs[1]);
    }
}

ASM_FUNC("asm/code_0802A4FC/pair_frame_update.s", void pair_frame_update(void))

ASM_FUNC("asm/code_0802A4FC/update_sub_frame.s", void update_sub_frame(void))

extern const u8 gDirMask[];
u8 obj_push_out(void *obj, void *entry);
void obj_set_slot_value(void *obj, s32 value);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);

// CtObj.link: which of the room's entities the object touches (obj_push_out records it).
struct CtLink
{
    s8 idx : 7;     // the entity's index in CtPair.entries, CT_NO_CONTACT for none
    u8 contact : 1; // 1: it is that entity's partner that is touched
} __attribute__((packed));

#define CT_NO_CONTACT -1

// CtObj.flags20C
#define CT_ACTIVE         0x01 // takes part in collisions
#define CT_BUMPS_PARTNER  0x40 // collides with the other brother (CtPair.entries[group ^ 1])
#define CT_SOLID          0x80 // others are stopped by it (obj_push_out)

// CtObj.type20E & CT_TYPE_MASK: which brothers' moves an entity reacts to (CtObj.mode352 & 1, & 2)
#define CT_TYPE_MASK      0x38
#define CT_TYPE_A         0x08
#define CT_TYPE_B         0x10

// CtObj.flags216
#define CT_DISABLED       0x20

struct CtKind
{
    u8 b0 : 1;
    u8 kind : 3;
    u8 hi : 4;
} __attribute__((packed));

struct CtLo4
{
    u8 lo4 : 4;
    u8 hi4 : 4;
} __attribute__((packed));

struct CtDir
{
    u8 lo5 : 5;
    u8 dir : 3;
} __attribute__((packed));

struct CtMid
{
    u8 lo2 : 2;
    u8 mid : 3;
    u8 hi : 3;
} __attribute__((packed));

struct CtBit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi : 3;
} __attribute__((packed));

struct CtObj
{
    u8 filler0[2];
    u8 unk2;
    u8 filler3[0xC - 3];
    s32 x;
    s32 y;
    s32 z;
    s32 zStep;
    u8 filler1C[0x40 - 0x1C];
    s32 zVel;
    u8 filler44[0x54 - 0x44];
    u8 group;
    u8 filler55[0x20C - 0x55];
    u8 flags20C;
    u8 flags20D;
    u8 type20E;
    u8 flags20F;
    u8 filler210[0x216 - 0x210];
    u8 flags216;
    u8 filler217[0x270 - 0x217];
    s16 offsetY;
    u16 width;
    u8 filler274[2];
    u16 height;
    u8 filler278[0x284 - 0x278];
    s32 boxX;
    s32 boxY;
    s32 boxZ;
    u8 filler290[0x33C - 0x290];
    u8 link;
    u8 filler33D[3];
    u8 state;
    u8 prevState;
    u8 filler342[2];
    struct CtObj *partner;
    u8 filler348[0x350 - 0x348];
    u8 mask350;
    u8 filler351;
    u8 mode352;
    u8 filler353[0x358 - 0x353];
    u8 flags358;
    u8 filler359[0x35B - 0x359];
    u8 flags35B;
};

struct CtPair
{
    u8 filler0[0x28];
    struct CtObj *entries[0x39];
    u8 filler10C[2];
    u8 pStart;
    u8 pCount;
    u8 aStart;
    u8 aCount;
    u8 cStart;
    u8 cCount;
    u8 filler114[0x11A - 0x114];
    u8 dStart;
    u8 dCount;
    u8 filler11C[0x12A - 0x11C];
    u8 action;
    u8 filler12B[0x208 - 0x12B];
    u8 flags208;
    u8 flags209;
    u8 filler20A[0x246 - 0x20A];
    u8 state246;
};

/* One brother's collisions in the room, each frame: sets his collision box, bumps him against
   the other brother, then looks for the first entity he touches in list C (or an entity's
   partner), in the entity list (where an entity of a CtKind > 1 he walks into head-on, or
   faces, becomes his state) and in list D. Then moves his state history on and advances his
   animation for the new state. */
void obj_update_contacts(struct CtPair *room, struct CtObj *bro, u8 skipEntities)
{
    struct CtObj **slot;
    struct CtObj *entity;
    struct CtObj *partner;
    struct CtObj *touched;
    u32 left;
    s32 first;
    s32 broX, broY, touchedX, touchedY, halfWidths;
    u32 type, sides, state;
    s32 against;

    if (bro == NULL)
        goto update_state;
    if (((struct CtLink *)&bro->link)->idx != CT_NO_CONTACT)
        goto update_state;
    if (!(bro->flags20C & CT_ACTIVE))
        goto update_state;
    if (bro->flags216 & CT_DISABLED)
        goto update_state;
    if (!(bro->flags20F & 1))
        goto update_state;

    bro->boxX = bro->x - (bro->width >> 1);
    bro->boxY = bro->y + bro->offsetY;
    bro->boxZ = (bro->zStep > 0 || (bro->zStep < 0 && bro->zVel < 0)) ? bro->z + bro->zStep : bro->z;

    if (!(room->flags208 & 0x10) && (bro->flags20C & CT_BUMPS_PARTNER))
        obj_push_out(bro, room->entries[bro->group ^ 1]);

    if (!(bro->flags20D & 1))
        goto update_state;
    if (bro->flags35B & 0x10)
        goto update_state;

    // list C: an entity, or its partner
    if (((struct CtLink *)&bro->link)->idx == CT_NO_CONTACT && room->cStart != 0)
    {
        slot = &room->entries[room->cStart];
        for (left = room->cCount; left != 0; left--, slot++)
        {
            entity = *slot;
            if (obj_push_out(bro, entity))
            {
                ((struct CtLink *)&bro->link)->contact = 0;
                break;
            }
            partner = entity->partner;
            if (partner != NULL && (partner->flags20C & CT_ACTIVE) && obj_push_out(bro, partner))
            {
                ((struct CtLink *)&bro->link)->contact = 1;
                break;
            }
        }
    }

    if (skipEntities != 0)
        goto update_state;
    if (((struct CtLink *)&bro->link)->idx != CT_NO_CONTACT)
        goto update_state;

    // the entity list: from list P on, or only list A
    if ((!(room->flags208 & 0x10) || (bro->mode352 & 0x80)
         || ((struct CtMid *)&room->state246)->mid == 3 || ((struct CtMid *)&room->state246)->mid == 6)
        && room->pStart != 0)
        first = room->pStart;
    else if (room->aStart != 0)
        first = room->aStart;
    else
        first = 0;

    if (first > 0)
    {
        slot = &room->entries[first];
        for (left = room->pCount + room->aCount - first + 2; left != 0; left--, slot++)
        {
            if (obj_push_out(bro, *slot) && !(bro->flags358 & 8))
            {
                touched = room->entries[((struct CtLink *)&bro->link)->idx];
                type = touched->type20E & CT_TYPE_MASK;
                if ((type == CT_TYPE_A && (bro->mode352 & 1)) || (type == CT_TYPE_B && (bro->mode352 & 2)))
                {
                    if ((touched->type20E & CT_TYPE_MASK) == CT_TYPE_A
                        && ((struct CtKind *)&touched->flags20C)->kind > 1 && bro->zStep == 0)
                    {
                        broX = bro->x;
                        broY = bro->y + bro->offsetY;
                        touchedX = touched->x;
                        touchedY = touched->y + touched->offsetY;
                        halfWidths = (bro->width + touched->width) >> 1;
                        against = 0;
                        if (broX - halfWidths < touchedX && broX + halfWidths > touchedX
                            && broY - bro->height < touchedY && broY + touched->height > touchedY)
                        {
                            against = 1;
                        }
                        else
                        {
                            // touched on the side he faces: does he overlap it along that side?
                            if (gDirMask[((struct CtDir *)&bro->unk2)->dir] & (sides = ((struct CtLo4 *)&bro->mask350)->lo4))
                            {
                                switch (sides)
                                {
                                case 1:
                                case 4:
                                    if (broX + (bro->width >> 1) > touchedX - (touched->width >> 1)
                                        && broX - (bro->width >> 1) < touchedX + (touched->width >> 1))
                                        against = 1;
                                    break;
                                case 2:
                                case 8:
                                    if (broY > touchedY - touched->height && broY - bro->height < touchedY)
                                        against = 1;
                                    break;
                                }
                            }
                        }
                        if (against)
                            bro->state = ((struct CtKind *)&touched->flags20C)->kind;
                    }
                    break;
                }
            }
        }
    }

    // list D: the first entity touched
    if (((struct CtLink *)&bro->link)->idx == CT_NO_CONTACT && room->dStart != 0)
    {
        struct CtObj **dSlot = &room->entries[room->dStart];
        u32 dLeft;

        for (dLeft = room->dCount; dLeft != 0; dLeft--, dSlot++)
        {
            if (obj_push_out(bro, *dSlot))
                break;
        }
    }

update_state:
    if (!(room->flags209 & 1) && room->action != 0 && room->action <= 2
        && ((room->flags208 & 0x10) || (bro->mode352 & 0x80)))
    {
        state = bro->state;
        if (state == 2)
        {
            obj_set_slot_value(bro, 1);
            advance_anim_frame(room, bro, (bro->mode352 & 0x80) ? 0 : ((struct CtBit4 *)&room->flags208)->bit4, 0);
        }
        else if ((bro->mode352 & 0x80) && state == 3)
        {
            obj_set_slot_value(bro, 2);
            advance_anim_frame(room, bro, (bro->mode352 & 0x80) ? 0 : ((struct CtBit4 *)&room->flags208)->bit4, 0);
        }
        else
        {
            if (bro->prevState == bro->state)
                return;
            obj_shift_state_slots(bro);
            advance_anim_frame(room, bro, (bro->mode352 & 0x80) ? 0 : ((struct CtBit4 *)&room->flags208)->bit4, 0);
        }
        return;
    }
    obj_shift_state_slots(bro);
}
