#ifndef LEITOR_TEXTO_H
#define LEITOR_TEXTO_H

using namespace std;

#include <string>
#include <vector>

struct EntradaLinguagem {
    vector<string> palavrasChave;
    vector<string> regrasGramatica;
};

// Declaração da função que lê e categoriza o conteúdo do arquivo
EntradaLinguagem processarArquivoGramatica(const std::string& caminhoArquivo);

#endif