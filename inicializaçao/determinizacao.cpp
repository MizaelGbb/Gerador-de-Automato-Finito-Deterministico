#include "determinizacao.h"
#include <queue>
#include <map>

using namespace std;

// 1. Fecho-Epsilon: "Se eu estou nestes estados, para onde mais eu caio de graça?"
set<int> Determinizador::fechoEpsilon(Automato& afnd, const set<int>& estados) {
    set<int> fecho = estados;
    queue<int> fila;
    
    for (int e : estados) fila.push(e);

    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();
        
        for (auto& tv : afnd.transicoesVazias) {
            if (tv.origem == atual && fecho.find(tv.destino) == fecho.end()) {
                fecho.insert(tv.destino);
                fila.push(tv.destino);
            }
        }
    }
    return fecho;
}

// 2. Mover: "Estou nestes estados. Se eu ler o 'simbolo', para onde eu vou?"
set<int> Determinizador::mover(Automato& afnd, const set<int>& estados, char simbolo) {
    set<int> alcancados;
    for (int e : estados) {
        for (auto& t : afnd.transicoes) {
            if (t.origem == e && t.simbolo == simbolo) {
                alcancados.insert(t.destino);
            }
        }
    }
    return alcancados;
}

// 3. Algoritmo de Construção de Subconjuntos
Automato Determinizador::determinizar(Automato& afnd) {
    Automato afd;
    
    // Mapeia um conjunto de estados antigos (AFND) para um único ID de estado novo (AFD)
    map<set<int>, int> mapaEstadosDFA;
    queue<set<int>> estadosNaoProcessados;

    // O estado inicial do AFD é o fecho-épsilon do estado inicial do AFND
    set<int> inicialND = { afnd.estadoInicial }; // Geralmente o 0
    set<int> inicialDFA = fechoEpsilon(afnd, inicialND);

    // Registra esse novo "super estado" como o estado 0 do novo autômato
    mapaEstadosDFA[inicialDFA] = afd.estadoInicial; 
    estadosNaoProcessados.push(inicialDFA);

    while (!estadosNaoProcessados.empty()) {
        set<int> atual = estadosNaoProcessados.front();
        estadosNaoProcessados.pop();
        
        int idAtualDFA = mapaEstadosDFA[atual];

        // Para cada símbolo do alfabeto original, verificamos para onde o "super estado" vai
        for (char simb : afnd.alfabeto) {
            set<int> m = mover(afnd, atual, simb);
            set<int> dest = fechoEpsilon(afnd, m);

            if (dest.empty()) continue; // Se não vai para lugar nenhum, ignora (limbo)

            // Se descobrimos um "super estado" novo, cadastramos no AFD
            if (mapaEstadosDFA.find(dest) == mapaEstadosDFA.end()) {
                mapaEstadosDFA[dest] = afd.adicionarEstado();
                estadosNaoProcessados.push(dest);                                                                                                                                                       
            }

            // Cria a transição definitiva no novo autômato
            afd.adicionarTransicao(idAtualDFA, simb, mapaEstadosDFA[dest]);
        }

        // Se o "super estado" contém algum estado final do AFND, ele também é final no AFD
        for (int e : atual) {
            if (!afnd.estadosFinais[e].empty()) {
                for (string token : afnd.estadosFinais[e]) {
                    afd.marcarEstadoFinal(idAtualDFA, token);
                }
            }
        }
    }
    
    return afd;
}
