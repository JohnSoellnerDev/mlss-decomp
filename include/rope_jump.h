#ifndef GUARD_ROPE_JUMP_H
#define GUARD_ROPE_JUMP_H

// The jump-rope minigame (code_08166078, code_08167E7C).

struct GridSprite
{
    u8 filler0[0xC];
    u16 word0C;
    u8 filler0E[0x12 - 0xE];
    u8 attr12;
    u8 filler13;
    u16 unk14;
    u8 filler16[0x1F - 0x16];
    u8 prio1F; /* low 2 bits: drawing priority (3 = behind the rope, 2 = in front) */
};

struct GridObj
{
    u8 filler0[8];
    void *unk8;
    void *cls;
    void *unk10;
    void *unk14;
    void *unk18;
    u32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 filler30[0x3C - 0x30];
    void *anchor3C;
    void *points40;
    void *grid44;
    u8 filler48[0x52 - 0x48];
    u16 dir52;  /* facing direction (from gDirTab), picks the sprite record with mode56 */
    u16 unk54;  /* dir52 the sprite record was last loaded for */
    u16 mode56; /* animation mode */
    u16 mode58; /* mode56 the sprite record was last loaded for */
    u8 filler5A[0x6C - 0x5A];
    u16 flags;
    u16 counter;
    u8 filler70[0x74 - 0x70];
    s32 offY;
    s32 offX;
    s32 scale;
    u8 filler80[0x21C - 0x80];
    u32 unk21C;
    u32 unk220;
    u32 unk224;
    u8 filler228[0x230 - 0x228];
    void *path230;
    void *path234;
};

// The sub-object at +0x30 of the phase-4 controller.
struct Phase4Sub
{
    u8 filler0[4];
    struct GridObj *unk4;
    struct GridObj *unk8;
    struct GridObj *unkC;
};

// The jump-rope object's class. Its entry at 0x10 is the destructor; thisOffset is added to the
// object before the call.
struct RopeJumpClass
{
    u8 filler0[0x10];
    s16 thisOffset;
    u16 filler12;
    void (*destroy)(void *obj, u32 flags);
};

// The jump-rope minigame: two turners swing a rope between them, A makes Mario jump it and
// B Luigi. Three misses for either brother end the try. obj_state_machine runs it by phase
// (unk54, ROPE_JUMP_PHASE_*).
struct Phase4Obj
{
    u8 filler0[0x14];
    void *unk14;
    const struct RopeJumpClass *cls;
    void *unk1C;
    struct GridObj *unk20; // Mario (jumps with A). His counter is his miss count.
    struct GridObj *unk24; // Luigi (jumps with B). His counter is his miss count.
    struct GridObj *unk28; // rope turner A: holds one end; text bubbles show above him
    struct GridObj *unk2C; // rope turner B: holds the other end
    struct Phase4Sub *sub30;
    void *unk34; // the miss marks: 3 per brother (obj_triples_done, obj_set_child_class_3a04)
    void *unk38; // the level banner (obj_init_pair, obj_init_single, obj_slot0_is_empty)
    void *unk3C; // scroll tween (obj_start_tween_len)
    struct GridObj *path40; // a path object (freed like path_free)
    void *unk44; // freed with obj_free_pair_bufs
    void *win48; // the text window
    void *unk4C;
    struct { u16 limit; u16 counter; } wait; // counted up by timer_tick in phases 1 and 6
    u32 unk54; // phase, ROPE_JUMP_PHASE_*
    u32 state; // step within the phase
    s32 unk5C; // goes up by one, within 1..5, each time a level 1-4 is cleared
    s32 unk60; // level 1..5; level 5 plays ROPE_JUMP_FINAL_PATTERNS patterns in a row
    s32 unk64; // the pattern being played in level 5
    s32 unk68; // score: 6 minus both brothers' misses
    u32 flags; // ROPE_JUMP_*
};

// Phase4Obj.unk54
#define ROPE_JUMP_PHASE_START     0 // reload the palettes, start the music
#define ROPE_JUMP_PHASE_FADE_IN   1 // wait for the timer
#define ROPE_JUMP_PHASE_MENU      2 // obj_panel_sequence
#define ROPE_JUMP_PHASE_TUTORIAL  3 // obj_key_state_update
#define ROPE_JUMP_PHASE_PLAY      4 // playing a level: obj_phase4_step
#define ROPE_JUMP_PHASE_PAUSED    5 // START goes back to playing: obj_set_mode_if_flag8
#define ROPE_JUMP_PHASE_FADE_OUT  6 // wait for the timer
#define ROPE_JUMP_PHASE_EXIT      7 // black screen, stop the music, destroy the minigame

// Phase4Obj.flags
#define ROPE_JUMP_ROPE_FOLLOWS    0x01 // the rope's ends follow the turners' hands
#define ROPE_JUMP_NO_MISS_MARKS   0x02 // a miss isn't shown on the miss marks
#define ROPE_JUMP_JUMP_INPUT      0x04 // A and B make the brothers jump
#define ROPE_JUMP_FACE_ROPE       0x08 // everyone turns towards the rope's middle
#define ROPE_JUMP_NO_HITS         0x10 // the rope can't hit the brothers
#define ROPE_JUMP_ALT_ENDING      0x20 // after the result: the other scripts and one more text

#define ROPE_JUMP_MAX_MISSES      2 // one more ends the try
#define ROPE_JUMP_LEVELS          5
#define ROPE_JUMP_FINAL_PATTERNS  4 // patterns of level 5 (gPhase4ChildCTable[0..3])

#endif // GUARD_ROPE_JUMP_H
