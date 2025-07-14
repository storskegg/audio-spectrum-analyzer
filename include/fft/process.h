//
// Created by Liam Conrad on 11/15/24.
//

#ifndef AUDIO_SPECTRUM_ANALYZER_FFT_H
#define AUDIO_SPECTRUM_ANALYZER_FFT_H

#pragma once

#include <cstdint>
#include <cmath>
#include "fft/window/hamming.h"
#include <kiss_fftr.h>

class WindowFnHamming;

class Process
{
private:
  WindowFnHamming *m_hamming_window;
  int m_fft_size;
  int m_window_size;
  kiss_fftr_cfg m_cfg;
  kiss_fft_cpx *m_fft_output;

public:
  double *m_energy;
  double *m_fft_input;

  explicit Process(int window_size);
  void update(const double *samples);
};

#endif //AUDIO_SPECTRUM_ANALYZER_FFT_H
