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
    maxIterILS = distancias->n_vertices >= 150 ? distancias->n_vertices / 2 : distancias->n_vertices;

    ILS_MLP(distancias, 50, maxIterILS);
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

        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * infoCusto.size()));

        inserirNaSolucao(&s1, infoCusto, selecionado);

        CL.erase(remove(CL.begin(), CL.end(), infoCusto[selecionado].noInserido), CL.end());    
    }

    s1.calcularValorObj(distancias);

    return s1;
}


/*bool ILS_MLP::bestImprovementSwap(Solucao *s, subseq_matrix &m){

}
*/

void ILS_MLP:: BuscaLocal(Solucao *s, subseq_matrix &m){

   v_inteiros NL = {1, 2, 3, 4, 5};

   bool improved = false;

   /*
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
   */

}


void ILS_MLP:: solve(){
   Solucao *s = new Solucao();

   *s = Construcao();

   s->exibir();

   UpdateAllSubseq(s, sub_m, distancias);

   this->s_final->setSequence(s->sequencia);
   this->s_final->valorObj = s->valorObj;
}