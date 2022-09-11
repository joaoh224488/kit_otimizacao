#include "ILS_MLP.h"
#include "Organizers_MLP.h"

using namespace ILS_MLP_;
using namespace Organizers_MLP;
using namespace std;

ILS_MLP::ILS_MLP(Organizers_MLP::Data_MLP *distancias, int maxIter, int maxIterILS){

   this->distancias = distancias;
   this->maxIter = maxIter;
   this->maxIterILS = maxIterILS;
   this->s_final = new Solucao();
   this->sub_m = subseq_matrix(distancias->n_vertices + 1, std::vector<Subsequence>(distancias->n_vertices + 1));

 }

 ILS_MLP::ILS_MLP (Organizers_MLP::Data_MLP *distancias){
   this->distancias = distancias;
   this->maxIter = 10;
   this->maxIterILS = min(100, distancias->n_vertices);
   this->s_final = new Solucao();
   this->sub_m = subseq_matrix(distancias->n_vertices + 1, std::vector<Subsequence>(distancias->n_vertices + 1));
  
 }

Solucao ILS_MLP::Construcao(){

   srand(time(NULL));
   Solucao s1;
 
   int n_vertices = this->distancias->n_vertices;

   int q_tour_inicial = 3;

   v_inteiros V;                         
   v_inteiros CL;     

   for (int i = 2; i <= n_vertices; i++){

      V.push_back(i);                             
   }

   CL = V;

   std::random_shuffle(CL.begin(), CL.end());

   int chs;                         

   s1.sequencia.push_back(1);

   for (int i = 0; i < q_tour_inicial - 1; i++){
      chs = CL[i];

      s1.sequencia.push_back(chs);

      CL.erase(remove(CL.begin(), CL.end(), chs), CL.end());
   }   
   s1.sequencia.push_back(1);

   
   std::vector<InsertionInfo> infoCusto;

    while (!CL.empty()){
    
        infoCusto = InsertionInfo::calcularCustoInsercao(&s1, distancias->adjMatriz, CL);
        sort(infoCusto.begin(), infoCusto.end(), InsertionInfo::ordernarPorCusto);

        double alpha = (double) (rand() % 25) / 100;
        
        int selecionado = (alpha == 0) ? 0 : rand() % ((int) ceil(alpha * infoCusto.size())) ;

        inserirNaSolucao(&s1, infoCusto, selecionado);

        CL.erase(remove(CL.begin(), CL.end(), infoCusto[selecionado].noInserido), CL.end());    
    }

   UpdateAllSubseq(&s1, sub_m, distancias);
   s1.valorObj = sub_m[0][distancias->n_vertices].C;

   return s1;
}

Solucao ILS_MLP:: perturbacao(Solucao *s){

   Solucao seq;
   seq.setSequence(s->sequencia);
   seq.valorObj = s->valorObj;

   int n_lim = std::ceil(seq.get_size() / 10.0);       

   int n1_elem = 0, n2_elem = 0, ident = 0; 

   int inicio_1 = 0, inicio_2 = 0, direita_1 = 0;


   n1_elem = std::max(2, rand() % n_lim);
   inicio_1 = rand() % (seq.get_size() - n1_elem - 1) + 1;
   direita_1 = inicio_1 + n1_elem;

   if ((inicio_1 == 1) || (inicio_1 == 2)){
      n2_elem = std::max(2, rand() % (n_lim));
      inicio_2 = rand() % (seq.get_size()- n2_elem - n1_elem  - inicio_1 - 1) + direita_1;  
      

   }
   else if (direita_1 >= seq.get_size() - 2){
      n2_elem = std::max(2, rand() % (n_lim));
      inicio_2 = rand() % (seq.get_size() -  n2_elem - n1_elem 
                  - (seq.get_size() - direita_1)) + 1;   
               
   
   }
   else{
      if (rand() % 2 == 0){    

         n2_elem = std::max(2, rand() % std::min(seq.get_size() - direita_1, n_lim));
         inicio_2 = rand() % (seq.get_size() - direita_1 - n2_elem) + direita_1;

         
      }
      else{ 

         n2_elem =  std::max(2, rand() % std::min(inicio_1, n_lim)); 
         inicio_2 = rand() % (inicio_1 - n2_elem) + 1;
      
         
      }
   }

   v_inteiros bloco1(seq.sequencia.begin() + inicio_1, seq.sequencia.begin() + inicio_1 + n1_elem);
   v_inteiros bloco2(seq.sequencia.begin() + inicio_2, seq.sequencia.begin() + inicio_2 + n2_elem);


   if (inicio_1 < inicio_2)
   {
      seq.sequencia.erase(seq.sequencia.begin() + inicio_2, seq.sequencia.begin() + inicio_2 + n2_elem);
      seq.sequencia.insert(seq.sequencia.begin() + inicio_2, bloco1.begin(), bloco1.end());
      seq.sequencia.erase(seq.sequencia.begin() + inicio_1, seq.sequencia.begin() + inicio_1 + n1_elem);
      seq.sequencia.insert(seq.sequencia.begin() + inicio_1, bloco2.begin(), bloco2.end());
   }
   else
   {
      seq.sequencia.erase(seq.sequencia.begin() + inicio_1, seq.sequencia.begin() + inicio_1 + n1_elem);
      seq.sequencia.insert(seq.sequencia.begin() + inicio_1, bloco2.begin(), bloco2.end());
      seq.sequencia.erase(seq.sequencia.begin() + inicio_2, seq.sequencia.begin() + inicio_2 + n2_elem);
      seq.sequencia.insert(seq.sequencia.begin() + inicio_2, bloco1.begin(), bloco1.end());
   }
   UpdateAllSubseq(&seq, sub_m, distancias);
   seq.valorObj = sub_m[0][distancias->n_vertices].C;

   return seq;

}



bool ILS_MLP::bestImprovementSwap(Solucao *s){


   double bestDelta = s->valorObj, delta = 0.0;
   int best_i, best_j;  
   bool improved = false;

   for (int i = 1; i < s->sequencia.size() - 2; i++){
      for (int j = i + 1; j < s->sequencia.size() -1; j++){
         delta = calculateSwapCost(s , i, j);

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
      s->valorObj = bestDelta;
   }

   return improved;

}


double ILS_MLP::calculateSwapCost(Solucao *s, int i, int j){
   int n = distancias->n_vertices;
   Subsequence sigma_1, sigma_2, sigma_3, complete_sub;

   if (j == i + 1){

   sigma_1 = Concatenate(sub_m[0][i - 1], sub_m[j][j], distancias);

   sigma_2 = Concatenate(sigma_1, sub_m[i][i], distancias);

   complete_sub = Concatenate(sigma_2, sub_m[j + 1][n], distancias);

   }
   else{

      sigma_1 = Concatenate(sub_m[0][i - 1], sub_m[j][j], distancias);

      sigma_2 = Concatenate(sigma_1, sub_m[i + 1][j - 1], distancias);

      sigma_3 = Concatenate(sigma_2, sub_m[i][i], distancias);

      complete_sub = Concatenate(sigma_3, sub_m[j + 1][n], distancias);
   }

   return complete_sub.C;
}

bool ILS_MLP::bestImprovementTwoOpt(Solucao *s){
    double bestDelta = s->valorObj;
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
        s->valorObj = bestDelta;
    }

    return improved;
}


double ILS_MLP:: calculateTwoOptCost(Solucao *s, int i, int j){

   int n = distancias->n_vertices;

   Subsequence sigma_1, complete_sub;

   sigma_1 = Concatenate(sub_m[0][i-1], sub_m[j][i], distancias);

   complete_sub = Concatenate(sigma_1, sub_m[j + 1][n], distancias);

   return complete_sub.C;
}

void ILS_MLP::orOpt(Solucao * s, int i, int j, int size){
   v_inteiros bloco(s->sequencia.begin() + i, s->sequencia.begin() + i + size);

   s->sequencia.erase(s->sequencia.begin() + i, s->sequencia.begin() + i + size);
   s->sequencia.insert(s->sequencia.begin() + j, bloco.begin(), bloco.end());
}


bool ILS_MLP::bestImprovementOrOpt(Solucao *s, int size){
   double bestDelta = s->valorObj, delta = 0.0;
    int best_i, best_j;  
    bool improved = false;    

    for (int i = 1; i < s->sequencia.size() - size; i++){
        for (int j = 1; j < s->sequencia.size() - size; j++){
            if (i == j)
               continue;


            delta = calculateOrOptCost(s, i, j, size) ;

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
        s->valorObj = bestDelta;
    }
    return improved;      
}

double ILS_MLP::calculateOrOptCost(Solucao *s, int i, int j, int size){
   int n = distancias->n_vertices;

   Subsequence sigma_1, sigma_2, complete_sub;

   if (j > i){

      sigma_1 = Concatenate(sub_m[0][i-1], sub_m[i + size][j + size - 1], distancias);

      sigma_2 = Concatenate(sigma_1, sub_m[i][i + size - 1], distancias);

      complete_sub = Concatenate(sigma_2, sub_m[j + size][n], distancias);

   }
   else if (i > j){

      sigma_1 = Concatenate(sub_m[0][j-1], sub_m[i][i + size - 1], distancias);

      sigma_2 = Concatenate(sigma_1, sub_m[j][i - 1], distancias);

      complete_sub = Concatenate(sigma_2, sub_m[i + size][n], distancias);
   }
   else{
      return 0;
   }

   return complete_sub.C;



}

void ILS_MLP:: BuscaLocal(Solucao *s){

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
      {
         NL = {1, 2, 3, 4, 5};
         UpdateAllSubseq(s, sub_m, distancias);
      }

      else
         NL.erase(NL.begin() + n);
   }
   

}

void ILS_MLP:: solve(){
   auto start = std::chrono::high_resolution_clock::now();
    srand(time(NULL));

    Solucao bestOfAll;

    for (int i = 0; i < maxIter; i++){

        Solucao s = Construcao();

        Solucao best = s;

         if (i == 0){
            bestOfAll.valorObj = s.valorObj;
        }

        int iterILS = 0;
     
        while (iterILS <= maxIterILS)
        {
            BuscaLocal(&s);
            if (improve(best.valorObj, s.valorObj))

            {
                best = s;
         
                iterILS = 0;
            }
            
            s = perturbacao(&best);

            if (s.valorObj < 0){
                s.exibir();
                break;
            }
   
            iterILS++;
        }
        if (improve(bestOfAll.valorObj, best.valorObj))
        { 
            bestOfAll = best;     
         }
    }

   this->s_final->setSequence(bestOfAll.sequencia);
   this->s_final->valorObj = bestOfAll.valorObj;

   auto end = std::chrono::high_resolution_clock::now();

   std::chrono::duration<double, std::milli> float_ms = end - start;
   std::cout << "Tempo de execução:  " << float_ms.count() / 1000.0000000000000 << " segundos" << std::endl;
}

void ILS_MLP::test(){

   Solucao s = Construcao();
   s.exibir();


   /*
   cout << endl;

   cout << "Teste:   " << calculateOrOptCost(&s, 6, 7, 1) << endl;
   orOpt(&s, 6, 7, 1);
   s.calcularValorObj(distancias);
   UpdateAllSubseq(&s, sub_m, distancias);

   s.exibir();

   cout << "Correto:    " << sub_m[0][distancias->n_vertices].C << endl;
   */
   //s.exibir();

}