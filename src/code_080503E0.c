#include "global.h"

ASM_FUNC("asm/code_080503E0/obj_update_flags_from_child.s", void obj_update_flags_from_child(void))

ASM_FUNC("asm/code_080503E0/obj_attach_child.s", void obj_attach_child(void))

ASM_FUNC("asm/code_080503E0/forward_child_70.s", void forward_child_70(void))

ASM_FUNC("asm/code_080503E0/forward_child_68.s", void forward_child_68(void))

ASM_FUNC("asm/code_080503E0/forward_child_60.s", void forward_child_60(void))

ASM_FUNC("asm/code_080503E0/forward_child_58.s", void forward_child_58(void))

ASM_FUNC("asm/code_080503E0/forward_child_50.s", void forward_child_50(void))

ASM_FUNC("asm/code_080503E0/forward_child_88.s", void forward_child_88(void))

ASM_FUNC("asm/code_080503E0/forward_child_48.s", void forward_child_48(void))

ASM_FUNC("asm/code_080503E0/forward_child_40.s", void forward_child_40(void))

ASM_FUNC("asm/code_080503E0/forward_child_flag_20.s", void forward_child_flag_20(void))

ASM_FUNC("asm/code_080503E0/forward_child_28.s", void forward_child_28(void))

ASM_FUNC("asm/code_080503E0/forward_child_30.s", void forward_child_30(void))

ASM_FUNC("asm/code_080503E0/forward_child_18.s", void forward_child_18(void))

ASM_FUNC("asm/code_080503E0/forward_child_10.s", void forward_child_10(void))

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

ASM_FUNC("asm/code_080503E0/obj_teardown.s", void obj_teardown(void))

ASM_FUNC("asm/code_080503E0/obj_init_state.s", void obj_init_state(void))

ASM_FUNC("asm/code_080503E0/obj_init_from_entry.s", void obj_init_from_entry(void))

ASM_FUNC("asm/code_080503E0/obj_replace_sprite.s", void obj_replace_sprite(void))

ASM_FUNC("asm/code_080503E0/obj_apply_pending_sprite.s", void obj_apply_pending_sprite(void))

ASM_FUNC("asm/code_080503E0/obj_apply_anim_code.s", void obj_apply_anim_code(void))

ASM_FUNC("asm/code_080503E0/obj_activate_sprite.s", void obj_activate_sprite(void))

ASM_FUNC("asm/code_080503E0/obj_commit_sprite.s", void obj_commit_sprite(void))

ASM_FUNC("asm/code_080503E0/obj_step_anim.s", void obj_step_anim(void))

ASM_FUNC("asm/code_080503E0/obj_clear_mask_if_timer.s", void obj_clear_mask_if_timer(void))

ASM_FUNC("asm/code_080503E0/script_init_obj_refresh_sprite.s", void script_init_obj_refresh_sprite(void))

ASM_FUNC("asm/code_080503E0/obj_init_flags.s", void obj_init_flags(void))

ASM_FUNC("asm/code_080503E0/script_init_obj_sync_sprite_pos.s", void script_init_obj_sync_sprite_pos(void))

ASM_FUNC("asm/code_080503E0/script_init_obj_setup_sprite.s", void script_init_obj_setup_sprite(void))

ASM_FUNC("asm/code_080503E0/stub_nop_080514BE.s", void stub_nop_080514BE(void))

ASM_FUNC("asm/code_080503E0/sub_080514C4.s", void sub_080514C4(void))

ASM_FUNC("asm/code_080503E0/sub_080514C8.s", void sub_080514C8(void))

ASM_FUNC("asm/code_080503E0/sub_080514CC.s", void sub_080514CC(void))

ASM_FUNC("asm/code_080503E0/sub_080514D0.s", void sub_080514D0(void))

ASM_FUNC("asm/code_080503E0/sub_080514D4.s", void sub_080514D4(void))

ASM_FUNC("asm/code_080503E0/obj_random_pick.s", void obj_random_pick(void))

ASM_FUNC("asm/code_080503E0/obj_random_enter.s", void obj_random_enter(void))

ASM_FUNC("asm/code_080503E0/obj_bind_callbacks.s", void obj_bind_callbacks(void))

ASM_FUNC("asm/code_080503E0/obj_bind_callbacks_entry.s", void obj_bind_callbacks_entry(void))

ASM_FUNC("asm/code_080503E0/obj_call_hook_1a0.s", void obj_call_hook_1a0(void))

ASM_FUNC("asm/code_080503E0/window_create.s", void window_create(void))

ASM_FUNC("asm/code_080503E0/window_state_step.s", void window_state_step(void))

ASM_FUNC("asm/code_080503E0/scene_update_sprites.s", void scene_update_sprites(void))

ASM_FUNC("asm/code_080503E0/obj_emit_oam.s", void obj_emit_oam(void))

ASM_FUNC("asm/code_080503E0/scroll_clamp.s", void scroll_clamp(void))

ASM_FUNC("asm/code_080503E0/window_free.s", void window_free(void))

ASM_FUNC("asm/code_080503E0/window_sub_block_remove.s", void window_sub_block_remove(void))

ASM_FUNC("asm/code_080503E0/window_sub_block_init.s", void window_sub_block_init(void))

ASM_FUNC("asm/code_080503E0/window_place.s", void window_place(void))

ASM_FUNC("asm/code_080503E0/window_anim_step.s", void window_anim_step(void))

ASM_FUNC("asm/code_080503E0/textbox_open.s", void textbox_open(void))

ASM_FUNC("asm/code_080503E0/textbox_task_step.s", void textbox_task_step(void))

ASM_FUNC("asm/code_080503E0/apply_display_flags.s", void apply_display_flags(void))

ASM_FUNC("asm/code_080503E0/load_tiles_and_voice.s", void load_tiles_and_voice(void))

ASM_FUNC("asm/code_080503E0/textbox_draw_rows.s", void textbox_draw_rows(void))

ASM_FUNC("asm/code_080503E0/textbox_setup_tiles.s", void textbox_setup_tiles(void))

ASM_FUNC("asm/code_080503E0/textbox_place.s", void textbox_place(void))

ASM_FUNC("asm/code_080503E0/textbox_emit_oam.s", void textbox_emit_oam(void))

ASM_FUNC("asm/code_080503E0/textbox_open_scene.s", void textbox_open_scene(void))

ASM_FUNC("asm/code_080503E0/textbox_free.s", void textbox_free(void))

ASM_FUNC("asm/code_080503E0/textbox_step.s", void textbox_step(void))

ASM_FUNC("asm/code_080503E0/textbox_create.s", void textbox_create(void))

ASM_FUNC("asm/code_080503E0/textbox_update.s", void textbox_update(void))

ASM_FUNC("asm/code_080503E0/textbox_place_style.s", void textbox_place_style(void))

ASM_FUNC("asm/code_080503E0/textbox_task_free.s", void textbox_task_free(void))

ASM_FUNC("asm/code_080503E0/oam_emit_objects.s", void oam_emit_objects(void))

ASM_FUNC("asm/code_080503E0/textbox_place_rect.s", void textbox_place_rect(void))

ASM_FUNC("asm/code_080503E0/textbox_place_anchor.s", void textbox_place_anchor(void))

ASM_FUNC("asm/code_080503E0/textbox_free_disable_link.s", void textbox_free_disable_link(void))

ASM_FUNC("asm/code_080503E0/task_descr_box_init.s", void task_descr_box_init(void))

ASM_FUNC("asm/code_080503E0/desc_child_init.s", void desc_child_init(void))

ASM_FUNC("asm/code_080503E0/overlay_init.s", void overlay_init(void))

ASM_FUNC("asm/code_080503E0/overlay_update.s", void overlay_update(void))

ASM_FUNC("asm/code_080503E0/textbox_draw_oam.s", void textbox_draw_oam(void))

ASM_FUNC("asm/code_080503E0/overlay_free.s", void overlay_free(void))

ASM_FUNC("asm/code_080503E0/overlay_child_init.s", void overlay_child_init(void))

ASM_FUNC("asm/code_080503E0/scene_create.s", void scene_create(void))

ASM_FUNC("asm/code_080503E0/scene_step.s", void scene_step(void))

ASM_FUNC("asm/code_080503E0/scene_start_task.s", void scene_start_task(void))

ASM_FUNC("asm/code_080503E0/scene_reset_sprites.s", void scene_reset_sprites(void))

ASM_FUNC("asm/code_080503E0/map_obj_draw_obj_setup_sprites.s", void map_obj_draw_obj_setup_sprites(void))

ASM_FUNC("asm/code_080503E0/hblank_scroll_table_dma.s", void hblank_scroll_table_dma(void))

ASM_FUNC("asm/code_080503E0/map_scene_update.s", void map_scene_update(void))

ASM_FUNC("asm/code_080503E0/oam_draw_unit_markers.s", void oam_draw_unit_markers(void))

ASM_FUNC("asm/code_080503E0/set_obj_digits.s", void set_obj_digits(void))

ASM_FUNC("asm/code_080503E0/map_scene_free.s", void map_scene_free(void))

ASM_FUNC("asm/code_080503E0/flip_oam_buffer.s", void flip_oam_buffer(void))

ASM_FUNC("asm/code_080503E0/scene_step_free.s", void scene_step_free(void))

ASM_FUNC("asm/code_080503E0/scene_child_init.s", void scene_child_init(void))

ASM_FUNC("asm/code_080503E0/sio_send_callback.s", void sio_send_callback(void))

ASM_FUNC("asm/code_080503E0/sio_reset_and_ack.s", void sio_reset_and_ack(void))

ASM_FUNC("asm/code_080503E0/sio_multi_setup.s", void sio_multi_setup(void))

ASM_FUNC("asm/code_080503E0/check_warm_reset_counter.s", void check_warm_reset_counter(void))

ASM_FUNC("asm/code_080503E0/map_obj_start.s", void map_obj_start(void))

ASM_FUNC("asm/code_080503E0/obj_release_parts.s", void obj_release_parts(void))
