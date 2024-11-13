#ifndef CELLULAR_AUTOMATON_ENCRYPTION_H
#define CELLULAR_AUTOMATON_ENCRYPTION_H

#include <string>
#include <vector>

class CellularAutomaton {
public:
  // Applies composite rules to a given string based on the specified action and
  // key index
  static void applyCompositeRules(std::string &block, const std::string &action,
                                  int keyIndex);

  // Generates encryption values using Rule 30 cellular automaton
  static std::vector<int>
  rule30Encryption(const std::vector<std::vector<std::string>> &blocks);

  // Encrypts blocks using the specified action and key values
  static void encryptBlocks(std::vector<std::vector<std::string>> &blocks,
                            const std::string &action,
                            const std::vector<int> &key);

private:
  CellularAutomaton() = default; // Prevent instantiation
};

#endif // CELLULAR_AUTOMATON_ENCRYPTION_H
