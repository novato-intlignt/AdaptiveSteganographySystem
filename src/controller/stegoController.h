#ifndef STEGO_CONTROLLER_H
#define STEGO_CONTROLLER_H

#include <string>
// #include "model/AudioProcessor.h"
// #include "model/CellularAutomaton.h"
// #include "model/ChaosGenerator.h"
// #include "model/WaveletTransform.h"
// #include "model/AdaptiveSteganography.h"
// #include "model/Verifier.h"

class StegoController {
public:
  StegoController();

  bool startEmbedding(const std::string &videoFile,
                      const std::string &imageFile);
  void retrieveData(const std::string &videoFile);
  void verifyResults();

private:
  // AudioProcessor audioProcessor;
  // CellularAutomaton cellularAutomaton;
  // ChaosGenerator chaosGenerator;
  // WaveletTransform waveletTransform;
  // AdaptiveSteganography steganography;
  // Verifier verifier;
};

#endif // STEGO_CONTROLLER_H
