#include "Data.h"

using namespace D;

Data:: Data(int n_vertices, matriz adjMatriz){

    this->n_vertices = n_vertices;
    this->adjMatriz = adjMatriz;
}

Data:: Data(matriz adjMatriz){

    this->adjMatriz = adjMatriz;

    this->n_vertices = adjMatriz.size();
}

void Data:: show_data(){
    std::cout << std::endl;
    for (int i = 0; i < n_vertices; i++){
        for (int j = 0; j < n_vertices; j++){
            std:: cout << adjMatriz[i][j] <<  "    ";
        }
        std:: cout << std::endl;
    }

}

