//
// Created by Liam Conrad on 11/13/24.
//

#ifndef AUDIO_SPECTRUM_ANALYZER_MAIN_H
#define AUDIO_SPECTRUM_ANALYZER_MAIN_H

/*******************************************************
 * Imports
 */
#include <Arduino.h>
#include <Arduino_GigaDisplay.h>
#include <Arduino_GigaDisplay_GFX.h>
#include <Arduino_GigaDisplayTouch.h>
#include <Arduino_AdvancedAnalog.h>
#include <SPI.h>

#include "mbed_version.h"

#include "binning.h"
#include "dds.h"

/*******************************************************
 * Generic Settings
 */
unsigned long serial_wait_time = 4000;

/*******************************************************
 * Giga Display Shield Items
 */
GigaDisplayRGB rgb;
Arduino_GigaDisplayTouch touch;
GigaDisplay_GFX display;

#define WHITE 0xffff
#define BLACK 0x0000

#define screen_size_y 480
#define screen_size_x 800

int touch_x;
int touch_y;

unsigned long touch_last;
unsigned long touch_threshold = 650; // millis

bool touch_switch_dds_toggle;

void touch_toggle(uint8_t contacts, GDTpoint_t* points);
void set_screen_on();
void set_screen_off();


/*******************************************************
 * Binning related shit
 */
boundaries_t b = {
  .f_min =  10,
  .f_max =  440
};
const size_t num_bins = 21;
//size_t max_bins = 4001;
double* fourier_bin_frequencies = nullptr;

/*******************************************************
 * Serial printing formatted string
 */
char* s = (char*) malloc(sizeof(char) * 256);

/*******************************************************
 * ADC related shit
 */
const double bit_resolution = 3.3/65536;
uint64_t iter = 0;
unsigned long start = 0; // microseconds
unsigned long end = 0;   // microseconds

AdvancedADC adc(A7);

/*******************************************************
 * DDS Function shit
 */
int dds_state = OFF;
float dds_freq = 1000.0; // Hz
unsigned long dds_last_toggle = 0; // millis

#endif //AUDIO_SPECTRUM_ANALYZER_MAIN_H
