//
// Created by Liam Conrad on 11/13/24.
//

#ifndef AUDIO_SPECTRUM_ANALYZER_DDS_H
#define AUDIO_SPECTRUM_ANALYZER_DDS_H

#include <SPI.h>
#include <AD9833.h>

enum Waveform {
  OFF = AD9833_OFF,
  ON = AD9833_SINE,
};

// CS_AD9833 is the chip select pin for the AD9833 SPI device
const int CS_AD9833 = D7;

static AD9833 AD(CS_AD9833, &SPI);

void dds_on(AD9833* ad = &AD);
void dds_off(AD9833* ad = &AD);

float dds_set_frequency(float freq, AD9833* ad = &AD);

#endif //AUDIO_SPECTRUM_ANALYZER_DDS_H
