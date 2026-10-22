#include "global.h"
#include "ram_routines.h"

extern void *gUnk_03001074;
extern u32 *gUnk_03001070;
u32 arc_tan2(s32 x, s32 y);
void cpu_set(const void *src, void *dst, u32 ctl);

struct SoundVoice
{
    u32 unk0;
    u8 filler4[4];
    void *unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
};

struct SoundVoiceFlags
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

ASM_FUNC("asm/sound_0819A4F0/thunk_set_r0_91_jump.s", void thunk_set_r0_91_jump(void))

u8 *mem_routines_init(u8 *buf)
{
    u8 *dst;
    u32 size;
    u32 base;

    if (buf == 0)
    {
        gMemRoutines = iwram_fill;
        return 0;
    }
    dst = (u8 *)(((u32)buf + 3) & -4);
    base = (u32)iwram_fill;
    size = (u32)iwram_sqrt - base;
    cpu_set(iwram_fill, dst, (size << 10) >> 11);
    gMemRoutines = dst;
    dst += size;
    return dst;
}

u8 vcount_store(u8 *out)
{
    *out = REG_VCOUNT;
    return *out;
}

u32 atan2_angle(s16 x, s16 y)
{
    return (arc_tan2(x, y) << 16) >> 24;
}

u8 *math_routines_init(u8 *buf)
{
    u8 *dst;
    u32 size;
    u32 base;

    if (buf == 0)
    {
        gMathRoutines = iwram_sqrt;
        return 0;
    }
    dst = (u8 *)(((u32)buf + 3) & -4);
    base = (u32)iwram_sqrt;
    size = (u32)gSineTable - base;
    cpu_set(iwram_sqrt, dst, ((size << 9) >> 11) | 0x04000000);
    gMathRoutines = dst;
    dst += size;
    return dst;
}

ASM_FUNC("asm/sound_0819A4F0/driver_tables_init.s", void driver_tables_init(void))

ASM_FUNC("asm/sound_0819A4F0/track_run.s", void track_run(void))

ASM_FUNC("asm/sound_0819A4F0/track_command.s", void track_command(void))

ASM_FUNC("asm/sound_0819A4F0/track_scan.s", void track_scan(void))

ASM_FUNC("asm/sound_0819A4F0/track_voice_step.s", void track_voice_step(void))

u32 load_table_03001070(u32 *p, u32 i)
{
    p[1] = gUnk_03001070[i];
    return 0;
}

void *voice_set_data_ptr(struct SoundVoice *ch, u8 a1, u8 a2, void *ptr)
{
    void *old = ch->unk8;
    ch->unkC = a1;
    ch->unkD = a2;
    ch->unk8 = ptr;
    ((struct SoundVoiceFlags *)&ch->unk12)->bit5 = 0;
    return old;
}

ASM_FUNC("asm/sound_0819A4F0/voice_setup_params.s", void voice_setup_params(void))

ASM_FUNC("asm/sound_0819A4F0/psg_wave_setup.s", void psg_wave_setup(void))

void set_03001074(void *p)
{
    gUnk_03001074 = p;
}

ASM_FUNC("asm/sound_0819A4F0/mt19937_next.s", void mt19937_next(void))

ASM_FUNC("asm/sound_0819A4F0/mt19937_seed.s", void mt19937_seed(void))

ASM_FUNC("asm/sound_0819A4F0/pitch_table_lookup.s", void pitch_table_lookup(void))
