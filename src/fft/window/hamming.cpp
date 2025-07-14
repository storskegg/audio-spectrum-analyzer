//
// Created by Liam Conrad on 11/15/24.
//

#include <cstdlib>
#include <cmath>
#include "fft/window/hamming.h"

WindowFnHamming::WindowFnHamming(int window_size)
{
  m_window_size = window_size;
  m_coefficients = static_cast<double *>(malloc(sizeof(double) * m_window_size));
  // create the constants for a hamming window
  const double arg = M_PI * 2.0 / window_size;
  for (int i = 0; i < window_size; i++)
  {
    double f_val = 0.5 - (0.5 * cos(arg * (i + 0.5)));
    // Scale it to fixed point and round it.
    m_coefficients[i] = f_val;
  }
}

WindowFnHamming::~WindowFnHamming()
{
  free(m_coefficients);
}

void WindowFnHamming::applyWindow(double *input)
{
  for (int i = 0; i < m_window_size; i++)
  {
    input[i] = input[i] * m_coefficients[i];
  }
}
