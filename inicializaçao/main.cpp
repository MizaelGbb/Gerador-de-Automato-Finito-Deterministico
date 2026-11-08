#include <iostream>
#include "automato.h"

using namespace std;

int main() {

    Automato automato;

    automato.adicionarToken("se", "SE");
    automato.adicionarToken("senao", "SENAO");
    automato.adicionarToken("entao", "ENTAO");

    automato.exibirMatriz();

    return 0;
}