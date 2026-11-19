#include "leitorArquivo.h"
#include <iostream>
#include <fstream>
#include <sstream>

EntradaLinguagem processarArquivoGramatica(const string& caminhoArquivo) {
    EntradaLinguagem dados;
    ifstream arquivo(caminhoArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o arquivo " << caminhoArquivo << endl;
        return dados;
    }

    string linha;
    while (getline(arquivo, linha)) {
        // Regra 1: Remover espaços vazios nas pontas ou ignorar linhas vazias
        // (Aqui podemos fazer uma verificação básica de tamanho ou conteúdo)
        if (linha.empty()) {
            continue;
        }

        // Regra 3: Identificar se é uma Gramática Regular (contém o operador '::=')
        if (linha.find("::=") != string::npos) {
            dados.regrasGramatica.push_back(linha);
        } 
        else {
            // Regra 4: Caso contrário, tratamos como palavra reservada / símbolo / cadeia isolada
            dados.palavrasChave.push_back(linha);
        }
    }

    return dados;
}