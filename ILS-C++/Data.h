#ifndef Data_H
#define Data_H

#include <iostream>
#include <vector>

typedef std::vector <std:: vector < double>> matriz;
typedef std::vector<int> v_inteiros;

struct Data
{
        int n_vertices;
        matriz adjMatriz;
        // Construtor:
        Data(int n_vertices, matriz adjMatriz);
        Data(matriz adjMatriz);


        // Método para mostrar coisas do objeto:
        void show_data();


};


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



#endif