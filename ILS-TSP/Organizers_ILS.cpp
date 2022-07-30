#include <iostream>
#include <limits>
#include <cmath>
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

    cout.precision(numeric_limits<double> :: max_digits10);

    cout << "Sequence:    ";
    
    for (int i = 0 ; i < this->sequencia.size() - 1 ; i++){ 
        cout << this->sequencia[i] << " -> ";
    }

    cout << this-> sequencia.back() << endl;

    printf("Cost:   %.2lf \n", this->valorObj);
     

}


int Solucao:: valorNaPos(int pos){
    return this->sequencia[pos];
}

void Solucao:: setSequence(v_inteiros seq){
    this->sequencia.assign(seq.begin(), seq.end());
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
    vector<InsertionInfo> custoInsercao;

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

void Organizers::inserirNaSolucao(Solucao *s, std::vector <InsertionInfo> infoCusto, int selecionado){
    s->sequencia.insert(s->sequencia.begin() + infoCusto[selecionado].arestaRemovida + 1, infoCusto[selecionado].noInserido);
}


double Organizers::epsilon(double a, double b) {
    return fabs(a + b) * numeric_limits<double>::epsilon() * 15;
};

bool Organizers::improve(double value_1, double value_2) {
    return (value_1 - value_2) > Organizers::epsilon(value_1, value_2);
};