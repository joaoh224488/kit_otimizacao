#include <iostream>
#include "Organizers_ILS.h"

using namespace Organizers;
using namespace std;

Data :: Data(int n_vertices, double ** adjMatriz){

    this->n_vertices = n_vertices;

    this->adjMatriz = adjMatriz;
}

Solucao :: Solucao(){
    this->sequencia = {};
    this->valorObj = 0.0;
}

void Solucao:: calcularValorObj(Data *distancias)
{
    this->valorObj = 0.0;
    for(int i = 0; i < sequencia.size() - 1; i++){

        valorObj += distancias->adjMatriz[sequencia[i]][sequencia[i+1]];
        
    }

}


void Solucao:: exibirSequencia(){

    cout << "Sequence:    ";
    
    for (int i = 0 ; i < this->sequencia.size() - 1 ; i++){ 
        cout << this->sequencia[i] << " -> ";
    }

    cout << this-> sequencia.back() << endl;

        cout << "Cost:  " << this->valorObj << endl; 

}


int Solucao:: valorNaPos(int pos){
    return this->sequencia[pos];
}

void Solucao:: setSequence(v_inteiros seq){
    this->sequencia = seq;
}