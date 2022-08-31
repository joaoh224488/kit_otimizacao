#include <iostream>
#include "Organizers_MLP.h"

using namespace Organizers_MLP;
using namespace std;


Data_MLP :: Data_MLP(int n_vertices, double ** adjMatriz){

    this->n_vertices = n_vertices;

    this->adjMatriz = adjMatriz;
}

void Data_MLP:: printData(){
    cout << "Vertex:    " << this->n_vertices << endl;

    for (int i = 1; i <= this->n_vertices; i++){
        for (int j = 1; j<= this->n_vertices; j++){
            cout << this->adjMatriz[i][j] << "      ";
        }
        cout << endl;
    }
    cout << endl;
}

Solucao:: Solucao(){
    this->sequencia = {};
    this->valorObj = 0.0;
}


void Solucao:: calcularValorObj(Data_MLP *distancias){
    double custo = 0.0;
    int n = distancias->n_vertices;

    for (int i = 0; i < n; i++){
        custo += (n - i) * distancias->adjMatriz[sequencia[i]][sequencia[i + 1]];
    }

    this->valorObj = custo;
}

void Solucao:: exibirSolucao(){
    cout << "Sequence:    ";
    
    for (int i = 0; i < this->sequencia.size() - 1 ; i++){ 
        cout << this->sequencia[i] << " -> ";
    }

    cout << this-> sequencia.back() << endl;

    printf("Cost:   %.2lf \n", this->valorObj);
}