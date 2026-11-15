#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Transicao {

    int origem;
    char simbolo;
    int destino;

};

struct TransicaoVazia {
    int origem;
    int destino;
};

class Automato {

private:

    vector<char> alfabeto;

    vector<Transicao> transicoes;

    vector<TransicaoVazia> transicoesVazias;

    vector<vector<string>> estadosFinais;

    int estadoInicial;

public:

    friend class Minimizador;
    friend class Determinizador;

    Automato();

    int adicionarEstado();

    void adicionarSimbolo(char simbolo);

    void adicionarTransicao(int origem, char simbolo, int destino);

    void adicionarTransicaoVazia(int origem, int destino);

    void marcarEstadoFinal(int estado, string token);

    void adicionarToken(string token, string nomeToken);

    void exibirMatriz();

};

#endif
