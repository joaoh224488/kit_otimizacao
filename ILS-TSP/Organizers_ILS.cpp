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

void Solucao:: alterarValorObj(double delta){
    this->valorObj += delta;
}

void Solucao:: exibirSequencia(){

    cout << "Sequence:    ";
    
    for (int i = 0 ; i < this->sequencia.size() - 1 ; i++){ 
        cout << this->sequencia[i] << " -> ";
    }

    cout << this-> sequencia.back() << endl;

        cout << "Cost:  " << this->valorObj << endl; 

}
void Solucao:: adicionarNo(int no){
    this->sequencia.push_back(no);
}

void Solucao:: retirarUltimoNo(){
    this->sequencia.pop_back();
    }

void Solucao:: inicializarSequencia(int n){
    for (int i = 1; i <= n; i++){
        adicionarNo(i);
    }
}

void Solucao:: inserirValor(int pos, int no){
    this->sequencia.insert(this->sequencia.begin() + pos, no);
}

void Solucao:: apagarNo(int pos){
    this->sequencia.erase(this->sequencia.begin() + pos);
}

void Solucao:: apagarSubseq(int inicio, int fim){
    this->sequencia.erase(this->sequencia.begin() + inicio, this->sequencia.begin() + fim);
}

int Solucao:: valorNaPos(int pos){
    return this->sequencia[pos];
}

void Solucao:: setSequence(v_inteiros seq){
    this->sequencia = seq;
}