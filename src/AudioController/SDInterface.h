// ----------------------------------------------------------------------------
// SDInterface.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef _SD_INTERFACE_H_
#define _SD_INTERFACE_H_
#include <SPI.h>
#include <SD.h>

#include "Constants.h"


class SDInterface
{
public:
  SDInterface();
  void setup();
  bool getDetected();
  String getType();
  bool getFormatted();
  String getFormat();
  int getVolumeSize();
  bool getInitialized();
private:
  bool detected_;
  String type_;
  bool formatted_;
  bool initialized_;
  String format_;
  int volume_size_;
};

#endif
