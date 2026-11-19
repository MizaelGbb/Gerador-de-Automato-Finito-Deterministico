#include "automato.h"
#include <sstream>

// Construtor
Automato::Automato() {
    estadoInicial = 0;
    adicionarEstado();
}

// Adiciona um novo estado
int Automato::adicionarEstado() {
    int novoEstado = estadosFinais.size();

    estadosFinais.push_back(vector<string>());

    return novoEstado;
}

// Adiciona um símbolo ao alfabeto
void Automato::adicionarSimbolo(char simbolo) {

    // Verifica se o símbolo já existe
    for (int i = 0; i < alfabeto.size(); i++) {
        if (alfabeto[i] == simbolo) {
            return;
        }
    }

    // Se não existir, adiciona
    alfabeto.push_back(simbolo);
}

// Adiciona uma transição normal
void Automato::adicionarTransicao(int origem, char simbolo, int destino) {

    // Verifica se os estados existem
    if (origem < 0 || destino < 0 ||
        origem >= estadosFinais.size() ||
        destino >= estadosFinais.size()) {

        cout << "Erro: estado inexistente!" << endl;
        return;
    }

    // Garante que o símbolo esteja no alfabeto
    adicionarSimbolo(simbolo);

    Transicao nova;

    nova.origem = origem;
    nova.simbolo = simbolo;
    nova.destino = destino;

    transicoes.push_back(nova);
}

// Adiciona uma transição vazia (epsilon)
void Automato::adicionarTransicaoVazia(int origem, int destino) {

    // Verifica se os estados existem
    if (origem < 0 || destino < 0 ||
        origem >= estadosFinais.size() ||
        destino >= estadosFinais.size()) {

        cout << "Erro: estado inexistente!" << endl;
        return;
    }

    TransicaoVazia nova;

    nova.origem = origem;
    nova.destino = destino;

    transicoesVazias.push_back(nova);
}

// Marca um estado como final
void Automato::marcarEstadoFinal(int estado, string token) {

    if (estado < 0 || estado >= estadosFinais.size()) {
        cout << "Erro: estado inexistente!" << endl;
        return;
    }

    // Evita adicionar o mesmo token duas vezes
    for (int i = 0; i < estadosFinais[estado].size(); i++) {
        if (estadosFinais[estado][i] == token) {
            return;
        }
    }

    estadosFinais[estado].push_back(token);
}

// Exibe a matriz de transições
void Automato::exibirMatriz() {

    cout << "\n========== AUTOMATO ==========\n";

    cout << "Estado inicial: q" << estadoInicial << endl;

    cout << "\nAlfabeto: ";

    for (int i = 0; i < alfabeto.size(); i++) {
        cout << alfabeto[i] << " ";
    }

    cout << "\n\n";

    // Cabeçalho da matriz
    cout << "Estado\t";

    for (int i = 0; i < alfabeto.size(); i++) {
        cout << alfabeto[i] << "\t";
    }

    cout << "epsilon\tFinal" << endl;

    // Percorre todos os estados
    for (int estado = 0; estado < estadosFinais.size(); estado++) {

        // Identifica o estado inicial
        if (estado == estadoInicial) {
            cout << "->q" << estado << "\t";
        } else {
            cout << "q" << estado << "\t";
        }

        // Percorre cada símbolo do alfabeto
        for (int i = 0; i < alfabeto.size(); i++) {

            char simbolo = alfabeto[i];
            bool encontrou = false;

            // Procura transições com esse símbolo
            for (int j = 0; j < transicoes.size(); j++) {

                if (transicoes[j].origem == estado &&
                    transicoes[j].simbolo == simbolo) {

                    if (encontrou) {
                        cout << ",";
                    }

                    cout << "q" << transicoes[j].destino;

                    encontrou = true;
                }
            }

            if (!encontrou) {
                cout << "-";
            }

            cout << "\t";
        }

        // Exibe transições epsilon
        bool encontrouEpsilon = false;

        for (int i = 0; i < transicoesVazias.size(); i++) {

            if (transicoesVazias[i].origem == estado) {

                if (encontrouEpsilon) {
                    cout << ",";
                }

                cout << "q" << transicoesVazias[i].destino;

                encontrouEpsilon = true;
            }
        }

        if (!encontrouEpsilon) {
            cout << "-";
        }

        cout << "\t";

        // Exibe os tokens associados ao estado final
        if (estadosFinais[estado].empty()) {
            cout << "-";
        } else {

            for (int i = 0; i < estadosFinais[estado].size(); i++) {

                if (i > 0) {
                    cout << ",";
                }

                cout << estadosFinais[estado][i];
            }
        }

        cout << endl;
    }

    cout << "==============================\n";
}

void Automato::adicionarGramatica(const vector<string>& regras, string nomeToken) {

    if (regras.empty()) {
        return;
    }

    // Relaciona cada não terminal ao seu estado
    map<string, int> estadosGramatica;

    // O primeiro não terminal utiliza o estado inicial do autômato
    string inicial = regras[0].substr(0, regras[0].find("::="));

    size_t inicio = inicial.find_first_not_of(" \t");
    size_t fim = inicial.find_last_not_of(" \t");

    inicial = inicial.substr(inicio, fim - inicio + 1);

    estadosGramatica[inicial] = estadoInicial;

    // Percorre todas as produções
    for (const string& regra : regras) {

        size_t posicao = regra.find("::=");

        if (posicao == string::npos) {
            continue;
        }

        // Identifica o não terminal da esquerda
        string origem = regra.substr(0, posicao);

        // Remove espaços das extremidades
        size_t inicio = origem.find_first_not_of(" \t");
        size_t fim = origem.find_last_not_of(" \t");

        origem = origem.substr(inicio, fim - inicio + 1);

        if (estadosGramatica.find(origem) == estadosGramatica.end()) {
            estadosGramatica[origem] = adicionarEstado();
        }
        int estadoOrigem = estadosGramatica[origem];

        // Obtém as alternativas da produção
        string alternativas = regra.substr(posicao + 3);

        stringstream ss(alternativas);
        string alternativa;

       while (getline(ss, alternativa, '|')) {

        // Remove espaços da alternativa
        size_t inicio = alternativa.find_first_not_of(" \t");
        size_t fim = alternativa.find_last_not_of(" \t");

        if (inicio == string::npos) {
            continue;
        }

        alternativa = alternativa.substr(inicio, fim - inicio + 1);

        // Verifica se é epsilon
        if (alternativa == "ε") {
            marcarEstadoFinal(estadoOrigem, nomeToken);
            continue;
        }

        // CORREÇÃO: Se a alternativa começa diretamente com um não-terminal (ex: <B>)
        if (alternativa[0] == '<') {
            size_t posAbertura = alternativa.find('<');
            size_t posFechamento = alternativa.find('>');

            string destino = alternativa.substr(
                posAbertura,
                posFechamento - posAbertura + 1
            );

            // Cria o estado do não terminal, se necessário
            if (estadosGramatica.find(destino) == estadosGramatica.end()) {
                estadosGramatica[destino] = adicionarEstado();
            }

            int estadoDestino = estadosGramatica[destino];

            // Adiciona uma transição vazia (epsilon), pois nenhum terminal foi consumido
            adicionarTransicaoVazia(estadoOrigem, estadoDestino);
            continue;
        }

        // Caso contrário, temos um terminal no início (ex: a <B> ou apenas a)
        char simbolo = alternativa[0];

        size_t posAbertura = alternativa.find('<');
        size_t posFechamento = alternativa.find('>');

        int estadoDestino;

        if (posAbertura != string::npos &&
            posFechamento != string::npos) {

            string destino = alternativa.substr(
                posAbertura,
                posFechamento - posAbertura + 1
            );

            // Cria o estado do não terminal, se necessário
            if (estadosGramatica.find(destino) == estadosGramatica.end()) {
                estadosGramatica[destino] = adicionarEstado();
            }

            estadoDestino = estadosGramatica[destino];

        } else {

            // Caso não exista não terminal, cria um estado final
            estadoDestino = adicionarEstado();
            marcarEstadoFinal(estadoDestino, nomeToken);
        }

        // Adiciona a transição normal com o símbolo terminal
        adicionarTransicao(estadoOrigem, simbolo, estadoDestino);
    }
    }
}

void Automato::adicionarToken(string token, string nomeToken) {

    int estadoAtual = estadoInicial;

    // Percorre cada caractere da palavra
    for (int i = 0; i < token.size(); i++) {

        // Cria um novo estado
        int novoEstado = adicionarEstado();

        // Cria uma transição entre os estados
        adicionarTransicao(estadoAtual, token[i], novoEstado);

        // Atualiza o estado atual
        estadoAtual = novoEstado;
    }

    // Marca o último estado como final
    marcarEstadoFinal(estadoAtual, nomeToken);

}