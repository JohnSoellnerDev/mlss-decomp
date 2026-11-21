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

/* An object with one sprite (obj_init_big, 0x6C bytes), as the time bar uses it. */
struct BigObj
{
    u8 filler0[0x10];
    struct SubObj *sprite;
    u8 filler14[4];
    u32 x;           /* 1/0x100 px */
    u32 y;
    u8 filler20[0x56 - 0x20];
    u16 animMode;     /* the animation it should play */
    u16 prevAnimMode; /* last frame's animMode (obj_copy_mode_fields) */
};

/* The time bar (obj_init_counter): an icon that crosses the screen towards a label as time runs. */
struct TimeBar
{
    s32 limit;       /* frames the bar takes to fill: 0x1C20 (2 minutes) */
    s32 elapsed;     /* frames counted so far */
    s32 boostFrames; /* while nonzero, time runs 4 times as fast */
    struct BigObj *icon;
    struct BigObj *label;
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

struct PosXY
{
    s32 x;
    u8 filler4[4];
    s32 y;
};

/* A shot (obj_init_projectile, stepped by obj_line_bounce_step). */
struct ProjObj
{
    u8 filler0[0xC];
    void *unkC;             // class table (gClassEbb8)
    struct SubObj *unk10;   // sprite
    u8 filler14[4];
    struct PosXY pos18;     // position (x, height, y), moved by unk70-unk78 every frame
    u8 filler24[0x6C - 0x24];
    u32 unk6C;              // state: 0 enemy shot, 1 player shot, 2-3 reflected, 4-5 fading, 6-7 hit
    s32 unk70;              // velocity x
    s32 unk74;              // velocity height (always 0)
    s32 unk78;              // velocity y
    u32 unk7C;              // direction angle (0-255)
    s32 unk80;              // frames left before it fades, then the fade/hit timer
    s32 unk84;
    u16 unk88;
    u16 unk8A;
    u16 unk8C;
    u16 unk8E;
    u16 unk90;
    u8 filler92[6];
    u16 unk98;
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
    s32 unk7C;
};

struct Flags6C
{
    u16 b0 : 1;
    u16 b1 : 1;
    u16 b2 : 1;
    u16 b3 : 1;
    u16 rest : 12;
} __attribute__((packed));

/* A player-steered pair of parts (grid_lines_obj_step_motion): the d-pad moves it, A/B turn it,
   R/L fire from the first/second part. */
struct GridLinesObj
{
    struct GridPart *unk0;  // first part, fires with R
    struct GridPart *unk4;  // second part, fires with L
    u8 filler8[0xC];
    struct PosXY pos14;
    struct PosXY vel20;     // this frame's d-pad step
    s32 unk2C;              // angle (0-255)
    s32 unk30;              // sine of the angle, also copied to both parts
    s32 unk34;              // cosine of the angle, also copied to both parts
    u8 filler38[0x50 - 0x38];
    s32 unk50;              // nonzero: input is ignored
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

/* The screen task run by child_states_obj_run_state (built by obj_init_screen, class gClassEb68,
   destroyed by obj_destroy_all). */
struct ChildStatesObj
{
    u8 filler0[0x14];
    void *unk14;                    // passed to set_flags_2_3 when the screen is destroyed
    struct VtB *vtbl;               // slot fn(this, 3) destroys it
    struct ObjA *list;              // third argument of grid_lines_obj_step_motion (obj_init_empty)
    struct GridLinesObj *controls;  // the player-steered pair
    void *region;                   // second argument of grid_lines_obj_step_motion
    void *group5005;                // create_obj_group_5005
    struct ObjB *childTask;         // task of class gClassEb80 that points back at the screen
    void *tween;                    // copied to its shadow every frame
    u8 filler34[0x38 - 0x34];
    void *window;                   // window_obj_setup
    struct HolderObj *counter;      // the TimeBar from obj_init_counter
    u8 filler40[0x6C - 0x40];
    void *group500B;                // create_obj_group_500b
    s32 timer;                      // timer_tick: fade-in (state 1) and fade-out (state 6)
    u8 filler74[0x474 - 0x74];
    u32 state;                      // 0-7, see child_states_obj_run_state
    u8 filler478[0x484 - 0x478];
    u16 result;                     // written to var 0x1CE1 when the screen ends
};

extern u16 gUnk_02000050[];
void palette_fill(u32 v);
void slot_reset(u32 id);
void set_slot_target(u32 a, u32 b, u32 c);
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
#define SYS_LANGUAGE 0x8C0 /* gSys[SYS_LANGUAGE]: the game's language, 0-4 */
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *obj_init_big(void *obj, u32 first);
void obj_create_sprite_10(void *obj, u32 a, u32 kind, u32 b, u32 c, u32 d);
void obj_counter_step(struct TimeBar *bar);
void obj_copy_mode_fields(void *obj);
void update_obj_sprite_pos(void *obj, u32 camX, u32 camY);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
s32 lerp_step_08164EE0(s32 a, s32 b, s32 steps, s32 t);

struct NodeAttr
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 b3 : 1;
    u8 bit4 : 1;
    u8 hi3 : 3;
};


// The sprite of an orbiter (obj_node_update).
struct OrbiterSprite
{
    u8 filler0[4];
    u16 scaleX;
    u16 scaleY;
    u8 filler8[4];
    u16 rotation;
    u8 fillerE[4];
    u8 attr; // struct NodeAttr; 0x10: the animation has finished
};

// The object the orbiters circle (obj_node_update's owner).
struct OrbiterHub
{
    u8 filler0[0x14];
    s32 x;
    u8 filler18[4];
    s32 y;
    u8 filler20[0x50 - 0x20];
    u32 mode; // enum OrbiterHubMode
    s32 hitCountA;
    s32 hitCountB;
    s32 turnSpeed; // reversed each time an orbiter is hit
};

enum OrbiterHubMode
{
    ORBITER_HUB_ENDING = 2, // orbiters fade out or vanish
    ORBITER_HUB_FROZEN = 3, // orbiters stop moving
};

// An enemy circling its hub on an ellipse (built by obj_init_bullet).
struct Orbiter
{
    u32 phase;
    u8 filler4[8];
    struct VtA *vtable;
    struct OrbiterSprite *sprite;
    u8 filler14[4];
    struct PosXY pos;
    u8 filler24[0x6C - 0x24];
    s32 centerX;
    u8 filler70[4];
    s32 centerY;
    s32 baseAngle;
    u32 angle; // baseAngle plus the hub's turn
    s32 radiusX;
    s32 radiusY;
    u16 idleTime; // frames spent orbiting
    s16 hitTimer;
    s16 state; // enum OrbiterState
    s16 stateTimer;
    u16 unk90; // cleared by obj_init_bullet
    u16 shotMode;
    void *shotTarget;
    u16 hits; // after the first hit, the damaged animations are used
};

enum OrbiterState
{
    ORBITER_APPEAR = 0,    // grows in while it moves onto its orbit
    ORBITER_ORBIT = 1,
    ORBITER_HIT = 2,       // set from outside when it gets hit
    ORBITER_KNOCKED = 3,   // flies outward, leaving a trail
    ORBITER_CHARGE = 4,    // set by obj_init_fields_8c_load: fires after 30 frames
    ORBITER_FIRED = 5,     // waits for the firing animation to end
    ORBITER_FADE_OUT = 6,  // the hub ends: shrinks away
    ORBITER_GONE = 7,      // hidden for 120 frames
    ORBITER_REAPPEAR = 8,
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

/* Tests a flag bit of a part's flag halfword. */
static inline u32 part_has_flag(struct GridPart *p, u32 mask)
{
    return p->flags6C & mask;
}

/* One frame of the player-steered part pair: unless input is locked (unk50), the d-pad gives this
   frame's step (gKeyStepTable), A/B turn the pair (by 2, or by 4 on their own while a part's
   counter runs), and a new R or L press fires a projectile from the first or second part when it
   is active. Then the angle's sine/cosine go to both parts, the pair is moved and clamped to the
   region, the parts' scripts run and obj_place_pair lays them out (moved: anything changed). */
void grid_lines_obj_step_motion(struct GridLinesObj *obj, void *region, void *list)
{
    u16 *keys = gUnk_03000374; /* [0] held, [1] newly pressed */
    u32 moved;
    s32 stepY;
    u32 turn;
    s32 stepX;
    u32 dpad;
    struct ProjObj *proj;
    struct GridPart *partA;
    struct GridPart *partB;
    s32 sinValue;
    s32 cosValue;

    moved = 0;
    turn = 0;
    stepY = 0;
    stepX = 0;
    if (obj->unk50 == 0)
    {
        if ((keys[0] & (DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN)) != 0)
        {
            dpad = (keys[0] >> 4) & 0xF;
            stepX = gKeyStepTable[dpad << 1];
            stepY = (gKeyStepTable + 1)[dpad << 1];
        }
        if (obj->unk0->ctr6E + obj->unk4->ctr6E > 0)
        {
            if (obj->unk0->ctr6E != 0)
            {
                turn = 4;
            }
            if (obj->unk4->ctr6E != 0)
            {
                turn -= 4;
            }
        }
        else
        {
            if ((keys[0] & B_BUTTON) != 0)
            {
                turn = 2;
            }
            if ((keys[0] & A_BUTTON) != 0)
            {
                turn -= 2;
            }
        }
        if (stepX != 0 || stepY != 0 || turn != 0)
        {
            moved = 1;
        }
        if ((keys[1] & R_BUTTON) && part_has_flag(obj->unk0, 1))
        {
            proj = obj_init_projectile(heap_alloc2(0x84, 0, gHeapTagProjectile, 0), &obj->unk0->pos18, &obj->pos14, 1);
            list_insert(list, proj);
            obj_set_speed_from(proj, obj->unk0);
        }
        else
        {
            if ((keys[1] & L_BUTTON) && part_has_flag(obj->unk4, 1))
            {
                proj = obj_init_projectile(heap_alloc2(0x84, 0, gHeapTagProjectile, 0), &obj->unk4->pos18, &obj->pos14, 1);
                list_insert(list, proj);
                obj_set_speed_from(proj, obj->unk4);
            }
        }
    }
    obj->unk2C = (obj->unk2C + turn) & 0xFF;
    partA = obj->unk0;
    partB = obj->unk4;
    sinValue = gSineTable[obj->unk2C & 0xFF] / 0x40;
    obj->unk30 = sinValue;
    partB->unk74 = sinValue;
    partA->unk74 = sinValue;
    cosValue = gCosineTable[obj->unk2C & 0xFF] / 0x40;
    obj->unk34 = cosValue;
    partB->unk78 = cosValue;
    partA->unk78 = cosValue;
    obj->vel20.x = stepX;
    obj->vel20.y = stepY;
    obj_clamp_pos(region, &obj->pos14, &obj->vel20);
    obj_copy_mode_fields(obj->unk0);
    obj_script_step(obj->unk0);
    obj_copy_mode_fields(obj->unk4);
    obj_script_step(obj->unk4);
    obj_place_pair(obj, moved);
}

/* Sets up a shot (class Ebb8) at src aimed at dst, with 120 frames to live. A player shot
 * (fromPlayer != 0) uses sprite 7, flies at 9/8 of a unit per frame and starts one step ahead
 * of src; an enemy shot uses sprite 9 and flies at 13/16 of a unit. Returns obj. */
struct ProjObj *obj_init_projectile(struct ProjObj *obj, struct PosXY *src, struct PosXY *dst, u32 fromPlayer)
{
    s32 dx;
    s32 dy;
    u32 angle;
    s32 sinValue;
    s32 cosValue;

    obj_init_big(obj, 5);
    obj->unkC = (void *)gClassEbb8;
    dx = (s32)((u32)(dst->x - src->x) << 8) >> 16;
    dy = (s32)((u32)(dst->y - src->y) << 8) >> 16;
    angle = ((u32)arc_tan2(dx, dy) << 16) >> 24;
    obj->unk7C = angle;
    sinValue = gSineTable[angle] / 0x40;
    cosValue = gCosineTable[angle] / 0x40;
    obj->unk74 = 0;
    obj->unk80 = 120;
    if (fromPlayer != 0)
    {
        obj_create_sprite_10(obj, 4, 0x5006, 0, 7, 2);
        obj->unk6C = 1;
        obj->unk70 = (cosValue * 9) >> 3;
        obj->unk78 = (sinValue * 9) >> 3;
        obj_set_fields_18(obj, src->x + obj->unk70, 0, src->y + obj->unk78);
    }
    else
    {
        obj_create_sprite_10(obj, 4, 0x5006, 0, 9, 2);
        obj->unk6C = fromPlayer;
        obj->unk70 = (cosValue * 0xD0) >> 8;
        obj->unk78 = (sinValue * 0xD0) >> 8;
    }
    return obj;
}

ASM_FUNC("asm/code_08169710/obj_line_bounce_step.s", void obj_line_bounce_step(void))

ASM_FUNC("asm/code_08169710/obj_check_contact.s", u32 obj_check_contact(struct ProjObj *obj, struct GridPart *target))
// Sets up an orbiter (class Eba8) circling (centerX, centerY) on an ellipse with radii radiusX,
// radiusY at the given angle: it starts at twice the radius and moves onto its orbit over
// moveTime + 60 frames, at double size, in the ORBITER_APPEAR state.
struct Orbiter *obj_init_bullet(struct Orbiter *obj, u32 centerX, u32 centerY, u32 angle, s32 radiusX, s32 radiusY, s32 moveTime)
{
    u8 one = 1;
    s32 orbitX;
    s32 orbitY;
    s32 startX;
    s32 startY;
    struct OrbiterSprite *sprite;

    obj_init_big(obj, 3);
    obj->vtable = (struct VtA *)gClassEba8;
    obj_create_sprite_10(obj, 4, 0x5006, 0, 5, 2);
    ((struct NodeAttr *)&obj->sprite->attr)->bit4 = 1;
    sprite = obj->sprite;
    sprite->scaleY = 0x200;
    sprite->scaleX = 0x200;
    obj->stateTimer = 0;
    obj->hitTimer = 0;
    obj->angle = angle;
    obj->baseAngle = angle;
    obj->radiusX = radiusX;
    obj->radiusY = radiusY;
    obj->unk90 = 0;
    obj->hits = 0;
    obj->idleTime = rand_scaled(0, 0x80);
    triple_set(&obj->centerX, centerX, 0, centerY);
    startX = centerX + (((gSineTable[angle & 0xFF] / 0x40) * radiusX) << 1);
    startY = (((gCosineTable[angle & 0xFF] / 0x40) * radiusY) << 1) + centerY;
    obj_set_fields_18(obj, startX, 0, startY);
    orbitX = centerX + (gSineTable[angle & 0xFF] / 0x40) * radiusX;
    orbitY = (gCosineTable[angle & 0xFF] / 0x40) * radiusY + centerY;
    obj_setup_slot(obj, orbitX, 0, orbitY, moveTime + 60, one);
    obj_set_table_44(obj, gObjScript08217b64);
    obj->state = ORBITER_APPEAR;
    return obj;
}

// Per-frame update of an orbiter: runs its state machine (appear, orbit, charge and
// fire at shotTarget, get hit and knocked out, reappear, fade out when the hub ends)
// and turns its sprite to face the hub. Returns 1 while it is orbiting, 0 otherwise
// (also when it destroyed itself).
u32 obj_node_update(struct Orbiter *obj, struct OrbiterHub *hub, u32 turn, void *list)
{
    struct ProjObj *proj;
    struct OrbiterSprite *sprite;
    void *trail;
    u32 hubMode;
    s32 scale;
    u8 zeroFlag = 0;
    u8 oneFlag = 1;

    obj->angle = (turn + obj->baseAngle) & 0xFF;
    obj_copy_mode_fields(obj);
    obj_script_step(obj);
    switch (obj->state)
    {
    case ORBITER_APPEAR:
        if (obj_is_done(obj))
        {
            set_slot_value(0x120, -1);
            obj->phase = 3;
            obj->state = ORBITER_ORBIT;
            sprite = obj->sprite;
            sprite->scaleY = 0x100;
            sprite->scaleX = 0x100;
        }
        else
        {
            scale = smoothstep_lerp(0x200, 0x100, 60, obj->stateTimer);
            sprite = obj->sprite;
            sprite->scaleY = scale;
            sprite->scaleX = scale;
            obj->stateTimer++;
        }
        break;
    case ORBITER_ORBIT:
        hubMode = hub->mode;
        if (hubMode == ORBITER_HUB_ENDING)
        {
            obj->state = ORBITER_FADE_OUT;
            obj->phase = 4;
        }
        else if (hubMode != ORBITER_HUB_FROZEN)
        {
            obj->idleTime++;
            child_states_obj_orbit_step(obj);
        }
        break;
    case ORBITER_CHARGE:
        hubMode = hub->mode;
        if (hubMode == ORBITER_HUB_ENDING)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
                obj->vtable->fn((u8 *)obj + obj->vtable->thisOff, 3);
            return 0;
        }
        if (hubMode != ORBITER_HUB_FROZEN)
            child_states_obj_orbit_step(obj);
        if (++obj->stateTimer > 29)
        {
            proj = obj_init_projectile(heap_alloc2(0x84, 0, gHeapTagProjectile, 0), &obj->pos, obj->shotTarget, obj->shotMode);
            list_insert(list, proj);
            if (obj->hits > 1)
            {
                obj_set_fields_18(proj, obj->pos.x + (proj->unk70 << 3), 0, obj->pos.y + (proj->unk78 << 3));
                obj_load_record(obj->sprite, 0x11, 0, 0, 0);
            }
            else
            {
                obj_set_fields_18(proj, obj->pos.x + (proj->unk70 << 4), 0, obj->pos.y + (proj->unk78 << 4));
                obj_load_record(obj->sprite, 5, 0, 0, 0);
            }
            ((struct NodeAttr *)&obj->sprite->attr)->mode = 1;
            obj->state = ORBITER_FIRED;
        }
        break;
    case ORBITER_FIRED:
        hubMode = hub->mode;
        if (hubMode == ORBITER_HUB_ENDING)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
                obj->vtable->fn((u8 *)obj + obj->vtable->thisOff, 3);
            return 0;
        }
        if (hubMode != ORBITER_HUB_FROZEN)
            child_states_obj_orbit_step(obj);
        if (obj->sprite->attr & 0x10)
        {
            obj->idleTime = rand_scaled(0, 0x80);
            if (obj->hits > 1)
                obj_load_record(obj->sprite, 0x11, 0, 0, 0);
            else
                obj_load_record(obj->sprite, 5, 0, 0, 0);
            ((struct NodeAttr *)&obj->sprite->attr)->bit4 = 1;
            obj->state = ORBITER_ORBIT;
        }
        break;
    case ORBITER_HIT:
        set_slot_value(0x40, -1);
        hub->turnSpeed = -hub->turnSpeed;
        hub->hitCountA++;
        hub->hitCountB++;
        if (obj->hits > 1)
            obj_load_record(obj->sprite, 0x12, 0, 0, 0);
        else
            obj_load_record(obj->sprite, 6, 0, 0, 0);
        // Fly out to twice the orbit's radius over 60 frames.
        obj_setup_slot(obj, obj->centerX + (((gSineTable[obj->angle & 0xFF] / 0x40) * obj->radiusX) << 1), 0,
                       obj->centerY + (((gCosineTable[obj->angle & 0xFF] / 0x40) * obj->radiusY) << 1), 60, zeroFlag);
        obj->state = ORBITER_KNOCKED;
        obj->hits++;
        /* fall through */
    case ORBITER_KNOCKED:
        if (++obj->hitTimer > 59)
        {
            obj->stateTimer = 0;
            obj_release_sprites(obj);
            obj->state = ORBITER_GONE;
        }
        else if ((obj->hitTimer & 7) == 1)
        {
            trail = heap_alloc2(0x6C, 1, gHeapTagRingShotSub, 0);
            list_insert(list, obj_init_sprite_offset(trail, &obj->pos, (gSineTable[obj->angle & 0xFF] / 0x40) << 3,
                                                     (gCosineTable[obj->angle & 0xFF] / 0x40) << 3));
        }
        break;
    case ORBITER_FADE_OUT:
        if (++obj->hitTimer > 59)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
                obj->vtable->fn((u8 *)obj + obj->vtable->thisOff, 3);
            return 0;
        }
        scale = smoothstep_lerp(0x100, 0x10, 60, obj->hitTimer);
        sprite = obj->sprite;
        sprite->scaleY = scale;
        sprite->scaleX = scale;
        break;
    case ORBITER_GONE:
        if (hub->mode == ORBITER_HUB_ENDING)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
                obj->vtable->fn((u8 *)obj + obj->vtable->thisOff, 3);
            return 0;
        }
        if (++obj->stateTimer > 119)
        {
            obj->hitTimer = 0;
            obj->stateTimer = 0;
            obj->state = ORBITER_REAPPEAR;
        }
        break;
    case ORBITER_REAPPEAR:
        if (hub->mode == ORBITER_HUB_ENDING)
        {
            obj_list_unlink(list, obj);
            if (obj != 0)
                obj->vtable->fn((u8 *)obj + obj->vtable->thisOff, 3);
            return 0;
        }
        obj->idleTime = rand_scaled(0, 0x80);
        obj_update_sprites(obj);
        if (obj->hits > 1)
            obj_load_record(obj->sprite, 0x11, 0, 0, 0);
        else
            obj_load_record(obj->sprite, 5, 0, 0, 0);
        ((struct NodeAttr *)&obj->sprite->attr)->bit4 = 1;
        // Start at twice the orbit's radius and move back onto it over 60 frames.
        obj_set_fields_18(obj, obj->centerX + (((gSineTable[obj->angle & 0xFF] / 0x40) * obj->radiusX) << 1), 0,
                          obj->centerY + (((gCosineTable[obj->angle & 0xFF] / 0x40) * obj->radiusY) << 1));
        obj_setup_slot(obj, obj->centerX + (gSineTable[obj->angle & 0xFF] / 0x40) * obj->radiusX, 0,
                       obj->centerY + (gCosineTable[obj->angle & 0xFF] / 0x40) * obj->radiusY, 60, oneFlag);
        obj->state = ORBITER_APPEAR;
        break;
    }
    obj->sprite->rotation = arc_tan2((s32)((hub->y - obj->pos.y) << 8) >> 16,
                                     (s32)((hub->x - obj->pos.x) << 8) >> 16);
    if (obj->state == ORBITER_ORBIT)
        return 1;
    return 0;
}

/* Creates the time bar: its icon (animation 0xE) at y 0xA0 and the current language's label at
   x 0xE0, y 0xA0 (positions in 8.8 fixed point), with 0x1C20 frames to fill, and places them. */
struct TimeBar *obj_init_counter(struct TimeBar *bar)
{
    bar->icon = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagProjectile, 0), 0);
    obj_create_sprite_10(bar->icon, 4, 0x5006, 0, 0xE, 2);
    bar->icon->y = 0xA0 << 8;
    bar->label = obj_init_big(heap_alloc2(0x6C, 0, gHeapTagCounterB, 0), 0);
    obj_create_sprite_10(bar->label, 4, 0x5006, 0, gCounterSpriteRecordByLang[gSys[SYS_LANGUAGE]], 2);
    bar->label->x = 0xE0 << 8;
    bar->label->y = 0xA0 << 8;
    bar->boostFrames = 0;
    bar->elapsed = 0;
    bar->limit = 0x1C20; /* 2 minutes at 60 frames a second */
    obj_counter_step(bar);
    return bar;
}

/* One frame of the time bar (see obj_init_counter): until it is full, time runs on by 4 frames
   while boostFrames last (the icon on its fast animation 0x14), else by 1 (animation 0xE), and the
   icon moves from x 0x10 to 0xE0 with it. Then the icon and the label are placed. */
void obj_counter_step(struct TimeBar *bar)
{
    s32 boost;
    s32 elapsed;
    u16 *animMode;
    u16 mode;

    obj_copy_mode_fields(bar->icon);
    elapsed = bar->elapsed;
    if (elapsed < bar->limit)
    {
        boost = bar->boostFrames;
        if (boost != 0)
        {
            bar->boostFrames = boost - 1;
            bar->elapsed = elapsed + 4;
            animMode = &bar->icon->animMode;
            boost = 1;
        }
        else
        {
            bar->elapsed = elapsed + 1;
            animMode = &bar->icon->animMode;
        }
        // 1 while boosted, else 0
        *animMode = boost;
        bar->icon->x = lerp_step_08164EE0(0x1000, 0xE000, bar->limit, bar->elapsed);
    }
    else
    {
        bar->icon->animMode = 0;
    }
    if (bar->icon->animMode != bar->icon->prevAnimMode)
    {
        mode = bar->icon->animMode;
        switch (mode)
        {
        case 0:
            obj_load_record(bar->icon->sprite, 0xE, 0, 0, mode);
            break;
        case 1:
            obj_load_record(bar->icon->sprite, 0x14, 0, 0, 0);
            break;
        }
    }
    update_obj_sprite_pos(bar->icon, 0, 0);
    bar->icon->sprite->unkE = 0;
    update_obj_sprite_pos(bar->label, 0, 0);
    bar->label->sprite->unkE = 4;
}

ASM_FUNC("asm/code_08169710/obj_clamp_pos.s", void obj_clamp_pos(void *region, void *a, void *b))

ASM_FUNC("asm/code_08169710/obj_init_screen.s", void obj_init_screen(void))

/* Destructor of the screen task (obj_init_screen): stops the HBlank scroll DMA, destroys or frees
 * everything the screen made (tween, controls, groups, counter, region, child task, window, list),
 * then its timer, and removes the task (freeing it when freeMem says so). */
void obj_destroy_all(struct ChildStatesObj *screen, u32 freeMem)
{
    struct HolderObj *counter;
    struct ObjA *list;
    struct ObjB *childTask;

    screen->vtbl = (void *)gClassEb68;
    set_irq_callback(2, 0);
    REG_DMA0CNT_H = 0;
    if (screen->tween != 0)
    {
        free_if_bit0(screen->tween, 3);
    }
    if (screen->controls != 0)
    {
        heap_free_b(screen->controls);
    }
    if (screen->group500B != 0)
    {
        obj_run_children_free(screen->group500B, 3);
    }
    counter = screen->counter;
    if (counter != 0)
    {
        if (counter->unk10 != 0)
        {
            counter->unk10->vt->fn((u8 *)counter->unk10 + counter->unk10->vt->thisOff, 3);
        }
        if (counter->unkC != 0)
        {
            counter->unkC->vt->fn((u8 *)counter->unkC + counter->unkC->vt->thisOff, 3);
        }
        heap_free_b(counter);
    }
    if (screen->region != 0)
    {
        heap_free_b(screen->region);
    }
    if (screen->group5005 != 0)
    {
        obj_run_children3_free(screen->group5005, 3);
    }
    childTask = screen->childTask;
    if (childTask != 0)
    {
        childTask->vt->fn((u8 *)childTask + childTask->vt->thisOff, 3);
    }
    if (screen->window != 0)
    {
        window_destroy(screen->window, 3);
    }
    list = screen->list;
    if (list != 0)
    {
        list->vt->fn((u8 *)list + list->vt->thisOff, 3);
    }
    obj_run_callbacks();
    set_flags_2_3(screen->unk14);
    free_if_bit0_b(&screen->timer, 2);
    task_remove(screen, freeMem);
}

/* Runs the screen's state machine for a frame: 0 reloads every palette and starts fading in,
 * 1 lets the player steer until the fade-in timer runs out, 2-5 are the screen's stages, 6 waits
 * for the fade-out timer, and 7 ends the screen: clears palettes, the fade and the blend shadow
 * registers, stores the result in var 0x1CE1 and destroys the task. */
void child_states_obj_run_state(struct ChildStatesObj *screen)
{
    rand_scaled(0, 2);
    switch (screen->state)
    {
    case 0:
        {
            u8 *sys = gSys;
            *(s32 *)(sys + 0x884) = -1; /* reload every palette */
        }
        set_slot_target(0, 3, -1);
        screen->state = 1;
    case 1:
        if (timer_tick(&screen->timer))
        {
            screen->state = 2;
        }
        else
        {
            grid_lines_obj_step_motion(screen->controls, screen->region, screen->list);
        }
        break;
    case 2:
        child_states_obj_run_stage(screen);
        break;
    case 3:
        obj_run_phases(screen);
        break;
    case 4:
        obj_run_substates(screen);
        break;
    case 5:
        child_states_obj_state4_if_flag8(screen);
        break;
    case 6:
        if (timer_tick(&screen->timer))
        {
            screen->state = 7;
        }
        break;
    case 7:
        palette_fill(0);
        slot_reset(0);
        write_var(0x1CE1, screen->result);
        {
            u16 *blend = gUnk_02000050; /* shadow of REG_BLDCNT */
            *blend = 0;
            blend += 2;                 /* shadow of REG_BLDY */
            *blend = 0;
        }
        if (screen != 0)
        {
            screen->vtbl->fn((u8 *)screen + screen->vtbl->thisOff, 3);
        }
        return;
    }
    obj_copy_pos_to_shadow(screen->tween);
    obj_update_children(screen);
}
