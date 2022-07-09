

#include <iostream>
#include <vector>
#include "ILS.h"
#include "Data.h"

using namespace std;

int main(){
    matriz adjMatriz =    {
            {0, 2, 1, 4, 9, 1},
            {2, 0, 5, 9, 7, 2},
            {1, 5, 0, 3, 8, 6},
            {4, 9, 3, 0, 2, 5},
            {9, 7, 8, 2, 0, 2},
            {1, 2, 6, 5, 2, 0}
        };


    Data *dist = new Data(adjMatriz);

    //dist->show_data();

    ILS *Solucao =  new ILS(dist, 50, 1);

    Solucao->solve();
    Solucao->exibirSolucao();

    return 0;
}