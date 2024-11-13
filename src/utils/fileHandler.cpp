#include "fileHandler.h"
#include <bitset>
#include <iostream>
#include <opencv2/opencv.hpp> // Usado para manejar archivos de imagen
#include <sndfile.hh> // Biblioteca para manejar archivos de audio WAV (libsndfile)

const int BLOCK_SIZE = 8;
// Divide la imagen en bloques de tamaño blockSize
std::vector<std::vector<std::string>>
FileHandler::divideImageIntoBlocks(const std::string &imagePath) {
  // Cargar la imagen en escala de grises
  // cv::Mat img = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
  cv::Mat img = cv::imread(imagePath);
  if (img.empty()) {
    std::cerr << "Error: No se pudo cargar la imagen " << imagePath
              << std::endl;
    return {};
  }

  // Inicializar la estructura para almacenar los bloques
  std::vector<std::vector<std::string>> blocks;
  std::vector<std::string> block;

  // Recorrer la imagen en pasos de BLOCKSIZE x BLOCKSIZE
  for (int row = 0; row < img.rows; row += BLOCK_SIZE) {
    for (int col = 0; col < img.cols; col += BLOCK_SIZE) {
      block.clear(); // Limpiar el bloque para el nuevo conjunto de píxeles

      // Recorrer el bloque de tamaño BLOCKSIZE x BLOCKSIZE
      for (int y = row; y < row + BLOCK_SIZE && y < img.rows; ++y) {
        for (int x = col; x < col + BLOCK_SIZE && x < img.cols; ++x) {
          // Convertir cada píxel a cadena binaria de 8 bits
          block.push_back(std::bitset<8>(img.at<uint8_t>(y, x)).to_string());
        }
      }

      // Agregar el bloque actual al vector de bloques
      blocks.push_back(block);
    }
  }

  return blocks;
}

// Guarda los bloques recuperados en un archivo de imagen
bool FileHandler::saveEncryptedImage(
    const std::vector<std::vector<std::string>> &blocks,
    const std::string &outputFilePath, int width, int height) {
  cv::Mat image(width, height, CV_8UC1);

#pragma omp parallel for collapse(2)
  for (int i = 0; i < image.rows; i += BLOCK_SIZE) {
    for (int j = 0; j < image.cols; j += BLOCK_SIZE) {
      int blockIdx =
          (i / BLOCK_SIZE) * (image.cols / BLOCK_SIZE) + (j / BLOCK_SIZE);
      for (int y = 0; y < BLOCK_SIZE; y++) {
        for (int g = 0; g < BLOCK_SIZE; g++) {
          image.at<uint8_t>(i + y, j + g) = static_cast<uint8_t>(
              std::bitset<8>(blocks[blockIdx][y * BLOCK_SIZE + g]).to_ulong());
        }
      }
    }
  }

  return cv::imwrite(outputFilePath, image);
}

bool FileHandler::extractAudio(const std::string &videoPath,
                               const std::string &outputAudioPath) {
  std::string command =
      "ffmpeg -i " + videoPath + " -map 0:a:0 -q:a 0 " + outputAudioPath;
  int result = std::system(command.c_str());
  return result == 0;
}

// Carga el archivo de audio en un vector de amplitudes (usando libsndfile)
std::vector<double> FileHandler::loadAudio(const std::string &audioPath) {
  SndfileHandle audioFile(audioPath);
  if (audioFile.error()) {
    std::cerr << "Error al cargar el archivo de audio " << audioPath
              << std::endl;
    return {};
  }

  // Configuración del archivo de audio
  std::vector<double> audioData(audioFile.frames() * audioFile.channels());
  audioFile.read(&audioData[0], audioData.size());

  return audioData;
}

// Guarda el archivo de audio modificado en formato WAV (usando libsndfile)
bool FileHandler::saveAudio(const std::string &outputAudioPath,
                            const std::vector<double> &audioData) {
  SndfileHandle outputFile(outputAudioPath, SFM_WRITE,
                           SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1,
                           44100); // Formato WAV

  if (outputFile.error()) {
    std::cerr << "Error al guardar el archivo de audio " << outputAudioPath
              << std::endl;
    return false;
  }

  outputFile.write(&audioData[0], audioData.size());
  return true;
}
