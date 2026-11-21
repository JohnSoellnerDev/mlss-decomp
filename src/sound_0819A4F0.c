/* agbcc: -funroll-loops */
#include "global.h"
#include "ram_routines.h"

extern void *gUnk_03001074;
extern u32 *gUnk_03001070;
u32 arc_tan2(s32 x, s32 y);
void cpu_set(const void *src, void *dst, u32 ctl);

/* A text printer (the "voice"/"track" names here are an older guess at sound code): reads a byte
   string from `text` and draws it glyph by glyph; command bytes are handled by track_command, and
   control codes 0x01-0x1F by the handler set_03001074 installs (text_stream_step). */
struct TextPrinter
{
    u32 unk0;
    u32 font;       // set by command 0x4n: entry n of the table driver_tables_init was given
    void *text;     // the next byte to read
    u8 x;           // cursor, in pixels
    u8 y;
    u8 leftMargin;  // pixels
    u8 rightMargin; // pixels
    u8 unk10;
    u8 levels;      // struct TextLevels
    u8 flags;       // struct TextFlags
    u8 width;       // bits 2-7: line width in tiles
};

struct SoundVoiceFlags
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

/* TextPrinter.levels: command 0x2n sets cur = base + n (at most 14). */
struct TextLevels
{
    u8 base : 4;
    u8 cur : 4;
} __attribute__((packed));

/* TextPrinter.flags */
struct TextFlags
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 align : 2; // TEXT_ALIGN_*
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 hi2 : 2;
} __attribute__((packed));

/* TextPrinter.flags and .width as one u16 */
struct TextFlags16
{
    u16 lo6 : 6;
    u16 table : 4; // set by command 0x5n
    u16 hi6 : 6;
};

#define TEXT_ALIGN_LEFT   0
#define TEXT_ALIGN_CENTER 1
#define TEXT_ALIGN_RIGHT  2

#define TEXT_LEVELS(p) ((struct TextLevels *)&(p)->levels)
#define TEXT_FLAGS(p) ((struct TextFlags *)&(p)->flags)

void mem_copy_fast(void *dst, const void *src, u32 n);
void track_scan();
u32 load_table_03001070(u32 *p, u32 i);

/* ARM code (umull r2, r0, r1, r0; bx lr): returns the high 32 bits of a * b, i.e.
 * ((u64)a * b) >> 32. agbcc only emits Thumb code, so it stays assembly. */
ASM_FUNC("asm/sound_0819A4F0/thunk_set_r0_91_jump.s", u32 thunk_set_r0_91_jump(u32 a, u32 b))

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

extern u8 iwram_overlay[];
extern u32 gUnk_03001040[];
/* Sets up the text printer: `fonts` becomes the font table (gUnk_03001070, command 0x4n picks
 * entry n) and the control-code handler (gUnk_03001074) is cleared. Without a buffer the 12
 * routines of the gUnk_03001040 table run from ROM (iwram_overlay); with one, the code from
 * iwram_overlay up to this file (0x0819A4F0) is copied there and the table points at the copies.
 * Returns the end of the copy in the buffer, or NULL. */
u8 *driver_tables_init(u8 *ramBuf, u32 *fonts)
{
    register u32 romBase asm("r4");
    register u32 routine asm("r0");
    u32 size;

    gUnk_03001070 = fonts;
    gUnk_03001074 = 0;
    if (ramBuf == 0)
    {
        gUnk_03001040[0] = routine = (u32)iwram_overlay;
        gUnk_03001040[1] = routine = (u32)(iwram_overlay + 0x1D4);
        gUnk_03001040[2] = routine = (u32)(iwram_overlay + 0x45C);
        gUnk_03001040[3] = routine = (u32)(iwram_overlay + 0x4F4);
        gUnk_03001040[4] = routine = (u32)(iwram_overlay + 0x70);
        gUnk_03001040[5] = routine = (u32)(iwram_overlay + 0x270);
        gUnk_03001040[6] = routine = (u32)(iwram_overlay + 0x5D4);
        gUnk_03001040[7] = routine = (u32)(iwram_overlay + 0x6D0);
        gUnk_03001040[8] = routine = (u32)(iwram_overlay + 0x124);
        gUnk_03001040[9] = routine = (u32)(iwram_overlay + 0x368);
        gUnk_03001040[10] = routine = (u32)(iwram_overlay + 0x840);
        gUnk_03001040[11] = routine = (u32)(iwram_overlay + 0x968);
        return 0;
    }
    romBase = (u32)iwram_overlay;
    size = 0x0819A4F0 - romBase;
    cpu_set((void *)romBase, ramBuf, (size << 10) >> 11);
    gUnk_03001040[0] = (u32)ramBuf;
    routine = (u32)(iwram_overlay + 0x1D4);
    gUnk_03001040[1] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x45C);
    gUnk_03001040[2] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x4F4);
    gUnk_03001040[3] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x70);
    gUnk_03001040[4] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x270);
    gUnk_03001040[5] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x5D4);
    gUnk_03001040[6] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x6D0);
    gUnk_03001040[7] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x124);
    gUnk_03001040[8] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x368);
    gUnk_03001040[9] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x840);
    gUnk_03001040[10] = (u32)ramBuf - (romBase - routine);
    routine = (u32)(iwram_overlay + 0x968);
    romBase -= routine;
    romBase = (u32)ramBuf - romBase;
    gUnk_03001040[11] = romBase;
    return ramBuf + size;
}

ASM_FUNC("asm/sound_0819A4F0/track_run.s", void track_run(void))

/* Runs the text command byte just read (text[-1]):
     0x00       end: clear flags bit 5      0x01-0x1F  control code for the installed handler
     0x2n       levels.cur = base + n       0x30-0x33  flags bits 0-1 = n
     0x34-0x36  align the line left/center/right (measures it with track_scan unless noScan)
     0x37-0x38  flags bit 4 = n & 1         0x4n       use font n
     0x5n       flags16.table = n           0x60-0xDF  move the cursor right/left/down/up by 1-32 */
void track_command(struct TextPrinter *printer, u32 noScan)
{
    u8 cmd = ((u8 *)printer->text)[-1];

    if (cmd < 0x20)
    {
        if ((cmd & 0x1F) == 0)
            TEXT_FLAGS(printer)->bit5 = 0;
        else if (gUnk_03001074 != NULL)
            ((void (*)(struct TextPrinter *, u32))gUnk_03001074)(printer, noScan);
    }
    else if (cmd < 0x30)
    {
        TEXT_LEVELS(printer)->cur = TEXT_LEVELS(printer)->base + (cmd & 0xF);
        if (TEXT_LEVELS(printer)->cur > 14)
            TEXT_LEVELS(printer)->cur = 14;
    }
    else if (cmd < 0x40)
    {
        u8 arg = cmd & 0xF;

        if (arg < 4)
        {
            u32 bit = 1;

            TEXT_FLAGS(printer)->bit0 = arg & bit;
            bit = arg;
            TEXT_FLAGS(printer)->bit1 = bit >> 1;
        }
        else if (arg < 7)
        {
            if (noScan == 0)
            {
                struct TextPrinter copy;
                u32 measure;

                // Measure the rest of the line on a copy; its width in pixels is the upper half.
                mem_copy_fast(&copy, printer, 0x14);
                measure = ((u32 (*)(struct TextPrinter))track_scan)(copy);
                if (TEXT_FLAGS(printer)->align != arg - 4)
                {
                    TEXT_FLAGS(printer)->align = arg - 4;
                    switch ((u32)TEXT_FLAGS(printer)->align)
                    {
                    case TEXT_ALIGN_LEFT:
                        printer->x = printer->leftMargin;
                        break;
                    case TEXT_ALIGN_CENTER:
                        printer->x = (((printer->width >> 2) << 3) + printer->leftMargin - printer->rightMargin - (measure >> 16)) >> 1;
                        break;
                    case TEXT_ALIGN_RIGHT:
                        printer->x = ((printer->width >> 2) << 3) - printer->rightMargin - (measure >> 16);
                        break;
                    }
                }
            }
        }
        else if (arg < 9)
        {
            TEXT_FLAGS(printer)->bit4 = arg;
        }
    }
    else if (cmd < 0x50)
    {
        load_table_03001070((u32 *)printer, cmd & 0xF);
    }
    else if (cmd < 0x60)
    {
        ((struct TextFlags16 *)&printer->flags)->table = cmd & 0xF;
    }
    else if (cmd < 0x80)
    {
        printer->x += cmd - 0x5F;
    }
    else if (cmd < 0xA0)
    {
        printer->x -= cmd - 0x7F;
    }
    else if (cmd < 0xC0)
    {
        printer->y += cmd - 0x9F;
    }
    else if (cmd < 0xE0)
    {
        printer->y -= cmd - 0xBF;
    }
}

ASM_FUNC("asm/sound_0819A4F0/track_scan.s", void track_scan(void))

ASM_FUNC("asm/sound_0819A4F0/track_voice_step.s", void track_voice_step(void))

u32 load_table_03001070(u32 *p, u32 i)
{
    p[1] = gUnk_03001070[i];
    return 0;
}

void *voice_set_data_ptr(struct TextPrinter *ch, u8 a1, u8 a2, void *ptr)
{
    void *old = ch->text;
    ch->x = a1;
    ch->y = a2;
    ch->text = ptr;
    ((struct SoundVoiceFlags *)&ch->flags)->bit5 = 0;
    return old;
}

ASM_FUNC("asm/sound_0819A4F0/voice_setup_params.s", void voice_setup_params(void))

ASM_FUNC("asm/sound_0819A4F0/psg_wave_setup.s", void psg_wave_setup(void))

void set_03001074(void *p)
{
    gUnk_03001074 = p;
}

#define MT_N 624
#define MT_M 397
#define MT_MATRIX_A 0x9908B0DF
#define MT_UPPER_MASK 0x80000000
#define MT_LOWER_MASK 0x7FFFFFFF
#define MT_MIXBITS(u, v) (((u) & MT_UPPER_MASK) | ((v) & MT_LOWER_MASK))
#define MT_TWIST(u, v) ((MT_MIXBITS(u, v) >> 1) ^ ((v) & 1 ? MT_MATRIX_A : 0))
#define MT_TEMPERING_MASK_B 0x9D2C5680
#define MT_TEMPERING_MASK_C 0xEFC60000

/* Mersenne Twister (MT19937) generator state. */
struct Mt19937
{
    u32 state[MT_N];
    s32 left;               /* words left before the state is regenerated */
    u32 *next;              /* next state word to temper and return */
};

/* The game's random number generator. */
extern struct Mt19937 *gUnk_03001078;


/*
 * Returns the next 32-bit random number of the Mersenne Twister (MT19937),
 * regenerating all MT_N state words first once they are used up.
 * (The dead "= 0" initializers keep each mask inside its loop.)
 */
u32 mt19937_next(void)
{
    u32 y;
    u32 *word = gUnk_03001078->state;

    if (--gUnk_03001078->left == 0)
    {
        s32 count;
        u32 upperBits;

        gUnk_03001078->left = MT_N;
        gUnk_03001078->next = gUnk_03001078->state;

        /* words 0 .. N-M-1 twist with the word M ahead */
        for (count = MT_N - MT_M + 1; --count; word++)
        {
            u32 upper = 0;
            u32 lower = 0;
            s32 offset = 0;
            u32 bit = 0;
            u32 mixed;
            register u32 nextWord asm("r2");
            u32 partner;
            register u32 result asm("r2");

            upperBits = word[0];
            upper = MT_UPPER_MASK;
            upperBits &= upper;
            nextWord = word[1];
            lower = MT_LOWER_MASK;
            mixed = (upperBits | (nextWord & lower)) >> 1;
            offset = MT_M * 4;
            partner = *(u32 *)((u8 *)word + offset);
            bit = 1;
            nextWord &= bit;
            if (nextWord)
            {
                result = MT_MATRIX_A;
                result ^= partner;
            }
            else
                result = partner;
            result ^= mixed;
            *word = result;
        }

        /* words N-M .. N-2 wrap around to the start of the state */
        for (count = MT_M; --count; word++)
        {
            u32 upper = 0;
            u32 lower = 0;
            s32 offset = 0;
            u32 bit = 0;
            u32 mixed;
            register u32 nextWord asm("r2");
            u32 partner;
            register u32 result asm("r0");

            upperBits = word[0];
            upper = MT_UPPER_MASK;
            upperBits &= upper;
            nextWord = word[1];
            lower = MT_LOWER_MASK;
            mixed = (upperBits | (nextWord & lower)) >> 1;
            offset = (MT_M - MT_N) * 4;
            partner = *(u32 *)((u8 *)word + offset);
            bit = 1;
            nextWord &= bit;
            if (nextWord)
            {
                result = MT_MATRIX_A;
                result ^= partner;
            }
            else
                result = partner;
            result ^= mixed;
            *word = result;
        }

        /* the last word twists with state[0] */
        *word = word[MT_M - MT_N] ^ MT_TWIST(word[0], gUnk_03001078->state[0]);
    }

    /* tempering */
    y = *gUnk_03001078->next++;
    y ^= y >> 11;
    y ^= (y << 7) & MT_TEMPERING_MASK_B;
    y ^= (y << 15) & MT_TEMPERING_MASK_C;
    y ^= y >> 18;
    return y;
}

ASM_FUNC("asm/sound_0819A4F0/mt19937_seed.s", void mt19937_seed(void))

ASM_FUNC("asm/sound_0819A4F0/pitch_table_lookup.s", void pitch_table_lookup(void))
