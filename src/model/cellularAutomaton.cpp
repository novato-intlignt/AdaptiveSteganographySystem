#include "cellularAutomaton.h"
#include <bitset>
#include <omp.h>
#include <tuple>
#include <unordered_map>
#include <vector>

std::tuple<std::vector<int>, int, int> CellularAutomaton::rule30(
    const std::vector<std::vector<std::vector<std::string>>> &listBits) {
  static const std::unordered_map<std::string, char> regla30 = {
      {"111", '0'}, {"110", '0'}, {"101", '0'}, {"100", '0'},
      {"011", '1'}, {"010", '1'}, {"001", '1'}, {"000", '1'}};
  int rows = listBits.size();
  int cols = listBits[0].size();
  const int num_rules = rows * cols;
  const int num_rules_8 = (rows * cols) / 2;
  std::srand(std::time(0));

  std::string List_base = "";
  std::string List_base_new = "";

  std::string num_iterations = "";
  if (num_rules_8 % 2 == 0) {
#pragma omp parallel for
    for (int i = 0; i < num_rules_8; i++) {
      int k = rand() % rows, j = rand() % rows;
#pragma omp critical
      List_base += listBits[k][j][0];
    }
  } else {
#pragma omp parallel for
    for (int i = 0; i < num_rules_8; i++) {
      if (i != num_rules_8 - 1) {
        int k = rand() % rows, j = rand() % rows;
#pragma omp critical
        List_base += listBits[k][j][0];
      } else {
        int k = rand() % rows, j = rand() % rows;
#pragma omp critical
        List_base += (listBits[k][j][0] + listBits[k][j][0].substr(0, 4));
        num_iterations += listBits[j][k][0].substr(0, 4);
      }
    }
  }

  std::bitset<4> iterations(num_iterations);
  unsigned long base_10_iterations = iterations.to_ullong();

#pragma omp parallel for
  for (int j = 0; j < base_10_iterations; j++) {
    List_base_new = ""; // Reset List_base_new before each iteration
#pragma omp parallel for
    for (int i = 0; i < List_base.size(); i++) {
      std::string vecindad;
      vecindad.reserve(3);

      // Correcci�n para manejo de bordes

      vecindad += (i > 0) ? List_base[i - 1] : List_base[List_base.size() - 1];
      vecindad += List_base[i];
      vecindad += (i < List_base.size() - 1) ? List_base[i + 1] : List_base[0];

#pragma omp critical
      List_base_new += regla30.at(vecindad);
    }
    List_base = List_base_new;
  }

  std::vector<int> index_encriptation;
  index_encriptation.reserve(num_rules);

  // Asegurar indices correctos
#pragma omp parallel for
  for (int i = 0; i < num_rules; i++) {
    std::string index_string;
    for (int j = 0; j < 4 && (i * 4 + j) < List_base.size(); j++) {
      if (i * 4 + j < List_base.size()) {
        index_string += List_base[i * 4 + j];
      }
    }
    while (index_string.size() < 4) {
      index_string += '0';
    }
#pragma omp critical
    index_encriptation.push_back(std::bitset<4>(index_string).to_ulong() % 16);
  }
  return {index_encriptation, rows, cols};
}

void CellularAutomaton::encryptBlocks(
    std::vector<std::vector<std::vector<std::string>>> &listBits,
    const std::vector<int> &key, const std::string &action) {
  std::cout << "Primeros pixeles antes: \n";
  std::cout << listBits[0][0][0] << std::endl;
  std::cout << listBits[1][0][1] << std::endl;
  std::cout << listBits[2][0][2] << std::endl;

  // Definici�n de reglas de encriptaci�n.
  std::map<char, std::map<std::string, char>> rules;
  rules['a'] = {{"111", '0'}, {"110", '0'}, {"101", '0'}, {"100", '0'},
                {"011", '1'}, {"010", '1'}, {"001", '1'}, {"000", '1'}};
  rules['c'] = {{"111", '0'}, {"110", '1'}, {"101", '0'}, {"100", '1'},
                {"011", '0'}, {"010", '1'}, {"001", '0'}, {"000", '1'}};
  rules['C'] = {{"111", '1'}, {"110", '0'}, {"101", '1'}, {"100", '0'},
                {"011", '1'}, {"010", '0'}, {"001", '1'}, {"000", '0'}};
  rules['A'] = {{"111", '1'}, {"110", '1'}, {"101", '1'}, {"100", '1'},
                {"011", '0'}, {"010", '0'}, {"001", '0'}, {"000", '0'}};
  rules['B'] = {{"111", '1'}, {"110", '1'}, {"101", '0'}, {"100", '0'},
                {"011", '1'}, {"010", '1'}, {"001", '0'}, {"000", '0'}};
  rules['b'] = {{"111", '0'}, {"110", '0'}, {"101", '1'}, {"100", '1'},
                {"011", '0'}, {"010", '0'}, {"001", '1'}, {"000", '1'}};

  // Definici�n de las reglas compuestas para la encriptaci�n.
  std::map<int, std::map<std::string, std::string>> rule_comp;
  rule_comp[0] = {{"E", "AAAaaAaa"}, {"D", "CCccCccC"}};
  rule_comp[1] = {{"E", "AaaAaaAA"}, {"D", "ccCccCCC"}};
  rule_comp[2] = {{"E", "aAaaAAAa"}, {"D", "CccCCCcc"}};
  rule_comp[3] = {{"E", "aaAAAaaA"}, {"D", "cCCCccCc"}};
  rule_comp[4] = {{"E", "CCcCCccc"}, {"D", "aAAaAAaa"}};
  rule_comp[5] = {{"E", "ccCCcCCc"}, {"D", "aaaAAaAA"}};
  rule_comp[6] = {{"E", "CcccCCcC"}, {"D", "AAaaaAAa"}};
  rule_comp[7] = {{"E", "cCCcccCC"}, {"D", "AaAAaaaA"}};
  rule_comp[8] = {{"E", "BBbBCaca"}, {"D", "BBbBcAca"}};
  rule_comp[9] = {{"E", "bcABcABb"}, {"D", "bCaBCaBb"}};
  rule_comp[10] = {{"E", "CabbBCaB"}, {"D", "cAbbBcAB"}};
  rule_comp[11] = {{"E", "bBCabbCA"}, {"D", "bBcAbbCA"}};
  rule_comp[12] = {{"E", "bcACAbbB"}, {"D", "bCaCAbbB"}};
  rule_comp[13] = {{"E", "bBbcACAb"}, {"D", "bBbCaCAb"}};
  rule_comp[14] = {{"E", "CABcaBbb"}, {"D", "CABcaBbb"}};
  rule_comp[15] = {{"E", "BbcAbcAB"}, {"D", "BbCabCaB"}};

  int cols = listBits[0].size();
  int rows = listBits.size();

#pragma omp parallel for collapse(2)
  for (int iteration = 0; iteration < 7; iteration++) {
    int count = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        for (int k = 0; k < 3; k++) {
          // Tomamos el estado del p�xel en la imagen
          std::string base_states = listBits[i][j][k];
          std::string evol_states = "";

          // Obtenemos la cadena de reglas correspondiente de acuerdo al valor
          // de la clave
          std::string cadena_rule = rule_comp[key[count]][action];

          // Procesamos cada bit del estado base
          for (int l = 0; l < base_states.size(); l++) {
            std::string vecindad = "";
            vecindad.reserve(3);

            // Correcci�n para el manejo de bordes en base_states
            vecindad +=
                (l > 0) ? base_states[l - 1]
                        : base_states.back(); // Primer bit: vecino a la
                                              // izquierda o �ltimo de la cadena
            vecindad += base_states[l]; // Bit actual
            vecindad += (l < base_states.size() - 1)
                            ? base_states[l + 1]
                            : base_states[0]; // �ltimo bit: vecino a la derecha
                                              // o primero de la cadena

            // Aplicamos la regla de acuerdo con la cadena de la regla compuesta
            evol_states += rules[cadena_rule[l]][vecindad];
          }
          listBits[i][j][k] = evol_states;
        }
        count++; // Incrementamos el �ndice de la clave para el siguiente p�xel
      }
    }
  }

  std::cout << "Primeros pixeles evolucionados: \n";
  std::cout << listBits[0][0][0] << std::endl;
  std::cout << listBits[1][0][1] << std::endl;
  std::cout << listBits[2][0][2] << std::endl;
}
