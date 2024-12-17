#include "fileHandler.h"
#include <bitset>
#include <fstream>
#include <iostream>
#include <random>
#include <sndfile.hh> // Biblioteca para manejar archivos de audio WAV (libsndfile)
#include <vector>

cv::Mat FileHandler::imageProcess(const std::string &imagePath) {
  cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::runtime_error("Error: Could not load image at " + imagePath);
  }
  return image;
}

std::vector<std::vector<std::vector<std::string>>>
FileHandler::listBits(const std::string &imagePath) {
  cv::Mat img = imageProcess(imagePath);
  std::vector<std::vector<std::vector<std::string>>> listBits(
      img.rows, std::vector<std::vector<std::string>>(
                    img.cols, std::vector<std::string>(3, "")));

#pragma omp parallel for collapse(2)
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
      for (int k = 0; k < 3; k++) {
        std::bitset<8> binaryValue(pixel[k]);
        listBits[i][j][k] = binaryValue.to_string();
      }
    }
  }
  return listBits;
}

bool FileHandler::extractAudio(const std::string &videoPath,
                               const std::string &outputAudioPath) {
  std::string command =
      "ffmpeg -i " + videoPath + " -map 0:a:0 -q:a 0 " + outputAudioPath;
  int result = std::system(command.c_str());

  return result == 0;
}

// Carga el archivo de audio en un vector de amplitudes (usando libsndfile)

std::vector<double> FileHandler::loadAudioData(const std::string &audioPath,
                                               int &sampleRate, int &channels,
                                               int &totalSamples) {
  // Abrir archivo de audio
  SndfileHandle audioFile(audioPath);
  if (audioFile.error()) {
    throw std::runtime_error("Error opening audio file: " + audioPath);
  }

  // Obtener información del archivo
  size_t numFrames = audioFile.frames();
  sampleRate = audioFile.samplerate();
  channels = audioFile.channels();
  totalSamples = channels * numFrames;

  // Leer datos del archivo
  std::vector<int16_t> buffer(numFrames * channels);
  audioFile.read(buffer.data(), buffer.size());

  // Convertir datos a formato flotante y almacenar en un vector unidimensional
  std::vector<double> audioData(buffer.size());
  for (size_t i = 0; i < buffer.size(); i++) {
    audioData[i] = static_cast<double>(buffer[i]) / 32767.0;
  }

  // Imprimir información del archivo de audio
  std::cout << "Audio File: " << audioPath << std::endl;
  std::cout << "Sample Rate: " << sampleRate << " Hz" << std::endl;
  std::cout << "Channels: " << channels << std::endl;
  std::cout << "Total Frames: " << numFrames << std::endl;
  std::cout << "Total Samples: " << buffer.size() << std::endl;
  std::cout << "Duration: " << static_cast<double>(numFrames) / sampleRate
            << " seconds" << std::endl;

  return audioData;
}

void FileHandler::saveAudioData(const std::string &outputPath,
                                const std::vector<double> &audioData,
                                int sampleRate, int channels) {
  // Validar datos de entrada
  if (audioData.empty() || channels <= 0 || audioData.size() % channels != 0) {
    throw std::invalid_argument("Invalid audio data or channel configuration.");
  }

  size_t numFrames = audioData.size() / channels;

  // Crear archivo de salida
  SndfileHandle file(outputPath, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16,
                     channels, sampleRate);

  if (file.error()) {
    throw std::runtime_error("Error creating audio file: " + outputPath);
  }

  // Convertir y escribir datos intercalados
  std::vector<int16_t> interleavedData(audioData.size());
  for (size_t i = 0; i < numFrames; i++) {
    for (int ch = 0; ch < channels; ch++) {
      interleavedData[i * channels + ch] = static_cast<int16_t>(std::clamp(
          audioData[i * channels + ch] * 32767.0, -32768.0, 32767.0));
    }
  }

  // Escribir datos al archivo
  sf_count_t framesWritten =
      file.write(interleavedData.data(), interleavedData.size());
  if (framesWritten != static_cast<sf_count_t>(interleavedData.size())) {
    throw std::runtime_error("Error writing audio data to file.");
  }

  std::cout << "Audio file saved successfully: " << outputPath << std::endl;
}

void FileHandler::saveBinaryData(const std::string &filePath,
                                 const std::vector<int> &dataKey, int rows,
                                 int cols, size_t dataSize, double seed,
                                 double r) {
  std::ofstream file(filePath, std::ios::binary);
  if (!file) {
    throw std::runtime_error(
        "Error: Could not open file to save binary data at " + filePath);
  }

  // Guardar clave
  file.write(reinterpret_cast<const char *>(dataKey.data()),
             dataKey.size() * sizeof(int));

  // Guardar dimensiones
  file.write(reinterpret_cast<const char *>(&rows), sizeof(int));
  file.write(reinterpret_cast<const char *>(&cols), sizeof(int));

  // Guardar tamaño de datos
  file.write(reinterpret_cast<const char *>(&dataSize), sizeof(size_t));

  file.close();
  std::cout << "Data successfully saved to " << filePath << std::endl;
}

std::tuple<std::vector<int>, int, int, size_t>
FileHandler::extractBinaryData(const std::string &filePath) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file) {
    throw std::runtime_error(
        "Error: Could not open file to extract binary data at " + filePath);
  }

  file.seekg(0, std::ios::end);
  size_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // Leer dimensiones
  file.seekg(-static_cast<int>(sizeof(int) * 2 + sizeof(size_t)),
             std::ios::end);
  int rows, cols;
  file.read(reinterpret_cast<char *>(&rows), sizeof(int));
  file.read(reinterpret_cast<char *>(&cols), sizeof(int));

  // Leer tamaño de datos
  size_t dataSize;
  file.read(reinterpret_cast<char *>(&dataSize), sizeof(size_t));

  // Leer clave
  size_t keySize = (fileSize - sizeof(int) * 2 - sizeof(size_t)) / sizeof(int);
  file.seekg(0, std::ios::beg);
  std::vector<int> key(keySize);
  file.read(reinterpret_cast<char *>(key.data()), keySize * sizeof(int));

  file.close();
  return {key, rows, cols, dataSize};
}

void FileHandler::imageReconstruction(
    const std::vector<std::vector<std::vector<std::string>>> &List_bits,
    const std::string &savePath) {
#pragma omp parallel for collapse(2) schedule(dynamic)
  int width = List_bits[0].size();
  int height = List_bits.size();
  cv::Mat image = cv::Mat::zeros(width, height, CV_8UC3);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cv::Vec3b &pixel = image.at<cv::Vec3b>(i, j);
      for (int k = 0; k < 3; k++) {
        const std::string &binary_str = List_bits[i][j][k];

        if (binary_str.size() != 8) {

          std::cerr << "Error: Cadena binaria no tiene 8 bits." << std::endl;
          return;
        }

        unsigned char value = 0;
        for (size_t l = 0; l < binary_str.size(); l++) {
          if (binary_str[l] == '1') {
            value |= (1 << (7 - l));
          }
        }

        pixel[k] = value;
      }
    }
  }

  // Guardar imagen en formato sin compresi�n
  if (savePath == "E") {
    cv::imwrite(savePath, image);
    std::cout << "Imagen cifrada guardada." << std::endl;
  } else {
    cv::imwrite(savePath, image);
    std::cout << "Imagen descifrada guardada." << std::endl;
  }
}
