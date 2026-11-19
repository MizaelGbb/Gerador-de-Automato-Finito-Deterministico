#include <iostream>
#include "leitorArquivo.h"
#include "automato.h"
#include "determinizacao.h"
#include "minimizacao.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Verifica se o arquivo foi passado como argumento
    if (argc < 2) {
        cerr << "Erro: Arquivo de entrada nao informado." << endl;
        cerr << "Uso: " << argv[0] << " <arquivo_de_gramatica.txt>" << endl;
        return 1; // Encerra o programa com erro
    }

    // Pega o nome do arquivo a partir do terminal (posição 1)
    string arquivoEntrada = argv[1];
    
    EntradaLinguagem dados = processarArquivoGramatica(arquivoEntrada);

    // 1. Cria o autômato original chamando-o de 'afnd'
    Automato afnd;

    // Adiciona palavras-chave
    for (const string& palavra : dados.palavrasChave) {
        afnd.adicionarToken(palavra, palavra);
    }

    // Adiciona gramáticas
    if (!dados.regrasGramatica.empty()) {
        afnd.adicionarGramatica(dados.regrasGramatica, "VARIAVEL");
    }

    afnd.exibirMatriz();

    // 2. Determinização: consome o 'afnd' e cria o 'afd'
    Automato afd = Determinizador::determinizar(afnd);

    cout << "### AFD DETERMINIZADO ###" << endl;
    afd.exibirMatriz();

    // 3. Minimização: consome o 'afd' e cria o 'afdMinimizado'
    Automato afdMinimizado = Minimizador::minimizar(afd);
    
    cout << "\n### AFD DEPOIS DA MINIMIZACAO ###" << endl;
    afdMinimizado.exibirMatriz();

    return 0;
}
