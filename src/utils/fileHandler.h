#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

class FileHandler {
public:
  // Divide la imagen en bloques de tamaño fijo
  static std::vector<std::vector<std::string>>
  divideImageIntoBlocks(const std::string &imagePath);

  // Guarda los bloques recuperados en un archivo de imagen
  static bool
  saveEncryptedImage(const std::vector<std::vector<std::string>> &blocks,
                     const std::string &outputFilePath, int width, int height);

  // Lee el archivo de audio en formato WAV y lo carga en un vector de
  // amplitudes
  static std::vector<double> loadAudio(const std::string &audioPath);

  static bool extractAudio(const std::string &videoPath,
                           const std::string &outputAudioPath);
  // Guarda el archivo de audio modificado en formato WAV
  static bool saveAudio(const std::string &outputAudioPath,
                        const std::vector<double> &audioData);

private:
  FileHandler() = default; // Evitar instanciación de la clase
};

#endif // FILEHANDLER_H
