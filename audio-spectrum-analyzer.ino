#include <Arduino.h>
#include <stdlib.h>
#include "mbed_config.h"

#if !MBED_CONF_RTOS_PRESENT
#error NO MBED BOO
#endif

struct boundaries_t {
  double f_min;
  double f_max;
};

boundaries_t b = {20, 4000};
size_t num_bins = 1001;

char* s = (char*) malloc(sizeof(char) * 256);

unsigned long start;
unsigned long end;
unsigned long iter = 0;

void setup() {
  Serial.begin(2000000);

  while (!Serial) {
    delay(10);
  }
}

void loop() {
  iter++;

  start = micros();

  double* ffs = generate_bins(&b, 1, num_bins);
  end = micros();

  for (int i = 0; i < int(num_bins); i++) {
    sprintf(s, "[%6d] %.1f", i+1, ffs[i]);
    Serial.println(s);
  }

  sprintf(s, "[%6ld] It took %ld micros to calculate logarithmic Fourier frequencies for %d bins.", iter, end - start, int(num_bins));
  Serial.println(s);

  free(ffs);

  delay(60000); // min
}

double g(boundaries_t* b) {
  return log(b->f_max) - log(b->f_min);
}

double bin_frequency(int idx, boundaries_t* b, int num_bins) {
  return b->f_min * exp((double(idx) * g(b))/double(num_bins-1));
}

double to_precision(double n, int places) {
  if (places == 0) {
    return n;
  }
  float p = pow(10, places);
  return round(n * p) / p;
}

double* generate_bins(boundaries_t* b, int precision, size_t num_bins) {
  // At some point, disallow even number of bins
  if (num_bins % 2 == 0) {
    return NULL;
  }

  double* v = (double*) malloc(num_bins * sizeof(double));

  for (size_t i = 0; i < num_bins; i++) {
    v[i] = to_precision(bin_frequency(i, b, num_bins), precision);
  }

  return v;
}
