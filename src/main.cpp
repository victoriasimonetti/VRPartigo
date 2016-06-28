#include "../include/Instance.h"
#include "../include/nodes.h"
#include "../include/route.h"
#include "../include/solution.h"
#include "../include/neighborhood.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>   
#include <math.h> 
using namespace std;

/* Garantimos em uma solução que todos os clientes são atendidos e que não se pode "voltar no tempo". As restrições de janela de tempo e capacidade podem ser desrespeitadas e são contabilizadas na função objetivo com peso 1000. */

Instance *instance = NULL;

//Busca Tabu
Solution BuscaTabu(Solution s){

	Solution bestSolution(instance);
	bestSolution = s;
	Solution currentSolution(instance);
	currentSolution = s;
	int iter = 0;
	int melhorIter = 0;
	int BTmax = 0; //numero max de iteraçõe sem melhora << ajustar parametro
	vector<Solution> listaTabu; //tamanho 100.000
	double aspiracao;

	while(iter - melhorIter <= BTmax){
		iter++;
		currentSolution  = nb->interRoutes(s); //gera vizinho inter-rota *** verificar se n ta na lista tabu ou se atende ao criterio de aspiracao
		//atualiza a lista tabu

		if(currentSolution.getTotalCost() < bestSolution.getTotalCost()){
			bestSolution = currentSolution;
			melhorIter = iter;
		}
		//atualiza funcao de aspiracao

	}

	return bestSolution;
}



//Construtivo Aleatório
Solution construction(){
    
    Solution initialSolution(instance);
	int veiculos = instance->getNumVehicles();
	
	for(int c=1; c<instance->getNumNodes(); c++){
	    int v = rand() % veiculos;
	    int indice =0;
	    while(initialSolution.getRoute(v)->getForward(indice) != -1){ //Busca último elemento adicionado, então força sair do depósito.
	        indice = initialSolution.getRoute(v)->getForward(indice);
	    }  
	
	    initialSolution.getRoute(v)->setForward(indice, c); 
	    initialSolution.getRoute(v)->setBackward(c, indice); 
	}
	
	//força voltar para o depósito.
	for(int r=0; r<instance->getNumVehicles(); r++){
	    int indice =0;
	    while(initialSolution.getRoute(r)->getForward(indice) != -1){ //Busca último elemento adicionado
	        indice = initialSolution.getRoute(r)->getForward(indice);
	    }  
		initialSolution.getRoute(r)->setForward(indice, instance->getNumNodes()); 
	    initialSolution.getRoute(r)->setBackward(instance->getNumNodes(), indice); 
	    initialSolution.calculateTimeServiceAndFaults(indice, instance->getNumNodes(), r);
	}

	initialSolution.forcaBrutaRecalculaSolution();
	//Adcionar a função objetivo os custo das infactibilidades de capacidade janela de tempo.
    cout << endl << "----------------- SOLUCAO INICIAL ---------------------------" << endl << endl;
	initialSolution.printSolution();   
	return initialSolution;    	
}


int main(int argc, char** argv){

    time_t seconds;
	time(&seconds);
    //srand((unsigned int) seconds);
    srand((unsigned int) 10);
    instance = new Instance(argv[1]); //Parâmetro 1: nome do arquivo de instância para ser lido.

    instance->readFile();
	//instance->createGLPKinstanceFile();
	Solution s = construction();

	//double t0 = initialTemperature(1.2, 0.95, s, instance->getNumNodes(), 2);

	s = simulatedAnnealing(0.998, s, 10000, instance->getNumNodes()); //double alfa, Solution s, double t0, int maxIterTemp
    s.forcaBrutaRecalculaSolution();
	cout << endl << "----------------- SOLUCAO FINAL ---------------------------" << endl << endl;

	s.printSolution();

	return 0;
}

