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
    .part_number=1151,
    .version_major=1,
    .version_minor=0,
  };

const bool sd_exists = false;
const size_t sd_card_cs_pin = 10;
const size_t sd_card_mosi_pin = 7;
const size_t sd_card_sck_pin = 14;

// Interrupts
CONSTANT_STRING(int_a_interrupt_name,"int_a");
const size_t int_a_pin = 2;

CONSTANT_STRING(int_b_interrupt_name,"int_b");
const size_t int_b_pin = 3;

CONSTANT_STRING(int_c_interrupt_name,"int_c");
const size_t int_c_pin = 4;

CONSTANT_STRING(int_d_interrupt_name,"int_d");
const size_t int_d_pin = 5;

// Units

// Properties

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
