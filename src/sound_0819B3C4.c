#include "global.h"

// One track of a player: 0x20 bytes, 12 per player.
struct SndTrack
{
    u16 status;          // +0x00: bit 0 active, 0x100 pitch LFO on, 0x200/0x400 pitch LFO phase, 0x800 volume LFO on, 0x1000/0x2000 volume LFO phase
    u8 unk2;
    u8 unk3;
    u32 cmdTable;        // +0x04
    u16 unk8;
    u8 unkA;
    u8 key;              // +0x0B
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    s8 bend;             // +0x10
    s8 fine;             // +0x11
    u8 voice;            // +0x12: index of the voice slot
    u8 span;             // +0x13
    u8 volDepth;         // +0x14
    u8 volReload;        // +0x15
    u8 unk16;
    u8 volCounter;       // +0x17
    u8 pitchDepth;       // +0x18
    u8 pitchReload;      // +0x19
    u8 pitchDelay;       // +0x1A
    u8 pitchCounter;     // +0x1B
    u8 filler1C[4];
};

struct SndPlayer
{
    struct SndTrack tracks[12];
};

// Driver state, pointer at 0x03007FF0.
struct SoundInfo
{
    struct SndPlayer players[3];
    u16 cmdQueue[8];     // +0x480
    s16 cmdRead;         // +0x490
    s16 cmdWrite;        // +0x492
    u8 playerFlags[4];   // +0x494
    u16 volTable[6];     // +0x498, indexed by track span * 2
    u32 playerWord[3];   // +0x4A4
    u16 trackSong[12];   // +0x4B0
    u8 cgbRec[5][12];    // +0x4C8, 12 bytes per CGB channel
};

// One voice slot, 16 bytes.
struct Voice
{
    u8 flags;            // +0x00
    u8 filler1;
    u16 unk2;            // +0x02
    u32 unk4;            // +0x04
    u8 volLeft;          // +0x08
    u8 volRight;         // +0x09
    u16 pitch;           // +0x0A
    u8 unkC;             // +0x0C
    u8 unkD;
    u8 unkE;
    u8 unkF;
};

// Entry of a tone list: key range and four bytes of data.
struct ToneEntry
{
    u8 keyLo;            // +0x00
    u8 keyHi;            // +0x01
    u16 unk2;            // +0x02
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
};

// Release handler table entry: callback at +0x14.
struct ReleaseEntry
{
    u8 filler0[0x14];
    void (*callback)(struct SndTrack *);
};

// Release table entry used by the volume update: callback at +8.
struct VolEntry
{
    u8 filler0[8];
    void (*callback)(struct SndTrack *);
};

// Track class: callbacks at +0 and +0x18.
struct TrackClass
{
    void (*fn0)(struct SndTrack *);
    u8 filler4[0x14];
    void (*fn18)(struct SndTrack *);
};

// Owner of a volume LFO: callback at +8.
struct LfoVolOwner
{
    u8 filler0[8];
    void (*callback)(struct SndTrack *);
};

// Owner of a pitch LFO: callback at +0x10.
struct LfoPitchOwner
{
    u8 filler0[0x10];
    void (*callback)(struct SndTrack *);
};

extern u8 gUnk_03001B50[];
extern const u8 gKeyValueTable[];
extern const u8 gPsgWaveTable[];
extern const u8 gWaveVolumeCodes[];
extern u8 *const gSongTableLo[];
extern u8 *const gSongTableId[];
extern void *const gReleaseTable[];
extern const u16 gPsgFreqTable[];
extern const u16 gToneProgramOffsets[];
extern u8 gUnk_03001B5A[];
extern u8 gTrackClassDefault[];

void track_step(struct SndTrack *tr, struct TrackClass *cls, u32 word);

struct ToneEntry *find_tone(u32 prog, s32 key);
void track_key_off(u32 idx, struct SndTrack *ch);
u32 cgb_env_level(u32 idx);

static inline struct SoundInfo *sound_info(void)
{
    return *(struct SoundInfo **)0x03007FF0;
}

void voice_set_pitch(struct SndTrack *ch);
void voice_set_volume(struct SndTrack *ch);
void process_cmd_queue(void);
void players_update(void);
void cgb_update_all(void);
void cgb_sequencer(u32 ch);
void cgb_wave_step(void);
void cgb_set_freq(struct SndTrack *ch);
void cgb_update_pan(struct SndTrack *ch);
void cgb_update_volume(struct SndTrack *ch);
void psg_wave_load(u32 wave);
void player_start(u32 player, u32 song);

ASM_FUNC("asm/sound_0819B3C4/sound_voices_mix.s", void sound_voices_mix(void))

ASM_FUNC("asm/sound_0819B3C4/sound_driver_init.s", void sound_driver_init(void))

asm(".include \"asm/macros.inc\"\n"
    ".syntax unified\n"
    "\tnon_word_aligned_thumb_func_start sound_dma_flip\n"
    ".include \"asm/sound_0819B3C4/sound_dma_flip.s\"\n"
    "\tfunc_end sound_dma_flip\n"
    ".syntax divided\n\t.thumb\n");

ASM_FUNC("asm/sound_0819B3C4/sound_buffer_kick.s", void sound_buffer_kick(void))

void lfo_vol_clear_phase(struct SndTrack *ch)
{
    ch->status &= 0xCFFF;
    ch->volCounter = ch->unk16;
}

void lfo_vol_step(struct SndTrack *ch, struct LfoVolOwner *owner)
{
    if (ch->volCounter != 0)
    {
        ch->volCounter--;
        return;
    }
    ch->volCounter = ch->volReload;
    if (ch->status & 0x1000)
    {
        ch->status ^= 0x2000;
        owner->callback(ch);
    }
    else
    {
        ch->status |= 0x1000;
    }
}

s32 lfo_vol_value(struct SndTrack *ch)
{
    if (ch->status & 0x1000)
    {
        if (ch->status & 0x2000)
            return -ch->volDepth;
        return ch->volDepth;
    }
    return 0;
}

void lfo_pitch_reset(struct SndTrack *ch)
{
    ch->status &= 0xF9FF;
    ch->pitchCounter = ch->pitchDelay;
}

void lfo_pitch_step(struct SndTrack *ch, struct LfoPitchOwner *owner)
{
    if (ch->pitchCounter != 0)
    {
        ch->pitchCounter--;
        return;
    }
    ch->pitchCounter = ch->pitchReload;
    if (ch->status & 0x200)
    {
        ch->status ^= 0x400;
        owner->callback(ch);
    }
    else
    {
        ch->status |= 0x200;
    }
}

s32 lfo_pitch_value(struct SndTrack *ch)
{
    if (ch->status & 0x200)
    {
        if (ch->status & 0x400)
            return -ch->pitchDepth;
        return ch->pitchDepth;
    }
    return 0;
}

void voice_set_pitch(struct SndTrack *ch)
{
    u32 k = ch->key << 8;
    s32 f;

    k = (u16)(k + ch->bend * (ch->unkF << 1));
    f = ch->fine;
    k = (u16)(k + f);
    if (ch->status & 0x100)
        k = (u16)(k + lfo_pitch_value(ch));
    *(u16 *)(gUnk_03001B5A + (ch->voice << 4)) = k;
}

void track_note_on(struct SndTrack *ch)
{
    struct Voice *v = (struct Voice *)(gUnk_03001B50 + (ch->voice << 4));
    struct ToneEntry *tone;

    v->flags = 0;
    tone = find_tone(ch->unkC, ch->key);
    if (tone != NULL)
    {
        voice_set_pitch(ch);
        v->unk2 = tone->unk2;
        v->unkC = tone->unk4;
        v->unkD = tone->unk5;
        v->unkE = tone->unk6;
        v->unkF = tone->unk7;
        v->unk4 = 0;
        v->flags = 0x80;
    }
}

void voice_set_volume(struct SndTrack *ch)
{
    s32 panL;
    s32 panR;
    s32 vol;
    s32 scale;
    s32 left;
    s32 right;
    u8 *base;

    if ((s8)ch->unkE >= 0)
    {
        panL = 0x7F;
        panR = ch->unkE;
    }
    else
    {
        panL = (u8)(0xFF - ch->unkE);
        panR = 0x7F;
    }
    vol = ch->unkD;
    if (ch->status & 0x800)
    {
        vol += lfo_vol_value(ch);
        if (vol < 0)
            vol = 0;
        else if (vol > 0xFF)
            vol = 0xFF;
    }
    vol = (vol * ((sound_info()->volTable[ch->span] >> 8) + 1)) >> 8;
    scale = vol + 1;
    left = (panL * scale) >> 8;
    right = (panR * scale) >> 8;
    base = gUnk_03001B50;
    ((struct Voice *)(base + (ch->voice << 4)))->volLeft = left;
    ((struct Voice *)(gUnk_03001B50 + (ch->voice << 4)))->volRight = right;
}

void voice_set_flag_40(struct SndTrack *ch)
{
    u8 *base = gUnk_03001B50;
    struct Voice *v = (struct Voice *)(base + (ch->voice << 4));
    v->flags |= 0x40;
}

void voice_update(struct SndTrack *ch)
{
    voice_set_pitch(ch);
    voice_set_volume(ch);
}

struct ToneEntry *find_tone(u32 prog, s32 key)
{
    u8 *p = (u8 *)gToneProgramOffsets + gToneProgramOffsets[prog];
    u8 *end = (u8 *)gToneProgramOffsets + gToneProgramOffsets[prog + 1];

    while (p != end)
    {
        struct ToneEntry *t = (struct ToneEntry *)p;
        if (t->keyLo <= key && t->keyHi >= key)
            return t;
        p += 8;
    }
    return NULL;
}

void sound_hw_init(void)
{
    *(vu8 *)0x04000080 = 0x77;
    *(vu8 *)0x04000084 = 0x8F;
    *(vu16 *)0x04000082 = (*(vu16 *)0x04000082 & 0xFFFC) | 2;
    *(vu8 *)0x04000065 = 0x80;
    *((u8 *)sound_info() + 0x4F8) = 0xFF;
}

u32 pitch_to_psg_freq(u32 pitch)
{
    s32 idx = (pitch << 16) >> 24;
    u32 mask = 0xFF0000;
    u32 fine;
    u32 lo;
    u32 d;

    mask &= pitch << 16;
    fine = mask >> 16;
    idx -= 0x24;
    if (idx < 0)
        return 0;
    lo = gPsgFreqTable[idx];
    d = (gPsgFreqTable[idx + 1] - lo) * fine;
    return (u16)(0x800 - (lo + (d >> 8)));
}

u32 key_to_cgb_value(s32 key)
{
    if (key <= 0x14)
        return 0xD7;
    if (key > 0x50)
        return 0;
    return gKeyValueTable[key - 0x15];
}

void psg_wave_load(u32 wave)
{
    const u16 *src = (const u16 *)(gPsgWaveTable + (wave << 4));
    vu16 *dst;

    *(vu8 *)0x04000070 = 0xC0;
    dst = (vu16 *)0x04000090;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = *src;
    *(vu8 *)0x04000070 = 0x80;
}

void cgb_set_freq(struct SndTrack *ch)
{
    u32 k = ch->key << 8;
    u32 freq;
    u8 *rec;
    s32 f;

    k = (u16)(k + ch->bend * ch->unkF);
    f = ch->fine;
    k = (u16)(k + f);
    if (ch->status & 0x100)
        k = (u16)(k + lfo_pitch_value(ch));
    freq = pitch_to_psg_freq(k);
    rec = (u8 *)sound_info();
    rec += (ch->voice & 3) * 12;
    *(u16 *)(rec + 0x4D0) = freq;
}

void cgb_set_key_value(struct SndTrack *ch)
{
    u32 v = key_to_cgb_value(ch->key);
    u8 *base = (u8 *)(*(struct SoundInfo **)0x03007FF0);
    u8 w = v;
    *(u16 *)(base + 0x4F4) = w;
}

void cgb_begin_release(struct SndTrack *ch)
{
    u8 *state = (u8 *)sound_info() + ((ch->voice & 3) * 12);
    state[0x4CF] = 5;
}

void cgb_load_tone(struct SndTrack *ch)
{
    struct ToneEntry *tone = find_tone(ch->unkC, ch->key);

    if (tone != NULL)
    {
        struct SoundInfo **pp = (struct SoundInfo **)0x03007FF0;
        u32 off = 0x4C8 + ((ch->voice & 3) * 12);
        u8 *rec = (u8 *)*pp + off;
        rec[0] = tone->unk4;
        rec[1] = tone->unk5;
        rec[2] = tone->unk6;
        rec[3] = tone->unk7;
        rec[6] = tone->unk2;
        rec[7] = 1;
    }
}

void cgb_select_wave(struct SndTrack *ch)
{
    struct ToneEntry *tone = find_tone(ch->unkC, ch->key);

    if (tone != NULL)
    {
        u8 *wave = (u8 *)sound_info() + 0x4F8;
        if (*wave != tone->unk2)
        {
            *wave = tone->unk2;
            psg_wave_load(*((u8 *)sound_info() + 0x4F8));
        }
        *((u8 *)sound_info() + 0x4E7) = 1;
    }
}

void cgb_update_pan(struct SndTrack *ch)
{
    u32 bit = (0x1000000u << (ch->voice & 3)) >> 24;
    u32 old = *(vu8 *)0x04000081;
    u32 hi = bit << 4;
    u32 v = old & ~(bit | hi);
    u32 pan = ch->unkE;

    if (pan <= 0x80)
        v |= bit;
    if (pan > 0x7E)
    {
        v |= hi;
        v = (u8)v;
    }
    *(vu8 *)0x04000081 = v;
}

void cgb_update_volume(struct SndTrack *ch)
{
    struct SoundInfo *si = sound_info();
    u8 *rec = (u8 *)si + ((ch->voice & 3) * 12);
    s32 scale = (si->volTable[ch->span] >> 8) + 1;
    s32 v = (ch->unkD * scale) >> 12;

    rec[0x4CC] = v;
}

void cgb_update_track(struct SndTrack *ch)
{
    cgb_set_freq(ch);
    cgb_update_volume(ch);
    cgb_update_pan(ch);
}

u32 cgb_env_level(u32 idx)
{
    u8 *rec = (u8 *)sound_info() + idx * 12;
    u32 r3 = rec[0x4D2];
    s32 r4 = r3 >> 4;
    s32 div;

    if ((r3 & 0xF) == 0)
        return r4;
    div = r3 & 7;
    if (r3 & 8)
    {
        r4 += rec[0x4CD] / div;
        if (r4 > 0xF)
            r4 = 0xF;
    }
    else
    {
        r4 -= rec[0x4CD] / div;
        if (r4 < 0)
            r4 = 0;
    }
    return (u8)r4;
}

ASM_FUNC("asm/sound_0819B3C4/cgb_sequencer.s", void cgb_sequencer(u32 ch))

NONMATCH("asm/sound_0819B3C4/cgb_wave_step.s", void cgb_wave_step(void))
{
    u8 *rec = (u8 *)sound_info() + 0x4E0;
    u32 next;

    switch (rec[7])
    {
    case 0:
        return;
    case 1:
        *(vu16 *)0x04000074 = *(u16 *)(rec + 8) | 0x8000;
        *(vu16 *)0x04000072 = gWaveVolumeCodes[rec[4] >> 2] << 8;
        next = rec[7] + 1;
        break;
    case 2:
        *(vu16 *)0x04000074 = *(u16 *)(rec + 8);
        return;
    case 5:
        *(vu8 *)0x04000073 = 0;
        next = 0;
        break;
    default:
        return;
    }
    rec[7] = next;
}
END_NONMATCH

void cgb_update_all(void)
{
    cgb_sequencer(0);
    cgb_sequencer(1);
    cgb_wave_step();
    cgb_sequencer(3);
}

void noop_callback_0819C078(void)
{
}

ASM_FUNC("asm/sound_0819B3C4/m4a_sound_init.s", void m4a_sound_init(void))

void track_init(struct SndTrack *ch, u32 cmdTable)
{
    ch->status = 0x83;
    ch->cmdTable = cmdTable;
    ch->unk8 = 0;
    ch->unk2 = 0x78;
    ch->unk3 = 0;
    ch->unkC = 0;
    ch->unkE = 0x7F;
    ch->unkA = 1;
    ch->unkD = 0xC8;
    ch->bend = 0;
    ch->unkF = 2;
    ch->fine = 0;
}

void track_key_off(u32 idx, struct SndTrack *ch)
{
    if (ch->status & 2)
        return;
    if (ch->status & 0x80)
    {
        ((struct ReleaseEntry *)gReleaseTable[ch->voice])->callback(ch);
    }
    ch->status |= 2;
}

void player_key_off(u32 player)
{
    s32 t;
    u32 off;
    struct SndTrack *tr;
    struct SoundInfo **pp;

    t = 0;
    pp = (struct SoundInfo **)0x03007FF0;
    off = player * 0x180;
    tr = (struct SndTrack *)((u8 *)*pp + off);
    for (; t <= 11; t++, tr++)
    {
        if (tr->status & 1)
            track_key_off(t, tr);
    }
    sound_info()->playerFlags[player] &= 0xFE;
}

void sound_info_set_4a4(u32 idx, u32 value)
{
    sound_info()->playerWord[idx] = value;
}

NONMATCH("asm/sound_0819B3C4/player_start.s", void player_start(u32 player, u32 song))
{
    s32 i;
    struct SndTrack *tr;
    u16 bits;
    u16 *offs;
    u8 *base;

    player_key_off(player);
    sound_info_set_4a4(player, 0x4B);
    sound_info()->playerFlags[player] |= 1;
    sound_info()->playerFlags[player] &= 0xFD;
    offs = (u16 *)gSongTableLo[song];
    base = (u8 *)offs;
    tr = (struct SndTrack *)((u8 *)sound_info() + ((player * 3) << 7));
    bits = *offs;
    offs++;
    for (i = 11; i >= 0; i--)
    {
        if (bits & 1)
        {
            u32 data = (u32)(base + *offs);
            offs++;
            track_init(tr, data);
        }
        else
        {
            tr->status = 0;
        }
        tr++;
        bits >>= 1;
    }
}
END_NONMATCH

void player_enable(u32 player)
{
    sound_info()->playerFlags[player] |= 1;
}

NONMATCH("asm/sound_0819B3C4/player_set_volume.s", void player_set_volume(u32 player, u32 vol))
{
    u32 player2;
    u32 block;
    s32 t;
    struct SndTrack *tr;

    sound_info()->volTable[player] = (vol << 8) + vol;
    player2 = player << 1;
    if (sound_info()->playerFlags[player] & 1)
    {
        t = 0;
        block = (player2 + player) << 7;
        for (; t <= 11; t++)
        {
            tr = (struct SndTrack *)((u8 *)sound_info() + block + (t << 5));
            if (tr->status & 1)
            {
                if (!(tr->status & 2))
                    ((struct VolEntry *)gReleaseTable[t])->callback(tr);
            }
        }
    }
}
END_NONMATCH

void song_play(u32 id)
{
    u8 *song;
    u16 *offs;
    u16 bits;
    u32 cmd;
    s32 t;
    struct SndTrack *tr;

    song = gSongTableId[id];
    offs = (u16 *)song;
    bits = *offs;
    offs++;
    tr = sound_info()->players[0].tracks;
    for (t = 0; t <= 11; t++, tr++, bits >>= 1)
    {
        if (bits & 1)
        {
            cmd = (u32)(song + *offs);
            offs++;
            track_init(tr, cmd);
            sound_info()->trackSong[t] = id;
        }
    }
}

void song_stop(u32 id)
{
    s32 t;
    struct SndTrack *tr;

    for (t = 0, tr = sound_info()->players[0].tracks; t <= 11; t++, tr++)
    {
        if (tr->status & 1)
        {
            if (sound_info()->trackSong[t] == id)
            {
                track_key_off(t, tr);
                tr->status &= 0xFFFE;
            }
        }
    }
}

void stop_all_tracks(void)
{
    s32 t;
    struct SndTrack *tr;

    for (t = 0, tr = sound_info()->players[0].tracks; t <= 11; t++, tr++)
    {
        if (tr->status & 1)
        {
            track_key_off(t, tr);
            tr->status &= 0xFFFE;
        }
    }
}

ASM_FUNC("asm/sound_0819B3C4/process_cmd_queue.s", void process_cmd_queue(void))

u32 cmd_queue_push(u16 cmd)
{
    sound_info()->cmdQueue[sound_info()->cmdWrite] = cmd;
    sound_info()->cmdWrite = (sound_info()->cmdWrite + 1) & 7;
    return 0;
}

ASM_FUNC("asm/sound_0819B3C4/track_step.s", void track_step(struct SndTrack *tr, struct TrackClass *cls, u32 word))

ASM_FUNC("asm/sound_0819B3C4/players_update.s", void players_update(void))

void sound_tick(void)
{
    process_cmd_queue();
    players_update();
    cgb_update_all();
}

u32 queue_player_start(u32 player, u32 song)
{
    return cmd_queue_push((player << 8) | song);
}

u32 queue_player_key_off(u32 player)
{
    return cmd_queue_push((player << 8) | 0x1000);
}

u32 queue_player_enable(u32 player)
{
    return cmd_queue_push((player << 8) | 0x2000);
}

NONMATCH("asm/sound_0819B3C4/queue_player_volume.s", u32 queue_player_volume(u32 player, u32 vol))
{
    u32 cmd = player << 8;
    cmd |= 0xFFFF9000;
    cmd |= vol;
    return cmd_queue_push(cmd);
}
END_NONMATCH

u32 queue_song_play(u32 id)
{
    return cmd_queue_push(id | 0x5000);
}

u32 queue_song_stop(u32 id)
{
    return cmd_queue_push(id | 0x6000);
}

u32 queue_stop_all(void)
{
    return cmd_queue_push(0x7000);
}

NONMATCH("asm/sound_0819B3C4/queue_player_word.s", u32 queue_player_word(u32 player, u32 value))
{
    u32 cmd = player << 8;
    cmd |= 0xFFFF8000;
    cmd |= value;
    return cmd_queue_push(cmd);
}
END_NONMATCH

ASM_FUNC("asm/sound_0819B3C4/active_player_mask.s", void active_player_mask(void))

ASM_FUNC("asm/sound_0819B3C4/sub_0819CB90.s", void sub_0819CB90(void))
