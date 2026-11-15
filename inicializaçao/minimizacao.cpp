#include "minimizacao.h"
#include <algorithm>

using namespace std;

int Minimizador::obterDestino(Automato& afd, int estadoOrigem, char simbolo) {
    for (auto& t : afd.transicoes) {
        if (t.origem == estadoOrigem && t.simbolo == simbolo) {
            return t.destino;
        }
    }
    return -1; // -1 representa um estado de erro/limbo (não tem transição)
}

int Minimizador::descobrirGrupo(const vector<vector<int>>& particao, int estado) {
    if (estado == -1) return -1;
    for (int i = 0; i < particao.size(); i++) {
        if (find(particao[i].begin(), particao[i].end(), estado) != particao[i].end()) {
            return i;
        }
    }
    return -1;
}

Automato Minimizador::minimizar(Automato& afd) {
    // PASSO 1: Partição inicial
    // Agrupamos os estados pelas suas "saídas" (tokens). 
    // Estados não-finais (vetor vazio) ficam no mesmo grupo.
    // Estados finais que geram tokens diferentes ficam em grupos diferentes.
    map<vector<string>, vector<int>> separacaoInicial;
    for (int i = 0; i < afd.estadosFinais.size(); i++) {
        separacaoInicial[afd.estadosFinais[i]].push_back(i);
    }
    
    vector<vector<int>> particao;
    for (auto& par : separacaoInicial) {
        particao.push_back(par.second);
    }

    // PASSO 2: Refinamento dos grupos
    bool houveDivisao = true;
    while (houveDivisao) {
        houveDivisao = false;
        vector<vector<int>> novaParticao;

        for (auto& grupo : particao) {
            if (grupo.size() <= 1) {
                novaParticao.push_back(grupo); // Grupo unitário não pode ser mais dividido
                continue;
            }

            // Mapeia uma "assinatura de transição" para os estados que a possuem.
            // A assinatura é um vetor indicando para qual GRUPO cada estado vai ao ler cada símbolo.
            map<vector<int>, vector<int>> divisoes;

            for (int estado : grupo) {
                vector<int> assinatura;
                for (char simb : afd.alfabeto) {
                    int destino = obterDestino(afd, estado, simb);
                    int grupoDoDestino = descobrirGrupo(particao, destino);
                    assinatura.push_back(grupoDoDestino);
                }
                divisoes[assinatura].push_back(estado);
            }

            // Os estados com a mesma assinatura ficam juntos. 
            for (auto& par : divisoes) {
                novaParticao.push_back(par.second);
            }

            // Se um grupo original gerou 2 ou mais divisões, o algoritmo precisa rodar de novo
            if (divisoes.size() > 1) {
                houveDivisao = true;
            }
        }
        particao = novaParticao;
    }

    // PASSO 3: Construção do novo Autômato Minimizado
    Automato afdMinimo;
    
    // Identificar qual grupo contém o estado inicial original
    int grupoInicial = descobrirGrupo(particao, afd.estadoInicial);
    
    // Para facilitar a leitura, vamos mapear o id do Grupo para o novo Id de estado do autômato mínimo
    map<int, int> mapaGruposParaEstados;
    
    // O grupo que contém o estado inicial original DEVE ser o estado 0 do novo autômato
    mapaGruposParaEstados[grupoInicial] = afdMinimo.estadoInicial;

    // Cadastra os outros grupos
    for (int i = 0; i < particao.size(); i++) {
        if (i != grupoInicial) {
            mapaGruposParaEstados[i] = afdMinimo.adicionarEstado();
        }
    }

    // Reconstruir transições e marcações finais usando o primeiro estado de cada grupo como representante
    for (int i = 0; i < particao.size(); i++) {
        int idNovoEstado = mapaGruposParaEstados[i];
        int estadoRepresentante = particao[i][0]; // Pegamos um porta-voz do grupo

        // Transcriar transições
        for (char simb : afd.alfabeto) {
            int destOriginal = obterDestino(afd, estadoRepresentante, simb);
            if (destOriginal != -1) {
                int grupoDestino = descobrirGrupo(particao, destOriginal);
                afdMinimo.adicionarTransicao(idNovoEstado, simb, mapaGruposParaEstados[grupoDestino]);
            }
        }

        // Transcriar marcações de estado final
        for (string token : afd.estadosFinais[estadoRepresentante]) {
            afdMinimo.marcarEstadoFinal(idNovoEstado, token);
        }
    }

    return afdMinimo;
}
