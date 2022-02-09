// ----------------------------------------------------------------------------
// AudioApparatus.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../AudioApparatus.h"


namespace audio_apparatus
{
// GUItool: begin automatically generated code
AudioSynthNoiseWhite     g_noise_left;   //xy=85,251
AudioSynthNoiseWhite     g_noise_right;  //xy=89,288
AudioSynthWaveformSine   g_tone_left;    //xy=250,176
AudioSynthWaveformSine   g_tone_right;   //xy=256,215
#if !defined(__IMXRT1062__)
AudioPlaySdWav           g_play_sd_wav;  //xy=263,102
#endif
AudioFilterBiquad        g_biquad_left;  //xy=263,251
#if !defined(__IMXRT1062__)
AudioPlaySdRaw           g_play_sd_raw;  //xy=264,139
#endif
AudioFilterBiquad        g_biquad_right; //xy=265,289
AudioMixer4              g_mixer_left;   //xy=500,111
AudioMixer4              g_mixer_right;  //xy=506,187
#if !defined(__IMXRT1062__)
AudioMixer4              g_mixer_dac;    //xy=702,188
#endif
AudioOutputI2S           g_stereo_speaker; //xy=717,126
#if !defined(__IMXRT1062__)
AudioOutputAnalog        g_pcb_speaker;  //xy=890,188
#endif
AudioConnection          patchCord1(g_noise_left, g_biquad_left);
AudioConnection          patchCord2(g_noise_right, g_biquad_right);
AudioConnection          patchCord3(g_tone_left, 0, g_mixer_left, 2);
AudioConnection          patchCord4(g_tone_right, 0, g_mixer_right, 2);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord5(g_play_sd_wav, 0, g_mixer_left, 0);
AudioConnection          patchCord6(g_play_sd_wav, 1, g_mixer_right, 0);
#endif
AudioConnection          patchCord7(g_biquad_left, 0, g_mixer_left, 3);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord8(g_play_sd_raw, 0, g_mixer_left, 1);
AudioConnection          patchCord9(g_play_sd_raw, 0, g_mixer_right, 1);
#endif
AudioConnection          patchCord10(g_biquad_right, 0, g_mixer_right, 3);
AudioConnection          patchCord11(g_mixer_left, 0, g_stereo_speaker, 0);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord12(g_mixer_left, 0, g_mixer_dac, 0);
#endif
AudioConnection          patchCord13(g_mixer_right, 0, g_stereo_speaker, 1);
#if !defined(__IMXRT1062__)
AudioConnection          patchCord14(g_mixer_right, 0, g_mixer_dac, 1);
AudioConnection          patchCord15(g_mixer_dac, g_pcb_speaker);
#endif
AudioControlSGTL5000     g_sgtl5000;     //xy=498,36
// GUItool: end automatically generated code
}
