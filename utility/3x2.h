// ----------------------------------------------------------------------------
// 3x2.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_3X2_CONSTANTS_H
#define AUDIO_CONTROLLER_3X2_CONSTANTS_H
#include "Constants.h"


#if defined(__MK20DX256__)

namespace audio_controller
{
namespace constants
{
enum {CHANNEL_COUNT=2};

extern const size_t enable_pins[CHANNEL_COUNT];
extern const size_t dir_a_pins[CHANNEL_COUNT];
extern const size_t dir_b_pins[CHANNEL_COUNT];
extern const size_t sense_pins[CHANNEL_COUNT];

// Units

// Fields
// Field values must be long, double, bool, long[], double[], bool[], char[], ConstantString *
extern const bool polarity_reversed_default[CHANNEL_COUNT];

extern const bool channels_enabled_default[CHANNEL_COUNT];

// Parameters

// Methods

// Errors
}
}
#endif
#endif
