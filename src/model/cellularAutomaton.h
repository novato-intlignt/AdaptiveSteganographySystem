#ifndef CELLULAR_AUTOMATON_ENCRYPTION_H
#define CELLULAR_AUTOMATON_ENCRYPTION_H
//
// #include <string>
// #include <vector>
//
// class CellularAutomaton {
// public:
//   // Applies composite rules to a given string based on the specified action
//   and
//   // key index
//   static void applyCompositeRules(std::string &block, const std::string
//   &action,
//                                   int keyIndex);
//
//   // Generates encryption values using Rule 31 cellular automaton
//   static std::vector<int>
//   rule31Encryption(const std::vector<std::vector<std::string>> &blocks);
//
//   // Encrypts blocks using the specified action and key values
//   static void encryptBlocks(std::vector<std::vector<std::string>> &blocks,
//                             const std::string &action,
//                             const std::vector<int> &key);
//
// private:
//   CellularAutomaton() = default; // Prevent instantiation
// };
//
// #endif // CELLULAR_AUTOMATON_ENCRYPTION_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class CellularAutomaton {
public:
  // Applies Rule 31 cellular automaton for encryption
  static std::tuple<std::vector<int>, int, int>
  rule30(const std::vector<std::vector<std::vector<std::string>>> &listBits);

  // Encrypts or decrypts blocks using the specified action and key
  static void
  encryptBlocks(std::vector<std::vector<std::vector<std::string>>> &listBits,
                const std::vector<int> &key, const std::string &action);

  // Converts the list of bits into a single-byte representation
  static std::vector<unsigned char> colorToBytes(
      const std::vector<std::vector<std::vector<std::string>>> &listBits);

  // Converts bytes back to the list of bits
  static std::vector<std::vector<std::vector<std::string>>>
  bytesToColor(const std::vector<unsigned char> &byteList,
               std::pair<int, int> dimensions);

  // Reads dimensions from the binary file
  static std::pair<int, int> readDimensions(const std::string &keyFilePath);

  // Reads the encryption key from the binary file
  static std::vector<int> readKey(const std::string &keyFilePath);

private:
  CellularAutomaton() = default; // Prevent instantiation
};

#endif // CELLULAR_AUTOMATON_ENCRYPTION_H
