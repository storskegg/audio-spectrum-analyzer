//
// Created by Liam Conrad on 11/13/24.
//

#pragma once

#ifndef AUDIO_SPECTRUM_ANALYZER_BINNING_H
#define AUDIO_SPECTRUM_ANALYZER_BINNING_H

#include <cstdlib>
#include <cmath>

struct boundaries_t {
  double f_min;
  double f_max;
};

double* generate_bins(const boundaries_t* bt, int precision, size_t n);
double to_precision(double n, int places);
double bin_frequency(size_t idx, const boundaries_t* bt, size_t n);
double g(const boundaries_t* bt);

#endif //AUDIO_SPECTRUM_ANALYZER_BINNING_H
