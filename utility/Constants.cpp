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
    .version_major=1,
    .version_minor=0,
    .version_patch=0,
  };

CONSTANT_STRING(hardware_name,"audio_controller");

const int bad_index = -1;

const char * const audio_ext_raw = ".RAW";
const char * const audio_ext_wav = ".WAV";
const char * const audio_exts[AUDIO_EXT_COUNT] = {audio_ext_raw, audio_ext_wav};

const char * const sd_prefix = "/SD/";

// Units
CONSTANT_STRING(ms_unit,"ms");
CONSTANT_STRING(hz_unit,"Hz");

// Fields
CONSTANT_STRING(volume_field_name,"volume");
const double volume_min = 0.0;
const double volume_max = 1.0;
const double volume_default = 0.25;

// Parameters
CONSTANT_STRING(audio_path_parameter_name,"audio_path");

CONSTANT_STRING(percent_parameter_name,"percent");
const int percent_min = 0;
const int percent_max = 100;

CONSTANT_STRING(frequency_parameter_name,"frequency");
const int frequency_min = 0;
const int frequency_max = 22000;

CONSTANT_STRING(speaker_parameter_name,"speaker");
CONSTANT_STRING(speaker_all,"ALL");
CONSTANT_STRING(speaker_left,"LEFT");
CONSTANT_STRING(speaker_right,"RIGHT");
modular_server::SubsetMemberType speaker_str_subset[SPEAKER_SUBSET_LENGTH] =
  {
    {.cs_ptr=&speaker_all},
    {.cs_ptr=&speaker_left},
    {.cs_ptr=&speaker_right},
  };

// Methods
CONSTANT_STRING(get_sd_card_info_method_name,"getSDCardInfo");
CONSTANT_STRING(get_audio_paths_method_name,"getAudioPaths");
CONSTANT_STRING(play_path_method_name,"playPath");
CONSTANT_STRING(play_tone_method_name,"playTone");
CONSTANT_STRING(play_noise_method_name,"playNoise");
CONSTANT_STRING(stop_method_name,"stop");
CONSTANT_STRING(is_playing_method_name,"isPlaying");
CONSTANT_STRING(get_last_audio_path_played_method_name,"getLastAudioPathPlayed");
CONSTANT_STRING(get_position_method_name,"getPosition");
CONSTANT_STRING(get_length_method_name,"getLength");
CONSTANT_STRING(get_percent_complete_method_name,"getPercentComplete");

// Errors
}
}
