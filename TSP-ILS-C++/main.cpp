
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


    //Solucao->Construcao();
    //Solucao->exibirSolucao();
    //bool a = Solucao->bestImprovementSwap();
    Solucao->solve();
    Solucao->exibirSolucao();

    cout << endl;
 
    
    return 0;
}