#include <iostream>



#include "readData.h"
#include "Organizers_MLP.h"
#include "ILS_MLP.h"


using namespace std;
using namespace Organizers_MLP;
using namespace ILS_MLP_;

int main(int argc, char *argv[]){
    double ** matrizAdj;
    int dimension;

    readData(argc, argv, &dimension, &matrizAdj);
    
    Data_MLP *dist = new Data_MLP(dimension, matrizAdj);
    
    //dist->printData();

    ILS_MLP *x = new ILS_MLP(dist, 15, 100);

    //x->test();
    x->solve();
    x->exibirSolucao();


    
    return 0;
}