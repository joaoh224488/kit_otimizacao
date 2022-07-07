#ifndef ILS_H
#define ILS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <time.h>
#include "Data.h"

class ILS
{
    private:
        v_inteiros sequencia;
        double valorObj;

        Data *distancias;
        int maxIter;
        int maxIterILS;
        
        
    public:
        ILS(Data *distancias, int maxIter, int maxIterILS);

        void exibirSolucao();
        double calcularValorObj(v_inteiros sequencia);

        void Construcao();

        // PARA A BUSCA LOCAL
        v_inteiros swap(int i, int j, v_inteiros sequencia);
        double calculateSwapCost(int i, int j);
        bool bestImprovementSwap();

        void BuscaLocal();

        // Perturbacao

        void perturbacao();


        //std::vector<InsertionInfo> calcularCustoInsercao(v_inteiros CL);

        // procedimento que resolve o ILS:

        void Solution();
};

// facilita para avaliar o custo das soluções

ILS :: ILS (Data *distancias, int maxIter, int maxIterILS){
    this->distancias = distancias;
    this->maxIter = maxIter;
    this->maxIterILS = maxIterILS;


    Solution();
    
}

void ILS:: exibirSolucao(){

    std:: cout << "Sequence:    ";
    for (int i = 0 ; i < this->sequencia.size() - 1 ; i++){ 
        std::cout << this->sequencia[i] + 1 << " -> ";
    }

    std::cout << this-> sequencia.back() + 1 << std::endl;

    std:: cout << "Cost:  " << calcularValorObj(sequencia) << std::endl; 
}


double ILS:: calcularValorObj(v_inteiros sequencia)
{
            double valorObj = 0.0;
            
            for(int i = 0; i < sequencia.size() - 1; i++){

           
             
                valorObj += (double) distancias->adjMatriz[sequencia[i]][sequencia[i+1]];
                
            }
            valorObj += (double) distancias->adjMatriz[sequencia.back()][sequencia[0]];

            return valorObj;
  
}


void ILS :: Construcao(){
    srand(time(NULL));              // Garante a aleatoriedade do random

    int n_vertices = this->distancias->n_vertices;

    int q_tour_inicial = n_vertices / 2;

    // SEÇÃO PARA PARTES 1 E 2 DAS ETAPAS DO MÉTODO CONSTRUÇÃO:

    std::vector <int> V;             // Todos os vértices
    std::vector <int> s1;            // Solução aleatória
    std::vector <int> CL;            // Vértices a inserir

    // PREENCHENDO V COM OS VÉRTICES:

    for (int i = 0; i < n_vertices; i++){

        V.push_back(i);                             
    }

    CL = V;

    // PROVAVELMENTE VAI TER UM FOR DAQUI PRA 
  
    // DESORDENANDO A LISTA DE CANDIDATOS
    std::random_shuffle(CL.begin(), CL.end());


    // 3, 0, 1, 5, 4, 2, 6
    
    int chs;                            // Chosen

    // GERANDO A SOLUÇÃO INICIAL(s1) E A LISTA DE CANDIDATOS (CL)


    for (int i = 0; i < q_tour_inicial; i++){
        chs = CL[i];

        s1.push_back(chs);

        CL.erase(remove(CL.begin(), CL.end(), chs), CL.end());
    }

    
   // s1.push_back(s1[0]);

    // MESMA COISA QUE A STRUCT SÓ QUE FUNCIONA
    
    //std:: vector <double> custos;
   // std:: vector <int> no_inserido;
   // std:: vector <int> aresta_removida;


    int i_menor;
    int atual, menor_custo;
    int maximo = CL.size();
    
    std::vector<std::vector<int>> arestas;
    std::vector<int> custos;
    std::vector<int> aux;



    while (!CL.empty()){

        arestas.clear();
        custos.clear();
        
        for (int i = 0; i < CL.size(); i++){
            for (int j = 0; j < s1.size() - 1; j++){
                
                aux.clear();
                    
                atual += distancias->adjMatriz[s1[j]][CL[i]];
                atual += distancias->adjMatriz[s1[j + 1]][CL[i]];
                atual -= distancias->adjMatriz[s1[j]][s1[j + 1]];

                aux.push_back(j);
                aux.push_back(CL[i]);
                aux.push_back(j + 1);

                arestas.push_back(aux);
                custos.push_back(atual);

                atual = 0;
            }

            int cont = 0;

            for (int custo : custos){
                if (cont == 0){
                    menor_custo = custo;
                    i_menor = cont;
                }

                if (menor_custo > custo){
                menor_custo = custo;
                i_menor = cont;
                }

                cont++;

            }
            
        }
    
        s1.insert(s1.begin() + arestas[i_menor][0], arestas[i_menor][1]);

        int a_excluir;

        for (int i = 0; i < CL.size(); i++){
            if (arestas[i_menor][1] == CL[i]){
                a_excluir = i;
                break;
            }
        }

        CL.erase(CL.begin() + a_excluir);

    }

    s1.push_back(s1[0]);

    this->sequencia = s1;

    } 

 v_inteiros ILS:: swap(int primeiro_indice, int segundo_indice, v_inteiros seq){
    
    int aux;

    aux = seq[primeiro_indice];

    seq[primeiro_indice] = seq[segundo_indice];

    seq[segundo_indice] = aux;

    return seq;

}

double ILS :: calculateSwapCost(int primeiro_indice, int segundo_indice){

    v_inteiros teste = this->sequencia;

    double antes_swap, depois_swap, delta;

    antes_swap = calcularValorObj(teste);

    teste = swap(primeiro_indice, segundo_indice, teste); 

    depois_swap = calcularValorObj(teste);

    delta = depois_swap - antes_swap;

    return delta;

}

bool ILS:: bestImprovementSwap(){
    int bestDelta = 0;
    int best_i, best_j;             // Perceba que o algoritmo nunca mexe com as pontas do vector sequencia

    for (int i = 1; i < this->sequencia.size() - 1; i++){
        for (int j = i + 1; j < this->sequencia.size() - 1; j++){
            double delta = calculateSwapCost(i, j);

            if (delta < bestDelta){
                best_i = i;
                best_j = j;
                bestDelta = delta;
            }
        }
    }

    if (bestDelta < 0){
        this->sequencia = swap(best_i, best_j, this->sequencia);
        return true;
    }

    return false;
}


void ILS:: BuscaLocal(){

     srand(time(NULL));

    std:: vector <int> NL = {1};

    bool improved = false;


    while (!NL.empty())
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {

        case 1:
            improved = bestImprovementSwap();
            break;
        
        }

        if (improved)
        
            NL = {1};
        

        else
            NL.erase(NL.begin() + n);
    
    }

}

void ILS :: perturbacao(){

    srand(time(NULL));

    int n_elem = std::ceil(sequencia.size() / 10.0);     // numero maximo de elementos

    int n1_elem, n2_elem; // numero de elementos dos vetores a gerar

    // Escolha de tamanho dos elementos
    int n1_aleatorio, n2_aleatorio;

    n1_aleatorio = rand() % n_elem;

    n2_aleatorio = rand() % n_elem;         // NÃO SE PREOCUPE. N_ELEM NUNCA VAI SER 0


    n1_elem = n1_aleatorio > 2 ? n1_aleatorio : 2;

    n2_elem = n2_aleatorio > 2 ? n2_aleatorio : 2;

    // Escolha dos elementos a serem trocados

    // AQUI VOCÊ DEVE DESCOBRIR COMO FAZ PRA INSERIR UM VETOR NO OUTRO


}

void ILS:: Solution(){

    v_inteiros bestOfAll;
    double best_costOfAll = INFINITY;

    for (int i = 0; i < maxIter; i++){

        Construcao();

        v_inteiros best = this->sequencia;

        double best_cost = calcularValorObj(sequencia);

        int iterILS = 0;

        while (iterILS <= maxIterILS)
        {
          
            BuscaLocal();

            if (calcularValorObj(sequencia) < best_cost)

            {
                best = sequencia;
                best_cost = calcularValorObj(sequencia);
                iterILS = 0;
            }

            //perturbacao();            TERMINAR PERTURBACAO

            iterILS++;

        }
        
        if (best_cost < best_costOfAll){

            bestOfAll = best;
            best_costOfAll = best_cost;
        }
    }

    sequencia = bestOfAll;

    valorObj = best_costOfAll;
    
}




#endif