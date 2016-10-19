// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_CONSTANTS_H
#define AUDIO_CONTROLLER_CONSTANTS_H
#include "ConstantVariable.h"
#include "ModularServer.h"

#include "EventController.h"


namespace audio_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{FIELD_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=4};
enum{METHOD_COUNT_MAX=11};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

enum{EVENT_COUNT_MAX=8};
enum{INDEXED_PULSES_COUNT_MAX=4};
extern const int bad_index;

struct PulseInfo
{
  uint32_t channels;
  Polarity polarity;
  EventIdPair event_id_pair;
};

extern const char * const audio_ext_raw;
extern const char * const audio_ext_wav;
extern const char * const audio_exts[AUDIO_EXT_COUNT];
enum audio_t
  {
    RAW_TYPE,
    WAV_TYPE,
    TONE_TYPE,
    NOISE_TYPE,
  };

extern const char * const sd_prefix;

// Units
extern ConstantString ms_unit;
extern ConstantString hz_unit;

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[], ConstantString *
extern ConstantString volume_field_name;
extern const double volume_min;
extern const double volume_max;
extern const double volume_default;

// Parameters
extern ConstantString audio_path_parameter_name;

extern ConstantString percent_parameter_name;
extern const int percent_min;
extern const int percent_max;

extern ConstantString frequency_parameter_name;
extern const int frequency_min;
extern const int frequency_max;

extern ConstantString speaker_field_name;
enum{SPEAKER_SUBSET_LENGTH=3};
extern ConstantString speaker_all;
extern ConstantString speaker_left;
extern ConstantString speaker_right;
extern const modular_server::SubsetMemberType speaker_ptr_subset[SPEAKER_SUBSET_LENGTH];
extern const ConstantString * const speaker_ptr_default;

// Methods
extern ConstantString get_sd_card_info_method_name;
extern ConstantString get_audio_paths_method_name;
extern ConstantString play_path_method_name;
extern ConstantString play_tone_method_name;
extern ConstantString play_noise_method_name;
extern ConstantString stop_method_name;
extern ConstantString is_playing_method_name;
extern ConstantString get_last_audio_path_played_method_name;
extern ConstantString get_position_method_name;
extern ConstantString get_length_method_name;
extern ConstantString get_percent_complete_method_name;

// Errors
}
}
#include "5x3.h"
#include "3x2.h"
#endif
