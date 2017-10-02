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
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{INTERRUPT_COUNT_MAX=1};

// Interrupts

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
