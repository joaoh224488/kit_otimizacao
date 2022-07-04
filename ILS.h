#ifndef ILS_H
#define ILS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include "Data.h"

class ILS
{
    v_inteiros sequencia;
    double valorObj;
    Data *distancias;
    
public:

    ILS(Data *distancias);
    ILS(Data *distancias, v_inteiros exemplo); // EXCLUIR HOJE

    void exibirSolucao();
    double calcularValorObj();

    void Construcao();

    // PARA A BUSCA LOCAL
    void swap(int i, int j);
    double calculateSwapCost(int i, int j);

    //std::vector<InsertionInfo> calcularCustoInsercao(v_inteiros CL);


};

ILS::ILS(Data *distancias, v_inteiros exemplo)
{
    this->distancias = distancias;                  // EXCLUIR HOJE

    this->sequencia = exemplo;
}

ILS :: ILS (Data *distancias){
    this->distancias = distancias;
}

void ILS:: exibirSolucao(){
    for (int i = 0 ; i < this->sequencia.size() -1 ; i++){ 
        std::cout << this->sequencia[i] + 1 << " -> ";
    }

    std::cout << this-> sequencia.back() + 1 << std::endl;
            

}


double ILS:: calcularValorObj()
{
            this->valorObj = 0.0;
            
            for(int i = 0; i < this->sequencia.size() - 1; i++){
           
             
                this->valorObj += (double) distancias->adjMatriz[this->sequencia[i]][this->sequencia[i+1]];
                ;
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



    for (int k = 0; k < maximo; k++){

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













#endif