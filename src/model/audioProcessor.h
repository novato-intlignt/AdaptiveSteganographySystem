#ifndef AUDIO_PROCESSOR2_H
#define AUDIO_PROCESSOR2_H

#include <tuple>
#include <vector>

class AudioProcessor {
public:
  // Incrusta datos en el audio utilizando los coeficientes de Wavelet
  static void embedDataInAudio3(std::vector<double> &audioData,
                                const std::vector<unsigned char> &dataToEmbed);
  static void embedDataInAudio2(std::vector<std::vector<double>> &audioData,
                                const std::vector<unsigned char> &dataToEmbed,
                                int channels);
  static std::tuple<double, double>
  embedDataInAudio(std::vector<double> &audioData,
                   const std::vector<unsigned char> &dataToEmbed);

  static std::vector<unsigned char>
  recoverDataFromAudio2(const std::vector<std::vector<double>> &audioData,
                        int channels, size_t dataSize);
  static std::vector<unsigned char>
  recoverDataFromAudio3(const std::vector<double> &audioData, size_t dataSize);

  static std::vector<unsigned char>
  recoverDataFromAudio(const std::vector<double> &audioData, size_t dataSize,
                       double seed, double r);
};

#endif // AUDIO_PROCESSOR_H
