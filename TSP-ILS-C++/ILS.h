#ifndef ILS_H
#define ILS_H


#include "Data.h"
using namespace D;


namespace N

{
    class ILS
    {
        public:
            v_inteiros sequencia;
            double valorObj;

            Data *distancias;
            int maxIter;
            int maxIterILS;
            
            double calcularValorObj(v_inteiros sequencia);

            void Construcao();

            v_inteiros swap(v_inteiros sequencia, int primeiro_indice, int segundo_indice);
            double calculateSwapCost(int primeiro_indice, int segundo_indice);
            bool bestImprovementSwap();

            v_inteiros twoOpt(v_inteiros sequencia, int primeiro_indice, int segundo_indice);
            double calculateTwoOptCost(int primeiro_indice, int segundo_indice);
            bool bestImprovementTwoOpt();

            v_inteiros orOpt(v_inteiros sequencia, int primeiro_indice, int segundo_indice, int size);
            double calculateOrOptCost(int primeiro_indice, int segundo_indice, int size);
            bool bestImprovementOrOpt(int size);

            void BuscaLocal();


            v_inteiros perturbacao(v_inteiros seq);
            
            
        public:
            ILS(Data *distancias, int maxIter, int maxIterILS);
            ILS(Data *distancias);
            
            double get_current_cost();
            v_inteiros get_current_vector();

            void exibirSolucao();

            void solve();
};




}


#endif