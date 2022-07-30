#include "ILS.h"
#include <limits>
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
    this->s_final = new Solucao();

}

ILS :: ILS(Data *distancias){

    this->distancias = distancias;
    this->maxIter = 50;
    this->maxIterILS = distancias->n_vertices >= 150 ? distancias->n_vertices / 2 : distancias->n_vertices;
    this->s_final = new Solucao();
}


double ILS :: get_current_cost(){
    return this->s_final->valorObj;
}
double ILS:: distanciaEntreVertices(Solucao *s, int a, int b){
    return distancias->adjMatriz[s->valorNaPos(a)][s->valorNaPos(b)];
}

v_inteiros ILS :: get_current_vector(){
    return this->s_final->sequencia;
}

void ILS:: exibirSolucao(){
    //this->s->calcularValorObj(distancias);
    s_final->exibirSequencia();
}

Solucao ILS:: Construcao(){

    Solucao s1;
 
    int n_vertices = this->distancias->n_vertices;

    int q_tour_inicial = 3;

    v_inteiros V;                         
    v_inteiros CL;     

    for (int i = 1; i <= n_vertices; i++){

        V.push_back(i);                             
    }

    CL = V;

    std::random_shuffle(CL.begin(), CL.end());

    int chs;                         


    for (int i = 0; i < q_tour_inicial; i++){
        chs = CL[i];

        s1.sequencia.push_back(chs);

        CL.erase(remove(CL.begin(), CL.end(), chs), CL.end());
    }   

    s1.sequencia.push_back(s1.sequencia[0]);


    while (!CL.empty()){
    
        vector<InsertionInfo> infoCusto;
    
        infoCusto = InsertionInfo::calcularCustoInsercao(&s1, distancias->adjMatriz, CL);
        sort(infoCusto.begin(), infoCusto.end(), InsertionInfo::ordernarPorCusto);

        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * infoCusto.size()));

        inserirNaSolucao(&s1, infoCusto, selecionado);

        CL.erase(remove(CL.begin(), CL.end(), infoCusto[selecionado].noInserido), CL.end());    

    
    }

    s1.calcularValorObj(distancias);

    return s1;

}

void ILS:: swap(Solucao *s, int i, int j){

    int aux = s->valorNaPos(i);

    s->sequencia[i] = s->sequencia[j];

    s->sequencia[j] = aux;
}

double ILS:: calculateSwapCost(Solucao *s, int i, int j){
    double  a_subtrair, a_somar, delta;

    if ((j == i + 1)){
        
        a_subtrair = distanciaEntreVertices(s, i - 1, i) + distanciaEntreVertices(s, j, j+ 1);
        a_somar = distanciaEntreVertices(s, i -1, j) + distanciaEntreVertices(s, i, j + 1 );
        
    }
    else{
         a_subtrair = distanciaEntreVertices(s, i - 1, i) + distanciaEntreVertices(s, i, i + 1) 
                    + distanciaEntreVertices(s, j -1 , j) + distanciaEntreVertices(s, j, j+1);

        a_somar = distanciaEntreVertices(s, i - 1, j) + distanciaEntreVertices(s, j, i + 1)
                + distanciaEntreVertices(s, j - 1, i) + distanciaEntreVertices(s, i, j + 1);
    }

    delta = a_somar - a_subtrair;

    return delta;
}

bool ILS:: bestImprovementSwap(Solucao *s){
    double bestDelta = 0.0, delta = 0.0;
    int best_i, best_j;  
    bool improved = false;

    for (int i = 1; i < s->sequencia.size() - 2; i++){
        for (int j = i + 1; j < s->sequencia.size() -1; j++){
            delta = calculateSwapCost(s, i, j);

            if (improve(bestDelta, delta)){
                bestDelta = delta;
                best_i = i;
                best_j = j;
                improved = true;
            }
        }
    }
    if (improved){
        swap(s, best_i, best_j);
        s->valorObj += bestDelta;
    }

    return improved;
}

void ILS:: twoOpt(Solucao *s, int i, int j){
    reverse(s->sequencia.begin() + i, s->sequencia.begin() + j + 1);
}


double ILS:: calculateTwoOptCost(Solucao *s, int i, int j){
    double a_subtrair, a_somar, delta;

    a_subtrair = distanciaEntreVertices(s, i - 1, i) + distanciaEntreVertices(s, j, j + 1);
    a_somar = distanciaEntreVertices(s, i - 1, j) + distanciaEntreVertices(s, i, j + 1);

    delta = a_somar - a_subtrair;

    return delta;
}

bool ILS:: bestImprovementTwoOpt(Solucao *s){
    double bestDelta = 0.0;
    double delta = 0.0;
    int best_i, best_j;  
    bool improved = false;           

    for (int i = 1; i < s->sequencia.size() - 2; i++){
        for (int j = i + 1; j < s->sequencia.size() - 1; j++){

            delta = calculateTwoOptCost(s, i, j);
        
            if (improve(bestDelta, delta)){
                best_i = i;
                best_j = j;
                bestDelta = delta;
                improved = true;
            }
        }
    }
    if (improved){
        twoOpt(s, best_i, best_j);
        s->valorObj += bestDelta;
    }

    return improved;
}


void ILS:: orOpt(Solucao *s, int i, int j, int size){
    v_inteiros bloco(s->sequencia.begin() + i, s->sequencia.begin() + i + size);

    s->sequencia.erase(s->sequencia.begin() + i, s->sequencia.begin() + i + size);
    s->sequencia.insert(s->sequencia.begin() + j, bloco.begin(), bloco.end());

}


double ILS:: calculateOrOptCost(Solucao *s, int i, int j, int size){
    double a_subtrair, a_somar, delta;

    if (i > j){
        a_subtrair = distanciaEntreVertices(s, j - 1, j) + distanciaEntreVertices(s, i - 1, i)
                   + distanciaEntreVertices(s, i + size - 1, i + size);

        a_somar = distanciaEntreVertices(s, j - 1, i) + distanciaEntreVertices(s, i + size - 1, j)
                + distanciaEntreVertices(s, i - 1, i + size);
                }
    else if (j > i){
        a_subtrair = distanciaEntreVertices(s, i - 1, i) + distanciaEntreVertices(s, i + size - 1, i + size)
                   + distanciaEntreVertices(s, j + size - 1, j + size);

        a_somar = distanciaEntreVertices(s, i - 1, i + size) + distanciaEntreVertices(s, j + size - 1, i)
                + distanciaEntreVertices(s, i + size - 1, j + size);
                 }

    else{
        a_somar = 0.0;
        a_subtrair = 0.0;
    }
    delta = a_somar - a_subtrair;

    return delta;

}


bool ILS:: bestImprovementOrOpt(Solucao *s, int size){
    double bestDelta = 0.0, delta = 0.0;
    int best_i, best_j;  
    bool improved = false;    

    for (int i = 1; i < s->sequencia.size() - size; i++){
        for (int j = 1; j < s->sequencia.size() - size; j++){
            delta = calculateOrOptCost(s, i, j, size);

                if (improve(bestDelta, delta)){
                best_i = i;
                best_j = j;
                bestDelta = delta;
                improved = true;
                }
        }
    }

    if (improved){
        orOpt(s, best_i, best_j, size);
        s->valorObj += bestDelta;
    }
    return improved;      
}


void ILS:: BuscaLocal(Solucao *s){
    v_inteiros NL = {1, 2, 3, 4, 5};

    bool improved = false;


    while (!NL.empty())
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {

        case 1:
            improved = bestImprovementSwap(s);
            break;
        case 2:
            improved = bestImprovementTwoOpt(s);
            break;
        case 3:
            improved = bestImprovementOrOpt(s, 1);
            break;
        case 4:
            improved = bestImprovementOrOpt(s, 2);
            break;
        case 5:
            improved = bestImprovementOrOpt(s,3);
            break;
         }

        if (improved)
        
            NL = {1, 2, 3, 4, 5};
        

        else
            NL.erase(NL.begin() + n);
    }

}

Solucao ILS:: perturbacao(Solucao *s){

    Solucao seq;
    seq.setSequence(s->sequencia);
    seq.valorObj = s->valorObj;
  
    int n_elem = std::ceil(seq.sequencia.size() / 10.0);       

    int n1_elem, n2_elem; 

    int escolha_1 = 0, escolha_2 = 0, fim_1 = 0, fim_2 = 0;

    while ((escolha_1 <= escolha_2 && escolha_2 <= fim_1) || (escolha_2 <= escolha_1 && escolha_1 <= fim_2) )
    {
        n1_elem = std::max(2, rand() % n_elem);
        escolha_1 = rand() % (seq.sequencia.size() - n1_elem - 1) + 1;
        fim_1 = escolha_1 + n1_elem - 1;

        n2_elem = std::max(2, rand() % n_elem);
        escolha_2 = rand() % (seq.sequencia.size() - n2_elem - 1) + 1 ;
        fim_2 = escolha_2 + n2_elem - 1  ;
    }

    v_inteiros bloco1(seq.sequencia.begin() + escolha_1, seq.sequencia.begin() + escolha_1 + n1_elem);
    v_inteiros bloco2(seq.sequencia.begin() + escolha_2, seq.sequencia.begin() + escolha_2 + n2_elem);

    seq.valorObj += calculatePerturbacaoCost(&seq, escolha_1, n1_elem, escolha_2, n2_elem);

    if (escolha_1 < escolha_2)
    {
        seq.sequencia.erase(seq.sequencia.begin() + escolha_2, seq.sequencia.begin() + escolha_2 + n2_elem);
        seq.sequencia.insert(seq.sequencia.begin() + escolha_2, bloco1.begin(), bloco1.end());
        seq.sequencia.erase(seq.sequencia.begin() + escolha_1, seq.sequencia.begin() + escolha_1 + n1_elem);
        seq.sequencia.insert(seq.sequencia.begin() + escolha_1, bloco2.begin(), bloco2.end());
    }
    else
    {
        seq.sequencia.erase(seq.sequencia.begin() + escolha_1, seq.sequencia.begin() + escolha_1 + n1_elem);
        seq.sequencia.insert(seq.sequencia.begin() + escolha_1, bloco2.begin(), bloco2.end());
        seq.sequencia.erase(seq.sequencia.begin() + escolha_2, seq.sequencia.begin() + escolha_2 + n2_elem);
        seq.sequencia.insert(seq.sequencia.begin() + escolha_2, bloco1.begin(), bloco1.end());
    }

    return seq;

}



double ILS:: calculatePerturbacaoCost(Solucao *s, int i, int size_i, int j, int size_j){

    double a_subtrair, a_somar, delta;

    if (i + size_i == j){

        a_subtrair = distanciaEntreVertices(s, i - 1, i) + distanciaEntreVertices(s, i + size_i - 1, j)
                   + distanciaEntreVertices(s, j + size_j - 1, j + size_j);
        
        a_somar = distanciaEntreVertices(s, i - 1, j) + distanciaEntreVertices(s, j + size_j - 1, i) 
                + distanciaEntreVertices(s, i + size_i - 1, j + size_j);
    }
    else if (j + size_j == i){

        a_subtrair = distanciaEntreVertices(s, j - 1, j) + distanciaEntreVertices(s, j + size_j - 1, i)
                   + distanciaEntreVertices(s, i + size_i - 1, i + size_i);
        
        a_somar = distanciaEntreVertices(s, j - 1, i) + distanciaEntreVertices(s, i + size_i - 1, j)
                + distanciaEntreVertices(s, j + size_j - 1, i + size_i);
    }

    else{
        a_subtrair = distanciaEntreVertices(s, i - 1, i) + distanciaEntreVertices(s, i + size_i - 1, i + size_i)
                + distanciaEntreVertices(s, j - 1, j) + distanciaEntreVertices(s, j + size_j - 1, j + size_j);
        
        a_somar = distanciaEntreVertices(s, i - 1, j) + distanciaEntreVertices(s, j + size_j - 1, i + size_i)
                + distanciaEntreVertices(s, j - 1, i) + distanciaEntreVertices(s, i + size_i - 1, j + size_j);
    }

    delta = a_somar - a_subtrair;

    return delta;
}

void ILS:: solve(){
    srand(time(NULL));
    int q_nos = distancias->n_vertices + 1;

    Solucao bestOfAll;

    for (int i = 0; i < maxIter; i++){

        Solucao s = Construcao();

        Solucao best = s;

          if (i == 0){
            bestOfAll.valorObj = s.valorObj;
        }
        

        int iterILS = 0;

        cout << "Iteração:    " << i + 1 << endl;

        cout << "Construção com ValorObj:   " << s.valorObj << endl;
     
        while (iterILS <= maxIterILS)
        {
            BuscaLocal(&s);

            if (improve(best.valorObj, s.valorObj))

            {
                best = s;
                cout << "Rolou na Construção  " << i << "  na Busca  " << iterILS << endl;
                cout << "Melhor valor: " << best.valorObj << endl;
                iterILS = 0;
            }
            
            s = perturbacao(&best);
   
            iterILS++;
        }
        if (improve(bestOfAll.valorObj, best.valorObj))
        { 
            bestOfAll = best;
            cout << "ATUALIZOU O ALL" << endl;
         }
    }
   this->s_final->setSequence(bestOfAll.sequencia);
   this->s_final->valorObj = bestOfAll.valorObj;
}
