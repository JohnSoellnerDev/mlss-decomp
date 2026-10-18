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

NONMATCH("asm/code_08023DE8/scene_obj_return_copy.s", void scene_obj_return_copy(struct PackObj *obj))
{
    cpu_set(gUnk_03000FC0, obj->attrPtr + 4, 0x0400015D);
    ((struct AttrBits *)obj->attrPtr)->b1 = ((u32)*(u8 *)&obj->attrs << 31) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b2 = ((u32)*(u8 *)&obj->attrs << 30) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b3 = ((u32)*(u8 *)&obj->attrs << 29) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b4 = ((u32)*(u8 *)&obj->attrs << 28) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b5 = ((u32)*(u8 *)&obj->attrs << 27) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b6 = ((u32)*(u8 *)&obj->attrs << 25) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b7 = ((u32)*(u8 *)&obj->attrs << 24) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b0 = 1;
}
END_NONMATCH

NONMATCH("asm/code_08023DE8/obj_pack_attrs.s", void obj_pack_attrs(struct PackObj *obj))
{
    cpu_set(obj->attrPtr + 4, gUnk_03000FC0, 0x0400015D);
    obj->attrs.b0 = ((u32)*obj->attrPtr << 30) >> 31;
    obj->attrs.b1 = ((u32)*obj->attrPtr << 29) >> 31;
    obj->attrs.b2 = ((u32)*obj->attrPtr << 28) >> 31;
    obj->attrs.b3 = ((u32)*obj->attrPtr << 27) >> 31;
    obj->attrs.b4 = ((u32)*obj->attrPtr << 26) >> 31;
    obj->attrs.b6 = ((u32)*obj->attrPtr << 25) >> 31;
    obj->attrs.b7 = ((u32)*obj->attrPtr << 24) >> 31;
    ((struct AttrBits *)obj->attrPtr)->b0 = 0;
}
END_NONMATCH
