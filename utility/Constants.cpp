// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace audio_controller
{
namespace constants
{
CONSTANT_STRING(device_name,"audio_controller");

CONSTANT_STRING(firmware_name,"AudioController");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
  {
    .name_ptr=&firmware_name,
    .version_major=2,
    .version_minor=0,
    .version_patch=1,
  };

CONSTANT_STRING(hardware_name,"audio_controller");

const char * const audio_ext_raw = ".RAW";
const char * const audio_ext_wav = ".WAV";
const char * const audio_exts[AUDIO_EXT_COUNT] = {audio_ext_raw, audio_ext_wav};

const char * const sd_prefix = "/SD/";

const int bad_index = -1;

const double allpass_filter_coefs[FILTER_COEF_COUNT] = {1.0,
                                                        0.0,
                                                        1.0,
                                                        0.0,
                                                        1.0};

const double volume_stop_threshold = 0.001;

// Interrupts

// Units
CONSTANT_STRING(ms_units,"ms");
CONSTANT_STRING(hz_units,"Hz");
CONSTANT_STRING(octaves_units,"octaves");

// Properties
CONSTANT_STRING(volume_property_name,"volume");
const double volume_min = 0.0;
const double volume_max = 1.0;
const double volume_default = 0.25;

// Parameters
CONSTANT_STRING(audio_path_parameter_name,"audio_path");

CONSTANT_STRING(percent_parameter_name,"percent");
const int percent_min = 0;
const int percent_max = 100;

CONSTANT_STRING(frequency_parameter_name,"frequency");
const int frequency_min = 200;
const int frequency_max = 22000;

CONSTANT_STRING(speaker_parameter_name,"speaker");
CONSTANT_STRING(speaker_all,"ALL");
CONSTANT_STRING(speaker_left,"LEFT");
CONSTANT_STRING(speaker_right,"RIGHT");
modular_server::SubsetMemberType speaker_subset[SPEAKER_SUBSET_LENGTH] =
  {
    {.cs_ptr=&speaker_all},
    {.cs_ptr=&speaker_left},
    {.cs_ptr=&speaker_right},
  };

CONSTANT_STRING(delay_parameter_name,"delay");
const long delay_min = 0;
const long delay_max = 2000000000;

CONSTANT_STRING(period_parameter_name,"period");
const long period_min = 2;
const long period_max = 2000000000;

CONSTANT_STRING(on_duration_parameter_name,"on_duration");
const long on_duration_min = 1;
const long on_duration_max = 2000000000;

CONSTANT_STRING(count_parameter_name,"count");
const long count_min = 1;
const long count_max = 2000000000;

CONSTANT_STRING(pwm_index_parameter_name,"pwm_index");

CONSTANT_STRING(volume_parameter_name,"volume");

CONSTANT_STRING(bandwidth_parameter_name,"bandwidth");
const double bandwidth_min = 0.001;
const double bandwidth_max = 10;

// Functions
CONSTANT_STRING(get_audio_memory_usage_function_name,"getAudioMemoryUsage");
CONSTANT_STRING(get_audio_memory_usage_max_function_name,"getAudioMemoryUsageMax");
CONSTANT_STRING(reset_audio_memory_usage_max_function_name,"resetAudioMemoryUsageMax");
CONSTANT_STRING(get_audio_processor_usage_function_name,"getAudioProcessorUsage");
CONSTANT_STRING(get_audio_processor_usage_max_function_name,"getAudioProcessorUsageMax");
CONSTANT_STRING(reset_audio_processor_usage_max_function_name,"resetAudioProcessorUsageMax");
CONSTANT_STRING(get_sd_card_info_function_name,"getSDCardInfo");
CONSTANT_STRING(get_audio_paths_function_name,"getAudioPaths");
CONSTANT_STRING(play_path_function_name,"playPath");
CONSTANT_STRING(play_tone_function_name,"playTone");
CONSTANT_STRING(play_tone_at_function_name,"playToneAt");
CONSTANT_STRING(play_noise_function_name,"playNoise");
CONSTANT_STRING(play_noise_at_function_name,"playNoiseAt");
CONSTANT_STRING(play_filtered_noise_function_name,"playFilteredNoise");
CONSTANT_STRING(play_filtered_noise_at_function_name,"playFilteredNoiseAt");
CONSTANT_STRING(stop_function_name,"stop");
CONSTANT_STRING(is_playing_function_name,"isPlaying");
CONSTANT_STRING(get_last_audio_path_played_function_name,"getLastAudioPathPlayed");
CONSTANT_STRING(get_position_function_name,"getPosition");
CONSTANT_STRING(get_length_function_name,"getLength");
CONSTANT_STRING(get_percent_complete_function_name,"getPercentComplete");
CONSTANT_STRING(add_tone_pwm_function_name,"addTonePwm");
CONSTANT_STRING(add_tone_pwm_at_function_name,"addTonePwmAt");
CONSTANT_STRING(start_tone_pwm_function_name,"startTonePwm");
CONSTANT_STRING(start_tone_pwm_at_function_name,"startTonePwmAt");
CONSTANT_STRING(add_noise_pwm_function_name,"addNoisePwm");
CONSTANT_STRING(add_noise_pwm_at_function_name,"addNoisePwmAt");
CONSTANT_STRING(start_noise_pwm_function_name,"startNoisePwm");
CONSTANT_STRING(start_noise_pwm_at_function_name,"startNoisePwmAt");
CONSTANT_STRING(add_filtered_noise_pwm_function_name,"addFilteredNoisePwm");
CONSTANT_STRING(add_filtered_noise_pwm_at_function_name,"addFilteredNoisePwmAt");
CONSTANT_STRING(start_filtered_noise_pwm_function_name,"startFilteredNoisePwm");
CONSTANT_STRING(start_filtered_noise_pwm_at_function_name,"startFilteredNoisePwmAt");
CONSTANT_STRING(stop_pwm_function_name,"stopPwm");
CONSTANT_STRING(stop_all_pwm_function_name,"stopAllPwm");
CONSTANT_STRING(is_pulsing_function_name,"isPulsing");

// Callbacks

// Errors
CONSTANT_STRING(pwm_error,"Maximum number of pwm already set, must stop one to add another.");
}
}
