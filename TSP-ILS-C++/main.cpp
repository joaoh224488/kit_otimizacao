
#include <iostream>


#include "ILS.h"
#include "Data.h"
#include "readData.h"

using namespace N;
using namespace std;

int main(int argc, char *argv[]){
    double ** matrizAdj;
    int dimension;

    readData(argc, argv, &dimension, &matrizAdj);
    
    Data *dist = new Data(dimension, matrizAdj);

    ILS *Solucao =  new ILS(dist);


    Solucao->Construcao();
    Solucao->exibirSolucao();
    //bool a = Solucao->bestImprovementSwap();
    //Solucao->solve();

    cout << "custo: " <<Solucao->calculateSwapCost(2, 4) << endl;
    Solucao->sequencia = Solucao->swap(Solucao->sequencia, 2, 4);
    Solucao->valorObj = Solucao->calcularValorObj(Solucao->sequencia);
    Solucao->exibirSolucao();
    
    
   // cout << Solucao->delta << endl;

    cout << endl;


    
    return 0;
}