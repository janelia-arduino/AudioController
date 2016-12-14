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
enum{PROPERTY_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=9};
enum{FUNCTION_COUNT_MAX=17};
enum{CALLBACK_COUNT_MAX=1};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

enum{AUDIO_EXT_COUNT=2};
enum{STRING_LENGTH_PATH=255};
enum{STRING_LENGTH_ERROR_MESSAGE=257};

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

extern const size_t sd_card_cs_pin;
extern const size_t sd_card_mosi_pin;
extern const size_t sd_card_sck_pin;

enum{EVENT_COUNT_MAX=8};
enum{INDEXED_PULSES_COUNT_MAX=4};
extern const int bad_index;

struct PulseInfo
{
  size_t frequency;
  ConstantString * speaker_ptr;
  EventIdPair event_id_pair;
};

// Interrupts

// Units
extern ConstantString ms_unit;
extern ConstantString hz_unit;

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern ConstantString volume_property_name;
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

extern ConstantString speaker_parameter_name;
enum{SPEAKER_SUBSET_LENGTH=3};
extern ConstantString speaker_all;
extern ConstantString speaker_left;
extern ConstantString speaker_right;
extern modular_server::SubsetMemberType speaker_subset[SPEAKER_SUBSET_LENGTH];

extern ConstantString delay_parameter_name;
extern const long delay_min;
extern const long delay_max;

extern ConstantString period_parameter_name;
extern const long period_min;
extern const long period_max;

extern ConstantString on_duration_parameter_name;
extern const long on_duration_min;
extern const long on_duration_max;

extern ConstantString count_parameter_name;
extern const long count_min;
extern const long count_max;

extern ConstantString pwm_index_parameter_name;

// Functions
extern ConstantString get_sd_card_info_function_name;
extern ConstantString get_audio_paths_function_name;
extern ConstantString play_path_function_name;
extern ConstantString play_tone_function_name;
extern ConstantString play_noise_function_name;
extern ConstantString stop_function_name;
extern ConstantString is_playing_function_name;
extern ConstantString get_last_audio_path_played_function_name;
extern ConstantString get_position_function_name;
extern ConstantString get_length_function_name;
extern ConstantString get_percent_complete_function_name;
extern ConstantString add_tone_pwm_function_name;
extern ConstantString start_tone_pwm_function_name;
extern ConstantString add_noise_pwm_function_name;
extern ConstantString start_noise_pwm_function_name;
extern ConstantString stop_pwm_function_name;
extern ConstantString stop_all_pwm_function_name;

// Callbacks

// Errors
extern ConstantString pwm_error;
}
}
#include "5x3.h"
#include "3x2.h"
#endif
