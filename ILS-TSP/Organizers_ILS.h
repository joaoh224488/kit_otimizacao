#ifndef ORGANIZERS_ILS_H
#define ORGANIZERS_ILS_H


#include <vector>

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

    
}

#endif