#include "global.h"

ASM_FUNC("asm/code_0802A4FC/sync_mode_353.s", void sync_mode_353(void))

ASM_FUNC("asm/code_0802A4FC/update_sub_objects.s", void update_sub_objects(void))

ASM_FUNC("asm/code_0802A4FC/pair_frame_update.s", void pair_frame_update(void))

ASM_FUNC("asm/code_0802A4FC/update_sub_frame.s", void update_sub_frame(void))

extern const u8 gDirMask[];
u32 obj_push_out(void *obj, u32 entry);
void obj_set_slot_value(void *obj, s32 value);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);

NONMATCH("asm/code_0802A4FC/obj_update_contacts.s", void obj_update_contacts(u8 *ctx, u8 *obj, u8 flag))
{
    u32 slot, entry, pushed, partner, left, start, other;
    u32 obj_x, obj_y, other_x, other_y, half_sum, hit, use_p, sb;
    u32 link_index, type_bits, mode, anim, state, d_start, d_left, action;
    s32 z, zs;
    s32 *box_x, *box_y, *box_z;

    if (obj == NULL)
        goto part2;
    if ((obj[0x33C] & 0x7F) != 0x7F)
        goto part2;
    if ((obj[0x20C] & 1) == 0)
        goto part2;
    if ((obj[0x216] & 0x20) != 0)
        goto part2;
    if ((obj[0x20F] & 1) == 0)
        goto part2;

    // box of the object
    box_x = (s32 *)(obj + 0x284);
    box_y = (s32 *)(obj + 0x288);
    box_z = (s32 *)(obj + 0x28C);
    *box_x = *(s32 *)(obj + 0x0C) - (*(u16 *)(obj + 0x272) >> 1);
    *box_y = *(s32 *)(obj + 0x10) + *(s16 *)(obj + 0x270);
    z = *(s32 *)(obj + 0x14);
    zs = *(s32 *)(obj + 0x18);
    if (zs > 0 || (zs < 0 && *(s32 *)(obj + 0x40) < 0))
        z += zs;
    *box_z = z;

    if ((ctx[0x208] & 0x10) == 0) {
        if ((obj[0x20C] & 0x40) != 0) {
            entry = *(u32 *)(ctx + 0x28 + ((obj[0x54] ^ 1) << 2));
            obj_push_out(obj, entry);
        }
    }

    if ((obj[0x20D] & 1) == 0)
        goto part2;
    if ((obj[0x35B] & 0x10) != 0)
        goto part2;

    // list C: contact flag in bit 7 of the link byte
    if ((obj[0x33C] & 0x7F) == 0x7F && ctx[0x112] != 0) {
        slot = (u32)ctx + 0x28 + (ctx[0x112] << 2);
        left = ctx[0x113];
        while (left != 0) {
            entry = *(u32 *)slot;
            pushed = obj_push_out(obj, entry);
            if ((pushed << 24) != 0) {
                obj[0x33C] &= 0x7F;
                break;
            }
            partner = *(u32 *)(entry + 0x344);
            if (partner != 0 && (*(u8 *)(partner + 0x20C) & 1) != 0) {
                pushed = obj_push_out(obj, partner);
                if ((pushed << 24) != 0) {
                    obj[0x33C] |= 0x80;
                    break;
                }
            }
            left--;
            slot += 4;
        }
    }

    if (flag != 0)
        goto part2;
    if ((obj[0x33C] & 0x7F) != 0x7F)
        goto part2;

    // the entity list the scan starts with: P or A
    if ((ctx[0x208] & 0x10) == 0)
        use_p = 1;
    else if ((obj[0x352] & 0x80) != 0)
        use_p = 1;
    else {
        sb = ctx[0x246] & 0x1C;
        use_p = (sb == 0xC) || (sb == 0x18);
    }
    if (use_p && ctx[0x10E] != 0)
        start = ctx[0x10E];
    else
        start = ctx[0x110];

    // overlap test of the entities from `start` on
    if ((s32)start > 0) {
        slot = (u32)ctx + 0x28 + (start << 2);
        left = ctx[0x10F] + ctx[0x111] - start + 2;
        if (left != 0) {
            other = 0;
            while (1) {
                entry = *(u32 *)slot;
                pushed = obj_push_out(obj, entry);
                if ((pushed << 24) != 0 && (obj[0x358] & 8) == 0) {
                    link_index = ((s32)((u32)obj[0x33C] << 25)) >> 23;
                    other = *(u32 *)(ctx + 0x28 + link_index);
                    type_bits = *(u8 *)(other + 0x20E) & 0x38;
                    if (type_bits == 8 && (obj[0x352] & 1) != 0)
                        goto probe_found;
                    if (type_bits == 0x10 && (obj[0x352] & 2) != 0)
                        goto probe_found;
                }
                left--;
                slot += 4;
                if (left == 0)
                    goto scan_end;
            }
probe_found:
            if ((*(u8 *)(other + 0x20E) & 0x38) != 8)
                goto scan_end;
            if ((((u32)*(u8 *)(other + 0x20C) << 28) >> 29) <= 1)
                goto scan_end;
            if (*(s32 *)(obj + 0x18) != 0)
                goto scan_end;

            obj_x = *(u32 *)(obj + 0x0C);
            obj_y = *(u32 *)(obj + 0x10) + *(s16 *)(obj + 0x270);
            other_x = *(u32 *)(other + 0x0C);
            other_y = *(u32 *)(other + 0x10) + *(s16 *)(other + 0x270);
            half_sum = (*(u16 *)(other + 0x272) + *(u16 *)(obj + 0x272)) >> 1;
            hit = 0;
            if ((s32)(obj_x - half_sum) >= (s32)other_x)
                goto axis;
            if ((s32)(obj_x + half_sum) <= (s32)other_x)
                goto axis;
            if ((s32)(obj_y - *(u16 *)(obj + 0x276)) >= (s32)other_y)
                goto axis;
            if ((s32)(obj_y + *(u16 *)(other + 0x276)) > (s32)other_y)
                goto contact;
axis:
            mode = obj[0x350] & 0xF;
            if ((gDirMask[obj[2] >> 5] & mode) == 0)
                goto axis_end;
            if (mode == 2)
                goto y_test;
            if (mode > 2) {
                if (mode == 4)
                    goto x_test;
                if (mode == 8)
                    goto y_test;
                goto axis_end;
            }
            if (mode == 1)
                goto x_test;
            goto axis_end;
x_test:
            {
                u32 half_obj = *(u16 *)(obj + 0x272) >> 1;
                u32 half_other = *(u16 *)(other + 0x272) >> 1;
                if ((s32)(obj_x + half_obj) <= (s32)(other_x - half_other))
                    goto axis_end;
                if ((s32)(obj_x - half_obj) >= (s32)(other_x + half_other))
                    goto axis_end;
                goto contact;
            }
y_test:
            if ((s32)obj_y <= (s32)(other_y - *(u16 *)(other + 0x276)))
                goto axis_end;
            if ((s32)(obj_y - *(u16 *)(obj + 0x276)) >= (s32)other_y)
                goto axis_end;
            hit = 1;
axis_end:
            if (hit == 0)
                goto scan_end;
contact:
            obj[0x340] = ((u32)*(u8 *)(other + 0x20C) << 28) >> 29;
        }
    }
scan_end:
    if ((obj[0x33C] & 0x7F) != 0x7F)
        goto part2;
    d_start = ctx[0x11A];
    if (d_start == 0)
        goto part2;
    slot = (u32)ctx + 0x28 + (d_start << 2);
    d_left = ctx[0x11B];
    while (d_left != 0) {
        entry = *(u32 *)slot;
        pushed = obj_push_out(obj, entry);
        if ((pushed << 24) != 0)
            break;
        d_left--;
        slot += 4;
    }

part2:
    if ((ctx[0x209] & 1) != 0)
        goto shift_only;
    action = ctx[0x12A];
    if (action == 0 || action > 2)
        goto shift_only;
    if ((ctx[0x208] & 0x10) == 0 && (obj[0x352] & 0x80) == 0)
        goto shift_only;

    state = obj[0x340];
    if (state == 2) {
        obj_set_slot_value(obj, 1);
    } else if (state == 3 && (obj[0x352] & 0x80) != 0) {
        obj_set_slot_value(obj, 2);
    } else {
        if (obj[0x341] == obj[0x340])
            return;
        obj_shift_state_slots(obj);
    }
    anim = ((obj[0x352] & 0x80) != 0) ? 0 : ((ctx[0x208] >> 4) & 1);
    advance_anim_frame(ctx, obj, anim, 0);
    return;
shift_only:
    obj_shift_state_slots(obj);
}
END_NONMATCH
