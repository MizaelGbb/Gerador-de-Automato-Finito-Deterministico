#include <iostream>
#include "leitorArquivo.h"
#include "automato.h"
#include "determinizacao.h"
#include "minimizacao.h"

using namespace std;

int main() {
    EntradaLinguagem dados = processarArquivoGramatica("gramatica.txt");

    // 1. Cria o autômato original chamando-o de 'afnd'
    Automato afnd;

    // Adiciona palavras-chave
    for (const string& palavra : dados.palavrasChave) {
        afnd.adicionarToken(palavra, palavra);
    }

    // Adiciona gramáticas
    afnd.adicionarGramatica(dados.regrasGramatica, "VARIAVEL");

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