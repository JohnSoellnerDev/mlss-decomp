#include "global.h"

ASM_FUNC("asm/code_080502DC/obj_set_mode_4.s", void obj_set_mode_4(void))

ASM_FUNC("asm/code_080502DC/obj_update_flags_from_child.s", void obj_update_flags_from_child(void))

ASM_FUNC("asm/code_080502DC/obj_attach_child.s", void obj_attach_child(void))

ASM_FUNC("asm/code_080502DC/forward_child_70.s", void forward_child_70(void))

ASM_FUNC("asm/code_080502DC/forward_child_68.s", void forward_child_68(void))

ASM_FUNC("asm/code_080502DC/forward_child_60.s", void forward_child_60(void))

ASM_FUNC("asm/code_080502DC/forward_child_58.s", void forward_child_58(void))

ASM_FUNC("asm/code_080502DC/forward_child_50.s", void forward_child_50(void))

ASM_FUNC("asm/code_080502DC/forward_child_88.s", void forward_child_88(void))

ASM_FUNC("asm/code_080502DC/forward_child_48.s", void forward_child_48(void))

ASM_FUNC("asm/code_080502DC/forward_child_40.s", void forward_child_40(void))

ASM_FUNC("asm/code_080502DC/forward_child_flag_20.s", void forward_child_flag_20(void))

ASM_FUNC("asm/code_080502DC/forward_child_28.s", void forward_child_28(void))

ASM_FUNC("asm/code_080502DC/forward_child_30.s", void forward_child_30(void))

ASM_FUNC("asm/code_080502DC/forward_child_18.s", void forward_child_18(void))

ASM_FUNC("asm/code_080502DC/forward_child_10.s", void forward_child_10(void))

struct ObjVtable
{
    u8 filler0[0x20];
    s16 thisOffset;
    u16 filler22;
    void (*update)(void *);
};

struct ChildObj
{
    u8 filler0[0x22C];
    u32 unk22C_0 : 10;
    u32 unk22C_10 : 12;
    u32 unk22C_22 : 10;
    u8 filler230[0x338 - 0x230];
    struct ObjVtable *vtable;
    u8 filler33C[2];
    u8 flags;
    u8 filler33F[0x344 - 0x33F];
    u8 *parent;
};

extern u8 *gUnk_03000FC0;
void set_flag(u8 *flags, u32 a, u32 b, u32 c);

void obj_update_child(struct ChildObj *obj)
{
    struct ObjVtable *vtable;

    set_flag(gUnk_03000FC0 + 0x38C, 0, obj->unk22C_10 + 0x960, 0);
    if (obj->flags & 0x10)
    {
        obj->parent[0x20C] |= 1;
        vtable = obj->vtable;
        vtable->update((u8 *)obj + vtable->thisOffset);
    }
}

ASM_FUNC("asm/code_080502DC/obj_teardown.s", void obj_teardown(void))

ASM_FUNC("asm/code_080502DC/obj_init_state.s", void obj_init_state(void))

ASM_FUNC("asm/code_080502DC/obj_init_from_entry.s", void obj_init_from_entry(void))

ASM_FUNC("asm/code_080502DC/obj_replace_sprite.s", void obj_replace_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_apply_pending_sprite.s", void obj_apply_pending_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_apply_anim_code.s", void obj_apply_anim_code(void))

ASM_FUNC("asm/code_080502DC/obj_activate_sprite.s", void obj_activate_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_commit_sprite.s", void obj_commit_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_step_anim.s", void obj_step_anim(void))

ASM_FUNC("asm/code_080502DC/obj_clear_mask_if_timer.s", void obj_clear_mask_if_timer(void))

ASM_FUNC("asm/code_080502DC/script_init_obj_refresh_sprite.s", void script_init_obj_refresh_sprite(void))

ASM_FUNC("asm/code_080502DC/obj_init_flags.s", void obj_init_flags(void))

ASM_FUNC("asm/code_080502DC/script_init_obj_sync_sprite_pos.s", void script_init_obj_sync_sprite_pos(void))

ASM_FUNC("asm/code_080502DC/script_init_obj_setup_sprite.s", void script_init_obj_setup_sprite(void))

ASM_FUNC("asm/code_080502DC/stub_nop_080514BE.s", void stub_nop_080514BE(void))

ASM_FUNC("asm/code_080502DC/sub_080514C4.s", void sub_080514C4(void))

ASM_FUNC("asm/code_080502DC/sub_080514C8.s", void sub_080514C8(void))

ASM_FUNC("asm/code_080502DC/sub_080514CC.s", void sub_080514CC(void))

ASM_FUNC("asm/code_080502DC/sub_080514D0.s", void sub_080514D0(void))

ASM_FUNC("asm/code_080502DC/sub_080514D4.s", void sub_080514D4(void))

ASM_FUNC("asm/code_080502DC/obj_random_pick.s", void obj_random_pick(void))

ASM_FUNC("asm/code_080502DC/obj_random_enter.s", void obj_random_enter(void))

ASM_FUNC("asm/code_080502DC/obj_bind_callbacks.s", void obj_bind_callbacks(void))

ASM_FUNC("asm/code_080502DC/obj_bind_callbacks_entry.s", void obj_bind_callbacks_entry(void))

ASM_FUNC("asm/code_080502DC/obj_call_hook_1a0.s", void obj_call_hook_1a0(void))

ASM_FUNC("asm/code_080502DC/window_create.s", void window_create(void))

ASM_FUNC("asm/code_080502DC/window_state_step.s", void window_state_step(void))

ASM_FUNC("asm/code_080502DC/scene_update_sprites.s", void scene_update_sprites(void))

ASM_FUNC("asm/code_080502DC/obj_emit_oam.s", void obj_emit_oam(void))
