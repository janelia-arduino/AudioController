// ----------------------------------------------------------------------------
// SDInterface.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "SDInterface.h"


using namespace audio_controller;

SDInterface::SDInterface()
{
  detected_ = false;
  type_ = String("Unknown");
  formatted_ = false;
  initialized_ = false;
  format_ = String("Unknown");
  volume_size_ = 0.0;
}

void SDInterface::setup()
{
  if (!constants::sd_exists)
  {
    return;
  }
  SPI.setMOSI(constants::sd_card_mosi_pin);
  SPI.setSCK(constants::sd_card_sck_pin);

  Sd2Card card;
  SdVolume volume;
  SdFile root;

  if (card.init(SPI_HALF_SPEED,constants::sd_card_cs_pin))
  {
    detected_ = true;
  }
  else
  {
    return;
  }

  switch (card.type())
  {
    case SD_CARD_TYPE_SD1:
      type_ = String("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      type_ = String("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      type_ = String("SDHC");
      break;
    default:
      type_ = String("Unknown");
  }

  if (volume.init(card))
  {
    formatted_ = true;
  }
  else
  {
    return;
  }

  format_ = String("FAT") + String(volume.fatType(),DEC);

  double volume_size;
  volume_size = volume.blocksPerCluster(); // clusters are collections of blocks
  volume_size *= volume.clusterCount(); // we'll have a lot of clusters
  volume_size *= 512; // SD card blocks are always 512 bytes
  volume_size /= 1024; //Kbytes
  volume_size /= 1024; //Mbytes
  volume_size /= 1024; //Gbytes
  volume_size_ = round(volume_size);

  if (SD.begin(constants::sd_card_cs_pin))
  {
    initialized_ = true;
  }
}

bool SDInterface::getDetected()
{
  return detected_;
}

String SDInterface::getType()
{
  return type_;
}

bool SDInterface::getFormatted()
{
  return formatted_;
}

String SDInterface::getFormat()
{
  return format_;
}

int SDInterface::getVolumeSize()
{
  return volume_size_;
}

bool SDInterface::getInitialized()
{
  return initialized_;
}

