#ifndef ILS_MLP_H
#define ILS_MLP_H

#include "Organizers_MLP.h"

using namespace Organizers_MLP;

namespace ILS_MLP_{

    class ILS_MLP
    {
        private:
            Organizers_MLP::Data_MLP *distancias;
            Solucao *s_final;
            subseq_matrix sub_m;
            int maxIter;
            int maxIterILS;

        public:
            ILS_MLP(Organizers_MLP::Data_MLP *distancias);
            ILS_MLP(Organizers_MLP::Data_MLP *distancias, int maxIter, int maxIterILS);

            void exibirSolucao(){s_final->exibir();};
            double distanciaEntreVertices(Solucao *s, int a, int b) {return distancias->adjMatriz[s->sequencia[a]][s->sequencia[b]];};

            Solucao Construcao();
            Solucao perturbacao(Solucao *s);

            void BuscaLocal(Solucao *s);

            void swap(Solucao *s, int i, int j) {std::swap(s->sequencia[i], s->sequencia[j]) ;};
            bool bestImprovementSwap(Solucao *s);
            double calculateSwapCost(Solucao *s, int i, int j);
            void updateAfterSwap(Solucao * s, int i, int j);



            void twoOpt(Solucao *s, int i, int j) {reverse(s->sequencia.begin() + i, s->sequencia.begin() + j + 1);};

            void solve();
            void test();

        };
    
    
}

#endif