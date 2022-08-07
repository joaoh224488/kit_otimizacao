#ifndef ORGANIZERS_ILS_H
#define ORGANIZERS_ILS_H


#include <vector>
#include <bits/stdc++.h>
#include <limits>
#include <cmath>


typedef std::vector<int> v_inteiros;

namespace Organizers{
    
    struct Data
    {
        int n_vertices;
        double ** adjMatriz;
        Data(int n_vertices, double ** adjMatriz);

    };
    

    struct Solucao
    {
        v_inteiros sequencia;
        double valorObj;

        Solucao();

        void calcularValorObj(Data *distancias);
        void exibirSequencia();
        void setSequence(v_inteiros seq) {this->sequencia.assign(seq.begin(), seq.end());};

        int valorNaPos(int pos) {return this->sequencia[pos];}; // vetor

        double get_ValorObj() {return this->valorObj;};


    };

    struct InsertionInfo{
        int noInserido;
        int arestaRemovida;
        double custo;

        InsertionInfo();
        InsertionInfo(int no, int aresta, double custo);
        static std::vector<InsertionInfo> calcularCustoInsercao(Solucao *s, double **adjMatriz, v_inteiros CL);
        static bool ordernarPorCusto(InsertionInfo &, InsertionInfo &) ;
    };


    inline double epsilon(double a, double b){
        return fabs(a + b) * std::numeric_limits<double>::epsilon() * 15;
    }

    inline bool improve(double value_1, double value_2){
        return (value_1 - value_2) > Organizers::epsilon(value_1, value_2);
    }

    inline void inserirNaSolucao(Solucao *s, std::vector <InsertionInfo> infoCusto, int selecionado){
        s->sequencia.insert(s->sequencia.begin() + infoCusto[selecionado].arestaRemovida + 1, infoCusto[selecionado].noInserido);
    }

    
}

#endif