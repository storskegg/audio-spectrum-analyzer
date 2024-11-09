#include <Arduino.h>
#include <cstdlib>
//#include "mbed_config.h"

struct boundaries_t {
  double f_min;
  double f_max;
};

boundaries_t b = {10, 100000};
size_t num_bins = 4001;
//size_t max_bins = 4001;
double* fourier_bin_frequencies = nullptr;

char* s = (char*) malloc(sizeof(char) * 256);

unsigned long start;
unsigned long end;
unsigned long iter = 0;

double* generate_bins(const boundaries_t* bt, int precision, size_t n);
double to_precision(double n, int places);
double bin_frequency(size_t idx, const boundaries_t* bt, size_t n);
double g(const boundaries_t* bt);

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

// g accepts the max and min fourier frequencies, and returns the difference between the logarithms of each.
// The naming of this comes from the academic article "Improved spectrum estimation from..." cited in the references
// section of README.md.
double g(const boundaries_t* bt) {
  return log(bt->f_max) - log(bt->f_min);
}

// bin_frequency is an implementation for determining the fourier frequency of a given bin on a logarithmic frequency
// axis. This method was found in the academic article "Improved spectrum estimation from..." cited in the references
// section of README.md.
double bin_frequency(size_t idx, const boundaries_t* bt, size_t n) {
  return bt->f_min * exp((double(idx) * g(bt))/double(n-1));
}

// to_precision rounds a number to the specified number of decimal places.
double to_precision(double n, int places) {
  if (places == 0) {
    return n;
  }
  double p = pow(10, places);
  return round(n * p) / p;
}

// generate_bins generates an array of logarithmic frequencies for the given boundaries and number of bins.
double* generate_bins(const boundaries_t* bt, int precision, size_t n) {
  // At some point, disallow even number of bins
  if (n % 2 == 0) {
    return nullptr;
  }

  auto v = new double[n];

  for (size_t i = 0; i < n; i++) {
    v[i] = to_precision(bin_frequency(i, bt, n), precision);
  }

  return v;
}

