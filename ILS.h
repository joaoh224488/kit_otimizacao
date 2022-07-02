#ifndef ILS_H
#define ILS_H

#include <iostream>
#include <vector>
#include "Data.h"

class ILS
{
private:
    v_inteiros sequencia;
    int valorObj;
    Data *distancias;
public:
    ILS();
    ILS(Data *distancias);
    ILS(Data *distancias, v_inteiros exemplo); // EXCLUIR HOJE

    void exibirSolucao();
    int calcularValorObj();
};

ILS::ILS(Data *distancias, v_inteiros exemplo)
{
    this->distancias = distancias;                  // EXCLUIR HOJE

    this->sequencia = exemplo;
}

ILS :: ILS(Data *distancias){
    this->distancias = distancias;
}

void ILS:: exibirSolucao(){
    for (int i = 0; i < this->sequencia.size(); i++){
        std::cout << this->sequencia[i] << " -> ";
    }

    std::cout << this->sequencia[0] << std::endl;
            

}

// CONSERTAR VALOR OBJETIVO

int ILS:: calcularValorObj()
{
    this->valorObj = 0;
    for(int i = 0; i < this->sequencia.size() - 1; i++){
                this->valorObj += distancias->adjMatriz[this->sequencia[i]][this->sequencia[i+1]];
            }
            this->valorObj += distancias->adjMatriz[this->sequencia.back()][this->sequencia[0]];

    
    return this->valorObj;

    }






#endif