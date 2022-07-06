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
        
    public:

        ILS(Data *distancias);

        // Para a copia que o bestImprovement faz
        ILS(Data *distancias, v_inteiros exemplo); 

        void exibirSolucao();
        double calcularValorObj();

        void Construcao();

        // PARA A BUSCA LOCAL
        void swap(int i, int j);
        double calculateSwapCost(int i, int j);
        bool bestImprovementSwap();

        void BuscaLocal();

        // Perturbacao

        void perturbacao();


        //std::vector<InsertionInfo> calcularCustoInsercao(v_inteiros CL);
};

// facilita para avaliar o custo das soluções

ILS::ILS(Data *distancias, v_inteiros exemplo)
{
    this->distancias = distancias;                 

    this->sequencia = exemplo;
}

ILS :: ILS (Data *distancias){
    this->distancias = distancias;
}

void ILS:: exibirSolucao(){
    for (int i = 0 ; i < this->sequencia.size() - 1 ; i++){ 
        std::cout << this->sequencia[i] + 1 << " -> ";
    }

    std::cout << this-> sequencia.back() + 1 << std::endl;
}


double ILS:: calcularValorObj()
{
            this->valorObj = 0.0;
            
            for(int i = 0; i < this->sequencia.size() - 1; i++){

           
             
                this->valorObj += (double) distancias->adjMatriz[this->sequencia[i]][this->sequencia[i+1]];
                
            }
            this->valorObj += (double) distancias->adjMatriz[this->sequencia.back()][this->sequencia[0]];

            return this->valorObj;
  
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


    int i, j, i_menor;
    int custo_atual, menor_custo;
    int maximo = CL.size();
    
    std::vector<std::vector<int>> arestas;
    std::vector<int> custo_aresta;
    std::vector<int> aux;



    while (!CL.empty()){

        arestas.clear();
        custo_aresta.clear();
        
        for (i = 0; i < CL.size(); i++){
            for (j = 0; j < s1.size() - 1; j++){
                
                aux.clear();
                    
                custo_atual += distancias->adjMatriz[s1[j]][CL[i]];
                custo_atual += distancias->adjMatriz[s1[j + 1]][CL[i]];
                custo_atual -= distancias->adjMatriz[s1[j]][s1[j + 1]];

                aux.push_back(j);
                aux.push_back(CL[i]);
                aux.push_back(j + 1);

                arestas.push_back(aux);
                custo_aresta.push_back(custo_atual);

                custo_atual = 0;
            }

            int cont = 0;

            for (int custo : custo_aresta){
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

        int indice_para_excluir;

        for (i = 0; i < CL.size(); i++){
            if (arestas[i_menor][1] == CL[i]){
                indice_para_excluir = i;
                break;
            }
        }

        CL.erase(CL.begin() + indice_para_excluir);

    }

    s1.push_back(s1[0]);

    this->sequencia = s1;

    } 

void ILS:: swap(int primeiro_indice, int segundo_indice){      // VERIFICAR DEPOIS SE HÁ PROBLEMA NO SWAP

    int aux;                                                    // testar no arquivo aux

    aux = this->sequencia[primeiro_indice];

    this->sequencia[primeiro_indice] = this->sequencia[segundo_indice];

    this->sequencia[segundo_indice] = aux;

}

double ILS :: calculateSwapCost(int primeiro_indice, int segundo_indice){

    ILS teste(this->distancias, this->sequencia);

    double antes_swap, depois_swap, delta;

    antes_swap = teste.calcularValorObj();

    teste.swap(primeiro_indice, segundo_indice);

    depois_swap = teste.calcularValorObj();

    delta = depois_swap - antes_swap;

    return delta;

}

bool ILS:: bestImprovementSwap(){
    int bestDelta = 0;
    int best_i, best_j;             // Perceba que o algoritmo nunca mexe com as pontas do vector sequencia

    for (int i = 1; i < sequencia.size() - 1; i++){
        for (int j = i + 1; j < sequencia.size() - 1; j++){
            double delta = calculateSwapCost(i, j);

            if (delta < bestDelta){
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0){
        swap(best_i, best_j);
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



/* 2 opt


SOLUÇÃO INICIAL:

7 - 6 - 4 - 10 - 3 - 9 - 2 - 8 - 5 - 1   

SOLUÇÃO ALTERADA PELA 2-OPT:

7 - 6 - 4 - 10 - 3 - 9 - 1 - 5 - 8 - 2  


SOLUÇÃO INICIAL:

5 - 1 - 7 - 6 - 4 - 10 - 3 - 9 - 2 - 8

DEPOIS DO OR - OPT - 2:

5 - 1 - 7 - 6 - 3 - 9 - 2 - 8 - 10 - 4

DEPOIS DO OR - OPT - 3:

5 - 1 - 7 - 6 - 9 - 2 - 8 - 3 - 10 - 4


s' = m(s)
*/












#endif