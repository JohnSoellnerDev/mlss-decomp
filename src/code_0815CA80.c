#include "global.h"

// Class table of the objects obj_cmd_drop handles: each slot is a this-offset and a function.
struct E0E0Table
{
    u8 filler0[0x10];
    s16 unk10;
    u8 filler12[2];
    void (*unk14)(void *, u32, u32, u32, u32);
    u8 filler18[0x18];
    s16 unk30;
    u8 filler32[2];
    void (*unk34)(void *);
    s16 unk38;
    u8 filler3A[2];
    void (*unk3C)(void *);
};

struct VtSlot
{
    s16 offset;
    u16 filler2;
    void (*fn)();
};

struct MotionClass
{
    u8 filler0[0x10];
    struct VtSlot slot10;
    u8 filler18[0x30 - 0x18];
    struct VtSlot slot30;
    struct VtSlot slot38;
};

struct MotionObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[8];
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u8 unk24;
    u8 filler25[0x30 - 0x25];
    struct MotionClass *cls30;
    struct ChildSprite *child34;
    s32 unk38;
    s32 unk3C;
};

void obj_init_from_record(void *obj, u32 rec);
void obj_create_sprite(void *obj, u32 a);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
extern u8 gClassE8a0[];
extern u8 gClassE620[];
void sprite_update_cd0(void *sprite);
void obj_set_sprite_kind(void *obj, u32 kind);
void obj_release_sprite(void *obj);
u32 view_coord(u32 a, s32 b);
void set_slot_value(u32 id, s32 value);
s16 obj_get_pos18(void *obj);

struct SprChild
{
    u16 unk0;
    u16 unk2;
};

/* A 3-digit counter on screen (the game's score display, heap tag "Scor"). */
struct SpriteGroup
{
    struct SprChild *child0; /* hundreds digit sprite, at (0xD8, 0x10) */
    struct SprChild *child4; /* tens digit sprite, at (0xE0, 0x10) */
    struct SprChild *child8; /* ones digit sprite, at (0xE8, 0x10) */
    struct SprChild *childC; /* marker sprite (record 0x26), parked at SPRITE_OFFSCREEN when unused */
    s32 unk10;               /* marker's base x, 8.8 fixed point */
    s32 unk14;               /* marker's y, 8.8 fixed point */
    u16 unk18;               /* the value shown */
    u8 unk1A;                /* enum ScoreEffect */
    u8 unk1B;                /* frames left of the effect */
    u8 unk1C;                /* SCORE_SHAKE: 0 or 4, the digits move by it - 2 each frame */
    u8 unk1D;                /* SCORE_SWING: the marker's cosine phase */
};

/* Record of digit 0 in the digit sprites; digit n is SCORE_DIGIT_RECORD + n. */
#define SCORE_DIGIT_RECORD 0xA

/* x and y that park a sprite off screen. */
#define SPRITE_OFFSCREEN 0xFFF0

enum ScoreEffect
{
    SCORE_EFFECT_NONE = 0,
    SCORE_EFFECT_SHAKE = 1, /* the digits jitter 2 px left and right */
    SCORE_EFFECT_SWING = 2, /* the marker swings left and right around its base x */
};

struct InitObj
{
    u8 filler0[0x24];
    u8 unk24;
    u8 filler25[0x2C - 0x25];
    u8 unk2C;
    u8 unk2D;
    u8 filler2E[0x30 - 0x2E];
    struct MotionClass *cls30;
    s32 unk34;
    struct ChildSprite *child38;
    u16 unk3C;
    u16 unk3E;
    u8 unk40;
    u8 unk41;
};

struct ChildSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xE - 4];
    u16 unkE;
    u8 filler10[0x12 - 0x10];
    u8 unk12;
    u8 filler13[0x1F - 0x13];
    u8 g0 : 2;
    u8 g2 : 2;
    u8 g4 : 2;
    u8 g6 : 2;
    u8 filler20[0x21 - 0x20];
    u8 unk21;
};

struct E620Obj
{
    u8 filler0[0x30];
    struct MotionClass *cls30;
    struct ChildSprite *child34;
    u16 unk38;
};

struct SpriteRef
{
    u16 x;
    u16 y;
    u8 filler4[0x1F - 4];
    u8 flags1F;
};

struct SprBits12
{
    u8 filler0[0x12];
    u8 lo1 : 1;
    u8 mode : 2;
    u8 mid2 : 2;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

extern u8 *gUnk_03001020;
extern s16 gCosineTable[];
extern const u8 gSceneLayerSetupTable[];
void *obj_get_sprite(void *obj);

struct SprBits11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

/* An object that waits, then flies at the scene's first character (obj_cmd_seek_char). */
struct HomingObj
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[8];
    u16 vx;           /* 8.8 pixels a frame */
    u8 filler16[2];
    u16 vy;
    u8 filler1A[2];
    u16 heading;      /* HOMING_CHASE: 8.8 angle it flies at; HOMING_KNOCKED: x acceleration */
    u8 filler1E[2];
    u16 extraGravity; /* added to the scene's gravity while knocked away */
    u8 filler22[2];
    u8 state;         /* enum HomingState */
    u8 filler25;
    u8 waitBehind;    /* HOMING_WAIT: 0 = until the character is 50 px past it, else until it is within 100 px */
    u8 filler27[0x30 - 0x27];
    struct MotionClass *cls30;
    u8 filler34[4];
    u16 bobPhase;     /* index into gCosineTable for the up-and-down bob */
};

enum HomingState
{
    HOMING_GONE,
    HOMING_CHASE,
    HOMING_WAIT,
    HOMING_KNOCKED,
};

struct HomingSprite
{
    u8 filler0[0xC];
    u16 offsetY;
};

/* the character it chases */
#define HOMING_TARGET (*(struct HomingObj **)(gUnk_03001020 + 0x1C98))

/* the scene's lead character, as a MotionObj (obj_cmd_fly) */
#define LEAD_CHAR (*(struct MotionObj **)(gUnk_03001020 + 0x1C98))

/* the game's level: gGameMode & 0x1F, an index into gSceneLayerSetupTable */
#define GAME_LEVEL (*(u8 *)(gUnk_03001020 + 0x1EA2))

extern s16 gSineTable[];
s32 atan2_angle(s32 x, s32 y);

ASM_FUNC("asm/code_0815CA80/obj_cmd_follow_b.s", void obj_cmd_follow_b(void))


ASM_FUNC("asm/code_0815CA80/obj_cmd_approach_char.s", void obj_cmd_approach_char(void))

/*
 * Command handler of a homing object. mode 3 releases it, mode 7 (hit) knocks it away from the
 * character it chases, any other mode runs one frame: it bobs in place until the character comes
 * close, then flies at the character turning a little each frame, and once knocked away falls
 * until it is off screen. Returns 1 once it has left the screen (it is gone), else 0.
 */
u32 obj_cmd_seek_char(struct HomingObj *obj, u32 unused1, u16 gravity, s32 *view, u8 mode)
{
    struct MotionClass *cls;
    s32 y;
    s32 angle;
    s32 fall;

    switch (mode)
    {
    case 3:
        cls = obj->cls30;
        cls->slot30.fn((u8 *)obj + cls->slot30.offset);
        break;
    case 4:
        break;
    case 7:
        if (obj->state == HOMING_WAIT)
            break;
        if (obj->state == HOMING_KNOCKED)
            break;
        /* 2 px a frame away from the character */
        if (obj->x > HOMING_TARGET->x)
            obj->vx = 0x200;
        else
            obj->vx = 0xFE00;
        obj->heading = 0;
        obj->vy = 0;
        if (obj_get_sprite(obj) != 0)
        {
            obj_set_sprite_kind(obj, 0x16);
            ((struct HomingSprite *)obj_get_sprite(obj))->offsetY = 0;
        }
        obj->state = HOMING_KNOCKED;
        break;
    default:
        switch (obj->state)
        {
        case HOMING_GONE:
            break;
        case HOMING_WAIT:
            if (obj->waitBehind == 0)
            {
                if (obj->x - HOMING_TARGET->x < -0x3200)
                {
                    obj->heading = 0x4000;
                    obj_set_sprite_kind(obj, 0x16);
                    ((struct HomingSprite *)obj_get_sprite(obj))->offsetY = 0;
                    obj->state = HOMING_CHASE;
                }
                else
                {
                    obj->bobPhase += 4;
                    ((struct HomingSprite *)obj_get_sprite(obj))->offsetY = (gCosineTable[obj->bobPhase & 0xFF] / 512) << 8;
                }
            }
            else
            {
                if (obj->x - HOMING_TARGET->x < 0x6400)
                {
                    obj->heading = 0x4000;
                    obj_set_sprite_kind(obj, 0x16);
                    ((struct HomingSprite *)obj_get_sprite(obj))->offsetY = 0;
                    obj->state = HOMING_CHASE;
                }
                else
                {
                    obj->bobPhase += 4;
                    ((struct HomingSprite *)obj_get_sprite(obj))->offsetY = (gCosineTable[obj->bobPhase & 0xFF] / 512) << 8;
                }
            }
            break;
        case HOMING_CHASE:
        {
            s32 dx;
            s32 bobY;

            obj->x += (s16)obj->vx;
            obj->y += (s16)obj->vy;
            obj->bobPhase += 2;
            bobY = obj->y + ((gCosineTable[obj->bobPhase & 0xFF] / 64) >> 1);
            obj->y = bobY;
            /* turn towards a point 32 px below the character, both angles wrapped to -127..127 */
            dx = HOMING_TARGET->x - obj->x;
            bobY += 0x2000;
            angle = atan2_angle(dx, HOMING_TARGET->y - bobY);
            if (angle > 0x7F)
                angle -= 0x100;
            else if (angle < -0x7F)
                angle = 0x100 - angle;
            angle = ((s16)obj->heading >> 8) - angle;
            if (angle > 0x7F)
                angle -= 0x100;
            else if (angle < -0x7F)
                angle = 0x100 - angle;
            if (angle < 0)
                obj->heading += 0x96;
            else
                obj->heading -= 0x96;
            obj->vx = gCosineTable[obj->heading >> 8] / 64;
            obj->vy = gSineTable[obj->heading >> 8] / 64;
            /* face the character */
            if (obj->x - HOMING_TARGET->x > 0)
                ((struct SprBits11 *)obj_get_sprite(obj))->bit6 = 0;
            else
                ((struct SprBits11 *)obj_get_sprite(obj))->bit6 = 1;
            break;
        }
        case HOMING_KNOCKED:
            obj->vx = obj->vx + obj->heading;
            fall = (s16)gravity;
            fall += obj->extraGravity;
            obj->vy = obj->vy + fall;
            obj->x += (s16)obj->vx;
            y = obj->y + (s16)obj->vy;
            obj->y = y;
            /* gone once it is more than 256 px left of the view or 300 px right of it */
            if (obj->x - *view < -0x10000 || obj->x - *view > 0x12C00)
            {
                obj->x = 0;
                obj->state = HOMING_GONE;
                return 1;
            }
            obj->bobPhase += 4;
            obj->y = y + ((gCosineTable[obj->bobPhase & 0xFF] / 64) >> 1);
            break;
        }
        break;
    }
    return 0;
}

u32 obj_overlap_box(void *a, void *b);
void obj_step_pos18(void *pos, s32 delta);

// A thing on the ground that one of the heroes collects (obj_cmd_motion, obj_cmd_move_spawn).
struct Pickup
{
    u8 filler0[4];
    s32 x;
    s32 y;
    u8 filler0C[8];
    u16 velX;
    u8 filler16[2];
    u16 velY;
    u8 filler1A[2];
    u16 accelX;
    u8 filler1E[2];
    u16 weight;       // added to the frame's gravity
    u8 filler22[2];
    u8 state;         // PICKUP_*
    u8 filler25;
    u8 collector;     // 0: hero A takes it, 1: hero B
    u8 filler27[9];
    struct E0E0Table *klass;
};

struct PickupHero
{
    u8 filler0[4];
    s32 x;
    u8 filler08[0xC];
    u16 velX;
};

#define PICKUP_HERO_A (*(struct PickupHero **)(gUnk_03001020 + 0x1C98))
#define PICKUP_HERO_B (*(struct PickupHero **)(gUnk_03001020 + 0x1C9C))
// The counter a collected pickup adds one to (obj_step_pos18).
#define PICKUP_COUNTER (*(void **)(gUnk_03001020 + 0x1CB8))

// Messages a pickup's behavior gets besides the per-frame update.
enum
{
    PICKUP_MSG_START = 3,
    PICKUP_MSG_FREE = 4,
    PICKUP_MSG_JUMP_TO_HERO_A = 6, // jump so it comes down where hero A will be in 90 frames
    PICKUP_MSG_JUMP_AWAY = 0x12,   // jump and fall out of the screen
};

enum
{
    PICKUP_IDLE = 0,
    PICKUP_COLLECTED = 1, // pops up out of the hero that took it
    PICKUP_WAITING = 2,   // lies on the ground until its hero touches it
    PICKUP_LANDING = 3,   // falls until it reaches the ground
    PICKUP_LEAVING = 4,   // falls out of the screen
};

// Sound a pickup plays when it is collected.
#define SE_PICKUP 0x30
// Upward speed (-0x190 in velY's 8.8 fixed point) a pickup jumps with.
#define PICKUP_JUMP_SPEED 0xFE70

/* Behavior of a pickup whose sprite already exists (obj_cmd_move_spawn without the sprite
   creation) that one hero collects (collector 0: hero A, 1: hero B): it falls to the ground and
   waits there; touched by its hero it plays SE_PICKUP, adds one to PICKUP_COUNTER and pops up out
   of the hero until the top of its jump (returns 1, it is done). `gravity` is the frame's fall
   speed step, `ctx` gives the ground height (view_coord). */
u32 obj_cmd_motion(struct Pickup *obj, u32 ctx, u16 gravity, u32 unused3, u8 msg)
{
    switch (msg)
    {
    case PICKUP_MSG_START:
        obj->klass->unk34((u8 *)obj + obj->klass->unk30);
        *((u8 *)obj_get_sprite(obj) + 0x20) = 8;
        obj->state = PICKUP_WAITING;
        return 0;
    case PICKUP_MSG_FREE:
        obj_release_sprite(obj);
        return 0;
    case PICKUP_MSG_JUMP_AWAY:
        obj->velY = PICKUP_JUMP_SPEED;
        obj->state = PICKUP_LEAVING;
        return 0;
    case PICKUP_MSG_JUMP_TO_HERO_A:
        if (obj->state == PICKUP_LANDING)
            return 0;
        obj->velX = (PICKUP_HERO_A->x + (s16)PICKUP_HERO_A->velX * 90 - obj->x) / 90;
        obj->velY = PICKUP_JUMP_SPEED;
        obj->state = PICKUP_LANDING;
        return 0;
    }

    switch (obj->state)
    {
    case PICKUP_IDLE:
        return 0;
    case PICKUP_COLLECTED:
    {
        s32 fall;

        obj->velX = obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += (s16)obj->velX;
        obj->y += (s16)obj->velY;
        // Done at the top of the jump (velY's high byte reaches 0).
        if (*(s8 *)((u8 *)obj + 0x19) == 0)
        {
            obj->state = PICKUP_IDLE;
            return 1;
        }
        return 0;
    }
    case PICKUP_WAITING:
        if (obj->collector == 0)
        {
            if (obj_overlap_box(obj, PICKUP_HERO_A) == 0)
                return 0;
            set_slot_value(SE_PICKUP, -1);
            obj_step_pos18(PICKUP_COUNTER, 1);
            obj->velY = PICKUP_JUMP_SPEED;
            obj->velX = PICKUP_HERO_A->velX;
            obj->x = PICKUP_HERO_A->x;
            ((struct ChildSprite *)obj_get_sprite(obj))->unkE = 0x13;
            obj_set_sprite_kind(obj, 0x26);
            obj->state = PICKUP_COLLECTED;
        }
        else if (obj->collector == 1)
        {
            if (obj_overlap_box(obj, PICKUP_HERO_B) == 0)
                return 0;
            set_slot_value(SE_PICKUP, -1);
            obj_step_pos18(PICKUP_COUNTER, 1);
            obj->velY = PICKUP_JUMP_SPEED;
            obj->velX = PICKUP_HERO_B->velX;
            obj->x = PICKUP_HERO_B->x;
            ((struct ChildSprite *)obj_get_sprite(obj))->unkE = 0x1D;
            obj_set_sprite_kind(obj, 0x26);
            obj->state = PICKUP_COLLECTED;
        }
        return 0;
    case PICKUP_LEAVING:
    {
        s32 fall;

        obj->velX = obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += (s16)obj->velX;
        obj->y += (s16)obj->velY;
        // Gone once it has fallen below the screen.
        if (obj->y > 0x10000)
            return 1;
        return 0;
    }
    case PICKUP_LANDING:
    {
        s32 fall;

        obj->velX = obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += (s16)obj->velX;
        obj->y += (s16)obj->velY;
        // Lands on the ground height under it (whole pixels, to 8.8 fixed point).
        if (obj->y > (s32)(view_coord(ctx, obj->x) << 16) >> 8)
        {
            obj->y = (s32)(view_coord(ctx, obj->x) << 16) >> 8;
            obj->state = PICKUP_WAITING;
        }
        return 0;
    }
    }
    return 0;
}

/* Behavior of a pickup with a sprite of its own (made on PICKUP_MSG_START, freed on PICKUP_MSG_FREE)
   that one hero collects (collector 0: hero A, 1: hero B): it falls to the ground and waits there;
   touched by its hero it plays SE_PICKUP, adds one to PICKUP_COUNTER and pops up out of the hero
   until the top of its jump (returns 1, it is done). `gravity` is the frame's fall speed step,
   `ctx` gives the ground height (view_coord). */
u32 obj_cmd_move_spawn(struct Pickup *obj, u32 ctx, u16 gravity, u32 unused3, u8 msg)
{
    switch (msg)
    {
    case PICKUP_MSG_START:
        obj_create_sprite(obj, 0);
        obj->klass->unk34((u8 *)obj + obj->klass->unk30);
        *((u8 *)obj_get_sprite(obj) + 0x20) = 8;
        obj->state = PICKUP_WAITING;
        return 0;
    case PICKUP_MSG_FREE:
        obj_release_sprite(obj);
        return 0;
    case PICKUP_MSG_JUMP_AWAY:
        obj->velY = PICKUP_JUMP_SPEED;
        obj->state = PICKUP_LEAVING;
        return 0;
    case PICKUP_MSG_JUMP_TO_HERO_A:
        if (obj->state == PICKUP_LANDING)
            return 0;
        obj->velX = (PICKUP_HERO_A->x + (s16)PICKUP_HERO_A->velX * 90 - obj->x) / 90;
        obj->velY = PICKUP_JUMP_SPEED;
        obj->state = PICKUP_LANDING;
        return 0;
    }

    switch (obj->state)
    {
    case PICKUP_IDLE:
        return 0;
    case PICKUP_COLLECTED:
    {
        s32 fall;

        obj->velX = obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += (s16)obj->velX;
        obj->y += (s16)obj->velY;
        // Done at the top of the jump (velY's high byte reaches 0).
        if (*(s8 *)((u8 *)obj + 0x19) == 0)
        {
            obj->state = PICKUP_IDLE;
            return 1;
        }
        return 0;
    }
    case PICKUP_WAITING:
        if (obj->collector == 0)
        {
            if (obj_overlap_box(obj, PICKUP_HERO_A) == 0)
                return 0;
            set_slot_value(SE_PICKUP, -1);
            obj_step_pos18(PICKUP_COUNTER, 1);
            obj->velY = PICKUP_JUMP_SPEED;
            obj->velX = PICKUP_HERO_A->velX;
            obj->x = PICKUP_HERO_A->x;
            ((struct ChildSprite *)obj_get_sprite(obj))->unkE = 0x13;
            obj_set_sprite_kind(obj, 0x26);
            obj->state = PICKUP_COLLECTED;
        }
        else if (obj->collector == 1)
        {
            if (obj_overlap_box(obj, PICKUP_HERO_B) == 0)
                return 0;
            set_slot_value(SE_PICKUP, -1);
            obj_step_pos18(PICKUP_COUNTER, 1);
            obj->velY = PICKUP_JUMP_SPEED;
            obj->velX = PICKUP_HERO_B->velX;
            obj->x = PICKUP_HERO_B->x;
            ((struct ChildSprite *)obj_get_sprite(obj))->unkE = 0x1D;
            obj_set_sprite_kind(obj, 0x26);
            obj->state = PICKUP_COLLECTED;
        }
        return 0;
    case PICKUP_LEAVING:
    {
        s32 fall;

        obj->velX = obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += (s16)obj->velX;
        obj->y += (s16)obj->velY;
        // Gone once it has fallen below the screen.
        if (obj->y > 0x10000)
            return 1;
        return 0;
    }
    case PICKUP_LANDING:
    {
        s32 fall;

        obj->velX = obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += (s16)obj->velX;
        obj->y += (s16)obj->velY;
        // Lands on the ground height under it (whole pixels, to 8.8 fixed point).
        if (obj->y > (s32)(view_coord(ctx, obj->x) << 16) >> 8)
        {
            obj->y = (s32)(view_coord(ctx, obj->x) << 16) >> 8;
            obj->state = PICKUP_WAITING;
        }
        return 0;
    }
    }
    return 0;
}

/*
 * Sets up spawn kind 2 (obj_spawn_by_kind): class gClassE8a0, unk3C = param, its other fields
 * cleared, its own sprite and a child sprite with record 0x27. The child gets unkE from unk2C and
 * unk2D in all four 2-bit fields of its byte 0x1F (g0, g2, g4, g6). Returns obj.
 */
u32 obj_init_e8a0(struct InitObj *obj, u32 rec, u16 param)
{
    struct ChildSprite *child;

    obj_init_from_record(obj, rec);
    obj->cls30 = (struct MotionClass *)gClassE8a0;
    obj->unk3C = param;
    obj->unk34 = 0;
    obj->unk24 = 0;
    obj->unk40 = 0;
    obj->unk41 = 0;
    obj->child38 = 0;
    obj->unk3E = 0;
    obj_create_sprite(obj, 0);
    child = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj->child38 = (struct ChildSprite *)child;
    obj_load_record(child, 0x27, -1, 0, 0);
    obj->child38->unkE = obj->unk2C;
    obj->child38->g0 = obj->unk2D;
    obj->child38->g2 = obj->unk2D;
    obj->child38->g4 = obj->unk2D;
    obj->child38->g6 = obj->unk2D;
    return (u32)obj;
}

ASM_FUNC("asm/code_0815CA80/obj_cmd_home.s", void obj_cmd_home(void))

/*
 * Sets up spawn kind 12 (heap tag "JUMP"): class gClassE620, unk38 = param, its own sprite and a
 * child sprite with record 0x30 + the game's level (levels above 3 use 0x30). The child sits at the
 * sprite's position with unkE 0x13 and copies the four 2-bit fields of its byte 0x1F (g0, g2, g4,
 * g6) one by one. Returns obj.
 */
u32 obj_init_e620(struct E620Obj *obj, u32 rec, u16 param)
{
    struct SpriteRef *sprite;
    struct ChildSprite *child;
    u32 zero;
    s32 none;
    u8 *level;
    u32 bits;
    s32 cur;

    obj_init_from_record(obj, rec);
    obj->cls30 = (struct MotionClass *)gClassE620;
    zero = 0;
    obj->unk38 = param;
    obj_create_sprite(obj, 0);
    obj->child34 = (struct ChildSprite *)create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    none = -1;
    level = &GAME_LEVEL;
    if (*level <= 3)
    {
        obj_load_record(obj->child34, *level + 0x30, none, 0, zero);
    }
    else
    {
        obj_load_record(obj->child34, 0x30, none, 0, zero);
    }
    sprite_update_cd0(obj->child34);
    ((struct SprBits12 *)obj->child34)->bit5 = 1;
    sprite = obj_get_sprite(obj);
    obj->child34->unk0 = sprite->x;
    sprite = obj_get_sprite(obj);
    obj->child34->unk2 = sprite->y;
    obj->child34->unkE = 0x13;

    /* child->g0 = sprite's g0 */
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = sprite->flags1F;
    bits = bits << 30;
    bits = bits >> 30;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & -4) | bits;
    ((u8 *)child)[0x1F] = cur;
    /* g2 */
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = sprite->flags1F & 0xC;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & -13) | bits;
    ((u8 *)child)[0x1F] = cur;
    /* g4 */
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = sprite->flags1F & 0x30;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & -0x31) | bits;
    ((u8 *)child)[0x1F] = cur;
    /* g6 */
    sprite = obj_get_sprite(obj);
    child = obj->child34;
    bits = sprite->flags1F;
    bits = bits >> 6;
    bits = bits << 6;
    cur = ((u8 *)child)[0x1F];
    cur = (cur & 0x3F) | bits;
    ((u8 *)child)[0x1F] = cur;
    return (u32)obj;
}

ASM_FUNC("asm/code_0815CA80/obj_cmd_follow.s", void obj_cmd_follow(void))

/*
 * Command handler of an object that paces and then hops away. mode 3 runs the class's slot-0x30
 * method and starts pacing, mode 4 releases its sprite, any other mode runs one frame. Pacing
 * (state 1), it swings left and right (x speed = 2/3 of a cosine wave, facing where it goes)
 * until the lead character is less than 40 px left of it; then it switches to sprite kind 0x35
 * and jumps (y speed -0x3E8). Jumping (state 2), it falls with the scene's gravity plus unk20,
 * changes its sprite's four 2-bit fields to 2 once it falls, and faces the character.
 * unk14/unk18 are its 8.8 x/y speed, unk1C the wave phase (state 1) or x acceleration (state 2).
 * Always returns 0.
 */
u32 obj_cmd_fly(struct MotionObj *obj, u32 unused1, u16 gravity, u32 unused3, u8 mode)
{
    struct MotionClass *cls;
    s32 fall;
    u16 phase;

    switch (mode)
    {
    case 3:
        cls = obj->cls30;
        cls->slot30.fn((u8 *)obj + cls->slot30.offset);
        obj->unk24 = 1;
        break;
    case 4:
        obj_release_sprite(obj);
        break;
    default:
        switch (obj->unk24)
        {
        case 0:
            break;
        case 1:
            obj->x += (s16)obj->unk14;
            phase = obj->unk1C;
            obj->unk14 = gCosineTable[(u8)phase] / 64 * 2 / 3;
            obj->unk1C = (phase + 1) & 0xFF;
            if ((s16)obj->unk14 > 0)
                ((struct SprBits11 *)obj_get_sprite(obj))->bit6 = 1;
            else
                ((struct SprBits11 *)obj_get_sprite(obj))->bit6 = 0;
            if (LEAD_CHAR->x - obj->x > -0x2800)
            {
                obj_set_sprite_kind(obj, 0x35);
                obj->unk1C = 0;
                obj->unk20 = 0;
                obj->unk18 = 0xFC18;
                obj->unk14 = 0;
                obj->unk24 = 2;
            }
            break;
        case 2:
            obj->unk14 += obj->unk1C;
            fall = (s16)gravity + obj->unk20;
            obj->unk18 = obj->unk18 + fall;
            obj->x += (s16)obj->unk14;
            obj->y += (s16)obj->unk18;
            if ((s16)obj->unk18 > 0)
            {
                ((struct ChildSprite *)obj_get_sprite(obj))->g0 = 2;
                ((struct ChildSprite *)obj_get_sprite(obj))->g2 = 2;
                ((struct ChildSprite *)obj_get_sprite(obj))->g4 = 2;
                ((struct ChildSprite *)obj_get_sprite(obj))->g6 = 2;
            }
            /* face the character */
            if (LEAD_CHAR->x - obj->x > 0)
                ((struct SprBits11 *)obj_get_sprite(obj))->bit6 = 1;
            else
                ((struct SprBits11 *)obj_get_sprite(obj))->bit6 = 0;
            break;
        }
        break;
    }
    return 0;
}

/*
 * Command handler of an object that keeps hopping from one spot (unk38, unk3C). mode 3 runs the
 * class's slot-0x30 method and starts the first hop (y speed -500), mode 4 runs the slot-0x38 method
 * and stops it (state 2), mode 5 does nothing, any other mode runs one frame. Hopping (state 1), it
 * moves by its 8.8 speeds (unk14 x, unk18 y); x speed grows by unk1C, y speed by the scene's gravity
 * plus unk20. Once it falls below its spot it is put back there and hops again. Always returns 0.
 */
u32 obj_cmd_drop(struct MotionObj *obj, u32 unused1, u16 gravity, u32 unused3, u8 mode)
{
    struct MotionClass *cls;
    s32 fall;
    s32 y;

    switch (mode)
    {
    case 3:
        cls = obj->cls30;
        cls->slot30.fn((u8 *)obj + cls->slot30.offset);
        obj->unk18 = 0xFE0C;
        obj->unk24 = 1;
        break;
    case 4:
        cls = obj->cls30;
        cls->slot38.fn((u8 *)obj + cls->slot38.offset);
        obj->unk24 = 2;
        break;
    case 5:
        break;
    default:
        if (obj->unk24 == 1)
        {
            obj->unk14 += obj->unk1C;
            fall = (s16)gravity + obj->unk20;
            obj->unk18 = obj->unk18 + fall;
            obj->x += (s16)obj->unk14;
            y = obj->y + (s16)obj->unk18;
            obj->y = y;
            if (y > obj->unk3C)
            {
                s32 homeX = obj->unk38;
                s32 homeY = *(volatile s32 *)&obj->unk3C;

                obj->x = homeX;
                obj->y = homeY;
                obj->unk18 = 0xFE0C;
            }
        }
        break;
    }
    return 0;
}

static inline void *spawn_5008(void)
{
    return create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
}

NONMATCH("asm/code_0815CA80/sprite_group4_init.s", struct SpriteGroup *sprite_group4_init(struct SpriteGroup *g))
{
    u16 k;

    g->unk18 = 0;
    g->child8 = 0;
    g->child4 = 0;
    g->child0 = 0;
    g->unk1A = 0;
    g->unk1B = 0;
    g->unk1C = 0;
    g->unk1D = 0;
    g->unk10 = 0xE800;
    g->unk14 = 0x1400;
    g->child0 = spawn_5008();
    obj_load_record(g->child0, 0xA, -1, 0, 0);
    sprite_update_cd0(g->child0);
    g->child0->unk0 = 0xD8;
    g->child0->unk2 = k = 0x10;
    g->child4 = spawn_5008();
    obj_load_record(g->child4, 0xA, -1, 0, 0);
    sprite_update_cd0(g->child4);
    g->child4->unk0 = 0xE0;
    g->child4->unk2 = k;
    g->child8 = spawn_5008();
    obj_load_record(g->child8, 0xA, -1, 0, 0);
    sprite_update_cd0(g->child8);
    g->child8->unk0 = 0xE8;
    g->child8->unk2 = k;
    g->childC = spawn_5008();
    obj_load_record(g->childC, 0x26, -1, 0, 0);
    sprite_update_cd0(g->childC);
    g->childC->unk0 = 0xFFF0;
    g->childC->unk2 = 0xFFF0;
    return g;

}
END_NONMATCH

/*
 * Per-frame update of the score display: reloads each digit sprite whose digit of the value changed,
 * then runs the current effect until its timer runs out (SCORE_EFFECT_SHAKE: the digits jitter 2 px;
 * SCORE_EFFECT_SWING: the marker swings around its base x), parking the marker off screen after it.
 */
void obj_update_sprites_e130(struct SpriteGroup *obj)
{
    s32 minusTwo;
    struct ChildSprite *hundreds;
    struct ChildSprite *tens;
    struct ChildSprite *ones;
    s32 value;
    s32 digit;
    s32 timer;
    s32 x;
    s32 cos;

    value = (s16)obj->unk18;
    digit = (s16)(value / 100);
    hundreds = (struct ChildSprite *)obj->child0;
    if (digit != (s32)hundreds->unk21 - SCORE_DIGIT_RECORD)
    {
        obj_load_record(hundreds, digit + SCORE_DIGIT_RECORD, 0, 0, 0);
    }
    value = (s16)obj->unk18;
    digit = (s16)(value / 10);
    tens = (struct ChildSprite *)obj->child4;
    if (digit != (s32)tens->unk21 - SCORE_DIGIT_RECORD)
    {
        obj_load_record(tens, (s16)(digit % 10) + SCORE_DIGIT_RECORD, 0, 0, 0);
    }
    value = (s16)obj->unk18;
    digit = (s16)(value % 10);
    ones = (struct ChildSprite *)obj->child8;
    if (digit != (s32)ones->unk21 - SCORE_DIGIT_RECORD)
    {
        obj_load_record(ones, digit + SCORE_DIGIT_RECORD, 0, 0, 0);
    }
    switch (obj->unk1A)
    {
    case SCORE_EFFECT_NONE:
        break;
    case SCORE_EFFECT_SHAKE:
        timer = obj->unk1B - 1;
        obj->unk1B = timer;
        timer = (u8)timer;
        if (timer == 0)
        {
            obj->unk1A = timer;
        }
        else
        {
            obj->childC->unk0 = SPRITE_OFFSCREEN;
            obj->childC->unk2 = SPRITE_OFFSCREEN;
            obj->unk1C = 4 - obj->unk1C;
            minusTwo = 0xFFFE; /* -2 in the u16 x */
            obj->child0->unk0 = obj->unk1C + minusTwo + obj->child0->unk0;
            obj->child4->unk0 = obj->unk1C + minusTwo + obj->child4->unk0;
            obj->child8->unk0 = obj->unk1C + minusTwo + obj->child8->unk0;
        }
        break;
    case SCORE_EFFECT_SWING:
        timer = obj->unk1B - 1;
        obj->unk1B = timer;
        if ((u8)timer == 0)
        {
            obj->childC->unk0 = SPRITE_OFFSCREEN;
            obj->childC->unk2 = SPRITE_OFFSCREEN;
            obj->unk1A = SCORE_EFFECT_NONE;
        }
        else
        {
            obj->unk1D = obj->unk1D + 1;
            x = obj->unk10;
            cos = gCosineTable[obj->unk1D];
            x = x + (cos / 0x40) * 8;
            obj->childC->unk0 = x >> 8;
            obj->childC->unk2 = obj->unk14 >> 8;
        }
        break;
    }
}
