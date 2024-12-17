#include "LSBHandler.h"
#include <cmath>
#include <iostream>
#include <vector>

// Incrustar un bit en el LSB de un coeficiente
double LSBHandler::embedLSB(double coefficient, unsigned char bit,
                            double scaleFactor) {
  // Escalar el coeficiente a un número entero
  int scaledCoeff = static_cast<int>(std::round(coefficient * scaleFactor));

  // Limpiar el LSB
  scaledCoeff &= ~1;

  // Incrustar el bit en el LSB
  scaledCoeff |= bit;

  // Restaurar al formato de punto flotante
  return static_cast<double>(scaledCoeff) / scaleFactor;
}

// Extraer el LSB de un coeficiente
unsigned char LSBHandler::extractLSB(double coefficient, double scaleFactor) {
  // Escalar el coeficiente a un número entero
  int scaledCoeff = static_cast<int>(std::round(coefficient * scaleFactor));

  // Extraer el LSB
  return scaledCoeff & 1;
}

// Incrustar un byte en una secuencia de coeficientes
void LSBHandler::embedByteInCoefficients(double *coefficients, int startIdx,
                                         unsigned char byte,
                                         double scaleFactor) {
  for (int i = 0; i < 8; ++i) {
    // Extraer el bit i-ésimo del byte
    unsigned char bit = (byte >> (7 - i)) & 1;

    // Incrustar el bit en el coeficiente correspondiente
    coefficients[startIdx + i] =
        embedLSB(coefficients[startIdx + i], bit, scaleFactor);
  }
}

// Extraer un byte desde una secuencia de coeficientes
unsigned char LSBHandler::extractByteFromCoefficients(double *coefficients,
                                                      int startIdx,
                                                      double scaleFactor) {
  unsigned char byte = 0;

  for (int i = 0; i < 8; ++i) {
    // Extraer el LSB y colocarlo en la posición adecuada
    unsigned char bit = extractLSB(coefficients[startIdx + i], scaleFactor);
    byte |= (bit << (7 - i));
    // if (startIdx > 1902613 && startIdx < 1902639) {
    //   std::cout << static_cast<int>(bit) << ":" << static_cast<int>(byte)
    //             << ",";
    // }
  }

  return byte;
}

// Incrustar datos completos en coeficientes
void LSBHandler::embedData(double *coefficients, int startIdx, int endIdx,
                           const std::vector<unsigned char> &dataToEmbed,
                           size_t &dataIndex, double scaleFactor) {
  size_t coefIndex = startIdx;
  size_t totalCoefficients = endIdx;

  // Recorrer cada byte de los datos
  while (dataIndex < dataToEmbed.size() && coefIndex + 8 <= totalCoefficients) {
    // Incrustar el byte en los coeficientes
    embedByteInCoefficients(coefficients, coefIndex, dataToEmbed[dataIndex++],
                            scaleFactor);

    // Avanzar al siguiente grupo de 8 coeficientes
    coefIndex += 8;
  }

  if (dataIndex < dataToEmbed.size()) {
    std::cerr << "Error: Insufficient coefficients to embed the entire data!"
              << std::endl;
  }
}

// Extraer datos completos desde coeficientes
std::vector<unsigned char> LSBHandler::extractData(double *coefficients,
                                                   int startIdx, int endIdx,
                                                   size_t dataSize,
                                                   double scaleFactor) {
  std::vector<unsigned char> extractedData;
  extractedData.reserve(dataSize);

  size_t coefIndex = startIdx;
  size_t totalCoefficients = endIdx;

  // Recuperar datos de los coeficientes
  while (extractedData.size() < dataSize &&
         coefIndex + 8 <= totalCoefficients) {
    // Extraer un byte de los coeficientes
    extractedData.push_back(
        extractByteFromCoefficients(coefficients, coefIndex, scaleFactor));

    // Avanzar al siguiente grupo de 8 coeficientes
    coefIndex += 8;
  }

  if (extractedData.size() < dataSize) {
    std::cerr << "Error: Not enough coefficients to recover the entire data!"
              << std::endl;
  }

  return extractedData;
}
