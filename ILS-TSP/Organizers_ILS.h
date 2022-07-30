#ifndef ORGANIZERS_ILS_H
#define ORGANIZERS_ILS_H


#include <vector>
#include <bits/stdc++.h>

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
        void setSequence(v_inteiros seq);

        int valorNaPos(int pos); // vetor

        double get_ValorObj();


    };

    struct InsertionInfo{
        int noInserido;
        int arestaRemovida;
        double custo;

        InsertionInfo();
        InsertionInfo(int no, int aresta, double custo);
        static std::vector<InsertionInfo> calcularCustoInsercao(Solucao *s, double **adjMatriz, v_inteiros CL);
        static bool ordernarPorCusto(InsertionInfo &, InsertionInfo &);
    };


    double epsilon(double a, double b);

    bool improve(double value_1, double value_2);

    void inserirNaSolucao(Solucao *s, std::vector <InsertionInfo> infoCusto, int selecionado);

    
}

#endif