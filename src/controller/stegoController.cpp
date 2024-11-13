#include "stegoController.h"
#include "./../model/cellularAutomaton.h"
#include "./../utils/fileHandler.h"
#include <iostream>
#include <string>
#include <vector>

StegoController::StegoController() {}

bool StegoController::startEmbedding(const std::string &videoFile,
                                     const std::string &imageFile) {
  std::vector<std::vector<std::string>> imageBlocks =
      FileHandler::divideImageIntoBlocks(imageFile);

  std::vector<int> key = CellularAutomaton::rule30Encryption(imageBlocks);
  std::string outputFIle = "./../image.jpg";
  CellularAutomaton::encryptBlocks(imageBlocks, "E", key);
  bool resImg =
      FileHandler::saveEncryptedImage(imageBlocks, outputFIle, 256, 256);
  bool resAudio = FileHandler::extractAudio(videoFile, "./../audio.wav");

  return resImg && resAudio;
}

void StegoController::retrieveData(const std::string &videoFile) {
  std::cout << "empezando a desencriptar";
}

void StegoController::verifyResults() { std::cout << "verificando"; }
