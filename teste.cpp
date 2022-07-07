

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

    int vertices = 6;

    Data *dist = new Data(vertices, adjMatriz);

    dist->show_data();

    vector <int> path = {1, 5, 4, 6, 3, 2, 1};

    for (int i = 0; i < path.size(); i ++){
        path[i] = path[i] - 1;
    }

    //ILS *Solucao =  new ILS(dist, path);

    //Solucao->exibirSolucao();


  //  cout << Solucao->calcularValorObj() << endl;

    ILS *Solucao =  new ILS(dist);

    Solucao->Construcao();
    

    Solucao->exibirSolucao();


    cout << Solucao->calcularValorObj() << endl;

    Solucao->BuscaLocal();

    Solucao->exibirSolucao();
    
    cout << Solucao->calcularValorObj() << endl;


    return 0;
}