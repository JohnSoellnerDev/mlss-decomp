#include "global.h"

struct AttrBits
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct PackObj
{
    u8 filler0[0x20];
    u8 *attrPtr;
    u8 filler24[0x208 - 0x24];
    struct AttrBits attrs;
};

extern u8 *gUnk_03000FC0;
void cpu_set(const void *src, void *dst, u32 ctl);

ASM_FUNC("asm/code_08023DE8/scene_obj_update.s", void scene_obj_update(void))

/* Saves the scene object's state into its save block (attrPtr): copies the 0x574-byte work buffer
   at gUnk_03000FC0 (cpu_set, 0x15D words) to attrPtr + 4, puts attrs bits 0-4 into the block's
   flag byte as bits 1-5 (bits 6 and 7 stay where they are) and sets bit 0 to mark the block as
   holding a copy. obj_pack_attrs does the reverse. */
void scene_obj_return_copy(struct PackObj *obj)
{
    cpu_set(gUnk_03000FC0, obj->attrPtr + 4, 0x0400015D);
    ((struct AttrBits *)obj->attrPtr)->b1 = ((u32)*(u8 *)&obj->attrs << 31) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b2 = ((u32)*(u8 *)&obj->attrs << 30) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b3 = ((u32)*(u8 *)&obj->attrs << 29) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b4 = ((u32)*(u8 *)&obj->attrs << 28) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b5 = ((u32)*(u8 *)&obj->attrs << 27) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b6 = (u8)(*(u8 *)&obj->attrs << 1) >> 7;
    ((struct AttrBits *)obj->attrPtr)->b7 = ((u32)*(u8 *)&obj->attrs << 24) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b0 = 1;
}

/* Restores the scene object's state from its save block (attrPtr), the reverse of
   scene_obj_return_copy: copies the 0x15D words at attrPtr + 4 back into the work buffer at
   gUnk_03000FC0, takes the block's flag bits 1-5 back into attrs bits 0-4 and its bits 6-7 into
   attrs bits 6-7, and clears bit 0: the block no longer holds a copy. */
void obj_pack_attrs(struct PackObj *obj)
{
    cpu_set(obj->attrPtr + 4, gUnk_03000FC0, 0x0400015D); /* 32-bit words, 0x15D of them */
    obj->attrs.b0 = ((u32)*obj->attrPtr << 30) >> 31;
    obj->attrs.b1 = ((u32)*obj->attrPtr << 29) >> 31;
    obj->attrs.b2 = ((u32)*obj->attrPtr << 28) >> 31;
    obj->attrs.b3 = ((u32)*obj->attrPtr << 27) >> 31;
    obj->attrs.b4 = ((u32)*obj->attrPtr << 26) >> 31;
    obj->attrs.b6 = (u8)(*obj->attrPtr << 1) >> 7;
    obj->attrs.b7 = ((u32)*obj->attrPtr << 24) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b0 = 0;
}
