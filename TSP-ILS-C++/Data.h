#ifndef Data_H
#define Data_H

#include <iostream>
#include <vector>

namespace D{


typedef std::vector<int> v_inteiros;

struct Data
{
        int n_vertices;
        double ** adjMatriz;
        // Construtor:
        Data(int n_vertices, double ** adjMatriz);

};
}


#endif