#ifndef GUARD_PAIR_SHOT_H
#define GUARD_PAIR_SHOT_H

// The linked pair of 0x4025 shots fired at both heroes (countdown_spawn_pair_link, code_08077FB8)
// and what follows them (spawn_child_4025_adopt, code_08078708).

// Which shots fly past their hero, on through the hero's position to the left edge
// (x = -0x1000). Any other value: both hit.
enum PairShotPattern
{
    PAIR_SHOT_PAST_A = 0,
    PAIR_SHOT_PAST_B = 1,
    PAIR_SHOT_PAST_BOTH = 2,
};

// Where each shot (0: at hero A, 1: at hero B) flies to: x, y and height, 1/256 px.
extern s32 gUnk_03000EB0[];
extern s32 gUnk_03000EB8[];
extern s32 gUnk_03000EC0[];

#endif // GUARD_PAIR_SHOT_H
