#include <Arduino.h>
#include <stdlib.h>
#include "mbed_config.h"

struct boundaries_t {
  double f_min;
  double f_max;
};

boundaries_t b = {10, 100000};
size_t num_bins = 4001;
size_t max_bins = 4001;
double* fourier_bin_frequencies = NULL;

char* s = (char*) malloc(sizeof(char) * 256);

unsigned long start;
unsigned long end;
unsigned long iter = 0;

double* generate_bins(const boundaries_t* b, int precision, size_t num_bins);
double to_precision(double n, int places);
double bin_frequency(int idx, const boundaries_t* b, int num_bins);
double g(const boundaries_t* b);

void setup() {
  Serial.begin(2000000);

  while (!Serial) {
    delay(10);
  }
}

void loop() {
  iter++;

  start = micros();

  fourier_bin_frequencies = generate_bins(&b, 1, num_bins);
  end = micros();

  for (int i = 0; i < int(num_bins); i++) {
    sprintf(s, "[%6d] %.1f", i+1, fourier_bin_frequencies[i]);
    Serial.println(s);
  }

  sprintf(s, "[%6lu] It took %lu micros to calculate logarithmic Fourier frequencies for %d bins.", iter, end - start, int(num_bins));
  Serial.println(s);

  free(fourier_bin_frequencies);

  delay(60000); // min
}

double g(const boundaries_t* b) {
  return log(b->f_max) - log(b->f_min);
}

double bin_frequency(int idx, const boundaries_t* b, int num_bins) {
  return b->f_min * exp((double(idx) * g(b))/double(num_bins-1));
}

double to_precision(double n, int places) {
  if (places == 0) {
    return n;
  }
  float p = pow(10, places);
  return round(n * p) / p;
}

double* generate_bins(const boundaries_t* b, int precision, size_t num_bins) {
  // At some point, disallow even number of bins
  if (num_bins % 2 == 0) {
    return NULL;
  }

  double* v = new double[num_bins];

  for (size_t i = 0; i < num_bins; i++) {
    v[i] = to_precision(bin_frequency(i, b, num_bins), precision);
  }

  return v;
}

