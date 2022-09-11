#ifndef ORGANIZERS_ILS_H
#define ORGANIZERS_ILS_H

#include <vector>
#include <bits/stdc++.h>
#include <limits>
#include <cmath>


typedef std::vector<int> v_inteiros;

namespace Organizers_MLP{
    
    struct Data_MLP
    {
        int n_vertices;
        double ** adjMatriz;

        Data_MLP(int n_vertices, double ** adjMatriz);
        void printData();
    };
    
    struct Solucao
    {
 
        v_inteiros sequencia;
        double valorObj;

        Solucao();

        void calcularValorObj(Data_MLP *);
        void exibir();
        void setSequence(v_inteiros seq) {this->sequencia.assign(seq.begin(), seq.end());};

        int valorNaPos(int pos) {return this->sequencia[pos];}; // vetor
        double get_ValorObj() {return this->valorObj;};
        int get_size() {return this->sequencia.size();};

    
    };

    struct Subsequence{
        
        double T = 0, C = 0;
        int W = 0, first = 0, last = 0;  

        inline void exibir_subseq(){
            std:: cout << "T:   " << this->T << std:: endl;
            std:: cout << "C:   " << this->C << std:: endl;
            std:: cout << "W:   " << this->W << std:: endl;
            std:: cout << "first:   " << this->first << std:: endl;
            std:: cout << "last:   " << this->last << std:: endl << std:: endl;
        }         
    };

    typedef std::vector<std::vector<Subsequence>> subseq_matrix;



    inline static Subsequence Concatenate(Subsequence &sigma_1, Subsequence &sigma_2, Data_MLP *m){

        Subsequence sigma;

        double temp = m->adjMatriz[sigma_1.last][sigma_2.first];

        sigma.W = sigma_1.W + sigma_2.W;
        sigma.T = sigma_1.T + temp + sigma_2.T;
        sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;

        sigma.first = sigma_1.first;
        sigma.last = sigma_2.last;


        return sigma;
    }


    inline void UpdateAllSubseq(Solucao * s, subseq_matrix &s_matrix, Data_MLP *m){

        int n = s->sequencia.size();

        for (int i = 0; i < n; i++){
       
            s_matrix[i][i].W = (i > 0);
            s_matrix[i][i].C = 0;
            s_matrix[i][i].T = 0;
            s_matrix[i][i].first = s->sequencia[i];
            s_matrix[i][i].last = s->sequencia[i];

           // s_matrix[i][i].exibir_subseq();
        }

        for (int i = 0; i < n; i++){
            for (int j = i + 1; j < n; j++){
                s_matrix[i][j] = Concatenate(s_matrix[i][j-1], s_matrix[j][j], m);
                //s_matrix[i][j].exibir_subseq();
            }
        }

        for (int i = n - 1; i >= 0; i--){
            for (int j = i -1; j >= 0; j--){
                s_matrix[i][j] = Concatenate(s_matrix[i][j+1], s_matrix[j][j], m);
            }
        }
    }

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
        return (value_1 - value_2) > Organizers_MLP::epsilon(value_1, value_2);
    }
    inline void inserirNaSolucao(Solucao *s, std::vector <InsertionInfo> infoCusto, int selecionado){
        s->sequencia.insert(s->sequencia.begin() + infoCusto[selecionado].arestaRemovida + 1, infoCusto[selecionado].noInserido);
    }




}
#endif