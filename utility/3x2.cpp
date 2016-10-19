// ----------------------------------------------------------------------------
// 3x2.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "3x2.h"


#if defined(__MK20DX256__)

namespace audio_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
  {
    .name_ptr=&hardware_name,
    .part_number=1171,
    .version_major=1,
    .version_minor=0,
  };

const size_t enable_pins[CHANNEL_COUNT] = {8,9};
const size_t dir_a_pins[CHANNEL_COUNT] = {30,32};
const size_t dir_b_pins[CHANNEL_COUNT] = {31,33};
const size_t sense_pins[CHANNEL_COUNT] = {0,1};

// Units

// Fields
const bool polarity_reversed_default[CHANNEL_COUNT] = {false,false};

const bool channels_enabled_default[CHANNEL_COUNT] = {true,true};

// Parameters

// Methods

// Errors
}
}
#endif
