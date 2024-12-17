#ifndef LSB_HANDLER_H
#define LSB_HANDLER_H

#include <vector>

class LSBHandler {
public:
    // Incrustar un bit en el LSB de un coeficiente
    static double embedLSB(double coefficient, unsigned char bit, double scaleFactor = 1e6);

    // Extraer el LSB de un coeficiente
    static unsigned char extractLSB(double coefficient, double scaleFactor = 1e6);

    // Incrustar un byte en una secuencia de coeficientes
    static void embedByteInCoefficients(double* coefficients, int startIdx, unsigned char byte, double scaleFactor = 1e6);

    // Extraer un byte desde una secuencia de coeficientes
    static unsigned char extractByteFromCoefficients(double* coefficients, int startIdx, double scaleFactor = 1e6);

    // Incrustar datos completos en coeficientes
    static void embedData(double* coefficients, int startIdx, int endIdx, const std::vector<unsigned char>& dataToEmbed, size_t& dataIndex, double scaleFactor = 1e6);

    // Extraer datos completos desde coeficientes
    static std::vector<unsigned char> extractData(double* coefficients, int startIdx, int endIdx, size_t dataSize, double scaleFactor = 1e6);
};

#endif // LSB_HANDLER_H
