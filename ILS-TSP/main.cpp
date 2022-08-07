
#include <iostream>


#include "ILS.h"
#include "Organizers_ILS.h"
#include "readData.h"

using namespace ILS_Class;
using namespace std;

int main(int argc, char *argv[]){
    double ** matrizAdj;
    int dimension;

    readData(argc, argv, &dimension, &matrizAdj);
    
    Data *dist = new Data(dimension, matrizAdj);

    ILS *Solucao =  new ILS(dist);

    

    /*
    for (int i = 1; i <=14; i ++){
        cout << i << ": " ;
        for (int j = 1; j <= 14; j++){
            cout << matrizAdj[i][j] << ", ";
        }
        cout << endl << endl;
    }
    */
    
    
    Solucao->solve();
    Solucao->exibirSolucao();
    

/*
    Organizers::Solucao tst = Solucao->Construcao();
    


    tst.exibirSequencia();

    tst = Solucao->perturbacao(&tst);

    tst.exibirSequencia();

    tst.calcularValorObj(dist);
    cout << endl <<"Valor testado: " << tst.valorObj << endl;


    */
   // cout << Solucao->delta << endl;

    cout << endl;

    
    return 0;
}