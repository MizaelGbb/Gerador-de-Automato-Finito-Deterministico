#include "minimizacao.h"
#include <queue>
#include <map>

using namespace std;

int Minimizador::obterDestino(Automato& afd, int estadoOrigem, char simbolo) {
    for (auto& t : afd.transicoes) {
        if (t.origem == estadoOrigem && t.simbolo == simbolo) {
            return t.destino;
        }
    }
    return -1;
}

// Retorna os estados que conseguimos visitar a partir do estado inicial
set<int> Minimizador::obterAlcancaveis(Automato& afd) {
    set<int> alcancaveis;
    queue<int> fila;
    
    fila.push(afd.estadoInicial);
    alcancaveis.insert(afd.estadoInicial);
    
    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();
        
        for (char simb : afd.alfabeto) {
            int dest = obterDestino(afd, atual, simb);
            if (dest != -1 && alcancaveis.find(dest) == alcancaveis.end()) {
                alcancaveis.insert(dest);
                fila.push(dest);
            }
        }
    }
    return alcancaveis;
}

// Retorna os estados que têm algum caminho até a um estado final
set<int> Minimizador::obterVivos(Automato& afd) {
    set<int> vivos;
    queue<int> fila;
    
    // Passo 1: Todos os estados finais estão vivos por natureza
    for (int i = 0; i < afd.estadosFinais.size(); i++) {
        if (!afd.estadosFinais[i].empty()) {
            vivos.insert(i);
            fila.push(i);
        }
    }
    
    // Passo 2: Pesquisa inversa. Quem consegue chegar aos vivos, também está vivo.
    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();
        
        for (auto& t : afd.transicoes) {
            if (t.destino == atual && vivos.find(t.origem) == vivos.end()) {
                vivos.insert(t.origem);
                fila.push(t.origem);
            }
        }
    }
    return vivos;
}

Automato Minimizador::minimizar(Automato& afd) {
    Automato afdLimpo;
    
    set<int> alcancaveis = obterAlcancaveis(afd);
    set<int> vivos = obterVivos(afd);
    
    // Mapeamento dos IDs antigos para os novos IDs no autómato limpo
    map<int, int> mapaEstados;
    
    // Garantir que o estado inicial é o 0 no novo autómato
    mapaEstados[afd.estadoInicial] = afdLimpo.estadoInicial;
    
    // Adicionar os restantes estados que são SIMULTANEAMENTE alcançáveis e vivos
    for (int i = 0; i < afd.estadosFinais.size(); i++) {
        if (i != afd.estadoInicial && alcancaveis.count(i) && vivos.count(i)) {
            mapaEstados[i] = afdLimpo.adicionarEstado();
        }
    }
    
    // Reconstruir as transições apenas com os estados válidos
    for (auto const& [antigo, novo] : mapaEstados) {
        // Transições
        for (char simb : afd.alfabeto) {
            int destOriginal = obterDestino(afd, antigo, simb);
            if (destOriginal != -1 && mapaEstados.count(destOriginal)) {
                afdLimpo.adicionarTransicao(novo, simb, mapaEstados[destOriginal]);
            }
        }
        
        // Tokens (Finais)
        for (string token : afd.estadosFinais[antigo]) {
            afdLimpo.marcarEstadoFinal(novo, token);
        }
    }
    
    // ==========================================
    // REQUISITO DO PDF: ESTADO DE ERRO
    // ==========================================
    int estadoErro = afdLimpo.adicionarEstado(); // Cria o estado extra
    
    // Preenche todos os "buracos" da matriz a apontar para o erro
    // (incluindo o próprio estado de erro, que fica em loop para si mesmo)
    for (int i = 0; i <= estadoErro; i++) {
        for (char simb : afdLimpo.alfabeto) {
            if (obterDestino(afdLimpo, i, simb) == -1) {
                afdLimpo.adicionarTransicao(i, simb, estadoErro);
            }
        }
    }
    
    return afdLimpo;
}
