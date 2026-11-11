/* 
            Trabalho 1 - Grupo 08
            Trabalho feito por: NICOLLY PELEGRINI - 2333481
                                JÚLIA MERSCHBACHER - 2376150
*/
#include "fila.h"
using namespace std;

Fila::Fila(){
    quantidade = 0;
    inicio = 0;
    fim = 0;

}

void Fila::esvaziar(){ 
    quantidade = 0;
    inicio = 0;
    fim = 0;

}

bool Fila::vazia() const{
    return quantidade == 0;

}

bool Fila::cheia() const{
    return quantidade == CAPACIDADE;

}

int Fila::tamanho() const{
    return quantidade;

}   

bool Fila::consultarFrente(int &item) const{
    if(vazia()) return false;

    item = itens[inicio];
    return true;
}

bool Fila::enfileirar(int item){
    if(cheia()) return false;

    itens[fim] = item;
    fim = (fim + 1) % CAPACIDADE; // Se fim é 0, (0 + 1) % 40 = 1. Se fim é 39, (39 + 1) % 40 = 0.
    quantidade++;
    return true;
}

bool Fila::desenfileirar(int &item){
    if(vazia()) return false;

    item = itens[inicio];
    
    inicio = (inicio +1) % CAPACIDADE;
    quantidade--;
    return true;
}
