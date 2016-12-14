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
enum{INTERRUPT_COUNT_MAX=4};

// Interrupts
extern ConstantString int_a_interrupt_name;
extern const size_t int_a_pin;

extern ConstantString int_b_interrupt_name;
extern const size_t int_b_pin;

extern ConstantString int_c_interrupt_name;
extern const size_t int_c_pin;

extern ConstantString int_d_interrupt_name;
extern const size_t int_d_pin;

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
