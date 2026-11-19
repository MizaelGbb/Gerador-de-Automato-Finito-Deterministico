#ifndef MINIMIZACAO_H
#define MINIMIZACAO_H

#include "automato.h"
#include <set>

using namespace std;

class Minimizador {
public:
    // Remove inalcançáveis/mortos e adiciona o Estado de Erro
    static Automato minimizar(Automato& afd);

private:
    static int obterDestino(Automato& afd, int estadoOrigem, char simbolo);
    static set<int> obterAlcancaveis(Automato& afd);
    static set<int> obterVivos(Automato& afd);
};

#endif
