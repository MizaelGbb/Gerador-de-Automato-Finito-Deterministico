#ifndef LEITOR_TEXTO_H
#define LEITOR_TEXTO_H

using namespace std;

#include <string>
#include <vector>

struct EntradaLinguagem {
    std::vector<std::string> palavrasChave;
    std::vector<std::string> regrasGramatica;
};

// Declaração da função que lê e categoriza o conteúdo do arquivo
EntradaLinguagem processarArquivoGramatica(const std::string& caminhoArquivo);

#endif