#include "global.h"
#include "ram_routines.h"

struct Char {
    u8 filler0[4];
    u32 unk04;
    s32 unk08;
    u8 filler0C[8];
    u16 unk14;
};

struct CharSpeedS {
    u8 filler0[0x14];
    s16 unk14;
};

struct GameState {
    u8 filler0[0x1C98];
    struct Char *charA;
    struct Char *charB;
    u8 filler1CA0[0x1CB0 - 0x1CA0];
    void *unk1CB0;
    void *unk1CB4;
    u8 filler1CB8[0x1CC4 - 0x1CB8];
    u16 unk1CC4;
    u8 filler1CC6[0x1F1C - 0x1CC6];
    u16 unk1F1C; /* initial jump speed of the walker (StateObj), 500 */
    u8 filler1F1E[2];
    u8 unk1F20;
};

extern struct GameState *gGamePtr;

struct Sprite {
    u16 unk00; /* screen x */
    u16 unk02; /* screen y */
};

/* A three-digit counter on screen (obj_create_digits, obj_add_value, obj_update_digits). */
struct DigitObj {
    struct Sprite *unk00; /* hundreds digit */
    struct Sprite *unk04; /* tens digit */
    struct Sprite *unk08; /* ones digit */
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C; /* the value shown */
    u32 unk20; /* 0 still, 1 shaking (value went down), 2 blinking (value went up) */
    u32 unk24; /* frames left of the shake or blink */
    u32 unk28; /* shake: x offset, 0 or 4 */
    u32 unk2C;
    u32 unk30; /* first digit record: 0, or 10 for the other colour while blinking */
    u32 unk34; /* blink: frames until the colour flips */
};

struct TrackObj {
    u8 filler0[4];
    u32 unk04;
    u8 filler8[0xC];
    u16 unk14;
    u8 filler16[0x22];
    s16 unk38;
};

struct SubObj {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u8 filler08[0x4];
    u16 unkC;
    u16 unkE;
    u8 filler10;
    u8 unk11;
    u8 unk12;
    u8 filler13[0xC];
    u8 unk1F;
};

extern const u8 gTransitionClassE080[];
struct SubObj *obj_get_sprite(void *obj);

struct FlagBit5 {
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct SubFlagBits {
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

extern const u8 gClassE380[];
u32 mt19937_next(void);
s32 rand_below(s32 n, u32 seed);
void obj_init_from_record(void *obj, u32 arg);
void obj_create_sprite(void *obj, u32 zero);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);

void obj_update_digits(struct DigitObj *obj);
void set_slot_value(u32 slot, s32 value);
void sprite_update_cd0(void *sprite);
void release_obj_131c(struct Sprite *sprite);
void obj_set_sprite_kind(void *obj, u32 kind);
void obj_release_sprite(void *obj);
u32 hitboxes_overlap(void *a, void *b);
void obj_add_value(void *a, s32 b);
u32 view_coord(u32 a, s32 b);

struct E0E0Table {
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

/* Messages an object's behavior gets besides the per-frame update (0). */
enum {
    OBJ_MSG_START = 3,
    OBJ_MSG_FREE = 4,
    OBJ_MSG_JUMP_TO_HERO_A = 6, // jump so it comes down where hero A will be in 90 frames
    OBJ_MSG_JUMP_AWAY = 0x12,   // jump and fall out of the screen
};

// What a behavior function returns.
enum {
    OBJ_KEEP = 0,
    OBJ_DONE = 1,     // remove it
    OBJ_MISSED = 2,   // left behind by the bros
};

enum {
    PICKUP_IDLE = 0,
    PICKUP_COLLECTED = 1, // pops up out of the hero that took it
    PICKUP_WAITING = 2,   // lies on the ground until its hero touches it
    PICKUP_LANDING = 3,   // falls until it reaches the ground
    PICKUP_LEAVING = 4,   // falls out of the screen
};

// Sound a pickup plays when it is collected.
#define SE_PICKUP 0x3B
// Upward speed (-0x190 in velY's 8.8 fixed point) a pickup jumps with.
#define PICKUP_JUMP_SPEED 0xFE70

// A thing on the ground that one of the heroes collects (obj_behavior_e160, obj_behavior_e1a0).
struct Pickup {
    struct SubObj *unk00;
    s32 x;
    s32 y;
    u8 filler0C[8];
    s16 velX;
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
    u8 filler34[4];
    s32 unk38;
    s32 unk3C;
};

ASM_FUNC("asm/code_08154490/obj_behavior_e1e0.s", void obj_behavior_e1e0(void))

/* Behavior of a pickup that one hero collects (collector 0: hero A, 1: hero B): it falls to the
   ground and waits there; touched by its hero it plays SE_PICKUP, adds one to the counter at
   gGamePtr->unk1CB0 and pops up out of the hero until the top of its jump (returns OBJ_DONE).
   Left more than 0x3000 behind its hero it returns OBJ_MISSED. `gravity` is the frame's fall
   speed step, `ctx` gives the ground height (view_coord). */
u32 obj_behavior_e1a0(struct Pickup *obj, u32 ctx, u16 gravity, u32 unused3, u8 msg)
{
    struct Char *hero;
    struct GameState *game;

    switch (msg) {
    case OBJ_MSG_START:
        obj->klass->unk34((u8 *)obj + obj->klass->unk30);
        *((u8 *)obj_get_sprite(obj) + 0x20) = 8;
        obj->state = PICKUP_WAITING;
        return OBJ_KEEP;
    case OBJ_MSG_FREE:
        return OBJ_KEEP;
    case OBJ_MSG_JUMP_AWAY:
        obj->velY = 0xFE70;
        obj->state = PICKUP_LEAVING;
        return OBJ_KEEP;
    case OBJ_MSG_JUMP_TO_HERO_A:
        if (obj->state == PICKUP_LANDING)
            return OBJ_KEEP;
        obj->velX = ((s32)gGamePtr->charA->unk04 + ((struct CharSpeedS *)gGamePtr->charA)->unk14 * 90 - obj->x) / 90;
        obj->velY = 0xFE70;
        obj->state = PICKUP_LANDING;
        return OBJ_KEEP;
    }

    switch (obj->state) {
    case PICKUP_IDLE:
        return OBJ_KEEP;
    case PICKUP_COLLECTED: {
        s32 fall;

        obj->velX = (u16)obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += obj->velX;
        obj->y += (s16)obj->velY;
        // Done at the top of the jump (velY's high byte reaches 0).
        if (*(s8 *)((u8 *)obj + 0x19) == 0) {
            obj->state = PICKUP_IDLE;
            return OBJ_DONE;
        }
        return OBJ_KEEP;
    }
    case PICKUP_WAITING: {
        if (obj->collector == 0) {
            if (hitboxes_overlap(obj, gGamePtr->charA) != 0) {
                set_slot_value(SE_PICKUP, -1);
                obj_add_value(gGamePtr->unk1CB0, 1);
                obj->velY = 0xFE70;
                obj->velX = gGamePtr->charA->unk14;
                obj->x = gGamePtr->charA->unk04;
                obj_get_sprite(obj)->unkE = 0x13;
                obj_set_sprite_kind(obj, 0x26);
                obj->state = PICKUP_COLLECTED;
                return OBJ_KEEP;
            }
            game = gGamePtr;
            hero = game->charA;
        } else if (obj->collector == 1) {
            if (hitboxes_overlap(obj, gGamePtr->charB) != 0) {
                set_slot_value(SE_PICKUP, -1);
                obj_add_value(gGamePtr->unk1CB0, 1);
                obj->velY = 0xFE70;
                obj->velX = gGamePtr->charB->unk14;
                obj->x = gGamePtr->charB->unk04;
                obj_get_sprite(obj)->unkE = 0x1D;
                obj_set_sprite_kind(obj, 0x26);
                obj->state = PICKUP_COLLECTED;
                return OBJ_KEEP;
            }
            game = gGamePtr;
            hero = game->charB;
        } else {
            return OBJ_KEEP;
        }
        if ((s32)hero->unk04 - obj->x > 0x3000 && (game->unk1CC4 & 0x20) != 0)
            return OBJ_MISSED;
        return OBJ_KEEP;
    }
    case PICKUP_LEAVING: {
        s32 fall;

        obj->velX = (u16)obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += obj->velX;
        obj->y += (s16)obj->velY;
        if (obj->y > 0x10000)
            return OBJ_DONE;
        return OBJ_KEEP;
    }
    case PICKUP_LANDING: {
        s32 fall;

        obj->velX = (u16)obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += obj->velX;
        obj->y += (s16)obj->velY;
        if (obj->y > (s32)(view_coord(ctx, obj->x) << 16) >> 8) {
            obj->y = (s32)(view_coord(ctx, obj->x) << 16) >> 8;
            obj->state = PICKUP_WAITING;
        }
        return OBJ_KEEP;
    }
    }
    return OBJ_KEEP;
}

struct Obj;


struct E0E0Char {
    u8 filler0[4];
    s32 unk04;
    s32 unk08;
    u8 filler0C[0x18];
    u8 unk24;
    u8 filler25[0xB];
    struct E0E0Table *unk30;
    struct SubObj *unk34;
    u16 unk38;
};

struct CharPos {
    u8 filler0[4];
    s32 unk04;
    u8 filler08[0xC];
    u16 unk14;
};


/* Behavior of a pickup with a sprite of its own (made on OBJ_MSG_START, freed on OBJ_MSG_FREE)
   that one hero collects (collector 0: hero A, 1: hero B): it falls to the ground and waits there;
   touched by its hero it plays SE_PICKUP, adds one to the counter at gGamePtr->unk1CB0 and pops up
   out of the hero until the top of its jump (returns OBJ_DONE). Unlike obj_behavior_e1a0 it is
   never missed. `gravity` is the frame's fall speed step, `ctx` gives the ground height. */
u32 obj_behavior_e160(struct Pickup *obj, u32 ctx, u16 gravity, u32 unused3, u8 msg)
{
    switch (msg) {
    case OBJ_MSG_START:
        obj_create_sprite(obj, 0);
        obj->klass->unk34((u8 *)obj + obj->klass->unk30);
        *((u8 *)obj_get_sprite(obj) + 0x20) = 8;
        obj->state = PICKUP_WAITING;
        return OBJ_KEEP;
    case OBJ_MSG_FREE:
        obj_release_sprite(obj);
        return OBJ_KEEP;
    case OBJ_MSG_JUMP_AWAY:
        obj->velY = PICKUP_JUMP_SPEED;
        obj->state = PICKUP_LEAVING;
        return OBJ_KEEP;
    case OBJ_MSG_JUMP_TO_HERO_A:
        if (obj->state == PICKUP_LANDING)
            return OBJ_KEEP;
        obj->velX = ((s32)gGamePtr->charA->unk04 + ((struct CharSpeedS *)gGamePtr->charA)->unk14 * 90 - obj->x) / 90;
        obj->velY = PICKUP_JUMP_SPEED;
        obj->state = PICKUP_LANDING;
        return OBJ_KEEP;
    }

    switch (obj->state) {
    case PICKUP_IDLE:
        return OBJ_KEEP;
    case PICKUP_COLLECTED: {
        s32 fall;

        obj->velX = (u16)obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += obj->velX;
        obj->y += (s16)obj->velY;
        // Done at the top of the jump (velY's high byte reaches 0).
        if (*(s8 *)((u8 *)obj + 0x19) == 0) {
            obj->state = PICKUP_IDLE;
            return OBJ_DONE;
        }
        return OBJ_KEEP;
    }
    case PICKUP_WAITING:
        if (obj->collector == 0) {
            if (hitboxes_overlap(obj, gGamePtr->charA) == 0)
                return OBJ_KEEP;
            set_slot_value(SE_PICKUP, -1);
            obj_add_value(gGamePtr->unk1CB0, 1);
            obj->velY = PICKUP_JUMP_SPEED;
            obj->velX = gGamePtr->charA->unk14;
            obj->x = gGamePtr->charA->unk04;
            obj_get_sprite(obj)->unkE = 0x13;
            obj_set_sprite_kind(obj, 0x26);
            obj->state = PICKUP_COLLECTED;
        } else if (obj->collector == 1) {
            if (hitboxes_overlap(obj, gGamePtr->charB) == 0)
                return OBJ_KEEP;
            set_slot_value(SE_PICKUP, -1);
            obj_add_value(gGamePtr->unk1CB0, 1);
            obj->velY = PICKUP_JUMP_SPEED;
            obj->velX = gGamePtr->charB->unk14;
            obj->x = gGamePtr->charB->unk04;
            obj_get_sprite(obj)->unkE = 0x1D;
            obj_set_sprite_kind(obj, 0x26);
            obj->state = PICKUP_COLLECTED;
        }
        return OBJ_KEEP;
    case PICKUP_LEAVING: {
        s32 fall;

        obj->velX = (u16)obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += obj->velX;
        obj->y += (s16)obj->velY;
        // Gone once it has fallen below the screen.
        if (obj->y > 0x10000)
            return OBJ_DONE;
        return OBJ_KEEP;
    }
    case PICKUP_LANDING: {
        s32 fall;

        obj->velX = (u16)obj->velX + obj->accelX;
        fall = (s16)gravity;
        fall += obj->weight;
        obj->velY = obj->velY + fall;
        obj->x += obj->velX;
        obj->y += (s16)obj->velY;
        // Lands on the ground height under it (whole pixels, to 8.8 fixed point).
        if (obj->y > (s32)(view_coord(ctx, obj->x) << 16) >> 8) {
            obj->y = (s32)(view_coord(ctx, obj->x) << 16) >> 8;
            obj->state = PICKUP_WAITING;
        }
        return OBJ_KEEP;
    }
    }
    return OBJ_KEEP;
}


struct Bits8 {
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

/* An enemy (class E380, made by obj_init_e380) that floats towards hero A and vanishes
   when it stays in the light circle around hero A. Positions are 8.8 pixels. 0x44 bytes. */
struct LightChaser {
    struct SubObj *sprite;  /* unk04/unk06: scale (0x100 = full size), unk11 bit 6: mirror */
    s32 posX;
    s32 posY;
    u8 filler0C[8];
    s16 velX;
    u8 filler16[2];
    u16 velY;
    u8 filler1A[6];
    u16 fallAccel;          /* added to the frame's gravity while falling */
    u8 filler22[2];
    u8 state;               /* LIGHT_CHASER_* */
    u8 filler25[7];
    u8 unk2C;               /* unk2C/unk2D: bytes 0xA/0xB of the init record */
    u8 unk2D;
    u8 filler2E[2];
    struct E0E0Table *klass;
    u32 unk34;
    struct SubObj *poof;    /* puff shown where it vanishes */
    u16 kind;
    u16 bobPhase;           /* gSineTable index of the up-and-down bob */
    u8 lightTimer;          /* frames spent in the light */
    u8 shake;               /* 0 or 4: x jitter while in the light */
};

enum {
    LIGHT_CHASER_GONE = 0,
    LIGHT_CHASER_CHASE = 2,
    LIGHT_CHASER_FALL = 4,
    LIGHT_CHASER_POOF = 5,
    LIGHT_CHASER_IN_LIGHT = 6,
};

/* Sets up a light chaser (class E380) of the given kind from its init record: a random bob phase,
   its sprite, and the puff it shows where it vanishes (record 0x27; record bytes 0xA and 0xB go to
   the puff's unkE and to each of its four 2-bit fields at +0x1F). */
struct LightChaser *obj_init_e380(struct LightChaser *obj, u32 initArg, u16 kind)
{
    obj_init_from_record(obj, initArg);
    obj->klass = (struct E0E0Table *)gClassE380;
    obj->kind = kind;
    obj->unk34 = 0;
    obj->state = LIGHT_CHASER_GONE;
    obj->lightTimer = 0;
    obj->shake = 0;
    obj->poof = NULL;
    obj->bobPhase = rand_below(0xFF, mt19937_next());
    obj_create_sprite(obj, 0);
    obj->poof = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    obj_load_record(obj->poof, 0x27, -1, 0, 0);
    obj->poof->unkE = obj->unk2C;
    ((struct SubFlagBits *)&obj->poof->unk1F)->f0 = obj->unk2D;
    ((struct SubFlagBits *)&obj->poof->unk1F)->f2 = obj->unk2D;
    ((struct SubFlagBits *)&obj->poof->unk1F)->f4 = obj->unk2D;
    ((struct SubFlagBits *)&obj->poof->unk1F)->f6 = obj->unk2D;
    return obj;
}


extern s16 gSineTable[];
u32 obj_point_in_iris(void *iris, void *a, void *b);

/* Behavior of the light chaser: grows in, then floats towards hero A (bobbing, facing its
   way, sending hero A message 5 on touch). Caught in the light it shakes, and after 30
   frames shrinks to a puff and is gone (returns 1). Out of the light again, it chases on.
   `gravity` is the frame's fall speed step. */
u32 obj_behavior_e360(struct LightChaser *obj, u32 ctx, u16 gravity, u32 arg3, u8 msg)
{
    switch (msg) {
    case OBJ_MSG_START:
        obj->klass->unk34((u8 *)obj + obj->klass->unk30);
        obj->state = LIGHT_CHASER_CHASE;
        return 0;
    case OBJ_MSG_FREE:
        obj_release_sprite(obj);
        return 0;
    case 1:
    case 2:
        return 0;
    }

    switch (obj->state) {
    case LIGHT_CHASER_POOF:
        if ((obj->poof->unk12 & 8) == 0) {
            obj->poof->unk00 = obj->sprite->unk00;
            obj->poof->unk02 = obj->sprite->unk02;
            ((struct FlagBit5 *)&obj->sprite->unk12)->bit5 = 1;
            return 0;
        }
        ((struct FlagBit5 *)&obj->poof->unk12)->bit5 = 1;
        obj->state = LIGHT_CHASER_GONE;
        return 1;
    case LIGHT_CHASER_FALL: {
        s32 t;

        obj->posY += (s16)obj->velY;
        t = (s16)gravity + obj->fallAccel;
        obj->velY = obj->velY + t;
        obj->fallAccel = 0;
        break;
    }
    case LIGHT_CHASER_IN_LIGHT:
        if (obj_point_in_iris(gGamePtr->unk1CB4, &obj->posX, &((struct E0E0Char *)gGamePtr->charA)->unk04) == 0) {
            obj_set_sprite_kind(obj, 2);
            obj->state = LIGHT_CHASER_CHASE;
            return 0;
        }
        obj->lightTimer += 1;
        if (obj->lightTimer > 0x1D) {
            obj->sprite->unk04 -= 4;
            obj->sprite->unk06 -= 4;
            if ((s16)obj->sprite->unk04 <= 0x20) {
                obj->poof->unk00 = obj->sprite->unk00;
                obj->poof->unk02 = obj->sprite->unk02;
                sprite_update_cd0(obj->poof);
                obj->state = LIGHT_CHASER_POOF;
                return 0;
            }
        }
        obj->shake = 4 - obj->shake;
        obj->posX += ((s8)obj->shake - 2) << 8;
        return 0;
    case LIGHT_CHASER_CHASE: {
        struct E0E0Table *heroClass;
        s32 dx;
        s32 dy;
        s32 dist;
        s32 t;
        s32 sn;
        s32 heightOffset;

        heightOffset = 0x2000;
        if (obj_point_in_iris(gGamePtr->unk1CB4, &obj->posX, &((struct E0E0Char *)gGamePtr->charA)->unk04) != 0) {
            obj->lightTimer = 0;
            obj_set_sprite_kind(obj, 7);
            obj->state = LIGHT_CHASER_IN_LIGHT;
            return 0;
        }
        if (hitboxes_overlap(obj, gGamePtr->charA) != 0) {
            heroClass = ((struct E0E0Char *)gGamePtr->charA)->unk30;
            heroClass->unk14((u8 *)gGamePtr->charA + heroClass->unk10, ctx, (s16)gravity, arg3, 5);
        }
        if ((s16)obj->sprite->unk04 > 0xFF) {
            /* full size: move half a pixel a frame towards hero A */
            obj->sprite->unk04 = 0x100;
            obj->sprite->unk06 = 0x100;
            dx = ((struct E0E0Char *)gGamePtr->charA)->unk04 - obj->posX;
            dy = ((struct E0E0Char *)gGamePtr->charA)->unk08 - (obj->posY + heightOffset);
            dist = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dx * dx + dy * dy);
            dy = (dy << 7) / dist;
            dx = (dx << 7) / dist;
            obj->velX = dx;
            obj->velY = dy;
            obj->posX += obj->velX;
            t = obj->posY + (s16)obj->velY;
            obj->posY = t;
            sn = gSineTable[obj->bobPhase & 0xFF];
            if (sn < 0)
                sn += 0x3F;
            obj->posY = t + ((sn >> 6) << 1);
            obj->bobPhase = obj->bobPhase + 2;
        } else {
            /* growing in */
            obj->sprite->unk04 += 4;
            obj->sprite->unk06 += 4;
        }
        if ((s16)obj->velX > 0) {
            ((struct Bits8 *)&obj->sprite->unk11)->b6 = 1;
        } else {
            ((struct Bits8 *)&obj->sprite->unk11)->b6 = 0;
        }
        return 0;
    }
    }
    return 0;
}
struct ModeBits {
    s8 mode : 5;
};

extern const u8 gClassE100[];
extern u8 gGameMode;

/* Class E100 object (made by obj_init_e100): a sprite with an overlay sprite that the scene
   state picks. */
struct E100Obj {
    struct SubObj *sprite;
    u8 filler04[0x2C];
    const void *klass;
    struct SubObj *overlay; /* 0 in game modes above 5 */
    u16 kind;
};

/* Sets up a class E100 object of the given kind. In game modes above 5 it only gets sprite kind
   0x3B. Otherwise it gets an overlay sprite, hidden, at the sprite's position with the sprite's
   2-bit fields at +0x1F; the overlay's record is 0x30-0x33 for scene states 1-4, 0xA for
   scene state 5 and 0x30 for any other. */
struct E100Obj *obj_init_e100(struct E100Obj *obj, u32 initArg, u16 kind)
{
    struct SubObj *sprite;

    obj_init_from_record(obj, initArg);
    obj->klass = gClassE100;
    obj->kind = kind;
    obj_create_sprite(obj, 0);
    obj->overlay = NULL;
    if (((struct ModeBits *)&gGameMode)->mode > 5) {
        obj_set_sprite_kind(obj, 0x3B);
        return obj;
    }
    obj->overlay = create_obj_de4(4, 0x5008, 0, -1, -1, -1, -1);
    /* gGamePtr->unk1F20: the scene state */
    if ((u8)(gGamePtr->unk1F20 - 1) <= 4) {
        if (gGamePtr->unk1F20 == 5)
            obj_load_record(obj->overlay, 0xA, -1, 0, 0);
        else
            obj_load_record(obj->overlay, gGamePtr->unk1F20 + 0x2F, -1, 0, 0);
    } else {
        obj_load_record(obj->overlay, 0x30, -1, 0, 0);
    }
    sprite_update_cd0(obj->overlay);
    ((struct FlagBit5 *)&obj->overlay->unk12)->bit5 = 1; /* hidden */

    /* the overlay sits on the sprite, with its 2-bit fields at +0x1F */
    sprite = obj_get_sprite(obj);
    obj->overlay->unk00 = sprite->unk00;
    sprite = obj_get_sprite(obj);
    obj->overlay->unk02 = sprite->unk02;
    obj->overlay->unkE = 0x13;

    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f0 = (u32)(sprite->unk1F << 30) >> 30;
    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f2 = (sprite->unk1F & 0xC) >> 2;
    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f4 = (sprite->unk1F & 0x30) >> 4;
    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f6 = sprite->unk1F >> 6;
    return obj;
}

struct SpriteMode11 {
    u8 low2 : 2;
    u8 mode : 2;
    u8 high : 4;
};

struct SpriteMode12 {
    u8 low1 : 1;
    u8 mode : 2;
    u8 high : 5;
};

/* obj_behavior_e0e0's states (E0E0Char.unk24) */
enum {
    E0E0_IDLE = 0,
    E0E0_WATCH_HEROES = 1, // tells each hero past it (OBJ_MSG_HERO_PASSED), counts them
    E0E0_WAIT_HEROES = 2,  // both are past: done once both heroes are in state 8
    E0E0_TRIGGERED = 3,    // shows its triggered sprite
};

#define OBJ_MSG_E0E0_TRIGGER 0xC // what obj_behavior_e0e0 gets to switch to E0E0_TRIGGERED
#define OBJ_MSG_HERO_PASSED 0xD  // what obj_behavior_e0e0 sends a hero that is past it
#define SE_E0E0_TRIGGER 0xAC

/* Behavior of a marker on the bros' path: started (OBJ_MSG_START) it sits on the ground under it and,
   each frame, sends OBJ_MSG_HERO_PASSED to every hero whose x is past its own; once both are past, it is
   done (returns OBJ_DONE) when both heroes are in state 8. OBJ_MSG_E0E0_TRIGGER plays SE_E0E0_TRIGGER
   and switches it to its triggered sprite (0x2F, or 0x3C without a sprite of its own). */
u32 obj_behavior_e0e0(struct E0E0Char *obj, u32 ctx, u32 unused2, u32 unused3, u8 msg)
{
    struct E0E0Char *hero;
    struct SubObj *sprite;

    switch (msg) {
    case OBJ_MSG_START:
        obj->unk30->unk34((u8 *)obj + obj->unk30->unk30);
        sprite = obj->unk34;
        if (sprite != 0)
            ((struct FlagBit5 *)&sprite->unk12)->bit5 = 0;
        obj->unk24 = E0E0_WATCH_HEROES;
        return OBJ_KEEP;
    case OBJ_MSG_FREE:
        obj_release_sprite(obj);
        return OBJ_KEEP;
    case OBJ_MSG_E0E0_TRIGGER:
        set_slot_value(SE_E0E0_TRIGGER, -1);
        obj->unk24 = E0E0_TRIGGERED;
        return OBJ_KEEP;
    }

    switch (obj->unk24) {
    case E0E0_IDLE:
        return OBJ_KEEP;
    case E0E0_WATCH_HEROES:
        /* unk38: how many heroes are past it this frame; each gets its class's message handler */
        obj->unk38 = 0;
        hero = (struct E0E0Char *)gGamePtr->charA;
        if (hero->unk04 > obj->unk04) {
            hero->unk30->unk14((u8 *)hero + hero->unk30->unk10, 0, 0, 0, OBJ_MSG_HERO_PASSED);
            obj->unk38++;
        }
        hero = (struct E0E0Char *)gGamePtr->charB;
        if (hero->unk04 > obj->unk04) {
            hero->unk30->unk14((u8 *)hero + hero->unk30->unk10, 0, 0, 0, OBJ_MSG_HERO_PASSED);
            obj->unk38++;
        }
        if (obj->unk38 > 1)
            obj->unk24 = E0E0_WAIT_HEROES;
        /* stand on the ground height under it (whole pixels, to 8.8 fixed point) */
        obj->unk08 = (s32)(view_coord(ctx, obj->unk04) << 16) >> 8;
        return OBJ_KEEP;
    case E0E0_WAIT_HEROES:
        if (((struct E0E0Char *)gGamePtr->charA)->unk24 != 8 || ((struct E0E0Char *)gGamePtr->charB)->unk24 != 8)
            return OBJ_KEEP;
        return OBJ_DONE;
    case E0E0_TRIGGERED:
        if (obj->unk34 != 0)
            obj_set_sprite_kind(obj, 0x2F);
        else
            obj_set_sprite_kind(obj, 0x3C);
        ((struct SpriteMode12 *)&obj_get_sprite(obj)->unk12)->mode = 1;
        return OBJ_KEEP;
    }
    return OBJ_KEEP;
}

/* Class E080 object (made by obj_init_e080): a sprite with a hidden overlay sprite at the
   screen centre. */
struct E080Obj {
    struct SubObj *sprite;
    u8 filler04[0x2C];
    const void *klass;
    struct SubObj *overlay;
    u16 kind;
};

/* Sets up a class E080 object of the given kind: its sprite (unk11 mode 1) and an overlay
   sprite (record 2 of 0x500A), hidden, at the screen centre (120, 80), with the sprite's
   2-bit fields at +0x1F copied one by one. */
struct E080Obj *obj_init_e080(struct E080Obj *obj, u32 initArg, u16 kind)
{
    struct SubObj *sprite;

    obj_init_from_record(obj, initArg);
    obj->klass = gTransitionClassE080;
    obj->kind = kind;
    obj_create_sprite(obj, 0);
    ((struct SpriteMode11 *)&obj->sprite->unk11)->mode = 1;
    obj->overlay = create_obj_de4(4, 0x500A, 0, -1, -1, -1, -1);
    obj_load_record(obj->overlay, 2, -1, 0, 0);
    sprite_update_cd0(obj->overlay);
    ((struct FlagBit5 *)&obj->overlay->unk12)->bit5 = 1; /* hidden */
    obj->overlay->unk00 = 0x78;
    obj->overlay->unk02 = 0x50;
    obj->overlay->unkE = 0x13;

    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f0 = (u32)(sprite->unk1F << 30) >> 30;
    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f2 = (sprite->unk1F & 0xC) >> 2;
    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f4 = (sprite->unk1F & 0x30) >> 4;
    sprite = obj_get_sprite(obj);
    ((struct SubFlagBits *)&obj->overlay->unk1F)->f6 = sprite->unk1F >> 6;
    return obj;
}

/* An enemy (class E060) that sways from side to side under the surface until hero A comes
   near, then leaps out with a splash and falls back. Positions are 8.8 pixels. */
struct Leaper {
    struct SubObj *sprite;  /* unk11 bit 6: mirror, unk1F: the parts' priorities */
    s32 posX;
    s32 posY;
    u8 filler0C[8];
    s16 velX;
    u8 filler16[2];
    u16 velY;
    u8 filler1A[2];
    u16 phase;              /* LEAPER_SWAY: gCosineTable index; LEAPER_LEAP: x acceleration */
    u8 filler1E[2];
    u16 fallAccel;          /* added to the frame's gravity */
    u8 filler22[2];
    u8 state;               /* LEAPER_* */
    u8 filler25[0xB];
    struct E0E0Table *klass;
    struct SubObj *splash;  /* shown once where it breaks the surface */
    u16 flags;              /* bit 0: the splash has been shown */
};

enum {
    LEAPER_IDLE = 0,
    LEAPER_SWAY = 1,
    LEAPER_LEAP = 2,
};

/* Upward speed (-0x320 in velY's 8.8 fixed point) a leaper jumps with. */
#define LEAPER_JUMP_SPEED 0xFCE0
/* How close (8.8 pixels) hero A comes from the left before it leaps. */
#define LEAPER_TRIGGER_DIST 0x2800

/* Shared per-line buffer (see hblank_iris_window); its first s16 is the surface's world y in pixels. */
extern u8 *gUnk_03001014;

extern s16 gCosineTable[];


/* Behavior of the leaper: it sways from side to side (mirrored to face its way) until hero A is
   within LEAPER_TRIGGER_DIST, then leaps up. Rising past the surface line it shows its splash
   once and hides it when the splash's animation ends; falling, its parts go to priority 2.
   While leaping it faces hero A. `gravity` is the frame's fall speed step. */
u32 obj_behavior_e060(struct Leaper *obj, u32 ctx, u16 gravity, u32 unused3, u8 msg)
{
    u16 phase;
    s32 dv;

    switch (msg) {
    case OBJ_MSG_START:
        obj->klass->unk34((u8 *)obj + obj->klass->unk30);
        obj->state = LEAPER_SWAY;
        break;
    case OBJ_MSG_FREE:
        obj->klass->unk3C((u8 *)obj + obj->klass->unk38);
        ((struct FlagBit5 *)&obj->splash->unk12)->bit5 = 1;
        break;
    default:
        switch (obj->state) {
        case LEAPER_IDLE:
            break;
        case LEAPER_SWAY:
            obj->posX += (s16)obj->velX;
            phase = obj->phase;
            obj->velX = gCosineTable[(u8)phase] / 64 * 2 / 3;
            obj->phase = (phase + 1) & 0xFF;
            if ((s16)obj->velX > 0)
                ((struct Bits8 *)&obj_get_sprite(obj)->unk11)->b6 = 1;
            else
                ((struct Bits8 *)&obj_get_sprite(obj)->unk11)->b6 = 0;
            if (((struct E0E0Char *)gGamePtr->charA)->unk04 - obj->posX > -LEAPER_TRIGGER_DIST) {
                obj_set_sprite_kind(obj, 0x35);
                obj->phase = 0;
                obj->fallAccel = 0;
                obj->velY = LEAPER_JUMP_SPEED;
                obj->velX = 0;
                obj->state = LEAPER_LEAP;
            }
            break;
        case LEAPER_LEAP:
            obj->velX += obj->phase;
            dv = (s16)gravity + obj->fallAccel;
            obj->velY = obj->velY + dv;
            obj->posX += (s16)obj->velX;
            obj->posY += (s16)obj->velY;
            if ((s16)obj->velY > 0) {
                ((struct SubFlagBits *)&obj_get_sprite(obj)->unk1F)->f0 = 2;
                ((struct SubFlagBits *)&obj_get_sprite(obj)->unk1F)->f2 = 2;
                ((struct SubFlagBits *)&obj_get_sprite(obj)->unk1F)->f4 = 2;
                ((struct SubFlagBits *)&obj_get_sprite(obj)->unk1F)->f6 = 2;
            } else if ((s32)(obj->posY >> 8) < *(s16 *)gUnk_03001014) {
                if ((((u8)obj->flags ^ 1) & 1) != 0) {
                    /* first time above the surface: show the splash */
                    ((struct FlagBit5 *)&obj->splash->unk12)->bit5 = 0;
                    ((struct SubFlagBits *)&obj->sprite->unk11)->f2 = 0;
                    obj->flags |= 1;
                } else if (obj->splash->unk12 & 8) {
                    obj->splash->unk12 |= 0x20;
                }
            }
            if (((struct E0E0Char *)gGamePtr->charA)->unk04 - obj->posX > 0)
                ((struct Bits8 *)&obj_get_sprite(obj)->unk11)->b6 = 1;
            else
                ((struct Bits8 *)&obj_get_sprite(obj)->unk11)->b6 = 0;
            break;
        }
        break;
    }
    return OBJ_KEEP;
}
// Upward speed (-0x1F4 in velY's 8.8 fixed point) a hopper jumps with.
#define HOPPER_JUMP_SPEED 0xFE0C

enum {
    HOPPER_IDLE = 0,
    HOPPER_HOPPING = 1,
    HOPPER_FREED = 2,
};

// An object that keeps hopping on its home spot (obj_behavior_dfe0).
struct DfeObj {
    u8 filler0[4];
    s32 x;              // 24.8 fixed point
    s32 y;
    u8 filler0C[8];
    u16 velX;           // read as s16
    u8 filler16[2];
    u16 velY;
    u8 filler1A[2];
    u16 accelX;
    u8 filler1E[2];
    u16 weight;         // added to the frame's gravity
    u8 filler22[2];
    u8 state;           // HOPPER_*
    u8 filler25[0xB];
    struct E0E0Table *klass;
    u8 filler34[4];
    s32 homeX;
    s32 homeY;          // the ground: a hop ends once y passes it
};

/* Behavior of an object that keeps hopping on its home spot. OBJ_MSG_START runs its class's start
   hook and sets it hopping; OBJ_MSG_FREE runs the class's free hook. Each frame (msg 0) while
   hopping, velX grows by accelX and velY by the frame's `gravity` plus weight; once y passes the
   ground (homeY) it is put back on its home spot and jumps again. Always returns OBJ_KEEP. */
u32 obj_behavior_dfe0(struct DfeObj *obj, u32 unused1, u16 gravity, u32 unused3, u8 msg)
{
    struct E0E0Table *klass;
    s32 dv;
    s32 y;
    switch (msg)
    {
    case OBJ_MSG_START:
        klass = obj->klass;
        klass->unk34((u8 *)obj + klass->unk30);
        obj->velY = HOPPER_JUMP_SPEED;
        obj->state = HOPPER_HOPPING;
        break;
    case OBJ_MSG_FREE:
        klass = obj->klass;
        klass->unk3C((u8 *)obj + klass->unk38);
        obj->state = HOPPER_FREED;
        break;
    case 5:
        break;
    default:
        if (obj->state == HOPPER_HOPPING)
        {
            obj->velX += obj->accelX;
            dv = (s16)gravity + obj->weight;
            obj->velY = obj->velY + dv;
            obj->x += (s16)obj->velX;
            y = obj->y + (s16)obj->velY;
            obj->y = y;
            if (y > obj->homeY)
            {
                s32 homeX = obj->homeX;
                s32 homeY = *(volatile s32 *)&obj->homeY;
                obj->x = homeX;
                obj->y = homeY;
                obj->velY = HOPPER_JUMP_SPEED;
            }
        }
        break;
    }
    return OBJ_KEEP;
}

/* An object that walks after char A and hops once it passes WALKER_JUMP_X. */
struct StateObj {
    void *sprite;
    s32 x; /* 24.8 fixed point */
    u8 filler08[0xC];
    s16 vx;
    u8 filler16[2];
    u16 vy;
    u8 filler1A[0xA];
    u8 state; /* enum WalkerState */
    u8 filler25[0x11];
    u16 timer; /* frames left in the current jump phase */
};

enum WalkerState {
    WALKER_STAND = 0,
    WALKER_WALK = 1,
    WALKER_JUMP_START = 2,
    WALKER_JUMP_AIR = 3,
    WALKER_JUMP_LAND = 4,
    WALKER_WALK_PAST = 10 /* walking on after the jump, won't jump again */
};

/* Sprite animations used by the walker. */
enum WalkerAnim {
    WALKER_ANIM_WALK = 0,
    WALKER_ANIM_STAND = 1,
    WALKER_ANIM_JUMP_START = 2,
    WALKER_ANIM_JUMP_AIR = 3,
    WALKER_ANIM_JUMP_LAND = 4
};

#define WALKER_JUMP_X 0x37F00 /* x (24.8) past which the walker jumps */

/*
 * Per-frame animation state machine of a walker that follows char A: it walks
 * while it moves, stands when it stops, and once past WALKER_JUMP_X it jumps
 * (start, air and land phases, each on a timer) and then walks on.
 */
void obj_state_load_step(struct StateObj *obj)
{
    s32 vx;
    s32 timer;

    switch (obj->state) {
    case WALKER_STAND:
        if (obj->vx == 0)
            break;
        obj_load_record(obj->sprite, WALKER_ANIM_WALK, 0, 0, 0);
        if (obj->x >= WALKER_JUMP_X)
            obj->state = WALKER_WALK_PAST;
        else
            obj->state = WALKER_WALK;
        break;
    case WALKER_WALK:
        if (obj->x >= WALKER_JUMP_X) {
            obj->vy = -gGamePtr->unk1F1C;
            obj_load_record(obj->sprite, WALKER_ANIM_JUMP_START, 0, 0, 0);
            obj->timer = 10;
            obj->state = WALKER_JUMP_START;
            break;
        }
        vx = obj->vx;
        goto check_stopped;
    case WALKER_WALK_PAST:
        vx = obj->vx;
check_stopped:
        if (vx != 0)
            break;
        obj_load_record(obj->sprite, WALKER_ANIM_STAND, 0, 0, 0);
        obj->state = vx; /* WALKER_STAND */
        break;
    case WALKER_JUMP_START:
        timer = obj->timer;
        timer -= 1;
        obj->timer = timer;
        if ((s16)timer > 0)
            break;
        obj_load_record(obj->sprite, WALKER_ANIM_JUMP_AIR, 0, 0, 0);
        obj->timer = 60;
        obj->state += 1;
        break;
    case WALKER_JUMP_AIR:
        timer = obj->timer;
        timer -= 1;
        obj->timer = timer;
        if ((s16)timer > 0)
            break;
        obj_load_record(obj->sprite, WALKER_ANIM_JUMP_LAND, 0, 0, 0);
        obj->timer = 10;
        obj->state += 1;
        break;
    case WALKER_JUMP_LAND:
        timer = obj->timer;
        timer -= 1;
        obj->timer = timer;
        if ((s16)timer > 0)
            break;
        obj_load_record(obj->sprite, WALKER_ANIM_WALK, 0, 0, 0);
        obj->state = WALKER_WALK_PAST;
        break;
    default:
        break;
    }
}

void obj_track_char_a(struct TrackObj *obj)
{
    struct Char *charA;
    u16 speed;
    s16 s;

    charA = gGamePtr->charA;
    switch (obj->unk38)
    {
    case 0:
        if ((s16)charA->unk14 > 0)
            obj->unk38 = 1;
        break;
    case 1:
        speed = charA->unk14;
        s = speed;
        if (s == 0)
        {
            if ((s32)(charA->unk04 - obj->unk04) <= 0x3BFF)
                obj->unk38 = 2;
        }
        else if ((s16)obj->unk14 >= s)
            obj->unk14 = speed;
        else
            obj->unk14 += 2;
        break;
    case 2:
        obj->unk14 = 0;
        obj->unk38 = 0;
        break;
    }
}

/* Sets up a three-digit counter at 0: clears its state and creates one sprite per digit, 8 pixels
 * apart from (0xD8, 0x10), then shows the value. */
struct DigitObj *obj_create_digits(struct DigitObj *obj)
{
    s32 i;
    struct Sprite **digit;

    obj->unk1C = 0;
    obj->unk08 = 0;
    obj->unk04 = 0;
    obj->unk00 = 0;
    obj->unk20 = 0;
    obj->unk24 = 0;
    obj->unk28 = 0;
    obj->unk2C = 0;
    obj->unk14 = 0xE800;
    obj->unk18 = 0x1400;
    obj->unk30 = 0;
    obj->unk34 = 0;

    for (i = 0; i < 3; i++) {
        digit = (struct Sprite **)obj + i; /* unk00, unk04, unk08 */
        *digit = create_obj_de4(4, 0x500C, 0, -1, -1, -1, -1);
        obj_load_record(*digit, 0xA, -1, 0, 0);
        sprite_update_cd0(*digit);
        (*digit)->unk00 = 0xD8 + i * 8;
        (*digit)->unk02 = 0x10;
    }

    obj_update_digits(obj);
    return obj;
}

void obj_shake_or_countdown(struct DigitObj *obj)
{
    if (obj->unk20 != 1) {
        if ((s32)obj->unk20 <= 1)
            return;
        if (obj->unk20 == 2)
            goto state2;
        return;
    }
    obj->unk24 -= 1;
    if ((s32)obj->unk24 <= 0) {
        obj->unk20 = 0;
    } else {
        obj->unk28 = 4 - obj->unk28;
        {
            s32 t = obj->unk00->unk00 - 2;
            obj->unk00->unk00 = obj->unk28 + t;
        }
        {
            s32 t = obj->unk04->unk00 - 2;
            obj->unk04->unk00 = obj->unk28 + t;
        }
        {
            s32 t = obj->unk08->unk00 - 2;
            obj->unk08->unk00 = obj->unk28 + t;
        }
    }
    return;
state2:
    obj->unk24 -= 1;
    if ((s32)obj->unk24 <= 0) {
        obj->unk34 = 0;
        obj->unk30 = 0;
        obj->unk20 = 0;
        obj_update_digits(obj);
    } else {
        obj->unk34 -= 1;
        if ((s32)obj->unk34 < 0) {
            obj->unk30 = 0xA - obj->unk30;
            obj->unk34 = 5;
            obj_update_digits(obj);
        }
        obj->unk2C += 1;
    }
}

/* A sprite that slides in from the left, holds, then slides out to the right (obj_anim_fall_step). */
struct FallObj {
    u16 unk00;            /* hold timer, frames */
    u8 filler02[2];
    struct Sprite *unk04; /* its sprite */
    u16 unk08;            /* 1: slide in, 2: hold, 3: slide out */
    u8 filler0A[2];
    u32 unk0C;            /* x (8.8) */
    u8 filler10[4];
    u32 unk14;            /* x speed (8.8) */
    u8 filler18[4];
    u32 unk1C;            /* x acceleration (8.8) */
};


/* Steps a sliding sprite: state 1 slides in, slowing down until it turns back; state 2 holds for
 * 60 frames, then plays song 0x4B and returns 3; state 3 speeds up to the right and, once past
 * x = 240 px, releases the sprite and returns 2. Returns 0 while nothing happens. */
u32 obj_anim_fall_step(struct FallObj *obj)
{
    s32 state = (s16)obj->unk08;

    if (state != 1) {
        if (state <= 1)
            return 0;
        if (state == 2)
            goto hold;
        if (state == 3)
            goto slide_out;
        return 0;
    }
    /* slide in */
    obj->unk14 -= obj->unk1C;
    obj->unk0C += obj->unk14;
    obj->unk04->unk00 = (s32)obj->unk0C >> 8;
    if ((s32)obj->unk14 >= 0)
        return 0;
    obj->unk00 = 0;
    obj->unk08 += 1;
hold:
    obj->unk00 += 1;
    if ((s16)obj->unk00 > 59) {
        set_slot_value(0x4B, -1);
        obj->unk00 = 0;
        obj->unk08 += 1;
        return 3;
    }
    return 0;
slide_out:
    obj->unk14 += obj->unk1C;
    obj->unk0C += obj->unk14;
    obj->unk04->unk00 = (s32)obj->unk0C >> 8;
    if ((s32)obj->unk0C < (240 << 8))
        return 0;
    release_obj_131c(obj->unk04);
    obj->unk04 = 0;
    return 2;
}

/* The same object as FallObj, seen from obj_create_sprite_record. */
struct RecObj {
    u8 filler0[4];
    struct Sprite *unk04; /* its sprite */
    u16 unk08;            /* state of obj_anim_fall_step, 1: moving */
    u8 filler0A[2];
    u32 unk0C;            /* x (8.8) */
    u32 unk10;            /* y (8.8) */
};

extern u8 gSys[];
#define SYS_LANGUAGE 0x8C0 /* gSys[SYS_LANGUAGE]: the game's language, 0-4 */

/* Creates obj's sprite with the record for the game's language (0x1C-0x20 for languages 0-4,
   0x1B otherwise), puts it at obj's position in whole pixels and starts obj_anim_fall_step
   (state 1). */
void obj_create_sprite_record(struct RecObj *obj)
{
    obj->unk04 = create_obj_de4(4, 0x5005, 1, -1, -1, -1, -1);
    switch (gSys[SYS_LANGUAGE]) {
    case 0:
        obj_load_record(obj->unk04, 0x1C, -1, 0, 0);
        break;
    case 1:
        obj_load_record(obj->unk04, 0x1D, -1, 0, 0);
        break;
    case 2:
        obj_load_record(obj->unk04, 0x1E, -1, 0, 0);
        break;
    case 3:
        obj_load_record(obj->unk04, 0x1F, -1, 0, 0);
        break;
    case 4:
        obj_load_record(obj->unk04, 0x20, -1, 0, 0);
        break;
    default:
        obj_load_record(obj->unk04, 0x1B, -1, 0, 0);
        break;
    }
    sprite_update_cd0(obj->unk04);
    obj->unk04->unk00 = (s32)obj->unk0C >> 8;
    obj->unk04->unk02 = (s32)obj->unk10 >> 8;
    obj->unk08 = 1;
}

ASM_FUNC("asm/code_08154490/transition_setup_scene.s", void transition_setup_scene(void))