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
        void exibirSolucao();
        void setSequence(v_inteiros seq) {this->sequencia.assign(seq.begin(), seq.end());};

        int valorNaPos(int pos) {return this->sequencia[pos];}; // vetor
        double get_ValorObj() {return this->valorObj;};
        int get_size() {return this->sequencia.size();};

    
    };

    struct Subsequence{
        
        double T, C;
        int W, first, last;

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
            
    };

    typedef std::vector<std::vector<Subsequence>> subseq_matrix;


    inline void UpdateAllSubseq(Solucao * s, subseq_matrix &s_matrix, Data_MLP *m){

        int n = s->sequencia.size();

        for (int i = 0; i < n; i++){
            int v = s->sequencia[i];

            s_matrix[i][i].W = (i > 0);
            s_matrix[i][i].C = 0;
            s_matrix[i][i].T = 0;
            s_matrix[i][i].first = s->sequencia[i];
            s_matrix[i][i].last = s->sequencia[i];
        }

        for (int i = 0; i < n; i++){
            for (int j = i + 1; j < n; j++){
                s_matrix[i][j] = Subsequence::Concatenate(s_matrix[i][j-1], s_matrix[j][j], m);
            }
        }

        for (int i = n - 1; i >= 0; i--){
            for (int j = i -1; j >= 0; j--){
                s_matrix[i][j] = Subsequence::Concatenate(s_matrix[i][j+1], s_matrix[j][j], m);
            }
        }

    }


    inline double epsilon(double a, double b){
        return fabs(a + b) * std::numeric_limits<double>::epsilon() * 15;
    }
    inline bool improve(double value_1, double value_2){
        return (value_1 - value_2) > Organizers_MLP::epsilon(value_1, value_2);
    }

}
#endif