/* 
            Trabalho 1 - Grupo 08
            Trabalho feito por: NICOLLY PELEGRINI - 2333481
                                JÚLIA MERSCHBACHER - 2376150
*/
#include <iostream>
#include "pilha.h"
using namespace std;

Pilha::Pilha(){
    topo = -1;

}

bool Pilha::empilhar(int item){
    if(cheia()) return false;

    topo++;
    itens[topo] = item;
    return true;
}

bool Pilha::desempilhar(int &item){
    if(vazia()) return false;

    item = itens[topo];
    topo--;
    return true;
}

bool Pilha::consultarTopo(int &item) const{
    if(vazia()) return false;

    item = itens[topo];
    return true;
}

bool Pilha::vazia() const{
    return tamanho() == 0;

}

bool Pilha::cheia() const{
    return tamanho() == CAPACIDADE;
}

int Pilha::tamanho() const{    
    return topo + 1;
    
}

void Pilha::esvaziar(){
    topo = -1;

}



