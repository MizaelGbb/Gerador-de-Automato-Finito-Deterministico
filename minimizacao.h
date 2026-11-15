#ifndef MINIMIZACAO_H
#define MINIMIZACAO_H

#include "automato.h"
#include <vector>
#include <map>

using namespace std;

class Minimizador {
public:
    // Recebe um AFD e retorna um AFD minimizado (sem estados redundantes)
    static Automato minimizar(Automato& afd);

private:
    // Função auxiliar para descobrir para onde um estado vai ao ler um símbolo
    static int obterDestino(Automato& afd, int estadoOrigem, char simbolo);
    
    // Função auxiliar para descobrir a qual grupo um estado pertence
    static int descobrirGrupo(const vector<vector<int>>& particao, int estado);
};

#endif