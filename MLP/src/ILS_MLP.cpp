#include "ILS_MLP.h"
#include "Organizers_MLP.h"

using namespace ILS_MLP_;
using namespace Organizers_MLP;

ILS_MLP::ILS_MLP(Organizers_MLP::Data_MLP *distancias, int maxIter, int maxIterILS){

    this->distancias = distancias;
    this->maxIter = maxIter;
    this->maxIterILS = maxIterILS;
    this->s_final = new Solucao();
    this->sub_m = subseq_matrix(distancias->n_vertices, std::vector<Subsequence>(distancias->n_vertices));

 }

 ILS_MLP::ILS_MLP (Organizers_MLP::Data_MLP *distancias){
    maxIterILS = distancias->n_vertices >= 150 ? distancias->n_vertices / 2 : distancias->n_vertices;

    ILS_MLP(distancias, 50, maxIterILS);
 }

