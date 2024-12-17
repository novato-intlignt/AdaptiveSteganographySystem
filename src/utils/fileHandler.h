#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <opencv2/opencv.hpp> // Usado para manejar archivos de imagen
#include <string>
#include <tuple>
#include <vector>

class FileHandler {
public:
  static cv::Mat imageProcess(const std::string &imagePath);

  // Converts the image into a list of bits
  static std::vector<std::vector<std::vector<std::string>>>
  listBits(const std::string &imagePath);
  // Reconstructs the image from the list of bits and saves it
  //
  static void imageReconstruction(
      const std::vector<std::vector<std::vector<std::string>>> &listBits,
      const std::string &savePath);

  // Lee el archivo de audio en formato WAV y lo carga en un vector de
  // amplitudes
  static std::vector<double> loadAudioData(const std::string &audioPath,
                                           int &sampleRate, int &channels,
                                           int &totalSamples);

  static void saveAudioData(const std::string &outputPath,
                            const std::vector<double> &audioData,
                            int sampleRate, int channels);

  static bool extractAudio(const std::string &videoPath,
                           const std::string &outputAudioPath);

  static void saveBinaryData(const std::string &filePath,
                             const std::vector<int> &dataKey, int rows,
                             int cols, size_t dataSize, double seed, double r);

  static std::tuple<std::vector<int>, int, int, size_t>
  extractBinaryData(const std::string &filePath);

private:
  FileHandler() = default; // Evitar instanciación de la clase
};

#endif // FILEHANDLER_H
