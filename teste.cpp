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

    vector <int> path = {1, 6, 3, 2, 5, 4};

    ILS *Solucao =  new ILS(dist, path);


    Solucao->exibirSolucao();

    

    


/*
    vector <int> path = {1, 6, 3, 2, 5, 4, 1};


    ILS *sol = new ILS(path);

    sol->exibirSolucao();

    cout<< sol->get_valor_obj()<< endl;

    */
}