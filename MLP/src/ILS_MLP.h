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

            Solucao Construcao();



            void solve();

        };
    
    
}

#endif