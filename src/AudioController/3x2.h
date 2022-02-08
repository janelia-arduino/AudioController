// ----------------------------------------------------------------------------
// 3x2.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_3X2_CONSTANTS_H
#define AUDIO_CONTROLLER_3X2_CONSTANTS_H
#include "Constants.h"


#if defined(__MK20DX256__)

namespace audio_apparatus
{
namespace constants
{
extern const int pcb_speaker_reference;
extern const double pcb_speaker_channel_gain;
extern const double pcb_speaker_pre_gain;
extern const size_t pcb_speaker_enable_pin;

extern const bool sd_exists;
extern const char * const sd_prefix;
extern const size_t sd_card_cs_pin;
extern const size_t sd_card_mosi_pin;
extern const size_t sd_card_sck_pin;

}
}

namespace audio_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PIN_COUNT_MAX=1};

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
#endif
