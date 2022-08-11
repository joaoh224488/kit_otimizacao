#ifndef ILS_H
#define ILS_H

#include "Organizers_ILS.h"

using namespace Organizers;

namespace ILS_Class{

    class ILS
    {
    private:
        Data *distancias;
        Solucao *s_final;
        int maxIter;
        int maxIterILS;

    public:
        ILS(Data *distancias);
        ILS (Data *distancias, int maxIter, int maxIterILS);
        ~ILS();


        double get_current_cost();
        double distanciaEntreVertices(Solucao *s, int a, int b) {return distancias->adjMatriz[s->valorNaPos(a)][s->valorNaPos(b)];};
        v_inteiros get_current_vector();

        void exibirSolucao();
        Solucao Construcao();

        void swap(Solucao *s, int i, int j) {std::swap(s->sequencia[i], s->sequencia[j]) ;};
        double calculateSwapCost(Solucao *s, int i, int j);
        bool bestImprovementSwap(Solucao *s);

        void twoOpt(Solucao *s, int i, int j) {reverse(s->sequencia.begin() + i, s->sequencia.begin() + j + 1);};
        double calculateTwoOptCost(Solucao *s, int i, int j);
        bool bestImprovementTwoOpt(Solucao *s);

        void orOpt(Solucao *s, int i, int j, int size);
        double calculateOrOptCost(Solucao *s, int primeiro_indice, int segundo_indice, int size);
        bool bestImprovementOrOpt(Solucao *s, int size);

        Solucao perturbacao(Solucao *s);
        double calculatePerturbacaoCost(Solucao *s, int i, int size_i, int j, int size_j);

        void BuscaLocal(Solucao *s);
        void solve();



    };
    


}


#endif