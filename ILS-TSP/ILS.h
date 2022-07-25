#ifndef ILS_H
#define ILS_H

#include "Organizers_ILS.h"

using namespace Organizers;

namespace ILS_Class{

    class ILS
    {
    private:
        Data *distancias;
        Solucao *s;
        int maxIter;
        int maxIterILS;

    public:
        ILS(Data *distancias);
        ILS (Data *distancias, int maxIter, int maxIterILS);
        ~ILS();


        double get_current_cost();
        double distanciaEntreVertices(int a, int b);
        v_inteiros get_current_vector();

        void exibirSolucao();
        void Construcao();

        void swap(int i, int j);
        double calculateSwapCost(int i, int j);
        bool bestImprovementSwap();

        void twoOpt(int i, int j);
        double calculateTwoOptCost(int i, int j);
        bool bestImprovementTwoOpt();

        void orOpt(int i, int j, int size);
        double calculateOrOptCost(int primeiro_indice, int segundo_indice, int size);
        bool bestImprovementOrOpt(int size);

        void perturbacao();
        double calculatePerturbacaoCost(int i, int size_i, int j, int size_j);

        void BuscaLocal();
        void solve();



    };
    
    


}


#endif