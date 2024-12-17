#ifndef CHAOS_GENERATOR_H
#define CHAOS_GENERATOR_H

#include <stdexcept>
#include <vector>

class ChaosGenerator {
public:
  /**
   * Generate a chaotic sequence using the logistic map.
   * @param initialSeed Initial value for the sequence (0 < initialSeed < 1).
   * @param r Control parameter (3.7 <= r <= 4.0).
   * @param length Length of the chaotic sequence to generate.
   * @return A vector containing the chaotic sequence.
   */
  static std::tuple<std::vector<int>, double, double>
  generateRandomSequence(int startCoef, int endCoef);

  static std::vector<int> regenerateSequence(int startCoef, int endCoef,
                                             double seed, double r);

private:
  ChaosGenerator() = default; // Prevent instantiation
};

#endif // CHAOS_GENERATOR_H
