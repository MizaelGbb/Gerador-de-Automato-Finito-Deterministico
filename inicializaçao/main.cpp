#include <iostream>
#include "leitorArquivo.h"
#include "automato.h"

using namespace std;

int main() {

    EntradaLinguagem dados = processarArquivoGramatica("gramatica.txt");

    Automato automato;

    // Adiciona palavras-chave
    for (const string& palavra : dados.palavrasChave) {
        automato.adicionarToken(palavra, palavra);
    }

    // Adiciona gramáticas
    automato.adicionarGramatica(dados.regrasGramatica, "VARIAVEL");

    automato.exibirMatriz();

    return 0;
}


