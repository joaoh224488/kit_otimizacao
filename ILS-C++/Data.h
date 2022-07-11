#ifndef Data_H
#define Data_H

#include <iostream>
#include <vector>

namespace D{

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
}


#endif