#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <string>
#include <vector>

class DataProcessor {
public:
  // Converts the list of bits into a single-byte representation
  static std::vector<unsigned char> colorToBytes(
      const std::vector<std::vector<std::vector<std::string>>> &listBits);

  // Converts bytes back to the list of bits
  static std::vector<std::vector<std::vector<std::string>>>
  bytesToColor(const std::vector<unsigned char> &byteList, int rows, int cols);

  static bool isAudioSuitableForEmbedding(size_t totalSamples, int levels,
                                          int imageWidth, int imageHeight);

private:
  DataProcessor() = default; // Prevent instantiation
};

#endif // DATA_PROCESSOR_H
