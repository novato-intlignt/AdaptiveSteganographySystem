#include "dataProcessor.h";
#include <bitset>
#include <cmath>
#include <iostream>
#include <stdexcept>

std::vector<unsigned char> DataProcessor::colorToBytes(
    const std::vector<std::vector<std::vector<std::string>>> &listBits) {
  std::vector<unsigned char> byteList;
  byteList.reserve(listBits.size() * listBits[0].size() * 3);

  for (const auto &row : listBits) {
    for (const auto &col : row) {
      for (const auto &channel : col) {
        byteList.push_back(
            static_cast<unsigned char>(std::bitset<8>(channel).to_ulong()));
      }
    }
  }

  return byteList;
}

std::vector<std::vector<std::vector<std::string>>>
DataProcessor::bytesToColor(const std::vector<unsigned char> &byteList,
                            int rows, int cols) {
  std::vector<std::vector<std::vector<std::string>>> listBits(
      rows, std::vector<std::vector<std::string>>(
                cols, std::vector<std::string>(3, "")));

  size_t counter = 0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
        listBits[i][j][k] = std::bitset<8>(byteList[counter++]).to_string();
      }
    }
  }

  return listBits;
}

bool DataProcessor::isAudioSuitableForEmbedding(size_t totalSamples, int levels,
                                                int imageWidth,
                                                int imageHeight) {
  if (levels <= 0) {
    throw std::invalid_argument("Number of levels must be greater than 0.");
  }

  // Total de bits a incrustar en la imagen (8 bits por cada byte/píxel de la
  // imagen)
  size_t totalImageBits = imageWidth * imageHeight * 8;

  // Calcular coeficientes disponibles en los últimos dos niveles
  size_t coefficientsLevelN =
      totalSamples / static_cast<size_t>(std::pow(2, levels));
  size_t coefficientsLevelNMinus1 =
      totalSamples / static_cast<size_t>(std::pow(2, levels - 1));

  size_t totalAvailableCoefficients =
      coefficientsLevelN + coefficientsLevelNMinus1;

  std::cout << "Audio Analysis:" << std::endl;
  std::cout << "Total Samples: " << totalSamples << std::endl;
  std::cout << "Decomposition Levels: " << levels << std::endl;
  std::cout << "Coefficients at Level N: " << coefficientsLevelN << std::endl;
  std::cout << "Coefficients at Level N-1: " << coefficientsLevelNMinus1
            << std::endl;
  std::cout << "Total Available Coefficients (Level N + N-1): "
            << totalAvailableCoefficients << std::endl;
  std::cout << "Bits Required for Image: " << totalImageBits << std::endl;

  // Verificar si los coeficientes disponibles son suficientes
  if (totalAvailableCoefficients >= totalImageBits) {
    std::cout << "Result: The audio is suitable for embedding the image."
              << std::endl;
    return true;
  } else {
    std::cout << "Result: The audio is NOT suitable for embedding the image."
              << std::endl;
    return false;
  }
}
