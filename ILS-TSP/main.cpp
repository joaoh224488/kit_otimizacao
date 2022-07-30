
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
    
  // Solucao->solve();
   //Solucao->exibirSolucao();

   v_inteiros max = {1, 2, 14, 3, 4, 5, 6, 12, 7, 13, 11, 9, 10, 8, 1};
   Solucao->s_final->sequencia = max;
   Solucao->s_final->calcularValorObj(dist);
   Solucao->s_final->exibirSequencia();
    
   // cout << Solucao->delta << endl;

    cout << endl;
 
    
    return 0;
}