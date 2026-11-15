#ifndef DETERMINIZACAO_H
#define DETERMINIZACAO_H

#include "automato.h"
#include <set>

using namespace std;

class Determinizador {
public:
    // Função principal que recebe o AFND e cospe um AFD limpo
    static Automato determinizar(Automato& afnd);

private:
    // Encontra todos os estados alcançáveis apenas por transições vazias
    static set<int> fechoEpsilon(Automato& afnd, const set<int>& estados);
    
    // Encontra todos os estados alcançáveis lendo um símbolo específico
    static set<int> mover(Automato& afnd, const set<int>& estados, char simbolo);
};

#endif