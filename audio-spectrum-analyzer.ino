#include <Arduino.h>
#include <stdlib.h>

struct boundaries_t {
  double f_min;
  double f_max;
};


boundaries_t b = {40, 4000};
size_t num_bins = 21;

char* s;

unsigned long start;
unsigned long end;

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  start = millis();

  double* ffs = generate_bins(&b, 2, num_bins);
  // if (ffs == NULL) {
  //   Serial.println("ERR: could not create bins");
  //   while (true) {
  //     delay(10);
  //   }
  // }

  end = millis();

  char* s = (char*) malloc(sizeof(char) * 256);
  sprintf(s, "It took %ld millis to calculate logarithmic Fourier frequencies for %d bins.", end - start, int(num_bins));
  Serial.println(s);

  for (size_t i = 0; i < num_bins; i++) {
    sprintf(s, "[%2d] %8.2f", int(i), ffs[i]);
    Serial.println(s);
  }

  free(ffs);
  free(s);
}

void loop() {
  // put your main code here, to run repeatedly:

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

