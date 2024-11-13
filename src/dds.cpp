//
// Created by Liam Conrad on 11/13/24.
//
#include "dds.h"

void dds_on(AD9833* ad) {
  ad->setWave(ON);
};

void dds_off(AD9833* ad) {
  ad->setWave(OFF);
};

float dds_set_frequency(float freq, AD9833* ad) {
  return ad->setFrequency(freq, 0); // 0 is the channel index
}
