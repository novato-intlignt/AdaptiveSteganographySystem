#include "waveletTransform.h"
#include <cmath>
#include <stdexcept>
#include <vector>

wt_object WaveletTransform::decomposeSignal(const std::vector<double> &signal,
                                            int levels) {
  // Validar la señal
  if (signal.empty()) {
    throw std::invalid_argument("Input signal is empty.");
  }

  int maxLevels = static_cast<int>(std::log2(signal.size()));
  levels = std::min(levels, maxLevels);

  if (levels < 2) {
    throw std::invalid_argument(
        "Signal size is too small for the requested levels.");
  }

  wave_object wave = wave_init("db8");
  wt_object wt = wt_init(wave, "dwt", signal.size(), levels);

  setDWTExtension(wt, "sym");
  setWTConv(wt, "direct");

  // Realizar la descomposición
  dwt(wt, signal.data());

  // Extraer los coeficientes de detalle para cada nivel
  std::vector<std::vector<double>> allDetails;
  double t = wt->length[0];
  for (int i = 0; i < levels; i++) {
    int startIdx = wt->length[i];
    int endIdx = wt->length[i + 1];
    allDetails.emplace_back(wt->output + startIdx, wt->output + endIdx);
  }

  // Coeficientes de aproximación (último nivel)
  int approxStart = 0;
  int approxEnd = wt->length[0];
  std::vector<double> approximation(wt->output + approxStart,
                                    wt->output + approxEnd);

  wt_summary(wt);

  // Retornar coeficientes relevantes
  return wt;
}

std::vector<double> WaveletTransform::reconstructSignal(wt_object &wt) {

  // if (wt->output.empty()) {
  //   throw std::invalid_argument("Coefficients are empty.");
  // }

  // Reconstrucción
  std::vector<double> reconstructedSignal(wt->siglength, 0.0);
  idwt(wt, reconstructedSignal.data());

  wave_free(wt->wave);
  wt_free(wt);

  return reconstructedSignal;
}
