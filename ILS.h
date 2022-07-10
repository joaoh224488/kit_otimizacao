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
        v_inteiros swap(v_inteiros sequencia, int primeiro_indice, int segundo_indice);
        double calculateSwapCost(int primeiro_indice, int segundo_indice);
        bool bestImprovementSwap();

        v_inteiros twoOpt(v_inteiros sequencia, int primeiro_indice, int segundo_indice);
        double calculateTwoOptCost(int primeiro_indice, int segundo_indice);
        bool bestImprovementTwoOpt();

        v_inteiros orOpt(v_inteiros sequencia, int primeiro_indice, int segundo_indice, int size);
        double calculateOrOptCost(int primeiro_indice, int segundo_indice, int size);
        bool bestImprovementOrOpt(int size);


        void BuscaLocal();

        // Perturbacao

        v_inteiros perturbacao(v_inteiros seq);


        //std::vector<InsertionInfo> calcularCustoInsercao(v_inteiros CL);

        // procedimento que resolve o ILS:

        void solve();
};

// facilita para avaliar o custo das soluções

ILS :: ILS (Data *distancias, int maxIter, int maxIterILS){
    this->distancias = distancias;
    this->maxIter = maxIter;
    this->maxIterILS = maxIterILS;


    //solve();
    
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


    int indice = 0;
    int selecionado;
    int atual;
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
                aux.push_back(j + 1);



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

    this->sequencia = s1;

}

/*
Para simplificar na hora de inserir o novo vértice no vetor, as arestas são salvas na forma:

[indice_v_a_excluir, v_a_inserir, indice_v_seguinte]

Isso acontece por motivos de diminuição da escrita do código na parte do insert(evita provocar confusão)
*/

 v_inteiros ILS:: swap( v_inteiros seq, int primeiro_indice, int segundo_indice){
    
    int aux;

    aux = seq[primeiro_indice];

    seq[primeiro_indice] = seq[segundo_indice];

    seq[segundo_indice] = aux;

    return seq;

}

double ILS :: calculateSwapCost(int primeiro_indice, int segundo_indice){

    v_inteiros teste = this->sequencia;

    double antes, depois, delta;

    antes = calcularValorObj(teste);

    teste = swap(teste, primeiro_indice, segundo_indice); 

    depois = calcularValorObj(teste);

    delta = depois - antes;

    return delta;

}

bool ILS:: bestImprovementSwap(){
    double bestDelta = 0.0;
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
        this->sequencia = swap(this->sequencia, best_i, best_j);
        return true;
    }

    return false;
}


v_inteiros ILS:: twoOpt(v_inteiros sequencia, int primeiro_indice, int segundo_indice){

    std::reverse(sequencia.begin() + primeiro_indice, sequencia.begin() + segundo_indice + 1);

    return sequencia;

}
// Inverte até onde o ponteiro apontar

double ILS:: calculateTwoOptCost(int primeiro_indice, int segundo_indice){

    v_inteiros teste = this->sequencia;

    double antes, depois, delta;

    antes = calcularValorObj(teste);

    teste = twoOpt(teste, primeiro_indice, segundo_indice); 

    depois = calcularValorObj(teste);

    delta = depois - antes;

    return delta;
}

bool ILS:: bestImprovementTwoOpt(){
    double bestDelta = 0.0;
    int best_i, best_j;             // Perceba que o algoritmo nunca mexe com as pontas do vector sequencia

    for (int i = 1; i < this->sequencia.size() - 1; i++){
        for (int j = i + 1; j < this->sequencia.size() - 1; j++){
            double delta = calculateTwoOptCost(i, j);
           

            if (delta < bestDelta){
                best_i = i;
                best_j = j;
                bestDelta = delta;
            }
        }
    }

    if (bestDelta < 0){
        this->sequencia = twoOpt(this->sequencia, best_i, best_j);
        return true;
    }

    return false;
}

v_inteiros ILS:: orOpt(v_inteiros sequencia, int primeiro_indice, int segundo_indice, int size){


    v_inteiros bloco(sequencia.begin() + primeiro_indice, sequencia.begin() + primeiro_indice + size);

    sequencia.erase(sequencia.begin() + primeiro_indice, sequencia.begin() + primeiro_indice + size);
    sequencia.insert(sequencia.begin() + segundo_indice, bloco.begin(), bloco.end());
    
    return sequencia;
}

double ILS:: calculateOrOptCost(int primeiro_indice, int segundo_indice, int size){
    v_inteiros teste = this->sequencia;

    double antes, depois, delta;

    antes = calcularValorObj(teste);

    teste = orOpt(teste, primeiro_indice, segundo_indice, size); 

    depois = calcularValorObj(teste);

    delta = depois - antes;

    return delta;

}

bool ILS:: bestImprovementOrOpt(int size){
    double bestDelta = 0.0;
    int best_i, best_j;             // Perceba que o algoritmo nunca mexe com as pontas do vector sequencia

    for (int i = 1; i < this->sequencia.size() - size; i++){
        for (int j = i + 1; j < this->sequencia.size() - size; j++){
            double delta = calculateOrOptCost(i, j, size);
           

            if (delta < bestDelta){
                best_i = i;
                best_j = j;
                bestDelta = delta;
            }
        }
    }

    if (bestDelta < 0){
        this->sequencia = orOpt(this->sequencia, best_i, best_j, size);
        return true;
    }

    return false;
}

void ILS:: BuscaLocal(){

    std:: vector <int> NL = {1, 2, 3, 4, 5};

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

v_inteiros ILS:: perturbacao(v_inteiros seq){

    int n_elem = std::ceil(seq.size() / 10.0);        // numero maximo de elementos

    int n1_elem, n2_elem; 

    n1_elem = rand() % n_elem;

    n2_elem = rand() % n_elem;         

    n1_elem = std::max(2, n1_elem);

    n2_elem = std::max(2, n2_elem);

    int escolha_1 = 0, escolha_2 = 0, fim_1 = 0, fim_2 = 0;

    while ((escolha_1 <= escolha_2 && escolha_2 <= fim_1) || (escolha_2 <= escolha_1 && escolha_1 <= fim_2) )
    {
        escolha_1 = rand() % (seq.size() - n1_elem - 1) + 1;
        n1_elem = std::max(2, rand() % n_elem);
        fim_1 = escolha_1 + n1_elem - 1;

        escolha_2 = rand() % (seq.size() - n2_elem - 1) + 1 ;
        n2_elem = std::max(2, rand() % n_elem);
        fim_2 = escolha_2 + n2_elem - 1  ;
    }
    

    v_inteiros bloco1(seq.begin() + escolha_1, seq.begin() + escolha_1 + n1_elem);
    v_inteiros bloco2(seq.begin() + escolha_2, seq.begin() + escolha_2 + n2_elem);
    seq.erase(seq.begin() + escolha_1, seq.begin() + escolha_1 + n1_elem);

    seq.insert(seq.begin() + escolha_1, bloco2.begin() , bloco2.end());

    seq.erase(seq.begin() + escolha_2, seq.begin() + escolha_2 + n2_elem);

    seq.insert(seq.begin() + escolha_2, bloco1.begin(), bloco1.end());
   
    return seq;
    
}

void ILS:: solve(){

    srand(time(NULL));

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

            this->sequencia = perturbacao(this->sequencia);            
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