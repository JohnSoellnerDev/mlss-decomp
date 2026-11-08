#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
    u8 filler8[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F[0x20 - 0x1F];
    u8 frame;
    u8 unk21;
    u8 filler22;
    u8 unk23;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 flip : 1;
    u8 unk7 : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroSpan
{
    s16 a;
    s16 b;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x114 - 0xEE];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    s16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

struct SpriteListItem
{
    struct SpriteListItem *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 layerFlags;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct SpriteListItem *sprites;
};

struct WorldModeBits
{
    u8 unk0 : 4;
    u8 field4 : 2;
    u8 unk6 : 2;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 held;
};

extern struct World *gWorld;
extern struct KeyState gSys;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void enter_finish_rebuild(struct HeroBody *body);
void start_action_4(struct HeroBody *body);
void hero_b_progress_end(struct HeroBody *body);
void hero_b_active_hand_over(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_link_to_partner_fx(struct HeroBody *body);

void hero_pair_finish_b_place_a_action_copy(struct HeroBody *body);
void hero_b_move_to_partner_pose(struct HeroBody *body);
void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_active_place_partner_anims(struct HeroBody *body);
void hero_a_walk_to_partner_then_pair(struct HeroBody *body);
void hero_pair_place_action_both(void);
void hero_b_active_place_at_a_pose(struct HeroBody *body);
void hero_b_active_start_step(struct HeroBody *body);
void hero_a_snap_to_b_target(struct HeroBody *body);
void hero_a_active_follow_b(struct HeroBody *body);
void hero_pair_follow_step(struct HeroBody *body);

/* Mode bits 4-5 of the world: bit 5 set, bit 4 kept. */
static inline void world_mode_set_bit5(void)
{
    u32 mode = gWorld->modeByte;
    u32 field = ((mode << 26) >> 30) | 2;
    gWorld->modeByte = (mode & ~0x30) | (field << 4);
}

/* Mode bits 1-2 of the sprite flags. */
static inline void sprite_mode_set(struct HeroBody *body, u32 field)
{
    ((struct HeroSpriteBits *)&body->sprite->flags)->mode = field;
}

NONMATCH("asm/code_080AB11C/hero_pair_follow_step.s", void hero_pair_follow_step(struct HeroBody *body))
{    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    struct HeroBody *s;
    struct SpriteListItem *item;
    s32 zLimit;
    s32 zA;
    s32 rate;
    s32 rest8;
    s32 rest6;
    s32 ahead4;
    s32 lim;
    s32 lo;
    s32 hi;
    s32 hi3;
    s32 reached;
    s32 mode;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s32 ofsVal;
    s32 tz2;
    s32 sum;
    s32 sq;
    s32 half;
    s32 stepDone;
    u32 mode_a;
    u32 mode_b;

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    step_motion(a);

    zLimit = b->ofsZ - 0xC;
    zA = a->posZ;
    if (zA < 0) {
        zA += 0xFF;
    }
    zA >>= 8;
    if (zA < zLimit) {
        if (b->sprite->scale == 0x100) {
            set_slot_value(0x7C, -1);
        }
        b->sprite->scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->posZ, zLimit);
    }

    rate = a->rate;
    rest8 = -(rate << 3) + 0x4000;
    rest6 = 0x4000 - (((rate << 1) + rate) << 1);
    ahead4 = (rate << 2) + 0x4000;
    if (a->progress < ahead4) {
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0) {
        goto L706;
    }
    lim = rest8;
    if (lim < 0) {
        lim = 0;
    }
    if (lim > 0x7FFF) {
        lim = 0x7FFF;
    }
    reached = 1;
    if (a->step > lim) {
        reached = 0;
    } else if (a->progress < lim) {
        reached = 0;
    }
    if (reached) {
        if ((gWorld->layerFlags & 8) != 0) {
            s = a;
            if (s != 0) {
                do {
                    s->frame = 0x33;
                    s = s->child;
                } while (s != 0);
            }
            s = b;
            if (s != 0) {
                do {
                    s->frame = 0x33;
                    s = s->child;
                } while (s != 0);
            }
            s = partner;
            if (s != 0) {
                do {
                    s->frame = 0x33;
                    s = s->child;
                } while (s != 0);
            }
            item = gWorld->sprites;
            if (item != 0) {
                do {
                    if (item->sprite != 0) {
                        item->sprite->frame = 3;
                    }
                    item = item->next;
                } while (item != 0);
            }
        } else {
            s = a;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            s = b;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            s = partner;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            item = gWorld->sprites;
            if (item != 0) {
                do {
                    if (item->sprite != 0) {
                        item->sprite->frame = 0x10;
                    }
                    item = item->next;
                } while (item != 0);
            }
        }
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    lo = rest6;
    hi = ahead4;
    if (lo < 0) {
        lo = 0;
    }
    if (hi < 0) {
        hi = 0;
    }
    if (lo > 0x4000) {
        lo = 0x4000;
    }
    if (hi > 0x7FFF) {
        hi = 0x7FFF;
    }
    if (hi < lo) {
        lo = hi;
    }
    reached = 0;
    if (a->progress >= lo) {
        reached = 1;
        if (a->step > hi) {
            reached = 0;
        }
    }
    if (reached) {
        set_body_part(b, 2, 0x18, 0, 0);
        if ((gSys.held & 2) != 0) {
            body_set_pose(1, b, 0x2E, 0);
            obj_free_parts(b);
            world_mode_set_bit5();
            if ((gWorld->modeByte & 4) == 0) {
                if ((gWorld->layerFlags & 8) != 0) {
                    s = a;
                    if (s != 0) {
                        do {
                            s->frame = 0x100;
                            s = s->child;
                        } while (s != 0);
                    }
                    s = b;
                    if (s != 0) {
                        do {
                            s->frame = 0x100;
                            s = s->child;
                        } while (s != 0);
                    }
                    s = partner;
                    if (s != 0) {
                        do {
                            s->frame = 0x100;
                            s = s->child;
                        } while (s != 0);
                    }
                    item = gWorld->sprites;
                    if (item != 0) {
                        do {
                            if (item->sprite != 0) {
                                item->sprite->frame = 0x10;
                            }
                            item = item->next;
                        } while (item != 0);
                    }
                } else {
                    s = a;
                    if (s != 0) {
                        do {
                            s->frame = 0x100;
                            s = s->child;
                        } while (s != 0);
                    }
                    s = b;
                    if (s != 0) {
                        do {
                            s->frame = 0x100;
                            s = s->child;
                        } while (s != 0);
                    }
                    s = partner;
                    if (s != 0) {
                        do {
                            s->frame = 0x100;
                            s = s->child;
                        } while (s != 0);
                    }
                    item = gWorld->sprites;
                    if (item != 0) {
                        do {
                            if (item->sprite != 0) {
                                item->sprite->frame = 0x10;
                            }
                            item = item->next;
                        } while (item != 0);
                    }
                }
                display_flush();
            }
            a->timer = (a->timer & 0xFFF) | 0x2000;
        }
    }

    if ((a->timer >> 12) != 0) {
        goto L706;
    }
    hi3 = ahead4;
    if (hi3 < 0) {
        hi3 = 0;
    }
    if (hi3 > 0x7FFF) {
        hi3 = 0x7FFF;
    }
    reached = 0;
    if (a->step <= hi3) {
        if (a->progress >= hi3) {
            reached = 1;
        }
    }
    if (reached) {
        world_mode_set_bit5();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->layerFlags & 8) != 0) {
            s = a;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            s = b;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            s = partner;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            item = gWorld->sprites;
            if (item != 0) {
                do {
                    if (item->sprite != 0) {
                        item->sprite->frame = 0x10;
                    }
                    item = item->next;
                } while (item != 0);
            }
        } else {
            s = a;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            s = b;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            s = partner;
            if (s != 0) {
                do {
                    s->frame = 0x100;
                    s = s->child;
                } while (s != 0);
            }
            item = gWorld->sprites;
            if (item != 0) {
                do {
                    if (item->sprite != 0) {
                        item->sprite->frame = 0x10;
                    }
                    item = item->next;
                } while (item != 0);
            }
        }
        display_flush();
        obj_free_parts(b);
        goto L706;
    }
    if (a->progress > ahead4) {
        goto L706;
    }
    if ((gSys.held & 3) == 0) {
        goto L706;
    }
    world_mode_set_bit5();
    if ((gWorld->modeByte & 4) != 0) {
        goto L706;
    }
    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->layerFlags & 8) != 0) {
        s = a;
        if (s != 0) {
            do {
                s->frame = 0x100;
                s = s->child;
            } while (s != 0);
        }
        s = b;
        if (s != 0) {
            do {
                s->frame = 0x100;
                s = s->child;
            } while (s != 0);
        }
        s = partner;
        if (s != 0) {
            do {
                s->frame = 0x100;
                s = s->child;
            } while (s != 0);
        }
        item = gWorld->sprites;
        if (item != 0) {
            do {
                if (item->sprite != 0) {
                    item->sprite->frame = 0x10;
                }
                item = item->next;
            } while (item != 0);
        }
    } else {
        s = a;
        if (s != 0) {
            do {
                s->frame = 0x100;
                s = s->child;
            } while (s != 0);
        }
        s = b;
        if (s != 0) {
            do {
                s->frame = 0x100;
                s = s->child;
            } while (s != 0);
        }
        s = partner;
        if (s != 0) {
            do {
                s->frame = 0x100;
                s = s->child;
            } while (s != 0);
        }
        item = gWorld->sprites;
        if (item != 0) {
            do {
                if (item->sprite != 0) {
                    item->sprite->frame = 0x10;
                }
                item = item->next;
            } while (item != 0);
        }
    }
    display_flush();
    obj_free_parts(b);
    body_set_pose(6, b, 0x2E, 0);

L706:
    mode = a->timer >> 12;
    if (mode == 0) {
        goto L780;
    }
    lo = rest6;
    hi = ahead4;
    if (lo < 0) {
        lo = 0;
    }
    if (hi < 0) {
        hi = 0;
    }
    if (lo > 0x4000) {
        lo = 0x4000;
    }
    if (hi > 0x7FFF) {
        hi = 0x7FFF;
    }
    if (hi < lo) {
        lo = hi;
    }
    stepDone = 0;
    if (a->progress >= lo) {
        if (a->step <= hi) {
            stepDone = 1;
        }
    }
    if (stepDone == 0) {
        goto L780;
    }
    if (mode <= 1) {
        goto L780;
    }
    a->timer = a->timer & 0xFFF;
    {
        void (*next)(struct HeroBody *) = hero_a_on_partner;

        body->state = next;
        next(body);
    }
    goto L8C2;

L780:
    if ((a->flags & 0x20) == 0) {
        goto L8C2;
    }
    set_slot_value(0x30, -1);
    mode_a = a->mode & 6;
    if (mode_a == 2 || mode_a == 4) {
        t = partner->targetX;
        if (t < 0) {
            t += 0xFF;
        }
        px = (t >> 8) + partner->ofsX - 8;
        t = partner->targetY;
        if (t < 0) {
            t += 0xFF;
        }
        py = (t >> 8) + partner->ofsY;
        t = partner->targetZ;
        if (t < 0) {
            t += 0xFF;
        }
        pz = (t >> 8) + partner->ofsZ - 8;
        place_targets(a, px, py, pz, -1);
        if ((u32)partner == (u32)partner->data + 8) {
            ofsVal = partner->ofsZ;
        } else {
            find_frame_entry(partner, 0);
            ofsVal = partner->unk47;
        }
        tz2 = partner->targetZ;
        if (tz2 < 0) {
            tz2 += 0xFF;
        }
        sum = ofsVal + (tz2 >> 8);
        sq = sum * sum;
        if (sq < 0) {
            sq += 0x3F;
        }
        half = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x1000, (sq >> 6) + 0x40);
        a->height = sum + half;
        set_motion_offsets(a, 0x600);
    }
    mode_a = a->mode & 6;
    if (mode_a == 2 || mode_a == 4) {
        sprite_set_action(a, 0xC, 0x2000, 0);
        sprite_mode_set(a, 0);
    }
    mode_b = b->mode & 6;
    if (mode_b == 2 || mode_b == 4) {
        sprite_set_action(b, 7, 0x205F, 0);
        sprite_mode_set(b, 1);
    }
    body->state = hero_a_link_to_partner_fx;

L8C2:
    return;
}
END_NONMATCH
