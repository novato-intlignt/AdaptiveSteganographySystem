#ifndef WAVELET_TRANSFORM_H
#define WAVELET_TRANSFORM_H

#include <vector>
#include <wavelib.h>

class WaveletTransform {
public:
  // Descompone la señal en coeficientes de aproximación y detalle
  static wt_object decomposeSignal(const std::vector<double> &signal,
                                   int levels);

  // Reconstruye la señal a partir de coeficientes de aproximación y detalle
  static std::vector<double> reconstructSignal(wt_object &wt);
};

#endif // WAVELET_TRANSFORM_H
