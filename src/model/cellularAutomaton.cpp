#include "cellularAutomaton.h"
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <opencv2/opencv.hpp>

constexpr int BLOCK_SIZE = 8;

void CellularAutomaton::applyCompositeRules(std::string &block,
                                            const std::string &action,
                                            int keyIndex) {
  static const std::unordered_map<char, std::unordered_map<std::string, char>>
      rules = {{'a',
                {{"111", '0'},
                 {"110", '0'},
                 {"101", '0'},
                 {"100", '0'},
                 {"011", '1'},
                 {"010", '1'},
                 {"001", '1'},
                 {"000", '1'}}},
               {'c',
                {{"111", '0'},
                 {"110", '1'},
                 {"101", '0'},
                 {"100", '1'},
                 {"011", '0'},
                 {"010", '1'},
                 {"001", '0'},
                 {"000", '1'}}},
               {'C',
                {{"111", '1'},
                 {"110", '0'},
                 {"101", '1'},
                 {"100", '0'},
                 {"011", '1'},
                 {"010", '0'},
                 {"001", '1'},
                 {"000", '0'}}},
               {'A',
                {{"111", '1'},
                 {"110", '1'},
                 {"101", '1'},
                 {"100", '1'},
                 {"011", '0'},
                 {"010", '0'},
                 {"001", '0'},
                 {"000", '0'}}},
               {'B',
                {{"111", '1'},
                 {"110", '1'},
                 {"101", '0'},
                 {"100", '0'},
                 {"011", '1'},
                 {"010", '1'},
                 {"001", '0'},
                 {"000", '0'}}},
               {'b',
                {{"111", '0'},
                 {"110", '0'},
                 {"101", '1'},
                 {"100", '1'},
                 {"011", '0'},
                 {"010", '0'},
                 {"001", '1'},
                 {"000", '1'}}}};

  static const std::vector<std::pair<std::string, std::string>>
      compositeRules[] = {{{"E", "AAAaaAaa"}, {"D", "CCccCccC"}},
                          {{"E", "AaaAaaAA"}, {"D", "ccCccCCC"}},
                          {{"E", "aAaaAAAa"}, {"D", "CccCCCcc"}},
                          {{"E", "aaAAAaaA"}, {"D", "cCCCccCc"}},
                          {{"E", "CCcCCccc"}, {"D", "aAAaAAaa"}},
                          {{"E", "ccCCcCCc"}, {"D", "aaaAAaAA"}},
                          {{"E", "CcccCCcC"}, {"D", "AAaaaAAa"}},
                          {{"E", "cCCcccCC"}, {"D", "AaAAaaaA"}},
                          {{"E", "BBbBCaca"}, {"D", "BBbBcAca"}},
                          {{"E", "bcABcABb"}, {"D", "bCaBCaBb"}},
                          {{"E", "CabbBCaB"}, {"D", "cAbbBcAB"}},
                          {{"E", "BbcabbCA"}, {"D", "bBcAbbCA"}},
                          {{"E", "bcACAbbB"}, {"D", "bCaCAbbB"}},
                          {{"E", "bBbcACAb"}, {"D", "bBbCaCAb"}},
                          {{"E", "CABcaBbb"}, {"D", "CABcaBbb"}},
                          {{"E", "BbcAbcAB"}, {"D", "BbCabCaB"}}};

  const std::string &ruleString =
      compositeRules[keyIndex % 16][action == "E" ? 0 : 1].second;
  std::vector<char> newBlock(block.size());

#pragma omp parallel for
  for (int i = 0; i < block.size(); i++) {
    std::string neighborhood;
    neighborhood.reserve(3);
    neighborhood += (i > 0) ? block[i - 1] : block.back();
    neighborhood += block[i];
    neighborhood += (i < block.size() - 1) ? block[i + 1] : block[0];
    newBlock[i] = rules.at(ruleString[i]).at(neighborhood);
  }

  block.assign(newBlock.begin(), newBlock.end());
}

std::vector<int> CellularAutomaton::rule30Encryption(
    const std::vector<std::vector<std::string>> &blocks) {
  std::string baseList;
  baseList.reserve(256);

  static const std::unordered_map<std::string, char> rule30 = {
      {"111", '0'}, {"110", '0'}, {"101", '0'}, {"100", '0'},
      {"011", '1'}, {"010", '1'}, {"001", '1'}, {"000", '1'}};

  for (int i = 0; i < 32; i++) {
    srand(static_cast<unsigned>(time(0)));
    int value = rand() % 24 + 2;
    baseList += blocks[value][i];
  }

  std::bitset<4> numIterations(blocks[0][8].substr(0, 4));
  unsigned long iterations = numIterations.to_ulong();

  std::vector<char> currentChain(baseList.begin(), baseList.end());
  std::vector<char> newChain(currentChain.size());

  for (int iter = 0; iter < iterations; iter++) {
#pragma omp parallel for
    for (int i = 0; i < currentChain.size(); i++) {
      std::string neighborhood;
      neighborhood.reserve(3);
      neighborhood += (i > 0) ? currentChain[i - 1] : currentChain.back();
      neighborhood += currentChain[i];
      neighborhood +=
          (i < currentChain.size() - 1) ? currentChain[i + 1] : currentChain[0];
      newChain[i] = rule30.at(neighborhood);
    }
    currentChain = newChain;
  }

  std::vector<int> encryptionValues;
  encryptionValues.reserve(currentChain.size() / 4);

  for (int i = 0; i < currentChain.size(); i += 4) {
    std::string chainIndex(currentChain.begin() + i,
                           currentChain.begin() + i + 4);
    encryptionValues.push_back(std::bitset<4>(chainIndex).to_ulong());
  }

  return encryptionValues;
}

void CellularAutomaton::encryptBlocks(
    std::vector<std::vector<std::string>> &blocks, const std::string &action,
    const std::vector<int> &key) {
#pragma omp parallel for collapse(2)
  for (int t = 0; t < 11; t++) {
    for (int i = 0; i < blocks.size(); i++) {
      for (int pixel = 0; pixel < BLOCK_SIZE * BLOCK_SIZE; pixel++) {
        std::string pixelValue = blocks[i][pixel];
        applyCompositeRules(pixelValue, action, key[pixel]);
        blocks[i][pixel] = pixelValue;
      }
    }
  }
}
