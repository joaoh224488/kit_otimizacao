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

    v_inteiros test;

    for (int i = 1; i <= dimension; i++){
        test.push_back(i);
    }

    test.push_back(1);


    Solucao * seq = new Solucao();
    seq->setSequence(test);

    seq->calcularValorObj(dist);

    seq->exibirSolucao();

    cout << endl;

    ILS_MLP *x = new ILS_MLP(dist, 30, 40);


    
    return 0;
}