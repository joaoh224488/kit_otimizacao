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

void Solucao:: exibir(){
    cout << "Sequence:    ";
    
    for (int i = 0; i < this->sequencia.size() - 1 ; i++){ 
        cout << this->sequencia[i] << " -> ";
    }

    cout << this-> sequencia.back() << endl;

    printf("Cost:   %.2lf \n", this->valorObj);
}


InsertionInfo:: InsertionInfo(){
    this->noInserido = -1;
    this->arestaRemovida = -1;
    this->custo = -1;
}

InsertionInfo:: InsertionInfo(int no, int aresta, double custo){
    this->noInserido = no;
    this->arestaRemovida = aresta;
    this->custo = custo;
}


std::vector<InsertionInfo> InsertionInfo::calcularCustoInsercao(Solucao *s, double **adjMatriz, v_inteiros CL){
    InsertionInfo insInfo;
    std::vector<InsertionInfo> custoInsercao;

    int l = 0;
    for(int a = 0, b = 1; l < s->sequencia.size() - 1; a++, b++){
        int i = s->valorNaPos(a);
        int j = s->valorNaPos(b);
        
        for(auto k : CL){
            insInfo.noInserido = k;
            insInfo.arestaRemovida = a;
            insInfo.custo = (adjMatriz[i][k] + adjMatriz[j][k] - adjMatriz[i][j]);
            custoInsercao.push_back(insInfo);
        }
        l++;
    }

    return custoInsercao;
}

bool InsertionInfo::ordernarPorCusto(InsertionInfo &i, InsertionInfo &j){
    if(i.custo < j.custo){
        return true;
    }
    return false;
}
