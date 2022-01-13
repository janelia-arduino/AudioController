// ----------------------------------------------------------------------------
// TEENSY40.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "TEENSY40.h"


#if defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41)
namespace audio_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=1156,
  .version_major=1,
  .version_minor=0,
};

// Pins

// Units

// Properties

// Parameters

// Functions

// Callbacks

// Errors
}
}

#endif
