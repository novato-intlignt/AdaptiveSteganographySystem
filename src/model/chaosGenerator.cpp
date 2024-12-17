#include "chaosGenerator.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

std::tuple<std::vector<int>, double, double>
ChaosGenerator::generateRandomSequence(int startCoef, int endCoef) {
  // Validar longitud
  if (startCoef > endCoef) {
    throw std::invalid_argument("Is not in the range");
  }

  // Generador de números aleatorios
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> seedDist(0.01,
                                            0.99); // Semilla inicial: (0, 1)
  std::uniform_real_distribution<> rDist(
      3.7, 4.0); // Parámetro de control: [3.7, 4.0]

  // Generar valores aleatorios
  double initialSeed = seedDist(gen);
  double r = rDist(gen);

  // Generar secuencia caótica
  std::vector<double> sequence(endCoef - startCoef);
  sequence[0] = initialSeed;

  for (size_t i = 0; i < sequence.size(); i++) {
    sequence[i] = r * sequence[i - 1] * (1.0 - sequence[i - 1]);
  }
  // Escalar la secuencia dentro del rango startCoef y endCoef
  double minValue = *std::min_element(sequence.begin(), sequence.end());
  double maxValue = *std::max_element(sequence.begin(), sequence.end());

  for (auto &value : sequence) {
    value = startCoef +
            (endCoef - startCoef) * (value - minValue) / (maxValue - minValue);
  }
  std::vector<int> intSequence(sequence.size());
  for (size_t i = 0; i < sequence.size(); i++) {
    intSequence[i] = std::round(sequence[i]); // Redondear al entero más cercano
  }
  return {intSequence, initialSeed, r};
}

std::vector<int> ChaosGenerator::regenerateSequence(int startCoef, int endCoef,
                                                    double seed, double r) {
  // Validar longitud
  if (startCoef > endCoef) {
    throw std::invalid_argument("Is not in the range");
  }
  // Generar secuencia caótica
  std::vector<double> sequence(endCoef - startCoef);
  sequence[0] = seed;

  for (size_t i = startCoef; i < endCoef; i++) {
    sequence[i] = r * sequence[i - 1] * (1.0 - sequence[i - 1]);
  }

  // Escalar la secuencia dentro del rango startCoef y endCoef
  double minValue = *std::min_element(sequence.begin(), sequence.end());
  double maxValue = *std::max_element(sequence.begin(), sequence.end());

  for (auto &value : sequence) {
    value = startCoef +
            (endCoef - startCoef) * (value - minValue) / (maxValue - minValue);
  }
  std::vector<int> intSequence(sequence.size());
  for (size_t i = 0; i < sequence.size(); i++) {
    intSequence[i] = std::round(sequence[i]); // Redondear al entero más cercano
  }
  return intSequence;
}
