// ----------------------------------------------------------------------------
// 5x3.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "5x3.h"


#if defined(__MK64FX512__)

namespace audio_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=1150,
  .version_major=1,
  .version_minor=0,
};

const int pcb_speaker_reference = INTERNAL;
const double pcb_speaker_channel_gain = 0.5;
const double pcb_speaker_pre_gain = 0.5;
const size_t pcb_speaker_enable_pin = 6;

const bool sd_exists = false;
const char * const sd_prefix = "/SD/";
const size_t sd_card_cs_pin = 10;
const size_t sd_card_mosi_pin = 7;
const size_t sd_card_sck_pin = 14;

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
