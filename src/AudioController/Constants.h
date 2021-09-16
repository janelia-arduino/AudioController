// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_CONSTANTS_H
#define AUDIO_CONTROLLER_CONSTANTS_H
#include <ConstantVariable.h>
#include <ModularServer.h>

#include <EventController.h>


namespace audio_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=4};
enum{PARAMETER_COUNT_MAX=10};
enum{FUNCTION_COUNT_MAX=36};
enum{CALLBACK_COUNT_MAX=1};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

enum{AUDIO_EXT_COUNT=2};
enum{STRING_LENGTH_PATH=255};
enum{STRING_LENGTH_ERROR_MESSAGE=257};

enum{AUDIO_MEMORY_BLOCK_COUNT=50};

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

extern const size_t pcb_speaker_enable_pin;

extern const char * const sd_prefix;

extern const bool sd_exists;
extern const size_t sd_card_cs_pin;
extern const size_t sd_card_mosi_pin;
extern const size_t sd_card_sck_pin;

enum{EVENT_COUNT_MAX=8};
enum{INDEXED_PULSES_COUNT_MAX=4};

extern const int bad_index;

struct PulseInfo
{
  size_t frequency;
  double bandwidth;
  const ConstantString * speaker_ptr;
  long volume;
  EventIdPair event_id_pair;
};

enum {FILTER_STAGE_0=0};
enum {FILTER_STAGE_1=1};
enum {FILTER_STAGE_2=2};
enum {FILTER_STAGE_3=3};
enum {FILTER_COEF_COUNT=5};
extern const double allpass_filter_coefs[FILTER_COEF_COUNT];

extern const size_t frequency_stop_threshold;

extern const int pcb_speaker_reference;
extern const double pcb_speaker_channel_gain;
extern const double pcb_speaker_pre_gain;

// Pins

// Units
extern ConstantString ms_units;
extern ConstantString hz_units;
extern ConstantString octaves_units;
extern ConstantString percent_units;

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern ConstantString volume_property_name;
extern const long volume_min;
extern const long volume_max;
extern const long volume_default;

extern ConstantString stereo_speaker_gain_property_name;
extern const double gain_min;
extern const double gain_max;
extern const double stereo_speaker_gain_default;

extern ConstantString pcb_speaker_gain_property_name;
extern const double pcb_speaker_gain_default;

extern ConstantString playing_signal_property_name;
enum{PLAYING_SIGNAL_SUBSET_LENGTH=3};
extern ConstantString playing_signal_bnc_a;
extern ConstantString playing_signal_bnc_b;
extern ConstantString playing_signal_both;
extern modular_server::SubsetMemberType playing_signal_subset[PLAYING_SIGNAL_SUBSET_LENGTH];
extern const ConstantString * const playing_signal_ptr_default;

// Parameters
extern ConstantString audio_path_parameter_name;

extern ConstantString frequency_parameter_name;
extern const long frequency_min;
extern const long frequency_max;

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

extern ConstantString volume_parameter_name;

extern ConstantString bandwidth_parameter_name;
extern const double bandwidth_min;
extern const double bandwidth_max;

// Functions
extern ConstantString get_audio_memory_usage_function_name;
extern ConstantString get_audio_memory_usage_max_function_name;
extern ConstantString reset_audio_memory_usage_max_function_name;
extern ConstantString get_audio_processor_usage_function_name;
extern ConstantString get_audio_processor_usage_max_function_name;
extern ConstantString reset_audio_processor_usage_max_function_name;
extern ConstantString get_sd_card_info_function_name;
extern ConstantString get_audio_paths_function_name;
extern ConstantString play_path_function_name;
extern ConstantString play_tone_function_name;
extern ConstantString play_tone_at_function_name;
extern ConstantString play_noise_function_name;
extern ConstantString play_noise_at_function_name;
extern ConstantString play_filtered_noise_function_name;
extern ConstantString play_filtered_noise_at_function_name;
extern ConstantString stop_function_name;
extern ConstantString is_playing_function_name;
extern ConstantString get_last_audio_path_played_function_name;
extern ConstantString get_position_function_name;
extern ConstantString get_length_function_name;
extern ConstantString get_percent_complete_function_name;
extern ConstantString add_tone_pwm_function_name;
extern ConstantString add_tone_pwm_at_function_name;
extern ConstantString start_tone_pwm_function_name;
extern ConstantString start_tone_pwm_at_function_name;
extern ConstantString add_noise_pwm_function_name;
extern ConstantString add_noise_pwm_at_function_name;
extern ConstantString start_noise_pwm_function_name;
extern ConstantString start_noise_pwm_at_function_name;
extern ConstantString add_filtered_noise_pwm_function_name;
extern ConstantString add_filtered_noise_pwm_at_function_name;
extern ConstantString start_filtered_noise_pwm_function_name;
extern ConstantString start_filtered_noise_pwm_at_function_name;
extern ConstantString stop_pwm_function_name;
extern ConstantString stop_all_pwm_function_name;
extern ConstantString is_pulsing_function_name;

// Callbacks

// Errors
extern ConstantString pwm_error;
}
}
#include "5x3.h"
#include "3x2.h"
#endif
