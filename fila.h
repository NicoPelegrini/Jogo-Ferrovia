/*
 * fila.h - TAD Fila estatica circular de inteiros
 * Estrutura de Dados - Centro Universitario Barao de Maua - Trabalho I (2026)
 *
 * ARQUIVO FORNECIDO PELO PROFESSOR - NAO ALTERE ESTE ARQUIVO.
 * Implemente as operacoes em fila.cpp.
 *
 * A fila armazena inteiros: o ID de um elemento do jogo (a posicao do
 * registro no vetor lido do arquivo CSV) ou o numero de um jogador.
 * Os itens ficam em um vetor circular de capacidade fixa (CAPACIDADE):
 * depois da ultima posicao do vetor, os indices voltam para a posicao 0.
 * Requer C++11 ou superior.
 */
#ifndef FILA_H
#define FILA_H

class Fila {
public:
    // Numero maximo de itens da fila.
    static const int CAPACIDADE = 40;

    // Cria uma fila vazia.
    Fila();

    // Insere item no fim da fila.
    // Retorna false (sem alterar a fila) se a fila estiver cheia.
    bool enfileirar(int item);

    // Remove o item da frente e o copia para item.
    // Retorna false (sem alterar item) se a fila estiver vazia.
    bool desenfileirar(int &item);

    // Copia o item da frente para item, sem remove-lo.
    // Retorna false (sem alterar item) se a fila estiver vazia.
    bool consultarFrente(int &item) const;

    // Retorna true se a fila nao tiver itens.
    bool vazia() const;

    // Retorna true se a fila tiver CAPACIDADE itens.
    bool cheia() const;

    // Retorna a quantidade de itens da fila.
    int tamanho() const;

    // Remove todos os itens. Pos: vazia() == true.
    void esvaziar();

    // Atencao: copiar uma Fila (atribuicao ou passagem por valor) copia o
    // vetor inteiro, e as alteracoes feitas na copia nao afetam a original.
    // Para alterar uma fila dentro de uma funcao, passe-a por referencia (Fila &).

private:
    int itens[CAPACIDADE];  // vetor circular
    int inicio;             // indice do item da frente
    int fim;                // indice onde entrara o proximo item
    int quantidade;         // numero de itens
};

#endif
