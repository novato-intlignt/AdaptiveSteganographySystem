#include "audioProcessor.h"
#include "LSBHandler.h"
#include "chaosGenerator.h"
#include "waveletTransform.h"
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

void AudioProcessor::embedDataInAudio3(
    std::vector<double> &audioData,
    const std::vector<unsigned char> &dataToEmbed) {
  int levels = 4; // Número de niveles de descomposición

  // Descomponer el canal usando WaveletTransform
  auto wtObj = WaveletTransform::decomposeSignal(audioData, levels);

  // Definir los índices para los niveles de detalle de interés
  int levelNStart = wtObj->length[1];
  int levelNEnd = wtObj->length[2];
  int levelN2Start = wtObj->length[2];
  int levelN2End = wtObj->length[3];

  size_t dataIndex = 0; // Índice para el vector de datos a incrustar

  // Incrustar en el nivel N (último nivel de detalle)
  LSBHandler::embedData(wtObj->output, levelNStart, levelNEnd, dataToEmbed,
                        dataIndex, /*scaleFactor=*/1e6);

  // Incrustar en el nivel N-1 (penúltimo nivel de detalle)
  LSBHandler::embedData(wtObj->output, levelN2Start, levelN2End, dataToEmbed,
                        dataIndex, /*scaleFactor=*/1e6);

  // Reconstruir el canal usando WaveletTransform
  audioData = WaveletTransform::reconstructSignal(wtObj);
}

std::tuple<double, double> AudioProcessor::embedDataInAudio(
    std::vector<double> &audioData,
    const std::vector<unsigned char> &dataToEmbed) {
  int levels = 4; // Número de niveles de descomposición

  // Descomponer el canal usando WaveletTransform
  auto wtObj = WaveletTransform::decomposeSignal(audioData, levels);

  // Definir los índices para los niveles de detalle de interés
  int levelNStart = wtObj->length[1];
  int levelNEnd = wtObj->length[2];
  int levelN2Start = wtObj->length[2];
  int levelN2End = wtObj->length[3];

  size_t dataIndex = 0; // Índice para el vector de datos a incrustar
  auto [sequence, seed, r] =
      ChaosGenerator::generateRandomSequence(levelNStart, levelN2End);

  // Incrustar en el nivel N (último nivel de detalle)
  LSBHandler::embedData(wtObj->output, levelNStart, levelNEnd, dataToEmbed,
                        dataIndex, /*scaleFactor=*/1e6);

  // Incrustar en el nivel N-1 (penúltimo nivel de detalle)
  LSBHandler::embedData(wtObj->output, levelN2Start, levelN2End, dataToEmbed,
                        dataIndex, /*scaleFactor=*/1e6);

  // Reconstruir el canal usando WaveletTransform
  audioData = WaveletTransform::reconstructSignal(wtObj);
  return {seed, r};
}
std::vector<unsigned char>
AudioProcessor::recoverDataFromAudio(const std::vector<double> &audioData,
                                     size_t dataSize, double seed, double r) {
  int levels = 4; // Número de niveles de descomposición
  std::vector<unsigned char> recoveredData;

  // Descomponer el canal usando WaveletTransform
  auto wtObj = WaveletTransform::decomposeSignal(audioData, levels);

  // Definir los índices para los niveles de detalle de interés
  int levelNStart = wtObj->length[1];
  int levelNEnd = wtObj->length[2];
  int levelN2Start = wtObj->length[2];
  int levelN2End = wtObj->length[3];

  size_t remainingData = dataSize - recoveredData.size();

  // Recuperar datos del nivel N (último nivel de detalle)
  auto dataFromN = LSBHandler::extractData(wtObj->output, levelNStart,
                                           levelNEnd, remainingData,
                                           /*scaleFactor=*/1e6);
  recoveredData.insert(recoveredData.end(), dataFromN.begin(), dataFromN.end());

  // Recuperar datos del nivel N-1 (penúltimo nivel de detalle)
  remainingData = dataSize - recoveredData.size();
  auto dataFromN2 = LSBHandler::extractData(wtObj->output, levelN2Start,
                                            levelN2End, remainingData,
                                            /*scaleFactor=*/1e6);
  recoveredData.insert(recoveredData.end(), dataFromN2.begin(),
                       dataFromN2.end());

  // Si ya hemos recuperado todos los datos necesarios, terminamos
  // if (recoveredData.size() >= dataSize) {
  //   break;
  // }

  std::cout << "Data recovery completed.\n";
  return recoveredData;
}
