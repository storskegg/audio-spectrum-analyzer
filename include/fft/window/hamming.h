//
// Created by Liam Conrad on 11/15/24.
//

#ifndef AUDIO_SPECTRUM_ANALYZER_HAMMING_H
#define AUDIO_SPECTRUM_ANALYZER_HAMMING_H

#include <cinttypes>

class WindowFnHamming
{
private:
  double *m_coefficients;
  int m_window_size;

public:
  WindowFnHamming(int window_size);
  ~WindowFnHamming();
  void applyWindow(double *input);
};

#endif //AUDIO_SPECTRUM_ANALYZER_HAMMING_H
