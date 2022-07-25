#include "ILS.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace Organizers;
using namespace ILS_Class;
using namespace std;

ILS :: ILS (Data *distancias, int maxIter, int maxIterILS){
    this->distancias = distancias;
    this->maxIter = maxIter;
    this->maxIterILS = maxIterILS;
    this->s = new Solucao();

}

ILS :: ILS(Data *distancias){

    this->distancias = distancias;
    this->maxIter = 50;
    this->maxIterILS = distancias->n_vertices >= 150 ? distancias->n_vertices / 2 : distancias->n_vertices;
    this->s = new Solucao();
}


double ILS :: get_current_cost(){
    return this->s->valorObj;
}
double ILS:: distanciaEntreVertices(int a, int b){
    return distancias->adjMatriz[s->valorNaPos(a)][s->valorNaPos(b)];
}

v_inteiros ILS :: get_current_vector(){
    return this->s->sequencia;
}

void ILS:: exibirSolucao(){
    this->s->calcularValorObj(distancias);
    this->s->exibirSequencia();
}

void ILS:: Construcao(){
 
    int n_vertices = this->distancias->n_vertices;

    int q_tour_inicial = 2;

    v_inteiros V;             
    v_inteiros s1;            
    v_inteiros CL;     

    for (int i = 1; i <= n_vertices; i++){

        V.push_back(i);                             
    }

    CL = V;

    std::random_shuffle(CL.begin(), CL.end());

    int chs;                         


    for (int i = 0; i < q_tour_inicial; i++){
        chs = CL[i];

        s1.push_back(chs);

        CL.erase(remove(CL.begin(), CL.end(), chs), CL.end());
    }

    int indice = 0;
    int selecionado;
    double atual;
    double alpha;
    
    std::vector<v_inteiros> arestas;
    std::vector < std::pair <double, int>> custos;
    v_inteiros aux;      

        while (!CL.empty()){
        
        for (int i = 0; i < CL.size(); i++){
            arestas.clear();
            custos.clear();
            for (int j = 0; j < s1.size() - 1; j++){
                
                aux.clear();
                    
                atual += distancias->adjMatriz[s1[j]][CL[i]];
                atual += distancias->adjMatriz[s1[j + 1]][CL[i]];
                atual -= distancias->adjMatriz[s1[j]][s1[j + 1]];

                aux.push_back(j);
                aux.push_back(CL[i]);

                arestas.push_back(aux);
                custos.push_back(std::make_pair(atual, indice));

                atual = 0;
                indice++;
            }

            sort(custos.begin(), custos.end());

            alpha = (double) rand() / RAND_MAX;
            selecionado = rand() % ((int) ceil(alpha * custos.size()));
            
    
        s1.insert(s1.begin() + arestas[selecionado][0], arestas[selecionado][1]);

        int a_excluir;

        for (int i = 0; i < CL.size(); i++){
            if (CL[i] == arestas[selecionado][1]){
                a_excluir = i;
                break;
            }
        }

        CL.erase(CL.begin() + a_excluir);

    }
    }
    
    s1.push_back(s1[0]);

    this->s->setSequence(s1);
    this->s->calcularValorObj(distancias);

}

void ILS:: swap(int i, int j){

    int aux = s->valorNaPos(i);

    s->sequencia[i] = s->sequencia[j];

    s->sequencia[j] = aux;
}

double ILS:: calculateSwapCost(int i, int j){
    double  a_subtrair, a_somar, delta;

    if ((j == i + 1)){
        
        a_subtrair = distanciaEntreVertices(i - 1, i) + distanciaEntreVertices(j, j+ 1);
        a_somar = distanciaEntreVertices(i -1, j) + distanciaEntreVertices(i, j + 1 );
        
    }
    else{
         a_subtrair = distanciaEntreVertices(i - 1, i) + distanciaEntreVertices(i, i + 1) 
                    + distanciaEntreVertices(j -1 , j) + distanciaEntreVertices(j, j+1);



        a_somar = distanciaEntreVertices(i - 1, j) + distanciaEntreVertices(j, i + 1)
                + distanciaEntreVertices(j - 1, i) + distanciaEntreVertices(i, j + 1);
    }

    delta = a_somar - a_subtrair;

    return delta;
}

bool ILS:: bestImprovementSwap(){
    double bestDelta = 0.0, delta = 0.0;
    int best_i, best_j;  
    bool improved = false;

    for (int i = 1; i < s->sequencia.size() - 2; i++){
        for (int j = i + 1; j < s->sequencia.size() -1; j++){
            delta = calculateSwapCost(i, j);

            if (delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
                improved = true;
            }
        }
    }
    if (improved){
        swap(best_i, best_j);
        s->valorObj += bestDelta;
    }

    return improved;
}

void ILS:: twoOpt(int i, int j){
    reverse(s->sequencia.begin() + i, s->sequencia.begin() + j + 1);
}


double ILS:: calculateTwoOptCost(int i, int j){
    double a_subtrair, a_somar, delta;

    a_subtrair = distanciaEntreVertices(i - 1, i) + distanciaEntreVertices(j, j + 1);
    a_somar = distanciaEntreVertices(i - 1, j) + distanciaEntreVertices(i, j + 1);

    delta = a_somar - a_subtrair;

    return delta;
}

bool ILS:: bestImprovementTwoOpt(){
    double bestDelta = 0.0, delta = 0.0;
    int best_i, best_j;  
    bool improved = false;           

    for (int i = 1; i < s->sequencia.size() - 1; i++){
        for (int j = i + 1; j < s->sequencia.size() - 1; j++){

            delta = calculateTwoOptCost(i, j);
        
            if (delta < bestDelta){
                best_i = i;
                best_j = j;
                bestDelta = delta;
                improved = true;
            }
        }
    }
    if (improved){
        twoOpt(best_i, best_j);
        s->valorObj += bestDelta;
    }

    return improved;
}


void ILS:: orOpt(int i, int j, int size){
    v_inteiros bloco(s->sequencia.begin() + i, s->sequencia.begin() + i + size);

    s->sequencia.erase(s->sequencia.begin() + i, s->sequencia.begin() + i + size);
    s->sequencia.insert(s->sequencia.begin() + j, bloco.begin(), bloco.end());

}


double ILS:: calculateOrOptCost(int i, int j, int size){
    double a_subtrair, a_somar, delta;

    if (i > j){
        a_subtrair = distanciaEntreVertices(j - 1, j) + distanciaEntreVertices(i - 1, i)
                   + distanciaEntreVertices(i + size - 1, i + size);

        a_somar = distanciaEntreVertices(j - 1, i) + distanciaEntreVertices(i + size - 1, j)
                + distanciaEntreVertices(i - 1, i + size);
                }
    else if (j > i){
        a_subtrair = distanciaEntreVertices(i - 1, i) + distanciaEntreVertices(i + size - 1, i + size)
                   + distanciaEntreVertices(j + size - 1, j + size);

        a_somar = distanciaEntreVertices(i - 1, i + size) + distanciaEntreVertices(j + size - 1, i)
                + distanciaEntreVertices(i + size - 1, j + size);
                 }

    else{
        a_somar = 0.0;
        a_subtrair = 0.0;
    }
    delta = a_somar - a_subtrair;

    return delta;

}


bool ILS:: bestImprovementOrOpt(int size){
    double bestDelta = 0.0, delta = 0.0;
    int best_i, best_j;  
    bool improved = false;    

    for (int i = 1; i < s->sequencia.size() - size; i++){
        for (int j = 1; j < s->sequencia.size() - size; j++){
            delta = calculateOrOptCost(i, j, size);

                if (delta < bestDelta){
                best_i = i;
                best_j = j;
                bestDelta = delta;
                improved = true;
                }
        }
    }

    if (improved){
        orOpt(best_i, best_j, size);
        s->valorObj += bestDelta;
    }
    return improved;      
}


void ILS:: BuscaLocal(){
    v_inteiros NL = {1, 2, 3, 4, 5};

    bool improved = false;


    while (!NL.empty())
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {

        case 1:
            improved = bestImprovementSwap();
            break;
        case 2:
            improved = bestImprovementTwoOpt();
            break;
        case 3:
            improved = bestImprovementOrOpt(1);
            break;
        case 4:
            improved = bestImprovementOrOpt(2);
            break;
        case 5:
            improved = bestImprovementOrOpt(3);
            break;
         }

        if (improved)
        
            NL = {1, 2, 3, 4, 5};
        

        else
            NL.erase(NL.begin() + n);
    }

}

void ILS:: solve(){
    srand(time(NULL));

    v_inteiros bestOfAll;
    double best_costOfAll = INFINITY;

    for (int i = 0; i < maxIter; i++){

        Construcao();

        v_inteiros best = s->sequencia;

        double best_cost = s->valorObj;

        int iterILS = 0;

        std:: cout << "Iteração:    " << i + 1 << std:: endl;
        while (iterILS <= maxIterILS)
        {
            BuscaLocal();

            if (s->valorObj < best_cost)

            {
                best = s->sequencia;
                best_cost = s->valorObj;
                iterILS = 0;
            }

            //this->sequencia = perturbacao(this->sequencia);            
            iterILS++;
        }
        if (best_cost < best_costOfAll){

            bestOfAll = best;
            best_costOfAll = best_cost;
        }
    }

    s->sequencia = bestOfAll;
    s->valorObj = best_costOfAll;
}
/*
 for (auto k: CL){
        cout << k << " ->  ";
    }
   cout <<  endl;
*/