/*
 * pilha.h - TAD Pilha estatica de inteiros
 * Estrutura de Dados - Centro Universitario Barao de Maua - Trabalho I (2026)
 *
 * ARQUIVO FORNECIDO PELO PROFESSOR - NAO ALTERE ESTE ARQUIVO.
 * Implemente as operacoes em pilha.cpp.
 *
 * A pilha armazena inteiros: o ID de um elemento do jogo (a posicao do
 * registro no vetor lido do arquivo CSV) ou o numero de um jogador.
 * Os itens ficam em um vetor de capacidade fixa (CAPACIDADE).
 * Requer C++11 ou superior.
 */
#ifndef PILHA_H
#define PILHA_H

class Pilha {
public:
    // Numero maximo de itens da pilha.
    static const int CAPACIDADE = 40;

    // Cria uma pilha vazia.
    Pilha();

    // Insere item no topo.
    // Retorna false (sem alterar a pilha) se a pilha estiver cheia.
    bool empilhar(int item);

    // Remove o item do topo e o copia para item.
    // Retorna false (sem alterar item) se a pilha estiver vazia.
    bool desempilhar(int &item);

    // Copia o item do topo para item, sem remove-lo.
    // Retorna false (sem alterar item) se a pilha estiver vazia.
    bool consultarTopo(int &item) const;

    // Retorna true se a pilha nao tiver itens.
    bool vazia() const;

    // Retorna true se a pilha tiver CAPACIDADE itens.
    bool cheia() const;

    // Retorna a quantidade de itens da pilha.
    int tamanho() const;

    // Remove todos os itens. Pos: vazia() == true.
    void esvaziar();

    // Atencao: copiar uma Pilha (atribuicao ou passagem por valor) copia o
    // vetor inteiro, e as alteracoes feitas na copia nao afetam a original.
    // Para alterar uma pilha dentro de uma funcao, passe-a por referencia (Pilha &).

private:
    int itens[CAPACIDADE];  // itens[0] e o fundo da pilha
    int topo;               // indice do item do topo (-1 se vazia)
};

#endif