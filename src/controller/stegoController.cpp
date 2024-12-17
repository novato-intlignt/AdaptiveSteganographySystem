#include "stegoController.h"
#include "./../model/audioProcessor.h"
#include "./../model/cellularAutomaton.h"
#include "./../model/dataProcessor.h"
#include "./../utils/fileHandler.h"
#include <iostream>
#include <string>
#include <vector>

StegoController::StegoController() {}

bool StegoController::startEmbedding(const std::string &videoFile,
                                     const std::string &imageFile) {
  std::vector<std::vector<std::vector<std::string>>> listBits =
      FileHandler::listBits(imageFile);

  int levels = 4;
  FileHandler::imageReconstruction(listBits, "./../image.png");
  auto [key, height, width] = CellularAutomaton::rule30(listBits);
  CellularAutomaton::encryptBlocks(listBits, key, "E");

  auto imageData = DataProcessor::colorToBytes(listBits);

  bool resAudio = FileHandler::extractAudio(videoFile, "./../audio.wav");

  int sampleRate, channels, totalSamples;

  // Cargar audio
  auto audioData = FileHandler::loadAudioData("./../audio.wav", sampleRate,
                                              channels, totalSamples);
  bool suitable = DataProcessor::isAudioSuitableForEmbedding(
      totalSamples, levels, width, height);
  if (!suitable) {
    return false;
  }

  // Incrustar datos
  // AudioProcessor::embedDataInAudio(audioData, imageData, channels);
  auto [seed, r] = AudioProcessor::embedDataInAudio(audioData, imageData);

  size_t dataSize = imageData.size();
  // std::vector<unsigned char> recoveredData =
  //     AudioProcessor::recoverDataFromAudio(audioData, channels, dataSize);
  std::vector<unsigned char> recoveredData =
      AudioProcessor::recoverDataFromAudio(audioData, dataSize, seed, r);
  std::vector<std::vector<std::vector<std::string>>> codedData =
      DataProcessor::bytesToColor(recoveredData, height, width);

  CellularAutomaton::encryptBlocks(codedData, key, "D");

  FileHandler::saveBinaryData("./../data.bin", key, height, width, dataSize,
                              seed, r);

  FileHandler::imageReconstruction(listBits, "./../imageCoded.png");
  // Guardar audio modificado
  FileHandler::saveAudioData("./../audioEnc.wav", audioData, sampleRate,
                             channels);

  FileHandler::imageReconstruction(codedData, "./../imageDecoded.png");
  std::cout << "Audio saved to " << "./../audioEnc.wav" << " with " << channels
            << " channels.\n"
            << " image has " << dataSize;

  // return resImg && resAudio;
  return true;
}
void StegoController::retrieveData(const std::string &videoFile) {
  // int sampleRate, channels;
  // auto audioData = FileHandler::loadAudioData("./../audioModified.wav",
  //                                             sampleRate, channels);
  // auto [key, height, width, dataSize] =
  //     FileHandler::extractBinaryData("./../data.bin");
  //
  // std::vector<unsigned char> recoveredData =
  //     AudioProcessor::recoverDataFromAudio(audioData, dataSize);
  // std::vector<std::vector<std::vector<std::string>>> codedData =
  //     DataProcessor::bytesToColor(recoveredData, height, width);
  //
  // CellularAutomaton::encryptBlocks(codedData, key, "D");
  //
  // // Guardar audio modificado
  // FileHandler::saveAudioData("./../audioEnc.wav", audioData, sampleRate,
  //                            channels);
  //
  // FileHandler::imageReconstruction(codedData, "./../imageDecoded.png");
  // std::cout << "Audio saved to " << "./../audioEnc.wav" << " with " <<
  // channels
  //           << " channels.\n"
  //           << " image has " << dataSize;
}
// void StegoController::retrieveData(const std::string &videoFile) {
//   int sampleRate, channels;
//   auto audioData =
//       FileHandler::loadAudioData("./../audioEnc.wav", sampleRate, channels);
//   size_t dataSize = 691200;
//
//   // std::vector<unsigned char> recoveredData =
//   //     AudioProcessor::recoverDataFromAudio(audioData, channels, dataSize);
//
//   std::vector<unsigned char> recoveredData =
//       AudioProcessor::recoverDataFromAudio(audioData, dataSize);
//
//   FileHandler::saveDataAsImage(recoveredData, "./../imageDecoded.png", 480,
//   480,
//                                3);
// }
//
// void StegoController::verifyResults() { std::cout << "verificando"; }
