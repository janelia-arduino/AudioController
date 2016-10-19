// ----------------------------------------------------------------------------
// 5x3.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef AUDIO_CONTROLLER_5X3_CONSTANTS_H
#define AUDIO_CONTROLLER_5X3_CONSTANTS_H
#include "Constants.h"


#if defined(__AVR_ATmega2560__)

namespace audio_controller
{
namespace constants
{
enum {CHANNEL_COUNT=4};

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
